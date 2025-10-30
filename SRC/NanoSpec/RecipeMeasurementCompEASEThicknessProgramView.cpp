// RecipeMeasurementCompEASEThicknessProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "TestMeasurementDlg.h"
#include "..\\..\\INC\\measys.hxx"
#include "RecipeMeasurementCompEASEThicknessProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMeasurementCompEASEThicknessProgramView
// #########################################################################

IMPLEMENT_DYNCREATE(CRecipeMeasurementCompEASEThicknessProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMeasurementCompEASEThicknessProgramView::CRecipeMeasurementCompEASEThicknessProgramView()
	: CNanoRecipeUI(CRecipeMeasurementCompEASEThicknessProgramView::IDD)
{
	CString l_strBuffer;

	LoadStringML(IDS_TITLE_COMPEASE_THICKNESS, l_strBuffer, "EASE THICKNESS");
	m_strCaption = l_strBuffer;

	//{{AFX_DATA_INIT(CRecipeMeasurementCompEASEThicknessProgramView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeMeasurementCompEASEThicknessProgramView::~CRecipeMeasurementCompEASEThicknessProgramView()
{
}

// =========================================================================
//
void CRecipeMeasurementCompEASEThicknessProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementCompEASEThicknessProgramView)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_THICKNESS_PROG_NAME, m_ThickMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_CompEASERecipeText(pDX, IDC_COMPEASE_RECIPE, m_ThickMeas.ScanParams._COMPEASE.szCompEASERecipe, COMPEASE_RECIPE_NAME_LEN + 1);
	DDX_Check(pDX, IDC_COMPEASE_RESULT_AUTO_SAVE_CHECK, m_ThickMeas.ScanParams._COMPEASE.bAutoSaveResultInCompEASE);
	DDX_Text(pDX, IDC_COMMENT, m_ThickMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// =========================================================================
//
void CRecipeMeasurementCompEASEThicknessProgramView::DDX_CompEASERecipeText(CDataExchange* pDX, int nIDC, LPTSTR value, int nMaxLen)
{
	CString strBuffer;

	if ( pDX->m_bSaveAndValidate ) {
		DDX_Text(pDX, nIDC, strBuffer);
		strBuffer += ".recipe";
		strncpy(value, strBuffer, nMaxLen - 1);
	}
	else {
		strBuffer = value;
		strBuffer.Replace(".recipe", "");
		DDX_Text(pDX, nIDC, strBuffer);
	}
}

BEGIN_MESSAGE_MAP(CRecipeMeasurementCompEASEThicknessProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementCompEASEThicknessProgramView)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
// CRecipeMeasurementCompEASEThicknessProgramView メッセージ ハンドラ

// =========================================================================
//
void CRecipeMeasurementCompEASEThicknessProgramView::OnInitialUpdate()
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
	InitCombo_CompEASERecipe();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_THICKNESS_PROG_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_THICKNESS_PROG_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMMENT)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
//
void CRecipeMeasurementCompEASEThicknessProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ThickMeas, COMPEASE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_COMPEASE_THICKNESS) ) {
			m_ThickMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_COMPEASE;
			m_ThickMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_COMPEASE_THICKNESS;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_ThickMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {			// RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_ThickMeas, COMPEASE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_COMPEASE_THICKNESS) ) {
				m_ThickMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_COMPEASE;
				m_ThickMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_COMPEASE_THICKNESS;
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
void CRecipeMeasurementCompEASEThicknessProgramView::InitCombo_CompEASERecipe()
{
	BOOL bRet;
	CStringArray* pListRecipes = NULL; // データをAddする側（dll側）で領域を確保しないとデストラクタで例外発生します
	CString strBuffer;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMPEASE_RECIPE);

	bRet = MEAS_CompEASEHead_ListRecipes(&pListRecipes);
	if ( bRet ) {
		if ( pListRecipes != NULL ) {
			for ( int i = 0; i < pListRecipes->GetSize(); i++ ) {
				strBuffer = pListRecipes->GetAt(i);
				strBuffer.Replace(".recipe", "");
				pCombo->AddString(strBuffer);
			}
		}
	}

	if ( pListRecipes != NULL ) {
		delete pListRecipes;
		pListRecipes = NULL;
	}

	strBuffer = m_ThickMeas.ScanParams._COMPEASE.szCompEASERecipe;
	strBuffer.Replace(".recipe", "");
	int nIndex = pCombo->FindStringExact(-1, strBuffer);
	pCombo->SetCurSel(nIndex);
}

// =========================================================================
//
BOOL CRecipeMeasurementCompEASEThicknessProgramView::CheckData()
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

	///// CompleteEASE Recipe /////
	if ( _tcsicmp(m_ThickMeas.ScanParams._COMPEASE.szCompEASERecipe, ".recipe") == 0 )
	{
		LoadStringML(IDS_COMPEASE_RECIPE_EMPTY, l_strBuffer, "CompleteEASE recipe is empty.");
		MessageBox(l_strBuffer, strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementCompEASEThicknessProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementCompEASEThicknessProgramView::SaveRecipeData()
{
	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_ThickMeas, COMPEASE_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_COMPEASE_THICKNESS) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_ThickMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_ThickMeas, m_szRecipeName, RECIPE_FILE_COMPEASE_THICKNESS) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldThickMeas, &m_ThickMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementCompEASEThicknessProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementCompEASEThicknessProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}
