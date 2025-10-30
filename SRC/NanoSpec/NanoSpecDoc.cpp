// NanoSpecDoc.cpp : CNanoSpecDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "Alarm.h"
#include "AlarmHistoryListView.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "MessageDlg.h"
/* added 2009.09.08 hmenjo AF ���������Y�֑ؑΉ� ---------- { ---------- */
#include "NSStage.hxx"
#include "NanoSpec.h"
/* added 2009.09.08 hmenjo AF ���������Y�֑ؑΉ� ---------- } ---------- */
/* added 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
#include	<math.h>
/* added 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.10.09 bagus StagePGM ���ʉ� --{--
static struct {
	int	nHead;
	int	nScan;
	int	nStagePGM;
} s_StagePGMTable[] = {
	// HeadType,		ScanType,											StagePGM
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_THICKNESS,						STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_REFLECTANCE,						STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_TRANSMITTANCE,					STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_TRANSMITTANCE_G,					STAGE_PGM_TYPE_SR_GANTRY	},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_REFLECTANCE_CIE,					STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE,				STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_DISTANCE,							STAGE_PGM_TYPE_SR_DISTANCE	},
	{ HEAD_TYPE_SR,		MEAS_PROG_TYPE_SR_OPTICAL_DENSITY,					STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_SE,		MEAS_PROG_TYPE_SE_THICKNESS,						STAGE_PGM_TYPE_STANDARD		},
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	{ HEAD_TYPE_COMPEASE,MEAS_PROG_TYPE_COMPEASE_THICKNESS,					STAGE_PGM_TYPE_STANDARD		},
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
#if 0
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_THICKNESS,						STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_OPTICAL_CONSTANT,				STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_CALC_REFLECTANCE_TRANSMITTANCE,	STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_OPTICAL_COARSENESS,				STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_MIXTURE_MATERIAL,				STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_OPTICAL_BANDGAP,				STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_IRSE,	MEAS_PROG_TYPE_IRSE_LTPS_PARAM,						STAGE_PGM_TYPE_STANDARD		},
#else
	{ HEAD_TYPE_MS,		MEAS_PROG_TYPE_MS_MEAS,								STAGE_PGM_TYPE_STANDARD		},
#endif
// 2009.10.19 bagus MS �ǉ� --}--
	{ HEAD_TYPE_4PP,	MEAS_PROG_TYPE_4PP_MEAS,							STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_CTA,	MEAS_PROG_TYPE_CTA_MEAS,							STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_STRESS,	MEAS_PROG_TYPE_STRESS_MEAS,							STAGE_PGM_TYPE_STRESS		},
};
// 2009.10.09 bagus StagePGM ���ʉ� --}--


/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc 														   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CNanoSpecDoc, CDocument)

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CNanoSpecDoc, CDocument)
	//{{AFX_MSG_MAP(CNanoSpecDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc �N���X�̍\�z/����
CNanoSpecDoc::CNanoSpecDoc()
{
	//Kojika 20090525 Add
	CString strBuffer;
	//Kojika 20090525 Add End
	char szBuff[100];

	///// �f�B���N�g�����쐬 /////
	MakeNanospecDir();

	///// OperationLogFile /////
	m_OperationLogFile.Create(g_szLog_Dir,								// LOG_DIR,
								OPERATION_LOG_NAME,
								OPERATION_LOG_SIZE,
								OPERATION_LOG_MAX);

/* modified 2016.05.12 hmenjo 6500/TS3100 �� version ---------- { ---------- */
//	sprintf(szBuff, "START Nanospec %s (%s %s)", SOFT_VERSION, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 �� version ----------              */
	char l_szVersion[128];
	switch (g_lModelType) {
	case MODEL_T3100:	strcpy(l_szVersion, SOFT_VERSION_TS3100);	break;
	default:			strcpy(l_szVersion, SOFT_VERSION);			break;
	}
	sprintf(szBuff, "START Nanospec %s (%s %s)", l_szVersion, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 �� version ---------- } ---------- */
// 2013.11.15 Bagus Add (TohoSpec�Ή�) -->
	if(g_lAppNameType != APP_NAME_NANO){
		CString strMsg;
		strMsg = szBuff;
		strMsg.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		strcpy(szBuff, strMsg);
	}
// 2013.11.15 Bagus Add (TohoSpec�Ή�) <--
	OperationLogging(szBuff);
// hmenjo ChiefLog -->
	// Chief ���O�t�@�C���̏�����
	m_ChiefLogFile.Create(	g_szLog_Dir,								// LOG_DIR,
						CHIEF_LOG_NAME,
						CHIEF_LOG_SIZE,
						CHIEF_LOG_MAX);
// hmenjo ChiefLog <--

// 2013.01.10 bagus stage driver alarm io -->
	// StageError ���O�t�@�C���̏�����
	m_StageErrorLogFile.Create(	g_szLog_Dir,								// LOG_DIR,
						STAGE_ERROR_LOG_NAME,
						STAGE_ERROR_LOG_SIZE,
						STAGE_ERROR_LOG_MAX);
// 2013.01.10 bagus stage driver alarm io <--

	///// NanoSpec.ini�̃O���[�o���ϐ��̏����� /////
	ConfigFile_LoadAllNanoSpecIni();
	//Saiki 20090723 Delete ----->
	//ConfigFile_SaveAllNanoSpecIni();
	//Saiki 20090723 Delete <-----

	///// Alarm /////
	//memset(m_AlarmHis, 0, sizeof(m_AlarmHis));
	//m_AlarmHis_Count = 0;
	//AlarmHis_Restore(g_szLog_Dir, ALARM_LOG_NAME, ALARM_LOG_MAX, ALARM_HIS_MAX);

	///// Mode /////
	m_iHostMode					= HOST_LOCAL;
	m_iPifCommConnectStatus		= 0;

	///// Status /////
	m_iVacuumStatus				= VACUUM_EMPTY;
	m_iProcessStatus			= PROCESS_INIT;
	m_iAlarmStatus				= ALARM_NON;
	m_iWarningStatus			= WARNING_NON;
	m_iDispStatus				= MAIN_MENU_MEASUREMENT;
	m_iInitFinished				= FALSE;
	m_bJoyStickSelectEnabled	= TRUE;							// Default�͐ؑ֋���
	memset(m_bDoPointMeasFlag, 0, sizeof(m_bDoPointMeasFlag));	// �|�C���g���̑��肷��/���Ȃ��t���O

	m_bPatRecFlag = FALSE;
	memset(&m_ActuateFlags, 0, sizeof(ACTUATE_FLAGS));

	///// User /////
	memset(&m_User, 0, sizeof(m_User));

	m_pMeasureddata = (MEASURED_DATA*)VirtualAlloc(NULL, sizeof(MEASURED_DATA), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if ( !m_pMeasureddata ) {
//		AfxMessageBox("MEASURED_DATA �������m�ێ��s");
		//AfxMessageBox("����f�[�^ �������m�ێ��s");
		//Kojika 20090525 Change
		LoadStringML(IDS_MEASURED_DATA_ALLOC_FAILED , strBuffer, "MEASURED_DATA Memory Alloc failed");
		AfxMessageBox(strBuffer);
		//Kojika 20090525 Change End
		return;
	}

	m_iSeqMeasMode = SEQ_NOT_MEASURE;	//���蒆�łȂ�(���茋�ʉ�ʂ�WAIT���܂�)
	m_iMeasMode = MEASMODE_NOT_MEASURE; //���蒆�łȂ�(���茋�ʉ�ʂ�WAIT���͊܂܂Ȃ�)
	m_iManuMeasMode = MANUMODE_NOT; 	//�}�j���A�����胂�[�h�łȂ�

	m_bPauseFlg = FALSE;   //�|�[�Y����t���O(TRUE:�|�[�Y FALSE:����ȊO)

	InitMeasData();

	m_ActualJoyStickFlag = FALSE;
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
	m_lZ_AF = 0x80000000;
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */

// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	m_bSetSampleIdFromUI = FALSE;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
}

