#pragma once

typedef struct _RSMEASSTATUS {
	TCHAR szMeasStatus[3 + 1];
} RSMEASSTATUS;

typedef struct _RSMEASPARAM {
	TCHAR szMeasRange[2 + 1];
	TCHAR szLimitVoltage[1 + 1];
	TCHAR szMeasProbe[1 + 1];
	TCHAR szFilmThick[5 + 1];
	TCHAR szFilmThickUnit[2 + 1];
	TCHAR szRCF[5 + 1];
} RSMEASPARAM;

typedef struct _RSMEASDATA {
	TCHAR chDataValid;
	TCHAR szResistance[9 + 1];
	TCHAR szSurfaceResistivity[9 + 1];
	TCHAR szVolumeResistivity[9 + 1];
	TCHAR szConductivity[9 + 1];
	double dResistance;
	double dSurfaceResistivity;
	double dVolumeResistivity;
	double dConductivity;
} RSMEASDATA;

// ===========================================================================
// CRsHead class
// ===========================================================================
class CRsHead
{
public:
	CRsHead() {::ZeroMemory(m_szErrorMessage, sizeof(m_szErrorMessage));}
	virtual ~CRsHead() {}

	virtual BOOL InitInstance()=0;
	virtual void ExitInstance()=0;

	virtual BOOL Reset()=0;
	virtual BOOL AdOffset()=0;
	virtual BOOL SetRange(int iRange)=0;
	virtual BOOL SetProbe(int iProbe)=0;
	virtual BOOL SetLimitV(int iMode)=0;
	virtual BOOL MeasStart()=0;
	virtual BOOL MeasEnd()=0;
	virtual BOOL GetMeasStatus(RSMEASSTATUS* pRsMeasStatus)=0;
	virtual BOOL GetMeasParam(RSMEASPARAM* pRsMeasParam)=0;
	virtual BOOL GetMeasData(RSMEASDATA* pRsMeasData)=0;
	virtual BOOL SetRCF(double dCoef)=0;
	virtual BOOL SetThickData(double dThick, LPCTSTR szUnit)=0;
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
// CRsHeadDesktop class
// ===========================================================================
class CRsHeadDesktop : public CRsHead
{
public:
	CRsHeadDesktop() {}
	virtual ~CRsHeadDesktop() {}

	virtual BOOL InitInstance() {return TRUE;}
	virtual void ExitInstance() {}

