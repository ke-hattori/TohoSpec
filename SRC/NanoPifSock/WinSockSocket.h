#pragma once

#include "stdafx.h"
#include <process.h>	/* _beginthread, _endthread */

#define WM_SERVER_ACCEPT		(WM_USER + 0x0100)
#define WM_SOCKET_CLOSE 		(WM_USER + 0x0101)


// データ送受信用ソケット
class CWinSockSocket : public CAsyncSocket
{
public:
	CWinSockSocket(CWnd* parent)
		: m_parent(parent),
		  m_hThread(NULL),
		  m_nThreadId(0L),
		  m_bRun(TRUE),
		  m_nBytesSent(0),
		  m_nBytesBufferSize(0)
	{
		TRACE(_T("CWinSockSocket::CWinSockSocket(CWnd* parent) constructor\n"));
	}

	virtual ~CWinSockSocket()
	{
		TRACE(_T("CWinSockSocket::~CWinSockSocket() destructor\n"));

// 2009.06.16 K.Matsuo Bug Fix -->
//		Exit();
// 2009.06.16 K.Matsuo Bug Fix <--
	}

	BOOL Execute();

protected:
	static UINT WINAPI ThreadProc(LPVOID lpData);

	virtual UINT ThreadProc();

	void Exit()
	{
//		  TRACE(_T("CWinSockSocket::Exit()\n"));

		if ( m_hThread )
		{
// 2009.06.16 K.Matsuo Bug Fix -->
//			m_bRun = FALSE;
// 2009.06.16 K.Matsuo Bug Fix <--
			::WaitForSingleObject(m_hThread, INFINITE);
			::CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	virtual void DoAsyncSendBuff();

	virtual void OnConnect(int nErrorCode)
	{
		TRACE(_T("CWinSockSocket::OnConnect(int nErrorCode)\n"));

		if ( nErrorCode == 0 ) {
			m_parent->PostMessage(WM_SERVER_ACCEPT, (WPARAM)this);		   // Connected Socket
		}
		else {
			TRACE(_T("CWinSockSocket::OnConnect Error occured : %d\n"), nErrorCode);
			m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
		}
		CAsyncSocket::OnConnect(nErrorCode);
	}

	virtual void OnClose(int nErrorCode)
	{
		TRACE(_T("CWinSockSocket::OnClose(int nErrorCode)\n"));

		if ( nErrorCode != 0 ) {
			TRACE(_T("CWinSockSocket::OnConnect Error occured : %d\n"), nErrorCode);
		}
// 2009.06.16 K.Matsuo Bug Fix -->
//		Exit();
//		m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
// 2009.06.16 K.Matsuo Bug Fix <--

		CAsyncSocket::OnClose(nErrorCode);
	}

// Overrides
public:
	//{{AFX_VIRTUAL(CWinSockSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CWinSockSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

protected:
	CWnd* m_parent;
	HANDLE m_hThread;
	UINT m_nThreadId;
	BOOL m_bRun;

	CString m_sendBuffer;	//for async send
	int m_nBytesSent;
	int m_nBytesBufferSize;

};

/////////////////////////////////////////////////////////////////////////////


// リスナーソケット（サーバーソケット）
template<class T>
class CWinSockLstn : public CAsyncSocket
{
public:
	CWinSockLstn(CWnd* parent, LPCTSTR pszExeName) : m_parent(parent), m_strExeName(pszExeName), m_sock(NULL)
	{
		TRACE(_T("CWinSockLstn::CWinSockLstn(CWnd* parent) constructor\n"));
	}

	virtual ~CWinSockLstn()
	{
		TRACE(_T("CWinSockLstn::~CWinSockLstn() destructor\n"));
	}

	BOOL Open(UINT Port, LPCTSTR pszSocketAddress)
	{
		if ( !Create(Port, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, pszSocketAddress) )
		{
			TRACE(_T("Create error %d\n"), GetLastError());
			return FALSE;
		}
		if ( !Listen() )
		{
			TRACE(_T("Listen error %d\n"), GetLastError());
			return FALSE;
		}
		return TRUE;
	}

	virtual void OnAccept(int nErrorCode)
	{
		if ( !nErrorCode ) {
//			if ( m_sock != NULL ) {
//				CWinSockSocket tempSvSk(NULL);
//				Accept(tempSvSk);
//				tempSvSk.Close();
//			}
//			else {
				T* pSock = new T(m_parent, m_strExeName);
				if ( !pSock ) {
					TRACE(_T("CWinSockSocket error\n"));
				}
				else {
					if ( !Accept(*pSock) ) {
						TRACE(_T("Accept error %d\n"), GetLastError());
						delete pSock;
						pSock = NULL;
					}
					else {
						pSock->Execute();
						m_parent->PostMessage(WM_SERVER_ACCEPT, (WPARAM)pSock); 		// Connected Socket
						m_sock = pSock;
					}
				}
//			}
		}
		CAsyncSocket::OnAccept(nErrorCode);
	}

protected:
	CWnd* m_parent;
	CString m_strExeName;
	CWinSockSocket* m_sock;
};

/////////////////////////////////////////////////////////////////////////////


