// ManualMeasurementFormView.cpp : インプリメンテーション ファイル
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
//#define MEASUREMENT_CAPTION	  "手動測定 - ポイントティーチング"
//#define MEASUREMENT_MESSAGE	  "手動測定 - ポイントティーチング"

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

// 2009.11.09 bagus MS 修正 --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS 修正 --}--
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
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		DDX_Control(pDX, IDC_JOYSTICK1, *m_joyStick1);
		DDX_Control(pDX, IDC_JOYSTICK2, *m_joyStick2);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
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
// 2009.11.09 bagus MS 修正 --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS 修正 --}--
END_MESSAGE_MAP()

// ==========================================================================
//
void CManualMeasurementFormView::OnInitialUpdate()
{
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// 画面が表示される時はデフォルトはソフトJoyとする
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
		// Radioボタンの設定
		m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
		m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

// 2009.11.02 bagus MS 追加 --{--
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.02 bagus MS 追加 --}--

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

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_PointTeachingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		/// Point Teaching Button ///
		m_PointTeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_PointTeachingButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

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


// 2009.10.30 bagus MS 追加 --{--
	///// Lens Combo /////
//	RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);

	if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		InitCombo_Lens();
		//2009.12.25 bagus MS --{--
		OnSelendokMicroScopeFocusPosition();
		//起動時にはジョグモードにしておく
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
		//2009.12.25 bagus MS --}--
		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);

	} else {
		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_HIDE);
	}
// 2009.10.30 bagus MS 追加 --}--
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


	// ジョイスティック有効／無効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// H/W有効なら禁止する
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
			// ジョイスティック許可
			bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
			if( bJoyStk == TRUE ){
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			}
		}
#if 1
		else{
			// H/W無効なら許可する
			m_XY_UpButton.SetEnabled(TRUE);
			m_XY_DownButton.SetEnabled(TRUE);
			m_XY_RightButton.SetEnabled(TRUE);
			m_XY_LeftButton.SetEnabled(TRUE);
			// ジョイスティック禁止
			bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		}
#endif
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	m_bNexMntSwOld = TRUE;
	m_bNexEngiMntSwOld = FALSE;
	m_iProcessStatusOld = -1;

	ButtonEnableChange(MANU_MEAS_FORM_CREATE);

	SetTimer(MANUOPE_MAINTENANCE_WATCH_TIMER, 50, NULL);
	SetTimer(MANUOPE_BUTTON_ENABLE_TIMER, 50, NULL);
// 2009.11.09 bagus MS 修正 --{--
	//2009.11.26 bagus MS 修正 --{--
	//SetTimer(JOG_WATCH_TIMER_INTERVAL, JOG_WATCH_TIMER_ID, NULL);
	SetTimer(JOG_WATCH_TIMER_ID, JOG_WATCH_TIMER_INTERVAL, NULL);
	//2009.11.26 bagus MS 修正 --}--
	SetTimer(JOG_MODE_LAMP_TIMER_ID, JOG_MODE_LAMP_TIMER_INTERVAL, NULL);
// 2009.11.09 bagus MS 修正 --}--
}

