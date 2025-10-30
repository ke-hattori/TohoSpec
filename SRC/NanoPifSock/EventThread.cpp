// EventThread.cpp : implementation file
//

#include "stdafx.h"
#include "NanoPifSock.h"
#include "WinSockSocket.h"
#include "Pifmessage.h"
#include "EventThread.h"
#include "Mailslot.h"
#include "LogFile.h"


CRITICAL_SECTION CEventThread::m_csWait;
std::list<CPifMessage*> CEventThread::m_eventList;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventThread

IMPLEMENT_DYNCREATE(CEventThread, CWinThread)

CEventThread::CEventThread()
{
}

CEventThread::~CEventThread()
{
}

BOOL CEventThread::InitInstance()
{
	TRACE(_T("CEventThread::InitInstance()\n"));

	m_bProcess = FALSE;
	m_bShutDown = FALSE;
	m_bAutoDelete = FALSE;

	::InitializeCriticalSection(&m_csWait);

	::EnterCriticalSection(&m_csWait);

	m_eventList.clear();

	::LeaveCriticalSection(&m_csWait);

	m_strMailslotServer.LoadString(IDS_MAILSLOT_SERVER);
	m_strMailslotClient.LoadString(IDS_MAILSLOT_CLIENT);

	m_mailslot.Create(m_strMailslotServer);

	return TRUE;
}

int CEventThread::ExitInstance()
{
	TRACE(_T("CEventThread::ExitInstance()\n"));

	::EnterCriticalSection(&m_csWait);

	if ( m_bProcess )
		DeleteEvent();

	while ( GetEvent() ) {
		DeleteEvent();
	}

	::DeleteCriticalSection(&m_csWait);

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CEventThread, CWinThread)
	//{{AFX_MSG_MAP(CEventThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventThread message handlers
