// TestMeasurementDlg.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
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
// CTestMeasurementDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O

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
/* added 2015.03.10 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
	DDX_Control(pDX, IDC_BTN_SAVE, m_BtnSave);
/* added 2015.03.10 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MEAS_INTEGRATION_TIME, m_Meas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_WAVELENGTH_START, (short&)m_Meas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_WAVELENGTH_END,	(short&)m_Meas.ScanParams._SR.WavelenRange.wEnd);
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
// CTestMeasurementDlg ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½

BOOL CTestMeasurementDlg::OnInitDialog()
{
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// ï¿½ï¿½Ê‚ï¿½ï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é��ï¿½Íƒfï¿½tï¿½Hï¿½ï¿½ï¿½gï¿½Íƒ\ï¿½tï¿½gJoyï¿½Æ‚ï¿½ï¿½ï¿½
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		// Radioï¿½{ï¿½^ï¿½ï¿½ï¿½Ì�İ’ï¿½
		m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
		m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

// 2013.01.11 bagus jog direction -->
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_JogDirSetting, CONFIG_FILE_JOG_DIR);
// 2013.01.11 bagus jog direction <--

	//
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pTestMeasDlgObj = this;
	m_iConditionFlg = TESTMODE_INIT;	//ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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

// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//	if(g_lModelType == MODEL_T3100){
	if(!m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		m_AutoFocusButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
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
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

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

// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nJoystickType == JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		GetDlgItem(IDC_Z_AXIS_STATIC)->ShowWindow(SW_HIDE);
		m_Z_UpButton.ShowWindow(SW_HIDE);
		m_Z_DownButton.ShowWindow(SW_HIDE);
		m_Z_HomePosButton.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MOTION_MODE_STATIC)->ShowWindow(SW_HIDE);
		m_cbMotionElev.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
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
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	/// Cancel Button ///
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	/// OK Button ///
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

/* added 2015.03.10 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
		m_BtnSave.ShowWindow(SW_SHOW);
		m_BtnSave.SetShade(	BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
								BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_BtnSave.DrawFlatFocus(TRUE);
	} else {
		m_BtnSave.ShowWindow(SW_HIDE);
	}
/* added 2015.03.10 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */

// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
//	if(g_lModelType == MODEL_T3100){
	if(!m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		m_AfMeasurementButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
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
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

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
		SetDlgItemInt(IDC_WAVELENGTH_START, MIN_TARGET_WAVELENGTH); //ï¿½gï¿½ï¿½(START)
// 2009.05.15 k-matsuo -->
//		SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);	//ï¿½gï¿½ï¿½(END)
		if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);
		else
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH_800);
// 2009.05.15 k-matsuo <--
	}
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);	//ï¿½gï¿½ï¿½(START)
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);	//ï¿½gï¿½ï¿½(END)