// ==========================================================================
//
void CManualMeasurementFormView::OnDestroy()
{
BOOL bJoyStk;
BOOL result;

#if 0
	if( m_pMainFrame->GetJoyStickMode()!=0 ){
		// ハードジョイスティック
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
#else
	// 終了する場合は、強制的にソフトJoyへ切り替える
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

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_joyStick1 != NULL ){
			delete m_joyStick1;
			m_joyStick1 = NULL;
		}
		if( m_joyStick2 != NULL ){
			delete m_joyStick2;
			m_joyStick2 = NULL;
		}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	//2009.12.25 bagus MS --{--
	nexioMS_ModeLamp(FALSE);
	//2009.12.25 bagus MS --}--

	//マニュアル測定モード設定
	m_pDoc->SetManuMeasMode(MANUMODE_NOT); //マニュアルモード起動直後(リファレンス測定中含む)(測定を一回もやっていない状態)

	ButtonEnableChange(MANU_MEAS_FORM_DELETE);

	CNanoUI::OnDestroy();
}
//Saiki 20090924 Add ----->
// ==========================================================================
//
void CManualMeasurementFormView::Init_SR_XYStageMenu()
{
// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
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
// 2013.11.08 Bagus Add (TohoSpec対応) <--
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
// 2009.10.29 bagus Jog 追加修正 --{--
//		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		if (m_SystemConfig.nJoystickType == 0) {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		} else {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		}
// 2009.10.29 bagus Jog 追加修正 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

// 2009.10.26 K.Matsuo リソースにないので、落ちてしまうため、delete -->
//	//Lens Disable
//	GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);
// 2009.10.26 K.Matsuo リソースにないので、落ちてしまうため、delete <--

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

// 2009.10.26 K.Matsuo リソースにないので、落ちてしまうため、delete -->
//	//Lens Disable
//	GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);
// 2009.10.26 K.Matsuo リソースにないので、落ちてしまうため、delete <--

	//Measurement Point Group Show
	GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_SHOW);

	//Point No. Static Show
	GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_SHOW);

	//Gantry Number Of Measure Point Show & Initialize
	GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_SHOW);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	//2009.12.11 bagus 修正 --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//pCombo->AddString("<Select>");
	pCombo->AddString(strSelectCaption);
	//2009.12.11 bagus 修正 --{--
// 2010.01.29 bagus Gantry --{--
#if 0
	for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
		itoa(iMeasPt, szMeasPt, 10);
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
		/* GTR の場合*/
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
					itoa(iMeasPt, szMeasPt, 10);
					pCombo->AddString(szMeasPt);
					break;
				}
			}
		}
	} else {
		for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
			itoa(iMeasPt, szMeasPt, 10);
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
	// １ポイント測定用のコードを追加してください
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

/* added 2016.02.24 hmenjo バッチ処理 ---------- { ---------- */
	/* キー同時押しチェック	*/
	BOOL l_bBatch = FALSE;
	if (
		(0 != (::GetAsyncKeyState(VK_SHIFT) & 0x8000)) &&		/* Shift キー	*/
		(0 != (::GetAsyncKeyState(VK_CONTROL) & 0x8000)) &&	/* Ctrl キー	*/
		(0 != (::GetAsyncKeyState(VK_MENU) & 0x8000)) &&		/* Alt キー	*/
		1
		) {
		/* バッチ確定	*/
		l_bBatch = TRUE;
	}
/* added 2016.02.24 hmenjo バッチ処理 ---------- } ---------- */

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;


	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if( m_pDoc->Rap_IsAirPressureLowON(1)!= OFF) {AlarmIf_Set(ALID_AirPressureDown); return;} /* エア圧力低下 */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲



	//マニュアル測定モード設定
	m_pDoc->SetManuMeasMode(MANUMODE_ON_MEASURE); //測定中に設定


/* added 2016.02.24 hmenjo バッチ処理 ---------- { ---------- */
	if (FALSE != l_bBatch) {
		/* バッチ確定	*/
		MEAS_BatchFlagSet(1);
	}
/* added 2016.02.24 hmenjo バッチ処理 ---------- } ---------- */
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoClr();
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ShowCompletionPointSnap(0, 0);
	m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSet(0, FALSE, FALSE);
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */

	ManuMeas();

/* deleted 2014.10.28 hmenjo 測定結果 Auto Scroll ---------- { ---------- */
/*		保留	*/
///* added 2014.10.27 hmenjo 測定結果 Auto Scroll ---------- { ---------- */
//	// 2014.10.23 bagus. for lost focus protection. -->
//	SetFocus();
//	// 2014.10.23 bagus. for lost focus protection. <--
///* added 2014.10.27 hmenjo 測定結果 Auto Scroll ---------- } ---------- */
/* deleted 2014.10.28 hmenjo 測定結果 Auto Scroll ---------- } ---------- */
}