int CEventThread::Run()
{
	TRACE(_T("CEventThread::Run()\n"));

	DWORD nNumberOfBytesRead;
	int iState;

	// NanoSpec/AFTへの送信ステータス
	enum { STATE_SENDMAIL, STATE_WAITRECVMAIL };

	// 受信メッセージのレングス情報
	const int nEnqLen		  	= CPifMessage::ENQLEN;
	const int nMessageLenLen  	= CPifMessage::MESSAGELENLEN;
	const int nCommandLen 		= CPifMessage::COMMANDLEN;
	const int nCrLen			= CPifMessage::CRLEN;
	const int nAckCodeLen 		= CPifMessage::ACKLEN;
	const int nMessageMaxLen  	= CPifMessage::MESSAGEMAXLEN;
	const int nHeaderLen  		= nEnqLen + nMessageLenLen;
	const int nMessagePos 		= nHeaderLen;

	TCHAR* pszRecvBuff = (TCHAR*)VirtualAlloc(NULL, sizeof(TCHAR) * (nMessageMaxLen + 1), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	TCHAR szCommandId[nCommandLen + 1];

//	CLogFile pifMailslotLogFile(_T("NanoPifMailslot"));

	while ( !m_bShutDown )
	{
		// CIM/PCからの受信コマンドをNanoSpec/AFTへ転送処理する（P9xxは除く）
		if ( !m_bProcess ) {
			GetEvent();
			iState = STATE_SENDMAIL;
		}

		if ( m_bProcess ) {
			_tcscpy(pszRecvBuff, m_pEventCurr->GetItemRecvCommandId());
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
			if ( _tcscmp(pszRecvBuff, _T("P105")) == 0 ||
				 _tcscmp(pszRecvBuff, _T("P106")) == 0 ||
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
				 _tcscmp(pszRecvBuff, _T("P201")) == 0 ||
			 	 _tcscmp(pszRecvBuff, _T("P302")) == 0 ||
			 	 _tcscmp(pszRecvBuff, _T("P303")) == 0 ||
			 	 _tcscmp(pszRecvBuff, _T("P306")) == 0 ||
			 	 _tcscmp(pszRecvBuff, _T("P308")) == 0 ||
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
			 	 _tcscmp(pszRecvBuff, _T("P310")) == 0 ||
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--
			 	 _tcscmp(pszRecvBuff, _T("P401")) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 -->
			 	 _tcscmp(pszRecvBuff, _T("P411")) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 <--
  				 _tcscmp(pszRecvBuff, _T("P501")) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 -->
  				 _tcscmp(pszRecvBuff, _T("P511")) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 <--
// 2014.01.07 bagus Add(Stage None対応) -->
  				 _tcscmp(pszRecvBuff, _T("P514")) == 0 ||
  				 _tcscmp(pszRecvBuff, _T("P516")) == 0 ||
// 2014.01.07 bagus Add(Stage None対応) <--
		  		 _tcscmp(pszRecvBuff, _T("P601")) == 0 ||
			  	 _tcscmp(pszRecvBuff, _T("P602")) == 0 ||
  				 _tcscmp(pszRecvBuff, _T("P603")) == 0 ||
	  			 _tcscmp(pszRecvBuff, _T("P604")) == 0 ||
		  		 _tcscmp(pszRecvBuff, _T("P605")) == 0 ||
  				 _tcscmp(pszRecvBuff, _T("P606")) == 0 ||
	  			 _tcscmp(pszRecvBuff, _T("P607")) == 0 ||
			  	 _tcscmp(pszRecvBuff, _T("P608")) == 0 ||
		  		 _tcscmp(pszRecvBuff, _T("P609")) == 0 ||
		  		 _tcscmp(pszRecvBuff, _T("P610")) == 0 ||
		  		 _tcscmp(pszRecvBuff, _T("P611")) == 0 )
			{
				switch ( iState ) {
				case STATE_SENDMAIL:
					if ( CMailslot::Send(m_strMailslotClient, m_pEventCurr->GetRecvMessagePtr(), _tcslen(m_pEventCurr->GetRecvMessagePtr())) ) {
//						pifMailslotLogFile.Logging(MakeLogText(_T("Send"), pszRecvBuff));
						iState = STATE_WAITRECVMAIL;
					}
					break;
				case STATE_WAITRECVMAIL:
					break;
				}
			}
			else if ( _tcscmp(pszRecvBuff, _T("P901")) == 0 )
				EventProcessP901();
			else if ( _tcscmp(pszRecvBuff, _T("P902")) == 0 )
				EventProcessP902();
			else if ( _tcscmp(pszRecvBuff, _T("P903")) == 0 )
				EventProcessP903();
			else if ( _tcscmp(pszRecvBuff, _T("P904")) == 0 )
				EventProcessP904();
			else
			{
				UndefinedEventProcess();
			}
		}

		// NanoSpec/AFTからの返信コマンド or イベント報告をCIM/PCへ転送処理する
		if ( m_mailslot.Recieve(pszRecvBuff, &nNumberOfBytesRead) ) {
			_tcsncpy(szCommandId, &pszRecvBuff[nMessagePos], nCommandLen);
			szCommandId[nCommandLen] = _TCHAR('\0');
//			pifMailslotLogFile.Logging(MakeLogText(_T("Recv"), szCommandId));

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
			if ( _tcsncmp(&pszRecvBuff[nMessagePos], _T("S105"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S106"), nCommandLen) == 0 ||
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S201"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S302"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S303"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S306"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S308"), nCommandLen) == 0 ||
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S310"), nCommandLen) == 0 ||
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S401"), nCommandLen) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 -->
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S411"), nCommandLen) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 <--
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S501"), nCommandLen) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 -->
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S511"), nCommandLen) == 0 ||
// 2009.09.10 K.Matsuo 透過率リファレンス（ガントリー）取得機能追加 <--
// 2014.01.07 bagus Add(Stage None対応) -->
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S514"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S516"), nCommandLen) == 0 ||
// 2014.01.07 bagus Add(Stage None対応) <--
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S601"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S602"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S603"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S604"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S605"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S606"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S607"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S608"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S609"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S610"), nCommandLen) == 0 ||
				 _tcsncmp(&pszRecvBuff[nMessagePos], _T("S611"), nCommandLen) == 0 )
			{
				EventProcessSxxxDefault(pszRecvBuff);
			}
			else if ( _tcsncmp(&pszRecvBuff[nMessagePos], _T("P"), 1) == 0 )
			{
				CPifSockSocket::AddRequMessage(pszRecvBuff);
			}
			else
			{
				ASSERT( FALSE );
			}
		}

		::Sleep(10);
	}

	VirtualFree(pszRecvBuff, 0, MEM_DECOMMIT | MEM_RELEASE);

	::PostQuitMessage(0);

	return CWinThread::Run();
}

