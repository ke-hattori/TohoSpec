// ManualMeasurementFormView.cpp : �C���v�������e�[�V���� �t�@�C��
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
//#define MEASUREMENT_CAPTION	  "�蓮���� - �|�C���g�e�B�[�`���O"
//#define MEASUREMENT_MESSAGE	  "�蓮���� - �|�C���g�e�B�[�`���O"

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

// 2009.11.09 bagus MS �C�� --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS �C�� --}--
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
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		DDX_Control(pDX, IDC_JOYSTICK1, *m_joyStick1);
		DDX_Control(pDX, IDC_JOYSTICK2, *m_joyStick2);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
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
// 2009.11.09 bagus MS �C�� --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS �C�� --}--
END_MESSAGE_MAP()

// ==========================================================================
//
void CManualMeasurementFormView::OnInitialUpdate()
{
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// ��ʂ��\������鎞�̓f�t�H���g�̓\�t�gJoy�Ƃ���
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Mod (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
		// Radio�{�^���̐ݒ�
		m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
		m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

// 2009.11.02 bagus MS �ǉ� --{--
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.02 bagus MS �ǉ� --}--

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

// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		m_PointTeachingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		/// Point Teaching Button ///
		m_PointTeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_PointTeachingButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

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


// 2009.10.30 bagus MS �ǉ� --{--
	///// Lens Combo /////
//	RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);

	if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		InitCombo_Lens();
		//2009.12.25 bagus MS --{--
		OnSelendokMicroScopeFocusPosition();
		//�N�����ɂ̓W���O���[�h�ɂ��Ă���
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
		//2009.12.25 bagus MS --}--
		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);

	} else {
		GetDlgItem(IDC_MS_FOCUS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->ShowWindow(SW_HIDE);
	}
// 2009.10.30 bagus MS �ǉ� --}--
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


	// �W���C�X�e�B�b�N�L���^����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// H/W�L���Ȃ�֎~����
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
			// �W���C�X�e�B�b�N����
			bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
			if( bJoyStk == TRUE ){
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			}
		}
#if 1
		else{
			// H/W�����Ȃ狖����
			m_XY_UpButton.SetEnabled(TRUE);
			m_XY_DownButton.SetEnabled(TRUE);
			m_XY_RightButton.SetEnabled(TRUE);
			m_XY_LeftButton.SetEnabled(TRUE);
			// �W���C�X�e�B�b�N�֎~
			bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		}
#endif
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	m_bNexMntSwOld = TRUE;
	m_bNexEngiMntSwOld = FALSE;
	m_iProcessStatusOld = -1;

	ButtonEnableChange(MANU_MEAS_FORM_CREATE);

	SetTimer(MANUOPE_MAINTENANCE_WATCH_TIMER, 50, NULL);
	SetTimer(MANUOPE_BUTTON_ENABLE_TIMER, 50, NULL);
// 2009.11.09 bagus MS �C�� --{--
	//2009.11.26 bagus MS �C�� --{--
	//SetTimer(JOG_WATCH_TIMER_INTERVAL, JOG_WATCH_TIMER_ID, NULL);
	SetTimer(JOG_WATCH_TIMER_ID, JOG_WATCH_TIMER_INTERVAL, NULL);
	//2009.11.26 bagus MS �C�� --}--
	SetTimer(JOG_MODE_LAMP_TIMER_ID, JOG_MODE_LAMP_TIMER_INTERVAL, NULL);
// 2009.11.09 bagus MS �C�� --}--
}

