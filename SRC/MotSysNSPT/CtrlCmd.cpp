
#include	"stdafx.h"
#include	<windows.h>
#include	"..\\..\\inc\\MotsysMsg.h"
#include	"MotSysDef.h"
#include	"MotSysDlg.h"
#include	"Sio.h"
#include	"SioDef.h"
#include	"APIDIO.h"
#define	MASTER
#include	"CtrlCmd.h"
#include	"CtrlCmdInit.h"
#include	"MotionDef.h"
#include	"Motion.h"

#include	"FileNameDef.h"

#include	"logfile.h"

extern CSio *g_pCSio;

extern UINT MOTDRV_SHOWALARM;		// �A���[���\���p
extern UINT MOTDRV_SRVINITCOMP;		// �T�[�{�������������b�Z�[�W��`

static CMotSysDlg* g_pCMotSysDlg;

/*
 * DIO �֌W
 */
HANDLE	g_CC_hDrv = 0;			// DIO �̃f�o�C�X�n���h��
WORD	g_CC_DrvNo = 0;			// DIO �̃h���C�o�ԍ�
WORD	g_CC_GrpNo = 0;			// DIO �̃O���[�v�ԍ�


// �R���g���[��������(�N���֐�)
BOOL CC_InitServo(LPVOID pParam)
{
	g_pCMotSysDlg = (CMotSysDlg*) pParam;

	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_STATUS, _T("Initializing Controller..."));

	// Drv No. ��\��
	TCHAR l_tszText[32];
	_stprintf(l_tszText, _T("%02u"), g_CC_DrvNo);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_DRVNO, l_tszText);
	// Grp No. ��\��
	_stprintf(l_tszText, _T("%02u"), g_CC_GrpNo);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_GRPNO, l_tszText);

	int l_ret;
	if (0 != (l_ret = CC_Initialize_Controller())) {
		// ���������s
		g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to initialize"));
		if (l_ret == -1) {
			// �����������t���O���O�ɂȂ�Ȃ�����
			g_pCMotSysDlg->ShowAlarm(4, 0, 0, 0);
		} else {
			CC_MsgReboot();
		}
		return FALSE;
	} else {
		g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_STATUS, _T("Initialized Controller"));
		return TRUE;
	}
}
// �R���g���[��������
DWORD CC_Initialize_Controller(void)
{
	char	l_Senddata[256];
	char	l_Recvdata[256];
	int		l_result;
	int		*l_Data;
	int		i;

	// ���[�^�R���g���[���̏�Ԃ��m�F
	strcpy(l_Senddata, "P100");
	if ((l_result = CC_ReadCommand(l_Senddata, l_Recvdata)) < 0) {
		return -2;	// �V�X�e���G���[(��ԓǏo���G���[)
	} else {
		// �������f�[�^���t�@�C������Ǎ���
		if (CC_LoadInitData() != TRUE) {
			return -3;	// �ǂݍ��ݎ��s
		} else {
			// ���ʃf�[�^
			l_Data = (int*) &CC_InitDataCommon;
			for (i = 0; i < (sizeof(CC_InitDataCommon) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 501, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -4;	// �V�X�e���G���[(���ʃf�[�^�����ݎ��s)
				}
			}
			// X �f�[�^
			l_Data = (int*) &CC_Initdata_X;
			for (i = 0; i < (sizeof(CC_Initdata_X) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 600, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -5;	// �V�X�e���G���[(X �f�[�^�����ݎ��s)
				}
			}
			// Y1 �f�[�^
			l_Data = (int*) &CC_Initdata_Y1;
			for (i = 0; i < (sizeof(CC_Initdata_Y1) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 625, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -6;	// �V�X�e���G���[(Y1 �f�[�^�����ݎ��s)
				}
			}
			// Y2 �f�[�^
			l_Data = (int*) &CC_Initdata_Y2;
			for (i = 0; i < (sizeof(CC_Initdata_Y2) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 650, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -7;	// �V�X�e���G���[(Y2 �f�[�^�����ݎ��s)
				}
			}
			// Z �f�[�^
			l_Data = (int*) &CC_Initdata_Z;
			for (i = 0; i < (sizeof(CC_Initdata_Z) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 675, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -8;	// �V�X�e���G���[(Z �f�[�^�����ݎ��s)
				}
			}
			// T �f�[�^
			l_Data = (int*) &CC_Initdata_T;
			for (i = 0; i < (sizeof(CC_Initdata_T) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 700, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -9;	// �V�X�e���G���[(T �f�[�^�����ݎ��s)
				}
			}
			// �����ݒ�l�̕ύX�J�n
			strcpy(l_Senddata, "P500=1");
			if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
				return -10;	// �V�X�e���G���[(�����ݒ�l�̕ύX�J�n���s)
			}
			// �ύX�������Ď�(P500 �� 0 �ɂȂ�̂��܂�)
			int l_Loop = FALSE;
			DWORD l_StartTime = GetTickCount();
			DWORD l_CurrentTime, l_DelayTime;
			strcpy(l_Senddata, "P500");
			while (l_Loop == FALSE) {
				if ((l_result = CC_ReadCommand(l_Senddata, l_Recvdata)) < 0) {
					return -11;	// �V�X�e���G���[(�ύX�����Ǎ��ݎ��s)
				}
				if (strcmp(l_Recvdata, "0") == 0) {
					l_Loop = TRUE;	// 0 �ɂȂ���
				} else {
					// ��莞�Ԉȓ��� 0 �ɂȂ�Ȃ��Ȃ�ُ�
					l_CurrentTime = GetTickCount();
					if (l_StartTime <= l_CurrentTime) {
						l_DelayTime = l_CurrentTime - l_StartTime;
					} else {
						l_DelayTime = (((DWORD) 0xffffffff) - l_StartTime) + l_CurrentTime;
					}
					if (CC_INIT_TIMEOUT < l_DelayTime) {
						return -1;	// �V�X�e���G���[(�ύX�����^�C���A�E�g)
					}
				}
			}
		}
	}

	// �T�[�{�p�����^�Ǐo��
	if (CC_LoadServoParam() != TRUE) {
		return FALSE;	// �V�X�e���G���[
	}

	// �N���O�̏�ԓǏo��(1��̂�)
	while (1) {
		if (CC_PollingStatus() == 0) {
			break;
		}
	}

	// ����������
	g_CC_bInitialize = TRUE;

	return 0;
}

