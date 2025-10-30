// ChifTransiAF.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <Active.hxx>
#include "ChifTransiAF.h"
#include "ChiefExports.h"
#include "nanospec.h"
#include "ChiefView.h"
#include "MainFrm.h"
#include "ChifTransiMaster.h"
#include "ChifTransiSrRefer.h"
#include "ChifTransiDeskew.h"
#include "ChifTransiSr1Point.h"
#include "NanoSpecDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	�}�N����`
 */


/*
 *	�\���̒�`
 */


/*
 *	���[�J���O���[�o����`
 */
UINT WM_CHIF_TRAF_SETEVENT = RegisterWindowMessage("WM_CHIF_TRAF_SETEVENT");	// �g�����W�V�����C�x���g���s�p�E�B���h�E���b�Z�[�W
UINT WM_CHIF_TRAF_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRAF_HEARTBEAT");	// HeartBeat �E�B���h�E���b�Z�[�W


// ##########################################################################
// class CChiefTransiAF : AF �g�����W�V����
// ##########################################################################

// =========================================================================//
//	�g�����W�V�����e�[�u��
// =========================================================================//
const Transition TransTable_AF[] =
{
	//	OldState,				EventId,				NewState

	{	ST_AAF_INIT,			EV_AAF_DO_INIT,			ST_AAF_INIT				},	// ���������������s(���� EV_AAF_INIT_DONE ��)
	{	ST_AAF_INIT,			EV_AAF_INIT_DONE,		ST_AAF_IDLE				},	// �������������ăA�C�h����

	{	ST_AAF_IDLE,			EV_AAF_START,			ST_AAF_AF				},	// AF ���s���āCAF �����҂���

	{	ST_AAF_AF,				EV_AAF_OK,				ST_AAF_IDLE				},	// AF OK ����(���� EV_AAF_IDLE ��)�A�C�h����
	{	ST_AAF_AF,				EV_AAF_RO_PROCEED,		ST_AAF_IDLE				},	// AF NG(������������) ����(���� EV_AAF_IDLE ��)�A�C�h����
	{	ST_AAF_AF,				EV_AAF_RO_SKIP,			ST_AAF_IDLE				},	// AF NG(�����X�L�b�v) ����(���� EV_AAF_IDLE ��)�A�C�h����
	{	ST_AAF_AF,				EV_AAF_RO_CANCEL,		ST_AAF_IDLE				},	// AF NG(���V�s�I��) ����(���� EV_AAF_IDLE ��)�A�C�h����
	{	ST_AAF_AF,				EV_AAF_RO_POPUP,		ST_AAF_POPUP			},	// AF NG(�|�b�v�A�b�v) �����C�|�b�v�A�b�v���s���ă|�b�v�A�b�v�I���҂���

	{	ST_AAF_POPUP,			EV_AAF_POPUP_RETRY,		ST_AAF_POPUP_RETRY		},	// ���g���C�{�^��(���� EV_AAF_START ��)
	{	ST_AAF_POPUP_RETRY,		EV_AAF_START,			ST_AAF_AF				},	// �����������s���� AF �����҂���
	{	ST_AAF_POPUP,			EV_AAF_POPUP_IGNORE,	ST_AAF_POPUP_IGNORE		},	// AF �����{�^��(���� EV_AAF_IDLE ��)
	{	ST_AAF_POPUP_IGNORE,	EV_AAF_IDLE,			ST_AAF_IDLE				},	// �����������s���ăA�C�h����
	{	ST_AAF_POPUP,			EV_AAF_POPUP_SKIP,		ST_AAF_POPUP_SKIP		},	// �X�L�b�v�{�^��(���� EV_AAF_IDLE ��)
	{	ST_AAF_POPUP_SKIP,		EV_AAF_IDLE,			ST_AAF_IDLE				},	// �����������s���ăA�C�h����
	{	ST_AAF_POPUP,			EV_AAF_POPUP_CANCEL,	ST_AAF_POPUP_CANCEL		},	// �L�����Z���{�^��(���� EV_AAF_IDLE ��)
	{	ST_AAF_POPUP_CANCEL,	EV_AAF_IDLE,			ST_AAF_IDLE				},	// �����������s���ăA�C�h����

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};

