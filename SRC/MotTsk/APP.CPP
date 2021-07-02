/****************************************************************
 *		Copyright (C) 2003 by Tencor Instruments
 *
 *		Source Upgrade From VC1.52 to VC6.0
 *
 *		Updated by Ouyang-jc
 *
 *		Date:	2003/03/26
 ****************************************************************/

// ===========================================================================
// Include files
// ===========================================================================

#include <afxwin.h>

#include "..\\..\\inc\\MotSysMsg.h"

#include "app.h"
#include "win.h"




// ===========================================================================
// Create instance of application (equivalent to WinMain)
// ===========================================================================

static CMotTskApp ThisApp;



/*---------------------------------------------------------------------------

   CMotTskApp

   InitInstance:
   Called once when the application begins.  All we need to do is to create
   a new CMainWnd object which can handle itself once made visible.

---------------------------------------------------------------------------*/
BOOL CALLBACK FindWindowProc(HWND hwnd, LPARAM lParam);

BOOL CMotTskApp::InitInstance ()
{
	// allow only one instance of this app
	if (m_hPrevInstance != NULL)
		return FALSE;

#ifdef _DEBUG			   // Enable trace flags, output debug Window
	afxTraceFlags	= 0;
	afxTraceEnabled = 1;
#endif

   TRACE ("MOTION SYSTEM Task -- Started \n");

   	// 二重起動を防止
//	m_hMutex_Run = ::CreateMutex(NULL, TRUE, m_pszExeName);
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, IDENT_CODE);	// exe のリネームでも実行出来ないようにするため特定の名前を設定
	if ((m_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		// すでに起動しているプロセスを前面にする
		::EnumWindows(&FindWindowProc, NULL);
		return	FALSE;
	}

	m_pMainWnd = new cMotTskWnd();
	m_pMainWnd->ShowWindow(m_nCmdShow);
/* added 2009.06.01 hmenjo Servo.ini で初期最小化を設定 ---------- { ---------- */
	TCHAR	l_tszText[32];
	::GetPrivateProfileString(_T("Window"), _T("AutoHide"), _T("2"), l_tszText, sizeof(l_tszText), _T("..\\cfg\\Servo.ini"));
	int l_iAutoHide = _ttoi(l_tszText);
	if ((l_iAutoHide < 0) || (2 < l_iAutoHide)) {
		l_iAutoHide = 2;
	}
	switch (l_iAutoHide) {
	case 1:
		m_pMainWnd->ShowWindow(SW_MINIMIZE);
	case 2:
		m_pMainWnd->ShowWindow(SW_HIDE);
	case 0:
	default:
		break;
	}
/* added 2009.06.01 hmenjo Servo.ini で初期最小化を設定 ---------- } ---------- */
	m_pMainWnd->UpdateWindow();

	return TRUE;
}
int CMotTskApp::ExitInstance()
{
	AfxTrace("CMotTskApp::ExitInstance\n");
	if (m_pMainWnd) {
		delete m_pMainWnd;
	}

	// 二重起動の防止のための Mutex 開放
	if (m_hMutex_Run != NULL) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}

	return true;
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
