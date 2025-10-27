// TestMeasurementDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MeasurementGraphDlg.h"
#include "MeasurementGraphDlg2.h"
#include "MainFrm.h"
#include "TestMeasurementDlg.h"
#include "ExecutionDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TESTMEASDLG_ENABLE_CHANGE_TIMER 1

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementDlg �_�C�A���O

CTestMeasurementDlg::CTestMeasurementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMeasurementDlg::IDD, pParent)
{
	m_strLens = "";
	m_strOpticalFilter = "";
	m_dIntegTime = 0;
// 2009.05.28 K.Matsuo SPT -->
	m_bIsMoveAtSpeed = FALSE;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.05.28 K.Matsuo SPT <--
}

void CTestMeasurementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMeasurementDlg)
	DDX_Control(pDX, IDC_MEASUREMENT_TAB, m_MeasurementTab);
	DDX_Control(pDX, IDC_AUTO_FOCUS_BUTTON, m_AutoFocusButton);
	DDX_Control(pDX, IDC_REFERENCE_BUTTON, m_ReferenceButton);
	DDX_Control(pDX, IDC_MEASUREMENT_BUTTON, m_MeasurementButton);
	DDX_Control(pDX, IDC_STAGE_UP_BUTTON, m_XY_UpButton);
	DDX_Control(pDX, IDC_STAGE_DOWN_BUTTON, m_XY_DownButton);
	DDX_Control(pDX, IDC_STAGE_RIGHT_BUTTON, m_XY_RightButton);
	DDX_Control(pDX, IDC_STAGE_LEFT_BUTTON, m_XY_LeftButton);
	DDX_Control(pDX, IDC_STAGE_MOTION_MODE, m_cbMotionStage);
	DDX_Control(pDX, IDC_ELEV_UP_BUTTON, m_Z_UpButton);
	//Saiki 20090908 Add ----->
	DDX_Control(pDX, IDC_H_POSITON_BUTTON, m_Z_HomePosButton);
	//Saiki 20090908 Add <-----
	DDX_Control(pDX, IDC_ELEV_DOWN_BUTTON, m_Z_DownButton);
	DDX_Control(pDX, IDC_ELEV_MOTION_MODE, m_cbMotionElev);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_AF_AND_MEASUREMENT_BUTTON, m_AfMeasurementButton);
	DDX_CBStringExact(pDX, IDC_SR_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_SR_OPT_FILTER, m_strOpticalFilter);
/* added 2015.03.10 hmenjo �e�X�g����ۑ� ---------- { ---------- */
	DDX_Control(pDX, IDC_BTN_SAVE, m_BtnSave);
/* added 2015.03.10 hmenjo �e�X�g����ۑ� ---------- } ---------- */
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MEAS_INTEGRATION_TIME, m_Meas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_WAVELENGTH_START, (short&)m_Meas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_WAVELENGTH_END,	(short&)m_Meas.ScanParams._SR.WavelenRange.wEnd);
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
	//Saiki 20090908 Add ----->
	DDX_Control(pDX, IDC_MEAS_POINT_X, m_stcMeasurementPointX);
	DDX_Control(pDX, IDC_MEAS_POINT_Y, m_stcMeasurementPointY);
	//Saiki 20090908 Add <-----
}


BEGIN_MESSAGE_MAP(CTestMeasurementDlg, CDialog)
	//{{AFX_MSG_MAP(CTestMeasurementDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MEASUREMENT_TAB, OnSelchangeMeasurementTab)
	ON_BN_CLICKED(IDC_AUTO_FOCUS_BUTTON, OnAutoFocusButton)
	ON_BN_CLICKED(IDC_REFERENCE_BUTTON, OnReferenceButton)
	ON_BN_CLICKED(IDC_MEASUREMENT_BUTTON, OnMeasurementButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AF_AND_MEASUREMENT_BUTTON, OnAfAndMeasurementButton)
	ON_EN_KILLFOCUS(IDC_WAVELENGTH_START, OnKillfocusWavelengthStart)
	ON_EN_KILLFOCUS(IDC_MEAS_INTEGRATION_TIME, OnKillfocusMeasIntegrationTime)
	ON_EN_KILLFOCUS(IDC_WAVELENGTH_END, OnKillfocusWavelengthEnd)
	ON_CBN_SETFOCUS(IDC_SR_LENS, OnSetfocusSrLens)
	ON_CBN_SETFOCUS(IDC_SR_OPT_FILTER, OnSetfocusSrOptFilter)
	ON_CBN_KILLFOCUS(IDC_SR_OPT_FILTER, OnKillfocusSrOptFilter)
	ON_EN_SETFOCUS(IDC_MEAS_INTEGRATION_TIME, OnSetfocusMeasIntegrationTime)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_START, OnSetfocusWavelengthStart)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_END, OnSetfocusWavelengthEnd)
	ON_CBN_KILLFOCUS(IDC_SR_LENS, OnKillfocusSrLens)
	ON_CBN_SELCHANGE(IDC_COMBO_MEAS_POINT, OnSelchangeComboMeasPoint)
	ON_CBN_SELCHANGE(IDC_SR_LENS, OnSelchangeSrLens)
	ON_CBN_SELCHANGE(IDC_SR_OPT_FILTER, OnSelchangeSrOptFilter)
	ON_BN_CLICKED(IDC_H_POSITON_BUTTON, OnHomePositonButton)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DO_STAGE_MOVE, OnDoStageMove)
	ON_MESSAGE(WM_STAGE_MOVE_DONE, OnStageMoveDone)
	ON_MESSAGE(WM_DO_ELEVATOR_MOVE, OnDoElevatorMove)
	ON_MESSAGE(WM_TESTMODE_ONEPOINT_END, OnTestModeOnePointEnd)
	ON_MESSAGE(WM_TESTMODE_REF_END, OnTestModeRefEnd)
	ON_MESSAGE(WM_JOYSTICK, OnJoyStick)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementDlg ���b�Z�[�W �n���h��

BOOL CTestMeasurementDlg::OnInitDialog()
{
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// ��ʂ��\������鎞�̓f�t�H���g�̓\�t�gJoy�Ƃ���
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		// Radio�{�^���̐ݒ�
		m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
		m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

// 2013.01.11 bagus jog direction -->
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_JogDirSetting, CONFIG_FILE_JOG_DIR);
// 2013.01.11 bagus jog direction <--

	//
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pTestMeasDlgObj = this;
	m_iConditionFlg = TESTMODE_INIT;	//�e�X�g���[�h�N������
	m_iConditionFlgOld = -1;
	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iProcessStatusOld = -1;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	LoadRecipeData();
	m_Meas.ScanParams._SR.dIntegTime = m_dIntegTime;
	m_Meas.ScanParams._SR.WavelenRange.wStart = m_wStart;
	m_Meas.ScanParams._SR.WavelenRange.wEnd = m_wEnd;

	CDialog::OnInitDialog();

//	m_strLens = m_SrTurret[m_Meas.ScanParams._SR.iLens].szName;

	//Saiki 20090908 Add ----->
	///// Get Recipe Data /////
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);
	//Saiki 20090908 Add <-----

	///// Control Initialize /////
	InitCombo_Lens();
	//Saiki 20090908 Change ----->
	//InitCombo_SrOpticalFilter();
	if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
	else{
		InitCombo_SrOpticalFilter();
	}
	//Saiki 20090908 Change <-----

// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//	if(g_lModelType == MODEL_T3100){
	if(!m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		m_AutoFocusButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		/// AutoFocus Button ///
		//Saiki 20090908 Change ----->
		//m_AutoFocusButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		//	BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		//m_AutoFocusButton.DrawFlatFocus(TRUE);
		if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
			GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->ShowWindow(SW_HIDE);
		}
		else{
			m_AutoFocusButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
				BUTTON_COLORING, BUTTON_NORMAL_COLOR);
			m_AutoFocusButton.DrawFlatFocus(TRUE);
		}
		//Saiki 20090908 Change <-----
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	/// Reference Button ///
	m_ReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceButton.DrawFlatFocus(TRUE);

	/// Measurement Button ///
	m_MeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementButton.DrawFlatFocus(TRUE);

	//Saiki 20090908 Change ----->
//	  /// XY Up Button ///
//	  m_XY_UpButton.SetIcon(IDI_ARROW_ROUND_UP, (int) BTNST_AUTO_GRAY);
//	  m_XY_UpButton.DrawBorder(FALSE);
//	  m_XY_UpButton.SetParams(VK_UP, WM_DO_STAGE_MOVE);
//
//	  /// XY Down Button ///
//	  m_XY_DownButton.SetIcon(IDI_ARROW_ROUND_DOWN, (int) BTNST_AUTO_GRAY);
//	  m_XY_DownButton.DrawBorder(FALSE);
//	  m_XY_DownButton.SetParams(VK_DOWN, WM_DO_STAGE_MOVE);
//
//	  /// XY Right Button ///
//	  m_XY_RightButton.SetIcon(IDI_ARROW_ROUND_RIGHT, (int) BTNST_AUTO_GRAY);
//	  m_XY_RightButton.DrawBorder(FALSE);
//	  m_XY_RightButton.SetParams(VK_RIGHT, WM_DO_STAGE_MOVE);
//
//	  /// XY Left Button ///
//	  m_XY_LeftButton.SetIcon(IDI_ARROW_ROUND_LEFT, (int) BTNST_AUTO_GRAY);
//	  m_XY_LeftButton.DrawBorder(FALSE);
//	  m_XY_LeftButton.SetParams(VK_LEFT, WM_DO_STAGE_MOVE);

	if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
		Init_SR_Transmittance_StageMenu();
	}
	else{
		Init_SR_XYStageMenu();
	}
	//Saiki 20090908 Change <-----

// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nJoystickType == JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		GetDlgItem(IDC_Z_AXIS_STATIC)->ShowWindow(SW_HIDE);
		m_Z_UpButton.ShowWindow(SW_HIDE);
		m_Z_DownButton.ShowWindow(SW_HIDE);
		m_Z_HomePosButton.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MOTION_MODE_STATIC)->ShowWindow(SW_HIDE);
		m_cbMotionElev.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		/// Z Up Button ///
#if 0
		m_Z_UpButton.SetIcon(IDI_ARROW_ROUND_UP, (int) BTNST_AUTO_GRAY);
		m_Z_UpButton.DrawBorder(FALSE);
		m_Z_UpButton.SetParams(VK_UP, WM_DO_ELEVATOR_MOVE);
#else
		m_Z_UpButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_Z_UpButton.DrawFlatFocus(TRUE);
		m_Z_UpButton.SetParams(VK_UP, WM_DO_ELEVATOR_MOVE);
#endif

		/// Z Down Button ///
#if 0
		m_Z_DownButton.SetIcon(IDI_ARROW_ROUND_DOWN, (int) BTNST_AUTO_GRAY);
		m_Z_DownButton.DrawBorder(FALSE);
		m_Z_DownButton.SetParams(VK_DOWN, WM_DO_ELEVATOR_MOVE);
#else
		m_Z_DownButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_Z_DownButton.DrawFlatFocus(TRUE);
		m_Z_DownButton.SetParams(VK_DOWN, WM_DO_ELEVATOR_MOVE);
#endif
//Saiki 20090908 Add ----->
		/// Z Home Position Button
#if 0

#else
		m_Z_HomePosButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_Z_HomePosButton.DrawFlatFocus(TRUE);
#endif


//Saiki 20090908 Add <-----
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	/// Cancel Button ///
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	/// OK Button ///
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

/* added 2015.03.10 hmenjo �e�X�g����ۑ� ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
		m_BtnSave.ShowWindow(SW_SHOW);
		m_BtnSave.SetShade(	BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
								BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_BtnSave.DrawFlatFocus(TRUE);
	} else {
		m_BtnSave.ShowWindow(SW_HIDE);
	}
/* added 2015.03.10 hmenjo �e�X�g����ۑ� ---------- } ---------- */

// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//	if(g_lModelType == MODEL_T3100){
	if(!m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		m_AfMeasurementButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		/// AF / Measurement Button ///
		//Saiki 20090908 Change ----->
		//m_AfMeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		//	BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		//m_AfMeasurementButton.DrawFlatFocus(TRUE);
		if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
			GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->ShowWindow(SW_HIDE);
		}
		else{
			m_AfMeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
				BUTTON_COLORING, BUTTON_NORMAL_COLOR);
			m_AfMeasurementButton.DrawFlatFocus(TRUE);
		}
		//Saiki 20090908 Change <-----
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	//Saiki 20090908 Delete ----->
	//RCP_DATA rcp_data;
	//m_pDoc->GetRcpData(&rcp_data);
	//Saiki 20090908 Delete <-----
	if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE   ||
	// 2009.10.26 K.Matsuo -->
		rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE ||
		rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G )
	// 2009.10.26 K.Matsuo <--
	{
		SetDlgItemInt(IDC_WAVELENGTH_START, MIN_TARGET_WAVELENGTH); //�g��(START)
// 2009.05.15 k-matsuo -->
//		SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);	//�g��(END)
		if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);
		else
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH_800);
// 2009.05.15 k-matsuo <--
	}
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);	//�g��(START)
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);	//�g��(END)

