// MeasurementProcessView.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MeasurementListDlg.h"
#include "MeasurementGraphDlg.h"
#include "MeasurementTabView.h"
#include "MeasurementProcessView.h"
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
#include "OverlayDialog.h"
#include "PointSetupTeachingMSDlg.h"
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--
#include "NanoSpecDoc.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define JOG_WATCH_TIMER_ID				(1)
#define JOG_WATCH_TIMER_INTERVAL		(10)
#define JOG_MODE_LAMP_TIMER_ID			(2)
#define JOG_MODE_LAMP_TIMER_INTERVAL	(1000)

// #########################################################################
// CMeasurementProcessView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CMeasurementProcessView, CNanoUI)

// =========================================================================
//
CMeasurementProcessView::CMeasurementProcessView()
	: CNanoUI(CMeasurementProcessView::IDD)
{
	//{{AFX_DATA_INIT(CMeasurementProcessView)
	//}}AFX_DATA_INIT

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
}

// =========================================================================
//
CMeasurementProcessView::~CMeasurementProcessView()
{
}

// =========================================================================
//
void CMeasurementProcessView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementProcessView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_MEASUREMENT_CANCEL_BUTTON, m_MeasurementCancelButton);
	DDX_Control(pDX, IDC_MEASUREMENT_PAUSE_BUTTON, m_MeasurementPauseButton);
	DDX_Control(pDX, IDC_REMEASUREMENT_BUTTON, m_RemeasurementButton);
	DDX_Control(pDX, IDC_ADUJST_REMEASUREMENT_BUTTON, m_AdujstRemeasurementButton);
	DDX_Control(pDX, IDC_VALUES_TO_DISPLAY_BUTTON, m_ValuesToDisplayButton);
	DDX_Control(pDX, IDC_SAVE_BUTTON, m_SaveButton);
	DDX_Control(pDX, IDC_PRINT_BUTTON, m_PrintButton);
	DDX_Control(pDX, IDC_EXIT_BUTTON, m_ExitButton);
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
	DDX_Control(pDX, IDC_NEXT_POINT_BUTTON, m_NextPointButton);
	DDX_Control(pDX, IDC_POINT_TEACHING_BUTTON, m_PointTeachingButton);
	DDX_Control(pDX, IDC_STAGE_UP_BUTTON, m_XY_UpButton);
	DDX_Control(pDX, IDC_STAGE_DOWN_BUTTON, m_XY_DownButton);
	DDX_Control(pDX, IDC_STAGE_RIGHT_BUTTON, m_XY_RightButton);
	DDX_Control(pDX, IDC_STAGE_LEFT_BUTTON, m_XY_LeftButton);
	DDX_Control(pDX, IDC_STAGE_MOTION_MODE, m_cbMotionStage);
	DDX_Control(pDX, IDC_JOYSTICK1, *m_joyStick1);
	DDX_Control(pDX, IDC_JOYSTICK2, *m_joyStick2);
	DDX_Control(pDX, IDC_MEAS_POINT_X, m_stcMeasurementPointX);
	DDX_Control(pDX, IDC_MEAS_POINT_Y, m_stcMeasurementPointY);
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMeasurementProcessView, CNanoUI)
	//{{AFX_MSG_MAP(CMeasurementProcessView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MEASUREMENT_CANCEL_BUTTON, OnMeasurementCancelButton)
	ON_BN_CLICKED(IDC_MEASUREMENT_PAUSE_BUTTON, OnMeasurementPauseButton)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_BN_CLICKED(IDC_REMEASUREMENT_BUTTON, OnRemeasurementButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, OnSaveButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ADUJST_REMEASUREMENT_BUTTON, OnAdujstRemeasurementButton)
	ON_BN_CLICKED(IDC_NEXT_POINT_BUTTON, OnNextPointButton)
	ON_BN_CLICKED(IDC_POINT_TEACHING_BUTTON, OnPointTeachingButton)
	ON_WM_DESTROY()
	ON_CBN_SELENDOK(IDC_MICRO_SCOPE_FOCUS_POSITION, OnSelendokMicroScopeFocusPosition)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_DISPMENU_MEASUREMENT_CANCEL, OnMeaProcFuncCall)
	ON_MESSAGE(WM_DISPMENU_MEASUREMENT_RESUME, OnMeaProcFuncCall)
	ON_MESSAGE(WM_DISPMENU_MEASUREMENT_PAUSE, OnMeaProcFuncCall)
	ON_MESSAGE(WM_DISPMENU_REMEASUREMENT, OnMeaProcFuncCall)
	ON_MESSAGE(WM_DISPMENU_ADUJST_REMEASUREMENT, OnMeaProcFuncCall)
	ON_MESSAGE(WM_DISPMENU_SEQMEAS_EXIT, OnMeaProcFuncCall)
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
	ON_MESSAGE(WM_DISPMENU_POINT_TEACHING, OnMeaProcFuncCall)
	ON_MESSAGE(WM_JOYSTICK, OnJoyStick)
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
END_MESSAGE_MAP()

// =========================================================================
// CMeasurementProcessView ï¿½fï¿½f

#ifdef _DEBUG
// =========================================================================
//
void CMeasurementProcessView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CMeasurementProcessView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CMeasurementProcessView ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½

// =========================================================================
//
int CMeasurementProcessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNanoUI::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉŒÅ—Lï¿½Ì�ì�¬ï¿½pï¿½Rï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	return 0;
}

