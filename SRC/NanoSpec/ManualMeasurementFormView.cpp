// ManualMeasurementFormView.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//
#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "OverlayDialog.h"
#include "AdditionalMeasurementDlg.h"
#include "ManualMeasurementFormView.h"
#include "NanoSpecDoc.h"
#include "ExecutionDlg.h"
#include "MeasurementTabView.h"
#include "MeasurementListDlg.h"
#include "MeasurementGraphDlg.h"
#include "NEXIOBASE.HXX"
#include "..\\..\\INC\\NSStage.hxx"
#include "SampleIdDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define																   //
// ==========================================================================
//#define MEASUREMENT_CAPTION	  "ï¿½è“®ï¿½ï¿½ï¿½ï¿½ - ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½eï¿½Bï¿½[ï¿½`ï¿½ï¿½ï¿½O"
//#define MEASUREMENT_MESSAGE	  "ï¿½è“®ï¿½ï¿½ï¿½ï¿½ - ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½eï¿½Bï¿½[ï¿½`ï¿½ï¿½ï¿½O"

#define MANUOPE_MAINTENANCE_WATCH_TIMER 1
#define MANUOPE_BUTTON_ENABLE_TIMER 	2

#define JOG_WATCH_TIMER_ID				(3)
#define JOG_WATCH_TIMER_INTERVAL		(10)
#define JOG_MODE_LAMP_TIMER_ID			(4)
#define JOG_MODE_LAMP_TIMER_INTERVAL	(1000)

/////////////////////////////////////////////////////////////////////////////
// CManualMeasurementFormView

IMPLEMENT_DYNCREATE(CManualMeasurementFormView, CNanoUI)

CManualMeasurementFormView::CManualMeasurementFormView()
	: CNanoUI(CManualMeasurementFormView::IDD)
{
	//{{AFX_DATA_INIT(CManualMeasurementFormView)
	//}}AFX_DATA_INIT

// 2009.05.28 K.Matsuo SPT -->
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.05.28 K.Matsuo SPT <--

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
}

CManualMeasurementFormView::~CManualMeasurementFormView()
{
}

void CManualMeasurementFormView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualMeasurementFormView)
	DDX_Control(pDX, IDC_MEASUREMENT_BUTTON, m_MeasurementButton);
	DDX_Control(pDX, IDC_MEASUREMENT_COMPLETE_BUTTON, m_MeasurementCompleteButton);
	DDX_Control(pDX, IDC_POINT_TEACHING_BUTTON, m_PointTeachingButton);
	DDX_Control(pDX, IDC_EXIT_BUTTON, m_ExitButton);
	DDX_Control(pDX, IDC_STAGE_UP_BUTTON, m_XY_UpButton);
	DDX_Control(pDX, IDC_STAGE_DOWN_BUTTON, m_XY_DownButton);
	DDX_Control(pDX, IDC_STAGE_RIGHT_BUTTON, m_XY_RightButton);
	DDX_Control(pDX, IDC_STAGE_LEFT_BUTTON, m_XY_LeftButton);
	DDX_Control(pDX, IDC_STAGE_MOTION_MODE, m_cbMotionStage);
	//}}AFX_DATA_MAP
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		DDX_Control(pDX, IDC_JOYSTICK1, *m_joyStick1);
		DDX_Control(pDX, IDC_JOYSTICK2, *m_joyStick2);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	DDX_Control(pDX, IDC_MEAS_POINT_X, m_stcMeasurementPointX);
	DDX_Control(pDX, IDC_MEAS_POINT_Y, m_stcMeasurementPointY);
}

BEGIN_MESSAGE_MAP(CManualMeasurementFormView, CNanoUI)
	//{{AFX_MSG_MAP(CManualMeasurementFormView)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MEASUREMENT_BUTTON, OnMeasurementButton)
	ON_BN_CLICKED(IDC_MEASUREMENT_COMPLETE_BUTTON, OnMeasurementCompleteButton)
	ON_BN_CLICKED(IDC_POINT_TEACHING_BUTTON, OnPointTeachingButton)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_MEAS_POINT, OnSelchangeComboMeasPoint)
	ON_CBN_SELENDOK(IDC_MICRO_SCOPE_FOCUS_POSITION, OnSelendokMicroScopeFocusPosition)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DO_STAGE_MOVE, OnDoStageMove)
	ON_MESSAGE(WM_STAGE_MOVE_DONE, OnStageMoveDone)
	ON_MESSAGE(WM_DISPMENU_MANUAL_MEASUREMENT_START, OnManuMeaFormFuncCall)
	ON_MESSAGE(WM_DISPMENU_MEASUREMENT_COMPLETE, OnManuMeaFormFuncCall)
	ON_MESSAGE(WM_DISPMENU_POINT_TEACHING, OnManuMeaFormFuncCall)
	ON_MESSAGE(WM_DISPMENU_MANUMEAS_EXIT, OnManuMeaFormFuncCall)
	ON_MESSAGE(WM_JOYSTICK, OnJoyStick)
	ON_MESSAGE(WM_TESTMODE_ONEPOINT_END, OnManuModeOnePointEnd)
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
END_MESSAGE_MAP()

// ==========================================================================
//
void CManualMeasurementFormView::OnInitialUpdate()
{
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// ï¿½ï¿½Ê‚ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é��ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Íƒ\ï¿½tï¿½gJoyï¿½Æ‚ï¿½ï¿½ï¿½
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
		// Radioï¿½{ï¿½^ï¿½ï¿½ï¿½Ì�İ’ï¿½
		m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
		m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --{--
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--

// 2013.01.11 bagus jog direction -->
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_JogDirSetting, CONFIG_FILE_JOG_DIR);
// 2013.01.11 bagus jog direction <--

	CNanoUI::OnInitialUpdate();

	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();


	m_pMainFrame->m_pManuMeaFormViewObj = this;
	m_pAddMeasDlgObj = NULL;

//Saiki 20090924 Add ----->
	///// Get Recipe Data /////
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
//Saiki 20090924 Add <-----
//	  GetReferenceManuStart();


	/// Measurement Button ///
	m_MeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementButton.DrawFlatFocus(TRUE);

	/// Measurement Complete Button ///
	m_MeasurementCompleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementCompleteButton.DrawFlatFocus(TRUE);

// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		m_PointTeachingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		/// Point Teaching Button ///
		m_PointTeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_PointTeachingButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	/// Exit Button ///
	m_ExitButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ExitButton.DrawFlatFocus(TRUE);

//Saiki 20090924 Change ----->
//	/// XY Up Button ///
//	m_XY_UpButton.SetIcon(IDI_ARROW_ROUND_UP, (int) BTNST_AUTO_GRAY);
//	m_XY_UpButton.DrawBorder(FALSE);
//	m_XY_UpButton.SetParams(VK_UP, WM_DO_STAGE_MOVE);
//
//	/// XY Down Button ///
//	m_XY_DownButton.SetIcon(IDI_ARROW_ROUND_DOWN, (int) BTNST_AUTO_GRAY);
//	m_XY_DownButton.DrawBorder(FALSE);
//	m_XY_DownButton.SetParams(VK_DOWN, WM_DO_STAGE_MOVE);
//
//	/// XY Right Button ///
//	m_XY_RightButton.SetIcon(IDI_ARROW_ROUND_RIGHT, (int) BTNST_AUTO_GRAY);
//	m_XY_RightButton.DrawBorder(FALSE);
//	m_XY_RightButton.SetParams(VK_RIGHT, WM_DO_STAGE_MOVE);
//
//	/// XY Left Button ///
//	m_XY_LeftButton.SetIcon(IDI_ARROW_ROUND_LEFT, (int) BTNST_AUTO_GRAY);
//	m_XY_LeftButton.DrawBorder(FALSE);
//	m_XY_LeftButton.SetParams(VK_LEFT, WM_DO_STAGE_MOVE);
	if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
		Init_SR_Transmittance_StageMenu();
	}
	else{
		Init_SR_XYStageMenu();
	}
