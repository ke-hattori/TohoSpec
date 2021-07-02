// UserSettingOperationSrSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "SrUserSettingPatternRecSettingDlg.h"
#include "SrReferencePositionSettingDlg.h"
#include "SrRecipeDefaultValueSettingDlg.h"
#include "SrFigureFormatSettingDlg.h"
#include "UserSettingOperationSrSettingDlg.h"
#include "SrAutoFocusRetryTimesDlg.h"
#include "SitePatternEditDlg.h"
// 2009.09.30 bagus Gantry --{--
#include "SrTransmittanceSettingDlg.h"
// 2009.09.30 bagus Gantry --}--
#include "NEXIOBASE.HXX"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CUserSettingOperationSrSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CUserSettingOperationSrSettingDlg::CUserSettingOperationSrSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSettingOperationSrSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserSettingOperationSrSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CUserSettingOperationSrSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSettingOperationSrSettingDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_PATTERN_REC_SETTING_BUTTON, m_PatternRecSettingButton);
	DDX_Control(pDX, IDC_REFERENCE_POSITION_SETTING_BUTTON, m_ReferencePositionSettingButton);
	DDX_Control(pDX, IDC_RECIPE_DEFAULT_VALUE_SETTING_BUTTON, m_RecipeDefaultValueSettingButton);
	DDX_Control(pDX, IDC_FIGURE_FORMAT_SETTING_BUTTON, m_FigureFormatSettingButton);
	DDX_Control(pDX, IDC_FOCUS_RETRY_TIMES_SETTING_BUTTON, m_FocusRetryTimesSettingButton);
	DDX_Control(pDX, IDC_DELETE_SITE_PR_IMAGE_BUTTON, m_DeleteSitePRImageButton);
// 2009.09.30 bagus Gantry --{--
	DDX_Control(pDX, IDC_TRANSMITTANCE_SETTING_BUTTON, m_TransmittanceSettingButton);
// 2009.09.30 bagus Gantry --}--
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CUserSettingOperationSrSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CUserSettingOperationSrSettingDlg)
	ON_BN_CLICKED(IDC_PATTERN_REC_SETTING_BUTTON, OnPatternRecSettingButton)
	ON_BN_CLICKED(IDC_REFERENCE_POSITION_SETTING_BUTTON, OnReferencePositionSettingButton)
	ON_BN_CLICKED(IDC_RECIPE_DEFAULT_VALUE_SETTING_BUTTON, OnRecipeDefaultValueSettingButton)
	ON_BN_CLICKED(IDC_FIGURE_FORMAT_SETTING_BUTTON, OnFigureFormatSettingButton)
	ON_BN_CLICKED(IDC_FOCUS_RETRY_TIMES_SETTING_BUTTON, OnFocusRetryTimesSettingButton)
	ON_BN_CLICKED(IDC_DELETE_SITE_PR_IMAGE_BUTTON, OnDeleteSitePRImageButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRANSMITTANCE_SETTING_BUTTON, OnTransmittanceSettingButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CUserSettingOperationSrSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CUserSettingOperationSrSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// Load System Config File /////
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_PatternRecSettingButton.ShowWindow(SW_HIDE);
		m_ReferencePositionSettingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// Pattern Rec Setting Button /////
		m_PatternRecSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_PatternRecSettingButton.DrawFlatFocus(TRUE);

		///// Reference Position Setting Button /////
		m_ReferencePositionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_ReferencePositionSettingButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Recipe Default Value Setting Button /////
	m_RecipeDefaultValueSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RecipeDefaultValueSettingButton.DrawFlatFocus(TRUE);

	///// Figure Format Setting Button /////
	m_FigureFormatSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_FigureFormatSettingButton.DrawFlatFocus(TRUE);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_FocusRetryTimesSettingButton.ShowWindow(SW_HIDE);
		m_DeleteSitePRImageButton.ShowWindow(SW_HIDE);
		m_TransmittanceSettingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// Focus Retry Times Setting Button /////
		if(m_SrConfig.bAutoFocus){
			m_FocusRetryTimesSettingButton.ShowWindow(SW_SHOWDEFAULT);
			m_FocusRetryTimesSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
				BUTTON_COLORING, BUTTON_NORMAL_COLOR);
			m_FocusRetryTimesSettingButton.DrawFlatFocus(TRUE);
		}
		else{
			m_FocusRetryTimesSettingButton.ShowWindow(SW_HIDE);
		}

		///// Delete Site PR Image Button /////
		m_DeleteSitePRImageButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_DeleteSitePRImageButton.DrawFlatFocus(TRUE);
