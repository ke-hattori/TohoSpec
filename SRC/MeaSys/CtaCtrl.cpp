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
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	�}�N����`
 */
#define	SECTION_CTA	_T("CTA")	/* NanoSpec.ini ���̃Z�N�V����	*/
/*���g�p*///#define	INTVL_TIME	50			/* ������^�C�}[ms]	*/
#define	SEQWAIT_TIMEOUT			(60 * 1000 * 10)	/* �V�[�P���X�����҂��^�C���A�E�g[ms](10min)	*/
#define	CTA_TIMEOUT_RES			(5000)				/* �^�C���A�E�g[ms] ���X�|���X��M�҂�	*/
#define	CTA_TIMEOUT_T9_NORM		(5000)				/* �^�C���A�E�g[ms] ��b�^�C���A�E�g(�C�x���g�҂�)	*/
#define	CTA_TIMEOUT_T9_ORG		(10000)				/* �^�C���A�E�g[ms] ��b�^�C���A�E�g(���_���A�҂�)	*/
#define	CTA_TIMEOUT_T9_MEAS		(60000)				/* �^�C���A�E�g[ms] ��b�^�C���A�E�g(���蒆)	*/
#define	CTA_TIMEOUT_T9_ALLEND	(60000)				/* �^�C���A�E�g[ms] ��b�^�C���A�E�g(�S�|�C���g�I����M�҂�)	*/
/* modified 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
//#define	CTA_TIMEOUT_ILPI		(1000)				/* �^�C���A�E�g[ms] PI �C���^���b�N�M���^�C���A�E�g	*/
//#define	CTA_TIMEOUT_ILPO		(3000)				/* �^�C���A�E�g[ms] PO �C���^���b�N�M���^�C���A�E�g(Sleep())	*/
/* modified 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ----------              */
#define	CTA_TIMEOUT_ILPI		(5000)				/* �^�C���A�E�g[ms] PI �C���^���b�N�M���^�C���A�E�g	*/
#define	CTA_TIMEOUT_ILPO		(5000)				/* �^�C���A�E�g[ms] PO �C���^���b�N�M���^�C���A�E�g(Sleep())	*/
/* modified 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */
#define	CTA_TIMEOUT_RETRY		(200)				/* �^�C���A�E�g[ms] ���g���C�^�C�}(Sleep())	*/
#define	CTA_TIMEOUT_MPE_WAIT	(2500)				/* �^�C���A�E�g[ms] MPE ���M�҂��E�F�C�g(Sleep())	*/
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
/* ����v���Z�X�ԍ���`	*/
#define	PRC_INIT_IDLE	1000
#define	PRC_MEAS_IDLE	3000
#define	PRC_MEND_IDLE	4000
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
#define	CTA_ILPO_DELAY	(1500)						/* �^�C���A�E�g[ms] PO CTA �F���f�B���C(Sleep())	*/
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */


/*
 *	�\���̒�`
 */


/*
 *	���[�J���O���[�o����`
 */
static CCtaCtrl* lgs_pclsCCtaCtrl;	/* �����̃N���X�|�C���^	*/
static UINT_PTR lgs_uiTimerID = 0;	/* �^�C�} ID	*/
/* �E�B���h�E���b�Z�[�W	*/
UINT WM_CTACTL_TIMER = ::RegisterWindowMessage("WM_CTACTL_TIMER");	/* �^�C�}	*/
UINT WM_CTACTL_SEQKICK = ::RegisterWindowMessage("WM_CTACTL_SEQKICK");	/* �V�[�P���X�L�b�N	*/


/*
 *	Extern ��`
 */
extern CLogFile* pLogFile;	/* from MEASYS.cpp	*/
extern TCHAR g_tszProcDir[_MAX_PATH];	/* from MEASYS.cpp	*/
extern HWND g_hNotifyWnd;	/* from MEASYS.cpp	*/
extern CSharedMemory<CTARESULT> lg_smCtaResultDataBase;	/* from MEASYS.cpp	*/
extern BOOL CtaRecalib(LPCTSTR szRecalib, double& dData);
extern BOOL bHwSimulation;	/* from MEASYS.cpp	*/
/* �E�B���h�E���b�Z�[�W	*/
extern UINT WM_CTASIO_RECVED;		/* �f�[�^��M				from CtaSio.cpp	*/
extern UINT WM_CTASIO_RCVBUFFULL;	/* ��M�o�b�t�@�t��			from CtaSio.cpp	*/
extern UINT WM_CTASIO_RCVFAIL;		/* ��M���s(�X���b�h���[�v)	from CtaSio.cpp	*/


/////////////////////////////////////////////////////////////////////////////
// CCtaCtrl

IMPLEMENT_DYNCREATE(CCtaCtrl, CWinThread)