//Saiki 20090924 Change <-----


// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
	///// Lens Combo /////
//	RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);

	if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		InitCombo_Lens();
		//2009.12.25 bagus MS --{--
		OnSelendokMicroScopeFocusPosition();
		//ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½É‚ÍƒWï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½[ï¿½hï¿½É‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
		//2009.12.25 bagus MS --}--
		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);

	} else {
		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_HIDE);
	}
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--
	//2009.10.30 bagus 2point-Distance --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_LEFT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_UP_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_RIGHT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_DOWN_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_X_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_Y_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		OnPointTeachingButton();
	}
	//2009.10.30 bagus 2point-Distance --}--


	// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Lï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
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
#if 1
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
#endif
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	m_bNexMntSwOld = TRUE;
	m_bNexEngiMntSwOld = FALSE;
	m_iProcessStatusOld = -1;

	ButtonEnableChange(MANU_MEAS_FORM_CREATE);

	SetTimer(MANUOPE_MAINTENANCE_WATCH_TIMER, 50, NULL);
	SetTimer(MANUOPE_BUTTON_ENABLE_TIMER, 50, NULL);
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	//2009.11.26 bagus MS ï¿½Cï¿½ï¿½ --{--
	//SetTimer(JOG_WATCH_TIMER_INTERVAL, JOG_WATCH_TIMER_ID, NULL);
	SetTimer(JOG_WATCH_TIMER_ID, JOG_WATCH_TIMER_INTERVAL, NULL);
	//2009.11.26 bagus MS ï¿½Cï¿½ï¿½ --}--
	SetTimer(JOG_MODE_LAMP_TIMER_ID, JOG_MODE_LAMP_TIMER_INTERVAL, NULL);
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
}

// ==========================================================================
//
void CManualMeasurementFormView::OnDestroy()
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

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		if( m_joyStick1 != NULL ){
			delete m_joyStick1;
			m_joyStick1 = NULL;
		}
		if( m_joyStick2 != NULL ){
			delete m_joyStick2;
			m_joyStick2 = NULL;
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	//2009.12.25 bagus MS --{--
	nexioMS_ModeLamp(FALSE);
	//2009.12.25 bagus MS --}--

	//ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½İ’ï¿½
	m_pDoc->SetManuMeasMode(MANUMODE_NOT); //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½è’†ï¿½Ü‚ï¿½)(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½)

	ButtonEnableChange(MANU_MEAS_FORM_DELETE);

	CNanoUI::OnDestroy();
}
//Saiki 20090924 Add ----->
// ==========================================================================
//
void CManualMeasurementFormView::Init_SR_XYStageMenu()
{
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		m_XY_UpButton.ShowWindow(SW_HIDE);
		m_XY_DownButton.ShowWindow(SW_HIDE);
		m_XY_RightButton.ShowWindow(SW_HIDE);
		m_XY_LeftButton.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
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
//		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		if (m_SystemConfig.nJoystickType == 0) {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		} else {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		}
// 2009.10.29 bagus Jog ï¿½Ç‰ï¿½ï¿½Cï¿½ï¿½ --}--
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

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

// ==========================================================================
//
void CManualMeasurementFormView::Init_SR_Transmittance_StageMenu()
{
	char szMeasPt[16];
	// XY STAGE BUTTON HIDE
	GetDlgItem(IDC_STAGE_UP_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STAGE_RIGHT_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STAGE_LEFT_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STAGE_DOWN_BUTTON)->ShowWindow(SW_HIDE);

	//XY STAGE GROUP HIDE
	GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);

	//JoyStick HIDE
	GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);

// 2009.10.26 K.Matsuo ï¿½ï¿½ï¿½\ï¿½[ï¿½Xï¿½É‚È‚ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ß�Adelete -->
//	//Lens Disable
//	GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);
// 2009.10.26 K.Matsuo ï¿½ï¿½ï¿½\ï¿½[ï¿½Xï¿½É‚È‚ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ß�Adelete <--

	//Measurement Point Group Show
	GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_SHOW);

	//Point No. Static Show
	GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_SHOW);

	//Gantry Number Of Measure Point Show & Initialize
	GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_SHOW);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	//2009.12.11 bagus ï¿½Cï¿½ï¿½ --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//pCombo->AddString("<Select>");
	pCombo->AddString(strSelectCaption);
	//2009.12.11 bagus ï¿½Cï¿½ï¿½ --{--
// 2010.01.29 bagus Gantry --{--
#if 0
	for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
		_itoa(iMeasPt, szMeasPt, 10);
		pCombo->AddString(szMeasPt);
	}
#else
	RCP_DATA rcp_data;
	char szRecipeName[RECIPE_NAME_LEN + 1];
	STAGE_COORD tScanPoint[SCAN_POINT_MAX];
	SR_REFERENCE_POSITION SrReferencePosition;
	STAGE_PROG_INFO_HDR tStageProgInfoHdr;
	WORD wNumScans;
	int j;

	m_pDoc->GetRcpData(&rcp_data);
	::ZeroMemory(tScanPoint, sizeof(tScanPoint));

	if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
		/* GTR ï¿½Ì�ê�‡*/
		strcpy(szRecipeName, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);

		ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

		if ( !RecipeFile_LoadRecipe(&tStageProgInfoHdr, szRecipeName, RECIPE_FILE_STAGE_PROGRAM) ) {
			wNumScans = 0;
		} else {
			wNumScans = tStageProgInfoHdr.wNumScans;
		}
		if ( !RecipeFile_LoadPointList(tScanPoint, wNumScans, szRecipeName) ) {
			::ZeroMemory(tScanPoint, sizeof(tScanPoint));
		}

		for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
			for (j=0; j<wNumScans; j++) {
				if ((SrReferencePosition.LocGantryTransmitReference[iMeasPt-1].lX == tScanPoint[j].lX)
				 && (SrReferencePosition.LocGantryTransmitReference[iMeasPt-1].lY == tScanPoint[j].lY)) {
					_itoa(iMeasPt, szMeasPt, 10);
					pCombo->AddString(szMeasPt);
					break;
				}
			}
		}
	} else {
		for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
			_itoa(iMeasPt, szMeasPt, 10);
			pCombo->AddString(szMeasPt);
		}
	}
