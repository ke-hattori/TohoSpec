
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

extern UINT MOTDRV_SHOWALARM;		// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½ï¿½p
extern UINT MOTDRV_SRVINITCOMP;		// ï¿½Tï¿½[ï¿½{ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½`

static CMotSysDlg* g_pCMotSysDlg;

/*
 * DIO ï¿½ÖŒW
 */
HANDLE	g_CC_hDrv = 0;			// DIO ï¿½Ìƒfï¿½oï¿½Cï¿½Xï¿½nï¿½ï¿½ï¿½hï¿½ï¿½
WORD	g_CC_DrvNo = 0;			// DIO ï¿½Ìƒhï¿½ï¿½ï¿½Cï¿½oï¿½Ô�ï¿½
WORD	g_CC_GrpNo = 0;			// DIO ï¿½ÌƒOï¿½ï¿½ï¿½[ï¿½vï¿½Ô�ï¿½


// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½Nï¿½ï¿½ï¿½Ö�ï¿½)
BOOL CC_InitServo(LPVOID pParam)
{
	g_pCMotSysDlg = (CMotSysDlg*) pParam;

	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_STATUS, _T("Initializing Controller..."));

	// Drv No. ï¿½ï¿½\ï¿½ï¿½
	TCHAR l_tszText[32];
	_stprintf(l_tszText, _T("%02u"), g_CC_DrvNo);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_DRVNO, l_tszText);
	// Grp No. ï¿½ï¿½\ï¿½ï¿½
	_stprintf(l_tszText, _T("%02u"), g_CC_GrpNo);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_GRPNO, l_tszText);

	int l_ret;
	if (0 != (l_ret = CC_Initialize_Controller())) {
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½s
		g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_STATUS, _T("Failed to initialize"));
		if (l_ret == -1) {
			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½Oï¿½É‚È‚ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½
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
// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
DWORD CC_Initialize_Controller(void)
{
	char	l_Senddata[256];
	char	l_Recvdata[256];
	int		l_result;
	int		*l_Data;
	int		i;

	// ï¿½ï¿½ï¿½[ï¿½^ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ì�ï¿½Ô‚ï¿½ï¿½mï¿½F
	strcpy(l_Senddata, "P100");
	if ((l_result = CC_ReadCommand(l_Senddata, l_Recvdata)) < 0) {
		return -2;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(ï¿½ï¿½Ô“Ç�oï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[)
	} else {
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç�ï¿½ï¿½ï¿½
		if (CC_LoadInitData() != TRUE) {
			return -3;	// ï¿½Ç‚İ�ï¿½ï¿½İ�ï¿½ï¿½s
		} else {
			// ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^
			l_Data = (int*) &CC_InitDataCommon;
			for (i = 0; i < (sizeof(CC_InitDataCommon) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 501, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -4;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(ï¿½ï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½s)
				}
			}
			// X ï¿½fï¿½[ï¿½^
			l_Data = (int*) &CC_Initdata_X;
			for (i = 0; i < (sizeof(CC_Initdata_X) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 600, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -5;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(X ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½s)
				}
			}
			// Y1 ï¿½fï¿½[ï¿½^
			l_Data = (int*) &CC_Initdata_Y1;
			for (i = 0; i < (sizeof(CC_Initdata_Y1) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 625, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -6;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(Y1 ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½s)
				}
			}
			// Y2 ï¿½fï¿½[ï¿½^
			l_Data = (int*) &CC_Initdata_Y2;
			for (i = 0; i < (sizeof(CC_Initdata_Y2) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 650, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -7;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(Y2 ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½s)
				}
			}
			// Z ï¿½fï¿½[ï¿½^
			l_Data = (int*) &CC_Initdata_Z;
			for (i = 0; i < (sizeof(CC_Initdata_Z) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 675, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -8;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(Z ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½s)
				}
			}
			// T ï¿½fï¿½[ï¿½^
			l_Data = (int*) &CC_Initdata_T;
			for (i = 0; i < (sizeof(CC_Initdata_T) / sizeof(int)); i++) {
				sprintf(l_Senddata, "P%03u=%d", i + 700, *l_Data++);
				if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
					return -9;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(T ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½İ�ï¿½ï¿½s)
				}
			}
			// ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½lï¿½Ì•Ï�Xï¿½Jï¿½n
			strcpy(l_Senddata, "P500=1");
			if ((l_result = CC_WriteCommand(l_Senddata, l_Recvdata)) < 0) {
				return -10;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½lï¿½Ì•Ï�Xï¿½Jï¿½nï¿½ï¿½ï¿½s)
			}
			// ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä�ï¿½(P500 ï¿½ï¿½ 0 ï¿½É‚È‚ï¿½Ì‚ï¿½ï¿½Ü‚ï¿½)
			int l_Loop = FALSE;
			DWORD l_StartTime = GetTickCount();
			DWORD l_CurrentTime, l_DelayTime;
			strcpy(l_Senddata, "P500");
			while (l_Loop == FALSE) {
				if ((l_result = CC_ReadCommand(l_Senddata, l_Recvdata)) < 0) {
					return -11;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½ï¿½Ç�ï¿½ï¿½İ�ï¿½ï¿½s)
				}
				if (strcmp(l_Recvdata, "0") == 0) {
					l_Loop = TRUE;	// 0 ï¿½É‚È‚ï¿½ï¿½ï¿½
				} else {
					// ï¿½ï¿½è��ï¿½ÔˆÈ“ï¿½ï¿½ï¿½ 0 ï¿½É‚È‚ï¿½È‚ï¿½ï¿½È‚ï¿½Ù�ï¿½
					l_CurrentTime = GetTickCount();
					if (l_StartTime <= l_CurrentTime) {
						l_DelayTime = l_CurrentTime - l_StartTime;
					} else {
						l_DelayTime = (((DWORD) 0xffffffff) - l_StartTime) + l_CurrentTime;
					}
					if (CC_INIT_TIMEOUT < l_DelayTime) {
						return -1;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[(ï¿½Ï�Xï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g)
					}
				}
			}
		}
	}

	// ï¿½Tï¿½[ï¿½{ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½Ç�oï¿½ï¿½
	if (CC_LoadServoParam() != TRUE) {
		return FALSE;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
	}

	// ï¿½Nï¿½ï¿½ï¿½Oï¿½Ì�ï¿½Ô“Ç�oï¿½ï¿½(1ï¿½ï¿½Ì‚ï¿½)
	while (1) {
		if (CC_PollingStatus() == 0) {
			break;
		}
	}

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	g_CC_bInitialize = TRUE;

	return 0;
}

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç�ï¿½ï¿½ï¿½
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
			// ï¿½ï¿½ï¿½ï¿½Rï¿½[ï¿½hï¿½ï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½ï¿½ï¿½ï¿½ NULL ï¿½É‚ï¿½ï¿½ï¿½
			for (i = 0; i < strlen(l_cBuff); i++) {
				if (iscntrl(l_cBuff[i]) != 0) {
					l_cBuff[i] = 0x00;
					break;
				}
			}
			// ï¿½fï¿½[ï¿½^ï¿½ï¿½Ø�oï¿½ï¿½
			strcpy(l_cBuff2, &(l_cBuff[5]));
			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
			if (10 < strlen(l_cBuff2)) {
				break;
			}
			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç‚ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N
			for (i = 0; i < strlen(l_cBuff2); i++) {
// '-'ï¿½ï¿½OK				if (isdigit(l_cBuff2[i]) == 0) {
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
		// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�ï¿½ï¿½^ï¿½ï¿½ï¿½ì�¬
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
			// ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½ï¿½
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
// ï¿½Tï¿½[ï¿½{ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç�oï¿½ï¿½
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
		// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		l_rc = FALSE;
		_stprintf(l_tszText, _T("Could not open Servo Parameter file[%s]."), l_tszPathName);
		if (0 == g_pCMotSysDlg->m_bOnClosing) {
			::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
		}
	}

	// X ï¿½ï¿½ï¿½Ì�Å‘å‘¬ï¿½xï¿½ï¿½ï¿½ï¿½
	::GetPrivateProfileString(_T("MaxSpeed"), _T("BigXMaxSpeed"), _T("2000"), l_tszText, sizeof(l_tszText), SERVO_INI);
	g_CC_MaxSpeed[CC_AXIS_X] = _ttol(l_tszText);
	if (CC_InitDataCommon.P534 < g_CC_MaxSpeed[CC_AXIS_X]) {g_CC_MaxSpeed[CC_AXIS_X] = CC_InitDataCommon.P534;}
	if (g_CC_MaxSpeed[CC_AXIS_X] < 10) {g_CC_MaxSpeed[CC_AXIS_X] = 10;}

	// Y ï¿½ï¿½ï¿½Ì�Å‘å‘¬ï¿½xï¿½ï¿½ï¿½ï¿½
	::GetPrivateProfileString(_T("MaxSpeed"), _T("BigYMaxSpeed"), _T("2000"), l_tszText, sizeof(l_tszText), SERVO_INI);
	g_CC_MaxSpeed[CC_AXIS_Y] = _ttol(l_tszText);
	if (CC_InitDataCommon.P535 < g_CC_MaxSpeed[CC_AXIS_Y]) {g_CC_MaxSpeed[CC_AXIS_Y] = CC_InitDataCommon.P535;}
	if (g_CC_MaxSpeed[CC_AXIS_Y] < 10) {g_CC_MaxSpeed[CC_AXIS_Y] = 10;}

	// Z ï¿½ï¿½ï¿½Ì�Å‘å‘¬ï¿½xï¿½ï¿½ï¿½ï¿½
	g_CC_MaxSpeed[CC_AXIS_Z] = CC_InitDataCommon.P536;

	// T ï¿½ï¿½ï¿½Ì�Å‘å‘¬ï¿½xï¿½ï¿½ï¿½ï¿½
	::GetPrivateProfileString(_T("MaxSpeed"), _T("ThetaMaxSpeed"), _T("10"), l_tszText, sizeof(l_tszText), SERVO_INI);
	g_CC_MaxSpeed[CC_AXIS_T] = _ttol(l_tszText);
	if (10 < g_CC_MaxSpeed[CC_AXIS_T]) {g_CC_MaxSpeed[CC_AXIS_T] = 10;}
	if (g_CC_MaxSpeed[CC_AXIS_T] <	5) {g_CC_MaxSpeed[CC_AXIS_T] =	5;}

	return l_rc;
}

