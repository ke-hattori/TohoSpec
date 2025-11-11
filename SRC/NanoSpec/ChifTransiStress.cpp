// ChifTransiStress.cpp : implementation file
//

#include "stdafx.h"
#include <Active.hxx>
#include "nanospec.h"
#include "ChifTransiStress.h"
#include "ChiefExports.h"
#include "ChiefView.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ChifTransiSeq.h"
#include "ChifTransiMaster.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "..\\..\\INC\\NEXIF.HXX"
#include "..\\..\\INC\\NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	�}�N����`
 */
#define	STRS_HWS_TIME	1000	/* [ms] HWS ���[�h Sleep ����	*/


/*
 *	�\���̒�`
 */


/*
 *	���[�J���O���[�o����`
 */
UINT WM_CHIF_TRSTRS_SETEVENT = RegisterWindowMessage("WM_CHIF_TRSTRS_SETEVENT");	/* �g�����W�V�����C�x���g���s�p�E�B���h�E���b�Z�[�W	*/
UINT WM_CHIF_TRSTRS_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRSTRS_HEARTBEAT");	/* HeartBeat �E�B���h�E���b�Z�[�W	*/


/* ##########################################################################
 * class CChiefTransiStress : �X�g���X �g�����W�V����
 * ########################################################################*/

/* =========================================================================//
 *	�g�����W�V�����e�[�u��
 * =========================================================================*/