#endif
// 2010.01.29 bagus Gantry --}--
	pCombo->SetCurSel(0);

	//XY Position Show
	GetDlgItem(IDC_MEAS_POINT_X_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MEAS_POINT_Y_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MEAS_POINT_X)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MEAS_POINT_Y)->ShowWindow(SW_SHOW);

	m_stcMeasurementPointX.SetBkColor(REFERENCE_COLOR);
	m_stcMeasurementPointY.SetBkColor(REFERENCE_COLOR);

	GetDlgItem(IDC_MEAS_POINT_X)->SetWindowText("-----");
	GetDlgItem(IDC_MEAS_POINT_Y)->SetWindowText("-----");
}
//Saiki 20090924 Add <-----
// =========================================================================
//
void CManualMeasurementFormView::OnMeasurementButton()
{
	// ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
	/* ï¿½Lï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N	*/
	BOOL l_bBatch = FALSE;
	if (
		(0 != (::GetAsyncKeyState(VK_SHIFT) & 0x8000)) &&		/* Shift ï¿½Lï¿½[	*/
		(0 != (::GetAsyncKeyState(VK_CONTROL) & 0x8000)) &&	/* Ctrl ï¿½Lï¿½[	*/
		(0 != (::GetAsyncKeyState(VK_MENU) & 0x8000)) &&		/* Alt ï¿½Lï¿½[	*/
		1
		) {
		/* ï¿½oï¿½bï¿½`ï¿½mï¿½ï¿½	*/
		l_bBatch = TRUE;
	}
/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */

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



	//ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½İ’ï¿½
	m_pDoc->SetManuMeasMode(MANUMODE_ON_MEASURE); //ï¿½ï¿½ï¿½è’†ï¿½É�İ’ï¿½


/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
	if (FALSE != l_bBatch) {
		/* ï¿½oï¿½bï¿½`ï¿½mï¿½ï¿½	*/
		MEAS_BatchFlagSet(1);
	}
/* added 2016.02.24 hmenjo ï¿½oï¿½bï¿½`ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoClr();
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ShowCompletionPointSnap(0, 0);
	m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSet(0, FALSE, FALSE);
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */

	ManuMeas();

/* deleted 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
/*		ï¿½Û—ï¿½	*/
///* added 2014.10.27 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- { ---------- */
//	// 2014.10.23 bagus. for lost focus protection. -->
//	SetFocus();
//	// 2014.10.23 bagus. for lost focus protection. <--
///* added 2014.10.27 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */
/* deleted 2014.10.28 hmenjo ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ Auto Scroll ---------- } ---------- */
}

// =========================================================================
//
void CManualMeasurementFormView::OnMeasurementCompleteButton()
{
	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

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



	SYSTEMTIME syEndDateTime;	  //ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	GetLocalTime(&syEndDateTime); //ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½V
	m_pDoc->SetEndDateTime(&syEndDateTime);


	//ï¿½ê–‡ï¿½Iï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Ö�ï¿½ï¿½ï¿½call(EXITï¿½{ï¿½^ï¿½ï¿½ï¿½Ì‚ï¿½ï¿½)
	if(m_pDoc->GetHostMode() == HOST_REMOTE)
	{
		//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½Íƒ}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½è‚ªï¿½È‚ï¿½ï¿½Ì‚Å�Aï¿½ï¿½ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Í‚ï¿½ï¿½ï¿½È‚ï¿½ï¿½Í‚ï¿½ï¿½B
		if ( m_pMainFrame->MeasEnd() == IDCANCEL )
			return;
	}


	//ï¿½`ï¿½[ï¿½tï¿½Öƒï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ğ“Š‚ï¿½ï¿½ï¿½
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		::PostMessage(l_hChiefDlg, WM_DATA_MEAS_END, 0, 0); //1ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	} else {
			AlarmIf_Set(ALID_OneSampledMeasurementFailed);
	}

	m_pDoc->SetProcessStatus(PROCESS_WAIT);

	//ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½tï¿½ï¿½ï¿½Oï¿½İ’ï¿½
	m_pDoc->SetSeqMeasMode(SEQ_NOT_MEASURE); //ï¿½ï¿½ï¿½è’†ï¿½Å‚È‚ï¿½ï¿½É�İ’ï¿½

	m_pDoc->SetReMeasurePointCnt(0);


	//ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½İ’ï¿½
	m_pDoc->SetManuMeasMode(MANUMODE_MEAS_COMP); //ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½É�İ’ï¿½

	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_COMP);

}

// =========================================================================
//
void CManualMeasurementFormView::OnPointTeachingButton()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg, strMsg;
	// Kojika 20090529 Add End

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	//2009.10.30 bagus 2point-distance --{--
	//2ï¿½_ï¿½Ô‹ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Íƒ|ï¿½Cï¿½ï¿½ï¿½gï¿½eï¿½Bï¿½[ï¿½`ï¿½ï¿½ï¿½Oï¿½Í‘ï¿½ï¿½è�ˆï¿½ï¿½
	{
		RCP_DATA rcp_data;
		m_pDoc->GetRcpData(&rcp_data);
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			OnMeasurementButton();
			return;
		}
	}
	//2009.10.30 bagus 2point-distance --}--
	//2009.11.12 bagus MS --{--
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Í�Aï¿½ï¿½ï¿½è�ˆï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½B
	{
		RCP_DATA rcp_data;
		m_pDoc->GetRcpData(&rcp_data);
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
			OnMeasurementButton();
			return;
		}
	}
	//2009.11.12 bagus MS --}--

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


	CAdditionalMeasurementDlg dlg;

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
// 2009.10.30 bagus Stage-Sample ï¿½Cï¿½ï¿½ --}--
	//2009.11.29 bagus GTR --{--
	if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
		dlg.m_bGTR = TRUE;
	}else{
		dlg.m_bGTR = FALSE;
	}
	//2009.11.29 bagus GTR --}--

// 2013.02.22 bagus Substrate thickness setting -->
	if ( dlg.m_nSampleMode != 0 ) {
		BOOL bSelected;
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
		bSelected = SelectSubThickNotify(rcp_data.StageProgInfoHdr.SampleInfo.dThickness);
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

		if ( !bSelected ) {
			AlarmIf_Set(ALID_SubstrateThicknessError);
		}
	}
// 2013.02.22 bagus Substrate thickness setting <--

	dlg.DoModal();

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
}

// =========================================================================
//
void CManualMeasurementFormView::OnExitButton()
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

