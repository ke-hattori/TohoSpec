// PifComm.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Resource.h"
#include <afxdllx.h>
#include "..\\..\\INC\\Globals.hxx"
#include "PifMessage.h"
#include "MailThread.h"
#include "SharedMemory.h"

#define _PIFCOMMDLL_
#include "..\\..\\INC\\PifComm.hxx"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE PifCommDLL = { NULL, NULL };

#define SHAREDMEMORY_NAME	_T("EQMonitor")

// Global varivable
HWND g_hNotifyWnd;
SYSTEMTIME g_setTime;
int g_iKindOfRecipe;
TCHAR g_szSampleSizeRecipeName[RECIPE_NAME_LEN + 1];
TCHAR g_szReferenceDataCheckRecipeName[RECIPE_NAME_LEN + 1];
TCHAR g_szRecipeName[RECIPE_NAME_LEN + 1];
TCHAR g_szSampleId[SAMPLE_ID_LEN + 1];
TCHAR g_szLotId[LOT_ID_LEN + 1];
double g_dSampleSizeX;
double g_dSampleSizeY;
int g_iMovePosId;

static CMailThread* pMailThread;
static CSharedMemory<EQMONITORSET>* pSharedMemory;


// Helper Fucntion
// 指数表記へ変換する関数（printf書式の%E相当だが、指数部を2桁表示にしたいため自作関数を用意）
static void ConvertToExpNumber(double dNumber, LPTSTR pszExpNumber)
{
	sprintf(pszExpNumber, "%.3E", dNumber);

	// Two Digit Exponent
	pszExpNumber[strlen(pszExpNumber) - 3] = pszExpNumber[strlen(pszExpNumber) - 2];
	pszExpNumber[strlen(pszExpNumber) - 2] = pszExpNumber[strlen(pszExpNumber) - 1];
	pszExpNumber[strlen(pszExpNumber) - 1] = '\0';
}


// DllMain
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("PifComm"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		TRACE0("PIFCOMM.DLL Initializing!\n");

		if (!AfxInitExtensionModule(PifCommDLL, hInstance))
			return 0;

		new CDynLinkLibrary(PifCommDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PIFCOMM.DLL Terminating!\n");

		AfxTermExtensionModule(PifCommDLL);
		if ( pMailThread ) {
			pMailThread->ShutDown();
			::WaitForSingleObject(pMailThread->m_hThread, INFINITE);
			delete pMailThread;
			pMailThread = NULL;
		}

		if ( pSharedMemory ) {
			delete pSharedMemory;
			pSharedMemory = NULL;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
	}
	return 1;
}

// PifComm_Init
extern "C"
void PIFCOMMAPI PifComm_Init()
{
	pSharedMemory = new CSharedMemory<EQMONITORSET>();
	if ( !pSharedMemory->Create(SHAREDMEMORY_NAME) ) {
		if ( pSharedMemory->GetLastError() == ERROR_ALREADY_EXISTS )
			pSharedMemory->Open(SHAREDMEMORY_NAME);
		else
			AfxMessageBox(_T("CSharedMemory Error"));
	}

	pMailThread = (CMailThread*)AfxBeginThread(RUNTIME_CLASS(CMailThread));
}

// PifComm_SetNotifyWnd
extern "C"
void PIFCOMMAPI PifComm_SetNotifyWnd(HWND hWnd)
{
	g_hNotifyWnd = hWnd;
}

// PifComm_DoStateDone
extern "C"
void PIFCOMMAPI PifComm_DoStateDone(int iAckCode)
{
	pMailThread->DoStateDone(iAckCode);
}

// P201 時刻合わせ要求
extern "C"
void PifComm_GetSetTime(SYSTEMTIME* pSystemTime)
{
	::CopyMemory(pSystemTime, &g_setTime, sizeof(SYSTEMTIME));
}

// P301 装置モニター情報
extern "C"
EQMONITORSET* PifComm_GetEqMonitorPtr()
{
	return pSharedMemory->GetSharedMemoryPtr();
}

// 2009.12.21 K.Matsuo -->
extern "C"
void PIFCOMMAPI PifComm_EqMonitortReport()
{
	CPifMessage301 pifMsg301;
	pifMsg301.SetItemLocalRemote(PifComm_GetEqMonitorPtr()->iLocalRemote);
	pifMsg301.SetItemEqProcessStatus(PifComm_GetEqMonitorPtr()->iEqProcessStatus);
	pifMsg301.SetItemAlarmStatus(PifComm_GetEqMonitorPtr()->iAlarmStatus);
	pifMsg301.SetItemGlassExist(PifComm_GetEqMonitorPtr()->iGlassExist);
	pifMsg301.SetItemLoadPosition(PifComm_GetEqMonitorPtr()->iLoadPosition);
	pifMsg301.SetItemVaccum1(PifComm_GetEqMonitorPtr()->iVaccumOn1);
	pifMsg301.SetItemShutterOpen(PifComm_GetEqMonitorPtr()->iShutterOpen);
	pifMsg301.SetItemShutterClose(PifComm_GetEqMonitorPtr()->iShutterClose);
	pifMsg301.SetItemPinUp(PifComm_GetEqMonitorPtr()->iPinUp);
	pifMsg301.SetItemPinDown(PifComm_GetEqMonitorPtr()->iPinDown);
	pifMsg301.SetItemArmSensor(PifComm_GetEqMonitorPtr()->iArmSensor);
	pifMsg301.SetItemDoorInterlock(PifComm_GetEqMonitorPtr()->iDoorInterlock);
	pifMsg301.SendRequ();
}
// 2009.12.21 K.Matsuo <--

// P303 レシピ一覧取得要求
extern "C" int PifComm_RequestKindOfRecipe()
{
	return g_iKindOfRecipe;
}

// P304 レシピ一覧報告
extern "C"
void PIFCOMMAPI PifComm_RecipeListReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate, UINT nOccurence)
{
	CPifMessage304 pifMsg304;
	pifMsg304.SetItemKindOfRecipe(iKindOfRecipe);
	for ( UINT i = 0; i < nOccurence; i++ ) {
		pifMsg304.SetItemRecipe(pstRecipeNameDate[i].szRecipeNameExt, &pstRecipeNameDate[i].recipeDateTime);
	}
	pifMsg304.SendRequ();
}

// P305 レシピ変更報告
extern "C"
void PIFCOMMAPI PifComm_ChangeRecipeDeleteReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate)
{
	CPifMessage305 pifMsg305;
	pifMsg305.SetItemKindOfRecipe(iKindOfRecipe);
	pifMsg305.SetItemRecipe(_TCHAR('D'), pstRecipeNameDate->szRecipeNameExt, &pstRecipeNameDate->recipeDateTime);
	pifMsg305.SendRequ();
}

extern "C"
void PIFCOMMAPI PifComm_ChangeRecipeInsertReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate)
{
	CPifMessage305 pifMsg305;
	pifMsg305.SetItemKindOfRecipe(iKindOfRecipe);
	pifMsg305.SetItemRecipe(_TCHAR('I'), pstRecipeNameDate->szRecipeNameExt, &pstRecipeNameDate->recipeDateTime);
	pifMsg305.SendRequ();
}