// =========================================================================
//
void CMeasurementProcessView::OnInitialUpdate()
{
	//ï¿½ï¿½ï¿½Î‰ï¿½ï¿½@ï¿½\HIDE
	GetDlgItem(IDC_VALUES_TO_DISPLAY_BUTTON)->ShowWindow(SW_HIDE);

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// ï¿½ï¿½Ê‚ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é��ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Íƒ\ï¿½tï¿½gJoyï¿½Æ‚ï¿½ï¿½ï¿½
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

	// Radioï¿½{ï¿½^ï¿½ï¿½ï¿½Ì�İ’ï¿½
	m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
	m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --{--
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--

	CNanoUI::OnInitialUpdate();

	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉŒÅ—Lï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Aï¿½Ü‚ï¿½ï¿½ÍŠï¿½{ï¿½Nï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
//	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	///// Measurement Cancel Button /////
	m_MeasurementCancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementCancelButton.DrawFlatFocus(TRUE);

	///// Measurement Pause Button /////
	m_MeasurementPauseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementPauseButton.DrawFlatFocus(TRUE);

	///// Remeasurement Button /////
	m_RemeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RemeasurementButton.DrawFlatFocus(TRUE);

	///// AdujstRemeasurement Button /////
	m_AdujstRemeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AdujstRemeasurementButton.DrawFlatFocus(TRUE);

	///// Values To Display Button /////
	m_ValuesToDisplayButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ValuesToDisplayButton.DrawFlatFocus(TRUE);

	///// Save Button /////
	m_SaveButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SaveButton.DrawFlatFocus(TRUE);

	///// Print Button /////
	m_PrintButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PrintButton.DrawFlatFocus(TRUE);

	///// Exit Button /////
	m_ExitButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ExitButton.DrawFlatFocus(TRUE);

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
	/// Point Teaching Button ///
	m_PointTeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PointTeachingButton.DrawFlatFocus(TRUE);

	/// Next Point Button ///
	m_NextPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NextPointButton.DrawFlatFocus(TRUE);
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
	///// Lens Combo /////
//	RecipeFile_LoadRecipe(&l_rcp_data.MainRcpInfo, l_rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);

// 2009.12.17 bagus MS ï¿½Ç‰ï¿½ --}--
	if (HEAD_TYPE_MS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		ChangeOperationEnable(TRUE);
	}
// 2009.12.17 bagus MS ï¿½Ç‰ï¿½ --}--

	if (l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		Init_SR_XYStageMenu();
		InitCombo_Lens();

		CRect rect;
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->GetWindowRect(rect);
		ScreenToClient(rect);
		GetDlgItem(IDC_EXIT_BUTTON)->MoveWindow(rect);

// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --{--
//		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->ShowWindow(SW_HIDE);
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_VALUES_TO_DISPLAY_BUTTON)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_PRINT_BUTTON)->ShowWindow(SW_HIDE);
//		GetDlgItem(IDC_SAVE_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NEXT_POINT_BUTTON)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_UP_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_LEFT_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_RIGHT_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_DOWN_BUTTON)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_MEAS_POINT_X_STATIC)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_MEAS_POINT_Y_STATIC)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_MEAS_POINT_X)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_MEAS_POINT_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_SHOW);
// 2009.12.09 bagus MS --{--
//		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		if (m_SystemConfig.nJoystickType == 0) {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		} else {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		}
// 2009.12.09 bagus MS --}--
		GetDlgItem(IDC_POINT_TEACHING_BUTTON)->ShowWindow(SW_SHOW);

		//ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½É‚ÍƒWï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½[ï¿½hï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
	} else {
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --{--
		GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->ShowWindow(SW_SHOW);
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--
		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_VALUES_TO_DISPLAY_BUTTON)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_PRINT_BUTTON)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_SAVE_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_NEXT_POINT_BUTTON)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_UP_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_LEFT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_RIGHT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_DOWN_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_X_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_Y_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_TEACHING_BUTTON)->ShowWindow(SW_HIDE);
	}

	//2009.09.13 bagus stress --{--
	//ï¿½Ä‘ï¿½ï¿½ï¿½Aï¿½Ê’uï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Í”ï¿½\ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
	}
	//2009.09.13 bagus stress --}--
	//2009.10.28 bagus 2point-distance --{--
	//ï¿½Ê’uï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Í”ï¿½\ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
	else if(HEAD_TYPE_SR == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType
	&& MEAS_PROG_TYPE_SR_DISTANCE == l_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType
	){
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
	}
	//2009.10.28 bagus 2point-distance --}--
	//2009.12.07 bagus SR GTR ï¿½Cï¿½ï¿½ --{--
	//ï¿½Ê’uï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Í”ï¿½\ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½
	else if(HEAD_TYPE_SR == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType
	&& MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType
	){
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
	}
	//2009.12.07 bagus SR GTR ï¿½Cï¿½ï¿½ --}--


	// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Lï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½Ö�~ï¿½ï¿½ï¿½ï¿½
		m_XY_UpButton.SetEnabled(FALSE);
		m_XY_DownButton.SetEnabled(FALSE);
		m_XY_RightButton.SetEnabled(FALSE);
		m_XY_LeftButton.SetEnabled(FALSE);
		// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
		bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
	}
	else{
		// H/Wï¿½ï¿½ï¿½ï¿½ï¿½È‚ç‹–ï¿½Â‚ï¿½ï¿½ï¿½
		m_XY_UpButton.SetEnabled(TRUE);
		m_XY_DownButton.SetEnabled(TRUE);
		m_XY_RightButton.SetEnabled(TRUE);
		m_XY_LeftButton.SetEnabled(TRUE);
		// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ö�~
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

	SetTimer(1, 50, NULL);
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	SetTimer(JOG_WATCH_TIMER_ID, JOG_WATCH_TIMER_INTERVAL, NULL);
	SetTimer(JOG_MODE_LAMP_TIMER_ID, JOG_MODE_LAMP_TIMER_INTERVAL, NULL);
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
}

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
void CMeasurementProcessView::OnDestroy()
{
BOOL bJoyStk;
BOOL result;

#if 0
	if( m_pMainFrame->GetJoyStickMode()!=0 ){
		// ï¿½nï¿½[ï¿½hï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½N
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
#else
	// ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Í�Aï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½Éƒ\ï¿½tï¿½gJoyï¿½Ö�Ø‚ï¿½Ö‚ï¿½ï¿½ï¿½
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
	bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
#endif

	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageStop();
// DEL 2009.06.01
//			if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
		}
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageElevatorStop();
// DEL 2009.06.01
//			if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
		}
	}

	if( m_joyStick1 != NULL ){
		delete m_joyStick1;
		m_joyStick1 = NULL;
	}
	if( m_joyStick2 != NULL ){
		delete m_joyStick2;
		m_joyStick2 = NULL;
	}

	//2009.12.25 bagus MS --{--
	nexioMS_ModeLamp(FALSE);
	//2009.12.25 bagus MS --}--

	//ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½İ’ï¿½
	m_pDoc->SetManuMeasMode(MANUMODE_NOT); //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½è’†ï¿½Ü‚ï¿½)(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½)

