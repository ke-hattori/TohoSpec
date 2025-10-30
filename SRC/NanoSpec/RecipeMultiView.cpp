// RecipeMultiView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MultiLangHelper.h"
#include "RecipeMultiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipeMultiView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CRecipeMultiView, CNanoRecipeUI)

// =========================================================================
//
CRecipeMultiView::CRecipeMultiView()
	: CNanoRecipeUI(CRecipeMultiView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeMultiView)
	//}}AFX_DATA_INIT

// Kojika 20090527 Change
//	m_strCaption = "MULTI RECIPE";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_SR_THICKNESS, l_strBuffer, "MULTI RECIPE");
	m_strCaption = l_strBuffer;
// Kojika 20090527 Change End

	m_iWnd = RECIPE_MULTI_WND;

	::ZeroMemory(&m_MultiRcpInfoHdr, sizeof(m_MultiRcpInfoHdr));
	::ZeroMemory(&m_OldMultiRcpInfoHdr, sizeof(m_OldMultiRcpInfoHdr));
	::ZeroMemory(&m_MultiRcpMainRcpList, sizeof(m_MultiRcpMainRcpList));
	::ZeroMemory(&m_OldMultiRcpMainRcpList, sizeof(m_OldMultiRcpMainRcpList));
	m_iNumMainRecipe = 0;
}

// =========================================================================
//
CRecipeMultiView::~CRecipeMultiView()
{
}

// =========================================================================
//
void CRecipeMultiView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMultiView)
	DDX_Control(pDX, IDC_MAIN_RECIPE_LIST, m_mainListCtrl);
	DDX_Control(pDX, IDC_MULTI_RECIPE_LIST, m_multiListCtrl);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_MULTI_RECIPE_ADD_BUTTON, m_MultiRecipeAddButton);
	DDX_Control(pDX, IDC_MULTI_RECIPE_DELETE_BUTTON, m_MultiRecipeDeleteButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeMultiView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeMultiView)
	ON_BN_CLICKED(IDC_MULTI_RECIPE_ADD_BUTTON, OnMultiRecipeAddButton)
	ON_BN_CLICKED(IDC_MULTI_RECIPE_DELETE_BUTTON, OnMultiRecipeDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CRecipeMultiView 診断

#ifdef _DEBUG
// =========================================================================
//
void CRecipeMultiView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

// =========================================================================
//
void CRecipeMultiView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CRecipeMultiView メッセージ ハンドラ

// =========================================================================
//
void CRecipeMultiView::OnInitialUpdate()
{
	///// Load Multi Recip Main Recipe File /////
	LoadRecipeData();

	CNanoRecipeUI::OnInitialUpdate();

	///// Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Static Control /////
	m_stcMultiRecipeName.SubclassWindow(GetDlgItem(IDC_MULTI_RECIPE_NAME)->GetSafeHwnd());
	m_stcMultiRecipeName.SetBkColor(WATER_COLOR);

	///// Multi Recipe Add Button /////
	m_MultiRecipeAddButton.SetIcon(IDI_ARROW_LEFT, (int)BTNST_AUTO_GRAY);
	m_MultiRecipeAddButton.DrawBorder(FALSE);

	///// Multi Recipe Delete Button /////
	m_MultiRecipeDeleteButton.SetIcon(IDI_GARBAGE_CAN, (int)BTNST_AUTO_GRAY);
	m_MultiRecipeDeleteButton.DrawBorder(TRUE);

	///// Get Current Execute Window /////
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_iWnd = pMainFrame->GetCurrExeWnd();

	///// List Init /////
	MainRecipeList_Init();
	MultiRecipeList_Init();

	///// Set Data /////
	SetData();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);
}

