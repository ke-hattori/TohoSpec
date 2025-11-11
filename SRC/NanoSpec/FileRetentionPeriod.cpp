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


	// 選択内容から年月取得
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

	//現在の時間を取得
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	// 現在日付からの削除年月
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


	//タイムスパンの計算
	CTimeSpan ts = CTime(systemTime.wYear, systemTime.wMonth, systemTime.wDay, 0, 0, 0) - CTime(iDeleteYear, iDeleteMonth, systemTime.wDay, 0, 0, 0);

	CTimeSpan timeSpan( ts.GetDays(),
						0,	//Hour
						0,	//Minute
						0); //Sec

	//削除期間が設定なし、もしくは設定されている項目以上でないか確認
	if(m_iDeleteDataPeriod > PeriodSearchTable[0].nSelect && DELETE_DATA_PERIOD_MAX > m_iDeleteDataPeriod){
//		bEnable = TRUE;
//	}
//
//	switch(*m_pProcess){
//	case 0://Processの開始
//		//if(bEnable == TRUE){
//			if(m_szPath[0] != '\0'){
//				*m_pProcess = 100;
//				break;
//			}
//		//}
//		break;
//
//	case 100:
//		//ファイルの検索
//		sWildCard.Format("%s*.*", m_szPath);
//        try{
//			m_bLoop = m_Finder.FindFile(sWildCard);
//		}
//        catch(CFileException* fe){
//            long ErrorCode = fe->m_lOsError;
//            sprintf_s(szBuff, _countof(szBuff), "(m_pProcess=100) %s, ErrorCode = %l", "File Invalid Error", ErrorCode);
//			FileRetentionLogging(szBuff);
//            break;
//        }
//
//		if(m_bLoop == 0){
//			//ファイルがない
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
//            sprintf_s(szBuff, _countof(szBuff), "(m_pProcess=200) %s, ErrorCode = %l", "File Invalid Error", ErrorCode);
//			FileRetentionLogging(szBuff);
//            break;
//        }
//
//		if(m_Finder.IsDots()) break;
//		if(m_Finder.IsDirectory()) break;
////		if(m_Finder.IsDirectory()){
////			//Stress測定データディレクトリか、個人作成ディレクトリの場合
////			*m_pProcess = 1000;
////			 break;
////		}
//
//		strcpy(m_szFilePath, m_Finder.GetFilePath());
//		_tsplitpath(m_szFilePath, szDrive, szDirName, szFileName, szExtension); //パスを分解
//		if(strcmp(szExtension, DAT_EXT) != 0) break;
//
//		retv = m_Finder.GetLastWriteTime(FileTime);
//
//		if(retv == 0) break;
//
//		sFileTimeString = FileTime.Format("%Y%m%d%H%M%S");
//		iFileTime = _atoi64(sFileTimeString.GetBuffer(0));
//		if(iFileTime < m_iLimitTime){
//			//検索したファイルが保管期限を過ぎている
//			*m_pProcess = 300;
//			break;
//		}
//
//        if(m_bLoop == 0){
//			//ファイルがない
//			*m_pProcess = 9000;
//			break;
//		}
//		break;
//	//検索したファイルに対応するディレクトリが存在するか
//	case 300:
//		sFileTitle = m_Finder.GetFileTitle();
//		sPathName.Format("%s%s", m_szPath, sFileTitle);
//		if(FileOrDirExists(sPathName) == FALSE){
//			//対応するディレクトリが存在しない
//			*m_pProcess = 400;
//		}
//		else{
//			//対応するディレクトリが存在する
//            strcpy(m_szNextPath, sPathName);
//			*m_pProcess = 500;
//		}
//	break;
//
//	//対応するディレクトリがない場合削除する(4PP, CTA測定データ) もしくは ディレクトリ削除後の測定データ
//	case 400:
//        ::DeleteFile(m_szFilePath);
//		if(m_bLoop == FALSE){
//			*m_pProcess = 9000;
//			break;
//		}
//		*m_pProcess = 200;
//	break;
//
//	//対応するディレクトリがある場合、まずディレクトリの中のファイルを全て削除する(SR, SE, 2Point Distance測定データ)
//	case 500:
//		if(DeleteAllData(m_szNextPath) == FALSE){
//			*m_pProcess = 8000;
//		}
//		else{
//			*m_pProcess = 600;
//		}
//	break;
//
//	//ディレクトリ内のデータを消したので、空ディレクトであれば消す
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
//	//階層を一つあがる
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
//	///// StressDataが入ったディレクトリか、個人作成フォルダ /////
//	case 1000:
//		if(CheckStressResultDir(m_Finder.GetFilePath())){
//			// Stress測定の結果データディレクトリ
//			*m_pProcess = 2000;
//		}
//		else{
//			// 個人作成のディレクトリ
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
		case 0://Processの開始
			//if(bEnable == TRUE){
				if(m_szPath[0] != '\0'){
					*m_pProcess = 100;
					break;
				}
			//}
			break;

		case 100:
			//ファイルの検索
			sWildCard.Format("%s*.*", m_szPath);
	        try{
				m_bLoop = m_Finder.FindFile(sWildCard);
			}
	        catch(CFileException* fe){
	            long ErrorCode = fe->m_lOsError;
	            sprintf_s(szBuff, _countof(szBuff), "(m_pProcess=100) %s, ErrorCode = %l", "File Invalid Error", ErrorCode);
				FileRetentionLogging(szBuff);
				*m_pProcess = 8000;
	            break;
	        }

			if(m_bLoop == 0){
				//ファイルがない
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
	            sprintf_s(szBuff, _countof(szBuff), "(m_pProcess=200) %s, ErrorCode = %l", "File Invalid Error", ErrorCode);
				FileRetentionLogging(szBuff);
				*m_pProcess = 8000;
	            break;
	        }

			if(m_Finder.IsDots()) break;
			if(m_Finder.IsDirectory()) break;
	//		if(m_Finder.IsDirectory()){
	//			//Stress測定データディレクトリか、個人作成ディレクトリの場合
	//			*m_pProcess = 1000;
	//			 break;
	//		}

			m_CurTime = CTime::GetCurrentTime();
			LimitTime = m_CurTime - timeSpan;
			sLimitTimeString = LimitTime.Format("%Y%m%d%H%M%S");
			m_iLimitTime = _atoi64(sLimitTimeString.GetBuffer(0));


			strcpy(m_szFilePath, m_Finder.GetFilePath());
			_tsplitpath(m_szFilePath, szDrive, szDirName, szFileName, szExtension); //パスを分解
			if(strcmp(szExtension, DAT_EXT) != 0) break;

			retv = m_Finder.GetLastWriteTime(FileTime);

			if(retv == 0) break;

			sFileTimeString = FileTime.Format("%Y%m%d%H%M%S");
			iFileTime = _atoi64(sFileTimeString.GetBuffer(0));
			if(iFileTime < m_iLimitTime){
				//検索したファイルが保管期限を過ぎている
				*m_pProcess = 300;
				break;
			}

	        if(m_bLoop == 0){
				//ファイルがない
				*m_pProcess = 9000;
				break;
			}
			break;
		//検索したファイルに対応するディレクトリが存在するか
		case 300:
			sFileTitle = m_Finder.GetFileTitle();
			sPathName.Format("%s%s", m_szPath, sFileTitle);
			if(FileOrDirExists(sPathName) == FALSE){
				//対応するディレクトリが存在しない
				*m_pProcess = 400;
			}
			else{
				//対応するディレクトリが存在する
	            strcpy(m_szNextPath, sPathName);
				*m_pProcess = 500;
			}
		break;

		//対応するディレクトリがない場合削除する(4PP, CTA測定データ) もしくは ディレクトリ削除後の測定データ
		case 400:
            sprintf_s(szBuff, _countof(szBuff), "[PROCESS_DirCheck] Delete File %s", m_szFilePath);
			FileRetentionLogging(szBuff);
	        ::DeleteFile(m_szFilePath);
			if(m_bLoop == FALSE){
				*m_pProcess = 9000;
				break;
			}
			*m_pProcess = 200;
		break;

		//対応するディレクトリがある場合、まずディレクトリの中のファイルを全て削除する(SR, SE, 2Point Distance測定データ)
		case 500:
			if(DeleteAllData(m_szNextPath) == FALSE){
				*m_pProcess = 8000;
			}
			else{
				*m_pProcess = 600;
			}
		break;

		//ディレクトリ内のデータを消したので、空ディレクトであれば消す
		case 600:
			if(CheckDirectory(m_szNextPath)){
	            sprintf_s(szBuff, _countof(szBuff), "[PROCESS_DirCheck] Remove Directory %s", m_szNextPath);
				FileRetentionLogging(szBuff);
				::RemoveDirectory(m_szNextPath);
				*m_pProcess = 700;
			}
			else{
				*m_pProcess = 500;
			}
		break;

		//階層を一つあがる
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

		///// StressDataが入ったディレクトリか、個人作成フォルダ /////
		case 1000:
			if(CheckStressResultDir(m_Finder.GetFilePath())){
				// Stress測定の結果データディレクトリ
				*m_pProcess = 2000;
			}
			else{
				// 個人作成のディレクトリ
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
//ファイル存在チェック
// ==========================================================================
BOOL CFileRetentionPeriod::FileOrDirExists(LPCTSTR pszFileName)
{
	return GetFileAttributes(pszFileName) != 0xffffffff;
}

// ==========================================================================
// 関数名：BOOL DeleteDirectory(LPCTSTR lpPathName)
// 引　数：lpPathName　削除するディレクトリのパス名が入ったNULLで終わる文字列。
// 戻り値：関数が成功すると、0以外の値が返り、関数が失敗すると、0が返ります。
// 解　説：ディレクトリを丸ごと削除する。
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

			// ファイル名が"."か".."の場合は次を検索
			if(fnd.IsDots())
				continue;

			// 削除するファイル名取得
			// GetFilePath()にはバグがあり正確に取得できない場合があるので使わない
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// フォルダだった場合、再帰呼び出しでそのフォルダを削除
			if(fnd.IsDirectory())
				DeleteDirectory(strDeleteFile);

			// ファイルの削除
			else{
	            sprintf_s(szBuff, _countof(szBuff), "[DeleteDirectory] Delete File %s", strDeleteFile);
				FileRetentionLogging(szBuff);
				::DeleteFile(strDeleteFile);
			}
		}
		fnd.Close();

		// フォルダの削除
        sprintf_s(szBuff, _countof(szBuff), "[DeleteDirectory] Remove Directory %s", lpPathName);
		FileRetentionLogging(szBuff);
		return ::RemoveDirectory(lpPathName);
	}
	return FALSE;
}

