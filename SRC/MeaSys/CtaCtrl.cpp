// CtaCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CtaSio.h"
#include "LogFile.h"
#include "..\\..\\inc\globals.hxx"
#include "..\\..\\inc\ConfigFile.hxx"
#include "..\\..\\inc\MeaSys.hxx"
#include "..\\..\\inc\\NEXIOBASE.HXX"
#include "..\\..\\inc\\SharedMemory.h"
#include "CtaCtrl.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	マクロ定義
 */
#define	SECTION_CTA	_T("CTA")	/* NanoSpec.ini 内のセクション	*/
/*未使用*///#define	INTVL_TIME	50			/* 定周期タイマ[ms]	*/
#define	SEQWAIT_TIMEOUT			(60 * 1000 * 10)	/* シーケンス完了待ちタイムアウト[ms](10min)	*/
#define	CTA_TIMEOUT_RES			(5000)				/* タイムアウト[ms] レスポンス受信待ち	*/
#define	CTA_TIMEOUT_T9_NORM		(5000)				/* タイムアウト[ms] 会話タイムアウト(イベント待ち)	*/
#define	CTA_TIMEOUT_T9_ORG		(10000)				/* タイムアウト[ms] 会話タイムアウト(原点復帰待ち)	*/
#define	CTA_TIMEOUT_T9_MEAS		(60000)				/* タイムアウト[ms] 会話タイムアウト(測定中)	*/
#define	CTA_TIMEOUT_T9_ALLEND	(60000)				/* タイムアウト[ms] 会話タイムアウト(全ポイント終了受信待ち)	*/
/* modified 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
//#define	CTA_TIMEOUT_ILPI		(1000)				/* タイムアウト[ms] PI インタロック信号タイムアウト	*/
//#define	CTA_TIMEOUT_ILPO		(3000)				/* タイムアウト[ms] PO インタロック信号タイムアウト(Sleep())	*/
/* modified 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ----------              */
#define	CTA_TIMEOUT_ILPI		(5000)				/* タイムアウト[ms] PI インタロック信号タイムアウト	*/
#define	CTA_TIMEOUT_ILPO		(5000)				/* タイムアウト[ms] PO インタロック信号タイムアウト(Sleep())	*/
/* modified 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */
#define	CTA_TIMEOUT_RETRY		(200)				/* タイムアウト[ms] リトライタイマ(Sleep())	*/
#define	CTA_TIMEOUT_MPE_WAIT	(2500)				/* タイムアウト[ms] MPE 送信待ちウェイト(Sleep())	*/
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
/* 制御プロセス番号定義	*/
#define	PRC_INIT_IDLE	1000
#define	PRC_MEAS_IDLE	3000
#define	PRC_MEND_IDLE	4000
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
#define	CTA_ILPO_DELAY	(1500)						/* タイムアウト[ms] PO CTA 認識ディレイ(Sleep())	*/
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */


/*
 *	構造体定義
 */


/*
 *	ローカルグローバル定義
 */
static CCtaCtrl* lgs_pclsCCtaCtrl;	/* 自分のクラスポインタ	*/
static UINT_PTR lgs_uiTimerID = 0;	/* タイマ ID	*/
/* ウィンドウメッセージ	*/
UINT WM_CTACTL_TIMER = ::RegisterWindowMessage("WM_CTACTL_TIMER");	/* タイマ	*/
UINT WM_CTACTL_SEQKICK = ::RegisterWindowMessage("WM_CTACTL_SEQKICK");	/* シーケンスキック	*/


/*
 *	Extern 定義
 */
extern CLogFile* pLogFile;	/* from MEASYS.cpp	*/
extern TCHAR g_tszProcDir[_MAX_PATH];	/* from MEASYS.cpp	*/
extern HWND g_hNotifyWnd;	/* from MEASYS.cpp	*/
extern CSharedMemory<CTARESULT> lg_smCtaResultDataBase;	/* from MEASYS.cpp	*/
extern BOOL CtaRecalib(LPCTSTR szRecalib, double& dData);
extern BOOL bHwSimulation;	/* from MEASYS.cpp	*/
/* ウィンドウメッセージ	*/
extern UINT WM_CTASIO_RECVED;		/* データ受信				from CtaSio.cpp	*/
extern UINT WM_CTASIO_RCVBUFFULL;	/* 受信バッファフル			from CtaSio.cpp	*/
extern UINT WM_CTASIO_RCVFAIL;		/* 受信失敗(スレッドループ)	from CtaSio.cpp	*/


/////////////////////////////////////////////////////////////////////////////
// CCtaCtrl

IMPLEMENT_DYNCREATE(CCtaCtrl, CWinThread)

CCtaCtrl::CCtaCtrl(DWORD dwCreateFlags/* = 0*/)
{
	TRACE(_T("CCtaCtrl::CCtaCtrl() \n"));

	this->Logging(_T("constructor."));

	/* 初期設定	*/
	m_bInitedInstance = FALSE;
	this->m_bAutoDelete = FALSE;	/* オブジェクトの自動破棄を禁止	*/
	lgs_pclsCCtaCtrl = this;
	m_bTimeOut = FALSE;
	m_bRcvData = FALSE;
	m_dwSeqPattern = SEQPTN_NONE;
	m_iSeqResult = 0;
	m_dwRetryCnt = 0;
	m_CtaMeasInf.iCtaMode = -1;
	m_bRecvedCMA = FALSE;
	m_bForceEnd = FALSE;
	m_bHWS = FALSE;
	m_uiTimeout_T9_Meas = CTA_TIMEOUT_T9_MEAS;
	m_uiTimeout_T9_AllEnd = CTA_TIMEOUT_T9_ALLEND;
	m_uiTimeout_ILPO = CTA_TIMEOUT_ILPO;
	m_uiTimeout_Retry = CTA_TIMEOUT_RETRY;
	m_uiTimeout_MPE_Wait = CTA_TIMEOUT_MPE_WAIT;
/* added 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- { ---------- */
	m_bErrRep = FALSE;
/* added 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- } ---------- */
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
	m_uiTimeout_ILPI = CTA_TIMEOUT_ILPI;
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
	m_uiDelay_ILPO = CTA_ILPO_DELAY;
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */

	/* クリティカルセクションを定義	*/
	::InitializeCriticalSection(&m_csCtrlBusy);
	::EnterCriticalSection(&m_csCtrlBusy);

	/* SIO スレッドを起動します．	*/
	this->Logging(_T("Starting CCtaSio..."));
	m_pclsCCtaSio = 0;
	m_pclsCCtaSio = new CCtaSio(CREATE_SUSPENDED);
	if (0 == m_pclsCCtaSio) {
		/* SIO スレッド構築失敗	*/
		this->Logging(_T("Failed to create CCtaSio. (new)"));
	} else
	if (0 == m_pclsCCtaSio->m_hThread) {
		/* SIO スレッド起動失敗	*/
		this->Logging(_T("Failed to start CCtaSio thread. (thread)"));
	} else {
		/* SIO スレッド起動成功	*/
		this->Logging(_T("Started CCtaSio."));
		if (TRUE == this->InitializeSIO()) {

			/* スレッドの起動待ち	*/
			m_dwCreateFlags = dwCreateFlags;
			BOOL l_bThreadOK = this->CreateThread(m_dwCreateFlags);
//			if (0 != this->m_hThread) {
			if (0 != l_bThreadOK) {
				if (CREATE_SUSPENDED != m_dwCreateFlags) {
					CSingleLock	l_SingleLock(&m_cSyncEvent, TRUE);
				}
			}
		}
	}

	::LeaveCriticalSection(&m_csCtrlBusy);
}

CCtaCtrl::~CCtaCtrl()
{
	TRACE(_T("CCtaCtrl::~CCtaCtrl() \n"));

	this->Logging(_T("destructor."));

	if (0 != lgs_uiTimerID) {
		::KillTimer(0, lgs_uiTimerID);
		lgs_uiTimerID = 0;
	}

	/* SIO スレッドを終了	*/
	if (0 != m_pclsCCtaSio) {
		this->Logging(_T("Deleting CCtaSio..."));
		delete m_pclsCCtaSio;
		m_pclsCCtaSio = 0;
		this->Logging(_T("Deleted CCtaSio."));
	}

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
	::EnterCriticalSection(&m_csCtrlBusy);	/* 取得されたままかも知れないので待つ	*/
	::LeaveCriticalSection(&m_csCtrlBusy);	/* 開放する								*/
	::DeleteCriticalSection(&m_csCtrlBusy);
}

BOOL CCtaCtrl::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("CCtaCtrl::InitInstance() \n"));

	this->Logging(_T("InitInstance()."));

//	m_bInitedInstance = TRUE;

	((CCtaSio*) m_pclsCCtaSio)->ResumeSIO(this->m_nThreadID);

//	/* コンストラクタの待ちを解除します	*/
//	/*		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．	*/
//	if (CREATE_SUSPENDED != m_dwCreateFlags) {
//		m_cSyncEvent.SetEvent();
//	}

	return TRUE;
}

int CCtaCtrl::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	TRACE(_T("CCtaCtrl::ExitInstance() \n"));

	this->Logging(_T("ExitInstance()."));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCtaCtrl, CWinThread)
	//{{AFX_MSG_MAP(CCtaCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CTACTL_SEQKICK, OnCtaCtrlSeqKick)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTACTL_TIMER, OnCtaCtrlTimer)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTASIO_RECVED, OnCtaSioRecved)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTASIO_RCVBUFFULL, OnCtaSioRcvBufFull)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTASIO_RCVFAIL, OnCtaSioRcvFail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtaCtrl message handlers

