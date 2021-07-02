#pragma once

#include "../../INC/Globals.hxx"
#include "../../INC/MeaSys.hxx"

#define WM_PIFCOMM							(WM_APP + 0x0400)
#define WM_PIFCOMM_CONNECT					(WM_PIFCOMM + 0x00)

// -----------------------------------------------------------------------------
// Mailslot名
// -----------------------------------------------------------------------------
#define MAILSLOT_CHIEF		_T("\\\\.\\mailslot\\Chief")
#define MAILSLOT_MAINFRM	_T("\\\\.\\mailslot\\Mainfrm")

// -----------------------------------------------------------------------------
// Mailslot 通知用Message
// -----------------------------------------------------------------------------
// NanoSpec.exe (Chief)
#define PIFCOMM_P201		 _T("P201")
#define PIFCOMM_P401		 _T("P401")
#define PIFCOMM_P501		 _T("P501")
#define PIFCOMM_P601		 _T("P601")
#define PIFCOMM_P602		 _T("P602")
#define PIFCOMM_P603		 _T("P603")
#define PIFCOMM_P604		 _T("P604")
#define PIFCOMM_P605		 _T("P605")
#define PIFCOMM_P606		 _T("P606")
#define PIFCOMM_P607		 _T("P607")
#define PIFCOMM_P608		 _T("P608")
#define PIFCOMM_P609		 _T("P609")
#define PIFCOMM_P610		 _T("P610")
#define PIFCOMM_P611		 _T("P611")
#define PIFCOMM_P306		 _T("P306")
// 2009.09.10 K.Matsuo -->
#define PIFCOMM_P411		 _T("P411")
#define PIFCOMM_P511		 _T("P511")
// 2009.09.10 K.Matsuo <--
// 2014.01.07 bagus Add(Stage None対応) -->
#define PIFCOMM_P514		 _T("P514")
#define PIFCOMM_P516		 _T("P516")
// 2014.01.07 bagus Add(Stage None対応) <--

// NanoSpec.exe (MainFrm)
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
#define PIFCOMM_P105		 _T("P105")
#define PIFCOMM_P106		 _T("P106")
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
#define PIFCOMM_P303		 _T("P303")
#define PIFCOMM_P306		 _T("P306")
#define PIFCOMM_P308		 _T("P308")
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
#define PIFCOMM_P310		 _T("P310")
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--
// 2009.12.21 K.Matsuo -->
#define PIFCOMM_P302		 _T("P302")
// 2009.12.21 K.Matsuo <--

// -----------------------------------------------------------------------------
// 構造体定義
// -----------------------------------------------------------------------------
// 装置モニター情報セット構造体
typedef struct tagEQMONITORSET
{
	int iLocalRemote;
	int iEqProcessStatus;
	int iAlarmStatus;
	int iGlassExist;
	int iLoadPosition;
	int iVaccumOn1;
	int iShutterOpen;
	int iShutterClose;
	int iPinUp;
	int iPinDown;
	int iArmSensor;
	int iDoorInterlock;
	int iVaccumOn2;
	int iFiller[7];
} EQMONITORSET;

// トレースデータ情報セット構造体
typedef struct tagEXCHANGETIMEINFO
{
	SYSTEMTIME lastExchange;					// 最終交換日付
	long lElapseSecs;					   		// 使用経過（秒）
	long lWarningSecs;						  	// 警告期間（秒）
} EXCHANGETIMEINFO;

typedef struct tagTRACEDATASET
{
	EXCHANGETIMEINFO SrVisReflectanceLamp;		// SrVis反射光ランプ
	EXCHANGETIMEINFO SrVisTransmittanceLamp;	// SrVis透過光ランプ
	EXCHANGETIMEINFO SrUvLamp;					// SrUvランプ
	EXCHANGETIMEINFO SeVisReflectanceLamp;		// SeVis反射光ランプ
	EXCHANGETIMEINFO SeUvLamp;					// SeUvランプ
} TRACEDATASET;

// レシピファイル名・最終更新日時セット構造体
typedef struct tagRECIPENAMEDATESET
{
	TCHAR szRecipeNameExt[28 + 1];
	SYSTEMTIME recipeDateTime;
} RECIPENAMEDATESET;

// 統計データセット構造体
typedef struct tagSTATISTICSDATESET
{
	double dMin;
	double dMax;
	double dAve;
	double dSd;
	double dUnif;
} STATISTICSDATESET;

