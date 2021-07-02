// LogFile.cpp (LogFile.h)
//

#include "stdAfx.h"
#include "LogFile.h"

//#define LOGFILE_SIZE_MAX	100000
//#define LOGFILE_SIZE_MAX	500000
#define LOGFILE_SIZE_MAX	1000000
#define LOGFILE_NUM_MAX		30

//////////////////////////////////////////////////////////////
//		CLogFile
CLogFile::CLogFile(TCHAR* ptszLogFileName)
{
	strcpy(m_tszLogFileName, ptszLogFileName);
}

CLogFile::~CLogFile()
{

}

///////////////////////////////////////////////////////////////////////////////
// Logging
void CLogFile::Logging(TCHAR* ptszString)
{
	TCHAR l_tszBuff[256];
	CFile l_file;
	TCHAR l_tszFileName[_MAX_PATH];

	///// Log File Checking /////
	CheckLogFiles();

	///// Log File Saving /////
	_stprintf(l_tszFileName, _T("%s%s.001"), m_tszLogFilePath, m_tszLogFileName);
	_stprintf(l_tszBuff, _T("%s\x0d\x0a"), ptszString);
	m_file.Open(l_tszFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	m_file.SeekToEnd();
	m_file.Write(l_tszBuff, _tcslen(l_tszBuff));
	m_file.Close();
}

///////////////////////////////////////////////////////////////////////////////
// Check Number of Files
//   (Parameters)
//     char* LogFileName;
//     int FileSize;
//     int Max;
void CLogFile::CheckLogFiles(void)
{
	CFileStatus l_FileStatus;
	WIN32_FIND_DATA l_FindData;
	TCHAR l_tszFileName[_MAX_PATH];
	TCHAR l_tszReName[_MAX_PATH];
	HANDLE l_Handle;
	int l_FileNo, l_MaxNo, l_Index;

	_stprintf(l_tszFileName, _T("%s%s.001"), m_tszLogFilePath, m_tszLogFileName);
	if(!CFile::GetStatus(l_tszFileName, l_FileStatus)){
		::CloseHandle(CreateFile(l_tszFileName, 0, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL));
	}
	else{
		if (l_FileStatus.m_size > LOGFILE_SIZE_MAX){
			_stprintf(l_tszFileName, _T("%s%s.*"), m_tszLogFilePath, m_tszLogFileName);
			l_Handle = FindFirstFile(l_tszFileName, &l_FindData);
			l_MaxNo = 0;
			do {
				l_FileNo = _ttoi(&l_FindData.cFileName[strlen(l_FindData.cFileName) - 3]);
				l_MaxNo = __max(l_MaxNo, l_FileNo);
			} while (FindNextFile(l_Handle, &l_FindData) != 0);
			::FindClose(l_Handle);

			for(l_Index = l_MaxNo; l_Index > 0; l_Index--){
				_stprintf(l_tszFileName, _T("%s%s.%03d"), m_tszLogFilePath, m_tszLogFileName, l_Index);
				if (l_Index >= LOGFILE_NUM_MAX){
					::DeleteFile(l_tszFileName);
				}
				else{
					_stprintf(l_tszReName, _T("%s%s.%03d"), m_tszLogFilePath, m_tszLogFileName, l_Index + 1);
					if (CFile::GetStatus(l_tszFileName, l_FileStatus))
						CFile::Rename(l_tszFileName, l_tszReName);
				}
			}
		}
	}
}

// 文字列中の ''-'z' 以外のデータをバイナリ表記に変換する
//		0x19 ---> [19]
//		文字列 Src の長さが 511 バイト以上の場合は FALSE リターン
BOOL CLogFile::ConvBinToHex(TCHAR *ptszSrc, TCHAR *ptszDst, DWORD dwLength)
{
	int		l_iSrcIndex;
	int		l_iDstIndex;
	TCHAR	l_tszBuff[2048];
	int		l_icnt;

	ptszDst[0] = '\0';
//	int Len = strlen(Src);
	int l_iLen = dwLength;
	if (511 <= l_iLen) {
		return FALSE;
	}

	for (l_iSrcIndex = 0, l_iDstIndex = 0; l_iSrcIndex < l_iLen; l_iSrcIndex++) {
		// in case of code from ' ' to 'z'. (0x20 to 0x7a)
		if (ptszSrc[l_iSrcIndex] >= _TCHAR(' ') && ptszSrc[l_iSrcIndex] <= _TCHAR('z')) {
		//if(isprint(Src[SrcIndex])){
			ptszDst[l_iDstIndex] = ptszSrc[l_iSrcIndex];
			ptszDst[l_iDstIndex + 1] = '\0';
		}
		// in case of code is NULL
		else if (ptszSrc[l_iSrcIndex] == 0x00) {
			_stprintf(l_tszBuff, _T("[00]"));
			l_icnt = _tcslen(l_tszBuff);
			_tcscat(&ptszDst[l_iDstIndex], l_tszBuff);
			if (l_icnt > 0) {
				l_iDstIndex = l_iDstIndex + (l_icnt - 1);
			}
		}
		// in case of code except above
		else {
			_stprintf(l_tszBuff, _T("[%02X]"), (unsigned char) ptszSrc[l_iSrcIndex]);
			l_icnt = _tcslen(l_tszBuff);
			_tcscat(&ptszDst[l_iDstIndex], l_tszBuff);
			if (l_icnt > 0){
				l_iDstIndex = l_iDstIndex + (l_icnt - 1);
			}
		}
		l_iDstIndex++;
	}
	ptszDst[l_iDstIndex] = _TCHAR('\0');

	return TRUE;
}

// 現在時刻，日付を追加する
//		文字列 cData の長さが 511 バイト以上の場合は FALSE リターン
BOOL CLogFile::AddTime(TCHAR *ptszData, short mode)
{
/* deleted 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- { ---------- */
//	TCHAR l_tszBuff[32];
/* deleted 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- } ---------- */

	int l_Len = _tcslen(ptszData);
	if (511 <= l_Len) {
		return FALSE;
	}

/* modified 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- { ---------- */
//	CTime l_time = CTime::GetCurrentTime();
//	switch (mode) {
//	case 0:		// 時刻のみ
//		_tcscat(ptszData, l_time.Format("%H:%M:%S"));
//		break;
//	case 1:		// 日付のみ
//		_stprintf(l_tszBuff, _T("%04d.%02d.%02d"), l_time.GetYear(), l_time.GetMonth(), l_time.GetDay());
//		_tcscat(ptszData, l_tszBuff);
//		break;
//	case 2:		// 日付と時刻
//		_stprintf(l_tszBuff, _T("%04d.%02d.%02d %s"), l_time.GetYear(), l_time.GetMonth(), l_time.GetDay(), l_time.Format("%H:%M:%S"));
//		_tcscat(ptszData, l_tszBuff);
//		break;
//	default:	// パラメタエラー
//		return FALSE;
//		break;
//	}
/* modified 2009.12.14 hmenjo MotSys DIO ログ追加 ----------			  */
	SYSTEMTIME l_SystemTime;
	::GetLocalTime(&l_SystemTime);
	switch (mode) {
	case 0:		/* 時刻のみ	*/
		_stprintf(ptszData, _T("%02d:%02d:%02d"), l_SystemTime.wHour, l_SystemTime.wMinute, l_SystemTime.wSecond);
		break;
	case 1:		/* 日付のみ	*/
		_stprintf(ptszData, _T("%04d:%02d:%02d"), l_SystemTime.wYear, l_SystemTime.wMonth, l_SystemTime.wDay);
		break;
	case 2:		/* 日付と時刻	*/
		_stprintf(ptszData, _T("%04d:%02d:%02d %02d:%02d:%02d"),
						l_SystemTime.wYear, l_SystemTime.wMonth, l_SystemTime.wDay,
						l_SystemTime.wHour, l_SystemTime.wMinute, l_SystemTime.wSecond);
		break;
	case 3:		/* 時刻(ms)のみ	*/
		_stprintf(ptszData, _T("%02d:%02d:%02d.%03d"), l_SystemTime.wHour, l_SystemTime.wMinute, l_SystemTime.wSecond, l_SystemTime.wMilliseconds);
		break;
	case 4:		/* 日付と時刻(ms)	*/
		_stprintf(ptszData, _T("%04d:%02d:%02d %02d:%02d:%02d.%03d"),
						l_SystemTime.wYear, l_SystemTime.wMonth, l_SystemTime.wDay,
						l_SystemTime.wHour, l_SystemTime.wMinute, l_SystemTime.wSecond, l_SystemTime.wMilliseconds);
		break;
	default:	// パラメタエラー
		return FALSE;
		break;
	}
/* modified 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- } ---------- */

	return TRUE;
}

// ログをクリア
void CLogFile::ClearLog()
{
	TCHAR l_tszFileName[_MAX_PATH];
	TCHAR l_tszText[80];

	_stprintf(l_tszFileName, _T("%s%s.*"), m_tszLogFilePath, m_tszLogFileName);
	HANDLE	l_hFind;
	WIN32_FIND_DATA	l_FindFileData;
	l_hFind = FindFirstFile(l_tszFileName, &l_FindFileData);
	if (l_hFind != INVALID_HANDLE_VALUE) {
		do {
			_stprintf(l_tszFileName, _T("%s%s"), m_tszLogFilePath, l_FindFileData.cFileName);
			if (::DeleteFile(l_tszFileName) == 0) {
				_stprintf(l_tszText, _T("Could not delete Com log file[%s]"), l_tszFileName);
				::MessageBox(NULL, l_tszText, _T("Delete Com Log"), MB_OK | MB_SYSTEMMODAL);
			}
		} while (::FindNextFile(l_hFind, &l_FindFileData) != 0);
	}
}
// matsuhisa 20090224 add >>>
/////////////////////////////////////////////////////////////////////////////
// CLogFileEx
CLogFileEx::CLogFileEx(TCHAR* ptszPath, TCHAR* ptszFileName, int Size, int Max, TCHAR* ptszTitle, TCHAR* ptszTitleEx[], int TitleExRowMax)
{
	int i;

	_tcscpy(m_tszPath, ptszPath);
	_tcscpy(m_tszFileName, ptszFileName);
	m_Size = Size;
	m_Max = Max;
	memset(m_tszTitle, 0, sizeof(m_tszTitle));
	memset(m_tszTitleEx, 0, sizeof(m_tszTitleEx));
	m_TitleExRowMax = 0;

	if(ptszTitle != NULL){
		_tcscpy(m_tszTitle, ptszTitle);
		_tcscat(m_tszTitle, _T("\x0d\x0a"));
	}

	if(ptszTitleEx != NULL){
		m_TitleExRowMax = (TitleExRowMax > TITLE_EX_ROW_MAX) ? TITLE_EX_ROW_MAX : TitleExRowMax;
		for(i = 0 ; i < m_TitleExRowMax ; i++){
			_tcscpy(m_tszTitleEx[i], ptszTitleEx[i]);
			_tcscat(m_tszTitleEx[i], _T("\x0d\x0a"));
		}
	}

	m_hMutex = CreateMutex(NULL,FALSE,NULL);
	ReleaseMutex(m_hMutex);
}

/////////////////////////////////////////////////////////////////////////////
//
CLogFileEx::~CLogFileEx()
{
	::ReleaseMutex(m_hMutex);
	::CloseHandle(m_hMutex);
}

/////////////////////////////////////////////////////////////////////////////
// Logging
void CLogFileEx::Logging(TCHAR* ptszString)
{
	CFile file;
	TCHAR l_tszFileName[_MAX_PATH];
	int retv;
	int i;

	::WaitForSingleObject(m_hMutex, 5000);

	///// Log File Checking /////
	retv = CheckLogFiles();

	///// Log File Saving /////
	_stprintf(l_tszFileName, _T("%s\\%s001") FILE_EXTENSION, m_tszPath, m_tszFileName);

	if(!file.Open(l_tszFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	file.SeekToEnd();

	if(retv){
		if(m_tszTitle[0]){
			file.Write(m_tszTitle, strlen(m_tszTitle));
		}

		if(m_tszTitleEx[0][0]){
			for(i = 0 ; i < m_TitleExRowMax ; i++){
				file.Write(m_tszTitleEx[i], _tcslen(m_tszTitleEx[i]));
			}
		}
	}

	_stprintf(m_tszBuff, _T("%s\x0d\x0a"), ptszString);
	file.Write(m_tszBuff, _tcslen(m_tszBuff));

	file.Close();

	::ReleaseMutex(m_hMutex);
}

/////////////////////////////////////////////////////////////////////////////
//
int CLogFileEx::CheckLogFiles(void)
{
	CFileStatus status;
	TCHAR l_tszfname[256];
	TCHAR l_tszrename[256];
	int Index;
	int retv;

	// Make File Name
	_stprintf(l_tszfname, _T("%s\\%s001") FILE_EXTENSION, m_tszPath, m_tszFileName);

	// Check File Exist and Get File Sattus
	if (!CFile::GetStatus(l_tszfname, status))
		return 1;					// No Exist

	// Check File Size
	if (status.m_size < m_Size)
		return 0;					// File Size OK

	// Delete Max File
	_stprintf(l_tszfname, _T("%s\\%s%03d") FILE_EXTENSION, m_tszPath, m_tszFileName, m_Max);
	::DeleteFile(l_tszfname);

	// Rename Log File
	retv = 0;
	for(Index = m_Max-1; Index >= 1; Index--){
		_stprintf(l_tszfname, _T("%s\\%s%03d") FILE_EXTENSION, m_tszPath, m_tszFileName, Index);
		if (CFile::GetStatus(l_tszfname, status)){			// Exist?
			_stprintf(l_tszrename, _T("%s\\%s%03d") FILE_EXTENSION, m_tszPath, m_tszFileName, (Index+1));
			CFile::Rename(l_tszfname, l_tszrename);
			retv = 1;
		}
	}
	return retv;
}
// matsuhisa 20090224 add <<<