int CCtaCtrl::Run()
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

BOOL CCtaCtrl::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

	this->CtrlSeq();

	return CWinThread::OnIdle(lCount);
}

/*
 *	制御シーケンス起動 メッセージハンドラ
 */
void CCtaCtrl::OnCtaCtrlSeqKick(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTACTL_SEQKICK"), wparam, lparam);
}

/*
 *	タイマ メッセージハンドラ
 */
void CCtaCtrl::OnCtaCtrlTimer(WPARAM wparam, LPARAM lparam)
{
#if 1	/* 定周期タイマの場合はログしないでください．	*/
	this->Logging_WinMsg(_T("WM_CTACTL_TIMER"), wparam, lparam);
#endif
	m_bTimeOut = TRUE;
}

/*
 *	データ受信 メッセージハンドラ
 */
void CCtaCtrl::OnCtaSioRecved(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RECVED"), wparam, lparam);

	if (0 != wparam) {
		m_bRecvedCMA = TRUE;
	} else {
		m_bRcvData = TRUE;
	}
}

/*
 *	受信バッファフル メッセージハンドラ
 */
void CCtaCtrl::OnCtaSioRcvBufFull(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RCVBUFFULL"), wparam, lparam);

	/* 上位に通知	*/
	this->NotifyAlarm(CTASIO_ERR_RCVBUFFULL);
}

/*
 *	受信失敗(CCtaSio スレッドループ) メッセージハンドラ
 */
void CCtaCtrl::OnCtaSioRcvFail(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RCVFAIL"), wparam, lparam);

	/* 上位に通知	*/
	this->NotifyAlarm(CTASIO_ERR_RCVFAIL);
}

/****************************************************************************
	制御用関数
****************************************************************************/

/*
 *	CTA 初期化処理
 */
BOOL CCtaCtrl::Cta_Initialize()
{
	this->Logging(_T("Started InitializeCTA()"));

	BOOL l_bRet = TRUE;

	/* この関数のみココで行います．	*/
	::EnterCriticalSection(&m_csCtrlBusy);

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		l_bRet = this->WaitSeqEndStart(SEQPTN_INIT);
	}

	::LeaveCriticalSection(&m_csCtrlBusy);

	this->Logging(_T("Ended   InitializeCTA()"));

	return l_bRet;
}

/*
 *	CTA 測定モード設定
 *		MEAS_CTA_MODE_1	モード１：基板単位・オペレータデータ確認なし
 *		MEAS_CTA_MODE_2	モード２：基板単位・オペレータデータ確認あり
 *		MEAS_CTA_MODE_3	モード３：カセット単位・オペレータデータ確認あり
 */
BOOL CCtaCtrl::Cta_ModeSet(int iCtaMode)
{
	this->Logging(_T("Started Cta_ModeSet()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		switch (iCtaMode) {
		case MEAS_CTA_MODE_1:
		case MEAS_CTA_MODE_2:
		case MEAS_CTA_MODE_3:
			::EnterCriticalSection(&m_csCtrlBusy);
			m_CtaMeasInf.iCtaMode = iCtaMode;
			::LeaveCriticalSection(&m_csCtrlBusy);
			l_bRet = TRUE;
			break;
		default:
			l_bRet = FALSE;
			break;
		}
	}

	this->Logging(_T("Ended   Cta_ModeSet()"));

	return l_bRet;
}

/*
 *	CTA カセット ID，基板 ID 設定
 */
