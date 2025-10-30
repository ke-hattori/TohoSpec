// TestMeasurementOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "TestMeasurementOperationView.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CTestMeasurementOperationView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CTestMeasurementOperationView, CNanoUI)

// =========================================================================
//
CTestMeasurementOperationView::CTestMeasurementOperationView()
	: CNanoUI(CTestMeasurementOperationView::IDD)
{
	//{{AFX_DATA_INIT(CTestMeasurementOperationView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CTestMeasurementOperationView::~CTestMeasurementOperationView()
{
}

// =========================================================================
//
void CTestMeasurementOperationView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMeasurementOperationView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_TEST_MEASUREMENT_BUTTON, m_MeasurementButton);
	DDX_Control(pDX, IDC_TEST_REMEASUREMENT_BUTTON, m_RemeasurementButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_GET_REFERENCE_BUTTON, m_GetReferenceButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_HANDLER_LOAD_POSITION_BUTTON, m_HandlerLoadPositionButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_MANUAL_LOAD_POSITION_BUTTON, m_ManualLoadPositionButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_VACUUM_BUTTON, m_VacuumButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_GLASS_LIFTER_BUTTON, m_GlassLifterButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_DATA_SAVE_BUTTON, m_DataSaveButton);
	DDX_Control(pDX, IDC_TEST_MEASUREMENT_DATA_LOAD_BUTTON, m_DataLoadButton);
	DDX_Control(pDX, IDC_EXIT_BUTTON, m_ExitButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CTestMeasurementOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CTestMeasurementOperationView)
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CTestMeasurementOperationView 診断

#ifdef _DEBUG
// =========================================================================
//
void CTestMeasurementOperationView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CTestMeasurementOperationView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CTestMeasurementOperationView メッセージ ハンドラ

// =========================================================================
//
void CTestMeasurementOperationView::OnExitButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	pMainFrame->ChangeOperationPanel(OP_RECIPE_SETTING);
	pMainFrame->ChangeExeWnd(RECIPE_MEAS_LIST_WND);
}

// =========================================================================
//
void CTestMeasurementOperationView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	///// Measurement Button /////
	m_MeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, GREEN_COLOR);
	m_MeasurementButton.DrawFlatFocus(TRUE);

	///// Remeasurement Button /////
	m_RemeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RemeasurementButton.DrawFlatFocus(TRUE);

	///// Get Referenece Button /////
	m_GetReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GetReferenceButton.DrawFlatFocus(TRUE);

	///// Handler Load Position Button /////
	m_HandlerLoadPositionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_HandlerLoadPositionButton.DrawFlatFocus(TRUE);

	///// Manual Load Position Button /////
	m_ManualLoadPositionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualLoadPositionButton.DrawFlatFocus(TRUE);

	///// Vacuum Button /////
	m_VacuumButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_VacuumButton.DrawFlatFocus(TRUE);

	///// Glass Lifter Button /////
	m_GlassLifterButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GlassLifterButton.DrawFlatFocus(TRUE);

	///// Data Save Button /////
	m_DataSaveButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DataSaveButton.DrawFlatFocus(TRUE);

	///// Data Load Button /////
	m_DataLoadButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DataLoadButton.DrawFlatFocus(TRUE);

	///// Exit Button /////
	m_ExitButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ExitButton.DrawFlatFocus(TRUE);

	SetTimer(1, 50, NULL);
}

HBRUSH CTestMeasurementOperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CTestMeasurementOperationView::OnTimer(UINT nIDEvent)
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