//	ButtonEnableChange(MANU_MEAS_FORM_DELETE);

	CNanoUI::OnDestroy();
}
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

// =========================================================================
//
void CMeasurementProcessView::OnMeasurementCancelButton()
{
		AfxGetMainWnd()->PostMessage(WM_DO_CANCEL, 0, 0);

// 2009.12.17 bagus MS ï¿½Ç‰ï¿½ --}--
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);

	if (HEAD_TYPE_MS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		ChangeOperationEnable(FALSE);
	}
// 2009.12.17 bagus MS ï¿½Ç‰ï¿½ --}--
}

// =========================================================================
//
void CMeasurementProcessView::OnMeasurementPauseButton()
{
	CString strBuff, strMsg, l_strBuffer;
	GetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, strBuff);
	// Kojika 20090601 Change
//	if ( strBuff == "PAUSE" )
	//if ( strBuff == "ï¿½ï¿½ï¿½è’†ï¿½f" )
	LoadStringML(IDS_MEASUREMENT_PAUSE, l_strBuffer, "PAUSE");
	if ( strBuff == l_strBuffer )
	{
	// Kojika 20090601 Change End

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
	}
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

		AfxGetMainWnd()->PostMessage(WM_DO_PAUSE, 0, 0);
		// Kojika 20090601 Change
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "RESUME");
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "ï¿½ï¿½ï¿½ï¿½ÄŠJ");
		LoadStringML(IDS_MEASUREMENT_RESUME, l_strBuffer, "RESUME");
		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, l_strBuffer);
		// Kojika 20090601 Change End

		m_pDoc->SetPauseFlg(TRUE); //ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½|ï¿½[ï¿½Yï¿½ï¿½Ô‚É�İ’ï¿½
	}
	else
	{
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Eï¿½Xï¿½Cï¿½bï¿½`ï¿½Eï¿½Iï¿½ï¿½
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰º */
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
/* modified 2009.08.21 hmenjo Pin Aling ï¿½Ê’uï¿½Lï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
//	  if(m_pDoc->CheckUnitStatus() == TRUE){return;}
/* modified 2009.08.21 hmenjo Pin Aling ï¿½Ê’uï¿½Lï¿½ï¿½ï¿½ï¿½ ----------			   */
	BOOL l_bEnablePinAlign = FALSE;
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		l_bEnablePinAlign = TRUE;
	}
	if (m_pDoc->CheckUnitStatus(l_bEnablePinAlign) == TRUE) {return;}
/* modified 2009.08.21 hmenjo Pin Aling ï¿½Ê’uï¿½Lï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


		AfxGetMainWnd()->PostMessage(WM_DO_RESUME, 0, 0);
		// Kojika 20090601 Change
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "PAUSE");
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "ï¿½ï¿½ï¿½è’†ï¿½f");
		LoadStringML(IDS_MEASUREMENT_PAUSE, l_strBuffer, "PAUSE");
		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, l_strBuffer);
		// Kojika 20090601 Change End
		m_pDoc->SetPauseFlg(FALSE); //
	}
}

// =========================================================================
//
void CMeasurementProcessView::OnExitButton()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
	}
	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½



//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½MeasEnd()ï¿½ÖˆÚ“ï¿½

	if ( ((CMainFrame*)AfxGetMainWnd())->MeasEnd() == IDCANCEL )
		return;

	//2009.11.11 bagus MS --{--
	//ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ÉŒï¿½ï¿½É–ß‚ï¿½
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		nexioMS_ModeLamp(FALSE);
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
		MSG msg;
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½
		while (1){
			if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				break;
			}else{
				Sleep(10);
			}
		}
		nexifRC_MoveToTeachPos(this->m_hWnd,MICROSCOPE_UPPER_POSITION_NO);
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½
		while (1){
			if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
				break;
			}else{
				Sleep(10);
			}
		}
		nexioMS_ModeLamp(FALSE);
	}
	//2009.11.11 bagus MS --}--

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 090326 hibino add //
	((CMainFrame*)AfxGetMainWnd())->ChangeOperationPanel(OP_MEASUREMENT);
	((CMainFrame*)AfxGetMainWnd())->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 090326 hibino add

}

void CMeasurementProcessView::OnRemeasurementButton()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Eï¿½Xï¿½Cï¿½bï¿½`ï¿½Eï¿½Iï¿½ï¿½
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰º */
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090531 Change <-----

	//2009.09.10 bagus stress --{--
	//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ì�ê�‡ï¿½É‚Í�Ä‘ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½í‚¹ï¿½È‚ï¿½
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {

		LoadStringML(IDS_REMEAS_STRESS_ERR, strMsg, "Re-Measurement is not supportted for STRESS Recipe.");
		MessageBox(strMsg, "", MB_OK | MB_ICONWARNING); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½ï¿½
		return ;
	}
	//2009.09.10 bagus stress --}--

	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½



	BOOL bRtnChk = StartRemeasurement(1);	//PMAï¿½ï¿½ï¿½ï¿½

	if(bRtnChk == TRUE)
	{
		m_pDoc->OperationLogging("Measurement Operation - REMEASUREMENT BUTTON - Measurement was initiated.");
	}

}


