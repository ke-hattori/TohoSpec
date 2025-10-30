#pragma once

/*-------------------------------------------------------------------
 *	Chief �O�����W���[���̂��߂̒�`�t�@�C��
 *-----------------------------------------------------------------*/

#define	CHIEF_INI	_T("Chief.ini")	// Chief �p�ݒ�t�@�C����
#define CHIEF_DLG_CAPTION	_T("TRANSITION MONITOR")	// Chief �_�C�A���O�̃L���v�V����
//#define ADAP_EXE_PATH	_T("C:\\WVASE32\\Adap\\jaw_adap.exe")	// XMP �p�X
#define	CHIEF_TRMAS_INIT_TIME	40000	// [ms] �}�X�^�g�����W�V�����������҂�����
#define	CHIEF_TRMAS_END_TIME	20000	// [ms] �}�X�^�g�����W�V�����I���҂�����
#define	TIMER_DELAY_COMPLETE	2000	// Complete �f�B���C�^�C�}�l[ms]
#define	TIMER_DELAY_ABORT		2000	// Abort �f�B���C�^�C�}�l[ms]
#define CIE_MINWAVE				(380)	// CIE/OD�l�ŏ��g��		(SubTransition.cpp ����ړ�)
#define CIE_MAXWAVE				(780)	// CIE/OD�l�ő�g��		(SubTransition.cpp ����ړ�)
#define	TRANSI_TIME				10		// �g�����W�V�����ԃ^�C�~���O����(�Ƃ肠���������ǍP�v�����E�E)
#define	CHIEF_STGMVABS_MODE		TRUE	// StageMoveAbsoluteEx() �̑҂����[�h�w��(TRUE�F�҂CFALSE�F�҂��Ȃ�)
#define	TIMER_EQPWOFF_TIMEOUT	10000	// ���u�d���I�t�Ď�����[ms]
// �g�����W�V���� ID ��`
enum TRANSI_ID {
	TRANSI_MASTER = 1,		// MAS	�}�X�^ �g�����W�V����
	TRANSI_SEQ,				// SEQ	�V�[�P���X���� �g�����W�V����
	TRANSI_SR_REFER,		// SREF ���t�@�����X���� �g�����W�V����
	TRANSI_DESKEW,			// DSKW �f�X�L���[ �g�����W�V����
	TRANSI_AF,				// AAF	AF �g�����W�V����
	TRANSI_SR_1POINT,		// SR1P �P�|�C���g���� �g�����W�V����
/* added 2009.07.31 hmenjo �X�g���X�@�\�ǉ�(9) ---------- { ---------- */
	TRANSI_STRESS_SEQ,		/* STRS	�X�g���X���� �g�����W�V����	*/
/* added 2009.07.31 hmenjo �X�g���X�@�\�ǉ�(9) ---------- } ---------- */
// bagus add 2014.01.10 -->
	TRANSI_MASTER_STD,		// MAS_STD	�}�X�^�X�^���_�[�h �g�����W�V����
	TRANSI_MASTER_DRK,		// MAS_DRK	�}�X�^�_�[�N �g�����W�V����
// bagus add 2014.01.10 <--
	TRANSI_SR_STD_REFER,	// SREF_STD �X�^���_�[�h���t�@�����X���� �g�����W�V����
};
// AF VALID �f�[�^��`
#if 0
typedef union {
	DWORD	dwValid;
	struct {
		BYTE	ucRetry;				// AF ���g���C��
		BYTE	ucResultAF;				// AF ����				(0:AF �����C1:AF ����C2:������������C3:�����X�L�b�v�C4:AF ���s�C5:���V�s�I��)
		BYTE	ucResultPopup;			// �|�b�v�A�b�v����		(0:�|�b�v�A�b�v�����C1:���g���C�{�^���C2:����(OK)�{�^���C3:�X�L�b�v�{�^���C4:�L�����Z���{�^��)
										//			�|�b�v�A�b�v���������ꍇ�́C���O�ɂȂ�܂��D
		BYTE	bit1ManuXY		: 1;	// XY �}�j���A������	(1:����L��)
		BYTE	bit1ManuZ		: 1;	// Z �}�j���A������		(1:����L��)
		BYTE	bit1ManuAF		: 1;	// AF �}�j���A������	(1:����L��)
		BYTE	bit1ManuAfRslt	: 1;	// AF �}�j���A�����쌋��(1:�}�j���A�� AF ���s)
		BYTE	bit3Reserve		: 3;	// (�\��)
		BYTE	bit1Err			: 1;	// (�\��F�g�p�֎~)(���̃r�b�g���P�̏ꍇ�C���̃r�b�g�ȊO�̃r�b�g�̓G���[�X�e�[�^�X�������ꍇ������܂�)
	};
} AF_VALID;
#else
typedef union {			// �����炪�ŐV�ł��D
	DWORD	dwValid;
	struct {
		BYTE	ucRetry;				// AF ���g���C��
		BYTE	ucResultAF		: 3;	// AF ����				(0:AF �����C1:AF ����C2:������������C3:�����X�L�b�v�C4:AF ���s�C5:���V�s�I��)
		BYTE	ucResultPopup	: 3;	// �|�b�v�A�b�v����		(0:�|�b�v�A�b�v�����C1:���g���C�{�^���C2:����(OK)�{�^���C3:�X�L�b�v�{�^���C4:�L�����Z���{�^��)
										//			�|�b�v�A�b�v���������ꍇ�́C���O�ɂȂ�܂��D
		BYTE	bit2PopupKind	: 2;	// �|�b�v�A�b�v���		(0:AF �̂݁C1:PMA �̂݁C2:AF & PMA)
		BYTE	bitReserve16_23	: 8;	// (�\��)
		BYTE	bit1ManuXY		: 1;	// XY �}�j���A������	(1:����L��)
		BYTE	bit1ManuZ		: 1;	// Z �}�j���A������		(1:����L��)
		BYTE	bit1ManuAF		: 1;	// AF �}�j���A������	(1:����L��)
		BYTE	bit1ManuAfRslt	: 1;	// AF �}�j���A�����쌋��(1:�}�j���A�� AF ���s)
		BYTE	bitReserve28_30	: 3;	// (�\��)
		BYTE	bit1Err			: 1;	// (�\��F�g�p�֎~)(���̃r�b�g���P�̏ꍇ�C���̃r�b�g�ȊO�̃r�b�g�̓G���[�X�e�[�^�X�������ꍇ������܂�)
	};
} AF_VALID;
#endif
// �P�|�C���g����I��(WM_COPYDATA�FWMCD_CHIF_1POINT_END)�f�[�^��`
typedef struct {
		DWORD	dwPointNo;		// �I���|�C���g�ԍ�
		DWORD	dwAFValid;		// AF_VALID �f�[�^
		long	lX;				// X ���ʒu
		long	lY;				// Y ���ʒu
		long	lZ;				// Z ���ʒu
} MEAS_1POINT_RESULT;
// ����J�n�ʒm(WM_COPYDATA�FWMCD_CHIF_MEAS_START)�f�[�^��`
/* added 2009.09.09 hmenjo ���ߗ� Seq ���t�@�����X���[�h(16) ---------- { ---------- */
enum MEASMODE_ID {
	MEASMODE_NORM = 0,	/* �ʏ푪��(SR�CSE�C���ߗ�)	*/
	MEASMODE_GTRREF,	/* �K���g�����ߗ����t�@�����X����	*/
//2009.11.03 bagus 2poin-distance �폜 --{--
//	MEASMODE_DISTANCE,	/* 2�_�ԋ��� */
//2009.11.03 bagus 2poin-distance �폜 --}--
/* added 2009.10.29 hmenjo CTA Seq API �ǉ� ---------- { ---------- */
	MEASMODE_CTA,		/* CTA ����	*/
/* added 2009.10.29 hmenjo CTA Seq API �ǉ� ---------- } ---------- */
//2009.11.03 bagus MS �ǉ� --{--
	MEASMODE_MS,
//2009.11.03 bagus MS �ǉ� --}--
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	MEASMODE_COMPEASE,
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
};
/* added 2009.09.09 hmenjo ���ߗ� Seq ���t�@�����X���[�h(16) ---------- } ---------- */
typedef struct {
	BOOL	bGetSamplID;		// FALSE�F�T���v�� ID �̎w��͖���(��ʂ���̃V�[�P���X����J�n�̂���)
								// TRUE	�F�T���v�� ID �̎w�肪�L��(Pif ����̃V�[�P���X����J�n)
	TCHAR	szSampleID[256];	// �T���v�� ID ������
/* added 2009.10.29 hmenjo CTA ���b�g ID �Ή� ---------- { ---------- */
	TCHAR	tszLotID[256];		/* ���b�g ID ������ (bGetSamplID �Ɉ˂�)	*/
/* added 2009.10.29 hmenjo CTA ���b�g ID �Ή� ---------- } ---------- */
/* added 2009.09.09 hmenjo ���ߗ� Seq ���t�@�����X���[�h(16) ---------- { ---------- */
	MEASMODE_ID	enMeasMode;		/* ���胂�[�h MEASMODE_ID	*/
/* added 2009.09.09 hmenjo ���ߗ� Seq ���t�@�����X���[�h(16) ---------- } ---------- */
} SEQ_MEAS_START;

