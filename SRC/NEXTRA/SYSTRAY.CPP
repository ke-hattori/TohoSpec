// SYSTRAY.CPP : implementation file
//

#include "stdafx.h"
#include "common.hpp"
#include "nextra.h"
#include "systray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------------
// CSystemTray

CSystemTray::CSystemTray()
{
	::ZeroMemory(&m_NotifyIconData, sizeof(m_NotifyIconData));
	nexiniGetParameter(&m_params);
}

CSystemTray::~CSystemTray()
{
	// Remove icon from system tray
	m_NotifyIconData.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);

	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSystemTray, CWnd)
	//{{AFX_MSG_MAP(CSystemTray)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//---------------------------------------------------------------------------
// CSystemTray message handlers

//---------------------------------------------------------------------------
// Create
BOOL CSystemTray::Create(UINT uCallbackMessage, LPCTSTR lpszToolTip, HICON hIcon, UINT uID)
{
	// Create dummy window
	if ( !CWnd::CreateEx(0, AfxRegisterWndClass(0), "CSystemTray Dummy Window", WS_POPUP, 0, 0, 0, 0, NULL, 0) )
		return FALSE;

	// Setup NOTIFYICONDATA
	m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIconData.hWnd	= m_hWnd;
	m_NotifyIconData.uID	= uID;
	m_NotifyIconData.hIcon	= hIcon;
	m_NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_NotifyIconData.uCallbackMessage = uCallbackMessage;
	lstrcpy(m_NotifyIconData.szTip, lpszToolTip);

	// Add tray icon
	return Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
}

//---------------------------------------------------------------------------
// SetIcon
BOOL CSystemTray::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);
	m_NotifyIconData.uFlags = NIF_ICON;
	m_NotifyIconData.hIcon = hIcon;
	return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData);
}

//---------------------------------------------------------------------------
// SetNotificationWnd
BOOL CSystemTray::SetNotificationWnd(CWnd* pNotifyWnd)
{
	m_NotifyIconData.hWnd = pNotifyWnd->GetSafeHwnd();
	m_NotifyIconData.uFlags = 0;
	return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData);

}

//---------------------------------------------------------------------------
// OnTrayNotification
LRESULT CSystemTray::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if ( wParam != m_NotifyIconData.uID )
		return 0L;

	CMenu menu, *pSubMenu;
	CWnd* pMainWnd = AfxGetMainWnd();

	switch ( LOWORD(lParam) ) {
	case WM_RBUTTONUP:
	{
		if ( !menu.LoadMenu(m_NotifyIconData.uID) )
			return 0L;

		pSubMenu = menu.GetSubMenu(0);

		if ( pSubMenu == NULL )
			return 0L;

		if ( !((CNextraApp*) AfxGetApp())->IsHostOption() ) {
			pSubMenu->EnableMenuItem(IDM_MANUAL_OPERATION, MF_ENABLED | MF_BYCOMMAND);
			switch ( m_params.System.Type ) {
			case NEXTRA_TYPEA:
				pSubMenu->EnableMenuItem(IDM_TEACH_OPERATION, MF_ENABLED | MF_BYCOMMAND);
				break;
			case NEXTRA_TYPEB:
			default:
				break;
			}
		}

		CPoint pos;
		GetCursorPos(&pos);

		pMainWnd->SetForegroundWindow();

		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, pos.x, pos.y, pMainWnd, NULL);

		pMainWnd->PostMessage(WM_NULL, 0, 0);

		menu.DestroyMenu();
		break;
	}
	case WM_LBUTTONDBLCLK:
	{
		pMainWnd->ShowWindow(SW_SHOW);
		pMainWnd->SetForegroundWindow();
		pMainWnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		break;
	}
	default:
		break;
	}
	return 1;
}

//---------------------------------------------------------------------------
// WindowProc
LRESULT CSystemTray::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message == m_NotifyIconData.uCallbackMessage ) {
		return OnTrayNotification(wParam, lParam);
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