// ï¿½ï¿½ï¿½[ï¿½hï¿½Rï¿½}ï¿½ï¿½ï¿½h
int CC_ReadCommand(char *SendData, char *RecvData)
{
	int				l_retry = 0;
	int				l_result = 0;
	TCHAR			l_tszText[256];
	int				i;
	unsigned char	l_Sum;
	DWORD			l_RecvLength;
	static BOOL		ls_OnceError = TRUE;	// ï¿½Å�ï¿½ï¿½Ì’Ê�Mï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å–ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½É‚ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½O
												/*	ï¿½ï¿½Lï¿½ï¿½ FP ï¿½Vï¿½ï¿½ï¿½[ï¿½Yï¿½Ì�ê�‡ï¿½D
													Nano ï¿½Å‚Í‰ï¿½ï¿½Lï¿½ï¿½ #if ï¿½É‚Ä�Cï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Ì‚ï¿½
													ï¿½wï¿½èƒŠï¿½gï¿½ï¿½ï¿½Cï¿½ñ�”‚Ì‚Pï¿½Oï¿½{ï¿½Ìƒï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½ï¿½
													ï¿½sï¿½È‚ï¿½ï¿½æ‚¤ï¿½É•Ï�Xï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D
												*/
	int				l_iRetryTimes;

	while (1) {
//if (0 == g_CC_ThreadLoop) {return 0x80000000;}
if ((0 == g_CC_ThreadLoop) || (0 != g_CC_CommAbort)) {l_retry = 0; return 0x80000000;}
		l_result = g_pCSio->CommCommand(SendData, RecvData, &l_RecvLength);
		if ((l_result & 0x10000000) != 0) {RETRY_TIMES;
			// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½
			send_motion_message(-1, 111);	// ï¿½Ê�M(Read)ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½
			_stprintf(l_tszText, _T("Communication Functions is not work(ReadCommand:0=0x%08x)."), l_result);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
			break;;
		} else {
			if (l_result != MSC_COMM_OK) {
				// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C
#if 0
				if ((ls_OnceError != TRUE) && (RETRY_TIMES <= l_retry++)) {
#else
				l_iRetryTimes = (0 != ls_OnceError)? RETRY_TIMES * 10: RETRY_TIMES;
				if ((l_iRetryTimes <= l_retry++)) {
#endif
					// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½ÍƒVï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
					send_motion_message(-1, 112);	// ï¿½Ê�M(Read)ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½Pï¿½ï¿½ï¿½ï¿½
					l_result = -1;
					_stprintf(l_tszText, _T("Communication retry out(ReadCommand:1=0x%08x). Retry times : %d."), l_result, l_iRetryTimes);
					if (0 == g_pCMotSysDlg->m_bOnClosing) {
						::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
					}
					break;
				}
			} else {
				// ï¿½ï¿½ï¿½í‰�ï¿½ï¿½
				ls_OnceError = FALSE; l_retry = 0;
				// ï¿½ï¿½ï¿½[ï¿½hï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½o
				for (i = 0; i < 255; i++) {
					if (RecvData[i] == COM_CR) {
						l_Sum = RecvData[i + 1];
						RecvData[i + 1] = 0x00;
						// ï¿½Tï¿½ï¿½ï¿½Ìƒ`ï¿½Fï¿½bï¿½N
						if (l_Sum != g_pCSio->CalcCheckSum(RecvData, 1)) {
							// ï¿½Tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
							l_result = -3;
						}
						if (12 < strlen(RecvData)) {
							// ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(10ï¿½ï¿½ï¿½ï¿½ï¿½ğ’´‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½)
							l_result = -2;
						}
						RecvData[i] = 0x00;
						break;
					}
				}
				if ((255 <= i) || (l_result != MSC_COMM_OK)) {
					// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C
					if ((ls_OnceError != TRUE) && (RETRY_TIMES <= l_retry++)) {
						// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½ÍƒVï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
						send_motion_message(-1, 113);	// ï¿½Ê�M(Read)ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½Qï¿½ï¿½ï¿½ï¿½
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
// ï¿½ï¿½ï¿½[ï¿½hï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½Q
int CC_ReadCommand2(
					char *SendData,		// ï¿½ï¿½ï¿½Mï¿½fï¿½[ï¿½^
					char *RecvData,		// ï¿½ï¿½Mï¿½fï¿½[ï¿½^
					short DataNum,		// ï¿½fï¿½[ï¿½^ï¿½ï¿½
					void *ReadData		// ï¿½ï¿½ï¿½[ï¿½hï¿½fï¿½[ï¿½^
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
			// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½
			send_motion_message(-1, 121);	// ï¿½Ê�M(Read2)ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½
			_stprintf(l_tszText, _T("Communication Functions is not work(ReadCommand2:0=0x%08x)."), l_result);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
			break;;
		} else {
			if (l_result != MSC_COMM_OK) {
				// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C
				if (RETRY_TIMES <= l_retry++) {
					// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½ÍƒVï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
					send_motion_message(-1, 122);	// ï¿½Ê�M(Read2)ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½Pï¿½ï¿½ï¿½ï¿½
					l_result = -1;
					_stprintf(l_tszText, _T("Communication retry out(ReadCommand2:1=0x%08x). Retry times : %d."), l_result, RETRY_TIMES);
					if (0 == g_pCMotSysDlg->m_bOnClosing) {
						::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
					}
					break;
				}
			} else {
				// ï¿½ï¿½ï¿½í‰�ï¿½ï¿½
				memcpy(l_pRecvData, RecvData, l_RecvLength);
				// ï¿½ï¿½ï¿½[ï¿½hï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½o
				j = 0; ii = 0;
				for (i = 0; i < 256; i++) {
					if (l_pRecvData[i] == COM_CR) {
						l_Sum = l_pRecvData[i + 1];
						l_pRecvData[i + 1] = 0x00;
						strcpy(&((*l_pReadData)[j][0]), &(l_pRecvData[ii]));
						ii = i + 2;
						i = i + 1;
						// ï¿½Tï¿½ï¿½ï¿½Ìƒ`ï¿½Fï¿½bï¿½N
						if (l_Sum != g_pCSio->CalcCheckSum(&((*l_pReadData)[j][0]), 1)) {
							// ï¿½Tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
							l_result = -3; break;
						}
						if (12 < strlen(&((*l_pReadData)[j][0]))) {
							// ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(10ï¿½ï¿½ï¿½ï¿½ï¿½ğ’´‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½)
							l_result = -2; break;
						}
						// CR ï¿½ï¿½ NULL ï¿½É‚ï¿½ï¿½ï¿½(ï¿½Iï¿½[ï¿½ï¿½Ç‰ï¿½)
						for (k = 0; k < strlen(&((*l_pReadData)[j][0])); k++) {
							if ((*l_pReadData)[j][k] == COM_CR) {
								(*l_pReadData)[j][k] = 0x00; break;
							}
						}
						j++;
						if (DataNum <= j) {
							if (l_pRecvData[ii] != COM_ACK) {
								// ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
								l_result = -4; break;
							} else {
								break;
							}
						}
					}
				}
				if ((255 <= i) || (l_result != MSC_COMM_OK)) {
					// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C
					if (RETRY_TIMES <= l_retry++) {
						// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½ÍƒVï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
						send_motion_message(-1, 123);	// ï¿½Ê�M(Read2)ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½Qï¿½ï¿½ï¿½ï¿½
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

// ï¿½ï¿½ï¿½Cï¿½gï¿½Rï¿½}ï¿½ï¿½ï¿½h
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
			// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½
			send_motion_message(-1, 131);	// ï¿½Ê�M(Write)ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½
			_stprintf(l_tszText, _T("Communication Functions is not work. (0x%08x)"), l_result);
			if (0 == g_pCMotSysDlg->m_bOnClosing) {
				::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
			}
			break;;
		} else {
			if (l_result != MSC_COMM_OK) {
				// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½C
				if (RETRY_TIMES <= l_retry++) {
					// ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½ÍƒVï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
					send_motion_message(-1, 132);	// ï¿½Ê�M(Write)ï¿½ï¿½ï¿½gï¿½ï¿½ï¿½Cï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½ï¿½
					l_result = -1;
					_stprintf(l_tszText, _T("Communication retry out(WriteCommand:1). Retry times : %d."), RETRY_TIMES);
					if (0 == g_pCMotSysDlg->m_bOnClosing) {
						::MessageBox(NULL, l_tszText, MSD_MESSAGEBOX_TITLE, MB_OK | MB_SYSTEMMODAL);
					}
					break;
				}
			} else {
				// ï¿½ï¿½ï¿½í‰�ï¿½ï¿½
				break;;
			}
		}
	}

	return l_result;
}

// DIO ï¿½Iï¿½[ï¿½vï¿½ï¿½
DWORD CC_DioOpen(void)
{
	DWORD	dwRet;

	g_CC_DrvNo = ::GetPrivateProfileInt(_T("DIO"), _T("DrvNo"), 2, SERVO_INI);
	g_CC_GrpNo = ::GetPrivateProfileInt(_T("DIO"), _T("GrpNo"), 1, SERVO_INI);

	dwRet = DioOpen(&g_CC_hDrv, g_CC_DrvNo, g_CC_GrpNo);

	return dwRet;
}
// DIO ï¿½Nï¿½ï¿½ï¿½[ï¿½Y
DWORD CC_DioClose(void)
{
	DWORD	dwRet;

	if (g_CC_hDrv == NULL) {
		// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		return -1;
	}

	dwRet = DioClose(g_CC_hDrv);
	g_CC_hDrv = NULL;

	return dwRet;
}
// DIO ï¿½|ï¿½[ï¿½gï¿½ï¿½ï¿½[ï¿½h
DWORD CC_DioInpDWord(DWORD *pdwRData)
{
	DWORD	l_dwRet = 0;
	BYTE	l_ucInPort0, l_ucInPort1, l_ucInPort2, l_ucInPort3;

	if (g_CC_hDrv == NULL) {
		// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
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
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- { ---------- */
	CC_DioLog(g_CC_InPort, g_CC_OutPort);
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- } ---------- */

	return l_dwRet;
}
// DIO ï¿½rï¿½bï¿½gï¿½ï¿½ï¿½[ï¿½h
DWORD CC_DioInpBit(WORD wRBitNo, BYTE *pucRBitData)
{
	DWORD	l_dwRet = 0;
	BYTE	l_ucBitData;

	if (g_CC_hDrv == NULL) {
		// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		return -1;
	}
	if (31 < wRBitNo) {
		// ï¿½rï¿½bï¿½gï¿½Ô�ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½
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
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- { ---------- */
	CC_DioLog(g_CC_InPort, g_CC_OutPort);
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- } ---------- */

	return l_dwRet;
}
// DIO ï¿½rï¿½bï¿½gï¿½ï¿½ï¿½Cï¿½g
DWORD CC_DioOutBit(WORD wWBitNo, BYTE ucWBitData)
{
	DWORD	l_dwRet = 0;

	if (g_CC_hDrv == NULL) {
		// ï¿½Iï¿½[ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		return -1;
	}
	if (31 < wWBitNo) {
		// ï¿½rï¿½bï¿½gï¿½Ô�ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½
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
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- { ---------- */
	CC_DioLog(g_CC_InPort, g_CC_OutPort);
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- } ---------- */

	return l_dwRet;
}
// DIO ï¿½Gï¿½ï¿½ï¿½[
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
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- { ---------- */
/* DIO ï¿½ï¿½ï¿½O	*/
void CC_DioLog(DWORD dwDi, DWORD dwDo)
{
	static DWORD ls_dwDiPre = 0;
	static DWORD ls_dwDoPre = 0;

	if (0 != g_pCMotSysDlg) {
		if ((ls_dwDiPre != dwDi) || (ls_dwDoPre != dwDo)) {
			/* ï¿½Ï‰ï¿½ï¿½Lï¿½ï¿½	*/
			ls_dwDiPre = dwDi;
			ls_dwDoPre = dwDo;
			g_pCMotSysDlg->DioLogging(dwDi, dwDo);
		}
	}
}
/* added 2009.12.14 hmenjo MotSys DIO ï¿½ï¿½ï¿½Oï¿½Ç‰ï¿½ ---------- } ---------- */

// WD ï¿½Ä�ï¿½
BOOL	lg_bWD_once = FALSE;
BOOL CC_checkWD(void)
{
	BOOL	l_rc = TRUE;
	BYTE	l_BitData = 0;

	if (g_CC_bInitialize != TRUE) {
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½Ä�ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Dï¿½ï¿½ï¿½íƒŠï¿½^ï¿½[ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½ï¿½
	} else {
		// ï¿½ï¿½xï¿½Å‚ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½H
		if (lg_bWD_once != TRUE) {
			// ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
			// ï¿½Iï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
			if (CC_DioInpBit(0, &l_BitData) != 0) {
				CC_DioError(); l_BitData = 0;
			}
			if (l_BitData != 0) {
				lg_bWD_once = TRUE;
			}
		} else {
			// ï¿½ï¿½ï¿½ï¿½
			if (CC_DioInpBit(0, &l_BitData) != 0) {
				CC_DioError(); l_BitData = 0;
			}
			if (l_BitData == 0) {
				// ï¿½Iï¿½tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
				l_rc = FALSE;
				lg_bWD_once = FALSE;
			}
		}
	}

	return l_rc;
}

// ï¿½Tï¿½[ï¿½{ ON/OFF
BOOL CC_ServoONOFF(int on_off)
{
	BYTE	l_ucServo;

	if (on_off == -1) {
		// ï¿½ï¿½Ô‚ï¿½Ô‚ï¿½ï¿½ï¿½ï¿½ï¿½
	} else if (on_off == TRUE) {
		// ï¿½Tï¿½[ï¿½{ ON
		if (CC_DioOutBit(CC_OUTBIT_SERVO, 1) != 0) {
			CC_DioError();	return FALSE;	// ï¿½ï¿½ï¿½s
		}
		g_CC_ServoONReq = TRUE;
		g_CC_ServoOFFReq = FALSE;
	} else {
		// ï¿½Tï¿½[ï¿½{ OFF
		if (CC_DioOutBit(CC_OUTBIT_SERVO, 0) != 0) {
			CC_DioError();	return FALSE;	// ï¿½ï¿½ï¿½s
		}
		g_CC_ServoONReq = FALSE;
		g_CC_ServoOFFReq = TRUE;
	}

	// ï¿½ï¿½Ô‚ï¿½Ç�oï¿½ï¿½
	if (CC_DioInpBit(CC_INBIT_SERVO, &l_ucServo) != 0) {
		CC_DioError();	return FALSE;	// ï¿½ï¿½ï¿½s
	}

	return (l_ucServo != 0)? TRUE : FALSE;
}

// ï¿½ï¿½ï¿½uï¿½[ï¿½gï¿½ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½bï¿½Zï¿½[ï¿½W
void CC_MsgReboot(void)
{
	g_CC_ComError = TRUE;
	CC_ServoOFF_RunOFF();
	CC_ServoFailure();
	g_pCMotSysDlg->ShowAlarm(2, 0, 0, 0);
}

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
UINT CC_Main_Loop(LPVOID pParam)
{
	// ï¿½Ï�ï¿½ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
	CC_Initial();

	// DO ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	for (int i = 0; i < 32; i++) {
		CC_DioOutBit(i, 0);
	}
	// ï¿½Tï¿½[ï¿½{ï¿½ï¿½ï¿½jï¿½bï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	g_CC_ThreadLoop = TRUE;
	g_CC_CommAbort = FALSE;
	BOOL l_bInitRc = CC_InitServo(pParam);
	if (0 == l_bInitRc) {
		g_CC_ThreadLoop = FALSE;
		g_pCMotSysDlg->PostMessage(MOTDRV_SRVINITCOMP, (WPARAM) FALSE, 0);
	} else {
		g_CC_ThreadLoop = TRUE;
		CC_ChangeGain(CC_AXIS_X, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½
		CC_ChangeGain(CC_AXIS_Y, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½
		CC_ChangeGain(CC_AXIS_Z, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½
		CC_ChangeGain(CC_AXIS_Y, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½
		g_pCMotSysDlg->PostMessage(MOTDRV_SRVINITCOMP, (WPARAM) TRUE, 0);
	}

	while (g_CC_ThreadLoop == TRUE) {
/* modified 2009.10.15 hmenjo ï¿½^ï¿½Nï¿½gï¿½Zï¿½kï¿½pï¿½Xï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½Ô�kï¿½ï¿½ ---------- { ---------- */
////		::Sleep(200);	// FP ï¿½Vï¿½ï¿½ï¿½[ï¿½Yï¿½Å‚Í‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
//		::Sleep(100);
/* modified 2009.10.15 hmenjo ï¿½^ï¿½Nï¿½gï¿½Zï¿½kï¿½pï¿½Xï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½Ô�kï¿½ï¿½ ----------			   */
		::Sleep(50);
/* modified 2009.10.15 hmenjo ï¿½^ï¿½Nï¿½gï¿½Zï¿½kï¿½pï¿½Xï¿½ï¿½ï¿½[ï¿½vï¿½ï¿½ï¿½Ô�kï¿½ï¿½ ---------- } ---------- */

		if (g_CC_ComError != TRUE) {
			// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ WD ï¿½`ï¿½Fï¿½bï¿½N
			if (CC_checkWD() != TRUE) {
				CC_ServoFailure();
				CC_ServoONOFF(0);
				g_pCMotSysDlg->ShowAlarm(1, 0, 0, 0);
			}
			// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ä�ï¿½
			CC_PollingAlarm();
			// ï¿½ï¿½Ô�æ“¾
			CC_PollingStatus();
			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç—ï¿½

#define	BEFORE_INP_WAIT	200	// INP ï¿½Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½
			CC_X_Manager();		// X ï¿½ï¿½
			CC_Y_Manager();		// Y ï¿½ï¿½
			CC_Z_Manager();		// Z ï¿½ï¿½
			CC_T_Manager();		// T ï¿½ï¿½

#if CC_DEB_BUTTON==0
			mmtestORGXdone();	//debdeb for test
			mmtestORGYdone();	//debdeb for test
			mmtestORGZdone();	//debdeb for test
			mmtestORGTdone();	//debdeb for test
#endif
		}
		// ï¿½Tï¿½[ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		CC_ServoON_Ctrl();
	}

	// DO ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	for (int i = 0; i < 32; i++) {
		CC_DioOutBit(i, 0);
	}

//	AfxEndThread(0);	// ï¿½ï¿½ï¿½ï¿½Å‚È‚ï¿½ï¿½Ä�C
	return (UINT) 0;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Å‚æ‚¢
}

// ï¿½Tï¿½[ï¿½{ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
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
			// ï¿½Tï¿½[ï¿½{ ON ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if (CC_DioInpDWord(&l_InpAlarm) != 0) {
				CC_DioError();	// ï¿½ï¿½ï¿½s
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
					// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
					l_prc = 1;
				}
			}
		} else if (g_CC_ServoOFFReq == TRUE) {
			l_prc = 3;	// ï¿½Tï¿½[ï¿½{ OFF ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		} else if (((g_CC_OutPort & (0x0001 << CC_OUTBIT_SERVO)) != 0)
				&& (CC_ServoONOFF(-1) == FALSE)) {
			// ï¿½Tï¿½[ï¿½{ ON ï¿½wï¿½ï¿½ ON ï¿½È‚Ì‚ï¿½ OFF ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½(EMG ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
			CC_ServoOFF_RunOFF();	// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½Sï¿½ï¿½ï¿½ï¿½~
			CC_ServoFailure();
			// ï¿½ï¿½ï¿½ï¿½~ï¿½\ï¿½ï¿½
			g_pCMotSysDlg->ShowAlarm(7, 0, 0, 0);
		}
		break;
	case 1:		// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½ï¿½
		if (CC_ServoONOFF(-1) == TRUE) {
			// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
			g_CC_ServoONReq = FALSE;
			l_prc = 0;
		} else {
			// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
			if ((g_CC_OutPort & (0x0001 << CC_OUTBIT_SERVO)) != 0) {
				// ï¿½Tï¿½[ï¿½{ ON ï¿½oï¿½Í‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
			} else {
				// ï¿½Tï¿½[ï¿½{ ON ï¿½oï¿½Í‚ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
				CC_ServoONOFF(1);	// ï¿½Tï¿½[ï¿½{ ON
			}
			ls_StartTime = GetTickCount();
			l_prc = 2;
		}
		break;
	case 2:		// ï¿½Tï¿½[ï¿½{ ON ï¿½mï¿½ï¿½Ò‚ï¿½
		if (CC_ServoONOFF(-1) == TRUE) {
			// ï¿½Tï¿½[ï¿½{ ON ï¿½mï¿½ï¿½
			g_CC_ServoONReq = FALSE;
			l_prc = 0;
		} else {
			// ï¿½ï¿½è��ï¿½ÔˆÈ“ï¿½ï¿½ï¿½ ON ï¿½ï¿½ï¿½È‚ï¿½ï¿½È‚ï¿½Ù�ï¿½
			l_CurrentTime = GetTickCount();
			if (ls_StartTime <= l_CurrentTime) {
				l_DelayTime = l_CurrentTime - ls_StartTime;
			} else {
				l_DelayTime = (((DWORD) 0xffffffff) - ls_StartTime) + l_CurrentTime;
			}
			if (CC_SERVO_ONOFF_TIMEOUT < l_DelayTime) {
				CC_ServoOFF_RunOFF();	// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½Sï¿½ï¿½ï¿½ï¿½~
				CC_ServoFailure();
				g_CC_ServoONOFFerr = TRUE;
				l_prc = 5;
				g_pCMotSysDlg->ShowAlarm(6, 0, 1, 0);	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
			}
		}
		break;
	case 3:		// ï¿½Tï¿½[ï¿½{ OFF ï¿½ï¿½ï¿½ï¿½
		if (CC_ServoONOFF(-1) == FALSE) {
			// ï¿½Tï¿½[ï¿½{ OFF ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
			g_CC_ServoOFFReq = FALSE;
			l_prc = 0;
		} else {
			// ï¿½Tï¿½[ï¿½{ OFF ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
			if ((g_CC_OutPort & (0x0001 << CC_OUTBIT_SERVO)) == 0) {
				// ï¿½Tï¿½[ï¿½{ OFF ï¿½oï¿½Í‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
			} else {
				// ï¿½Tï¿½[ï¿½{ OFF ï¿½oï¿½Í‚ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
				CC_ServoOFF_RunOFF();
			}
			ls_StartTime = GetTickCount();
			l_prc = 4;
		}
		break;
	case 4:		// ï¿½Tï¿½[ï¿½{ OFF ï¿½mï¿½ï¿½Ò‚ï¿½
		if (CC_ServoONOFF(-1) == FALSE) {
			// ï¿½Tï¿½[ï¿½{ OFF ï¿½mï¿½ï¿½
			g_CC_ServoOFFReq = FALSE;
			l_prc = 0;
		} else {
			// ï¿½ï¿½è��ï¿½ÔˆÈ“ï¿½ï¿½ï¿½ OFF ï¿½ï¿½ï¿½È‚ï¿½ï¿½È‚ï¿½Ù�ï¿½
			l_CurrentTime = GetTickCount();
			if (ls_StartTime <= l_CurrentTime) {
				l_DelayTime = l_CurrentTime - ls_StartTime;
			} else {
				l_DelayTime = (((DWORD) 0xffffffff) - ls_StartTime) + l_CurrentTime;
			}
			if (CC_SERVO_ONOFF_TIMEOUT < l_DelayTime) {
				CC_ServoOFF_RunOFF();	// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½Sï¿½ï¿½ï¿½ï¿½~
				CC_ServoFailure();
				g_CC_ServoONOFFerr = TRUE;
				l_prc = 5;
				g_pCMotSysDlg->ShowAlarm(6, 0, 0, 0);	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
			}
		}
		break;
	case 5:		// ï¿½Tï¿½[ï¿½{ ON ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
		if (g_CC_ServoONOFFerr == FALSE) {
			l_prc = 0;
		}
		break;
	}
}

// ï¿½ï¿½Ô�æ“¾
WORD CC_PollingStatus(void)
{
	char	l_Recvdata[256];
	TCHAR	l_tszText1[32];
	char	l_szText1[32];
	int		l_result;
	WORD	l_AxisNo;			// ï¿½ï¿½ï¿½Ô�ï¿½
	static WORD	ls_AxisLoop = 1;	// ï¿½ï¿½ï¿½Ô�ï¿½
	char	l_ReadData[16][12];		// ï¿½ï¿½ï¿½[ï¿½hï¿½fï¿½[ï¿½^
									/* ï¿½Yï¿½ï¿½	X ï¿½ï¿½		YM ï¿½ï¿½		Z ï¿½ï¿½		T ï¿½ï¿½		YS ï¿½ï¿½
										0	ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½h	ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½h	ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½h	ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½h	ï¿½Ê’u
										1	ï¿½Ê’u		ï¿½Ê’u		ï¿½Ê’u		ï¿½Ê’u		ï¿½ï¿½ï¿½x
										2	ï¿½ï¿½ï¿½x		ï¿½ï¿½ï¿½x		ï¿½ï¿½ï¿½x		ï¿½ï¿½ï¿½x		ï¿½dï¿½ï¿½ï¿½l
										3	ï¿½dï¿½ï¿½ï¿½l		ï¿½dï¿½ï¿½ï¿½l		ï¿½wï¿½ßˆÊ’u	ï¿½wï¿½ßˆÊ’u	ï¿½wï¿½ßˆÊ’u
										4	ï¿½wï¿½ßˆÊ’u	ï¿½wï¿½ßˆÊ’u	ï¿½wï¿½ß‘ï¿½ï¿½x	ï¿½wï¿½ß‘ï¿½ï¿½x	ï¿½wï¿½ß‘ï¿½ï¿½x
										5	ï¿½wï¿½ß‘ï¿½ï¿½x	ï¿½wï¿½ß‘ï¿½ï¿½x	ï¿½ï¿½ï¿½		ï¿½ï¿½ï¿½		ï¿½wï¿½ß“dï¿½ï¿½ï¿½l
										6	ï¿½wï¿½ß“dï¿½ï¿½ï¿½l	ï¿½wï¿½ß“dï¿½ï¿½ï¿½l	ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½	ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½	ï¿½ï¿½ï¿½
										7	ï¿½ï¿½ï¿½		ï¿½ï¿½ï¿½		ï¿½Ê’uï¿½Î�ï¿½	ï¿½Ê’uï¿½Î�ï¿½	ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½
										8	ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½	ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½							ï¿½Ê’uï¿½Î�ï¿½
										9	ï¿½Ê’uï¿½Î�ï¿½	ï¿½Ê’uï¿½Î�ï¿½
									 */
	{
		switch (l_AxisNo = ls_AxisLoop++) {
		case CC_AXIS_X:	// X ï¿½ï¿½
		case CC_AXIS_Y:	// YM ï¿½ï¿½
			if (0 != motion_data[CC_CnvAxisFromSPT(l_AxisNo)].bEnable) {
				sprintf(l_szText1, "P%1u50..%1u59", l_AxisNo + 1, l_AxisNo + 1);
				if ((l_result = CC_ReadCommand2(l_szText1, l_Recvdata, 10, l_ReadData)) < 0) {
					CC_MsgReboot();	return ls_AxisLoop;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
				}
				g_CC_AxisInfo[l_AxisNo].Mode = atoi(l_ReadData[0]);				// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½h
				g_CC_AxisInfo[l_AxisNo].Position = atol(l_ReadData[1]);			// ï¿½Ê’u
				g_CC_AxisInfo[l_AxisNo].Velocity = atol(l_ReadData[2]);			// ï¿½ï¿½ï¿½x
				g_CC_AxisInfo[l_AxisNo].Current = atol(l_ReadData[3]);			// ï¿½dï¿½ï¿½ï¿½l
				g_CC_AxisInfo[l_AxisNo].Order_Position = atol(l_ReadData[4]);	// ï¿½wï¿½ßˆÊ’u
				g_CC_AxisInfo[l_AxisNo].Order_Velocity = atol(l_ReadData[5]);	// ï¿½wï¿½ß‘ï¿½ï¿½x
				g_CC_AxisInfo[l_AxisNo].Order_Current = atol(l_ReadData[6]);	// ï¿½wï¿½ß“dï¿½ï¿½ï¿½l
				g_CC_AxisInfo[l_AxisNo].Status = atoi(l_ReadData[7]);			// ï¿½ï¿½ï¿½
				g_CC_AxisInfo[l_AxisNo].d_Position = atoi(l_ReadData[9]);		// ï¿½Ê’uï¿½Î�ï¿½
				g_pCMotSysDlg->DispdPos(CC_CnvAxisFromSPT(l_AxisNo), g_CC_AxisInfo[l_AxisNo].d_Position);
				g_pCMotSysDlg->CnvWordToBin(g_CC_AxisInfo[l_AxisNo].Status, l_tszText1, 2);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[1][CC_CnvAxisFromSPT(l_AxisNo)], l_tszText1);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[2][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[0]);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[3][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[1]);
			}
			break;
		case CC_AXIS_Z:	// Z ï¿½ï¿½
		case CC_AXIS_T:	// T ï¿½ï¿½
			if (0 != motion_data[CC_CnvAxisFromSPT(l_AxisNo)].bEnable) {
				sprintf(l_szText1, "P%1u50..%1u57", l_AxisNo + 1, l_AxisNo + 1);
				if ((l_result = CC_ReadCommand2(l_szText1, l_Recvdata, 8, l_ReadData)) < 0) {
					CC_MsgReboot();	return ls_AxisLoop;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
				}
				g_CC_AxisInfo[l_AxisNo].Mode = atoi(l_ReadData[0]);				// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½h
				g_CC_AxisInfo[l_AxisNo].Position = atol(l_ReadData[1]);			// ï¿½Ê’u
				g_CC_AxisInfo[l_AxisNo].Velocity = atol(l_ReadData[2]);			// ï¿½ï¿½ï¿½x
				g_CC_AxisInfo[l_AxisNo].Order_Position = atol(l_ReadData[3]);	// ï¿½wï¿½ßˆÊ’u
				g_CC_AxisInfo[l_AxisNo].Order_Velocity = atol(l_ReadData[4]);	// ï¿½wï¿½ß‘ï¿½ï¿½x
				g_CC_AxisInfo[l_AxisNo].Status = atoi(l_ReadData[5]);			// ï¿½ï¿½ï¿½
				g_CC_AxisInfo[l_AxisNo].d_Position = atoi(l_ReadData[7]);		// ï¿½Ê’uï¿½Î�ï¿½
				g_pCMotSysDlg->DispdPos(CC_CnvAxisFromSPT(l_AxisNo), g_CC_AxisInfo[l_AxisNo].d_Position);
				g_pCMotSysDlg->CnvWordToBin(g_CC_AxisInfo[l_AxisNo].Status, l_tszText1, 2);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[1][CC_CnvAxisFromSPT(l_AxisNo)], l_tszText1);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[2][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[0]);
				g_pCMotSysDlg->SetDlgItemText(g_iDlgCtrlID[3][CC_CnvAxisFromSPT(l_AxisNo)], l_ReadData[1]);
			}
			break;
		default:	// YS ï¿½ï¿½
			if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Y)].bEnable) {
				if ((l_result = CC_ReadCommand2("P261..269", l_Recvdata, 9, l_ReadData)) < 0) {
					CC_MsgReboot();	return ls_AxisLoop;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
				}
				g_CC_AxisInfo[l_AxisNo].Position = atol(l_ReadData[0]);			// ï¿½Ê’u
				g_CC_AxisInfo[l_AxisNo].Velocity = atol(l_ReadData[1]);			// ï¿½ï¿½ï¿½x
				g_CC_AxisInfo[l_AxisNo].Current = atol(l_ReadData[2]);			// ï¿½dï¿½ï¿½ï¿½l
				g_CC_AxisInfo[l_AxisNo].Order_Position = atol(l_ReadData[3]);	// ï¿½wï¿½ßˆÊ’u
				g_CC_AxisInfo[l_AxisNo].Order_Velocity = atol(l_ReadData[4]);	// ï¿½wï¿½ß‘ï¿½ï¿½x
				g_CC_AxisInfo[l_AxisNo].Order_Current = atol(l_ReadData[5]);	// ï¿½wï¿½ß“dï¿½ï¿½ï¿½l
				g_CC_AxisInfo[l_AxisNo].Status = atoi(l_ReadData[6]);			// ï¿½ï¿½ï¿½
				g_CC_AxisInfo[l_AxisNo].d_Position = atoi(l_ReadData[8]);		// ï¿½Ê’uï¿½Î�ï¿½
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


// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Ä�ï¿½
void CC_PollingAlarm(void)
{
	DWORD	l_InpAlarm;
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_Code1;
	WORD	l_Code2;
	int		i;
	WORD	l_AxisNo;			// ï¿½ï¿½ï¿½Ô�ï¿½
	BOOL	*l_pSetAlarm;		// ï¿½Zï¿½bï¿½gï¿½tï¿½ï¿½ï¿½O
	BOOL	*l_pClearAlarm;		// ï¿½Nï¿½ï¿½ï¿½Aï¿½tï¿½ï¿½ï¿½O
	WORD	l_SetBit;			// ï¿½ï¿½ï¿½ï¿½ï¿½rï¿½bï¿½g
	WORD	l_ClearBit;			// ï¿½Nï¿½ï¿½ï¿½Aï¿½rï¿½bï¿½g
	char	l_DetailAddr[8];	// ï¿½Ú�×ƒRï¿½[ï¿½hï¿½ÌƒAï¿½hï¿½ï¿½ï¿½X
	char	l_DetailAddr2[8];	// ï¿½Ú�×ƒRï¿½[ï¿½hï¿½ÌƒAï¿½hï¿½ï¿½ï¿½X
	static DWORD	ls_ClearAlarmTimer[CC_AXIS_NUM];	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½Aï¿½^ï¿½Cï¿½}
	DWORD	*l_pClrAlmTimer;
	DWORD	l_CurrentTime, l_DelayTime;

	if (CC_DioInpDWord(&l_InpAlarm) != 0) {
		CC_DioError();	return;	// ï¿½ï¿½ï¿½s
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
//			if (l_AxisNo != -1) {	ï¿½oï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[2009.04.01]
			if (l_AxisNo < CC_AXIS_NUM) {
				if		  ((*l_pSetAlarm != TRUE) && (*l_pClearAlarm != TRUE)) {
					// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò‚ï¿½
					if ((~l_InpAlarm) & l_SetBit) {
						// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
						*l_pClearAlarm = FALSE;	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A OFF
						// ï¿½Ú�×ƒRï¿½[ï¿½hï¿½ï¿½Ç�oï¿½ï¿½
						if ((l_result = CC_ReadCommand(l_DetailAddr, l_Recvdata)) < 0) {
							CC_MsgReboot();	return;		// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
						}
						l_Code1 = atoi(l_Recvdata);
						if ((l_AxisNo == CC_AXIS_Y) && (0 != motion_data[l_AxisNo].bEnable)) {
							if ((l_result = CC_ReadCommand(l_DetailAddr2, l_Recvdata)) < 0) {
								CC_MsgReboot(); return;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
							}
							l_Code2 = atoi(l_Recvdata);
						} else {
							l_Code2 = 0;
						}
						*l_pSetAlarm = TRUE;	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Zï¿½bï¿½g ON
						CC_ServoOFF_RunOFF();	// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½Sï¿½ï¿½ï¿½ï¿½~
						CC_ServoFailure();
						// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½\ï¿½ï¿½
						g_pCMotSysDlg->ShowAlarm(3, CC_CnvAxisFromSPT(l_AxisNo), l_Code1, l_Code2);
					}
				} else if (*l_pSetAlarm == TRUE) {
					// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½Aï¿½Ò‚ï¿½
					if (*l_pClearAlarm == TRUE) {
						// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A ON
 						if (CC_DioOutBit(l_ClearBit, 1) != 0) {
							CC_DioError();	return;	// ï¿½ï¿½ï¿½s
						}
						*l_pSetAlarm = FALSE;	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Zï¿½bï¿½g OFF
						// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½Aï¿½^ï¿½Cï¿½}ï¿½Jï¿½n
						*l_pClrAlmTimer = GetTickCount();
					}
				} else if ((*l_pSetAlarm != TRUE) && (*l_pClearAlarm == TRUE)) {
					// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½rï¿½bï¿½g OFF ï¿½Ò‚ï¿½
					if (((~l_InpAlarm) & l_SetBit) == 0) {
						// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½Aï¿½rï¿½bï¿½g OFF
						if (CC_DioOutBit(l_ClearBit, 0) != 0) {
							CC_DioError();	return;	// ï¿½ï¿½ï¿½s
						}
						*l_pClearAlarm = FALSE;	// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A OFF
						CC_ServoONOFF(1);	// ï¿½Tï¿½[ï¿½{ ON
					} else {
						// ï¿½Aï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½Aï¿½^ï¿½Cï¿½}ï¿½Ä�ï¿½
						l_CurrentTime = GetTickCount();
						if (*l_pClrAlmTimer <= l_CurrentTime) {
							l_DelayTime = l_CurrentTime - *l_pClrAlmTimer;
						} else {
							l_DelayTime = (((DWORD) 0xffffffff) - *l_pClrAlmTimer) + l_CurrentTime;
						}
						if (CC_CLEAR_ALARM_TIMEOUT < l_DelayTime) {
							//return;	// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
							*l_pClearAlarm = TRUE;
						}
					}
				}
			}
		}
	}
}

// ï¿½Ï�ï¿½ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
void CC_Initial(void)
{
	memset(&g_CC_bSetAlarm, 0, sizeof(g_CC_bSetAlarm));
	memset(&g_CC_bClearAlarm, 0, sizeof(g_CC_bClearAlarm));
	g_CC_ServoONReq = TRUE;		// ï¿½Tï¿½[ï¿½{ ON ï¿½vï¿½ï¿½
	g_CC_ServoOFFReq = FALSE;	// ï¿½Tï¿½[ï¿½{ OFF ï¿½vï¿½ï¿½
	g_CC_ServoONOFFerr = FALSE;	// ï¿½Tï¿½[ï¿½{ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Gï¿½ï¿½ï¿½[
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

// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½ï¿½ï¿½Jï¿½nï¿½Mï¿½ï¿½ï¿½ï¿½ OFF
void CC_ServoOFF_RunOFF(void)
{
	// ï¿½Tï¿½[ï¿½{ OFF
	CC_ServoONOFF(0);
	// ï¿½Sï¿½ï¿½ï¿½ï¿½~
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_X)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_X, 0);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Y)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_Y, 0);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_Z)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_Z, 0);}
	if (0 != motion_data[CC_CnvAxisFromSPT(CC_AXIS_T)].bEnable) {CC_DioOutBit(CC_OUTBIT_RUN_T, 0);}
}

// ï¿½Tï¿½[ï¿½{ ï¿½nï¿½ÌˆÙ�ï¿½ï¿½~
void CC_ServoFailure(void)
{
	short	axis;

	// ï¿½Tï¿½[ï¿½{ OFFï¿½Cï¿½ï¿½ï¿½Jï¿½nï¿½Mï¿½ï¿½ OFF ï¿½ï¿½ CC_ServoOFF_RunOFF(void) ï¿½Å�sï¿½ï¿½

	for (axis = 0; axis < CC_AXIS_NUM; axis++) {
		if ((g_CC_Axis_prc[axis] != 0) || (g_CC_Axis_mode[axis] != 0)) {
			// ï¿½ï¿½ï¿½ì’†ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if (0 != g_CC_Axis_prc[axis]) {
				// ï¿½Ù�ï¿½Iï¿½ï¿½ï¿½ï¿½Ê’mï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
				send_motion_message(CC_CnvAxisFromSPT(axis), MM_FAILURE);
			}
			g_CC_Axis_prc[axis] = 0;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚ï¿½ï¿½Nï¿½ï¿½ï¿½A
			g_CC_Axis_mode[axis] = 0;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
		}
	}
}

// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç—ï¿½
void CC_X_Manager(void)		// X ï¿½ï¿½
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_X;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚Ì‚Å�Iï¿½ï¿½
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Oï¿½Ì‘Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_X, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_X, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
			send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
			g_CC_Axis_mode[l_axis] = 0;
			g_CC_Axis_pattern[l_axis] = 0;
			CC_ChangeGain(l_axis, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½ hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- } ---------- */
}
void CC_Y_Manager(void)		// Y ï¿½ï¿½
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_Y;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚Ì‚Å�Iï¿½ï¿½
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Oï¿½Ì‘Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_Y, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Y, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
			send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
			g_CC_Axis_mode[l_axis] = 0;
			g_CC_Axis_pattern[l_axis] = 0;
			CC_ChangeGain(l_axis, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½ hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- } ---------- */
}
void CC_Z_Manager(void)		// Z ï¿½ï¿½
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_Z;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚Ì‚Å�Iï¿½ï¿½
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Oï¿½Ì‘Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_Z, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Z, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
			send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
			g_CC_Axis_mode[l_axis] = 0;
			g_CC_Axis_pattern[l_axis] = 0;
			CC_ChangeGain(l_axis, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½ hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- } ---------- */
}
void CC_T_Manager(void)		// T ï¿½ï¿½
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;
	int		l_axis = CC_AXIS_T;
	char	l_szCmd[16];

	if (0 == motion_data[l_axis].bEnable) {
		return;		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚Ì‚Å�Iï¿½ï¿½
	}

	switch (g_CC_Axis_prc[l_axis]) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Oï¿½Ì‘Ò‚ï¿½ï¿½ï¿½ï¿½ï¿½
		::Sleep(BEFORE_INP_WAIT);
		g_CC_Axis_prc[l_axis] = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_T, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			g_CC_Axis_prc[l_axis] = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		sprintf(l_szCmd, "P%1u50", l_axis + 1);
		if ((l_result = CC_ReadCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_T, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
			sprintf(l_szCmd, "P%1u00=0", l_axis + 1);
			if ((l_result = CC_WriteCommand(l_szCmd, l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			g_CC_Axis_prc[l_axis] = 0;
// ï¿½È‰ï¿½ï¿½ï¿½ if ï¿½uï¿½ï¿½ï¿½bï¿½Nï¿½Í—ï¿½ï¿½Rï¿½sï¿½ï¿½ï¿½Å‚ï¿½ï¿½Dhmenjo 2009.03.27
			if ((g_CC_Axis_ORG_Req[l_axis] == TRUE) && (g_CC_Axis_mode[l_axis] == 4)) {
				// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Oï¿½ï¿½ INC ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚ÅŒï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½sï¿½ï¿½
				CC_motion_go_home(l_axis);
			} else {
				send_motion_message(CC_CnvAxisFromSPT(l_axis), MM_DONE);
				g_CC_Axis_mode[l_axis] = 0;
				g_CC_Axis_pattern[l_axis] = 0;
			}
			CC_ChangeGain(l_axis, 1);	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½ hmenjo4 20030801
		}
		break;
	}
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- { ---------- */
	CC_MotTimeoutCheck(l_axis, &(g_CC_Axis_prc[l_axis]));
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- } ---------- */
}

/*
 *	motion.c ï¿½ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½Ì‘ï¿½ÖŠÖ�ï¿½ï¿½Q
 */
// ï¿½eï¿½ï¿½ï¿½ÌƒRï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ï¿½Ç�oï¿½ï¿½
long CC_get_true_position(short axis)
{
	char	l_CountAddr[6];	// ï¿½Jï¿½Eï¿½ï¿½ï¿½^ï¿½ÌƒAï¿½hï¿½ï¿½ï¿½X
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
		CC_MsgReboot();		// ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Gï¿½ï¿½ï¿½[
		g_CC_Count[axis] = 0;
	} else {
		g_CC_Count[axis] = atol(l_Recvdata);
	}

	return g_CC_Count[axis];
}

// ï¿½Ù‹}ï¿½ï¿½~ï¿½ï¿½ï¿½ï¿½
void CC_local_emergency_stop(short axis)
{
	WORD	l_RunBit;			// ï¿½Jï¿½n/ï¿½ï¿½~ ï¿½rï¿½bï¿½g
	char	l_cMode[32];		// ï¿½ï¿½ï¿½[ï¿½hï¿½Aï¿½hï¿½ï¿½ï¿½X
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
		// ï¿½ï¿½ï¿½ì’†ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�Iï¿½ï¿½ï¿½ï¿½Ê’mï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
		send_motion_message(CC_CnvAxisFromSPT(axis), MM_STOPPED);
	}
	g_CC_Axis_prc[axis] = 0;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚ï¿½ï¿½Nï¿½ï¿½ï¿½A
	g_CC_Axis_mode[axis] = 0;	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	// ï¿½ï¿½~
	if (CC_DioOutBit(l_RunBit, 0) != 0) {
		CC_DioError();	return;	// ï¿½ï¿½ï¿½s
	}
	// ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½A
	sprintf(l_cMode, "P%1u00=0", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return ;}
	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½
	CC_ChangeGain(axis, 1);
}