/* added 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- { ---------- */
typedef struct {
	UINT nFocus;
	UINT nAutoFocusFailOption;
} HEAD_MAIN_RCP_INFO, *LPHEAD_MAIN_RCP_INFO;
typedef const HEAD_MAIN_RCP_INFO *LPCHEAD_MAIN_RCP_INFO;
/* added 2009.09.29 hmenjo Head �� Main Rcp Inf �Ǐo�� ---------- } ---------- */

/* added 2009.10.15 hmenjo Stage PGM ���ʉ��Ή� �w�b�h�^�C�v���� ---------- { ---------- */
/* �w�b�h�^�C�v STD �ϊ��e�[�u��	*/
const WORD HEADTYPE_STD_TBL[] = {
	HEAD_TYPE_SR,
	HEAD_TYPE_SE,
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	HEAD_TYPE_COMPEASE,
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
//	HEAD_TYPE_IRSE,
// 2009.10.19 bagus MS �ǉ� --}--
	HEAD_TYPE_4PP,
	HEAD_TYPE_CTA,
// 2009.10.19 bagus MS �ǉ� --{--
	HEAD_TYPE_MS,
// 2009.10.19 bagus MS �ǉ� --}--
	static_cast<WORD>(-1),
};
/* added 2009.10.15 hmenjo Stage PGM ���ʉ��Ή� �w�b�h�^�C�v���� ---------- } ---------- */

