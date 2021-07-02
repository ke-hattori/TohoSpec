// TcpAsyncPort.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/MeaSys.hxx"
#include "LogFile.h"
#include "CompEASETcpAsyncPort.h"
#include "TcpAsyncPort.h"

#define BUFFER_SIZE					(1024 * 4)
#define EASE_RECV_ACQUISITION_COMPLETED		"Acquisition Completed"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hNotifyWnd;
extern CLogFile* pLogFile;

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncPort

IMPLEMENT_DYNCREATE(CTcpAsyncPort, CWinThread)

CTcpAsyncPort::CTcpAsyncPort()
{
	m_Run = TRUE;
	m_bAutoDelete = FALSE;
}

CTcpAsyncPort::~CTcpAsyncPort()
{
}

BOOL CTcpAsyncPort::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return TRUE;
}

int CTcpAsyncPort::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTcpAsyncPort, CWinThread)
	//{{AFX_MSG_MAP(CTcpAsyncPort)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncPort メッセージ ハンドラ
int CTcpAsyncPort::Run()
{
	pLogFile->Logging("CTcpAsyncPort : Run()");

	char szData[BUFFER_SIZE];
	CCompEASETcpAsyncPort socketAsyncPort;

	socketAsyncPort.InitInstance();

	while(m_Run) {
		if ( socketAsyncPort.RecvAsyncData(szData) ) {
			if ( _tcsncmp(szData, EASE_RECV_ACQUISITION_COMPLETED, _tcslen(EASE_RECV_ACQUISITION_COMPLETED)) == 0 ) {
				if ( g_hNotifyWnd ) {
					::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
					pLogFile->Logging("CTcpAsyncPort : Post WM_MEAS_RECVDATA");
				}
			}
		}

		::Sleep(10);
	}

	socketAsyncPort.ExitInstance();

	pLogFile->Logging("CTcpAsyncPort : PostQuitMessage");

	::PostQuitMessage(0);

	return CWinThread::Run();
}
