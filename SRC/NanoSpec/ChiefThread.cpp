// ChiefThread.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "ChiefThread.h"
#include "ChiefView.h"
#include "ChiefExports.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChiefFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChiefThread

IMPLEMENT_DYNCREATE(CChiefThread, CWinThread)

CChiefThread::CChiefThread(CWnd* pParent /*=NULL*/)
{
	TRACE(_T("CChiefThread::CChiefThread() \n"));

	m_pcMainFrame = (CFrameWnd*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent, TRUE);
	}
	((CMainFrame*) m_pcMainFrame)->m_pDoc->ChiefLogging(_T("Started  CChiefThread."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

}

CChiefThread::~CChiefThread()
{
	TRACE(_T("CChiefThread::~CChiefThread() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
//			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CMainFrame*) m_pcMainFrame)->m_pDoc->ChiefLogging(_T("Ended    CChiefThread."));
		}
	}
}

BOOL CChiefThread::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefThread::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// Chief ダイアログを生成
	m_pMainWnd = new CChiefFrame();
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CChiefView);
	((CChiefFrame*)m_pMainWnd)->Create(AfxRegisterWndClass(0), CHIEF_DLG_CAPTION, WS_OVERLAPPEDWINDOW, CFrameWnd::rectDefault, NULL, NULL, 0, &context);
	((CChiefFrame*)m_pMainWnd)->InitialUpdateFrame(NULL, TRUE);

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CChiefThread::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefThread::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefThread, CWinThread)
	//{{AFX_MSG_MAP(CChiefThread)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefThread メッセージ ハンドラ
