// MaintenancePositionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "MaintenancePositionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define																   //
//																		   //
// ==========================================================================
//#define MAINTENANCEPOSITIONSETTING_CAPTION		"MAINTENANCE POSITION SETTING"
//#define MAINTENANCEPOSITIONSETTING_MESSAGE		"MAINTENANCE POSITION SETTING"
// Kojika 20090529 Change IDSに変更
//#define MAINTENANCEPOSITIONSETTING_CAPTION	  "メンテナンスポジション設定"
//#define MAINTENANCEPOSITIONSETTING_MESSAGE	  "メンテナンスポジション設定"
// Kojika 20090529 Change End
// #########################################################################
// CMaintenancePositionSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CMaintenancePositionSettingDlg::CMaintenancePositionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaintenancePositionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaintenancePositionSettingDlg)
	//}}AFX_DATA_INIT
	m_iCurrNo = 0;
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenancePositionSettingDlg)
	//}}AFX_DATA_MAP
//	  DDX_Text(pDX, IDC_MAINTENANCE_POSITION_X, m_TempMaintenancePosition[m_iCurrNo].Loc.lX);
//	  DDX_Text(pDX, IDC_MAINTENANCE_POSITION_Y, m_TempMaintenancePosition[m_iCurrNo].Loc.lY);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION_X, m_stcMaintenancePosX);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION_Y, m_stcMaintenancePosY);
	DDX_Text(pDX, IDC_MAINTENANCE_POSITION_COMMENT, m_TempMaintenancePosition[m_iCurrNo].szComment, MAINTENANCE_POSITION_COMMENT_LEN + 1);
	DDX_CBIndex(pDX, IDC_MAINTENANCE_POSITION_NO, m_iCurrNo);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_MAINTENANCE_POSITION_SETUP_BUTTON, m_MaintenancePositionSetupButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMaintenancePositionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CMaintenancePositionSettingDlg)
	ON_CBN_SELCHANGE(IDC_MAINTENANCE_POSITION_NO, OnSelchangeMaintenancePositionNo)
	ON_BN_CLICKED(IDC_MAINTENANCE_POSITION_SETUP_BUTTON, OnMaintenancePositionSetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CMaintenancePositionSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CMaintenancePositionSettingDlg::OnInitDialog()
{
	char szTemp[256];

	CDialog::OnInitDialog();

	UpdateData();

	m_iCurrNo = 0;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	////// Load File /////
	ConfigFile_GetNanoSpecIni(m_TempMaintenancePosition, CONFIG_FILE_MAINTENANCE_POSITION);

	// Maintenance Position
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "%.3lf", (double)(m_TempMaintenancePosition[m_iCurrNo].Loc.lX)/MICROMETRE);
	SetDlgItemText(IDC_MAINTENANCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "%.3lf", (double)(m_TempMaintenancePosition[m_iCurrNo].Loc.lY)/MICROMETRE);
	SetDlgItemText(IDC_MAINTENANCE_POSITION_Y, szTemp);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// General Position Setup Button /////
	m_MaintenancePositionSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MaintenancePositionSetupButton.DrawFlatFocus(TRUE);

	///// Static Control /////
//	m_stcMaintenancePosX.SubclassWindow(GetDlgItem(IDC_MAINTENANCE_POSITION_X)->GetSafeHwnd());
	m_stcMaintenancePosX.SetBkColor(REFERENCE_COLOR);
//	m_stcMaintenancePosY.SubclassWindow(GetDlgItem(IDC_MAINTENANCE_POSITION_Y)->GetSafeHwnd());
	m_stcMaintenancePosY.SetBkColor(REFERENCE_COLOR);

	///// Control Initialize /////
	InitCombo_MaintenancePosNo();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_MAINTENANCE_POSITION_COMMENT))->SetLimitText(MAINTENANCE_POSITION_COMMENT_LEN);

	UpdateData(FALSE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	SetOperationLog("OK Button was push.");

	ConfigFile_SetNanoSpecIni(m_TempMaintenancePosition, CONFIG_FILE_MAINTENANCE_POSITION);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。
	SetOperationLog("OK Button was push.");

	CDialog::OnCancel();
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::OnSelchangeMaintenancePositionNo()
{
	UpdateData();			// No切り替え前の情報を取得する

	char szTemp[256];
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "%.3lf", (double)(m_TempMaintenancePosition[m_iCurrNo].Loc.lX)/MICROMETRE);
	SetDlgItemText(IDC_MAINTENANCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf(szTemp, "%.3lf", (double)(m_TempMaintenancePosition[m_iCurrNo].Loc.lY)/MICROMETRE);
	SetDlgItemText(IDC_MAINTENANCE_POSITION_Y, szTemp);

	UpdateData(FALSE);		// No切り替え後の情報にて画面更新する
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::OnMaintenancePositionSetupButton()
{
	char szTemp[256];

	// Kojika 20090529 Add
	CString l_strBuffer;
	// Kojika 20090529 Add End

	UpdateData();

	SetOperationLog("SETUP Button was push.");

	COverlayDialog dlgOverlay;

	dlgOverlay.m_lX = m_TempMaintenancePosition[m_iCurrNo].Loc.lX;
	dlgOverlay.m_lY = m_TempMaintenancePosition[m_iCurrNo].Loc.lY;
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	// Kojika 20090529 Change
	//dlgOverlay.SetCaption(MAINTENANCEPOSITIONSETTING_CAPTION);
	LoadStringML(IDS_MAINTE_POSI_SETTING_CAPTION, l_strBuffer, "MAINTENANCE POSITION SETTING");
	dlgOverlay.SetCaption(l_strBuffer);
	//dlgOverlay.SetMessage(MAINTENANCEPOSITIONSETTING_MESSAGE);
	LoadStringML(IDS_MAINTE_POSI_SETTING_MESSAGE, l_strBuffer, "MAINTENANCE POSITION SETTING");
	dlgOverlay.SetMessage(l_strBuffer);
	// Kojika 20090529 Change End

	if ( dlgOverlay.DoModal() == IDOK ) {
		m_TempMaintenancePosition[m_iCurrNo].Loc.lX = dlgOverlay.m_lX;
		m_TempMaintenancePosition[m_iCurrNo].Loc.lY = dlgOverlay.m_lY;
		// X
		memset(szTemp, 0, sizeof(szTemp));
		sprintf(szTemp, "%.3lf", (double)(m_TempMaintenancePosition[m_iCurrNo].Loc.lX)/MICROMETRE);
		SetDlgItemText(IDC_MAINTENANCE_POSITION_X, szTemp);
		// Y
		memset(szTemp, 0, sizeof(szTemp));
		sprintf(szTemp, "%.3lf", (double)(m_TempMaintenancePosition[m_iCurrNo].Loc.lY)/MICROMETRE);
		SetDlgItemText(IDC_MAINTENANCE_POSITION_Y, szTemp);
	}

	UpdateData(FALSE);
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::InitCombo_MaintenancePosNo()
{
	CString strBuffer;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MAINTENANCE_POSITION_NO);
	pCombo->ResetContent();

	for ( int i = 0; i < MAINTENANCE_POSITION_MAX; i++ ) {
		strBuffer.Format("%d", i + 1);
		pCombo->AddString(strBuffer);
	}
	pCombo->SetCurSel(m_iCurrNo);
}

// =========================================================================
//
void CMaintenancePositionSettingDlg::SetOperationLog(LPCTSTR pszLog)
{
	char strmsg[1024];

	// 操作ログを取得
	memset( strmsg, 0, sizeof(strmsg));
	strcpy( strmsg, "[");
	::GetWindowText(this->m_hWnd, &strmsg[1], 1000);
	_tcscat( strmsg, "] ");
	_tcscat( strmsg, pszLog);

	m_pDoc->OperationLogging(strmsg);
}
