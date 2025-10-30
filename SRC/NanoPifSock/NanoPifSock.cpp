// NanoPifSock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#define _MASTER_
#include "System.h"
#include "NanoPifSock.h"
#include "NanoPifSockWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Global variable
HANDLE g_hEventShutDown;


/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockApp

BEGIN_MESSAGE_MAP(CNanoPifSockApp, CWinApp)
	//{{AFX_MSG_MAP(CNanoPifSockApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockApp construction

CNanoPifSockApp::CNanoPifSockApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNanoPifSockApp object

CNanoPifSockApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockApp initialization

BOOL CNanoPifSockApp::InitInstance()
{
	// アプリケーションの二重起動防止
	::CreateMutex(NULL, TRUE, m_pszExeName);
	if ( ::GetLastError() == ERROR_ALREADY_EXISTS ) {
		return FALSE;
	}

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	g_lAppNameType = APP_NAME_NANO; // default application name
	for(int i = 0; i < APP_NAME_MAX; i++){
		if(_tcsncmp(m_pszExeName, g_lpszAppPrefix4[i], 4) == 0){
			g_lAppNameType = i;
			break;
		}
	}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//First free the string allocated by MFC at CWinApp startup.
	//The string is allocated before InitInstance is called.
	free((void*)m_pszProfileName);

	//Change the name of the .INI file.
	//The CWinApp destructor will free the memory.
	m_pszProfileName = _tcsdup(CFG_DIR SYSTEMCOMMON_INIFILENAME);

	g_hEventShutDown = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	m_pMainWnd = new CNanoPifSockWnd();
	m_pMainWnd->CreateEx(0, AfxRegisterWndClass(0), "NanoPifSock", WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), NULL, 0);
//	m_pMainWnd->ShowWindow(SW_SHOW);
//	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CNanoPifSockApp::ExitInstance()
{
	if ( g_hEventShutDown ) {
		::CloseHandle(g_hEventShutDown);
		g_hEventShutDown = NULL;
	}

	return CWinApp::ExitInstance();
}
