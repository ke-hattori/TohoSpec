// NanoSpecDoc.cpp : CNanoSpecDoc クラスの動作の定義を行います。
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
/* added 2009.09.08 hmenjo AF 自動レンズ切替対応 ---------- { ---------- */
#include "NSStage.hxx"
#include "NanoSpec.h"
/* added 2009.09.08 hmenjo AF 自動レンズ切替対応 ---------- } ---------- */
/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
#include	<math.h>
/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.10.09 bagus StagePGM 共通化 --{--
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
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	{ HEAD_TYPE_COMPEASE,MEAS_PROG_TYPE_COMPEASE_THICKNESS,					STAGE_PGM_TYPE_STANDARD		},
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
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
// 2009.10.19 bagus MS 追加 --}--
	{ HEAD_TYPE_4PP,	MEAS_PROG_TYPE_4PP_MEAS,							STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_CTA,	MEAS_PROG_TYPE_CTA_MEAS,							STAGE_PGM_TYPE_STANDARD		},
	{ HEAD_TYPE_STRESS,	MEAS_PROG_TYPE_STRESS_MEAS,							STAGE_PGM_TYPE_STRESS		},
};
// 2009.10.09 bagus StagePGM 共通化 --}--


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
// CNanoSpecDoc クラスの構築/消滅
CNanoSpecDoc::CNanoSpecDoc()
{
	//Kojika 20090525 Add
	CString strBuffer;
	//Kojika 20090525 Add End
	char szBuff[100];

	///// ディレクトリを作成 /////
	MakeNanospecDir();

	///// OperationLogFile /////
	m_OperationLogFile.Create(g_szLog_Dir,								// LOG_DIR,
								OPERATION_LOG_NAME,
								OPERATION_LOG_SIZE,
								OPERATION_LOG_MAX);

/* modified 2016.05.12 hmenjo 6500/TS3100 別 version ---------- { ---------- */
//	sprintf(szBuff, "START Nanospec %s (%s %s)", SOFT_VERSION, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 別 version ----------              */
	char l_szVersion[128];
	switch (g_lModelType) {
	case MODEL_T3100:	strcpy(l_szVersion, SOFT_VERSION_TS3100);	break;
	default:			strcpy(l_szVersion, SOFT_VERSION);			break;
	}
	sprintf(szBuff, "START Nanospec %s (%s %s)", l_szVersion, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 別 version ---------- } ---------- */
// 2013.11.15 Bagus Add (TohoSpec対応) -->
	if(g_lAppNameType != APP_NAME_NANO){
		CString strMsg;
		strMsg = szBuff;
		strMsg.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		strcpy(szBuff, strMsg);
	}
// 2013.11.15 Bagus Add (TohoSpec対応) <--
	OperationLogging(szBuff);
// hmenjo ChiefLog -->
	// Chief ログファイルの初期化
	m_ChiefLogFile.Create(	g_szLog_Dir,								// LOG_DIR,
						CHIEF_LOG_NAME,
						CHIEF_LOG_SIZE,
						CHIEF_LOG_MAX);
// hmenjo ChiefLog <--

// 2013.01.10 bagus stage driver alarm io -->
	// StageError ログファイルの初期化
	m_StageErrorLogFile.Create(	g_szLog_Dir,								// LOG_DIR,
						STAGE_ERROR_LOG_NAME,
						STAGE_ERROR_LOG_SIZE,
						STAGE_ERROR_LOG_MAX);
// 2013.01.10 bagus stage driver alarm io <--

	///// NanoSpec.iniのグローバル変数の初期化 /////
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
	m_bJoyStickSelectEnabled	= TRUE;							// Defaultは切替許可
	memset(m_bDoPointMeasFlag, 0, sizeof(m_bDoPointMeasFlag));	// ポイント毎の測定する/しないフラグ

	m_bPatRecFlag = FALSE;
	memset(&m_ActuateFlags, 0, sizeof(ACTUATE_FLAGS));

	///// User /////
	memset(&m_User, 0, sizeof(m_User));

	m_pMeasureddata = (MEASURED_DATA*)VirtualAlloc(NULL, sizeof(MEASURED_DATA), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if ( !m_pMeasureddata ) {
//		AfxMessageBox("MEASURED_DATA メモリ確保失敗");
		//AfxMessageBox("測定データ メモリ確保失敗");
		//Kojika 20090525 Change
		LoadStringML(IDS_MEASURED_DATA_ALLOC_FAILED , strBuffer, "MEASURED_DATA Memory Alloc failed");
		AfxMessageBox(strBuffer);
		//Kojika 20090525 Change End
		return;
	}

	m_iSeqMeasMode = SEQ_NOT_MEASURE;	//測定中でない(測定結果画面のWAIT時含む)
	m_iMeasMode = MEASMODE_NOT_MEASURE; //測定中でない(測定結果画面のWAIT時は含まない)
	m_iManuMeasMode = MANUMODE_NOT; 	//マニュアル測定モードでない

	m_bPauseFlg = FALSE;   //ポーズ判定フラグ(TRUE:ポーズ FALSE:それ以外)

	InitMeasData();

	m_ActualJoyStickFlag = FALSE;
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	m_lZ_AF = 0x80000000;
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	m_bSetSampleIdFromUI = FALSE;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
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
/* modified 2016.05.12 hmenjo 6500/TS3100 別 version ---------- { ---------- */
//	sprintf(szBuff, "END    Nanospec %s (%s %s)", SOFT_VERSION, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 別 version ----------              */
	char l_szVersion[128];
	switch (g_lModelType) {
	case MODEL_T3100:	strcpy(l_szVersion, SOFT_VERSION_TS3100);	break;
	default:			strcpy(l_szVersion, SOFT_VERSION);			break;
	}
	sprintf(szBuff, "END    Nanospec %s (%s %s)", l_szVersion, __DATE__, __TIME__);
/* modified 2016.05.12 hmenjo 6500/TS3100 別 version ---------- } ---------- */
// 2013.11.15 Bagus Add (TohoSpec対応) -->
	if(g_lAppNameType != APP_NAME_NANO){
		CString strMsg;
		strMsg = szBuff;
		strMsg.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		strcpy(szBuff, strMsg);
	}
// 2013.11.15 Bagus Add (TohoSpec対応) <--

	OperationLogging(szBuff);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc シリアライゼーション
void CNanoSpecDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()){
		// TODO: この位置に保存用のコードを追加してください。
	}
	else{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecDoc クラスの診断

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
// CNanoSpecDoc コマンド

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
		FilePointer = file.GetPosition();

		///// check the number of terminator /////
		TerminatorNum = 0;
		FilePointer = file.Seek(-2 ,CFile::current);
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
				FilePointer = file.Seek(-2 ,CFile::current);
				file.Read(&Character, 1);
				Count++;
				if(FilePointer == 0){
					Count = Count - (TerminatorNum - 1);
					FilePointer = file.Seek(-1 ,CFile::current);
					ReadNum = file.Read(szBuff, Count);
					FilePointer = file.Seek(-ReadNum ,CFile::current);
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
					FilePointer = file.Seek(-ReadNum ,CFile::current);
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
	//ユーザが変更されたのでバックアップの設定を更新する
	SYSTEM_CONFIG l_SystemConfig;
	USER_DATA	 l_UserData;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	if(l_SystemConfig.bBackup){
		RecipeFile_SetBackupPath(l_UserData.BackupData.BackupPath,l_UserData.BackupData.bAutoBackup);
		PatternFile_SetBackupPath(l_UserData.BackupData.BackupPath,l_UserData.BackupData.bAutoBackup);
	}else{
		//バックアップ機能が有効になっていない
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
//	m_measureddataの初期化
void CNanoSpecDoc::InitMeasData(void)
{

	memset(m_pMeasureddata, 0, sizeof(MEASURED_DATA));
	m_iLastMeasPoint = 0;
	m_iCurrentSetPoint = 0;
	m_iReMeasNumScans = 0;
	m_iReMeasurePointCnt = 0; //hibino 確認中
	//2009.08.17 bagus stress --{--
	//ストレス関連追加初期化
	m_iLastMeasStressLine = 0;			//ストレス測定の最終測定ストレスライン
	m_iCurrentSetStressLine = 0;		//一番最後にデータを格納したストレスライン
	m_iCurrentMeasStressLine = 0;		//現在測定中のストレスライン
	//2009.08.17 bagus stress --}--

}



/////////////////////////////////////////////////////////////////////////////
//	測定データ格納するもの全体を取得 (datファイル作成時のみ使用)
void CNanoSpecDoc::GetMeasDataAll(MEASURED_DATA* meadat)
{
	*meadat = *m_pMeasureddata;
}


/////////////////////////////////////////////////////////////////////////////
//	測定データ格納するもの全体を格納　【使用不可】
void CNanoSpecDoc::SetMeasDataAll(MEASURED_DATA* meadat)
{
	*m_pMeasureddata = *meadat;
}

/////////////////////////////////////////////////////////////////////////////
//	レシピデータ取得
void CNanoSpecDoc::GetRcpData(RCP_DATA* rcp_data)
{
	*rcp_data = m_pMeasureddata->rcp_data;
}

/////////////////////////////////////////////////////////////////////////////
//	レシピデータ格納
void CNanoSpecDoc::SetRcpData(RCP_DATA* rcp_data)
{
	m_pMeasureddata->rcp_data = *rcp_data;
}

/////////////////////////////////////////////////////////////////////////////
//	1ポイント測定データ・統計データ取得 【非推奨】
//引数: 1ポイント分測定データ, その時点までの統計データ, 何ポイント目の測定分かの指定(1ポイント目＝1)
BOOL CNanoSpecDoc::GetOnePointAndStatistics(ONE_POINT_DATA *OnePointData, STATISTICS *Statistics, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //取得失敗
	}
	*OnePointData = m_pMeasureddata->OnePointData[iPoint - 1];
	memcpy(&Statistics[0], &m_pMeasureddata->Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //取得成功
}

/////////////////////////////////////////////////////////////////////////////
//	1ポイント測定データ・統計データ格納 【非推奨】
//引数: 1ポイント分測定データ, その時点までの統計データ, 何ポイント目の測定分かの指定(1ポイント目＝1)
BOOL CNanoSpecDoc::SetOnePointAndStatistics(ONE_POINT_DATA *OnePointData, STATISTICS *Statistics, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //格納失敗
	}
	m_pMeasureddata->OnePointData[iPoint - 1] = *OnePointData;
	memcpy(&m_pMeasureddata->Statistics[0], &Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //格納成功
}


/////////////////////////////////////////////////////////////////////////////
//	1ポイント測定データ取得 【推奨】
//引数: 1ポイント分測定データ, 何ポイント目の測定分かの指定(1ポイント目＝1)
BOOL CNanoSpecDoc::GetOnePointData(ONE_POINT_DATA *OnePointData, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //取得失敗
	}
	*OnePointData = m_pMeasureddata->OnePointData[iPoint - 1];
	return TRUE; //取得成功
}

/////////////////////////////////////////////////////////////////////////////
//	1ポイント測定データ格納 【推奨】
//引数: 1ポイント分測定データ, 何ポイント目の測定分かの指定(1ポイント目＝1)
BOOL CNanoSpecDoc::SetOnePointData(ONE_POINT_DATA *OnePointData, int iPoint)
{
	if((iPoint > SCAN_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //格納失敗
	}
	m_pMeasureddata->OnePointData[iPoint - 1] = *OnePointData;
	return TRUE; //格納成功
}


/////////////////////////////////////////////////////////////////////////////
//	テストモード1ポイント測定データ格納
//引数: 1ポイント分測定データ, 何ポイント目の測定分かの指定(1ポイント目＝1)
BOOL CNanoSpecDoc::SetTestModeData(TEST_MODE_DATA *TestModeData, int iPoint)
{
	if((iPoint > TESTDATA_POINT_MAX) || (iPoint < 1 ))
	{
		return FALSE; //格納失敗
	}
	m_pMeasureddata->TestModeData[iPoint - 1] = *TestModeData;
	return TRUE; //格納成功
}


/////////////////////////////////////////////////////////////////////////////
//	統計データ取得 【推奨】
//引数: その時点までの統計データ
BOOL CNanoSpecDoc::GetStatisticsData(STATISTICS *Statistics)
{
	memcpy(&Statistics[0], &m_pMeasureddata->Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //取得成功
}

/////////////////////////////////////////////////////////////////////////////
//	統計データ格納 【推奨】
//引数: その時点までの統計データ
BOOL CNanoSpecDoc::SetStatisticsData(STATISTICS *Statistics)
{
	memcpy(&m_pMeasureddata->Statistics[0], &Statistics[0], sizeof(STATISTICS[ADAPRESULT_COLS_MAX]));
	return TRUE; //格納成功
}


/////////////////////////////////////////////////////////////////////////////
//	統計データ有効Point数(統計データに加えるPoint数)取得
int CNanoSpecDoc::GetStatisticsValidPointCount(void)
{
	return m_pMeasureddata->iStatisticsValidPointCount;
}

/////////////////////////////////////////////////////////////////////////////
//	統計データ有効Point数(統計データに加えるPoint数)格納
void CNanoSpecDoc::SetStatisticsValidPointCount(int iStatisticsValidPointCount)
{
	m_pMeasureddata->iStatisticsValidPointCount = iStatisticsValidPointCount;
}


// 2009.09.30 K.Matsuo Delete -->
/////////////////////////////////////////////////////////////////////////////
//	反射率データの取得（回数、波長380～800）　//格納はSetOnePointDataで。
//double CNanoSpecDoc::NanoSpecDoc_GetAnalysisData(int iCnt, int iWave)
//{
//	return m_pMeasureddata->OnePointData[iCnt - 1].dAnalysisData[iWave - 1];
//}
//
/////////////////////////////////////////////////////////////////////////////
//	XmpからのGenDataの取得（回数、波長380～800）　//格納はSetOnePointDataで。
//double CNanoSpecDoc::NanoSpecDoc_GetAnalysisGenData(int iCnt, int iWave)
//{
//	return m_pMeasureddata->OnePointData[iCnt - 1].dAnalysisGenData[iWave - 1];
//}
// 2009.09.30 K.Matsuo Delete <--

// 2014.04.03 bagus wavelength step modified -->
// /////////////////////////////////////////////////////////////////////////////
// //	サンプルデータの取得(テストモード専用)
// //（回数、波長380～800）　//格納はSetTestModeDataで。
// double CNanoSpecDoc::NanoSpecDoc_GetScanSampleData(int iCnt, int iWave)
// {
// 	return m_pMeasureddata->TestModeData[iCnt - 1].dScanSampleData[iWave - 1];
// }
//
//
// /////////////////////////////////////////////////////////////////////////////
// //	リファレンスの取得(テストモード専用)
// //（回数、波長380～800）　//格納はSetTestModeDataで。
// double CNanoSpecDoc::NanoSpecDoc_GetScan1stReferenceData(int iCnt, int iWave)
// {
// 	return m_pMeasureddata->TestModeData[iCnt - 1].dScan1stReferenceData[iWave - 1];
// }
//
//
// /////////////////////////////////////////////////////////////////////////////
// //	リファレンスの取得(テストモード専用)
// //（回数、波長380～800）　//格納はSetTestModeDataで。
// double CNanoSpecDoc::NanoSpecDoc_GetScanDark_T1Data(int iCnt, int iWave)
// {
// 	return m_pMeasureddata->TestModeData[iCnt - 1].dScanDark_T1Data[iWave - 1];
// }
/////////////////////////////////////////////////////////////////////////////
//	サンプルデータの取得(テストモード専用)
//（回数、波長380～800）　//格納はSetTestModeDataで。
double CNanoSpecDoc::NanoSpecDoc_GetScanSampleData(int iCnt, double dWave)
{
	return m_pMeasureddata->TestModeData[iCnt - 1].GetScanSampleData(dWave);
}


/////////////////////////////////////////////////////////////////////////////
//	リファレンスの取得(テストモード専用)
//（回数、波長380～800）　//格納はSetTestModeDataで。
double CNanoSpecDoc::NanoSpecDoc_GetScan1stReferenceData(int iCnt, double dWave)
{
	return m_pMeasureddata->TestModeData[iCnt - 1].GetScan1stReferenceData(dWave);
}


/////////////////////////////////////////////////////////////////////////////
//	リファレンスの取得(テストモード専用)
//（回数、波長380～800）　//格納はSetTestModeDataで。
double CNanoSpecDoc::NanoSpecDoc_GetScanDark_T1Data(int iCnt, double dWave)
{
	return m_pMeasureddata->TestModeData[iCnt - 1].GetScanDark_T1Data(dWave);
}
// 2014.04.03 bagus wavelength step modified <--


/////////////////////////////////////////////////////////////////////////////
//	テストモード検知条件取得   //格納はSetTestModeDataで。
void CNanoSpecDoc::GetMeasRcpCon(int iCnt, TEST_MODE_MEASRCP_CONDITION* MeasRcpCon)
{
	*MeasRcpCon = m_pMeasureddata->TestModeData[iCnt - 1].MeasRcpCon;
}


/////////////////////////////////////////////////////////////////////////////
//	テストモードデータ取得	 //格納はSetTestModeDataで。
void CNanoSpecDoc::GetTestModeData(TEST_MODE_DATA* TestModeData, int iCnt)
{
	*TestModeData = m_pMeasureddata->TestModeData[iCnt - 1];
}

/////////////////////////////////////////////////////////////////////////////
//	dData(ONE_POINT_DATA内)の取得 　//格納はSetOnePointDataで。
void CNanoSpecDoc::GetOnepointDdata(int iPoint, double* dData)
{
	memcpy(dData, m_pMeasureddata->OnePointData[iPoint - 1].dData, sizeof(m_pMeasureddata->OnePointData[iPoint - 1].dData) );

}


/////////////////////////////////////////////////////////////////////////////
//	ラベル名など取得(『X,Y,Z』,『AF』,『Thick～』,『Thick～の個数』)
void CNanoSpecDoc::GetLabel(char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1],  //ラベル(X,Y,Z)
							char szTitleAf[AF_HEADER_LEN + 1],		 //ラベル(AF)
							char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], //ラベル(Thick～)
							int *iLabelCount)			  //『ラベル(Thick～)』の格納個数
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
//	ラベル名など格納(『X,Y,Z』,『AF』,『Thick～』,『Thick～の個数』)
void CNanoSpecDoc::SetLabel(char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1],  //ラベル(X,Y,Z)
							char szTitleAf[AF_HEADER_LEN + 1],		 //ラベル(AF)
							char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], //ラベル(Thick～)
							char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], //DDE受信のラベル(Thick～)
							int iLabelCount)			 //『ラベル(Thick～)』の格納個数
{
	memcpy(m_pMeasureddata->szTitleXyz, szTitleXyz, sizeof(m_pMeasureddata->szTitleXyz));
	memcpy(m_pMeasureddata->szTitleAf, szTitleAf, sizeof(m_pMeasureddata->szTitleAf));
	memcpy(m_pMeasureddata->szLabel, szLabel, sizeof(m_pMeasureddata->szLabel));
	memcpy(m_pMeasureddata->szLabelOri, szLabelOri, sizeof(m_pMeasureddata->szLabelOri));
	m_pMeasureddata->iLabelCount = iLabelCount;
}


