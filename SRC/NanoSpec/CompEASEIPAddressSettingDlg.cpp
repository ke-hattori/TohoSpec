// CompEASEIPAddressSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "CompEASEIPAddressSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MSGBOX_CAPTION		"EASE IPADDRESS SETTING"

// #########################################################################
// CCompEASEIPAddressSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CCompEASEIPAddressSettingDlg::CCompEASEIPAddressSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompEASEIPAddressSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompEASEIPAddressSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CCompEASEIPAddressSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompEASEIPAddressSettingDlg)
	DDX_Control(pDX, IDC_COMPEASE_ADDRESS, m_IpAddress);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CCompEASEIPAddressSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CCompEASEIPAddressSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CCompEASEIPAddressSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CCompEASEIPAddressSettingDlg::OnInitDialog()
{
	CString strBuffer;
	int i1, i2, i3, i4;

	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_CompEaseConfig, CONFIG_FILE_COMPEASE_CONFIG);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMPEASE_PORT_EDIT))->SetLimitText(5);
	((CEdit*)GetDlgItem(IDC_COMPEASE_ASYNC_PORT_EDIT))->SetLimitText(5);

	///// Set EASE Address /////
	if ( strlen(m_CompEaseConfig.szIpAddress) != 0 )
		sscanf(m_CompEaseConfig.szIpAddress, "%d.%d.%d.%d", &i1, &i2, &i3, &i4);
	else
		i1 = i2 = i3 = i4 = 0;
	m_IpAddress.SetAddress(MAKEIPADDRESS(i1, i2, i3, i4));

	///// Set EASE Port /////
	strBuffer.Format("%d", m_CompEaseConfig.nPort);
	SetDlgItemText(IDC_COMPEASE_PORT_EDIT, strBuffer);

	///// Set EASE Async Port /////
	strBuffer.Format("%d", m_CompEaseConfig.nAsyncPort);
	SetDlgItemText(IDC_COMPEASE_ASYNC_PORT_EDIT, strBuffer);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CCompEASEIPAddressSettingDlg::OnOK()
{
	CString strBuffer;

	if ( !UpdateData() ) {
		return;
	}

	///// Save /////
	BYTE by1, by2, by3, by4;

	m_IpAddress.GetAddress(by1, by2, by3, by4);
	strBuffer.Format("%d.%d.%d.%d", by1, by2, by3, by4);
	if ( strBuffer == "0.0.0.0" ) {
		strBuffer = "";
	}
	strcpy(m_CompEaseConfig.szIpAddress, strBuffer);

	GetDlgItemText(IDC_COMPEASE_PORT_EDIT, strBuffer);
	m_CompEaseConfig.nPort = atoi(strBuffer);

	GetDlgItemText(IDC_COMPEASE_ASYNC_PORT_EDIT, strBuffer);
	m_CompEaseConfig.nAsyncPort = atoi(strBuffer);

	ConfigFile_SetNanoSpecIni(&m_CompEaseConfig, CONFIG_FILE_COMPEASE_CONFIG);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMPEASE_INI_FILE);

	CDialog::OnOK();
}
