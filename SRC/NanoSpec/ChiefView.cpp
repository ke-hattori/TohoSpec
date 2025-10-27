// ChiefView.cpp : Cve[V t@C
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChifRcvMailThread.h"
#include "ChiefView.h"
#include <NEXIF.HXX>
#include <PIFCOMM.HXX>
//#include <NEXIO.HXX>		hmenjo gp֎~
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
/* added 2009.07.30 hmenjo XgX@\ǉ(2) ---------- { ---------- */
#include "ChifTransiStress.h"
/* added 2009.07.30 hmenjo XgX@\ǉ(2) ---------- } ---------- */
/* added 2009.08.07 hmenjo XgX@\ǉ(35) ---------- { ---------- */
#define	CHIEF_PFUNCS_MAS
#include "ChiefPFuncs.h"
/* added 2009.08.07 hmenjo XgX@\ǉ(35) ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	ChiefExport.h ɏo悢`
 */
#if 0
#define	CHIEF_INI	_T(".\\Chief.ini")	// Chief pݒt@C
#define	CHIEF_DLG_CAPTION	_T("Chief Monitor")	// Chief _CAÕLvV
// eA[񍐗p`
#endif

/*
 *	^C}`
 */
enum CHIEF_TIMER_ID {
	ID_TIMER_HIDEDLG = 201,		// Chief _CAO\^C} ID
	ID_TIMER_1S_PERIOD,			// 1s	 ^C} ID
	ID_TIMER_100MS_PERIOD,		// 100ms ^C} ID
	ID_TIMER_50MS_PERIOD,		// 50ms  ^C} ID
	ID_TIMER_10MS_PERIOD,		// 10ms  ^C} ID
// 2009.02.05 K.Matsuo delete -->
//	ID_TIMER_TRACEDATA,			// g[Xf[^񍐃^C} ID
// 2009.02.05 K.Matsuo delete <--
	ID_TIMER_EQPWOFF,			// udItVbg^C} ID
/* added 2009.07.31 hmenjo XgX@\ǉ(3) ---------- { ---------- */
	ID_TIMER_HEPASTOP,			/* HEPA ~҂^C} ID	*/
/* added 2009.07.31 hmenjo XgX@\ǉ(3) ---------- } ---------- */
};
#define	ID_MMTIMER_5MS_PERIOD	0		// 5ms	 ^C}(}`fBA^C}) ID
#define	TIMER_1S_PERIOD			1000	// 1s	 ^C}l[ms]
#define	TIMER_100MS_PERIOD		100		// 100ms ^C}l[ms]

//----- Kawashima 2008.12.01 Debug ----->
#define	TIMER_50MS_PERIOD		100		// 50ms  ^C}l[ms]
#define	TIMER_20MS_PERIOD		100		// 20ms  ^C}l[ms]
#define	TIMER_10MS_PERIOD		100		// 10ms  ^C}l[ms]
#define	TIMER_5MS_PERIOD		100		// 5ms	^C}l[ms](}`fBA^C}ł̂ݎg͂)

//#define	TIMER_50MS_PERIOD		50		// 50ms  ^C}l[ms]
//#define	TIMER_20MS_PERIOD		20		// 20ms  ^C}l[ms]
//#define	TIMER_10MS_PERIOD		10		// 10ms  ^C}l[ms]
//#define	TIMER_5MS_PERIOD		5		// 5ms	^C}l[ms](}`fBA^C}ł̂ݎg͂)
//<--------------------------------

/*
 *	[JO[o`
 */
/* modified 2009.12.09 hmenjo  Seq ͎wwbhœ(ǉ) ---------- { ---------- */
//CChiefView*	g_pcChiefView;	// static pNX|C^
/* modified 2009.12.09 hmenjo  Seq ͎wwbhœ(ǉ) ----------				*/
CChiefView*	g_pcChiefView = 0;	/* static pNX|C^	*/
/* modified 2009.12.09 hmenjo  Seq ͎wwbhœ(ǉ) ---------- } ---------- */

// ԑp֐Q
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
// CChiefView _CAO


CChiefView::CChiefView()
	: CFormView(CChiefView::IDD)
{
	//{{AFX_DATA_INIT(CChiefView)
		//  - ClassWizard ͂̈ʒuɃ}bsOp̃}Nǉ܂͍폜܂B
	//}}AFX_DATA_INIT

	TRACE(_T("CChiefView::CChiefView() \n"));

	m_pcMainFrame = (CFrameWnd*) AfxGetApp()->m_pMainWnd;						// CMainFrame ̃|C^ۑ
	m_pcNanoSpecDoc = ((CMainFrame*) m_pcMainFrame)->m_pDoc;		// CNanoSpecDoc ̃|C^ۑ

	LogChief(_T("Started  Chief Dialog."));

/* added 2009.12.09 hmenjo  Seq ͎wwbhœ(ǉ) ---------- { ---------- */
	m_pcChiefTransiAF = 0;
	m_pcChiefTransiDeskew = 0;
	m_pcChiefTransiMaster = 0;
	m_pcChiefTransiSeq = 0;
	m_pcChiefTransiSr1Point = 0;
	m_pcChiefTransiSrRefer = 0;
	m_pcChiefTransiStress = 0;
/* added 2009.12.09 hmenjo  Seq ͎wwbhœ(ǉ) ---------- } ---------- */
	g_pcChiefView = this;		// static p CChiefView NX|C^
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	m_lPreAFafterPreMove = 0;
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */

	// Vsp擾
	RecipesMalloc(TRUE);

//	BOOL l_bRet = Create(CChiefView::IDD, 0);
//	// _CAOŏɃ^XNo[ɓ悤ɂD	// KvɂȂ܂(2008.12.15 ̃T[o\[X)
//	ModifyStyleEx(0, WS_EX_APPWINDOW, SWP_DRAWFRAME);	//svǎcĂ܂D
}

void CChiefView::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CChiefView)
		//  - ClassWizard ͂̈ʒuɃ}bsOp̃}Nǉ܂͍폜܂B
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
/* added 2009.08.05 hmenjo XgX@\ǉ(25) ---------- { ---------- */
	ON_MESSAGE(WM_MEAS_LINE_END, OnMeasLineEnd)
	ON_MESSAGE(WM_DATA_LINE_END, OnDataLineEnd)
	ON_MESSAGE(WM_DATA_STRS_MEAS_END, OnDataStressMeasEnd)
/* added 2009.08.05 hmenjo XgX@\ǉ(25) ---------- } ---------- */
//2009.10.29 bagus 2point-distance --{--
	ON_MESSAGE(WM_DISP_DISTANCE_POPUP_END, OnDistancePopupEnd)
//2009.10.29 bagus 2point-distance --}--
/* added 2009.10.30 hmenjo CTA A[nh ---------- { ---------- */
	ON_MESSAGE(WM_MEAS_CTA_ALARM, OnMeasAlarmCTA)
/* added 2009.10.30 hmenjo CTA A[nh ---------- } ---------- */
//2009.11.03 bagus MS --{--
	ON_MESSAGE(WM_DISP_MS_POPUP_END, OnMSPopupEnd)
//2009.11.03 bagus MS --}--
// 2013.02.01 bagus CompleteEASEwbhǉ -->
	ON_MESSAGE(WM_MEAS_COMPEASE_STATUS, OnMeasCompEASEStatus)
	ON_MESSAGE(WM_MEAS_COMPEASE_ERROR, OnMeasCompEASEError)
// 2013.02.01 bagus CompleteEASEwbhǉ <--
// 2013.11.07 Bagus Add (TohoSpecΉ) -->
	ON_MESSAGE(WM_DISP_CONFIRM_POPUP_END, OnConfirmPopupEnd)
// 2013.11.07 Bagus Add (TohoSpecΉ) <--
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefView bZ[W nh

void CChiefView::PostNcDestroy()
{
	// E -------------------------------------------------------------
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

	// TODO: ̈ʒuɏ̕⑫ǉĂ

	return 0;  // Rg[ɃtH[JXݒ肵ȂƂA߂l TRUE ƂȂ܂
				  // O: OCX vpeB y[W̖߂l FALSE ƂȂ܂
}

/*
 *	}`fBA^C} CallBack nh
 *			"WM_TIMER"|XgĂ񂾂ǁEEE(₱Ȃ̂ŁEEE)
 *			"WinMM.lib" NȂĂgǁCǂŃNĂ܂HH
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
			// fobOpł
			static int l_iTmp = 0;
			l_iTmp++;
			if ((10000 / TIMER_5MS_PERIOD) < l_iTmp) {
				l_iTmp = 0;
			}
		}
#endif
		// RR 5ms (100ms ɂꂽ by Kawashima 2008.12.01)-----------------------------------------------
		// 쒆ُ̈͂`FbN
		if ((PROCESS_INIT != g_pcChiefView->ProcStatusGet()) && (0 != lg_uiRunFlag)) {
			g_pcChiefView->CheckDIO_Running();
		}
		// Nextra  DI 擾
		g_pcChiefView->GetDiInfo(&(g_pcChiefView->m_DiInfo));

#if 0		// (100ms ɂꂽ by Kawashima 2008.12.01 ̂ō폜Ă܂)
		// RR 10ms ----------------------------------------------
		if (TIMER_10MS_PERIOD / TIMER_5MS_PERIOD < lg_uiMMTimer10msCounter++) {
			lg_uiMMTimer10msCounter = 0;
			// ̉珈ǉĂ
#endif

			// DIO ĎJn^C}(A[ñEBhEオ܂ł̑҂)
			if (0 != lg_uiRunFlagCount) {
				lg_uiRunFlagCount--;
				if (0 == lg_uiRunFlagCount) {
					lg_uiRunFlag = TRUE;
				}
			}
#if 0		// (100ms ɂꂽ by Kawashima 2008.12.01 ̂ō폜Ă܂)
		}
#endif
#if 0		// (100ms ɂꂽ by Kawashima 2008.12.01 ̂ō폜Ă܂)
		// RR 20ms ----------------------------------------------
		if (TIMER_20MS_PERIOD / TIMER_5MS_PERIOD < lg_uiMMTimer20msCounter++) {
			lg_uiMMTimer20msCounter = 0;
			// ̉珈ǉĂ
		}
		// RR 50ms ----------------------------------------------
		if (TIMER_50MS_PERIOD / TIMER_5MS_PERIOD < lg_uiMMTimer50msCounter++) {
			lg_uiMMTimer50msCounter = 0;
			// ̉珈ǉĂ
		}
#endif
	}
}

/*
 *	MFC ̃^C} bZ[Wnh
 */
void CChiefView::OnTimer(UINT nIDEvent)
{
	// TODO: ̈ʒuɃbZ[W nhp̃R[hǉ邩܂̓ftHg̏ĂяoĂ

	switch (nIDEvent) {
/* added 2009.07.31 hmenjo XgX@\ǉ(3) ---------- { ---------- */
	case ID_TIMER_HEPASTOP:
		if (0 == this->KillTimer(ID_TIMER_HEPASTOP)) {
			/* ^C}Cxg폜s	*/
			this->LogChief(CHIEF_REP_ALARM_MSGTEXT[38]);
			this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(38, 0));
		}
		((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_HEPA_STOP, (cEventParams*) EV_STRS_HEPA_STOP);
		break;
/* added 2009.07.31 hmenjo XgX@\ǉ(3) ---------- } ---------- */
	case ID_TIMER_1S_PERIOD:		// 1s ^C}
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_1S_PERIOD);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		PostMessage(WM_CHIF_HEARTBEAT, 0, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->SetTimer(ID_TIMER_1S_PERIOD, TIMER_1S_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_100MS_PERIOD:		// 100ms ^C}
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_100MS_PERIOD);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		PostMessage(WM_CHIF_UPDATESTATESDLG, 0, 0);
		IsStageIdle();
		// ud͂̃ItĎ
		{
			if (0 != m_bCheckEqPowerOFF) {
				if (0 == m_DiInfo.bEQPower) {
					// ud͂It܂D
					nexioEquipmentPowerOFF(FALSE);	// udIto͂It܂D
					if (2 != m_bCheckEqPowerOFF) {
						if (0 == KillTimer(ID_TIMER_EQPWOFF)) {
							// ^C}Cxg폜s
							LogChief(CHIEF_REP_ALARM_MSGTEXT[35]);
							PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(35, 0));
						}
					}
					m_bCheckEqPowerOFF = 0;
				}
			}
		}
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
		/* AF seq ACh`FbN	*/
		if (2 == (m_lPreAFafterPreMove & 0x7fffffff)) {
			if (true == ((CChiefTransiAF*) m_pcChiefTransiAF)->IsIdle()) {
				/* AF seq AChłD	*/
				if (0 == (m_lPreAFafterPreMove & 0x80000000)) {
					/* AF 	*/
					this->LogChief(_T("PreAF - AF Success. (m_lPreAFafterPreMove = 3) EV_SEQ_POINT_MOVE_DONE"));
					m_lPreAFafterPreMove = 3;
				} else {
					/* AF s	*/
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
	case ID_TIMER_50MS_PERIOD:		// 50ms ^C}(100ms ɂꂽ by Kawashima 2008.12.01)
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_50MS_PERIOD);
		this->SetTimer(ID_TIMER_50MS_PERIOD, TIMER_50MS_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_10MS_PERIOD:		// 10ms ^C}(100ms ɂꂽ by Kawashima 2008.12.01)
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->KillTimer(ID_TIMER_10MS_PERIOD);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		PostMessage(WM_CHIF_PIFDIREFRESH, 0, 0);
		PostMessage(WM_CHIF_EQRUNCHECK, 0, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- { ---------- */
		this->SetTimer(ID_TIMER_10MS_PERIOD, TIMER_10MS_PERIOD, 0);
/* added 2013.06.04 hmenjo Chief KillTimer() ---------- } ---------- */
		break;
	case ID_TIMER_HIDEDLG:			// _CAO\^C}
		if (0 == KillTimer(ID_TIMER_HIDEDLG)) {
			// ^C}Cxg폜s
			LogChief(CHIEF_REP_ALARM_MSGTEXT[3]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(3, 0));
		}
		switch (m_DlgShowSW.dwShowSWprc) {
		case 0:
			GetParentFrame()->ShowWindow(SW_SHOWMINIMIZED);	// UŏĂ
			m_DlgShowSW.dwShowSWprc = 1;
			if (ID_TIMER_HIDEDLG != SetTimer(ID_TIMER_HIDEDLG, 500, 0)) {
				// ^C}Ns
				LogChief(CHIEF_REP_ALARM_MSGTEXT[2]);
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(2, 0));
			}
			break;
		case 1:
			GetParentFrame()->ShowWindow(SW_SHOWNORMAL);	// \
			m_DlgShowSW.dwShowSWprc = 2;
			if (ID_TIMER_HIDEDLG != SetTimer(ID_TIMER_HIDEDLG, m_DlgShowSW.dwDelay * 1000, 0)) {
				// ^C}Ns
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
//	case ID_TIMER_TRACEDATA:		// g[Xf[^莞(M)^C}
//		if (0 == KillTimer(ID_TIMER_TRACEDATA)) {
//			// ^C}Cxg폜s
//			LogChief(CHIEF_REP_ALARM_MSGTEXT[1]);
//			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(1, 0));
//			//
//		}
//		// g[Xf[^莞M^C}N
//		if (0 != (m_uiTraceDataPeriod = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetTraceDataPeriod())) {
//			// g[Xf[^(M)
//			OnSendTraceData(0, 0);
//			if (0 == StartTimerTraceData(m_uiTraceDataPeriod)) {
//				// ^C}Ns
//				m_bTraceDataTimer = FALSE;
//				LogChief(CHIEF_REP_ALARM_MSGTEXT[0]);
//				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(0, 0));
//			}
//		} else {
//			// ݒ肪OɂȂĂ̂Œ莞~
//			m_bTraceDataTimer = FALSE;
//		}
//		break;
// 2009.02.05 K.Matsuo delete <--
	case ID_TIMER_EQPWOFF:		// udIt͊Ď^C}
		// ud͂Ȃ̂ŁCudItF
//		nexioEquipmentPowerOFF(FALSE);		ŜߑudIt͉܂D
//		m_bCheckEqPowerOFF = 0;			ĎtOIt܂D
		m_bCheckEqPowerOFF = 2;		// ^CAEgςɂ܂D
		// A[Zbg
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EquipmentPowerOffControlError));
		if (0 == KillTimer(ID_TIMER_EQPWOFF)) {
			// ^C}Cxg폜s
			LogChief(CHIEF_REP_ALARM_MSGTEXT[35]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(35, 0));
		}
		break;
	}

	CFormView::OnTimer(nIDEvent);
}

/*
 *	Chief 쒆\ bZ[Wnh
 */
LRESULT CChiefView::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeatCnt++;
	CString l_strTmp;
	l_strTmp.Format("%d", m_dwHeartBeatCnt);
	SetDlgItemText(IDC_STATIC_HEART, l_strTmp);

	// eXbhp HeartBeat pX
	((CChiefTransiAF*) m_pcChiefTransiAF)->HeartBeatPulse();
	((CChiefTransiDeskew*) m_pcChiefTransiDeskew)->HeartBeatPulse();
	((CChiefTransiMaster*) m_pcChiefTransiMaster)->HeartBeatPulse();
	((CChiefTransiSeq*) m_pcChiefTransiSeq)->HeartBeatPulse();
	((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->HeartBeatPulse();
	((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->HeartBeatPulse();

// 2009.02.05 K.Matsuo delete -->
//	// g[Xf[^莞M^C}N
//	if (0 == m_bTraceDataTimer) {
//		// 莞~ĂƂ̂݃`FbN܂D
//		if (0 != (m_uiTraceDataPeriod = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetTraceDataPeriod())) {
//			if (0 == StartTimerTraceData(m_uiTraceDataPeriod)) {
//				// ^C}Ns
//				LogChief(CHIEF_REP_ALARM_MSGTEXT[0]);
//				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(0, 0));
//			} else {
//				// ^C}N
//				m_bTraceDataTimer = TRUE;
//			}
//		}
//	}
// 2009.02.05 K.Matsuo delete <--

	return 0L;
}

/*
 *	LZ(uCancelv{^CuESCvL[) bZ[Wnh
 */
void CChiefView::OnCancel()
{
	// TODO: ̈ʒuɓʂȌ㏈ǉĂB

	// u~v{^uESCvL[Ń_CAOIȂ悤ɃRgɂ
//	CDialog::OnCancel();
}

/*
 *	uHidev{^(\{^) bZ[Wnh
 */
void CChiefView::OnBtnHide()
{
	// TODO: ̈ʒuɃRg[ʒmnhp̃R[hǉĂ

	KillTimer(ID_TIMER_HIDEDLG);	// _CAO\fBC^C}Ă
	m_DlgShowSW.dwType = 0;
	GetParentFrame()->ShowWindow(SW_HIDE);
}

/*
 *	uTopv{^(̓`FbN{bNX) bZ[Wnh
 */
void CChiefView::OnChkTop()
{
	// TODO: ̈ʒuɃRg[ʒmnhp̃R[hǉĂ

	if (0 == ((CButton*) GetDlgItem(IDC_CHK_TOP))->GetCheck()) {
		// őOʂ
		GetParentFrame()->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	} else {
		// őOʂɂ
		GetParentFrame()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

/*
 *	Nextra ̉()bZ[WWnh
 */
LRESULT CChiefView::OnNextraResponse(WPARAM wparam, LPARAM lparam)
{
	DWORD	l_dwDeviceCode = (DWORD) wparam;
	int		l_iResult = (int) ((0 == lparam)? 1 : 0);

	LogChief_WinMsg(_T("WM_NEX_RESPONSE"), wparam, lparam);

	if ((0 != m_bVacuumOnFromDisp) && (nexVacuumOn == l_dwDeviceCode)) {
		// oL[ ON 䂪ʂ̎w߂̏ꍇ
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_iResult, 0);
		m_bVacuumOnFromDisp = FALSE;
	} else if ((0 != m_bVacuumOffFromDisp) && (nexVacuumOff == l_dwDeviceCode)) {
		// oL[ OFF 䂪ʂ̎w߂̏ꍇ
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_iResult, 0);
		m_bVacuumOffFromDisp = FALSE;
	} else {
/* modified 2009.08.06 hmenjo XgX@\ǉ(28) ---------- { ---------- */
//		if ((nexMoveToUpper == l_dwDeviceCode) || (nexMoveToLower == l_dwDeviceCode)) {
/* modified 2009.08.06 hmenjo XgX@\ǉ(28) ----------			   */
		if ((nexMoveToUpper == l_dwDeviceCode) || (nexMoveToLower == l_dwDeviceCode) || (nexMoveToAlignment == l_dwDeviceCode)) {
/* modified 2009.08.06 hmenjo XgX@\ǉ(28) ---------- } ---------- */
			ActuateFlagsSet(ACTUATE_PIN, FALSE);			// 쒆tO(s)It
		}
		if ((nexOpenShutter == l_dwDeviceCode) || (nexCloseShutter == l_dwDeviceCode)) {
			ActuateFlagsSet(ACTUATE_SHUTTER, FALSE);		// 쒆tO(Vb^)It
		}
		if ((nexLoad == l_dwDeviceCode) || (nexUnload == l_dwDeviceCode)) {
			ActuateFlagsSet(ACTUATE_PIN, FALSE);			// 쒆tO(s)It
			ActuateFlagsSet(ACTUATE_WORKGUIDE, FALSE);	// 쒆tO([NKCh)It
		}
		switch (l_dwDeviceCode) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- { ---------- */
//		case nexLoad:			PifComm_AlignmentLoadResultReport(l_iResult);	break;	// ACg Load
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ----------			  */
		case nexLoad:																	/* ACg Load	*/
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
					/* ُ	*/
					l_iEvent = EV_STRS_CANCEL;
					l_iErrCode = TR_STRS_PINMOVE_FAIL;
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
				} else {
					/* 	*/
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_PIN_DONE, (cEventParams*) EV_STRS_PIN_DONE);
				}
			}
// 2010.02.16 K.Matsuo Bug fix -->
			break;
// 2010.02.16 K.Matsuo Bug fix <--
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- } ---------- */
		case nexUnload:			PifComm_AlignmentUnloadResultReport(l_iResult);	break;	// ACg Unload
		case nexMoveToUpper:	PifComm_PinUpResultReport(l_iResult);			break;	// s㏸[ړ
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- { ---------- */
///* deleted 2009.08.03 hmenjo XgX@\ǉ(17) ---------- { ---------- */
////		case nexMoveToLower:	PifComm_PinDownResultReport(l_iResult);			break;	// s~[ړ
///* deleted 2009.08.03 hmenjo XgX@\ǉ(17) ---------- } ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ----------			  */
		case nexMoveToLower:	PifComm_PinDownResultReport(l_iResult);			break;	// s~[ړ
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- } ---------- */
		case nexOpenShutter:	PifComm_ShutterOpenResultReport(l_iResult);		break;	// Vb^ Open
		case nexCloseShutter:	PifComm_ShutterCloseResultReport(l_iResult);	break;	// Vb^ Close
		case nexVacuumOn:		PifComm_VaccumOnResultReport(l_iResult);		break;	// oL[ ON
		case nexVacuumOff:		PifComm_VaccumOffResultReport(l_iResult);		break;	// oL[ OFF
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- { ---------- */
///* added 2009.08.03 hmenjo XgX@\ǉ(17) ---------- { ---------- */
//		case nexMoveToLower:															/* s~[ړ	*/
//		case nexMoveToAlignment:														/* sACgʒuړ	*/
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
//					/* ُ	*/
//					l_iEvent = EV_STRS_CANCEL;
//					l_iErrCode = TR_STRS_PINMOVE_FAIL;
//// 2009.08.22 K.Matsuo -->
//					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
//// 2009.08.22 K.Matsuo <--
//				} else {
//					/* 	*/
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
///* added 2009.08.03 hmenjo XgX@\ǉ(17) ---------- } ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ----------			  */
		case nexMoveToAlignment:														/* sACgʒuړ	*/
			if (0 == m_dwPinMoveState) {
				if (nexMoveToLower == l_dwDeviceCode) {
					PifComm_PinDownResultReport(l_iResult);
				}
			} else {
				m_dwPinMoveState = 0;
				int l_iEvent;
				int l_iErrCode;
				if (0 != l_iResult) {
					/* ُ	*/
					l_iEvent = EV_STRS_CANCEL;
					l_iErrCode = TR_STRS_PINMOVE_FAIL;
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(l_iEvent, (cEventParams*) l_iErrCode);
				} else {
					/* 	*/
					((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_PIN_DONE, (cEventParams*) EV_STRS_PIN_DONE);
				}
			}
			break;
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- } ---------- */
		case nexInitialize:																// (HHHۗ)
		case nexReturnToOrigin:															// s_AC[NKChJ
/* deleted 2009.08.03 hmenjo XgX@\ǉ(17) ---------- { ---------- */
//		case nexMoveToAlignment:														// sACgʒuړ
/* deleted 2009.08.03 hmenjo XgX@\ǉ(17) ---------- } ---------- */
		case nexOpenWorkGuide:															// [NKChJ
		case nexCloseWorkGuide:															// [NKCh
		default:
			/*	(2008.09.03)C̎w߂́CChief ͔s܂D
				āCbZ[WĂCĂ܂D	*/
			LogChief(_T("Last Nextra Response was invalid."));
			break;
		}
	}

	return 0L;
}

/*
 *	Pif R}hM\ bZ[Wnh
 *		擪̃R}hR[hS_CAOɕ\܂D
 */
LRESULT CChiefView::OnDispRecvPifCmd(WPARAM wparam, LPARAM lparam)
{
	CString	l_strTemp;

	l_strTemp = ((CChiefRcvMailThread*) m_pcChiefRcvMailThread)->GetPifRcvCmd();

	SetDlgItemText(IDC_STATIC_PIFRECV, l_strTemp);

	return 0L;
}

/*
 *	_CAÕXe[^Xj^XV bZ[Wnh
 *		EXe[^X
 *		EʃXe[^X([h)
 *		E쒆tO
 *		EPR tO
 *		EegWV
 *		EeW[
 */
LRESULT CChiefView::OnUpdateStatesOnDlg(WPARAM wparam, LPARAM lparam)
{
	//Saiki 20090527 Change ----->
	//// Xe[^X ------------------------------------------------------
	//SetDlgItemText(IDC_STATIC_PRCSTS, PROCESS_STATUS_DISP[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()]);
	//// ʃXe[^X([h) ----------------------------------------------
	//SetDlgItemText(IDC_STATIC_DSPSTS, DISPMODE_STATUS_DISP[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()]);
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if(l_SystemConfig.nLanguage == 0){
		// Xe[^X ------------------------------------------------------
		SetDlgItemText(IDC_STATIC_PRCSTS, PROCESS_STATUS_DISP_ENU[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()]);
		// ʃXe[^X([h) ----------------------------------------------
		SetDlgItemText(IDC_STATIC_DSPSTS, DISPMODE_STATUS_DISP_ENU[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()]);
	}
	else{
		// Xe[^X ------------------------------------------------------
		SetDlgItemText(IDC_STATIC_PRCSTS, PROCESS_STATUS_DISP_JPN[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()]);
		// ʃXe[^X([h) ----------------------------------------------
		SetDlgItemText(IDC_STATIC_DSPSTS, DISPMODE_STATUS_DISP_JPN[((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()]);
	}

	//Saiki 20090527 Change <-----
	// 쒆tO --------------------------------------------------------
	SetDlgItemText(IDC_STATIC_ACTXY,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTZ,		(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTPIN,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_PIN))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTSHUT,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_SHUTTER))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTWG,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_WORKGUIDE))? _T("ON") : _T("OFF"));
	SetDlgItemText(IDC_STATIC_ACTTRET,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_TURRET))? _T("ON") : _T("OFF"));
	//2009.11.10 bagus MS --{-- ǉꂽ쒆tO̕\
