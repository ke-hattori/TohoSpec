#ifndef __LOG_FILE_H__
#define __LOG_FILE_H__

#pragma once

namespace TOHO
{

#define LOG_FILE_MAX_SIZE						(500000)
#define LOG_FILE_MAX_NUM						(50)
#define LOG_FILE_EXT							_TEXT(".log")

#define CRCHAR									TCHAR('\r')		/* carriage return */
#define LFCHAR									TCHAR('\n')		/* line feed */
#define NULLCHAR								TCHAR('\0')
#define SPACECHAR								TCHAR(' ')
#define TABCHAR									TCHAR('\t')
#define DQUOTECHAR								TCHAR('\"')
#define SLASHCHAR								TCHAR('\\')
#define CR										TEXT("\r")
#define LF										TEXT("\n")
#define CRLF									TEXT("\r\n")
#define SPACE									TEXT(" ")
#define TAB										TEXT("\t")
#define BLANK									TEXT("")

/*
#define PERMIT_FILE_SHARE_WRITE
*/

//===========================================================================
// class CLogFile
//===========================================================================
class CLogFile
{
public:
	CLogFile() {::InitializeCriticalSection(&m_csWait);}
	CLogFile(PSTR pszPathName, PSTR pszFileName, DWORD dwMaxFileSize = LOG_FILE_MAX_SIZE,
		DWORD dwMaxFileNum = LOG_FILE_MAX_NUM, PCSTR pszTitle = NULL);
	~CLogFile();

	void Create(PSTR pszPathName, PSTR pszFileName, DWORD dwMaxFileSize = LOG_FILE_MAX_SIZE,
		DWORD dwMaxFileNum = LOG_FILE_MAX_NUM, PCSTR pszTitle = NULL);
	void Logging(PCSTR pszBuff);
	void LoggingV(PCSTR pszFormat, ...);

	inline void Lock(void)	{::EnterCriticalSection(&m_csWait);}
	inline void Unlock(void) {::LeaveCriticalSection(&m_csWait);}

	// misc
	BOOL IsExist(DWORD);
//	void GetHistory(std::list<std::string>&, UINT);

private:
	TCHAR m_szPathName[MAX_PATH];
	TCHAR m_szFileName[MAX_PATH];
//	std::string m_strTitleName;
	TCHAR m_szTitleName[MAX_PATH];
	DWORD m_dwMaxFileSize;
	DWORD m_dwMaxFileNum;

	BOOL IsOver(PCSTR pszFileName);
	BOOL Rename(DWORD dwFileNum);

	CRITICAL_SECTION m_csWait;
};

}; // namespace TOHO

#endif // __LOG_FILE_H__
