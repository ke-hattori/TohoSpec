// CMainFrmMailRecvThread.cpp : implementation file
//

#include "stdafx.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\PifComm.hxx"
#include "..\\..\\INC\\RecipeFile.hxx"
#include "..\\..\\INC\\Measys.hxx"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "InterlockDlg.h"
#include "MainFrmMailRecvThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrmMailRecvThread

IMPLEMENT_DYNCREATE(CMainFrmMailRecvThread, CWinThread)

CMainFrmMailRecvThread::CMainFrmMailRecvThread()
{
}

CMainFrmMailRecvThread::~CMainFrmMailRecvThread()
{
}

BOOL CMainFrmMailRecvThread::InitInstance()
{
	TRACE(_T("CMainFrmMailRecvThread::InitInstance()\n"));

	m_bShutDown = FALSE;
	m_bAutoDelete = FALSE;
	m_mailslot.Create(MAILSLOT_MAINFRM);

	return TRUE;
}

int CMainFrmMailRecvThread::ExitInstance()
{
	TRACE(_T("CMainFrmMailRecvThread::ExitInstance()\n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMainFrmMailRecvThread, CWinThread)
	//{{AFX_MSG_MAP(CMainFrmMailRecvThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrmMailRecvThread message handlers
int CMainFrmMailRecvThread::Run()
{
	TRACE(_T("CMainFrmMailRecvThread::Run()\n"));

	DWORD nNumberOfBytesRead;

	const int nMessageMaxLen = 256;
	TCHAR szRecvBuff[nMessageMaxLen];

	::CopyMemory(&m_eqMonitor, PifComm_GetEqMonitorPtr(), sizeof(EQMONITORSET));
	::CopyMemory(&m_eqMonitorOld, &m_eqMonitor, sizeof(EQMONITORSET));

	while ( !m_bShutDown )
	{
		if ( m_mailslot.Recieve(szRecvBuff, &nNumberOfBytesRead) ) {
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
			if ( _tcscmp(szRecvBuff, PIFCOMM_P105) == 0 )
				EventProcessP105();
			else if ( _tcscmp(szRecvBuff, PIFCOMM_P106) == 0 )
				EventProcessP106();
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
// 2009.12.21 K.Matsuo -->
			else if ( _tcscmp(szRecvBuff, PIFCOMM_P302) == 0 )
				EventProcessP302();
// 2009.12.21 K.Matsuo <--
			else if ( _tcscmp(szRecvBuff, PIFCOMM_P303) == 0 )
				EventProcessP303();
			else if ( _tcscmp(szRecvBuff, PIFCOMM_P306) == 0 )
				EventProcessP306();
			else if ( _tcscmp(szRecvBuff, PIFCOMM_P308) == 0 )
				EventProcessP308();
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
			else if ( _tcscmp(szRecvBuff, PIFCOMM_P310) == 0 )
				EventProcessP310();
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--
			else
				ASSERT( FALSE );
		}

		EventProcessP301();

		::Sleep(10);
	}

	::PostQuitMessage(0);

	return CWinThread::Run();
}

void CMainFrmMailRecvThread::ShutDown()
{
	m_bShutDown = TRUE;
}

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
//---------------------------------------------------------------------------
// EventProcessP105
void CMainFrmMailRecvThread::EventProcessP105()
{
	BOOL bRet = CallLock(TRUE);		// Lock UI
	if ( bRet ) {
		PifComm_DoStateDone(0);
		m_pInterlockDlg->CenterWindow();
		m_pInterlockDlg->ShowWindow(SW_SHOWNORMAL);
	}
	else {
		PifComm_DoStateDone(1);
	}
}

//---------------------------------------------------------------------------
// EventProcessP106
void CMainFrmMailRecvThread::EventProcessP106()
{
	BOOL bRet = CallLock(FALSE);	// Unlock UI
	if ( bRet ) {
		PifComm_DoStateDone(0);
	}
	else {
		PifComm_DoStateDone(1);
	}
	m_pInterlockDlg->ShowWindow(SW_HIDE);
}
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--

//---------------------------------------------------------------------------
// EventProcessP301
void CMainFrmMailRecvThread::EventProcessP301()
{
	PifComm_GetEqMonitorPtr()->iLocalRemote = m_pDoc->GetHostMode();
	PifComm_GetEqMonitorPtr()->iEqProcessStatus = m_pDoc->GetProcessStatus();
	if ( m_pDoc->GetAlarmStatus() )
		PifComm_GetEqMonitorPtr()->iAlarmStatus = 2;
	else if ( m_pDoc->GetWarningStatus() )
		PifComm_GetEqMonitorPtr()->iAlarmStatus = 1;
	else
		PifComm_GetEqMonitorPtr()->iAlarmStatus = 0;

// 2009.12.21 K.Matsuo -->
	::CopyMemory(&m_eqMonitor, PifComm_GetEqMonitorPtr(), sizeof(EQMONITORSET));

	if ( memcmp(&m_eqMonitorOld, &m_eqMonitor, sizeof(EQMONITORSET)) != 0 )
		PifComm_EqMonitortReport();

	::CopyMemory(&m_eqMonitorOld, &m_eqMonitor, sizeof(EQMONITORSET));
// 2009.12.21 K.Matsuo <--
}

// 2009.12.21 K.Matsuo -->
//---------------------------------------------------------------------------
// EventProcessP302
void CMainFrmMailRecvThread::EventProcessP302()
{
	PifComm_DoStateDone(0);

	PifComm_EqMonitortReport();
}
// 2009.12.21 K.Matsuo <--

//---------------------------------------------------------------------------
// EventProcessP303
void CMainFrmMailRecvThread::EventProcessP303()
{
	int iKindOfRecipe;
	iKindOfRecipe =	PifComm_RequestKindOfRecipe();
	PifComm_DoStateDone(0);

	// レシピ報告一覧作成
	RecipeFile_Lock();

	CString strBuffer;
	CFileFind Finder;
	TCHAR szDirPath[_MAX_PATH];
	BOOL bLoop;
	CTime time;
/* modified 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
//	RECIPENAMEDATESET recipeDataSet[10000];
/* modified 2016.08.11 hmenjo 最大レシピ数制限 ----------              */
	RECIPENAMEDATESET recipeDataSet[RCP_PGM_NUM_MAX];
/* modified 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */
	UINT nOccurence = 0;

	_stprintf(szDirPath, _T("%s*%s"), g_szDb_Main_Recipe_Dir, MAINRECIPE_EXT);
	bLoop = Finder.FindFile(szDirPath);
	while ( bLoop ) {
		bLoop = Finder.FindNextFile();
		if ( Finder.IsDots() )
			continue;
		// ファイル名を取得
		strBuffer = Finder.GetFileName();
		if ( strBuffer[0] == SYSTEM_RECIPE_BEGINNING_CHAR )
			continue;
		_tcscpy(recipeDataSet[nOccurence].szRecipeNameExt, strBuffer);
		// 最終変更日時を取得
		Finder.GetLastWriteTime(time);
		time.GetAsSystemTime(recipeDataSet[nOccurence].recipeDateTime);
		nOccurence++;
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- { ---------- */
		if (RCP_PGM_NUM_MAX <= nOccurence) {
			break;
		}
/* added 2016.08.11 hmenjo 最大レシピ数制限 ---------- } ---------- */
	}
	PifComm_RecipeListReport(iKindOfRecipe, recipeDataSet, nOccurence);

	RecipeFile_Unlock();
}

//---------------------------------------------------------------------------
// EventProcessP306
void CMainFrmMailRecvThread::EventProcessP306()
{
	PifComm_DoStateDone(0);

	// トレースデータ報告
	TRACEDATASET traceData;
	MAINTENANCE_LAMP maintenanceLamp;
	COleDateTime dateTime;
	COleDateTimeSpan timeSpan;

	::ZeroMemory(&traceData, sizeof(TRACEDATASET));

	// SrVisReflectance Lamp
	maintenanceLamp = m_pDoc->GetMaintenanceLamp();

	if ( dateTime.ParseDateTime(maintenanceLamp.MaintenanceDate, LOCALE_NOUSEROVERRIDE) )
		dateTime.GetAsSystemTime(traceData.SrVisReflectanceLamp.lastExchange);

	timeSpan.m_span = maintenanceLamp.MaintenanceHour;
	traceData.SrVisReflectanceLamp.lElapseSecs	= (long)timeSpan.GetTotalSeconds();

	long lHourSpan[] = MAINTENANCE_LIFETIME_INIT;	// ref.System.h
	traceData.SrVisReflectanceLamp.lWarningSecs = lHourSpan[maintenanceLamp.nHourSpan] * 60 * 60;

	PifComm_TraceDataReport(&traceData);
}

//---------------------------------------------------------------------------
// EventProcessP308
void CMainFrmMailRecvThread::EventProcessP308()
{
	TCHAR szRecipeName[RECIPE_NAME_LEN + 1];
	MAIN_RCP_INFO mainRcpInfo;
	STAGE_PROG_INFO_HDR stageProgInfoHdr;

	PifComm_GetSampleSizeRecipeName(szRecipeName);
	if ( !RecipeFile_LoadRecipe(&mainRcpInfo, szRecipeName, RECIPE_FILE_MAIN_RECIPE) ) {
		PifComm_DoStateDone(1);
		return;
	}
	if ( !RecipeFile_LoadRecipe(&stageProgInfoHdr, mainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM) ) {
		PifComm_DoStateDone(1);
		return;
	}
	PifComm_DoStateDone(0);
	PifComm_SampleSizeDataReport(stageProgInfoHdr.SampleInfo.Size.dx, stageProgInfoHdr.SampleInfo.Size.dy);
}

// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 -->
//---------------------------------------------------------------------------
// EventProcessP310
void CMainFrmMailRecvThread::EventProcessP310()
{
	TCHAR szRecipeName[RECIPE_NAME_LEN + 1];
	MAIN_RCP_INFO mainRcpInfo;
	MEAS_PROG_INFO measProgInfo;
	double dLifeTime;
	int iRet;

	PifComm_GetReferenceDataCheckRecipeName(szRecipeName);
	if ( !RecipeFile_LoadRecipe(&mainRcpInfo, szRecipeName, RECIPE_FILE_MAIN_RECIPE) ) {
		// メインレシピが存在しない場合でも、リファレンスデータを確認する
		dLifeTime = 0.0;
		if ( 1 == MEAS_CheckRefFileElapsedTimeOut(szRecipeName, dLifeTime) )
			PifComm_DoStateDone(1);		// ファイルが存在しない
		else
			PifComm_DoStateDone(2);		// 期限切れ
		return;
	}

	if ( !RecipeFile_LoadRecipe(&measProgInfo, mainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM) ) {
		// 測定PGMが存在しない場合でも、リファレンスデータを確認する
		dLifeTime = 0.0;
		if ( 1 == MEAS_CheckRefFileElapsedTimeOut(szRecipeName, dLifeTime) )
			PifComm_DoStateDone(1);		// ファイルが存在しない
		else
			PifComm_DoStateDone(2);		// 期限切れ
		return;
	}

	iRet = MEAS_CheckRefFileElapsedTimeOut(szRecipeName, measProgInfo.Ref.hdr.dLifeTime);
	PifComm_DoStateDone(iRet);
}
// 2009.09.10 K.Matsuo リファレンスデータ確認機能追加 <--

// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 -->
//---------------------------------------------------------------------------
// CallLock
BOOL CMainFrmMailRecvThread::CallLock(BOOL order)
{
	char szFilePath[MAX_PATH];

	if(!order)
	{
		HWND hwnd;
		hwnd = ::FindWindow(NULL,"GLASSINE");
		if(!hwnd)
			return TRUE;
		long aa = ::SendMessage(hwnd, WM_CLOSE, 0L, 0L);
	}
	else if(order)
	{
		sprintf(szFilePath, "%s%s", g_szBin_Dir, "Glassine.exe");
		HINSTANCE hInst = ShellExecute(NULL, "open", szFilePath,
								 "-S1", NULL, SW_SHOWNORMAL );
		int r = int(hInst);
		if ( r <= 32 )
			return FALSE;
	}

	return TRUE;
}
// 2009.06.02 K.Matsuo 装置画面操作インターロック機能追加 <--
