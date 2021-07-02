#pragma once

#if !defined(_MEASYSTDLL_)
#define MEASYSAPI __declspec(dllimport)
#else
#define MEASYSAPI __declspec(dllexport)
#endif

#include "Globals.hxx"
#include "DdeClient.hxx"

#define WM_MEAS 								(WM_APP + 0x0300)
#define WM_MEAS_RECVDATA						(WM_MEAS + 0x00)
#define WM_MEAS_LINE_END						(WM_MEAS + 0x01)
/* added 2009.10.26 hmenjo CTA アラーム用 WM_ ---------- { ---------- */
#define WM_MEAS_CTA_ALARM						(WM_MEAS + 0x02)	/* CTA アラーム	*/
/* added 2009.10.26 hmenjo CTA アラーム用 WM_ ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
#define WM_MEAS_COMPEASE_STATUS					(WM_MEAS + 0x03)
#define WM_MEAS_COMPEASE_ERROR					(WM_MEAS + 0x04)
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// ADAPからのRESULTデータの最大文字数
#define ADAPRESULTSTRINGLENMAX					(20)

enum EASE_STATUS {
	EASE_STATUS_CHECK_PASS = -1,		// 別コマンド実行中のため、ステータス取得できない

	EASE_STATUS_UNKNOWN = 0,			//
	EASE_STATUS_COMM_ERROR,				//
	EASE_STATUS_COMM_ESTABLISH,			//
	EASE_STATUS_EXECUTE_CMD_ERROR,		//
	EASE_STATUS_EXECUTE_CMD_TIMEOUT,	//

	EASE_STATUS_WAIT_ACQUIRE = 100,		// from Polling status
	EASE_STATUS_HARDWARE_BUSY,			//
	EASE_STATUS_ACQUIRING,				//
	EASE_STATUS_INIT_HARDWARE,			//
	EASE_STATUS_ALIGN_HARDWARE,			//
	EASE_STATUS_CALC_DC_OFFSET,			//
	EASE_STATUS_CALIB_HARDWARE,			//
	EASE_STATUS_PERFORM_SYSTEM_CHECK,	//
	EASE_STATUS_DISPLAY_RAW_SIGNAL,		//
	EASE_STATUS_HARDWARE_NOT_INIT,		//
	EASE_STATUS_HARDWARE_NOT_CALIB,		//
	EASE_STATUS_MAX,
};

enum EASE_ERROR {
	EASE_ERROR_RUN_RECIPE_NOT_FOUND = 0,
	EASE_ERROR_MAX,
};

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================


// --------------------------------------------------------------------
// Common
//	 Initialization/Shutdown
BOOL MEASYSAPI MEAS_Initialize(BOOL bHWS=FALSE);
void MEASYSAPI MEAS_SetNotifyWnd(HWND hWnd);
BOOL MEASYSAPI MEAS_Shutdown(void);


// --------------------------------------------------------------------
// General(Sr,Se etc)
//
BOOL MEASYSAPI MEAS_Dark_T1(const MEAS_PROG_INFO* pMeasProgInfo);
BOOL MEASYSAPI MEAS_Dark_T2(const MEAS_PROG_INFO* pMeasProgInfo);
BOOL MEASYSAPI MEAS_1stReference(const MEAS_PROG_INFO* pMeasProgInfo);
BOOL MEASYSAPI MEAS_2ndReference_T1(const MEAS_PROG_INFO* pMeasProgInfo);
BOOL MEASYSAPI MEAS_2ndReference_T2(const MEAS_PROG_INFO* pMeasProgInfo);
void MEASYSAPI MEAS_InitFirstMeasure();
// 2009.08.18 K.Matsuo ストレス対応 -->
//BOOL MEASYSAPI MEAS_Measure(const MEAS_PROG_INFO* pMeasProgInfo, int iScanCnt);
BOOL MEASYSAPI MEAS_PrepareForMeasure(LPCTSTR pszLotId, LPCTSTR pszSampleId, LPCTSTR pszMainRecipeName, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr);
BOOL MEASYSAPI MEAS_Measure(int iScanCnt, const STAGE_COORD_XYZ* pStageCoordXyz);
// 2009.08.18 K.Matsuo ストレス対応 <--
BOOL MEASYSAPI MEAS_Analysis(int iScanCnt, CDdeClient* pXmpDde);
/* added 2009.10.23 hmenjo CTA API 定義追加 ---------- { ---------- */
/* CTA 測定モード設定	*/
#define	MEAS_CTA_MODE_1	1	/* モード１：基板単位・オペレータデータ確認なし	*/
#define	MEAS_CTA_MODE_2	2	/* モード２：基板単位・オペレータデータ確認あり	*/
#define	MEAS_CTA_MODE_3	3	/* モード３：カセット単位・オペレータデータ確認あり	*/
BOOL MEASYSAPI MEAS_CtaModeSet(int iCtaMode);
/* CTA 情報設定	*/
/*	DWORD dwThick		：基板厚み情報[0.1mm]，範囲(0.0～10.0mm)	*/
BOOL MEASYSAPI MEAS_CtaSetThick(DWORD dwThick);
/* CTA 測定終了	*/
BOOL MEASYSAPI MEAS_CtaMeasEnd(void);
/* CTA 強制終了	*/
BOOL MEASYSAPI MEAS_CtaForceEnd(void);
/* CTA 初期化	*/
BOOL MEASYSAPI MEAS_CtaInitialize(void);
/* CTA カセット ID，基板 ID 設定	*/
/*	LPCSTR pstrCstID				：カセット ID	：ASCII 80 文字
 *	LPCSTR pstrSampleID				：基板 ID		：ASCII 80 文字
 *	MAIN_RCP_INFO* pMainRcpInfo		：メインレシピのアドレス	*/