// �������f�[�^���t�@�C������Ǎ���
BOOL CC_LoadInitData(void)
{
	BOOL	l_rc = FALSE;
	FILE	*fp;
	TCHAR	l_tszFile[_MAX_PATH];
	TCHAR	l_tszText[256];
	char	l_cBuff[256];
	char	l_cBuff2[256];
	unsigned short	i, ii;
	BOOL	l_chkOK = TRUE;

	_tcscpy(l_tszFile, CC_INITDATA_PATH);
	_tcscat(l_tszFile, CC_INITDATA_FILE);
	if ((fp = _tfopen(l_tszFile, "r")) != NULL) {
		ii = 0;
		while (fgets(l_cBuff, 255, fp) != NULL) {
			// ����R�[�h���������� NULL �ɂ���
			for (i = 0; i < strlen(l_cBuff); i++) {
				if (iscntrl(l_cBuff[i]) != 0) {
					l_cBuff[i] = 0x00;
					break;
				}
			}
			// �f�[�^��؏o��
			strcpy(l_cBuff2, &(l_cBuff[5]));
			// ���������`�F�b�N
			if (10 < strlen(l_cBuff2)) {
				break;
			}
			// �������ǂ����`�F�b�N
			for (i = 0; i < strlen(l_cBuff2); i++) {
// '-'��OK				if (isdigit(l_cBuff2[i]) == 0) {
				if ((isdigit(l_cBuff2[i]) == 0) && (l_cBuff2[i] != '-')) {
					l_chkOK = FALSE;
					break;
				}
				if ((i != 0) && (l_cBuff2[i] == '-')) {
					l_chkOK = FALSE;
					break;
				}
			}
			if (l_chkOK == FALSE) {
				break;
			}
			CC_InitALL[ii] = atol(l_cBuff2);
			ii++;
		}
		fclose(fp);
		if ((ii == 299) && (l_chkOK == TRUE)) {
			l_rc = TRUE;
		}
	}
	if (l_rc == FALSE) {
		// �t�@�C�����������������̂Ő��^���쐬
		if ((fp = _tfopen(l_tszFile, "w")) != NULL) {
			memset(CC_InitALL, 0, sizeof(CC_InitALL));
			for (i = 0; i < (sizeof(CC_InitALL) / sizeof(int)); i++) {
				sprintf(l_cBuff, "P%03d %d\n", i + 501, CC_InitALL[i]);
				fputs(l_cBuff, fp);
			}
			fclose(fp);
			_stprintf(l_tszText, _T("Not found init data template file[%s]."), CC_INITDATA_FILE);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
		} else {
			// ���^�����Ȃ�����
			_stprintf(l_tszText, _T("Could not create init data template file[%s]."), CC_INITDATA_FILE);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
		}
	} else {
		memcpy(&CC_InitDataCommon,	&(CC_InitALL[501 - 501]), sizeof(CC_InitDataCommon));	// P501-543
		memcpy(&CC_Initdata_X,		&(CC_InitALL[600 - 501]), sizeof(CC_Initdata_X));		// P600-614
		memcpy(&CC_Initdata_Y1,		&(CC_InitALL[625 - 501]), sizeof(CC_Initdata_Y1));		// P625-639
		memcpy(&CC_Initdata_Y2,		&(CC_InitALL[650 - 501]), sizeof(CC_Initdata_Y2));		// P650-664
		memcpy(&CC_Initdata_Z,		&(CC_InitALL[675 - 501]), sizeof(CC_Initdata_Z));		// P675-689
		memcpy(&CC_Initdata_T,		&(CC_InitALL[700 - 501]), sizeof(CC_Initdata_T));		// P700-714
	}

	return l_rc;
}
// �T�[�{�p�����^���t�@�C������Ǐo��
BOOL CC_LoadServoParam(void)
{
	BOOL	l_rc = FALSE;

	TCHAR	l_tszPathName[MAX_PATH];
	FILE	*fp;
	TCHAR	l_tszText[256];

	_tcscpy(l_tszPathName, SERVOPARAMNAME);

	if ((fp = _tfopen(l_tszPathName, "r")) != NULL) {
		fread(ServoParam, sizeof(ServoParam), 1, fp);
		fclose(fp);
		l_rc = TRUE;
	} else {
		// �t�@�C������������
		l_rc = FALSE;
		_stprintf(l_tszText, _T("Could not open Servo Parameter file[%s]."), l_tszPathName);
		if (0 == g_pCMotSysDlg->m_bOnClosing) {
			::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}

	// X ���̍ő呬�x����
	::GetPrivateProfileString(_T("MaxSpeed"), _T("BigXMaxSpeed"), _T("2000"), l_tszText, sizeof(l_tszText), SERVO_INI);
	g_CC_MaxSpeed[CC_AXIS_X] = _ttol(l_tszText);
	if (CC_InitDataCommon.P534 < g_CC_MaxSpeed[CC_AXIS_X]) {g_CC_MaxSpeed[CC_AXIS_X] = CC_InitDataCommon.P534;}
	if (g_CC_MaxSpeed[CC_AXIS_X] < 10) {g_CC_MaxSpeed[CC_AXIS_X] = 10;}

	// Y ���̍ő呬�x����
	::GetPrivateProfileString(_T("MaxSpeed"), _T("BigYMaxSpeed"), _T("2000"), l_tszText, sizeof(l_tszText), SERVO_INI);
	g_CC_MaxSpeed[CC_AXIS_Y] = _ttol(l_tszText);
	if (CC_InitDataCommon.P535 < g_CC_MaxSpeed[CC_AXIS_Y]) {g_CC_MaxSpeed[CC_AXIS_Y] = CC_InitDataCommon.P535;}
	if (g_CC_MaxSpeed[CC_AXIS_Y] < 10) {g_CC_MaxSpeed[CC_AXIS_Y] = 10;}

	// Z ���̍ő呬�x����
	g_CC_MaxSpeed[CC_AXIS_Z] = CC_InitDataCommon.P536;

	// T ���̍ő呬�x����
	::GetPrivateProfileString(_T("MaxSpeed"), _T("ThetaMaxSpeed"), _T("10"), l_tszText, sizeof(l_tszText), SERVO_INI);
	g_CC_MaxSpeed[CC_AXIS_T] = _ttol(l_tszText);
	if (10 < g_CC_MaxSpeed[CC_AXIS_T]) {g_CC_MaxSpeed[CC_AXIS_T] = 10;}
	if (g_CC_MaxSpeed[CC_AXIS_T] <	5) {g_CC_MaxSpeed[CC_AXIS_T] =	5;}

	return l_rc;
}

// ���[�h�R�}���h
int CC_ReadCommand(char *SendData, char *RecvData)
{
	int				l_retry = 0;
	int				l_result = 0;
	TCHAR			l_tszText[256];
	int				i;
	unsigned char	l_Sum;
	DWORD			l_RecvLength;
	static BOOL		ls_OnceError = TRUE;	// �ŏ��̒ʐM����������܂Ŗ������g���C�ɂ���t���O
												/*	��L�� FP �V���[�Y�̏ꍇ�D
													Nano �ł͉��L�� #if �ɂāC����̃��g���C�̂�
													�w�胊�g���C�񐔂̂P�O�{�̃��g���C��
													�s�Ȃ��悤�ɕύX���܂����D
												*/
	int				l_iRetryTimes;

	while (1) {
//if (0 == g_CC_ThreadLoop) {return 0x80000000;}
if ((0 == g_CC_ThreadLoop) || (0 != g_CC_CommAbort)) {l_retry = 0; return 0x80000000;}
		l_result = g_pCSio->CommCommand(SendData, RecvData, &l_RecvLength);
		if ((l_result & 0x10000000) != 0) {RETRY_TIMES;
			// �V�X�e���G���[����
			send_motion_message(-1, 111);	// �ʐM(Read)�V�X�e���G���[����
			_stprintf(l_tszText, _T("Communication Functions is not work(ReadCommand:0=0x%08x)."), l_result);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
			break;;
		} else {
			if (l_result != MSC_COMM_OK) {
				// ���g���C
#if 0
				if ((ls_OnceError != TRUE) && (RETRY_TIMES <= l_retry++)) {
#else
				l_iRetryTimes = (0 != ls_OnceError)? RETRY_TIMES * 10: RETRY_TIMES;
				if ((l_iRetryTimes <= l_retry++)) {
#endif
					// ���g���C�A�E�g�̓V�X�e���G���[
					send_motion_message(-1, 112);	// �ʐM(Read)���g���C�A�E�g�P����
					l_result = -1;
					_stprintf(l_tszText, _T("Communication retry out(ReadCommand:1=0x%08x). Retry times : %d."), l_result, l_iRetryTimes);
					if (0 == g_pCMotSysDlg->m_bOnClosing) {
						::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
					}
					break;
				}
			} else {
				// ���퉞��
				ls_OnceError = FALSE; l_retry = 0;
				// ���[�h�f�[�^���o
				for (i = 0; i < 255; i++) {
					if (RecvData[i] == COM_CR) {
						l_Sum = RecvData[i + 1];
						RecvData[i + 1] = 0x00;
						// �T���̃`�F�b�N
						if (l_Sum != g_pCSio->CalcCheckSum(RecvData, 1)) {
							// �T���������
							l_result = -3;
						}
						if (12 < strlen(RecvData)) {
							// �f�[�^����������(10�����𒴂��Ă���)
							l_result = -2;
						}
						RecvData[i] = 0x00;
						break;
					}
				}
				if ((255 <= i) || (l_result != MSC_COMM_OK)) {
					// ���g���C
					if ((ls_OnceError != TRUE) && (RETRY_TIMES <= l_retry++)) {
						// ���g���C�A�E�g�̓V�X�e���G���[
						send_motion_message(-1, 113);	// �ʐM(Read)���g���C�A�E�g�Q����
						l_result = (l_result & 0x8000ffff) | 0xa0000000;
						_stprintf(l_tszText, _T("Communication retry out(ReadCommand:2=0x%08x). Retry times : %d."), l_result, RETRY_TIMES);
						if (0 == g_pCMotSysDlg->m_bOnClosing) {
							::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
						}
						break;
					}
				} else {
					break;
				}
			}
		}
	}

	return l_result;
}
// ���[�h�R�}���h�Q
int CC_ReadCommand2(
					char *SendData,		// ���M�f�[�^
					char *RecvData,		// ��M�f�[�^
					short DataNum,		// �f�[�^��
					void *ReadData		// ���[�h�f�[�^
				)
{
	int				l_retry = 0;
	int				l_result = 0;
	TCHAR			l_tszText[256];
	int				i, j, ii;
	unsigned short	k;
	unsigned char	l_Sum;
	char			(*l_pReadData)[16][12];
	char			l_pRecvData[256];
	DWORD			l_RecvLength;

	l_pReadData = (char (*)[16][12]) ReadData;

	while (1) {
//if (0 == g_CC_ThreadLoop) {return 0x80000000;}
if ((0 == g_CC_ThreadLoop) || (0 != g_CC_CommAbort)) {l_retry = 0; return 0x80000000;}
		l_result = g_pCSio->CommCommand(SendData, RecvData, &l_RecvLength);
		if ((l_result & 0x10000000) != 0) {
			// �V�X�e���G���[����
			send_motion_message(-1, 121);	// �ʐM(Read2)�V�X�e���G���[����
			_stprintf(l_tszText, _T("Communication Functions is not work(ReadCommand2:0=0x%08x)."), l_result);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
			break;;
		} else {
			if (l_result != MSC_COMM_OK) {
				// ���g���C
				if (RETRY_TIMES <= l_retry++) {
					// ���g���C�A�E�g�̓V�X�e���G���[
					send_motion_message(-1, 122);	// �ʐM(Read2)���g���C�A�E�g�P����
					l_result = -1;
					_stprintf(l_tszText, _T("Communication retry out(ReadCommand2:1=0x%08x). Retry times : %d."), l_result, RETRY_TIMES);
					if (0 == g_pCMotSysDlg->m_bOnClosing) {
						::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
					}
					break;
				}
			} else {
				// ���퉞��
				memcpy(l_pRecvData, RecvData, l_RecvLength);
				// ���[�h�f�[�^���o
				j = 0; ii = 0;
				for (i = 0; i < 256; i++) {
					if (l_pRecvData[i] == COM_CR) {
						l_Sum = l_pRecvData[i + 1];
						l_pRecvData[i + 1] = 0x00;
						strcpy(&((*l_pReadData)[j][0]), &(l_pRecvData[ii]));
						ii = i + 2;
						i = i + 1;
						// �T���̃`�F�b�N
						if (l_Sum != g_pCSio->CalcCheckSum(&((*l_pReadData)[j][0]), 1)) {
							// �T���������
							l_result = -3; break;
						}
						if (12 < strlen(&((*l_pReadData)[j][0]))) {
							// �f�[�^����������(10�����𒴂��Ă���)
							l_result = -2; break;
						}
						// CR �� NULL �ɂ���(�I�[��ǉ�)
						for (k = 0; k < strlen(&((*l_pReadData)[j][0])); k++) {
							if ((*l_pReadData)[j][k] == COM_CR) {
								(*l_pReadData)[j][k] = 0x00; break;
							}
						}
						j++;
						if (DataNum <= j) {
							if (l_pRecvData[ii] != COM_ACK) {
								// �f�[�^����������
								l_result = -4; break;
							} else {
								break;
							}
						}
					}
				}
				if ((255 <= i) || (l_result != MSC_COMM_OK)) {
					// ���g���C
					if (RETRY_TIMES <= l_retry++) {
						// ���g���C�A�E�g�̓V�X�e���G���[
						send_motion_message(-1, 123);	// �ʐM(Read2)���g���C�A�E�g�Q����
						l_result = (l_result & 0x8000ffff) | 0xa0000000;
						_stprintf(l_tszText, _T("Communication retry out(ReadCommand2:2=0x%08x). Retry times : %d."), l_result, RETRY_TIMES);
						if (0 == g_pCMotSysDlg->m_bOnClosing) {
							::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
						}
						break;
					}
				} else {
					break;
				}
			}
		}
	}

	return l_result;
}

// ���C�g�R�}���h
int CC_WriteCommand(char *SendData, char *RecvData)
{
	int		l_retry = 0;
	int		l_result;
	TCHAR	l_tszText[256];
	DWORD	l_RecvLength;

	while (1) {
//if (0 == g_CC_ThreadLoop) {return 0x80000000;}
if ((0 == g_CC_ThreadLoop) || (0 != g_CC_CommAbort)) {l_retry = 0; return 0x80000000;}
		l_result = g_pCSio->CommCommand(SendData, RecvData, &l_RecvLength);
		if ((l_result & 0x10000000) != 0) {
			// �V�X�e���G���[����
			send_motion_message(-1, 131);	// �ʐM(Write)�V�X�e���G���[����
			_stprintf(l_tszText, _T("Communication Functions is not work. (0x%08x)"), l_result);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
			break;;
		} else {
			if (l_result != MSC_COMM_OK) {
				// ���g���C
				if (RETRY_TIMES <= l_retry++) {
					// ���g���C�A�E�g�̓V�X�e���G���[
					send_motion_message(-1, 132);	// �ʐM(Write)���g���C�A�E�g����
					l_result = -1;
					_stprintf(l_tszText, _T("Communication retry out(WriteCommand:1). Retry times : %d."), RETRY_TIMES);
					if (0 == g_pCMotSysDlg->m_bOnClosing) {
						::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
					}
					break;
				}
			} else {
				// ���퉞��
				break;;
			}
		}
	}

	return l_result;
}

// DIO �I�[�v��
DWORD CC_DioOpen(void)
{
	DWORD	dwRet;

	g_CC_DrvNo = ::GetPrivateProfileInt(_T("DIO"), _T("DrvNo"), 2, SERVO_INI);
	g_CC_GrpNo = ::GetPrivateProfileInt(_T("DIO"), _T("GrpNo"), 1, SERVO_INI);

	dwRet = DioOpen(&g_CC_hDrv, g_CC_DrvNo, g_CC_GrpNo);

	return dwRet;
}
// DIO �N���[�Y
DWORD CC_DioClose(void)
{
	DWORD	dwRet;

	if (g_CC_hDrv == NULL) {
		// �I�[�v������Ă��Ȃ�
		return -1;
	}

	dwRet = DioClose(g_CC_hDrv);
	g_CC_hDrv = NULL;

	return dwRet;
}
// DIO �|�[�g���[�h
DWORD CC_DioInpDWord(DWORD *pdwRData)
{
	DWORD	l_dwRet = 0;
	BYTE	l_ucInPort0, l_ucInPort1, l_ucInPort2, l_ucInPort3;

	if (g_CC_hDrv == NULL) {
		// �I�[�v������Ă��Ȃ�
		return -1;
	}

	l_dwRet = DioInpByte(g_CC_hDrv, 0, &l_ucInPort0);
	if (0 == l_dwRet) {
		l_dwRet = DioInpByte(g_CC_hDrv, 1, &l_ucInPort1);
	}
	if (0 == l_dwRet) {
		l_dwRet = DioInpByte(g_CC_hDrv, 2, &l_ucInPort2);
	}
	if (0 == l_dwRet) {
		l_dwRet = DioInpByte(g_CC_hDrv, 3, &l_ucInPort3);
	}
	if (0 == l_dwRet) {
		*pdwRData = g_CC_InPort =	(((DWORD) l_ucInPort3) << 24)
								  | (((DWORD) l_ucInPort2) << 16)
								  | (((DWORD) l_ucInPort1) <<  8)
								  | (((DWORD) l_ucInPort0));
	} else {
		*pdwRData = g_CC_InPort;
	}
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
	CC_DioLog(g_CC_InPort, g_CC_OutPort);
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */

	return l_dwRet;
}
// DIO �r�b�g���[�h
DWORD CC_DioInpBit(WORD wRBitNo, BYTE *pucRBitData)
{
	DWORD	l_dwRet = 0;
	BYTE	l_ucBitData;

	if (g_CC_hDrv == NULL) {
		// �I�[�v������Ă��Ȃ�
		return -1;
	}
	if (31 < wRBitNo) {
		// �r�b�g�ԍ����s��
		return -1;
	}

	l_dwRet = DioInpBit(g_CC_hDrv, wRBitNo, &l_ucBitData);
	if (0 == l_dwRet) {
		l_ucBitData = (0 != l_ucBitData)? 1 : 0;
		*pucRBitData = l_ucBitData;
		if (0 != l_ucBitData) {
			g_CC_InPort |=	 ((DWORD) l_ucBitData) << wRBitNo;
		} else {
			g_CC_InPort &= ~(((DWORD) l_ucBitData) << wRBitNo);
		}
	} else {
		if (0 != (g_CC_InPort & (0x00000001 << wRBitNo))) {
			*pucRBitData = 1;
		} else {
			*pucRBitData = 0;
		}
	}
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
	CC_DioLog(g_CC_InPort, g_CC_OutPort);
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */

	return l_dwRet;
}
// DIO �r�b�g���C�g
DWORD CC_DioOutBit(WORD wWBitNo, BYTE ucWBitData)
{
	DWORD	l_dwRet = 0;

	if (g_CC_hDrv == NULL) {
		// �I�[�v������Ă��Ȃ�
		return -1;
	}
	if (31 < wWBitNo) {
		// �r�b�g�ԍ����s��
		return -1;
	}

	ucWBitData = (0 != ucWBitData)? 1 : 0;
	l_dwRet = DioOutBit(g_CC_hDrv, wWBitNo, ucWBitData);
	if (0 == l_dwRet) {
		if (0 != ucWBitData) {
			g_CC_OutPort |=   ((DWORD) 0x00000001) << wWBitNo;
		} else {
			g_CC_OutPort &= ~(((DWORD) 0x00000001) << wWBitNo);
		}
	}
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
	CC_DioLog(g_CC_InPort, g_CC_OutPort);
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */

	return l_dwRet;
}
// DIO �G���[
void CC_DioError(void)
{
	static BOOL	ls_Error = FALSE;

	if (ls_Error == FALSE) {
		ls_Error = TRUE;
		CC_ServoOFF_RunOFF();
		CC_ServoFailure();
		g_pCMotSysDlg->ShowAlarm(5, 0, 0, 0);
	}
}
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
/* DIO ���O	*/
void CC_DioLog(DWORD dwDi, DWORD dwDo)
{
	static DWORD ls_dwDiPre = 0;
	static DWORD ls_dwDoPre = 0;

	if (0 != g_pCMotSysDlg) {
		if ((ls_dwDiPre != dwDi) || (ls_dwDoPre != dwDo)) {
			/* �ω��L��	*/
			ls_dwDiPre = dwDi;
			ls_dwDoPre = dwDo;
			g_pCMotSysDlg->DioLogging(dwDi, dwDo);
		}
	}
}
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */

// WD �Ď�
BOOL	lg_bWD_once = FALSE;
BOOL CC_checkWD(void)
{
	BOOL	l_rc = TRUE;
	BYTE	l_BitData = 0;

	if (g_CC_bInitialize != TRUE) {
		// ���������Ȃ�Ď����Ȃ��D���탊�^�[���Ƃ���
	} else {
		// ��x�ł��I���������H
		if (lg_bWD_once != TRUE) {
			// ���Ă��Ȃ�
			// �I����҂�
			if (CC_DioInpBit(0, &l_BitData) != 0) {
				CC_DioError(); l_BitData = 0;
			}
			if (l_BitData != 0) {
				lg_bWD_once = TRUE;
			}
		} else {
			// ����
			if (CC_DioInpBit(0, &l_BitData) != 0) {
				CC_DioError(); l_BitData = 0;
			}
			if (l_BitData == 0) {
				// �I�t������G���[
				l_rc = FALSE;
				lg_bWD_once = FALSE;
			}
		}
	}

	return l_rc;
}

// �T�[�{ ON/OFF
BOOL CC_ServoONOFF(int on_off)
{
	BYTE	l_ucServo;

	if (on_off == -1) {
		// ��Ԃ�Ԃ�����
	} else if (on_off == TRUE) {
		// �T�[�{ ON
		if (CC_DioOutBit(CC_OUTBIT_SERVO, 1) != 0) {
			CC_DioError();	return FALSE;	// ���s
		}
		g_CC_ServoONReq = TRUE;
		g_CC_ServoOFFReq = FALSE;
	} else {
		// �T�[�{ OFF
		if (CC_DioOutBit(CC_OUTBIT_SERVO, 0) != 0) {
			CC_DioError();	return FALSE;	// ���s
		}
		g_CC_ServoONReq = FALSE;
		g_CC_ServoOFFReq = TRUE;
	}

	// ��Ԃ�Ǐo��
	if (CC_DioInpBit(CC_INBIT_SERVO, &l_ucServo) != 0) {
		CC_DioError();	return FALSE;	// ���s
	}

	return (l_ucServo != 0)? TRUE : FALSE;
}

// ���u�[�g���Ȃ����̃��b�Z�[�W
void CC_MsgReboot(void)
{
	g_CC_ComError = TRUE;
	CC_ServoOFF_RunOFF();
	CC_ServoFailure();
	g_pCMotSysDlg->ShowAlarm(2, 0, 0, 0);
}

// ���������
UINT CC_Main_Loop(LPVOID pParam)
{
	// �ϐ����̏�����
	CC_Initial();

	// DO ���N���A
	for (int i = 0; i < 32; i++) {
		CC_DioOutBit(i, 0);
	}
	// �T�[�{���j�b�g������
	g_CC_ThreadLoop = TRUE;
	g_CC_CommAbort = FALSE;
	BOOL l_bInitRc = CC_InitServo(pParam);
	if (0 == l_bInitRc) {
		g_CC_ThreadLoop = FALSE;
		g_pCMotSysDlg->PostMessage(MOTDRV_SRVINITCOMP, (WPARAM) FALSE, 0);
	} else {
		g_CC_ThreadLoop = TRUE;
		CC_ChangeGain(CC_AXIS_X, 1);	// �Q�C����؊���
		CC_ChangeGain(CC_AXIS_Y, 1);	// �Q�C����؊���
		CC_ChangeGain(CC_AXIS_Z, 1);	// �Q�C����؊���
		CC_ChangeGain(CC_AXIS_Y, 1);	// �Q�C����؊���
		g_pCMotSysDlg->PostMessage(MOTDRV_SRVINITCOMP, (WPARAM) TRUE, 0);
	}

	while (g_CC_ThreadLoop == TRUE) {
/* modified 2009.10.15 hmenjo �^�N�g�Z�k�p�X���[�v���ԏk�� ---------- { ---------- */
////		::Sleep(200);	// FP �V���[�Y�ł͂�����������
//		::Sleep(100);
/* modified 2009.10.15 hmenjo �^�N�g�Z�k�p�X���[�v���ԏk�� ----------			   */
		::Sleep(50);
/* modified 2009.10.15 hmenjo �^�N�g�Z�k�p�X���[�v���ԏk�� ---------- } ---------- */

		if (g_CC_ComError != TRUE) {
			// �R���g���[���� WD �`�F�b�N
			if (CC_checkWD() != TRUE) {
				CC_ServoFailure();
				CC_ServoONOFF(0);
				g_pCMotSysDlg->ShowAlarm(1, 0, 0, 0);
			}
			// �A���[���Ď�
			CC_PollingAlarm();
			// ��Ԏ擾
			CC_PollingStatus();
			// ������Ǘ�

#define	BEFORE_INP_WAIT	200	// INP �҂�����
			CC_X_Manager();		// X ��
			CC_Y_Manager();		// Y ��
			CC_Z_Manager();		// Z ��
			CC_T_Manager();		// T ��

#if CC_DEB_BUTTON==0
			mmtestORGXdone();	//debdeb for test
			mmtestORGYdone();	//debdeb for test
			mmtestORGZdone();	//debdeb for test
			mmtestORGTdone();	//debdeb for test
#endif
		}
		// �T�[�{�I������
		CC_ServoON_Ctrl();
	}

	// DO ���N���A
	for (int i = 0; i < 32; i++) {
		CC_DioOutBit(i, 0);
	}

//	AfxEndThread(0);	// ����łȂ��āC
	return (UINT) 0;	// �������ł悢
}

// �T�[�{�I������
void CC_ServoON_Ctrl(void)
{
	static WORD		l_prc = 0;
	static DWORD	ls_StartTime;
	DWORD			l_CurrentTime;
	DWORD			l_DelayTime;
	DWORD			l_InpAlarm;

	switch (l_prc) {
	case 0:
		if (g_CC_ServoONReq == TRUE) {
			// �T�[�{ ON �v������
			if (CC_DioInpDWord(&l_InpAlarm) != 0) {
				CC_DioError();	// ���s
			} else {
				DWORD l_dwAlarmBit = 0;
				l_dwAlarmBit |= (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_X)].bEnable)? 0x00000001 << CC_INBIT_ALARM_X : 0;
				l_dwAlarmBit |= (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Y)].bEnable)? 0x00000001 << CC_INBIT_ALARM_Y : 0;
				l_dwAlarmBit |= (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Z)].bEnable)? 0x00000001 << CC_INBIT_ALARM_Z : 0;
				l_dwAlarmBit |= (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_T)].bEnable)? 0x00000001 << CC_INBIT_ALARM_T : 0;