// 2009.11.24 K.Matsuo Delete -->
//	SetDlgItemText(IDC_STATIC_ACTHEAD,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_HEAD))? _T("ON") : _T("OFF"));
// 2009.11.24 K.Matsuo Delete <--
	SetDlgItemText(IDC_STATIC_ACTMS,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_MICROSCOPE))? _T("ON") : _T("OFF"));
	//2009.11.10 bagus MS --}-- ǉꂽ쒆tO̕\

	// PR tO ---------------------------------------------------------
	SetDlgItemText(IDC_STATIC_PRSTS,	(0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetPatRecFlag())? _T("ON") : _T("OFF"));
	// egWV ------------------------------------------------
	// }X^ gWV
	SetDlgItemText(IDC_STATIC_TRMASTER, STATES_MASTER_DISP[((CChiefTransiMaster*) m_pcChiefTransiMaster)->GetCurrentState()]);
	// V[PX gWV
	SetDlgItemText(IDC_STATIC_TRSEQ, STATES_SEQ_DISP[((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentState()]);
	// SR t@X gWV
	SetDlgItemText(IDC_STATIC_TRSRREFER, STATES_SR_REFER_DISP[((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->GetCurrentState()]);
	// fXL[ gWV
	SetDlgItemText(IDC_STATIC_TRDESKEW, STATES_DESKEW_DISP[((CChiefTransiDeskew*) m_pcChiefTransiDeskew)->GetCurrentState()]);
	// AF gWV
	SetDlgItemText(IDC_STATIC_TRAAF, STATES_AAF_DISP[((CChiefTransiAF*) m_pcChiefTransiAF)->GetCurrentState()]);
	// SR P|Cg gWV
	SetDlgItemText(IDC_STATIC_TRSR1POINT, STATES_SR_1POINT_DISP[((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->GetCurrentState()]);
/* added 2009.08.06 hmenjo XgX@\ǉ(27) ---------- { ---------- */
	// XgX V[PX gWV
	SetDlgItemText(IDC_STATIC_TRSTRS, STATES_STRS_DISP[((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState()]);
/* added 2009.08.06 hmenjo XgX@\ǉ(27) ---------- } ---------- */
	// eW[ ----------------------------------------------------
	// Xe[WW[
	SetDlgItemText(IDC_STATIC_MODSTAGE, (0 == m_dwModuleState_Stage)? _T("Uninitialize") : _T("Initialized"));
	// SR 胂W[
	SetDlgItemText(IDC_STATIC_MODSRMEAS, (0 == m_dwModuleState_SR_Meas)? _T("Uninitialize") : _T("Initialized"));
	// Pif W[
	SetDlgItemText(IDC_STATIC_MODPIF, (0 == m_dwModuleState_Pif)? _T("Uninitialize") : _T("Initialized"));
	// Nextra W[
	SetDlgItemText(IDC_STATIC_MODNEXTRA, (0 == m_dwModuleState_Nextra)? _T("Uninitialize") : _T("Initialized"));
	// eXbhp HeartBeat pX ---------------------------------------
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
/* added 2009.05.27 hmenjo SPT A[̌o ---------- { ---------- */
	SPTAlarmDetector();
/* added 2009.05.27 hmenjo SPT A[̌o ---------- } ---------- */

	return 0L;
}

// 2009.02.05 K.Matsuo delete -->
///*
// *	g[Xf[^莞(M)p^C} X^[g֐
// */
//BOOL CChiefView::StartTimerTraceData(UINT uiTraceDataPeriod)
//{
//	if (ID_TIMER_TRACEDATA != SetTimer(ID_TIMER_TRACEDATA, uiTraceDataPeriod, 0)) {
//		// ^C}Ns
//		return FALSE;
//	} else {
//		// ^C}N
//		return TRUE;
//	}
//}
//
///*
// *	g[Xf[^M bZ[Wnh
// */
//LRESULT CChiefView::OnSendTraceData(WPARAM wparam, LPARAM lparam)
//{
//	PifComm_TraceDataReport();
//		/*
//		 *	̊֐́C܂Cł̂ŁCύX̉\܂D
//		 */
//
//	return 0L;
//}
// 2009.02.05 K.Matsuo delete <--

/*
 *	DI ()擾
 *		Nextra IO 擾܂
 */
void CChiefView::GetDiInfo(CHIEF_DI_INFO* pDiInfo)
{
	pDiInfo->bEMOStop			= nexioIsEmergencyStop();
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
	pDiInfo->bSafetyPlug		= nexioIsSafetyPlug();
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
	pDiInfo->bDoorInterlock		= nexioIsDoorInterlock();
	pDiInfo->bEQPower			= nexioIsEquipmentPower();
//	pDiInfo->bAlignmentPowerOn	= TRUE;	//nexioGetInpDataPtr()->AlignmentPowerOn;	͍폜܂(2008.10.27)D
	pDiInfo->bMaintenanceSW		= nexioIsMaintenanceSwitch();
	pDiInfo->bTHMaintenanceSW	= nexioIsEngineerMaintenanceSwitch();
/* modified hmenjo 2009.05.20 GA͒ቺõbp֐ -------- { -------- */
//	pDiInfo->bAirPressureLow	= nexioIsAirPressureLevelLow();
/* modified hmenjo 2009.05.20 GA͒ቺõbp֐ -------- 		   */
	pDiInfo->bAirPressureLow	= ((CNanoSpecDoc*) m_pcNanoSpecDoc)->Rap_IsAirPressureLowON();
/* modified hmenjo 2009.05.20 GA͒ቺõbp֐ -------- } -------- */
	pDiInfo->bGlassExist		= nexioIsGlassExist();
	pDiInfo->bLoadPos			= nexioIsStageLoadPos();
	pDiInfo->bShutterOpen		= nexioIsShutterOpen();
	pDiInfo->bShutterClose		= nexioIsShutterClose();
	pDiInfo->bRobotArm			= nexioIsRobotArmDetect();
	pDiInfo->bVacuumPressure1	= nexioIsVacuumOn();
//	pDiInfo->bVacuumPressure2	= nexioIsVacuumOn();		͍폜܂(2008.10.27)D
	pDiInfo->bPinUp				= nexioIsPinUpperPos();
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
//	pDiInfo->bPinDown			= nexioIsPinDownPos();
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ----------			   */
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
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */
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
 *	Pif DI tbV bZ[Wnh
 *		DI  Pif ̋LGA(vZXXe[^X񍐗pt@C}bsO)ɃZbg܂D
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
//	PifComm_GetEqMonitorPtr()->iVaccumOn2		= m_DiInfo.bVacuumPressure2;	͍폜܂(2008.10.27)

	return 0L;
}

/*
 *	EQ 쒆`FbN bZ[Wnh
 */
LRESULT CChiefView::OnEQRunCheck(WPARAM wparam, LPARAM lparam)
{
	BOOL	l_bEQRun = FALSE;

	// vZXXe[^X`FbN
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
		// 쒆tO`FbN
		if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
			l_bEQRun = TRUE;
		}
	}
	if (FALSE == l_bEQRun) {
		// WCXeBbN`FbN
		if (0 != ((CMainFrame*) m_pcMainFrame)->GetJoyStickMode()) {
			// n[hWC
			l_bEQRun = TRUE;
		}
	}

	if ((0 != m_DiInfo.bMaintenanceSW)
	 && (0 == m_DiInfo.bTHMaintenanceSW)) {
		// e SW ItCCe SW ItĈƂ̓I܂D
		// ܂Cʏ탂[ĥƂłD
		// A̓쒆͋It͂܂D
		if (PROCESS_INIT != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus()) {
			l_bEQRun = FALSE;
		}
	}

	if (0 == m_DiInfo.bEQPower) {
		// udItȂ̂ŃI܂D
		l_bEQRun = FALSE;
	}
	if (m_bEQRunPrev != l_bEQRun) {
		// EQ 쒆̏Ԃɕω̂ EQ 쒆o͂ݒ
		nexioEquipmentStatusRun(l_bEQRun);
		m_bEQRunPrev = l_bEQRun;
	}

	return 0L;
}

/*
 *	egWV XbhN
 */
DWORD CChiefView::TransitionsStart()
{
	DWORD	l_dwRc = 0;

	// AF gWV XbhN
	LogChief(_T("Requested to start CChiefTransiAF."));
	m_pcChiefTransiAF = new CChiefTransiAF((CWnd*) this);
	if (0 == m_pcChiefTransiAF->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiAF."));
		l_dwRc = 6;
	}
	// fXL[ gWV XbhN
	LogChief(_T("Requested to start CChiefTransiDeskew."));
	m_pcChiefTransiDeskew = new CChiefTransiDeskew((CWnd*) this);
	if (0 == m_pcChiefTransiDeskew->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiDeskew."));
		l_dwRc = 5;
	}
	// SR t@X gWV XbhN
	LogChief(_T("Requested to start CChiefTransiSrRefer."));
	m_pcChiefTransiSrRefer = new CChiefTransiSrRefer((CWnd*) this);
	if (0 == m_pcChiefTransiSrRefer->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiSrRefer."));
		l_dwRc = 4;
	}
	// SR P|Cg gWV XbhN
	LogChief(_T("Requested to start CChiefTransiSr1Point."));
	m_pcChiefTransiSr1Point = new CChiefTransiSr1Point((CWnd*) this);
	if (0 == m_pcChiefTransiSr1Point->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiSr1Point."));
		l_dwRc = 3;
	}
	// V[PX gWV XbhN
	LogChief(_T("Requested to start CChiefTransiSeq."));
	m_pcChiefTransiSeq = new CChiefTransiSeq((CWnd*) this);
	if (0 == m_pcChiefTransiSeq->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiSeq."));
		l_dwRc = 2;
	}
/* added 2009.07.30 hmenjo XgX@\ǉ(2) ---------- { ---------- */
	// XgX gWV XbhN
	LogChief(_T("Requested to start CChiefTransiStress."));
	m_pcChiefTransiStress = new CChiefTransiStress((CWnd*) this);
	if (0 == m_pcChiefTransiStress->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiStress."));
		l_dwRc = 2;
	}
/* added 2009.07.30 hmenjo XgX@\ǉ(2) ---------- } ---------- */
	// }X^ gWV XbhN
	LogChief(_T("Requested to start CChiefTransiMaster."));
	m_pcChiefTransiMaster = new CChiefTransiMaster((CWnd*) this);
	if (0 == m_pcChiefTransiMaster->m_hThread) {
		// XbhNs
		LogChief(_T("Failed to start CChiefTransiMaster."));
		l_dwRc = 1;
	}

	return l_dwRc;
}

/*
 *	egWV XbhI
 */
void CChiefView::TransitionsEnd()
{
	// AF gWV XbhI
	if (0 != m_pcChiefTransiAF) {
		LogChief(_T("Deleting CChiefTransiAF..."));
		delete m_pcChiefTransiAF;
		m_pcChiefTransiAF = 0;
		LogChief(_T("Deleted  CChiefTransiAF."));
	}
	// fXL[ gWV XbhI
	if (0 != m_pcChiefTransiDeskew) {
		LogChief(_T("Deleting CChiefTransiDeskew..."));
		delete m_pcChiefTransiDeskew;
		m_pcChiefTransiDeskew = 0;
		LogChief(_T("Deleted  CChiefTransiDeskew."));
	}
	// SR t@X gWV XbhI
	if (0 != m_pcChiefTransiSrRefer) {
		LogChief(_T("Deleting CChiefTransiSrRefer..."));
		delete m_pcChiefTransiSrRefer;
		m_pcChiefTransiSrRefer = 0;
		LogChief(_T("Deleted  CChiefTransiSrRefer."));
	}
	// SR P|Cg gWV XbhI
	if (0 != m_pcChiefTransiSr1Point) {
		LogChief(_T("Deleting CChiefTransiSr1Point..."));
		delete m_pcChiefTransiSr1Point;
		m_pcChiefTransiSr1Point = 0;
		LogChief(_T("Deleted  CChiefTransiSr1Point."));
	}
/* added 2009.07.30 hmenjo XgX@\ǉ(2) ---------- { ---------- */
	// V[PX gWV XbhI
	if (0 != m_pcChiefTransiStress) {
		LogChief(_T("Deleting CChiefTransiStress..."));
		delete m_pcChiefTransiStress;
		m_pcChiefTransiStress = 0;
		LogChief(_T("Deleted  CChiefTransiStress."));
	}
/* added 2009.07.30 hmenjo XgX@\ǉ(2) ---------- } ---------- */
	// V[PX gWV XbhI
	if (0 != m_pcChiefTransiSeq) {
		LogChief(_T("Deleting CChiefTransiSeq..."));
		delete m_pcChiefTransiSeq;
		m_pcChiefTransiSeq = 0;
		LogChief(_T("Deleted  CChiefTransiSeq."));
	}
	// }X^ gWV XbhI
	if (0 != m_pcChiefTransiMaster) {
		LogChief(_T("Deleting CChiefTransiMaster..."));
		delete m_pcChiefTransiMaster;
		m_pcChiefTransiMaster = 0;
		LogChief(_T("Deleted  CChiefTransiMaster."));
	}
}
/*
 *	eA[p bZ[Wnh
 */
/*			p^(`"ChiefExports.h"ɂ܂)
 *				wparam
 *						[P]FLOBYTE(LOWORD(wparam))	񍐕@
 *															bZ[W{bNX̕\̂
 *															A[񍐂̂
 *															
 *						[Q]FHIBYTE(LOWORD(wparam))	eʒm(CMainFrame)
 *															Ȃ
 *															(̌̏͐eł)
 *						[R]FLOBYTE(HIWORD(wparam))	bZ[W{bNX̃^Cgԍ
 *						[S]FHIBYTE(HIWORD(wparam))	bZ[W{bNX̃{^\
 *															OK
 *															YES/NO(YES ̏ꍇ͐eʒm̃AvIp^ TRUE ɂ)
 *				lparam
 *						[T]FLOWORD(lparam)			bZ[W{bNX̃eLXgԍ
 *						[U]FHIWORD(lparam)			A[񍐂 ALID
 *			w@
 *				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD([P], [Q]), MAKEWORD([R], [S])), MAKELPARAM([T], [U]));
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

/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- { ---------- */
		switch (l_dwALID) {
		case ALID_CTA_Setinf:
			this->CtaReset(1);
			break;
		}
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- } ---------- */

		// A[ ------------------------------------------------------
//		if ((CHRAMTD_BOTH == l_dwMethod) || (CHRAMTD_REP_ALARM == l_dwMethod)) {
		if (0 != (0x00000002 & l_dwMethod)) {
			AlarmIf_Set(l_dwALID);
		}
		// bZ[W{bNX\ --------------------------------------------
		DWORD l_dwNotifyCode = 0;
//		if ((CHRAMTD_BOTH == l_dwMethod) || (CHRAMTD_MSGBOX == l_dwMethod)) {
		if (0 != (0x00000001 & l_dwMethod)) {
			// bZ[W{bNX̃{^`쐬
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
			// NbN{^
			switch (l_dwMsgButton) {
			case CHRAMSG_YESNO:
				if (IDYES == l_iMB_Result) {
					l_dwNotifyCode = 1;		// AvIw
				}
				break;
			case CHRAMSG_OK:
			default:
				break;
			}
		}
		// eʒm ------------------------------------------------------------
		if (CHRANFY_NOTIFY_ON == l_dwNotify) {
			m_pcMainFrame->PostMessage(WM_CHIF_ERROR_NOTIFY, (WPARAM) l_dwNotifyCode, 0);
		}
	}

	return 0L;
}

/*
 *	ėp|WVړw(Pif ) bZ[Wnh
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
		// ͈͊Oł
		//	ɂ͉͂܂D
		LogChief(_T("WM_CHIF_MOVE_SETPOS : Position No. was out of range."));
	} else {
		if (0 == l_dwMovePosId) {
			// nh [h |WV
			ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);
		} else if ((1 <= l_dwMovePosId) && (l_dwMovePosId <= GENERAL_POSITION_MAX)) {
			// ėp |WV
			GENERAL_POSITION l_GeneralPosition[GENERAL_POSITION_MAX];
			ConfigFile_GetNanoSpecIni(l_GeneralPosition, CONFIG_FILE_GENERAL_POSITION);
			l_SetPosition = l_GeneralPosition[l_dwMovePosId - 1].Loc;
		}
		// Xe[Wړ
#ifndef CHIEF_STAGE_ON
		{
//	StageMoveAbsoluteEx() ܂ŗLłD
			if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
				ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 쒆tO(XY Xe[W)I
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
/* added 2009.11.26 hmenjo  Seq ͎wwbhœ ---------- { ---------- */
		/* SR wbhɂ܂D	*/
		this->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo  Seq ͎wwbhœ ---------- } ---------- */
		if (0 != ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
			// n[hV~[g[h̏ꍇ
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
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
				m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_HLDPOS_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
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
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
				m_pcMainFrame->SetMessageText(IDS_CHIF_HWS_GENPOS_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
				//Saiki 20090602 Change <-----
			}
		} else
#ifndef CHIEF_STAGE_ON
		NS_ConvertToStageMoveCoord(&l_SetPosition);					// ␳
//		if (0 == StageMoveAbsoluteEx(&l_SetPosition, CHIEF_STGMVABS_MODE)) {		Stage.dll Ή܂ł͉Ľďo@ɂȂ܂D
		if (0 == StageMoveAbsolute(&l_SetPosition)) {
#else
		if (0 == NS_StageMoveAbsoluteEx(&l_SetPosition)) {
#endif
			// s
			if (0 == l_dwMovePosId) {
				// Xe[WړJns(StageMoveAbsolute())
				PifComm_LoadPositionMovementResultReport(2);
			} else {
				// Xe[WړJns(StageMoveAbsolute())
				PifComm_GeneralPurposePositionMovementResultReport(2);
			}
#ifndef CHIEF_STAGE_ON
			{
//	StageMoveAbsoluteEx() ܂ŗLłD
				ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 쒆tO(XY Xe[W)It
			}
#endif
		} else {
			// 
			if (0 == l_dwMovePosId) {
				m_dwStageMoveState = 2;			// nh [h |WV ړ
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HLDPOS_MOVING, strMsg, "Moving Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			} else {
				m_dwStageMoveState = 3;			// ėp |WV ړ
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_GENPOS_MOVING, strMsg, "Moving General Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			}
#ifdef CHIEF_STAGE_ON
				ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 쒆tO(XY Xe[W)I
#endif
		}
	}

	return 0L;
}

/*
 *	ėp|WVړw(ʂ) bZ[Wnh
 */
LRESULT CChiefView::OnMoveSetPosFromDisp(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_MOVE_SETPOS"), wparam, lparam);
	//Saiki 20090602 Add ----->
	CString strMsg;
	//Saiki 20090602 Add <-----

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		// EMOChAC^bNCup[It
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) (30 + l_dwEMO), 0);
		return 0L;
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		// {bgA[o
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 34, 0);
		return 0L;
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		// s_EIt
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 35, 0);
		return 0L;
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		// GA͒ቺI
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 36, 0);
		return 0L;
	}
	if (
		(0 == m_DiInfo.bTHMaintenanceSW)
	 && ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen))
	) {
		// eItŁCCVb^ CLOSE ItCCVb^ OPEN I
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 37, 0);
		return 0L;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		/* CTAILPI It	*/
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 50, 0);
		return 0L;
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		if (1 == iResistStatus) {
			/* v[u㏸[ZT[ It	*/
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 52, 0);
		} else if (2 == iResistStatus) {
			/* v[u㏸[E~[ZT[ I	*/
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 53, 0);
		} else {
			/* v[u㏸[E~[ZT[ It	*/
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 54, 0);
		}
		return 0L;
	}
	//2009.11.12 bagus MS --{--
	//[ʒuQ̃C^[bNmF
	if (0 == this->IsMSILPI()) {
		/* CTAILPI It	*/
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 55, 0);
		return 0L;
	}
	//2009.11.12 bagus MS --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- } ---------- */
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		// [JłȂ
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 2, 0);
		return 0L;
	}
#if 0	// ^[bg쒆tOɒǉ߁C^[bg͏O
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
#else	// ^[bg쒆tOɒǉ߁C^[bg͏O
	DWORD l_dwActFlags = ACTUATE_XYSTAGE | ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_SHUTTER | ACTUATE_WORKGUIDE;
	if (0 != (l_dwActFlags & ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
#endif	// ^[bg쒆tOɒǉ߁C^[bg͏O
		// 쒆tOI
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 6, 0);
		return 0L;
	}
	int l_iProcStatus = ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		// WaitCDown ȊO
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 8, 0);
		return 0L;
	}
	if (MAIN_MENU_DATA == ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()) {
		// ʃ[huf[^[hv
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 11, 0);
		return 0L;
	}
	if (0 == m_DiInfo.bMaintenanceSW) {
		// eiX SW ÍEEE
		if (0 == m_DiInfo.bTHMaintenanceSW) {
			// ɁCe SW It̏ꍇC֎~
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 15, 0);
			return 0L;
		}
#if 1	// ełȂCȅꍇɓ֎~ɂ܂
	} else {
		// eiX SW It́EEE
		if (0 != m_DiInfo.bTHMaintenanceSW) {
			// e SW ȈꍇC֎~
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 15, 0);
			return 0L;
		}
#endif
	}

	STAGE_COORD l_SetPosition;
	long l_dwMovePosId = (long) wparam;

	if ((l_dwMovePosId < -1) || (GENERAL_POSITION_MAX < l_dwMovePosId)) {
		// ͈(-1`GENERAL_POSITION_MAX)OG[
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 1, 0);
		LogChief(_T("WM_DISP_MOVE_SETPOS : Position No. was out of range."));
	} else {
		if (-1 == l_dwMovePosId) {
			// }jA [h |WV
			ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_MANUAL_LOAD_POSITION);
		} else if (0 == l_dwMovePosId) {
			// nh [h |WV
			ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);
		} else if (( 1<= l_dwMovePosId) && (l_dwMovePosId <= GENERAL_POSITION_MAX)) {
			// ėp |WV
			GENERAL_POSITION l_GeneralPosition[GENERAL_POSITION_MAX];
			ConfigFile_GetNanoSpecIni(l_GeneralPosition, CONFIG_FILE_GENERAL_POSITION);
			l_SetPosition = l_GeneralPosition[l_dwMovePosId - 1].Loc;
		}
		// Xe[Wړ ---------------------------------
#ifndef CHIEF_STAGE_ON
		{
//	StageMoveAbsoluteEx() ܂ŗLłD
			if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
				ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 쒆tO(XY Xe[W)I
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
/* added 2009.11.26 hmenjo  Seq ͎wwbhœ ---------- { ---------- */
		/* SR wbhɂ܂D	*/
		this->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo  Seq ͎wwbhœ ---------- } ---------- */
		if (0 != ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
			// n[hV~[g[h̏ꍇ
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
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
				m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
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
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
				m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
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
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
				m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
				m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
				//Saiki 20090602 Change <-----
			}
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, 0, 0);
		} else
#ifndef CHIEF_STAGE_ON
		NS_ConvertToStageMoveCoord(&l_SetPosition);
//		if (0 == StageMoveAbsoluteEx(&l_SetPosition, CHIEF_STGMVABS_MODE)) {		Stage.dll Ή܂ł͉Ľďo@ɂȂ܂D
		if (0 == StageMoveAbsolute(&l_SetPosition)) {
#else
		if (0 == NS_StageMoveAbsoluteEx(&l_SetPosition)) {
#endif
			// s
#ifndef CHIEF_STAGE_ON
			{
//	StageMoveAbsoluteEx() ܂ŗLłD
				ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 쒆tO(XY Xe[W)It
			}
#endif
			m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 1, 0);
		} else {
			// 
			if (-1 == l_dwMovePosId) {
				m_dwStageMoveState = 11;		// ʃ}jA [h |WV ړ
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_MLDPOS_MOVING, strMsg, "Moving Manual Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			} else if (0 == l_dwMovePosId) {
				m_dwStageMoveState = 12;		// ʃnh [h |WV ړ
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_HLDPOS_MOVING, strMsg, "Moving Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			} else {
				m_dwStageMoveState = 13;		// ʔėp |WV ړ
				//Saiki 20090602 Change ----->
				//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVING);  // Status Bar
				LoadStringML(IDS_CHIF_MLDPOS_MOVING, strMsg, "Moving Manual Load Position...");
				m_pcMainFrame->SetMessageText(strMsg);	// Status Bar
				//Saiki 20090602 Change <-----
			}
#ifdef CHIEF_STAGE_ON
			ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 쒆tO(XY Xe[W)I
#endif
		}
	}

	return 0L;
}

/*
 *	Xe[W̒~ԂmF
 *		^C}ȂǂŃ|[Oɂ邱
 */
