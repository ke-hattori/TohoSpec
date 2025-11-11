// Alarm.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <nexif.hxx>
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "Alarm.h"
#include "StatusView.h"
#include "AlarmLogDlg.h"
#include "AlarmHistoryListView.h"
#include "..\\..\\INC\\PifComm.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarm                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CAlarm, CWinThread)

/////////////////////////////////////////////////////////////////////////////
//
CAlarm::CAlarm()
{
	m_Run = TRUE;

	m_pAlarmLogFile = new CLogFile(	g_szLog_Dir, 						// LOG_DIR,
									ALARM_LOG_NAME,
									ALARM_LOG_SIZE,
									ALARM_LOG_MAX,
									NULL);

	///// Alarm /////
	memset(m_AlarmList, 0, sizeof(m_AlarmList));
	m_AlarmMax = 0;
	AlarmListLoading();
	m_AlarmStatus = (ALARM_STATUS*)malloc(m_AlarmMax * sizeof(ALARM_STATUS));
	m_AlarmSet = (ALARM_SET*)malloc(m_AlarmMax * sizeof(ALARM_SET));
	memset(m_AlarmStatus, 0, m_AlarmMax * sizeof(ALARM_STATUS));
	memset(m_AlarmSet, 0, m_AlarmMax * sizeof(ALARM_SET));

	m_pMainFrame = NULL;
	m_pDoc = NULL;
	m_pStatusView = NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
CAlarm::~CAlarm()
{
	free(m_AlarmStatus);
	free(m_AlarmSet);

	if(m_pAlarmLogFile){
		delete m_pAlarmLogFile;
		m_pAlarmLogFile = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAlarm::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
int CAlarm::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。
	return CWinThread::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CAlarm, CWinThread)
	//{{AFX_MSG_MAP(CAlarm)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarm メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
int CAlarm::Run()
{
	while(m_Run){

		AlarmProcessing();
		AlarmCheck();

		//AlarmDebug();

		Sleep(100);
	}

// AfxBeginThreadで呼び出されているのでAfxEndThreadで終了する
//	ExitThread(0);
	AfxEndThread(0);

	return CWinThread::Run();
}

/////////////////////////////////////////////////////////////////////////////
// Alarm List Loading
void CAlarm::AlarmListLoading()
{
	int pos;
	CStdioFile File;
	CString sBuff;
	char szFilePath[MAX_PATH];

// K.Matsuo 2009.08.03 言語別アラームリスト読込み -->
	// ver 2.1.0 or higher ・・・コンフィグの言語指定により、AlarmList_E.ini or AlarmList_J.iniを読み込む
	// less than ver 2.1.0 ・・・AlarmList.iniを読み込むことになる
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	if(l_SystemConfig.nLanguage == LANGUAGE_ENGLISH)
		sprintf(szFilePath, "%s%s", g_szCfg_Dir, ALARM_FILE_E_NAME);
	else
		sprintf(szFilePath, "%s%s", g_szCfg_Dir, ALARM_FILE_J_NAME);

	// recipe data file open
	if(!File.Open(szFilePath, CFile::modeRead | CFile::typeText)){
		sprintf(szFilePath, "%s%s", g_szCfg_Dir, ALARM_FILE_NAME);
		if(!File.Open(szFilePath, CFile::modeRead | CFile::typeText)){
			AfxMessageBox("AlarmList.ini file Not Found.");
			return;
		}
	}
// K.Matsuo 2009.08.03 言語別アラームリスト読込み <--

	while(File.ReadString(sBuff) && m_AlarmMax < ALARM_LIST_MAX){

		pos = 0;

		///// Invalidate /////
		if(sBuff == ""){
			continue;
		}

		if(*sBuff.GetBuffer(0) == '/'){
			continue;
		}

		///// ALID /////
		m_AlarmList[m_AlarmMax].Id = atoi(sBuff.GetBuffer(0));

		///// ALCD /////
		pos = sBuff.Find(",", pos) + 1;
		m_AlarmList[m_AlarmMax].Alcd = atoi(sBuff.Mid(pos).GetBuffer(0));

		///// Level /////
		pos = sBuff.Find(",", pos) + 1;
		m_AlarmList[m_AlarmMax].Level = *sBuff.Mid(pos, 1).GetBuffer(0);

		///// Text /////
		pos = sBuff.Find(",", pos) + 1;
		strcpy(m_AlarmList[m_AlarmMax].Text, sBuff.Mid(pos, 80).GetBuffer(0));

		///// Next Alarm /////
		m_AlarmMax++;
	}

	File.Close();

	return;
}

/////////////////////////////////////////////////////////////////////////////
// Alarm Processing
void CAlarm::AlarmProcessing(void)
{
	int Index = 0;
	char szBuff[256] = {0};

	// Copy Alarm Status
	for(Index = 0;Index < m_AlarmMax;Index++){
		m_AlarmStatus[Index].Status = m_AlarmSet[Index].Status;
		memcpy(m_AlarmStatus[Index].SubItem,m_AlarmSet[Index].SubItem,sizeof(m_AlarmStatus[Index].SubItem));
	}

	// Alarm Up / Down Status
	for(Index = 0;Index < m_AlarmMax;Index++){
		m_AlarmStatus[Index].Up = (m_AlarmStatus[Index].Status | m_AlarmStatus[Index].Old) ^ m_AlarmStatus[Index].Old;
		m_AlarmStatus[Index].Down = (m_AlarmStatus[Index].Status | m_AlarmStatus[Index].Old) ^ m_AlarmStatus[Index].Status;
		m_AlarmStatus[Index].Old = m_AlarmStatus[Index].Status;
	}

	// Alarm Processing
	for(Index = 0;Index < m_AlarmMax;Index++){
		if(m_AlarmStatus[Index].Up == 1){
			AlarmOccurrence(Index);
		}
		if(m_AlarmStatus[Index].Down == 1){
			AlarmRelease(Index);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::AlarmOccurrence(int Index)
{
	CTime time = CTime::GetCurrentTime();
	char szLogText[MAX_PATH];
	char szText[MAX_PATH];
	ALARM_HIS AlarmHis;
	int AlarmHisIndex = 0;
	int len;

	memset(&AlarmHis, 0, sizeof(ALARM_HIS));

	///// Date /////
	strcpy_s(AlarmHis.Date, _countof(AlarmHis.Date), time.Format("%Y.%m.%d").GetBuffer(0));

	///// Time /////
	strcpy_s(AlarmHis.Time, _countof(AlarmHis.Time), time.Format("%H:%M:%S").GetBuffer(0));

	///// Occurrence Time /////
	strcpy(m_AlarmStatus[Index].OccurrenceTime, time.Format("%Y%m%d%H%M%S").GetBuffer(0));

	///// Level /////
	strcpy_s(AlarmHis.Level, _countof(AlarmHis.Level), (m_AlarmList[Index].Level == 'A') ? "ALARM" : "WARN");

	///// Set /////
	strcpy_s(AlarmHis.Set, _countof(AlarmHis.Set), "SET");

	///// ID /////
	sprintf(AlarmHis.Id, "%04d", m_AlarmList[Index].Id);

	///// Text /////
	memset(szText, 0, sizeof(szText));
	strcpy(szText, m_AlarmList[Index].Text);
	if(m_AlarmStatus[Index].SubItem[0] != '\0'){
		strcat(szText, " ");
		strcat(szText, m_AlarmStatus[Index].SubItem);
	}
	len = strlen(szText);
	if(len >= ALARM_LOG_TEXT_LEN) len = ALARM_LOG_TEXT_LEN;
	memcpy(AlarmHis.Text, szText, len);

	///// Add List Box /////
	if(m_pStatusView) m_pStatusView->AddAlarmList(Index, &AlarmHis);

	///// Add Alarm History /////
	if(g_pAlarmLogDlg) g_pAlarmLogDlg->AddAlarmHis(-1, &AlarmHis);

	///// Log Text /////
	sprintf(szLogText, "%s %s [%-5s] <%s> %s %s",
		AlarmHis.Date,
		AlarmHis.Time,
		AlarmHis.Level,
		AlarmHis.Set,
		AlarmHis.Id,
		AlarmHis.Text
	);

	///// Add Log File /////
	m_pAlarmLogFile->Logging(szLogText);

//	///// Alarm History Memory /////
//	AlarmHisIndex = m_pDoc->AlarmHis_Add(-1, &AlarmHis);
//
//	// Alarm History View
//	if(m_pMainFrame->m_iCurrExeWnd == ALARM_HISTORY_LIST_WND){
//		if(AlarmHisIndex >= 0){
//			if(m_pMainFrame->m_pCurrExeView != NULL){
//				m_pMainFrame->m_pCurrExeView->PostMessage(WM_ALARM_HIS_ADD, AlarmHisIndex, 0);
//			}
//		}
//	}

	///// Send To Host /////
	ALARMDATASET AlarmData;

	memset(&AlarmData, 0, sizeof(AlarmData));

	AlarmData.chEventType = 'S';
	AlarmData.chAlarmLevel = AlarmHis.Level[0];
	AlarmData.iAlarmId = m_AlarmList[Index].Id;
	AlarmData.iAlarmCode = m_AlarmList[Index].Alcd;
	strcpy(AlarmData.szAlarmMessage, AlarmHis.Text);

	PifComm_AlarmReport(&AlarmData);
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::AlarmRelease(int Index)
{
	CTime time = CTime::GetCurrentTime();
	char szLogText[MAX_PATH];
	char szText[MAX_PATH];
	ALARM_HIS AlarmHis;
	int AlarmHisIndex = 0;
	int len;

	memset(&AlarmHis, 0, sizeof(ALARM_HIS));

	///// Date /////
	strcpy_s(AlarmHis.Date, _countof(AlarmHis.Date), time.Format("%Y.%m.%d").GetBuffer(0));

	///// Time /////
	strcpy_s(AlarmHis.Time, _countof(AlarmHis.Time), time.Format("%H:%M:%S").GetBuffer(0));

	///// Occurrence Time /////
	strcpy(m_AlarmStatus[Index].OccurrenceTime, time.Format("%Y%m%d%H%M%S").GetBuffer(0));

	///// Level /////
	strcpy_s(AlarmHis.Level, _countof(AlarmHis.Level), (m_AlarmList[Index].Level == 'A') ? "ALARM" : "WARN");

	///// Set /////
	strcpy(AlarmHis.Set, "RST");

	///// ID /////
	sprintf(AlarmHis.Id, "%04d", m_AlarmList[Index].Id);

	///// Text /////
	memset(szText, 0, sizeof(szText));
	strcpy(szText, m_AlarmList[Index].Text);
	if(m_AlarmStatus[Index].SubItem[0] != '\0'){
		strcat(szText, " ");
		strcat(szText, m_AlarmStatus[Index].SubItem);
	}
	len = strlen(szText);
	if(len >= ALARM_LOG_TEXT_LEN) len = ALARM_LOG_TEXT_LEN;
	memcpy(AlarmHis.Text, szText, len);

	// Delete List Box
	DeleteAlarmList(Index);

	///// Add Alarm History /////
	g_pAlarmLogDlg->AddAlarmHis(-1, &AlarmHis);

	///// Log Text /////
	sprintf(szLogText, "%s %s [%-5s] <%s> %s %s",
		AlarmHis.Date,
		AlarmHis.Time,
		AlarmHis.Level,
		AlarmHis.Set,
		AlarmHis.Id,
		AlarmHis.Text
	);

	///// Add Log File /////
	m_pAlarmLogFile->Logging(szLogText);

//	///// Alarm History Memory /////
//	AlarmHisIndex = m_pDoc->AlarmHis_Add(-1, &AlarmHis);
//
//	// Alarm History View
//	if(m_pMainFrame->m_iCurrExeWnd == ALARM_HISTORY_LIST_WND){
//		if(AlarmHisIndex >= 0){
//			if(m_pMainFrame->m_pCurrExeView != NULL){
//				m_pMainFrame->m_pCurrExeView->PostMessage(WM_ALARM_HIS_ADD, AlarmHisIndex, 0);
//			}
//		}
//	}

	///// Send To Host /////
	ALARMDATASET AlarmData;

	memset(&AlarmData, 0, sizeof(AlarmData));

	AlarmData.chEventType = 'R';
	AlarmData.chAlarmLevel = AlarmHis.Level[0];
	AlarmData.iAlarmId = m_AlarmList[Index].Id;
	AlarmData.iAlarmCode = m_AlarmList[Index].Alcd;
	strcpy(AlarmData.szAlarmMessage, AlarmHis.Text);

	PifComm_AlarmReport(&AlarmData);
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::AlarmSet(int Id, char* subitem/*="\0"*/)
{
	int Index;

	// Search Index
	if((Index = AlarmSearchIndex(Id)) == -1) return;

	// Alarm Set
	m_AlarmSet[Index].Status = 1;

	// SubItem
	if(subitem != NULL){
		memset(m_AlarmSet[Index].SubItem,0,sizeof(m_AlarmSet[Index].SubItem));
		strncpy(m_AlarmSet[Index].SubItem, subitem, SUBITEM_LEN);
	}
	else{
		m_AlarmSet[Index].SubItem[0] = '\0';
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::AlarmReset(int Id)
{
	int Index;

	// Search Index
	if((Index = AlarmSearchIndex(Id)) == -1) return;

	// Clear Alarm
	m_AlarmSet[Index].Status = 0;
	m_AlarmSet[Index].SubItem[0] = '\0';
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::AlarmAllReset()
{
	nexifAlarmAllReset(NULL);

	int Index;
	for(Index = 0; Index < m_AlarmMax; Index++){
		m_AlarmSet[Index].Status = 0;
		m_AlarmSet[Index].SubItem[0] = '\0';
	}
}

/////////////////////////////////////////////////////////////////////////////
//
int CAlarm::AlarmSearchIndex(int Id)
{
	int Index;

	for(Index = 0; Index < m_AlarmMax; Index++){
		if(m_AlarmList[Index].Id == Id) return Index;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::DeleteAlarmList(int Index)
{
	CListCtrl* pAlarmList = (CListCtrl*)m_pStatusView->GetDlgItem(IDC_ALARM_LIST);
	char strIndex[10+1];
	int Item;
	int Count;

	Count = pAlarmList->GetItemCount();

	if(pAlarmList){
		for(Item = 0; Item < Count; Item++){
			pAlarmList->GetItemText(Item, 0, strIndex, 10);
			if(Index == atoi(strIndex)){
				pAlarmList->DeleteItem(Item);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CAlarm::AlarmCheck()
{
	int Index;
	int Alarm, Warning;

	Alarm = Warning = 0;

	for(Index = 0; Index < m_AlarmMax; Index++){
		// Check Alarm Exist
		if(m_AlarmStatus[Index].Status){
			switch(m_AlarmList[Index].Level){
			case 'A':
				Alarm = 1;
				break;
			case 'W':
				Warning = 1;
				break;
			case 'O':
				break;
			}
		}
	}

	m_pDoc->SetAlarmStatus(Alarm);
	m_pDoc->SetWarningStatus(Warning);
}

/////////////////////////////////////////////////////////////////////////////
//
int CAlarm::AlarmStatusCheck(int Id)
{
	int Index;

	// Search Index
	if((Index = AlarmSearchIndex(Id)) == -1) return -1;

	// Alarm Set check
	if(m_AlarmStatus[Index].Status) return 1;
	else return 0;
}
