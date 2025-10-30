// TestMeasurementGantryDlg.cpp : ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MeasurementGraphDlg.h"
#include "MeasurementGraphDlg2.h"
#include "MainFrm.h"
#include "TestMeasurementGantryDlg.h"
#include "ExecutionDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TESTMEASDLG_ENABLE_CHANGE_TIMER 1

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementGantryDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O


CTestMeasurementGantryDlg::CTestMeasurementGantryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMeasurementGantryDlg::IDD, pParent)
{
	m_strLens = "";
	m_strOpticalFilter = "";
	m_dIntegTime = 0;
	m_bIsMoveAtSpeed = FALSE;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
}


void CTestMeasurementGantryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMeasurementGantryDlg)
	DDX_Control(pDX, IDC_MEASUREMENT_TAB, m_MeasurementTab);
	DDX_Control(pDX, IDC_REFERENCE_BUTTON, m_ReferenceButton);
	DDX_Control(pDX, IDC_ONE_POINT_REFERENCE_BUTTON, m_OnePointReferenceButton);
	DDX_Control(pDX, IDC_REFERENCE_CANCEL_BUTTON, m_ReferenceCancelButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_CBStringExact(pDX, IDC_SR_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_SR_OPT_FILTER, m_strOpticalFilter);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MEAS_INTEGRATION_TIME, m_Meas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_WAVELENGTH_START, (short&)m_Meas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_WAVELENGTH_END,	(short&)m_Meas.ScanParams._SR.WavelenRange.wEnd);
	DDX_Control(pDX, IDC_MEAS_POINT_X, m_stcMeasurementPointX);
	DDX_Control(pDX, IDC_MEAS_POINT_Y, m_stcMeasurementPointY);
}


BEGIN_MESSAGE_MAP(CTestMeasurementGantryDlg, CDialog)
	//{{AFX_MSG_MAP(CTestMeasurementGantryDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MEASUREMENT_TAB, OnSelchangeMeasurementTab)
	ON_BN_CLICKED(IDC_REFERENCE_BUTTON, OnReferenceButton)
	ON_BN_CLICKED(IDC_ONE_POINT_REFERENCE_BUTTON, OnOnePointReferenceButton)
	ON_BN_CLICKED(IDC_REFERENCE_CANCEL_BUTTON, OnReferenceCancelButton)
	ON_WM_TIMER()
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TESTMODE_ONEPOINT_END, OnTestModeOnePointEnd)
	ON_MESSAGE(WM_TESTMODE_REF_END, OnTestModeRefEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementGantryDlg ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
BOOL CTestMeasurementGantryDlg::OnInitDialog()
{
	char szMeasPt[16];

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	//
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pTestMeasGantryDlgObj = this;
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

	///// Get Recipe Data /////
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);

	///// Control Initialize /////
	InitCombo_Lens();
	if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
	else{
		InitCombo_SrOpticalFilter();
	}

	/// Reference Button ///
	m_ReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceButton.DrawFlatFocus(TRUE);

	/// 1Point Reference Button ///
	m_OnePointReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OnePointReferenceButton.DrawFlatFocus(TRUE);

	/// Reference Cancel Button ///
	m_ReferenceCancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceCancelButton.DrawFlatFocus(TRUE);

	/// Cancel Button ///
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	/// OK Button ///
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE   ||
		rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE ||
		rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G )
	{
		SetDlgItemInt(IDC_WAVELENGTH_START, MIN_TARGET_WAVELENGTH); //ï¿½gï¿½ï¿½(START)
		if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);
		else
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH_800);
	}
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);	//ï¿½gï¿½ï¿½(START)
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);	//ï¿½gï¿½ï¿½(END)


	///// Grid Control Initialize /////
	MeasurementTab_Init();

	//Lens Disable
	//2010.01.09 bagus --{--
	//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);
	GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);
	//2010.01.09 bagus --}--

	//Gantry Number Of Measure Point Show & Initialize
	GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_SHOW);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	pCombo->AddString(strSelectCaption);
	for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
		_itoa(iMeasPt, szMeasPt, 10);
		pCombo->AddString(szMeasPt);
	}
	pCombo->SetCurSel(0);

	m_stcMeasurementPointX.SetBkColor(REFERENCE_COLOR);
	m_stcMeasurementPointY.SetBkColor(REFERENCE_COLOR);

	GetDlgItem(IDC_MEAS_POINT_X)->SetWindowText("-----");
	GetDlgItem(IDC_MEAS_POINT_Y)->SetWindowText("-----");

	m_AfExecuteFlg = FALSE;
	m_AfExecuteFlgOld = FALSE;
	m_iAlarmStatus = ALARM_NON;
	m_iAlarmStatusOld = ALARM_NON;

	SetTimer(TESTMEASDLG_ENABLE_CHANGE_TIMER, 50, NULL);

	return TRUE;  // ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Éƒtï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½İ’è‚µï¿½È‚ï¿½ï¿½Æ‚ï¿½ï¿½Aï¿½ß‚ï¿½lï¿½ï¿½ TRUE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
					// ï¿½ï¿½O: OCX ï¿½vï¿½ï¿½ï¿½pï¿½eï¿½B ï¿½yï¿½[ï¿½Wï¿½Ì–ß‚ï¿½lï¿½ï¿½ FALSE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnDestroy()
{
BOOL result;

	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageStop();
		}
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageElevatorStop();
		}
	}

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
	m_pMainFrame->m_pTestMeasGantryDlgObj = NULL;
