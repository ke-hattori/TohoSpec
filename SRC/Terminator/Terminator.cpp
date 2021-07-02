// Terminator.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "Terminator.h"
#include "TerminatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerminatorApp

BEGIN_MESSAGE_MAP(CTerminatorApp, CWinApp)
	//{{AFX_MSG_MAP(CTerminatorApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerminatorApp クラスの構築

CTerminatorApp::CTerminatorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CTerminatorApp オブジェクト

CTerminatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTerminatorApp クラスの初期化

BOOL CTerminatorApp::InitInstance()
{
	// 標準的な初期化処理

	CTerminatorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
		else if (nResponse == IDCANCEL)
	{
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
