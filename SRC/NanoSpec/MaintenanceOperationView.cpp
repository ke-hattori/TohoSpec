// MaintenanceOperationView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MotionDlg.h"
#include "MaintenanceDateDlg.h"
#include "MaintenanceOperationView.h"
#include "ChiefExports.h"
#include "NEXIOBASE.HXX"

#include "..\\..\\INC\\NexIoBase.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceOperationView                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CMaintenanceOperationView, CNanoUI)

/////////////////////////////////////////////////////////////////////////////
//
CMaintenanceOperationView::CMaintenanceOperationView()
	: CNanoUI(CMaintenanceOperationView::IDD)
{
	//{{AFX_DATA_INIT(CMaintenanceOperationView)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
CMaintenanceOperationView::~CMaintenanceOperationView()
{
}

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaintenanceOperationView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_MOTION_CHECK_BUTTON, m_MotionButton);
	DDX_Control(pDX, IDC_DIO_MONITOR_BUTTON, m_DioMonitorButton);
	DDX_Control(pDX, IDC_TRANSITION_MONITOR_BUTTON, m_TransitionMonitorButton);
	DDX_Control(pDX, IDC_MAINTENANCE_DATE_BUTTON, m_MaintenanceDateButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CMaintenanceOperationView, CNanoUI)
	//{{AFX_MSG_MAP(CMaintenanceOperationView)
	ON_BN_CLICKED(IDC_MOTION_BUTTON, OnMotionButton)
	ON_BN_CLICKED(IDC_DIO_MONITOR_BUTTON, OnDioMonitorButton)
	ON_BN_CLICKED(IDC_TRANSITION_MONITOR_BUTTON, OnTransitionMonitorButton)
	ON_BN_CLICKED(IDC_MAINTENANCE_DATE_BUTTON, OnMaintenanceDateButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceOperationView 診断

#ifdef _DEBUG

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::AssertValid() const
{
	CNanoUI::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceOperationView メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

// 2014.01.17 Bagus Add (Stage None対応) -->
	//システムコンフィグ設定読み込み
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage None対応) <--

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_MotionButton.ShowWindow(SW_HIDE);
		m_DioMonitorButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// Motion Button /////
		m_MotionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_MotionButton.DrawFlatFocus(TRUE);

		///// DIO Monitor Button /////
		m_DioMonitorButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_DioMonitorButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Transition Monitor Button /////
	m_TransitionMonitorButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TransitionMonitorButton.DrawFlatFocus(TRUE);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Add (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None対応) <--
		m_MaintenanceDateButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// Maintenance Data Button /////
		m_MaintenanceDateButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_MaintenanceDateButton.DrawFlatFocus(TRUE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	SetTimer(1, 50, NULL);
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMaintenanceOperationView::CheckUserAccessPrivilege()
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( !pDoc->User_Access(ACCESS_MAINTENANCE) ) {
// Kojika 20090526 Change
		//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
//		MessageBox(IDS_USER_ACCESS_STOP_TEXT, IDS_USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		//Saiki 20090128 Change <-----
		LoadStringML(IDS_NOT_ACCESS_PRIVILEGE, strBuffer1, "You don't have Access Privilege");
		LoadStringML(IDS_TITLE_ACCESS_PRIVILEGE, strBuffer2, "ACCESS PRIVILEGE");
		MessageBox(strBuffer1, strBuffer2, MB_OK | MB_ICONSTOP);
// Kojika 20090526 Change End
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::OnMotionButton()
{
	if(CheckUserAccessPrivilege() == FALSE) return;

	CMotionDlg dlg;
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::OnDioMonitorButton()
{
	if(CheckUserAccessPrivilege() == FALSE) return;

	nexioMonitor(350, 180);
}

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::OnTransitionMonitorButton()
{
	if(CheckUserAccessPrivilege() == FALSE) return;

	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg != NULL){
		::PostMessage(l_hChiefDlg, WM_CHIF_SHOWSW, 1, 0);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CMaintenanceOperationView::OnMaintenanceDateButton()
{
#if 0 //UserSettingOperationCommonSettingDlgへ移動
	CMaintenanceDateDlg dlg;

	dlg.DoModal();
#endif
}

HBRUSH CMaintenanceOperationView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if( pWnd == this ) // 自分自身を描画するときは
	{
		if((sbNexMntSw == OFF)||(sbNexEngiMntSw == ON))
		{
			hbr = ::CreateSolidBrush( RGB( 255, 0, 0 ) );
		}
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

void CMaintenanceOperationView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	static BOOL sbNexMntSw = nexioIsMaintenanceSwitch();
	static BOOL sbNexMntSwOld = TRUE;
	static BOOL sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();
	static BOOL sbNexEngiMntSwOld = FALSE;

	sbNexMntSw = nexioIsMaintenanceSwitch();
	sbNexEngiMntSw = nexioIsEngineerMaintenanceSwitch();

	if((sbNexMntSw != sbNexMntSwOld)||(sbNexEngiMntSw != sbNexEngiMntSwOld))
	{
		Invalidate(TRUE); //OnCtlColor()の処理をさせる為にCall
	}
	sbNexMntSwOld = sbNexMntSw; //前回値書換え
	sbNexEngiMntSwOld = sbNexEngiMntSw; //前回値書換え

	CNanoUI::OnTimer(nIDEvent);
}
