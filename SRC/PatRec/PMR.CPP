#include <windows.h>
#include <crtdbg.h>
#include <tchar.h>
#include <mil.h>

#include "patrec.h"

#pragma comment(lib, "mil.lib")
#pragma comment(lib, "milpat.lib")
#pragma comment(lib, "milim.lib")

// =========================================================================
// Class Name :  CPatMatchRec
//
// Description： パターン認識ラッパクラス
//
// Author:		 T.Matsuhisa
//
// =========================================================================

// =========================================================================
// Construction/Destruction
CPatMatchRec::CPatMatchRec()
	: CPatRecMil80()
{
	// Memory will be allocated for the following. Set their initial state
	// to NULL so that allocation can be checked before memory free operations
	// are invoked.
	//
	MilApplication	= NULL; 	 MilSystem		 = NULL;
	MilImage		= NULL; 	 MilSubImage	 = NULL;
	MilGrayImage	= NULL; 	 MilModel		 = NULL;
	MilResult		= NULL; 	 MilDisplay 	 = NULL;
	MilGraphic		= NULL;

	::ZeroMemory(MilDeskewSite, sizeof(MilDeskewSite));
}

CPatMatchRec::~CPatMatchRec()
{
}

// =========================================================================
// Name：	   Teach
// Purpose：
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CPatMatchRec::Teach(LPRECT lpRect)
{
	BOOL bRet = TRUE;

	Halt();

	ImageGrab();

	long lWidth, lHeight;
	lWidth	= lpRect->right  - lpRect->left;
	lHeight = lpRect->bottom - lpRect->top;

	MIL_ID MilTempImage;
	if ( MbufChild2d(MilImage, lpRect->left, lpRect->top,
			 lWidth, lHeight, &MilTempImage) == M_NULL ) {
		return FALSE;
	}

	// Allocate a grayscale template model if one does not already exist
	//
	if ( MilSubImage != M_NULL ) {
		MbufFree(MilSubImage);
		MilSubImage = M_NULL;
	}

	// Allocate a 2D data buffer.
	//
	if ( MbufAlloc2d(MilSystem, lWidth, lHeight, 8 + M_UNSIGNED,
			 M_IMAGE+ M_PROC, &MilSubImage) == M_NULL ) {
		return FALSE;
	}

	MbufCopy(MilTempImage, MilSubImage);

	MbufFree(MilTempImage);
	MilTempImage = M_NULL;

	// Free a pattern matching model.
	//
	if ( MilModel != NULL ) {
		MpatFree(MilModel);
		MilModel = NULL;
	}

	MpatAllocModel(MilSystem, MilSubImage, 0L, 0L, lWidth, lHeight, M_NORMALIZED, &MilModel);

	_RPTF1(_CRT_WARN, _TEXT("CPatRecMil80Mil60::Teach(), Alocated Model Size: %d\n"), (lWidth * lHeight));

	MpatPreprocModel(MilSubImage, MilModel, M_DEFAULT); 	  // Preprocess

	Continuous();

	return bRet;
}

// =========================================================================
// Name：	   GetModel
// Purpose：
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CPatMatchRec::GetModel(PSTR pszModelName)
{
	BOOL bRet = TRUE;

	_RPTF0(_CRT_WARN, _TEXT("CPatMatchRec GetModel\n"));

	Halt();

	_RPTF0(_CRT_WARN, _TEXT("\tAfter check for error\n"));

	// The MbufRestore function will allocate storage for the buffer, so
	// release any previous storage.
	//
	if ( MilSubImage != NULL ) {
		_RPTF0(_CRT_WARN, _TEXT("\tBefore MbufFree(MilSubImage)\n"));
		MbufFree(MilSubImage);
		MilSubImage = NULL;
	}

	// The MpatRestore function will allocate storage for the Model, so
	// release any previous storage.
	//
	if ( MilModel != NULL )
	{
		_RPTF0(_CRT_WARN, _TEXT("\tBefore MpatFree(Model)\n"));
		MpatFree(MilModel);
		MilModel = NULL;
	}

	// Restore the buffer from the disk file.
	//
	_RPTF0(_CRT_WARN, _TEXT("Before MbufRestore()\n"));

	// Get the requested buffer from the disk
	if ( MbufImport(pszModelName, M_BMP, M_RESTORE, MilSystem,	&MilSubImage) == 0 ) {
		_RPTF1(_CRT_WARN, _TEXT("CPatMatchRec::GetModel() Failure to get buffer: %s from the disk\n"), pszModelName);
		bRet = FALSE;
	}

	// Inquires about a specified setting of a context data buffer.
	long lWidth, lHeight = 0L;
	MbufInquire(MilSubImage, M_SIZE_X, &lWidth);
	MbufInquire(MilSubImage, M_SIZE_Y, &lHeight);

	// Allocate a pattern matching model from a source image.
	//
	MpatAllocModel(MilSystem, MilSubImage, 0L, 0L, lWidth, lHeight, M_NORMALIZED, &MilModel);

	_RPTF1(_CRT_WARN, _TEXT("CPatMatchRec::GetModel(), Alocated Model Size: %d\n"), (lWidth * lHeight));

	MpatPreprocModel(MilSubImage, MilModel, M_DEFAULT); 	  // Preprocess

	Continuous();

	return bRet;
}

