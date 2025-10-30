// RecipeMeasurementCTAProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "TestMeasurementDlg.h"
#include "RecipeMeasurementCTAProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRecipeMeasurementCTAProgramView, CNanoRecipeUI)

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementCTAProgramView ダイアログ

// =========================================================================
//
CRecipeMeasurementCTAProgramView::CRecipeMeasurementCTAProgramView()
	: CNanoRecipeUI(CRecipeMeasurementCTAProgramView::IDD)
{
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_CTA, l_strBuffer, "CONTACT ANGLE");
	m_strCaption = l_strBuffer;

	//{{AFX_DATA_INIT(CRecipeMeasurementCTAProgramView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementCTAProgramView::~CRecipeMeasurementCTAProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementCTAProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementCTAProgramView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CONTACT_ANGLE, m_CTAMeas.ScanParams._CA.bContactAngle);
	DDX_Check(pDX, IDC_RADIUS, m_CTAMeas.ScanParams._CA.bRadius);
	DDX_Check(pDX, IDC_LIQUID_VOLUME, m_CTAMeas.ScanParams._CA.bLiquidVolume);
	DDX_Text(pDX, IDC_COMMENT, m_CTAMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
	DDX_Text(pDX, IDC_THICKNESS_PROG_NAME, m_CTAMeas.hdr.szName, RECIPE_NAME_LEN + 1);
}


BEGIN_MESSAGE_MAP(CRecipeMeasurementCTAProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementCTAProgramView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementCTAProgramView メッセージ ハンドラ

// =========================================================================
//
void CRecipeMeasurementCTAProgramView::OnInitialUpdate()
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

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	if ((IsDataChanged() == TRUE) && (m_nOpenMode == modeNormal))
	{
		//ストラテジィを書き換えた場合、
		//レシピを読込んだ値と画面表示値(光学フィルタ・波長)に差がでてしまうことがあるので、レシピを強制的に書換える。
		SaveRecipeData();
		LoadStringML(IDS_SAVE_OPTICAL_FILTER_WAVELENG, l_strBuffer, "The optical filter and the wavelength were saved\n by the change in the strategy.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
	}

	m_CTAMeas.ScanParams._CA.bContactAngle = TRUE;
	CheckDlgButton(IDC_CONTACT_ANGLE, TRUE);
	GetDlgItem(IDC_CONTACT_ANGLE)->EnableWindow(FALSE);

// 2010.01.06 bagus CTA --{--
	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
// 2010.01.13 bagus CTA --{--
		GetDlgItem(IDC_THICKNESS_PROG_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_THICKNESS_PROG_NAME)->ShowWindow(SW_HIDE);
// 2010.01.13 bagus CTA --}--
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
	}
// 2010.01.06 bagus CTA --}--
}

// =========================================================================
//
void CRecipeMeasurementCTAProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_CTAMeas, CTA_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_CTA) ) {
			m_CTAMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_CTA;
			m_CTAMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_CTA_MEAS;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_CTAMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {		  // RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_CTAMeas, CTA_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_CTA) ) {
				m_CTAMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_CTA;
				m_CTAMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_CTA_MEAS;
			}
		}
		break;
	}
	::CopyMemory(&m_OldCTAMeas, &m_CTAMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_CTAMeas.hdr.szName, l_strBuffer);
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementCTAProgramView::CheckData()
{
	CString l_strBuffer, l_strTitle;

	UpdateData(TRUE);

	CString strBuffer;
	CString strCaption;
	BOOL bValidWaveLen = FALSE;

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

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementCTAProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldCTAMeas, &m_CTAMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementCTAProgramView::SaveRecipeData()
{
	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_CTAMeas, CTA_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_CTA) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_CTAMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_CTAMeas, m_szRecipeName, RECIPE_FILE_CTA) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldCTAMeas, &m_CTAMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementCTAProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementCTAProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
BOOL CRecipeMeasurementCTAProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
//ダミーメジャーメントプログラムを作成
BOOL CRecipeMeasurementCTAProgramView::MakeTempCTAMeasurementRecipe(LPCTSTR pszRecipeName)
{
	return RecipeFile_SaveRecipe(&m_CTAMeas, pszRecipeName, RECIPE_FILE_CTA);
}

// =========================================================================
//ダミーメインプログラムを作成
BOOL CRecipeMeasurementCTAProgramView::MakeTempCTAMainRecipe(LPCTSTR pszRecipeNameMain, LPCTSTR pszRecipeNameMeas)
{
	MAIN_RCP_INFO MainRcpInfo_Temp;
	memset(&MainRcpInfo_Temp, 0, sizeof(MainRcpInfo_Temp));

	strcpy(MainRcpInfo_Temp.hdr.szName, pszRecipeNameMain);
	strcpy(MainRcpInfo_Temp.MainRcpParam.hdr.szMeas, pszRecipeNameMeas);

	MainRcpInfo_Temp.MainRcpParam.hdr.wHeadType = HEAD_TYPE_CTA;

	return RecipeFile_SaveRecipe(&MainRcpInfo_Temp, pszRecipeNameMain, RECIPE_FILE_CTA_MAIN_RECIPE);
}

// =========================================================================
//仮レシピをNanoSpecDocへ格納
BOOL CRecipeMeasurementCTAProgramView::TempRecipeDocSet(LPCTSTR pszRecipeName)
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
	rcp_data.MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_CTA_MEAS;

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

#if 0
	///// 画面表示(データ保存)用タイトル /////
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		//画面表示(データ保存)用タイトル ←X,Y,Z,AFは含まず
	int iDispLabelCount;
	memset(szDispLabel, 0, sizeof(szDispLabel)); //

	pMainFrame->GetStrategyhead(m_CTAMeas.ScanParams._SE.szStrategyEntry, szDispLabel, &iDispLabelCount);
	pDoc->SetDispLabel(szDispLabel);
	pDoc->SetDispLabelCount(iDispLabelCount);
#endif

	return TRUE;
}

// =========================================================================
//レシピ名作成
BOOL CRecipeMeasurementCTAProgramView::GetTestModeRecipeName(LPTSTR pszFileName, int iRcpType)
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
BOOL CRecipeMeasurementCTAProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}