/* added 2009.12.02 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½) ---------- { ---------- */
	BOOL l_bRc = SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);		/* SR ï¿½wï¿½bï¿½hï¿½É–ß‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
/* added 2009.12.02 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½) ---------- } ---------- */
/* added 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
	/* SR ï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½ï¿½ OPEN ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/* added 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */

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

	if ( m_pMainFrame->MeasEnd() == IDCANCEL )
		return;


	//2009.11.04 bagus CA --{--
	//ï¿½Rï¿½ï¿½ï¿½^ï¿½Nï¿½gï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½É‚Í�Aï¿½ã�ˆï¿½ï¿½ï¿½ï¿½ï¿½Kï¿½v
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
		m_pDoc->CleanupManualCAMeasure();
	}
	//2009.11.04 bagus CA --}--

	//2009.12.25 bagus MS --{--
	//ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ÉƒVï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½Sï¿½Ê’uï¿½É–ß‚ï¿½
	if (rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
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
	//2009.12.25 bagus MS --}--



	if(m_pDoc->GetProcessStatus() == PROCESS_PROC)
	{
		m_pDoc->SetProcessStatus(PROCESS_WAIT);
	}

//	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 090326 hibino add
	((CMainFrame*)AfxGetMainWnd())->ChangeOperationPanel(OP_MEASUREMENT);
	((CMainFrame*)AfxGetMainWnd())->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 090326 hibino add

}

// ==========================================================================
//
LRESULT CManualMeasurementFormView::OnDoStageMove(WPARAM wParam, LPARAM lParam)
{
STAGE_COORD Pos;
BOOL result;
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return 0L;

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return 0L;} // ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Ù�ï¿½
	}
	else{												// ï¿½Ê�í“®ï¿½ï¿½
	}
	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return 0L;} // MOVOï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ 				ï¿½ï¿½ï¿½k
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckUnitStatus() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/

	//
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090531 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return 0L;
		}	// ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†
		//Saiki 20090531 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	short sign = 1;
	WORD wAxis;

	switch(wParam){
	case VK_DOWN:
		sign *= -1;
	case VK_UP:
		wAxis = Y;
		break;
	case VK_LEFT:
		sign *= -1;
	case VK_RIGHT:
		wAxis = X;
		break;
	default:
		return 0L;
	}

// 2013.01.11 bagus jog direction -->
	if(m_JogDirSetting.bSwapXyDir){
		if(wAxis == X){
			wAxis = Y;
		}
		else{
			wAxis = X;
		}
	}

	if(m_JogDirSetting.bChangeXSign){
		if(wAxis == X){
			sign *= -1;
		}
	}

	if(m_JogDirSetting.bChangeYSign){
		if(wAxis == Y){
			sign *= -1;
		}
	}
// 2013.01.11 bagus jog direction <--

// 2009.05.28 K.Matsuo SPT -->
	UINT nMode = m_cbMotionStage.GetMode();

	CCursor::BeginWaitCursor();

	int MotionJog = GetMotionJog(wAxis, nMode);

	///// Load File /////
	STAGE_MOTION_SPEED_DATA l_StageMotionSpeedData[MAX_AXIS];
	ConfigFile_GetNanoSpecIni(&l_StageMotionSpeedData[0], CONFIG_FILE_STAGE_MOTION_SPEED);

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if(!m_pMainFrame->HardwareSimulation()){
		do{
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if(m_pDoc->IsInterLock() == TRUE)
				break;
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			switch ( m_SystemConfig.nStageType ) {
			case STAGE_TYPE_SPT:
				if(nMode < STAGE_MOTION_JOG_LOW){
					// ï¿½á‘¬ï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Eï¿½ï¿½ï¿½ï¿½
					if ( StageIsIdle() ) {
						StageMoveAtSpeedEx(wAxis, (double)(MotionJog * sign)/MICROMETRE, FALSE);
						m_bIsMoveAtSpeed = TRUE;
						if ( lParam == 1 ) {
							// ï¿½Lï¿½[ï¿½{ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½Í�AWM_STAGE_MOVE_DONEï¿½Ì�ï¿½Mï¿½Å’ï¿½~ï¿½ï¿½ï¿½ï¿½
							return 0L;
						}
					}
				}else{
					// ï¿½Wï¿½ï¿½ï¿½Oï¿½iï¿½ï¿½jï¿½Eï¿½Wï¿½ï¿½ï¿½Oï¿½iï¿½ï¿½ï¿½jï¿½Eï¿½Wï¿½ï¿½ï¿½Oï¿½iï¿½ï¿½ï¿½j
					Pos.lX = Pos.lY = 0;
					switch ( wAxis ) {
					case X:
						Pos.lX = MotionJog * sign;
						break;
					case Y:
						Pos.lY = MotionJog * sign;
						break;
					}
					result = StageMoveRelativeAtSpeed(&Pos, (double)l_StageMotionSpeedData[wAxis].Speed[nMode-3]/MICROMETRE);
// DEL 2009.06.01
//					if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
				}
				break;
			default:
// 2009.07.15 K.Matsuo -->
				switch ( wAxis ) {
				case X:
					Pos.lX = MotionJog * sign;
					break;
				case Y:
					Pos.lY = MotionJog * sign;
					break;
				}
				result = StageMoveRelative(&Pos);
// 2009.07.15 K.Matsuo <--
// DEL 2009.06.01
//				if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
				break;
			}
			Sleep(200);
		}while(::GetAsyncKeyState(VK_LBUTTON) < 0);

		if ( m_SystemConfig.nStageType == STAGE_TYPE_SPT ) {
			if ( m_bIsMoveAtSpeed ) {
				// ï¿½}ï¿½Eï¿½Xï¿½ï¿½ï¿½ï¿½Í�Aï¿½ï¿½ï¿½ï¿½ï¿½Å’ï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
				StageStop();
				m_bIsMoveAtSpeed = FALSE;
			}
		}
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	CCursor::EndWaitCursor();

// 2009.05.28 K.Matsuo SPT <--

	return 0L;
}

// ==========================================================================
//
LRESULT CManualMeasurementFormView::OnStageMoveDone(WPARAM wParam, LPARAM lParam)
{
// 2009.05.28 K.Matsuo SPT -->
//BOOL result;

	if ( m_SystemConfig.nStageType != STAGE_TYPE_SPT ) return 0L;
	if ( m_cbMotionStage.GetMode() >= STAGE_MOTION_JOG_LOW ) return 0L;
	if ( m_pMainFrame->HardwareSimulation() ) return 0L;
//	if ( m_pMainFrame->GetJoyStickMode() != 0 ) return 0L;

	if ( m_bIsMoveAtSpeed ) {
		StageStop();
		m_bIsMoveAtSpeed = FALSE;
//		 if( result ){
//			TRACE("COverlayDialog::OnStageMoveDone StageStop\n");
//		  }
//		  else{
//			  AlarmIf_Set(ALID_SubControllerReplayAbnormal);
//		}
	}
// 2009.05.28 K.Matsuo SPT <--

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	CCursor::EndWaitCursor();

	return 0L;
}