//---------------------------------------------------------------------------
// ShutDown
void CEventThread::ShutDown()
{
	m_bShutDown = TRUE;
}

//---------------------------------------------------------------------------
// EventProcessSxxxDefault
void CEventThread::EventProcessSxxxDefault(LPCTSTR psz)
{
	TRACE(_T("CEventThread::EventProcessSxxxDefault(LPCTSTR psz)\n"));

	const int nEnqLen		  	= CPifMessage::ENQLEN;
	const int nMessageLenLen  	= CPifMessage::MESSAGELENLEN;
	const int nCommandLen 		= CPifMessage::COMMANDLEN;
	const int nAckCodeLen 		= CPifMessage::ACKLEN;
	const int nHeaderLen  		= nEnqLen + nMessageLenLen;
	const int nMessagePos 		= nHeaderLen;

	TCHAR szBuff[256];
	_tcsncpy(szBuff, &psz[nMessagePos], nCommandLen);
	szBuff[0] = _TCHAR('P');
	szBuff[nCommandLen] = _TCHAR('\0');
	ASSERT( m_bProcess == TRUE );
	ASSERT( _tcscmp(m_pEventCurr->GetItemRecvCommandId(), szBuff) == 0 );

	_tcsncpy(szBuff, &psz[nMessagePos+ nCommandLen], nAckCodeLen);
	szBuff[nAckCodeLen] = _TCHAR('\0');
	m_pEventCurr->SetItemAckCode(szBuff);
	m_pEventCurr->SendResp();
	DeleteEvent();
}

//---------------------------------------------------------------------------
// EventProcessP901
void CEventThread::EventProcessP901()
{
	TRACE(_T("CEventThread::EventProcessP901()\n"));

	DeleteEvent();
}

//---------------------------------------------------------------------------
// EventProcessP902
void CEventThread::EventProcessP902()
{
	TRACE(_T("CEventThread::EventProcessP902()\n"));

	DeleteEvent();
}

//---------------------------------------------------------------------------
// EventProcessP903
void CEventThread::EventProcessP903()
{
	TRACE(_T("CEventThread::EventProcessP903()\n"));

	DeleteEvent();
}

//---------------------------------------------------------------------------
// EventProcessP904
void CEventThread::EventProcessP904()
{
	TRACE(_T("CEventThread::EventProcessP904()\n"));

	DeleteEvent();
}

//---------------------------------------------------------------------------
// UndefinedEventProcess
void CEventThread::UndefinedEventProcess()
{
	TRACE(_T("CEventThread::UndefinedEventProcess() %s\n"), m_pEventCurr->GetItemRecvCommandId());

	CPifMessage901 pifMsg901;
	pifMsg901.SetItemIllegalCommandId(m_pEventCurr->GetItemRecvCommandId());
	pifMsg901.Send();

	DeleteEvent();
}

//---------------------------------------------------------------------------
// MakeLogText
CString CEventThread::MakeLogText(LPCTSTR pszDir, LPCTSTR pszMsg)
{
	CString strBuffer;
	TCHAR szDate[256];
	TCHAR szTime[256];
	SYSTEMTIME systemTime;

	::GetLocalTime(&systemTime);
	_stprintf(szDate, _T("%04d/%02d/%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	_stprintf(szTime, _T("%02d:%02d:%02d.%03d"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	strBuffer  = szDate;
	strBuffer += _T(" ");
	strBuffer += szTime;
	strBuffer += _T(" ");
	strBuffer += pszDir;
	strBuffer += _T(" ");
	strBuffer += pszMsg;

	return strBuffer;
}
