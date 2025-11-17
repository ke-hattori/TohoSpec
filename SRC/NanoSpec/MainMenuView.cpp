// MainMenuView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MainMenuView.h"
#include "..\\..\\INC\\PifComm.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Define                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
#define LABEL_MAX						(200)
#define BUTTON_MAX						(200)

#define STATUS_TIMER					1		// 0.1 sec timer
#define STATUS_TIME 					100
#define COUNTER_TIMER					2		// 0.1 sec timer
#define COUNTER_TIME					100
#define REFRESH_TIMER					3
#define REFRESH_TIME					100

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView                                                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CMainMenuView, CNanoUI)

/////////////////////////////////////////////////////////////////////////////
//
CMainMenuView::CMainMenuView()
	: CNanoUI(CMainMenuView::IDD)
{
	//{{AFX_DATA_INIT(CMainMenuView)
	//}}AFX_DATA_INIT

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

	m_MainMenu		= MAIN_MENU_MEASUREMENT;

	memset(m_Disp, 0, sizeof(m_Disp));
	memset(m_Button, 0, sizeof(m_Button));;
}

/////////////////////////////////////////////////////////////////////////////
//
CMainMenuView::~CMainMenuView()
{

}

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainMenuView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MAIN_MENU_MEASUREMENT_BUTTON, m_MainMenuMeasurementButton);
	DDX_Control(pDX, IDC_MAIN_MENU_DATA_BUTTON, m_MainMenuDataButton);
	DDX_Control(pDX, IDC_MAIN_MENU_RECIPE_SETTING_BUTTON, m_MainMenuRecipeSettingButton);
	DDX_Control(pDX, IDC_MAIN_MENU_ALARM_HISTORY_BUTTON, m_MainMenuAlarmHistoryButton);
	DDX_Control(pDX, IDC_MAIN_MENU_LOG_ON_BUTTON, m_MainMenuLogOnButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CMainMenuView, CNanoUI)
	//{{AFX_MSG_MAP(CMainMenuView)
	ON_BN_CLICKED(IDC_MAIN_MENU_LOG_ON_BUTTON, OnMainMenuLogOnButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MAIN_MENU_DATA_BUTTON, OnMainMenuDataButton)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_MAIN_MENU_MEASUREMENT_BUTTON, IDC_MAIN_MENU_ALARM_HISTORY_BUTTON, OnMainMenuButton)
	ON_MESSAGE(WM_PIFCOMM_CONNECT, OnPifCommConnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2014.01.17 Bagus Add (Stage None対応) -->
	//システムコンフィグ設定読み込み
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage None対応) <--

	///// Data Button /////
