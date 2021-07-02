#include "stdafx.h"
#include "../../INC/Globals.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/DdeClient.hxx"
#include "SeAnalysis.h"
#include "resource.h"
#include "LogFile.h"
#include "Mojiretsu.h"
#include "Xmp.h"

extern CXmp* pXmp;
extern CLogFile* pLogFile;
extern CMojiretsu* pMojiretsu;

// --------------------------------------------------------------------
// CSeAnalysis
CSeAnalysis::CSeAnalysis()
{
}

// --------------------------------------------------------------------
// ~CSeAnalysis
CSeAnalysis::~CSeAnalysis()
{
}

// --------------------------------------------------------------------
// InitInstance
BOOL CSeAnalysis::InitInstance()
{
	TRACE(_T("CSeAnalysis::InitInstance()\n"));

	return TRUE;
}

// --------------------------------------------------------------------
// ExitInstance
void CSeAnalysis::ExitInstance()
{
	TRACE(_T("CSeAnalysis::ExitInstance()\n"));
}

// --------------------------------------------------------------------
// DoXmp
BOOL CSeAnalysis::DoXmp(LPTSTR pszXmpDdeText, const MEAS_PROG_INFO* pMeasProgInfo, CDdeClient* pXmpDde)
{
	TRACE(_T("CSeAnalysis::DoXmp()\n"));

	TCHAR szBuff[1024];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFilename[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	SE_XMP seXmp;

	ConfigFile_GetNanoSpecIni(&seXmp, CONFIG_FILE_SE_XMP);
	_splitpath(seXmp.szDataPath, szDrive, szDir, szFilename, szExt);

	// FITDATAコマンド送信
	_stprintf(szBuff, _T("FitData(StratNum=%02d,DataPath=%s%s,SEDataFile=%s%s)"),
			atoi(pMeasProgInfo->ScanParams._SE.szStrategyEntry), szDrive, szDir, szFilename, szExt);

	// DoXmp
	if ( !pXmp->DoXmp(pszXmpDdeText, szBuff, pXmpDde) )
		return FALSE;

	pLogFile->LoggingV("SE Analysis : %s", pszXmpDdeText);

	return TRUE;
}

