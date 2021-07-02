#pragma once

#if !defined(_NSSTAGETDLL_)
#define NSSTAGEAPI __declspec(dllimport)
#else
#define NSSTAGEAPI __declspec(dllexport)
#endif

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================

// --------------------------------------------------------------------
// Initialization
BOOL NSSTAGEAPI NS_StageInitialize();


// --------------------------------------------------------------------
// General Using Functions.

// For NanoSpec(UI) base coord.(Stage Config coord.)
void NSSTAGEAPI NS_StageGetPos(STAGE_COORD_XYZ* pCoordXyz);
void NSSTAGEAPI NS_StageGetPos(STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_SampleCoordStageGetPos(STAGE_COORD_XYZ* pCoordXyz, const STAGE_COORD* pSampleOriginCoordXy);
void NSSTAGEAPI NS_SampleCoordStageGetPos(STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy);

// For Operating StageAPI with conversion UI to API(moving) coord.
BOOL NSSTAGEAPI NS_StageMoveAbsolute(const STAGE_COORD* pCoordXy);

// For getting current head type.
WORD NSSTAGEAPI NS_GetCurrentHeadType();

// For Operating StageAPI with Head / Lens offset coord.
BOOL NSSTAGEAPI NS_SelectHeadType(WORD wHeadType, BOOL bStageMove);	// ヘッド切替え関数。内部ヘッドオフセット保持。
BOOL NSSTAGEAPI NS_StageSelectLens(UINT nLens);						// レンズ切替え関数。内部レンズオフセット保持。

// For getting NanoSpec(UI) base coord from param.
void NSSTAGEAPI NS_ConvertToStageGetPosCoord(STAGE_COORD_XYZ* pCoordXyz);	// Caution!! param: StageMoveCoord
void NSSTAGEAPI NS_ConvertToStageGetPosCoord(STAGE_COORD* pCoordXy);		// Caution!! param: StageMoveCoord

// For getting Stage API(moving) coord before Operating StageAPI.
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD_XYZ* pCoordXyz, const STAGE_COORD* pSampleOriginCoordXy);
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy);
void NSSTAGEAPI NS_ConvertToStageMoveCoord(long* plZ);
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD_XYZ* pCoordXyz, LPCTSTR pszCurrentMainRecipeName = NULL);
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD* pCoordXy, LPCTSTR pszCurrentMainRecipeName = NULL);

// For Stage Coord Conversion.(Non Offset)
void NSSTAGEAPI NS_ConvertToXyAxisCoord(STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_ConvertToXyAxisCoord(long* plX, long* plY);

// For Xy Axis Conversion.
void NSSTAGEAPI NS_ConvertToStageConfigXyAxis(STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_ConvertToStageConfigXyAxis(long* plX, long* plY);
void NSSTAGEAPI NS_ConvertToStageMoveXyAxis(STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_ConvertToStageMoveXyAxis(long* plX, long* plY);

// For Operating StageAPI with Switching AutoFocus Lens.
BOOL NSSTAGEAPI NS_StageDoAutoFocus();

// 2013.02.22 bagus Substrate thickness setting -->
BOOL NSSTAGEAPI NS_SelectSubstrateThickness(int iIndex);
int NSSTAGEAPI NS_GetCurrentSubstrateThickness();
// 2013.02.22 bagus Substrate thickness setting <--

// --------------------------------------------------------------------
// Only Use for Measurement Process (Chief and Chief child thread(SitePT etc..)).

// For Operating StageAPI with conversion UI to API(moving) coord With Deskew.
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsolute(const STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy, int iMeasHead = -1);	// 呼び出し時のみヘッドオフセット有効。ヘッド切替えではない
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsolute(const STAGE_COORD* pCoordXy, LPCTSTR pszCurrentMainRecipeName, int iMeasHead = -1);			// 呼び出し時のみヘッドオフセット有効。ヘッド切替えではない

// For Async Stage Moving.
BOOL NSSTAGEAPI NS_StageMoveAbsoluteEx(const STAGE_COORD* pCoordXy);

// For Async Stage Moving With Deskew.
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsoluteEx(const STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy, int iMeasHead = -1);	// 呼び出し時のみヘッドオフセット有効。ヘッド切替えではない
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsoluteEx(const STAGE_COORD* pCoordXy, LPCTSTR pszCurrentMainRecipeName, int iMeasHead = -1);			// 呼び出し時のみヘッドオフセット有効。ヘッド切替えではない

// For Async Stage Moving by API(moving) coord Param.
BOOL NSSTAGEAPI NS_StageMoveAbsoluteAsync(const STAGE_COORD* pStageMoveCoordXy);	// Caution!! param: StageMoveCoord
BOOL NSSTAGEAPI NS_IsIdleStageMoveAsync();

// For Temporary Stage Speed Setting.
BOOL NSSTAGEAPI NS_SetStageSpeed(long lSpeed);
BOOL NSSTAGEAPI NS_RestoreStageSpeed();


// --------------------------------------------------------------------
// Do Not Use below Functions.
void NSSTAGEAPI NS_GetDiffHeadTypeOffset(WORD wHeadType, STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_GetCurrOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz);
void NSSTAGEAPI NS_GetCurrHeadTypeOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz);
// 2013.02.22 bagus Substrate thickness setting -->
//void NSSTAGEAPI NS_GetCurrSrLensOffsetStageCoord(STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_GetCurrSrLensOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz);
// 2013.02.22 bagus Substrate thickness setting <--
void NSSTAGEAPI NS_GetHeadTypeCoordLoc(WORD wHeadType, STAGE_COORD_XYZ* pCoordXyz);
// 2013.02.22 bagus Substrate thickness setting -->
//void NSSTAGEAPI NS_GetSrLensOffsetCoord(UINT nLens, STAGE_COORD* pCoordXy);
void NSSTAGEAPI NS_GetSrLensOffsetCoord(UINT nLens, STAGE_COORD_XYZ* pCoordXyz);
// 2013.02.22 bagus Substrate thickness setting <--