//		// まだ表示させない
//		m_DeleteSitePRImageButton.ShowWindow(SW_HIDE);
// 2009.11.24 bagus SR 修正 --{--
		SYSTEM_CONFIG	SystemConfig;

		ConfigFile_GetNanoSpecIni(&SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

		//2009.12.07 bagus 修正 --{--
		//if (SystemConfig.nPRMethod > 0 && !SystemConfig.bSitePatternRec) {
		if ((SystemConfig.nPRMethod > 0 && !SystemConfig.bSitePatternRec)
		|| SystemConfig.nPRMethod == 0
		) {
		//2009.12.07 bagus 修正 --}--
			m_DeleteSitePRImageButton.ShowWindow(SW_HIDE);
		} else {
			m_DeleteSitePRImageButton.ShowWindow(SW_SHOW);
		}
// 2009.11.24 bagus SR 修正 --}--

// 2009.09.30 bagus Gantry --{--
		m_TransmittanceSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_TransmittanceSettingButton.DrawFlatFocus(TRUE);
// 2009.09.30 bagus Gantry --}--

// 2010.01.06 bagus Gantry --{--
// 2010.01.13 bagus Gantry --{--
//		if (m_SrConfig.bTransmittance && m_SrConfig.bGantryType) {
		if (m_SrConfig.bTransmittance) {
// 2010.01.13 bagus Gantry --}--
			m_TransmittanceSettingButton.ShowWindow(SW_SHOW);
		} else {
			m_TransmittanceSettingButton.ShowWindow(SW_HIDE);
		}
// 2010.01.06 bagus Gantry --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	SetTimer(1, 50, NULL);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CUserSettingOperationSrSettingDlg::OnPatternRecSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CSrUserSettingPatternRecSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationSrSettingDlg::OnReferencePositionSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CSrReferencePositionSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationSrSettingDlg::OnRecipeDefaultValueSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CSrRecipeDefaultValueSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationSrSettingDlg::OnFigureFormatSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	SrFigureFormatSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
BOOL CUserSettingOperationSrSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message){
	case WM_KEYDOWN:
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		break;
	case WM_SYSKEYDOWN:
		if(pMsg->wParam == VK_F4)
			return TRUE;
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// =========================================================================
//
BOOL CUserSettingOperationSrSettingDlg::ShowSettingDlg(CDialog* pDlg)
{
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( !pDoc->User_Access(ACCESS_USERSETTING) ) {
//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
//Saiki 20090128 Change <-----
		return FALSE;
	}

	// 右のウィンドウをUser Setting画面に戻す
	pMainFrame->ChangeExeWnd(USERCONF_WND);

	return ( pDlg->DoModal() == IDOK );
}

void CUserSettingOperationSrSettingDlg::OnFocusRetryTimesSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	SrAutoFocusRetryTimesDlg dlg;
	ShowSettingDlg(&dlg);

}

void CUserSettingOperationSrSettingDlg::OnDeleteSitePRImageButton()
{
	CSitePatternEditDlg dlg;
	g_pSitePatternEditDlg = &dlg;

	dlg.m_iMode = SITEPATTERN_MODE_DEL;
	dlg.DoModal();
}

HBRUSH CUserSettingOperationSrSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

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

void CUserSettingOperationSrSettingDlg::OnTimer(UINT nIDEvent)
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

	CDialog::OnTimer(nIDEvent);
}

// 2009.09.30 bagus Gantry --{--
void CUserSettingOperationSrSettingDlg::OnTransmittanceSettingButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CSrTransmittanceSettingDlg dlg;

	dlg.DoModal();
}
// 2009.09.30 bagus Gantry --}--