//				if (
//					((~l_InpAlarm) & ((0x00000001 << CC_INBIT_ALARM_X)
//									| (0x00000001 << CC_INBIT_ALARM_Y)
//									| (0x00000001 << CC_INBIT_ALARM_Z)
//									| (0x00000001 << CC_INBIT_ALARM_T))) == 0
//					) {
				if (((~l_InpAlarm) & l_dwAlarmBit) == 0	) {
					// �A���[��������
					l_prc = 1;
				}
			}
		} else if (g_CC_ServoOFFReq == TRUE) {
			l_prc = 3;	// �T�[�{ OFF �v������
		} else if (((g_CC_OutPort & (0x0001 << CC_OUTBIT_SERVO)) != 0)
				&& (CC_ServoONOFF(-1) == FALSE)) {
			// �T�[�{ ON �w�� ON �Ȃ̂� OFF ���Ă���(EMG ��������)
			CC_ServoOFF_RunOFF();	// �T�[�{ OFF�C�S����~
			CC_ServoFailure();
			// ����~�\��
			g_pCMotSysDlg->ShowAlarm(7, 0, 0, 0);
		}
		break;
	case 1:		// �T�[�{ ON ����
		if (CC_ServoONOFF(-1) == TRUE) {
			// �T�[�{ ON ���Ă���
			g_CC_ServoONReq = FALSE;
			l_prc = 0;
		} else {
			// �T�[�{ ON ���Ă��Ȃ�
			if ((g_CC_OutPort & (0x0001 << CC_OUTBIT_SERVO)) != 0) {
				// �T�[�{ ON �o�͂��Ă���
			} else {
				// �T�[�{ ON �o�͂��Ă��Ȃ�
				CC_ServoONOFF(1);	// �T�[�{ ON
			}
			ls_StartTime = GetTickCount();
			l_prc = 2;
		}
		break;
	case 2:		// �T�[�{ ON �m��҂�
		if (CC_ServoONOFF(-1) == TRUE) {
			// �T�[�{ ON �m��
			g_CC_ServoONReq = FALSE;
			l_prc = 0;
		} else {
			// ��莞�Ԉȓ��� ON ���Ȃ��Ȃ�ُ�
			l_CurrentTime = GetTickCount();
			if (ls_StartTime <= l_CurrentTime) {
				l_DelayTime = l_CurrentTime - ls_StartTime;
			} else {
				l_DelayTime = (((DWORD) 0xffffffff) - ls_StartTime) + l_CurrentTime;
			}
			if (CC_SERVO_ONOFF_TIMEOUT < l_DelayTime) {
				CC_ServoOFF_RunOFF();	// �T�[�{ OFF�C�S����~
				CC_ServoFailure();
				g_CC_ServoONOFFerr = TRUE;
				l_prc = 5;
				g_pCMotSysDlg->ShowAlarm(6, 0, 1, 0);	// �V�X�e���G���[
			}
		}
		break;
	case 3:		// �T�[�{ OFF ����
		if (CC_ServoONOFF(-1) == FALSE) {
			// �T�[�{ OFF ���Ă���
			g_CC_ServoOFFReq = FALSE;
			l_prc = 0;
		} else {
			// �T�[�{ OFF ���Ă��Ȃ�
			if ((g_CC_OutPort & (0x0001 << CC_OUTBIT_SERVO)) == 0) {
				// �T�[�{ OFF �o�͂��Ă���
			} else {
				// �T�[�{ OFF �o�͂��Ă��Ȃ�
				CC_ServoOFF_RunOFF();
			}
			ls_StartTime = GetTickCount();
			l_prc = 4;
		}
		break;
	case 4:		// �T�[�{ OFF �m��҂�
		if (CC_ServoONOFF(-1) == FALSE) {
			// �T�[�{ OFF �m��
			g_CC_ServoOFFReq = FALSE;
			l_prc = 0;
		} else {
			// ��莞�Ԉȓ��� OFF ���Ȃ��Ȃ�ُ�
			l_CurrentTime = GetTickCount();
			if (ls_StartTime <= l_CurrentTime) {
				l_DelayTime = l_CurrentTime - ls_StartTime;
			} else {
				l_DelayTime = (((DWORD) 0xffffffff) - ls_StartTime) + l_CurrentTime;
			}
			if (CC_SERVO_ONOFF_TIMEOUT < l_DelayTime) {
				CC_ServoOFF_RunOFF();	// �T�[�{ OFF�C�S����~
				CC_ServoFailure();
				g_CC_ServoONOFFerr = TRUE;
				l_prc = 5;
				g_pCMotSysDlg->ShowAlarm(6, 0, 0, 0);	// �V�X�e���G���[
			}
		}
		break;
	case 5:		// �T�[�{ ON �^�C���A�E�g�G���[�����҂�
		if (g_CC_ServoONOFFerr == FALSE) {
			l_prc = 0;
		}
		break;
	}
}