/////////////////////////////////////////////////////////////////////////////
//	測定結果表示(保存)用ラベル名の取得(『Thick～』)
void CNanoSpecDoc::GetDispLabel(char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1])
{
	memcpy(szDispLabel, m_pMeasureddata->szDispLabel, sizeof(m_pMeasureddata->szDispLabel));
}

/////////////////////////////////////////////////////////////////////////////
//	測定結果表示(保存)用ラベル名の格納(『Thick～』)
void CNanoSpecDoc::SetDispLabel(char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1])
{
	memcpy(m_pMeasureddata->szDispLabel, szDispLabel, sizeof(m_pMeasureddata->szDispLabel));
}

/////////////////////////////////////////////////////////////////////////////
//	測定結果表示(保存)用ラベル名の格納数取得(『Thick～』)
int CNanoSpecDoc::GetDispLabelCount(void)
{
	return m_pMeasureddata->iDispLabelCount;
}

/////////////////////////////////////////////////////////////////////////////
//	測定結果表示(保存)用ラベル名の格納数格納(『Thick～』)
void CNanoSpecDoc::SetDispLabelCount(int iDispLabelCount)
{
	m_pMeasureddata->iDispLabelCount = iDispLabelCount;
}


/////////////////////////////////////////////////////////////////////////////
//	格納済件数の取得 (注：何ポイント測定したかの数)
int CNanoSpecDoc::GetStoreCount(void)
{
	return m_pMeasureddata->iSetPointCount;
}