void CChiefView::IsStageIdle()
{
	// Xe[WɈړw߂oĂ鎞̂
	if (0 != m_dwStageMoveState) {
#ifndef CHIEF_STAGE_ON
		if (0 != StageIsIdle()) {
#else
		if (0 != NS_IsIdleStageMoveAsync()) {
#endif
			// XY ~
/* added 2009.11.04 hmenjo Xe[W~oO ---------- { ---------- */
			TCHAR l_tszLog[128];
			_stprintf(l_tszLog, _T("Detected Stage stopping (m_dwStageMoveState = %d). Notify WM_CHIF_STAGE_STOP."), m_dwStageMoveState);
			this->LogChief(l_tszLog);
/* added 2009.11.04 hmenjo Xe[W~oO ---------- } ---------- */
			StageStop();			// ߂l͖Ă܂
			PostMessage(WM_CHIF_STAGE_STOP, 0, 0);
			ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 쒆tO(XY Xe[W)It
		}
	}
}

/*
 *	Xe[W~o bZ[Wnh
 */
LRESULT CChiefView::OnStageStop(WPARAM wparam, LPARAM lparam)
{
	//Saiki 20090602 Add ----->
	CString strMsg;
	//Saiki 20090602 Add <-----

	// Xe[WɈړw߂oĂ鎞̂
	switch (m_dwStageMoveState) {
	case 2:		// Pif nh [h |WV ړ
		// Pif nh [h |WV ړ 
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVED);   // Status Bar
		LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
		m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected Pif-Load position move stopping."));
		PifComm_LoadPositionMovementResultReport(0);
		break;
	case 3:		// Pif ėp |WV ړ
		// Pif ėp |WV ړ 
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVED);   // Status Bar
		LoadStringML(IDS_CHIF_GENPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
		m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected Pif-General position move stopping."));
		PifComm_GeneralPurposePositionMovementResultReport(0);
		break;
	case 11:	//  }jA [h |WV ړ
	case 12:	//  nh [h |WV ړ
	case 13:	//  ėp |WV ړ
		//  }jA/[h/ėp |WVړ 
		switch (m_dwStageMoveState) {
		case 11:
			//Saiki 20090602 Change ----->
			//m_pcMainFrame->SetMessageText(IDS_CHIF_MLDPOS_MOVED);   // Status Bar
			LoadStringML(IDS_CHIF_MLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
			m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
			m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
			//Saiki 20090602 Change <-----
			LogChief(_T("Detected Disp-Man position move stopping."));
			break;
		case 12:
			//Saiki 20090602 Change ----->
			//m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVED);   // Status Bar
			LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
			m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
			m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
			//Saiki 20090602 Change <-----
			LogChief(_T("Detected Disp-Load position move stopping."));
			break;
		case 13:
		default:
			//Saiki 20090602 Change ----->
			//m_pcMainFrame->SetMessageText(IDS_CHIF_GENPOS_MOVED);   // Status Bar
			LoadStringML(IDS_CHIF_GENPOS_MOVED, strMsg, " ");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
			m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
			m_pcMainFrame->SetMessageText(_T(" "));   // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
			//Saiki 20090602 Change <-----
			LogChief(_T("Detected Disp-General position move stopping."));
			break;
		}
		m_pcMainFrame->PostMessage(WM_CHIF_MOVE_SETPOS_END, (WPARAM) 0, 0);
		break;
	case 21:	// SR t@X (m[})t@Xʒu ړ
		// ړ
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_STAGE_MOVED); // Status Bar
		LoadStringML(IDS_CHIF_SREF_N_STAGE_MOVED, strMsg, "Reference Measurement:(Normal)Reference Measuremnt Position(XY) Move Complete");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg); // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
		m_pcMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected SR (Normal) Reference position move stopping."));
		WaitTimeBeforeMeas(0);		// JnÕwbhh҂(2008.12.18 ǉ)̎擾
		((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_N_MOVE_DONE);
		break;
	case 22:	// SR t@X _[Nt@Xʒu ړ
		// ړ
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_STAGE_MOVED); // Status Bar
		LoadStringML(IDS_CHIF_SREF_D_STAGE_MOVED, strMsg, "Reference Measurement:Dark Reference Measuremnt Position(XY) Move Complete");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg); // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
		m_pcMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
		//Saiki 20090602 Change <-----
		LogChief(_T("Detected SR Dark Reference position move stopping."));
		WaitTimeBeforeMeas(0);		// JnÕwbhh҂(2008.12.18 ǉ)̎擾
		((CChiefTransiSrRefer*) m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_D_MOVE_DONE);
		break;
	case 31:	// V[PXʒu ړ
	case 32:	// V[PXʒu sړ
		// ړ
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	  if (32 != m_dwStageMoveState) {
		/* sړ̓Xe[^Xo[\͂ȂD	*/
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
		//Saiki 20090602 Change ----->
		//m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_STAGE_MOVED);	  // Status Bar
		LoadStringML(IDS_CHIF_SEQ_STAGE_MOVED, strMsg, "Sequence Measurement:Measurement Position(XY)Complete");
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- { ---------- */
		m_pcMainFrame->SetMessageText(strMsg);	  // Status Bar
#else						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ----------			   */
		m_pcMainFrame->SetMessageText(_T(" "));    // Status Bar
#endif						/* modified 2009.08.05 hmenjo bZ[W\Ȃ ---------- } ---------- */
		//Saiki 20090602 Change <-----
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	  }
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
		LogChief(_T("Detected SEQ position move stopping."));
		WaitTimeBeforeMeas(0);		// JnÕwbhh҂(2008.12.18 ǉ)̎擾
/* modified 2013.02.01 hmenjo PreAF ---------- { ---------- */
//		((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_POINT_MOVE_DONE);
/* modified 2013.02.01 hmenjo PreAF ----------              */
		if ((32 == m_dwStageMoveState) && (1 == m_lPreAFafterPreMove)) {
			if (FALSE != this->IsBusyTransi(3, 7)) {
				this->LogChief(_T("PreAF - AF Seq is busy."));
				/* AF gWVrW[łD	*/
				m_lPreAFafterPreMove = 0;
				/* Pre AF ͍s܂D	*/
			} else {
				/* tB^ I[v	*/
				if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
					// ُ펞ł܂D
					this->LogChief(_T("PreAF - Failed to MEAS_SrHead_ChangeCcdShutter()."));
				}
				this->WaitTimeBeforeMeas(1);
				/* AF s	*/
				((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(0, 0), 0));
				this->LogChief(_T("PreAF - Start AF Seq."));
			}
		} else {
			((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_POINT_MOVE_DONE);
		}
/* modified 2013.02.01 hmenjo PreAF ---------- } ---------- */
		break;
/* added 2009.08.03 hmenjo XgX@\ǉ(17) ---------- { ---------- */
	case 72:	/* XgX Seq nh [h |WV ړ	*/
		LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
		LogChief(_T("Detected Stress-Load position move stopping."));
		((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LDPOS_DONE);
		break;
/* added 2009.08.03 hmenjo XgX@\ǉ(17) ---------- } ---------- */
/* added 2009.08.03 hmenjo XgX@\ǉ(18) ---------- { ---------- */
	case 73:	/* XgX Seq C ړ	*/
		LoadStringML(IDS_CHIF_HLDPOS_MOVED, strMsg, " ");
		m_pcMainFrame->SetMessageText(strMsg);	 // Status Bar
		LogChief(_T("Detected Stress-Line move stopping."));
		((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
		break;
/* added 2009.08.03 hmenjo XgX@\ǉ(18) ---------- } ---------- */
	default:
		break;
	}
	if (0 != m_dwStageMoveState) {
		m_dwStageMoveState = 0;
	}

	return 0L;
}

/*
 *	oL[ ON/OFF w bZ[Wnh
 */
LRESULT CChiefView::OnVacuumFromDisp(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_VACUUM_ONOFF"), wparam, lparam);

	// C^bN
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		// [JłȂ
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 2, 0);
		return 0L;
	}
	int l_iProcStatus = ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		// WaitCDown ȊO
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 8, 0);
		return 0L;
	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_DATA == l_iDispStatus) || (MAIN_MENU_USER_SETTING == l_iDispStatus)) {
		// ʃ[huf[^[hvu[Uݒ胂[hv
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 14, 0);
		return 0L;
	}
	if (
		(0 == m_DiInfo.bTHMaintenanceSW)
	 && ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen))
	) {
		// eItŁCCVb^ CLOSE ItCCVb^ OPEN I
		m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) 37, 0);
		return 0L;
	}

	BOOL l_bNexResult;
	if (0 == wparam) {
		// OFF w
		l_bNexResult = nexifVacuumOff(this->m_hWnd);
		if (0 == l_bNexResult) {
			m_bVacuumOffFromDisp = TRUE;
		} else {
			m_pcMainFrame->PostMessage(WM_CHIF_VACUUM_END, (WPARAM) l_bNexResult, 0);
		}
	} else {
		// ON w
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
 *	WM_COPYDATA bZ[Wnh
 */
BOOL CChiefView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: ̈ʒuɃbZ[W nhp̃R[hǉ邩܂̓ftHg̏ĂяoĂ

// 2009.03.19 k-matsuo change --->
//	switch (pCopyDataStruct->dwData) {
//	case WMCD_CHIF_RECIPE_NOTIFY:	// Pif ̒HVsʒm
//		DWORD l_dwLength;
//		if (sizeof(m_szMainRecipeName) < pCopyDataStruct->cbData) {
//			l_dwLength = sizeof(m_szMainRecipeName);
//		} else {
//			l_dwLength = pCopyDataStruct->cbData;
//		}
//		memcpy(m_szMainRecipeName, pCopyDataStruct->lpData, l_dwLength);
//		m_szMainRecipeName[RECIPE_NAME_LEN] = 0;
//		this->PostMessage(WM_CHIF_SET_RECIPE, 0, 0);	// Jn̕ǂ܂D
//		return 1;
//		break;
//	default:
//		break;
//	}

	DWORD l_dwLength;

	switch (pCopyDataStruct->dwData) {
	case WMCD_CHIF_RECIPE_NOTIFY:	// Pif ̒HVsʒm
		if (RECIPE_NAME_LEN < pCopyDataStruct->cbData) {
			l_dwLength = RECIPE_NAME_LEN;
		} else {
			l_dwLength = pCopyDataStruct->cbData;
		}
		memcpy(m_szMainRecipeName, pCopyDataStruct->lpData, l_dwLength);
		m_szMainRecipeName[l_dwLength] = 0;
		this->PostMessage(WM_CHIF_SET_RECIPE, 0, 0);	// Jn̕ǂ܂D
		return 1;
		break;
	default:
		break;
	}
// 2009.03.19 k-matsuo change <---

	return CFormView::OnCopyData(pWnd, pCopyDataStruct);
}

/*
 *	Vsݒ bZ[Wnh
 *		Pif ʒmꂽHVsʃW[ɒʒm܂D
 */
LRESULT CChiefView::OnSetRecipe(WPARAM wparam, LPARAM lparam)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Notifyed Recipe(%s) from Pif to Dis."), m_szMainRecipeName);
	LogChief(l_tszLogText);

	// ʃW[ɒʒm
	COPYDATASTRUCT l_CopyData;
	l_CopyData.dwData = WMCD_CHIF_SET_RECIPE;
	l_CopyData.cbData = RECIPE_NAME_LEN + 1;
	l_CopyData.lpData = m_szMainRecipeName;
	m_pcMainFrame->SendMessage(WM_COPYDATA, (WPARAM) this->m_hWnd, (LPARAM) &l_CopyData);

	return 0L;
}

/*
 *	AF |bvAbvI bZ[Wnh
 *		ʂ AF ̃|bvAbvID
 */
LRESULT CChiefView::OnAFPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_AF_POPUP_END"), wparam, lparam);

	switch (wparam) {
	case 0:		// gC {^
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_RETRY, (cEventParams*) lparam);
		break;
	case 1:		// AF  {^
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_IGNORE, (cEventParams*) lparam);
		break;
	case 2:		// XLbv {^
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_SKIP, (cEventParams*) lparam);
		break;
	case 3:		// LZ {^
		((CChiefTransiAF*) m_pcChiefTransiAF)->TransiEvent(EV_AAF_POPUP_CANCEL, (cEventParams*) lparam);
		break;
	default:
		// ܂D
		break;
	}

	return 0L;
}

/*
 *	}jATCgp^[ |bvAbvI bZ[Wnh
 *		ʂ }jATCgp^[ ̃|bvAbvID
 */
LRESULT CChiefView::OnManualSitePtPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MANUALSITE_POPUP_END"), wparam, lparam);
	DWORD dwAFValid = 0;
	AF_VALID*	l_pAFValid = (AF_VALID*) &dwAFValid;

	switch (wparam) {
	//Saiki 20090728 Add ----->
	case 1:		//Manual Site PR AF
		l_pAFValid->ucResultPopup = 2;		// (OK){^
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
	case 2:		//Manual Site PR AFs
		l_pAFValid->ucResultPopup = 4;		// LZ{^
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
		// ܂D
		break;
	}

	return 0L;
}

/*
 *	JgCVs擾܂D
 */
void CChiefView::GetCurrentMainRecipeName(TCHAR *pszCurrentMainRecipeName)
{
	_tcscpy(pszCurrentMainRecipeName, m_szMainRecipeName);
}

/*
 *	f[^MJn bZ[Wnh
 *		胂W[ CCD f[^MCsړ\ɂȂʒm
 */
LRESULT CChiefView::OnMeasRecvData(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MEAS_RECVDATA (PreMove)"), wparam, lparam);

/* modified 2009.08.06 hmenjo XgX@\ǉ(28) ---------- { ---------- */
//	((CChiefTransiSeq*) m_pcChiefTransiSeq)->PreMove();
/* modified 2009.08.06 hmenjo XgX@\ǉ(28) ----------			   */
	if (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle()) {
		((CChiefTransiSeq*) m_pcChiefTransiSeq)->PreMove();
	} else if (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle()) {
		((CChiefTransiStress*) m_pcChiefTransiStress)->PreMove();
	}
/* modified 2009.08.06 hmenjo XgX@\ǉ(28) ---------- } ---------- */

	return 0L;
}

/*
 *	e탌VsǍ݂܂
 */
DWORD CChiefView::RecipesGet(
		TCHAR *pszMainRecipName,	// CVs
		DWORD dwMode				// 0FXe[WvO܂߂C0F܂߂Ȃ
	)
{
	int k;
	// CVs
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
	if (0 == RecipeFile_LoadRecipe(l_pMainRcpInfo, pszMainRecipName, RECIPE_FILE_MAIN_RECIPE)) {
		// CVsǍُ݈
		return 1;
	}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- { ---------- */
	WORD l_wHeadTypeMainRcp = l_pMainRcpInfo->MainRcpParam.hdr.wHeadType;
	if (HEAD_TYPE_STRESS == l_wHeadTypeMainRcp) {
		if (0 == l_pMainRcpInfo->MainRcpParam.hdr.bSampleID) {
			/* CVsǍُ݈(Tv ID gpɂȂĂȂ)	*/
			return 1;
		}
//Saiki 20110311 Add ----->
		LPSTRESS_PARAM_INFO l_pStressParamInfo = (LPSTRESS_PARAM_INFO) m_ChiefRecipes.pStressParamInfo;
		if (0 == RecipeFile_LoadStressParam(l_pStressParamInfo, pszMainRecipName, RECIPE_FILE_STRESS_MAIN_RECIPE)){
			return 1;
		}
//Saiki 20110311 Add <-----
	}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- } ---------- */
	// vO
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
	if (0 == RecipeFile_LoadRecipe(l_pMeasProgInfo, l_pMainRcpInfo->MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM)) {
		// vOǍُ݈
		return 2;
	} else {
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- { ---------- */
		if (l_wHeadTypeMainRcp != l_pMeasProgInfo->ScanParams.hdr.wHeadType) {
			/* vOǍُ݈(wbh^Cvsv)	*/
			return 2;
		}
		if (0 == ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsValidScanType(l_pMeasProgInfo->ScanParams.hdr.wScanType, l_wHeadTypeMainRcp)) {
			/* vOǍُ݈(wbh^CvƃXL^Cvsv)	*/
			return 2;
		}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- } ---------- */
		// Vssǉ
		switch (l_pMeasProgInfo->ScanParams.hdr.wScanType) {
		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:		// ːFx
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:	// ߐFx
		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:		// wZx
			l_pMeasProgInfo->ScanParams._SR.WavelenRange.wStart = CIE_MINWAVE;
			l_pMeasProgInfo->ScanParams._SR.WavelenRange.wEnd = CIE_MAXWAVE;
			break;
		default:
			// ܂D
			break;
		}
	}

/* modified 2009.11.20 hmenjo CTA Stage PGM Ǎ ---------- { ---------- */
///* added 2009.10.29 hmenjo CTA Seq API ǉ ---------- { ---------- */
//	/* Xe[WvOǍ	*/
//		/*	CTA ̊݂擾Ȃ΂ȂȂ߁CuXe[WvO܂߂ȂvݒłC
//			RRŁCXe[WvOǍ݂܂D	*/
//	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
//	BOOL l_bStgPgmValid = RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
//	if (0 == l_bStgPgmValid) {
//		/* Xe[WvOǍُ݈	*/
//		return 3;
//	}
///* added 2009.10.29 hmenjo CTA Seq API ǉ ---------- } ---------- */
/* modified 2009.11.20 hmenjo CTA Stage PGM Ǎ ----------			   */
	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;

// 2014.01.07 bagus Add(Stage NoneΉ) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	if(l_SystemConfig.nStageType == STAGE_TYPE_NONE){
		::ZeroMemory(l_pStageProgInfoHdr, sizeof(_STAGE_PROG_INFO_HDR));
		l_pStageProgInfoHdr->wNumScans = 1;
		return 0;
	}
// 2014.01.07 bagus Add(Stage NoneΉ) <--

	if (HEAD_TYPE_CTA == l_wHeadTypeMainRcp) {
		/* RRł CTA ̏ꍇ̂݃Xe[WvOǍ	*/
		/*	CTA ̊݂擾Ȃ΂ȂȂ߁CuXe[WvO܂߂ȂvݒłC
			RRŁCXe[WvOǍ݂܂D	*/
		BOOL l_bStgPgmValid = RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
		if (0 == l_bStgPgmValid) {
			/* Xe[WvOǍُ݈	*/
			return 3;
		}
	}
