// SeHeadSock.cpp : Defines the SeHeadSock routines.
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/MeaSys.hxx"
#include "resource.h"
#include "System.h"
#include "LogFile.h"
#include "Mojiretsu.h"
#include "SeHead.h"

#define WVASE_PORT_NO		(6520)
#define WVASE_IP			_T("127.0.0.1")
#define WVASE_PATH			_T("C:\\WVASE32\\Wvase32.exe /TCPPORT=6520")
#define WVASE_TITLE			_T("V.A.S.E. for Windows, Port #6520")

#define WVASE_HIDE			_T("Window(Main,Hide)")
#define WVASE_MAX			_T("Window(Main,Maximize)")
#define WVASE_GETVER		_T("getversion")
#define WVASE_HWSTAT		_T("hardstatus")

#define WVASE_EXIT			_T("Exit()")

#define WVASE_INIT			_T("HardInit()")
#define WVASE_SHUTTER_OPEN	_T("MoveShutter(All=1)")
#define WVASE_SHUTTER_CLOSE	_T("MoveShutter(All=0)")
#define WVASE_POL			_T("MOVEPOL(%d)")
#define WVASE_SCAN			_T("Dynoscan(revs=%d,pol=%d,single)")
#define WVASE_HIGHSCAN		_T("Dynoscan(highaccuracy,revs=%d,pol=%d,single)")

#define WVASE_EXPSAVE		_T("Expsave(%s)")

#define BUFFER_LEN			(1024)

extern HWND g_hNotifyWnd;
extern CLogFile* pLogFile;
extern CMojiretsu* pMojiretsu;

static char g_szMsgBoxCaption[256];

//---------------------------------------------------------------------------
// InitInstance
BOOL CSeHeadSock::InitInstance()
{
	TRACE(_T("CSeHeadSock::InitInstance()\n"));

	CTimer timer;

	strcpy(g_szMsgBoxCaption, "NanoSpec");
	if(g_lAppNameType != APP_NAME_NANO){
		strncpy(g_szMsgBoxCaption, g_lpszAppPrefix4[g_lAppNameType], 4);
	}

	// SE WVASE32 起動
	if ( ::FindWindow(NULL, WVASE_TITLE) == NULL )	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		::ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		si.wShowWindow = SW_MINIMIZE;
		::ZeroMemory( &pi, sizeof(pi) );

		// Start the child process.
		if ( !::CreateProcess(NULL, WVASE_PATH, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) ) {
			MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING31), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		// Close process and thread handles.
		::CloseHandle( pi.hProcess );
		::CloseHandle( pi.hThread );


		// SE WVASE32が正常起動したかチェック
		timer.Restart(10);
		while ( ::FindWindow(NULL, WVASE_TITLE) == NULL )	{
			if ( timer.IsTimeout() ) {
				// WVASE32応答タイムアウトエラー
				MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING32), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
				return FALSE;
			}
			::Sleep(10);
		}
	}

	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD( 2, 2 );

	m_iWSAErr = WSAStartup( wVersionRequested, &wsaData );
	if ( m_iWSAErr != 0 ) {
		return FALSE;
	}

	if ( !Connect() ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING33), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	if ( !InitWVase() ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING34), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	if ( !CloseShutter() )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CSeHeadSock::ExitInstance()
{
	TRACE(_T("CSeHeadSock::ExitInstance()\n"));

	TCHAR szRecvBuff[BUFFER_LEN];

	if ( m_sock != INVALID_SOCKET ) {
		CloseShutter();						// 戻り値は、見ない

		if ( !SendCommand(WVASE_HIDE) )
			goto Finally;
		if ( !RecvData(szRecvBuff) )
			goto Finally;
		if ( !CheckCmdAndStatusCode(WVASE_HIDE, szRecvBuff) )
			goto Finally;

		if ( !SendCommand(WVASE_EXIT) )
			goto Finally;
		if ( !RecvData(szRecvBuff) )
			goto Finally;
		if ( !CheckCmdAndStatusCode(WVASE_EXIT, szRecvBuff) )
			goto Finally;
	}

Finally:
	// 通信でシャットダウンできない場合の考慮
	HWND hWndWvase;
	if ( (hWndWvase = ::FindWindow(NULL, WVASE_TITLE)) != NULL ) {
		::PostMessage(hWndWvase, WM_SYSCOMMAND, SC_CLOSE, 0L);
	}

	CloseSocket();

	if ( m_iWSAErr == 0 )
		WSACleanup();
}

//---------------------------------------------------------------------------
// GetVersion
BOOL CSeHeadSock::GetVersion(LPTSTR pszVersion)
{
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !SendCommand(WVASE_GETVER) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
//	if ( !CheckCmdAndStatusCode(WVASE_GETVER, szRecvBuff) )		// 不要
//		return FALSE;

	strcpy(pszVersion, szRecvBuff);

	return TRUE;
}

//---------------------------------------------------------------------------
// Scan
BOOL CSeHeadSock::Scan(LPCTSTR pszSaveFilePath, int iNumScans /*=50*/, BOOL bHighAccuracy /*=TRUE*/)
{
	TCHAR szSendBuff[BUFFER_LEN];
	TCHAR szRecvBuff[BUFFER_LEN];

//	const int iDefPolPos = -45;
//	const int iDefPolPos = 45;

	if ( bHighAccuracy ) {
//		if ( !MovePol(iDefPolPos) )
//			return FALSE;
//		::Sleep(100);
//		sprintf(szSendBuff, WVASE_HIGHSCAN, iNumScans, -45);
		sprintf(szSendBuff, WVASE_HIGHSCAN, iNumScans, 45);
	}
	else {
		sprintf(szSendBuff, WVASE_SCAN, iNumScans, 45);
	}
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	sprintf(szSendBuff, WVASE_EXPSAVE, pszSaveFilePath);
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	if ( g_hNotifyWnd )
		::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);

	return TRUE;
}

//---------------------------------------------------------------------------
// OpenShutter
BOOL CSeHeadSock::OpenShutter()
{
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !SendCommand(WVASE_SHUTTER_OPEN) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(WVASE_SHUTTER_OPEN, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// CloseShutter
BOOL CSeHeadSock::CloseShutter()
{
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !SendCommand(WVASE_SHUTTER_CLOSE) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(WVASE_SHUTTER_CLOSE, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// MovePol
BOOL CSeHeadSock::MovePol(int iPolPos)
{
	TCHAR szSendBuff[BUFFER_LEN];
	TCHAR szRecvBuff[BUFFER_LEN];

	sprintf(szSendBuff, WVASE_POL, iPolPos);
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// Connect
BOOL CSeHeadSock::Connect()
{
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	sockaddr_in s_out;
	memset(&s_out,0,sizeof(s_out));

	s_out.sin_family = AF_INET;
	s_out.sin_port = htons(WVASE_PORT_NO);
	s_out.sin_addr.S_un.S_addr = inet_addr(WVASE_IP);

	if (s_out.sin_addr.S_un.S_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(WVASE_IP);
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
void CSeHeadSock::CloseSocket()
{
	TCHAR szRecvBuff[BUFFER_LEN];
	int iRet;

	if ( m_sock == INVALID_SOCKET )
		return;

	// Call shutdown with how=SD_SEND.
	shutdown(m_sock, SD_SEND);
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
	};
	// Call closesocket.
	closesocket(m_sock);
}

//---------------------------------------------------------------------------
// InitWVase
BOOL CSeHeadSock::InitWVase()
{
	TCHAR szRecvBuff[BUFFER_LEN];

	if ( !SendCommand(WVASE_HIDE) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(WVASE_HIDE, szRecvBuff) )
		return FALSE;

	if ( !SendCommand(WVASE_INIT) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(WVASE_INIT, szRecvBuff) )
		return FALSE;

	if ( !SendCommand(WVASE_HWSTAT) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
//	if ( !CheckCmdAndStatusCode(WVASE_HWSTAT, szRecvBuff) )		// 不要
//		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// SendCommand
BOOL CSeHeadSock::SendCommand(LPCTSTR pszCmd)
{
	const char ch = 0x1A;

	BOOL bRet = FALSE;
	CString strSendBuffer;
	int iRet;
	int iSend;

	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	iSend = 0;
	strSendBuffer = pszCmd;
	strSendBuffer += ch;
	while ( 1 ) {
		iRet = send(m_sock, (LPCTSTR)strSendBuffer + iSend, strSendBuffer.GetLength() - iSend, 0);
		if ( iRet == 0 ) {
			pLogFile->Logging("CSeHeadSock Send closed.");
			break;
		}
		else if ( iRet < 0 ) {
			// SOCKET_ERROR
			pLogFile->LoggingV("CSeHeadSock Send SOCKET_ERROR : %d", WSAGetLastError());
			break;
		}
		else {
			iSend += iRet;
			if ( iSend == strSendBuffer.GetLength() ) {
				bRet = TRUE;
				break;
			}
			else if ( iSend > strSendBuffer.GetLength() ) {
				pLogFile->Logging("CSeHeadSock Send BYTE Count overflow error.");
				break;
			}
		}
	}

	pLogFile->Logging(pszCmd);

	return bRet;
}

//---------------------------------------------------------------------------
// RecvData
BOOL CSeHeadSock::RecvData(LPTSTR pszData)
{
	const char ch = 0x1A;

	BOOL bRet = FALSE;
	TCHAR szRecvBuff[BUFFER_LEN];
	int iRet;
	int iRecv;

	if ( m_sock == INVALID_SOCKET )
		return FALSE;

	iRecv = 0;
	while ( 1 ) {
		iRet = recv(m_sock, szRecvBuff + iRecv, BUFFER_LEN - iRecv, 0);
		if ( iRet == 0 ) {
			pLogFile->Logging("CSeHeadSock Recv closed.");
			break;
		}
		else if ( iRet < 0 ) {
			// SOCKET_ERROR
			pLogFile->LoggingV("CSeHeadSock SOCKET_ERROR : %d", WSAGetLastError());
			break;
		}
		else {
			iRecv += iRet;
			if ( szRecvBuff[iRecv - 1] == ch ) {
				szRecvBuff[iRecv - 1] = '\0';
				bRet = TRUE;
				break;
			}
			if ( iRecv >= BUFFER_LEN ) {
				pLogFile->Logging("CSeHeadSock Recv buffer overflow error.");
				szRecvBuff[BUFFER_LEN - 1] = '\0';
				break;
			}
		}
	}

	pLogFile->Logging(szRecvBuff);

	strcpy(pszData, szRecvBuff);

	return bRet;
}

//---------------------------------------------------------------------------
// CheckCmdAndStatusCode
BOOL CSeHeadSock::CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData)
{
	LPCTSTR WVASE_RESPONSE_OK = "OK";
	LPCTSTR WVASE_SHUTTERRESPONSE = "Shutter";

	LPCTSTR pszStatusCode;

	if ( strncmp(pszSendCmd, "MoveShutter", strlen("MoveShutter")) == 0 )
		pszStatusCode = WVASE_SHUTTERRESPONSE;
	else
		pszStatusCode = WVASE_RESPONSE_OK;

	return ( strncmp(pszRecvData, pszStatusCode, strlen(pszStatusCode)) == 0 );
}