extern "C"
void PIFCOMMAPI PifComm_ChangeRecipeReplaceReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate)
{
	CPifMessage305 pifMsg305;
	pifMsg305.SetItemKindOfRecipe(iKindOfRecipe);
	pifMsg305.SetItemRecipe(_TCHAR('R'), pstRecipeNameDate->szRecipeNameExt, &pstRecipeNameDate->recipeDateTime);
	pifMsg305.SendRequ();
}

// P307 トレースデータ報告
extern "C"
void PIFCOMMAPI PifComm_TraceDataReport(const TRACEDATASET* pstTraceData)
{
	CPifMessage307 pifMsg307;
	pifMsg307.SetItemSrVisReflectLampLastExchange(&pstTraceData->SrVisReflectanceLamp.lastExchange);
	pifMsg307.SetItemSrVisReflectLampElapse(pstTraceData->SrVisReflectanceLamp.lElapseSecs);
	pifMsg307.SetItemSrVisReflectLampLifeTime(pstTraceData->SrVisReflectanceLamp.lWarningSecs);
	pifMsg307.SetItemSrVisTransmitLampLastExchange(&pstTraceData->SrVisTransmittanceLamp.lastExchange);
	pifMsg307.SetItemSrVisTransmitLampElapse(pstTraceData->SrVisTransmittanceLamp.lElapseSecs);
	pifMsg307.SetItemSrVisTransmitLampLifeTime(pstTraceData->SrVisTransmittanceLamp.lWarningSecs);
	pifMsg307.SetItemSrUvLampLastExchange(&pstTraceData->SrUvLamp.lastExchange);
	pifMsg307.SetItemSrUvLampElapse(pstTraceData->SrUvLamp.lElapseSecs);
	pifMsg307.SetItemSrUvLampLifeTime(pstTraceData->SrUvLamp.lWarningSecs);
	pifMsg307.SetItemSeVisReflectLampLastExchange(&pstTraceData->SeVisReflectanceLamp.lastExchange);
	pifMsg307.SetItemSeVisReflectLampElapse(pstTraceData->SeVisReflectanceLamp.lElapseSecs);
	pifMsg307.SetItemSeVisReflectLampLifeTime(pstTraceData->SeVisReflectanceLamp.lWarningSecs);
	pifMsg307.SetItemSeUvReflectLampLastExchange(&pstTraceData->SeUvLamp.lastExchange);
	pifMsg307.SetItemSeUvReflectLampElapse(pstTraceData->SeUvLamp.lElapseSecs);
	pifMsg307.SetItemSeUvReflectLampLifeTime(pstTraceData->SeUvLamp.lWarningSecs);
	pifMsg307.SendRequ();
}

// P308 サンプルサイズデータ取得要求
extern "C"
void PIFCOMMAPI PifComm_GetSampleSizeRecipeName(LPTSTR pszRecipeName)
{
	_tcscpy(pszRecipeName, g_szSampleSizeRecipeName);
}

// P309 サンプルサイズデータ報告
extern "C"
void PIFCOMMAPI PifComm_SampleSizeDataReport(double dX, double dY)
{
	CPifMessage309 pifMsg309;
	pifMsg309.SetItemSampleSizeX(dX);
	pifMsg309.SetItemSampleSizeY(dY);
	pifMsg309.SendRequ();
}

// P310 リファレンスデータ確認要求
extern "C"
void PIFCOMMAPI PifComm_GetReferenceDataCheckRecipeName(LPTSTR pszRecipeName)
{
	_tcscpy(pszRecipeName, g_szReferenceDataCheckRecipeName);
}

// P401 着工レシピ指示要求
// P411 リファレンス取得レシピ指示要求
extern "C"
void PIFCOMMAPI PifComm_GetRecipeName(LPTSTR pszRecipeName)
{
	_tcscpy(pszRecipeName, g_szRecipeName);
}

// P501 測定開始指示要求
// P511 リファレンス取得開始指示要求
extern "C"
void PIFCOMMAPI PifComm_GetSampleId(LPTSTR pszSampleId)
{
	_tcscpy(pszSampleId, g_szSampleId);
}

// P501 測定開始指示要求
// P511 リファレンス取得開始指示要求
extern "C"
void PIFCOMMAPI PifComm_GetLotId(LPTSTR pszLotId)
{
	_tcscpy(pszLotId, g_szLotId);
}

