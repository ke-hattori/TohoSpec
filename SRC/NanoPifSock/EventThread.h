#if !defined(AFX_EVENTTHREAD_H__2055DAB8_719F_408A_99B9_EBE08B7672D3__INCLUDED_)
#define AFX_EVENTTHREAD_H__2055DAB8_719F_408A_99B9_EBE08B7672D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventThread.h : header file
//

#pragma warning (disable:4786)

#include "stdafx.h"
#include "PifComm.hxx"
#include "Pifmessage.h"
#include "Mailslot.h"
#include <list>

/////////////////////////////////////////////////////////////////////////////
// CEventThread thread

class CEventThread : public CWinThread
{
	DECLARE_DYNCREATE(CEventThread)
protected:
	CEventThread(); 		  // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual ~CEventThread();
	void ShutDown();

	static void AddEvent(LPCTSTR psz, size_t count)
	{
		::EnterCriticalSection(&m_csWait);

		CPifMessage* pPifMessage = new CPifMessage();
		pPifMessage->Recv(psz, count);
		m_eventList.push_back(pPifMessage);

		::LeaveCriticalSection(&m_csWait);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EventProcessSxxxDefault(LPCTSTR psz);
	void EventProcessP901();
	void EventProcessP902();
	void EventProcessP903();
	void EventProcessP904();
	void UndefinedEventProcess();
	CString MakeLogText(LPCTSTR pszDir, LPCTSTR pszMsg);

	BOOL GetEvent()
	{
		BOOL bRet = FALSE;

		::EnterCriticalSection(&m_csWait);

		if ( m_eventList.size() > 0 ) {
			m_pEventCurr = m_eventList.front();
			m_eventList.pop_front();
			m_bProcess = TRUE;
			bRet = TRUE;
		}

		::LeaveCriticalSection(&m_csWait);

		return bRet;
	}

	BOOL DeleteEvent()
	{
		BOOL bRet = FALSE;

		if ( m_pEventCurr ) {
			delete m_pEventCurr;
			m_pEventCurr = NULL;
			m_bProcess = FALSE;
			bRet = TRUE;
		}
		return bRet;
	}

	// Generated message map functions
	//{{AFX_MSG(CEventThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	static CRITICAL_SECTION m_csWait;
	static std::list<CPifMessage*> m_eventList;

	BOOL m_bShutDown;
	BOOL m_bProcess;
	CPifMessage* m_pEventCurr;
	CMailslot m_mailslot;
	CString m_strMailslotServer;
	CString m_strMailslotClient;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTTHREAD_H__2055DAB8_719F_408A_99B9_EBE08B7672D3__INCLUDED_)
