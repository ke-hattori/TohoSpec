// CompEASEPollingStatus.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/MeaSys.hxx"
#include "LogFile.h"
#include "CompEASEPollingStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hNotifyWnd;
extern CLogFile* pLogFile;

#define POLLING_INTERVAL_SEC		(1)

/////////////////////////////////////////////////////////////////////////////
// CCompEASEPollingStatus

IMPLEMENT_DYNCREATE(CCompEASEPollingStatus, CWinThread)

CCompEASEPollingStatus::CCompEASEPollingStatus()
{
	m_Run = TRUE;
	m_bAutoDelete = FALSE;
}

CCompEASEPollingStatus::~CCompEASEPollingStatus()
{
}

BOOL CCompEASEPollingStatus::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return TRUE;
}

int CCompEASEPollingStatus::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCompEASEPollingStatus, CWinThread)
	//{{AFX_MSG_MAP(CCompEASEPollingStatus)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompEASEPollingStatus メッセージ ハンドラ
int CCompEASEPollingStatus::Run()
{
	pLogFile->Logging("CCompEASEPollingStatus : Run()");

	int iStatus;
	CTimer timer;

	timer.Restart(POLLING_INTERVAL_SEC);
	while(m_Run) {
		if ( timer.IsTimeout() ) {
			iStatus = MEAS_CompEASEHead_IsStatus();
			if ( g_hNotifyWnd ) {
				::PostMessage(g_hNotifyWnd, WM_MEAS_COMPEASE_STATUS, (WPARAM)iStatus, 0L);
			}
			timer.Restart();
		}

		::Sleep(10);
	}

	pLogFile->Logging("CCompEASEPollingStatus : PostQuitMessage");

	::PostQuitMessage(0);

	return CWinThread::Run();
}
