// HostSetupDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "nanospec.h"
#include "HostSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostSetupDlg ダイアログ


CHostSetupDlg::CHostSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHostSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHostSetupDlg)
	m_nPortNumber = 0;
	//}}AFX_DATA_INIT
}

void CHostSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);

	//{{AFX_DATA_MAP(CHostSetupDlg)
	DDX_Control(pDX, IDC_MY_IP_ADDRESS, m_localIpAddress);
	DDX_Text(pDX, IDC_MY_PORT, m_nPortNumber);
	DDV_MinMaxUInt(pDX, m_nPortNumber, 0, 65535);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHostSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CHostSetupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHostSetupDlg メッセージ ハンドラ

BOOL CHostSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int i1, i2, i3, i4;

	///// Get Data /////
	ConfigFile_GetNanoSpecIni(&m_HostAddress, CONFIG_FILE_HOST);

	if ( strlen(m_HostAddress.szMyIpAddress) != 0 )
		sscanf(m_HostAddress.szMyIpAddress, "%d.%d.%d.%d", &i1, &i2, &i3, &i4);
	else
		i1 = i2 = i3 = i4 = 0;
	m_localIpAddress.SetAddress(MAKEIPADDRESS(i1, i2, i3, i4));
	m_nPortNumber = m_HostAddress.nPort;

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);


	UpdateData(FALSE);

	return TRUE;
}

void CHostSetupDlg::OnOK()
{
	if ( !UpdateData() )
		return;

	char szBuff[256];
	BYTE by1, by2, by3, by4;

	m_localIpAddress.GetAddress(by1, by2, by3, by4);
	sprintf(szBuff, "%d.%d.%d.%d", by1, by2, by3, by4);
	if ( strcmp(szBuff, "0.0.0.0") == 0 )
		strcpy(szBuff, "");
	strcpy(m_HostAddress.szMyIpAddress, szBuff);
	m_HostAddress.nPort = m_nPortNumber;

	ConfigFile_SetNanoSpecIni(&m_HostAddress, CONFIG_FILE_HOST);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