BOOL CCtaCtrl::Cta_SetCstSample(
		LPCSTR pstrCstID,						/* カセット ID	：ASCII 80 文字	*/
		LPCSTR pstrSampleID,					/* 基板 ID		：ASCII 66 文字	*/	// 2010.03.04 K.Matsuo 80文字 - 14文字(yyyymmddhhmmss)です
		const MAIN_RCP_INFO* pMainRcpInfo		/* メインレシピ	*/
	)
{
	this->Logging(_T("Started Cta_SetCstSample()"));

	BOOL l_bRet = TRUE;
	char l_szCstID[80 + 2];
	memset(l_szCstID, 0, sizeof(l_szCstID));
	strncpy(l_szCstID, pstrCstID, 80);
	char l_szSampleID[80 + 2];
	memset(l_szSampleID, 0, sizeof(l_szSampleID));
	strncpy(l_szSampleID, pstrSampleID, 80);
// 2010.03.04 K.Matsuo FAMAS ID重複エラー対策 -->
	char l_szSystemTime[255 + 1];
	SYSTEMTIME l_systemTime;
// 2010.03.04 K.Matsuo FAMAS ID重複エラー対策 <--

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (-1 == m_CtaMeasInf.iCtaMode) {
		l_bRet = FALSE;	/* 測定モードが未設定です．	*/
	} else if (0 == strlen(l_szCstID)) {
		l_bRet = FALSE;	/* カセット ID が null です．	*/
	} else if (-1 != this->IsStrAlNum(l_szCstID)) {
		l_bRet = FALSE;	/* カセット ID に無効な文字があります．	*/
	} else if (0 == strlen(l_szSampleID)) {
		l_bRet = FALSE;	/* 基板 ID が null です．	*/
	} else if (-1 != this->IsStrAlNum(l_szSampleID)) {
		l_bRet = FALSE;	/* 基板 ID に無効な文字があります．	*/
	} else if (0 == pMainRcpInfo) {
		l_bRet = FALSE;	/* メインレシピ指定が nullです．	*/
	} else if ((RECIPE_NAME_LEN < _tcslen(pMainRcpInfo->MainRcpParam._CA.szRecalib[0]))
			|| (RECIPE_NAME_LEN < _tcslen(pMainRcpInfo->MainRcpParam._CA.szRecalib[1]))) {
		l_bRet = FALSE;	/* リキャリブ PGM 名が長すぎます．	*/
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		/* カセット ID で 80 文字に足りない部分を 0x20 で埋めます．	*/
		memset(m_CtaMeasInf.szCstID, 0x20, sizeof(m_CtaMeasInf.szCstID));
		m_CtaMeasInf.szCstID[80] = 0x00;
		memcpy(m_CtaMeasInf.szCstID, l_szCstID, strlen(l_szCstID));

// 2010.03.04 K.Matsuo FAMAS ID重複エラー対策 -->
// 基板IDに、日時を付加することで、IDが重複しないようにする
		GetLocalTime(&l_systemTime);
		sprintf(l_szSystemTime, "%04d%02d%02d%02d%02d%02d",
				l_systemTime.wYear,
				l_systemTime.wMonth,
				l_systemTime.wDay,
				l_systemTime.wHour,
				l_systemTime.wMinute,
				l_systemTime.wSecond);
		strcat(l_szSampleID, l_szSystemTime);
// 2010.03.04 K.Matsuo FAMAS ID重複エラー対策 <--
		/* 基板 ID で 80 文字に足りない部分を 0x20 で埋めます．	*/
		memset(m_CtaMeasInf.szSampleID, 0x20, sizeof(m_CtaMeasInf.szSampleID));
		m_CtaMeasInf.szSampleID[80] = 0x00;
		memcpy(m_CtaMeasInf.szSampleID, l_szSampleID, strlen(l_szSampleID));
/* modified 2009.11.16 hmenjo CTA リキャリブ有効チェック ---------- { ---------- */
//		/* リキャリブ PGM 名をコピーしておきます．	*/
//		if (0 != pMainRcpInfo->MainRcpParam._CA.RecalibItem[0]) {
//			_tcscpy(m_CtaMeasInf.tszRecalibCA, pMainRcpInfo->MainRcpParam._CA.szRecalib[0]);
//		} else {
//			_tcscpy(m_CtaMeasInf.tszRecalibCA, _T(""));
//		}
//		if (0 != pMainRcpInfo->MainRcpParam._CA.RecalibItem[1]) {
//			_tcscpy(m_CtaMeasInf.tszRecalibRad, pMainRcpInfo->MainRcpParam._CA.szRecalib[1]);
//		} else {
//			_tcscpy(m_CtaMeasInf.tszRecalibRad, _T(""));
//		}
/* modified 2009.11.16 hmenjo CTA リキャリブ有効チェック ---------- 			 */
		/* リキャリブ PGM 名をコピーしておきます．	*/
		_tcscpy(m_CtaMeasInf.tszRecalibCA, pMainRcpInfo->MainRcpParam._CA.szRecalib[0]);
		_tcscpy(m_CtaMeasInf.tszRecalibRad, pMainRcpInfo->MainRcpParam._CA.szRecalib[1]);
/* modified 2009.11.16 hmenjo CTA リキャリブ有効チェック ---------- } ---------- */

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_SetCstSample()"));

	return l_bRet;
}

/*
 *	CTA 基板厚み設定
 */
BOOL CCtaCtrl::Cta_SetThick(
		DWORD dwThick			/* 基板厚み情報[0.1mm]，範囲(0.0～10.0mm)	*/
	)
{
	this->Logging(_T("Started Cta_SetThick()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (-1 == m_CtaMeasInf.iCtaMode) {
		l_bRet = FALSE;	/* 測定モードが未設定です．	*/
	} else if (100 < dwThick) {
		l_bRet = FALSE;	/* 基板厚み情報が範囲外です．	*/
	} else if (0 == strlen(m_CtaMeasInf.szCstID)) {
		l_bRet = FALSE;	/* カセット ID が null です．	*/
	} else if (0 == strlen(m_CtaMeasInf.szSampleID)) {
		l_bRet = FALSE;	/* 基板 ID が null です．	*/
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		m_CtaMeasInf.dwThick = dwThick;

		l_bRet = this->WaitSeqEndStart(SEQPTN_SETINF);

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_SetThick()"));

	return l_bRet;
}

/*
 *	CTA 測定開始
 */
BOOL CCtaCtrl::Cta_Measure(
		int iScanCnt,							/* 測定番号	*/
		const STAGE_COORD_XYZ* pStageCoordXyz	/* 測定位置座標	*/
	)
{
	this->Logging(_T("Started Cta_Measure()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if ((iScanCnt <= 0) || (SCAN_POINT_CTA_MAX < iScanCnt)) {
		l_bRet = FALSE;
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		memset(&m_CtaMeasData, 0, sizeof(CTAMEASDATA));
		m_CtaMeasData.i_ScanCnt = iScanCnt;
		m_CtaMeasData.lScanX = pStageCoordXyz->lX;
		m_CtaMeasData.lScanY = pStageCoordXyz->lY;
		m_CtaMeasData.lScanZ = pStageCoordXyz->lZ;

		l_bRet = this->WaitSeqEndStart(SEQPTN_MEASSTA);

		if (0 != l_bRet) {
			/* 測定終了を通知します．	*/
			::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
			/* ココにリキャリブ処理を入れてもよいです．	*/
		}

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_Measure()"));

	return l_bRet;
}

/*
 *	CTA 測定終了
 */
BOOL CCtaCtrl::Cta_MeasEnd()
{
	this->Logging(_T("Started Cta_MeasEnd()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		l_bRet = this->WaitSeqEndStart(SEQPTN_MEASEND);

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_MeasEnd()"));

	return l_bRet;
}

/*
 *	CTA 強制終了
 */
BOOL CCtaCtrl::Cta_ForceEnd()
{
	this->Logging(_T("Started Cta_ForceEnd()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		m_bForceEnd = TRUE;

		::EnterCriticalSection(&m_csCtrlBusy);

		m_bForceEnd = FALSE;

		l_bRet = this->WaitSeqEndStart(SEQPTN_INIT);

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_ForceEnd()"));

	return l_bRet;
}

/*
 *	CTA 状態取得
 */
BOOL CCtaCtrl::Cta_GetStatus(long* plStatus)
{
	this->Logging(_T("Started Cta_GetStatus()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (0 == plStatus) {
		l_bRet = FALSE;
	} else if ((SEQPTN_NONE != m_dwSeqPattern) && (SEQPTN_MAX != m_dwSeqPattern)) {
		l_bRet = FALSE;
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		m_lCtaStatus = 0;

		l_bRet = this->WaitSeqEndStart(SEQPTN_GETSTAT);

		*plStatus = m_lCtaStatus;	/*	LOBYTE(LOWORD(m_lCtaStatus));	ヘッド：Z 軸位置
										HIBYTE(LOWORD(m_lCtaStatus));	状態１：測定状態
										LOBYTE(HIWORD(m_lCtaStatus));	状態２：装置状態
										HIBYTE(HIWORD(m_lCtaStatus));	(未使用)
										*/

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_GetStatus()"));

	return l_bRet;
}

/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	SIO 初期化処理
 */
BOOL CCtaCtrl::InitializeSIO()
{
	TCHAR l_tszNanoSpecIni[_MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	_stprintf(l_tszNanoSpecIni, _T("%s") _T(CFG_DIR) _T(NANOSPEC_INIFILENAME), g_tszProcDir);

	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if(g_lAppNameType != APP_NAME_NANO){
		strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}
	_stprintf(l_tszNanoSpecIni, _T("%s%s%s"), _T(CFG_DIR), g_tszProcDir, strFilename);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	TCHAR l_tszText[255];
	/* リトライカウント読出し	*/
	::GetPrivateProfileString(SECTION_CTA, _T("RETRY"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	m_dwRetryCnt = _ttoi(l_tszText);
	if (m_dwRetryCnt <= 0) {
		m_dwRetryCnt = 0;
	}
	/* HWS 読出し	*/
	::GetPrivateProfileString(SECTION_CTA, _T("HWS"), _T("1"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	int l_iHWS1 = _ttoi(l_tszText);
	::GetPrivateProfileString(SECTION_CTA, _T("HWS"), _T("2"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	int l_iHWS2 = _ttoi(l_tszText);
	if ((1 == l_iHWS1) && (2 == l_iHWS2)) {
		/* キーが無かった．	*/
		m_bHWS = (0 != bHwSimulation)? TRUE : FALSE;
	} else {
		m_bHWS = (0 != l_iHWS2)? TRUE : FALSE;
	}
	/* 会話タイムアウト(測定中)	*/
	m_uiTimeout_T9_Meas = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_T9_MEAS"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_T9_Meas < 500) || (CTA_TIMEOUT_T9_MEAS < m_uiTimeout_T9_Meas)) {
		m_uiTimeout_T9_Meas = CTA_TIMEOUT_T9_MEAS;
	}
	/* 会話タイムアウト(全ポイント終了受信待ち)	*/
	m_uiTimeout_T9_AllEnd = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_T9_ALLEND"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_T9_AllEnd < 500) || (CTA_TIMEOUT_T9_ALLEND < m_uiTimeout_T9_AllEnd)) {
		m_uiTimeout_T9_AllEnd = CTA_TIMEOUT_T9_ALLEND;
	}
	/* PO インタロック信号タイムアウト	*/
	m_uiTimeout_ILPO = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_ILPO"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_ILPO < 10) || (CTA_TIMEOUT_ILPO < m_uiTimeout_ILPO)) {
		m_uiTimeout_ILPO = CTA_TIMEOUT_ILPO;
	}
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
	/* PI インタロック信号タイムアウト	*/
	m_uiTimeout_ILPI = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_ILPI"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_ILPI < 10) || (CTA_TIMEOUT_ILPI < m_uiTimeout_ILPI)) {
		m_uiTimeout_ILPI = CTA_TIMEOUT_ILPI;
	}
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */
	/* リトライタイマ	*/
	m_uiTimeout_Retry = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_RETRY"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_Retry < 200) || (5000 < m_uiTimeout_Retry)) {
		m_uiTimeout_Retry = CTA_TIMEOUT_RETRY;
	}
	/* MPE 送信待ちウェイト	*/
	m_uiTimeout_MPE_Wait = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_MPE_WAIT"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_MPE_Wait < 10) || (5000 < m_uiTimeout_MPE_Wait)) {
		m_uiTimeout_MPE_Wait = CTA_TIMEOUT_MPE_WAIT;
	}
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
	/* PO CTA 認識ディレイ	*/
	m_uiDelay_ILPO = ::GetPrivateProfileInt(SECTION_CTA, _T("ILPO_DELAY"), -1, l_tszNanoSpecIni);
	if ((m_uiDelay_ILPO < 10) || (10000 < m_uiDelay_ILPO)) {
		m_uiDelay_ILPO = CTA_ILPO_DELAY;
	}
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */

// 2009.11.18 K.Matsuo -->
//	/* ポート番号読出し	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("COM"), _T("1"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iComNo = _ttoi(l_tszText);
//	if (l_iComNo <= 0) {
//		l_iComNo = 1;
//	}
//	/* ボーレート読出し	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("BaudRate"), _T("9600"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iBaudRate = _ttoi(l_tszText);
//	/* データ長読出し	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("ByteSize"), _T("8"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iByteSize = _ttoi(l_tszText);
//	/* パリティ読出し	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("Parity"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iParity = _ttoi(l_tszText);
//	switch (l_iParity) {
//	case 1:		l_iParity = ODDPARITY; break;
//	case 2:		l_iParity = EVENPARITY; break;
//	case 3:		l_iParity = MARKPARITY; break;
//	case 4:		l_iParity = SPACEPARITY; break;
//	case 0:
//	default:	l_iParity = NOPARITY; break;
//	}
//	/* ストップビット読出し	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("StopBits"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iStopBits = _ttoi(l_tszText);
//	switch (l_iStopBits) {
//	case 1:		l_iStopBits = ONE5STOPBITS; break;
//	case 2:		l_iStopBits = TWOSTOPBITS; break;
//	case 0:
//	default:	l_iStopBits = ONESTOPBIT; break;
//	}
//	if (TRUE != ((CCtaSio*) m_pclsCCtaSio)->Initialize(l_iComNo, l_iBaudRate, l_iByteSize, l_iParity, l_iStopBits, m_bHWS)) {
//		return FALSE;
//	}

	///// Get Config Data /////
	COM_SETTING comSetting;
	ConfigFile_GetNanoSpecIni(&comSetting, CONFIG_FILE_COM);

	if (TRUE != ((CCtaSio*) m_pclsCCtaSio)->Initialize(comSetting.CaComPort.szCommNo, comSetting.CaComPort.szBaudRate, comSetting.CaComPort.szByteSize, comSetting.CaComPort.szParity, comSetting.CaComPort.szStopBits, m_bHWS)) {
		return FALSE;
	}
// 2009.11.18 K.Matsuo <--

	return TRUE;
}

/*
 *	ログ
 */
void CCtaCtrl::Logging(LPCTSTR strLog)
{
	if (0 == strLog) {
		return;
	}

	CString l_strLogText;
	l_strLogText.Format(_T("CCtaCtrl : %s"), strLog);

	pLogFile->Logging(l_strLogText);
}

/*
 *	ログ - メッセージ受信用
 */
void CCtaCtrl::Logging_WinMsg(LPCTSTR strLog, WPARAM wParam, LPARAM lParam)
{
	CString l_strLogText;
	l_strLogText.Format(_T("Received %s (WPARAM = 0x%08x, LPARAM = 0x%08x)."), strLog, wParam, lParam);
	this->Logging(l_strLogText);
}

/*
 *	制御シーケンス
 */
int CCtaCtrl::CtrlSeq()
{
	int l_iRet = 0;

	/* CTA ユニットからの受信確認 ------------------------------------------*/
	PACKET_CODE l_enumPacketCode = PKT_MAX;
	COMMAND_CODE l_enumCmdCode = EVT_MAX;
	char l_szRcvData[PKT_DATA_LEN_MAX + 1];
	CTARCVDATA l_CtaRcvData;
	memset(&l_CtaRcvData, 0, sizeof(CTARCVDATA));
	if (0 != m_bRcvData) {
		/* 受信データ有り	*/
		m_bRcvData = FALSE;
		int l_iRslt =	((CCtaSio*) m_pclsCCtaSio)->GetRcvData(
								&l_enumPacketCode,
								&l_enumCmdCode,
								l_szRcvData
							);
		l_CtaRcvData.pdwPktCode = (LPDWORD) &l_enumPacketCode;
		l_CtaRcvData.pdwCmdCode = (LPDWORD) &l_enumCmdCode;
		l_CtaRcvData.pszRcvData = l_szRcvData;
		if (CTASIO_ERR_NORCVDATA == l_iRslt) {
			/* 受信データはありませんでした．	*/
		} else if (0 != l_iRslt) {
			/* 受信データエラーです．	*/
			if (PKT_EVT == *l_CtaRcvData.pdwPktCode) {
				this->SendErrRes(l_iRslt);	/* レスポンスを返します．	*/
			}
			/* 上位に通知	*/
			this->NotifyAlarm(l_iRslt);
		} else {
			/*	・イベントパケット受信
				・レスポンスパケット受信
				・エラーレスポンスパケット受信	*/
			/* 各シーケンスに受信通知	*/
			l_CtaRcvData.bRecved = TRUE;
			/* イベントパケットの場合はココでレスポンスを返します．*/
			if (PKT_EVT == l_enumPacketCode) {
				this->SendEvtRes(&l_CtaRcvData);
				/* エラー発生イベントの場合は，Chief への通知が必要です．
					ただし，リセットコード(000)と初期化中の場合は通知しません．	*/
				if (EVT_REP == l_enumCmdCode) {
					if ((0 != l_CtaRcvData.dwErrCode) && (SEQPTN_INIT != m_dwSeqPattern)) {
						/* 上位に通知	*/
						this->NotifyAlarm(l_CtaRcvData.dwErrCode);
					}
				}
			}
		}
	}

	/* 各シーケンス --------------------------------------------------------*/
	switch (m_dwSeqPattern) {
	case SEQPTN_NONE:		/* シーケンス無し	*/
		break;
	case SEQPTN_INIT:		/* 初期化	*/
		l_iRet = this->SeqInitCTA(&l_CtaRcvData);
		break;
	case SEQPTN_SETINF:		/* 情報設定	*/
		l_iRet = this->SeqSetInf(&l_CtaRcvData);
		break;
	case SEQPTN_MEASSTA:	/* 測定開始	*/
		l_iRet = this->SeqMeasStart(&l_CtaRcvData);
		break;
	case SEQPTN_MEASEND:	/* 測定終了	*/
		l_iRet = this->SeqMeasEnd(&l_CtaRcvData);
		break;
	case SEQPTN_GETSTAT:	/* 状態取得	*/
		l_iRet = this->SeqGetStat(&l_CtaRcvData);
		break;
	default:
		ASSERT(0);
		m_dwSeqPattern = SEQPTN_NONE;
		break;
	}

	/* その他 --------------------------------------------------------------*/
	/* ココまでに受信通知が処理されなかった場合は，
		不要と判断して捨てます．	*/
	if (TRUE == l_CtaRcvData.bRecved) {
		l_CtaRcvData.bRecved = FALSE;
	}

	/* 受信データがまだある場合は制御シーケンスをキックします．	*/
	if (0 == ((CCtaSio*) m_pclsCCtaSio)->IsExistRcvData()) {
		/* 受信データがあった．	*/
		m_bRcvData = TRUE;
		this->KickCtrlSeq(1);
	}

	return l_iRet;
}

/*
 *	シーケンスの起動指令と終了待ち
 */
BOOL CCtaCtrl::WaitSeqEndStart(SEQ_PATTERN enumSeqPattern)
{
	BOOL l_bRet = TRUE;

	if ((enumSeqPattern <= SEQPTN_NONE) || (SEQPTN_MAX <= enumSeqPattern)) {
		this->Logging(_T("Invalid Seq Pattern in WaitSeqEndStart()."));
		return FALSE;
	}

	m_hEvWaitEndSeq = ::CreateEvent(0, TRUE, FALSE, 0);
	if (0 == m_hEvWaitEndSeq) {
		this->Logging(_T("Failed to create event - m_hEvWaitEndSeq in WaitSeqEndStart()."));
		return FALSE;
	}

	/* 制御シーケンスをキック	*/
	m_dwSeqPattern = enumSeqPattern;
	m_iSeqResult = 0;
	if (0 == this->KickCtrlSeq(2)) {
		this->Logging(_T("Failed to KickCtrlSeq() in WaitSeqEndStart()."));
		l_bRet = FALSE;
	}

	if (TRUE == l_bRet) {
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvWaitEndSeq, SEQWAIT_TIMEOUT)) {
			/* タイムアウトしました．	*/
			this->Logging(_T("Timeout WaitForSingleObject() in WaitSeqEndStart()."));
			l_bRet = FALSE;
		} else {
			if (0 != m_iSeqResult) {
				l_bRet = FALSE;
			}
		}
	}

	if (0 == ::ResetEvent(m_hEvWaitEndSeq)) {
		this->Logging(_T("Failed to reset event - m_hEvWaitEndSeq in WaitSeqEndStart()."));
		l_bRet = FALSE;
	}
	if (0 == ::CloseHandle(m_hEvWaitEndSeq)) {
		this->Logging(_T("Failed to close event handle in WaitSeqEndStart()."));
		return FALSE;
	}

	return l_bRet;
}

/*
 *	制御シーケンスをキック
 */
BOOL CCtaCtrl::KickCtrlSeq(int iKickNo/* = -1*/)
{
	BOOL l_bRet = TRUE;

	TCHAR l_tszLogMsg[80];
	_stprintf(l_tszLogMsg, _T("Post thread message WM_CTACTL_SEQKICK in KickCtrlSeq(%d)."), iKickNo);
	this->Logging(l_tszLogMsg);

	if (0 == this->PostThreadMessage(WM_CTACTL_SEQKICK, 0, 0)) {
		this->Logging(_T("Failed to PostThreadMessage() in KickCtrlSeq()."));
		l_bRet = FALSE;
	}

	return l_bRet;
}

/*
 *	タイマ コールバック関数
 */
void CALLBACK TimerHandler(
		HWND		hWnd,		/* ウィンドウのハンドル	*/
		UINT		uiMsg,		/* WM_TIMER メッセージ	*/
		UINT_PTR	idEvent,	/* タイマの識別子		*/
		DWORD		dwTime		/* 現在のシステム時刻	*/
	)
{
	if (WM_TIMER == uiMsg) {
		if (lgs_uiTimerID == idEvent) {
			if (0 == ::KillTimer(0, lgs_uiTimerID)) {ASSERT(0);}
			if (0 != lgs_pclsCCtaCtrl) {
				lgs_pclsCCtaCtrl->PostThreadMessage(WM_CTACTL_TIMER, 0, 0);
			}
		}
	}
}

/*
 *	タイムアウト監視 設定/解除
 */
BOOL CCtaCtrl::SetTimeOut(BOOL bMode, UINT uiElapse)
{
	BOOL l_bRet = TRUE;

	if (0 == bMode) {
		/* 解除	*/
		if ((0 != lgs_uiTimerID) && (0 == m_bTimeOut)) {
			BOOL l_bRstl = ::KillTimer(0, lgs_uiTimerID);
			lgs_uiTimerID = 0;
			m_bTimeOut = FALSE;
			if (0 == l_bRstl) {
				l_bRet = FALSE;
//				ASSERT(0);	/* 不要です．	*/
			}
		}
	} else {
		/* 設定(開始)	*/
		m_bTimeOut = FALSE;
		if (0 == uiElapse) {
			l_bRet = FALSE;
		} else {
			lgs_uiTimerID = ::SetTimer(0, 0, uiElapse, (TIMERPROC) TimerHandler);
			if (0 == lgs_uiTimerID) {
				l_bRet = FALSE;
				ASSERT(0);
			}
		}
	}

	return l_bRet;
}

/*
 *	受信データ部解析
 */
int CCtaCtrl::AnaRcvData(LPCTARCVDATA pCtaRcvData, LPCTADATAINF pCtaDataInf)
{
	int l_iRet = 0;

	if (0 == pCtaRcvData) {
		l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
	} else if (0 == pCtaRcvData->bRecved) {
		l_iRet = CTACTL_ERR_DATANONE;	/* データはありません．	*/
	} else {
		if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
			/* エラーレスポンス --------------------------------------------*/
			if (3 != strlen(pCtaRcvData->pszRcvData)) {
				l_iRet = CTACTL_ERR_DATAFORMAT;	/* データ(フォーマット)エラー	*/
			} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
				l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
			} else {
				if (0 != pCtaDataInf) {
					pCtaDataInf->ResERR.dwErrRes = atol(pCtaRcvData->pszRcvData);
				}
			}
		} else {
			/* レスポンス，または，イベント --------------------------------*/
			switch (*pCtaRcvData->pdwCmdCode) {
			case CMD_STA:	/* STA レスポンス ------------------------------*/
				if (3 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* データ(フォーマット)エラー	*/
				} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
				} else {
					DWORD l_dwZState = pCtaRcvData->pszRcvData[0] & 0x0f;
					DWORD l_dwStatus1 = pCtaRcvData->pszRcvData[1] & 0x0f;
					DWORD l_dwStatus2 = pCtaRcvData->pszRcvData[2] & 0x0f;
					if ((l_dwZState < 1) || (3 < l_dwZState)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
					} else if ((l_dwStatus1 < 1) || (3 < l_dwStatus1)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
					} else if ((l_dwStatus2 < 1)
							|| (4 < l_dwStatus2)
							|| (2 == l_dwStatus2)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
					} else {
						if (0 != pCtaDataInf) {
							pCtaDataInf->CmdSTA.dwZState = l_dwZState;
							pCtaDataInf->CmdSTA.dwStatus1 = l_dwStatus1;
							pCtaDataInf->CmdSTA.dwStatus2 = l_dwStatus2;
						}
					}
				}
				break;
			case CMD_RST:	/* RST レスポンス ------------------------------*/
			case CMD_ORG:	/* ORG レスポンス ------------------------------*/
			case CMD_MOD:	/* MOD レスポンス ------------------------------*/
			case CMD_CID:	/* CID レスポンス ------------------------------*/
			case CMD_PID:	/* PID レスポンス ------------------------------*/
			case CMD_SPT:	/* SPT レスポンス ------------------------------*/
			case CMD_MST:	/* MST レスポンス ------------------------------*/
			case CMD_MPE:	/* MPE レスポンス ------------------------------*/
			case CMD_MCE:	/* MCE レスポンス ------------------------------*/
			case EVT_CMO:	/* CMO イベント --------------------------------*/
			case EVT_CST:	/* CST イベント --------------------------------*/
			case EVT_CMA:	/* CMA イベント --------------------------------*/
				if (0 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* データ(フォーマット)エラー	*/
				}
				break;
			case EVT_REP:	/* REP イベント --------------------------------*/
				if (3 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* データ(フォーマット)エラー	*/
				} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
				} else {
					if (0 != pCtaDataInf) {
						pCtaDataInf->EvtREP.dwErrCode = atol(pCtaRcvData->pszRcvData);
					}
				}
				break;
			case EVT_CMP:	/* CMP イベント --------------------------------*/
				if (17 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* データ(フォーマット)エラー	*/
				} else {
					struct {
						char l_szPointNo[8];	/* 4 ポイント No.	*/
						char l_szCAngle[8];		/* 4 接触角[0.1degree]	*/
						char l_szRadius[8];		/* 5 半径[0.1um]	*/
						char l_szLiquid[8];		/* 4 液量[0.01uL]	*/
					} l_DataTmp;
					memset(&l_DataTmp, 0, sizeof(l_DataTmp));
					memcpy(l_DataTmp.l_szPointNo, &(pCtaRcvData->pszRcvData[0]), 4);
					memcpy(l_DataTmp.l_szCAngle, &(pCtaRcvData->pszRcvData[4]), 4);
					memcpy(l_DataTmp.l_szRadius, &(pCtaRcvData->pszRcvData[8]), 5);
					memcpy(l_DataTmp.l_szLiquid, &(pCtaRcvData->pszRcvData[13]), 4);
					char *pBlankPos;
					int l_iPointNo;
					if (0 == l_iRet) {	/* ポイント No.	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szPointNo)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szPointNo, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iPointNo = atol(l_DataTmp.l_szPointNo);
							if ((l_iPointNo <= 0) || (SCAN_POINT_CTA_MAX < l_iPointNo)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
							}
						}
					}
					int l_iCAngle;
					if (0 == l_iRet) {	/* 接触角	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szCAngle)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szCAngle, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iCAngle = atol(l_DataTmp.l_szCAngle);
							if ((l_iCAngle < 0) || (9999 < l_iCAngle)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
							}
						}
					}
					int l_iRadius;
					if (0 == l_iRet) {	/* 半径	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szRadius)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szRadius, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iRadius = atol(l_DataTmp.l_szRadius);
							if ((l_iRadius < 0) || (99999 < l_iRadius)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
							}
						}
					}
					int l_iLiquid;
					if (0 == l_iRet) {	/* 液量	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szLiquid)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szLiquid, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iLiquid = atol(l_DataTmp.l_szLiquid);
							if ((l_iLiquid < 0) || (9999 < l_iLiquid)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* パラメタエラー	*/
							}
						}
					}
					if (0 == l_iRet) {	/* すべて正常なのでコピー	*/
						pCtaDataInf->EvtCMP.iPointNo = l_iPointNo;
						pCtaDataInf->EvtCMP.iCAngle = l_iCAngle;
						pCtaDataInf->EvtCMP.iRadius = l_iRadius;
						pCtaDataInf->EvtCMP.iLiquid = l_iLiquid;
					}
				}
				break;
			default:
				ASSERT(0);
				break;
			}
		}
	}

	return l_iRet;
}

/*
 *	文字列の１０進数チェック
 *		戻り値：	-1：すべて０(0x30)～９(0x39)でした．
 *					以外：１０進数以外の文字位置(オフセット)
 */
int CCtaCtrl::IsStrDecimal(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == isdigit(pstrSrc[i])) {
			l_iRet = i;
			break;
		}
		i++;
	}

	return l_iRet;
}

/*
 *	文字列の１０進数チェック符号付き
 *		１０進数か符号(+/-：先頭文字のみ)かブランク(最後の方のみ)をチェックします．
 *		戻り値：	-1：すべて０(0x30)～９(0x39)でした．
 *					以外：１０進数以外の文字位置(オフセット)
 */
int CCtaCtrl::IsStrDecimalSign(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	BOOL l_bFoundBlank = FALSE;
	BOOL l_bSign = FALSE;
	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == i) {
			/* １桁目は数字(0～9)か符号(+/-)以外はエラー	*/
			if (0 != isdigit(pstrSrc[i])) {
				;	/* 数字だった	*/
			} else if (('+' != pstrSrc[i]) && ('-' != pstrSrc[i])) {
				l_bSign = TRUE;	/* 符号だった	*/
			} else {
				l_iRet = i;
				break;
			}
		} else if (1 == i) {
			/* ２桁目	*/
			if (0 != l_bSign) {
				/* １桁目が符号だった場合	*/
				if (0 == isdigit(pstrSrc[i])) {
					/* ２桁目は数字でなければならない	*/
					l_iRet = i;
					break;
				}
			} else if (0 != isdigit(pstrSrc[i])) {
				;	/* 数字だった	*/
			} else if (' ' == pstrSrc[i]) {
				l_bFoundBlank = TRUE;	/* ブランク(0x20)だった	*/
			} else {
				l_iRet = i;
				break;
			}
		} else {
			/* ３桁目以降	*/
			if (0 != l_bFoundBlank) {
				/* 以前にブランクがあった場合	*/
				if (' ' != pstrSrc[i]) {
					/* ブランクであり続けなければならない	*/
					l_iRet = i;
					break;
				}
			} else if (0 != isdigit(pstrSrc[i])) {
				;	/* 数字だった	*/
			} else if (' ' == pstrSrc[i]) {
				l_bFoundBlank = TRUE;	/* ブランク(0x20)だった	*/
			} else {
				l_iRet = i;
				break;
			}
		}
		i++;
	}

	return l_iRet;
}

