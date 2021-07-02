// TestMeasurementDlg.cpp : インプリメンテーション ファイル
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
// CTestMeasurementDlg ダイアログ

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
/* added 2015.03.10 hmenjo テスト測定保存 ---------- { ---------- */
	DDX_Control(pDX, IDC_BTN_SAVE, m_BtnSave);
/* added 2015.03.10 hmenjo テスト測定保存 ---------- } ---------- */
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MEAS_INTEGRATION_TIME, m_Meas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_WAVELENGTH_START, (short&)m_Meas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_WAVELENGTH_END,	(short&)m_Meas.ScanParams._SR.WavelenRange.wEnd);
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
// CTestMeasurementDlg メッセージ ハンドラ

BOOL CTestMeasurementDlg::OnInitDialog()
{
BOOL bJoyStk;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	// 画面が表示される時はデフォルトはソフトJoyとする
	m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		// Radioボタンの設定
		m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
		m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

// 2013.01.11 bagus jog direction -->
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_JogDirSetting, CONFIG_FILE_JOG_DIR);
// 2013.01.11 bagus jog direction <--

	//
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pTestMeasDlgObj = this;
	m_iConditionFlg = TESTMODE_INIT;	//テストモード起動直後
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

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(!m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_AutoFocusButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
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
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

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

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nJoystickType == JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		GetDlgItem(IDC_Z_AXIS_STATIC)->ShowWindow(SW_HIDE);
		m_Z_UpButton.ShowWindow(SW_HIDE);
		m_Z_DownButton.ShowWindow(SW_HIDE);
		m_Z_HomePosButton.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MOTION_MODE_STATIC)->ShowWindow(SW_HIDE);
		m_cbMotionElev.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
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
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	/// Cancel Button ///
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	/// OK Button ///
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

/* added 2015.03.10 hmenjo テスト測定保存 ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
		m_BtnSave.ShowWindow(SW_SHOW);
		m_BtnSave.SetShade(	BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
								BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_BtnSave.DrawFlatFocus(TRUE);
	} else {
		m_BtnSave.ShowWindow(SW_HIDE);
	}
/* added 2015.03.10 hmenjo テスト測定保存 ---------- } ---------- */

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(!m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_AfMeasurementButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
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
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

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
		SetDlgItemInt(IDC_WAVELENGTH_START, MIN_TARGET_WAVELENGTH); //波長(START)
// 2009.05.15 k-matsuo -->
//		SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);	//波長(END)
		if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);
		else
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH_800);
// 2009.05.15 k-matsuo <--
	}
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);	//波長(START)
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);	//波長(END)