/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
	RECT l_rectWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &l_rectWorkArea, 0);
	RECT l_rectTestMeasDlg;	this->GetWindowRect(&l_rectTestMeasDlg);
	if ((g_lModelType == MODEL_T3100) && (l_rectWorkArea.bottom < l_rectTestMeasDlg.bottom)) {
		/* �f�X�N�g�b�v�̍�ƃG���A�c�����_�C�A���O���傫���ꍇ	*/
		/* �����̃��j���[�{�^���̃O���[�v�{�b�N�X�͂��̂܂܂̃T�C�Y�Ƃ��܂��D	*/
		/* �_�C�A���O��ӂ̈ړ�����	*/
		long l_ldYDlgBottom = l_rectTestMeasDlg.bottom - (l_rectWorkArea.bottom - 2);
		/* �ꎟ�^�u�ƃ_�C�A���O��ӂƂ̋���	*/
		RECT l_rectMeasTab;	m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
		/* �ꎟ�^�u�̒�ӂ̈ʒu���킹	*/
		m_MeasurementTab.SetWindowPos(	0,
							0, 0,
							l_rectMeasTab.right - l_rectMeasTab.left,
							l_rectMeasTab.bottom - l_rectMeasTab.top - l_ldYDlgBottom,
							SWP_NOZORDER | SWP_NOMOVE);
		/* �����{�^���̈ʒu���킹	*/
		RECT l_rectBtnOK;	this->GetDlgItem(IDOK)->GetWindowRect(&l_rectBtnOK);
		POINT l_ptBtnOK = {l_rectBtnOK.left, l_rectBtnOK.top - l_ldYDlgBottom};
		this->ScreenToClient(&l_ptBtnOK);
		this->GetDlgItem(IDOK)->SetWindowPos(0, l_ptBtnOK.x, l_ptBtnOK.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		RECT l_rectBtnCANCEL;	this->GetDlgItem(IDCANCEL)->GetWindowRect(&l_rectBtnCANCEL);
		POINT l_ptBtnCANCEL = {l_rectBtnCANCEL.left, l_rectBtnCANCEL.top - l_ldYDlgBottom};
		this->ScreenToClient(&l_ptBtnCANCEL);
		this->GetDlgItem(IDCANCEL)->SetWindowPos(0, l_ptBtnCANCEL.x, l_ptBtnCANCEL.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		RECT l_rectBtnSave;	this->GetDlgItem(IDC_BTN_SAVE)->GetWindowRect(&l_rectBtnSave);
		POINT l_ptBtnSave = {l_rectBtnSave.left, l_rectBtnSave.top - l_ldYDlgBottom};
		this->ScreenToClient(&l_ptBtnSave);
		this->GetDlgItem(IDC_BTN_SAVE)->SetWindowPos(0, l_ptBtnSave.x, l_ptBtnSave.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* �_�C�A���O�̏c�T�C�Y���f�X�N�g�b�v�̍�ƃG���A�c���ɍ��킹�܂��D	*/
		this->SetWindowPos(	0,
							0, 0,
							l_rectTestMeasDlg.right, l_rectWorkArea.bottom - 2,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */

	///// Grid Control Initialize /////
	MeasurementTab_Init();

	// �W���C�X�e�B�b�N�L���^����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// H/W�L���Ȃ�֎~����
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
			// �W���C�X�e�B�b�N����
			if(!m_pMainFrame->HardwareSimulation()){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
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
			if(!m_pMainFrame->HardwareSimulation()){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
#endif
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	m_AfExecuteFlg = FALSE;
	m_AfExecuteFlgOld = FALSE;
	m_iAlarmStatus = ALARM_NON;
	m_iAlarmStatusOld = ALARM_NON;

/* deleted 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
///* added 2015.03.19 hmenjo FWXGA �Ή� ---------- { ---------- */
//	RECT l_rectWorkArea;
//	::SystemParametersInfo(SPI_GETWORKAREA, 0, &l_rectWorkArea, 0);
//	RECT l_rectTestMeasDlg;	this->GetWindowRect(&l_rectTestMeasDlg);
//	if ((g_lModelType == MODEL_T3100) && (l_rectWorkArea.bottom < l_rectTestMeasDlg.bottom)) {
//		/* �f�X�N�g�b�v�̍�ƃG���A�c�����_�C�A���O���傫���ꍇ	*/
//		/* �����̃��j���[�{�^���̃O���[�v�{�b�N�X�͂��̂܂܂̃T�C�Y�Ƃ��܂��D	*/
//		/* �_�C�A���O��ӂ̈ړ�����	*/
//		long l_ldYDlgBottom = l_rectTestMeasDlg.bottom - (l_rectWorkArea.bottom - 2);
//		/* �ꎟ�^�u�ƃ_�C�A���O��ӂƂ̋���	*/
//		RECT l_rectMeasTab;	m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
//		/* �ꎟ�^�u�̒�ӂ̈ʒu���킹	*/
//		m_MeasurementTab.SetWindowPos(	0,
//							0, 0,
//							l_rectMeasTab.right - l_rectMeasTab.left,
//							l_rectMeasTab.bottom - l_rectMeasTab.top - l_ldYDlgBottom,
//							SWP_NOZORDER | SWP_NOMOVE);
//		/* �����{�^���̈ʒu���킹	*/
//		RECT l_rectBtnOK;	this->GetDlgItem(IDOK)->GetWindowRect(&l_rectBtnOK);
//		POINT l_ptBtnOK = {l_rectBtnOK.left, l_rectBtnOK.top - l_ldYDlgBottom};
//		this->ScreenToClient(&l_ptBtnOK);
//		this->GetDlgItem(IDOK)->SetWindowPos(0, l_ptBtnOK.x, l_ptBtnOK.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
//		RECT l_rectBtnCANCEL;	this->GetDlgItem(IDCANCEL)->GetWindowRect(&l_rectBtnCANCEL);
//		POINT l_ptBtnCANCEL = {l_rectBtnCANCEL.left, l_rectBtnCANCEL.top - l_ldYDlgBottom};
//		this->ScreenToClient(&l_ptBtnCANCEL);
//		this->GetDlgItem(IDCANCEL)->SetWindowPos(0, l_ptBtnCANCEL.x, l_ptBtnCANCEL.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
//		RECT l_rectBtnSave;	this->GetDlgItem(IDC_BTN_SAVE)->GetWindowRect(&l_rectBtnSave);
//		POINT l_ptBtnSave = {l_rectBtnSave.left, l_rectBtnSave.top - l_ldYDlgBottom};
//		this->ScreenToClient(&l_ptBtnSave);
//		this->GetDlgItem(IDC_BTN_SAVE)->SetWindowPos(0, l_ptBtnSave.x, l_ptBtnSave.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
//		/* �_�C�A���O�̏c�T�C�Y���f�X�N�g�b�v�̍�ƃG���A�c���ɍ��킹�܂��D	*/
//		this->SetWindowPos(	0,
//							0, 0,
//							l_rectTestMeasDlg.right, l_rectWorkArea.bottom - 2,
//							SWP_NOZORDER | SWP_NOMOVE);
//		m_pMeasurementGraphDlg1->ChgSizeCtrl(-l_ldYDlgBottom);
//		m_pMeasurementGraphDlg2->ChgSizeCtrl(-l_ldYDlgBottom);
//	}
/* added 2015.03.19 hmenjo FWXGA �Ή� ---------- } ---------- */
/* deleted 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */

	SetTimer(TESTMEASDLG_ENABLE_CHANGE_TIMER, 50, NULL);


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// ==========================================================================
//
void CTestMeasurementDlg::OnDestroy()
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

// 2014.01.17 Bagus Add (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
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

	if( m_pMeasurementGraphDlg1 != NULL ){
		delete m_pMeasurementGraphDlg1;
		m_pMeasurementGraphDlg1 = NULL;
	}
	if( m_pMeasurementGraphDlg2 != NULL ){
		delete m_pMeasurementGraphDlg2;
		m_pMeasurementGraphDlg2 = NULL;
	}


	m_pDoc->SetMeasMode(MEASMODE_NOT_MEASURE); //���蒆�łȂ�
	m_pDoc->InitMeasData(); //�������[������

	DummyRcpDelete(); //�_�~�[���V�s�폜

	//�X�e�[�^�X�o�[�����N���A
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

// 2010.02.01 K.Matsuo ASSERT ������� -->
	m_pMainFrame->m_pTestMeasDlgObj = NULL;
// 2010.02.01 K.Matsuo ASSERT ������� <--

	CDialog::OnDestroy();
}

// =========================================================================
//
void CTestMeasurementDlg::OnOK()
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

	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	/*//Saiki 20090520 Change <-----*/
	//ActuateFlag�`�F�b�N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������


	CString csBuff;

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_dIntegTime = atof(csBuff);

//	HookKeyUnhook();

// 2009.09.30 K.Matsuo Add -->
	///// �w��t�H���_���݊m�F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ����f�[�^���t�H���_���c���Ă�����폜 /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}
// 2009.09.30 K.Matsuo Add <--

	CDialog::OnOK();
}

// =========================================================================
//
void CTestMeasurementDlg::OnCancel()
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

//	HookKeyUnhook();

// 2009.09.30 K.Matsuo Add -->
	///// �w��t�H���_���݊m�F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ����f�[�^���t�H���_���c���Ă�����폜 /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}
// 2009.09.30 K.Matsuo Add <--

	CDialog::OnCancel();
}

// =========================================================================
//
void CTestMeasurementDlg::MeasurementTab_Init()
{
	///// Tab	/////
	//m_MeasurementTab.InsertItem(0, "Reflectance Data");
	//m_MeasurementTab.InsertItem(1, "Raw Data");
	//Saiki 20090528 Change ----->
	//m_MeasurementTab.InsertItem(0, "���˗��f�[�^"); //Reflectance Data
	//m_MeasurementTab.InsertItem(1, "�����x�f�[�^"); //Raw Data

	CString strBuf1, strBuf2;

	LoadStringML(IDS_CAPTION_REFLECTANCE_DATA, strBuf1, "Reflectance Data");
	LoadStringML(IDS_CAPTION_RAW_DATA, strBuf2, "Raw Data");
	m_MeasurementTab.InsertItem(0, strBuf1); //Reflectance Data
	m_MeasurementTab.InsertItem(1, strBuf2); //Raw Data
	//Saiki 20090528 Change <-----
	///// List	/////
	int dx = 5;
	int dy = 24;

	///// Graph(�ʏ�) /////
	m_pMeasurementGraphDlg1 = new CMeasurementGraphDlg2(GRAPHDLG_MODE_TEST_NOMAL);
	m_pMeasurementGraphDlg1->Create(CMeasurementGraphDlg2::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg1->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Graph(RAW) /////
	m_pMeasurementGraphDlg2 = new CMeasurementGraphDlg2(GRAPHDLG_MODE_TEST_RAW);
	m_pMeasurementGraphDlg2->Create(CMeasurementGraphDlg2::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg2->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Set Current Selection Item (Default Select Tab) ////
	int Index;
	Index = m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay;
	TabChange(Index);
	m_MeasurementTab.SetCurSel(Index);

}

// =========================================================================
//
void CTestMeasurementDlg::TabChange(int Index)
{
	m_MeasurementTab.HighlightItem(0, FALSE);
	m_MeasurementTab.HighlightItem(1, FALSE);
	m_MeasurementTab.HighlightItem(Index, TRUE);

	///// Hide All Dialog /////
	m_pMeasurementGraphDlg1->ShowWindow(SW_HIDE);
	m_pMeasurementGraphDlg2->ShowWindow(SW_HIDE);

	///// Show Dialog /////
	switch(Index){
	case 0:
		m_pMeasurementGraphDlg1->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pMeasurementGraphDlg2->ShowWindow(SW_SHOW);
		break;
	}
}

// =========================================================================
//
void CTestMeasurementDlg::OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index = m_MeasurementTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CTestMeasurementDlg::LoadRecipeData()
{

	//���V�s�Ǎ���

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	RecipeFile_LoadRecipe(&m_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE); //
	RecipeFile_LoadRecipe(&m_Meas, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
}

// =========================================================================
//�����Y �R���{�C�j�V��������
//
void CTestMeasurementDlg::InitCombo_Lens()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
		if ( m_SrTurret[i].bEnable ) {
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	pCombo->SetCurSel(nIndex);

// 2009.10.01 K.Matsuo Bug Fix -->
	if ( nIndex == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nIndex);
// 2009.10.01 K.Matsuo Bug Fix <--
}

// =========================================================================
// �����Y �R���{�I����
//
void CTestMeasurementDlg::OnSelchangeSrLens()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----
	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//�捞�ݏ���(Detector Condition)�ύX��

	UpdateData(FALSE);
}

// =========================================================================
//
void CTestMeasurementDlg::InitCombo_SrOpticalFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

// 2013.12.18 Bagus Add (TohoSpec�Ή�) -->
	BOOL bUseFilter;

	if(g_lModelType != MODEL_T3100){
		bUseFilter = m_SrConfig.bAutoFilter;	//�I�[�g�t�B���^�̃R���t�B�O�ݒ�̓��e
	}
	else{
		bUseFilter = TRUE;	// T3100�ł́A�t�B���^�ݒ����Ɏg�p�i�\���j����
	}
// 2013.12.18 Bagus Add (TohoSpec�Ή�) <--

// 2013.12.18 Bagus Mod (TohoSpec�Ή�) -->
//	//�I�[�g�t�B���^����̏ꍇ
//	if (m_SrConfig.bAutoFilter){
	// �t�B���^�ݒ���g�p����ꍇ
	if (bUseFilter){
// 2013.12.18 Bagus Mod (TohoSpec�Ή�) <--
		pCombo->EnableWindow(FALSE);
		pCombo->ResetContent();

		SR_FILTER srFilter[SR_FILTER_MAX];
		ConfigFile_GetNanoSpecIni(srFilter, CONFIG_FILE_SR_FILTER);

		int nIndex;
		int iFilterIndex;
		for ( int iOptItem = 0; iOptItem < OPT_FILTER_MAX; iOptItem++ ) {
			switch ( iOptItem ) {
			case OPT_FILTER_UNKNOWN:
				break;
			case OPT_FILTER_OPEN:
				// Kojika 20090528 Change
				//nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM[iOptItem]);
				if(l_SystemConfig.nLanguage == 0){
					nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM_ENU[iOptItem]);
				}else{
					nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM_JPN[iOptItem]);
				}
				// Kojika 20090528 Change End
				pCombo->SetItemData(nIndex, iOptItem);
				break;
			case OPT_FILTER_DARK:
				break;
			case OPT_FILTER_POS1:
			case OPT_FILTER_POS2:
			case OPT_FILTER_POS3:
				iFilterIndex = iOptItem - 3;
				if ( srFilter[iFilterIndex].bEnable ) {
					nIndex = pCombo->AddString(srFilter[iFilterIndex].szName);
					pCombo->SetItemData(nIndex, iOptItem);
				}
				break;
			default:
				break;
			}
		}

//		LPCTSTR pszFilterName[] = {OPTICAL_FILTER_TYPE_ITEM[0], OPTICAL_FILTER_TYPE_ITEM[1], OPTICAL_FILTER_TYPE_ITEM[2], srFilter[0].szName, srFilter[1].szName, srFilter[2].szName};

		nIndex = pCombo->FindStringExact(-1, m_strOpticalFilter);
		pCombo->SetCurSel(nIndex);
	}

	//�I�[�g�t�B���^�Ȃ��̏ꍇ
	else{
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
// �I�v�e�B�J���t�B���^�[ �R���{�I����
//
void CTestMeasurementDlg::OnSelchangeSrOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.wOpticsFilterType = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//�捞�ݏ���(Detector Condition)�ύX��

}

#if 0
// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnKeyHook(WPARAM wParam, LPARAM lParam)
{
	TRACE("CManualMeasurementFormView::OnKeyHook\n");

	switch ( wParam ) {
	case VK_RIGHT:
	case VK_LEFT:
	case VK_UP:
	case VK_DOWN:
		if ( HIWORD(lParam) & KF_UP ) {
			CCursor::EndWaitCursor();
			StageStop();
			PostMessage(WM_STAGE_MOVE_DONE, 0L, 0L);
		}
		break;
	default:
		return 0L;
	}

	return 0L;
}

// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnMouseHook(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}
#endif

// ==========================================================================
//
void CTestMeasurementDlg::Init_SR_XYStageMenu()
{
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType == MODEL_T3100){
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
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			m_XY_UpButton.ShowWindow(SW_HIDE);
			m_XY_DownButton.ShowWindow(SW_HIDE);
			m_XY_RightButton.ShowWindow(SW_HIDE);
			m_XY_LeftButton.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);
		}
		else{
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
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
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
		}
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
// 2009.10.29 bagus Jog �ǉ��C�� --{--
// 		//JoyStick Show
// 		GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		if (m_SystemConfig.nJoystickType == JOYSTICK_TYPE_NONE) {
			GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_HIDE);
	 		GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
		} else {
			GetDlgItem(IDC_JOG_MODE_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STAGE_MOTION_MODE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
		}
// 2009.10.29 bagus Jog �ǉ��C�� --}--
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	//Lens Disable
	GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);

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
void CTestMeasurementDlg::Init_SR_Transmittance_StageMenu()
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

	//Lens Disable
	GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);

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
	for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
		_itoa(iMeasPt, szMeasPt, 10);
		pCombo->AddString(szMeasPt);
	}
	pCombo->SetCurSel(0);

	//XY Position Show
	GetDlgItem(IDC_MEAS_POINT_X_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MEAS_POINT_Y_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MEAS_POINT_X)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_MEAS_POINT_Y)->ShowWindow(SW_SHOW);

