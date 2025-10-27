// NEXTRA.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "common.hpp"
#include "nextra.h"
#include "alarm.h"
#include "nextradlg.h"
#include "jogopdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CNextraApp
// ===========================================================================

BEGIN_MESSAGE_MAP(CNextraApp, CWinApp)
	//{{AFX_MSG_MAP(CNextraApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// ===========================================================================
// CNextraApp construction
// ===========================================================================

CNextraApp::CNextraApp() : m_lfs(NEXTRA_LOGFILE_NAME)
{
}

// ===========================================================================
// The one and only CNextraApp object
// ===========================================================================

CNextraApp theApp;

// ===========================================================================
// CNextraApp initialization
// ===========================================================================

BOOL CNextraApp::InitInstance()
{
	// Standard initialization

	::CreateMutex(NULL, TRUE, m_pszExeName);
	if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
		CWnd* pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while ( pPrevWnd ) {
			if ( ::GetProp(pPrevWnd->GetSafeHwnd(), m_pszExeName) ) {
				if ( pPrevWnd->IsIconic() )
					pPrevWnd->ShowWindow(SW_RESTORE);

				pPrevWnd->SetForegroundWindow();
				pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
			}
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		return FALSE;
	}

	NEXINIPARAMS params;
	nexiniGetParameter(&params);
	HANDLE m_hModule = NexResourceLoadLibrary(params.System.Language);
	AfxSetResourceHandle((HINSTANCE) m_hModule);

#ifdef _AFXDLL
	// Enable3dControls(); 		// No longer needed in modern MFC
#else
	// Enable3dControlsStatic();	// No longer needed in modern MFC
#endif
	ParseCommandLine(m_cmdInfo);

	m_lfs << arrange
		  << datetime
		  << "START Nextra Application "
		  << NEXTRA_SOFT_VERSION
		  << " ("
		  << __DATE__
		  << SPACE
		  << __TIME__
		  << ")"
		  << std::endl;
	m_lfs.close();

	int nResponse;
	if ( params.System.Mode == FA22_PLS ) {
		if ( IsHostOption() ) {
			MyMessageBox( NULL, IDS_JOGOP_EXECUTE_FAILED, IDS_NEXTRA_APP_TITLE,
				MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
			return FALSE;
		}
		// Create the tray icon
		if ( !m_TrayIcon.Create(WM_ICON_NOTIFY, 			// Icon notify message to use
							 "Jog Application", 			// tooltip
							 LoadIcon(IDI_OFFLINE), 		// Icon
							 NULL) )						// ID of tray icon
			return -1;

		CJogOperationDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	}
	else {
		// Create the tray icon
		if ( !m_TrayIcon.Create(WM_ICON_NOTIFY, 			// Icon notify message to use
							 "Nextra Application",			// tooltip
							 LoadIcon(IDI_OFFLINE), 		// Icon
							 IDR_POPUP_MENU) )				// ID of tray icon
			return -1;

		CNextraDlg dlg;
		m_pMainWnd = &dlg;
		nResponse = dlg.DoModal();
	}
	if ( nResponse == IDOK ) {
	}
	else
	if	(nResponse == IDCANCEL ) {
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//	application, rather than start the application's message pump.
	return FALSE;
}

int CNextraApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
