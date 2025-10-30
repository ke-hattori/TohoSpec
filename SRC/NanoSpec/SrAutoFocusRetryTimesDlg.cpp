// SrAutoFocusRetryTimesDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "nanospec.h"
#include "SrAutoFocusRetryTimesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SrAutoFocusRetryTimesDlg ダイアログ


SrAutoFocusRetryTimesDlg::SrAutoFocusRetryTimesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SrAutoFocusRetryTimesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SrAutoFocusRetryTimesDlg)
	//}}AFX_DATA_INIT
}


void SrAutoFocusRetryTimesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SrAutoFocusRetryTimesDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Text(pDX, IDC_RETRY_TIMES, m_SrAutoFocusRetry.nTimes);
	DDV_MinMaxUInt(pDX, m_SrAutoFocusRetry.nTimes, 0, 10);
}


BEGIN_MESSAGE_MAP(SrAutoFocusRetryTimesDlg, CDialog)
	//{{AFX_MSG_MAP(SrAutoFocusRetryTimesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SrAutoFocusRetryTimesDlg メッセージ ハンドラ
void SrAutoFocusRetryTimesDlg::OnOK()
{
	UpdateData(TRUE);

	ConfigFile_SetNanoSpecIni(&m_SrAutoFocusRetry, CONFIG_FILE_SR_AUTOFOCUS_RETRY);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);

	CDialog::OnOK();
}


BOOL SrAutoFocusRetryTimesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Get Data /////
	ConfigFile_GetNanoSpecIni(&m_SrAutoFocusRetry, CONFIG_FILE_SR_AUTOFOCUS_RETRY);

	UpdateData(FALSE);

	return TRUE;
}
