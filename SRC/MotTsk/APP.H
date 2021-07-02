// app.h : Defines the interface of the application class
/****************************************************************
 *		Copyright (C) 2003 by Tencor Instruments
 *
 *		Source Upgrade From VC1.52 to VC6.0
 *
 *		Updated by Ouyang-jc
 *
 *		Date:	2003/03/26
 ****************************************************************/

#ifndef APP_H
#define APP_H


/////////////////////////////////////////////////////////////////////////////
// CMotTskApp:
// This class represents the application as a whole.  The base class CWinApp
// does most of this for us, such as creating and running the message loop.
//
class CMotTskApp : public CWinApp
{
public:
   // This member function is called automatically, so we can set up
   // our application-specific information.
   //
	BOOL InitInstance ();
	int ExitInstance();

private:
	HANDLE m_hMutex_Run;

};

/////////////////////////////////////////////////////////////////////////////

#endif
