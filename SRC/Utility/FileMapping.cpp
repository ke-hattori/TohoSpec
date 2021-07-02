#define WIN32_LEAN_AND_MEAN

//===========================================================================
// FileMapping.cpp (FileMapping.hxx)
//===========================================================================
#include <common.hxx>
#include <filemapping.hxx>

//===========================================================================
// class : CFileMapping
// Construction/Destruction
//===========================================================================

CFileMapping::CFileMapping()
	: m_hFile(0L), m_lpBuffer(NULL), m_bAlreadyExists(FALSE), m_dwDataSize(0L)
{
}

CFileMapping::CFileMapping(PCSTR pszFileName, DWORD dwDataSize, LPSECURITY_ATTRIBUTES lpAttributes/*=NULL*/)
	: m_hFile(0L), m_lpBuffer(NULL), m_bAlreadyExists(FALSE), m_dwDataSize(dwDataSize)
{

	Create(pszFileName, m_dwDataSize, lpAttributes);
}

CFileMapping::CFileMapping(PCSTR pszFileName)
	: m_hFile(0L), m_lpBuffer(NULL), m_bAlreadyExists(FALSE), m_dwDataSize(0L)
{
	Open(pszFileName);
}
CFileMapping::~CFileMapping()
{
	Detach();

	Close();
}

//---------------------------------------------------------------------------
// Create
void CFileMapping::Create(PCSTR pszFileName, DWORD dwDataSize, LPSECURITY_ATTRIBUTES lpAttributes/*=NULL*/)
{
	assert(pszFileName);

	m_strFileName = pszFileName;

	m_hFile = ::CreateFileMapping( (HANDLE)0xFFFFFFFF,
								   lpAttributes,
								   PAGE_READWRITE,
								   0L,
								   dwDataSize,
								   m_strFileName.c_str() );

	if ( m_hFile != NULL )
		m_bAlreadyExists = ::GetLastError() == ERROR_ALREADY_EXISTS;

	Attach();
}

//---------------------------------------------------------------------------
// Open
void CFileMapping::Open(PCSTR pszFileName)
{
	assert(pszFileName);

	m_strFileName = pszFileName;

	m_hFile = ::OpenFileMapping( FILE_MAP_ALL_ACCESS,
								 FALSE,
								 m_strFileName.c_str() );

	Attach();
}

//---------------------------------------------------------------------------
// Close
void CFileMapping::Close()
{
	if ( m_hFile )
		::CloseHandle(m_hFile);
}

//---------------------------------------------------------------------------
// Attach
void CFileMapping::Attach()
{
	m_lpBuffer = ::MapViewOfFile( m_hFile,
								  FILE_MAP_ALL_ACCESS,
								  0L, 0L, 0L );
}

//---------------------------------------------------------------------------
// Detach
void CFileMapping::Detach()
{
	if ( m_lpBuffer ) {
		::UnmapViewOfFile(m_lpBuffer);
		m_lpBuffer = NULL;
	}
}
