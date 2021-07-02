// LogOnDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "LogOnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CLogOnDlg ダイアログ
// #########################################################################

// =========================================================================
//
CLogOnDlg::CLogOnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogOnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogOnDlg)
	m_strUserName = _T("");
	m_strUserPassword = _T("");
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CLogOnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogOnDlg)
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_USER_PASSWORD, m_strUserPassword);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CLogOnDlg, CDialog)
	//{{AFX_MSG_MAP(CLogOnDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CLogOnDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CLogOnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_USER_NAME))->SetLimitText(RECIPE_NAME_LEN);
	((CEdit*)GetDlgItem(IDC_USER_PASSWORD))->SetLimitText(PASSWORD_LEN);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CLogOnDlg::OnOK()
{
// Kojika 20090526 Add
	CString strBuffer, strTitle;
// Kojika 20090526 Add End

	UpdateData();

	// ユーザアカウント入力なし
	if ( m_strUserName.IsEmpty() ) {
// Kojika 20090526 Change
//		MessageBox("User Account is empty", "NanoSpec", MB_OK | MB_ICONSTOP);
//		MessageBox("ユーザアカウントが入力されていません", "NanoSpec", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCOUNT_EMPTY, strBuffer, "User Account is empty");
		LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End

		GetDlgItem(IDC_USER_NAME)->SetFocus();
		return;
	}

	if ( m_strUserName == USER_ACCOUNT_TOHO_ENGINEER ) {
		strcpy(m_UserAccount.szName, USER_ACCOUNT_TOHO_ENGINEER);
		m_UserAccount.nAccountLevel = ACCOUNT_LEVEL_ADMINISTRATOR;
		m_UserAccount.bEnablePassword = TRUE;
		strcpy(m_UserAccount.szPassword, USER_ACCOUNT_TOHO_PASSWORD);
	}
	else {
		// ユーザアカウント該当なし
		if ( !RecipeFile_LoadRecipe(&m_UserAccount, m_strUserName, RECIPE_FILE_USER_ACCOUNT) ) {
// Kojika 20090526 Change
//			MessageBox("User Account or Password is diffrent", "NanoSpec", MB_OK | MB_ICONSTOP);
//			MessageBox("ユーザアカウントかパスワードが違います", "NanoSpec", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_ACCOUNT_PASS_DIFF, strBuffer, "User Account or Password is diffrent");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End

			GetDlgItem(IDC_USER_NAME)->SetFocus();
			return;
		}
	}

	// パスワードチェック
	if ( m_UserAccount.bEnablePassword ) {
		if ( m_strUserPassword != m_UserAccount.szPassword ) {
// Kojika 20090526 Change
//			MessageBox("User Name or Password is diffrent", "NanoSpec", MB_OK | MB_ICONSTOP);
//			MessageBox("ユーザ名かパスワードが違います", "NanoSpec", MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_NAME_PASS_DIFF, strBuffer, "User Name or Password is diffrent");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End
			GetDlgItem(IDC_USER_NAME)->SetFocus();
			return;
		}
	}

	// ユーザーレベル チェック
	if ( m_UserAccount.nAccountLevel < ACCOUNT_LEVEL_ADMINISTRATOR || m_UserAccount.nAccountLevel >= ACCOUNT_LEVEL_MAX ) {
		m_UserAccount.nAccountLevel = ACCOUNT_LEVEL_OPERATOR;
	}

	CDialog::OnOK();
}