// =========================================================================
//
void CManualMeasurementFormView::OnMeasurementCompleteButton()
{
	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲



	SYSTEMTIME syEndDateTime;	  //測定終了日時
	GetLocalTime(&syEndDateTime); //測定終了日時更新
	m_pDoc->SetEndDateTime(&syEndDateTime);


	//一枚終了の処理関数をcall(EXITボタンのやつ)
	if(m_pDoc->GetHostMode() == HOST_REMOTE)
	{
		//リモートはマニュアル測定がないので、ここの処理はされないはず。
		if ( m_pMainFrame->MeasEnd() == IDCANCEL )
			return;
	}


	//チーフへメッセージを投げる
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		::PostMessage(l_hChiefDlg, WM_DATA_MEAS_END, 0, 0); //1枚測定終了応答
	} else {
			AlarmIf_Set(ALID_OneSampledMeasurementFailed);
	}

	m_pDoc->SetProcessStatus(PROCESS_WAIT);

	//シーケンス測定モードフラグ設定
	m_pDoc->SetSeqMeasMode(SEQ_NOT_MEASURE); //測定中でないに設定

	m_pDoc->SetReMeasurePointCnt(0);


	//マニュアル測定モード設定
	m_pDoc->SetManuMeasMode(MANUMODE_MEAS_COMP); //測定終了に設定

	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_COMP);

}

// =========================================================================
//
void CManualMeasurementFormView::OnPointTeachingButton()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg, strMsg;
	// Kojika 20090529 Add End

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	//2009.10.30 bagus 2point-distance --{--
	//2点間距離の場合にはポイントティーチングは測定処理
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
	//顕微鏡測定の場合には、測定処理を行う。
	{
		RCP_DATA rcp_data;
		m_pDoc->GetRcpData(&rcp_data);
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
			OnMeasurementButton();
			return;
		}
	}
	//2009.11.12 bagus MS --}--

	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if( m_pDoc->Rap_IsAirPressureLowON(1)!= OFF) {AlarmIf_Set(ALID_AirPressureDown); return;} /* エア圧力低下 */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	CAdditionalMeasurementDlg dlg;

	dlg.m_joyContinue = TRUE;					// JoyStickの状態を継続させる

	// Kojika 20090529 Change
	//dlg.SetCaption(MEASUREMENT_CAPTION);
	//dlg.SetMessage(MEASUREMENT_MESSAGE);
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_CAPTION, l_strCap, "MANUAL MEASUREMENT - POINT TEACHING");
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_MESSAGE, l_strMsg, "MANUAL MEASUREMENT - POINT TEACHING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
// 2009.10.30 bagus Stage-Sample 修正 --{--
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	BOOL bMain,bStage;

	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);

	if((bMain == FALSE) || (bStage == FALSE)){
		//レシピが読めなかったのでサンプル表示はなし
		dlg.m_nSampleMode = 0;
	}else{
		dlg.m_nSampleMode = 1;
	}

	strcpy(dlg.m_szRecipeName, rcp_data.StageProgInfoHdr.SampleInfo.szName);