/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	RECT l_rectWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &l_rectWorkArea, 0);
	RECT l_rectTestMeasDlg;	this->GetWindowRect(&l_rectTestMeasDlg);
	if ((g_lModelType == MODEL_T3100) && (l_rectWorkArea.bottom < l_rectTestMeasDlg.bottom)) {
		/* デスクトップの作業エリア縦幅よりダイアログが大きい場合	*/
		/* 左側のメニューボタンのグループボックスはそのままのサイズとします．	*/
		/* ダイアログ底辺の移動距離	*/
		long l_ldYDlgBottom = l_rectTestMeasDlg.bottom - (l_rectWorkArea.bottom - 2);
		/* 一次タブとダイアログ底辺との距離	*/
		RECT l_rectMeasTab;	m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
		/* 一次タブの底辺の位置合わせ	*/
		m_MeasurementTab.SetWindowPos(	0,
							0, 0,
							l_rectMeasTab.right - l_rectMeasTab.left,
							l_rectMeasTab.bottom - l_rectMeasTab.top - l_ldYDlgBottom,
							SWP_NOZORDER | SWP_NOMOVE);
		/* 下部ボタンの位置合わせ	*/
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
		/* ダイアログの縦サイズをデスクトップの作業エリア縦幅に合わせます．	*/
		this->SetWindowPos(	0,
							0, 0,
							l_rectTestMeasDlg.right, l_rectWorkArea.bottom - 2,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */

	///// Grid Control Initialize /////
	MeasurementTab_Init();

	// ジョイスティック有効／無効
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			// H/W有効なら禁止する
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
			// ジョイスティック許可
			if(!m_pMainFrame->HardwareSimulation()){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
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
			if(!m_pMainFrame->HardwareSimulation()){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
#endif
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	m_AfExecuteFlg = FALSE;
	m_AfExecuteFlgOld = FALSE;
	m_iAlarmStatus = ALARM_NON;
	m_iAlarmStatusOld = ALARM_NON;

/* deleted 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
///* added 2015.03.19 hmenjo FWXGA 対応 ---------- { ---------- */
//	RECT l_rectWorkArea;
//	::SystemParametersInfo(SPI_GETWORKAREA, 0, &l_rectWorkArea, 0);
//	RECT l_rectTestMeasDlg;	this->GetWindowRect(&l_rectTestMeasDlg);
//	if ((g_lModelType == MODEL_T3100) && (l_rectWorkArea.bottom < l_rectTestMeasDlg.bottom)) {
//		/* デスクトップの作業エリア縦幅よりダイアログが大きい場合	*/
//		/* 左側のメニューボタンのグループボックスはそのままのサイズとします．	*/
//		/* ダイアログ底辺の移動距離	*/
//		long l_ldYDlgBottom = l_rectTestMeasDlg.bottom - (l_rectWorkArea.bottom - 2);
//		/* 一次タブとダイアログ底辺との距離	*/
//		RECT l_rectMeasTab;	m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
//		/* 一次タブの底辺の位置合わせ	*/
//		m_MeasurementTab.SetWindowPos(	0,
//							0, 0,
//							l_rectMeasTab.right - l_rectMeasTab.left,
//							l_rectMeasTab.bottom - l_rectMeasTab.top - l_ldYDlgBottom,
//							SWP_NOZORDER | SWP_NOMOVE);
//		/* 下部ボタンの位置合わせ	*/
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
//		/* ダイアログの縦サイズをデスクトップの作業エリア縦幅に合わせます．	*/
//		this->SetWindowPos(	0,
//							0, 0,
//							l_rectTestMeasDlg.right, l_rectWorkArea.bottom - 2,
//							SWP_NOZORDER | SWP_NOMOVE);
//		m_pMeasurementGraphDlg1->ChgSizeCtrl(-l_ldYDlgBottom);
//		m_pMeasurementGraphDlg2->ChgSizeCtrl(-l_ldYDlgBottom);
//	}
/* added 2015.03.19 hmenjo FWXGA 対応 ---------- } ---------- */
/* deleted 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */

	SetTimer(TESTMEASDLG_ENABLE_CHANGE_TIMER, 50, NULL);


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
//
void CTestMeasurementDlg::OnDestroy()
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

// 2014.01.17 Bagus Add (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
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

	if( m_pMeasurementGraphDlg1 != NULL ){
		delete m_pMeasurementGraphDlg1;
		m_pMeasurementGraphDlg1 = NULL;
	}
	if( m_pMeasurementGraphDlg2 != NULL ){
		delete m_pMeasurementGraphDlg2;
		m_pMeasurementGraphDlg2 = NULL;
	}


	m_pDoc->SetMeasMode(MEASMODE_NOT_MEASURE); //測定中でない
	m_pDoc->InitMeasData(); //メモリー初期化

	DummyRcpDelete(); //ダミーレシピ削除

	//ステータスバー文字クリア
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

// 2010.02.01 K.Matsuo ASSERT 発生回避 -->
	m_pMainFrame->m_pTestMeasDlgObj = NULL;
// 2010.02.01 K.Matsuo ASSERT 発生回避 <--

	CDialog::OnDestroy();
}

// =========================================================================
//
void CTestMeasurementDlg::OnOK()
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

	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	/*//Saiki 20090520 Change <-----*/
	//ActuateFlagチェック
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	CString csBuff;

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_dIntegTime = atof(csBuff);

//	HookKeyUnhook();

// 2009.09.30 K.Matsuo Add -->
	///// 指定フォルダ存在確認 /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// 測定データ仮フォルダが残っていたら削除 /////
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
	// TODO: この位置に特別な後処理を追加してください。

//	HookKeyUnhook();

// 2009.09.30 K.Matsuo Add -->
	///// 指定フォルダ存在確認 /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// 測定データ仮フォルダが残っていたら削除 /////
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
	//m_MeasurementTab.InsertItem(0, "反射率データ"); //Reflectance Data
	//m_MeasurementTab.InsertItem(1, "光強度データ"); //Raw Data

	CString strBuf1, strBuf2;

	LoadStringML(IDS_CAPTION_REFLECTANCE_DATA, strBuf1, "Reflectance Data");
	LoadStringML(IDS_CAPTION_RAW_DATA, strBuf2, "Raw Data");
	m_MeasurementTab.InsertItem(0, strBuf1); //Reflectance Data
	m_MeasurementTab.InsertItem(1, strBuf2); //Raw Data
	//Saiki 20090528 Change <-----
	///// List	/////
	int dx = 5;
	int dy = 24;

	///// Graph(通常) /////
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

	//レシピ読込み

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	RecipeFile_LoadRecipe(&m_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE); //
	RecipeFile_LoadRecipe(&m_Meas, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
}

// =========================================================================
//レンズ コンボイニシャル処理
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
// レンズ コンボ選択時
//
void CTestMeasurementDlg::OnSelchangeSrLens()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----
	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// メンテナンス異常
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// シャッター以外が動いていたら動作禁止
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // ステージ動作中
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲

	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//取込み条件(Detector Condition)変更後

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

// 2013.12.18 Bagus Add (TohoSpec対応) -->
	BOOL bUseFilter;

	if(g_lModelType != MODEL_T3100){
		bUseFilter = m_SrConfig.bAutoFilter;	//オートフィルタのコンフィグ設定の内容
	}
	else{
		bUseFilter = TRUE;	// T3100では、フィルタ設定を常に使用（表示）する
	}
// 2013.12.18 Bagus Add (TohoSpec対応) <--

// 2013.12.18 Bagus Mod (TohoSpec対応) -->
//	//オートフィルタありの場合
//	if (m_SrConfig.bAutoFilter){
	// フィルタ設定を使用する場合
	if (bUseFilter){
// 2013.12.18 Bagus Mod (TohoSpec対応) <--
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

	//オートフィルタなしの場合
	else{
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
// オプティカルフィルター コンボ選択時
//
void CTestMeasurementDlg::OnSelchangeSrOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.wOpticsFilterType = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//取込み条件(Detector Condition)変更後

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
// 2013.11.08 Bagus Add (TohoSpec対応) -->
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
// 2013.11.08 Bagus Add (TohoSpec対応) <--
// 2014.01.17 Bagus Add (Stage None対応) -->
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
			m_XY_UpButton.ShowWindow(SW_HIDE);
			m_XY_DownButton.ShowWindow(SW_HIDE);
			m_XY_RightButton.ShowWindow(SW_HIDE);
			m_XY_LeftButton.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_XYSTAGE_STATIC)->ShowWindow(SW_HIDE);
		}
		else{
// 2014.01.17 Bagus Add (Stage None対応) <--
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
// 2014.01.17 Bagus Add (Stage None対応) -->
		}
// 2014.01.17 Bagus Add (Stage None対応) <--
// 2009.10.29 bagus Jog 追加修正 --{--
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
// 2009.10.29 bagus Jog 追加修正 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

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
	//2009.12.11 bagus 修正 --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//pCombo->AddString("<Select>");
	pCombo->AddString(strSelectCaption);
	//2009.12.11 bagus 修正 --{--
	for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
		itoa(iMeasPt, szMeasPt, 10);
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
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return 0L;
		}	// ステージ動作中
		//Saiki 20090601 Change <-----
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
		}	// ステージ動作中
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲

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
			// H/W無効なら
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
			// H/W無効なら
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
	}

	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strBuffer, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strBuffer);
		return;
	} //ステージが動作中です
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	BOOL bJoyStk;

	m_AfExecuteFlg = TRUE;

	// H/W有効なら
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ジョイスティックを使用不可に設定
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
	}

	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	if( NS_StageDoAutoFocus()==FALSE ){
//		m_AutoFocusResult	 = 1;	// AutoFocus Result NG
		//Saiki 20090528 Change ----->
		//MessageBox("オートフォーカス失敗!!!", "AUTO FOCUS", MB_OK|MB_ICONWARNING);
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

	// H/W有効なら
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //ジョイスティックを使用可に設定
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
	}

	m_AfExecuteFlg = FALSE;

}


