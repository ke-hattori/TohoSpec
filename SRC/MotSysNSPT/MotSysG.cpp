// MotSys.cpp : アプリケーション用クラスの定義を行います。
//

#include "stdafx.h"
#include "MotSysDef.h"
#include "..\\..\\inc\\MotsysMsg.h"
#include "MotSysDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp

BEGIN_MESSAGE_MAP(CMotSysApp, CWinApp)
	//{{AFX_MSG_MAP(CMotSysApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp クラスの構築

CMotSysApp::CMotSysApp()
{
	// TODO: この位置に構築用のコードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CMotSysApp オブジェクト

CMotSysApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp クラスの初期化

#define PATTERN	2	// 二重起動の防止機能のパターン選択
					/*	1 : FindWindow を使う場合
						2 : Mutex を使う場合
					 */
BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);

BOOL CMotSysApp::InitInstance()
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
#if PATTERN == 1
	if (FindWindow(NULL, "Motion System Driver") != NULL) {
		return FALSE;
	}
#elif PATTERN == 2
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, IDENT_CODE);	// exe のリネームでも実行出来ないようにするため特定の名前を設定
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		// すでに起動しているプロセスを前面にする
		::EnumWindows(&FindWindowProc, NULL);
		return	FALSE;
	}
#endif


	CMotSysDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//       記述してください。
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

int CMotSysApp::ExitInstance()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	// 二重起動の防止のための Mutex 開放
#if PATTERN == 2
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}
#endif

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