// =========================================================================//
//	��ԕʏ����֐��e�[�u��
// =========================================================================//
BEGIN_STATEPROC_MAP(CChiefTransiAF)
	/*	0 */	ENTER_STATE(ST_AAF_INIT,			enter_ST_AAF_INIT),
	/*	1 */	ENTER_STATE(ST_AAF_IDLE,			enter_ST_AAF_IDLE),
	/*	2 */	ENTER_STATE(ST_AAF_AF,				enter_ST_AAF_AF),					// (WAIT ��Ԗ���)
	/*	3 */	ENTER_STATE(ST_AAF_POPUP,			enter_ST_AAF_POPUP),
	/*	4 */	ENTER_STATE(ST_AAF_POPUP_RETRY,		enter_ST_AAF_POPUP_RETRY),			// (WAIT ��Ԗ���)
	/*	5 */	ENTER_STATE(ST_AAF_POPUP_IGNORE,	enter_ST_AAF_POPUP_IGNORE),			// (WAIT ��Ԗ���)
	/*	6 */	ENTER_STATE(ST_AAF_POPUP_SKIP,		enter_ST_AAF_POPUP_SKIP),			// (WAIT ��Ԗ���)
	/*	7 */	ENTER_STATE(ST_AAF_POPUP_CANCEL,	enter_ST_AAF_POPUP_CANCEL),			// (WAIT ��Ԗ���)

END_STATEPROC_MAP(CChiefTransiAF)


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiAF

IMPLEMENT_DYNCREATE(CChiefTransiAF, CWinThread)

CChiefTransiAF::CChiefTransiAF(CWnd* pParent /*=NULL*/) : cActive(TransTable_AF)
{
	TRACE(_T("CChiefTransiAF::CChiefTransiAF() \n"));

	m_pcChiefView = (CView*) pParent;

	//	�X���b�h�̋N���҂�
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiAF."));

	//	�I�u�W�F�N�g�̎����j�����֎~
	m_bAutoDelete = FALSE;

}

CChiefTransiAF::~CChiefTransiAF()
{
	TRACE(_T("CChiefTransiAF::~CChiefTransiAF() \n"));

	//	�X���b�h�̏��ő҂�
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiAF."));
		}
	}
}