/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- { ---------- */
	RECT l_rectWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &l_rectWorkArea, 0);
	RECT l_rectTestMeasDlg;	this->GetWindowRect(&l_rectTestMeasDlg);
	if ((g_lModelType == MODEL_T3100) && (l_rectWorkArea.bottom < l_rectTestMeasDlg.bottom)) {
		/* ï¿½fï¿½Xï¿½Nï¿½gï¿½bï¿½vï¿½Ì�ï¿½ÆƒGï¿½ï¿½ï¿½Aï¿½cï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½å‚«ï¿½ï¿½ï¿½ê�‡	*/
		/* ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½{ï¿½^ï¿½ï¿½ï¿½ÌƒOï¿½ï¿½ï¿½[ï¿½vï¿½{ï¿½bï¿½Nï¿½Xï¿½Í‚ï¿½ï¿½Ì‚Ü‚Ü‚ÌƒTï¿½Cï¿½Yï¿½Æ‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
		/* ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½Ó‚ÌˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½	*/
		long l_ldYDlgBottom = l_rectTestMeasDlg.bottom - (l_rectWorkArea.bottom - 2);
		/* ï¿½ê�Ÿï¿½^ï¿½uï¿½Æƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½Ó‚Æ‚Ì‹ï¿½ï¿½ï¿½	*/
		RECT l_rectMeasTab;	m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
		/* ï¿½ê�Ÿï¿½^ï¿½uï¿½Ì’ï¿½Ó‚ÌˆÊ’uï¿½ï¿½ï¿½í‚¹	*/
		m_MeasurementTab.SetWindowPos(	0,
							0, 0,
							l_rectMeasTab.right - l_rectMeasTab.left,
							l_rectMeasTab.bottom - l_rectMeasTab.top - l_ldYDlgBottom,
							SWP_NOZORDER | SWP_NOMOVE);
		/* ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ï¿½ï¿½í‚¹	*/
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
		/* ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½Ì�cï¿½Tï¿½Cï¿½Yï¿½ï¿½ï¿½fï¿½Xï¿½Nï¿½gï¿½bï¿½vï¿½Ì�ï¿½ÆƒGï¿½ï¿½ï¿½Aï¿½cï¿½ï¿½ï¿½É�ï¿½ï¿½í‚¹ï¿½Ü‚ï¿½ï¿½D	*/
		this->SetWindowPos(	0,
							0, 0,
							l_rectTestMeasDlg.right, l_rectWorkArea.bottom - 2,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- } ---------- */

	///// Grid Control Initialize /////
	MeasurementTab_Init();

	// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Lï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½Ö�~ï¿½ï¿½ï¿½ï¿½
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
			// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
			if(!m_pMainFrame->HardwareSimulation()){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
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
			if(!m_pMainFrame->HardwareSimulation()){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
#endif
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	m_AfExecuteFlg = FALSE;
	m_AfExecuteFlgOld = FALSE;
	m_iAlarmStatus = ALARM_NON;
	m_iAlarmStatusOld = ALARM_NON;

/* deleted 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- { ---------- */
///* added 2015.03.19 hmenjo FWXGA ï¿½Î‰ï¿½ ---------- { ---------- */
//	RECT l_rectWorkArea;
//	::SystemParametersInfo(SPI_GETWORKAREA, 0, &l_rectWorkArea, 0);
//	RECT l_rectTestMeasDlg;	this->GetWindowRect(&l_rectTestMeasDlg);
//	if ((g_lModelType == MODEL_T3100) && (l_rectWorkArea.bottom < l_rectTestMeasDlg.bottom)) {
//		/* ï¿½fï¿½Xï¿½Nï¿½gï¿½bï¿½vï¿½Ì�ï¿½ÆƒGï¿½ï¿½ï¿½Aï¿½cï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½å‚«ï¿½ï¿½ï¿½ê�‡	*/
//		/* ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½jï¿½ï¿½ï¿½[ï¿½{ï¿½^ï¿½ï¿½ï¿½ÌƒOï¿½ï¿½ï¿½[ï¿½vï¿½{ï¿½bï¿½Nï¿½Xï¿½Í‚ï¿½ï¿½Ì‚Ü‚Ü‚ÌƒTï¿½Cï¿½Yï¿½Æ‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
//		/* ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½Ó‚ÌˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½	*/
//		long l_ldYDlgBottom = l_rectTestMeasDlg.bottom - (l_rectWorkArea.bottom - 2);
//		/* ï¿½ê�Ÿï¿½^ï¿½uï¿½Æƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½Ó‚Æ‚Ì‹ï¿½ï¿½ï¿½	*/
//		RECT l_rectMeasTab;	m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
//		/* ï¿½ê�Ÿï¿½^ï¿½uï¿½Ì’ï¿½Ó‚ÌˆÊ’uï¿½ï¿½ï¿½í‚¹	*/
//		m_MeasurementTab.SetWindowPos(	0,
//							0, 0,
//							l_rectMeasTab.right - l_rectMeasTab.left,
//							l_rectMeasTab.bottom - l_rectMeasTab.top - l_ldYDlgBottom,
//							SWP_NOZORDER | SWP_NOMOVE);
//		/* ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ï¿½ï¿½í‚¹	*/
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
//		/* ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½Ì�cï¿½Tï¿½Cï¿½Yï¿½ï¿½ï¿½fï¿½Xï¿½Nï¿½gï¿½bï¿½vï¿½Ì�ï¿½ÆƒGï¿½ï¿½ï¿½Aï¿½cï¿½ï¿½ï¿½É�ï¿½ï¿½í‚¹ï¿½Ü‚ï¿½ï¿½D	*/
//		this->SetWindowPos(	0,
//							0, 0,
//							l_rectTestMeasDlg.right, l_rectWorkArea.bottom - 2,
//							SWP_NOZORDER | SWP_NOMOVE);
//		m_pMeasurementGraphDlg1->ChgSizeCtrl(-l_ldYDlgBottom);
//		m_pMeasurementGraphDlg2->ChgSizeCtrl(-l_ldYDlgBottom);
//	}
/* added 2015.03.19 hmenjo FWXGA ï¿½Î‰ï¿½ ---------- } ---------- */
/* deleted 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- } ---------- */

	SetTimer(TESTMEASDLG_ENABLE_CHANGE_TIMER, 50, NULL);


	return TRUE;  // ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Éƒtï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½İ’è‚µï¿½È‚ï¿½ï¿½Æ‚ï¿½ï¿½Aï¿½ß‚ï¿½lï¿½ï¿½ TRUE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
					// ï¿½ï¿½O: OCX ï¿½vï¿½ï¿½ï¿½pï¿½eï¿½B ï¿½yï¿½[ï¿½Wï¿½Ì–ß‚ï¿½lï¿½ï¿½ FALSE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
}

// ==========================================================================
//
void CTestMeasurementDlg::OnDestroy()
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

// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
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

	if( m_pMeasurementGraphDlg1 != NULL ){
		delete m_pMeasurementGraphDlg1;
		m_pMeasurementGraphDlg1 = NULL;
	}
	if( m_pMeasurementGraphDlg2 != NULL ){
		delete m_pMeasurementGraphDlg2;
		m_pMeasurementGraphDlg2 = NULL;
	}


	m_pDoc->SetMeasMode(MEASMODE_NOT_MEASURE); //ï¿½ï¿½ï¿½è’†ï¿½Å‚È‚ï¿½
	m_pDoc->InitMeasData(); //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	DummyRcpDelete(); //ï¿½_ï¿½~ï¿½[ï¿½ï¿½ï¿½Vï¿½sï¿½í�œ

	//ï¿½Xï¿½eï¿½[ï¿½^ï¿½Xï¿½oï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

// 2010.02.01 K.Matsuo ASSERT ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
	m_pMainFrame->m_pTestMeasDlgObj = NULL;
// 2010.02.01 K.Matsuo ASSERT ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--

	CDialog::OnDestroy();
}

// =========================================================================
//
void CTestMeasurementDlg::OnOK()
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

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	/*//Saiki 20090520 Change <-----*/
	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	CString csBuff;

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_dIntegTime = atof(csBuff);

//	HookKeyUnhook();

// 2009.09.30 K.Matsuo Add -->
	///// ï¿½wï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½İŠmï¿½F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½í�œ /////
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
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½É“ï¿½ï¿½Ê‚ÈŒã�ˆï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B

//	HookKeyUnhook();

// 2009.09.30 K.Matsuo Add -->
	///// ï¿½wï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½İŠmï¿½F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½í�œ /////
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
	//m_MeasurementTab.InsertItem(0, "ï¿½ï¿½ï¿½Ë—ï¿½ï¿½fï¿½[ï¿½^"); //Reflectance Data
	//m_MeasurementTab.InsertItem(1, "ï¿½ï¿½ï¿½ï¿½ï¿½xï¿½fï¿½[ï¿½^"); //Raw Data

	CString strBuf1, strBuf2;

	LoadStringML(IDS_CAPTION_REFLECTANCE_DATA, strBuf1, "Reflectance Data");
	LoadStringML(IDS_CAPTION_RAW_DATA, strBuf2, "Raw Data");
	m_MeasurementTab.InsertItem(0, strBuf1); //Reflectance Data
	m_MeasurementTab.InsertItem(1, strBuf2); //Raw Data
	//Saiki 20090528 Change <-----
	///// List	/////
	int dx = 5;
	int dy = 24;

	///// Graph(ï¿½Ê�ï¿½) /////
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

	//ï¿½ï¿½ï¿½Vï¿½sï¿½Ç�ï¿½ï¿½ï¿½

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	RecipeFile_LoadRecipe(&m_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE); //
	RecipeFile_LoadRecipe(&m_Meas, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
}

// =========================================================================
//ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Cï¿½jï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
// ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½
//
void CTestMeasurementDlg::OnSelchangeSrLens()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----
	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Ù�ï¿½
	}
	else{												// ï¿½Ê�í“®ï¿½ï¿½
	}
	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½Vï¿½ï¿½ï¿½bï¿½^ï¿½[ï¿½ÈŠOï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ç“®ï¿½ï¿½Ö�~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½

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

// 2013.12.18 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	BOOL bUseFilter;

	if(g_lModelType != MODEL_T3100){
		bUseFilter = m_SrConfig.bAutoFilter;	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½ÌƒRï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½İ’ï¿½Ì“ï¿½ï¿½e
	}
	else{
		bUseFilter = TRUE;	// T3100ï¿½Å‚Í�Aï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½İ’ï¿½ï¿½ï¿½ï¿½É�gï¿½pï¿½iï¿½\ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½ï¿½
	}
// 2013.12.18 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

// 2013.12.18 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
//	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½Ì�ê�‡
//	if (m_SrConfig.bAutoFilter){
	// ï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½İ’ï¿½ï¿½ï¿½gï¿½pï¿½ï¿½ï¿½ï¿½ê�‡
	if (bUseFilter){
// 2013.12.18 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
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

	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½È‚ï¿½ï¿½Ì�ê�‡
	else{
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
// ï¿½Iï¿½vï¿½eï¿½Bï¿½Jï¿½ï¿½ï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[ ï¿½Rï¿½ï¿½ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½
//
void CTestMeasurementDlg::OnSelchangeSrOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(nSelect));

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½

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
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
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
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			m_XY_UpButton.ShowWindow(SW_HIDE);
			m_XY_DownButton.ShowWindow(SW_HIDE);
			m_XY_RightButton.ShowWindow(SW_HIDE);
			m_XY_LeftButton.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);
		}
		else{
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
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
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
		}
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
// 2009.10.29 bagus Jog ï¿½Ç‰ï¿½ï¿½Cï¿½ï¿½ --{--
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
// 2009.10.29 bagus Jog ï¿½Ç‰ï¿½ï¿½Cï¿½ï¿½ --}--
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

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
	//2009.12.11 bagus ï¿½Cï¿½ï¿½ --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//pCombo->AddString("<Select>");
	pCombo->AddString(strSelectCaption);
	//2009.12.11 bagus ï¿½Cï¿½ï¿½ --{--
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
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return 0L;
		}	// ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†
		//Saiki 20090601 Change <-----
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
		}	// ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

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
			// H/Wï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
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
			// H/Wï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
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
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strBuffer, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strBuffer);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	BOOL bJoyStk;

	m_AfExecuteFlg = TRUE;

	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½gï¿½pï¿½sï¿½Â‚É�İ’ï¿½
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
	}

	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	if( NS_StageDoAutoFocus()==FALSE ){
//		m_AutoFocusResult	 = 1;	// AutoFocus Result NG
		//Saiki 20090528 Change ----->
		//MessageBox("ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½s!!!", "AUTO FOCUS", MB_OK|MB_ICONWARNING);
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

	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½gï¿½pï¿½Â‚É�İ’ï¿½
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
	}

	m_AfExecuteFlg = FALSE;

}