// ==========================================================================
//
BOOL CManualMeasurementFormView::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN ) {
		switch ( pMsg->wParam ) {
		case VK_DOWN:
		case VK_UP:
		case VK_LEFT:
		case VK_RIGHT:
			// H/Wï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
			if( m_pMainFrame->GetJoyStickMode() == 0 ){
				if ( !CCursor::m_bWaitCursor ) {
					CCursor::BeginWaitCursor();
//					PostMessage(WM_DO_STAGE_MOVE, pMsg->wParam, 0L);
					PostMessage(WM_DO_STAGE_MOVE, pMsg->wParam, 1L);
					TRACE("CManualMeasurementFormView::PreTranslateMessage WM_KEYDOWN\n");
				}
				return TRUE;
			}
			break;
		default:
			break;
		}
	}
	else if ( pMsg->message == WM_KEYUP ) {
		switch ( pMsg->wParam ) {
		case VK_DOWN:
		case VK_UP:
		case VK_LEFT:
		case VK_RIGHT:
			// H/Wï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
			if( m_pMainFrame->GetJoyStickMode() == 0 ){
				PostMessage(WM_STAGE_MOVE_DONE, pMsg->wParam, 0L);
				TRACE("CManualMeasurementFormView::PreTranslateMessage WM_KEYUP\n");
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	return CNanoUI::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
void CManualMeasurementFormView::ClearReference(char* szRecipeName)
{
	char szFilePath[MAX_PATH];
	memset(szFilePath, 0, sizeof(szFilePath));

	sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, szRecipeName, DAT_EXT);
	DeleteFile(szFilePath);
}

// ==========================================================================
//
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
//void CManualMeasurementFormView::ManuMeas(void)
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ----------				*/
BOOL CManualMeasurementFormView::ManuMeas(void)
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */
{
// Kojika 20090604 Add
	CString strBuffer, strTitle;
// Kojika 20090604 Add End

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
//	char* pszCaption = "MANUAL MEASUREMENT START";
	//char* pszCaption = "ï¿½è“®ï¿½ï¿½ï¿½ï¿½Jï¿½n";
	CString l_strBuffer;
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	if(m_pDoc->GetStoreCount() >= SCAN_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
// Kojika 20090604 Change
//		sprintf(szMsgBuff, "Measurement data %d over!", SCAN_POINT_MAX);
//		sprintf(szMsgBuff, "ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ %d ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ğ’´‚ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½!", SCAN_POINT_MAX);
//		MessageBox(szMsgBuff, "NANO SPEC", MB_OK);
		LoadStringML(IDS_MANUAL_MEASURE_OVER, strBuffer, "Measurement data %d over!");
		sprintf(szMsgBuff, strBuffer, SCAN_POINT_MAX);
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
		MessageBox(szMsgBuff, strTitle, MB_OK);
// Kojika 20090604 Change End
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */
	}


	///// ï¿½`ï¿½[ï¿½tï¿½Ö‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ğ‘—�M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_ManualMeasurement);
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */
	}

	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		//Saiki 20090411 Add ----->
		///// ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½Ø‘ï¿½ /////
		m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
		StageSelectLensNotify(rcp_data.MeasProgInfo.ScanParams._SR.iLens);
		m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);
		//Saiki 20090411 Add <-----

// 2013.02.22 bagus Substrate thickness setting -->
		BOOL bSelected;
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
		bSelected = SelectSubThickNotify(rcp_data.StageProgInfoHdr.SampleInfo.dThickness);
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

		if ( !bSelected ) {
			AlarmIf_Set(ALID_SubstrateThicknessError);
		}
// 2013.02.22 bagus Substrate thickness setting <--
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--

/* modified 2009.11.27 hmenjo ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Å‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT/*WM_DISP_START_SEQ*/, (WPARAM)rcp_data.szRecipeName, 0); //ï¿½Ê�í‘ªï¿½ï¿½
/* modified 2009.11.27 hmenjo ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Å‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ ----------			   */
	int l_iMeasPos;
	if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
		/* GTR ï¿½Ì�ê�‡*/
		TCHAR l_tszGTRpos[32];
		this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
		l_iMeasPos = _ttol(l_tszGTRpos);

		//2010.01.29 bagus GTR ï¿½Cï¿½ï¿½ --{--
		SR_REFERENCE_POSITION l_SrReferencePosition;
		SR_CONFIG l_SrConfig;
		memset(&l_SrReferencePosition, 0, sizeof(l_SrReferencePosition));
		memset(&l_SrConfig, 0, sizeof(l_SrConfig));
		ConfigFile_GetNanoSpecIni(&l_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);
		ConfigFile_GetNanoSpecIni(&l_SrConfig, CONFIG_FILE_SR_CONFIG);
		if(l_iMeasPos < 1 || l_iMeasPos > l_SrConfig.wGantryNumOfMeasPoint){
			l_iMeasPos = 0;
		}else{
			if(!m_pMainFrame->HardwareSimulation()){
				STAGE_COORD l_Pos;
				NS_StageGetPos(&l_Pos);
				if(l_SrReferencePosition.LocGantryTransmitReference[l_iMeasPos-1].lX != l_Pos.lX
					|| l_SrReferencePosition.LocGantryTransmitReference[l_iMeasPos-1].lY != l_Pos.lY){
					LoadStringML(IDS_MANUAL_MEASURE_SELECT_POINT, strBuffer, "Select measurement point.");
					LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
					MessageBox(strBuffer, strTitle, MB_OK);
					ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
					return FALSE;
				}
			}
		}
		//2010.01.29 bagus GTR ï¿½Cï¿½ï¿½ --}--

	} else {
/* modified 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
//		l_iMeasPos = m_pDoc->GetStoreCount() + 1;
/* modified 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ----------              */
		if (0 == m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
			/* ï¿½Ê�ï¿½(ï¿½Vï¿½K)ï¿½ï¿½ï¿½ï¿½	*/
			l_iMeasPos = m_pDoc->GetStoreCount() + 1;
		} else {
			/* ï¿½Ä‘ï¿½ï¿½ï¿½Ì�wï¿½ï¿½	*/
			l_iMeasPos = m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoGet();
		}
/* modified 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */
	}
	if (0 == l_iMeasPos) {
		ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
// 2010.01.29 bagus Gantry --{--
		LoadStringML(IDS_MANUAL_MEASURE_SELECT_POINT, strBuffer, "Select measurement point.");
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
		MessageBox(strBuffer, strTitle, MB_OK);
// 2010.01.29 bagus Gantry --}--
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
//		 return;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */
	}
/* added 2009.12.02 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½) ---------- } ---------- */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(l_iMeasPos, 0));
/* modified 2009.11.27 hmenjo ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Å‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ ---------- } ---------- */

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	// Kojika 20090529 Change
	LoadStringML(IDS_MANUAL_MEASUREMENT_START, l_strBuffer, "MANUAL MEASUREMENT START");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - MANUAL MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */
	}

