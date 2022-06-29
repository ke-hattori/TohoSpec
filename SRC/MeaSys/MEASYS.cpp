// MEASYS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include <time.h>
#include <math.h>
/* added 2015.12.10 hmenjo リカリブ逆相関対応 ---------- { ---------- */
#include <FLOAT.H>
/* added 2015.12.10 hmenjo リカリブ逆相関対応 ---------- } ---------- */

#ifdef _DEBUG
#pragma comment(lib, "dtnsconfigfile.lib")
#pragma comment(lib, "dtnsrecipefile.lib")
#pragma comment(lib, "dtnsnexio.lib")
#else
#pragma comment(lib, "tnsconfigfile.lib")
#pragma comment(lib, "tnsrecipefile.lib")
#pragma comment(lib, "tnsnexio.lib")
#endif

#include "../../INC/RecipeFile.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/SharedMemory.h"
#include "../../INC/Title.hxx"
#include "LogFile.h"
#include "SrMeasure.h"
#include "SrAnalysis.h"
#include "SrRefFile.h"
#include "ScanRange.h"
#include "SrHead.h"
#include "SeMeasure.h"
#include "SeAnalysis.h"
#include "SeHead.h"
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
#include "CompEASEMeasure.h"
#include "CompEASEHead.h"
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
#include "Mojiretsu.h"
#include "Xmp.h"
#include "StressMeasure.h"
#include "StressAnalysis.h"
#include "StressHead.h"
#include "RsMeasure.h"
#include "resource.h"

#define _MEASYSTDLL_
#include "../../INC/MeaSys.hxx"

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#define _MASTER_
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