// 2013.11.12 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
/* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- { ---------- */
//	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
/* modified 2014.05.22 hmenjo 手動時 NanoMap ----------              */
	if ((m_SystemConfig.nStageType == STAGE_TYPE_NONE) && (g_lModelType != MODEL_T3100)) {
/* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- } ---------- */
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_MainMenuDataButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.12 Bagus Add (TohoSpec対応) <--
		//m_MainMenuDataButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_MainMenuDataButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);	// 未選択カラー
// 2013.11.12 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.12 Bagus Add (TohoSpec対応) <--

/* added 2014.11.21 hmenjo LOCAL 非表示 ---------- { ---------- */
	if (g_lModelType == MODEL_T3100) {
		this->GetDlgItem(IDC_HOST_MODE)->ShowWindow(SW_HIDE);
	}
/* added 2014.11.21 hmenjo LOCAL 非表示 ---------- } ---------- */

	///// Log On Button /////
	//m_MainMenuLogOnButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MainMenuLogOnButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);	// 未選択カラー

	SelectMainMenuButton(MAIN_MENU_MEASUREMENT);

	///// Timer /////
	SetTimer(STATUS_TIMER, STATUS_TIME, NULL);
	SetTimer(COUNTER_TIMER, COUNTER_TIME, NULL);	// 0.1 sec timer
	SetTimer(REFRESH_TIMER, REFRESH_TIME, NULL);

	///// Auto Log On /////
	char szAutoLogonName[RECIPE_NAME_LEN + 1];
	USER_ACCOUNT userAccount;

	if ( ((CMainFrame*)AfxGetMainWnd())->HardwareSimulation() ) {
		strcpy(userAccount.szName, USER_ACCOUNT_TOHO_ENGINEER);
		userAccount.nAccountLevel = ACCOUNT_LEVEL_ADMINISTRATOR;
		userAccount.bEnablePassword = TRUE;
		strcpy(userAccount.szPassword, USER_ACCOUNT_TOHO_PASSWORD);
		m_pDoc->User_Entry(userAccount);
	}
	else {
		ConfigFile_GetNanoSpecIni(szAutoLogonName, CONFIG_FILE_AUTO_LOGON_NAME);
		if ( strlen(szAutoLogonName) != 0 )
			if ( RecipeFile_LoadRecipe(&userAccount, szAutoLogonName, RECIPE_FILE_USER_ACCOUNT) )
				m_pDoc->User_Entry(userAccount);
	}
	RefreshDlg(TRUE);

	/////
	PifComm_SetNotifyWnd(m_hWnd);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::OnDestroy()
{
	CNanoUI::OnDestroy();

	KillTimer(STATUS_TIMER);
	KillTimer(COUNTER_TIMER);
	KillTimer(REFRESH_TIMER);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::OnClose()
{

	CNanoUI::OnClose();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent){
	case STATUS_TIMER:
		break;
	case COUNTER_TIMER:
		break;
	case REFRESH_TIMER:
		KillTimer(REFRESH_TIMER);
		RefreshDlg();
		SetTimer(REFRESH_TIMER, REFRESH_TIME, NULL);
		break;
	default:
		break;
	}

	CNanoUI::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CMainMenuView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// User Name /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_USER_NAME)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_UserName, WATER_COLOR);
			return m_Brush_UserName;
		}

		///// Host Status /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_HOST_MODE)->GetSafeHwnd()){
			if ( m_pDoc->GetPifCommConnectStatus() )
				ControlColor(pDC, &m_Brush_HostMode, HOST_MODE_COLOR[m_pDoc->GetHostMode()]);
			else
				ControlColor(pDC, &m_Brush_HostMode, RED_COLOR);
			return m_Brush_HostMode;
		}

		///// Process Status /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_PROCESS_STATUS)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_ProcessStatus, PROCESS_STATUS_COLOR[m_pDoc->GetProcessStatus()]);
			return m_Brush_ProcessStatus;
		}

		///// Alarm Status /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_ALARME_STATUS)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_AlarmStatus, ALARM_STATUS_COLOR[m_pDoc->GetAlarmStatus()]);
			return m_Brush_AlarmStatus;
		}

		///// Warning Status /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_WARNING_STATUS)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_WarningStatus, WARNING_STATUS_COLOR[m_pDoc->GetWarningStatus()]);
			return m_Brush_WarningStatus;
		}

		break;
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}