/* modified 2009.11.20 hmenjo CTA Stage PGM Ǎ ---------- } ---------- */

	switch (dwMode) {
	case 0:
		{
			// Xe[WvO
/* modified 2009.11.20 hmenjo CTA Stage PGM Ǎ ---------- { ---------- */
/* deleted 2009.10.29 hmenjo CTA Seq API ǉ ---------- { ---------- */
//			LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
//			if (0 == RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM)) {
//				// Xe[WvOǍُ݈
//				return 3;
//			}
/* deleted 2009.10.29 hmenjo CTA Seq API ǉ ---------- } ---------- */
/* modified 2009.11.20 hmenjo CTA Stage PGM Ǎ ----------			   */
			if (HEAD_TYPE_CTA != l_wHeadTypeMainRcp) {
				/* CTA ȊȌꍇ̂݃RRŃXe[WvOǍ	*/
				if (0 == RecipeFile_LoadRecipe(l_pStageProgInfoHdr, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM)) {
					// Xe[WvOǍُ݈
					return 3;
				}
			}
/* modified 2009.11.20 hmenjo CTA Stage PGM Ǎ ---------- } ---------- */
/* added 2009.10.15 hmenjo Stage PGM ʉΉ wbh^Cv ---------- { ---------- */
			/* wbh^Cvϊ
				Xe[WvÕwbh^Cv STD(=99) ̏ꍇ
				CVs̃wbh^Cvϊe[uɂ΁C
				Xe[WvÕwbh^CvCVs̃wbh^Cv
				u܂D	*/
			if (99 == l_pStageProgInfoHdr->wHeadType) {
/* modified 2009.11.20 hmenjo Stage PGM wbh^CvϊP ---------- { ---------- */
//				for (int l_i = 0; l_i < 99; l_i++) {
//					if (l_wHeadTypeMainRcp == HEADTYPE_STD_TBL[l_i]) {
//						l_pStageProgInfoHdr->wHeadType = l_wHeadTypeMainRcp;
//						break;
//					}
//				}
/* modified 2009.11.20 hmenjo Stage PGM wbh^CvϊP ---------- 			 */
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
/* modified 2009.11.20 hmenjo Stage PGM wbh^CvϊP ---------- } ---------- */
			}
/* added 2009.10.15 hmenjo Stage PGM ʉΉ wbh^Cv ---------- } ---------- */
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- { ---------- */
			if (l_wHeadTypeMainRcp != l_pStageProgInfoHdr->wHeadType) {
				/* Xe[WvOǍُ݈(wbh^Cvsv)	*/
				return 3;
			} else if (HEAD_TYPE_STRESS == l_wHeadTypeMainRcp) {
				/* XgXwbhȂ̂ŁCXe[W PGM Ǐo	*/
				LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) m_ChiefRecipes.pStageProgStress;
				if (0 == RecipeFile_LoadRecipe(l_pStageProgStress, l_pMainRcpInfo->MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS)) {
					/* Xe[WvOǍُ݈(XgXp)	*/
					return 3;
				}
			}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- } ---------- */
			// SEQ |Cg
			LPSTAGE_COORD l_pScanPoint = (LPSTAGE_COORD) m_ChiefRecipes.pScanPoint;
			if ((l_pStageProgInfoHdr->wNumScans <= 0) || (SCAN_POINT_MAX < l_pStageProgInfoHdr->wNumScans)) {
				// SEQ |Cgݒُ
				return 4;
			//2009.10.28 bagus 2point-distance -->
			} else if (l_pStageProgInfoHdr->wHeadType == HEAD_TYPE_SR && l_pStageProgInfoHdr->wScanType == SCAN_TYPE_SR_DISTANCE){
				if(0 == RecipeFile_Load2PointList(l_pScanPoint, l_pStageProgInfoHdr->wNumScans, l_pMainRcpInfo->MainRcpParam.hdr.szStage)) {
				// SEQ |CgǍُ݈
				LogChief(_T("Failed to StagePoint RecipeFile_Load2PointList() error."));
				return 5;
				}
			} else if (0 == RecipeFile_LoadPointList(l_pScanPoint, l_pStageProgInfoHdr->wNumScans, l_pMainRcpInfo->MainRcpParam.hdr.szStage)) {
			//2009.10.28 bagus 2point-distance <--
				// SEQ |CgǍُ݈
				LogChief(_T("Failed to StagePoint RecipeFile_LoadPointList() error."));
				return 5;
			} else {
				// Qƕ@́C(l_pScanPoint[0]).lX Ȋ
			}
/* added 2009.11.20 hmenjo CTA 1000 |Cg܂łŐ튮 ---------- { ---------- */
			if (HEAD_TYPE_CTA == l_wHeadTypeMainRcp) {
				/* CTA wbh̏ꍇ́C	*/
				if (SCAN_POINT_CTA_MAX < l_pStageProgInfoHdr->wNumScans) {
					/* |Cg̏𐧌܂D	*/
					l_pStageProgInfoHdr->wNumScans = SCAN_POINT_CTA_MAX;
				}
			}
/* added 2009.11.20 hmenjo CTA 1000 |Cg܂łŐ튮 ---------- } ---------- */

// 2009.06.11 K.Matsuo -->

			// Deskew SystemConfig LmF
			SYSTEM_CONFIG *l_SystemConfig = &(((CMainFrame*)m_pcMainFrame)->m_SystemConfig);
			ConfigFile_GetNanoSpecIni(l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			if (l_SystemConfig->nPRMethod <= 0 || l_SystemConfig->bSamplePatternRec != TRUE) {
				// Deskew{Ȃ
				;
			} else {
				//	 Deskew{邩H Xe[Wofl擾
				char szFilePath[MAX_PATH];
				switch(l_pStageProgInfoHdr->nDeskewMode) {
				case DESKEW_MODE_PATTERN_DESKEW:
					// DeskewImage̓o^mF
					_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, l_pStageProgInfoHdr->hdr.szName);
					if ( ::GetFileAttributes(szFilePath) == 0xffffffff ) {
						// Ǎُ݈
						LogChief(_T("Failed to Deskew Image 1 error."));
						return 3;
					}
					break;
					_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, l_pStageProgInfoHdr->hdr.szName);
					if ( ::GetFileAttributes(szFilePath) == 0xffffffff ) {
						// Ǎُ݈
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

			// TCgp^[ SystemConfig LmF
// 2009.06.10 K.Matsuo -->
//			SYSTEM_CONFIG *l_SystemConfig = &(((CMainFrame*)m_pcMainFrame)->m_SystemConfig);
//			ConfigFile_GetNanoSpecIni(l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
//			if (l_SystemConfig->bSitePatternRec != TRUE) {
			if (l_SystemConfig->nPRMethod <= 0 || l_SystemConfig->bSitePatternRec != TRUE) {
// 2009.06.10 K.Matsuo <--
				// TCgp^[{Ȃ
				;
			} else {
				//	 TCgp^[{邩H Xe[Wofl擾
				LPSITE_PATTERN l_pSitePattern = (LPSITE_PATTERN) m_ChiefRecipes.pSitePattern;
				switch(l_pStageProgInfoHdr->nSitePatternMode) {
				case SITE_PR_MODE_USE_PR:					// correct with use pattern recognition
					// TCgp^[ e[u
					if (0 == PatternFile_LoadPointList(l_pSitePattern, &l_pStageProgInfoHdr->wNumScans, l_pMainRcpInfo->MainRcpParam.hdr.szStage)) {
						// SitePR` Ǎُ݈
						LogChief(_T("Failed to SitePr PatternFile_LoadPointList() error."));
						return 3;
					} else {
						// Qƕ@́C(l_pSitePattern[0]).xxxx Ȋ
/* added 2009.06.22 hmenjo SPR ItZbgǍ/݋@\ǉ ---------- { ---------- */
						/* p^[C[Wt@C̑݊mF
							ƁCp^[Tut@C̑݊mF	*/
						/* p^[W */
						TCHAR (*l_ptszPatName)[RECIPE_NAME_LEN + 1] = 0;
						int l_iPatCount = 0;
						LPVOID l_pVoid;
						for (int i = 0; i < l_pStageProgInfoHdr->wNumScans; i++) {
							for (int j = 0; j < SITE_PATTERN_MAX; j++) {
								if (0 != _tcscmp(l_pSitePattern[i].PatternInfo[j].szSitePatternName, _T(""))) {
									/* p^[̓o^D */
									if (0 == l_ptszPatName) {
										/* o^ */
										if (0 == (l_ptszPatName = (TCHAR(*)[RECIPE_NAME_LEN + 1]) malloc(sizeof(*l_ptszPatName)))) {
											/* 擾s */
											LogChief(_T("Failed to allocate memory(s) in RecipesGet()."));
											return 3;
										}
										_tcscpy(l_ptszPatName[l_iPatCount++], l_pSitePattern[i].PatternInfo[j].szSitePatternName);
									} else {
										for (k = 0; k < l_iPatCount; k++) {
											if (0 == _tcscmp(l_ptszPatName[k], l_pSitePattern[i].PatternInfo[j].szSitePatternName)) {
												break;
											}
										}
										if (l_iPatCount <= k) {
											/* ʂ̃p^[𔭌 */
											l_pVoid = realloc(l_ptszPatName, sizeof(*l_ptszPatName) * (l_iPatCount + 1));
											if (0 == l_pVoid) {
												/* Ď擾s */
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
						/* p^[C[Wt@CƃTut@C̑݊mF	*/
						TCHAR l_tszSprImgPath[_MAX_PATH];
						BOOL l_bErr = FALSE;
						SPR_SUB_INFO l_PatSubInfoDmy;
						for (i = 0; i < l_iPatCount; i++) {
//							PatternFile_MakePatternImageFilePath(l_tszSprImgPath, l_ptszPatName[i], 0);
							PatternFile_MakePatternImageFilePath(l_tszSprImgPath, l_ptszPatName[i]);
							if (-1 == ::GetFileAttributes(l_tszSprImgPath)) {
								/* C[Wt@C݂Ȃ\܂D */
								LogChief(_T("Failed to read Site PR image file(s) in RecipesGet()."));
								l_bErr = TRUE;
								break;
							} else if (FALSE == PatternFile_GetSubInfo(l_ptszPatName[i], &l_PatSubInfoDmy)) {
								/* Tut@C݂Ȃ\܂D */
								LogChief(_T("Failed to read Site PR sub-info file(s) in RecipesGet()."));
								l_bErr = TRUE;
								break;
							}
						}
						free(l_ptszPatName);
						if (TRUE == l_bErr) {
							return 3;
						}
/* added 2009.06.22 hmenjo SPR ItZbgǍ/݋@\ǉ ---------- } ---------- */
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
		// ܂D
		break;
	}

	return 0;
}

/*
 *	e탌Vsp̃̎擾ƊJs܂
 */
void CChiefView::RecipesMalloc(BOOL bAlloc)
{
	if (0 != bAlloc) {
		// 擾 ---------------------------------------------------------------
		DWORD l_dwMsgCode = 0;
		// CVs
		if (0 == (m_ChiefRecipes.pMainRcpInfo = new MAIN_RCP_INFO)) {
			l_dwMsgCode = 24;	// 擾s
		}
		// vO
		if (0 == (m_ChiefRecipes.pMeasProgInfo = new MEAS_PROG_INFO)) {
			l_dwMsgCode = 25;	// 擾s
		}
		// Xe[WvO
		if (0 == (m_ChiefRecipes.pStageProgInfoHdr = new STAGE_PROG_INFO_HDR)) {
			l_dwMsgCode = 26;	// 擾s
		}
		// SEQ |Cg
		if (0 == (m_ChiefRecipes.pScanPoint = new STAGE_COORD[SCAN_POINT_MAX])) {
			l_dwMsgCode = 27;	// 擾s
		}
// 2009.05.17 myanagida -->
		// TCgp^[ e[u
		if (0 == (m_ChiefRecipes.pSitePattern = new SITE_PATTERN[SCAN_POINT_MAX])) {
			l_dwMsgCode = 36;	// 擾s
		}
// 2009.05.17 myanagida <--
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- { ---------- */
		/* Xe[WvO XgXp	*/
		if (0 == (m_ChiefRecipes.pStageProgStress = new STAGE_PROG_STRESS)) {
			l_dwMsgCode = 26;	/* 擾s	*/
		}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- } ---------- */
//Saiki 20110308 Add ----->
		// CVs XgXp p[^
		if (0 == (m_ChiefRecipes.pStressParamInfo = new STRESS_PARAM_INFO)) {
			l_dwMsgCode = 24;	// 擾s
		}
//Saiki 20110308 Add <-----
		if (0 != l_dwMsgCode) {
			LogChief(CHIEF_REP_ALARM_MSGTEXT[l_dwMsgCode]);
			::MessageBox(0, CHIEF_REP_ALARM_MSGTEXT[l_dwMsgCode], CHIEF_REP_ALARM_MSGTITLE[1], MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		}
	} else {
		//  ---------------------------------------------------------------
		// CVs (316 bytesF2008.09.30 )
		if (0 != m_ChiefRecipes.pMainRcpInfo) {
			delete m_ChiefRecipes.pMainRcpInfo;
		}
		// vO (984 bytesF2008.09.30 )
		if (0 != m_ChiefRecipes.pMeasProgInfo) {
			delete m_ChiefRecipes.pMeasProgInfo;
		}
		// Xe[WvO (256 bytesF2008.09.30 )
		if (0 != m_ChiefRecipes.pStageProgInfoHdr) {
			delete m_ChiefRecipes.pStageProgInfoHdr;
		}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- { ---------- */
		/* Xe[WvO XgXp	*/
		if (0 != m_ChiefRecipes.pStageProgStress) {
			delete m_ChiefRecipes.pStageProgStress;
		}
/* added 2009.08.03 hmenjo XgX@\ǉ(13) ---------- } ---------- */
		// SEQ |Cg (8 bytes ~ ő|CgF2008.09.30 )
		if (0 != m_ChiefRecipes.pScanPoint) {
			delete m_ChiefRecipes.pScanPoint;
		}
// 2009.05.17 myanagida -->
		// TCgp^[ e[u (62 bytes ~ ő|CgF2009.05.17 )
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
 *	P|CgI bZ[Wnh
 *		f[^W[łP|Cgf[^̏D
 */
LRESULT CChiefView::OnData1PointEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_1POINT_END"), wparam, lparam);

	((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_ANA_DONE);

	return 0L;
}

/*
 *	PI bZ[Wnh
 *		f[^W[łPf[^̏D
 */
LRESULT CChiefView::OnDataMeasEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_MEAS_END"), wparam, lparam);

	((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_COMPLETE);

	return 0L;
}

/*
 *	Y(^[bg)ݒ肵܂
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

/* added 2009.11.26 hmenjo  Seq ͎wwbhœ ---------- { ---------- */
/*
 *	wbhݒ肵܂
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
/* added 2009.11.26 hmenjo  Seq ͎wwbhœ ---------- } ---------- */

// 2013.02.22 bagus Substrate thickness setting -->
BOOL CChiefView::SelectSubstrateThickIndex(int iIndex)
{
	BOOL l_bRc;

	ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	l_bRc = SelectSubThickNotifyIndex(iIndex);
	ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("Selected Substrate Thickness(=%lf), Result(=%d)"), iIndex, l_bRc);
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
	_stprintf(l_tszLogText, _T("Selected Substrate Thickness(=%lf), Result(=%d)"), dVal, l_bRc);
	LogChief(l_tszLogText);

	if (0 == l_bRc) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}

	return l_bRc;
}
// 2013.02.22 bagus Substrate thickness setting <--

/*
 *	AAF Jn bZ[Wnh
 */
LRESULT CChiefView::OnStartAAF(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_AAF"), wparam, lparam);

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMOChAC^bNCup[It
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// {bgA[o
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;	// s_EIt
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;	// GA͒ቺI
	}
	if (
		(0 == m_DiInfo.bTHMaintenanceSW)
	 && ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen))
	) {
		return 37;		// eItŁCCVb^ CLOSE ItCCVb^ OPEN I
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI It	*/
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//[ʒuQ̃ZT[ONĂ
	}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// [JłȂ
	}
	int l_iProcStatus = ProcStatusGet();
	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
		return 8L;		// WaitCDown ȊO
	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if (MAIN_MENU_DATA == l_iDispStatus) {
		return 11L;		// ʃ[huf[^[hv
	}
	if ((MAIN_MENU_USER_SETTING == l_iDispStatus) && (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll())) {
		return 12L;		// ʃ[hu[Uݒ胂[hv̏ꍇɁC쒆tOׂăItłȂ
	}
#if 0	// ^[bg쒆tOɒǉ߁C^[bg܂߂܂
	if (((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_RECIPE_SETTING == l_iDispStatus))
	 && (0 != (0x00000016 & ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()))) {
#else	// ^[bg쒆tOɒǉ߁C^[bg܂߂܂
	if (((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_RECIPE_SETTING == l_iDispStatus))
	 && (0 != ((ACTUATE_ZAXIS | ACTUATE_PIN | ACTUATE_WORKGUIDE | ACTUATE_TURRET) & ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()))) {
#endif	// ^[bg쒆tOɒǉ߁C^[bg܂߂܂
		return 13L;		// ʃ[hu胂[hvuVs[hv̏ꍇɓ쒆tOŁuXY Xe[WvƁuVb^vȊOɃI
	}

	((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UIAAF);

	return 0L;
}

/*
 *	ʂ̃V[PXJn bZ[Wnh
 */
LRESULT CChiefView::OnStartDispSeq(WPARAM wparam, LPARAM lparam)
/* added 2009.09.09 hmenjo Rgǉ ---------- { ---------- */
/*				LOWORD(lparam)FW[[h
/*									OFʏ탂[h
/*									OFW[[h(t@XƃfXL[)
/*				HIWORD(lparam)FPMA w
/*									OFCVsʂ
/*									PFPMA 
/*									QFPMA L(S_)
/*									RFCVsʂ
/* added 2009.09.09 hmenjo Rgǉ ---------- } ---------- */
{
	LogChief_WinMsg(_T("WM_DISP_START_SEQ"), wparam, lparam);

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMOChAC^bNCup[It
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;		// {bgA[o
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;		// s_EIt
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;		// GA͒ቺI
	}
	if ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen)) {
		return 37;		// Vb^ CLOSE ItCCVb^ OPEN I
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI It	*/
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//[ʒuQ̃ZT[ONĂ
	}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// [JłȂ
	}
	if (PROCESS_WAIT != ProcStatusGet()) {
		return 5L;		// Wait łȂ
	}
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		return 6L;		// 쒆tOI
	}
	if (MAIN_MENU_MEASUREMENT != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus()) {
		return 7L;		// ʃ[hu胂[hvłȂ
	}
	if (0 == m_DiInfo.bMaintenanceSW) {
		// eiX SW ÍEEE
		if (0 == m_DiInfo.bTHMaintenanceSW) {
			// ɁCe SW It̏ꍇC֎~
			return 15;
		}
#if 1	// ełȂCȅꍇɓ֎~ɂ܂
	} else {
		// eiX SW It́EEE
		if (0 != m_DiInfo.bTHMaintenanceSW) {
			// e SW ȈꍇC֎~
			return 15;
		}
#endif
	}

	m_bGotRecipeFromPif = FALSE;
	// Vs擾
	_tcscpy(m_szMainRecipeName, (TCHAR*) wparam);
	//		wparam  CMainFrame CCVs̃|C^ZbgĂ͂łD
	if (0 != RecipesGetCheck(m_szMainRecipeName, 0)) {
		// VsǍُ݈Ȃ̂ŁC܂D
	} else {
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
		if (0 == HeadTypeCheck(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
			// wbh^CvłD
			return 16;
		} else {
/* added 2009.09.11 hmenjo Ot@Xf[^mF ---------- { ---------- */
			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 && (0 != this->CheckReferenceData(	m_szMainRecipeName,
													l_pMeasProgInfo->Ref.hdr.dLifeTime,
													l_pMeasProgInfo->Ref2nd.bMeasure))) {
				/* KgߗŁCt@Xf[^ُ
					(t@Xf[^t@C݂ȂC܂́CLO)	*/
				return 48;
			}
/* added 2009.09.11 hmenjo Ot@Xf[^mF ---------- } ---------- */
/* added 2009.09.11 hmenjo OKgߗvmF ---------- { ---------- */
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 && (TRUE != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsGTRLampOn())) {
				/* KgߗŁCKgߗpnQvIt	*/
				return 49;
			}
/* added 2009.09.11 hmenjo OKgߗvmF ---------- } ---------- */
#ifdef CHIEF_REMEASURE_ON	// W[Ή 20081225
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
				// 肷/Ȃ tOׂāuȂvɂȂĂ܂
				return 38;
			} else {
				// Tv ID "0x0dC0x00"Ă(ʑœ͂Ă邽)
				m_szSampleID[0] = 0x000d;
				m_szSampleID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA bg ID Ή ---------- { ---------- */
				/* bg ID "0x0dC0x00"Ă(ʑœ͂Ă邽)	*/
				m_szLotID[0] = 0x000d; m_szLotID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA bg ID Ή ---------- } ---------- */
				// V[PXJn -----------------------------------------
				// PMA w(Kv΃Vs܂)
				switch (HIWORD(lparam)) {
				case 1:		// PMA 
					((LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_NONE;
					break;
				case 2:		// PMA L(S_)
					((LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo)->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_ALL_POINT_EVERY_ADJUSTMENT;
					break;
				case 0:		// CVsʂ
				default:	// CVsʂ
					// CVsʂł̂ŃVs͕ύX܂D
					break;
				}
/* added 2009.11.30 hmenjo  Seq ͎wwbh()œ ---------- { ---------- */
				if (POINT_MANUAL_ADJUSTMENT_NONE != l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment) {
					/* MS wbhȊO PMA L̏ꍇ AF(1|Cgڂ)ɐݒ肵܂D	*/
					switch (l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
					case HEAD_TYPE_SR:
					case HEAD_TYPE_STRESS:	l_pMainRcpInfo->MainRcpParam._SR.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
					case HEAD_TYPE_SE:		l_pMainRcpInfo->MainRcpParam._SE.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._SE.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
// 2013.02.01 bagus CompleteEASEwbhǉ -->
					case HEAD_TYPE_COMPEASE:l_pMainRcpInfo->MainRcpParam._COMPEASE.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._COMPEASE.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
// 2013.02.01 bagus CompleteEASEwbhǉ <--
					case HEAD_TYPE_4PP:		l_pMainRcpInfo->MainRcpParam._RS.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._RS.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
					case HEAD_TYPE_CTA:		l_pMainRcpInfo->MainRcpParam._CA.nFocus = 1;
											l_pMainRcpInfo->MainRcpParam._CA.nAutoFocusFailOption = AFOCUS_FAIL_OPTION_PROCEED;	break;
					case HEAD_TYPE_MS:
					default:
						break;
					}
				}
/* added 2009.11.30 hmenjo  Seq ͎wwbh()œ ---------- } ---------- */
/* modified 2009.09.10 hmenjo P511 R}hǉ ---------- { ---------- */
//				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) LOWORD(lparam));
/* modified 2009.09.10 hmenjo P511 R}hǉ ----------				*/
				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) MAKELONG(LOWORD(lparam), 0));
/* modified 2009.09.10 hmenjo P511 R}hǉ ---------- } ---------- */
			}
#else
			// Tv ID "0x0dC0x00"Ă(ʑœ͂Ă邽)
			m_szSampleID[0] = 0x000d;
			m_szSampleID[1] = 0x0000;
			// V[PXJn
			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ);
#endif
		}
	}

	return 0L;
}

/*
 *	ʂ̂P|CgJn bZ[Wnh
 */
LRESULT CChiefView::OnStart1Point(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_1POINT"), wparam, lparam);

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMOChAC^bNCup[It
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// {bgA[o
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;	// s_EIt
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;	// GA͒ቺI
	}
	if ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen)) {
		return 37;		// Vb^ CLOSE ItCCVb^ OPEN I
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI It	*/
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//[ʒuQ̃ZT[ONĂ
	}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// [JłȂ
	}
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		return 6L;		// 쒆tOI
	}
//	int l_iProcStatus = ProcStatusGet();
//	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
//		return 8L;		// WaitCDown ȊO
//	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
// modified hmenjo 2009.01.25 ---- { ----
//	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)) {
//		return 9L;		// ʃ[hu胂[hvuVs[hvłȂ
//	}
//	if ((MAIN_MENU_MEASUREMENT == l_iDispStatus) && (PROCESS_WAIT != l_iProcStatus)) {
//		return 10L;		// ʃ[hu胂[hv̏ꍇ Wait ԂłȂ
//	}
// modified hmenjo 2009.01.25 ----
	if (
		(MAIN_MENU_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_MANUAL_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)
		) {
		return 9L;		// ʃ[hu胂[hvu}jA胂[hvuVs[hvłȂ
	}
//	if (
//		((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_MANUAL_MEASUREMENT == l_iDispStatus))
//	 && (PROCESS_WAIT != l_iProcStatus)
//		) {
//		return 10L;		// ʃ[hu胂[hvu}jA胂[hv̏ꍇ Wait ԂłȂ
//	}
// modified hmenjo 2009.01.25 ----	----

	m_bGotRecipeFromPif = FALSE;
	// Vs擾
	_tcscpy(m_szMainRecipeName, (TCHAR*) wparam);
	//		wparam  CMainFrame CCVs̃|C^ZbgĂ͂łD
	if (0 != RecipesGetCheck(m_szMainRecipeName, 1)) {
		// VsǍُ݈Ȃ̂ŁC܂D
	} else {
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
		if (0 == HeadTypeCheck(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
			// wbh^CvłD
			return 16;
		} else {
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- { ---------- */
///* modified 2009.11.27 hmenjo P|Cgő|Cgԍ ---------- { ---------- */
////			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT);
///* modified 2009.11.27 hmenjo P|Cgő|Cgԍ ---------- 			 */
//			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT, (cEventParams*) LOWORD(lparam));
///* modified 2009.11.27 hmenjo P|Cgő|Cgԍ ---------- } ---------- */
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- 			 */
			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT, (cEventParams*) MAKELONG(LOWORD(lparam), 0));
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- } ---------- */
		}
	}

	return 0L;
}

/*
 *	ʂ̃t@XJn bZ[Wnh
 */
LRESULT CChiefView::OnStartSrRefer(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_START_SRREF"), wparam, lparam);

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMOChAC^bNCup[It
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// {bgA[o
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
		return 35;	// s_EIt
	}
	if (0 != CheckDIO_IsAirPressureLowON()) {
		return 36;	// GA͒ቺI
	}
	if ((0 == m_DiInfo.bShutterClose) || (0 != m_DiInfo.bShutterOpen)) {
		return 37;		// Vb^ CLOSE ItCCVb^ OPEN I
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI It	*/
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//[ʒuQ̃ZT[ONĂ
	}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// [JłȂ
	}
	if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGetAll()) {
		return 6L;		// 쒆tOI
	}
//	int l_iProcStatus = ProcStatusGet();
//	if ((PROCESS_WAIT != l_iProcStatus) && (PROCESS_DOWN != l_iProcStatus)) {
//		return 8L;		// WaitCDown ȊO
//	}
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
// modified hmenjo 2009.01.25 ---- { ----
//	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)) {
//		return 9L;		// ʃ[hu胂[hvuVs[hvłȂ
//	}
//	if ((MAIN_MENU_MEASUREMENT == l_iDispStatus) && (PROCESS_WAIT != l_iProcStatus)) {
//		return 10L;		// ʃ[hu胂[hv̏ꍇ Wait ԂłȂ
//	}
// modified hmenjo 2009.01.25 ----
	if (
		(MAIN_MENU_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_MANUAL_MEASUREMENT != l_iDispStatus)
	 && (MAIN_MENU_RECIPE_SETTING != l_iDispStatus)
		) {
		return 9L;		// ʃ[hu胂[hvu}jA胂[hvuVs[hvłȂ
	}
//	if (
//		((MAIN_MENU_MEASUREMENT == l_iDispStatus) || (MAIN_MENU_MANUAL_MEASUREMENT == l_iDispStatus))
//	 && (PROCESS_WAIT != l_iProcStatus)
//		) {
//		return 10L;		// ʃ[hu胂[hvu}jA胂[hv̏ꍇ Wait ԂłȂ
//	}
// modified hmenjo 2009.01.25 ----	----
	if (0 == m_DiInfo.bMaintenanceSW) {
		// eiX SW ÍEEE
		if (0 == m_DiInfo.bTHMaintenanceSW) {
			// ɁCe SW It̏ꍇC֎~
			return 15;
		}
#if 1	// ełȂCȅꍇɓ֎~ɂ܂
	} else {
		// eiX SW It́EEE
		if (0 != m_DiInfo.bTHMaintenanceSW) {
			// e SW ȈꍇC֎~
			return 15;
		}
#endif
	}

	m_bGotRecipeFromPif = FALSE;
	// Vs擾
	_tcscpy(m_szMainRecipeName, (TCHAR*) wparam);
	//		wparam  CMainFrame CCVs̃|C^ZbgĂ͂łD
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- { ---------- */
//	if (0 != RecipesGetCheck(m_szMainRecipeName, 1)) {
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- 			 */
	/* wbhƃXL^CvmF̂߁CXe[WvOœǍ݁D	*/
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
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- } ---------- */
		// VsǍُ݈Ȃ̂ŁC܂D
	} else {
/* added 2009.09.11 hmenjo ʂKgߗN ---------- { ---------- */
		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			&& (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
			/* Kgߗt@XȂ̂ŃXe[WvO܂߂ăVsǂݒ܂D	*/
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- { ---------- */
//			if (0 != RecipesGetCheck(m_szMainRecipeName, 0)) {
//				/* VsǍُ݈Ȃ̂ŁC܂D	*/
//				return 0L;
//			}
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- 			 */
			if (0 == LOWORD(lparam)) {
				/* GTR ʏ탊t@X(S|Cg)̏ꍇ̂	*/
				if (0 != RecipesGetCheck(m_szMainRecipeName, 0)) {
					/* VsǍُ݈Ȃ̂ŁC܂D	*/
					return 0L;
				}
			}
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- } ---------- */
		}
/* added 2009.09.11 hmenjo ʂKgߗN ---------- } ---------- */
		LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) m_ChiefRecipes.pMainRcpInfo;
		if (0 == HeadTypeCheck(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
			// wbh^CvłD
			return 16;
		} else {
/* added 2009.11.30 hmenjo  Seq ͎wwbh()œ ---------- { ---------- */
			/* PMA ݒ͖ɂ܂D	*/
			l_pMainRcpInfo->MainRcpParam.hdr.nPointManualAdjustment = POINT_MANUAL_ADJUSTMENT_NONE;
/* added 2009.11.30 hmenjo  Seq ͎wwbh()œ ---------- } ---------- */
/* added 2009.09.11 hmenjo OKgߗvmF ---------- { ---------- */
//			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 && (TRUE != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsGTRLampOn())) {
				/* Kgߗt@XŁCKgߗpnQvIt	*/
				return 49;
			}
/* added 2009.09.11 hmenjo OKgߗvmF ---------- } ---------- */
/* modified 2009.09.11 hmenjo ʂKgߗN ---------- { ---------- */
//			((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISREF);
/* modified 2009.09.11 hmenjo ʂKgߗN ---------- 			 */
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
				/* SR Kgߗt@X	*/
				/* 肷/Ȃ tOׂăZbg܂D	*/
				LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) m_ChiefRecipes.pStageProgInfoHdr;
				for (int i = 0; i < l_pStageProgInfoHdr->wNumScans; i++) {
					((CNanoSpecDoc*) m_pcNanoSpecDoc)->m_bDoPointMeasFlag[i] = TRUE;
				}
				/* Tv ID "0x0dC0x00"Ă(ʑœ͂Ă邽)	*/
				m_szSampleID[0] = 0x000d;
				m_szSampleID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA bg ID Ή ---------- { ---------- */
				/* bg ID "0x0dC0x00"Ă(ʑœ͂Ă邽)	*/
				m_szLotID[0] = 0x000d; m_szLotID[1] = 0x0000;
/* added 2009.10.29 hmenjo CTA bg ID Ή ---------- } ---------- */
				/* t@X[hŋN܂D	*/
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- { ---------- */
//				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) MAKELONG(0, TRUE));
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- 			 */
				if (0 == LOWORD(lparam)) {
					/* GTR ʏ탊t@X(S|Cg)	*/
					((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISEQ, (cEventParams*) MAKELONG(0, TRUE));
				} else {
					/* GTR P|Cg t@X	*/
					((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UI1POINT, (cEventParams*) MAKELONG(LOWORD(lparam), 1));
				}
/* modified 2009.11.27 hmenjo GTR P|Cgt@X ---------- } ---------- */
			} else {
				/*	SR t@X
				 *	SR ʏ퓧ߗt@X
				 */
				((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DO_UISREF);
			}
/* modified 2009.09.11 hmenjo ʂKgߗN ---------- } ---------- */
		}
	}

	return 0L;
}

// 2013.11.07 Bagus Add (TohoSpecΉ) -->
/*
 *	mFʌʃbZ[Wnh
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
// 2013.11.07 Bagus Add (TohoSpecΉ) <--

/*
 *	e탌VsǍ݃G[`FbN܂
 *		Ǎُ݈̕młD
 */
DWORD CChiefView::RecipesGetCheck(TCHAR *pszMainRecipeName, DWORD dwMode)
{
	DWORD l_dwResult;
	DWORD l_dwMsgTextCode;
	DWORD l_dwALID;

	l_dwResult = RecipesGet(pszMainRecipeName, dwMode);

	switch (l_dwResult) {
	case 0:		// 
		break;
	case 1:		// CVsǍُ݈
		l_dwMsgTextCode = 28;
		l_dwALID = ALID_MainRecipeReadError;
		break;
	case 2:		// vOǍُ݈
		l_dwMsgTextCode = 29;
		l_dwALID = ALID_MeasurementProgramReadError;
		break;
	case 3:		// Xe[WvOǍُ݈
		l_dwMsgTextCode = 30;
		l_dwALID = ALID_StageProgramReadError;
		break;
	case 4:		// SEQ |Cgݒُ
		l_dwMsgTextCode = 31;
		l_dwALID = ALID_SequenceMeasurementPointsSettingError;
		break;
	case 5:		// SEQ |CgǍُ݈
		l_dwMsgTextCode = 32;
		l_dwALID = ALID_SequenceMeasurementPointReadError;
		break;
	default:	// ̑`ُ̈
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
 *	|[Y{^ bZ[Wnh
 */
LRESULT CChiefView::OnSeqPause(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_PAUSE_SEQ"), wparam, lparam);

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
/* modified 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
///* modified 2009.06.02 hmenjo hAJő|[Y ---------- { ---------- */
////		return 30 + l_dwEMO;	// EMOChAC^bNCup[It
///* modified 2009.06.02 hmenjo hAJő|[Y ----------			   */
//		if (ALID_DoorOpen != l_dwEMO) {
//			return 30 + l_dwEMO;	// EMOChAC^bNCup[It
//		}
///* modified 2009.06.02 hmenjo hAJő|[Y ---------- } ---------- */
/* modified 2009.08.17 hmenjo Z[teBvOǉ ----------			   */
		if ((ALID_DoorOpen != l_dwEMO) && (ALID_SafetyPlugOpen != l_dwEMO)) {
			return 30 + l_dwEMO;	// EMOChAC^bNCup[It
		}
/* modified 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// {bgA[o
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- { ---------- */
//		return 35;	// s_EIt
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ----------			   */
		if (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle()) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
//			if ((0 == m_DiInfo.bPinDown) && (0 != nexioIsPinAlignmentPos()) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ----------			   */
			if ((0 == m_DiInfo.bPinDown) && (0 != m_DiInfo.bPinAlign) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */
				;	/* sԈʒuZT݂̂I OK łD	*/
			} else {
				return 35;	/* sԈʒuZTIt	*/
			}
		} else {
			return 35;	/* s_EIt	*/
		}
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- } ---------- */
	}
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// [JłȂ
	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- { ---------- */
//	if ((PROCESS_PROC != ProcStatusGet())
//	 || (true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		return 3L;		// Processing łȂ
//	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	if (0 == PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
		return 16L;	/* wbh^Cvُ	*/
	}
	if ((PROCESS_PROC != ProcStatusGet())
	 || (true == PFC_IsIdleAll(this))) {
		return 3L;		// Processing łȂ
	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- } ---------- */
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		return 4L;		// ʃ[hu胂[hvuf[^[hvłȂ
	}

/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- { ---------- */
//	((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ----------			   */
	/* |[YCxg𔭍s	*/
	(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iPAUSE, 0);
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- } ---------- */

	return 0L;
}

/*
 *	背W[{^ bZ[Wnh
 */
LRESULT CChiefView::OnSeqResume(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_RESUME_SEQ"), wparam, lparam);

	// C^bN
	DWORD l_dwEMO = CheckDIO_IsEMO();
	if (0 != l_dwEMO) {
		return 30 + l_dwEMO;	// EMOChAC^bNCup[It
	}
	if (0 != CheckDIO_IsRobotArmON()) {
		return 34;	// {bgA[o
	}
	if (0 != CheckDIO_IsPinDownOFF()) {
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- { ---------- */
//		return 35;	// s_EIt
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ----------			   */
		if (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle()) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
//			if ((0 == m_DiInfo.bPinDown) && (0 != nexioIsPinAlignmentPos()) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ----------			   */
			if ((0 == m_DiInfo.bPinDown) && (0 != m_DiInfo.bPinAlign) && (0 == m_DiInfo.bPinUp)) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */
				;	/* sԈʒuZT݂̂I OK łD	*/
			} else {
				return 35;	/* sԈʒuZTIt	*/
			}
		} else {
			return 35;	/* s_EIt	*/
		}
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- } ---------- */
	}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
	if (0 == this->IsCtaILPI()) {
		return 50;	/* CTAILPI It	*/
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
	int iResistStatus = this->CheckResistIL();
	if (0 != iResistStatus) {
		LONG lCode = 52 + iResistStatus - 1;		// 52,53,54
		return lCode;
	}
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
	if( 0 == this->IsMSILPI()){
		return 55;		//[ʒuQ̃ZT[ONĂ
	}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */
/* deleted 2009.06.02 hmenjo hAJő|[Y ---------- { ---------- */
//	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
//		return 2L;		// [JłȂ
//	}
/* deleted 2009.06.02 hmenjo hAJő|[Y ---------- } ---------- */
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- { ---------- */
//	if ((PROCESS_PROC != ProcStatusGet())
//	 || (true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		return 3L;		// Processing łȂ
//	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	if (0 == PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
		return 16L;	/* wbh^Cvُ	*/
	}
	if ((PROCESS_PROC != ProcStatusGet())
	 || (true == PFC_IsIdleAll(this))) {
		return 3L;		// Processing łȂ
	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- } ---------- */
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		return 4L;		// ʃ[hu胂[hvuf[^[hvłȂ
	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- { ---------- */
///* added 2009.06.08 hmenjo |[YO Resume s΍ ---------- { ---------- */
//	/* V[PX肪|[Y̏ꍇ̂݃W[\łD */
//	if (ST_SEQ_PAUSE != ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentState()) {
//		return -2L;		/* |[YÕW[w߂łD */
//	}
///* added 2009.06.08 hmenjo |[YO Resume s΍ ---------- } ---------- */
//
//	((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_RESUME);
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ----------			   */
	if (l_ChiefPFuncs.State.iPAUSE != (*l_ChiefPFuncs.GetCurrentState)(this)) {
		return -2L;		/* |[YÕW[w߂łD */
	}
	(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iRESUME, 0);
/* modified 2009.08.07 hmenjo XgX@\ǉ(35) ---------- } ---------- */

	return 0L;
}

/*
 *	LZ{^ bZ[Wnh
 */
LRESULT CChiefView::OnSeqCancel(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_CANCEL_SEQ"), wparam, lparam);

	// C^bN
#if 0	// hmenjo [g Pif 瑪蒆łʂ̑LZ͎t܂D
	if (HOST_LOCAL != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetHostMode()) {
		return 2L;		// [JłȂ
	}
#endif
/* modified 2009.08.06 hmenjo XgX@\ǉ(32) ---------- { ---------- */
//	if ((PROCESS_PROC != ProcStatusGet())
//	 || (true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		return 3L;		// Processing łȂ
//	}
/* modified 2009.08.06 hmenjo XgX@\ǉ(32) ----------			   */
	CHIEF_PFUNCS l_ChiefPFuncs;
	if (0 == PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
		return 16L;	/* wbh^Cvُ	*/
	}
	if ((PROCESS_PROC != ProcStatusGet())
	 || (true == PFC_IsIdleAll(this))) {
		return 3L;		// Processing łȂ
	}
/* modified 2009.08.06 hmenjo XgX@\ǉ(32) ---------- } ---------- */
#if 0	// hmenjo ʂ̃LZ͂łt܂
	int l_iDispStatus = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus();
	if ((MAIN_MENU_MEASUREMENT != l_iDispStatus) && (MAIN_MENU_DATA != l_iDispStatus)) {
		return 4L;		// ʃ[hu胂[hvuf[^[hvłȂ
	}
#endif

/* modified 2009.08.06 hmenjo XgX@\ǉ(32) ---------- { ---------- */
//	if (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle()) {
//		// LZCxg𔭍s
//		((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_CANCEL);
//		m_bReqCancelComplete = TRUE;	// LZ񍐎wߗL
//		// LZ{^
//		PifComm_PressMeasureCancelButtonReport();
//		return 0L;
//	} else {
//		return 17L;		// V[PXgWVACh
//	}
/* modified 2009.08.06 hmenjo XgX@\ǉ(32) ----------			   */
	if (false == (*l_ChiefPFuncs.IsIdle)(this)) {
		// LZCxg𔭍s
		(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iCANCEL, 0);
		m_bReqCancelComplete = TRUE;	// LZ񍐎wߗL
		// LZ{^
		PifComm_PressMeasureCancelButtonReport();
		return 0L;
	} else {
		return 17L;		// V[PXgWVACh
	}
/* modified 2009.08.06 hmenjo XgX@\ǉ(32) ---------- } ---------- */
}

/*
 *	LZ
 *		LZ̊ Pif ɕ񍐂܂D
 */
void CChiefView::ReportCancelComplete(int iResult)
{
	if (m_bReqCancelComplete) {
		m_bReqCancelComplete = FALSE;
		PifComm_MeasureCancelResultReport(iResult);
	}
}

/*
 *	vZXXe[^XZbg
 *		Chief ȉ̃W[̃ANZX(R[h)ȒPɂ邽߂Ƀbv܂
 */
void CChiefView::ProcStatusSet(int iStatus)
{
	// Xe[^X̕ω𐧌܂
// modified hmenjo 2009.01.23 ---- { ----
//	if (PROCESS_DOWN == ProcStatusGet()) {
//		// DOWN ԂꍇEEE
//		switch (iStatus) {
//		case PROCESS_PROC:
//		case PROCESS_COMP:
//		case PROCESS_ABRT:
//		case PROCESS_WAIT:
//			return;			// ܂D
//			break;
//		default:
//			break;
//		}
// modified hmenjo 2009.01.23 ----
	if ((PROCESS_DOWN == ProcStatusGet()) || (PROCESS_INIT == ProcStatusGet())) {
// modified hmenjo 2009.01.23 ---- } ----
		// DOWN or INIT ԂꍇEEE
		return;			// ܂D
	}
	// Xe[^XɕύXꍇ́C܂D
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
 *	vZXXe[^X擾
 *		Chief ȉ̃W[̃ANZX(R[h)ȒPɂ邽߂Ƀbv܂
 */
int CChiefView::ProcStatusGet()
{
	return ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetProcessStatus();
}

/*
 *	쒆tOZbg܂
 *		Chief ȉ̃W[̃ANZX(R[h)ȒPɂ邽߂Ƀbv܂
 */
void CChiefView::ActuateFlagsSet(ACTUATE_FLAGS_SEL ActFgSel, BOOL bFlag)
{
	((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsSet(ActFgSel, bFlag);
}

/* modified 2009.07.23 hmenjo wbh^Cv`FbN֐ύX ---------- { ---------- */
///*
// *	wbh^Cv`FbN܂
// *		2008.10.27 ݂ SR ̂ݗLłD
// */
//BOOL CChiefView::HeadTypeCheck(WORD wHeadType)
//{
//	BOOL l_bRc;
//
//	// wbh^Cṽ`FbN
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
/* modified 2009.07.23 hmenjo wbh^Cv`FbN֐ύX ----------			   */
/*
 *	wbh^Cv`FbN܂
 */
BOOL CChiefView::HeadTypeCheck(WORD wHeadType)
{
	return ((CNanoSpecDoc*) m_pcNanoSpecDoc)->IsValidHeadType(wHeadType);
}
/* modified 2009.07.23 hmenjo wbh^Cv`FbN֐ύX ---------- } ---------- */

/*
 *	Chief_CAO\ bZ[Wnh
 */
LRESULT CChiefView::OnDlgShowSW(WPARAM wparam, LPARAM lparam)
{
	switch (wparam) {
	case 0:
		GetParentFrame()->ShowWindow(SW_HIDE);		// \
		break;
	default:
		GetParentFrame()->ShowWindow(SW_SHOWNORMAL);	// \
		GetParentFrame()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	}

	return 0L;
}

/*
 *	DIO ͂`FbN | ~n̓͂`FbN
 *			EMO					FALSE
 *			hAC^bN	FALSE
 *			up[It		FALSE
 *		o܂D
 */
DWORD CChiefView::CheckDIO_IsEMO()
{
	DWORD l_dwRc;

	if (0 == m_DiInfo.bEMOStop) {
		l_dwRc = ALID_EMOSwitchOn;
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
	} else if ((0 == m_DiInfo.bSafetyPlug) && (0 == m_DiInfo.bTHMaintenanceSW)) {
		l_dwRc = ALID_SafetyPlugOpen;
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
	} else if ((0 == m_DiInfo.bDoorInterlock) && (0 == m_DiInfo.bTHMaintenanceSW)) {
		l_dwRc = ALID_DoorOpen;
// added hmenjo 2009.05.12 hAJotOǉ ---------- { ----------
		// eŁChAJo֎~̂Ƃُ͈ɂ܂D
		if ((0 == m_DiInfo.bMaintenanceSW) && (FALSE == g_bIL_DoorOpen)) {
			l_dwRc = 0;
		}
// added hmenjo 2009.05.12 hAJotOǉ ---------- } ----------
	} else if (0 == m_DiInfo.bEQPower) {
		l_dwRc = ALID_EquipmentPowerOff;
	} else {
		l_dwRc = 0;
	}

	return l_dwRc;
}

/*
 *	DIO ͂`FbN | {bgA[ o
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
 *	DIO ͂`FbN | s_E o
 */
BOOL CChiefView::CheckDIO_IsPinDownOFF()
{
	if (0 != m_DioIgnoreSW.bPinDown) {
		return FALSE;
	}

	if (0 == m_DiInfo.bPinDown) {
		return TRUE;		// s~[ZTIt
	}
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
//	if (0 != nexioIsPinAlignmentPos()) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ----------			   */
	if (0 != m_DiInfo.bPinAlign) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */
		return TRUE;		// sԈʒuZTI
	}
	if (0 != m_DiInfo.bPinUp) {
		return TRUE;		// s㏸[ZTI
	}

	return FALSE;
}

/*
 *	DIO ͂`FbN | GA͒ቺ o
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
 * DIO ͂`FbN | X1A[o
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
 * DIO ͂`FbN | X2A[o
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
 * DIO ͂`FbN | YA[o
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
 *	ُ탊Zbgw bZ[Wnh
 *		ʂȂǂُ̈(A[)Zbgł
 */
LRESULT CChiefView::OnResetAlarm(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_CHIF_RESET_ALARM"), wparam, lparam);

/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- { ---------- */
	this->CtaReset(2);	/* CTA Zbg	*/
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- } ---------- */

/* modified 2009.06.02 hmenjo _EA[Zbg ---------- { ---------- */
////	if (PROCESS_DOWN == ProcStatusGet()) {
////		// Down Ԃ̏ꍇ̂ݎs܂
//	if ((0 != ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) && (PROCESS_DOWN == ProcStatusGet())) {
/* modified 2009.06.02 hmenjo _EA[Zbg ---------- 			 */
	if (PROCESS_DOWN == ProcStatusGet()) {
/* modified 2009.06.02 hmenjo _EA[Zbg ---------- } ---------- */
		// n[hV~[g[hŁCDown Ԃ̏ꍇ̂ݎs܂
		// A[LtOZbg
		if (0 != m_AlarmFlags.bAirPressureLowOn)	{m_AlarmFlags.bAirPressureLowOn		= FALSE;	AlarmIf_Reset(ALID_AirPressureDown);}
		if (0 != m_AlarmFlags.bMaintenanceSWOn)		{m_AlarmFlags.bMaintenanceSWOn		= FALSE;	AlarmIf_Reset(ALID_MaintenanceSwitchOn);}
		if (0 != m_AlarmFlags.bPinDownOff)			{m_AlarmFlags.bPinDownOff			= FALSE;	AlarmIf_Reset(ALID_LifterLowestPositionSensorOff);}
		if (0 != m_AlarmFlags.bRobotArmOn)			{m_AlarmFlags.bRobotArmOn			= FALSE;	AlarmIf_Reset(ALID_RobotArmDetected);}
		if (0 != m_AlarmFlags.bVacuumPressure1Off)	{m_AlarmFlags.bVacuumPressure1Off	= FALSE;	AlarmIf_Reset(ALID_VacuumSensorOff);}
		if (0 != m_AlarmFlags.bWorkGuideOpenOff)	{m_AlarmFlags.bWorkGuideOpenOff		= FALSE;	AlarmIf_Reset(ALID_WorkGuideOpenSensorOff);}
		if (0 != m_AlarmFlags.bShutterCloseOff)		{m_AlarmFlags.bShutterCloseOff		= FALSE;	AlarmIf_Reset(ALID_ShutterCloseError);}
/* 2009.08.20 K.Matsuo ALID `ύX ---------- { ---------- */
///* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- { ---------- */
//		if (0 != m_AlarmFlags.bRetractZPosOff)		{m_AlarmFlags.bRetractZPosOff		= FALSE;	AlarmIf_Reset(ZPOSOFF_ALID);}
///* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- } ---------- */
/* 2009.08.20 K.Matsuo ALID `ύX ----------			   */
		if (0 != m_AlarmFlags.bRetractZPosOff)		{m_AlarmFlags.bRetractZPosOff		= FALSE;	AlarmIf_Reset(ALID_Z_PositionRetractFailed);}
/* 2009.08.20 K.Matsuo ALID `ύX ---------- } ---------- */
// 2013.01.10 bagus stage driver alarm io -->
		if (0 != m_AlarmFlags.bStageAlarmOn)		{m_AlarmFlags.bStageAlarmOn			= FALSE;	AlarmIf_Reset(ALID_StageError);}
		if (0 != m_AlarmFlags.bStageOverTravelOn)	{m_AlarmFlags.bStageOverTravelOn	= FALSE;	AlarmIf_Reset(ALID_StageOverTravelError);}
		if (0 != m_AlarmFlags.bStageUnknownOn)		{m_AlarmFlags.bStageUnknownOn		= FALSE;	AlarmIf_Reset(ALID_StageUnknownError);}
// 2013.01.10 bagus stage driver alarm io -->

		DWORD l_dwEMO = CheckDIO_IsEMO();
		if (0 != l_dwEMO) {
			// Down Ԃ𑱍s
			ProcStatusSet(PROCESS_DOWN);	// Ô Down Zbg
			switch (l_dwEMO) {
			case ALID_EMOSwitchOn:		// EMO
				if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
				if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
				if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
				// A[ʒm
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EMOSwitchOn));
				break;
			case ALID_DoorOpen:		// hAC^bN
				if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
				if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
				if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
				// A[ʒm
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_DoorOpen));
				break;
			case ALID_EquipmentPowerOff:		// up[It
				if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
				if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
				if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
				// A[ʒm
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EquipmentPowerOff));
				break;
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
			case ALID_SafetyPlugOpen:		/* Z[teBvO	*/
				if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
				if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
				if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
				/* A[ʒm	*/
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_SafetyPlugOpen));
				break;
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
			default:
				// ܂D
				break;
			}
		} else {
			if (0 != m_AlarmFlags.bEMOStopOn)		{m_AlarmFlags.bEMOStopOn		= FALSE;	AlarmIf_Reset(ALID_EMOSwitchOn);}
			if (0 != m_AlarmFlags.bDoorInterlockOn)	{m_AlarmFlags.bDoorInterlockOn	= FALSE;	AlarmIf_Reset(ALID_DoorOpen);}
			if (0 != m_AlarmFlags.bEQPowerOn)		{m_AlarmFlags.bEQPowerOn		= FALSE;	AlarmIf_Reset(ALID_EquipmentPowerOff);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
			if (0 != m_AlarmFlags.bSafetyPlugOn)	{m_AlarmFlags.bSafetyPlugOn		= FALSE;	AlarmIf_Reset(ALID_SafetyPlugOpen);}
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
			// Wait ɂ܂D
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
/* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- { ---------- */
		if (0 != m_AlarmFlags.bRetractZPosOff)		{m_AlarmFlags.bRetractZPosOff		= FALSE;}
/* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- } ---------- */
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
 *	DIO ͂`FbN | 쒆ُ̈͂`FbN
 *		ُ͂łΒ~s܂D
 */
void CChiefView::CheckDIO_Running()
{
	// GbWo -------------------------------------------------------------
	// {bgA[o H->L
	BOOL l_bRobotArmOnDetected;
	if (0 != m_DioIgnoreSW.bRobotArm) {
		l_bRobotArmOnDetected = FALSE;
	} else {
		l_bRobotArmOnDetected = ((0 != m_DiInfo.bRobotArm) && (0 == nexioIsRobotArmDetect()))? TRUE : FALSE;
	}
	// eiX SW I H->L
	BOOL l_bMaintenanceSWOnDetected = ((0 != m_DiInfo.bMaintenanceSW) && (0 == nexioIsMaintenanceSwitch()))? TRUE : FALSE;
	// eiX SW It L->H
	BOOL l_bMaintenanceSWOffDetected = ((0 == m_DiInfo.bMaintenanceSW) && (0 != nexioIsMaintenanceSwitch()))? TRUE : FALSE;
	// GA͒ቺ L->H
	BOOL l_bAirPressureLowDetected;
	if (0 != m_DioIgnoreSW.bAirPressureLow) {
		l_bAirPressureLowDetected = FALSE;
	} else {
/* modified hmenjo 2009.05.20 GA͒ቺõbp֐ -------- { -------- */
//		l_bAirPressureLowDetected = ((0 == m_DiInfo.bAirPressureLow) && (0 != nexioIsAirPressureLevelLow()))? TRUE : FALSE;
/* modified hmenjo 2009.05.20 GA͒ቺõbp֐ -------- 		   */
		l_bAirPressureLowDetected = ((0 == m_DiInfo.bAirPressureLow) && (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->Rap_IsAirPressureLowON()))? TRUE : FALSE;
/* modified hmenjo 2009.05.20 GA͒ቺõbp֐ -------- } -------- */
	}
	// oL[ZTPIt H->L
	BOOL l_bVacuumPressure1OffDetected = ((0 != m_DiInfo.bVacuumPressure1) && (0 == nexioIsVacuumOn()))? TRUE : FALSE;
	// s_E H->L
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
/* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- { ---------- */
	if ((true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle()) && (false == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle())) {
		/* XgXV[PX݂̂́CsC~[C邢́CACgʒuȊO NG łD	*/
		if (
			(0 != m_DiInfo.bPinUp)											/* ㏸[I	*/
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
//		 || ((0 != m_DiInfo.bPinDown) && (0 != nexioIsPinAlignmentPos()))	/* ~[ƃACgʒu̗I	*/
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ----------			   */
		 || ((0 != m_DiInfo.bPinDown) && (0 != m_DiInfo.bPinAlign))	/* ~[ƃACgʒu̗I	*/
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */
			) {
			l_bPinDownOffDetected = TRUE;
		} else {
			if (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsMeasuring()) {
				/* XgX蒆	*/
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
//				if ((0 == m_DiInfo.bPinUp) && (0 != nexioIsPinAlignmentPos()) && (0 == m_DiInfo.bPinDown)) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ----------			   */
				if ((0 == m_DiInfo.bPinUp) && (0 != m_DiInfo.bPinAlign) && (0 == m_DiInfo.bPinDown)) {
/* modified 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */
					/* ㏸[ItCCACgʒuIC~[ItCȂُłȂ	*/
					l_bPinDownOffDetected = FALSE;
				} else {
					l_bPinDownOffDetected = TRUE;
				}
			} else {
				l_bPinDownOffDetected = FALSE;
			}
		}
	}
/* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- } ---------- */
	// [NKChJIt H->L
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
	// Vb^It H->L
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

	// Xe[Wx -------------------------------------------------------
	if (0 != l_bMaintenanceSWOnDetected) {
		// eiX[hɂȂ
// modified hmenjo 2009.05.12 xύXtOǉ ---------- { ----------
//		// x
//		StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 xύXtOǉ ----------
		if (TRUE == g_bIL_SpeedDown) {
			// x
			StageEnableSpeedLimit();
		}
// modified hmenjo 2009.05.12 xύXtOǉ ---------- } ----------
//		if (0 == m_bSpeedLimiterOFF) {
//			// x[hZbg
//			((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(TRUE);
//		}
	}
	if (0 != l_bMaintenanceSWOffDetected) {
		// eiX[hłȂȂ
		// xȂ
		StageDisableSpeedLimit();
//		// x[hZbg
//		((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(FALSE);
	}

/* added 2009.08.04 hmenjo 蒆ȊOhAJXe[W~ ---------- { ---------- */
	/* 蒆ȊOŃhAJȂCXe[W~܂D	*/
	if ((true == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())
	 && (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsIdle())) {
		/* V[PX肪AChłD	 */
		if ((0 == m_DiInfo.bDoorInterlock) && (0 == m_DiInfo.bTHMaintenanceSW)) {
			/* hAJ oD	*/
			if ((0 == m_DiInfo.bMaintenanceSW) && (FALSE == g_bIL_DoorOpen)) {
				;	/* eŁChAJo֎~̂Ƃ͊JƔF܂D	*/
			} else {
				/* {ɌoD	*/
				if (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
					/* XY Xe[W쒆łD	*/
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 쒆tO(XY Xe[W)It	*/
					AlarmIf_Set(ALID_DoorOpen);		/* hAJ	*/
				}
			}
		}
/* added 2009.10.30 hmenjo CTA CTAILPI 펞`FbN ---------- { ---------- */
		if (0 == this->IsCtaILPI()) {
			/* CTAILPI Ito	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_CTA_IL_PI_On);		/* CTA IL PI Ito	*/
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- { ---------- */
				this->CtaReset(1);
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- } ---------- */
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 쒆tO(XY Xe[W)It	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* ߂l͖Ă܂	*/
					StageElevatorStop();	/* ߂l͖Ă܂	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 쒆tO(Z )It	*/
				}
			}
		}
/* added 2009.10.30 hmenjo CTA CTAILPI 펞`FbN ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position 펞`FbN ---------- { ---------- */
		int iResistStatus = this->CheckResistIL();
		if (0 != iResistStatus) {
			/* ResistIL ُ팟o	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				if (1 == iResistStatus) {
					AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL Ito	*/
				} else {
					AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL ُ팟o	*/
				}
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 쒆tO(XY Xe[W)It	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* ߂l͖Ă܂	*/
					StageElevatorStop();	/* ߂l͖Ă܂	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 쒆tO(Z )It	*/
				}
			}
		}
/* added 2009.11.06 K.Matsuo RS Seq Head Position 펞`FbN ---------- } ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
		if( 0 == this->IsMSILPI()){
			//[ʒuQ̃ZT[ONo
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_MS_IL_POS_ERROR);		/* CTA IL PI Ito	*/
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 쒆tO(XY Xe[W)It	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* ߂l͖Ă܂	*/
					StageElevatorStop();	/* ߂l͖Ă܂	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 쒆tO(Z )It	*/
				}
			}
		}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
	} else {
/* added 2009.10.30 hmenjo CTA CTAILPI 펞`FbN ---------- { ---------- */
		/* V[PXŃAChȊO܂D	*/
		if (0 == this->IsCtaILPI()) {
			/* CTAILPI Ito	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_CTA_IL_PI_On);	/* CTA IL PI Ito	*/
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- { ---------- */
				this->CtaReset(1);
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- } ---------- */
				EqPowerOffPulse();				/* udIt|o̓I	*/
				StopAxisAllEMO();				/* n̑~w߂𔭍s	*/
				CancelSeqForAlarm();			/* LZs	*/
				ProcStatusSet(PROCESS_DOWN);	/*  DOWN Ԃɂ	*/
			}
		}
/* added 2009.10.30 hmenjo CTA CTAILPI 펞`FbN ---------- } ---------- */
/* added 2009.11.06 K.Matsuo RS Seq Head Position 펞`FbN ---------- { ---------- */
		/* V[PXŃAChȊO܂D	*/
		int iResistStatus = this->CheckResistIL();
		if (0 != iResistStatus) {
			/* ResistIL ُ팟o	*/
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				if (1 == iResistStatus) {
					AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL Ito	*/
				} else {
					AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL ُ팟o	*/
				}
				EqPowerOffPulse();				/* udIt|o̓I	*/
				StopAxisAllEMO();				/* n̑~w߂𔭍s	*/
				CancelSeqForAlarm();			/* LZs	*/
				ProcStatusSet(PROCESS_DOWN);	/*  DOWN Ԃɂ	*/
			}
		}
/* added 2009.11.06 K.Matsuo RS Seq Head Position 펞`FbN ---------- } ---------- */
// 2009.11.12 bagus MS [ʒuQ`FbN --{--
		if( 0 == this->IsMSILPI()){
			//[ʒuQ̃ZT[ONo
			BOOL l_bXYStg = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE);
			BOOL l_bEV = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS);
			if ((0 != l_bXYStg) || (0 != l_bEV)) {
				AlarmIf_Set(ALID_MS_IL_POS_ERROR);		/* CTA IL PI Ito	*/
				if (0 != l_bXYStg) {
					StageStop();
					ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 쒆tO(XY Xe[W)It	*/
				}
				if (0 != l_bEV) {
					StageAbortAutoFocus();	/* ߂l͖Ă܂	*/
					StageElevatorStop();	/* ߂l͖Ă܂	*/
					ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			/* 쒆tO(Z )It	*/
				}
			}
		}
// 2009.11.12 bagus MS [ʒuQ`FbN --}--
	}
