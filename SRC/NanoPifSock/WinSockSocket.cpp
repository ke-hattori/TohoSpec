// WinSockSocket.cpp : implementation file
//

#include "stdafx.h"
#include "WinSockSocket.h"

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CWinSockSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CWinSockSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

//---------------------------------------------------------------------------
// Execute
BOOL CWinSockSocket::Execute()
{
	BOOL bRet = FALSE;

	if ( m_hThread ==  NULL )
	{
		m_hThread = (HANDLE) _beginthreadex(NULL, 0, ThreadProc, LPVOID(this), 0, &m_nThreadId);
		if ( m_hThread == NULL )
		{
			return FALSE;
		}
		bRet = TRUE;
	}
	return bRet;
}

//---------------------------------------------------------------------------
// ThreadProc
UINT WINAPI CWinSockSocket::ThreadProc(LPVOID lpData)
{
	CWinSockSocket* pThis = reinterpret_cast<CWinSockSocket*>(lpData);

	return pThis->ThreadProc();
}

//---------------------------------------------------------------------------
// ThreadProc
UINT CWinSockSocket::ThreadProc()
{
	TCHAR buff[4096 + 1];
	int nRead;

	while ( m_bRun )
	{
		// éÛêMèàóù
		nRead = Receive(buff, 4096);
		switch (nRead)
		{
		case 0:
			m_bRun = FALSE;
			m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
			break;

		case SOCKET_ERROR:
			if ( GetLastError() != WSAEWOULDBLOCK )
			{
				TRACE(_T("SOCKET_ERROR occurred %d\n"), GetLastError());
				m_bRun = FALSE;
				m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
			}
			break;

		default:
			buff[nRead] = 0; //terminate the string
			break;
		}

		// ëóêMèàóù
//		if ( m_nBytesSent == m_nBytesBufferSize ) {
//			m_sendBuffer = _T("Hoge");
//			m_nBytesSent = 0;
//			m_nBytesBufferSize = m_sendBuffer.GetLength() + 1;
//			DoAsyncSendBuff();
//		}
//		else {
//			DoAsyncSendBuff();
//		}

		::Sleep(10);
	}

	return 0L;
}

//---------------------------------------------------------------------------
// DoAsyncSendBuff
//	 îÒìØä˙ëóêM
void CWinSockSocket::DoAsyncSendBuff()
{
	TRACE("CWinSockSocket::DoAsyncSendBuff()\n");

	while (m_nBytesSent < m_nBytesBufferSize)
	{
		int dwBytes;

		if ((dwBytes = Send((LPCTSTR)m_sendBuffer + m_nBytesSent, m_nBytesBufferSize - m_nBytesSent))
			== SOCKET_ERROR)
		{
			if (GetLastError() != WSAEWOULDBLOCK) {
				TCHAR szError[256];
				_stprintf(szError, _T("Server Socket failed to send: %d\n"), GetLastError());
				//AfxMessageBox (szError);
				TRACE(_T("%s\n"), szError);
				m_bRun = FALSE;
				m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
			}
			break;
		}
		else
		{
			m_nBytesSent += dwBytes;
		}
	}

//	m_nBytesSent = m_nBytesBufferSize = 0;
//	m_sendBuffer.Empty();
}