const Transition TransTable_Stress[] =
{
	//	OldState,				EventId,				NewState

	{	ST_STRS_INIT,			EV_STRS_DO_INIT,		ST_STRS_INIT			},	/* ���������������s(���� EV_AAF_INIT_DONE ��)			*/
	{	ST_STRS_INIT,			EV_STRS_INIT_DONE,		ST_STRS_IDLE			},	/* �������������ăA�C�h����								*/

	{	ST_STRS_IDLE,			EV_STRS_START,			ST_STRS_START			},	/* ���t�@�����X or ������ ���� ��						*/

	{	ST_STRS_START,			EV_STRS_MODE_MEAS,		ST_STRS_SRMEAS			},	/* �����胂�[�h�ŁFSR ����V�[�P���X�J�n & �����҂� ��	*/
	{	ST_STRS_START,			EV_STRS_MODE_REF,		ST_STRS_PRELDPOS		},	/* ���t�@�����X���[�h�F����O ���[�h Pos �����҂� ��	*/
//Saiki 20110308 Add ----->
	{	ST_STRS_START,			EV_STRS_MODE_FIX_MEAS,	ST_STRS_PRELDPOS		},	/* �����胂�[�h(�Œ薌���l)�F����O ���[�h Pos �����҂� ��	*/
//Saiki 20110308 Add <-----
	{	ST_STRS_START,			EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/

	{	ST_STRS_SRMEAS,			EV_STRS_SRMEAS_DONE,	ST_STRS_SRMEAS_DONE		},	/* SR ����V�[�P���X�������� ��							*/
	{	ST_STRS_SRMEAS,			EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
	{	ST_STRS_SRMEAS,			EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* �|�[�Y�����҂� ��									*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */

	{	ST_STRS_SRMEAS_DONE,	EV_STRS_MODE_MEAS,		ST_STRS_PRELDPOS		},	/* ����O ���[�h Pos �����҂� ��						*/
	{	ST_STRS_SRMEAS_DONE,	EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/

	{	ST_STRS_PRELDPOS,		EV_STRS_LDPOS_DONE,		ST_STRS_PINALIGN		},	/* �s�� �A���C�����g�҂� ��								*/
	{	ST_STRS_PRELDPOS,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
	{	ST_STRS_PRELDPOS,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* �|�[�Y�����҂� ��									*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */

	{	ST_STRS_PINALIGN,		EV_STRS_PIN_DONE,		ST_STRS_LMVSTAPOS		},	/* ���C������J�n�ʒu �ړ� & �����҂� ��				*/
	{	ST_STRS_PINALIGN,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
	{	ST_STRS_PINALIGN,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* �|�[�Y�����҂� ��									*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */

	{	ST_STRS_LMVSTAPOS,		EV_STRS_LINEMOVE_DONE,	ST_STRS_LMVSTAPOS		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_LMVSTAPOS,		EV_STRS_HEPA_STOP,		ST_STRS_LMVSTAPOS		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_LMVSTAPOS,		EV_STRS_TO_LMENDPOS,	ST_STRS_LMVENDPOS		},	/* ���C������I���ʒu �ړ� & �����҂� ��				*/
	{	ST_STRS_LMVSTAPOS,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
	{	ST_STRS_LMVSTAPOS,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* �|�[�Y�����҂� ��									*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */

	{	ST_STRS_LMVENDPOS,		EV_STRS_LINEMOVE_DONE,	ST_STRS_LMVENDPOS		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINEMEAS_DONE,	ST_STRS_LMVENDPOS		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINEDATA_DONE,	ST_STRS_LMVENDPOS		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINE_NEXT,		ST_STRS_LMVSTAPOS		},	/* ���� ���C������J�n�ʒu �ړ� & �����҂� ��			*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_LINE_ALLDONE,	ST_STRS_COMPLDPOS		},	/* ������ ���[�h Pos �����҂� ��						*/
	{	ST_STRS_LMVENDPOS,		EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
	{	ST_STRS_LMVENDPOS,		EV_STRS_PAUSE,			ST_STRS_PAUSE			},	/* �|�[�Y�����҂� ��									*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */

	{	ST_STRS_COMPLDPOS,		EV_STRS_LDPOS_DONE,		ST_STRS_PINDOWNCOMP		},	/* ������ �s�� �_�E�� & Complete �҂� ��				*/
// 2009.08.22 K.Matsuo -->
	{	ST_STRS_COMPLDPOS,		EV_STRS_COMPLETE,		ST_STRS_COMPLDPOS		},	/* ��ԋL���݂̂ł��̂܂�								*/
// 2009.08.22 K.Matsuo -->

	{	ST_STRS_PINDOWNCOMP,	EV_STRS_PIN_DONE,		ST_STRS_PINDOWNCOMP		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_PINDOWNCOMP,	EV_STRS_COMPLETE,		ST_STRS_PINDOWNCOMP		},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_PINDOWNCOMP,	EV_STRS_PINCMP_DONE,	ST_STRS_COMPLETEDELAY	},	/* Complete ���ԑ҂� ��									*/
/* added 2009.10.19 hmenjo Stress �g�����W�V�����e�[�u���ǉ� ---------- { ---------- */
	{	ST_STRS_PINDOWNCOMP,	EV_STRS_CANCEL,			ST_STRS_PINDOWNCOMP		},	/* (�s�����s������)��ԋL���݂̂ł��̂܂�				*/
/* added 2009.10.19 hmenjo Stress �g�����W�V�����e�[�u���ǉ� ---------- } ---------- */

	{	ST_STRS_COMPLETEDELAY,	EV_STRS_COMPLETE_DONE,	ST_STRS_IDLE			},	/* �A�C�h�� ��											*/

	{	ST_STRS_WAITABORT,		EV_STRS_CANCEL_DONE,	ST_STRS_ABORTLDPOS		},	/* Abort �� ���[�h Pos �����҂� ��						*/

	{	ST_STRS_ABORTLDPOS,		EV_STRS_LDPOS_DONE,		ST_STRS_ABORTPINDOWN	},	/* Abort �� �s�� �_�E���҂� ��							*/

	{	ST_STRS_ABORTPINDOWN,	EV_STRS_PIN_DONE,		ST_STRS_ABORTDELAY		},	/* Abort ���ԑ҂� ��									*/
/* added 2009.10.19 hmenjo Stress �g�����W�V�����e�[�u���ǉ� ---------- { ---------- */
	{	ST_STRS_ABORTPINDOWN,	EV_STRS_CANCEL,			ST_STRS_ABORTDELAY		},	/* (�s�����s������)Abort ���ԑ҂� ��					*/
/* added 2009.10.19 hmenjo Stress �g�����W�V�����e�[�u���ǉ� ---------- } ---------- */

	{	ST_STRS_ABORTDELAY,		EV_STRS_ABORT_DONE,		ST_STRS_IDLE			},	/* �A�C�h�� ��											*/

	{	ST_STRS_PAUSE,			EV_STRS_RESUME,			ST_STRS_PRE_RESUME		},	/* ���W���[���O���� ��									*/
	{	ST_STRS_PAUSE,			EV_STRS_SRMEAS_DONE,	ST_STRS_PAUSE			},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_PAUSE,			EV_STRS_LDPOS_DONE,		ST_STRS_PAUSE			},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_PAUSE,			EV_STRS_PIN_DONE,		ST_STRS_PAUSE			},	/* ��ԋL���݂̂ł��̂܂�								*/
	{	ST_STRS_PAUSE,			EV_STRS_CANCEL,			ST_STRS_WAITABORT		},	/* �ُ�C�L�����Z���ŁCAbort ��~�҂� ��				*/

	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_SRMEAS,		ST_STRS_SRMEAS			},	/* SR ����V�[�P���X�J�n & �����҂� ��					*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_SRMEASED,	ST_STRS_SRMEAS_DONE		},	/* SR ����V�[�P���X�������� ��							*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_PRELDPOS,	ST_STRS_PRELDPOS		},	/* ����O ���[�h Pos �����҂� ��						*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_PINALIGN,	ST_STRS_PINALIGN		},	/* �s�� �A���C�����g�҂� ��								*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_LMVSTAPOS,	ST_STRS_LMVSTAPOS		},	/* ���C������J�n�ʒu �ړ� & �����҂� ��				*/
	{	ST_STRS_PRE_RESUME,		EV_STRS_RSM_LMVENDPOS,	ST_STRS_LMVENDPOS		},	/* ���C������I���ʒu �ړ� & �����҂� ��				*/

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

/* =========================================================================//
 *	��ԕʏ����֐��e�[�u��
 * =========================================================================*/
BEGIN_STATEPROC_MAP(CChiefTransiStress)
	/*	0 */	ENTER_STATE(ST_STRS_INIT,			enter_ST_STRS_INIT),
	/*	1 */	ENTER_STATE(ST_STRS_IDLE,			enter_ST_STRS_IDLE),
	/*	2 */	ENTER_STATE(ST_STRS_START,			enter_ST_STRS_START),				/* (WAIT ��Ԗ���)			*/
	/*	3 */	ENTER_STATE(ST_STRS_SRMEAS,			enter_ST_STRS_SRMEAS),				/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	4 */	ENTER_STATE(ST_STRS_SRMEAS_DONE,	enter_ST_STRS_SRMEAS_DONE),			/* (WAIT ��Ԗ���)			*/
	/*	5 */	ENTER_STATE(ST_STRS_PRELDPOS,		enter_ST_STRS_PRELDPOS),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	6 */	ENTER_STATE(ST_STRS_PINALIGN,		enter_ST_STRS_PINALIGN),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	7 */	ENTER_STATE(ST_STRS_LMVSTAPOS,		enter_ST_STRS_LMVSTAPOS),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	8 */	ENTER_STATE(ST_STRS_LMVENDPOS,		enter_ST_STRS_LMVENDPOS),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	9 */	ENTER_STATE(ST_STRS_COMPLDPOS,		enter_ST_STRS_COMPLDPOS),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	10 */	ENTER_STATE(ST_STRS_PINDOWNCOMP,	enter_ST_STRS_PINDOWNCOMP),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	11 */	ENTER_STATE(ST_STRS_COMPLETEDELAY,	enter_ST_STRS_COMPLETEDELAY),		/* (WAIT ��Ԗ���)			*/
	/*	12 */	ENTER_STATE(ST_STRS_WAITABORT,		enter_ST_STRS_WAITABORT),			/* (WAIT ��Ԗ���)			*/
	/*	13 */	ENTER_STATE(ST_STRS_ABORTLDPOS,		enter_ST_STRS_ABORTLDPOS),			/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	14 */	ENTER_STATE(ST_STRS_ABORTPINDOWN,	enter_ST_STRS_ABORTPINDOWN),		/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	15 */	ENTER_STATE(ST_STRS_ABORTDELAY,		enter_ST_STRS_ABORTDELAY),			/* (WAIT ��Ԗ���)			*/
	/*	16 */	ENTER_STATE(ST_STRS_PAUSE,			enter_ST_STRS_PAUSE),				/* (WAIT ��Ԗ����̏ꍇ�L��)*/
	/*	17 */	ENTER_STATE(ST_STRS_PRE_RESUME,		enter_ST_STRS_PRE_RESUME),			/* (WAIT ��Ԗ���)			*/

END_STATEPROC_MAP(CChiefTransiStress)

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiStress

IMPLEMENT_DYNCREATE(CChiefTransiStress, CWinThread)

CChiefTransiStress::CChiefTransiStress(CWnd* pParent /*=NULL*/) : cActive(TransTable_Stress)
{
	TRACE(_T("CChiefTransiStress::CChiefTransiStress() \n"));

	m_pcChiefView = (CView*) pParent;

	/*	�X���b�h�̋N���҂�	*/
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiStress."));

	/*	�I�u�W�F�N�g�̎����j�����֎~	*/
	m_bAutoDelete = FALSE;

}

CChiefTransiStress::~CChiefTransiStress()
{
	TRACE(_T("CChiefTransiStress::~CChiefTransiStress() \n"));

	/*	�X���b�h�̏��ő҂�	*/
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiStress."));
		}
	}
}

BOOL CChiefTransiStress::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	TRACE(_T("CChiefTransiStress::InitInstance() \n"));

	/* �R���X�g���N�^�̑҂����������܂�
	 *		�܂�C�X���b�h�̐����ƃX���b�h�N���X�̍\�z�̓���������Ă��܂��D	*/
	m_cSyncEvent.SetEvent();

	/* ������ԏ����p�C�x���g���s	*/
	TransiEvent(0, 0);

	/* �ϐ�������	*/
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiStress::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here

	TRACE(_T("CChiefTransiStress::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiStress, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiStress)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRSTRS_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRSTRS_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiStress message handlers

/*
 *	�g�����W�V�����C�x���g���s ���b�Z�[�W�n���h��
 */
void CChiefTransiStress::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	/* �g�����W�V�����ԃ^�C�~���O����(�Ƃ肠���������ǍP�v�����E�E)	*/

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat ���b�Z�[�W�n���h��
 */
void CChiefTransiStress::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	���̑��֐�
****************************************************************************/

/*
 *	�g�����W�V�����C�x���g���s�p�֐�
 */
void CChiefTransiStress::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
	switch (iEventNo) {
	case EV_STRS_CANCEL:
		m_bCancel = TRUE;
		/* �����ł����炩���� ABORT �ɂ��Ă����܂�	*/
		if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);}
		break;
	case EV_STRS_HEPA_STOP:
		m_bHepaStopped = TRUE;
		break;
	case EV_STRS_PAUSE:
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
//		{
//			int l_iCurrentState = this->GetCurrentState();
//			if (
//				(ST_STRS_PAUSE == l_iCurrentState)
//			 || (ST_STRS_PRE_RESUME == l_iCurrentState)
//			 || (TRUE == m_bPause)
//			 || (ST_STRS_LMVSTAPOS == l_iCurrentState)
//			 || (ST_STRS_LMVENDPOS == l_iCurrentState)
//				) {
//				return;
//			}
//			m_iPausePreState = l_iCurrentState;
//		}
//		m_bPause = TRUE;
//		if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
//			/* �V�[�P���X���� �g�����W�V�����փ|�[�Y�w��	*/
//			((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);
//		}
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ----------			   */
		{
			int l_iCurrentState = this->GetCurrentState();
			switch (l_iCurrentState) {
			case ST_STRS_PAUSE:
			case ST_STRS_PRE_RESUME:
				return;
				break;
			default:
				if (TRUE == m_bPause) {
					return;
				}
				break;
			}
			m_iPausePreState = l_iCurrentState;
			m_bPause = TRUE;
			if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
				/* �V�[�P���X���� �g�����W�V�����փ|�[�Y�w��	*/
				((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_PAUSE);
			}
			if (
				(ST_STRS_LMVSTAPOS == l_iCurrentState)
			 || (ST_STRS_LMVENDPOS == l_iCurrentState)
				) {
				/*	enter_ST_STRS_LMVSTAPOS()�Center_ST_STRS_LMVENDPOS() ���ŁC
				 *	���ځCEvent() ���g���܂��̂ŁCWM_CHIF_TRSTRS_SETEVENT �͔��s���܂���D	*/
				return;
			}
		}
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */
		break;
	}

/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRSTRS_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRSTRS_SETEVENT, _T("WM_CHIF_TRSTRS_SETEVENT"), 9, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
}

/*
 *	HeartBeat ���s�p�֐�
 */
void CChiefTransiStress::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRSTRS_HEARTBEAT, 0, 0);
}

/*
 *	���݂̏�Ԃ��u�A�C�h���v���ǂ������`�F�b�N
 *		�߂�l�F	true	�A�C�h���ł���
 *					false	�A�C�h���łȂ�
 */
bool CChiefTransiStress::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_STRS_IDLE) {
		l_isRc = true;
	} else {
		l_isRc = false;
	}

	return l_isRc;
}

/*
 *	���݂̏�Ԃ��擾
 *		�߂�l�F	���݂̏��
 */
int CChiefTransiStress::GetCurrentState()
{
	return CurrState;
}

/*
 *	���݂̑���|�C���g�ԍ����擾
 *		�߂�l�F	����|�C���g�ԍ�
 */
DWORD CChiefTransiStress::GetCurrentLineNo()
{
	return m_dwLineNo;
}

/*
 *	���݂̃|�[�Y��Ԃ��擾
 *		�߂�l�F	�|�[�Y���
 */
bool CChiefTransiStress::IsPauseSeq()
{
	return (0 == m_bPause)? false : true;
}

/*
 *	���݁C���蒆���ǂ������擾
 *		�߂�l�F	������
 */
bool CChiefTransiStress::IsMeasuring()
{
	return (0 == m_bMeasStress)? false : true;
}

/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
bool CChiefTransiStress::IsPinAligningHWS()
{
	return (0 == m_bPinAligning)? false : true;
}
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */

/*
 *	�n���h�� ���[�h�|�W�V�����ֈړ�
 */
BOOL CChiefTransiStress::MoveToLdPos()
{
	/* ���W���擾���܂��D	*/
	STAGE_COORD l_SetPosition;
	ConfigFile_GetNanoSpecIni(&l_SetPosition, CONFIG_FILE_HANDLER_LOAD_POSITION);

/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- { ---------- */
	/* SR �w�b�h�ɂ��܂��D	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- } ---------- */

	return NS_StageMoveAbsoluteEx(&l_SetPosition);
}

/*
 *	����|�C���g��s�ړ�
 */
void CChiefTransiStress::PreMove()
{
	if (0 != m_bPause) {
		return;		/* �|�[�Y�w�ߎ��͐�s�ړ����܂���D	*/
	}

	if (2 == m_dwLineMoveToEndPos) {
		/* ���C������I���ʒu�ֈړ��ς������D	*/
		if (0 != ((CChiefView*) m_pcChiefView)->m_bPreMoveSW) {
			/* ��s�ړ��ݒ肪�C�l�[�u��	*/
			if (
				(ST_STRS_LMVENDPOS == CurrState)	/* ���C������I���ʒu �ړ� & �����҂�	*/
				) {
				/* ��s�ړ����L��	*/
				STRESS_CONFIG l_StressConfig;
				ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
				DWORD l_dwLineNo = m_dwLineNo + 1;
				if (((CChiefView*) m_pcChiefView)->GetNextLineNo(&l_dwLineNo) <= l_StressConfig.dwLiftPinNumberOfLine) {
					/* ���̑���Ώۃ��C�����������D	*/
					NS_RestoreStageSpeed();	/* ���x��߂��܂��D	*/
					/*
					 *	�R�R�ł́C�s���ʒu�� Z ���ʒu�̓`�F�b�N���܂���D
					 */
					/* ���C�����W��Ǐo��	*/
					STAGE_COORD l_SetPosition;
					STRESS_CONFIG l_StressConfig;
					ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
					l_SetPosition.lX = l_StressConfig.lLineStartPosX;		/* ���C������J�n���W X	*/
					LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(52) ---------- { ---------- */
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ---------- { ---------- */
////					l_SetPosition.lY = l_pStageProgStress->Line[l_dwLineNo - 1].LineSec.SectPos[0].lScanPosY;	/* ���C������J�n���W Y	*/
////																					/*	�Z�N�V�����P�� Y ���W���g���܂��D	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ----------				*/
//					l_SetPosition.lY = l_pStageProgStress->Line[l_dwLineNo - 1].SectPos[0].lScanPosY;	/* ���C������J�n���W Y	*/
//																					/*	�Z�N�V�����P�� Y ���W���g���܂��D	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ---------- } ---------- */
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(52) ----------			   */
					for (int i = 0; i < static_cast<int>(l_StressConfig.Line[l_dwLineNo - 1].dwSectionNum); i++) {
						if (0 != l_pStageProgStress->Line[l_dwLineNo - 1].bScanValid[i]) {
							break;
						}
					}
					if (static_cast<int>(l_StressConfig.Line[l_dwLineNo - 1].dwSectionNum) <= i) {
						TCHAR l_tszLog[128];
						_stprintf(l_tszLog, _T("No Sections in Line No.%d."), l_dwLineNo);
						((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
						/* �L�����Z�����|�X�g	*/
						this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_NOSECTION);
					} else {
					l_SetPosition.lY = l_pStageProgStress->Line[l_dwLineNo - 1].SectPos[i].lScanPosY;	/* ���C������J�n���W Y	*/
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(52) ---------- } ---------- */
					TCHAR l_tszLogText[256];
					_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d. (PreMove)"), l_dwLineNo);
					((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//					((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_STAPOS_PREMOVING);  // Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
// 2009.08.27 K.Matsuo -->
					TCHAR l_szMainRcpName[256];
					((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// 2009.08.27 K.Matsuo <--
					if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
						/* HWS �ł��D	*/
						::Sleep(STRS_HWS_TIME);
						m_dwLineMoveToStartPos = 1;	/* ���C������J�n�ʒu�F�ړ���	*/
						m_bPreMoving = TRUE;	/* ��s�ړ���	*/
						this->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
					} else
// 2009.08.27 K.Matsuo -->
//					if (0 == NS_StageMoveAbsoluteEx(&l_SetPosition)) {
					if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_SetPosition, l_szMainRcpName, HEAD_TYPE_STRESS)) {
// 2009.08.27 K.Matsuo <--
						/* �ړ��J�n���s	*/
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed_PreMotion));
						/* �L�����Z�����|�X�g	*/
						this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_LINEMOVE_FAIL);
					} else {
						/* �ړ��J�n����	*/
						m_dwLineMoveToStartPos = 1;	/* ���C������J�n�ʒu�F�ړ���	*/
						m_bPreMoving = TRUE;	/* ��s�ړ���	*/
						((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 73;	/* ���C������ �ړ���	*/
						((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* ���쒆�t���O(XY �X�e�[�W)���I��	*/
					}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(52) ---------- { ---------- */
					}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(52) ---------- } ---------- */
				}
			}
		}
	}
}

/*--------------------------------------------------------------------------//
 *	��ԕʏ����֐��Q
 *--------------------------------------------------------------------------*/
/*
 *	�������
 */
void CChiefTransiStress::enter_ST_STRS_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[0], (DWORD) pEvParams);

	m_bPinAligning = FALSE;		/* �s���̃A���C�����g�ʒu��(HWS �p)	*//* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) */
}

/*
 *	�A�C�h��
 */
void CChiefTransiStress::enter_ST_STRS_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[1], (DWORD) pEvParams);

	m_bHepaStopped = FALSE;
	m_dwLineMoveToStartPos = 0;
	m_dwLineMoveToEndPos = 0;
	m_bCompleted = FALSE;
	m_iCancelStat = 0;
	m_bCancel = FALSE;
	m_bPreMoving = FALSE;
	m_bPause = FALSE;
	m_iPausePreState = 0;
	memset(&m_PauseStat, 0, sizeof(m_PauseStat));
	m_bMeasStress = FALSE;
	m_bPinAligning = FALSE;		/* �s���̃A���C�����g�ʒu��(HWS �p)	*//* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) */

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_WAIT);}

	NS_RestoreStageSpeed();	/* ���x��߂��܂��D	*/

	/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
}

/*
 * ���t�@�����X or ������ ����(WAIT ��Ԗ���)
 */
void CChiefTransiStress::enter_ST_STRS_START(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo �����R�����g�ǉ� ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))�F�ďo���g�����W�V����
/* added 2009.09.09 hmenjo �����R�����g�ǉ� ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[2], (DWORD) pEvParams);

	/* �ďo�����g�����W�V������ۑ�	*/
/* modified 2009.09.09 hmenjo �����ύX ---------- { ---------- */
//	m_dwParentTransi = LOWORD(pEvParams);
/* modified 2009.09.09 hmenjo �����ύX ----------			   */
	m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
/* modified 2009.09.09 hmenjo �����ύX ---------- } ---------- */
	/* ���胉�C���ԍ���������	*/
	m_dwLineNo = 0;

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_PROC);}

	/* HEPA ��~	*/
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stopping HEPA..."));
	char l_cHepaAlLevel;
	if (TRUE != ((CChiefView*) m_pcChiefView)->HepaOnOff(FALSE, &l_cHepaAlLevel)) {
		/* ��~�ُ�ł����D	*/
		AlarmIf_Set(ALID_HepaStopFailed);
		if ('W' != l_cHepaAlLevel) {
			/* �A���[�����x�����x���ȊO�������̂ŏI�����܂��D	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to stop HEPA(alarm)."));
			/* �g�����W�V�����X�V	*/
			this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_HEPASTP_FAIL);
			return;
		} else {
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to stop HEPA(warning)."));
		}
	}

	/* �X�g���X �V�[�P���X����J�n����ʃ��W���[���֒ʒm	*/
	SEQ_MEAS_START l_SeqMeasStart;
	if ((0x0d == ((CChiefView*) m_pcChiefView)->m_szSampleID[0]) && (0x00 == ((CChiefView*) m_pcChiefView)->m_szSampleID[1])) {
		l_SeqMeasStart.bGetSamplID = FALSE;
	} else {
		l_SeqMeasStart.bGetSamplID = TRUE;
	}
	_tcscpy(l_SeqMeasStart.szSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
	COPYDATASTRUCT l_CopyData;
	l_CopyData.dwData = WMCD_CHIF_MEAS_START;
	l_CopyData.cbData = sizeof(SEQ_MEAS_START);
	l_CopyData.lpData = &l_SeqMeasStart;
	if (TRANSI_MASTER == m_dwParentTransi) {
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(48) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WMCD_CHIF_MEAS_START to Data module."));
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(48) ---------- } ---------- */
		((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
	}

	/* �T���v�� ID ���擾	*/
	TCHAR l_tszSampleID[SAMPLE_ID_LEN + 1];
	if (TRUE == l_SeqMeasStart.bGetSamplID) {
		_tcscpy(l_tszSampleID, ((CChiefView*) m_pcChiefView)->m_szSampleID);
	} else {
		//2009.08.12 bagus stress --{--
		//((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView))->GetSampleId(l_tszSampleID);
		((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetSampleId(l_tszSampleID);
		//2009.08.12 bagus stress --}--
	}
	/* ���胂�[�h����	*/
	/*		[�T���v�� ID].dat �̗L�����m�F���Ĕ��肵�܂��D	*/
	TCHAR l_RefFilePath[_MAX_PATH];
	_stprintf(l_RefFilePath, _T("%s%s.dat"), g_szData_StressRef_Dir, l_tszSampleID);
	m_iMeasMode = 1;	/* ������	*/
	if (-1 == ::GetFileAttributes(l_RefFilePath)) {
		m_iMeasMode = 0;	/* (�t�@�C�������������̂�)���t�@�����X����	*/
	}
	/* �T���v�� ID �𑪒胂�W���[���֒ʒm	*/
	// 2009.08.18 K.Matsuo �X�g���X�Ή� -->
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo;
	LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo;
// 2009.10.06 K.Matsuo -->
	LPSTAGE_PROG_INFO_HDR l_pStageProgInfoHdr = (LPSTAGE_PROG_INFO_HDR) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgInfoHdr;
	LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
//Saiki 20110308 Change ----->
//	BOOL l_bRet = MEAS_PrepareForStressMeasure(m_iMeasMode, l_tszSampleID, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgStress);
//	BOOL l_bRet = MEAS_PrepareForStressMeasure(m_iMeasMode, l_tszSampleID, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr, l_pStageProgStress);
	LPSTRESS_PARAM_INFO l_pStressParamInfo = (LPSTRESS_PARAM_INFO) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStressParamInfo;
	BOOL l_bRet = MEAS_PrepareForStressMeasure(m_iMeasMode, l_tszSampleID, l_pMainRcpInfo, l_pMeasProgInfo, l_pStageProgInfoHdr, l_pStageProgStress, l_pStressParamInfo);
//Saiki 20110308 Change <-----
// 2009.10.06 K.Matsuo <--
	// 2009.08.18 K.Matsuo �X�g���X�Ή� <--
	if (TRUE != l_bRet) {
		/* ���胂�W���[������ُ킪�Ԃ�܂����D	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_MEASPRE_FAIL);
	} else {
		if (0 == m_iMeasMode) {
			((CChiefView*) m_pcChiefView)->LogChief(_T("Start reference mode."));
			this->TransiEvent(EV_STRS_MODE_REF);	/* ���t�@�����X����	*/
		} else {
//Saiki 20110308 Change ----->
//			((CChiefView*) m_pcChiefView)->LogChief(_T("Start stress mode."));
//			this->TransiEvent(EV_STRS_MODE_MEAS);	/* ������	*/
			if(TRUE == l_pStressParamInfo->bPriority){
				((CChiefView*) m_pcChiefView)->LogChief(_T("Start stress fixed mode."));
				this->TransiEvent(EV_STRS_MODE_FIX_MEAS);	/* ������(�Œ�l)	*/
			}
			else{
				((CChiefView*) m_pcChiefView)->LogChief(_T("Start stress actual mode."));
				this->TransiEvent(EV_STRS_MODE_MEAS);	/* ������(�����l)	*/
			}
//Saiki 20110308 Change <-----
		}
	}
}

/*
 * SR ����V�[�P���X�J�n & �����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_SRMEAS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[3], (DWORD) pEvParams);

	if (1 == m_PauseStat.dwSrMeasuring) {
		/* ���蒆�ŁC�|�[�Y���畜�A�̂��߁D*/
		m_PauseStat.dwSrMeasuring = 0;
		/* WAIT ��Ԃɂ��邽�߃g�����W�V�������X�V���܂���D	*/
		return;
	}

	/* SR �V�[�P���X���� �J�n	*/
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
//	if (false == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->IsIdle()) {
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(6, 9)) {
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
		/* SEQ �g�����W�V�������r�W�[�ł����D	*/
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
//		((CChiefView*) m_pcChiefView)->LogChief(_T("SR Seq is busy."));
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ----------              */
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiStress::enter_ST_STRS_SRMEAS() - SR Seq is busy."));
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
		/* �g�����W�V�����X�V	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_TRSEQ_BUSY);
	} else {
		/* �V�[�P���X���� ���s	*/
/* modified 2009.09.09 hmenjo �����ύX ---------- { ---------- */
//		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(TRANSI_STRESS_SEQ, pEvParams));
/* modified 2009.09.09 hmenjo �����ύX ----------			   */
		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_DO_MEAS, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_STRESS_SEQ, 0), pEvParams));