// =========================================================================
//
void CRecipeMultiView::OnMultiRecipeAddButton()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	int iMainRecipeIndex;
	char szMainRecipeName[RECIPE_NAME_LEN + 1];
	int nIndex;
	int iMultiRecipeCount;

	iMainRecipeIndex = m_mainListCtrl.GetSelectionMark();
	if ( iMainRecipeIndex == -1 )
		return;

	iMultiRecipeCount = m_multiListCtrl.GetItemCount();
	if ( iMultiRecipeCount >= MULTI_RCP_MAIN_RCP_MAX ) {
// Kojika 20090527 Change
//		MessageBox("Can not add to Multi Recipe." "\n(Max 100)", m_strCaption, MB_ICONSTOP | MB_OK);
		LoadStringML(IDS_NOT_ADD_MULTI_RECIPE, l_strBuffer, "Can not add to Multi Recipe." "\n(Max 100)");
		MessageBox(l_strBuffer, m_strCaption, MB_ICONSTOP | MB_OK);
// Kojika 20090527 Change End
		return;
	}

	m_mainListCtrl.GetItemText(iMainRecipeIndex, 1, szMainRecipeName, sizeof(szMainRecipeName));

	nIndex = List_FindItem(&m_multiListCtrl, 1, szMainRecipeName);

	if ( nIndex >= 0 ) {
// Kojika 20090527 Change
//		MessageBox("This Main Recipe is already exists.", m_strCaption, MB_ICONSTOP | MB_OK);
		LoadStringML(IDS_MULTI_RECIPE_EXIST, l_strBuffer, "This Main Recipe is already exists.");
		MessageBox(l_strBuffer, m_strCaption, MB_ICONSTOP | MB_OK);
// Kojika 20090527 Change End
		return;
	}

	MultiRecipeList_Add(szMainRecipeName);
}

// =========================================================================
//
void CRecipeMultiView::OnMultiRecipeDeleteButton()
{
	int iIndex;
	int iListCount;

	iIndex = m_multiListCtrl.GetSelectionMark();
	if ( iIndex == -1 )
		return;

	iListCount = m_multiListCtrl.GetItemCount();

	m_multiListCtrl.DeleteItem(iIndex);
	if ( iIndex == iListCount - 1 )
		iIndex--;

	m_multiListCtrl.SetSelectionMark(iIndex);
	if ( iIndex != -1 )
		m_multiListCtrl.SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
}

// =========================================================================
//
void CRecipeMultiView::MainRecipeList_Init()
{
	LV_COLUMN listcol;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER };

	int Width[] = {
		0,		// (0) Index
// 2009.12.09 bagus --{--
//		240,	// (1) Name
//		70,		// (2) Head Type
		200,	// (1) Name
		110,		// (2) Head Type
// 2009.12.09 bagus --}--
		170,	// (3) Measurement Item
	};

	LPTSTR pszItem[] = {
		"",
		"Name",
		"Head",
		"Meas Item",
	};

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);

	m_mainListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for ( int i = 0; i < ItemMax; i++ ) {
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		listcol.pszText = pszItem[i];
		m_mainListCtrl.InsertColumn(i,&listcol);
	}

	m_mainListCtrl.DeleteAllItems();
}

// =========================================================================
//
void CRecipeMultiView::MainRecipeList_Add(LPTSTR lpszName, int iHeadType, int iMeasType)
{
	LV_ITEM listitem;
	int item;

	listitem.iItem = m_mainListCtrl.GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item = m_mainListCtrl.InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = lpszName;
	m_mainListCtrl.SetItem(&listitem);

	///// Head /////
	listitem.iSubItem = 2;
	listitem.pszText = (char*)HEAD_TYPE_ITEM[iHeadType];
	m_mainListCtrl.SetItem(&listitem);

	///// Measurement Item /////
	listitem.iSubItem = 3;
	CMLScanType mlScanTypeItem(iHeadType);
	listitem.pszText = (LPTSTR)((LPCTSTR)mlScanTypeItem[iMeasType]);
	m_mainListCtrl.SetItem(&listitem);

	m_mainListCtrl.SetItemState(item, LVIS_SELECTED, LVIS_SELECTED);
//	m_mainListCtrl.EnsureVisible(item, FALSE);
}