/////////////////////////////////////////////////////////////////////////////
//	格納済件数の格納 (注：何ポイント測定したかの数)
void CNanoSpecDoc::SetStoreCount(int iSetPointCount)
{
	m_pMeasureddata->iSetPointCount = iSetPointCount;
}

/////////////////////////////////////////////////////////////////////////////
//	測定ポイント数の取得 (注：何ポイント測定するかの数)
int CNanoSpecDoc::GetMeasurePointCount(void)
{
	return m_pMeasureddata->iMeasureNum;
}

/////////////////////////////////////////////////////////////////////////////
//	測定ポイント数の格納 (注：何ポイント測定するかの数) ←●追加分の測定時にインクリメントすること！
void CNanoSpecDoc::SetMeasurePointCount(int iMeasureNum)
{
	m_pMeasureddata->iMeasureNum = iMeasureNum;
}

/////////////////////////////////////////////////////////////////////////////
//	ユーザIDの取得
void CNanoSpecDoc::GetUserId(char szUserId[MAX_PATH + 1])
{
	memcpy(szUserId, m_pMeasureddata->szUserId, sizeof(m_pMeasureddata->szUserId));
}

/////////////////////////////////////////////////////////////////////////////
//	ユーザIDの格納
void CNanoSpecDoc::SetUserId(char szUserId[MAX_PATH + 1])
{
	memcpy(m_pMeasureddata->szUserId, szUserId, sizeof(m_pMeasureddata->szUserId));
}



/////////////////////////////////////////////////////////////////////////////
//	測定開始日時の取得
void CNanoSpecDoc::GetStartDateTime(SYSTEMTIME* syStartDateTime)
{
	*syStartDateTime = m_pMeasureddata->syStartDateTime;
}

/////////////////////////////////////////////////////////////////////////////
//	測定開始日時の格納
void CNanoSpecDoc::SetStartDateTime(SYSTEMTIME* syStartDateTime)
{
	m_pMeasureddata->syStartDateTime = *syStartDateTime;
}