// 2009.10.30 bagus Stage-Sample 修正 --}--
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
	// H/W有効なら
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

	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	BOOL l_bRc = SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);		/* SR ヘッドに戻します．	*/
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	/* SR フィルタを OPEN します．	*/
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲

	if ( m_pMainFrame->MeasEnd() == IDCANCEL )
		return;


	//2009.11.04 bagus CA --{--
	//コンタクトアングル測定の場合には、後処理が必要
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
		m_pDoc->CleanupManualCAMeasure();
	}
	//2009.11.04 bagus CA --}--

	//2009.12.25 bagus MS --{--
	//終了時にシリンダを安全位置に戻す
	if (rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		nexioMS_ModeLamp(FALSE);
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
		MSG msg;
		//応答を処理するまで待つ
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
		//応答を処理するまで待つ
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

//▽▽▽ 090326 hibino add
	((CMainFrame*)AfxGetMainWnd())->ChangeOperationPanel(OP_MEASUREMENT);
	((CMainFrame*)AfxGetMainWnd())->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
//△△△ 090326 hibino add

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

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return 0L;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return 0L;} // メンテナンス異常
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return 0L;} // MOVOアラーム 				＝Ｌ
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
		}	// ステージ動作中
		//Saiki 20090531 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲

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
					// 低速・中速・高速
					if ( StageIsIdle() ) {
						StageMoveAtSpeedEx(wAxis, (double)(MotionJog * sign)/MICROMETRE, FALSE);
						m_bIsMoveAtSpeed = TRUE;
						if ( lParam == 1 ) {
							// キーボード操作は、WM_STAGE_MOVE_DONEの受信で停止する
							return 0L;
						}
					}
				}else{
					// ジョグ（低）・ジョグ（中）・ジョグ（高）
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
				// マウス操作は、ここで停止させる
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
			// H/W無効なら
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
			// H/W無効なら
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
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//void CManualMeasurementFormView::ManuMeas(void)
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
BOOL CManualMeasurementFormView::ManuMeas(void)
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
{
// Kojika 20090604 Add
	CString strBuffer, strTitle;
// Kojika 20090604 Add End

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
//	char* pszCaption = "MANUAL MEASUREMENT START";
	//char* pszCaption = "手動測定開始";
	CString l_strBuffer;
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	if(m_pDoc->GetStoreCount() >= SCAN_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
// Kojika 20090604 Change
//		sprintf(szMsgBuff, "Measurement data %d over!", SCAN_POINT_MAX);
//		sprintf(szMsgBuff, "測定データが %d ポイントを超えています!", SCAN_POINT_MAX);
//		MessageBox(szMsgBuff, "NANO SPEC", MB_OK);
		LoadStringML(IDS_MANUAL_MEASURE_OVER, strBuffer, "Measurement data %d over!");
		sprintf(szMsgBuff, strBuffer, SCAN_POINT_MAX);
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
		MessageBox(szMsgBuff, strTitle, MB_OK);
// Kojika 20090604 Change End
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}


	///// チーフへ測定開始メッセージを送信 /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_ManualMeasurement);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}

	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		//Saiki 20090411 Add ----->
		///// レンズ切替 /////
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
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

/* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT/*WM_DISP_START_SEQ*/, (WPARAM)rcp_data.szRecipeName, 0); //通常測定
/* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ----------			   */
	int l_iMeasPos;
	if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
		/* GTR の場合*/
		TCHAR l_tszGTRpos[32];
		this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
		l_iMeasPos = _ttol(l_tszGTRpos);

		//2010.01.29 bagus GTR 修正 --{--
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
		//2010.01.29 bagus GTR 修正 --}--

	} else {
/* modified 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
//		l_iMeasPos = m_pDoc->GetStoreCount() + 1;
/* modified 2016.05.05 hmenjo 手動 再測定/指定統計 ----------              */
		if (0 == m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
			/* 通常(新規)測定	*/
			l_iMeasPos = m_pDoc->GetStoreCount() + 1;
		} else {
			/* 再測定の指定	*/
			l_iMeasPos = m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoGet();
		}
/* modified 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */
	}
	if (0 == l_iMeasPos) {
		ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
// 2010.01.29 bagus Gantry --{--
		LoadStringML(IDS_MANUAL_MEASURE_SELECT_POINT, strBuffer, "Select measurement point.");
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
		MessageBox(strBuffer, strTitle, MB_OK);
// 2010.01.29 bagus Gantry --}--
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		 return;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(l_iMeasPos, 0));
/* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- } ---------- */

	///// チーフの結果 /////
	// Kojika 20090529 Change
	LoadStringML(IDS_MANUAL_MEASUREMENT_START, l_strBuffer, "MANUAL MEASUREMENT START");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - MANUAL MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
	}

/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	return TRUE;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
}


