// RecipeMeasurementMSProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "RecipeMeasurementMSProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementMSProgramView

IMPLEMENT_DYNCREATE(CRecipeMeasurementMSProgramView, CNanoRecipeUI)

CRecipeMeasurementMSProgramView::CRecipeMeasurementMSProgramView()
	: CNanoRecipeUI(CRecipeMeasurementMSProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeMeasurementMSProgramView)
		// メモ: ClassWizard はこの位置にメンバの初期化処理を追加します
	//}}AFX_DATA_INIT

	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_MS, l_strBuffer, "Micro Scope");
	m_strCaption = l_strBuffer;

	m_strLens = _T("");
}

CRecipeMeasurementMSProgramView::~CRecipeMeasurementMSProgramView()
{
}

void CRecipeMeasurementMSProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMeasurementMSProgramView)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しを追加します
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MS_PROG_NAME, m_MsMeas.hdr.szName, RECIPE_NAME_LEN + 1);
	//2009.12.25 bagus 修正 --{--
	DDX_Text(pDX, IDC_COMMENT, m_MsMeas.hdr.szComment, RECIPE_COMMENT_LEN + 1);
	//2009.12.25 bagus 修正 --}--
	DDX_CBStringExact(pDX, IDC_MS_LENS, m_strLens);
}


BEGIN_MESSAGE_MAP(CRecipeMeasurementMSProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMeasurementMSProgramView)
	ON_CBN_SELCHANGE(IDC_MS_LENS, OnSelchangeMsLens)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementMSProgramView 診断

#ifdef _DEBUG
void CRecipeMeasurementMSProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

void CRecipeMeasurementMSProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRecipeMeasurementMSProgramView メッセージ ハンドラ

void CRecipeMeasurementMSProgramView::OnInitialUpdate()
{
	CString l_strBuffer, l_strTitle;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);
	LoadRecipeData();

	m_strLens = m_MsConfig.RoboCylinder[m_MsMeas.ScanParams._MScope.iMagLensIndex].szName;

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
	m_stcProgName.SubclassWindow(GetDlgItem(IDC_MS_PROG_NAME)->GetSafeHwnd());
	m_stcProgName.SetBkColor(WATER_COLOR);

	///// Control Initialize /////
	InitCombo_Lens();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Default Recipe Setup /////
	if ( m_nOpenMode == modeDefaultSetting) {
		GetDlgItem(IDC_NAME_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MS_PROG_NAME)->ShowWindow(SW_HIDE);
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
void CRecipeMeasurementMSProgramView::LoadRecipeData()
{
	CString l_strBuffer;

	switch ( m_nOpenMode )
	{
	case modeNew:				// レシピ新規作成
	case modeDefaultSetting:	// コンフィグ画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_MsMeas, MS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_MS) ) {
			m_MsMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_MS;
			m_MsMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_MS_MEAS;
		}
		break;
	default:					// レシピメインリスト画面から遷移してきたケース
		if ( !RecipeFile_LoadRecipe(&m_MsMeas, m_szRecipeName, RECIPE_FILE_MEASUREMENT_PROGRAM) ) { 		 // RECIPE_FILE_MEASUREMENT_PROGRAM で正しいらしい...
			m_nOpenMode = modeNew;
			if ( !RecipeFile_LoadRecipe(&m_MsMeas, MS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_MS) ) {
				m_MsMeas.ScanParams.hdr.wHeadType = HEAD_TYPE_MS;
				m_MsMeas.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_MS_MEAS;
			}
		}
		break;
	}
	::CopyMemory(&m_OldMsMeas, &m_MsMeas, sizeof(MEAS_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_MsMeas.hdr.szName, l_strBuffer);
	}
}

// =========================================================================
//
BOOL CRecipeMeasurementMSProgramView::CheckData()
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
		GetDlgItem(IDC_MS_LENS)->SetFocus();
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementMSProgramView::IsDataChanged()
{
	UpdateData(TRUE);

	return memcmp(&m_OldMsMeas, &m_MsMeas, sizeof(MEAS_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeMeasurementMSProgramView::SaveRecipeData()
{
	switch ( m_nOpenMode )
	{
	case modeDefaultSetting:
		if ( !RecipeFile_SaveRecipe(&m_MsMeas, MS_HEAD_DEFAULT_NAME, RECIPE_FILE_DEF_MS) )
			return FALSE;
		break;
	default:
		// 新しいレシピ名を設定（Save, SaveAs兼用）
		strcpy(m_MsMeas.hdr.szName, m_szRecipeName);
		if ( !RecipeFile_SaveRecipe(&m_MsMeas, m_szRecipeName, RECIPE_FILE_MS) )
			return FALSE;
		break;
	}
	UpdateData(FALSE);
	::CopyMemory(&m_OldMsMeas, &m_MsMeas, sizeof(MEAS_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMeasurementMSProgramView::IsAccessPrivilege()
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
int CRecipeMeasurementMSProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_MEASUREMENT_PROGRAM;
}

// =========================================================================
//
BOOL CRecipeMeasurementMSProgramView::CheckTransData(int iData , CString strData)
{
	CString strTransiData;

	strTransiData.Format("%d",iData);
	return strTransiData == strData;
}

// =========================================================================
// レンズ コンボ選択時
//
void CRecipeMeasurementMSProgramView::OnSelchangeMsLens()
{
	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MS_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_MsMeas.ScanParams._MScope.iMagLensIndex = pCombo->GetItemData(nSelect);

	UpdateData(FALSE);
}

// =========================================================================
//レンズ コンボイニシャル処理
//
void CRecipeMeasurementMSProgramView::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MS_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++ ) {
		if (m_MsConfig.RoboCylinder[i].bEnable ) {
			nIndex = pCombo->AddString(m_MsConfig.RoboCylinder[i].szName);
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
// 2009.12.14 bagus MS --{--
			m_MsMeas.ScanParams._MScope.iMagLensIndex = pCombo->GetItemData(0);
// 2009.12.14 bagus MS --}--
		}
		else{
			nIndex = 0;
			pCombo->SetCurSel(nIndex);
// 2009.12.09 bagus MS --{--
			m_MsMeas.ScanParams._MScope.iMagLensIndex = pCombo->GetItemData(0);
// 2009.12.09 bagus MS --}--
			SaveRecipeData();
			LoadStringML(IDS_LENS_ZERO, l_strBuffer, "Invalid for the specified item, Lens is saved as an INDEX to 0.");
			LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
			MessageBox(l_strBuffer, l_strTitle, MB_OK);
		}
	}
}

//ファイル存在チェック(ファイル存在時はTRUEを返す)
BOOL CRecipeMeasurementMSProgramView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}

