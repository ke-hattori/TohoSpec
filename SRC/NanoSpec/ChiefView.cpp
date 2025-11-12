// ChiefView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChifRcvMailThread.h"
#include "ChiefView.h"
#include <NEXIF.HXX>
#include <PIFCOMM.HXX>
//#include <NEXIO.HXX>		hmenjo 使用禁止
#include <NEXIOBASE.HXX>
#include "ChifTransiAF.h"
#include "ChifTransiDeskew.h"
#include "ChifTransiMaster.h"
#include "ChifTransiSeq.h"
#include "ChifTransiSr1Point.h"
#include "ChifTransiSrRefer.h"
#include "ChiefExports.h"
#include "ChiefThread.h"
#include "LensComboBox.h"
#include "..\\..\\INC\\MeaSys.hxx"
#include "..\\..\\INC\\NSStage.hxx"
//Saiki 20090602 Add ----->
#include "resource.h"
//Saiki 20090602 Add <-----
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- { ---------- */
#include "ChifTransiStress.h"
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- } ---------- */
/* added 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
#define	CHIEF_PFUNCS_MAS
#include "ChiefPFuncs.h"
/* added 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	ChiefExport.h に出した方がよい定義
 */
#if 0
#define	CHIEF_INI	_T(".\\Chief.ini")	// Chief 用設定ファイル名
#define	CHIEF_DLG_CAPTION	_T("Chief Monitor")	// Chief ダイアログのキャプション
// 各種アラーム報告用定義
#endif

/*
 *	タイマ定義
 */
enum CHIEF_TIMER_ID {
	ID_TIMER_HIDEDLG = 201,		// Chief ダイアログ非表示タイマ ID
	ID_TIMER_1S_PERIOD,			// 1s	 定周期タイマ ID
	ID_TIMER_100MS_PERIOD,		// 100ms 定周期タイマ ID
	ID_TIMER_50MS_PERIOD,		// 50ms  定周期タイマ ID
	ID_TIMER_10MS_PERIOD,		// 10ms  定周期タイマ ID
// 2009.02.05 K.Matsuo delete -->
//	ID_TIMER_TRACEDATA,			// トレースデータ報告タイマ ID
// 2009.02.05 K.Matsuo delete <--
	ID_TIMER_EQPWOFF,			// 装置電源オフワンショットタイマ ID
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- { ---------- */
	ID_TIMER_HEPASTOP,			/* HEPA 停止待ちタイマ ID	*/
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- } ---------- */
};
#define	ID_MMTIMER_5MS_PERIOD	0		// 5ms	 定周期タイマ(マルチメディアタイマ) ID
#define	TIMER_1S_PERIOD			1000	// 1s	 定周期タイマ値[ms]
#define	TIMER_100MS_PERIOD		100		// 100ms 定周期タイマ値[ms]

//----- Kawashima 2008.12.01 Debug ----->
#define	TIMER_50MS_PERIOD		100		// 50ms  定周期タイマ値[ms]
#define	TIMER_20MS_PERIOD		100		// 20ms  定周期タイマ値[ms]
#define	TIMER_10MS_PERIOD		100		// 10ms  定周期タイマ値[ms]
#define	TIMER_5MS_PERIOD		100		// 5ms	定周期タイマ値[ms](マルチメディアタイマでのみ使うはず)

//#define	TIMER_50MS_PERIOD		50		// 50ms  定周期タイマ値[ms]
//#define	TIMER_20MS_PERIOD		20		// 20ms  定周期タイマ値[ms]
//#define	TIMER_10MS_PERIOD		10		// 10ms  定周期タイマ値[ms]
//#define	TIMER_5MS_PERIOD		5		// 5ms	定周期タイマ値[ms](マルチメディアタイマでのみ使うはず)
//<--------------------------------

/*
 *	ローカルグローバル定義
 */
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//CChiefView*	g_pcChiefView;	// static 用クラスポインタ
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
CChiefView*	g_pcChiefView = 0;	/* static 用クラスポインタ	*/
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

// 処理時間測定用関数群
static LONGLONG lg_llFrequency = 0;
static LONGLONG lg_llStartCount = 0;
static LONGLONG lg_llDelta = 0;
static void ZGetPerformanceStart(LONGLONG *pFrequency)
{
	LARGE_INTEGER	l_liFrequency;
	::QueryPerformanceFrequency(&l_liFrequency);
	*pFrequency = l_liFrequency.QuadPart;
	LARGE_INTEGER	l_liStartCounter;
	::QueryPerformanceCounter(&l_liStartCounter);
	lg_llStartCount = l_liStartCounter.QuadPart;
}
static void ZGetPerformanceStop(LONGLONG *pllDelta)
{
	LARGE_INTEGER	l_liStopCounter;
	::QueryPerformanceCounter(&l_liStopCounter);
	*pllDelta = l_liStopCounter.QuadPart - lg_llStartCount;
}
#if 0
ZGetPerformanceStart(&lg_llFrequency);
ZGetPerformanceStop(&lg_llDelta);
#endif

IMPLEMENT_DYNCREATE(CChiefView, CFormView)

/////////////////////////////////////////////////////////////////////////////
// CChiefView ダイアログ


CChiefView::CChiefView()
	: CFormView(CChiefView::IDD)
{
	//{{AFX_DATA_INIT(CChiefView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	TRACE(_T("CChiefView::CChiefView() \n"));

	m_pcMainFrame = (CFrameWnd*) AfxGetApp()->m_pMainWnd;						// CMainFrame のポインタを保存
	m_pcNanoSpecDoc = ((CMainFrame*) m_pcMainFrame)->m_pDoc;		// CNanoSpecDoc のポインタを保存

	LogChief(_T("Started  Chief Dialog."));

/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	m_pcChiefTransiAF = 0;
	m_pcChiefTransiDeskew = 0;
	m_pcChiefTransiMaster = 0;
	m_pcChiefTransiSeq = 0;
	m_pcChiefTransiSr1Point = 0;
	m_pcChiefTransiSrRefer = 0;
	m_pcChiefTransiStress = 0;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	g_pcChiefView = this;		// static 用 CChiefView クラスポインタ
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	m_lPreAFafterPreMove = 0;
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */

	// レシピ情報用メモリを取得
	RecipesMalloc(TRUE);

//	BOOL l_bRet = Create(CChiefView::IDD, 0);
//	// ダイアログ最小化時にタスクバーに入るようにする．	// 必要になりました(2008.12.15 頃のサーバソースから)
//	ModifyStyleEx(0, WS_EX_APPWINDOW, SWP_DRAWFRAME);	//不要だけど残しておきます．
}

void CChiefView::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CChiefView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_BTN_HIDE, m_HideButton);
	DDX_Control(pDX, IDC_CHK_TOP, m_TopButton);
	DDX_Control(pDX, IDC_BTN_TEST, m_TestButton);
}

BEGIN_MESSAGE_MAP(CChiefView, CFormView)
	//{{AFX_MSG_MAP(CChiefView)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_CHK_TOP, OnChkTop)
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnTest)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHIF_HEARTBEAT, OnHeartBeat)
	ON_MESSAGE(WM_NEX_RESPONSE, OnNextraResponse)
	ON_MESSAGE(WM_CHIF_RECV_PIFCMD, OnDispRecvPifCmd)
	ON_MESSAGE(WM_CHIF_UPDATESTATESDLG, OnUpdateStatesOnDlg)
// 2009.02.05 K.Matsuo delete -->
//	ON_MESSAGE(WM_CHIF_SENDTRACEDATA, OnSendTraceData)
// 2009.02.05 K.Matsuo delete <--
	ON_MESSAGE(WM_CHIF_PIFDIREFRESH, OnPifDiRefresh)
	ON_MESSAGE(WM_CHIF_EQRUNCHECK, OnEQRunCheck)
	ON_MESSAGE(WM_CHIF_REPORTALARM, OnReportAlarms)
	ON_MESSAGE(WM_CHIF_MOVE_SETPOS, OnMoveSetPosFromPif)
	ON_MESSAGE(WM_DISP_MOVE_SETPOS, OnMoveSetPosFromDisp)
	ON_MESSAGE(WM_CHIF_STAGE_STOP, OnStageStop)
	ON_MESSAGE(WM_DISP_VACUUM_ONOFF, OnVacuumFromDisp)
	ON_MESSAGE(WM_CHIF_SET_RECIPE, OnSetRecipe)
	ON_MESSAGE(WM_DISP_AF_POPUP_END, OnAFPopupEnd)
// 2009.07.29 K.Matsuo -->
	ON_MESSAGE(WM_MANUALSITE_POPUP_END, OnManualSitePtPopupEnd)
// 2009.07.29 K.Matsuo <--
	ON_MESSAGE(WM_MEAS_RECVDATA, OnMeasRecvData)
	ON_MESSAGE(WM_DATA_1POINT_END, OnData1PointEnd)
	ON_MESSAGE(WM_DATA_MEAS_END, OnDataMeasEnd)
	ON_MESSAGE(WM_DISP_START_AAF, OnStartAAF)
	ON_MESSAGE(WM_DISP_START_SEQ, OnStartDispSeq)
	ON_MESSAGE(WM_DISP_START_1POINT, OnStart1Point)
	ON_MESSAGE(WM_DISP_PAUSE_SEQ, OnSeqPause)
	ON_MESSAGE(WM_DISP_RESUME_SEQ, OnSeqResume)
	ON_MESSAGE(WM_DISP_CANCEL_SEQ, OnSeqCancel)
	ON_MESSAGE(WM_DISP_START_SRREF, OnStartSrRefer)
	ON_MESSAGE(WM_CHIF_SHOWSW, OnDlgShowSW)
	ON_MESSAGE(WM_CHIF_RESET_ALARM, OnResetAlarm)
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- { ---------- */
	ON_MESSAGE(WM_MEAS_LINE_END, OnMeasLineEnd)
	ON_MESSAGE(WM_DATA_LINE_END, OnDataLineEnd)
	ON_MESSAGE(WM_DATA_STRS_MEAS_END, OnDataStressMeasEnd)
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- } ---------- */
//2009.10.29 bagus 2point-distance --{--
	ON_MESSAGE(WM_DISP_DISTANCE_POPUP_END, OnDistancePopupEnd)
//2009.10.29 bagus 2point-distance --}--
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- { ---------- */
	ON_MESSAGE(WM_MEAS_CTA_ALARM, OnMeasAlarmCTA)
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- } ---------- */
//2009.11.03 bagus MS --{--
	ON_MESSAGE(WM_DISP_MS_POPUP_END, OnMSPopupEnd)
//2009.11.03 bagus MS --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	ON_MESSAGE(WM_MEAS_COMPEASE_STATUS, OnMeasCompEASEStatus)
	ON_MESSAGE(WM_MEAS_COMPEASE_ERROR, OnMeasCompEASEError)
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	ON_MESSAGE(WM_DISP_CONFIRM_POPUP_END, OnConfirmPopupEnd)
// 2013.11.07 Bagus Add (TohoSpec対応) <--
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefView メッセージ ハンドラ

void CChiefView::PostNcDestroy()
{
	// 自分を殺す -------------------------------------------------------------
	LogChief(_T("Ended    Chief Dialog."));

	CFormView::PostNcDestroy();	//	delete this;
}

//hmenjo debdeb 20081117
static UINT lg_uiRunFlagCount = 0;
static UINT lg_uiRunFlag = FALSE;

int CChiefView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に初期化の補足処理を追加してください

	return 0;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*
 *	マルチメディアタイマ CallBack ハンドラ
 *			ここから"WM_TIMER"をポストしてもいいんだけど・・・(ややこしくなるので・・・)
 *			"WinMM.lib" をリンクしなくても使えたけど，どこかでリンクしていますか？？
 */
UINT CChiefView::m_uiMMTimerID[16] = {0};
static UINT lg_uiMMTimer10msCounter = 0;
static UINT lg_uiMMTimer20msCounter = 0;
static UINT lg_uiMMTimer50msCounter = 0;
void CALLBACK CChiefView::MMTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if ( g_pcChiefView == NULL )
		return;

	if (uTimerID == m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD]) {
#if 0
		{
			// デバッグ用です
			static int l_iTmp = 0;
			l_iTmp++;
			if ((10000 / TIMER_5MS_PERIOD) < l_iTmp) {
				l_iTmp = 0;
			}
		}
#endif
		// ココは 5ms (100ms にされた by Kawashima 2008.12.01)-----------------------------------------------
		// 動作中の異常入力をチェック
		if ((PROCESS_INIT != g_pcChiefView->ProcStatusGet()) && (0 != lg_uiRunFlag)) {
			g_pcChiefView->CheckDIO_Running();
		}
		// Nextra から DI 情報を取得
		g_pcChiefView->GetDiInfo(&(g_pcChiefView->m_DiInfo));

#if 0		// (100ms にされた by Kawashima 2008.12.01 ので削除しておきます)
		// ココは 10ms ----------------------------------------------
		if (TIMER_10MS_PERIOD / TIMER_5MS_PERIOD < lg_uiMMTimer10msCounter++) {
			lg_uiMMTimer10msCounter = 0;
			// この下から処理を追加してください
#endif

			// DIO 監視開始タイマ(アラーム系のウィンドウが立ち上がるまでの待ち処理)
			if (0 != lg_uiRunFlagCount) {
				lg_uiRunFlagCount--;
				if (0 == lg_uiRunFlagCount) {
					lg_uiRunFlag = TRUE;
				}
			}
#if 0		// (100ms にされた by Kawashima 2008.12.01 ので削除しておきます)
		}
#endif
#if 0		// (100ms にされた by Kawashima 2008.12.01 ので削除しておきます)
		// ココは 20ms ----------------------------------------------
		if (TIMER_20MS_PERIOD / TIMER_5MS_PERIOD < lg_uiMMTimer20msCounter++) {
			lg_uiMMTimer20msCounter = 0;
			// この下から処理を追加してください
		}
		// ココは 50ms ----------------------------------------------
		if (TIMER_50MS_PERIOD / TIMER_5MS_PERIOD < lg_uiMMTimer50msCounter++) {
			lg_uiMMTimer50msCounter = 0;
			// この下から処理を追加してください
		}
#endif
	}
}

/*
 *	MFC のタイマ メッセージハンドラ
 */
void CChiefView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	switch (nIDEvent) {
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- { ---------- */
	case ID_TIMER_HEPASTOP:
		if (0 == this->KillTimer(ID_TIMER_HEPASTOP)) {
			/* タイマイベント削除失敗	*/
			this->LogChief(CHIEF_REP_ALARM_MSGTEXT[38]);
			this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(38, 0));
		}
		((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_HEPA_STOP, (cEventParams*) EV_STRS_HEPA_STOP);
		break;
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- } ---------- */
	case ID_TIMER_1S_PERIOD:		// 1s 定周期タイマ
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_1S_PERIOD);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		PostMessage(WM_CHIF_HEARTBEAT, 0, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->SetTimer(ID_TIMER_1S_PERIOD, TIMER_1S_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_100MS_PERIOD:		// 100ms 定周期タイマ
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_100MS_PERIOD);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		PostMessage(WM_CHIF_UPDATESTATESDLG, 0, 0);
		IsStageIdle();
		// 装置電源入力のオフを監視
		{
			if (0 != m_bCheckEqPowerOFF) {
				if (0 == m_DiInfo.bEQPower) {
					// 装置電源入力がオフしました．
					nexioEquipmentPowerOFF(FALSE);	// 装置電源オフ出力をオフします．
					if (2 != m_bCheckEqPowerOFF) {
						if (0 == KillTimer(ID_TIMER_EQPWOFF)) {
							// タイマイベント削除失敗
							LogChief(CHIEF_REP_ALARM_MSGTEXT[35]);
							PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(35, 0));
						}
					}
					m_bCheckEqPowerOFF = 0;
				}
			}
		}
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
		/* AF seq アイドルチェック	*/
		if (2 == (m_lPreAFafterPreMove & 0x7fffffff)) {
			if (true == ((CChiefTransiAF*) m_pcChiefTransiAF)->IsIdle()) {
				/* AF seq アイドルでした．	*/
				if (0 == (m_lPreAFafterPreMove & 0x80000000)) {
					/* AF 成功	*/
					this->LogChief(_T("PreAF - AF Success. (m_lPreAFafterPreMove = 3) EV_SEQ_POINT_MOVE_DONE"));
					m_lPreAFafterPreMove = 3;
				} else {
					/* AF 失敗	*/
					this->LogChief(_T("PreAF - AF Failure. (m_lPreAFafterPreMove = 0)"));
					m_lPreAFafterPreMove = 0;
				}
				((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_POINT_MOVE_DONE);
			}
		}
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->SetTimer(ID_TIMER_100MS_PERIOD, TIMER_100MS_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_50MS_PERIOD:		// 50ms 定周期タイマ(100ms にされた by Kawashima 2008.12.01)
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_50MS_PERIOD);
		this->SetTimer(ID_TIMER_50MS_PERIOD, TIMER_50MS_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_10MS_PERIOD:		// 10ms 定周期タイマ(100ms にされた by Kawashima 2008.12.01)
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_10MS_PERIOD);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		PostMessage(WM_CHIF_PIFDIREFRESH, 0, 0);
		PostMessage(WM_CHIF_EQRUNCHECK, 0, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->SetTimer(ID_TIMER_10MS_PERIOD, TIMER_10MS_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_HIDEDLG:			// ダイアログ非表示タイマ
		if (0 == KillTimer(ID_TIMER_HIDEDLG)) {
			// タイマイベント削除失敗
			LogChief(CHIEF_REP_ALARM_MSGTEXT[3]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(3, 0));
		}
		switch (m_DlgShowSW.dwShowSWprc) {
		case 0:
			GetParentFrame()->ShowWindow(SW_SHOWMINIMIZED);	// 一旦最小化しておく
			m_DlgShowSW.dwShowSWprc = 1;
			if (ID_TIMER_HIDEDLG != SetTimer(ID_TIMER_HIDEDLG, 500, 0)) {
				// タイマ起動失敗
				LogChief(CHIEF_REP_ALARM_MSGTEXT[2]);
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(2, 0));
			}
			break;
		case 1:
			GetParentFrame()->ShowWindow(SW_SHOWNORMAL);	// 表示する
			m_DlgShowSW.dwShowSWprc = 2;
			if (ID_TIMER_HIDEDLG != SetTimer(ID_TIMER_HIDEDLG, m_DlgShowSW.dwDelay * 1000, 0)) {
				// タイマ起動失敗
				LogChief(CHIEF_REP_ALARM_MSGTEXT[2]);
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(2, 0));
			}
			break;
		default:
			switch (m_DlgShowSW.dwType) {
			case 0:
				GetParentFrame()->ShowWindow(SW_HIDE);
				break;
			case 1:
				GetParentFrame()->ShowWindow(SW_MINIMIZE);
				break;
			}
			break;
		}
		break;
// 2009.02.05 K.Matsuo delete -->
//	case ID_TIMER_TRACEDATA:		// トレースデータ定時報告(送信)タイマ
//		if (0 == KillTimer(ID_TIMER_TRACEDATA)) {
//			// タイマイベント削除失敗
//			LogChief(CHIEF_REP_ALARM_MSGTEXT[1]);
//			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(1, 0));
//			//
//		}
//		// トレースデータ定時送信タイマ起動
//		if (0 != (m_uiTraceDataPeriod = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetTraceDataPeriod())) {
//			// トレースデータ報告(送信)
//			OnSendTraceData(0, 0);
//			if (0 == StartTimerTraceData(m_uiTraceDataPeriod)) {
//				// タイマ起動失敗
//				m_bTraceDataTimer = FALSE;
//				LogChief(CHIEF_REP_ALARM_MSGTEXT[0]);
//				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(0, 0));
//			}
//		} else {
//			// 周期設定が０になっていたので定時処理停止
//			m_bTraceDataTimer = FALSE;
//		}
//		break;
// 2009.02.05 K.Matsuo delete <--
	case ID_TIMER_EQPWOFF:		// 装置電源オフ入力監視タイマ
		// 装置電源入力が反応しないので，装置電源オフ：解除
//		nexioEquipmentPowerOFF(FALSE);		安全のため装置電源オフは解除しません．
//		m_bCheckEqPowerOFF = 0;			同じく監視フラグもオフしません．
		m_bCheckEqPowerOFF = 2;		// タイムアウト済にします．
		// アラームセット
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EquipmentPowerOffControlError));
		if (0 == KillTimer(ID_TIMER_EQPWOFF)) {
			// タイマイベント削除失敗
			LogChief(CHIEF_REP_ALARM_MSGTEXT[35]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(35, 0));
		}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

/*
 *	Chief 動作中表示 メッセージハンドラ
 */
LRESULT CChiefView::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeatCnt++;
	CString l_strTmp;
	l_strTmp.Format("%d", m_dwHeartBeatCnt);
	SetDlgItemText(IDC_STATIC_HEART, l_strTmp);

	// 各スレッド用 HeartBeat パルス
	((CChiefTransiAF*) m_pcChiefTransiAF)->HeartBeatPulse();
	((CChiefTransiDeskew*) m_pcChiefTransiDeskew)->HeartBeatPulse();
	((CChiefTransiMaster*) m_pcChiefTransiMaster)->HeartBeatPulse();
	((CChiefTransiSeq*) m_pcChiefTransiSeq)->HeartBeatPulse();
	((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->HeartBeatPulse();
	((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->HeartBeatPulse();

// 2009.02.05 K.Matsuo delete -->
//	// トレースデータ定時送信タイマ起動
//	if (0 == m_bTraceDataTimer) {
//		// 定時処理が停止しているときのみチェックします．
//		if (0 != (m_uiTraceDataPeriod = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetTraceDataPeriod())) {
//			if (0 == StartTimerTraceData(m_uiTraceDataPeriod)) {
//				// タイマ起動失敗
//				LogChief(CHIEF_REP_ALARM_MSGTEXT[0]);
//				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(0, 0));
//			} else {
//				// タイマ起動成功
//				m_bTraceDataTimer = TRUE;
//			}
//		}
//	}
// 2009.02.05 K.Matsuo delete <--

	return 0L;
}

/*
 *	キャンセル(「Cancel」ボタン，「ESC」キー) メッセージハンドラ
 */
void CChiefView::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	// 「×」ボタンや「ESC」キーでダイアログが終了しないようにコメントにする
//	CDialog::OnCancel();
}

/*
 *	「Hide」ボタン(非表示ボタン) メッセージハンドラ
 */
void CChiefView::OnBtnHide()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	KillTimer(ID_TIMER_HIDEDLG);	// ダイアログ表示ディレイタイマを消しておく
	m_DlgShowSW.dwType = 0;
	GetParentFrame()->ShowWindow(SW_HIDE);
}

/*
 *	「Top」ボタン(元はチェックボックス) メッセージハンドラ
 */
void CChiefView::OnChkTop()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if (0 == ((CButton*) GetDlgItem(IDC_CHK_TOP))->GetCheck()) {
		// 最前面を解除
		GetParentFrame()->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	} else {
		// 最前面にする
		GetParentFrame()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

/*
 *	Nextra からの応答(完了)メッセージジハンドラ
 */
LRESULT CChiefView::OnNextraResponse(WPARAM wparam, LPARAM lparam)
{
	DWORD	l_dwDeviceCode = (DWORD) wparam;
	int		l_iResult = (int) ((0 == lparam)? 1 : 0);

	LogChief_WinMsg(_T("WM_NEX_RESPONSE"), wparam, lparam);

	if ((0 != m_bVacuumOnFromDisp) && (nexVacuumOn == l_dwDeviceCode)) {
		// バキューム ON 制御が画面からの指令の場合
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_iResult, 0);
		m_bVacuumOnFromDisp = FALSE;
	} else if ((0 != m_bVacuumOffFromDisp) && (nexVacuumOff == l_dwDeviceCode)) {
		// バキューム OFF 制御が画面からの指令の場合
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_iResult, 0);
		m_bVacuumOffFromDisp = FALSE;
	} else {
/* modified 2009.08.06 hmenjo ストレス機能追加(28) ---------- { ---------- */
//		if ((nexMoveToUpper == l_dwDeviceCode) || (nexMoveToLower == l_dwDeviceCode)) {
/* modified 2009.08.06 hmenjo ストレス機能追加(28) ----------			   */
		if ((nexMoveToUpper == l_dwDeviceCode) || (nexMoveToLower == l_dwDeviceCode) || (nexMoveToAlignment == l_dwDeviceCode)) {
/* modified 2009.08.06 hmenjo ストレス機能追加(28) ---------- } ---------- */
			ActuateFlagsSet(ACTUATE_PIN, FALSE);			// 動作中フラグ(ピン)をオフ
		}
		if ((nexOpenShutter == l_dwDeviceCode) || (nexCloseShutter == l_dwDeviceCode)) {
			ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);		// 動作中フラグ(シャッタ)をオフ
		}
		if ((nexLoad == l_dwDeviceCode) || (nexUnload == l_dwDeviceCode)) {
			ActuateFlagsSet(ACTUATE_PIN, FALSE);			// 動作中フラグ(ピン)をオフ
			ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);	// 動作中フラグ(ワークガイド)をオフ
		}
		switch (l_dwDeviceCode) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
//		case nexLoad:			PifComm_AlignmentLoadResultReport(l_iResult);	break;	// アライメント Load
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
		case nexLoad:																	/* アライメント Load	*/
			if (0 == m_dwPinMoveState) {
// 2010.02.16 K.Matsuo Bug fix -->
//				PifComm_AlignmentLoadResultReport(l_iResult);	break;
				PifComm_AlignmentLoadResultReport(l_iResult);
// 2010.02.16 K.Matsuo Bug fix <--
			} else {
				m_dwPinMoveState = 0;
				int l_iEvent;
				int l_iErrCode;
				if (0 != l_iResult) {
					/* 異常	*/
					l_iEvent = EV_STRS_CANCEL;
					l_iErrCode = TR_STRS_PINMOVE_FAIL;
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
				} else {
					/* 正常	*/
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_PIN_DONE, (cEventParams*) EV_STRS_PIN_DONE);
				}
			}
// 2010.02.16 K.Matsuo Bug fix -->
			break;
// 2010.02.16 K.Matsuo Bug fix <--
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
		case nexUnload:			PifComm_AlignmentUnloadResultReport(l_iResult);	break;	// アライメント Unload
		case nexMoveToUpper:	PifComm_PinUpResultReport(l_iResult);			break;	// ピン上昇端移動
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
///* deleted 2009.08.03 hmenjo ストレス機能追加(17) ---------- { ---------- */
////		case nexMoveToLower:	PifComm_PinDownResultReport(l_iResult);			break;	// ピン下降端移動
///* deleted 2009.08.03 hmenjo ストレス機能追加(17) ---------- } ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
		case nexMoveToLower:	PifComm_PinDownResultReport(l_iResult);			break;	// ピン下降端移動
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
		case nexOpenShutter:	PifComm_ShutterOpenResultReport(l_iResult);		break;	// シャッタ Open
		case nexCloseShutter:	PifComm_ShutterCloseResultReport(l_iResult);	break;	// シャッタ Close
		case nexVacuumOn:		PifComm_VaccumOnResultReport(l_iResult);		break;	// バキューム ON
		case nexVacuumOff:		PifComm_VaccumOffResultReport(l_iResult);		break;	// バキューム OFF
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
///* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- { ---------- */
//		case nexMoveToLower:															/* ピン下降端移動	*/
//		case nexMoveToAlignment:														/* ピンアライメント位置移動	*/
//			if (0 == m_dwPinMoveState) {
//				if (nexMoveToLower == l_dwDeviceCode) {
//					PifComm_PinDownResultReport(l_iResult);
//				}
//			} else {
//				m_dwPinMoveState = 0;
//				int l_iEvent;
//				int l_iErrCode;
//// 2009.08.21 K.Matsuo -->
////				if (0 == l_iResult) {
//				if (0 != l_iResult) {
//// 2009.08.21 K.Matsuo <--
//					/* 異常	*/
//					l_iEvent = EV_STRS_CANCEL;
//					l_iErrCode = TR_STRS_PINMOVE_FAIL;
//// 2009.08.22 K.Matsuo -->
//					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
//// 2009.08.22 K.Matsuo <--
//				} else {
//					/* 正常	*/
//// 2009.08.22 K.Matsuo -->
////					l_iEvent = EV_STRS_PIN_DONE;
////					l_iErrCode = 0;
//					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_PIN_DONE, (cEventParams*) EV_STRS_PIN_DONE);
//// 2009.08.22 K.Matsuo <--
//				}
//// 2009.08.22 K.Matsuo -->
////				((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
//// 2009.08.22 K.Matsuo <--
//			}
//			break;
///* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- } ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
		case nexMoveToAlignment:														/* ピンアライメント位置移動	*/
			if (0 == m_dwPinMoveState) {
				if (nexMoveToLower == l_dwDeviceCode) {
					PifComm_PinDownResultReport(l_iResult);
				}
			} else {
				m_dwPinMoveState = 0;
				int l_iEvent;
				int l_iErrCode;
				if (0 != l_iResult) {
					/* 異常	*/
					l_iEvent = EV_STRS_CANCEL;
					l_iErrCode = TR_STRS_PINMOVE_FAIL;
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
				} else {
					/* 正常	*/
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_PIN_DONE, (cEventParams*) EV_STRS_PIN_DONE);
				}
			}
			break;
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
		case nexInitialize:																// (初期化？？？保留中)
		case nexReturnToOrigin:															// ピン原点復帰，ワークガイド開
/* deleted 2009.08.03 hmenjo ストレス機能追加(17) ---------- { ---------- */
//		case nexMoveToAlignment:														// ピンアライメント位置移動
/* deleted 2009.08.03 hmenjo ストレス機能追加(17) ---------- } ---------- */
		case nexOpenWorkGuide:															// ワークガイド開
		case nexCloseWorkGuide:															// ワークガイド閉
		default:
			/*	現在(2008.09.03)，これらの指令は，Chief からは発行されません．
				したがって，メッセージを受取っても，何も処理していません．	*/
			LogChief(_T("Last Nextra Response was invalid."));
			break;
		}
	}

	return 0L;
}

/*
 *	Pif コマンド受信表示 メッセージハンドラ
 *		先頭のコマンドコード４文字をダイアログに表示します．
 */
LRESULT CChiefView::OnDispRecvPifCmd(WPARAM wparam, LPARAM lparam)
{
	CString	l_strTemp;

	l_strTemp = ((CChiefRcvMailThread*) m_pcChiefRcvMailThread)->GetPifRcvCmd();

	SetDlgItemText(IDC_STATIC_PIFRECV, l_strTemp);

	return 0L;
}

/*
 *	ダイアログ上のステータスモニタを更新 メッセージハンドラ
 *		・測定ステータス
 *		・画面ステータス(モード)
 *		・動作中フラグ
 *		・PR 中フラグ
 *		・各トランジション状態
 *		・各モジュール状態
 */
