// RecipeOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SelectMeasurementTypeDlg.h"
#include "RecipeOperationView.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int EditRecipeMenuButton_IDs[] = {
	IDC_MAIN_RECIPE_BUTTON,
	IDC_MULTI_RECIPE_BUTTON,
	IDC_MEASUREMENT_PROGRAM_BUTTON,
	IDC_STAGE_PROGRAM_BUTTON,
	IDC_RECALIBRATION_PROGRAM_BUTTON,
};

// #########################################################################
// CRecipeOperationView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CRecipeOperationView, CNanoUI)

// =========================================================================
//
CRecipeOperationView::CRecipeOperationView()
	: CNanoUI(CRecipeOperationView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeOperationView)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
CRecipeOperationView::~CRecipeOperationView()
{
}

// =========================================================================
//
void CRecipeOperationView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeOperationView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_MAIN_RECIPE_BUTTON, m_MainRecipeButton);
	DDX_Control(pDX, IDC_MULTI_RECIPE_BUTTON, m_MultiRecipeButton);
	DDX_Control(pDX, IDC_MEASUREMENT_PROGRAM_BUTTON, m_MeasurementProgramButton);
	DDX_Control(pDX, IDC_STAGE_PROGRAM_BUTTON, m_StageProgramButton);
	DDX_Control(pDX, IDC_RECALIBRATION_PROGRAM_BUTTON, m_RecalibrationProgramButton);
	DDX_Control(pDX, IDC_POINT_DESKEW_PROGRAM_BUTTON, m_PointDeskewProgramButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CRecipeOperationView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MAIN_RECIPE_BUTTON, OnMainRecipeButton)
	ON_BN_CLICKED(IDC_MULTI_RECIPE_BUTTON, OnMultiRecipeButton)
	ON_BN_CLICKED(IDC_MEASUREMENT_PROGRAM_BUTTON, OnMeasurementProgramButton)
	ON_BN_CLICKED(IDC_STAGE_PROGRAM_BUTTON, OnStageProgramButton)
	ON_BN_CLICKED(IDC_RECALIBRATION_PROGRAM_BUTTON, OnRecalibrationProgramButton)
	ON_BN_CLICKED(IDC_POINT_DESKEW_PROGRAM_BUTTON, OnPointDeskewProgramButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CRecipeOperationView 診断

#ifdef _DEBUG
// =========================================================================
//
void CRecipeOperationView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CRecipeOperationView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CRecipeOperationView メッセージ ハンドラ

// =========================================================================
//
int CRecipeOperationView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNanoUI::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: この位置に固有の作成用コードを追加してください

	return 0;
}

// =========================================================================
//
void CRecipeOperationView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	//システムコンフィグ設定読み込み
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Multi Recipeが設定されていない場合は、ボタン類の動的レイアウト変更 /////
	if(!m_SystemConfig.bMultiRecipe){
		int iShift,iSubShift;
		CWnd* pWnd;
		WINDOWPLACEMENT placement;

		m_MultiRecipeButton.ShowWindow(SW_HIDE);
		m_MultiRecipeButton.GetWindowPlacement(&placement);
		iShift = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top ;

		pWnd=GetDlgItem(IDC_RECIPE_GROUP);
		pWnd->GetWindowPlacement(&placement);
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - iShift;
		pWnd->SetWindowPlacement(&placement);

		m_MeasurementProgramButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - iShift;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - iShift;
		m_MeasurementProgramButton.SetWindowPlacement(&placement);

// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
			m_StageProgramButton.GetWindowPlacement(&placement);
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - iShift;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - iShift;
			m_StageProgramButton.SetWindowPlacement(&placement);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

		m_RecalibrationProgramButton.GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - iShift;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - iShift;
		m_RecalibrationProgramButton.SetWindowPlacement(&placement);

		//// Point Deskewが選択されていない場合はボタン類の動的レイアウト変更 ////
		iSubShift = 0;
		m_PointDeskewProgramButton.GetWindowPlacement(&placement);
//★暫定対応　忘れるな！
#if 0
		if ( m_SystemConfig.nPRMethod >0 && m_SystemConfig.bSitePatternRec){
			placement.rcNormalPosition.top	= placement.rcNormalPosition.top - iShift;
			placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - iShift;
			m_PointDeskewProgramButton.SetWindowPlacement(&placement);
		}
		else{
			iSubShift = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top ;
			m_PointDeskewProgramButton.ShowWindow(SW_HIDE);
		}
#else
			iSubShift = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top ;
			m_PointDeskewProgramButton.ShowWindow(SW_HIDE);
#endif
		pWnd=GetDlgItem(IDC_PROGRAM_GROUP);
		pWnd->GetWindowPlacement(&placement);
		placement.rcNormalPosition.top	= placement.rcNormalPosition.top - iShift;
		placement.rcNormalPosition.bottom  = placement.rcNormalPosition.bottom - iShift - iSubShift;
		pWnd->SetWindowPlacement(&placement);

	}

	SelectMenuButton(RECIPE_SETTING_MAIN_RECIPE);

	SetTimer(1, 50, NULL);
}

// =========================================================================
//
void CRecipeOperationView::OnMainRecipeButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if ( !pMainFrame->MenuExit() )
		return;

	pMainFrame->ChangeExeWnd(RECIPE_MAIN_LIST_WND);

	SelectMenuButton(RECIPE_SETTING_MAIN_RECIPE);
}

