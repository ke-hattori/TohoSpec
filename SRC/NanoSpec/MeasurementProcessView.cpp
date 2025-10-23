// MeasurementProcessView.cpp : �C���v�������e�[�V���� �t�@�C��
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
// 2009.10.30 bagus MS �ǉ� --{--
#include "OverlayDialog.h"
#include "PointSetupTeachingMSDlg.h"
// 2009.10.30 bagus MS �ǉ� --}--
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

// 2009.10.30 bagus MS �ǉ� --{--
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.09 bagus MS �C�� --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS �C�� --}--
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
// 2009.10.30 bagus MS �ǉ� --{--
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
// 2009.10.30 bagus MS �ǉ� --}--
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
// 2009.10.30 bagus MS �ǉ� --{--
	ON_MESSAGE(WM_DISPMENU_POINT_TEACHING, OnMeaProcFuncCall)
	ON_MESSAGE(WM_JOYSTICK, OnJoyStick)
// 2009.10.30 bagus MS �ǉ� --}--
// 2009.11.09 bagus MS �C�� --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS �C�� --}--
END_MESSAGE_MAP()

// =========================================================================
// CMeasurementProcessView �f�f

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
// CMeasurementProcessView ���b�Z�[�W �n���h��

// =========================================================================
//
int CMeasurementProcessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNanoUI::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������

	return 0;
}

// =========================================================================
//
void CMeasurementProcessView::OnInitialUpdate()
{
	//���Ή��@�\HIDE
	GetDlgItem(IDC_VALUES_TO_DISPLAY_BUTTON)->ShowWindow(SW_HIDE);

// 2009.10.30 bagus MS �ǉ� --{--
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// ��ʂ��\������鎞�̓f�t�H���g�̓\�t�gJoy�Ƃ���
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

	// Radio�{�^���̐ݒ�
	m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
	m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.02 bagus MS �ǉ� --{--
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
// 2009.11.02 bagus MS �ǉ� --}--

	CNanoUI::OnInitialUpdate();

	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

// 2009.10.30 bagus MS �ǉ� --{--
//	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
// 2009.10.30 bagus MS �ǉ� --}--
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

// 2009.10.30 bagus MS �ǉ� --{--
	/// Point Teaching Button ///
	m_PointTeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PointTeachingButton.DrawFlatFocus(TRUE);

	/// Next Point Button ///
	m_NextPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NextPointButton.DrawFlatFocus(TRUE);
// 2009.10.30 bagus MS �ǉ� --}--

	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
// 2009.10.30 bagus MS �ǉ� --{--
	///// Lens Combo /////
//	RecipeFile_LoadRecipe(&l_rcp_data.MainRcpInfo, l_rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);

// 2009.12.17 bagus MS �ǉ� --}--
	if (HEAD_TYPE_MS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		ChangeOperationEnable(TRUE);
	}
// 2009.12.17 bagus MS �ǉ� --}--

	if (l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		Init_SR_XYStageMenu();
		InitCombo_Lens();

		CRect rect;
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->GetWindowRect(rect);
		ScreenToClient(rect);
		GetDlgItem(IDC_EXIT_BUTTON)->MoveWindow(rect);

// 2009.11.02 bagus MS �ǉ� --{--
//		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->ShowWindow(SW_HIDE);
// 2009.11.02 bagus MS �ǉ� --}--
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

		//�N�����ɂ̓W���O���[�h�ɂ��Ă���
		nexifRC_SelectMode(this->m_hWnd, RCOPMODE_JOG);
	} else {
// 2009.11.02 bagus MS �ǉ� --{--
		GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->ShowWindow(SW_SHOW);
// 2009.11.02 bagus MS �ǉ� --}--
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
	//�đ���A�ʒu����{�^���͔�\���ɂ���
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		GetDlgItem(IDC_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
	}
	//2009.09.13 bagus stress --}--
	//2009.10.28 bagus 2point-distance --{--
	//�ʒu����{�^���͔�\���ɂ���
	else if(HEAD_TYPE_SR == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType
	&& MEAS_PROG_TYPE_SR_DISTANCE == l_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType
	){
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
	}
	//2009.10.28 bagus 2point-distance --}--
	//2009.12.07 bagus SR GTR �C�� --{--
	//�ʒu����{�^���͔�\���ɂ���
	else if(HEAD_TYPE_SR == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType
	&& MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_rcp_data.MeasProgInfo.ScanParams.hdr.wScanType
	){
		GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
	}
	//2009.12.07 bagus SR GTR �C�� --}--


	// �W���C�X�e�B�b�N�L���^����
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
// 2009.10.30 bagus MS �ǉ� --}--

	SetTimer(1, 50, NULL);
// 2009.11.09 bagus MS �C�� --{--
	SetTimer(JOG_WATCH_TIMER_ID, JOG_WATCH_TIMER_INTERVAL, NULL);
	SetTimer(JOG_MODE_LAMP_TIMER_ID, JOG_MODE_LAMP_TIMER_INTERVAL, NULL);
// 2009.11.09 bagus MS �C�� --}--
}

// 2009.10.30 bagus MS �ǉ� --{--
void CMeasurementProcessView::OnDestroy()
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

	//�}�j���A�����胂�[�h�ݒ�
	m_pDoc->SetManuMeasMode(MANUMODE_NOT); //�}�j���A�����[�h�N������(���t�@�����X���蒆�܂�)(�������������Ă��Ȃ����)

//	ButtonEnableChange(MANU_MEAS_FORM_DELETE);

	CNanoUI::OnDestroy();
}
// 2009.10.30 bagus MS �ǉ� --}--

// =========================================================================
//
void CMeasurementProcessView::OnMeasurementCancelButton()
{
		AfxGetMainWnd()->PostMessage(WM_DO_CANCEL, 0, 0);

// 2009.12.17 bagus MS �ǉ� --}--
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);

	if (HEAD_TYPE_MS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		ChangeOperationEnable(FALSE);
	}
// 2009.12.17 bagus MS �ǉ� --}--
}