LRESULT CChiefView::OnUpdateStatesOnDlg(WPARAM wparam, LPARAM lparam)
{
	//Saiki 20090527 Change ----->
	//// 測定ステータス ------------------------------------------------------
	//SetDlgItemText(IDC_STATIC_PRCSTS, PROCESS_STATUS_DISP[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()]);
	//// 画面ステータス(モード) ----------------------------------------------
	//SetDlgItemText(IDC_STATIC_DSPSTS, DISPMODE_STATUS_DISP[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()]);
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if(l_SystemConfig.nLanguage == 0){
		// 測定ステータス ------------------------------------------------------
		SetDlgItemText(IDC_STATIC_PRCSTS, PROCESS_STATUS_DISP_ENU[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()]);
		// 画面ステータス(モード) ----------------------------------------------
		SetDlgItemText(IDC_STATIC_DSPSTS, DISPMODE_STATUS_DISP_ENU[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()]);
	}
	else{
		// 測定ステータス ------------------------------------------------------
		SetDlgItemText(IDC_STATIC_PRCSTS, PROCESS_STATUS_DISP_JPN[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()]);
		// 画面ステータス(モード) ----------------------------------------------
		SetDlgItemText(IDC_STATIC_DSPSTS, DISPMODE_STATUS_DISP_JPN[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()]);
	}

	//Saiki 20090527 Change <-----
	// 動作中フラグ --------------------------------------------------------
	SetDlgItemText(IDC_STATIC_ACTXY,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTZ,		(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTPIN,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_PIN))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTSHUT,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_SHUTTER))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTWG,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_WORKGUIDE))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTTRET,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_TURRET))? _T("ON") : _T("OFF"));
	//2009.11.10 bagus MS --{-- 追加された動作中フラグの表示
// 2009.11.24 K.Matsuo Delete -->
//	SetDlgItemText(IDC_STATIC_ACTHEAD,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_HEAD))? _T("ON") : _T("OFF"));
// 2009.11.24 K.Matsuo Delete <--
	SetDlgItemText(IDC_STATIC_ACTMS,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_MICROSCOPE))? _T("ON") : _T("OFF"));
	//2009.11.10 bagus MS --}-- 追加された動作中フラグの表示

	// PR 中フラグ ---------------------------------------------------------
	SetDlgItemText(IDC_STATIC_PRSTS,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetPatRecFlag())? _T("ON") : _T("OFF"));
	// 各トランジション状態 ------------------------------------------------
	// マスタ トランジション
	SetDlgItemText(IDC_STATIC_TRMASTER, STATES_MASTER_DISP[((CChiefTransiMaster*) m_pcChiefTransiMaster)->GetCurrentState()]);
	// シーケンス測定 トランジション
	SetDlgItemText(IDC_STATIC_TRSEQ, STATES_SEQ_DISP[((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentState()]);
	// SR リファレンス測定 トランジション
	SetDlgItemText(IDC_STATIC_TRSRREFER, STATES_SR_REFER_DISP[((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->GetCurrentState()]);
	// デスキュー トランジション
	SetDlgItemText(IDC_STATIC_TRDESKEW, STATES_DESKEW_DISP[((CChiefTransiDeskew*) m_pcChiefTransiDeskew)->GetCurrentState()]);
	// AF トランジション
	SetDlgItemText(IDC_STATIC_TRAAF, STATES_AAF_DISP[((CChiefTransiAF*) m_pcChiefTransiAF)->GetCurrentState()]);
	// SR １ポイント測定 トランジション
	SetDlgItemText(IDC_STATIC_TRSR1POINT, STATES_SR_1POINT_DISP[((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->GetCurrentState()]);
/* added 2009.08.06 hmenjo ストレス機能追加(27) ---------- { ---------- */
	// ストレス シーケンス測定 トランジション
	SetDlgItemText(IDC_STATIC_TRSTRS, STATES_STRS_DISP[((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState()]);
/* added 2009.08.06 hmenjo ストレス機能追加(27) ---------- } ---------- */
	// 各モジュール状態 ----------------------------------------------------
	// ステージモジュール
	SetDlgItemText(IDC_STATIC_MODSTAGE, (0 == m_dwModuleState_Stage)? _T("Uninitialize") : _T("Initialized"));
	// SR 測定モジュール
	SetDlgItemText(IDC_STATIC_MODSRMEAS, (0 == m_dwModuleState_SR_Meas)? _T("Uninitialize") : _T("Initialized"));
	// Pif モジュール
	SetDlgItemText(IDC_STATIC_MODPIF, (0 == m_dwModuleState_Pif)? _T("Uninitialize") : _T("Initialized"));
	// Nextra モジュール
	SetDlgItemText(IDC_STATIC_MODNEXTRA, (0 == m_dwModuleState_Nextra)? _T("Uninitialize") : _T("Initialized"));
	// 各スレッド用 HeartBeat パルス ---------------------------------------
	CString l_strTmp;
	l_strTmp.Format("%02X", ((CChiefTransiAF*) m_pcChiefTransiAF)->m_dwHeartBeat & 0x000000ff);
	SetDlgItemText(IDC_STATIC_TRAAF_HB, l_strTmp);
	l_strTmp.Format("%02X", ((CChiefTransiDeskew*) m_pcChiefTransiDeskew)->m_dwHeartBeat & 0x000000ff);
	SetDlgItemText(IDC_STATIC_TRDESKEW_HB, l_strTmp);
	l_strTmp.Format("%02X", ((CChiefTransiMaster*) m_pcChiefTransiMaster)->m_dwHeartBeat & 0x000000ff);
	SetDlgItemText(IDC_STATIC_TRMASTER_HB, l_strTmp);
	l_strTmp.Format("%02X", ((CChiefTransiSeq*) m_pcChiefTransiSeq)->m_dwHeartBeat & 0x000000ff);
	SetDlgItemText(IDC_STATIC_TRSEQ_HB, l_strTmp);
	l_strTmp.Format("%02X", ((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->m_dwHeartBeat & 0x000000ff);
	SetDlgItemText(IDC_STATIC_TRSR1POINT_HB, l_strTmp);
	l_strTmp.Format("%02X", ((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->m_dwHeartBeat & 0x000000ff);
	SetDlgItemText(IDC_STATIC_TRSRREFER_HB, l_strTmp);
/* added 2009.05.27 hmenjo SPT アラームの検出 ---------- { ---------- */
	SPTAlarmDetector();
/* added 2009.05.27 hmenjo SPT アラームの検出 ---------- } ---------- */

	return 0L;
}

// 2009.02.05 K.Matsuo delete -->
///*
// *	トレースデータ定時報告(送信)用タイマ スタート関数
// */
//BOOL CChiefView::StartTimerTraceData(UINT uiTraceDataPeriod)
//{
//	if (ID_TIMER_TRACEDATA != SetTimer(ID_TIMER_TRACEDATA, uiTraceDataPeriod, 0)) {
//		// タイマ起動失敗
//		return FALSE;
//	} else {
//		// タイマ起動成功
//		return TRUE;
//	}
//}
//
///*
// *	トレースデータ送信 メッセージハンドラ
// */
//LRESULT CChiefView::OnSendTraceData(WPARAM wparam, LPARAM lparam)
//{
//	PifComm_TraceDataReport();
//		/*
//		 *	この関数は，まだ，未完成ですので，変更の可能性があります．
//		 */
//
//	return 0L;
//}
// 2009.02.05 K.Matsuo delete <--

/*
 *	DI 状態(情報)取得
 *		Nextra IO から取得します
 */
void CChiefView::GetDiInfo(CHIEF_DI_INFO* pDiInfo)
{
	pDiInfo->bEMOStop			= nexioIsEmergencyStop();
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
	pDiInfo->bSafetyPlug		= nexioIsSafetyPlug();
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
	pDiInfo->bDoorInterlock		= nexioIsDoorInterlock();
	pDiInfo->bEQPower			= nexioIsEquipmentPower();
//	pDiInfo->bAlignmentPowerOn	= TRUE;	//nexioGetInpDataPtr()->AlignmentPowerOn;	これは削除されました(2008.10.27)．
	pDiInfo->bMaintenanceSW		= nexioIsMaintenanceSwitch();
	pDiInfo->bTHMaintenanceSW	= nexioIsEngineerMaintenanceSwitch();
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//	pDiInfo->bAirPressureLow	= nexioIsAirPressureLevelLow();
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
	pDiInfo->bAirPressureLow	= ((CNanoSpecDoc*) m_pcNanoSpecDoc)->Rap_IsAirPressureLowON();
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	pDiInfo->bGlassExist		= nexioIsGlassExist();
	pDiInfo->bLoadPos			= nexioIsStageLoadPos();
	pDiInfo->bShutterOpen		= nexioIsShutterOpen();
	pDiInfo->bShutterClose		= nexioIsShutterClose();
	pDiInfo->bRobotArm			= nexioIsRobotArmDetect();
	pDiInfo->bVacuumPressure1	= nexioIsVacuumOn();
//	pDiInfo->bVacuumPressure2	= nexioIsVacuumOn();		これは削除されました(2008.10.27)．
	pDiInfo->bPinUp				= nexioIsPinUpperPos();
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//	pDiInfo->bPinDown			= nexioIsPinDownPos();
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
	if ((0 != this->IsHWS()) && (0 != m_pcChiefTransiStress)) {
		if (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsPinAligningHWS()) {
			pDiInfo->bPinDown		= nexioIsPinAlignmentPos();
			pDiInfo->bPinAlign		= nexioIsPinDownPos();
		} else {
			pDiInfo->bPinDown		= nexioIsPinDownPos();
			pDiInfo->bPinAlign		= nexioIsPinAlignmentPos();
		}
	} else {
		pDiInfo->bPinDown		= nexioIsPinDownPos();
		pDiInfo->bPinAlign		= nexioIsPinAlignmentPos();
	}
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
	pDiInfo->bWorkGuideOpen		= nexioIsWorkGuideOpen();
	pDiInfo->bWorkGuideClose	= nexioIsWorkGuideClose();

// 2013.01.10 bagus stage driver alarm io -->
	pDiInfo->bStageAlarm		= nexioIsStageAlarm();

	pDiInfo->bX1AxisAlarm[0]	= nexioIsStageDriverX1AxisAlarm(0);
	pDiInfo->bX1AxisAlarm[1]	= nexioIsStageDriverX1AxisAlarm(1);
	pDiInfo->bX1AxisAlarm[2]	= nexioIsStageDriverX1AxisAlarm(2);

	pDiInfo->bX2AxisAlarm[0]	= nexioIsStageDriverX2AxisAlarm(0);
	pDiInfo->bX2AxisAlarm[1]	= nexioIsStageDriverX2AxisAlarm(1);
	pDiInfo->bX2AxisAlarm[2]	= nexioIsStageDriverX2AxisAlarm(2);

	pDiInfo->bYAxisAlarm[0]		= nexioIsStageDriverYAxisAlarm(0);
	pDiInfo->bYAxisAlarm[1]		= nexioIsStageDriverYAxisAlarm(1);
	pDiInfo->bYAxisAlarm[2]		= nexioIsStageDriverYAxisAlarm(2);
// 2013.01.10 bagus stage driver alarm io <--
}

/*
 *	Pif DI リフレッシュ メッセージハンドラ
 *		DI 情報を Pif の共有エリア(プロセスステータス報告用ファイルマッピング)にセットします．
 */
LRESULT CChiefView::OnPifDiRefresh(WPARAM wparam, LPARAM lparam)
{
	PifComm_GetEqMonitorPtr()->iGlassExist		= m_DiInfo.bGlassExist;
	PifComm_GetEqMonitorPtr()->iLoadPosition	= m_DiInfo.bLoadPos;
	PifComm_GetEqMonitorPtr()->iVaccumOn1		= m_DiInfo.bVacuumPressure1;
	PifComm_GetEqMonitorPtr()->iShutterOpen		= m_DiInfo.bShutterOpen;
	PifComm_GetEqMonitorPtr()->iShutterClose	= m_DiInfo.bShutterClose;
	PifComm_GetEqMonitorPtr()->iPinUp			= m_DiInfo.bPinUp;
	PifComm_GetEqMonitorPtr()->iPinDown			= m_DiInfo.bPinDown;
	PifComm_GetEqMonitorPtr()->iArmSensor		= m_DiInfo.bRobotArm;
	PifComm_GetEqMonitorPtr()->iDoorInterlock	= m_DiInfo.bDoorInterlock;
//	PifComm_GetEqMonitorPtr()->iVaccumOn2		= m_DiInfo.bVacuumPressure2;	これは削除されました(2008.10.27)

	return 0L;
}

/*
 *	EQ 動作中チェック メッセージハンドラ
 */
LRESULT CChiefView::OnEQRunCheck(WPARAM wparam, LPARAM lparam)
{
	BOOL	l_bEQRun = FALSE;

	// プロセスステータスをチェック
	switch (((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()) {
	case PROCESS_PROC:
	case PROCESS_ABRT:
	case PROCESS_COMP:
		l_bEQRun = TRUE;
		break;
	default:
		l_bEQRun = FALSE;
		break;
	}
	if (FALSE == l_bEQRun) {
		// 動作中フラグをチェック
		if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
			l_bEQRun = TRUE;
		}
	}
	if (FALSE == l_bEQRun) {
		// ジョイスティックをチェック
		if (0 != ((CMainFrame*) m_pcMainFrame)->GetJoyStickMode()) {
			// ハードジョイだった
			l_bEQRun = TRUE;
		}
	}

	if ((0 != m_DiInfo.bMaintenanceSW)
	 && (0 == m_DiInfo.bTHMaintenanceSW)) {
		// メンテ SW オフ，かつ，東朋メンテ SW オフ，のときはオンしません．
		// つまり，通常モードのことです．
		// しかし、初期化中の動作中は強制オフはしません．
		if (PROCESS_INIT != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()) {
			l_bEQRun = FALSE;
		}
	}

	if (0 == m_DiInfo.bEQPower) {
		// 装置電源がオフなのでオンさせません．
		l_bEQRun = FALSE;
	}
	if (m_bEQRunPrev != l_bEQRun) {
		// EQ 動作中の状態に変化があったので EQ 動作中出力を設定
		nexioEquipmentStatusRun(l_bEQRun);
		m_bEQRunPrev = l_bEQRun;
	}

	return 0L;
}

/*
 *	各トランジション スレッドを起動
 */
DWORD CChiefView::TransitionsStart()
{
	DWORD	l_dwRc = 0;

	// AF トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiAF."));
	m_pcChiefTransiAF = new CChiefTransiAF((CWnd*) this);
	if (0 == m_pcChiefTransiAF->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiAF."));
		l_dwRc = 6;
	}
	// デスキュー トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiDeskew."));
	m_pcChiefTransiDeskew = new CChiefTransiDeskew((CWnd*) this);
	if (0 == m_pcChiefTransiDeskew->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiDeskew."));
		l_dwRc = 5;
	}
	// SR リファレンス測定 トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiSrRefer."));
	m_pcChiefTransiSrRefer = new CChiefTransiSrRefer((CWnd*) this);
	if (0 == m_pcChiefTransiSrRefer->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiSrRefer."));
		l_dwRc = 4;
	}
	// SR １ポイント測定 トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiSr1Point."));
	m_pcChiefTransiSr1Point = new CChiefTransiSr1Point((CWnd*) this);
	if (0 == m_pcChiefTransiSr1Point->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiSr1Point."));
		l_dwRc = 3;
	}
	// シーケンス測定 トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiSeq."));
	m_pcChiefTransiSeq = new CChiefTransiSeq((CWnd*) this);
	if (0 == m_pcChiefTransiSeq->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiSeq."));
		l_dwRc = 2;
	}
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- { ---------- */
	// ストレス トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiStress."));
	m_pcChiefTransiStress = new CChiefTransiStress((CWnd*) this);
	if (0 == m_pcChiefTransiStress->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiStress."));
		l_dwRc = 2;
	}
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- } ---------- */
	// マスタ トランジション スレッドを起動
	LogChief(_T("Requested to start CChiefTransiMaster."));
	m_pcChiefTransiMaster = new CChiefTransiMaster((CWnd*) this);
	if (0 == m_pcChiefTransiMaster->m_hThread) {
		// スレッド起動失敗
		LogChief(_T("Failed to start CChiefTransiMaster."));
		l_dwRc = 1;
	}

	return l_dwRc;
}

/*
 *	各トランジション スレッドを終了
 */
void CChiefView::TransitionsEnd()
{
	// AF トランジション スレッドを終了
	if (0 != m_pcChiefTransiAF) {
		LogChief(_T("Deleting CChiefTransiAF..."));
		delete m_pcChiefTransiAF;
		m_pcChiefTransiAF = 0;
		LogChief(_T("Deleted  CChiefTransiAF."));
	}
	// デスキュー トランジション スレッドを終了
	if (0 != m_pcChiefTransiDeskew) {
		LogChief(_T("Deleting CChiefTransiDeskew..."));
		delete m_pcChiefTransiDeskew;
		m_pcChiefTransiDeskew = 0;
		LogChief(_T("Deleted  CChiefTransiDeskew."));
	}
	// SR リファレンス測定 トランジション スレッドを終了
	if (0 != m_pcChiefTransiSrRefer) {
		LogChief(_T("Deleting CChiefTransiSrRefer..."));
		delete m_pcChiefTransiSrRefer;
		m_pcChiefTransiSrRefer = 0;
		LogChief(_T("Deleted  CChiefTransiSrRefer."));
	}
	// SR １ポイント測定 トランジション スレッドを終了
	if (0 != m_pcChiefTransiSr1Point) {
		LogChief(_T("Deleting CChiefTransiSr1Point..."));
		delete m_pcChiefTransiSr1Point;
		m_pcChiefTransiSr1Point = 0;
		LogChief(_T("Deleted  CChiefTransiSr1Point."));
	}
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- { ---------- */
	// シーケンス測定 トランジション スレッドを終了
	if (0 != m_pcChiefTransiStress) {
		LogChief(_T("Deleting CChiefTransiStress..."));
		delete m_pcChiefTransiStress;
		m_pcChiefTransiStress = 0;
		LogChief(_T("Deleted  CChiefTransiStress."));
	}
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- } ---------- */
	// シーケンス測定 トランジション スレッドを終了
	if (0 != m_pcChiefTransiSeq) {
		LogChief(_T("Deleting CChiefTransiSeq..."));
		delete m_pcChiefTransiSeq;
		m_pcChiefTransiSeq = 0;
		LogChief(_T("Deleted  CChiefTransiSeq."));
	}
	// マスタ トランジション スレッドを終了
	if (0 != m_pcChiefTransiMaster) {
		LogChief(_T("Deleting CChiefTransiMaster..."));
		delete m_pcChiefTransiMaster;
		m_pcChiefTransiMaster = 0;
		LogChief(_T("Deleted  CChiefTransiMaster."));
	}
}
/*
 *	各種アラーム処理用 メッセージハンドラ
 */
/*			パラメタ説明(定義は"ChiefExports.h"にあります)
 *				wparam
 *						[１]：LOBYTE(LOWORD(wparam))	報告方法
 *															メッセージボックスの表示のみ
 *															アラーム報告のみ
 *															両方
 *						[２]：HIBYTE(LOWORD(wparam))	親通知(CMainFrame)
 *															しない
 *															する(その後の処理は親次第です)
 *						[３]：LOBYTE(HIWORD(wparam))	メッセージボックスのタイトル番号
 *						[４]：HIBYTE(HIWORD(wparam))	メッセージボックスのボタン表示
 *															OK
 *															YES/NO(YES の場合は親通知時のアプリ終了パラメタを TRUE にする)
 *				lparam
 *						[５]：LOWORD(lparam)			メッセージボックスのテキスト番号
 *						[６]：HIWORD(lparam)			アラーム報告の ALID
 *			指定方法
 *				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD([１], [２]), MAKEWORD([３], [４])), MAKELPARAM([５], [６]));
 */
LRESULT CChiefView::OnReportAlarms(WPARAM wparam, LPARAM lparam)
{
	if (0 != LOBYTE(LOWORD(wparam))) {
		DWORD l_dwMethod = LOBYTE(LOWORD(wparam));
		DWORD l_dwNotify = HIBYTE(LOWORD(wparam));
		DWORD l_dwMsgTitleNo = LOBYTE(HIWORD(wparam));
		DWORD l_dwMsgButton = HIBYTE(HIWORD(wparam));
		DWORD l_dwMsgTextNo = LOWORD(lparam);
		DWORD l_dwALID = HIWORD(lparam);

/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- { ---------- */
		switch (l_dwALID) {
		case ALID_CTA_Setinf:
			this->CtaReset(1);
			break;
		}
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- } ---------- */

		// アラーム報告 ------------------------------------------------------
//		if ((CHRAMTD_BOTH == l_dwMethod) || (CHRAMTD_REP_ALARM == l_dwMethod)) {
		if (0 != (0x00000002 & l_dwMethod)) {
			AlarmIf_Set(l_dwALID);
		}
		// メッセージボックス表示 --------------------------------------------
		DWORD l_dwNotifyCode = 0;
//		if ((CHRAMTD_BOTH == l_dwMethod) || (CHRAMTD_MSGBOX == l_dwMethod)) {
		if (0 != (0x00000001 & l_dwMethod)) {
			// メッセージボックスのボタン定義作成
//			UINT l_uiType = MB_ICONSTOP;
			UINT l_uiType = MB_ICONERROR;
//			UINT l_uiType = MB_ICONHAND;
			l_uiType |= MB_DEFBUTTON2;
			l_uiType |= MB_SYSTEMMODAL;
			switch (l_dwMsgButton) {
			case CHRAMSG_YESNO:
				l_uiType |= MB_YESNO;
				break;
			case CHRAMSG_OK:
			default:
				l_uiType |= MB_OK;
				break;
			}
			int l_iMB_Result = ::MessageBox(0, CHIEF_REP_ALARM_MSGTEXT[l_dwMsgTextNo], CHIEF_REP_ALARM_MSGTITLE[l_dwMsgTitleNo], l_uiType);
			// クリックボタン判定
			switch (l_dwMsgButton) {
			case CHRAMSG_YESNO:
				if (IDYES == l_iMB_Result) {
					l_dwNotifyCode = 1;		// アプリ終了指定
				}
				break;
			case CHRAMSG_OK:
			default:
				break;
			}
		}
		// 親通知 ------------------------------------------------------------
		if (CHRANFY_NOTIFY_ON == l_dwNotify) {
			m_pcMainFrame->PostMessage(WM_CHIF_ERROR_NOTIFY, (WPARAM) l_dwNotifyCode, 0);
		}
	}

	return 0L;
}

/*
 *	汎用ポジション移動指令(Pif から) メッセージハンドラ
 */
LRESULT CChiefView::OnMoveSetPosFromPif(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_CHIF_MOVE_SETPOS"), wparam, lparam);

	STAGE_COORD l_SetPosition;
	//Saiki 20090602 Add ----->
	CString strMsg;
	//Saiki 20090602 Add <-----
	long l_dwMovePosId = (long) wparam;

	if ((l_dwMovePosId < 0) || (GENERAL_POSITION_MAX < l_dwMovePosId)) {
		// 範囲外です
		//	ここには何も処理はありません．
		LogChief(_T("WM_CHIF_MOVE_SETPOS : Position No. was out of range."));
	} else {
		if (0 == l_dwMovePosId) {
			// ハンドラ ロード ポジション
			ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);
		} else if ((1 <= l_dwMovePosId) && (l_dwMovePosId <= GENERAL_POSITION_MAX)) {
			// 汎用 ポジション
			GENERAL_POSITION l_GeneralPosition[GENERAL_POSITION_MAX];
			ConfigFile_GetNanoSpecIni(l_GeneralPosition, CONFIG_FILE_GENERAL_POSITION);
			l_SetPosition = l_GeneralPosition[l_dwMovePosId - 1].Loc;
		}
		// ステージを移動
#ifndef CHIEF_STAGE_ON
		{
//	StageMoveAbsoluteEx() が復活するまで有効です．
			if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
				ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
				if (0 == l_dwMovePosId) {
					//Saiki 20090602 Change ----->
					//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
					LoadStringML(IDS_CHIF_HLDPOS_MOVING, strMsg, "Moving Load Position...");
					m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
					//Saiki 20090602 Change <-----
				} else {
					//Saiki 20090602 Change ----->
					//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVING);  // Status Bar
					LoadStringML(IDS_CHIF_GENPOS_MOVING, strMsg, "Moving General Position...");
					m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVING);	// Status Bar
					//Saiki 20090602 Change <-----
				}
			}
		}
#endif
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
		/* SR ヘッドにします．	*/
		this->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
		if (0 != ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
			// ハードシミュレートモードの場合
			if (0 == l_dwMovePosId) {
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_HLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HWS_HLDPOS_MOVING, strMsg, "[HWS]Moving Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
#ifdef CHIEF_USE_SLEEP
				::Sleep(3000);
#endif
				PifComm_LoadPositionMovementResultReport(0);
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_HLDPOS_MOVED);   // Status Bar
				LoadStringML(IDS_CHIF_HWS_HLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
				m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_HLDPOS_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
				//Saiki 20090602 Change <-----
			} else {
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_GENPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HWS_GENPOS_MOVING, strMsg, "[HWS]Moving General Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
#ifdef CHIEF_USE_SLEEP
				::Sleep(3000);
#endif
				PifComm_GeneralPurposePositionMovementResultReport(0);
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_GENPOS_MOVED);   // Status Bar
				LoadStringML(IDS_CHIF_HWS_GENPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
				m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_GENPOS_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
				//Saiki 20090602 Change <-----
			}
		} else
#ifndef CHIEF_STAGE_ON
		NS_ConvertToStageMoveCoord(&l_SetPosition);					// 補正
//		if (0 == StageMoveAbsoluteEx(&l_SetPosition, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
		if (0 == StageMoveAbsolute(&l_SetPosition)) {
#else
		if (0 == NS_StageMoveAbsoluteEx(&l_SetPosition)) {
#endif
			// 失敗
			if (0 == l_dwMovePosId) {
				// ステージ移動開始失敗(StageMoveAbsolute())
				PifComm_LoadPositionMovementResultReport(2);
			} else {
				// ステージ移動開始失敗(StageMoveAbsolute())
				PifComm_GeneralPurposePositionMovementResultReport(2);
			}
#ifndef CHIEF_STAGE_ON
			{
//	StageMoveAbsoluteEx() が復活するまで有効です．
				ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 動作中フラグ(XY ステージ)をオフ
			}
#endif
		} else {
			// 成功
			if (0 == l_dwMovePosId) {
				m_dwStageMoveState = 2;			// ハンドラ ロード ポジション 移動中
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HLDPOS_MOVING, strMsg, "Moving Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			} else {
				m_dwStageMoveState = 3;			// 汎用 ポジション 移動中
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_GENPOS_MOVING, strMsg, "Moving General Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			}
#ifdef CHIEF_STAGE_ON
				ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
#endif
		}
	}

	return 0L;
}

/*
 *	汎用ポジション移動指令(画面から) メッセージハンドラ
 */
LRESULT CChiefView::OnMoveSetPosFromDisp(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_MOVE_SETPOS"), wparam, lparam);
	//Saiki 20090602 Add ----->
	CString strMsg;
	//Saiki 20090602 Add <-----

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		// EMO，ドアインタロック，装置パワーオフ
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) (30 + l_dwEMO), 0);
		return 0L;
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		// ロボットアーム検出
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 34, 0);
		return 0L;
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		// ピンダウンがオフ
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 35, 0);
		return 0L;
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		// エア圧力低下がオン
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 36, 0);
		return 0L;
	}
	if (
		(0 == m_DiInfo.bTHMaintenanceSW)
	 && ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen))
	) {
		// 東朋メンテがオフで，かつ，シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 37, 0);
		return 0L;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		/* CTAILPI がオフ	*/
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 50, 0);
		return 0L;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		if (1 == iResistStatus) {
			/* プローブ上昇端センサー がオフ	*/
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 52, 0);
		} else if (2 == iResistStatus) {
			/* プローブ上昇端・下降端センサー が両方オン	*/
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 53, 0);
		} else {
			/* プローブ上昇端・下降端センサー が両方オフ	*/
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 54, 0);
		}
		return 0L;
	}
	//2009.11.12 bagus MS --{--
	//下端位置２のインターロックを確認する
	if (0 == this->IsMSILPI()) {
		/* CTAILPI がオフ	*/
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 55, 0);
		return 0L;
	}
	//2009.11.12 bagus MS --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		// ローカルでなかった
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 2, 0);
		return 0L;
	}
#if 0	// ターレット動作中をフラグに追加したため，ターレットは除外
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
#else	// ターレット動作中をフラグに追加したため，ターレットは除外
	DWORD l_dwActFlags = ACTUATE_XYSTAGE | ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_SHUTTER | ACTUATE_WORKGUIDE;
	if (0 != (l_dwActFlags & ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
#endif	// ターレット動作中をフラグに追加したため，ターレットは除外
		// 動作中フラグがオンだった
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 6, 0);
		return 0L;
	}
	int l_iProcStatus = ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		// Wait，Down 以外だった
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 8, 0);
		return 0L;
	}
	if (MAIN_MENU_DATA == ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()) {
		// 画面モードが「データモード」だった
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 11, 0);
		return 0L;
	}
	if (0 == m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == m_DiInfo.bTHMaintenanceSW) {
			// さらに，東朋メンテ SW がオフの場合，禁止
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 15, 0);
			return 0L;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 15, 0);
			return 0L;
		}
#endif
	}

	STAGE_COORD l_SetPosition;
	long l_dwMovePosId = (long) wparam;

	if ((l_dwMovePosId < -1) || (GENERAL_POSITION_MAX < l_dwMovePosId)) {
		// 範囲(-1～GENERAL_POSITION_MAX)外エラー
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 1, 0);
		LogChief(_T("WM_DISP_MOVE_SETPOS : Position No. was out of range."));
	} else {
		if (-1 == l_dwMovePosId) {
			// マニュアル ロード ポジション
			ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_MANUAL_LOAD_POSITION);
		} else if (0 == l_dwMovePosId) {
			// ハンドラ ロード ポジション
			ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);
		} else if (( 1<= l_dwMovePosId) && (l_dwMovePosId <= GENERAL_POSITION_MAX)) {
			// 汎用 ポジション
			GENERAL_POSITION l_GeneralPosition[GENERAL_POSITION_MAX];
			ConfigFile_GetNanoSpecIni(l_GeneralPosition, CONFIG_FILE_GENERAL_POSITION);
			l_SetPosition = l_GeneralPosition[l_dwMovePosId - 1].Loc;
		}
		// ステージを移動 ---------------------------------
#ifndef CHIEF_STAGE_ON
		{
//	StageMoveAbsoluteEx() が復活するまで有効です．
			if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
				ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
				if (-1 == l_dwMovePosId) {
					//Saiki 20090602 Change ----->
					//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);  // Status Bar
					LoadStringML(IDS_CHIF_MLDPOS_MOVING, strMsg, "Moving Manual Load Position...");
					m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
					//Saiki 20090602 Change <-----
				} else if (0 == l_dwMovePosId) {
					//Saiki 20090602 Change ----->
					//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
					LoadStringML(IDS_CHIF_HLDPOS_MOVING, strMsg, "Moving Load Position...");
					m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
					//Saiki 20090602 Change <-----
				} else {
					//Saiki 20090602 Change ----->
					//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);  // Status Bar
					LoadStringML(IDS_CHIF_MLDPOS_MOVING, strMsg, "Moving Manual Load Position...");
					m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);	// Status Bar
					//Saiki 20090602 Change <-----
				}
			}
		}
