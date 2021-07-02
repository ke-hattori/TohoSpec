#pragma once

#include <windows.h>
#include <tchar.h>

#define MAILSLOT_SEND_BUF_SIZE_MAX				(0)
//#define MAILSLOT_RECV_BUF_SIZE_MAX			  MAILSLOT_SEND_BUF_SIZE_MAX

class CMailslot
{
public:
	CMailslot() : m_hMailslot(NULL), m_dwLastError(0L)
	{
	}

	virtual ~CMailslot()
	{
		Close();
	}

	BOOL Create(LPCTSTR pszMailslotName)
	{
		_tcscpy(m_szMailslotName, pszMailslotName);
		m_hMailslot = ::CreateMailslot(m_szMailslotName, MAILSLOT_SEND_BUF_SIZE_MAX, MAILSLOT_WAIT_FOREVER, NULL);
		if ( m_hMailslot == INVALID_HANDLE_VALUE ) {
			m_hMailslot = NULL;
			goto ErrorReturn;
		}
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	static BOOL Send(LPCTSTR pszMailslotName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
	{
		BOOL bRet = TRUE;
		HANDLE hFile;
		DWORD dwNumberOfBytesWritten;

		hFile = ::CreateFile(pszMailslotName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if ( hFile == INVALID_HANDLE_VALUE )
			return FALSE;
		if ( !::WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite + 1, &dwNumberOfBytesWritten, NULL) )
			bRet = FALSE;
		::CloseHandle(hFile);
		return bRet;
	}

	BOOL Recieve(LPVOID lpBuffer, LPDWORD lpNumberOfBytesRead)
	{
		DWORD dwNextSize;

		if ( !::GetMailslotInfo(m_hMailslot, NULL, &dwNextSize, NULL, NULL) )
			goto ErrorReturn;
		if ( dwNextSize == MAILSLOT_NO_MESSAGE ) {
			*lpNumberOfBytesRead = 0L;
			return FALSE;
		}
		if ( !::ReadFile(m_hMailslot, lpBuffer, dwNextSize, lpNumberOfBytesRead, NULL) )
			goto ErrorReturn;
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	BOOL Close()
	{
		if ( m_hMailslot ) {
			::CloseHandle(m_hMailslot);
			m_hMailslot = NULL;
		}
		return TRUE;
	}

	inline DWORD GetLastError() const {return m_dwLastError;}

protected:
	void RetrieveErrorMessages()
	{
		LPVOID lpMessageBuffer;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessageBuffer, 0L, NULL);
		::MessageBox(NULL, (LPCTSTR)lpMessageBuffer, m_szMailslotName, MB_OK | MB_ICONERROR);
		::LocalFree(lpMessageBuffer);
	}

private:
	HANDLE m_hMailslot;
	DWORD m_dwLastError;
	TCHAR m_szMailslotName[256];
};

/////////////////////////////////////////////////////////////////////////////


