#include "stdafx.h"
#include <math.h>
#include "../../INC/Common.hxx"
#include "../../INC/ConfigFile.hxx"
#include "../../INC/SharedMemory.h"
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/MeaSys.hxx"
#include "LogFile.h"
#include "CompEASEMeasure.h"
#include "CompEASEPollingStatus.h"
#include "TcpAsyncPort.h"

extern CLogFile* pLogFile;
extern BOOL bHwSimulation;

extern TCHAR g_tszProcDir[_MAX_PATH];		/* ďovZX̃fBNg('\'t)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* fBNg('\'t)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
extern void AddAbsPath(LPTSTR ptszPath);
extern CSharedMemory<ADAPRESULTDATABASE> smAdapResultDataBase;	/* ADAP 茋 DB LGA	*/
extern CSharedMemory<COMPEASERESULT_HELPER> smCompEASEResultHelperDataBase;	/* CompleteEASE 茋 Helper DB LGA	*/

// --------------------------------------------------------------------------
// CCompEASEMeasure
CCompEASEMeasure::CCompEASEMeasure()
{
	m_pCompEASEHead = NULL;
	m_pCompEASEPolling = NULL;
	m_pTcpAsyncPort = NULL;
	::ZeroMemory(m_szListRecipBuff, sizeof(m_szListRecipBuff));
}

// --------------------------------------------------------------------------
// ~CCompEASEMeasure
CCompEASEMeasure::~CCompEASEMeasure()
{
}

// --------------------------------------------------------------------------
// InitInstance
BOOL CCompEASEMeasure::InitInstance()
{
	TRACE(_T("CCompEASEMeasure::InitInstance()\n"));

	// CompEASEwbh̃C^tF[XI
	if ( bHwSimulation ) {
//		m_pCompEASEHead = new CCompEASEHeadDesktop();
//		pLogFile->Logging("CompEASE Head Type : CCompEASEHeadDesktop");
		m_pCompEASEHead = new CCompEASEHeadSock();
		pLogFile->Logging("CompEASE Head Type : CCompEASEHeadSock");
	}
	else {
		m_pCompEASEHead = new CCompEASEHeadSock();
		pLogFile->Logging("CompEASE Head Type : CCompEASEHeadSock");
	}

	// CompEASEwbh̏
	if ( !m_pCompEASEHead->InitInstance() ) {
		pLogFile->Logging("CompEASE Head Initialize Error");
		pLogFile->Logging(m_pCompEASEHead->GetLastError());
		return FALSE;
	}

	///// CompEASEPollingStatus Thread /////
	m_pCompEASEPolling = (CCompEASEPollingStatus*)AfxBeginThread(
		RUNTIME_CLASS(CCompEASEPollingStatus),
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED,
		NULL);
	m_pCompEASEPolling->ResumeThread();
	pLogFile->Logging("CompEASE : CompEASE Polling Status Start.");

	// TCP񓯊|[g
	///// Tcp Async Port Thread /////
	m_pTcpAsyncPort = (CTcpAsyncPort*)AfxBeginThread(
		RUNTIME_CLASS(CTcpAsyncPort),
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED,
		NULL);
	m_pTcpAsyncPort->ResumeThread();
	pLogFile->Logging("CompEASE : Tcp Async Port Thread Start.");

	return TRUE;
}

