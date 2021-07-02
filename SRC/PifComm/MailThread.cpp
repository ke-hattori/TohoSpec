// MailThread.cpp : implementation file
//

#include "stdafx.h"
#include "..\\..\\INC\\PifComm.hxx"
#include "Pifmessage.h"
#include "MailThread.h"
#include "Mailslot.h"

extern HWND g_hNotifyWnd;
extern SYSTEMTIME g_setTime;
extern int g_iKindOfRecipe;
extern TCHAR g_szSampleSizeRecipeName[RECIPE_NAME_LEN + 1];
extern TCHAR g_szReferenceDataCheckRecipeName[RECIPE_NAME_LEN + 1];
extern TCHAR g_szRecipeName[RECIPE_NAME_LEN + 1];
extern TCHAR g_szSampleId[SAMPLE_ID_LEN + 1];
extern TCHAR g_szLotId[LOT_ID_LEN + 1];
extern double g_dSampleSizeX;
extern double g_dSampleSizeY;
extern int g_iMovePosId;

#define MAILSLOT_SERVER 		_T("\\\\.\\mailslot\\NanoSpec")
#define MAILSLOT_CLIENT 		_T("\\\\.\\mailslot\\NanoPifSock")

CRITICAL_SECTION CMailThread::m_csWait;
CStringList CMailThread::m_sendMessageList;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailThread

IMPLEMENT_DYNCREATE(CMailThread, CWinThread)

CMailThread::CMailThread()
{
}

CMailThread::~CMailThread()
{
}

BOOL CMailThread::InitInstance()
{
	TRACE(_T("CMailThread::InitInstance()\n"));

	m_pEventCurr = NULL;
	m_iState = STATE_RECV;
	m_iAckCode = 0;
	m_bShutDown = FALSE;
	m_strMailslotSendName.Empty();
	m_strSendCommandId.Empty();
	m_bAutoDelete = FALSE;
	m_mailslot.Create(MAILSLOT_SERVER);

	::InitializeCriticalSection(&m_csWait);

	return TRUE;
}

int CMailThread::ExitInstance()
{
	TRACE(_T("CMailThread::ExitInstance()\n"));

	if ( m_pEventCurr )	{
		delete m_pEventCurr;
		m_pEventCurr = NULL;
	}

	::EnterCriticalSection(&m_csWait);

	m_sendMessageList.RemoveAll();

	::DeleteCriticalSection(&m_csWait);

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMailThread, CWinThread)
	//{{AFX_MSG_MAP(CMailThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailThread message handlers
int CMailThread::Run()
{
	TRACE(_T("CMailThread::Run()\n"));

	DWORD nNumberOfBytesRead;
	CString strBuffer;

	const int nMessageMaxLen = 256;
	TCHAR szRecvBuff[nMessageMaxLen];

	while ( !m_bShutDown )
	{
		// CIM/PCからの受信コマンド
		switch ( m_iState ) {
		case STATE_RECV:
			if ( m_mailslot.Recieve(szRecvBuff, &nNumberOfBytesRead) ) {

				ASSERT( nNumberOfBytesRead <= nMessageMaxLen );

				m_pEventCurr = new CPifMessage();
				m_pEventCurr->Recv(szRecvBuff, nNumberOfBytesRead);
				_tcscpy(szRecvBuff, m_pEventCurr->GetItemRecvCommandId());
				EventProcess(szRecvBuff);
				m_iState = STATE_PROC;
			}
			break;
		case STATE_PROC:
			m_iAckCode = 0;
			m_iState = STATE_WAIT;		// バグ修正：CMailslot::Send()より前にしないと、稀にDoStateDone()が先に呼出されて、ASSERT発生することがあった
			CMailslot::Send(m_strMailslotSendName, m_strSendCommandId, m_strSendCommandId.GetLength());
//			m_iState = STATE_WAIT;
			break;
		case STATE_WAIT:
			break;
//		case STATE_DONE:
//			m_pEventCurr->SetItemAckCode(m_iAckCode);
//			m_pEventCurr->SendResp();
//
//			delete m_pEventCurr;
//			m_pEventCurr = NULL;
//			m_iState = STATE_RECV;
//			break;
		}

		// NanoSpec/AFTからの返信・送信コマンド
		if ( PopSendMessageList(strBuffer) ) {
			if ( CMailslot::Send(MAILSLOT_CLIENT, strBuffer, strBuffer.GetLength()) ) {
				if ( g_hNotifyWnd )
					::PostMessage(g_hNotifyWnd, WM_PIFCOMM_CONNECT, TRUE, 0);
			}
			else {
				if ( g_hNotifyWnd )
					::PostMessage(g_hNotifyWnd, WM_PIFCOMM_CONNECT, FALSE, 0);
			}
		}

		::Sleep(10);
	}

	::PostQuitMessage(0);

	return CWinThread::Run();
}

//---------------------------------------------------------------------------
// ShutDown
void CMailThread::ShutDown()
{
	m_bShutDown = TRUE;
}