// ==========================================================================
//
void CTestMeasurementDlg::OnReferenceButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	//Saiki 20090530 Add ----->
	CString strMsg, strTitle;
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
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		//Saiki 20090601 Change ----->
		//MessageBox(	"動作中です",	"警告", MB_OK);
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		//Saiki 20090601 Change <-----
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	//Saiki 20090528 Change ----->
	//char* pszCaption = "テストモードリファレンス測定開始";
	//char* pszCaption;
	CString strBuffer;
	//Saiki 20090528 Change <-----


	m_pDoc->GetRcpData(&rcp_data);


	///// Clear Reference /////
	ClearReference(rcp_data.szRecipeName);


	//レンズ設定
	UINT uiLens = m_Meas.ScanParams._SR.iLens;
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(uiLens);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	//仮メジャーメントプログラムを修正
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
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// チーフの結果 /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
// ADD 2009.06.01
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.01
		return;
	}

/* modified 2017.01.13 hmenjo テスト系ログ修正 ---------- { ---------- */
//	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");
/* modified 2017.01.13 hmenjo テスト系ログ修正 ----------              */
	m_pDoc->OperationLogging("Test Meas. Reference Button was push.");
/* modified 2017.01.13 hmenjo テスト系ログ修正 ---------- } ---------- */

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	// H/W有効なら
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ジョイスティックを使用不可に設定
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnReferenceButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
	}

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
			CExecutionDlg dlg;
//			dlg.SetCaption(EXECUTION_CAPTION);
			LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
			dlg.SetCaption(strBuffer);
			//Saiki 20090128 Change <-----
			//Saiki 20090528 Change ----->
			//dlg.SetMessage("リファレンス測定中...");
			LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
			dlg.SetMessage(strBuffer);
			//Saiki 20090528 Change <-----
			dlg.DoModal();
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	m_iConditionFlg = TESTMODE_REF_AFTER;	//リファレンス測定後(上のモーダルのダイアログボックスをリファレンス測定終了まで抜けないこと)

	return;

}


