// SrHeadParallel.cpp : Defines the SrHeadParallel routines.
//

#include "stdafx.h"
#include "SrHead.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\MeaSys.hxx"
#include "resource.h"
#include "Mojiretsu.h"
// 2013.11.07 Bagus Mod (TohoSpecΉ) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpecΉ) <--

#define DUALPORTRAM_TIMEOUT 		(5)
#define DUALPORTRAM_INTEG_TIMEOUT	(45)

// Position
#define DP_RAM_ECHO_FLAG		(01)
#define DP_RAM_SINC_FLAG		(02)
#define DP_RAM_SEND_BUFFER		(16) //   16 to  510( 494) to PCB(Send) Buffers
#define DP_RAM_IN1_P		   (510)
#define DP_RAM_IN2_P		   (511)
#define DP_RAM_RCV_BUFFER	   (512) //  512 to  896( 384) from PCB(Recieve) Buffers
#define RS_BUFF0			   (896)
#define DP_RAM_OUT1_P		  (1535)
#define DP_232C1_RCV_BUFFER   (1536) // 1536 to 1663( 128) 232C1 from PCB(Send) Buffers
#define DP_232C1_SND_BUFFER   (1664) // 1664 to 1791( 128) 232C1 to PCB(Send) Buffers
#define DP_232C2_RCV_BUFFER   (1792) // 1792 to 1919( 128) 232C2 from PCB(Send) Buffers
#define DP_232C2_SND_BUFFER   (1920) // 1920 to 2047( 128) 232C2 to PCB(Send) Buffers

#define DP_RAM_MAX_SEND_CHAR  (DP_RAM_IN1_P - DP_RAM_SEND_BUFFER)
#define DP_RAM_MAX_RCV_CHAR   (RS_BUFF0 - DP_RAM_RCV_BUFFER)

//	R}h
#define SETCHP			   _T("G")		 // Set UV shutter position
#define CCDSCN			   _T("L")		 // CCD Scan
#define INITIL			   _T("Q")		 // Initialize & Calibrate Wavelength
#define SENDPN			   _T("R")		 // Send Software Part Number			   //get version
#define RAMCHK			   _T("S")		 // RAM Check
#define PRCHK			   _T("T")		 // PROM Check
#define GOSHUT			   _T("Z")		 // GO CCD SHUTTER POSITION

#define DP_ECHO_BACK	   _T("a")
#define FIFO_ECHO_BACK	   _T("b")

#define TRANSSHUTTER_P	  DP_RAM_OUT1_P  // Transmission Light
#define TRANSSHUTTER_B	  (0x20)

extern HWND g_hNotifyWnd;
extern CMojiretsu* pMojiretsu;

static char g_szMsgBoxCaption[256];

