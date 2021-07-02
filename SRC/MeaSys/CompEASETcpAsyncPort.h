#pragma once

#include "Winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

// ===========================================================================
// CCompEASETcpAsyncPort class
// ===========================================================================
class CCompEASETcpAsyncPort
{
public:
	CCompEASETcpAsyncPort();
	virtual ~CCompEASETcpAsyncPort();

	virtual BOOL InitInstance();
	virtual void ExitInstance();

	virtual BOOL RecvAsyncData(LPTSTR pszData);

	virtual void MyMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
	{
		_tcscpy(m_szErrorMessage, lpText);
		::MessageBox(hWnd, lpText, lpCaption, uType);
	}
	virtual LPCTSTR GetLastError() {return m_szErrorMessage;}

private:
	BOOL Connect();
	void CloseSocket(BOOL bReset = FALSE);
	BOOL ResetConnection();

private:
	TCHAR m_szErrorMessage[256];

	CString m_strIPAddress;
	WORD m_nPortNo;

	int m_iWSAErr;
	SOCKET m_sock;
	BOOL m_bConnect;
};