#endif
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
		/* SR ヘッドにします．	*/
		this->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
		if (0 != ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
			// ハードシミュレートモードの場合
			if (-1 == l_dwMovePosId) {
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_MLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HWS_MLDPOS_MOVING, strMsg, "[HWS]Moving Manual Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
#ifdef CHIEF_USE_SLEEP
				::Sleep(3000);
#endif
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_MLDPOS_MOVED);   // Status Bar
				LoadStringML(IDS_CHIF_HWS_MLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
				m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
				//Saiki 20090602 Change <-----
			} else if (0 == l_dwMovePosId) {
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_HLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HWS_HLDPOS_MOVING, strMsg, "[HWS]Moving Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
#ifdef CHIEF_USE_SLEEP
				::Sleep(3000);
#endif
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_HLDPOS_MOVED);   // Status Bar
				LoadStringML(IDS_CHIF_HWS_HLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
				m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
				//Saiki 20090602 Change <-----
			} else {
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_GENPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HWS_GENPOS_MOVING, strMsg, "[HWS]Moving General Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
#ifdef CHIEF_USE_SLEEP
				::Sleep(3000);
#endif
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_GENPOS_MOVED);   // Status Bar
				LoadStringML(IDS_CHIF_HWS_GENPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
				m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
				//Saiki 20090602 Change <-----
			}
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, 0, 0);
		} else
#ifndef CHIEF_STAGE_ON
		NS_ConvertToStageMoveCoord(&l_SetPosition);
//		if (0 == StageMoveAbsoluteEx(&l_SetPosition, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
		if (0 == StageMoveAbsolute(&l_SetPosition)) {
#else
		if (0 == NS_StageMoveAbsoluteEx(&l_SetPosition)) {
#endif
			// 失敗
#ifndef CHIEF_STAGE_ON
			{
//	StageMoveAbsoluteEx() が復活するまで有効です．
				ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 動作中フラグ(XY ステージ)をオフ
			}
#endif
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 1, 0);
		} else {
			// 成功
			if (-1 == l_dwMovePosId) {
				m_dwStageMoveState = 11;		// 画面マニュアル ロード ポジション 移動中
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_MLDPOS_MOVING, strMsg, "Moving Manual Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			} else if (0 == l_dwMovePosId) {
				m_dwStageMoveState = 12;		// 画面ハンドラ ロード ポジション 移動中
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HLDPOS_MOVING, strMsg, "Moving Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			} else {
				m_dwStageMoveState = 13;		// 画面汎用 ポジション 移動中
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_MLDPOS_MOVING, strMsg, "Moving Manual Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			}
#ifdef CHIEF_STAGE_ON
			ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
#endif
		}
	}

	return 0L;
}

/*
 *	ステージの停止状態を確認
 *		タイマなどでポーリングにすること
 */
void CChiefView::IsStageIdle()
{
	// ステージに移動指令を出している時のみ
	if (0 != m_dwStageMoveState) {
#ifndef CHIEF_STAGE_ON
		if (0 != StageIsIdle()) {
#else
		if (0 != NS_IsIdleStageMoveAsync()) {
#endif
			// XY 軸を停止
/* added 2009.11.04 hmenjo ステージ停止検出ログ ---------- { ---------- */
			TCHAR l_tszLog[128];
			_stprintf(l_tszLog, _T("Detected Stage stopping (m_dwStageMoveState = %d). Notify WM_CHIF_STAGE_STOP."), m_dwStageMoveState);
			this->LogChief(l_tszLog);
/* added 2009.11.04 hmenjo ステージ停止検出ログ ---------- } ---------- */
			StageStop();			// 戻り値は無視しています
			PostMessage(WM_CHIF_STAGE_STOP, 0, 0);
			ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 動作中フラグ(XY ステージ)をオフ
		}
	}
}

/*
 *	ステージ停止検出 メッセージハンドラ
 */
LRESULT CChiefView::OnStageStop(WPARAM wparam, LPARAM lparam)
{
	//Saiki 20090602 Add ----->
	CString strMsg;
	//Saiki 20090602 Add <-----

	// ステージに移動指令を出している時のみ
	switch (m_dwStageMoveState) {
	case 2:		// Pif ハンドラ ロード ポジション 移動中
		// Pif ハンドラ ロード ポジション 移動 完了
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVED);   // Status Bar
		LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected Pif-Load position move stopping."));
		PifComm_LoadPositionMovementResultReport(0);
		break;
	case 3:		// Pif 汎用 ポジション 移動中
		// Pif 汎用 ポジション 移動 完了
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVED);   // Status Bar
		LoadStringML(IDS_CHIF_GENPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected Pif-General position move stopping."));
		PifComm_GeneralPurposePositionMovementResultReport(0);
		break;
	case 11:	// 画面 マニュアル ロード ポジション 移動中
	case 12:	// 画面 ハンドラ ロード ポジション 移動中
	case 13:	// 画面 汎用 ポジション 移動中
		// 画面 マニュアル/ロード/汎用 ポジション移動 完了
		switch (m_dwStageMoveState) {
		case 11:
			//Saiki 20090602 Change ----->
			//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVED);   // Status Bar
			LoadStringML(IDS_CHIF_MLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
			//Saiki 20090602 Change <-----
			LogChief(_T("Detected Disp-Man position move stopping."));
			break;
		case 12:
			//Saiki 20090602 Change ----->
			//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVED);   // Status Bar
			LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
			//Saiki 20090602 Change <-----
			LogChief(_T("Detected Disp-Load position move stopping."));
			break;
		case 13:
		default:
			//Saiki 20090602 Change ----->
			//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVED);   // Status Bar
			LoadStringML(IDS_CHIF_GENPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
			//Saiki 20090602 Change <-----
			LogChief(_T("Detected Disp-General position move stopping."));
			break;
		}
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 0, 0);
		break;
	case 21:	// SR リファレンス測定 (ノーマル)リファレンス測定位置 移動中
		// 移動完了
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_STAGE_MOVED); // Status Bar
		LoadStringML(IDS_CHIF_SREF_N_STAGE_MOVED, strMsg, "Reference Measurement:(Normal)Reference Measuremnt Position(XY) Move Complete");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg); // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		m_pcMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected SR (Normal) Reference position move stopping."));
		WaitTimeBeforeMeas(0);		// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)の時刻取得
		((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_MOVE_DONE);
		break;
	case 22:	// SR リファレンス測定 ダークリファレンス測定位置 移動中
		// 移動完了
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_STAGE_MOVED); // Status Bar
		LoadStringML(IDS_CHIF_SREF_D_STAGE_MOVED, strMsg, "Reference Measurement:Dark Reference Measuremnt Position(XY) Move Complete");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg); // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		m_pcMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected SR Dark Reference position move stopping."));
		WaitTimeBeforeMeas(0);		// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)の時刻取得
		((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_D_MOVE_DONE);
		break;
	case 31:	// シーケンス測定位置 移動中
	case 32:	// シーケンス測定位置 先行移動中
		// 移動完了
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	  if (32 != m_dwStageMoveState) {
		/* 先行移動時はステータスバー表示はさわらない．	*/
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_STAGE_MOVED);	  // Status Bar
		LoadStringML(IDS_CHIF_SEQ_STAGE_MOVED, strMsg, "Sequence Measurement:Measurement Position(XY)Complete");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg);	  // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		m_pcMainFrame->SetMessageText(_T(" "));    // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		//Saiki 20090602 Change <-----
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	  }
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
		LogChief(_T("Detected SEQ position move stopping."));
		WaitTimeBeforeMeas(0);		// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)の時刻取得
/* modified 2013.02.01 hmenjo PreAF ---------- { ---------- */
//		((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_POINT_MOVE_DONE);
/* modified 2013.02.01 hmenjo PreAF ----------              */
		if ((32 == m_dwStageMoveState) && (1 == m_lPreAFafterPreMove)) {
			if (FALSE != this->IsBusyTransi(3, 7)) {
				this->LogChief(_T("PreAF - AF Seq is busy."));
				/* AF トランジションがビジーでした．	*/
				m_lPreAFafterPreMove = 0;
				/* Pre AF は行いません．	*/
			} else {
				/* フィルタ オープン	*/
				if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
					// 異常時でも無視します．
					this->LogChief(_T("PreAF - Failed to MEAS_SrHead_ChangeCcdShutter()."));
				}
				this->WaitTimeBeforeMeas(1);
				/* AF 発行	*/
				((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(0, 0), 0));
				this->LogChief(_T("PreAF - Start AF Seq."));
			}
		} else {
			((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_POINT_MOVE_DONE);
		}
/* modified 2013.02.01 hmenjo PreAF ---------- } ---------- */
		break;
/* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- { ---------- */
	case 72:	/* ストレス Seq ハンドラ ロード ポジション 移動中	*/
		LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
		LogChief(_T("Detected Stress-Load position move stopping."));
		((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LDPOS_DONE);
		break;
/* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- } ---------- */
/* added 2009.08.03 hmenjo ストレス機能追加(18) ---------- { ---------- */
	case 73:	/* ストレス Seq ライン動作 移動中	*/
		LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
		LogChief(_T("Detected Stress-Line move stopping."));
		((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
		break;
/* added 2009.08.03 hmenjo ストレス機能追加(18) ---------- } ---------- */
	default:
		break;
	}
	if (0 != m_dwStageMoveState) {
		m_dwStageMoveState = 0;
	}

	return 0L;
}

/*
 *	バキューム ON/OFF 指令 メッセージハンドラ
 */
LRESULT CChiefView::OnVacuumFromDisp(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_VACUUM_ONOFF"), wparam, lparam);

	// インタロック
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		// ローカルでなかった
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 2, 0);
		return 0L;
	}
	int l_iProcStatus = ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		// Wait，Down 以外だった
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 8, 0);
		return 0L;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_DATA == l_iDispStatus) || (MAIN_MENU_USER_SETTING == l_iDispStatus)) {
		// 画面モードが「データモード」か「ユーザ設定モード」だった
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 14, 0);
		return 0L;
	}
	if (
		(0 == m_DiInfo.bTHMaintenanceSW)
	 && ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen))
	) {
		// 東朋メンテがオフで，かつ，シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 37, 0);
		return 0L;
	}

	BOOL l_bNexResult;
	if (0 == wparam) {
		// OFF 指令
		l_bNexResult = nexifVacuumOff(this->m_hWnd);
		if (0 == l_bNexResult) {
			m_bVacuumOffFromDisp = TRUE;
		} else {
			m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_bNexResult, 0);
		}
	} else {
		// ON 指令
		l_bNexResult = nexifVacuumOn(this->m_hWnd);
		if (0 == l_bNexResult) {
			m_bVacuumOnFromDisp = TRUE;
		} else {
			m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_bNexResult, 0);
		}
	}

	return 0L;
}

/*
 *	WM_COPYDATA メッセージハンドラ
 */
BOOL CChiefView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

// 2009.03.19 k-matsuo change --->
//	switch (pCopyDataStruct->dwData) {
//	case WMCD_CHIF_RECIPE_NOTIFY:	// Pif からの着工レシピ通知
//		DWORD l_dwLength;
//		if (sizeof(m_szMainRecipeName) < pCopyDataStruct->cbData) {
//			l_dwLength = sizeof(m_szMainRecipeName);
//		} else {
//			l_dwLength = pCopyDataStruct->cbData;
//		}
//		memcpy(m_szMainRecipeName, pCopyDataStruct->lpData, l_dwLength);
//		m_szMainRecipeName[RECIPE_NAME_LEN] = 0;
//		this->PostMessage(WM_CHIF_SET_RECIPE, 0, 0);	// 測定開始時の方が良いかもしれません．
//		return 1;
//		break;
//	default:
//		break;
//	}

	DWORD l_dwLength;

	switch (pCopyDataStruct->dwData) {
	case WMCD_CHIF_RECIPE_NOTIFY:	// Pif からの着工レシピ通知
		if (RECIPE_NAME_LEN < pCopyDataStruct->cbData) {
			l_dwLength = RECIPE_NAME_LEN;
		} else {
			l_dwLength = pCopyDataStruct->cbData;
		}
		memcpy(m_szMainRecipeName, pCopyDataStruct->lpData, l_dwLength);
		m_szMainRecipeName[l_dwLength] = 0;
		this->PostMessage(WM_CHIF_SET_RECIPE, 0, 0);	// 測定開始時の方が良いかもしれません．
		return 1;
		break;
	default:
		break;
	}
// 2009.03.19 k-matsuo change <---

	return CFormView::OnCopyData(pWnd, pCopyDataStruct);
}

/*
 *	レシピ設定 メッセージハンドラ
 *		Pif から通知された着工レシピを画面モジュールに通知します．
 */
LRESULT CChiefView::OnSetRecipe(WPARAM wparam, LPARAM lparam)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Notifyed Recipe(%s) from Pif to Dis."), m_szMainRecipeName);
	LogChief(l_tszLogText);

	// 画面モジュールに通知
	COPYDATASTRUCT l_CopyData;
	l_CopyData.dwData = WMCD_CHIF_SET_RECIPE;
	l_CopyData.cbData = RECIPE_NAME_LEN + 1;
	l_CopyData.lpData = m_szMainRecipeName;
	m_pcMainFrame->SendMessage(WM_COPYDATA, (WPARAM) this->m_hWnd, (LPARAM) &l_CopyData);

	return 0L;
}

/*
 *	AF ポップアップ終了 メッセージハンドラ
 *		画面からの AF のポップアップが終了した．
 */
LRESULT CChiefView::OnAFPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_AF_POPUP_END"), wparam, lparam);

	switch (wparam) {
	case 0:		// リトライ ボタン
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_RETRY, (cEventParams*) lparam);
		break;
	case 1:		// AF 無視 ボタン
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_IGNORE, (cEventParams*) lparam);
		break;
	case 2:		// スキップ ボタン
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_SKIP, (cEventParams*) lparam);
		break;
	case 3:		// キャンセル ボタン
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_CANCEL, (cEventParams*) lparam);
		break;
	default:
		// 何もしません．
		break;
	}

	return 0L;
}

/*
 *	マニュアルサイトパターン ポップアップ終了 メッセージハンドラ
 *		画面からの マニュアルサイトパターン のポップアップが終了した．
 */
LRESULT CChiefView::OnManualSitePtPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MANUALSITE_POPUP_END"), wparam, lparam);
	DWORD dwAFValid = 0;
	AF_VALID*	l_pAFValid = (AF_VALID*) &dwAFValid;

	switch (wparam) {
	//Saiki 20090728 Add ----->
	case 1:		//Manual Site PR AF成功
		l_pAFValid->ucResultPopup = 2;		// 閉じる(OK)ボタン
		l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(lparam)))? 0 : 1;
		l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(lparam)))? 0 : 1;
		l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(lparam)))? 0 : 1;
		l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(lparam)))? 0 : 1;

		if(l_pAFValid->bit1ManuAfRslt == 0){
			l_pAFValid->ucResultAF = 1;
		} else {
			l_pAFValid->ucResultAF = 4;
		}

		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->m_dwAFValid = l_pAFValid->dwValid;
		break;
	case 2:		//Manual Site PR AF失敗
		l_pAFValid->ucResultPopup = 4;		// キャンセルボタン
		l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(lparam)))? 0 : 1;
		l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(lparam)))? 0 : 1;
		l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(lparam)))? 0 : 1;
		l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(lparam)))? 0 : 1;

		if(l_pAFValid->bit1ManuAfRslt == 0){
			l_pAFValid->ucResultAF = 1;
		} else {
			l_pAFValid->ucResultAF = 4;
		}

		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->m_dwAFValid = l_pAFValid->dwValid;
		break;
	//Saiki 20090728 Add <-----
	default:
		// 何もしません．
		break;
	}

	return 0L;
}

/*
 *	カレントメインレシピ名を取得します．
 */
void CChiefView::GetCurrentMainRecipeName(TCHAR *pszCurrentMainRecipeName)
{
	_tcscpy(pszCurrentMainRecipeName, m_szMainRecipeName);
}

/*
 *	データ受信開始 メッセージハンドラ
 *		測定モジュールで CCD データ受信完了し，先行移動が可能になった通知
 */
LRESULT CChiefView::OnMeasRecvData(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MEAS_RECVDATA (PreMove)"), wparam, lparam);

/* modified 2009.08.06 hmenjo ストレス機能追加(28) ---------- { ---------- */
//	((CChiefTransiSeq*) m_pcChiefTransiSeq)->PreMove();
/* modified 2009.08.06 hmenjo ストレス機能追加(28) ----------			   */
	if (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle()) {
		((CChiefTransiSeq*) m_pcChiefTransiSeq)->PreMove();
	} else if (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle()) {
		((CChiefTransiStress*) m_pcChiefTransiStress)->PreMove();
	}
/* modified 2009.08.06 hmenjo ストレス機能追加(28) ---------- } ---------- */

	return 0L;
}

/*
 *	各種レシピを読込みます
 */
DWORD CChiefView::RecipesGet(
		TCHAR *pszMainRecipName,	// メインレシピ名
		DWORD dwMode				// ＝0：ステージプログラムを含める，≠0：含めない
	)
{
	// メインレシピ
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
	if (0 == RecipeFile_LoadRecipe(l_pMainRcpInfo, pszMainRecipName, RECIPE_FILE_MAIN_RECIPE)) {
		// メインレシピ読込み異常
		return 1;
	}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- { ---------- */
	WORD l_wHeadTypeMainRcp = l_pMainRcpInfo->MainRcpParam.hdr.wHeadType;
	if (HEAD_TYPE_STRESS == l_wHeadTypeMainRcp) {
		if (0 == l_pMainRcpInfo->MainRcpParam.hdr.bSampleID) {
			/* メインレシピ読込み異常(サンプル ID 使用になっていない)	*/
			return 1;
		}
//Saiki 20110311 Add ----->
		LPSTRESS_PARAM_INFO l_pStressParamInfo = (LPSTRESS_PARAM_INFO) m_ChiefRecipes.pStressParamInfo;
		if (0 == RecipeFile_LoadStressParam(l_pStressParamInfo, pszMainRecipName, RECIPE_FILE_STRESS_MAIN_RECIPE)){
			return 1;
		}
//Saiki 20110311 Add <-----
	}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- } ---------- */
	// 測定プログラム
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
	if (0 == RecipeFile_LoadRecipe(l_pMeasProgInfo, l_pMainRcpInfo->MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM)) {
		// 測定プログラム読込み異常
		return 2;
	} else {
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- { ---------- */
		if (l_wHeadTypeMainRcp != l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
			/* 測定プログラム読込み異常(ヘッドタイプ不一致)	*/
			return 2;
		}
		if (0 == ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsValidScanType(l_pMeasProgInfo->ScanParams.hdr.wScanType, l_wHeadTypeMainRcp)) {
			/* 測定プログラム読込み異常(ヘッドタイプとスキャンタイプ不一致)	*/
			return 2;
		}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- } ---------- */
		// レシピ不足情報を追加
		switch (l_pMeasProgInfo->ScanParams.hdr.wScanType) {
		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:		// 反射色度
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:	// 透過色度
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:		// 光学濃度
			l_pMeasProgInfo->ScanParams._SR.WavelenRange.wStart = CIE_MINWAVE;
			l_pMeasProgInfo->ScanParams._SR.WavelenRange.wEnd = CIE_MAXWAVE;
			break;
		default:
			// 何も処理しません．
			break;
		}
	}

/* modified 2009.11.20 hmenjo CTA Stage PGM 読込み ---------- { ---------- */
///* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
//	/* ステージプログラムを読込み	*/
//		/*	CTA の基板厚みを取得しなければならないため，「ステージプログラムを含めない」設定でも，
//			ココで，ステージプログラムを読込みます．	*/
//	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
//	BOOL l_bStgPgmValid = RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
//	if (0 == l_bStgPgmValid) {
//		/* ステージプログラム読込み異常	*/
//		return 3;
//	}
///* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */
/* modified 2009.11.20 hmenjo CTA Stage PGM 読込み ----------			   */
	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;

// 2014.01.07 bagus Add(Stage None対応) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	if(l_SystemConfig.nStageType == STAGE_TYPE_NONE){
		::ZeroMemory(l_pStageProgInfoHdr, sizeof(_STAGE_PROG_INFO_HDR));
		l_pStageProgInfoHdr->wNumScans = 1;
		return 0;
	}
// 2014.01.07 bagus Add(Stage None対応) <--

	if (HEAD_TYPE_CTA == l_wHeadTypeMainRcp) {
		/* ココでは CTA の場合のみステージプログラムを読込み	*/
		/*	CTA の基板厚みを取得しなければならないため，「ステージプログラムを含めない」設定でも，
			ココで，ステージプログラムを読込みます．	*/
		BOOL l_bStgPgmValid = RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
		if (0 == l_bStgPgmValid) {
			/* ステージプログラム読込み異常	*/
			return 3;
		}
	}
/* modified 2009.11.20 hmenjo CTA Stage PGM 読込み ---------- } ---------- */

	switch (dwMode) {
	case 0:
		{
			// ステージプログラム
/* modified 2009.11.20 hmenjo CTA Stage PGM 読込み ---------- { ---------- */
/* deleted 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
//			LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
//			if (0 == RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM)) {
//				// ステージプログラム読込み異常
//				return 3;
//			}
/* deleted 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */
/* modified 2009.11.20 hmenjo CTA Stage PGM 読込み ----------			   */
			if (HEAD_TYPE_CTA != l_wHeadTypeMainRcp) {
				/* CTA 以外の場合のみココでステージプログラムを読込み	*/
				if (0 == RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM)) {
					// ステージプログラム読込み異常
					return 3;
				}
			}
/* modified 2009.11.20 hmenjo CTA Stage PGM 読込み ---------- } ---------- */
/* added 2009.10.15 hmenjo Stage PGM 共通化対応 ヘッドタイプ書換 ---------- { ---------- */
			/* ヘッドタイプ変換
				ステージプログラムのヘッドタイプが STD(=99) の場合に
				メインレシピのヘッドタイプが変換テーブルにあれば，
				ステージプログラムのヘッドタイプをメインレシピのヘッドタイプで
				置換します．	*/
			if (99 == l_pStageProgInfoHdr->wHeadType) {
/* modified 2009.11.20 hmenjo Stage PGM ヘッドタイプ変換検索改善 ---------- { ---------- */
//				for (int l_i = 0; l_i < 99; l_i++) {
//					if (l_wHeadTypeMainRcp == HEADTYPE_STD_TBL[l_i]) {
//						l_pStageProgInfoHdr->wHeadType = l_wHeadTypeMainRcp;
//						break;
//					}
//				}
/* modified 2009.11.20 hmenjo Stage PGM ヘッドタイプ変換検索改善 ---------- 			 */
				int l_i = 0;
// 2013.02.06 bagus bug fix -->
//				while (-1 != HEADTYPE_STD_TBL[l_i]) {
				while ((WORD)-1 != HEADTYPE_STD_TBL[l_i]) {
// 2013.02.06 bagus bug fix <--
					if (l_wHeadTypeMainRcp == HEADTYPE_STD_TBL[l_i]) {
						l_pStageProgInfoHdr->wHeadType = l_wHeadTypeMainRcp;
						break;
					}
					l_i++;
				}
/* modified 2009.11.20 hmenjo Stage PGM ヘッドタイプ変換検索改善 ---------- } ---------- */
			}
/* added 2009.10.15 hmenjo Stage PGM 共通化対応 ヘッドタイプ書換 ---------- } ---------- */
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- { ---------- */
			if (l_wHeadTypeMainRcp != l_pStageProgInfoHdr->wHeadType) {
				/* ステージプログラム読込み異常(ヘッドタイプ不一致)	*/
				return 3;
			} else if (HEAD_TYPE_STRESS == l_wHeadTypeMainRcp) {
				/* ストレスヘッドなので，ステージ PGM 読出し	*/
				LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) m_ChiefRecipes.pStageProgStress;
				if (0 == RecipeFile_LoadRecipe(l_pStageProgStress, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS)) {
					/* ステージプログラム読込み異常(ストレス用)	*/
					return 3;
				}
			}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- } ---------- */
			// SEQ 測定ポイント
			LPSTAGE_COORD l_pScanPoint = (LPSTAGE_COORD) m_ChiefRecipes.pScanPoint;
			if ((l_pStageProgInfoHdr->wNumScans <= 0) || (SCAN_POINT_MAX < l_pStageProgInfoHdr->wNumScans)) {
				// SEQ 測定ポイント数設定異常
				return 4;
			//2009.10.28 bagus 2point-distance -->
			} else if (l_pStageProgInfoHdr->wHeadType == HEAD_TYPE_SR && l_pStageProgInfoHdr->wScanType == SCAN_TYPE_SR_DISTANCE){
				if(0 == RecipeFile_Load2PointList(l_pScanPoint, l_pStageProgInfoHdr->wNumScans, l_pMainRcpInfo->MainRcpParam.hdr.szStage)) {
				// SEQ 測定ポイント読込み異常
				LogChief(_T("Failed to StagePoint RecipeFile_Load2PointList() error."));
				return 5;
				}
			} else if (0 == RecipeFile_LoadPointList(l_pScanPoint, l_pStageProgInfoHdr->wNumScans, l_pMainRcpInfo->MainRcpParam.hdr.szStage)) {
			//2009.10.28 bagus 2point-distance <--
				// SEQ 測定ポイント読込み異常
				LogChief(_T("Failed to StagePoint RecipeFile_LoadPointList() error."));
				return 5;
			} else {
				// 参照方法は，(l_pScanPoint[0]).lX な感じ
			}
/* added 2009.11.20 hmenjo CTA 1000 ポイントまでで正常完了 ---------- { ---------- */
			if (HEAD_TYPE_CTA == l_wHeadTypeMainRcp) {
				/* CTA ヘッドの場合は，	*/
				if (SCAN_POINT_CTA_MAX < l_pStageProgInfoHdr->wNumScans) {
					/* 測定ポイント数の上限を制限します．	*/
					l_pStageProgInfoHdr->wNumScans = SCAN_POINT_CTA_MAX;
				}
			}
/* added 2009.11.20 hmenjo CTA 1000 ポイントまでで正常完了 ---------- } ---------- */

// 2009.06.11 K.Matsuo -->

			// Deskew SystemConfig 有効確認
			SYSTEM_CONFIG *l_SystemConfig = &(((CMainFrame*)m_pcMainFrame)->m_SystemConfig);
			ConfigFile_GetNanoSpecIni(l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			if (l_SystemConfig->nPRMethod <= 0 || l_SystemConfig->bSamplePatternRec != TRUE) {
				// Deskew処理実施しない
				;
			} else {
				//	 Deskew実施するか？ 情報をステージＰＧＭから取得する
				char szFilePath[MAX_PATH];
				switch(l_pStageProgInfoHdr->nDeskewMode) {
				case DESKEW_MODE_PATTERN_DESKEW:
					// DeskewImageの登録確認
					_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, l_pStageProgInfoHdr->hdr.szName);
					if ( ::GetFileAttributes(szFilePath) == 0xffffffff ) {
						// 読込み異常
						LogChief(_T("Failed to Deskew Image 1 error."));
						return 3;
					}
					break;
					_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, l_pStageProgInfoHdr->hdr.szName);
					if ( ::GetFileAttributes(szFilePath) == 0xffffffff ) {
						// 読込み異常
						LogChief(_T("Failed to Deskew Image 2 error."));
						return 3;
					}
					break;
				default:
					break;
				}
			}

// 2009.06.11 K.Matsuo <--

// 2009.05.21 myanagida -->

			// サイトパターン SystemConfig 有効確認
// 2009.06.10 K.Matsuo -->
//			SYSTEM_CONFIG *l_SystemConfig = &(((CMainFrame*)m_pcMainFrame)->m_SystemConfig);
//			ConfigFile_GetNanoSpecIni(l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//			if (l_SystemConfig->bSitePatternRec != TRUE) {
			if (l_SystemConfig->nPRMethod <= 0 || l_SystemConfig->bSitePatternRec != TRUE) {
// 2009.06.10 K.Matsuo <--
				// サイトパターン処理実施しない
				;
			} else {
				//	 サイトパターン実施するか？ 情報をステージＰＧＭから取得する
				LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) m_ChiefRecipes.pSitePattern;
				switch(l_pStageProgInfoHdr->nSitePatternMode) {
				case SITE_PR_MODE_USE_PR:					// correct with use pattern recognition
					// サイトパターン テーブル
					if (0 == PatternFile_LoadPointList(l_pSitePattern, &l_pStageProgInfoHdr->wNumScans, l_pMainRcpInfo->MainRcpParam.hdr.szStage)) {
						// SitePR定義 読込み異常
						LogChief(_T("Failed to SitePr PatternFile_LoadPointList() error."));
						return 3;
					} else {
						// 参照方法は，(l_pSitePattern[0]).xxxx な感じ
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
						/* パターンイメージファイルの存在確認
							と，パターンサブ情報ファイルの存在確認	*/
						/* パターン名を収集 */
						TCHAR (*l_ptszPatName)[RECIPE_NAME_LEN + 1] = 0;
						int l_iPatCount = 0;
						LPVOID l_pVoid;
						for (int i = 0; i < l_pStageProgInfoHdr->wNumScans; i++) {
							for (int j = 0; j < SITE_PATTERN_MAX; j++) {
								if (0 != _tcscmp(l_pSitePattern[i].PatternInfo[j].szSitePatternName, _T(""))) {
									/* パターン名の登録があった． */
									if (0 == l_ptszPatName) {
										/* 初登録 */
										if (0 == (l_ptszPatName = (TCHAR(*)[RECIPE_NAME_LEN + 1]) malloc(sizeof(*l_ptszPatName)))) {
											/* メモリ取得失敗 */
											LogChief(_T("Failed to allocate memory(s) in RecipesGet()."));
											return 3;
										}
										_tcscpy(l_ptszPatName[l_iPatCount++], l_pSitePattern[i].PatternInfo[j].szSitePatternName);
									} else {
										for (int k = 0; k < l_iPatCount; k++) {
											if (0 == _tcscmp(l_ptszPatName[k], l_pSitePattern[i].PatternInfo[j].szSitePatternName)) {
												break;
											}
										}
										if (l_iPatCount <= k) {
											/* 別のパターン名を発見 */
											l_pVoid = realloc(l_ptszPatName, sizeof(*l_ptszPatName) * (l_iPatCount + 1));
											if (0 == l_pVoid) {
												/* メモリ再取得失敗 */
												LogChief(_T("Failed to re-allocate memory(s) in RecipesGet()."));
												free(l_ptszPatName);
												return 3;
											}
											l_ptszPatName = (TCHAR(*)[RECIPE_NAME_LEN + 1]) l_pVoid;
											_tcscpy(l_ptszPatName[l_iPatCount++], l_pSitePattern[i].PatternInfo[j].szSitePatternName);
										}
									}
								}
							}
						}
						/* パターンイメージファイルとサブ情報ファイルの存在確認	*/
						TCHAR l_tszSprImgPath[_MAX_PATH];
						BOOL l_bErr = FALSE;
						SPR_SUB_INFO l_PatSubInfoDmy;
						for (i = 0; i < l_iPatCount; i++) {
//							PatternFile_MakePatternImageFilePath(l_tszSprImgPath, l_ptszPatName[i], 0);
							PatternFile_MakePatternImageFilePath(l_tszSprImgPath, l_ptszPatName[i]);
							if (-1 == ::GetFileAttributes(l_tszSprImgPath)) {
								/* イメージファイルが存在しない可能性があります． */
								LogChief(_T("Failed to read Site PR image file(s) in RecipesGet()."));
								l_bErr = TRUE;
								break;
							} else if (FALSE == PatternFile_GetSubInfo(l_ptszPatName[i], &l_PatSubInfoDmy)) {
								/* サブ情報ファイルが存在しない可能性があります． */
								LogChief(_T("Failed to read Site PR sub-info file(s) in RecipesGet()."));
								l_bErr = TRUE;
								break;
							}
						}
						free(l_ptszPatName);
						if (TRUE == l_bErr) {
							return 3;
						}
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */
					}
					break;

				case SITE_PR_MODE_NO_PR:					// correct with no pattern recognition
				default:
					break;
				}
			}

