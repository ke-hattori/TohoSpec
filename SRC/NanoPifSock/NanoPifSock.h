// NanoPifSock.h : main header file for the NANOPIFSOCK application
//

#if !defined(AFX_NANOPIFSOCK_H__AC8FD394_AF18_455A_BCFE_803C78C594EE__INCLUDED_)
#define AFX_NANOPIFSOCK_H__AC8FD394_AF18_455A_BCFE_803C78C594EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockApp:
// See NanoPifSock.cpp for the implementation of this class
//

class CNanoPifSockApp : public CWinApp
{
public:
	CNanoPifSockApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNanoPifSockApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNanoPifSockApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NANOPIFSOCK_H__AC8FD394_AF18_455A_BCFE_803C78C594EE__INCLUDED_)
