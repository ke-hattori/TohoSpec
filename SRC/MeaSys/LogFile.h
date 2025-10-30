#pragma once

#include <stdlib.h>
#include <tchar.h>

#define LOGFILE_SIZEMAX 		(5 * 1024 * 1024)
#define LOGFILE_NUMMAX			(30)
#define LOGFILE_DIR 			_T("..\\Log\\")
#define LOGFILE_EXT 			_T(".log")


class CLogFile
{
public:
	CLogFile(LPCTSTR pszFileName, LPCTSTR pszDirectoryName = LOGFILE_DIR, int nSizeMax = LOGFILE_SIZEMAX, int nNumMax = LOGFILE_NUMMAX)
		: m_nSizeMax(nSizeMax), m_nNumMax(nNumMax)
	{
		::InitializeCriticalSection(&m_csWait);

		if ( !IsExist(pszDirectoryName) )
			::CreateDirectory(pszDirectoryName, NULL);
		_tcscpy(m_szPathName, pszDirectoryName);
		_tcscat(m_szPathName, pszFileName);
	}

	virtual ~CLogFile()
	{
		::EnterCriticalSection(&m_csWait);
		::DeleteCriticalSection(&m_csWait);
	}

	void Logging(LPCTSTR psz)
	{
		::EnterCriticalSection(&m_csWait);

		if ( IsSizeOver() )
			RenameFile();

		CString strName;
		CString strLogText;
		char szBuff[256];
		SYSTEMTIME systemTime;

		strName.Format(_T("%s%s%s") ,m_szPathName, _T("001"), LOGFILE_EXT);

		::GetLocalTime(&systemTime);
		sprintf(szBuff, "%04d.%02d.%02d %02d:%02d:%02d.%03d ",
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour,
			systemTime.wMinute,
			systemTime.wSecond,
			systemTime.wMilliseconds);
		strLogText.Format(_T("%s%s\n"), szBuff, psz);

		CStdioFile stdioFile;
		if ( !stdioFile.Open(strName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) ) {
			::MessageBox(NULL, _T("CLogFile Open Error"), m_szPathName, MB_OK | MB_ICONERROR);
			goto CleanUp;
		}
		stdioFile.SeekToEnd();
		stdioFile.WriteString(strLogText);
		stdioFile.Close();

	CleanUp:
		::LeaveCriticalSection(&m_csWait);
	}

	void LoggingV(LPCTSTR pszFormat, ...)
	{
		CString strBuffer;

	   // format and write the data we were given
		va_list args;
		va_start(args, pszFormat);
		strBuffer.FormatV(pszFormat, args);

		Logging(strBuffer);
	}

protected:
	BOOL IsSizeOver()
	{
		TCHAR szPathName[MAX_PATH];
		CFileStatus fileStatus;

		_stprintf(szPathName, _T("%s%s%s") ,m_szPathName, _T("001"), LOGFILE_EXT);
		if ( !IsExist(szPathName) )
			return FALSE;
		if ( !CFile::GetStatus(szPathName, fileStatus) )
			return FALSE;
		return ( fileStatus.m_size >= m_nSizeMax );
	}

	void RenameFile()
	{
		CString strName;
		CString strNewName;

		strName.Format(_T("%s%03d%s"), m_szPathName, m_nNumMax, LOGFILE_EXT);
		if ( IsExist(strName) )
			CFile::Remove(strName);

		for ( int i = m_nNumMax - 1; i > 0; i-- ) {
			strName.Format(_T("%s%03d%s"), m_szPathName, i, LOGFILE_EXT);
			strNewName.Format(_T("%s%03d%s"), m_szPathName, i + 1, LOGFILE_EXT);
			if ( IsExist(strName) )
				CFile::Rename(strName, strNewName);
		}
	}

	inline BOOL IsExist(LPCTSTR psz) {return ::GetFileAttributes(psz) != 0xffffffff;}

private:
	CRITICAL_SECTION m_csWait;
	TCHAR m_szPathName[MAX_PATH];
	int m_nSizeMax;
	int m_nNumMax;
};

/////////////////////////////////////////////////////////////////////////////