// =========================================================================
//
void CMeasurementProcessView::OnMeasurementPauseButton()
{
	CString strBuff, strMsg, l_strBuffer;
	GetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, strBuff);
	// Kojika 20090601 Change
//	if ( strBuff == "PAUSE" )
	//if ( strBuff == "���蒆�f" )
	LoadStringML(IDS_MEASUREMENT_PAUSE, l_strBuffer, "PAUSE");
	if ( strBuff == l_strBuffer )
	{
	// Kojika 20090601 Change End

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
	// ���C���^�[���b�N������

		AfxGetMainWnd()->PostMessage(WM_DO_PAUSE, 0, 0);
		// Kojika 20090601 Change
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "RESUME");
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "����ĊJ");
		LoadStringML(IDS_MEASUREMENT_RESUME, l_strBuffer, "RESUME");
		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, l_strBuffer);
		// Kojika 20090601 Change End

		m_pDoc->SetPauseFlg(TRUE); //�t���O���|�[�Y��Ԃɐݒ�
	}
	else
	{
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
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* �G�A���͒ቺ */
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
/* modified 2009.08.21 hmenjo Pin Aling �ʒu�L���� ---------- { ---------- */
//	  if(m_pDoc->CheckUnitStatus() == TRUE){return;}
/* modified 2009.08.21 hmenjo Pin Aling �ʒu�L���� ----------			   */
	BOOL l_bEnablePinAlign = FALSE;
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		l_bEnablePinAlign = TRUE;
	}
	if (m_pDoc->CheckUnitStatus(l_bEnablePinAlign) == TRUE) {return;}
/* modified 2009.08.21 hmenjo Pin Aling �ʒu�L���� ---------- } ---------- */
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


		AfxGetMainWnd()->PostMessage(WM_DO_RESUME, 0, 0);
		// Kojika 20090601 Change
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "PAUSE");
//		SetDlgItemText(IDC_MEASUREMENT_PAUSE_BUTTON, "���蒆�f");
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



//������MeasEnd()�ֈړ�

	if ( ((CMainFrame*)AfxGetMainWnd())->MeasEnd() == IDCANCEL )
		return;

	//2009.11.11 bagus MS --{--
	//�I�����Ɍ��ɖ߂�
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
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
	//2009.11.11 bagus MS --}--

//������ 090326 hibino add //
	((CMainFrame*)AfxGetMainWnd())->ChangeOperationPanel(OP_MEASUREMENT);
	((CMainFrame*)AfxGetMainWnd())->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
//������ 090326 hibino add

}