	virtual BOOL Reset() {return TRUE;}
	virtual BOOL AdOffset() {return TRUE;}
	virtual BOOL SetRange(int iRange) {return TRUE;}
	virtual BOOL SetProbe(int iProbe) {return TRUE;}
	virtual BOOL SetLimitV(int iMode) {return TRUE;}
	virtual BOOL MeasStart() {return TRUE;}
	virtual BOOL MeasEnd() {return TRUE;}
	virtual BOOL GetMeasStatus(RSMEASSTATUS* pRsMeasStatus)
	{
		strcpy(pRsMeasStatus->szMeasStatus, "HLD");

		return TRUE;
	}
	virtual BOOL GetMeasParam(RSMEASPARAM* pRsMeasParam)
	{
		strcpy(pRsMeasParam->szMeasRange, 		"+0");
		strcpy(pRsMeasParam->szLimitVoltage, 	"0");
		strcpy(pRsMeasParam->szMeasProbe, 		"5");
		strcpy(pRsMeasParam->szFilmThick,	 	"1.000");
		strcpy(pRsMeasParam->szFilmThickUnit,	"UM");
		strcpy(pRsMeasParam->szRCF, 			"4.532");

		return TRUE;
	}
	virtual BOOL GetMeasData(RSMEASDATA* pRsMeasData)
	{
		DWORD dwTickCount = ::GetTickCount();
		int r = dwTickCount % 9;
		switch ( r ) {
		case 0:
		case 1:
		case 2:
		case 3:
			pRsMeasData->chDataValid = 'N';
			strcpy(pRsMeasData->szResistance, 			"0.991E+00");
			strcpy(pRsMeasData->szSurfaceResistivity, 	"4.496E+00");
			strcpy(pRsMeasData->szVolumeResistivity, 	"4.496E-04");
			strcpy(pRsMeasData->szConductivity, 		"2.224E+03");
			pRsMeasData->dResistance 			= atof(pRsMeasData->szResistance);
			pRsMeasData->dSurfaceResistivity 	= atof(pRsMeasData->szSurfaceResistivity);
			pRsMeasData->dVolumeResistivity 	= atof(pRsMeasData->szVolumeResistivity);
			pRsMeasData->dConductivity 			= atof(pRsMeasData->szConductivity);
			break;
		case 4:
		case 5:
		case 6:
			pRsMeasData->chDataValid = 'N';
			strcpy(pRsMeasData->szResistance, 			"2.081E+00");
			strcpy(pRsMeasData->szSurfaceResistivity, 	"9.431E+00");
			strcpy(pRsMeasData->szVolumeResistivity, 	"9.431E-04");
			strcpy(pRsMeasData->szConductivity, 		"1.060E+03");
			pRsMeasData->dResistance 			= atof(pRsMeasData->szResistance);
			pRsMeasData->dSurfaceResistivity 	= atof(pRsMeasData->szSurfaceResistivity);
			pRsMeasData->dVolumeResistivity 	= atof(pRsMeasData->szVolumeResistivity);
			pRsMeasData->dConductivity 			= atof(pRsMeasData->szConductivity);
			break;
		case 7: // オーバーレンジ
			pRsMeasData->chDataValid = 'O';
			strcpy(pRsMeasData->szResistance, 			"0.000E+00");
			strcpy(pRsMeasData->szSurfaceResistivity, 	"0.000E+00");
			strcpy(pRsMeasData->szVolumeResistivity, 	"0.000E+00");
			strcpy(pRsMeasData->szConductivity, 		"0.000E+00");
			pRsMeasData->dResistance 			= atof(pRsMeasData->szResistance);
			pRsMeasData->dSurfaceResistivity 	= atof(pRsMeasData->szSurfaceResistivity);
			pRsMeasData->dVolumeResistivity 	= atof(pRsMeasData->szVolumeResistivity);
			pRsMeasData->dConductivity 			= atof(pRsMeasData->szConductivity);
			break;
		case 8: // オーバーロード
			pRsMeasData->chDataValid = 'L';
			strcpy(pRsMeasData->szResistance, 			"0.000E+00");
			strcpy(pRsMeasData->szSurfaceResistivity, 	"0.000E+00");
			strcpy(pRsMeasData->szVolumeResistivity, 	"0.000E+00");
			strcpy(pRsMeasData->szConductivity, 		"0.000E+00");
			pRsMeasData->dResistance 			= atof(pRsMeasData->szResistance);
			pRsMeasData->dSurfaceResistivity 	= atof(pRsMeasData->szSurfaceResistivity);
			pRsMeasData->dVolumeResistivity 	= atof(pRsMeasData->szVolumeResistivity);
			pRsMeasData->dConductivity 			= atof(pRsMeasData->szConductivity);
			break;
		}

		return TRUE;
	}
	virtual BOOL SetRCF(double dCoef) {return TRUE;}
	virtual BOOL SetThickData(double dThick, LPCTSTR szUnit) {return TRUE;}
};

// ===========================================================================
// CRsHeadMCPT610 class
// ===========================================================================
class CRsHeadMCPT610 : public CRsHead
{
public:
	CRsHeadMCPT610() : m_hComm(NULL), m_pszSendBuff(NULL), m_pszRecvBuff(NULL) {}
	virtual ~CRsHeadMCPT610() {}

	virtual BOOL InitInstance();
	virtual void ExitInstance();

	virtual BOOL Reset();
	virtual BOOL AdOffset();
	virtual BOOL SetRange(int iRange);
	virtual BOOL SetProbe(int iProbe);
	virtual BOOL SetLimitV(int iMode);
	virtual BOOL MeasStart();
	virtual BOOL MeasEnd();
	virtual BOOL GetMeasStatus(RSMEASSTATUS* pRsMeasStatus);
	virtual BOOL GetMeasParam(RSMEASPARAM* pRsMeasParam);
	virtual BOOL GetMeasData(RSMEASDATA* pRsMeasData);
	virtual BOOL SetRCF(double dCoef);
	virtual BOOL SetThickData(double dThick, LPCTSTR szUnit);

protected:
	BOOL SendCommand(LPCTSTR pszCmd);
	BOOL WaitForRecvData(int iTimeoutSec);
	BOOL RecvData();
	BOOL CheckCmdAndStatusCode();
	BOOL CheckStatus();
	BOOL CheckParam();
	BOOL CheckMeasData();
	void ConvertFromDoubleToChar(double dNum, char* lpszBuff);

	int m_iRange;
	int m_iLVMode;
	int m_iProbe;
	double m_dFilmThick;
	TCHAR m_szFilmThickUnit[2 + 1];
	double m_dRCF;

	HANDLE m_hComm;
   	TCHAR* m_pszSendBuff;
   	TCHAR* m_pszRecvBuff;
};