// --------------------------------------------------------------------------
// ExitInstance
void CCompEASEMeasure::ExitInstance()
{
	TRACE(_T("CCompEASEMeasure::ExitInstance()\n"));

	CTimer timer;
	const int iTimeoutSec = 2;
	DWORD dwStatus = 0;

	///// CompEASEPollingStatus Thread /////
	if(m_pCompEASEPolling){
		m_pCompEASEPolling->Quit();
	}

	///// Tcp Async Port Thread /////
	if(m_pTcpAsyncPort){
		m_pTcpAsyncPort->Quit();
	}

	///// CompEASEPollingStatus Thread /////
	if(m_pCompEASEPolling){
		timer.Restart(iTimeoutSec);
		while ( 1 ) {
			if ( timer.IsTimeout() ) {
				break;
			}
			GetExitCodeThread(m_pCompEASEPolling->m_hThread, &dwStatus);
			if(dwStatus != STILL_ACTIVE){
				delete m_pCompEASEPolling;
				m_pCompEASEPolling = NULL;
				break;
			}
			::Sleep(10);
		}
	}

	///// Tcp Async Port Thread /////
	if(m_pTcpAsyncPort){
		timer.Restart(iTimeoutSec);
		while ( 1 ) {
			if ( timer.IsTimeout() ) {
				break;
			}
			GetExitCodeThread(m_pTcpAsyncPort->m_hThread, &dwStatus);
			if(dwStatus != STILL_ACTIVE){
				delete m_pTcpAsyncPort;
				m_pTcpAsyncPort = NULL;
				break;
			}
			::Sleep(10);
		}
	}

	if ( m_pCompEASEHead ) {
		m_pCompEASEHead->ExitInstance();
		delete m_pCompEASEHead;
		m_pCompEASEHead = NULL;
	}
}

// --------------------------------------------------------------------------
// ListRecipes
BOOL CCompEASEMeasure::ListRecipes(CStringArray* pListRecipes)
{
	TRACE(_T("CCompEASEMeasure::ListRecipes()\n"));

	BOOL bRet;
	CSingleLock lock(&m_CriticalSection, TRUE);

	if ( pListRecipes == NULL ) {
		return FALSE;
	}

	pListRecipes->RemoveAll();
	::ZeroMemory(m_szListRecipBuff, sizeof(m_szListRecipBuff));
	bRet = m_pCompEASEHead->ListRecipes(m_szListRecipBuff);
	if ( bRet == FALSE ) {
		return FALSE;
	}

	Split(m_szListRecipBuff, '/', pListRecipes);

	return TRUE;
}

// --------------------------------------------------------------------------
// Measure
BOOL CCompEASEMeasure::Measure(DWORD dwPointNo, LPCTSTR pszSampleId, const MEAS_PROG_INFO* pMeasProgInfo, const MAIN_RCP_INFO* pMainRcpInfo)
{
	TRACE(_T("CCompEASEMeasure::Measure()\n"));

	TCHAR szBuff[1024];
	TCHAR szDate[32];
	TCHAR szTime[32];
	TCHAR szCompEASERecipename[256];
	TCHAR szSaveFilename[256];
	TCHAR szMeasResult[1024 * 4];
	BOOL bUseSampleId;

	CSingleLock lock(&m_CriticalSection, TRUE);
	SYSTEMTIME systime;
	::GetLocalTime(&systime);

	if(pszSampleId == NULL || _tcslen(pszSampleId) <= 0){
		bUseSampleId = FALSE;
	}
	else{
		bUseSampleId = TRUE;
	}

	_stprintf(szDate, _T("%04d%02d%02d"), systime.wYear, systime.wMonth, systime.wDay);
	_stprintf(szTime, _T("%02d%02d%02d"), systime.wHour, systime.wMinute, systime.wSecond);
	_tsplitpath(pMeasProgInfo->ScanParams._COMPEASE.szCompEASERecipe, NULL, NULL, szCompEASERecipename, NULL);

	if ( pMeasProgInfo->ScanParams._COMPEASE.bAutoSaveResultInCompEASE ) {
		_stprintf(szSaveFilename, "%s%s%s_P%05d_%s_%s",
				  pszSampleId,
				  ( bUseSampleId ? "_" : "" ),
				  pMeasProgInfo->hdr.szName,
				  dwPointNo,
				  szDate,
				  szTime);
		// e.g. "TestRecipe1/SampleId_TestEASE_P00001_20130208_121212"
		_stprintf(szBuff, "%s/%s",
				  szCompEASERecipename,
				  szSaveFilename);
	}
	else {
		_stprintf(szBuff, szCompEASERecipename);
	}

	if ( !m_pCompEASEHead->RunRecipe(szBuff, szMeasResult) ) {
		return FALSE;
	}

	// 茋ʃf[^
	if ( !SetToAdapResultDataBase(dwPointNo, szMeasResult, pMainRcpInfo) ) {
		return FALSE;
	}

	return TRUE;
}