#include "XmpHelper.hxx"
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- { ---------- */
#include "..\\..\\inc\\NEXIOBASE.HXX"
#include "CtaCtrl.h"
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = __FILE__;
#endif

#define MEASYS_LOG_NAME 		"MeaSys"
#define MEASYS_LOG_MAX			(30)
#define MEASYS_LOG_SIZE 		(3 * 1024 * 1024)

// 2009.12.10 K.Matsuo -->
#define MICROMETRE				(1000L)
// 2009.12.10 K.Matsuo <--

// CCDの最大PIXEL
#define CCD_PIXEL_1024					(1024)

// 2013.02.01 bagus CompleteEASEヘッド追加 MeaSys.hxxへ移動 -->
// BOOL IsNoRecalibration(LPCTSTR szRecalib);
// BOOL MEAS_Recalib(LPCTSTR szRecalib, double& dData);
// BOOL MEAS_RecalibMinusEntry(LPCTSTR szRecalib, double& dData);
// 2013.02.01 bagus CompleteEASEヘッド追加 MeaSys.hxxへ移動 <--

////////////////////////////////////////////////////////////////////////////////
// Global Variables
HWND g_hNotifyWnd;

////////////////////////////////////////////////////////////////////////////////
// Variables
BOOL bHwSimulation;
static int ccdData[CCD_PIXEL_1024];
static SCANDATA scanData[MEASYS_STAT_TYPE_MAX_NUM];
static ANALYSISDATA analysisData;

// Prepare Data
typedef struct _PREPARE_DATA {
	LPCTSTR pszLotId;
	LPCTSTR pszSampleId;
	LPCTSTR pszMainRecipeName;
	const MAIN_RCP_INFO* pMainRcpInfo;
	const MEAS_PROG_INFO* pMeasProgInfo;
	const STAGE_PROG_INFO_HDR* pStageProgInfoHdr;
	const STAGE_PROG_STRESS* pStageProgStress;
//Saiki 20110307 Add ----->
	const STRESS_PARAM_INFO* pStressParamInfo;
//Saiki 20110307 Add <-----
	int iStressMode;
} PREPARE_DATA;

static PREPARE_DATA g_prepareData;


// クラスポインタ
CLogFile* pLogFile;
CMojiretsu* pMojiretsu;
CXmp* pXmp;

static CSrMeasure* pSrMeasure;
static CSrAnalysis* pSrAnalysis;
static CSeMeasure* pSeMeasure;
static CSeAnalysis* pSeAnalysis;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
static CCompEASEMeasure* pCompEASEMeasure;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
static CRsMeasure* pRsMeasure;
static CStressMeasure* pStressMeasure;
static CStressAnalysis* pStressAnalysis;
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- { ---------- */
static CCtaCtrl* lgs_pclsCCtaCtrl = 0;
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- } ---------- */

STAGE_COORD_XYZ g_MeasPosXyz;

THICKNESSFORSTRESS thicknessForStress[STRESS_LINES_MAX];

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//static CSharedMemory<ADAPRESULTDATABASE> smAdapResultDataBase;
CSharedMemory<ADAPRESULTDATABASE> smAdapResultDataBase;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
CSharedMemory<STRESSRESULT> smStressResultDataBase;
CSharedMemory<RESISTRESULT> smResistResultDataBase; /* RS 測定結果 DB 共有エリア	*/
/* added 2009.10.27 hmenjo CTA 測定結果 DB ---------- { ---------- */
CSharedMemory<CTARESULT> lg_smCtaResultDataBase;	/* CTA 測定結果 DB 共有エリア	*/
/* added 2009.10.27 hmenjo CTA 測定結果 DB ---------- } ---------- */
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
CSharedMemory<COMPEASERESULT_HELPER> smCompEASEResultHelperDataBase;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

static AFX_EXTENSION_MODULE MEASYSDLL = { NULL, NULL };

/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
TCHAR g_tszProcDir[_MAX_PATH] = _T(""); 	/* 呼出しプロセスのディレクトリ('\'付き)*/          /* C:\NanoSpec\BIN\ or C:\NanoSpec\BIN_DEB\ */
TCHAR g_tszBaseDir[_MAX_PATH] = _T(""); 	/* 基準ディレクトリ('\'付き)*/                      /* C:\NanoSpec\ */
TCHAR g_tszData_Ref_Dir[_MAX_PATH] = _T("");
void GetProcBaseDir(
		LPTSTR ptszProcDir,
		LPTSTR ptszBaseDir
	)
{
	if (0 == _tcscmp(ptszProcDir, _T(""))) {
		TCHAR l_tszProcessFName[_MAX_PATH]; /* 呼出しプロセスのフルパス*/
		::GetModuleFileName(0, l_tszProcessFName, sizeof(l_tszProcessFName));
		TCHAR l_tszDrive[_MAX_DRIVE];
		TCHAR l_tszDir[_MAX_DIR];
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
		TCHAR l_tszFilename[_MAX_FNAME];
//		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, 0, 0);
		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, l_tszFilename, 0);

		g_lAppNameType = APP_NAME_NANO; // default application name
		for(int i = 0; i < APP_NAME_MAX; i++){
			if(_tcsncmp(l_tszFilename, g_lpszAppPrefix4[i], 4) == 0){
				g_lAppNameType = i;
				break;
			}
		}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		_stprintf(ptszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
		if (0 != _tcslen(ptszProcDir)) {
			if (_T('\\') != ptszProcDir[_tcslen(ptszProcDir) - 1]) {
				_tcscat(ptszProcDir, _T("\\"));
			}
		}
	}
	if ((0 != _tcscmp(ptszProcDir, _T("")))
	 && (0 == _tcscmp(ptszBaseDir, _T("")))) {
		TCHAR l_tszProcDirUp[_MAX_PATH];
		_stprintf(l_tszProcDirUp, _T("%s.."), ptszProcDir);
		_tfullpath(ptszBaseDir, l_tszProcDirUp, sizeof(g_tszBaseDir));
		if ((0 != _tcslen(ptszBaseDir)) && (_T('\\') != ptszBaseDir[_tcslen(ptszBaseDir) - 1])) {
			_tcscat(ptszBaseDir, _T("\\"));
		}
	}

	sprintf(g_tszData_Ref_Dir, "%s%s", g_tszBaseDir, "DATA\\REF\\");
}
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */

// --------------------------------------------------------------------
// NanoSpecTitle
void NanoSpecTitle(int iParam)
{
	HWND hWnd;

	hWnd = ::FindWindow(NULL, NANOSPEC_TITLE_WINDOW_NAME);
	if(hWnd != NULL){
		::PostMessage(hWnd, WM_SET_MESSAGE2, iParam, 0L);
	}
}

// --------------------------------------------------------------------
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
		if (FALSE == DllMutexCreate(_T("MeaSys"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		TRACE0(_T("MEASYS.DLL Initializing!\n"));

		if (!AfxInitExtensionModule(MEASYSDLL, hInstance))
			return 0;

/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
		GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */

		new CDynLinkLibrary(MEASYSDLL);

		TCHAR l_tszLogDir[_MAX_PATH];
		_stprintf(l_tszLogDir, _T("%s") _T(LOG_DIR), g_tszProcDir);

		pLogFile = new CLogFile(MEASYS_LOG_NAME,
								l_tszLogDir,
								MEASYS_LOG_SIZE,
								MEASYS_LOG_MAX);
		pMojiretsu = new CMojiretsu();
		pLogFile->Logging("Start Measys.dll DLL_PROCESS_ATTACH");

		smAdapResultDataBase.Create(ADAPRESULTFILEMAPPINGNAME);
		smStressResultDataBase.Create(STRESSFILEMAPPINGNAME);
		smResistResultDataBase.Create(RESISTFILEMAPPINGNAME);
/* added 2009.10.27 hmenjo CTA 測定結果 DB ---------- { ---------- */
		lg_smCtaResultDataBase.Create(CTAFILEMAPPINGNAME);
/* added 2009.10.27 hmenjo CTA 測定結果 DB ---------- } ---------- */
		smCompEASEResultHelperDataBase.Create(COMPEASERESULT_HELPER_FILEMAPPINGNAME);

		pXmp = new CXmp();
		pSrMeasure = new CSrMeasure();
		pSrAnalysis = new CSrAnalysis();
		pSeMeasure = new CSeMeasure();
		pSeAnalysis = new CSeAnalysis();
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		pCompEASEMeasure = new CCompEASEMeasure();
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		pRsMeasure = new CRsMeasure();
		pStressMeasure = new CStressMeasure();
		pStressAnalysis = new CStressAnalysis();
		pStressMeasure->SetStressAnalysisPtr(pStressAnalysis);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0(_T("MEASYS.DLL Terminating!\n"));
		AfxTermExtensionModule(MEASYSDLL);
		if ( pSrMeasure ) {
			delete pSrMeasure;
			pSrMeasure = NULL;
		}
		if ( pSrAnalysis ) {
			delete pSrAnalysis;
			pSrAnalysis = NULL;
		}
		if ( pSeMeasure ) {
			delete pSeMeasure;
			pSeMeasure = NULL;
		}
		if ( pSeAnalysis ) {
			delete pSeAnalysis;
			pSeAnalysis = NULL;
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if ( pCompEASEMeasure ) {
			delete pCompEASEMeasure;
			pCompEASEMeasure = NULL;
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		if ( pRsMeasure ) {
			delete pRsMeasure;
			pRsMeasure = NULL;
		}
		if ( pStressMeasure ) {
			delete pStressMeasure;
			pStressMeasure = NULL;
		}
		if ( pStressAnalysis ) {
			delete pStressAnalysis;
			pStressAnalysis = NULL;
		}

		if ( pXmp ) {
			delete pXmp;
			pXmp = NULL;
		}
		if ( pMojiretsu ) {
			delete pMojiretsu;
			pMojiretsu = NULL;
		}
		if ( pLogFile ) {
			pLogFile->Logging("End Measys.dll DLL_PROCESS_DETACH");
			delete pLogFile;
			pLogFile = NULL;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
	}
	return 1;
}

// --------------------------------------------------------------------
// MEAS_Initialize
BOOL MEASYSAPI MEAS_Initialize(BOOL bHWS/*=FALSE*/)
{
	TRACE(_T("MEAS_Initialize()\n"));

	TCHAR l_tszMsgFail[256];
	TCHAR l_tszMsgTitle[256];

	::ZeroMemory(ccdData, sizeof(ccdData));
	::ZeroMemory(scanData, sizeof(scanData));

	bHwSimulation = bHWS;

	SYSTEM_CONFIG systemConfig;
	ConfigFile_GetNanoSpecIni(&systemConfig, CONFIG_FILE_SYSTEM_CONFIG);

#ifdef _DEBUG
	HANDLE hModule = ResourceLoadLibrary(systemConfig.nLanguage, "dTnsMeaSys"); // Macro : ResourceLoadLibrary // ref. globals.hxx
#else
	HANDLE hModule = ResourceLoadLibrary(systemConfig.nLanguage, "TnsMeaSys");	// Macro : ResourceLoadLibrary // ref. globals.hxx
#endif
//	AfxSetResourceHandle((HINSTANCE) hModule);

	pMojiretsu->SetResourceHandle((HINSTANCE) hModule);

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//	if ( systemConfig.HeadType.bSR || systemConfig.HeadType.bSE ) {
	if ( (systemConfig.HeadType.bSR) ||
		 (systemConfig.HeadType.bSE && systemConfig.HeadType.bCompEASE == FALSE) ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		if ( !pXmp->InitInstance() )
			return FALSE;
	}

	if ( systemConfig.HeadType.bSR ) {
		NanoSpecTitle(TITLE_MSG2_INIT_MEAS_HEAD_SR);
		if ( !pSrMeasure->InitInstance() )
			return FALSE;
		if ( !pSrAnalysis->InitInstance() )
			return FALSE;
		MEAS_SrHead_ChangeCcdShutter(CSrHead::FILTER_OPEN);


		for(int i = 0; i < MEASYS_STAT_TYPE_MAX_NUM; i++)
		{
			switch(pSrMeasure->GetWaveLengthStepFromConfig()){
			case 1: /* SR_WAVELENGTH_STEP_05_NM */
				scanData[i].dWavelengthStep = 0.5;
				break;
			case 0: /* SR_WAVELENGTH_STEP_1_NM */
			default:
				scanData[i].dWavelengthStep = 1.0;
				break;
			}
		}

	}

	if ( systemConfig.HeadType.bSE ) {
		NanoSpecTitle(TITLE_MSG2_INIT_MEAS_HEAD_SE);
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if (systemConfig.HeadType.bCompEASE) {
			if ( !pCompEASEMeasure->InitInstance() )
				return FALSE;
		}
		else {
		if ( !pSeMeasure->InitInstance() )
			return FALSE;
		if ( !pSeAnalysis->InitInstance() )
			return FALSE;
	}
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	}

	if ( systemConfig.HeadType.bResist ) {
		NanoSpecTitle(TITLE_MSG2_INIT_MEAS_HEAD_RS);
		if ( !pRsMeasure->InitInstance() ) {
			_tcscpy(l_tszMsgFail, pMojiretsu->LoadString(IDS_RS_INITFAILCONT));
			_tcscpy(l_tszMsgTitle, pMojiretsu->LoadString(IDS_RS_TITLE));
			::MessageBox(NULL, l_tszMsgFail, l_tszMsgTitle, MB_OK | MB_ICONSTOP);
			pLogFile->Logging(l_tszMsgFail);
			return FALSE;
		}
	}

	if ( systemConfig.HeadType.bStress ) {
		NanoSpecTitle(TITLE_MSG2_INIT_MEAS_HEAD_STRESS);
		if ( !pStressMeasure->InitInstance() )
			return FALSE;
		if ( !pStressAnalysis->InitInstance() )
			return FALSE;
	}
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- { ---------- */
	/* CTA スレッドを起動	*/
	lgs_pclsCCtaCtrl = 0;
	if (0 != systemConfig.HeadType.bCTA) {
		NanoSpecTitle(TITLE_MSG2_INIT_MEAS_HEAD_CTA);
		nexioCA_Interlock(FALSE);	/* CTAILPO オフ */
		pLogFile->Logging(_T("MEASYS   : Starting CCtaCtrl..."));
		lgs_pclsCCtaCtrl = new CCtaCtrl(0);
		BOOL l_bRslt = TRUE;
		if (0 == lgs_pclsCCtaCtrl) {
			/* スレッド構築失敗 */
			pLogFile->Logging(_T("MEASYS   : Failed create CCtaCtrl. (new)"));
			l_bRslt = FALSE;
		} else
		if (0 == lgs_pclsCCtaCtrl->m_hThread) {
			/* スレッド起動失敗 */
			pLogFile->Logging(_T("MEASYS   : Failed start CCtaCtrl thread. (thread)"));
			l_bRslt = FALSE;
		}
		if (FALSE != l_bRslt) {
			pLogFile->Logging(_T("MEASYS   : Started CCtaCtrl."));
			/* 初期化処理	*/
			pLogFile->Logging(_T("MEASYS   : Initializing Cta unit..."));
			if (FALSE == lgs_pclsCCtaCtrl->Cta_Initialize()) {
				pLogFile->Logging(_T("MEASYS   : Failed to initialize Cta unit."));
				l_bRslt = FALSE;
			} else {
				pLogFile->Logging(_T("MEASYS   : Initialized Cta unit."));
			}
		}
// 2010.02.25 K.Matsuo -->
		if (FALSE == l_bRslt) {
			_tcscpy(l_tszMsgFail, pMojiretsu->LoadString(IDS_CTA_INITFAILCONT));
			_tcscpy(l_tszMsgTitle, pMojiretsu->LoadString(IDS_CTA_TITLE));
			::MessageBox(NULL, l_tszMsgFail, l_tszMsgTitle, MB_OK | MB_ICONSTOP);
			pLogFile->Logging(l_tszMsgFail);
			return FALSE;
		}
// 2010.02.25 K.Matsuo <--
	}
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- } ---------- */

	return TRUE;
}

// --------------------------------------------------------------------
// MEAS_SetNotifyWnd
void MEASYSAPI MEAS_SetNotifyWnd(HWND hWnd)
{
	TRACE(_T("MEAS_SetNotifyWnd()\n"));

	g_hNotifyWnd = hWnd;
}

// --------------------------------------------------------------------
// MEAS_Shutdown
BOOL MEASYSAPI MEAS_Shutdown(void)
{
	TRACE(_T("MEAS_Shutdown()\n"));

/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- { ---------- */
	/* CTA スレッドを終了	*/
	if (0 != lgs_pclsCCtaCtrl) {
		pLogFile->Logging(_T("MEASYS   : Deleting CCtaCtrl..."));
		delete lgs_pclsCCtaCtrl;
		lgs_pclsCCtaCtrl = 0;
		pLogFile->Logging(_T("MEASYS   : Deleted CCtaCtrl."));
	}
/* added 2009.10.20 hmenjo CTA 制御追加(１) ---------- } ---------- */

	SYSTEM_CONFIG systemConfig;
	ConfigFile_GetNanoSpecIni(&systemConfig, CONFIG_FILE_SYSTEM_CONFIG);

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
//	if ( systemConfig.HeadType.bSR || systemConfig.HeadType.bSE ) {
	if ( (systemConfig.HeadType.bSR) ||
		 (systemConfig.HeadType.bSE && systemConfig.HeadType.bCompEASE == FALSE) ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		pXmp->ExitInstance();
	}

	if ( systemConfig.HeadType.bSR ) {
		pSrMeasure->ExitInstance();
		pSrAnalysis->ExitInstance();
	}
	if ( systemConfig.HeadType.bSE ) {
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if (systemConfig.HeadType.bCompEASE) {
			pCompEASEMeasure->ExitInstance();
		}
		else {
		pSeMeasure->ExitInstance();
		pSeAnalysis->ExitInstance();
	}
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	}
	if ( systemConfig.HeadType.bResist ) {
		pRsMeasure->ExitInstance();
	}
	if ( systemConfig.HeadType.bStress ) {
		pStressMeasure->ExitInstance();
		pStressAnalysis->ExitInstance();
	}

	return TRUE;
}

// --------------------------------------------------------------------
// MEAS_Dark_T1
BOOL MEASYSAPI MEAS_Dark_T1(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("MEAS_Dark_T1()\n"));

	ASSERT(pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_SR || pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS);

	return pSrMeasure->Measure(scanData, ccdData, MEASYS_DARK_T1, pMeasProgInfo);
}

// --------------------------------------------------------------------
// MEAS_Dark_T2
BOOL MEASYSAPI MEAS_Dark_T2(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("MEAS_Dark_T2()\n"));

	ASSERT(pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_SR || pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS);

	return pSrMeasure->Measure(scanData, ccdData, MEASYS_DARK_T2, pMeasProgInfo);
}

// --------------------------------------------------------------------
// MEAS_1stReference
BOOL MEASYSAPI MEAS_1stReference(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("MEAS_1stReference()\n"));

	ASSERT(pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_SR || pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS);

	return pSrMeasure->Measure(scanData, ccdData, MEASYS_STDR_T1, pMeasProgInfo);
}

// --------------------------------------------------------------------
// MEAS_2ndReference_T1
BOOL MEASYSAPI MEAS_2ndReference_T1(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("MEAS_2ndReference_T1()\n"));

	ASSERT(pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_SR || pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS);

	return pSrMeasure->Measure(scanData, ccdData, MEASYS_EXTR_T1, pMeasProgInfo);
}

// --------------------------------------------------------------------
// MEAS_2ndReference_T2
BOOL MEASYSAPI MEAS_2ndReference_T2(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("MEAS_2ndReference_T2()\n"));

	ASSERT(pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_SR || pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS);

	return pSrMeasure->Measure(scanData, ccdData, MEASYS_EXTR_T2, pMeasProgInfo);
}

// --------------------------------------------------------------------
// MEAS_InitFirstMeasure
void MEASYSAPI MEAS_InitFirstMeasure()
{
	TRACE(_T("MEAS_InitFirstMeasure()\n"));

	::ZeroMemory(smAdapResultDataBase.GetSharedMemoryPtr(), sizeof(ADAPRESULTDATABASE));
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	::ZeroMemory(smCompEASEResultHelperDataBase.GetSharedMemoryPtr(), sizeof(COMPEASERESULT_HELPER));
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
}

// --------------------------------------------------------------------
// MEAS_PrepareForMeasure
BOOL MEASYSAPI MEAS_PrepareForMeasure(LPCTSTR pszLotId, LPCTSTR pszSampleId, LPCTSTR pszMainRecipeName, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr)
{
// 2009.10.02 K.Matsuo ストレス内のSR測定で問題があるので、初期化しない -->
//	::ZeroMemory(&g_prepareData, sizeof(g_prepareData));
// 2009.10.02 K.Matsuo ストレス内のSR測定で問題があるので、初期化しない <--

	g_prepareData.pszLotId			= pszLotId;
	g_prepareData.pszSampleId		= pszSampleId;
	g_prepareData.pszMainRecipeName = pszMainRecipeName;
	g_prepareData.pMainRcpInfo		= pMainRcpInfo;
	g_prepareData.pMeasProgInfo 	= pMeasProgInfo;
	g_prepareData.pStageProgInfoHdr = pStageProgInfoHdr;

/* deleted 2009.10.28 hmenjo CTA ロット，サンプル設定関数 ---------- { ---------- */
///* added 2009.10.27 hmenjo CTA ロット，サンプル設定関数 ---------- { ---------- */
//	if (HEAD_TYPE_CTA == pMainRcpInfo->MainRcpParam.hdr.wHeadType) {
//		/* CTA ヘッドの場合 */
//		if (0 == lgs_pclsCCtaCtrl) {
//			return FALSE;
//		} else {
//			::ZeroMemory(lg_smCtaResultDataBase.GetSharedMemoryPtr(), sizeof(CTARESULT));	/* CTA 用共有エリア クリア	*/
//			return lgs_pclsCCtaCtrl->Cta_SetCstSample(
//															g_prepareData.pszLotId, g_prepareData.pszSampleId,
//															pMainRcpInfo,
//															pMeasProgInfo
//														);
//		}
//	}
/* added 2009.10.27 hmenjo CTA ロット，サンプル設定関数 ---------- } ---------- */
/* deleted 2009.10.28 hmenjo CTA ロット，サンプル設定関数 ---------- } ---------- */

	return TRUE;
}

// --------------------------------------------------------------------
// MEAS_Measure
BOOL MEASYSAPI MEAS_Measure(int iScanCnt, const STAGE_COORD_XYZ* pStageCoordXyz)
{
	TRACE(_T("MEAS_Measure()\n"));

	BOOL bRet = FALSE;

	g_MeasPosXyz = *pStageCoordXyz;

	WORD wHeadType = g_prepareData.pMeasProgInfo->ScanParams.hdr.wHeadType;
	// ストレス測定内の膜厚測定
	if (wHeadType == HEAD_TYPE_STRESS) {
		wHeadType = HEAD_TYPE_SR;
	}

	switch ( wHeadType ) {
	case HEAD_TYPE_SR:
		if ( g_prepareData.pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G ) {
			// STAGE CONFIGとの突合せ必要
// 2009.10.26 K.Matsuo TR -->
			if ( MEASYS_GTR_1 <= iScanCnt && iScanCnt <= MEASYS_GTR_LAST ) {
				bRet = CSrRefFile::ReadGTrRefFile(scanData, g_prepareData.pszMainRecipeName, iScanCnt);
				if ( bRet )
					bRet = pSrMeasure->Measure(scanData, ccdData, MEASYS_SAMP, g_prepareData.pMeasProgInfo);
			}
// 2009.10.26 K.Matsuo TR <--
		}
		//2009.10.28 bagus 2point-distance --{--
		else if( g_prepareData.pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE) {
			//2点間距離測定の場合にはここに来てはいけない
			bRet = FALSE;
		}
		//2009.10.28 bagus 2point-distance --}--
		else {
			bRet = pSrMeasure->Measure(scanData, ccdData, MEASYS_SAMP, g_prepareData.pMeasProgInfo);
		}
		break;
	case HEAD_TYPE_SE:
		bRet = pSeMeasure->Measure(g_prepareData.pMeasProgInfo);
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.19 -->
//		bRet = pCompEASEMeasure->Measure(g_prepareData.pszSampleId, g_prepareData.pMeasProgInfo, g_prepareData.pMainRcpInfo);
		bRet = pCompEASEMeasure->Measure(iScanCnt, g_prepareData.pszSampleId, g_prepareData.pMeasProgInfo, g_prepareData.pMainRcpInfo);
// 2013.02.19 <--
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case HEAD_TYPE_4PP:
		bRet = pRsMeasure->Measure(g_prepareData.pMeasProgInfo);
		break;
/* added 2009.10.26 hmenjo CTA MEAS_Measure()呼出し ---------- { ---------- */
	case HEAD_TYPE_CTA:
		if (0 == lgs_pclsCCtaCtrl) {
			bRet = FALSE;
		} else {
			bRet = lgs_pclsCCtaCtrl->Cta_Measure(iScanCnt, pStageCoordXyz);
		}
		break;
/* added 2009.10.26 hmenjo CTA MEAS_Measure()呼出し ---------- } ---------- */
	default:
		ASSERT(FALSE);
		break;
	}

	return bRet;
}

// --------------------------------------------------------------------
// MEAS_GTrReferenceMeasure
BOOL MEASYSAPI MEAS_GTrReferenceMeasure(int iScanCnt, const STAGE_COORD_XYZ* pStageCoordXyz)
{
	TRACE(_T("MEAS_GTrReferenceMeasure()\n"));

	BOOL bRet = FALSE;
	int iMeaSysStatus;

	g_MeasPosXyz = *pStageCoordXyz;

	if ( g_prepareData.pMeasProgInfo->ScanParams.hdr.wHeadType != HEAD_TYPE_SR )
		return FALSE;
	if ( g_prepareData.pMeasProgInfo->ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G )
		return FALSE;

// 2009.11.20 K.Matsuo GTr ごみのリファレンスデータが残っているので初期化する -->
	if ( MEASYS_GTR_1 == iScanCnt )
		::ZeroMemory(scanData, sizeof(scanData));
// 2009.11.20 K.Matsuo GTr ごみのリファレンスデータが残っているので初期化する <--

// 2009.10.26 K.Matsuo TR -->
	if ( MEASYS_GTR_1 <= iScanCnt && iScanCnt <= MEASYS_GTR_LAST ) {
		// SR GTr（ガントリー透過率）の場合、リファレンスポイント番号の位置に、データを格納
		iMeaSysStatus = iScanCnt;
		bRet = pSrMeasure->GTrReferecneMeasure(scanData, ccdData, iMeaSysStatus, g_prepareData.pMeasProgInfo);
	}
// 2009.10.26 K.Matsuo TR <--

	return bRet;
}

// --------------------------------------------------------------------
// MEAS_Analysis
BOOL MEASYSAPI MEAS_Analysis(int iScanCnt, CDdeClient* pXmpDde)
{
	TRACE(_T("MEAS_Analysis()\n"));

	TRACE("********MEASYS MEAS_Analysis start %s", CTime::GetCurrentTime().Format("%H:%M:%S"));

	TCHAR szBuff[256];
	TCHAR szAdapResult[4096];
	TCHAR szAdapRename[ADAPRESULTSTRINGLENMAX + 1];
	TCHAR szAdapUnit[ADAPRESULTSTRINGLENMAX + 1];
	TCHAR* token;
	CScanRange scanRange(g_prepareData.pMeasProgInfo);
	CXmpHelper xmpHelper;
	int i;
	int iRow, iCol;
	RECALIB_PROG_INFO recalibProgInfo;
	SR_XMP srXmp;
	BOOL bFoundThickness;
	LPTSTR pszAdapResult;

	// CleanUp
	iRow = iScanCnt -1;
	for ( iCol = 0; iCol < ADAPRESULT_COLS_MAX; iCol++ ) {
		strcpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], "");
		strcpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], "");
		smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol] = 0.0;
	}

	WORD wHeadType = g_prepareData.pMeasProgInfo->ScanParams.hdr.wHeadType;