// 2009.12.08 bagus --{--
#if 0
	m_stcMeasurementPointX.SetBkColor(GREEN_COLOR);
	m_stcMeasurementPointY.SetBkColor(GREEN_COLOR);
#else
	m_stcMeasurementPointX.SetBkColor(REFERENCE_COLOR);
	m_stcMeasurementPointY.SetBkColor(REFERENCE_COLOR);
#endif
// 2009.12.08 bagus --}--

	GetDlgItem(IDC_MEAS_POINT_X)->SetWindowText("-----");
	GetDlgItem(IDC_MEAS_POINT_Y)->SetWindowText("-----");
}

// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnDoStageMove(WPARAM wParam, LPARAM lParam)
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
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return 0L;
		}	// �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
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
LRESULT CTestMeasurementDlg::OnStageMoveDone(WPARAM wParam, LPARAM lParam)
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
LRESULT CTestMeasurementDlg::OnDoElevatorMove(WPARAM wParam, LPARAM lParam)
{
BOOL result;
short sign = 1;
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
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckUnitStatus() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	//
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
				return 0L;
		}	// �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

	switch ( wParam ) {
	case VK_DOWN:
		sign *= -1;
	case VK_UP:
		break;
	default:
		return 0L;
	}

// 2009.05.28 K.Matsuo SPT -->
	UINT nElevMode = m_cbMotionElev.GetElevMode();

	CCursor::BeginWaitCursor();

	int MotionJog = GetMotionJog(Z, nElevMode);

	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);

	if(!m_pMainFrame->HardwareSimulation()){
		do{
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if(m_pDoc->IsInterLock() == TRUE)
				break;
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			result = StageElevatorMoveRelative(MotionJog * sign);
// DEL 2009.06.01
//			if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
			Sleep(200);
		}while( ::GetAsyncKeyState(VK_LBUTTON) < 0);
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	CCursor::EndWaitCursor();

// 2009.05.28 K.Matsuo SPT <--

	return 0L;
}

// ==========================================================================
//
BOOL CTestMeasurementDlg::PreTranslateMessage(MSG* pMsg)
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
					TRACE("CTestMeasurementDlg::PreTranslateMessage WM_KEYDOWN\n");
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
				TRACE("CTestMeasurementDlg::PreTranslateMessage WM_KEYUP\n");
				return TRUE;
			}
			break;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
void CTestMeasurementDlg::OnAutoFocusButton()
{
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----
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
	}

	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlag�`�F�b�N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strBuffer, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strBuffer);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������


	BOOL bJoyStk;

	m_AfExecuteFlg = TRUE;

	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //�W���C�X�e�B�b�N���g�p�s�ɐݒ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}

	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	if( NS_StageDoAutoFocus()==FALSE ){
//		m_AutoFocusResult	 = 1;	// AutoFocus Result NG
		//Saiki 20090528 Change ----->
		//MessageBox("�I�[�g�t�H�[�J�X���s!!!", "AUTO FOCUS", MB_OK|MB_ICONWARNING);
		LoadStringML(IDS_AUTOFOCUS_FAILED, strBuffer, "Auto Focus Failed !!!");
		LoadStringML(IDS_TITLE_AUTO_FOCUS, strTitle, "AUTO FOCUS");
		MessageBox(strBuffer, strTitle, MB_OK|MB_ICONWARNING);
		//Saiki 20090528 Change <-----
	}
	else{
//		m_AutoFocusResult	 = 0;	// AutoFocus Result OK
		m_pDoc->OperationLogging("AUTO FOCUS Button was push.");
	}
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
	CCursor::EndWaitCursor();

	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //�W���C�X�e�B�b�N���g�p�ɐݒ�
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}

	m_AfExecuteFlg = FALSE;

}


// ==========================================================================
//
void CTestMeasurementDlg::OnReferenceButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	//Saiki 20090530 Add ----->
	CString strMsg, strTitle;
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
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlag�`�F�b�N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		//Saiki 20090601 Change ----->
		//MessageBox(	"���쒆�ł�",	"�x��", MB_OK);
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	//Saiki 20090528 Change ----->
	//char* pszCaption = "�e�X�g���[�h���t�@�����X����J�n";
	//char* pszCaption;
	CString strBuffer;
	//Saiki 20090528 Change <-----


	m_pDoc->GetRcpData(&rcp_data);


	///// Clear Reference /////
	ClearReference(rcp_data.szRecipeName);


	//�����Y�ݒ�
	UINT uiLens = m_Meas.ScanParams._SR.iLens;
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(uiLens);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	//�����W���[�����g�v���O�������C��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
	{
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_REFLECTANCE);
			break;
		case MEAS_PROG_TYPE_SR_THICKNESS:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_THICKNESS);

			break;
		default:
			break;
	}


	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
// ADD 2009.06.01
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// �`�[�t�̌��� /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
// ADD 2009.06.01
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
		return;
	}