/////////////////////////////////////////////////////////////////////////////
//	測定終了日時の取得
void CNanoSpecDoc::GetEndDateTime(SYSTEMTIME* syEndDateTime)
{
	*syEndDateTime = m_pMeasureddata->syEndDateTime;
}

/////////////////////////////////////////////////////////////////////////////
//	測定終了日時の格納
void CNanoSpecDoc::SetEndDateTime(SYSTEMTIME* syEndDateTime)
{
	m_pMeasureddata->syEndDateTime = *syEndDateTime;
}


/////////////////////////////////////////////////////////////////////////////
//	サンプルIDの取得
void CNanoSpecDoc::GetSampleId(char szSampleId[SAMPLE_ID_LEN + 1])
{
	memcpy(szSampleId, m_pMeasureddata->szSampleId, sizeof(m_pMeasureddata->szSampleId));
}

/////////////////////////////////////////////////////////////////////////////
//	サンプルIDの格納
void CNanoSpecDoc::SetSampleId(char szSampleId[SAMPLE_ID_LEN + 1])
{
	memcpy(m_pMeasureddata->szSampleId, szSampleId, sizeof(m_pMeasureddata->szSampleId));
}

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
/////////////////////////////////////////////////////////////////////////////
//	サンプルIDダイアログから入力設定された場合、TRUE
void CNanoSpecDoc::SetSampleIdFromUI(BOOL bSet)
{
	m_bSetSampleIdFromUI = bSet;
}

/////////////////////////////////////////////////////////////////////////////
//	サンプルIDダイアログから入力設定されたかどうか
BOOL CNanoSpecDoc::IsSetSampleIdFromUI()
{
	return m_bSetSampleIdFromUI;
}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

//2009.10.13 bagus RS --{--
//ロットID追加
/////////////////////////////////////////////////////////////////////////////
//	ロットIDの取得
void CNanoSpecDoc::GetLotId(char szLotId[LOT_ID_LEN + 1])
{
	memcpy(szLotId, m_pMeasureddata->szLotId, sizeof(m_pMeasureddata->szLotId));
}

/////////////////////////////////////////////////////////////////////////////
//	ロットIDの格納
void CNanoSpecDoc::SetLotId(char szLotId[LOT_ID_LEN + 1])
{
	memcpy(m_pMeasureddata->szLotId, szLotId, sizeof(m_pMeasureddata->szLotId));
}
//2009.10.13 bagus RS --}--

/////////////////////////////////////////////////////////////////////////////
//	測定結果単位名の取得
void CNanoSpecDoc::GetUnitName(char szUnit[DISP_UNIT_NAME_LEN + 1])
{
	memcpy(szUnit, m_pMeasureddata->szUnit, sizeof(m_pMeasureddata->szUnit));
}

/////////////////////////////////////////////////////////////////////////////
//	測定結果単位名の格納
void CNanoSpecDoc::SetUnitName(char szUnit[DISP_UNIT_NAME_LEN + 1])
{
	memcpy(m_pMeasureddata->szUnit, szUnit, sizeof(m_pMeasureddata->szUnit));
}


/////////////////////////////////////////////////////////////////////////////
//	測定結果単位indexの取得
int CNanoSpecDoc::GetUnitIndex(void)
{
	return m_pMeasureddata->iUnit;
}

/////////////////////////////////////////////////////////////////////////////
//	測定結果単位indexの格納
void CNanoSpecDoc::SetUnitIndex(int iUnit)
{
	m_pMeasureddata->iUnit = iUnit;
}



/////////////////////////////////////////////////////////////////////////////
//	 //画面に表示済みのポイント数の取得(MeasurementListDlg用)
int CNanoSpecDoc::GetShowCompPointList(void)
{
	return m_pMeasureddata->iShowCompletionPointList;
}

/////////////////////////////////////////////////////////////////////////////
//	 //画面に表示済みのポイント数の格納(MeasurementListDlg用)
void CNanoSpecDoc::SetShowCompPointList(int iShowCompletionPointList)
{
	m_pMeasureddata->iShowCompletionPointList = iShowCompletionPointList;
}

/////////////////////////////////////////////////////////////////////////////
//	 //画面に表示済みのポイント数の取得(MeasurementGraphDlg用)
int CNanoSpecDoc::GetShowCompPointGraph(void)
{
	return m_pMeasureddata->iShowCompletionPointGraph;
}

/////////////////////////////////////////////////////////////////////////////
//	 //画面に表示済みのポイント数の格納(MeasurementGraphDlg用)
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
//予定最終測定点取得
//通常測定時:測定ポイント数
//リメジャーモード時:再測定ポイントの一番大きい点
//マニュアル測定時,テストモード時:測定追加時に随時インクリメント(←未作成 hibino)
int CNanoSpecDoc::GetLastMeasPoint(void)
{
	return m_iLastMeasPoint;
}

/////////////////////////////////////////////////////////////////////////////
//予定最終測定点格納
//通常測定時:測定ポイント数
//リメジャーモード時:再測定ポイントの一番大きい点
//マニュアル測定時,テストモード時:測定追加時に随時インクリメント(←未作成 hibino)
void CNanoSpecDoc::SetLastMeasPoint(int iLastMeasPoint)
{
	m_iLastMeasPoint = iLastMeasPoint;
}


/////////////////////////////////////////////////////////////////////////////
//一番最後にデータを格納(書換え)したポイント番号の取得
//(通常測定時はGetStoreCount()で得られる値と同じ)
int CNanoSpecDoc::GetCurrentSetPoint(void)
{
	return m_iCurrentSetPoint;
}

/////////////////////////////////////////////////////////////////////////////
//一番最後にデータを格納(書換え)したポイント番号の格納
//(通常測定時はGetStoreCount()で得られる値と同じ)
void CNanoSpecDoc::SetCurrentSetPoint(int iCurrentSetPoint)
{
	m_iCurrentSetPoint = iCurrentSetPoint;
}


/////////////////////////////////////////////////////////////////////////////
//シーケンス測定モードの測定状態取得(シーケンス測定中のみ使用可(リメジャー時含む))
//(測定中でない/通常測定中/リメジャー中)
int CNanoSpecDoc::GetSeqMeasMode(void)
{
	return m_iSeqMeasMode;
}

/////////////////////////////////////////////////////////////////////////////
//シーケンス測定モードの測定状態格納(シーケンス測定中のみ使用可(リメジャー時含む))
//(測定中でない/通常測定中/リメジャー中)
void CNanoSpecDoc::SetSeqMeasMode(int iSeqMeasMode)
{
	m_iSeqMeasMode = iSeqMeasMode;
}


/////////////////////////////////////////////////////////////////////////////
//測定モード取得
//(測定中でない/通常測定中orリメジャー中/マニュアル測定中/テストモード中)
int CNanoSpecDoc::GetMeasMode(void)
{
	return m_iMeasMode;
}

/////////////////////////////////////////////////////////////////////////////
//測定モード格納
//(測定中でない/通常測定中orリメジャー中/マニュアル測定中/テストモード中)
void CNanoSpecDoc::SetMeasMode(int iMeasMode)
{
	m_iMeasMode = iMeasMode;
}


/////////////////////////////////////////////////////////////////////////////
//マニュアル測定モードの状態を取得(注：マニュアル測定モード中以外は使用しない)
int CNanoSpecDoc::GetManuMeasMode(void)
{
	return m_iManuMeasMode;
}

/////////////////////////////////////////////////////////////////////////////
//マニュアル測定モードの状態を格納(注：マニュアル測定モード中以外は使用しない)
void CNanoSpecDoc::SetManuMeasMode(int iManuMeasMode)
{
	m_iManuMeasMode = iManuMeasMode;
}