// =========================================================================
//
void CRecipeMultiView::MainRecipeList_DataSet()
{
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	CString strMainRecipeName;
	CString strMeasProgName;
	BOOL bLoop;
	CTime time;
	MAIN_RCP_INFO MainRcpInfo;
	MEAS_PROG_INFO MeasProgInfo;
	WORD wHeadType;
	WORD wScanType;

	m_mainListCtrl.DeleteAllItems();
	sprintf(szDirPath,	"%s*%s", g_szDb_Main_Recipe_Dir, MAINRECIPE_EXT);

	bLoop = Finder.FindFile(szDirPath);

	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

		memset(&MainRcpInfo, 0, sizeof(MainRcpInfo));
		memset(&MeasProgInfo, 0, sizeof(MeasProgInfo));

		///// Load Main Recipe /////
		// ファイル名を取得
		strMainRecipeName = Finder.GetFileTitle();
		if ( strMainRecipeName[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		strMainRecipeName.Insert(RECIPE_NAME_LEN, '\0');
		if(!RecipeFile_LoadRecipe(&MainRcpInfo, strMainRecipeName.GetBuffer(0), RECIPE_FILE_MAIN_RECIPE)){
			continue;
		}

		/////// Check Measurement Program Existence /////
		//strMeasProgName = MainRcpInfo.MainRcpParam.hdr.szMeas;
		//if(!RecipeFile_LoadMeasurementProgram(&MeasProgInfo, strMeasProgName)){
		//	continue;
		//}

		///// Add Main Recipe List /////
		wHeadType = MainRcpInfo.MainRcpParam.hdr.wHeadType;
		wScanType = MeasProgInfo.ScanParams.hdr.wScanType;
		MainRecipeList_Add(strMainRecipeName.GetBuffer(0), (int)wHeadType, (int)wScanType);
	}
}

// =========================================================================
//
void CRecipeMultiView::MultiRecipeList_Init()
{
	LV_COLUMN listcol;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT };

	int Width[] = {
		0,		// (0) Index
		240,	// (1) Name
	};

	LPTSTR pszItem[] = {
		"",
		"Main Recipe Name",
	};

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);

	m_multiListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for ( int i = 0; i < ItemMax; i++) {
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		listcol.pszText = pszItem[i];
		m_multiListCtrl.InsertColumn(i,&listcol);
	}

	m_multiListCtrl.DeleteAllItems();
}

// =========================================================================
//
void CRecipeMultiView::MultiRecipeList_Add(LPTSTR lpszName)
{
	LV_ITEM listitem;
	int item;

	listitem.iItem = m_multiListCtrl.GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item =m_multiListCtrl.InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = lpszName;
	m_multiListCtrl.SetItem(&listitem);

	m_multiListCtrl.SetSelectionMark(listitem.iItem);
	m_multiListCtrl.SetItemState(item, LVIS_SELECTED, LVIS_SELECTED);
//	m_multiListCtrl.EnsureVisible(item, FALSE);
}

// =========================================================================
//
void CRecipeMultiView::MultiRecipeList_DataSet()
{
	for ( int i = 0; i < m_iNumMainRecipe; i++ )
		MultiRecipeList_Add(m_MultiRcpMainRcpList[i].szName);
}

// =========================================================================
//
void CRecipeMultiView::LoadRecipeData()
{
	// 新規作成の場合
	if ( m_nOpenMode == modeNew ) {
		;
	}
	else {
		///// Multi Recipe Header /////
		if ( !RecipeFile_LoadRecipe(&m_MultiRcpInfoHdr, m_szRecipeName, RECIPE_FILE_MULTI_RECIPE) ) {
			m_nOpenMode = modeNew;
		}
		else {
			::CopyMemory(&m_OldMultiRcpInfoHdr, &m_MultiRcpInfoHdr, sizeof(MULTI_RCP_INFO_HDR));
			m_iNumMainRecipe = (int)m_MultiRcpInfoHdr.wNumMainRecipe;
			if ( m_iNumMainRecipe == 0 ) {
				return;
			}

			///// Multi Recipe List /////
			if ( !RecipeFile_LoadMainRecipeList(m_MultiRcpMainRcpList, m_iNumMainRecipe, m_szRecipeName) ) {
				m_iNumMainRecipe = 0;
			}

			::CopyMemory(&m_OldMultiRcpMainRcpList, &m_MultiRcpMainRcpList, sizeof(m_MultiRcpMainRcpList));
		}
	}

	if ( m_nOpenMode == modeNew ) {
		strcpy(m_MultiRcpInfoHdr.hdr.szName, "Untitled");
	}
}

