// DataReviewOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "DataReviewOperationView.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDataReviewOperationView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CDataReviewOperationView, CNanoUI)

// =========================================================================
//
CDataReviewOperationView::CDataReviewOperationView()
	: CNanoUI(CDataReviewOperationView::IDD)
{
	//{{AFX_DATA_INIT(CDataReviewOperationView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CDataReviewOperationView::~CDataReviewOperationView()
{
}

// =========================================================================
//
void CDataReviewOperationView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataReviewOperationView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_GRID_PARAMETER_SETTING_BUTTON, m_GridParameterSettingButton);
	DDX_Control(pDX, IDC_VALUES_TO_DISPLAY_BUTTON, m_ValuesToDisplayButton);
	DDX_Control(pDX, IDC_SAVE_BUTTON, m_SaveButton);
	DDX_Control(pDX, IDC_SAVE_BITMAP_BUTTON, m_SaveBitmapButton);
	DDX_Control(pDX, IDC_PRINT_BUTTON, m_PrintButton);
	DDX_Control(pDX, IDC_EXIT_BUTTON, m_ExitButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CDataReviewOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CDataReviewOperationView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CDataReviewOperationView 診断

#ifdef _DEBUG
// =========================================================================
//
void CDataReviewOperationView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CDataReviewOperationView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CDataReviewOperationView メッセージ ハンドラ

// =========================================================================
//
void CDataReviewOperationView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	///// Grid Parameter Setting Button /////
	m_GridParameterSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GridParameterSettingButton.DrawFlatFocus(TRUE);

	///// Values To Display Button /////
	m_ValuesToDisplayButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ValuesToDisplayButton.DrawFlatFocus(TRUE);

	///// Save Button /////
	m_SaveButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SaveButton.DrawFlatFocus(TRUE);

	///// Save Bitmap Button /////
	m_SaveBitmapButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SaveBitmapButton.DrawFlatFocus(TRUE);

	///// Print Button /////
	m_PrintButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PrintButton.DrawFlatFocus(TRUE);

	///// Exit Button /////
	m_ExitButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ExitButton.DrawFlatFocus(TRUE);

	SetTimer(1, 50, NULL);
}

// =========================================================================
//
void CDataReviewOperationView::OnExitButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	pMainFrame->ChangeOperationPanel(OP_DATA);
	pMainFrame->ChangeExeWnd(DATA_LIST_WND);
}

// =========================================================================
//
BOOL CDataReviewOperationView::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	switch(pMsg->message){
	case WM_KEYDOWN:
		switch(pMsg->wParam){
		case	VK_ESCAPE:
			break;
		}
		break;
	}

	return CNanoUI::PreTranslateMessage(pMsg);
}

HBRUSH CDataReviewOperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CDataReviewOperationView::OnTimer(UINT nIDEvent)
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
