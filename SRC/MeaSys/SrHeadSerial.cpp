// SrHeadSerial.cpp : Defines the SrHeadSerial routines.
//

#include "stdafx.h"
#include "SrHead.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\ConfigFile.hxx"
#include "..\\..\\INC\\MeaSys.hxx"
#include "resource.h"
#include "Mojiretsu.h"
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--

#define RS232C_RECV_TIMEOUT 		(5)
#define RS232C_RECV_INTEG_TIMEOUT	(45)
#define RS232C_SEND_CHAR			(256)
#define RS232C_RECV_CHAR			(4096 * 4)

//	R}h
#define SETCHP			   _T("G")		 // Set UV shutter position
#define CCDSCN			   _T("L")		 // CCD Scan
#define INITIL			   _T("Q")		 // Initialize & Calibrate Wavelength
#define SENDPN			   _T("R")		 // Send Software Part Number			   //get version
#define RAMCHK			   _T("S")		 // RAM Check
#define PRCHK			   _T("T")		 // PROM Check
#define RDPORT			   _T("W")		 // Read Port
#define WRPORT			   _T("X")		 // Write Port
#define GOSHUT			   _T("Z")		 // GO CCD SHUTTER POSITION

#define TRANSSHUTTER_P	   _T("45") 	 // Transmission Light					   // D_OUT_PORT1	  Offset:1535d, I8255_PB2/045H, MASK 0E0H
#define TRANSSHUTTER_B	   (0x20)

extern HWND g_hNotifyWnd;
extern CMojiretsu* pMojiretsu;
/* added 2009.07.07 hmenjo dll ΃pXΉ MeaSys.dll ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];		/* ďovZX̃fBNg('\'t)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* fBNg('\'t)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
extern void AddAbsPath(LPTSTR ptszPath);
/* added 2009.07.07 hmenjo dll ΃pXΉ MeaSys.dll ---------- } ---------- */

static char g_szMsgBoxCaption[256];

