// DataListOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MysplitterWnd.h"
#include "Mainfrm.h"
#include "DataSearchDlg.h"
#include "DataListOperationView.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDataListOperationView
// #########################################################################

IMPLEMENT_DYNCREATE(CDataListOperationView, CNanoUI)

CDataListOperationView::CDataListOperationView()
	: CNanoUI(CDataListOperationView::IDD)
{
	//{{AFX_DATA_INIT(CDataListOperationView)
	//}}AFX_DATA_INIT
}

CDataListOperationView::~CDataListOperationView()
{
}

void CDataListOperationView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataListOperationView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_REVIEW_BUTTON, m_ReviewButton);
	DDX_Control(pDX, IDC_SEARCH_BUTTON, m_SearchButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_IMPORT_BUTTON, m_ImportButton);
	DDX_Control(pDX, IDC_EXPORT_BUTTON, m_ExportButton);
}


BEGIN_MESSAGE_MAP(CDataListOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CDataListOperationView)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, OnDataSearchButton)
	ON_BN_CLICKED(IDC_REVIEW_BUTTON, OnDataReviewButton)
	ON_BN_CLICKED(IDC_EXPORT_BUTTON, OnDataExportButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CDataListOperationView 診断

#ifdef _DEBUG
void CDataListOperationView::AssertValid() const
{
	CNanoUI::AssertValid();
}

void CDataListOperationView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CDataListOperationView メッセージ ハンドラ

// =========================================================================
//
void CDataListOperationView::OnDataSearchButton()
{
	CDataSearchDlg dlg;
	dlg.DoModal();
}

// =========================================================================
//
void CDataListOperationView::OnDataReviewButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->ChangeOperationPanel(OP_DATA_REVIEW);
	pMainFrame->ChangeExeWnd(DATA_SR_WND);
}

// =========================================================================
//
void CDataListOperationView::OnDataExportButton()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

	if ( !IsAccessPrivilege() ) {
// Kojika 20090526 Change
//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
//		MessageBox(IDS_USER_ACCESS_STOP_TEXT, IDS_USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
//Saiki 20090128 Change <-----
		LoadStringML(IDS_NOT_ACCESS_PRIVILEGE , strBuffer1, "You don't have Access Privilege");
		LoadStringML(IDS_TITLE_ACCESS_PRIVILEGE , strBuffer2, "ACCESS PRIVILEGE");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End
		return;
	}
}

// =========================================================================
//
void CDataListOperationView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	///// Review Button /////
	m_ReviewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReviewButton.DrawFlatFocus(TRUE);

	///// Search Button /////
	m_SearchButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SearchButton.DrawFlatFocus(TRUE);

	///// Delete Button /////
	m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteButton.DrawFlatFocus(TRUE);

	///// Import Button /////
	m_ImportButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ImportButton.DrawFlatFocus(TRUE);

	///// Export Button /////
	m_ExportButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ExportButton.DrawFlatFocus(TRUE);


	SetTimer(1, 50, NULL);
}

// =========================================================================
//
BOOL CDataListOperationView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_DATA);
}

HBRUSH CDataListOperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if( pWnd == this ) // 自分自身を描画するときは
	{
		if((sbNexMntSw == OFF)||(sbNexEngiMntSw == ON))
		{
			hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
		}
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

void CDataListOperationView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	static BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	static BOOL sbNexMntSwOld = TRUE;
	static BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();
	static BOOL sbNexEngiMntSwOld = FALSE;

	sbNexMntSw = nexioIsMaintenanceSwitch();
	sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if((sbNexMntSw != sbNexMntSwOld)||(sbNexEngiMntSw != sbNexEngiMntSwOld))
	{
		Invalidate(TRUE); //OnCtlColor()の処理をさせる為にCall
	}
	sbNexMntSwOld = sbNexMntSw; //前回値書換え
	sbNexEngiMntSwOld = sbNexEngiMntSw; //前回値書換え


	CNanoUI::OnTimer(nIDEvent);
}
