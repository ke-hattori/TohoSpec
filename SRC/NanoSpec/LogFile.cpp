#define WIN32_LEAN_AND_MEAN

#pragma warning (disable:4786)

#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include <string>
#include <time.h>
#include <bitset>
#include <list>

//===========================================================================
// LogFile.cpp (LogFile.h)
//===========================================================================
#include "stdafx.h"
#include "Common.h"
#include "LogFile.h"

using namespace TOHO;

//===========================================================================
// Construction/Destruction
//===========================================================================
CLogFile::CLogFile(PSTR pszPathName, PSTR pszFileName,
	DWORD dwMaxFileSize/*=500000*/, DWORD dwMaxFileNum/*=50*/, PCSTR pszTitleName/*=NULL*/)
{
	::InitializeCriticalSection(&m_csWait);

	Create(pszPathName, pszFileName, dwMaxFileSize, dwMaxFileNum, pszTitleName);
}

CLogFile::~CLogFile()
{
	::EnterCriticalSection(&m_csWait);
	::DeleteCriticalSection(&m_csWait);
}

// =========================================================================
// Create
void CLogFile::Create(PSTR pszPathName, PSTR pszFileName,
	DWORD dwMaxFileSize/*=500000*/, DWORD dwMaxFileNum/*=50*/, PCSTR pszTitleName/*=NULL*/)
{
	_tcscpy(m_szPathName, pszPathName);
	_tcscpy(m_szFileName, pszFileName);

	m_dwMaxFileSize = dwMaxFileSize;
	m_dwMaxFileNum	= dwMaxFileNum;

//	if ( pszTitleName != NULL )
//		m_strTitleName = pszTitleName;
	memset(m_szTitleName, 0, sizeof(m_szTitleName));
	if ( pszTitleName != NULL )
		_tcscpy(m_szTitleName, pszTitleName);
}
// =========================================================================
// Logging
void CLogFile::Logging(PCSTR pszBuffer)
{
	BOOL bRet = TRUE;
	TCHAR szFileName[MAX_PATH];
	HANDLE hFile;
	DWORD dwNumBytesWritten;
	const TCHAR szCRLF[] = CRLF;

	_ASSERT(pszBuffer);

	Lock();

	_stprintf(szFileName, _TEXT("%s\\%s001") LOG_FILE_EXT,
		m_szPathName, m_szFileName);
	if ( IsOver(szFileName) )
		Rename(m_dwMaxFileNum);

#ifdef PERMIT_FILE_SHARE_WRITE
	hFile = ::CreateFile( szFileName,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL );
#else
	hFile = ::CreateFile( szFileName,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL );
#endif
	if ( hFile != INVALID_HANDLE_VALUE ) {
		if ( ::GetLastError() == ERROR_SUCCESS ) {
#ifdef _UNICODE
			WORD wMarker = 0xFEFF;
			::WriteFile( hFile,
							&wMarker,
							sizeof(WORD),
							&dwNumBytesWritten,
							NULL );
#endif
			::WriteFile( hFile,
							m_szTitleName,
							strlen(m_szTitleName),
							&dwNumBytesWritten,
							NULL );
		} else {
			::SetFilePointer( hFile,
								0,
								0,
								FILE_END );
		}

		int iLength = _tcslen(pszBuffer);
		PSTR pszText = new TCHAR[iLength + 1];
		_tcscpy(pszText, pszBuffer);
		PSTR pszToken = _tcstok(pszText, CRLF);
		while ( pszToken != NULL ) {
			::WriteFile( hFile,
							pszToken,
							_tcslen(pszToken) * sizeof(TCHAR),
							&dwNumBytesWritten,
							NULL );

			::WriteFile( hFile,
							szCRLF,
							_tcslen(szCRLF) * sizeof(TCHAR),
							&dwNumBytesWritten,
							NULL );
			pszToken = _tcstok(NULL, CRLF);
		}
		if ( pszText != NULL ) {
			delete pszText;
			pszText = NULL;
		}

		::CloseHandle(hFile);
	}

	Unlock();
}

// =========================================================================
// LoggingV
void CLogFile::LoggingV(PCSTR pszFormat, ...)
{
	TCHAR szBuffer[1024] = { 0 };

	// Format and write the data we were given
	va_list args;
	va_start(args, pszFormat);
	_vstprintf(szBuffer, pszFormat, args);

	Logging(szBuffer);
}

