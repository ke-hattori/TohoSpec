// FileRetentionPeriod.cpp : implementation file
//

#include "stdafx.h"
#include "system.h"
//#include "Procon.h"
//#include "ProconDlg.h"
//#include "Nanoif.h"
//#include "MainInfoDlg.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "FileRetentionPeriod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileRetentionPeriod                                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CFileRetentionPeriod, CWinThread)

// ==========================================================================
//
// ==========================================================================
CFileRetentionPeriod::CFileRetentionPeriod()
{
	m_Run = TRUE;

	m_pProcess = NULL;
	m_bLoop = FALSE;
	m_iLimitTime = 0;
// 2013.11.13 Bagus Add (Bug Fix) -->
	m_iDeleteDataPeriod = 0;
// 2013.11.13 Bagus Add (Bug Fix) <--
	memset(m_szPath, 0, sizeof(m_szPath));

	//Saiki 20100517 Add ----->
	///// FileRetentionLogFile /////
	m_FRetentionLogFile.Create(g_szLog_Dir,								// LOG_DIR,
								FRETEN_LOG_NAME,
								FRETEN_LOG_SIZE,
								FRETEN_LOG_MAX);
	FileRetentionLogging("Start CFileRetentionPeriod constructor");

	m_pProcessLogFile.Create(	g_szLog_Dir,
								FRETENPRO_LOG_NAME,
								FRETENPRO_LOG_SIZE,
								FRETENPRO_LOG_MAX);
}

// ==========================================================================
//
// ==========================================================================
CFileRetentionPeriod::~CFileRetentionPeriod()
{
}

// ==========================================================================
//
// ==========================================================================
BOOL CFileRetentionPeriod::InitInstance()
{
	VariableInitialize();
	ProcessInitialize();
	ProcessChangeInitialize();
	TimerInitialize();

	return TRUE;
}

// ==========================================================================
//
// ==========================================================================
int CFileRetentionPeriod::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

// ==========================================================================
//
// ==========================================================================
BEGIN_MESSAGE_MAP(CFileRetentionPeriod, CWinThread)
	//{{AFX_MSG_MAP(CFileRetentionPeriod)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileRetentionPeriod message handlers                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// ==========================================================================
