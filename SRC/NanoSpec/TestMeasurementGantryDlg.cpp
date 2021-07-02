// TestMeasurementGantryDlg.cpp : インプリメンテーション ファイル
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
// CTestMeasurementGantryDlg ダイアログ


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
// CTestMeasurementGantryDlg メッセージ ハンドラ
BOOL CTestMeasurementGantryDlg::OnInitDialog()
{
	char szMeasPt[16];

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	//
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pTestMeasGantryDlgObj = this;
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
		SetDlgItemInt(IDC_WAVELENGTH_START, MIN_TARGET_WAVELENGTH); //波長(START)
		if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);
		else
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH_800);
	}
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);	//波長(START)
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);	//波長(END)


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
		itoa(iMeasPt, szMeasPt, 10);
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

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
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

	m_pDoc->SetMeasMode(MEASMODE_NOT_MEASURE); //測定中でない
	m_pDoc->InitMeasData(); //メモリー初期化

	DummyRcpDelete(); //ダミーレシピ削除

	//ステータスバー文字クリア
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

// 2010.02.01 K.Matsuo ASSERT 発生回避 -->
	m_pMainFrame->m_pTestMeasGantryDlgObj = NULL;
// 2010.02.01 K.Matsuo ASSERT 発生回避 <--

	CDialog::OnDestroy();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnOK()
{
	CString strMsg;

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
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ▲インターロック条件▲


	CString csBuff;

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_dIntegTime = atof(csBuff);

//	HookKeyUnhook();

	///// 指定フォルダ存在確認 /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// 測定データ仮フォルダが残っていたら削除 /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}

	CDialog::OnOK();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

//	HookKeyUnhook();

	///// 指定フォルダ存在確認 /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// 測定データ仮フォルダが残っていたら削除 /////
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

	///// Graph(通常) /////
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
// レンズ コンボ選択時
//
void CTestMeasurementGantryDlg::OnSelchangeSrLens()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	CString strMsg;

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// メンテナンス異常
	}
	else{												// 通常動作
	}

	// 各I/Oチェック
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// シャッター以外が動いていたら動作禁止
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // ステージ動作中
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
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
void CTestMeasurementGantryDlg::InitCombo_SrOpticalFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	//オートフィルタありの場合
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
void CTestMeasurementGantryDlg::OnSelchangeSrOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.wOpticsFilterType = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//取込み条件(Detector Condition)変更後

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnReferenceButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	CString strMsg, strTitle;

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
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ▲インターロック条件▲


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		// 必ずJoyStickの状態を戻すこと
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
		// 必ずJoyStickの状態を戻すこと
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// チーフの結果 /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
		// 必ずJoyStickの状態を戻すこと
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

	m_iConditionFlg = TESTMODE_REF_AFTER;	//リファレンス測定後(上のモーダルのダイアログボックスをリファレンス測定終了まで抜けないこと)

	return;

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnOnePointReferenceButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	CString strMsg, strTitle;

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
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ▲インターロック条件▲


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		// 必ずJoyStickの状態を戻すこと
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
		// 必ずJoyStickの状態を戻すこと
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

//	リファレンスポジションを取得する
	int nRefPos;

	nRefPos = ((CComboBox *)GetDlgItem(IDC_COMBO_MEAS_POINT))->GetCurSel();
	if(nRefPos <= 0){
		//リファレンスポジションが選択されていない
		LoadStringML(IDS_WARNING_NO_REF_POS, strMsg, "Please Select Reference Position.");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, nRefPos);

	///// チーフの結果 /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
		// 必ずJoyStickの状態を戻すこと
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

	m_iConditionFlg = TESTMODE_REF_AFTER;	//リファレンス測定後(上のモーダルのダイアログボックスをリファレンス測定終了まで抜けないこと)

	return;

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnReferenceCancelButton()
{
	//測定中のみキャンセルを実行する
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
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
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
		TRACE("CTestMeasurementGantryDlg::OnMeasurementButton()関数内 ChangeJoyStick()の戻り値:%d\n", bJoyStk);
	}
