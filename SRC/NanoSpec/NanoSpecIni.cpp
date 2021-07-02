// NanoSpecIni.cpp: implementation of the CNanoSpecIni class.
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpecIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct
{
	LPCSTR	section;
	LPCSTR	key;
	char	type;
	LPCSTR	def;
	void*	data;
} INIFILE_T;

// << ini file >>
// [Section Name]
// (key name)=***
// (key name)=***
// 	.
// 	.
//
// <data type>
// 'b' = boolean
// 'B' = BYTE
// 'S' = short
// 'I' = integer
// 'L' = long
// 'F' = float
// 'C' = character (max length 20)

const INIFILE_T IniDesc[] =
{
//	Section Name			Key Name							Data Type	Defaule		Variable Pointer
	{INISECTION_SIMULATION,	INIKEY_HWS,							'b',		"FALSE",	&g_bHWS							},

	{INISECTION_HEADCOM,	INIKEY_COMMNO,						'C',		"COM1",		&g_szCommNo						},
	{"",					INIKEY_BAUDRATE,					'C',		"19200",	&g_szBaudRate					},
	{"",					INIKEY_PARITY,						'C',		"N",		&g_szParity						},
	{"",					INIKEY_BYTESIZE,					'C',		"8",		&g_szByteSize					},
	{"",					INIKEY_STOPBITS,					'C',		"1",		&g_szStopBits					},
	{"",					INIKEY_CCDDATATRANSFERMODEBINARY,	'I',		"1",		&g_szCcdDataTransferModeBinary	},

	{INISECTION_STAGESIZE,	INIKEY_HWSTAGESIMULATIN,			'b',		"FALSE",	&g_bHWStageSimulation			},
	{"",					INIKEY_HWS_STAGESIZEX,				'I',		"100",		&g_lHWStageSizeX				},
	{"",					INIKEY_HWS_STAGESIZEY,				'I',		"100",		&g_lHWStageSizeY				},
	{"",					INIKEY_HWS_STAGEEDGESIZEX,			'I',		"20",		&g_lHWEdgeSizeX					},
	{"",					INIKEY_HWS_STAGEEDGESIZEY,			'I',		"20",		&g_lHWEdgeSizeY					},
	{"",					INIKEY_HWS_STAGEORIGINOFFSETX,		'I',		"0",		&g_lHWOriginOffsetX				},
	{"",					INIKEY_HWS_STAGEORIGINOFFSETY,		'I',		"0",		&g_lHWOriginOffsetY				},
// added hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- { ----------
	{INISECTION_EQMAINTE,	INIKEY_EQM_CYCLESTOP,				'b',		"TRUE",		&g_bIL_CycleStop				},
// added hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- } ----------
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- { ----------
	{INISECTION_EQMAINTE,	INIKEY_EQM_DOOROPEN,				'b',		"TRUE",		&g_bIL_DoorOpen					},
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- } ----------
// added hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- { ----------
	{INISECTION_EQMAINTE,	INIKEY_EQM_SPEEDDOWN,				'b',		"TRUE",		&g_bIL_SpeedDown				},
// added hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- } ----------
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- { ---------- */
	{INISECTION_EQMAINTE,	INIKEY_EQM_EQPOWEROFF,				'b',		"TRUE",		&g_bIL_EQPowerOff				},
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- } ---------- */
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	{INISECTION_EQMAINTE,	INIKEY_EQM_DOOROPENPAUSE,			'b',		"FALSE",	&g_bIL_DoorOpenPause			},
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- { ---------- */
	{INISECTION_SUBCONPWR,	INIKEY_SBCPWR_OFFTIME,				'I',		"0",		&g_lSBCPwr_OffTime				},
	{"",					INIKEY_SBCPWR_WAITAFTERON,			'I',		"2",		&g_lSBCPwr_WaitAfterOn			},
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- } ---------- */
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	{INISECTION_MODELTYPE,	INIKEY_MODELTYPE_TYPE,				'I',		"6500",		&g_lModelType					},
	{INISECTION_OVIDEO,	INIKEY_OVIDEO_ENABLE,				'I',		"0",		&g_lOVideoEnable					},
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
	{"",					INIKEY_OVIDEO_FORMATSIZE,			'C',	"UYVY (640x480)",	&g_szVideoFmtSize			},
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
// 2013.11.08 Bagus Add (TohoSpec対応) <--

};

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction

