#include <windows.h>
#include <crtdbg.h>
#include <tchar.h>
#include <mil.h>

#include "patrec.h"

#pragma comment(lib, "mil.lib")
#pragma comment(lib, "milmod.lib")
#pragma comment(lib, "milim.lib")

// =========================================================================
// Class Name :  GeometricModelFinder
//
// Description： パターン認識ラッパクラス
//
// Author:		 T.Matsuhisa
//
// =========================================================================

// =========================================================================
// Construction/Destruction
GeometricModelFinder::GeometricModelFinder()
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

GeometricModelFinder::~GeometricModelFinder()
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
BOOL GeometricModelFinder::Teach(LPRECT lpRect)
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

	// Free a Model Finder context.
	//
	if ( MilModel != NULL ) {
		MmodFree(MilModel);
		MilModel = NULL;
	}

	MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilModel);

	MmodDefine(MilModel, M_IMAGE, MilSubImage, 0L, 0L, lWidth, lHeight);

	_RPTF1(_CRT_WARN, _TEXT("GeometricModelFinder::Teach(), Define Model Size: %d\n"), (lWidth * lHeight));

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
BOOL GeometricModelFinder::GetModel(PSTR pszModelName)
{
	BOOL bRet = TRUE;

	_RPTF0(_CRT_WARN, _TEXT("GeometricModelFinder GetModel\n"));

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

	// Restore the buffer from the disk file.
	//
	_RPTF0(_CRT_WARN, _TEXT("Before MbufRestore()\n"));

	// Get the requested buffer from the disk
	if ( MbufImport(pszModelName, M_BMP, M_RESTORE, MilSystem,	&MilSubImage) == 0 ) {
		_RPTF1(_CRT_WARN, _TEXT("GeometricModelFinder::GetModel() Failure to get buffer: %s from the disk\n"), pszModelName);
		bRet = FALSE;
	}

	// Inquires about a specified setting of a context data buffer.
	long lWidth, lHeight = 0L;
	MbufInquire(MilSubImage, M_SIZE_X, &lWidth);
	MbufInquire(MilSubImage, M_SIZE_Y, &lHeight);

	// Free a Model Finder context.
	//
	if ( MilModel != NULL ) {
		MmodFree(MilModel);
		MilModel = NULL;
	}

	MmodAlloc(MilSystem, M_GEOMETRIC, M_DEFAULT, &MilModel);

	// Add a model to, or delete a model from, a Model Finder context.
	MmodDefine(MilModel, M_IMAGE, MilSubImage, 0L, 0L, lWidth, lHeight);

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
BOOL GeometricModelFinder::Match(LPDESKEW_RESULT lpDeskewResult, BOOL bSaveLayer, int iCurrLayer)
{
	MIL_ID MilMaskImage = M_NULL;

	Halt();

	ImageGrab();

#if 1
	const long detail_level[] = { M_MEDIUM, M_HIGH, M_VERY_HIGH };
	int Retries = 0;
	double Score = 0.0;

RETRY:
	MmodControl(MilModel, M_CONTEXT, M_DETAIL_LEVEL, detail_level[Retries]);
#endif

	if ( m_patternRec.Mask.bEnable ) {
		// Masking
		MbufAlloc2d(MilSystem,
			MmodInquire (MilModel, M_DEFAULT, M_ALLOC_SIZE_X, M_NULL),
			MmodInquire (MilModel, M_DEFAULT, M_ALLOC_SIZE_Y, M_NULL),
			8+M_UNSIGNED, M_IMAGE+M_PROC, &MilMaskImage);

		MgraColor(MilGraphic, M_COLOR_BLACK);
		MgraClear(MilGraphic, MilMaskImage);
		MgraColor(MilGraphic, M_COLOR_WHITE);

		MgraArcFill(M_DEFAULT, MilMaskImage,
			MmodInquire (MilModel, M_DEFAULT, M_ALLOC_SIZE_X, M_NULL) / 2,
			MmodInquire (MilModel, M_DEFAULT, M_ALLOC_SIZE_Y, M_NULL) / 2,
			m_patternRec.Mask.XRad, m_patternRec.Mask.YRad, 0, 360);

		MmodMask(MilModel, M_DEFAULT, MilMaskImage, M_DONT_CARES, M_DEFAULT);
	}

//	  // Save layer
//	  if ( DeskewOpt.bAutoSaveLayer )
//		  SaveLayer(bSaveLayer, iCurrLayer);
//
	MmodControl(MilModel, M_CONTEXT, M_ACCURACY, M_HIGH);	  // Set the search accuracy
	MmodControl(MilModel, M_CONTEXT, M_SPEED, M_VERY_HIGH); 		// Set the search model speed

	// to emulate old MIL, Profiler SW will determine match
	//
#if 0
	MmodControl(MilModel, M_DEFAULT, M_ACCEPTANCE, m_patternRec.dLowestMatchScore);
	MmodControl(MilModel, M_DEFAULT, M_CERTAINTY, m_patternRec.dStopGropingScore);
#else
	MmodControl(MilModel, M_DEFAULT, M_ACCEPTANCE, 60);
	MmodControl(MilModel, M_DEFAULT, M_CERTAINTY, 90);
#endif
//
//	  if ( DeskewOpt.bSearchAngle )
//	  {
//		  // Activate the search model angle mode.
//		  //
//		  MmodControl(MilModel, M_CONTEXT, M_SEARCH_ANGLE_RANGE, M_ENABLE);
//
//		  // Set the search model range angle.
//		  //
//		  MmodControl(MilModel, M_DEFAULT, M_ANGLE_DELTA_NEG, DeskewOpt.AngleDelta);
//		  MmodControl(MilModel, M_DEFAULT, M_ANGLE_DELTA_POS, DeskewOpt.AngleDelta);
//	  }
//	  else
//	  {
		// Disable the search model angle mode
		//
		MmodControl(MilModel, M_CONTEXT, M_SEARCH_ANGLE_RANGE, M_DISABLE);
//	  }

	// Preprocess the model.
	//
	MmodPreprocess(MilModel, M_DEFAULT);

	// Free a result buffer.
	if ( MilResult != NULL ) {
		MmodFree(MilResult);
		MilResult = NULL;
	}

	if ( MilMaskImage != M_NULL ) {
		MbufFree(MilMaskImage);
		MilMaskImage = M_NULL;
	}

	// Allocate a result buffer.
	//
	if ( MmodAllocResult(MilSystem, M_DEFAULT, &MilResult) == 0 ) {
		_RPTF0(_CRT_WARN, _TEXT("GeometricModelFinder::Match() Failure to allocate result buffer\n"));
		lpDeskewResult->Score = 0.0;
		return FALSE;
	}
	// Dummy first find operation for better function timing accuracy (model cache effet,...).
	//
	MmodFind(MilModel, MilImage, MilResult);

	// Reset the timer.
	//
	MappTimer(M_TIMER_RESET + M_SYNCHRONOUS, M_NULL);

	// Find the model in the target image buffer.
	//
	MmodFind(MilModel, MilImage, MilResult);
	lpDeskewResult->Score = 0.0;

	// Read the find time.
	//
	double Time;
	MappTimer(M_TIMER_READ + M_SYNCHRONOUS, &Time);

	long lNumResults = 0L;
	MmodGetResult(MilResult, M_DEFAULT, M_NUMBER + M_TYPE_LONG, &lNumResults);
	if ( lNumResults  >= 1L ) {
		// Get the results of the match from the result structure
		//
		MmodGetResult(MilResult, 0L, M_SCORE, &lpDeskewResult->Score);
		MmodGetResult(MilResult, 0L, M_POSITION_X, &lpDeskewResult->XPos);
		MmodGetResult(MilResult, 0L, M_POSITION_Y, &lpDeskewResult->YPos);
		MmodGetResult(MilResult, 0L, M_SCORE_TARGET, &lpDeskewResult->TargetScore);
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

#if 1
	if ( lpDeskewResult->Score < m_patternRec.dStopGropingScore ) {
		Retries++;
		if ( Retries < countof(detail_level) ) {
			Score = max(Score, lpDeskewResult->Score);
			MmodPreprocess(MilModel, M_RESET);
			goto RETRY;
		}
	}
	lpDeskewResult->Score = max(Score, lpDeskewResult->Score);
#endif

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
BOOL GeometricModelFinder::End(void)
{
	Halt();

	// Free the model, result buffer.
	//
	if ( MilModel != NULL ) {
		MmodFree(MilModel);
		MilModel = NULL;
	}
	if ( MilResult != NULL ) {
		MmodFree(MilResult);
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

	_RPTF0(_CRT_WARN, _TEXT("GeometricModelFinder::End()\n"));

	return TRUE;
}
