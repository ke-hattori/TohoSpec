// NanoPifSockWnd.h : header file
//

#pragma once

#include "PifSockSocket.h"
#include "EventThread.h"

/////////////////////////////////////////////////////////////////////////////
// CNanoPifSockWnd dialog

class CNanoPifSockWnd : public CWnd
{
// Construction
public:
	CNanoPifSockWnd();	// standard constructor

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNanoPifSockWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CNanoPifSockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnConnected(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisConnected(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	CWinSockLstn<CPifSockSocket>* m_pLstn;
	CWinSockSocket* m_pSock;
	CEventThread* m_pEventThread;
};