CCtaCtrl::CCtaCtrl(DWORD dwCreateFlags/* = 0*/)
{
	TRACE(_T("CCtaCtrl::CCtaCtrl() \n"));

	this->Logging(_T("constructor."));

	/* �����ݒ�	*/
	m_bInitedInstance = FALSE;
	this->m_bAutoDelete = FALSE;	/* �I�u�W�F�N�g�̎����j�����֎~	*/
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
/* added 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- { ---------- */
	m_bErrRep = FALSE;
/* added 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- } ---------- */
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
	m_uiTimeout_ILPI = CTA_TIMEOUT_ILPI;
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
	m_uiDelay_ILPO = CTA_ILPO_DELAY;
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */

	/* �N���e�B�J���Z�N�V�������`	*/
	::InitializeCriticalSection(&m_csCtrlBusy);
	::EnterCriticalSection(&m_csCtrlBusy);

	/* SIO �X���b�h���N�����܂��D	*/
	this->Logging(_T("Starting CCtaSio..."));
	m_pclsCCtaSio = 0;
	m_pclsCCtaSio = new CCtaSio(CREATE_SUSPENDED);
	if (0 == m_pclsCCtaSio) {
		/* SIO �X���b�h�\�z���s	*/
		this->Logging(_T("Failed to create CCtaSio. (new)"));
	} else
	if (0 == m_pclsCCtaSio->m_hThread) {
		/* SIO �X���b�h�N�����s	*/
		this->Logging(_T("Failed to start CCtaSio thread. (thread)"));
	} else {
		/* SIO �X���b�h�N������	*/
		this->Logging(_T("Started CCtaSio."));
		if (TRUE == this->InitializeSIO()) {

			/* �X���b�h�̋N���҂�	*/
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

	/* SIO �X���b�h���I��	*/
	if (0 != m_pclsCCtaSio) {
		this->Logging(_T("Deleting CCtaSio..."));
		delete m_pclsCCtaSio;
		m_pclsCCtaSio = 0;
		this->Logging(_T("Deleted CCtaSio."));
	}

	/* �X���b�h�̏��ő҂�	*/
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

	/* �N���e�B�J���Z�N�V�������J��	*/
	::EnterCriticalSection(&m_csCtrlBusy);	/* �擾���ꂽ�܂܂����m��Ȃ��̂ő҂�	*/
	::LeaveCriticalSection(&m_csCtrlBusy);	/* �J������								*/
	::DeleteCriticalSection(&m_csCtrlBusy);
}

BOOL CCtaCtrl::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("CCtaCtrl::InitInstance() \n"));

	this->Logging(_T("InitInstance()."));

//	m_bInitedInstance = TRUE;

	((CCtaSio*) m_pclsCCtaSio)->ResumeSIO(this->m_nThreadID);

//	/* �R���X�g���N�^�̑҂����������܂�	*/
//	/*		�܂�C�X���b�h�̐����ƃX���b�h�N���X�̍\�z�̓���������Ă��܂��D	*/
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
		/* �R���X�g���N�^�̑҂����������܂�	*/
		/*		�܂�C�X���b�h�̐����ƃX���b�h�N���X�̍\�z�̓���������Ă��܂��D	*/
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
 *	����V�[�P���X�N�� ���b�Z�[�W�n���h��
 */
void CCtaCtrl::OnCtaCtrlSeqKick(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTACTL_SEQKICK"), wparam, lparam);
}

/*
 *	�^�C�} ���b�Z�[�W�n���h��
 */
void CCtaCtrl::OnCtaCtrlTimer(WPARAM wparam, LPARAM lparam)
{
#if 1	/* ������^�C�}�̏ꍇ�̓��O���Ȃ��ł��������D	*/
	this->Logging_WinMsg(_T("WM_CTACTL_TIMER"), wparam, lparam);
#endif
	m_bTimeOut = TRUE;
}

/*
 *	�f�[�^��M ���b�Z�[�W�n���h��
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
 *	��M�o�b�t�@�t�� ���b�Z�[�W�n���h��
 */
void CCtaCtrl::OnCtaSioRcvBufFull(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RCVBUFFULL"), wparam, lparam);

	/* ��ʂɒʒm	*/
	this->NotifyAlarm(CTASIO_ERR_RCVBUFFULL);
}

/*
 *	��M���s(CCtaSio �X���b�h���[�v) ���b�Z�[�W�n���h��
 */
void CCtaCtrl::OnCtaSioRcvFail(WPARAM wparam, LPARAM lparam)
{
	this->Logging_WinMsg(_T("WM_CTASIO_RCVFAIL"), wparam, lparam);

	/* ��ʂɒʒm	*/
	this->NotifyAlarm(CTASIO_ERR_RCVFAIL);
}

/****************************************************************************
	����p�֐�
****************************************************************************/

/*
 *	CTA ����������
 */
BOOL CCtaCtrl::Cta_Initialize()
{
	this->Logging(_T("Started InitializeCTA()"));

	BOOL l_bRet = TRUE;

	/* ���̊֐��̂݃R�R�ōs���܂��D	*/
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
 *	CTA ���胂�[�h�ݒ�
 *		MEAS_CTA_MODE_1	���[�h�P�F��P�ʁE�I�y���[�^�f�[�^�m�F�Ȃ�
 *		MEAS_CTA_MODE_2	���[�h�Q�F��P�ʁE�I�y���[�^�f�[�^�m�F����
 *		MEAS_CTA_MODE_3	���[�h�R�F�J�Z�b�g�P�ʁE�I�y���[�^�f�[�^�m�F����
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
 *	CTA �J�Z�b�g ID�C��� ID �ݒ�
 */
BOOL CCtaCtrl::Cta_SetCstSample(
		LPCSTR pstrCstID,						/* �J�Z�b�g ID	�FASCII 80 ����	*/
		LPCSTR pstrSampleID,					/* ��� ID		�FASCII 66 ����	*/	// 2010.03.04 K.Matsuo 80���� - 14����(yyyymmddhhmmss)�ł�
		const MAIN_RCP_INFO* pMainRcpInfo		/* ���C�����V�s	*/
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
// 2010.03.04 K.Matsuo FAMAS ID�d���G���[�΍� -->
	char l_szSystemTime[255 + 1];
	SYSTEMTIME l_systemTime;
// 2010.03.04 K.Matsuo FAMAS ID�d���G���[�΍� <--

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (-1 == m_CtaMeasInf.iCtaMode) {
		l_bRet = FALSE;	/* ���胂�[�h�����ݒ�ł��D	*/
	} else if (0 == strlen(l_szCstID)) {
		l_bRet = FALSE;	/* �J�Z�b�g ID �� null �ł��D	*/
	} else if (-1 != this->IsStrAlNum(l_szCstID)) {
		l_bRet = FALSE;	/* �J�Z�b�g ID �ɖ����ȕ���������܂��D	*/
	} else if (0 == strlen(l_szSampleID)) {
		l_bRet = FALSE;	/* ��� ID �� null �ł��D	*/
	} else if (-1 != this->IsStrAlNum(l_szSampleID)) {
		l_bRet = FALSE;	/* ��� ID �ɖ����ȕ���������܂��D	*/
	} else if (0 == pMainRcpInfo) {
		l_bRet = FALSE;	/* ���C�����V�s�w�肪 null�ł��D	*/
	} else if ((RECIPE_NAME_LEN < _tcslen(pMainRcpInfo->MainRcpParam._CA.szRecalib[0]))
			|| (RECIPE_NAME_LEN < _tcslen(pMainRcpInfo->MainRcpParam._CA.szRecalib[1]))) {
		l_bRet = FALSE;	/* ���L�����u PGM �����������܂��D	*/
	} else {
		::EnterCriticalSection(&m_csCtrlBusy);

		/* �J�Z�b�g ID �� 80 �����ɑ���Ȃ������� 0x20 �Ŗ��߂܂��D	*/
		memset(m_CtaMeasInf.szCstID, 0x20, sizeof(m_CtaMeasInf.szCstID));
		m_CtaMeasInf.szCstID[80] = 0x00;
		memcpy(m_CtaMeasInf.szCstID, l_szCstID, strlen(l_szCstID));

// 2010.03.04 K.Matsuo FAMAS ID�d���G���[�΍� -->
// ���ID�ɁA������t�����邱�ƂŁAID���d�����Ȃ��悤�ɂ���
		GetLocalTime(&l_systemTime);
		sprintf(l_szSystemTime, "%04d%02d%02d%02d%02d%02d",
				l_systemTime.wYear,
				l_systemTime.wMonth,
				l_systemTime.wDay,
				l_systemTime.wHour,
				l_systemTime.wMinute,
				l_systemTime.wSecond);
		strcat(l_szSampleID, l_szSystemTime);
// 2010.03.04 K.Matsuo FAMAS ID�d���G���[�΍� <--
		/* ��� ID �� 80 �����ɑ���Ȃ������� 0x20 �Ŗ��߂܂��D	*/
		memset(m_CtaMeasInf.szSampleID, 0x20, sizeof(m_CtaMeasInf.szSampleID));
		m_CtaMeasInf.szSampleID[80] = 0x00;
		memcpy(m_CtaMeasInf.szSampleID, l_szSampleID, strlen(l_szSampleID));
/* modified 2009.11.16 hmenjo CTA ���L�����u�L���`�F�b�N ---------- { ---------- */
//		/* ���L�����u PGM �����R�s�[���Ă����܂��D	*/
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
/* modified 2009.11.16 hmenjo CTA ���L�����u�L���`�F�b�N ---------- 			 */
		/* ���L�����u PGM �����R�s�[���Ă����܂��D	*/
		_tcscpy(m_CtaMeasInf.tszRecalibCA, pMainRcpInfo->MainRcpParam._CA.szRecalib[0]);
		_tcscpy(m_CtaMeasInf.tszRecalibRad, pMainRcpInfo->MainRcpParam._CA.szRecalib[1]);
/* modified 2009.11.16 hmenjo CTA ���L�����u�L���`�F�b�N ---------- } ---------- */

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_SetCstSample()"));

	return l_bRet;
}

/*
 *	CTA ����ݐݒ�
 */
BOOL CCtaCtrl::Cta_SetThick(
		DWORD dwThick			/* ����ݏ��[0.1mm]�C�͈�(0.0�`10.0mm)	*/
	)
{
	this->Logging(_T("Started Cta_SetThick()"));

	BOOL l_bRet = TRUE;

	if (TRUE != m_bInitedInstance) {
		l_bRet = FALSE;
	} else if (-1 == m_CtaMeasInf.iCtaMode) {
		l_bRet = FALSE;	/* ���胂�[�h�����ݒ�ł��D	*/
	} else if (100 < dwThick) {
		l_bRet = FALSE;	/* ����ݏ�񂪔͈͊O�ł��D	*/
	} else if (0 == strlen(m_CtaMeasInf.szCstID)) {
		l_bRet = FALSE;	/* �J�Z�b�g ID �� null �ł��D	*/
	} else if (0 == strlen(m_CtaMeasInf.szSampleID)) {
		l_bRet = FALSE;	/* ��� ID �� null �ł��D	*/
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
 *	CTA ����J�n
 */
BOOL CCtaCtrl::Cta_Measure(
		int iScanCnt,							/* ����ԍ�	*/
		const STAGE_COORD_XYZ* pStageCoordXyz	/* ����ʒu���W	*/
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
			/* ����I����ʒm���܂��D	*/
			::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
			/* �R�R�Ƀ��L�����u���������Ă��悢�ł��D	*/
		}

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_Measure()"));

	return l_bRet;
}

/*
 *	CTA ����I��
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
 *	CTA �����I��
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
 *	CTA ��Ԏ擾
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

		*plStatus = m_lCtaStatus;	/*	LOBYTE(LOWORD(m_lCtaStatus));	�w�b�h�FZ ���ʒu
										HIBYTE(LOWORD(m_lCtaStatus));	��ԂP�F������
										LOBYTE(HIWORD(m_lCtaStatus));	��ԂQ�F���u���
										HIBYTE(HIWORD(m_lCtaStatus));	(���g�p)
										*/

		::LeaveCriticalSection(&m_csCtrlBusy);
	}

	this->Logging(_T("Ended   Cta_GetStatus()"));

	return l_bRet;
}

/****************************************************************************
	���̑��֐�
****************************************************************************/

/*
 *	SIO ����������
 */
BOOL CCtaCtrl::InitializeSIO()
{
	TCHAR l_tszNanoSpecIni[_MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
//	_stprintf(l_tszNanoSpecIni, _T("%s") _T(CFG_DIR) _T(NANOSPEC_INIFILENAME), g_tszProcDir);

	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if(g_lAppNameType != APP_NAME_NANO){
	strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
}
_stprintf(l_tszNanoSpecIni, _T("%s%s%s"), _T(CFG_DIR), g_tszProcDir, (LPCTSTR)strFilename);
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--

	TCHAR l_tszText[255];
	/* ���g���C�J�E���g�Ǐo��	*/
	::GetPrivateProfileString(SECTION_CTA, _T("RETRY"), _T("0"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	m_dwRetryCnt = _ttoi(l_tszText);
	if (m_dwRetryCnt <= 0) {
		m_dwRetryCnt = 0;
	}
	/* HWS �Ǐo��	*/
	::GetPrivateProfileString(SECTION_CTA, _T("HWS"), _T("1"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	int l_iHWS1 = _ttoi(l_tszText);
	::GetPrivateProfileString(SECTION_CTA, _T("HWS"), _T("2"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
	int l_iHWS2 = _ttoi(l_tszText);
	if ((1 == l_iHWS1) && (2 == l_iHWS2)) {
		/* �L�[�����������D	*/
		m_bHWS = (0 != bHwSimulation)? TRUE : FALSE;
	} else {
		m_bHWS = (0 != l_iHWS2)? TRUE : FALSE;
	}
	/* ��b�^�C���A�E�g(���蒆)	*/
	m_uiTimeout_T9_Meas = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_T9_MEAS"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_T9_Meas < 500) || (CTA_TIMEOUT_T9_MEAS < m_uiTimeout_T9_Meas)) {
		m_uiTimeout_T9_Meas = CTA_TIMEOUT_T9_MEAS;
	}
	/* ��b�^�C���A�E�g(�S�|�C���g�I����M�҂�)	*/
	m_uiTimeout_T9_AllEnd = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_T9_ALLEND"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_T9_AllEnd < 500) || (CTA_TIMEOUT_T9_ALLEND < m_uiTimeout_T9_AllEnd)) {
		m_uiTimeout_T9_AllEnd = CTA_TIMEOUT_T9_ALLEND;
	}
	/* PO �C���^���b�N�M���^�C���A�E�g	*/
	m_uiTimeout_ILPO = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_ILPO"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_ILPO < 10) || (CTA_TIMEOUT_ILPO < m_uiTimeout_ILPO)) {
		m_uiTimeout_ILPO = CTA_TIMEOUT_ILPO;
	}
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
	/* PI �C���^���b�N�M���^�C���A�E�g	*/
	m_uiTimeout_ILPI = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_ILPI"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_ILPI < 10) || (CTA_TIMEOUT_ILPI < m_uiTimeout_ILPI)) {
		m_uiTimeout_ILPI = CTA_TIMEOUT_ILPI;
	}
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */
	/* ���g���C�^�C�}	*/
	m_uiTimeout_Retry = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_RETRY"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_Retry < 200) || (5000 < m_uiTimeout_Retry)) {
		m_uiTimeout_Retry = CTA_TIMEOUT_RETRY;
	}
	/* MPE ���M�҂��E�F�C�g	*/
	m_uiTimeout_MPE_Wait = ::GetPrivateProfileInt(SECTION_CTA, _T("TIMEOUT_MPE_WAIT"), -1, l_tszNanoSpecIni);
	if ((m_uiTimeout_MPE_Wait < 10) || (5000 < m_uiTimeout_MPE_Wait)) {
		m_uiTimeout_MPE_Wait = CTA_TIMEOUT_MPE_WAIT;
	}
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
	/* PO CTA �F���f�B���C	*/
	m_uiDelay_ILPO = ::GetPrivateProfileInt(SECTION_CTA, _T("ILPO_DELAY"), -1, l_tszNanoSpecIni);
	if ((m_uiDelay_ILPO < 10) || (10000 < m_uiDelay_ILPO)) {
		m_uiDelay_ILPO = CTA_ILPO_DELAY;
	}
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */

// 2009.11.18 K.Matsuo -->
//	/* �|�[�g�ԍ��Ǐo��	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("COM"), _T("1"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iComNo = _ttoi(l_tszText);
//	if (l_iComNo <= 0) {
//		l_iComNo = 1;
//	}
//	/* �{�[���[�g�Ǐo��	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("BaudRate"), _T("9600"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iBaudRate = _ttoi(l_tszText);
//	/* �f�[�^���Ǐo��	*/
//	::GetPrivateProfileString(SECTION_CTA, _T("ByteSize"), _T("8"), l_tszText, sizeof(l_tszText), l_tszNanoSpecIni);
//	int l_iByteSize = _ttoi(l_tszText);
//	/* �p���e�B�Ǐo��	*/
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
//	/* �X�g�b�v�r�b�g�Ǐo��	*/
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
 *	���O
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
 *	���O - ���b�Z�[�W��M�p
 */
void CCtaCtrl::Logging_WinMsg(LPCTSTR strLog, WPARAM wParam, LPARAM lParam)
{
	CString l_strLogText;
	l_strLogText.Format(_T("Received %s (WPARAM = 0x%08x, LPARAM = 0x%08x)."), strLog, wParam, lParam);
	this->Logging(l_strLogText);
}

/*
 *	����V�[�P���X
 */
int CCtaCtrl::CtrlSeq()
{
	int l_iRet = 0;

	/* CTA ���j�b�g����̎�M�m�F ------------------------------------------*/
	PACKET_CODE l_enumPacketCode = PKT_MAX;
	COMMAND_CODE l_enumCmdCode = EVT_MAX;
	char l_szRcvData[PKT_DATA_LEN_MAX + 1];
	CTARCVDATA l_CtaRcvData;
	memset(&l_CtaRcvData, 0, sizeof(CTARCVDATA));
	if (0 != m_bRcvData) {
		/* ��M�f�[�^�L��	*/
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
			/* ��M�f�[�^�͂���܂���ł����D	*/
		} else if (0 != l_iRslt) {
			/* ��M�f�[�^�G���[�ł��D	*/
			if (PKT_EVT == *l_CtaRcvData.pdwPktCode) {
				this->SendErrRes(l_iRslt);	/* ���X�|���X��Ԃ��܂��D	*/
			}
			/* ��ʂɒʒm	*/
			this->NotifyAlarm(l_iRslt);
		} else {
			/*	�E�C�x���g�p�P�b�g��M
				�E���X�|���X�p�P�b�g��M
				�E�G���[���X�|���X�p�P�b�g��M	*/
			/* �e�V�[�P���X�Ɏ�M�ʒm	*/
			l_CtaRcvData.bRecved = TRUE;
			/* �C�x���g�p�P�b�g�̏ꍇ�̓R�R�Ń��X�|���X��Ԃ��܂��D*/
			if (PKT_EVT == l_enumPacketCode) {
				this->SendEvtRes(&l_CtaRcvData);
				/* �G���[�����C�x���g�̏ꍇ�́CChief �ւ̒ʒm���K�v�ł��D
					�������C���Z�b�g�R�[�h(000)�Ə��������̏ꍇ�͒ʒm���܂���D	*/
				if (EVT_REP == l_enumCmdCode) {
					if ((0 != l_CtaRcvData.dwErrCode) && (SEQPTN_INIT != m_dwSeqPattern)) {
						/* ��ʂɒʒm	*/
						this->NotifyAlarm(l_CtaRcvData.dwErrCode);
					}
				}
			}
		}
	}

	/* �e�V�[�P���X --------------------------------------------------------*/
	switch (m_dwSeqPattern) {
	case SEQPTN_NONE:		/* �V�[�P���X����	*/
		break;
	case SEQPTN_INIT:		/* ������	*/
		l_iRet = this->SeqInitCTA(&l_CtaRcvData);
		break;
	case SEQPTN_SETINF:		/* ���ݒ�	*/
		l_iRet = this->SeqSetInf(&l_CtaRcvData);
		break;
	case SEQPTN_MEASSTA:	/* ����J�n	*/
		l_iRet = this->SeqMeasStart(&l_CtaRcvData);
		break;
	case SEQPTN_MEASEND:	/* ����I��	*/
		l_iRet = this->SeqMeasEnd(&l_CtaRcvData);
		break;
	case SEQPTN_GETSTAT:	/* ��Ԏ擾	*/
		l_iRet = this->SeqGetStat(&l_CtaRcvData);
		break;
	default:
		ASSERT(0);
		m_dwSeqPattern = SEQPTN_NONE;
		break;
	}

	/* ���̑� --------------------------------------------------------------*/
	/* �R�R�܂łɎ�M�ʒm����������Ȃ������ꍇ�́C
		�s�v�Ɣ��f���Ď̂Ă܂��D	*/
	if (TRUE == l_CtaRcvData.bRecved) {
		l_CtaRcvData.bRecved = FALSE;
	}

	/* ��M�f�[�^���܂�����ꍇ�͐���V�[�P���X���L�b�N���܂��D	*/
	if (0 == ((CCtaSio*) m_pclsCCtaSio)->IsExistRcvData()) {
		/* ��M�f�[�^���������D	*/
		m_bRcvData = TRUE;
		this->KickCtrlSeq(1);
	}

	return l_iRet;
}

/*
 *	�V�[�P���X�̋N���w�߂ƏI���҂�
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

	/* ����V�[�P���X���L�b�N	*/
	m_dwSeqPattern = enumSeqPattern;
	m_iSeqResult = 0;
	if (0 == this->KickCtrlSeq(2)) {
		this->Logging(_T("Failed to KickCtrlSeq() in WaitSeqEndStart()."));
		l_bRet = FALSE;
	}

	if (TRUE == l_bRet) {
		if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hEvWaitEndSeq, SEQWAIT_TIMEOUT)) {
			/* �^�C���A�E�g���܂����D	*/
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
 *	����V�[�P���X���L�b�N
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
 *	�^�C�} �R�[���o�b�N�֐�
 */
void CALLBACK TimerHandler(
		HWND		hWnd,		/* �E�B���h�E�̃n���h��	*/
		UINT		uiMsg,		/* WM_TIMER ���b�Z�[�W	*/
		UINT_PTR	idEvent,	/* �^�C�}�̎��ʎq		*/
		DWORD		dwTime		/* ���݂̃V�X�e������	*/
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
 *	�^�C���A�E�g�Ď� �ݒ�/����
 */
BOOL CCtaCtrl::SetTimeOut(BOOL bMode, UINT uiElapse)
{
	BOOL l_bRet = TRUE;

	if (0 == bMode) {
		/* ����	*/
		if ((0 != lgs_uiTimerID) && (0 == m_bTimeOut)) {
			BOOL l_bRstl = ::KillTimer(0, lgs_uiTimerID);
			lgs_uiTimerID = 0;
			m_bTimeOut = FALSE;
			if (0 == l_bRstl) {
				l_bRet = FALSE;
//				ASSERT(0);	/* �s�v�ł��D	*/
			}
		}
	} else {
		/* �ݒ�(�J�n)	*/
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
 *	��M�f�[�^�����
 */
int CCtaCtrl::AnaRcvData(LPCTARCVDATA pCtaRcvData, LPCTADATAINF pCtaDataInf)
{
	int l_iRet = 0;

	if (0 == pCtaRcvData) {
		l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
	} else if (0 == pCtaRcvData->bRecved) {
		l_iRet = CTACTL_ERR_DATANONE;	/* �f�[�^�͂���܂���D	*/
	} else {
		if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
			/* �G���[���X�|���X --------------------------------------------*/
			if (3 != strlen(pCtaRcvData->pszRcvData)) {
				l_iRet = CTACTL_ERR_DATAFORMAT;	/* �f�[�^(�t�H�[�}�b�g)�G���[	*/
			} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
				l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
			} else {
				if (0 != pCtaDataInf) {
					pCtaDataInf->ResERR.dwErrRes = atol(pCtaRcvData->pszRcvData);
				}
			}
		} else {
			/* ���X�|���X�C�܂��́C�C�x���g --------------------------------*/
			switch (*pCtaRcvData->pdwCmdCode) {
			case CMD_STA:	/* STA ���X�|���X ------------------------------*/
				if (3 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* �f�[�^(�t�H�[�}�b�g)�G���[	*/
				} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
				} else {
					DWORD l_dwZState = pCtaRcvData->pszRcvData[0] & 0x0f;
					DWORD l_dwStatus1 = pCtaRcvData->pszRcvData[1] & 0x0f;
					DWORD l_dwStatus2 = pCtaRcvData->pszRcvData[2] & 0x0f;
					if ((l_dwZState < 1) || (3 < l_dwZState)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
					} else if ((l_dwStatus1 < 1) || (3 < l_dwStatus1)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
					} else if ((l_dwStatus2 < 1)
							|| (4 < l_dwStatus2)
							|| (2 == l_dwStatus2)) {
						l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
					} else {
						if (0 != pCtaDataInf) {
							pCtaDataInf->CmdSTA.dwZState = l_dwZState;
							pCtaDataInf->CmdSTA.dwStatus1 = l_dwStatus1;
							pCtaDataInf->CmdSTA.dwStatus2 = l_dwStatus2;
						}
					}
				}
				break;
			case CMD_RST:	/* RST ���X�|���X ------------------------------*/
			case CMD_ORG:	/* ORG ���X�|���X ------------------------------*/
			case CMD_MOD:	/* MOD ���X�|���X ------------------------------*/
			case CMD_CID:	/* CID ���X�|���X ------------------------------*/
			case CMD_PID:	/* PID ���X�|���X ------------------------------*/
			case CMD_SPT:	/* SPT ���X�|���X ------------------------------*/
			case CMD_MST:	/* MST ���X�|���X ------------------------------*/
			case CMD_MPE:	/* MPE ���X�|���X ------------------------------*/
			case CMD_MCE:	/* MCE ���X�|���X ------------------------------*/
			case EVT_CMO:	/* CMO �C�x���g --------------------------------*/
			case EVT_CST:	/* CST �C�x���g --------------------------------*/
			case EVT_CMA:	/* CMA �C�x���g --------------------------------*/
				if (0 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* �f�[�^(�t�H�[�}�b�g)�G���[	*/
				}
				break;
			case EVT_REP:	/* REP �C�x���g --------------------------------*/
				if (3 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* �f�[�^(�t�H�[�}�b�g)�G���[	*/
				} else if (-1 != this->IsStrDecimal(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
				} else {
					if (0 != pCtaDataInf) {
						pCtaDataInf->EvtREP.dwErrCode = atol(pCtaRcvData->pszRcvData);
					}
				}
				break;
			case EVT_CMP:	/* CMP �C�x���g --------------------------------*/
				if (17 != strlen(pCtaRcvData->pszRcvData)) {
					l_iRet = CTACTL_ERR_DATAFORMAT;	/* �f�[�^(�t�H�[�}�b�g)�G���[	*/
				} else {
					struct {
						char l_szPointNo[8];	/* 4 �|�C���g No.	*/
						char l_szCAngle[8];		/* 4 �ڐG�p[0.1degree]	*/
						char l_szRadius[8];		/* 5 ���a[0.1um]	*/
						char l_szLiquid[8];		/* 4 �t��[0.01uL]	*/
					} l_DataTmp;
					memset(&l_DataTmp, 0, sizeof(l_DataTmp));
					memcpy(l_DataTmp.l_szPointNo, &(pCtaRcvData->pszRcvData[0]), 4);
					memcpy(l_DataTmp.l_szCAngle, &(pCtaRcvData->pszRcvData[4]), 4);
					memcpy(l_DataTmp.l_szRadius, &(pCtaRcvData->pszRcvData[8]), 5);
					memcpy(l_DataTmp.l_szLiquid, &(pCtaRcvData->pszRcvData[13]), 4);
					char *pBlankPos;
					int l_iPointNo;
					if (0 == l_iRet) {	/* �|�C���g No.	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szPointNo)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szPointNo, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iPointNo = atol(l_DataTmp.l_szPointNo);
							if ((l_iPointNo <= 0) || (SCAN_POINT_CTA_MAX < l_iPointNo)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
							}
						}
					}
					int l_iCAngle;
					if (0 == l_iRet) {	/* �ڐG�p	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szCAngle)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szCAngle, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iCAngle = atol(l_DataTmp.l_szCAngle);
							if ((l_iCAngle < 0) || (9999 < l_iCAngle)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
							}
						}
					}
					int l_iRadius;
					if (0 == l_iRet) {	/* ���a	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szRadius)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szRadius, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iRadius = atol(l_DataTmp.l_szRadius);
							if ((l_iRadius < 0) || (99999 < l_iRadius)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
							}
						}
					}
					int l_iLiquid;
					if (0 == l_iRet) {	/* �t��	*/
						if (-1 != this->IsStrDecimalSign(l_DataTmp.l_szLiquid)) {
							l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
						} else {
							pBlankPos = strchr(l_DataTmp.l_szLiquid, ' ');
							if (0 != pBlankPos) {*pBlankPos = 0x00;}
							l_iLiquid = atol(l_DataTmp.l_szLiquid);
							if ((l_iLiquid < 0) || (9999 < l_iLiquid)) {
								l_iRet = CTACTL_ERR_PARAMERR;	/* �p�����^�G���[	*/
							}
						}
					}
					if (0 == l_iRet) {	/* ���ׂĐ���Ȃ̂ŃR�s�[	*/
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
 *	������̂P�O�i���`�F�b�N
 *		�߂�l�F	-1�F���ׂĂO(0x30)�`�X(0x39)�ł����D
 *					�ȊO�F�P�O�i���ȊO�̕����ʒu(�I�t�Z�b�g)
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
 *	������̂P�O�i���`�F�b�N�����t��
 *		�P�O�i��������(+/-�F�擪�����̂�)���u�����N(�Ō�̕��̂�)���`�F�b�N���܂��D
 *		�߂�l�F	-1�F���ׂĂO(0x30)�`�X(0x39)�ł����D
 *					�ȊO�F�P�O�i���ȊO�̕����ʒu(�I�t�Z�b�g)
 */
int CCtaCtrl::IsStrDecimalSign(LPCSTR pstrSrc)
{
	int l_iRet = -1;

	BOOL l_bFoundBlank = FALSE;
	BOOL l_bSign = FALSE;
	int i = 0;
	while ((0x00 != pstrSrc[i]) && (i < 1024)) {
		if (0 == i) {
			/* �P���ڂ͐���(0�`9)������(+/-)�ȊO�̓G���[	*/
			if (0 != isdigit(pstrSrc[i])) {
				;	/* ����������	*/
			} else if (('+' != pstrSrc[i]) && ('-' != pstrSrc[i])) {
				l_bSign = TRUE;	/* ����������	*/
			} else {
				l_iRet = i;
				break;
			}
		} else if (1 == i) {
			/* �Q����	*/
			if (0 != l_bSign) {
				/* �P���ڂ������������ꍇ	*/
				if (0 == isdigit(pstrSrc[i])) {
					/* �Q���ڂ͐����łȂ���΂Ȃ�Ȃ�	*/
					l_iRet = i;
					break;
				}
			} else if (0 != isdigit(pstrSrc[i])) {
				;	/* ����������	*/
			} else if (' ' == pstrSrc[i]) {
				l_bFoundBlank = TRUE;	/* �u�����N(0x20)������	*/
			} else {
				l_iRet = i;
				break;
			}
		} else {
			/* �R���ڈȍ~	*/
			if (0 != l_bFoundBlank) {
				/* �ȑO�Ƀu�����N���������ꍇ	*/
				if (' ' != pstrSrc[i]) {
					/* �u�����N�ł��葱���Ȃ���΂Ȃ�Ȃ�	*/
					l_iRet = i;
					break;
				}
			} else if (0 != isdigit(pstrSrc[i])) {
				;	/* ����������	*/
			} else if (' ' == pstrSrc[i]) {
				l_bFoundBlank = TRUE;	/* �u�����N(0x20)������	*/
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
 *	������̉p���`�F�b�N
 *		�߂�l�F	-1�F���ׂĂO(0x30)�`�X(0x39)�CA(0x41)�`Z(0x5a)�Ca(0x61)�`z(0x7a)�ł����D
 *					�ȊO�F�p���ȊO�̕����ʒu(�I�t�Z�b�g)
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
 *	�G���[���X�|���X���M
 */
int CCtaCtrl::SendErrRes(int iResult)
{
	int l_iRet = 0;

	int l_iErrCode = 0;
	switch (iResult) {
	case CTASIO_ERR_INVALID_PKT:	/* �p�P�b�g�R�[�h���͈͊O�ł�	*/
		l_iErrCode = CTACTL_ERCD_HEAD;	/* �w�b�_�G���[	*/
		break;
	case CTASIO_ERR_INVALID_CMD:	/* �R�}���h�R�[�h���͈͊O�ł�	*/
	case CTASIO_ERR_INVALID_EVT:	/* �C�x���g�R�[�h���͈͊O�ł�	*/
	case CTASIO_ERR_CTRLCODE:		/* ����R�[�h��������	*/
	case CTACTL_ERR_DATAFORMAT:		/* �f�[�^(�t�H�[�}�b�g)�G���[	*/
	case CTACTL_ERR_PARAMERR:		/* �p�����^�G���[	*/
		l_iErrCode = CTACTL_ERCD_CMD;	/* �R�}���h�G���[	*/
		break;
	case CTASIO_ERR_SUM:			/* �`�F�b�N�T���s��v	*/
		l_iErrCode = CTACTL_ERCD_SUM;	/* �`�F�b�N�T���G���[	*/
		break;
	case CTASIO_ERR_TOOSHORT:		/* ��M�f�[�^�����Z�����܂�	*/
	case CTASIO_ERR_INVALIDLEN:		/* ��M�f�[�^���ُ�	*/
	case CTASIO_ERR_NOLF:			/* LF ������܂���	*/
	case CTASIO_ERR_NOCR:			/* CR ������܂���	*/
		l_iErrCode = CTACTL_ERCD_LEN;	/* ��M�f�[�^���G���[	*/
		break;
		break;
	default:
		/* ����`���ʂ͉������܂���D	*/
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
 *	�C�x���g�ɑ΂��郌�X�|���X�𑗐M���܂��D
 */
int CCtaCtrl::SendEvtRes(LPCTARCVDATA pCtaRcvData)
{
	int l_iRet = 0;

	CTADATAINF l_CtaDataInf;
	int l_iRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
	pCtaRcvData->iRslt = l_iRslt;
	pCtaRcvData->bAnaed = TRUE;
	if (0 != l_iRslt) {
		/* �G���[���X�|���X�ŉ���	*/
		l_iRet = this->SendErrRes(l_iRslt);
	} else {
		/* ���탌�X�|���X����	*/
		l_iRet = ((CCtaSio*) m_pclsCCtaSio)->CmdSend(PKT_RES, (COMMAND_CODE) *pCtaRcvData->pdwCmdCode, "");
		if (EVT_REP == *pCtaRcvData->pdwCmdCode) {
			pCtaRcvData->dwErrCode = l_CtaDataInf.EvtREP.dwErrCode;
/* added 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- { ---------- */
			if (CTACTL_ERCD_RST == pCtaRcvData->dwErrCode) {
				m_bErrRep = FALSE;
			} else {
				m_bErrRep = TRUE;
			}
/* added 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- } ---------- */
		}
	}

	if (0 == l_iRet) {
		l_iRet = pCtaRcvData->iRslt;
	}

	return l_iRet;
}

/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
/*
 *	�V�[�P���X�F������
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
///* modified 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- { ---------- */
////	static BOOL ls_bRST2nd = FALSE;
///* modified 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ----------              */
//	static BOOL ls_bRST2nd = TRUE;
///* modified 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- } ---------- */
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;
	int l_iPrcRec = 0;

	switch (l_iPrcRec = ls_iPrc) {
	case PRC_INIT_CMD_RST0 + 00:	/* �g���u�����Z�b�g ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqInitCTA()"));
			nexioCA_Interlock(TRUE);	/* CTAILPO �I��	*/
			::Sleep(m_uiTimeout_ILPO);
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_RST, "");
		ls_iPrc = PRC_INIT_CMD_RST0 + 10;
		break;
	case PRC_INIT_CMD_RST0 + 10:	/* �g���u�����Z�b�g�̃��X�|���X��M�҂� ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v�����������邱�ƁE�E�E	*/
				}
			} else if (CMD_RST == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					/* �t�H�[�}�b�g�n�G���[������	*/
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					ls_iPrc = PRC_INIT_CMD_STA;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_RST0;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_INIT_CMD_STA + 00:		/* ��ԗv�� --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqInitCTA()"));
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_STA, "");
		ls_iPrc = PRC_INIT_CMD_STA + 10;
		break;
	case PRC_INIT_CMD_STA + 10:		/* ��ԗv���̃��X�|���X��M�҂� ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v�����������邱�ƁE�E�E	*/
				}
			} else if (CMD_STA == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else if ((1 != l_CtaDataInf.CmdSTA.dwStatus1) && (2 != l_CtaDataInf.CmdSTA.dwStatus1)) {
					/* �u���蒆�F�P�v���u�ҋ@���F�Q�v�ȊO�̏ꍇ	*/
					l_iRslt = CTACTL_ERR_EQBUSY;
				} else if ((1 != l_CtaDataInf.CmdSTA.dwStatus2) && (3 != l_CtaDataInf.CmdSTA.dwStatus2)) {
					/* �u�P�F�G���[�����v���u�w�b�h�G���[�F�R�v�ȊO�̏ꍇ	*/
					l_iRslt = CTACTL_ERR_EQERR;
				} else {
					ls_dwRetryCnt = 0;
/* modified 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- { ---------- */
//					ls_iPrc = PRC_INIT_CMD_RST;
/* modified 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ----------              */
					if (FALSE != m_bErrRep) {
						ls_iPrc = PRC_INIT_CMD_RST;
					} else {
						ls_iPrc = PRC_INIT_CMD_ORG;
					}
/* modified 2010.07.15 hmenjo PO �I���� RST �R�}���h�Ή� ---------- } ---------- */
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_STA;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_INIT_CMD_RST + 00:		/* �g���u�����Z�b�g�O�� ILPO �I�� ----------------------------- */
		ls_iPrc = PRC_INIT_CMD_RST + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);
		nexioCA_Interlock(TRUE);		/* CTAILPO �I��	*/
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_INIT_CMD_RST + 10:		/* �g���u�����Z�b�g�O�� ILPI �I���`�F�b�N --------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* �^�C���A�E�g�Ď� ����	*/
			ls_iPrc = PRC_INIT_CMD_RST + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_INIT_CMD_RST + 20:		/* �g���u�����Z�b�g ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_RST, "");
		ls_iPrc = PRC_INIT_CMD_RST + 30;
		break;
	case PRC_INIT_CMD_RST + 30:		/* �g���u�����Z�b�g�̃��X�|���X��M�҂� ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v�����������邱�ƁE�E�E	*/
				}
			} else if (CMD_RST == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					/* �t�H�[�}�b�g�n�G���[������	*/
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_NORM);	/* (��b)�^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_INIT_CMD_RST + 40;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_RST + 20;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_INIT_CMD_RST + 40:		/* �G���[�����C�x���g��M�҂� --------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_REP == *pCtaRcvData->pdwCmdCode) {
				/* �G���[�����C�x���g�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
					l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
				} else
				if (CTACTL_ERCD_RST == l_CtaDataInf.EvtREP.dwErrCode) {
					ls_iPrc = PRC_INIT_CMD_ORG;
					ls_dwRetryCnt = 0;
					this->KickCtrlSeq(l_iPrcRec + 1);
				} else {
					/* �C���^���b�N�ُ�ł��D	*/
					l_iRslt = CTACTL_ERR_ILPO;
					l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		}
		break;
	case PRC_INIT_CMD_ORG + 00:		/* ���_���A�O�� ILPO �I�� ------------------------------------- */
		ls_iPrc = PRC_INIT_CMD_ORG + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);
		nexioCA_Interlock(TRUE);		/* CTAILPO �I��	*/
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_INIT_CMD_ORG + 10:		/* ���_���A�O�� ILPI �I���`�F�b�N ----------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* �^�C���A�E�g�Ď� ����	*/
			ls_iPrc = PRC_INIT_CMD_ORG + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_INIT_CMD_ORG + 20:		/* ���_���A --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_ORG, "");
		ls_iPrc = PRC_INIT_CMD_ORG + 30;
		break;
	case PRC_INIT_CMD_ORG + 30:		/* ���_���A�̃��X�|���X��M�҂� ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v�����������邱�ƁE�E�E	*/
				}
			} else if (CMD_ORG == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_ORG);	/* (��b)�^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_INIT_EVT_CMO;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_INIT_CMD_ORG + 20;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_INIT_EVT_CMO + 00:		/* �ړ������C�x���g��M�҂� ----------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMO == *pCtaRcvData->pdwCmdCode) {
				/* �ړ������C�x���g�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
					l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_ILPI * 2);	/* IL �^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_INIT_ILPI_ON;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		}
		break;
	case PRC_INIT_ILPI_ON + 00:		/* CTAILPI �I���҂� ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO �I�t	*/
			this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* �����I��	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* �V�[�P���X�I��	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO �I�t	*/
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
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */

/*
 *	�V�[�P���X�F���ݒ�
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
	case 0:		/* �R�}���h���M	*/
		switch (ls_enumCmdCode) {
		case CMD_MOD:		/* ���胂�[�h	*/
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
		case CMD_CID:		/* �J�Z�b�g���Z�b�g	*/
			strcpy(l_szSendData, m_CtaMeasInf.szCstID);
			break;
		case CMD_PID:		/* ����Z�b�g	*/
			strcpy(l_szSendData, m_CtaMeasInf.szSampleID);
			break;
		case CMD_SPT:		/* ����ݏ��Z�b�g	*/
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
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, ls_enumCmdCode, l_szSendData);
		ls_iPrc = 1;
		break;
	case 1:		/* �R�}���h�̃��X�|���X��M�҂�	*/
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v���������邱�ƁE�E�E	*/
				}
			} else if (ls_enumCmdCode == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
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
						this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_NORM);	/* (��b)�^�C���A�E�g�Ď� �J�n	*/
					} else {
						this->KickCtrlSeq(211);
					}
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = 0;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(212);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case 2:		/* �ݒ芮���C�x���g��M�҂�	*/
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CST == *pCtaRcvData->pdwCmdCode) {
				/* �ړ������C�x���g�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				}
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* �����I��	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* �V�[�P���X�I��	*/
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

/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
/*
 *	�V�[�P���X�F����J�n
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
	case PRC_MEAS_CMD_MST + 00:		/* ����J�n�O�� ILPO �I�� ------------------------------------- */
		this->Logging(_T("Started SeqMeasStart()"));
		ls_iPrc = PRC_MEAS_CMD_MST + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI);
		nexioCA_Interlock(TRUE);		/* CTAILPO �I��	*/
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_MEAS_CMD_MST + 10:		/* ����J�n�O�� ILPI �I���`�F�b�N ----------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* �^�C���A�E�g�Ď� ����	*/
			ls_iPrc = PRC_MEAS_CMD_MST + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_MEAS_CMD_MST + 20:		/* ����J�n --------------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
			m_bRecvedCMA = FALSE;
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MST, "");
		ls_iPrc = PRC_MEAS_CMD_MST + 30;
		break;
	case PRC_MEAS_CMD_MST + 30:		/* ����J�n�̃��X�|���X��M�҂� ------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v���������邱�ƁE�E�E	*/
				}
			} else if (CMD_MST == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_T9_Meas);	/* (��b)�^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_MEAS_EVT_CMP;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEAS_CMD_MST + 20;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_MEAS_EVT_CMP + 00:		/* �P�|�C���g���芮���C�x���g��M�҂� ------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMP == *pCtaRcvData->pdwCmdCode) {
				/* �P�|�C���g���芮���C�x���g�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
					l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
				} else {
					/* �f�[�^�Z�b�g ----------------------------------------*/
					CTAPOINTDATA l_CtaPointData;
					l_CtaPointData.iPointNo = l_CtaDataInf.EvtCMP.iPointNo;
					l_CtaPointData.dCAngle = ((double) l_CtaDataInf.EvtCMP.iCAngle) / 10.0;
					l_CtaPointData.dRadius = ((double) l_CtaDataInf.EvtCMP.iRadius) / 10.0;
					l_CtaPointData.dLiquid = ((double) l_CtaDataInf.EvtCMP.iLiquid) / 100.0;
					/* ���L�����u���[�V�������܂��D	*/
/* modified 2009.11.16 hmenjo CTA ���L�����u�L���`�F�b�N ---------- { ---------- */
//					if (0 < _tcslen(m_CtaMeasInf.tszRecalibCA)) {
//						/* �ڐG�p�̃��L�����u�w��L��	*/
//						if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibCA, l_CtaPointData.dCAngle)) {
//							l_iRslt = CTACTL_ERR_RECALIB;	/* ���L�����u�G���[*/
//						}
//					}
//					if ((0 == l_iRslt)
//					 && (0 < _tcslen(m_CtaMeasInf.tszRecalibRad))) {
//						/* ���a�̃��L�����u�w��L��	*/
//						if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibRad, l_CtaPointData.dRadius)) {
//							l_iRslt = CTACTL_ERR_RECALIB;	/* ���L�����u�G���[*/
//						}
//					}
/* modified 2009.11.16 hmenjo CTA ���L�����u�L���`�F�b�N ---------- 			 */
					/* �ڐG�p�̃��L�����u�w��L��	*/
					if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibCA, l_CtaPointData.dCAngle)) {
						l_iRslt = CTACTL_ERR_RECALIB;	/* ���L�����u�G���[*/
					} else
					/* ���a�̃��L�����u�w��L��	*/
					if (0 == CtaRecalib(m_CtaMeasInf.tszRecalibRad, l_CtaPointData.dRadius)) {
						l_iRslt = CTACTL_ERR_RECALIB;	/* ���L�����u�G���[*/
					}
/* modified 2009.11.16 hmenjo CTA ���L�����u�L���`�F�b�N ---------- } ---------- */
					if (0 != l_iRslt) {
						l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
					} else {
						/* ���茋�� DB �ɃZ�b�g	*/
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->iPointNo = l_CtaPointData.iPointNo;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dCAngle = l_CtaPointData.dCAngle;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dRadius = l_CtaPointData.dRadius;
						lg_smCtaResultDataBase.GetSharedMemoryPtr()->dLiquid = l_CtaPointData.dLiquid;

						this->SetTimeOut(TRUE, m_uiTimeout_ILPI);	/* IL �^�C���A�E�g�Ď� �J�n	*/
						ls_iPrc = PRC_MEAS_ILPI_ON;
						this->KickCtrlSeq(l_iPrcRec + 1);
					}
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		}
		break;
	case PRC_MEAS_ILPI_ON + 00:		/* CTAILPI �I���҂� ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO �I�t	*/
			this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* �����I��	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* �V�[�P���X�I��	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO �I�t	*/
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
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */

/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- { ---------- */
/*
 *	�V�[�P���X�F����I��
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
	case PRC_MEND_CMD_MPE + 00:		/* ��I���ʒm�O�� ILPO �I�� --------------------------------- */
		this->Logging(_T("Started SeqMeasEnd()"));
		::Sleep(m_uiTimeout_MPE_Wait);
		ls_iPrc = PRC_MEND_CMD_MPE + 10;
		this->SetTimeOut(TRUE, m_uiTimeout_ILPI);
		nexioCA_Interlock(TRUE);		/* CTAILPO �I��	*/
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- { ---------- */
		::Sleep(m_uiDelay_ILPO);
/* added 2011.02.03 hmenjo CTA PO ��f�B���C ---------- } ---------- */
		this->KickCtrlSeq(l_iPrcRec + 1);
		break;
	case PRC_MEND_CMD_MPE + 10:		/* ��I���ʒm�O�� ILPI �I���`�F�b�N ------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			this->SetTimeOut(FALSE,  0);	/* �^�C���A�E�g�Ď� ����	*/
			ls_iPrc = PRC_MEND_CMD_MPE + 20;
			this->KickCtrlSeq(l_iPrcRec + 1);
		} else if (TRUE == m_bTimeOut) {
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 2);
		}
		break;
	case PRC_MEND_CMD_MPE + 20:		/* ��I���ʒm ----------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
			m_bRecvedCMA = FALSE;
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MPE, "");
		ls_iPrc = PRC_MEND_CMD_MPE + 30;
		break;
	case PRC_MEND_CMD_MPE + 30:		/* ��I���ʒm�̃��X�|���X��M�҂� --------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v���������邱�ƁE�E�E	*/
				}
			} else if (CMD_MPE == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					this->SetTimeOut(TRUE, CTA_TIMEOUT_T9_ORG);	/* (��b)�^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_MEND_EVT_CMO;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEND_CMD_MPE + 20;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 1);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_MEND_EVT_CMO + 00:		/* �ړ������C�x���g��M�҂� ----------------------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (EVT_CMO == *pCtaRcvData->pdwCmdCode) {
				/* �ړ������C�x���g�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
					l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_T9_AllEnd);	/* (��b)�^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_MEND_EVT_CMA;
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		}
		break;
	case PRC_MEND_EVT_CMA + 00:		/* �S�|�C���g���芮���C�x���g��M�����X�|���X���M�����҂� ----- */
		if (0 != m_bRecvedCMA) {
			m_bRecvedCMA = FALSE;
			this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
			ls_dwRetryCnt = 0;
			ls_iPrc = PRC_MEND_CMD_MCE;
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		}
		break;
	case PRC_MEND_CMD_MCE + 00:		/* �J�Z�b�g�I���ʒm ------------------------------------------- */
		if (0 == ls_dwRetryCnt) {
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_MCE, m_CtaMeasInf.szCstID);
		ls_iPrc = PRC_MEND_CMD_MCE + 10;
		break;
	case PRC_MEND_CMD_MCE + 10:		/* �J�Z�b�g�I���ʒm�̃��X�|���X��M�҂� ----------------------- */
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v���������邱�ƁE�E�E	*/
				}
			} else if (CMD_MCE == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					this->SetTimeOut(TRUE, m_uiTimeout_ILPI);	/* IL �^�C���A�E�g�Ď� �J�n	*/
					ls_iPrc = PRC_MEND_ILPI_ON;
					this->KickCtrlSeq(l_iPrcRec + 1);
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = PRC_MEND_CMD_MCE;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(l_iPrcRec + 2);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	case PRC_MEND_ILPI_ON + 00:		/* CTAILPI �I���҂� ------------------------------------------- */
		if (0 != nexioIsCA_Interlock()) {
			nexioCA_Interlock(FALSE);	/* CTAILPO �I�t	*/
			this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
			l_iRslt = 0;
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
			l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
		} else {
			::Sleep(m_uiTimeout_Retry / 2);
			this->KickCtrlSeq(l_iPrcRec + 1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* �����I��	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* �V�[�P���X�I��	*/
		nexioCA_Interlock(FALSE);	/* CTAILPO �I�t	*/
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
/* added 2010.09.09 hmenjo CTA PO/PI �n���h�V�F�[�N ---------- } ---------- */
/*
 *	�V�[�P���X�F��Ԏ擾
 */
int CCtaCtrl::SeqGetStat(LPCTARCVDATA pCtaRcvData)
{
	static int ls_iPrc = 0;
	static DWORD ls_dwRetryCnt = 0;
	int l_iRslt = 0;
	int l_iAnaRslt = 0;
	BOOL l_bSeqEnd = FALSE;

	switch (ls_iPrc) {
	case 0:		/* ��ԗv��	*/
		if (0 == ls_dwRetryCnt) {
			this->Logging(_T("Started SeqGetStat()"));
			ls_dwRetryCnt = m_dwRetryCnt + 1;	/* ���g���C�J�E���^�Z�b�g	*/
		}
		this->SetTimeOut(TRUE, CTA_TIMEOUT_RES);	/* �^�C���A�E�g�Ď� �J�n	*/
		((CCtaSio*) this->m_pclsCCtaSio)->CmdSend(PKT_CMD, CMD_STA, "");
		ls_iPrc = 1;
		break;
	case 1:		/* ��ԗv���̃��X�|���X��M�҂�	*/
		if (0 != pCtaRcvData->bRecved) {
			/* ��M�f�[�^�������D	*/
			CTADATAINF l_CtaDataInf;
			l_iAnaRslt = this->AnaRcvData(pCtaRcvData, &l_CtaDataInf);
			if (PKT_ERR == *pCtaRcvData->pdwPktCode) {
				/* �G���[���X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					l_iRslt = l_CtaDataInf.ResERR.dwErrRes;
					/* �G���[���e�ɂ�苭���I���̕K�v�����������邱�ƁE�E�E	*/
				}
			} else if (CMD_STA == *pCtaRcvData->pdwCmdCode) {
				/* �������X�|���X�������D	*/
				pCtaRcvData->bRecved = FALSE;
				this->SetTimeOut(FALSE, 0);	/* �^�C���A�E�g�Ď� ����	*/
				if (0 != l_iAnaRslt) {
					l_iRslt = l_iAnaRslt;	/* �t�H�[�}�b�g�n�G���[������	*/
				} else {
					m_lCtaStatus =	MAKELONG(	/* �X�e�[�^�X	*/
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
					l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
				}
			}
		}
		if (TRUE == m_bTimeOut) {
			/* �^�C���A�E�g���܂����D	*/
			m_bTimeOut = FALSE;
			l_iRslt = CTACTL_ERR_TIMEOUT;	/* �^�C���A�E�g	*/
		}
		if (0 != l_iRslt) {
			ls_dwRetryCnt--;
			if (0 != ls_dwRetryCnt) {
				ls_iPrc = 0;	/* ���g���C	*/
				::Sleep(m_uiTimeout_Retry);
				this->KickCtrlSeq(511);
			} else {
				l_bSeqEnd = TRUE;	/* �V�[�P���X�I��	*/
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	/* �����I��	*/
	if (0 != m_bForceEnd) {
		this->SetTimeOut(FALSE, 0);
		m_bForceEnd = FALSE;
		l_bSeqEnd = TRUE;
		l_iRslt = CTACTL_ERR_FORCEEND;
	}

	if (TRUE == l_bSeqEnd) {
		/* �V�[�P���X�I��	*/
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
 *	��ʂւ̃A���[���ʒm
 */
BOOL CCtaCtrl::NotifyAlarm(int iAlarmCode)
{
	if (0 == iAlarmCode) {
		return FALSE;	/* ���ʒm	*/
	}

	int l_iNotifyCode = 0;
	int l_iLevel = 0;		/*	0:�x��
								1:
								*/

	switch (iAlarmCode) {
	/* CTASIO �G���[��`	*/
	default:						l_iNotifyCode = 100; l_iLevel = 0;	break;	/* ����`�G���[	*/
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
	/* CTACTL �G���[��`	*/
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
	/* CTA ���j�b�g �G���[�R�[�h	*/
/*	case CTACTL_ERCD_RST:			l_iNotifyCode = 140; l_iLevel = 0;	break;	���g�p	*/
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
