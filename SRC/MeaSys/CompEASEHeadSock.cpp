// CompEASEHeadSock.cpp : Defines the CompEASEHeadSock routines.
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/MeaSys.hxx"
#include "resource.h"
#include "LogFile.h"
#include "Mojiretsu.h"
#include "CompEASEHead.h"

#define EASE_PORT_NO		(4444)
#define EASE_IP				_T("127.0.0.1")

#define EASE_READY			_T("Ready()")
#define EASE_STATUS			_T("Status()")
#define EASE_SHUTTER_OPEN	_T("SystemLampShutterOpen()")
#define EASE_SHUTTER_CLOSE	_T("SystemLampShutterClosed()")
#define EASE_RUNRECIPE		_T("RunRecipe(%s)")
#define EASE_LISTRECIPES	_T("ListRecipes()")

#define EASE_RECV_STATUS_WAITING_TO_ACQUIRE_DATA		"Waiting to Acquire Data"  // (The ellipsometer is ready and waiting for commands.)
#define EASE_RECV_STATUS_HARDWARE_BUSY					"Hardware Busy..."		   // (The ellispometer is performing a hardware operation. The ellipsometer is not ready for commands.)
#define EASE_RECV_STATUS_ACQUIRING_DATA					"Acquiring Data"		   // (The ellipsometer is acquiring data. This is for insitu acquisition only.)
#define EASE_RECV_STATUS_INITIALIZING_HARDWARE			"Initializing Hardware"    // (The hardware is being initialized. The ellipsometer is not ready for commands.)
#define EASE_RECV_STATUS_ALIGINING_HARDWARE				"Aligning Hardware"		   // (The hardware is in alignment mode. The ellipsometer is not ready for commands.)
#define EASE_RECV_STATUS_CALCULATING_DC_OFFSET			"Calculating DC Offset"    // (The hardware is calculating the DC offset. The ellipsometer is not ready for commands.)
#define EASE_RECV_STATUS_CALIBRATING_HARDWARE			"Calibrating Hardware"	   // (The hardware is calibrating. The ellipsometer is not ready for commands.)
#define EASE_RECV_STATUS_PERFORMING_SYSTEM_CHECK		"Performing System Check"  // (The hardware is performing a System Check. The ellipsometer is not ready for commands.)
#define EASE_RECV_STATUS_DISPLAYING_RAW_SIGNAL			"Displaying Raw Signal"	   // (The hardware is displaying intensity information. The ellipsometer is not ready for commands)
#define EASE_RECV_STATUS_HARDWARE_NOT_INITIALIZED		"Hardware Not Initialized" // (The hardware is not initialized. The ellipsometer is not ready for commands and needs attention to discover why it is not initialized. )
#define EASE_RECV_STATUS_HARDWARE_NOT_CALIBRATED		"Hardware Not Calibrated"  // (The hardware is not calibrated. This only occurs if the configuration files are not present.)

#define EASE_RECV_RUN_RECIPE_ERROR_NOT_FOUND			"Error:  Recipe File Not Found"

#define BUFFER_LEN			(1024 * 4)

#define WAIT_STATUS_CHANGE_TIMEOUT_SEC		(5)
#define RECV_GENERAL_COMMAND_TIMEOUT_SEC	(5)
#define RESET_CONNECTION_RETRY_MAX			(3)
#define RESET_CONNECTION_WAIT_TIME			(100)

extern CLogFile* pLogFile;
extern CMojiretsu* pMojiretsu;
extern HWND g_hNotifyWnd;

//---------------------------------------------------------------------------
// Constructor
CCompEASEHeadSock::CCompEASEHeadSock()
	 : m_iWSAErr(WSASYSNOTREADY), m_sock(INVALID_SOCKET), m_bConnect(FALSE),
	   m_iGeneralCommandTimeout(RECV_GENERAL_COMMAND_TIMEOUT_SEC), m_iAnalyzeTimeout(120),
	   m_strIPAddress(EASE_IP), m_nPortNo(EASE_PORT_NO)
{
}

//---------------------------------------------------------------------------
// Destructor
CCompEASEHeadSock::~CCompEASEHeadSock()
{
}