// 2010.02.01 K.Matsuo ASSERT ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--

	CDialog::OnDestroy();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnOK()
{
	CString strMsg;

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
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	CString csBuff;

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_dIntegTime = atof(csBuff);

//	HookKeyUnhook();

	///// ï¿½wï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½İŠmï¿½F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½í�œ /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}

	CDialog::OnOK();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnCancel()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½É“ï¿½ï¿½Ê‚ÈŒã�ˆï¿½ï¿½ï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B

//	HookKeyUnhook();

	///// ï¿½wï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½İŠmï¿½F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½í�œ /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}

	CDialog::OnCancel();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::MeasurementTab_Init()
{
	///// Tab	/////
	CString strBuf;

	LoadStringML(IDS_CAPTION_RAW_DATA, strBuf, "Raw Data");
	m_MeasurementTab.InsertItem(0, strBuf); //Raw Data

	///// List	/////
	int dx = 5;
	int dy = 24;

	///// Graph(ï¿½Ê�ï¿½) /////
	m_pMeasurementGraphDlg1 = new CMeasurementGraphDlg2(GRAPHDLG_MODE_TEST_RAW);
	m_pMeasurementGraphDlg1->Create(CMeasurementGraphDlg2::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg1->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Graph(RAW) /////
	m_pMeasurementGraphDlg2 = new CMeasurementGraphDlg2(GRAPHDLG_MODE_TEST_RAW);
	m_pMeasurementGraphDlg2->Create(CMeasurementGraphDlg2::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg2->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Set Current Selection Item (Default Select Tab) ////
	int Index;
//	Index = m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay;
	Index = 0;
	TabChange(Index);
	m_MeasurementTab.SetCurSel(Index);

}

// =========================================================================
//
void CTestMeasurementGantryDlg::TabChange(int Index)
{
	m_MeasurementTab.HighlightItem(0, FALSE);
//	m_MeasurementTab.HighlightItem(1, FALSE);
//	m_MeasurementTab.HighlightItem(Index, TRUE);
	m_MeasurementTab.HighlightItem(0, TRUE);

	///// Hide All Dialog /////
	m_pMeasurementGraphDlg1->ShowWindow(SW_HIDE);
	m_pMeasurementGraphDlg2->ShowWindow(SW_HIDE);

	///// Show Dialog /////
//	switch(Index){
//	case 0:
		m_pMeasurementGraphDlg1->ShowWindow(SW_SHOW);
//		break;
//	case 1:
//		m_pMeasurementGraphDlg2->ShowWindow(SW_SHOW);
//		break;
//	}
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index = m_MeasurementTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CTestMeasurementGantryDlg::LoadRecipeData()
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
void CTestMeasurementGantryDlg::InitCombo_Lens()
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

	if ( nIndex == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nIndex);
}

// =========================================================================
// ï¿½ï¿½ï¿½ï¿½ï¿½Y ï¿½Rï¿½ï¿½ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½
//
void CTestMeasurementGantryDlg::OnSelchangeSrLens()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	CString strMsg;

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½iï¿½ï¿½ï¿½Xï¿½Ù�ï¿½
	}
	else{												// ï¿½Ê�í“®ï¿½ï¿½
	}

	// ï¿½eI/Oï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// ï¿½Vï¿½ï¿½ï¿½bï¿½^ï¿½[ï¿½ÈŠOï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ç“®ï¿½ï¿½Ö�~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ì’†
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
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
void CTestMeasurementGantryDlg::InitCombo_SrOpticalFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	//ï¿½Iï¿½[ï¿½gï¿½tï¿½Bï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½Ì�ê�‡
	if (m_SrConfig.bAutoFilter){
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
				if(l_SystemConfig.nLanguage == 0){
					nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM_ENU[iOptItem]);
				}else{
					nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM_JPN[iOptItem]);
				}
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
void CTestMeasurementGantryDlg::OnSelchangeSrOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.wOpticsFilterType = static_cast<WORD>(pCombo->GetItemData(nSelect));

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnReferenceButton()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	CString strMsg, strTitle;

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
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
//		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	CString strBuffer;

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
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
			break;
		default:
			break;
	}


	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	CExecutionDlg dlg;