void CMeasurementProcessView::OnAdujstRemeasurementButton()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Eï¿½Xï¿½Cï¿½bï¿½`ï¿½Eï¿½Iï¿½ï¿½
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰º */
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090531 Change <-----
	//2009.09.10 bagus stress --{--
	//ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½Ì�ê�‡ï¿½É‚Í�Ä‘ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½í‚¹ï¿½È‚ï¿½
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		LoadStringML(IDS_REMEAS_STRESS_ERR, strMsg, "Re-Measurement is not supportted for STRESS Recipe.");
		MessageBox(strMsg, "", MB_OK | MB_ICONWARNING); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½ï¿½
		return ;
	}
	//2009.09.10 bagus stress --}--
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	BOOL bRtnChk = StartRemeasurement(2);	//PMAï¿½Lï¿½ï¿½(ï¿½Sï¿½_)

	if(bRtnChk == TRUE)
	{
		m_pDoc->OperationLogging("Measurement Operation - ADUJST REMEASUREMENT BUTTON - Measurement was initiated.");
	}

}


void CMeasurementProcessView::OnSaveButton()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

// Kojika 20090526 Add
	CString strBuffer;
// Kojika 20090526 Add End

//ï¿½ï¿½ï¿½ï¿½ï¿½ÌƒZï¿½[ï¿½uï¿½{ï¿½^ï¿½ï¿½ï¿½ÍƒVï¿½ï¿½ï¿½[ï¿½vï¿½dï¿½lï¿½Å‚Í–ï¿½ï¿½gï¿½p

	BOOL bCheck;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	bCheck = m_pMainFrame->m_pMeaTabViewObj->SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_EXIT_SAVE/*ï¿½á‚¤*/);
	if(bCheck == TRUE)
	{
// Kojika 20090526 Change
//		MessageBox("Save Completed.", "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½ï¿½
//		MessageBox("ï¿½Û‘ï¿½ï¿½ï¿½ï¿½ï¿½", "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½ï¿½
		LoadStringML(IDS_SAVE_COMPLETED, strBuffer, "Save Completed.");
		MessageBox(strBuffer, "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½ï¿½
// Kojika 20090526 Change End
	}
	else
	{
// Kojika 20090526 Change
//		MessageBox("Save failure.", "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
//		MessageBox("ï¿½Û‘ï¿½ï¿½ï¿½ï¿½s", "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
		LoadStringML(IDS_SAVE_FAILURE, strBuffer, "Save failure.");
		MessageBox(strBuffer, "", MB_OK); //ï¿½Zï¿½[ï¿½uï¿½ï¿½ï¿½s
// Kojika 20090526 Change End
	}

}


//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌŠÖ�ï¿½ï¿½Rï¿½[ï¿½ï¿½
LRESULT CMeasurementProcessView::OnMeaProcFuncCall(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
		case WM_DISPMENU_MEASUREMENT_CANCEL:
			OnMeasurementCancelButton();
			break;
		case WM_DISPMENU_MEASUREMENT_PAUSE:
		case WM_DISPMENU_MEASUREMENT_RESUME:
			OnMeasurementPauseButton();
			break;
		case WM_DISPMENU_REMEASUREMENT:
			OnRemeasurementButton();
			break;
		case WM_DISPMENU_ADUJST_REMEASUREMENT:
			OnAdujstRemeasurementButton();
			break;
		case WM_DISPMENU_SEQMEAS_EXIT:
			OnExitButton();
			break;
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
		case WM_DISPMENU_POINT_TEACHING:
			OnPointTeachingButton();
			break;
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

	}
	return 0L;
}


HBRUSH CMeasurementProcessView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ï¿½ DC ï¿½ÌƒAï¿½gï¿½ï¿½ï¿½rï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½Ï�Xï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if( pWnd == this ) // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½`ï¿½æ‚·ï¿½ï¿½Æ‚ï¿½ï¿½ï¿½
	{
		if((sbNexMntSw == OFF)||(sbNexEngiMntSw == ON))
		{
			hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
		}
	}

	// TODO: ï¿½fï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ìƒuï¿½ï¿½ï¿½Vï¿½ï¿½ï¿½]ï¿½İ‚Ì‚ï¿½ï¿½Ì‚Å‚È‚ï¿½ï¿½ê�‡ï¿½É‚Í�Aï¿½á‚¤ï¿½uï¿½ï¿½ï¿½Vï¿½ï¿½Ô‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	return hbr;
}

void CMeasurementProcessView::OnTimer(UINT nIDEvent)
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½Éƒï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½é‚©ï¿½Ü‚ï¿½ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	static BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	static BOOL sbNexMntSwOld = TRUE;
	static BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();
	static BOOL sbNexEngiMntSwOld = FALSE;

	sbNexMntSw = nexioIsMaintenanceSwitch();
	sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if((sbNexMntSw != sbNexMntSwOld)||(sbNexEngiMntSw != sbNexEngiMntSwOld))
	{
		Invalidate(TRUE); //OnCtlColor()ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½×‚ï¿½Call
	}
	sbNexMntSwOld = sbNexMntSw; //ï¿½Oï¿½ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	sbNexEngiMntSwOld = sbNexEngiMntSw; //ï¿½Oï¿½ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½Æ‚ï¿½ï¿½É‚Í‚È‚É‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		switch ( nIDEvent ) {
		//2009.11.16 bagus MS --{--
		//case JOG_WATCH_TIMER_INTERVAL:		//Up/Down Switch ï¿½ï¿½ï¿½ï¿½
		case JOG_WATCH_TIMER_ID:
		//2009.11.16 bagus MS --}--