//---------------------------------------------------------------------------
// InitInstance
BOOL CSrHeadSerial::InitInstance()
{
	TRACE(_T("CSrHeadSerial::InitInstance()\n"));

	TCHAR szBuff[256];
	TCHAR szCommNo[256];
	int iCcdDataTransferModeBinary;
	TCHAR szSendBuff[RS232C_SEND_CHAR];
	TCHAR szRecvBuff[RS232C_RECV_CHAR];
	BYTE bySend;
	BYTE byRecv;
	CString strBuffer;
	UINT nID;

	DCB dcb;
	COMSTAT stat;
	DWORD dwErrorFlags;
	DWORD dwNumberOfBytesWritten, dwNumberOfBytesRead;
	CTimer timer;

	strcpy(g_szMsgBoxCaption, "NanoSpec");
	if(g_lAppNameType != APP_NAME_NANO){
		strncpy(g_szMsgBoxCaption, g_lpszAppPrefix4[g_lAppNameType], 4);
	}

/* modified 2009.07.07 hmenjo dll ΃pXΉ MeaSys.dll ---------- { ---------- */
	TCHAR l_tszIniPath[_MAX_PATH];
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
//	_stprintf(l_tszIniPath, _T("%s") CFG_DIR NANOSPEC_INIFILENAME, g_tszProcDir);

	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if(g_lAppNameType != APP_NAME_NANO){
		strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}
	_stprintf(l_tszIniPath, _T("%s%s%s"), CFG_DIR, g_tszProcDir, strFilename);
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--
	iCcdDataTransferModeBinary = ::GetPrivateProfileInt(INISECTION_HEADCOM, INIKEY_CCDDATATRANSFERMODEBINARY, 1, l_tszIniPath);
/* modified 2009.07.07 hmenjo dll ΃pXΉ MeaSys.dll ---------- } ---------- */

	///// Get Config Data /////
	COM_SETTING comSetting;
	ConfigFile_GetNanoSpecIni(&comSetting, CONFIG_FILE_COM);

	_stprintf(szCommNo, _T("\\\\.\\%s"), comSetting.SrComPort.szCommNo);
	m_hComm = ::CreateFile(szCommNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( m_hComm == INVALID_HANDLE_VALUE ) {
		strBuffer.Format(pMojiretsu->LoadString(IDS_STRING01), comSetting.SrComPort.szCommNo);
		MyMessageBox(NULL, strBuffer, g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		m_hComm = NULL;
		return FALSE;
	}
	::SetupComm(m_hComm, 4096 * 4, 256);
	::GetCommState(m_hComm, &dcb);

	_stprintf(szBuff, _T("baud=%s parity=%s data=%s stop=%s"), comSetting.SrComPort.szBaudRate, comSetting.SrComPort.szParity, comSetting.SrComPort.szByteSize, comSetting.SrComPort.szStopBits);
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

	bySend = (BYTE)_TCHAR('/');
	if ( !::WriteFile(m_hComm, &bySend, 1, &dwNumberOfBytesWritten, NULL) ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING02), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) )
		return FALSE;
	if ( !::ReadFile(m_hComm, &byRecv, 1, &dwNumberOfBytesRead, NULL) ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING03), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	if ( byRecv != bySend || dwNumberOfBytesRead != 1 ) {
		MyMessageBox(NULL, pMojiretsu->LoadString(IDS_STRING04), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	_stprintf(szSendBuff, _T("%s%s"), INITIL, _T("5")); 			// "Q5"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;
	if ( _tcsncmp(&szRecvBuff[5], _T("00"), 2) != 0 ) {
		if ( _tcsncmp(&szRecvBuff[5], _T("04"), 2) == 0 ) {
			nID = IDS_STRING11;
		}
		else if ( _tcsncmp(&szRecvBuff[5], _T("06"), 2) == 0 ) {
			nID = IDS_STRING12;
		}
		else {
			nID = IDS_STRING13;
		}
		MyMessageBox(NULL, pMojiretsu->LoadString(nID), g_szMsgBoxCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	_stprintf(szSendBuff, _T("%s"), RAMCHK);						// "S"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	_stprintf(szSendBuff, _T("%s"), PRCHK); 						// "T"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	_stprintf(szSendBuff, _T("%s%02d"), _T("i"), iCcdDataTransferModeBinary);	// "i00" Ascii, "i01" Binary
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	m_bTransferModeBinary = ( iCcdDataTransferModeBinary == 1 );

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CSrHeadSerial::ExitInstance()
{
	TRACE(_T("CSrHeadSerial::ExitInstance()\n"));

	if ( m_hComm ) {
		::CloseHandle(m_hComm);
		m_hComm = NULL;
	}
}

//---------------------------------------------------------------------------
// GetVersion
BOOL CSrHeadSerial::GetVersion(LPTSTR pszVersion)
{
	TRACE(_T("CSrHeadSerial::GetVersion()\n"));

	TCHAR szSendBuff[RS232C_SEND_CHAR];
	TCHAR szRecvBuff[RS232C_RECV_CHAR];

	_stprintf(szSendBuff, _T("%s"), SENDPN);						// "R"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	_tcscpy(pszVersion, &szRecvBuff[5]);

	return TRUE;
}

//---------------------------------------------------------------------------
// CcdScan
BOOL CSrHeadSerial::CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans/*=1*/, BOOL bProhibitNotify/*=FALSE*/)	// ccdScanData[iPixels]mۂĂ邱
{
	TRACE(_T("CSrHeadSerial::CcdScan()\n"));

	// 232C̃XL񐔂́A1̂ݑΉi{[h̓]f[^16rbgŒ\̎dlȂ̂ŁBj
	// Ƀ{[hɃXL1ȊÕp[^nĂAp[^G[ŕԋpĂ͂
	ASSERT( iScans == 1 );

	// sAŏ̏蒼B1{gC2܂Ŏ{
	//	G o  T o ix֑Jځj̒úAcontinuegp

	TCHAR szSendBuff[RS232C_SEND_CHAR];
	TCHAR szRecvBuff[RS232C_RECV_CHAR];
	BYTE byRecv;
	DWORD dwNumberOfBytesRead;
	BOOL bError;
	int iPixIdx;
	int iCcdData;
	LPTSTR token;

	const int MAXTIMES = 3;
	int iTry;

	for ( iTry = 0; iTry < MAXTIMES; iTry++ ) {
		_stprintf(szSendBuff, _T("%s%04X%s%02X"), CCDSCN, iExposure, _T(","), iScans);		// for example. "L04B0,01"
		if ( !SendCommand(szSendBuff) ) {
//			return FALSE;
			goto CleanUp;
		}

		if ( m_bTransferModeBinary ) {
			bError = FALSE;
			int i;

			for ( i = 0; i < 4; i++ ) { 												// for example. "L,00yyzz..."
				if ( !WaitForRecvData(RS232C_RECV_INTEG_TIMEOUT) ) {
					bError = TRUE;
					break;
				}
				if ( !::ReadFile(m_hComm, &szRecvBuff[i], 1, &dwNumberOfBytesRead, NULL) ) {
					bError = TRUE;
					break;
				}
			}
			if ( bError )
				continue;		// Error -> Retry
			szRecvBuff[i] = _TCHAR('\0');
			if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
				continue;		// Error -> Retry

// @@@ Ή
// 탊^[R[hMAbZ[Wʒm
			if ( !bProhibitNotify ) {
				if ( g_hNotifyWnd )
					::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
			}

			for ( iPixIdx = 0; iPixIdx < iPixels; iPixIdx++ ) {
				if ( (iCcdData = GetCcdScanData1By1()) != -1 )
					ccdScanData[iPixIdx] = iCcdData;
				else
					break;
			}
			if ( iCcdData == -1 )
// @@@			continue;		// Error -> Retry
//				return FALSE;
				goto CleanUp;

			if ( !WaitForRecvData(RS232C_RECV_INTEG_TIMEOUT) )
// @@@			continue;		// Error -> Retry
//				return FALSE;
				goto CleanUp;
			if ( !::ReadFile(m_hComm, &byRecv, 1, &dwNumberOfBytesRead, NULL) )
// @@@			continue;		// Error -> Retry
//				return FALSE;
				goto CleanUp;
			if ( byRecv != _TCHAR('\r') )
// @@@			continue;		// Error -> Retry
//				return FALSE;
				goto CleanUp;
		}
		else {
			if ( !RecvData(szRecvBuff) )													// for example. "L,00,yyy1,yyy2, , ,yyyn"
				continue;		// Error -> Retry
			if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
				continue;		// Error -> Retry

			iPixIdx = 0;
			token = _tcstok(&szRecvBuff[5], _T(","));
			while ( token )
			{
				if ( (iCcdData = StringHexToint(token, _tcslen(token))) != -1 ) {
					if ( iPixIdx < iPixels ) {
						ccdScanData[iPixIdx] = iCcdData;
						iPixIdx++;
						token = _tcstok(NULL, _T(","));
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			if ( iCcdData == -1 )
				continue;		// Error -> Retry
			if ( iPixIdx != iPixels )
				continue;		// Error -> Retry
			if ( token != NULL )
				continue;		// Error -> Retry
		}

		return TRUE;		// Normal End
	}

CleanUp:

	COMSTAT stat;
	DWORD dwErrorFlags;

	::ClearCommError(m_hComm, &dwErrorFlags, &stat);
	::PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR);
	::PurgeComm(m_hComm, PURGE_RXABORT | PURGE_RXCLEAR);

	_stprintf(szSendBuff, _T("%s%s"), INITIL, _T("5")); 			// "Q5"
	SendCommand(szSendBuff);
	RecvData(szRecvBuff);

	return FALSE;
}

//---------------------------------------------------------------------------
// ChangeCcdShutter
BOOL CSrHeadSerial::ChangeCcdShutter(int iPos)
{
	TRACE(_T("CSrHeadSerial::ChangeCcdShutter()\n"));

	static int iCurrentPos = -1;
	TCHAR szSendBuff[RS232C_SEND_CHAR];
	TCHAR szRecvBuff[RS232C_RECV_CHAR];

	if ( iPos == iCurrentPos )
		return TRUE;
	_stprintf(szSendBuff, _T("%s%02X"), GOSHUT, iPos);				// for example. "Z01"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;
	iCurrentPos = iPos;
	return TRUE;
}

//---------------------------------------------------------------------------
// OpenUvShutter
BOOL CSrHeadSerial::OpenUvShutter()
{
	TRACE(_T("CSrHeadSerial::OpenUvShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeUvShutter(UV_OPEN);
}

//---------------------------------------------------------------------------
// CloseUvShutter
BOOL CSrHeadSerial::CloseUvShutter()
{
	TRACE(_T("CSrHeadSerial::CloseUvShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeUvShutter(UV_CLOSE);
}

//---------------------------------------------------------------------------
// OpenTransShutter
BOOL CSrHeadSerial::OpenTransShutter()
{
	TRACE(_T("CSrHeadSerial::OpenTransShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeTransShutter(TRANS_OPEN);
}

//---------------------------------------------------------------------------
// CloseTransShutter
BOOL CSrHeadSerial::CloseTransShutter()
{
	TRACE(_T("CSrHeadSerial::CloseTransShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeTransShutter(TRANS_CLOSE);
}

//---------------------------------------------------------------------------
// ChangeUvShutter
BOOL CSrHeadSerial::ChangeUvShutter(int iPos)
{
	static int iCurrentPos = -1;
	TCHAR szSendBuff[RS232C_SEND_CHAR];
	TCHAR szRecvBuff[RS232C_RECV_CHAR];

	if ( iPos == iCurrentPos )
		return TRUE;
	_stprintf(szSendBuff, _T("%s%02X"), SETCHP, iPos);				// for example. "G00"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;
	iCurrentPos = iPos;
	return TRUE;
}

//---------------------------------------------------------------------------
// ChangeTransShutter
BOOL CSrHeadSerial::ChangeTransShutter(int iPos)
{
	static int iCurrentPos = -1;
	TCHAR szSendBuff[RS232C_SEND_CHAR];
	TCHAR szRecvBuff[RS232C_RECV_CHAR];
	BYTE byData;

	if ( iPos == iCurrentPos )
		return TRUE;
	_stprintf(szSendBuff, _T("%s%s"), RDPORT, TRANSSHUTTER_P);							// "W45"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	if ( (byData = (BYTE)StringHexToint(&szRecvBuff[5], 2)) == -1 )
		return FALSE;
	if ( iPos )
		byData |= TRANSSHUTTER_B;
	else
		byData &= ~TRANSSHUTTER_B;

	_stprintf(szSendBuff, _T("%s%s%s%02X"), WRPORT, TRANSSHUTTER_P, _T(","), byData);	// for example. "X45,11"
	if ( !SendCommand(szSendBuff) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;
	iCurrentPos = iPos;
	return TRUE;
}

//---------------------------------------------------------------------------
// SendCommand
BOOL CSrHeadSerial::SendCommand(LPCTSTR pszCmd)
{
	TRACE(_T("CSrHeadSerial::SendCommand()\n"));
	TRACE1(_T("%s\n"), pszCmd);

	BOOL bError;
	BYTE byRecv;
	BYTE bySend;
	DWORD dwNumberOfBytesWritten, dwNumberOfBytesRead;

	const int MAXTIMES = 3;
	int iTry;

	for ( iTry = 0; iTry < MAXTIMES; iTry++ ) {
		bError = FALSE;
		int i;

		for ( i = 0; pszCmd[i]; i++ ) {
			if ( !::WriteFile(m_hComm, &pszCmd[i], 1, &dwNumberOfBytesWritten, NULL) ) {
				bError = TRUE;
				break;
			}
			if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) ) {
				bError = TRUE;
				break;
			}
			if ( !::ReadFile(m_hComm, &byRecv, 1, &dwNumberOfBytesRead, NULL) ) {
				bError = TRUE;
				break;
			}
			if ( pszCmd[i] != byRecv || dwNumberOfBytesRead != 1 ) {
				bError = TRUE;
				break;
			}
		}
		if ( bError )
			continue;		// Error -> Retry

		bySend = (BYTE)_TCHAR('\r');
		if ( !::WriteFile(m_hComm, &bySend, 1, &dwNumberOfBytesWritten, NULL) ) // I[CR̐ݒ
			continue;		// Error -> Retry
		if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) )
			continue;		// Error -> Retry
		if ( !::ReadFile(m_hComm, &byRecv, 1, &dwNumberOfBytesRead, NULL) )
			continue;		// Error -> Retry
		if ( byRecv != bySend || dwNumberOfBytesRead != 1 )
			continue;		// Error -> Retry
		return TRUE;		// Normal End
	}
	return FALSE;
}

//---------------------------------------------------------------------------
// WaitForRecvData
BOOL CSrHeadSerial::WaitForRecvData(int iTimeoutSec)
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
BOOL CSrHeadSerial::RecvData(LPTSTR pszData)
{
	TRACE(_T("CSrHeadSerial::RecvData()\n"));

	TCHAR szRecvBuff[RS232C_RECV_CHAR];
	DWORD dwNumberOfBytesRead;

	int i;


	for ( i = 0; i < RS232C_RECV_CHAR; i++ ) {
		if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) )
			return FALSE;
		if ( !::ReadFile(m_hComm, &szRecvBuff[i], 1, &dwNumberOfBytesRead, NULL) )
			return FALSE;
		if ( szRecvBuff[i] == _TCHAR('\r') ) {
			szRecvBuff[i] = NULL;									// I[CRNULLɒu
			break;
		}
	}
	if ( i == RS232C_RECV_CHAR ) {									// I[CRȂ
		return FALSE;
	}
	_tcscpy(pszData, szRecvBuff);

	if ( _tcslen(szRecvBuff) > 511 )
		szRecvBuff[511] = _TCHAR('\0');
	TRACE1(_T("%s\n"), szRecvBuff);

	return TRUE;
}

//---------------------------------------------------------------------------
// CheckCmdAndStatusCode
BOOL CSrHeadSerial::CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData)
{
	return (( pszRecvData[0] == pszSendCmd[0] ) && ( _tcsncmp(&pszRecvData[2], _T("00"), 2) == 0 ));
}

//---------------------------------------------------------------------------
// GetCcdScanData1By1
int CSrHeadSerial::GetCcdScanData1By1()
{
	// CCD 1hbǵA2oCgi16rbgjPʂŃf[^MB
	// VAʐMɂāAXL񐔂1ŒȂ̂ŁAő65535i16rbgMAXjƂ

	int iCcdData = 0;
	BYTE byData;
	DWORD dwNumberOfBytesRead;

	int i;


	for ( i = 2; i > 0; i-- ) {
		if ( !WaitForRecvData(RS232C_RECV_TIMEOUT) )
			return -1;
		if ( !::ReadFile(m_hComm, &byData, 1, &dwNumberOfBytesRead, NULL) )
			return -1;
		iCcdData += byData << (8 * (i - 1));						// byData  (8bit * (i - 1))VtgĉZ
	}
	return iCcdData;
}

//---------------------------------------------------------------------------
// StringHexToint
//	 -1 : ϊs\isw肳ꂽj
//	 xx : int^l
int CSrHeadSerial::StringHexToint(LPCTSTR psz, size_t count)
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