// ==========================================================================
//
void CManualMeasurementFormView::OnDestroy()
{
BOOL bJoyStk;
BOOL result;

#if 0
	if( m_pMainFrame->GetJoyStickMode()!=0 ){
		// �n�[�h�W���C�X�e�B�b�N
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
#else
	// �I������ꍇ�́A�����I�Ƀ\�t�gJoy�֐؂�ւ���
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

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_joyStick1 != NULL ){
			delete m_joyStick1;
			m_joyStick1 = NULL;
		}
		if( m_joyStick2 != NULL ){
			delete m_joyStick2;
			m_joyStick2 = NULL;
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	//2009.12.25 bagus MS --{--
	nexioMS_ModeLamp(FALSE);
	//2009.12.25 bagus MS --}--

	//�}�j���A�����胂�[�h�ݒ�
	m_pDoc->SetManuMeasMode(MANUMODE_NOT); //�}�j���A�����[�h�N������(���t�@�����X���蒆�܂�)(�������������Ă��Ȃ����)

	ButtonEnableChange(MANU_MEAS_FORM_DELETE);

	CNanoUI::OnDestroy();
}
//Saiki 20090924 Add ----->
// ==========================================================================
//
void CManualMeasurementFormView::Init_SR_XYStageMenu()
{
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
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
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
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
// 2009.10.29 bagus Jog �ǉ��C�� --{--
//		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		if (m_SystemConfig.nJoystickType == 0) {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		} else {
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		}
// 2009.10.29 bagus Jog �ǉ��C�� --}--
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

// 2009.10.26 K.Matsuo ���\�[�X�ɂȂ��̂ŁA�����Ă��܂����߁Adelete -->
//	//Lens Disable
//	GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);
// 2009.10.26 K.Matsuo ���\�[�X�ɂȂ��̂ŁA�����Ă��܂����߁Adelete <--

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

// 2009.10.26 K.Matsuo ���\�[�X�ɂȂ��̂ŁA�����Ă��܂����߁Adelete -->
//	//Lens Disable
//	GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);
// 2009.10.26 K.Matsuo ���\�[�X�ɂȂ��̂ŁA�����Ă��܂����߁Adelete <--

	//Measurement Point Group Show
	GetDlgItem(IDC_MEAS_POINT_STATIC)->ShowWindow(SW_SHOW);

	//Point No. Static Show
	GetDlgItem(IDC_POINT_NO_STATIC)->ShowWindow(SW_SHOW);

	//Gantry Number Of Measure Point Show & Initialize
	GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_SHOW);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	//2009.12.11 bagus �C�� --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//pCombo->AddString("<Select>");
	pCombo->AddString(strSelectCaption);
	//2009.12.11 bagus �C�� --{--
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
		/* GTR �̏ꍇ*/
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
	// �P�|�C���g����p�̃R�[�h��ǉ����Ă�������
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

/* added 2016.02.24 hmenjo �o�b�`���� ---------- { ---------- */
	/* �L�[���������`�F�b�N	*/
	BOOL l_bBatch = FALSE;
	if (
		(0 != (::GetAsyncKeyState(VK_SHIFT) & 0x8000)) &&		/* Shift �L�[	*/
		(0 != (::GetAsyncKeyState(VK_CONTROL) & 0x8000)) &&	/* Ctrl �L�[	*/
		(0 != (::GetAsyncKeyState(VK_MENU) & 0x8000)) &&		/* Alt �L�[	*/
		1
		) {
		/* �o�b�`�m��	*/
		l_bBatch = TRUE;
	}
/* added 2016.02.24 hmenjo �o�b�`���� ---------- } ---------- */

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;


	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //�����e�i���X�E�X�C�b�`�E�I��
	}

	// �eI/O�`�F�b�N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- { -------- */
	if( m_pDoc->Rap_IsAirPressureLowON(1)!= OFF) {AlarmIf_Set(ALID_AirPressureDown); return;} /* �G�A���͒ቺ */
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������



	//�}�j���A�����胂�[�h�ݒ�
	m_pDoc->SetManuMeasMode(MANUMODE_ON_MEASURE); //���蒆�ɐݒ�


/* added 2016.02.24 hmenjo �o�b�`���� ---------- { ---------- */
	if (FALSE != l_bBatch) {
		/* �o�b�`�m��	*/
		MEAS_BatchFlagSet(1);
	}
/* added 2016.02.24 hmenjo �o�b�`���� ---------- } ---------- */
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoClr();
	m_pMainFrame->m_pMeaTabViewObj->MRCS_ShowCompletionPointSnap(0, 0);
	m_pMainFrame->m_pMeaTabViewObj->MRCS_IncludeColorSet(0, FALSE, FALSE);
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

	ManuMeas();

/* deleted 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- { ---------- */
/*		�ۗ�	*/
///* added 2014.10.27 hmenjo ���茋�� Auto Scroll ---------- { ---------- */
//	// 2014.10.23 bagus. for lost focus protection. -->
//	SetFocus();
//	// 2014.10.23 bagus. for lost focus protection. <--
///* added 2014.10.27 hmenjo ���茋�� Auto Scroll ---------- } ---------- */
/* deleted 2014.10.28 hmenjo ���茋�� Auto Scroll ---------- } ---------- */
}