//2009.12.15 bagus MS --{--
//			if (m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
			if (m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) || m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)
			//2009.12.25 bagus ï¿½ï¿½ï¿½è’†ï¿½Å‚È‚ï¿½ï¿½ï¿½Î�~ï¿½ß‚ï¿½ --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			) {
//2009.12.15 bagus MS --}--
				//ï¿½ï¿½ï¿½ì’†ï¿½È‚Ì‚Å“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
				//2009.12.15 bagus MS --{--
				if ( nexioRC_GetJOGP() || nexioRC_GetJOGM() ) {
					if(nexifRC_JogStop(this->m_hWnd)){
						m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
					}
				}
				//2009.12.15 bagus MS --}--
			}else{
				switch(nexioRC_GetCurrentMode()){
				case RCOPMODE_JOG:
				case RCOPMODE_INCHING:
					if(nexioIsMS_DownSwitch() && nexioIsMS_UpSwitch()){
						//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
					}else if(nexioIsMS_UpSwitch() && nexioIsMS_UpperPos()){
						if(nexifRC_JogMinus(this->m_hWnd)){
							m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						}
					}else if(nexioIsMS_DownSwitch() ){
						if(!nexioIsMS_LowerPos1() && !nexioIsMS_LensKind()){
							//ï¿½ï¿½ï¿½~ï¿½[ï¿½Pï¿½Åƒï¿½ï¿½ï¿½ï¿½Yï¿½ï¿½Ê‚ï¿½1-50x
						}else if(!nexioIsMS_LowerPos2()){
							//ï¿½ï¿½Ô‰ï¿½
						}else{
							//ï¿½ã�¸
							if(nexifRC_JogPlus(this->m_hWnd)){
								m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
							}
						}
					}else{
						//ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½Ì‚ÅƒWï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½~ï¿½ß‚ï¿½
						if ( nexioRC_GetJOGP() || nexioRC_GetJOGM() ) {
							if(nexifRC_JogStop(this->m_hWnd)){
								m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
							}
						}
					}
					break;
				}

				if(nexioIsMS_ModeSwitch()){
					switch(nexioRC_GetCurrentMode()){
					case RCOPMODE_JOG:
						m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						nexifRC_SelectMode(this->m_hWnd,RCOPMODE_INCHING);
						break;
					case RCOPMODE_INCHING:
						m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						nexifRC_SelectMode(this->m_hWnd,RCOPMODE_JOG);
						break;
					default:
						m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
						nexifRC_SelectMode(this->m_hWnd,RCOPMODE_JOG);
						break;
					}
				}else{
				}
			}

			break;
		case JOG_MODE_LAMP_TIMER_ID:	//MODE CHANGEï¿½ï¿½ï¿½ï¿½ï¿½vï¿½Ì“_ï¿½ï¿½/ï¿½_ï¿½ï¿½/ï¿½ï¿½ï¿½ï¿½
			if(m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) || m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)
			//2009.12.25 bagus ï¿½ï¿½ï¿½è’†ï¿½Å‚È‚ï¿½ï¿½ï¿½Î�ï¿½ï¿½ï¿½ --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			){
				//ï¿½ï¿½ï¿½ì’†ï¿½Í�ï¿½ï¿½ï¿½
				nexioMS_ModeLamp(FALSE);
			}else{
				switch(nexioRC_GetCurrentMode()){
				case RCOPMODE_JOG:
					nexioMS_ModeLamp(TRUE);
					break;
				case RCOPMODE_INCHING:
					if(m_bMSModeLampFilcker)
						nexioMS_ModeLamp(FALSE);
					else
						nexioMS_ModeLamp(TRUE);
					m_bMSModeLampFilcker = !m_bMSModeLampFilcker;
					break;
				default:
					nexioMS_ModeLamp(FALSE);
					break;
				}
			}
			break;
		default:
			break;
		}
	}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--

	CNanoUI::OnTimer(nIDEvent);
}