/* added 2009.08.04 hmenjo 蒆ȊOhAJXe[W~ ---------- } ---------- */

	// 쒆 DI `FbN -------------------------------------------------------------------------------------------
	// ً}~ -------------------------------------------------------------------------------
	// EMOChAC^bNCup[It o -----------------------------
/* modified 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
//	if ((0 == m_AlarmFlags.bEMOStopOn) || (0 == m_AlarmFlags.bDoorInterlockOn) || (0 == m_AlarmFlags.bEQPowerOn)) {
/* modified 2009.08.17 hmenjo Z[teBvOǉ ----------			   */
	if ((0 == m_AlarmFlags.bEMOStopOn) || (0 == m_AlarmFlags.bDoorInterlockOn) || (0 == m_AlarmFlags.bEQPowerOn) || (0 == m_AlarmFlags.bSafetyPlugOn)) {
/* modified 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
		DWORD l_dwResult;
		if (0 != (l_dwResult = CheckDIO_IsEMO())) {
//			// n̑~w߂𔭍s
//			StopAxisAllEMO();
//			// LZs
//			CancelSeqForAlarm();
//			//  DOWN Ԃɂ
//			ProcStatusSet(PROCESS_DOWN);
			// A[ʒm
			switch (l_dwResult) {
			case ALID_EMOSwitchOn:		// EMO
				if (0 == m_AlarmFlags.bEMOStopOn) {
					m_AlarmFlags.bEMOStopOn = TRUE;
					// udIt|o̓I
					EqPowerOffPulse();
					// n̑~w߂𔭍s
					StopAxisAllEMO();
					// LZs
					CancelSeqForAlarm();
					//  DOWN Ԃɂ
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EMOSwitchOn));
				}
				break;
			case ALID_DoorOpen:		// hAC^bN
/* added 2009.06.02 hmenjo hAJő|[Y ---------- { ---------- */
				if (TRUE == g_bIL_DoorOpenPause) {
					SentMeasPauseCheck(1);
					break;
				}
/* added 2009.06.02 hmenjo hAJő|[Y ---------- } ---------- */
				if (0 == m_AlarmFlags.bDoorInterlockOn) {
					m_AlarmFlags.bDoorInterlockOn = TRUE;
					// udIt|o̓I
					EqPowerOffPulse();
					// n̑~w߂𔭍s
					StopAxisAllEMO();
					// LZs
					CancelSeqForAlarm();
					//  DOWN Ԃɂ
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_DoorOpen));
				}
				break;
			case ALID_EquipmentPowerOff:	// up[It
				if (0 == m_AlarmFlags.bEQPowerOn) {
					m_AlarmFlags.bEQPowerOn = TRUE;
//					// udIt|o̓I		up[Ito͑udIto͕͂svłD
//					EqPowerOffPulse();
					// n̑~w߂𔭍s
					StopAxisAllEMO();
					// LZs
					CancelSeqForAlarm();
					//  DOWN Ԃɂ
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_EquipmentPowerOff));
				}
				break;
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- { ---------- */
			case ALID_SafetyPlugOpen:		/* Z[teBvO	*/
				if (0 == m_AlarmFlags.bSafetyPlugOn) {
					m_AlarmFlags.bSafetyPlugOn = TRUE;
					// udIt|o̓I
					EqPowerOffPulse();
					// n̑~w߂𔭍s
					StopAxisAllEMO();
					// LZs
					CancelSeqForAlarm();
					//  DOWN Ԃɂ
					ProcStatusSet(PROCESS_DOWN);
					PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_SafetyPlugOpen));
				}
				break;