/*
 *	�e��A���[���񍐗p��`
 */
/*			�p�����^����
 *				wparam
 *						[�P]�FLOBYTE(LOWORD(wparam))	�񍐕��@
 *															���b�Z�[�W�{�b�N�X�̕\���̂�
 *															�A���[���񍐂̂�
 *															����
 *						[�Q]�FHIBYTE(LOWORD(wparam))	�e�ʒm(CMainFrame)
 *															���Ȃ�
 *															����(���̌�̏����͐e����ł�)
 *						[�R]�FLOBYTE(HIWORD(wparam))	���b�Z�[�W�{�b�N�X�̃^�C�g���ԍ�
 *						[�S]�FHIBYTE(HIWORD(wparam))	���b�Z�[�W�{�b�N�X�̃{�^���\��
 *															OK
 *															YES/NO(YES �̏ꍇ�͐e�ʒm���̃A�v���I���p�����^�� TRUE �ɂ���)
 *				lparam
 *						[�T]�FLOWORD(lparam)			���b�Z�[�W�{�b�N�X�̃e�L�X�g�ԍ�
 *						[�U]�FHIWORD(lparam)			�A���[���񍐂� ALID
 *			�w����@
 *				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD([�P], [�Q]), MAKEWORD([�R], [�S])), MAKELPARAM([�T], [�U]));
 */
