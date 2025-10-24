#if !defined( _NEXMISC_HXX_ )
#define _NEXMISC_HXX_

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <crtdbg.h>
#include <vector>
#include <map>
#include <string>
#include <process.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#define __NEXTRA_INTERNAL
#include <nexcom.hxx>

namespace nano
{
using namespace std;

struct dio_config_t {
	unsigned short driver;
	unsigned short group;
	unsigned short port;
};

struct sio_config_t {
	unsigned short port;
	unsigned short baudrate;
};

// =========================================================================
// class : basic_interface
// =========================================================================
class basic_interface
{
public:
	template<typename T>
	static void zeromemory	  (T* rhs, unsigned int count = 1L) 		{		 memset(rhs, 0,   sizeof(T) * count); }
	template<typename T>
	static void copymemory	  (T* lhs, T* rhs, unsigned int count = 1L) {		 memcpy(lhs, rhs, sizeof(T) * count); }
	template<typename T>
	static int	comparememory (T* lhs, T* rhs, unsigned int count = 1L) { return memcmp(lhs, rhs, sizeof(T) * count); }
};

// =========================================================================
// class : system_time
// =========================================================================
class system_time : public SYSTEMTIME
{
public:
	void current (void) { GetLocalTime((LPSYSTEMTIME) this); }
};

// =========================================================================
// class : process_directory
// =========================================================================
class process_directory
{
	process_directory ()
	{
		char temp  [_MAX_PATH];
		char drive [_MAX_DRIVE];
		char dir   [_MAX_DIR];

		GetModuleFileName(NULL, temp, sizeof(temp));
		_splitpath(temp, drive, dir, NULL, NULL);
		sprintf(path, "%s%s", drive, dir);
		if ( strlen(path) > 0 ) {
			if ( path[strlen(path) - 1] != SLASHCHAR ) {
				strcat(path, SLASH);
			}
		}
	}
	static char path  [_MAX_PATH];
public:
	static void make (char* _s, const char* fname)
	{
		static process_directory dummy; sprintf(_s, "%s%s", path, fname); 
	}
	static string make (const char* fname, BOOL _f = TRUE)
	{
		static process_directory dummy;
		string str(path);
		str += fname;
		if ( _f ) create(str.c_str());
		return str;
	}
	static BOOL create (const char* _s)
	{
		BOOL retval = TRUE;
		char dir[_MAX_DIR], temp[_MAX_PATH];
		int i;
		LPSECURITY_ATTRIBUTES p = NULL;
 
		_splitpath(_s, NULL, temp, NULL, NULL);

		if ( CreateDirectory(temp, p) )
			return retval;

		for ( i = strlen(temp); i > 0; i-- ) {
			if ( temp[i] =='\\' || temp[i] == '/' ) {
				break;
			}
		}

		if ( i <= 0 )
			return FALSE;

		strcpy(dir, temp);
		dir[i] = NULL;

		if ( create(dir) ) {
			return CreateDirectory(temp, p);
		}
		else {
			retval = FALSE;
		}

		return retval;
	}
};
__declspec(selectany) char process_directory::path[_MAX_PATH] = {0};

// =========================================================================
// class : basic_timer
// =========================================================================
class basic_timer
{
public:
	basic_timer () : start(GetTickCount()), elapsed(0), run(false) { current(); }
	basic_timer (unsigned long msec) { set(msec); }
	basic_timer& operator = (const basic_timer& rhs) { duration = rhs.duration; return *this; }
	basic_timer& operator = (unsigned long msec) { duration = msec; return *this; }

