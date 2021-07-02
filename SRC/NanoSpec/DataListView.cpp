// DataListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MainFrm.h"
#include "DataListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDataListView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CDataListView, CNanoListUI)

// =========================================================================
//
CDataListView::CDataListView()
{
}

// =========================================================================
//
CDataListView::~CDataListView()
{
}

// =========================================================================
//
BOOL CDataListView::PreTranslateMessage(MSG* pMsg)
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
void CDataListView::DoDataExchange(CDataExchange* pDX)
{
	CNanoListUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataListView)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CDataListView, CNanoListUI)
	//{{AFX_MSG_MAP(CDataListView)
	ON_NOTIFY(NM_CLICK, IDC_MAIN_RECIPE_LIST, OnClickDataList)
	ON_NOTIFY(NM_DBLCLK, IDC_MAIN_RECIPE_LIST, OnDblclkDataList)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EXE_WND_ENTRY, OnReturnDataList)
END_MESSAGE_MAP()

// =========================================================================
// CDataListView メッセージ ハンドラ

// =========================================================================
//
void CDataListView::OnInitialUpdate()
{
	CNanoListUI::OnInitialUpdate();

	///// Data Tool Bar /////
	if(!m_wndDataBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDataBar.LoadToolBar(IDR_DATA_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// 作成に失敗
	}

	RECT rect;
	GetClientRect(&rect);
	m_wndDataBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	if ( CNanoListUI::rfListCtrl().GetItemCount() > 0) {
		CNanoListUI::rfListCtrl().SetSelectionMark(0);
		CNanoListUI::rfListCtrl().SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}


// =========================================================================
//
void CDataListView::List_HeaderSet()
{
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_LEFT };

	int Width[] = {
		0,		// (0) Index
// 2009.12.09 bagus --{--
//		240,	// (1) Name
//		70,		// (2) Head Type
		200,	// (1) Name
		110,		// (2) Head Type
// 2009.12.09 bagus --}--
		120,	// (3) Measurement Item
		160,	// (4) SampleID
		50,		// (5) Measurement Point
		140,//335,	// (6) Date
	};

	char* Item[] = {
		"",
		"Name",
		"Head",
		"Meas Item",
		"SampleID",
		"Point",
		"Date",
	};

	ItemMax = sizeof(Fmt) / sizeof(Fmt[0]);

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		listcol.pszText = Item[i];
		CNanoListUI::rfListCtrl().InsertColumn(i,&listcol);
	}
}

// =========================================================================
//
void CDataListView::List_DataSet()
{
	typedef struct{
		char Name[50];
		char Head[50];
		char MeasItem[50];
		char SampleId[50];
		int Point;
		char Date[50];
	} DATA_LIST;

	DATA_LIST Data[] = {
		{"DataWWWWWWWWWWWWWWWWWWWW", "SR",			"THICKNESS",		"CIMTEST", 10,	"2007.07.23 09:30:01"	},
		{"DataAAAAAAAAAAAAAAAAAAAA", "SE",			"THICKNESS",		"GOTO",	20,	"2007.07.24 09:40:01"	},
		{"DataBBBBBBBBBBBBBBBBBBBB", "CONTACT ANGLE", "CONTANC ANGLE", "NANO",	10000, "2007.07.25 09:50:01"	},
	};

	int ListMax = sizeof(Data) / sizeof(Data[0]);

	CNanoListUI::rfListCtrl().DeleteAllItems();
	for ( int i = 0 ; i < ListMax ; i++ )
		List_Add(Data[i].Name, Data[i].Head, Data[i].MeasItem, Data[i].SampleId, Data[i].Point, Data[i].Date);
}

// =========================================================================
//
void CDataListView::List_Add(char* Name, char* Head, char* MeasItem, char* SampleId, int Point, char* Date)
{
	LV_ITEM listitem;
	char szPoint[256];

	listitem.iItem = CNanoListUI::rfListCtrl().GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	int item = CNanoListUI::rfListCtrl().InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = Name;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Head /////
	listitem.iSubItem = 2;
	listitem.pszText = Head;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Measurement Item /////
	listitem.iSubItem = 3;
	listitem.pszText = MeasItem;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// SampleID /////
	listitem.iSubItem = 4;
	listitem.pszText = SampleId;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Measurement Point /////
	listitem.iSubItem = 5;
	sprintf(szPoint, "%d", Point);
	listitem.pszText = szPoint;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	///// Date /////
	listitem.iSubItem = 6;
	listitem.pszText = Date;
	CNanoListUI::rfListCtrl().SetItem(&listitem);

	CNanoListUI::rfListCtrl().SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CDataListView::OnClickDataList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

// =========================================================================
//
void CDataListView::OnDblclkDataList(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	pMainFrame->ChangeOperationPanel(OP_DATA_REVIEW);
	pMainFrame->ChangeExeWnd(DATA_SR_WND);

	*pResult = 0;
}

// =========================================================================
//
LRESULT CDataListView::OnReturnDataList(WPARAM wparam, LPARAM lparam)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	pMainFrame->ChangeOperationPanel(OP_DATA_REVIEW);
	pMainFrame->ChangeExeWnd(DATA_SR_WND);

	return 0L;
}