enum CHIEF_REP_ALARM_METHOD {		// [�P]�񍐕��@
	CHRAMTD_NONE = 0,					// (�������Ȃ�)
	CHRAMTD_MSGBOX,						// ���b�Z�[�W�{�b�N�X�̕\���̂�
	CHRAMTD_REP_ALARM,					// �A���[���񍐂̂�
	CHRAMTD_BOTH,						// ����
};
enum CHIEF_REP_ALARM_NOTIFY {		// [�Q]�e�ʒm
	CHRANFY_NOTIFY_OFF = 0,				// ���Ȃ�
	CHRANFY_NOTIFY_ON,					// ����(���̌�̏����͐e����ł�)
};
enum CHIEF_REP_ALARM_MSGBUTTON {	// [�S]���b�Z�[�W�{�b�N�X�̃{�^���\��
	CHRAMSG_OK = 0,						// OK
	CHRAMSG_YESNO,						// YES/NO(YES �̏ꍇ�͐e�ʒm���̃A�v���I���p�����^�� TRUE �ɂ���)
};
const LPSTR CHIEF_REP_ALARM_MSGTITLE[] = {		// [�R]���b�Z�[�W�{�b�N�X�̃^�C�g��
	/*	0 */ _T("NanoSpec Chief CChiefThread"),
	/*	1 */ _T("NanoSpec Chief CChiefView"),
	/*	2 */ _T("NanoSpec Chief CChiefRcvMailThread"),
	/*	3 */ _T("NanoSpec Chief CChiefTransiAF"),
	/*	4 */ _T("NanoSpec Chief CChiefTransiDeskew"),
	/*	5 */ _T("NanoSpec Chief CChiefTransiMaster"),
	/*	6 */ _T("NanoSpec Chief CChiefTransiSeq"),
	/*	7 */ _T("NanoSpec Chief CChiefTransiSr1Point"),
	/*	8 */ _T("NanoSpec Chief CChiefTransiSrRefer"),
/* added 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
	/*	9 */ _T("NanoSpec Chief CChiefTransiStress"),
/* added 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
	/*	0 */ _T("NONE"),
};
const LPSTR CHIEF_REP_ALARM_MSGTEXT[] = {		// [�T]���b�Z�[�W�{�b�N�X�̃e�L�X�g
	/*	0 */ _T("Failed to Start timer(SetTimer()) for Trace Data. \nTerminate this application ???"),
	/*	1 */ _T("Failed to Kill timer(KillTimer()) for Trace Data. \nTerminate this application ???"),
	/*	2 */ _T("Failed to Start Timer(SetTimer()) to Autohide Chief Dialog. \nTerminate this application ???"),
	/*	3 */ _T("Failed to Kill Timer(KillTimer()) to Autohide Chief Dialog. \nTerminate this application ???"),
	/*	4 */ _T("Failed to Start Thread for Master Transition (CChiefTransiMaster). \nTerminate this application ???"),
	/*	5 */ _T("Failed to Start Thread for Sequence Transition (CChiefTransiSeq). \nTerminate this application ???"),
	/*	6 */ _T("Failed to Start Thread for SR 1 Point Transition (CChiefTransiSr1Point). \nTerminate this application ???"),
	/*	7 */ _T("Failed to Start Thread for SR Reference Transition (CChiefTransiSrRefer). \nTerminate this application ???"),
	/*	8 */ _T("Failed to Start Thread for Deskew Transition (CChiefTransiDeskew). \nTerminate this application ???"),
	/*	9 */ _T("Failed to Start Thread for AF Transition (CChiefTransiAF). \nTerminate this application ???"),
	/* 10 */ _T("Failed to Start Thread for Receive Mail (CChiefRcvMailThread). \nTerminate this application ???"),
	/* 11 */ _T("Failed to Start timer(SetTimer() : 10ms period). \nTerminate this application ???"),
	/* 12 */ _T("Failed to Start timer(SetTimer() : 50ms period). \nTerminate this application ???"),
	/* 13 */ _T("Failed to Start timer(SetTimer() : 100ms period). \nTerminate this application ???"),
	/* 14 */ _T("Failed to Start timer(SetTimer() : 1s period). \nTerminate this application ???"),
	/* 15 */ _T("Failed to Start Multimedia timer(::timeSetEvent()). \nTerminate this application ???"),
	/* 16 */ _T("Received undefined command from Pif."),
	/* 17 */ _T("Failed to create Event for Master Transition initialized. \nTerminate this application ???"),
	/* 18 */ _T("Failed to initialize Master Transition. \nTerminate this application ???"),
	/* 19 */ _T("Failed to start process (CreateProcess) Adap. \nTerminate this application ???"),
	/* 20 */ _T("PatternRec Initialize Error. \nTerminate this application ???"),
	/* 21 */ _T("Stage Initialize Error. \nTerminate this application ???"),
	/* 22 */ _T("Measys Initialize Error. \nTerminate this application ???"),
	/* 23 */ _T("Failed to terminate Master Transition."),
	/* 24 */ _T("Failed to allocate memory for Main Recipe. \nTerminate this application."),
	/* 25 */ _T("Failed to allocate memory for Measurement Program. \nTerminate this application."),
	/* 26 */ _T("Failed to allocate memory for Stage Program. \nTerminate this application."),
	/* 27 */ _T("Failed to allocate memory for Scan Points. \nTerminate this application."),
	/* 28 */ _T("Failed to read Recipe (Main Recipe)."),
	/* 29 */ _T("Failed to read Recipe (Measurement Program)."),
	/* 30 */ _T("Failed to read Recipe (Stage Program)."),
	/* 31 */ _T("Failed to read Recipe (Out of Range for number of measurement points)."),
	/* 32 */ _T("Failed to read Recipe (Measurement points)."),
	/* 33 */ _T("Failed to read Recipe (others)."),
	/* 34 */ _T("Failed to Start timer(SetTimer() for EQ Power OFF timeout). \nTerminate this application ???"),
	/* 35 */ _T("Failed to Kill timer(KillTimer() for EQ Power OFF timeout). \nTerminate this application ???"),
// 2009.05.17 myanagida -->
	/* 36 */ _T("Failed to allocate memory for SitePattern table. \nTerminate this application."),
// 2009.05.17 myanagida <--
/* added 2009.07.31 hmenjo �X�g���X�@�\�ǉ�(3) ---------- { ---------- */
	/* 37 */ _T("Failed to Start timer(SetTimer()) for HEPA stop wait. \nTerminate this application ???"),
	/* 38 */ _T("Failed to Kill timer(KillTimer()) for HEPA stop wait. \nTerminate this application ???"),
/* added 2009.07.31 hmenjo �X�g���X�@�\�ǉ�(3) ---------- } ---------- */
/* added 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
	/* 39 */ _T("Failed to TransiEvent(PostThreadMessage). \nTerminate this application ???"),
	/* 40 */ _T("Transition busy. \nTerminate this application ???"),
/* added 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
	/*	0 */ _T("NONE"),	//	0�F
};

