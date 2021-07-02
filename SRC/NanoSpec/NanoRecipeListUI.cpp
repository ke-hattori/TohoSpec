// NanoListUI.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoRecipeListUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CNanoRecipeListUI
// #########################################################################

IMPLEMENT_DYNCREATE(CNanoRecipeListUI, CNanoListUI)

// =========================================================================
// Constructor
CNanoRecipeListUI::CNanoRecipeListUI()
{
}

// =========================================================================
// Destructor
CNanoRecipeListUI::~CNanoRecipeListUI()
{
}

// =========================================================================
//
BOOL CNanoRecipeListUI::PreTranslateMessage(MSG* pMsg)
{
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			PostMessage(WM_EXE_WND_ENTRY, 0L, 0L);
			break;
		}
		break;
	}

	return CNanoListUI::PreTranslateMessage(pMsg);
}

// =========================================================================
//
void CNanoRecipeListUI::DoDataExchange(CDataExchange* pDX)
{
	CNanoListUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNanoRecipeListUI)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CNanoRecipeListUI, CNanoListUI)
	//{{AFX_MSG_MAP(CNanoRecipeListUI)
	ON_COMMAND(IDM_NEW, OnNew)
	ON_COMMAND(IDM_DELETE, OnDelete)
	ON_NOTIFY(NM_CLICK, IDC_MAIN_RECIPE_LIST, OnClickRecipeMainList)
	ON_NOTIFY(NM_DBLCLK, IDC_MAIN_RECIPE_LIST, OnDblclkRecipeMainList)
	ON_UPDATE_COMMAND_UI(IDM_NEW, OnUpdateNew)
	ON_UPDATE_COMMAND_UI(IDM_DELETE, OnUpdateDelete)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EXE_WND_ENTRY, OnReturnRecipeMainList)
END_MESSAGE_MAP()

// =========================================================================
// 初期画面編集
void CNanoRecipeListUI::OnInitialUpdate()
{
	CNanoListUI::OnInitialUpdate();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	char szName[RECIPE_NAME_LEN + 1];

	///// Edit Tool Bar /////
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_LIST_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// 作成に失敗
	}

	RECT rect;
	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, 20, SWP_NOMOVE);

	LayoutToolBar();

	///// Sort /////
	CNanoListUI::SortColumn(1);

	///// Cursor /////
	pMainFrame->GetSelectListName(szName);
	SetCursorPosition(1, szName);
}

// =========================================================================
// ツールバーの表示・非表示
void CNanoRecipeListUI::LayoutToolBar()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	int iOperationPanel = pMainFrame->GetCurrOperationPanel();
	if ( iOperationPanel == OP_MEASUREMENT )
		m_wndToolBar.ShowWindow(SW_HIDE);
	else
		m_wndToolBar.ShowWindow(SW_SHOW);
}

// =========================================================================
// メニューやボタンの新規作成から呼ばれた時の処理
void CNanoRecipeListUI::OnNew()
{
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----
	///// User Access Privilege /////
	if ( !IsAccessPrivilege() ) {
		//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
		//Saiki 20090128 Change <-----
		return;
	}

	RecipeEntry(TRUE);
}

// =========================================================================
// メニューやボタンの削除から呼ばれた時の処理
void CNanoRecipeListUI::OnDelete()
{
// Kojika 20090526 Add
	CString strBuffer, strTitle;
// Kojika 20090526 Add End

	char szName[RECIPE_NAME_LEN + 1];
	char szMessage[256];
	int iListCount;
	int iIndex;

	// リストの選択行取得
	iIndex = CNanoListUI::rfListCtrl().GetSelectionMark();
	if ( iIndex == -1 )
		return;

	///// User Access Privilege /////
	if ( !IsAccessPrivilege() ) {
//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
//Saiki 20090128 Change <-----
		return;
	}

	CNanoListUI::rfListCtrl().GetItemText(iIndex, 1, szName, sizeof(szName));
// Kojika 20090526 Change
//	sprintf(szMessage, "Do you want to Delete \"%s\"?", szName);
//	sprintf(szMessage, "\"%s\"を削除しますか？", szName);
	LoadStringML(IDS_WANT_DELETE, strBuffer, "Do you want to Delete ""%s""?");
	sprintf(szMessage, strBuffer, szName);
// Kojika 20090526 Change End
	// Kojika 20090603 Change
	//if ( MessageBox(szMessage, "MAIN LIST", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDCANCEL )
	LoadStringML(IDS_TITLE_MAIN_LIST, strTitle, "MAIN LIST");
	if ( MessageBox(szMessage, strTitle, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDCANCEL )
		return;
	// Kojika 20090603 Change End
	if ( DeleteRecipeData(szName) )
	{
		iListCount = CNanoListUI::rfListCtrl().GetItemCount();
		List_DataSet();
/* added 2015.06.01 hmenjo リスト削除後ソート ---------- { ---------- */
		CNanoListUI::SortColumn(1);
		CNanoListUI::SortColumn(1);
/* added 2015.06.01 hmenjo リスト削除後ソート ---------- } ---------- */

		if( iIndex == iListCount - 1 )
			iIndex--;
		CNanoListUI::rfListCtrl().SetSelectionMark(iIndex);
		if ( iIndex != -1 )
			CNanoListUI::rfListCtrl().SetItemState(iIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

// =========================================================================
// クリック時、何もしない
void CNanoRecipeListUI::OnClickRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

// =========================================================================
// ダブルクリックでレシピ選択された場合、レシピ編集を呼出す
void CNanoRecipeListUI::OnDblclkRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( pMainFrame->GetCurrOperationPanel() == OP_RECIPE_SETTING )
		RecipeEntry(FALSE);
/* added 2015.04.12 hmenjo ダブルクリック手動測定 ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
		if (pMainFrame->GetCurrOperationPanel() == OP_MEASUREMENT) {
			/* 手動測定に入ります．	*/
			pMainFrame->m_wndSplitter1.GetPane(1, 0)->PostMessage(WM_DISPMENU_MANUAL_MEASUREMENT, (WPARAM) WM_DISPMENU_MANUAL_MEASUREMENT, 0);
		}
	}
/* added 2015.04.12 hmenjo ダブルクリック手動測定 ---------- } ---------- */

	*pResult = 0;
}


// =========================================================================
// エンターキーでレシピ選択された場合、レシピ編集を呼出す
LRESULT CNanoRecipeListUI::OnReturnRecipeMainList(WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( pMainFrame->GetCurrOperationPanel() == OP_RECIPE_SETTING )
		RecipeEntry(FALSE);
/* added 2015.04.12 hmenjo ダブルクリック手動測定 ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
		if (pMainFrame->GetCurrOperationPanel() == OP_MEASUREMENT) {
			/* 手動測定に入ります．	*/
			pMainFrame->m_wndSplitter1.GetPane(1, 0)->PostMessage(WM_DISPMENU_MANUAL_MEASUREMENT, (WPARAM) WM_DISPMENU_MANUAL_MEASUREMENT, 0);
		}
	}
