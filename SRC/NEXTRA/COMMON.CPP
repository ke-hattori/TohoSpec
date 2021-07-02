#include "stdafx.h"
#include "common.hpp"
#include "alarm.h"
#include "nextra.h"

#ifdef _DEBUG
#pragma comment(lib, "dtnsnexif.lib")
#pragma comment(lib, "dtnsnexproc.lib")
#pragma comment(lib, "dtnsnexini.lib")
#pragma comment(lib, "dtnsnexio.lib")
#pragma comment(lib, "dtnsnexax.lib")
#pragma comment(lib, "dtnsnexui.lib")
#pragma comment(lib, "dtnsalarmif.lib")
#pragma comment(lib, "dtnshook.lib")
#else
#pragma comment(lib, "tnsnexif.lib")
#pragma comment(lib, "tnsnexproc.lib")
#pragma comment(lib, "tnsnexini.lib")
#pragma comment(lib, "tnsnexio.lib")
#pragma comment(lib, "tnsnexax.lib")
#pragma comment(lib, "tnsnexui.lib")
#pragma comment(lib, "tnsalarmif.lib")
#pragma comment(lib, "tnshook.lib")
#endif

BOOL CCursor::m_bShowWaitCursor = FALSE;
BOOL CCursor::m_bWaitCursor = FALSE;

// ---------------------------------------------------------------------------
void EnableControlGroup (CWnd *pParent, BOOL bEnable, int nIDs, ...)
{
	int nID = nIDs;
	HWND hWnd;
	va_list args;

	va_start(args, nIDs);
	while ( nID != -1 ) {
		hWnd = ::GetDlgItem(pParent->m_hWnd, nID);
		if ( hWnd == NULL ) {
			TRACE1("Error: no control with ID 0x%04X\n", nID);
			ASSERT(FALSE);
		}
		else	// don't enable null handles, dbwin complains
			::EnableWindow(hWnd, bEnable);
		nID = va_arg(args, int);
	}
};

// ---------------------------------------------------------------------------
void OperationLogging (LPCSTR lpszText)
{
	((CNextraApp*) AfxGetApp())->OperationLogging(lpszText);
}

// ---------------------------------------------------------------------------
void EquipmentStatusRun (BOOL bOn/*=ON*/)
{
	if ( bOn ) {
		if ( nexioIsMaintenanceSwitch() == ON ||
			 nexioIsEngineerMaintenanceSwitch() == ON ) {
			bOn = OFF;
		}
	}
	nexioEquipmentStatusRun(bOn);
}

// ---------------------------------------------------------------------------
int MyMessageBox (HWND hWnd, UINT nIDText, UINT nIDCaption, UINT nType)
{
	int iRet = 0;

	CString strMsgTitle, strMsgText;
	strMsgTitle.LoadString(nIDText);
	strMsgText.LoadString(nIDCaption);
	if ( !strMsgTitle.IsEmpty() && !strMsgTitle.IsEmpty() )
		iRet = nexuiMessageBox( hWnd, strMsgText, strMsgTitle, nType );

	return iRet;
}