// =========================================================================
//
void CManualMeasurementFormView::OnMeasurementCompleteButton()
{
	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
	}

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������



	SYSTEMTIME syEndDateTime;	  //����I������
	GetLocalTime(&syEndDateTime); //����I�������X�V
	m_pDoc->SetEndDateTime(&syEndDateTime);


	//�ꖇ�I���̏����֐���call(EXIT�{�^���̂��)
	if(m_pDoc->GetHostMode() == HOST_REMOTE)
	{
		//�����[�g�̓}�j���A�����肪�Ȃ��̂ŁA�����̏����͂���Ȃ��͂��B
		if ( m_pMainFrame->MeasEnd() == IDCANCEL )
			return;
	}


	//�`�[�t�փ��b�Z�[�W�𓊂���
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		::PostMessage(l_hChiefDlg, WM_DATA_MEAS_END, 0, 0); //1������I������
	} else {
			AlarmIf_Set(ALID_OneSampledMeasurementFailed);
	}

	m_pDoc->SetProcessStatus(PROCESS_WAIT);

	//�V�[�P���X���胂�[�h�t���O�ݒ�
	m_pDoc->SetSeqMeasMode(SEQ_NOT_MEASURE); //���蒆�łȂ��ɐݒ�

	m_pDoc->SetReMeasurePointCnt(0);


	//�}�j���A�����胂�[�h�ݒ�
	m_pDoc->SetManuMeasMode(MANUMODE_MEAS_COMP); //����I���ɐݒ�

	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_COMP);

}

// =========================================================================
//
void CManualMeasurementFormView::OnPointTeachingButton()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg, strMsg;
	// Kojika 20090529 Add End

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	//2009.10.30 bagus 2point-distance --{--
	//2�_�ԋ����̏ꍇ�ɂ̓|�C���g�e�B�[�`���O�͑��菈��
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
	//����������̏ꍇ�ɂ́A���菈�����s���B
	{
		RCP_DATA rcp_data;
		m_pDoc->GetRcpData(&rcp_data);
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
			OnMeasurementButton();
			return;
		}
	}
	//2009.11.12 bagus MS --}--

	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //�����e�i���X�E�X�C�b�`�E�I��
	}

	// �eI/O�`�F�b�N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- { -------- */
	if( m_pDoc->Rap_IsAirPressureLowON(1)!= OFF) {AlarmIf_Set(ALID_AirPressureDown); return;} /* �G�A���͒ቺ */
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������


	CAdditionalMeasurementDlg dlg;

	dlg.m_joyContinue = TRUE;					// JoyStick�̏�Ԃ��p��������

	// Kojika 20090529 Change
	//dlg.SetCaption(MEASUREMENT_CAPTION);
	//dlg.SetMessage(MEASUREMENT_MESSAGE);
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_CAPTION, l_strCap, "MANUAL MEASUREMENT - POINT TEACHING");
	LoadStringML(IDS_MANUAL_MEASUREMENT_POINT_TEACHING_MESSAGE, l_strMsg, "MANUAL MEASUREMENT - POINT TEACHING");
	dlg.SetCaption(l_strCap);
	dlg.SetMessage(l_strMsg);
	// Kojika 20090529 Change End
// 2009.10.30 bagus Stage-Sample �C�� --{--
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	BOOL bMain,bStage;

	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bStage = RecipeFile_LoadRecipe(&rcp_data.StageProgInfoHdr, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);

	if((bMain == FALSE) || (bStage == FALSE)){
		//���V�s���ǂ߂Ȃ������̂ŃT���v���\���͂Ȃ�
		dlg.m_nSampleMode = 0;
	}else{
		dlg.m_nSampleMode = 1;
	}

	strcpy(dlg.m_szRecipeName, rcp_data.StageProgInfoHdr.SampleInfo.szName);
// 2009.10.30 bagus Stage-Sample �C�� --}--
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
	// H/W�L���Ȃ�
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

	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
	}