// ��Ԏ擾
WORD CC_PollingStatus(void)
{
	char	l_Recvdata[256];
	TCHAR	l_tszText1[32];
	char	l_szText1[32];
	int		l_result;
	WORD	l_AxisNo;			// ���ԍ�
	static WORD	ls_AxisLoop = 1;	// ���ԍ�
	char	l_ReadData[16][12];		// ���[�h�f�[�^
									/* �Y��	X ��		YM ��		Z ��		T ��		YS ��
										0	���샂�[�h	���샂�[�h	���샂�[�h	���샂�[�h	�ʒu
										1	�ʒu		�ʒu		�ʒu		�ʒu		���x
										2	���x		���x		���x		���x		�d���l
										3	�d���l		�d���l		�w�߈ʒu	�w�߈ʒu	�w�߈ʒu
										4	�w�߈ʒu	�w�߈ʒu	�w�ߑ��x	�w�ߑ��x	�w�ߑ��x
										5	�w�ߑ��x	�w�ߑ��x	���		���		�w�ߓd���l
										6	�w�ߓd���l	�w�ߓd���l	�A���[��	�A���[��	���
										7	���		���		�ʒu�΍�	�ʒu�΍�	�A���[��
										8	�A���[��	�A���[��							�ʒu�΍�
										9	�ʒu�΍�	�ʒu�΍�
									 */
	{
		switch (l_AxisNo = ls_AxisLoop++) {
		case CC_AXIS_X:	// X ��
		case CC_AXIS_Y:	// YM ��
			if (0 != motion_data[CC_CnvAxisFromSPT(l_AxisNo)].bEnable) {
				sprintf(l_szText1, "P%1u50..%1u59", l_AxisNo + 1, l_AxisNo + 1);
				if ((l_result = CC_ReadCommand2(l_szText1, l_Recvdata, 10, l_ReadData)) < 0) {
					CC_MsgReboot();	return ls_AxisLoop;	// �V�X�e���G���[
				}
				g_CC_AxisInfo[l_AxisNo].Mode = atoi(l_ReadData[0]);				// ���샂�[�h
				g_CC_AxisInfo[l_AxisNo].Position = atol(l_ReadData[1]);			// �ʒu
				g_CC_AxisInfo[l_AxisNo].Velocity = atol(l_ReadData[2]);			// ���x
				g_CC_AxisInfo[l_AxisNo].Current = atol(l_ReadData[3]);			// �d���l
				g_CC_AxisInfo[l_AxisNo].Order_Position = atol(l_ReadData[4]);	// �w�߈ʒu
				g_CC_AxisInfo[l_AxisNo].Order_Velocity = atol(l_ReadData[5]);	// �w�ߑ��x
				g_CC_AxisInfo[l_AxisNo].Order_Current = atol(l_ReadData[6]);	// �w�ߓd���l
				g_CC_AxisInfo[l_AxisNo].Status = atoi(l_ReadData[7]);			// ���
				g_CC_AxisInfo[l_AxisNo].d_Position = atoi(l_ReadData[9]);		// �ʒu�΍�
				g_pCMotSysDlg->DispdPos(CC_CnvAxisFromSPT(l_AxisNo), g_CC_AxisInfo[l_AxisNo].d_Position);
				g_pCMotSysDlg->CnvWordToBin(g_CC_AxisInfo[l_AxisNo].Status, l_tszText1, 2);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[1][CC_CnvAxisFromSPT(l_AxisNo)], l_tszText1);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[2][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[0]);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[3][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[1]);
			}
			break;
		case CC_AXIS_Z:	// Z ��
		case CC_AXIS_T:	// T ��
			if (0 != motion_data[CC_CnvAxisFromSPT(l_AxisNo)].bEnable) {
				sprintf(l_szText1, "P%1u50..%1u57", l_AxisNo + 1, l_AxisNo + 1);
				if ((l_result = CC_ReadCommand2(l_szText1, l_Recvdata, 8, l_ReadData)) < 0) {
					CC_MsgReboot();	return ls_AxisLoop;	// �V�X�e���G���[
				}
				g_CC_AxisInfo[l_AxisNo].Mode = atoi(l_ReadData[0]);				// ���샂�[�h
				g_CC_AxisInfo[l_AxisNo].Position = atol(l_ReadData[1]);			// �ʒu
				g_CC_AxisInfo[l_AxisNo].Velocity = atol(l_ReadData[2]);			// ���x
				g_CC_AxisInfo[l_AxisNo].Order_Position = atol(l_ReadData[3]);	// �w�߈ʒu
				g_CC_AxisInfo[l_AxisNo].Order_Velocity = atol(l_ReadData[4]);	// �w�ߑ��x
				g_CC_AxisInfo[l_AxisNo].Status = atoi(l_ReadData[5]);			// ���
				g_CC_AxisInfo[l_AxisNo].d_Position = atoi(l_ReadData[7]);		// �ʒu�΍�
				g_pCMotSysDlg->DispdPos(CC_CnvAxisFromSPT(l_AxisNo), g_CC_AxisInfo[l_AxisNo].d_Position);
				g_pCMotSysDlg->CnvWordToBin(g_CC_AxisInfo[l_AxisNo].Status, l_tszText1, 2);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[1][CC_CnvAxisFromSPT(l_AxisNo)], l_tszText1);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[2][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[0]);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[3][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[1]);
			}
			break;
		default:	// YS ��
			if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Y)].bEnable) {
				if ((l_result = CC_ReadCommand2("P261..269", l_Recvdata, 9, l_ReadData)) < 0) {
					CC_MsgReboot();	return ls_AxisLoop;	// �V�X�e���G���[
				}
				g_CC_AxisInfo[l_AxisNo].Position = atol(l_ReadData[0]);			// �ʒu
				g_CC_AxisInfo[l_AxisNo].Velocity = atol(l_ReadData[1]);			// ���x
				g_CC_AxisInfo[l_AxisNo].Current = atol(l_ReadData[2]);			// �d���l
				g_CC_AxisInfo[l_AxisNo].Order_Position = atol(l_ReadData[3]);	// �w�߈ʒu
				g_CC_AxisInfo[l_AxisNo].Order_Velocity = atol(l_ReadData[4]);	// �w�ߑ��x
				g_CC_AxisInfo[l_AxisNo].Order_Current = atol(l_ReadData[5]);	// �w�ߓd���l
				g_CC_AxisInfo[l_AxisNo].Status = atoi(l_ReadData[6]);			// ���
				g_CC_AxisInfo[l_AxisNo].d_Position = atoi(l_ReadData[8]);		// �ʒu�΍�
				g_pCMotSysDlg->DispdPos(CC_CnvAxisFromSPT(l_AxisNo), g_CC_AxisInfo[l_AxisNo].d_Position);
				g_pCMotSysDlg->CnvWordToBin(g_CC_AxisInfo[l_AxisNo].Status, l_tszText1, 2);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[1][CC_CnvAxisFromSPT(l_AxisNo)], l_tszText1);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[3][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[0]);
			}
			ls_AxisLoop = 0;
			break;
		}
	}

	return ls_AxisLoop;
}


// �A���[���Ď�
void CC_PollingAlarm(void)
{
	DWORD	l_InpAlarm;
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_Code1;
	WORD	l_Code2;
	int		i;
	WORD	l_AxisNo;			// ���ԍ�
	BOOL	*l_pSetAlarm;		// �Z�b�g�t���O
	BOOL	*l_pClearAlarm;		// �N���A�t���O
	WORD	l_SetBit;			// �����r�b�g
	WORD	l_ClearBit;			// �N���A�r�b�g
	char	l_DetailAddr[8];	// �ڍ׃R�[�h�̃A�h���X
	char	l_DetailAddr2[8];	// �ڍ׃R�[�h�̃A�h���X
	static DWORD	ls_ClearAlarmTimer[CC_AXIS_NUM];	// �A���[���N���A�^�C�}
	DWORD	*l_pClrAlmTimer;
	DWORD	l_CurrentTime, l_DelayTime;

	if (CC_DioInpDWord(&l_InpAlarm) != 0) {
		CC_DioError();	return;	// ���s
	} else {
		for (i = 0; i < CC_AXIS_NUM; i++) {
			l_pClrAlmTimer = &(ls_ClearAlarmTimer[i]);
			l_pSetAlarm = &(g_CC_bSetAlarm[i]);
			l_pClearAlarm = &(g_CC_bClearAlarm[i]);
			switch (i) {
			case CC_AXIS_X:		// X
			case CC_AXIS_Y:		// YM & YS
				if (0 != motion_data[CC_CnvAxisFromSPT(i)].bEnable) {
					l_AxisNo = i;
					if (i == CC_AXIS_X) {l_SetBit = 0x00000001 << CC_INBIT_ALARM_X;}
					if (i == CC_AXIS_Y) {l_SetBit = 0x00000001 << CC_INBIT_ALARM_Y;}
					l_ClearBit = CC_OUTBIT_CLEAR_ALARM;
					sprintf(l_DetailAddr, "P%1u58", i + 1);
					sprintf(l_DetailAddr2, "P%1u68", i + 1);
				} else {
					l_AxisNo = -1;
				}
				break;
			case CC_AXIS_Z:		// Z(Elevator)
			case CC_AXIS_T:		// Theta
				if (0 != motion_data[CC_CnvAxisFromSPT(i)].bEnable) {
					l_AxisNo = i;
					if (i == CC_AXIS_Z) {l_SetBit = 0x00000001 << CC_INBIT_ALARM_Z;}
					if (i == CC_AXIS_T) {l_SetBit = 0x00000001 << CC_INBIT_ALARM_T;}
					l_ClearBit = CC_OUTBIT_CLEAR_ALARM;
					sprintf(l_DetailAddr, "P%1u56", i + 1);
				} else {
					l_AxisNo = -1;
				}
				break;
			default:
				l_AxisNo = -1;
				break;
			}
//			if (l_AxisNo != -1) {	�o�O������[2009.04.01]
			if (l_AxisNo < CC_AXIS_NUM) {
				if		  ((*l_pSetAlarm != TRUE) && (*l_pClearAlarm != TRUE)) {
					// �A���[�������҂�
					if ((~l_InpAlarm) & l_SetBit) {
						// �A���[������
						*l_pClearAlarm = FALSE;	// �A���[���N���A OFF
						// �ڍ׃R�[�h��Ǐo��
						if ((l_result = CC_ReadCommand(l_DetailAddr, l_Recvdata)) < 0) {
							CC_MsgReboot();	return;		// �V�X�e���G���[
						}
						l_Code1 = atoi(l_Recvdata);
						if ((l_AxisNo == CC_AXIS_Y) && (0 != motion_data[l_AxisNo].bEnable)) {
							if ((l_result = CC_ReadCommand(l_DetailAddr2, l_Recvdata)) < 0) {
								CC_MsgReboot(); return;	// �V�X�e���G���[
							}
							l_Code2 = atoi(l_Recvdata);
						} else {
							l_Code2 = 0;
						}
						*l_pSetAlarm = TRUE;	// �A���[���Z�b�g ON
						CC_ServoOFF_RunOFF();	// �T�[�{ OFF�C�S����~
						CC_ServoFailure();
						// �A���[���\��
						g_pCMotSysDlg->ShowAlarm(3, CC_CnvAxisFromSPT(l_AxisNo), l_Code1, l_Code2);
					}
				} else if (*l_pSetAlarm == TRUE) {
					// �A���[���N���A�҂�
					if (*l_pClearAlarm == TRUE) {
						// �A���[���N���A ON
 						if (CC_DioOutBit(l_ClearBit, 1) != 0) {
							CC_DioError();	return;	// ���s
						}
						*l_pSetAlarm = FALSE;	// �A���[���Z�b�g OFF
						// �A���[���N���A�^�C�}�J�n
						*l_pClrAlmTimer = GetTickCount();
					}
				} else if ((*l_pSetAlarm != TRUE) && (*l_pClearAlarm == TRUE)) {
					// �A���[���r�b�g OFF �҂�
					if (((~l_InpAlarm) & l_SetBit) == 0) {
						// �A���[���N���A�r�b�g OFF
						if (CC_DioOutBit(l_ClearBit, 0) != 0) {
							CC_DioError();	return;	// ���s
						}
						*l_pClearAlarm = FALSE;	// �A���[���N���A OFF
						CC_ServoONOFF(1);	// �T�[�{ ON
					} else {
						// �A���[���N���A�^�C�}�Ď�
						l_CurrentTime = GetTickCount();
						if (*l_pClrAlmTimer <= l_CurrentTime) {
							l_DelayTime = l_CurrentTime - *l_pClrAlmTimer;
						} else {
							l_DelayTime = (((DWORD) 0xffffffff) - *l_pClrAlmTimer) + l_CurrentTime;
						}
						if (CC_CLEAR_ALARM_TIMEOUT < l_DelayTime) {
							//return;	// �V�X�e���G���[
							*l_pClearAlarm = TRUE;
						}
					}
				}
			}
		}
	}
}

// �ϐ����̏�����
void CC_Initial(void)
{
	memset(&g_CC_bSetAlarm, 0, sizeof(g_CC_bSetAlarm));
	memset(&g_CC_bClearAlarm, 0, sizeof(g_CC_bClearAlarm));
	g_CC_ServoONReq = TRUE;		// �T�[�{ ON �v��
	g_CC_ServoOFFReq = FALSE;	// �T�[�{ OFF �v��
	g_CC_ServoONOFFerr = FALSE;	// �T�[�{�^�C���A�E�g�G���[
	g_CC_ComError = FALSE;
	memset(g_CC_Count, 0, sizeof(g_CC_Count));
	memset(g_CC_HP_Pos, 0, sizeof(g_CC_HP_Pos));
	memset(g_CC_Axis_prc, 0, sizeof(g_CC_Axis_prc));
	memset(g_CC_Axis_mode, 0, sizeof(g_CC_Axis_mode));
	memset(g_CC_Axis_HP_Req, 0, sizeof(g_CC_Axis_HP_Req));
	memset(g_CC_Axis_ORG_Req, 0, sizeof(g_CC_Axis_ORG_Req));
	memset(g_CC_Axis_pattern, 0, sizeof(g_CC_Axis_pattern));
	memset(g_CC_MaxSpeed, 0, sizeof(g_CC_MaxSpeed));	// hmenjo10 20031009
}

// �T�[�{ OFF�C���J�n�M���� OFF
void CC_ServoOFF_RunOFF(void)
{
	// �T�[�{ OFF
	CC_ServoONOFF(0);
	// �S����~
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_X)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_X, 0);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Y)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_Y, 0);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Z)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_Z, 0);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_T)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_T, 0);}
}

// �T�[�{ �n�ُ̈��~
void CC_ServoFailure(void)
{
	short	axis;

	// �T�[�{ OFF�C���J�n�M�� OFF �� CC_ServoOFF_RunOFF(void) �ōs��

	for (axis = 0; axis < CC_AXIS_NUM; axis++) {
		if ((g_CC_Axis_prc[axis] != 0) || (g_CC_Axis_mode[axis] != 0)) {
			// ���쒆��������
			if (0 != g_CC_Axis_prc[axis]) {
				// �ُ�I����ʒm���܂��D
				send_motion_message(CC_CnvAxisFromSPT(axis), MM_FAILURE);
			}
			g_CC_Axis_prc[axis] = 0;	// ������Ԃ��N���A
			g_CC_Axis_mode[axis] = 0;	// �������[�h���N���A
		}
	}
}

