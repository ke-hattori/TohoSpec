// CtaSio.cpp : implementation file
//

#include "stdafx.h"
#include "LogFile.h"
#include "CtaSioDef.h"
#include "CtaSio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	マクロ定義
 */
#define	PKT_LEN_MAX			(PKT_DATA_LEN_MAX + 4)	/* 最大パケット長	*/
#define	RCV_BUF_NUM			(8)						/* 受信バッファ数	*/
#define	PKT_LEN_MIN			(8)						/* 最小パケット長	*/
#define	TLRCV_LEN_MAX		16384					/* スレッドループ用 最大受信データ長	*/
#define	TLRCV_TIME			50						/* スレッドループ用 受信待ち時間[ms]	*/


/*
 *	構造体定義
 */


/*
 *	ローカルグローバル定義
 */
static char lgs_cCR = 0x0d;
static char lgs_cLF = 0x0a;
/* パケットコード	*/
const char lg_cPktCode[] = {
	'!',	/* コマンド	*/
	'!',	/* イベントレポート	*/
	'#',	/* レスポンス	*/
	'%',	/* エラーレスポンス	*/
	'-',	/* (最大値)	*/
};
/* コマンドコード	*/
const char* lg_pszCmdCode[] = {
	"STA",	/* CMD_STA	コマンド：状態要求				*/
	"ORG",	/* CMD_ORG	コマンド：原点復帰				*/
	"MOD",	/* CMD_MOD	コマンド：測定モードセット		*/
	"CID",	/* CMD_CID	コマンド：カセット情報セット	*/
	"PID",	/* CMD_PID	コマンド：基板情報セット		*/
	"SPT",	/* CMD_SPT	コマンド：基板厚み情報セット	*/
	"MST",	/* CMD_MST	コマンド：測定開始				*/
	"MPE",	/* CMD_MPE	コマンド：基板終了通知			*/
	"MCE",	/* CMD_MCE	コマンド：カセット終了通知		*/
	"RST",	/* CMD_RST	コマンド：トラブルリセット		*/
	"END",	/* CMD_END	コマンド：FAMAS 終了			*/
	"---",	/* CMD_MAX	コマンド：(最大値)				*/
	"CMO",	/* EVT_CMO	イベント：移動完了				*/
	"CST",	/* EVT_CST	イベント：設定完了				*/
	"CMP",	/* EVT_CMP	イベント：１ポイント測定完了	*/
	"CMA",	/* EVT_CMA	イベント：全ポイント測定完了	*/
	"REP",	/* EVT_REP	イベント：エラー発生			*/
	"---",	/* EVT_MAX	イベント：(最大値)				*/
};
/* ウィンドウメッセージ	*/
UINT WM_CTASIO_RECVED = ::RegisterWindowMessage("WM_CTASIO_RECVED");	/* データ受信	*/
UINT WM_CTASIO_RCVBUFFULL = ::RegisterWindowMessage("WM_CTASIO_RCVBUFFULL");	/* 受信バッファフル	*/
UINT WM_CTASIO_RCVFAIL = ::RegisterWindowMessage("WM_CTASIO_RCVFAIL");	/* 受信失敗(スレッドループ)	*/


/*
 *	Extern 定義
 */
extern CLogFile* pLogFile;	/* from MEASYS.cpp	*/


/////////////////////////////////////////////////////////////////////////////
// CCtaSio

IMPLEMENT_DYNCREATE(CCtaSio, CWinThread)

CCtaSio::CCtaSio(DWORD dwCreateFlags/* = 0*/)
{
	TRACE(_T("CCtaSio::CCtaSio() \n"));

	this->Logging(_T("constructor."));

	/* 初期化	*/
	m_bInited = FALSE;
	m_bInitedInstance = FALSE;
	this->m_bAutoDelete = FALSE;	/* オブジェクトの自動破棄を禁止	*/
	m_bHWS = FALSE;
	m_pszCmdBufHWS = 0x00;

	/* クリティカルセクションを定義	*/
	::InitializeCriticalSection(&m_csComDev);
	::InitializeCriticalSection(&m_csSend);
	::InitializeCriticalSection(&m_csRcvBuf);
	::EnterCriticalSection(&m_csComDev);
	::EnterCriticalSection(&m_csSend);
	::EnterCriticalSection(&m_csRcvBuf);

	m_pvRcvBufs = malloc((PKT_LEN_MAX + 5) * RCV_BUF_NUM);
	if (0 != m_pvRcvBufs) {
		/* スレッドの起動待ち	*/
		m_dwCreateFlags = dwCreateFlags;
		BOOL l_bThreadOK = this->CreateThread(m_dwCreateFlags);
		if (0 != this->m_hThread) {
			if (CREATE_SUSPENDED != m_dwCreateFlags) {
				CSingleLock	l_SingleLock(&m_cSyncEvent, TRUE);
			}
		}
	}

	::LeaveCriticalSection(&m_csRcvBuf);
	::LeaveCriticalSection(&m_csSend);
	::LeaveCriticalSection(&m_csComDev);
}

CCtaSio::~CCtaSio()
{
	TRACE(_T("CCtaSio::~CCtaSio() \n"));

	this->Logging(_T("destructor."));

	m_bInited = FALSE;

	/* スレッドの消滅待ち	*/
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			if (TRUE == m_bInitedInstance) {
				DWORD l_dwSuspendCount = 0;
				do {
					l_dwSuspendCount = this->ResumeThread();
				} while ((0 != l_dwSuspendCount) && (0xffffffff != l_dwSuspendCount));
				::Sleep(50);
				this->PostThreadMessage(WM_QUIT, 0, 0);
				::WaitForSingleObject(m_hThread, INFINITE);
			}
		}
	}

	/* クリティカルセクションを開放	*/
	::EnterCriticalSection(&m_csRcvBuf);	/* 取得されたままかも知れないので待つ	*/
	::LeaveCriticalSection(&m_csRcvBuf);	/* 開放する								*/
	::DeleteCriticalSection(&m_csRcvBuf);
	::EnterCriticalSection(&m_csSend);		/* 取得されたままかも知れないので待つ	*/
	::LeaveCriticalSection(&m_csSend);		/* 開放する								*/
	::DeleteCriticalSection(&m_csSend);
	::EnterCriticalSection(&m_csComDev);	/* 取得されたままかも知れないので待つ	*/
	::LeaveCriticalSection(&m_csComDev);	/* 開放する								*/
	::DeleteCriticalSection(&m_csComDev);

	if (0 != m_hCom) {
		::CloseHandle(m_hCom);
	}
	if (0 != m_pvRcvBufs) {
		free(m_pvRcvBufs);
		m_pvRcvBufs = 0;
	}
}