// ==========================================================================
//
void CTestMeasurementDlg::OnReferenceButton()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	//Saiki 20090530 Add ----->
	CString strMsg, strTitle;
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
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		//Saiki 20090601 Change ----->
		//MessageBox(	"ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½",	"ï¿½xï¿½ï¿½", MB_OK);
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	//Saiki 20090528 Change ----->
	//char* pszCaption = "ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Jï¿½n";
	//char* pszCaption;
	CString strBuffer;
	//Saiki 20090528 Change <-----


	m_pDoc->GetRcpData(&rcp_data);


	///// Clear Reference /////
	ClearReference(rcp_data.szRecipeName);


	//ï¿½ï¿½ï¿½ï¿½ï¿½Yï¿½İ’ï¿½
	UINT uiLens = m_Meas.ScanParams._SR.iLens;
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(uiLens);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	//ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½
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
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
// ADD 2009.06.01
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
		return;
	}

/* modified 2017.01.13 hmenjo ï¿½eï¿½Xï¿½gï¿½nï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- { ---------- */
//	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");
/* modified 2017.01.13 hmenjo ï¿½eï¿½Xï¿½gï¿½nï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ----------              */
	m_pDoc->OperationLogging("Test Meas. Reference Button was push.");
/* modified 2017.01.13 hmenjo ï¿½eï¿½Xï¿½gï¿½nï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- } ---------- */

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½gï¿½pï¿½sï¿½Â‚É�İ’ï¿½
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnReferenceButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
	}

// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
			CExecutionDlg dlg;
//			dlg.SetCaption(EXECUTION_CAPTION);
			LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
			dlg.SetCaption(strBuffer);
			//Saiki 20090128 Change <-----
			//Saiki 20090528 Change ----->
			//dlg.SetMessage("ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½è’†...");
			LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
			dlg.SetMessage(strBuffer);
			//Saiki 20090528 Change <-----
			dlg.DoModal();
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

	m_iConditionFlg = TESTMODE_REF_AFTER;	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½Ìƒï¿½ï¿½[ï¿½_ï¿½ï¿½ï¿½Ìƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½{ï¿½bï¿½Nï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ü‚Å”ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½)

	return;

}


// ==========================================================================
//
void CTestMeasurementDlg::OnMeasurementButton()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle, strMsg;
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
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE/*_Ed*/)
	{
		// Kojika 20090529 Change
		//MessageBox("ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½B\nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½æ“¾ï¿½ï¿½É�Ä“xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B", "NanoSpec", MB_OK);
		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
		return;
	}


	//ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 0;
	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
	{
		// Kojika 20090529 Change
		//MessageBox("ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½sï¿½tï¿½ï¿½ï¿½Oï¿½Ì�Xï¿½Vï¿½É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½", "NanoSpec", MB_OK);
		LoadStringML(IDS_FAILED_UPDATE_REFER_EXE, l_strBuffer, "It failed in the update of the reference execution flag");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}
	m_pDoc->SetRcpData(&rcp_data);

