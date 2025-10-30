// Procon.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "Procon.h"
#include "ProconDlg.h"

#include "..\\..\\INC\\AlarmIf.hxx"

#pragma comment(lib, "..\\..\\LIB\\dtnsAlarmIf.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProconApp

BEGIN_MESSAGE_MAP(CProconApp, CWinApp)
	//{{AFX_MSG_MAP(CProconApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProconApp クラスの構築

CProconApp::CProconApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CProconApp オブジェクト

CProconApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProconApp クラスの初期化

BOOL CProconApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 標準的な初期化処理

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	CProconDlg dlg;
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
