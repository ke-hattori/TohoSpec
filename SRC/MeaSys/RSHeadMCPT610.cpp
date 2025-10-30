// RSHeadMCPT610.cpp : Defines the RSHeadMCPT610 routines.
//

#include "stdafx.h"
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "resource.h"
#include "System.h"
#include "Mojiretsu.h"
#include "LogFile.h"
#include "RSHead.h"

#define RS232C_RECV_TIMEOUT 	(5)
#define RS232C_SEND_CHAR		(256)
#define RS232C_RECV_CHAR		(4096)

//	R}h
#define RESET		 		_T("RST")		// 
#define AD_OFFSET	   		_T("ZRD")		// A/DRo[^ÃÉItZbgsi10b‘Åj
#define RANGE		 		_T("RNG")		// ËÉåW›íiRNGnn, nn=-3`+7j
#define PROBE	  			_T("PRB")		// v[u›íiPRBnj
#define LIMITV 			   	_T("LMT")		// ~b^d›íiLMTn, n=0:10V,1:90Vj
#define MEAS_START	   		_T("MES")		// Jn
#define MEAS_END			_T("HLD")		// I
#define STATUSREQ	 		_T("SRQ")		// Xe[^Xv
#define PARAMREQ			_T("PRQ")		// p[^v
#define MEAS_DATAREQ		_T("DRQ")		// f[^v
#define RCF					_T("RCF")		// R‚ê≥WiRCFXXXXX, XXXXX=0.001`9999i≈ë5jj
#define THICKDATA			_T("THK")		// ›Éf[^iTHKXXXXX##, XXXXX=0.001`9999i≈ë5j, ##=MM,UM,NM,AMj

extern CLogFile* pLogFile;
extern CMojiretsu* pMojiretsu;

static char g_szMsgBoxCaption[256];