// ==========================================================================
//
void CTestMeasurementDlg::OnMeasurementButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle, strMsg;
	// Kojika 20090529 Add End

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
	}

	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090601 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090601 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE/*_Ed*/)
	{
		// Kojika 20090529 Change
		//MessageBox("取込み条件が変更されています。\nリファレンスを取得後に再度測定を実行してください。", "NanoSpec", MB_OK);
		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
		return;
	}


	//仮メインレシピ書き換え
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 0;
	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
	{
		// Kojika 20090529 Change
		//MessageBox("リファレンス実行フラグの更新に失敗しました", "NanoSpec", MB_OK);
		LoadStringML(IDS_FAILED_UPDATE_REFER_EXE, l_strBuffer, "It failed in the update of the reference execution flag");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}
	m_pDoc->SetRcpData(&rcp_data);

/* added 2017.01.13 hmenjo テスト系ログ修正 ---------- { ---------- */
	m_pDoc->OperationLogging("Test Meas. Measurement Button was push.");
/* added 2017.01.13 hmenjo テスト系ログ修正 ---------- } ---------- */

	StartTestModeMeas();

}


//Saiki 20130123 change --->
// ==========================================================================
//
//void CTestMeasurementDlg::OnAfAndMeasurementButton()
//{
//	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//
//	// Kojika 20090529 Add
//	CString l_strBuffer, l_strTitle, strMsg;
//	// Kojika 20090529 Add End
//
//	//リモート時は何もしない
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
//
//
//	// ▼インターロック条件▼
//	//メンテナンスモードチェック(2軸同時動作制限)
//	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
//	{
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
//		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
//			この場合、2軸同時動作可能 */
//	}
//	else //東朋メンテナンスモード以外
//	{
//	}
//
//	// 各I/Oチェック
//	/*//Saiki 20090520 Change ----->*/
//	if(m_pDoc->IsInterLock() == TRUE){return;}
//	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
//	/*//Saiki 20090520 Change <-----*/
//
//	//ActuateFlagチェック
//	//Saiki 20090601 Change ----->
//	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
//		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
//		m_pDoc->MessageStringIf_Set(strMsg);
//		return;
//	} //ステージが動作中です
//	//Saiki 20090601 Change <-----
//	/*//Saiki 20090520 Change ----->*/
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
//	/*//Saiki 20090520 Change <-----*/
//	// ▲インターロック条件▲
//
//
//	if(m_iConditionFlg == TESTMODE_CONDITION_CANGE)
//	{
//		// Kojika 20090529 Change
//		//MessageBox("取込み条件が変更されています。\nリファレンスを取得後に再度測定を実行してください。", "NanoSpec", MB_OK);
//		LoadStringML(IDS_TAKING_CONDITION_CHANGE, l_strBuffer, "The taking condition has been changed.\nPlease execute the measurement again after acquiring the reference.");
//		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
//		MessageBox(l_strBuffer, l_strTitle, MB_OK);
//		// Kojika 20090529 Change End
//		m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;
//		return;
//	}
//
//
//	//仮メインレシピ書き換え
//	RCP_DATA rcp_data;
//	memset(&rcp_data, 0, sizeof(rcp_data));
//	m_pDoc->GetRcpData(&rcp_data);
//	rcp_data.MainRcpInfo.MainRcpParam._SR.nFocus = 1;
//	if ( !RecipeFile_SaveRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_SR_MAIN_RECIPE) )
//	{
//		// Kojika 20090529 Change
//		//MessageBox("リファレンス実行フラグの更新に失敗しました", "NanoSpec", MB_OK);
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
	}

	// 各I/Oチェック
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, l_strBuffer, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(l_strBuffer);
		return;
	} //ステージが動作中です
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ▲インターロック条件▲


	BOOL bJoyStk;

	m_AfExecuteFlg = TRUE;

	// H/W有効なら
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ジョイスティックを使用不可に設定
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
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

	// H/W有効なら
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		bJoyStk= m_pDoc->ChangeJoyStick(TRUE); //ジョイスティックを使用可に設定
		if( bJoyStk == TRUE ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
		TRACE("CTestMeasurementDlg::OnAutoFocusButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
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


	//仮メインレシピ書き換え
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


	//測定数制限
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


	//仮メジャーメントプログラムを修正
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


	///// チーフへ測定開始メッセージを送信 /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

// 一時的にハードJoyStickを殺す
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1ポイント測定

	///// チーフの結果 /////
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //測定後

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
	//char* pszCaption = "テストモード測定開始";
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	//測定数制限
	if(m_pDoc->GetStoreCount() >= TESTDATA_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
		// Kojika 20090529 Change
		//sprintf(szMsgBuff, "測定データ数 %d を超えています!", TESTDATA_POINT_MAX);
		LoadStringML(IDS_MEASURE_DATA_NUMBER_EXCEED, l_strBuffer, "Measurement data of number %d is exceeded.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		sprintf(szMsgBuff, l_strBuffer, TESTDATA_POINT_MAX);
		MessageBox(szMsgBuff, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}


	//仮メジャーメントプログラムを修正
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


	///// チーフへ測定開始メッセージを送信 /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

#if 1
// ADD 2009.06.02 H.Kawamura
// 一時的にハードJoyStickを殺す
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();

		return;
	}
// ADD 2009.06.02 H.Kawamura
#endif

/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */
/* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM)rcp_data.szRecipeName, 0); //1ポイント測定
/* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ----------			   */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1ポイント測定
/* modified 2009.11.27 hmenjo １ポイント測定で測定ポイント番号 ---------- } ---------- */

	///// チーフの結果 /////
	// Kojika 20090529 Change
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");

#if 1
// ADD 2009.06.02 H.Kawamura
		// 必ずJoyStickの状態を戻すこと
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.02 H.Kawamura
#endif

		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

#if 0
// DEL 2009.06.02 H.Kawamura
// 一時的にハードJoyStickを殺す処理に置き換える
	// H/W有効なら
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //ジョイスティックを使用不可に設定
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementDlg::OnMeasurementButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
	}
// DEL 2009.06.02 H.Kawamura
#endif

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //測定後
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
//テストモード1ポイント測定終了時処理

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);

	return 0L;
}



