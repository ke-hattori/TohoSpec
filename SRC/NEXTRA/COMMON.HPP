#if !defined( _COMMON_HPP_ )
#define _COMMON_HPP_

#pragma once

#pragma warning(disable:4786)

#define __NEXTRA_INTERNAL

#include <vector>
#include <nexmisc.hxx>
#include <nexini.hxx>
#include <nexif.hxx>
#include <nexrc.hxx>
#include <nexiobase.hxx>
#include <nexax.hxx>
#include <nexproc.hxx>
#include <nexui.hxx>
#include <alarmif.hxx>
#include "colorstc.h"
#include "colorbtn.h"

// ===========================================================================
// STRUCTURE / UNION
// ===========================================================================

// ===========================================================================
// DEFINITION
// ===========================================================================
/*
 * color
*/
#define RED_COLOR						RGB(255, 0, 0)
#define GREEN_COLOR 					RGB(0, 255, 0)
#define BLUE_COLOR						RGB(0, 0, 255)
#define YELLOW_COLOR					RGB(255, 255, 0)
#define MAGENTA_COLOR					RGB(255, 0, 255)
#define CYAN_COLOR						RGB(0, 255, 255)
#define WHITE_COLOR 					RGB(255, 255, 255)
#define EXIST_COLOR 					RGB(0, 128, 255)
#define BTN_COLOR						GetSysColor(COLOR_BTNFACE)

#define WM_ICON_NOTIFY					(WM_USER + 0x0301)
#define WM_BUTTON_EVENT 				(WM_USER + 0x0002)

/*
 * timer
*/
#define REFRESH_TIMER_ID				(1)
#define REFRESH_TIMER_ELAPSE			(500L)
#define EVENT_TIMER_ID					(2)
#define EVENT_TIMER_ELAPSE				(100L)

// ===========================================================================
// CONSTANT VALUE
// ===========================================================================

// ===========================================================================
// FUNCTION
// ===========================================================================
void EnableControlGroup (CWnd *pParent, BOOL bEnable, int nIDs, ...);
void OperationLogging (LPCSTR lpszText);
void EquipmentStatusRun (BOOL bOn = ON);
int MyMessageBox (HWND hWnd, UINT nIDText, UINT nIDCaption, UINT nType = MB_OK);

class CCursor
{
public:

	static void BeginWaitCursor (BOOL bShowWaitCursor = TRUE) {
		m_bWaitCursor = TRUE;
		m_bShowWaitCursor = bShowWaitCursor;

		if ( m_bShowWaitCursor ) {
			AfxGetApp()->m_pMainWnd->SetCapture();
			AfxGetApp()->BeginWaitCursor();
		}
	}
	static void EndWaitCursor (void) {
		m_bWaitCursor = FALSE;

		if ( m_bShowWaitCursor ) {
			AfxGetApp()->EndWaitCursor();
			ReleaseCapture();
		}
	}
	static BOOL IsWaitCursor (void) { return m_bWaitCursor; }
private:

public:
	static BOOL m_bWaitCursor;
	static BOOL m_bShowWaitCursor;
};
#endif // _COMMON_HPP_