BOOL CMeasurementProcessView::StartRemeasurement(int iPma)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	//ï¿½ï¿½ï¿½İ‘Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½éƒ�ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½h
	CView* pView;

	pView = (CView*)((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter2.GetPane(0,0);
	int iIndex = m_pMainFrame->m_pMeaTabViewObj->m_MeasurementTab.GetCurSel();
	CGridCtrl* pGrid;
	if(iIndex == 0) //Listï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½
	{
		pGrid = &(((CMeasurementTabView *)pView)->m_pMeasurementListDlg->m_MeasurementListGrid);
	}
	else if(iIndex == 1) //Graphï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½
	{
		pGrid = &(((CMeasurementTabView *)pView)->m_pMeasurementGraphDlg->m_MeasurementListGrid);
	}
	else
	{
		return FALSE;
	}

// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

	int ChiefRet = 0;
	BOOL bResult = FALSE;
//	char* pszCaption = "REMEASUREMENT START";
//	  char* pszCaption = "ï¿½Ä‘ï¿½ï¿½ï¿½Jï¿½n";
	RCP_DATA rcp_data;
	int iLastMeasPoint=0;
	int iReMeasNumScans = 0; //ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½(ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½è‚·ï¿½é‚©ï¿½Ì�ï¿½)

	m_pDoc->GetRcpData(&rcp_data);

	///// ï¿½`ï¿½[ï¿½tï¿½Ö‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ğ‘—�M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_ReMeasurement);
		return FALSE;
	}

	memset(m_pDoc->m_bDoPointMeasFlag, FALSE, sizeof(m_pDoc->m_bDoPointMeasFlag));
	//2009.11.11 bagus 2point-distance --{--
	//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½Í�Aï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½á‚¤
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		for(int i=0; i</*rcp_data.StageProgInfoHdr.wNumScans*/m_pDoc->GetStoreCount(); i++)
		{
			int iFocus = (pGrid->GetItemState(i+1, 1) & /*GVIS_FOCUSED*//*GVIS_DROPHILITED*/GVIS_SELECTED);
			if(iFocus != 0)
			{
				m_pDoc->m_bDoPointMeasFlag[i*2] = TRUE; 	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½tï¿½ï¿½ï¿½Oï¿½ğ—§‚Ä‚ï¿½
				m_pDoc->m_bDoPointMeasFlag[i*2+1] = TRUE; 	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½tï¿½ï¿½ï¿½Oï¿½ğ—§‚Ä‚ï¿½
				iLastMeasPoint = i*2 + 1; //ï¿½Å�Iï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Xï¿½V
				iReMeasNumScans++;
				iReMeasNumScans++;
			}
		}
	}else{
	//2009.11.11 bagus 2point-distance --}--
		for(int i=0; i</*rcp_data.StageProgInfoHdr.wNumScans*/m_pDoc->GetStoreCount(); i++)
		{
			int iFocus = (pGrid->GetItemState(i+1, 1) & /*GVIS_FOCUSED*//*GVIS_DROPHILITED*/GVIS_SELECTED);
			if(iFocus != 0)
			{
				m_pDoc->m_bDoPointMeasFlag[i] = TRUE; //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½tï¿½ï¿½ï¿½Oï¿½ğ—§‚Ä‚ï¿½
				iLastMeasPoint = i + 1; //ï¿½Å�Iï¿½ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Xï¿½V
				iReMeasNumScans++;
			}
		}
	//2009.11.11 bagus 2point-distance --{--
	}
	//2009.11.11 bagus 2point-distance --}--
	//Saiki 20090406 Add ----->
	if(iReMeasNumScans < 1){
// Kojika 20090526 Change
//		MessageBox("ï¿½Ä‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½", pszCaption, MB_OK);
		LoadStringML(IDS_REMEASUREING_POINT_NOT_SELECT, strBuffer1, "The remeasuring point has not been selected");
		LoadStringML(IDS_TITLE_REMESURE_START, strBuffer2, "REMEASUREMENT START");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change End
		return FALSE;
	}
	//Saiki 20090406 Add <-----
	m_pDoc->SetLastMeasPoint(iLastMeasPoint); //ï¿½Å�Iï¿½ï¿½ï¿½ï¿½_ï¿½iï¿½[
	TRACE("%s%d", "ï¿½sCMeasurementProcessView::OnRemeasurementButtonï¿½t iLastMeasPoint:", iLastMeasPoint);

	m_pDoc->SetReMeasNumScans(iReMeasNumScans);
	m_pDoc->SetShowCompPointList(0);
	m_pDoc->SetShowCompPointGraph(0);
	m_pDoc->SetCurrentSetPoint(0);

	m_pMainFrame->m_pMeaTabViewObj->m_pMeasurementListDlg->SetTimer(SHOW_LIST_TIMER, 500, NULL);
	m_pMainFrame->m_pMeaTabViewObj->m_pMeasurementGraphDlg->SetTimer(SHOW_GRAPH_TIMER, 500, NULL);

	LPARAM lparam = (LPARAM)MAKELONG(1, iPma);
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SEQ, (WPARAM)rcp_data.szRecipeName, /*1*/lparam); //ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½[ï¿½h


	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	//Saiki 20090527 Change ---->
	//bResult = pMainFrame->JudgeChiefResult(ChiefRet, pszCaption);
	LoadStringML(IDS_TITLE_REMESURE_START, strBuffer2, "REMEASUREMENT START");
	bResult = pMainFrame->JudgeChiefResult(ChiefRet, strBuffer2);
	//Saiki 20090527 Change <----

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - REMEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		return FALSE;
	}

	//ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½tï¿½ï¿½ï¿½Oï¿½İ’ï¿½
	m_pDoc->SetSeqMeasMode(SEQ_ON_REMEASURE); //ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½É�İ’ï¿½


	//ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½tï¿½ï¿½ï¿½Oï¿½İ’ï¿½
	m_pDoc->SetMeasMode(MEASMODE_SEQ_OR_REMEASURE); //ï¿½Ê�í‘ªï¿½è’†orï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½


	return TRUE;
}

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
// =========================================================================
//ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Cï¿½jï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//
void CMeasurementProcessView::InitCombo_Lens()
{
	///// Load File /////
	MS_CONFIG MsConfig;
	ConfigFile_GetNanoSpecIni(&MsConfig, CONFIG_FILE_MS_CONFIG);

	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++ ) {
		if (MsConfig.RoboCylinder[i].bEnable ) {
			nIndex = pCombo->AddString(MsConfig.RoboCylinder[i].szName);
			pCombo->SetItemData(nIndex, i);
// matsuhisa 2009.12.26 added >>>
			if (MsConfig.RoboCylinder[i].bSensor) {
				m_nLensSensorItem = i;
			}
// matsuhisa 2009.12.26 added <<<
		}
	}
	pCombo->SetCurSel(0);
}

// ==========================================================================
//
void CMeasurementProcessView::Init_SR_XYStageMenu()
{
	/// XY Up Button ///
	m_XY_UpButton.SetIcon(IDI_ARROW_ROUND_UP, (int) BTNST_AUTO_GRAY);
	m_XY_UpButton.DrawBorder(FALSE);
	m_XY_UpButton.SetParams(VK_UP, WM_DO_STAGE_MOVE);

	/// XY Down Button ///
	m_XY_DownButton.SetIcon(IDI_ARROW_ROUND_DOWN, (int) BTNST_AUTO_GRAY);
	m_XY_DownButton.DrawBorder(FALSE);
	m_XY_DownButton.SetParams(VK_DOWN, WM_DO_STAGE_MOVE);

	/// XY Right Button ///
	m_XY_RightButton.SetIcon(IDI_ARROW_ROUND_RIGHT, (int) BTNST_AUTO_GRAY);
	m_XY_RightButton.DrawBorder(FALSE);
	m_XY_RightButton.SetParams(VK_RIGHT, WM_DO_STAGE_MOVE);

	/// XY Left Button ///
	m_XY_LeftButton.SetIcon(IDI_ARROW_ROUND_LEFT, (int) BTNST_AUTO_GRAY);
	m_XY_LeftButton.DrawBorder(FALSE);
	m_XY_LeftButton.SetParams(VK_LEFT, WM_DO_STAGE_MOVE);

	//XY STAGE GROUP Show
	GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_SHOW);

	//JoyStick Show
	GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_SHOW);