// =========================================================================
//
void CRecipeOperationView::OnMultiRecipeButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if ( !pMainFrame->MenuExit() )
		return;

	pMainFrame->ChangeExeWnd(RECIPE_MULTI_LIST_WND);

	SelectMenuButton(RECIPE_SETTING_MULTI_RECIPE);
}

// =========================================================================
//
void CRecipeOperationView::OnMeasurementProgramButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if ( !pMainFrame->MenuExit() )
		return;

	pMainFrame->ChangeExeWnd(RECIPE_MEAS_LIST_WND);

	SelectMenuButton(RECIPE_SETTING_MEASUREMENT_PROGRAM);
}

// =========================================================================
//
void CRecipeOperationView::OnStageProgramButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if ( !pMainFrame->MenuExit() )
		return;

	pMainFrame->ChangeExeWnd(RECIPE_STAGE_LIST_WND);

	SelectMenuButton(RECIPE_SETTING_STAGE_PROGRAM);
}

// =========================================================================
//
void CRecipeOperationView::OnRecalibrationProgramButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if ( !pMainFrame->MenuExit() )
		return;

	pMainFrame->ChangeExeWnd(RECIPE_RECALIBRATION_LIST_WND);

	SelectMenuButton(RECIPE_SETTING_RECALIBRATION_PROGRAM);
}

// =========================================================================
//
void CRecipeOperationView::OnPointDeskewProgramButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if ( !pMainFrame->MenuExit() )
		return;

	pMainFrame->ChangeExeWnd(RECIPE_POINT_DESKEW_LIST_WND);

	SelectMenuButton(RECIPE_SETTING_POINT_DESKEW_PROGRAM);
}

// =========================================================================
//
void CRecipeOperationView::SetMenuButton(int iEditMode)
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	switch(iEditMode){
	case RECIPE_SETTING_MULTI_RECIPE:
		pMainFrame->ChangeExeWnd(RECIPE_MULTI_LIST_WND);
		break;
	case RECIPE_SETTING_MEASUREMENT_PROGRAM:
		pMainFrame->ChangeExeWnd(RECIPE_MEAS_LIST_WND);
		break;
	case RECIPE_SETTING_STAGE_PROGRAM:
		pMainFrame->ChangeExeWnd(RECIPE_STAGE_LIST_WND);
		break;
	case RECIPE_SETTING_RECALIBRATION_PROGRAM:
		pMainFrame->ChangeExeWnd(RECIPE_RECALIBRATION_LIST_WND);
		break;
	case RECIPE_SETTING_POINT_DESKEW_PROGRAM:
		pMainFrame->ChangeExeWnd(RECIPE_POINT_DESKEW_LIST_WND);
		break;
	case RECIPE_SETTING_MAIN_RECIPE:
	default:
		pMainFrame->ChangeExeWnd(RECIPE_MAIN_LIST_WND);
		break;
	}

	SelectMenuButton(iEditMode);
}

// =========================================================================
//
void CRecipeOperationView::SelectMenuButton(int iSelect)
{
	CShadeButtonST* pButton;

	m_MainRecipeButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);
	m_MultiRecipeButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);
	m_MeasurementProgramButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
		m_StageProgramButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		m_StageProgramButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	m_RecalibrationProgramButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);
	m_PointDeskewProgramButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);

	switch(iSelect){
	case RECIPE_SETTING_MAIN_RECIPE:
		m_iCurrEditMode = RECIPE_SETTING_MAIN_RECIPE;
		pButton = &m_MainRecipeButton;
		break;
	case RECIPE_SETTING_MULTI_RECIPE:
		m_iCurrEditMode = RECIPE_SETTING_MULTI_RECIPE;
		pButton = &m_MultiRecipeButton;
		break;
	case RECIPE_SETTING_MEASUREMENT_PROGRAM:
		m_iCurrEditMode = RECIPE_SETTING_MEASUREMENT_PROGRAM;
		pButton = &m_MeasurementProgramButton;
		break;
	case RECIPE_SETTING_STAGE_PROGRAM:
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//		if(g_lModelType == MODEL_T3100){
		if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
			pButton = NULL;
		}
		else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
			m_iCurrEditMode = RECIPE_SETTING_STAGE_PROGRAM;
			pButton = &m_StageProgramButton;
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		break;
	case RECIPE_SETTING_RECALIBRATION_PROGRAM:
		m_iCurrEditMode = RECIPE_SETTING_RECALIBRATION_PROGRAM;
		pButton = &m_RecalibrationProgramButton;
		break;
	case RECIPE_SETTING_POINT_DESKEW_PROGRAM:
		m_iCurrEditMode = RECIPE_SETTING_POINT_DESKEW_PROGRAM;
		pButton = &m_PointDeskewProgramButton;
		break;
	}

// 2013.11.11 Bagus Add (TohoSpec対応) -->
	if(pButton != NULL){
// 2013.11.11 Bagus Add (TohoSpec対応) <--
		pButton->SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_RADIO_ON_COLOR);

		InvalidateRect(NULL, TRUE);
// 2013.11.11 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.11 Bagus Add (TohoSpec対応) <--
}

HBRUSH CRecipeOperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	BOOL bNexMntSw = nexioIsMaintenanceSwitch();
	BOOL bNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if((bNexMntSw == OFF)||(bNexEngiMntSw == ON))
	{
		switch(nCtlColor)
		{
			case CTLCOLOR_STATIC:
				pDC->SetBkColor(RGB(255,0,0));
			default:
				hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
			break;
		}
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

void CRecipeOperationView::OnTimer(UINT nIDEvent)
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