// =========================================================================
// IsOver
BOOL CLogFile::IsOver(PCSTR pszFileName)
{
	WIN32_FILE_ATTRIBUTE_DATA faData;

	if ( ::GetFileAttributesEx(pszFileName, GetFileExInfoStandard, &faData ) != 0 )
		return (faData.nFileSizeLow >= m_dwMaxFileSize);
	else
		return FALSE;
}

// =========================================================================
// Rename
BOOL CLogFile::Rename(DWORD dwFileNum)
{
	if ( dwFileNum <= 0 ) return TRUE;

	TCHAR szFileName[MAX_PATH], szFileReName[MAX_PATH];

	_stprintf(szFileName, _TEXT("%s\\%s%03d") LOG_FILE_EXT,
		m_szPathName, m_szFileName, dwFileNum);

	if ( ::GetFileAttributes(szFileName) != 0xFFFFFFFF ) {
		if ( dwFileNum >= m_dwMaxFileNum ) {
			unlink(szFileName);
		} else {
			_stprintf(szFileReName, _TEXT("%s\\%s%03d") LOG_FILE_EXT,
				m_szPathName, m_szFileName, dwFileNum + 1);

			rename(szFileName, szFileReName);
		}
	}
	return Rename(dwFileNum - 1);
}

// =========================================================================
// IsExist
BOOL CLogFile::IsExist(DWORD dwFileNum)
{
	if ( dwFileNum <= 0 ) return TRUE;

	TCHAR szFileName[MAX_PATH];

	_stprintf(szFileName, _TEXT("%s\\%s%03d") LOG_FILE_EXT,
		m_szPathName, m_szFileName, dwFileNum);

	return (::GetFileAttributes(szFileName) != 0xFFFFFFFF);
}

// =========================================================================
// GetHistory
//void CLogFile::GetHistory(std::list<std::string>& listHistory, UINT nItemCount)
//{
//	DWORD dwFileNum = 1;
//	TCHAR szCurrFileName[MAX_PATH];
//	TCHAR szTempFileName[MAX_PATH];
//	HANDLE hFile = INVALID_HANDLE_VALUE;
//	HANDLE hFileMap = NULL;
//	DWORD dwFileSize;
//	PSTR pszBuffer = NULL;
//	PSTR pszText = NULL;
//
//	Lock();
//
//	for ( ; listHistory.size() < nItemCount - 1; dwFileNum++ ) {
//		_stprintf(szCurrFileName, _TEXT("%s\\%s%03d") LOG_FILE_EXT,
//			m_szPathName, m_szFileName, dwFileNum);
//		::GetTempFileName(m_szPathName, _T("log"), dwFileNum, szTempFileName);
//		if ( !::CopyFile(szCurrFileName, szTempFileName, FALSE) ) {
//			break;
//		}
//		hFile = ::CreateFile( szTempFileName,
//								GENERIC_READ | GENERIC_WRITE,
//								FILE_SHARE_READ,
//								NULL,
//								OPEN_ALWAYS,
//								FILE_ATTRIBUTE_NORMAL,
//								NULL );
//		if ( hFile == INVALID_HANDLE_VALUE ) {
//			break;
//		}
//		dwFileSize = ::GetFileSize(hFile, NULL);
//		hFileMap = ::CreateFileMapping( hFile,
//										NULL,
//										PAGE_READWRITE,
//										0,
//										dwFileSize + sizeof(TCHAR),
//										NULL );
//		if ( hFileMap == NULL ) {
//			::CloseHandle(hFile);
//			break;
//		}
//		pszBuffer = (PSTR) ::MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);
//		if ( pszBuffer == NULL ) {
//			::CloseHandle(hFileMap);
//			::CloseHandle(hFile);
//			break;
//		}
//
//		pszBuffer[dwFileSize / sizeof(TCHAR)] = NULLCHAR;
//		_tcsrev(pszBuffer);
//		pszText = _tcstok(pszBuffer, CRLF);
//		while ( pszText != NULL ) {
//			listHistory.push_front(_tcsrev(pszText));
//			if ( listHistory.size() > nItemCount - 1 ) {
//				break;
//			}
//			pszText = _tcstok(NULL, CRLF);
//		}
//
//		::UnmapViewOfFile(pszBuffer);
//		::CloseHandle(hFileMap);
//		::CloseHandle(hFile);
//		::DeleteFile(szTempFileName);
//	}
//	Unlock();
//}
