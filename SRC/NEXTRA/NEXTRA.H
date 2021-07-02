// NEXTRA.h : main header file for the NEXTRA application
//

#if !defined(AFX_NEXTRA_H__0032DF48_0B84_47D0_A38C_0C51B107B257__INCLUDED_)
#define AFX_NEXTRA_H__0032DF48_0B84_47D0_A38C_0C51B107B257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <nexrc.hxx>
#include "systray.h"

// ===========================================================================
// CNextraApp:
// See NEXTRA.cpp for the implementation of this class
//
// ===========================================================================
class CNextraApp : public CWinApp
{
public:
	CNextraApp();

	CSystemTray m_TrayIcon;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNextraApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	inline void OperationLogging (LPCSTR lpszText)
	{
		m_lfs << arrange
			  << datetime
			  << lpszText
			  << std::endl;
		m_lfs.close();
	}

	inline BOOL IsHostOption (void) const { return m_cmdInfo.IsHostOption(); }

	//{{AFX_MSG(CNextraApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	class CNexCommandLineInfo : public CCommandLineInfo
	{
	public:
		CNexCommandLineInfo () : m_bHostOption(FALSE) {}
		void ParseParam (const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
		{
			CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
			if ( bFlag ) {
				CString strParam(pszParam);
				if ( strParam == TEXT("H") ) {
					m_bHostOption = TRUE;
				}
			}
		}
		inline BOOL IsHostOption (void) const { return m_bHostOption; }
	private:
		BOOL m_bHostOption;
	} m_cmdInfo;

	lfstream	 m_lfs;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEXTRA_H__0032DF48_0B84_47D0_A38C_0C51B107B257__INCLUDED_)