//---------------------------------------------------------------------------
// DoStateDone
void CMailThread::DoStateDone(int iAckCode)
{
	TRACE(_T("CMailThread::DoStateDone(int iAckCode)\n"));

	ASSERT( m_iState == STATE_WAIT );

	m_pEventCurr->SetItemAckCode(iAckCode);
	m_pEventCurr->SendResp();

	delete m_pEventCurr;
	m_pEventCurr = NULL;
	m_iState = STATE_RECV;
}

//---------------------------------------------------------------------------
// EventProcess
void CMailThread::EventProcess(LPCTSTR pszRecvCommandId)
{
	TRACE(_T("CMailThread::EventProcess()\n"));

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
	if ( _tcscmp(pszRecvCommandId, _T("P105")) == 0 )
		EventProcessP105();
	else if ( _tcscmp(pszRecvCommandId, _T("P106")) == 0 )
		EventProcessP106();
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
	else if ( _tcscmp(pszRecvCommandId, _T("P201")) == 0 )
		EventProcessP201();
	else if ( _tcscmp(pszRecvCommandId, _T("P302")) == 0 )
		EventProcessP302();
	else if ( _tcscmp(pszRecvCommandId, _T("P303")) == 0 )
		EventProcessP303();
	else if ( _tcscmp(pszRecvCommandId, _T("P306")) == 0 )
		EventProcessP306();
	else if ( _tcscmp(pszRecvCommandId, _T("P308")) == 0 )
		EventProcessP308();
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
	else if ( _tcscmp(pszRecvCommandId, _T("P310")) == 0 )
		EventProcessP310();
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--
	else if ( _tcscmp(pszRecvCommandId, _T("P401")) == 0 )
		EventProcessP401();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
	else if ( _tcscmp(pszRecvCommandId, _T("P411")) == 0 )
		EventProcessP411();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--
	else if ( _tcscmp(pszRecvCommandId, _T("P501")) == 0 )
		EventProcessP501();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
	else if ( _tcscmp(pszRecvCommandId, _T("P511")) == 0 )
		EventProcessP511();
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--
// 2014.01.07 bagus Add(Stage None対応) -->
	else if ( _tcscmp(pszRecvCommandId, _T("P514")) == 0 )
		EventProcessP514();
	else if ( _tcscmp(pszRecvCommandId, _T("P516")) == 0 )
		EventProcessP516();
// 2014.01.07 bagus Add(Stage None対応) <--
	else if ( _tcscmp(pszRecvCommandId, _T("P601")) == 0 )
		EventProcessP601();
	else if ( _tcscmp(pszRecvCommandId, _T("P602")) == 0 )
		EventProcessP602();
	else if ( _tcscmp(pszRecvCommandId, _T("P603")) == 0 )
		EventProcessP603();
	else if ( _tcscmp(pszRecvCommandId, _T("P604")) == 0 )
		EventProcessP604();
	else if ( _tcscmp(pszRecvCommandId, _T("P605")) == 0 )
		EventProcessP605();
	else if ( _tcscmp(pszRecvCommandId, _T("P606")) == 0 )
		EventProcessP606();
	else if ( _tcscmp(pszRecvCommandId, _T("P607")) == 0 )
		EventProcessP607();
	else if ( _tcscmp(pszRecvCommandId, _T("P608")) == 0 )
		EventProcessP608();
	else if ( _tcscmp(pszRecvCommandId, _T("P609")) == 0 )
		EventProcessP609();
	else if ( _tcscmp(pszRecvCommandId, _T("P610")) == 0 )
		EventProcessP610();
	else if ( _tcscmp(pszRecvCommandId, _T("P611")) == 0 )
		EventProcessP611();
	else
		ASSERT( FALSE );
}

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
//---------------------------------------------------------------------------
// EventProcessP105
void CMailThread::EventProcessP105()
{
	TRACE(_T("CMailThread::EventProcessP105()\n"));

	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P105;
}

//---------------------------------------------------------------------------
// EventProcessP106
void CMailThread::EventProcessP106()
{
	TRACE(_T("CMailThread::EventProcessP106()\n"));

	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P106;
}
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--

//---------------------------------------------------------------------------
// EventProcessP201
void CMailThread::EventProcessP201()
{
	TRACE(_T("CMailThread::EventProcessP201()\n"));

	((CPifMessage201*)m_pEventCurr)->GetItemDateTime(&g_setTime);
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P201;
}

//---------------------------------------------------------------------------
// EventProcessP302
void CMailThread::EventProcessP302()
{
	TRACE(_T("CMailThread::EventProcessP302()\n"));

	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P302;
}

//---------------------------------------------------------------------------
// EventProcessP303
void CMailThread::EventProcessP303()
{
	TRACE(_T("CMailThread::EventProcessP303()\n"));

	g_iKindOfRecipe = ((CPifMessage303*)m_pEventCurr)->GetItemKindOfRecipe();
	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P303;
}

//---------------------------------------------------------------------------
// EventProcessP306
void CMailThread::EventProcessP306()
{
	TRACE(_T("CMailThread::EventProcessP306()\n"));

	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P306;
}

