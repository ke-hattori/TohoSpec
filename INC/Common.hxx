#if !defined( _COMMON_HXX_ )
#define _COMMON_HXX_

#pragma once

#include <windows.h>
#include <tchar.h>
#include <list>
#include <cstring>
#include <ctime>
#include <cassert>

//----------------------------------------------------------------------------
// DEFINITIONS
//----------------------------------------------------------------------------
#define SEC 					(1)
#define ONE_SEC 				(1)
#define TWO_SEC 				(2)
#define THREE_SEC				(3)
#define FOUR_SEC				(4)
#define FIVE_SEC				(5)
#define SIX_SEC 				(6)
#define SEVEN_SEC				(7)
#define EIGHT_SEC				(8)
#define NINE_SEC				(9)
#define TEN_SEC 				(10)

/*
function
*/
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT) * 8 - 1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT) * 8 - 1))) != 0 )
#define IsALTpressed()	 ( (GetKeyState(VK_MENU) & (1 << (sizeof(SHORT) * 8 - 1))) != 0 )
#define FILEEXIST(lpszFileName) (::GetFileAttributes(lpszFileName) != 0xFFFFFFFF)

// =========================================================================
// Name�F DoEvents
void DoEvents(void);

// =========================================================================
// Name�F Delay
void Delay(DWORD dwDuration);

// =========================================================================
// Name�F WaitWithMessageLoop
BOOL WaitWithMessageLoop(HANDLE hEvent, DWORD dwTimeout);

//===========================================================================
// class CError
//===========================================================================
class CError
{
public:
	static DWORD GetLastError(void) { return ::GetLastError(); }

	static std::string GetErrorDescription(DWORD dwLastError = GetLastError())
	{
		LPVOID lpBuffer = NULL;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					   reinterpret_cast<LPTSTR>(&lpBuffer), 0, NULL);