// 2014.04.04 bagus wavelength step added -->
	SR_CONFIG srConfig;
	ConfigFile_GetNanoSpecIni(&srConfig, CONFIG_FILE_SR_CONFIG);
// 2014.04.04 bagus wavelength step added <--

	switch ( wHeadType )
	{
	case HEAD_TYPE_SR:
	case HEAD_TYPE_STRESS:	// ストレス測定内の膜厚測定
		::ZeroMemory(&analysisData, sizeof(analysisData));

		analysisData.iScanStart = scanRange.GetScanStartRange();
		analysisData.iScanEnd = scanRange.GetScanEndRange();
// 2014.04.04 bagus wavelength step added -->
		switch(srConfig.nWavelengthStep){
		case 1: /* SR_WAVELENGTH_STEP_05_NM */
			analysisData.dWavelengthStep = 0.5;
			break;
		case 0: /* SR_WAVELENGTH_STEP_1_NM */
		default:
			analysisData.dWavelengthStep = 1.0;
			break;
		}
// 2014.04.04 bagus wavelength step added <--
		if ( !pSrAnalysis->ComputeRate(&analysisData, scanData, g_prepareData.pMeasProgInfo) )
			return FALSE;

		switch ( g_prepareData.pMeasProgInfo->ScanParams.hdr.wScanType )
		{
		case MEAS_PROG_TYPE_SR_THICKNESS:
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
			pLogFile->Logging("MEAS_Analysis() : DoXmp() Start.");
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */
			if ( !pSrAnalysis->DoXmp(szAdapResult, &analysisData, g_prepareData.pMainRcpInfo, g_prepareData.pMeasProgInfo, pXmpDde) )
				return FALSE;
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
			pLogFile->Logging("MEAS_Analysis() : DoXmp() End.");
			pLogFile->Logging("MEAS_Analysis() : LoadParams() Start.");
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */
			if ( !xmpHelper.LoadParams(g_prepareData.pMeasProgInfo->ScanParams._SR.XMPDesc.szStrategyEntry) )
				return FALSE;
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
			pLogFile->Logging("MEAS_Analysis() : LoadParams() End.");
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */

			// ヘッダーアイテム名を保存
			iCol = 0;
			iRow = iScanCnt - 1;
			if ( strncmp(szAdapResult, "Fit:", 4) == 0 )
				pszAdapResult = &szAdapResult[4];
			else
				pszAdapResult = szAdapResult;
			token = _tcstok(pszAdapResult, _T("=,"));			// 最初のトークンを取得します
			while ( token != NULL )
			{
				// 2009.08.19 K.Matsuo -->
				bFoundThickness = FALSE;
				// 2009.08.19 K.Matsuo <--

				// Label
				if ( xmpHelper.GetAdapRename(szAdapRename, szAdapUnit, token) )
					_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], szAdapRename);
				else
					_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], token);
