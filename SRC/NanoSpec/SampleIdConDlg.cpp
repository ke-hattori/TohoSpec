// SampleIdConDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "SampleIdConDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleIdConDlg ダイアログ


CSampleIdConDlg::CSampleIdConDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleIdConDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleIdConDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSampleIdConDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleIdConDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSampleIdConDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleIdConDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleIdConDlg メッセージ ハンドラ

void CSampleIdConDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください



	CDialog::OnOK();
}