// アラームデータセット構造体
typedef struct tagALARMDATASET
{
	TCHAR chEventType;
	int iAlarmCode;
	int iAlarmId;
	TCHAR chAlarmLevel;
	TCHAR szAlarmMessage[80 + 1];
} ALARMDATASET;

// -----------------------------------------------------------------------------
// API
// -----------------------------------------------------------------------------
// dllimport / dllexport
#if !defined(_PIFCOMMDLL_)
#define PIFCOMMAPI __declspec(dllimport)
#else
#define PIFCOMMAPI __declspec(dllexport)
#endif

// Dll初期化
extern "C" PIFCOMMAPI void PifComm_Init();

// ネットワーク接続状況通知用ウィンドウハンドルの設定
extern "C" PIFCOMMAPI void PifComm_SetNotifyWnd(HWND hWnd);

// Sxxx 応答コード設定
extern "C" PIFCOMMAPI void PifComm_DoStateDone(int iAckCode);

// P201 時刻合わせ要求
extern "C" PIFCOMMAPI void PifComm_GetSetTime(SYSTEMTIME* pSystemTime);

// P301 装置モニター情報
extern "C" PIFCOMMAPI EQMONITORSET* PifComm_GetEqMonitorPtr();
extern "C" PIFCOMMAPI void PifComm_EqMonitortReport();

// P303 レシピ一覧取得要求
extern "C" PIFCOMMAPI int PifComm_RequestKindOfRecipe();

// P304 レシピ一覧報告
extern "C" PIFCOMMAPI void PifComm_RecipeListReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate, UINT nOccurence);

// P305 レシピ変更報告
extern "C" PIFCOMMAPI void PifComm_ChangeRecipeDeleteReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate);
extern "C" PIFCOMMAPI void PifComm_ChangeRecipeInsertReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate);
extern "C" PIFCOMMAPI void PifComm_ChangeRecipeReplaceReport(int iKindOfRecipe, const RECIPENAMEDATESET* pstRecipeNameDate);

// P307 トレースデータ報告
extern "C" PIFCOMMAPI void PifComm_TraceDataReport(const TRACEDATASET* pstTraceData);

// P308 サンプルサイズデータ取得要求
extern "C" PIFCOMMAPI void PifComm_GetSampleSizeRecipeName(LPTSTR pszRecipeName);

// P309 サンプルサイズデータ報告
extern "C" PIFCOMMAPI void PifComm_SampleSizeDataReport(double dX, double dY);

// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
// P310 リファレンスデータ確認要求
extern "C" PIFCOMMAPI void PifComm_GetReferenceDataCheckRecipeName(LPTSTR pszRecipeName);
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--

// P401 着工レシピ指示要求
// 2009.10.29 K.Matsuo -->
// P411 リファレンス取得レシピ指示要求
// 2009.10.29 K.Matsuo <--
extern "C" PIFCOMMAPI void PifComm_GetRecipeName(LPTSTR pszRecipeName);


// P501 測定開始指示要求
// 2009.10.29 K.Matsuo -->
// P511 リファレンス取得開始指示要求
//	 サンプルIDが未設定の場合もあります。（ex.ガントリー透過率）
//	 ロットIDは追加項目のため、未設定の場合もあります。
// 2009.10.29 K.Matsuo <--
extern "C" PIFCOMMAPI void PifComm_GetSampleId(LPTSTR pszSampleId);
// 2009.10.29 K.Matsuo Add -->
extern "C" PIFCOMMAPI void PifComm_GetLotId(LPTSTR pszLotId);
// 2009.10.29 K.Matsuo Add <--

// P502 測定ポイント完了報告
//	 iFormatId = 001 SR 膜厚, 002 SR 反射率, 003 SR 透過率, 004 SE
extern "C" PIFCOMMAPI void PifComm_CompleteMeasurePointReport(int iFormatId, int iPointNo, LPCTSTR pszAF_ST, const STAGE_COORD_XYZ* pCoordXyz, const char pszLabelOri[][ADAPRESULTSTRINGLENMAX + 1], const double* dData, const int* iSpecificWavelen);
//	 iFormatId = 005 RS
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureResistPointReport(int iPointNo, LPCTSTR /*pszAF_ST*/, const STAGE_COORD_XYZ* pCoordXyz, TCHAR chDataValid, LPCTSTR pszResistance, LPCTSTR pszSurfaceResistivity, LPCTSTR pszVolumeResistivity, LPCTSTR pszConductivity);
//	 iFormatId = 006 CA
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureContactAnglePointReport(int iPointNo, LPCTSTR /*pszAF_ST*/, const STAGE_COORD_XYZ* pCoordXyz, double dCAngle, double dRadius, double dLiquid);
//	 iFormatId = 101
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureStressLineReport(int iMeasMode, int iLineNo, const STAGE_PROG_STRESS* pStageProgStress, const double* dStress);