/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
	BOOL l_bRc = SelectHeadTypeNotify(HEAD_TYPE_SR, FALSE);		/* SR �w�b�h�ɖ߂��܂��D	*/
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	/* SR �t�B���^�� OPEN ���܂��D	*/
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */

	//ActuateFlag�`�F�b�N
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

	if ( m_pMainFrame->MeasEnd() == IDCANCEL )
		return;


	//2009.11.04 bagus CA --{--
	//�R���^�N�g�A���O������̏ꍇ�ɂ́A�㏈�����K�v
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA){
		m_pDoc->CleanupManualCAMeasure();
	}
	//2009.11.04 bagus CA --}--

	//2009.12.25 bagus MS --{--
	//�I�����ɃV�����_�����S�ʒu�ɖ߂�
	if (rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		nexioMS_ModeLamp(FALSE);
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
		MSG msg;
		//��������������܂ő҂�
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
		//��������������܂ő҂�
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

//������ 090326 hibino add
	((CMainFrame*)AfxGetMainWnd())->ChangeOperationPanel(OP_MEASUREMENT);
	((CMainFrame*)AfxGetMainWnd())->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
//������ 090326 hibino add

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

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return 0L;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return 0L;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return 0L;} // MOVO�A���[�� 				���k
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
		}	// �X�e�[�W���쒆
		//Saiki 20090531 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

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
					// �ᑬ�E�����E����
					if ( StageIsIdle() ) {
						StageMoveAtSpeedEx(wAxis, (double)(MotionJog * sign)/MICROMETRE, FALSE);
						m_bIsMoveAtSpeed = TRUE;
						if ( lParam == 1 ) {
							// �L�[�{�[�h����́AWM_STAGE_MOVE_DONE�̎�M�Œ�~����
							return 0L;
						}
					}
				}else{
					// �W���O�i��j�E�W���O�i���j�E�W���O�i���j
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
				// �}�E�X����́A�����Œ�~������
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
			// H/W�����Ȃ�
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
			// H/W�����Ȃ�
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
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//void CManualMeasurementFormView::ManuMeas(void)
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
BOOL CManualMeasurementFormView::ManuMeas(void)
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
{
// Kojika 20090604 Add
	CString strBuffer, strTitle;
// Kojika 20090604 Add End

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
//	char* pszCaption = "MANUAL MEASUREMENT START";
	//char* pszCaption = "�蓮����J�n";
	CString l_strBuffer;
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	if(m_pDoc->GetStoreCount() >= SCAN_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
// Kojika 20090604 Change
//		sprintf(szMsgBuff, "Measurement data %d over!", SCAN_POINT_MAX);
//		sprintf(szMsgBuff, "����f�[�^�� %d �|�C���g�𒴂��Ă��܂�!", SCAN_POINT_MAX);
//		MessageBox(szMsgBuff, "NANO SPEC", MB_OK);
		LoadStringML(IDS_MANUAL_MEASURE_OVER, strBuffer, "Measurement data %d over!");
		sprintf(szMsgBuff, strBuffer, SCAN_POINT_MAX);
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
		MessageBox(szMsgBuff, strTitle, MB_OK);
// Kojika 20090604 Change End
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
	}


	///// �`�[�t�֑���J�n���b�Z�[�W�𑗐M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_ManualMeasurement);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
	}

	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		//Saiki 20090411 Add ----->
		///// �����Y�ؑ� /////
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
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--

/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT/*WM_DISP_START_SEQ*/, (WPARAM)rcp_data.szRecipeName, 0); //�ʏ푪��
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ----------			   */
	int l_iMeasPos;
	if ((HEAD_TYPE_SR == rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)) {
		/* GTR �̏ꍇ*/
		TCHAR l_tszGTRpos[32];
		this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
		l_iMeasPos = _ttol(l_tszGTRpos);

		//2010.01.29 bagus GTR �C�� --{--
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
		//2010.01.29 bagus GTR �C�� --}--

	} else {
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
//		l_iMeasPos = m_pDoc->GetStoreCount() + 1;
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ----------              */
		if (0 == m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoGet()) {
			/* �ʏ�(�V�K)����	*/
			l_iMeasPos = m_pDoc->GetStoreCount() + 1;
		} else {
			/* �đ���̎w��	*/
			l_iMeasPos = m_pMainFrame->m_pMeaTabViewObj->MRCS_ReMeasureNoGet();
		}
/* modified 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
	}
	if (0 == l_iMeasPos) {
		ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
// 2010.01.29 bagus Gantry --{--
		LoadStringML(IDS_MANUAL_MEASURE_SELECT_POINT, strBuffer, "Select measurement point.");
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NANO SPEC");
		MessageBox(strBuffer, strTitle, MB_OK);
// 2010.01.29 bagus Gantry --}--
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//		 return;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
	}
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(l_iMeasPos, 0));
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ---------- } ---------- */

	///// �`�[�t�̌��� /////
	// Kojika 20090529 Change
	LoadStringML(IDS_MANUAL_MEASUREMENT_START, l_strBuffer, "MANUAL MEASUREMENT START");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - MANUAL MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//		return;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
		return FALSE;
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
	}