// ==========================================================================
// 関数名：BOOL DeleteAllData(LPCTSTR lpPathName)
// 引　数：lpPathName　削除するディレクトリのパス名が入ったNULLで終わる文字列。
// 戻り値：関数が成功すると、0以外の値が返り、関数が失敗すると、0が返ります。
// 解　説：lpPathNameで指定したディレクトリの中のファイルを全て消す
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

			// ファイル名が"."か".."の場合は次を検索
			if(fnd.IsDots())
				continue;

			// 削除するファイル名取得
			// GetFilePath()にはバグがあり正確に取得できない場合があるので使わない
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// フォルダだった場合、次を検索
			if(fnd.IsDirectory())
				continue;

			// ファイルの削除
			else{
	            sprintf_s(szBuff, _countof(szBuff), "[DeleteAllData] Delete File %s", strDeleteFile);
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
// 関数名：BOOL CheckDirectory(LPCTSTR lpPathName)
// 引　数：lpPathName　削除するディレクトリのパス名が入ったNULLで終わる文字列。
// 戻り値：関数が成功すると、0以外の値が返り、関数が失敗すると、0が返ります。
// 解　説：lpPathNameで指定したディレクトリの中にファイルやディレクトリが存在するか確認
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

			// ファイル名が"."か".."の場合は次を検索
			if(fnd.IsDots())
				continue;

			if(fnd.IsDirectory()){
				// フォルダが見つかった
                    strcpy(m_szNextPath, fnd.GetFilePath());
                    bRet = FALSE;
					break;
			}
			else{
				//ファイルが見つかった
	            sprintf_s(szBuff, _countof(szBuff), "[CheckDirectory] Delete File %s", fnd.GetFilePath());
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
// 関数名：BOOL CheckStressResultDir(LPCTSTR lpPathName)
// 引　数：lpPathName　削除するディレクトリのパス名が入ったNULLで終わる文字列。
// 戻り値：関数が成功すると、0以外の値が返り、関数が失敗すると、0が返ります。
// 解　説：lpPathNameで指定したディレクトリの中にファイルやディレクトリが存在するか確認
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
			csFilePath = fnd.GetFilePath(); //ファイルパス取得
			// "." , ".."を無視
			if(fnd.IsDots()){
				continue;
			}

			//検索結果の判定
			if(fnd.IsDirectory()){
				continue;
			}

			strcpy(szFilePath, csFilePath);
			_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //パスを分解
			if(strcmp(szFileName, STRESS_REFERENCE_NAME)==0){
				//ファイル名が%StressReference%の場合は以降の処理をしない
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
		sprintf_s(szBuff, _countof(szBuff), "  %02d ", i + 1);
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
		sprintf_s(szBuff, _countof(szBuff), "%04d.%02d.%02d %02d:%02d:%02d.%03d",
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