// 2009.05.21 myanagida <--

		}
		break;
	case 1:
	default:
		// 何もしません．
		break;
	}

	return 0;
}

/*
 *	各種レシピ用のメモリの取得と開放を行います
 */
void CChiefView::RecipesMalloc(BOOL bAlloc)
{
	if (0 != bAlloc) {
		// 取得 ---------------------------------------------------------------
		DWORD l_dwMsgCode = 0;
		// メインレシピ
		if (0 == (m_ChiefRecipes.pMainRcpInfo = new MAIN_RCP_INFO)) {
			l_dwMsgCode = 24;	// 取得失敗
		}
		// 測定プログラム
		if (0 == (m_ChiefRecipes.pMeasProgInfo = new MEAS_PROG_INFO)) {
			l_dwMsgCode = 25;	// 取得失敗
		}
		// ステージプログラム
		if (0 == (m_ChiefRecipes.pStageProgInfoHdr = new STAGE_PROG_INFO_HDR)) {
			l_dwMsgCode = 26;	// 取得失敗
		}
		// SEQ 測定ポイント
		if (0 == (m_ChiefRecipes.pScanPoint = new STAGE_COORD[SCAN_POINT_MAX])) {
			l_dwMsgCode = 27;	// 取得失敗
		}
// 2009.05.17 myanagida -->
		// サイトパターン テーブル
		if (0 == (m_ChiefRecipes.pSitePattern = new SITE_PATTERN[SCAN_POINT_MAX])) {
			l_dwMsgCode = 36;	// 取得失敗
		}
// 2009.05.17 myanagida <--
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- { ---------- */
		/* ステージプログラム ストレス用	*/
		if (0 == (m_ChiefRecipes.pStageProgStress = new STAGE_PROG_STRESS)) {
			l_dwMsgCode = 26;	/* 取得失敗	*/
		}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- } ---------- */
//Saiki 20110308 Add ----->
		// メインレシピ ストレス測定用 パラメータ
		if (0 == (m_ChiefRecipes.pStressParamInfo = new STRESS_PARAM_INFO)) {
			l_dwMsgCode = 24;	// 取得失敗
		}
//Saiki 20110308 Add <-----
		if (0 != l_dwMsgCode) {
			LogChief(CHIEF_REP_ALARM_MSGTEXT[l_dwMsgCode]);
			::MessageBox(0, CHIEF_REP_ALARM_MSGTEXT[l_dwMsgCode], CHIEF_REP_ALARM_MSGTITLE[1], MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		}
	} else {
		// 解放 ---------------------------------------------------------------
		// メインレシピ (316 bytes：2008.09.30 現在)
		if (0 != m_ChiefRecipes.pMainRcpInfo) {
			delete m_ChiefRecipes.pMainRcpInfo;
		}
		// 測定プログラム (984 bytes：2008.09.30 現在)
		if (0 != m_ChiefRecipes.pMeasProgInfo) {
			delete m_ChiefRecipes.pMeasProgInfo;
		}
		// ステージプログラム (256 bytes：2008.09.30 現在)
		if (0 != m_ChiefRecipes.pStageProgInfoHdr) {
			delete m_ChiefRecipes.pStageProgInfoHdr;
		}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- { ---------- */
		/* ステージプログラム ストレス用	*/
		if (0 != m_ChiefRecipes.pStageProgStress) {
			delete m_ChiefRecipes.pStageProgStress;
		}
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- } ---------- */
		// SEQ 測定ポイント (8 bytes × 最大ポイント数：2008.09.30 現在)
		if (0 != m_ChiefRecipes.pScanPoint) {
			delete m_ChiefRecipes.pScanPoint;
		}
// 2009.05.17 myanagida -->
		// サイトパターン テーブル (62 bytes × 最大ポイント数：2009.05.17 現在)
		if (0 != m_ChiefRecipes.pSitePattern) {
			delete m_ChiefRecipes.pSitePattern;
		}
// 2009.05.17 myanagida <--
//Saiki 20110308 Add ----->
		if (0 != m_ChiefRecipes.pStressParamInfo) {
			delete m_ChiefRecipes.pStressParamInfo;
		}
//Saiki 20110308 Add <-----
	}
}

/*
 *	１ポイント測定終了応答 メッセージハンドラ
 *		データ処理モジュールで１ポイント測定データの処理が完了した．
 */
LRESULT CChiefView::OnData1PointEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_1POINT_END"), wparam, lparam);

	((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_ANA_DONE);

	return 0L;
}

/*
 *	１枚測定終了応答 メッセージハンドラ
 *		データ処理モジュールで１枚データの処理が完了した．
 */
LRESULT CChiefView::OnDataMeasEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_MEAS_END"), wparam, lparam);

	((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_COMPLETE);

	return 0L;
}

/*
 *	レンズ(ターレット)を設定します
 */
BOOL CChiefView::SelectLens(UINT uiLens)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Selected Lens(=%d)"), uiLens);
	LogChief(l_tszLogText);

	BOOL l_bRc = FALSE;

	ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(uiLens);
	ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	return l_bRc;
}

/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
/*
 *	ヘッドを設定します
 */
BOOL CChiefView::SelectHead(WORD wHeadType, BOOL bStageMove)
{
	BOOL l_bRc = TRUE;

	ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	l_bRc = SelectHeadTypeNotify(wHeadType, bStageMove);
	ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Selected Head(=%d), StageMove(=%d), Result(=%d)"), wHeadType, bStageMove, l_bRc);
	LogChief(l_tszLogText);

	if (0 == l_bRc) {
		AlarmIf_Set(ALID_SelectHeadError);
	}

	return l_bRc;
}
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */

// 2013.02.22 bagus Substrate thickness setting -->
BOOL CChiefView::SelectSubstrateThickIndex(int iIndex)
{
	BOOL l_bRc;

	ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	l_bRc = SelectSubThickNotifyIndex(iIndex);
	ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Selected Substrate Thickness(=%d), Result(=%d)"), iIndex, l_bRc);
	LogChief(l_tszLogText);

	if (0 == l_bRc) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}

	return l_bRc;
}

BOOL CChiefView::SelectSubstrateThick(double dVal)
{
	BOOL l_bRc;

	ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	l_bRc = SelectSubThickNotify(dVal);
	ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Selected Substrate Thickness(=%d), Result(=%d)"), (int)dVal, l_bRc);
	LogChief(l_tszLogText);

	if (0 == l_bRc) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}

	return l_bRc;
}
// 2013.02.22 bagus Substrate thickness setting <--

/*
 *	AAF 開始 メッセージハンドラ
 */
LRESULT CChiefView::OnStartAAF(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_AAF"), wparam, lparam);

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// ロボットアーム検出
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;	// ピンダウンがオフ
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;	// エア圧力低下がオン
	}
	if (
		(0 == m_DiInfo.bTHMaintenanceSW)
	 && ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen))
	) {
		return 37;		// 東朋メンテがオフで，かつ，シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI がオフ	*/
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//下端位置２のセンサーがONしている
	}
// 2009.11.12 bagus MS 下端位置２チェック --}--
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// ローカルでなかった
	}
	int l_iProcStatus = ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		return 8L;		// Wait，Down 以外だった
	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if (MAIN_MENU_DATA == l_iDispStatus) {
		return 11L;		// 画面モードが「データモード」だった
	}
	if ((MAIN_MENU_USER_SETTING == l_iDispStatus) && (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
		return 12L;		// 画面モードが「ユーザ設定モード」の場合に，動作中フラグがすべてオフでなかった
	}
#if 0	// ターレット動作中をフラグに追加したため，ターレットを含めます
	if (((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_RECIPE_SETTING == l_iDispStatus))
	 && (0 != (0x00000016 & ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()))) {
#else	// ターレット動作中をフラグに追加したため，ターレットを含めます
	if (((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_RECIPE_SETTING == l_iDispStatus))
	 && (0 != ((ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_WORKGUIDE | ACTUATE_TURRET) & ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()))) {
#endif	// ターレット動作中をフラグに追加したため，ターレットを含めます
		return 13L;		// 画面モードが「測定モード」か「レシピモード」の場合に動作中フラグで「XY ステージ」と「シャッタ」以外にオンがあった
	}

	((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UIAAF);

	return 0L;
}

/*
 *	画面からのシーケンス測定開始 メッセージハンドラ
 */
LRESULT CChiefView::OnStartDispSeq(WPARAM wparam, LPARAM lparam)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOWORD(lparam)：リメジャーモード
/*									＝０：通常モード
/*									≠０：リメジャーモード(リファレンスとデスキュー無し)
/*				HIWORD(lparam)：PMA 指定
/*									＝０：メインレシピ通り
/*									＝１：PMA 無し
/*									＝２：PMA 有り(全点)
/*									≧３：メインレシピ通り
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	LogChief_WinMsg(_T("WM_DISP_START_SEQ"), wparam, lparam);

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;		// ロボットアーム検出
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;		// ピンダウンがオフ
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;		// エア圧力低下がオン
	}
	if ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen)) {
		return 37;		// シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI がオフ	*/
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//下端位置２のセンサーがONしている
	}
// 2009.11.12 bagus MS 下端位置２チェック --}--
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// ローカルでなかった
	}
	if (PROCESS_WAIT != ProcStatusGet()) {
		return 5L;		// Wait でない
	}
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		return 6L;		// 動作中フラグがオンだった
	}
	if (MAIN_MENU_MEASUREMENT != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()) {
		return 7L;		// 画面モードが「測定モード」でなかった
	}
	if (0 == m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == m_DiInfo.bTHMaintenanceSW) {
			// さらに，東朋メンテ SW がオフの場合，禁止
			return 15;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			return 15;
		}
#endif
	}

	m_bGotRecipeFromPif = FALSE;
	// レシピ取得
	_tcscpy(m_szMainRecipeName, (TCHAR*) wparam);
	//		wparam に CMainFrame から，メインレシピ名のポインタがセットされているはずです．
	if (0 != RecipesGetCheck(m_szMainRecipeName, 0)) {
		// レシピ読込み異常なので，何もしません．
	} else {
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
		if (0 == HeadTypeCheck(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
			// ヘッドタイプが無効です．
			return 16;
		} else {
/* added 2009.09.11 hmenjo 測定前リファレンスデータ確認 ---------- { ---------- */
			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 && (0 != this->CheckReferenceData(	m_szMainRecipeName,
													l_pMeasProgInfo->Ref.hdr.dLifeTime,
													l_pMeasProgInfo->Ref2nd.bMeasure))) {
				/* ガントリ透過率測定で，リファレンスデータ異常
					(リファレンスデータファイルが存在しない，または，有効期限外)	*/
				return 48;
			}
/* added 2009.09.11 hmenjo 測定前リファレンスデータ確認 ---------- } ---------- */
/* added 2009.09.11 hmenjo 測定前ガントリ透過率ランプ確認 ---------- { ---------- */
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 && (TRUE != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsGTRLampOn())) {
				/* ガントリ透過率測定で，ガントリ透過率用ハロゲンランプがオフ	*/
				return 49;
			}
/* added 2009.09.11 hmenjo 測定前ガントリ透過率ランプ確認 ---------- } ---------- */
#ifdef CHIEF_REMEASURE_ON	// リメジャー対応 20081225
			LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
			DWORD l_dwSearchStartNo = 1;
			//2009.11.12 bagus 2point-distance --{--
			WORD wNumScans = l_pStageProgInfoHdr->wNumScans;
			if(l_pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_SR
				&& l_pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
				wNumScans *= 2;
			}
//			if (l_pStageProgInfoHdr->wNumScans < GetNextPointNo(&l_dwSearchStartNo)) {
			if (wNumScans < GetNextPointNo(&l_dwSearchStartNo)) {
			//2009.11.12 bagus 2point-distance --}--
				// 測定する/しない フラグがすべて「しない」になってます
				return 38;
			} else {
				// サンプル ID に"0x0d，0x00"を入れておく(画面側で入力されるているため)
				m_szSampleID[0] = 0x000d;
				m_szSampleID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- { ---------- */
				/* ロット ID に"0x0d，0x00"を入れておく(画面側で入力されるているため)	*/
				m_szLotID[0] = 0x000d; m_szLotID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- } ---------- */
				// シーケンス測定開始 -----------------------------------------
				// PMA 指定を処理(必要があればレシピを書換えます)
				switch (HIWORD(lparam)) {
				case 1:		// PMA 無し
					((LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_NONE;
					break;
				case 2:		// PMA 有り(全点)
					((LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_ALL_POINT_EVERY_ADJUSTMENT;
					break;
				case 0:		// メインレシピ通り
				default:	// メインレシピ通り
					// メインレシピ通りですのでレシピは変更しません．
					break;
				}
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
				if (POINT_MANUAL_ADJUSTMENT_NONE != l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment) {
					/* MS ヘッド以外で PMA 有りの場合は AF(1ポイント目だけ)するに設定します．	*/
					switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
					case HEAD_TYPE_SR:
					case HEAD_TYPE_STRESS:	l_pMainRcpInfo->MainRcpParam._SR.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
					case HEAD_TYPE_SE:		l_pMainRcpInfo->MainRcpParam._SE.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._SE.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
					case HEAD_TYPE_COMPEASE:l_pMainRcpInfo->MainRcpParam._COMPEASE.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._COMPEASE.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
					case HEAD_TYPE_4PP:		l_pMainRcpInfo->MainRcpParam._RS.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._RS.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
					case HEAD_TYPE_CTA:		l_pMainRcpInfo->MainRcpParam._CA.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._CA.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
					case HEAD_TYPE_MS:
					default:
						break;
					}
				}
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- { ---------- */
//				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) LOWORD(lparam));
/* modified 2009.09.10 hmenjo P511 コマンド追加 ----------				*/
				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) MAKELONG(LOWORD(lparam), 0));
/* modified 2009.09.10 hmenjo P511 コマンド追加 ---------- } ---------- */
			}
#else
			// サンプル ID に"0x0d，0x00"を入れておく(画面側で入力されるているため)
			m_szSampleID[0] = 0x000d;
			m_szSampleID[1] = 0x0000;
			// シーケンス測定開始
			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ);
#endif
		}
	}

	return 0L;
}

/*
 *	画面からの１ポイント測定開始 メッセージハンドラ
 */
LRESULT CChiefView::OnStart1Point(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_1POINT"), wparam, lparam);

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// ロボットアーム検出
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;	// ピンダウンがオフ
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;	// エア圧力低下がオン
	}
	if ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen)) {
		return 37;		// シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI がオフ	*/
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//下端位置２のセンサーがONしている
	}
// 2009.11.12 bagus MS 下端位置２チェック --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// ローカルでなかった
	}
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		return 6L;		// 動作中フラグがオンだった
	}
//	int l_iProcStatus = ProcStatusGet();
//	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
//		return 8L;		// Wait，Down 以外だった
//	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
// modified hmenjo 2009.01.25 ---- { ----
//	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)) {
//		return 9L;		// 画面モードが「測定モード」か「レシピモード」でなかった
//	}
//	if ((MAIN_MENU_MEASUREMENT == l_iDispStatus) && (PROCESS_WAIT != l_iProcStatus)) {
//		return 10L;		// 画面モードが「測定モード」の場合に Wait 状態でなかった
//	}
// modified hmenjo 2009.01.25 ----
	if (
		(MAIN_MENU_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_MANUAL_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)
		) {
		return 9L;		// 画面モードが「測定モード」か「マニュアル測定モード」か「レシピモード」でなかった
	}
//	if (
//		((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_MANUAL_MEASUREMENT == l_iDispStatus))
//	 && (PROCESS_WAIT != l_iProcStatus)
//		) {
//		return 10L;		// 画面モードが「測定モード」か「マニュアル測定モード」の場合に Wait 状態でなかった
//	}
// modified hmenjo 2009.01.25 ----	----

	m_bGotRecipeFromPif = FALSE;
	// レシピ取得
	_tcscpy(m_szMainRecipeName, (TCHAR*) wparam);
	//		wparam に CMainFrame から，メインレシピ名のポインタがセットされているはずです．
	if (0 != RecipesGetCheck(m_szMainRecipeName, 1)) {
		// レシピ読込み異常なので，何もしません．
	} else {
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
		if (0 == HeadTypeCheck(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
			// ヘッドタイプが無効です．
			return 16;
		} else {
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
///* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- { ---------- */
////			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT);
///* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- 			 */
//			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT, (cEventParams*) LOWORD(lparam));
///* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- } ---------- */
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- 			 */
			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT, (cEventParams*) MAKELONG(LOWORD(lparam), 0));
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
		}
	}

	return 0L;
}

/*
 *	画面からのリファレンス測定開始 メッセージハンドラ
 */
LRESULT CChiefView::OnStartSrRefer(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_SRREF"), wparam, lparam);

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// ロボットアーム検出
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;	// ピンダウンがオフ
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;	// エア圧力低下がオン
	}
	if ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen)) {
		return 37;		// シャッタ CLOSE がオフ，か，シャッタ OPEN がオン
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI がオフ	*/
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//下端位置２のセンサーがONしている
	}
// 2009.11.12 bagus MS 下端位置２チェック --}--
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// ローカルでなかった
	}
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		return 6L;		// 動作中フラグがオンだった
	}
//	int l_iProcStatus = ProcStatusGet();
//	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
//		return 8L;		// Wait，Down 以外だった
//	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
// modified hmenjo 2009.01.25 ---- { ----
//	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)) {
//		return 9L;		// 画面モードが「測定モード」か「レシピモード」でなかった
//	}
//	if ((MAIN_MENU_MEASUREMENT == l_iDispStatus) && (PROCESS_WAIT != l_iProcStatus)) {
//		return 10L;		// 画面モードが「測定モード」の場合に Wait 状態でなかった
//	}
// modified hmenjo 2009.01.25 ----
	if (
		(MAIN_MENU_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_MANUAL_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)
		) {
		return 9L;		// 画面モードが「測定モード」か「マニュアル測定モード」か「レシピモード」でなかった
	}
//	if (
//		((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_MANUAL_MEASUREMENT == l_iDispStatus))
//	 && (PROCESS_WAIT != l_iProcStatus)
//		) {
//		return 10L;		// 画面モードが「測定モード」か「マニュアル測定モード」の場合に Wait 状態でなかった
//	}
// modified hmenjo 2009.01.25 ----	----
	if (0 == m_DiInfo.bMaintenanceSW) {
		// メンテナンス SW がオン時は・・・
		if (0 == m_DiInfo.bTHMaintenanceSW) {
			// さらに，東朋メンテ SW がオフの場合，禁止
			return 15;
		}
#if 1	// メンテでなく，東朋メンテの場合に動作禁止にします
	} else {
		// メンテナンス SW がオフ時は・・・
		if (0 != m_DiInfo.bTHMaintenanceSW) {
			// 東朋メンテ SW がオンの場合，禁止
			return 15;
		}
#endif
	}

	m_bGotRecipeFromPif = FALSE;
	// レシピ取得
	_tcscpy(m_szMainRecipeName, (TCHAR*) wparam);
	//		wparam に CMainFrame から，メインレシピ名のポインタがセットされているはずです．
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
//	if (0 != RecipesGetCheck(m_szMainRecipeName, 1)) {
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- 			 */
	/* ヘッドとスキャンタイプ確認のため，ステージプログラム無しで読込み．	*/
	DWORD l_dwMode = 1;
	if (0 == RecipesGetCheck(m_szMainRecipeName, 1)) {
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
			if (0 == LOWORD(lparam)) {
				l_dwMode = 0;
			}
		}
	}
	if (0 != RecipesGetCheck(m_szMainRecipeName, l_dwMode)) {
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
		// レシピ読込み異常なので，何もしません．
	} else {
/* added 2009.09.11 hmenjo 画面からガントリ透過率起動 ---------- { ---------- */
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			&& (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
			/* ガントリ透過率リファレンスなのでステージプログラムを含めてレシピを読み直します．	*/
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
//			if (0 != RecipesGetCheck(m_szMainRecipeName, 0)) {
//				/* レシピ読込み異常なので，何もしません．	*/
//				return 0L;
//			}
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- 			 */
			if (0 == LOWORD(lparam)) {
				/* GTR 通常リファレンス(全ポイント)の場合のみ	*/
				if (0 != RecipesGetCheck(m_szMainRecipeName, 0)) {
					/* レシピ読込み異常なので，何もしません．	*/
					return 0L;
				}
			}
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
		}
/* added 2009.09.11 hmenjo 画面からガントリ透過率起動 ---------- } ---------- */
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
		if (0 == HeadTypeCheck(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
			// ヘッドタイプが無効です．
			return 16;
		} else {
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
			/* PMA 設定は無効にします．	*/
			l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_NONE;
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */
/* added 2009.09.11 hmenjo 測定前ガントリ透過率ランプ確認 ---------- { ---------- */
//			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 && (TRUE != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsGTRLampOn())) {
				/* ガントリ透過率リファレンス測定で，ガントリ透過率用ハロゲンランプがオフ	*/
				return 49;
			}
/* added 2009.09.11 hmenjo 測定前ガントリ透過率ランプ確認 ---------- } ---------- */
/* modified 2009.09.11 hmenjo 画面からガントリ透過率起動 ---------- { ---------- */
//			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISREF);
/* modified 2009.09.11 hmenjo 画面からガントリ透過率起動 ---------- 			 */
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
				/* SR ガントリ透過率リファレンス	*/
				/* 測定する/しない フラグをすべてセットします．	*/
				LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
				for (int i = 0; i < l_pStageProgInfoHdr->wNumScans; i++) {
					((CNanoSpecDoc*) m_pcNanoSpecDoc)->m_bDoPointMeasFlag[i] = TRUE;
				}
				/* サンプル ID に"0x0d，0x00"を入れておく(画面側で入力されるているため)	*/
				m_szSampleID[0] = 0x000d;
				m_szSampleID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- { ---------- */
				/* ロット ID に"0x0d，0x00"を入れておく(画面側で入力されるているため)	*/
				m_szLotID[0] = 0x000d; m_szLotID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- } ---------- */
				/* リファレンスモードで起動します．	*/
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- { ---------- */
//				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) MAKELONG(0, TRUE));
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- 			 */
				if (0 == LOWORD(lparam)) {
					/* GTR 通常リファレンス(全ポイント)	*/
					((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) MAKELONG(0, TRUE));
				} else {
					/* GTR １ポイント リファレンス	*/
					((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT, (cEventParams*) MAKELONG(LOWORD(lparam), 1));
				}
/* modified 2009.11.27 hmenjo GTR １ポイントリファレンス ---------- } ---------- */
			} else {
				/*	SR リファレンス
				 *	SR 通常透過率リファレンス
				 */
				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISREF);
			}
/* modified 2009.09.11 hmenjo 画面からガントリ透過率起動 ---------- } ---------- */
		}
	}

	return 0L;
}

// 2013.11.07 Bagus Add (TohoSpec対応) -->
/*
 *	確認画面結果メッセージハンドラ
 */
LRESULT CChiefView::OnConfirmPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_SRREF"), wparam, lparam);

	int iSrcCmdId = (int)wparam;
	int iDlgResult = (int)lparam;

	switch(iSrcCmdId){
	case 0: // set lens and filter.
		((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_SHOW_INFO_DLG_DONE);
		break;
	case 1: // normal reference expire.
		if(iDlgResult == IDYES){
			((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_CONFIRM_EXPIRE_YES);
		}
		else{
			((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_CONFIRM_EXPIRE_NO);
		}
		break;
	case 10: // move normal reference position.
		if(iDlgResult == IDOK){
			((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_CONFIRM_MOVE_OK);
		}
		else{
			((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_CONFIRM_MOVE_CANCEL);
		}
		break;
	case 11: // move dark reference position.
		if(iDlgResult == IDOK){
			((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_D_CONFIRM_MOVE_OK);
		}
		else{
			((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_D_CONFIRM_MOVE_CANCEL);
		}
		break;
	default: // other
		// ignore.
		break;
	}

	return 0L;
}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

/*
 *	各種レシピを読込みエラーチェックします
 *		読込み異常の報知だけです．
 */
DWORD CChiefView::RecipesGetCheck(TCHAR *pszMainRecipeName, DWORD dwMode)
{
	DWORD l_dwResult;
	DWORD l_dwMsgTextCode;
	DWORD l_dwALID;

	l_dwResult = RecipesGet(pszMainRecipeName, dwMode);

	switch (l_dwResult) {
	case 0:		// 正常
		break;
	case 1:		// メインレシピ読込み異常
		l_dwMsgTextCode = 28;
		l_dwALID = ALID_MainRecipeReadError;
		break;
	case 2:		// 測定プログラム読込み異常
		l_dwMsgTextCode = 29;
		l_dwALID = ALID_MeasurementProgramReadError;
		break;
	case 3:		// ステージプログラム読込み異常
		l_dwMsgTextCode = 30;
		l_dwALID = ALID_StageProgramReadError;
		break;
	case 4:		// SEQ 測定ポイント数設定異常
		l_dwMsgTextCode = 31;
		l_dwALID = ALID_SequenceMeasurementPointsSettingError;
		break;
	case 5:		// SEQ 測定ポイント読込み異常
		l_dwMsgTextCode = 32;
		l_dwALID = ALID_SequenceMeasurementPointReadError;
		break;
	default:	// その他未定義の異常
		l_dwMsgTextCode = 33;
		l_dwALID = ALID_RecipeReadError;
		break;
	}

	if (0 != l_dwResult) {
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_BOTH, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(l_dwMsgTextCode, l_dwALID));
	}

	return l_dwResult;
}

/*
 *	測定ポーズボタン メッセージハンドラ
 */
LRESULT CChiefView::OnSeqPause(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_PAUSE_SEQ"), wparam, lparam);

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
///* modified 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
////		return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
///* modified 2009.06.02 hmenjo ドア開で測定ポーズ ----------			   */
//		if (ALID_DoorOpen != l_dwEMO) {
//			return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
//		}
///* modified 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
/* modified 2009.08.17 hmenjo セーフティプラグ処理追加 ----------			   */
		if ((ALID_DoorOpen != l_dwEMO) && (ALID_SafetyPlugOpen != l_dwEMO)) {
			return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
		}
/* modified 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// ロボットアーム検出
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- { ---------- */
//		return 35;	// ピンダウンがオフ
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ----------			   */
		if (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle()) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//			if ((0 == m_DiInfo.bPinDown) && (0 != nexioIsPinAlignmentPos()) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
			if ((0 == m_DiInfo.bPinDown) && (0 != m_DiInfo.bPinAlign) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				;	/* ピン中間位置センサのみがオンは OK です．	*/
			} else {
				return 35;	/* ピン中間位置センサもオフ	*/
			}
		} else {
			return 35;	/* ピンダウンがオフ	*/
		}
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- } ---------- */
	}
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// ローカルでなかった
	}
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
//	if ((PROCESS_PROC != ProcStatusGet())
//	 || (true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		return 3L;		// Processing でない
//	}
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	if (0 == PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
		return 16L;	/* ヘッドタイプ異常	*/
	}
	if ((PROCESS_PROC != ProcStatusGet())
	 || (true == PFC_IsIdleAll(this))) {
		return 3L;		// Processing でない
	}
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		return 4L;		// 画面モードが「測定モード」か「データモード」でなかった
	}

/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
//	((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ----------			   */
	/* ポーズイベントを発行	*/
	(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iPAUSE, 0);
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */

	return 0L;
}

/*
 *	測定レジュームボタン メッセージハンドラ
 */
LRESULT CChiefView::OnSeqResume(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_RESUME_SEQ"), wparam, lparam);

	// インタロック
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMO，ドアインタロック，装置パワーオフ
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// ロボットアーム検出
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- { ---------- */
//		return 35;	// ピンダウンがオフ
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ----------			   */
		if (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle()) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//			if ((0 == m_DiInfo.bPinDown) && (0 != nexioIsPinAlignmentPos()) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
			if ((0 == m_DiInfo.bPinDown) && (0 != m_DiInfo.bPinAlign) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
				;	/* ピン中間位置センサのみがオンは OK です．	*/
			} else {
				return 35;	/* ピン中間位置センサもオフ	*/
			}
		} else {
			return 35;	/* ピンダウンがオフ	*/
		}
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- } ---------- */
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI がオフ	*/
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//下端位置２のセンサーがONしている
	}
// 2009.11.12 bagus MS 下端位置２チェック --}--
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* deleted 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
//	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
//		return 2L;		// ローカルでなかった
//	}
/* deleted 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
//	if ((PROCESS_PROC != ProcStatusGet())
//	 || (true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		return 3L;		// Processing でない
//	}
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	if (0 == PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
		return 16L;	/* ヘッドタイプ異常	*/
	}
	if ((PROCESS_PROC != ProcStatusGet())
	 || (true == PFC_IsIdleAll(this))) {
		return 3L;		// Processing でない
	}
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		return 4L;		// 画面モードが「測定モード」か「データモード」でなかった
	}
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- { ---------- */
///* added 2009.06.08 hmenjo ポーズ前の Resume 不具合対策 ---------- { ---------- */
//	/* シーケンス測定がポーズの場合のみレジューム可能です． */
//	if (ST_SEQ_PAUSE != ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentState()) {
//		return -2L;		/* ポーズ前のレジューム指令です． */
//	}
///* added 2009.06.08 hmenjo ポーズ前の Resume 不具合対策 ---------- } ---------- */
//
//	((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_RESUME);
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ----------			   */
	if (l_ChiefPFuncs.State.iPAUSE != (*l_ChiefPFuncs.GetCurrentState)(this)) {
		return -2L;		/* ポーズ前のレジューム指令です． */
	}
	(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iRESUME, 0);
/* modified 2009.08.07 hmenjo ストレス機能追加(35) ---------- } ---------- */

	return 0L;
}

/*
 *	測定キャンセルボタン メッセージハンドラ
 */
LRESULT CChiefView::OnSeqCancel(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_CANCEL_SEQ"), wparam, lparam);

	// インタロック
#if 0	// hmenjo リモートで Pif から測定中でも画面からの測定キャンセルは受付けます．
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// ローカルでなかった
	}