void CMeasurementProcessView::OnRemeasurementButton()
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
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //�����e�i���X�E�X�C�b�`�E�I��
	}

	// �eI/O�`�F�b�N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* �G�A���͒ቺ */
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

	//2009.09.10 bagus stress --{--
	//�X�g���X�̏ꍇ�ɂ͍đ�����s�킹�Ȃ�
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {

		LoadStringML(IDS_REMEAS_STRESS_ERR, strMsg, "Re-Measurement is not supportted for STRESS Recipe.");
		MessageBox(strMsg, "", MB_OK | MB_ICONWARNING); //�Z�[�u����
		return ;
	}
	//2009.09.10 bagus stress --}--

	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������



	BOOL bRtnChk = StartRemeasurement(1);	//PMA����

	if(bRtnChk == TRUE)
	{
		m_pDoc->OperationLogging("Measurement Operation - REMEASUREMENT BUTTON - Measurement was initiated.");
	}

}


void CMeasurementProcessView::OnAdujstRemeasurementButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //�����e�i���X�E�X�C�b�`�E�I��
	}

	// �eI/O�`�F�b�N
	/*Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- { -------- */
	if (m_pDoc->Rap_IsAirPressureLowON(1) != OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	/* �G�A���͒ቺ */
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
	//2009.09.10 bagus stress --{--
	//�X�g���X�̏ꍇ�ɂ͍đ�����s�킹�Ȃ�
	RCP_DATA l_rcp_data;
	m_pDoc->GetRcpData(&l_rcp_data);
	if (HEAD_TYPE_STRESS == l_rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType) {
		LoadStringML(IDS_REMEAS_STRESS_ERR, strMsg, "Re-Measurement is not supportted for STRESS Recipe.");
		MessageBox(strMsg, "", MB_OK | MB_ICONWARNING); //�Z�[�u����
		return ;
	}
	//2009.09.10 bagus stress --}--
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������


	BOOL bRtnChk = StartRemeasurement(2);	//PMA�L��(�S�_)

	if(bRtnChk == TRUE)
	{
		m_pDoc->OperationLogging("Measurement Operation - ADUJST REMEASUREMENT BUTTON - Measurement was initiated.");
	}

}


void CMeasurementProcessView::OnSaveButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

// Kojika 20090526 Add
	CString strBuffer;
// Kojika 20090526 Add End

//�����̃Z�[�u�{�^���̓V���[�v�d�l�ł͖��g�p

	BOOL bCheck;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	bCheck = m_pMainFrame->m_pMeaTabViewObj->SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_EXIT_SAVE/*�Ⴄ*/);
	if(bCheck == TRUE)
	{
// Kojika 20090526 Change
//		MessageBox("Save Completed.", "", MB_OK); //�Z�[�u����
//		MessageBox("�ۑ�����", "", MB_OK); //�Z�[�u����
		LoadStringML(IDS_SAVE_COMPLETED, strBuffer, "Save Completed.");
		MessageBox(strBuffer, "", MB_OK); //�Z�[�u����
// Kojika 20090526 Change End
	}
	else
	{
// Kojika 20090526 Change
//		MessageBox("Save failure.", "", MB_OK); //�Z�[�u���s
//		MessageBox("�ۑ����s", "", MB_OK); //�Z�[�u���s
		LoadStringML(IDS_SAVE_FAILURE, strBuffer, "Save failure.");
		MessageBox(strBuffer, "", MB_OK); //�Z�[�u���s
// Kojika 20090526 Change End
	}

}


//���j���[�{�^������̊֐��R�[��
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
// 2009.10.30 bagus MS �ǉ� --{--
		case WM_DISPMENU_POINT_TEACHING:
			OnPointTeachingButton();
			break;
// 2009.10.30 bagus MS �ǉ� --}--

	}
	return 0L;
}


HBRUSH CMeasurementProcessView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������

	BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if( pWnd == this ) // �������g��`�悷��Ƃ���
	{
		if((sbNexMntSw == OFF)||(sbNexEngiMntSw == ON))
		{
			hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
		}
	}

	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	return hbr;
}

void CMeasurementProcessView::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	static BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	static BOOL sbNexMntSwOld = TRUE;
	static BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();
	static BOOL sbNexEngiMntSwOld = FALSE;

	sbNexMntSw = nexioIsMaintenanceSwitch();
	sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if((sbNexMntSw != sbNexMntSwOld)||(sbNexEngiMntSw != sbNexEngiMntSwOld))
	{
		Invalidate(TRUE); //OnCtlColor()�̏�����������ׂ�Call
	}
	sbNexMntSwOld = sbNexMntSw; //�O��l������
	sbNexEngiMntSwOld = sbNexEngiMntSw; //�O��l������

