// RecipeMeasurementSeThicknessProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "TestMeasurementDlg.h"
#include "RecipeMeasurementSeThicknessProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementSeThicknessProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementSeThicknessProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementSeThicknessProgramView::CRecipeMeasurementSeThicknessProgramView()
	: CNanoRecipeUI(CRecipeMeasurementSeThicknessProgramView::IDD)
{
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SE_THICKNESS, l_strBuffer, "SE THICKNESS");
	m_strCaption = l_strBuffer;

	//{{AFX_DATA_INIT(CRecipeMeasurementSeThicknessProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementSeThicknessProgramView::~CRecipeMeasurementSeThicknessProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementSeThicknessProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementSeThicknessProgramView)
	DDX_Control(pDX, IDC_XMP_MEASUREMENT_ITEM_LIST, m_XmpMeasurementItemListCtrl);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_THICKNESS_PROG_NAME, m_ThickMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Text(pDX, IDC_SE_THICK_STRATEGY, m_ThickMeas.ScanParams._SE.szStrategyEntry, STRATEGY_NAME_LEN + 1);
	DDX_Text(pDX, IDC_COMMENT, m_ThickMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
// 2009.09.19 bagus SE --{--
	DDX_Text(pDX, IDC_START_WAVELENGTH, (short&)m_ThickMeas.ScanParams._SE.WavelenRange.wStart);
	DDX_Text(pDX, IDC_END_WAVELENGTH, (short&)m_ThickMeas.ScanParams._SE.WavelenRange.wEnd);
// 2009.09.19 bagus SE --}--
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementSeThicknessProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementSeThicknessProgramView)
	ON_CBN_SELCHANGE(IDC_SE_THICK_STRATEGY, OnSelchangeSeThickStrategy)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementSeThicknessProgramView メッセージ ハンドラ

// =========================================================================
//
void CRecipeMeasurementSeThicknessProgramView::OnInitialUpdate()
{
	CString l_strBuffer, l_strTitle;

	///// Load File /////
	LoadRecipeData();

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
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_THICKNESS_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	InitCombo_SeThickStrategy();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_THICKNESS_PROG_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_THICKNESS_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
	}

	InitItemList();
	OnSelchangeSeThickStrategy();
// 2009.09.19 bagus SE --{--
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);
// 2009.09.19 bagus SE --}--

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
void CRecipeMeasurementSeThicknessProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ThickMeas, SE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SE_THICKNESS) ) {
			m_ThickMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SE;
			m_ThickMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SE_THICKNESS;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ThickMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {			// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ThickMeas, SE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SE_THICKNESS) ) {
				m_ThickMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_SE;
				m_ThickMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SE_THICKNESS;
			}
		}
		break;
	}
	::CopyMemory(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_ThickMeas.hdr.szName, l_strBuffer);
	}
}
// =========================================================================
//
void CRecipeMeasurementSeThicknessProgramView::InitCombo_SeThickStrategy()
{
	CString l_strBuffer;
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 -->
	BOOL bSe, bDummy, bDummy2;
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 <--

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SE_THICK_STRATEGY);

	pCombo->ResetContent();

	SE_XMP SeXmp;
	ConfigFile_GetNanoSpecIni(&SeXmp, CONFIG_FILE_SE_XMP);
	CString strBuffer;

	TRY
	{
		CStdioFile stdioFile(SeXmp.szStrategyListPath, CFile::modeRead);
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 -->
		while ( stdioFile.ReadString(strBuffer) ) {
			MEAS_GetStratetyType(bDummy, bSe, bDummy2, strBuffer);
			if ( bSe )
				pCombo->AddString(strBuffer);
		}
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 <--
		stdioFile.Close();
	}
	CATCH(CFileException, e)
	{
		CString strCause;
		LoadStringML(IDS_CAUSE_CODE, l_strBuffer, "CauseCode:%d\n%s");
		strCause.Format(l_strBuffer, e->m_cause, e->m_strFileName);

		switch ( e->m_cause )
		{
		case CFileException::fileNotFound :
			LoadStringML(IDS_FILE_NOT_FOUND, l_strBuffer, " FileNotFound");
			strCause += l_strBuffer;
			break;
		case CFileException::badPath :
			LoadStringML(IDS_BAD_PATH, l_strBuffer, " BadPath");
			strCause += l_strBuffer;
			break;
		default :
			break;
		}
		MessageBox(strCause, NULL, MB_ICONERROR | MB_OK);
	}
	END_CATCH

	int nIndex = pCombo->FindStringExact(-1, m_ThickMeas.ScanParams._SE.szStrategyEntry);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