BOOL CCtaSio::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("CCtaSio::InitInstance() \n"));

	this->Logging(_T("InitInstance()."));

//	m_bInitedInstance = TRUE;

//	/* コンストラクタの待ちを解除します	*/
//	/*		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．	*/
//	if (CREATE_SUSPENDED != m_dwCreateFlags) {
//		m_cSyncEvent.SetEvent();
//	}

	return TRUE;
}

int CCtaSio::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	TRACE(_T("CCtaSio::ExitInstance() \n"));

	this->Logging(_T("ExitInstance()."));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCtaSio, CWinThread)
	//{{AFX_MSG_MAP(CCtaSio)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtaSio message handlers

int CCtaSio::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	static BOOL ls_bFirst = FALSE;
	if (TRUE != ls_bFirst) {
		ls_bFirst = TRUE;
		/* コンストラクタの待ちを解除します	*/
		/*		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．	*/
		if (CREATE_SUSPENDED != m_dwCreateFlags) {
			m_cSyncEvent.SetEvent();
		}
		m_bInitedInstance = TRUE;
	}

	return CWinThread::Run();
}

BOOL CCtaSio::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

//	RecvLoop();	/* 受信ループ	*/
	if (0 != m_bHWS) {
		this->HWS_RecvLoop();	/* 受信ループ - HWS モード	*/
	} else {
		this->RecvLoop();	/* 受信ループ	*/
	}

	return CWinThread::OnIdle(lCount);
}

/****************************************************************************
	制御用関数
****************************************************************************/

/*
 *	SIO 初期化処理
 */
// 2009.11.18 K.Matsuo -->
//BOOL CCtaSio::Initialize(
//		int iComNo,
//		int iBaudRate,
//		int iByteSize,
//		int iParity,
//		int iStopBits,
//		BOOL bHWS
//	)
// 2009.11.18 K.Matsuo <--
BOOL CCtaSio::Initialize(
		LPCTSTR pszCommNo,
		LPCTSTR pszBaudRate,
		LPCTSTR pszByteSize,
		LPCTSTR pszParity,
		LPCTSTR pszStopBits,
		BOOL bHWS
	)
{
	m_bHWS = bHWS;

	::EnterCriticalSection(&m_csComDev);
	::EnterCriticalSection(&m_csSend);
	::EnterCriticalSection(&m_csRcvBuf);

	if (0 != m_bHWS) {
		int i = 0;
		while (0 != _tcscmp(ls_pszHWSMsg[i], "")) {
			this->Logging(ls_pszHWSMsg[i++]);
		}
	}

	BOOL l_bRet = TRUE;

	/* COM 番号を定義	*/
	TCHAR l_tszCommNo[_MAX_PATH];
// 2009.11.18 K.Matsuo -->
//	_stprintf(l_tszCommNo, _T("\\\\.\\COM%d"), iComNo);
	_stprintf(l_tszCommNo, _T("\\\\.\\%s"), pszCommNo);
// 2009.11.18 K.Matsuo <--

	/* ハンドルを無効にしておく	*/
	m_hCom = 0;
	/* 受信数をクリア	*/
	m_iIdxWr = -1;
	m_iIdxRd = -1;

	if (0 == m_bHWS) {
		/* COM ポートをオープン	*/
		m_hCom =	::CreateFile(
							l_tszCommNo,
							GENERIC_READ | GENERIC_WRITE,	/* アクセスモード：ReadWrite	*/
							0,	/* 共有モード：共有なし	*/
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL
						);
		if ((INVALID_HANDLE_VALUE == m_hCom) || (0 == m_hCom)) {
			// オープン失敗
			m_hCom = NULL;
			l_bRet = FALSE;
		} else {
			/* デバイス制御ブロックを設定	*/
			BOOL l_bRslt;
			DCB l_dcb;
// 2009.11.18 K.Matsuo -->
			char szBuff[256];
// 2009.11.18 K.Matsuo <--
			l_dcb.DCBlength = sizeof(DCB);	/* デバイス制御ブロックのバイト数	*/
			GetCommState(m_hCom, &l_dcb);	/* 現在の設定を取得	*/

// 2009.11.18 K.Matsuo -->
//			l_dcb.BaudRate = iBaudRate;	/* ボーレート	*/
//			l_dcb.ByteSize = iByteSize;	/* 8bit or 7bit	*/
//			l_dcb.Parity   = iParity;	/* パリティ	*/
//			l_dcb.StopBits = iStopBits;	/* ストップビット	*/
//			l_dcb.fOutxDsrFlow = 0;
//			l_dcb.fDtrControl = DTR_CONTROL_ENABLE;	/* DTR は常に ON	*/
//			l_dcb.fOutxCtsFlow = 0;
//			l_dcb.fRtsControl = RTS_CONTROL_ENABLE;	/* RTS は常に ON	*/
//			l_dcb.fInX = 0;
//			l_dcb.fOutX = 0;
			_stprintf(szBuff, _T("baud=%s parity=%s data=%s stop=%s"), pszBaudRate, pszParity, pszByteSize, pszStopBits);
			::BuildCommDCB(szBuff, &l_dcb);
// 2009.11.18 K.Matsuo <--
			l_bRslt = ::SetCommState(m_hCom, &l_dcb);	// 書換え
			if (0 != l_bRslt) {
				/* タイムアウトを設定(ココではタイムアウトは設定しない)	*/
				m_CommTimeOuts.ReadIntervalTimeout = 0;
				m_CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
				m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
				m_CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
				m_CommTimeOuts.WriteTotalTimeoutConstant = 0;
				l_bRet = ::SetCommTimeouts(m_hCom, &m_CommTimeOuts);
				if (0 == l_bRslt) {
					l_bRet = FALSE;
				}
			} else {
				l_bRet = FALSE;
			}
		}
	}

	if (TRUE == l_bRet) {
		m_bInited = TRUE;	/* 初期化完了	*/
	}

	::LeaveCriticalSection(&m_csRcvBuf);
	::LeaveCriticalSection(&m_csSend);
	::LeaveCriticalSection(&m_csComDev);

	return l_bRet;
}