// ������Ǘ�
void CC_X_Manager(void)		// X ��
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_X;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ���������Ȃ̂ŏI��
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��O�̑҂�����
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_X, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_X, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
			send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
			g_CC_Axis_mode[l_axis] = 0;
			g_CC_Axis_pattern[l_axis] = 0;
			CC_ChangeGain(l_axis, 1);	// �Q�C����؊��� hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- } ---------- */
}
void CC_Y_Manager(void)		// Y ��
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_Y;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ���������Ȃ̂ŏI��
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��O�̑҂�����
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_Y, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Y, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
			send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
			g_CC_Axis_mode[l_axis] = 0;
			g_CC_Axis_pattern[l_axis] = 0;
			CC_ChangeGain(l_axis, 1);	// �Q�C����؊��� hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- } ---------- */
}
void CC_Z_Manager(void)		// Z ��
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_Z;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ���������Ȃ̂ŏI��
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��O�̑҂�����
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_Z, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Z, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
			send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
			g_CC_Axis_mode[l_axis] = 0;
			g_CC_Axis_pattern[l_axis] = 0;
			CC_ChangeGain(l_axis, 1);	// �Q�C����؊��� hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- } ---------- */
}
void CC_T_Manager(void)		// T ��
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_T;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ���������Ȃ̂ŏI��
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��O�̑҂�����
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_T, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_T, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
// �ȉ��� if �u���b�N�͗��R�s���ł��Dhmenjo 2009.03.27
			if ((g_CC_Axis_ORG_Req[l_axis] == TRUE) && (g_CC_Axis_mode[l_axis] == 4)) {
				// ���_���A�O�� INC �ړ��������̂Ō��_���A���s��
				CC_motion_go_home(l_axis);
			} else {
				send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
				g_CC_Axis_mode[l_axis] = 0;
				g_CC_Axis_pattern[l_axis] = 0;
			}
			CC_ChangeGain(l_axis, 1);	// �Q�C����؊��� hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- } ---------- */
}

/*
 *	motion.c ���̏����̑�֊֐��Q
 */
// �e���̃R���g���[������J�E���^��Ǐo��
long CC_get_true_position(short axis)
{
	char	l_CountAddr[6];	// �J�E���^�̃A�h���X
	char	l_Recvdata[256];
	int		l_result;

	switch (axis) {
	case CC_AXIS_X:
	case CC_AXIS_Y:
	case CC_AXIS_Z:
	case CC_AXIS_T:
		sprintf(l_CountAddr, "P%1u51", axis + 1);
		break;
	default:
		return g_CC_Count[axis];
		break;
	}

	if ((l_result = CC_ReadCommand(l_CountAddr, l_Recvdata)) < 0) {
		CC_MsgReboot();		// �V�X�e���G���[
		g_CC_Count[axis] = 0;
	} else {
		g_CC_Count[axis] = atol(l_Recvdata);
	}

	return g_CC_Count[axis];
}

// �ً}��~����
void CC_local_emergency_stop(short axis)
{
	WORD	l_RunBit;			// �J�n/��~ �r�b�g
	char	l_cMode[32];		// ���[�h�A�h���X
	int		l_result;
	char	l_Recvdata[256];

	switch (axis) {
	case CC_AXIS_X:	l_RunBit = CC_OUTBIT_RUN_X; break;
	case CC_AXIS_Y:	l_RunBit = CC_OUTBIT_RUN_Y; break;
	case CC_AXIS_Z:	l_RunBit = CC_OUTBIT_RUN_Z; break;
	case CC_AXIS_T:	l_RunBit = CC_OUTBIT_RUN_T; break;
	default:	break;
	}
	if (0 != g_CC_Axis_prc[axis]) {
		// ���쒆�������̂ŏI����ʒm���܂��D
		send_motion_message(CC_CnvAxisFromSPT(axis), MM_STOPPED);
	}
	g_CC_Axis_prc[axis] = 0;	// ������Ԃ��N���A
	g_CC_Axis_mode[axis] = 0;	// �������[�h���N���A
	// ��~
	if (CC_DioOutBit(l_RunBit, 0) != 0) {
		CC_DioError();	return;	// ���s
	}
	// ���[�h���N���A
	sprintf(l_cMode, "P%1u00=0", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return ;}
	// �Q�C����؊���
	CC_ChangeGain(axis, 1);
}

// HP �ʒu��ݒ�
int CC_motion_set_soft_home(short axis, long soft_home)
{
	long	l_PLimit;
	long	l_MLimit;

	switch (axis) {
	case CC_AXIS_X:	l_PLimit = CC_InitDataCommon.P509; l_MLimit = CC_InitDataCommon.P510; break;
	case CC_AXIS_Y:	l_PLimit = CC_InitDataCommon.P511; l_MLimit = CC_InitDataCommon.P512; break;
	case CC_AXIS_Z:	l_PLimit = CC_InitDataCommon.P513; l_MLimit = CC_InitDataCommon.P514; break;
	case CC_AXIS_T:	l_PLimit = CC_InitDataCommon.P515; l_MLimit = CC_InitDataCommon.P516; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	if (soft_home < l_MLimit)
		soft_home = l_MLimit + 1;
	if (l_PLimit < soft_home)
		soft_home = l_PLimit - 1;

	g_CC_HP_Pos[axis] = soft_home;
	TCHAR l_tszText[64];
	_stprintf(l_tszText, _T("%11d"), g_CC_HP_Pos[CC_CnvAxisFromSPT(axis)]);
	g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[13][CC_CnvAxisFromSPT(axis)], l_tszText);

	return MS_NO_ERROR;
}

