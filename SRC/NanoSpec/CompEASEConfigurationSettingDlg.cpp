// CompEASEConfigurationSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "CompEASEConfigurationSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompEASEConfigurationSettingDlg ダイアログ


CCompEASEConfigurationSettingDlg::CCompEASEConfigurationSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompEASEConfigurationSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompEASEConfigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CCompEASEConfigurationSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompEASEConfigurationSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CCompEASEConfigurationSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CCompEASEConfigurationSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompEASEConfigurationSettingDlg メッセージ ハンドラ

BOOL CCompEASEConfigurationSettingDlg::OnInitDialog()
{
	CString strBuffer;

	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_CompEaseConfig, CONFIG_FILE_COMPEASE_CONFIG);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	// 0-9999までの入力制限
	((CEdit*)GetDlgItem(IDC_EASE_ANALYZE_TIMEOUT_EDIT))->SetLimitText(4);
	GetDlgItem(IDC_EASE_ANALYZE_TIMEOUT_EDIT)->ModifyStyle(0, ES_NUMBER);

	///// Set EASE Analyze Timeout /////
	strBuffer.Format("%d", m_CompEaseConfig.nAnalyzeTimeout);
	SetDlgItemText(IDC_EASE_ANALYZE_TIMEOUT_EDIT, strBuffer);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CCompEASEConfigurationSettingDlg::OnOK()
{
	CString strBuffer;

	///// Save /////
	GetDlgItemText(IDC_EASE_ANALYZE_TIMEOUT_EDIT, strBuffer);
	m_CompEaseConfig.nAnalyzeTimeout = atoi(strBuffer);

	ConfigFile_SetNanoSpecIni(&m_CompEaseConfig, CONFIG_FILE_COMPEASE_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMPEASE_INI_FILE);

	CDialog::OnOK();
}