/*
 *	パケット送信
 */
int CCtaSio::PacketSend(const char* pszSendPkt)
{
	::EnterCriticalSection(&m_csComDev);

	char l_szSendBuf[PKT_LEN_MAX + 5];
	memset(l_szSendBuf, 0, sizeof(l_szSendBuf));

	strncpy(l_szSendBuf, pszSendPkt, PKT_LEN_MAX);
	char l_szSum[4];
	memset(l_szSum, 0, sizeof(l_szSum));
	this->CalcCheckSum(l_szSendBuf, l_szSum);
	strcat(l_szSendBuf, l_szSum);
	l_szSendBuf[strlen(l_szSendBuf)] = lgs_cCR;
	l_szSendBuf[strlen(l_szSendBuf)] = lgs_cLF;

	/* 送信する	*/
	DWORD l_dwLen = strlen(l_szSendBuf);
	DWORD l_dwWriteSize = 0;
	this->LoggingSIO(TRUE, l_szSendBuf, l_dwLen);
	if (0 == m_bHWS) {
		/* 実機モード	*/
		if (0 == ::WriteFile(m_hCom, l_szSendBuf, l_dwLen, &l_dwWriteSize, NULL)) {
			/* 送信失敗	*/
			l_dwWriteSize = CTASIO_ERR_SEND_FAIL;	/* 送信失敗	*/
		}
	} else {
		/* HWS モード	*/
		m_pszCmdBufHWS = l_szSendBuf;
		while (0 != m_pszCmdBufHWS) {
			::Sleep(100);
		}
//		if (0 == HWS_WriteFile(l_szSendBuf)) {
//			/* 送信失敗	*/
//			l_dwWriteSize = CTASIO_ERR_SEND_FAIL;	/* 送信失敗	*/
//		}
	}

	::LeaveCriticalSection(&m_csComDev);

	return (int) l_dwWriteSize;
}

/*
 *	コマンド送信
 */
int CCtaSio::CmdSend(const PACKET_CODE enumPacketCode, const COMMAND_CODE enumdwCmdCode, const char* pszData)
{
	if ((enumPacketCode < 0) || (PKT_MAX <= enumPacketCode)) {

		return CTASIO_ERR_INVALID_PKT;	/* パケットコードが範囲外です．	*/
	}
	if ((enumdwCmdCode < 0) || (CMD_MAX == enumdwCmdCode) || (EVT_MAX <= enumdwCmdCode)) {

		return CTASIO_ERR_INVALID_CMD;	/* コマンドコードが範囲外です．	*/
	}

	::EnterCriticalSection(&m_csSend);

	char l_szData[PKT_DATA_LEN_MAX + 1];
	memset(l_szData, 0, sizeof(l_szData));
	strncpy(l_szData, pszData, PKT_DATA_LEN_MAX);

	/* パケットコード	*/
	char l_szSendCmd[PKT_LEN_MAX + 1];
	l_szSendCmd[0] = lg_cPktCode[enumPacketCode];
	l_szSendCmd[1] = 0x00;
	if (PKT_ERR == enumPacketCode) {
		/* エラーコード	*/
		strcat(l_szSendCmd, l_szData);
		strcpy(l_szData, "");
	} else {
		/* コマンドコード	*/
		strcat(l_szSendCmd, lg_pszCmdCode[enumdwCmdCode]);
	}
	/* データ	*/
	strcat(l_szSendCmd, l_szData);

	int l_iRet = this->PacketSend(l_szSendCmd);

	::LeaveCriticalSection(&m_csSend);

	return l_iRet;
}

/*
 *	受信データの存在をチェック
 */
int CCtaSio::IsExistRcvData()
{
	if (0 == m_bInited) {
		return CTASIO_ERR_UNINIT;	/* 未初期化です．	*/
	} else if (-1 == m_iIdxRd) {
		return CTASIO_ERR_NORCVDATA;	/* 受信データはありません．	*/
	}

	return 0;
}

/*
 *	受信データを受信バッファから取得(上位用)
 */