/* added 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- { ---------- */
	return TRUE;
/* added 2009.12.09 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½) ---------- } ---------- */
}


// ==========================================================================
// ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½æ“¾ï¿½Aï¿½Jï¿½nï¿½ï¿½ï¿½Ô�æ“¾ï¿½È‚ï¿½
BOOL CManualMeasurementFormView::GetReferenceManuStart(void)
{

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
//	char* pszCaption = "MANUAL MEASUREMENT REFERENCE START";
//	char* pszCaption = "ï¿½è“®ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Jï¿½n";
	CString l_strBuffer;
	// Kojika 20090529 Change End

	SYSTEMTIME syStartDateTime; 	//ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½


	m_pDoc->GetRcpData(&rcp_data);


	///// ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½ï¿½æ“¾ /////
	GetLocalTime(&syStartDateTime);
	m_pDoc->SetStartDateTime(&syStartDateTime);

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		///// Clear Reference /////
		ClearReference(rcp_data.szRecipeName);
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		return FALSE;
	}

	// ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½Ï�X
	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	// Kojika 20090529 Change
	LoadStringML(IDS_MANUAL_MEASUREMENT_REF_START, l_strBuffer, "MANUAL MEASUREMENT REFERENCE START");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		// ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ß‚ï¿½
//		ButtonEnableChange(MANU_MEAS_FORM_CREATE);
		return FALSE;
	}


//	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		CExecutionDlg dlg;
		// Kojika 20090529 Change
		//Saiki 20090128 Change ----->
//		dlg.SetCaption(EXECUTION_CAPTION);
//		dlg.SetCaption(IDS_EXECUTION_CAPTION);
		//Saiki 20090128 Change <-----
		//CString l_strBuffer;
		LoadStringML(IDS_EXECUTION_CAPTION , l_strBuffer, "EXECUTION");
		dlg.SetCaption(l_strBuffer);

//		dlg.SetMessage("Reference Executing...");
//		dlg.SetMessage("ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X ï¿½ï¿½ï¿½è’†...");
		LoadStringML(IDS_REFER_EXECUTE_MEAASGE , l_strBuffer, "Reference Executing...");
		dlg.SetMessage(l_strBuffer);
		// Kojika 20090529 Change End

		dlg.DoModal();
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.07 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	return TRUE;
}


// ==========================================================================
// ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌŠÖ�ï¿½ï¿½Rï¿½[ï¿½ï¿½
LRESULT CManualMeasurementFormView::OnManuMeaFormFuncCall(WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
		case WM_DISPMENU_MANUAL_MEASUREMENT_START:
			OnMeasurementButton();
			break;
		case WM_DISPMENU_MEASUREMENT_COMPLETE:
			OnMeasurementCompleteButton();
			break;
		case WM_DISPMENU_POINT_TEACHING:
			OnPointTeachingButton();
			break;
		case WM_DISPMENU_MANUMEAS_EXIT:
			OnExitButton();
			break;
	}
	return 0L;
}


// ==========================================================================
//
LRESULT CManualMeasurementFormView::OnManuModeOnePointEnd(WPARAM wparam, LPARAM lparam)
{
	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
	if(m_pAddMeasDlgObj != NULL)
	{
		m_pAddMeasDlgObj->PostMessage(WM_TESTMODE_ONEPOINT_END, (WPARAM)0, (LPARAM)0);
	}

	return 0L;
}



HBRUSH CManualMeasurementFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ï¿½ DC ï¿½ÌƒAï¿½gï¿½ï¿½ï¿½rï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½Ï�Xï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	BOOL bNexMntSw = nexioIsMaintenanceSwitch();
	BOOL bNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();


	if((bNexMntSw == OFF)||(bNexEngiMntSw == ON))
	{
		switch(nCtlColor)
		{
			case CTLCOLOR_STATIC:
				pDC->SetBkColor(RGB(255,0,0));
			default:
				hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
			break;
		}
	}

	// TODO: ï¿½fï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Ìƒuï¿½ï¿½ï¿½Vï¿½ï¿½ï¿½]ï¿½İ‚Ì‚ï¿½ï¿½Ì‚Å‚È‚ï¿½ï¿½ê�‡ï¿½É‚Í�Aï¿½á‚¤ï¿½uï¿½ï¿½ï¿½Vï¿½ï¿½Ô‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	return hbr;
}

void CManualMeasurementFormView::OnTimer(UINT nIDEvent)
{
BOOL sbNexMntSw;
BOOL sbNexEngiMntSw;

	switch(nIDEvent)
	{
		case MANUOPE_MAINTENANCE_WATCH_TIMER:

			sbNexMntSw = nexioIsMaintenanceSwitch();
			sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

			if((sbNexMntSw != m_bNexMntSwOld)||(sbNexEngiMntSw != m_bNexEngiMntSwOld))
			{
				Invalidate(TRUE); //OnCtlColor()ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½×‚ï¿½Call
			}
			m_bNexMntSwOld = sbNexMntSw; //ï¿½Oï¿½ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			m_bNexEngiMntSwOld = sbNexEngiMntSw; //ï¿½Oï¿½ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

			break;

		case MANUOPE_BUTTON_ENABLE_TIMER:
// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
			JoyStickEnableCheck();
// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½N			ï¿½ï¿½ï¿½g */
				if (0 != m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
					/* XY ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½ï¿½D	*/
					StageStop();
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* ï¿½ï¿½ï¿½ì’†ï¿½tï¿½ï¿½ï¿½O(XY ï¿½Xï¿½eï¿½[ï¿½W)ï¿½ï¿½ï¿½Iï¿½t	*/
				}
			}
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			break;
//2009.11.12 bagus MS --{--
		case JOG_WATCH_TIMER_ID:	//JOG Up/Down Switch
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
				RCP_DATA rcp;
				m_pDoc->GetRcpData(&rcp);
				if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
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
			}
			break;
		case JOG_MODE_LAMP_TIMER_ID:	//MODE CHANGEï¿½{ï¿½^ï¿½ï¿½ï¿½Ì“_ï¿½ï¿½/ï¿½_ï¿½ï¿½/ï¿½ï¿½ï¿½ï¿½
			if(m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) || m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)
			//2009.12.25 bagus ï¿½ï¿½ï¿½è’†ï¿½Å‚È‚ï¿½ï¿½ï¿½Î�ï¿½ï¿½ï¿½ --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			){
				//ï¿½ï¿½ï¿½ì’†ï¿½Í�ï¿½ï¿½ï¿½
				nexioMS_ModeLamp(FALSE);
			}else{
				RCP_DATA rcp;
				m_pDoc->GetRcpData(&rcp);
				if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
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
				}else{
					//ï¿½ï¿½ï¿½èƒŒï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å‚Í‚È‚ï¿½ï¿½Ì‚ï¿½
					nexioMS_ModeLamp(FALSE);
				}
			}
			break;