/////////////////////////////////////////////////////////////////////////////
//
CNanoSpecIni::CNanoSpecIni()
{
	CFileFind Finder;
// 2013.11.07 Bagus Delete (TohoSpec対応) -->
//	char path[MAX_PATH];
// 2013.11.07 Bagus Delete (TohoSpec対応) <--
	int retv;

	m_IniCount = sizeof(IniDesc) / sizeof(IniDesc[0]);

// 2013.11.07 Bagus Add (TohoSpec対応) -->
	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if(g_lAppNameType != APP_NAME_NANO){
		strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}
	m_strIniFilePath.Format("%s%s", g_szCfg_Dir, strFilename);
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	Get();

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	sprintf(path, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
//	if((retv = Finder.FindFile(path)) == 0){
	if((retv = Finder.FindFile(m_strIniFilePath)) == 0){
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		Set();
	}

	Finder.Close();
}

/////////////////////////////////////////////////////////////////////////////
//
CNanoSpecIni::~CNanoSpecIni()
{

}

/////////////////////////////////////////////////////////////////////////////
// GetNanoSpecIni
void CNanoSpecIni::Get()
{
	int index;
	LPCSTR section;
	char buff[1024 + 1];
	char szFilePath[MAX_PATH];

	for(index = 0 ; index < m_IniCount ; index++){
		// get section name
		if(strlen(IniDesc[index].section) > 0)
			section = IniDesc[index].section;

		// get ini file data
		memset(buff, NULL, sizeof(buff));
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
		strcpy(szFilePath, m_strIniFilePath);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		GetPrivateProfileString(section,
								IniDesc[index].key,
								IniDesc[index].def,
								buff,
								sizeof(buff)-1,
								szFilePath);

		// set ini file data to variable
		switch(IniDesc[index].type){
		case 'b':
			*(BOOL*)IniDesc[index].data = (strcmp(buff, "TRUE") == 0) ? TRUE : FALSE;
			break;
		case 'B':
			*(BYTE*)IniDesc[index].data = (BYTE)atoi(buff);
			break;
		case 'S':
			*(short*)IniDesc[index].data = (short)atoi(buff);
			break;
		case 'I':
			*(int*)IniDesc[index].data = (int)atoi(buff);
			break;
		case 'L':
			*(long*)IniDesc[index].data = (long)atol(buff);
			break;
		case 'F':
			*(float*)IniDesc[index].data = (float)atof(buff);
			break;
		case 'C':
			strcpy((char*)IniDesc[index].data, buff);
			break;
		default:
			(void*)IniDesc[index].data = NULL;
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// SetNanoSpecIni
void CNanoSpecIni::Set()
{
	int index;
	LPCSTR section;
	char buff[1024 + 1];
	char szFilePath[MAX_PATH];
	int len;

	for(index = 0; index < m_IniCount; index++){
		// get section name
		if(strlen(IniDesc[index].section) > 0)
			section = IniDesc[index].section;

		// set ini file data
		memset(buff, NULL, sizeof(buff));
		switch(IniDesc[index].type){
		case 'b':
			strcpy(buff, (*(BOOL*)IniDesc[index].data) ? "TRUE" : "FALSE");
			break;
		case 'B':
			_itoa(*(BYTE*)IniDesc[index].data, buff, 10);
			break;
		case 'S':
			_itoa(*(short*)IniDesc[index].data, buff, 10);
			break;
		case 'I':
			_itoa(*(int*)IniDesc[index].data, buff, 10);
			break;
		case 'L':
			_ltoa(*(long*)IniDesc[index].data, buff, 10);
			break;
		case 'F':
			sprintf(buff, "%-10.3f", *(float*)IniDesc[index].data);
			break;
		case 'C':
			len = strlen((char*)IniDesc[index].data);
			strncpy(buff, (char*)IniDesc[index].data, len);
			break;
		default:
			break;
		}

		// write procon ini file
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
		strcpy(szFilePath, m_strIniFilePath);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		WritePrivateProfileString(section,
								  IniDesc[index].key,
								  strtrim(buff),
								  szFilePath);
	}
}
