#define WIN32_LEAN_AND_MEAN

#include <crtdbg.h>
#include <tchar.h>
#include <nexiobase.hxx>
#include <string>
#include "sio.h"

// ===========================================================================
// Constructor/destructor
// ===========================================================================

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
CSio::CSio() : nano::basic_sio()
{
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CSio::~CSio()
{
	_RPTF0(_CRT_WARN, "CSio - Destructor\n");
}

// ---------------------------------------------------------------------------
// Read
// ---------------------------------------------------------------------------
DWORD CSio::Read(PSTR pszBuffer)
{
	DWORD dwRet = ERROR_READ_FAULT;
	TCHAR ch;
	string buffer;

	pszBuffer[0] = NULLCHAR;

	if ( !IsValid() ) return SERR_INVALID_HANDLE;

	_RPTF0(_CRT_WARN, TEXT("CSio::Read - ENTER \n"));

	do {
		// timeout
		timer.reset();

		// read success
		if ( get(&ch) ) {
			buffer += ch;

			// check overflow
			if ( buffer.size() > MAX_BUFFER_LENGTH ) {
				dwRet = SERR_BUFFER_OVERFLOW;
				_RPTF0(_CRT_WARN, TEXT("CSio::Read - Buffer overflow error \n"));
				break;
			}
			if ( buffer[0] != STXCHAR ) {
				dwRet = SERR_BAD_FORMAT;
				_RPTF0(_CRT_WARN, TEXT("CSio::Read - Format error \n"));
				break;
			}
			else
			if ( ch == CRCHAR ) {
				dwRet = SERR_SUCCESS;
				_RPTF0(_CRT_WARN, TEXT("CSio::Read - Success \n"));
				break;
			}
		}
		else
		if ( timer.is_timeout() ) {
			_RPTF0(_CRT_WARN, TEXT("CSio::Read - Timeout error \n"));
			dwRet = SERR_TIMEOUT;
			break;
		}
	} while ( buffer.size() > 0 );

	if ( buffer.size() > 0 )
		_tcscpy(pszBuffer, buffer.c_str());

	_RPTF0(_CRT_WARN, TEXT("CSio::Read - LEAVE \n"));

	return dwRet;
}

// ---------------------------------------------------------------------------
// Write
// ---------------------------------------------------------------------------
DWORD CSio::Write(PCSTR pszBuffer)
{
	DWORD dwRet = SERR_WRITE_FAULT;

	if ( !IsValid() ) return SERR_INVALID_HANDLE;

	if ( write(string(pszBuffer)) ) {
		dwRet = SERR_SUCCESS;
	}

	return dwRet;
}

