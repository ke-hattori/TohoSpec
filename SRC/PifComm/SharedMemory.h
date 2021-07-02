#pragma once

#include <windows.h>
#include <tchar.h>

template<class T>
class CSharedMemory
{
public:
	CSharedMemory() : m_hMapFile(NULL), m_hMutex(NULL), m_dwLastError(0L), m_pSharedMemory(NULL)
	{
	}

	virtual ~CSharedMemory()
	{
		Close();
	}

	BOOL Create(LPCTSTR pszSharedMemoryName, LPSECURITY_ATTRIBUTES lpAttributes = NULL)
	{
		TCHAR szBuff[256];

		_tcscpy(m_szSharedMemoryName, pszSharedMemoryName);
		m_hMapFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, lpAttributes, PAGE_READWRITE, 0, sizeof(T), m_szSharedMemoryName);
		if ( !m_hMapFile )
			goto ErrorReturn;
		if ( (m_dwLastError = ::GetLastError()) == ERROR_ALREADY_EXISTS ) {
			::CloseHandle(m_hMapFile);
			m_hMapFile = NULL;
			return FALSE;
		}
		if ( !MapView() )
			goto ErrorReturn;

		_tcscpy(szBuff, m_szSharedMemoryName);
		_tcscat(szBuff, _T("Mutex"));
		if ( (m_hMutex = ::CreateMutex(NULL, FALSE, szBuff)) == NULL )
			goto ErrorReturn;
		if ( ::GetLastError() == ERROR_ALREADY_EXISTS ) {
			::CloseHandle(m_hMutex);
			m_hMutex = NULL;
			return FALSE;
		}
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	BOOL Open(LPCTSTR pszSharedMemoryName)
	{
		TCHAR szBuff[256];

		_tcscpy(m_szSharedMemoryName, pszSharedMemoryName);
		m_hMapFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_szSharedMemoryName);
		if ( !m_hMapFile )
			goto ErrorReturn;
		if ( !MapView() )
			goto ErrorReturn;

		_tcscpy(szBuff, m_szSharedMemoryName);
		_tcscat(szBuff, _T("Mutex"));
		if ( (m_hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, szBuff)) == NULL )
			goto ErrorReturn;
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	BOOL Close()
	{
		if ( m_hMutex ) {
			::WaitForSingleObject(m_hMutex, INFINITE);
			::CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
		if ( !UnMapView() )
			goto ErrorReturn;
		if ( m_hMapFile ) {
			::CloseHandle(m_hMapFile);
			m_hMapFile = NULL;
		}
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	void Get(T* lpBuffer) const
	{
		Lock();

		::CopyMemory(lpBuffer, GetSharedMemoryPtr(), sizeof(T));

		Unlock();
	}

	void Set(const T* lpBuffer)
	{
		Lock();

		::CopyMemory(GetSharedMemoryPtr(), lpBuffer, sizeof(T));

		Unlock();
	}

	inline T* GetSharedMemoryPtr() const {return m_pSharedMemory;}
	inline DWORD GetLastError() const {return m_dwLastError;}

protected:
	BOOL MapView()
	{
		m_pSharedMemory = (T*)::MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0L, 0L, 0L);
		if ( !m_pSharedMemory )
			goto ErrorReturn;
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	BOOL UnMapView()
	{
		if ( m_pSharedMemory ) {
			if ( !::UnmapViewOfFile(m_pSharedMemory) )
				goto ErrorReturn;
			m_pSharedMemory = NULL;
		}
		return TRUE;

	ErrorReturn:
		m_dwLastError = ::GetLastError();
		RetrieveErrorMessages();
		return FALSE;
	}

	inline DWORD Lock() const {return ::WaitForSingleObject(m_hMutex, INFINITE);}
	inline BOOL Unlock() const {return ::ReleaseMutex(m_hMutex);}

	void RetrieveErrorMessages() const
	{
		LPVOID lpMessageBuffer;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessageBuffer, 0L, NULL);
		::MessageBox(NULL, (LPCTSTR)lpMessageBuffer, m_szSharedMemoryName, MB_OK | MB_ICONERROR);
		::LocalFree(lpMessageBuffer);
	}

private:
	HANDLE m_hMapFile;
	HANDLE m_hMutex;
	DWORD m_dwLastError;
	TCHAR m_szSharedMemoryName[256];
	T* m_pSharedMemory;
};

/////////////////////////////////////////////////////////////////////////////

