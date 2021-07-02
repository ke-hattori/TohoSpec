// FocusConfigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "FocusConfigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// #########################################################################
// CFocusConfigurationSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CFocusConfigurationSettingDlg::CFocusConfigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFocusConfigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFocusConfigurationSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CFocusConfigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFocusConfigurationSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_Z_ENCODER, m_FocusConfig.Enc.bValid);
	DDX_Check(pDX, IDC_CHECK_Z_OFFSET_CONTROL, m_FocusConfig.bOffsetCtrl);
	DDX_Text(pDX, IDC_Z_ENCODER_DIV_VALUE, m_FocusConfig.Enc.dDiv);
	DDX_Text(pDX, IDC_Z_ENCODER_OFFSET, m_FocusConfig.Enc.dOffset);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CFocusConfigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CFocusConfigurationSettingDlg)
	ON_BN_CLICKED(IDC_CHECK_Z_ENCODER, OnCheckZEncoder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CFocusConfigurationSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CFocusConfigurationSettingDlg::OnInitDialog()
{
	///// Load Data /////
	ConfigFile_GetNanoSpecIni(&m_FocusConfig, CONFIG_FILE_FOCUS_CONFIG);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	OnCheckZEncoder();

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CFocusConfigurationSettingDlg::OnOK()
{
	///// Save /////
	if ( !UpdateData() )
		return;

	ConfigFile_SetNanoSpecIni(&m_FocusConfig, CONFIG_FILE_FOCUS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CFocusConfigurationSettingDlg::OnCheckZEncoder()
{
	UpdateData();

	BOOL bCheck = m_FocusConfig.Enc.bValid;
	GetDlgItem(IDC_Z_ENCODER_DIV_VALUE)->EnableWindow(bCheck);
	GetDlgItem(IDC_Z_ENCODER_OFFSET)->EnableWindow(bCheck);

}

// =========================================================================
//
BOOL CFocusConfigurationSettingDlg::CheckData()
{
	if ( !UpdateData() )
		return FALSE;

	CString strBuffer;

	if ( m_FocusConfig.Enc.bValid ) {
		///// Z Encoder Div Value //////
		GetDlgItemText(IDC_Z_ENCODER_DIV_VALUE, strBuffer);
		if ( CheckMinMaxDouble(strBuffer, MIN_Z_ENCORDER, MAX_Z_ENCORDER) != CHECK_DATA_OK ) {
			GetDlgItem(IDC_Z_ENCODER_DIV_VALUE)->SetFocus();
			return FALSE;
		}

		///// Z Encoder Offset /////
		GetDlgItemText(IDC_Z_ENCODER_OFFSET, strBuffer);
		if ( CheckMinMaxDouble(strBuffer, MIN_Z_ENCORDER, MAX_Z_ENCORDER) != CHECK_DATA_OK) {
			GetDlgItem(IDC_Z_ENCODER_OFFSET)->SetFocus();
			return FALSE;
		}
	}
	return TRUE;
}