/* modified 2009.09.09 hmenjo �����ύX ---------- } ---------- */
		/* WAIT ��Ԃɂ��邽�߃g�����W�V�������X�V���܂���D	*/
	}
}

/*
 * SR ����V�[�P���X��������(WAIT ��Ԗ���)
 */
void CChiefTransiStress::enter_ST_STRS_SRMEAS_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[4], (DWORD) pEvParams);

	/* SR �V�[�P���X���� ���ʊm�F	*/
	if (TR_SEQ_OK != (DWORD) pEvParams) {
		/* �ُ�I���Ȃ̂ŃL�����Z�������ɂ��܂��D	*/
		((CChiefView*) m_pcChiefView)->LogChief(_T("SR seq is abnormal end."));
		/* �g�����W�V�����X�V	*/
		this->TransiEvent(EV_STRS_CANCEL, pEvParams);
	} else if (0 != m_bCancel) {
		/* �L�����Z���w��	*/
		this->TransiEvent(EV_STRS_CANCEL);
	} else {
		/* �g�����W�V�����X�V	*/
		this->TransiEvent(EV_STRS_MODE_MEAS);
	}
}

/*
 * ����O ���[�h Pos �����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_PRELDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[5], (DWORD) pEvParams);

	if (1 == m_PauseStat.dwPreLoadingPos) {
		/* �ړ����ŁC�|�[�Y���畜�A�̂��߁D*/
		m_PauseStat.dwPreLoadingPos = 0;
		/* WAIT ��Ԃɂ��邽�߃g�����W�V�������X�V���܂���D	*/
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- } ---------- */
		return;
	}

	/* ���[�h Pos �ֈړ� -------------------------------------- */
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to LD Pos. (before-meas)"));
	if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
		/* HWS �ł��D	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		::Sleep(STRS_HWS_TIME);
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else
	if (0 == this->MoveToLdPos()) {
		/* �ړ��J�n���s	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LoadPositionMoveStartFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to LD Pos. (before-meas)"));
		/* �L�����Z�����|�X�g	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_HLDPOSMOVE_FAIL);
	} else {
		/* �ړ��J�n����	*/
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 72;	/* �n���h�� ���[�h �|�W�V���� �ړ���	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* ���쒆�t���O(XY �X�e�[�W)���I��	*/
		/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
	}
}