BOOL MEASYSAPI MEAS_CtaSetCstSample(LPCSTR pstrCstID, LPCSTR pstrSampleID, const MAIN_RCP_INFO* pMainRcpInfo);
/* CTA 状態取得	*/
/*	LOBYTE(LOWORD(*plStatus));	ヘッド：Z 軸位置
	HIBYTE(LOWORD(*plStatus));	状態１：測定状態
	LOBYTE(HIWORD(*plStatus));	状態２：装置状態
	HIBYTE(HIWORD(*plStatus));	(未使用)	*/
BOOL MEASYSAPI MEAS_CtaGetStatus(long* plStatus);
/* added 2009.10.23 hmenjo CTA API 定義追加 ---------- } ---------- */

// 2013.02.01 bagus CompleteEASEヘッド追加 MeaSys内で使用する関数 -->
BOOL IsNoRecalibration(LPCTSTR szRecalib);
BOOL MEAS_Recalib(LPCTSTR szRecalib, double& dData);
BOOL MEAS_RecalibMinusEntry(LPCTSTR szRecalib, double& dData);
// 2013.02.01 bagus CompleteEASEヘッド追加 MeaSys内で使用する関数 <--

// --------------------------------------------------------------------
// Sr Reference File
//
int MEASYSAPI MEAS_CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime);
BOOL MEASYSAPI MEAS_ReadRefFile(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_ReadGTrRefFile(LPCTSTR pszMainRecipeName, int iCoordRefPos);
BOOL MEASYSAPI MEAS_ReadRefFile2ndRefT1(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_ReadRefFile2ndRefT2(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_WriteRefFile(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_WriteGTrRefFile(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_WriteRefFile2ndRefT1(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_WriteRefFile2ndRefT2(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_Is2ndRefT1FileExist(LPCTSTR pszMainRecipeName);
BOOL MEASYSAPI MEAS_Is2ndRefT2FileExist(LPCTSTR pszMainRecipeName);

// --------------------------------------------------------------------
// Sr Transmittance(Gantry)
//
//BOOL MEASYSAPI MEAS_PrepareForGTrMeasure(int iMode, LPCTSTR pszLotId, LPCTSTR pszSampleId, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr);
BOOL MEASYSAPI MEAS_GTrReferenceMeasure(int iScanCnt, const STAGE_COORD_XYZ* pStageCoordXyz);
//BOOL MEASYSAPI MEAS_GTrAnalysis(int iScanCnt, CDdeClient* pXmpDde);

// --------------------------------------------------------------------
// Stress
//
//	測定モジュールが、ストレス測定の準備をするための関数です。
//	ストレスシーケンスの初期段階（SR測定開始前）に関数を呼び出してください。
//	測定モジュール内部で、初期化処理や、SR測定（ストレス計算用か否か）の判断に使用します。
//Saiki 20110307 Change ----->
//BOOL MEASYSAPI MEAS_PrepareForStressMeasure(int iMode, LPCTSTR pszSampleId, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr, const STAGE_PROG_STRESS* pStageProgStress);	// iMode = 0:Reference, 1:Normal
BOOL MEASYSAPI MEAS_PrepareForStressMeasure(int iMode, LPCTSTR pszSampleId, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr, const STAGE_PROG_STRESS* pStageProgStress, const STRESS_PARAM_INFO* pStressParamInfo);	// iMode = 0:Reference, 1:Normal
//Saiki 20110307 Change <-----

//	ストレス開始時（ライン測定毎）に呼び出してください。
//	呼び出し元と非同期になります。（内部でスレッド化します。）
//	測定・解析終了時には、MEAS_SetNotifyWnd(HWND )で登録されたハンドルへ、WMメッセージ（WM_MEAS_LINE_END）を通知します。
BOOL MEASYSAPI MEAS_StressMeasure(int iLineNo);	// 即リターン

//	測定をアボート処理したい時に呼び出ししてください。
//	アボード終了時には、WMメッセージ（WM_MEAS_LINE_END）で通知します。
//	（通常の測定終了と同じWMメッセージになります。）
void MEASYSAPI MEAS_StressCancel();


// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// 単体機能呼出し
BOOL MEASYSAPI MEAS_SrHead_CcdScan(double dIntegTime);
int MEASYSAPI MEAS_SrHead_GetCcdData(int iPixel);
BOOL MEASYSAPI MEAS_SrHead_ChangeCcdShutter(WORD wFilter);
BOOL MEASYSAPI MEAS_SrHead_OpenUvShutter(void);
BOOL MEASYSAPI MEAS_SrHead_CloseUvShutter(void);
BOOL MEASYSAPI MEAS_SrHead_OpenTransShutter(void);
BOOL MEASYSAPI MEAS_SrHead_CloseTransShutter(void);

int MEASYSAPI MEAS_GetCcdData(int iPixel);				   		// Ccdデータの取得（ピクセル（1～512 or 1024）
#if 0 // 2014.04.04 bagus wavelength step modified -->
// double MEASYSAPI MEAS_GetScanDark_T1Data(int iWave);				// Light Intensityの取得（波長380～800 or 380～850）
// double MEASYSAPI MEAS_GetScanDark_T2Data(int iWave);				// Light Intensityの取得（波長380～800 or 380～850）
// double MEASYSAPI MEAS_GetScan1stReferenceData(int iWave);			// Light Intensityの取得（波長380～800 or 380～850）
// double MEASYSAPI MEAS_GetScan2ndReference_T1Data(int iWave);		// Light Intensityの取得（波長380～800 or 380～850）
// double MEASYSAPI MEAS_GetScan2ndReference_T2Data(int iWave);		// Light Intensityの取得（波長380～800 or 380～850）
// double MEASYSAPI MEAS_GetScanSampleData(int iWave); 	   		// Light Intensityの取得（波長380～800 or 380～850）
// //double MEASYSAPI MEAS_GetAnalysisData(int iWave);		   		// 反射率データの取得（波長380～800 or 380～850）
// //double MEASYSAPI MEAS_GetAnalysisGenData(int iWave);	   		// XmpからのGenDataの取得（波長380～800 or 380～850）
// //2010.01.11 bagus GTR --{--
// double MEASYSAPI MEAS_GetScanGTrReferenceData(int iRefPos,int iWave);		// GTRリファレンス測定時のリファレンスデータを取得
//2010.01.11 bagus GTR --}--
#else
double MEASYSAPI MEAS_GetScanDark_T1Data(double dWave);						// Light Intensityの取得（波長380～800 or 380～850）
double MEASYSAPI MEAS_GetScanDark_T2Data(double dWave);						// Light Intensityの取得（波長380～800 or 380～850）
double MEASYSAPI MEAS_GetScan1stReferenceData(double dWave);				// Light Intensityの取得（波長380～800 or 380～850）
double MEASYSAPI MEAS_GetScan2ndReference_T1Data(double dWave);				// Light Intensityの取得（波長380～800 or 380～850）
double MEASYSAPI MEAS_GetScan2ndReference_T2Data(double dWave);				// Light Intensityの取得（波長380～800 or 380～850）
double MEASYSAPI MEAS_GetScanSampleData(double dWave);						// Light Intensityの取得（波長380～800 or 380～850）
double MEASYSAPI MEAS_GetScanGTrReferenceData(int iRefPos,double dWave);	// GTRリファレンス測定時のリファレンスデータを取得
#endif // 2014.04.04 bagus wavelength step modified -->

//BOOL MEASYSAPI MEAS_Recalib(LPCTSTR szRecalib, double& dData);
// 2009.09.15 K.Matsuo レキャリブレーションのマイナス値入力対応 -->
//BOOL MEASYSAPI MEAS_RecalibMinusEntry(LPCTSTR szRecalib, double& dData);
// 2009.09.15 K.Matsuo レキャリブレーションのマイナス値入力対応 <--
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 -->
void MEASYSAPI MEAS_GetStratetyType(BOOL& bSr, BOOL& bSe, BOOL& bStress, LPCTSTR pszStrategyEntry);
// 2009.09.18 K.Matsuo ストラテジーの測定タイプ判定 <--
BOOL MEASYSAPI MEAS_GetAdapScriptFileDescriptDdeTextItem(LPTSTR pszDdeTextItem, int& iStartWaveLen, int& iEndWaveLen, int& iColorFilter, LPCTSTR pszStrategyEntry);
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
BOOL MEASYSAPI MEAS_ShowXmp(int iShow);
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */
/* added 2016.02.24 hmenjo バッチ処理 ---------- { ---------- */
/* バッチ要求フラグ参照	*/
long MEASYSAPI MEAS_BatchFlagSet(long lBatchFlag);
long MEASYSAPI MEAS_BatchFlagGet(void);
/* added 2016.02.24 hmenjo バッチ処理 ---------- } ---------- */


// SeHead Functions
BOOL MEASYSAPI MEAS_SeHead_OpenLampShutter(void);
BOOL MEASYSAPI MEAS_SeHead_CloseLampShutter(void);

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// CompEASEHead Functions
int MEAS_CompEASEHead_IsStatus(void);
BOOL MEASYSAPI MEAS_CompEASEHead_ListRecipes(CStringArray** ppListRecipes);
BOOL MEASYSAPI MEAS_CompEASEHead_OpenLampShutter(void);
BOOL MEASYSAPI MEAS_CompEASEHead_CloseLampShutter(void);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

////////////////////////////////////////////////////////////////////////////////
// Structures
typedef struct _ADAPRESULTDATABASE {
	char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	double dData[ADAPRESULT_COLS_MAX];
	_ADAPRESULTDATABASE::_ADAPRESULTDATABASE()
	{::ZeroMemory(this, sizeof(*this));}
} ADAPRESULTDATABASE, *PADAPRESULTDATABASE;

#define ADAPRESULTFILEMAPPINGNAME	"AdapResultDataBase"		/* ファイルマッピングネーム	*/

// 2009.08.05 K.Matsuo Stress Add -->
typedef struct _DEFLECTION {
	double dPosum;
	double dDefl;
} DEFLECTION;

// θ（傾き）= dy/dx
typedef struct _DYDX {
	double dPosum;
	double dTheta;
} DYDX;

//// dθ/dx（傾きθの傾き）= 1/R（曲率） 、R（曲率半径）
//typedef struct _DTHETADX {
//	double dPosum;
//	double dDthetaDx;
//} DTHETADX;

// 応力（ストレス）
typedef struct _STRESSLINE {
	double dPosum;
	double dStress;
} STRESSLINE;


/*
 * ストレスの測定結果構造体（ファイルマッピングで使用します）
 *
 * ストレスの測定結果を１ラインの測定ごと格納します
 *
 * リファレンス測定時の格納データ
 *	 生データ・・・
 *	   DEFLECTION DeflectionRaw[0]です
 *	   ライン単位に結果を格納していますので、
 *	   セクションごとに結果取得する場合は、FromToで抽出してください。
 *	   レシピでSCANVALID==TRUEのセクションであることを確認してアクセスしてください。
 *
 *	   From<=dPosum<=To
 *		 From : lScanStartPosX		セクション開始 X 位置[um]
 *		 To   : lScanEndPosX		セクション終了 X 位置[um]
 *
 * 応力測定時の格納データ
 *	 生データ・・・
 *	   DEFLECTION DeflectionRaw[1]、DYDX DyDxRaw、STRESSLINE dStressRawの３つあります
 *	   ライン単位に結果を格納していますので、
 *	   セクションごとに結果取得する場合は、FromToで抽出してください。
 *	   レシピでSCANVALID==TRUEのセクションであることを確認してアクセスしてください。
 *
 *	   From<=dPosum<=To
 *		 From : lScanStartPosX		セクション開始 X 位置[um]
 *		 To   : lScanEndPosX		セクション終了 X 位置[um]
 *
 *	 ストレスデータ・・・
 *	   STRESSRESULT
 *		 セクションごとに保持しています
 *		 レシピでSCANVALID==TRUEのセクションであることを確認してアクセスしてください。
 */
typedef struct _STRESSRESULT {
	// 変位量の生データ
	int nDeflectionOccurence[2];	// 格納したデータ個数（＝配列の上限+1）	// 0:リファレンス、1:応力測定
	DEFLECTION DeflectionRaw[2][DEFL_CNT_MAX];			// 0:リファレンス、1:応力測定

	// 傾きの生データ
	int nDyDxOccurence;				// 格納したデータ個数（＝配列の上限+1）
	DYDX DyDxRaw[DYDX_CNT_MAX];

	// ストレスの生データ
	int nStressLineOccurrence;		// 格納したデータ個数（＝配列の上限+1）
	STRESSLINE StressRaw[STRESS_CNT_MAX];								// シンボル名誤り修正：dStressRaw -> StressRaw

	// 応力（ストレス）（セクションごと）
	int nSectionOccurrence;			// 格納したデータ個数（＝配列の上限+1）
	double dStressSection[STRESS_SECTIONS_MAX];
} STRESSRESULT;

// ファイルマッピングネーム
#define STRESSFILEMAPPINGNAME	"STERSSRESULTDATABASE"

// 2009.08.05 K.Matsuo Stress Add <--

BOOL MEASYSAPI ReadReferenceData(LPCTSTR pszSampleId);

/* added 2009.10.27 hmenjo CTA 測定結果 DB ---------- { ---------- */
/*
 *	CTA の測定結果構造体 (ファイルマッピングで使用します)
 *		１ポイントの測定データです．
 *
 */
typedef struct {
	int		iPointNo;	/* 1～1000		ポイント番号(未使用：CTA ユニットから取得したデータです(参考用))	*/
	double	dCAngle;	/* 0.1～999.9	接触角[dgree]	*/
	double	dRadius;	/* 0.1～9999.9	半径[um]	*/
	double	dLiquid;	/* 0.01～99.99	液量[uL]	*/
} CTARESULT, *LPCTARESULT;
typedef const CTARESULT *LPCCTARESULT;
#define CTAFILEMAPPINGNAME	"CTARESULTDATABASE"		/* ファイルマッピングネーム	*/
/* added 2009.10.27 hmenjo CTA 測定結果 DB ---------- } ---------- */

// 2009.11.05 K.Matsuo Resist Add -->
/*
 *	Resist の測定結果構造体 (ファイルマッピングで使用します)
 *		１ポイントの測定データです．
 *		文字列の項目をグリッドに表示してください。（”0.991E+00”などの文字列が入っています。）
 *		統計情報でも利用しやすいように、double型でも用意しました。（中身の数値は、文字列型のものとイコールです。）
 */
typedef struct _RESISTRESULT {
	TCHAR chDataValid;					/* N:測定データ有効、O:オーバーレンジ、L:オーバーロード (未使用：RS ユニットから取得したデータです(参考用)) */
	TCHAR szResistance[9 + 1];			/* 抵抗値（Ω） */
	TCHAR szSurfaceResistivity[9 + 1];	/* 表面抵抗率（Ω／□（またはΩ/sq.）） */
	TCHAR szVolumeResistivity[9 + 1];	/* 体積抵抗率（Ω・cm）*/
	TCHAR szConductivity[9 + 1];		/* 導電率（S/cm） */
	double dResistance;					/* 上記同項目のdouble型 */
	double dSurfaceResistivity;			/* 上記同項目のdouble型 */
	double dVolumeResistivity;			/* 上記同項目のdouble型 */
	double dConductivity;				/* 上記同項目のdouble型 */
} RESISTRESULT;

#define RESISTFILEMAPPINGNAME	"RESISTRESULTDATABASE"		/* ファイルマッピングネーム	*/
// 2009.11.05 K.Matsuo Resist Add <--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// 測定1ポイント目にグリッドにラベル名（結果項目名）を表示する際に利用する
// bItemEnableがTRUEの場合、ラベル名をADAPRESULTDATABASEから取得してください
typedef struct _COMPEASERESULT_HELPER_DATABASE {
	BOOL bItemEnable[100];
	int iDispIndex[100];
	int iItemEnableCount;
	_COMPEASERESULT_HELPER_DATABASE::_COMPEASERESULT_HELPER_DATABASE()
	{::ZeroMemory(this, sizeof(*this));}
} COMPEASERESULT_HELPER, *PCOMPEASERESULT_HELPER;

#define COMPEASERESULT_HELPER_FILEMAPPINGNAME	"CompEASEResultHelperDataBase"		/* ファイルマッピングネーム	*/
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
