// NEXTRADLG.cpp : implementation file
//

#include "stdafx.h"
#include "common.hpp"
#include "nextra.h"
#include "alarm.h"
#include "nextradlg.h"
#include "alarm.h"
#include "informdlg.h"
#include "teachdlg.h"
#include "alhisdlg.h"
#include "manopdlg.h"
#include "syscfgdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CAboutDlg dialog used for App About
// ===========================================================================

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strText;
	strText.Format("%s (%s %s)", NEXTRA_SOFT_VERSION, __DATE__, __TIME__);
	SetDlgItemText(IDC_NEXTRA_VERSION, strText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ===========================================================================
// CNextraDlg dialog
// ===========================================================================

CNextraDlg::CNextraDlg(CWnd* pParent /*=NULL*/)
	: m_pdlgManOp(NULL), CDialog(CNextraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNextraDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	nexiniGetParameter(&m_params);
}

void CNextraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNextraDlg)
	 DDX_Control(pDX, IDC_ALARM_LIST, m_listAlarm);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNextraDlg, CDialog)
	//{{AFX_MSG_MAP(CNextraDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_QUIT, OnQuit)
	ON_COMMAND(IDM_ALARM_HISTORY, OnButtonAlarmHistory)
	ON_COMMAND(IDM_DIO_MONITOR, OnButtonOnDioOperation)
	ON_COMMAND(IDM_MANUAL_OPERATION, OnButtonManualOperation)
	ON_COMMAND(IDM_TEACH_OPERATION, OnButtonTeachOperation)
	ON_COMMAND(IDM_SYSTEM_CONFIGURATION, OnButtonSystemConfiguration)
	ON_BN_CLICKED(IDC_BUTTON_TEACH_OPERATION, OnButtonTeachOperation)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_OPERATION, OnButtonManualOperation)
	ON_BN_CLICKED(IDC_BUTTON_DIO_OPERATION, OnButtonOnDioOperation)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM_CONFIGURATION, OnButtonSystemConfiguration)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PASSWORD, OnButtonChangePassword)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_HISTORY, OnButtonAlarmHistory)
	ON_BN_CLICKED(IDC_BUTTON_ALARM_RESET, OnButtonAlarmReset)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEX_INITIALIZE, OnNexInitialize)
	ON_MESSAGE(WM_NEX_RETURN_TO_ORIGIN, OnNexReturnToOrigin)
	ON_MESSAGE(WM_NEX_LOAD, OnNexLoad)
	ON_MESSAGE(WM_NEX_UNLOAD, OnNexUnload)
	ON_MESSAGE(WM_NEX_MOVE_TO_UPPER, OnNexMoveToUpper)
	ON_MESSAGE(WM_NEX_MOVE_TO_ALIGNMENT, OnNexMoveToAlignment)
	ON_MESSAGE(WM_NEX_MOVE_TO_LOWER, OnNexMoveToLower)
	ON_MESSAGE(WM_NEX_OPEN_WORK_GUIDE, OnNexOpenWorkGuide)
	ON_MESSAGE(WM_NEX_CLOSE_WORK_GUIDE, OnNexCloseWorkGuide)
	ON_MESSAGE(WM_NEX_OPEN_SHUTTER, OnNexOpenShutter)
	ON_MESSAGE(WM_NEX_CLOSE_SHUTTER, OnNexCloseShutter)
	ON_MESSAGE(WM_NEX_VACUUM_ON, OnNexVacuumOn)
	ON_MESSAGE(WM_NEX_VACUUM_OFF, OnNexVacuumOff)
	ON_MESSAGE(WM_NEX_AIR_FLOAT_ON, OnNexAirFloatOn)
	ON_MESSAGE(WM_NEX_AIR_FLOAT_OFF, OnNexAirFloatOff)
	ON_MESSAGE(WM_NEX_CANCEL, OnNexCancel)
	ON_MESSAGE(WM_NEX_ALARM_ALL_RESET, OnNexAlarmAllReset)
	ON_MESSAGE(WM_NEX_ALARM_SET, OnNexAlarmSet)
	ON_MESSAGE(WM_NEX_ALARM_RESET, OnNexAlarmReset)
	ON_MESSAGE(WM_NEX_RC_MOVE_TO_TEACH_POS, OnNexRC_MoveToTeachPos)
	ON_MESSAGE(WM_NEX_RC_JOG_PLUS, OnNexRC_JogPlus)
	ON_MESSAGE(WM_NEX_RC_JOG_MINUS, OnNexRC_JogMinus)
	ON_MESSAGE(WM_NEX_RC_JOG_STOP, OnNexRC_JogStop)
	ON_MESSAGE(WM_NEX_RC_SELECT_MODE, OnNexRC_SelectMode)