/////////////////////////////////////////////////////////////////////////////
//
CNanoSpecDoc::~CNanoSpecDoc()
{
	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	if ( m_pMeasureddata ) {
		VirtualFree(m_pMeasureddata, 0, MEM_DECOMMIT | MEM_RELEASE);
		m_pMeasureddata = NULL;
	}

	char szBuff[100];
/* modified 2016.05.12 hmenjo 6500/TS3100 �� version ---------- { ---------- */
//	sprintf(szBuff, "END    Nanospec %s (%s %s)", SOFT_VERSION, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 �� version ----------              */
	char l_szVersion[128];
	switch (g_lModelType) {
	case MODEL_T3100:	strcpy(l_szVersion, SOFT_VERSION_TS3100);	break;
	default:			strcpy(l_szVersion, SOFT_VERSION);			break;
	}
	sprintf(szBuff, "END    Nanospec %s (%s %s)", l_szVersion, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 �� version ---------- } ---------- */
// 2013.11.15 Bagus Add (TohoSpec�Ή�) -->
	if(g_lAppNameType != APP_NAME_NANO){
		CString strMsg;
		strMsg = szBuff;
		strMsg.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		strcpy(szBuff, strMsg);
	}
// 2013.11.15 Bagus Add (TohoSpec�Ή�) <--

	OperationLogging(szBuff);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc �V���A���C�[�[�V����
void CNanoSpecDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc �N���X�̐f�f

#ifdef _DEBUG
/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::AssertValid() const
{
	CDocument::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc �R�}���h

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::MakeNanospecDir()
{
	MakeDirectory(g_szBin_Dir);
	MakeDirectory(g_szLog_Dir);
	MakeDirectory(g_szCfg_Dir);
	MakeDirectory(g_szDb_Dir);
	MakeDirectory(g_szCfg_System_Dir);
	MakeDirectory(g_szCfg_System_Sample_Dir);
	MakeDirectory(g_szCfg_User_Dir);
	MakeDirectory(g_szCfg_User_User_Account_Dir);
	MakeDirectory(g_szDb_Main_Recipe_Dir);
	MakeDirectory(g_szDb_Multi_Recipe_Dir);
	MakeDirectory(g_szDb_Measurement_Program_Dir);
	MakeDirectory(g_szDb_Stage_Program_Dir);
	MakeDirectory(g_szDb_Deskew_Img_Dir);
	MakeDirectory(g_szDb_Pattern_Dir);
	MakeDirectory(g_szDb_Pattern_Img_Dir);
	MakeDirectory(g_szDb_Recalibration_Program_Dir);
	MakeDirectory(g_szDb_Point_Deskew_Program_Dir);
	MakeDirectory(g_szResult_Dir);
	MakeDirectory(g_szLog_Pr_Dir);
}

/////////////////////////////////////////////////////////////////////////////
//
int CNanoSpecDoc::MakeDirectory(char* lpszDir)
{
	char szBuff[256];
	int i;
	int iRet;
	SECURITY_ATTRIBUTES sa;

	sa.lpSecurityDescriptor = NULL;

	if((iRet = CreateDirectory(lpszDir, &sa)) == TRUE)
		return 1;

	for(i = strlen(lpszDir); i > 0; i--){
		if (lpszDir[i] == '\\' || lpszDir[i] == '/')
			break;
	}

	if(i <= 0)
		return 0;

	strcpy(szBuff, lpszDir);
	szBuff[i] = NULL;
	if(iRet = MakeDirectory(szBuff))
		return (iRet = CreateDirectory(lpszDir, &sa)) == TRUE ? 1 : 0;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::AddTime(char* lpszBuff)
{
	SYSTEMTIME csTime;

	::GetLocalTime(&csTime);
	sprintf(lpszBuff, "%04d.%02d.%02d %02d:%02d:%02d.%03d",
		csTime.wYear, csTime.wMonth, csTime.wDay,
		csTime.wHour, csTime.wMinute, csTime.wSecond,
		csTime.wMilliseconds);
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::OperationLogging(char* lpszLogText)
{
	char szBuff[1024];

	AddTime(szBuff);
	_tcscat(szBuff, " ");
	_tcscat(szBuff, lpszLogText);

	m_OperationLogFile.Logging(szBuff);
}

// hmenjo ChiefLog -->
/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::ChiefLogging(TCHAR* pszLogText)
{
	char l_szBuff[1024];

	AddTime(l_szBuff);
	_tcscat(l_szBuff, " ");
	_tcscat(l_szBuff, pszLogText);

	m_ChiefLogFile.Logging(l_szBuff);
}
// hmenjo ChiefLog <--

// 2013.01.10 bagus stage driver alarm io -->
void CNanoSpecDoc::StageErrorLogging(TCHAR* pszLogText)
{
	char l_szBuff[1024];

	AddTime(l_szBuff);
	_tcscat(l_szBuff, " ");
	_tcscat(l_szBuff, pszLogText);

	m_StageErrorLogFile.Logging(l_szBuff);
}
// 2013.01.10 bagus stage driver alarm io <--

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::AlarmHis_Restore(char* Path, char* filename,int LogMax,int HisMax)
{
	CFile file;
	char FileName[_MAX_PATH];
	DWORD FilePointer;
	char Character;
	char szBuff[1024];
	int Count;
	int ReadNum;
	int TerminatorNum;
	int i;
	int ListCount;
	int FileNo;
	ALARM_HIS AlarmHis;

	ListCount = 0;
	for(FileNo = 1 ; FileNo <= LogMax ; FileNo++){
		///// Log File Saving /////
		sprintf(FileName, "%s\\%s%03d" LOG_FILE_EXT, Path, filename, FileNo);
		if(!file.Open(FileName, CFile::modeRead | CFile::typeBinary)){
			continue;
		}

		file.SeekToEnd();
		FilePointer = static_cast<long>(file.GetPosition());

		///// check the number of terminator /////
		TerminatorNum = 0;
		FilePointer = static_cast<long>(file.Seek(-2 ,CFile::current));
		for(i = 0 ; i < 2 ; i++){
			file.Read(&Character, 1);
			if(Character == 0x0d || Character == 0x0a){
				TerminatorNum++;
			}
		}

		///// read data /////
		Count = 0;
		try{
			while(FilePointer != 0){
				FilePointer = static_cast<long>(file.Seek(-2 ,CFile::current));
				file.Read(&Character, 1);
				Count++;
				if(FilePointer == 0){
					Count = Count - (TerminatorNum - 1);
					FilePointer = static_cast<long>(file.Seek(-1 ,CFile::current));
					ReadNum = file.Read(szBuff, Count);
					FilePointer = static_cast<long>(file.Seek(-ReadNum ,CFile::current));
					szBuff[Count] = '\0';

					AlarmHis_StrToHis(szBuff, &AlarmHis);
					AlarmHis_Add(0, &AlarmHis);

					Count = 0;
					if(++ListCount >= HisMax){
						break;
					}
					continue;
				}
				if(Character == 0x0a){
					Count = Count - TerminatorNum;
					ReadNum = file.Read(szBuff,Count);
					FilePointer = static_cast<long>(file.Seek(-ReadNum ,CFile::current));
					szBuff[Count] = '\0';

					AlarmHis_StrToHis(szBuff, &AlarmHis);
					AlarmHis_Add(0, &AlarmHis);

					Count = 0;
					if(++ListCount >= HisMax){
						break;
					}
					continue;
				}
			}
		}
		catch(CFileException* fe){
			long ErrorCode = fe->m_lOsError;
		}

		file.Close();
	}

}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::AlarmHis_StrToHis(char* pszStr, ALARM_HIS* pAlarmHis)
{
	char szTemp[MAX_PATH];
	CString sBuff;

	memset(pAlarmHis, 0, sizeof(ALARM_HIS));

	///// Date /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[0], ALARM_LOG_DATE_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Date, sBuff.GetBuffer(0));

	///// Time /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[11], ALARM_LOG_TIME_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Time, sBuff.GetBuffer(0));

	///// Level /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[21], ALARM_LOG_LEVEN_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Level, sBuff.GetBuffer(0));

	///// Set /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[29], ALARM_LOG_SET_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Set, sBuff.GetBuffer(0));

	///// ID /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[34], ALARM_LOG_ID_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Id, sBuff.GetBuffer(0));

	///// Text /////
	memset(szTemp, 0, sizeof(szTemp));
	memcpy(szTemp, &pszStr[39], ALARM_LOG_TEXT_LEN);
	sBuff = szTemp;
	sBuff.TrimLeft();
	sBuff.TrimRight();
	strcpy(pAlarmHis->Text, sBuff.GetBuffer(0));
}

/////////////////////////////////////////////////////////////////////////////
//
int CNanoSpecDoc::AlarmHis_Add(int Index, ALARM_HIS* pAlarmHis)
{
	if(Index >= ALARM_HIS_MAX) return -1;

	if(m_AlarmHis_Count >= ALARM_HIS_MAX){
		memmove(&m_AlarmHis[0], &m_AlarmHis[1], sizeof(ALARM_HIS) * (ALARM_HIS_MAX - 1));
		if(--m_AlarmHis_Count < 0) m_AlarmHis_Count = 0;
	}

	if(Index == -1){
		Index = m_AlarmHis_Count;
	}

	memmove(&m_AlarmHis[Index + 1], &m_AlarmHis[Index], sizeof(ALARM_HIS) * ((ALARM_HIS_MAX - Index) - 1));

	memset(&m_AlarmHis[Index], 0, sizeof(ALARM_HIS));

	memcpy(&m_AlarmHis[Index], pAlarmHis, sizeof(ALARM_HIS));

	m_AlarmHis_Count++;

	return Index;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecDoc::AlarmHis_Get(int Index, ALARM_HIS* pAlarmHis)
{
	if(m_AlarmHis_Count == 0) return FALSE;

	if(Index >= ALARM_HIS_MAX) return FALSE;

	if(Index == -1){
		Index = m_AlarmHis_Count - 1;
	}

	memcpy(pAlarmHis, &m_AlarmHis[Index], sizeof(ALARM_HIS));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecDoc::User_Is()
{
	if(m_User.szName[0] != '\0') return TRUE;
	else return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::User_Entry(USER_ACCOUNT User)
{
	memcpy(&m_User, &User, sizeof(m_User));
	//2009.12.10 bagus Recipe backup --{--
	//���[�U���ύX���ꂽ�̂Ńo�b�N�A�b�v�̐ݒ���X�V����
	SYSTEM_CONFIG l_SystemConfig;
	USER_DATA	 l_UserData;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	if(l_SystemConfig.bBackup){
		RecipeFile_SetBackupPath(l_UserData.BackupData.BackupPath,l_UserData.BackupData.bAutoBackup);
		PatternFile_SetBackupPath(l_UserData.BackupData.BackupPath,l_UserData.BackupData.bAutoBackup);
	}else{
		//�o�b�N�A�b�v�@�\���L���ɂȂ��Ă��Ȃ�
		RecipeFile_SetBackupPath("",FALSE);
		PatternFile_SetBackupPath("",FALSE);
	}
	//2009.12.10 bagus Recipe backup --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecDoc::User_Remove()
{
	memset(&m_User, 0, sizeof(m_User));
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecDoc::User_Access(int iAccessItem)
{
	ACCESS_LEVEL AccessLevel[ACCOUNT_LEVEL_MAX];
	UINT nAccountLevel;
	BOOL bAccess = FALSE;

	if(User_Is() == TRUE){
		nAccountLevel = m_User.nAccountLevel;
	}
	else{
		nAccountLevel = ACCOUNT_LEVEL_LOGOFF;
	}

	if(nAccountLevel == ACCOUNT_LEVEL_LOGOFF)
	{
		switch(iAccessItem)
		{
			case ACCESS_MEASUREMENT:
				bAccess = TRUE;
				break;
			case ACCESS_RECIPE:
			case ACCESS_PROGRAM:
			case ACCESS_DATA:
			case ACCESS_USERSETTING:
			case ACCESS_MAINTENANCE:
			case ACCESS_SYSTEMCONFIG:
				bAccess = FALSE;
				break;
		}
	}
	else
	{
		ConfigFile_GetNanoSpecIni(AccessLevel, CONFIG_FILE_ACCESS_LEVEL);

		switch(iAccessItem){
		case ACCESS_MEASUREMENT:
			bAccess = AccessLevel[nAccountLevel].bMeasurement;
			break;
		case ACCESS_RECIPE:
			bAccess = AccessLevel[nAccountLevel].bRecipe;
			break;
		case ACCESS_PROGRAM:
			bAccess = AccessLevel[nAccountLevel].bProgram;
			break;
		case ACCESS_DATA:
			bAccess = AccessLevel[nAccountLevel].bData;
			break;
		case ACCESS_USERSETTING:
			bAccess = AccessLevel[nAccountLevel].bUserSetting;
			break;
		case ACCESS_MAINTENANCE:
			bAccess = AccessLevel[nAccountLevel].bMaintenance;
			break;
		case ACCESS_SYSTEMCONFIG:
			bAccess = ( strcmp(m_User.szName, USER_ACCOUNT_TOHO_ENGINEER) == 0 );
			break;
		}
	}

	return bAccess;
}




/////////////////////////////////////////////////////////////////////////////
//	m_measureddata�̏�����
void CNanoSpecDoc::InitMeasData(void)
{

	memset(m_pMeasureddata, 0, sizeof(MEASURED_DATA));
	m_iLastMeasPoint = 0;
	m_iCurrentSetPoint = 0;
	m_iReMeasNumScans = 0;
	m_iReMeasurePointCnt = 0; //hibino �m�F��
	//2009.08.17 bagus stress --{--
	//�X�g���X�֘A�ǉ�������
	m_iLastMeasStressLine = 0;			//�X�g���X����̍ŏI����X�g���X���C��
	m_iCurrentSetStressLine = 0;		//��ԍŌ�Ƀf�[�^���i�[�����X�g���X���C��
	m_iCurrentMeasStressLine = 0;		//���ݑ��蒆�̃X�g���X���C��
	//2009.08.17 bagus stress --}--

}



/////////////////////////////////////////////////////////////////////////////
//	����f�[�^�i�[������̑S�̂��擾 (dat�t�@�C���쐬���̂ݎg�p)
void CNanoSpecDoc::GetMeasDataAll(MEASURED_DATA* meadat)
{
	*meadat = *m_pMeasureddata;
}


/////////////////////////////////////////////////////////////////////////////
//	����f�[�^�i�[������̑S�̂��i�[�@�y�g�p�s�z
void CNanoSpecDoc::SetMeasDataAll(MEASURED_DATA* meadat)
{
	*m_pMeasureddata = *meadat;
}

/////////////////////////////////////////////////////////////////////////////
//	���V�s�f�[�^�擾
void CNanoSpecDoc::GetRcpData(RCP_DATA* rcp_data)
{
	*rcp_data = m_pMeasureddata->rcp_data;
}

/////////////////////////////////////////////////////////////////////////////
//	���V�s�f�[�^�i�[
void CNanoSpecDoc::SetRcpData(RCP_DATA* rcp_data)
{
	m_pMeasureddata->rcp_data = *rcp_data;
}

/////////////////////////////////////////////////////////////////////////////
//	1�|�C���g����f�[�^�E���v�f�[�^�擾 �y�񐄏��z
//����: 1�|�C���g������f�[�^, ���̎��_�܂ł̓��v�f�[�^, ���|�C���g�ڂ̑��蕪���̎w��(1�|�C���g�ځ�1)
BOOL CNanoSpecDoc::GetOnePointAndStatistics(ONE_POINT_DATA *OnePointData, STATISTICS *Statistics, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //�擾���s
	}
	*OnePointData = m_pMeasureddata->OnePointData[iPoint - 1];
	memcpy(&Statistics[0], &m_pMeasureddata->Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //�擾����
}

/////////////////////////////////////////////////////////////////////////////
//	1�|�C���g����f�[�^�E���v�f�[�^�i�[ �y�񐄏��z
//����: 1�|�C���g������f�[�^, ���̎��_�܂ł̓��v�f�[�^, ���|�C���g�ڂ̑��蕪���̎w��(1�|�C���g�ځ�1)
BOOL CNanoSpecDoc::SetOnePointAndStatistics(ONE_POINT_DATA *OnePointData, STATISTICS *Statistics, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //�i�[���s
	}
	m_pMeasureddata->OnePointData[iPoint - 1] = *OnePointData;
	memcpy(&m_pMeasureddata->Statistics[0], &Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //�i�[����
}


/////////////////////////////////////////////////////////////////////////////
//	1�|�C���g����f�[�^�擾 �y�����z
//����: 1�|�C���g������f�[�^, ���|�C���g�ڂ̑��蕪���̎w��(1�|�C���g�ځ�1)
BOOL CNanoSpecDoc::GetOnePointData(ONE_POINT_DATA *OnePointData, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //�擾���s
	}
	*OnePointData = m_pMeasureddata->OnePointData[iPoint - 1];
	return TRUE; //�擾����
}

/////////////////////////////////////////////////////////////////////////////
//	1�|�C���g����f�[�^�i�[ �y�����z
//����: 1�|�C���g������f�[�^, ���|�C���g�ڂ̑��蕪���̎w��(1�|�C���g�ځ�1)
BOOL CNanoSpecDoc::SetOnePointData(ONE_POINT_DATA *OnePointData, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //�i�[���s
	}
	m_pMeasureddata->OnePointData[iPoint - 1] = *OnePointData;
	return TRUE; //�i�[����
}


/////////////////////////////////////////////////////////////////////////////
//	�e�X�g���[�h1�|�C���g����f�[�^�i�[
//����: 1�|�C���g������f�[�^, ���|�C���g�ڂ̑��蕪���̎w��(1�|�C���g�ځ�1)
BOOL CNanoSpecDoc::SetTestModeData(TEST_MODE_DATA *TestModeData, int iPoint)
{
	if((iPoint > TESTDATA_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //�i�[���s
	}
	m_pMeasureddata->TestModeData[iPoint - 1] = *TestModeData;
	return TRUE; //�i�[����
}


/////////////////////////////////////////////////////////////////////////////
//	���v�f�[�^�擾 �y�����z
//����: ���̎��_�܂ł̓��v�f�[�^
BOOL CNanoSpecDoc::GetStatisticsData(STATISTICS *Statistics)
{
	memcpy(&Statistics[0], &m_pMeasureddata->Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //�擾����
}

/////////////////////////////////////////////////////////////////////////////
//	���v�f�[�^�i�[ �y�����z
//����: ���̎��_�܂ł̓��v�f�[�^
BOOL CNanoSpecDoc::SetStatisticsData(STATISTICS *Statistics)
{
	memcpy(&m_pMeasureddata->Statistics[0], &Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //�i�[����
}


/////////////////////////////////////////////////////////////////////////////
//	���v�f�[�^�L��Point��(���v�f�[�^�ɉ�����Point��)�擾
int CNanoSpecDoc::GetStatisticsValidPointCount(void)
{
	return m_pMeasureddata->iStatisticsValidPointCount;
}

/////////////////////////////////////////////////////////////////////////////
//	���v�f�[�^�L��Point��(���v�f�[�^�ɉ�����Point��)�i�[
void CNanoSpecDoc::SetStatisticsValidPointCount(int iStatisticsValidPointCount)
{
	m_pMeasureddata->iStatisticsValidPointCount = iStatisticsValidPointCount;
}


// 2009.09.30 K.Matsuo Delete -->
/////////////////////////////////////////////////////////////////////////////
//	���˗��f�[�^�̎擾�i�񐔁A�g��380�`800�j�@//�i�[��SetOnePointData�ŁB
//double CNanoSpecDoc::NanoSpecDoc_GetAnalysisData(int iCnt, int iWave)
//{
//	return m_pMeasureddata->OnePointData[iCnt - 1].dAnalysisData[iWave - 1];
//}
//
/////////////////////////////////////////////////////////////////////////////
//	Xmp�����GenData�̎擾�i�񐔁A�g��380�`800�j�@//�i�[��SetOnePointData�ŁB
//double CNanoSpecDoc::NanoSpecDoc_GetAnalysisGenData(int iCnt, int iWave)
//{
//	return m_pMeasureddata->OnePointData[iCnt - 1].dAnalysisGenData[iWave - 1];
//}
// 2009.09.30 K.Matsuo Delete <--

// 2014.04.03 bagus wavelength step modified -->
// /////////////////////////////////////////////////////////////////////////////
// //	�T���v���f�[�^�̎擾(�e�X�g���[�h��p)
// //�i�񐔁A�g��380�`800�j�@//�i�[��SetTestModeData�ŁB
// double CNanoSpecDoc::NanoSpecDoc_GetScanSampleData(int iCnt, int iWave)
// {
// 	return m_pMeasureddata->TestModeData[iCnt - 1].dScanSampleData[iWave - 1];
// }
//
//
// /////////////////////////////////////////////////////////////////////////////
// //	���t�@�����X�̎擾(�e�X�g���[�h��p)
// //�i�񐔁A�g��380�`800�j�@//�i�[��SetTestModeData�ŁB
// double CNanoSpecDoc::NanoSpecDoc_GetScan1stReferenceData(int iCnt, int iWave)
// {
// 	return m_pMeasureddata->TestModeData[iCnt - 1].dScan1stReferenceData[iWave - 1];
// }
//
//
// /////////////////////////////////////////////////////////////////////////////
// //	���t�@�����X�̎擾(�e�X�g���[�h��p)
// //�i�񐔁A�g��380�`800�j�@//�i�[��SetTestModeData�ŁB
// double CNanoSpecDoc::NanoSpecDoc_GetScanDark_T1Data(int iCnt, int iWave)
// {
// 	return m_pMeasureddata->TestModeData[iCnt - 1].dScanDark_T1Data[iWave - 1];
// }
/////////////////////////////////////////////////////////////////////////////
//	�T���v���f�[�^�̎擾(�e�X�g���[�h��p)
//�i�񐔁A�g��380�`800�j�@//�i�[��SetTestModeData�ŁB
double CNanoSpecDoc::NanoSpecDoc_GetScanSampleData(int iCnt, double dWave)
{
	return m_pMeasureddata->TestModeData[iCnt - 1].GetScanSampleData(dWave);
}


/////////////////////////////////////////////////////////////////////////////
//	���t�@�����X�̎擾(�e�X�g���[�h��p)
//�i�񐔁A�g��380�`800�j�@//�i�[��SetTestModeData�ŁB
double CNanoSpecDoc::NanoSpecDoc_GetScan1stReferenceData(int iCnt, double dWave)
{
	return m_pMeasureddata->TestModeData[iCnt - 1].GetScan1stReferenceData(dWave);
}


/////////////////////////////////////////////////////////////////////////////
//	���t�@�����X�̎擾(�e�X�g���[�h��p)
//�i�񐔁A�g��380�`800�j�@//�i�[��SetTestModeData�ŁB
double CNanoSpecDoc::NanoSpecDoc_GetScanDark_T1Data(int iCnt, double dWave)
{
	return m_pMeasureddata->TestModeData[iCnt - 1].GetScanDark_T1Data(dWave);
}
// 2014.04.03 bagus wavelength step modified <--


/////////////////////////////////////////////////////////////////////////////
//	�e�X�g���[�h���m�����擾   //�i�[��SetTestModeData�ŁB
void CNanoSpecDoc::GetMeasRcpCon(int iCnt, TEST_MODE_MEASRCP_CONDITION* MeasRcpCon)
{
	*MeasRcpCon = m_pMeasureddata->TestModeData[iCnt - 1].MeasRcpCon;
}


/////////////////////////////////////////////////////////////////////////////
//	�e�X�g���[�h�f�[�^�擾	 //�i�[��SetTestModeData�ŁB
void CNanoSpecDoc::GetTestModeData(TEST_MODE_DATA* TestModeData, int iCnt)
{
	*TestModeData = m_pMeasureddata->TestModeData[iCnt - 1];
}

/////////////////////////////////////////////////////////////////////////////
//	dData(ONE_POINT_DATA��)�̎擾 �@//�i�[��SetOnePointData�ŁB
void CNanoSpecDoc::GetOnepointDdata(int iPoint, double* dData)
{
	memcpy(dData, m_pMeasureddata->OnePointData[iPoint - 1].dData, sizeof(m_pMeasureddata->OnePointData[iPoint - 1].dData) );

}


/////////////////////////////////////////////////////////////////////////////
//	���x�����Ȃǎ擾(�wX,Y,Z�x,�wAF�x,�wThick�`�x,�wThick�`�̌��x)
void CNanoSpecDoc::GetLabel(char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1],  //���x��(X,Y,Z)
							char szTitleAf[AF_HEADER_LEN + 1],		 //���x��(AF)
							char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], //���x��(Thick�`)
							int *iLabelCount)			  //�w���x��(Thick�`)�x�̊i�[��
{
	memcpy(szTitleXyz, m_pMeasureddata->szTitleXyz, sizeof(m_pMeasureddata->szTitleXyz));
	memcpy(szTitleAf, m_pMeasureddata->szTitleAf, sizeof(m_pMeasureddata->szTitleAf));
	memcpy(szLabel, m_pMeasureddata->szLabel, sizeof(m_pMeasureddata->szLabel));
	*iLabelCount = m_pMeasureddata->iLabelCount;
}

/////////////////////////////////////////////////////////////////////////////
void CNanoSpecDoc::GetLabelOri(char pszLabelOri[][ADAPRESULTSTRINGLENMAX + 1])
{
	memcpy(pszLabelOri, m_pMeasureddata->szLabelOri, sizeof(m_pMeasureddata->szLabelOri));
}

/////////////////////////////////////////////////////////////////////////////
//	���x�����ȂǊi�[(�wX,Y,Z�x,�wAF�x,�wThick�`�x,�wThick�`�̌��x)
void CNanoSpecDoc::SetLabel(char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1],  //���x��(X,Y,Z)
							char szTitleAf[AF_HEADER_LEN + 1],		 //���x��(AF)
							char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], //���x��(Thick�`)
							char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], //DDE��M�̃��x��(Thick�`)
							int iLabelCount)			 //�w���x��(Thick�`)�x�̊i�[��
{
	memcpy(m_pMeasureddata->szTitleXyz, szTitleXyz, sizeof(m_pMeasureddata->szTitleXyz));
	memcpy(m_pMeasureddata->szTitleAf, szTitleAf, sizeof(m_pMeasureddata->szTitleAf));
	memcpy(m_pMeasureddata->szLabel, szLabel, sizeof(m_pMeasureddata->szLabel));
	memcpy(m_pMeasureddata->szLabelOri, szLabelOri, sizeof(m_pMeasureddata->szLabelOri));
	m_pMeasureddata->iLabelCount = iLabelCount;
}


/////////////////////////////////////////////////////////////////////////////
//	���茋�ʕ\��(�ۑ�)�p���x�����̎擾(�wThick�`�x)
void CNanoSpecDoc::GetDispLabel(char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1])
{
	memcpy(szDispLabel, m_pMeasureddata->szDispLabel, sizeof(m_pMeasureddata->szDispLabel));
}

/////////////////////////////////////////////////////////////////////////////
//	���茋�ʕ\��(�ۑ�)�p���x�����̊i�[(�wThick�`�x)
void CNanoSpecDoc::SetDispLabel(char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1])
{
	memcpy(m_pMeasureddata->szDispLabel, szDispLabel, sizeof(m_pMeasureddata->szDispLabel));
}

/////////////////////////////////////////////////////////////////////////////
//	���茋�ʕ\��(�ۑ�)�p���x�����̊i�[���擾(�wThick�`�x)
int CNanoSpecDoc::GetDispLabelCount(void)
{
	return m_pMeasureddata->iDispLabelCount;
}

/////////////////////////////////////////////////////////////////////////////
//	���茋�ʕ\��(�ۑ�)�p���x�����̊i�[���i�[(�wThick�`�x)
void CNanoSpecDoc::SetDispLabelCount(int iDispLabelCount)
{
	m_pMeasureddata->iDispLabelCount = iDispLabelCount;
}


/////////////////////////////////////////////////////////////////////////////
//	�i�[�ό����̎擾 (���F���|�C���g���肵�����̐�)
int CNanoSpecDoc::GetStoreCount(void)
{
	return m_pMeasureddata->iSetPointCount;
}

/////////////////////////////////////////////////////////////////////////////
//	�i�[�ό����̊i�[ (���F���|�C���g���肵�����̐�)
void CNanoSpecDoc::SetStoreCount(int iSetPointCount)
{
	m_pMeasureddata->iSetPointCount = iSetPointCount;
}

/////////////////////////////////////////////////////////////////////////////
//	����|�C���g���̎擾 (���F���|�C���g���肷�邩�̐�)
int CNanoSpecDoc::GetMeasurePointCount(void)
{
	return m_pMeasureddata->iMeasureNum;
}

/////////////////////////////////////////////////////////////////////////////
//	����|�C���g���̊i�[ (���F���|�C���g���肷�邩�̐�) �����ǉ����̑��莞�ɃC���N�������g���邱�ƁI
void CNanoSpecDoc::SetMeasurePointCount(int iMeasureNum)
{
	m_pMeasureddata->iMeasureNum = iMeasureNum;
}

/////////////////////////////////////////////////////////////////////////////
//	���[�UID�̎擾
void CNanoSpecDoc::GetUserId(char szUserId[MAX_PATH + 1])
{
	memcpy(szUserId, m_pMeasureddata->szUserId, sizeof(m_pMeasureddata->szUserId));
}

/////////////////////////////////////////////////////////////////////////////
//	���[�UID�̊i�[
void CNanoSpecDoc::SetUserId(char szUserId[MAX_PATH + 1])
{
	memcpy(m_pMeasureddata->szUserId, szUserId, sizeof(m_pMeasureddata->szUserId));
}



/////////////////////////////////////////////////////////////////////////////
//	����J�n�����̎擾
void CNanoSpecDoc::GetStartDateTime(SYSTEMTIME* syStartDateTime)
{
	*syStartDateTime = m_pMeasureddata->syStartDateTime;
}

/////////////////////////////////////////////////////////////////////////////
//	����J�n�����̊i�[
void CNanoSpecDoc::SetStartDateTime(SYSTEMTIME* syStartDateTime)
{
	m_pMeasureddata->syStartDateTime = *syStartDateTime;
}


/////////////////////////////////////////////////////////////////////////////
//	����I�������̎擾
void CNanoSpecDoc::GetEndDateTime(SYSTEMTIME* syEndDateTime)
{
	*syEndDateTime = m_pMeasureddata->syEndDateTime;
}

/////////////////////////////////////////////////////////////////////////////
//	����I�������̊i�[
void CNanoSpecDoc::SetEndDateTime(SYSTEMTIME* syEndDateTime)
{
	m_pMeasureddata->syEndDateTime = *syEndDateTime;
}


/////////////////////////////////////////////////////////////////////////////
//	�T���v��ID�̎擾
void CNanoSpecDoc::GetSampleId(char szSampleId[SAMPLE_ID_LEN + 1])
{
	memcpy(szSampleId, m_pMeasureddata->szSampleId, sizeof(m_pMeasureddata->szSampleId));
}

/////////////////////////////////////////////////////////////////////////////
//	�T���v��ID�̊i�[
void CNanoSpecDoc::SetSampleId(char szSampleId[SAMPLE_ID_LEN + 1])
{
	memcpy(m_pMeasureddata->szSampleId, szSampleId, sizeof(m_pMeasureddata->szSampleId));
}

// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
/////////////////////////////////////////////////////////////////////////////
//	�T���v��ID�_�C�A���O������͐ݒ肳�ꂽ�ꍇ�ATRUE
void CNanoSpecDoc::SetSampleIdFromUI(BOOL bSet)
{
	m_bSetSampleIdFromUI = bSet;
}

/////////////////////////////////////////////////////////////////////////////
//	�T���v��ID�_�C�A���O������͐ݒ肳�ꂽ���ǂ���
BOOL CNanoSpecDoc::IsSetSampleIdFromUI()
{
	return m_bSetSampleIdFromUI;
}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--

//2009.10.13 bagus RS --{--
//���b�gID�ǉ�
/////////////////////////////////////////////////////////////////////////////
//	���b�gID�̎擾
void CNanoSpecDoc::GetLotId(char szLotId[LOT_ID_LEN + 1])
{
	memcpy(szLotId, m_pMeasureddata->szLotId, sizeof(m_pMeasureddata->szLotId));
}

/////////////////////////////////////////////////////////////////////////////
//	���b�gID�̊i�[
void CNanoSpecDoc::SetLotId(char szLotId[LOT_ID_LEN + 1])
{
	memcpy(m_pMeasureddata->szLotId, szLotId, sizeof(m_pMeasureddata->szLotId));
}
//2009.10.13 bagus RS --}--

/////////////////////////////////////////////////////////////////////////////
//	���茋�ʒP�ʖ��̎擾
void CNanoSpecDoc::GetUnitName(char szUnit[DISP_UNIT_NAME_LEN + 1])
{
	memcpy(szUnit, m_pMeasureddata->szUnit, sizeof(m_pMeasureddata->szUnit));
}

/////////////////////////////////////////////////////////////////////////////
//	���茋�ʒP�ʖ��̊i�[
void CNanoSpecDoc::SetUnitName(char szUnit[DISP_UNIT_NAME_LEN + 1])
{
	memcpy(m_pMeasureddata->szUnit, szUnit, sizeof(m_pMeasureddata->szUnit));
}


/////////////////////////////////////////////////////////////////////////////
//	���茋�ʒP��index�̎擾
int CNanoSpecDoc::GetUnitIndex(void)
{
	return m_pMeasureddata->iUnit;
}

/////////////////////////////////////////////////////////////////////////////
//	���茋�ʒP��index�̊i�[
void CNanoSpecDoc::SetUnitIndex(int iUnit)
{
	m_pMeasureddata->iUnit = iUnit;
}



/////////////////////////////////////////////////////////////////////////////
//	 //��ʂɕ\���ς݂̃|�C���g���̎擾(MeasurementListDlg�p)
int CNanoSpecDoc::GetShowCompPointList(void)
{
	return m_pMeasureddata->iShowCompletionPointList;
}

/////////////////////////////////////////////////////////////////////////////
//	 //��ʂɕ\���ς݂̃|�C���g���̊i�[(MeasurementListDlg�p)
void CNanoSpecDoc::SetShowCompPointList(int iShowCompletionPointList)
{
	m_pMeasureddata->iShowCompletionPointList = iShowCompletionPointList;
}

/////////////////////////////////////////////////////////////////////////////
//	 //��ʂɕ\���ς݂̃|�C���g���̎擾(MeasurementGraphDlg�p)
int CNanoSpecDoc::GetShowCompPointGraph(void)
{
	return m_pMeasureddata->iShowCompletionPointGraph;
}

/////////////////////////////////////////////////////////////////////////////
//	 //��ʂɕ\���ς݂̃|�C���g���̊i�[(MeasurementGraphDlg�p)
void CNanoSpecDoc::SetShowCompPointGraph(int iShowCompletionPointGraph)
{
	m_pMeasureddata->iShowCompletionPointGraph = iShowCompletionPointGraph;
}
//2009.09.02 bagus stress --{--
int CNanoSpecDoc::GetShowCompStressList(void)
{
	return m_pMeasureddata->iShowCompletionStressLineList;
}
void CNanoSpecDoc::SetShowCompStressList(int iShowCompletionStressLineList)
{
	m_pMeasureddata->iShowCompletionStressLineList = iShowCompletionStressLineList;
}
int CNanoSpecDoc::GetShowCompStressGraph(void)
{
	return m_pMeasureddata->iShowCompletionStressLineGraph;
}
void CNanoSpecDoc::SetShowCompStressGraph(int iShowCompletionStressLineGraph)
{
	m_pMeasureddata->iShowCompletionStressLineGraph = iShowCompletionStressLineGraph;
}
//2009.09.02 bagus stress --}--


/////////////////////////////////////////////////////////////////////////////
//�\��ŏI����_�擾
//�ʏ푪�莞:����|�C���g��
//�����W���[���[�h��:�đ���|�C���g�̈�ԑ傫���_
//�}�j���A�����莞,�e�X�g���[�h��:����ǉ����ɐ����C���N�������g(�����쐬 hibino)
int CNanoSpecDoc::GetLastMeasPoint(void)
{
	return m_iLastMeasPoint;
}

/////////////////////////////////////////////////////////////////////////////
//�\��ŏI����_�i�[
//�ʏ푪�莞:����|�C���g��
//�����W���[���[�h��:�đ���|�C���g�̈�ԑ傫���_
//�}�j���A�����莞,�e�X�g���[�h��:����ǉ����ɐ����C���N�������g(�����쐬 hibino)
void CNanoSpecDoc::SetLastMeasPoint(int iLastMeasPoint)
{
	m_iLastMeasPoint = iLastMeasPoint;
}


/////////////////////////////////////////////////////////////////////////////
//��ԍŌ�Ƀf�[�^���i�[(������)�����|�C���g�ԍ��̎擾
//(�ʏ푪�莞��GetStoreCount()�œ�����l�Ɠ���)
int CNanoSpecDoc::GetCurrentSetPoint(void)
{
	return m_iCurrentSetPoint;
}

/////////////////////////////////////////////////////////////////////////////
//��ԍŌ�Ƀf�[�^���i�[(������)�����|�C���g�ԍ��̊i�[
//(�ʏ푪�莞��GetStoreCount()�œ�����l�Ɠ���)
void CNanoSpecDoc::SetCurrentSetPoint(int iCurrentSetPoint)
{
	m_iCurrentSetPoint = iCurrentSetPoint;
}


/////////////////////////////////////////////////////////////////////////////
//�V�[�P���X���胂�[�h�̑����Ԏ擾(�V�[�P���X���蒆�̂ݎg�p��(�����W���[���܂�))
//(���蒆�łȂ�/�ʏ푪�蒆/�����W���[��)
int CNanoSpecDoc::GetSeqMeasMode(void)
{
	return m_iSeqMeasMode;
}

/////////////////////////////////////////////////////////////////////////////
//�V�[�P���X���胂�[�h�̑����Ԋi�[(�V�[�P���X���蒆�̂ݎg�p��(�����W���[���܂�))
//(���蒆�łȂ�/�ʏ푪�蒆/�����W���[��)
void CNanoSpecDoc::SetSeqMeasMode(int iSeqMeasMode)
{
	m_iSeqMeasMode = iSeqMeasMode;
}


/////////////////////////////////////////////////////////////////////////////
//���胂�[�h�擾
//(���蒆�łȂ�/�ʏ푪�蒆or�����W���[��/�}�j���A�����蒆/�e�X�g���[�h��)
int CNanoSpecDoc::GetMeasMode(void)
{
	return m_iMeasMode;
}

/////////////////////////////////////////////////////////////////////////////
//���胂�[�h�i�[
//(���蒆�łȂ�/�ʏ푪�蒆or�����W���[��/�}�j���A�����蒆/�e�X�g���[�h��)
void CNanoSpecDoc::SetMeasMode(int iMeasMode)
{
	m_iMeasMode = iMeasMode;
}


/////////////////////////////////////////////////////////////////////////////
//�}�j���A�����胂�[�h�̏�Ԃ��擾(���F�}�j���A�����胂�[�h���ȊO�͎g�p���Ȃ�)
int CNanoSpecDoc::GetManuMeasMode(void)
{
	return m_iManuMeasMode;
}

/////////////////////////////////////////////////////////////////////////////
//�}�j���A�����胂�[�h�̏�Ԃ��i�[(���F�}�j���A�����胂�[�h���ȊO�͎g�p���Ȃ�)
void CNanoSpecDoc::SetManuMeasMode(int iManuMeasMode)
{
	m_iManuMeasMode = iManuMeasMode;
}


/////////////////////////////////////////////////////////////////////////////
//�����W���[���̑���|�C���g���擾(���F���|�C���g���肷�邩�̐�)
int CNanoSpecDoc::GetReMeasNumScans(void)
{
	return m_iReMeasNumScans;
}

/////////////////////////////////////////////////////////////////////////////
//�����W���[���̑���|�C���g���i�[(���F���|�C���g���肷�邩�̐�)
void CNanoSpecDoc::SetReMeasNumScans(int iReMeasNumScans)
{
	m_iReMeasNumScans = iReMeasNumScans;
}


/////////////////////////////////////////////////////////////////////////////
//�����W���[���̑���σ|�C���g���擾(���F���|�C���g���肵�����̐�)
int CNanoSpecDoc::GetReMeasurePointCnt(void)
{
	return m_iReMeasurePointCnt;
}

/////////////////////////////////////////////////////////////////////////////
//�����W���[���̑���σ|�C���g���i�[(���F���|�C���g���肵�����̐�)
void CNanoSpecDoc::SetReMeasurePointCnt(int iReMeasurePointCnt)
{
	m_iReMeasurePointCnt = iReMeasurePointCnt;
}


/////////////////////////////////////////////////////////////////////////////
//�|�[�Y��ԃt���O�擾
BOOL CNanoSpecDoc::GetPauseFlg(void)
{
	return m_bPauseFlg;
}

/////////////////////////////////////////////////////////////////////////////
//�|�[�Y��ԃt���O�i�[
void CNanoSpecDoc::SetPauseFlg(BOOL bPauseFlg)
{
	m_bPauseFlg = bPauseFlg;
}



/////////////////////////////////////////////////////////////////////////////
//	PatRec ���쒆�t���O���Z�b�g���܂�
void CNanoSpecDoc::SetPatRecFlag(BOOL bFlag)
{
	if (0 != bFlag) {
		::InterlockedExchange((LPLONG) &m_bPatRecFlag, (LONG) TRUE);
	} else {
		::InterlockedExchange((LPLONG) &m_bPatRecFlag, (LONG) FALSE);
	}

}

/////////////////////////////////////////////////////////////////////////////
//	PatRec ���쒆�t���O���擾���܂�
BOOL CNanoSpecDoc::GetPatRecFlag()
{
	BOOL	l_bRc;

	/*	m_bPatRecFlag �́C�u0�v���u1�v�̒l�ɂ����Ȃ�܂���̂�
	 *	�u99�v���r�Ώےl�Ƃ��邱�Ƃŏ����݂��s���Ȃ��悤�ɂ��Ă��܂��D	*/
	if (0 != ::InterlockedCompareExchange((LONG*) &m_bPatRecFlag, (LONG) 99, (LONG) 99)) {
		l_bRc = TRUE;
	} else {
		l_bRc = FALSE;
	}

	return l_bRc;
}

/*
 *	���쒆�t���O�Q�A�N�Z�X�֐��Q
 */
/////////////////////////////////////////////////////////////////////////////
//	���쒆�t���O���Z�b�g���܂�
void CNanoSpecDoc::ActuateFlagsSet(
		ACTUATE_FLAGS_SEL ActFgSel, 	// ���ʒ萔(enum ACTUATE_FLAGS_SEL)
		BOOL bFlag						// TRUE or FALSE
	)
{

#if 0
	BOOL*	l_pbActuateFlag = (BOOL*) &m_ActuateFlags;
	if (ActFgSel < ACTUATE_MAX) {
		if (0 != bFlag) {
			::InterlockedExchange((LPLONG) l_pbActuateFlag, (LONG) TRUE);
		} else {
			::InterlockedExchange((LPLONG) l_pbActuateFlag, (LONG) FALSE);
		}
	}
#else
	BOOL* l_pbActuateFlag = 0;
	switch (ActFgSel) {
	case ACTUATE_XYSTAGE:
		l_pbActuateFlag = &(m_ActuateFlags.bXYStage);
		break;
	case ACTUATE_ZAXIS:
		l_pbActuateFlag = &(m_ActuateFlags.bZAxis);
		break;
	case ACTUATE_PIN:
		l_pbActuateFlag = &(m_ActuateFlags.bPin);
		break;
	case ACTUATE_SHUTTER:
		l_pbActuateFlag = &(m_ActuateFlags.bShutter);
		break;
	case ACTUATE_WORKGUIDE:
		l_pbActuateFlag = &(m_ActuateFlags.bWorkGuide);
		break;
	case ACTUATE_TURRET:
		l_pbActuateFlag = &(m_ActuateFlags.bTurret);
		break;
// 2009.11.10 bagus MS --{--
	case ACTUATE_MICROSCOPE:
		l_pbActuateFlag = &(m_ActuateFlags.bMS);
		break;
// 2009.11.10 bagus MS --}--
	default:
		break;
	}
	if (0 != l_pbActuateFlag) {
		if (0 != bFlag) {
			::InterlockedExchange((LPLONG) l_pbActuateFlag, (LONG) TRUE);
		} else {
			::InterlockedExchange((LPLONG) l_pbActuateFlag, (LONG) FALSE);
		}
	}
#endif

}

/////////////////////////////////////////////////////////////////////////////
//	���쒆�t���O���擾���܂�
BOOL CNanoSpecDoc::ActuateFlagsGet(ACTUATE_FLAGS_SEL ActFgSel)
{
	BOOL	l_bRc = TRUE;	// ����`�̏ꍇ�C�����Ȃ������ɂ��邽�� TRUE �ɂ��Ă���܂��D

#if 0
	BOOL*	l_pbActuateFlag = (BOOL*) &m_ActuateFlags;
	if (ActFgSel < ACTUATE_MAX) {
		if (0 != ::InterlockedCompareExchange((LONG*) (l_pbActuateFlag + ActFgSel), (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
	}
#else
	switch (ActFgSel) {
	case ACTUATE_XYSTAGE:
		/*	�t���O�́C�u0�v���u1�v�̒l�ɂ����Ȃ�܂���̂�
		 *	�u99�v���r�Ώےl�Ƃ��邱�Ƃŏ����݂��s���Ȃ��悤�ɂ��Ă��܂��D	*/
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bXYStage, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_ZAXIS:
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bZAxis, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_PIN:
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bPin, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_SHUTTER:
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bShutter, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_WORKGUIDE:
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bWorkGuide, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_TURRET:
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bTurret, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
// 2009.11.10 bagus MS --{--
	case ACTUATE_MICROSCOPE:
		if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bMS, (LONG) 99, (LONG) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
// 2009.11.10 bagus MS --}--
	default:
		break;
	}
#endif

	return l_bRc;
}

/////////////////////////////////////////////////////////////////////////////
//	���쒆�t���O���擾���܂�
//		�ǂꂩ�P�ł� TRUE �Ȃ� TRUE �ŕԂ��܂��D

DWORD CNanoSpecDoc::ActuateFlagsGetAll()
{
	DWORD	l_dwRc = 0x00000000;

#if 0
	l_bRc = FALSE;
	BOOL*	l_pbActuateFlag = (BOOL*) &m_ActuateFlags;
	for (int i = 0; i < ACTUATE_MAX; i++) {
		if (0 != ::InterlockedCompareExchange((LONG*) (l_pbActuateFlag + i), (LONG) 99, (LONG) 99)) {
			l_dwRc = TRUE;
			break;
		}
	}
#else
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bXYStage, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_XYSTAGE;
	}
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bZAxis, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_ZAXIS;
	}
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bPin, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_PIN;
	}
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bShutter, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_SHUTTER;
	}
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bWorkGuide, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_WORKGUIDE;
	}
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bTurret, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_TURRET;
	}
// 2009.11.10 bagus MS --{--
	if (0 != ::InterlockedCompareExchange((LONG*) &m_ActuateFlags.bMS, (LONG) 99, (LONG) 99)) {
		l_dwRc |= ACTUATE_MICROSCOPE;
	}
// 2009.11.10 bagus MS --}--
#endif

	return l_dwRc;
}

/////////////////////////////////////////////////////////////////////////////
//	�X�e�[�W�̈ړ����x����
void CNanoSpecDoc::StageSpeedLimiter(
		BOOL bLimiter	// TRUE �F���x�������܂�
						// FALSE�F���x�������������܂�
	)
{
	if (0 == bLimiter) {
		// �������������܂� ---------------------------------------------------
		// ���x�������[�h�����Z�b�g
		StageSetLocalSpeed(1, 1);
	} else {
		// �������܂� ---------------------------------------------------------
		// ���x�������[�h���Z�b�g
		double l_dSharpMainteSpdX = 200.0;	// X ���F�V���[�v�̃����e���x
		double l_dSharpMainteSpdY = 200.0;	// Y ���F�V���[�v�̃����e���x
		double l_dMaxSpdX, l_dMaxSpdY;
		StageGetMaxStageSpeed(&l_dMaxSpdX, &l_dMaxSpdY);	// Stage.dll ����ő呬�x��Ǐo��
		if (l_dMaxSpdX <= 0) {l_dMaxSpdX = l_dSharpMainteSpdX;}
		if (l_dMaxSpdY <= 0) {l_dMaxSpdY = l_dSharpMainteSpdY;}
		double l_dSpdRateX = l_dSharpMainteSpdX / l_dMaxSpdX;
		if (l_dSpdRateX <= 0) {l_dSpdRateX = 1.0;}
		if (1.0 < l_dSpdRateX) {l_dSpdRateX = 1.0;}
		double l_dSpdRateY = l_dSharpMainteSpdY / l_dMaxSpdY;
		if (l_dSpdRateY <= 0) {l_dSpdRateY = 1.0;}
		if (1.0 < l_dSpdRateY) {l_dSpdRateY = 1.0;}
		StageSetLocalSpeed(l_dSpdRateX, l_dSpdRateY);
	}
}

// ==========================================================================
// �W���C�X�e�B�b�N�̐ؑւ�r���I�ɏ�������
// ==========================================================================
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- { -------- */
//BOOL CNanoSpecDoc::ChangeJoyStick(BOOL flag)
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- 		   */
BOOL CNanoSpecDoc::ChangeJoyStick(BOOL flag, int iSpeedSel)
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- } -------- */
{
BOOL bJoyStk = FALSE;

	if( m_ActualJoyStickFlag == FALSE ){
		m_ActualJoyStickFlag = TRUE;

		CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
		if(!m_pMainFrame->HardwareSimulation()){
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- { -------- */
//			bJoyStk = StageEnableJoyStick(flag);
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- 		   */
			SYSTEM_CONFIG l_SystemConfig;
			ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			if (STAGE_TYPE_SPT == l_SystemConfig.nStageType) {
				/* SPT �X�e�[�W�̏ꍇ */
				bJoyStk = StageEnableJoyStickSPT(flag, iSpeedSel);
			} else {
				/* TOHO�CNTN �X�e�[�W�̏ꍇ */
				bJoyStk = StageEnableJoyStick(flag);
			}
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- } -------- */
// DEL 2009.06.01
//			if( !bJoyStk ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
		}
		else{
			bJoyStk = TRUE;
		}
// 2009.08.07 K.Matsuo Bug Fix SPT�X�e�[�W������~����JOYSTICK�ύX -->
//		ActuateFlagsSet(ACTUATE_XYSTAGE, flag);
		if ( bJoyStk )
			ActuateFlagsSet(ACTUATE_XYSTAGE, flag);
// 2009.08.07 K.Matsuo Bug Fix SPT�X�e�[�W������~����JOYSTICK�ύX <--
		m_ActualJoyStickFlag = FALSE;
	}
	return bJoyStk;
}
//Saiki 20090608 Delete ----->
// ==========================================================================
// �x�����b�Z�[�W��\������
// ==========================================================================
//void CNanoSpecDoc::MessageIf_Set(int m_MsgID)
//{
//
//	  CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
//	  m_pMainFrame->PostMessage(WM_MESSAGE_SHOW, m_MsgID, 0);
//}
//Saiki 20090608 Delete <-----

/* ==========================================================================
   �eI/O���`�F�b�N����
   ========================================================================*/
BOOL CNanoSpecDoc::IsInterLock()
{
	if( nexioIsEmergencyStop()		!= ON ) {AlarmIf_Set(ALID_EMOSwitchOn); return TRUE;} /* EMO�X�C�b�`�E�I�� */
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- { -------- */
//	if( nexioIsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* �h�A�I�[�v�� */
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- 		   */
	if( Rap_IsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* �h�A�I�[�v�� */
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- } -------- */
	if( nexioIsEquipmentPower() 	!= ON ) {AlarmIf_Set(ALID_EquipmentPowerOff); return TRUE;} /* ���u�d���E�I�t */
/* added 2009.10.30 hmenjo CTA CTAILPI ����O�`�F�b�N ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_CTA)) {
		/* CTA �̏ꍇ�̂�	*/
		if (0 == nexioIsCA_Interlock()) {AlarmIf_Set(ALID_CTA_IL_PI_On); return TRUE;}	/* CTAILPI �I�t	*/
	}
/* added 2009.10.30 hmenjo CTA CTAILPI ����O�`�F�b�N ---------- } ---------- */

/* added 2009.11.06 K.Matsuo Resist ����O�`�F�b�N ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_4PP)) {
		/* Resist �̏ꍇ�̂�	*/
		BOOL l_bUpper = nexioIsRS_ProbeHeadUpperPos();
		BOOL l_bLower = nexioIsRS_ProbeHeadLowerPos();
		if (0 != l_bUpper && 0 == l_bLower) {
			;
		} else if (0 == l_bUpper && 0 != l_bLower) {
			AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL �I�t���o	*/
			return TRUE;
		} else {
			AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL �ُ팟�o	*/
			return TRUE;
		}
	}
/* added 2009.11.06 K.Matsuo Resist ����O�`�F�b�N ---------- } ---------- */

// 2009.11.03 bagus MS --{--
	if(0 != IsValidHeadType(HEAD_TYPE_MS)){
		//if(0 != !nexioIsMS_LowerPos2()){ AlarmIf_Set(ALID_MS_IL_POS_ERROR); return TRUE; }
		if(1 != nexioIsMS_LowerPos2()){ AlarmIf_Set(ALID_MS_IL_POS_ERROR); return TRUE; }
	}
// 2009.11.03 bagus MS --}--
	return FALSE;
}
/* ==========================================================================
   �eI/O���`�F�b�N����
   ========================================================================*/
BOOL CNanoSpecDoc::IsInterLockForMScope()
{
	if( nexioIsEmergencyStop()		!= ON ) {AlarmIf_Set(ALID_EMOSwitchOn); return TRUE;} /* EMO�X�C�b�`�E�I�� */
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- { -------- */
//	if( nexioIsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* �h�A�I�[�v�� */
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- 		   */
	if( Rap_IsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* �h�A�I�[�v�� */
/* modified hmenjo 2009.05.20 SPT �p JoyStick �Ή� -------- } -------- */
	if( nexioIsEquipmentPower() 	!= ON ) {AlarmIf_Set(ALID_EquipmentPowerOff); return TRUE;} /* ���u�d���E�I�t */
/* added 2009.10.30 hmenjo CTA CTAILPI ����O�`�F�b�N ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_CTA)) {
		/* CTA �̏ꍇ�̂�	*/
		if (0 == nexioIsCA_Interlock()) {AlarmIf_Set(ALID_CTA_IL_PI_On); return TRUE;}	/* CTAILPI �I�t	*/
	}
/* added 2009.10.30 hmenjo CTA CTAILPI ����O�`�F�b�N ---------- } ---------- */

/* added 2009.11.06 K.Matsuo Resist ����O�`�F�b�N ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_4PP)) {
		/* Resist �̏ꍇ�̂�	*/
		BOOL l_bUpper = nexioIsRS_ProbeHeadUpperPos();
		BOOL l_bLower = nexioIsRS_ProbeHeadLowerPos();
		if (0 != l_bUpper && 0 == l_bLower) {
			;
		} else if (0 == l_bUpper && 0 != l_bLower) {
			AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL �I�t���o	*/
			return TRUE;
		} else {
			AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL �ُ팟�o	*/
			return TRUE;
		}
	}
/* added 2009.11.06 K.Matsuo Resist ����O�`�F�b�N ---------- } ---------- */

	return FALSE;
}


/* modified 2009.08.20 hmenjo Pin Aling �ʒu�L���� ---------- { ---------- */
//BOOL CNanoSpecDoc::CheckUnitStatus()
/* modified 2009.08.20 hmenjo Pin Aling �ʒu�L���� ----------			   */
BOOL CNanoSpecDoc::CheckUnitStatus(BOOL bEnablePinAlign/* = FALSE*/)
/* modified 2009.08.20 hmenjo Pin Aling �ʒu�L���� ---------- } ---------- */
{
	//Saiki 20090530 Change ----->
	CString strMsg;
	if( nexioIsShutterClose()		!= ON ) {
		LoadStringML(IDS_LOADING_SHUTTER_OPEN, strMsg, "Sample Loading Shutter is opened.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ���[�f�B���O�V���b�^�[�[�O��
	if( nexioIsRobotArmDetect() 	!= ON ) {
		LoadStringML(IDS_DETECT_ROBOT_ARM, strMsg, "Robot Arm is detected.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ���{�b�g�A�[�����o
/* modified 2009.08.20 hmenjo Pin Aling �ʒu�L���� ---------- { ---------- */
//	if( nexioIsPinDownPos() 		!= ON ) {
//		  LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
//		MessageStringIf_Set(strMsg);
//		return TRUE;
//	}	// �s�����[�O��
/* modified 2009.08.20 hmenjo Pin Aling �ʒu�L���� ----------			   */
	/* �s�����~�[(Align�ʒu)�O�� �`�F�b�N*/
	if (TRUE != bEnablePinAlign) {
		if (nexioIsPinDownPos() != ON) {
			LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
			MessageStringIf_Set(strMsg);
			return TRUE;
		}	// �s�����[�O��
	} else {
		/* Align �ʒu���܂߂܂��D	*/
		if (
			(ON == nexioIsPinUpperPos())
		 || ((ON == nexioIsPinDownPos()) && (ON == nexioIsPinAlignmentPos()))
			) {
			LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
			MessageStringIf_Set(strMsg);
			return TRUE;
		}
	}
/* modified 2009.08.20 hmenjo Pin Aling �ʒu�L���� ---------- } ---------- */
	if( nexioIsWorkGuideClose() 	!= OFF) {
		LoadStringML(IDS_CLAMP_NOT_CLOSED, strMsg, "Work Guide is not Close Position.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ���[�N�K�C�hClose�ʒu
	if( nexioIsWorkGuideOpen()		!= ON ) {
		LoadStringML(IDS_CLAMP_NOT_OPEND, strMsg, "Work Guide is not Open Position.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ���[�N�K�C�hOpen�ʒu

	//Saiki 20090530 Change <-----


	return FALSE;
}

/* ==========================================================================
   �eActive Flag���`�F�b�N����
   ========================================================================*/
BOOL CNanoSpecDoc::CheckActiveFlag()
{
	CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	//Saiki 20090530 Change ----->
	CString strMsg;

	if(ActuateFlagsGet(ACTUATE_ZAXIS))		{
		LoadStringML(IDS_Z_AXIS_WORKING, strMsg, "Z Axis is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //�y�������쒆�ł�
	if(ActuateFlagsGet(ACTUATE_PIN))		{
		LoadStringML(IDS_LIFTER_WORKING, strMsg, "Sample Lifter is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //���t�^�[�����쒆�ł�
	if(ActuateFlagsGet(ACTUATE_WORKGUIDE))	{
		LoadStringML(IDS_CLAMP_WORKING, strMsg, "Work Guide is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //���[�N�K�C�h�����쒆�ł�
	if(ActuateFlagsGet(ACTUATE_TURRET)) 	{
		LoadStringML(IDS_TURRET_WORKING, strMsg, "Turret is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //�^�[���b�g�����쒆�ł�
	//Saiki 20090530 Change <-----
	return FALSE;
}

/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- { -------- */
BOOL CNanoSpecDoc::Rap_IsAirPressureLowON(BOOL bIgnore/* = 0*/)
{
	BOOL l_bRc = FALSE;

//	BOOL l_bActiveHigh = TRUE;		/* �A�N�e�B�u High */
	BOOL l_bActiveHigh = FALSE;		/* �A�N�e�B�u Low */

	/* Chief �ȊO�ł͖�������d�l�ɂȂ���(by�쓇20090521)����
	   ������ bIgnore �����Ă��܂�(�f�t�H���g�͖������܂���)�D
	   �������Ȃ��Ȃ�C���� if �u���b�N���폜���Ă��������D */
	if (1 == bIgnore) {
		return FALSE;
	}

	/* �G�A���͒ቺ�̖����t���O�m�F(ini �t�@�C���Ȃǂ���) */
	if (0) {	// �K�v�ł���΁C�����ɖ����t���O�̔�������Ă��������D
		return FALSE;
	}

	if (0 == nexioIsAirPressureLevelLow()) {
		/* Low ���x���ł����D */
		l_bRc = (TRUE == l_bActiveHigh)? FALSE : TRUE;
	} else {
		/* High ���x���ł����D */
		l_bRc = (TRUE == l_bActiveHigh)? TRUE : FALSE;
	}

	return l_bRc;
}
/* added hmenjo 2009.05.20 �G�A���͒ቺ���o�̃��b�p�֐� -------- } -------- */

/* added hmenjo 2009.05.20 �h�A�C���^���b�N�������Ή� -------- { -------- */
BOOL CNanoSpecDoc::Rap_IsDoorInterlock()
{
	BOOL l_bRc;

	if ((0 == nexioIsMaintenanceSwitch()) && (FALSE == g_bIL_DoorOpen)) {
		// �����e�i���X���[�h�ŁC���o�֎~�ݒ�̏ꍇ�́C�h�A�ŕԂ��܂��D
		l_bRc = TRUE;
	} else {
		l_bRc = nexioIsDoorInterlock();
	}

	return l_bRc;
}
/* added hmenjo 2009.05.20 �h�A�C���^���b�N�������Ή� -------- } -------- */
//Saiki 20090530 Add ----->
void CNanoSpecDoc::MessageStringIf_Set(CString strBuffer)
{

	CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pMainFrame->MessageShow(strBuffer);
}
//Saiki 20090530 Add <-----

// ==========================================================================
// �n�[�hJoy���ꎞ�I�ɋ֎~�ɂ���
BOOL CNanoSpecDoc::JoyStickChangeDisable()
{
// 2013.12.16 Bagus Add (TohoSpec�Ή�) -->
	BOOL bRet = FALSE;
// 2013.12.16 Bagus Add (TohoSpec�Ή�) <--

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// JoyStick��ؑ֋֎~�ɂ���
	m_pDoc->SetJoyStickSelectEnabled(FALSE);

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

// 2013.12.16 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.12.16 Bagus Add (TohoSpec�Ή�) <--
	if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		// �n�[�hJoyStick���I������Ă�����
		if( m_pMainFrame->GetJoyStickMode() == JOY_HARD_MODE ){
			// JoyStick���ꎞ�I�ɋ֎~�ɂ���i�A���A������Ԃ͕ύX���Ȃ��j
			if( !m_pDoc->ChangeJoyStick(FALSE) ) return FALSE;
				Sleep(300);
		}
// 2013.12.16 Bagus Delete (TohoSpec�Ή�) -->
//		BOOL bRet = FALSE;
// 2013.12.16 Bagus Delte (TohoSpec�Ή�) <--
		if(!g_bHWS){
			bRet = StageGetJoyStickSPT();
		}
// 2013.12.16 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.12.16 Bagus Add (TohoSpec�Ή�) <--

	return bRet == FALSE ? bRet = TRUE : bRet = FALSE;
}

// ==========================================================================
// �n�[�hJoy�̏�Ԃ����ɖ߂�
BOOL CNanoSpecDoc::JoyStickStatusRestore()
{
	BOOL bRet = TRUE;
	BOOL bJoyStk;

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

// 2013.12.16 Bagus Add (TohoSpec�Ή�) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.12.16 Bagus Add (TohoSpec�Ή�) <--
	if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
		// JoyStick�̃��[�h��߂�
		if( m_pMainFrame->GetJoyStickMode() == JOY_HARD_MODE ){
			// �n�[�hJoyStick���I������Ă�����
			if(!g_bHWS){
				if( !StageGetJoyStickSPT() ){
					bJoyStk = ChangeJoyStick(TRUE);		// JoyStick�����ɂ���
					Sleep(300);
					if( !StageGetJoyStickSPT() ){
						bJoyStk = ChangeJoyStick(TRUE);		// JoyStick�����ɂ���
						Sleep(300);
						if( !StageGetJoyStickSPT() ){
							bRet = FALSE;
						}
					}
				}
			}
			else{
				bJoyStk = ChangeJoyStick(TRUE);		// JoyStick�����ɂ���
			}
		}
		else{
			// �\�t�gJoyStick���I������Ă�����
			BOOL bJoyStk = ChangeJoyStick(FALSE);		// JoyStick���֎~�ɂ���
		}
// 2013.12.16 Bagus Add (TohoSpec�Ή�) -->
	}
// 2013.12.16 Bagus Add (TohoSpec�Ή�) <--

	// JoyStick�ؑ֋��ɖ߂�
	SetJoyStickSelectEnabled(TRUE);

	return bRet;
}
/* added 2009.07.23 hmenjo �w�b�h�^�C�v�`�F�b�N�֐��ǉ� ---------- { ---------- */
/*	�w�b�h�^�C�v���V�X�e���ݒ�ŗL�����ǂ����`�F�b�N���܂��D	*/
BOOL CNanoSpecDoc::IsValidHeadType(WORD wHeadType)
{
	/* �V�X�e���R���t�B�O����w�b�h�L��/�������擾	*/
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	BOOL l_bValidHead = FALSE;
	switch (wHeadType) {
	case HEAD_TYPE_SR:		l_bValidHead = l_SystemConfig.HeadType.bSR;		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
//	case HEAD_TYPE_SE:		l_bValidHead = l_SystemConfig.HeadType.bSE;		break;
	case HEAD_TYPE_SE:		l_bValidHead = l_SystemConfig.HeadType.bSE && !l_SystemConfig.HeadType.bCompEASE;	break;
	case HEAD_TYPE_COMPEASE:l_bValidHead = l_SystemConfig.HeadType.bSE && l_SystemConfig.HeadType.bCompEASE;	break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
//	case HEAD_TYPE_IRSE:	l_bValidHead = l_SystemConfig.HeadType.bIRSE;	break;
	case HEAD_TYPE_MS:		l_bValidHead = l_SystemConfig.HeadType.bMS;		break;
// 2009.10.19 bagus MS �ǉ� --}--
	case HEAD_TYPE_4PP:		l_bValidHead = l_SystemConfig.HeadType.bResist;	break;
	case HEAD_TYPE_CTA:		l_bValidHead = l_SystemConfig.HeadType.bCTA;	break;
	case HEAD_TYPE_STRESS:	l_bValidHead = l_SystemConfig.HeadType.bStress;	break;
	default:
		l_bValidHead = FALSE;
		break;
	}

	return l_bValidHead;
}
/* added 2009.07.23 hmenjo �w�b�h�^�C�v�`�F�b�N�֐��ǉ� ---------- } ---------- */
/* added 2009.07.23 hmenjo �X�L�����^�C�v�`�F�b�N�֐��ǉ� ---------- { ---------- */
/*	�X�L�����^�C�v�ƃw�b�h�^�C�v�̐��������`�F�b�N���܂��D*/
/*			�e�w�b�h���ɒ�`����Ă���X�L�����^�C�v�͂��ׂĂO�Ŏn�܂��Ă��܂��D
 *			�܂�C�ʂ̃w�b�h�̃X�L�����^�C�v���ݒ肳��Ă��Ă��l�������Ȃ��߁C
 *			�L���ƂȂ��Ă��܂����Ƃ�����܂��D
 *			�������C����͓��͉�ʂŐ�������Ă���͂��ł��̂ŁC����v���O�����t�@�C����
 *			���ڕύX���Ȃ�����s�s���͋N���܂���D
 *			���ۂɂ́C�������̕ύX����������Ȃ��߁C�R�R�ł͔͈̓`�F�b�N�݂̂Ƃ��Ă��܂��D*/
BOOL CNanoSpecDoc::IsValidScanType(WORD wScanType, WORD wHeadType)
{
	/* �܂��w�b�h�^�C�v���`�F�b�N���܂��D*/
	if (0 == IsValidHeadType(wHeadType)) {
		return FALSE;
	}

	/* �w�b�h�^�C�v���ɃX�L�����^�C�v���`�F�b�N���܂��D*/
	BOOL l_bRet = FALSE;
	switch (wHeadType) {
	case HEAD_TYPE_SR:
		if ((MEAS_PROG_TYPE_SR_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_SR_MAX)) {l_bRet = TRUE;}
		break;
	case HEAD_TYPE_SE:
		if ((MEAS_PROG_TYPE_SE_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_SE_MAX)) {l_bRet = TRUE;}
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
		if ((MEAS_PROG_TYPE_COMPEASE_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_COMPEASE_MAX)) {l_bRet = TRUE;}
		break;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
// 2009.10.19 bagus MS �ǉ� --{--
#if 0
	case HEAD_TYPE_IRSE:
		if ((MEAS_PROG_TYPE_IRSE_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_IRSE_MAX)) {l_bRet = TRUE;}
		break;
#else
	case HEAD_TYPE_MS:
		if ((MEAS_PROG_TYPE_MS_MEAS <= wScanType) && (wScanType < MEAS_PROG_TYPE_MS_MAX)) {l_bRet = TRUE;}
		break;
#endif
// 2009.10.19 bagus MS �ǉ� --}--
	case HEAD_TYPE_4PP:
		if ((MEAS_PROG_TYPE_4PP_MEAS <= wScanType) && (wScanType < MEAS_PROG_TYPE_4PP_MAX)) {l_bRet = TRUE;}
		break;
	case HEAD_TYPE_CTA:
		if ((MEAS_PROG_TYPE_CTA_MEAS <= wScanType) && (wScanType < MEAS_PROG_TYPE_CTA_MAX)) {l_bRet = TRUE;}
		break;
	case HEAD_TYPE_STRESS:
		if ((MEAS_PROG_TYPE_STRESS_MEAS <= wScanType) && (wScanType < MEAS_PROG_TYPE_STRESS_MAX)) {l_bRet = TRUE;}
		break;
	default:
		l_bRet = FALSE;
		break;
	}

	return l_bRet;
}
/* added 2009.07.23 hmenjo �X�L�����^�C�v�`�F�b�N�֐��ǉ� ---------- } ---------- */
/* added 2009.07.30 hmenjo �X�g���X�@�\�ǉ�(4) ---------- { ---------- */
/*
 *	�A���[�����擾
 *		AlarmList.ini �ɓo�^����Ă������Ǐo���܂��D
 */
BOOL CNanoSpecDoc::GetAlarmInf(int iAlId, int* piAlCd, char* pcLevel, TCHAR* ptszAlText, int iTextBufLen)
{
	BOOL l_bRet = FALSE;

	if ((iAlId < 1) || (ALARM_LIST_MAX <= iAlId)) {
		l_bRet = FALSE;
	} else if ((0 != ptszAlText) && (iTextBufLen < (ALARM_LOG_TEXT_LEN + 1))) {
		l_bRet = FALSE;
	} else {
		((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_pAlarm;
		int l_iAlId = iAlId - 1;
		if (0 != piAlCd) {
			*piAlCd = ((CAlarm*) ((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_pAlarm)->m_AlarmList[l_iAlId].Alcd;
		}
		if (0 != pcLevel) {
			*pcLevel = ((CAlarm*) ((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_pAlarm)->m_AlarmList[l_iAlId].Level;
		}
		if (0 != ptszAlText) {
			_tcscpy(ptszAlText, ((CAlarm*) ((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_pAlarm)->m_AlarmList[l_iAlId].Text);
		}
		l_bRet = TRUE;
	}

	if (FALSE == l_bRet) {
		if (0 != piAlCd) {*piAlCd = 0;}
		if (0 != pcLevel) {*pcLevel = 0;}
		if (0 != ptszAlText) {_tcscpy(ptszAlText, _T(""));}
	}

	return l_bRet;
}
/* added 2009.07.30 hmenjo �X�g���X�@�\�ǉ�(4) ---------- } ---------- */
/* added 2009.07.31 hmenjo �X�g���X�@�\�ǉ�(3) ---------- { ---------- */
/*
 *	HEPA �I��/�I�t ���b�p�֐�
 */
BOOL CNanoSpecDoc::HepaOnOff(BOOL bOn, char* pcHepaAlarmLevel/*= 0*/)
{
	/* HEPA �A���[�����x�����擾	*/
	if (0 != pcHepaAlarmLevel) {
		char l_cLevel;
/* 2009.08.20 K.Matsuo ALID ��`��ύX ---------- { ---------- */
//		this->GetAlarmInf(HEPA_ALID, 0, &l_cLevel, 0, 0);
/* 2009.08.20 K.Matsuo ALID ��`��ύX ----------			   */
		this->GetAlarmInf(ALID_HepaStopFailed, 0, &l_cLevel, 0, 0);
/* 2009.08.20 K.Matsuo ALID ��`��ύX ---------- } ---------- */
		*pcHepaAlarmLevel = l_cLevel;
	}

	/* �R�R�� NEXTRA �̊֐����������H�H�H	*/
	/*	NEXTRA ����̖߂肪����	*/
	/*	NEXTRA ����̖߂肪����	*/
	BOOL l_bRet = FALSE;
	CTimer timer;

	nexioHepaPowerOFF(!bOn);
	timer.Restart(3);
	while ( !timer.IsTimeout() && !l_bRet ) l_bRet = nexioIsHepaStatus();

	return l_bRet;
}
/* added 2009.07.31 hmenjo �X�g���X�@�\�ǉ�(3) ---------- } ---------- */
//2009.08.20 bagus stress --{--
BOOL CNanoSpecDoc::SetOneStressLineData(STRESSRESULT *OneStressLineData,int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //�i�[���s
	}
	m_pMeasureddata->StressData[iLineNo - 1] = *OneStressLineData;
	return TRUE; //�i�[����
}
BOOL CNanoSpecDoc::GetOneStressLineData(STRESSRESULT *OneStressLineData,int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //�擾���s
	}
	*OneStressLineData = m_pMeasureddata->StressData[iLineNo - 1];
	return TRUE; //�擾����
}
BOOL CNanoSpecDoc::GetOneStressLineDataValid(int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //�擾���s
	}
	return m_pMeasureddata->StressDataValid[iLineNo - 1];
}
void CNanoSpecDoc::SetOneStressLineDataValid(int iLineNo,BOOL bValid)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return; //�擾���s
	}
	m_pMeasureddata->StressDataValid[iLineNo - 1] = bValid;
}
BOOL CNanoSpecDoc::GetOneStressLineDataRef(int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //�擾���s
	}
	return m_pMeasureddata->StressDataRef[iLineNo - 1];
}
void CNanoSpecDoc::SetOneStressLineDataRef(int iLineNo,BOOL bRef)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return; //�擾���s
	}
	m_pMeasureddata->StressDataRef[iLineNo - 1] = bRef;
}
BOOL CNanoSpecDoc::IsStressRefereneMeasuremnt()
{
	BOOL bRef = FALSE;

	for(int n = 0;n < STRESS_LINES_MAX;n++){
		if(m_pMeasureddata->StressDataRef[n]){
			bRef = TRUE;
			break;
		}
	}
	return bRef;
}
//2009.08.20 bagus stress --}--
//2009.08.30 bagus stress --{---
int CNanoSpecDoc::GetCurrentMeasureStressLine()
{
	return m_iCurrentMeasStressLine;
}
void CNanoSpecDoc::SetCurrentMeasureStressLine(int nLineNo)
{
	m_iCurrentMeasStressLine = nLineNo;
}
//2009.08.30 bagus stress --}---

//2009.08.31 bagus stress --{--
void CNanoSpecDoc::GetStressStatisticsData(STATISTICS *pStat)
{
	*pStat = m_pMeasureddata->StressStatistics;
}
void CNanoSpecDoc::SetStressStatisticsData(STATISTICS *pStat)
{
	m_pMeasureddata->StressStatistics = *pStat;
}
void CNanoSpecDoc::GetStressLineStatisticsData(int iLineNo,STATISTICS *pStat)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return ;
	}
	*pStat = m_pMeasureddata->StressLineStatistics[iLineNo-1];
}
void CNanoSpecDoc::SetStressLineStatisticsData(int iLineNo,STATISTICS *pStat)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return ;
	}
	m_pMeasureddata->StressLineStatistics[iLineNo-1] = *pStat;
}
//2009.08.31 bagus stress --}--
//2009.09.01 bagus stress --{--
int CNanoSpecDoc::GetMeasuredStressLSNum()
{
	int nLine;
	int nLSNum = 0;

	for(nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
		if(m_pMeasureddata->StressDataValid[nLine]){
			nLSNum += m_pMeasureddata->StressData[nLine].nSectionOccurrence;
		}
	}
	return nLSNum;
}
//2009.09.01 bagus stress --}--
//2009.10.28 bagus 2point-Distance --{--
STAGE_COORD CNanoSpecDoc::GetLastDistancePoint()
{
	return m_pMeasureddata->LastDistancePoint;
}
void CNanoSpecDoc::SetLastDistancePoint(STAGE_COORD tCoord)
{
	m_pMeasureddata->LastDistancePoint = tCoord;
}
//2009.10.28 bagus 2point-Distance --}--
/* added 2009.10.20 hmenjo GTR �����v������� ---------- { ---------- */
/* added 2009.09.07 hmenjo ���ߗ� �����v���� ---------- { ---------- */
/*
 *	���ߗ��p�����v����
 */
BOOL CNanoSpecDoc::SetTransmittanceLamp(WORD wFilter/* = FILTER_DARK*/)
{
	static WORD ls_wFilterPre = -1;	/* �����l	*/
	BOOL l_bRet = TRUE;
	WORD l_wFilter = wFilter;

	if (ls_wFilterPre != l_wFilter) {
/* deleted 2009.11.20 hmenjo GTR �����v�������� Close ---------- { ---------- */
//		ls_wFilterPre = l_wFilter;
/* deleted 2009.11.20 hmenjo GTR �����v�������� Close ---------- } ---------- */
		/* �R�R�ɓ��ߗ������v�̐�������Ă��������D	*/
		switch (l_wFilter) {
		case FILTER_OPEN:	/* OPEN	*/
			nexioTransmitLightSource1Shutter(FALSE);
			nexioTransmitLightSource2Shutter(FALSE);
			nexioTransmitLightSource3Shutter(FALSE);
			break;
		case FILTER_DARK:	/* CLOSE	*/
		default:
			nexioTransmitLightSource1Shutter(TRUE);
			nexioTransmitLightSource2Shutter(TRUE);
			nexioTransmitLightSource3Shutter(TRUE);
			break;
		}
		/* �V���b�^���슮���҂�	*/
		SR_CONFIG l_SrConfig;
		ConfigFile_GetNanoSpecIni(&l_SrConfig, CONFIG_FILE_SR_CONFIG);
		DWORD l_dwShutterWait = ((DWORD) l_SrConfig.wTrShutterMoveWaitTime) + 10;	/* �덷�������Z	*/
		::Sleep(l_dwShutterWait);
		/* ���퐫�`�F�b�N	*/
		BOOL l_bResult1 = nexioIsTransmitLight1Condition();
		BOOL l_bResult2 = nexioIsTransmitLight2Condition();
		BOOL l_bResult3 = nexioIsTransmitLight3Condition();
/* modified 2009.11.24 hmenjo GTR �����v�G���[���o�C�� ---------- { ---------- */
//		BOOL l_bResultAll = l_bResult1 && l_bResult2 && l_bResult3;
//		switch (l_wFilter) {
//		case FILTER_OPEN:	/* OPEN	*/
//			if ((0 != l_bResult1) && (0 != l_bResult2) && (0 != l_bResult3)) {
//				l_bRet = TRUE;
//			} else {
//				l_bRet = FALSE;
//			}
//			break;
//		case FILTER_DARK:	/* CLOSE	*/
//		default:
//			if ((0 == l_bResult1) && (0 == l_bResult2) && (0 == l_bResult3)) {
//				l_bRet = TRUE;
//			} else {
//				l_bRet = FALSE;
//			}
//			break;
//		}
/* modified 2009.11.24 hmenjo GTR �����v�G���[���o�C�� ----------			   */
		BOOL l_bResultAll = l_bResult1 || l_bResult2 || l_bResult3;
		switch (l_wFilter) {
		case FILTER_OPEN:
			if (0 != l_bResultAll) {
				l_bRet = FALSE;
			}
			break;
		case FILTER_DARK:
		default:
			l_bRet = TRUE;
			break;
		}
/* modified 2009.11.24 hmenjo GTR �����v�G���[���o�C�� ---------- } ---------- */
	}

/* added 2009.11.20 hmenjo GTR �����v�������� Close ---------- { ---------- */
	if (TRUE == l_bRet) {
		ls_wFilterPre = l_wFilter;
	}
/* added 2009.11.20 hmenjo GTR �����v�������� Close ---------- } ---------- */

	return l_bRet;
}
/* added 2009.09.07 hmenjo ���ߗ� �����v���� ---------- } ---------- */
/* added 2009.10.20 hmenjo GTR �����v������� ---------- } ---------- */

/* added 2009.10.20 hmenjo GTR �����v�`�F�b�N���� ---------- { ---------- */
/* added 2009.09.10 hmenjo �K���g�����ߗ��p�����v�`�F�b�N ---------- { ---------- */
/*
 *	�K���g�����ߗ��p�n���Q�������v �I���m�F
 */
BOOL CNanoSpecDoc::IsGTRLampOn(void)
{
	BOOL l_bLampOn;

	l_bLampOn = nexioIsTransmitLightSourcePower();

	return l_bLampOn;
}
/* added 2009.09.10 hmenjo �K���g�����ߗ��p�����v�`�F�b�N ---------- } ---------- */
/* added 2009.10.20 hmenjo GTR �����v�`�F�b�N���� ---------- } ---------- */

// 2009.10.09 bagus StagePGM ���ʉ� --{--
int CNanoSpecDoc::GetStagePGM(int nHeadType, int nScanType)
{
	int	nRet = STAGE_PGM_TYPE_STANDARD;
	int	i;

	for(i=0; i<sizeof(s_StagePGMTable)/sizeof(s_StagePGMTable[0]); i++) {
		if ((s_StagePGMTable[i].nHead == nHeadType) && (s_StagePGMTable[i].nScan == nScanType)) {
			nRet = s_StagePGMTable[i].nStagePGM;
			break;
		}
	}

	return nRet;
}

BOOL CNanoSpecDoc::IsValidStagePGM(int nHeadType, int nScanType, int nStagePGMType)
{
	BOOL bRet = FALSE;
	int	i;

	for(i=0; i<sizeof(s_StagePGMTable)/sizeof(s_StagePGMTable[0]); i++) {
		if ((s_StagePGMTable[i].nHead == nHeadType) && (s_StagePGMTable[i].nScan == nScanType) && (s_StagePGMTable[i].nStagePGM == nStagePGMType)) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}
// 2009.10.09 bagus StagePGM ���ʉ� --}--
//2009.11.04 bagus CA --{--
BOOL CNanoSpecDoc::SetOneContactAngleData(CTARESULT *OneCTAResult,int iPoint)
{
	if(iPoint < 1 || iPoint >= SCAN_POINT_MAX){
		return FALSE;
	}
	m_pMeasureddata->ContactAngleData[iPoint-1] = *OneCTAResult;

	return TRUE;
}

BOOL CNanoSpecDoc::GetOneContactAngleData(CTARESULT *OneCTAResult,int iPoint)
{
	if(iPoint < 1 || iPoint >= SCAN_POINT_MAX){
		return FALSE;
	}
	*OneCTAResult = m_pMeasureddata->ContactAngleData[iPoint-1];

	return TRUE;
}
//2009.11.04 bagus CA --}--
//2009.11.10 bagus RS --{--
BOOL CNanoSpecDoc::SetOneResistData(RESISTRESULT *OneRESISTResult,int iPoint)
{
	if(iPoint < 1 || iPoint >= SCAN_POINT_MAX){
		return FALSE;
	}
	m_pMeasureddata->ResistData[iPoint-1] = *OneRESISTResult;

	return TRUE;
}

BOOL CNanoSpecDoc::GetOneResistData(RESISTRESULT *OneRESISTResult,int iPoint)
{
	if(iPoint < 1 || iPoint >= SCAN_POINT_MAX){
		return FALSE;
	}
	*OneRESISTResult = m_pMeasureddata->ResistData[iPoint-1] ;

	return TRUE;
}
//2009.11.10 bagus RS --}--
//2009.11.04 bagus CA --{--
//�蓮���莞�̏���/��n������
//�����ł̓��b�gID/�J�Z�b�gID�⃌�V�s�̓Z�b�g���ꂽ��ŌĂ΂�邱�Ƃ�z�肵�Ă���
BOOL CNanoSpecDoc::PrepareManualCAMeasure()
{
	BOOL bRet;
	char szCstID[LOT_ID_LEN+1];
	char szSampleId[SAMPLE_ID_LEN+1];

	bRet = MEAS_CtaModeSet(MEAS_CTA_MODE_1);
	if(!bRet)	return FALSE;

	GetSampleId(szSampleId);
	GetLotId(szCstID);
	bRet = MEAS_CtaSetCstSample(szCstID,szSampleId,&m_pMeasureddata->rcp_data.MainRcpInfo);
	if(!bRet)	return FALSE;

	bRet = MEAS_CtaSetThick((DWORD)(m_pMeasureddata->rcp_data.StageProgInfoHdr.SampleInfo.dThickness / 100.0));
	if(!bRet)	return FALSE;

	return TRUE;
}
BOOL CNanoSpecDoc::CleanupManualCAMeasure()
{
	BOOL bRet;

	bRet = MEAS_CtaMeasEnd();

	return bRet;
}
//2009.11.04 bagus CA --}--

/* added 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
long CNanoSpecDoc::GetExpDouble(double dData)
{
	double l_dData = dData;
	long l_iLog10 = (long) (log10(fabs(l_dData)));
	long l_iExp = (((long) (l_dData / (pow(10, l_iLog10)))) < 1)?
					l_iLog10 + (-1) :
					l_iLog10;

	return l_iExp;
}
/* added 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