BOOL CRecipeMeasurementSeThicknessProgramView::CheckData()
{
	CString l_strBuffer, l_strTitle;

	UpdateData(TRUE);

	CString strBuffer;
	CString strCaption;
	BOOL bValidWaveLen = FALSE;

// 2009.09.24 K.Matsuo delete -->
// 2009.09.19 bagus SE --{--
//	int MinWaveLength;
//	int MaxWaveLength;
//	const WORD wStart = m_ThickMeas.ScanParams._SE.WavelenRange.wStart;
//	const WORD wEnd = m_ThickMeas.ScanParams._SE.WavelenRange.wEnd;
// 2009.09.19 bagus SE --}--
// 2009.09.24 K.Matsuo delete <--

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

	///// Strategy /////
	if ( strlen(m_ThickMeas.ScanParams._SE.szStrategyEntry) == 0 )
	{
		LoadStringML(IDS_STRATEGY_EMPTY, l_strBuffer, "Strategy is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

// 2009.09.24 K.Matsuo delete -->
// 2009.09.19 bagus SE --{--
//	///// Wave Length /////
//	if ( wStart >=	wEnd)
//	{
//		LoadStringML(IDS_START_WAVE_LARGER, l_strBuffer, "Start Wavelength is Larger than End Wavelength.");
//		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
//		GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(TRUE);
//		GetDlgItem(IDC_WAVELENGTH_START)->SetFocus();
//		return FALSE;
//	}
//
//	MinWaveLength = MIN_TARGET_WAVELENGTH;
//	MaxWaveLength = MAX_TARGET_WAVELENGTH;
//
//	if ( wStart < MinWaveLength || MaxWaveLength < wStart )
//	{
//		LoadStringML(IDS_START_WAVE_OUT_RANGE, l_strBuffer, "Start Wavelength is out of range.""\n(%d - %d)");
//		strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength);
//		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
//		GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(TRUE);
//		GetDlgItem(IDC_WAVELENGTH_START)->SetFocus();
//		return FALSE;
//	}
//
//	if ( wEnd < MinWaveLength || MaxWaveLength < wEnd )
//	{
//		LoadStringML(IDS_END_WAVE_OUT_RANGE, l_strBuffer, "End Wavelength is out of range.""\n(%d - %d)");
//		strBuffer.Format(l_strBuffer, MinWaveLength, MaxWaveLength);
//		MessageBox(strBuffer, strCaption, MB_OK | MB_ICONSTOP);
//		GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(TRUE);
//		GetDlgItem(IDC_WAVELENGTH_END)->SetFocus();
//		return FALSE;
//	}
// 2009.09.19 bagus SE --}--
// 2009.09.24 K.Matsuo delete <--

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSeThicknessProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementSeThicknessProgramView::SaveRecipeData()
{
// 2009.09.29 bagus Stress --{--
#if 0
// 2009.09.19 bagus SE --{--
	SR_CONFIG	SrConfig;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	m_ThickMeas.ScanParams._SE.iLens = SrConfig.iDefaultMeasLensNo;
	m_ThickMeas.ScanParams._SE.wOpticsFilterType = SrConfig.wDefaultMeasFilter;
// 2009.09.19 bagus SE --}--
#endif
// 2009.09.29 bagus Stress --}--

	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ThickMeas, SE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_SE_THICKNESS) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ThickMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ThickMeas, m_szRecipeName, RECIPE_FILE_SE_THICKNESS) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementSeThicknessProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementSeThicknessProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
void CRecipeMeasurementSeThicknessProgramView::InitItemList()
{
	LV_COLUMN listcol;
	int ItemMax;

	int Width[] = {
		40, 	// (0) Index
		79, 	// (1) Item
	};

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	LPTSTR pszItemJPN[] = {
		"Index",
		"アイテム",
	};
	LPTSTR pszItemENU[] = {
		"Index",
		"Item",
	};

//	  int Fmt[] = { LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };
	int Fmt[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };


	ItemMax = sizeof(Width) / sizeof(int);

	m_XmpMeasurementItemListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		if(l_SystemConfig.nLanguage == 0){
			listcol.pszText = pszItemENU[i];
		}
		else{
			listcol.pszText = pszItemJPN[i];
		}
		m_XmpMeasurementItemListCtrl.InsertColumn(i,&listcol);
	}

	m_XmpMeasurementItemListCtrl.DeleteAllItems();
}

// =========================================================================
//
void CRecipeMeasurementSeThicknessProgramView::OnSelchangeSeThickStrategy()
{
	UpdateData(TRUE);

	TCHAR szBuff[256];
	TCHAR szDdeTextItem[1024];
	LPTSTR token;
	int iItem;
	int iStartWaveLen = 380;
	int iEndWaveLen = 1000;
	int iOpticalFilter = 0;

	m_XmpMeasurementItemListCtrl.DeleteAllItems();

	if ( !MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iStartWaveLen, iEndWaveLen, iOpticalFilter, m_ThickMeas.ScanParams._SE.szStrategyEntry) ) {
		// デフォルト値設定
		m_ThickMeas.ScanParams._SE.WavelenRange.wStart = iStartWaveLen;
		m_ThickMeas.ScanParams._SE.WavelenRange.wEnd = iEndWaveLen;
		UpdateData(FALSE);
//		m_ThickMeas.ScanParams._SE.wOpticsFilterType = iOpticalFilter;
		return;
	}

	m_ThickMeas.ScanParams._SE.WavelenRange.wStart = iStartWaveLen;
	m_ThickMeas.ScanParams._SE.WavelenRange.wEnd = iEndWaveLen;
	UpdateData(FALSE);

	token = _tcstok(szDdeTextItem, _T(","));
	while ( token ) {
		iItem = m_XmpMeasurementItemListCtrl.GetItemCount();
		_stprintf(szBuff, _T("%d"), iItem + 1);
		m_XmpMeasurementItemListCtrl.InsertItem(iItem, szBuff);

		if(strcmp(token, "MSE") == 0)
		{
			//『MSE→Fit』の置換
			m_XmpMeasurementItemListCtrl.SetItem(iItem, 1, LVIF_TEXT, /*token*/"Fit", 0, 0, 0, NULL);
		}
		else
		{
			m_XmpMeasurementItemListCtrl.SetItem(iItem, 1, LVIF_TEXT, token, 0, 0, 0, NULL);
		}

		token = _tcstok(NULL, _T(","));
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementSeThicknessProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
//ダミーメジャーメントプログラムを作成
BOOL CRecipeMeasurementSeThicknessProgramView::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName)
{
	return RecipeFile_SaveRecipe(&m_ThickMeas, pszRecipeName, RECIPE_FILE_SE_THICKNESS);
}

// =========================================================================
//ダミーメインプログラムを作成
BOOL CRecipeMeasurementSeThicknessProgramView::MakeTempSrMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas)
{
	MAIN_RCP_INFO MainRcpInfo_Temp;
	memset(&MainRcpInfo_Temp, 0, sizeof(MainRcpInfo_Temp));

	strcpy(MainRcpInfo_Temp.hdr.szName, pszRecipeNameMain);
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szMeas, pszRecipeNameMeas);

	MainRcpInfo_Temp.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SE;

	return RecipeFile_SaveRecipe(&MainRcpInfo_Temp, pszRecipeNameMain, RECIPE_FILE_SE_MAIN_RECIPE);
}