//2009.11.12 bagus MS --}--
	}

	CNanoUI::OnTimer(nIDEvent);
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- { ---------- */
//void CManualMeasurementFormView::OnJoyStick()
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ----------			   */
LRESULT CManualMeasurementFormView::OnJoyStick(WPARAM wparam, LPARAM lparam)
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- } ---------- */
{
	// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Lï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
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
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
/* added 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- { ---------- */
	return 0;
/* added 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- } ---------- */
}

// ==========================================================================
//ï¿½ï¿½ï¿½è’†ï¿½Ì‘ï¿½ï¿½ï¿½Iï¿½ï¿½ ï¿½ï¿½ï¿½Ìƒ{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½/ï¿½ï¿½ï¿½ï¿½ï¿½Ì�Ø‘Ö‚ï¿½ (ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½p)
void CManualMeasurementFormView::ButtonEnableChange(int Select)
{

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‘ï¿½ï¿½ï¿½ï¿½Ô‚É‚ï¿½ï¿½ï¿½ï¿½ï¿½ç‚¸ï¿½Aï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) //ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½
	{
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½É‚Íƒ}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í‘ï¿½ï¿½İ‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ê‰�ï¿½ï¿½`)
		{
			//IDD_MAIN_MENU_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE); //[LOG ON]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

			//IDD_MANUAL_MEASUREMENT_OPERATION_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
				GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
				GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODEï¿½Iï¿½ğ–³Œï¿½
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
			}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

			//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			//Saiki 20090410 Add ----->
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö–ï¿½ï¿½ï¿½
			//Saiki 20090410 Add <-----

			//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
			m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
			m_pMainFrame->m_ManualMeasMenuEnableFlg = FALSE;

			//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
//			SoftJoyStickXyEnable(FALSE);
			JoyStickXyEnable(FALSE);
		}
			return;
	}



	switch(Select){

	case MEASUREMENT_MANUALMEASURE_S:

		//IDD_MANUAL_MEASUREMENT_OPERATION_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODEï¿½Iï¿½ğ–³Œï¿½
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö–ï¿½ï¿½ï¿½
		//Saiki 20090410 Add <-----
		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = FALSE;

		//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
//		SoftJoyStickXyEnable(FALSE);
		JoyStickXyEnable(FALSE);

		break;



	case MEASUREMENT_MANUALMEASURE_E:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(TRUE); 		//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(TRUE);//[MEASUREMENT COMPLETE]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(TRUE);		//[POINT TEACHING]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);			//MOTION MODEï¿½Iï¿½ï¿½Lï¿½ï¿½
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_pDoc->GetManuMeasMode() == MANUMODE_INIT)
		{
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		}

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Ï�Xï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö—Lï¿½ï¿½
		//Saiki 20090410 Add <-----
		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = TRUE;

		//2009.12.23 bagus MS --{--
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(TRUE); 			//MOTION MODEï¿½Iï¿½ğ–³Œï¿½
		//2009.12.23 bagus MS --}--

		//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
//		SoftJoyStickXyEnable(TRUE);
		JoyStickXyEnable(TRUE);

		break;


	case MEASUREMENT_MANUALMEASURE_COMP:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODEï¿½Iï¿½ğ–³Œï¿½
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö–ï¿½ï¿½ï¿½
		//Saiki 20090410 Add <-----

		//2009.12.23 bagus MS --{--
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(FALSE); 			//MOTION MODEï¿½Iï¿½ğ–³Œï¿½
		//2009.12.23 bagus MS --}--

		//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
//		SoftJoyStickXyEnable(FALSE);
		JoyStickXyEnable(FALSE);

		break;


	case MANU_MEAS_FORM_CREATE:
		//IDD_MAIN_MENU_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(FALSE);		//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(FALSE);	//[RECIPE]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE);	//[LOG ON]ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //ï¿½}ï¿½jï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		{
			//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ğ–³Œï¿½
		}


		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;


	case MANU_MEAS_FORM_DELETE:
		//IDD_MAIN_MENU_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(TRUE);//[RECIPE]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		if(m_pDoc->GetHostMode() != HOST_REMOTE) //ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½ï¿½
		{
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(TRUE);	//[LOG ON]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		}

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Ï�Xï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö–ï¿½ï¿½ï¿½
		//Saiki 20090410 Add <-----


		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;

// 2009.10.30 bagus 2ï¿½_ï¿½ï¿½ ï¿½Cï¿½ï¿½ --{--
	case MEASUREMENT_MANUAL_DISTANCE_S:
	case MEASUREMENT_MANUAL_DISTANCE_E:
	case MEASUREMENT_MANUAL_DISTANCE_COMP:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE); 		//[MEASUREMENT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(TRUE);//[MEASUREMENT COMPLETE]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		if(m_pDoc->GetManuMeasMode() == MANUMODE_INIT)
		{
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		}
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(TRUE);		//[POINT TEACHING]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);			//MOTION MODEï¿½Iï¿½ï¿½Lï¿½ï¿½

			// XY STAGE BUTTON HIDE
			GetDlgItem(IDC_STAGE_UP_BUTTON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_RIGHT_BUTTON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_LEFT_BUTTON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_DOWN_BUTTON)->ShowWindow(SW_HIDE);

			//XY STAGE GROUP HIDE
			GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);

			//JoyStick HIDE
			GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

		//IDD_VIEW_WINDOW_FORMï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½{ï¿½ï¿½ï¿½Ï�Xï¿½Lï¿½ï¿½
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ï¿½{ï¿½^ï¿½ï¿½ï¿½Lï¿½ï¿½
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//ï¿½wï¿½bï¿½hï¿½Ø‘Ö—Lï¿½ï¿½
		//Saiki 20090410 Add <-----
		//ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½oï¿½[ï¿½ï¿½(IDR_MEASUREMENT_MENU)ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = TRUE;

		//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì—Lï¿½ï¿½ï¿½ï¿½
//		SoftJoyStickXyEnable(TRUE);
		JoyStickXyEnable(TRUE);

		break;
// 2009.10.30 bagus 2ï¿½_ï¿½ï¿½ ï¿½Cï¿½ï¿½ --}--
	}

}

// =========================================================================
//
void CManualMeasurementFormView::JoyStickXyEnable(BOOL bEnable)
{
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// ï¿½nï¿½[ï¿½hï¿½Wï¿½ï¿½ï¿½C
			m_pDoc->ChangeJoyStick(bEnable);

			//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
		}
		else{
			// ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½C
			m_XY_UpButton.SetEnabled(bEnable);
			m_XY_DownButton.SetEnabled(bEnable);
			m_XY_RightButton.SetEnabled(bEnable);
			m_XY_LeftButton.SetEnabled(bEnable);

			//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
}

#if 0
// =========================================================================
//
void CManualMeasurementFormView::SoftJoyStickXyEnable(BOOL bEnable)
{
	if( m_pMainFrame->GetJoyStickMode() != 0 ) return;
	if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ) return;

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		//SoftJoyStick
		m_XY_UpButton.SetEnabled(bEnable);
		m_XY_DownButton.SetEnabled(bEnable);
		m_XY_RightButton.SetEnabled(bEnable);
		m_XY_LeftButton.SetEnabled(bEnable);
//		m_Z_UpButton.SetEnabled(bEnable);
//		m_Z_DownButton.SetEnabled(bEnable);

		//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
		GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
		GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
}
#endif

// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
// =========================================================================
//
void CManualMeasurementFormView::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½N			ï¿½ï¿½ï¿½g */
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			if ( m_joyStick2->GetCheck() ) {
				// ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½Éƒ\ï¿½tï¿½gJoyï¿½Ö�Ø‚ï¿½Ö‚ï¿½ï¿½ï¿½
				m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
				m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				m_joyStick1->Checked();
				m_joyStick2->UnChecked();
				m_joyStick1->SetFocus();
			}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	}
}
// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--

void CManualMeasurementFormView::OnSelchangeComboMeasPoint()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 2010.02.02 K.Matsuo PIN UPPER ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½`ï¿½Fï¿½bï¿½Nï¿½Rï¿½ï¿½Î‰ï¿½ -->
	CString strMsg;

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Ù�ï¿½
	}
	else{												// ï¿½Ê�í“®ï¿½ï¿½
	}
	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVOï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ 				ï¿½ï¿½ï¿½k
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// ï¿½Vï¿½ï¿½ï¿½bï¿½^ï¿½[ï¿½ÈŠOï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ç“®ï¿½ï¿½Ö�~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 2010.02.02 K.Matsuo PIN UPPER ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½`ï¿½Fï¿½bï¿½Nï¿½Rï¿½ï¿½Î‰ï¿½ <--

	//ï¿½Xï¿½eï¿½[ï¿½Wï¿½Rï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½Ìƒï¿½ï¿½[ï¿½h
	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½[ï¿½h
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
// 2010.01.29 bagus Gantry --{--
#if 0
	iSelectMeasPt = pCombo->GetCurSel();
#else
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
		/* GTR ï¿½Ì�ê�‡*/
		TCHAR l_tszGTRpos[32];
		this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
		iSelectMeasPt = _ttol(l_tszGTRpos);
	} else {
		iSelectMeasPt = pCombo->GetCurSel();
	}
