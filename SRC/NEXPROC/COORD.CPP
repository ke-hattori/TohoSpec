#pragma warning (disable:4786)

#include <windows.h>
#include <set>
#include <algorithm>
#include <active.hxx>
#include <alarmif.hxx>
#define __NEXTRA_INTERNAL
#include <nexmisc.hxx>
#include <nexini.hxx>
#include <nexiobase.hxx>
#include <nexrc.hxx>
#include "coord.hpp"

#ifdef _DEBUG
#pragma comment(lib, "dtnsactive.lib")
#pragma comment(lib, "dtnsalarmif.lib")
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsactive.lib")
#pragma comment(lib, "tnsalarmif.lib")
#pragma comment(lib, "tnsnexini.lib")
#endif

// ===========================================================================
// Constructor/destructor
// ===========================================================================

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
cCoord::cCoord(void)
	: m_hModule(NULL), m_initialized(FALSE), m_emergency(FALSE), m_vacuum(NULL), m_airfloat(NULL), m_shutter(NULL), m_robo(NULL), m_lfs(NEXPROC_LOGFILE_NAME)
{
	basic_interface::zeromemory (&client);

	m_hModule = NexResourceLoadLibrary(m_params.System.Language);

	m_list.push_back(m_shutter = new cShutter());

	if ( m_params.Options.RoboCylinder ) {
		m_list.push_back(m_robo = new cRoboCylinder());
	}

	begin();
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
cCoord::~cCoord(void)
{
	list<basic_coord*>::iterator iter(m_list.begin());
	for ( ; iter != m_list.end(); ++iter ) {
		if ( *iter ) {
			delete *iter;
			*iter = NULL;
		}
	}

	if ( m_hModule != NULL ) {
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}
