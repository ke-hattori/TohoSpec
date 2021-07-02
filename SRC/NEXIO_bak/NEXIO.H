#if !defined( _NEXIO_H_ )
#define _NEXIO_H_

#pragma once

#include <nexmisc.hxx>
#include <nexif.hxx>

#ifdef _DEBUG
#pragma comment(lib, "dtnsnexif.lib")
#else
#pragma comment(lib, "tnsnexif.lib")
#endif

namespace nano
{

template <class T, int N, int F = 0>
class basic_dio : public T
{
public:
	basic_dio (const char* _s) : value(_s, N)
	{
		value.attach();

		for ( int n = F; n < N; n++ ) port[n - F] = n;
		Buf = (unsigned char*) value.pptr();
		if ( !value.is_exists() ) {
			memset(value, 0, N);
		}
		memset(prev, 0, N);
		memset(mask, 0xFF, N);
	}
	T* operator& () { return (T*) this; }
	operator T*  () { return (T*) this; }

	inline void execute (unsigned long ( FAR PASCAL* _f )( HANDLE, T FAR* ), HANDLE _h) { if ( _h != NULL ) _f(_h, (T*) this); }
	inline int size (void) const { return PortNum; }
	// マスキング処理
	inline void masking (void)
	{
		basic_device* p = (basic_device*) value.pptr();
		for ( int n = 0; n < size(); n++ )
			(p++)->byte &= mask[n];
	}
	inline void copy (void) { memcpy(prev, value.pptr(), size()); }
	inline bool is_update(void) const { return memcmp(prev, value.pptr(), size()) != 0; }

	// ログストリーム
	friend ostream& operator << (ostream& os, basic_dio& rhs)
	{
		basic_device* p = (basic_device*) rhs.value.pptr();
		os << *p;
		for ( int n = 1; n < rhs.size(); n++ ) os << SPACECHAR << *(++p);

		return os;
	}

	unsigned short port[N];
	basic_shared   value;
	unsigned char  mask[N];

private:
	unsigned char prev[N];
};

class cDioBaseThread : public cDioInterface, thread
{
public:
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexio.dll ---------- { ---------- */
//	  cDioBaseThread ()
//		  : m_handle(NULL), m_module(NULL), m_lfs(NEXIO_LOGFILE_NAME),
//			m_dinp(EQPIOINP_KERNELSTR), m_dout(EQPIOOUT_KERNELSTR) {}
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexio.dll ---------- 			 */
	cDioBaseThread ()
		: m_handle(NULL), m_module(NULL), m_lfs(NEXIO_LOGFILE_NAME),
		  m_dinp(EQPIOINP_KERNELSTR), m_dout(EQPIOOUT_KERNELSTR) {
			TCHAR l_tszProcessFPath[_MAX_PATH];	/* 呼出しプロセスのフルパス*/
			::GetModuleFileName(0, l_tszProcessFPath, sizeof(l_tszProcessFPath));
			TCHAR l_tszDrive[_MAX_DRIVE];
			TCHAR l_tszDir[_MAX_DIR];
			_tsplitpath(l_tszProcessFPath, l_tszDrive, l_tszDir, 0, 0);
			TCHAR l_tszProcDir[_MAX_PATH];
			_stprintf(l_tszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
			if (0 != _tcslen(l_tszProcDir)) {
				if (_T('\\') != l_tszProcDir[_tcslen(l_tszProcDir) - 1]) {
					_tcscat(l_tszProcDir, _T("\\"));
				}
			}
			TCHAR l_tszNexioLogFPath[_MAX_PATH];
			_stprintf(l_tszNexioLogFPath, _T("%s") _T(NEXIO_LOGFILE_NAME), l_tszProcDir);
			m_lfs.create(l_tszNexioLogFPath);
	}
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexio.dll ---------- } ---------- */

	virtual ~ cDioBaseThread ()
	{
		DioClose(m_handle);
		if ( IsValidModule() ) {
			FreeLibrary(m_module);
			m_module = NULL;
		}
	}

	BOOL IsValidModule (void) const { return m_module != NULL; }
	HMODULE GetModuleHandle (void) const { return m_module; }
	BOOL Initialize (void)
	{
		// 設定読み込み
		nexiniGetParameter(&m_params);

		basic_interface::copymemory(&m_config, &m_params.Dio);

		// エラーメッセージリソース読み込み
		m_module = NexResourceLoadLibrary(m_params.System.Language);

		if ( !IsValidModule() ) return FALSE;

		TCHAR szTitleMsg[256] = {0}, szErrorMsg[256] = {0};
		if ( DioOpen(&m_handle, m_config.driver, m_config.group) != 0 ) {
			// DI/Oボード初期化オープン失敗
			LoadString(m_module, IDS_NEXIO_TITLE, szTitleMsg, sizeof(szTitleMsg));
			LoadString(m_module, IDS_NEXIO_OPEN_FAILED, szErrorMsg, sizeof(szErrorMsg));
			m_lfs << arrange
				  << datetime
				  << szErrorMsg
				  << std::endl;
			m_lfs.close();
			nexifAlarmSet(NEXALID_DioOpenError);
			nexuiMessageBox(NULL, szErrorMsg, szTitleMsg, MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
		}
		m_dinp.PortNum = m_dout.PortNum = m_config.port;
		m_dinp.InpPortNo = m_dinp.port; m_dout.OutPortNo = m_dout.port;

		// スレッド開始
		begin();

		return TRUE;
	}

	inline bool IsOpen (void) const { return m_handle != NULL; }
	inline unsigned short GetNumSlots (void) const { return m_config.port / 8; }

	virtual basic_device* GetInpDataPtr (void) { return (basic_device*) m_dinp.value.pptr(); }
	virtual basic_device* GetOutDataPtr (void) { return (basic_device*) m_dout.value.pptr(); }

	// マニュアル操作クラス
	class basic_manual
	{
	public:
		basic_manual () { value = false; }
		inline bool is_enable (void) const { return value; }
		inline void flip (void) { value = !value; }

	private:
		bool value;
	};

	struct manual_t {
		basic_manual input;
		basic_manual output;
	} manual;

protected:
	unsigned int start_address (void)
	{
		_RPTF0(_CRT_WARN, TEXT("cDioBaseThread::ThreadProc - Enter \n"));

		while ( is_loop() ) {
			// マニュアル操作確認
			if ( !manual.input.is_enable() ) {
				// 入力
				m_dinp.execute(DioInp, m_handle);
			}
			// 出力
			m_dout.execute(DioOut, m_handle);

			// ログ出力
			Logging();

			Sleep(10);
		}

		_RPTF0(_CRT_WARN, TEXT("cDioBaseThread::ThreadProc - Exit \n"));

		return 0L;
	}

	void Logging (void)
	{
		m_dout.masking();
		if ( m_dinp.is_update() || m_dout.is_update() ) {
			m_lfs << arrange
				  << datetime
				  << m_dinp
				  << SPACECHAR
				  << SPACECHAR
				  << m_dout
				  << std::endl;
			m_lfs.close();
		}
		m_dinp.copy(); m_dout.copy();
	}

protected:
	basic_dio<DINP, EQPIOINP_BYTES> m_dinp;
	basic_dio<DOUT, EQPIOOUT_BYTES> m_dout;

	dio_config_t  m_config;

private:
	HANDLE		  m_handle;
	HMODULE 	  m_module;
	lfstream	  m_lfs;
	NEXINIPARAMS  m_params;
};

}; // namespace nano

#endif // _NEXIO_H_