BOOL CChiefTransiAF::InitInstance()
{
	// TODO: ���̈ʒu�ɃX���b�h�P�ʂ̏������R�[�h��ǉ����Ă��������B

	TRACE(_T("CChiefTransiAF::InitInstance() \n"));

	// �R���X�g���N�^�̑҂����������܂�
	//		�܂�C�X���b�h�̐����ƃX���b�h�N���X�̍\�z�̓���������Ă��܂��D
	m_cSyncEvent.SetEvent();

	// ������ԏ����p�C�x���g���s
	TransiEvent(0, 0);

	// �ϐ�������
	m_dwAFValid = 0;
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiAF::ExitInstance()
{
	// TODO: ���̈ʒu�ɃX���b�h�P�ʂ̏������R�[�h��ǉ����Ă��������B

	TRACE(_T("CChiefTransiAF::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiAF, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiAF)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ����܂��B
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRAF_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRAF_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiAF ���b�Z�[�W �n���h��

/*
 *	�g�����W�V�����C�x���g���s ���b�Z�[�W�n���h��
 */
void CChiefTransiAF::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// �g�����W�V�����ԃ^�C�~���O����(�Ƃ肠���������ǍP�v�����E�E)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat ���b�Z�[�W�n���h��
 */
void CChiefTransiAF::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	���̑��֐�
****************************************************************************/

/*
 *	�g�����W�V�����C�x���g���s�p�֐�
 */
void CChiefTransiAF::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRAF_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRAF_SETEVENT, _T("WM_CHIF_TRAF_SETEVENT"), 3, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
}

/*
 *	HeartBeat ���s�p�֐�
 */
void CChiefTransiAF::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRAF_HEARTBEAT, 0, 0);
}

/*
 *	���݂̏�Ԃ��u�A�C�h���v���ǂ������`�F�b�N
 *		�߂�l�F	true	�A�C�h���ł���
 *					false	�A�C�h���łȂ�
 */
bool CChiefTransiAF::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_AAF_IDLE) {
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
int CChiefTransiAF::GetCurrentState()
{
	return CurrState;
}

/*
 *	�ďo���g�����W�V�����Ɋ�����ʒm
 */
void CChiefTransiAF::NotifyComplete(const cEventParams *const pEvParams)
{
	switch (m_dwParentTransi) {
	case TRANSI_SEQ:
	case TRANSI_AF:
// bagus add 2014.01.10 -->
	case TRANSI_MASTER_STD:
	case TRANSI_MASTER_DRK:
// bagus add 2014.01.10 <--
	default:
		// �������܂���D
		break;
	case TRANSI_MASTER:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_UIAAF_DONE, pEvParams);
		break;
	case TRANSI_SR_REFER:
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_AF_DONE, pEvParams);
		break;
	case TRANSI_DESKEW:
		((CChiefTransiDeskew*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiDeskew)->TransiEvent(EV_DSKW_AF_DONE, pEvParams);
		break;
	case TRANSI_SR_1POINT:
		((CChiefTransiSr1Point*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSr1Point)->TransiEvent(EV_SR1P_AF_DONE, pEvParams);
		break;
// bagus add 2014.01.10 -->
	case TRANSI_SR_STD_REFER:
		((CChiefTransiSrRefer*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSrRefer)->TransiEvent(EV_SREF_STD_AF_DONE, pEvParams);
		break;
// bagus add 2014.01.10 <--
	}
}

/*
 *	AF �g�����W�V�������L�����Z���I�������܂��D
 */
void CChiefTransiAF::CancelAF()
{
	m_bCancelReq = TRUE;
}

//--------------------------------------------------------------------------//
//	��ԕʏ����֐��Q
//--------------------------------------------------------------------------//
/*
 *	�������
 */
void CChiefTransiAF::enter_ST_AAF_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[0], (DWORD) pEvParams);

	// �����������͂���܂���D
}

/*
 *	�A�C�h��
 */
void CChiefTransiAF::enter_ST_AAF_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[1], (DWORD) pEvParams);

	m_bCancelReq = FALSE;
	// WAIT ��Ԃɂ��邽�ߏ����͂���܂���D
}

/*
 *	AF �����҂�(WAIT ��Ԗ���)
 */
void CChiefTransiAF::enter_ST_AAF_AF(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo �����R�����g�ǉ� ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))�F�ďo���g�����W�V����
/*				HIWORD(pEvParams)�FAF ���[�h
/*										�O�FAF �̂�
/*										�P�FPMA �̂�
/*										�Q�FAF �� PMA ����
/* added 2009.09.09 hmenjo �����R�����g�ǉ� ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[2], (DWORD) pEvParams);

	// ���̃u���b�N�͉��ϐ��̒�`�ł��� -----------------------------------------------------------------------------------------------
	// AF �֌W�̐ݒ�ϐ����s���Ȃ��߂ł��D
//		DWORD	l_dwRetryTime = 0;	// ���g���C�񐔐ݒ�
//		BOOL	l_bPopup = FALSE;	// AF �G���[���Ƀ|�b�v�A�b�v���邩�ǂ���
//		DWORD	l_dwNGMethod = 0;	// �|�b�v�A�b�v���Ȃ��ꍇ�̏I�����@
									//		1�F�X�L�b�v
									//		2�F��������(�v���V�[�h)(N/A �ɂ��Ȃ�)
									//		3�F��������(�v���V�[�h)(N/A �ɂ���)
									//		4�F���V�s�I��
	// ���̃u���b�N�͉��ϐ��̒�`�ł��� -----------------------------------------------------------------------------------------------
	// ���g���C�񐔐ݒ���擾
	DWORD	l_dwRetryTime = 0;	// ���g���C�񐔐ݒ�
	ConfigFile_GetNanoSpecIni(&l_dwRetryTime, CONFIG_FILE_SR_AUTOFOCUS_RETRY);
	if (50 < l_dwRetryTime) {	// �ő�ݒ�l�̐������T�O��ɂ��Ă����܂��D
		l_dwRetryTime = 50;
	}
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
/* added 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- { ---------- */
	HEAD_MAIN_RCP_INFO l_HeadMainRcpInf;
	((CChiefView*) m_pcChiefView)->GetMainRcpInfHead(l_pMainRcpInfo, l_pMainRcpInfo->MainRcpParam.hdr.wHeadType, &l_HeadMainRcpInf);