int CCtaSio::GetRcvData(PACKET_CODE* penumPacketCode, COMMAND_CODE* penumCmdCode, char* pszRcvData)
{
	if ((0 == penumPacketCode) || (0 == penumCmdCode) || (0 == pszRcvData)) {
		return CTASIO_ERR_PARAM;	/* パラメタエラー	*/
	}

	/* 受信データ取得	*/
	char l_szRcvData[PKT_LEN_MAX + 5];
	int l_iRet = this->RcvBufGet(l_szRcvData);
	if (0 != l_iRet) {
		*penumPacketCode = PKT_ERR;
		return l_iRet;
	}

	/* フォーマットチェック ------------------------------------------------*/
	/* パケットコード	*/
	DWORD dwi = 0;
	PACKET_CODE l_enumPktCode;
	for (dwi = 0; dwi < PKT_MAX; dwi++) {
		if (lg_cPktCode[dwi] == l_szRcvData[0]) {
			l_enumPktCode = (PACKET_CODE) dwi;
			if (PKT_CMD == l_enumPktCode) {
				l_enumPktCode = PKT_EVT;
			}
			break;
		}
	}
	if (PKT_MAX <= dwi) {
		*penumPacketCode = PKT_ERR;
		return CTASIO_ERR_INVALID_PKT;	/* パケットコードが範囲外です．	*/
	}
	*penumPacketCode = l_enumPktCode;
	/* データ長	*/
	if (strlen(l_szRcvData) < PKT_LEN_MIN) {
		return CTASIO_ERR_TOOSHORT;	/* 受信データ長が短すぎます．	*/
	}
	/* LF 確認	*/
	if (lgs_cLF != l_szRcvData[strlen(l_szRcvData) - 1]) {
		return CTASIO_ERR_NOLF;	/* LF がありません．	*/
	}
	/* CR 確認	*/
	if (lgs_cCR != l_szRcvData[strlen(l_szRcvData) - 2]) {
		return CTASIO_ERR_NOCR;	/* CR がありません．	*/
	}
	/* チェックサム照合 ----------------------------------------------------*/
	char l_szSumRcv[4];
	l_szSumRcv[0] = l_szRcvData[strlen(l_szRcvData) - 4];
	l_szSumRcv[1] = l_szRcvData[strlen(l_szRcvData) - 3];
	l_szSumRcv[2] = 0x00;
	l_szRcvData[strlen(l_szRcvData) - 4] = 0x00;
	char l_szSumCalc[4];
	this->CalcCheckSum(l_szRcvData, l_szSumCalc);
	if (0 != strcmp(l_szSumRcv, l_szSumCalc)) {
		return CTASIO_ERR_SUM;	/* チェックサム不一致	*/
	}
	/* フォーマットチェック ------------------------------------------------*/
	/* データ(ASCII)チェック	*/
	for (dwi = 0; dwi < strlen(l_szRcvData); dwi++) {
		if (0 != iscntrl(l_szRcvData[dwi])) {
			return CTASIO_ERR_CTRLCODE;	/* 制御コードがあった．	*/
		}
	}
	/* コマンドコード	*/
	COMMAND_CODE l_enumCmdCode;
	char l_szCmdCode[4];
	l_szCmdCode[0] = l_szRcvData[1];
	l_szCmdCode[1] = l_szRcvData[2];
	l_szCmdCode[2] = l_szRcvData[3];
	l_szCmdCode[3] = 0;
	switch (l_enumPktCode) {
	case PKT_CMD:
	case PKT_EVT:
		{	/* コマンドを受信することはありえませんので
				イベントコードのみのチェックです．	*/
			if (strlen(l_szRcvData) < 4) {
				return CTASIO_ERR_TOOSHORT;	/* 受信データ長が短すぎます	*/
			}
			for (dwi = CMD_MAX + 1; dwi < EVT_MAX; dwi++) {
				if (0 == strcmp(lg_pszCmdCode[dwi], l_szCmdCode)) {
					l_enumCmdCode = (COMMAND_CODE) dwi;
					break;
				}
			}
			if (EVT_MAX <= dwi) {
				return CTASIO_ERR_INVALID_EVT;	/* イベントコードが範囲外です．	*/
			}
			*penumCmdCode = l_enumCmdCode;
			strcpy(pszRcvData, &(l_szRcvData[4]));
		}
		break;
	case PKT_RES:
		{
			if (strlen(l_szRcvData) < 4) {
				return CTASIO_ERR_TOOSHORT;	/* 受信データ長が短すぎます	*/
			}
			for (dwi = 0; dwi < CMD_MAX; dwi++) {
				if (0 == strcmp(lg_pszCmdCode[dwi], l_szCmdCode)) {
					l_enumCmdCode = (COMMAND_CODE) dwi;
					break;
				}
			}
			if (CMD_MAX <= dwi) {
				return CTASIO_ERR_INVALID_CMD;	/* コマンドコードが範囲外です．	*/
			}
			*penumCmdCode = l_enumCmdCode;
			strcpy(pszRcvData, &(l_szRcvData[4]));
		}
		break;
	case PKT_ERR:
		{
			if (4 != strlen(l_szRcvData)) {
				return CTASIO_ERR_INVALIDLEN;	/* 受信データ長異常	*/
			}
			*penumCmdCode = CMD_MAX;
			strcpy(pszRcvData, l_szCmdCode);
		}
		break;
	default:
		return CTASIO_ERR_INVALID_PKT;	/* パケットコードが範囲外です．	*/
		break;
	}

	return 0;
}

/*
 *	スレッドのサスペンドカウントをデクリメントします．
 */
DWORD CCtaSio::ResumeSIO(DWORD dwParentThreadID)
{
	m_dwParentThreadID = dwParentThreadID;

	return this->ResumeThread();
}

/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	チェックサム計算
 *		8 ビット add モードです．
 */
int CCtaSio::CalcCheckSum(const char* pszData, char* pszChkSum)
{
	if ((0 == pszData) || (0 == pszChkSum)) {
		return -1;
	}
	char l_cSum = 0;
	int l_iCount = 0;
	while ((l_iCount < PKT_LEN_MAX) && (0 == iscntrl(*pszData))) {
		l_cSum += *pszData++;
		l_iCount++;
	}
	char l_cSumH = (l_cSum >> 4) & 0x0f;
	char l_cSumL = l_cSum & 0x0f;
	if (l_cSumH < 0x0a) {
		l_cSumH |= 0x30;
	} else {
		l_cSumH += 0x37;
	}
	if (l_cSumL < 0x0a) {
		l_cSumL |= 0x30;
	} else {
		l_cSumL += 0x37;
	}
	pszChkSum[0] = l_cSumH;
	pszChkSum[1] = l_cSumL;
	pszChkSum[2] = 0x00;

	return l_iCount;
}

/*
 *	受信データを受信バッファにセット
 */