// =========================================================================
//
void CRecipeMultiView::SetData()
{
	///// Name /////
	SetDlgItemText(IDC_MULTI_RECIPE_NAME, m_MultiRcpInfoHdr.hdr.szName);

	///// Main Recipe List /////
	MainRecipeList_DataSet();

	if ( m_mainListCtrl.GetItemCount() > 0 ) {
		m_mainListCtrl.SetSelectionMark(0);
		m_mainListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	///// Multi Recipe List /////
	MultiRecipeList_DataSet();

	if ( m_multiListCtrl.GetItemCount() > 0 ) {
		m_multiListCtrl.SetSelectionMark(0);
		m_multiListCtrl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	///// Comment /////
	SetDlgItemText(IDC_COMMENT, m_MultiRcpInfoHdr.hdr.szComment);
}

// =========================================================================
//
void CRecipeMultiView::UpDate()
{
	char szMainRecipeName[RECIPE_NAME_LEN + 1];
	char szBuff[256];
	SYSTEMTIME lastWriteSystemTime;

	///// Multi Recipe /////
	m_iNumMainRecipe = m_multiListCtrl.GetItemCount();

	m_MultiRcpInfoHdr.wNumMainRecipe = (WORD)m_iNumMainRecipe;

	for ( int i = 0; i < m_iNumMainRecipe; i++ ) {
		m_multiListCtrl.GetItemText(i, 1, szMainRecipeName, RECIPE_NAME_LEN + 1);
		if ( !RecipeFile_ExistRecipe(szMainRecipeName, &lastWriteSystemTime, RECIPE_FILE_MAIN_RECIPE) ) {
			sprintf(szBuff, "\"%s\" does not exist", szMainRecipeName);
			MessageBox(szBuff, m_strCaption, MB_ICONSTOP | MB_OK);
			return;
		}
		strcpy(m_MultiRcpMainRcpList[i].szName, szMainRecipeName);
	}

	///// Comment /////
	GetDlgItemText(IDC_COMMENT, m_MultiRcpInfoHdr.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// ==========================================================================
//
BOOL CRecipeMultiView::SaveRecipeData()
{
	// 新しいレシピ名を設定（Save, SaveAs兼用）
	strcpy(m_MultiRcpInfoHdr.hdr.szName, m_szRecipeName);
	if ( !RecipeFile_SaveRecipe(&m_MultiRcpInfoHdr, m_MultiRcpInfoHdr.hdr.szName, RECIPE_FILE_MULTI_RECIPE) )
		return FALSE;

	if ( !RecipeFile_SaveMainRecipeList(m_MultiRcpMainRcpList, m_MultiRcpInfoHdr.wNumMainRecipe, m_MultiRcpInfoHdr.hdr.szName) )
		return FALSE;

	SetDlgItemText(IDC_MULTI_RECIPE_NAME, m_MultiRcpInfoHdr.hdr.szName);
	::CopyMemory(&m_OldMultiRcpInfoHdr, &m_MultiRcpInfoHdr, sizeof(MULTI_RCP_INFO_HDR));
	::CopyMemory(&m_OldMultiRcpMainRcpList, &m_MultiRcpMainRcpList, sizeof(m_MultiRcpMainRcpList));

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeMultiView::CheckData()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	UpDate();

	if ( m_multiListCtrl.GetItemCount() == 0) {
// Kojika 20090527 Change
//		MessageBox("Multi recipe is empty.", m_strCaption, MB_ICONSTOP | MB_OK);
		LoadStringML(IDS_MULTI_RECIPE_EMPTY, l_strBuffer, "Multi recipe is empty.");
		MessageBox(l_strBuffer, m_strCaption, MB_ICONSTOP | MB_OK);
// Kojika 20090527 Change End
		return FALSE;
	}

	return TRUE;
}

// ==========================================================================
//
BOOL CRecipeMultiView::IsDataChanged()
{
	UpDate();

	return ( memcmp(&m_MultiRcpInfoHdr, &m_OldMultiRcpInfoHdr, sizeof(MULTI_RCP_INFO_HDR)) != 0 ||
			 memcmp(&m_MultiRcpMainRcpList, &m_OldMultiRcpMainRcpList, sizeof(MULTI_RCP_MAIN_RCP_LIST)) != 0 );
}

// =========================================================================
//
BOOL CRecipeMultiView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	switch ( m_iWnd ) {
	case MANUAL_RECIPE_WND:
		return FALSE;
	case RECIPE_MULTI_WND:
		///// User Access Privilege /////
		return pDoc->User_Access(ACCESS_RECIPE);
		break;
	}

	return FALSE;
}

// =========================================================================
//
int CRecipeMultiView::SaveAsDlgInfo()
{
	return SAVE_AS_MULTI_RECIPE;
}