		 std::string strErrorMessage;
		 if( lpBuffer ) {
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

	inline void Restart(void)
	{
		m_dStart = clock();
	}
	BOOL Restart(double dTimeout)
	{
		if ( dTimeout <= 0 ) return FALSE;

		m_dTimeout = dTimeout;
		Restart();

		return TRUE;
	}
	inline double GetElapsedTime(void) const
	{
		return (static_cast<double>(clock()) - m_dStart) / CLOCKS_PER_SEC;
	}

	BOOL IsTimeout(void)
	{
		return GetElapsedTime() > m_dTimeout;
	}
	inline void GetCurrent(void) { ::GetLocalTime(&m_sysTime); }
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
// class _stringex
//===========================================================================
template<class T = std::string>
class _stringex : public T
{
protected:
	typedef T					traits_type;
	typedef typename T::reference		reference;
	typedef typename T::const_reference	const_reference;
	typedef typename T::size_type		size_type;
	typedef typename T::pointer			pointer;
	typedef typename T::const_pointer	const_pointer;
public:
	_stringex() {}
	_stringex(traits_type val) {
		this->swap(val);
	}
	const _stringex<traits_type>& operator=(const_reference rhs) {
		return *this = traits_type(1, rhs);
	}
	const _stringex<traits_type>& operator=(traits_type str) {
		*this = _stringex<traits_type>(str);
		return *this; 
	}
	const _stringex<traits_type>& operator+=(const_reference rhs ) {
		return *this += traits_type(1, rhs);
	}
	const _stringex<traits_type>& operator+=(traits_type str) {
		(*this).append(str);
		return *this;
	}
	const _stringex<traits_type>& operator+(const_reference rhs) {
		return *this + traits_type(1, rhs);
	}
	const _stringex<traits_type>& operator+(traits_type str) {
		(*this).append(str);
		return *this;
	}
	const_reference operator[](size_type pos) const {
		return (*this).at(pos);
	}
	reference operator[](size_type pos) {
		return (*this).at(pos);
	}
	bool operator==(const_reference rhs) {
		return *this == traits_type(1, rhs);
	}
	bool operator==(_stringex str) {
		return (*this).compare(str) == 0);
	}
	bool operator<=(const_reference rhs) {
		return *this <= traits_type(1, rhs);
	}
	bool operator<=(_stringex str) {
		return (*this).compare(str) <= 0);
	}
	bool operator>=(const_reference rhs) {
		return *this >= traits_type(1, rhs);
	}
	bool operator>=(_stringex str) {
		return (*this).compare(str) >= 0);
	}
	bool operator!=(const_reference rhs) {
		return *this != traits_type(1, rhs);
	}
	bool operator!=(_stringex str) {
		return (*this).compare(str) != 0);
	}
	bool operator<(const_reference rhs) {
		return *this < traits_type(1, rhs);
	}
	bool operator<(_stringex str) {
		return (*this).compare(str) < 0);
	}
	bool operator>(const_reference rhs) {
		return *this > traits_type(1, rhs);
	}
	bool operator>(_stringex str) {
		return (*this).compare(str) > 0);
	}
	// CString ���l�؂�...
	traits_type& format(traits_type fmt, ...) {
		va_list args;
		va_start(args, fmt);
		va_list args_save = args;

		int max_len = 0;
		for ( iterator iter(fmt.begin()), last(fmt.end()); iter != last; iter++ ) {
			if ( *iter != '%' || *iter++ == '%' ) {
				max_len += _tclen(iter);
				continue;
			}
			int item_len = 0, width = 0;
			for ( ; iter != last; iter++ ) {
				if ( *iter == '#' )
					max_len += 2;
				else if ( *iter == '*' )
					width = va_arg(args, int);
				else if ( *iter == '-' || *iter == '+' || *iter == '0' || *iter == ' ' )
					;
				else
					break;
			}
			if ( width == 0 ) {
				width = _ttoi(iter);
				for ( ; iter != last && _istdigit(*iter); iter++ )
					;
			}
			_ASSERT(width >= 0);

			int precision = 0;
			if ( *iter == '.' ) {
				iter++;
				if ( *iter == '*' ) {
					precision = va_arg(args, int);
					iter++;
				}
				else {
					precision = _ttoi(iter);
					for ( ; iter != last && _istdigit(*iter); iter++ )
						;
				}
				_ASSERT(precision >= 0);
			}
			int modifier = 0;
			if ( _tcsncmp(iter, _T("I64"), 3) == 0 ) {
				iter += 3;
				modifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			_ASSERT(FALSE);
#endif // !defined(_X86_) && !defined(_ALPHA_)
			}
			else {
				switch ( *iter ) {
				case 'h':
					modifier = FORCE_ANSI;
					iter++;
					break;
				case 'l':
					modifier = FORCE_UNICODE;
					iter++;
					break;
				case 'F': case 'N': case 'L':
					iter++;
					break;
				}
			}
			switch ( *iter | modifier ) {
			case 'c': case 'C':
				item_len = 2;
				va_arg(args, TCHAR_ARG);
				break;
			case 'c' | FORCE_ANSI: case 'C' | FORCE_ANSI:
				item_len = 2;
				va_arg(args, CHAR_ARG);
				break;
			case 'c' | FORCE_UNICODE: case 'C' | FORCE_UNICODE:
				item_len = 2;
				va_arg(args, WCHAR_ARG);
				break;
			case 'iter':
				{
					LPCTSTR next_arg = va_arg(args, LPCTSTR);
					if ( next_arg == NULL )
					   item_len = 6;
					else
					{
					   item_len = lstrlen(next_arg);
					   item_len = max(1, item_len);
					}
				}
				break;
			case 'S':
				{
#ifndef _UNICODE
					LPWSTR next_arg = va_arg(args, LPWSTR);
					if ( next_arg == NULL )
					   item_len = 6;
					else
					{
					   item_len = wcslen(next_arg);
					   item_len = max(1, item_len);
					}
#else // _UNICODE
					LPCSTR next_arg = va_arg(args, LPCSTR);
					if ( next_arg == NULL )
					   item_len = 6;
					else {
					   item_len = lstrlenA(next_arg);
					   item_len = max(1, item_len);
					}
#endif // _UNICODE
				}
				break;
			case 'iter' | FORCE_ANSI: case 'S' | FORCE_ANSI:
				{
					LPCSTR next_arg = va_arg(args, LPCSTR);
					if ( next_arg == NULL )
					   item_len = 6;
					else {
					   item_len = lstrlenA(next_arg);
					   item_len = max(1, item_len);
					}
				}
				break;
			case 'iter' | FORCE_UNICODE: case 'S' | FORCE_UNICODE:
				{
					LPWSTR next_arg = va_arg(args, LPWSTR);
					if ( next_arg == NULL )
					   item_len = 6;
					else {
					   item_len = wcslen(next_arg);
					   item_len = max(1, item_len);
					}
				}
				break;
			}
			if ( item_len != 0 ) {
				if ( precision != 0 )
					item_len = min(item_len, precision);
				item_len = max(item_len, width);
			}
			else
			{
				switch ( *iter ) {
				case 'd': case 'i': case 'u':
				case 'x': case 'X': case 'o':
					if ( modifier & FORCE_INT64 )
						va_arg(args, __int64);
					else
						va_arg(args, int);
					item_len = 32;
					item_len = max(item_len, width + precision);
					break;
				case 'e': case 'g': case 'G':
					va_arg(args, DOUBLE_ARG);
					item_len = 128;
					item_len = max(item_len, width + precision);
					break;
				case 'f':
					{
						double f;
						LPTSTR temp;
						temp = (LPTSTR)_alloca(max(width, 312 + precision + 6));
						f = va_arg(args, double);
						_stprintf( temp, _T( "%*.*f" ), width, precision + 6, f );
						item_len = _tcslen(temp);
					}
					break;
				case 'p':
					va_arg(args, void*);
					item_len = 32;
					item_len = max(item_len, width + precision);
					break;
				case 'n':
					va_arg(args, int*);
					break;
				default:
					_ASSERT(FALSE);
				}
			}
			max_len += item_len;
		}
		max_len++;
		LPTSTR str = (LPTSTR) _alloca(max_len);
		_vstprintf(str, fmt.c_str(), args_save);
		va_end(args);
		*this = _stringex<traits_type>(str);
		return *this;
	}
	_stringex<traits_type> mid(size_type pos = 0, size_type n = npos) {
		return _stringex<traits_type>((*this).substr(pos, n));
	}
	_stringex<traits_type>& toupper() {
		for ( iterator iter(begin()); iter != end(); iter++ )
			*iter = _toupper(*iter);
		return *this;
	}
	_stringex<traits_type>& tolower() {
		for ( iterator iter(begin()); iter != end(); iter++ )
			*iter = _tolower(*iter);
		return *this;
	}
	int toi() {
		return _ttoi((*this).c_str());
	}
	long tol() {
		return _ttol((*this).c_str());
	}
	double tof() {
		return atof((*this).c_str());
	}
};
typedef _stringex<> stringex;

#endif