// 2009.11.09 bagus MS �C�� --{--
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	//���������L���łȂ��Ƃ��ɂ͂Ȃɂ������Ȃ�
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		switch ( nIDEvent ) {
		//2009.11.16 bagus MS --{--
		//case JOG_WATCH_TIMER_INTERVAL:		//Up/Down Switch ����
		case JOG_WATCH_TIMER_ID:
		//2009.11.16 bagus MS --}--
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

			break;
		case JOG_MODE_LAMP_TIMER_ID:	//MODE CHANGE�����v�̓_��/�_��/����
			if(m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE) || m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)
			//2009.12.25 bagus ���蒆�łȂ���Ώ��� --{--
			|| m_pDoc->GetProcessStatus() != PROCESS_PROC
			//2009.12.25 bagus --}--
			){
				//���쒆�͏���
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
// 2009.11.09 bagus MS �C�� --}--

	CNanoUI::OnTimer(nIDEvent);
}


BOOL CMeasurementProcessView::StartRemeasurement(int iPma)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	//���ݑI������Ă��郁�C�����V�s�����[�h
	CView* pView;

	pView = (CView*)((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter2.GetPane(0,0);
	int iIndex = m_pMainFrame->m_pMeaTabViewObj->m_MeasurementTab.GetCurSel();
	CGridCtrl* pGrid;
	if(iIndex == 0) //List���\������Ă�����
	{
		pGrid = &(((CMeasurementTabView *)pView)->m_pMeasurementListDlg->m_MeasurementListGrid);
	}
	else if(iIndex == 1) //Graph���\������Ă�����
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
//	  char* pszCaption = "�đ���J�n";
	RCP_DATA rcp_data;
	int iLastMeasPoint=0;
	int iReMeasNumScans = 0; //�����W���[���̑���|�C���g��(���|�C���g���肷�邩�̐�)

	m_pDoc->GetRcpData(&rcp_data);

	///// �`�[�t�֑���J�n���b�Z�[�W�𑗐M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_ReMeasurement);
		return FALSE;
	}

	memset(m_pDoc->m_bDoPointMeasFlag, FALSE, sizeof(m_pDoc->m_bDoPointMeasFlag));
	//2009.11.11 bagus 2point-distance --{--
	//2�_�ԋ�������̏ꍇ�́A�|�C���g�����Ⴄ
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		for(int i=0; i</*rcp_data.StageProgInfoHdr.wNumScans*/m_pDoc->GetStoreCount(); i++)
		{
			int iFocus = (pGrid->GetItemState(i+1, 1) & /*GVIS_FOCUSED*//*GVIS_DROPHILITED*/GVIS_SELECTED);
			if(iFocus != 0)
			{
				m_pDoc->m_bDoPointMeasFlag[i*2] = TRUE; 	//������s�t���O�𗧂Ă�
				m_pDoc->m_bDoPointMeasFlag[i*2+1] = TRUE; 	//������s�t���O�𗧂Ă�
				iLastMeasPoint = i*2 + 1; //�ŏI����|�C���g���X�V
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
				m_pDoc->m_bDoPointMeasFlag[i] = TRUE; //������s�t���O�𗧂Ă�
				iLastMeasPoint = i + 1; //�ŏI����|�C���g���X�V
				iReMeasNumScans++;
			}
		}
	//2009.11.11 bagus 2point-distance --{--
	}
	//2009.11.11 bagus 2point-distance --}--
	//Saiki 20090406 Add ----->
	if(iReMeasNumScans < 1){
// Kojika 20090526 Change
//		MessageBox("�đ���|�C���g���I������Ă��܂���", pszCaption, MB_OK);
		LoadStringML(IDS_REMEASUREING_POINT_NOT_SELECT, strBuffer1, "The remeasuring point has not been selected");
		LoadStringML(IDS_TITLE_REMESURE_START, strBuffer2, "REMEASUREMENT START");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change End
		return FALSE;
	}
	//Saiki 20090406 Add <-----
	m_pDoc->SetLastMeasPoint(iLastMeasPoint); //�ŏI����_�i�[
	TRACE("%s%d", "�sCMeasurementProcessView::OnRemeasurementButton�t iLastMeasPoint:", iLastMeasPoint);

	m_pDoc->SetReMeasNumScans(iReMeasNumScans);
	m_pDoc->SetShowCompPointList(0);
	m_pDoc->SetShowCompPointGraph(0);
	m_pDoc->SetCurrentSetPoint(0);

	m_pMainFrame->m_pMeaTabViewObj->m_pMeasurementListDlg->SetTimer(SHOW_LIST_TIMER, 500, NULL);
	m_pMainFrame->m_pMeaTabViewObj->m_pMeasurementGraphDlg->SetTimer(SHOW_GRAPH_TIMER, 500, NULL);

	LPARAM lparam = (LPARAM)MAKELONG(1, iPma);
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SEQ, (WPARAM)rcp_data.szRecipeName, /*1*/lparam); //�����W���[���[�h


	///// �`�[�t�̌��� /////
	//Saiki 20090527 Change ---->
	//bResult = pMainFrame->JudgeChiefResult(ChiefRet, pszCaption);
	LoadStringML(IDS_TITLE_REMESURE_START, strBuffer2, "REMEASUREMENT START");
	bResult = pMainFrame->JudgeChiefResult(ChiefRet, strBuffer2);
	//Saiki 20090527 Change <----

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - REMEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		return FALSE;
	}

	//�V�[�P���X���胂�[�h�t���O�ݒ�
	m_pDoc->SetSeqMeasMode(SEQ_ON_REMEASURE); //�����W���[���ɐݒ�


	//���胂�[�h�t���O�ݒ�
	m_pDoc->SetMeasMode(MEASMODE_SEQ_OR_REMEASURE); //�ʏ푪�蒆or�����W���[��


	return TRUE;
}