// �w�肵�������w�肵�����x�Ń��~�b�g�܂ňړ�����
int CC_motion_move_at_speed(short axis, short velocity)
{
	long	l_PLimit;
	long	l_MLimit;
	long	l_Destination_Pos;
	long	l_Velocity;
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// �J�n�r�b�g
	char	l_cMode[32];	// ���[�h�A�h���X

	// Z ���̏ꍇ�͑��x�𔽓]
	if (axis == CC_AXIS_Z) {
		velocity = -velocity;
	}

	g_CC_Axis_pattern[axis] = 1;
//	CC_DispFuncParam(axis, velocity, 0);

	if (g_CC_Axis_prc[axis] != 0) {
		// �܂��C�ړ���
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// �T�[�{ ON ���Ă��Ȃ�
		return MS_NO_HARDWARE;
	}

	// �ڕW�ʒu(���~�b�g�̎�O��ݒ�)
	switch (axis) {
	case CC_AXIS_X:	l_PLimit = CC_InitDataCommon.P509; l_MLimit = CC_InitDataCommon.P510; break;
	case CC_AXIS_Y:	l_PLimit = CC_InitDataCommon.P511; l_MLimit = CC_InitDataCommon.P512; break;
	case CC_AXIS_Z:	l_PLimit = CC_InitDataCommon.P513; l_MLimit = CC_InitDataCommon.P514; break;
	case CC_AXIS_T:	l_PLimit = CC_InitDataCommon.P515; l_MLimit = CC_InitDataCommon.P516; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	switch (axis) {
	case CC_AXIS_X:	l_RunBit = CC_OUTBIT_RUN_X; break;
	case CC_AXIS_Y:	l_RunBit = CC_OUTBIT_RUN_Y; break;
	case CC_AXIS_Z:	l_RunBit = CC_OUTBIT_RUN_Z; break;
	case CC_AXIS_T:	l_RunBit = CC_OUTBIT_RUN_T; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	// �ڕW
	l_Destination_Pos = (velocity < 0)? (l_MLimit + 4) : (l_PLimit - 4);

	CC_DispFuncParam(axis, velocity, l_Destination_Pos);

	// ���x
	if (axis == CC_AXIS_Z) {
		l_Velocity = (velocity < 0)? -velocity : velocity;
		l_Velocity = CC_ConvSpeed_Z(l_Velocity);
	} else {
		l_Velocity = (velocity < 0)? -velocity : velocity;
#if 1
		l_Velocity = CC_ConvSpeed_XYT(axis, l_Velocity);
#else
		l_Velocity = ServoParam[axis].ABS_Speed;
#endif
	}

	// �R���g���[�����N��(ABS �ړ�)
	g_CC_Axis_mode[axis] = 3;
	sprintf(l_cCmd[0], "P%1u18=%d", axis + 1, ServoParam[axis].ABS_AccelTimeT);
	sprintf(l_cCmd[1], "P%1u19=%d", axis + 1, ServoParam[axis].ABS_AccelTimeS);
	sprintf(l_cCmd[2], "P%1u20=%d", axis + 1, l_Velocity);
	sprintf(l_cCmd[3], "P%1u21=%d", axis + 1, l_Destination_Pos);
	if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// �Q�C����߂�
	CC_ChangeGain(axis, 0);
	// ABS �ړ����[�h���Z�b�g
	sprintf(l_cMode, "P%1u00=3", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// �J�n�� ON
	if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ���s */}
	CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g	*/
	g_CC_Axis_prc[axis] = 1;

	return MS_NO_ERROR;
}
// �w�肵�������w�肵�����x�Ŏw�肵���ʒu�Ɉړ�����
int CC_motion_move_to_position(short axis, long position, short velocity)
{
	long	l_PLimit;
	long	l_MLimit;
	long	l_Destination_Pos;
	long	l_Velocity;
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// �J�n�r�b�g
	char	l_cMode[32];	// ���[�h�A�h���X
	long	l_AccelTimeT, l_AccelTimeS;

	g_CC_Axis_pattern[axis] = 2;

	if (g_CC_Axis_prc[axis] != 0) {
		// �܂��C�ړ���
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// �T�[�{ ON ���Ă��Ȃ�
		return MS_NO_HARDWARE;
	}

	// �����̃��~�b�g���擾
	switch (axis) {
	case CC_AXIS_X:	l_PLimit = CC_InitDataCommon.P509; l_MLimit = CC_InitDataCommon.P510; break;
	case CC_AXIS_Y:	l_PLimit = CC_InitDataCommon.P511; l_MLimit = CC_InitDataCommon.P512; break;
	case CC_AXIS_Z:	l_PLimit = CC_InitDataCommon.P513; l_MLimit = CC_InitDataCommon.P514; break;
	case CC_AXIS_T:	l_PLimit = CC_InitDataCommon.P515; l_MLimit = CC_InitDataCommon.P516; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	switch (axis) {
	case CC_AXIS_X:	l_RunBit = CC_OUTBIT_RUN_X; break;
	case CC_AXIS_Y:	l_RunBit = CC_OUTBIT_RUN_Y; break;
	case CC_AXIS_Z:	l_RunBit = CC_OUTBIT_RUN_Z; break;
	case CC_AXIS_T:	l_RunBit = CC_OUTBIT_RUN_T; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	CC_DispFuncParam(axis, velocity, position);

	// �ڕW
	l_Destination_Pos = position;
	switch (axis) {
	case CC_AXIS_Z:
		if (l_Destination_Pos <= l_MLimit) {l_Destination_Pos = l_MLimit + 4;}
		if (l_PLimit <= l_Destination_Pos) {l_Destination_Pos = l_PLimit - 4;}
		break;
	case CC_AXIS_T:
		CC_ThetaAdjust(&l_Destination_Pos, l_PLimit, l_MLimit);
		break;
	default:
		if (l_Destination_Pos <= l_MLimit) {l_Destination_Pos = l_MLimit + 1;}
		if (l_PLimit <= l_Destination_Pos) {l_Destination_Pos = l_PLimit - 1;}
		break;
	}
	// Y ���̉������Ԃ𒲐�
	l_AccelTimeT = ServoParam[axis].ABS_AccelTimeT;
	l_AccelTimeS = ServoParam[axis].ABS_AccelTimeS;
	if (axis == 1) {
		CC_AdjustAccel_Y(l_Destination_Pos, &l_AccelTimeT, &l_AccelTimeS);
	}

	if (axis == CC_AXIS_Z) {
		l_Velocity = (velocity < 0)? -velocity : velocity;
		l_Velocity = CC_ConvSpeed_Z(l_Velocity);
	} else {
		l_Velocity = (velocity < 0)? -velocity : velocity;
#if 1	// hmenjo10 20031009 -->
		l_Velocity = CC_ConvSpeed_XYT(axis, l_Velocity);
#else	// hmenjo10 20031009
		l_Velocity = ServoParam[axis].ABS_Speed;
#endif	// hmenjo10 20031009 <--
	}

	// ���[�h���X�V
	// �R���g���[�����N��(ABS �ړ�)
	g_CC_Axis_mode[axis] = 3;
	sprintf(l_cCmd[0], "P%1u18=%d", axis + 1, l_AccelTimeT);
	sprintf(l_cCmd[1], "P%1u19=%d", axis + 1, l_AccelTimeS);
	sprintf(l_cCmd[2], "P%1u20=%d", axis + 1, l_Velocity);
	sprintf(l_cCmd[3], "P%1u21=%d", axis + 1, l_Destination_Pos);
	if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// �Q�C����߂�
	CC_ChangeGain(axis, 0);
	sprintf(l_cMode, "P%1u00=3", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// �J�n�� ON
	if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ���s */}
	CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g	*/
	g_CC_Axis_prc[axis] = 1;

	return MS_NO_ERROR;
}
// �w�肵�������w�肵�����x�Ŏw�肵���ʒu�Ɉړ�����(������͎g��Ȃ����Ƃɂ��܂�)
int CC_motion_move_at_speed_to_dest(short axis, long destination, short velocity)
{
	long	l_PLimit;
	long	l_MLimit;
	long	l_Destination_Pos;
	long	l_Velocity;
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// �J�n�r�b�g
	char	l_cMode[32];	// ���[�h�A�h���X
	long	l_AccelTimeT, l_AccelTimeS;

	g_CC_Axis_pattern[axis] = 3;

	if (g_CC_Axis_prc[axis] != 0) {
		// �܂��C�ړ���
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// �T�[�{ ON ���Ă��Ȃ�
		return MS_NO_HARDWARE;
	}

	// �����̃��~�b�g���擾
	switch (axis) {
	case CC_AXIS_X:	l_PLimit = CC_InitDataCommon.P509; l_MLimit = CC_InitDataCommon.P510; break;
	case CC_AXIS_Y:	l_PLimit = CC_InitDataCommon.P511; l_MLimit = CC_InitDataCommon.P512; break;
	case CC_AXIS_Z:	l_PLimit = CC_InitDataCommon.P513; l_MLimit = CC_InitDataCommon.P514; break;
	case CC_AXIS_T:	l_PLimit = CC_InitDataCommon.P515; l_MLimit = CC_InitDataCommon.P516; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	switch (axis) {
	case CC_AXIS_X:	l_RunBit = CC_OUTBIT_RUN_X; break;
	case CC_AXIS_Y:	l_RunBit = CC_OUTBIT_RUN_Y; break;
	case CC_AXIS_Z:	l_RunBit = CC_OUTBIT_RUN_Z; break;
	case CC_AXIS_T:	l_RunBit = CC_OUTBIT_RUN_T; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	CC_DispFuncParam(axis, velocity, destination);

	// �ڕW
	l_Destination_Pos = destination;
	switch (axis) {
	case CC_AXIS_T:
		CC_ThetaAdjust(&l_Destination_Pos, l_PLimit, l_MLimit);
		break;
	default:
		if (l_Destination_Pos < l_MLimit) {l_Destination_Pos = l_MLimit + 4;}
		if (l_PLimit < l_Destination_Pos) {l_Destination_Pos = l_PLimit - 4;}
		break;
	}

	// Y ���̉������Ԃ𒲐�
	l_AccelTimeT = ServoParam[axis].ABS_AccelTimeT;
	l_AccelTimeS = ServoParam[axis].ABS_AccelTimeS;
	if (axis == CC_AXIS_Y) {
		CC_AdjustAccel_Y(l_Destination_Pos, &l_AccelTimeT, &l_AccelTimeS);
	}

	if (axis == CC_AXIS_Z) {
		l_Velocity = (velocity < 0)? -velocity : velocity;
		l_Velocity = CC_ConvSpeed_Z(l_Velocity);
	} else {
		l_Velocity = (velocity < 0)? -velocity : velocity;
#if 1
		l_Velocity = CC_ConvSpeed_XYT(axis, l_Velocity);
#else
		l_Velocity = ServoParam[axis].ABS_Speed;
#endif
	}

	// ���[�h���X�V
	// �R���g���[�����N��(ABS �ړ�)
	g_CC_Axis_mode[axis] = 3;
	sprintf(l_cCmd[0], "P%1u18=%d", axis + 1, l_AccelTimeT);
	sprintf(l_cCmd[1], "P%1u19=%d", axis + 1, l_AccelTimeS);
	sprintf(l_cCmd[2], "P%1u20=%d", axis + 1, l_Velocity);
	sprintf(l_cCmd[3], "P%1u21=%d", axis + 1, l_Destination_Pos);
	if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// �Q�C����߂�
	CC_ChangeGain(axis, 0);
	// ABS �ړ����[�h���Z�b�g
	sprintf(l_cMode, "P%1u00=3", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// �J�n�� ON
	if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ���s */}
	CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g	*/
	g_CC_Axis_prc[axis] = 1;

	return MS_NO_ERROR;
}

// �w�肵������ HP �Ɉړ�����
int CC_motion_go_home(short axis)
{
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// �J�n�r�b�g
	char	l_cMode[32];	// ���[�h�A�h���X
	long	l_Position;		// �ڕW�ʒu
	TCHAR	l_tszText[32];
	long	l_Theta;

	g_CC_Axis_pattern[axis] = 4;

	if (g_CC_Axis_prc[axis] != 0) {
		// �܂��C�ړ���
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// �T�[�{ ON ���Ă��Ȃ�
		return MS_NO_HARDWARE;
	}

	switch (axis) {
	case CC_AXIS_X:	l_RunBit = CC_OUTBIT_RUN_X; l_Position = g_CC_HP_Pos[axis]; break;
	case CC_AXIS_Y:	l_RunBit = CC_OUTBIT_RUN_Y; l_Position = g_CC_HP_Pos[axis]; break;
	case CC_AXIS_Z:	l_RunBit = CC_OUTBIT_RUN_Z; l_Position = g_CC_HP_Pos[axis]; break;
	case CC_AXIS_T:	l_RunBit = CC_OUTBIT_RUN_T; l_Position = g_CC_HP_Pos[axis]; break;
	default:
		return MS_PARAMETER_ERROR; break;
	}

	CC_DispFuncParam(axis, 0, 0);

	// ���_���A�ς��H
	if (g_CC_AxisInfo[axis].Status & 0x0008) {
		// ���A�� : HP �ړ����s��
		// �R���g���[�����N��(HP �ړ�)
		g_CC_Axis_mode[axis] = 2;
		sprintf(l_cCmd[0], "P%1u13=%d", axis + 1, l_Position);
		sprintf(l_cCmd[1], "P%1u14=%d", axis + 1, ServoParam[axis].HP_AccelTimeT);
		sprintf(l_cCmd[2], "P%1u15=%d", axis + 1, ServoParam[axis].HP_AccelTimeS);
//		sprintf(l_cCmd[3], "P%1u16=%d", axis + 1, ServoParam[axis].HP_Speed);
		long l_iSpeed = CC_ConvSpeed_XYT(axis, ServoParam[axis].HP_Speed);
		sprintf(l_cCmd[3], "P%1u16=%d", axis + 1, l_iSpeed);
		sprintf(l_cCmd[4], "P%1u17=%d", axis + 1, ServoParam[axis].HP_BaseBand);
		if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[4], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		// �Q�C����߂�
		CC_ChangeGain(axis, 0);
		// HP �ړ����[�h���Z�b�g
		sprintf(l_cMode, "P%1u00=2", axis + 1);
		if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		// �J�n�� ON
		if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ���s */}
		CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g	*/
		g_CC_Axis_prc[axis] = 1;
	} else {
		// �����A : ���_���A���s��(���A��CHP �ړ�)
		if (axis == CC_AXIS_T) {
			/* Theta �̏ꍇ�͋L���ʒu���}175���𒴂��Ă���Ƃ��́C�܂� INC �Ō��_�����Ɂ}30���ړ��C
			   ���̌�C���_���A�Ƃ���*/
			::GetPrivateProfileString(_T("CurrentPosition"), _T("Theta"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
//��L�ݒ�������ޏ��������������Ȃ��ł����H�H�H[2009.03.25]
			l_Theta = _ttol(l_tszText);
			if ((g_CC_Axis_ORG_Req[axis] == FALSE) && ((l_Theta < -486111) || (486111 < l_Theta))) {
				// INC �ړ�(30��)���ŏ��ɍs��
				g_CC_Axis_mode[axis] = 4;
				sprintf(l_cCmd[0], "P%1u22=%d", axis + 1, ServoParam[axis].INC_AccelTimeT);
				sprintf(l_cCmd[1], "P%1u23=%d", axis + 1, ServoParam[axis].INC_AccelTimeS);
//				sprintf(l_cCmd[2], "P%1u24=%d", axis + 1, ServoParam[axis].INC_Speed);
				long l_iSpeed = CC_ConvSpeed_XYT(axis, ServoParam[axis].HP_Speed);
				sprintf(l_cCmd[2], "P%1u24=%d", axis + 1, l_iSpeed);
				if (l_Theta < 0) {
					sprintf(l_cCmd[3], "P%1u25=%s", axis + 1, "83333");
				} else {
					sprintf(l_cCmd[3], "P%1u25=%s", axis + 1, "-83333");
				}
				if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
				if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
				if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
				if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
				// �Q�C����߂�
				CC_ChangeGain(axis, 0);
				// INC �ړ����[�h���Z�b�g
				sprintf(l_cCmd[0], "P%1u00=4", axis + 1);
				if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
				// �J�n�� ON
				if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ���s */}
				CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g	*/
				g_CC_Axis_prc[axis] = 1;
				g_CC_Axis_ORG_Req[axis] = TRUE;
				return MS_NO_ERROR;
			} else {
				g_CC_Axis_ORG_Req[axis] = FALSE;
			}
		}
		// �R���g���[�����N��(���_���A)
		g_CC_Axis_mode[axis] = 1;
//		g_CC_Axis_HP_Req[axis] = TRUE;	// ���_���A��CHP �ړ��v��
		sprintf(l_cCmd[0], "P%1u01=%d", axis + 1, ServoParam[axis].ORG1_AccelTimeT);
		sprintf(l_cCmd[1], "P%1u02=%d", axis + 1, ServoParam[axis].ORG1_AccelTimeS);
//		sprintf(l_cCmd[2], "P%1u03=%d", axis + 1, ServoParam[axis].ORG1_Speed);
		long l_iSpeed = CC_ConvSpeed_XYT(axis, ServoParam[axis].ORG1_Speed);
		sprintf(l_cCmd[2], "P%1u03=%d", axis + 1, l_iSpeed);
		sprintf(l_cCmd[3], "P%1u04=%d", axis + 1, ServoParam[axis].ORG2_AccelTimeT);
		sprintf(l_cCmd[4], "P%1u05=%d", axis + 1, ServoParam[axis].ORG2_AccelTimeS);
//		sprintf(l_cCmd[5], "P%1u06=%d", axis + 1, ServoParam[axis].ORG2_Speed);
		l_iSpeed = CC_ConvSpeed_XYT(axis, ServoParam[axis].ORG2_Speed);
		sprintf(l_cCmd[5], "P%1u06=%d", axis + 1, l_iSpeed);
		sprintf(l_cCmd[6], "P%1u07=%d", axis + 1, ServoParam[axis].ORG3_AccelTimeT);
		sprintf(l_cCmd[7], "P%1u08=%d", axis + 1, ServoParam[axis].ORG3_AccelTimeS);
//		sprintf(l_cCmd[8], "P%1u09=%d", axis + 1, ServoParam[axis].ORG3_Speed);
		l_iSpeed = CC_ConvSpeed_XYT(axis, ServoParam[axis].ORG3_Speed);
		sprintf(l_cCmd[8], "P%1u09=%d", axis + 1, l_iSpeed);
		if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[4], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[5], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[6], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[7], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		if ((l_result = CC_WriteCommand(l_cCmd[8], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}

		// �Q�C����߂�
		CC_ChangeGain(axis, 0);
		// ���_���A���[�h���Z�b�g
		sprintf(l_cMode, "P%1u00=1", axis + 1);
		if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		// �J�n�� ON
		if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ���s */}
		CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g	*/
		g_CC_Axis_prc[axis] = 1;
	}

	return MS_NO_ERROR;
}

// Theta �p�̐���
void CC_ThetaAdjust(long *position, long pluslimit, long minuslimit)
{
	long	l_Position = *position;

	// 360 ���ȉ��ɕϊ�
	l_Position = l_Position % 1000000;
	// �|�����~�b�g
	if (l_Position < minuslimit) {
		l_Position = minuslimit + 4;
	}
	// �{�����~�b�g
	if (pluslimit < l_Position) {
		l_Position = pluslimit - 4;
	}

	*position = l_Position;
}

// Z �����x�ϊ�
long CC_ConvSpeed_Z(long org_speed)
{
	long	l_rc;

	if (org_speed < 10) {
		l_rc = org_speed - 4;
	} else {
		l_rc = org_speed * 5;
	}
//	l_rc = org_speed * 4;
//	l_rc = org_speed * 2;

	if (l_rc <= 0) {
		l_rc = 1;
	}

	return l_rc;
}

// Y ���ňړ����������ݒ�l�����Ȃ� X �̉����p�����^���g��
void CC_AdjustAccel_Y(long Dest, long *AccelTimeT, long *AccelTimeS)
{
	long l_dPos;

	TCHAR	l_tszText[32];
	::GetPrivateProfileString(_T("Axis"), _T("YAccelXRange"), _T("80000"), l_tszText, sizeof(l_tszText), SERVO_INI);
	long l_lRange = _ttol(l_tszText);

	l_dPos = g_CC_AxisInfo[CC_AXIS_Y].Position - Dest;
	if (l_dPos < 0) {
		l_dPos *= -1;
	}
	if (l_dPos < l_lRange) {
		// �ړ����������ݒ�l�����Ȃ� X ���̉����p�����^���g��
		*AccelTimeT = ServoParam[CC_AXIS_X].ABS_AccelTimeT;
		*AccelTimeS = ServoParam[CC_AXIS_X].ABS_AccelTimeS;
	}
}

// �Q�C���؊��� ALL	(���ݖ��g�p)
void CC_ChangeGainAll(BOOL onoff)
{
	DWORD	l_dwGainSel;

	if (onoff != TRUE) {
		l_dwGainSel = 0;
	} else {
		l_dwGainSel = 1;
	}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_X)].bEnable) {CC_ChangeGain(CC_AXIS_X, l_dwGainSel);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Y)].bEnable) {CC_ChangeGain(CC_AXIS_Y, l_dwGainSel);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Z)].bEnable) {CC_ChangeGain(CC_AXIS_Z, l_dwGainSel);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_T)].bEnable) {CC_ChangeGain(CC_AXIS_T, l_dwGainSel);}
}
// �Q�C���؊��� X/Y/Z/T ��
void CC_ChangeGain(short sAxis, DWORD dwGainSel)
{
	int		l_iGainBit;
	switch (sAxis) {
	case CC_AXIS_X:	l_iGainBit = CC_OUTBIT_CHANGE_GAIN_X; break;
	case CC_AXIS_Y:	l_iGainBit = CC_OUTBIT_CHANGE_GAIN_Y; break;
	case CC_AXIS_Z:	l_iGainBit = CC_OUTBIT_CHANGE_GAIN_Z; break;
	case CC_AXIS_T:	l_iGainBit = CC_OUTBIT_CHANGE_GAIN_T; break;
	default:
		return;
		break;
	}

	BYTE	l_ucGainSel = 0;
	if (0 != dwGainSel) {l_ucGainSel = 1;}
	if (CC_DioOutBit(l_iGainBit, l_ucGainSel) != 0) {CC_DioError(); return;/* ���s */}
}