#endif
/* modified 2009.08.06 hmenjo ストレス機能追加(32) ---------- { ---------- */
//	if ((PROCESS_PROC != ProcStatusGet())
//	 || (true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		return 3L;		// Processing でない
//	}
/* modified 2009.08.06 hmenjo ストレス機能追加(32) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	if (0 == PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
		return 16L;	/* ヘッドタイプ異常	*/
	}
	if ((PROCESS_PROC != ProcStatusGet())
	 || (true == PFC_IsIdleAll(this))) {
		return 3L;		// Processing でない
	}
/* modified 2009.08.06 hmenjo ストレス機能追加(32) ---------- } ---------- */
#if 0	// hmenjo 画面からのキャンセルはいつでも受付けます
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		return 4L;		// 画面モードが「測定モード」か「データモード」でなかった
	}
#endif

/* modified 2009.08.06 hmenjo ストレス機能追加(32) ---------- { ---------- */
//	if (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle()) {
//		// キャンセルイベントを発行
//		((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_CANCEL);
//		m_bReqCancelComplete = TRUE;	// 測定キャンセル完了報告指令有り
//		// 測定キャンセルボタン押下報告
//		PifComm_PressMeasureCancelButtonReport();
//		return 0L;
//	} else {
//		return 17L;		// シーケンス測定トランジションがアイドルだった
//	}
/* modified 2009.08.06 hmenjo ストレス機能追加(32) ----------			   */
	if (false == (*l_ChiefPFuncs.IsIdle)(this)) {
		// キャンセルイベントを発行
		(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iCANCEL, 0);
		m_bReqCancelComplete = TRUE;	// 測定キャンセル完了報告指令有り
		// 測定キャンセルボタン押下報告
		PifComm_PressMeasureCancelButtonReport();
		return 0L;
	} else {
		return 17L;		// シーケンス測定トランジションがアイドルだった
	}
/* modified 2009.08.06 hmenjo ストレス機能追加(32) ---------- } ---------- */
}

/*
 *	測定キャンセル完了報告
 *		測定キャンセルの完了を Pif に報告します．
 */
void CChiefView::ReportCancelComplete(int iResult)
{
	if (m_bReqCancelComplete) {
		m_bReqCancelComplete = FALSE;
		PifComm_MeasureCancelResultReport(iResult);
	}
}

/*
 *	プロセスステータスをセット
 *		Chief 以下のモジュールからのアクセスを(コードも)簡単にするためにラップします
 */
void CChiefView::ProcStatusSet(int iStatus)
{
	// ステータスの変化を制限します
// modified hmenjo 2009.01.23 ---- { ----
//	if (PROCESS_DOWN == ProcStatusGet()) {
//		// DOWN 状態だった場合・・・
//		switch (iStatus) {
//		case PROCESS_PROC:
//		case PROCESS_COMP:
//		case PROCESS_ABRT:
//		case PROCESS_WAIT:
//			return;			// 何もしません．
//			break;
//		default:
//			break;
//		}
// modified hmenjo 2009.01.23 ----
	if ((PROCESS_DOWN == ProcStatusGet()) || (PROCESS_INIT == ProcStatusGet())) {
// modified hmenjo 2009.01.23 ---- } ----
		// DOWN or INIT 状態だった場合・・・
		return;			// 何もしません．
	}
	// 同じステータスに変更する場合は，何もしません．
	if (iStatus == ProcStatusGet()) {
		return;
	}

	((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetProcessStatus(iStatus);

	TCHAR l_tszLogText[256];
	//Saiki 20090527 Change ----->
	//_stprintf(l_tszLogText, _T("Changed Process Status to %s."), PROCESS_STATUS_DISP[iStatus]);
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if(l_SystemConfig.nLanguage == 0){
		_stprintf(l_tszLogText, _T("Changed Process Status to %s."), PROCESS_STATUS_DISP_ENU[iStatus]);
	}
	else{
		_stprintf(l_tszLogText, _T("Changed Process Status to %s."), PROCESS_STATUS_DISP_JPN[iStatus]);
	}
	//Saiki 20090527 Change <-----
	LogChief(l_tszLogText);
}

/*
 *	プロセスステータスを取得
 *		Chief 以下のモジュールからのアクセスを(コードも)簡単にするためにラップします
 */
int CChiefView::ProcStatusGet()
{
	return ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus();
}

/*
 *	動作中フラグをセットします
 *		Chief 以下のモジュールからのアクセスを(コードも)簡単にするためにラップします
 */
void CChiefView::ActuateFlagsSet(ACTUATE_FLAGS_SEL ActFgSel, BOOL bFlag)
{
	((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsSet(ActFgSel, bFlag);
}

/* modified 2009.07.23 hmenjo ヘッドタイプチェック関数変更 ---------- { ---------- */
///*
// *	ヘッドタイプをチェックします
// *		2008.10.27 現在は SR のみ有効です．
// */
//BOOL CChiefView::HeadTypeCheck(WORD wHeadType)
//{
//	BOOL l_bRc;
//
//	// ヘッドタイプのチェック
//	switch (wHeadType) {
//	case HEAD_TYPE_SR:
//		l_bRc = TRUE;
//		break;
//	case HEAD_TYPE_SE:
//	case HEAD_TYPE_IRSE:
//	case HEAD_TYPE_4PP:
//	case HEAD_TYPE_CTA:
//	case HEAD_TYPE_STRESS:
//	default:
//		l_bRc = FALSE;
//		break;
//	}
//
//	return l_bRc;
//}
/* modified 2009.07.23 hmenjo ヘッドタイプチェック関数変更 ----------			   */
/*
 *	ヘッドタイプをチェックします
 */
BOOL CChiefView::HeadTypeCheck(WORD wHeadType)
{
	return ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsValidHeadType(wHeadType);
}
/* modified 2009.07.23 hmenjo ヘッドタイプチェック関数変更 ---------- } ---------- */

/*
 *	Chiefダイアログ表示制御 メッセージハンドラ
 */
LRESULT CChiefView::OnDlgShowSW(WPARAM wparam, LPARAM lparam)
{
	switch (wparam) {
	case 0:
		GetParentFrame()->ShowWindow(SW_HIDE);		// 非表示
		break;
	default:
		GetParentFrame()->ShowWindow(SW_SHOWNORMAL);	// 表示
		GetParentFrame()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	}

	return 0L;
}

/*
 *	DIO 入力をチェック － 非常停止系の入力をチェック
 *			EMO					FALSE
 *			ドアインタロック	FALSE
 *			装置パワーオフ		FALSE
 *		を検出します．
 */
DWORD CChiefView::CheckDIO_IsEMO()
{
	DWORD l_dwRc;

	if (0 == m_DiInfo.bEMOStop) {
		l_dwRc = ALID_EMOSwitchOn;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
	} else if ((0 == m_DiInfo.bSafetyPlug) && (0 == m_DiInfo.bTHMaintenanceSW)) {
		l_dwRc = ALID_SafetyPlugOpen;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
	} else if ((0 == m_DiInfo.bDoorInterlock) && (0 == m_DiInfo.bTHMaintenanceSW)) {
		l_dwRc = ALID_DoorOpen;
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- { ----------
		// メンテで，ドア開検出禁止のときは異常にしません．
		if ((0 == m_DiInfo.bMaintenanceSW) && (FALSE == g_bIL_DoorOpen)) {
			l_dwRc = 0;
		}
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- } ----------
	} else if (0 == m_DiInfo.bEQPower) {
		l_dwRc = ALID_EquipmentPowerOff;
	} else {
		l_dwRc = 0;
	}

	return l_dwRc;
}

/*
 *	DIO 入力をチェック － ロボットアーム 検出
 */
BOOL CChiefView::CheckDIO_IsRobotArmON()
{
	if (0 != m_DioIgnoreSW.bRobotArm) {
		return FALSE;
	}

	if (0 == m_DiInfo.bRobotArm) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*
 *	DIO 入力をチェック － ピンダウン 検出
 */
BOOL CChiefView::CheckDIO_IsPinDownOFF()
{
	if (0 != m_DioIgnoreSW.bPinDown) {
		return FALSE;
	}

	if (0 == m_DiInfo.bPinDown) {
		return TRUE;		// ピン下降端センサがオフ
	}
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//	if (0 != nexioIsPinAlignmentPos()) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
	if (0 != m_DiInfo.bPinAlign) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
		return TRUE;		// ピン中間位置センサがオン
	}
	if (0 != m_DiInfo.bPinUp) {
		return TRUE;		// ピン上昇端センサがオン
	}

	return FALSE;
}

/*
 *	DIO 入力をチェック － エア圧力低下 検出
 */
BOOL CChiefView::CheckDIO_IsAirPressureLowON()
{
	if (0 != m_DioIgnoreSW.bAirPressureLow) {
		return FALSE;
	}

	if (0 == m_DiInfo.bAirPressureLow) {
		return FALSE;
	} else {
		return TRUE;
	}
}

// 2013.01.10 bagus stage driver alarm io -->
/*
 * DIO 入力をチェック － X1軸アラーム検出
 *  0x0000: No Error
 *  0x1000: Stage Alarm Error
 *  0x0100: Alarm bit 0 On
 *  0x0010: Alarm bit 1 On
 *  0x0001: Alarm bit 2 On
 */
int CChiefView::CheckDIO_IsX1AxisAlarm()
{
	int iRet = 0;

	// Stage Alarm Error
	if (0 != m_DiInfo.bStageAlarm) {
		iRet |= 0x1000;
	}

	// X1AxisAlarm0
	if (0 != m_DiInfo.bX1AxisAlarm[0]) {
		iRet |= 0x0100;
	}

	// X1AxisAlarm1
	if (0 != m_DiInfo.bX1AxisAlarm[1]) {
		iRet |= 0x0010;
	}

	// X1AxisAlarm2
	if (0 != m_DiInfo.bX1AxisAlarm[2]) {
		iRet |= 0x0001;
	}

	return iRet;
}

/*
 * DIO 入力をチェック － X2軸アラーム検出
 *  0x0000: No Error
 *  0x1000: Stage Alarm Error
 *  0x0100: Alarm bit 0 On
 *  0x0010: Alarm bit 1 On
 *  0x0001: Alarm bit 2 On
 */
int CChiefView::CheckDIO_IsX2AxisAlarm()
{
	int iRet = 0;

	// Stage Alarm Error
	if (0 != m_DiInfo.bStageAlarm) {
		iRet |= 0x1000;
	}

	// X2AxisAlarm0
	if (0 != m_DiInfo.bX2AxisAlarm[0]) {
		iRet |= 0x0100;
	}

	// X2AxisAlarm1
	if (0 != m_DiInfo.bX2AxisAlarm[1]) {
		iRet |= 0x0010;
	}

	// X2AxisAlarm2
	if (0 != m_DiInfo.bX2AxisAlarm[2]) {
		iRet |= 0x0001;
	}

	return iRet;
}

/*
 * DIO 入力をチェック － Y軸アラーム検出
 *  0x0000: No Error
 *  0x1000: Stage Alarm Error
 *  0x0100: Alarm bit 0 On
 *  0x0010: Alarm bit 1 On
 *  0x0001: Alarm bit 2 On
 */
int CChiefView::CheckDIO_IsYAxisAlarm()
{
	int iRet = 0;

	// Stage Alarm Error
	if (0 != m_DiInfo.bStageAlarm) {
		iRet |= 0x1000;
	}

	// YAxisAlarm0
	if (0 != m_DiInfo.bYAxisAlarm[0]) {
		iRet |= 0x0100;
	}

	// YAxisAlarm1
	if (0 != m_DiInfo.bYAxisAlarm[1]) {
		iRet |= 0x0010;
	}

	// YAxisAlarm2
	if (0 != m_DiInfo.bYAxisAlarm[2]) {
		iRet |= 0x0001;
	}

	return iRet;
}

// 2013.01.10 bagus stage driver alarm io <--

/*
 *	異常リセット指令 メッセージハンドラ
 *		画面などからの異常(アラーム)リセット操作です
 */
LRESULT CChiefView::OnResetAlarm(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_CHIF_RESET_ALARM"), wparam, lparam);

/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- { ---------- */
	this->CtaReset(2);	/* CTA リセット	*/
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- } ---------- */

/* modified 2009.06.02 hmenjo ダウン時アラームリセット化 ---------- { ---------- */
////	if (PROCESS_DOWN == ProcStatusGet()) {
////		// Down 状態の場合のみ実行します
//	if ((0 != ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) && (PROCESS_DOWN == ProcStatusGet())) {
/* modified 2009.06.02 hmenjo ダウン時アラームリセット化 ---------- 			 */
	if (PROCESS_DOWN == ProcStatusGet()) {
/* modified 2009.06.02 hmenjo ダウン時アラームリセット化 ---------- } ---------- */
		// ハードシミュレートモード時で，Down 状態の場合のみ実行します
		// アラーム記憶フラグをリセット
		if (0 != m_AlarmFlags.bAirPressureLowOn)	{m_AlarmFlags.bAirPressureLowOn		= FALSE;	AlarmIf_Reset(ALID_AirPressureDown);}
		if (0 != m_AlarmFlags.bMaintenanceSWOn)		{m_AlarmFlags.bMaintenanceSWOn		= FALSE;	AlarmIf_Reset(ALID_MaintenanceSwitchOn);}
		if (0 != m_AlarmFlags.bPinDownOff)			{m_AlarmFlags.bPinDownOff			= FALSE;	AlarmIf_Reset(ALID_LifterLowestPositionSensorOff);}
		if (0 != m_AlarmFlags.bRobotArmOn)			{m_AlarmFlags.bRobotArmOn			= FALSE;	AlarmIf_Reset(ALID_RobotArmDetected);}
		if (0 != m_AlarmFlags.bVacuumPressure1Off)	{m_AlarmFlags.bVacuumPressure1Off	= FALSE;	AlarmIf_Reset(ALID_VacuumSensorOff);}
		if (0 != m_AlarmFlags.bWorkGuideOpenOff)	{m_AlarmFlags.bWorkGuideOpenOff		= FALSE;	AlarmIf_Reset(ALID_WorkGuideOpenSensorOff);}
		if (0 != m_AlarmFlags.bShutterCloseOff)		{m_AlarmFlags.bShutterCloseOff		= FALSE;	AlarmIf_Reset(ALID_ShutterCloseError);}
/* 2009.08.20 K.Matsuo ALID 定義を変更 ---------- { ---------- */
///* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- { ---------- */
//		if (0 != m_AlarmFlags.bRetractZPosOff)		{m_AlarmFlags.bRetractZPosOff		= FALSE;	AlarmIf_Reset(ZPOSOFF_ALID);}
///* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- } ---------- */
/* 2009.08.20 K.Matsuo ALID 定義を変更 ----------			   */
		if (0 != m_AlarmFlags.bRetractZPosOff)		{m_AlarmFlags.bRetractZPosOff		= FALSE;	AlarmIf_Reset(ALID_Z_PositionRetractFailed);}
/* 2009.08.20 K.Matsuo ALID 定義を変更 ---------- } ---------- */
// 2013.01.10 bagus stage driver alarm io -->
		if (0 != m_AlarmFlags.bStageAlarmOn)		{m_AlarmFlags.bStageAlarmOn			= FALSE;	AlarmIf_Reset(ALID_StageError);}
		if (0 != m_AlarmFlags.bStageOverTravelOn)	{m_AlarmFlags.bStageOverTravelOn	= FALSE;	AlarmIf_Reset(ALID_StageOverTravelError);}
		if (0 != m_AlarmFlags.bStageUnknownOn)		{m_AlarmFlags.bStageUnknownOn		= FALSE;	AlarmIf_Reset(ALID_StageUnknownError);}
// 2013.01.10 bagus stage driver alarm io -->

		DWORD l_dwEMO = CheckDIO_IsEMO();
		if (0 != l_dwEMO) {
			// Down 状態を続行
			ProcStatusSet(PROCESS_DOWN);	// 念のため Down をセット
			switch (l_dwEMO) {
			case ALID_EMOSwitchOn:		// EMO
				if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
				if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
				if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
				// アラーム通知
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EMOSwitchOn));
				break;
			case ALID_DoorOpen:		// ドアインタロック
				if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
				if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
				if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
				// アラーム通知
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_DoorOpen));
				break;
			case ALID_EquipmentPowerOff:		// 装置パワーオフ
				if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
				if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
				if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
				// アラーム通知
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EquipmentPowerOff));
				break;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
			case ALID_SafetyPlugOpen:		/* セーフティプラグ	*/
				if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
				if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
				if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
				/* アラーム通知	*/
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_SafetyPlugOpen));
				break;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
			default:
				// 何もしません．
				break;
			}
		} else {
			if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
			if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
			if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
			if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
			// Wait にします．
			((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetProcessStatus(PROCESS_WAIT);
			TCHAR l_tszLogText[256];
			//Saiki 20090527 Change ----->
			//_stprintf(l_tszLogText, _T("Changed Process Status to %s."), PROCESS_STATUS_DISP[PROCESS_WAIT]);
			SYSTEM_CONFIG l_SystemConfig;
			ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			if(l_SystemConfig.nLanguage == 0){
				_stprintf(l_tszLogText, _T("Changed Process Status to %s."), PROCESS_STATUS_DISP_ENU[PROCESS_WAIT]);
			}
			else{
				_stprintf(l_tszLogText, _T("Changed Process Status to %s."), PROCESS_STATUS_DISP_JPN[PROCESS_WAIT]);
			}
			//Saiki 20090527 Change <-----
			LogChief(l_tszLogText);
		}
	}
	// 2009.07.29 K.Matsuo -->
	else {
		if (0 != m_AlarmFlags.bAirPressureLowOn)	{m_AlarmFlags.bAirPressureLowOn		= FALSE;}
		if (0 != m_AlarmFlags.bMaintenanceSWOn)		{m_AlarmFlags.bMaintenanceSWOn		= FALSE;}
		if (0 != m_AlarmFlags.bPinDownOff)			{m_AlarmFlags.bPinDownOff			= FALSE;}
		if (0 != m_AlarmFlags.bRobotArmOn)			{m_AlarmFlags.bRobotArmOn			= FALSE;}
		if (0 != m_AlarmFlags.bVacuumPressure1Off)	{m_AlarmFlags.bVacuumPressure1Off	= FALSE;}
		if (0 != m_AlarmFlags.bWorkGuideOpenOff)	{m_AlarmFlags.bWorkGuideOpenOff		= FALSE;}
		if (0 != m_AlarmFlags.bShutterCloseOff)		{m_AlarmFlags.bShutterCloseOff		= FALSE;}
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- { ---------- */
		if (0 != m_AlarmFlags.bRetractZPosOff)		{m_AlarmFlags.bRetractZPosOff		= FALSE;}
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- } ---------- */
// 2013.01.10 bagus stage driver alarm io -->
		if (0 != m_AlarmFlags.bStageAlarmOn)		{m_AlarmFlags.bStageAlarmOn			= FALSE;}
		if (0 != m_AlarmFlags.bStageOverTravelOn)	{m_AlarmFlags.bStageOverTravelOn	= FALSE;}
		if (0 != m_AlarmFlags.bStageUnknownOn)		{m_AlarmFlags.bStageUnknownOn		= FALSE;}
// 2013.01.10 bagus stage driver alarm io <--
	}
	// 2009.07.29 K.Matsuo <--

	return 0L;
}

/*
 *	DIO 入力をチェック － 動作中の異常入力をチェック
 *		異常入力であれば停止動作を行います．
 */
void CChiefView::CheckDIO_Running()
{
	// エッジ検出 -------------------------------------------------------------
	// ロボットアーム検出 H->L
	BOOL l_bRobotArmOnDetected;
	if (0 != m_DioIgnoreSW.bRobotArm) {
		l_bRobotArmOnDetected = FALSE;
	} else {
		l_bRobotArmOnDetected = ((0 != m_DiInfo.bRobotArm) && (0 == nexioIsRobotArmDetect()))? TRUE : FALSE;
	}
	// メンテナンス SW オン H->L
	BOOL l_bMaintenanceSWOnDetected = ((0 != m_DiInfo.bMaintenanceSW) && (0 == nexioIsMaintenanceSwitch()))? TRUE : FALSE;
	// メンテナンス SW オフ L->H
	BOOL l_bMaintenanceSWOffDetected = ((0 == m_DiInfo.bMaintenanceSW) && (0 != nexioIsMaintenanceSwitch()))? TRUE : FALSE;
	// エア圧力低下 L->H
	BOOL l_bAirPressureLowDetected;
	if (0 != m_DioIgnoreSW.bAirPressureLow) {
		l_bAirPressureLowDetected = FALSE;
	} else {
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
//		l_bAirPressureLowDetected = ((0 == m_DiInfo.bAirPressureLow) && (0 != nexioIsAirPressureLevelLow()))? TRUE : FALSE;
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- 		   */
		l_bAirPressureLowDetected = ((0 == m_DiInfo.bAirPressureLow) && (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->Rap_IsAirPressureLowON()))? TRUE : FALSE;
/* modified hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	}
	// バキュームセンサ１オフ H->L
	BOOL l_bVacuumPressure1OffDetected = ((0 != m_DiInfo.bVacuumPressure1) && (0 == nexioIsVacuumOn()))? TRUE : FALSE;
	// ピンダウン H->L
	BOOL l_bPinDownOffDetected;
	static BOOL ls_bPinDownOffPrev = FALSE;
	if (0 != m_DioIgnoreSW.bPinDown) {
		l_bPinDownOffDetected = FALSE;
	} else {
		if ((0 == ls_bPinDownOffPrev) && (0 != CheckDIO_IsPinDownOFF())) {
			l_bPinDownOffDetected = TRUE;
		} else {
			l_bPinDownOffDetected = FALSE;
		}
	}
	ls_bPinDownOffPrev = CheckDIO_IsPinDownOFF();
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- { ---------- */
	if ((true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle()) && (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle())) {
		/* ストレス測定シーケンス中のみは，ピンが，下降端，あるいは，アライメント位置以外は NG です．	*/
		if (
			(0 != m_DiInfo.bPinUp)											/* 上昇端がオン	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//		 || ((0 != m_DiInfo.bPinDown) && (0 != nexioIsPinAlignmentPos()))	/* 下降端とアライメント位置の両方がオン	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
		 || ((0 != m_DiInfo.bPinDown) && (0 != m_DiInfo.bPinAlign))	/* 下降端とアライメント位置の両方がオン	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
			) {
			l_bPinDownOffDetected = TRUE;
		} else {
			if (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsMeasuring()) {
				/* ストレス測定中	*/
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
//				if ((0 == m_DiInfo.bPinUp) && (0 != nexioIsPinAlignmentPos()) && (0 == m_DiInfo.bPinDown)) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ----------			   */
				if ((0 == m_DiInfo.bPinUp) && (0 != m_DiInfo.bPinAlign) && (0 == m_DiInfo.bPinDown)) {
/* modified 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
					/* 上昇端オフ，かつ，アライメント位置オン，下降端オフ，なら異常でない	*/
					l_bPinDownOffDetected = FALSE;
				} else {
					l_bPinDownOffDetected = TRUE;
				}
			} else {
				l_bPinDownOffDetected = FALSE;
			}
		}
	}
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- } ---------- */
	// ワークガイド開オフ H->L
//	BOOL l_bWorkGuideOpenOffDetected = ((0 != m_DiInfo.bWorkGuideOpen) && (0 == nexioIsWorkGuideOpen()))? TRUE : FALSE;
	BOOL l_bWorkGuideOpenOffDetected;
	static BOOL ls_bWorkGuideOpenOffPrev = FALSE;
	BOOL l_bResult = ((0 != m_DiInfo.bWorkGuideOpen) && (0 == m_DiInfo.bWorkGuideClose))? FALSE : TRUE;
	if ((0 == ls_bWorkGuideOpenOffPrev) && (0 != l_bResult)) {
		l_bWorkGuideOpenOffDetected = TRUE;
	} else {
		l_bWorkGuideOpenOffDetected = FALSE;
	}
	ls_bWorkGuideOpenOffPrev = l_bResult;
	// シャッタ閉オフ H->L
//	BOOL l_bShutterCloseOffDetected = ((0 != m_DiInfo.bShutterClose) && (0 == nexioIsShutterClose()))? TRUE : FALSE;
	BOOL l_bShutterCloseOffDetected;
	static BOOL ls_bShutterCloseOffPrev = FALSE;
	l_bResult = ((0 != m_DiInfo.bShutterClose) && (0 == m_DiInfo.bShutterOpen))? FALSE : TRUE;
	if ((0 == ls_bShutterCloseOffPrev) && (0 != l_bResult)) {
		l_bShutterCloseOffDetected = TRUE;
	} else {
		l_bShutterCloseOffDetected = FALSE;
	}
	ls_bShutterCloseOffPrev = l_bResult;

	// ステージ速度制限 -------------------------------------------------------
	if (0 != l_bMaintenanceSWOnDetected) {
		// メンテナンスモードになった
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- { ----------
//		// 速度制限する
//		StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ----------
		if (TRUE == g_bIL_SpeedDown) {
			// 速度制限する
			StageEnableSpeedLimit();
		}
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- } ----------
//		if (0 == m_bSpeedLimiterOFF) {
//			// 速度制限モードをセット
//			((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(TRUE);
//		}
	}
	if (0 != l_bMaintenanceSWOffDetected) {
		// メンテナンスモードでなくなった
		// 速度制限しない
		StageDisableSpeedLimit();
//		// 速度制限モードをリセット
//		((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(FALSE);
	}

/* added 2009.08.04 hmenjo 測定中以外ドア開ステージ停止 ---------- { ---------- */
	/* 測定中以外でドア開なら，ステージを停止させます．	*/
	if ((true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())
	 && (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle())) {
		/* シーケンス測定がアイドルです．	 */
		if ((0 == m_DiInfo.bDoorInterlock) && (0 == m_DiInfo.bTHMaintenanceSW)) {
			/* ドア開 検出した．	*/
			if ((0 == m_DiInfo.bMaintenanceSW) && (FALSE == g_bIL_DoorOpen)) {
				;	/* メンテで，ドア開検出禁止のときは開と認識しません．	*/
			} else {
				/* 本当に検出した．	*/
				if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
					/* XY ステージ動作中です．	*/
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 動作中フラグ(XY ステージ)をオフ	*/
					AlarmIf_Set(ALID_DoorOpen);		/* ドア開	*/
				}
			}
		}
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- { ---------- */
		if (0 == this->IsCtaILPI()) {
			/* CTAILPI オフ検出	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_CTA_IL_PI_On);		/* CTA IL PI オフ検出	*/
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- { ---------- */
				this->CtaReset(1);
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- } ---------- */
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 動作中フラグ(XY ステージ)をオフ	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* 戻り値は無視しています	*/
					StageElevatorStop();	/* 戻り値は無視しています	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 動作中フラグ(Z 軸)をオフ	*/
				}
			}
		}
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position 常時チェック ---------- { ---------- */
		int iResistStatus = this->CheckResistIL();
		if (0 != iResistStatus) {
			/* ResistIL 異常検出	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				if (1 == iResistStatus) {
					AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL オフ検出	*/
				} else {
					AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL 異常検出	*/
				}
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 動作中フラグ(XY ステージ)をオフ	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* 戻り値は無視しています	*/
					StageElevatorStop();	/* 戻り値は無視しています	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 動作中フラグ(Z 軸)をオフ	*/
				}
			}
		}
/* added 2009.11.06 K.Matsuo RS Seq Head Position 常時チェック ---------- } ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
		if( 0 == this->IsMSILPI()){
			//下端位置２のセンサーONを検出
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_MS_IL_POS_ERROR);		/* CTA IL PI オフ検出	*/
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 動作中フラグ(XY ステージ)をオフ	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* 戻り値は無視しています	*/
					StageElevatorStop();	/* 戻り値は無視しています	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 動作中フラグ(Z 軸)をオフ	*/
				}
			}
		}
// 2009.11.12 bagus MS 下端位置２チェック --}--
	} else {
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- { ---------- */
		/* シーケンス測定でアイドル以外があります．	*/
		if (0 == this->IsCtaILPI()) {
			/* CTAILPI オフ検出	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_CTA_IL_PI_On);	/* CTA IL PI オフ検出	*/
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- { ---------- */
				this->CtaReset(1);
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- } ---------- */
				EqPowerOffPulse();				/* 装置電源オフ－出力オン	*/
				StopAxisAllEMO();				/* 軸系の即停止指令を発行	*/
				CancelSeqForAlarm();			/* キャンセル発行	*/
				ProcStatusSet(PROCESS_DOWN);	/* 即 DOWN 状態にする	*/
			}
		}
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position 常時チェック ---------- { ---------- */
		/* シーケンス測定でアイドル以外があります．	*/
		int iResistStatus = this->CheckResistIL();
		if (0 != iResistStatus) {
			/* ResistIL 異常検出	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				if (1 == iResistStatus) {
					AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL オフ検出	*/
				} else {
					AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL 異常検出	*/
				}
				EqPowerOffPulse();				/* 装置電源オフ－出力オン	*/
				StopAxisAllEMO();				/* 軸系の即停止指令を発行	*/
				CancelSeqForAlarm();			/* キャンセル発行	*/
				ProcStatusSet(PROCESS_DOWN);	/* 即 DOWN 状態にする	*/
			}
		}
/* added 2009.11.06 K.Matsuo RS Seq Head Position 常時チェック ---------- } ---------- */
// 2009.11.12 bagus MS 下端位置２チェック --{--
		if( 0 == this->IsMSILPI()){
			//下端位置２のセンサーONを検出
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_MS_IL_POS_ERROR);		/* CTA IL PI オフ検出	*/
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 動作中フラグ(XY ステージ)をオフ	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* 戻り値は無視しています	*/
					StageElevatorStop();	/* 戻り値は無視しています	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 動作中フラグ(Z 軸)をオフ	*/
				}
			}
		}
// 2009.11.12 bagus MS 下端位置２チェック --}--
	}
/* added 2009.08.04 hmenjo 測定中以外ドア開ステージ停止 ---------- } ---------- */

	// 動作中の DI チェック -------------------------------------------------------------------------------------------
	// 緊急停止扱い -------------------------------------------------------------------------------
	// EMO，ドアインタロック，装置パワーオフ 検出 -----------------------------