/* modified 2017.01.13 hmenjo �e�X�g�n���O�C�� ---------- { ---------- */
//	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");
/* modified 2017.01.13 hmenjo �e�X�g�n���O�C�� ----------              */
	m_pDoc->OperationLogging("Test Meas. Reference Button was push.");
/* modified 2017.01.13 hmenjo �e�X�g�n���O�C�� ---------- } ---------- */

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //�W���C�X�e�B�b�N���g�p�s�ɐݒ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnReferenceButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}

// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
			CExecutionDlg dlg;
//			dlg.SetCaption(EXECUTION_CAPTION);
			LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
			dlg.SetCaption(strBuffer);
			//Saiki 20090128 Change <-----
			//Saiki 20090528 Change ----->
			//dlg.SetMessage("���t�@�����X���蒆...");
			LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
			dlg.SetMessage(strBuffer);
			//Saiki 20090528 Change <-----
			dlg.DoModal();
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

	m_iConditionFlg = TESTMODE_REF_AFTER;	//���t�@�����X�����(��̃��[�_���̃_�C�A���O�{�b�N�X�����t�@�����X����I���܂Ŕ����Ȃ�����)

	return;

}


// ==========================================================================
//
void CTestMeasurementDlg::OnMeasurementButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle, strMsg;
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
	}

	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlag�`�F�b�N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������


	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE/*_Ed*/)
	{
		// Kojika 20090529 Change
		//MessageBox("�捞�ݏ������ύX����Ă��܂��B\n���t�@�����X���擾��ɍēx��������s���Ă��������B", "NanoSpec", MB_OK);
		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
		return;
	}


	//�����C�����V�s��������
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 0;
	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
	{
		// Kojika 20090529 Change
		//MessageBox("���t�@�����X���s�t���O�̍X�V�Ɏ��s���܂���", "NanoSpec", MB_OK);
		LoadStringML(IDS_FAILED_UPDATE_REFER_EXE, l_strBuffer, "It failed in the update of the reference execution flag");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}
	m_pDoc->SetRcpData(&rcp_data);

/* added 2017.01.13 hmenjo �e�X�g�n���O�C�� ---------- { ---------- */
	m_pDoc->OperationLogging("Test Meas. Measurement Button was push.");
/* added 2017.01.13 hmenjo �e�X�g�n���O�C�� ---------- } ---------- */

	StartTestModeMeas();

}


//Saiki 20130123 change --->
// ==========================================================================
//
//void CTestMeasurementDlg::OnAfAndMeasurementButton()
//{
//	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
//
//	// Kojika 20090529 Add
//	CString l_strBuffer, l_strTitle, strMsg;
//	// Kojika 20090529 Add End
//
//	//�����[�g���͉������Ȃ�
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
//
//
//	// ���C���^�[���b�N������
//	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
//	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
//	{
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
//		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
//			���̏ꍇ�A2����������\ */
//	}
//	else //���������e�i���X���[�h�ȊO
//	{
//	}
//
//	// �eI/O�`�F�b�N
//	/*//Saiki 20090520 Change ----->*/
//	if(m_pDoc->IsInterLock() == TRUE){return;}
//	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
//	/*//Saiki 20090520 Change <-----*/
//
//	//ActuateFlag�`�F�b�N
//	//Saiki 20090601 Change ----->
//	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
//		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
//		m_pDoc->MessageStringIf_Set(strMsg);
//		return;
//	} //�X�e�[�W�����쒆�ł�
//	//Saiki 20090601 Change <-----
//	/*//Saiki 20090520 Change ----->*/
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
//	/*//Saiki 20090520 Change <-----*/
//	// ���C���^�[���b�N������
//
//
//	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE)
//	{
//		// Kojika 20090529 Change
//		//MessageBox("�捞�ݏ������ύX����Ă��܂��B\n���t�@�����X���擾��ɍēx��������s���Ă��������B", "NanoSpec", MB_OK);
//		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
//		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
//		MessageBox(l_strBuffer, l_strTitle, MB_OK);
//		// Kojika 20090529 Change End
//		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
//		return;
//	}
//
//
//	//�����C�����V�s��������
//	RCP_DATA rcp_data;
//	memset(&rcp_data, 0, sizeof(rcp_data));
//	m_pDoc->GetRcpData(&rcp_data);
//	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 1;
//	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
//	{
//		// Kojika 20090529 Change
//		//MessageBox("���t�@�����X���s�t���O�̍X�V�Ɏ��s���܂���", "NanoSpec", MB_OK);
//		LoadStringML(IDS_FAILED_UPDATE_REFER_EXE, l_strBuffer, "It failed in the update of the reference execution flag");
//		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
//		MessageBox(l_strBuffer, l_strTitle, MB_OK);
//		// Kojika 20090529 Change End
//		return;
//	}
//	m_pDoc->SetRcpData(&rcp_data);
//
//	StartTestModeMeas();
//
//}
// ==========================================================================
//
void CTestMeasurementDlg::OnAfAndMeasurementButton()
{
	CString l_strBuffer, l_strTitle;
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
	}

	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlag�`�F�b�N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, l_strBuffer, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(l_strBuffer);
		return;
	} //�X�e�[�W�����쒆�ł�
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ���C���^�[���b�N������


	BOOL bJoyStk;

	m_AfExecuteFlg = TRUE;

	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //�W���C�X�e�B�b�N���g�p�s�ɐݒ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}

	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	if( NS_StageDoAutoFocus()==FALSE ){
		LoadStringML(IDS_AUTOFOCUS_FAILED, l_strBuffer, "Auto Focus Failed !!!");
		LoadStringML(IDS_TITLE_AUTO_FOCUS, l_strTitle, "AUTO FOCUS");
		MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONWARNING);
	}
	else{
		m_pDoc->OperationLogging("AUTO FOCUS Button was push.");
	}
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
	CCursor::EndWaitCursor();

	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //�W���C�X�e�B�b�N���g�p�ɐݒ�
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}

	m_AfExecuteFlg = FALSE;

	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE/*_Ed*/)
	{
		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
		return;
	}


	//�����C�����V�s��������
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 0;
	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
	{
		LoadStringML(IDS_FAILED_UPDATE_REFER_EXE, l_strBuffer, "It failed in the update of the reference execution flag");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		return;
	}
	m_pDoc->SetRcpData(&rcp_data);

	int ChiefRet = 0;
	BOOL bResult = FALSE;

	m_pDoc->GetRcpData(&rcp_data);


	//���萔����
	if(m_pDoc->GetStoreCount() >= TESTDATA_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
		LoadStringML(IDS_MEASURE_DATA_NUMBER_EXCEED, l_strBuffer, "Measurement data of number %d is exceeded.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		sprintf(szMsgBuff, l_strBuffer, TESTDATA_POINT_MAX);
		MessageBox(szMsgBuff, l_strTitle, MB_OK);
		return;
	}


	//�����W���[�����g�v���O�������C��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
	{
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_REFLECTANCE);
			break;
		case MEAS_PROG_TYPE_SR_THICKNESS:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_THICKNESS);

			break;
		default:
			break;
	}


	///// �`�[�t�֑���J�n���b�Z�[�W�𑗐M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

// �ꎞ�I�Ƀn�[�hJoyStick���E��
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1�|�C���g����

	///// �`�[�t�̌��� /////
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //�����

}

//Saiki 20130123 change --->
// ==========================================================================
//
void CTestMeasurementDlg::StartTestModeMeas()
{

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
	CString l_strBuffer, l_strTitle;
	//char* pszCaption = "�e�X�g���[�h����J�n";
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	//���萔����
	if(m_pDoc->GetStoreCount() >= TESTDATA_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
		// Kojika 20090529 Change
		//sprintf(szMsgBuff, "����f�[�^�� %d �𒴂��Ă��܂�!", TESTDATA_POINT_MAX);
		LoadStringML(IDS_MEASURE_DATA_NUMBER_EXCEED, l_strBuffer, "Measurement data of number %d is exceeded.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		sprintf(szMsgBuff, l_strBuffer, TESTDATA_POINT_MAX);
		MessageBox(szMsgBuff, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}


	//�����W���[�����g�v���O�������C��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
	{
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_REFLECTANCE);
			break;
		case MEAS_PROG_TYPE_SR_THICKNESS:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_THICKNESS);

			break;
		default:
			break;
	}


	///// �`�[�t�֑���J�n���b�Z�[�W�𑗐M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

#if 1
// ADD 2009.06.02 H.Kawamura
// �ꎞ�I�Ƀn�[�hJoyStick���E��
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();

		return;
	}
// ADD 2009.06.02 H.Kawamura
#endif

/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM)rcp_data.szRecipeName, 0); //1�|�C���g����
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ----------			   */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1�|�C���g����
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ---------- } ---------- */

	///// �`�[�t�̌��� /////
	// Kojika 20090529 Change
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");

#if 1
// ADD 2009.06.02 H.Kawamura
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.02 H.Kawamura
#endif

		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

#if 0
// DEL 2009.06.02 H.Kawamura
// �ꎞ�I�Ƀn�[�hJoyStick���E�������ɒu��������
	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //�W���C�X�e�B�b�N���g�p�s�ɐݒ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnMeasurementButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}
