// DataSearchDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "DataSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CDataSearchDlg ダイアログ
// #########################################################################

// =========================================================================
//
CDataSearchDlg::CDataSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataSearchDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CDataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSearchDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_SEARCH_BUTTON, m_SearchButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CDataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CDataSearchDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CDataSearchDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CDataSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// Search Button /////
	m_SearchButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SearchButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}
