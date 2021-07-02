// MaintenanceView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MaintenanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CMaintenanceView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CMaintenanceView, CNanoUI)

// =========================================================================
//
CMaintenanceView::CMaintenanceView()
	: CNanoUI(CMaintenanceView::IDD)
{
	//{{AFX_DATA_INIT(CMaintenanceView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CMaintenanceView::~CMaintenanceView()
{
}

// =========================================================================
//
void CMaintenanceView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenanceView)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMaintenanceView, CNanoUI)
	//{{AFX_MSG_MAP(CMaintenanceView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CMaintenanceView 診断

#ifdef _DEBUG
// =========================================================================
//
void CMaintenanceView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CMaintenanceView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CMaintenanceView メッセージ ハンドラ

// =========================================================================
//
HBRUSH CMaintenanceView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください

	switch(nCtlColor){
	case CTLCOLOR_DLG:
		///// 背景色 /////
		ControlColor(pDC, &m_Brush_BackColor, BLACK_COLOR);
		return m_Brush_BackColor;
		break;
	}

	return hbr;
}

// =========================================================================
//
void CMaintenanceView::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
void CMaintenanceView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	RECT l_rectDesktop;
	this->GetDesktopWindow()->GetClientRect(&l_rectDesktop);
	if ((g_lModelType == MODEL_T3100) && (l_rectDesktop.bottom < 1024)) {
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
	}
}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