/* added 2009.08.17 hmenjo Z[teBvOǉ ---------- } ---------- */
			default:	// ܂
				break;
			}
		}
	}
/* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- { ---------- */
	/* XgX蒆Ȃ Z ޔʒu`FbN	*/
	if (true == ((CChiefTransiStress*) m_pcChiefTransiStress)->IsMeasuring()) {
		if (0 == nexioIsRetractPosZ()) {
			// udIt|o̓I
			EqPowerOffPulse();
			// n̑~w߂𔭍s
			StopAxisAllEMO();
			// LZs
			CancelSeqForAlarm();
			//  DOWN Ԃɂ
			ProcStatusSet(PROCESS_DOWN);
/* 2009.08.20 K.Matsuo ALID `ύX ---------- { ---------- */
//				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ZPOSOFF_ALID));
/* 2009.08.20 K.Matsuo ALID `ύX ----------			   */
			if (0 == m_AlarmFlags.bRetractZPosOff) {
				// A[ʒm
				m_AlarmFlags.bRetractZPosOff = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_PositionRetractFailed));
/* 2009.08.20 K.Matsuo ALID `ύX ---------- } ---------- */
			}
		}
	}
/* added 2009.08.17 hmenjo XgX@\ǉ(37) ---------- } ---------- */
	// {bgA[ICs_EItC[NKChJIt o ------------
#if 0		// V[vdlŃVb^J̌oǉ
	if ((0 == m_AlarmFlags.bRobotArmOn) || (0 == m_AlarmFlags.bPinDownOff) || (0 == m_AlarmFlags.bWorkGuideOpenOff)) {
		if ((0 != l_bRobotArmOnDetected) || (0 != l_bPinDownOffDetected) || (0 != l_bWorkGuideOpenOffDetected)) {
#else
	if ((0 == m_AlarmFlags.bRobotArmOn) || (0 == m_AlarmFlags.bPinDownOff) || (0 == m_AlarmFlags.bWorkGuideOpenOff) || (0 == m_AlarmFlags.bShutterCloseOff)) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- { ---------- */
//		if ((0 != l_bRobotArmOnDetected) || (0 != l_bPinDownOffDetected) || (0 != l_bWorkGuideOpenOffDetected) || (0 != l_bShutterCloseOffDetected)) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ----------			  */
		if (
			(0 != l_bRobotArmOnDetected)
		 || (0 != l_bPinDownOffDetected)
		 || ((0 != l_bWorkGuideOpenOffDetected)	&& (ST_STRS_PINDOWNCOMP != ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState())
		 										&& (ST_STRS_ABORTPINDOWN != ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState()))
		 || (0 != l_bShutterCloseOffDetected)
			) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- } ---------- */
#endif
			if (
				(PROCESS_PROC == ProcStatusGet())											// Processing
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY Xe[W쒆
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 쒆
				) {
				// udIt|o̓I
				EqPowerOffPulse();
				// n̑~w߂𔭍s
				StopAxisAllEMO();
				// LZs
				CancelSeqForAlarm();
				//  DOWN Ԃɂ
				ProcStatusSet(PROCESS_DOWN);
				// A[ʒm
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
	// TCN~ ---------------------------------------------------------------------------
	// eiX SW I o ----------------------------------------------
	if (0 == m_AlarmFlags.bMaintenanceSWOn) {
// modified hmenjo 2009.05.12 ُ펞TCNXgbvtOǉ ---------- { ----------
//		if (0 != l_bMaintenanceSWOnDetected) {
// modified hmenjo 2009.05.12 ُ펞TCNXgbvtOǉ ----------
		if ((0 != l_bMaintenanceSWOnDetected) && (TRUE == g_bIL_CycleStop)) {
// modified hmenjo 2009.05.12 ُ펞TCNXgbvtOǉ ---------- } ----------
			if (
				(0 == m_DiInfo.bTHMaintenanceSW)											// e SW It
			 && (
					(PROCESS_PROC == ProcStatusGet())											// Processing
				 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY Xe[W쒆
				 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 쒆
				)
				) {
				// n̑~w߂𔭍s
				StopAxisAllEMO();
				// LZs
				CancelSeqForAlarm();
				// A[ʒm
				m_AlarmFlags.bMaintenanceSWOn = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_MaintenanceSwitchOn));
			}
		}
	}
	// GA͒ቺ o ----------------------------------------------
	if (0 == m_AlarmFlags.bAirPressureLowOn) {
		if (0 != l_bAirPressureLowDetected) {
			if (
				(PROCESS_PROC == ProcStatusGet())											// Processing
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY Xe[W쒆
			 || (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 쒆
				) {
				// n̑~w߂𔭍s
				StopAxisAllEMO();
				// LZs
				CancelSeqForAlarm();
				// A[ʒm
				m_AlarmFlags.bAirPressureLowOn = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_AirPressureDown));
			}
		}
	}
	// oL[ZTP It o ----------------------------------------------
	if (0 == m_AlarmFlags.bVacuumPressure1Off) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- { ---------- */
//		if (0 != l_bVacuumPressure1OffDetected) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ----------			  */
		if ((0 != l_bVacuumPressure1OffDetected) && (ST_STRS_PINALIGN != ((CChiefTransiStress*) m_pcChiefTransiStress)->GetCurrentState())) {
/* modified 2009.10.06 hmenjo Stress nexifLoad ɕύX ---------- } ---------- */
			if (
				(PROCESS_PROC == ProcStatusGet())											// Processing
				) {
				// n̑~w߂𔭍s
				StopAxisAllEMO();
				// LZs
				CancelSeqForAlarm();
				// A[ʒm
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
		if( iDioX1AxisAlarm != 0x0000 && // G[Ȃ
			iDioX1AxisAlarm != 0x0100 && // I[o[gx
			iDioX1AxisAlarm != 0x0010 && // Xe[W`
			iDioX1AxisAlarm != 0x0001 )  // Xe[W`
		{
			bError = TRUE;
			OutputStageError(iDioX1AxisAlarm, 0);
		}

		if( iDioX2AxisAlarm != 0x0000 && // G[Ȃ
			iDioX2AxisAlarm != 0x0100 && // I[o[gx
			iDioX2AxisAlarm != 0x0010 && // Xe[W`
			iDioX2AxisAlarm != 0x0001 )  // Xe[W`
		{
			bError = TRUE;
			OutputStageError(iDioX2AxisAlarm, 1);
		}

		if( iDioYAxisAlarm != 0x0000 && // G[Ȃ
			iDioYAxisAlarm != 0x0100 && // I[o[gx
			iDioYAxisAlarm != 0x0010 && // Xe[W`
			iDioYAxisAlarm != 0x0001 )  // Xe[W`
		{
			bError = TRUE;
			OutputStageError(iDioYAxisAlarm, 2);
		}

		if (bError == TRUE) {
// 			if (
// 				(PROCESS_PROC == ProcStatusGet())											// Processing
// 				|| (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_XYSTAGE))	// XY Xe[W쒆
// 				|| (0 != ((CNanoSpecDoc*) m_pcNanoSpecDoc)->ActuateFlagsGet(ACTUATE_ZAXIS))	// Z 쒆
// 				) {
				// udIt|o̓I
				EqPowerOffPulse();
				// n̑~w߂𔭍s
				StopAxisAllEMO();
				// LZs
				CancelSeqForAlarm();
				//  DOWN Ԃɂ
				ProcStatusSet(PROCESS_DOWN);
				// A[ʒm
				m_AlarmFlags.bStageAlarmOn = TRUE;
				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_StageError));
//			}
		}
	}

	if (0 == m_AlarmFlags.bStageOverTravelOn) {
		bError = FALSE;
		if( iDioX1AxisAlarm == 0x0100 ) // I[o[gx
		{
			bError = TRUE;
			OutputStageError(iDioX1AxisAlarm, 0);
		}

		if( iDioX2AxisAlarm == 0x0100 ) // I[o[gx
		{
			bError = TRUE;
			OutputStageError(iDioX2AxisAlarm, 1);
		}

		if( iDioYAxisAlarm == 0x0100 ) // I[o[gx
		{
			bError = TRUE;
			OutputStageError(iDioYAxisAlarm, 2);
		}

		if (bError == TRUE) {
			// LZs
			CancelSeqForAlarm();
			// A[ʒm
			m_AlarmFlags.bStageOverTravelOn = TRUE;
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, ALID_StageOverTravelError));
		}
	}

	if (0 == m_AlarmFlags.bStageUnknownOn) {
		bError = FALSE;
		if( iDioX1AxisAlarm == 0x0010 ||
			iDioX1AxisAlarm == 0x0001) // Xe[W`
		{
			bError = TRUE;
			OutputStageError(iDioX1AxisAlarm, 0);
		}

		if( iDioX2AxisAlarm == 0x0010 ||
			iDioX2AxisAlarm == 0x0001) // Xe[W`
		{
			bError = TRUE;
			OutputStageError(iDioX2AxisAlarm, 1);
		}

		if( iDioYAxisAlarm == 0x0010 ||
			iDioYAxisAlarm == 0x0001) // Xe[W`
		{
			bError = TRUE;
			OutputStageError(iDioYAxisAlarm, 2);
		}

		if (bError == TRUE) {
			// LZs
			CancelSeqForAlarm();
			// A[ʒm
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
		sprintf(szMessage, "A.0xx / A.8xx / A.bxx Error (Amp = %s, ErrorCode = 1-111)", szAxisName);
		break;
	case 0x1011:
		sprintf(szMessage, "A.100 Error (Amp = %s, ErrorCode = 1-011)", szAxisName);
		break;
	case 0x1001:
		sprintf(szMessage, "A.3xx / A.dxx Error (Amp = %s, ErrorCode = 1-001)", szAxisName);
		break;
	case 0x1110:
		sprintf(szMessage, "A.4xx Error (Amp = %s, ErrorCode = 1-110)", szAxisName);
		break;
	case 0x1010:
		sprintf(szMessage, "A.5xx / A.Cxx Error (Amp = %s, ErrorCode = 1-010)", szAxisName);
		break;
	case 0x1000:
		sprintf(szMessage, "A.7xx Error (Amp = %s, ErrorCode = 1-000)", szAxisName);
		break;
	case 0x1100:
		sprintf(szMessage, "A.Exx Error (Amp = %s, ErrorCode = 1-100)", szAxisName);
		break;
	case 0x1101:
		sprintf(szMessage, "A.F10 Error (Amp = %s, ErrorCode = 1-101)", szAxisName);
		break;
	case 0x0111:
		sprintf(szMessage, "A.900 / A.901 Error (Amp = %s, ErrorCode = 0-111)", szAxisName);
		break;
	case 0x0011:
		sprintf(szMessage, "A.910 / A.911 Error (Amp = %s, ErrorCode = 0-011)", szAxisName);
		break;
	case 0x0101:
		sprintf(szMessage, "A.920 / A.921 Error (Amp = %s, ErrorCode = 0-101)", szAxisName);
		break;
	case 0x0110:
		sprintf(szMessage, "A.941Error (Amp = %s, ErrorCode = 0-110)", szAxisName);
		break;
	case 0x0100:
		sprintf(szMessage, "A.9A0 OverTravel (Amp = %s, ErrorCode = 0-100)", szAxisName);
		break;
	default:
		sprintf(szMessage, "Unknown Error (Amp = %s, ErrorCode = 0-001)", szAxisName);
		break;
	}

	LogStageError(szMessage);
}

/*
 *	n̑~w
 */
void CChiefView::StopAxisAllEMO()
{
	// XY ~
	StageStop();			// ߂l͖Ă܂
	ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		// 쒆tO(XY Xe[W)It
	// AF ~
	StageAbortAutoFocus();	// ߂l͖Ă܂
	// Z ~
	StageElevatorStop();	// ߂l͖Ă܂
	ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);			// 쒆tO(Z )It
}

/*
 *	A[̃V[PX LZ
 */
void CChiefView::CancelSeqForAlarm()
{
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- { ---------- */
//	if ((PROCESS_PROC == ProcStatusGet()) && (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())) {
//		// Processing ŃV[PX肪 IDLE ȊȌꍇ
//		((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_CANCEL);
//	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ----------			   */
	if (PROCESS_PROC == ProcStatusGet()) {
		/* Processing 	*/
		CHIEF_PFUNCS l_ChiefPFuncs;
		if (0 != PFC_FuncSet(this, &l_ChiefPFuncs, 0)) {
			if (false == (*l_ChiefPFuncs.IsIdle)(this)) {
				/* V[PX肪 IDLE ȊȌꍇ	*/
				(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iCANCEL, 0);
			}
		}
	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- } ---------- */
}

/*
 *	(AF)JnÕwbhh҂
 */
void CChiefView::WaitTimeBeforeMeas(
		DWORD dwMode	// 0FJn擾C1Fԑ҂
	)
{
	if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
		// @̏ꍇ̂ݎ{
		switch (dwMode) {
		case 0:
			m_WaitMeasTime.dwStartTime = ::GetTickCount();
			break;
		default:
			if (0 != m_WaitMeasTime.dwWaitTimeSetting) {
				// ݒlȌꍇ̂ݏ܂
				DWORD l_dwDeltaTime;
				m_WaitMeasTime.dwEndTime = ::GetTickCount();
				if (m_WaitMeasTime.dwStartTime <= m_WaitMeasTime.dwEndTime) {
					l_dwDeltaTime = m_WaitMeasTime.dwEndTime - m_WaitMeasTime.dwStartTime;
				} else {
					// GetTickCount() ̃JE^I[ot[Ăꍇ
					l_dwDeltaTime = (0xffffffff - m_WaitMeasTime.dwStartTime) + m_WaitMeasTime.dwEndTime;
				}
				if (l_dwDeltaTime < m_WaitMeasTime.dwWaitTimeSetting) {
					// ݒl̎c莞ԂX[v܂
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
 *	́u肷vɂȂĂ鑪|Cgԍ擾܂
 */
DWORD CChiefView::GetNextPointNo(
		DWORD *pdwPointNo	// Jnԍ
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
/* added 2009.08.03 hmenjo XgX@\ǉ(18) ---------- { ---------- */
/*
 *	w肳ꂽJnԍuLvɂȂĂ鑪胉Cԍ擾܂
 */
DWORD CChiefView::GetNextLineNo(
		DWORD *pdwLineNo	// Jnԍ
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
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 폜 ---------- { ---------- */
//		if (0 != l_pStageProgStress->Line[i - 1].LineSec.bValidLine) {
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 폜 ----------			  */
		if (0 != l_pStageProgStress->Line[i - 1].bValidLine) {
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 폜 ---------- } ---------- */
			break;
		}
	}

	*pdwLineNo = i;

	return i;
}
/* added 2009.08.03 hmenjo XgX@\ǉ(18) ---------- } ---------- */

/*
 *	Ot@C֐ - CNanoSpecDoc::ChiefLogging() ̃bp֐
 */
void CChiefView::LogChief(TCHAR *pszLogText)
{
	if (0 != m_pcNanoSpecDoc) {
		((CNanoSpecDoc*) m_pcNanoSpecDoc)->ChiefLogging(pszLogText);
	}
}

/*
 *	Ot@C֐ - bZ[WMp
 */
void CChiefView::LogChief_WinMsg(TCHAR* ptszMsgName, WPARAM wparam, LPARAM lparam)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, "Received %s (WPARAM = 0x%08x, LPARAM = 0x%08x).", ptszMsgName, wparam, lparam);
	LogChief(l_tszLogText);
}

/*
 *	Ot@C֐ - gWVp
 */
void CChiefView::LogChief_Transi(TCHAR* ptszTransiState, DWORD dwParam)
{
	TCHAR l_tszLogText[256];
	_stprintf(l_tszLogText, _T("%s : param = 0x%08x"), ptszTransiState, dwParam);
	LogChief(l_tszLogText);
}

// 2013.01.10 bagus stage driver alarm io -->
/*
 *	Ot@C֐ - CNanoSpecDoc::StageErrorLogging() ̃bp֐
 */
void CChiefView::LogStageError(TCHAR *pszLogText)
{
	if (0 != m_pcNanoSpecDoc) {
		((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageErrorLogging(pszLogText);
	}
}
// 2013.01.10 bagus stage driver alarm io <--

/*
 *	udIt|o
 */
void CChiefView::EqPowerOffPulse()
{
/* added 2009.05.26 hmenjo udIto̗͂L/XCb`ǉ ---------- { ---------- */
	if (0 == g_bIL_EQPowerOff) {
		return;
	}
/* added 2009.05.26 hmenjo udIto̗͂L/XCb`ǉ ---------- } ---------- */
	// udItFo
	nexioEquipmentPowerOFF(TRUE);
	m_bCheckEqPowerOFF = 1;		// ĎtOZbg(Ďɂ)
	// ͊Ď^C}N
	if (ID_TIMER_EQPWOFF != SetTimer(ID_TIMER_EQPWOFF, TIMER_EQPWOFF_TIMEOUT, 0)) {
		// ^C}Ns
		LogChief(CHIEF_REP_ALARM_MSGTEXT[34]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(34, 0));
	}
}




// hmenjo 20090109 - DI }jA ----- --->
//	fobÔ߂ɁCNextra  DIO j^Őݒ肷
//	}jA͂ƃpX[h("toho"Œ)CƁC
//	Kv DI Iɓ͂܂D
HANDLE lg_hEvFoundControl;
HWND lg_hwndFoundControl;
struct {
	TCHAR tszJPN[128];
	TCHAR tszENU[128];
} lg_DiName[] = {
	_T("ً}~"),					_T("Emergency Stop"),		// 101
	_T("hAC^[bN"),		_T("Door Interlock"),		// 102
	_T("ud"),					_T("Eq Power"),				// 103
	_T("eiXXCb`"),		_T("Maintenance SW"),		// 104
	_T("t^[1 [ZT["),	_T("Pin1 Down Pos"),		// 105
	_T("t^[2 [ZT["),	_T("Pin2 Down Pos"),		// 106
	_T("Vb^[V_1["),	_T("Shutter Top1"),			// 107
	_T("{bgA["),			_T("Robot Arm"),			// 108
	_T("oL[1"),				_T("Vacuum Pressure1"),		// 109
	_T("oL[2"),				_T("Vacuum Pressure2"),		// 110
	_T("X [h|WV"),		_T("Load Position X"),		// 111
	_T("Y [h|WV"),		_T("Load Position Y"),		// 112
	_T("[NKCh1 REV"),		_T("Work Guide1 REV"),		// 113
	_T("[NKCh2 REV"),		_T("Work Guide2 REV"),		// 114
	_T("[NKCh3 REV"),		_T("Work Guide3 REV"),		// 115
	_T("[NKCh4 REV"),		_T("Work Guide4 REV"),		// 116
	_T("[NKCh5 REV"),		_T("Work Guide5 REV"),		// 117
	_T("[NKCh6 REV"),		_T("Work Guide6 REV"),		// 118
	_T("[NKCh7 REV"),		_T("Work Guide7 REV"),		// 119
	_T("[NKCh8 REV"),		_T("Work Guide8 REV"),		// 120
	_T(""),							_T(""),						// 165
};
BOOL CALLBACK EnumChildProc_SearchControl(HWND hWnd, LPARAM lParam)
{
	TCHAR l_tszSearchTextJ[256];
	TCHAR l_tszSearchTextE[256];
	switch (lParam) {
	case 1:
		_tcscpy(l_tszSearchTextJ, _T("}jA"));
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
		// 
	} else if (0 == _tcscmp(l_tszGotText, l_tszSearchTextE)) {
		// 
	} else {
		return TRUE;
	}
	lg_hwndFoundControl = hWnd;
	BOOL l_bRc = ::SetEvent(lg_hEvFoundControl);

	return FALSE;
}
void CChiefView::OnBtnTest()
{
	// TODO: ̈ʒuɃRg[ʒmnhp̃R[hǉĂ

	// uNexI/O j^[vT܂D
	HWND l_hwndNexIoMonitor = 0;
	BOOL l_bFind = FALSE;
	{
		for (int i = 0; i < 180; i++) {
			l_hwndNexIoMonitor = ::FindWindow(0, _T("NexI/O j^["));
			if (0 != l_hwndNexIoMonitor) {
				l_bFind = TRUE; break;	// 
			} else {
				l_hwndNexIoMonitor = ::FindWindow(0, _T("NexI/O Monitor"));
				if (0 != l_hwndNexIoMonitor) {
					l_bFind = TRUE; break;	// 
				}
			}
			::Sleep(1000);
		}
	}
	if (0 == l_bFind) {
		// 
		::MessageBox(m_hWnd, _T("Cannot found DIO Monitor Dialog."), _T("DIO Monitor Auto Manual Input"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	// u}jÁv`FbN{bNXT܂D
	lg_hEvFoundControl = ::CreateEvent(0, TRUE, FALSE, 0);		// oCxg
	EnumChildWindows(l_hwndNexIoMonitor, EnumChildProc_SearchControl, 1);
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
		// CxgȂ
		return;
	}
	::PostMessage(lg_hwndFoundControl, BM_CLICK, 0, 0);
	// upX[h̊mFv_CAOT܂D
	HWND l_hwndPassWord = 0;
	l_bFind = FALSE;
	{
		for (int i = 0; i < 1800; i++) {
			l_hwndPassWord = ::FindWindow(0, _T("pX[h̊mF"));
			if (0 != l_hwndPassWord) {
				l_bFind = TRUE; break;	// 
			} else {
				l_hwndPassWord = ::FindWindow(0, _T("Password"));
				if (0 != l_hwndPassWord) {
					l_bFind = TRUE; break;	// 
				}
			}
			::Sleep(100);
		}
	}
	if (0 == l_bFind) {
		// 
		::MessageBox(m_hWnd, _T("Cannot found Password Dialog."), _T("DIO Monitor Auto Manual Input"), MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	// GfBbg{bNXT
	BOOL l_bRc = ::ResetEvent(lg_hEvFoundControl);
	EnumChildWindows(l_hwndPassWord, EnumChildProc_SearchControl, 2);
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
		// CxgȂ
		return;
	}
	::SendMessage(lg_hwndFoundControl, WM_SETTEXT, 0, (LPARAM) _T("toho"));
	// OK {^T
	l_bRc = ::ResetEvent(lg_hEvFoundControl);
	EnumChildWindows(l_hwndPassWord, EnumChildProc_SearchControl, 3);
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
		// CxgȂ
		return;
	}
	::PostMessage(lg_hwndFoundControl, BM_CLICK, 0, 0);
	// Kv DI NbN
	{
		DWORD l_dwCounter = 0;
		while ((0 != lg_DiName[l_dwCounter].tszJPN[0]) && (0 != lg_DiName[l_dwCounter].tszENU[0])) {
			l_bRc = ::ResetEvent(lg_hEvFoundControl);
			EnumChildWindows(l_hwndNexIoMonitor, EnumChildProc_SearchControl, l_dwCounter + 101);
			if (WAIT_OBJECT_0 != ::WaitForSingleObject(lg_hEvFoundControl, 2000)) {
				// CxgȂ
				return;
			}
			::PostMessage(lg_hwndFoundControl, BM_CLICK, 0, 0);
			l_dwCounter++;
		}
	}
}
// hmenjo 20090109 - DI }jA ----- <---


void CChiefView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: ̈ʒuɌŗL̏ǉ邩A܂͊{NXĂяoĂ
}

void CChiefView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	LogChief(_T("Started  CChiefView::OnInitialUpdate()."));

	// Chief _CAÕLvVݒ
	SetWindowText(CHIEF_DLG_CAPTION);

	// ϐ ------------------------------------------------------------
	m_dwHeartBeatCnt = 0;
// 2009.02.05 K.Matsuo delete -->
//	m_uiTraceDataPeriod = 0;
//	m_bTraceDataTimer = FALSE;
// 2009.02.05 K.Matsuo delete <--
	m_bEQRunPrev = FALSE;
	m_dwStageMoveState = 0;
/* added 2009.08.03 hmenjo XgX@\ǉ(17) ---------- { ---------- */
	m_dwPinMoveState = 0;
/* added 2009.08.03 hmenjo XgX@\ǉ(17) ---------- } ---------- */
	m_bVacuumOnFromDisp = FALSE;
	m_bVacuumOffFromDisp = FALSE;
	memset(m_szMainRecipeName, 0, sizeof(m_szMainRecipeName));
	m_dwModuleState_Stage = 0;	// (Uninitialize)
	m_dwModuleState_SR_Meas = 0;	// (Uninitialize)
	m_dwModuleState_Pif = 0;	// (Uninitialize)
	m_dwModuleState_Nextra = 0;	// (Uninitialize)
	m_hEvTrMaster = 0;
	m_bGotRecipeFromPif = FALSE;
	m_bReqCancelComplete = FALSE;
	memset(&m_AlarmFlags, 0, sizeof(ALARM_FLAGS));
	memset(&m_WaitMeasTime, 0, sizeof(WAIT_MEAS_TIME));
	m_bCheckEqPowerOFF = 0;
/* added 2009.08.20 hmenjo XgX@\ǉ(46) ---------- { ---------- */
	m_pcChiefTransiStress = 0;
/* added 2009.08.20 hmenjo XgX@\ǉ(46) ---------- } ---------- */

/* added 2009.10.30 hmenjo CTA CTAILPI 펞`FbN ---------- { ---------- */
	/* wbhRtBORs[	*/
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	memcpy(&m_EnableHead, &l_SystemConfig.HeadType, sizeof(HEAD_TYPE_CONFIG));
/* added 2009.10.30 hmenjo CTA CTAILPI 펞`FbN ---------- } ---------- */


	///// HIDE Button /////
	m_HideButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_HideButton.DrawFlatFocus(TRUE);

	///// TOP Button /////
	m_TopButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TopButton.DrawFlatFocus(TRUE);

	///// TEST Button /////
	m_TestButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TestButton.DrawFlatFocus(TRUE);

	// ݒt@CǍ ----------------------------------------------------
	// ݒt@CǍ - uforTESTv{^̗L/ IDC_BTN_TEST
	char szFilePath[MAX_PATH];
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	BOOL l_bforTESTBtn = GetPrivateProfileInt(_T("ForTest"), _T("forTESTBtn"), 0, szFilePath);
	if (0 == l_bforTESTBtn) {
		GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_BTN_TEST)->ShowWindow(SW_SHOW);
	}
	// ݒt@CǍ - ͖XCb` - {bgA[ o
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DioIgnoreSW.bRobotArm = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("RobotArm"), 0, szFilePath);
	if (0 == m_DioIgnoreSW.bRobotArm) {
		m_DioIgnoreSW.bRobotArm = FALSE;
	} else {
		m_DioIgnoreSW.bRobotArm = TRUE;
	}
	// ݒt@CǍ - ͖XCb` - s_E o
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DioIgnoreSW.bPinDown = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("PinDown"), 0, szFilePath);
	if (0 == m_DioIgnoreSW.bPinDown) {
		m_DioIgnoreSW.bPinDown = FALSE;
	} else {
		m_DioIgnoreSW.bPinDown = TRUE;
	}
	// ݒt@CǍ - ͖XCb` - GA͒ቺ o
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DioIgnoreSW.bAirPressureLow = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("AirPressureLow"), 0, szFilePath);
	if (0 == m_DioIgnoreSW.bAirPressureLow) {
		m_DioIgnoreSW.bAirPressureLow = FALSE;
	} else {
		m_DioIgnoreSW.bAirPressureLow = TRUE;
	}
/* added 2009.11.27 hmenjo Pif s&Vb^  ---------- { ---------- */
	/* ݒt@CǍ - sVb^	*/
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--
	m_DioIgnoreSW.bPinShutterILInvalid = GetPrivateProfileInt(_T("DioIgnoreSW"), _T("PinShutterILInvalid"), 0, szFilePath);
	if (1 == m_DioIgnoreSW.bPinShutterILInvalid) {
		m_DioIgnoreSW.bPinShutterILInvalid = TRUE;
	} else {
		m_DioIgnoreSW.bPinShutterILInvalid = FALSE;
	}
/* added 2009.11.27 hmenjo Pif s&Vb^  ---------- } ---------- */
	// ݒt@CǍ - ړxݒ
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--
	m_bSpeedLimiterOFF = GetPrivateProfileInt(_T("SpeedLimit"), _T("LimiterOFF"), 0, szFilePath);
	if (0 == m_bSpeedLimiterOFF) {
		m_bSpeedLimiterOFF = FALSE;
	} else {
		m_bSpeedLimiterOFF = TRUE;
	}
	// ݒt@CǍ - SEQ sړtO
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--
	m_bPreMoveSW = GetPrivateProfileInt(_T("PreMove"), _T("PreMoveSW"), 0, szFilePath);
	if (0 == m_bPreMoveSW) {
		m_bPreMoveSW = FALSE;
	} else {
		m_bPreMoveSW = TRUE;
	}
	// ݒt@CǍ - O҂
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	_tcscpy(szFilePath, AfxGetApp()->m_pszProfileName);
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--
	m_WaitMeasTime.dwWaitTimeSetting = GetPrivateProfileInt(_T("PreMove"), _T("WaitMeasTime"), 0, szFilePath);
	if (10000 < m_WaitMeasTime.dwWaitTimeSetting) {
		m_WaitMeasTime.dwWaitTimeSetting = 10000;
	}
	// ݒt@CǍ - _CAO\@
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DlgShowSW.dwType = GetPrivateProfileInt(_T("ShowSW"), _T("Type"), 0, szFilePath);
	if (2 < m_DlgShowSW.dwType) {
		m_DlgShowSW.dwType = 0;
	}
	// ݒt@CǍ - _CAO\fBC [s]
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, CHIEF_INI);
	m_DlgShowSW.dwDelay = GetPrivateProfileInt(_T("ShowSW"), _T("Delay"), 5, szFilePath);
	if (m_DlgShowSW.dwDelay < 2) {
		m_DlgShowSW.dwDelay = 2;
	}
	if (10 < m_DlgShowSW.dwDelay) {
		m_DlgShowSW.dwDelay = 10;
	}
	// Chief _CAO\^C}N
	if (2 != m_DlgShowSW.dwType) {
		m_DlgShowSW.dwShowSWprc = 0;
		if (ID_TIMER_HIDEDLG != SetTimer(ID_TIMER_HIDEDLG, 500, 0)) {
			// ^C}Ns
			LogChief(CHIEF_REP_ALARM_MSGTEXT[2]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(2, 0));
		}
	}

#if 0		// ̏ NanoSpec.exe ɏIoȂ߁CX MainFrame ɖ߂܂ --------------------
//	// NextraIO  ------------------------------------------------------
//	m_dwModuleState_Nextra = 0;	// (Uninitialize)
//	if (0 != nexifInitialize(this->m_hWnd)) {
//		m_dwModuleState_Nextra = 1;	// (Initialized)
//	}
#endif		// ̏ NanoSpec.exe ɏIoȂ߁CX MainFrame ɖ߂܂ --------------------
#if 0	// hmenjo NEXIOBASE.HXX ɕς̂ŕsv
//	m_dwModuleState_Nextra = 0;	// (Uninitialize)
//	if (0 != nexioInitialize(this->m_hWnd)) {
//		m_dwModuleState_Nextra = 1;	// (Initialized)
//	}
#endif 	// hmenjo NEXIOBASE.HXX ɕς̂ŕsv

#if 0		// ̏ NanoSpec.exe ɏIoȂ߁CX MainFrame ɖ߂܂ --------------------
//	// Pif ̏ -----------------------------------------------------------
//	m_dwModuleState_Pif = 0;	// (Uninitialize)
//	PifComm_Init();
//	m_dwModuleState_Pif = 1;	// (Initialized)
#endif		// ̏ NanoSpec.exe ɏIoȂ߁CX MainFrame ɖ߂܂ --------------------
	//  DI l擾ċLGA(vZXXe[^X񍐗pt@C}bsO)ɏlƂăZbg܂D
	GetDiInfo(&m_DiInfo);
	OnPifDiRefresh(0, 0);

	// ^C}nݒ ----------------------------------------------------------
	// }`fBA ^C}N
	if (0 == (m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD] = ::timeSetEvent(TIMER_5MS_PERIOD, 1, MMTimerProc, (DWORD) m_hWnd, TIME_PERIODIC))) {
		// ^C}Ns
		LogChief(CHIEF_REP_ALARM_MSGTEXT[15]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(15, 0));
	}
	// 1s ^C}N
	if (ID_TIMER_1S_PERIOD != SetTimer(ID_TIMER_1S_PERIOD, TIMER_1S_PERIOD, 0)) {
		// ^C}Ns
		LogChief(CHIEF_REP_ALARM_MSGTEXT[14]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(14, 0));
	}
	// 100ms ^C}N
	if (ID_TIMER_100MS_PERIOD != SetTimer(ID_TIMER_100MS_PERIOD, TIMER_100MS_PERIOD, 0)) {
		// ^C}Ns
		LogChief(CHIEF_REP_ALARM_MSGTEXT[13]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(13, 0));
	}
	// 50ms ^C}N
	if (ID_TIMER_50MS_PERIOD != SetTimer(ID_TIMER_50MS_PERIOD, TIMER_50MS_PERIOD, 0)) {
		// ^C}Ns
		LogChief(CHIEF_REP_ALARM_MSGTEXT[12]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(12, 0));
	}
	// 10ms ^C}N
	if (ID_TIMER_10MS_PERIOD != SetTimer(ID_TIMER_10MS_PERIOD, TIMER_10MS_PERIOD, 0)) {
		// ^C}Ns
		LogChief(CHIEF_REP_ALARM_MSGTEXT[11]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(11, 0));
	}

// 2009.02.05 K.Matsuo delete -->
//	// g[Xf[^莞M^C}N
//	if (0 != (m_uiTraceDataPeriod = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetTraceDataPeriod())) {
//		if (0 == StartTimerTraceData(m_uiTraceDataPeriod)) {
//			// ^C}Ns
//			LogChief(CHIEF_REP_ALARM_MSGTEXT[0]);
//			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(0, 0));
//		} else {
//			// ^C}N
//			m_bTraceDataTimer = TRUE;
//		}
//	}
// 2009.02.05 K.Matsuo delete <--

	// [MXbh̋N ----------------------------------------------
	LogChief(_T("Requested to start CChiefRcvMailThread."));
	m_pcChiefRcvMailThread = (CChiefRcvMailThread*) AfxBeginThread(
														RUNTIME_CLASS(CChiefRcvMailThread),
														THREAD_PRIORITY_NORMAL,
														0,
														CREATE_SUSPENDED,
														0
													);
	if (0 != m_pcChiefRcvMailThread) {
		// ɂ̓Xbh̏̏(KvȂ)Ă
		m_pcChiefRcvMailThread->m_bAutoDelete = FALSE;							// XbhIɃIuWFNgIɔjȂݒłD
		((CChiefRcvMailThread*) m_pcChiefRcvMailThread)->m_pcChiefView = this;	// e()NXXbhɓnĂ
		// XbhN
		m_pcChiefRcvMailThread->ResumeThread();
	} else {
		// [MXbhNs
		LogChief(CHIEF_REP_ALARM_MSGTEXT[10]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(10, 0));
	}

	// egWV Xbh̋N ----------------------------------------------
	m_hEvTrMaster = ::CreateEvent(0, TRUE, FALSE, 0);	// }X^gWVXbh̏҂p
	DWORD l_dwTransi;
	if (0 != (l_dwTransi = TransitionsStart())) {
		// egWV XbhNs
		LogChief(CHIEF_REP_ALARM_MSGTEXT[l_dwTransi + 3]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(l_dwTransi + 3, 0));
	}
	// }X^gWV̏҂ -----------------------------------------
	if (0 == m_hEvTrMaster) {
		// Cxg쐬s
		LogChief(CHIEF_REP_ALARM_MSGTEXT[17]);
		PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(17, 0));
	} else {
		// CHIEF_TRMAS_INIT_TIME[ms] ҂
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvTrMaster, CHIEF_TRMAS_INIT_TIME)) {
			// ^CAEg܂(}X^gWV̏܂ł)D
			LogChief(CHIEF_REP_ALARM_MSGTEXT[18]);
			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(18, 0));
		}
	}

	// DIO ĎJn^C}(A[ñEBhEオ܂ł̑҂)
	lg_uiRunFlagCount = 5000 / TIMER_10MS_PERIOD;	// DIO ĎJn^C} 5[s]

	// 胂W[փEBhEbZ[WMp Chief ̃EBhEnhʒm
	MEAS_SetNotifyWnd(m_hWnd);

