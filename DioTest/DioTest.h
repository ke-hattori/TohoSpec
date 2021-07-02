// DioTest.h : main header file for the DIOTEST application
//

#if !defined(AFX_DIOTEST_H__F5F3FE24_3B1E_4C53_87AE_C1CF1679F60B__INCLUDED_)
#define AFX_DIOTEST_H__F5F3FE24_3B1E_4C53_87AE_C1CF1679F60B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDioTestApp:
// See DioTest.cpp for the implementation of this class
//

class CDioTestApp : public CWinApp
{
public:
	CDioTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDioTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDioTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIOTEST_H__F5F3FE24_3B1E_4C53_87AE_C1CF1679F60B__INCLUDED_)