//---------------------------------------------------------------------------
// InitInstance
BOOL CCompEASEHeadSock::InitInstance()
{
	TRACE(_T("CCompEASEHeadSock::InitInstance()\n"));

	////// Load File /////
	COMPEASE_CONFIG compEaseConfig;

	ConfigFile_GetNanoSpecIni(&compEaseConfig, CONFIG_FILE_COMPEASE_CONFIG);
	m_iAnalyzeTimeout = compEaseConfig.nAnalyzeTimeout;
	m_strIPAddress = compEaseConfig.szIpAddress;
	m_nPortNo = compEaseConfig.nPort;

	////// Connect /////
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD( 2, 2 );

	m_iWSAErr = WSAStartup( wVersionRequested, &wsaData );
	if ( m_iWSAErr != 0 ) {
		return FALSE;
	}

	if ( !Connect() ) {
		pLogFile->LoggingV("CCompEASEHeadSoc Connect Error : %d", WSAGetLastError());
//		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING41), "NanoSpec", MB_OK | MB_ICONSTOP);
		if ( g_hNotifyWnd ) {
			::PostMessage(g_hNotifyWnd, WM_MEAS_COMPEASE_STATUS, EASE_STATUS_COMM_ESTABLISH, 0L);
		}
		return FALSE;
	}

	m_bConnect = TRUE;

	if ( !InitEASE() ) {
//		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING42), "NanoSpec", MB_OK | MB_ICONSTOP);
		if ( g_hNotifyWnd ) {
			::PostMessage(g_hNotifyWnd, WM_MEAS_COMPEASE_STATUS, EASE_STATUS_COMM_ERROR, 0L);
		}
		return FALSE;
	}

	if ( !CloseShutter() ) {
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CCompEASEHeadSock::ExitInstance()
{
	TRACE(_T("CCompEASEHeadSock::ExitInstance()\n"));

	if ( m_bConnect ) {
		CloseShutter();						// 戻り値は、見ない
	}

	CloseSocket();

	m_bConnect = FALSE;

	if ( m_iWSAErr == 0 )
		WSACleanup();
}

//---------------------------------------------------------------------------
// ResetConnection
BOOL CCompEASEHeadSock::ResetConnection()
{
	CloseSocket(TRUE);

	m_bConnect = FALSE;

	if ( !Connect() ) {
		pLogFile->LoggingV("CCompEASEHeadSoc Connect Error : %d", WSAGetLastError());
		return FALSE;
	}

	m_bConnect = TRUE;

	return TRUE;
}

//---------------------------------------------------------------------------
// Ready
BOOL CCompEASEHeadSock::Ready()
{
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !SendCommand(EASE_READY) )
		return FALSE;
	if ( !RecvData(szRecvBuff, m_iGeneralCommandTimeout) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(EASE_READY, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// Status
int CCompEASEHeadSock::Status()
{
	int iRet = EASE_STATUS_UNKNOWN;
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !SendCommand(EASE_STATUS, FALSE) ) {
		pLogFile->Logging("CCompEASEHeadSock::Status() SendCommand Error");
		return iRet;
	}
	if ( !RecvData(szRecvBuff, 1) ) {
		pLogFile->Logging("CCompEASEHeadSock::Status() RecvData Error");
		return iRet;
	}

	if ( strcmp(szRecvBuff, EASE_RECV_STATUS_WAITING_TO_ACQUIRE_DATA) == 0 ) {
		iRet = EASE_STATUS_WAIT_ACQUIRE;
	}
	else {
		// errors.
		if ( strcmp(szRecvBuff, EASE_RECV_STATUS_HARDWARE_BUSY) == 0 ) {
			iRet = EASE_STATUS_HARDWARE_BUSY;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_ACQUIRING_DATA) == 0 ) {
			iRet = EASE_STATUS_ACQUIRING;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_INITIALIZING_HARDWARE) == 0 ) {
			iRet = EASE_STATUS_INIT_HARDWARE;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_ALIGINING_HARDWARE) == 0 ) {
			iRet = EASE_STATUS_ALIGN_HARDWARE;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_CALCULATING_DC_OFFSET) == 0 ) {
			iRet = EASE_STATUS_CALC_DC_OFFSET;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_CALIBRATING_HARDWARE) == 0 ) {
			iRet = EASE_STATUS_CALIB_HARDWARE;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_PERFORMING_SYSTEM_CHECK) == 0 ) {
			iRet = EASE_STATUS_PERFORM_SYSTEM_CHECK;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_DISPLAYING_RAW_SIGNAL) == 0 ) {
			iRet = EASE_STATUS_DISPLAY_RAW_SIGNAL;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_HARDWARE_NOT_INITIALIZED) == 0 ) {
			iRet = EASE_STATUS_HARDWARE_NOT_INIT;
		}
		else if ( strcmp(szRecvBuff, EASE_RECV_STATUS_HARDWARE_NOT_CALIBRATED) == 0 ) {
			iRet = EASE_STATUS_HARDWARE_NOT_CALIB;
		}
		else {
			iRet = EASE_STATUS_UNKNOWN;
		}
	}

	return iRet;
}