/* modified 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
//	if ((0 == m_AlarmFlags.bEMOStopOn) || (0 == m_AlarmFlags.bDoorInterlockOn) || (0 == m_AlarmFlags.bEQPowerOn)) {
/* modified 2009.08.17 hmenjo セーフティプラグ処理追加 ----------			   */
	if ((0 == m_AlarmFlags.bEMOStopOn) || (0 == m_AlarmFlags.bDoorInterlockOn) || (0 == m_AlarmFlags.bEQPowerOn) || (0 == m_AlarmFlags.bSafetyPlugOn)) {
/* modified 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
		DWORD l_dwResult;
		if (0 != (l_dwResult = CheckDIO_IsEMO())) {
//			// 軸系の即停止指令を発行
//			StopAxisAllEMO();
//			// キャンセル発行
//			CancelSeqForAlarm();
//			// 即 DOWN 状態にする
//			ProcStatusSet(PROCESS_DOWN);
			// アラーム通知
			switch (l_dwResult) {
			case ALID_EMOSwitchOn:		// EMO
				if (0 == m_AlarmFlags.bEMOStopOn) {
					m_AlarmFlags.bEMOStopOn = TRUE;
					// 装置電源オフ－出力オン
					EqPowerOffPulse();
					// 軸系の即停止指令を発行
					StopAxisAllEMO();
					// キャンセル発行
					CancelSeqForAlarm();
					// 即 DOWN 状態にする
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EMOSwitchOn));
				}
				break;
			case ALID_DoorOpen:		// ドアインタロック
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
				if (TRUE == g_bIL_DoorOpenPause) {
					SentMeasPauseCheck(1);
					break;
				}
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
				if (0 == m_AlarmFlags.bDoorInterlockOn) {
					m_AlarmFlags.bDoorInterlockOn = TRUE;
					// 装置電源オフ－出力オン
					EqPowerOffPulse();
					// 軸系の即停止指令を発行
					StopAxisAllEMO();
					// キャンセル発行
					CancelSeqForAlarm();
					// 即 DOWN 状態にする
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_DoorOpen));
				}
				break;
			case ALID_EquipmentPowerOff:	// 装置パワーオフ
				if (0 == m_AlarmFlags.bEQPowerOn) {
					m_AlarmFlags.bEQPowerOn = TRUE;
//					// 装置電源オフ－出力オン		装置パワーオフ検出時は装置電源オフ出力は不要です．
//					EqPowerOffPulse();
					// 軸系の即停止指令を発行
					StopAxisAllEMO();
					// キャンセル発行
					CancelSeqForAlarm();
					// 即 DOWN 状態にする
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EquipmentPowerOff));
				}
				break;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
			case ALID_SafetyPlugOpen:		/* セーフティプラグ	*/
				if (0 == m_AlarmFlags.bSafetyPlugOn) {
					m_AlarmFlags.bSafetyPlugOn = TRUE;
					// 装置電源オフ－出力オン
					EqPowerOffPulse();
					// 軸系の即停止指令を発行
					StopAxisAllEMO();
					// キャンセル発行
					CancelSeqForAlarm();
					// 即 DOWN 状態にする
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_SafetyPlugOpen));
				}
				break;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
			default:	// 何もしません
				break;
			}
		}
	}
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- { ---------- */
	/* ストレス測定中なら Z 軸退避位置チェック	*/
	if (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsMeasuring()) {
		if (0 == nexioIsRetractPosZ()) {
			// 装置電源オフ－出力オン
			EqPowerOffPulse();
			// 軸系の即停止指令を発行
			StopAxisAllEMO();
			// キャンセル発行
			CancelSeqForAlarm();
			// 即 DOWN 状態にする
			ProcStatusSet(PROCESS_DOWN);
/* 2009.08.20 K.Matsuo ALID 定義を変更 ---------- { ---------- */
//				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ZPOSOFF_ALID));
/* 2009.08.20 K.Matsuo ALID 定義を変更 ----------			   */
			if (0 == m_AlarmFlags.bRetractZPosOff) {
				// アラーム通知
				m_AlarmFlags.bRetractZPosOff = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_PositionRetractFailed));
/* 2009.08.20 K.Matsuo ALID 定義を変更 ---------- } ---------- */
			}
		}
	}
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- } ---------- */
	// ロボットアームオン，ピンダウンオフ，ワークガイド開オフ 検出 ------------
#if 0		// シャープ仕様でシャッタ開の検出を追加
	if ((0 == m_AlarmFlags.bRobotArmOn) || (0 == m_AlarmFlags.bPinDownOff) || (0 == m_AlarmFlags.bWorkGuideOpenOff)) {
		if ((0 != l_bRobotArmOnDetected) || (0 != l_bPinDownOffDetected) || (0 != l_bWorkGuideOpenOffDetected)) {
#else
	if ((0 == m_AlarmFlags.bRobotArmOn) || (0 == m_AlarmFlags.bPinDownOff) || (0 == m_AlarmFlags.bWorkGuideOpenOff) || (0 == m_AlarmFlags.bShutterCloseOff)) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
//		if ((0 != l_bRobotArmOnDetected) || (0 != l_bPinDownOffDetected) || (0 != l_bWorkGuideOpenOffDetected) || (0 != l_bShutterCloseOffDetected)) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
		if (
			(0 != l_bRobotArmOnDetected)
		 || (0 != l_bPinDownOffDetected)
		 || ((0 != l_bWorkGuideOpenOffDetected)	&& (ST_STRS_PINDOWNCOMP != ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState())
		 										&& (ST_STRS_ABORTPINDOWN != ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState()))
		 || (0 != l_bShutterCloseOffDetected)
			) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
#endif
			if (
				(PROCESS_PROC == ProcStatusGet())											// Processing
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY ステージ動作中
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 軸動作中
				) {
				// 装置電源オフ－出力オン
				EqPowerOffPulse();
				// 軸系の即停止指令を発行
				StopAxisAllEMO();
				// キャンセル発行
				CancelSeqForAlarm();
				// 即 DOWN 状態にする
				ProcStatusSet(PROCESS_DOWN);
				// アラーム通知
				if ((0 == m_AlarmFlags.bRobotArmOn) && (0 != l_bRobotArmOnDetected)) {
					m_AlarmFlags.bRobotArmOn = TRUE;
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_RobotArmDetected));
				}
				if ((0 == m_AlarmFlags.bPinDownOff) && (0 != l_bPinDownOffDetected)) {
					m_AlarmFlags.bPinDownOff = TRUE;
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterLowestPositionSensorOff));
				}
				if ((0 == m_AlarmFlags.bWorkGuideOpenOff) && (0 != l_bWorkGuideOpenOffDetected)) {
					m_AlarmFlags.bWorkGuideOpenOff = TRUE;
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_WorkGuideOpenSensorOff));
				}
				if ((0 == m_AlarmFlags.bShutterCloseOff) && (0 != l_bShutterCloseOffDetected)) {
					m_AlarmFlags.bShutterCloseOff = TRUE;
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_ShutterCloseError));
				}
			}
		}
	}
	// サイクル停止扱い ---------------------------------------------------------------------------
	// メンテナンス SW オン 検出 ----------------------------------------------
	if (0 == m_AlarmFlags.bMaintenanceSWOn) {
// modified hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- { ----------
//		if (0 != l_bMaintenanceSWOnDetected) {
// modified hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ----------
		if ((0 != l_bMaintenanceSWOnDetected) && (TRUE == g_bIL_CycleStop)) {
// modified hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- } ----------
			if (
				(0 == m_DiInfo.bTHMaintenanceSW)											// 東朋メンテ SW オフ
			 && (
					(PROCESS_PROC == ProcStatusGet())											// Processing
				 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY ステージ動作中
				 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 軸動作中
				)
				) {
				// 軸系の即停止指令を発行
				StopAxisAllEMO();
				// キャンセル発行
				CancelSeqForAlarm();
				// アラーム通知
				m_AlarmFlags.bMaintenanceSWOn = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_MaintenanceSwitchOn));
			}
		}
	}
	// エア圧力低下 検出 ----------------------------------------------
	if (0 == m_AlarmFlags.bAirPressureLowOn) {
		if (0 != l_bAirPressureLowDetected) {
			if (
				(PROCESS_PROC == ProcStatusGet())											// Processing
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY ステージ動作中
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 軸動作中
				) {
				// 軸系の即停止指令を発行
				StopAxisAllEMO();
				// キャンセル発行
				CancelSeqForAlarm();
				// アラーム通知
				m_AlarmFlags.bAirPressureLowOn = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_AirPressureDown));
			}
		}
	}
	// バキュームセンサ１ オフ 検出 ----------------------------------------------
	if (0 == m_AlarmFlags.bVacuumPressure1Off) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- { ---------- */
//		if (0 != l_bVacuumPressure1OffDetected) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ----------			  */
		if ((0 != l_bVacuumPressure1OffDetected) && (ST_STRS_PINALIGN != ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState())) {
/* modified 2009.10.06 hmenjo Stress nexifLoad に変更 ---------- } ---------- */
			if (
				(PROCESS_PROC == ProcStatusGet())											// Processing
				) {
				// 軸系の即停止指令を発行
				StopAxisAllEMO();
				// キャンセル発行
				CancelSeqForAlarm();
				// アラーム通知
				m_AlarmFlags.bVacuumPressure1Off = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_VacuumSensorOff));
			}
		}
	}

// 2013.01.10 bagus stage driver alarm io -->
	BOOL bError = FALSE;
	int iDioX1AxisAlarm = CheckDIO_IsX1AxisAlarm();
	int iDioX2AxisAlarm = CheckDIO_IsX2AxisAlarm();
	int iDioYAxisAlarm = CheckDIO_IsYAxisAlarm();

	if (0 == m_AlarmFlags.bStageAlarmOn) {
		bError = FALSE;
		if( iDioX1AxisAlarm != 0x0000 && // エラーなし
			iDioX1AxisAlarm != 0x0100 && // オーバートラベル
			iDioX1AxisAlarm != 0x0010 && // ステージ未定義
			iDioX1AxisAlarm != 0x0001 )  // ステージ未定義
		{
			bError = TRUE;
			OutputStageError(iDioX1AxisAlarm, 0);
		}

		if( iDioX2AxisAlarm != 0x0000 && // エラーなし
			iDioX2AxisAlarm != 0x0100 && // オーバートラベル
			iDioX2AxisAlarm != 0x0010 && // ステージ未定義
			iDioX2AxisAlarm != 0x0001 )  // ステージ未定義
		{
			bError = TRUE;
			OutputStageError(iDioX2AxisAlarm, 1);
		}

		if( iDioYAxisAlarm != 0x0000 && // エラーなし
			iDioYAxisAlarm != 0x0100 && // オーバートラベル
			iDioYAxisAlarm != 0x0010 && // ステージ未定義
			iDioYAxisAlarm != 0x0001 )  // ステージ未定義
		{
			bError = TRUE;
			OutputStageError(iDioYAxisAlarm, 2);
		}

		if (bError == TRUE) {
// 			if (
// 				(PROCESS_PROC == ProcStatusGet())											// Processing
// 				|| (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY ステージ動作中
// 				|| (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 軸動作中
// 				) {
				// 装置電源オフ－出力オン
				EqPowerOffPulse();
				// 軸系の即停止指令を発行
				StopAxisAllEMO();
				// キャンセル発行
				CancelSeqForAlarm();
				// 即 DOWN 状態にする
				ProcStatusSet(PROCESS_DOWN);
				// アラーム通知
				m_AlarmFlags.bStageAlarmOn = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_StageError));
//			}
		}
	}

	if (0 == m_AlarmFlags.bStageOverTravelOn) {
		bError = FALSE;
		if( iDioX1AxisAlarm == 0x0100 ) // オーバートラベル
		{
			bError = TRUE;
			OutputStageError(iDioX1AxisAlarm, 0);
		}

		if( iDioX2AxisAlarm == 0x0100 ) // オーバートラベル
		{
			bError = TRUE;
			OutputStageError(iDioX2AxisAlarm, 1);
		}

		if( iDioYAxisAlarm == 0x0100 ) // オーバートラベル
		{
			bError = TRUE;
			OutputStageError(iDioYAxisAlarm, 2);
		}

		if (bError == TRUE) {
			// キャンセル発行
			CancelSeqForAlarm();
			// アラーム通知
			m_AlarmFlags.bStageOverTravelOn = TRUE;
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_StageOverTravelError));
		}
	}

	if (0 == m_AlarmFlags.bStageUnknownOn) {
		bError = FALSE;
		if( iDioX1AxisAlarm == 0x0010 ||
			iDioX1AxisAlarm == 0x0001) // ステージ未定義
		{
			bError = TRUE;
			OutputStageError(iDioX1AxisAlarm, 0);
		}

		if( iDioX2AxisAlarm == 0x0010 ||
			iDioX2AxisAlarm == 0x0001) // ステージ未定義
		{
			bError = TRUE;
			OutputStageError(iDioX2AxisAlarm, 1);
		}

		if( iDioYAxisAlarm == 0x0010 ||
			iDioYAxisAlarm == 0x0001) // ステージ未定義
		{
			bError = TRUE;
			OutputStageError(iDioYAxisAlarm, 2);
		}

		if (bError == TRUE) {
			// キャンセル発行
			CancelSeqForAlarm();
			// アラーム通知
			m_AlarmFlags.bStageUnknownOn = TRUE;
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_StageUnknownError));
		}
	}
// 2013.01.10 bagus stage driver alarm io <--
}

void CChiefView::OutputStageError(int iDioAxisAlarm, int iAxis)
{
	TCHAR szAxisName[MAX_PATH];
	TCHAR szMessage[MAX_PATH];

	switch ( iAxis ) {
	case 0:
		strcpy(szAxisName, "X1");
		break;
	case 1:
		strcpy(szAxisName, "X2");
		break;
	case 2:
		strcpy(szAxisName, "Y");
		break;
	default:
		return;
		break;
	}

	switch ( iDioAxisAlarm ) {
	case 0x0000:
		return;
		break;
	case 0x1111:
		sprintf_s(szMessage, _countof(szMessage), "A.0xx / A.8xx / A.bxx Error (Amp = %s, ErrorCode = 1-111)", szAxisName);
		break;
	case 0x1011:
		sprintf_s(szMessage, _countof(szMessage), "A.100 Error (Amp = %s, ErrorCode = 1-011)", szAxisName);
		break;
	case 0x1001:
		sprintf_s(szMessage, _countof(szMessage), "A.3xx / A.dxx Error (Amp = %s, ErrorCode = 1-001)", szAxisName);
		break;
	case 0x1110:
		sprintf_s(szMessage, _countof(szMessage), "A.4xx Error (Amp = %s, ErrorCode = 1-110)", szAxisName);
		break;
	case 0x1010:
		sprintf_s(szMessage, _countof(szMessage), "A.5xx / A.Cxx Error (Amp = %s, ErrorCode = 1-010)", szAxisName);
		break;
	case 0x1000:
		sprintf_s(szMessage, _countof(szMessage), "A.7xx Error (Amp = %s, ErrorCode = 1-000)", szAxisName);
		break;
	case 0x1100:
		sprintf_s(szMessage, _countof(szMessage), "A.Exx Error (Amp = %s, ErrorCode = 1-100)", szAxisName);
		break;
	case 0x1101:
		sprintf_s(szMessage, _countof(szMessage), "A.F10 Error (Amp = %s, ErrorCode = 1-101)", szAxisName);
		break;
	case 0x0111:
		sprintf_s(szMessage, _countof(szMessage), "A.900 / A.901 Error (Amp = %s, ErrorCode = 0-111)", szAxisName);
		break;
	case 0x0011:
		sprintf_s(szMessage, _countof(szMessage), "A.910 / A.911 Error (Amp = %s, ErrorCode = 0-011)", szAxisName);
		break;
	case 0x0101:
		sprintf_s(szMessage, _countof(szMessage), "A.920 / A.921 Error (Amp = %s, ErrorCode = 0-101)", szAxisName);
		break;
	case 0x0110:
		sprintf_s(szMessage, _countof(szMessage), "A.941Error (Amp = %s, ErrorCode = 0-110)", szAxisName);
		break;
	case 0x0100:
		sprintf_s(szMessage, _countof(szMessage), "A.9A0 OverTravel (Amp = %s, ErrorCode = 0-100)", szAxisName);
		break;
	default:
		sprintf_s(szMessage, _countof(szMessage), "Unknown Error (Amp = %s, ErrorCode = 0-001)", szAxisName);
		break;
	}

	LogStageError(szMessage);
}

/*
 *	軸系の即停止指令
 */
void CChiefView::StopAxisAllEMO()
{
	// XY 軸を停止
	StageStop();			// 戻り値は無視しています
	ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 動作中フラグ(XY ステージ)をオフ
	// AF を停止
	StageAbortAutoFocus();	// 戻り値は無視しています
	// Z 軸を停止
	StageElevatorStop();	// 戻り値は無視しています
	ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			// 動作中フラグ(Z 軸)をオフ
}

/*
 *	アラーム時のシーケンス測定 キャンセル
 */
void CChiefView::CancelSeqForAlarm()
{
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- { ---------- */
//	if ((PROCESS_PROC == ProcStatusGet()) && (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		// Processing でシーケンス測定が IDLE 以外の場合
//		((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_CANCEL);
//	}
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ----------			   */
	if (PROCESS_PROC == ProcStatusGet()) {
		/* Processing で	*/
		CHIEF_PFUNCS l_ChiefPFuncs;
		if (0 != PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
			if (false == (*l_ChiefPFuncs.IsIdle)(this)) {
				/* シーケンス測定が IDLE 以外の場合	*/
				(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iCANCEL, 0);
			}
		}
	}
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- } ---------- */
}

/*
 *	測定(AF)開始前のヘッド揺れ収束待ち
 */
void CChiefView::WaitTimeBeforeMeas(
		DWORD dwMode	// 0：開始時刻取得，1：時間待ち処理
	)
{
	if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
		// 実機の場合のみ実施
		switch (dwMode) {
		case 0:
			m_WaitMeasTime.dwStartTime = ::GetTickCount();
			break;
		default:
			if (0 != m_WaitMeasTime.dwWaitTimeSetting) {
				// 設定値が≠０の場合のみ処理します
				DWORD l_dwDeltaTime;
				m_WaitMeasTime.dwEndTime = ::GetTickCount();
				if (m_WaitMeasTime.dwStartTime <= m_WaitMeasTime.dwEndTime) {
					l_dwDeltaTime = m_WaitMeasTime.dwEndTime - m_WaitMeasTime.dwStartTime;
				} else {
					// GetTickCount() のカウンタがオーバフローしていた場合
					l_dwDeltaTime = (0xffffffff - m_WaitMeasTime.dwStartTime) + m_WaitMeasTime.dwEndTime;
				}
				if (l_dwDeltaTime < m_WaitMeasTime.dwWaitTimeSetting) {
					// 設定値の残り時間だけスリープします
#if 0
					::Sleep(m_WaitMeasTime.dwWaitTimeSetting - l_dwDeltaTime);
#else
					DWORD l_dwSleepTime = m_WaitMeasTime.dwWaitTimeSetting - l_dwDeltaTime;
					TCHAR l_tszLogText[256];
					_stprintf(l_tszLogText, _T("Wait until sway is settled. (=%d[ms])"), l_dwSleepTime);
					::Sleep(l_dwSleepTime);
#endif
				}
			}
			break;
		}
	}
}

/*
 *	次の「測定する」になっている測定ポイント番号を取得します
 */
DWORD CChiefView::GetNextPointNo(
		DWORD *pdwPointNo	// 検索開始番号
	)
{
	if (0 == *pdwPointNo) {
		*pdwPointNo = SCAN_POINT_MAX + 1;
		return *pdwPointNo;
	}

	for (int i = *pdwPointNo; i <= SCAN_POINT_MAX; i++) {
		if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->m_bDoPointMeasFlag[i - 1]) {
			break;
		}
	}

	*pdwPointNo = i;

	return i;
}
/* added 2009.08.03 hmenjo ストレス機能追加(18) ---------- { ---------- */
/*
 *	指定された検索開始番号から「有効」になっている測定ライン番号を取得します
 */
DWORD CChiefView::GetNextLineNo(
		DWORD *pdwLineNo	// 検索開始番号
	)
{
	if (0 == *pdwLineNo) {
		*pdwLineNo = STRESS_LINES_MAX + 1;
		return *pdwLineNo;
	}

	STRESS_CONFIG l_StressConfig;
	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	int l_iMaxLine = l_StressConfig.dwLiftPinNumberOfLine;
	if ((l_iMaxLine <= 0) || (STRESS_LINES_MAX < l_iMaxLine)) {
		l_iMaxLine = STRESS_LINES_MAX;
	}
	LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) m_ChiefRecipes.pStageProgStress;
	for (int i = *pdwLineNo; i <= l_iMaxLine; i++) {
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//		if (0 != l_pStageProgStress->Line[i - 1].LineSec.bValidLine) {
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
		if (0 != l_pStageProgStress->Line[i - 1].bValidLine) {
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
			break;
		}
	}

	*pdwLineNo = i;

	return i;
}
/* added 2009.08.03 hmenjo ストレス機能追加(18) ---------- } ---------- */

/*
 *	ログファイル関数 - CNanoSpecDoc::ChiefLogging() のラッパ関数
 */
void CChiefView::LogChief(TCHAR *pszLogText)
{
	if (0 != m_pcNanoSpecDoc) {
		((CNanoSpecDoc*) m_pcNanoSpecDoc)->ChiefLogging(pszLogText);
	}
}

/*
 *	ログファイル関数 - メッセージ受信用
 */
void CChiefView::LogChief_WinMsg(TCHAR* ptszMsgName, WPARAM wparam, LPARAM lparam)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, "Received %s (WPARAM = 0x%08x, LPARAM = 0x%08x).", ptszMsgName, wparam, lparam);
	LogChief(l_tszLogText);
}

/*
 *	ログファイル関数 - トランジション用
 */
void CChiefView::LogChief_Transi(TCHAR* ptszTransiState, DWORD dwParam)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("%s : param = 0x%08x"), ptszTransiState, dwParam);
	LogChief(l_tszLogText);
}

// 2013.01.10 bagus stage driver alarm io -->
/*
 *	ログファイル関数 - CNanoSpecDoc::StageErrorLogging() のラッパ関数
 */
void CChiefView::LogStageError(TCHAR *pszLogText)
{
	if (0 != m_pcNanoSpecDoc) {
		((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageErrorLogging(pszLogText);
	}
}
// 2013.01.10 bagus stage driver alarm io <--

/*
 *	装置電源オフ－出力
 */
void CChiefView::EqPowerOffPulse()
{
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- { ---------- */
	if (0 == g_bIL_EQPowerOff) {
		return;
	}
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- } ---------- */
	// 装置電源オフ：出力
	nexioEquipmentPowerOFF(TRUE);
	m_bCheckEqPowerOFF = 1;		// 監視フラグをセット(監視中にする)
	// 入力監視タイマ起動
	if (ID_TIMER_EQPWOFF != SetTimer(ID_TIMER_EQPWOFF, TIMER_EQPWOFF_TIMEOUT, 0)) {
		// タイマ起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[34]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(34, 0));
	}
}




// hmenjo 20090109 - DI 自動マニュアル入力 ----- --->
//	机上デバッグのために，Nextra の DIO モニタで設定する
//	マニュアル入力とパスワード("toho"固定)，それと，
//	必要な DI を自動的に入力します．
HANDLE lg_hEvFoundControl;
HWND lg_hwndFoundControl;
struct {
	TCHAR tszJPN[128];
	TCHAR tszENU[128];
} lg_DiName[] = {
	_T("緊急停止"),					_T("Emergency Stop"),		// 101
	_T("ドアインターロック"),		_T("Door Interlock"),		// 102
	_T("装置電源"),					_T("Eq Power"),				// 103
	_T("メンテナンススイッチ"),		_T("Maintenance SW"),		// 104
	_T("リフター1 下端センサー"),	_T("Pin1 Down Pos"),		// 105
	_T("リフター2 下端センサー"),	_T("Pin2 Down Pos"),		// 106
	_T("シャッターシリンダ1上端"),	_T("Shutter Top1"),			// 107
	_T("ロボットアーム"),			_T("Robot Arm"),			// 108
	_T("バキューム1"),				_T("Vacuum Pressure1"),		// 109
	_T("バキューム2"),				_T("Vacuum Pressure2"),		// 110
	_T("X軸 ロードポジション"),		_T("Load Position X"),		// 111
	_T("Y軸 ロードポジション"),		_T("Load Position Y"),		// 112
	_T("ワークガイド1 REV"),		_T("Work Guide1 REV"),		// 113
	_T("ワークガイド2 REV"),		_T("Work Guide2 REV"),		// 114
	_T("ワークガイド3 REV"),		_T("Work Guide3 REV"),		// 115
	_T("ワークガイド4 REV"),		_T("Work Guide4 REV"),		// 116
	_T("ワークガイド5 REV"),		_T("Work Guide5 REV"),		// 117
	_T("ワークガイド6 REV"),		_T("Work Guide6 REV"),		// 118
	_T("ワークガイド7 REV"),		_T("Work Guide7 REV"),		// 119
	_T("ワークガイド8 REV"),		_T("Work Guide8 REV"),		// 120
	_T(""),							_T(""),						// 165
};
BOOL CALLBACK EnumChildProc_SearchControl(HWND hWnd, LPARAM lParam)
{
	TCHAR l_tszSearchTextJ[256];
	TCHAR l_tszSearchTextE[256];
	switch (lParam) {
	case 1:
		_tcscpy(l_tszSearchTextJ, _T("マニュアル入力"));
		_tcscpy(l_tszSearchTextE, _T("Manual Input"));
		break;
	case 2:
		_tcscpy(l_tszSearchTextJ, _T(""));
		_tcscpy(l_tszSearchTextE, _T(""));
		break;
	case 3:
		_tcscpy(l_tszSearchTextJ, _T("OK"));
		_tcscpy(l_tszSearchTextE, _T("OK"));
		break;
	default:
		if ((101 <= lParam) && (lParam <= 164)) {
			_tcscpy(l_tszSearchTextJ, lg_DiName[lParam - 101].tszJPN);
			_tcscpy(l_tszSearchTextE, lg_DiName[lParam - 101].tszENU);
		}
		break;
	}
	TCHAR l_tszGotText[256];
	::GetWindowText(hWnd, l_tszGotText, sizeof(l_tszGotText));
	if (0 == _tcscmp(l_tszGotText, l_tszSearchTextJ)) {
		// 見つかった
	} else if (0 == _tcscmp(l_tszGotText, l_tszSearchTextE)) {
		// 見つかった
	} else {
		return TRUE;
	}
	lg_hwndFoundControl = hWnd;
	BOOL l_bRc = ::SetEvent(lg_hEvFoundControl);

	return FALSE;
}
void CChiefView::OnBtnTest()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// 「NexI/O モニター」を探します．
	HWND l_hwndNexIoMonitor = 0;
	BOOL l_bFind = FALSE;
	{
		for (int i = 0; i < 180; i++) {
			l_hwndNexIoMonitor = ::FindWindow(0, _T("NexI/O モニター"));
			if (0 != l_hwndNexIoMonitor) {
				l_bFind = TRUE; break;	// 見つかった
			} else {
				l_hwndNexIoMonitor = ::FindWindow(0, _T("NexI/O Monitor"));
				if (0 != l_hwndNexIoMonitor) {
					l_bFind = TRUE; break;	// 見つかった
				}
			}
			::Sleep(1000);
		}
	}
	if (0 == l_bFind) {
		// 無かった
		::MessageBox(m_hWnd, _T("Cannot found DIO Monitor Dialog."), _T("DIO Monitor Auto Manual Input"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	// 「マニュアル入力」チェックボックスを探します．
	lg_hEvFoundControl = ::CreateEvent(0, TRUE, FALSE, 0);		// 検出イベント
	EnumChildWindows(l_hwndNexIoMonitor, EnumChildProc_SearchControl, 1);
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
		// イベントが来なかった
		return;
	}
	::PostMessage(lg_hwndFoundControl, BM_CLICK, 0, 0);
	// 「パスワードの確認」ダイアログを探します．
	HWND l_hwndPassWord = 0;
	l_bFind = FALSE;
	{
		for (int i = 0; i < 1800; i++) {
			l_hwndPassWord = ::FindWindow(0, _T("パスワードの確認"));
			if (0 != l_hwndPassWord) {
				l_bFind = TRUE; break;	// 見つかった
			} else {
				l_hwndPassWord = ::FindWindow(0, _T("Password"));
				if (0 != l_hwndPassWord) {
					l_bFind = TRUE; break;	// 見つかった
				}
			}
			::Sleep(100);
		}
	}
	if (0 == l_bFind) {
		// 無かった
		::MessageBox(m_hWnd, _T("Cannot found Password Dialog."), _T("DIO Monitor Auto Manual Input"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	// エディットボックスを探す
	BOOL l_bRc = ::ResetEvent(lg_hEvFoundControl);
	EnumChildWindows(l_hwndPassWord, EnumChildProc_SearchControl, 2);
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
		// イベントが来なかった
		return;
	}
	::SendMessage(lg_hwndFoundControl, WM_SETTEXT, 0, (LPARAM) _T("toho"));
	// OK ボタンを探す
	l_bRc = ::ResetEvent(lg_hEvFoundControl);
	EnumChildWindows(l_hwndPassWord, EnumChildProc_SearchControl, 3);
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
		// イベントが来なかった
		return;
	}
	::PostMessage(lg_hwndFoundControl, BM_CLICK, 0, 0);
	// 必要な DI をクリックする
	{
		DWORD l_dwCounter = 0;
		while ((0 != lg_DiName[l_dwCounter].tszJPN[0]) && (0 != lg_DiName[l_dwCounter].tszENU[0])) {
			l_bRc = ::ResetEvent(lg_hEvFoundControl);
			EnumChildWindows(l_hwndNexIoMonitor, EnumChildProc_SearchControl, l_dwCounter + 101);
			if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
				// イベントが来なかった
				return;
			}
			::PostMessage(lg_hwndFoundControl, BM_CLICK, 0, 0);
			l_dwCounter++;
		}
	}
}
// hmenjo 20090109 - DI 自動マニュアル入力 ----- <---


void CChiefView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
}

void CChiefView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	LogChief(_T("Started  CChiefView::OnInitialUpdate()."));

	// Chief ダイアログのキャプションを設定
	SetWindowText(CHIEF_DLG_CAPTION);

	// 変数初期化 ------------------------------------------------------------
	m_dwHeartBeatCnt = 0;
// 2009.02.05 K.Matsuo delete -->
//	m_uiTraceDataPeriod = 0;
//	m_bTraceDataTimer = FALSE;
// 2009.02.05 K.Matsuo delete <--
	m_bEQRunPrev = FALSE;
	m_dwStageMoveState = 0;
/* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- { ---------- */
	m_dwPinMoveState = 0;
/* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- } ---------- */
	m_bVacuumOnFromDisp = FALSE;
	m_bVacuumOffFromDisp = FALSE;
	memset(m_szMainRecipeName, 0, sizeof(m_szMainRecipeName));
	m_dwModuleState_Stage = 0;	// 未初期化(Uninitialize)
	m_dwModuleState_SR_Meas = 0;	// 未初期化(Uninitialize)
	m_dwModuleState_Pif = 0;	// 未初期化(Uninitialize)
	m_dwModuleState_Nextra = 0;	// 未初期化(Uninitialize)
	m_hEvTrMaster = 0;
	m_bGotRecipeFromPif = FALSE;
	m_bReqCancelComplete = FALSE;
	memset(&m_AlarmFlags, 0, sizeof(ALARM_FLAGS));
	memset(&m_WaitMeasTime, 0, sizeof(WAIT_MEAS_TIME));
	m_bCheckEqPowerOFF = 0;
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
	m_pcChiefTransiStress = 0;
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */

/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- { ---------- */
	/* ヘッドコンフィグをコピー	*/
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	memcpy(&m_EnableHead, &l_SystemConfig.HeadType, sizeof(HEAD_TYPE_CONFIG));
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- } ---------- */


	///// HIDE Button /////
	m_HideButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_HideButton.DrawFlatFocus(TRUE);

	///// TOP Button /////
	m_TopButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TopButton.DrawFlatFocus(TRUE);

	///// TEST Button /////
	m_TestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TestButton.DrawFlatFocus(TRUE);

	// 設定ファイル読込み ----------------------------------------------------
	// 設定ファイル読込み - 「forTEST」ボタンの有効/無効 IDC_BTN_TEST
	char szFilePath[MAX_PATH];
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	BOOL l_bforTESTBtn = GetPrivateProfileInt(_T("ForTest"), _T("forTESTBtn"), 0, szFilePath);
	if (0 == l_bforTESTBtn) {
		GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_SHOW);
	}
	// 設定ファイル読込み - 入力無効スイッチ - ロボットアーム 検出
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DioIgnoreSW.bRobotArm = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("RobotArm"), 0, szFilePath);
	if (0 == m_DioIgnoreSW.bRobotArm) {
		m_DioIgnoreSW.bRobotArm = FALSE;
	} else {
		m_DioIgnoreSW.bRobotArm = TRUE;
	}
	// 設定ファイル読込み - 入力無効スイッチ - ピンダウン 検出
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DioIgnoreSW.bPinDown = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("PinDown"), 0, szFilePath);
	if (0 == m_DioIgnoreSW.bPinDown) {
		m_DioIgnoreSW.bPinDown = FALSE;
	} else {
		m_DioIgnoreSW.bPinDown = TRUE;
	}
	// 設定ファイル読込み - 入力無効スイッチ - エア圧力低下 検出
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DioIgnoreSW.bAirPressureLow = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("AirPressureLow"), 0, szFilePath);
	if (0 == m_DioIgnoreSW.bAirPressureLow) {
		m_DioIgnoreSW.bAirPressureLow = FALSE;
	} else {
		m_DioIgnoreSW.bAirPressureLow = TRUE;
	}