int CCtaSio::RcvBufSet(char* pszRcvData)
{
	if (0 == m_bInited) {
		return CTASIO_ERR_UNINIT;	/* 未初期化です．	*/
	}

	::EnterCriticalSection(&m_csRcvBuf);

	int l_iRet = 0;

	char (*l_pszRcvBufs)[][PKT_LEN_MAX + 5];
	l_pszRcvBufs = (char(*)[][PKT_LEN_MAX + 5]) m_pvRcvBufs;

	if (-1 == m_iIdxWr) {
		m_iIdxWr = 0;
		memset((*l_pszRcvBufs)[m_iIdxWr], 0, PKT_LEN_MAX + 5);
		strncpy((*l_pszRcvBufs)[m_iIdxWr], pszRcvData, PKT_LEN_MAX + 4);
		/* ライトインデックス更新	*/
		m_iIdxRd = m_iIdxWr;
		m_iIdxWr++; if (RCV_BUF_NUM <= m_iIdxWr) {m_iIdxWr = 0;}
		this->NotifyParent(WM_CTASIO_RECVED, 0, 0);	/* 親スレッドに通知	*/
	} else {
		if (m_iIdxRd == m_iIdxWr) {
			/* リードインデックスに到達してしまっていた．	*/
			/* 書込みはしません．*/
			/* バッファフルエラーです．	*/
			l_iRet = CTASIO_ERR_RCVBUFFULL;	/* 受信バッファフル	*/
			this->NotifyParent(WM_CTASIO_RCVBUFFULL, 0, 0);	/* 親スレッドに通知	*/
		} else {
			memset((*l_pszRcvBufs)[m_iIdxWr], 0, PKT_LEN_MAX + 5);
			strncpy((*l_pszRcvBufs)[m_iIdxWr], pszRcvData, PKT_LEN_MAX + 4);
			/* ライトインデックス更新	*/
			if (-1 == m_iIdxRd) {m_iIdxRd = m_iIdxWr;}
			m_iIdxWr++; if (RCV_BUF_NUM <= m_iIdxWr) {m_iIdxWr = 0;}
			this->NotifyParent(WM_CTASIO_RECVED, 0, 0);	/* 親スレッドに通知	*/
		}
	}

	::LeaveCriticalSection(&m_csRcvBuf);

	return l_iRet;
}

/*
 *	受信データを受信バッファから取得
 */
int CCtaSio::RcvBufGet(char* pszRcvData)
{
	if (0 == m_bInited) {
		return CTASIO_ERR_UNINIT;	/* 未初期化です．	*/
	}
	if (0 == pszRcvData) {
		return CTASIO_ERR_PARAM;	/* 格納用バッファのアドレスが異常です．	*/
	}

	::EnterCriticalSection(&m_csRcvBuf);

	int l_iRet = 0;

	if (-1 == m_iIdxRd) {
		l_iRet = CTASIO_ERR_NORCVDATA;	/* 受信データはありません．	*/
	} else {
		char (*l_pszRcvBufs)[][PKT_LEN_MAX + 5];
		l_pszRcvBufs = (char(*)[][PKT_LEN_MAX + 5]) m_pvRcvBufs;

		memset(pszRcvData, 0, PKT_LEN_MAX + 5);
		strncpy(pszRcvData, (*l_pszRcvBufs)[m_iIdxRd], PKT_LEN_MAX + 4);
		/* リードインデックス更新	*/
		m_iIdxRd++; if (RCV_BUF_NUM <= m_iIdxRd) {m_iIdxRd = 0;}
		if (m_iIdxWr == m_iIdxRd) {
			/* ライトインデックスに到達してしまった．	*/
			m_iIdxRd = -1;
		}
	}

	::LeaveCriticalSection(&m_csRcvBuf);

	return l_iRet;
}

/*
 *	ログ
 */
void CCtaSio::Logging(LPCTSTR strLog)
{
	if (0 == strLog) {
		return;
	}

	CString l_strLogText;
	l_strLogText.Format(_T("CCtaSio  : %s"), strLog);

	pLogFile->Logging(l_strLogText);
}

/*
 *	送受信データログ
 */
void CCtaSio::LoggingSIO(BOOL bMode, LPCSTR pszLogData, DWORD dwLen)
{
	if (0 == pszLogData) {
		return;
	}

	char l_szLogData[(PKT_LEN_MAX + 5) * 2];
	memset(l_szLogData, 0, sizeof(l_szLogData));

	strcpy(l_szLogData, (TRUE == bMode)? "CtaSio   : Send " : "CtaSio   : Recv ");
	DWORD l_dwSrc = 0;
	DWORD l_dwDst = strlen(l_szLogData);
	DWORD l_dwLen = dwLen;
	if ((0 == l_dwLen) || (PKT_LEN_MAX + 5 < l_dwLen)) {
		l_dwLen = PKT_LEN_MAX + 5;
	}
	while (l_dwSrc < l_dwLen) {
		if ((0 != iscntrl(pszLogData[l_dwSrc])) || (0 == isascii(pszLogData[l_dwSrc]))) {
			l_szLogData[l_dwDst++] = '[';
			char l_szH = (pszLogData[l_dwSrc] >> 4) & 0x0f;
			l_szH += (l_szH <= 0x09)? 0x30 : 0x37;
			char l_szL = pszLogData[l_dwSrc] & 0x0f;
			l_szL += (l_szL <= 0x09)? 0x30 : 0x37;
			l_szLogData[l_dwDst++] = l_szH;
			l_szLogData[l_dwDst++] = l_szL;
			l_szLogData[l_dwDst++] = ']';
			l_dwSrc++;
		} else {
			l_szLogData[l_dwDst++] = pszLogData[l_dwSrc++];
		}
	}

	pLogFile->Logging(l_szLogData);
}

/*
 *	親スレッドに(ウィンドウメッセージを)通知
 */
BOOL CCtaSio::NotifyParent(UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (0 == m_dwParentThreadID) {
		return FALSE;
	}

	TCHAR l_tszMsg[32];
	if			(WM_CTASIO_RECVED == uiMsg) {
		_tcscpy(l_tszMsg, _T("WM_CTASIO_RECVED"));
	} else if	(WM_CTASIO_RCVBUFFULL == uiMsg) {
		_tcscpy(l_tszMsg, _T("WM_CTASIO_RCVBUFFULL"));
	} else {
		return FALSE;
	}
	TCHAR l_tszLog[256];
	_stprintf(l_tszLog, _T("Notify %s (WPARAM = 0x%08x, LPARAM = 0x%08x) to CtaCtrl."), l_tszMsg, wParam, lParam);
	this->Logging(l_tszLog);

	return ::PostThreadMessage(m_dwParentThreadID, uiMsg, wParam, lParam);
}

/*
 *	受信ループ
 */