/* added 2015.04.12 hmenjo ダブルクリック手動測定 ---------- } ---------- */

	return 0L;
}

// =========================================================================
// レシピ名から、リスト上で選択状態にセットしておく
int CNanoRecipeListUI::SetCursorPosition(int iCol, LPCTSTR psz)
{
	char Buff[256];
	int nListMax;
	int nIndex = 0;
	int retv = -1;

	nListMax = CNanoListUI::rfListCtrl().GetItemCount();

	if(nListMax <= 0) return retv;

	if(psz[0] != '\0'){
		for(nIndex = 0 ; nIndex < nListMax ; nIndex++){
			CNanoListUI::rfListCtrl().GetItemText(nIndex, iCol, Buff, sizeof(Buff));
			if(strcmp(psz, Buff) == 0){
				break;
			}
		}

		if(nIndex >= nListMax) nIndex = 0;
	}

	retv = CNanoListUI::rfListCtrl().SetSelectionMark(nIndex);
	CNanoListUI::rfListCtrl().SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	retv = nIndex;

	return retv;
}
// =========================================================================
// リスト上に選択されたレシピがあるか
BOOL CNanoRecipeListUI::CheckSelectRecipe()
{
	return CNanoListUI::rfListCtrl().GetSelectionMark() != -1;
}

// =========================================================================
// リストのヘッダ部作成
void CNanoRecipeListUI::List_HeaderSet()
{
	// サブクラスで定義
}

// =========================================================================
// リストのデータ部作成
void CNanoRecipeListUI::List_DataSet()
{
	// サブクラスで定義
}

// =========================================================================
// レシピ登録または編集
void CNanoRecipeListUI::RecipeEntry(BOOL bNew)
{
	// サブクラスで定義
}

// =========================================================================
// レシピ削除
BOOL CNanoRecipeListUI::DeleteRecipeData(LPCTSTR pszRecipeName)
{
	// サブクラスで定義
	return FALSE;
}

// =========================================================================
// User Access Privilege
BOOL CNanoRecipeListUI::IsAccessPrivilege()
{
	// サブクラスで定義
	return TRUE;
}

// =========================================================================
//
void CNanoRecipeListUI::OnUpdateNew(CCmdUI* pCmdUI)
{
/* modified 2016.08.05 hmenjo レシピ編集 menu バグ ---------- { ---------- */
//	pCmdUI->Enable(IsAccessPrivilege());
/* modified 2016.08.05 hmenjo レシピ編集 menu バグ ----------              */
	BOOL l_bRc = FALSE;
	/* ユーザの実行権チェック	*/
	l_bRc = IsAccessPrivilege();
	if (FALSE != l_bRc) {
		/* 実行可能	*/
		int l_iItemCount = m_lstCtrl.GetItemCount();
		if (RCP_PGM_NUM_MAX <= l_iItemCount) {
			/* リストの最大数に達しているので追加不可能	*/
			l_bRc = FALSE;
		}
	}
	pCmdUI->Enable(l_bRc);
/* modified 2016.08.05 hmenjo レシピ編集 menu バグ ---------- } ---------- */

}

// =========================================================================
//
void CNanoRecipeListUI::OnUpdateDelete(CCmdUI* pCmdUI)
{
/* modified 2016.08.05 hmenjo レシピ編集 menu バグ ---------- { ---------- */
//	pCmdUI->Enable(IsAccessPrivilege());
/* modified 2016.08.05 hmenjo レシピ編集 menu バグ ----------              */
	BOOL l_bRc = FALSE;
	/* ユーザの実行権チェック	*/
	l_bRc = IsAccessPrivilege();
	if (FALSE != l_bRc) {
		/* 実行可能	*/
		UINT l_uiSelectedCount = m_lstCtrl.GetSelectedCount();
		if (1 != l_uiSelectedCount) {
			/* 選択行が無い，あるいは，複数なので実行不可能	*/
			l_bRc = FALSE;
		}
	}
	pCmdUI->Enable(l_bRc);
/* modified 2016.08.05 hmenjo レシピ編集 menu バグ ---------- } ---------- */
}