/* added 2017.01.13 hmenjo ï¿½eï¿½Xï¿½gï¿½nï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- { ---------- */
	m_pDoc->OperationLogging("Test Meas. Measurement Button was push.");
/* added 2017.01.13 hmenjo ï¿½eï¿½Xï¿½gï¿½nï¿½ï¿½ï¿½Oï¿½Cï¿½ï¿½ ---------- } ---------- */

	StartTestModeMeas();

}


//Saiki 20130123 change --->
// ==========================================================================
//
//void CTestMeasurementDlg::OnAfAndMeasurementButton()
//{
//	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//
//	// Kojika 20090529 Add
//	CString l_strBuffer, l_strTitle, strMsg;
//	// Kojika 20090529 Add End
//
//	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
//
//
//	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//	//ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½`ï¿½Fï¿½bï¿½N(2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ì�§ï¿½ï¿½)
//	if( nexioIsEngineerMaintenanceSwitch()==ON )// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h
//	{
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½gï¿½İ�ï¿½ï¿½í‚¹ï¿½Gï¿½ï¿½ï¿½[
//		/*ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½[ï¿½hï¿½Ì�ï¿½ï¿½Í�Aï¿½Kï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½h(L)ï¿½Æ‚È‚ï¿½
//			ï¿½ï¿½ï¿½Ì�ê�‡ï¿½A2ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â”\ */
//	}
//	else //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½[ï¿½hï¿½ÈŠO
//	{
//	}
//
//	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
//	/*//Saiki 20090520 Change ----->*/
//	if(m_pDoc->IsInterLock() == TRUE){return;}
//	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
//	/*//Saiki 20090520 Change <-----*/
//
//	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
//	//Saiki 20090601 Change ----->
//	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
//		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
//		m_pDoc->MessageStringIf_Set(strMsg);
//		return;
//	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
//	//Saiki 20090601 Change <-----
//	/*//Saiki 20090520 Change ----->*/
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
//	/*//Saiki 20090520 Change <-----*/
//	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//
//
//	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE)
//	{
//		// Kojika 20090529 Change
//		//MessageBox("ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½B\nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½æ“¾ï¿½ï¿½É�Ä“xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B", "NanoSpec", MB_OK);
//		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
//		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
//		MessageBox(l_strBuffer, l_strTitle, MB_OK);
//		// Kojika 20090529 Change End
//		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
//		return;
//	}
//
//
//	//ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//	RCP_DATA rcp_data;
//	memset(&rcp_data, 0, sizeof(rcp_data));
//	m_pDoc->GetRcpData(&rcp_data);
//	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 1;
//	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
//	{
//		// Kojika 20090529 Change
//		//MessageBox("ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½sï¿½tï¿½ï¿½ï¿½Oï¿½Ì�Xï¿½Vï¿½É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½", "NanoSpec", MB_OK);
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
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, l_strBuffer, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(l_strBuffer);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	BOOL bJoyStk;

	m_AfExecuteFlg = TRUE;

	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½gï¿½pï¿½sï¿½Â‚É�İ’ï¿½
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
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

	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½gï¿½pï¿½Â‚É�İ’ï¿½
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
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


	//ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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


	//ï¿½ï¿½ï¿½è�”ï¿½ï¿½ï¿½ï¿½
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


	//ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½
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


	///// ï¿½`ï¿½[ï¿½tï¿½Ö‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ğ‘—�M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

// ï¿½ê��ï¿½Iï¿½Éƒnï¿½[ï¿½hJoyStickï¿½ï¿½ï¿½Eï¿½ï¿½
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //ï¿½ï¿½ï¿½ï¿½ï¿½

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
	//char* pszCaption = "ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½Jï¿½n";
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	//ï¿½ï¿½ï¿½è�”ï¿½ï¿½ï¿½ï¿½
	if(m_pDoc->GetStoreCount() >= TESTDATA_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
		// Kojika 20090529 Change
		//sprintf(szMsgBuff, "ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ %d ï¿½ğ’´‚ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½!", TESTDATA_POINT_MAX);
		LoadStringML(IDS_MEASURE_DATA_NUMBER_EXCEED, l_strBuffer, "Measurement data of number %d is exceeded.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		sprintf(szMsgBuff, l_strBuffer, TESTDATA_POINT_MAX);
		MessageBox(szMsgBuff, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}


	//ï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½
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


	///// ï¿½`ï¿½[ï¿½tï¿½Ö‘ï¿½ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ğ‘—�M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

#if 1
// ADD 2009.06.02 H.Kawamura
// ï¿½ê��ï¿½Iï¿½Éƒnï¿½[ï¿½hJoyStickï¿½ï¿½ï¿½Eï¿½ï¿½
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();

		return;
	}
// ADD 2009.06.02 H.Kawamura
#endif

/* added 2009.12.02 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo ï¿½ï¿½ï¿½ï¿½ Seq ï¿½Í�wï¿½ï¿½wï¿½bï¿½hï¿½Å“ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½) ---------- } ---------- */
/* modified 2009.11.27 hmenjo ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Å‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM)rcp_data.szRecipeName, 0); //1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½
/* modified 2009.11.27 hmenjo ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Å‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ ----------			   */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½
/* modified 2009.11.27 hmenjo ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Å‘ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Ô�ï¿½ ---------- } ---------- */

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	// Kojika 20090529 Change
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");

#if 1
// ADD 2009.06.02 H.Kawamura
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.02 H.Kawamura
#endif

		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

#if 0
// DEL 2009.06.02 H.Kawamura
// ï¿½ê��ï¿½Iï¿½Éƒnï¿½[ï¿½hJoyStickï¿½ï¿½ï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É’uï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½gï¿½pï¿½sï¿½Â‚É�İ’ï¿½
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnMeasurementButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
	}
// DEL 2009.06.02 H.Kawamura
#endif

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //ï¿½ï¿½ï¿½ï¿½ï¿½
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
//ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);

	return 0L;
}



// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnTestModeRefEnd(WPARAM wParam, LPARAM lParam)
{
//ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);

	return 0L;
}


// =========================================================================
//ï¿½_ï¿½~ï¿½[ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ã�‘ï¿½ï¿½
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
//ï¿½_ï¿½~ï¿½[ï¿½ï¿½ï¿½Vï¿½sï¿½í�œ
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


// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
// =========================================================================
//
void CTestMeasurementDlg::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½N			ï¿½ï¿½ï¿½g */
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
			if ( m_joyStick2->GetCheck() ) {
				// ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½Éƒ\ï¿½tï¿½gJoyï¿½Ö�Ø‚ï¿½Ö‚ï¿½ï¿½ï¿½
				m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
				m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				m_joyStick1->Checked();
				m_joyStick2->UnChecked();
//				m_joyStick1->SetFocus();		// ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½ï¿½Ìƒ`ï¿½Fï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚È‚ï¿½ï¿½È‚ï¿½ï¿½è‚ªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ÅƒRï¿½ï¿½ï¿½ï¿½ï¿½gï¿½Aï¿½Eï¿½gï¿½Bï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½B
			}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	}
}
// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--

void CTestMeasurementDlg::OnTimer(UINT nIDEvent)
{
BOOL bJoyStk;

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iAlarmStatus = m_pDoc->GetAlarmStatus();

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

	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld))
	{
//================
		//ï¿½nï¿½[ï¿½hï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ì�§ï¿½ï¿½(ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ï¿½)
		if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_EXIST))
		{
			// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
		else if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_NON))
		{
			// H/Wï¿½Lï¿½ï¿½ï¿½È‚ï¿½
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
			}
		}