// =========================================================================
// Name：	   Match
// Purpose：
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CPatMatchRec::Match(LPDESKEW_RESULT lpDeskewResult, BOOL bSaveLayer, int iCurrLayer)
{
	MIL_ID MilMaskImage = M_NULL;

	Halt();

	ImageGrab();

	MpatSetAccuracy(MilModel, M_HIGH);	   // Set the search accuracy
	MpatSetSpeed(MilModel, M_VERY_HIGH);   // Set the search model speed

	// to emulate old MIL, Profiler SW will determine match
	//
#if 0
	MpatSetAcceptance(MilModel, m_patternRec.dLowestMatchScore);
	MpatSetCertainty(MilModel, m_patternRec.dStopGropingScore);
#else
	MpatSetAcceptance(MilModel, 60);
	MpatSetCertainty(MilModel, 90);
#endif

	// Disable the search model angle mode
	//
	MpatSetAngle(MilModel, M_SEARCH_ANGLE_MODE, M_DISABLE);

	// Preprocess the model.
	//
	MpatPreprocModel(MilImage, MilModel, M_DEFAULT);

	// Free a result buffer.
	if ( MilResult != NULL ) {
		MpatFree(MilResult);
		MilResult = NULL;
	}

	// Allocate a result buffer.
	//
	if ( MpatAllocResult(MilSystem, 1L, &MilResult) == 0 ) {
		_RPTF0(_CRT_WARN, _TEXT("CPatMatchRec::Match() Failure to allocate result buffer\n"));
		lpDeskewResult->Score = 0.0;
		return FALSE;
	}

	// Dummy first find operation for better function timing accuracy (model cache effet,...).
	//
	MpatFindModel(MilImage, MilModel, MilResult);

	// Reset the timer.
	//
	MappTimer(M_TIMER_RESET + M_SYNCHRONOUS, M_NULL);

	// Find the model in the target image buffer.
	//
	MpatFindModel(MilImage, MilModel, MilResult);
	lpDeskewResult->Score = 0.0;

	// Read the find time.
	//
	double Time;
	MappTimer(M_TIMER_READ + M_SYNCHRONOUS, &Time);

	if ( MpatGetNumber(MilResult, M_NULL)  >= 1L ) {
		// Get the results of the match from the result structure
		//
		MpatGetResult(MilResult, M_SCORE, &lpDeskewResult->Score);
		MpatGetResult(MilResult, M_POSITION_X, &lpDeskewResult->XPos);
		MpatGetResult(MilResult, M_POSITION_Y, &lpDeskewResult->YPos);
		MpatGetResult(MilResult, M_SCORE, &lpDeskewResult->TargetScore);
		_RPTF4(_CRT_WARN, "score=%lf, xPos=%lf, yPos%lf, target score=%lf\n",
			lpDeskewResult->Score, lpDeskewResult->XPos, lpDeskewResult->YPos, lpDeskewResult->TargetScore);
	}
	else {
		_RPTF0(_CRT_WARN, _TEXT("Match failed! Pattern not found properly.\n"));
		lpDeskewResult->Score		= 0.0;
		lpDeskewResult->XPos		= 0.0;
		lpDeskewResult->YPos		= 0.0;
		lpDeskewResult->TargetScore = 0.0;
	}

	if ( (lpDeskewResult->Score > 100.0) || (lpDeskewResult->Score < 0.0) )
		lpDeskewResult->Score = 0.0;

	if ( (lpDeskewResult->TargetScore > 100.0) || (lpDeskewResult->TargetScore < 0.0) )
		lpDeskewResult->TargetScore = 0.0;

return TRUE;
}

// =========================================================================
// Name：	   End
// Purpose：
// Parameters: None
//
// Returns:    BOOL - TRUE = 成功
//
// Notes:
//
BOOL CPatMatchRec::End(void)
{
	Halt();

	// Free the model, result buffer.
	//
	if ( MilModel != NULL ) {
		MpatFree(MilModel);
		MilModel = NULL;
	}
	if ( MilResult != NULL ) {
		MpatFree(MilResult);
		MilResult = NULL;
	}

	// Free a data buffer.
	//
	if ( MilGrayImage != NULL ) {
		MbufFree(MilGrayImage);
		MilGrayImage = NULL;
	}
	if ( MilSubImage != NULL ) {
		MbufFree(MilSubImage);
		MilSubImage = NULL;
	}

	MdispFree(MilDisplay);

	MgraFree(MilGraphic);

	// Free default Mil objects.
	//
	MappFreeDefault(MilApplication, MilSystem, M_NULL, M_NULL, MilImage);

	_RPTF0(_CRT_WARN, _TEXT("CPatMatchRec::End()\n"));

	return TRUE;
}