// ̃Xe[Wx MainFram Ɉړ܂D
//	// ()Xe[Wx
//	if (0 == m_DiInfo.bMaintenanceSW) {
//		if (0 == m_bSpeedLimiterOFF) {
//			// x[hZbg
//			((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(TRUE);
//		}
//	} else {
//		// x[hZbg
//		((CNanoSpecDoc*) m_pcNanoSpecDoc)->StageSpeedLimiter(FALSE);
//	}

	LogChief(_T("Ended    CChiefView::OnInitialUpdate()."));

}


void CChiefView::PrepareToDestroy()
{
	// TODO: ̈ʒuɌŗL̏ǉ邩A܂͊{NXĂяoĂ
	LogChief(_T("Ending   Chief Dialog..."));

	TRACE(_T("CChiefView::PostNcDestroy() \n"));

	// EQ 쒆o͂It܂D
	nexioEquipmentStatusRun(FALSE);
// modified hmenjo 2009.05.12 xύXtOǉ ---------- { ----------
//	// ~b^LɂďI܂D
//	StageEnableSpeedLimit();
// modified hmenjo 2009.05.12 xύXtOǉ ----------
	if (TRUE == g_bIL_SpeedDown) {
		// ~b^LɂďI܂D
		StageEnableSpeedLimit();
	}
// modified hmenjo 2009.05.12 xύXtOǉ ---------- } ----------

/* added 2009.08.04 hmenjo 蒆ȊOhAJXe[W~ ---------- { ---------- */
	// }`fBA^C}폜 ---------------------------------------------
	::timeKillEvent(m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD]);
/* added 2009.08.04 hmenjo 蒆ȊOhAJXe[W~ ---------- } ---------- */

	// [MXbȟn ---------------------------------------------
	if (0 != m_pcChiefRcvMailThread) {
		LogChief(_T("Deleting CChiefRcvMailThread..."));
		((CChiefRcvMailThread*) m_pcChiefRcvMailThread)->ShutDown();
		::WaitForSingleObject(m_pcChiefRcvMailThread->m_hThread, INFINITE);
		delete m_pcChiefRcvMailThread;
		m_pcChiefRcvMailThread = NULL;
		LogChief(_T("Deleted  CChiefRcvMailThread."));
	}

	// }X^gWVI ---------------------------------------------
	// }X^gWV̏I҂
	::ResetEvent(m_hEvTrMaster);
	// }X^gWVI
	((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_END);
	if (0 != m_hEvTrMaster) {
		// CHIEF_TRMAS_END_TIME[ms] ҂
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvTrMaster, CHIEF_TRMAS_END_TIME)) {
			// ^CAEg܂(}X^gWV̏I܂ł)D
//			PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(18, 0));
			LogChief(CHIEF_REP_ALARM_MSGTITLE[1]);
			::MessageBox(0, CHIEF_REP_ALARM_MSGTEXT[23], CHIEF_REP_ALARM_MSGTITLE[1], MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
		}
	}

	// egWVXbȟn ---------------------------------------
	TransitionsEnd();

/* deleted 2009.08.04 hmenjo 蒆ȊOhAJXe[W~ ---------- { ---------- */
//	// }`fBA^C}폜 ---------------------------------------------
//	::timeKillEvent(m_uiMMTimerID[ID_MMTIMER_5MS_PERIOD]);
/* deleted 2009.08.04 hmenjo 蒆ȊOhAJXe[W~ ---------- } ---------- */

	// NextraIO I --------------------------------------------------------
//	nexioExitInstance();	hmenjo NEXIOBASE.HXX ɕς̂ŕsv
	m_dwModuleState_Nextra = 9;	// I(Terminated)

	// Pif ̏ -----------------------------------------------------------
	//		Pif ̏I͕svłD

	// Vsp
	RecipesMalloc(FALSE);

	// |C^
	g_pcChiefView = NULL;
}

/* added 2009.05.27 hmenjo SPT A[̌o ---------- { ---------- */
void CChiefView::SPTAlarmDetector(void)
{
// 2009.08.20 K.Matsuo ALID change (Ref. alarmIf.hxx) -->
	// ALID_SptAlarmStartNo	1020	// A[R[hJnԍ
	switch (StageGetMotSysErr(TRUE)) {
	case 0:												break;	// G[
	case 1:		AlarmIf_Set(ALID_SptAlarmStartNo + 0);	break;	// WD G[											ALID_SptStageWdError
	case 2:		AlarmIf_Set(ALID_SptAlarmStartNo + 1);	break;	// u[gȂ̃bZ[W 						ALID_SptStageNeedRebootPc
	case 3:		AlarmIf_Set(ALID_SptAlarmStartNo + 2);	break;	// A[ 										ALID_SptStageAlarm
	case 4:		AlarmIf_Set(ALID_SptAlarmStartNo + 3);	break;	// tOOɂȂȂ					ALID_SptStageInitCompFlagNotZero
	case 5:		AlarmIf_Set(ALID_SptAlarmStartNo + 4);	break;	// DIO G[											ALID_SptStageDioError
	case 6:		AlarmIf_Set(ALID_SptAlarmStartNo + 5);	break;	// T[{IG[(IȂ/ItȂ)		ALID_SptStageServoOnError
	case 7:		AlarmIf_Set(ALID_SptAlarmStartNo + 6);	break;	// ~(T[{IG[(IĂȂ))		ALID_SptStageEmergencyStop
	case 111:	AlarmIf_Set(ALID_SptAlarmStartNo + 7);	break;	// ʐM(Read)VXeG[ 						ALID_SptStageCommRead_SystemError
	case 112:	AlarmIf_Set(ALID_SptAlarmStartNo + 8);	break;	// ʐM(Read)gCAEgP						ALID_SptStageCommRead_RetryOut1
	case 113:	AlarmIf_Set(ALID_SptAlarmStartNo + 9);	break;	// ʐM(Read)gCAEgQ						ALID_SptStageCommRead_RetryOut2
	case 121:	AlarmIf_Set(ALID_SptAlarmStartNo + 10);	break;	// ʐM(Read2)VXeG[						ALID_SptStageCommRead2_SystemError
	case 122:	AlarmIf_Set(ALID_SptAlarmStartNo + 11);	break;	// ʐM(Read2)gCAEgP						ALID_SptStageCommRead2_RetryOut1
	case 123:	AlarmIf_Set(ALID_SptAlarmStartNo + 12);	break;	// ʐM(Read2)gCAEgQ						ALID_SptStageCommRead2_RetryOut2
	case 131:	AlarmIf_Set(ALID_SptAlarmStartNo + 13);	break;	// ʐM(Write)VXeG[						ALID_SptStageCommWrite_SystemError
	case 132:	AlarmIf_Set(ALID_SptAlarmStartNo + 14);	break;	// ʐM(Write)gCAEg						ALID_SptStageCommWrite_RetryOut
/* added 2009.11.11 hmenjo MotSys ^CAEg ---------- { ---------- */
	case 23:	AlarmIf_Set(ALID_SptAlarmStartNo + 16);	break;	/* ړ^CAEg 	  MM_TIMEOUT				ALID_SptStageMotionTimeout	*/
/* added 2009.11.11 hmenjo MotSys ^CAEg ---------- } ---------- */
	default:	AlarmIf_Set(ALID_SptAlarmStartNo + 15);	break;	// `G[ 										ALID_SptStageNotDefinedError
// 2009.08.20 K.Matsuo ALID change <--
	}
}
/* added 2009.05.27 hmenjo SPT A[̌o ---------- } ---------- */

/* added 2009.06.02 hmenjo hAJő|[Y ---------- { ---------- */
/*
 *	|[YʃW[ɒʒm
 */
void CChiefView::SentMeasPause(int iReason, BOOL bRelease/* = FALSE*/)
	/*	int iReason		|[YR
	 *	BOOL bRelease	TRUEF|[Y	*/
{
/* added 2009.06.09 hmenjo ُ펞|[Y Oǉ ---------- { ---------- */
	TCHAR l_tszLogMsg[256];
	_stprintf(l_tszLogMsg, _T("Notify WM_CHIF_NOTIFY_MEAS_PAUSE (WPARAM = 0x%08x, LPARAM = 0x%08x)"), iReason, bRelease);
	LogChief(l_tszLogMsg);
/* added 2009.06.09 hmenjo ُ펞|[Y Oǉ ---------- } ---------- */
	((CMainFrame*) m_pcMainFrame)->PostMessage(WM_CHIF_NOTIFY_MEAS_PAUSE, (WPARAM) iReason, (LPARAM) bRelease);
}
/* added 2009.06.02 hmenjo hAJő|[Y ---------- } ---------- */

/* added 2009.06.02 hmenjo hAJő|[Y ---------- { ---------- */
/*
 *	|[YʃW[ɒʒm
 */
void CChiefView::SentMeasPauseCheck(int iReason)
{
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- { ---------- */
//	if (
//		(PROCESS_PROC == ProcStatusGet())
//	 && (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsIdle())
//	 && (ST_SEQ_PAUSE != ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentState())
//	 && (false == ((CChiefTransiSeq*) m_pcChiefTransiSeq)->IsPauseSeq())
//		) {
//		// Processing ŁCV[PX肪 IDLE ƃ|[YȊȌꍇ
//		DWORD l_dwCurrPointNo = ((CChiefTransiSeq*) m_pcChiefTransiSeq)->GetCurrentPointNo();
//		LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) (m_ChiefRecipes.pStageProgInfoHdr);
//		DWORD l_dwNextPointNo = l_dwCurrPointNo + 1;
//		if (
//			(l_dwCurrPointNo < l_pStageProgInfoHdr->wNumScans)
//		 && (GetNextPointNo(&l_dwNextPointNo) <= l_pStageProgInfoHdr->wNumScans)
//			) {
//			// ŏI|CgȊOȂ|[Y܂D
//			((CChiefTransiSeq*) m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);	// |[Y
//			// |[Yʒm
//			SentMeasPause(iReason);
//		}
//	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ----------			   */
	if (PROCESS_PROC == ProcStatusGet()) {
		/* Processing ̏ꍇ̂	*/
		CHIEF_PFUNCS l_ChiefPFuncs;
		WORD l_wHeadType;
		if (0 != PFC_FuncSet(this, &l_ChiefPFuncs, &l_wHeadType)) {
			if (
				(false == (*l_ChiefPFuncs.IsIdle)(this))
			 && (l_ChiefPFuncs.State.iPAUSE != (*l_ChiefPFuncs.GetCurrentState)(this))
			 && (false == (*l_ChiefPFuncs.IsPauseSeq)(this))
				) {
				/* V[PX肪 IDLE ƃ|[YȊȌꍇ	*/
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
					// ŏI|CgȊOȂ|[Y܂D
					(*l_ChiefPFuncs.TransiEvent)(this, l_ChiefPFuncs.Event.iPAUSE, 0);	/* |[Y	*/
					// |[Yʒm
					this->SentMeasPause(iReason);
				}
			}
		}
	}
/* modified 2009.08.07 hmenjo XgX@\ǉ(37) ---------- } ---------- */
}
/* added 2009.06.02 hmenjo hAJő|[Y ---------- } ---------- */
/* added 2009.07.30 hmenjo XgX@\ǉ(3) ---------- { ---------- */
/*
 *	HEPA I/It𐧌
 */
BOOL CChiefView::HepaOnOff(BOOL bOn, char* pcHepaAlarmLevel/*= 0*/)
{
	STRESS_CONFIG l_StressConfig;
	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	l_StressConfig.dwScanStartWaitTime;	/* HEPA ҂[s]	*/
	if (0 == l_StressConfig.dwScanStartWaitTime) {
		/* ҂ԂO[s]Ȃ̂ HEPA 䂵ȂD	*/
		if (FALSE == bOn) {
			((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_HEPA_STOP, (cEventParams*) EV_STRS_HEPA_STOP);
		}
		if (0 != pcHepaAlarmLevel) {*pcHepaAlarmLevel = 0;}
		return TRUE;
	}

	BOOL l_bRet = ((CNanoSpecDoc*) m_pcNanoSpecDoc)->HepaOnOff(bOn, pcHepaAlarmLevel);

	if ((TRUE == l_bRet) && (FALSE == bOn)) {
		/* HEPA Itw߂ŐIȂ̂ŁC҂^C}Jn	*/
		if (ID_TIMER_HEPASTOP != this->SetTimer(ID_TIMER_HEPASTOP, l_StressConfig.dwScanStartWaitTime * 1000, 0)) {
			/* ^C}Ns	*/
			((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_HEPA_STOP, (cEventParams*) EV_STRS_HEPA_STOP);
			this->LogChief(CHIEF_REP_ALARM_MSGTEXT[37]);
			this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_MSGBOX, CHRANFY_NOTIFY_ON), MAKEWORD(1, CHRAMSG_YESNO)), MAKELPARAM(37, 0));
		}
	}

	return l_bRet;
}
/* added 2009.07.30 hmenjo XgX@\ǉ(3) ---------- } ---------- */
/* added 2009.08.05 hmenjo XgX@\ǉ(25) ---------- { ---------- */
/*
 *	PC芮 bZ[Wnh (XgXp)
 *		胂W[łPCf[^̎擾D
 */
LRESULT CChiefView::OnMeasLineEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MEAS_LINE_END"), wparam, lparam);

	((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LINEMEAS_DONE, (cEventParams*) MAKELONG(EV_STRS_LINEMEAS_DONE, LOWORD(wparam)));

	return 0L;
}
/*
 *	PCf[^() bZ[Wnh (XgXp)
 *		f[^W[łPCf[^̏D
 */
LRESULT CChiefView::OnDataLineEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_LINE_END"), wparam, lparam);

	((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_LINEDATA_DONE, (cEventParams*) EV_STRS_LINEDATA_DONE);

	return 0L;
}
/*
 *	PI bZ[Wnh (XgXp)
 *		f[^W[łPf[^̏D
 */
LRESULT CChiefView::OnDataStressMeasEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DATA_STRS_MEAS_END"), wparam, lparam);

	((CChiefTransiStress*) m_pcChiefTransiStress)->TransiEvent(EV_STRS_COMPLETE, (cEventParams*) EV_STRS_COMPLETE);

	return 0L;
}
/* added 2009.08.05 hmenjo XgX@\ǉ(25) ---------- } ---------- */
/* added 2009.08.06 hmenjo XgX@\ǉ(31) ---------- { ---------- */
BOOL CChiefView::IsHWS()
{
	return ((CMainFrame*) m_pcMainFrame)->HardwareSimulation();
}
/* added 2009.08.06 hmenjo XgX@\ǉ(31) ---------- } ---------- */

/* added 2009.08.25 hmenjo SE vtB^֐ ---------- { ---------- */
/*
 *	vtB^q
 */