//	dlg.SetCaption(EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	dlg.SetCaption(strBuffer);
	LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
	dlg.SetMessage(strBuffer);
	dlg.DoModal();

	m_iConditionFlg = TESTMODE_REF_AFTER;	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½Ìƒï¿½ï¿½[ï¿½_ï¿½ï¿½ï¿½Ìƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½{ï¿½bï¿½Nï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ü‚Å”ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½)

	return;

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnOnePointReferenceButton()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	//ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½ï¿½Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	CString strMsg, strTitle;

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
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ï¿½Xï¿½eï¿½[ï¿½Wï¿½Gï¿½ï¿½ï¿½[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagï¿½`ï¿½Fï¿½bï¿½N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ï¿½Xï¿½eï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½ì’†ï¿½Å‚ï¿½
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
//		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	CString strBuffer;

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
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
			break;
		default:
			break;
	}


	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

//	ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½ï¿½
	int nRefPos;

	nRefPos = ((CComboBox *)GetDlgItem(IDC_COMBO_MEAS_POINT))->GetCurSel();
	if(nRefPos <= 0){
		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½Wï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		LoadStringML(IDS_WARNING_NO_REF_POS, strMsg, "Please Select Reference Position.");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, nRefPos);

	///// ï¿½`ï¿½[ï¿½tï¿½ÌŒï¿½ï¿½ï¿½ /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
		// ï¿½Kï¿½ï¿½JoyStickï¿½Ì�ï¿½Ô‚ï¿½ß‚ï¿½ï¿½ï¿½ï¿½ï¿½
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	CExecutionDlg dlg;
//	dlg.SetCaption(EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	dlg.SetCaption(strBuffer);
	LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
	dlg.SetMessage(strBuffer);
	dlg.DoModal();

	m_iConditionFlg = TESTMODE_REF_AFTER;	//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½Ìƒï¿½ï¿½[ï¿½_ï¿½ï¿½ï¿½Ìƒ_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½{ï¿½bï¿½Nï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ü‚Å”ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½)

	return;

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnReferenceCancelButton()
{
	//ï¿½ï¿½ï¿½è’†ï¿½Ì‚İƒLï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½
//	if(m_pDoc->GetProcessStatus() == PROCESS_PROC){
		AfxGetMainWnd()->PostMessage(WM_DO_CANCEL, 0, 0);
	//}
}

// ==========================================================================
//
void CTestMeasurementGantryDlg::StartTestModeMeas()
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
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
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
		TRACE("CTestMeasurementGantryDlg::OnMeasurementButton()ï¿½Ö�ï¿½ï¿½ï¿½ ChangeJoyStick()ï¿½Ì–ß‚ï¿½l:%d\n", bJoyStk);
	}
// DEL 2009.06.02 H.Kawamura
#endif

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //ï¿½ï¿½ï¿½ï¿½ï¿½
}

// ==========================================================================
//
void CTestMeasurementGantryDlg::ClearReference(char* szRecipeName)
{
	char szFilePath[MAX_PATH];
	memset(szFilePath, 0, sizeof(szFilePath));

	sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, szRecipeName, DAT_EXT);
	DeleteFile(szFilePath);
}



// ==========================================================================
//
LRESULT CTestMeasurementGantryDlg::OnTestModeOnePointEnd(WPARAM wParam, LPARAM lParam)
{
//ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);

	return 0L;
}

// ==========================================================================
//
LRESULT CTestMeasurementGantryDlg::OnTestModeRefEnd(WPARAM wParam, LPARAM lParam)
{
//ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½h1ï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	//2010.01.15 bagus --{--
	//ï¿½eï¿½Xï¿½gï¿½ï¿½ï¿½[ï¿½hï¿½Åƒ_ï¿½[ï¿½Nï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½ÌŠmï¿½ï¿½^ï¿½Cï¿½~ï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½Ìƒ^ï¿½Cï¿½~ï¿½ï¿½ï¿½Oï¿½È‚Ì‚Å‚ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½ï¿½xï¿½Xï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);

	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
	&& m_pDoc->GetMeasMode() == MEASMODE_TEST){
		m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
		m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	}
	//2010.01.15 bagus --}--
	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);

	Invalidate();
	return 0L;
}