END_MESSAGE_MAP()

// ===========================================================================
// Request functions
// ===========================================================================
BOOL CNextraDlg::Initialize(void)
{
	m_listAlarm.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	int nWidth[] = { 0, 120, 50, 60, 280 };
	int nFormat[] = { LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT };

	int nCount = countof(nWidth);
	int nTotal = 0;
	for ( int index = 0; index < nCount; index++ ) nTotal += nWidth[index];

	RECT rect;
	m_listAlarm.GetClientRect(&rect);
	int cxScroll = ::GetSystemMetrics(SM_CXVSCROLL);

	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_FMT;
	for ( int nColumn = 0; nColumn < nCount; nColumn++ ) {
		lvColumn.cx = (int)((rect.right - cxScroll) * nWidth[nColumn] / nTotal + 0.5);
		lvColumn.fmt = nFormat[nColumn];
		m_listAlarm.InsertColumn(nColumn, &lvColumn);
	}
	m_alarm.Attach(m_listAlarm.GetSafeHwnd());

	nexioInitialize(m_hWnd);

	// NEXTRA ’P‘Ì‹N“®
	if ( !((CNextraApp*) AfxGetApp())->IsHostOption() ) {
		nexioEquipmentStatusRun(ON);
		if ( nexifInitialize(m_hWnd) == NEXIF_OK ) {
			((CNextraApp*) AfxGetApp())->m_TrayIcon.SetIcon(IDR_MAINFRAME);
		}
		nexioEquipmentStatusRun(OFF);
		switch ( m_params.System.Type ) {
		case NEXTRA_TYPEA:
			GetDlgItem(IDC_BUTTON_TEACH_OPERATION)->EnableWindow(TRUE);
			GetDlgItem(IDC_TEACH_OPERATION)->EnableWindow(TRUE);
			break;
		case NEXTRA_TYPEB:
		default:
			break;
		}

		GetDlgItem(IDC_BUTTON_MANUAL_OPERATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_MANUAL_OPERATION)->EnableWindow(TRUE);
	}
	else
	if ( m_params.Shutter.InitialClose ) {
		nexprocCloseShutter(m_hWnd);
	}
	return TRUE;
}

// ===========================================================================
// CNextraDlg message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
BOOL CNextraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if ( pSysMenu != NULL ) {
		CString strMenuText;

		strMenuText.LoadString(IDS_SYSTEMU_MENU_MOVE);
		pSysMenu->ModifyMenu(SC_MOVE, MF_STRING, SC_MOVE, strMenuText);

		strMenuText.LoadString(IDS_ABOUTBOX);
		if ( !strMenuText.IsEmpty() ) {
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strMenuText);
		}

		pSysMenu->AppendMenu(MF_SEPARATOR);

		strMenuText.LoadString(IDS_NEXTRA_QUIT_TITLE);
		if ( !strMenuText.IsEmpty() ) {
			pSysMenu->AppendMenu(MF_STRING, IDM_QUIT, strMenuText);
		}
#if 0
		strMenuText.LoadString(IDS_SYSTEMU_MENU_CLOSE);
		pSysMenu->ModifyMenu(SC_CLOSE, MF_STRING, SC_CLOSE, strMenuText);
#else
		pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);