/* added 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- { ---------- */
	/* 設定ファイル読込み - ピン＆シャッタ同時動作	*/
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	m_DioIgnoreSW.bPinShutterILInvalid = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("PinShutterILInvalid"), 0, szFilePath);
	if (1 == m_DioIgnoreSW.bPinShutterILInvalid) {
		m_DioIgnoreSW.bPinShutterILInvalid = TRUE;
	} else {
		m_DioIgnoreSW.bPinShutterILInvalid = FALSE;
	}
/* added 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- } ---------- */
	// 設定ファイル読込み - 移動速度制限設定
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	m_bSpeedLimiterOFF = GetPrivateProfileInt(_T("SpeedLimit"), _T("LimiterOFF"), 0, szFilePath);
	if (0 == m_bSpeedLimiterOFF) {
		m_bSpeedLimiterOFF = FALSE;
	} else {
		m_bSpeedLimiterOFF = TRUE;
	}
	// 設定ファイル読込み - SEQ 先行移動フラグ
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	m_bPreMoveSW = GetPrivateProfileInt(_T("PreMove"), _T("PreMoveSW"), 0, szFilePath);
	if (0 == m_bPreMoveSW) {
		m_bPreMoveSW = FALSE;
	} else {
		m_bPreMoveSW = TRUE;
	}
	// 設定ファイル読込み - 測定前待ち時間
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	m_WaitMeasTime.dwWaitTimeSetting = GetPrivateProfileInt(_T("PreMove"), _T("WaitMeasTime"), 0, szFilePath);
	if (10000 < m_WaitMeasTime.dwWaitTimeSetting) {
		m_WaitMeasTime.dwWaitTimeSetting = 10000;
	}
	// 設定ファイル読込み - ダイアログ表示方法
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DlgShowSW.dwType = GetPrivateProfileInt(_T("ShowSW"), _T("Type"), 0, szFilePath);
	if (2 < m_DlgShowSW.dwType) {
		m_DlgShowSW.dwType = 0;
	}
	// 設定ファイル読込み - ダイアログ表示ディレイ [s]
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DlgShowSW.dwDelay = GetPrivateProfileInt(_T("ShowSW"), _T("Delay"), 5, szFilePath);
	if (m_DlgShowSW.dwDelay < 2) {
		m_DlgShowSW.dwDelay = 2;
	}
	if (10 < m_DlgShowSW.dwDelay) {
		m_DlgShowSW.dwDelay = 10;
	}
	// Chief ダイアログ表示タイマ起動
	if (2 != m_DlgShowSW.dwType) {
		m_DlgShowSW.dwShowSWprc = 0;
		if (ID_TIMER_HIDEDLG != SetTimer(ID_TIMER_HIDEDLG, 500, 0)) {
			// タイマ起動失敗
			LogChief(CHIEF_REP_ALARM_MSGTEXT[2]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(2, 0));
		}
	}

#if 0		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------
//	// NextraIO を初期化 ------------------------------------------------------
//	m_dwModuleState_Nextra = 0;	// 未初期化(Uninitialize)
//	if (0 != nexifInitialize(this->m_hWnd)) {
//		m_dwModuleState_Nextra = 1;	// 初期化完了(Initialized)
//	}
#endif		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------
#if 0	// hmenjo NEXIOBASE.HXX に変わったので不要
//	m_dwModuleState_Nextra = 0;	// 未初期化(Uninitialize)
//	if (0 != nexioInitialize(this->m_hWnd)) {
//		m_dwModuleState_Nextra = 1;	// 初期化完了(Initialized)
//	}
#endif 	// hmenjo NEXIOBASE.HXX に変わったので不要

#if 0		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------
//	// Pif の初期化 -----------------------------------------------------------
//	m_dwModuleState_Pif = 0;	// 未初期化(Uninitialize)
//	PifComm_Init();
//	m_dwModuleState_Pif = 1;	// 初期化完了(Initialized)
#endif		// ここの初期化処理があると NanoSpec.exe 正常に終了出来ないため，元々の MainFrame に戻しました --------------------
	// 初期 DI 値を取得して共有エリア(プロセスステータス報告用ファイルマッピング)に初期値としてセットします．
	GetDiInfo(&m_DiInfo);
	OnPifDiRefresh(0, 0);

	// タイマ系設定 ----------------------------------------------------------
	// マルチメディア タイマ起動
	if (0 == (m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD] = ::timeSetEvent(TIMER_5MS_PERIOD, 1, MMTimerProc, (DWORD) m_hWnd, TIME_PERIODIC))) {
		// タイマ起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[15]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(15, 0));
	}
	// 1s 定周期タイマ起動
	if (ID_TIMER_1S_PERIOD != SetTimer(ID_TIMER_1S_PERIOD, TIMER_1S_PERIOD, 0)) {
		// タイマ起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[14]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(14, 0));
	}
	// 100ms 定周期タイマ起動
	if (ID_TIMER_100MS_PERIOD != SetTimer(ID_TIMER_100MS_PERIOD, TIMER_100MS_PERIOD, 0)) {
		// タイマ起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[13]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(13, 0));
	}
	// 50ms 定周期タイマ起動
	if (ID_TIMER_50MS_PERIOD != SetTimer(ID_TIMER_50MS_PERIOD, TIMER_50MS_PERIOD, 0)) {
		// タイマ起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[12]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(12, 0));
	}
	// 10ms 定周期タイマ起動
	if (ID_TIMER_10MS_PERIOD != SetTimer(ID_TIMER_10MS_PERIOD, TIMER_10MS_PERIOD, 0)) {
		// タイマ起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[11]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(11, 0));
	}

// 2009.02.05 K.Matsuo delete -->
//	// トレースデータ定時送信タイマ起動
//	if (0 != (m_uiTraceDataPeriod = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetTraceDataPeriod())) {
//		if (0 == StartTimerTraceData(m_uiTraceDataPeriod)) {
//			// タイマ起動失敗
//			LogChief(CHIEF_REP_ALARM_MSGTEXT[0]);
//			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(0, 0));
//		} else {
//			// タイマ起動成功
//			m_bTraceDataTimer = TRUE;
//		}
//	}
// 2009.02.05 K.Matsuo delete <--

	// メール受信スレッドの起動 ----------------------------------------------
	LogChief(_T("Requested to start CChiefRcvMailThread."));
	m_pcChiefRcvMailThread = (CChiefRcvMailThread*) AfxBeginThread(
														RUNTIME_CLASS(CChiefRcvMailThread),
														THREAD_PRIORITY_NORMAL,
														0,
														CREATE_SUSPENDED,
														0
													);
	if (0 != m_pcChiefRcvMailThread) {
		// ここにはスレッドの初期化の処理を(必要なら)書いてください
		m_pcChiefRcvMailThread->m_bAutoDelete = FALSE;							// スレッド終了時にオブジェクトを自動的に破棄しない設定です．
		((CChiefRcvMailThread*) m_pcChiefRcvMailThread)->m_pcChiefView = this;	// 親(自分)クラスをスレッドに渡しておく
		// スレッド起動
		m_pcChiefRcvMailThread->ResumeThread();
	} else {
		// メール受信スレッド起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[10]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(10, 0));
	}

	// 各トランジション スレッドの起動 ----------------------------------------------
	m_hEvTrMaster = ::CreateEvent(0, TRUE, FALSE, 0);	// マスタトランジションスレッドの初期化完了待ち用
	DWORD l_dwTransi;
	if (0 != (l_dwTransi = TransitionsStart())) {
		// 各トランジション スレッド起動失敗
		LogChief(CHIEF_REP_ALARM_MSGTEXT[l_dwTransi + 3]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(l_dwTransi + 3, 0));
	}
	// マスタトランジションの初期化完了を待つ -----------------------------------------
	if (0 == m_hEvTrMaster) {
		// イベント作成が失敗した
		LogChief(CHIEF_REP_ALARM_MSGTEXT[17]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(17, 0));
	} else {
		// CHIEF_TRMAS_INIT_TIME[ms] 待つ
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvTrMaster, CHIEF_TRMAS_INIT_TIME)) {
			// タイムアウトしました(マスタトランジションの初期化が完了しませんでした)．
			LogChief(CHIEF_REP_ALARM_MSGTEXT[18]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(18, 0));
		}
	}

	// DIO 監視開始タイマ(アラーム系のウィンドウが立ち上がるまでの待ち処理)
	lg_uiRunFlagCount = 5000 / TIMER_10MS_PERIOD;	// DIO 監視開始タイマ 5[s]

	// 測定モジュールへウィンドウメッセージ送信用の Chief のウィンドウハンドルを通知
	MEAS_SetNotifyWnd(m_hWnd);

// 初期化時のステージ速度制限は MainFram に移動しました．
//	// (初期化時の)ステージ速度制限
//	if (0 == m_DiInfo.bMaintenanceSW) {
//		if (0 == m_bSpeedLimiterOFF) {
//			// 速度制限モードをセット
//			((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(TRUE);
//		}
//	} else {
//		// 速度制限モードをリセット
//		((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(FALSE);
//	}

	LogChief(_T("Ended    CChiefView::OnInitialUpdate()."));

}


void CChiefView::PrepareToDestroy()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	LogChief(_T("Ending   Chief Dialog..."));

	TRACE(_T("CChiefView::PostNcDestroy() \n"));

	// EQ 動作中出力をオフします．
	nexioEquipmentStatusRun(FALSE);
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- { ----------
//	// リミッタを有効にして終了します．
//	StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ----------
	if (TRUE == g_bIL_SpeedDown) {
		// リミッタを有効にして終了します．
		StageEnableSpeedLimit();
	}
// modified hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- } ----------

/* added 2009.08.04 hmenjo 測定中以外ドア開ステージ停止 ---------- { ---------- */
	// マルチメディアタイマを削除 ---------------------------------------------
	::timeKillEvent(m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD]);
/* added 2009.08.04 hmenjo 測定中以外ドア開ステージ停止 ---------- } ---------- */

	// メール受信スレッドの後始末 ---------------------------------------------
	if (0 != m_pcChiefRcvMailThread) {
		LogChief(_T("Deleting CChiefRcvMailThread..."));
		((CChiefRcvMailThread*) m_pcChiefRcvMailThread)->ShutDown();
		::WaitForSingleObject(m_pcChiefRcvMailThread->m_hThread, INFINITE);
		delete m_pcChiefRcvMailThread;
		m_pcChiefRcvMailThread = NULL;
		LogChief(_T("Deleted  CChiefRcvMailThread."));
	}

	// マスタトランジションを終了 ---------------------------------------------
	// マスタトランジションの終了完了を待つ
	::ResetEvent(m_hEvTrMaster);
	// マスタトランジションを終了
	((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_END);
	if (0 != m_hEvTrMaster) {
		// CHIEF_TRMAS_END_TIME[ms] 待つ
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvTrMaster, CHIEF_TRMAS_END_TIME)) {
			// タイムアウトしました(マスタトランジションの終了が完了しませんでした)．
//			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(18, 0));
			LogChief(CHIEF_REP_ALARM_MSGTITLE[1]);
			::MessageBox(0, CHIEF_REP_ALARM_MSGTEXT[23], CHIEF_REP_ALARM_MSGTITLE[1], MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		}
	}

	// 各トランジションスレッドの後始末 ---------------------------------------
	TransitionsEnd();

/* deleted 2009.08.04 hmenjo 測定中以外ドア開ステージ停止 ---------- { ---------- */
//	// マルチメディアタイマを削除 ---------------------------------------------
//	::timeKillEvent(m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD]);
/* deleted 2009.08.04 hmenjo 測定中以外ドア開ステージ停止 ---------- } ---------- */

	// NextraIO を終了 --------------------------------------------------------
//	nexioExitInstance();	hmenjo NEXIOBASE.HXX に変わったので不要
	m_dwModuleState_Nextra = 9;	// 終了(Terminated)

	// Pif の初期化 -----------------------------------------------------------
	//		Pif の終了処理は不要です．

	// レシピ情報用メモリを解放
	RecipesMalloc(FALSE);

	// ポインタ無効化
	g_pcChiefView = NULL;
}

/* added 2009.05.27 hmenjo SPT アラームの検出 ---------- { ---------- */
void CChiefView::SPTAlarmDetector(void)
{
// 2009.08.20 K.Matsuo ALID change (Ref. alarmIf.hxx) -->
	// ALID_SptAlarmStartNo	1020	// アラームコード開始番号
	switch (StageGetMotSysErr(TRUE)) {
	case 0:												break;	// エラー無し
	case 1:		AlarmIf_Set(ALID_SptAlarmStartNo + 0);	break;	// WD エラー											ALID_SptStageWdError
	case 2:		AlarmIf_Set(ALID_SptAlarmStartNo + 1);	break;	// リブートしなさいのメッセージ 						ALID_SptStageNeedRebootPc
	case 3:		AlarmIf_Set(ALID_SptAlarmStartNo + 2);	break;	// アラーム発生 										ALID_SptStageAlarm
	case 4:		AlarmIf_Set(ALID_SptAlarmStartNo + 3);	break;	// 初期化完了フラグが０にならなかった					ALID_SptStageInitCompFlagNotZero
	case 5:		AlarmIf_Set(ALID_SptAlarmStartNo + 4);	break;	// DIO エラー											ALID_SptStageDioError
	case 6:		AlarmIf_Set(ALID_SptAlarmStartNo + 5);	break;	// サーボオンエラー(オンしなかった/オフしなかった)		ALID_SptStageServoOnError
	case 7:		AlarmIf_Set(ALID_SptAlarmStartNo + 6);	break;	// 非常停止(サーボオンエラー(オンしていなかった))		ALID_SptStageEmergencyStop
	case 111:	AlarmIf_Set(ALID_SptAlarmStartNo + 7);	break;	// 通信(Read)システムエラー発生 						ALID_SptStageCommRead_SystemError
	case 112:	AlarmIf_Set(ALID_SptAlarmStartNo + 8);	break;	// 通信(Read)リトライアウト１発生						ALID_SptStageCommRead_RetryOut1
	case 113:	AlarmIf_Set(ALID_SptAlarmStartNo + 9);	break;	// 通信(Read)リトライアウト２発生						ALID_SptStageCommRead_RetryOut2
	case 121:	AlarmIf_Set(ALID_SptAlarmStartNo + 10);	break;	// 通信(Read2)システムエラー発生						ALID_SptStageCommRead2_SystemError
	case 122:	AlarmIf_Set(ALID_SptAlarmStartNo + 11);	break;	// 通信(Read2)リトライアウト１発生						ALID_SptStageCommRead2_RetryOut1
	case 123:	AlarmIf_Set(ALID_SptAlarmStartNo + 12);	break;	// 通信(Read2)リトライアウト２発生						ALID_SptStageCommRead2_RetryOut2
	case 131:	AlarmIf_Set(ALID_SptAlarmStartNo + 13);	break;	// 通信(Write)システムエラー発生						ALID_SptStageCommWrite_SystemError
	case 132:	AlarmIf_Set(ALID_SptAlarmStartNo + 14);	break;	// 通信(Write)リトライアウト発生						ALID_SptStageCommWrite_RetryOut
/* added 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- { ---------- */
	case 23:	AlarmIf_Set(ALID_SptAlarmStartNo + 16);	break;	/* 移動タイムアウト発生 	  MM_TIMEOUT				ALID_SptStageMotionTimeout	*/
/* added 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- } ---------- */
	default:	AlarmIf_Set(ALID_SptAlarmStartNo + 15);	break;	// 未定義エラー 										ALID_SptStageNotDefinedError
// 2009.08.20 K.Matsuo ALID change <--
	}
}
/* added 2009.05.27 hmenjo SPT アラームの検出 ---------- } ---------- */

/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
/*
 *	測定ポーズを画面モジュールに通知
 */
void CChiefView::SentMeasPause(int iReason, BOOL bRelease/* = FALSE*/)
	/*	int iReason		ポーズ理由
	 *	BOOL bRelease	TRUE：ポーズ解除	*/
{
/* added 2009.06.09 hmenjo 異常時測定ポーズ ログ追加 ---------- { ---------- */
	TCHAR l_tszLogMsg[256];
	_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_NOTIFY_MEAS_PAUSE (WPARAM = 0x%08x, LPARAM = 0x%08x)"), iReason, bRelease);
	LogChief(l_tszLogMsg);
/* added 2009.06.09 hmenjo 異常時測定ポーズ ログ追加 ---------- } ---------- */
	((CMainFrame*) m_pcMainFrame)->PostMessage(WM_CHIF_NOTIFY_MEAS_PAUSE, (WPARAM) iReason, (LPARAM) bRelease);
}
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */

/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
/*
 *	測定ポーズを画面モジュールに通知
 */
void CChiefView::SentMeasPauseCheck(int iReason)
{
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- { ---------- */
//	if (
//		(PROCESS_PROC == ProcStatusGet())
//	 && (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())
//	 && (ST_SEQ_PAUSE != ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentState())
//	 && (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsPauseSeq())
//		) {
//		// Processing で，シーケンス測定が IDLE とポーズ以外の場合
//		DWORD l_dwCurrPointNo = ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentPointNo();
//		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (m_ChiefRecipes.pStageProgInfoHdr);
//		DWORD l_dwNextPointNo = l_dwCurrPointNo + 1;
//		if (
//			(l_dwCurrPointNo < l_pStageProgInfoHdr->wNumScans)
//		 && (GetNextPointNo(&l_dwNextPointNo) <= l_pStageProgInfoHdr->wNumScans)
//			) {
//			// 最終ポイント以外ならポーズさせます．
//			((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);	// ポーズ
//			// 測定ポーズを通知
//			SentMeasPause(iReason);
//		}
//	}
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ----------			   */
	if (PROCESS_PROC == ProcStatusGet()) {
		/* Processing の場合のみ	*/
		CHIEF_PFUNCS l_ChiefPFuncs;
		WORD l_wHeadType;
		if (0 != PFC_FuncSet(this, &l_ChiefPFuncs, &l_wHeadType)) {
			if (
				(false == (*l_ChiefPFuncs.IsIdle)(this))
			 && (l_ChiefPFuncs.State.iPAUSE != (*l_ChiefPFuncs.GetCurrentState)(this))
			 && (false == (*l_ChiefPFuncs.IsPauseSeq)(this))
				) {
				/* シーケンス測定が IDLE とポーズ以外の場合	*/
				BOOL l_bDoPause = FALSE;
				switch (l_wHeadType) {
				case HEAD_TYPE_SR:
					{
						DWORD l_dwCurrPointNo = ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentPointNo();
						LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (m_ChiefRecipes.pStageProgInfoHdr);
						DWORD l_dwNextPointNo = l_dwCurrPointNo + 1;
						if (
							(l_dwCurrPointNo < l_pStageProgInfoHdr->wNumScans)
						 && (this->GetNextPointNo(&l_dwNextPointNo) <= l_pStageProgInfoHdr->wNumScans)
							) {
							l_bDoPause = TRUE;
						}
					}
					break;
				case HEAD_TYPE_STRESS:
					{
						DWORD l_dwCurrLineNo = ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentLineNo();
						STRESS_CONFIG l_StressConfig;
						ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
						DWORD l_dwNextLineNo = l_dwCurrLineNo + 1;
						if (
							(l_dwCurrLineNo < l_StressConfig.dwLiftPinNumberOfLine)
						 && (this->GetNextLineNo(&l_dwNextLineNo) <= l_StressConfig.dwLiftPinNumberOfLine)
							) {
							l_bDoPause = TRUE;
						}
					}
					break;
				}
				if (TRUE == l_bDoPause) {
					// 最終ポイント以外ならポーズさせます．
					(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iPAUSE, 0);	/* ポーズ	*/
					// 測定ポーズを通知
					this->SentMeasPause(iReason);
				}
			}
		}
	}
/* modified 2009.08.07 hmenjo ストレス機能追加(37) ---------- } ---------- */
}
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
/* added 2009.07.30 hmenjo ストレス機能追加(3) ---------- { ---------- */
/*
 *	HEPA オン/オフを制御
 */
BOOL CChiefView::HepaOnOff(BOOL bOn, char* pcHepaAlarmLevel/*= 0*/)
{
	STRESS_CONFIG l_StressConfig;
	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	l_StressConfig.dwScanStartWaitTime;	/* HEPA 待ち時間[s]	*/
	if (0 == l_StressConfig.dwScanStartWaitTime) {
		/* 待ち時間が０[s]なので HEPA 制御しない．	*/
		if (FALSE == bOn) {
			((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_HEPA_STOP, (cEventParams*) EV_STRS_HEPA_STOP);
		}
		if (0 != pcHepaAlarmLevel) {*pcHepaAlarmLevel = 0;}
		return TRUE;
	}

	BOOL l_bRet = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->HepaOnOff(bOn, pcHepaAlarmLevel);

	if ((TRUE == l_bRet) && (FALSE == bOn)) {
		/* HEPA オフ指令で正常終了なので，待ちタイマ開始	*/
		if (ID_TIMER_HEPASTOP != this->SetTimer(ID_TIMER_HEPASTOP, l_StressConfig.dwScanStartWaitTime * 1000, 0)) {
			/* タイマ起動失敗	*/
			((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_HEPA_STOP, (cEventParams*) EV_STRS_HEPA_STOP);
			this->LogChief(CHIEF_REP_ALARM_MSGTEXT[37]);
			this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(37, 0));
		}
	}

	return l_bRet;
}
/* added 2009.07.30 hmenjo ストレス機能追加(3) ---------- } ---------- */
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- { ---------- */
/*
 *	１ライン測定完了 メッセージハンドラ (ストレス専用)
 *		測定モジュールで１ライン測定データの取得が完了した．
 */
LRESULT CChiefView::OnMeasLineEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MEAS_LINE_END"), wparam, lparam);

	((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LINEMEAS_DONE, (cEventParams*) MAKELONG(EV_STRS_LINEMEAS_DONE, LOWORD(wparam)));

	return 0L;
}
/*
 *	１ラインデータ処理完了(応答) メッセージハンドラ (ストレス専用)
 *		データ処理モジュールで１ライン測定データの処理が完了した．
 */
LRESULT CChiefView::OnDataLineEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_LINE_END"), wparam, lparam);

	((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LINEDATA_DONE, (cEventParams*) EV_STRS_LINEDATA_DONE);

	return 0L;
}
/*
 *	１枚測定終了応答 メッセージハンドラ (ストレス専用)
 *		データ処理モジュールで１枚データの処理が完了した．
 */
LRESULT CChiefView::OnDataStressMeasEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_STRS_MEAS_END"), wparam, lparam);

	((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_COMPLETE, (cEventParams*) EV_STRS_COMPLETE);

	return 0L;
}
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- } ---------- */
/* added 2009.08.06 hmenjo ストレス機能追加(31) ---------- { ---------- */
BOOL CChiefView::IsHWS()
{
	return ((CMainFrame*) m_pcMainFrame)->HardwareSimulation();
}
/* added 2009.08.06 hmenjo ストレス機能追加(31) ---------- } ---------- */

/* added 2009.08.25 hmenjo SE ランプフィルタ制御関数 ---------- { ---------- */
/*
 *	ランプフィルタ制御Ｒ
 */
#define	LFR_LOG	1	/* 1：ログ出力する	*/
BOOL CChiefView::LampFilterR(
		WORD wFilter,	/* 仕様による	デフォルト：0	*/
		WORD wMode		/* 仕様による	デフォルト：0	*/
	)
{
	BOOL l_bRet = TRUE;
	WORD l_wFilter = wFilter;
	WORD l_wMode = wMode;

/* modified 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- { ---------- */
/*		コメントを削除します．(見にくいので)	*/
///* added 2009.09.14 hmenjo LampFilterR() ログ追加 ---------- { ---------- */
//	BOOL l_bLogWR = (LFR_LOG)? TRUE : FALSE;
///* added 2009.09.14 hmenjo LampFilterR() ログ追加 ---------- } ---------- */
//
//	switch (l_wMode) {
///* modified 2009.09.07 hmenjo 透過率 ランプ制御 ---------- { ---------- */
////	case 1:	/* SE ヘッドの場合 --------------------------------------------------------*/
////	case 2:	/* SE ヘッドで通常モードの場合 --------------------------------------------*/
////		if (0 != m_ChiefRecipes.pMeasProgInfo) {
////			if (HEAD_TYPE_SE == ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType) {
////				/* SE ヘッド	*/
/////* modified 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
//////				SE_CONFIG l_SeConfig;
//////				ConfigFile_GetNanoSpecIni(&l_SeConfig, CONFIG_FILE_SE_CONFIG);
//////				if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeConfig.bDoNotMoveShutter))) {
/////* modified 2009.09.01 hmenjo SE 構造体追加(12) ----------				*/
////				SE_SETTING l_SeSetting;
////				ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
////				if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeSetting.bDoNotMoveShutter))) {
/////* modified 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */
////					/* モード１か，モード２で通常モードの場合のみ実行します．	*/
////					switch (l_wFilter) {
////					case FILTER_OPEN:
////						MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
////// K.Matsuo 2009.09.01 -->
////						MEAS_SeHead_OpenLampShutter();
////// K.Matsuo 2009.09.01 <--
////						break;
////					case FILTER_DARK:
////					default:
////// K.Matsuo 2009.09.01 -->
////						MEAS_SeHead_CloseLampShutter();
////// K.Matsuo 2009.09.01 <--
////						MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
////						break;
////					}
////				}
////			}
////		}
///* modified 2009.09.07 hmenjo 透過率 ランプ制御 ----------			  */
//	case 1:	/* 無条件で実行 -----------------------------------------------------------*/
//	case 2:	/* 通常モードの場合のみ実行 -----------------------------------------------*/
//		if (0 != m_ChiefRecipes.pMeasProgInfo) {
//			WORD l_wHeadType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType;
//			switch (l_wHeadType) {
///* added 2009.10.29 hmenjo CTA 常に SR フィルタ CLOSE ---------- { ---------- */
//			case HEAD_TYPE_CTA:
///* added 2009.11.06 K.Matsuo RS 常に SR フィルタ CLOSE ---------- { ---------- */
//			case HEAD_TYPE_4PP:
///* added 2009.11.06 K.Matsuo RS 常に SR フィルタ CLOSE ---------- } ---------- */
//// 2009.11.07 bagus MS --{--
//			case HEAD_TYPE_MS:
//// 2009.11.07 bagus MS --}--
//				if (1 == l_wMode) {
//					switch (l_wFilter) {
//					case FILTER_OPEN:
//						MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
//						break;
//					case FILTER_DARK:
//					default:
//						MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
//						break;
//					}
//				}
//				break;
///* added 2009.10.29 hmenjo CTA 常に SR フィルタ CLOSE ---------- } ---------- */
//			case HEAD_TYPE_SE:
//				{
//					SE_SETTING l_SeSetting;
//					ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
//					if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeSetting.bDoNotMoveShutter))) {
//						/* モード１か，モード２で通常モードの場合のみ実行します．	*/
//						switch (l_wFilter) {
//						case FILTER_OPEN:
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- { ---------- */
/////* modified 2009.09.14 hmenjo SE 時は SR ランプ OPEN でよい ---------- { ---------- */
//////							MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
/////* modified 2009.09.14 hmenjo SE 時は SR ランプ OPEN でよい ----------				*/
////							MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/////*hdebdeb*/this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));
/////* modified 2009.09.14 hmenjo SE 時は SR ランプ OPEN でよい ---------- } ---------- */
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- 			 */
//							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- } ---------- */
//							MEAS_SeHead_OpenLampShutter();
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_OPEN-1,2"));}
//							break;
//						case FILTER_DARK:
//						default:
//							MEAS_SeHead_CloseLampShutter();
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_DARK-1,2"));}
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- { ---------- */
////							MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/////*hdebdeb*/this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- 			 */
//							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- } ---------- */
//							break;
//						}
//					}
//				}
//				break;
//			case HEAD_TYPE_SR:
//				{
//					WORD l_wScanType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wScanType;
//					if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType)
//					 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_wScanType)) {
//						/* SR ヘッドで透過率の場合	*/
//						SR_TRANSMIT l_SrTransmittance;
//						ConfigFile_GetNanoSpecIni(&l_SrTransmittance, CONFIG_FILE_SR_TRANSMIT);
//						if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SrTransmittance.bDoNotCheckMeasPoint))) {
//							/* モード１か，モード２で通常モードの場合のみ実行します．	*/
//							switch (l_wFilter) {
//							case FILTER_OPEN:
//								MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-1,2"));}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
////								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ----------			  */
//								if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//									MEAS_SrHead_OpenTransShutter();
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_OPEN-1,2"));}
//								} else {
//									((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_OPEN-1,2"));}
//								}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
//								break;
//							case FILTER_DARK:
//							default:
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- { ---------- */
////								MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- 			 */
//								MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SR.wOpticsFilterType);
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- } ---------- */
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
////								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ----------			  */
//								if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//									MEAS_SrHead_CloseTransShutter();
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_DARK-1,2"));}
//								} else {
//									((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_DARK-1,2"));}
//								}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
//								break;
//							}
//						}
//					}
//				}
//				break;
//			default:
//				/* 何もしません．	*/
//				break;
//			}
//		}
///* modified 2009.09.07 hmenjo 透過率 ランプ制御 ---------- } ---------- */
//		break;
///* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
//	case 3:	/* 両方とも無条件で指定された状態にする -----------------------------------*/
//		if (0 != m_ChiefRecipes.pMeasProgInfo) {
//			WORD l_wHeadType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType;
//			switch (l_wHeadType) {
//			case HEAD_TYPE_SE:
//				switch (l_wFilter) {
//				case FILTER_OPEN:
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- { ---------- */
////					MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- 			 */
//					MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
//					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- } ---------- */
//					MEAS_SeHead_OpenLampShutter();
//					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_OPEN-3"));}
//					break;
//				case FILTER_DARK:
//				default:
//					MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
//					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-3"));}
//					MEAS_SeHead_CloseLampShutter();
//					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_DARK-3"));}
//					break;
//				}
//				break;
//			case HEAD_TYPE_SR:
//				{
//					WORD l_wScanType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wScanType;
//					if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType)
//					 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_wScanType)) {
//						/* SR ヘッドで透過率の場合	*/
//						switch (l_wFilter) {
//						case FILTER_OPEN:
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- { ---------- */
////							MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- 			 */
//							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SR.wOpticsFilterType);
///* modified 2009.09.14 hmenjo LampFilterR SR は測定 PGM 設定値 ---------- } ---------- */
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
////							((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ----------			  */
//							if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//								MEAS_SrHead_OpenTransShutter();
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_OPEN-3"));}
//							} else {
//								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_OPEN-3"));}
//							}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
//							break;
//						case FILTER_DARK:
//						default:
//							MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-3"));}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
////							((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ----------			  */
//							if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//								MEAS_SrHead_CloseTransShutter();
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_DARK-3"));}
//							} else {
//								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_DARK-3"));}
//							}
///* modified 2009.09.09 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
//							break;
//						}
//					}
//				}
//				break;
//			default:
//				/* 何もしません．	*/
//				break;
//			}
//		}
//		break;
///* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
//	default:
//		/* 何もしません．	*/
//		break;
//	}
/* modified 2009.11.20 hmenjo GTR ランプ初期化時 Close ----------			   */
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		// オートフィルターは実行せずに終了する
		return l_bRet;
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	BOOL l_bLogWR = (LFR_LOG)? TRUE : FALSE;
	BOOL l_bGTRLampOK = TRUE;

	switch (l_wMode) {
	case 1:	/* 無条件で実行 -----------------------------------------------------------*/
	case 2:	/* 通常モードの場合のみ実行 -----------------------------------------------*/
		if (0 != m_ChiefRecipes.pMeasProgInfo) {
			WORD l_wHeadType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType;
			switch (l_wHeadType) {
			case HEAD_TYPE_CTA:
			case HEAD_TYPE_4PP:
			case HEAD_TYPE_MS:
				if (1 == l_wMode) {
					switch (l_wFilter) {
					case FILTER_OPEN:
						MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
						break;
					case FILTER_DARK:
					default:
						MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
						break;
					}
				}
				break;
			case HEAD_TYPE_SE:
				{
					SE_SETTING l_SeSetting;
					ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
					if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeSetting.bDoNotMoveShutter))) {
						/* モード１か，モード２で通常モードの場合のみ実行します．	*/
						switch (l_wFilter) {
						case FILTER_OPEN:
							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
							MEAS_SeHead_OpenLampShutter();
							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_OPEN-1,2"));}
							break;
						case FILTER_DARK:
						default:
							MEAS_SeHead_CloseLampShutter();
							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_DARK-1,2"));}
							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
							break;
						}
					}
				}
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			case HEAD_TYPE_COMPEASE:
				{
					if( ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
						COMPEASE_SETTING l_EASESetting;
						ConfigFile_GetNanoSpecIni(&l_EASESetting, CONFIG_FILE_COMPEASE_SETTING);
						if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_EASESetting.bDoNotMoveShutter))) {
							/* モード１か，モード２で通常モードの場合のみ実行します．	*/
							switch (l_wFilter) {
							case FILTER_OPEN:
// 								MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._COMPEASE.wOpticsFilterType);
// 								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
								MEAS_CompEASEHead_OpenLampShutter();
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - EASE: FILTER_OPEN-1,2"));}
								break;
							case FILTER_DARK:
							default:
								MEAS_CompEASEHead_CloseLampShutter();
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - EASE: FILTER_DARK-1,2"));}
// 								MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._COMPEASE.wOpticsFilterType);
// 								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
								break;
							}
						}
					}
				}
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			case HEAD_TYPE_SR:
				{
					WORD l_wScanType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wScanType;
					if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType)
					 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_wScanType)) {
						/* SR ヘッドで透過率の場合	*/
						SR_TRANSMIT l_SrTransmittance;
						ConfigFile_GetNanoSpecIni(&l_SrTransmittance, CONFIG_FILE_SR_TRANSMIT);
						if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SrTransmittance.bDoNotCheckMeasPoint))) {
							/* モード１か，モード２で通常モードの場合のみ実行します．	*/
							switch (l_wFilter) {
							case FILTER_OPEN:
								MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-1,2"));}
								if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
									MEAS_SrHead_OpenTransShutter();
									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_OPEN-1,2"));}
								} else {
									l_bGTRLampOK = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_OPEN-1,2"));}
								}
								break;
							case FILTER_DARK:
							default:
								MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SR.wOpticsFilterType);
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
								if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
									MEAS_SrHead_CloseTransShutter();
									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_DARK-1,2"));}
								} else {
									l_bGTRLampOK = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_DARK-1,2"));}
								}
								break;
							}
						}
					}
				}
				break;
			default:
				/* 何もしません．	*/
				break;
			}
		}
		break;
	case 3:	/* 両方とも無条件で指定された状態にする -----------------------------------*/
		if (0 != m_ChiefRecipes.pMeasProgInfo) {
			WORD l_wHeadType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType;
			switch (l_wHeadType) {
			case HEAD_TYPE_SE:
				switch (l_wFilter) {
				case FILTER_OPEN:
					MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
					MEAS_SeHead_OpenLampShutter();
					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_OPEN-3"));}
					break;
				case FILTER_DARK:
				default:
					MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-3"));}
					MEAS_SeHead_CloseLampShutter();
					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_DARK-3"));}
					break;
				}
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			case HEAD_TYPE_COMPEASE:
				if( ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
					switch (l_wFilter) {
					case FILTER_OPEN:
// 						MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
// 						if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
						MEAS_CompEASEHead_OpenLampShutter();
						if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - EASE: FILTER_OPEN-3"));}
						break;
					case FILTER_DARK:
					default:
						MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
						if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-3"));}
// 						MEAS_CompEASEHead_CloseLampShutter();
// 						if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - EASE: FILTER_DARK-3"));}
						break;
					}
				}
				break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			case HEAD_TYPE_SR:
				{
					WORD l_wScanType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wScanType;
					if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType)
					 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_wScanType)) {
						/* SR ヘッドで透過率の場合	*/
						switch (l_wFilter) {
						case FILTER_OPEN:
							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SR.wOpticsFilterType);
							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
							if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
								MEAS_SrHead_OpenTransShutter();
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_OPEN-3"));}
							} else {
								l_bGTRLampOK = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_OPEN-3"));}
							}
							break;
						case FILTER_DARK:
						default:
							MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-3"));}
							if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
								MEAS_SrHead_CloseTransShutter();
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_DARK-3"));}
							} else {
								l_bGTRLampOK = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_DARK-3"));}
							}
							break;
						}
					}
				}
				break;
			default:
				/* 何もしません．	*/
				break;
			}
		}
		break;
	default:
		/* 何もしません．	*/
		break;
	}