// =========================================================================
//ï¿½_ï¿½~ï¿½[ï¿½ï¿½ï¿½Wï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½vï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ã�‘ï¿½ï¿½
BOOL CTestMeasurementGantryDlg::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName, int iRecipeTipe)
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
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			if ( !RecipeFile_SaveRecipe(&m_Meas, pszRecipeName, RECIPE_FILE_SR_TRANSMITTANCE) )
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
BOOL CTestMeasurementGantryDlg::DummyRcpDelete()
{
	char szDeleteRcpPathMain[255 + 1];
	char szDeleteRcpPathMeas[255 + 1];
	char szDeleteRcpPathRef[255 + 1];
	char szDeleteRcpPathStage[255 + 1];
	memset(szDeleteRcpPathMain, 0, sizeof(szDeleteRcpPathMain));
	memset(szDeleteRcpPathMeas, 0, sizeof(szDeleteRcpPathMeas));
	memset(szDeleteRcpPathRef, 0, sizeof(szDeleteRcpPathRef));
	memset(szDeleteRcpPathStage, 0, sizeof(szDeleteRcpPathStage));

	sprintf(szDeleteRcpPathMain, "%s%s%s",
			g_szDb_Main_Recipe_Dir, m_MainRcpInfo.hdr.szName, MAINRECIPE_EXT);
	sprintf(szDeleteRcpPathMeas, "%s%s%s",
			g_szDb_Measurement_Program_Dir, m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEASUREMENTPGM_EXT);
	sprintf(szDeleteRcpPathRef, "%s%s%s",
		g_szData_Ref_Dir, m_MainRcpInfo.hdr.szName, DAT_EXT);
	sprintf(szDeleteRcpPathStage, "%s%s%s",
			g_szDb_Stage_Program_Dir, m_MainRcpInfo.MainRcpParam.hdr.szStage, STAGEPGM_EXT);


	BOOL bMainDel = DeleteFile( szDeleteRcpPathMain );
	BOOL bMeasDel = DeleteFile( szDeleteRcpPathMeas );
	BOOL bRefDel = DeleteFile( szDeleteRcpPathRef );
	BOOL bStageDel = DeleteFile( szDeleteRcpPathStage );


	if((bMainDel == FALSE)||(bMeasDel == FALSE)||(bRefDel == FALSE))
	{
		return FALSE;
	}

	return TRUE;
}


