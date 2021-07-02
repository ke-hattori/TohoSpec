// ManualLoadPositionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "ManualLoadPositionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define                                                                  //
//                                                                         //
// ==========================================================================
//#define MANUALLOADPOSITIONSETTING_CAPTION 	"MANUAL LOAD POSITION SETTING"
//#define MANUALLOADPOSITIONSETTING_MESSAGE 	"MANUAL LOAD POSITION SETTING"
//#define MANUALLOADPOSITIONSETTING_CAPTION 	  "マニュアルロードポジション設定"
//#define MANUALLOADPOSITIONSETTING_MESSAGE 	  "マニュアルロードポジション設定"

// #########################################################################
// CManualLoadPositionSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CManualLoadPositionSettingDlg::CManualLoadPositionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualLoadPositionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualLoadPositionSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CManualLoadPositionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualLoadPositionSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MANUAL_LOAD_POSITION_X, m_ManualLoadPosition.lX);
	DDX_Text(pDX, IDC_MANUAL_LOAD_POSITION_Y, m_ManualLoadPosition.lY);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_MANUAL_LOAD_POSITION_SETUP_BUTTON, m_ManualLoadPositionSetupButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CManualLoadPositionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CManualLoadPositionSettingDlg)
	ON_BN_CLICKED(IDC_MANUAL_LOAD_POSITION_SETUP_BUTTON, OnManualLoadPositionSetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CManualLoadPositionSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CManualLoadPositionSettingDlg::OnInitDialog()
{
	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_ManualLoadPosition, CONFIG_FILE_MANUAL_LOAD_POSITION);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Manual Load Position Setup Button /////
	m_ManualLoadPositionSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ManualLoadPositionSetupButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_stcManualLoadPosX.SubclassWindow(GetDlgItem(IDC_MANUAL_LOAD_POSITION_X)->GetSafeHwnd());
	m_stcManualLoadPosX.SetBkColor(REFERENCE_COLOR);
	m_stcManualLoadPosY.SubclassWindow(GetDlgItem(IDC_MANUAL_LOAD_POSITION_Y)->GetSafeHwnd());
	m_stcManualLoadPosY.SetBkColor(REFERENCE_COLOR);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CManualLoadPositionSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(&m_ManualLoadPosition, CONFIG_FILE_MANUAL_LOAD_POSITION);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CManualLoadPositionSettingDlg::OnManualLoadPositionSetupButton()
{
	UpdateData();

	COverlayDialog dlgOverlay;
	//Saiki 20090529 Add ----->
	CString strBuffer;
	//Saiki 20090529 Add <-----
	dlgOverlay.m_lX = m_ManualLoadPosition.lX;
	dlgOverlay.m_lY = m_ManualLoadPosition.lY;

	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(MANUALLOADPOSITIONSETTING_CAPTION);
	//dlgOverlay.SetMessage(MANUALLOADPOSITIONSETTING_CAPTION);
	LoadStringML(IDS_MANUALLOADPOSITIONSETTING_CAPTION, strBuffer, "MANUAL LOAD POSITION SETTING");
	dlgOverlay.SetCaption(strBuffer);
	dlgOverlay.SetMessage(strBuffer);
	//Saiki 20090529 Change <-

	if ( dlgOverlay.DoModal() == IDOK ) {
		m_ManualLoadPosition.lX = dlgOverlay.m_lX;
		m_ManualLoadPosition.lY = dlgOverlay.m_lY;
	}

	UpdateData(FALSE);
}