//---------------------------------------------------------------------------
// OpenShutter
BOOL CCompEASEHeadSock::OpenShutter()
{
	pLogFile->Logging("CCompEASEHeadSock::OpenShutter()");

// Need Comment Out for sim ease
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !CheckWaitStatusForEASE(WAIT_STATUS_CHANGE_TIMEOUT_SEC) )
		return FALSE;
	if ( !SendCommand(EASE_SHUTTER_OPEN) )
		return FALSE;
	if ( !RecvData(szRecvBuff, m_iGeneralCommandTimeout) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(EASE_SHUTTER_OPEN, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// CloseShutter
BOOL CCompEASEHeadSock::CloseShutter()
{
	pLogFile->Logging("CCompEASEHeadSock::CloseShutter()");

// Need Comment Out for sim ease
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !CheckWaitStatusForEASE(WAIT_STATUS_CHANGE_TIMEOUT_SEC) )
		return FALSE;
	if ( !SendCommand(EASE_SHUTTER_CLOSE) )
		return FALSE;
	if ( !RecvData(szRecvBuff, m_iGeneralCommandTimeout) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(EASE_SHUTTER_CLOSE, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// ListRecipes
BOOL CCompEASEHeadSock::ListRecipes(LPTSTR pszListRecipes)
{
	if ( pszListRecipes == NULL ) {
		return FALSE;
	}

	if ( !CheckWaitStatusForEASE(WAIT_STATUS_CHANGE_TIMEOUT_SEC) )
		return FALSE;
	if ( !SendCommand(EASE_LISTRECIPES) )
		return FALSE;
	if ( !RecvData(pszListRecipes, m_iGeneralCommandTimeout) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// RunRecipe
BOOL CCompEASEHeadSock::RunRecipe(LPCTSTR pszRecipeId, LPTSTR pszMeasResult)
{
	TCHAR szSendBuff[BUFFER_LEN];
	TCHAR szRecvBuff[BUFFER_LEN];

	sprintf(szSendBuff, EASE_RUNRECIPE, pszRecipeId);

	if ( !CheckWaitStatusForEASE(WAIT_STATUS_CHANGE_TIMEOUT_SEC) )
		return FALSE;
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff, m_iAnalyzeTimeout) )
		return FALSE;

	_tcscpy(pszMeasResult, szRecvBuff);

	// Check Recipe File exist.
	if ( _tcscmp(szRecvBuff, EASE_RECV_RUN_RECIPE_ERROR_NOT_FOUND) == 0 ) {
		::PostMessage(g_hNotifyWnd, WM_MEAS_COMPEASE_ERROR, EASE_ERROR_RUN_RECIPE_NOT_FOUND, 0L);
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Connect
BOOL CCompEASEHeadSock::Connect()
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
void CCompEASEHeadSock::CloseSocket(BOOL bReset/*=FALSE*/)
{
	TCHAR szRecvBuff[BUFFER_LEN];
	int iRet;

	if ( m_sock == INVALID_SOCKET )
		return;

	// Call shutdown with how=SD_SEND.
	shutdown(m_sock, SD_SEND);

	if ( !bReset ) {
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
// InitEASE
BOOL CCompEASEHeadSock::InitEASE()
{
	return Ready();
}

//---------------------------------------------------------------------------
// SendCommand
BOOL CCompEASEHeadSock::SendCommand(LPCTSTR pszCmd, BOOL bRetry/*= TRUE*/)
{
	BOOL bRet = FALSE;
	CString strSendBuffer;
	int iRet;
	int iSend;
	int iRetry;

	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	iSend = 0;
	strSendBuffer.Format("%s\r\n", pszCmd);

	pLogFile->LoggingV("CCompEASEHeadSock Send : %s", pszCmd);

	iRetry = 0;
	while ( 1 ) {
		if( iRetry >= RESET_CONNECTION_RETRY_MAX ) {
			pLogFile->Logging("CCompEASEHeadSock Send Retry Connection Count Over.");
			break;
		}
		iRet = send(m_sock, (LPCTSTR)strSendBuffer + iSend, strSendBuffer.GetLength() - iSend, 0);
		if ( iRet == 0 ) {
			pLogFile->Logging("CCompEASEHeadSock Send closed.");
//			break;
			Sleep(RESET_CONNECTION_WAIT_TIME);
			ResetConnection();
			if ( bRetry ) {
				iSend = 0;
				iRetry++;
			}
			else {
				break;
			}
		}
		else if ( iRet < 0 ) {
			// SOCKET_ERROR
			pLogFile->LoggingV("CCompEASEHeadSock Send SOCKET_ERROR : %d", WSAGetLastError());
//			break;
			Sleep(RESET_CONNECTION_WAIT_TIME);
			ResetConnection();
			if ( bRetry ) {
				iSend = 0;
				iRetry++;
			}
			else {
				break;
			}
		}
		else {
			iSend += iRet;
			if ( iSend == strSendBuffer.GetLength() ) {
				bRet = TRUE;
				break;
			}
			else if ( iSend > strSendBuffer.GetLength() ) {
				pLogFile->Logging("CCompEASEHeadSock Send BYTE Count overflow error.");
				break;
			}
		}
	}

	return bRet;
}

//---------------------------------------------------------------------------
// CheckWaitStatusForEASE
BOOL CCompEASEHeadSock::CheckWaitStatusForEASE(int iTimeoutSec)
{
	int iStatus;
	CTimer timer;

	timer.Restart(iTimeoutSec);
	while ( 1 ) {
		if ( timer.IsTimeout() ) {
			return FALSE;
		}
		iStatus = Status();
		switch ( iStatus ) {
		case EASE_STATUS_WAIT_ACQUIRE:
			return TRUE;
			break;
		default:
			break;
		}

		::Sleep(10);
	}

	return FALSE;
}

//---------------------------------------------------------------------------
// RecvData
BOOL CCompEASEHeadSock::RecvData(LPTSTR pszData, int iTimeoutSec)
{
	BOOL bRet = FALSE;
	TCHAR szRecvBuff[BUFFER_LEN];
	int iRet;
	int iRecv;
	int iRetry;
	CTimer timer;

	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	fd_set fdset, readfds;
	struct timeval timeout;
	FD_ZERO( &readfds );
	FD_SET(m_sock, &readfds);

	// timeout0秒。つまりselectはすぐに制御を返してくる
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	iRecv = 0;
	iRetry = 0;
	szRecvBuff[0] = '\0';

	timer.Restart(iTimeoutSec);

	while ( 1 ) {
		// 読み込み用fd_setの初期化
		// selectが毎回内容を上書きしてしまうので、毎回初期化します
		memcpy(&fdset, &readfds, sizeof(fd_set));

		select( m_sock + 1 , &fdset , NULL , NULL , &timeout );

		if ( iTimeoutSec > 0 ) {
			if ( timer.IsTimeout() ) {
				pLogFile->Logging("CCompEASEHeadSock Timeout.");
				if ( g_hNotifyWnd ) {
					::PostMessage(g_hNotifyWnd, WM_MEAS_COMPEASE_STATUS, EASE_STATUS_EXECUTE_CMD_TIMEOUT, 0L);
				}
				Sleep(RESET_CONNECTION_WAIT_TIME);
				ResetConnection();
				break;
			}
		}

		if (FD_ISSET(m_sock, &fdset)) {
			iRet = recv(m_sock, szRecvBuff + iRecv, BUFFER_LEN - iRecv, 0);
			if ( iRet == 0 ) {
				pLogFile->Logging("CCompEASEHeadSock Recv closed.");
				Sleep(RESET_CONNECTION_WAIT_TIME);
				ResetConnection();
				break;
			}
			else if ( iRet < 0 ) {
				// SOCKET_ERROR
				pLogFile->LoggingV("CCompEASEHeadSock Recv SOCKET_ERROR : %d", WSAGetLastError());
				if ( g_hNotifyWnd ) {
					::PostMessage(g_hNotifyWnd, WM_MEAS_COMPEASE_STATUS, EASE_STATUS_COMM_ERROR, 0L);
				}
				Sleep(RESET_CONNECTION_WAIT_TIME);
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
					break;
				}
				if ( iRecv >= BUFFER_LEN ) {
					szRecvBuff[BUFFER_LEN - 1] = '\0';
					break;
				}
			}
		}
		if ( iTimeoutSec == 0 ) {
			Sleep(RESET_CONNECTION_WAIT_TIME);
			ResetConnection();
			break;
		}
	}

	strcpy(pszData, szRecvBuff);

	pLogFile->LoggingV("CCompEASEHeadSock Recv : %s", pszData);

	return bRet;
}

//---------------------------------------------------------------------------
// CheckCmdAndStatusCode
BOOL CCompEASEHeadSock::CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData)
{
	LPCTSTR EASE_RESPONSE_OK = "OK";

	LPCTSTR pszStatusCode;

	pszStatusCode = EASE_RESPONSE_OK;

	return ( strncmp(pszRecvData, pszStatusCode, strlen(pszStatusCode)) == 0 );
}