void CTestMeasurementGantryDlg::OnTimer(UINT nIDEvent)
{
BOOL bJoyStk;

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iAlarmStatus = m_pDoc->GetAlarmStatus();

// 2009.06.05 K.Matsuo ï¿½hï¿½Aï¿½Cï¿½ï¿½ï¿½^ï¿½[ï¿½ï¿½ï¿½bï¿½Nï¿½ï¿½ï¿½oï¿½ï¿½ï¿½ï¿½JoyStickï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
//	JoyStickEnableCheck();
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(TRUE);		//1Pointï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½XCancel

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(TRUE);		//1Pointï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(TRUE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½XCancel

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(FALSE);		//1Pointï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					//2010.01.13 bagus GTR --{--
					//GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(FALSE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½XCancel
					//2010.01.13 bagus GTR --}--

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(FALSE);		//1Pointï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
					//2010.01.13 bagus GTR --{--
					//GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(FALSE);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½XCancel
					//2010.01.13 bagus GTR --}--

					//ï¿½Iï¿½ï¿½
					GetDlgItem(IDOK)->EnableWindow(FALSE);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
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
		GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//1Pointï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½X
		GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//ï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½XCancel

		//ï¿½Iï¿½ï¿½
		GetDlgItem(IDOK)->EnableWindow(!m_AfExecuteFlg);						//[OK]ï¿½{ï¿½^ï¿½ï¿½
		GetDlgItem(IDCANCEL)->EnableWindow(!m_AfExecuteFlg);					//[CANCEL]ï¿½{ï¿½^ï¿½ï¿½
	}


#if 0 //ProcessStatusï¿½ï¿½ActuateFlagsï¿½ğ•¹—p
//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½ï¿½Ò�Wï¿½ï¿½ï¿½Aï¿½Eï¿½Fï¿½Cï¿½gï¿½ÈŠOï¿½Aï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½AAFï¿½ï¿½ï¿½sï¿½ï¿½ ï¿½Íƒ{ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É‚ï¿½ï¿½ï¿½B
	//ï¿½\ï¿½tï¿½gï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½
	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld)||(m_AfExecuteFlg != m_AfExecuteFlgOld))
	{
		if((m_iConditionFlg == TESTMODE_CONDITION_CANGE_EDIT) || ( m_iProcessStatus != PROCESS_WAIT ) || (m_iAlarmStatus == ALARM_EXIST) || (m_AfExecuteFlg == TRUE))
		{
			SoftJoyStickXyzEnable(FALSE); //ï¿½ï¿½ï¿½ï¿½Ö�~

			//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(FALSE);
		}
		else
		{
			SoftJoyStickXyzEnable(TRUE); //ï¿½ï¿½ï¿½ì‹–ï¿½ï¿½

			//ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ø‘Ö‚ï¿½(ï¿½\ï¿½tï¿½gï¿½Ìƒnï¿½[ï¿½h)ï¿½ï¿½ï¿½Wï¿½Iï¿½{ï¿½^ï¿½ï¿½
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(TRUE);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(TRUE);
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



void CTestMeasurementGantryDlg::OnSetfocusSrLens()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnKillfocusSrLens()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnSetfocusSrOptFilter()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnKillfocusSrOptFilter()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ï�Xï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnSetfocusMeasIntegrationTime()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnKillfocusMeasIntegrationTime()
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

void CTestMeasurementGantryDlg::OnSetfocusWavelengthStart()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnKillfocusWavelengthStart()
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

void CTestMeasurementGantryDlg::OnSetfocusWavelengthEnd()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½ÉƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’mï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½pï¿½ÌƒRï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//ï¿½æ��ï¿½İ�ï¿½ï¿½ï¿½(Detector Condition)ï¿½Ò�Wï¿½ï¿½
}

void CTestMeasurementGantryDlg::OnKillfocusWavelengthEnd()
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
BOOL CTestMeasurementGantryDlg::CheckData()
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

// =========================================================================
//
void CTestMeasurementGantryDlg::OnSelchangeComboMeasPoint()
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
void CTestMeasurementGantryDlg::SetPointData(SR_REFERENCE_POSITION SrReferencePosition, int iSelectMeasPt)
{
	CString csPointNo, csX, csY;

	// X
	csX.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX) / MICROMETRE);
	// Y
	csY.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY) / MICROMETRE);

	SetDlgItemText(IDC_MEAS_POINT_X, csX);
	SetDlgItemText(IDC_MEAS_POINT_Y, csY);
}
