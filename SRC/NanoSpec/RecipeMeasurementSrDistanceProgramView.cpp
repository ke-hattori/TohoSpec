// RecipeMeasurementSrDistanceProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "RecipeMeasurementSrDistanceProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementSrDistanceProgramView

IMPLEMENT_DYNCREATE(CRecipeMeasurementSrDistanceProgramView, CNanoRecipeUI)

CRecipeMeasurementSrDistanceProgramView::CRecipeMeasurementSrDistanceProgramView()
	: CNanoRecipeUI(CRecipeMeasurementSrDistanceProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeMeasurementSrDistanceProgramView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT

	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_DISTANCE, l_strBuffer, "SR DISTANCE");
	m_strCaption = l_strBuffer;

	m_strLens = _T("");
}

CRecipeMeasurementSrDistanceProgramView::~CRecipeMeasurementSrDistanceProgramView()
{
}

void CRecipeMeasurementSrDistanceProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementSrDistanceProgramView)
// 2010.01.07 bagus Distance 修正 --{--
	DDX_Text(pDX, IDC_COMMENT, m_DistanceMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
// 2010.01.07 bagus Distance 修正 --}--
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_DISTANCE_PROG_NAME, m_DistanceMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_CBStringExact(pDX, IDC_SR_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_OPTICAL_FILTER, m_strOptFilter);
}


BEGIN_MESSAGE_MAP(CRecipeMeasurementSrDistanceProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementSrDistanceProgramView)
	ON_CBN_SELCHANGE(IDC_SR_LENS, OnSelchangeSrThickLens)
	ON_CBN_SELCHANGE(IDC_OPTICAL_FILTER, OnSelchangeOptFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementSrDistanceProgramView 診断

#ifdef _DEBUG
void CRecipeMeasurementSrDistanceProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

void CRecipeMeasurementSrDistanceProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementSrDistanceProgramView メッセージ ハンドラ
//
void CRecipeMeasurementSrDistanceProgramView::OnInitialUpdate()
{
	CString l_strBuffer, l_strTitle;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	ConfigFile_GetNanoSpecIni(m_srFilter, CONFIG_FILE_SR_FILTER);
	LoadRecipeData();

	m_strLens = m_SrTurret[m_DistanceMeas.ScanParams._SR.iLens].szName;

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	///// Static Control /////
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_DISTANCE_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	InitCombo_Lens();
	InitCombo_OpticalFilter();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DISTANCE_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
	}

	if ((IsDataChanged() == TRUE) && (m_nOpenMode == modeNormal))
	{
		//ストラテジィを書き換えた場合、
		//レシピを読込んだ値と画面表示値(光学フィルタ・波長)に差がでてしまうことがあるので、レシピを強制的に書換える。
		SaveRecipeData();
		LoadStringML(IDS_SAVE_OPTICAL_FILTER_WAVELENG, l_strBuffer, "The optical filter and the wavelength were saved\n by the change in the strategy.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}
}

// =========================================================================
//
void CRecipeMeasurementSrDistanceProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_DistanceMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_DISTANCE) ) {
			m_DistanceMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
			m_DistanceMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_DISTANCE;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_DistanceMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {		   // RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_DistanceMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_DISTANCE) ) {
				m_DistanceMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
				m_DistanceMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_DISTANCE;
			}
		}
		break;
	}
	::CopyMemory(&m_OldDistanceMeas, &m_DistanceMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_DistanceMeas.hdr.szName, l_strBuffer);
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementSrDistanceProgramView::CheckData()
{
	CString l_strBuffer, l_strTitle;

	UpdateData(TRUE);

	CString strBuffer;
	CString strCaption;

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		LoadStringML(IDS_CAPTION_DEF_SETTING, l_strBuffer, "DEFAULT SETTING - ");
		strCaption = l_strBuffer;
		break;
	default:
		LoadStringML(IDS_CAPTION_MEASURE_PROG, l_strBuffer, "MEASUREMET PROGRAM - ");
		strCaption = l_strBuffer;
		break;
	}
	strCaption += m_strCaption;

	///// Lens /////
	if ( m_strLens.IsEmpty() )
	{
		LoadStringML(IDS_SELECT_LENS, l_strBuffer, "Please Select Lens.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		GetDlgItem(IDC_SR_LENS)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrDistanceProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldDistanceMeas, &m_DistanceMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrDistanceProgramView::SaveRecipeData()
{
	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_DistanceMeas, SR_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SR_DISTANCE) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_DistanceMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_DistanceMeas, m_szRecipeName, RECIPE_FILE_SR_DISTANCE) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldDistanceMeas, &m_DistanceMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrDistanceProgramView::IsAccessPrivilege()
{
	int iAccessItem;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( m_nOpenMode == modeDefaultSetting )
		iAccessItem = ACCESS_USERSETTING;
	else
		iAccessItem = ACCESS_PROGRAM;

	return pDoc->User_Access(iAccessItem);
}

// =========================================================================
//
int CRecipeMeasurementSrDistanceProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
BOOL CRecipeMeasurementSrDistanceProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
// レンズ コンボ選択時
//
void CRecipeMeasurementSrDistanceProgramView::OnSelchangeSrThickLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_DistanceMeas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}