// ==========================================================================
// リファレンス取得、開始時間取得など
BOOL CManualMeasurementFormView::GetReferenceManuStart(void)
{

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
//	char* pszCaption = "MANUAL MEASUREMENT REFERENCE START";
//	char* pszCaption = "手動リファレンス測定開始";
	CString l_strBuffer;
	// Kojika 20090529 Change End

	SYSTEMTIME syStartDateTime; 	//測定開始日時


	m_pDoc->GetRcpData(&rcp_data);


	///// 測定開始日時取得 /////
	GetLocalTime(&syStartDateTime);
	m_pDoc->SetStartDateTime(&syStartDateTime);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		///// Clear Reference /////
		ClearReference(rcp_data.szRecipeName);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		return FALSE;
	}

	// ボタンを変更
	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// チーフの結果 /////
	// Kojika 20090529 Change
	LoadStringML(IDS_MANUAL_MEASUREMENT_REF_START, l_strBuffer, "MANUAL MEASUREMENT REFERENCE START");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		// ボタンを戻す
//		ButtonEnableChange(MANU_MEAS_FORM_CREATE);
		return FALSE;
	}


//	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
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
//		dlg.SetMessage("リファレンス 測定中...");
		LoadStringML(IDS_REFER_EXECUTE_MEAASGE , l_strBuffer, "Reference Executing...");
		dlg.SetMessage(l_strBuffer);
		// Kojika 20090529 Change End

		dlg.DoModal();
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	return TRUE;
}


// ==========================================================================
// メニューボタンからの関数コール
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

	// TODO: この位置で DC のアトリビュートを変更してください

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

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
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
				Invalidate(TRUE); //OnCtlColor()の処理をさせる為にCall
			}
			m_bNexMntSwOld = sbNexMntSw; //前回値書換え
			m_bNexEngiMntSwOld = sbNexEngiMntSw; //前回値書換え

			break;

		case MANUOPE_BUTTON_ENABLE_TIMER:
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
			JoyStickEnableCheck();
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 <--
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* ドアインターロック			＝Ｈ */
				if (0 != m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
					/* XY ステージ動作中です．	*/
					StageStop();
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* 動作中フラグ(XY ステージ)をオフ	*/
				}
			}
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			break;
//2009.11.12 bagus MS --{--
		case JOG_WATCH_TIMER_ID:	//JOG Up/Down Switch
//2009.12.15 bagus MS --{--
//			if (m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
			if (m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) || m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)
			//2009.12.25 bagus 測定中でなければ止める --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			) {
//2009.12.15 bagus MS --}--
				//動作中なので動かさせない
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
							//同時押し
						}else if(nexioIsMS_UpSwitch() && nexioIsMS_UpperPos()){
							if(nexifRC_JogMinus(this->m_hWnd)){
								m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
							}
						}else if(nexioIsMS_DownSwitch() ){
							if(!nexioIsMS_LowerPos1() && !nexioIsMS_LensKind()){
								//下降端１でレンズ種別が1-50x
							}else if(!nexioIsMS_LowerPos2()){
								//一番下
							}else{
								//上昇
								if(nexifRC_JogPlus(this->m_hWnd)){
									m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
								}
							}
						}else{
							//ボタンが離されたのでジョグを止める
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
		case JOG_MODE_LAMP_TIMER_ID:	//MODE CHANGEボタンの点灯/点滅/消灯
			if(m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) || m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)
			//2009.12.25 bagus 測定中でなければ消す --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			){
				//動作中は消灯
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
					//測定レシピが顕微鏡ではないので
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
/* modified 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
//void CManualMeasurementFormView::OnJoyStick()
/* modified 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ----------			   */
LRESULT CManualMeasurementFormView::OnJoyStick(WPARAM wparam, LPARAM lparam)
/* modified 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */
{
	// ジョイスティック有効／無効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			m_joyStick1->UnChecked();
			m_joyStick2->Checked();
			// ジョイスティック許可
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
			m_joyStick1->Checked();
			m_joyStick2->UnChecked();
			// ジョイスティック禁止
			m_XY_UpButton.SetEnabled(TRUE);
			m_XY_DownButton.SetEnabled(TRUE);
			m_XY_RightButton.SetEnabled(TRUE);
			m_XY_LeftButton.SetEnabled(TRUE);
		}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
	return 0;
