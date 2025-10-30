// SeHeadDesktop.cpp : Defines the SeHeadDesktop routines.
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include "SeHead.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\MeaSys.hxx"

extern HWND g_hNotifyWnd;

//---------------------------------------------------------------------------
// InitInstance
BOOL CSeHeadDesktop::InitInstance()
{
	TRACE(_T("CSeHeadDesktop::InitInstance()\n"));

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CSeHeadDesktop::ExitInstance()
{
	TRACE(_T("CSeHeadDesktop::ExitInstance()\n"));

}

//---------------------------------------------------------------------------
// GetVersion
BOOL CSeHeadDesktop::GetVersion(LPTSTR pszVersion)
{
	_tcscpy(pszVersion, _T("SeHead Simulation"));

	return TRUE;
}

//---------------------------------------------------------------------------
// Scan
BOOL CSeHeadDesktop::Scan(LPCTSTR pszSaveFilePath, int iNumScans /*=50*/, BOOL bHighAccuracy /*=TRUE*/)
{
	if ( g_hNotifyWnd )
		::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);

	return TRUE;
}

//---------------------------------------------------------------------------
// OpenShutter
BOOL CSeHeadDesktop::OpenShutter()
{
	Sleep(500);

	return TRUE;
}

//---------------------------------------------------------------------------
// OpenUvShutter
BOOL CSeHeadDesktop::CloseShutter()
{
	Sleep(500);

	return TRUE;
}

//---------------------------------------------------------------------------
// MovePol
BOOL CSeHeadDesktop::MovePol(int iPolPos)
{
	return TRUE;
}
