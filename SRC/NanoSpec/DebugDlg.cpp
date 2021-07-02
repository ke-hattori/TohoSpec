// DebugDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MeasurementListDlg.h"
#include "MeasurementTabView.h"
#include "Alarm.h"
#include "AlarmHistoryListView.h"
#include "DebugDlg.h"
#include "OverlayDialog.h"
#include "ManualStageDlg.h"
#include "..\\..\\INC\\PifComm.hxx"
#include "..\\..\\INC\\NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg ダイアログ                                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugDlg)
	//}}AFX_DATA_INIT

	m_loop = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugDlg)
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	//{{AFX_MSG_MAP(CDebugDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SEND_MEASUREMENT_DATA, OnSendMeasurementData)
	ON_BN_CLICKED(IDC_SEND_START_STATES, OnSendStartStates)
	ON_BN_CLICKED(IDC_SEND_ERROR_STATES, OnSendErrorStates)
	ON_BN_CLICKED(IDC_SEND_MANUAL_MEASUREMENT, OnSendManualMeasurement)
	ON_BN_CLICKED(IDC_DEB_ALARM_SET_BUTTON, OnDebAlarmSetButton)
	ON_BN_CLICKED(IDC_DEB_ALARM_RESET_BUTTON, OnDebAlarmResetButton)
	ON_BN_CLICKED(IDC_DEB_GET_PANE_BUTTON, OnDebGetPaneButton)
	ON_CBN_SELCHANGE(IDC_DEB_PROCESS_STATUS_COMBO, OnSelchangeDebProcessStatusCombo)
	ON_BN_CLICKED(IDC_DEB_PIFCOMM_BTN_P304, OnDebPifcommBtnP304)
	ON_BN_CLICKED(IDC_DEB_PIFCOMM_BTN_P305, OnDebPifcommBtnP305)
	ON_BN_CLICKED(IDC_DEB_PIFCOMM_BTN_P307, OnDebPifcommBtnP307)
	ON_BN_CLICKED(IDC_DEB_PIFCOMM_BTN_P502, OnDebPifcommBtnP502)
	ON_BN_CLICKED(IDC_DEB_PIFCOMM_BTN_P503, OnDebPifcommBtnP503)
	ON_BN_CLICKED(IDC_DEB_PIFCOMM_BTN_P504, OnDebPifcommBtnP504)
	ON_BN_CLICKED(IDC_DEB_NEXIO_MONITOR, OnDebNexioMonitor)
	ON_BN_CLICKED(IDC_HIBINO_DEBUG1, OnHibinoDebug1)
	ON_BN_CLICKED(IDC_DEB_MANUAL_STAGE_BUTTON2, OnDebManualStageButton2)
	ON_BN_CLICKED(IDC_DEB_MES_SEND, OnDebMesSend)
	ON_BN_CLICKED(IDC_DEB_MES_CANCEL, OnDebMesCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DEB_PATTERN_LOAD, OnDebPatternLoad)
	ON_BN_CLICKED(IDC_DEB_PATTERN_SAVE, OnDebPatternSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDlg メッセージ ハンドラ                                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CDebugDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	int i;
	//Saiki 20090527 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----

	m_iPoint = 1;

	///// Process Status /////
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DEB_PROCESS_STATUS_COMBO);
	for(i = 0 ; i < PROCESS_MAX ; i++){
		//Saiki 20090527 Change ----->
		//pCombo->AddString(PROCESS_STATUS_DISP[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(PROCESS_STATUS_DISP_ENU[i]);
		}
		else{
			pCombo->AddString(PROCESS_STATUS_DISP_JPN[i]);
		}
		//Saiki 20090527 Change <-----
	}
	pCombo->SetCurSel(0);

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnSendMeasurementData()
{
	//m_pView = ((CView*)((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter2.GetPane(0, 0));
	//((CMeasurementTabView*) m_pView)->PostMessage(WM_MEAS_COMP_EVENT, m_iPoint, 0);
	(CMainFrame*)AfxGetMainWnd()->PostMessage(WM_MEAS_COMP_EVENT, m_iPoint, 0); // MainFrameへ測定ポイント回数をWParam

	m_iPoint++;
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnSendStartStates()
{
	m_pView = ((CView*)((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter2.GetPane(0, 0));

	((CMeasurementTabView*) m_pView)->PostMessage(WM_MEAS_START_EVENT, m_iPoint, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnSendErrorStates()
{
	m_pView = ((CView*)((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter2.GetPane(0, 0));

	((CMeasurementTabView*) m_pView)->PostMessage(WM_MEAS_ERROR_EVENT, m_iPoint, 0);
	m_iPoint++;
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnSendManualMeasurement()
{
	m_pView = ((CView*)((CMainFrame*) AfxGetApp()->m_pMainWnd)->m_wndSplitter2.GetPane(0, 0));

	((CMeasurementTabView*) m_pView)->PostMessage(WM_MANUAL_START_EVENT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebAlarmSetButton()
{
	CString sId;
	CString sSubText;
	int Id;

	///// Alarm ID /////
	GetDlgItemText(IDC_DEB_ALARM_ID, sId);
	Id = atoi(sId.GetBuffer(0));

	AlarmIf_Set(Id);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebAlarmResetButton()
{
	CString sId;
	int Id;

	///// Alarm ID /////
	GetDlgItemText(IDC_DEB_ALARM_ID, sId);
	Id = atoi(sId.GetBuffer(0));

	AlarmIf_Reset(Id);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebGetPaneButton()
{
	CAlarmHistoryListView* pView = NULL;
	pView = (CAlarmHistoryListView*)m_pMainFrame->GetCurrentExeView();

}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnSelchangeDebProcessStatusCombo()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DEB_PROCESS_STATUS_COMBO);
	int Status;

	Status = pCombo->GetCurSel();
	m_pDoc->SetProcessStatus(Status);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebPifcommBtnP304()
{
	TRACE(_T("CDebugDlg::OnDebPifcommBtnP304()\n"));

	RECIPENAMEDATESET recipeNameDate[2];
	_tcscpy(recipeNameDate[0].szRecipeNameExt, _T("test1.mai"));
	::GetLocalTime(&recipeNameDate[0].recipeDateTime);
	_tcscpy(recipeNameDate[1].szRecipeNameExt, _T("test2.mai"));
	::GetLocalTime(&recipeNameDate[1].recipeDateTime);

	PifComm_RecipeListReport(0, recipeNameDate, 2);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebPifcommBtnP305()
{
	TRACE(_T("CDebugDlg::OnDebPifcommBtnP305()\n"));

	RECIPENAMEDATESET recipeNameDate;
	_tcscpy(recipeNameDate.szRecipeNameExt, _T("test1.mai"));
	::GetLocalTime(&recipeNameDate.recipeDateTime);

	PifComm_ChangeRecipeDeleteReport(0, &recipeNameDate);
	PifComm_ChangeRecipeInsertReport(0, &recipeNameDate);
	PifComm_ChangeRecipeReplaceReport(0, &recipeNameDate);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebPifcommBtnP307()
{
	TRACE(_T("CDebugDlg::OnDebPifcommBtnP307()\n"));

//	PifComm_TraceDataReport();
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebPifcommBtnP502()
{
	TRACE(_T("CDebugDlg::OnDebPifcommBtnP502()\n"));

//	PifComm_CompleteMeasurePointReport();
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebPifcommBtnP503()
{
	TRACE(_T("CDebugDlg::OnDebPifcommBtnP503()\n"));

//	PifComm_CompleteMeasureAllPointsReport();
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebPifcommBtnP504()
{
	TRACE(_T("CDebugDlg::OnDebPifcommBtnP504()\n"));

	PifComm_PressMeasureCancelButtonReport();
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebNexioMonitor()
{
	nexioMonitor(0, 0);
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnHibinoDebug1()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	(CMainFrame*)AfxGetMainWnd()->PostMessage(WM_CHIF_MOVE_SETPOS_END, m_iPoint, 0); // MainFrameへ

}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebManualStageButton2()
{
	CManualStageDlg dlg;
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebMesSend()
{
	CString	szBuf;
	long	m_count;

	CButton* pButton = (CButton*)GetDlgItem(IDC_DEB_MES_LOOP);
	m_loop = pButton->GetCheck();

	///// Counter /////
	GetDlgItemText(IDC_DEB_SEND_MES_COUNT, szBuf);
	m_count = atol(szBuf.GetBuffer(0));
	szBuf.Format("%ld", ++m_count);
	SetDlgItemText(IDC_DEB_SEND_MES_COUNT, szBuf);

	CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pMainFrame->PostMessage(WM_DEB_MESSAGE, m_count, 0); // MainFrameへ
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnDebMesCancel()
{

	CButton* pButton = (CButton*)GetDlgItem(IDC_DEB_MES_LOOP);
	pButton->SetCheck(0);

	m_loop = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CDebugDlg::OnTimer(UINT nIDEvent)
{
	CString	szBuf;

	if( m_loop ){

		OnDebMesSend();

	}

	CMainFrame* m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	szBuf.Format("%ld", m_pMainFrame->m_DbgCount);
	SetDlgItemText(IDC_DEB_RECV_MES_COUNT, szBuf);

	CDialog::OnTimer(nIDEvent);
}

void CDebugDlg::OnDebPatternLoad()
{
// Kojika 20090526 Add
	CString strBuffer;
// Kojika 20090526 Add End

	SITE_PATTERN	SitePattern[SCAN_POINT_MAX];
	WORD			wNumScans;

	::ZeroMemory(&SitePattern, sizeof(SitePattern));
	wNumScans = NULL;
	BOOL bRet = PatternFile_LoadPointList(SitePattern, &wNumScans, "test_l");
// Kojika 20090526 Change
//	if( !bRet ) MessageBox("PatternFile Load Faile",NULL,MB_OK);
	LoadStringML(IDS_PATTERNFILE_LOAD_FAILE, strBuffer, "PatternFile Load Faile");
	if( !bRet ) MessageBox(strBuffer,NULL,MB_OK);
// Kojika 20090526 Change End
}

void CDebugDlg::OnDebPatternSave()
{
// Kojika 20090526 Add
	CString strBuffer;
// Kojika 20090526 Add End

	SITE_PATTERN	SitePattern[SCAN_POINT_MAX];
	WORD			wNumScans;

	::ZeroMemory(&SitePattern, sizeof(SitePattern));
	wNumScans = 10;
	BOOL bRet = PatternFile_SavePointList(SitePattern, wNumScans, "test_s");
// Kojika 20090526 Change
//	if( !bRet ) MessageBox("PatternFile Save Faile",NULL,MB_OK);
	LoadStringML(IDS_PATTERNFILE_SAVE_FAILE, strBuffer, "PatternFile Save Faile");
	if( !bRet ) MessageBox(strBuffer,NULL,MB_OK);
// Kojika 20090526 Change End
}