/* added 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- } ---------- */
	// ���g���C�L���̔��f
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- { ---------- */
//	switch (l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ----------			  */
	switch (l_HeadMainRcpInf.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- } ---------- */
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE:
		break;
	case AFOCUS_FAIL_OPTION_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_PROCEED:
	case AFOCUS_FAIL_OPTION_CANCEL_RECIPE:
	default:
		l_dwRetryTime = 0;		// ���g���C�����Ƃ��܂�
		break;
	}
	// �|�b�v�A�b�v�w����擾
	BOOL	l_bPopup;
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- { ---------- */
//	switch (l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ----------			  */
	switch (l_HeadMainRcpInf.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- } ---------- */
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_MANUAL_FOCUS:
		l_bPopup = TRUE;		// �|�b�v�A�b�v����
		break;
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE:
	case AFOCUS_FAIL_OPTION_PROCEED:
	case AFOCUS_FAIL_OPTION_CANCEL_RECIPE:
	default:
		l_bPopup = FALSE;		// �|�b�v�A�b�v���Ȃ�
		break;
	}
	// �I�����@���擾
	DWORD	l_dwNGMethod;
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- { ---------- */
//	switch (l_pMainRcpInfo->MainRcpParam._SR.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ----------			  */
	switch (l_HeadMainRcpInf.nAutoFocusFailOption) {
/* modified 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- } ---------- */
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED:
	case AFOCUS_FAIL_OPTION_PROCEED:
		l_dwNGMethod = 2;		// ��������(�v���V�[�h)(N/A �ɂ��Ȃ�)
		break;
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_MANUAL_FOCUS:
	case AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE:
	case AFOCUS_FAIL_OPTION_CANCEL_RECIPE:
	default:
		l_dwNGMethod = 4;		// ���V�s�I��
		break;
	}

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;

	// ����ďo���� VALID �f�[�^���N���A
/* modified 2009.09.09 hmenjo �����ύX ---------- { ---------- */
//	if (0 != LOWORD(pEvParams)) {
/* modified 2009.09.09 hmenjo �����ύX ----------			   */
	if (0 != LOBYTE(LOWORD(pEvParams))) {
/* modified 2009.09.09 hmenjo �����ύX ---------- } ---------- */
		m_dwAFValid = 0;
		// �ďo���g�����W�V������ۑ�
/* modified 2009.09.09 hmenjo �����ύX ---------- { ---------- */
//		m_dwParentTransi = LOWORD(pEvParams);
/* modified 2009.09.09 hmenjo �����ύX ----------			   */
		m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
/* modified 2009.09.09 hmenjo �����ύX ---------- } ---------- */
		// ���[�h��ۑ� (0:AF �̂݁C1:PMA �̂݁C2:AF �� PMA �̗���)
		m_dwAfMode = HIWORD(pEvParams);
	}

	if (0 == ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		// ���@�̏ꍇ ---------------------------------------------------------
		BOOL l_bAF_Success = FALSE;
		DWORD l_dwRetryCnt = 0;	// ���g���C�J�E���^
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
		if (3 == ((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove) {
			/* ���łɁCPreAF �� AF �������Ă���̂Ńp�X���܂��D*/
			((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove = 4;
			l_bAF_Success = TRUE;
			((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - Pass AF. (m_lPreAFafterPreMove = 4)"));
		} else
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
		if ((1 != m_dwAfMode)) {
			// PMA �݂̂łȂ��ꍇ�̂� AF �����s���܂�
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
			if (1 == ((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove) {
				((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove = 2 | 0x80000000;
				l_bPopup = FALSE;	/* �|�b�v�A�b�v����	*/
				((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - Start AF. (m_lPreAFafterPreMove = 2 | 0x80000000)"));
			}
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- { ---------- */
			/* AF �� SR �w�b�h�ōs���܂��D	*/
			((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, TRUE);
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- } ---------- */
			do {
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ���쒆�t���O(Z ��)���I��
				if (0 == NS_StageDoAutoFocus()) {
					// AF ���s
					l_dwRetryCnt++;
				} else {
					// AF ����
					l_bAF_Success = TRUE;
					((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
					break;		// ���[�v�𔲂��܂�
				}
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
// �L�����Z���Ή�����			} while (l_dwRetryCnt <= l_dwRetryTime);
			} while ((l_dwRetryCnt <= l_dwRetryTime) && (0 == m_bCancelReq));
		}
#if 1	// �L�����Z���Ή�����
		if (0 != m_bCancelReq) {
			m_bCancelReq = FALSE;
			// �g�����W�V�����X�V -----------------
			TransiEvent(EV_AAF_RO_CANCEL);
			// �ďo���g�����W�V�����Ɋ�����ʒm
			l_pAFValid->ucResultAF = 5;		// ���V�s�I��
			NotifyComplete((cEventParams*) m_dwAFValid);
		} else
#endif
//		if (0 == l_bAF_Success) {
		if ((0 == l_bAF_Success) || (0 != m_dwAfMode)) {
			// AF ���s�C���邢�́CAF ���[�h�� PMA ������ꍇ
			if ((1 != m_dwAfMode) && (0 == l_bAF_Success)) {
				// AF ���s�����ꍇ�̂�(PMA �݂̂łȂ��ꍇ)
			// ���g���C�I�[�o���Z�b�g
			l_pAFValid->ucRetry = static_cast<BYTE>(l_dwRetryCnt - 1);
			}
//			if (0 == l_bPopup) {
			if ((0 == l_bPopup) && ((0 == m_dwAfMode) || ((2 == m_dwAfMode) && (2 != l_dwNGMethod)))) {
				// �|�b�v�A�b�v�����w��ŁC���[�h�� AF �݂̂��CAF. �݈̂ȊO�Ŏ�����������ȊO�̏ꍇ
				// �I������ ---------------------------
				l_pAFValid->ucResultPopup = 0;	// �|�b�v�A�b�v����
				int l_iEventNext;
				switch (l_dwNGMethod) {
				case 1:		// �X�L�b�v
					l_pAFValid->ucResultAF = 3;		// �����X�L�b�v
					l_iEventNext = EV_AAF_RO_SKIP;
					break;
				case 2:		// ��������(N/A �ɂ��Ȃ�)
				case 3:		// ��������(N/A �ɂ���)
					l_pAFValid->ucResultAF = 2;		// ������������
					l_iEventNext = EV_AAF_RO_PROCEED;
					break;
				case 4:		// ���V�s�I��
				default:
					l_pAFValid->ucResultAF = 5;		// ���V�s�I��
					l_iEventNext = EV_AAF_RO_CANCEL;
					break;
				}
				// �g�����W�V�����X�V -----------------
				TransiEvent(l_iEventNext);
				// �ďo���g�����W�V�����Ɋ�����ʒm
				NotifyComplete((cEventParams*) m_dwAFValid);
			} else {
				// �|�b�v�A�b�v���� -------------------
				//		���̃g�����W�V�����Ń|�b�v�A�b�v�w�߂𔭍s���Ă��܂��D
				// AF ���ʂ�ݒ�
				if (0 != l_bAF_Success) {
					// AF ������
					l_pAFValid->ucResultAF = 1;		// AF ����
				} else {
					// AF ���s��
					if (1 == m_dwAfMode) {
						// PMA �݂̂̏ꍇ�� AF ���Ă��Ȃ��̂�
						l_pAFValid->ucResultAF = 0;		// AF ����
					} else {
						switch (l_dwNGMethod) {
						case 2:		// ��������(N/A �ɂ��Ȃ�)
						case 3:		// ��������(N/A �ɂ���)
							l_pAFValid->ucResultAF = 2;		// ������������
							break;
						default:
							l_pAFValid->ucResultAF = 4;		// AF ���s
							break;
						}
					}
				}
				// �g�����W�V�����X�V -----------------
//				TransiEvent(EV_AAF_RO_POPUP);
				DWORD l_dwPopupMode;
				if (0 == l_bPopup) {
					l_dwPopupMode = 1;			// PMA �̂�
				} else if (1 == m_dwAfMode) {
					l_dwPopupMode = 1;			// PMA �̂�
				} else if (0 != m_dwAfMode) {
					if (0 != l_bAF_Success) {
						l_dwPopupMode = 1;		// PMA �̂�
					} else {
						l_dwPopupMode = 2;		// AF �� PMA �̗���
					}
				} else {
					l_dwPopupMode = 0;			// AF �̂�
				}
				l_pAFValid->bit2PopupKind = (BYTE) l_dwPopupMode;		// �|�b�v�A�b�v���
				TransiEvent(EV_AAF_RO_POPUP, (cEventParams*) l_dwPopupMode);
			}
		} else {
			// AF ���� --------------------------------------------------------
			l_pAFValid->ucResultAF = 1;		// AF ����
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
			if (0 != (((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove & 0x80000000)) {
				((CChiefView*) m_pcChiefView)->m_lPreAFafterPreMove &= 0x7fffffff;
				((CChiefView*) m_pcChiefView)->LogChief(_T("PreAF - m_lPreAFafterPreMove = 2"));
			}
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
			// �g�����W�V�����X�V ---------------------
			TransiEvent(EV_AAF_OK);
			// �ďo���g�����W�V�����Ɋ�����ʒm
			NotifyComplete((cEventParams*) m_dwAFValid);
		}
	} else {
		// �n�[�h�V�~�����[�g���[�h�̏ꍇ�͐��튮���ɂ��� ---------------------
#ifdef CHIEF_USE_SLEEP
		::Sleep(1000);
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(48) ---------- { ---------- */
#else
		::Sleep(200);
/* added 2009.08.20 hmenjo �X�g���X�@�\�ǉ�(48) ---------- } ---------- */
#endif
		l_pAFValid->ucRetry = 0;
		l_pAFValid->ucResultAF = 0;
		l_pAFValid->ucResultPopup = 0;
		l_pAFValid->bit1ManuXY = 0;
		l_pAFValid->bit1ManuZ = 0;
		l_pAFValid->bit1ManuAF = 0;
		l_pAFValid->bit1ManuAfRslt = 0;
		// �g�����W�V�����X�V -------------------------
		TransiEvent(EV_AAF_OK);
		// �ďo���g�����W�V�����Ɋ�����ʒm
		NotifyComplete((cEventParams*) m_dwAFValid);
	}
}

/*
 *	�|�b�v�A�b�v�I���҂�
 */
void CChiefTransiAF::enter_ST_AAF_POPUP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[3], (DWORD) pEvParams);

	// �|�b�v�A�b�v���A���[���Œʒm���܂��D
	if (HOST_LOCAL != ((CNanoSpecDoc*) (((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc))->GetHostMode()) {
		WORD l_wAlID;
		switch ((long) pEvParams) {
		case 0:		l_wAlID = ALID_AutoFocusRequired;			break;	// AF �̂�
		case 1:		l_wAlID = ALID_StagePositioningRequired;	break;	// PMA �̂�
		case 2:
		default:	l_wAlID = ALID_FocusAndPositioningRequired;	break;	// AF �� PMA �̗���
		}
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_OFF), MAKEWORD(1, CHRAMSG_OK)), MAKELPARAM(0, l_wAlID));
	}
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- { ---------- */
	/* PMA �L��̏ꍇ�ŁC���V�s��(MS)�w�b�h�Ȃ�C(MS)�w�b�h�ɐؑւ��܂��D	*/
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	if ((0 != ((long) pEvParams)) && (HEAD_TYPE_MS == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)) {
		if (1 != m_dwAfMode) {
			/* AF �����{�����ꍇ�̂�	*/
			((CChiefView*) m_pcChiefView)->SelectHead(l_pMainRcpInfo->MainRcpParam.hdr.wHeadType, TRUE);
		}
	}
/* added 2009.11.26 hmenjo ���� Seq �͎w��w�b�h�œ��� ---------- } ---------- */
	// �|�b�v�A�b�v���� -------------------
	// ��ʂɃ|�b�v�A�b�v�w�߃��b�Z�[�W�� Post ����
//	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AF_POPUP, 0, 0);
//	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AF_POPUP, (WPARAM) m_dwAfMode, 0);
	((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->PostMessage(WM_CHIF_AF_POPUP, (WPARAM) pEvParams, 0);

	// WAIT ��Ԃɂ��邽�߃g�����W�V�������X�V���܂���D
}

/*
 *	�|�b�v�A�b�v�I�� ���g���C�{�^��(WAIT ��Ԗ���)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_RETRY(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[4], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 1;		// ���g���C�{�^��
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// �g�����W�V�����X�V -----------------------------------------------------
	TransiEvent(EV_AAF_START);
}

/*
 *	�|�b�v�A�b�v�I�� AF �����{�^��(WAIT ��Ԗ���)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_IGNORE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[5], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 2;		// ����(OK)�{�^��
// �R�R�ł͐ݒ肵�܂���D	l_pAFValid->ucResultAF = 4;
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// �g�����W�V�����X�V -----------------------------------------------------
	TransiEvent(EV_AAF_IDLE);

	// �ďo���g�����W�V�����Ɋ�����ʒm
	NotifyComplete((cEventParams*) m_dwAFValid);
}

/*
 *	�|�b�v�A�b�v�I�� �X�L�b�v�{�^��(WAIT ��Ԗ���)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_SKIP(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[6], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 3;		// �X�L�b�v�{�^��
// �R�R�ł͐ݒ肵�܂���D	l_pAFValid->ucResultAF = 4;
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// �g�����W�V�����X�V -----------------------------------------------------
	TransiEvent(EV_AAF_IDLE);

	// �ďo���g�����W�V�����Ɋ�����ʒm
	NotifyComplete((cEventParams*) m_dwAFValid);
}

/*
 *	�|�b�v�A�b�v�I�� �L�����Z���{�^��(WAIT ��Ԗ���)
 */
void CChiefTransiAF::enter_ST_AAF_POPUP_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_AAF_DISP[7], (DWORD) pEvParams);

	AF_VALID*	l_pAFValid = (AF_VALID*) &m_dwAFValid;
	l_pAFValid->ucResultPopup = 4;		// �L�����Z���{�^��
// �R�R�ł͐ݒ肵�܂���D	l_pAFValid->ucResultAF = 4;
	l_pAFValid->bit1ManuXY		= (0 == LOBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuZ		= (0 == HIBYTE(LOWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAF		= (0 == LOBYTE(HIWORD(pEvParams)))? 0 : 1;
	l_pAFValid->bit1ManuAfRslt	= (0 == HIBYTE(HIWORD(pEvParams)))? 0 : 1;

	// �g�����W�V�����X�V -----------------------------------------------------
	TransiEvent(EV_AAF_IDLE);

	// �ďo���g�����W�V�����Ɋ�����ʒm
	NotifyComplete((cEventParams*) m_dwAFValid);
}