// DEL 2009.06.02 H.Kawamura
#endif

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //�����
}



// ==========================================================================
//
void CTestMeasurementDlg::ClearReference(char* szRecipeName)
{
	char szFilePath[MAX_PATH];
	memset(szFilePath, 0, sizeof(szFilePath));

	sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, szRecipeName, DAT_EXT);
	DeleteFile(szFilePath);
}



// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnTestModeOnePointEnd(WPARAM wParam, LPARAM lParam)
{
//�e�X�g���[�h1�|�C���g����I��������

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);

	return 0L;
}



// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnTestModeRefEnd(WPARAM wParam, LPARAM lParam)
{
//�e�X�g���[�h1�|�C���g����I��������

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);

	return 0L;
}


// =========================================================================
//�_�~�[���W���[�����g�v���O�������㏑��
BOOL CTestMeasurementDlg::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName, int iRecipeTipe)
{
	switch(iRecipeTipe)
	{
		case MEAS_PROG_TYPE_SR_THICKNESS:
			if ( !RecipeFile_SaveRecipe(&m_Meas, pszRecipeName, RECIPE_FILE_SR_THICKNESS) )
			{
				return FALSE;
			}
			break;

		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			if ( !RecipeFile_SaveRecipe(&m_Meas, pszRecipeName, RECIPE_FILE_SR_REFLECTANCE) )
			{
				return FALSE;
			}
			break;

		default:
				return FALSE;
			break;
	}
	return TRUE;
}


// =========================================================================
//�_�~�[���V�s�폜
BOOL CTestMeasurementDlg::DummyRcpDelete()
{
	char szDeleteRcpPathMain[255 + 1];
	char szDeleteRcpPathMeas[255 + 1];
	char szDeleteRcpPathRef[255 + 1];
	memset(szDeleteRcpPathMain, 0, sizeof(szDeleteRcpPathMain));
	memset(szDeleteRcpPathMeas, 0, sizeof(szDeleteRcpPathMeas));
	memset(szDeleteRcpPathRef, 0, sizeof(szDeleteRcpPathRef));

	sprintf(szDeleteRcpPathMain, "%s%s%s",
			g_szDb_Main_Recipe_Dir, m_MainRcpInfo.hdr.szName, MAINRECIPE_EXT);
	sprintf(szDeleteRcpPathMeas, "%s%s%s",
			g_szDb_Measurement_Program_Dir, m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEASUREMENTPGM_EXT);
	sprintf(szDeleteRcpPathRef, "%s%s%s",
		g_szData_Ref_Dir, m_MainRcpInfo.hdr.szName, DAT_EXT);


	BOOL bMainDel = DeleteFile( szDeleteRcpPathMain );
	BOOL bMeasDel = DeleteFile( szDeleteRcpPathMeas );
	BOOL bRefDel = DeleteFile( szDeleteRcpPathRef );


	if((bMainDel == FALSE)||(bMeasDel == FALSE)||(bRefDel == FALSE))
	{
		return FALSE;
	}

	return TRUE;
}


// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ -->
// =========================================================================
//
void CTestMeasurementDlg::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* �h�A�C���^�[���b�N			���g */
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
			if ( m_joyStick2->GetCheck() ) {
				// �����I�Ƀ\�t�gJoy�֐؂�ւ���
				m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
				m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				m_joyStick1->Checked();
				m_joyStick2->UnChecked();
//				m_joyStick1->SetFocus();		// ���W�I�{�^���̃`�F�b�N�������ĂȂ��Ȃ��肪��������̂ŃR�����g�A�E�g�B�����s���B
			}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	}
}
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ <--

void CTestMeasurementDlg::OnTimer(UINT nIDEvent)
{
BOOL bJoyStk;

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iAlarmStatus = m_pDoc->GetAlarmStatus();

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

	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld))
	{
//================
		//�n�[�h�W���C�X�e�B�b�N���쐧��(�A���[�����̂�)
		if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_EXIST))
		{
			// H/W�L���Ȃ�
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
		else if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_NON))
		{
			// H/W�L���Ȃ�
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
			}
		}

//================
		//�R���g���[����Enable����(�P�ƃI�[�g�t�H�[�J�X���쎞�ȊO)
		switch(nIDEvent)
		{
			case  TESTMEASDLG_ENABLE_CHANGE_TIMER:

				if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_INIT)||(m_iConditionFlg == TESTMODE_CONDITION_CANGE_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //�wWAIT�x ���� (�w�e�X�g���[�h�N������x �܂��� �w�捞�ݏ���(Detector Condition)�ύX��x) ���� �w�A���[���Ȃ��x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//�����Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//���t�@�����X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//�I�[�g�t�H�[�J�X�^����
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);		//�I�[�g�t�H�[�J�X
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

					//�X�e�[�W
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);		//���[�h(XY�X�e�[�W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(TRUE);		//���[�h(Z��)
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					//�����{�^���̓{�^�����ŏ���

					//�I��
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[Save]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- } ---------- */
				}
				else if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_REF_AFTER)||(m_iConditionFlg == TESTMODE_MEAS_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //�wWAIT�x ���� (�w���t�@�����X�����x �܂��� �w�����x) ���� �w�A���[���Ȃ��x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//�����Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//���t�@�����X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(TRUE); 	//����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//�I�[�g�t�H�[�J�X�^����
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);		//�I�[�g�t�H�[�J�X
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

					//�X�e�[�W
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);		//���[�h(XY�X�e�[�W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(TRUE);		//���[�h(Z��)
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					//�����{�^���̓{�^�����ŏ���

					//�I��
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[sSave]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- } ---------- */
				}
				else if((m_iProcessStatus == PROCESS_DOWN) || (m_iAlarmStatus != ALARM_NON))
				{ //�wDOWN�x �܂��� �w�A���[���������x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//�����Y
					}
					//Saiki 20090908 Change <-----
					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//���t�@�����X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//�I�[�g�t�H�[�J�X�^����
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 	//�I�[�g�t�H�[�J�X
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

					//�X�e�[�W
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 	//���[�h(XY�X�e�[�W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(FALSE);		//���[�h(Z��)
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					//�����{�^���̓{�^�����ŏ���

					//�I��
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[Save]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- } ---------- */
				}
				else if((m_iProcessStatus == PROCESS_INIT)
					||(m_iProcessStatus == PROCESS_PROC)
					||(m_iProcessStatus == PROCESS_ABRT)
					||(m_iProcessStatus == PROCESS_COMP))
				{ //�wINIT�x�܂��́wPROC�x�܂��́wABRT�x�܂��́wCOMP�x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE); 			   //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);				 //�����Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(FALSE); 		//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(FALSE); //���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//���t�@�����X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//�I�[�g�t�H�[�J�X�^����
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 	//�I�[�g�t�H�[�J�X
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

					//�X�e�[�W
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 	//���[�h(XY�X�e�[�W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(FALSE);		//���[�h(Z��)
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
					}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
					//�����{�^���̓{�^�����ŏ���

					//�I��
					GetDlgItem(IDOK)->EnableWindow(FALSE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);					//[CANCEL]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- { ---------- */
					m_BtnSave.EnableWindow(FALSE);								//[Save]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- } ---------- */
				}
				else
				{
					//�R�R�ɂ͂��Ȃ��͂�
				}

				break;
		}
	}

//================

	//�R���g���[����Enable����(�P�ƃI�[�g�t�H�[�J�X���쎞)
	if(m_AfExecuteFlg != m_AfExecuteFlgOld)
	{
		//�捞�ݏ���
		GetDlgItem(IDC_SR_LENS)->EnableWindow(!m_AfExecuteFlg); 				//�����Y
		if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
		{
			GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(!m_AfExecuteFlg);		//���w�t�B���^�[
		}
		GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(!m_AfExecuteFlg);	//���莞��

		//����n
		GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//���t�@�����X
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//����
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(!m_AfExecuteFlg);	//�I�[�g�t�H�[�J�X�^����
			GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//�I�[�g�t�H�[�J�X
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--

		//�X�e�[�W
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(!m_AfExecuteFlg);		//���[�h(XY�X�e�[�W)
			GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(!m_AfExecuteFlg);		//���[�h(Z��)
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		//�����{�^���̓{�^�����ŏ���

		//�I��
		GetDlgItem(IDOK)->EnableWindow(!m_AfExecuteFlg);						//[OK]�{�^��
		GetDlgItem(IDCANCEL)->EnableWindow(!m_AfExecuteFlg);					//[CANCEL]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- { ---------- */
		m_BtnSave.EnableWindow(!m_AfExecuteFlg);								//[Save]�{�^��
/* added 2015.03.21 hmenjo �e�X�g����ۑ� ---------- } ---------- */
	}


