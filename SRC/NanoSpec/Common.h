#ifndef __COMMON_H__
#define __COMMON_H__

#include <windows.h>
#include <string>
#include <time.h>
#include <tchar.h>

#define SEC						(1000L)
#define MSEC					(100L)
#define ONE_SEC					(1000L)
#define TWO_SEC					(2000L)
#define THREE_SEC				(3000L)
#define FOUR_SEC				(4000L)
#define FIVE_SEC				(5000L)
#define SIX_SEC					(6000L)
#define SEVEN_SEC				(7000L)
#define EIGHT_SEC				(8000L)
#define NINE_SEC				(9000L)
#define TEN_SEC					(10000L)

namespace TOHO
{
//===========================================================================
// class CError
//===========================================================================
class CError
{
public:
	static DWORD GetLastError(void) { return ::GetLastError(); }

	std::string GetErrorDescription(DWORD dwLastError = GetLastError())
	{
		LPVOID lpBuffer = NULL;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

			std::string strErrorMessage;
			if( lpBuffer )
			{
			strErrorMessage = reinterpret_cast<LPTSTR>(lpBuffer);
			LocalFree(lpBuffer);
			}
			return strErrorMessage;
		}
};

//===========================================================================
// class CTimer
//===========================================================================
class CTimer
{
public:
	CTimer() : m_dStart(clock()) { ::GetLocalTime(&m_sysTime); }

	void Restart(double dTimeout = 0.0)
	{
		m_dTimeout = dTimeout;
		m_dStart = clock();
	}

	double GetElapsedTime(void) const
	{
		return (static_cast<double>(clock()) - m_dStart) / CLOCKS_PER_SEC;
	}

	BOOL IsTimeout(void)
	{
		return GetElapsedTime() > m_dTimeout;
	}
	void GetCurrent(void) { ::GetLocalTime(&m_sysTime); }
	PCSTR ToChar(void)
	{
		_stprintf(m_szText,
			"%04d.%02d.%02d %02d:%02d:%02d:%03d",
			m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay,
			m_sysTime.wHour, m_sysTime.wMinute, m_sysTime.wSecond, m_sysTime.wMilliseconds);
		return m_szText;
	}
private:
	double m_dTimeout;
	double m_dStart;
	SYSTEMTIME m_sysTime;
	TCHAR m_szText[23 + 1];
};

//===========================================================================
// class CThException
//===========================================================================
class CThException
{
public:
	CThException(LPCTSTR lpszMessage)
		: m_strMessage(lpszMessage), m_dwError(CError::GetLastError()) {}
	virtual ~CThException() {};

public:
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL)
	{
		if( m_dwError == 0 )
			_sntprintf(lpszError, nMaxError, _TEXT("%s error"), m_strMessage.c_str());
		else
			_sntprintf(lpszError, nMaxError, _TEXT("%s error 0x%08x"), m_strMessage.c_str(), m_dwError);
		return TRUE;
	}
	virtual std::string GetErrorMessage(PUINT pnHelpContext = NULL)
	{
		TCHAR szErrorMessage[512] = { 0 };
		GetErrorMessage(szErrorMessage, sizeof(szErrorMessage));
		return szErrorMessage;
	}

private:
	DWORD		m_dwError;
	std::string m_strMessage;
};

//===========================================================================
// class CMutexException
//===========================================================================
class CMutexException : public CThException
{
public:
	CMutexException(LPCTSTR lpszMessage)
		: CThException(lpszMessage) {}
};

//===========================================================================
// class CThMutex
//===========================================================================
class CThMutex
{
public:
	CThMutex(LPCTSTR lpszMutexName)
		: m_hMutex(NULL), m_lpszMutexName(_TEXT(lpszMutexName)) {}
	virtual ~CThMutex() {}

public:
	void Create(LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL,	BOOL bInitialOwner = FALSE) throw (CMutexException)
	{
		m_hMutex = ::CreateMutex(lpMutexAttributes, bInitialOwner, m_lpszMutexName);
		if ( m_hMutex == NULL )
			throw CMutexException(m_lpszMutexName);
	}

	void Open(DWORD dwDesiredAccess = MUTEX_ALL_ACCESS, BOOL bInheritHandle = TRUE) throw (CMutexException)
	{
		m_hMutex = ::OpenMutex(dwDesiredAccess, bInheritHandle, m_lpszMutexName);
		if ( m_hMutex == NULL )
			throw CMutexException(m_lpszMutexName);
	}

	void Close() throw()
	{
		if ( m_hMutex )
			::CloseHandle(m_hMutex);
	}

	BOOL Lock(DWORD dwTimeout = INFINITE) throw (CMutexException)
	{
		DWORD dwResult = ::WaitForSingleObject(m_hMutex, dwTimeout);
		switch ( dwResult )
		{
			case WAIT_OBJECT_0:
				return TRUE;
				break;
			case WAIT_TIMEOUT:
				return FALSE;
				break;
			case WAIT_ABANDONED:
				throw CMutexException(_TEXT("Abandoned"));
				break;
		}
	}

	void Unlock() throw()
	{
		if ( m_hMutex )
			::ReleaseMutex(m_hMutex);
	}

private:
	HANDLE	m_hMutex;
	LPCTSTR m_lpszMutexName;
};
};

#endif