void CCtaSio::RecvLoop()
{
	static BOOL ls_bLoop = TRUE;
	static BOOL ls_bRunOnce = FALSE;

	char	l_szRecvBuf[TLRCV_LEN_MAX + 1];	/* 受信バッファ	*/
	char	l_szRecvDat[TLRCV_LEN_MAX + 1];	/* 受信データ	*/
	COMSTAT	l_ComStat;						/* デバイスの状態	*/
	DWORD	l_dwReadCount;					/* 読出したバイト数	*/
	DWORD	l_dwErrors;						/* エラー情報	*/
	DWORD	l_dwRecvLen;					/* 受信バイト数	*/
	int 	i = 0;

	while (0 != ls_bLoop) {
		::EnterCriticalSection(&m_csComDev);

		if (TRUE != m_bInited) {
			/* 未初期化なので，何もしません．	*/
		} else
		if (TRUE != ls_bRunOnce) {
			ls_bRunOnce = TRUE;
			/* 初回のみ実行 ------------------------------------------------*/
			l_dwReadCount = 0;
			l_dwErrors = 0;
			l_dwRecvLen = 0;
			i = 0;
			memset(l_szRecvBuf, 0, sizeof(l_szRecvBuf));
			memset(l_szRecvDat, 0, sizeof(l_szRecvDat));
			/* 既に受信していたデータがあれば読み捨てる	*/
			m_CommTimeOuts.ReadTotalTimeoutConstant = 0;
			if (0 == ::SetCommTimeouts(m_hCom, &m_CommTimeOuts)) {
				/* 関数失敗	*/
				this->NotifyParent(WM_CTASIO_RCVFAIL, 0, 0);
				this->Logging(_T("RcvLoop Failed to SetCommTimeouts() - 1"));
				ls_bLoop = FALSE;	/* スレッドループ終了	*/
				return;
			} else
			if (0 == ::ClearCommError(m_hCom, &l_dwErrors, &l_ComStat)) {
				/* 関数失敗	*/
				this->NotifyParent(WM_CTASIO_RCVFAIL, 0, 0);
				this->Logging(_T("RcvLoop Failed to ClearCommError() - 1"));
				ls_bLoop = FALSE;	/* スレッドループ終了	*/
				return;
			} else {
				if (l_ComStat.cbInQue != 0) {
					/* 受信データがあれば読み捨て	*/
					if (0 == ::ReadFile(m_hCom, &l_szRecvBuf, l_ComStat.cbInQue, &l_dwReadCount, NULL)) {
						/* 関数失敗	*/
						this->NotifyParent(WM_CTASIO_RCVFAIL, 0, 0);
						this->Logging(_T("RcvLoop Failed to ReadFile() - 1"));
						ls_bLoop = FALSE;	/* スレッドループ終了	*/
						return;
					}
				}
			}
			m_CommTimeOuts.ReadTotalTimeoutConstant = TLRCV_TIME;
			if (0 == ::SetCommTimeouts(m_hCom, &m_CommTimeOuts)) {
				/* 関数失敗	*/
				this->NotifyParent(WM_CTASIO_RCVFAIL, 0, 0);
				this->Logging(_T("RcvLoop Failed to SetCommTimeouts() - 2"));
				ls_bLoop = FALSE;	/* スレッドループ終了	*/
				return;
			}
			l_dwRecvLen = 0;
			this->Logging(_T("RcvLoop started."));
		} else {
			/* メインループ ------------------------------------------------*/
			if (0 == ::ReadFile(m_hCom, &l_szRecvBuf, 1, &l_dwReadCount, NULL)) {
				/* 関数失敗	*/
				this->NotifyParent(WM_CTASIO_RCVFAIL, 0, 0);
				this->Logging(_T("RcvLoop Failed to ReadFile() - 2"));
				ls_bLoop = FALSE;	/* スレッドループ終了	*/
				this->Logging(_T("RcvLoop ended."));
				return;
			} else if (0 == l_dwReadCount) {
				/* 受信していない場合は何もしません．	*/
			} else {
				static int ls_iPrc = 0;
				switch (ls_iPrc) {
				case 0:		/* パケットコード受信待ち	*/
					for (i = 0; i < PKT_MAX; i++) {
						if (lg_cPktCode[i] == l_szRecvBuf[0]) {
							break;
						}
					}
					if (PKT_MAX <= i) {
						/* 無効データ受信	*/
						/* は，ログのみで無視します．	*/
						l_szRecvBuf[1] = 0x00;
						this->LoggingSIO(FALSE, l_szRecvBuf, 1);
					} else {
						l_szRecvDat[l_dwRecvLen++] = l_szRecvBuf[0];
						l_szRecvDat[l_dwRecvLen] = 0x00;
						ls_iPrc = 1;
					}
					break;
				case 1:		/* LF 受信待ち	*/
					if (lgs_cLF == l_szRecvBuf[0]) {
						/* LF だった	*/
						l_szRecvDat[l_dwRecvLen++] = l_szRecvBuf[0];
						l_szRecvDat[l_dwRecvLen] = 0x00;
						char l_szRcvCmd[4];
						l_szRcvCmd[0] = l_szRecvDat[1];
						l_szRcvCmd[1] = l_szRecvDat[2];
						l_szRcvCmd[2] = l_szRecvDat[3];
						l_szRcvCmd[3] = 0x00;
						if ((PKT_LEN_MAX + 4) < l_dwRecvLen) {
							/*	実際にはありえないくらい大きいパケットは無視します．	*/
							this->LoggingSIO(FALSE, l_szRecvDat, l_dwRecvLen);
							/* CMA の場合はココでレスポンスを返します．
								以外は無応答です．	*/
							if (0 == strcmp(l_szRcvCmd, lg_pszCmdCode[EVT_CMA])) {
								this->CmdSend(PKT_RES, EVT_CMA, "");
								this->NotifyParent(WM_CTASIO_RECVED, (WPARAM) TRUE, 0);	/* 親スレッドに通知	*/
							}
						} else if (0 == strcmp(l_szRcvCmd, lg_pszCmdCode[EVT_CMA])) {
							/* CMA は不要なのでログのみ．
								なので，受信バッファに入れない．*/
							/* CMA の場合はココでレスポンスを返します．	*/
							this->LoggingSIO(FALSE, l_szRecvDat, l_dwRecvLen);
							this->CmdSend(PKT_RES, EVT_CMA, "");
							this->NotifyParent(WM_CTASIO_RECVED, (WPARAM) TRUE, 0);	/* 親スレッドに通知	*/
						} else {
							this->LoggingSIO(FALSE, l_szRecvDat, l_dwRecvLen);
							/* 受信バッファに格納	*/
							this->RcvBufSet(l_szRecvDat);
						}
						ls_iPrc = 0;
						l_dwRecvLen = 0;
					} else {
						l_szRecvDat[l_dwRecvLen++] = l_szRecvBuf[0];
						l_szRecvDat[l_dwRecvLen] = 0x00;
						if (TLRCV_LEN_MAX < l_dwRecvLen) {
							/* バッファオーバフロー	*/
							this->Logging(_T("RcvLoop receive buffer overflow."));
							this->LoggingSIO(FALSE, l_szRecvDat, l_dwRecvLen);
							ls_iPrc = 0;
							l_dwRecvLen = 0;
						}
					}
					break;
				}
			}

			/* スレッドループ終了チェック ----------------------------------*/
			if (TRUE != m_bInited) {
				/* バッファに存在する受信データをログします．	*/
				if (0 != l_dwRecvLen) {
					this->LoggingSIO(FALSE, l_szRecvDat, l_dwRecvLen);
					l_dwRecvLen = 0;
				}
				ls_bLoop = FALSE;	/* スレッドループ終了	*/
				this->Logging(_T("RcvLoop ended."));
			}
		}

		::LeaveCriticalSection(&m_csComDev);
	}
}