//---------------------------------------------------------------------------
// EventProcessP308
void CMailThread::EventProcessP308()
{
	TRACE(_T("CMailThread::EventProcessP308()\n"));

	((CPifMessage308*)m_pEventCurr)->GetItemRecipeName(g_szSampleSizeRecipeName);
	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P308;
}

// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
//---------------------------------------------------------------------------
// EventProcessP310
void CMailThread::EventProcessP310()
{
	TRACE(_T("CMailThread::EventProcessP310()\n"));

	((CPifMessage310*)m_pEventCurr)->GetItemRecipeName(g_szReferenceDataCheckRecipeName);
	m_strMailslotSendName = MAILSLOT_MAINFRM;
	m_strSendCommandId = PIFCOMM_P310;
}
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--

//---------------------------------------------------------------------------
// EventProcessP401
void CMailThread::EventProcessP401()
{
	TRACE(_T("CMailThread::EventProcessP401()\n"));

	((CPifMessage401*)m_pEventCurr)->GetItemRecipeName(g_szRecipeName);
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P401;
}

// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
//---------------------------------------------------------------------------
// EventProcessP411
void CMailThread::EventProcessP411()
{
	TRACE(_T("CMailThread::EventProcessP411()\n"));

	((CPifMessage411*)m_pEventCurr)->GetItemRecipeName(g_szRecipeName);
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P411;
}
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--

//---------------------------------------------------------------------------
// EventProcessP501
void CMailThread::EventProcessP501()
{
	TRACE(_T("CMailThread::EventProcessP501()\n"));

	((CPifMessage501*)m_pEventCurr)->GetItemSampleId(g_szSampleId);
	((CPifMessage501*)m_pEventCurr)->GetItemLotId(g_szLotId);
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P501;
}

// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
//---------------------------------------------------------------------------
// EventProcessP511
void CMailThread::EventProcessP511()
{
	TRACE(_T("CMailThread::EventProcessP511()\n"));

	((CPifMessage511*)m_pEventCurr)->GetItemSampleId(g_szSampleId);
	((CPifMessage511*)m_pEventCurr)->GetItemLotId(g_szLotId);
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P511;
}
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--

// 2014.01.07 bagus Add(Stage None対応) -->
//---------------------------------------------------------------------------
// EventProcessP514
void CMailThread::EventProcessP514()
{
	TRACE(_T("CMailThread::EventProcessP514()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P514;
}

//---------------------------------------------------------------------------
// EventProcessP516
void CMailThread::EventProcessP516()
{
	TRACE(_T("CMailThread::EventProcessP516()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P516;
}
// 2014.01.07 bagus Add(Stage None対応) <--

//---------------------------------------------------------------------------
// EventProcessP601
void CMailThread::EventProcessP601()
{
	TRACE(_T("CMailThread::EventProcessP601()\n"));

	g_dSampleSizeX = ((CPifMessage601*)m_pEventCurr)->GetItemSampleSizeX();
	g_dSampleSizeY = ((CPifMessage601*)m_pEventCurr)->GetItemSampleSizeY();
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P601;
}

//---------------------------------------------------------------------------
// EventProcessP602
void CMailThread::EventProcessP602()
{
	TRACE(_T("CMailThread::EventProcessP602()\n"));

	g_iMovePosId = ((CPifMessage602*)m_pEventCurr)->GetItemMovePosId();
	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P602;
}

//---------------------------------------------------------------------------
// EventProcessP603
void CMailThread::EventProcessP603()
{
	TRACE(_T("CMailThread::EventProcessP603()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P603;
}

//---------------------------------------------------------------------------
// EventProcessP604
void CMailThread::EventProcessP604()
{
	TRACE(_T("CMailThread::EventProcessP604()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P604;
}

//---------------------------------------------------------------------------
// EventProcessP605
void CMailThread::EventProcessP605()
{
	TRACE(_T("CMailThread::EventProcessP605()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P605;
}

//---------------------------------------------------------------------------
// EventProcessP606
void CMailThread::EventProcessP606()
{
	TRACE(_T("CMailThread::EventProcessP606()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P606;
}

//---------------------------------------------------------------------------
// EventProcessP607
void CMailThread::EventProcessP607()
{
	TRACE(_T("CMailThread::EventProcessP607()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P607;
}

//---------------------------------------------------------------------------
// EventProcessP608
void CMailThread::EventProcessP608()
{
	TRACE(_T("CMailThread::EventProcessP608()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P608;
}

//---------------------------------------------------------------------------
// EventProcessP609
void CMailThread::EventProcessP609()
{
	TRACE(_T("CMailThread::EventProcessP609()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P609;
}

//---------------------------------------------------------------------------
// EventProcessP610
void CMailThread::EventProcessP610()
{
	TRACE(_T("CMailThread::EventProcessP610()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P610;
}

//---------------------------------------------------------------------------
// EventProcessP611
void CMailThread::EventProcessP611()
{
	TRACE(_T("CMailThread::EventProcessP611()\n"));

	m_strMailslotSendName = MAILSLOT_CHIEF;
	m_strSendCommandId = PIFCOMM_P611;
}