// P502 測定ポイント完了報告
extern "C"
void PIFCOMMAPI PifComm_CompleteMeasurePointReport(int iFormatId, int iPointNo, LPCTSTR pszAF_ST, const STAGE_COORD_XYZ* pCoordXyz, const char pszLabelOri[][ADAPRESULTSTRINGLENMAX + 1], const double* dData, const int* iSpecificWavelen)
{
	int i;
	char szBuff[256];
	char szLayer[16 + 1];
	CPifMessage502 pifMsg502;

	// 編集処理の準備 ： 関数ポインタの配列を作成
	i = 0;
	void (CPifMessage502::*pfnSetItemThick[8])(double);
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK1;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK2;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK3;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK4;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK5;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK6;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK7;
	pfnSetItemThick[i++] = &CPifMessage502::SetItemTHICK8;

	i = 0;
	void (CPifMessage502::*pfnSetItemN[8])(double);
	pfnSetItemN[i++] = &CPifMessage502::SetItemN1;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN2;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN3;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN4;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN5;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN6;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN7;
	pfnSetItemN[i++] = &CPifMessage502::SetItemN8;

	i = 0;
	void (CPifMessage502::*pfnSetItemK[8])(double);
	pfnSetItemK[i++] = &CPifMessage502::SetItemK1;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK2;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK3;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK4;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK5;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK6;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK7;
	pfnSetItemK[i++] = &CPifMessage502::SetItemK8;

	i = 0;
	void (CPifMessage502::*pfnSetItemCONST[8])(double);
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST1;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST2;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST3;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST4;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST5;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST6;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST7;
	pfnSetItemCONST[i++] = &CPifMessage502::SetItemCONST8;

	i = 0;
	void (CPifMessage502::*pfnSetItemREFLECT_WAVE[3])(double);
	pfnSetItemREFLECT_WAVE[i++] = &CPifMessage502::SetItemREFLECT1_WAVE;
	pfnSetItemREFLECT_WAVE[i++] = &CPifMessage502::SetItemREFLECT2_WAVE;
	pfnSetItemREFLECT_WAVE[i++] = &CPifMessage502::SetItemREFLECT3_WAVE;

	i = 0;
	void (CPifMessage502::*pfnSetItemREFLECT[3])(double);
	pfnSetItemREFLECT[i++] = &CPifMessage502::SetItemREFLECT1;
	pfnSetItemREFLECT[i++] = &CPifMessage502::SetItemREFLECT2;
	pfnSetItemREFLECT[i++] = &CPifMessage502::SetItemREFLECT3;

	i = 0;
	void (CPifMessage502::*pfnSetItemTRANSMIT_WAVE[3])(double);
	pfnSetItemTRANSMIT_WAVE[i++] = &CPifMessage502::SetItemTRANSMIT1_WAVE;
	pfnSetItemTRANSMIT_WAVE[i++] = &CPifMessage502::SetItemTRANSMIT2_WAVE;
	pfnSetItemTRANSMIT_WAVE[i++] = &CPifMessage502::SetItemTRANSMIT3_WAVE;

	i = 0;
	void (CPifMessage502::*pfnSetItemTRANSMIT[3])(double);
	pfnSetItemTRANSMIT[i++] = &CPifMessage502::SetItemTRANSMIT1;
	pfnSetItemTRANSMIT[i++] = &CPifMessage502::SetItemTRANSMIT2;
	pfnSetItemTRANSMIT[i++] = &CPifMessage502::SetItemTRANSMIT3;

	i = 0;
	void (CPifMessage502::*pfnSetItemEG[8])(double);
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG1;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG2;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG3;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG4;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG5;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG6;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG7;
	pfnSetItemEG[i++] = &CPifMessage502::SetItemEG8;

	i = 0;
	void (CPifMessage502::*pfnSetItemFRACTION[8])(double);
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION1;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION2;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION3;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION4;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION5;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION6;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION7;
	pfnSetItemFRACTION[i++] = &CPifMessage502::SetItemFRACTION8;

	// データをセットする
	pifMsg502.SetItemFormatId(iFormatId);
	pifMsg502.SetItemPointNo(iPointNo);
	// 001 SR膜厚, 004 SE, 007 EASE
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//	if ( iFormatId == 1 || iFormatId == 4 ) {
	if ( iFormatId == 1 || iFormatId == 4 || iFormatId == 7 ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		pifMsg502.SetItemAF_ST(pszAF_ST);
		pifMsg502.SetItemXCOORD(pCoordXyz->lX);
		pifMsg502.SetItemYCOORD(pCoordXyz->lY);
		pifMsg502.SetItemZCOORD(pCoordXyz->lZ);
		strcpy(szLayer, "00000000");

		int inc = 0;
//		while ( strcmp(pszLabelOri[inc], "") != 0 )		// これだと、うまく動作しない。
		while ( pszLabelOri[inc][0] != '\0' )
		{
			for ( int i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Thick%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					szLayer[i] = '1';
					(pifMsg502.*pfnSetItemThick[i])(dData[inc]);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "N%d", i + 1);
//				if ( strcmp(pszLabelOri[inc], szBuff) == 0 )
				if ( strstr(pszLabelOri[inc], szBuff) != NULL )	// ex.N1[633]
					(pifMsg502.*pfnSetItemN[i])(dData[inc]);
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "K%d", i + 1);
//				if ( strcmp(pszLabelOri[inc], szBuff) == 0 )
				if ( strstr(pszLabelOri[inc], szBuff) != NULL )	// ex.K1[633]
					(pifMsg502.*pfnSetItemK[i])(dData[inc]);
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Const%d", i + 1);
//				if ( strcmp(pszLabelOri[inc], szBuff) == 0 )
				if ( strstr(pszLabelOri[inc], szBuff) != NULL )	// ex.Const1
					(pifMsg502.*pfnSetItemCONST[i])(dData[inc]);
			}

			if ( strcmp(pszLabelOri[inc], "MSE") == 0 )
				pifMsg502.SetItemFIT(dData[inc]);

			for ( i = 0; i < 3; i++ ) {
				sprintf(szBuff, "Reflect%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					(pifMsg502.*pfnSetItemREFLECT_WAVE[i])((double)iSpecificWavelen[i]);
					(pifMsg502.*pfnSetItemREFLECT[i])(dData[inc]);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Eg%d", i + 1);
				if ( strstr(pszLabelOri[inc], szBuff) != NULL )	// ex.Eg1
					(pifMsg502.*pfnSetItemEG[i])(dData[inc]);
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Fraction%d", i + 1);
				if ( strstr(pszLabelOri[inc], szBuff) != NULL )	// ex.Fraction1
					(pifMsg502.*pfnSetItemFRACTION[i])(dData[inc]);
			}

			inc++;
		}
		pifMsg502.SetItemLAYERFLG(szLayer);
	}
	// 002 SR 反射率
	else if ( iFormatId == 2 ) {
		pifMsg502.SetItemAF_ST(pszAF_ST);
		pifMsg502.SetItemXCOORD(pCoordXyz->lX);
		pifMsg502.SetItemYCOORD(pCoordXyz->lY);
		pifMsg502.SetItemZCOORD(pCoordXyz->lZ);

		int inc = 0;
		while ( pszLabelOri[inc][0] != '\0' )
		{
			for ( i = 0; i < 3; i++ ) {
				sprintf(szBuff, "Reflect%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					(pifMsg502.*pfnSetItemREFLECT_WAVE[i])((double)iSpecificWavelen[i]);
					(pifMsg502.*pfnSetItemREFLECT[i])(dData[inc]);
				}
			}
			inc++;
		}
	}
	// 003 SR 透過率
	else if ( iFormatId == 3 ) {
		pifMsg502.SetItemAF_ST(pszAF_ST);
		pifMsg502.SetItemXCOORD(pCoordXyz->lX);
		pifMsg502.SetItemYCOORD(pCoordXyz->lY);
		pifMsg502.SetItemZCOORD(pCoordXyz->lZ);

		int inc = 0;
		while ( pszLabelOri[inc][0] != '\0' )
		{
			for ( i = 0; i < 3; i++ ) {
				sprintf(szBuff, "Transmit%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					(pifMsg502.*pfnSetItemTRANSMIT_WAVE[i])((double)iSpecificWavelen[i]);
					(pifMsg502.*pfnSetItemTRANSMIT[i])(dData[inc]);
				}
			}
			inc++;
		}
	}
	else
		return;		// ASSERT( FALSE );

	pifMsg502.SendRequ();
}

// 005 RS
extern "C"
PIFCOMMAPI void PifComm_CompleteMeasureResistPointReport(int iPointNo, LPCTSTR /*pszAF_ST*/, const STAGE_COORD_XYZ* pCoordXyz, TCHAR chDataValid, LPCTSTR pszResistance, LPCTSTR pszSurfaceResistivity, LPCTSTR pszVolumeResistivity, LPCTSTR pszConductivity)
{
	CPifMessage502 pifMsg502;
	const int iFormatId = 5;

	TCHAR szDataValid[1 + 1];

	pifMsg502.SetItemFormatId(iFormatId);
	pifMsg502.SetItemPointNo(iPointNo);
//	pifMsg502.SetItemAF_ST(pszAF_ST);
	pifMsg502.SetItemXCOORD(pCoordXyz->lX);
	pifMsg502.SetItemYCOORD(pCoordXyz->lY);
	pifMsg502.SetItemZCOORD(pCoordXyz->lZ);
	szDataValid[0] = chDataValid;
	szDataValid[1] = '\0';
	pifMsg502.SetItemRSDATAVALID(szDataValid);
	pifMsg502.SetItemRSRESIST(pszResistance);
	pifMsg502.SetItemRSSURFACERESIST(pszSurfaceResistivity);
	pifMsg502.SetItemRSVOLUMERESIST(pszVolumeResistivity);
	pifMsg502.SetItemRSCONDUCTIVITY(pszConductivity);

	pifMsg502.SendRequ();
}

