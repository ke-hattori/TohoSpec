//	System.h

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#ifndef _MASTER_
#define _EXTERN extern
#else
#define _EXTERN
#endif	// _MASTER_

#include "..\\..\\INC\\Globals.hxx"

/////////////////////////////////////////////////////////////////////////////
// Define																   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Variable 															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
// Application Name Type
// ==========================================================================
_EXTERN long	g_lAppNameType;


/////////////////////////////////////////////////////////////////////////////
// Function																   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ResourceLoadLibrary
#ifdef _MASTER_
HMODULE ResourceLoadLibrary(int langid, LPCTSTR lpszResourceName)
{
	TCHAR l_szResourceFullName[_MAX_FNAME];

	_tcscpy(l_szResourceFullName, lpszResourceName);
	if(langid == LANGUAGE_ENGLISH){
		_tcscat(l_szResourceFullName, "RCENU.dll");
	}
	else{
		_tcscat(l_szResourceFullName, "RCJPN.dll");
	}

	return ::LoadLibrary(l_szResourceFullName);
}
#else
extern HMODULE ResourceLoadLibrary(int langid, LPCTSTR lpszResourceName);
#endif

#endif	// _SYSTEM_H_