//Saiki 20090602 Delete ----->
/*
 *	�ȉ��� MainFrame �̃X�e�[�^�X�o�[�֕\������e�L�X�g��`�ł��D
 *		�����C���\�[�X�� String table �̃��\�[�X ID �ɂȂ�\�������邽�߁C
 *		�����悤�Ȗ��O�Œ�`���܂��D
 */
/*//// Chief �_�C�A���O�ł̐���
//#define	IDS_CHIF_MLDPOS_MOVING				_T("�}�j���A�����[�h�|�W�V�����ֈړ����E�E�E")
////#define IDS_CHIF_MLDPOS_MOVED				_T("�}�j���A�����[�h�|�W�V�����ֈړ������D")
//#define	IDS_CHIF_MLDPOS_MOVED				_T("")
//#define	IDS_CHIF_HLDPOS_MOVING				_T("�n���h�����[�h�|�W�V�����ֈړ����E�E�E")
////#define IDS_CHIF_HLDPOS_MOVED				_T("�n���h�����[�h�|�W�V�����ֈړ������D")
//#define	IDS_CHIF_HLDPOS_MOVED				_T("")
//#define	IDS_CHIF_GENPOS_MOVING				_T("�ėp�|�W�V�����ֈړ����E�E�E")
////#define IDS_CHIF_GENPOS_MOVED				_T("�ėp�|�W�V�����ֈړ������E�E�E")
//#define	IDS_CHIF_GENPOS_MOVED				_T("")
//#define	IDS_CHIF_HWS_MLDPOS_MOVING			_T("[�n�[�h�V�~�����[�g���[�h]�}�j���A�����[�h�|�W�V�����ֈړ����E�E�E")
////#define IDS_CHIF_HWS_MLDPOS_MOVED			_T("[�n�[�h�V�~�����[�g���[�h]�}�j���A�����[�h�|�W�V�����ֈړ������D")
//#define	IDS_CHIF_HWS_MLDPOS_MOVED			_T("")
//#define	IDS_CHIF_HWS_HLDPOS_MOVING			_T("[�n�[�h�V�~�����[�g���[�h]�n���h�����[�h�|�W�V�����ֈړ����E�E�E")
////#define IDS_CHIF_HWS_HLDPOS_MOVED			_T("[�n�[�h�V�~�����[�g���[�h]�n���h�����[�h�|�W�V�����ֈړ������D")
//#define	IDS_CHIF_HWS_HLDPOS_MOVED			_T("")
//#define	IDS_CHIF_HWS_GENPOS_MOVING			_T("[�n�[�h�V�~�����[�g���[�h]�ėp�|�W�V�����ֈړ����E�E�E")
////#define IDS_CHIF_HWS_GENPOS_MOVED			_T("[�n�[�h�V�~�����[�g���[�h]�ėp�|�W�V�����ֈړ������E�E�E")
//#define	IDS_CHIF_HWS_GENPOS_MOVED			_T("")
//// ���t�@�����X����
//#define IDS_CHIF_SREF_HWS_N_1ST_MEASING		_T("���t�@�����X����F[�n�[�h�V�~�����[�g���[�h](�m�[�}��)1st���t�@�����X���蒆�E�E�E")
//#define IDS_CHIF_SREF_HWS_D_T1_MEASING		_T("���t�@�����X����F[�n�[�h�V�~�����[�g���[�h]�_�[�N���t�@�����XT1���蒆�E�E�E")
//#define IDS_CHIF_SREF_HWS_D_T1_COMPLETE		_T("���t�@�����X����F[�n�[�h�V�~�����[�g���[�h]�_�[�N���t�@�����XT1���芮���D")
//#define IDS_CHIF_SREF_N_STAGE_MOVING		_T("���t�@�����X����F(�m�[�}��)���t�@�����X����ʒu(�w�x)�ֈړ����E�E�E")
//#define IDS_CHIF_SREF_N_STAGE_MOVED			_T("���t�@�����X����F(�m�[�}��)���t�@�����X����ʒu(�w�x)�ֈړ������D")
//#define	IDS_CHIF_SREF_N_Z_MOVING			_T("���t�@�����X����F(�m�[�}��)���t�@�����X����ʒu(�y)�ֈړ����E�E�E")
//#define	IDS_CHIF_SREF_N_Z_MOVED 			_T("���t�@�����X����F(�m�[�}��)���t�@�����X����ʒu(�y)�ֈړ������D")
//#define	IDS_CHIF_SREF_N_SWAYSETTLING		_T("���t�@�����X����F(�m�[�}��)���t�@�����X�F�w�b�h�h������҂��E�E�E")
//#define	IDS_CHIF_SREF_N_FOCUSING			_T("���t�@�����X����F(�m�[�}��)���t�@�����X�F�t�H�[�J�X���E�E�E")
//#define	IDS_CHIF_SREF_N_FOCUS_FAILED		_T("���t�@�����X����F(�m�[�}��)���t�@�����X�F�t�H�[�J�X���s�D")
//#define	IDS_CHIF_SREF_N_1ST_MEASING 		_T("���t�@�����X����F(�m�[�}��)1st���t�@�����X���蒆�E�E�E")
//#define	IDS_CHIF_SREF_N_1ST_MEAS_FAILED 	_T("���t�@�����X����F(�m�[�}��)1st���t�@�����X���莸�s�D")
//#define	IDS_CHIF_SREF_N_2ND_MEASING 		_T("���t�@�����X����F(�m�[�}��)2nd���t�@�����X���蒆�E�E�E")
//#define	IDS_CHIF_SREF_N_2ND_T1_MEAS_FAILED	_T("���t�@�����X����F(�m�[�}��)2nd���t�@�����XT1���莸�s�D")
//#define	IDS_CHIF_SREF_N_2ND_T2_MEAS_FAILED	_T("���t�@�����X����F(�m�[�}��)2nd���t�@�����XT2���莸�s�D")
//#define	IDS_CHIF_SREF_D_STAGE_MOVING		_T("���t�@�����X����F�_�[�N���t�@�����X����ʒu(�w�x)�ֈړ����E�E�E")
//#define	IDS_CHIF_SREF_D_STAGE_MOVED 		_T("���t�@�����X����F�_�[�N���t�@�����X����ʒu(�w�x)�ֈړ������D")
//#define	IDS_CHIF_SREF_D_Z_MOVING			_T("���t�@�����X����F�_�[�N���t�@�����X����ʒu(�y)�ֈړ����E�E�E")
//#define	IDS_CHIF_SREF_D_Z_MOVED 			_T("���t�@�����X����F�_�[�N���t�@�����X����ʒu(�y)�ֈړ������D")
//#define	IDS_CHIF_SREF_D_SWAYSETTLING		_T("���t�@�����X����F�_�[�N���t�@�����X�F�w�b�h�h������҂��E�E�E")
//#define	IDS_CHIF_SREF_D_T1_MEASING			_T("���t�@�����X����F�_�[�N���t�@�����XT1���蒆�E�E�E")
//#define	IDS_CHIF_SREF_D_T1_MEAS_FAILED		_T("���t�@�����X����F�_�[�N���t�@�����XT1���莸�s�D")
//#define	IDS_CHIF_SREF_D_T2_MEASING			_T("���t�@�����X����F�_�[�N���t�@�����XT2���蒆�E�E�E")
//#define	IDS_CHIF_SREF_D_T2_MEAS_FAILED		_T("���t�@�����X����F�_�[�N���t�@�����XT2���莸�s�D")
//#define	IDS_CHIF_SREF_COMPLETED 			_T("���t�@�����X����F�����D")
//// �f�X�L���[
//#define	IDS_CHIF_DSKW_HWS_DESKEWING 		_T("�f�X�L���[�F[�n�[�h�V�~�����[�g���[�h]�f�X�L���[���E�E�E")
//#define	IDS_CHIF_DSKW_HWS_DESKEWED			_T("�f�X�L���[�F[�n�[�h�V�~�����[�g���[�h]�f�X�L���[�����D")
//#define	IDS_CHIF_DSKW_FOCUSING				_T("�f�X�L���[�F�t�H�[�J�X���E�E�E")
//#define	IDS_CHIF_DSKW_FOCUS_CANCEL			_T("�f�X�L���[�F�t�H�[�J�X �L�����Z���D")
//#define	IDS_CHIF_DSKW_AUTO_ING				_T("�f�X�L���[�F�I�[�g�f�X�L���[���E�E�E")
//#define	IDS_CHIF_DSKW_AUTO_COMPLETE 		_T("�f�X�L���[�F�I�[�g�f�X�L���[�����D")
//#define	IDS_CHIF_DSKW_AUTO_FAILED			_T("�f�X�L���[�F�I�[�g�f�X�L���[���s�D")
//#define	IDS_CHIF_DSKW_AUTO_CANCEL			_T("�f�X�L���[�F�I�[�g�f�X�L���[ �L�����Z���D")
//#define	IDS_CHIF_DSKW_MANU_ING				_T("�f�X�L���[�F�}�j���A���f�X�L���[���E�E�E")
//#define	IDS_CHIF_DSKW_MANU_COMPLETE 		_T("�f�X�L���[�F�}�j���A���f�X�L���[�����D")
//#define	IDS_CHIF_DSKW_MANU_CANCEL			_T("�f�X�L���[�F�}�j���A���f�X�L���[ �L�����Z���D")
//#define	IDS_CHIF_DSKW_MANU_IGNORE			_T("�f�X�L���[�F�}�j���A���f�X�L���[�����D")
//// �P�|�C���g����
//#define	IDS_CHIF_SR1P_Z_MOVING				_T("�|�C���g����F����ʒu(�y)�ֈړ����E�E�E")
//#define	IDS_CHIF_SR1P_Z_MOVED				_T("�|�C���g����F����ʒu(�y)�ֈړ������D")
//#define	IDS_CHIF_SR1P_FOCUSING				_T("�|�C���g����F�t�H�[�J�X���E�E�E")
//#define	IDS_CHIF_SR1P_FOCUS_ABORTED 		_T("�|�C���g����F�t�H�[�J�X���f�D")
//#define	IDS_CHIF_SR1P_HWS_MEASING			_T("�|�C���g����F[�n�[�h�V�~�����[�g���[�h]���蒆�E�E�E")
//#define	IDS_CHIF_SR1P_MEASING				_T("�|�C���g����F���蒆�E�E�E")
//#define	IDS_CHIF_SR1P_MEAS_FAILED			_T("�|�C���g����F���莸�s�D")
//#define	IDS_CHIF_SR1P_ANALYZING 			_T("�|�C���g����F��͒��E�E�E")
//#define	IDS_CHIF_SR1P_ANALYZE_FAILED		_T("�|�C���g����F��͎��s�D")
//#define	IDS_CHIF_SR1P_DATA_PROCING			_T("�|�C���g����F�f�[�^�������E�E�E")
//#define	IDS_CHIF_SR1P_DATA_PROC_COMPLETE	_T("�|�C���g����F�f�[�^���������D")
//// 2009.05.11 K.Matsuo SitePR -->
//// �T�C�g�p�^�[��
//#define	IDS_CHIF_SITEPR_AUTO_ING			_T("�T�C�g�p�^�[���F�I�[�g�T�C�g�p�^�[�����E�E�E")
//#define	IDS_CHIF_SITEPR_AUTO_COMPLETE		_T("�T�C�g�p�^�[���F�I�[�g�T�C�g�p�^�[�������D")
//#define	IDS_CHIF_SITEPR_AUTO_FAILED 		_T("�T�C�g�p�^�[���F�I�[�g�T�C�g�p�^�[�����s�D")
//#define	IDS_CHIF_SITEPR_AUTO_CANCEL 		_T("�T�C�g�p�^�[���F�I�[�g�T�C�g�p�^�[�� �L�����Z���D")
//#define	IDS_CHIF_SITEPR_MANU_ING			_T("�T�C�g�p�^�[���F�}�j���A���T�C�g�p�^�[�����E�E�E")
//#define	IDS_CHIF_SITEPR_MANU_COMPLETE		_T("�T�C�g�p�^�[���F�}�j���A���T�C�g�p�^�[�������D")
//#define	IDS_CHIF_SITEPR_MANU_CANCEL 		_T("�T�C�g�p�^�[���F�}�j���A���T�C�g�p�^�[�� �L�����Z���D")
//// 2009.05.11 K.Matsuo SitePR <--
//// �V�[�P���X����
//#define	IDS_CHIF_SEQ_STAGE_PREMOVING		_T("�V�[�P���X����F����ʒu(�w�x)�֐�s�ړ����E�E�E")
//#define IDS_CHIF_SEQ_STAGE_MOVED			_T("�V�[�P���X����F����ʒu(�w�x)�ֈړ������D")
//#define IDS_CHIF_SEQ_STAGE_MOVING 		_T("�V�[�P���X����F����ʒu(�w�x)�ֈړ����E�E�E")
//#define IDS_CHIF_SEQ_SWAYSETTLING 		_T("�V�[�P���X����F�w�b�h�h������҂��E�E�E")
//#define IDS_CHIF_SEQ_DATA_PROCING 		_T("�V�[�P���X����F�f�[�^�������E�E�E")
//#define IDS_CHIF_SEQ_DATA_PROC_COMPLETE		_T("�V�[�P���X����F�f�[�^���������D")
//#define IDS_CHIF_SEQ_CANCELING				_T("�V�[�P���X����F�L�����Z����~���E�E�E")
//#define IDS_CHIF_SEQ_CANCELED 			_T("�V�[�P���X����F�L�����Z�������D")
//#define IDS_CHIF_SEQ_PAUSING				_T("�V�[�P���X����F�ꎞ��~��(�|�[�Y)�E�E�E")
//Saiki 20090602 Delete*/

/* added 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- { ---------- */
/*
 *	�����E�B���h�E���b�Z�[�W�`�F�b�N�p ��`
 */
#define	CHIEF_TREVENT_RETRY_COUNT		10		/* WM_CHIF_TRxxxx_SETEVENT ���g���C��	*/
#define	CHIEF_TREVENT_RETRY_INTERVAL	50		/* WM_CHIF_TRxxxx_SETEVENT ���g���C�Ԋu[ms]	*/
#define	CHIEF_CHECK_BUSY_TIME			1000	/* �r�W�[�`�F�b�N����[ms]	*/
#define	CHIEF_CHECK_BUSY_INTERVAL		100		/* �r�W�[�`�F�b�N�Ԋu[ms]	*/
/* added 2012.01.23 hmenjo [�V]����A�{�[�g�΍� ---------- } ---------- */
