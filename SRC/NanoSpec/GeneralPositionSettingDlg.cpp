// GeneralPositionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "GeneralPositionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define                                                                  //
//                                                                         //
// ==========================================================================
//#define GENERALPOSITIONSETTING_CAPTION		"GENERAL POSITION SETTING"
//#define GENERALPOSITIONSETTING_MESSAGE		"GENERAL POSITION SETTING"

// #########################################################################
// CGeneralPositionSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CGeneralPositionSettingDlg::CGeneralPositionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralPositionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeneralPositionSettingDlg)
	//}}AFX_DATA_INIT
	m_iCurrNo = 0;
}

// =========================================================================
//
void CGeneralPositionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPositionSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_GENERAL_POSITION_X, m_TempGeneralPosition[m_iCurrNo].Loc.lX);
	DDX_Text(pDX, IDC_GENERAL_POSITION_Y, m_TempGeneralPosition[m_iCurrNo].Loc.lY);
	DDX_Text(pDX, IDC_GENERAL_POSITION_COMMENT, m_TempGeneralPosition[m_iCurrNo].szComment, GENERAL_POSITION_COMMENT_LEN + 1);
	DDX_CBIndex(pDX, IDC_GENERAL_POSITION_NO, m_iCurrNo);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_GENERAL_POSITION_SETUP_BUTTON, m_GeneralPositionSetupButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CGeneralPositionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralPositionSettingDlg)
	ON_CBN_SELCHANGE(IDC_GENERAL_POSITION_NO, OnSelchangeGeneralPositionNo)
	ON_BN_CLICKED(IDC_GENERAL_POSITION_SETUP_BUTTON, OnGeneralPositionSetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CGeneralPositionSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CGeneralPositionSettingDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	UpdateData();

	m_iCurrNo = 0;

	////// Load File /////
	ConfigFile_GetNanoSpecIni(m_TempGeneralPosition, CONFIG_FILE_GENERAL_POSITION);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// General Position Setup Button /////
	m_GeneralPositionSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GeneralPositionSetupButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_stcGeneralPosX.SubclassWindow(GetDlgItem(IDC_GENERAL_POSITION_X)->GetSafeHwnd());
	m_stcGeneralPosX.SetBkColor(REFERENCE_COLOR);
	m_stcGeneralPosY.SubclassWindow(GetDlgItem(IDC_GENERAL_POSITION_Y)->GetSafeHwnd());
	m_stcGeneralPosY.SetBkColor(REFERENCE_COLOR);

	///// Control Initialize /////
	InitCombo_GeneralPosNo();

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_GENERAL_POSITION_COMMENT))->SetLimitText(GENERAL_POSITION_COMMENT_LEN);

	UpdateData(FALSE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CGeneralPositionSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(m_TempGeneralPosition, CONFIG_FILE_GENERAL_POSITION);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CGeneralPositionSettingDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	CDialog::OnCancel();
}

// =========================================================================
//
void CGeneralPositionSettingDlg::OnSelchangeGeneralPositionNo()
{
	UpdateData();			// No切り替え前の情報を取得する
	UpdateData(FALSE);		// No切り替え後の情報にて画面更新する
}

// =========================================================================
//
void CGeneralPositionSettingDlg::OnGeneralPositionSetupButton()
{
	// Kojika 20090604 Add
	CString l_strCap, l_strMsg;
	// Kojika 20090604 Add End

	UpdateData();

	COverlayDialog dlgOverlay;

	dlgOverlay.m_lX = m_TempGeneralPosition[m_iCurrNo].Loc.lX;
	dlgOverlay.m_lY = m_TempGeneralPosition[m_iCurrNo].Loc.lY;
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	// Kojika 20090604 Change
	//dlgOverlay.SetCaption(GENERALPOSITIONSETTING_CAPTION);
	//dlgOverlay.SetMessage(GENERALPOSITIONSETTING_MESSAGE);
	LoadStringML(IDS_GENERAL_POSITION_SETTING_CAPTION, l_strCap, "GENERAL POSITION SETTING");
	LoadStringML(IDS_GENERAL_POSITION_SETTING_MESSAGE, l_strMsg, "GENERAL POSITION SETTING");
	dlgOverlay.SetCaption(l_strCap);
	dlgOverlay.SetMessage(l_strMsg);
	// Kojika 20090604 Change End

	if ( dlgOverlay.DoModal() == IDOK ) {
		m_TempGeneralPosition[m_iCurrNo].Loc.lX = dlgOverlay.m_lX;
		m_TempGeneralPosition[m_iCurrNo].Loc.lY = dlgOverlay.m_lY;
	}

	UpdateData(FALSE);
}

// =========================================================================
//
void CGeneralPositionSettingDlg::InitCombo_GeneralPosNo()
{
	CString strBuffer;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_GENERAL_POSITION_NO);
	pCombo->ResetContent();

	for ( int i = 0; i < GENERAL_POSITION_MAX; i++ ) {
		strBuffer.Format("%d", i + 1);
		pCombo->AddString(strBuffer);
	}
	pCombo->SetCurSel(m_iCurrNo);
}