/////////////////////////////////////////////////////////////////////////////
//リメジャー時の測定ポイント数取得(注：何ポイント測定するかの数)
int CNanoSpecDoc::GetReMeasNumScans(void)
{
	return m_iReMeasNumScans;
}

/////////////////////////////////////////////////////////////////////////////
//リメジャー時の測定ポイント数格納(注：何ポイント測定するかの数)
void CNanoSpecDoc::SetReMeasNumScans(int iReMeasNumScans)
{
	m_iReMeasNumScans = iReMeasNumScans;
}


/////////////////////////////////////////////////////////////////////////////
//リメジャー時の測定済ポイント数取得(注：何ポイント測定したかの数)
int CNanoSpecDoc::GetReMeasurePointCnt(void)
{
	return m_iReMeasurePointCnt;
}

/////////////////////////////////////////////////////////////////////////////
//リメジャー時の測定済ポイント数格納(注：何ポイント測定したかの数)
void CNanoSpecDoc::SetReMeasurePointCnt(int iReMeasurePointCnt)
{
	m_iReMeasurePointCnt = iReMeasurePointCnt;
}


/////////////////////////////////////////////////////////////////////////////
//ポーズ状態フラグ取得
BOOL CNanoSpecDoc::GetPauseFlg(void)
{
	return m_bPauseFlg;
}

/////////////////////////////////////////////////////////////////////////////
//ポーズ状態フラグ格納
void CNanoSpecDoc::SetPauseFlg(BOOL bPauseFlg)
{
	m_bPauseFlg = bPauseFlg;
}



/////////////////////////////////////////////////////////////////////////////
//	PatRec 動作中フラグをセットします
void CNanoSpecDoc::SetPatRecFlag(BOOL bFlag)
{
	if (0 != bFlag) {
		::InterlockedExchange((LPLONG) &m_bPatRecFlag, (LONG) TRUE);
	} else {
		::InterlockedExchange((LPLONG) &m_bPatRecFlag, (LONG) FALSE);
	}

}

/////////////////////////////////////////////////////////////////////////////
//	PatRec 動作中フラグを取得します
BOOL CNanoSpecDoc::GetPatRecFlag()
{
	BOOL	l_bRc;

	/*	m_bPatRecFlag は，「0」か「1」の値にしかなりませんので
	 *	「99」を比較対象値とすることで書込みが行われないようにしています．	*/
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_bPatRecFlag, (LONG)99, (LONG)99)) {
		l_bRc = TRUE;
	} else {
		l_bRc = FALSE;
	}

	return l_bRc;
}

/*
 *	動作中フラグ群アクセス関数群
 */
