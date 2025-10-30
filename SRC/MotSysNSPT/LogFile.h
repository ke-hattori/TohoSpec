// LogFile.h (LogFile.cpp)
// CLogFile Class

#ifndef _LOGFILE_H_
#define _LOGFILE_H_

class CLogFile
{
public:
	void ClearLog(void);
	BOOL AddTime(TCHAR *ptszData, short mode);
	BOOL ConvBinToHex(TCHAR *ptszSrc, TCHAR *ptszDst, DWORD dwLength);
	TCHAR m_tszLogFilePath[_MAX_PATH];
	CLogFile(TCHAR* ptszLogFileName);
	~CLogFile();

	CFile m_file;
	TCHAR m_tszLogFileName[256];

	void Logging(TCHAR* ptszString);
	void CheckLogFiles(void);
};
// matsuhisa 20090224 add >>>
#define FILE_EXTENSION		_T(".log")
#define TITLE_EX_ROW_MAX	10

class CLogFileEx
{
public:
	CLogFileEx(TCHAR* ptszPath, TCHAR* ptszFileName, int Size, int Max, TCHAR* ptszTitle, TCHAR* ptszTitleEx[], int TitleExRowMax);
	~CLogFileEx();

	CFile m_file;
	TCHAR m_tszPath[256];
	TCHAR m_tszFileName[256];
	int m_Size;
	int m_Max;
	TCHAR m_tszTitle[1024];
	TCHAR m_tszTitleEx[TITLE_EX_ROW_MAX][10240];
	char m_TitleExRowMax;
	TCHAR m_tszBuff[10240];
	HANDLE m_hMutex;

	void Logging(TCHAR* ptszString);
	int CheckLogFiles(void);
};

// matsuhisa 20090224 add <<<
#endif // _HIFLOGFILE_H_
