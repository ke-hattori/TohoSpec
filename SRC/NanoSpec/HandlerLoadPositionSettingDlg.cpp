// HandlerLoadPositionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "HandlerLoadPositionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define																   //
//																		   //
// ==========================================================================
//#define HANDERLOADPOSITIONSETTING_CAPTION 	"HANDLER LOAD POSITION SETTING"
//#define HANDERLOADPOSITIONSETTING_CAPTION 	  "ハンドラー・ロードポジション セッティング"

// ##########################################################################
// CHandlerLoadPositionSettingDlg ダイアログ
// ##########################################################################

// ==========================================================================
//
CHandlerLoadPositionSettingDlg::CHandlerLoadPositionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHandlerLoadPositionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHandlerLoadPositionSettingDlg)
	//}}AFX_DATA_INIT
}

// ==========================================================================
//
void CHandlerLoadPositionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHandlerLoadPositionSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_HANDLER_LOAD_POSITION_X, m_HandlerLoadPosition.lX);
	DDX_Text(pDX, IDC_HANDLER_LOAD_POSITION_Y, m_HandlerLoadPosition.lY);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_HANDLER_LOAD_POSITION_SETUP_BUTTON, m_HandlerLoadPositionSetupButton);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CHandlerLoadPositionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CHandlerLoadPositionSettingDlg)
	ON_BN_CLICKED(IDC_HANDLER_LOAD_POSITION_SETUP_BUTTON, OnHandlerLoadPositionSetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ==========================================================================
// CHandlerLoadPositionSettingDlg メッセージ ハンドラ

// ==========================================================================
//
BOOL CHandlerLoadPositionSettingDlg::OnInitDialog()
{
	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_HandlerLoadPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Handler Load Position Setup Button /////
	m_HandlerLoadPositionSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_HandlerLoadPositionSetupButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_stcHandlerLoadPosX.SubclassWindow(GetDlgItem(IDC_HANDLER_LOAD_POSITION_X)->GetSafeHwnd());
	m_stcHandlerLoadPosX.SetBkColor(REFERENCE_COLOR);
	m_stcHandlerLoadPosY.SubclassWindow(GetDlgItem(IDC_HANDLER_LOAD_POSITION_Y)->GetSafeHwnd());
	m_stcHandlerLoadPosY.SetBkColor(REFERENCE_COLOR);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
//
void CHandlerLoadPositionSettingDlg::OnOK()
{
	///// Save /////
	UpdateData();

	ConfigFile_SetNanoSpecIni(&m_HandlerLoadPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// ==========================================================================
//
void CHandlerLoadPositionSettingDlg::OnHandlerLoadPositionSetupButton()
{
	UpdateData();

	COverlayDialog dlgOverlay;
	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(HANDERLOADPOSITIONSETTING_CAPTION);
	CString strBuffer;

	LoadStringML(IDS_HANDERLOADPOSITIONSETTING_CAPTION, strBuffer, "HANDLER LOAD POSITION SETTING");
	dlgOverlay.SetCaption(strBuffer);
	//Saiki 20090529 Change <-----

	dlgOverlay.m_lX = m_HandlerLoadPosition.lX;
	dlgOverlay.m_lY = m_HandlerLoadPosition.lY;
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	if ( dlgOverlay.DoModal() == IDOK ) {
		m_HandlerLoadPosition.lX = dlgOverlay.m_lX;
		m_HandlerLoadPosition.lY = dlgOverlay.m_lY;
	}

	UpdateData(FALSE);
}
