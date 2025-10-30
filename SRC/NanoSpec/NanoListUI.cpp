// NanoListUI.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoListUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Callback関数
static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString strBuffer1, strBuffer2;

	BOOL bOrder = (BOOL)LOWORD(lParamSort);
	int iIndex = (int)HIWORD(lParamSort);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = (CView*)pMainFrame->m_wndSplitter2.GetPane(0, 0);
	CListCtrl* pListCtrl = (CListCtrl*)(pView->GetDlgItem(IDC_MAIN_RECIPE_LIST));
	strBuffer1 = pListCtrl->GetItemText(lParam1, iIndex);
	strBuffer2 = pListCtrl->GetItemText(lParam2, iIndex);

	return bOrder ? strcmp(strBuffer2, strBuffer1) /*sort desc*/ : strcmp(strBuffer1, strBuffer2) /*sort asc*/;
}

// #########################################################################
// CNanoListUI
// #########################################################################

IMPLEMENT_DYNAMIC(CNanoListUI, CNanoUI)

// =========================================================================
//
CNanoListUI::CNanoListUI()
	: CNanoUI(CNanoListUI::IDD)
{
	::ZeroMemory(m_bSort, sizeof(m_bSort));
}

// =========================================================================
//
CNanoListUI::~CNanoListUI()
{
}

// =========================================================================
//
void CNanoListUI::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeMainListView)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CNanoListUI, CNanoUI)
	//{{AFX_MSG_MAP(CNanoListUI)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_MAIN_RECIPE_LIST, OnColumnclickRecipeMainList)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

// =========================================================================
//
void CNanoListUI::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	m_lstCtrl.SubclassWindow(GetDlgItem(IDC_MAIN_RECIPE_LIST)->GetSafeHwnd());
	m_lstCtrl.SetViewStyle();

	///// List /////
	List_HeaderSet();
	List_DataSet();
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	RECT l_rectListDlg;	this->GetWindowRect(&l_rectListDlg);
	RECT l_rectList;	this->GetDlgItem(IDC_MAIN_RECIPE_LIST)->GetWindowRect(&l_rectList);
	if ((g_lModelType == MODEL_T3100) &&
		(l_rectListDlg.bottom < l_rectList.bottom)) {
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		/* タブの底辺を移動	*/
		this->GetDlgItem(IDC_MAIN_RECIPE_LIST)->SetWindowPos(	0,
							0, 0,
							l_rectList.right - l_rectList.left,
							l_rectListDlg.bottom - l_rectList.top,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
}

// =========================================================================
//
void CNanoListUI::OnColumnclickRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	SortColumn(pNMListView->iSubItem);

	*pResult = 0;
}

// =========================================================================
//
void CNanoListUI::SortColumn(int iCol)
{
	RenumberItem();
	m_lstCtrl.SortItems(CompareFunc, MAKELPARAM((WORD)m_bSort[iCol], WORD(iCol)));
	m_bSort[iCol] = !m_bSort[iCol];
}

// =========================================================================
//
void CNanoListUI::RenumberItem()
{
	LV_ITEM listitem;

	for ( int i = 0; i < rfListCtrl().GetItemCount(); i++ ) {
		listitem.iItem = i;
		listitem.iSubItem = 0;
		listitem.mask = LVIF_PARAM;
		listitem.lParam = i;
		m_lstCtrl.SetItem(&listitem);
	}
}