//
// ==========================================================================
int CFileRetentionPeriod::Run()
{
	//Saiki 20100517 Add ----->
	if(m_Run) FileRetentionLogging("CFileRetentionPeriod::Run");
	//Saiki 20100517 Add <-----
	while(m_Run){

		///// Timer /////
		Timer();

		PROCESS_DirCheck();
        ProcessLogging();
		Sleep(1000);
	}

	//Saiki 20100517 Add ----->
	FileRetentionLogging("ExitThread");
	//Saiki 20100517 Add <-----

	ExitThread(0);

	return CWinThread::Run();
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::VariableInitialize()
{
	///// Load Data /////
	ConfigFile_GetNanoSpecIni(&m_iDeleteDataPeriod, CONFIG_FILE_DELETE_DATA_PERIOD);
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::ProcessInitialize()
{
	memset(&m_PROCESS, 0, sizeof(m_PROCESS));
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::ProcessChangeInitialize()
{
	int i;

	memset(m_pProcessChange, 0, sizeof(m_pProcessChange));
	memset(m_Old_ProcessChange, -1, sizeof(m_Old_ProcessChange));

	m_ProcessChangeMax = 0;

	m_pProcessChange[m_ProcessChangeMax++] = &m_PROCESS.DirCheck;

	for(i = 0 ; i < m_ProcessChangeMax ; i++){
		*m_pProcessChange[i] = 0;
	}
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::TimerInitialize()
{
	int i;

	memset(m_pTimer,0,sizeof(m_pTimer));

	m_TimerMax = 0;

	m_pTimer[m_TimerMax++] = &m_MyTimer;

	///// Timer Initialixe /////
	for(i = 0 ; i < m_TimerMax ; i++){
		ResetTimer(m_pTimer[i]);
	}
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::Timer()
{
	int i;

	DWORD curr = 0L;

	curr = GetTickCount();

	for(i = 0 ; i < m_TimerMax ; i++){
		if(m_pTimer[i]->Valid == TRUE){
			if(curr >= m_pTimer[i]->Start){
				m_pTimer[i]->Estimate = curr - m_pTimer[i]->Start;
			}
			else{
				m_pTimer[i]->Estimate = curr + (0xffffffff - m_pTimer[i]->Start + 1);
			}
			if(m_pTimer[i]->Estimate >= m_pTimer[i]->Value){
				m_pTimer[i]->Up = TRUE;
				m_pTimer[i]->Valid = FALSE;
			}
		}
	}
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::StartTimer(TIMER_STATUS* Timer, int Value)
{
	Timer->Valid	= TRUE;
	Timer->Up		= FALSE;
	Timer->Value	= Value * 100;
	Timer->Start	= GetTickCount();
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::ResetTimer(TIMER_STATUS* Timer)
{
	Timer->Valid	= FALSE;
	Timer->Up		= FALSE;
	Timer->Value	= 0;
	Timer->Start	= 0;
	Timer->Estimate = 0;
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::PROCESS_DirCheck()
{
	m_pProcess = &m_PROCESS.DirCheck;

	CString sFileName;
	CString sWildCard;
	CString sPathName;
	CString sLimitTimeString;
	CTime LimitTime;
	CTime FileTime;
	__int64 iFileTime = 0;
	int retv;
    int iBackSlash;
    CString sFileTimeString;
	BOOL bEnable = FALSE;
	char szBuff[256];
	char szDrive[16+1];
	char szDirName[256+1];
	char szFileName[128+1];
	char szExtension[16+1];
	CString sFileTitle;
    CString strUpDir;
    CString strPath;
    CFileFind FindDir;


	// ï¿½Iï¿½ï¿½ï¿½ï¿½eï¿½ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½æ“¾
	typedef struct _PERIODSEARCH {
		int nSelect;
		int iYear;
		int iMonth;
	} PERIODSEARCH, *PPERIODSEARCH;

	PERIODSEARCH PeriodSearchTable[] =
	{
		{ DELETE_DATA_PERIOD_NONE,				0,	0	},
		{ DELETE_DATA_PERIOD_ONE_MONTH, 		0,	1	},
		{ DELETE_DATA_PERIOD_THREE_MONTHS,		0,	3	},
		{ DELETE_DATA_PERIOD_SIX_MONTHS,		0,	6	},
		{ DELETE_DATA_PERIOD_ONE_YEAR,			1,	0	},
		{ DELETE_DATA_PERIOD_ONE_HALF_YEARS,	1,	6	},
		{ DELETE_DATA_PERIOD_TWO_YEARS, 		2,	0	},
		{ DELETE_DATA_PERIOD_TWO_HALF_YEARS,	2,	6	},
		{ DELETE_DATA_PERIOD_THREE_YEARS,		3,	0	},
		{ DELETE_DATA_PERIOD_THREE_HALF_YEARS,	3,	6	},
		{ DELETE_DATA_PERIOD_FOUR_YEARS,		4,	0	},
		{ DELETE_DATA_PERIOD_FOUR_HALF_YEARS,	4,	6	},
		{ DELETE_DATA_PERIOD_FIVE_YEARS,		5,	0	},
	};

	//ï¿½ï¿½ï¿½İ‚Ì�ï¿½ï¿½Ô‚ï¿½ï¿½æ“¾
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	// ï¿½ï¿½ï¿½İ“ï¿½ï¿½tï¿½ï¿½ï¿½ï¿½Ì�í�œï¿½Nï¿½ï¿½
	int iDeleteYear;
	int iDeleteMonth;
	if ( (int)systemTime.wMonth > PeriodSearchTable[m_iDeleteDataPeriod].iMonth ) {
		iDeleteYear = (int)systemTime.wYear - PeriodSearchTable[m_iDeleteDataPeriod].iYear;
		iDeleteMonth = (int)systemTime.wMonth - PeriodSearchTable[m_iDeleteDataPeriod].iMonth;
	}
	else {
		iDeleteYear = ((int)systemTime.wYear - 1) - PeriodSearchTable[m_iDeleteDataPeriod].iYear;
		iDeleteMonth = ((int)systemTime.wMonth + 12) - PeriodSearchTable[m_iDeleteDataPeriod].iMonth;
	}


	//ï¿½^ï¿½Cï¿½ï¿½ï¿½Xï¿½pï¿½ï¿½ï¿½ÌŒvï¿½Z
	CTimeSpan ts = CTime(systemTime.wYear, systemTime.wMonth, systemTime.wDay, 0, 0, 0) - CTime(iDeleteYear, iDeleteMonth, systemTime.wDay, 0, 0, 0);

	CTimeSpan timeSpan( static_cast<LONG>(ts.GetDays()),
					0,	//Hour
					0,	//Minute
					0); //Sec

	//ï¿½í�œï¿½ï¿½ï¿½Ô‚ï¿½ï¿½İ’ï¿½È‚ï¿½ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í�İ’è‚³ï¿½ï¿½Ä‚ï¿½ï¿½é�€ï¿½ÚˆÈ�ï¿½Å‚È‚ï¿½ï¿½ï¿½ï¿½mï¿½F
	if(m_iDeleteDataPeriod > PeriodSearchTable[0].nSelect && DELETE_DATA_PERIOD_MAX > m_iDeleteDataPeriod){
//		bEnable = TRUE;
//	}
//
//	switch(*m_pProcess){
//	case 0://Processï¿½ÌŠJï¿½n
//		//if(bEnable == TRUE){
//			if(m_szPath[0] != '\0'){
//				*m_pProcess = 100;
//				break;
//			}
//		//}
//		break;
//
//	case 100:
//		//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ÌŒï¿½ï¿½ï¿½
//		sWildCard.Format("%s*.*", m_szPath);
//        try{
//			m_bLoop = m_Finder.FindFile(sWildCard);
//		}
//        catch(CFileException* fe){
//            long ErrorCode = fe->m_lOsError;
//            sprintf(szBuff, "(m_pProcess=100) %s, ErrorCode = %l", "File Invalid Error", ErrorCode);
//			FileRetentionLogging(szBuff);
//            break;
//        }
//
//		if(m_bLoop == 0){
//			//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
//			*m_pProcess = 9000;
//			break;
//		}
//
//		m_CurTime = CTime::GetCurrentTime();
//		LimitTime = m_CurTime - timeSpan;
//		sLimitTimeString = LimitTime.Format("%Y%m%d%H%M%S");
//		m_iLimitTime = _atoi64(sLimitTimeString.GetBuffer(0));
//
//		*m_pProcess = 200;
//		break;
//
//	case 200:
//		try{
//			m_bLoop = m_Finder.FindNextFile();
//		}
//        catch(CFileException* fe){
//            long ErrorCode = fe->m_lOsError;
//            sprintf(szBuff, "(m_pProcess=200) %s, ErrorCode = %l", "File Invalid Error", ErrorCode);
//			FileRetentionLogging(szBuff);
//            break;
//        }
//
//		if(m_Finder.IsDots()) break;
//		if(m_Finder.IsDirectory()) break;
////		if(m_Finder.IsDirectory()){
////			//Stressï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Â�lï¿½ì�¬ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì�ê�‡
////			*m_pProcess = 1000;
////			 break;
////		}
//
//		strcpy(m_szFilePath, m_Finder.GetFilePath());
//		_tsplitpath(m_szFilePath, szDrive, szDirName, szFileName, szExtension); //ï¿½pï¿½Xï¿½ğ•ª‰ï¿½
//		if(strcmp(szExtension, DAT_EXT) != 0) break;
//
//		retv = m_Finder.GetLastWriteTime(FileTime);
//
//		if(retv == 0) break;
//
//		sFileTimeString = FileTime.Format("%Y%m%d%H%M%S");
//		iFileTime = _atoi64(sFileTimeString.GetBuffer(0));
//		if(iFileTime < m_iLimitTime){
//			//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ÛŠÇŠï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
//			*m_pProcess = 300;
//			break;
//		}
//
//        if(m_bLoop == 0){
//			//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
//			*m_pProcess = 9000;
//			break;
//		}
//		break;
//	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½É‘Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½é‚©
//	case 300:
//		sFileTitle = m_Finder.GetFileTitle();
//		sPathName.Format("%s%s", m_szPath, sFileTitle);
//		if(FileOrDirExists(sPathName) == FALSE){
//			//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½È‚ï¿½
//			*m_pProcess = 400;
//		}
//		else{
//			//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½
//            strcpy(m_szNextPath, sPathName);
//			*m_pProcess = 500;
//		}
//	break;
//
//	//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ê�‡ï¿½í�œï¿½ï¿½ï¿½ï¿½(4PP, CTAï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^) ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½í�œï¿½ï¿½Ì‘ï¿½ï¿½ï¿½fï¿½[ï¿½^
//	case 400:
//        ::DeleteFile(m_szFilePath);
//		if(m_bLoop == FALSE){
//			*m_pProcess = 9000;
//			break;
//		}
//		*m_pProcess = 200;
//	break;
//
//	//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½Ü‚ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì’ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½Sï¿½Ä�í�œï¿½ï¿½ï¿½ï¿½(SR, SE, 2Point Distanceï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^)
//	case 500:
//		if(DeleteAllData(m_szNextPath) == FALSE){
//			*m_pProcess = 8000;
//		}
//		else{
//			*m_pProcess = 600;
//		}
//	break;
//
//	//ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ìƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�Aï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½Å‚ï¿½ï¿½ï¿½Î�ï¿½ï¿½ï¿½
//	case 600:
//		if(CheckDirectory(m_szNextPath)){
//			::RemoveDirectory(m_szNextPath);
//			*m_pProcess = 700;
//		}
//		else{
//			*m_pProcess = 500;
//		}
//	break;
//
//	//ï¿½Kï¿½wï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½ï¿½ï¿½ï¿½
//	case 700:
//        strPath.Format("%s", m_szNextPath);
//		iBackSlash = strPath.ReverseFind('\\');
//		strUpDir = strPath.Left(iBackSlash+1);
//		if(strUpDir.Compare(m_szPath) == 0){
//			*m_pProcess = 400;
//		}
//		else{
//            strcpy(m_szNextPath, strPath.Left(iBackSlash));
//			*m_pProcess = 600;
//		}
//	break;
//
//	///// StressDataï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Â�lï¿½ì�¬ï¿½tï¿½Hï¿½ï¿½ï¿½_ /////
//	case 1000:
//		if(CheckStressResultDir(m_Finder.GetFilePath())){
//			// Stressï¿½ï¿½ï¿½ï¿½ÌŒï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½
//			*m_pProcess = 2000;
//		}
//		else{
//			// ï¿½Â�lï¿½ì�¬ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½
//			*m_pProcess = 3000;
//		}
//	break;
//
//	case 2000:
//	break;
//
//	case 3000:
//	break;
//
//	///// Abnormal End /////
//	case 8000:
//		m_Finder.Close();
//		*m_pProcess = 0;
//		break;
//
//	///// Normal End /////
//	case 9000:
//		m_Finder.Close();
//		*m_pProcess = 0;
//		break;
//    }
		switch(*m_pProcess){
		case 0://Processï¿½ÌŠJï¿½n
			//if(bEnable == TRUE){
				if(m_szPath[0] != '\0'){
					*m_pProcess = 100;
					break;
				}
			//}
			break;

		case 100:
			//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ÌŒï¿½ï¿½ï¿½
			sWildCard.Format("%s*.*", m_szPath);
	        try{
				m_bLoop = m_Finder.FindFile(sWildCard);
			}
	        catch(CFileException* fe){
	            long ErrorCode = fe->m_lOsError;
	            sprintf(szBuff, "(m_pProcess=100) %s, ErrorCode = %ld", "File Invalid Error", ErrorCode);
				FileRetentionLogging(szBuff);
				*m_pProcess = 8000;
	            break;
	        }

			if(m_bLoop == 0){
				//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
				*m_pProcess = 9000;
				break;
			}

//			m_CurTime = CTime::GetCurrentTime();
//			LimitTime = m_CurTime - timeSpan;
//			sLimitTimeString = LimitTime.Format("%Y%m%d%H%M%S");
//			m_iLimitTime = _atoi64(sLimitTimeString.GetBuffer(0));

			*m_pProcess = 200;
			break;

		case 200:
			try{
				m_bLoop = m_Finder.FindNextFile();
			}
	        catch(CFileException* fe){
	            long ErrorCode = fe->m_lOsError;
	            sprintf(szBuff, "(m_pProcess=200) %s, ErrorCode = %ld", "File Invalid Error", ErrorCode);
				FileRetentionLogging(szBuff);
				*m_pProcess = 8000;
	            break;
	        }

			if(m_Finder.IsDots()) break;
			if(m_Finder.IsDirectory()) break;
	//		if(m_Finder.IsDirectory()){
	//			//Stressï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Â�lï¿½ì�¬ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì�ê�‡
	//			*m_pProcess = 1000;
	//			 break;
	//		}

			m_CurTime = CTime::GetCurrentTime();
			LimitTime = m_CurTime - timeSpan;
			sLimitTimeString = LimitTime.Format("%Y%m%d%H%M%S");
			m_iLimitTime = _atoi64(sLimitTimeString.GetBuffer(0));


			strcpy(m_szFilePath, m_Finder.GetFilePath());
			_tsplitpath(m_szFilePath, szDrive, szDirName, szFileName, szExtension); //ï¿½pï¿½Xï¿½ğ•ª‰ï¿½
			if(strcmp(szExtension, DAT_EXT) != 0) break;

			retv = m_Finder.GetLastWriteTime(FileTime);

			if(retv == 0) break;

			sFileTimeString = FileTime.Format("%Y%m%d%H%M%S");
			iFileTime = _atoi64(sFileTimeString.GetBuffer(0));
			if(iFileTime < m_iLimitTime){
				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ÛŠÇŠï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½
				*m_pProcess = 300;
				break;
			}

	        if(m_bLoop == 0){
				//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
				*m_pProcess = 9000;
				break;
			}
			break;
		//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½É‘Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½é‚©
		case 300:
			sFileTitle = m_Finder.GetFileTitle();
			sPathName.Format("%s%s", m_szPath, sFileTitle);
			if(FileOrDirExists(sPathName) == FALSE){
				//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½È‚ï¿½
				*m_pProcess = 400;
			}
			else{
				//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½ï¿½
	            strcpy(m_szNextPath, sPathName);
				*m_pProcess = 500;
			}
		break;

		//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½ê�‡ï¿½í�œï¿½ï¿½ï¿½ï¿½(4PP, CTAï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^) ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½í�œï¿½ï¿½Ì‘ï¿½ï¿½ï¿½fï¿½[ï¿½^
		case 400:
            sprintf(szBuff, "[PROCESS_DirCheck] Delete File %s", m_szFilePath);
			FileRetentionLogging(szBuff);
	        ::DeleteFile(m_szFilePath);
			if(m_bLoop == FALSE){
				*m_pProcess = 9000;
				break;
			}
			*m_pProcess = 200;
		break;

		//ï¿½Î‰ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½Ü‚ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì’ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½Sï¿½Ä�í�œï¿½ï¿½ï¿½ï¿½(SR, SE, 2Point Distanceï¿½ï¿½ï¿½ï¿½fï¿½[ï¿½^)
		case 500:
			if(DeleteAllData(m_szNextPath) == FALSE){
				*m_pProcess = 8000;
			}
			else{
				*m_pProcess = 600;
			}
		break;

		//ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Ìƒfï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�Aï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½Å‚ï¿½ï¿½ï¿½Î�ï¿½ï¿½ï¿½
		case 600:
			if(CheckDirectory(m_szNextPath)){
	            sprintf(szBuff, "[PROCESS_DirCheck] Remove Directory %s", m_szNextPath);
				FileRetentionLogging(szBuff);
				::RemoveDirectory(m_szNextPath);
				*m_pProcess = 700;
			}
			else{
				*m_pProcess = 500;
			}
		break;

		//ï¿½Kï¿½wï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½ï¿½ï¿½ï¿½
		case 700:
	        strPath.Format("%s", m_szNextPath);
			iBackSlash = strPath.ReverseFind('\\');
			strUpDir = strPath.Left(iBackSlash+1);
			if(strUpDir.Compare(m_szPath) == 0){
				*m_pProcess = 400;
			}
			else{
	            strcpy(m_szNextPath, strPath.Left(iBackSlash));
				*m_pProcess = 600;
			}
		break;

		///// StressDataï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Â�lï¿½ì�¬ï¿½tï¿½Hï¿½ï¿½ï¿½_ /////
		case 1000:
			if(CheckStressResultDir(m_Finder.GetFilePath())){
				// Stressï¿½ï¿½ï¿½ï¿½ÌŒï¿½ï¿½Êƒfï¿½[ï¿½^ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½
				*m_pProcess = 2000;
			}
			else{
				// ï¿½Â�lï¿½ì�¬ï¿½Ìƒfï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½
				*m_pProcess = 3000;
			}
		break;

		case 2000:
		break;

		case 3000:
		break;

		///// Abnormal End /////
		case 8000:
			FileRetentionLogging("File Retention Abnormal End");
			m_Finder.Close();
			*m_pProcess = 0;
			break;

		///// Normal End /////
		case 9000:
			FileRetentionLogging("File Retention Normal End");
			m_Finder.Close();
			*m_pProcess = 0;
			break;
	    }
	}
}

// ==========================================================================
//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½İƒ`ï¿½Fï¿½bï¿½N
// ==========================================================================
BOOL CFileRetentionPeriod::FileOrDirExists(LPCTSTR pszFileName)
{
	return GetFileAttributes(pszFileName) != 0xffffffff;
}

// ==========================================================================
// ï¿½Ö�ï¿½ï¿½ï¿½ï¿½FBOOL DeleteDirectory(LPCTSTR lpPathName)
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½@ï¿½í�œï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ìƒpï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½NULLï¿½Å�Iï¿½ï¿½é•¶ï¿½ï¿½ï¿½ï¿½B
// ï¿½ß‚ï¿½lï¿½Fï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ÈŠOï¿½Ì’lï¿½ï¿½ï¿½Ô‚ï¿½Aï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ï¿½ï¿½Ô‚ï¿½Ü‚ï¿½ï¿½B
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½Fï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½Û‚ï¿½ï¿½Æ�í�œï¿½ï¿½ï¿½ï¿½B
// ==========================================================================
BOOL CFileRetentionPeriod::DeleteDirectory(LPCTSTR lpPathName)
{
	char szBuff[256];
	CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	if(fnd.FindFile(strPathName, 0)){
		int i = 1;
		while(i){
			i = fnd.FindNextFile();

			// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"."ï¿½ï¿½".."ï¿½Ì�ê�‡ï¿½Í�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if(fnd.IsDots())
				continue;

			// ï¿½í�œï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½æ“¾
			// GetFilePath()ï¿½É‚Íƒoï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½è�³ï¿½mï¿½É�æ“¾ï¿½Å‚ï¿½ï¿½È‚ï¿½ï¿½ê�‡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�gï¿½ï¿½È‚ï¿½
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½Ä‹Aï¿½Ä‚Ñ�oï¿½ï¿½ï¿½Å‚ï¿½ï¿½Ìƒtï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½í�œ
			if(fnd.IsDirectory())
				DeleteDirectory(strDeleteFile);

			// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ì�í�œ
			else{
	            sprintf(szBuff, "[DeleteDirectory] Delete File %s", (LPCSTR)strDeleteFile);
				FileRetentionLogging(szBuff);
				::DeleteFile(strDeleteFile);
			}
		}
		fnd.Close();

		// ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½Ì�í�œ
        sprintf(szBuff, "[DeleteDirectory] Remove Directory %s", lpPathName);
		FileRetentionLogging(szBuff);
		return ::RemoveDirectory(lpPathName);
	}
	return FALSE;
}

// ==========================================================================
// ï¿½Ö�ï¿½ï¿½ï¿½ï¿½FBOOL DeleteAllData(LPCTSTR lpPathName)
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½@ï¿½í�œï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ìƒpï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½NULLï¿½Å�Iï¿½ï¿½é•¶ï¿½ï¿½ï¿½ï¿½B
// ï¿½ß‚ï¿½lï¿½Fï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ÈŠOï¿½Ì’lï¿½ï¿½ï¿½Ô‚ï¿½Aï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ï¿½ï¿½Ô‚ï¿½Ü‚ï¿½ï¿½B
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½Å�wï¿½è‚µï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì’ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½Sï¿½Ä�ï¿½ï¿½ï¿½
// ==========================================================================
BOOL CFileRetentionPeriod::DeleteAllData(LPCTSTR lpPathName)
{
	char szBuff[256];
    CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	if(fnd.FindFile(strPathName, 0)){
		int i = 1;
		while(i){
			i = fnd.FindNextFile();

			// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"."ï¿½ï¿½".."ï¿½Ì�ê�‡ï¿½Í�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if(fnd.IsDots())
				continue;

			// ï¿½í�œï¿½ï¿½ï¿½ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½æ“¾
			// GetFilePath()ï¿½É‚Íƒoï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½è�³ï¿½mï¿½É�æ“¾ï¿½Å‚ï¿½ï¿½È‚ï¿½ï¿½ê�‡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì‚Å�gï¿½ï¿½È‚ï¿½
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê�‡ï¿½Aï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if(fnd.IsDirectory())
				continue;

			// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½Ì�í�œ
			else{
	            sprintf(szBuff, "[DeleteAllData] Delete File %s", (LPCSTR)strDeleteFile);
				FileRetentionLogging(szBuff);
				::DeleteFile(strDeleteFile);
			}
		}
		fnd.Close();
		return TRUE;
	}
	return FALSE;
}

// ==========================================================================
// ï¿½Ö�ï¿½ï¿½ï¿½ï¿½FBOOL CheckDirectory(LPCTSTR lpPathName)
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½@ï¿½í�œï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ìƒpï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½NULLï¿½Å�Iï¿½ï¿½é•¶ï¿½ï¿½ï¿½ï¿½B
// ï¿½ß‚ï¿½lï¿½Fï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ÈŠOï¿½Ì’lï¿½ï¿½ï¿½Ô‚ï¿½Aï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ï¿½ï¿½Ô‚ï¿½Ü‚ï¿½ï¿½B
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½Å�wï¿½è‚µï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì’ï¿½ï¿½Éƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½é‚©ï¿½mï¿½F
// ==========================================================================
BOOL CFileRetentionPeriod::CheckDirectory(LPCTSTR lpPathName)
{
	char szBuff[256];
    CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	BOOL bRet = FALSE;

	if(fnd.FindFile(strPathName, 0)){
		int i = 1;
		bRet = TRUE;
		while(i){
			i = fnd.FindNextFile();

			// ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"."ï¿½ï¿½".."ï¿½Ì�ê�‡ï¿½Í�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			if(fnd.IsDots())
				continue;

			if(fnd.IsDirectory()){
				// ï¿½tï¿½Hï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½ï¿½ï¿½ï¿½
                    strcpy(m_szNextPath, fnd.GetFilePath());
                    bRet = FALSE;
					break;
			}
			else{
				//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â‚ï¿½ï¿½ï¿½ï¿½ï¿½
	            sprintf(szBuff, "[CheckDirectory] Delete File %s", (LPCSTR)fnd.GetFilePath());
				FileRetentionLogging(szBuff);
				bRet = ::DeleteFile(fnd.GetFilePath());
			}
		}
		fnd.Close();
		return bRet;
	}
	return TRUE;
}

// ==========================================================================
// ï¿½Ö�ï¿½ï¿½ï¿½ï¿½FBOOL CheckStressResultDir(LPCTSTR lpPathName)
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½@ï¿½í�œï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ìƒpï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½NULLï¿½Å�Iï¿½ï¿½é•¶ï¿½ï¿½ï¿½ï¿½B
// ï¿½ß‚ï¿½lï¿½Fï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ÈŠOï¿½Ì’lï¿½ï¿½ï¿½Ô‚ï¿½Aï¿½Ö�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½ï¿½Æ�A0ï¿½ï¿½ï¿½Ô‚ï¿½Ü‚ï¿½ï¿½B
// ï¿½ï¿½ï¿½@ï¿½ï¿½ï¿½FlpPathNameï¿½Å�wï¿½è‚µï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½Ì’ï¿½ï¿½Éƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½İ‚ï¿½ï¿½é‚©ï¿½mï¿½F
// ==========================================================================
BOOL CFileRetentionPeriod::CheckStressResultDir(LPCTSTR lpPathName)
{
	CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
    CString csFilePath;
	BOOL bStressData = FALSE;
    BOOL bFileEnd;
    char szDrive[16+1];
	char szFilePath[256+1];
	char szDirName[256+1];
	char szFileName[128+1];
	char szExtension[16+1];

	if(fnd.FindFile(strPathName, 0)){
		do{
			bFileEnd = fnd.FindNextFile();
			csFilePath = fnd.GetFilePath(); //ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½pï¿½Xï¿½æ“¾
			// "." , ".."ï¿½ğ–³�ï¿½
			if(fnd.IsDots()){
				continue;
			}

			//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê‚Ì”ï¿½ï¿½ï¿½
			if(fnd.IsDirectory()){
				continue;
			}

			strcpy(szFilePath, csFilePath);
			_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //ï¿½pï¿½Xï¿½ğ•ª‰ï¿½
			if(strcmp(szFileName, STRESS_REFERENCE_NAME)==0){
				//ï¿½tï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½%StressReference%ï¿½Ì�ê�‡ï¿½ÍˆÈ�~ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½
				bStressData = TRUE;
				break;
			}
		} while( bFileEnd );
		fnd.Close();
	}
	return bStressData;
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::SetPath(char* pszPath)
{
	memset(m_szPath, 0, sizeof(m_szPath));

	strncpy(m_szPath, pszPath, MAX_PATH);
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::ProcessLogTitle_Initialize()
{
	int i;
				// 2007.02.28 22:59:12.123
	char *Space = "                       ";
	char szBuff[MAX_PATH];

	memset(m_Process_LogTitle, 0, sizeof(m_Process_LogTitle));
	strcpy(m_Process_LogTitle, Space);
	for(i = 0 ; i < m_ProcessChangeMax ; i++){
		sprintf(szBuff, "  %02d ", i + 1);
		strcat(m_Process_LogTitle, szBuff);
	}
}

// ==========================================================================
//
// ==========================================================================
void CFileRetentionPeriod::ProcessLogging()
{
	char szBuff[1024];
	char strProcess[MAX_PATH];
	SYSTEMTIME systime;
	int i;
	int Change;

	GetLocalTime(&systime);

	Change = 0;
	for(i = 0 ; i < m_ProcessChangeMax ; i++){
		if(*m_pProcessChange[i] != m_Old_ProcessChange[i]){
			Change = 1;
		}
	}

	if(Change){
		sprintf(szBuff, "%04d.%02d.%02d %02d:%02d:%02d.%03d",
			systime.wYear,
			systime.wMonth,
			systime.wDay,
			systime.wHour,
			systime.wMinute,
			systime.wSecond,
			systime.wMilliseconds
		);

		for(i = 0 ; i < m_ProcessChangeMax ; i++){
			sprintf(strProcess, " %04d", *m_pProcessChange[i]);
			strcat(szBuff, strProcess);
		}

		m_pProcessLogFile.Logging(szBuff);
	}

	for(i = 0 ; i < m_ProcessChangeMax ; i++){
		m_Old_ProcessChange[i] = *m_pProcessChange[i];
	}
}

//Saiki 20100517 Add ----->
/////////////////////////////////////////////////////////////////////////////
//
void CFileRetentionPeriod::AddTime(char* lpszBuff)
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
void CFileRetentionPeriod::FileRetentionLogging(char* lpszLogText)
{
	char szBuff[1024];

	AddTime(szBuff);
	_tcscat(szBuff, " ");
	_tcscat(szBuff, lpszLogText);

	m_FRetentionLogFile.Logging(szBuff);
}
//Saiki 20100517 Add <-----
