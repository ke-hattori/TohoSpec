#pragma once

#include "Winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

// ===========================================================================
// CSeHead class
// ===========================================================================
class CSeHead
{
public:
	CSeHead() {::ZeroMemory(m_szErrorMessage, sizeof(m_szErrorMessage));}
	virtual ~CSeHead() {}

	virtual BOOL InitInstance()=0;
	virtual void ExitInstance()=0;
	virtual BOOL GetVersion(LPTSTR pszVersion)=0;
	virtual BOOL OpenShutter()=0;
	virtual BOOL CloseShutter()=0;
	virtual BOOL MovePol(int iPolPos)=0;
	virtual BOOL Scan(LPCTSTR pszSaveFilePath, int iNumScans = 50, BOOL bHighAccuracy = TRUE)=0;
	virtual void MyMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
	{
		_tcscpy(m_szErrorMessage, lpText);
		::MessageBox(hWnd, lpText, lpCaption, uType);
	}
	virtual LPCTSTR GetLastError() {return m_szErrorMessage;}

protected:
	TCHAR m_szErrorMessage[256];
};

// ===========================================================================
// CSeHeadDesktop class
// ===========================================================================
class CSeHeadDesktop : public CSeHead
{
public:
	CSeHeadDesktop() {}
	virtual ~CSeHeadDesktop() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL GetVersion(LPTSTR pszVersion);
	virtual BOOL OpenShutter();
	virtual BOOL CloseShutter();
	virtual BOOL MovePol(int iPolPos);
	virtual BOOL Scan(LPCTSTR pszSaveFilePath, int iNumScans = 50, BOOL bHighAccuracy = TRUE);
};

// ===========================================================================
// CSeHeadSock class
// ===========================================================================
class CSeHeadSock : public CSeHead
{
public:
	CSeHeadSock() : m_iWSAErr(WSASYSNOTREADY), m_sock(INVALID_SOCKET) {}
	virtual ~CSeHeadSock() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL GetVersion(LPTSTR pszVersion);
	virtual BOOL OpenShutter();
	virtual BOOL CloseShutter();
	virtual BOOL MovePol(int iPolPos);
	virtual BOOL Scan(LPCTSTR pszSaveFilePath, int iNumScans = 50, BOOL bHighAccuracy = TRUE);

private:
	BOOL Connect();
	void CloseSocket();
	BOOL InitWVase();
	BOOL SendCommand(LPCTSTR pszCmd);
	BOOL RecvData(LPTSTR pszData);
	BOOL CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData);

private:
	int m_iWSAErr;
	SOCKET m_sock;
};