/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	return TRUE;
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
}


// ==========================================================================
// ���t�@�����X�擾�A�J�n���Ԏ擾�Ȃ�
BOOL CManualMeasurementFormView::GetReferenceManuStart(void)
{

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
//	char* pszCaption = "MANUAL MEASUREMENT REFERENCE START";
//	char* pszCaption = "�蓮���t�@�����X����J�n";
	CString l_strBuffer;
	// Kojika 20090529 Change End

	SYSTEMTIME syStartDateTime; 	//����J�n����


	m_pDoc->GetRcpData(&rcp_data);


	///// ����J�n�����擾 /////
	GetLocalTime(&syStartDateTime);
	m_pDoc->SetStartDateTime(&syStartDateTime);

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		///// Clear Reference /////
		ClearReference(rcp_data.szRecipeName);
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		return FALSE;
	}

	// �{�^����ύX
	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// �`�[�t�̌��� /////
	// Kojika 20090529 Change
	LoadStringML(IDS_MANUAL_MEASUREMENT_REF_START, l_strBuffer, "MANUAL MEASUREMENT REFERENCE START");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		// �{�^����߂�
//		ButtonEnableChange(MANU_MEAS_FORM_CREATE);
		return FALSE;
	}


//	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
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
//		dlg.SetMessage("���t�@�����X ���蒆...");
		LoadStringML(IDS_REFER_EXECUTE_MEAASGE , l_strBuffer, "Reference Executing...");
		dlg.SetMessage(l_strBuffer);
		// Kojika 20090529 Change End

		dlg.DoModal();
// 2013.11.07 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.07 Bagus Add (TohoSpec�Ή�) <--

	return TRUE;
}


// ==========================================================================
// ���j���[�{�^������̊֐��R�[��
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

	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������

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

	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
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
				Invalidate(TRUE); //OnCtlColor()�̏�����������ׂ�Call
			}
			m_bNexMntSwOld = sbNexMntSw; //�O��l������
			m_bNexEngiMntSwOld = sbNexEngiMntSw; //�O��l������

			break;

		case MANUOPE_BUTTON_ENABLE_TIMER:
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ -->
			JoyStickEnableCheck();
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ <--
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* �h�A�C���^�[���b�N			���g */
				if (0 != m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
					/* XY �X�e�[�W���쒆�ł��D	*/
					StageStop();
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
				}
			}
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			break;
//2009.11.12 bagus MS --{--
		case JOG_WATCH_TIMER_ID:	//JOG Up/Down Switch
//2009.12.15 bagus MS --{--
//			if (m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
			if (m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) || m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)
			//2009.12.25 bagus ���蒆�łȂ���Ύ~�߂� --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			) {
//2009.12.15 bagus MS --}--
				//���쒆�Ȃ̂œ��������Ȃ�
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
							//��������
						}else if(nexioIsMS_UpSwitch() && nexioIsMS_UpperPos()){
							if(nexifRC_JogMinus(this->m_hWnd)){
								m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
							}
						}else if(nexioIsMS_DownSwitch() ){
							if(!nexioIsMS_LowerPos1() && !nexioIsMS_LensKind()){
								//���~�[�P�Ń����Y��ʂ�1-50x
							}else if(!nexioIsMS_LowerPos2()){
								//��ԉ�
							}else{
								//�㏸
								if(nexifRC_JogPlus(this->m_hWnd)){
									m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);
								}
							}
						}else{
							//�{�^���������ꂽ�̂ŃW���O���~�߂�
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
		case JOG_MODE_LAMP_TIMER_ID:	//MODE CHANGE�{�^���̓_��/�_��/����
			if(m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) || m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)
			//2009.12.25 bagus ���蒆�łȂ���Ώ��� --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			){
				//���쒆�͏���
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
					//���背�V�s���������ł͂Ȃ��̂�
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
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
//void CManualMeasurementFormView::OnJoyStick()
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ----------			   */
LRESULT CManualMeasurementFormView::OnJoyStick(WPARAM wparam, LPARAM lparam)
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- } ---------- */
{
	// �W���C�X�e�B�b�N�L���^����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			m_joyStick1->UnChecked();
			m_joyStick2->Checked();
			// �W���C�X�e�B�b�N����
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
			m_joyStick1->Checked();
			m_joyStick2->UnChecked();
			// �W���C�X�e�B�b�N�֎~
			m_XY_UpButton.SetEnabled(TRUE);
			m_XY_DownButton.SetEnabled(TRUE);
			m_XY_RightButton.SetEnabled(TRUE);
			m_XY_LeftButton.SetEnabled(TRUE);
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
/* added 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
	return 0;
/* added 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- } ---------- */
}