#if 1 //ProcessStatus��ActuateFlags�𕹗p
//�捞�ݏ����ҏW���A�E�F�C�g�ȊO�A�A���[���������AAF���s�� �̓{�^�������ɂ���B
	//�\�t�g�W���C�X�e�B�b�N����
	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld)||(m_AfExecuteFlg != m_AfExecuteFlgOld))
	{
		if((m_iConditionFlg == TESTMODE_CONDITION_CANGE_EDIT) || ( m_iProcessStatus != PROCESS_WAIT ) || (m_iAlarmStatus == ALARM_EXIST) || (m_AfExecuteFlg == TRUE))
		{
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
			if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
				SoftJoyStickXyzEnable(FALSE); //����֎~

				//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
				GetDlgItem(IDC_JOYSTICK1)->EnableWindow(FALSE);
				GetDlgItem(IDC_JOYSTICK2)->EnableWindow(FALSE);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
			}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		}
		else
		{
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
			if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
				SoftJoyStickXyzEnable(TRUE); //���싖��

				//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
				GetDlgItem(IDC_JOYSTICK1)->EnableWindow(TRUE);
				GetDlgItem(IDC_JOYSTICK2)->EnableWindow(TRUE);
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
			}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
		}
	}
#endif

//================
	//�O��l�X�V
	m_iConditionFlgOld = m_iConditionFlg;
	m_iProcessStatusOld = m_iProcessStatus;
	m_iAlarmStatusOld = m_iAlarmStatus;
	m_AfExecuteFlgOld = m_AfExecuteFlg;

//================

	CDialog::OnTimer(nIDEvent);
}



void CTestMeasurementDlg::OnSetfocusSrLens()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementDlg::OnKillfocusSrLens()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//�捞�ݏ���(Detector Condition)�ύX��
}

void CTestMeasurementDlg::OnSetfocusSrOptFilter()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementDlg::OnKillfocusSrOptFilter()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//�捞�ݏ���(Detector Condition)�ύX��
}

void CTestMeasurementDlg::OnSetfocusMeasIntegrationTime()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementDlg::OnKillfocusMeasIntegrationTime()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_Meas.ScanParams._SR.dIntegTime = atof(csBuff);

	m_pDoc->SetRcpData(&rcp_data);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//�捞�ݏ���(Detector Condition)�ύX��

}

void CTestMeasurementDlg::OnSetfocusWavelengthStart()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementDlg::OnKillfocusWavelengthStart()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
	{
		m_pDoc->GetRcpData(&rcp_data);
		GetDlgItemText(IDC_WAVELENGTH_START, csBuff);
		rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart = m_Meas.ScanParams._SR.WavelenRange.wStart = atoi(csBuff);
		m_pDoc->SetRcpData(&rcp_data);
	}
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //�捞�ݏ���(Detector Condition)�ύX��

}

void CTestMeasurementDlg::OnSetfocusWavelengthEnd()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementDlg::OnKillfocusWavelengthEnd()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
	{
		m_pDoc->GetRcpData(&rcp_data);
		GetDlgItemText(IDC_WAVELENGTH_END, csBuff);
		rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = m_Meas.ScanParams._SR.WavelenRange.wEnd = atoi(csBuff);
		m_pDoc->SetRcpData(&rcp_data);
	}
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //�捞�ݏ���(Detector Condition)�ύX��
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
//void CTestMeasurementDlg::OnJoyStick()
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ----------			   */
LRESULT CTestMeasurementDlg::OnJoyStick(WPARAM wparam, LPARAM lparam)
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
			// �W���C�X�e�B�b�N����
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
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

// =========================================================================
//
BOOL CTestMeasurementDlg::CheckData()
{
	CString strBuffer;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	if(UpdateData(TRUE) == 0) return FALSE;

	///// Integration Time /////
	if ( m_Meas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_Meas.ScanParams._SR.dIntegTime )
	{
		// Kojika 20090529 Change
//		strBuffer.Format("Integration Time is out of range.\n(%.3f - %.3f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		//strBuffer.Format("���莞�Ԃ��͈͊O�ɐݒ肳��Ă��܂�\n(%.3f - %.3f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		//MessageBox(strBuffer, "NanoSpec", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_INTEGRA_TIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.3f - %.3f)");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}
	return TRUE;
}

#if 0
// =========================================================================
//
void CTestMeasurementDlg::ButtonEnableCheck()
{
// ����삳���
// ���蒆�Ȃǂ̏����͕�����܂���̂ŒǋL�肢�܂�

//���wSoftJoyStickXyzEnable()�̒��x�ɋL�q���܂����B hibino

	// XY���{�^��
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			//�\�t�g�W���C�X�e�B�b�N����
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
			if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ){
				//�\�t�g�W���C�X�e�B�b�N����
				m_XY_UpButton.SetEnabled(FALSE);
				m_XY_DownButton.SetEnabled(FALSE);
				m_XY_RightButton.SetEnabled(FALSE);
				m_XY_LeftButton.SetEnabled(FALSE);
			}
			else{
				//�\�t�g�W���C�X�e�B�b�N�L��
				m_XY_UpButton.SetEnabled(TRUE);
				m_XY_DownButton.SetEnabled(TRUE);
				m_XY_RightButton.SetEnabled(TRUE);
				m_XY_LeftButton.SetEnabled(TRUE);
			}
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	// �W���C�X�e�B�b�N



	// Z���{�^��
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		if( m_pDoc->ActuateFlagsGet( ACTUATE_ZAXIS ) ){
			m_Z_UpButton.SetEnabled(FALSE);
			m_Z_DownButton.SetEnabled(FALSE);
		}
		else{
			m_Z_UpButton.SetEnabled(TRUE);
			m_Z_DownButton.SetEnabled(TRUE);
		}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
}
#endif

// =========================================================================
//
void CTestMeasurementDlg::SoftJoyStickXyzEnable(BOOL bEnable)
{
	if( m_pMainFrame->GetJoyStickMode() != 0 ) return;
	if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ) return;
	if( m_pDoc->ActuateFlagsGet( ACTUATE_ZAXIS ) ) return;

	//SoftJoyStick
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		m_XY_UpButton.SetEnabled(bEnable);
		m_XY_DownButton.SetEnabled(bEnable);
		m_XY_RightButton.SetEnabled(bEnable);
		m_XY_LeftButton.SetEnabled(bEnable);
		m_Z_UpButton.SetEnabled(bEnable);
		m_Z_DownButton.SetEnabled(bEnable);
		//Saiki 20090908 Add ----->
		m_Z_HomePosButton.SetEnabled(bEnable);
		//Saiki 20090908 Add <-----
// 2013.11.08 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.11.08 Bagus Add (TohoSpec�Ή�) <--
}

// =========================================================================
//
void CTestMeasurementDlg::OnSelchangeComboMeasPoint()
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

	//���t�@�����X�|�W�V�����̃��[�h
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	iSelectMeasPt = pCombo->GetCurSel();
	if(0 < iSelectMeasPt && iSelectMeasPt <= m_SrConfig.wGantryNumOfMeasPoint){
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
void CTestMeasurementDlg::SetPointData(SR_REFERENCE_POSITION SrReferencePosition, int iSelectMeasPt)
{
	CString csPointNo, csX, csY;

	// X
	csX.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX) / MICROMETRE);
	// Y
	csY.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY) / MICROMETRE);

	SetDlgItemText(IDC_MEAS_POINT_X, csX);
	SetDlgItemText(IDC_MEAS_POINT_Y, csY);
}

void CTestMeasurementDlg::OnHomePositonButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
// 2010.02.03 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
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
// 2010.02.03 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z ���̏�������~�ʒu
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z ���̏�������~�ʒu�t�@�N�^

	// �n�[�h�V�~�����[�g���[�h���� -------------------------------------------
	if (0 != m_pMainFrame->HardwareSimulation()) {
		// �n�[�h�V�~�����[�g���[�h�������̂ŁC���芮���҂� ��
		//m_MeasPosXYZ.lZ = 0;
		// �g�����W�V�����X�V
	}
	else {
		// ���@�̏ꍇ ---------------------------------------------------------
		// AF �v/�s�v����
		// �s�v
//			  m_dwAFValid = 0;	  // AF ����
			// (�S�_ AF �����̏ꍇ)Z ���ړ����K�v���`�F�b�N
//			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF)) {
//			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi)) {
//				// �s�v
//				// �g�����W�V�����X�V
//				if (0 != IsPointManualAdjust(m_dwPointNo)) {
//					// PMA �݂̂��K�v
//					TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
//				} else {
//					TransiEvent(EV_SR1P_MEAS_START);
//				}
//			} else {
		// �K�v(�d�l��K�v�Ȃ�)
		// �y���ړ�
		//TCHAR l_szMainRcpName[256];
		//((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// ��������~�ʒu[pulse]��[0.1um]�ɕϊ����܂�
		LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
		l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// ��������~�ʒu[pulse]���t�@�N�^[pulse/mm]
		l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]�ɕϊ�����
		long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

		NS_ConvertToStageMoveCoord(&l_lZ);		// �␳

		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ���쒆�t���O(Z ��)���I��
#if 0	//----- Kawashima 2008.12.01 ----->
//				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
		m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// �ړ��J�n���s
			//((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
			// �g�����W�V�����X�V