//---------------------------------------------------------------------------
// InitInstance
BOOL CRsHeadMCPT610::InitInstance()
{
	TRACE(_T("CRsHeadMCPT610::InitInstance()\n"));

	TCHAR szBuff[256];
	TCHAR szCommNo[256];
	CString strBuffer;

	strcpy(g_szMsgBoxCaption, "NanoSpec - Resist");
	if(g_lAppNameType != APP_NAME_NANO){
		strncpy(g_szMsgBoxCaption, g_lpszAppPrefix4[g_lAppNameType], 4);
	}

	m_pszSendBuff = new TCHAR[RS232C_SEND_CHAR];
	m_pszRecvBuff = new TCHAR[RS232C_RECV_CHAR];

	DCB dcb;
	COMSTAT stat;
	DWORD dwErrorFlags;

	///// Get Config Data /////
	COM_SETTING comSetting;
	ConfigFile_GetNanoSpecIni(&comSetting, CONFIG_FILE_COM);

	_stprintf(szCommNo, _T("\\\\.\\%s"), comSetting.RsComPort.szCommNo);
	m_hComm = ::CreateFile(szCommNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hComm == INVALID_HANDLE_VALUE ) {
		strBuffer.Format(pMojiretsu->LoadString(IDS_STRING01), comSetting.RsComPort.szCommNo);
		MyMessageBox(NULL, strBuffer, g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		m_hComm = NULL;
		return FALSE;
	}
	::SetupComm(m_hComm, 1200, 256);
	::GetCommState(m_hComm, &dcb);

	_stprintf(szBuff, _T("baud=%s parity=%s data=%s stop=%s"), comSetting.RsComPort.szBaudRate, comSetting.RsComPort.szParity, comSetting.RsComPort.szByteSize, comSetting.RsComPort.szStopBits);
	::BuildCommDCB(szBuff, &dcb);
	::SetCommState(m_hComm, &dcb);

#if 0
	COMMTIMEOUTS commTimeouts;
	::GetCommTimeouts(m_hComm, &commTimeouts);
	commTimeouts.ReadIntervalTimeout		 = MAXDWORD;
	commTimeouts.ReadTotalTimeoutMultiplier  = 0;
	commTimeouts.ReadTotalTimeoutConstant	 = 0;
	commTimeouts.WriteTotalTimeoutMultiplier = 0;
	commTimeouts.WriteTotalTimeoutConstant	 = 0;
	::SetCommTimeouts(m_hComm, &commTimeouts);
#endif

	::ClearCommError(m_hComm, &dwErrorFlags, &stat);
	::PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR);
	::PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);

	// X^ÃÉTvR[hR}h2ÒëóêMƒÇÃÇ≈ÅA}l
	int i;

	for ( i = 0; i < 2; i++ ) {
		if ( !Reset() )
			return FALSE;
	}

	// ›ÇÃê›íp[^ÊìæB‹ÇA êM≈ÇÈÇ©ÃÉ`FbNÀÇƒÅA‘ÇR}hëóêMƒÇ
	RSMEASPARAM rsMeasParam;
	if ( !GetMeasParam(&rsMeasParam) ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING04), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	m_iRange = atoi(rsMeasParam.szMeasRange);
	m_iLVMode = atoi(rsMeasParam.szLimitVoltage);
	m_iProbe = atoi(rsMeasParam.szMeasProbe);
	m_dFilmThick = atof(rsMeasParam.szFilmThick);
	strcpy(m_szFilmThickUnit, rsMeasParam.szFilmThickUnit);
	m_dRCF = atof(rsMeasParam.szRCF);

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CRsHeadMCPT610::ExitInstance()
{
	TRACE(_T("CRsHeadMCPT610::ExitInstance()\n"));

	if ( m_hComm ) {
		::CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	if ( m_pszSendBuff ) {
		delete [] m_pszSendBuff;
		m_pszSendBuff = NULL;
	}
	if ( m_pszRecvBuff ) {
		delete [] m_pszRecvBuff;
		m_pszRecvBuff = NULL;
	}
}

//---------------------------------------------------------------------------
// Reset
BOOL CRsHeadMCPT610::Reset()
{
	return SendCommand(RESET);
}

//---------------------------------------------------------------------------
// AdOffset
BOOL CRsHeadMCPT610::AdOffset()
{
	return SendCommand(AD_OFFSET);
}

//---------------------------------------------------------------------------
// SetRange
BOOL CRsHeadMCPT610::SetRange(int iRange)
{
	TCHAR szBuff[256];

	if ( iRange == m_iRange )
		return TRUE;

	sprintf(szBuff, _T("%s%+d"), RANGE, iRange);
	if ( !SendCommand(szBuff) )
		return FALSE;

	m_iRange = iRange;

	return TRUE;
}

//---------------------------------------------------------------------------
// SetProbe
BOOL CRsHeadMCPT610::SetProbe(int iProbe)
{
	TCHAR szBuff[256];

	if ( iProbe == m_iProbe )
		return TRUE;

	sprintf(szBuff, _T("%s%d"), PROBE, iProbe);
	if ( !SendCommand(szBuff) )
		return FALSE;

	m_iProbe = iProbe;

	return TRUE;
}

//---------------------------------------------------------------------------
// SetLimitV
BOOL CRsHeadMCPT610::SetLimitV(int iMode)
{
	TCHAR szBuff[256];

	if ( iMode == m_iLVMode )
		return TRUE;

	sprintf(szBuff, _T("%s%d"), LIMITV, iMode);
	if ( !SendCommand(szBuff) )
		return FALSE;

	m_iLVMode = iMode;

	return TRUE;
}

//---------------------------------------------------------------------------
// MeasStart
BOOL CRsHeadMCPT610::MeasStart()
{
	return SendCommand(MEAS_START);
}

//---------------------------------------------------------------------------
// MeasEnd
BOOL CRsHeadMCPT610::MeasEnd()
{
	return SendCommand(MEAS_END);
}

//---------------------------------------------------------------------------
// GetMeasStatus
BOOL CRsHeadMCPT610::GetMeasStatus(RSMEASSTATUS* pRsMeasStatus)
{
	if ( !SendCommand(STATUSREQ) )
		return FALSE;
	if ( !RecvData() )
		return FALSE;
	if ( !CheckCmdAndStatusCode() )
		return FALSE;

	//
	// 0123456789
	// STS###,Enn
	sprintf(pRsMeasStatus->szMeasStatus, "%.3s", m_pszRecvBuff + 3);

	return TRUE;
}

//---------------------------------------------------------------------------
// GetMeasParam
BOOL CRsHeadMCPT610::GetMeasParam(RSMEASPARAM* pRsMeasParam)
{
	if ( !SendCommand(PARAMREQ) )
		return FALSE;
	if ( !RecvData() )
		return FALSE;
	if ( !CheckCmdAndStatusCode() )
		return FALSE;

	//           1         2         3
	// 01234567890123456789012345678901234567
	// PRMRNGnn,LMTn,PRBn,THKXXXXX##,RCFXXXXX
	sprintf(pRsMeasParam->szMeasRange, 			"%.2s",	m_pszRecvBuff + 6);
	sprintf(pRsMeasParam->szLimitVoltage, 		"%.1s",	m_pszRecvBuff + 12);
	sprintf(pRsMeasParam->szMeasProbe, 			"%.1s",	m_pszRecvBuff + 17);
	sprintf(pRsMeasParam->szFilmThick, 			"%.5s",	m_pszRecvBuff + 22);
	sprintf(pRsMeasParam->szFilmThickUnit, 		"%.2s",	m_pszRecvBuff + 27);
	sprintf(pRsMeasParam->szRCF, 				"%.5s",	m_pszRecvBuff + 33);

	return TRUE;
}

//---------------------------------------------------------------------------
// GetMeasData
BOOL CRsHeadMCPT610::GetMeasData(RSMEASDATA* pRsMeasData)
{
	if ( !SendCommand(MEAS_DATAREQ) )
		return FALSE;
	if ( !RecvData() )
		return FALSE;
	if ( !CheckCmdAndStatusCode() )
		return FALSE;

	//           1         2         3         4
	// 012345678901234567890123456789012345678901234
	// D#RX.XXXE+XX,SX.XXXE+XX,VX.XXXE+XX,CX.XXXE+XX
	//          -          -          -          -
	pRsMeasData->chDataValid = m_pszRecvBuff[1];
	sprintf(pRsMeasData->szResistance, 			"%.9s", m_pszRecvBuff + 3);
	sprintf(pRsMeasData->szSurfaceResistivity, 	"%.9s", m_pszRecvBuff + 14);
	sprintf(pRsMeasData->szVolumeResistivity, 	"%.9s", m_pszRecvBuff + 25);
	sprintf(pRsMeasData->szConductivity, 		"%.9s", m_pszRecvBuff + 36);
	pRsMeasData->dResistance 			= atof(pRsMeasData->szResistance);
	pRsMeasData->dSurfaceResistivity 	= atof(pRsMeasData->szSurfaceResistivity);
	pRsMeasData->dVolumeResistivity 	= atof(pRsMeasData->szVolumeResistivity);
	pRsMeasData->dConductivity 			= atof(pRsMeasData->szConductivity);

	return TRUE;
}

//---------------------------------------------------------------------------
// SetRCF
BOOL CRsHeadMCPT610::SetRCF(double dCoef)
{
	TCHAR szBuff[256];
	TCHAR szCoef[256];

	if ( dCoef == m_dRCF )
		return TRUE;

	ConvertFromDoubleToChar(dCoef, szCoef);
	sprintf(szBuff, _T("%s%s"), RCF, szCoef);

	if ( !SendCommand(szBuff) )
		return FALSE;

	m_dRCF = dCoef;

	return TRUE;
}

//---------------------------------------------------------------------------
// SetThickData
BOOL CRsHeadMCPT610::SetThickData(double dThick, LPCTSTR szUnit)
{
	TCHAR szBuff[256];
	TCHAR szThick[256];

	if ( dThick == m_dFilmThick && strcmp(szUnit, m_szFilmThickUnit) == 0 )
		return TRUE;

	ConvertFromDoubleToChar(dThick, szThick);
	sprintf(szBuff, _T("%s%s%s"), THICKDATA, szThick, szUnit);

	if ( !SendCommand(szBuff) )
		return FALSE;

	m_dFilmThick = dThick;
	strcpy(m_szFilmThickUnit, szUnit);

	return TRUE;
}

//---------------------------------------------------------------------------
// SendCommand
BOOL CRsHeadMCPT610::SendCommand(LPCTSTR pszCmd)
{
	TRACE(_T("CRsHeadMCPT610::SendCommand()\n"));

	BOOL bError;
	BYTE bySend;
	DWORD dwNumberOfBytesWritten;

	_tcscpy(m_pszSendBuff, pszCmd);

	const int MAXTIMES = 1;
	int iTry;

	for ( iTry = 0; iTry < MAXTIMES; iTry++ ) {
		bError = FALSE;
		int i;

		for ( i = 0; pszCmd[i]; i++ ) {
			if ( !::WriteFile(m_hComm, &pszCmd[i], 1, &dwNumberOfBytesWritten, NULL) ) {
				bError = TRUE;
				break;
			}
		}
		if ( bError )
			continue;		// Error -> Retry

		bySend = (BYTE)_TCHAR('\n');
		if ( !::WriteFile(m_hComm, &bySend, 1, &dwNumberOfBytesWritten, NULL) ) // I[LFÃê›í
			continue;		// Error -> Retry

		pLogFile->Logging(m_pszSendBuff);

		return TRUE;		// Normal End
	}
	return FALSE;
}

//---------------------------------------------------------------------------
// WaitForRecvData
BOOL CRsHeadMCPT610::WaitForRecvData(int iTimeoutSec)
{
	DWORD dwErrorFlags;
	COMSTAT stat;
	CTimer timer;

	timer.Restart(iTimeoutSec);
	while ( 1 ) {
		::ClearCommError(m_hComm, &dwErrorFlags, &stat);
		if ( stat.cbInQue > 0 )
			break;
		if ( timer.IsTimeout() )
			return FALSE;
		::Sleep(1);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// RecvData
BOOL CRsHeadMCPT610::RecvData()
{
	DWORD dwNumberOfBytesRead;

	int i;


	for ( i = 0; i < RS232C_RECV_CHAR; i++ ) {
		if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) )
			return FALSE;
		if ( !::ReadFile(m_hComm, &m_pszRecvBuff[i], 1, &dwNumberOfBytesRead, NULL) )
			return FALSE;
		if ( m_pszRecvBuff[i] == _TCHAR('\n') ) {
			m_pszRecvBuff[i] = NULL;								// I[LFNULL…íu
			break;
		}
	}
	if ( i == RS232C_RECV_CHAR ) {									// I[LF¬Ç»Ç
		return FALSE;
	}

	pLogFile->Logging(m_pszRecvBuff);

	return TRUE;
}

//---------------------------------------------------------------------------
// CheckCmdAndStatusCode
BOOL CRsHeadMCPT610::CheckCmdAndStatusCode()
{
	if ( strncmp(m_pszSendBuff, m_pszRecvBuff, 1) != 0 ) {
		return FALSE;
	}

	switch ( m_pszRecvBuff[0] )
	{
	case 'S':
		return CheckStatus();
		break;
	case 'P':
		return CheckParam();
		break;
	case 'D':
		return CheckMeasData();
		break;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// CheckStatus
BOOL CRsHeadMCPT610::CheckStatus()
{
	//
	// 0123456789
	// STS###,Enn
	TRACE(_T("Measure: %.3s, Status: %.2s\n"), m_pszRecvBuff + 3, m_pszRecvBuff + 8);

	return ( strncmp(m_pszRecvBuff + 8, _T("00"), 2) == 0 );
}

//------------------------------------------------------------------------------
// CheckParam
BOOL CRsHeadMCPT610::CheckParam()
{
	//           1         2         3
	// 01234567890123456789012345678901234567
	// PRMRNGnn,LMTn,PRBn,THKXXXXX##,RCFXXXXX

	TRACE(_T("Meas Range: %.2s\n"), 		m_pszRecvBuff + 6);
	TRACE(_T("Limit Voltage: %.1s\n"), 		m_pszRecvBuff + 12);
	TRACE(_T("Meas Probe: %.1s\n"),			m_pszRecvBuff + 17);
	TRACE(_T("Sample Thick: %.5s\n"), 		m_pszRecvBuff + 22);
	TRACE(_T("Sample Thick Unit: %.2s\n"),	m_pszRecvBuff + 27);
	TRACE(_T("RCF: %.5s\n\n"), 				m_pszRecvBuff + 33);

	return TRUE;
}

//------------------------------------------------------------------------------
// CheckMeasData
BOOL CRsHeadMCPT610::CheckMeasData()
{
	//           1         2         3         4
	// 012345678901234567890123456789012345678901234
	// D#RX.XXXE+XX,SX.XXXE+XX,VX.XXXE+XX,CX.XXXE+XX
	//          -          -          -          -

	BOOL bRet = FALSE;
	double dResistance;
	double dSurfaceResistivity;
	double dVolumeResistivity;
	double dConductivity;

	switch ( m_pszRecvBuff[1] )
	{
	case _T('N'):
		TRACE(_T("Measurement Data OK\n"));
		dResistance 		= atof(m_pszRecvBuff + 3);
		dSurfaceResistivity = atof(m_pszRecvBuff + 14);
		dVolumeResistivity 	= atof(m_pszRecvBuff + 25);
		dConductivity 		= atof(m_pszRecvBuff + 36);
		TRACE(_T("Resistance: %.9lf\n"), 			dResistance);
		TRACE(_T("SurfaceResistivity: %.9lf\n"), 	dSurfaceResistivity);
		TRACE(_T("VolumeResistivity: %.9lf\n"), 	dVolumeResistivity);
		TRACE(_T("Conductivity: %.9lf\n\n"), 		dConductivity);
		bRet = TRUE;
		break;
	case _T('E'):
		TRACE(_T("Measurement Error\n\n"));
		break;
	case _T('O'):
		TRACE(_T("Measurement OverRange\n\n"));
		bRet = TRUE;
		break;
	case _T('L'):
		TRACE(_T("Measurement OverLoad\n\n"));
		bRet = TRUE;
		break;
	default:
		TRACE(_T("Measurement Error\n\n"));
		break;
	}

	return bRet;
}

//---------------------------------------------------------------------------
// ConvertFromDoubleToChar
void CRsHeadMCPT610::ConvertFromDoubleToChar(double dNum, char* lpszBuff)
{
	char*	   lpszText;
	int 	   iDecimal, iSign, iIndex;
	char	   szText[256];

	lpszText = _ecvt(dNum, 5, &iDecimal, &iSign);
	memset(szText, NULL, sizeof(szText));
	sprintf(szText, _T("%lf"), dNum);
	szText[5] = _T('\0');

	if(iDecimal == 4)
		szText[iDecimal] = _T('\0');
	else
	{
		iIndex = 4;
		while((iIndex >= iDecimal) && (iIndex > 0))
		{
			if((szText[iIndex] == _T('0')) || (szText[iIndex] == _T('.')))
				szText[iIndex] = _T('\0');
			else
				break;
			iIndex--;
		}
	}

	strcpy(lpszBuff, szText);
}
