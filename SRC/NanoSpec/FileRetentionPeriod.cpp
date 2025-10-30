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


	// �I����e����N���擾
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

	//���݂̎��Ԃ��擾
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	// ���ݓ��t����̍폜�N��
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


	//�^�C���X�p���̌v�Z
	CTimeSpan ts = CTime(systemTime.wYear, systemTime.wMonth, systemTime.wDay, 0, 0, 0) - CTime(iDeleteYear, iDeleteMonth, systemTime.wDay, 0, 0, 0);

	CTimeSpan timeSpan( static_cast<LONG>(ts.GetDays()),
					0,	//Hour
					0,	//Minute
					0); //Sec

	//�폜���Ԃ��ݒ�Ȃ��A�������͐ݒ肳��Ă��鍀�ڈȏ�łȂ����m�F
	if(m_iDeleteDataPeriod > PeriodSearchTable[0].nSelect && DELETE_DATA_PERIOD_MAX > m_iDeleteDataPeriod){
//		bEnable = TRUE;
//	}
//
//	switch(*m_pProcess){
//	case 0://Process�̊J�n
//		//if(bEnable == TRUE){
//			if(m_szPath[0] != '\0'){
//				*m_pProcess = 100;
//				break;
//			}
//		//}
//		break;
//
//	case 100:
//		//�t�@�C���̌���
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
//			//�t�@�C�����Ȃ�
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
////			//Stress����f�[�^�f�B���N�g�����A�l�쐬�f�B���N�g���̏ꍇ
////			*m_pProcess = 1000;
////			 break;
////		}
//
//		strcpy(m_szFilePath, m_Finder.GetFilePath());
//		_tsplitpath(m_szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
//		if(strcmp(szExtension, DAT_EXT) != 0) break;
//
//		retv = m_Finder.GetLastWriteTime(FileTime);
//
//		if(retv == 0) break;
//
//		sFileTimeString = FileTime.Format("%Y%m%d%H%M%S");
//		iFileTime = _atoi64(sFileTimeString.GetBuffer(0));
//		if(iFileTime < m_iLimitTime){
//			//���������t�@�C�����ۊǊ������߂��Ă���
//			*m_pProcess = 300;
//			break;
//		}
//
//        if(m_bLoop == 0){
//			//�t�@�C�����Ȃ�
//			*m_pProcess = 9000;
//			break;
//		}
//		break;
//	//���������t�@�C���ɑΉ�����f�B���N�g�������݂��邩
//	case 300:
//		sFileTitle = m_Finder.GetFileTitle();
//		sPathName.Format("%s%s", m_szPath, sFileTitle);
//		if(FileOrDirExists(sPathName) == FALSE){
//			//�Ή�����f�B���N�g�������݂��Ȃ�
//			*m_pProcess = 400;
//		}
//		else{
//			//�Ή�����f�B���N�g�������݂���
//            strcpy(m_szNextPath, sPathName);
//			*m_pProcess = 500;
//		}
//	break;
//
//	//�Ή�����f�B���N�g�����Ȃ��ꍇ�폜����(4PP, CTA����f�[�^) �������� �f�B���N�g���폜��̑���f�[�^
//	case 400:
//        ::DeleteFile(m_szFilePath);
//		if(m_bLoop == FALSE){
//			*m_pProcess = 9000;
//			break;
//		}
//		*m_pProcess = 200;
//	break;
//
//	//�Ή�����f�B���N�g��������ꍇ�A�܂��f�B���N�g���̒��̃t�@�C����S�č폜����(SR, SE, 2Point Distance����f�[�^)
//	case 500:
//		if(DeleteAllData(m_szNextPath) == FALSE){
//			*m_pProcess = 8000;
//		}
//		else{
//			*m_pProcess = 600;
//		}
//	break;
//
//	//�f�B���N�g�����̃f�[�^���������̂ŁA��f�B���N�g�ł���Ώ���
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
//	//�K�w���������
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
//	///// StressData���������f�B���N�g�����A�l�쐬�t�H���_ /////
//	case 1000:
//		if(CheckStressResultDir(m_Finder.GetFilePath())){
//			// Stress����̌��ʃf�[�^�f�B���N�g��
//			*m_pProcess = 2000;
//		}
//		else{
//			// �l�쐬�̃f�B���N�g��
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
		case 0://Process�̊J�n
			//if(bEnable == TRUE){
				if(m_szPath[0] != '\0'){
					*m_pProcess = 100;
					break;
				}
			//}
			break;

		case 100:
			//�t�@�C���̌���
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
				//�t�@�C�����Ȃ�
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
	//			//Stress����f�[�^�f�B���N�g�����A�l�쐬�f�B���N�g���̏ꍇ
	//			*m_pProcess = 1000;
	//			 break;
	//		}

			m_CurTime = CTime::GetCurrentTime();
			LimitTime = m_CurTime - timeSpan;
			sLimitTimeString = LimitTime.Format("%Y%m%d%H%M%S");
			m_iLimitTime = _atoi64(sLimitTimeString.GetBuffer(0));


			strcpy(m_szFilePath, m_Finder.GetFilePath());
			_tsplitpath(m_szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
			if(strcmp(szExtension, DAT_EXT) != 0) break;

			retv = m_Finder.GetLastWriteTime(FileTime);

			if(retv == 0) break;

			sFileTimeString = FileTime.Format("%Y%m%d%H%M%S");
			iFileTime = _atoi64(sFileTimeString.GetBuffer(0));
			if(iFileTime < m_iLimitTime){
				//���������t�@�C�����ۊǊ������߂��Ă���
				*m_pProcess = 300;
				break;
			}

	        if(m_bLoop == 0){
				//�t�@�C�����Ȃ�
				*m_pProcess = 9000;
				break;
			}
			break;
		//���������t�@�C���ɑΉ�����f�B���N�g�������݂��邩
		case 300:
			sFileTitle = m_Finder.GetFileTitle();
			sPathName.Format("%s%s", m_szPath, sFileTitle);
			if(FileOrDirExists(sPathName) == FALSE){
				//�Ή�����f�B���N�g�������݂��Ȃ�
				*m_pProcess = 400;
			}
			else{
				//�Ή�����f�B���N�g�������݂���
	            strcpy(m_szNextPath, sPathName);
				*m_pProcess = 500;
			}
		break;

		//�Ή�����f�B���N�g�����Ȃ��ꍇ�폜����(4PP, CTA����f�[�^) �������� �f�B���N�g���폜��̑���f�[�^
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

		//�Ή�����f�B���N�g��������ꍇ�A�܂��f�B���N�g���̒��̃t�@�C����S�č폜����(SR, SE, 2Point Distance����f�[�^)
		case 500:
			if(DeleteAllData(m_szNextPath) == FALSE){
				*m_pProcess = 8000;
			}
			else{
				*m_pProcess = 600;
			}
		break;

		//�f�B���N�g�����̃f�[�^���������̂ŁA��f�B���N�g�ł���Ώ���
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

		//�K�w���������
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

		///// StressData���������f�B���N�g�����A�l�쐬�t�H���_ /////
		case 1000:
			if(CheckStressResultDir(m_Finder.GetFilePath())){
				// Stress����̌��ʃf�[�^�f�B���N�g��
				*m_pProcess = 2000;
			}
			else{
				// �l�쐬�̃f�B���N�g��
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
//�t�@�C�����݃`�F�b�N
// ==========================================================================
BOOL CFileRetentionPeriod::FileOrDirExists(LPCTSTR pszFileName)
{
	return GetFileAttributes(pszFileName) != 0xffffffff;
}

// ==========================================================================
// �֐����FBOOL DeleteDirectory(LPCTSTR lpPathName)
// ���@���FlpPathName�@�폜����f�B���N�g���̃p�X����������NULL�ŏI��镶����B
// �߂�l�F�֐�����������ƁA0�ȊO�̒l���Ԃ�A�֐������s����ƁA0���Ԃ�܂��B
// ���@���F�f�B���N�g�����ۂ��ƍ폜����B
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

			// �t�@�C������"."��".."�̏ꍇ�͎�������
			if(fnd.IsDots())
				continue;

			// �폜����t�@�C�����擾
			// GetFilePath()�ɂ̓o�O�����萳�m�Ɏ擾�ł��Ȃ��ꍇ������̂Ŏg��Ȃ�
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// �t�H���_�������ꍇ�A�ċA�Ăяo���ł��̃t�H���_���폜
			if(fnd.IsDirectory())
				DeleteDirectory(strDeleteFile);

			// �t�@�C���̍폜
			else{
	            sprintf(szBuff, "[DeleteDirectory] Delete File %s", (LPCSTR)strDeleteFile);
				FileRetentionLogging(szBuff);
				::DeleteFile(strDeleteFile);
			}
		}
		fnd.Close();

		// �t�H���_�̍폜
        sprintf(szBuff, "[DeleteDirectory] Remove Directory %s", lpPathName);
		FileRetentionLogging(szBuff);
		return ::RemoveDirectory(lpPathName);
	}
	return FALSE;
}