	void operator += (unsigned long msec) { duration += msec; }
	void operator -= (unsigned long msec) { duration -= msec; }
	operator char* ()
	{
		sprintf(buffer, "%04d.%02d.%02d %02d:%02d:%02d:%03d",
			systime.wYear, systime.wMonth, systime.wDay,
				systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
		return buffer;
	}

	inline void set (unsigned long msec = 0)
	{
		if ( msec > 0 ) duration = msec;
		run 	= true;
		elapsed = 0;
		start	= GetTickCount();
	}
	inline void reset(void) { set(); }
	inline void kill (void) { run = false; }

	static void do_events(void)
	{
		MSG msg;

		// window message
		while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	static void do_events(unsigned long msec)
	{
		MSG msg;

		// window message
		while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(msec);
	}
	inline void delay (unsigned long msec)
	{
		// set timer
		set(msec);

		while ( !is_timeout() ) do_events();
	}

	inline unsigned long get_elapsed (void)
	{
		// add to elapsed time
		curr = GetTickCount();	  // wrapped
		if ( curr < start )
			start = 0;

		elapsed += (curr - start);
		start	 = curr;

		return elapsed;
	}

	inline bool is_timeout (void) { return run ? get_elapsed() > duration : false; }

	inline basic_timer& current (void)
	{
		systime.current();
		return *this;
	}

	friend ostream& operator << (ostream& os, const basic_timer& rhs)
	{
		return os
				<< dec
				<< right
				<< setw(4) << setfill('0') << rhs.systime.wYear	<< DOTCHAR
		   		<< setw(2) << setfill('0') << rhs.systime.wMonth  << DOTCHAR
		   		<< setw(2) << setfill('0') << rhs.systime.wDay
		   		<< SPACECHAR
		   		<< setw(2) << setfill('0') << rhs.systime.wHour	 << COLONCHAR
		   		<< setw(2) << setfill('0') << rhs.systime.wMinute << COLONCHAR
		   		<< setw(2) << setfill('0') << rhs.systime.wSecond << COLONCHAR
		   		<< setw(3) << setfill('0') << rhs.systime.wMilliseconds
				<< SPACECHAR
				<< left;
	}

protected:
	system_time    systime;

private:
	unsigned long  start, curr, elapsed;
	unsigned long  duration;
	bool		   run;
	char		   buffer[32 + 1];
};

// =========================================================================
// class : basic_kernel
// =========================================================================
class basic_kernel
{
public:
	basic_kernel () : handle(NULL), valid(false), exists(false) {}
	~ basic_kernel ()
	{
		close();
		if ( is_open() ) {
			handle = NULL;
		}
	}
	operator HANDLE () { return handle; }

	inline void close (void)
	{
		if ( is_valid() && is_open() ) {
			CloseHandle(handle);
			valid = false;
		}
	}
	inline bool is_valid  (void) const { return valid; }
	inline bool is_open   (void) const { return handle != NULL; }
	inline bool is_exists (void) const { return exists; }

protected:
	HANDLE handle;
	bool   valid;
	bool   exists;
};

// =========================================================================
// class : kernel_interface
// =========================================================================
class kernel_interface
{
public:
	virtual void lock	(void) = 0;
	virtual void unlock (void) = 0;
};

// =========================================================================
// class : critical_section
// =========================================================================
class critical_section : public kernel_interface
{
public:
	critical_section () { InitializeCriticalSection(&cs); }
	~ critical_section () { DeleteCriticalSection(&cs); }

public:
	virtual void lock	(void) { EnterCriticalSection(&cs); }
	virtual void unlock (void) { LeaveCriticalSection(&cs); }

private:
	CRITICAL_SECTION cs;
};

// =========================================================================
// class : mutex
// =========================================================================
class mutex : public kernel_interface, public basic_kernel
{
public:
	mutex () : basic_kernel() {};
	mutex (const char* _s, bool owner = false) : basic_kernel()
	{
		create(_s, owner);
	}
	void create (const char* _s = NULL, bool owner = false)
	{
		handle = CreateMutex(NULL, owner, _s);
		if ( is_open() ) {
			valid = true;
			if ( GetLastError() == ERROR_ALREADY_EXISTS )
				exists = true;
		}
	}
	~ mutex () { lock(); unlock(); }

public:
	virtual void lock	(void) { lock(INFINITE); }
	virtual void unlock (void) { ReleaseMutex(handle); }

	virtual unsigned long wait	 (unsigned long msec = 0L)
	{
		unsigned long retval;

		while (1) {
			// wait for event or message, if it's a message, process it and return to waiting state
			retval = MsgWaitForMultipleObjects(1, &handle, FALSE, msec, QS_ALLINPUT);
			if	( retval == WAIT_OBJECT_0 ) {
				return retval;
			}
			else
			if ( retval == WAIT_OBJECT_0 + 1 ) {
				// process window messages
				basic_timer::do_events();
			}
			else {
				// timed out !
				return retval;
			}
		}
	}

	inline bool lock (unsigned long msec) { return WaitForSingleObject(handle, msec) == WAIT_OBJECT_0; }
};

// =========================================================================
// class : event
// =========================================================================
class event : public mutex
{
public:
	event () : mutex() { create(NULL, true, true); };
	event (const char* _s, bool manual = true, bool init = true) : mutex()
	{
		create(_s, manual, init);
	}
	void create (const char* _s = NULL, bool manual = true, bool init = false)
	{
		handle = CreateEvent(NULL, manual, init, _s);
		if ( is_open() ) {
			valid = true;
			if ( GetLastError() == ERROR_ALREADY_EXISTS )
				exists = true;
		}
	}

public:
	virtual void lock	(void) { ResetEvent(handle); }
	virtual void unlock (void) { SetEvent(handle);	 }
};

// =========================================================================
// class : basic_syncarray
// =========================================================================
template<typename E, typename T>
class basic_syncarray : public E, public T
{
public:
	basic_syncarray () {}
	virtual ~ basic_syncarray () {};
};

// =========================================================================
// class : sync_list
// =========================================================================
template<typename E, typename T>
class sync_list : public basic_syncarray<list<E>, T>
{
public:
	sync_list () {}
	virtual ~ sync_list () {};
};

// =========================================================================
// class : basic_sio
// =========================================================================
class basic_sio : public basic_kernel
{
public:
	basic_sio () : timer(T1_TIMEOUT), basic_kernel() {}
	basic_sio (const char* device, unsigned long baudrate = CBR_9600, unsigned long timeout = T1_TIMEOUT)
		: timer(timeout), basic_kernel() { open(device, baudrate); }
	virtual ~ basic_sio () {}

	bool open (const char* device)
	{
		if ( is_open() ) return false;

		handle = CreateFile
					(
					 device,
					 GENERIC_READ | GENERIC_WRITE,
					 0,
					 NULL,
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL,
					 NULL
					);

		if ( handle == INVALID_HANDLE_VALUE ) {
			handle = NULL;
			return false;
		}
		else {
			valid = true;
		}

		return true;
	}
	inline bool open  (const char* device, unsigned long baudrate) { return open(device) && setup(baudrate); }
	inline bool write (const char* _s, size_t size) 			   { return put(_s, size);					 }
	inline bool write (const string _s) 						   { return write(_s.c_str(), _s.size());	 }
	inline bool read  (char* _s, unsigned long count, char delim = LFCHAR)
	{
		bool ret = false;
		const char* pch = _s;
		unsigned long length = 0;
		do {
			timer.set();
			if ( get(_s) ) {
				if ( *pch == delim ) {
					ret = true;
					break;
				}
				pch++; length++;
			}
			else
			if ( timer.is_timeout() )
				break;
		} while ( count != length );

		return ret;
	}
	inline bool put (const char* _s, size_t size)
	{
		unsigned long count;
		if ( WriteFile(handle, _s, size, &count, NULL) )
			return count == size ? true : false;
		else
			return false;
	}
	inline bool put (char ch)
	{
		unsigned long count;
		if ( WriteFile(handle, &ch, sizeof(char), &count, NULL) )
			return count == sizeof(char) ? true : false;
		else
			false;
	}
	inline bool get (char* _c) { if ( (*_c = get()) != NULLCHAR ) return true; else return false; }
	inline char get (void)
	{
		char ch;
		unsigned long count;
		ReadFile(handle, &ch, sizeof(char), &count, NULL);
		return count > 0 ? ch : NULLCHAR;
	}
	inline unsigned long length (void) { COMSTAT stat; unsigned long errors; return ClearCommError(handle, &errors, &stat) ? stat.cbInQue : 0; }
	bool setup
			(
			 unsigned long baudrate = CBR_9600,
			 unsigned char bytesize = 8,
			 unsigned char parity	= NOPARITY,
			 unsigned char stopbits = ONESTOPBIT
			)
	{
		if ( !is_open() ) return false;

		DCB dcb;
		if ( !GetCommState(handle, &dcb) ) return false;

		dcb.BaudRate = baudrate;
		dcb.ByteSize = bytesize;
		dcb.Parity	 = parity;
		dcb.StopBits = stopbits;

		dcb.fParity  = (parity != NOPARITY);

		if ( !SetCommState(handle, &dcb) ) return false;

		return timeouts();
	}

protected:
	basic_timer 	 timer;

private:
	bool timeouts (bool blocking = false)
	{
		if ( !is_open() ) return false;

		COMMTIMEOUTS cto;
		if ( !GetCommTimeouts(handle, &cto) ) return false;

		cto.ReadIntervalTimeout 	   = blocking ? 0 : MAXDWORD;
		cto.ReadTotalTimeoutConstant   = 0;
		cto.ReadTotalTimeoutMultiplier = 0;

		if ( !SetCommTimeouts(handle, &cto) ) return false;

		return true;
	}
};

// =========================================================================
// class : basic_shared
// =========================================================================
class basic_shared : public basic_kernel
{
public:
	basic_shared () : ptr(NULL), basic_kernel() {}
	basic_shared (const char* _s, unsigned long n) : ptr(NULL), basic_kernel() { create(_s, n); }
	basic_shared (const char* _s) : ptr(NULL), basic_kernel() { open(_s); }
	virtual ~ basic_shared () { finalize(); }

	void*		  operator-> () { return ptr;  }
	basic_shared* operator&  () { return this; }
	operator	  void* 	 () { return ptr;  }

	basic_shared& create (const char* _s, unsigned long n)
	{
		if ( is_open() ) finalize();

		handle = CreateFileMapping
					 (
					  (HANDLE) 0xFFFFFFFF,
					  NULL,
					  PAGE_READWRITE,
					  0,
					  n,
					  _s
					 );

		if ( is_open() ) {
			valid = true;
			exists = GetLastError() == ERROR_ALREADY_EXISTS;
		}

		return *this;
	}

	basic_shared& open (const char* _s)
	{
		if ( is_open() ) finalize();

		handle = OpenFileMapping
					 (
					  FILE_MAP_ALL_ACCESS,
					  0,
					  _s
					 );

		return *this;
	}

	inline void finalize (void)
	{
		detach();
		close();
	}

	inline void attach (void)
	{
		if ( !is_open() ) return;

		if ( !is_valid_ptr() ) detach();

		ptr = MapViewOfFile
				  (
				   handle,
				   FILE_MAP_ALL_ACCESS,
				   0, 0, 0
				  );
	}

	inline void detach (void)
	{
		if ( is_valid_ptr() ) {
			UnmapViewOfFile(ptr);
			ptr = NULL;
		}
	}
	inline void*		 pptr		  (void) const { return ptr;									   }
	inline unsigned long size		  (void) const { return is_open() ? GetFileSize(handle, NULL) : 0; }
	inline bool 		 is_valid_ptr (void) const { return ptr != NULL;							   }

private:
	void*  ptr;
};

template <class T>
class shared_memory : public nano::basic_shared
{
public:
	shared_memory ();
	shared_memory (const char* _s) : nano::basic_shared(_s, sizeof(T)) { attach(); }
	virtual ~ shared_memory () {}

	T* operator-> () { return (T*) pptr(); }
};

// =========================================================================
// class : basic_thread
// =========================================================================
class basic_thread : public basic_kernel
{
public:
	basic_thread () : basic_kernel() {}
	virtual ~ basic_thread () {}

	inline bool begin (unsigned ( __stdcall* start_address )( void* ), void* arglist = NULL)
	{
		if ( is_valid() ) return false;

		try {
			handle = (HANDLE) _beginthreadex(NULL, 0, start_address, (void*) arglist, 0, &thrdaddr);
			_RPTF0(_CRT_WARN, "basic_thread::begin is success\n");
			if ( is_open() ) {
				valid = true;
			}
		}
		catch (...) {
			_RPTF0(_CRT_WARN, "basic_thread::begin is failed\n");
		}
		return is_valid();
	}

	virtual void finalize (void)
	{
		if ( is_valid() ) {
			_RPTF0(_CRT_WARN, "basic_thread::close...\n");
			WaitForSingleObject(handle, INFINITE);
			close();
		}
	}
	inline unsigned getecd (void) { return ecd; }

	inline void suspend (void) { SuspendThread(handle); }
	inline void resume	(void) { ResumeThread(handle);	}

protected:
	virtual void end (void) { _endthreadex(ecd); }

private:
	unsigned thrdaddr;
	unsigned ecd;
};

// =========================================================================
// class : thread
// =========================================================================
class thread : protected basic_thread
{
public:
	thread () : loop(true), basic_thread() {}

protected:
	inline bool begin (void) { return basic_thread::begin(start_address, this); }
	inline bool begin (bool _f)
	{
		if ( _f ) {
			finalize();
			loop = true;
		}
		return begin();
	}

	static unsigned int __stdcall start_address (void* arglist) { return reinterpret_cast<thread*>(arglist)->start_address(); }
	virtual unsigned int start_address (void) = 0;

	void finalize (void) { loop = false; basic_thread::finalize(); }

	inline bool is_loop (void) const { return loop; }

private:
	bool loop;
};

// =========================================================================
// class : lfstream
// =========================================================================
static unsigned long const max_file_size = 500000;
static unsigned long const max_file_num  = 50;
class lfstream : public ofstream
{
public:
	lfstream () {}
	lfstream (const char* path, unsigned long size = max_file_size, unsigned long n = max_file_num)
		: max_fsize(size), max_fnum(n) { create(path); }

	inline lfstream& operator << (lfstream& (__cdecl * _f)(lfstream&));
	inline lfstream& operator << (ostream& (__cdecl * _f)(ostream&));

	friend inline ostream& time 		  (ostream& os);
	friend inline ostream& date 		  (ostream& os);
	friend inline ostream& datetime 	  (ostream& os);
	friend inline ostream& datetime_space (ostream& os);

	inline lfstream& create (const char* s)
	{
		assert(s != NULL);
		_splitpath(s, NULL, dir, fname, ext);
		return *this;
	}

	inline void logging (const char* s)
	{
		arrange();

#ifdef __AUTO_DATETIME
//                1         2         3
//      0123456789012345678901234567890
//      yyyy.mm.dd hh:mm:ss.sss 
		datetime(*this);
#endif

		*this << s << endl;

		close();
	}

	inline bool is_over (const char* path) const
	{
		WIN32_FILE_ATTRIBUTE_DATA attribute;

		if ( GetFileAttributesEx(path, GetFileExInfoStandard, &attribute ) != 0 )
			return (attribute.nFileSizeLow > max_fsize);
		else
			return false;
	}

	static bool is_exist (const char* path) { return (GetFileAttributes(path) != 0xFFFFFFFF); }

	friend inline lfstream& arrange (lfstream& lfs);
	bool arrange (void)
	{
		string path;

		if ( is_open() ) close();

		makepath(path, 001);

		if ( is_over(path.c_str()) )
			arrange(max_fnum);

		ofstream::open(path.c_str(), ios::app | ios::out);
		return is_open();
	}
private:
	bool arrange (unsigned long n)
	{
		if ( n <= 0 ) return false;

		string path;
		makepath(path, n);

		if ( is_exist(path.c_str()) ) {
			if ( n >= max_fnum ) {
				_unlink(path.c_str());
			}
			else {
				string temp;
				makepath(temp, n + 1);
				rename(path.c_str(), temp.c_str());
			}
		}
		return arrange(n - 1);
	}

	inline void makepath (string& _s, unsigned long n)
	{
		ostringstream oss;

		oss << dir
			<< fname
			<< setw(3) << setfill('0') << n
			<< ext;

		_s = oss.str();
	}

	char dir   [_MAX_DIR];
	char fname [_MAX_FNAME];
	char ext   [_MAX_EXT];

	unsigned long max_fsize;
	unsigned long max_fnum;
};
	inline lfstream& lfstream::operator << (lfstream& (__cdecl * _f)(lfstream&)) { (*_f)(*this); return *this; }
	inline lfstream& lfstream::operator << (ostream& (__cdecl * _f)(ostream&)) { (*_f)(*this); return *this; }

	inline ostream& time (ostream& os)
	{
		system_time systime;
		systime.current();

		return os << setw(2) << setfill('0') << systime.wHour	<< COLONCHAR
				  << setw(2) << setfill('0') << systime.wMinute << COLONCHAR
				  << setw(2) << setfill('0') << systime.wSecond << COLONCHAR
				  << setw(3) << setfill('0') << systime.wMilliseconds
				  << SPACECHAR;
	}
	inline ostream& date (ostream& os)
	{
		system_time systime;
		systime.current();

		return os << setw(4) << setfill('0') << systime.wYear	<< DOTCHAR
				  << setw(2) << setfill('0') << systime.wMonth	<< DOTCHAR
				  << setw(2) << setfill('0') << systime.wDay
				  << SPACECHAR;

	}

	inline ostream& datetime (ostream& os)
	{
		basic_timer systime;

		return os << systime.current();
	}

	inline ostream& datetime_space (ostream& os)
	{
		string buffer(24, SPACECHAR);

		return os << buffer;
	}

	inline lfstream& arrange (lfstream& lfs)
	{
		lfs.arrange();

		return lfs;
	}

}; // namespace nano

#endif // _NEXMISC_HXX_
