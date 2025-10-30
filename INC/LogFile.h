#ifndef __LOG_FILE_H__
#define __LOG_FILE_H__

#pragma once

#define LOG_FILE_MAX_FILE_SIZE					(500000)
#define LOG_FILE_MAX_FILE_NUM					(50)

//===========================================================================
// class CLogFile
//===========================================================================
class CLogFile
{
public:
	CLogFile(LPSTR lpszPathName, LPSTR lpszFileName, DWORD dwMaxFileSize = LOG_FILE_MAX_FILE_SIZE, DWORD dwMaxFileNum = LOG_FILE_MAX_FILE_NUM, LPSTR lpszTitle = NULL);
	~CLogFile();

	void Logging(LPCTSTR lpszBuff);
	void LoggingV(LPCTSTR lpszFormat, ...);

private:
	TCHAR m_szPathName[MAX_PATH];
	TCHAR m_szFileName[MAX_PATH];
	TCHAR m_szTitleName[256 + 1];
	DWORD m_dwMaxFileSize;
	DWORD m_dwMaxFileNum;

	BOOL IsOverFileSize(LPCTSTR lpszFileName);
	BOOL Rename(DWORD dwFileNum);
};

#endif // __LOG_FILE_H__