//---------------------------------------------------------------------------
// InitInstance
BOOL CSrHeadParallel::InitInstance()
{
	TRACE(_T("CSrHeadParallel::InitInstance()\n"));

	UINT nID;
	TCHAR szSendBuff[DP_RAM_MAX_SEND_CHAR];
	TCHAR szRecvBuff[DP_RAM_MAX_RCV_CHAR];

	strcpy(g_szMsgBoxCaption, "NanoSpec");
	if(g_lAppNameType != APP_NAME_NANO){
		strncpy(g_szMsgBoxCaption, g_lpszAppPrefix4[g_lAppNameType], 4);
	}

	if ( dpram::_init() != IOBASE_ERR_NONE )
		return FALSE;

	DualPortRam_Write(DP_232C2_SND_BUFFER + 127, 170);
	if ( DualPortRam_Read(DP_232C2_SND_BUFFER + 127) != 170 )
		return FALSE;
	DualPortRam_Write(DP_232C2_SND_BUFFER + 127, 85);
	if ( DualPortRam_Read(DP_232C2_SND_BUFFER + 127) != 85 )
		return FALSE;
	DualPortRam_Write(DP_232C2_SND_BUFFER + 127, 0);
	if ( DualPortRam_Read(DP_232C2_SND_BUFFER + 127) != 0 )
		return FALSE;

	_stprintf(szSendBuff, _T("%s%s"), DP_ECHO_BACK, _T("01,A"));	// "a01,A"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )									// "a,00,A"Młΐ
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;
	if ( szRecvBuff[5] != _TCHAR('A') )
		return FALSE;

	_stprintf(szSendBuff, _T("%s%s"), FIFO_ECHO_BACK, _T("01,A"));	// "b01,A"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )									// "b,00"Młΐ
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;
	if ( FifoRead() != _TCHAR('A') )
		return FALSE;

	_stprintf(szSendBuff, _T("%s%s"), INITIL, _T("5")); 			// "Q5"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
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
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	_stprintf(szSendBuff, _T("%s"), PRCHK); 						// "T"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
		return FALSE;
	if ( !RecvData(szRecvBuff) )
		return FALSE;
	if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
		return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CSrHeadParallel::ExitInstance()
{
	TRACE(_T("CSrHeadParallel::ExitInstance()\n"));
}

//---------------------------------------------------------------------------
// GetVersion
BOOL CSrHeadParallel::GetVersion(LPTSTR pszVersion)
{
	TRACE(_T("CSrHeadParallel::GetVersion()\n"));

	TCHAR szSendBuff[DP_RAM_MAX_SEND_CHAR];
	TCHAR szRecvBuff[DP_RAM_MAX_RCV_CHAR];

	_stprintf(szSendBuff, _T("%s"), SENDPN);						 // "R"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
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
BOOL CSrHeadParallel::CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans/*=1*/, BOOL bProhibitNotify/*=FALSE*/)	  // ccdScanData[iPixels]mۂĂ邱
{
	TRACE(_T("CSrHeadParallel::CcdScan()\n"));

	// sAŏ̏蒼B1{gC2܂Ŏ{
	//	G o  T o ix֑Jځj̒úAcontinuegp

	TCHAR szSendBuff[DP_RAM_MAX_SEND_CHAR];
	TCHAR szRecvBuff[DP_RAM_MAX_RCV_CHAR];
	int iCcdData;

	const int MAXTIMES = 3;
	int iTry;

	for ( iTry = 0; iTry < MAXTIMES; iTry++ ) {
		_stprintf(szSendBuff, _T("%s%04X%s%02X"), CCDSCN, iExposure, _T(","), iScans);	  // for example. "L04B0,01"
// added 20111110 hmenjo {
		_fifo_reset();
// added 20111110 hmenjo }
		SendCommand(szSendBuff);
		if ( !WaitForStatusChange(DP_RCVING, DUALPORTRAM_INTEG_TIMEOUT) )
			continue;		// Error -> Retry
		int iPixIdx;

		for ( iPixIdx = 0; iPixIdx < iPixels; iPixIdx++ ) {
			if ( (iCcdData = GetCcdScanData1By1(DUALPORTRAM_TIMEOUT)) != IOBASE_ERR_INVALID_ACCESS )
				ccdScanData[iPixIdx] = iCcdData;
			else
				break;
		}
		if ( iCcdData == IOBASE_ERR_INVALID_ACCESS )
			continue;		// Error -> Retry
		if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
			continue;		// Error -> Retry
		if ( !RecvData(szRecvBuff) )
			continue;		// Error -> Retry
		if ( !CheckCmdAndStatusCode(szSendBuff, szRecvBuff) )
			continue;		// Error -> Retry

		if ( !bProhibitNotify ) {
			if ( g_hNotifyWnd )
				::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
		}

		return TRUE;		// Normal End
	}
	return FALSE;
}

//---------------------------------------------------------------------------
// ChangeCcdShutter
BOOL CSrHeadParallel::ChangeCcdShutter(int iPos)
{
	TRACE(_T("CSrHeadParallel::ChangeCcdShutter()\n"));

	static int iCurrentPos = -1;
	TCHAR szSendBuff[DP_RAM_MAX_SEND_CHAR];
	TCHAR szRecvBuff[DP_RAM_MAX_RCV_CHAR];

	if ( iPos == iCurrentPos )
		return TRUE;
	_stprintf(szSendBuff, _T("%s%02X"), GOSHUT, iPos);				// for example. "Z01"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
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
BOOL CSrHeadParallel::OpenUvShutter()
{
	TRACE(_T("CSrHeadParallel::OpenUvShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeUvShutter(UV_OPEN);
}

//---------------------------------------------------------------------------
// CloseUvShutter
BOOL CSrHeadParallel::CloseUvShutter()
{
	TRACE(_T("CSrHeadParallel::CloseUvShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeUvShutter(UV_CLOSE);
}

//---------------------------------------------------------------------------
// OpenTransShutter
BOOL CSrHeadParallel::OpenTransShutter()
{
	TRACE(_T("CSrHeadParallel::OpenTransShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeTransShutter(TRANS_OPEN);
}

//---------------------------------------------------------------------------
// CloseTransShutter
BOOL CSrHeadParallel::CloseTransShutter()
{
	TRACE(_T("CSrHeadParallel::CloseTransShutter()\n"));

	// Vb^[̓슮񍐂͂܂̂
	// ĂьŁA^C}[fBCĂ
	return ChangeTransShutter(TRANS_CLOSE);
}

//---------------------------------------------------------------------------
// DualPortRam_Read
unsigned short CSrHeadParallel::DualPortRam_Read(unsigned short port)
{
	return _getch(port);
}

//---------------------------------------------------------------------------
// DualPortRam_Write
void CSrHeadParallel::DualPortRam_Write(unsigned short port, unsigned short dataword)
{
	_putch(port, dataword);
}

//---------------------------------------------------------------------------
// GetStatus
unsigned short CSrHeadParallel::GetStatus()
{
	return DualPortRam_Read(DP_RAM_SINC_FLAG);
}

//---------------------------------------------------------------------------
// SetStatus
void CSrHeadParallel::SetStatus(WORD wStatus)
{
	DualPortRam_Write(DP_RAM_SINC_FLAG, wStatus);
}

//---------------------------------------------------------------------------
// FifoRead
signed short CSrHeadParallel::FifoRead()
{
	return _fifo();
}

//---------------------------------------------------------------------------
// GetCcdScanData1By1
int CSrHeadParallel::GetCcdScanData1By1(int iTimeoutSec)
{
	// CCD 1hbǵA3oCgi24rbgjPʂŃf[^MB
	// XL1̎MAXĺA65535i16rbgMAXj
	// XL2,4,8̎́Aꂼ񐔕ZlMAXƂȂ̂
	// 24rbgGApӂĂ

	int iCcdData = 0;
	int iFifoRet;

	CTimer timer;
	timer.Restart(iTimeoutSec);
	int i;

	for ( i = 0; i < 3; i++ ) {
		while ( (iFifoRet = FifoRead()) == IOBASE_ERR_INVALID_ACCESS ) {   // f[^擾łȂ^C~O
			if ( timer.IsTimeout() )
				return IOBASE_ERR_INVALID_ACCESS;
		}
		iCcdData += iFifoRet << (8 * i);						// iFifoRet  (8bit * i)VtgĉZ
		timer.Restart(iTimeoutSec);
	}
	return iCcdData;
}

//---------------------------------------------------------------------------
// ChangeUvShutter
BOOL CSrHeadParallel::ChangeUvShutter(int iPos)
{
	static int iCurrentPos = -1;
	TCHAR szSendBuff[DP_RAM_MAX_SEND_CHAR];
	TCHAR szRecvBuff[DP_RAM_MAX_RCV_CHAR];

	if ( iPos == iCurrentPos )
		return TRUE;
	_stprintf(szSendBuff, _T("%s%02X"), SETCHP, iPos);			// for example. "G00"
	SendCommand(szSendBuff);
	if ( !WaitForStatusChange(DP_COMPLETE, DUALPORTRAM_TIMEOUT) )
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
BOOL CSrHeadParallel::ChangeTransShutter(int iPos)
{
	static int iCurrentPos = -1;

	if ( iPos == iCurrentPos )
		return TRUE;
	WORD dt = DualPortRam_Read(TRANSSHUTTER_P);
	if ( iPos )
		dt |= TRANSSHUTTER_B;
	else
		dt &= ~TRANSSHUTTER_B;
	DualPortRam_Write(TRANSSHUTTER_P, dt);
	iCurrentPos = iPos;
	return TRUE;
}

//---------------------------------------------------------------------------
// SendCommand
void CSrHeadParallel::SendCommand(LPCTSTR pszCmd)
{
	TRACE(_T("CSrHeadParallel::SendCommand()\n"));
	TRACE1(_T("%s\n"), pszCmd);

	int i;


	for ( i = 0; pszCmd[i]; i++ ) {
		DualPortRam_Write(DP_RAM_SEND_BUFFER + i, pszCmd[i]);
	}
	DualPortRam_Write(DP_RAM_SEND_BUFFER + i, _TCHAR('\r'));	// I[CR̐ݒ
	SetStatus(DP_SEND);
}

//---------------------------------------------------------------------------
// WaitForStatusChange
BOOL CSrHeadParallel::WaitForStatusChange(WORD wStatus, int iTimeoutSec)
{
	CTimer timer;
	timer.Restart(iTimeoutSec);

	while ( GetStatus() != wStatus ) {
		if ( timer.IsTimeout() ) {
			SetStatus(DP_IDLE);
			return FALSE;
		}
		::Sleep(10);
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// RecvData
BOOL CSrHeadParallel::RecvData(LPTSTR pszData)
{
	TRACE(_T("CSrHeadParallel::RecvData()\n"));

	TCHAR szRecvBuff[DP_RAM_MAX_RCV_CHAR];
	int i;

	for ( i = 0; i < DP_RAM_MAX_RCV_CHAR; i++ ) {
		szRecvBuff[i] = (TCHAR)DualPortRam_Read(DP_RAM_RCV_BUFFER + i);
		if ( szRecvBuff[i] == _TCHAR('\r') ) {
			szRecvBuff[i] = NULL;								// I[CRNULLɒu
			break;
		}
	}
	if ( i == DP_RAM_MAX_RCV_CHAR ) {							// I[CRȂ
		SetStatus(DP_IDLE);
		return FALSE;
	}
	_tcscpy(pszData, szRecvBuff);

	TRACE1(_T("%s\n"), pszData);

	SetStatus(DP_IDLE);
	return TRUE;
}

//---------------------------------------------------------------------------
// CheckCmdAndStatusCode
BOOL CSrHeadParallel::CheckCmdAndStatusCode(LPCTSTR pszSendCmd, LPCTSTR pszRecvData)
{
	return (( pszRecvData[0] == pszSendCmd[0] ) && ( _tcsncmp(&pszRecvData[2], _T("00"), 2) == 0 ));
}
