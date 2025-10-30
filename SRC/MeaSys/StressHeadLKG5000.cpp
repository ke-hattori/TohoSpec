// StressHeadLKG5000.cpp : Defines the StressHeadLKG5000 routines.
//

#include "stdafx.h"
#include <math.h>
#include "../../INC/Globals.hxx"
#include "../../INC/common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "LogFile.h"
#include "StressHead.h"
#include "resource.h"
#include "System.h"
#include "Mojiretsu.h"

#define RS232C_RECV_TIMEOUT 	(5)
#define RS232C_SEND_CHAR		(256)
#define RS232C_RECV_CHAR		(4096 * 4 *100)

//	R}h
#define COMMMODE		 		_T("Q0")
#define NORMALMODE		   		_T("R0")
#define CHANGEPROGNO	   		_T("PW")
#define CONFIRMPROGNO	   		_T("PR")
#define PANELLOCK		 		_T("KL")
#define INITDATASTRAGE	   		_T("AQ")
#define STARTDATASTRAGE 		_T("AS")
#define STOPDATASTRAGE	   		_T("AP")
#define STATUSINFODATASTRAGE	_T("AN")
#define OUTPUTDATASTRAGE		_T("AO")
#define AUTOZEROON				_T("VS")
#define AUTOZEROOFF				_T("WS")

#define DELIMITER_CHAR	   		_T(",")

extern CLogFile* pLogFile;
extern BOOL bHwSimulation;
extern CMojiretsu* pMojiretsu;

static char g_szMsgBoxCaption[256];

