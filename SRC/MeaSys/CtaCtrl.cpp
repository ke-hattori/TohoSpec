// CtaCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CtaSio.h"
#include "LogFile.h"
#include "..\\..\\inc\globals.hxx"
#include "..\\..\\inc\ConfigFile.hxx"
#include "..\\..\\inc\MeaSys.hxx"
#include "..\\..\\inc\\NEXIOBASE.HXX"
#include "..\\..\\inc\\SharedMemory.h"
#include "CtaCtrl.h"
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	ï¿½}ï¿½Nï¿½ï¿½ï¿½ï¿½`
 */
#define	SECTION_CTA	_T("CTA")	/* NanoSpec.ini ï¿½ï¿½ï¿½ÌƒZï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½	*/
/*ï¿½ï¿½ï¿½gï¿½p*///#define	INTVL_TIME	50			/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Cï¿½}[ms]	*/
#define	SEQWAIT_TIMEOUT			(60 * 1000 * 10)	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms](10min)	*/
#define	CTA_TIMEOUT_RES			(5000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½	*/
#define	CTA_TIMEOUT_T9_NORM		(5000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] ï¿½ï¿½bï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½Ò‚ï¿½)	*/
#define	CTA_TIMEOUT_T9_ORG		(10000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] ï¿½ï¿½bï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Ò‚ï¿½)	*/
#define	CTA_TIMEOUT_T9_MEAS		(60000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] ï¿½ï¿½bï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½ï¿½ï¿½è’†)	*/
#define	CTA_TIMEOUT_T9_ALLEND	(60000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] ï¿½ï¿½bï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½Sï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Iï¿½ï¿½ï¿½ï¿½Mï¿½Ò‚ï¿½)	*/
/* modified 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
//#define	CTA_TIMEOUT_ILPI		(1000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] PI ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Mï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
//#define	CTA_TIMEOUT_ILPO		(3000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] PO ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Mï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(Sleep())	*/
/* modified 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ----------              */
#define	CTA_TIMEOUT_ILPI		(5000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] PI ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Mï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
#define	CTA_TIMEOUT_ILPO		(5000)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] PO ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Mï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(Sleep())	*/
/* modified 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */
#define	CTA_TIMEOUT_RETRY		(200)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½^ï¿½Cï¿½}(Sleep())	*/
#define	CTA_TIMEOUT_MPE_WAIT	(2500)				/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] MPE ï¿½ï¿½ï¿½Mï¿½Ò‚ï¿½ï¿½Eï¿½Fï¿½Cï¿½g(Sleep())	*/
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
/* ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½Ô�ï¿½ï¿½ï¿½`	*/
#define	PRC_INIT_IDLE	1000
#define	PRC_MEAS_IDLE	3000
#define	PRC_MEND_IDLE	4000
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
#define	CTA_ILPO_DELAY	(1500)						/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms] PO CTA ï¿½Fï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C(Sleep())	*/
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */


/*
 *	ï¿½\ï¿½ï¿½ï¿½Ì’ï¿½`
 */


/*
 *	ï¿½ï¿½ï¿½[ï¿½Jï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½[ï¿½oï¿½ï¿½ï¿½ï¿½`
 */
static CCtaCtrl* lgs_pclsCCtaCtrl;	/* ï¿½ï¿½ï¿½ï¿½ï¿½ÌƒNï¿½ï¿½ï¿½Xï¿½|ï¿½Cï¿½ï¿½ï¿½^	*/
static UINT_PTR lgs_uiTimerID = 0;	/* ï¿½^ï¿½Cï¿½} ID	*/
/* ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W	*/
UINT WM_CTACTL_TIMER = ::RegisterWindowMessage("WM_CTACTL_TIMER");	/* ï¿½^ï¿½Cï¿½}	*/
UINT WM_CTACTL_SEQKICK = ::RegisterWindowMessage("WM_CTACTL_SEQKICK");	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Lï¿½bï¿½N	*/


/*
 *	Extern ï¿½ï¿½`
 */
extern CLogFile* pLogFile;	/* from MEASYS.cpp	*/
extern TCHAR g_tszProcDir[_MAX_PATH];	/* from MEASYS.cpp	*/
extern HWND g_hNotifyWnd;	/* from MEASYS.cpp	*/
extern CSharedMemory<CTARESULT> lg_smCtaResultDataBase;	/* from MEASYS.cpp	*/
extern BOOL CtaRecalib(LPCTSTR szRecalib, double& dData);
extern BOOL bHwSimulation;	/* from MEASYS.cpp	*/
/* ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W	*/
extern UINT WM_CTASIO_RECVED;		/* ï¿½fï¿½[ï¿½^ï¿½ï¿½M				from CtaSio.cpp	*/
extern UINT WM_CTASIO_RCVBUFFULL;	/* ï¿½ï¿½Mï¿½oï¿½bï¿½tï¿½@ï¿½tï¿½ï¿½			from CtaSio.cpp	*/
extern UINT WM_CTASIO_RCVFAIL;		/* ï¿½ï¿½Mï¿½ï¿½ï¿½s(ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½ï¿½ï¿½[ï¿½v)	from CtaSio.cpp	*/


/////////////////////////////////////////////////////////////////////////////
// CCtaCtrl

IMPLEMENT_DYNCREATE(CCtaCtrl, CWinThread)

CCtaCtrl::CCtaCtrl(DWORD dwCreateFlags/* = 0*/)
{
	TRACE(_T("CCtaCtrl::CCtaCtrl() \n"));

	this->Logging(_T("constructor."));

	/* ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½	*/
	m_bInitedInstance = FALSE;
	this->m_bAutoDelete = FALSE;	/* ï¿½Iï¿½uï¿½Wï¿½Fï¿½Nï¿½gï¿½Ì�ï¿½ï¿½ï¿½ï¿½jï¿½ï¿½ï¿½ï¿½ï¿½Ö�~	*/
	lgs_pclsCCtaCtrl = this;
	m_bTimeOut = FALSE;
	m_bRcvData = FALSE;
	m_dwSeqPattern = SEQPTN_NONE;
	m_iSeqResult = 0;
	m_dwRetryCnt = 0;
	m_CtaMeasInf.iCtaMode = -1;
	m_bRecvedCMA = FALSE;
	m_bForceEnd = FALSE;
	m_bHWS = FALSE;
	m_uiTimeout_T9_Meas = CTA_TIMEOUT_T9_MEAS;
	m_uiTimeout_T9_AllEnd = CTA_TIMEOUT_T9_ALLEND;
	m_uiTimeout_ILPO = CTA_TIMEOUT_ILPO;
	m_uiTimeout_Retry = CTA_TIMEOUT_RETRY;
	m_uiTimeout_MPE_Wait = CTA_TIMEOUT_MPE_WAIT;
/* added 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- { ---------- */
	m_bErrRep = FALSE;
/* added 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- } ---------- */
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
	m_uiTimeout_ILPI = CTA_TIMEOUT_ILPI;
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
	m_uiDelay_ILPO = CTA_ILPO_DELAY;
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */

	/* ï¿½Nï¿½ï¿½ï¿½eï¿½Bï¿½Jï¿½ï¿½ï¿½Zï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`	*/
	::InitializeCriticalSection(&m_csCtrlBusy);
	::EnterCriticalSection(&m_csCtrlBusy);

	/* SIO ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	this->Logging(_T("Starting CCtaSio..."));
	m_pclsCCtaSio = 0;
	m_pclsCCtaSio = new CCtaSio(CREATE_SUSPENDED);
	if (0 == m_pclsCCtaSio) {
		/* SIO ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½\ï¿½zï¿½ï¿½ï¿½s	*/
		this->Logging(_T("Failed to create CCtaSio. (new)"));
	} else
	if (0 == m_pclsCCtaSio->m_hThread) {
		/* SIO ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½s	*/
		this->Logging(_T("Failed to start CCtaSio thread. (thread)"));
	} else {
		/* SIO ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
		this->Logging(_T("Started CCtaSio."));
		if (TRUE == this->InitializeSIO()) {

			/* ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Ì‹Nï¿½ï¿½ï¿½Ò‚ï¿½	*/
			m_dwCreateFlags = dwCreateFlags;
			BOOL l_bThreadOK = this->CreateThread(m_dwCreateFlags);
//			if (0 != this->m_hThread) {
			if (0 != l_bThreadOK) {
				if (CREATE_SUSPENDED != m_dwCreateFlags) {
					CSingleLock	l_SingleLock(&m_cSyncEvent, TRUE);
				}
			}
		}
	}

	::LeaveCriticalSection(&m_csCtrlBusy);
}

CCtaCtrl::~CCtaCtrl()
{
	TRACE(_T("CCtaCtrl::~CCtaCtrl() \n"));

	this->Logging(_T("destructor."));

	if (0 != lgs_uiTimerID) {
		::KillTimer(0, lgs_uiTimerID);
		lgs_uiTimerID = 0;
	}

	/* SIO ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	if (0 != m_pclsCCtaSio) {
		this->Logging(_T("Deleting CCtaSio..."));
		delete m_pclsCCtaSio;
		m_pclsCCtaSio = 0;
		this->Logging(_T("Deleted CCtaSio."));
	}

	/* ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Ì�ï¿½ï¿½Å‘Ò‚ï¿½	*/
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			if (TRUE == m_bInitedInstance) {
				DWORD l_dwSuspendCount = 0;
				do {
					l_dwSuspendCount = this->ResumeThread();
				} while ((0 != l_dwSuspendCount) && (0xffffffff != l_dwSuspendCount));
				::Sleep(50);
				this->PostThreadMessage(WM_QUIT, 0, 0);
				::WaitForSingleObject(m_hThread, INFINITE);
			}
		}
	}

	/* ï¿½Nï¿½ï¿½ï¿½eï¿½Bï¿½Jï¿½ï¿½ï¿½Zï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Jï¿½ï¿½	*/
	::EnterCriticalSection(&m_csCtrlBusy);	/* ï¿½æ“¾ï¿½ï¿½ï¿½ê‚½ï¿½Ü‚Ü‚ï¿½ï¿½ï¿½ï¿½mï¿½ï¿½È‚ï¿½ï¿½Ì‚Å‘Ò‚ï¿½	*/
	::LeaveCriticalSection(&m_csCtrlBusy);	/* ï¿½Jï¿½ï¿½ï¿½ï¿½ï¿½ï¿½								*/
	::DeleteCriticalSection(&m_csCtrlBusy);
}

BOOL CCtaCtrl::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("CCtaCtrl::InitInstance() \n"));

	this->Logging(_T("InitInstance()."));

//	m_bInitedInstance = TRUE;

	((CCtaSio*) m_pclsCCtaSio)->ResumeSIO(this->m_nThreadID);

//	/* ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^ï¿½Ì‘Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½	*/
//	/*		ï¿½Â‚Ü‚ï¿½Cï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Ì�ï¿½ï¿½ï¿½ï¿½ÆƒXï¿½ï¿½ï¿½bï¿½hï¿½Nï¿½ï¿½ï¿½Xï¿½Ì�\ï¿½zï¿½Ì“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
//	if (CREATE_SUSPENDED != m_dwCreateFlags) {
//		m_cSyncEvent.SetEvent();
//	}

	return TRUE;
}