/* added 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- } ---------- */
}

// ==========================================================================
//測定中⇔測定終了 時のボタン有効/無効の切替え (マニュアルモード専用)
void CManualMeasurementFormView::ButtonEnableChange(int Select)
{

	//リモート時は測定状態にかかわらず、ボタン押下制限をかける
	if(m_pDoc->GetHostMode() == HOST_REMOTE) //リモート時
	{
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //マニュアル測定(←リモート時にはマニュアル測定は存在しないが一応定義)
		{
			//IDD_MAIN_MENU_FORM内コントロールの無効化
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE); //[LOG ON]ボタン無効

			//IDD_MANUAL_MEASUREMENT_OPERATION_FORM内コントロールの無効化
			GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ボタン無効
			GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ボタン無効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
				GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]ボタン無効
				GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODE選択無効
// 2013.11.08 Bagus Add (TohoSpec対応) -->
			}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ボタン無効

			//IDD_VIEW_WINDOW_FORM内コントロールの無効化
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//レンズ倍率変更無効
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ボタン無効
			//Saiki 20090410 Add ----->
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ヘッド切替無効
			//Saiki 20090410 Add <-----

			//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
			m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
			m_pMainFrame->m_ManualMeasMenuEnableFlg = FALSE;

			//ソフトジョイスティックの無効化
//			SoftJoyStickXyEnable(FALSE);
			JoyStickXyEnable(FALSE);
		}
			return;
	}



	switch(Select){

	case MEASUREMENT_MANUALMEASURE_S:

		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM内コントロールの無効化
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ボタン無効
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ボタン無効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]ボタン無効
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODE選択無効
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]ボタン無効

		//IDD_VIEW_WINDOW_FORM内コントロールの無効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//レンズ倍率変更無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ボタン無効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ヘッド切替無効
		//Saiki 20090410 Add <-----
		//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = FALSE;

		//ソフトジョイスティックの無効化
//		SoftJoyStickXyEnable(FALSE);
		JoyStickXyEnable(FALSE);

		break;



	case MEASUREMENT_MANUALMEASURE_E:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM内コントロールの有効化
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(TRUE); 		//[MEASUREMENT]ボタン有効
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(TRUE);//[MEASUREMENT COMPLETE]ボタン有効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(TRUE);		//[POINT TEACHING]ボタン有効
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);			//MOTION MODE選択有効
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_pDoc->GetManuMeasMode() == MANUMODE_INIT)
		{
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]ボタン有効
		}

		//IDD_VIEW_WINDOW_FORM内コントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//レンズ倍率変更有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ボタン有効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//ヘッド切替有効
		//Saiki 20090410 Add <-----
		//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = TRUE;

		//2009.12.23 bagus MS --{--
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(TRUE); 			//MOTION MODE選択無効
		//2009.12.23 bagus MS --}--

		//ソフトジョイスティックの有効化
//		SoftJoyStickXyEnable(TRUE);
		JoyStickXyEnable(TRUE);

		break;


	case MEASUREMENT_MANUALMEASURE_COMP:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM内コントロールの無効化
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ボタン無効
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ボタン無効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]ボタン無効
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODE選択無効
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]ボタン有効

		//IDD_VIEW_WINDOW_FORM内コントロールの無効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//レンズ倍率変更無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]ボタン無効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ヘッド切替無効
		//Saiki 20090410 Add <-----

		//2009.12.23 bagus MS --{--
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(FALSE); 			//MOTION MODE選択無効
		//2009.12.23 bagus MS --}--

		//ソフトジョイスティックの無効化