/////////////////////////////////////////////////////////////////////////////
// Refresh Dialog
void CMainMenuView::RefreshDlg(BOOL bInit /* = FALSE */)
{
	int CtrlIndex=0;
	int ButtonIndex=0;
	BOOL flag = FALSE;
	CString sBuff;
	char szUserName[MAX_PATH];
	static char szOld_UserName[MAX_PATH] = {-1};
	//Saiki 20090527 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----

	/////////////////////////////////////////////////////////////////////////
	// Screen

//	CTRL_DISP(CtrlIndex++, m_pDoc->GetHostMode(), bInit, HOST_MODE_DISP, IDC_HOST_MODE);
	//Saiki 20090527 Add ----->
	//CTRL_DISP(CtrlIndex++, m_pDoc->GetHostMode(), TRUE, HOST_MODE_DISP, IDC_HOST_MODE);
	/////// Process Status /////
	//CTRL_DISP(CtrlIndex++, m_pDoc->GetProcessStatus(), bInit, PROCESS_STATUS_DISP, IDC_PROCESS_STATUS);
	//
	/////// Alarm Status /////
	//CTRL_DISP(CtrlIndex++, m_pDoc->GetAlarmStatus(), bInit, ALARM_STATUS_DISP, IDC_ALARME_STATUS);
	//
	/////// Warning Status /////
	//CTRL_DISP(CtrlIndex++, m_pDoc->GetWarningStatus(), bInit, WARNING_STATUS_DISP, IDC_WARNING_STATUS);
	//
	/////// Log On Button /////
	//CTRL_DISP(CtrlIndex++, !m_pDoc->User_Is(), bInit, LOG_ON_STATUS_DISP, IDC_MAIN_MENU_LOG_ON_BUTTON);

	if(l_SystemConfig.nLanguage == 0){
	///// Host Mode /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetHostMode(), TRUE, HOST_MODE_DISP_ENU, IDC_HOST_MODE);

	///// Process Status /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetProcessStatus(), bInit, PROCESS_STATUS_DISP_ENU, IDC_PROCESS_STATUS);

	///// Alarm Status /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetAlarmStatus(), bInit, ALARM_STATUS_DISP_ENU, IDC_ALARME_STATUS);

	///// Warning Status /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetWarningStatus(), bInit, WARNING_STATUS_DISP_ENU, IDC_WARNING_STATUS);

	///// Log On Button /////
		CTRL_DISP(CtrlIndex++, !m_pDoc->User_Is(), bInit, LOG_ON_STATUS_DISP_ENU, IDC_MAIN_MENU_LOG_ON_BUTTON);
	}
	else{
		///// Host Mode /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetHostMode(), TRUE, HOST_MODE_DISP_JPN, IDC_HOST_MODE);

		///// Process Status /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetProcessStatus(), bInit, PROCESS_STATUS_DISP_JPN, IDC_PROCESS_STATUS);

		///// Alarm Status /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetAlarmStatus(), bInit, ALARM_STATUS_DISP_JPN, IDC_ALARME_STATUS);

		///// Warning Status /////
		CTRL_DISP(CtrlIndex++, m_pDoc->GetWarningStatus(), bInit, WARNING_STATUS_DISP_JPN, IDC_WARNING_STATUS);

		///// Log On Button /////
		CTRL_DISP(CtrlIndex++, !m_pDoc->User_Is(), bInit, LOG_ON_STATUS_DISP_JPN, IDC_MAIN_MENU_LOG_ON_BUTTON);

	}
	//Saiki 20090527 Add <-----

	///// User Name /////
	strcpy(szUserName, m_pDoc->User_GetName());
	if(strcmp(szUserName, szOld_UserName) != 0){
		SetDlgItemText(IDC_USER_NAME, szUserName);


		//IDD_MAIN_MENU_FORM内の[データ]ボタンの有効/無効切替え
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.12 Bagus Add (TohoSpec対応) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.11.12 Bagus Add (TohoSpec対応) <--
/* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- { ---------- */
//		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
/* modified 2014.05.22 hmenjo 手動時 NanoMap ----------              */
		if ((m_SystemConfig.nStageType != STAGE_TYPE_NONE) || (g_lModelType == MODEL_T3100)) {
/* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- } ---------- */
// 2014.01.17 Bagus Mod (Stage None対応) <--
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_DATA_BUTTON)->EnableWindow(pDoc->User_Access(ACCESS_DATA));
// 2013.11.12 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.12 Bagus Add (TohoSpec対応) <--

	}
	strcpy(szOld_UserName, szUserName);

	/////////////////////////////////////////////////////////////////////////
	// Button

}