/*
 *	受信ループ - HWS モード
 */
void CCtaSio::HWS_RecvLoop()
{
	static BOOL ls_bLoop = TRUE;
	static BOOL ls_bRunOnce = FALSE;
	static int ls_iPrc = -1;
	static int ls_iPrcSub = -1;
	static int ls_iRcvSTA = 0;
	PACKET_CODE l_enumPktCode;
	COMMAND_CODE l_enumCmdCode;

	while (0 != ls_bLoop) {
		if (TRUE != m_bInited) {
			/* 未初期化なので，何もしません．	*/
		} else
		if (TRUE != ls_bRunOnce) {
			ls_bRunOnce = TRUE;
			this->Logging(_T("RcvLoop(HWS) started."));
		} else {
			::Sleep(100);
			l_enumPktCode = PKT_MAX;
			l_enumCmdCode = CMD_MAX;
			if (0 != m_pszCmdBufHWS) {
				char l_szCmdBuf[PKT_LEN_MAX + 5];
				strcpy(l_szCmdBuf, m_pszCmdBufHWS);
				DWORD dwi = 0;
				/* パケットコード抽出	*/
				for (dwi = PKT_CMD; dwi < PKT_MAX; dwi++) {
					if (lg_cPktCode[dwi] == m_pszCmdBufHWS[0]) {
						l_enumPktCode = (PACKET_CODE) dwi;
						break;
					}
				}
				if (PKT_MAX <= dwi) {l_enumPktCode = PKT_MAX;}
				/* コマンドコード抽出	*/
				m_pszCmdBufHWS[4] = 0x00;
				for (dwi = CMD_STA; dwi < EVT_MAX; dwi++) {
					if (0 == strcmp(lg_pszCmdCode[dwi], &(m_pszCmdBufHWS[1]))) {
						l_enumCmdCode = (COMMAND_CODE) dwi;
						break;
					}
				}
				if (EVT_MAX <= dwi) {l_enumCmdCode = EVT_MAX;}
				/* パケットとコマンドコードの整合性をチェック	*/
				switch (l_enumPktCode) {
				case PKT_CMD:
				case PKT_RES:
					if ((EVT_MAX == l_enumCmdCode) || (CMD_MAX == l_enumCmdCode)) {
						this->HWS_RcvBufSet(PKT_ERR, CMD_STA, "101");
					}
					break;
				case PKT_ERR:
					break;
				default:
					this->HWS_RcvBufSet(PKT_ERR, CMD_STA, "100");
					break;
				}
				/* 受信完了	*/
				m_pszCmdBufHWS = 0;
			}

//hdebdeb			::EnterCriticalSection(&m_csComDev);

			switch (ls_iPrc) {
			case -1:	/* 新規シーケンス待ち	*/
				if ((PKT_CMD == l_enumPktCode) && (CMD_MAX != l_enumCmdCode)) {
/* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- { ---------- */
//					if (CMD_STA == l_enumCmdCode) {ls_iRcvSTA = 1;}
/* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ----------              */
					if (CMD_STA == l_enumCmdCode) {ls_iRcvSTA = 2;}
/* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- } ---------- */
					switch (l_enumCmdCode) {
					case CMD_STA:	this->HWS_RcvBufSet(PKT_RES, CMD_STA, "121");	break;
					case CMD_ORG:	this->HWS_RcvBufSet(PKT_RES, CMD_ORG, "");	ls_iPrc = CMD_ORG;	break;
					case CMD_MOD:	this->HWS_RcvBufSet(PKT_RES, CMD_MOD, "");	break;
					case CMD_CID:	this->HWS_RcvBufSet(PKT_RES, CMD_CID, "");	break;
					case CMD_PID:	this->HWS_RcvBufSet(PKT_RES, CMD_PID, "");	break;
					case CMD_SPT:	this->HWS_RcvBufSet(PKT_RES, CMD_SPT, "");	ls_iPrc = CMD_SPT;	break;
					case CMD_MST:	this->HWS_RcvBufSet(PKT_RES, CMD_MST, "");	ls_iPrc = CMD_MST;	break;
					case CMD_MPE:	this->HWS_RcvBufSet(PKT_RES, CMD_MPE, "");	ls_iPrc = CMD_MPE;	break;
					case CMD_MCE:	this->HWS_RcvBufSet(PKT_RES, CMD_MCE, "");	break;
					case CMD_RST:
						if (0 == ls_iRcvSTA) {
							this->HWS_RcvBufSet(PKT_RES, CMD_RST, "");
						} else {
							if (1 == ls_iRcvSTA) {
								this->HWS_RcvBufSet(PKT_ERR, CMD_STA, "200");
							} else {
								this->HWS_RcvBufSet(PKT_RES, CMD_RST, "");
							}
							ls_iPrc = CMD_RST;
						}
						break;
					case CMD_END:	this->HWS_RcvBufSet(PKT_RES, CMD_END, "");	break;
					default:		ASSERT(0);	break;
					}
				}
				break;
			case CMD_ORG:
				switch (ls_iPrcSub) {
				case -1:	::Sleep(2000);	ls_iPrcSub = 0;	break;
				case 0:
					this->HWS_RcvBufSet(PKT_EVT, EVT_CMO, "");
					ls_iPrcSub = -1; ls_iPrc = -1;
					break;
				default:	ASSERT(0);	break;
				}
				break;
			case CMD_SPT:
				this->HWS_RcvBufSet(PKT_EVT, EVT_CST, "");
				ls_iPrcSub = -1; ls_iPrc = -1;
				break;
			case CMD_MST:
				switch (ls_iPrcSub) {
				case -1:	::Sleep(2000);	ls_iPrcSub = 0;	break;
				case 0:
					{
						char l_szTemp[8];
						char l_szPointNo[8];
						sprintf(l_szTemp, "%d", (rand() % 1000) + 1);
						memset(l_szPointNo, 0x20, sizeof(l_szPointNo));
						strncpy(l_szPointNo, l_szTemp, strlen(l_szTemp)); l_szPointNo[4] = 0x00;
						char l_szCAngle[8];
						sprintf(l_szTemp, "%d", (rand() % 9999) + 1);
						memset(l_szCAngle, 0x20, sizeof(l_szCAngle));
						strncpy(l_szCAngle, l_szTemp, strlen(l_szTemp)); l_szCAngle[4] = 0x00;
						char l_szRadius[8];
						sprintf(l_szTemp, "%d", (rand() % 99999) + 1);
						memset(l_szRadius, 0x20, sizeof(l_szRadius));
						strncpy(l_szRadius, l_szTemp, strlen(l_szTemp)); l_szRadius[5] = 0x00;
						char l_szLiquid[8];
						sprintf(l_szTemp, "%d", (rand() % 9999) + 1);
						memset(l_szLiquid, 0x20, sizeof(l_szLiquid));
						strncpy(l_szLiquid, l_szTemp, strlen(l_szTemp)); l_szLiquid[4] = 0x00;
						char l_szData[32];
						sprintf(l_szData, "%s%s%s%s", l_szPointNo, l_szCAngle, l_szRadius, l_szLiquid);
						this->HWS_RcvBufSet(PKT_EVT, EVT_CMP, l_szData);
					}
					ls_iPrcSub = -1; ls_iPrc = -1;
					break;
				default:	ASSERT(0);	break;
				}
				break;
			case CMD_MPE:
				switch (ls_iPrcSub) {
				case -1:	::Sleep(2000);	ls_iPrcSub = 0;	break;
				case 0:
					this->HWS_RcvBufSet(PKT_EVT, EVT_CMO, "");
					ls_iPrcSub = 1;	break;
				case 1:
					if ((PKT_RES == l_enumPktCode) && (EVT_CMO == l_enumCmdCode)) {
						this->NotifyParent(WM_CTASIO_RECVED, (WPARAM) TRUE, 0);	/* 親スレッドに通知	*/
						ls_iPrcSub = -1; ls_iPrc = -1;
					}
					break;
				default:	ASSERT(0);	break;
				}
				break;
			case CMD_RST:
				switch (ls_iPrcSub) {
				case -1:
					if (1 == ls_iRcvSTA) {
						this->HWS_RcvBufSet(PKT_EVT, EVT_REP, "200");
						ls_iRcvSTA = 2;
					} else {
						this->HWS_RcvBufSet(PKT_EVT, EVT_REP, "000");
					}
					ls_iPrcSub = -1; ls_iPrc = -1;
					break;
				default:	ASSERT(0);	break;
				}
				break;
			default:	ASSERT(0);	break;
			}

			/* スレッドループ終了チェック ----------------------------------*/
			if (TRUE != m_bInited) {
				ls_bLoop = FALSE;	/* スレッドループ終了	*/
				this->Logging(_T("RcvLoop(HWS) ended."));
			}

//hdebdeb			::LeaveCriticalSection(&m_csComDev);
		}
	}
}

