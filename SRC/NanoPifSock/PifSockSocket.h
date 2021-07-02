// PifSockSocket.h : header file
//

#pragma once

#include "stdafx.h"
#include "WinSockSocket.h"
#include "LogFile.h"

// データ送受信用ソケット
class CPifSockSocket : public CWinSockSocket
{
public:
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	CPifSockSocket(CWnd* parent) : CWinSockSocket(parent), m_bRequProcess(FALSE), m_pifSockLogFile("NanoPifSock")
	CPifSockSocket(CWnd* parent, LPCTSTR pszLogFilename) : CWinSockSocket(parent), m_bRequProcess(FALSE), m_pifSockLogFile(pszLogFilename)
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	{
		TRACE(_T("CPifSockSocket::CPifSockSocket(CWnd* parent) constructor\n"));

		::InitializeCriticalSection(&m_csWait);

		::EnterCriticalSection(&m_csWait);

		m_respMessageList.RemoveAll();
		m_requMessageList.RemoveAll();

		::LeaveCriticalSection(&m_csWait);
	}

	virtual ~CPifSockSocket()
	{
		TRACE(_T("CPifSockSocket::~CPifSockSocket() destructor\n"));

		Exit();

		::EnterCriticalSection(&m_csWait);

		m_respMessageList.RemoveAll();
		m_requMessageList.RemoveAll();

		::DeleteCriticalSection(&m_csWait);
	}

	static void AddRespMessage(LPCTSTR psz)
	{
//		TRACE(_T("CPifSockSocket::AddRespMessage(LPCTSTR psz)\n"));

		if ( !m_bConnect )
			return;

		::EnterCriticalSection(&m_csWait);

		m_respMessageList.AddTail(psz);

		::LeaveCriticalSection(&m_csWait);
	}

	static void AddRequMessage(LPCTSTR psz)
	{
//		TRACE(_T("CPifSockSocket::AddRequMessage(LPCTSTR psz)\n"));

		if ( !m_bConnect )
			return;

		::EnterCriticalSection(&m_csWait);

		m_requMessageList.AddTail(psz);

		::LeaveCriticalSection(&m_csWait);
	}

protected:
	virtual UINT ThreadProc();
	virtual void DoAsyncSendBuff();

	BOOL PopRespMessageList(CString& str)
	{
//		TRACE(_T("CPifSockSocket::PopRespMessageList(CString& str)\n"));

		BOOL bRet = FALSE;

		::EnterCriticalSection(&m_csWait);

		if ( !m_respMessageList.IsEmpty() ) {
			str = m_respMessageList.GetHead();
			m_respMessageList.RemoveHead();
			bRet = TRUE;
		}
		else {
			str.Empty();
		}

		::LeaveCriticalSection(&m_csWait);

		return bRet;
	}

	BOOL PopRequMessageList(CString& str)
	{
//		TRACE(_T("CPifSockSocket::PopRequMessageList(CString& str)\n"));

		BOOL bRet = FALSE;

		::EnterCriticalSection(&m_csWait);

		if ( !m_requMessageList.IsEmpty() ) {
			str = m_requMessageList.GetHead();
			m_requMessageList.RemoveHead();
			bRet = TRUE;
		}
		else {
			str.Empty();
		}

		::LeaveCriticalSection(&m_csWait);

		return bRet;
	}

	int StringHexToint(LPCTSTR psz, size_t count);

	void AddRecvLogList(LPCTSTR pszRecvBuff, int nRead);
	void AddSendLogList();
	void AddSendLogList(LPCTSTR psz);
	void AddRecvLogList(LPCTSTR psz, LPCTSTR szDir =_T("Recv"));

// Overrides
public:
	//{{AFX_VIRTUAL(CPifSockSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPifSockSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

protected:
	static CRITICAL_SECTION m_csWait;
	static BOOL m_bConnect;
	BOOL m_bRequProcess;
	CString m_sendRetryBuffer;
	static CStringList m_respMessageList;
	static CStringList m_requMessageList;
	CLogFile m_pifSockLogFile;
};

/////////////////////////////////////////////////////////////////////////////