// =========================================================================
//仮レシピをNanoSpecDocへ格納
BOOL CRecipeMeasurementSeThicknessProgramView::TempRecipeDocSet(LPCTSTR pszRecipeName)
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
	rcp_data.MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SE_THICKNESS;

// 2009.09.19 bagus SE --{--
	//Wave範囲セット
	rcp_data.MeasProgInfo.ScanParams._SE.WavelenRange.wStart = (int)GetDlgItemInt(IDC_WAVELENGTH_START, NULL, FALSE);
	rcp_data.MeasProgInfo.ScanParams._SE.WavelenRange.wEnd = (int)GetDlgItemInt(IDC_WAVELENGTH_END, NULL, FALSE);
// 2009.09.19 bagus SE --}--


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
	SE_FIGURE_FORMAT SeFigureFormat;
	char szUnit[MAX_PATH];
	memset(szUnit, 0, sizeof(szUnit));

	ConfigFile_GetNanoSpecIni(&SeFigureFormat, CONFIG_FILE_SE_FIGURE_FORMAT);

	int nUnit = SeFigureFormat.nThicknessUnit;
	if(nUnit >= 0 && nUnit < FIGURE_UNIT_SE_THICKNESS_MAX){
		strcpy(szUnit, g_lpszFigureUnitSeThickness[nUnit]);
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

	pMainFrame->GetStrategyhead(m_ThickMeas.ScanParams._SE.szStrategyEntry, szDispLabel, &iDispLabelCount);
	pDoc->SetDispLabel(szDispLabel);
	pDoc->SetDispLabelCount(iDispLabelCount);


	return TRUE;
}

// =========================================================================
//レシピ名作成
BOOL CRecipeMeasurementSeThicknessProgramView::GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType)
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
BOOL CRecipeMeasurementSeThicknessProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}