// 2009.11.12 K.Matsuo ADAPの報告項目の制約対応。2個目のN1[XXX],K1[XXX]をのっとり、別項目の値を報告 -->
//				_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], token);
				if ( strncmp(szAdapRename, "Const", strlen("Const")) == 0 ||
					 strncmp(szAdapRename, "Eg", strlen("Eg")) == 0 ||
					 strncmp(szAdapRename, "Fraction", strlen("Fraction")) == 0 ) {
					_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], szAdapRename);
				}
				else {
					_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], token);
				}
// 2009.11.12 K.Matsuo ADAPの報告項目の制約対応。2個目のN1[XXX],K1[XXX]をのっとり、別項目の値を報告 <--
				// 2009.08.19 K.Matsuo -->
				// ストレス計算で膜厚を使用するため、SR測定時に横取りしておく
				if ( strcmp(token, _T("Thick1")) == 0 )
					bFoundThickness = TRUE;
				// 2009.08.19 K.Matsuo <--
				token = _tcstok(NULL, _T("=,"));			// 次のトークンを取得します
				// Data
				smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol] = atof(token);
				if ( wHeadType == HEAD_TYPE_SR ) {
					for  ( int i = 0; i < RECALIB_MAX; i++ ) {
						if ( g_prepareData.pMainRcpInfo->MainRcpParam._SR.RecalibItem[i] == 0 ) 				// RecalibItemのindex 0は、skip
							continue;
						if ( iCol == (g_prepareData.pMainRcpInfo->MainRcpParam._SR.RecalibItem[i] - 1) ) {	// RecalibItemのindexは、1から始まるため、1減算
							_tcscpy(szBuff, g_prepareData.pMainRcpInfo->MainRcpParam._SR.szRecalib[i]);
//							if ( _tcslen(szBuff) > 0 && (_tcscmp(szBuff, "None") != 0 && _tcscmp(szBuff, "無し") != 0) ) {
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
								pLogFile->Logging("MEAS_Analysis() : MEAS_Recalib() Start.");
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */
								if ( !MEAS_Recalib(szBuff, smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol]) )
									return FALSE;
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- { ---------- */
								pLogFile->Logging("MEAS_Analysis() : MEAS_Recalib() End.");
/* added 2019.02.01 hmenjo ログ追加 Ph1 ---------- } ---------- */
								break;							// リキャリブレーション成功
//							}
						}
					}
// 2009.12.10 K.Matsuo -->
				}
				if ( wHeadType == HEAD_TYPE_STRESS ) {
// 2009.12.10 K.Matsuo <--
					// 2009.08.19 K.Matsuo -->
					if ( bFoundThickness ) {
						for ( int i = 0; i < STRESS_LINES_MAX; i++ ) {
							if ( thicknessForStress[i].bValidLine == TRUE &&
								 thicknessForStress[i].lScanPosY == g_MeasPosXyz.lY ) {
// 2009.12.10 K.Matsuo -->
//								thicknessForStress[i].cf.point[thicknessForStress[i].cf.datanum] = (double)g_MeasPosXyz.lX;
								thicknessForStress[i].cf.point[thicknessForStress[i].cf.datanum] = ((double)g_MeasPosXyz.lX) / MICROMETRE;
// 2009.12.10 K.Matsuo -->
								thicknessForStress[i].cf.value[thicknessForStress[i].cf.datanum] = smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol];
								thicknessForStress[i].cf.datanum++;
								break;
							}
						}
					}
					// 2009.08.19 K.Matsuo <--
				}
				token = _tcstok(NULL, _T("=,"));			// 次のトークンを取得します
				iCol++;
			}

			if ( g_prepareData.pMeasProgInfo->ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag )	{
				int iIndex;
				for ( int i = 0; i < 3; i++ ) {
					if ( g_prepareData.pMeasProgInfo->ScanParams._SR.iSpecificWavelen[i] ) {
						sprintf(szBuff, "Target%d", i + 1);
						_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], szBuff);
						sprintf(szBuff, "Reflect%d", i + 1);
						_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], szBuff);

						switch(srConfig.nWavelengthStep){
						case 1: /* SR_WAVELENGTH_STEP_05_NM */
							iIndex = (g_prepareData.pMeasProgInfo->ScanParams._SR.iSpecificWavelen[i] - analysisData.iScanStart) * 2;
							break;
						case 0: /* SR_WAVELENGTH_STEP_1_NM */
						default:
							iIndex = g_prepareData.pMeasProgInfo->ScanParams._SR.iSpecificWavelen[i] - analysisData.iScanStart;
						}
						smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol] = analysisData.dAnalysisReflectanceData[iIndex];
						iCol++;
					}
				}
			}
			break;
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			iCol = 0;
			iRow = iScanCnt - 1;

			ConfigFile_GetNanoSpecIni(&srXmp, CONFIG_FILE_SR_XMP);

			// AFT2ADAP
			if ( !pSrAnalysis->WriteAft2AdapFile(&analysisData, g_prepareData.pMainRcpInfo, g_prepareData.pMeasProgInfo, srXmp.szDataPath) )
				return FALSE;

			for ( i = 0; i < 3; i++ ) {
				if ( g_prepareData.pMeasProgInfo->ScanParams._SR.iSpecificWavelen[i] ) {
					sprintf(szBuff, "Target%d", i + 1);
					_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], szBuff);
					// スキャンタイプ別のラベル名を作成
					if ( g_prepareData.pMeasProgInfo->ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE ) {
						sprintf(szBuff, "Reflect%d", i + 1);
					}
					else {
						//	MEAS_PROG_TYPE_SR_TRANSMITTANCE
						//	MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
						sprintf(szBuff, "Transmit%d", i + 1);
					}
					_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], szBuff);

					int iIndex;
					switch(srConfig.nWavelengthStep){
					case 1: /* SR_WAVELENGTH_STEP_05_NM */
						iIndex = (g_prepareData.pMeasProgInfo->ScanParams._SR.iSpecificWavelen[i] - analysisData.iScanStart) * 2;
						break;
					case 0: /* SR_WAVELENGTH_STEP_1_NM */
					default:
						iIndex = g_prepareData.pMeasProgInfo->ScanParams._SR.iSpecificWavelen[i] - analysisData.iScanStart;
					}
					smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol] = analysisData.dAnalysisReflectanceData[iIndex];

					// リキャリブレーション
					_tcscpy(szBuff, g_prepareData.pMainRcpInfo->MainRcpParam._SR.szRecalib[i]);
//					if ( _tcslen(szBuff) > 0 && (_tcscmp(szBuff, "None") != 0 && _tcscmp(szBuff, "無し") != 0) ) {
						if ( !MEAS_Recalib(szBuff, smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol]) )
							return FALSE;
//					}
					iCol++;
				}
			}
			break;
//		case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
//			pSrAnalysis->ComputeCIEpart1(g_prepareData.pMeasProgInfo, scanData);
//			pSrAnalysis->ComputeCIEpart2();
//			break;
//		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
//			pSrAnalysis->ComputeCIEpart1(g_prepareData.pMeasProgInfo, scanData);
//			pSrAnalysis->ComputeCIEpart2();
//			break;
//		case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
//			break;
		default:
			ASSERT( FALSE );
			break;
		}
		break;
	case HEAD_TYPE_SE:
		if ( !pSeAnalysis->DoXmp(szAdapResult, g_prepareData.pMeasProgInfo, pXmpDde) )
			return FALSE;
		if ( !xmpHelper.LoadParams(g_prepareData.pMeasProgInfo->ScanParams._SE.szStrategyEntry) )
			return FALSE;

		// ヘッダーアイテム名を保存
		iCol = 0;
		iRow = iScanCnt - 1;
		if ( strncmp(szAdapResult, "Fit:", 4) == 0 )
			pszAdapResult = &szAdapResult[4];
		else
			pszAdapResult = szAdapResult;
		token = _tcstok(pszAdapResult, _T("=,"));			// 最初のトークンを取得します
		while ( token != NULL )
		{
			// Label
			if ( xmpHelper.GetAdapRename(szAdapRename, szAdapUnit, token) )
				_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], szAdapRename);
			else
				_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], token);