//---------------------------------------------------------------------------
// InitInstance
BOOL CStressHeadLKG5000::InitInstance()
{
	TRACE(_T("CStressHeadLKG5000::InitInstance()\n"));

	TCHAR szBuff[256];
	TCHAR szCommNo[256];
	CString strBuffer;

	strcpy(g_szMsgBoxCaption, "NanoSpec - Stress");
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

	_stprintf(szCommNo, _T("\\\\.\\%s"), comSetting.StressComPort.szCommNo);
	m_hComm = ::CreateFile(szCommNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hComm == INVALID_HANDLE_VALUE ) {
		strBuffer.Format(pMojiretsu->LoadString(IDS_STRING01), comSetting.StressComPort.szCommNo);
		MyMessageBox(NULL, strBuffer, g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		m_hComm = NULL;
		return FALSE;
	}
	::SetupComm(m_hComm, 1200, 256);
	::GetCommState(m_hComm, &dcb);

	_stprintf(szBuff, _T("baud=%s parity=%s data=%s stop=%s"), comSetting.StressComPort.szBaudRate, comSetting.StressComPort.szParity, comSetting.StressComPort.szByteSize, comSetting.StressComPort.szStopBits);
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

/*
 *	ȉÃR}h`FbNȂǁB
 */

	// ʏ탂[hւ̈ڍs
//	if ( !ChangeNormalMode() )
//		return FALSE;
	ChangeNormalMode();				// OɃ[hmFłȂ悤Ȃ̂ŁAG[łOKƂ܂B

	// plbNI
	if ( !PanelLockOn() )
		return FALSE;

	int iProgNo;
	if ( !ConfirmProgramNo(iProgNo) )
		return FALSE;
	if ( m_iProgNo != iProgNo ) {
//		if ( !ChangeProgramNo(iProgNo) )
		if ( !ChangeProgramNo(m_iProgNo) )
			return FALSE;
	}

	// ݒ
	// vO؊
//	ChangeCommMode();				// OɃ[hmFłȂ悤Ȃ̂ŁAG[łOKƂ܂B
	if ( !ChangeCommMode() )
		return FALSE;

	if ( !Send("SW,HB,M,01,3") )	// 胂[h t@NVԍi0FWA1F́A2F́A3F2A4F
		return FALSE;
	if ( !Send("SW,HE,01,1") )		// ݒu[h t@NVԍi0FgUˁA1Fˁj
		return FALSE;

	if ( !Send("SW,CA,2") )			// TvO 2:10microsec 5:100microsec, 8:1000microsec
		return FALSE;
	if ( !Send("SW,CF,1200000,09") )// f[^Xg[Wݒ ~σf[^A~ώ 2:X5, 3:X10, 4:X20, 6:X100, 9:X1000
		return FALSE;
	if ( !Send("SW,CH,0") )			// A[o͌` 0:VXeA[A1:lA[A2:
		return FALSE;

	if ( !ChangeNormalMode() )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CStressHeadLKG5000::ExitInstance()
{
	TRACE(_T("CStressHeadLKG5000::ExitInstance()\n"));

//	if ( m_hComm ) {
//		PanelLockOff();
//	}

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
// ChangeCommMode
// [hύXR}h - ʐM[hւ̈ڍs
BOOL CStressHeadLKG5000::ChangeCommMode()
{
	return Send(COMMMODE);
}

//---------------------------------------------------------------------------
// ChangeNormalMode
// [hύXR}h - ʏ탂[hւ̈ڍs
BOOL CStressHeadLKG5000::ChangeNormalMode()
{
	return Send(NORMALMODE);
}

//---------------------------------------------------------------------------
// ChangeProgramNo
// 萧R}h - vO؊
BOOL CStressHeadLKG5000::ChangeProgramNo(int iNo)
{
	TCHAR szBuff[256];

	sprintf(szBuff, _T("%s,%d"), CHANGEPROGNO, iNo);
	return Send(szBuff);
}

//---------------------------------------------------------------------------
// ConfirmProgramNo
// 萧R}h - vOmF
BOOL CStressHeadLKG5000::ConfirmProgramNo(int& iNo)
{
	TCHAR szBuff[256];
	LPTSTR token;

	iNo = 0;
	if ( !Send(CONFIRMPROGNO) )
		return FALSE;

	_tcscpy(szBuff, m_pszRecvBuff);
	token = _tcstok(szBuff, DELIMITER_CHAR);
	if ( _tcscmp(token, CONFIRMPROGNO) != 0 )
		return FALSE;

	if ( (token = _tcstok(NULL, DELIMITER_CHAR)) != NULL ) {
		iNo = atoi(token);
	}
	else {
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// PanelLockOn
// 萧R}h - plbNI
BOOL CStressHeadLKG5000::PanelLockOn()
{
	TCHAR szBuff[256];

	sprintf(szBuff, _T("%s,%d"), PANELLOCK, 1);			// "KL,1"
	return Send(szBuff);
}

//---------------------------------------------------------------------------
// PanelLockOff
// 萧R}h - plbNIt
BOOL CStressHeadLKG5000::PanelLockOff()
{
	TCHAR szBuff[256];

	sprintf(szBuff, _T("%s,%d"), PANELLOCK, 0);			// "KL,0"
	return Send(szBuff);
}

//---------------------------------------------------------------------------
// AutoZeroOn
// 萧R}h - I[g[ONiPj
BOOL CStressHeadLKG5000::AutoZeroOn(int iChannel)
{
	TCHAR szBuff[256];

	sprintf(szBuff, _T("%s,%02d"), AUTOZEROON, iChannel);
	return Send(szBuff);
}

//---------------------------------------------------------------------------
// AutoZeroOff
// 萧R}h - I[g[OFFiPj
BOOL CStressHeadLKG5000::AutoZeroOff(int iChannel)
{
	TCHAR szBuff[256];

	sprintf(szBuff, _T("%s,%02d"), AUTOZEROOFF, iChannel);
	return Send(szBuff);
}

//---------------------------------------------------------------------------
// InitDataStrage
// 萧R}h - f[^Xg[W
BOOL CStressHeadLKG5000::InitDataStrage()
{
	return Send(INITDATASTRAGE);
}

//---------------------------------------------------------------------------
// StartDataStrage
// 萧R}h - f[^Xg[WJn
BOOL CStressHeadLKG5000::StartDataStrage()
{
	return Send(STARTDATASTRAGE);
}

//---------------------------------------------------------------------------
// StopDataStrage
// 萧R}h - f[^Xg[W~
BOOL CStressHeadLKG5000::StopDataStrage()
{
	return Send(STOPDATASTRAGE);
}

//---------------------------------------------------------------------------
// StatusInfoDataStrage
// 萧R}h - f[^Xg[WE~Ϗԏo
BOOL CStressHeadLKG5000::StatusInfoDataStrage(int& iState, int& iData)			// OUT01Œ
{
	TCHAR szBuff[256];
	LPTSTR token;

	iState = 0;
	iData = 0;
	if ( !Send(STATUSINFODATASTRAGE) )
		return FALSE;

	_tcscpy(szBuff, m_pszRecvBuff);
	token = _tcstok(szBuff, DELIMITER_CHAR);
	if ( _tcscmp(token, STATUSINFODATASTRAGE) != 0 )
		return FALSE;

	if ( (token = _tcstok(NULL, DELIMITER_CHAR)) != NULL )
		iState = atoi(token);
	else
		return FALSE;

	if ( (token = _tcstok(NULL, DELIMITER_CHAR)) != NULL )
		iData = atoi(token);
	else
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// OutputDataStrage
// 萧R}h - f[^Xg[WEf[^o
BOOL CStressHeadLKG5000::OutputDataStrage(int iChannel, double dDataList[], int nOccurence)		// nOccurence = StatusInfoDataStrageŎ擾f[^
{
	CStringArray stringArray;

	stringArray.RemoveAll();
	if ( !OutputDataStrage(iChannel, stringArray) )
		return FALSE;
	if ( stringArray.GetSize() != nOccurence )				// f[^Ȃ
		return FALSE;

	int i;


	for ( i = 0; i < nOccurence; i++ ) {
		if ( stringArray[i] == _T("XXXXXXXX") )				// 肦ȂƎvAÔ߃K[hĂ
			dDataList[i] = 0.0;
		else if ( stringArray[i] == _T("+FFFFFFF") )
			dDataList[i] = 0.0;
		else if ( stringArray[i] == _T("-FFFFFFF") )
			dDataList[i] = 0.0;
		else
			dDataList[i] = atof(stringArray[i]);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// OutputDataStrage
// 萧R}h - f[^Xg[WEf[^o
BOOL CStressHeadLKG5000::OutputDataStrage(int iChannel, CStringArray& rStringArray)
{
	TCHAR szBuff[256];
	TCHAR* psz;
	LPTSTR token;

	sprintf(szBuff, _T("%s,%02d"), OUTPUTDATASTRAGE, iChannel);			// ex. "AO,01"
	if ( !Send(szBuff) )
		return FALSE;

	rStringArray.RemoveAll();
	psz = new TCHAR[_tcslen(m_pszRecvBuff) + 1];
	_tcscpy(psz, m_pszRecvBuff);
	token = _tcstok(psz, DELIMITER_CHAR);
	if ( _tcscmp(token, OUTPUTDATASTRAGE) != 0 ) {
		delete [] psz;
		psz = NULL;
		return FALSE;
	}
	token = _tcstok(NULL, DELIMITER_CHAR);
	while ( token ) {
		rStringArray.Add(token);
		token = _tcstok(NULL, DELIMITER_CHAR);
	}

	delete [] psz;
	psz = NULL;

	return TRUE;
}

//---------------------------------------------------------------------------
// Send
BOOL CStressHeadLKG5000::Send(LPCTSTR pszCmd)
{
	_tcscpy(m_pszSendBuff, pszCmd);

	pLogFile->LoggingV("%s%s", "CStressHeadLKG5000::Send ", m_pszSendBuff);

	if ( !SendCommand() )
		return FALSE;
	if ( !RecvData() )
		return FALSE;

	pLogFile->LoggingV("%s%s", "CStressHeadLKG5000::Recv ", m_pszRecvBuff);

	if ( !CheckCmdAndStatusCode() )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// SendCommand
BOOL CStressHeadLKG5000::SendCommand()
{
	TRACE(_T("CStressHeadLKG5000::SendCommand()\n"));
	TRACE1(_T("%s\n"), m_pszSendBuff);

	BOOL bError;
	BYTE bySend;
	DWORD dwNumberOfBytesWritten;

	const int MAXTIMES = 1;
	int iTry;

	for ( iTry = 0; iTry < MAXTIMES; iTry++ ) {
		bError = FALSE;
		int i;

		for ( i = 0; m_pszSendBuff[i]; i++ ) {
			if ( !::WriteFile(m_hComm, &m_pszSendBuff[i], 1, &dwNumberOfBytesWritten, NULL) ) {
				bError = TRUE;
				break;
			}
		}
		if ( bError )
			continue;		// Error -> Retry

		bySend = (BYTE)_TCHAR('\r');
		if ( !::WriteFile(m_hComm, &bySend, 1, &dwNumberOfBytesWritten, NULL) ) // I[CR̐ݒ
			continue;		// Error -> Retry
		return TRUE;		// Normal End
	}
	return FALSE;
}

//---------------------------------------------------------------------------
// WaitForRecvData
BOOL CStressHeadLKG5000::WaitForRecvData(int iTimeoutSec)
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
BOOL CStressHeadLKG5000::RecvData()
{
	DWORD dwNumberOfBytesRead;

	int i;


	for ( i = 0; i < RS232C_RECV_CHAR; i++ ) {
		if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) )
			return FALSE;
		if ( !::ReadFile(m_hComm, &m_pszRecvBuff[i], 1, &dwNumberOfBytesRead, NULL) )
			return FALSE;
		if ( m_pszRecvBuff[i] == _TCHAR('\r') ) {
			m_pszRecvBuff[i] = NULL;								// I[CRNULLɒu
			break;
		}
	}
	if ( i == RS232C_RECV_CHAR ) {									// I[CRȂ
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// CheckCmdAndStatusCode
BOOL CStressHeadLKG5000::CheckCmdAndStatusCode()
{
	if ( m_pszRecvBuff[0] == 'E' &&
		 m_pszRecvBuff[1] == 'R' ) {
		return FALSE;
	}

	if ( strncmp(m_pszSendBuff, m_pszRecvBuff, 2) != 0 ) {
//		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// StringHexToint
//   -1 : ϊs\isw肳ꂽj
//   xx : int^l
int CStressHeadLKG5000::StringHexToint(LPCTSTR psz, size_t count)
{
	ASSERT( 0 < count );
	ASSERT( count <= sizeof(int) * 8 / 4 );

	int iBuff;
	int iRet = 0;
	int i = 0;

	while ( (iBuff = psz[i]) != NULL )
	{
		if ( iBuff >= _TCHAR('A') && iBuff <= _TCHAR('F') ) {
			iBuff = iBuff - _TCHAR('A') + 10;
		}
		else if ( iBuff >= _TCHAR('0') && iBuff <= _TCHAR('9') ) {
			iBuff -= _TCHAR('0');
		}
		else {
			iRet = -1;
			break;
		}
		iBuff <<= 4 * (count - i - 1);
		iRet |= iBuff;
		i++;
	}
	return iRet;
}