//		SoftJoyStickXyEnable(FALSE);
		JoyStickXyEnable(FALSE);

		break;


	case MANU_MEAS_FORM_CREATE:
		//IDD_MAIN_MENU_FORM内コントロールの無効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(FALSE);		//[MEASUREMENT]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(FALSE);	//[RECIPE]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE);	//[LOG ON]ボタン無効

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //マニュアル測定
		{
			//IDD_VIEW_WINDOW_FORM内コントロールの無効化
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//カメラ選択無効
		}


		//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;


	case MANU_MEAS_FORM_DELETE:
		//IDD_MAIN_MENU_FORM内コントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//[MEASUREMENT]ボタン有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(TRUE);//[RECIPE]ボタン有効
		if(m_pDoc->GetHostMode() != HOST_REMOTE) //ローカル時
		{
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(TRUE);	//[LOG ON]ボタン有効
		}

		//IDD_VIEW_WINDOW_FORM内コントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//レンズ倍率変更有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ボタン有効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//ヘッド切替無効
		//Saiki 20090410 Add <-----


		//メニューバー内(IDR_MEASUREMENT_MENU)の有効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;

// 2009.10.30 bagus 2点間 修正 --{--
	case MEASUREMENT_MANUAL_DISTANCE_S:
	case MEASUREMENT_MANUAL_DISTANCE_E:
	case MEASUREMENT_MANUAL_DISTANCE_COMP:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM内コントロールの有効化
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE); 		//[MEASUREMENT]ボタン有効
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(TRUE);//[MEASUREMENT COMPLETE]ボタン有効
		if(m_pDoc->GetManuMeasMode() == MANUMODE_INIT)
		{
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]ボタン有効
		}
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(TRUE);		//[POINT TEACHING]ボタン有効
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);			//MOTION MODE選択有効

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
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

		//IDD_VIEW_WINDOW_FORM内コントロールの有効化
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//レンズ倍率変更有効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]ボタン有効
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//ヘッド切替有効
		//Saiki 20090410 Add <-----
		//メニューバー内(IDR_MEASUREMENT_MENU)の無効化フラグ操作
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = TRUE;

		//ソフトジョイスティックの有効化
//		SoftJoyStickXyEnable(TRUE);
		JoyStickXyEnable(TRUE);

		break;
// 2009.10.30 bagus 2点間 修正 --}--
	}

}

// =========================================================================
//
void CManualMeasurementFormView::JoyStickXyEnable(BOOL bEnable)
{
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// ハードジョイ
			m_pDoc->ChangeJoyStick(bEnable);

			//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
		}
		else{
			// ソフトジョイ
			m_XY_UpButton.SetEnabled(bEnable);
			m_XY_DownButton.SetEnabled(bEnable);
			m_XY_RightButton.SetEnabled(bEnable);
			m_XY_LeftButton.SetEnabled(bEnable);

			//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
		}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}

#if 0
// =========================================================================
//
void CManualMeasurementFormView::SoftJoyStickXyEnable(BOOL bEnable)
{
	if( m_pMainFrame->GetJoyStickMode() != 0 ) return;
	if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ) return;

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		//SoftJoyStick
		m_XY_UpButton.SetEnabled(bEnable);
		m_XY_DownButton.SetEnabled(bEnable);
		m_XY_RightButton.SetEnabled(bEnable);
		m_XY_LeftButton.SetEnabled(bEnable);
//		m_Z_UpButton.SetEnabled(bEnable);
//		m_Z_DownButton.SetEnabled(bEnable);

		//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
		GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
		GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}
#endif

// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
// =========================================================================
//
void CManualMeasurementFormView::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* ドアインターロック			＝Ｈ */
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			if ( m_joyStick2->GetCheck() ) {
				// 強制的にソフトJoyへ切り替える
				m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
				m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				m_joyStick1->Checked();
				m_joyStick2->UnChecked();
				m_joyStick1->SetFocus();
			}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	}
}
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 <--

void CManualMeasurementFormView::OnSelchangeComboMeasPoint()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
// 2010.02.02 K.Matsuo PIN UPPER インターロックチェック漏れ対応 -->
	CString strMsg;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // メンテナンス異常
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVOアラーム 				＝Ｌ
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// シャッター以外が動いていたら動作禁止
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// ステージ動作中
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ▲インターロック条件▲
// 2010.02.02 K.Matsuo PIN UPPER インターロックチェック漏れ対応 <--

	//ステージコンフィグのロード
	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	//リファレンスポジションのロード
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
		/* GTR の場合*/
		TCHAR l_tszGTRpos[32];
		this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
		iSelectMeasPt = _ttol(l_tszGTRpos);
	} else {
		iSelectMeasPt = pCombo->GetCurSel();
	}