#endif
// 2010.01.29 bagus Gantry --}--
	if(0 < iSelectMeasPt && iSelectMeasPt <= SrConfig.wGantryNumOfMeasPoint){
		//XYï¿½ï¿½ï¿½Wï¿½Ì•\ï¿½ï¿½ï¿½ï¿½Ï�X
		SetPointData(SrReferencePosition, iSelectMeasPt);
		//ï¿½Jï¿½[ï¿½\ï¿½ï¿½ï¿½Ô�ï¿½ï¿½É‘Î‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½eï¿½[ï¿½Wï¿½Ê’uï¿½ÖˆÚ“ï¿½
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
		sampleCoord.lX = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX/* + m_StageProgInfoHdr.SampleInfo.Origin.lX*/;
		sampleCoord.lY = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY/* + m_StageProgInfoHdr.SampleInfo.Origin.lY*/;
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		//ï¿½{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Ì–ï¿½ï¿½ï¿½ï¿½ï¿½
	}
}
// ==========================================================================
// Nameï¿½F		SetPointData
void CManualMeasurementFormView::SetPointData(SR_REFERENCE_POSITION SrReferencePosition, int iSelectMeasPt)
{
	CString csPointNo, csX, csY;

	// X
	csX.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX) / MICROMETRE);
	// Y
	csY.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY) / MICROMETRE);

	SetDlgItemText(IDC_MEAS_POINT_X, csX);
	SetDlgItemText(IDC_MEAS_POINT_Y, csY);
}

// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --{--
// =========================================================================
//ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Cï¿½jï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//
void CManualMeasurementFormView::InitCombo_Lens()
{
	///// Load File /////
	MS_CONFIG MsConfig;
	ConfigFile_GetNanoSpecIni(&MsConfig, CONFIG_FILE_MS_CONFIG);

	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	pCombo->ResetContent();

	int nIndex;
//2010.01.19 bagus MS --{--
	m_nLensSensorItem = -1;
//2010.01.19 bagus MS --}--
	for ( int i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++ ) {
		if (MsConfig.RoboCylinder[i].bEnable ) {
			nIndex = pCombo->AddString(MsConfig.RoboCylinder[i].szName);
			pCombo->SetItemData(nIndex, i);
// matsuhisa 2009.12.26 added >>>
			if (MsConfig.RoboCylinder[i].bSensor) {
				m_nLensSensorItem = i;
// matsuhisa 2009.12.26 added <<<
			}
		}
	}
	//2009.12.25 bagus MS --{--
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(0 <= rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex
	&& rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex < pCombo->GetCount()){
		pCombo->SetCurSel(rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex);
		m_nSelLens = rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex;
	}else{
		pCombo->SetCurSel(0);
		m_nSelLens = 0;
	}
	//2009.12.25 bagus MS --}--
}
// 2009.10.30 bagus MS ï¿½Ç‰ï¿½ --}--

// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--
void CManualMeasurementFormView::OnSelendokMicroScopeFocusPosition()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	int	nIndex = pCombo->GetCurSel();
	int	nItemData = pCombo->GetItemData(nIndex);

	int	nPosNo = m_MsConfig.RoboCylinder[nItemData].nPosition;

	CString strBuffer, strTitle, strMsg;
// matsuhisa 2009.12.26 added >>>
	if ( !nexioIsMS_LensKind() ) {
//2010.01.19 bagus MS --{--
//		if ( nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
		if ( m_nLensSensorItem >= 0 && nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
//2010.01.19 bagus MS --}--
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
	//2009.12.25 bagus --{--
	//if (m_pDoc->IsInterLock() == TRUE) {
	if (m_pDoc->IsInterLockForMScope() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
	//2009.12.25 bagus --}--
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
		return;
	}

	// Zï¿½ï¿½ï¿½ï¿½ï¿½ì‹–ï¿½Âƒï¿½ï¿½ï¿½ï¿½v OFF
	nexioMS_ModeLamp(FALSE);

	// ï¿½Ê’uï¿½ï¿½ï¿½ï¿½
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

	// ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½

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
	}else{
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
}
// 2009.11.02 bagus MS ï¿½Ç‰ï¿½ --}--

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
LRESULT CManualMeasurementFormView::OnNexResponse(WPARAM wparam, LPARAM lparam)
{
	switch ( wparam ) {
	case nexRC_MoveToTeachPos:
	case nexRC_JogPlus:
	case nexRC_JogMinus:
	case nexRC_JogStop:
	case nexRC_SelectMode:
//		if (lparam == 1) {
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
			CCursor::EndWaitCursor();
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