/*
 *	パケット送信(サブ関数) - HWS モード
 */
BOOL CCtaSio::HWS_RcvBufSet(const PACKET_CODE enumPacketCode, const COMMAND_CODE enumdwCmdCode, const char* pszData)
{
	char l_szPktCode[2];
	l_szPktCode[0] = lg_cPktCode[enumPacketCode]; l_szPktCode[1] = 0x00;
	char l_szCmdCode[4] = "";
	char l_szData[PKT_DATA_LEN_MAX + 1]; strcpy(l_szData, pszData);
	if (PKT_ERR == enumPacketCode) {
		strcat(l_szCmdCode, l_szData);
		strcpy(l_szData, "");
	} else {
		strcpy(l_szCmdCode, lg_pszCmdCode[enumdwCmdCode]);
	}
	char l_szSendBuf[PKT_LEN_MAX + 5];
	memset(l_szSendBuf, 0, sizeof(l_szSendBuf));
	sprintf(l_szSendBuf, "%s%s%s", l_szPktCode, l_szCmdCode, l_szData);
	char l_szSum[4];
	this->CalcCheckSum(l_szSendBuf, l_szSum);
	strcat(l_szSendBuf, l_szSum);
	l_szSendBuf[strlen(l_szSendBuf)] = lgs_cCR; l_szSendBuf[strlen(l_szSendBuf)] = lgs_cLF;
	this->LoggingSIO(FALSE, l_szSendBuf, strlen(l_szSendBuf));
	this->RcvBufSet(l_szSendBuf);

	return TRUE;
}