#endif
// 2010.01.29 bagus Gantry --}--
	if(0 < iSelectMeasPt && iSelectMeasPt <= SrConfig.wGantryNumOfMeasPoint){
		//XY座標の表示を変更
		SetPointData(SrReferencePosition, iSelectMeasPt);
		//カーソル番号に対応したステージ位置へ移動
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
		sampleCoord.lX = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX/* + m_StageProgInfoHdr.SampleInfo.Origin.lX*/;
		sampleCoord.lY = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY/* + m_StageProgInfoHdr.SampleInfo.Origin.lY*/;
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		//ボタン等の無効化
	}
}
// ==========================================================================
// Name：		SetPointData
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

// 2009.10.30 bagus MS 追加 --{--
// =========================================================================
//レンズ コンボイニシャル処理
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
// 2009.10.30 bagus MS 追加 --}--

// 2009.11.02 bagus MS 追加 --}--
void CManualMeasurementFormView::OnSelendokMicroScopeFocusPosition()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

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

// 2009.11.09 bagus MS 修正 --{--
	if ( CCursor::m_bWaitCursor ) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
// 2009.11.09 bagus MS 修正 --{--
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
// 2009.11.09 bagus MS 修正 --{--
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
// 2009.11.09 bagus MS 修正 --}--
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
// 2009.11.09 bagus MS 修正 --{--
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
// 2009.11.09 bagus MS 修正 --}--

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
// 2009.11.09 bagus MS 修正 --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS 修正 --}--
	} //ステージが動作中です
// 2009.11.09 bagus MS 修正 --{--
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--
	// ▲インターロック条件▲

// 2009.11.09 bagus MS 修正 --{--
#if 0
	// ActuateFlagをセットする
	m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

	// 他の移動入力の受付を禁止する
	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	// Z軸動作許可ランプ OFF
	nexioMS_ModeLamp(FALSE);

	// 位置決め
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

	// 移動完了待ち

	// Z軸動作許可ランプ ON
	nexioMS_ModeLamp(TRUE);

	// 他の移動入力の受付を許可する
	{	// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
	}

	// ActuateFlagを戻す
	m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
#else
	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
// matsuhisa 2009.12.26 deleted >>>
//		if (m_MsConfig.RoboCylinder[nItemData].bSensor == nexioIsMS_LensKind()) {
// matsuhisa 2009.12.26 deleted <<<
			// 他の移動入力の受付を禁止する
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
				MessageBox(strMsg, strTitle, MB_OK);
				// 必ずJoyStickの状態を戻すこと
				m_pDoc->JoyStickStatusRestore();
				pCombo->SetCurSel(m_nSelLens);
				return;
			}

			// ActuateFlagをセットする
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

			// Mode切り替え
			unsigned char oldMode = nexioRC_GetCurrentMode();
			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
				MSG 	msg;

				//応答を処理するまで待つ
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

			// 位置決め
			if(nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo)){
				// ActuateFlagをセットする
				m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
				MSG 	msg;

				//応答を処理するまで待つ
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

				//応答を処理するまで待つ
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
//			// 顕微鏡のレンズの設定が一致しない
//			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
//			m_pDoc->MessageStringIf_Set(strMsg);
//			pCombo->SetCurSel(m_nSelLens);
//		}
// matsuhisa 2009.12.26 deleted <<<
	}else{
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS 修正 --}--
}
// 2009.11.02 bagus MS 追加 --}--

// 2009.11.09 bagus MS 修正 --{--
void CManualMeasurementFormView::OnNexResponse(WPARAM wparam, LPARAM lparam)
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
			// アラーム
//		}
		break;
	default:
		break;
	}
}
// 2009.11.09 bagus MS 修正 --}--