//					TransiEvent(EV_SR1P_ZMOVE_FAIL);
			// �ďo���g�����W�V�����Ɋ�����ʒm
//					  NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
		} else {
			// �ړ�����
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ---------- { ---------- */
			m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);   // Status Bar
#else						/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ----------			   */
			m_pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ---------- } ---------- */
//					((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to 0 position."));
//					// �g�����W�V�����X�V
//					if (0 != IsPointManualAdjust(m_dwPointNo)) {
//						// PMA �݂̂��K�v
//						TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
//					} else {
//						TransiEvent(EV_SR1P_ZMOVE_DONE);
//					}
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
	}
}

/* added 2015.03.10 hmenjo �e�X�g����ۑ� ---------- { ---------- */
void CTestMeasurementDlg::OnBtnSave()
{
	/* �\	*/
	CGridCtrl* l_pMeasListGrid = 0;
	CGridCtrl* l_pIntListGrid = 0;
	CGridCtrl* l_pRefListGrid = 0;
	m_pMeasurementGraphDlg1->TestMeasGetGrid(&l_pMeasListGrid, &l_pIntListGrid);
	m_pMeasurementGraphDlg2->TestMeasGetGrid(0, &l_pRefListGrid);
	/* l_pMeasListGrid�F��(Col)�C�s(Row)���擾	*/
	int l_iMeasListGridCnt_Col = l_pMeasListGrid->GetColumnCount();
	int l_iMeasListGridCnt_Row = l_pMeasListGrid->GetRowCount();
	/* l_pMeasListGrid�F�񍀖ڂ��m�F���ė񐔂��`�F�b�N�C��	*/
	CString l_strTmp;
	int i, j;
	for (i = 0; i < l_iMeasListGridCnt_Col; i++) {
		l_strTmp = l_pMeasListGrid->GetItemText(0, i);
		if (l_strTmp.GetLength() <= 0) {
			l_iMeasListGridCnt_Col = i;
			break;
		}
	}
	/* l_pMeasListGrid�F�P���ږڂ̃f�[�^���m�F���čs�����`�F�b�N�C��	*/
	for (j = 0; j < l_iMeasListGridCnt_Row; j++) {
		l_strTmp = l_pMeasListGrid->GetItemText(j, 1);
		if (l_strTmp.GetLength() <= 0) {
			l_iMeasListGridCnt_Row = j;
			break;
		}
	}
	/* �񐔂��Q�ȏ�̏ꍇ(�f�[�^�s�����݂���Ƃ�)�̂݉��L�����s���܂��D	*/
	if (1 < l_iMeasListGridCnt_Row) {
		/* ����v���O���������擾	*/
		RCP_DATA l_RcpData;
		memset(&l_RcpData, 0, sizeof(l_RcpData));
		m_pDoc->GetRcpData(&l_RcpData);
		/* �o�̓t�@�C��	*/
		SYSTEMTIME l_stNow;
		::GetLocalTime(&l_stNow);
		TCHAR l_tszOutFile[MAX_PATH];
		_stprintf(l_tszOutFile, _T("Test_%04d%02d%02d%02d%02d%02d_%s.txt"),
				l_stNow.wYear,
				l_stNow.wMonth,
				l_stNow.wDay,
				l_stNow.wHour,
				l_stNow.wMinute,
				l_stNow.wSecond,
				l_RcpData.MeasProgInfo.hdr.szName
			);
		TCHAR l_tszOutPath[MAX_PATH];
		_stprintf(l_tszOutPath, _T("%s%s"), g_szData_Dir, l_tszOutFile);
		FILE *l_fp = _tfopen(l_tszOutPath, _T("w"));
		if (0 != l_fp) {
			_ftprintf(l_fp, _T("Test Measurement,%04d.%02d.%02d,%02d:%02d:%02d\n"),
					l_stNow.wYear,
					l_stNow.wMonth,
					l_stNow.wDay,
					l_stNow.wHour,
					l_stNow.wMinute,
					l_stNow.wSecond
				);
			_ftprintf(l_fp, _T("%s\n"), l_RcpData.MeasProgInfo.hdr.szName);
			_fputts(_T("\n"), l_fp);
			TCHAR l_tszLine[512];
			int ix, iy;
			for (iy = 0; iy < l_iMeasListGridCnt_Row; iy++) {
				_tcscpy(l_tszLine, _T(""));
				for (ix = 0; ix < l_iMeasListGridCnt_Col; ix++) {
					if ((0 == ix) || (3 < ix)) {
						l_strTmp = l_pMeasListGrid->GetItemText(iy, ix);
						if (0 < ix) {
							_tcscat(l_tszLine, _T(","));
						}
						_tcscat(l_tszLine, l_strTmp);
					}
				}
				_tcscat(l_tszLine, _T("\n"));
				_fputts(l_tszLine, l_fp);
			}
			for (j = 1; j < l_iMeasListGridCnt_Row; j++) {
	/* ���݂̑I���s���擾	*/
		/*	���݂̑I���s�C���邢�́C���݂̕\���s���L�����Ă����āC
			�t�@�C���쐬��Ɍ��̕\���s�ɖ߂����Ǝv���܂������C
			int l_iMeasListGrid__CurRow = m_pMeasurementGraphDlg1->TestMeasGetGraphRow();
			���Ƃ߂�ǂ��Ȃ̂ŁC�ŏI�s�ɂȂ�Ƃ����d�l�ɂ��܂��D*/
				m_pMeasurementGraphDlg1->TestMeasChangeSelect(j);
				m_pMeasurementGraphDlg2->TestMeasChangeSelect(j);
				/* l_pIntListGrid�F��(Col)�C�s(Row)���擾	*/
				int l_iIntListGridCnt_Col = l_pIntListGrid->GetColumnCount();
				int l_iIntListGridCnt_Row = l_pIntListGrid->GetRowCount();
				/* l_pIntListGrid�F�񍀖ڂ��m�F���ė񐔂��`�F�b�N	*/
				for (i = 0; i < l_iIntListGridCnt_Col; i++) {
					l_strTmp = l_pIntListGrid->GetItemText(0, i);
					if (l_strTmp.GetLength() <= 0) {
						l_iIntListGridCnt_Col = i;
						break;
					}
				}
				/* l_pIntListGrid�F�w�b�_���m�F���čs�����`�F�b�N	*/
				for (i = 0; i < l_iIntListGridCnt_Row; i++) {
					l_strTmp = l_pIntListGrid->GetItemText(i, 0);
					if (l_strTmp.GetLength() <= 0) {
						l_iIntListGridCnt_Row = i;
						break;
					}
				}
				/* l_pRefListGrid�F��(Col)�C�s(Row)���擾	*/
				int l_iRefListGridCnt_Col = l_pRefListGrid->GetColumnCount();
				int l_iRefListGridCnt_Row = l_pRefListGrid->GetRowCount();
				/* l_pRefListGrid�F�񍀖ڂ��m�F���ė񐔂��`�F�b�N	*/
				for (i = 0; i < l_iRefListGridCnt_Col; i++) {
					l_strTmp = l_pRefListGrid->GetItemText(0, i);
					if (l_strTmp.GetLength() <= 0) {
						l_iRefListGridCnt_Col = i;
						break;
					}
				}
				/* l_pRefListGrid�F�w�b�_���m�F���čs�����`�F�b�N	*/
				for (i = 0; i < l_iRefListGridCnt_Row; i++) {
					l_strTmp = l_pRefListGrid->GetItemText(i, 0);
					if (l_strTmp.GetLength() <= 0) {
						l_iRefListGridCnt_Row = i;
						break;
					}
				}
				_fputts(_T("\n"), l_fp);
				_ftprintf(l_fp, _T("Ref-%04d\n"), j);
				for (iy = 0; iy < l_iIntListGridCnt_Row; iy++) {
					_tcscpy(l_tszLine, _T(""));
					for (ix = 0; ix < l_iIntListGridCnt_Col; ix++) {
						l_strTmp = l_pIntListGrid->GetItemText(iy, ix);
						if (0 < ix) {
							_tcscat(l_tszLine, _T(","));
						}
						_tcscat(l_tszLine, l_strTmp);
					}
					_tcscat(l_tszLine, _T("\n"));
					_fputts(l_tszLine, l_fp);
				}
				_fputts(_T("\n"), l_fp);
				_ftprintf(l_fp, _T("Int-%04d\n"), j);
				for (iy = 0; iy < l_iRefListGridCnt_Row; iy++) {
					_tcscpy(l_tszLine, _T(""));
					for (ix = 0; ix < l_iRefListGridCnt_Col; ix++) {
						l_strTmp = l_pRefListGrid->GetItemText(iy, ix);
						if (0 < ix) {
							_tcscat(l_tszLine, _T(","));
						}
						_tcscat(l_tszLine, l_strTmp);
					}
					_tcscat(l_tszLine, _T("\n"));
					_fputts(l_tszLine, l_fp);
				}
			}
			fclose(l_fp);
		}
	}
}
/* added 2015.03.10 hmenjo �e�X�g����ۑ� ---------- } ---------- */
