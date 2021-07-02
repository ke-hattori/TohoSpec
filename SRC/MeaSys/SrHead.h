#pragma once

#include "../../INC/Common.hxx"
#include "IoBase.hxx"


// ===========================================================================
// CSrHead class
// ===========================================================================
class CSrHead
{
public:
	CSrHead() {::ZeroMemory(m_szErrorMessage, sizeof(m_szErrorMessage));}
	virtual ~CSrHead() {}

	enum CCDSHUTTER_POS {FILTER_OPEN = 1, FILTER_DARK, FILTER_YELLOW, FILTER_ORANGE, FILTER_POS3};

	virtual BOOL InitInstance()=0;
	virtual void ExitInstance()=0;
	virtual BOOL GetVersion(LPTSTR pszVersion)=0;
	virtual BOOL CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans = 1, BOOL bProhibitNotify = FALSE)=0;
	virtual BOOL ChangeCcdShutter(int iPos)=0;
	virtual BOOL OpenUvShutter()=0;
	virtual BOOL CloseUvShutter()=0;
	virtual BOOL OpenTransShutter()=0;
	virtual BOOL CloseTransShutter()=0;
	virtual void MyMessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
	{
		_tcscpy(m_szErrorMessage, lpText);
		::MessageBox(hWnd, lpText, lpCaption, uType);
	}
	virtual LPCTSTR GetLastError() {return m_szErrorMessage;}

protected:
	enum UVSHUTTER_POS {UV_CLOSE = 0, UV_OPEN};
	enum TRANSSHUTTER_POS {TRANS_CLOSE = 0, TRANS_OPEN};

	TCHAR m_szErrorMessage[256];
};

// ===========================================================================
// CSrHeadDesktop class
// ===========================================================================
class CSrHeadDesktop : public CSrHead
{
public:
	CSrHeadDesktop() {}
	virtual ~CSrHeadDesktop() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL GetVersion(LPTSTR pszVersion);
	virtual BOOL CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans = 1, BOOL bProhibitNotify = FALSE);
	virtual BOOL ChangeCcdShutter(int iPos);
	virtual BOOL OpenUvShutter();
	virtual BOOL CloseUvShutter();
	virtual BOOL OpenTransShutter();
	virtual BOOL CloseTransShutter();
};

// ===========================================================================
// CSrHeadSerial class
// ===========================================================================
class CSrHeadSerial : public CSrHead
{
public:
	CSrHeadSerial() : m_hComm(NULL) {}
	virtual ~CSrHeadSerial() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL GetVersion(LPTSTR pszVersion);
	virtual BOOL CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans = 1, BOOL bProhibitNotify = FALSE);
	virtual BOOL ChangeCcdShutter(int iPos);
	virtual BOOL OpenUvShutter();
	virtual BOOL CloseUvShutter();
	virtual BOOL OpenTransShutter();
	virtual BOOL CloseTransShutter();

protected:
	BOOL ChangeUvShutter(int iPos);
	BOOL ChangeTransShutter(int iPos);

	BOOL SendCommand(LPCTSTR pszCmd);
	BOOL WaitForRecvData(int iTimeoutSec);
	BOOL RecvData(LPTSTR pszData);
	BOOL CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData);
	int GetCcdScanData1By1();
	BOOL StringHexToint(LPCTSTR psz, size_t count);

	HANDLE m_hComm;
	BOOL m_bTransferModeBinary;
};

// ===========================================================================
// CSrHeadParallel class
// ===========================================================================
class CSrHeadParallel : public CSrHead, protected dpram
{
public:
	CSrHeadParallel() {}
	virtual ~CSrHeadParallel() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL GetVersion(LPTSTR pszVersion);
	virtual BOOL CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans = 1, BOOL bProhibitNotify = FALSE);
	virtual BOOL ChangeCcdShutter(int iPos);
	virtual BOOL OpenUvShutter();
	virtual BOOL CloseUvShutter();
	virtual BOOL OpenTransShutter();
	virtual BOOL CloseTransShutter();

protected:
	enum DP_STATUS {DP_IDLE = 0, DP_SEND, DP_MOVING, DP_RCVING, DP_COMPLETE};
	unsigned short DualPortRam_Read(unsigned short port);
	void DualPortRam_Write(unsigned short port, unsigned short dataword);
	unsigned short GetStatus();
	void SetStatus(WORD wStatus);
	signed short FifoRead();

	int GetCcdScanData1By1(int iTimeoutSec);
	BOOL ChangeUvShutter(int iPos);
	BOOL ChangeTransShutter(int iPos);

	void SendCommand(LPCTSTR pszCmd);
	BOOL WaitForStatusChange(WORD wStatus, int iTimeoutSec);
	BOOL RecvData(LPTSTR pszData);
	BOOL CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData);
};