// ==========================================================================
//
LRESULT CTestMeasurementDlg::OnTestModeRefEnd(WPARAM wParam, LPARAM lParam)
{
//テストモード1ポイント測定終了時処理

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);

	return 0L;
}


// =========================================================================
//ダミーメジャーメントプログラムを上書き
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
//ダミーレシピ削除
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


// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
// =========================================================================
//
void CTestMeasurementDlg::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* ドアインターロック			＝Ｈ */
// 2014.01.17 Bagus Add (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
			if ( m_joyStick2->GetCheck() ) {
				// 強制的にソフトJoyへ切り替える
				m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
				m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
				m_joyStick1->Checked();
				m_joyStick2->UnChecked();
//				m_joyStick1->SetFocus();		// ラジオボタンのチェックが消えてなくなる問題が発生するのでコメントアウト。原因不明。
			}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	}
}
// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 <--

void CTestMeasurementDlg::OnTimer(UINT nIDEvent)
{
BOOL bJoyStk;

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iAlarmStatus = m_pDoc->GetAlarmStatus();

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

	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld))
	{
//================
		//ハードジョイスティック動作制限(アラーム時のみ)
		if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_EXIST))
		{
			// H/W有効なら
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
		else if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_NON))
		{
			// H/W有効なら
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
			}
		}

//================
		//コントロールのEnable操作(単独オートフォーカス動作時以外)
		switch(nIDEvent)
		{
			case  TESTMEASDLG_ENABLE_CHANGE_TIMER:

				if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_INIT)||(m_iConditionFlg == TESTMODE_CONDITION_CANGE_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //『WAIT』 かつ (『テストモード起動直後』 または 『取込み条件(Detector Condition)変更後』) かつ 『アラームなし』
					//取込み条件
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //レンズ
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//レンズ
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//光学フィルター
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//測定時間

					//測定系
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//リファレンス
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//測定
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//オートフォーカス／測定
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);		//オートフォーカス
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

					//ステージ
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);		//モード(XYステージ)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(TRUE);		//モード(Z軸)
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					//※矢印ボタンはボタン内で処理

					//終了
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[Save]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- } ---------- */
				}
				else if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_REF_AFTER)||(m_iConditionFlg == TESTMODE_MEAS_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //『WAIT』 かつ (『リファレンス測定後』 または 『測定後』) かつ 『アラームなし』
					//取込み条件
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //レンズ
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//レンズ
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//光学フィルター
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//測定時間

					//測定系
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//リファレンス
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(TRUE); 	//測定
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//オートフォーカス／測定
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);		//オートフォーカス
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

					//ステージ
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(TRUE);		//モード(XYステージ)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(TRUE);		//モード(Z軸)
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					//※矢印ボタンはボタン内で処理

					//終了
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[sSave]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- } ---------- */
				}
				else if((m_iProcessStatus == PROCESS_DOWN) || (m_iAlarmStatus != ALARM_NON))
				{ //『DOWN』 または 『アラーム発生時』
					//取込み条件
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //レンズ
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//レンズ
					}
					//Saiki 20090908 Change <-----
					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//光学フィルター
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//測定時間

					//測定系
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//リファレンス
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//測定
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//オートフォーカス／測定
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 	//オートフォーカス
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

					//ステージ
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 	//モード(XYステージ)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(FALSE);		//モード(Z軸)
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					//※矢印ボタンはボタン内で処理

					//終了
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- { ---------- */
					m_BtnSave.EnableWindow(TRUE);								//[Save]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- } ---------- */
				}
				else if((m_iProcessStatus == PROCESS_INIT)
					||(m_iProcessStatus == PROCESS_PROC)
					||(m_iProcessStatus == PROCESS_ABRT)
					||(m_iProcessStatus == PROCESS_COMP))
				{ //『INIT』または『PROC』または『ABRT』または『COMP』
					//取込み条件
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE); 			   //レンズ
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);				 //レンズ
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(FALSE); 		//光学フィルター
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(FALSE); //測定時間

					//測定系
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//リファレンス
					GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);	//測定
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(FALSE); 	//オートフォーカス／測定
						GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 	//オートフォーカス
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

					//ステージ
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//					if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
						GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(FALSE); 	//モード(XYステージ)
						GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(FALSE);		//モード(Z軸)