// --------------------------------------------------------------------------
// SetResults
BOOL CCompEASEMeasure::SetToAdapResultDataBase(DWORD dwPointNo, LPCTSTR pszMeasResult, const MAIN_RCP_INFO* pMainRcpInfo)
{
	TRACE(_T("CCompEASEMeasure::SetResults()\n"));

	TCHAR szBuff[1024];
	CStringArray strResults;
	CStringArray strData;
	CString strKey;
	CString strValue;
	int iIndex;

	// CleanUp
	int iCol;

	for ( iCol = 0; iCol < ADAPRESULT_COLS_MAX; iCol++ ) {
		strcpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iCol], "");
		strcpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iCol], "");
		smAdapResultDataBase.GetSharedMemoryPtr()->dData[iCol] = 0.0;
	}
	int i;

	for ( i = 0; i < 100; i++ ) {
		smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->bItemEnable[i] = FALSE;
	}
	smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->iItemEnableCount = 0;

	int iDispLabelCount = 0;

	// Meas Item Thickness
	for ( i = 0; i < 8; i++ ) {
		_stprintf(szBuff, "Thick%d", i + 1);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iDispLabelCount], szBuff);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iDispLabelCount], szBuff);
		iDispLabelCount++;
	}
	// Meas Item N
	for ( i = 0; i < 8; i++ ) {
		_stprintf(szBuff, "N%d", i + 1);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iDispLabelCount], szBuff);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iDispLabelCount], szBuff);
		iDispLabelCount++;
	}
	// Meas Item K
	for ( i = 0; i < 8; i++ ) {
		_stprintf(szBuff, "K%d", i + 1);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iDispLabelCount], szBuff);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iDispLabelCount], szBuff);
		iDispLabelCount++;
	}
	// Meas Item Eg
	for ( i = 0; i < 8; i++ ) {
		_stprintf(szBuff, "Eg%d", i + 1);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iDispLabelCount], szBuff);
		_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iDispLabelCount], szBuff);
		iDispLabelCount++;
	}
	// Meas Item MSE
	strcpy(szBuff, "MSE");
	_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iDispLabelCount], szBuff);
	_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iDispLabelCount], szBuff);
	iDispLabelCount++;

	Split(pszMeasResult, ',', &strResults);

	int iResult;


	for ( iResult = 0; iResult < strResults.GetSize(); iResult++ ) {
		// Split key and value.
		strData.RemoveAll();
		Split(strResults[iResult], '=', &strData);
		if ( strData.GetSize() != 2 ) {
			pLogFile->Logging("Illegal Format.");
			return FALSE;
		}
		strKey = strData[0];
		strKey.TrimLeft();
		strKey.TrimRight();
		strValue = strData[1];
		strValue.TrimLeft();
		strValue.TrimRight();

		// Target Item !?
		iIndex = IsTargetItem(strKey);
		if ( iIndex < 0 ) {
			continue;
		}

		// Check if data is already set, skip set data.
		if ( smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->bItemEnable[iIndex] ) {
			continue;
		}

		// Set Label (N, K, Eg)
		if ( 8 <= iIndex && iIndex <= 31 ) {
			_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabel[iIndex], strKey);
			_tcscpy(smAdapResultDataBase.GetSharedMemoryPtr()->szLabelOri[iIndex], strKey);
		}

		// Set Label Enable Info
		smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->bItemEnable[iIndex] = TRUE;
		smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->iDispIndex[iIndex]
			= smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->iItemEnableCount;
		smCompEASEResultHelperDataBase.GetSharedMemoryPtr()->iItemEnableCount++;

		// Check MSE OOS
		if( strKey == "MSE" ) {
			if ( strValue.Find("OOS") >= 0 ) {
				pLogFile->LoggingV("P%05d : Measured Data is Out of Spec", dwPointNo);
			}
		}

		// Data
		smAdapResultDataBase.GetSharedMemoryPtr()->dData[iIndex] = atof(strValue);
		int i;

		for ( i = 0; i < RECALIB_MAX; i++ ) {
			if ( pMainRcpInfo->MainRcpParam._COMPEASE.RecalibItem[i] == 0 ) 		// RecalibItemindex 0́Askip
				continue;
			if ( iIndex == (pMainRcpInfo->MainRcpParam._COMPEASE.RecalibItem[i] - 1) ) {	// RecalibItemindex́A1n܂邽߁A1Z
				_tcscpy(szBuff, pMainRcpInfo->MainRcpParam._COMPEASE.szRecalib[i]);
				if ( !MEAS_Recalib(szBuff, smAdapResultDataBase.GetSharedMemoryPtr()->dData[iIndex]) )
					return FALSE;
				break;							// Lu[V
			}
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// IsTargetItem
//  >=0 : target
//  <0 : not target
int CCompEASEMeasure::IsTargetItem(LPCTSTR pszKey)
{
	const int iCount = 33;
	const LPCSTR pszTargetItem[iCount] = {
		"Thickness # 1","Thickness # 2","Thickness # 3","Thickness # 4","Thickness # 5","Thickness # 6","Thickness # 7","Thickness # 8",
		"N1",			"N2",			 "N3",			  "N4",		   "N5",			"N6",			 "N7",			"N8",
		"K1",			"K2",			 "K3",			  "K4",		   "K5",			"K6",			 "K7",			"K8",
		"Eg1",			"Eg2",			 "Eg3",			  "Eg4",	   "Eg5",			"Eg6",			 "Eg7",			"Eg8",
		"MSE",
	};
	int iColumnIndexInfo[iCount];
	int iIndex;

	///// JCfbNX̍쐬
	iIndex = 0;
	int i;

	for ( i = 0; i < iCount; i++ ) {
		iColumnIndexInfo[i] = i;
	}

	CString strBuffer;
	int iFoundPos;

	strBuffer = pszKey;
	iFoundPos = strBuffer.Find('[');
	if ( iFoundPos >= 0 ) {
		strBuffer = strBuffer.Left(iFoundPos);
	}
	for ( i = 0; i < iCount; i++ ) {
		if ( strBuffer == pszTargetItem[i] ) {
			return iColumnIndexInfo[i];
		}
	}

	return -1; // not target item.
}

//---------------------------------------------------------------------------
// Status
int CCompEASEMeasure::Status()
{
	CSingleLock lock(&m_CriticalSection);

	if ( !lock.IsLocked() ) {
		lock.Lock(INFINITE);
	}
	else {
		return EASE_STATUS_CHECK_PASS;
	}

	if ( m_pCompEASEHead == NULL ) {
		return EASE_STATUS_UNKNOWN;
	}

	return m_pCompEASEHead->Status();
}

//---------------------------------------------------------------------------
// OpenShutter
BOOL CCompEASEMeasure::OpenShutter()
{
	CSingleLock lock(&m_CriticalSection, TRUE);

	return m_pCompEASEHead->OpenShutter();
}

//---------------------------------------------------------------------------
// CloseShutter
BOOL CCompEASEMeasure::CloseShutter()
{
	CSingleLock lock(&m_CriticalSection, TRUE);

	return m_pCompEASEHead->CloseShutter();
}

//---------------------------------------------------------------------------
// Split
void CCompEASEMeasure::Split(CString strRecord, TCHAR chDel, CStringArray* pStrTsvDatas)
{
	int iStartIdx = 0;
	CString strBuffer;

	// split by delimiter.
	int iPos;

	for ( iPos = 0; iPos < strRecord.GetLength(); iPos++ ) {
		if( strRecord[iPos] == chDel ) {
			strBuffer = strRecord.Mid(iStartIdx, iPos - iStartIdx);
			pStrTsvDatas->Add(strBuffer);
			iStartIdx = iPos + 1;
		}
	}

	// last value.
	strBuffer = strRecord.Mid(iStartIdx, iPos - iStartIdx);
	pStrTsvDatas->Add(strBuffer);
}