/* modified 2009.11.24 hmenjo GTR ランプエラー検出修正 ---------- { ---------- */
//	if (TRUE != l_bGTRLampOK) {
/* modified 2009.11.24 hmenjo GTR ランプエラー検出修正 ----------			   */
	if ((FILTER_OPEN == l_wFilter) && (TRUE != l_bGTRLampOK)) {
		/* オープン指令のときのみ検出します．	*/
/* modified 2009.11.24 hmenjo GTR ランプエラー検出修正 ---------- } ---------- */
		/* GTR ランプエラー	*/
		if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
			AlarmIf_Set(ALID_GTR_Lamp_Error);
		}
	}
/* modified 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- } ---------- */

	return l_bRet;
}
/* added 2009.08.25 hmenjo SE ランプフィルタ制御関数 ---------- } ---------- */
/* added 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
/*
 *	ヘッド毎の測定 PGM からレンズ設定値を読出します．
 */
int CChiefView::GetMeasLens(WORD wHeadType/* = HEAD_TYPE_SR*/)
{
	int l_iLens;

	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;

	if (0 == l_pMeasProgInfo) {
		return 0;	/* レシピ未設定	*/
	}

	switch (wHeadType) {
	case HEAD_TYPE_SE:		l_iLens = l_pMeasProgInfo->ScanParams._SE.iLens;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:l_iLens = l_pMeasProgInfo->ScanParams._COMPEASE.iLens;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:	l_iLens = l_pMeasProgInfo->ScanParams._IR.iLens;	break;
	case HEAD_TYPE_MS:		l_iLens = l_pMeasProgInfo->ScanParams._MScope.iLens;	break;
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:		l_iLens = l_pMeasProgInfo->ScanParams._RS.iLens;	break;
	case HEAD_TYPE_CTA:		l_iLens = l_pMeasProgInfo->ScanParams._CA.iLens;	break;
	/* 以下はデフォルトである SR から読出します．	*/
	case HEAD_TYPE_SR:
	case HEAD_TYPE_STRESS:
	default:				l_iLens = l_pMeasProgInfo->ScanParams._SR.iLens;	break;
	}

	return l_iLens;
}
/* added 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
/* added 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- { ---------- */
/*
 *	ヘッド毎の測定 PGM からフィルタ設定値を読出します．
 */
WORD CChiefView::GetMeasFilter(WORD wHeadType/* = HEAD_TYPE_SR*/)
{
	WORD l_wFilter;

	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;

	if (0 == l_pMeasProgInfo) {
		return FILTER_OPEN;	/* レシピ未設定	*/
	}

	switch (wHeadType) {
	case HEAD_TYPE_SE:		l_wFilter = l_pMeasProgInfo->ScanParams._SE.wOpticsFilterType;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:l_wFilter = l_pMeasProgInfo->ScanParams._COMPEASE.wOpticsFilterType;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:	l_wFilter = l_pMeasProgInfo->ScanParams._IR.wOpticsFilterType;	break;
	case HEAD_TYPE_MS:		l_wFilter = l_pMeasProgInfo->ScanParams._MScope.wOpticsFilterType;	break;
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:		l_wFilter = l_pMeasProgInfo->ScanParams._RS.wOpticsFilterType;	break;
	case HEAD_TYPE_CTA:		l_wFilter = l_pMeasProgInfo->ScanParams._CA.wOpticsFilterType;	break;
	/* 以下はデフォルトである SR から読出します．	*/
	case HEAD_TYPE_SR:
	case HEAD_TYPE_STRESS:
	default:				l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;	break;
	}

/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- { ---------- */
	/*	測定　PGM　から０を引いてしまった場合、０は仕様上ありえないので、
		FILTER_OPEN　にして返します．	*/
	if (0 == l_wFilter) {
		l_wFilter = FILTER_OPEN;
	}
/* added 2009.11.30 hmenjo 測定 Seq は指定ヘッド(改)で動作 ---------- } ---------- */

	return l_wFilter;
}
/* added 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- } ---------- */
/* added 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- { ---------- */
/*
 *	リファレンスデータをチェックします．
 *		戻り値：＝0：有効期限内，かつ，データファイルが存在する
 *				＝2：有効期限外，かつ，データファイルが存在する
 *				＝1：データファイルが存在しない
 */
int CChiefView::CheckReferenceData(TCHAR* ptszMainRcpName, double dLifeTime, BOOL bRef2ndMeasure/* = FALSE*/)
{
	/* リファレンスデータ基準ファイルの存在と有効期限をチェックします．	*/
	int l_iRet = MEAS_CheckRefFileElapsedTimeOut(ptszMainRcpName, dLifeTime);
	switch (l_iRet) {
	case 0:		/* 有効期限内，かつ，基準ファイルが存在する	*/
		break;
	case 2:		/* 有効期限外，かつ，基準ファイルが存在する	*/
		break;
	case 1:		/* 基準ファイルが存在しない	*/
	default:	/* その他の異常	*/
		l_iRet = 1;		/* 基準ファイルが存在しない	*/
		break;
	}

	/* セカンドリファレンスを使う場合	*/
	if ((1 != l_iRet) && (TRUE == bRef2ndMeasure)) {
		/* 基準ファイルが存在する場合は，セカンドリファレンスのデータファイルの存在確認もします．	*/
		if ((0 == MEAS_Is2ndRefT1FileExist(ptszMainRcpName))
		 || (0 == MEAS_Is2ndRefT2FileExist(ptszMainRcpName))) {
			/* データファイル(「セカンド リファレンス T1 データ」または「セカンド リファレンス T2 データ」)が無かった	*/
			l_iRet = 1;		/* セカンドリファレンスファイルが存在しない	*/
		}
	}

	return l_iRet;
}
/* added 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- } ---------- */

/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
/*
 *	ヘッド毎のメインレシピ情報を読出します．
 *		戻り値：＝0：正常
 *				＝1：失敗(パラメタ異常)
 */
BOOL CChiefView::GetMainRcpInfHead(LPMAIN_RCP_INFO pMainRcpInfo, WORD wHeadType, LPHEAD_MAIN_RCP_INFO pHeadMainRcpInf)
{
	BOOL l_bRet = TRUE;

	switch (wHeadType) {
	case HEAD_TYPE_SR:
	case HEAD_TYPE_STRESS:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._SR.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption;
		break;
	case HEAD_TYPE_SE:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._SE.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._SE.nAutoFocusFailOption;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._COMPEASE.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._COMPEASE.nAutoFocusFailOption;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._IR.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._IR.nAutoFocusFailOption;
		break;
#else
	case HEAD_TYPE_MS:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._MScope.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._MScope.nAutoFocusFailOption;
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._RS.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._RS.nAutoFocusFailOption;
		break;
	case HEAD_TYPE_CTA:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._CA.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._CA.nAutoFocusFailOption;
		break;
	default:
		pHeadMainRcpInf->nFocus = SR_FOCUS_NONE;
		pHeadMainRcpInf->nAutoFocusFailOption = AFOCUS_FAIL_OPTION_CANCEL_RECIPE;
		l_bRet = FALSE;
		break;
	}

	return l_bRet;
}
/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
//2009.10.28 bagus 2ponit-distance --{--
LRESULT CChiefView::OnDistancePopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_DISTANCE_POPUP_END"), wparam, lparam);

	switch (wparam) {
	case 0:		// キャンセル
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_ERR, (cEventParams*) lparam);
		break;
	case 1:		// 完了
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_DONE, (cEventParams*) lparam);
		break;
	case 2:		// IDLEへ戻してダイレクトに終了
	case 3:		//
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_IDLE, (cEventParams*) lparam);
		//((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_UI1POINT_DONE, (cEventParams*) lparam);
		break;
	default:
		// 何もしません．
		break;
	}

	return 0L;
}
//2009.10.28 bagus 2ponit-distance --}--

/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
/*
 *	CTA ユニットの IL 信号をチェックします．
 *		TRUE ：オン
 *		FALSE：オフ
 */
BOOL CChiefView::IsCtaILPI()
{
	BOOL l_bRet;

	if (0 == this->m_EnableHead.bCTA) {
		l_bRet = TRUE;
	} else {
		/* CTA 有りの場合	*/
		if (0 == nexioIsCA_Interlock()) {
			l_bRet = FALSE;
		} else {
			l_bRet = TRUE;
		}
	}

	return l_bRet;
}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */

// 2009.11.12 bagus MS --{--
/*
 *	MS ユニットの IL 信号をチェックします．(下端信号２がOFFしていればNG）B接
 *		TRUE ：オン
 *		FALSE：オフ
 */
BOOL CChiefView::IsMSILPI()
{
	BOOL l_bRet;

	if (0 == this->m_EnableHead.bMS) {
		l_bRet = TRUE;
	} else {
		/* MS 有りの場合	*/
		if (0 == nexioIsMS_LowerPos2()) {
			l_bRet = FALSE;
		} else {
			l_bRet = TRUE;
		}
	}

	return l_bRet;
}
// 2009.11.12 bagus MS --}--

/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- { ---------- */
/*
 *	RS ユニットの IL 信号をチェックします．
 *		TRUE ：オン
 *		FALSE：オフ
 */
int CChiefView::CheckResistIL()
{
	const int NO_ERR = 0;
	const int ERR_LOWER = 1;
	const int ERR_CRTICAL_BOTH_ON = 2;
	const int ERR_CRTICAL_BOTH_OFF = 3;

	int l_iRet;
	BOOL l_bUpper;
	BOOL l_bLower;

	if (0 == this->m_EnableHead.bResist) {
		l_iRet = NO_ERR;
	} else {
		/* Resit 有りの場合	*/
		// Upper	Lower
		// 1		0		OK
		// 1		1		NG(Critical)
		// 0		1		NG
		// 0		0		NG(Critical)
		l_bUpper = nexioIsRS_ProbeHeadUpperPos();
		l_bLower = nexioIsRS_ProbeHeadLowerPos();
		if ( 0 != l_bUpper && 0 == l_bLower ) {
			l_iRet = NO_ERR;
		} else if ( 0 == l_bUpper && 0 != l_bLower ) {
			l_iRet = ERR_LOWER;
		} else if ( 0 != l_bUpper && 0 != l_bLower ) {
			l_iRet = ERR_CRTICAL_BOTH_ON;
		} else {
			l_iRet = ERR_CRTICAL_BOTH_OFF;
		}
	}

	return l_iRet;
}
/* added 2009.11.06 K.Matsuo RS Seq Head Position チェック ---------- } ---------- */

/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- { ---------- */
/*
 *	CTA ユニットのアラームを受取ります．
 *		LOWORD(wparam)：アラーム ID		[100 ～ 169 ＝ 70 個]
 *		HIWORD(wparam)：アラームレベル	[＝0：警告，≠0：異常]
 */
LRESULT CChiefView::OnMeasAlarmCTA(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MEAS_CTA_ALARM"), wparam, lparam);

	DWORD l_dwAlarmID = LOWORD(wparam);
	DWORD l_dwAlarmLevel = HIWORD(wparam);

	AlarmIf_Set(l_dwAlarmID + 20);

/* added 2009.11.06 hmenjo CTA CTA エラーは Abort ---------- { ---------- */
	if (0 != l_dwAlarmLevel) {
		/* 重故障は Abort にします．	*/
		if (0 != this->m_EnableHead.bCTA) {
			this->CancelSeqForAlarm();
		}
	}
/* added 2009.11.06 hmenjo CTA CTA エラーは Abort ---------- } ---------- */

	return 0L;
}
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- } ---------- */
//2009.11.03 bagus MS --{--
LRESULT CChiefView::OnMSPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_MS_POPUP_END"), wparam, lparam);

	switch (wparam) {
	case 0:		// キャンセル
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_ERR, (cEventParams*) lparam);
		break;
	case 1:		// 完了
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_DONE, (cEventParams*) lparam);
		break;
	case 2:		// IDLEへ戻してダイレクトに終了
	case 3:		//
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_IDLE, (cEventParams*) lparam);
		break;
	default:
		// 何もしません．
		break;
	}
	return 0L;
}
//2009.11.03 bagus MS --}--

/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- { ---------- */
/*
 *	CTA リセット処理
 *		引数  ：DWORD dwMode
 *					0：CTA アラーム発生中を取得
 *					1：CTA アラーム発生中をセット
 *					以外：CTA アラーム発生中をクリアし，CTA ユニットをリセット
 *		戻り値：
 *					0：CTA アラーム発生中ではありません
 *					以外：CTA アラーム発生中です
 */
int CChiefView::CtaReset(DWORD dwMode/* = 0*/)
{
	static BOOL ls_bCTAAlarm = FALSE;

	if (0 != this->m_EnableHead.bCTA) {
		switch (dwMode) {
		case 0:		/* CTA アラーム発生中を取得	*/
			/* このモードに処理はありません．	*/
			break;
		case 1:		/* CTA アラーム発生中をセット	*/
			ls_bCTAAlarm = TRUE;
			break;
		default:	/* CTA アラーム発生中をクリアし，CTA ユニットをリセット	*/
			if (0 != ls_bCTAAlarm) {
				long l_lCtaStatus;
				BOOL l_bRslt = MEAS_CtaGetStatus(&l_lCtaStatus);
				if (0 != l_bRslt) {
					/* 取得が正常完了	*/
					int l_iHead		= LOBYTE(LOWORD(l_lCtaStatus));	/* ヘッド	*/
					int l_iStatus1	= HIBYTE(LOWORD(l_lCtaStatus));	/* 状態１	*/
					int l_iStatus2	= LOBYTE(HIWORD(l_lCtaStatus));	/* 状態２	*/
					int l_iReserve	= HIBYTE(HIWORD(l_lCtaStatus));	/* (未使用)	*/
					if (((1 != l_iHead) && (2 != l_iHead))
					 || (2 != l_iStatus1)
					 || (1 != l_iStatus2)) {
						/*	ヘッドが「原点位置：１」と「待機位置：２」以外の場合，か
							状態１が「待機中：２」以外の場合，か
							状態２が「１：エラー無し」」以外の場合	*/
						l_bRslt = MEAS_CtaForceEnd();
						if (0 != l_bRslt) {
							ls_bCTAAlarm = FALSE;
						}
					} else {
						ls_bCTAAlarm = FALSE;
					}
				}
			}
			break;
		}
	}

	int l_iRet = (0 == ls_bCTAAlarm)? 0 : 1;

	return l_iRet;
}
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- } ---------- */

/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
void CChiefView::PostTransiEvent(CWinThread* pcThread, UINT uiMsg, TCHAR* ptszMsg, BYTE byTransiKind, WPARAM wParam, LPARAM lParam)
{
	CString l_strMsg;
	l_strMsg.Format(_T("Failed to PostThreadMessage(%s), wParam = 0x%08X, lParam = 0x%08X"), ptszMsg, wParam, lParam);
	BOOL l_bRslt = FALSE;
	for (int i = 0 ; i < CHIEF_TREVENT_RETRY_COUNT; i++) {
		l_bRslt = pcThread->PostThreadMessage(uiMsg, wParam, lParam);
		if (FALSE != l_bRslt) {
			break;
		} else {
			/* 失敗ログ	*/
			this->LogChief((LPTSTR) ((LPCTSTR) l_strMsg));
		}
		::Sleep(CHIEF_TREVENT_RETRY_INTERVAL);	/* リトライ間隔	*/
	}
	if (FALSE == l_bRslt) {
		/* リトライアウト	*/
		this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_BOTH, CHRANFY_NOTIFY_ON), MAKEWORD(byTransiKind, CHRAMSG_YESNO)), MAKELPARAM(39, ALID_ChiefTransi_Failed_PostThreadMessage));
	}
}
extern UINT WM_CHIF_TRAF_SETEVENT;		// 3：CChiefTransiAF
extern UINT WM_CHIF_TRDSK_SETEVENT;		// 4：CChiefTransiDeskew
extern UINT WM_CHIF_TRMAS_SETEVENT;		// 5：CChiefTransiMaster
extern UINT WM_CHIF_TRSEQ_SETEVENT;		// 6：CChiefTransiSeq
extern UINT WM_CHIF_TR1P_SETEVENT;		// 7：CChiefTransiSr1Point
extern UINT WM_CHIF_TRREF_SETEVENT;		// 8：CChiefTransiSrRefer
extern UINT WM_CHIF_TRSTRS_SETEVENT;	// 9：CChiefTransiStress
BOOL CChiefView::IsBusyTransi(BYTE byTransiKindDst, BYTE byTransiKindSrc)
{
	BOOL l_bBusy = FALSE;

	bool l_bIdle = true;
	DWORD l_dwTimeSta = ::GetTickCount();
	do {
		l_bIdle = true;
		switch (byTransiKindDst) {
		case 3:		l_bIdle = ((CChiefTransiAF*)		m_pcChiefTransiAF)->IsIdle();		break;
		case 4:		l_bIdle = ((CChiefTransiDeskew*)	m_pcChiefTransiDeskew)->IsIdle();	break;
		case 5:		l_bIdle = ((CChiefTransiMaster*)	m_pcChiefTransiMaster)->IsIdle();	break;
		case 6:		l_bIdle = ((CChiefTransiSeq*)		m_pcChiefTransiSeq)->IsIdle();		break;
		case 7:		l_bIdle = ((CChiefTransiSr1Point*)	m_pcChiefTransiSr1Point)->IsIdle();	break;
		case 8:		l_bIdle = ((CChiefTransiSrRefer*)	m_pcChiefTransiSrRefer)->IsIdle();	break;
		case 9:		l_bIdle = ((CChiefTransiStress*)	m_pcChiefTransiStress)->IsIdle();	break;
		}
		if (false != l_bIdle) {
			break;
		}
		if (CHIEF_CHECK_BUSY_TIME < (::GetTickCount() - l_dwTimeSta)) {
			/* タイムアウト：一定時間経過してもビジーでした．	*/
			l_bBusy = TRUE;
			/* 正常シーケンスでウェイト無しでアイドルに遷移する状態なら強制アイドルへ	*/
			BOOL l_bIdleReq = FALSE;
			switch (byTransiKindDst) {
			case 4:	switch (((CChiefTransiDeskew*) m_pcChiefTransiDeskew)->GetCurrentState()) {
					case ST_DSKW_START:		m_pcChiefTransiDeskew->PostThreadMessage(WM_CHIF_TRDSK_SETEVENT,	(WPARAM) EV_DSKW_IDLE,			0);	l_bIdleReq = TRUE;	break;
					case ST_DSKW_AUTO_OK:	m_pcChiefTransiDeskew->PostThreadMessage(WM_CHIF_TRDSK_SETEVENT,	(WPARAM) EV_DSKW_IDLE,			0);	l_bIdleReq = TRUE;	break;
					case ST_DSKW_MANU_OK:	m_pcChiefTransiDeskew->PostThreadMessage(WM_CHIF_TRDSK_SETEVENT,	(WPARAM) EV_DSKW_IDLE,			0);	l_bIdleReq = TRUE;	break;
					}
					break;
			case 7:	switch (((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->GetCurrentState()) {
					case ST_SR1P_ANA_DONE:	m_pcChiefTransiSr1Point->PostThreadMessage(WM_CHIF_TR1P_SETEVENT,	(WPARAM) EV_SR1P_IDLE,			0);	l_bIdleReq = TRUE;	break;
					}
					break;
			case 8:	switch (((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->GetCurrentState()) {
					case ST_SREF_N_MOVE:	m_pcChiefTransiSrRefer->PostThreadMessage(WM_CHIF_TRREF_SETEVENT,	(WPARAM) EV_SREF_IDLE, 			0);	l_bIdleReq = TRUE;	break;
					case ST_SREF_D_MEAS:	m_pcChiefTransiSrRefer->PostThreadMessage(WM_CHIF_TRREF_SETEVENT,	(WPARAM) EV_SREF_D_MEAS_DONE,	0);	l_bIdleReq = TRUE;	break;
					}
					break;
			case 3:
			case 5:
			case 6:
			case 9:
			default:
				break;
			}
			if (FALSE != l_bIdleReq) {
				l_bBusy = FALSE;
				l_dwTimeSta = ::GetTickCount();
				do {
					l_bIdle = true;
					switch (byTransiKindDst) {
					case 3:		l_bIdle = ((CChiefTransiAF*)		m_pcChiefTransiAF)->IsIdle();		break;
					case 4:		l_bIdle = ((CChiefTransiDeskew*)	m_pcChiefTransiDeskew)->IsIdle();	break;
					case 5:		l_bIdle = ((CChiefTransiMaster*)	m_pcChiefTransiMaster)->IsIdle();	break;
					case 6:		l_bIdle = ((CChiefTransiSeq*)		m_pcChiefTransiSeq)->IsIdle();		break;
					case 7:		l_bIdle = ((CChiefTransiSr1Point*)	m_pcChiefTransiSr1Point)->IsIdle();	break;
					case 8:		l_bIdle = ((CChiefTransiSrRefer*)	m_pcChiefTransiSrRefer)->IsIdle();	break;
					case 9:		l_bIdle = ((CChiefTransiStress*)	m_pcChiefTransiStress)->IsIdle();	break;
					}
					if (false != l_bIdle) {
						break;
					}
					if (CHIEF_CHECK_BUSY_TIME < (::GetTickCount() - l_dwTimeSta)) {
						/* タイムアウト：一定時間経過してもビジーでした．	*/
						l_bBusy = TRUE;
						break;
					} else {
						::Sleep(CHIEF_CHECK_BUSY_INTERVAL);
					}
				} while (false == l_bIdle);
			}
			break;
		} else {
			::Sleep(CHIEF_CHECK_BUSY_INTERVAL);
		}
	} while (false == l_bIdle);

	if (FALSE != l_bBusy) {
		/* アラーム	*/
		this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_BOTH, CHRANFY_NOTIFY_ON), MAKEWORD(byTransiKindSrc, CHRAMSG_YESNO)), MAKELPARAM(40, ALID_ChiefTransi_BusyTransition));
	}

	return l_bBusy;
}
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//
//	CompEASEの状態ステータスを受取ります．
//
LRESULT CChiefView::OnMeasCompEASEStatus(WPARAM wparam, LPARAM lparam)
{
	DWORD l_dwStatus = (wparam);
	DWORD l_dwALID;

	switch ( l_dwStatus ) {
	case EASE_STATUS_UNKNOWN:
		l_dwALID = 190;
		break;
	case EASE_STATUS_COMM_ESTABLISH:
		l_dwALID = 191;
		break;
	case EASE_STATUS_COMM_ERROR:
		l_dwALID = 192;
		break;
	case EASE_STATUS_EXECUTE_CMD_ERROR:
		l_dwALID = 193;
		break;
	case EASE_STATUS_EXECUTE_CMD_TIMEOUT:
		l_dwALID = 194;
		break;
	case EASE_STATUS_HARDWARE_NOT_INIT:
		l_dwALID = 195;
		break;
	case EASE_STATUS_HARDWARE_NOT_CALIB:
		l_dwALID = 196;
		break;
	// アラーム追加時、OnMeasCompEASEError()関数で使用しているアラーム番号に注意してください
	default:
		return 0L;
	}

	LogChief_WinMsg(_T("WM_MEAS_COMPEASE_STATUS"), wparam, lparam);

	AlarmIf_Set(l_dwALID);

	return 0L;
}

//
//	CompEASEのエラー情報を受取ります．
//
LRESULT CChiefView::OnMeasCompEASEError(WPARAM wparam, LPARAM lparam)
{
	DWORD l_dwStatus = (wparam);
	DWORD l_dwALID;

	switch ( l_dwStatus ) {
	case EASE_ERROR_RUN_RECIPE_NOT_FOUND:
		l_dwALID = 197;
		break;
	// アラーム追加時、OnMeasCompEASEStatus()関数で使用しているアラーム番号に注意してください
	default:
		// ASSERT(FALSE);
		return 0L;
	}

	LogChief_WinMsg(_T("WM_MEAS_COMPEASE_ERROR"), wparam, lparam);

//	AlarmIf_Set(l_dwALID);	// 2013.02.25 Procon入替えできないため、暫定コメントアウト

	return 0L;
}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