// ==========================================================================
// �֐����FBOOL DeleteAllData(LPCTSTR lpPathName)
// ���@���FlpPathName�@�폜����f�B���N�g���̃p�X����������NULL�ŏI��镶����B
// �߂�l�F�֐�����������ƁA0�ȊO�̒l���Ԃ�A�֐������s����ƁA0���Ԃ�܂��B
// ���@���FlpPathName�Ŏw�肵���f�B���N�g���̒��̃t�@�C����S�ď���
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

			// �t�@�C������"."��".."�̏ꍇ�͎�������
			if(fnd.IsDots())
				continue;

			// �폜����t�@�C�����擾
			// GetFilePath()�ɂ̓o�O�����萳�m�Ɏ擾�ł��Ȃ��ꍇ������̂Ŏg��Ȃ�
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// �t�H���_�������ꍇ�A��������
			if(fnd.IsDirectory())
				continue;

			// �t�@�C���̍폜
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
// �֐����FBOOL CheckDirectory(LPCTSTR lpPathName)
// ���@���FlpPathName�@�폜����f�B���N�g���̃p�X����������NULL�ŏI��镶����B
// �߂�l�F�֐�����������ƁA0�ȊO�̒l���Ԃ�A�֐������s����ƁA0���Ԃ�܂��B
// ���@���FlpPathName�Ŏw�肵���f�B���N�g���̒��Ƀt�@�C����f�B���N�g�������݂��邩�m�F
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

			// �t�@�C������"."��".."�̏ꍇ�͎�������
			if(fnd.IsDots())
				continue;

			if(fnd.IsDirectory()){
				// �t�H���_����������
                    strcpy(m_szNextPath, fnd.GetFilePath());
                    bRet = FALSE;
					break;
			}
			else{
				//�t�@�C������������
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
// �֐����FBOOL CheckStressResultDir(LPCTSTR lpPathName)
// ���@���FlpPathName�@�폜����f�B���N�g���̃p�X����������NULL�ŏI��镶����B
// �߂�l�F�֐�����������ƁA0�ȊO�̒l���Ԃ�A�֐������s����ƁA0���Ԃ�܂��B
// ���@���FlpPathName�Ŏw�肵���f�B���N�g���̒��Ƀt�@�C����f�B���N�g�������݂��邩�m�F
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
			csFilePath = fnd.GetFilePath(); //�t�@�C���p�X�擾
			// "." , ".."�𖳎�
			if(fnd.IsDots()){
				continue;
			}

			//�������ʂ̔���
			if(fnd.IsDirectory()){
				continue;
			}

			strcpy(szFilePath, csFilePath);
			_tsplitpath(szFilePath, szDrive, szDirName, szFileName, szExtension); //�p�X�𕪉�
			if(strcmp(szFileName, STRESS_REFERENCE_NAME)==0){
				//�t�@�C������%StressReference%�̏ꍇ�͈ȍ~�̏��������Ȃ�
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