// �ő呬�x����
long CC_ConvSpeed_XYT(short Axis, long org_speed)
{
#if 0	// FP �V���[�Y�p ----------------------------------------------
	long	l_org_speed;
	long	l_Speed;

	l_org_speed = (org_speed < 0)? -org_speed : org_speed;
	if (l_org_speed < 1) {l_org_speed = 1;}
	if (g_CC_MaxSpeed[Axis] < l_org_speed) {l_org_speed = g_CC_MaxSpeed[Axis];}

	l_Speed = (ServoParam[Axis].ABS_Speed * l_org_speed) / g_CC_MaxSpeed[Axis];
#else	// Nano �V���[�Y�p --------------------------------------------
	long	l_Speed;

	l_Speed = (org_speed < 0)? -org_speed : org_speed;
	if (l_Speed < 1) {l_Speed = 1;}
	switch (Axis) {
	case CC_AXIS_X:
	case CC_AXIS_Y:
	case CC_AXIS_T:
		if (g_CC_MaxSpeed[Axis] < l_Speed) {
			l_Speed = g_CC_MaxSpeed[Axis];
		}
		break;
	default:
		break;
	}
#endif

	return l_Speed;
}

// Sodick ���֕ϊ�
short CC_CnvAxisToSPT(short Axis)
{
	short	l_Axis;

	switch (Axis) {
	case AXIS_BIG_X:
		l_Axis = CC_AXIS_X;
		break;
	case AXIS_BIG_Y:
		l_Axis = CC_AXIS_Y;
		break;
	case AXIS_Z:
		l_Axis = CC_AXIS_Z;
		break;
	case AXIS_T:
		l_Axis = CC_AXIS_T;
		break;
	case AXIS_NUM:
		l_Axis = CC_AXIS_NUM;
		break;
	default:
		l_Axis = -1;
		break;
	}

	return l_Axis;
}
// Sodick ������ϊ�
short CC_CnvAxisFromSPT(short Axis)
{
	short	l_Axis;

	switch (Axis) {
	case CC_AXIS_X:
		l_Axis = AXIS_BIG_X;
		break;
	case CC_AXIS_Y:
		l_Axis = AXIS_BIG_Y;
		break;
	case CC_AXIS_Z:
		l_Axis = AXIS_Z;
		break;
	case CC_AXIS_T:
		l_Axis = AXIS_T;
		break;
	case CC_AXIS_NUM:
		l_Axis = AXIS_NUM;
		break;
	default:
		l_Axis = -1;
		break;
	}

	return l_Axis;
}

