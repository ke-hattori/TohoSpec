// UserSettingOperationCommonSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ManualLoadPositionSettingDlg.h"
#include "UserAccountListDlg.h"
#include "AccessLevelSettingDlg.h"
#include "DeleteDataPeriodSettingDlg.h"
#include "SelectMeasurementTypeDlg.h"
#include "UserSettingOperationCommonSettingDlg.h"
#include "MaintenancePositionSettingDlg.h"
#include "MaintenanceDateDlg.h"
//2009.12.10 bagus Recipe Backup --{--
#include "BackupDataSettingDlg.h"
//2009.12.10 bagus Recipe Backup --}--
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CUserSettingOperationCommonSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CUserSettingOperationCommonSettingDlg::CUserSettingOperationCommonSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSettingOperationCommonSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserSettingOperationCommonSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSettingOperationCommonSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MANUAL_LOAD_POSITION_SETTING_BUTTON, m_ManualLoadPositionSettingButton);
	DDX_Control(pDX, IDC_USER_ACCOUNT_SETTING_BUTTON, m_UserAccountSettingButton);
	DDX_Control(pDX, IDC_ACCESS_LEVEL_SETTING_BUTTON, m_AccessLevelSettingButton);
	DDX_Control(pDX, IDC_DELETE_DATA_PERIOD_SETTING_BUTTON, m_DeleteDataPeriodSettingButton);
	DDX_Control(pDX, IDC_DEFAULT_HEAD_TYPE_SETTING_BUTTON, m_DefaultHeadTypeSettingButton);
	DDX_Control(pDX, IDC_STAGE_UNIT_SETTING_BUTTON, m_StageUnitSettingButton);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION_SETTING_BUTTON, m_MaintenancePositionSettingButton);
	DDX_Control(pDX, IDC_MAINTENANCE_DATE_BUTTON, m_MaintenanceDateButton);
	//2009.12.10 bagus Recipe Backup --{--
	DDX_Control(pDX, IDC_BACKUP_DATA_SETTING_BUTTON, m_BackupDataButton);
	//2009.12.10 bagus Recipe Backup --}--
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CUserSettingOperationCommonSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CUserSettingOperationCommonSettingDlg)
	ON_BN_CLICKED(IDC_MANUAL_LOAD_POSITION_SETTING_BUTTON, OnManualLoadPositionSettingButton)
	ON_BN_CLICKED(IDC_USER_ACCOUNT_SETTING_BUTTON, OnUserAccountSettingButton)
	ON_BN_CLICKED(IDC_ACCESS_LEVEL_SETTING_BUTTON, OnAccessLevelSettingButton)
	ON_BN_CLICKED(IDC_DELETE_DATA_PERIOD_SETTING_BUTTON, OnDeleteDataPeriodSettingButton)
	ON_BN_CLICKED(IDC_DEFAULT_HEAD_TYPE_SETTING_BUTTON, OnDefaultHeadTypeSettingButton)
	ON_BN_CLICKED(IDC_STAGE_UNIT_SETTING_BUTTON, OnStageUnitSettingButton)
	ON_BN_CLICKED(IDC_MAINTENANCE_POSITION_SETTING_BUTTON, OnMaintenancePositionSettingButton)
	ON_BN_CLICKED(IDC_BACKUP_DATA_SETTING_BUTTON, OnBackupDataButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MAINTENANCE_DATE_BUTTON, OnMaintenanceDateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CUserSettingOperationCommonSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CUserSettingOperationCommonSettingDlg::OnInitDialog()
{
	//未対応機能HIDE
	GetDlgItem(IDC_MANUAL_LOAD_POSITION_SETTING_BUTTON)->ShowWindow(SW_HIDE);
//	  GetDlgItem(IDC_DELETE_DATA_PERIOD_SETTING_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STAGE_UNIT_SETTING_BUTTON)->ShowWindow(SW_HIDE);

	CDialog::OnInitDialog();

	///// Manual Load Position Setting Button /////
	m_ManualLoadPositionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualLoadPositionSettingButton.DrawFlatFocus(TRUE);

	///// User Account Setting Button /////
	m_UserAccountSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_UserAccountSettingButton.DrawFlatFocus(TRUE);

	///// Access Level Setting Button /////
	m_AccessLevelSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AccessLevelSettingButton.DrawFlatFocus(TRUE);

	///// Delete Data Period Setting Button /////
	m_DeleteDataPeriodSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteDataPeriodSettingButton.DrawFlatFocus(TRUE);

	///// Default Head Type Setting Button /////
	m_DefaultHeadTypeSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DefaultHeadTypeSettingButton.DrawFlatFocus(TRUE);

	///// Stage Unit Setting Button /////
	m_StageUnitSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_StageUnitSettingButton.DrawFlatFocus(TRUE);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_MaintenancePositionSettingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// Maintenance Position Setting Button /////
		m_MaintenancePositionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_MaintenancePositionSettingButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	/////  /////
	m_MaintenanceDateButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MaintenanceDateButton.DrawFlatFocus(TRUE);

	/////  Backup Data Setting Button /////
	//2009.12.10 bagus Recipe Backup --{--
	SYSTEM_CONFIG l_SystemConfig;
	m_BackupDataButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_BackupDataButton.DrawFlatFocus(TRUE);
	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);
	m_BackupDataButton.ShowWindow(l_SystemConfig.bBackup ? SW_NORMAL : SW_HIDE);
	//2009.12.10 bagus Recipe Backup --}--

	SetTimer(1, 50, NULL);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnManualLoadPositionSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CManualLoadPositionSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnUserAccountSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CUserAccountListDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnAccessLevelSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CAccessLevelSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnDeleteDataPeriodSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CDeleteDataPeriodSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnDefaultHeadTypeSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CSelectMeasurementTypeDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnStageUnitSettingButton()
{
//	CStageUnitSettingDlg dlg;
//	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CUserSettingOperationCommonSettingDlg::OnMaintenancePositionSettingButton()
{
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CMaintenancePositionSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
BOOL CUserSettingOperationCommonSettingDlg::PreTranslateMessage(MSG* pMsg)
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
BOOL CUserSettingOperationCommonSettingDlg::ShowSettingDlg(CDialog* pDlg)
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

HBRUSH CUserSettingOperationCommonSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CUserSettingOperationCommonSettingDlg::OnTimer(UINT nIDEvent)
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


void CUserSettingOperationCommonSettingDlg::OnMaintenanceDateButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//Saiki 20090609 Add ----->
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;
	//Saiki 20090609 Add <-----
	CMaintenanceDateDlg dlg;

	dlg.DoModal();
}
//2009.12.10 bagus Recipe Baackup --{--
void CUserSettingOperationCommonSettingDlg::OnBackupDataButton()
{

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if ( !pMainFrame->MenuExit() )
		return;

	CBackupDataSettingDlg dlg;

	dlg.DoModal();

}
//2009.12.10 bagus Recipe Baackup --}--