// =========================================================================
//レンズ コンボイニシャル処理
//
void CRecipeMeasurementSrDistanceProgramView::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
		if ( m_SrTurret[i].bEnable && m_SrTurret[i].bAnalysis ) {
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	if(!(nIndex == CB_ERR)){
		pCombo->SetCurSel(nIndex);
	}
	else{
		if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
			m_DistanceMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.14 bagus SR --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus SR --{--
			m_DistanceMeas.ScanParams._SR.iLens = pCombo->GetItemData(0);
// 2009.12.09 bagus SR --}--
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
}

// =========================================================================
// オプティカルフィルター コンボ選択時
//
void CRecipeMeasurementSrDistanceProgramView::OnSelchangeOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_OPTICAL_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_DistanceMeas.ScanParams._SR.wOpticsFilterType = static_cast<long>(pCombo->GetItemData(nSelect));
}

// =========================================================================
//
void CRecipeMeasurementSrDistanceProgramView::InitCombo_OpticalFilter()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_OPTICAL_FILTER);

	//オートフィルタありの場合
	if (m_SrConfig.bAutoFilter){
// 2009.10.24 bagus 2点間 修正 --{--
//		pCombo->EnableWindow(FALSE);
// 2009.10.24 bagus 2点間 修正 --}--
		pCombo->ResetContent();

		int nIndex;
		int iFilterIndex;

		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

		LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
			{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Enu
			{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], m_srFilter[0].szName, m_srFilter[1].szName, m_srFilter[2].szName},	// Jpn
		};

		for ( int iOptItem = 0; iOptItem < OPT_FILTER_MAX; iOptItem++ ) {
			switch ( iOptItem ) {
			case OPT_FILTER_UNKNOWN:
				break;
			case OPT_FILTER_OPEN:
				nIndex = pCombo->AddString(pszFilterName[l_SystemConfig.nLanguage][iOptItem]);
				pCombo->SetItemData(nIndex, iOptItem);
				break;
			case OPT_FILTER_DARK:
				break;
			case OPT_FILTER_POS1:
			case OPT_FILTER_POS2:
			case OPT_FILTER_POS3:
				iFilterIndex = iOptItem - 3;
				if ( m_srFilter[iFilterIndex].bEnable ) {
					nIndex = pCombo->AddString(m_srFilter[iFilterIndex].szName);
					pCombo->SetItemData(nIndex, iOptItem);
				}
				break;
			default:
				break;
			}
		}

		if ((m_DistanceMeas.ScanParams._SR.wOpticsFilterType < 0) || (m_DistanceMeas.ScanParams._SR.wOpticsFilterType >= OPT_FILTER_MAX)) {
			nIndex = 0;
		} else {
			nIndex = pCombo->FindStringExact(-1, pszFilterName[l_SystemConfig.nLanguage][m_DistanceMeas.ScanParams._SR.wOpticsFilterType]);
		}

		if(!(nIndex == CB_ERR)){
			pCombo->SetCurSel(nIndex);
		}
		else{
			if(m_nOpenMode == modeNew || m_nOpenMode == modeDefaultSetting){
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.14 bagus SR --{--
				m_DistanceMeas.ScanParams._SR.wOpticsFilterType = static_cast<long>(pCombo->GetItemData(0));
// 2009.12.14 bagus SR --}--
			}
			else{
				nIndex = 0;
				pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus SR --{--
				m_DistanceMeas.ScanParams._SR.wOpticsFilterType = static_cast<long>(pCombo->GetItemData(0));
// 2009.12.09 bagus SR --}--
				SaveRecipeData();
				LoadStringML(IDS_OPTICAL_FILTER_ZERO, l_strBuffer, "Invalid for the specified item, Optical filter is saved as an INDEX to 0.");
				LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
				MessageBox(l_strBuffer, l_strTitle, MB_OK);
			}
		}
	}

	//オートフィルタなしの場合
	else{
		m_DistanceMeas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_THICK_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_THICK_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
//ダミーメジャーメントプログラムを作成
BOOL CRecipeMeasurementSrDistanceProgramView::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName)
{
	return RecipeFile_SaveRecipe(&m_DistanceMeas, pszRecipeName, RECIPE_FILE_SR_DISTANCE);
}

// =========================================================================
//ダミーメインプログラムを作成
BOOL CRecipeMeasurementSrDistanceProgramView::MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas)
{
	MAIN_RCP_INFO MainRcpInfo_Temp;
	memset(&MainRcpInfo_Temp, 0, sizeof(MainRcpInfo_Temp));


#if 0 //設定が必要なら追加すること
	MainRcpInfo_Temp.hdr.szComment;
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szStage, "---");
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szPointDeskew, "---");
	MainRcpInfo_Temp.MainRcpParam.hdr.nPointManualAdjustment;
	MainRcpInfo_Temp.MainRcpParam.hdr.bSampleID = FALSE;
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szSampleID, "---");
	MainRcpInfo_Temp.MainRcpParam.hdr.bSaveMeasResult = FALSE;
	MainRcpInfo_Temp.MainRcpParam.hdr.bSaveOverwrite = FALSE;
	MainRcpInfo_Temp.MainRcpParam._SR.RecalibItem[0] = 0; //
	strcpy(MainRcpInfo_Temp.MainRcpParam._SR.szRecalib[0], "---"); //
	MainRcpInfo_Temp.MainRcpParam._SR.nAutoFocusFailOption = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.nFinalFocusFailDataValidOption = 0;

	MainRcpInfo_Temp.MainRcpParam._SR.nDefaultDisplay = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.bMeasRef = FALSE;
