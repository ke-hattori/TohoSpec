// ManualMeasurementProcessView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ManualMeasurementProcessView.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CManualMeasurementProcessView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CManualMeasurementProcessView, CNanoUI)

// =========================================================================
//
CManualMeasurementProcessView::CManualMeasurementProcessView()
	: CNanoUI(CManualMeasurementProcessView::IDD)
{
	//{{AFX_DATA_INIT(CManualMeasurementProcessView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CManualMeasurementProcessView::~CManualMeasurementProcessView()
{
}

// =========================================================================
//
void CManualMeasurementProcessView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualMeasurementProcessView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_MEASUREMENT_CANCEL_BUTTON, m_MeasurementCancelButton);
	DDX_Control(pDX, IDC_MEASUREMENT_PAUSE_BUTTON, m_MeasurementPauseButton);
	DDX_Control(pDX, IDC_REMEASUREMENT_BUTTON, m_RemeasurementButton);
	DDX_Control(pDX, IDC_VALUES_TO_DISPLAY_BUTTON, m_ValuesToDisplayButton);
	DDX_Control(pDX, IDC_EXIT_BUTTON, m_ExitButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CManualMeasurementProcessView, CNanoUI)
	//{{AFX_MSG_MAP(CManualMeasurementProcessView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MEASUREMENT_CANCEL_BUTTON, OnMeasurementCancelButton)
	ON_BN_CLICKED(IDC_MEASUREMENT_PAUSE_BUTTON, OnMeasurementPauseButton)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_BN_CLICKED(IDC_REMEASUREMENT_BUTTON, OnRemeasurementButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CManualMeasurementProcessView 診断

#ifdef _DEBUG
// =========================================================================
//
void CManualMeasurementProcessView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CManualMeasurementProcessView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CManualMeasurementProcessView メッセージ ハンドラ

// =========================================================================
//
int CManualMeasurementProcessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNanoUI::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	return 0;
}

// =========================================================================
//
void CManualMeasurementProcessView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	///// Measurement Cancel Button /////
	m_MeasurementCancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementCancelButton.DrawFlatFocus(TRUE);

	///// Measurement Pause Button /////
	m_MeasurementPauseButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementPauseButton.DrawFlatFocus(TRUE);

	///// Remeasurement Button /////
	m_RemeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RemeasurementButton.DrawFlatFocus(TRUE);

	///// Values To Display Button /////
	m_ValuesToDisplayButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ValuesToDisplayButton.DrawFlatFocus(TRUE);

	///// Exit Button /////
	m_ExitButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ExitButton.DrawFlatFocus(TRUE);


	SetTimer(1, 50, NULL);
}

// =========================================================================
//
void CManualMeasurementProcessView::OnMeasurementCancelButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

}

// =========================================================================
//
void CManualMeasurementProcessView::OnMeasurementPauseButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

}

// =========================================================================
//
void CManualMeasurementProcessView::OnExitButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	pMainFrame->ChangeOperationPanel(OP_MEASUREMENT);
	pMainFrame->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
}

void CManualMeasurementProcessView::OnRemeasurementButton()
{
}

HBRUSH CManualMeasurementProcessView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CManualMeasurementProcessView::OnTimer(UINT nIDEvent)
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