/////////////////////////////////////////////////////////////////////////////
// MEASUREMENT、DATA、RECIPE_SETTING、ALARM_HISTORYのいずれかのボタン押下時の処理
LRESULT CMainMenuView::OnMainMenuButton(UINT nID)
{
	int iMenu = nID - IDC_MAIN_MENU_MEASUREMENT_BUTTON;
	//Saiki 20090410 Change ----->
	//if ( m_MainMenu == iMenu )
	//	return 0L;
//	  if(!(iMenu == MAIN_MENU_RECIPE_SETTING)){
//		  if ( m_MainMenu == iMenu )
//			  return 0L;
//	  }
//	  else{
//		  //Saiki 20090608 Change ----->
//		  //m_pMainFrame->PostMessage(WM_COMMAND, IDM_RECIPE_SETTING, 0L);
//		  CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
//		  if ( !pMainFrame->MenuExit() )
//			  return 0L;
//		  //Saiki 20090608 Change <-----
//	  }
	if((iMenu != MAIN_MENU_RECIPE_SETTING) && ( m_MainMenu == iMenu )){
			return 0L;
	}
	else if( iMenu == MAIN_MENU_ALARM_HISTORY){
		//Do Nothing
	}
	else{
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
		if ( !pMainFrame->MenuExit() )
			return 0L;
	}
	//Saiki 20090410 Change <------
	//Saiki 20090608 Delete ----->
//	  CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
//	  if ( !pMainFrame->MenuExit() )
//		  return 0L;
	//Saiki 20090608 Delete <-----

	switch ( iMenu ) {
	case MAIN_MENU_MEASUREMENT:
		m_pMainFrame->PostMessage(WM_COMMAND, IDM_MEASUREMENT, 0L);
		break;
	case MAIN_MENU_DATA:
		m_pMainFrame->PostMessage(WM_COMMAND, IDM_DATA, 0L);
		break;
	case MAIN_MENU_RECIPE_SETTING:
		m_pMainFrame->PostMessage(WM_COMMAND, IDM_RECIPE_SETTING, 0L);
		break;
	case MAIN_MENU_ALARM_HISTORY:
		m_pMainFrame->PostMessage(WM_COMMAND, IDM_ALARM, 0L);
		break;
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// LOG ON（LOG OFF）ボタン押下時の処理
void CMainMenuView::OnMainMenuLogOnButton()
{
	m_pMainFrame->PostMessage(WM_COMMAND, IDM_LOGON, 0L);
}

/////////////////////////////////////////////////////////////////////////////
//　ホストモード変更
void CMainMenuView::ChangeHostMode(int iMode)
{
	if(m_pDoc->GetHostMode() == iMode) return;

	m_pDoc->SetHostMode(iMode);
}

/////////////////////////////////////////////////////////////////////////////
// UpdateButtonStatus ボタンカラーを変更するため、ボタンステータスを取得する
void CMainMenuView::UpdateButtonStatus(int iMode)
{
	CMap<int, int, int, int> iBtnStatusMap;
	iBtnStatusMap[OP_MEASUREMENT]		= MAIN_MENU_MEASUREMENT;
	iBtnStatusMap[OP_RECIPE_SETTING]	= MAIN_MENU_RECIPE_SETTING;
	iBtnStatusMap[OP_DATA]				= MAIN_MENU_DATA;
	iBtnStatusMap[OP_USER_SETTING]		= MAIN_MENU_USER_SETTING;
	iBtnStatusMap[OP_MAINTENANCE]		= MAIN_MENU_MAINTENANCE;
	iBtnStatusMap[OP_SYSTEM]			= MAIN_MENU_SYSTEM;
	iBtnStatusMap[OP_ALARM_HISTORY] 	= MAIN_MENU_ALARM_HISTORY;

	int iBtnStatus;
	if ( iBtnStatusMap.Lookup(iMode, iBtnStatus) )
		SelectMainMenuButton(iBtnStatus);
}

/////////////////////////////////////////////////////////////////////////////
// ボタンカラーを描画する
void CMainMenuView::SelectMainMenuButton(int Select)
{
	m_MainMenu = Select;

	CMap<int, int, CShadeButtonST*, CShadeButtonST*> buttonMap;
	buttonMap[MAIN_MENU_MEASUREMENT]	= &m_MainMenuMeasurementButton;
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.12 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.12 Bagus Add (TohoSpec対応) <--
/* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- { ---------- */
//	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
/* modified 2014.05.22 hmenjo 手動時 NanoMap ----------              */
	if ((m_SystemConfig.nStageType != STAGE_TYPE_NONE) || (g_lModelType == MODEL_T3100)) {
/* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- } ---------- */
// 2014.01.17 Bagus Mod (Stage None対応) <--
		buttonMap[MAIN_MENU_DATA]			= &m_MainMenuDataButton;
// 2013.11.12 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.12 Bagus Add (TohoSpec対応) <--
	buttonMap[MAIN_MENU_RECIPE_SETTING] = &m_MainMenuRecipeSettingButton;
	buttonMap[MAIN_MENU_ALARM_HISTORY]	= &m_MainMenuAlarmHistoryButton;

	int key;
	CShadeButtonST* button;

	POSITION pos = buttonMap.GetStartPosition();
	while ( pos != NULL ) {
		buttonMap.GetNextAssoc(pos, key, button);
		if ( key == Select )
			button->SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_RADIO_ON_COLOR);	// 選択カラー
		else
			button->SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_RADIO_OFF_COLOR);	// 未選択カラー
	}

	InvalidateRect(NULL, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// ネットワーク接続状況
afx_msg LRESULT CMainMenuView::OnPifCommConnect(WPARAM wp, LPARAM lp)
{
	m_pDoc->SetPifCommConnectStatus(wp);
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainMenuView::OnMainMenuDataButton()
{
/* modified 2014.11.22 hmenjo TohoRecall へ変更 ---------- { ---------- */
//	HWND hWnd;
//	char szFilePath[MAX_PATH];
//
//	hWnd = ::FindWindow(NULL, NANOMAP_WINDOW_NAME);
//	if(hWnd == NULL){
//		STARTUPINFO StartInfo;
//		PROCESS_INFORMATION ProcessInfo;
//		ZeroMemory(&StartInfo,sizeof(StartInfo));
//		ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));
//		StartInfo.wShowWindow = SW_SHOWNORMAL;
//		StartInfo.cb = sizeof(StartInfo);
///* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- { ---------- */
////		sprintf(szFilePath, "%s%s", g_szBin_Dir, NANOMAP_EXE_NAME);
///* modified 2014.05.22 hmenjo 手動時 NanoMap ----------              */
//		CString l_strNanomapExeName = NANOMAP_EXE_NAME;
//		if (g_lAppNameType == APP_NAME_TOHO) {
//			l_strNanomapExeName.Replace(g_lpszAppPrefix4[0], g_lpszAppPrefix4[1]);
//		}
//		sprintf(szFilePath, "%s%s", g_szBin_Dir, l_strNanomapExeName);
///* modified 2014.05.22 hmenjo 手動時 NanoMap ---------- } ---------- */
//		int ret = CreateProcess(	NULL,
//									szFilePath, 						// BIN_DIR "\\" NANOMAP_EXE_NAME,
//									NULL,
//									NULL,
//									FALSE,
//									0,
//									NULL,
//									".",
//									&StartInfo,
//									&ProcessInfo
//								);
//		//Saiki 20090410 Add ------>
//		g_ProcessInfo = ProcessInfo;
//		//Saiki 20090410 Add <------
//	}
//	else{
//		::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
//	}
/* modified 2014.11.22 hmenjo TohoRecall へ変更 ----------              */
	m_pMainFrame->OnMenuDataFromExt();
/* modified 2014.11.22 hmenjo TohoRecall へ変更 ---------- } ---------- */
}