// 006 CA
extern "C"
PIFCOMMAPI void PifComm_CompleteMeasureContactAnglePointReport(int iPointNo, LPCTSTR /*pszAF_ST*/, const STAGE_COORD_XYZ* pCoordXyz, double dCAngle, double dRadius, double dLiquid)
{
	CPifMessage502 pifMsg502;
	const int iFormatId = 6;

	pifMsg502.SetItemFormatId(iFormatId);
	pifMsg502.SetItemPointNo(iPointNo);
//	pifMsg502.SetItemAF_ST(pszAF_ST);
	pifMsg502.SetItemXCOORD(pCoordXyz->lX);
	pifMsg502.SetItemYCOORD(pCoordXyz->lY);
	pifMsg502.SetItemZCOORD(pCoordXyz->lZ);
	pifMsg502.SetItemCACONTACTANGLE(dCAngle);
	pifMsg502.SetItemCARADIUS(dRadius);
	pifMsg502.SetItemCALIQUID(dLiquid);

	pifMsg502.SendRequ();
}

extern "C"
void PIFCOMMAPI PifComm_CompleteMeasureStressLineReport(int iMeasMode, int iLineNo, const STAGE_PROG_STRESS* pStageProgStress, const double* dStress)
{
	int i;
	CPifMessage502 pifMsg502;
	const int iFormatId = 101;

	// 編集処理の準備 ： 関数ポインタの配列を作成
	i = 0;
	void (CPifMessage502::*pfnSetItemSTRLxSx_Valid[20])(int);
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS1_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS2_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS3_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS4_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS5_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS6_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS7_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS8_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS9_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS10_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS11_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS12_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS13_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS14_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS15_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS16_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS17_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS18_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS19_Valid;
	pfnSetItemSTRLxSx_Valid[i++] = &CPifMessage502::SetItemSTRLxS20_Valid;

	i = 0;
	void (CPifMessage502::*pfnSetItemSTRLxSx_StartX[20])(double);
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS1_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS2_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS3_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS4_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS5_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS6_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS7_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS8_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS9_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS10_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS11_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS12_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS13_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS14_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS15_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS16_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS17_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS18_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS19_StartX;
	pfnSetItemSTRLxSx_StartX[i++] = &CPifMessage502::SetItemSTRLxS20_StartX;

	i = 0;
	void (CPifMessage502::*pfnSetItemSTRLxSx_StartY[20])(double);
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS1_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS2_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS3_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS4_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS5_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS6_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS7_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS8_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS9_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS10_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS11_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS12_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS13_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS14_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS15_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS16_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS17_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS18_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS19_StartY;
	pfnSetItemSTRLxSx_StartY[i++] = &CPifMessage502::SetItemSTRLxS20_StartY;

	i = 0;
	void (CPifMessage502::*pfnSetItemSTRLxSx_EndX[20])(double);
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS1_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS2_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS3_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS4_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS5_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS6_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS7_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS8_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS9_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS10_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS11_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS12_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS13_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS14_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS15_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS16_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS17_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS18_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS19_EndX;
	pfnSetItemSTRLxSx_EndX[i++] = &CPifMessage502::SetItemSTRLxS20_EndX;

	i = 0;
	void (CPifMessage502::*pfnSetItemSTRLxSx_EndY[20])(double);
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS1_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS2_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS3_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS4_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS5_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS6_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS7_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS8_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS9_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS10_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS11_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS12_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS13_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS14_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS15_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS16_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS17_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS18_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS19_EndY;
	pfnSetItemSTRLxSx_EndY[i++] = &CPifMessage502::SetItemSTRLxS20_EndY;

	i = 0;
	void (CPifMessage502::*pfnSetItemSTRLxSx_Stress[20])(double);
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS1_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS2_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS3_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS4_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS5_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS6_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS7_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS8_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS9_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS10_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS11_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS12_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS13_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS14_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS15_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS16_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS17_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS18_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS19_Stress;
	pfnSetItemSTRLxSx_Stress[i++] = &CPifMessage502::SetItemSTRLxS20_Stress;

	// データをセットする
	pifMsg502.SetItemFormatId(iFormatId);
	pifMsg502.SetItemLineNo(iLineNo);

	pifMsg502.SetItemSTRMEASMODE(iMeasMode);
	pifMsg502.SetItemSTRLx_Valid(1);
	for ( i = 0; i < 20; i++ ) {
	   	(pifMsg502.*pfnSetItemSTRLxSx_Valid[i])(pStageProgStress->Line[iLineNo-1].bScanValid[i] == TRUE ? 1 : 0);
	   	(pifMsg502.*pfnSetItemSTRLxSx_StartX[i])(pStageProgStress->Line[iLineNo-1].SectPos[i].lScanStartPosX);
	   	(pifMsg502.*pfnSetItemSTRLxSx_StartY[i])(pStageProgStress->Line[iLineNo-1].SectPos[i].lScanPosY);
	   	(pifMsg502.*pfnSetItemSTRLxSx_EndX[i])(pStageProgStress->Line[iLineNo-1].SectPos[i].lScanEndPosX);
	   	(pifMsg502.*pfnSetItemSTRLxSx_EndY[i])(pStageProgStress->Line[iLineNo-1].SectPos[i].lScanPosY);
	   	(pifMsg502.*pfnSetItemSTRLxSx_Stress[i])(dStress[i]);
	}

	pifMsg502.SendRequ();
}

