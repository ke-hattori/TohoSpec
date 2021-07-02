#define WIN32_LEAN_AND_MEAN

#define _DESKEWDLL_

#include <windows.h>
#include <math.h>
#include "..\\..\\INC\\globals.hxx"
#include "..\\..\\INC\\deskew.hxx"
#include "deskew.h"

// ####################################################################
// GLOBAL VARIABLES
// ####################################################################
static CDeskew* pDeskew = NULL;

/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- { ---------- */
#include <tchar.h>
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- } ---------- */
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("Deskew"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- } ---------- */
		if ( pDeskew == NULL ) {
			pDeskew = new CDeskew();
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if ( pDeskew != NULL ) {
			delete pDeskew;
			pDeskew = NULL;
		}
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL ‘½d‹N“®–hŽ~ ---------- } ---------- */
		break;
	}
	return TRUE;
}

/*_____________________________________________________________________________
 | Clear
 |
 |  Clear the deskew object such that when there is no Deskew_Set, the Deskew_
 |  CorrectToCurrent will not make any correction. This function should be
 |  called before each sequence (especially when the sequence is without deskew).
 |
 | So for each sequence, the way of using this deskew object is:
 |  1. Call DeskewClear
 |  2. Call DeskewSet   if there is deskew
 |  3. Call DeskewCorrectToCurrent
 |
 */
DESKEWAPI void __stdcall DeskewClear(void)
{
	pDeskew->Clear();
}


/*_____________________________________________________________________________
 | Set
 |
 |  Given the two deskew points from the prototype wafer(DeskewOrig1,
 |  DeskewOrig2) and the two deskew points from the current wafer
 |  (DeskewCurr1, DeskewCurr2), construct a CDeskew object
 |  for the current wafer.
 |
 |  Note: The order of the two points must be the same in both the prototype
 |  wafer and the current wafer. For any new wafer, must call this set
 |  again to get the correct result.
 |
 */
DESKEWAPI void __stdcall DeskewSet(
							 const STAGE_COORD& DeskewOriginal1,
							 const STAGE_COORD& DeskewOriginal2,
							 const STAGE_COORD& DeskewCurrent1,
							 const STAGE_COORD& DeskewCurrent2)
{
	pDeskew->Set(DeskewOriginal1, DeskewOriginal2,
				 DeskewCurrent1, DeskewCurrent2);
}

/*_____________________________________________________________________________
 | CDeskew::GetAngle
 |
 |  Given the two deskew points from the prototype wafer(DeskewOriginal1,
 |  DeskewOriginal2) and the two deskew points from the current wafer
 |  (DeskewCurrent1, DeskewCurrent2), find the angle between the two lines
 |  defined by the original and current deskew points. The angle can be used
 |  to correct the rotation angle between the current wafer and the original
 |  prototype wafer. The angle found will also be added to scan position angle
 |  when CorrectToCurrent is called. Stage should be called to do a relative
 |  move of this angle found before the second deskew process is started.
 |
 |  Note: The order of the two points must be the same in both the prototype
 |  wafer and the current wafer.
 |
 */
DESKEWAPI void __stdcall DeskewGetAngle(
							 const STAGE_COORD& DeskewOriginal1,
							 const STAGE_COORD& DeskewOriginal2,
							 const STAGE_COORD& DeskewCurrent1,
							 const STAGE_COORD& DeskewCurrent2)
{
	pDeskew->GetAngle(DeskewOriginal1, DeskewOriginal2,
				 DeskewCurrent1, DeskewCurrent2);
}

/*_____________________________________________________________________________
 | CDeskew::GetOffset
 |
 |  Given the two deskew points from the prototype wafer(DeskewOriginal1,
 |  DeskewOriginal2) and the two deskew points from the current wafer
 |  (DeskewCurrent1, DeskewCurrent2), find the translation between the two lines
 |  defined by the original and current deskew points. The translation can be used
 |  to optimize the second deskew process. The translation found here is only an
 |  approximation and the error can be enlarged by rotation to a degree that the
 |  offset can be totally wrong.
 |
 |  Note: The order of the two points must be the same in both the prototype
 |  wafer and the current wafer.
 |
 */
 DESKEWAPI void __stdcall DeskewGetOffset(
							 const STAGE_COORD& DeskewOriginal1,
							 const STAGE_COORD& DeskewOriginal2,
							 const STAGE_COORD& DeskewCurrent1,
							 const STAGE_COORD& DeskewCurrent2,
								   long*		xOff,
								   long*		yOff)
{
	pDeskew->GetOffset(DeskewOriginal1, DeskewOriginal2,
				 DeskewCurrent1, DeskewCurrent2, xOff, yOff);
}

/*_________________________________________________________________________________
 | CorrectToCurrent
 |
 |  Given a point (Origional) from the prototype wafer, find its corresponding
 | position on the corrent wafer.
 |
 */
DESKEWAPI void __stdcall DeskewCorrectToCurrent(
							 const STAGE_COORD& Original,
								   STAGE_COORD& Current)
{
	pDeskew->CorrectToCurrent(Original, Current);
}
