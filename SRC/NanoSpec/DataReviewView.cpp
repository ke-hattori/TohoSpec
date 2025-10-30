// DataReviewView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MeasurementListDlg.h"
#include "MeasurementGraphDlg.h"
#include "DataReviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDataReviewView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CDataReviewView, CNanoUI)

// =========================================================================
//
CDataReviewView::CDataReviewView()
	: CNanoUI(CDataReviewView::IDD)
{
	//{{AFX_DATA_INIT(CDataReviewView)
	//}}AFX_DATA_INIT
	m_pMeasurementListDlg = NULL;
	m_pMeasurementGraphDlg = NULL;
}

// =========================================================================
//
CDataReviewView::~CDataReviewView()
{
	if(m_pMeasurementListDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pMeasurementListDlg->DestroyWindow();
		delete m_pMeasurementListDlg;
		m_pMeasurementListDlg = NULL;
	}

	if(m_pMeasurementGraphDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pMeasurementGraphDlg->DestroyWindow();
		delete m_pMeasurementGraphDlg;
		m_pMeasurementGraphDlg = NULL;
	}
}

// =========================================================================
//
void CDataReviewView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataReviewView)
		// メモ: ClassWizard はこの位置に DDX および DDV の呼び出しを追加します
	DDX_Control(pDX, IDC_DATA_REVIEW_TAB, m_DataReviewTab);
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CDataReviewView, CNanoUI)
	//{{AFX_MSG_MAP(CDataReviewView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP

	ON_NOTIFY(TCN_SELCHANGE, IDC_DATA_REVIEW_TAB, OnSelchangeDataReviewTab)

END_MESSAGE_MAP()

// =========================================================================
// CDataReviewView 診断

#ifdef _DEBUG
// =========================================================================
//
void CDataReviewView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CDataReviewView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CDataReviewView メッセージ ハンドラ

// =========================================================================
//
void CDataReviewView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	// Tab Initialize
	Tab_Init();

	///// Edit Tool Bar /////
	if(!m_wndDataReviewBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDataReviewBar.LoadToolBar(IDR_DATA_REVIEW_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// 作成に失敗
	}

	RECT rect;
	GetClientRect(&rect);
	m_wndDataReviewBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);
}

// =========================================================================
//
void CDataReviewView::Tab_Init()
{
	int Index;
	int dx = 5;
	int dy = 24;

	///// TAB Item /////
//	for(Index = 0 ; Index < g_CurrentGraphMenuTabItemMax ; Index++){
//		m_CurrentGraphMenuTab.InsertItem(Index, CURRENT_GRAPH_MENU_TAB_ITEM[Index]);
//	}
	m_DataReviewTab.InsertItem(0, "List");
	m_DataReviewTab.InsertItem(1, "Graph");
	//m_DataReviewTab.InsertItem(2, "Map");
	//m_DataReviewTab.InsertItem(3, "2D");
	//m_DataReviewTab.InsertItem(4, "3D");

	///// List	/////
	m_pMeasurementListDlg = new CMeasurementListDlg();
	m_pMeasurementListDlg->Create(CMeasurementListDlg::IDD, &m_DataReviewTab);
	m_pMeasurementListDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	///// Graph /////
//	m_pMeasurementGraphDlg = new CMeasurementGraphDlg();
	// ★日比野さん：これは通常測定用？
	m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_SEQ);
	m_pMeasurementGraphDlg->Create(CMeasurementGraphDlg::IDD, &m_DataReviewTab);
	m_pMeasurementGraphDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	///// Set Current Selection Item /////
	Index = m_DataReviewTab.SetCurSel(0);
	TabChange(Index);
}

// =========================================================================
//
void CDataReviewView::OnSelchangeDataReviewTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index;

	Index = m_DataReviewTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CDataReviewView::TabChange(int Index)
{
//	int i;

	///// TAB Item Highlight /////
//	for(i = 0 ; i < g_CurrentGraphMenuTabItemMax ; i++){
//		m_CurrentGraphMenuTab.HighlightItem(i, FALSE);
//	}
//	m_CurrentGraphMenuTab.HighlightItem(Index, TRUE);
	m_DataReviewTab.HighlightItem(0, FALSE);
	m_DataReviewTab.HighlightItem(1, FALSE);
	m_DataReviewTab.HighlightItem(2, FALSE);
	m_DataReviewTab.HighlightItem(3, FALSE);
	m_DataReviewTab.HighlightItem(4, FALSE);
	m_DataReviewTab.HighlightItem(Index, TRUE);

	///// Hide All Dialog /////
	m_pMeasurementListDlg->ShowWindow(SW_HIDE);
	m_pMeasurementGraphDlg->ShowWindow(SW_HIDE);

	///// Show Dialog /////
	switch(Index){
	case 0:
		m_pMeasurementListDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pMeasurementGraphDlg->ShowWindow(SW_SHOW);
		break;
	}
}

// =========================================================================
//
BOOL CDataReviewView::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	switch(pMsg->message){
	case WM_KEYDOWN:
		switch(pMsg->wParam){
		case VK_ESCAPE:
			break;
		}
		break;
	}

	return CNanoUI::PreTranslateMessage(pMsg);
}