/////////////////////////////////////////////////////////////////////////////
//	動作中フラグをセットします
void CNanoSpecDoc::ActuateFlagsSet(
		ACTUATE_FLAGS_SEL ActFgSel, 	// 識別定数(enum ACTUATE_FLAGS_SEL)
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
//	動作中フラグを取得します
BOOL CNanoSpecDoc::ActuateFlagsGet(ACTUATE_FLAGS_SEL ActFgSel)
{
	BOOL	l_bRc = TRUE;	// 未定義の場合，動かない方向にするため TRUE にしてあります．

#if 0
	BOOL*	l_pbActuateFlag = (BOOL*) &m_ActuateFlags;
	if (ActFgSel < ACTUATE_MAX) {
		if (0 != ::InterlockedCompareExchange((PVOID*) (l_pbActuateFlag + ActFgSel), (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
	}
#else
	switch (ActFgSel) {
	case ACTUATE_XYSTAGE:
		/*	フラグは，「0」か「1」の値にしかなりませんので
		 *	「99」を比較対象値とすることで書込みが行われないようにしています．	*/
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bXYStage, (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_ZAXIS:
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bZAxis, (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_PIN:
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bPin, (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_SHUTTER:
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bShutter, (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_WORKGUIDE:
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bWorkGuide, (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
	case ACTUATE_TURRET:
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bTurret, (PVOID) 99, (PVOID) 99)) {
			l_bRc = TRUE;
		} else {
			l_bRc = FALSE;
		}
		break;
// 2009.11.10 bagus MS --{--
	case ACTUATE_MICROSCOPE:
		if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bMS, (PVOID) 99, (PVOID) 99)) {
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
//	動作中フラグを取得します
//		どれか１つでも TRUE なら TRUE で返します．

DWORD CNanoSpecDoc::ActuateFlagsGetAll()
{
	DWORD	l_dwRc = 0x00000000;

#if 0
	l_bRc = FALSE;
	BOOL*	l_pbActuateFlag = (BOOL*) &m_ActuateFlags;
	for (int i = 0; i < ACTUATE_MAX; i++) {
		if (0 != ::InterlockedCompareExchange((PVOID*) (l_pbActuateFlag + i), (PVOID) 99, (PVOID) 99)) {
			l_dwRc = TRUE;
			break;
		}
	}
#else
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bXYStage, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_XYSTAGE;
	}
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bZAxis, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_ZAXIS;
	}
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bPin, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_PIN;
	}
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bShutter, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_SHUTTER;
	}
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bWorkGuide, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_WORKGUIDE;
	}
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bTurret, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_TURRET;
	}
// 2009.11.10 bagus MS --{--
	if (0 != ::InterlockedCompareExchange((LONG volatile*) &m_ActuateFlags.bMS, (PVOID) 99, (PVOID) 99)) {
		l_dwRc |= ACTUATE_MICROSCOPE;
	}
// 2009.11.10 bagus MS --}--
#endif

	return l_dwRc;
}

/////////////////////////////////////////////////////////////////////////////
//	ステージの移動速度制限
void CNanoSpecDoc::StageSpeedLimiter(
		BOOL bLimiter	// TRUE ：速度制限します
						// FALSE：速度制限を解除します
	)
{
	if (0 == bLimiter) {
		// 制限を解除します ---------------------------------------------------
		// 速度制限モードをリセット
		StageSetLocalSpeed(1, 1);
	} else {
		// 制限します ---------------------------------------------------------
		// 速度制限モードをセット
		double l_dSharpMainteSpdX = 200.0;	// X 軸：シャープのメンテ速度
		double l_dSharpMainteSpdY = 200.0;	// Y 軸：シャープのメンテ速度
		double l_dMaxSpdX, l_dMaxSpdY;
		StageGetMaxStageSpeed(&l_dMaxSpdX, &l_dMaxSpdY);	// Stage.dll から最大速度を読出し
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
// ジョイスティックの切替を排他的に処理する
// ==========================================================================
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- { -------- */
//BOOL CNanoSpecDoc::ChangeJoyStick(BOOL flag)
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- 		   */
BOOL CNanoSpecDoc::ChangeJoyStick(BOOL flag, int iSpeedSel)
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- } -------- */
{
BOOL bJoyStk = FALSE;

	if( m_ActualJoyStickFlag == FALSE ){
		m_ActualJoyStickFlag = TRUE;

		CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
		if(!m_pMainFrame->HardwareSimulation()){
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- { -------- */
//			bJoyStk = StageEnableJoyStick(flag);
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- 		   */
			SYSTEM_CONFIG l_SystemConfig;
			ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
			if (STAGE_TYPE_SPT == l_SystemConfig.nStageType) {
				/* SPT ステージの場合 */
				bJoyStk = StageEnableJoyStickSPT(flag, iSpeedSel);
			} else {
				/* TOHO，NTN ステージの場合 */
				bJoyStk = StageEnableJoyStick(flag);
			}
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- } -------- */
// DEL 2009.06.01
//			if( !bJoyStk ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
		}
		else{
			bJoyStk = TRUE;
		}
// 2009.08.07 K.Matsuo Bug Fix SPTステージ減速停止中にJOYSTICK変更 -->
//		ActuateFlagsSet(ACTUATE_XYSTAGE, flag);
		if ( bJoyStk )
			ActuateFlagsSet(ACTUATE_XYSTAGE, flag);
// 2009.08.07 K.Matsuo Bug Fix SPTステージ減速停止中にJOYSTICK変更 <--
		m_ActualJoyStickFlag = FALSE;
	}
	return bJoyStk;
}
//Saiki 20090608 Delete ----->
// ==========================================================================
// 警告メッセージを表示する
// ==========================================================================
//void CNanoSpecDoc::MessageIf_Set(int m_MsgID)
//{
//
//	  CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
//	  m_pMainFrame->PostMessage(WM_MESSAGE_SHOW, m_MsgID, 0);
//}
//Saiki 20090608 Delete <-----

/* ==========================================================================
   各I/Oをチェックする
   ========================================================================*/
BOOL CNanoSpecDoc::IsInterLock()
{
	if( nexioIsEmergencyStop()		!= ON ) {AlarmIf_Set(ALID_EMOSwitchOn); return TRUE;} /* EMOスイッチ・オン */
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- { -------- */
//	if( nexioIsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* ドアオープン */
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- 		   */
	if( Rap_IsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* ドアオープン */
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- } -------- */
	if( nexioIsEquipmentPower() 	!= ON ) {AlarmIf_Set(ALID_EquipmentPowerOff); return TRUE;} /* 装置電源・オフ */
/* added 2009.10.30 hmenjo CTA CTAILPI 動作前チェック ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_CTA)) {
		/* CTA の場合のみ	*/
		if (0 == nexioIsCA_Interlock()) {AlarmIf_Set(ALID_CTA_IL_PI_On); return TRUE;}	/* CTAILPI オフ	*/
	}
/* added 2009.10.30 hmenjo CTA CTAILPI 動作前チェック ---------- } ---------- */

/* added 2009.11.06 K.Matsuo Resist 動作前チェック ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_4PP)) {
		/* Resist の場合のみ	*/
		BOOL l_bUpper = nexioIsRS_ProbeHeadUpperPos();
		BOOL l_bLower = nexioIsRS_ProbeHeadLowerPos();
		if (0 != l_bUpper && 0 == l_bLower) {
			;
		} else if (0 == l_bUpper && 0 != l_bLower) {
			AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL オフ検出	*/
			return TRUE;
		} else {
			AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL 異常検出	*/
			return TRUE;
		}
	}
/* added 2009.11.06 K.Matsuo Resist 動作前チェック ---------- } ---------- */

// 2009.11.03 bagus MS --{--
	if(0 != IsValidHeadType(HEAD_TYPE_MS)){
		//if(0 != !nexioIsMS_LowerPos2()){ AlarmIf_Set(ALID_MS_IL_POS_ERROR); return TRUE; }
		if(1 != nexioIsMS_LowerPos2()){ AlarmIf_Set(ALID_MS_IL_POS_ERROR); return TRUE; }
	}
// 2009.11.03 bagus MS --}--
	return FALSE;
}
/* ==========================================================================
   各I/Oをチェックする
   ========================================================================*/
BOOL CNanoSpecDoc::IsInterLockForMScope()
{
	if( nexioIsEmergencyStop()		!= ON ) {AlarmIf_Set(ALID_EMOSwitchOn); return TRUE;} /* EMOスイッチ・オン */
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- { -------- */
//	if( nexioIsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* ドアオープン */
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- 		   */
	if( Rap_IsDoorInterlock()		!= ON ) {AlarmIf_Set(ALID_DoorOpen); return TRUE;} /* ドアオープン */
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- } -------- */
	if( nexioIsEquipmentPower() 	!= ON ) {AlarmIf_Set(ALID_EquipmentPowerOff); return TRUE;} /* 装置電源・オフ */
/* added 2009.10.30 hmenjo CTA CTAILPI 動作前チェック ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_CTA)) {
		/* CTA の場合のみ	*/
		if (0 == nexioIsCA_Interlock()) {AlarmIf_Set(ALID_CTA_IL_PI_On); return TRUE;}	/* CTAILPI オフ	*/
	}
/* added 2009.10.30 hmenjo CTA CTAILPI 動作前チェック ---------- } ---------- */

/* added 2009.11.06 K.Matsuo Resist 動作前チェック ---------- { ---------- */
	if (0 != IsValidHeadType(HEAD_TYPE_4PP)) {
		/* Resist の場合のみ	*/
		BOOL l_bUpper = nexioIsRS_ProbeHeadUpperPos();
		BOOL l_bLower = nexioIsRS_ProbeHeadLowerPos();
		if (0 != l_bUpper && 0 == l_bLower) {
			;
		} else if (0 == l_bUpper && 0 != l_bLower) {
			AlarmIf_Set(ALID_ResistUpperPositionSensorOff);		/* Resist IL オフ検出	*/
			return TRUE;
		} else {
			AlarmIf_Set(ALID_ResistUpperPositionSensorError);	/* Resist IL 異常検出	*/
			return TRUE;
		}
	}
/* added 2009.11.06 K.Matsuo Resist 動作前チェック ---------- } ---------- */

	return FALSE;
}


/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ---------- { ---------- */
//BOOL CNanoSpecDoc::CheckUnitStatus()
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ----------			   */
BOOL CNanoSpecDoc::CheckUnitStatus(BOOL bEnablePinAlign/* = FALSE*/)
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ---------- } ---------- */
{
	//Saiki 20090530 Change ----->
	CString strMsg;
	if( nexioIsShutterClose()		!= ON ) {
		LoadStringML(IDS_LOADING_SHUTTER_OPEN, strMsg, "Sample Loading Shutter is opened.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ローディングシャッター閉端外れ
	if( nexioIsRobotArmDetect() 	!= ON ) {
		LoadStringML(IDS_DETECT_ROBOT_ARM, strMsg, "Robot Arm is detected.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ロボットアーム検出
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ---------- { ---------- */
//	if( nexioIsPinDownPos() 		!= ON ) {
//		  LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
//		MessageStringIf_Set(strMsg);
//		return TRUE;
//	}	// ピン下端外れ
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ----------			   */
	/* ピン下降端(Align位置)外れ チェック*/
	if (TRUE != bEnablePinAlign) {
		if (nexioIsPinDownPos() != ON) {
			LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
			MessageStringIf_Set(strMsg);
			return TRUE;
		}	// ピン下端外れ
	} else {
		/* Align 位置も含めます．	*/
		if (
			(ON == nexioIsPinUpperPos())
		 || ((ON == nexioIsPinDownPos()) && (ON == nexioIsPinAlignmentPos()))
			) {
			LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
			MessageStringIf_Set(strMsg);
			return TRUE;
		}
	}
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ---------- } ---------- */
	if( nexioIsWorkGuideClose() 	!= OFF) {
		LoadStringML(IDS_CLAMP_NOT_CLOSED, strMsg, "Work Guide is not Close Position.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ワークガイドClose位置
	if( nexioIsWorkGuideOpen()		!= ON ) {
		LoadStringML(IDS_CLAMP_NOT_OPEND, strMsg, "Work Guide is not Open Position.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	}	// ワークガイドOpen位置

	//Saiki 20090530 Change <-----


	return FALSE;
}

/* ==========================================================================
   各Active Flagをチェックする
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
	} //Ｚ軸が動作中です
	if(ActuateFlagsGet(ACTUATE_PIN))		{
		LoadStringML(IDS_LIFTER_WORKING, strMsg, "Sample Lifter is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //リフターが動作中です
	if(ActuateFlagsGet(ACTUATE_WORKGUIDE))	{
		LoadStringML(IDS_CLAMP_WORKING, strMsg, "Work Guide is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //ワークガイドが動作中です
	if(ActuateFlagsGet(ACTUATE_TURRET)) 	{
		LoadStringML(IDS_TURRET_WORKING, strMsg, "Turret is moving.");
		MessageStringIf_Set(strMsg);
		return TRUE;
	} //ターレットが動作中です
	//Saiki 20090530 Change <-----
	return FALSE;
}

/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
BOOL CNanoSpecDoc::Rap_IsAirPressureLowON(BOOL bIgnore/* = 0*/)
{
	BOOL l_bRc = FALSE;

//	BOOL l_bActiveHigh = TRUE;		/* アクティブ High */
	BOOL l_bActiveHigh = FALSE;		/* アクティブ Low */

	/* Chief 以外では無視する仕様になった(by川島20090521)ため
	   引数の bIgnore を見ています(デフォルトは無視しません)．
	   無視しないなら，この if ブロックを削除してください． */
	if (1 == bIgnore) {
		return FALSE;
	}

	/* エア圧力低下の無視フラグ確認(ini ファイルなどから) */
	if (0) {	// 必要であれば，ここに無視フラグの判定を入れてください．
		return FALSE;
	}

	if (0 == nexioIsAirPressureLevelLow()) {
		/* Low レベルでした． */
		l_bRc = (TRUE == l_bActiveHigh)? FALSE : TRUE;
	} else {
		/* High レベルでした． */
		l_bRc = (TRUE == l_bActiveHigh)? TRUE : FALSE;
	}

	return l_bRc;
}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */

/* added hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
BOOL CNanoSpecDoc::Rap_IsDoorInterlock()
{
	BOOL l_bRc;

	if ((0 == nexioIsMaintenanceSwitch()) && (FALSE == g_bIL_DoorOpen)) {
		// メンテナンスモードで，検出禁止設定の場合は，ドア閉で返します．
		l_bRc = TRUE;
	} else {
		l_bRc = nexioIsDoorInterlock();
	}

	return l_bRc;
}
/* added hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
//Saiki 20090530 Add ----->
void CNanoSpecDoc::MessageStringIf_Set(CString strBuffer)
{

	CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pMainFrame->MessageShow(strBuffer);
}
//Saiki 20090530 Add <-----

// ==========================================================================
// ハードJoyを一時的に禁止にする
BOOL CNanoSpecDoc::JoyStickChangeDisable()
{
// 2013.12.16 Bagus Add (TohoSpec対応) -->
	BOOL bRet = FALSE;
// 2013.12.16 Bagus Add (TohoSpec対応) <--

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// JoyStickを切替禁止にする
	m_pDoc->SetJoyStickSelectEnabled(FALSE);

// 2014.01.17 Bagus Mod (Stage None対応) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

// 2013.12.16 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.12.16 Bagus Add (TohoSpec対応) <--
	if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		// ハードJoyStickが選択されていたら
		if( m_pMainFrame->GetJoyStickMode() == JOY_HARD_MODE ){
			// JoyStickを一時的に禁止にする（但し、内部状態は変更しない）
			if( !m_pDoc->ChangeJoyStick(FALSE) ) return FALSE;
				Sleep(300);
		}
// 2013.12.16 Bagus Delete (TohoSpec対応) -->
//		BOOL bRet = FALSE;
// 2013.12.16 Bagus Delte (TohoSpec対応) <--
		if(!g_bHWS){
			bRet = StageGetJoyStickSPT();
		}
// 2013.12.16 Bagus Add (TohoSpec対応) -->
	}
// 2013.12.16 Bagus Add (TohoSpec対応) <--

	return bRet == FALSE ? bRet = TRUE : bRet = FALSE;
}

// ==========================================================================
// ハードJoyの状態を元に戻す
BOOL CNanoSpecDoc::JoyStickStatusRestore()
{
	BOOL bRet = TRUE;
	BOOL bJoyStk;

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();

// 2014.01.17 Bagus Mod (Stage None対応) -->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

// 2013.12.16 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.12.16 Bagus Add (TohoSpec対応) <--
	if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		// JoyStickのモードを戻す
		if( m_pMainFrame->GetJoyStickMode() == JOY_HARD_MODE ){
			// ハードJoyStickが選択されていたら
			if(!g_bHWS){
				if( !StageGetJoyStickSPT() ){
					bJoyStk = ChangeJoyStick(TRUE);		// JoyStickを許可にする
					Sleep(300);
					if( !StageGetJoyStickSPT() ){
						bJoyStk = ChangeJoyStick(TRUE);		// JoyStickを許可にする
						Sleep(300);
						if( !StageGetJoyStickSPT() ){
							bRet = FALSE;
						}
					}
				}
			}
			else{
				bJoyStk = ChangeJoyStick(TRUE);		// JoyStickを許可にする
			}
		}
		else{
			// ソフトJoyStickが選択されていたら
			BOOL bJoyStk = ChangeJoyStick(FALSE);		// JoyStickを禁止にする
		}
// 2013.12.16 Bagus Add (TohoSpec対応) -->
	}
// 2013.12.16 Bagus Add (TohoSpec対応) <--

	// JoyStick切替許可に戻す
	SetJoyStickSelectEnabled(TRUE);

	return bRet;
}
/* added 2009.07.23 hmenjo ヘッドタイプチェック関数追加 ---------- { ---------- */
/*	ヘッドタイプがシステム設定で有効かどうかチェックします．	*/
BOOL CNanoSpecDoc::IsValidHeadType(WORD wHeadType)
{
	/* システムコンフィグからヘッド有効/無効を取得	*/
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	BOOL l_bValidHead = FALSE;
	switch (wHeadType) {
	case HEAD_TYPE_SR:		l_bValidHead = l_SystemConfig.HeadType.bSR;		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//	case HEAD_TYPE_SE:		l_bValidHead = l_SystemConfig.HeadType.bSE;		break;
	case HEAD_TYPE_SE:		l_bValidHead = l_SystemConfig.HeadType.bSE && !l_SystemConfig.HeadType.bCompEASE;	break;
	case HEAD_TYPE_COMPEASE:l_bValidHead = l_SystemConfig.HeadType.bSE && l_SystemConfig.HeadType.bCompEASE;	break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case HEAD_TYPE_IRSE:	l_bValidHead = l_SystemConfig.HeadType.bIRSE;	break;
	case HEAD_TYPE_MS:		l_bValidHead = l_SystemConfig.HeadType.bMS;		break;
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:		l_bValidHead = l_SystemConfig.HeadType.bResist;	break;
	case HEAD_TYPE_CTA:		l_bValidHead = l_SystemConfig.HeadType.bCTA;	break;
	case HEAD_TYPE_STRESS:	l_bValidHead = l_SystemConfig.HeadType.bStress;	break;
	default:
		l_bValidHead = FALSE;
		break;
	}

	return l_bValidHead;
}
/* added 2009.07.23 hmenjo ヘッドタイプチェック関数追加 ---------- } ---------- */
/* added 2009.07.23 hmenjo スキャンタイプチェック関数追加 ---------- { ---------- */
/*	スキャンタイプとヘッドタイプの整合性をチェックします．*/
/*			各ヘッド毎に定義されているスキャンタイプはすべて０で始まっています．
 *			つまり，別のヘッドのスキャンタイプが設定されていても値が同じなため，
 *			有効となってしまうことがあります．
 *			しかし，これは入力画面で制限されているはずですので，測定プログラムファイルを
 *			直接変更しない限り不都合は起きません．
 *			実際には，他処理の変更部分が多大なため，ココでは範囲チェックのみとしています．*/
BOOL CNanoSpecDoc::IsValidScanType(WORD wScanType, WORD wHeadType)
{
	/* まずヘッドタイプをチェックします．*/
	if (0 == IsValidHeadType(wHeadType)) {
		return FALSE;
	}

	/* ヘッドタイプ毎にスキャンタイプをチェックします．*/
	BOOL l_bRet = FALSE;
	switch (wHeadType) {
	case HEAD_TYPE_SR:
		if ((MEAS_PROG_TYPE_SR_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_SR_MAX)) {l_bRet = TRUE;}
		break;
	case HEAD_TYPE_SE:
		if ((MEAS_PROG_TYPE_SE_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_SE_MAX)) {l_bRet = TRUE;}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		if ((MEAS_PROG_TYPE_COMPEASE_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_COMPEASE_MAX)) {l_bRet = TRUE;}
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		if ((MEAS_PROG_TYPE_IRSE_THICKNESS <= wScanType) && (wScanType < MEAS_PROG_TYPE_IRSE_MAX)) {l_bRet = TRUE;}
		break;
#else
	case HEAD_TYPE_MS:
		if ((MEAS_PROG_TYPE_MS_MEAS <= wScanType) && (wScanType < MEAS_PROG_TYPE_MS_MAX)) {l_bRet = TRUE;}
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
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
/* added 2009.07.23 hmenjo スキャンタイプチェック関数追加 ---------- } ---------- */
/* added 2009.07.30 hmenjo ストレス機能追加(4) ---------- { ---------- */
/*
 *	アラーム情報取得
 *		AlarmList.ini に登録されている情報を読出します．
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
/* added 2009.07.30 hmenjo ストレス機能追加(4) ---------- } ---------- */
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- { ---------- */
/*
 *	HEPA オン/オフ ラッパ関数
 */
BOOL CNanoSpecDoc::HepaOnOff(BOOL bOn, char* pcHepaAlarmLevel/*= 0*/)
{
	/* HEPA アラームレベルを取得	*/
	if (0 != pcHepaAlarmLevel) {
		char l_cLevel;
/* 2009.08.20 K.Matsuo ALID 定義を変更 ---------- { ---------- */
//		this->GetAlarmInf(HEPA_ALID, 0, &l_cLevel, 0, 0);
/* 2009.08.20 K.Matsuo ALID 定義を変更 ----------			   */
		this->GetAlarmInf(ALID_HepaStopFailed, 0, &l_cLevel, 0, 0);
/* 2009.08.20 K.Matsuo ALID 定義を変更 ---------- } ---------- */
		*pcHepaAlarmLevel = l_cLevel;
	}

	/* ココに NEXTRA の関数が書かれる？？？	*/
	/*	NEXTRA からの戻りが入る	*/
	/*	NEXTRA からの戻りが入る	*/
	BOOL l_bRet = FALSE;
	CTimer timer;

	nexioHepaPowerOFF(!bOn);
	timer.Restart(3);
	while ( !timer.IsTimeout() && !l_bRet ) l_bRet = nexioIsHepaStatus();

	return l_bRet;
}
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- } ---------- */
//2009.08.20 bagus stress --{--
BOOL CNanoSpecDoc::SetOneStressLineData(STRESSRESULT *OneStressLineData,int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //格納失敗
	}
	m_pMeasureddata->StressData[iLineNo - 1] = *OneStressLineData;
	return TRUE; //格納成功
}
BOOL CNanoSpecDoc::GetOneStressLineData(STRESSRESULT *OneStressLineData,int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //取得失敗
	}
	*OneStressLineData = m_pMeasureddata->StressData[iLineNo - 1];
	return TRUE; //取得成功
}
BOOL CNanoSpecDoc::GetOneStressLineDataValid(int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //取得失敗
	}
	return m_pMeasureddata->StressDataValid[iLineNo - 1];
}
void CNanoSpecDoc::SetOneStressLineDataValid(int iLineNo,BOOL bValid)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return; //取得失敗
	}
	m_pMeasureddata->StressDataValid[iLineNo - 1] = bValid;
}
BOOL CNanoSpecDoc::GetOneStressLineDataRef(int iLineNo)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return FALSE; //取得失敗
	}
	return m_pMeasureddata->StressDataRef[iLineNo - 1];
}
void CNanoSpecDoc::SetOneStressLineDataRef(int iLineNo,BOOL bRef)
{
	if((iLineNo > STRESS_LINES_MAX) || (iLineNo < 1 ))
	{
		return; //取得失敗
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
/* added 2009.10.20 hmenjo GTR ランプ制御実装 ---------- { ---------- */
/* added 2009.09.07 hmenjo 透過率 ランプ制御 ---------- { ---------- */
/*
 *	透過率用ランプ制御
 */
BOOL CNanoSpecDoc::SetTransmittanceLamp(WORD wFilter/* = FILTER_DARK*/)
{
	static WORD ls_wFilterPre = -1;	/* 初期値	*/
	BOOL l_bRet = TRUE;
	WORD l_wFilter = wFilter;

	if (ls_wFilterPre != l_wFilter) {
/* deleted 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- { ---------- */
//		ls_wFilterPre = l_wFilter;
/* deleted 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- } ---------- */
		/* ココに透過率ランプの制御を入れてください．	*/
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
		/* シャッタ動作完了待ち	*/
		SR_CONFIG l_SrConfig;
		ConfigFile_GetNanoSpecIni(&l_SrConfig, CONFIG_FILE_SR_CONFIG);
		DWORD l_dwShutterWait = ((DWORD) l_SrConfig.wTrShutterMoveWaitTime) + 10;	/* 誤差分を加算	*/
		::Sleep(l_dwShutterWait);
		/* 正常性チェック	*/
		BOOL l_bResult1 = nexioIsTransmitLight1Condition();
		BOOL l_bResult2 = nexioIsTransmitLight2Condition();
		BOOL l_bResult3 = nexioIsTransmitLight3Condition();
/* modified 2009.11.24 hmenjo GTR ランプエラー検出修正 ---------- { ---------- */
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
/* modified 2009.11.24 hmenjo GTR ランプエラー検出修正 ----------			   */
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
/* modified 2009.11.24 hmenjo GTR ランプエラー検出修正 ---------- } ---------- */
	}

/* added 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- { ---------- */
	if (TRUE == l_bRet) {
		ls_wFilterPre = l_wFilter;
	}
/* added 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- } ---------- */

	return l_bRet;
}
/* added 2009.09.07 hmenjo 透過率 ランプ制御 ---------- } ---------- */
/* added 2009.10.20 hmenjo GTR ランプ制御実装 ---------- } ---------- */

/* added 2009.10.20 hmenjo GTR ランプチェック実装 ---------- { ---------- */
/* added 2009.09.10 hmenjo ガントリ透過率用ランプチェック ---------- { ---------- */
/*
 *	ガントリ透過率用ハロゲンランプ オン確認
 */
BOOL CNanoSpecDoc::IsGTRLampOn(void)
{
	BOOL l_bLampOn;

	l_bLampOn = nexioIsTransmitLightSourcePower();

	return l_bLampOn;
}
/* added 2009.09.10 hmenjo ガントリ透過率用ランプチェック ---------- } ---------- */
/* added 2009.10.20 hmenjo GTR ランプチェック実装 ---------- } ---------- */

// 2009.10.09 bagus StagePGM 共通化 --{--
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
// 2009.10.09 bagus StagePGM 共通化 --}--
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
//手動測定時の準備/後始末処理
//ここではロットID/カセットIDやレシピはセットされた後で呼ばれることを想定している
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

/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
long CNanoSpecDoc::GetExpDouble(double dData)
{
	double l_dData = dData;
	long l_iLog10 = (long) (log10(fabs(l_dData)));
	long l_iExp = (((long) (l_dData / (pow(10, l_iLog10)))) < 1)?
					l_iLog10 + (-1) :
					l_iLog10;

	return l_iExp;
}
/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */
