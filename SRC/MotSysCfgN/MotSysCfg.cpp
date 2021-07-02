// MotSysCfg.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "MotSysCfg.h"
#include "MotSysCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotSysCfgApp

BEGIN_MESSAGE_MAP(CMotSysCfgApp, CWinApp)
	//{{AFX_MSG_MAP(CMotSysCfgApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysCfgApp クラスの構築

CMotSysCfgApp::CMotSysCfgApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CMotSysCfgApp オブジェクト

CMotSysCfgApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMotSysCfgApp クラスの初期化

BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);

BOOL CMotSysCfgApp::InitInstance()
{
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	// 二重起動の防止処理
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, IDENT_CODE);	// exe のリネームでも実行出来ないようにするため特定の名前を設定
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		// すでに起動しているプロセスを前面にする
		::EnumWindows(&FindWindowProc, NULL);
		return	FALSE;
	}

	CMotSysCfgDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//		 記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//		 記述してください。
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

int CMotSysCfgApp::ExitInstance()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	// 二重起動の防止のための Mutex 開放
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}

	return CWinApp::ExitInstance();
}

/* EnumWindowsProcコールバック関数 */
BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam)
{
	/* 見つかったウィンドウが、すでに起動しているアプリケーションのウィンドウか調べる */
	if(::GetProp(hwnd, IDENT_CODE)) {
		/* アイコン化されているのであれば、元のサイズに戻す */
		if (IsIconic(hwnd)) {
			::ShowWindow(hwnd, SW_RESTORE);
		} else {
			::ShowWindow(hwnd, SW_SHOWNORMAL);
		}
		/* 見つかったウィンドウをフォアグラウンドウィンドウにする */
		::SetForegroundWindow(::GetLastActivePopup(hwnd));
		// 表示する(ここ(SetForegroundWindowの後)じゃないと１回の SW_SHOW で表示されなかった)
		return FALSE;	/* 列挙を中断 */
	}
	return TRUE;	/* 列挙を続ける */
}