// 2013.11.08 Bagus Add (TohoSpec対応) -->
					}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
					//※矢印ボタンはボタン内で処理

					//終了
					GetDlgItem(IDOK)->EnableWindow(FALSE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);					//[CANCEL]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- { ---------- */
					m_BtnSave.EnableWindow(FALSE);								//[Save]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- } ---------- */
				}
				else
				{
					//ココにはこないはず
				}

				break;
		}
	}

//================

	//コントロールのEnable操作(単独オートフォーカス動作時)
	if(m_AfExecuteFlg != m_AfExecuteFlgOld)
	{
		//取込み条件
		GetDlgItem(IDC_SR_LENS)->EnableWindow(!m_AfExecuteFlg); 				//レンズ
		if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
		{
			GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(!m_AfExecuteFlg);		//光学フィルター
		}
		GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(!m_AfExecuteFlg);	//測定時間

		//測定系
		GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//リファレンス
		GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//測定
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SrConfig.bAutoFocus){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			GetDlgItem(IDC_AF_AND_MEASUREMENT_BUTTON)->EnableWindow(!m_AfExecuteFlg);	//オートフォーカス／測定
			GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//オートフォーカス
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

		//ステージ
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(!m_AfExecuteFlg);		//モード(XYステージ)
			GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(!m_AfExecuteFlg);		//モード(Z軸)
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		//※矢印ボタンはボタン内で処理

		//終了
		GetDlgItem(IDOK)->EnableWindow(!m_AfExecuteFlg);						//[OK]ボタン
		GetDlgItem(IDCANCEL)->EnableWindow(!m_AfExecuteFlg);					//[CANCEL]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- { ---------- */
		m_BtnSave.EnableWindow(!m_AfExecuteFlg);								//[Save]ボタン
/* added 2015.03.21 hmenjo テスト測定保存 ---------- } ---------- */
	}


#if 1 //ProcessStatusとActuateFlagsを併用
//取込み条件編集中、ウェイト以外、アラーム発生中、AF実行中 はボタン無効にする。
	//ソフトジョイスティック制限
	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld)||(m_AfExecuteFlg != m_AfExecuteFlgOld))
	{
		if((m_iConditionFlg == TESTMODE_CONDITION_CANGE_EDIT) || ( m_iProcessStatus != PROCESS_WAIT ) || (m_iAlarmStatus == ALARM_EXIST) || (m_AfExecuteFlg == TRUE))
		{
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
			if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
				SoftJoyStickXyzEnable(FALSE); //操作禁止

				//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
				GetDlgItem(IDC_JOYSTICK1)->EnableWindow(FALSE);
				GetDlgItem(IDC_JOYSTICK2)->EnableWindow(FALSE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
			}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		}
		else
		{
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
			if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
				SoftJoyStickXyzEnable(TRUE); //操作許可

				//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
				GetDlgItem(IDC_JOYSTICK1)->EnableWindow(TRUE);
				GetDlgItem(IDC_JOYSTICK2)->EnableWindow(TRUE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
			}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		}
	}
#endif

//================
	//前回値更新
	m_iConditionFlgOld = m_iConditionFlg;
	m_iProcessStatusOld = m_iProcessStatus;
	m_iAlarmStatusOld = m_iAlarmStatus;
	m_AfExecuteFlgOld = m_AfExecuteFlg;

//================

	CDialog::OnTimer(nIDEvent);
}



void CTestMeasurementDlg::OnSetfocusSrLens()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementDlg::OnKillfocusSrLens()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//取込み条件(Detector Condition)変更後
}

void CTestMeasurementDlg::OnSetfocusSrOptFilter()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementDlg::OnKillfocusSrOptFilter()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//取込み条件(Detector Condition)変更後
}

void CTestMeasurementDlg::OnSetfocusMeasIntegrationTime()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementDlg::OnKillfocusMeasIntegrationTime()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_Meas.ScanParams._SR.dIntegTime = atof(csBuff);

	m_pDoc->SetRcpData(&rcp_data);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//取込み条件(Detector Condition)変更後

}

