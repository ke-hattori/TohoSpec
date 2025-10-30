#pragma once

#include "Winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

// ===========================================================================
// CCompEASEHead class
// ===========================================================================
class CCompEASEHead
{
public:
	CCompEASEHead() {::ZeroMemory(m_szErrorMessage, sizeof(m_szErrorMessage));}
	virtual ~CCompEASEHead() {}

	virtual BOOL InitInstance()=0;
	virtual void ExitInstance()=0;

	virtual BOOL Ready()=0;
	virtual int Status()=0;
	virtual BOOL OpenShutter()=0;
	virtual BOOL CloseShutter()=0;
	virtual BOOL ListRecipes(LPTSTR pszListRecipes)=0;
	virtual BOOL RunRecipe(LPCTSTR pszRecipeId, LPTSTR pszMeasResult)=0;

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
// CCompEASEHeadDesktop class
// ===========================================================================
class CCompEASEHeadDesktop : public CCompEASEHead
{
public:
	CCompEASEHeadDesktop() {}
	virtual ~CCompEASEHeadDesktop() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();

	virtual BOOL Ready();
	virtual int Status();
	virtual BOOL OpenShutter();
	virtual BOOL CloseShutter();
	virtual BOOL ListRecipes(LPTSTR pszListRecipes);
	virtual BOOL RunRecipe(LPCTSTR pszRecipeId, LPTSTR pszMeasResult);
};

// ===========================================================================
// CCompEASEHeadSock class
// ===========================================================================
class CCompEASEHeadSock : public CCompEASEHead
{
public:
	CCompEASEHeadSock();
	virtual ~CCompEASEHeadSock();

	virtual BOOL InitInstance();
	virtual void ExitInstance();

	virtual BOOL Ready();
	virtual int Status();
	virtual BOOL OpenShutter();
	virtual BOOL CloseShutter();
	virtual BOOL ListRecipes(LPTSTR pszListRecipes);
	virtual BOOL RunRecipe(LPCTSTR pszRecipeId, LPTSTR pszMeasResult);

private:
	BOOL Connect();
	void CloseSocket(BOOL bReset = FALSE);
	BOOL InitEASE();
	BOOL SendCommand(LPCTSTR pszCmd, BOOL bRetry = TRUE);
	BOOL CheckWaitStatusForEASE(int iTimeoutSec);
	BOOL RecvData(LPTSTR pszData, int iTimeoutSec);
	BOOL CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData);
	BOOL ResetConnection();

private:
	int m_iGeneralCommandTimeout;
	int m_iAnalyzeTimeout;
	CString m_strIPAddress;
	WORD m_nPortNo;

	int m_iWSAErr;
	SOCKET m_sock;
	BOOL m_bConnect;
};