/*
 *	文字列の英数チェック
 *		戻り値：	-1：すべて０(0x30)～９(0x39)，A(0x41)～Z(0x5a)，a(0x61)～z(0x7a)でした．
 *					以外：英数以外の文字位置(オフセット)
 */
int CCtaCtrl::IsStrAlNum(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == isalnum(pstrSrc[i])) {
			l_iRet = i;
			break;
		}
		i++;
	}

	return l_iRet;
}

/*
 *	エラーレスポンス送信
 */
int CCtaCtrl::SendErrRes(int iResult)
{
	int l_iRet = 0;

	int l_iErrCode = 0;
	switch (iResult) {
	case CTASIO_ERR_INVALID_PKT:	/* パケットコードが範囲外です	*/
		l_iErrCode = CTACTL_ERCD_HEAD;	/* ヘッダエラー	*/
		break;
	case CTASIO_ERR_INVALID_CMD:	/* コマンドコードが範囲外です	*/
	case CTASIO_ERR_INVALID_EVT:	/* イベントコードが範囲外です	*/
	case CTASIO_ERR_CTRLCODE:		/* 制御コードがあった	*/
	case CTACTL_ERR_DATAFORMAT:		/* データ(フォーマット)エラー	*/
	case CTACTL_ERR_PARAMERR:		/* パラメタエラー	*/
		l_iErrCode = CTACTL_ERCD_CMD;	/* コマンドエラー	*/
		break;
	case CTASIO_ERR_SUM:			/* チェックサム不一致	*/
		l_iErrCode = CTACTL_ERCD_SUM;	/* チェックサムエラー	*/
		break;
	case CTASIO_ERR_TOOSHORT:		/* 受信データ長が短すぎます	*/
	case CTASIO_ERR_INVALIDLEN:		/* 受信データ長異常	*/
	case CTASIO_ERR_NOLF:			/* LF がありません	*/
	case CTASIO_ERR_NOCR:			/* CR がありません	*/
		l_iErrCode = CTACTL_ERCD_LEN;	/* 受信データ長エラー	*/
		break;
		break;
	default:
		/* 未定義結果は何もしません．	*/
		break;
	}

	if (0 != l_iErrCode) {
		l_iErrCode = l_iErrCode % 1000;
		char l_szData[4];
		sprintf(l_szData, "%03d", l_iErrCode);
		l_iRet = ((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_ERR, CMD_STA, l_szData);
	}

	return l_iRet;
}

/*
 *	イベントに対するレスポンスを送信します．
 */
int CCtaCtrl::SendEvtRes(LPCTARCVDATA pCtaRcvData)
{
	int l_iRet = 0;

	CTADATAINF l_CtaDataInf;
	int l_iRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
	pCtaRcvData->iRslt = l_iRslt;
	pCtaRcvData->bAnaed = TRUE;
	if (0 != l_iRslt) {
		/* エラーレスポンスで応答	*/
		l_iRet = this->SendErrRes(l_iRslt);
	} else {
		/* 正常レスポンス応答	*/
		l_iRet = ((CCtaSio*) m_pclsCCtaSio)->CmdSend(PKT_RES, (COMMAND_CODE) *pCtaRcvData->pdwCmdCode, "");
		if (EVT_REP == *pCtaRcvData->pdwCmdCode) {
			pCtaRcvData->dwErrCode = l_CtaDataInf.EvtREP.dwErrCode;
/* added 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- { ---------- */
			if (CTACTL_ERCD_RST == pCtaRcvData->dwErrCode) {
				m_bErrRep = FALSE;
			} else {
				m_bErrRep = TRUE;
			}
/* added 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- } ---------- */
		}
	}

	if (0 == l_iRet) {
		l_iRet = pCtaRcvData->iRslt;
	}

	return l_iRet;
}

/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
/*
 *	シーケンス：初期化
 */
#define	PRC_INIT_CMD_RST0	(PRC_INIT_IDLE + 100)
#define	PRC_INIT_CMD_STA	(PRC_INIT_IDLE + 200)
#define	PRC_INIT_CMD_RST	(PRC_INIT_IDLE + 300)
#define	PRC_INIT_CMD_ORG	(PRC_INIT_IDLE + 400)
#define	PRC_INIT_EVT_CMO	(PRC_INIT_IDLE + 500)
#define	PRC_INIT_ILPI_ON	(PRC_INIT_IDLE + 600)
int CCtaCtrl::SeqInitCTA(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = PRC_INIT_CMD_STA;
//	static int ls_iPrc = PRC_INIT_CMD_RST0;
///* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- { ---------- */
////	static BOOL ls_bRST2nd = FALSE;
///* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ----------              */
//	static BOOL ls_bRST2nd = TRUE;
///* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- } ---------- */
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_INIT_CMD_RST0 + 00:	/* トラブルリセット ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqInitCTA()"));
			nexioCA_Interlock(TRUE);	/* CTAILPO オン	*/
			::Sleep(m_uiTimeout_ILPO);
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_RST, "");
		ls_iPrc = PRC_INIT_CMD_RST0 + 10;
		break;
	case PRC_INIT_CMD_RST0 + 10:	/* トラブルリセットのレスポンス受信待ち ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要性を検討すること・・・	*/
				}
			} else if (CMD_RST == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					/* フォーマット系エラーだった	*/
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					ls_iPrc = PRC_INIT_CMD_STA;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_RST0;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_INIT_CMD_STA + 00:		/* 状態要求 --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqInitCTA()"));
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_STA, "");
		ls_iPrc = PRC_INIT_CMD_STA + 10;
		break;
	case PRC_INIT_CMD_STA + 10:		/* 状態要求のレスポンス受信待ち ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要性を検討すること・・・	*/
				}
			} else if (CMD_STA == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else if ((1 != l_CtaDataInf.CmdSTA.dwStatus1) && (2 != l_CtaDataInf.CmdSTA.dwStatus1)) {
					/* 「測定中：１」か「待機中：２」以外の場合	*/
					l_iRslt = CTACTL_ERR_EQBUSY;
				} else if ((1 != l_CtaDataInf.CmdSTA.dwStatus2) && (3 != l_CtaDataInf.CmdSTA.dwStatus2)) {
					/* 「１：エラー無し」か「ヘッドエラー：３」以外の場合	*/
					l_iRslt = CTACTL_ERR_EQERR;
				} else {
					ls_dwRetryCnt = 0;
/* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- { ---------- */
//					ls_iPrc = PRC_INIT_CMD_RST;
/* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ----------              */
					if (FALSE != m_bErrRep) {
						ls_iPrc = PRC_INIT_CMD_RST;
					} else {
						ls_iPrc = PRC_INIT_CMD_ORG;
					}
/* modified 2010.07.15 hmenjo PO オンで RST コマンド対応 ---------- } ---------- */
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_STA;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_INIT_CMD_RST + 00:		/* トラブルリセット前の ILPO オン ----------------------------- */
		ls_iPrc = PRC_INIT_CMD_RST + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);
		nexioCA_Interlock(TRUE);		/* CTAILPO オン	*/
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_INIT_CMD_RST + 10:		/* トラブルリセット前の ILPI オンチェック --------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* タイムアウト監視 解除	*/
			ls_iPrc = PRC_INIT_CMD_RST + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_INIT_CMD_RST + 20:		/* トラブルリセット ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_RST, "");
		ls_iPrc = PRC_INIT_CMD_RST + 30;
		break;
	case PRC_INIT_CMD_RST + 30:		/* トラブルリセットのレスポンス受信待ち ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要性を検討すること・・・	*/
				}
			} else if (CMD_RST == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					/* フォーマット系エラーだった	*/
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_NORM);	/* (会話)タイムアウト監視 開始	*/
					ls_iPrc = PRC_INIT_CMD_RST + 40;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_RST + 20;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_INIT_CMD_RST + 40:		/* エラー発生イベント受信待ち --------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_REP == *pCtaRcvData->pdwCmdCode) {
				/* エラー発生イベントだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
					l_bSeqEnd = TRUE;	/* シーケンス終了	*/
				} else
				if (CTACTL_ERCD_RST == l_CtaDataInf.EvtREP.dwErrCode) {
					ls_iPrc = PRC_INIT_CMD_ORG;
					ls_dwRetryCnt = 0;
					this->KickCtrlSeq(l_iPrcRec + 1);
				} else {
					/* インタロック異常です．	*/
					l_iRslt = CTACTL_ERR_ILPO;
					l_bSeqEnd = TRUE;	/* シーケンス終了	*/
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		}
		break;
	case PRC_INIT_CMD_ORG + 00:		/* 原点復帰前の ILPO オン ------------------------------------- */
		ls_iPrc = PRC_INIT_CMD_ORG + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);
		nexioCA_Interlock(TRUE);		/* CTAILPO オン	*/
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_INIT_CMD_ORG + 10:		/* 原点復帰前の ILPI オンチェック ----------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* タイムアウト監視 解除	*/
			ls_iPrc = PRC_INIT_CMD_ORG + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_INIT_CMD_ORG + 20:		/* 原点復帰 --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_ORG, "");
		ls_iPrc = PRC_INIT_CMD_ORG + 30;
		break;
	case PRC_INIT_CMD_ORG + 30:		/* 原点復帰のレスポンス受信待ち ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要性を検討すること・・・	*/
				}
			} else if (CMD_ORG == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_ORG);	/* (会話)タイムアウト監視 開始	*/
					ls_iPrc = PRC_INIT_EVT_CMO;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_ORG + 20;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_INIT_EVT_CMO + 00:		/* 移動完了イベント受信待ち ----------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMO == *pCtaRcvData->pdwCmdCode) {
				/* 移動完了イベントだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
					l_bSeqEnd = TRUE;	/* シーケンス終了	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);	/* IL タイムアウト監視 開始	*/
					ls_iPrc = PRC_INIT_ILPI_ON;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		}
		break;
	case PRC_INIT_ILPI_ON + 00:		/* CTAILPI オン待ち ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO オフ	*/
			this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* 強制終了	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* シーケンス終了	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO オフ	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqInitCTA() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = PRC_INIT_CMD_STA;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */

/*
 *	シーケンス：情報設定
 */
int CCtaCtrl::SeqSetInf(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = 0;
	static DWORD ls_dwRetryCnt = 0;
	static COMMAND_CODE ls_enumCmdCode = CMD_MOD;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	char l_szSendData[PKT_DATA_LEN_MAX + 1];

	switch (ls_iPrc) {
	case 0:		/* コマンド送信	*/
		switch (ls_enumCmdCode) {
		case CMD_MOD:		/* 測定モード	*/
		default:
			switch (m_CtaMeasInf.iCtaMode) {
			case MEAS_CTA_MODE_1:
			case MEAS_CTA_MODE_2:
			case MEAS_CTA_MODE_3:
				break;
			default:
				m_CtaMeasInf.iCtaMode = MEAS_CTA_MODE_1;
				break;
			}
			l_szSendData[0] = (m_CtaMeasInf.iCtaMode & 0x0f) | 0x30;
			l_szSendData[1] = 0x00;
			break;
		case CMD_CID:		/* カセット情報セット	*/
			strcpy(l_szSendData, m_CtaMeasInf.szCstID);
			break;
		case CMD_PID:		/* 基板情報セット	*/
			strcpy(l_szSendData, m_CtaMeasInf.szSampleID);
			break;
		case CMD_SPT:		/* 基板厚み情報セット	*/
			sprintf(l_szSendData, "%d", m_CtaMeasInf.dwThick);
			l_szSendData[3] = 0x00;
			if (0x00 == l_szSendData[1]) {
				l_szSendData[1] = ' ';
				l_szSendData[2] = ' ';
			}
			if (0x00 == l_szSendData[2]) {
				l_szSendData[2] = ' ';
			}
			break;
		}
		if (0 == ls_dwRetryCnt) {
			if (CMD_MOD == ls_enumCmdCode) {this->Logging(_T("Started SeqSetInf()"));}
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, ls_enumCmdCode, l_szSendData);
		ls_iPrc = 1;
		break;
	case 1:		/* コマンドのレスポンス受信待ち	*/
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要を検討すること・・・	*/
				}
			} else if (ls_enumCmdCode == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					ls_dwRetryCnt = 0;
					switch (ls_enumCmdCode) {
					case CMD_MOD:
					default:		ls_enumCmdCode = CMD_CID; ls_iPrc = 0;	break;
					case CMD_CID:	ls_enumCmdCode = CMD_PID; ls_iPrc = 0;	break;
					case CMD_PID:	ls_enumCmdCode = CMD_SPT; ls_iPrc = 0;	break;
					case CMD_SPT:	ls_enumCmdCode = CMD_MAX; ls_iPrc = 2;	break;
					}
					if (CMD_MAX == ls_enumCmdCode) {
						this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_NORM);	/* (会話)タイムアウト監視 開始	*/
					} else {
						this->KickCtrlSeq(211);
					}
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = 0;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(212);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case 2:		/* 設定完了イベント受信待ち	*/
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CST == *pCtaRcvData->pdwCmdCode) {
				/* 移動完了イベントだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				}
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* 強制終了	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* シーケンス終了	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqSetInf() [m_iSeqResult = %d, ls_iPrc = %d, ls_enumCmdCode = %d]"), m_iSeqResult, ls_iPrc, ls_enumCmdCode);
		this->Logging(l_strLog);
		ls_enumCmdCode = CMD_MOD;
		ls_iPrc = 0;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}

/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
/*
 *	シーケンス：測定開始
 */
#define	PRC_MEAS_CMD_MST	(PRC_MEAS_IDLE + 100)
#define	PRC_MEAS_EVT_CMP	(PRC_MEAS_IDLE + 200)
#define	PRC_MEAS_ILPI_ON	(PRC_MEAS_IDLE + 300)
int CCtaCtrl::SeqMeasStart(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = PRC_MEAS_CMD_MST;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_MEAS_CMD_MST + 00:		/* 測定開始前の ILPO オン ------------------------------------- */
		this->Logging(_T("Started SeqMeasStart()"));
		ls_iPrc = PRC_MEAS_CMD_MST + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI);
		nexioCA_Interlock(TRUE);		/* CTAILPO オン	*/
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_MEAS_CMD_MST + 10:		/* 測定開始前の ILPI オンチェック ----------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* タイムアウト監視 解除	*/
			ls_iPrc = PRC_MEAS_CMD_MST + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_MEAS_CMD_MST + 20:		/* 測定開始 --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
			m_bRecvedCMA = FALSE;
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MST, "");
		ls_iPrc = PRC_MEAS_CMD_MST + 30;
		break;
	case PRC_MEAS_CMD_MST + 30:		/* 測定開始のレスポンス受信待ち ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要を検討すること・・・	*/
				}
			} else if (CMD_MST == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_T9_Meas);	/* (会話)タイムアウト監視 開始	*/
					ls_iPrc = PRC_MEAS_EVT_CMP;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEAS_CMD_MST + 20;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_MEAS_EVT_CMP + 00:		/* １ポイント測定完了イベント受信待ち ------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMP == *pCtaRcvData->pdwCmdCode) {
				/* １ポイント測定完了イベントだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
					l_bSeqEnd = TRUE;	/* シーケンス終了	*/
				} else {
					/* データセット ----------------------------------------*/
					CTAPOINTDATA l_CtaPointData;
					l_CtaPointData.iPointNo = l_CtaDataInf.EvtCMP.iPointNo;
					l_CtaPointData.dCAngle = ((double) l_CtaDataInf.EvtCMP.iCAngle) / 10.0;
					l_CtaPointData.dRadius = ((double) l_CtaDataInf.EvtCMP.iRadius) / 10.0;
					l_CtaPointData.dLiquid = ((double) l_CtaDataInf.EvtCMP.iLiquid) / 100.0;
					/* リキャリブレーションします．	*/
/* modified 2009.11.16 hmenjo CTA リキャリブ有効チェック ---------- { ---------- */
//					if (0 < _tcslen(m_CtaMeasInf.tszRecalibCA)) {
//						/* 接触角のリキャリブ指定有り	*/
//						if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibCA, l_CtaPointData.dCAngle)) {
//							l_iRslt = CTACTL_ERR_RECALIB;	/* リキャリブエラー*/
//						}
//					}
//					if ((0 == l_iRslt)
//					 && (0 < _tcslen(m_CtaMeasInf.tszRecalibRad))) {
//						/* 半径のリキャリブ指定有り	*/
//						if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibRad, l_CtaPointData.dRadius)) {
//							l_iRslt = CTACTL_ERR_RECALIB;	/* リキャリブエラー*/
//						}
//					}
/* modified 2009.11.16 hmenjo CTA リキャリブ有効チェック ---------- 			 */
					/* 接触角のリキャリブ指定有り	*/
					if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibCA, l_CtaPointData.dCAngle)) {
						l_iRslt = CTACTL_ERR_RECALIB;	/* リキャリブエラー*/
					} else
					/* 半径のリキャリブ指定有り	*/
					if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibRad, l_CtaPointData.dRadius)) {
						l_iRslt = CTACTL_ERR_RECALIB;	/* リキャリブエラー*/
					}