// HP ï¿½Ê’uï¿½ï¿½İ’ï¿½
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

// ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½xï¿½Åƒï¿½ï¿½~ï¿½bï¿½gï¿½Ü‚ÅˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½
int CC_motion_move_at_speed(short axis, short velocity)
{
	long	l_PLimit;
	long	l_MLimit;
	long	l_Destination_Pos;
	long	l_Velocity;
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// ï¿½Jï¿½nï¿½rï¿½bï¿½g
	char	l_cMode[32];	// ï¿½ï¿½ï¿½[ï¿½hï¿½Aï¿½hï¿½ï¿½ï¿½X

	// Z ï¿½ï¿½ï¿½Ì�ê�‡ï¿½Í‘ï¿½ï¿½xï¿½ğ”½“]
	if (axis == CC_AXIS_Z) {
		velocity = -velocity;
	}

	g_CC_Axis_pattern[axis] = 1;
//	CC_DispFuncParam(axis, velocity, 0);

	if (g_CC_Axis_prc[axis] != 0) {
		// ï¿½Ü‚ï¿½ï¿½Cï¿½Ú“ï¿½ï¿½ï¿½
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		return MS_NO_HARDWARE;
	}

	// ï¿½Ú•Wï¿½Ê’u(ï¿½ï¿½ï¿½~ï¿½bï¿½gï¿½Ì�ï¿½Oï¿½ï¿½İ’ï¿½)
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

	// ï¿½Ú•W
	l_Destination_Pos = (velocity < 0)? (l_MLimit + 4) : (l_PLimit - 4);

	CC_DispFuncParam(axis, velocity, l_Destination_Pos);

	// ï¿½ï¿½ï¿½x
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

	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½(ABS ï¿½Ú“ï¿½)
	g_CC_Axis_mode[axis] = 3;
	sprintf(l_cCmd[0], "P%1u18=%d", axis + 1, ServoParam[axis].ABS_AccelTimeT);
	sprintf(l_cCmd[1], "P%1u19=%d", axis + 1, ServoParam[axis].ABS_AccelTimeS);
	sprintf(l_cCmd[2], "P%1u20=%d", axis + 1, l_Velocity);
	sprintf(l_cCmd[3], "P%1u21=%d", axis + 1, l_Destination_Pos);
	if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ß‚ï¿½
	CC_ChangeGain(axis, 0);
	// ABS ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
	sprintf(l_cMode, "P%1u00=3", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ï¿½ï¿½ï¿½s */}
	CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
	g_CC_Axis_prc[axis] = 1;

	return MS_NO_ERROR;
}
// ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½xï¿½Å�wï¿½è‚µï¿½ï¿½ï¿½Ê’uï¿½ÉˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½
int CC_motion_move_to_position(short axis, long position, short velocity)
{
	long	l_PLimit;
	long	l_MLimit;
	long	l_Destination_Pos;
	long	l_Velocity;
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// ï¿½Jï¿½nï¿½rï¿½bï¿½g
	char	l_cMode[32];	// ï¿½ï¿½ï¿½[ï¿½hï¿½Aï¿½hï¿½ï¿½ï¿½X
	long	l_AccelTimeT, l_AccelTimeS;

	g_CC_Axis_pattern[axis] = 2;

	if (g_CC_Axis_prc[axis] != 0) {
		// ï¿½Ü‚ï¿½ï¿½Cï¿½Ú“ï¿½ï¿½ï¿½
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		return MS_NO_HARDWARE;
	}

	// ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½~ï¿½bï¿½gï¿½ï¿½ï¿½æ“¾
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

	// ï¿½Ú•W
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
	// Y ï¿½ï¿½ï¿½Ì‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚ğ’²�ï¿½
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

	// ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Xï¿½V
	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½(ABS ï¿½Ú“ï¿½)
	g_CC_Axis_mode[axis] = 3;
	sprintf(l_cCmd[0], "P%1u18=%d", axis + 1, l_AccelTimeT);
	sprintf(l_cCmd[1], "P%1u19=%d", axis + 1, l_AccelTimeS);
	sprintf(l_cCmd[2], "P%1u20=%d", axis + 1, l_Velocity);
	sprintf(l_cCmd[3], "P%1u21=%d", axis + 1, l_Destination_Pos);
	if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ß‚ï¿½
	CC_ChangeGain(axis, 0);
	sprintf(l_cMode, "P%1u00=3", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ï¿½ï¿½ï¿½s */}
	CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
	g_CC_Axis_prc[axis] = 1;

	return MS_NO_ERROR;
}
// ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½xï¿½Å�wï¿½è‚µï¿½ï¿½ï¿½Ê’uï¿½ÉˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í�gï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Æ‚É‚ï¿½ï¿½Ü‚ï¿½)
int CC_motion_move_at_speed_to_dest(short axis, long destination, short velocity)
{
	long	l_PLimit;
	long	l_MLimit;
	long	l_Destination_Pos;
	long	l_Velocity;
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// ï¿½Jï¿½nï¿½rï¿½bï¿½g
	char	l_cMode[32];	// ï¿½ï¿½ï¿½[ï¿½hï¿½Aï¿½hï¿½ï¿½ï¿½X
	long	l_AccelTimeT, l_AccelTimeS;

	g_CC_Axis_pattern[axis] = 3;

	if (g_CC_Axis_prc[axis] != 0) {
		// ï¿½Ü‚ï¿½ï¿½Cï¿½Ú“ï¿½ï¿½ï¿½
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
		return MS_NO_HARDWARE;
	}

	// ï¿½ï¿½ï¿½ï¿½ï¿½Ìƒï¿½ï¿½~ï¿½bï¿½gï¿½ï¿½ï¿½æ“¾
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

	// ï¿½Ú•W
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

	// Y ï¿½ï¿½ï¿½Ì‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚ğ’²�ï¿½
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

	// ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Xï¿½V
	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½(ABS ï¿½Ú“ï¿½)
	g_CC_Axis_mode[axis] = 3;
	sprintf(l_cCmd[0], "P%1u18=%d", axis + 1, l_AccelTimeT);
	sprintf(l_cCmd[1], "P%1u19=%d", axis + 1, l_AccelTimeS);
	sprintf(l_cCmd[2], "P%1u20=%d", axis + 1, l_Velocity);
	sprintf(l_cCmd[3], "P%1u21=%d", axis + 1, l_Destination_Pos);
	if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[1], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[2], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	if ((l_result = CC_WriteCommand(l_cCmd[3], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ß‚ï¿½
	CC_ChangeGain(axis, 0);
	// ABS ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
	sprintf(l_cMode, "P%1u00=3", axis + 1);
	if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ï¿½ï¿½ï¿½s */}
	CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
	g_CC_Axis_prc[axis] = 1;

	return MS_NO_ERROR;
}

// ï¿½wï¿½è‚µï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ HP ï¿½ÉˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½
int CC_motion_go_home(short axis)
{
	char	l_cCmd[9][32];
	char	l_Recvdata[256];
	int		l_result;
	WORD	l_RunBit;		// ï¿½Jï¿½nï¿½rï¿½bï¿½g
	char	l_cMode[32];	// ï¿½ï¿½ï¿½[ï¿½hï¿½Aï¿½hï¿½ï¿½ï¿½X
	long	l_Position;		// ï¿½Ú•Wï¿½Ê’u
	TCHAR	l_tszText[32];
	long	l_Theta;

	g_CC_Axis_pattern[axis] = 4;

	if (g_CC_Axis_prc[axis] != 0) {
		// ï¿½Ü‚ï¿½ï¿½Cï¿½Ú“ï¿½ï¿½ï¿½
		return MS_FUNCTION_BUSY;
	}
	if (CC_ServoONOFF(-1) != TRUE) {
		// ï¿½Tï¿½[ï¿½{ ON ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½
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

	// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Ï‚ï¿½ï¿½H
	if (g_CC_AxisInfo[axis].Status & 0x0008) {
		// ï¿½ï¿½ï¿½Aï¿½ï¿½ : HP ï¿½Ú“ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½
		// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½(HP ï¿½Ú“ï¿½)
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
		// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ß‚ï¿½
		CC_ChangeGain(axis, 0);
		// HP ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
		sprintf(l_cMode, "P%1u00=2", axis + 1);
		if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		// ï¿½Jï¿½nï¿½ï¿½ ON
		if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ï¿½ï¿½ï¿½s */}
		CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		g_CC_Axis_prc[axis] = 1;
	} else {
		// ï¿½ï¿½ï¿½ï¿½ï¿½A : ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½sï¿½ï¿½(ï¿½ï¿½ï¿½Aï¿½ï¿½CHP ï¿½Ú“ï¿½)
		if (axis == CC_AXIS_T) {
			/* Theta ï¿½Ì�ê�‡ï¿½Í‹Lï¿½ï¿½ï¿½Ê’uï¿½ï¿½ï¿½}175ï¿½ï¿½ï¿½ğ’´‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½Í�Cï¿½Ü‚ï¿½ INC ï¿½ÅŒï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½É�}30ï¿½ï¿½ï¿½Ú“ï¿½ï¿½C
			   ï¿½ï¿½ï¿½ÌŒï¿½Cï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½Æ‚ï¿½ï¿½ï¿½*/
			::GetPrivateProfileString(_T("CurrentPosition"), _T("Theta"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
//ï¿½ï¿½Lï¿½İ’ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ş�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½Hï¿½Hï¿½H[2009.03.25]
			l_Theta = _ttol(l_tszText);
			if ((g_CC_Axis_ORG_Req[axis] == FALSE) && ((l_Theta < -486111) || (486111 < l_Theta))) {
				// INC ï¿½Ú“ï¿½(30ï¿½ï¿½)ï¿½ï¿½ï¿½Å�ï¿½ï¿½É�sï¿½ï¿½
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
				// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ß‚ï¿½
				CC_ChangeGain(axis, 0);
				// INC ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
				sprintf(l_cCmd[0], "P%1u00=4", axis + 1);
				if ((l_result = CC_WriteCommand(l_cCmd[0], l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
				// ï¿½Jï¿½nï¿½ï¿½ ON
				if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ï¿½ï¿½ï¿½s */}
				CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
				g_CC_Axis_prc[axis] = 1;
				g_CC_Axis_ORG_Req[axis] = TRUE;
				return MS_NO_ERROR;
			} else {
				g_CC_Axis_ORG_Req[axis] = FALSE;
			}
		}
		// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½(ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A)
		g_CC_Axis_mode[axis] = 1;
//		g_CC_Axis_HP_Req[axis] = TRUE;	// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½CHP ï¿½Ú“ï¿½ï¿½vï¿½ï¿½
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

		// ï¿½Qï¿½Cï¿½ï¿½ï¿½ï¿½ß‚ï¿½
		CC_ChangeGain(axis, 0);
		// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
		sprintf(l_cMode, "P%1u00=1", axis + 1);
		if ((l_result = CC_WriteCommand(l_cMode, l_Recvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}
		// ï¿½Jï¿½nï¿½ï¿½ ON
		if (CC_DioOutBit(l_RunBit, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;	/* ï¿½ï¿½ï¿½s */}
		CC_MotionTimeout(0, axis);	/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g	*/
		g_CC_Axis_prc[axis] = 1;
	}

	return MS_NO_ERROR;
}

// Theta ï¿½pï¿½Ì�ï¿½ï¿½ï¿½
void CC_ThetaAdjust(long *position, long pluslimit, long minuslimit)
{
	long	l_Position = *position;

	// 360 ï¿½ï¿½ï¿½È‰ï¿½ï¿½É•ÏŠï¿½
	l_Position = l_Position % 1000000;
	// ï¿½|ï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½bï¿½g
	if (l_Position < minuslimit) {
		l_Position = minuslimit + 4;
	}
	// ï¿½{ï¿½ï¿½ï¿½ï¿½ï¿½~ï¿½bï¿½g
	if (pluslimit < l_Position) {
		l_Position = pluslimit - 4;
	}

	*position = l_Position;
}

// Z ï¿½ï¿½ï¿½ï¿½ï¿½xï¿½ÏŠï¿½
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

// Y ï¿½ï¿½ï¿½ÅˆÚ“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ X ï¿½Ì‰ï¿½ï¿½ï¿½ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½gï¿½ï¿½
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
		// ï¿½Ú“ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ’ï¿½lï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ X ï¿½ï¿½ï¿½Ì‰ï¿½ï¿½ï¿½ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½gï¿½ï¿½
		*AccelTimeT = ServoParam[CC_AXIS_X].ABS_AccelTimeT;
		*AccelTimeS = ServoParam[CC_AXIS_X].ABS_AccelTimeS;
	}
}

// ï¿½Qï¿½Cï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½ ALL	(ï¿½ï¿½ï¿½İ–ï¿½ï¿½gï¿½p)
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
// ï¿½Qï¿½Cï¿½ï¿½ï¿½ØŠï¿½ï¿½ï¿½ X/Y/Z/T ï¿½ï¿½
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
	if (CC_DioOutBit(l_iGainBit, l_ucGainSel) != 0) {CC_DioError(); return;/* ï¿½ï¿½ï¿½s */}
}

// ï¿½Å‘å‘¬ï¿½xï¿½ï¿½ï¿½ï¿½
long CC_ConvSpeed_XYT(short Axis, long org_speed)
{
#if 0	// FP ï¿½Vï¿½ï¿½ï¿½[ï¿½Yï¿½p ----------------------------------------------
	long	l_org_speed;
	long	l_Speed;

	l_org_speed = (org_speed < 0)? -org_speed : org_speed;
	if (l_org_speed < 1) {l_org_speed = 1;}
	if (g_CC_MaxSpeed[Axis] < l_org_speed) {l_org_speed = g_CC_MaxSpeed[Axis];}

	l_Speed = (ServoParam[Axis].ABS_Speed * l_org_speed) / g_CC_MaxSpeed[Axis];
#else	// Nano ï¿½Vï¿½ï¿½ï¿½[ï¿½Yï¿½p --------------------------------------------
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

// Sodick ï¿½ï¿½ï¿½Ö•ÏŠï¿½
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
// Sodick ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÏŠï¿½
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

// JOYSTICK ï¿½ï¿½ï¿½[ï¿½hï¿½mï¿½F
BOOL CC_IsJoyMode(void)
{
	if (0 != (g_CC_InPort & (0x00000001 << CC_INBIT_JOY_STATUS))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// JOYSTICK ï¿½ï¿½ï¿½[ï¿½hï¿½İ’ï¿½
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
		// ï¿½ï¿½ï¿½ï¿½
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED1, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED2, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED3, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_MODE, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		break;
	case 1:
		// ï¿½İ’ï¿½
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED1, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED2, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED3, 0) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		switch (iSpeedSel) {
		case 1:
			if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED1, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
			break;
		case 2:
			if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED2, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
			break;
		case 3:
			if (CC_DioOutBit(CC_OUTBIT_JOY_SPEED3, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
			break;
		default:
			return MS_PARAMETER_ERROR;
			break;
		}
		if (CC_DioOutBit(CC_OUTBIT_JOY_MODE, 1) != 0) {CC_DioError(); return MS_NO_HARDWARE;/* ï¿½ï¿½ï¿½s */}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}

	return MS_NO_ERROR;
}

// ï¿½Å�ï¿½ï¿½ï¿½ï¿½xï¿½ï¿½ï¿½æ“¾
//		ï¿½ï¿½ï¿½ï¿½ï¿½lï¿½Å‚Í‚È‚ï¿½ï¿½ï¿½ï¿½Û‚É�wï¿½ß‚ï¿½ï¿½ï¿½Å�ï¿½ï¿½ï¿½ï¿½xï¿½Ì‚ï¿½ï¿½Æ‚Å‚ï¿½ï¿½D
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
		// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Ì‘ï¿½ï¿½xï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		if (g_CC_MaxSpeed[Axis] < l_lInitMaxSpeed) {
			// ini ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ì‘ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			l_lRetSpeed= g_CC_MaxSpeed[Axis];
		} else {
			// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Ì‘ï¿½ï¿½xï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			l_lRetSpeed= l_lInitMaxSpeed;
		}
	} else {
		// ServoParam.dat ï¿½Ì‘ï¿½ï¿½xï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		if (g_CC_MaxSpeed[Axis] < ServoParam[Axis].ABS_Speed) {
			// ini ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ì‘ï¿½ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			l_lRetSpeed= g_CC_MaxSpeed[Axis];
		} else {
			// ServoParam.dat ï¿½Ì‘ï¿½ï¿½xï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			l_lRetSpeed= ServoParam[Axis].ABS_Speed;
		}
	}

	return l_lRetSpeed;
}

// ï¿½ï¿½ï¿½Ì‰Ò“ï¿½ï¿½Íˆï¿½(ï¿½ï¿½ï¿½ï¿½)ï¿½ï¿½ï¿½æ“¾
//		ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Ìƒ\ï¿½tï¿½gï¿½ï¿½ï¿½~ï¿½bï¿½gï¿½ï¿½ï¿½ï¿½vï¿½Zï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
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

// ï¿½ï¿½ï¿½Ì“dï¿½qï¿½Mï¿½Aï¿½ï¿½ï¿½æ“¾
//		ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
int CC_GetElectronicGear(short Axis)
{
	WORD l_wElectronicGearA;	// ï¿½ï¿½ï¿½q
	WORD l_wElectronicGearB;	// ï¿½ï¿½ï¿½ï¿½

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

// ï¿½ï¿½ï¿½Ìƒ\ï¿½tï¿½gï¿½ï¿½ï¿½~ï¿½bï¿½gï¿½{ï¿½ï¿½ï¿½ï¿½ï¿½æ“¾
//		ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½Ìƒ\ï¿½tï¿½gï¿½ï¿½ï¿½~ï¿½bï¿½gï¿½ï¿½Ç�oï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D
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

// ï¿½Wï¿½ï¿½ï¿½Cï¿½Xï¿½eï¿½Bï¿½bï¿½Nï¿½Ì‘ï¿½ï¿½xï¿½ï¿½İ’è‚µï¿½Ü‚ï¿½ï¿½D
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
	// ï¿½ï¿½ï¿½xï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ�Å‘å‘¬ï¿½xï¿½ï¿½ï¿½ï¿½
	long	l_Velocity;
	l_Velocity = (velocity < 0)? -velocity : velocity;
	l_Velocity = CC_ConvSpeed_XYT(axis, l_Velocity);
	// ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½M(ï¿½ï¿½ï¿½xï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
	/*		X ï¿½ï¿½	1ï¿½FP582ï¿½C2ï¿½FP583ï¿½C3ï¿½FP584
			Y ï¿½ï¿½	1ï¿½FP587ï¿½C2ï¿½FP588ï¿½C3ï¿½FP589*/
	char l_szCmd[32];
	int l_iAxisBase = (CC_AXIS_X == axis)? 2 : 7;
	sprintf(l_szCmd, "P58%1u=%d", l_iAxisBase + (iSpeedSel - 1), l_Velocity);
	char l_szRecvdata[256];
	int l_result;
	if ((l_result = CC_WriteCommand(l_szCmd, l_szRecvdata)) < 0) {CC_MsgReboot(); return MS_NO_HARDWARE;}

	return l_Rc;
}

/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- { ---------- */
/* ï¿½ï¿½ï¿½Ú“ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½Nï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
#define	CC_MOTION_TIMEOUT	(10 * 60 * 1000)	/* ï¿½ï¿½ï¿½Ú“ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g[ms]	*/
int CC_MotionTimeout(
		int iMode,		/* ï¿½ï¿½ï¿½[ï¿½h
							0ï¿½Fï¿½Jï¿½n
							1ï¿½Fï¿½Iï¿½ï¿½
							2ï¿½Fï¿½`ï¿½Fï¿½bï¿½N	*/
		short wAxis		/*	ï¿½ï¿½	*/
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
		l_iRet = -1;		/* ï¿½wï¿½è‚³ï¿½ê‚½ï¿½ï¿½ï¿½Ô�ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Å‚ï¿½ï¿½D	*/
	} else {
		if (0 == iMode) {
			/* ï¿½Jï¿½n -----------------------------------*/
			l_dwCounter[wAxis] = ::GetTickCount();
			l_bCountFlag[wAxis] = TRUE;
			l_iRet = 0;
		} else if (1 == iMode) {
			/* ï¿½Iï¿½ï¿½ -----------------------------------*/
			l_bCountFlag[wAxis] = FALSE;
			l_iRet = 0;
		} else {
			/* ï¿½`ï¿½Fï¿½bï¿½N -------------------------------*/
			if (TRUE != l_bCountFlag[wAxis]) {
				l_iRet = -3;		/* ï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D	*/
			} else {
				DWORD l_dwCheck =  ::GetTickCount();
				DWORD l_dwElaps;
				if (l_dwCheck < l_dwCounter[wAxis]) {
					l_dwElaps = l_dwCheck + (ULONG_MAX - l_dwCounter[wAxis]) + 1;
				} else {
					l_dwElaps = l_dwCheck - l_dwCounter[wAxis];
				}
				if (CC_MOTION_TIMEOUT < l_dwElaps) {
					/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
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
/* ï¿½ï¿½ï¿½Ú“ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ì�ê�‡ï¿½Í‚ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½~ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
void CC_MotTimeoutCheck(short wAxis, short *wPrc)
{
	static short ls_wPrcPre[CC_AXIS_NUM] = {0, 0, 0, 0};
	short l_wPrcPre[CC_AXIS_NUM];

	/* pop --------------------------------------------*/
	memcpy(l_wPrcPre, ls_wPrcPre, sizeof(l_wPrcPre));

	if ((0 != l_wPrcPre[wAxis]) && (0 == *wPrc)) {
		/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚ï¿½ï¿½ï¿½~ï¿½ï¿½ï¿½É‚È‚ï¿½ï¿½ï¿½	*/
		CC_MotionTimeout(1, wAxis);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½Iï¿½ï¿½	*/
	} else if (0 != *wPrc) {
		/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô‚ï¿½ï¿½ï¿½ï¿½ì’†	*/
		int l_iRet = CC_MotionTimeout(2, wAxis);	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½Ä�ï¿½ ï¿½`ï¿½Fï¿½bï¿½N	*/
		if (1 == l_iRet) {
			/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
			*wPrc = 0;
			CC_local_emergency_stop(wAxis);
			send_motion_message(CC_CnvAxisFromSPT(wAxis), MM_TIMEOUT);
		}
	}
	l_wPrcPre[wAxis] = *wPrc;

	/* push -------------------------------------------*/
	memcpy(ls_wPrcPre, l_wPrcPre, sizeof(ls_wPrcPre));
}
/* added 2009.11.11 hmenjo MotSys ï¿½ï¿½ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½g ---------- } ---------- */

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

	// DI ï¿½ï¿½\ï¿½ï¿½
	TCHAR l_tszTempL[32], l_tszTempH[32];
	g_pCMotSysDlg->CnvWordToBin((WORD) (g_CC_InPort & 0x0000ffff), l_tszTempL, 2);
	g_pCMotSysDlg->CnvWordToBin((WORD) (g_CC_InPort >> 16), 	   l_tszTempH, 2);
	_stprintf(l_tszText, _T("%s  %s"), l_tszTempH, l_tszTempL);
	g_pCMotSysDlg->SetDlgItemText(IDC_STATIC_DI, l_tszText);
	// DO ï¿½ï¿½\ï¿½ï¿½
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




// ï¿½È‰ï¿½ï¿½ÌƒRï¿½[ï¿½hï¿½Íƒfï¿½oï¿½bï¿½Oï¿½p
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
	// ï¿½Tï¿½[ï¿½{ OFF ï¿½vï¿½ï¿½
	g_CC_ServoOFFReq = TRUE;
#endif
}

void mmtest2(void)
{
	// ï¿½Tï¿½[ï¿½{ ON ï¿½vï¿½ï¿½
	g_CC_ServoONReq = TRUE;
}

// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A ï¿½Nï¿½ï¿½(ï¿½eï¿½Xï¿½gï¿½ï¿½)
static short	lg_Xorg = 0;
void mmtestORGX(int mode)
{
	char	l_Recvdata[256];
	int		l_result;
	char	l_cCmd[32];

	// ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// HP ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ABS ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// INC ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
		if ((l_result = CC_WriteCommand("P100=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_X, 1) != 0) {
		CC_DioError();	return;	// ï¿½ï¿½ï¿½s
	}
	lg_Xorg = 1;
}
// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A ï¿½ï¿½ï¿½ï¿½(ï¿½eï¿½Xï¿½gï¿½ï¿½)
void mmtestORGXdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Xorg) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Ò‚ï¿½
		lg_Xorg = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_X, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Xorg = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		if ((l_result = CC_ReadCommand("P150", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_X, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
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

	// ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// HP ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ABS ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// INC ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
		if ((l_result = CC_WriteCommand("P200=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_Y, 1) != 0) {
		CC_DioError();	return;	// ï¿½ï¿½ï¿½s
	}
	lg_Yorg = 1;
}
// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A ï¿½ï¿½ï¿½ï¿½(ï¿½eï¿½Xï¿½gï¿½ï¿½)
void mmtestORGYdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Yorg) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Ò‚ï¿½
		lg_Yorg = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_Y, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Yorg = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		if ((l_result = CC_ReadCommand("P250", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Y, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
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

	// ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// HP ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ABS ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// INC ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
		if ((l_result = CC_WriteCommand("P300=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_Z, 1) != 0) {
		CC_DioError();	return;	// ï¿½ï¿½ï¿½s
	}
	lg_Zorg = 1;
}
// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A ï¿½ï¿½ï¿½ï¿½(ï¿½eï¿½Xï¿½gï¿½ï¿½)
void mmtestORGZdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Zorg) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Ò‚ï¿½
		lg_Zorg = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_Z, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Zorg = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		if ((l_result = CC_ReadCommand("P350", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_Z, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
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

	// ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^ï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½Aï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// HP ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// ABS ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
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
		// INC ï¿½Ú“ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½ï¿½ï¿½Zï¿½bï¿½g
		if ((l_result = CC_WriteCommand("P400=4", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		break;
	default:
		return; break;
	}
	// ï¿½Jï¿½nï¿½ï¿½ ON
	if (CC_DioOutBit(CC_OUTBIT_RUN_T, 1) != 0) {
		CC_DioError();	return;	// ï¿½ï¿½ï¿½s
	}
	lg_Torg = 1;
}
// ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½A ï¿½ï¿½ï¿½ï¿½(ï¿½eï¿½Xï¿½gï¿½ï¿½)
void mmtestORGTdone(void)
{
	char	l_Recvdata[256];
	int		l_result;
	BYTE	l_ucBitData;

	switch (lg_Torg) {
	case 0:		// ï¿½Nï¿½ï¿½ï¿½Ò‚ï¿½
		break;
	case 1:		// INP ï¿½Ä�ï¿½ï¿½Ò‚ï¿½
		lg_Torg = 2;
		break;
	case 2:		// INP ï¿½Ò‚ï¿½
		if (CC_DioInpBit(CC_INBIT_INP_T, &l_ucBitData) != 0) {CC_DioError(); l_ucBitData = 0;}
		if (l_ucBitData != 0) {
			lg_Torg = 3;
		}
		break;
	case 3:		// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½Ì’Ê�ï¿½Ö‚Ì•Ï‰ï¿½ï¿½Ò‚ï¿½
		if ((l_result = CC_ReadCommand("P450", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
		if (strcmp(l_Recvdata, "0") == 0) {
			// ï¿½Jï¿½nï¿½ï¿½ OFF
			if (CC_DioOutBit(CC_OUTBIT_RUN_T, 0) != 0) {
				CC_DioError();	return;	// ï¿½ï¿½ï¿½s
			}
			// ï¿½ï¿½ï¿½ìƒ‚ï¿½[ï¿½hï¿½ï¿½Ê�ï¿½ÖƒZï¿½bï¿½g
			if ((l_result = CC_WriteCommand("P400=0", l_Recvdata)) < 0) {CC_MsgReboot(); return;}
			lg_Torg = 0;
		}
		break;
	}
}