// ==========================================================================
//���蒆�̑���I�� ���̃{�^���L��/�����̐ؑւ� (�}�j���A�����[�h��p)
void CManualMeasurementFormView::ButtonEnableChange(int Select)
{

	//�����[�g���͑����Ԃɂ�����炸�A�{�^������������������
	if(m_pDoc->GetHostMode() == HOST_REMOTE) //�����[�g��
	{
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A������(�������[�g���ɂ̓}�j���A������͑��݂��Ȃ����ꉞ��`)
		{
			//IDD_MAIN_MENU_FORM���R���g���[���̖�����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE); //[LOG ON]�{�^������

			//IDD_MANUAL_MEASUREMENT_OPERATION_FORM���R���g���[���̖�����
			GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]�{�^������
			GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]�{�^������
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
				GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]�{�^������
				GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODE�I�𖳌�
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
			}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]�{�^������

			//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//�����Y�{���ύX����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]�{�^������
			//Saiki 20090410 Add ----->
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�w�b�h�֖ؑ���
			//Saiki 20090410 Add <-----

			//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
			m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
			m_pMainFrame->m_ManualMeasMenuEnableFlg = FALSE;

			//�\�t�g�W���C�X�e�B�b�N�̖�����
//			SoftJoyStickXyEnable(FALSE);
			JoyStickXyEnable(FALSE);
		}
			return;
	}



	switch(Select){

	case MEASUREMENT_MANUALMEASURE_S:

		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM���R���g���[���̖�����
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]�{�^������
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]�{�^������
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]�{�^������
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODE�I�𖳌�
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]�{�^������

		//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//�����Y�{���ύX����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]�{�^������
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�w�b�h�֖ؑ���
		//Saiki 20090410 Add <-----
		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = FALSE;

		//�\�t�g�W���C�X�e�B�b�N�̖�����
//		SoftJoyStickXyEnable(FALSE);
		JoyStickXyEnable(FALSE);

		break;



	case MEASUREMENT_MANUALMEASURE_E:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM���R���g���[���̗L����
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(TRUE); 		//[MEASUREMENT]�{�^���L��
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(TRUE);//[MEASUREMENT COMPLETE]�{�^���L��
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(TRUE);		//[POINT TEACHING]�{�^���L��
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);			//MOTION MODE�I��L��
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_pDoc->GetManuMeasMode() == MANUMODE_INIT)
		{
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]�{�^���L��
		}

		//IDD_VIEW_WINDOW_FORM���R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//�����Y�{���ύX�L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]�{�^���L��
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//�w�b�h�ؑ֗L��
		//Saiki 20090410 Add <-----
		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = TRUE;

		//2009.12.23 bagus MS --{--
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(TRUE); 			//MOTION MODE�I�𖳌�
		//2009.12.23 bagus MS --}--

		//�\�t�g�W���C�X�e�B�b�N�̗L����
//		SoftJoyStickXyEnable(TRUE);
		JoyStickXyEnable(TRUE);

		break;


	case MEASUREMENT_MANUALMEASURE_COMP:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM���R���g���[���̖�����
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]�{�^������
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]�{�^������
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(FALSE); 		//[POINT TEACHING]�{�^������
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 			//MOTION MODE�I�𖳌�
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]�{�^���L��

		//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//�����Y�{���ύX����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]�{�^������
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�w�b�h�֖ؑ���
		//Saiki 20090410 Add <-----

		//2009.12.23 bagus MS --{--
		GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION)->EnableWindow(FALSE); 			//MOTION MODE�I�𖳌�
		//2009.12.23 bagus MS --}--

		//�\�t�g�W���C�X�e�B�b�N�̖�����
//		SoftJoyStickXyEnable(FALSE);
		JoyStickXyEnable(FALSE);

		break;


	case MANU_MEAS_FORM_CREATE:
		//IDD_MAIN_MENU_FORM���R���g���[���̖�����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(FALSE);		//[MEASUREMENT]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(FALSE);	//[RECIPE]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE);	//[LOG ON]�{�^������

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A������
		{
			//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�J�����I�𖳌�
		}


		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;


	case MANU_MEAS_FORM_DELETE:
		//IDD_MAIN_MENU_FORM���R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//[MEASUREMENT]�{�^���L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(TRUE);//[RECIPE]�{�^���L��
		if(m_pDoc->GetHostMode() != HOST_REMOTE) //���[�J����
		{
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(TRUE);	//[LOG ON]�{�^���L��
		}

		//IDD_VIEW_WINDOW_FORM���R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//�����Y�{���ύX�L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]�{�^���L��
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�w�b�h�֖ؑ���
		//Saiki 20090410 Add <-----


		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̗L�����t���O����
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;

// 2009.10.30 bagus 2�_�� �C�� --{--
	case MEASUREMENT_MANUAL_DISTANCE_S:
	case MEASUREMENT_MANUAL_DISTANCE_E:
	case MEASUREMENT_MANUAL_DISTANCE_COMP:
		//IDD_MANUAL_MEASUREMENT_OPERATION_FORM���R���g���[���̗L����
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE); 		//[MEASUREMENT]�{�^���L��
		GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(TRUE);//[MEASUREMENT COMPLETE]�{�^���L��
		if(m_pDoc->GetManuMeasMode() == MANUMODE_INIT)
		{
			GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);			//[EXIT]�{�^���L��
		}
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			GetDlgItem(IDC_POINT_TEACHING_BUTTON)->EnableWindow(TRUE);		//[POINT TEACHING]�{�^���L��
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);			//MOTION MODE�I��L��

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
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

		//IDD_VIEW_WINDOW_FORM���R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//�����Y�{���ύX�L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]�{�^���L��
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//�w�b�h�ؑ֗L��
		//Saiki 20090410 Add <-----
		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;
		m_pMainFrame->m_ManualMeasMenuEnableFlg = TRUE;

		//�\�t�g�W���C�X�e�B�b�N�̗L����
//		SoftJoyStickXyEnable(TRUE);
		JoyStickXyEnable(TRUE);

		break;
// 2009.10.30 bagus 2�_�� �C�� --}--
	}

}

// =========================================================================
//
void CManualMeasurementFormView::JoyStickXyEnable(BOOL bEnable)
{
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// �n�[�h�W���C
			m_pDoc->ChangeJoyStick(bEnable);

			//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
		}
		else{
			// �\�t�g�W���C
			m_XY_UpButton.SetEnabled(bEnable);
			m_XY_DownButton.SetEnabled(bEnable);
			m_XY_RightButton.SetEnabled(bEnable);
			m_XY_LeftButton.SetEnabled(bEnable);

			//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
}

#if 0
// =========================================================================
//
void CManualMeasurementFormView::SoftJoyStickXyEnable(BOOL bEnable)
{
	if( m_pMainFrame->GetJoyStickMode() != 0 ) return;
	if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ) return;

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		//SoftJoyStick
		m_XY_UpButton.SetEnabled(bEnable);
		m_XY_DownButton.SetEnabled(bEnable);
		m_XY_RightButton.SetEnabled(bEnable);
		m_XY_LeftButton.SetEnabled(bEnable);
//		m_Z_UpButton.SetEnabled(bEnable);
//		m_Z_DownButton.SetEnabled(bEnable);

		//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
		GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
		GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
}
#endif

// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ -->
// =========================================================================
//
void CManualMeasurementFormView::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* �h�A�C���^�[���b�N			���g */
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			if ( m_joyStick2->GetCheck() ) {
				// �����I�Ƀ\�t�gJoy�֐؂�ւ���
				m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
				m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				m_joyStick1->Checked();
				m_joyStick2->UnChecked();
				m_joyStick1->SetFocus();
			}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	}
}
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ <--

void CManualMeasurementFormView::OnSelchangeComboMeasPoint()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
	CString strMsg;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVO�A���[�� 				���k
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ���C���^�[���b�N������
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	//�X�e�[�W�R���t�B�O�̃��[�h
	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	//���t�@�����X�|�W�V�����̃��[�h
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
		/* GTR �̏ꍇ*/
		TCHAR l_tszGTRpos[32];
		this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
		iSelectMeasPt = _ttol(l_tszGTRpos);
	} else {
		iSelectMeasPt = pCombo->GetCurSel();
	}
#endif
// 2010.01.29 bagus Gantry --}--
	if(0 < iSelectMeasPt && iSelectMeasPt <= SrConfig.wGantryNumOfMeasPoint){
		//XY���W�̕\����ύX
		SetPointData(SrReferencePosition, iSelectMeasPt);
		//�J�[�\���ԍ��ɑΉ������X�e�[�W�ʒu�ֈړ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
		sampleCoord.lX = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX/* + m_StageProgInfoHdr.SampleInfo.Origin.lX*/;
		sampleCoord.lY = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY/* + m_StageProgInfoHdr.SampleInfo.Origin.lY*/;
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		//�{�^�����̖�����
	}
}
// ==========================================================================
// Name�F		SetPointData
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

// 2009.10.30 bagus MS �ǉ� --{--
// =========================================================================
//�����Y �R���{�C�j�V��������
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
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.02 bagus MS �ǉ� --}--
void CManualMeasurementFormView::OnSelendokMicroScopeFocusPosition()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

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

// 2009.11.09 bagus MS �C�� --{--
	if ( CCursor::m_bWaitCursor ) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
// 2009.11.09 bagus MS �C�� --{--
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
// 2009.11.09 bagus MS �C�� --{--
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
// 2009.11.09 bagus MS �C�� --}--
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
	}

	// �eI/O�`�F�b�N
// 2009.11.09 bagus MS �C�� --{--
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
// 2009.11.09 bagus MS �C�� --}--

	//ActuateFlag�`�F�b�N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
// 2009.11.09 bagus MS �C�� --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS �C�� --}--
	} //�X�e�[�W�����쒆�ł�
// 2009.11.09 bagus MS �C�� --{--
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--
	// ���C���^�[���b�N������

// 2009.11.09 bagus MS �C�� --{--
#if 0
	// ActuateFlag���Z�b�g����
	m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

	// ���̈ړ����͂̎�t���֎~����
	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	// Z�����싖�����v OFF
	nexioMS_ModeLamp(FALSE);

	// �ʒu����
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

	// �ړ������҂�

	// Z�����싖�����v ON
	nexioMS_ModeLamp(TRUE);

	// ���̈ړ����͂̎�t��������
	{	// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
	}

	// ActuateFlag��߂�
	m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
#else
	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
// matsuhisa 2009.12.26 deleted >>>
//		if (m_MsConfig.RoboCylinder[nItemData].bSensor == nexioIsMS_LensKind()) {
// matsuhisa 2009.12.26 deleted <<<
			// ���̈ړ����͂̎�t���֎~����
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
				MessageBox(strMsg, strTitle, MB_OK);
				// �K��JoyStick�̏�Ԃ�߂�����
				m_pDoc->JoyStickStatusRestore();
				pCombo->SetCurSel(m_nSelLens);
				return;
			}

			// ActuateFlag���Z�b�g����
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);

			// Mode�؂�ւ�
			unsigned char oldMode = nexioRC_GetCurrentMode();
			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
				MSG 	msg;

				//��������������܂ő҂�
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

			// �ʒu����
			if(nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo)){
				// ActuateFlag���Z�b�g����
				m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
				MSG 	msg;

				//��������������܂ő҂�
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

				//��������������܂ő҂�
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
//			// �������̃����Y�̐ݒ肪��v���Ȃ�
//			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
//			m_pDoc->MessageStringIf_Set(strMsg);
//			pCombo->SetCurSel(m_nSelLens);
//		}
// matsuhisa 2009.12.26 deleted <<<
	}else{
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS �C�� --}--
}
// 2009.11.02 bagus MS �ǉ� --}--

// 2009.11.09 bagus MS �C�� --{--
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
			// �A���[��
//		}
		break;
	default:
		break;
	}
	return 0;
}
// 2009.11.09 bagus MS �C�� --}--