// JOYSTICK ���[�h�m�F
BOOL CC_IsJoyMode(void)
{
	if (0 != (g_CC_InPort & (0x00000001 << CC_INBIT_JOY_STATUS))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// JOYSTICK ���[�h�ݒ�
int CC_SetJoyMode(int iMode, int iSpeedSel)
{
	if (0 == g_CC_bInitialize) {
		return MS_AXIS_UNINITIALIZED;
	}

	int i;
	for (i = 0; i < CC_AXIS_NUM; i++) {
		if (0 != motion_data[CC_CnvAxisFromSPT(i)].bEnable) {
			if (0 != motion_data[CC_CnvAxisFromSPT(i)].uninitialized) {
				return MS_AXIS_UNINITIALIZED;
			}
		}
	}
	for (i = 0; i < CC_AXIS_NUM; i++) {
		if (0 != motion_data[CC_CnvAxisFromSPT(i)].bEnable) {
			if (0 == (g_CC_AxisInfo[i].Status & 0x0008)) {
				return MS_AXIS_NOT_HOME;
			}
		}
	}
	for (i = 0; i < CC_AXIS_NUM; i++) {
		if (0 != motion_data[CC_CnvAxisFromSPT(i)].bEnable) {
			if (
				(0 != g_CC_Axis_prc[i])
			 || (0 != g_CC_Axis_mode[i])
			 || ((0 != g_CC_AxisInfo[i].Mode) && (5 != g_CC_AxisInfo[i].Mode))
				) {
				return MS_FUNCTION_BUSY;
			}
		}
	}

	switch (iMode) {
	case 0:
		// ����
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED1, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED2, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED3, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_MODE, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		break;
	case 1:
		// �ݒ�
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED1, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED2, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED3, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		switch (iSpeedSel) {
		case 1:
			if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED1, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
			break;
		case 2:
			if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED2, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
			break;
		case 3:
			if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED3, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
			break;
		default:
			return MS_PARAMETER_ERROR;
			break;
		}
		if (CC_DioOutBit(CC_OUTBIT_JOY_MODE, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ���s */}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}

	return MS_NO_ERROR;
}

// �ō����x���擾
//		�����l�ł͂Ȃ����ۂɎw�߂���ō����x�̂��Ƃł��D
int CC_GetMaxSpeed(short Axis)
{
	long l_lRetSpeed;
	long l_lInitMaxSpeed;

	switch (Axis) {
	case CC_AXIS_X:	l_lInitMaxSpeed = CC_InitDataCommon.P534;	break;
	case CC_AXIS_Y:	l_lInitMaxSpeed = CC_InitDataCommon.P535;	break;
	case CC_AXIS_Z:	l_lInitMaxSpeed = CC_InitDataCommon.P536;	break;
	case CC_AXIS_T:	l_lInitMaxSpeed = CC_InitDataCommon.P537;	break;
	default:		return MS_GETPOS_PARAMERROR;				break;
	}

	if (l_lInitMaxSpeed < ServoParam[Axis].ABS_Speed) {
		// �������f�[�^�̑��x�̕���������
		if (g_CC_MaxSpeed[Axis] < l_lInitMaxSpeed) {
			// ini �t�@�C���̑��x�����̕���������
			l_lRetSpeed= g_CC_MaxSpeed[Axis];
		} else {
			// �������f�[�^�̑��x�̕���������
			l_lRetSpeed= l_lInitMaxSpeed;
		}
	} else {
		// ServoParam.dat �̑��x�̕���������
		if (g_CC_MaxSpeed[Axis] < ServoParam[Axis].ABS_Speed) {
			// ini �t�@�C���̑��x�����̕���������
			l_lRetSpeed= g_CC_MaxSpeed[Axis];
		} else {
			// ServoParam.dat �̑��x�̕���������
			l_lRetSpeed= ServoParam[Axis].ABS_Speed;
		}
	}

	return l_lRetSpeed;
}

// ���̉ғ��͈�(����)���擾
//		�������f�[�^�̃\�t�g���~�b�g����v�Z���܂��D
int CC_GetMotionSize(short Axis)
{
	long l_lRetSize;
	long l_lInitMaxLimit;
	long l_lInitMinLimit;

	switch (Axis) {
	case CC_AXIS_X:
		l_lInitMaxLimit = CC_InitDataCommon.P509;
		l_lInitMinLimit = CC_InitDataCommon.P510;
		break;
	case CC_AXIS_Y:
		l_lInitMaxLimit = CC_InitDataCommon.P511;
		l_lInitMinLimit = CC_InitDataCommon.P512;
		break;
	case CC_AXIS_Z:
		l_lInitMaxLimit = CC_InitDataCommon.P513;
		l_lInitMinLimit = CC_InitDataCommon.P514;
		break;
	case CC_AXIS_T:
		l_lInitMaxLimit = CC_InitDataCommon.P515;
		l_lInitMinLimit = CC_InitDataCommon.P516;
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}

	if (l_lInitMinLimit <= l_lInitMaxLimit) {
		l_lRetSize = l_lInitMaxLimit - l_lInitMinLimit;
	} else {
		l_lRetSize = l_lInitMinLimit - l_lInitMaxLimit;
	}


	return l_lRetSize;
}

// ���̓d�q�M�A���擾
//		�������f�[�^����擾���܂��D
int CC_GetElectronicGear(short Axis)
{
	WORD l_wElectronicGearA;	// ���q
	WORD l_wElectronicGearB;	// ����

	switch (Axis) {
	case CC_AXIS_X:
		l_wElectronicGearA = CC_Initdata_T.P750;
		l_wElectronicGearB = CC_Initdata_T.P751;
		break;
	case CC_AXIS_Y:
		l_wElectronicGearA = CC_Initdata_T.P752;
		l_wElectronicGearB = CC_Initdata_T.P753;
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}

	return MAKELONG(l_wElectronicGearB, l_wElectronicGearA);
}

// ���̃\�t�g���~�b�g�{�����擾
//		�������f�[�^�̃\�t�g���~�b�g��Ǐo���܂��D
int CC_GetSoftLimit(short Axis, short signflag)
{
	long l_lSoftLimit;

	switch (Axis) {
	case CC_AXIS_X:
		l_lSoftLimit = (0 <= signflag)? CC_InitDataCommon.P509 : CC_InitDataCommon.P510;
		break;
	case CC_AXIS_Y:
		l_lSoftLimit = (0 <= signflag)? CC_InitDataCommon.P511 : CC_InitDataCommon.P512;
		break;
	case CC_AXIS_Z:
		l_lSoftLimit = (0 <= signflag)? CC_InitDataCommon.P513 : CC_InitDataCommon.P514;
		break;
	case CC_AXIS_T:
		l_lSoftLimit = (0 <= signflag)? CC_InitDataCommon.P515 : CC_InitDataCommon.P516;
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}

	return l_lSoftLimit;
}

// �W���C�X�e�B�b�N�̑��x��ݒ肵�܂��D
int CC_SetJoySpeed(short axis, int iSpeedSel, short velocity)
{
	int l_Rc = MS_NO_ERROR;

	if (0 == g_CC_bInitialize) {
		return MS_AXIS_UNINITIALIZED;
	}
	switch (axis) {
	case CC_AXIS_X:
	case CC_AXIS_Y:
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
	switch (iSpeedSel) {
	case 1:
	case 2:
	case 3:
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
	// ���x�̕��������ƍő呬�x����
	long	l_Velocity;
	l_Velocity = (velocity < 0)? -velocity : velocity;
	l_Velocity = CC_ConvSpeed_XYT(axis, l_Velocity);
	// �R�}���h���M(���x�p�����^������)
	/*		X ��	1�FP582�C2�FP583�C3�FP584
			Y ��	1�FP587�C2�FP588�C3�FP589*/
	char l_szCmd[32];
	int l_iAxisBase = (CC_AXIS_X == axis)? 2 : 7;
	sprintf(l_szCmd, "P58%1u=%d", l_iAxisBase + (iSpeedSel - 1), l_Velocity);
	char l_szRecvdata[256];
	int l_result;
	if ((l_result = CC_WriteCommand(l_szCmd, l_szRecvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}

	return l_Rc;
}

/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- { ---------- */
/* ���ړ��^�C���A�E�g���`�F�b�N���܂��D	*/
#define	CC_MOTION_TIMEOUT	(10 * 60 * 1000)	/* ���ړ��^�C���A�E�g[ms]	*/
int CC_MotionTimeout(
		int iMode,		/* ���[�h
							0�F�J�n
							1�F�I��
							2�F�`�F�b�N	*/
		short wAxis		/*	��	*/
	)
{
	static DWORD ls_dwCounter[CC_AXIS_NUM] = {static_cast<DWORD>(-1), static_cast<DWORD>(-1), static_cast<DWORD>(-1), static_cast<DWORD>(-1)};
	static BOOL ls_bCountFlag[CC_AXIS_NUM] = {FALSE, FALSE, FALSE, FALSE};
	int l_iRet = 0;
	DWORD l_dwCounter[CC_AXIS_NUM];
	BOOL l_bCountFlag[CC_AXIS_NUM];

	/* pop --------------------------------------------*/
	memcpy(l_dwCounter, ls_dwCounter, sizeof(l_dwCounter));
	memcpy(l_bCountFlag, ls_bCountFlag, sizeof(l_bCountFlag));

	if ((wAxis < 0) || (CC_AXIS_NUM <= wAxis)) {
		l_iRet = -1;		/* �w�肳�ꂽ���ԍ����s���ł��D	*/
	} else {
		if (0 == iMode) {
			/* �J�n -----------------------------------*/
			l_dwCounter[wAxis] = ::GetTickCount();
			l_bCountFlag[wAxis] = TRUE;
			l_iRet = 0;
		} else if (1 == iMode) {
			/* �I�� -----------------------------------*/
			l_bCountFlag[wAxis] = FALSE;
			l_iRet = 0;
		} else {
			/* �`�F�b�N -------------------------------*/
			if (TRUE != l_bCountFlag[wAxis]) {
				l_iRet = -3;		/* �J�n����Ă��܂���D	*/
			} else {
				DWORD l_dwCheck =  ::GetTickCount();
				DWORD l_dwElaps;
				if (l_dwCheck < l_dwCounter[wAxis]) {
					l_dwElaps = l_dwCheck + (ULONG_MAX - l_dwCounter[wAxis]) + 1;
				} else {
					l_dwElaps = l_dwCheck - l_dwCounter[wAxis];
				}
				if (CC_MOTION_TIMEOUT < l_dwElaps) {
					/* �^�C���A�E�g���܂����D	*/
					l_bCountFlag[wAxis] = FALSE;
					l_iRet = 1;
				} else {
					l_iRet = 0;
				}
			}
		}
	}

	/* push -------------------------------------------*/
	memcpy(ls_bCountFlag, l_bCountFlag, sizeof(ls_bCountFlag));
	memcpy(ls_dwCounter, l_dwCounter, sizeof(ls_dwCounter));

	return l_iRet;
}
/* ���ړ��^�C���A�E�g�̏ꍇ�͂��̎����~���܂��D	*/
void CC_MotTimeoutCheck(short wAxis, short *wPrc)
{
	static short ls_wPrcPre[CC_AXIS_NUM] = {0, 0, 0, 0};
	short l_wPrcPre[CC_AXIS_NUM];

	/* pop --------------------------------------------*/
	memcpy(l_wPrcPre, ls_wPrcPre, sizeof(l_wPrcPre));

	if ((0 != l_wPrcPre[wAxis]) && (0 == *wPrc)) {
		/* ������Ԃ���~���ɂȂ���	*/
		CC_MotionTimeout(1, wAxis);	/* �^�C���A�E�g�Ď� �I��	*/
	} else if (0 != *wPrc) {
		/* ������Ԃ����쒆	*/
		int l_iRet = CC_MotionTimeout(2, wAxis);	/* �^�C���A�E�g�Ď� �`�F�b�N	*/
		if (1 == l_iRet) {
			/* �^�C���A�E�g���܂����D	*/
			*wPrc = 0;
			CC_local_emergency_stop(wAxis);
			send_motion_message(CC_CnvAxisFromSPT(wAxis), MM_TIMEOUT);
		}
	}
	l_wPrcPre[wAxis] = *wPrc;

	/* push -------------------------------------------*/
	memcpy(ls_wPrcPre, l_wPrcPre, sizeof(ls_wPrcPre));
}
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- } ---------- */

void CC_DispOthers(void)
{
	TCHAR	l_tszText[64];
	short	l_sAxis;

	if (0 == g_pCMotSysDlg) {
		return;
	}

	for (int i = 0; i < CC_AXIS_NUM; i++) {
		switch (i) {
		case CC_AXIS_X:
		case CC_AXIS_Y:
		case CC_AXIS_Z:
		case CC_AXIS_T:
			l_sAxis = CC_CnvAxisFromSPT(i);
			if (0 != motion_data[l_sAxis].bEnable) {
				_stprintf(l_tszText, _T("%1d"), g_CC_Axis_prc[i]);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[12][l_sAxis], l_tszText);
			}
			break;
		default:
			break;
		}
	}

	// DI ��\��
	TCHAR l_tszTempL[32], l_tszTempH[32];
	g_pCMotSysDlg->CnvWordToBin((WORD) (g_CC_InPort & 0x0000ffff), l_tszTempL, 2);
	g_pCMotSysDlg->CnvWordToBin((WORD) (g_CC_InPort >> 16), 	   l_tszTempH, 2);
	_stprintf(l_tszText, _T("%s  %s"), l_tszTempH, l_tszTempL);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_DI, l_tszText);
	// DO ��\��
	g_pCMotSysDlg->CnvWordToBin((WORD) (g_CC_OutPort & 0x0000ffff), l_tszTempL, 2);
	g_pCMotSysDlg->CnvWordToBin((WORD) (g_CC_OutPort >> 16),		l_tszTempH, 2);
	_stprintf(l_tszText, _T("%s  %s"), l_tszTempH, l_tszTempL);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_DO, l_tszText);
}
void CC_DispFuncParam(short axis, short Speed, long position)
{
	TCHAR	l_tszText[32];
	short	l_sAxis = CC_CnvAxisFromSPT(axis);

	switch (axis) {
	case CC_AXIS_X:
	case CC_AXIS_Y:
	case CC_AXIS_Z:
	case CC_AXIS_T:
		_stprintf(l_tszText, _T("%1d"), g_CC_Axis_pattern[axis]);
		g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[11][l_sAxis], l_tszText);
		_stprintf(l_tszText, _T("%7d"), Speed);
		g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[14][l_sAxis], l_tszText);
		_stprintf(l_tszText, _T("%11d"), position);
		g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[15][l_sAxis], l_tszText);
		break;
	default:
		break;
	}
}




// �ȉ��̃R�[�h�̓f�o�b�O�p
void mmtest(void)
{
#define DEB_JOB 3
#if DEB_JOB == 0
#elif DEB_JOB == 1
	char l_Senddata[255];
	char l_Recvdata[255];
	DWORD	RecvLength;

	strcpy(l_Senddata, "P100");
	int l_result = g_pCSio->CommCommand(l_Senddata, l_Recvdata, &RecvLength);
#elif DEB_JOB == 2
	g_pCMotSysDlg->ShowAlarm(3, 2, 3, 4);
#elif DEB_JOB == 3
	// �T�[�{ OFF �v��
	g_CC_ServoOFFReq = TRUE;
#endif
}

void mmtest2(void)
{
	// �T�[�{ ON �v��
	g_CC_ServoONReq = TRUE;
}

// ���_���A �N��(�e�X�g��)
static short	lg_Xorg = 0;
void mmtestORGX(int mode)
{
	char	l_Recvdata[256];
	int		l_result;
	char	l_cCmd[32];

	// �p�����^���Z�b�g
	switch (mode) {
	case 0:
		sprintf(l_cCmd, "P101=%d", ServoParam[0].ORG1_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P102=%d", ServoParam[0].ORG1_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P103=%d", ServoParam[0].ORG1_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P104=%d", ServoParam[0].ORG2_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P105=%d", ServoParam[0].ORG2_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P106=%d", ServoParam[0].ORG2_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P107=%d", ServoParam[0].ORG3_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P108=%d", ServoParam[0].ORG3_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P109=%d", ServoParam[0].ORG3_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// ���_���A���[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P100=1", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 1:
		if ((l_result = CC_WriteCommand("P113=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P114=%d", ServoParam[0].HP_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P115=%d", ServoParam[0].HP_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P116=%d", ServoParam[0].HP_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P117=%d", ServoParam[0].HP_BaseBand);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// HP �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P100=2", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 2:
	case 21:
		sprintf(l_cCmd, "P118=%d", ServoParam[0].ABS_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P119=%d", ServoParam[0].ABS_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P120=%d", ServoParam[0].ABS_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 2) {
			if ((l_result = CC_WriteCommand("P121=28000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P121=-28000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// ABS �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P100=3", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 3:
	case 31:
		sprintf(l_cCmd, "P122=%d", ServoParam[0].INC_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P123=%d", ServoParam[0].INC_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P124=%d", ServoParam[0].INC_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 3) {
			if ((l_result = CC_WriteCommand("P125=2000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P125=-2000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// INC �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P100=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// �J�n�� ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_X, 1) != 0) {
		CC_DioError();	return;	// ���s
	}
	lg_Xorg = 1;
}
// ���_���A ����(�e�X�g��)
void mmtestORGXdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Xorg) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��҂�
		lg_Xorg = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_X, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Xorg = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		if ((l_result = CC_ReadCommand("P150", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_X, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			if ((l_result = CC_WriteCommand("P100=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			lg_Xorg = 0;
		}
		break;
	}
}

static short	lg_Yorg = 0;
void mmtestORGY(int mode)
{
	char	l_Recvdata[256];
	int		l_result;
	char	l_cCmd[32];

	// �p�����^���Z�b�g
	switch (mode) {
	case 0:
		sprintf(l_cCmd, "P201=%d", ServoParam[1].ORG1_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P202=%d", ServoParam[1].ORG1_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P203=%d", ServoParam[1].ORG1_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P204=%d", ServoParam[1].ORG2_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P205=%d", ServoParam[1].ORG2_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P206=%d", ServoParam[1].ORG2_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P207=%d", ServoParam[1].ORG3_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P208=%d", ServoParam[1].ORG3_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P209=%d", ServoParam[1].ORG3_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// ���_���A���[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P200=1", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 1:
		if ((l_result = CC_WriteCommand("P213=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P214=%d", ServoParam[1].HP_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P215=%d", ServoParam[1].HP_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P216=%d", ServoParam[1].HP_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P217=%d", ServoParam[1].HP_BaseBand);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// HP �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P200=2", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 2:
	case 21:
		sprintf(l_cCmd, "P218=%d", ServoParam[1].ABS_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P219=%d", ServoParam[1].ABS_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P220=%d", ServoParam[1].ABS_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 2) {
			if ((l_result = CC_WriteCommand("P221=4000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P221=-4000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// ABS �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P200=3", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
		sprintf(l_cCmd, "P222=%d", ServoParam[1].INC_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P223=%d", ServoParam[1].INC_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P224=%d", ServoParam[1].INC_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 3) {
			if ((l_result = CC_WriteCommand("P225=8000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P225=-8000000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// INC �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P200=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// �J�n�� ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_Y, 1) != 0) {
		CC_DioError();	return;	// ���s
	}
	lg_Yorg = 1;
}
// ���_���A ����(�e�X�g��)
void mmtestORGYdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Yorg) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��҂�
		lg_Yorg = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_Y, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Yorg = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		if ((l_result = CC_ReadCommand("P250", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Y, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			if ((l_result = CC_WriteCommand("P200=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			lg_Yorg = 0;
		}
		break;
	}
}

static short	lg_Zorg = 0;
void mmtestORGZ(int mode)
{
	char	l_Recvdata[256];
	int		l_result;
	char	l_cCmd[32];

	// �p�����^���Z�b�g
	switch (mode) {
	case 0:
		sprintf(l_cCmd, "P301=%d", ServoParam[2].ORG1_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P302=%d", ServoParam[2].ORG1_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P303=%d", ServoParam[2].ORG1_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P304=%d", ServoParam[2].ORG2_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P305=%d", ServoParam[2].ORG2_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P306=%d", ServoParam[2].ORG2_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P307=%d", ServoParam[2].ORG3_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P308=%d", ServoParam[2].ORG3_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P309=%d", ServoParam[2].ORG3_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// ���_���A���[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P300=1", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 1:
		if ((l_result = CC_WriteCommand("P313=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P314=%d", ServoParam[2].HP_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P315=%d", ServoParam[2].HP_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P316=%d", ServoParam[2].HP_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P317=%d", ServoParam[2].HP_BaseBand);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// HP �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P300=2", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 2:
	case 21:
		sprintf(l_cCmd, "P318=%d", ServoParam[2].ABS_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P319=%d", ServoParam[2].ABS_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P320=%d", ServoParam[2].ABS_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 2) {
			if ((l_result = CC_WriteCommand("P321=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P321=-30000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// ABS �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P300=3", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 3:
	case 31:
		sprintf(l_cCmd, "P322=%d", ServoParam[2].INC_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P323=%d", ServoParam[2].INC_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P324=%d", ServoParam[2].INC_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 3) {
			if ((l_result = CC_WriteCommand("P325=10000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P325=-10000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// INC �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P300=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// �J�n�� ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_Z, 1) != 0) {
		CC_DioError();	return;	// ���s
	}
	lg_Zorg = 1;
}
// ���_���A ����(�e�X�g��)
void mmtestORGZdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Zorg) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��҂�
		lg_Zorg = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_Z, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Zorg = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		if ((l_result = CC_ReadCommand("P350", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Z, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			if ((l_result = CC_WriteCommand("P300=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			lg_Zorg = 0;
		}
		break;
	}
}

static short	lg_Torg = 0;
void mmtestORGT(int mode)
{
	char	l_Recvdata[256];
	int		l_result;
	char	l_cCmd[32];

	// �p�����^���Z�b�g
	switch (mode) {
	case 0:
		sprintf(l_cCmd, "P401=%d", ServoParam[4].ORG1_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P402=%d", ServoParam[4].ORG1_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P403=%d", ServoParam[4].ORG1_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P404=%d", ServoParam[4].ORG2_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P405=%d", ServoParam[4].ORG2_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P406=%d", ServoParam[4].ORG2_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P407=%d", ServoParam[4].ORG3_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P408=%d", ServoParam[4].ORG3_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P409=%d", ServoParam[4].ORG3_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// ���_���A���[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P400=1", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 1:
		if ((l_result = CC_WriteCommand("P413=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P414=%d", ServoParam[4].HP_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P415=%d", ServoParam[4].HP_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P416=%d", ServoParam[4].HP_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P417=%d", ServoParam[4].HP_BaseBand);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		// HP �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P400=2", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 2:
	case 21:
		sprintf(l_cCmd, "P418=%d", ServoParam[4].ABS_AccelTimeT);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P419=%d", ServoParam[4].ABS_AccelTimeS);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P420=%d", ServoParam[4].ABS_Speed);
		if ((l_result = CC_WriteCommand(l_cCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 2) {
			if ((l_result = CC_WriteCommand("P421=125000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P421=-125000", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// ABS �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P400=3", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	case 3:
	case 31:
		sprintf(l_cCmd, "P422=%d", ServoParam[4].INC_AccelTimeT);
		if ((l_result = CC_WriteCommand("P422=500", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P423=%d", ServoParam[4].INC_AccelTimeS);
		if ((l_result = CC_WriteCommand("P423=250", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		sprintf(l_cCmd, "P424=%d", ServoParam[4].INC_Speed);
		if ((l_result = CC_WriteCommand("P424=16", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (mode == 3) {
			if ((l_result = CC_WriteCommand("P425=27777", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		} else {
			if ((l_result = CC_WriteCommand("P425=-27777", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		}
		// INC �ړ����[�h���Z�b�g
		if ((l_result = CC_WriteCommand("P400=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// �J�n�� ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_T, 1) != 0) {
		CC_DioError();	return;	// ���s
	}
	lg_Torg = 1;
}
// ���_���A ����(�e�X�g��)
void mmtestORGTdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Torg) {
	case 0:		// �N���҂�
		break;
	case 1:		// INP �Ď��҂�
		lg_Torg = 2;
		break;
	case 2:		// INP �҂�
		if (CC_DioInpBit(CC_INBIT_INP_T, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Torg = 3;
		}
		break;
	case 3:		// ���샂�[�h�̒ʏ�ւ̕ω��҂�
		if ((l_result = CC_ReadCommand("P450", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// �J�n�� OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_T, 0) != 0) {
				CC_DioError();	return;	// ���s
			}
			// ���샂�[�h��ʏ�փZ�b�g
			if ((l_result = CC_WriteCommand("P400=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			lg_Torg = 0;
		}
		break;
	}
}