void CTestMeasurementDlg::OnSetfocusWavelengthStart()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementDlg::OnKillfocusWavelengthStart()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

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
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //取込み条件(Detector Condition)変更後

}

void CTestMeasurementDlg::OnSetfocusWavelengthEnd()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementDlg::OnKillfocusWavelengthEnd()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
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
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //取込み条件(Detector Condition)変更後
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ---------- { ---------- */
//void CTestMeasurementDlg::OnJoyStick()
/* modified 2009.06.15 hmenjo ユーザ定義メッセージハンドラ IF 修正 ----------			   */
LRESULT CTestMeasurementDlg::OnJoyStick(WPARAM wparam, LPARAM lparam)
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
			// ジョイスティック許可
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
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
		//strBuffer.Format("測定時間が範囲外に設定されています\n(%.3f - %.3f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
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
// 日比野さんへ
// 測定中などの条件は分かりませんので追記願います

//→『SoftJoyStickXyzEnable()の中』に記述しました。 hibino

	// XY軸ボタン
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_pMainFrame->GetJoyStickMode() != 0 ){
			//ソフトジョイスティック無効
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
		else{
			if( m_pDoc->ActuateFlagsGet( ACTUATE_XYSTAGE ) ){
				//ソフトジョイスティック無効
				m_XY_UpButton.SetEnabled(FALSE);
				m_XY_DownButton.SetEnabled(FALSE);
				m_XY_RightButton.SetEnabled(FALSE);
				m_XY_LeftButton.SetEnabled(FALSE);
			}
			else{
				//ソフトジョイスティック有効
				m_XY_UpButton.SetEnabled(TRUE);
				m_XY_DownButton.SetEnabled(TRUE);
				m_XY_RightButton.SetEnabled(TRUE);
				m_XY_LeftButton.SetEnabled(TRUE);
			}
		}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	// ジョイスティック



	// Z軸ボタン
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if( m_pDoc->ActuateFlagsGet( ACTUATE_ZAXIS ) ){
			m_Z_UpButton.SetEnabled(FALSE);
			m_Z_DownButton.SetEnabled(FALSE);
		}
		else{
			m_Z_UpButton.SetEnabled(TRUE);
			m_Z_DownButton.SetEnabled(TRUE);
		}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
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
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nJoystickType != JOYSTICK_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_XY_UpButton.SetEnabled(bEnable);
		m_XY_DownButton.SetEnabled(bEnable);
		m_XY_RightButton.SetEnabled(bEnable);
		m_XY_LeftButton.SetEnabled(bEnable);
		m_Z_UpButton.SetEnabled(bEnable);
		m_Z_DownButton.SetEnabled(bEnable);
		//Saiki 20090908 Add ----->
		m_Z_HomePosButton.SetEnabled(bEnable);
		//Saiki 20090908 Add <-----
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}

// =========================================================================
//
void CTestMeasurementDlg::OnSelchangeComboMeasPoint()
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

	//リファレンスポジションのロード
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	iSelectMeasPt = pCombo->GetCurSel();
	if(0 < iSelectMeasPt && iSelectMeasPt <= m_SrConfig.wGantryNumOfMeasPoint){
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
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
// 2010.02.03 K.Matsuo PIN UPPER インターロックチェック漏れ対応 -->
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
// 2010.02.03 K.Matsuo PIN UPPER インターロックチェック漏れ対応 <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z 軸の初期化停止位置
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z 軸の初期化停止位置ファクタ

	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != m_pMainFrame->HardwareSimulation()) {
		// ハードシミュレートモードだったので，測定完了待ち へ
		//m_MeasPosXYZ.lZ = 0;
		// トランジション更新
	}
	else {
		// 実機の場合 ---------------------------------------------------------
		// AF 要/不要判定
		// 不要
//			  m_dwAFValid = 0;	  // AF 無し
			// (全点 AF 無しの場合)Z 軸移動が必要かチェック
//			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF)) {
//			if ((0 != l_pMainRcpInfo->MainRcpParam._SR.nFocus) || (0 == l_bAF) || (TRANSI_MASTER == m_dwParentTransi)) {
//				// 不要
//				// トランジション更新
//				if (0 != IsPointManualAdjust(m_dwPointNo)) {
//					// PMA のみが必要
//					TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
//				} else {
//					TransiEvent(EV_SR1P_MEAS_START);
//				}
//			} else {
		// 必要(仕様上必要なら)
		// Ｚ軸移動
		//TCHAR l_szMainRcpName[256];
		//((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// 初期化停止位置[pulse]を[0.1um]に変換します
		LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
		l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// 初期化停止位置[pulse]÷ファクタ[pulse/mm]
		l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]に変換する
		long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

		NS_ConvertToStageMoveCoord(&l_lZ);		// 補正

		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
#if 0	//----- Kawashima 2008.12.01 ----->
//				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
		m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// 移動開始失敗
			//((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(7, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
			// トランジション更新