// DEL 2009.06.02 H.Kawamura
#endif

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //測定後
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
//テストモード1ポイント測定終了時処理

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);

	return 0L;
}

// ==========================================================================
//
LRESULT CTestMeasurementGantryDlg::OnTestModeRefEnd(WPARAM wParam, LPARAM lParam)
{
//テストモード1ポイント測定終了時処理
	//2010.01.15 bagus --{--
	//テストモードでダークリファレンスの確定タイミングがこのタイミングなのでここでもう一度更新させる
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
//ダミーメジャーメントプログラムを上書き
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
//ダミーレシピ削除
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

// 2009.06.05 K.Matsuo ドアインターロック検出時のJoyStick無効化 -->
//	JoyStickEnableCheck();
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(TRUE);		//1Pointリファレンス
					GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(TRUE);		//リファレンスCancel

					//終了
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ボタン
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(TRUE);		//1Pointリファレンス
					GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(TRUE);		//リファレンスCancel

					//終了
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ボタン
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(FALSE);		//1Pointリファレンス
					//2010.01.13 bagus GTR --{--
					//GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(FALSE);		//リファレンスCancel
					//2010.01.13 bagus GTR --}--

					//終了
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]ボタン
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
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(FALSE);		//1Pointリファレンス
					//2010.01.13 bagus GTR --{--
					//GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(FALSE);		//リファレンスCancel
					//2010.01.13 bagus GTR --}--

					//終了
					GetDlgItem(IDOK)->EnableWindow(FALSE);						//[OK]ボタン
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);					//[CANCEL]ボタン
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
		GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//1Pointリファレンス
		GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//リファレンスCancel

		//終了
		GetDlgItem(IDOK)->EnableWindow(!m_AfExecuteFlg);						//[OK]ボタン
		GetDlgItem(IDCANCEL)->EnableWindow(!m_AfExecuteFlg);					//[CANCEL]ボタン
	}


#if 0 //ProcessStatusとActuateFlagsを併用
//取込み条件編集中、ウェイト以外、アラーム発生中、AF実行中 はボタン無効にする。
	//ソフトジョイスティック制限
	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld)||(m_AfExecuteFlg != m_AfExecuteFlgOld))
	{
		if((m_iConditionFlg == TESTMODE_CONDITION_CANGE_EDIT) || ( m_iProcessStatus != PROCESS_WAIT ) || (m_iAlarmStatus == ALARM_EXIST) || (m_AfExecuteFlg == TRUE))
		{
			SoftJoyStickXyzEnable(FALSE); //操作禁止

			//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(FALSE);
		}
		else
		{
			SoftJoyStickXyzEnable(TRUE); //操作許可

			//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(TRUE);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(TRUE);
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



void CTestMeasurementGantryDlg::OnSetfocusSrLens()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementGantryDlg::OnKillfocusSrLens()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//取込み条件(Detector Condition)変更後
}

void CTestMeasurementGantryDlg::OnSetfocusSrOptFilter()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementGantryDlg::OnKillfocusSrOptFilter()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//取込み条件(Detector Condition)変更後
}

void CTestMeasurementGantryDlg::OnSetfocusMeasIntegrationTime()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementGantryDlg::OnKillfocusMeasIntegrationTime()
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

void CTestMeasurementGantryDlg::OnSetfocusWavelengthStart()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementGantryDlg::OnKillfocusWavelengthStart()
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

void CTestMeasurementGantryDlg::OnSetfocusWavelengthEnd()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//取込み条件(Detector Condition)編集中
}

void CTestMeasurementGantryDlg::OnKillfocusWavelengthEnd()
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

// =========================================================================
//
void CTestMeasurementGantryDlg::OnSelchangeComboMeasPoint()
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