#endif

	}

	SetIcon(m_hIcon, TRUE); 		// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pdlgManOp = new CManualOperationDlg();
	m_pdlgManOp->Create(CManualOperationDlg::IDD);

	Initialize();

	if ( m_params.System.CmdShow == NEXSW_MINIMIZE ) {
		ShowWindow(SW_MINIMIZE);
		ShowWindow(SW_HIDE);
	}

	if ( IsHardwareSimulation() ) {
		GetDlgItem(IDC_TEACH_OPERATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TEACH_OPERATION)->EnableWindow(FALSE);
	}

	SetTimer(EVENT_TIMER_ID, EVENT_TIMER_ELAPSE, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// About Nextra
	if ( (nID & 0xFFF0) == IDM_ABOUTBOX ) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	if ( nID == SC_MINIMIZE ) {
		ShowWindow(SW_HIDE);
	}
	else
	if ( (nID & 0xFFF0) == IDM_QUIT ) {
		if( IDOK == MyMessageBox( NULL, IDS_NEXTRA_QUIT_TEXT, IDS_NEXTRA_QUIT_TITLE,
						MB_ICONQUESTION | MB_OKCANCEL ) ) {
			//ProcessQuit();
			PostMessage(WM_CLOSE, 0, 0);
		}
	}
	else
	if ( nID == SC_CLOSE ) {
		// do nothing
	}
	else{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//	to draw the icon.  For MFC applications using the document/view model,
//	this is automatically done for you by the framework.

// ---------------------------------------------------------------------------
void CNextraDlg::OnPaint()
{
	if ( IsIconic() ) {
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialog::OnPaint();
	}
}

// ---------------------------------------------------------------------------
HCURSOR CNextraDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ---------------------------------------------------------------------------
BOOL CNextraDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent ) {
	case REFRESH_TIMER_ID:
		break;
	case EVENT_TIMER_ID:
		KillTimer(EVENT_TIMER_ID);
		if ( m_params.System.CmdShow == NEXSW_MINIMIZE ) {
			ShowWindow(SW_MINIMIZE);
			ShowWindow(SW_HIDE);
		}
		break;
	}
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
	if ( m_params.System.CmdShow == NEXSW_HIDE )
		lpwndpos->flags &= ~SWP_SHOWWINDOW;

	CDialog::OnWindowPosChanging(lpwndpos);
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnClose()
{
	OperationLogging("END Nextra Application");

	if ( m_pdlgManOp ) {
		m_pdlgManOp->DestroyWindow();
		delete m_pdlgManOp;
		m_pdlgManOp = NULL;
	}

	nexprocFinalize();
	nexioFinalize();

	Sleep(1L);

	EndDialog(IDOK);

//	  CDialog::OnClose();
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnQuit()
{
	CString strMsgTitle, strMsgText;
	strMsgTitle.LoadString(IDS_NEXTRA_QUIT_TITLE);
	strMsgText.LoadString(IDS_NEXTRA_QUIT_TEXT);
	if ( !strMsgTitle.IsEmpty() && !strMsgTitle.IsEmpty() ) {
		if( IDOK == MessageBox( strMsgText, strMsgTitle,
						MB_ICONQUESTION | MB_OKCANCEL ) ) {
			//ProcessQuit();
			PostMessage(WM_CLOSE, 0, 0);
		}
	}
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonTeachOperation()
{
	OperationLogging("NEXTRA - TEACH OPERATION Button was pushed.");

	CTeachLocationDlg dlgTeachLoc;
	dlgTeachLoc.DoModal();
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonManualOperation()
{
	OperationLogging("NEXTRA - MANUAL OPERATION Button was pushed.");

	m_pdlgManOp->ShowWindow(SW_HIDE);
	m_pdlgManOp->ShowWindow(SW_SHOW);
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonOnDioOperation()
{
	OperationLogging("NEXTRA - DI/O OPERATION Button was pushed.");

	nexioMonitor(0, 0);
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonSystemConfiguration()
{
	OperationLogging("NEXTRA - SYSTEM CONFIGURATION Button was pushed.");

	CSystemConfigurationDlg dlgSysConfig;
	dlgSysConfig.DoModal();
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonChangePassword()
{
	OperationLogging("NEXTRA - CHANGE PASSWORD Button was pushed.");

	nexuiPasswordConfig(m_hWnd);
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonAlarmHistory()
{
	OperationLogging("NEXTRA - ALARM HISTORY Button was pushed.");

	CAlarmHistoryDlg dlgAlarmHistory;
	dlgAlarmHistory.DoModal();
}

// ---------------------------------------------------------------------------
void CNextraDlg::OnButtonAlarmReset()
{
	OperationLogging("NEXTRA - ALARM RESET Button was pushed.");

	AlarmReset();
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexInitialize(WPARAM wp, LPARAM /*lp*/)
{
	OperationLogging("NEXTRA - CALL OnNexInitialize.");

	LRESULT lResult = nexprocInitialize();
	if ( lResult == NEXIF_OK )
		((CNextraApp*) AfxGetApp())->m_TrayIcon.SetIcon(IDR_MAINFRAME);
	return lResult;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexReturnToOrigin(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexReturnToOrigin.");

	nexprocReturnToOrigin(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexLoad(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexLoad.");

	nexprocLoad(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexUnload(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexUnload");

	nexprocUnload(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexMoveToUpper(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexMoveToUpper");

	nexprocMoveToUpper(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexMoveToAlignment(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexMoveToAlignment");

	nexprocMoveToAlignment(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexMoveToLower(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexMoveToLower");

	nexprocMoveToLower(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexOpenWorkGuide(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexOpenWorkGuide");

	nexprocOpenWorkGuide(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexCloseWorkGuide(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexCloseWorkGuide");

	nexprocCloseWorkGuide(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexOpenShutter(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexOpenShutter");

	nexprocOpenShutter(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexCloseShutter(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexCloseShutter");

	nexprocCloseShutter(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexVacuumOn(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexVacuumOn");

	nexprocVacuumOn(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexVacuumOff(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexVacuumOff");

	nexprocVacuumOff(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexAirFloatOn(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexAirFloatOn");

	nexprocAirFloatOn(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexAirFloatOff(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexAirFloatOff");

	nexprocAirFloatOff(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexCancel(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexCancel");

	nexprocCancel();

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexAlarmAllReset(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexAlarmAllReset");

	AlarmReset();

	if ( hwndClient != NULL )
		::PostMessage(hwndClient, WM_NEX_RESPONSE, nexAlarmAllReset, NEXIF_OK);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexAlarmSet(WPARAM wp, LPARAM /*lp*/)
{
	TCHAR szBuffer[256];
	_stprintf(szBuffer, "Alarm set %04d", wp);
	OperationLogging(szBuffer);

	m_alarm.Set((unsigned long) wp);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexAlarmReset(WPARAM wp, LPARAM lp)
{
	m_alarm.Reset((unsigned long) wp);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexRC_MoveToTeachPos(WPARAM wp, LPARAM lp)
{
	HWND hwndClient = (HWND) wp;
	unsigned char num = (unsigned char) lp;

	OperationLogging("NEXTRA - CALL OnNexRC_MoveToTeachPos");

	nexprocRC_MoveToTeachPos(hwndClient, num);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexRC_JogPlus(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexRC_JogPlus");

	nexprocRC_JogPlus(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexRC_JogMinus(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexRC_JogMinus");

	nexprocRC_JogMinus(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexRC_JogStop(WPARAM wp, LPARAM /*lp*/)
{
	HWND hwndClient = (HWND) wp;

	OperationLogging("NEXTRA - CALL OnNexRC_JogStop");

	nexprocRC_JogStop(hwndClient);

	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CNextraDlg::OnNexRC_SelectMode(WPARAM wp, LPARAM lp)
{
	HWND hwndClient = (HWND) wp;
	unsigned char mode = (unsigned char) lp;

	OperationLogging("NEXTRA - CALL nexifRC_SelectMode");

	nexprocRC_SelectMode(hwndClient, mode);

	return 0L;
}