//================
		//ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½Enableï¿½ï¿½ï¿½ï¿½(ï¿½Pï¿½ÆƒIï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½ì��ï¿½ÈŠO)
		switch(nIDEvent)
		{
			case  TESTMEASDLG_ENABLE_CHANGE_TIMER:

				if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_INIT)||(m_iConditionFlg == TESTMODE_CONDITION_CANGE_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //ï¿½wWAITï¿½x ï¿½ï¿½ï¿½ï¿½ (ï¿½wï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½x ï¿½Ü‚ï¿½ï¿½ï¿½ ï¿½wï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½x) ï¿½ï¿½ï¿½ï¿½ ï¿½wï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½È‚ï¿½ï¿½x
					//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //ï¿½ï¿½ï¿½ï¿½ï¿½Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//ï¿½ï¿½ï¿½ï¿½ï¿½Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//ï¿½ï¿½ï¿½wï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½è��ï¿½ï¿½

					//ï¿½ï¿½ï¿½ï¿½n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½^ï¿½ï¿½ï¿½ï¿½
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);		//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½X
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

					//ï¿½Xï¿½eï¿½[ï¿½W
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½[ï¿½h(XYï¿½Xï¿½eï¿½[ï¿½W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½[ï¿½h(Zï¿½ï¿½)
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					//ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[Save]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
				}
				else if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_REF_AFTER)||(m_iConditionFlg == TESTMODE_MEAS_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //ï¿½wWAITï¿½x ï¿½ï¿½ï¿½ï¿½ (ï¿½wï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½x ï¿½Ü‚ï¿½ï¿½ï¿½ ï¿½wï¿½ï¿½ï¿½ï¿½ï¿½x) ï¿½ï¿½ï¿½ï¿½ ï¿½wï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½È‚ï¿½ï¿½x
					//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //ï¿½ï¿½ï¿½ï¿½ï¿½Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//ï¿½ï¿½ï¿½ï¿½ï¿½Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//ï¿½ï¿½ï¿½wï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½è��ï¿½ï¿½

					//ï¿½ï¿½ï¿½ï¿½n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(TRUE); 	//ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½^ï¿½ï¿½ï¿½ï¿½
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);		//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½X
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

					//ï¿½Xï¿½eï¿½[ï¿½W
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½[ï¿½h(XYï¿½Xï¿½eï¿½[ï¿½W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½[ï¿½h(Zï¿½ï¿½)
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					//ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[sSave]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
				}
				else if((m_iProcessStatus == PROCESS_DOWN) || (m_iAlarmStatus != ALARM_NON))
				{ //ï¿½wDOWNï¿½x ï¿½Ü‚ï¿½ï¿½ï¿½ ï¿½wï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½x
					//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //ï¿½ï¿½ï¿½ï¿½ï¿½Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//ï¿½ï¿½ï¿½ï¿½ï¿½Y
					}
					//Saiki 20090908 Change <-----
					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//ï¿½ï¿½ï¿½wï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//ï¿½ï¿½ï¿½è��ï¿½ï¿½

					//ï¿½ï¿½ï¿½ï¿½n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½^ï¿½ï¿½ï¿½ï¿½
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½X
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

					//ï¿½Xï¿½eï¿½[ï¿½W
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 	//ï¿½ï¿½ï¿½[ï¿½h(XYï¿½Xï¿½eï¿½[ï¿½W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(FALSE);		//ï¿½ï¿½ï¿½[ï¿½h(Zï¿½ï¿½)
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					//ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[Save]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
				}
				else if((m_iProcessStatus == PROCESS_INIT)
					||(m_iProcessStatus == PROCESS_PROC)
					||(m_iProcessStatus == PROCESS_ABRT)
					||(m_iProcessStatus == PROCESS_COMP))
				{ //ï¿½wINITï¿½xï¿½Ü‚ï¿½ï¿½Í�wPROCï¿½xï¿½Ü‚ï¿½ï¿½Í�wABRTï¿½xï¿½Ü‚ï¿½ï¿½Í�wCOMPï¿½x
					//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE); 			   //ï¿½ï¿½ï¿½ï¿½ï¿½Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);				 //ï¿½ï¿½ï¿½ï¿½ï¿½Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(FALSE); 		//ï¿½ï¿½ï¿½wï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(FALSE); //ï¿½ï¿½ï¿½è��ï¿½ï¿½

					//ï¿½ï¿½ï¿½ï¿½n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½^ï¿½ï¿½ï¿½ï¿½
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½X
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

					//ï¿½Xï¿½eï¿½[ï¿½W
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 	//ï¿½ï¿½ï¿½[ï¿½h(XYï¿½Xï¿½eï¿½[ï¿½W)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(FALSE);		//ï¿½ï¿½ï¿½[ï¿½h(Zï¿½ï¿½)
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
					}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
					//ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(FALSE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
					m_BtnSave.EnableWindow(FALSE);								//[Save]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
				}
				else
				{
					//ï¿½Rï¿½Rï¿½É‚Í‚ï¿½ï¿½È‚ï¿½ï¿½Í‚ï¿½
				}

				break;
		}
	}

//================

	//ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½Enableï¿½ï¿½ï¿½ï¿½(ï¿½Pï¿½ÆƒIï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½ï¿½ì��)
	if(m_AfExecuteFlg != m_AfExecuteFlgOld)
	{
		//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½
		GetDlgItem(IDC_SR_LENS)->EnableWindow(!m_AfExecuteFlg); 				//ï¿½ï¿½ï¿½ï¿½ï¿½Y
		if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
		{
			GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(!m_AfExecuteFlg);		//ï¿½ï¿½ï¿½wï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½[
		}
		GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(!m_AfExecuteFlg);	//ï¿½ï¿½ï¿½è��ï¿½ï¿½

		//ï¿½ï¿½ï¿½ï¿½n
		GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//ï¿½ï¿½ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(!m_AfExecuteFlg);	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½Xï¿½^ï¿½ï¿½ï¿½ï¿½
			GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//ï¿½Iï¿½[ï¿½gï¿½tï¿½Hï¿½[ï¿½Jï¿½X
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

		//ï¿½Xï¿½eï¿½[ï¿½W
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(!m_AfExecuteFlg);		//ï¿½ï¿½ï¿½[ï¿½h(XYï¿½Xï¿½eï¿½[ï¿½W)
			GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(!m_AfExecuteFlg);		//ï¿½ï¿½ï¿½[ï¿½h(Zï¿½ï¿½)
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		//ï¿½ï¿½ï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½

		//ï¿½Iï¿½ï¿½
		GetDlgItem(IDOK)->EnableWindow(!m_AfExecuteFlg);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
		GetDlgItem(IDCANCEL)->EnableWindow(!m_AfExecuteFlg);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
		m_BtnSave.EnableWindow(!m_AfExecuteFlg);								//[Save]ï¿½{ï¿½^ï¿½ï¿½
/* added 2015.03.21 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
	}


#if 1 //ProcessStatusï¿½ï¿½ActuateFlagsï¿½ğ•¹—p
//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½ï¿½Ò�Wï¿½ï¿½ï¿½Aï¿½Eï¿½Fï¿½Cï¿½gï¿½ÈŠOï¿½Aï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½AAFï¿½ï¿½ï¿½sï¿½ï¿½ ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½B
	//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld)||(m_AfExecuteFlg != m_AfExecuteFlgOld))
	{
		if((m_iConditionFlg == TESTMODE_CONDITION_CANGE_EDIT) || ( m_iProcessStatus != PROCESS_WAIT ) || (m_iAlarmStatus == ALARM_EXIST) || (m_AfExecuteFlg == TRUE))
		{
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
			if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
				SoftJoyStickXyzEnable(FALSE); //ï¿½ï¿½ï¿½ï¿½Ö�~

				//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
				GetDlgItem(IDC_JOYSTICK1)->EnableWindow(FALSE);
				GetDlgItem(IDC_JOYSTICK2)->EnableWindow(FALSE);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
			}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		}
		else
		{
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
			if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
				SoftJoyStickXyzEnable(TRUE); //ï¿½ï¿½ï¿½ì‹–ï¿½ï¿½

				//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
				GetDlgItem(IDC_JOYSTICK1)->EnableWindow(TRUE);
				GetDlgItem(IDC_JOYSTICK2)->EnableWindow(TRUE);
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
			}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
		}
	}
#endif

//================
	//ï¿½Oï¿½ï¿½lï¿½Xï¿½V
	m_iConditionFlgOld = m_iConditionFlg;
	m_iProcessStatusOld = m_iProcessStatus;
	m_iAlarmStatusOld = m_iAlarmStatus;
	m_AfExecuteFlgOld = m_AfExecuteFlg;

//================

	CDialog::OnTimer(nIDEvent);
}



void CTestMeasurementDlg::OnSetfocusSrLens()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementDlg::OnKillfocusSrLens()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½
}

void CTestMeasurementDlg::OnSetfocusSrOptFilter()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementDlg::OnKillfocusSrOptFilter()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½
}

void CTestMeasurementDlg::OnSetfocusMeasIntegrationTime()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementDlg::OnKillfocusMeasIntegrationTime()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_Meas.ScanParams._SR.dIntegTime = atof(csBuff);

	m_pDoc->SetRcpData(&rcp_data);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½

}

void CTestMeasurementDlg::OnSetfocusWavelengthStart()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementDlg::OnKillfocusWavelengthStart()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

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
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½

}

void CTestMeasurementDlg::OnSetfocusWavelengthEnd()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementDlg::OnKillfocusWavelengthEnd()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ---------- { ---------- */
//void CTestMeasurementDlg::OnJoyStick()
/* modified 2009.06.15 hmenjo ï¿½ï¿½ï¿½[ï¿½Uï¿½ï¿½`ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ IF ï¿½Cï¿½ï¿½ ----------			   */
LRESULT CTestMeasurementDlg::OnJoyStick(WPARAM wparam, LPARAM lparam)
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
			// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
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
		//strBuffer.Format("ï¿½ï¿½ï¿½è��ï¿½Ô‚ï¿½ï¿½ÍˆÍŠOï¿½É�İ’è‚³ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½\n(%.3f - %.3f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
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
// ï¿½ï¿½ï¿½ï¿½ì‚³ï¿½ï¿½ï¿½
// ï¿½ï¿½ï¿½è’†ï¿½È‚Ç‚Ì�ï¿½ï¿½ï¿½ï¿½Í•ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½Ì‚Å’Ç‹Lï¿½è‚¢ï¿½Ü‚ï¿½

//ï¿½ï¿½ï¿½wSoftJoyStickXyzEnable()ï¿½Ì’ï¿½ï¿½xï¿½É‹Lï¿½qï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½B hibino

	// XYï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
			if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ){
				//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
				m_XY_UpButton.SetEnabled(FALSE);
				m_XY_DownButton.SetEnabled(FALSE);
				m_XY_RightButton.SetEnabled(FALSE);
				m_XY_LeftButton.SetEnabled(FALSE);
			}
			else{
				//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Lï¿½ï¿½
				m_XY_UpButton.SetEnabled(TRUE);
				m_XY_DownButton.SetEnabled(TRUE);
				m_XY_RightButton.SetEnabled(TRUE);
				m_XY_LeftButton.SetEnabled(TRUE);
			}
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½N



	// Zï¿½ï¿½ï¿½{ï¿½^ï¿½ï¿½
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		if( m_pDoc->ActuateFlagsGet( ACTUATE_ZAXIS ) ){
			m_Z_UpButton.SetEnabled(FALSE);
			m_Z_DownButton.SetEnabled(FALSE);
		}
		else{
			m_Z_UpButton.SetEnabled(TRUE);
			m_Z_DownButton.SetEnabled(TRUE);
		}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
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
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) -->
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage Noneï¿½Î‰ï¿½) <--
		m_XY_UpButton.SetEnabled(bEnable);
		m_XY_DownButton.SetEnabled(bEnable);
		m_XY_RightButton.SetEnabled(bEnable);
		m_XY_LeftButton.SetEnabled(bEnable);
		m_Z_UpButton.SetEnabled(bEnable);
		m_Z_DownButton.SetEnabled(bEnable);
		//Saiki 20090908 Add ----->
		m_Z_HomePosButton.SetEnabled(bEnable);
		//Saiki 20090908 Add <-----
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	}
// 2013.11.08 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--
}

