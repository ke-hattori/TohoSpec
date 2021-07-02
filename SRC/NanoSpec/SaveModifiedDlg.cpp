// SaveModifiedDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "SaveModifiedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSaveModifiedDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSaveModifiedDlg::CSaveModifiedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveModifiedDlg::IDD, pParent)
{
}

CSaveModifiedDlg::CSaveModifiedDlg(LPCTSTR pszCaption, CWnd* pParent /*=NULL*/)
	: CDialog(CSaveModifiedDlg::IDD, pParent)
{
	m_strModified = pszCaption;
}

// =========================================================================
//
void CSaveModifiedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveModifiedDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSaveModifiedDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveModifiedDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSaveModifiedDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSaveModifiedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//Saiki 20090527 Add ----->
	CString strBuffer;
	//Saiki 20090527 Add <-----
	///// Ok Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	//m_strModified += "\nhas been modified.";
	//Saiki 20090527 Change ----->
	//m_strModified += "\nは変更されています";
	LoadStringML(IDS_HAS_MODIFIED, strBuffer, "\nhas been modified.");
	m_strModified += strBuffer;
	//Saiki 20090527 Change <-----
	SetDlgItemText(IDC_TEXT, m_strModified);

	CheckDlgButton(IDC_RADIO_SAVE, BST_CHECKED);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSaveModifiedDlg::OnOK()
{
	m_iModified = IsDlgButtonChecked(IDC_RADIO_SAVE) == BST_CHECKED ? MODIFIED_SAVE : MODIFIED_DISCARD;

	CDialog::OnOK();
}