// P503 測定完了報告
//	 iFormatId = 001 SR 膜厚, 002 SR 反射率, 003 SR 透過率, 004 SE
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureAllPointsReport(int iFormatId, int iTotalPointCnt, const char pszLabelOri[][ADAPRESULTSTRINGLENMAX + 1], const STATISTICSDATESET* pStatistics, const int* iSpecificWavelen);
//	 iFormatId = 005 RS
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureResistAllPointsReport(int iTotalPointCnt, const STATISTICSDATESET* pStatistics/* occurs = 4*/);
//	 iFormatId = 006 CA
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureContactAngleAllPointsReport(int iTotalPointCnt, const STATISTICSDATESET* pStatistics/* occurs = 3*/);
//	 iFormatId = 101
extern "C" PIFCOMMAPI void PifComm_CompleteMeasureStressAllLinesReport(int iMeasMode, const STAGE_PROG_STRESS* pStageProgStress, const STATISTICSDATESET* pStatistics);

// P504 測定キャンセルボタン押下報告
extern "C" PIFCOMMAPI void PifComm_PressMeasureCancelButtonReport();

// 2009.10.29 K.Matsuo -->
// P512 リファレンス取得ポイント完了報告
extern "C" PIFCOMMAPI void PifComm_CompleteReferencePointReport(int iPointNo);

// P513 リファレンス取得完了報告
extern "C" PIFCOMMAPI void PifComm_CompleteReferenceAllPointsReport(int iTotalPointCnt);
// 2009.10.29 K.Matsuo <--

// 2014.01.07 bagus Add(Stage None対応) -->
// P515 Siリファレンス取得完了報告
extern "C" PIFCOMMAPI void PifComm_CompleteSiReferenceDoneReport(int iReferenceResult);

// P517 Darkリファレンス取得完了報告
extern "C" PIFCOMMAPI void PifComm_CompleteDarkReferenceDoneReport(int iReferenceResult);
// 2014.01.07 bagus Add(Stage None対応) <--

// P601ロードポジション移行指示要求
extern "C" PIFCOMMAPI void PifComm_GetSampleSize(double* dX, double* dY);

// P602 汎用ポジション移行指示要求
extern "C" PIFCOMMAPI int PifComm_GetMovePosId();

// P701 ロードポジション移行結果報告
extern "C" PIFCOMMAPI void PifComm_LoadPositionMovementResultReport(int iMovementResult);

// P702 汎用ポジション移行結果報告
extern "C" PIFCOMMAPI void PifComm_GeneralPurposePositionMovementResultReport(int iMovementResult);

// P703 バキュームON指示結果報告
extern "C" PIFCOMMAPI void PifComm_VaccumOnResultReport(int iMovementResult);

// P704 バキュームOFF指示結果報告
extern "C" PIFCOMMAPI void PifComm_VaccumOffResultReport(int iMovementResult);

// P705 アライメントLOAD指示結果報告
extern "C" PIFCOMMAPI void PifComm_AlignmentLoadResultReport(int iMovementResult);

// P706 アライメントUNLOAD指示結果報告
extern "C" PIFCOMMAPI void PifComm_AlignmentUnloadResultReport(int iMovementResult);

// P707 PINUP指示結果報告
extern "C" PIFCOMMAPI void PifComm_PinUpResultReport(int iMovementResult);

// P708 PINDOWN指示結果報告
extern "C" PIFCOMMAPI void PifComm_PinDownResultReport(int iMovementResult);

// P709 シャッターOPEN指示結果報告
extern "C" PIFCOMMAPI void PifComm_ShutterOpenResultReport(int iMovementResult);

// P710 シャッターCLOSE指示結果報告
extern "C" PIFCOMMAPI void PifComm_ShutterCloseResultReport(int iMovementResult);

// P711 測定キャンセル結果報告
extern "C" PIFCOMMAPI void PifComm_MeasureCancelResultReport(int iMovementResult);

// P801 アラーム発生・解除報告
extern "C" PIFCOMMAPI void PifComm_AlarmReport(const ALARMDATASET* pstAlarmData);
