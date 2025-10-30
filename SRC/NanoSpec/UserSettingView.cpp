// UserSettingView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "UserSettingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CUserSettingView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CUserSettingView, CNanoUI)

// =========================================================================
//
CUserSettingView::CUserSettingView()
	: CNanoUI(CUserSettingView::IDD)
{
	//{{AFX_DATA_INIT(CUserSettingView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CUserSettingView::~CUserSettingView()
{
}

// =========================================================================
//
void CUserSettingView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSettingView)
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CUserSettingView, CNanoUI)
	//{{AFX_MSG_MAP(CUserSettingView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CUserSettingView 診断

#ifdef _DEBUG
// =========================================================================
//
void CUserSettingView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CUserSettingView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CUserSettingView メッセージ ハンドラ

// =========================================================================
//
void CUserSettingView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	RECT l_rectDesktop;
	this->GetDesktopWindow()->GetClientRect(&l_rectDesktop);
	if ((g_lModelType == MODEL_T3100) && (l_rectDesktop.bottom < 1024)) {
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
	}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
}

// =========================================================================
//
HBRUSH CUserSettingView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください

	switch(nCtlColor){
	case CTLCOLOR_DLG:
		///// 背景色 /////
		//ControlColor(pDC, &m_Brush_BackColor, BLACK_COLOR);
		return m_Brush_BackColor;
	case CTLCOLOR_STATIC:
		///// Host Status /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_USER_SETTING_BACK_GROUND)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_UserSettingBackGround, BLACK_COLOR);
			return m_Brush_UserSettingBackGround;
		}
		break;
	}

	return hbr;
}

// =========================================================================
//
void CUserSettingView::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}