// 2009.10.29 bagus Jog ï¿½Ç‰ï¿½ï¿½Cï¿½ï¿½ --{--
//	GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	if (m_SystemConfig.nJoystickType == 0) {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	}
// 2009.10.29 bagus Jog ï¿½Ç‰ï¿½ï¿½Cï¿½ï¿½ --}--

// 2009.10.26 K.Matsuo ï¿½ï¿½ï¿½\ï¿½[ï¿½Xï¿½É‚È‚ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ß�Adelete -->
//	//Lens Disable
//	GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);
// 2009.10.26 K.Matsuo ï¿½ï¿½ï¿½\ï¿½[ï¿½Xï¿½É‚È‚ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ß�Adelete <--

	//Measurement Point Group Hide
	GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_HIDE);

	//Point No. Static Hide
	GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_HIDE);

	//Gantry Number Of Measure Point Hide
	GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_HIDE);

	//XY Position Hide
	GetDlgItem(IDC_MEAS_POINT_X_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MEAS_POINT_Y_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MEAS_POINT_X)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MEAS_POINT_Y)->ShowWindow(SW_HIDE);
}

// =========================================================================
//
void CMeasurementProcessView::OnPointTeachingButton()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg, strMsg;
	// Kojika 20090529 Add End

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;


	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Eï¿½Xï¿½Cï¿½bï¿½`ï¿½Eï¿½Iï¿½ï¿½
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- { -------- */
	if( m_pDoc->Rap_IsAirPressureLowON(1)!= OFF) {AlarmIf_Set(ALID_AirPressureDown); return;} /* ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰º */
/* added hmenjo 2009.05.20 ï¿½Gï¿½Aï¿½ï¿½ï¿½Í’á‰ºï¿½ï¿½ï¿½oï¿½Ìƒï¿½ï¿½bï¿½pï¿½Ö�ï¿½ -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	if(rcp_data.StageProgInfoHdr.wNumScans == m_pDoc->GetStoreCount()) //ï¿½ï¿½ï¿½è�”ï¿½ï¿½ï¿½\ï¿½è‘ªï¿½ï¿½É“ï¿½ï¿½Bï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
		return;

	m_pMainFrame->PostMessage(WM_CHIF_MS_POPUP,0,0);
#if 0
	CPointSetupTeachingMSDlg dlg;

	dlg.m_joyContinue = TRUE;					// JoyStickï¿½Ì�ï¿½Ô‚ï¿½ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	// Kojika 20090529 Change
	//dlg.SetCaption(MEASUREMENT_CAPTION);
	//dlg.SetMessage(MEASUREMENT_MESSAGE);
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_CAPTION, l_strCap, "MANUAL MEASUREMENT - POINT TEACHING");
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_MESSAGE, l_strMsg, "MANUAL MEASUREMENT - POINT TEACHING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
// 2009.10.30 bagus Stage-Sample ï¿½Cï¿½ï¿½ --{--
#if 0
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	BOOL bMain,bStage;

	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);

	if((bMain == FALSE) || (bStage == FALSE)){
		//ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½Ç‚ß‚È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ÅƒTï¿½ï¿½ï¿½vï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½Í‚È‚ï¿½
		dlg.m_nSampleMode = 0;
	}else{
		dlg.m_nSampleMode = 1;
	}

	strcpy(dlg.m_szRecipeName, rcp_data.StageProgInfoHdr.SampleInfo.szName);
#endif
// 2009.10.30 bagus Stage-Sample ï¿½Cï¿½ï¿½ --}--
	if(dlg.DoModal() == IDOK){


	}

#if 0
	BOOL bJoyStk;
	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
	}
#endif
#endif
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- { ---------- */
//void CManualMeasurementFormView::OnJoyStick()
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ----------			   */
LRESULT CMeasurementProcessView::OnJoyStick(WPARAM wparam, LPARAM lparam)
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- } ---------- */
{
	// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Lï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		m_joyStick1->UnChecked();
		m_joyStick2->Checked();
		// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
		m_XY_UpButton.SetEnabled(FALSE);
		m_XY_DownButton.SetEnabled(FALSE);
		m_XY_RightButton.SetEnabled(FALSE);
		m_XY_LeftButton.SetEnabled(FALSE);
	}
	else{
		m_joyStick1->Checked();
		m_joyStick2->UnChecked();
		// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ö�~
		m_XY_UpButton.SetEnabled(TRUE);
		m_XY_DownButton.SetEnabled(TRUE);
		m_XY_RightButton.SetEnabled(TRUE);
		m_XY_LeftButton.SetEnabled(TRUE);
	}
/* added 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- { ---------- */
	return 0;
/* added 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- } ---------- */
}

// ==========================================================================
// Nameï¿½F		OnNextPointButton
void CMeasurementProcessView::OnNextPointButton()
{
	RCP_DATA rcp;

	m_pDoc->GetRcpData(&rcp);
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Í‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒ{ï¿½^ï¿½ï¿½ï¿½Í—Lï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½Í‚ï¿½ï¿½B
		HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
		if(l_hChiefDlg != NULL){
			::PostMessage(l_hChiefDlg, WM_DISP_MS_POPUP_END, 1, 0);
		}
	}

}
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --{--
void CMeasurementProcessView::OnSelendokMicroScopeFocusPosition()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	int	nIndex = pCombo->GetCurSel();
	int	nItemData = pCombo->GetItemData(nIndex);

	int	nPosNo = m_MsConfig.RoboCylinder[nItemData].nPosition;

	CString strBuffer, strTitle, strMsg;
// matsuhisa 2009.12.26 added >>>
	if ( !nexioIsMS_LensKind() ) {
		if ( nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
			m_pDoc->MessageStringIf_Set(strMsg);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
	}
// matsuhisa 2009.12.26 added <<<

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	if ( CCursor::m_bWaitCursor ) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
	{
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
	}
	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
	{
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
//	if(m_pDoc->IsInterLock() == TRUE){return;}
//	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	//2009.12.25 bagus MS ï¿½Cï¿½ï¿½ --{--
	//if (m_pDoc->IsInterLock() == TRUE) {
	if (m_pDoc->IsInterLockForMScope() == TRUE) {
	//2009.12.25 bagus MS ï¿½Cï¿½ï¿½ --}--
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
	if (m_pDoc->CheckUnitStatus() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
#if 0
	// ActuateFlagï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½ï¿½
	m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

	// ï¿½ï¿½ï¿½ÌˆÚ“ï¿½ï¿½ï¿½ï¿½Í‚Ì�ï¿½tï¿½ï¿½ï¿½Ö�~ï¿½ï¿½ï¿½ï¿½
	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
	}

	// Zï¿½ï¿½ï¿½ï¿½ï¿½ì‹–ï¿½Âƒï¿½ï¿½ï¿½ï¿½v OFF
	nexioMS_ModeLamp(FALSE);

	// ï¿½Ê’uï¿½ï¿½ï¿½ï¿½
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

	// Zï¿½ï¿½ï¿½ï¿½ï¿½ì‹–ï¿½Âƒï¿½ï¿½ï¿½ï¿½v ON
	nexioMS_ModeLamp(TRUE);

	// ï¿½ï¿½ï¿½ÌˆÚ“ï¿½ï¿½ï¿½ï¿½Í‚Ì�ï¿½tï¿½ï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½ï¿½
	{	// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
	}

	// ActuateFlagï¿½ï¿½ß‚ï¿½
	m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
#else
	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
// matsuhisa 2009.12.26 deleted >>>
//		if (m_MsConfig.RoboCylinder[nItemData].bSensor == nexioIsMS_LensKind()) {
// matsuhisa 2009.12.26 deleted <<<
			// ï¿½ï¿½ï¿½ÌˆÚ“ï¿½ï¿½ï¿½ï¿½Í‚Ì�ï¿½tï¿½ï¿½ï¿½Ö�~ï¿½ï¿½ï¿½ï¿½
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
				MessageBox(strMsg, strTitle, MB_OK);
				// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
				m_pDoc->JoyStickStatusRestore();
				pCombo->SetCurSel(m_nSelLens);
				return;
			}

			// ActuateFlagï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½ï¿½
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

			// Modeï¿½Ø‚ï¿½Ö‚ï¿½
			unsigned char oldMode = nexioRC_GetCurrentMode();
			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
				MSG 	msg;

				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			// ï¿½Ê’uï¿½ï¿½ï¿½ï¿½
			if(nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo)){
				// ActuateFlagï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½ï¿½
				m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
				MSG 	msg;

				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, oldMode);
				MSG 	msg;

				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‘Ò‚ï¿½
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,FALSE);
// matsuhisa 2009.12.26 deleted >>>
//		} else {
//			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½ï¿½ï¿½Yï¿½Ì�İ’è‚ªï¿½ï¿½vï¿½ï¿½ï¿½È‚ï¿½
//			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
//			m_pDoc->MessageStringIf_Set(strMsg);
//			pCombo->SetCurSel(m_nSelLens);
//		}
// matsuhisa 2009.12.26 deleted <<<
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
}
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
LRESULT CMeasurementProcessView::OnNexResponse(WPARAM wparam, LPARAM lparam)
{
	switch ( wparam ) {
	case nexRC_MoveToTeachPos:
	case nexRC_JogPlus:
	case nexRC_JogMinus:
	case nexRC_JogStop:
	case nexRC_SelectMode:
//		if (lparam == 1) {
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
			//CCursor::EndWaitCursor();
//		} else {
			// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½
//		}
		break;
	default:
		break;
	}
	return 0;
}
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--

// 2009.12.17 bagus MS ï¿½Cï¿½ï¿½ --{--
void CMeasurementProcessView::ChangeOperationEnable(BOOL bEnable)
{
	GetDlgItem(IDC_NEXT_POINT_BUTTON)->EnableWindow(bEnable);
//2009.12.23 bagus MS ï¿½Cï¿½ï¿½ --{--
	//GetDlgItem(IDC_STAGE_UP_BUTTON)->EnableWindow(bEnable);
	//GetDlgItem(IDC_STAGE_LEFT_BUTTON)->EnableWindow(bEnable);
	//GetDlgItem(IDC_STAGE_RIGHT_BUTTON)->EnableWindow(bEnable);
	//GetDlgItem(IDC_STAGE_DOWN_BUTTON)->EnableWindow(bEnable);
	m_XY_UpButton.SetEnabled(bEnable);
	m_XY_DownButton.SetEnabled(bEnable);
	m_XY_RightButton.SetEnabled(bEnable);
	m_XY_LeftButton.SetEnabled(bEnable);
	GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(bEnable);
	GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(bEnable);
//2009.12.23 bagus MS ï¿½Cï¿½ï¿½ --}--
	GetDlgItem(IDC_COMBO_MEAS_POINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
	GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);

}
// 2009.12.17 bagus MS ï¿½Cï¿½ï¿½ --}--
