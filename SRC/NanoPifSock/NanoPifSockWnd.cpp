// NanoPifSockWnd.cpp : implementation file
//

#include "stdafx.h"
#include "NanoPifSock.h"
#include "NanoPifSockWnd.h"

// Global variable
extern HANDLE g_hEventShutDown;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INISECTION_WINSOCK			_T("NANOPIFSOCK")
#define INIKEY_SOCKETADDRESS		_T("SocketAddress")
#define INIKEY_PORTNUM				_T("Port")
#define INIKEY_HOSTADDRESS			_T("HostAddress")
#define INIKEY_RUNAS				_T("SetCheckRunAs")
#define INIKEY_AUTORUN				_T("AutoRun")
#define PORTNUM 					_T("5000")
#define SOCKETADDRESS				_T("")
#define HOSTADDRESS 				_T("127.0.0.1")

/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockWnd dialog

CNanoPifSockWnd::CNanoPifSockWnd()
	: m_pLstn(NULL), m_pSock(NULL), m_pEventThread(NULL)
{
}

BEGIN_MESSAGE_MAP(CNanoPifSockWnd, CWnd)
	//{{AFX_MSG_MAP(CNanoPifSockWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SERVER_ACCEPT, OnConnected)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnDisConnected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockWnd message handlers

//---------------------------------------------------------------------------
// OnCreate
int CNanoPifSockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Read inifile
	int iRunAs = AfxGetApp()->GetProfileInt(INISECTION_WINSOCK, INIKEY_RUNAS, 1);
	int iAutoRun = AfxGetApp()->GetProfileInt(INISECTION_WINSOCK, INIKEY_AUTORUN, 1);
	switch ( iRunAs ) {
	case 0:
		break;
	case 1:	// Server
		if ( iAutoRun ) {
			m_pLstn = new CWinSockLstn<CPifSockSocket>(this, AfxGetApp()->m_pszExeName);
			if ( m_pLstn ) {
				if ( !m_pLstn->Open(_ttoi(AfxGetApp()->GetProfileString(INISECTION_WINSOCK, INIKEY_PORTNUM, PORTNUM)), AfxGetApp()->GetProfileString(INISECTION_WINSOCK, INIKEY_SOCKETADDRESS, SOCKETADDRESS)) ) {
					delete m_pLstn;
					m_pLstn = NULL;
				}
			}
		}
		break;
	case 2:	// Client
		if ( iAutoRun ) {
			m_pSock = new CPifSockSocket(this, AfxGetApp()->m_pszExeName);
			if ( !m_pSock->Create(0, SOCK_STREAM, FD_CONNECT | FD_CLOSE, AfxGetApp()->GetProfileString(INISECTION_WINSOCK, INIKEY_SOCKETADDRESS, SOCKETADDRESS)) ) {
				TRACE(_T("Create Error occured : %d\n"), m_pSock->GetLastError());
				delete m_pSock;
				m_pSock = NULL;
				return 0;
			}

			if ( !m_pSock->Connect(AfxGetApp()->GetProfileString(INISECTION_WINSOCK, INIKEY_HOSTADDRESS, HOSTADDRESS), _ttoi(AfxGetApp()->GetProfileString(INISECTION_WINSOCK, INIKEY_PORTNUM, PORTNUM))) ) {
				if ( m_pSock->GetLastError() != WSAEWOULDBLOCK ) {
					TRACE(_T("Connect Error occured : %d\n"), m_pSock->GetLastError());
					delete m_pSock;
					m_pSock = NULL;
				}
			}
		}
		break;
	}

	return 0;
}

//---------------------------------------------------------------------------
// OnClose
void CNanoPifSockWnd::OnClose()
{
	TRACE(_T("CNanoPifSockWnd::OnClose()\n"));

	CPifMessage104 pifMsg104;
	pifMsg104.SendRequ();
	::WaitForSingleObject(g_hEventShutDown, 2000);

	if ( m_pSock ) {
		m_pSock->ShutDown();
		m_pSock->Close();
		delete m_pSock;
		m_pSock = NULL;
	}

	if ( m_pEventThread ) {
		m_pEventThread->ShutDown();
		::WaitForSingleObject(m_pEventThread->m_hThread, INFINITE);
		delete m_pEventThread;
		m_pEventThread = NULL;
	}

	if ( m_pLstn ) {
		m_pLstn->ShutDown();
		m_pLstn->Close();
		delete m_pLstn;
		m_pLstn = NULL;
	}

	CWnd::OnClose();
}

//---------------------------------------------------------------------------
// PostNcDestroy
void CNanoPifSockWnd::PostNcDestroy()
{
	delete this;
}

//---------------------------------------------------------------------------
// OnConnected
LRESULT CNanoPifSockWnd::OnConnected(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CNanoPifSockWnd::OnConnected(WPARAM wParam, LPARAM lParam)\n"));

	m_pEventThread = (CEventThread*)AfxBeginThread(RUNTIME_CLASS(CEventThread));
	m_pSock = (CWinSockSocket*)wParam;
	m_pSock->Execute();

	return 0L;
}

//---------------------------------------------------------------------------
// OnDisConnected
LRESULT CNanoPifSockWnd::OnDisConnected(WPARAM wParam, LPARAM lParam)
{
	TRACE(_T("CNanoPifSockWnd::OnDisConnected(WPARAM wParam, LPARAM lParam)\n"));

	if ( m_pSock ) {
		m_pSock->ShutDown();
		m_pSock->Close();
		delete m_pSock;
		m_pSock = NULL;
	}

	if ( m_pEventThread ) {
		m_pEventThread->ShutDown();
		::WaitForSingleObject(m_pEventThread->m_hThread, INFINITE);
		delete m_pEventThread;
		m_pEventThread = NULL;
	}

//	if ( m_pLstn ) {
//		m_pLstn->ShutDown();
//		m_pLstn->Close();
//		delete m_pLstn;
//		m_pLstn = NULL;
//	}

	return 0L;
}