/* modified 2009.11.16 hmenjo CTA リキャリブ有効チェック ---------- } ---------- */
					if (0 != l_iRslt) {
						l_bSeqEnd = TRUE;	/* シーケンス終了	*/
					} else {
						/* 測定結果 DB にセット	*/
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->iPointNo = l_CtaPointData.iPointNo;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dCAngle = l_CtaPointData.dCAngle;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dRadius = l_CtaPointData.dRadius;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dLiquid = l_CtaPointData.dLiquid;

						this->SetTimeOut(TRUE, m_uiTimeout_ILPI);	/* IL タイムアウト監視 開始	*/
						ls_iPrc = PRC_MEAS_ILPI_ON;
						this->KickCtrlSeq(l_iPrcRec + 1);
					}
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		}
		break;
	case PRC_MEAS_ILPI_ON + 00:		/* CTAILPI オン待ち ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO オフ	*/
			this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* 強制終了	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* シーケンス終了	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO オフ	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqMeasStart() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = PRC_MEAS_CMD_MST;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */

/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- { ---------- */
/*
 *	シーケンス：測定終了
 */
#define	PRC_MEND_CMD_MPE	(PRC_MEND_IDLE + 100)
#define	PRC_MEND_EVT_CMO	(PRC_MEND_IDLE + 200)
#define	PRC_MEND_EVT_CMA	(PRC_MEND_IDLE + 300)
#define	PRC_MEND_CMD_MCE	(PRC_MEND_IDLE + 400)
#define	PRC_MEND_ILPI_ON	(PRC_MEND_IDLE + 500)
int CCtaCtrl::SeqMeasEnd(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = PRC_MEND_CMD_MPE;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_MEND_CMD_MPE + 00:		/* 基板終了通知前の ILPO オン --------------------------------- */
		this->Logging(_T("Started SeqMeasEnd()"));
		::Sleep(m_uiTimeout_MPE_Wait);
		ls_iPrc = PRC_MEND_CMD_MPE + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI);
		nexioCA_Interlock(TRUE);		/* CTAILPO オン	*/
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO 後ディレイ ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_MEND_CMD_MPE + 10:		/* 基板終了通知前の ILPI オンチェック ------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* タイムアウト監視 解除	*/
			ls_iPrc = PRC_MEND_CMD_MPE + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_MEND_CMD_MPE + 20:		/* 基板終了通知 ----------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
			m_bRecvedCMA = FALSE;
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MPE, "");
		ls_iPrc = PRC_MEND_CMD_MPE + 30;
		break;
	case PRC_MEND_CMD_MPE + 30:		/* 基板終了通知のレスポンス受信待ち --------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要を検討すること・・・	*/
				}
			} else if (CMD_MPE == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_ORG);	/* (会話)タイムアウト監視 開始	*/
					ls_iPrc = PRC_MEND_EVT_CMO;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEND_CMD_MPE + 20;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_MEND_EVT_CMO + 00:		/* 移動完了イベント受信待ち ----------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMO == *pCtaRcvData->pdwCmdCode) {
				/* 移動完了イベントだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
					l_bSeqEnd = TRUE;	/* シーケンス終了	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_T9_AllEnd);	/* (会話)タイムアウト監視 開始	*/
					ls_iPrc = PRC_MEND_EVT_CMA;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		}
		break;
	case PRC_MEND_EVT_CMA + 00:		/* 全ポイント測定完了イベント受信＆レスポンス送信完了待ち ----- */
		if (0 != m_bRecvedCMA) {
			m_bRecvedCMA = FALSE;
			this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
			ls_dwRetryCnt = 0;
			ls_iPrc = PRC_MEND_CMD_MCE;
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		}
		break;
	case PRC_MEND_CMD_MCE + 00:		/* カセット終了通知 ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MCE, m_CtaMeasInf.szCstID);
		ls_iPrc = PRC_MEND_CMD_MCE + 10;
		break;
	case PRC_MEND_CMD_MCE + 10:		/* カセット終了通知のレスポンス受信待ち ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要を検討すること・・・	*/
				}
			} else if (CMD_MCE == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_ILPI);	/* IL タイムアウト監視 開始	*/
					ls_iPrc = PRC_MEND_ILPI_ON;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEND_CMD_MCE;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	case PRC_MEND_ILPI_ON + 00:		/* CTAILPI オン待ち ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO オフ	*/
			this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
			l_bSeqEnd = TRUE;	/* シーケンス終了	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* 強制終了	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* シーケンス終了	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO オフ	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqMeasEnd() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = PRC_MEND_CMD_MPE;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}
/* added 2010.09.09 hmenjo CTA PO/PI ハンドシェーク ---------- } ---------- */
/*
 *	シーケンス：状態取得
 */