#define	LFR_LOG	1	/* 1FOo͂	*/
BOOL CChiefView::LampFilterR(
		WORD wFilter,	/* dlɂ	ftHgF0	*/
		WORD wMode		/* dlɂ	ftHgF0	*/
	)
{
	BOOL l_bRet = TRUE;
	WORD l_wFilter = wFilter;
	WORD l_wMode = wMode;

/* modified 2009.11.20 hmenjo GTR v Close ---------- { ---------- */
/*		Rg폜܂D(ɂ̂)	*/
///* added 2009.09.14 hmenjo LampFilterR() Oǉ ---------- { ---------- */
//	BOOL l_bLogWR = (LFR_LOG)? TRUE : FALSE;
///* added 2009.09.14 hmenjo LampFilterR() Oǉ ---------- } ---------- */
//
//	switch (l_wMode) {
///* modified 2009.09.07 hmenjo ߗ v ---------- { ---------- */
////	case 1:	/* SE wbh̏ꍇ --------------------------------------------------------*/
////	case 2:	/* SE wbhŒʏ탂[h̏ꍇ --------------------------------------------*/
////		if (0 != m_ChiefRecipes.pMeasProgInfo) {
////			if (HEAD_TYPE_SE == ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType) {
////				/* SE wbh	*/
/////* modified 2009.09.01 hmenjo SE \̒ǉ(12) ---------- { ---------- */
//////				SE_CONFIG l_SeConfig;
//////				ConfigFile_GetNanoSpecIni(&l_SeConfig, CONFIG_FILE_SE_CONFIG);
//////				if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeConfig.bDoNotMoveShutter))) {
/////* modified 2009.09.01 hmenjo SE \̒ǉ(12) ----------				*/
////				SE_SETTING l_SeSetting;
////				ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
////				if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeSetting.bDoNotMoveShutter))) {
/////* modified 2009.09.01 hmenjo SE \̒ǉ(12) ---------- } ---------- */
////					/* [hPC[hQŒʏ탂[h̏ꍇ̂ݎs܂D	*/
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
///* modified 2009.09.07 hmenjo ߗ v ----------			  */
//	case 1:	/* Ŏs -----------------------------------------------------------*/
//	case 2:	/* ʏ탂[h̏ꍇ̂ݎs -----------------------------------------------*/
//		if (0 != m_ChiefRecipes.pMeasProgInfo) {
//			WORD l_wHeadType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType;
//			switch (l_wHeadType) {
///* added 2009.10.29 hmenjo CTA  SR tB^ CLOSE ---------- { ---------- */
//			case HEAD_TYPE_CTA:
///* added 2009.11.06 K.Matsuo RS  SR tB^ CLOSE ---------- { ---------- */
//			case HEAD_TYPE_4PP:
///* added 2009.11.06 K.Matsuo RS  SR tB^ CLOSE ---------- } ---------- */
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
///* added 2009.10.29 hmenjo CTA  SR tB^ CLOSE ---------- } ---------- */
//			case HEAD_TYPE_SE:
//				{
//					SE_SETTING l_SeSetting;
//					ConfigFile_GetNanoSpecIni(&l_SeSetting, CONFIG_FILE_SE_SETTING);
//					if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SeSetting.bDoNotMoveShutter))) {
//						/* [hPC[hQŒʏ탂[h̏ꍇ̂ݎs܂D	*/
//						switch (l_wFilter) {
//						case FILTER_OPEN:
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- { ---------- */
/////* modified 2009.09.14 hmenjo SE  SR v OPEN ł悢 ---------- { ---------- */
//////							MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
/////* modified 2009.09.14 hmenjo SE  SR v OPEN ł悢 ----------				*/
////							MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/////*hdebdeb*/this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));
/////* modified 2009.09.14 hmenjo SE  SR v OPEN ł悢 ---------- } ---------- */
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- 			 */
//							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- } ---------- */
//							MEAS_SeHead_OpenLampShutter();
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_OPEN-1,2"));}
//							break;
//						case FILTER_DARK:
//						default:
//							MEAS_SeHead_CloseLampShutter();
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SE  : FILTER_DARK-1,2"));}
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- { ---------- */
////							MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/////*hdebdeb*/this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- 			 */
//							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- } ---------- */
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
//						/* SR wbhœߗ̏ꍇ	*/
//						SR_TRANSMIT l_SrTransmittance;
//						ConfigFile_GetNanoSpecIni(&l_SrTransmittance, CONFIG_FILE_SR_TRANSMIT);
//						if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SrTransmittance.bDoNotCheckMeasPoint))) {
//							/* [hPC[hQŒʏ탂[h̏ꍇ̂ݎs܂D	*/
//							switch (l_wFilter) {
//							case FILTER_OPEN:
//								MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-1,2"));}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- { ---------- */
////								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
///* modified 2009.09.09 hmenjo ߗ v REF(W) ----------			  */
//								if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//									MEAS_SrHead_OpenTransShutter();
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_OPEN-1,2"));}
//								} else {
//									((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_OPEN-1,2"));}
//								}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- } ---------- */
//								break;
//							case FILTER_DARK:
//							default:
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- { ---------- */
////								MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- 			 */
//								MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SR.wOpticsFilterType);
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- } ---------- */
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-1,2"));}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- { ---------- */
////								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
///* modified 2009.09.09 hmenjo ߗ v REF(W) ----------			  */
//								if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//									MEAS_SrHead_CloseTransShutter();
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_DARK-1,2"));}
//								} else {
//									((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
//									if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_DARK-1,2"));}
//								}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- } ---------- */
//								break;
//							}
//						}
//					}
//				}
//				break;
//			default:
//				/* ܂D	*/
//				break;
//			}
//		}
///* modified 2009.09.07 hmenjo ߗ v ---------- } ---------- */
//		break;
///* added 2009.09.08 hmenjo ߗ v REF(W) ---------- { ---------- */
//	case 3:	/* ƂŎw肳ꂽԂɂ -----------------------------------*/
//		if (0 != m_ChiefRecipes.pMeasProgInfo) {
//			WORD l_wHeadType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wHeadType;
//			switch (l_wHeadType) {
//			case HEAD_TYPE_SE:
//				switch (l_wFilter) {
//				case FILTER_OPEN:
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- { ---------- */
////					MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- 			 */
//					MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SE.wOpticsFilterType);
//					if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- } ---------- */
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
//						/* SR wbhœߗ̏ꍇ	*/
//						switch (l_wFilter) {
//						case FILTER_OPEN:
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- { ---------- */
////							MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- 			 */
//							MEAS_SrHead_ChangeCcdShutter(((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams._SR.wOpticsFilterType);
///* modified 2009.09.14 hmenjo LampFilterR SR ͑ PGM ݒl ---------- } ---------- */
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_OPEN-3"));}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- { ---------- */
////							((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
///* modified 2009.09.09 hmenjo ߗ v REF(W) ----------			  */
//							if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//								MEAS_SrHead_OpenTransShutter();
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_OPEN-3"));}
//							} else {
//								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_OPEN);
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_OPEN-3"));}
//							}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- } ---------- */
//							break;
//						case FILTER_DARK:
//						default:
//							MEAS_SrHead_ChangeCcdShutter(FILTER_DARK);
//							if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - SR  : FILTER_DARK-3"));}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- { ---------- */
////							((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
///* modified 2009.09.09 hmenjo ߗ v REF(W) ----------			  */
//							if (MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType) {
//								MEAS_SrHead_CloseTransShutter();
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - TR  : FILTER_DARK-3"));}
//							} else {
//								((CNanoSpecDoc*) m_pcNanoSpecDoc)->SetTransmittanceLamp(FILTER_DARK);
//								if (0 != l_bLogWR) {this->LogChief(_T("@@@@@@@@@@ - GTR : FILTER_DARK-3"));}
//							}
///* modified 2009.09.09 hmenjo ߗ v REF(W) ---------- } ---------- */
//							break;
//						}
//					}
//				}
//				break;
//			default:
//				/* ܂D	*/
//				break;
//			}
//		}
//		break;
///* added 2009.09.08 hmenjo ߗ v REF(W) ---------- } ---------- */
//	default:
//		/* ܂D	*/
//		break;
//	}
/* modified 2009.11.20 hmenjo GTR v Close ----------			   */
// 2013.11.07 Bagus Add (TohoSpecΉ) -->
	if(g_lModelType == MODEL_T3100){
		// I[gtB^[͎sɏI
		return l_bRet;
	}
// 2013.11.07 Bagus Add (TohoSpecΉ) <--

	BOOL l_bLogWR = (LFR_LOG)? TRUE : FALSE;
	BOOL l_bGTRLampOK = TRUE;

	switch (l_wMode) {
	case 1:	/* Ŏs -----------------------------------------------------------*/
	case 2:	/* ʏ탂[h̏ꍇ̂ݎs -----------------------------------------------*/
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
						/* [hPC[hQŒʏ탂[h̏ꍇ̂ݎs܂D	*/
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
// 2013.02.01 bagus CompleteEASEwbhǉ -->
			case HEAD_TYPE_COMPEASE:
				{
					if( ((CNanoSpecDoc*) m_pcNanoSpecDoc)->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
						COMPEASE_SETTING l_EASESetting;
						ConfigFile_GetNanoSpecIni(&l_EASESetting, CONFIG_FILE_COMPEASE_SETTING);
						if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_EASESetting.bDoNotMoveShutter))) {
							/* [hPC[hQŒʏ탂[h̏ꍇ̂ݎs܂D	*/
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
// 2013.02.01 bagus CompleteEASEwbhǉ <--
			case HEAD_TYPE_SR:
				{
					WORD l_wScanType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wScanType;
					if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType)
					 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_wScanType)) {
						/* SR wbhœߗ̏ꍇ	*/
						SR_TRANSMIT l_SrTransmittance;
						ConfigFile_GetNanoSpecIni(&l_SrTransmittance, CONFIG_FILE_SR_TRANSMIT);
						if ((1 == l_wMode) || ((2 == l_wMode) && (FALSE == l_SrTransmittance.bDoNotCheckMeasPoint))) {
							/* [hPC[hQŒʏ탂[h̏ꍇ̂ݎs܂D	*/
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
				/* ܂D	*/
				break;
			}
		}
		break;
	case 3:	/* ƂŎw肳ꂽԂɂ -----------------------------------*/
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
// 2013.02.01 bagus CompleteEASEwbhǉ -->
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
// 2013.02.01 bagus CompleteEASEwbhǉ <--
			case HEAD_TYPE_SR:
				{
					WORD l_wScanType = ((LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo)->ScanParams.hdr.wScanType;
					if ((MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_wScanType)
					 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_wScanType)) {
						/* SR wbhœߗ̏ꍇ	*/
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
				/* ܂D	*/
				break;
			}
		}
		break;
	default:
		/* ܂D	*/
		break;
	}

/* modified 2009.11.24 hmenjo GTR vG[oC ---------- { ---------- */
//	if (TRUE != l_bGTRLampOK) {
/* modified 2009.11.24 hmenjo GTR vG[oC ----------			   */
	if ((FILTER_OPEN == l_wFilter) && (TRUE != l_bGTRLampOK)) {
		/* I[vw߂̂Ƃ̂݌o܂D	*/
/* modified 2009.11.24 hmenjo GTR vG[oC ---------- } ---------- */
		/* GTR vG[	*/
		if (0 == ((CMainFrame*) m_pcMainFrame)->HardwareSimulation()) {
			AlarmIf_Set(ALID_GTR_Lamp_Error);
		}
	}
/* modified 2009.11.20 hmenjo GTR v Close ---------- } ---------- */

	return l_bRet;
}
/* added 2009.08.25 hmenjo SE vtB^֐ ---------- } ---------- */
/* added 2009.09.07 hmenjo  PGM YǏo ---------- { ---------- */
/*
 *	wbh̑ PGM 烌YݒlǏo܂D
 */
int CChiefView::GetMeasLens(WORD wHeadType/* = HEAD_TYPE_SR*/)
{
	int l_iLens;

	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;

	if (0 == l_pMeasProgInfo) {
		return 0;	/* Vsݒ	*/
	}

	switch (wHeadType) {
	case HEAD_TYPE_SE:		l_iLens = l_pMeasProgInfo->ScanParams._SE.iLens;	break;
// 2013.02.01 bagus CompleteEASEwbhǉ -->
	case HEAD_TYPE_COMPEASE:l_iLens = l_pMeasProgInfo->ScanParams._COMPEASE.iLens;	break;
// 2013.02.01 bagus CompleteEASEwbhǉ <--
// 2009.10.19 bagus MS ǉ --{--
//	case HEAD_TYPE_IRSE:	l_iLens = l_pMeasProgInfo->ScanParams._IR.iLens;	break;
	case HEAD_TYPE_MS:		l_iLens = l_pMeasProgInfo->ScanParams._MScope.iLens;	break;
// 2009.10.19 bagus MS ǉ --}--
	case HEAD_TYPE_4PP:		l_iLens = l_pMeasProgInfo->ScanParams._RS.iLens;	break;
	case HEAD_TYPE_CTA:		l_iLens = l_pMeasProgInfo->ScanParams._CA.iLens;	break;
	/* ȉ̓ftHgł SR Ǐo܂D	*/
	case HEAD_TYPE_SR:
	case HEAD_TYPE_STRESS:
	default:				l_iLens = l_pMeasProgInfo->ScanParams._SR.iLens;	break;
	}

	return l_iLens;
}
/* added 2009.09.07 hmenjo  PGM YǏo ---------- } ---------- */
/* added 2009.09.07 hmenjo  PGM tB^Ǐo ---------- { ---------- */
/*
 *	wbh̑ PGM tB^ݒlǏo܂D
 */
WORD CChiefView::GetMeasFilter(WORD wHeadType/* = HEAD_TYPE_SR*/)
{
	WORD l_wFilter;

	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) m_ChiefRecipes.pMeasProgInfo;

	if (0 == l_pMeasProgInfo) {
		return FILTER_OPEN;	/* Vsݒ	*/
	}

	switch (wHeadType) {
	case HEAD_TYPE_SE:		l_wFilter = l_pMeasProgInfo->ScanParams._SE.wOpticsFilterType;	break;
// 2013.02.01 bagus CompleteEASEwbhǉ -->
	case HEAD_TYPE_COMPEASE:l_wFilter = l_pMeasProgInfo->ScanParams._COMPEASE.wOpticsFilterType;	break;
// 2013.02.01 bagus CompleteEASEwbhǉ <--
// 2009.10.19 bagus MS ǉ --{--
//	case HEAD_TYPE_IRSE:	l_wFilter = l_pMeasProgInfo->ScanParams._IR.wOpticsFilterType;	break;
	case HEAD_TYPE_MS:		l_wFilter = l_pMeasProgInfo->ScanParams._MScope.wOpticsFilterType;	break;
// 2009.10.19 bagus MS ǉ --}--
	case HEAD_TYPE_4PP:		l_wFilter = l_pMeasProgInfo->ScanParams._RS.wOpticsFilterType;	break;
	case HEAD_TYPE_CTA:		l_wFilter = l_pMeasProgInfo->ScanParams._CA.wOpticsFilterType;	break;
	/* ȉ̓ftHgł SR Ǐo܂D	*/
	case HEAD_TYPE_SR:
	case HEAD_TYPE_STRESS:
	default:				l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;	break;
	}

/* added 2009.11.30 hmenjo  Seq ͎wwbh()œ ---------- { ---------- */
	/*	@PGM@OĂ܂ꍇAO͎dl゠肦Ȃ̂ŁA
		FILTER_OPEN@ɂĕԂ܂D	*/
	if (0 == l_wFilter) {
		l_wFilter = FILTER_OPEN;
	}
/* added 2009.11.30 hmenjo  Seq ͎wwbh()œ ---------- } ---------- */

	return l_wFilter;
}
/* added 2009.09.07 hmenjo  PGM tB^Ǐo ---------- } ---------- */
/* added 2009.09.10 hmenjo t@Xf[^`FbN֐ǉ ---------- { ---------- */
/*
 *	t@Xf[^`FbN܂D
 *		߂lF0FLCCf[^t@C݂
 *				2FLOCCf[^t@C݂
 *				1Ff[^t@C݂Ȃ
 */
int CChiefView::CheckReferenceData(TCHAR* ptszMainRcpName, double dLifeTime, BOOL bRef2ndMeasure/* = FALSE*/)
{
	/* t@Xf[^t@C݂̑ƗL`FbN܂D	*/
	int l_iRet = MEAS_CheckRefFileElapsedTimeOut(ptszMainRcpName, dLifeTime);
	switch (l_iRet) {
	case 0:		/* LCCt@C݂	*/
		break;
	case 2:		/* LOCCt@C݂	*/
		break;
	case 1:		/* t@C݂Ȃ	*/
	default:	/* ُ̑̈	*/
		l_iRet = 1;		/* t@C݂Ȃ	*/
		break;
	}

	/* ZJht@Xgꍇ	*/
	if ((1 != l_iRet) && (TRUE == bRef2ndMeasure)) {
		/* t@C݂ꍇ́CZJht@X̃f[^t@C̑݊mF܂D	*/
		if ((0 == MEAS_Is2ndRefT1FileExist(ptszMainRcpName))
		 || (0 == MEAS_Is2ndRefT2FileExist(ptszMainRcpName))) {
			/* f[^t@C(uZJh t@X T1 f[^v܂́uZJh t@X T2 f[^v)	*/
			l_iRet = 1;		/* ZJht@Xt@C݂Ȃ	*/
		}
	}

	return l_iRet;
}
/* added 2009.09.10 hmenjo t@Xf[^`FbN֐ǉ ---------- } ---------- */

/* added 2009.09.29 hmenjo Head  Main Rcp Inf Ǐo ---------- { ---------- */
/*
 *	wbh̃CVsǏo܂D
 *		߂lF0F
 *				1Fs(p^ُ)
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
// 2013.02.01 bagus CompleteEASEwbhǉ -->
	case HEAD_TYPE_COMPEASE:
		pHeadMainRcpInf->nFocus = pMainRcpInfo->MainRcpParam._COMPEASE.nFocus;
		pHeadMainRcpInf->nAutoFocusFailOption = pMainRcpInfo->MainRcpParam._COMPEASE.nAutoFocusFailOption;
		break;
// 2013.02.01 bagus CompleteEASEwbhǉ <--
// 2009.10.19 bagus MS ǉ --{--
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
// 2009.10.19 bagus MS ǉ --}--
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
/* added 2009.09.29 hmenjo Head  Main Rcp Inf Ǐo ---------- } ---------- */
//2009.10.28 bagus 2ponit-distance --{--
LRESULT CChiefView::OnDistancePopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_DISTANCE_POPUP_END"), wparam, lparam);

	switch (wparam) {
	case 0:		// LZ
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_ERR, (cEventParams*) lparam);
		break;
	case 1:		// 
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_DONE, (cEventParams*) lparam);
		break;
	case 2:		// IDLE֖߂ă_CNgɏI
	case 3:		//
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_IDLE, (cEventParams*) lparam);
		//((CChiefTransiMaster*) m_pcChiefTransiMaster)->TransiEvent(EV_MAS_UI1POINT_DONE, (cEventParams*) lparam);
		break;
	default:
		// ܂D
		break;
	}

	return 0L;
}
//2009.10.28 bagus 2ponit-distance --}--

/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- { ---------- */
/*
 *	CTA jbg IL M`FbN܂D
 *		TRUE FI
 *		FALSEFIt
 */
BOOL CChiefView::IsCtaILPI()
{
	BOOL l_bRet;

	if (0 == this->m_EnableHead.bCTA) {
		l_bRet = TRUE;
	} else {
		/* CTA L̏ꍇ	*/
		if (0 == nexioIsCA_Interlock()) {
			l_bRet = FALSE;
		} else {
			l_bRet = TRUE;
		}
	}

	return l_bRet;
}
/* added 2009.10.29 hmenjo CTA Seq CTAILPI `FbN ---------- } ---------- */

// 2009.11.12 bagus MS --{--
/*
 *	MS jbg IL M`FbN܂D([MQOFFĂNGjB
 *		TRUE FI
 *		FALSEFIt
 */
BOOL CChiefView::IsMSILPI()
{
	BOOL l_bRet;

	if (0 == this->m_EnableHead.bMS) {
		l_bRet = TRUE;
	} else {
		/* MS L̏ꍇ	*/
		if (0 == nexioIsMS_LowerPos2()) {
			l_bRet = FALSE;
		} else {
			l_bRet = TRUE;
		}
	}

	return l_bRet;
}
// 2009.11.12 bagus MS --}--

/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- { ---------- */
/*
 *	RS jbg IL M`FbN܂D
 *		TRUE FI
 *		FALSEFIt
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
		/* Resit L̏ꍇ	*/
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
/* added 2009.11.06 K.Matsuo RS Seq Head Position `FbN ---------- } ---------- */

/* added 2009.10.30 hmenjo CTA A[nh ---------- { ---------- */
/*
 *	CTA jbg̃A[܂D
 *		LOWORD(wparam)FA[ ID		[100 ` 169  70 ]
 *		HIWORD(wparam)FA[x	[0FxC0Fُ]
 */
LRESULT CChiefView::OnMeasAlarmCTA(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_MEAS_CTA_ALARM"), wparam, lparam);

	DWORD l_dwAlarmID = LOWORD(wparam);
	DWORD l_dwAlarmLevel = HIWORD(wparam);

	AlarmIf_Set(l_dwAlarmID + 20);

/* added 2009.11.06 hmenjo CTA CTA G[ Abort ---------- { ---------- */
	if (0 != l_dwAlarmLevel) {
		/* d̏ Abort ɂ܂D	*/
		if (0 != this->m_EnableHead.bCTA) {
			this->CancelSeqForAlarm();
		}
	}
/* added 2009.11.06 hmenjo CTA CTA G[ Abort ---------- } ---------- */

	return 0L;
}
/* added 2009.10.30 hmenjo CTA A[nh ---------- } ---------- */
//2009.11.03 bagus MS --{--
LRESULT CChiefView::OnMSPopupEnd(WPARAM wparam, LPARAM lparam)
{
	LogChief_WinMsg(_T("WM_DISP_MS_POPUP_END"), wparam, lparam);

	switch (wparam) {
	case 0:		// LZ
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_ERR, (cEventParams*) lparam);
		break;
	case 1:		// 
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_MEAS_DONE, (cEventParams*) lparam);
		break;
	case 2:		// IDLE֖߂ă_CNgɏI
	case 3:		//
		((CChiefTransiSr1Point*) m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_IDLE, (cEventParams*) lparam);
		break;
	default:
		// ܂D
		break;
	}
	return 0L;
}
//2009.11.03 bagus MS --}--

/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- { ---------- */
/*
 *	CTA Zbg
 *		  FDWORD dwMode
 *					0FCTA A[擾
 *					1FCTA A[Zbg
 *					ȊOFCTA A[NACCTA jbgZbg
 *		߂lF
 *					0FCTA A[ł͂܂
 *					ȊOFCTA A[ł
 */
int CChiefView::CtaReset(DWORD dwMode/* = 0*/)
{
	static BOOL ls_bCTAAlarm = FALSE;

	if (0 != this->m_EnableHead.bCTA) {
		switch (dwMode) {
		case 0:		/* CTA A[擾	*/
			/* ̃[hɏ͂܂D	*/
			break;
		case 1:		/* CTA A[Zbg	*/
			ls_bCTAAlarm = TRUE;
			break;
		default:	/* CTA A[NACCTA jbgZbg	*/
			if (0 != ls_bCTAAlarm) {
				long l_lCtaStatus;
				BOOL l_bRslt = MEAS_CtaGetStatus(&l_lCtaStatus);
				if (0 != l_bRslt) {
					/* 擾튮	*/
					int l_iHead		= LOBYTE(LOWORD(l_lCtaStatus));	/* wbh	*/
					int l_iStatus1	= HIBYTE(LOWORD(l_lCtaStatus));	/* ԂP	*/
					int l_iStatus2	= LOBYTE(HIWORD(l_lCtaStatus));	/* ԂQ	*/
					int l_iReserve	= HIBYTE(HIWORD(l_lCtaStatus));	/* (gp)	*/
					if (((1 != l_iHead) && (2 != l_iHead))
					 || (2 != l_iStatus1)
					 || (1 != l_iStatus2)) {
						/*	wbhu_ʒuFPvƁuҋ@ʒuFQvȊȌꍇC
							ԂPuҋ@FQvȊȌꍇC
							ԂQuPFG[vvȊȌꍇ	*/
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
/* added 2009.11.06 hmenjo CTA A[Zbgُ탊Zbg ---------- } ---------- */

/* added 2012.01.23 hmenjo [V]A{[g΍ ---------- { ---------- */
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
			/* sO	*/
			this->LogChief((LPTSTR) ((LPCTSTR) l_strMsg));
		}
		::Sleep(CHIEF_TREVENT_RETRY_INTERVAL);	/* gCԊu	*/
	}
	if (FALSE == l_bRslt) {
		/* gCAEg	*/
		this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_BOTH, CHRANFY_NOTIFY_ON), MAKEWORD(byTransiKind, CHRAMSG_YESNO)), MAKELPARAM(39, ALID_ChiefTransi_Failed_PostThreadMessage));
	}
}
extern UINT WM_CHIF_TRAF_SETEVENT;		// 3FCChiefTransiAF
extern UINT WM_CHIF_TRDSK_SETEVENT;		// 4FCChiefTransiDeskew
extern UINT WM_CHIF_TRMAS_SETEVENT;		// 5FCChiefTransiMaster
extern UINT WM_CHIF_TRSEQ_SETEVENT;		// 6FCChiefTransiSeq
extern UINT WM_CHIF_TR1P_SETEVENT;		// 7FCChiefTransiSr1Point
extern UINT WM_CHIF_TRREF_SETEVENT;		// 8FCChiefTransiSrRefer
extern UINT WM_CHIF_TRSTRS_SETEVENT;	// 9FCChiefTransiStress
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
			/* ^CAEgF莞Ԍo߂ĂrW[łD	*/
			l_bBusy = TRUE;
			/* V[PXŃEFCgŃAChɑJڂԂȂ狭ACh	*/
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
						/* ^CAEgF莞Ԍo߂ĂrW[łD	*/
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
		/* A[	*/
		this->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_BOTH, CHRANFY_NOTIFY_ON), MAKEWORD(byTransiKindSrc, CHRAMSG_YESNO)), MAKELPARAM(40, ALID_ChiefTransi_BusyTransition));
	}

	return l_bBusy;
}
/* added 2012.01.23 hmenjo [V]A{[g΍ ---------- } ---------- */

// 2013.02.01 bagus CompleteEASEwbhǉ -->
//
//	CompEASȄԃXe[^X܂D
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
	// A[ǉAOnMeasCompEASEError()֐ŎgpĂA[ԍɒӂĂ
	default:
		return 0L;
	}

	LogChief_WinMsg(_T("WM_MEAS_COMPEASE_STATUS"), wparam, lparam);

	AlarmIf_Set(l_dwALID);

	return 0L;
}

//
//	CompEASẼG[܂D
//
LRESULT CChiefView::OnMeasCompEASEError(WPARAM wparam, LPARAM lparam)
{
	DWORD l_dwStatus = (wparam);
	DWORD l_dwALID;

	switch ( l_dwStatus ) {
	case EASE_ERROR_RUN_RECIPE_NOT_FOUND:
		l_dwALID = 197;
		break;
	// A[ǉAOnMeasCompEASEStatus()֐ŎgpĂA[ԍɒӂĂ
	default:
		// ASSERT(FALSE);
		return 0L;
	}

	LogChief_WinMsg(_T("WM_MEAS_COMPEASE_ERROR"), wparam, lparam);

//	AlarmIf_Set(l_dwALID);	// 2013.02.25 ProconւłȂ߁AbRgAEg

	return 0L;
}
// 2013.02.01 bagus CompleteEASEwbhǉ <--