// =========================================================================
//
void CTestMeasurementDlg::OnSelchangeComboMeasPoint()
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

	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½[ï¿½h
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	iSelectMeasPt = pCombo->GetCurSel();
	if(0 < iSelectMeasPt && iSelectMeasPt <= m_SrConfig.wGantryNumOfMeasPoint){
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
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
// 2010.02.03 K.Matsuo PIN UPPER ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½`ï¿½Fï¿½bï¿½Nï¿½Rï¿½ï¿½Î‰ï¿½ -->
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
// 2010.02.03 K.Matsuo PIN UPPER ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½`ï¿½Fï¿½bï¿½Nï¿½Rï¿½ï¿½Î‰ï¿½ <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½Ê’u
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½Ê’uï¿½tï¿½@ï¿½Nï¿½^

	// ï¿½nï¿½[ï¿½hï¿½Vï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½ -------------------------------------------
	if (0 != m_pMainFrame->HardwareSimulation()) {
		// ï¿½nï¿½[ï¿½hï¿½Vï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�Cï¿½ï¿½ï¿½èŠ®ï¿½ï¿½ï¿½Ò‚ï¿½ ï¿½ï¿½
		//m_MeasPosXYZ.lZ = 0;
		// ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½V
	}
	else {
		// ï¿½ï¿½ï¿½@ï¿½Ì�ê�‡ ---------------------------------------------------------
		// AF ï¿½v/ï¿½sï¿½vï¿½ï¿½ï¿½ï¿½
		// ï¿½sï¿½v
//			  m_dwAFValid = 0;	  // AF ï¿½ï¿½ï¿½ï¿½
			// (ï¿½Sï¿½_ AF ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡)Z ï¿½ï¿½ï¿½Ú“ï¿½ï¿½ï¿½ï¿½Kï¿½vï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
//			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF)) {
//			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi)) {
//				// ï¿½sï¿½v
//				// ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½V
//				if (0 != IsPointManualAdjust(m_dwPointNo)) {
//					// PMA ï¿½Ì‚İ‚ï¿½ï¿½Kï¿½v
//					TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
//				} else {
//					TransiEvent(EV_SR1P_MEAS_START);
//				}
//			} else {
		// ï¿½Kï¿½v(ï¿½dï¿½lï¿½ï¿½Kï¿½vï¿½È‚ï¿½)
		// ï¿½yï¿½ï¿½ï¿½Ú“ï¿½
		//TCHAR l_szMainRcpName[256];
		//((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½Ê’u[pulse]ï¿½ï¿½[0.1um]ï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½Ü‚ï¿½
		LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
		l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½Ê’u[pulse]ï¿½ï¿½ï¿½tï¿½@ï¿½Nï¿½^[pulse/mm]
		l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]ï¿½É•ÏŠï¿½ï¿½ï¿½ï¿½ï¿½
		long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

		NS_ConvertToStageMoveCoord(&l_lZ);		// ï¿½â�³

		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ï¿½ï¿½ï¿½ì’†ï¿½tï¿½ï¿½ï¿½O(Z ï¿½ï¿½)ï¿½ï¿½ï¿½Iï¿½ï¿½
#if 0	//----- Kawashima 2008.12.01 ----->
//				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
		m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// ï¿½Ú“ï¿½ï¿½Jï¿½nï¿½ï¿½ï¿½s
			//((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
			// ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½V
//					TransiEvent(EV_SR1P_ZMOVE_FAIL);
			// ï¿½Ä�oï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ÉŠï¿½ï¿½ï¿½ï¿½ï¿½Ê’m
//					  NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
		} else {
			// ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ ---------- { ---------- */
			m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);   // Status Bar
#else						/* modified 2009.08.05 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ ----------			   */
			m_pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ ---------- } ---------- */
//					((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to 0 position."));
//					// ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½V
//					if (0 != IsPointManualAdjust(m_dwPointNo)) {
//						// PMA ï¿½Ì‚İ‚ï¿½ï¿½Kï¿½v
//						TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
//					} else {
//						TransiEvent(EV_SR1P_ZMOVE_DONE);
//					}
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ï¿½ï¿½ï¿½ì’†ï¿½tï¿½ï¿½ï¿½O(Z ï¿½ï¿½)ï¿½ï¿½ï¿½Iï¿½t
	}
}

