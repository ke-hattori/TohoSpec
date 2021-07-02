// PifSockSocket.cpp : implementation file
//

#include "stdafx.h"
#include "NanoPifSock.h"
#include "LogFile.h"
#include "EventThread.h"
#include "PifSockSocket.h"

// Global variable
extern HANDLE g_hEventShutDown;

#define PIF_TIMER_INTERVAL				(5000)
#define SENDRETRYMAXTIMES				(3)
#define STRINGENQ						_T("[ENQ]")
#define STRINGCR						_T("[CR]")

//---------------------------------------------------------------------------
//
CRITICAL_SECTION CPifSockSocket::m_csWait;
BOOL CPifSockSocket::m_bConnect;
CStringList CPifSockSocket::m_respMessageList;
CStringList CPifSockSocket::m_requMessageList;

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPifSockSocket, CWinSockSocket)
	//{{AFX_MSG_MAP(CPifSockSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

//---------------------------------------------------------------------------
// ThreadProc
UINT CPifSockSocket::ThreadProc()
{
//	TRACE(_T("CPifSockSocket::ThreadProc()\n"));

	// 受信メッセージ解析のステータス
	enum { STATE_INIT, STATE_ENQ, STATE_MSGLEN, STATE_TERM };
	// 送信メッセージの非同期送信ステータス
	enum { STATE_NORMAL, STATE_ASYNCSENDRESP, STATE_ASYNCSENDREQU, STATE_ASYNCSENDREQURETRY };

	// 受信メッセージのレングス情報
	const int nEnqLen		  	= CPifMessage::ENQLEN;
	const int nMessageLenLen  	= CPifMessage::MESSAGELENLEN;
	const int nCommandLen 		= CPifMessage::COMMANDLEN;
	const int nCrLen			= CPifMessage::CRLEN;
	const int nAckCodeLen 		= CPifMessage::ACKLEN;
	const int nMessageMaxLen  	= CPifMessage::MESSAGEMAXLEN;
	const int nRecvMaxLen  		= 4096;
	const int nHeaderLen  		= nEnqLen + nMessageLenLen;
	const int nMessagePos 		= nHeaderLen;

	int nRead;
	int iState, iCntStateMsglen, iCntStateTerm, iAsyncSendState;
	int iMessageLen;
	int iRetry;
	TCHAR *psz, *pszEnq;
	TCHAR szBuff[256];
	DWORD dwStart, dwCurr, dwElapse;

	TCHAR szMessageLen[CPifMessage::MESSAGELENLEN + 1];
	TCHAR* const pszRecvBuff	 = (TCHAR*)VirtualAlloc(NULL, sizeof(TCHAR) * (nMessageMaxLen + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	TCHAR* const pszRecvedBuffer = (TCHAR*)VirtualAlloc(NULL, sizeof(TCHAR) * (nMessageMaxLen + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	TCHAR* const pszMessage 	 = (TCHAR*)VirtualAlloc(NULL, sizeof(TCHAR) * (nMessageMaxLen + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	pszRecvBuff[0]		= _TCHAR('\0');
	pszRecvedBuffer[0]	= _TCHAR('\0');
	pszMessage[0]		= _TCHAR('\0');

	m_bConnect = TRUE;

	iState = STATE_INIT;
	iAsyncSendState = STATE_NORMAL;
	psz = pszRecvedBuffer;

	while ( m_bRun )
	{
		// 受信処理
		nRead = Receive(pszRecvBuff, nRecvMaxLen);
		switch (nRead)
		{
		case 0:
			TRACE(_T("CPifSockSocket::ThreadProc() nRead = 0\n"));
			m_bRun = FALSE;
			m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
			break;

		case SOCKET_ERROR:
			if ( GetLastError() != WSAEWOULDBLOCK )
			{
				TRACE(_T("CPifSockSocket::ThreadProc() SOCKET_ERROR occurred : %d\n"), GetLastError());
				m_bRun = FALSE;
				m_parent->PostMessage(WM_SOCKET_CLOSE, (WPARAM)this);
			}
			break;

		default:
			pszRecvBuff[nRead] = _TCHAR('\0'); //terminate the string

			AddRecvLogList(pszRecvBuff, nRead);

			// 前回の受信データと今回分をデータ連結
			_tcscat(pszRecvedBuffer, pszRecvBuff);

			// 受信データの解析
			while ( *psz ) {
				switch ( iState ) {
				case  STATE_INIT:	// 初期化処理
					::ZeroMemory(szBuff, sizeof(szBuff));
					iCntStateMsglen = iCntStateTerm = iMessageLen = 0;
					iState = STATE_ENQ;
					break;
				case STATE_ENQ:		// ENQ文字を探す
					if ( CPifMessage::ENQ == *psz ) {
						pszEnq = psz;
						iState = STATE_MSGLEN;
					}
					psz++;
					break;
				case STATE_MSGLEN:	// メッセージ長の取得
					iCntStateMsglen++;
					if ( iCntStateMsglen < nMessageLenLen ) {
						;
					}
					else {
						_tcsncpy(szMessageLen, pszEnq + nEnqLen, nMessageLenLen);
						szMessageLen[nMessageLenLen] = _TCHAR('\0');
						iMessageLen = StringHexToint(szMessageLen, nMessageLenLen);
						iState = STATE_TERM;
					}
					psz++;
					break;
				case STATE_TERM:	// 終端文字を探す
					iCntStateTerm++;
					if ( CPifMessage::CR != *psz ) {
						if ( iCntStateTerm - nCrLen < iMessageLen ) {
							// 終端文字なし、メッセージ長に未到達（継続取得する）
							psz++;
							break;
						}
						else {
							// 終端文字なし、メッセージ長に到達
							TRACE(_T("CPifSockSocket::ThreadProc() Communication Error : CR not found\n"));
							_tcsncpy(pszMessage, pszEnq, nHeaderLen + iCntStateTerm);
							pszMessage[nHeaderLen + iCntStateTerm] = _TCHAR('\0');
							_tcsncpy(szBuff, &pszMessage[nMessagePos], nCommandLen);
							szBuff[nCommandLen] = _TCHAR('\0');
							CPifMessage902 pifMsg902;
							pifMsg902.SetItemRecvCommandId(szBuff);
							pifMsg902.Send();
						}
					}
					else {
						if ( iCntStateTerm - nCrLen < iMessageLen ) {
//							// 終端文字あり、メッセージ長とあわない
//							TRACE(_T("CPifSockSocket::ThreadProc() Communication Error : Message Data less than MessageLen\n"));
//							CPifMessage902 pifMsg902;
//							pifMsg902.SetItemRecvCommandId(_T("    "));
//							pifMsg902.Send();
						}
						else {
							// 終端文字あり、メッセージ長と一致（正常メッセージ受信）
							_tcsncpy(pszMessage, pszEnq, nHeaderLen + iCntStateTerm);
							pszMessage[nHeaderLen + iCntStateTerm] = _TCHAR('\0');
							if ( pszMessage[nMessagePos] == _TCHAR('P') ) { 				// Pxxx
								CEventThread::AddEvent(pszMessage, _tcslen(pszMessage));
							}
							else {															// Sxxx
								_stprintf(szBuff, _T("%c%s"), _TCHAR('S'), m_sendRetryBuffer.Mid(nMessagePos + 1, nCommandLen - 1));
								if ( _tcsncmp(&pszMessage[nMessagePos], _T("S104"), nCommandLen) == 0 )
									::SetEvent(g_hEventShutDown);
								if ( _tcsncmp(&pszMessage[nMessagePos], szBuff, nCommandLen) != 0 ) {
									_tcsncpy(szBuff, &pszMessage[nMessagePos], nCommandLen);
									szBuff[nCommandLen] = _TCHAR('\0');
									TRACE(_T("CPifSockSocket::ThreadProc() Recv Reply Messsage CommandId Error : %s\n"), szBuff);
								}
								else {
									if ( _tcsncmp(&pszMessage[nMessagePos + nCommandLen], _T("00"), nAckCodeLen) != 0 ) {
										_tcsncpy(szBuff, &pszMessage[nMessagePos], nCommandLen + nAckCodeLen); 	// Sxxx01 etc
										szBuff[nCommandLen + nAckCodeLen] = _TCHAR('\0');
										TRACE(_T("CPifSockSocket::ThreadProc() Recv Reply Messsage AckCode Error : %s\n"), szBuff);
									}
								}
								m_bRequProcess = FALSE;
							}
						}
					}
					_tcscpy(pszRecvedBuffer, &pszEnq[nMessagePos + iCntStateTerm]);
					psz = pszRecvedBuffer;
					iState = STATE_INIT;
					break;
				}
			}
			break;
		}


		switch ( iAsyncSendState ) {
		case STATE_NORMAL:
			// 送信処理（返信）
			if ( PopRespMessageList(m_sendBuffer) ) {
				m_nBytesSent = 0;
				m_nBytesBufferSize = m_sendBuffer.GetLength();
				DoAsyncSendBuff();
				if ( m_nBytesSent != m_nBytesBufferSize ) {
					iAsyncSendState = STATE_ASYNCSENDRESP;
					break;
				}
			}
			// 送信処理（リクエスト）
			if ( !m_bRequProcess ) {
				if ( PopRequMessageList(m_sendBuffer) ) {
					m_sendRetryBuffer = m_sendBuffer;
					m_nBytesSent = 0;
					m_nBytesBufferSize = m_sendBuffer.GetLength();
					DoAsyncSendBuff();
					if ( m_nBytesSent != m_nBytesBufferSize ) {
						iAsyncSendState = STATE_ASYNCSENDREQU;
						break;
					}
					m_bRequProcess = TRUE;
					dwStart = ::GetTickCount();
					dwElapse = 0L;
					iRetry = 0;
				}
			}
			else {
				dwCurr = ::GetTickCount();
				if ( dwCurr >= dwStart )
					dwElapse += (dwCurr - dwStart);
				else
					dwElapse += (dwCurr + (0xffffffff - dwStart));

				dwStart = dwCurr;

				if ( dwElapse < PIF_TIMER_INTERVAL ) {
					;
				}
				else {
					if ( iRetry < SENDRETRYMAXTIMES ) {
						// リトライ
						m_sendBuffer = m_sendRetryBuffer;
						m_nBytesSent = 0;
						m_nBytesBufferSize = m_sendBuffer.GetLength();
						DoAsyncSendBuff();
						if ( m_nBytesSent != m_nBytesBufferSize ) {
							iAsyncSendState = STATE_ASYNCSENDREQURETRY;
							break;
						}
						dwStart = ::GetTickCount();
						dwElapse = 0L;
						iRetry++;
					}
					else {
						// T3タイムアウト（リトライ回数オーバー）
						TRACE(_T("CPifSockSocket::ThreadProc() Communication Error : T3 Timeout\n"));
						CPifMessage903 pifMsg903;
						pifMsg903.SetItemSendCommandId(m_sendRetryBuffer.Mid(nMessagePos, nCommandLen));
						pifMsg903.Send();
						m_bRequProcess = FALSE;
					}
				}
			}
			break;
		case STATE_ASYNCSENDRESP:
			DoAsyncSendBuff();
			if ( m_nBytesSent != m_nBytesBufferSize )
				break;
			iAsyncSendState = STATE_NORMAL;
			break;
		case STATE_ASYNCSENDREQU:
			DoAsyncSendBuff();
			if ( m_nBytesSent != m_nBytesBufferSize )
				break;
			m_bRequProcess = TRUE;
			dwStart = ::GetTickCount();
			dwElapse = 0L;
			iRetry = 0;
			iAsyncSendState = STATE_NORMAL;
			break;
		case STATE_ASYNCSENDREQURETRY:
			DoAsyncSendBuff();
			if ( m_nBytesSent != m_nBytesBufferSize )
				break;
			dwStart = ::GetTickCount();
			dwElapse = 0L;
			iRetry++;
			iAsyncSendState = STATE_NORMAL;
			break;
		}

		::Sleep(10);
	}

	m_bConnect = FALSE;

	VirtualFree(pszRecvBuff, 0, MEM_DECOMMIT | MEM_RELEASE);
	VirtualFree(pszRecvedBuffer, 0, MEM_DECOMMIT | MEM_RELEASE);
	VirtualFree(pszMessage, 0, MEM_DECOMMIT | MEM_RELEASE);

	return 0L;
}

//---------------------------------------------------------------------------
// StringHexToint
//	 -1 : 変換不能（不正文字が指定された）
//	 xx : int型数値
int CPifSockSocket::StringHexToint(LPCTSTR psz, size_t count)
{
//	TRACE(_T("CPifSockSocket::StringHexToint(LPCTSTR psz, size_t count)\n"));

	ASSERT( 0 < count );
	ASSERT( count <= sizeof(int) * 8 / 4 );

	int iBuff;
	int iRet = 0;
	int i = 0;

	while ( (iBuff = psz[i]) != NULL )
	{
		if ( iBuff >= _TCHAR('A') && iBuff <= _TCHAR('F') ) {
			iBuff = iBuff - _TCHAR('A') + 10;
		}
		else if ( iBuff >= _TCHAR('0') && iBuff <= _TCHAR('9') ) {
			iBuff -= _TCHAR('0');
		}
		else {
			iRet = -1;
			break;
		}
		iBuff <<= 4 * (count - i - 1);
		iRet |= iBuff;
		i++;
	}
	return iRet;
}

//---------------------------------------------------------------------------
// AddRecvLogList
void CPifSockSocket::AddRecvLogList(LPCTSTR pszRecvBuff, int nRead)
{
//	TRACE(_T("CPifSockSocket::AddRecvLogList(LPCTSTR pszRecvBuff, int nRead)\n"));

	TCHAR* pszLog = new TCHAR[nRead + 256];
	for ( int i = 0, j = 0; pszRecvBuff[i] != _TCHAR('\0') && j < nRead + 256; i++ ) {
		if ( pszRecvBuff[i] == CPifMessage::ENQ ) {
			::CopyMemory(&pszLog[j], STRINGENQ, _tcslen(STRINGENQ));
			j += _tcslen(STRINGENQ);
		}
		else if ( pszRecvBuff[i] == CPifMessage::CR ) {
			::CopyMemory(&pszLog[j], STRINGCR, _tcslen(STRINGCR));
			j += _tcslen(STRINGCR);
		}
		else {
			pszLog[j++] = pszRecvBuff[i];
		}
	}
	pszLog[j] = _TCHAR('\0');
	AddRecvLogList(pszLog);
	delete [] pszLog;
	pszLog = NULL;
}

//---------------------------------------------------------------------------
// AddSendLogList
void CPifSockSocket::AddSendLogList()
{
//	TRACE(_T("CPifSockSocket::AddSendLogList()\n"));

	TCHAR* pszLog = new TCHAR[m_nBytesBufferSize + 256];
	for ( int i = 0, j = 0; i < m_nBytesBufferSize && j < m_nBytesBufferSize + 256; i++ ) {
		if ( m_sendBuffer[i] == CPifMessage::ENQ ) {
			::CopyMemory(&pszLog[j], STRINGENQ, _tcslen(STRINGENQ));
			j += _tcslen(STRINGENQ);
		}
		else if ( m_sendBuffer[i] == CPifMessage::CR ) {
			::CopyMemory(&pszLog[j], STRINGCR, _tcslen(STRINGCR));
			j += _tcslen(STRINGCR);
		}
		else {
			pszLog[j++] = m_sendBuffer[i];
		}
	}
	pszLog[j] = _TCHAR('\0');
	AddSendLogList(pszLog);
	delete [] pszLog;
	pszLog = NULL;
}

//---------------------------------------------------------------------------
// AddSendLogList
void CPifSockSocket::AddSendLogList(LPCTSTR psz)
{
	AddRecvLogList(psz, _T("Send"));
}

//---------------------------------------------------------------------------
// AddRecvLogList
void CPifSockSocket::AddRecvLogList(LPCTSTR psz, LPCTSTR szDir/*=_T("Recv")*/)
{
	CString strBuffer;
	TCHAR szDate[256];
	TCHAR szTime[256];

	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);
	_stprintf(szDate, _T("%04d/%02d/%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	_stprintf(szTime, _T("%02d:%02d:%02d.%03d"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	strBuffer  = szDate;
	strBuffer += szTime;
	strBuffer += szDir;

	strBuffer  = szDate;
	strBuffer += _T(" ");
	strBuffer += szTime;
	strBuffer += _T(" ");
	strBuffer += szDir;
	strBuffer += _T(" ");

	m_pifSockLogFile.Logging(strBuffer + psz);
}

//---------------------------------------------------------------------------
// DoAsyncSendBuff
//	 非同期送信
void CPifSockSocket::DoAsyncSendBuff()
{
	TRACE("CPifSockSocket::DoAsyncSendBuff()\n");

	CWinSockSocket::DoAsyncSendBuff();

	if ( m_nBytesSent == m_nBytesBufferSize )
		AddSendLogList();
}