// 2009.11.12 K.Matsuo ADAPの報告項目の制約対応。2個目のN1[XXX],K1[XXX]をのっとり、別項目の値を報告 -->
//			_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], token);
			if ( strncmp(szAdapRename, "Const", strlen("Const")) == 0 ||
				 strncmp(szAdapRename, "Eg", strlen("Eg")) == 0 ||
				 strncmp(szAdapRename, "Fraction", strlen("Fraction")) == 0 ) {
				_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], szAdapRename);
			}
			else {
				_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], token);
			}
// 2009.11.12 K.Matsuo ADAPの報告項目の制約対応。2個目のN1[XXX],K1[XXX]をのっとり、別項目の値を報告 <--
			token = _tcstok(NULL, _T("=,"));			// 次のトークンを取得します
			// Data
			smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol] = atof(token);
			for  ( int i = 0; i < RECALIB_MAX; i++ ) {
				if ( g_prepareData.pMainRcpInfo->MainRcpParam._SE.RecalibItem[i] == 0 ) 			// RecalibItemのindex 0は、skip
					continue;
				if ( iCol == (g_prepareData.pMainRcpInfo->MainRcpParam._SE.RecalibItem[i] - 1) ) {	// RecalibItemのindexは、1から始まるため、1減算
					_tcscpy(szBuff, g_prepareData.pMainRcpInfo->MainRcpParam._SE.szRecalib[i]);
//					if ( _tcslen(szBuff) > 0 && (_tcscmp(szBuff, "None") != 0 && _tcscmp(szBuff, "無し") != 0) ) {
						if ( !MEAS_Recalib(szBuff, smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol]) )
							return FALSE;
						break;							// リキャリブレーション成功
//					}
				}
			}
			token = _tcstok(NULL, _T("=,"));			// 次のトークンを取得します
			iCol++;
		}
		break;
//	case HEAD_TYPE_IRSE:
	case HEAD_TYPE_4PP:
		break;
//	case HEAD_TYPE_CTA:
//	case HEAD_TYPE_STRESS:
//		break;
	default:
		ASSERT( FALSE );
		break;
	}

	TRACE("********MEASYS MEAS_Analysis end %s", CTime::GetCurrentTime().Format("%H:%M:%S"));

	return TRUE;
}

int MEASYSAPI MEAS_CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime)
{
	return CSrRefFile::CheckRefFileElapsedTimeOut(pszMainRecipeName, dLifeTime);
}

int MEASYSAPI MEAS_CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime,struct tm &tmFileTime)
{
	return CSrRefFile::CheckRefFileElapsedTimeOut(pszMainRecipeName, dLifeTime,tmFileTime);
}

