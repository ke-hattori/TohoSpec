#include "stdafx.h"
#include <math.h>
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "LogFile.h"
#include "XmpHelper.hxx"
#include "SeMeasure.h"

extern CLogFile* pLogFile;
extern BOOL bHwSimulation;
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];		/* 呼出しプロセスのディレクトリ('\'付き)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* 基準ディレクトリ('\'付き)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
extern void AddAbsPath(LPTSTR ptszPath);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */

// --------------------------------------------------------------------------
// CSeMeasure
CSeMeasure::CSeMeasure()
{
	m_pSeHead = NULL;
}

// --------------------------------------------------------------------------
// ~CSeMeasure
CSeMeasure::~CSeMeasure()
{
}

// --------------------------------------------------------------------------
// InitInstance
BOOL CSeMeasure::InitInstance()
{
	TRACE(_T("CSeMeasure::InitInstance()\n"));

	char szVersion[256];

	// SEヘッドのインタフェースを選択する
	if ( bHwSimulation ) {
		m_pSeHead = new CSeHeadDesktop();
		pLogFile->Logging("Se Head Type : CSeHeadDesktop");
	}
	else {
		m_pSeHead = new CSeHeadSock();
		pLogFile->Logging("Se Head Type : CSeHeadSock");
	}

	// SEヘッドの初期化
	if ( !m_pSeHead->InitInstance() ) {
		pLogFile->Logging("Se Head Initialize Error");
		pLogFile->Logging(m_pSeHead->GetLastError());
		return FALSE;
	}

	// SEヘッド（=WVASE32）のバージョン情報の取得
	if ( m_pSeHead->GetVersion(szVersion) ) {
		pLogFile->LoggingV("%s%s", "Se Head Version : ", szVersion);
	}

	return TRUE;
}

// --------------------------------------------------------------------------
// ExitInstance
void CSeMeasure::ExitInstance()
{
	TRACE(_T("CSeMeasure::ExitInstance()\n"));

	if ( m_pSeHead ) {
		m_pSeHead->ExitInstance();
		delete m_pSeHead;
		m_pSeHead = NULL;
	}
}

// --------------------------------------------------------------------------
// Measure
BOOL CSeMeasure::Measure(const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CSeMeasure::Measure()\n"));

	SE_XMP seXmp;
	CXmpHelper xmpHelper;

	ConfigFile_GetNanoSpecIni(&seXmp, CONFIG_FILE_SE_XMP);

	if ( !xmpHelper.LoadNMD(pMeasProgInfo->ScanParams._SE.szStrategyEntry) ) {
		pLogFile->LoggingV("SE Measure LoadNMD File Read Error : %s", pMeasProgInfo->ScanParams._SE.szStrategyEntry);
		return FALSE;
	}

	pLogFile->LoggingV("SE Measure NumScans : %d, HighAccuracy : %d", xmpHelper.m_nmdParams.nCycles, xmpHelper.m_nmdParams.bHighAccuracy);

	return m_pSeHead->Scan(seXmp.szDataPath, xmpHelper.m_nmdParams.nCycles, xmpHelper.m_nmdParams.bHighAccuracy);
}

//---------------------------------------------------------------------------
// OpenShutter
BOOL CSeMeasure::OpenShutter()
{
	return m_pSeHead->OpenShutter();
}

//---------------------------------------------------------------------------
// CloseShutter
BOOL CSeMeasure::CloseShutter()
{
	return m_pSeHead->CloseShutter();
}