// 2009.10.30 bagus MS �ǉ� --{--
// =========================================================================
//�����Y �R���{�C�j�V��������
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
// 2009.10.29 bagus Jog �ǉ��C�� --{--
//	GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	if (m_SystemConfig.nJoystickType == 0) {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	}
// 2009.10.29 bagus Jog �ǉ��C�� --}--

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

// =========================================================================
//
void CMeasurementProcessView::OnPointTeachingButton()
{
	// Kojika 20090529 Add
	CString l_strCap, l_strMsg, strMsg;
	// Kojika 20090529 Add End

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

	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	if(rcp_data.StageProgInfoHdr.wNumScans == m_pDoc->GetStoreCount()) //���萔���\�葪��ɓ��B���Ă���
		return;

	m_pMainFrame->PostMessage(WM_CHIF_MS_POPUP,0,0);
#if 0
	CPointSetupTeachingMSDlg dlg;

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
#if 0
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
#endif
// 2009.10.30 bagus Stage-Sample �C�� --}--
	if(dlg.DoModal() == IDOK){


	}

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
#endif
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
//void CManualMeasurementFormView::OnJoyStick()
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ----------			   */
LRESULT CMeasurementProcessView::OnJoyStick(WPARAM wparam, LPARAM lparam)
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- } ---------- */
{
	// �W���C�X�e�B�b�N�L���^����
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
/* added 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
	return 0;
/* added 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- } ---------- */
}

// ==========================================================================
// Name�F		OnNextPointButton
void CMeasurementProcessView::OnNextPointButton()
{
	RCP_DATA rcp;

	m_pDoc->GetRcpData(&rcp);
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		//����������̏ꍇ�ɂ͂������̃{�^���͗L���łȂ��͂��B
		HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
		if(l_hChiefDlg != NULL){
			::PostMessage(l_hChiefDlg, WM_DISP_MS_POPUP_END, 1, 0);
		}
	}

}
// 2009.10.30 bagus MS �ǉ� --}--

// 2009.11.02 bagus MS �ǉ� --{--
void CMeasurementProcessView::OnSelendokMicroScopeFocusPosition()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

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
	//2009.12.25 bagus MS �C�� --{--
	//if (m_pDoc->IsInterLock() == TRUE) {
	if (m_pDoc->IsInterLockForMScope() == TRUE) {
	//2009.12.25 bagus MS �C�� --}--
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
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
// 2009.11.09 bagus MS �C�� --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS �C�� --}--
	}

	// Z�����싖�����v OFF
	nexioMS_ModeLamp(FALSE);

	// �ʒu����
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

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
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS �C�� --}--
}
// 2009.11.02 bagus MS �ǉ� --}--

// 2009.11.09 bagus MS �C�� --{--
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
			// �A���[��
//		}
		break;
	default:
		break;
	}
	return 0;
}
// 2009.11.09 bagus MS �C�� --}--

// 2009.12.17 bagus MS �C�� --{--
void CMeasurementProcessView::ChangeOperationEnable(BOOL bEnable)
{
	GetDlgItem(IDC_NEXT_POINT_BUTTON)->EnableWindow(bEnable);
//2009.12.23 bagus MS �C�� --{--
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
//2009.12.23 bagus MS �C�� --}--
	GetDlgItem(IDC_COMBO_MEAS_POINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bEnable);
	GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bEnable);

}
// 2009.12.17 bagus MS �C�� --}--