/*
 * �s�� �A���C�����g�҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_PINALIGN(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[6], (DWORD) pEvParams);

/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- { ---------- */
	/* Stress �w�b�h�ɂ��܂��D	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_STRESS, FALSE);
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- } ---------- */

	if (1 == m_PauseStat.dwPinAligning) {
		/* ���쒆�ŁC�|�[�Y���畜�A�̂��߁D*/
		m_PauseStat.dwPinAligning = 0;
		/* WAIT ��Ԃɂ��邽�߃g�����W�V�������X�V���܂���D	*/
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_LIFT_ALIGN);	// Status Bar
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- } ---------- */
		return;
	}

	/* Z �������O�ʒu�ֈړ�	*/
	((CChiefView*) m_pcChiefView)->LogChief(_T("Retracting Z axis..."));
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		/* ���쒆�t���O(Z ��)���I��	*/
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_Z_MOVING);	// Status Bar
/* modified 2009.09.29 hmenjo Stress Z ���X�L�����ʒu ---------- { ---------- */
//	if ((0 == StageElevatorMove(Z_RETRACT_POS)) && (0 == ((CChiefView*) m_pcChiefView)->IsHWS())) {
/* modified 2009.09.29 hmenjo Stress Z ���X�L�����ʒu ----------			  */
	STRESS_CONFIG l_StressConfig;
	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	if ((0 == StageElevatorMove(l_StressConfig.lScanZAxisPosition)) && (0 == ((CChiefView*) m_pcChiefView)->IsHWS())) {
/* modified 2009.09.29 hmenjo Stress Z ���X�L�����ʒu ---------- } ---------- */
		/* �ړ��J�n���s	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		/* ���쒆�t���O(Z ��)���I�t	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_AxisMoveFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to Z retract position."));
		/* �L�����Z�����|�X�g	*/
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_ZMOVE_FAIL);
	} else {
		if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
			/* HWS �ł��D	*/
			::Sleep(STRS_HWS_TIME);
		}
		/* �ړ�����	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		/* ���쒆�t���O(Z ��)���I�t	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_Z_MOVED);	// Status Bar
		((CChiefView*) m_pcChiefView)->LogChief(_T("Moved to Z retract position."));
		/* �s�����A���C�����g�ʒu�ֈړ� ----------------------------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_LIFT_ALIGN);	// Status Bar
		((CChiefView*) m_pcChiefView)->LogChief(_T("Moving Lifter to alignment pos..."));
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
			/* ���[�h�|�W�V�����ł͂Ȃ������D	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Not LD position."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
			/* �L�����Z�����|�X�g	*/
			this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_NO_HLDPOS);
		} else {
			BOOL l_bNexResult;
			if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
				/* HWS �ł��D	*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
				m_bPinAligning = TRUE;
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
				::Sleep(STRS_HWS_TIME);
				this->TransiEvent(EV_STRS_PIN_DONE);
			} else
			if (0 == (l_bNexResult = nexifMoveToAlignment(m_pcChiefView->m_hWnd))) {
				/* ���s	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Lifter to alignment pos."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
				/* �L�����Z�����|�X�g	*/
				this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) TR_STRS_PINMOVE_FAIL);
			} else {
				/* ����	*/
				((CChiefView*) m_pcChiefView)->m_dwPinMoveState = 71;	/* �s���ړ���	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
				/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
			}
		}
	}
}

/*
 * ���C������J�n�ʒu �ړ� & �����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_LMVSTAPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[7], (DWORD) pEvParams);

	BOOL l_bCancelReq = FALSE;
	DWORD l_dwErrCode = TR_STRS_OK;

	m_bMeasStress = TRUE;	/* �X�g���X���蒆	*/

	/* ���C������ړ����� �C�x���g �`�F�b�N	*/
	if ((1 == m_dwLineMoveToStartPos) && (EV_STRS_LINEMOVE_DONE == LOWORD(pEvParams))) {
		m_dwLineMoveToStartPos = 2;		/* �ړ���	*/
		if (TRUE == m_bPreMoving) {
			m_bPreMoving = FALSE;
			/* ���C���ԍ����X�V	*/
			m_dwLineNo++;
			((CChiefView*) m_pcChiefView)->GetNextLineNo(&m_dwLineNo);
		}
	}

	if (0 == m_dwLineMoveToStartPos) {
		if (TRUE == m_bPause) {
			/* �|�[�Y�w�ߗL�����D	*/
			m_iPausePreState = ST_STRS_LMVSTAPOS;
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
//			this->TransiEvent(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ----------			   */
			Event(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */
			return;
		}
		/* ���C������J�n�ʒu�ֈړ�	*/
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//		if (0 == nexioIsPinAlignmentPos()) {
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ----------			   */
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinAlign) {
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
			/* �s�����A���C�����g�ʒu�łȂ������D	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Not Lifter is alignment position."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterLowestPositionSensorOff));
			/* �L�����Z�����|�X�g	*/
			l_bCancelReq = TRUE;
			l_dwErrCode = TR_STRS_NO_PIN_ALIGN;
		} else if (0 == nexioIsRetractPosZ()) {
			/* �X�g���X�p Z ���ʒu�łȂ������D	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Z retract position is Invalid."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_PositionRetractFailed));
			/* �L�����Z�����|�X�g	*/
			l_bCancelReq = TRUE;
			l_dwErrCode = TR_STRS_NO_ZPOS;
		} else {
			NS_RestoreStageSpeed();	/* ���x��߂��܂��D	*/
			/* ���C���ԍ����X�V	*/
			m_dwLineNo++;
			((CChiefView*) m_pcChiefView)->GetNextLineNo(&m_dwLineNo);
			/* ���C�����W��Ǐo��	*/
			STAGE_COORD l_SetPosition;
			STRESS_CONFIG l_StressConfig;
			ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
			l_SetPosition.lX = l_StressConfig.lLineStartPosX;		/* ���C������J�n���W X	*/
			LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- { ---------- */
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ---------- { ---------- */
////			l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].LineSec.SectPos[0].lScanPosY;	/* ���C������J�n���W Y	*/
////																			/*	�Z�N�V�����P�� Y ���W���g���܂��D	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ----------				*/
//			l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[0].lScanPosY;	/* ���C������J�n���W Y	*/
//																			/*	�Z�N�V�����P�� Y ���W���g���܂��D	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ---------- } ---------- */
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ----------			   */
			for (int i = 0; i < static_cast<int>(l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum); i++) {
				if (0 != l_pStageProgStress->Line[m_dwLineNo - 1].bScanValid[i]) {
					break;
				}
			}
			if (static_cast<int>(l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum) <= i) {
				TCHAR l_tszLog[128];
				_stprintf(l_tszLog, _T("No Sections in Line No.%d."), m_dwLineNo);
				((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
				/* �L�����Z�����|�X�g	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_NOSECTION;
			} else {
			l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[i].lScanPosY;	/* ���C������J�n���W Y	*/
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- } ---------- */
			TCHAR l_tszLogText[256];
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- { ---------- */
//			_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d start position."), m_dwLineNo);
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ----------			   */
			_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d start position. [%10d, %10d]"), m_dwLineNo, l_SetPosition.lX, l_SetPosition.lY);
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- } ---------- */
			((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
			TCHAR l_szMainRcpName[256];
			((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
			if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
				/* HWS �ł��D	*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
				CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_STAPOS_MOVING, m_dwLineNo);
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
				::Sleep(STRS_HWS_TIME);
				m_dwLineMoveToStartPos = 1;		/* �ړ���	*/
				this->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
			} else
			// 2009.08.27 K.Matsuo -->
			if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_SetPosition, l_szMainRcpName, HEAD_TYPE_STRESS)) {
			// 2009.08.27 K.Matsuo <--
				/* �ړ��J�n���s	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to line start position."));
				/* �L�����Z�����|�X�g	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_LINEMOVE_FAIL;
			} else {
				/* �ړ��J�n����	*/
				m_dwLineMoveToStartPos = 1;		/* �ړ���	*/
				((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 73;	/* ���C������ �ړ���	*/
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_STAPOS_MOVING);	// Status Bar
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ----------			   */
				CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_STAPOS_MOVING, m_dwLineNo);
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* ���쒆�t���O(XY �X�e�[�W)���I��	*/
				/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
			}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- { ---------- */
			}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- } ---------- */
		}
	} else if ((0 != m_bHepaStopped) && (2 == m_dwLineMoveToStartPos)) {
		/* HEPA ��~�� �� �ړ���	*/
		m_dwLineMoveToStartPos = 0;
		/* �g�����W�V�����X�V	*/
		this->TransiEvent(EV_STRS_TO_LMENDPOS);
	} else {
		/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
	}

	/* �L�����Z������(����΁E�E�E)	*/
	if (TRUE == l_bCancelReq) {
		m_dwLineMoveToStartPos = 0;
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) l_dwErrCode);
	}
}

/*
 * ���C������I���ʒu �ړ� & �����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_LMVENDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[8], (DWORD) pEvParams);

	BOOL l_bCancelReq = FALSE;
	DWORD l_dwErrCode = TR_STRS_OK;

	static BOOL ls_bMeasEnd = FALSE;	/* �P���C�����芮��(���胂�W���[�����)	*/
	static BOOL ls_AnaEndReply = FALSE;		/* �P���C���f�[�^������������(�f�[�^�������W���[������)	*/

	/* �C�x���g �`�F�b�N	*/
	switch (LOWORD(pEvParams)) {
	case EV_STRS_LINEMOVE_DONE:		/* ���C������ړ�����	*/
		if (1 == m_dwLineMoveToEndPos) {
			m_dwLineMoveToEndPos = 2;		/* �ړ���	*/
		}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- { ---------- */
		if ((1 == m_dwLineMoveToStartPos) && (TRUE == m_bPreMoving)) {
			/* ��s�ړ��̊���	*/
			m_dwLineMoveToStartPos = 2;
		}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- } ---------- */
		break;
	case EV_STRS_LINEMEAS_DONE:		/* �P���C�����芮��	*/
		ls_bMeasEnd = TRUE;
		if (0 != HIWORD(pEvParams)) {
			/* �ُ�ʒm�ł����D	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Measurement was failure.."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementError));
			l_bCancelReq = TRUE;
			l_dwErrCode = TR_STRS_MEASEND_ERR;
		} else {
			/* �u�f�[�^�������W���[���v�ւP���C�����芮����ʒm���܂��D	*/
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_LINE_END, m_dwLineNo, m_iMeasMode);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_LINE_END to Data module."));
		}
		break;
	case EV_STRS_LINEDATA_DONE:		/* �P���C���f�[�^������������	*/
		ls_AnaEndReply = TRUE;
		break;
	default:
		;	/* �������܂���D	*/
		break;
	}

	if (TRUE != l_bCancelReq) {
		if (0 == m_dwLineMoveToEndPos) {
			if (TRUE == m_bPause) {
				/* �|�[�Y�w�ߗL�����D	*/
				m_iPausePreState = ST_STRS_LMVENDPOS;
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- { ---------- */
//				this->TransiEvent(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ----------			   */
				Event(EV_STRS_PAUSE);
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(49) ---------- } ---------- */
				return;
			}
			/* ���C������J�n�ʒu�ֈړ�	*/
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//			if (0 == nexioIsPinAlignmentPos()) {
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ----------			   */
			if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinAlign) {
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
				/* �s�����A���C�����g�ʒu�łȂ������D	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Not Lifter is alignment position."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterLowestPositionSensorOff));
				/* �L�����Z�����|�X�g	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_NO_PIN_ALIGN;
			} else if (0 == nexioIsRetractPosZ()) {
				/* �X�g���X�p Z ���ʒu�łȂ������D	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Z retract position is Invalid."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_Z_PositionRetractFailed));
				/* �L�����Z�����|�X�g	*/
				l_bCancelReq = TRUE;
				l_dwErrCode = TR_STRS_NO_ZPOS;
			} else {
				/* �u�f�[�^�������W���[���v�փ��C������J�n��ʒm���܂��D	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Start Measurement."));
				((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_LINE_START, m_dwLineNo, m_iMeasMode);
				/* �u���胂�W���[���v�փ��C������J�n��ʒm���܂��D	*/
				BOOL l_bRet = MEAS_StressMeasure(m_dwLineNo);
				if (TRUE != l_bRet) {
					/* �u���胂�W���[���v�J�n�G���[�ł����D	*/
					((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to run MEAS_StressMeasure()."));
					((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementError));
					/* �L�����Z�����|�X�g	*/
					l_bCancelReq = TRUE;
					l_dwErrCode = TR_STRS_MEASSTA_FAIL;
				} else {
					/* ���C�����W��Ǐo��	*/
					STAGE_COORD l_SetPosition;
					STRESS_CONFIG l_StressConfig;
					ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
					l_SetPosition.lX = l_StressConfig.lLineEndPosX;		/* ���C������I�����W X	*/
					/* ���葬�x �ݒ� [mm/s]=[um/ms]	*/
					NS_SetStageSpeed(l_StressConfig.dwScanSpeed);
					LPSTAGE_PROG_STRESS l_pStageProgStress = (LPSTAGE_PROG_STRESS) ((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pStageProgStress;
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- { ---------- */
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ---------- { ---------- */
////					l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].LineSec.SectPos[0].lScanPosY;	/* ���C������J�n���W Y	*/
////																					/*	�Z�N�V�����P�� Y ���W���g���܂��D	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ----------				*/
//					l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[0].lScanPosY;	/* ���C������J�n���W Y	*/
//																					/*	�Z�N�V�����P�� Y ���W���g���܂��D	*/
///* modified 2009.08.18 hmenjo STRESS_LINESECTION �폜 ---------- } ---------- */
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ----------			   */
					for (int i = 0; i < static_cast<int>(l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum); i++) {
						if (0 != l_pStageProgStress->Line[m_dwLineNo - 1].bScanValid[i]) {
							break;
						}
					}
					if (static_cast<int>(l_StressConfig.Line[m_dwLineNo - 1].dwSectionNum) <= i) {
						TCHAR l_tszLog[128];
						_stprintf(l_tszLog, _T("No Sections in Line No.%d."), m_dwLineNo);
						((CChiefView*) m_pcChiefView)->LogChief(l_tszLog);
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
						/* �L�����Z�����|�X�g	*/
						l_bCancelReq = TRUE;
						l_dwErrCode = TR_STRS_NOSECTION;
					} else {
					l_SetPosition.lY = l_pStageProgStress->Line[m_dwLineNo - 1].SectPos[i].lScanPosY;	/* ���C������J�n���W Y	*/
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- } ---------- */
					TCHAR l_tszLogText[256];
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- { ---------- */
//					_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d end position."), m_dwLineNo);
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ----------			   */
					_stprintf(l_tszLogText, _T("Stage is moving to Line No.%3d end position. [%10d, %10d]"), m_dwLineNo, l_SetPosition.lX, l_SetPosition.lY);
/* modified 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- } ---------- */
					((CChiefView*) m_pcChiefView)->LogChief(l_tszLogText);
					TCHAR l_szMainRcpName[256];
					((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
					if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
						/* HWS �ł��D	*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
						CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_ENDPOS_MOVING, m_dwLineNo);
						((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
						::Sleep(STRS_HWS_TIME);
						m_dwLineMoveToEndPos = 1;		/* �ړ���	*/
						this->TransiEvent(EV_STRS_LINEMOVE_DONE, (cEventParams*) EV_STRS_LINEMOVE_DONE);
					} else
					// 2009.08.27 K.Matsuo -->
					if (0 == NS_SampleCoordStageMoveAbsoluteEx(&l_SetPosition, l_szMainRcpName, HEAD_TYPE_STRESS)) {
					// 2009.08.27 K.Matsuo <--
						/* �ړ��J�n���s	*/
						((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
						((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_MeasurementPointMoveFailed));
						((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to line end position."));
						/* �L�����Z�����|�X�g	*/
						l_bCancelReq = TRUE;
						l_dwErrCode = TR_STRS_LINEMOVE_FAIL;
					} else {
						/* �ړ��J�n����	*/
						m_dwLineMoveToEndPos = 1;		/* �ړ���	*/
						((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 73;	/* ���C������ �ړ���	*/
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//						((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_ENDPOS_MOVING);	// Status Bar
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ----------			   */
						CString l_strStatBar; l_strStatBar.Format(IDS_CHIF_STRS_ENDPOS_MOVING, m_dwLineNo);
						((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(l_strStatBar);  // Status Bar
/* modified 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
						((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* ���쒆�t���O(XY �X�e�[�W)���I��	*/
						/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
					}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- { ---------- */
					}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(51) ---------- } ---------- */
				}
			}
		} else if ((2 == m_dwLineMoveToEndPos) && (TRUE == ls_bMeasEnd) && (TRUE == ls_AnaEndReply)) {
			/* �ړ��� �� ����� �� �f�[�^������	*/
			m_dwLineMoveToEndPos = 0;
			ls_bMeasEnd = FALSE;
			ls_AnaEndReply = FALSE;
			STRESS_CONFIG l_StressConfig;
			ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
			DWORD l_dwLineNo = m_dwLineNo + 1;
			if ((l_StressConfig.dwLiftPinNumberOfLine <= m_dwLineNo)
			 || (l_StressConfig.dwLiftPinNumberOfLine < ((CChiefView*) m_pcChiefView)->GetNextLineNo(&l_dwLineNo))) {
				/* �ŏI���C���ԍ��������̂őS�I��	*/
				/* �g�����W�V�����X�V	*/
				NS_RestoreStageSpeed();	/* ���x��߂��܂��D	*/
				this->TransiEvent(EV_STRS_LINE_ALLDONE);
			} else {
//				/* �|�[�Y�`�F�b�N	*/
//				if (0) {	/* �|�[�Y�n�̃f�o�b�O�I����C�{���ɕs�v�Ȃ�폜���邱�ƁD	*/
//					/**/
//				} else {
					/* ���̃��C����	*/
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- { ---------- */
					if ((2 == m_dwLineMoveToStartPos) && (TRUE == m_bPreMoving)) {
						m_bPreMoving = FALSE;
						/* ���C���ԍ����X�V	*/
						m_dwLineNo++;
						((CChiefView*) m_pcChiefView)->GetNextLineNo(&m_dwLineNo);
					}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(53) ---------- } ---------- */
					/* �g�����W�V�����X�V	*/
					this->TransiEvent(EV_STRS_LINE_NEXT);
//				}
			}
		} else {
			/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
		}
	}

	/* �L�����Z������(����΁E�E�E)	*/
	if (TRUE == l_bCancelReq) {
		m_dwLineMoveToEndPos = 0;
		ls_bMeasEnd = FALSE;
		ls_AnaEndReply = FALSE;
		this->TransiEvent(EV_STRS_CANCEL, (cEventParams*) l_dwErrCode);
	}
}

/*
 * ������ ���[�h Pos �����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_COMPLDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[9], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->SentMeasPause(0, TRUE);	// �|�[�Y�����ʒm

	m_bMeasStress = FALSE;	/* �X�g���X���蒆 ����	*/

// 2009.08.22 K.Matsuo -->
	/* �C�x���g�`�F�b�N	*/
	switch (LOWORD(pEvParams)) {
	case EV_STRS_COMPLETE:	/* �P�����芮�� ����	*/
		m_bCompleted = TRUE;
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROC_COMPLETE);	// Status Bar
//		break;
		return;
	default:
		;	/* �������܂���D	*/
		break;
	}
// 2009.08.22 K.Matsuo <--

	if (TRUE != m_bCompleted) {
		/* �u�f�[�^�������W���[���v�ւP�����芮����ʒm	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_STRS_MEAS_END, 0, m_iMeasMode);
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROCING);  // Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_STRS_MEAS_END to Data module."));
	}

	/* ���[�h Pos �ֈړ� -------------------------------------- */
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to LD Pos. (after-meas)"));
	if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
		/* HWS �ł��D	*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
		::Sleep(STRS_HWS_TIME);
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else
	if (0 == this->MoveToLdPos()) {
		/* �ړ��J�n���s	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LoadPositionMoveStartFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to LD Pos. (after-meas)"));
		/* ���̃g�����W�V������(���[�h Pos �ֈړ�������)	*/
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else {
		/* �ړ��J�n����	*/
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 72;	/* �n���h�� ���[�h �|�W�V���� �ړ���	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* ���쒆�t���O(XY �X�e�[�W)���I��	*/
		/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
	}
}

/*
 * ������ �s�� �_�E�� & Complete �҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_PINDOWNCOMP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[10], (DWORD) pEvParams);

	static BOOL ls_bPinDown = FALSE;	/* �s���_�E��	*/

	/* �C�x���g�`�F�b�N	*/
	switch (LOWORD(pEvParams)) {
	case EV_STRS_COMPLETE:	/* �P�����芮�� ����	*/
		m_bCompleted = TRUE;
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_DATA_PROC_COMPLETE);	// Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
		break;
	case EV_STRS_PIN_DONE:	/* �s���_�E������	*/
/* added 2009.10.19 hmenjo Stress �g�����W�V�����e�[�u���ǉ� ---------- { ---------- */
	case TR_STRS_PINMOVE_FAIL:	/* �s�����s	*/
/* added 2009.10.19 hmenjo Stress �g�����W�V�����e�[�u���ǉ� ---------- } ---------- */
		ls_bPinDown = TRUE;
		break;
	default:
		;	/* �������܂���D	*/
		break;
	}

	if ((TRUE == ls_bPinDown) && (TRUE == m_bCompleted)) {
		/* �s���_�E�� �� �P�����芮�� ����	*/
		m_bCompleted = FALSE;
		ls_bPinDown = FALSE;
		/* �g�����W�V�����X�V	*/
		this->TransiEvent(EV_STRS_PINCMP_DONE);
	} else if (TRUE != ls_bPinDown) {
		/* �s���_�E���łȂ��̂ŁC�s�������~�[�ֈړ�	*/
		if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
			/* ���[�h�|�W�V�����ł͂Ȃ������D	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Not LD position."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
			/* �s���_�E���o���Ȃ��̂ŁC���̃g�����W�V������(�s���_�E��������)	*/
			this->TransiEvent(EV_STRS_PINCMP_DONE);
		} else {
			BOOL l_bNexResult;
			if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
				/* HWS �ł��D	*/
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_STRS_LIFT_LOWER);  // Status Bar
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
				::Sleep(STRS_HWS_TIME);
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
				m_bPinAligning = FALSE;
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
				this->TransiEvent(EV_STRS_PINCMP_DONE);
			} else
/* added 2009.10.19 hmenjo ���~�[�ł̓s���_�E�����Ȃ� ---------- { ---------- */
			if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinDown) {
				/* �s�������łɉ��~�[�Ȃ̂Ŏ��̃g�����W�V������	*/
				this->TransiEvent(EV_STRS_PINCMP_DONE);
			} else
/* added 2009.10.19 hmenjo ���~�[�ł̓s���_�E�����Ȃ� ---------- { ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad �ɕύX ---------- { ---------- */
//			if (0 == (l_bNexResult = nexifMoveToLower(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad �ɕύX ----------			  */
			if (0 == (l_bNexResult = nexifLoad(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad �ɕύX ---------- } ---------- */
				/* ���s	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Lifter to lower pos."));
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
				/* ���̃g�����W�V������(�s���_�E��������)	*/
				this->TransiEvent(EV_STRS_PINCMP_DONE);
			} else {
				/* ����	*/
				((CChiefView*) m_pcChiefView)->m_dwPinMoveState = 71;	/* �s���ړ���	*/
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
				/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
			}
		}
	} else {
		/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
	}
}

/*
 * Complete ���ԑ҂�(WAIT ��Ԗ���)
 */
void CChiefTransiStress::enter_ST_STRS_COMPLETEDELAY(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[11], (DWORD) pEvParams);

/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- { ---------- */
	/* SR �w�b�h�ɖ߂��܂��D	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- } ---------- */

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_COMP);}

	/* HEPA �N��	*/
	((CChiefView*) m_pcChiefView)->HepaOnOff(TRUE);

	/* ���ԑ҂�	*/
	::Sleep(TIMER_DELAY_COMPLETE);

/* added 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
/* added 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */

	/* �g�����W�V�����X�V	*/
	TransiEvent(EV_STRS_COMPLETE_DONE);

	/* �ďo���g�����W�V�����Ɋ�����ʒm	*/
	((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, TR_STRS_OK);
}

/*
 * Abort ��~�҂�(WAIT ��Ԗ���)
 */
void CChiefTransiStress::enter_ST_STRS_WAITABORT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[12], (DWORD) pEvParams);

	if (TRANSI_MASTER == m_dwParentTransi) {((CChiefView*) m_pcChiefView)->ProcStatusSet(PROCESS_ABRT);}

	m_iCancelStat = (DWORD) pEvParams;

	NS_RestoreStageSpeed();	/* ���x��߂��܂��D	*/

	m_bCancel = FALSE;

	/* �|�[�Y�����ʒm	*/
	((CChiefView*) m_pcChiefView)->SentMeasPause(0, TRUE);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_CANCELING);	// Status Bar

	/* ���胂�W���[���ɃL�����Z����ʒm	*/
	((CChiefView*) m_pcChiefView)->LogChief(_T("Cancel Measurement."));
	MEAS_StressCancel();

	/* �V�[�P���X���� �g�����W�V�������~	*/
	BOOL l_bNeedStop = FALSE;
/* modified 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//	if (false == ((CChiefTransiStress*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress))->IsIdle()) {
//		/* �P�|�C���g���� �g�����W�V���������쒆	*/
//		l_bNeedStop = TRUE;
//		/* �I��������	*/
//		((CChiefTransiStress*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress))->TransiEvent(EV_STRS_CANCEL);
//	}
/* modified 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ----------			   */
	if (false == ((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->IsIdle()) {
		/* SR �V�[�P���X���� �g�����W�V���������쒆	*/
		l_bNeedStop = TRUE;
		/* �I��������	*/
		((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->TransiEvent(EV_SEQ_CANCEL);
	}
/* modified 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */

	/* �������쒆�̏ꍇ�̓R�R�Œ�~�����܂��D	*/
	if (0 == NS_IsIdleStageMoveAsync()) {
		l_bNeedStop = TRUE;	/*hdebdeb20090824*/
		StageStop();	/* �߂�l�͖������Ă��܂��D	*/
	}

	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/

	if (0 != l_bNeedStop) {
		/* �ȉ��̂��ׂẴg�����W�V�������A�C�h���ɂȂ�܂ő҂��܂��D	*/
		while (
/* modified 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
//			(false == ((CChiefTransiStress*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiStress))->IsIdle())
/* modified 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ----------			   */
			(false == ((CChiefTransiSeq*) (((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq))->IsIdle())
		 || (0 == NS_IsIdleStageMoveAsync())	/*hdebdeb20090824*/
/* modified 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */
			) {
			::Sleep(1);
		}
	}

	m_bMeasStress = FALSE;	/* �X�g���X���蒆 ����	*/

	::Sleep(2000);	/* �X�e�[�W��~�҂���҂��Ă݂�	*/

	/* �P������I���� �f�[�^���� �֒ʒm(�ُ�)	*/
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_CHIF_STRS_MEAS_END, 1, m_iMeasMode);
	((CChiefView*) m_pcChiefView)->LogChief(_T("Notify WM_CHIF_STRS_MEAS_END with NG(or Cancel)."));

	/* �g�����W�V�����X�V	*/
	TransiEvent(EV_STRS_CANCEL_DONE);
}

/*
 * Abort �� ���[�h Pos �����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_ABORTLDPOS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[13], (DWORD) pEvParams);

	/* ���[�h Pos �ֈړ� -------------------------------------- */
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to LD Pos. (abort)"));
	if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
		/* HWS �ł��D	*/
		::Sleep(STRS_HWS_TIME);
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else
	if (0 == this->MoveToLdPos()) {
		/* �ړ��J�n���s	*/
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);		/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LoadPositionMoveStartFailed));
		((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move to LD Pos. (abort)"));
		/* ���̃g�����W�V������(���[�h Pos �ֈړ�������)	*/
		this->TransiEvent(EV_STRS_LDPOS_DONE);
	} else {
		/* �ړ��J�n����	*/
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 72;	/* �n���h�� ���[�h �|�W�V���� �ړ���	*/
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_HLDPOS_MOVING);  // Status Bar
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		/* ���쒆�t���O(XY �X�e�[�W)���I��	*/
		/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
	}
}

/*
 * Abort �� �s�� �_�E���҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_ABORTPINDOWN(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[14], (DWORD) pEvParams);

	/* �s�������~�[�ֈړ�	*/
	if (0 == ((CChiefView*) m_pcChiefView)->m_DiInfo.bLoadPos) {
		/* ���[�h�|�W�V�����ł͂Ȃ������D	*/
		((CChiefView*) m_pcChiefView)->LogChief(_T("Not LD position."));
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
		/* �s���_�E���o���Ȃ��̂ŁC���̃g�����W�V������(�s���_�E��������)	*/
		this->TransiEvent(EV_STRS_PIN_DONE);
	} else {
		BOOL l_bNexResult;
		if (0 != ((CChiefView*) m_pcChiefView)->IsHWS()) {
			/* HWS �ł��D	*/
			::Sleep(STRS_HWS_TIME);
			this->TransiEvent(EV_STRS_PIN_DONE);
		} else
/* added 2009.10.19 hmenjo ���~�[�ł̓s���_�E�����Ȃ� ---------- { ---------- */
			if (0 != ((CChiefView*) m_pcChiefView)->m_DiInfo.bPinDown) {
				/* �s�������łɉ��~�[�Ȃ̂Ŏ��̃g�����W�V������	*/
				this->TransiEvent(EV_STRS_PIN_DONE);
			} else
/* added 2009.10.19 hmenjo ���~�[�ł̓s���_�E�����Ȃ� ---------- { ---------- */
/* modified 2009.10.06 hmenjo Stress nexifLoad �ɕύX ---------- { ---------- */
//		if (0 == (l_bNexResult = nexifMoveToLower(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad �ɕύX ----------			  */
		if (0 == (l_bNexResult = nexifLoad(m_pcChiefView->m_hWnd))) {
/* modified 2009.10.06 hmenjo Stress nexifLoad �ɕύX ---------- } ---------- */
			/* ���s	*/
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to move Lifter to lower pos."));
			((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(9, CHRAMSG_OK)), MAKELPARAM(0, ALID_LifterMoveStartFailed));
			/* ���̃g�����W�V������(�s���_�E��������)	*/
			this->TransiEvent(EV_STRS_PIN_DONE);
		} else {
			/* ����	*/
			((CChiefView*) m_pcChiefView)->m_dwPinMoveState = 71;	/* �s���ړ���	*/
			((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_PIN, TRUE);
			/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
		}
	}
}

/*
 * Abort ���ԑ҂�(WAIT ��Ԗ���)
 */
void CChiefTransiStress::enter_ST_STRS_ABORTDELAY(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[15], (DWORD) pEvParams);

/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- { ---------- */
	/* SR �w�b�h�ɖ߂��܂��D	*/
	((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- } ---------- */

	/* HEPA �N��	*/
	((CChiefView*) m_pcChiefView)->HepaOnOff(TRUE);

	/* ���ԑ҂�	*/
	::Sleep(TIMER_DELAY_ABORT);

	/* ����L�����Z�������� Pif �֕�	*/
	((CChiefView*) m_pcChiefView)->ReportCancelComplete(0);

/* added 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
/* added 2009.08.18 hmenjo �X�g���X�@�\�ǉ�(46) ---------- } ---------- */

	/* �g�����W�V�����X�V	*/
	TransiEvent(EV_STRS_ABORT_DONE);

	/* �ďo���g�����W�V�����Ɋ�����ʒm	*/
	((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_SEQ_DONE, (cEventParams*) m_iCancelStat);
}

/*
 * �|�[�Y�����҂�(WAIT ��Ԗ����̏ꍇ�L��)
 */
void CChiefTransiStress::enter_ST_STRS_PAUSE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[16], (DWORD) pEvParams);

	m_bPause = FALSE;

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SEQ_PAUSING);	// Status Bar

	switch (m_iPausePreState) {
	case ST_STRS_SRMEAS:
		switch (m_PauseStat.dwSrMeasuring) {
		case 0:	m_PauseStat.dwSrMeasuring = 1;	break;	/* ���蒆	*/
		case 1:	m_PauseStat.dwSrMeasuring = 2;			/* �����	*/
			switch ((DWORD) pEvParams) {
			case TR_SEQ_OK:	/* ���튮��	*/
				break;
			default:
				/* �ُ튮��	*/
				/* �L�����Z�����|�X�g	*/
				this->TransiEvent(EV_STRS_CANCEL, pEvParams);
				return;
				break;
			}
			break;
		default:
			break;
		}
		break;
	case ST_STRS_PRELDPOS:
		switch (m_PauseStat.dwPreLoadingPos) {
		case 0:	m_PauseStat.dwPreLoadingPos = 1;	break;	/* �ړ���	*/
		case 1:	m_PauseStat.dwPreLoadingPos = 2;	break;	/* �ړ���	*/
		default:	break;
		}
		break;
	case ST_STRS_PINALIGN:
		switch (m_PauseStat.dwPinAligning) {
		case 0:	m_PauseStat.dwPinAligning = 1;	break;	/* ���쒆	*/
		case 1:	m_PauseStat.dwPinAligning = 2;	break;	/* �����	*/
		default:	break;
		}
		break;
	case ST_STRS_LMVSTAPOS:
	case ST_STRS_LMVENDPOS:
	default:
		break;
	}

	/* WAIT ��Ԃɂ��邽�ߏ����͂���܂���D	*/
}

/*
 * ���W���[���O����(WAIT ��Ԗ���)
 */
void CChiefTransiStress::enter_ST_STRS_PRE_RESUME(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_STRS_DISP[17], (DWORD) pEvParams);

	m_bPause = FALSE;

	int l_iEventNo = 0;
	DWORD l_dwEvParam;
	switch (m_iPausePreState) {
	case ST_STRS_SRMEAS:
		if (1 == m_PauseStat.dwSrMeasuring) {
			l_iEventNo = EV_STRS_RSM_SRMEAS;	/* ���蒆�FST_STRS_SRMEAS ��		*/
			l_dwEvParam = 0;
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(50) ---------- { ---------- */
			if (ST_SEQ_PAUSE == ((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->GetCurrentState()) {
				/* �V�[�P���X���� �g�����W�V�����փ��W���[���w��	*/
				((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_RESUME);
			}
/* added 2009.08.21 hmenjo �X�g���X�@�\�ǉ�(50) ---------- } ---------- */
		} else {
			l_iEventNo = EV_STRS_RSM_SRMEASED;	/* ����ρFST_STRS_SRMEAS_DONE ��	*/
			l_dwEvParam = 0;
			m_PauseStat.dwSrMeasuring = 0;
		}
		break;
	case ST_STRS_PRELDPOS:
		if (1 == m_PauseStat.dwPreLoadingPos) {
			l_iEventNo = EV_STRS_RSM_PRELDPOS;	/* �ړ����FST_STRS_PRELDPOS ��	*/
			l_dwEvParam = 0;
		} else {
			l_iEventNo = EV_STRS_RSM_PINALIGN;	/* �ړ��ρFST_STRS_PINALIGN ��	*/
			l_dwEvParam = 0;
			m_PauseStat.dwPreLoadingPos = 0;
		}
		break;
	case ST_STRS_PINALIGN:
		if (1 == m_PauseStat.dwPinAligning) {
			l_iEventNo = EV_STRS_RSM_PINALIGN;	/* ���쒆�FST_STRS_PINALIGN ��	*/
			l_dwEvParam = 0;
		} else {
			l_iEventNo = EV_STRS_RSM_LMVSTAPOS;	/* ����ρFST_STRS_LMVSTAPOS ��	*/
			l_dwEvParam = 0;
			m_PauseStat.dwPinAligning = 0;
		}
		break;
	case ST_STRS_LMVSTAPOS:
		l_iEventNo = EV_STRS_RSM_LMVSTAPOS;		/* ST_STRS_LMVSTAPOS ��	*/
		l_dwEvParam = 0;
		break;
	case ST_STRS_LMVENDPOS:
		l_iEventNo = EV_STRS_RSM_LMVENDPOS;		/* ST_STRS_LMVENDPOS ��	*/
		l_dwEvParam = 0;
		break;
	}

	if (0 != l_iEventNo) {
		this->TransiEvent(l_iEventNo, (cEventParams*) l_dwEvParam);
	}
}
