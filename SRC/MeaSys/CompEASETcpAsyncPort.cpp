// CompEASETcpAsyncPort.cpp
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/MeaSys.hxx"
#include "resource.h"
#include "System.h"
#include "LogFile.h"
#include "Mojiretsu.h"
#include "CompEASETcpAsyncPort.h"

#define EASE_PORT_NO		(4445)
#define EASE_IP				_T("127.0.0.1")

#define BUFFER_LEN			(1024 * 4)

extern HWND g_hNotifyWnd;
extern CLogFile* pLogFile;
extern CMojiretsu* pMojiretsu;

static char g_szMsgBoxCaption[256];

//---------------------------------------------------------------------------
// Constructor
CCompEASETcpAsyncPort::CCompEASETcpAsyncPort()
	 : m_iWSAErr(WSASYSNOTREADY), m_sock(INVALID_SOCKET), m_bConnect(FALSE),
	   m_strIPAddress(EASE_IP), m_nPortNo(EASE_PORT_NO)
{
	::ZeroMemory(m_szErrorMessage, sizeof(m_szErrorMessage));
}

//---------------------------------------------------------------------------
// Destructor
CCompEASETcpAsyncPort::~CCompEASETcpAsyncPort()
{
}

//---------------------------------------------------------------------------
// InitInstance
BOOL CCompEASETcpAsyncPort::InitInstance()
{
	TRACE(_T("CCompEASETcpAsyncPort::InitInstance()\n"));

	strcpy(g_szMsgBoxCaption, "NanoSpec");
	if(g_lAppNameType != APP_NAME_NANO){
		strncpy(g_szMsgBoxCaption, g_lpszAppPrefix4[g_lAppNameType], 4);
	}

	////// Load File /////
	COMPEASE_CONFIG compEaseConfig;

	ConfigFile_GetNanoSpecIni(&compEaseConfig, CONFIG_FILE_COMPEASE_CONFIG);
	m_strIPAddress = compEaseConfig.szIpAddress;
	m_nPortNo = compEaseConfig.nAsyncPort;

	////// Connect /////
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD( 2, 2 );

	m_iWSAErr = WSAStartup( wVersionRequested, &wsaData );
	if ( m_iWSAErr != 0 ) {
		return FALSE;
	}

	if ( !Connect() ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING51), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	m_bConnect = TRUE;

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CCompEASETcpAsyncPort::ExitInstance()
{
	TRACE(_T("CCompEASETcpAsyncPort::ExitInstance()\n"));

	CloseSocket();

	m_bConnect = FALSE;

	if ( m_iWSAErr == 0 )
		WSACleanup();
}

//---------------------------------------------------------------------------
// ResetConnection
BOOL CCompEASETcpAsyncPort::ResetConnection()
{
	CloseSocket(TRUE);

	m_bConnect = FALSE;

	if ( !Connect() ) {
		pLogFile->LoggingV("CCompEASETcpAsyncPort Connect Error : %d", WSAGetLastError());
		return FALSE;
	}

	m_bConnect = TRUE;

	return TRUE;
}

//---------------------------------------------------------------------------
// Connect
BOOL CCompEASETcpAsyncPort::Connect()
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	sockaddr_in s_out;
	memset(&s_out,0,sizeof(s_out));

	s_out.sin_family = AF_INET;
	s_out.sin_port = htons(m_nPortNo);
	s_out.sin_addr.S_un.S_addr = inet_addr(m_strIPAddress);

	if (s_out.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(m_strIPAddress);
		if (lphost != NULL)
			s_out.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	return (connect(m_sock, (SOCKADDR*)&s_out, sizeof(SOCKADDR)) == 0);
}

//---------------------------------------------------------------------------
// CloseSocket
void CCompEASETcpAsyncPort::CloseSocket(BOOL bReset/*=FALSE*/)
{
	TCHAR szRecvBuff[BUFFER_LEN];
	int iRet;

	if ( m_sock == INVALID_SOCKET )
		return;

	// Call shutdown with how=SD_SEND.
	shutdown(m_sock, SD_SEND);

	fd_set fdset, readfds;
	struct timeval timeout;
	FD_ZERO( &readfds );
	FD_SET(m_sock, &readfds);

	// timeout0秒。つまりselectはすぐに制御を返してくる
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	// 読み込み用fd_setの初期化
	// selectが毎回内容を上書きしてしまうので、毎回初期化します
	memcpy(&fdset, &readfds, sizeof(fd_set));

	select( m_sock + 1 , &fdset , NULL , NULL , &timeout );
		
	if (FD_ISSET(m_sock, &fdset)) {
		// Call recv until zero returned, or SOCKET_ERROR.
		while ( 1 ) {
			iRet = recv(m_sock, szRecvBuff, BUFFER_LEN, 0);
			if ( iRet == 0 ) {
				// gracefully closed
				break;
			}
			else if ( iRet < 0 ) {
				// SOCKET_ERROR
				break;
			}
		}
	}

	shutdown(m_sock, SD_BOTH);
	// Call closesocket.
	closesocket(m_sock);
}

//---------------------------------------------------------------------------
// RecvAsyncData
BOOL CCompEASETcpAsyncPort::RecvAsyncData(LPTSTR pszData)
{
	BOOL bRet = FALSE;
	TCHAR szRecvBuff[BUFFER_LEN];
	int iRet;
	int iRecv;

	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	fd_set fdset, fdset2, readfds, errfds;
	struct timeval timeout;
	FD_ZERO( &readfds );
	FD_ZERO( &errfds );
	FD_SET(m_sock, &readfds);
	FD_SET(m_sock, &errfds);

	// timeout0秒。つまりselectはすぐに制御を返してくる
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

   iRecv = 0;
	while ( 1 ) {
		// 読み込み用fd_setの初期化
		// selectが毎回内容を上書きしてしまうので、毎回初期化します
		memcpy(&fdset, &readfds, sizeof(fd_set));
	
		memcpy(&fdset2, &errfds, sizeof(fd_set));

		select( m_sock + 1 , &fdset , NULL , &fdset2 , &timeout );
		
		if (FD_ISSET(m_sock, &fdset)) {
			iRet = recv(m_sock, szRecvBuff + iRecv, BUFFER_LEN - iRecv, 0);
			if ( iRet == 0 ) {
				ResetConnection();
				break;
			}
			else if ( iRet < 0 ) {
				// SOCKET_ERROR
				ResetConnection();
				break;
			}
			else {
				iRecv += iRet;
				if ( szRecvBuff[iRecv - 2] == '\r' &&
					 szRecvBuff[iRecv - 1] == '\n' ) {
					szRecvBuff[iRecv - 2] = '\0';
					szRecvBuff[iRecv - 1] = '\0';
					bRet = TRUE;
					strcpy(pszData, szRecvBuff);

					pLogFile->LoggingV("CCompEASETcpAsyncPort : %s", pszData);
					break;
				}
				if ( iRecv >= BUFFER_LEN ) {
					szRecvBuff[BUFFER_LEN - 1] = '\0';
					strcpy(pszData, szRecvBuff);

					pLogFile->LoggingV("CCompEASETcpAsyncPort : %s", pszData);
					break;
				}
			}
		}
		else {
			if (FD_ISSET(m_sock, &fdset2)) {
				ResetConnection();
			}
			break;
		}
	}


	return bRet;
}