BOOL MEASYSAPI MEAS_ReadRefFile(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::ReadRefFile(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_ReadGTrRefFile(LPCTSTR pszMainRecipeName, int iCoordRefPos)
{
	return CSrRefFile::ReadGTrRefFile(scanData, pszMainRecipeName, iCoordRefPos);
}

BOOL MEASYSAPI MEAS_ReadRefFile2ndRefT1(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::ReadRefFile2ndRefT1(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_ReadRefFile2ndRefT2(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::ReadRefFile2ndRefT2(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_WriteRefFile(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::WriteRefFile(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_WriteGTrRefFile(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::WriteGTrRefFile(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_WriteRefFile2ndRefT1(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::WriteRefFile2ndRefT1(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_WriteRefFile2ndRefT2(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::WriteRefFile2ndRefT2(scanData, pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_Is2ndRefT1FileExist(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::Is2ndRefT1FileExist(pszMainRecipeName);
}

BOOL MEASYSAPI MEAS_Is2ndRefT2FileExist(LPCTSTR pszMainRecipeName)
{
	return CSrRefFile::Is2ndRefT2FileExist(pszMainRecipeName);
}

// --------------------------------------------------------------------
// MEAS_SrHead_CcdScan
BOOL MEASYSAPI MEAS_SrHead_CcdScan(double dIntegTime)
{
	TRACE(_T("MEAS_SrHead_CcdScan()\n"));

	if ( !pSrMeasure->IsIdle() ) return FALSE;

	if ( !pSrMeasure->CcdScan(ccdData, dIntegTime, TRUE) )
		return FALSE;

// 2010.01.07 K.Matsuo -->
	SR_CONFIG srConfig;
	ConfigFile_GetNanoSpecIni(&srConfig, CONFIG_FILE_SR_CONFIG);

	SCANDATA* pScanData = &(scanData[MEASYS_STDR_T1]);
	::ZeroMemory(pScanData, sizeof(SCANDATA));

	pScanData->iScanStart = CScanRange::MIN_VS_SCANL;
	if ( srConfig.nHeadType == 5 )	/* SR_HEAD_TYPE_LAH1024_SS */
		pScanData->iScanEnd = CScanRange::MAX_SS_SCAN;
	else
		pScanData->iScanEnd = CScanRange::MAX_CS_SCAN;

// 2014.04.04 bagus wavelength step added -->
	switch(srConfig.nWavelengthStep){
	case 1: /* SR_WAVELENGTH_STEP_05_NM */
		pScanData->dWavelengthStep = 0.5;
		break;
	case 0: /* SR_WAVELENGTH_STEP_1_NM */
	default:
		pScanData->dWavelengthStep = 1.0;
		break;
	}
// 2014.04.04 bagus wavelength step added <--

	pSrMeasure->GetScanData(pScanData->dScanData, pScanData->iScanStart, pScanData->iScanEnd, ccdData);

	return TRUE;
// 2010.01.07 K.Matsuo <--
}

// --------------------------------------------------------------------
// MEAS_SrHead_GetCcdData
int MEASYSAPI MEAS_SrHead_GetCcdData(int iPixel)
{
	TRACE(_T("MEAS_SrHead_GetCcdData()\n"));

	return ccdData[iPixel-1];
}

// --------------------------------------------------------------------
// MEAS_SrHead_ChangeCcdShutter
BOOL MEASYSAPI MEAS_SrHead_ChangeCcdShutter(WORD wFilter)
{
	TRACE(_T("MEAS_SrHead_ChangeCcdShutter()\n"));

	if ( !pSrMeasure->IsIdle() ) return FALSE;

	return pSrMeasure->ChangeCcdShutter(wFilter);
}

// --------------------------------------------------------------------
// MEAS_SrHead_OpenUvShutter
BOOL MEASYSAPI MEAS_SrHead_OpenUvShutter(void)
{
	TRACE(_T("MEAS_SrHead_OpenUvShutter()\n"));

	if ( !pSrMeasure->IsIdle() ) return FALSE;

	return pSrMeasure->OpenUvShutter();
}

// --------------------------------------------------------------------
// MEAS_SrHead_CloseUvShutter
BOOL MEASYSAPI MEAS_SrHead_CloseUvShutter(void)
{
	TRACE(_T("MEAS_SrHead_CloseUvShutter()\n"));

	if ( !pSrMeasure->IsIdle() ) return FALSE;

	return pSrMeasure->CloseUvShutter();
}

// --------------------------------------------------------------------
// MEAS_SrHead_OpenTransShutter
BOOL MEASYSAPI MEAS_SrHead_OpenTransShutter(void)
{
	TRACE(_T("MEAS_SrHead_OpenTransShutter()\n"));

	if ( !pSrMeasure->IsIdle() ) return FALSE;

	return pSrMeasure->OpenTransShutter();
}

// --------------------------------------------------------------------
// MEAS_SrHead_CloseTransShutter
BOOL MEASYSAPI MEAS_SrHead_CloseTransShutter(void)
{
	TRACE(_T("MEAS_SrHead_CloseTransShutter()\n"));

	if ( !pSrMeasure->IsIdle() ) return FALSE;

	return pSrMeasure->CloseTransShutter();
}


// --------------------------------------------------------------------
// MEAS_GetCcdData
int MEASYSAPI MEAS_GetCcdData(int iPixel)
{
	TRACE(_T("MEAS_GetCcdData()\n"));

	// 測定ポジション（測定回数）のデータ位置取得 CCDデータの取得
	return ccdData[iPixel - 1];
}

#if 0 // 2014.04.04 bagus wavelength step modified -->
// // --------------------------------------------------------------------
// // MEAS_GetScanDark_T1Data
// double MEASYSAPI MEAS_GetScanDark_T1Data(int iWave)
// {
// 	TRACE(_T("MEAS_GetScanDark_T1Data()\n"));
//
// 	return scanData[MEASYS_DARK_T1].dScanData[iWave - 1];
// }
//
// // --------------------------------------------------------------------
// // MEAS_GetScanDark_T2Data
// double MEASYSAPI MEAS_GetScanDark_T2Data(int iWave)
// {
// 	TRACE(_T("MEAS_GetScanDark_T2Data()\n"));
//
// 	return scanData[MEASYS_DARK_T2].dScanData[iWave - 1];
// }
//
// // --------------------------------------------------------------------
// // MEAS_GetScan1stReferenceData
// double MEASYSAPI MEAS_GetScan1stReferenceData(int iWave)
// {
// 	TRACE(_T("MEAS_GetScan1stReferenceData()\n"));
//
// 	return scanData[MEASYS_STDR_T1].dScanData[iWave - 1];
// }
//
// // --------------------------------------------------------------------
// // MEAS_GetScan2ndReference_T1Data
// double MEASYSAPI MEAS_GetScan2ndReference_T1Data(int iWave)
// {
// 	TRACE(_T("MEAS_GetScan2ndReference_T1Data()\n"));
//
// 	return scanData[MEASYS_EXTR_T1].dScanData[iWave - 1];
// }
//
// // --------------------------------------------------------------------
// // MEAS_GetScan2ndReference_T2Data
// double MEASYSAPI MEAS_GetScan2ndReference_T2Data(int iWave)
// {
// 	TRACE(_T("MEAS_GetScan2ndReference_T2Data()\n"));
//
// 	return scanData[MEASYS_EXTR_T2].dScanData[iWave - 1];
// }
//
// // --------------------------------------------------------------------
// // MEAS_GetScanSampleData
// double MEASYSAPI MEAS_GetScanSampleData(int iWave)
// {
// 	TRACE(_T("MEAS_GetScanSampleData()\n"));
//
// 	return scanData[MEASYS_SAMP].dScanData[iWave - 1];
// }
//
#else
// --------------------------------------------------------------------
// MEAS_GetScanDark_T1Data
double MEASYSAPI MEAS_GetScanDark_T1Data(double dWave)
{
	TRACE(_T("MEAS_GetScanDark_T1Data()\n"));

	return scanData[MEASYS_DARK_T1].GetScanData(dWave);
}

// --------------------------------------------------------------------
// MEAS_GetScanDark_T2Data
double MEASYSAPI MEAS_GetScanDark_T2Data(double dWave)
{
	TRACE(_T("MEAS_GetScanDark_T2Data()\n"));

	return scanData[MEASYS_DARK_T2].GetScanData(dWave);
}

// --------------------------------------------------------------------
// MEAS_GetScan1stReferenceData
double MEASYSAPI MEAS_GetScan1stReferenceData(double dWave)
{
	TRACE(_T("MEAS_GetScan1stReferenceData()\n"));

	return scanData[MEASYS_STDR_T1].GetScanData(dWave);
}

// --------------------------------------------------------------------
// MEAS_GetScan2ndReference_T1Data
double MEASYSAPI MEAS_GetScan2ndReference_T1Data(double dWave)
{
	TRACE(_T("MEAS_GetScan2ndReference_T1Data()\n"));

	return scanData[MEASYS_EXTR_T1].GetScanData(dWave);
}

// --------------------------------------------------------------------
// MEAS_GetScan2ndReference_T2Data
double MEASYSAPI MEAS_GetScan2ndReference_T2Data(double dWave)
{
	TRACE(_T("MEAS_GetScan2ndReference_T2Data()\n"));

	return scanData[MEASYS_EXTR_T2].GetScanData(dWave);
}

// --------------------------------------------------------------------
// MEAS_GetScanSampleData
double MEASYSAPI MEAS_GetScanSampleData(double dWave)
{
	TRACE(_T("MEAS_GetScanSampleData()\n"));

	return scanData[MEASYS_SAMP].GetScanData(dWave);
}
#endif // 2014.04.04 bagus wavelength step modified <--

// 2009.11.16 K.Matsuo リキャリブレーション要・不要判定 -->
// --------------------------------------------------------------------
// IsNoRecalibration
BOOL IsNoRecalibration(LPCTSTR szRecalib)
{
	if ( _tcscmp(szRecalib, "None") == 0 || _tcscmp(szRecalib, "無し") == 0 ) {
		return TRUE;
	}

	return FALSE;
}
// 2009.11.16 K.Matsuo リキャリブレーション要・不要判定 <--

// --------------------------------------------------------------------
// MEAS_Recalib
BOOL MEAS_Recalib(LPCTSTR szRecalib, double& dData)
{
	TRACE(_T("MEAS_Recalib()\n"));

	double dXw;
	double dYw;
	double dDX;
	double dR;
	RECALIB_PROG_INFO recalibProgInfo;
	int iMaxEntryNum;

// 2009.11.16 K.Matsuo リキャリブレーション要・不要判定 -->
	if ( _tcslen(szRecalib) == 0 ) {
// 2010.02.01 K.Matsuo テストモード時、0文字でくるので、システムエラーから不要へ取扱いを変更する -->
//		// リキャリブレーションのデータ指定誤り（システムエラー）
//		return FALSE;
		return TRUE;
// 2010.02.01 K.Matsuo テストモード時、0文字でくるので、システムエラーから不要へ取扱いを変更する <--
	}

	if ( IsNoRecalibration(szRecalib) ) {
		// リキャリブレーション不要なので、リターンする
		return TRUE;
	}
// 2009.11.16 K.Matsuo リキャリブレーション要・不要判定 <--

	// リキャリブレーションPGMのデータ取得
	if ( !RecipeFile_LoadRecipe(&recalibProgInfo, szRecalib, RECIPE_FILE_RECALIBRATION_PROGRAM) )
		return FALSE;
/* added 2010.05.11 hmenjo リキャリブ単位バグ修正 ---------- { ---------- */
	if (1 == recalibProgInfo.wUnits) {
		/* 単位が[A]の場合のみ[nm]に変換します．	*/
		for (int i = 0; i < RECALIB_MAX_ENTRY; i++) {
			recalibProgInfo.RecalibEntry[i].dActual /= 10.0;
			recalibProgInfo.RecalibEntry[i].dMeasured /= 10.0;
		}
	}
/* added 2010.05.11 hmenjo リキャリブ単位バグ修正 ---------- } ---------- */

	RECALIB_ENTRY l_RecalibEntry[RECALIB_MAX_ENTRY + 1];						// リキャリブレーション

	::ZeroMemory(l_RecalibEntry, sizeof(l_RecalibEntry));
	for ( int i = 0; i < RECALIB_MAX_ENTRY; i++ )
		l_RecalibEntry[i+1] = recalibProgInfo.RecalibEntry[i];					// indexの0は、actual=0.0,measured=0.0とする
/* added 2015.12.06 hmenjo リカリブ逆相関対応 ---------- { ---------- */
	if (2 <= recalibProgInfo.iEntryNum) {
		/* v2.10 未満は逆相関データは存在しませんので処理されません．	*/
		if (l_RecalibEntry[1].dActual > l_RecalibEntry[2].dActual) {
/* modified 2015.12.10 hmenjo リカリブ逆相関対応 ---------- { ---------- */
//			/* 基準値のオフセット１→２の変化が０未満の場合は
//				基準値のオフセット０をオフセット１と同じ値にします	*/
//			l_RecalibEntry[0].dActual = l_RecalibEntry[1].dActual;
/* modified 2015.12.10 hmenjo リカリブ逆相関対応 ----------              */
			/* 基準値のオフセット１→２の変化が０未満の場合は
				基準値のオフセット０をオフセット１→２の傾きで求めます．	*/
			double l_dX1 = l_RecalibEntry[1].dMeasured;
			double l_dX2 = l_RecalibEntry[2].dMeasured;
			double l_dY1 = l_RecalibEntry[1].dActual;
			double l_dY2 = l_RecalibEntry[2].dActual;
			double l_dDx = l_dX2 - l_dX1;
			double l_dDy = l_dY2 - l_dY1;
			double l_dYY = l_dY1 + (0.0 - l_dX1) * l_dDy / l_dDx;
			if (DBL_MAX < l_dYY) {
				l_dYY = DBL_MAX;
			}
			l_RecalibEntry[0].dActual = l_dYY;
/* modified 2015.12.10 hmenjo リカリブ逆相関対応 ---------- } ---------- */
		}
	}
/* added 2015.12.06 hmenjo リカリブ逆相関対応 ---------- } ---------- */

	i = 0;
	if ( l_RecalibEntry[i].dMeasured < dData ) {
		if ( recalibProgInfo.iEntryNum == -1 ) {
			// v2.10未満とのデータ互換性のためのロジック
			iMaxEntryNum = RECALIB_MAX_ENTRY + 1;
			for ( i = 1; i < iMaxEntryNum; i++ ) {
				if ( i > 1 && l_RecalibEntry[i].dMeasured == 0.0 ) {	// データ個数が不明なので、0.0をストッパーとみなしている
					i -= 2;
					break;
				}
				if ( l_RecalibEntry[i].dMeasured >= dData ) {
					i -= 1;
					break;
				}
			}

			// インデックスが最後尾の場合、データ間に収まるように１減算
			if ( i >= iMaxEntryNum - 1 )
				i = iMaxEntryNum - 2;

			if ( l_RecalibEntry[i + 1].dMeasured == 0.0 )				// データ個数が不明なので、0.0をストッパーとみなしている
				i -= 1;
		}
		else {
			// v2.10以降のロジック
			iMaxEntryNum = recalibProgInfo.iEntryNum + 1;
			for ( i = 1; i < iMaxEntryNum; i++ ) {
				if ( l_RecalibEntry[i].dMeasured >= dData ) {
					i -= 1;
					break;
				}
			}

			// インデックスが最後尾の場合、データ間に収まるように１減算
			if ( i >= iMaxEntryNum - 1 )
				i = iMaxEntryNum - 2;
		}

		if ( i < 0 )				// Out of Recalibration
			return FALSE;
	}

	dXw = l_RecalibEntry[i + 1].dMeasured - l_RecalibEntry[i].dMeasured;
	dYw = l_RecalibEntry[i + 1].dActual - l_RecalibEntry[i].dActual;
	dDX = dData - l_RecalibEntry[i].dMeasured;
	dR = l_RecalibEntry[i].dActual + dDX * dYw / dXw;

	if ( dR < 0.0 ) 					// Recalibration is not correct.
		dData = 0.0;
	else
		dData = dR;

	return TRUE;
}

// 2009.09.15 K.Matsuo リキャリブレーションのマイナス値入力対応 -->
// --------------------------------------------------------------------
// MEAS_RecalibMinusEntry
BOOL MEAS_RecalibMinusEntry(LPCTSTR szRecalib, double& dData)
{
	TRACE(_T("MEAS_RecalibMinusEntry()\n"));

	double dXw;
	double dYw;
	double dDX;
	double dR;
	RECALIB_PROG_INFO recalibProgInfo;
	int iMaxEntryNum;

// 2009.11.16 K.Matsuo リキャリブレーション要・不要判定 -->
	if ( _tcslen(szRecalib) == 0 ) {
// 2010.02.01 K.Matsuo テストモード時、0文字でくるので、システムエラーから不要へ取扱いを変更する -->
//		// リキャリブレーションのデータ指定誤り（システムエラー）
//		return FALSE;
		return TRUE;
// 2010.02.01 K.Matsuo テストモード時、0文字でくるので、システムエラーから不要へ取扱いを変更する <--
	}

	if ( IsNoRecalibration(szRecalib) ) {
		// リキャリブレーション不要なので、リターンする
		return TRUE;
	}
// 2009.11.16 K.Matsuo リキャリブレーション要・不要判定 <--

	// リキャリブレーションPGMのデータ取得
	if ( !RecipeFile_LoadRecipe(&recalibProgInfo, szRecalib, RECIPE_FILE_RECALIBRATION_PROGRAM) )
		return FALSE;
/* added 2010.05.11 hmenjo リキャリブ単位バグ修正 ---------- { ---------- */
	if (1 == recalibProgInfo.wUnits) {
		/* 単位が[A]の場合のみ[nm]に変換します．	*/
		for (int i = 0; i < RECALIB_MAX_ENTRY; i++) {
			recalibProgInfo.RecalibEntry[i].dActual /= 10.0;
			recalibProgInfo.RecalibEntry[i].dMeasured /= 10.0;
		}
	}
/* added 2010.05.11 hmenjo リキャリブ単位バグ修正 ---------- } ---------- */

	int i = 0;
	iMaxEntryNum = recalibProgInfo.iEntryNum;
	if ( recalibProgInfo.RecalibEntry[i].dMeasured < dData ) {
		for ( i = 1; i < iMaxEntryNum; i++ ) {
			if ( recalibProgInfo.RecalibEntry[i].dMeasured >= dData ) {
				i -= 1;
				break;
			}
		}

		// インデックスが最後尾の場合、データ間に収まるように１減算
		if ( i >= iMaxEntryNum - 1 )
			i = iMaxEntryNum - 2;

		if ( i < 0 )				// Out of Recalibration
			return FALSE;
	}

	dXw = recalibProgInfo.RecalibEntry[i + 1].dMeasured - recalibProgInfo.RecalibEntry[i].dMeasured;
	dYw = recalibProgInfo.RecalibEntry[i + 1].dActual - recalibProgInfo.RecalibEntry[i].dActual;
	dDX = dData - recalibProgInfo.RecalibEntry[i].dMeasured;
	dR = recalibProgInfo.RecalibEntry[i].dActual + dDX * dYw / dXw;

	dData = dR;

	return TRUE;
}
// 2009.09.15 K.Matsuo リキャリブレーションのマイナス値入力対応 <--

// --------------------------------------------------------------------
// MEAS_GetStratetyType
void MEASYSAPI MEAS_GetStratetyType(BOOL& bSr, BOOL& bSe, BOOL& bStress, LPCTSTR pszStrategyEntry)
{
	BOOL bUsesReflectometer = FALSE;
	BOOL bUsesEllipsometer = FALSE;

// 2009.12.07 K.Matsuo ストラテジー番号1は、リスト対象外 -->
	if ( atoi(pszStrategyEntry) == 1 ) {
		bSr = FALSE;
		bSe = FALSE;
		bStress = FALSE;
		return;
	}
// 2009.12.07 K.Matsuo ストラテジー番号1は、リスト対象外 <--

// 2009.12.15 K.Matsuo Stress Type -->
//	 "(STR)"を含むストラテジー名は、ストレス用のストラテジーを示す。
//	（ADAP側ストラテジー作成者とのネーミングルールで運用している。）
//	 ストレス用ストラテジーは、膜が1層（Thick1）で定義されているはずであり、
//	 実際が多層膜であっても、1層にまとめて定義された、特別なストラテジーが作成されている。
	LPCTSTR pszSTR = _T("(STR)");
	if ( strstr(pszStrategyEntry, pszSTR) ) {
		bSr = FALSE;
		bSe = FALSE;
		bStress = TRUE;
		return;
	}
// 2009.12.15 K.Matsuo Stress Type <--

	CXmpHelper xmpHelper;

	if ( !xmpHelper.LoadNMD(pszStrategyEntry) ) {
		bSr = FALSE;
		bSe = FALSE;
		bStress = FALSE;
		return;
	}

	bUsesReflectometer = (xmpHelper.m_nmdParams.nUseReflectData > 0);
	bUsesEllipsometer = (xmpHelper.m_nmdParams.nNoM44_Data == 0);

	bSr = bUsesReflectometer;
	bSe = bUsesEllipsometer;
	bStress = FALSE;
}

// --------------------------------------------------------------------
// MEAS_GetAdapScriptFileDescriptDdeTextItem
BOOL MEASYSAPI MEAS_GetAdapScriptFileDescriptDdeTextItem(LPTSTR pszDdeTextItem, int& iStartWaveLen, int& iEndWaveLen, int& iColorFilter, LPCTSTR pszStrategyEntry)
{
	TRACE(_T("MEAS_GetAdapScriptFileDescriptDdeTextItem()\n"));

	CXmpHelper xmpHelper;
	return xmpHelper.GetScriptFileDescriptDdeTextItem(pszDdeTextItem, iStartWaveLen, iEndWaveLen, iColorFilter, pszStrategyEntry);
}


// --------------------------------------------------------------------
// MEAS_SeHead_OpenLampShutter
BOOL MEASYSAPI MEAS_SeHead_OpenLampShutter(void)
{
	return pSeMeasure->OpenShutter();
}

// --------------------------------------------------------------------
// MEAS_SeHead_CloseLampShutter
BOOL MEASYSAPI MEAS_SeHead_CloseLampShutter(void)
{
	return pSeMeasure->CloseShutter();
}

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// --------------------------------------------------------------------
// MEAS_CompEASEHead_IsStatus
int MEAS_CompEASEHead_IsStatus(void)
{
	if ( pCompEASEMeasure == NULL ) {
		return EASE_STATUS_CHECK_PASS;
	}

	return pCompEASEMeasure->Status();
}

// --------------------------------------------------------------------
// MEAS_CompEASEHead_ListRecipes
BOOL MEASYSAPI MEAS_CompEASEHead_ListRecipes(CStringArray** ppListRecipes)
{
	*ppListRecipes = new CStringArray();

	if ( *ppListRecipes == NULL ) {
		return FALSE;
	}

	return pCompEASEMeasure->ListRecipes(*ppListRecipes);
}

// --------------------------------------------------------------------
// MEAS_CompEASEHead_OpenLampShutter
BOOL MEASYSAPI MEAS_CompEASEHead_OpenLampShutter(void)
{
	return pCompEASEMeasure->OpenShutter();
}

// --------------------------------------------------------------------
// MEAS_CompEASEHead_CloseLampShutter
BOOL MEASYSAPI MEAS_CompEASEHead_CloseLampShutter(void)
{
	return pCompEASEMeasure->CloseShutter();
}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// --------------------------------------------------------------------
// MEAS_PrepareForStressMeasure
//Saiki 20110307 Change ----->
//BOOL MEAS_PrepareForStressMeasure(int iMode, LPCTSTR pszSampleId, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr, const STAGE_PROG_STRESS* pStageProgStress)	// iMode = 0:Reference (First), 1:Normal (Single)
BOOL MEAS_PrepareForStressMeasure(int iMode, LPCTSTR pszSampleId, const MAIN_RCP_INFO* pMainRcpInfo, const MEAS_PROG_INFO* pMeasProgInfo, const STAGE_PROG_INFO_HDR* pStageProgInfoHdr, const STAGE_PROG_STRESS* pStageProgStress, const STRESS_PARAM_INFO* pStressParamInfo)	// iMode = 0:Reference (First), 1:Normal (Single)
//Saiki 20110307 Change <-----
{
	::ZeroMemory(&g_prepareData, sizeof(g_prepareData));

	g_prepareData.iStressMode		= iMode;
	g_prepareData.pszSampleId		= pszSampleId;
	g_prepareData.pMainRcpInfo		= pMainRcpInfo;
	g_prepareData.pMeasProgInfo 	= pMeasProgInfo;
	g_prepareData.pStageProgInfoHdr = pStageProgInfoHdr;
	g_prepareData.pStageProgStress	= pStageProgStress;
	//Saiki 20110307 Add ----->
	g_prepareData.pStressParamInfo	= pStressParamInfo;
	//Saiki 20110307 Add <-----

	::ZeroMemory(smStressResultDataBase.GetSharedMemoryPtr(), sizeof(STRESSRESULT));
	::ZeroMemory(thicknessForStress, sizeof(thicknessForStress));
	for ( int i = 0; i < STRESS_LINES_MAX; i++ ) {
		thicknessForStress[i].bValidLine = g_prepareData.pStageProgStress->Line[i].bValidLine;
		if ( thicknessForStress[i].bValidLine ) {
			for ( int j = 0; j < STRESS_SECTIONS_MAX; j++ ) {
				if ( !g_prepareData.pStageProgStress->Line[i].bScanValid[j] )
					continue;
				thicknessForStress[i].lScanPosY = g_prepareData.pStageProgStress->Line[i].SectPos[j].lScanPosY;
				break;
			}
		}
	}

	//Saiki 20110314 Add ----->
	int iCol, j;
	STRESS_CONFIG	StressConfig;
	memset(&StressConfig, 0, sizeof(StressConfig));

	if ( g_prepareData.pMeasProgInfo->ScanParams.hdr.wHeadType == HEAD_TYPE_STRESS && g_prepareData.pStressParamInfo->bPriority == TRUE) {
		ConfigFile_GetNanoSpecIni(&StressConfig, CONFIG_FILE_STRESS_CONFIG);
		for(iCol = 0; iCol < PIN_INTERVAL; iCol++){
			for (j = 0; j < STRESS_LINES_MAX; j++ ) {
				if ( thicknessForStress[j].bValidLine == TRUE/* &&
					 thicknessForStress[j].lScanPosY == StressConfig.Line[j].MeasPos[i].lY*/) {
					thicknessForStress[j].cf.point[thicknessForStress[j].cf.datanum] = ((double)StressConfig.Line[j].MeasPos[iCol].lX) / MICROMETRE;
					thicknessForStress[j].cf.value[thicknessForStress[j].cf.datanum] = g_prepareData.pStressParamInfo->Line[j].dPtThick[iCol];
					thicknessForStress[j].cf.datanum++;
				}
			}
		}
	}
	//Saiki 20110314 Add <-----

	// ここでメモリに書き戻す
	if ( g_prepareData.iStressMode == 1 ) {
		if ( !ReadReferenceData(g_prepareData.pszSampleId) )
			return FALSE;
	}

	return TRUE;
}

// --------------------------------------------------------------------
// ReadReferenceData
BOOL MEASYSAPI ReadReferenceData(LPCTSTR pszSampleId)
{
//[C:\NanoSpec\RESULT\サンプルID 名\%StressReference%\StressRefRawData.dat]
//[MainRecipe]
//Name=SrHeadDesktop
//[ValidScanLine]
//Line1ValidScan=TRUE
//Line2ValidScan=FALSE
//Line3ValidScan=TRUE
//Line4ValidScan=TRUE
//Line5ValidScan=TRUE
//[Line1]
//ScanPosition[um] Height[um]
//-1240000 147456
//-1230000 147623
// ...

	char szBuff[256];
	char szFilePath[MAX_PATH];
	BOOL bLineValidScan[STRESS_LINES_MAX];
	CString strBuffer;
	char szLineKeyName[256];
	char* token;
	LPCTSTR seps = " \t\n";
	int iCnt;

	const LPCTSTR pszSectionName = "ValidScanLine";

//	sprintf(szFilePath, "%s%s%s%s%s%s", g_tszBaseDir, "RESULT\\", pszSampleId, "\\", STRESS_REFERENCE_NAME, "\\StressRefRawData.dat");
	sprintf(szFilePath, "%s%s%s%s", g_tszBaseDir, "DATA\\REF\\StressRef\\", pszSampleId, DAT_EXT);

	for ( int iLine = 0; iLine < STRESS_LINES_MAX; iLine++ ) {
		sprintf(szLineKeyName, "Line%dValidScan", iLine + 1);
		GetPrivateProfileString(pszSectionName, szLineKeyName, "FALSE", szBuff, sizeof(szBuff), szFilePath);
		bLineValidScan[iLine] = (strcmp(szBuff, "TRUE") == 0);
	}

//	double dDeflArray[DEFL_CNT_MAX][2];
	double** dDeflArray = new double*[DEFL_CNT_MAX];
	for ( int i = 0; i < DEFL_CNT_MAX; i++ )
		dDeflArray[i] = new double[2];

	TRY
	{
		CStdioFile stdioFile(szFilePath, CFile::modeRead);
		for ( int iLine = 0; iLine < STRESS_LINES_MAX; iLine++ ) {
			if ( !bLineValidScan[iLine] ) {
				continue;
			}

			for ( int i = 0; i < DEFL_CNT_MAX; i++ )
				::ZeroMemory(dDeflArray[i], sizeof(double) * 2);

			sprintf(szLineKeyName, "[Line%d]", iLine + 1);
			stdioFile.SeekToBegin();
			while ( stdioFile.ReadString(strBuffer) ) {
				// LineKeyName Found!?
				if ( strBuffer == szLineKeyName) {
					iCnt = 0;
					stdioFile.ReadString(strBuffer);			// skip line	//ScanPosition[um] Height[um]
					while ( stdioFile.ReadString(strBuffer) ) {
						if ( strBuffer.IsEmpty() )
							break;
						if ( strBuffer[0] == '[' )
							break;
						strcpy(szBuff, strBuffer);
						token = strtok(szBuff, seps);
						dDeflArray[iCnt][0] = atof(token);
						token = strtok(NULL, seps);
						dDeflArray[iCnt][1] = atof(token);
						iCnt++;
					}
					pStressAnalysis->SetDeflectionAtZero(0, iLine, dDeflArray, iCnt);
					break;
				}
			}
		}
		stdioFile.Close();
	}
	CATCH(CFileException, e)
	{
		CString strCause;
		strCause.Format(pMojiretsu->LoadString(IDS_CAUSE_CODE), e->m_cause, e->m_strFileName);

		switch ( e->m_cause )
		{
		case CFileException::fileNotFound :
			strCause += pMojiretsu->LoadString(IDS_FILE_NOT_FOUND);
			break;
		case CFileException::badPath :
			strCause += pMojiretsu->LoadString(IDS_BAD_PATH);
			break;
		default :
			break;
		}

		MessageBox(NULL, strCause, NULL, MB_ICONERROR | MB_OK);

		return FALSE;
	}
	END_CATCH

	for ( i = 0; i < DEFL_CNT_MAX; i++ )
		delete[] dDeflArray[i];
	delete[] dDeflArray;

	return TRUE;
}

// --------------------------------------------------------------------
// MEAS_StressMeasure
BOOL MEASYSAPI MEAS_StressMeasure(int iLineNo)
{
	pStressMeasure->SetMeasProgInfoPtr(g_prepareData.pMeasProgInfo);
	pStressMeasure->SetStageProgStressPtr(g_prepareData.pStageProgStress);

	pStressAnalysis->SetMainRcpInfoPtr(g_prepareData.pMainRcpInfo);
	pStressAnalysis->SetMeasProgInfoPtr(g_prepareData.pMeasProgInfo);
	pStressAnalysis->SetStageProgInfoHdrPtr(g_prepareData.pStageProgInfoHdr);
	pStressAnalysis->SetStageProgStressPtr(g_prepareData.pStageProgStress);

	int iLineIndex = iLineNo - 1;
	pStressMeasure->Measure(g_prepareData.iStressMode, iLineIndex);

	return TRUE;
}

// --------------------------------------------------------------------
// MEAS_StressCancel
void MEASYSAPI MEAS_StressCancel()
{
	pStressMeasure->Cancel();
}

/* added 2009.10.23 hmenjo CTA API 定義追加 ---------- { ---------- */
/*
 *	CTA 測定モード設定
 *		MEAS_CTA_MODE_1 モード１：基板単位・オペレータデータ確認なし
 *		MEAS_CTA_MODE_2 モード２：基板単位・オペレータデータ確認あり
 *		MEAS_CTA_MODE_3 モード３：カセット単位・オペレータデータ確認あり
 */
BOOL MEASYSAPI MEAS_CtaModeSet(int iCtaMode)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	return lgs_pclsCCtaCtrl->Cta_ModeSet(iCtaMode);
}
/*
 *	CTA 基板厚み設定
 */
BOOL MEASYSAPI MEAS_CtaSetThick(
		DWORD dwThick			/* 基板厚み情報[0.1mm]，範囲(0.0～10.0mm)	*/
	)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	::ZeroMemory(lg_smCtaResultDataBase.GetSharedMemoryPtr(), sizeof(CTARESULT));	/* CTA 用共有エリア クリア	*/

	return lgs_pclsCCtaCtrl->Cta_SetThick(dwThick);
}
/*
 *	CTA 測定終了
 */
BOOL MEASYSAPI MEAS_CtaMeasEnd(void)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	return lgs_pclsCCtaCtrl->Cta_MeasEnd();
}
/*
 *	CTA 強制終了
 */
BOOL MEASYSAPI MEAS_CtaForceEnd(void)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	return lgs_pclsCCtaCtrl->Cta_ForceEnd();
}
/*
 *	CTA 初期化
 */
BOOL MEASYSAPI MEAS_CtaInitialize(void)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	return lgs_pclsCCtaCtrl->Cta_Initialize();
}
/*
 *	リキャリブのラッパ
 */
BOOL CtaRecalib(LPCTSTR szRecalib, double& dData)
{
	return MEAS_Recalib(szRecalib, dData);
}
/*
 *	CTA カセット ID，基板 ID 設定
 */
BOOL MEASYSAPI MEAS_CtaSetCstSample(
		LPCSTR pstrCstID,					/* カセット ID	：ASCII 80 文字 */
		LPCSTR pstrSampleID,				/* 基板 ID		：ASCII 80 文字 */
		const MAIN_RCP_INFO* pMainRcpInfo	/* メインレシピのアドレス	*/
	)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	return lgs_pclsCCtaCtrl->Cta_SetCstSample(pstrCstID, pstrSampleID, pMainRcpInfo);
}
/*
 *	CTA 状態取得
 */
BOOL MEASYSAPI MEAS_CtaGetStatus(long* plStatus)
{
	if (0 == lgs_pclsCCtaCtrl) {
		return FALSE;
	}

	return lgs_pclsCCtaCtrl->Cta_GetStatus(plStatus);
}
/* added 2009.10.23 hmenjo CTA API 定義追加 ---------- } ---------- */

#if 0 // 2014.04.04 bagus wavelength step modified -->
// //2010.01.11 bagus GTR --{--
// // 0: Dark
// // 1-MAX: Reference Data
// double MEASYSAPI MEAS_GetScanGTrReferenceData(int iRefPos,int iWave)
// {
// 	if ( iRefPos == 0){
// 		return scanData[iRefPos].dScanData[iWave - 1];
// 	}else if ( MEASYS_GTR_1 <= iRefPos && iRefPos <= MEASYS_GTR_LAST ) {
// 		return scanData[iRefPos].dScanData[iWave - 1];
// 	}
// 	return 0.0;
// }
// //2010.01.11 bagus GTR --}--
#else
// 0: Dark
// 1-MAX: Reference Data
double MEASYSAPI MEAS_GetScanGTrReferenceData(int iRefPos,double dWave)
{
	if ( iRefPos == 0){
		return scanData[iRefPos].GetScanData(dWave);
	}else if ( MEASYS_GTR_1 <= iRefPos && iRefPos <= MEASYS_GTR_LAST ) {
		return scanData[iRefPos].GetScanData(dWave);
	}
	return 0.0;
}
#endif // 2014.04.04 bagus wavelength step modified <--

/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- { ---------- */
BOOL MEASYSAPI MEAS_ShowXmp(int iShow)
{
	if (0 == pXmp) {
		return FALSE;
	}

	return pXmp->ShowXmp(iShow);
}
/* added 2014.11.25 hmenjo XMP 表示/非表示 ---------- } ---------- */

/* added 2016.02.24 hmenjo バッチ処理 ---------- { ---------- */
long MEASYSAPI MEAS_BatchFlagSet(long lBatchFlag)
{
	g_lBatchFlag = lBatchFlag;

	return g_lBatchFlag;
}
long MEASYSAPI MEAS_BatchFlagGet(void)
{
	return g_lBatchFlag;
}
/* added 2016.02.24 hmenjo バッチ処理 ---------- } ---------- */