/* added 2015.03.10 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- { ---------- */
void CTestMeasurementDlg::OnBtnSave()
{
	/* ï¿½\	*/
	CGridCtrl* l_pMeasListGrid = 0;
	CGridCtrl* l_pIntListGrid = 0;
	CGridCtrl* l_pRefListGrid = 0;
	m_pMeasurementGraphDlg1->TestMeasGetGrid(&l_pMeasListGrid, &l_pIntListGrid);
	m_pMeasurementGraphDlg2->TestMeasGetGrid(0, &l_pRefListGrid);
	/* l_pMeasListGridï¿½Fï¿½ï¿½(Col)ï¿½Cï¿½s(Row)ï¿½ï¿½ï¿½æ“¾	*/
	int l_iMeasListGridCnt_Col = l_pMeasListGrid->GetColumnCount();
	int l_iMeasListGridCnt_Row = l_pMeasListGrid->GetRowCount();
	/* l_pMeasListGridï¿½Fï¿½ñ�€–Ú‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ä—ñ�”‚ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½Cï¿½ï¿½	*/
	CString l_strTmp;
	int i, j;
	for (i = 0; i < l_iMeasListGridCnt_Col; i++) {
		l_strTmp = l_pMeasListGrid->GetItemText(0, i);
		if (l_strTmp.GetLength() <= 0) {
			l_iMeasListGridCnt_Col = i;
			break;
		}
	}
	/* l_pMeasListGridï¿½Fï¿½Pï¿½ï¿½ï¿½Ú–Ú‚Ìƒfï¿½[ï¿½^ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ä�sï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½Cï¿½ï¿½	*/
	for (j = 0; j < l_iMeasListGridCnt_Row; j++) {
		l_strTmp = l_pMeasListGrid->GetItemText(j, 1);
		if (l_strTmp.GetLength() <= 0) {
			l_iMeasListGridCnt_Row = j;
			break;
		}
	}
	/* ï¿½ñ�”‚ï¿½ï¿½Qï¿½È�ï¿½Ì�ê�‡(ï¿½fï¿½[ï¿½^ï¿½sï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½Æ‚ï¿½)ï¿½Ì‚İ‰ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	if (1 < l_iMeasListGridCnt_Row) {
		/* ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ“¾	*/
		RCP_DATA l_RcpData;
		memset(&l_RcpData, 0, sizeof(l_RcpData));
		m_pDoc->GetRcpData(&l_RcpData);
		/* ï¿½oï¿½Íƒtï¿½@ï¿½Cï¿½ï¿½	*/
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
	/* ï¿½ï¿½ï¿½İ‚Ì‘Iï¿½ï¿½ï¿½sï¿½ï¿½ï¿½æ“¾	*/
		/*	ï¿½ï¿½ï¿½İ‚Ì‘Iï¿½ï¿½ï¿½sï¿½Cï¿½ï¿½ï¿½é‚¢ï¿½Í�Cï¿½ï¿½ï¿½İ‚Ì•\ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ä�C
			ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ì�¬ï¿½ï¿½ÉŒï¿½ï¿½Ì•\ï¿½ï¿½ï¿½sï¿½É–ß‚ï¿½ï¿½ï¿½ï¿½Æ�vï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½C
			int l_iMeasListGrid__CurRow = m_pMeasurementGraphDlg1->TestMeasGetGraphRow();
			ï¿½ï¿½ï¿½Æ‚ß‚ï¿½Ç‚ï¿½ï¿½È‚Ì‚Å�Cï¿½Å�Iï¿½sï¿½É‚È‚ï¿½Æ‚ï¿½ï¿½ï¿½ï¿½dï¿½lï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
				m_pMeasurementGraphDlg1->TestMeasChangeSelect(j);
				m_pMeasurementGraphDlg2->TestMeasChangeSelect(j);
				/* l_pIntListGridï¿½Fï¿½ï¿½(Col)ï¿½Cï¿½s(Row)ï¿½ï¿½ï¿½æ“¾	*/
				int l_iIntListGridCnt_Col = l_pIntListGrid->GetColumnCount();
				int l_iIntListGridCnt_Row = l_pIntListGrid->GetRowCount();
				/* l_pIntListGridï¿½Fï¿½ñ�€–Ú‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ä—ñ�”‚ï¿½ï¿½`ï¿½Fï¿½bï¿½N	*/
				for (i = 0; i < l_iIntListGridCnt_Col; i++) {
					l_strTmp = l_pIntListGrid->GetItemText(0, i);
					if (l_strTmp.GetLength() <= 0) {
						l_iIntListGridCnt_Col = i;
						break;
					}
				}
				/* l_pIntListGridï¿½Fï¿½wï¿½bï¿½_ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ä�sï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N	*/
				for (i = 0; i < l_iIntListGridCnt_Row; i++) {
					l_strTmp = l_pIntListGrid->GetItemText(i, 0);
					if (l_strTmp.GetLength() <= 0) {
						l_iIntListGridCnt_Row = i;
						break;
					}
				}
				/* l_pRefListGridï¿½Fï¿½ï¿½(Col)ï¿½Cï¿½s(Row)ï¿½ï¿½ï¿½æ“¾	*/
				int l_iRefListGridCnt_Col = l_pRefListGrid->GetColumnCount();
				int l_iRefListGridCnt_Row = l_pRefListGrid->GetRowCount();
				/* l_pRefListGridï¿½Fï¿½ñ�€–Ú‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ä—ñ�”‚ï¿½ï¿½`ï¿½Fï¿½bï¿½N	*/
				for (i = 0; i < l_iRefListGridCnt_Col; i++) {
					l_strTmp = l_pRefListGrid->GetItemText(0, i);
					if (l_strTmp.GetLength() <= 0) {
						l_iRefListGridCnt_Col = i;
						break;
					}
				}
				/* l_pRefListGridï¿½Fï¿½wï¿½bï¿½_ï¿½ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½Ä�sï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N	*/
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
/* added 2015.03.10 hmenjo ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½ï¿½Û‘ï¿½ ---------- } ---------- */
