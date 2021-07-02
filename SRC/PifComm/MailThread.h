#if !defined(AFX_MAILTHREAD_H__9AF210DD_A3A8_4DBC_85B8_9FBCEEFF8BFE__INCLUDED_)
#define AFX_MAILTHREAD_H__9AF210DD_A3A8_4DBC_85B8_9FBCEEFF8BFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MailThread.h : header file
//

#include "Mailslot.h"

class CPifMessage;

/////////////////////////////////////////////////////////////////////////////
// CMailThread thread

class CMailThread : public CWinThread
{
	DECLARE_DYNCREATE(CMailThread)
protected:
	CMailThread();			 // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual ~CMailThread();
	void ShutDown();
	void DoStateDone(int iAckCode);

	static void AddSendMessage(LPCTSTR psz)
	{
//		TRACE(_T("CMailThread::AddSendMessage(LPCTSTR psz)\n"));

		::EnterCriticalSection(&m_csWait);

		m_sendMessageList.AddTail(psz);

		::LeaveCriticalSection(&m_csWait);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMailThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	enum { STATE_RECV, STATE_PROC, STATE_WAIT, STATE_DONE };

	BOOL PopSendMessageList(CString& str)
	{
//		TRACE(_T("CMailThread::PopSendMessageList(CString& str)\n"));

		BOOL bRet = FALSE;

		::EnterCriticalSection(&m_csWait);

		if ( !m_sendMessageList.IsEmpty() ) {
			str = m_sendMessageList.GetHead();
			m_sendMessageList.RemoveHead();
			bRet = TRUE;
		}
		else {
			str.Empty();
		}

		::LeaveCriticalSection(&m_csWait);

		return bRet;
	}

	void EventProcess(LPCTSTR pszRecvCommandId);
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
	void EventProcessP105();
	void EventProcessP106();
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
	void EventProcessP201();
	void EventProcessP302();
	void EventProcessP303();
	void EventProcessP306();
	void EventProcessP308();
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
	void EventProcessP310();
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--
	void EventProcessP401();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
	void EventProcessP411();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--
	void EventProcessP501();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
	void EventProcessP511();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--
// 2014.01.07 bagus Add(Stage None対応) -->
	void EventProcessP514();
	void EventProcessP516();
// 2014.01.07 bagus Add(Stage None対応) <--
	void EventProcessP601();
	void EventProcessP602();
	void EventProcessP603();
	void EventProcessP604();
	void EventProcessP605();
	void EventProcessP606();
	void EventProcessP607();
	void EventProcessP608();
	void EventProcessP609();
	void EventProcessP610();
	void EventProcessP611();

	// Generated message map functions
	//{{AFX_MSG(CMailThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	static CRITICAL_SECTION m_csWait;
	static CStringList m_sendMessageList;

	CMailslot m_mailslot;
	BOOL m_bShutDown;

	CString m_strMailslotSendName;
	CString m_strSendCommandId;
	int m_iState;
	int m_iAckCode;
	CPifMessage* m_pEventCurr;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAILTHREAD_H__9AF210DD_A3A8_4DBC_85B8_9FBCEEFF8BFE__INCLUDED_)