int CCtaCtrl::SeqGetStat(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = 0;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;

	switch (ls_iPrc) {
	case 0:		/* 状態要求	*/
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqGetStat()"));
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* リトライカウンタセット	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* タイムアウト監視 開始	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_STA, "");
		ls_iPrc = 1;
		break;
	case 1:		/* 状態要求のレスポンス受信待ち	*/
		if (0 != pCtaRcvData->bRecved) {
			/* 受信データあった．	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* エラーレスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* エラー内容により強制終了の必要性を検討すること・・・	*/
				}
			} else if (CMD_STA == *pCtaRcvData->pdwCmdCode) {
				/* 応答レスポンスだった．	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* タイムアウト監視 解除	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* フォーマット系エラーだった	*/
				} else {
					m_lCtaStatus =	MAKELONG(	/* ステータス	*/
											MAKEWORD(
													l_CtaDataInf.CmdSTA.dwZState,	/* LL	*/
													l_CtaDataInf.CmdSTA.dwStatus1	/* LH	*/
												),
											MAKEWORD(
													l_CtaDataInf.CmdSTA.dwStatus2,	/* HL	*/
													0	/* HH	*/
												)
										);
					ls_dwRetryCnt = 0;
					l_bSeqEnd = TRUE;	/* シーケンス終了	*/
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* タイムアウトしました．	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* タイムアウト	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = 0;	/* リトライ	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(511);
			} else {
				l_bSeqEnd = TRUE;	/* シーケンス終了	*/
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* 強制終了	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* シーケンス終了	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqGetStat() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = 0;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}

