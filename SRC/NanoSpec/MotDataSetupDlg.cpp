// MotDataSetupDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "MotDataSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotDataSetupDlg ダイアログ


CMotDataSetupDlg::CMotDataSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotDataSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotDataSetupDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMotDataSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotDataSetupDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMotDataSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CMotDataSetupDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotDataSetupDlg メッセージ ハンドラ
