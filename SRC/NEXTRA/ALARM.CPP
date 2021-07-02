// ALARM.CPP: implementation of the cAlarm class.
//
// ---------------------------------------------------------------------------

#include "stdafx.h"
#include "common.hpp"
#include "alarm.h"

#include <malloc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ===========================================================================
// Construction/Destruction
// ===========================================================================

cAlarm::cAlarm()
	: m_hwnd(NULL), m_lfs(NEXALARM_LOGFILE_NAME)
{
	Initialize();
}

cAlarm::~cAlarm()
{
}

// ---------------------------------------------------------------------------
void cAlarm::Initialize(void)
{
	std::ifstream ifs(NEXALARM_INIFILE_NAME);

	std::string line;
	while ( std::getline(ifs, line) ) {
		std::istringstream iss(line);
		alarm_data item;
		iss >> item;
		m_table.push_back(item);
	}
}

// ---------------------------------------------------------------------------
void cAlarm::Set(unsigned long alid)
{
	alarm_table::iterator iter(m_table.begin());
	for ( ; iter != m_table.end(); iter++ ) {
		if ( iter->alid == alid && !iter->desc.flag ) {
			SetItemDesc(iter);
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void cAlarm::SetItemDesc(const alarm_table::iterator iter)
{
	m_wait.lock();

	iter->desc.flag = ON;

	AlarmIf_Set(iter->alid);

	if ( m_hwnd != NULL ) {
		int iItem = ListView_GetItemCount(m_hwnd);

		LV_ITEM lvItem = { LVIF_TEXT };

		// item index
		lvItem.iItem = iItem;
		lvItem.iSubItem = 0;
		char buffer[8];
		_ltot(iItem, buffer, 10);
		lvItem.pszText = buffer;
		ListView_InsertItem(m_hwnd, &lvItem);
		// date time
		ListView_SetItemText(m_hwnd, iItem, 1, iter->desc.time.current());
		// id
		sprintf(buffer, "%04d", iter->alid);
		ListView_SetItemText(m_hwnd, iItem, 2, buffer);
		// level
		ListView_SetItemText(m_hwnd, iItem, 3, iter->desc.level == "A" ? "Alarm" : "Warning");
		// text
		ListView_SetItemText(m_hwnd, iItem, 4, const_cast<LPSTR>(iter->desc.text.c_str()));
	}

	m_lfs << arrange
		  << *iter
		  << std::endl;
	m_lfs.close();

	m_wait.unlock();

}

// ---------------------------------------------------------------------------
void cAlarm::Reset(void)
{
	alarm_table::iterator iter(m_table.begin());
	for ( ; iter != m_table.end(); iter++ ) {
		if ( iter->desc.flag == ON ) {
			ResetItemDesc(iter);
		}
	}
}

// ---------------------------------------------------------------------------
void cAlarm::Reset(unsigned long alid)
{
	alarm_table::iterator iter(m_table.begin());
	for ( ; iter != m_table.end(); iter++ ) {
		if ( iter->alid == alid && iter->desc.flag == ON ) {
			ResetItemDesc(iter);
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void cAlarm::ResetItemDesc(const alarm_table::iterator iter)
{
	m_wait.lock();

	iter->desc.flag = OFF;

	m_lfs << arrange
		  << *iter
		  << std::endl;
	m_lfs.close();

	m_wait.unlock();

}