int CCtaCtrl::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	TRACE(_T("CCtaCtrl::ExitInstance() \n"));

	this->Logging(_T("ExitInstance()."));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCtaCtrl, CWinThread)
	//{{AFX_MSG_MAP(CCtaCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CTACTL_SEQKICK, OnCtaCtrlSeqKick)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTACTL_TIMER, OnCtaCtrlTimer)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTASIO_RECVED, OnCtaSioRecved)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTASIO_RCVBUFFULL, OnCtaSioRcvBufFull)
	ON_REGISTERED_THREAD_MESSAGE(WM_CTASIO_RCVFAIL, OnCtaSioRcvFail)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtaCtrl message handlers

int CCtaCtrl::Run()
{
	// TODO: Add your specialized code here and/or call the base class

	static BOOL ls_bFirst = FALSE;
	if (TRUE != ls_bFirst) {
		ls_bFirst = TRUE;
		/* ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^ï¿½Ì‘Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½	*/
		/*		ï¿½Â‚Ü‚ï¿½Cï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½Ì�ï¿½ï¿½ï¿½ï¿½ÆƒXï¿½ï¿½ï¿½bï¿½hï¿½Nï¿½ï¿½ï¿½Xï¿½Ì�\ï¿½zï¿½Ì“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
		if (CREATE_SUSPENDED != m_dwCreateFlags) {
			m_cSyncEvent.SetEvent();
		}
		m_bInitedInstance = TRUE;
	}

	return CWinThread::Run();
}

BOOL CCtaCtrl::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class

	this->CtrlSeq();

	return CWinThread::OnIdle(lCount);
}

/*
 *	ï¿½ï¿½ï¿½ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Nï¿½ï¿½ ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
 */
void CCtaCtrl::OnCtaCtrlSeqKick(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTACTL_SEQKICK"), wparam, lparam);
}

/*
 *	ï¿½^ï¿½Cï¿½} ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
 */
void CCtaCtrl::OnCtaCtrlTimer(WPARAM wparam, LPARAM lparam)
{
#if 1	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Cï¿½}ï¿½Ì�ê�‡ï¿½Íƒï¿½ï¿½Oï¿½ï¿½ï¿½È‚ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
	this->Logging_WinMsg(_T("WM_CTACTL_TIMER"), wparam, lparam);
#endif
	m_bTimeOut = TRUE;
}

/*
 *	ï¿½fï¿½[ï¿½^ï¿½ï¿½M ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
 */
void CCtaCtrl::OnCtaSioRecved(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RECVED"), wparam, lparam);

	if (0 != wparam) {
		m_bRecvedCMA = TRUE;
	} else {
		m_bRcvData = TRUE;
	}
}

/*
 *	ï¿½ï¿½Mï¿½oï¿½bï¿½tï¿½@ï¿½tï¿½ï¿½ ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
 */
void CCtaCtrl::OnCtaSioRcvBufFull(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RCVBUFFULL"), wparam, lparam);

	/* ï¿½ï¿½Ê‚É’Ê’m	*/
	this->NotifyAlarm(CTASIO_ERR_RCVBUFFULL);
}

/*
 *	ï¿½ï¿½Mï¿½ï¿½ï¿½s(CCtaSio ï¿½Xï¿½ï¿½ï¿½bï¿½hï¿½ï¿½ï¿½[ï¿½v) ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
 */
void CCtaCtrl::OnCtaSioRcvFail(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RCVFAIL"), wparam, lparam);

	/* ï¿½ï¿½Ê‚É’Ê’m	*/
	this->NotifyAlarm(CTASIO_ERR_RCVFAIL);
}

/****************************************************************************
	ï¿½ï¿½ï¿½ï¿½pï¿½Ö�ï¿½
****************************************************************************/

/*
 *	CTA ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
 */
BOOL CCtaCtrl::Cta_Initialize()
{
	this->Logging(_T("Started InitializeCTA()"));

	BOOL l_bRet = TRUE;

	/* ï¿½ï¿½ï¿½ÌŠÖ�ï¿½ï¿½Ì‚İƒRï¿½Rï¿½Å�sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	::EnterCriticalSection(&m_csCtrlBusy);

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		l_bRet = this->WaitSeqEndStart(SEQPTN_INIT);
	}

	::LeaveCriticalSection(&m_csCtrlBusy);

	this->Logging(_T("Ended   InitializeCTA()"));

	return l_bRet;
}

/*
 *	CTA ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½İ’ï¿½
 *		MEAS_CTA_MODE_1	ï¿½ï¿½ï¿½[ï¿½hï¿½Pï¿½Fï¿½ï¿½Â’Pï¿½Ê�Eï¿½Iï¿½yï¿½ï¿½ï¿½[ï¿½^ï¿½fï¿½[ï¿½^ï¿½mï¿½Fï¿½È‚ï¿½
 *		MEAS_CTA_MODE_2	ï¿½ï¿½ï¿½[ï¿½hï¿½Qï¿½Fï¿½ï¿½Â’Pï¿½Ê�Eï¿½Iï¿½yï¿½ï¿½ï¿½[ï¿½^ï¿½fï¿½[ï¿½^ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½
 *		MEAS_CTA_MODE_3	ï¿½ï¿½ï¿½[ï¿½hï¿½Rï¿½Fï¿½Jï¿½Zï¿½bï¿½gï¿½Pï¿½Ê�Eï¿½Iï¿½yï¿½ï¿½ï¿½[ï¿½^ï¿½fï¿½[ï¿½^ï¿½mï¿½Fï¿½ï¿½ï¿½ï¿½
 */
BOOL CCtaCtrl::Cta_ModeSet(int iCtaMode)
{
	this->Logging(_T("Started Cta_ModeSet()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		switch (iCtaMode) {
		case MEAS_CTA_MODE_1:
		case MEAS_CTA_MODE_2:
		case MEAS_CTA_MODE_3:
			::EnterCriticalSection(&m_csCtrlBusy);
			m_CtaMeasInf.iCtaMode = iCtaMode;
			::LeaveCriticalSection(&m_csCtrlBusy);
			l_bRet = TRUE;
			break;
		default:
			l_bRet = FALSE;
			break;
		}
	}

	this->Logging(_T("Ended   Cta_ModeSet()"));

	return l_bRet;
}

/*
 *	CTA ï¿½Jï¿½Zï¿½bï¿½g IDï¿½Cï¿½ï¿½ï¿½ ID ï¿½İ’ï¿½
 */
BOOL CCtaCtrl::Cta_SetCstSample(
		LPCSTR pstrCstID,						/* ï¿½Jï¿½Zï¿½bï¿½g ID	ï¿½FASCII 80 ï¿½ï¿½ï¿½ï¿½	*/
		LPCSTR pstrSampleID,					/* ï¿½ï¿½ï¿½ ID		ï¿½FASCII 66 ï¿½ï¿½ï¿½ï¿½	*/	// 2010.03.04 K.Matsuo 80ï¿½ï¿½ï¿½ï¿½ - 14ï¿½ï¿½ï¿½ï¿½(yyyymmddhhmmss)ï¿½Å‚ï¿½
		const MAIN_RCP_INFO* pMainRcpInfo		/* ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½s	*/
	)
{
	this->Logging(_T("Started Cta_SetCstSample()"));

	BOOL l_bRet = TRUE;
	char l_szCstID[80 + 2];
	memset(l_szCstID, 0, sizeof(l_szCstID));
	strncpy(l_szCstID, pstrCstID, 80);
	char l_szSampleID[80 + 2];
	memset(l_szSampleID, 0, sizeof(l_szSampleID));
	strncpy(l_szSampleID, pstrSampleID, 80);
// 2010.03.04 K.Matsuo FAMAS IDï¿½dï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½Î�ï¿½ -->
	char l_szSystemTime[255 + 1];
	SYSTEMTIME l_systemTime;
// 2010.03.04 K.Matsuo FAMAS IDï¿½dï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½Î�ï¿½ <--

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (-1 == m_CtaMeasInf.iCtaMode) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½Å‚ï¿½ï¿½D	*/
	} else if (0 == strlen(l_szCstID)) {
		l_bRet = FALSE;	/* ï¿½Jï¿½Zï¿½bï¿½g ID ï¿½ï¿½ null ï¿½Å‚ï¿½ï¿½D	*/
	} else if (-1 != this->IsStrAlNum(l_szCstID)) {
		l_bRet = FALSE;	/* ï¿½Jï¿½Zï¿½bï¿½g ID ï¿½É–ï¿½ï¿½ï¿½ï¿½È•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	} else if (0 == strlen(l_szSampleID)) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½ ID ï¿½ï¿½ null ï¿½Å‚ï¿½ï¿½D	*/
	} else if (-1 != this->IsStrAlNum(l_szSampleID)) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½ ID ï¿½É–ï¿½ï¿½ï¿½ï¿½È•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	} else if (0 == pMainRcpInfo) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½Vï¿½sï¿½wï¿½è‚ª nullï¿½Å‚ï¿½ï¿½D	*/
	} else if ((RECIPE_NAME_LEN < _tcslen(pMainRcpInfo->MainRcpParam._CA.szRecalib[0]))
			|| (RECIPE_NAME_LEN < _tcslen(pMainRcpInfo->MainRcpParam._CA.szRecalib[1]))) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½u PGM ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		/* ï¿½Jï¿½Zï¿½bï¿½g ID ï¿½ï¿½ 80 ï¿½ï¿½ï¿½ï¿½ï¿½É‘ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 0x20 ï¿½Å–ï¿½ï¿½ß‚Ü‚ï¿½ï¿½D	*/
		memset(m_CtaMeasInf.szCstID, 0x20, sizeof(m_CtaMeasInf.szCstID));
		m_CtaMeasInf.szCstID[80] = 0x00;
		memcpy(m_CtaMeasInf.szCstID, l_szCstID, strlen(l_szCstID));

// 2010.03.04 K.Matsuo FAMAS IDï¿½dï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½Î�ï¿½ -->
// ï¿½ï¿½ï¿½IDï¿½É�Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ‚Å�AIDï¿½ï¿½ï¿½dï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½æ‚¤ï¿½É‚ï¿½ï¿½ï¿½
		GetLocalTime(&l_systemTime);
		sprintf(l_szSystemTime, "%04d%02d%02d%02d%02d%02d",
				l_systemTime.wYear,
				l_systemTime.wMonth,
				l_systemTime.wDay,
				l_systemTime.wHour,
				l_systemTime.wMinute,
				l_systemTime.wSecond);
		strcat(l_szSampleID, l_szSystemTime);
// 2010.03.04 K.Matsuo FAMAS IDï¿½dï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½Î�ï¿½ <--
		/* ï¿½ï¿½ï¿½ ID ï¿½ï¿½ 80 ï¿½ï¿½ï¿½ï¿½ï¿½É‘ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ 0x20 ï¿½Å–ï¿½ï¿½ß‚Ü‚ï¿½ï¿½D	*/
		memset(m_CtaMeasInf.szSampleID, 0x20, sizeof(m_CtaMeasInf.szSampleID));
		m_CtaMeasInf.szSampleID[80] = 0x00;
		memcpy(m_CtaMeasInf.szSampleID, l_szSampleID, strlen(l_szSampleID));
/* modified 2009.11.16 hmenjo CTA ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Lï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//		/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½u PGM ï¿½ï¿½ï¿½ï¿½ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
//		if (0 != pMainRcpInfo->MainRcpParam._CA.RecalibItem[0]) {
//			_tcscpy(m_CtaMeasInf.tszRecalibCA, pMainRcpInfo->MainRcpParam._CA.szRecalib[0]);
//		} else {
//			_tcscpy(m_CtaMeasInf.tszRecalibCA, _T(""));
//		}
//		if (0 != pMainRcpInfo->MainRcpParam._CA.RecalibItem[1]) {
//			_tcscpy(m_CtaMeasInf.tszRecalibRad, pMainRcpInfo->MainRcpParam._CA.szRecalib[1]);
//		} else {
//			_tcscpy(m_CtaMeasInf.tszRecalibRad, _T(""));
//		}
/* modified 2009.11.16 hmenjo CTA ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Lï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- 			 */
		/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½u PGM ï¿½ï¿½ï¿½ï¿½ï¿½Rï¿½sï¿½[ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
		_tcscpy(m_CtaMeasInf.tszRecalibCA, pMainRcpInfo->MainRcpParam._CA.szRecalib[0]);
		_tcscpy(m_CtaMeasInf.tszRecalibRad, pMainRcpInfo->MainRcpParam._CA.szRecalib[1]);
/* modified 2009.11.16 hmenjo CTA ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Lï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_SetCstSample()"));

	return l_bRet;
}

/*
 *	CTA ï¿½ï¿½ÂŒï¿½ï¿½İ�İ’ï¿½
 */
BOOL CCtaCtrl::Cta_SetThick(
		DWORD dwThick			/* ï¿½ï¿½ÂŒï¿½ï¿½İ�ï¿½ï¿½[0.1mm]ï¿½Cï¿½Íˆï¿½(0.0ï¿½`10.0mm)	*/
	)
{
	this->Logging(_T("Started Cta_SetThick()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (-1 == m_CtaMeasInf.iCtaMode) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½Å‚ï¿½ï¿½D	*/
	} else if (100 < dwThick) {
		l_bRet = FALSE;	/* ï¿½ï¿½ÂŒï¿½ï¿½İ�ï¿½ñ‚ª”ÍˆÍŠOï¿½Å‚ï¿½ï¿½D	*/
	} else if (0 == strlen(m_CtaMeasInf.szCstID)) {
		l_bRet = FALSE;	/* ï¿½Jï¿½Zï¿½bï¿½g ID ï¿½ï¿½ null ï¿½Å‚ï¿½ï¿½D	*/
	} else if (0 == strlen(m_CtaMeasInf.szSampleID)) {
		l_bRet = FALSE;	/* ï¿½ï¿½ï¿½ ID ï¿½ï¿½ null ï¿½Å‚ï¿½ï¿½D	*/
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		m_CtaMeasInf.dwThick = dwThick;

		l_bRet = this->WaitSeqEndStart(SEQPTN_SETINF);

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_SetThick()"));

	return l_bRet;
}

/*
 *	CTA ï¿½ï¿½ï¿½ï¿½Jï¿½n
 */
BOOL CCtaCtrl::Cta_Measure(
		int iScanCnt,							/* ï¿½ï¿½ï¿½ï¿½Ô�ï¿½	*/
		const STAGE_COORD_XYZ* pStageCoordXyz	/* ï¿½ï¿½ï¿½ï¿½Ê’uï¿½ï¿½ï¿½W	*/
	)
{
	this->Logging(_T("Started Cta_Measure()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if ((iScanCnt <= 0) || (SCAN_POINT_CTA_MAX < iScanCnt)) {
		l_bRet = FALSE;
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		memset(&m_CtaMeasData, 0, sizeof(CTAMEASDATA));
		m_CtaMeasData.i_ScanCnt = iScanCnt;
		m_CtaMeasData.lScanX = pStageCoordXyz->lX;
		m_CtaMeasData.lScanY = pStageCoordXyz->lY;
		m_CtaMeasData.lScanZ = pStageCoordXyz->lZ;

		l_bRet = this->WaitSeqEndStart(SEQPTN_MEASSTA);

		if (0 != l_bRet) {
			/* ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½Ê’mï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
			::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
			/* ï¿½Rï¿½Rï¿½Éƒï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½æ‚¢ï¿½Å‚ï¿½ï¿½D	*/
		}

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_Measure()"));

	return l_bRet;
}

/*
 *	CTA ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½
 */
BOOL CCtaCtrl::Cta_MeasEnd()
{
	this->Logging(_T("Started Cta_MeasEnd()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		l_bRet = this->WaitSeqEndStart(SEQPTN_MEASEND);

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_MeasEnd()"));

	return l_bRet;
}

/*
 *	CTA ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½
 */
BOOL CCtaCtrl::Cta_ForceEnd()
{
	this->Logging(_T("Started Cta_ForceEnd()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else {
		m_bForceEnd = TRUE;

		::EnterCriticalSection(&m_csCtrlBusy);

		m_bForceEnd = FALSE;

		l_bRet = this->WaitSeqEndStart(SEQPTN_INIT);

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_ForceEnd()"));

	return l_bRet;
}

/*
 *	CTA ï¿½ï¿½Ô�æ“¾
 */
BOOL CCtaCtrl::Cta_GetStatus(long* plStatus)
{
	this->Logging(_T("Started Cta_GetStatus()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (0 == plStatus) {
		l_bRet = FALSE;
	} else if ((SEQPTN_NONE != m_dwSeqPattern) && (SEQPTN_MAX != m_dwSeqPattern)) {
		l_bRet = FALSE;
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		m_lCtaStatus = 0;

		l_bRet = this->WaitSeqEndStart(SEQPTN_GETSTAT);

		*plStatus = m_lCtaStatus;	/*	LOBYTE(LOWORD(m_lCtaStatus));	ï¿½wï¿½bï¿½hï¿½FZ ï¿½ï¿½ï¿½Ê’u
										HIBYTE(LOWORD(m_lCtaStatus));	ï¿½ï¿½Ô‚Pï¿½Fï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
										LOBYTE(HIWORD(m_lCtaStatus));	ï¿½ï¿½Ô‚Qï¿½Fï¿½ï¿½ï¿½uï¿½ï¿½ï¿½
										HIBYTE(HIWORD(m_lCtaStatus));	(ï¿½ï¿½ï¿½gï¿½p)
										*/

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_GetStatus()"));

	return l_bRet;
}

/****************************************************************************
	ï¿½ï¿½ï¿½Ì‘ï¿½ï¿½Ö�ï¿½
****************************************************************************/

/*
 *	SIO ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
 */
BOOL CCtaCtrl::InitializeSIO()
{
	TCHAR l_tszNanoSpecIni[_MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
//	_stprintf(l_tszNanoSpecIni, _T("%s") _T(CFG_DIR) _T(NANOSPEC_INIFILENAME), g_tszProcDir);

	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if(g_lAppNameType != APP_NAME_NANO){
	strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
}
_stprintf(l_tszNanoSpecIni, _T("%s%s%s"), _T(CFG_DIR), g_tszProcDir, (LPCTSTR)strFilename);
// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--

	TCHAR l_tszText[255];
	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½gï¿½Ç�oï¿½ï¿½	*/
	::GetPrivateProfileString(SECTION_CTA, _T("RETRY"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	m_dwRetryCnt = _ttoi(l_tszText);
	if (m_dwRetryCnt <= 0) {
		m_dwRetryCnt = 0;
	}
	/* HWS ï¿½Ç�oï¿½ï¿½	*/
	::GetPrivateProfileString(SECTION_CTA, _T("HWS"), _T("1"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	int l_iHWS1 = _ttoi(l_tszText);
	::GetPrivateProfileString(SECTION_CTA, _T("HWS"), _T("2"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	int l_iHWS2 = _ttoi(l_tszText);
	if ((1 == l_iHWS1) && (2 == l_iHWS2)) {
		/* ï¿½Lï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
		m_bHWS = (0 != bHwSimulation)? TRUE : FALSE;
	} else {
		m_bHWS = (0 != l_iHWS2)? TRUE : FALSE;
	}
	/* ï¿½ï¿½bï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½ï¿½ï¿½è’†)	*/
	m_uiTimeout_T9_Meas = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_T9_MEAS"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_T9_Meas < 500) || (CTA_TIMEOUT_T9_MEAS < m_uiTimeout_T9_Meas)) {
		m_uiTimeout_T9_Meas = CTA_TIMEOUT_T9_MEAS;
	}
	/* ï¿½ï¿½bï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g(ï¿½Sï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½Iï¿½ï¿½ï¿½ï¿½Mï¿½Ò‚ï¿½)	*/
	m_uiTimeout_T9_AllEnd = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_T9_ALLEND"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_T9_AllEnd < 500) || (CTA_TIMEOUT_T9_ALLEND < m_uiTimeout_T9_AllEnd)) {
		m_uiTimeout_T9_AllEnd = CTA_TIMEOUT_T9_ALLEND;
	}
	/* PO ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Mï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
	m_uiTimeout_ILPO = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_ILPO"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_ILPO < 10) || (CTA_TIMEOUT_ILPO < m_uiTimeout_ILPO)) {
		m_uiTimeout_ILPO = CTA_TIMEOUT_ILPO;
	}
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
	/* PI ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Mï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
	m_uiTimeout_ILPI = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_ILPI"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_ILPI < 10) || (CTA_TIMEOUT_ILPI < m_uiTimeout_ILPI)) {
		m_uiTimeout_ILPI = CTA_TIMEOUT_ILPI;
	}
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */
	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½^ï¿½Cï¿½}	*/
	m_uiTimeout_Retry = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_RETRY"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_Retry < 200) || (5000 < m_uiTimeout_Retry)) {
		m_uiTimeout_Retry = CTA_TIMEOUT_RETRY;
	}
	/* MPE ï¿½ï¿½ï¿½Mï¿½Ò‚ï¿½ï¿½Eï¿½Fï¿½Cï¿½g	*/
	m_uiTimeout_MPE_Wait = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_MPE_WAIT"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_MPE_Wait < 10) || (5000 < m_uiTimeout_MPE_Wait)) {
		m_uiTimeout_MPE_Wait = CTA_TIMEOUT_MPE_WAIT;
	}
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
	/* PO CTA ï¿½Fï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C	*/
	m_uiDelay_ILPO = ::GetPrivateProfileInt(SECTION_CTA, _T("ILPO_DELAY"), -1, l_tszNanoSpecIni);
	if ((m_uiDelay_ILPO < 10) || (10000 < m_uiDelay_ILPO)) {
		m_uiDelay_ILPO = CTA_ILPO_DELAY;
	}
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */

// 2009.11.18 K.Matsuo -->
//	/* ï¿½|ï¿½[ï¿½gï¿½Ô�ï¿½ï¿½Ç�oï¿½ï¿½	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("COM"), _T("1"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iComNo = _ttoi(l_tszText);
//	if (l_iComNo <= 0) {
//		l_iComNo = 1;
//	}
//	/* ï¿½{ï¿½[ï¿½ï¿½ï¿½[ï¿½gï¿½Ç�oï¿½ï¿½	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("BaudRate"), _T("9600"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iBaudRate = _ttoi(l_tszText);
//	/* ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Ç�oï¿½ï¿½	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("ByteSize"), _T("8"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iByteSize = _ttoi(l_tszText);
//	/* ï¿½pï¿½ï¿½ï¿½eï¿½Bï¿½Ç�oï¿½ï¿½	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("Parity"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iParity = _ttoi(l_tszText);
//	switch (l_iParity) {
//	case 1:		l_iParity = ODDPARITY; break;
//	case 2:		l_iParity = EVENPARITY; break;
//	case 3:		l_iParity = MARKPARITY; break;
//	case 4:		l_iParity = SPACEPARITY; break;
//	case 0:
//	default:	l_iParity = NOPARITY; break;
//	}
//	/* ï¿½Xï¿½gï¿½bï¿½vï¿½rï¿½bï¿½gï¿½Ç�oï¿½ï¿½	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("StopBits"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iStopBits = _ttoi(l_tszText);
//	switch (l_iStopBits) {
//	case 1:		l_iStopBits = ONE5STOPBITS; break;
//	case 2:		l_iStopBits = TWOSTOPBITS; break;
//	case 0:
//	default:	l_iStopBits = ONESTOPBIT; break;
//	}
//	if (TRUE != ((CCtaSio*) m_pclsCCtaSio)->Initialize(l_iComNo, l_iBaudRate, l_iByteSize, l_iParity, l_iStopBits, m_bHWS)) {
//		return FALSE;
//	}

	///// Get Config Data /////
	COM_SETTING comSetting;
	ConfigFile_GetNanoSpecIni(&comSetting, CONFIG_FILE_COM);

	if (TRUE != ((CCtaSio*) m_pclsCCtaSio)->Initialize(comSetting.CaComPort.szCommNo, comSetting.CaComPort.szBaudRate, comSetting.CaComPort.szByteSize, comSetting.CaComPort.szParity, comSetting.CaComPort.szStopBits, m_bHWS)) {
		return FALSE;
	}
// 2009.11.18 K.Matsuo <--

	return TRUE;
}

/*
 *	ï¿½ï¿½ï¿½O
 */
void CCtaCtrl::Logging(LPCTSTR strLog)
{
	if (0 == strLog) {
		return;
	}

	CString l_strLogText;
	l_strLogText.Format(_T("CCtaCtrl : %s"), strLog);

	pLogFile->Logging(l_strLogText);
}

/*
 *	ï¿½ï¿½ï¿½O - ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½Mï¿½p
 */
void CCtaCtrl::Logging_WinMsg(LPCTSTR strLog, WPARAM wParam, LPARAM lParam)
{
	CString l_strLogText;
	l_strLogText.Format(_T("Received %s (WPARAM = 0x%08x, LPARAM = 0x%08x)."), strLog, wParam, lParam);
	this->Logging(l_strLogText);
}

/*
 *	ï¿½ï¿½ï¿½ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½X
 */
int CCtaCtrl::CtrlSeq()
{
	int l_iRet = 0;

	/* CTA ï¿½ï¿½ï¿½jï¿½bï¿½gï¿½ï¿½ï¿½ï¿½Ì�ï¿½Mï¿½mï¿½F ------------------------------------------*/
	PACKET_CODE l_enumPacketCode = PKT_MAX;
	COMMAND_CODE l_enumCmdCode = EVT_MAX;
	char l_szRcvData[PKT_DATA_LEN_MAX + 1];
	CTARCVDATA l_CtaRcvData;
	memset(&l_CtaRcvData, 0, sizeof(CTARCVDATA));
	if (0 != m_bRcvData) {
		/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½Lï¿½ï¿½	*/
		m_bRcvData = FALSE;
		int l_iRslt =	((CCtaSio*) m_pclsCCtaSio)->GetRcvData(
								&l_enumPacketCode,
								&l_enumCmdCode,
								l_szRcvData
							);
		l_CtaRcvData.pdwPktCode = (LPDWORD) &l_enumPacketCode;
		l_CtaRcvData.pdwCmdCode = (LPDWORD) &l_enumCmdCode;
		l_CtaRcvData.pszRcvData = l_szRcvData;
		if (CTASIO_ERR_NORCVDATA == l_iRslt) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½Í‚ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½D	*/
		} else if (0 != l_iRslt) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½Gï¿½ï¿½ï¿½[ï¿½Å‚ï¿½ï¿½D	*/
			if (PKT_EVT == *l_CtaRcvData.pdwPktCode) {
				this->SendErrRes(l_iRslt);	/* ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Ô‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
			}
			/* ï¿½ï¿½Ê‚É’Ê’m	*/
			this->NotifyAlarm(l_iRslt);
		} else {
			/*	ï¿½Eï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½pï¿½Pï¿½bï¿½gï¿½ï¿½M
				ï¿½Eï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½pï¿½Pï¿½bï¿½gï¿½ï¿½M
				ï¿½Eï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½pï¿½Pï¿½bï¿½gï¿½ï¿½M	*/
			/* ï¿½eï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½É�ï¿½Mï¿½Ê’m	*/
			l_CtaRcvData.bRecved = TRUE;
			/* ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½pï¿½Pï¿½bï¿½gï¿½Ì�ê�‡ï¿½ÍƒRï¿½Rï¿½Åƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Ô‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
			if (PKT_EVT == l_enumPacketCode) {
				this->SendEvtRes(&l_CtaRcvData);
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½Ì�ê�‡ï¿½Í�CChief ï¿½Ö‚Ì’Ê’mï¿½ï¿½ï¿½Kï¿½vï¿½Å‚ï¿½ï¿½D
					ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½Rï¿½[ï¿½h(000)ï¿½Æ�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì�ê�‡ï¿½Í’Ê’mï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D	*/
				if (EVT_REP == l_enumCmdCode) {
					if ((0 != l_CtaRcvData.dwErrCode) && (SEQPTN_INIT != m_dwSeqPattern)) {
						/* ï¿½ï¿½Ê‚É’Ê’m	*/
						this->NotifyAlarm(l_CtaRcvData.dwErrCode);
					}
				}
			}
		}
	}

	/* ï¿½eï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½X --------------------------------------------------------*/
	switch (m_dwSeqPattern) {
	case SEQPTN_NONE:		/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½	*/
		break;
	case SEQPTN_INIT:		/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
		l_iRet = this->SeqInitCTA(&l_CtaRcvData);
		break;
	case SEQPTN_SETINF:		/* ï¿½ï¿½ï¿½İ’ï¿½	*/
		l_iRet = this->SeqSetInf(&l_CtaRcvData);
		break;
	case SEQPTN_MEASSTA:	/* ï¿½ï¿½ï¿½ï¿½Jï¿½n	*/
		l_iRet = this->SeqMeasStart(&l_CtaRcvData);
		break;
	case SEQPTN_MEASEND:	/* ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½	*/
		l_iRet = this->SeqMeasEnd(&l_CtaRcvData);
		break;
	case SEQPTN_GETSTAT:	/* ï¿½ï¿½Ô�æ“¾	*/
		l_iRet = this->SeqGetStat(&l_CtaRcvData);
		break;
	default:
		ASSERT(0);
		m_dwSeqPattern = SEQPTN_NONE;
		break;
	}

	/* ï¿½ï¿½ï¿½Ì‘ï¿½ --------------------------------------------------------------*/
	/* ï¿½Rï¿½Rï¿½Ü‚Å‚É�ï¿½Mï¿½Ê’mï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Í�C
		ï¿½sï¿½vï¿½Æ”ï¿½ï¿½fï¿½ï¿½ï¿½Ä�Ì‚Ä‚Ü‚ï¿½ï¿½D	*/
	if (TRUE == l_CtaRcvData.bRecved) {
		l_CtaRcvData.bRecved = FALSE;
	}

	/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Í�ï¿½ï¿½ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Lï¿½bï¿½Nï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	if (0 == ((CCtaSio*) m_pclsCCtaSio)->IsExistRcvData()) {
		/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
		m_bRcvData = TRUE;
		this->KickCtrlSeq(1);
	}

	return l_iRet;
}

/*
 *	ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Ì‹Nï¿½ï¿½ï¿½wï¿½ß‚Æ�Iï¿½ï¿½ï¿½Ò‚ï¿½
 */
BOOL CCtaCtrl::WaitSeqEndStart(SEQ_PATTERN enumSeqPattern)
{
	BOOL l_bRet = TRUE;

	if ((enumSeqPattern <= SEQPTN_NONE) || (SEQPTN_MAX <= enumSeqPattern)) {
		this->Logging(_T("Invalid Seq Pattern in WaitSeqEndStart()."));
		return FALSE;
	}

	m_hEvWaitEndSeq = ::CreateEvent(0, TRUE, FALSE, 0);
	if (0 == m_hEvWaitEndSeq) {
		this->Logging(_T("Failed to create event - m_hEvWaitEndSeq in WaitSeqEndStart()."));
		return FALSE;
	}

	/* ï¿½ï¿½ï¿½ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Lï¿½bï¿½N	*/
	m_dwSeqPattern = enumSeqPattern;
	m_iSeqResult = 0;
	if (0 == this->KickCtrlSeq(2)) {
		this->Logging(_T("Failed to KickCtrlSeq() in WaitSeqEndStart()."));
		l_bRet = FALSE;
	}

	if (TRUE == l_bRet) {
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvWaitEndSeq, SEQWAIT_TIMEOUT)) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			this->Logging(_T("Timeout WaitForSingleObject() in WaitSeqEndStart()."));
			l_bRet = FALSE;
		} else {
			if (0 != m_iSeqResult) {
				l_bRet = FALSE;
			}
		}
	}

	if (0 == ::ResetEvent(m_hEvWaitEndSeq)) {
		this->Logging(_T("Failed to reset event - m_hEvWaitEndSeq in WaitSeqEndStart()."));
		l_bRet = FALSE;
	}
	if (0 == ::CloseHandle(m_hEvWaitEndSeq)) {
		this->Logging(_T("Failed to close event handle in WaitSeqEndStart()."));
		return FALSE;
	}

	return l_bRet;
}

/*
 *	ï¿½ï¿½ï¿½ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Lï¿½bï¿½N
 */
BOOL CCtaCtrl::KickCtrlSeq(int iKickNo/* = -1*/)
{
	BOOL l_bRet = TRUE;

	TCHAR l_tszLogMsg[80];
	_stprintf(l_tszLogMsg, _T("Post thread message WM_CTACTL_SEQKICK in KickCtrlSeq(%d)."), iKickNo);
	this->Logging(l_tszLogMsg);

	if (0 == this->PostThreadMessage(WM_CTACTL_SEQKICK, 0, 0)) {
		this->Logging(_T("Failed to PostThreadMessage() in KickCtrlSeq()."));
		l_bRet = FALSE;
	}

	return l_bRet;
}

/*
 *	ï¿½^ï¿½Cï¿½} ï¿½Rï¿½[ï¿½ï¿½ï¿½oï¿½bï¿½Nï¿½Ö�ï¿½
 */
void CALLBACK TimerHandler(
		HWND		hWnd,		/* ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Ìƒnï¿½ï¿½ï¿½hï¿½ï¿½	*/
		UINT		uiMsg,		/* WM_TIMER ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W	*/
		UINT_PTR	idEvent,	/* ï¿½^ï¿½Cï¿½}ï¿½Ì�ï¿½ï¿½Ê�q		*/
		DWORD		dwTime		/* ï¿½ï¿½ï¿½İ‚ÌƒVï¿½Xï¿½eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
	)
{
	if (WM_TIMER == uiMsg) {
		if (lgs_uiTimerID == idEvent) {
			if (0 == ::KillTimer(0, lgs_uiTimerID)) {ASSERT(0);}
			if (0 != lgs_pclsCCtaCtrl) {
				lgs_pclsCCtaCtrl->PostThreadMessage(WM_CTACTL_TIMER, 0, 0);
			}
		}
	}
}

/*
 *	ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½İ’ï¿½/ï¿½ï¿½ï¿½ï¿½
 */
BOOL CCtaCtrl::SetTimeOut(BOOL bMode, UINT uiElapse)
{
	BOOL l_bRet = TRUE;

	if (0 == bMode) {
		/* ï¿½ï¿½ï¿½ï¿½	*/
		if ((0 != lgs_uiTimerID) && (0 == m_bTimeOut)) {
			BOOL l_bRstl = ::KillTimer(0, lgs_uiTimerID);
			lgs_uiTimerID = 0;
			m_bTimeOut = FALSE;
			if (0 == l_bRstl) {
				l_bRet = FALSE;
//				ASSERT(0);	/* ï¿½sï¿½vï¿½Å‚ï¿½ï¿½D	*/
			}
		}
	} else {
		/* ï¿½İ’ï¿½(ï¿½Jï¿½n)	*/
		m_bTimeOut = FALSE;
		if (0 == uiElapse) {
			l_bRet = FALSE;
		} else {
			lgs_uiTimerID = ::SetTimer(0, 0, uiElapse, (TIMERPROC) TimerHandler);
			if (0 == lgs_uiTimerID) {
				l_bRet = FALSE;
				ASSERT(0);
			}
		}
	}

	return l_bRet;
}

/*
 *	ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½
 */
int CCtaCtrl::AnaRcvData(LPCTARCVDATA pCtaRcvData, LPCTADATAINF pCtaDataInf)
{
	int l_iRet = 0;

	if (0 == pCtaRcvData) {
		l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
	} else if (0 == pCtaRcvData->bRecved) {
		l_iRet = CTACTL_ERR_DATANONE;	/* ï¿½fï¿½[ï¿½^ï¿½Í‚ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D	*/
	} else {
		if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
			/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X --------------------------------------------*/
			if (3 != strlen(pCtaRcvData->pszRcvData)) {
				l_iRet = CTACTL_ERR_DATAFORMAT;	/* ï¿½fï¿½[ï¿½^(ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g)ï¿½Gï¿½ï¿½ï¿½[	*/
			} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
				l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
			} else {
				if (0 != pCtaDataInf) {
					pCtaDataInf->ResERR.dwErrRes = atol(pCtaRcvData->pszRcvData);
				}
			}
		} else {
			/* ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½Cï¿½Ü‚ï¿½ï¿½Í�Cï¿½Cï¿½xï¿½ï¿½ï¿½g --------------------------------*/
			switch (*pCtaRcvData->pdwCmdCode) {
			case CMD_STA:	/* STA ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
				if (3 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* ï¿½fï¿½[ï¿½^(ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g)ï¿½Gï¿½ï¿½ï¿½[	*/
				} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
				} else {
					DWORD l_dwZState = pCtaRcvData->pszRcvData[0] & 0x0f;
					DWORD l_dwStatus1 = pCtaRcvData->pszRcvData[1] & 0x0f;
					DWORD l_dwStatus2 = pCtaRcvData->pszRcvData[2] & 0x0f;
					if ((l_dwZState < 1) || (3 < l_dwZState)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
					} else if ((l_dwStatus1 < 1) || (3 < l_dwStatus1)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
					} else if ((l_dwStatus2 < 1)
							|| (4 < l_dwStatus2)
							|| (2 == l_dwStatus2)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
					} else {
						if (0 != pCtaDataInf) {
							pCtaDataInf->CmdSTA.dwZState = l_dwZState;
							pCtaDataInf->CmdSTA.dwStatus1 = l_dwStatus1;
							pCtaDataInf->CmdSTA.dwStatus2 = l_dwStatus2;
						}
					}
				}
				break;
			case CMD_RST:	/* RST ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_ORG:	/* ORG ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_MOD:	/* MOD ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_CID:	/* CID ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_PID:	/* PID ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_SPT:	/* SPT ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_MST:	/* MST ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_MPE:	/* MPE ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case CMD_MCE:	/* MCE ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½X ------------------------------*/
			case EVT_CMO:	/* CMO ï¿½Cï¿½xï¿½ï¿½ï¿½g --------------------------------*/
			case EVT_CST:	/* CST ï¿½Cï¿½xï¿½ï¿½ï¿½g --------------------------------*/
			case EVT_CMA:	/* CMA ï¿½Cï¿½xï¿½ï¿½ï¿½g --------------------------------*/
				if (0 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* ï¿½fï¿½[ï¿½^(ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g)ï¿½Gï¿½ï¿½ï¿½[	*/
				}
				break;
			case EVT_REP:	/* REP ï¿½Cï¿½xï¿½ï¿½ï¿½g --------------------------------*/
				if (3 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* ï¿½fï¿½[ï¿½^(ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g)ï¿½Gï¿½ï¿½ï¿½[	*/
				} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
				} else {
					if (0 != pCtaDataInf) {
						pCtaDataInf->EvtREP.dwErrCode = atol(pCtaRcvData->pszRcvData);
					}
				}
				break;
			case EVT_CMP:	/* CMP ï¿½Cï¿½xï¿½ï¿½ï¿½g --------------------------------*/
				if (17 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* ï¿½fï¿½[ï¿½^(ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g)ï¿½Gï¿½ï¿½ï¿½[	*/
				} else {
					struct {
						char l_szPointNo[8];	/* 4 ï¿½|ï¿½Cï¿½ï¿½ï¿½g No.	*/
						char l_szCAngle[8];		/* 4 ï¿½Ú�Gï¿½p[0.1degree]	*/
						char l_szRadius[8];		/* 5 ï¿½ï¿½ï¿½a[0.1um]	*/
						char l_szLiquid[8];		/* 4 ï¿½tï¿½ï¿½[0.01uL]	*/
					} l_DataTmp;
					memset(&l_DataTmp, 0, sizeof(l_DataTmp));
					memcpy(l_DataTmp.l_szPointNo, &(pCtaRcvData->pszRcvData[0]), 4);
					memcpy(l_DataTmp.l_szCAngle, &(pCtaRcvData->pszRcvData[4]), 4);
					memcpy(l_DataTmp.l_szRadius, &(pCtaRcvData->pszRcvData[8]), 5);
					memcpy(l_DataTmp.l_szLiquid, &(pCtaRcvData->pszRcvData[13]), 4);
					char *pBlankPos;
					int l_iPointNo;
					if (0 == l_iRet) {	/* ï¿½|ï¿½Cï¿½ï¿½ï¿½g No.	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szPointNo)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szPointNo, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iPointNo = atol(l_DataTmp.l_szPointNo);
							if ((l_iPointNo <= 0) || (SCAN_POINT_CTA_MAX < l_iPointNo)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
							}
						}
					}
					int l_iCAngle;
					if (0 == l_iRet) {	/* ï¿½Ú�Gï¿½p	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szCAngle)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szCAngle, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iCAngle = atol(l_DataTmp.l_szCAngle);
							if ((l_iCAngle < 0) || (9999 < l_iCAngle)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
							}
						}
					}
					int l_iRadius;
					if (0 == l_iRet) {	/* ï¿½ï¿½ï¿½a	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szRadius)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szRadius, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iRadius = atol(l_DataTmp.l_szRadius);
							if ((l_iRadius < 0) || (99999 < l_iRadius)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
							}
						}
					}
					int l_iLiquid;
					if (0 == l_iRet) {	/* ï¿½tï¿½ï¿½	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szLiquid)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szLiquid, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iLiquid = atol(l_DataTmp.l_szLiquid);
							if ((l_iLiquid < 0) || (9999 < l_iLiquid)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
							}
						}
					}
					if (0 == l_iRet) {	/* ï¿½ï¿½ï¿½×‚Ä�ï¿½ï¿½ï¿½È‚Ì‚ÅƒRï¿½sï¿½[	*/
						pCtaDataInf->EvtCMP.iPointNo = l_iPointNo;
						pCtaDataInf->EvtCMP.iCAngle = l_iCAngle;
						pCtaDataInf->EvtCMP.iRadius = l_iRadius;
						pCtaDataInf->EvtCMP.iLiquid = l_iLiquid;
					}
				}
				break;
			default:
				ASSERT(0);
				break;
			}
		}
	}

	return l_iRet;
}

/*
 *	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Pï¿½Oï¿½iï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
 *		ï¿½ß‚ï¿½lï¿½F	-1ï¿½Fï¿½ï¿½ï¿½×‚Ä‚O(0x30)ï¿½`ï¿½X(0x39)ï¿½Å‚ï¿½ï¿½ï¿½ï¿½D
 *					ï¿½ÈŠOï¿½Fï¿½Pï¿½Oï¿½iï¿½ï¿½ï¿½ÈŠOï¿½Ì•ï¿½ï¿½ï¿½ï¿½Ê’u(ï¿½Iï¿½tï¿½Zï¿½bï¿½g)
 */
int CCtaCtrl::IsStrDecimal(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == isdigit(pstrSrc[i])) {
			l_iRet = i;
			break;
		}
		i++;
	}

	return l_iRet;
}

/*
 *	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Pï¿½Oï¿½iï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½
 *		ï¿½Pï¿½Oï¿½iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(+/-ï¿½Fï¿½æ“ªï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ï¿½)ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½N(ï¿½ÅŒï¿½Ì•ï¿½ï¿½Ì‚ï¿½)ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
 *		ï¿½ß‚ï¿½lï¿½F	-1ï¿½Fï¿½ï¿½ï¿½×‚Ä‚O(0x30)ï¿½`ï¿½X(0x39)ï¿½Å‚ï¿½ï¿½ï¿½ï¿½D
 *					ï¿½ÈŠOï¿½Fï¿½Pï¿½Oï¿½iï¿½ï¿½ï¿½ÈŠOï¿½Ì•ï¿½ï¿½ï¿½ï¿½Ê’u(ï¿½Iï¿½tï¿½Zï¿½bï¿½g)
 */
int CCtaCtrl::IsStrDecimalSign(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	BOOL l_bFoundBlank = FALSE;
	BOOL l_bSign = FALSE;
	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == i) {
			/* ï¿½Pï¿½ï¿½ï¿½Ú‚Í�ï¿½ï¿½ï¿½(0ï¿½`9)ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(+/-)ï¿½ÈŠOï¿½ÍƒGï¿½ï¿½ï¿½[	*/
			if (0 != isdigit(pstrSrc[i])) {
				;	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			} else if (('+' != pstrSrc[i]) && ('-' != pstrSrc[i])) {
				l_bSign = TRUE;	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			} else {
				l_iRet = i;
				break;
			}
		} else if (1 == i) {
			/* ï¿½Qï¿½ï¿½ï¿½ï¿½	*/
			if (0 != l_bSign) {
				/* ï¿½Pï¿½ï¿½ï¿½Ú‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡	*/
				if (0 == isdigit(pstrSrc[i])) {
					/* ï¿½Qï¿½ï¿½ï¿½Ú‚Í�ï¿½ï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½ï¿½Î‚È‚ï¿½È‚ï¿½	*/
					l_iRet = i;
					break;
				}
			} else if (0 != isdigit(pstrSrc[i])) {
				;	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			} else if (' ' == pstrSrc[i]) {
				l_bFoundBlank = TRUE;	/* ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½N(0x20)ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			} else {
				l_iRet = i;
				break;
			}
		} else {
			/* ï¿½Rï¿½ï¿½ï¿½ÚˆÈ�~	*/
			if (0 != l_bFoundBlank) {
				/* ï¿½È‘Oï¿½Éƒuï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡	*/
				if (' ' != pstrSrc[i]) {
					/* ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½Å‚ï¿½ï¿½è‘±ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½Î‚È‚ï¿½È‚ï¿½	*/
					l_iRet = i;
					break;
				}
			} else if (0 != isdigit(pstrSrc[i])) {
				;	/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			} else if (' ' == pstrSrc[i]) {
				l_bFoundBlank = TRUE;	/* ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½N(0x20)ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
			} else {
				l_iRet = i;
				break;
			}
		}
		i++;
	}

	return l_iRet;
}

/*
 *	ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‰pï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
 *		ï¿½ß‚ï¿½lï¿½F	-1ï¿½Fï¿½ï¿½ï¿½×‚Ä‚O(0x30)ï¿½`ï¿½X(0x39)ï¿½CA(0x41)ï¿½`Z(0x5a)ï¿½Ca(0x61)ï¿½`z(0x7a)ï¿½Å‚ï¿½ï¿½ï¿½ï¿½D
 *					ï¿½ÈŠOï¿½Fï¿½pï¿½ï¿½ï¿½ÈŠOï¿½Ì•ï¿½ï¿½ï¿½ï¿½Ê’u(ï¿½Iï¿½tï¿½Zï¿½bï¿½g)
 */
int CCtaCtrl::IsStrAlNum(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == isalnum(pstrSrc[i])) {
			l_iRet = i;
			break;
		}
		i++;
	}

	return l_iRet;
}

/*
 *	ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½M
 */
int CCtaCtrl::SendErrRes(int iResult)
{
	int l_iRet = 0;

	int l_iErrCode = 0;
	switch (iResult) {
	case CTASIO_ERR_INVALID_PKT:	/* ï¿½pï¿½Pï¿½bï¿½gï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½ÍˆÍŠOï¿½Å‚ï¿½	*/
		l_iErrCode = CTACTL_ERCD_HEAD;	/* ï¿½wï¿½bï¿½_ï¿½Gï¿½ï¿½ï¿½[	*/
		break;
	case CTASIO_ERR_INVALID_CMD:	/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½ÍˆÍŠOï¿½Å‚ï¿½	*/
	case CTASIO_ERR_INVALID_EVT:	/* ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½ÍˆÍŠOï¿½Å‚ï¿½	*/
	case CTASIO_ERR_CTRLCODE:		/* ï¿½ï¿½ï¿½ï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
	case CTACTL_ERR_DATAFORMAT:		/* ï¿½fï¿½[ï¿½^(ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½g)ï¿½Gï¿½ï¿½ï¿½[	*/
	case CTACTL_ERR_PARAMERR:		/* ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Gï¿½ï¿½ï¿½[	*/
		l_iErrCode = CTACTL_ERCD_CMD;	/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Gï¿½ï¿½ï¿½[	*/
		break;
	case CTASIO_ERR_SUM:			/* ï¿½`ï¿½Fï¿½bï¿½Nï¿½Tï¿½ï¿½ï¿½sï¿½ï¿½v	*/
		l_iErrCode = CTACTL_ERCD_SUM;	/* ï¿½`ï¿½Fï¿½bï¿½Nï¿½Tï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[	*/
		break;
	case CTASIO_ERR_TOOSHORT:		/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½	*/
	case CTASIO_ERR_INVALIDLEN:		/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Ù�ï¿½	*/
	case CTASIO_ERR_NOLF:			/* LF ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½	*/
	case CTASIO_ERR_NOCR:			/* CR ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½	*/
		l_iErrCode = CTACTL_ERCD_LEN;	/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[	*/
		break;
		break;
	default:
		/* ï¿½ï¿½ï¿½ï¿½`ï¿½ï¿½ï¿½Ê‚Í‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D	*/
		break;
	}

	if (0 != l_iErrCode) {
		l_iErrCode = l_iErrCode % 1000;
		char l_szData[4];
		sprintf(l_szData, "%03d", l_iErrCode);
		l_iRet = ((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_ERR, CMD_STA, l_szData);
	}

	return l_iRet;
}

/*
 *	ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½É‘Î‚ï¿½ï¿½éƒŒï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ğ‘—�Mï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
 */
int CCtaCtrl::SendEvtRes(LPCTARCVDATA pCtaRcvData)
{
	int l_iRet = 0;

	CTADATAINF l_CtaDataInf;
	int l_iRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
	pCtaRcvData->iRslt = l_iRslt;
	pCtaRcvData->bAnaed = TRUE;
	if (0 != l_iRslt) {
		/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½Å‰ï¿½ï¿½ï¿½	*/
		l_iRet = this->SendErrRes(l_iRslt);
	} else {
		/* ï¿½ï¿½ï¿½íƒŒï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½	*/
		l_iRet = ((CCtaSio*) m_pclsCCtaSio)->CmdSend(PKT_RES, (COMMAND_CODE) *pCtaRcvData->pdwCmdCode, "");
		if (EVT_REP == *pCtaRcvData->pdwCmdCode) {
			pCtaRcvData->dwErrCode = l_CtaDataInf.EvtREP.dwErrCode;
/* added 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- { ---------- */
			if (CTACTL_ERCD_RST == pCtaRcvData->dwErrCode) {
				m_bErrRep = FALSE;
			} else {
				m_bErrRep = TRUE;
			}
/* added 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- } ---------- */
		}
	}

	if (0 == l_iRet) {
		l_iRet = pCtaRcvData->iRslt;
	}

	return l_iRet;
}

/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
/*
 *	ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Fï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
 */
#define	PRC_INIT_CMD_RST0	(PRC_INIT_IDLE + 100)
#define	PRC_INIT_CMD_STA	(PRC_INIT_IDLE + 200)
#define	PRC_INIT_CMD_RST	(PRC_INIT_IDLE + 300)
#define	PRC_INIT_CMD_ORG	(PRC_INIT_IDLE + 400)
#define	PRC_INIT_EVT_CMO	(PRC_INIT_IDLE + 500)
#define	PRC_INIT_ILPI_ON	(PRC_INIT_IDLE + 600)
int CCtaCtrl::SeqInitCTA(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = PRC_INIT_CMD_STA;
//	static int ls_iPrc = PRC_INIT_CMD_RST0;
///* modified 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- { ---------- */
////	static BOOL ls_bRST2nd = FALSE;
///* modified 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ----------              */
//	static BOOL ls_bRST2nd = TRUE;
///* modified 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- } ---------- */
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_INIT_CMD_RST0 + 00:	/* ï¿½gï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½g ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqInitCTA()"));
			nexioCA_Interlock(TRUE);	/* CTAILPO ï¿½Iï¿½ï¿½	*/
			::Sleep(m_uiTimeout_ILPO);
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_RST, "");
		ls_iPrc = PRC_INIT_CMD_RST0 + 10;
		break;
	case PRC_INIT_CMD_RST0 + 10:	/* ï¿½gï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_RST == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					ls_iPrc = PRC_INIT_CMD_STA;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_RST0;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_INIT_CMD_STA + 00:		/* ï¿½ï¿½Ô—vï¿½ï¿½ --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqInitCTA()"));
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_STA, "");
		ls_iPrc = PRC_INIT_CMD_STA + 10;
		break;
	case PRC_INIT_CMD_STA + 10:		/* ï¿½ï¿½Ô—vï¿½ï¿½ï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_STA == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else if ((1 != l_CtaDataInf.CmdSTA.dwStatus1) && (2 != l_CtaDataInf.CmdSTA.dwStatus1)) {
					/* ï¿½uï¿½ï¿½ï¿½è’†ï¿½Fï¿½Pï¿½vï¿½ï¿½ï¿½uï¿½Ò‹@ï¿½ï¿½ï¿½Fï¿½Qï¿½vï¿½ÈŠOï¿½Ì�ê�‡	*/
					l_iRslt = CTACTL_ERR_EQBUSY;
				} else if ((1 != l_CtaDataInf.CmdSTA.dwStatus2) && (3 != l_CtaDataInf.CmdSTA.dwStatus2)) {
					/* ï¿½uï¿½Pï¿½Fï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½uï¿½wï¿½bï¿½hï¿½Gï¿½ï¿½ï¿½[ï¿½Fï¿½Rï¿½vï¿½ÈŠOï¿½Ì�ê�‡	*/
					l_iRslt = CTACTL_ERR_EQERR;
				} else {
					ls_dwRetryCnt = 0;
/* modified 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- { ---------- */
//					ls_iPrc = PRC_INIT_CMD_RST;
/* modified 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ----------              */
					if (FALSE != m_bErrRep) {
						ls_iPrc = PRC_INIT_CMD_RST;
					} else {
						ls_iPrc = PRC_INIT_CMD_ORG;
					}
/* modified 2010.07.15 hmenjo PO ï¿½Iï¿½ï¿½ï¿½ï¿½ RST ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Î‰ï¿½ ---------- } ---------- */
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_STA;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_INIT_CMD_RST + 00:		/* ï¿½gï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½Oï¿½ï¿½ ILPO ï¿½Iï¿½ï¿½ ----------------------------- */
		ls_iPrc = PRC_INIT_CMD_RST + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);
		nexioCA_Interlock(TRUE);		/* CTAILPO ï¿½Iï¿½ï¿½	*/
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_INIT_CMD_RST + 10:		/* ï¿½gï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½Oï¿½ï¿½ ILPI ï¿½Iï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N --------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			ls_iPrc = PRC_INIT_CMD_RST + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_INIT_CMD_RST + 20:		/* ï¿½gï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½g ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_RST, "");
		ls_iPrc = PRC_INIT_CMD_RST + 30;
		break;
	case PRC_INIT_CMD_RST + 30:		/* ï¿½gï¿½ï¿½ï¿½uï¿½ï¿½ï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_RST == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_NORM);	/* (ï¿½ï¿½b)ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_INIT_CMD_RST + 40;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_RST + 20;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_INIT_CMD_RST + 40:		/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½Mï¿½Ò‚ï¿½ --------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_REP == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
					l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
				} else
				if (CTACTL_ERCD_RST == l_CtaDataInf.EvtREP.dwErrCode) {
					ls_iPrc = PRC_INIT_CMD_ORG;
					ls_dwRetryCnt = 0;
					this->KickCtrlSeq(l_iPrcRec + 1);
				} else {
					/* ï¿½Cï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½bï¿½Nï¿½Ù�ï¿½Å‚ï¿½ï¿½D	*/
					l_iRslt = CTACTL_ERR_ILPO;
					l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		}
		break;
	case PRC_INIT_CMD_ORG + 00:		/* ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Oï¿½ï¿½ ILPO ï¿½Iï¿½ï¿½ ------------------------------------- */
		ls_iPrc = PRC_INIT_CMD_ORG + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);
		nexioCA_Interlock(TRUE);		/* CTAILPO ï¿½Iï¿½ï¿½	*/
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_INIT_CMD_ORG + 10:		/* ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Oï¿½ï¿½ ILPI ï¿½Iï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ----------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			ls_iPrc = PRC_INIT_CMD_ORG + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_INIT_CMD_ORG + 20:		/* ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_ORG, "");
		ls_iPrc = PRC_INIT_CMD_ORG + 30;
		break;
	case PRC_INIT_CMD_ORG + 30:		/* ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_ORG == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_ORG);	/* (ï¿½ï¿½b)ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_INIT_EVT_CMO;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_ORG + 20;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_INIT_EVT_CMO + 00:		/* ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½Mï¿½Ò‚ï¿½ ----------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMO == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
					l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);	/* IL ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_INIT_ILPI_ON;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		}
		break;
	case PRC_INIT_ILPI_ON + 00:		/* CTAILPI ï¿½Iï¿½ï¿½ï¿½Ò‚ï¿½ ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO ï¿½Iï¿½t	*/
			this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO ï¿½Iï¿½t	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqInitCTA() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = PRC_INIT_CMD_STA;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */

/*
 *	ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Fï¿½ï¿½ï¿½İ’ï¿½
 */
int CCtaCtrl::SeqSetInf(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = 0;
	static DWORD ls_dwRetryCnt = 0;
	static COMMAND_CODE ls_enumCmdCode = CMD_MOD;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	char l_szSendData[PKT_DATA_LEN_MAX + 1];

	switch (ls_iPrc) {
	case 0:		/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½M	*/
		switch (ls_enumCmdCode) {
		case CMD_MOD:		/* ï¿½ï¿½ï¿½èƒ‚ï¿½[ï¿½h	*/
		default:
			switch (m_CtaMeasInf.iCtaMode) {
			case MEAS_CTA_MODE_1:
			case MEAS_CTA_MODE_2:
			case MEAS_CTA_MODE_3:
				break;
			default:
				m_CtaMeasInf.iCtaMode = MEAS_CTA_MODE_1;
				break;
			}
			l_szSendData[0] = (m_CtaMeasInf.iCtaMode & 0x0f) | 0x30;
			l_szSendData[1] = 0x00;
			break;
		case CMD_CID:		/* ï¿½Jï¿½Zï¿½bï¿½gï¿½ï¿½ï¿½Zï¿½bï¿½g	*/
			strcpy(l_szSendData, m_CtaMeasInf.szCstID);
			break;
		case CMD_PID:		/* ï¿½ï¿½Â�ï¿½ï¿½Zï¿½bï¿½g	*/
			strcpy(l_szSendData, m_CtaMeasInf.szSampleID);
			break;
		case CMD_SPT:		/* ï¿½ï¿½ÂŒï¿½ï¿½İ�ï¿½ï¿½Zï¿½bï¿½g	*/
			sprintf(l_szSendData, "%d", m_CtaMeasInf.dwThick);
			l_szSendData[3] = 0x00;
			if (0x00 == l_szSendData[1]) {
				l_szSendData[1] = ' ';
				l_szSendData[2] = ' ';
			}
			if (0x00 == l_szSendData[2]) {
				l_szSendData[2] = ' ';
			}
			break;
		}
		if (0 == ls_dwRetryCnt) {
			if (CMD_MOD == ls_enumCmdCode) {this->Logging(_T("Started SeqSetInf()"));}
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, ls_enumCmdCode, l_szSendData);
		ls_iPrc = 1;
		break;
	case 1:		/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½	*/
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (ls_enumCmdCode == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					ls_dwRetryCnt = 0;
					switch (ls_enumCmdCode) {
					case CMD_MOD:
					default:		ls_enumCmdCode = CMD_CID; ls_iPrc = 0;	break;
					case CMD_CID:	ls_enumCmdCode = CMD_PID; ls_iPrc = 0;	break;
					case CMD_PID:	ls_enumCmdCode = CMD_SPT; ls_iPrc = 0;	break;
					case CMD_SPT:	ls_enumCmdCode = CMD_MAX; ls_iPrc = 2;	break;
					}
					if (CMD_MAX == ls_enumCmdCode) {
						this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_NORM);	/* (ï¿½ï¿½b)ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					} else {
						this->KickCtrlSeq(211);
					}
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = 0;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(212);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case 2:		/* ï¿½İ’èŠ®ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½Mï¿½Ò‚ï¿½	*/
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CST == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				}
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqSetInf() [m_iSeqResult = %d, ls_iPrc = %d, ls_enumCmdCode = %d]"), m_iSeqResult, ls_iPrc, ls_enumCmdCode);
		this->Logging(l_strLog);
		ls_enumCmdCode = CMD_MOD;
		ls_iPrc = 0;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}

/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
/*
 *	ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Fï¿½ï¿½ï¿½ï¿½Jï¿½n
 */
#define	PRC_MEAS_CMD_MST	(PRC_MEAS_IDLE + 100)
#define	PRC_MEAS_EVT_CMP	(PRC_MEAS_IDLE + 200)
#define	PRC_MEAS_ILPI_ON	(PRC_MEAS_IDLE + 300)
int CCtaCtrl::SeqMeasStart(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = PRC_MEAS_CMD_MST;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_MEAS_CMD_MST + 00:		/* ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½Oï¿½ï¿½ ILPO ï¿½Iï¿½ï¿½ ------------------------------------- */
		this->Logging(_T("Started SeqMeasStart()"));
		ls_iPrc = PRC_MEAS_CMD_MST + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI);
		nexioCA_Interlock(TRUE);		/* CTAILPO ï¿½Iï¿½ï¿½	*/
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_MEAS_CMD_MST + 10:		/* ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½Oï¿½ï¿½ ILPI ï¿½Iï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ----------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			ls_iPrc = PRC_MEAS_CMD_MST + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_MEAS_CMD_MST + 20:		/* ï¿½ï¿½ï¿½ï¿½Jï¿½n --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
			m_bRecvedCMA = FALSE;
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MST, "");
		ls_iPrc = PRC_MEAS_CMD_MST + 30;
		break;
	case PRC_MEAS_CMD_MST + 30:		/* ï¿½ï¿½ï¿½ï¿½Jï¿½nï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_MST == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_T9_Meas);	/* (ï¿½ï¿½b)ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_MEAS_EVT_CMP;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEAS_CMD_MST + 20;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_MEAS_EVT_CMP + 00:		/* ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½èŠ®ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½Mï¿½Ò‚ï¿½ ------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMP == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½Pï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½èŠ®ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
					l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
				} else {
					/* ï¿½fï¿½[ï¿½^ï¿½Zï¿½bï¿½g ----------------------------------------*/
					CTAPOINTDATA l_CtaPointData;
					l_CtaPointData.iPointNo = l_CtaDataInf.EvtCMP.iPointNo;
					l_CtaPointData.dCAngle = ((double) l_CtaDataInf.EvtCMP.iCAngle) / 10.0;
					l_CtaPointData.dRadius = ((double) l_CtaDataInf.EvtCMP.iRadius) / 10.0;
					l_CtaPointData.dLiquid = ((double) l_CtaDataInf.EvtCMP.iLiquid) / 100.0;
					/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
/* modified 2009.11.16 hmenjo CTA ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Lï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
//					if (0 < _tcslen(m_CtaMeasInf.tszRecalibCA)) {
//						/* ï¿½Ú�Gï¿½pï¿½Ìƒï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½wï¿½ï¿½Lï¿½ï¿½	*/
//						if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibCA, l_CtaPointData.dCAngle)) {
//							l_iRslt = CTACTL_ERR_RECALIB;	/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Gï¿½ï¿½ï¿½[*/
//						}
//					}
//					if ((0 == l_iRslt)
//					 && (0 < _tcslen(m_CtaMeasInf.tszRecalibRad))) {
//						/* ï¿½ï¿½ï¿½aï¿½Ìƒï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½wï¿½ï¿½Lï¿½ï¿½	*/
//						if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibRad, l_CtaPointData.dRadius)) {
//							l_iRslt = CTACTL_ERR_RECALIB;	/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Gï¿½ï¿½ï¿½[*/
//						}
//					}
/* modified 2009.11.16 hmenjo CTA ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Lï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- 			 */
					/* ï¿½Ú�Gï¿½pï¿½Ìƒï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½wï¿½ï¿½Lï¿½ï¿½	*/
					if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibCA, l_CtaPointData.dCAngle)) {
						l_iRslt = CTACTL_ERR_RECALIB;	/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Gï¿½ï¿½ï¿½[*/
					} else
					/* ï¿½ï¿½ï¿½aï¿½Ìƒï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½wï¿½ï¿½Lï¿½ï¿½	*/
					if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibRad, l_CtaPointData.dRadius)) {
						l_iRslt = CTACTL_ERR_RECALIB;	/* ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Gï¿½ï¿½ï¿½[*/
					}
/* modified 2009.11.16 hmenjo CTA ï¿½ï¿½ï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½uï¿½Lï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
					if (0 != l_iRslt) {
						l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
					} else {
						/* ï¿½ï¿½ï¿½èŒ‹ï¿½ï¿½ DB ï¿½ÉƒZï¿½bï¿½g	*/
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->iPointNo = l_CtaPointData.iPointNo;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dCAngle = l_CtaPointData.dCAngle;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dRadius = l_CtaPointData.dRadius;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dLiquid = l_CtaPointData.dLiquid;

						this->SetTimeOut(TRUE, m_uiTimeout_ILPI);	/* IL ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
						ls_iPrc = PRC_MEAS_ILPI_ON;
						this->KickCtrlSeq(l_iPrcRec + 1);
					}
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		}
		break;
	case PRC_MEAS_ILPI_ON + 00:		/* CTAILPI ï¿½Iï¿½ï¿½ï¿½Ò‚ï¿½ ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO ï¿½Iï¿½t	*/
			this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO ï¿½Iï¿½t	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqMeasStart() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = PRC_MEAS_CMD_MST;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */

/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- { ---------- */
/*
 *	ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Fï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½
 */
#define	PRC_MEND_CMD_MPE	(PRC_MEND_IDLE + 100)
#define	PRC_MEND_EVT_CMO	(PRC_MEND_IDLE + 200)
#define	PRC_MEND_EVT_CMA	(PRC_MEND_IDLE + 300)
#define	PRC_MEND_CMD_MCE	(PRC_MEND_IDLE + 400)
#define	PRC_MEND_ILPI_ON	(PRC_MEND_IDLE + 500)
int CCtaCtrl::SeqMeasEnd(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = PRC_MEND_CMD_MPE;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_MEND_CMD_MPE + 00:		/* ï¿½ï¿½Â�Iï¿½ï¿½ï¿½Ê’mï¿½Oï¿½ï¿½ ILPO ï¿½Iï¿½ï¿½ --------------------------------- */
		this->Logging(_T("Started SeqMeasEnd()"));
		::Sleep(m_uiTimeout_MPE_Wait);
		ls_iPrc = PRC_MEND_CMD_MPE + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI);
		nexioCA_Interlock(TRUE);		/* CTAILPO ï¿½Iï¿½ï¿½	*/
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_MEND_CMD_MPE + 10:		/* ï¿½ï¿½Â�Iï¿½ï¿½ï¿½Ê’mï¿½Oï¿½ï¿½ ILPI ï¿½Iï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			ls_iPrc = PRC_MEND_CMD_MPE + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_MEND_CMD_MPE + 20:		/* ï¿½ï¿½Â�Iï¿½ï¿½ï¿½Ê’m ----------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
			m_bRecvedCMA = FALSE;
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MPE, "");
		ls_iPrc = PRC_MEND_CMD_MPE + 30;
		break;
	case PRC_MEND_CMD_MPE + 30:		/* ï¿½ï¿½Â�Iï¿½ï¿½ï¿½Ê’mï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ --------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_MPE == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_ORG);	/* (ï¿½ï¿½b)ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_MEND_EVT_CMO;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEND_CMD_MPE + 20;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_MEND_EVT_CMO + 00:		/* ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½Mï¿½Ò‚ï¿½ ----------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMO == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
					l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_T9_AllEnd);	/* (ï¿½ï¿½b)ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_MEND_EVT_CMA;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		}
		break;
	case PRC_MEND_EVT_CMA + 00:		/* ï¿½Sï¿½|ï¿½Cï¿½ï¿½ï¿½gï¿½ï¿½ï¿½èŠ®ï¿½ï¿½ï¿½Cï¿½xï¿½ï¿½ï¿½gï¿½ï¿½Mï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½Mï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½ ----- */
		if (0 != m_bRecvedCMA) {
			m_bRecvedCMA = FALSE;
			this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			ls_dwRetryCnt = 0;
			ls_iPrc = PRC_MEND_CMD_MCE;
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		}
		break;
	case PRC_MEND_CMD_MCE + 00:		/* ï¿½Jï¿½Zï¿½bï¿½gï¿½Iï¿½ï¿½ï¿½Ê’m ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MCE, m_CtaMeasInf.szCstID);
		ls_iPrc = PRC_MEND_CMD_MCE + 10;
		break;
	case PRC_MEND_CMD_MCE + 10:		/* ï¿½Jï¿½Zï¿½bï¿½gï¿½Iï¿½ï¿½ï¿½Ê’mï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½ ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_MCE == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_ILPI);	/* IL ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
					ls_iPrc = PRC_MEND_ILPI_ON;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEND_CMD_MCE;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	case PRC_MEND_ILPI_ON + 00:		/* CTAILPI ï¿½Iï¿½ï¿½ï¿½Ò‚ï¿½ ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO ï¿½Iï¿½t	*/
			this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
			l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO ï¿½Iï¿½t	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqMeasEnd() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = PRC_MEND_CMD_MPE;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}
/* added 2010.09.09 hmenjo CTA PO/PI ï¿½nï¿½ï¿½ï¿½hï¿½Vï¿½Fï¿½[ï¿½N ---------- } ---------- */
/*
 *	ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Fï¿½ï¿½Ô�æ“¾
 */
int CCtaCtrl::SeqGetStat(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = 0;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;

	switch (ls_iPrc) {
	case 0:		/* ï¿½ï¿½Ô—vï¿½ï¿½	*/
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqGetStat()"));
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½Zï¿½bï¿½g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Jï¿½n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_STA, "");
		ls_iPrc = 1;
		break;
	case 1:		/* ï¿½ï¿½Ô—vï¿½ï¿½ï¿½Ìƒï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½Mï¿½Ò‚ï¿½	*/
		if (0 != pCtaRcvData->bRecved) {
			/* ï¿½ï¿½Mï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½eï¿½É‚ï¿½è‹­ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½Ì•Kï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚±ï¿½Æ�Eï¿½Eï¿½E	*/
				}
			} else if (CMD_STA == *pCtaRcvData->pdwCmdCode) {
				/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½|ï¿½ï¿½ï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½ï¿½ï¿½ï¿½	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* ï¿½tï¿½Hï¿½[ï¿½}ï¿½bï¿½gï¿½nï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
				} else {
					m_lCtaStatus =	MAKELONG(	/* ï¿½Xï¿½eï¿½[ï¿½^ï¿½X	*/
											MAKEWORD(
													l_CtaDataInf.CmdSTA.dwZState,	/* LL	*/
													l_CtaDataInf.CmdSTA.dwStatus1	/* LH	*/
												),
											MAKEWORD(
													l_CtaDataInf.CmdSTA.dwStatus2,	/* HL	*/
													0	/* HH	*/
												)
										);
					ls_dwRetryCnt = 0;
					l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = 0;	/* ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(511);
			} else {
				l_bSeqEnd = TRUE;	/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* ï¿½Vï¿½[ï¿½Pï¿½ï¿½ï¿½Xï¿½Iï¿½ï¿½	*/
		m_iSeqResult = l_iRslt;
		CString l_strLog;
		l_strLog.Format(_T("Ended   SeqGetStat() [m_iSeqResult = %d, ls_iPrc = %d]"), m_iSeqResult, ls_iPrc);
		this->Logging(l_strLog);
		ls_iPrc = 0;
		ls_dwRetryCnt = 0;

		m_dwSeqPattern = SEQPTN_NONE;
		::SetEvent(m_hEvWaitEndSeq);
	}

	return l_iRslt;
}

/*
 *	ï¿½ï¿½Ê‚Ö‚ÌƒAï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ê’m
 */
BOOL CCtaCtrl::NotifyAlarm(int iAlarmCode)
{
	if (0 == iAlarmCode) {
		return FALSE;	/* ï¿½ï¿½ï¿½Ê’m	*/
	}

	int l_iNotifyCode = 0;
	int l_iLevel = 0;		/*	0:ï¿½xï¿½ï¿½
								1:
								*/

	switch (iAlarmCode) {
	/* CTASIO ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½`	*/
	default:						l_iNotifyCode = 100; l_iLevel = 0;	break;	/* ï¿½ï¿½ï¿½ï¿½`ï¿½Gï¿½ï¿½ï¿½[	*/
	case CTASIO_ERR_UNINIT:			l_iNotifyCode = 101; l_iLevel = 0;	break;
	case CTASIO_ERR_RCVBUFFULL:		l_iNotifyCode = 102; l_iLevel = 0;	break;
	case CTASIO_ERR_SEND_FAIL:		l_iNotifyCode = 103; l_iLevel = 0;	break;
	case CTASIO_ERR_PARAM:			l_iNotifyCode = 104; l_iLevel = 0;	break;
	case CTASIO_ERR_NORCVDATA:		l_iNotifyCode = 105; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALID_PKT:	l_iNotifyCode = 106; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALID_CMD:	l_iNotifyCode = 107; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALID_EVT:	l_iNotifyCode = 108; l_iLevel = 0;	break;
	case CTASIO_ERR_NOLF:			l_iNotifyCode = 109; l_iLevel = 0;	break;
	case CTASIO_ERR_NOCR:			l_iNotifyCode = 110; l_iLevel = 0;	break;
	case CTASIO_ERR_SUM:			l_iNotifyCode = 111; l_iLevel = 0;	break;
	case CTASIO_ERR_TOOSHORT:		l_iNotifyCode = 112; l_iLevel = 0;	break;
	case CTASIO_ERR_CTRLCODE:		l_iNotifyCode = 113; l_iLevel = 0;	break;
	case CTASIO_ERR_INVALIDLEN:		l_iNotifyCode = 114; l_iLevel = 0;	break;
	case CTASIO_ERR_RCVFAIL:		l_iNotifyCode = 115; l_iLevel = 0;	break;
	/* CTACTL ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½`	*/
	case CTACTL_ERR_DATANONE:		l_iNotifyCode = 120; l_iLevel = 0;	break;
	case CTACTL_ERR_DATAFORMAT:		l_iNotifyCode = 121; l_iLevel = 0;	break;
	case CTACTL_ERR_PKTCMD:			l_iNotifyCode = 122; l_iLevel = 0;	break;
	case CTACTL_ERR_PARAMERR:		l_iNotifyCode = 123; l_iLevel = 0;	break;
	case CTACTL_ERR_EQBUSY:			l_iNotifyCode = 124; l_iLevel = 0;	break;
	case CTACTL_ERR_EQERR:			l_iNotifyCode = 125; l_iLevel = 0;	break;
	case CTACTL_ERR_TIMEOUT:		l_iNotifyCode = 126; l_iLevel = 0;	break;
	case CTACTL_ERR_ILPO:			l_iNotifyCode = 127; l_iLevel = 0;	break;
	case CTACTL_ERR_ILPI:			l_iNotifyCode = 128; l_iLevel = 0;	break;
	case CTACTL_ERR_FORCEEND:		l_iNotifyCode = 129; l_iLevel = 0;	break;
	case CTACTL_ERR_RECALIB:		l_iNotifyCode = 130; l_iLevel = 0;	break;
	/* CTA ï¿½ï¿½ï¿½jï¿½bï¿½g ï¿½Gï¿½ï¿½ï¿½[ï¿½Rï¿½[ï¿½h	*/
/*	case CTACTL_ERCD_RST:			l_iNotifyCode = 140; l_iLevel = 0;	break;	ï¿½ï¿½ï¿½gï¿½p	*/
	case CTACTL_ERCD_HEAD:			l_iNotifyCode = 141; l_iLevel = 0;	break;
	case CTACTL_ERCD_CMD:			l_iNotifyCode = 142; l_iLevel = 0;	break;
	case CTACTL_ERCD_SUM:			l_iNotifyCode = 143; l_iLevel = 0;	break;
	case CTACTL_ERCD_LEN:			l_iNotifyCode = 144; l_iLevel = 0;	break;
	case CTACTL_ERCD_IL:			l_iNotifyCode = 145; l_iLevel = 0;	break;
	case CTACTL_ERCD_CTRLR:			l_iNotifyCode = 146; l_iLevel = 1;	break;
	case CTACTL_ERCD_NO_MEAS:		l_iNotifyCode = 147; l_iLevel = 1;	break;
	case CTACTL_ERCD_MEASING:		l_iNotifyCode = 148; l_iLevel = 1;	break;
	case CTACTL_ERCD_NO_OPE:		l_iNotifyCode = 149; l_iLevel = 1;	break;
	case CTACTL_ERCD_UNSAVE:		l_iNotifyCode = 150; l_iLevel = 1;	break;
	case CTACTL_ERCD_MODE:			l_iNotifyCode = 151; l_iLevel = 1;	break;
	case CTACTL_ERCD_DUP_ID:		l_iNotifyCode = 152; l_iLevel = 1;	break;
	case CTACTL_ERCD_SET_VALUE:		l_iNotifyCode = 153; l_iLevel = 1;	break;
	case CTACTL_ERCD_PWR:			l_iNotifyCode = 154; l_iLevel = 1;	break;
	case CTACTL_ERCD_Z_CTRLR_COM:	l_iNotifyCode = 155; l_iLevel = 1;	break;
	case CTACTL_ERCD_GENDROP:		l_iNotifyCode = 156; l_iLevel = 1;	break;
	case CTACTL_ERCD_HOST_COM:		l_iNotifyCode = 157; l_iLevel = 1;	break;
	case CTACTL_ERCD_MOVE_TIMEOUT:	l_iNotifyCode = 158; l_iLevel = 1;	break;
	case CTACTL_ERCD_ORG:			l_iNotifyCode = 159; l_iLevel = 1;	break;
	case CTACTL_ERCD_Z_CTRLR:		l_iNotifyCode = 160; l_iLevel = 1;	break;
	case CTACTL_ERCD_PLC_COM:		l_iNotifyCode = 161; l_iLevel = 1;	break;
	}

	BOOL l_iRet = ::PostMessage(g_hNotifyWnd, WM_MEAS_CTA_ALARM, MAKEWPARAM(l_iNotifyCode, l_iLevel), 0);

	return l_iRet;
}
