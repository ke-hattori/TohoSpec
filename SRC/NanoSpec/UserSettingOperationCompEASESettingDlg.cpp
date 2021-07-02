// UserSettingOperationCompEASESettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "UserSettingOperationCompEASESettingDlg.h"
#include "SeFigureFormatSettingDlg.h"
#include "SrRecipeDefaultValueSettingDlg.h"
#include "SitePatternEditDlg.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationCompEASESettingDlg ダイアログ


CUserSettingOperationCompEASESettingDlg::CUserSettingOperationCompEASESettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSettingOperationCompEASESettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserSettingOperationCompEASESettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CUserSettingOperationCompEASESettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSettingOperationCompEASESettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RECIPE_DEFAULT_VALUE_SETTING_BUTTON, m_RecipeDefaultValueSettingButton);
	DDX_Control(pDX, IDC_FIGURE_FORMAT_SETTING_BUTTON, m_FigureFormatSettingButton);
}


BEGIN_MESSAGE_MAP(CUserSettingOperationCompEASESettingDlg, CDialog)
	//{{AFX_MSG_MAP(CUserSettingOperationCompEASESettingDlg)
	ON_BN_CLICKED(IDC_RECIPE_DEFAULT_VALUE_SETTING_BUTTON, OnRecipeDefaultValueSettingButton)
	ON_BN_CLICKED(IDC_FIGURE_FORMAT_SETTING_BUTTON, OnFigureFormatSettingButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationCompEASESettingDlg メッセージ ハンドラ
BOOL CUserSettingOperationCompEASESettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// Recipe Default Value Setting Button /////
	m_RecipeDefaultValueSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RecipeDefaultValueSettingButton.DrawFlatFocus(TRUE);

	///// Figure Format Setting Button /////
	m_FigureFormatSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_FigureFormatSettingButton.DrawFlatFocus(TRUE);

	SetTimer(1, 50, NULL);

	return TRUE;
}

// =========================================================================
//
void CUserSettingOperationCompEASESettingDlg::OnRecipeDefaultValueSettingButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;

	CSrRecipeDefaultValueSettingDlg dlg;
	dlg.m_DefaultHeadType = HEAD_TYPE_COMPEASE;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCompEASESettingDlg::OnFigureFormatSettingButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;

	SeFigureFormatSettingDlg dlg;
	dlg.m_iHeadType = HEAD_TYPE_COMPEASE;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
BOOL CUserSettingOperationCompEASESettingDlg::ShowSettingDlg(CDialog* pDlg)
{
	CString strBuffer, strTitle;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( !pDoc->User_Access(ACCESS_USERSETTING) ) {
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// 右のウィンドウをUser Setting画面に戻す
	pMainFrame->ChangeExeWnd(USERCONF_WND);

	return ( pDlg->DoModal() == IDOK );
}

// =========================================================================
//
HBRUSH CUserSettingOperationCompEASESettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

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

// =========================================================================
//
void CUserSettingOperationCompEASESettingDlg::OnTimer(UINT nIDEvent)
{
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
