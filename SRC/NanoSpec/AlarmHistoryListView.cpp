// AlarmHistoryListView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "Alarm.h"
#include "AlarmHistoryListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarmHistoryListView                                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CAlarmHistoryListView, CNanoListUI)

/////////////////////////////////////////////////////////////////////////////
//
CAlarmHistoryListView::CAlarmHistoryListView()
{
	m_pMainFrame	= NULL;
	m_pDoc			= NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
CAlarmHistoryListView::~CAlarmHistoryListView()
{
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::DoDataExchange(CDataExchange* pDX)
{
	CNanoListUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarmHistoryListView)
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CAlarmHistoryListView, CNanoListUI)
	//{{AFX_MSG_MAP(CAlarmHistoryListView)
	ON_WM_CREATE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_MAIN_RECIPE_LIST, OnColumnclickRecipeMainList)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_ALARM_HIS_ADD, OnAlarmHis_Add)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarmHistoryListView 診断

#ifdef _DEBUG

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::AssertValid() const
{
	CNanoListUI::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::Dump(CDumpContext& dc) const
{
	CNanoListUI::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAlarmHistoryListView メッセージ ハンドラ                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
int CAlarmHistoryListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNanoListUI::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::OnInitialUpdate()
{
	CNanoListUI::OnInitialUpdate();

}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::OnColumnclickRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult)
{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//	SortColumn(pNMListView->iSubItem);
//
//	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::List_HeaderSet()
{
	DWORD dwColor = GetSysColor(COLOR_BACKGROUND);
	LV_COLUMN listcol;
	DWORD dwStyle = 0;
	int ItemMax;
	//Saiki 20090527 Add ----->
	CString strBuffer;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----
	int Width[] = {
		0,		// (0) Index
		70, 	// (1) Date
		60, 	// (2) Time
		60, 	// (3) Level (Alarm / Warning)
		50, 	// (4) Set/Reset
		50, 	// (5) ID
		600,	// (6) Alarm
	};

//Saiki 20090527 Change ----->
//	  LPTSTR Item[] = {
////		"",
////	  "Date",
////	  "Time",
////	  "Level",
////	  "Set",
////	  "ID",
////	  "Alarm",
//		  "",
//		  "日付",
//		  "時間",
//		  "レベル",
//		  "セット",
//		"ID",
//		  "アラーム内容",
//	  };
	LPTSTR Item_ENU[] = {
	  "",
	  "Date",
	  "Time",
	  "Level",
	  "Set",
	  "ID",
	  "Alarm",
	};
	LPTSTR Item_JPN[] = {
		"",
		"日付",
		"時間",
		"レベル",
		"セット",
		"ID",
		"アラーム内容",
	};

//Saiki 20090527 Change <-----

	int Fmt[] = { LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };

	ItemMax = sizeof(Width) / sizeof(int);

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		//Saiki 20090527 Change ----->
		//listcol.pszText = Item[i];
		if(l_SystemConfig.nLanguage == 0){
			listcol.pszText = Item_ENU[i];
		}
		else{
			listcol.pszText = Item_JPN[i];
		}
		//Saiki 20090527 Change <-----
		rfListCtrl().InsertColumn(i,&listcol);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::List_DataSet()
{
	int i;
	ALARM_HIS AlarmHis;
	int ListCount;
	BOOL retv;

	ListCount = m_pDoc->AlarmHis_GetCount();
	for(i = 0 ; i < ListCount ; i++){
		retv = m_pDoc->AlarmHis_Get(i, &AlarmHis);

		if(retv == FALSE) break;

		List_Disp(&AlarmHis);
	}

	SetCursorToLast();
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::List_Disp(ALARM_HIS* pAlarmHis)
{
	LV_ITEM listitem;
	int item;

	listitem.iItem = rfListCtrl().GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	item = rfListCtrl().InsertItem(&listitem);

	///// Date /////
	listitem.iSubItem = 1;
	listitem.pszText = pAlarmHis->Date;
	rfListCtrl().SetItem(&listitem);

	///// Time /////
	listitem.iSubItem = 2;
	listitem.pszText = pAlarmHis->Time;
	rfListCtrl().SetItem(&listitem);

	///// Level /////
	listitem.iSubItem = 3;
	listitem.pszText = pAlarmHis->Level;
	rfListCtrl().SetItem(&listitem);

	///// Set /////
	listitem.iSubItem = 4;
	listitem.pszText = pAlarmHis->Set;
	rfListCtrl().SetItem(&listitem);

	///// ID /////
	listitem.iSubItem = 5;
	listitem.pszText = pAlarmHis->Id;
	rfListCtrl().SetItem(&listitem);

	///// Alarm Text /////
	listitem.iSubItem = 6;
	listitem.pszText = pAlarmHis->Text;
	rfListCtrl().SetItem(&listitem);
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::List_Add(ALARM_HIS* pAlarmHis)
{
	int ListCount;

	ListCount = m_pDoc->AlarmHis_GetCount();

	while(ListCount >= ALARM_HIS_MAX){
		rfListCtrl().DeleteItem(0);
		if(--ListCount < 0) ListCount = 0;
	}

	List_Disp(pAlarmHis);
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CAlarmHistoryListView::OnAlarmHis_Add(WPARAM wparam, LPARAM lparam)
{
	int Doc_Index;
	int Doc_BackIndex;
	int View_Index = wparam;
	ALARM_HIS Doc_AlarmHis;
	ALARM_HIS View_AlarmHis;
	int Doc_Count;
	int View_Count;
	int retv;

	///// Get Count /////
	Doc_Count = m_pDoc->AlarmHis_GetCount();
	View_Count = rfListCtrl().GetItemCount();

//	///// Verify Doc Count and View Count /////
//	if(Doc_Count != View_Count){
//		rfListCtrl().DeleteAllItems();
//		List_DataSet();
//		return 0L;
//	}

	///// Doc Count == 0 /////
	if(Doc_Count == 0){
		rfListCtrl().DeleteAllItems();
		return 0L;
	}

	///// Doc Count == 1 /////
	if(Doc_Count == 1){
		rfListCtrl().DeleteAllItems();
		List_DataSet();
		SetCursorToLast();
		return 0L;
	}

	///// Doc Count >= 2 /////
	Doc_BackIndex = Doc_Count - 2;

	///// Get Doc Back Alarm His /////
	retv = m_pDoc->AlarmHis_Get(Doc_BackIndex, &Doc_AlarmHis);
	if(retv == FALSE){
		return 0L;
	}

	///// Get View Alarm His /////
	if(View_Count == 0){
		return 0L;
	}

	View_Index = View_Count - 1;

	memset(&View_AlarmHis, 0, sizeof(View_AlarmHis));
	rfListCtrl().GetItemText(View_Index, 1, View_AlarmHis.Date, ALARM_LOG_DATE_LEN + 1);
	rfListCtrl().GetItemText(View_Index, 2, View_AlarmHis.Time, ALARM_LOG_TIME_LEN + 1);
	rfListCtrl().GetItemText(View_Index, 3, View_AlarmHis.Level, ALARM_LOG_LEVEN_LEN + 1);
	rfListCtrl().GetItemText(View_Index, 4, View_AlarmHis.Set, ALARM_LOG_SET_LEN + 1);
	rfListCtrl().GetItemText(View_Index, 5, View_AlarmHis.Id, ALARM_LOG_ID_LEN + 1);
	rfListCtrl().GetItemText(View_Index, 6, View_AlarmHis.Text, ALARM_LOG_TEXT_LEN + 1);

	///// Verify Doc Alarm His and View Alarm His /////
	if(memcmp(&Doc_AlarmHis, &View_AlarmHis, sizeof(ALARM_HIS)) != 0){
		rfListCtrl().DeleteAllItems();
		List_DataSet();
		SetCursorToLast();
		return 0L;
	}

	///// Get Doc Alarm His /////
	Doc_Index = Doc_Count - 1;
	retv = m_pDoc->AlarmHis_Get(Doc_Index, &Doc_AlarmHis);
	if(retv == FALSE){
		return 0L;
	}

	List_Add(&Doc_AlarmHis);

	SetCursorToLast();

	return 1L;
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarmHistoryListView::SetCursorToLast()
{
	int Index;

	Index = rfListCtrl().GetItemCount() - 1;
	if(Index < 0) Index = 0;

	rfListCtrl().SetItemState(Index, LVIS_SELECTED, LVIS_SELECTED);
	rfListCtrl().EnsureVisible(Index, FALSE);
}