//					TransiEvent(EV_SR1P_ZMOVE_FAIL);
			// 呼出元トランジションに完了を通知
//					  NotifyComplete((cEventParams*) TR_SR1P_ZMOVE_FAIL);
		} else {
			// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);   // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			m_pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
//					((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to 0 position."));
//					// トランジション更新
//					if (0 != IsPointManualAdjust(m_dwPointNo)) {
//						// PMA のみが必要
//						TransiEvent(EV_SR1P_AF_START, (cEventParams*) 1);
//					} else {
//						TransiEvent(EV_SR1P_ZMOVE_DONE);
//					}
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
	}
}

/* added 2015.03.10 hmenjo テスト測定保存 ---------- { ---------- */
void CTestMeasurementDlg::OnBtnSave()
{
	/* 表	*/
	CGridCtrl* l_pMeasListGrid = 0;
	CGridCtrl* l_pIntListGrid = 0;
	CGridCtrl* l_pRefListGrid = 0;
	m_pMeasurementGraphDlg1->TestMeasGetGrid(&l_pMeasListGrid, &l_pIntListGrid);
	m_pMeasurementGraphDlg2->TestMeasGetGrid(0, &l_pRefListGrid);
	/* l_pMeasListGrid：列(Col)，行(Row)を取得	*/
	int l_iMeasListGridCnt_Col = l_pMeasListGrid->GetColumnCount();
	int l_iMeasListGridCnt_Row = l_pMeasListGrid->GetRowCount();
	/* l_pMeasListGrid：列項目を確認して列数をチェック修正	*/
	CString l_strTmp;
	for (int i = 0; i < l_iMeasListGridCnt_Col; i++) {
		l_strTmp = l_pMeasListGrid->GetItemText(0, i);
		if (l_strTmp.GetLength() <= 0) {
			l_iMeasListGridCnt_Col = i;
			break;
		}
	}
	/* l_pMeasListGrid：１項目目のデータを確認して行数をチェック修正	*/
	for (int j = 0; j < l_iMeasListGridCnt_Row; j++) {
		l_strTmp = l_pMeasListGrid->GetItemText(j, 1);
		if (l_strTmp.GetLength() <= 0) {
			l_iMeasListGridCnt_Row = j;
			break;
		}
	}
	/* 列数が２以上の場合(データ行が存在するとき)のみ下記を実行します．	*/
	if (1 < l_iMeasListGridCnt_Row) {
		/* 測定プログラム名を取得	*/
		RCP_DATA l_RcpData;
		memset(&l_RcpData, 0, sizeof(l_RcpData));
		m_pDoc->GetRcpData(&l_RcpData);
		/* 出力ファイル	*/
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
	/* 現在の選択行を取得	*/
		/*	現在の選択行，あるいは，現在の表示行を記憶しておいて，
			ファイル作成後に元の表示行に戻そうと思いましたが，
			int l_iMeasListGrid__CurRow = m_pMeasurementGraphDlg1->TestMeasGetGraphRow();
			わりとめんどうなので，最終行になるという仕様にします．*/
				m_pMeasurementGraphDlg1->TestMeasChangeSelect(j);
				m_pMeasurementGraphDlg2->TestMeasChangeSelect(j);
				/* l_pIntListGrid：列(Col)，行(Row)を取得	*/
				int l_iIntListGridCnt_Col = l_pIntListGrid->GetColumnCount();
				int l_iIntListGridCnt_Row = l_pIntListGrid->GetRowCount();
				/* l_pIntListGrid：列項目を確認して列数をチェック	*/
				for (i = 0; i < l_iIntListGridCnt_Col; i++) {
					l_strTmp = l_pIntListGrid->GetItemText(0, i);
					if (l_strTmp.GetLength() <= 0) {
						l_iIntListGridCnt_Col = i;
						break;
					}
				}
				/* l_pIntListGrid：ヘッダを確認して行数をチェック	*/
				for (i = 0; i < l_iIntListGridCnt_Row; i++) {
					l_strTmp = l_pIntListGrid->GetItemText(i, 0);
					if (l_strTmp.GetLength() <= 0) {
						l_iIntListGridCnt_Row = i;
						break;
					}
				}
				/* l_pRefListGrid：列(Col)，行(Row)を取得	*/
				int l_iRefListGridCnt_Col = l_pRefListGrid->GetColumnCount();
				int l_iRefListGridCnt_Row = l_pRefListGrid->GetRowCount();
				/* l_pRefListGrid：列項目を確認して列数をチェック	*/
				for (i = 0; i < l_iRefListGridCnt_Col; i++) {
					l_strTmp = l_pRefListGrid->GetItemText(0, i);
					if (l_strTmp.GetLength() <= 0) {
						l_iRefListGridCnt_Col = i;
						break;
					}
				}
				/* l_pRefListGrid：ヘッダを確認して行数をチェック	*/
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
/* added 2015.03.10 hmenjo テスト測定保存 ---------- } ---------- */