/*
 *	上位へのアラーム通知
 */
BOOL CCtaCtrl::NotifyAlarm(int iAlarmCode)
{
	if (0 == iAlarmCode) {
		return FALSE;	/* 未通知	*/
	}

	int l_iNotifyCode = 0;
	int l_iLevel = 0;		/*	0:警告
								1:
								*/

	switch (iAlarmCode) {
	/* CTASIO エラー定義	*/
	default:						l_iNotifyCode = 100; l_iLevel = 0;	break;	/* 未定義エラー	*/
	case CTASIO_ERR_UNINIT:			l_iNotifyCode = 101; l_iLevel = 0;	break;
	case CTASIO_ERR_RCVBUFFULL:		l_iNotifyCode = 102; l_iLevel = 0;	break;
	case CTASIO_ERR_SEND_FAIL:		l_iNotifyCode = 103; l_iLevel = 0;	break;
	case CTASIO_ERR_PARAM:			l_iNotifyCode = 104; l_iLevel = 0;	break;
	case CTASIO_ERR_NORCVDATA:		l_iNotifyCode = 105; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALID_PKT:	l_iNotifyCode = 106; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALID_CMD:	l_iNotifyCode = 107; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALID_EVT:	l_iNotifyCode = 108; l_iLevel = 0;	break;
	case CTASIO_ERR_NOLF:			l_iNotifyCode = 109; l_iLevel = 0;	break;
	case CTASIO_ERR_NOCR:			l_iNotifyCode = 110; l_iLevel = 0;	break;
	case CTASIO_ERR_SUM:			l_iNotifyCode = 111; l_iLevel = 0;	break;
	case CTASIO_ERR_TOOSHORT:		l_iNotifyCode = 112; l_iLevel = 0;	break;
	case CTASIO_ERR_CTRLCODE:		l_iNotifyCode = 113; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALIDLEN:		l_iNotifyCode = 114; l_iLevel = 0;	break;
	case CTASIO_ERR_RCVFAIL:		l_iNotifyCode = 115; l_iLevel = 0;	break;
	/* CTACTL エラー定義	*/
	case CTACTL_ERR_DATANONE:		l_iNotifyCode = 120; l_iLevel = 0;	break;
	case CTACTL_ERR_DATAFORMAT:		l_iNotifyCode = 121; l_iLevel = 0;	break;
	case CTACTL_ERR_PKTCMD:			l_iNotifyCode = 122; l_iLevel = 0;	break;
	case CTACTL_ERR_PARAMERR:		l_iNotifyCode = 123; l_iLevel = 0;	break;
	case CTACTL_ERR_EQBUSY:			l_iNotifyCode = 124; l_iLevel = 0;	break;
	case CTACTL_ERR_EQERR:			l_iNotifyCode = 125; l_iLevel = 0;	break;
	case CTACTL_ERR_TIMEOUT:		l_iNotifyCode = 126; l_iLevel = 0;	break;
	case CTACTL_ERR_ILPO:			l_iNotifyCode = 127; l_iLevel = 0;	break;
	case CTACTL_ERR_ILPI:			l_iNotifyCode = 128; l_iLevel = 0;	break;
	case CTACTL_ERR_FORCEEND:		l_iNotifyCode = 129; l_iLevel = 0;	break;
	case CTACTL_ERR_RECALIB:		l_iNotifyCode = 130; l_iLevel = 0;	break;
	/* CTA ユニット エラーコード	*/
/*	case CTACTL_ERCD_RST:			l_iNotifyCode = 140; l_iLevel = 0;	break;	未使用	*/
	case CTACTL_ERCD_HEAD:			l_iNotifyCode = 141; l_iLevel = 0;	break;
	case CTACTL_ERCD_CMD:			l_iNotifyCode = 142; l_iLevel = 0;	break;
	case CTACTL_ERCD_SUM:			l_iNotifyCode = 143; l_iLevel = 0;	break;
	case CTACTL_ERCD_LEN:			l_iNotifyCode = 144; l_iLevel = 0;	break;
	case CTACTL_ERCD_IL:			l_iNotifyCode = 145; l_iLevel = 0;	break;
	case CTACTL_ERCD_CTRLR:			l_iNotifyCode = 146; l_iLevel = 1;	break;
	case CTACTL_ERCD_NO_MEAS:		l_iNotifyCode = 147; l_iLevel = 1;	break;
	case CTACTL_ERCD_MEASING:		l_iNotifyCode = 148; l_iLevel = 1;	break;
	case CTACTL_ERCD_NO_OPE:		l_iNotifyCode = 149; l_iLevel = 1;	break;
	case CTACTL_ERCD_UNSAVE:		l_iNotifyCode = 150; l_iLevel = 1;	break;
	case CTACTL_ERCD_MODE:			l_iNotifyCode = 151; l_iLevel = 1;	break;
	case CTACTL_ERCD_DUP_ID:		l_iNotifyCode = 152; l_iLevel = 1;	break;
	case CTACTL_ERCD_SET_VALUE:		l_iNotifyCode = 153; l_iLevel = 1;	break;
	case CTACTL_ERCD_PWR:			l_iNotifyCode = 154; l_iLevel = 1;	break;
	case CTACTL_ERCD_Z_CTRLR_COM:	l_iNotifyCode = 155; l_iLevel = 1;	break;
	case CTACTL_ERCD_GENDROP:		l_iNotifyCode = 156; l_iLevel = 1;	break;
	case CTACTL_ERCD_HOST_COM:		l_iNotifyCode = 157; l_iLevel = 1;	break;
	case CTACTL_ERCD_MOVE_TIMEOUT:	l_iNotifyCode = 158; l_iLevel = 1;	break;
	case CTACTL_ERCD_ORG:			l_iNotifyCode = 159; l_iLevel = 1;	break;
	case CTACTL_ERCD_Z_CTRLR:		l_iNotifyCode = 160; l_iLevel = 1;	break;
	case CTACTL_ERCD_PLC_COM:		l_iNotifyCode = 161; l_iLevel = 1;	break;
	}

	BOOL l_iRet = ::PostMessage(g_hNotifyWnd, WM_MEAS_CTA_ALARM, MAKEWPARAM(l_iNotifyCode, l_iLevel), 0);

	return l_iRet;
}