// P503 測定完了報告
extern "C"
void PIFCOMMAPI PifComm_CompleteMeasureAllPointsReport(int iFormatId, int iTotalPointCnt, const char pszLabelOri[][ADAPRESULTSTRINGLENMAX + 1], const STATISTICSDATESET* pStatistics, const int* iSpecificWavelen)
{
	int i;
	char szBuff[256];
	char szLayer[16 + 1];
	CPifMessage503 pifMsg503;

	// 編集処理の準備 ： 関数ポインタの配列を作成
	i = 0;
	void (CPifMessage503::*pfnSetItemThick_Min[8])(double);
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK1_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK2_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK3_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK4_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK5_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK6_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK7_MIN;
	pfnSetItemThick_Min[i++] = &CPifMessage503::SetItemTHICK8_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemThick_Max[8])(double);
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK1_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK2_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK3_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK4_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK5_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK6_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK7_MAX;
	pfnSetItemThick_Max[i++] = &CPifMessage503::SetItemTHICK8_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemThick_Ave[8])(double);
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK1_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK2_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK3_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK4_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK5_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK6_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK7_AVE;
	pfnSetItemThick_Ave[i++] = &CPifMessage503::SetItemTHICK8_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemThick_Sd[8])(double);
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK1_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK2_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK3_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK4_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK5_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK6_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK7_SD;
	pfnSetItemThick_Sd[i++] = &CPifMessage503::SetItemTHICK8_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemThick_Unif[8])(double);
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK1_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK2_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK3_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK4_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK5_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK6_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK7_UNIF;
	pfnSetItemThick_Unif[i++] = &CPifMessage503::SetItemTHICK8_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemN_Min[8])(double);
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN1_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN2_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN3_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN4_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN5_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN6_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN7_MIN;
	pfnSetItemN_Min[i++] = &CPifMessage503::SetItemN8_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemN_Max[8])(double);
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN1_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN2_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN3_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN4_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN5_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN6_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN7_MAX;
	pfnSetItemN_Max[i++] = &CPifMessage503::SetItemN8_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemN_Ave[8])(double);
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN1_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN2_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN3_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN4_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN5_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN6_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN7_AVE;
	pfnSetItemN_Ave[i++] = &CPifMessage503::SetItemN8_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemN_Sd[8])(double);
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN1_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN2_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN3_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN4_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN5_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN6_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN7_SD;
	pfnSetItemN_Sd[i++] = &CPifMessage503::SetItemN8_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemN_Unif[8])(double);
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN1_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN2_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN3_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN4_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN5_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN6_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN7_UNIF;
	pfnSetItemN_Unif[i++] = &CPifMessage503::SetItemN8_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemK_Min[8])(double);
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK1_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK2_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK3_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK4_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK5_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK6_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK7_MIN;
	pfnSetItemK_Min[i++] = &CPifMessage503::SetItemK8_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemK_Max[8])(double);
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK1_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK2_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK3_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK4_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK5_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK6_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK7_MAX;
	pfnSetItemK_Max[i++] = &CPifMessage503::SetItemK8_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemK_Ave[8])(double);
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK1_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK2_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK3_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK4_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK5_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK6_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK7_AVE;
	pfnSetItemK_Ave[i++] = &CPifMessage503::SetItemK8_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemK_Sd[8])(double);
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK1_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK2_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK3_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK4_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK5_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK6_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK7_SD;
	pfnSetItemK_Sd[i++] = &CPifMessage503::SetItemK8_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemK_Unif[8])(double);
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK1_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK2_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK3_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK4_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK5_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK6_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK7_UNIF;
	pfnSetItemK_Unif[i++] = &CPifMessage503::SetItemK8_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemConst_Min[8])(double);
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST1_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST2_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST3_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST4_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST5_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST6_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST7_MIN;
	pfnSetItemConst_Min[i++] = &CPifMessage503::SetItemCONST8_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemConst_Max[8])(double);
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST1_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST2_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST3_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST4_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST5_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST6_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST7_MAX;
	pfnSetItemConst_Max[i++] = &CPifMessage503::SetItemCONST8_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemConst_Ave[8])(double);
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST1_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST2_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST3_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST4_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST5_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST6_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST7_AVE;
	pfnSetItemConst_Ave[i++] = &CPifMessage503::SetItemCONST8_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemConst_Sd[8])(double);
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST1_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST2_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST3_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST4_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST5_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST6_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST7_SD;
	pfnSetItemConst_Sd[i++] = &CPifMessage503::SetItemCONST8_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemConst_Unif[8])(double);
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST1_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST2_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST3_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST4_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST5_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST6_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST7_UNIF;
	pfnSetItemConst_Unif[i++] = &CPifMessage503::SetItemCONST8_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemReflect_Wave[3])(double);
	pfnSetItemReflect_Wave[i++] = &CPifMessage503::SetItemREFLECT1_WAVE;
	pfnSetItemReflect_Wave[i++] = &CPifMessage503::SetItemREFLECT2_WAVE;
	pfnSetItemReflect_Wave[i++] = &CPifMessage503::SetItemREFLECT3_WAVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemReflect_Min[3])(double);
	pfnSetItemReflect_Min[i++] = &CPifMessage503::SetItemREFLECT1_MIN;
	pfnSetItemReflect_Min[i++] = &CPifMessage503::SetItemREFLECT2_MIN;
	pfnSetItemReflect_Min[i++] = &CPifMessage503::SetItemREFLECT3_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemReflect_Max[3])(double);
	pfnSetItemReflect_Max[i++] = &CPifMessage503::SetItemREFLECT1_MAX;
	pfnSetItemReflect_Max[i++] = &CPifMessage503::SetItemREFLECT2_MAX;
	pfnSetItemReflect_Max[i++] = &CPifMessage503::SetItemREFLECT3_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemReflect_Ave[3])(double);
	pfnSetItemReflect_Ave[i++] = &CPifMessage503::SetItemREFLECT1_AVE;
	pfnSetItemReflect_Ave[i++] = &CPifMessage503::SetItemREFLECT2_AVE;
	pfnSetItemReflect_Ave[i++] = &CPifMessage503::SetItemREFLECT3_AVE;
	i = 0;
	void (CPifMessage503::*pfnSetItemReflect_Sd[3])(double);
	pfnSetItemReflect_Sd[i++] = &CPifMessage503::SetItemREFLECT1_SD;
	pfnSetItemReflect_Sd[i++] = &CPifMessage503::SetItemREFLECT2_SD;
	pfnSetItemReflect_Sd[i++] = &CPifMessage503::SetItemREFLECT3_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemReflect_Unif[3])(double);
	pfnSetItemReflect_Unif[i++] = &CPifMessage503::SetItemREFLECT1_UNIF;
	pfnSetItemReflect_Unif[i++] = &CPifMessage503::SetItemREFLECT2_UNIF;
	pfnSetItemReflect_Unif[i++] = &CPifMessage503::SetItemREFLECT3_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemTransmit_Wave[3])(double);
	pfnSetItemTransmit_Wave[i++] = &CPifMessage503::SetItemTRANSMIT1_WAVE;
	pfnSetItemTransmit_Wave[i++] = &CPifMessage503::SetItemTRANSMIT2_WAVE;
	pfnSetItemTransmit_Wave[i++] = &CPifMessage503::SetItemTRANSMIT3_WAVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemTransmit_Min[3])(double);
	pfnSetItemTransmit_Min[i++] = &CPifMessage503::SetItemTRANSMIT1_MIN;
	pfnSetItemTransmit_Min[i++] = &CPifMessage503::SetItemTRANSMIT2_MIN;
	pfnSetItemTransmit_Min[i++] = &CPifMessage503::SetItemTRANSMIT3_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemTransmit_Max[3])(double);
	pfnSetItemTransmit_Max[i++] = &CPifMessage503::SetItemTRANSMIT1_MAX;
	pfnSetItemTransmit_Max[i++] = &CPifMessage503::SetItemTRANSMIT2_MAX;
	pfnSetItemTransmit_Max[i++] = &CPifMessage503::SetItemTRANSMIT3_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemTransmit_Ave[3])(double);
	pfnSetItemTransmit_Ave[i++] = &CPifMessage503::SetItemTRANSMIT1_AVE;
	pfnSetItemTransmit_Ave[i++] = &CPifMessage503::SetItemTRANSMIT2_AVE;
	pfnSetItemTransmit_Ave[i++] = &CPifMessage503::SetItemTRANSMIT3_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemTransmit_Sd[3])(double);
	pfnSetItemTransmit_Sd[i++] = &CPifMessage503::SetItemTRANSMIT1_SD;
	pfnSetItemTransmit_Sd[i++] = &CPifMessage503::SetItemTRANSMIT2_SD;
	pfnSetItemTransmit_Sd[i++] = &CPifMessage503::SetItemTRANSMIT3_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemTransmit_Unif[3])(double);
	pfnSetItemTransmit_Unif[i++] = &CPifMessage503::SetItemTRANSMIT1_UNIF;
	pfnSetItemTransmit_Unif[i++] = &CPifMessage503::SetItemTRANSMIT2_UNIF;
	pfnSetItemTransmit_Unif[i++] = &CPifMessage503::SetItemTRANSMIT3_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemEG_Min[8])(double);
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG1_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG2_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG3_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG4_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG5_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG6_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG7_MIN;
	pfnSetItemEG_Min[i++] = &CPifMessage503::SetItemEG8_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemEG_Max[8])(double);
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG1_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG2_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG3_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG4_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG5_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG6_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG7_MAX;
	pfnSetItemEG_Max[i++] = &CPifMessage503::SetItemEG8_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemEG_Ave[8])(double);
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG1_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG2_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG3_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG4_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG5_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG6_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG7_AVE;
	pfnSetItemEG_Ave[i++] = &CPifMessage503::SetItemEG8_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemEG_Sd[8])(double);
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG1_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG2_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG3_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG4_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG5_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG6_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG7_SD;
	pfnSetItemEG_Sd[i++] = &CPifMessage503::SetItemEG8_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemEG_Unif[8])(double);
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG1_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG2_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG3_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG4_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG5_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG6_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG7_UNIF;
	pfnSetItemEG_Unif[i++] = &CPifMessage503::SetItemEG8_UNIF;

	i = 0;
	void (CPifMessage503::*pfnSetItemFRACTION_Min[8])(double);
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION1_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION2_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION3_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION4_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION5_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION6_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION7_MIN;
	pfnSetItemFRACTION_Min[i++] = &CPifMessage503::SetItemFRACTION8_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemFRACTION_Max[8])(double);
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION1_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION2_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION3_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION4_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION5_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION6_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION7_MAX;
	pfnSetItemFRACTION_Max[i++] = &CPifMessage503::SetItemFRACTION8_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemFRACTION_Ave[8])(double);
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION1_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION2_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION3_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION4_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION5_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION6_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION7_AVE;
	pfnSetItemFRACTION_Ave[i++] = &CPifMessage503::SetItemFRACTION8_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemFRACTION_Sd[8])(double);
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION1_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION2_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION3_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION4_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION5_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION6_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION7_SD;
	pfnSetItemFRACTION_Sd[i++] = &CPifMessage503::SetItemFRACTION8_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemFRACTION_Unif[8])(double);
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION1_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION2_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION3_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION4_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION5_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION6_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION7_UNIF;
	pfnSetItemFRACTION_Unif[i++] = &CPifMessage503::SetItemFRACTION8_UNIF;

	// データをセットする
	pifMsg503.SetItemFormatId(iFormatId);
	pifMsg503.SetItemPointNo(iTotalPointCnt);
	// 001 SR膜厚, 004 SE, 007 EASE
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//	if ( iFormatId == 1 || iFormatId == 4 ) {
	if ( iFormatId == 1 || iFormatId == 4 || iFormatId == 7 ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		strcpy(szLayer, "00000000");

		int inc = 0;
//		while ( strcmp(pszLabelOri[inc], "") != 0 )		// これだと、うまく動作しない。
		while ( pszLabelOri[inc][0] != '\0' )
		{
			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Thick%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					szLayer[i] = '1';
					(pifMsg503.*pfnSetItemThick_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemThick_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemThick_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemThick_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemThick_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "N%d", i + 1);
//				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
				if ( strstr(pszLabelOri[inc], szBuff) != NULL ) {		// ex.N1[633]
					(pifMsg503.*pfnSetItemN_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemN_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemN_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemN_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemN_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "K%d", i + 1);
//				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
				if ( strstr(pszLabelOri[inc], szBuff) != NULL ) {		// ex.K1[633]
					(pifMsg503.*pfnSetItemK_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemK_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemK_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemK_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemK_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Const%d", i + 1);
//				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
				if ( strstr(pszLabelOri[inc], szBuff) != NULL ) {		// ex.Const1
					(pifMsg503.*pfnSetItemConst_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemConst_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemConst_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemConst_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemConst_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			for ( i = 0; i < 3; i++ ) {
				sprintf(szBuff, "Reflect%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					(pifMsg503.*pfnSetItemReflect_Wave[i])((double)iSpecificWavelen[i]);
					(pifMsg503.*pfnSetItemReflect_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemReflect_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemReflect_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemReflect_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemReflect_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Eg%d", i + 1);
				if ( strstr(pszLabelOri[inc], szBuff) != NULL ) {		// ex.Eg1
					(pifMsg503.*pfnSetItemEG_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemEG_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemEG_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemEG_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemEG_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			for ( i = 0; i < 8; i++ ) {
				sprintf(szBuff, "Fraction%d", i + 1);
				if ( strstr(pszLabelOri[inc], szBuff) != NULL ) {		// ex.Fraction1
					(pifMsg503.*pfnSetItemFRACTION_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemFRACTION_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemFRACTION_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemFRACTION_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemFRACTION_Unif[i])(pStatistics[inc].dUnif);
				}
			}

			inc++;
		}
		pifMsg503.SetItemLAYERFLG(szLayer);
	}
	// 002 SR 反射率
	else if ( iFormatId == 2 ) {
		int inc = 0;
		while ( pszLabelOri[inc][0] != '\0' )
		{
			for ( i = 0; i < 3; i++ ) {
				sprintf(szBuff, "Reflect%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					(pifMsg503.*pfnSetItemReflect_Wave[i])((double)iSpecificWavelen[i]);
					(pifMsg503.*pfnSetItemReflect_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemReflect_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemReflect_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemReflect_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemReflect_Unif[i])(pStatistics[inc].dUnif);
				}
			}
			inc++;
		}
	}
	// 003 SR 透過率
	else if ( iFormatId == 3 ) {
		int inc = 0;
		while ( pszLabelOri[inc][0] != '\0' )
		{
			for ( i = 0; i < 3; i++ ) {
				sprintf(szBuff, "Transmit%d", i + 1);
				if ( strcmp(pszLabelOri[inc], szBuff) == 0 ) {
					(pifMsg503.*pfnSetItemTransmit_Wave[i])((double)iSpecificWavelen[i]);
					(pifMsg503.*pfnSetItemTransmit_Min[i])(pStatistics[inc].dMin);
					(pifMsg503.*pfnSetItemTransmit_Max[i])(pStatistics[inc].dMax);
					(pifMsg503.*pfnSetItemTransmit_Ave[i])(pStatistics[inc].dAve);
					(pifMsg503.*pfnSetItemTransmit_Sd[i])(pStatistics[inc].dSd);
					(pifMsg503.*pfnSetItemTransmit_Unif[i])(pStatistics[inc].dUnif);
				}
			}
			inc++;
		}
	}
	else
		return;		// ASSERT( FALSE );

	pifMsg503.SendRequ();
}

// 005 RS
extern "C"
PIFCOMMAPI void PifComm_CompleteMeasureResistAllPointsReport(int iTotalPointCnt, const STATISTICSDATESET* pStatistics)
{
	int i = 0;
	TCHAR szBuff[16 + 1];
	CPifMessage503 pifMsg503;
	const int iFormatId = 5;

	pifMsg503.SetItemFormatId(iFormatId);
	pifMsg503.SetItemPointNo(iTotalPointCnt);

	ConvertToExpNumber(pStatistics[i].dMin, szBuff);	pifMsg503.SetItemRSRESIST_MIN(szBuff);
	ConvertToExpNumber(pStatistics[i].dMax, szBuff);	pifMsg503.SetItemRSRESIST_MAX(szBuff);
	ConvertToExpNumber(pStatistics[i].dAve, szBuff);	pifMsg503.SetItemRSRESIST_AVE(szBuff);
	pifMsg503.SetItemRSRESIST_STD(pStatistics[i].dSd);
	pifMsg503.SetItemRSRESIST_UNIF(pStatistics[i].dUnif);
	i++;
	ConvertToExpNumber(pStatistics[i].dMin, szBuff);	pifMsg503.SetItemRSSURFACE_MIN(szBuff);
	ConvertToExpNumber(pStatistics[i].dMax, szBuff);	pifMsg503.SetItemRSSURFACE_MAX(szBuff);
	ConvertToExpNumber(pStatistics[i].dAve, szBuff);	pifMsg503.SetItemRSSURFACE_AVE(szBuff);
	pifMsg503.SetItemRSSURFACE_STD(pStatistics[i].dSd);
	pifMsg503.SetItemRSSURFACE_UNIF(pStatistics[i].dUnif);
	i++;
	ConvertToExpNumber(pStatistics[i].dMin, szBuff);	pifMsg503.SetItemRSVOLUME_MIN(szBuff);
	ConvertToExpNumber(pStatistics[i].dMax, szBuff);	pifMsg503.SetItemRSVOLUME_MAX(szBuff);
	ConvertToExpNumber(pStatistics[i].dAve, szBuff);	pifMsg503.SetItemRSVOLUME_AVE(szBuff);
	pifMsg503.SetItemRSVOLUME_STD(pStatistics[i].dSd);
	pifMsg503.SetItemRSVOLUME_UNIF(pStatistics[i].dUnif);
	i++;
	ConvertToExpNumber(pStatistics[i].dMin, szBuff);	pifMsg503.SetItemRSCONDUCT_MIN(szBuff);
	ConvertToExpNumber(pStatistics[i].dMax, szBuff);	pifMsg503.SetItemRSCONDUCT_MAX(szBuff);
	ConvertToExpNumber(pStatistics[i].dAve, szBuff);	pifMsg503.SetItemRSCONDUCT_AVE(szBuff);
	pifMsg503.SetItemRSCONDUCT_STD(pStatistics[i].dSd);
	pifMsg503.SetItemRSCONDUCT_UNIF(pStatistics[i].dUnif);
	i++;

	pifMsg503.SendRequ();
}

// 006 CA
extern "C"
PIFCOMMAPI void PifComm_CompleteMeasureContactAngleAllPointsReport(int iTotalPointCnt, const STATISTICSDATESET* pStatistics)
{
	int i = 0;
	CPifMessage503 pifMsg503;
	const int iFormatId = 6;

	pifMsg503.SetItemFormatId(iFormatId);
	pifMsg503.SetItemPointNo(iTotalPointCnt);

	pifMsg503.SetItemCAANGLE_MIN(pStatistics[i].dMin);
	pifMsg503.SetItemCAANGLE_MAX(pStatistics[i].dMax);
	pifMsg503.SetItemCAANGLE_AVE(pStatistics[i].dAve);
	pifMsg503.SetItemCAANGLE_STD(pStatistics[i].dSd);
	pifMsg503.SetItemCAANGLE_UNIF(pStatistics[i].dUnif);
	i++;
	pifMsg503.SetItemCARADIUS_MIN(pStatistics[i].dMin);
	pifMsg503.SetItemCARADIUS_MAX(pStatistics[i].dMax);
	pifMsg503.SetItemCARADIUS_AVE(pStatistics[i].dAve);
	pifMsg503.SetItemCARADIUS_STD(pStatistics[i].dSd);
	pifMsg503.SetItemCARADIUS_UNIF(pStatistics[i].dUnif);
	i++;
	pifMsg503.SetItemCALIQUID_MIN(pStatistics[i].dMin);
	pifMsg503.SetItemCALIQUID_MAX(pStatistics[i].dMax);
	pifMsg503.SetItemCALIQUID_AVE(pStatistics[i].dAve);
	pifMsg503.SetItemCALIQUID_STD(pStatistics[i].dSd);
	pifMsg503.SetItemCALIQUID_UNIF(pStatistics[i].dUnif);
	i++;

	pifMsg503.SendRequ();
}

extern "C"
void PIFCOMMAPI PifComm_CompleteMeasureStressAllLinesReport(int iMeasMode, const STAGE_PROG_STRESS* pStageProgStress, const STATISTICSDATESET* pStatistics)
{
	int i;
	int iValidLineNum;
	char szBuff[256];
	CPifMessage503 pifMsg503;
	const int iFormatId = 101;

	// 編集処理の準備 ： 関数ポインタの配列を作成
	i = 0;
	void (CPifMessage503::*pfnSetItemSTRLx_Min[21])(double);
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL1_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL2_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL3_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL4_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL5_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL6_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL7_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL8_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL9_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL10_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL11_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL12_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL13_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL14_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL15_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL16_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL17_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL18_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL19_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRL20_MIN;
	pfnSetItemSTRLx_Min[i++] = &CPifMessage503::SetItemSTRWA_MIN;

	i = 0;
	void (CPifMessage503::*pfnSetItemSTRLx_Max[21])(double);
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL1_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL2_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL3_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL4_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL5_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL6_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL7_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL8_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL9_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL10_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL11_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL12_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL13_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL14_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL15_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL16_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL17_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL18_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL19_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRL20_MAX;
	pfnSetItemSTRLx_Max[i++] = &CPifMessage503::SetItemSTRWA_MAX;

	i = 0;
	void (CPifMessage503::*pfnSetItemSTRLx_Ave[21])(double);
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL1_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL2_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL3_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL4_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL5_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL6_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL7_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL8_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL9_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL10_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL11_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL12_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL13_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL14_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL15_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL16_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL17_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL18_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL19_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRL20_AVE;
	pfnSetItemSTRLx_Ave[i++] = &CPifMessage503::SetItemSTRWA_AVE;

	i = 0;
	void (CPifMessage503::*pfnSetItemSTRLx_Sd[21])(double);
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL1_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL2_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL3_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL4_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL5_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL6_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL7_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL8_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL9_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL10_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL11_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL12_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL13_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL14_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL15_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL16_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL17_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL18_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL19_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRL20_SD;
	pfnSetItemSTRLx_Sd[i++] = &CPifMessage503::SetItemSTRWA_SD;

	i = 0;
	void (CPifMessage503::*pfnSetItemSTRLx_Unif[21])(double);
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL1_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL2_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL3_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL4_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL5_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL6_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL7_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL8_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL9_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL10_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL11_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL12_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL13_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL14_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL15_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL16_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL17_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL18_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL19_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRL20_UNIF;
	pfnSetItemSTRLx_Unif[i++] = &CPifMessage503::SetItemSTRWA_UNIF;


	// データをセットする
	pifMsg503.SetItemFormatId(iFormatId);
	pifMsg503.SetItemSTRMEASMODE(iMeasMode);

	::ZeroMemory(szBuff, sizeof(szBuff));
	iValidLineNum = 0;
	for ( i = 0; i < 16; i++ ) {
	   	if ( pStageProgStress->Line[i].bValidLine ) {
			szBuff[i] = '1';
			iValidLineNum++;
		}
		else {
			szBuff[i] = '0';
		}
	}
	pifMsg503.SetItemSTRLxValidFLG0(szBuff);
	::ZeroMemory(szBuff, sizeof(szBuff));
	for ( ; i < 20; i++ ) {
	   	if ( pStageProgStress->Line[i].bValidLine ) {
		   	szBuff[i-16] = '1';
			iValidLineNum++;
		}
		else {
		   	szBuff[i-16] = '0';
		}
	}
	pifMsg503.SetItemSTRLxValidFLG1(szBuff);
	pifMsg503.SetItemLineNo(iValidLineNum);

	for ( i = 0; i < 21; i++ ) {			// WAも含むので、21
		(pifMsg503.*pfnSetItemSTRLx_Min[i])(pStatistics[i].dMin);
		(pifMsg503.*pfnSetItemSTRLx_Max[i])(pStatistics[i].dMax);
		(pifMsg503.*pfnSetItemSTRLx_Ave[i])(pStatistics[i].dAve);
		(pifMsg503.*pfnSetItemSTRLx_Sd[i])(pStatistics[i].dSd);
		(pifMsg503.*pfnSetItemSTRLx_Unif[i])(pStatistics[i].dUnif);
	}

	pifMsg503.SendRequ();
}


// P504 測定キャンセルボタン押下報告
extern "C"
void PIFCOMMAPI PifComm_PressMeasureCancelButtonReport()
{
	CPifMessage504 pifMsg504;
	pifMsg504.SendRequ();
}

// 2009.10.29 K.Matsuo リファレンス取得機能追加 -->
// P512 リファレンス取得ポイント完了報告
extern "C"
PIFCOMMAPI void PifComm_CompleteReferencePointReport(int iPointNo)
{
	CPifMessage512 pifMsg512;
	pifMsg512.SetItemPointNo(iPointNo);
	pifMsg512.SendRequ();
}

// P513 リファレンス取得完了報告
extern "C"
PIFCOMMAPI void PifComm_CompleteReferenceAllPointsReport(int iTotalPointCnt)
{
	CPifMessage513 pifMsg513;
	pifMsg513.SetItemPointNo(iTotalPointCnt);
	pifMsg513.SendRequ();
}
// 2009.10.29 K.Matsuo リファレンス取得機能追加 <--

// 2014.01.07 bagus Add(Stage None対応) -->
// P515 Siリファレンス取得完了報告
extern "C"
void PIFCOMMAPI PifComm_CompleteSiReferenceDoneReport(int iReferenceResult)
{
	CPifMessage515 pifMsg515;
	pifMsg515.SetItemReferenceResult(iReferenceResult);
	pifMsg515.SendRequ();
}

// P517 Darkリファレンス取得完了報告
extern "C"
void PIFCOMMAPI PifComm_CompleteDarkReferenceDoneReport(int iReferenceResult)
{
	CPifMessage517 pifMsg517;
	pifMsg517.SetItemReferenceResult(iReferenceResult);
	pifMsg517.SendRequ();
}

// 2014.01.07 bagus Add(Stage None対応) <--

// P601 ロードポジション移行指示要求
extern "C"
void PIFCOMMAPI PifComm_GetSampleSize(double* dX, double* dY)
{
	*dX = g_dSampleSizeX;
	*dY = g_dSampleSizeY;
}

// P602 汎用ポジション移行指示要求
extern "C"
int PIFCOMMAPI PifComm_GetMovePosId()
{
	return g_iMovePosId;
}

// P701 ロードポジション移行結果報告
extern "C"
void PIFCOMMAPI PifComm_LoadPositionMovementResultReport(int iMovementResult)
{
	CPifMessage701 pifMsg701;
	pifMsg701.SetItemMovementResult(iMovementResult);
	pifMsg701.SendRequ();
}

// P702 汎用ポジション移行結果報告
extern "C"
void PIFCOMMAPI PifComm_GeneralPurposePositionMovementResultReport(int iMovementResult)
{
	CPifMessage702 pifMsg702;
	pifMsg702.SetItemMovementResult(iMovementResult);
	pifMsg702.SendRequ();
}

// P703 バキュームON指示結果報告
extern "C"
void PIFCOMMAPI PifComm_VaccumOnResultReport(int iMovementResult)
{
	CPifMessage703 pifMsg703;
	pifMsg703.SetItemMovementResult(iMovementResult);
	pifMsg703.SendRequ();
}

// P704 バキュームOFF指示結果報告
extern "C"
void PIFCOMMAPI PifComm_VaccumOffResultReport(int iMovementResult)
{
	CPifMessage704 pifMsg704;
	pifMsg704.SetItemMovementResult(iMovementResult);
	pifMsg704.SendRequ();
}

// P705 アライメントLOAD指示結果報告
extern "C"
void PIFCOMMAPI PifComm_AlignmentLoadResultReport(int iMovementResult)
{
	CPifMessage705 pifMsg705;
	pifMsg705.SetItemMovementResult(iMovementResult);
	pifMsg705.SendRequ();
}

// P706 アライメントUNLOAD指示結果報告
extern "C"
void PIFCOMMAPI PifComm_AlignmentUnloadResultReport(int iMovementResult)
{
	CPifMessage706 pifMsg706;
	pifMsg706.SetItemMovementResult(iMovementResult);
	pifMsg706.SendRequ();
}

// P707 PINUP指示結果報告
extern "C"
void PIFCOMMAPI PifComm_PinUpResultReport(int iMovementResult)
{
	CPifMessage707 pifMsg707;
	pifMsg707.SetItemMovementResult(iMovementResult);
	pifMsg707.SendRequ();
}

// P708 PINDOWN指示結果報告
extern "C"
void PIFCOMMAPI PifComm_PinDownResultReport(int iMovementResult)
{
	CPifMessage708 pifMsg708;
	pifMsg708.SetItemMovementResult(iMovementResult);
	pifMsg708.SendRequ();
}

// P709 シャッターOPEN指示結果報告
extern "C"
void PIFCOMMAPI PifComm_ShutterOpenResultReport(int iMovementResult)
{
	CPifMessage709 pifMsg709;
	pifMsg709.SetItemMovementResult(iMovementResult);
	pifMsg709.SendRequ();
}

// P710 シャッターCLOSE指示結果報告
extern "C"
void PIFCOMMAPI PifComm_ShutterCloseResultReport(int iMovementResult)
{
	CPifMessage710 pifMsg710;
	pifMsg710.SetItemMovementResult(iMovementResult);
	pifMsg710.SendRequ();
}

// P711 測定キャンセル結果報告
extern "C"
void PIFCOMMAPI PifComm_MeasureCancelResultReport(int iMovementResult)
{
	CPifMessage711 pifMsg711;
	pifMsg711.SetItemMovementResult(iMovementResult);
	pifMsg711.SendRequ();
}

// P801 アラーム発生・解除報告
extern "C"
void PIFCOMMAPI PifComm_AlarmReport(const ALARMDATASET* pstAlarmData)
{
	CPifMessage801 pifMsg801;
	pifMsg801.SetItemAlarmEventType(pstAlarmData->chEventType);
	pifMsg801.SetItemAlarmCode(pstAlarmData->iAlarmCode);
	pifMsg801.SetItemAlarmId(pstAlarmData->iAlarmId);
	pifMsg801.SetItemAlarmLevel(pstAlarmData->chAlarmLevel);
	pifMsg801.SetItemAlarmMessage(pstAlarmData->szAlarmMessage);
	pifMsg801.SendRequ();
}