#endif

	strcpy(MainRcpInfo_Temp.hdr.szName, pszRecipeNameMain);
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szMeas, pszRecipeNameMeas);

	MainRcpInfo_Temp.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
	MainRcpInfo_Temp.MainRcpParam._SR.nFocus = 0;
	MainRcpInfo_Temp.MainRcpParam._SR.bRefWithAF = TRUE;

	return RecipeFile_SaveRecipe(&MainRcpInfo_Temp, pszRecipeNameMain, RECIPE_FILE_SR_MAIN_RECIPE);
}

// =========================================================================
//仮レシピをNanoSpecDocへ格納
BOOL CRecipeMeasurementSrDistanceProgramView::TempRecipeDocSet(LPCTSTR pszRecipeName)
{
//『メインレシピ名』『メインレシピ』『メジャーメントレシピ』『表示用ラベル』『単位名』のみ格納
//および『仮メジャーメントプログラムのWave範囲』、『測定種別』

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	///// メモリー初期化 /////
	pDoc->InitMeasData();

	//測定種別
	rcp_data.MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_DISTANCE;

	strcpy(rcp_data.szRecipeName, pszRecipeName);
	//レシピロード＆NanoSpecDocへの格納
	BOOL bMain = FALSE;
	BOOL bMeas = FALSE;
	bMain = RecipeFile_LoadRecipe(&rcp_data.MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE);
	bMeas = RecipeFile_LoadRecipe(&rcp_data.MeasProgInfo, rcp_data.MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	if((bMain == FALSE) || (bMeas == FALSE))
	{
		return FALSE;
	}
	pDoc->SetRcpData(&rcp_data);


	///// 単位名取得 /////
	SR_FIGURE_FORMAT SrFigureFormat;
	char szUnit[MAX_PATH];
	memset(szUnit, 0, sizeof(szUnit));

	ConfigFile_GetNanoSpecIni(&SrFigureFormat, CONFIG_FILE_SR_FIGURE_FORMAT);

	int nUnit = SrFigureFormat.nThicknessUnit;
	if(nUnit >= 0 && nUnit < FIGURE_UNIT_SR_THICKNESS_MAX){
		strcpy(szUnit, g_lpszFigureUnitSrThickness[nUnit]);
	}
	else{
		strcpy(szUnit, " ");
	}
	pDoc->SetUnitName(szUnit);
	pDoc->SetUnitIndex(nUnit);


	///// 画面表示(データ保存)用タイトル /////
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//画面表示(データ保存)用タイトル ←X,Y,Z,AFは含まず
	int iDispLabelCount;
	memset(szDispLabel, 0, sizeof(szDispLabel)); //

	pMainFrame->GetStrategyhead(m_DistanceMeas.ScanParams._SR.XMPDesc.szStrategyEntry, szDispLabel, &iDispLabelCount);
	pDoc->SetDispLabel(szDispLabel);
	pDoc->SetDispLabelCount(iDispLabelCount);


	return TRUE;
}

// =========================================================================
//レシピ名作成
BOOL CRecipeMeasurementSrDistanceProgramView::GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType)
{
	CString l_strBuffer, l_strTitle;

	//固定名でレシピ名を作る
	char szRcpName[RECIPE_NAME_LEN + 1];
	char szRcpNamePath[MAX_PATH + 1];

	sprintf(szRcpName, "%c%s", SYSTEM_RECIPE_BEGINNING_CHAR, TESTMODE_TEMP_RECIPE_NAME); //作成レシピ名

	//作成するレシピ名が重複していないか調べる為のパスを作成
	if(iRcpType == SAVE_AS_MAIN_RECIPE) { //メインレシピ
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Main_Recipe_Dir, szRcpName, MAINRECIPE_EXT); //重複レシピ名検索用
	}
	else if(iRcpType == SAVE_AS_MEASUREMENT_PROGRAM) { //メジャーメントプログラム
		sprintf(szRcpNamePath, "%s%s%s", g_szDb_Measurement_Program_Dir, szRcpName, MEASUREMENTPGM_EXT); //重複レシピ名検索用
	}

	//作成しようとするレシピの重複チェック
	if(FileOrDirExists(szRcpNamePath)==TRUE) //作成しようとしているファイル名に重複あり
	{
		//ソフト起動時に仮レシピを消すので、ここには来ないはず。
		LoadStringML(IDS_CAPTION_SR_TRANSMIT, l_strBuffer, "Temporary recipe for the test mode remains, \nthe test mode cannot be begun.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		return FALSE; //ファイル名重複(ソフト起動時、仮レシピ消去ミス)
	}

	strcpy(pszFileName, szRcpName);
	return TRUE;
}

// =========================================================================
//ファイル存在チェック(ファイル存在時はTRUEを返す)
BOOL CRecipeMeasurementSrDistanceProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}

