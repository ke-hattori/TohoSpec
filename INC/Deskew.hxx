#if !defined( _DESKEW_HXX_ )
#define _DESKEW_HXX_

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _DESKEWDLL_ )
	#define DESKEWAPI __declspec(dllimport)
#else
	#define DESKEWAPI __declspec(dllexport)
#endif

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================

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
DESKEWAPI void __stdcall DeskewClear(void);

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
							 const STAGE_COORD& DeskewCurrent2);

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
							 const STAGE_COORD& DeskewCurrent2);

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
							 long*				xOff,
							 long*				yOff);

/*_________________________________________________________________________________
 | CorrectToCurrent
 |
 |  Given a point (Origional) from the prototype wafer, find its corresponding
 | position on the corrent wafer. 
 |
 */
DESKEWAPI void __stdcall DeskewCorrectToCurrent(
							 const STAGE_COORD& Original,
								   STAGE_COORD& Current);

#ifdef __cplusplus
}
#endif

#endif // _DESKEW_HXX_
