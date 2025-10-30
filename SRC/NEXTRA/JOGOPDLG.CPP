// MANOPDLG.CPP : implementation file
//

#include "stdafx.h"
#include <hook.hxx>
#include "common.hpp"
#include "nextra.h"
#include "jogopdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CJogOperationDlg dialog
// ===========================================================================

CJogOperationDlg::CJogOperationDlg(CWnd* pParent /*=NULL*/)
	: m_axis(nexax_axis_1), CDialog(CJogOperationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJogOperationDlg)
	m_lJogSpeed = 100L;
	//}}AFX_DATA_INIT
	nexiniGetParameter(&m_params);
}

void CJogOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJogOperationDlg)
	DDX_Control(pDX, IDC_COMBO_CURRENT_AXIS, m_cbCurrentAxis);
	DDX_Control(pDX, IDC_CURRENT_POS, m_stcCurrentPos);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED, m_lJogSpeed);
	DDV_MinMaxLong(pDX, m_lJogSpeed, 1, 1000);
#if 1
	DDX_Control(pDX, IDC_BUTTON_MOTION_UP, m_btnMotionUp);
	DDX_Control(pDX, IDC_BUTTON_MOTION_DOWN, m_btnMotionDown);
#endif
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJogOperationDlg, CDialog)
	//{{AFX_MSG_MAP(CJogOperationDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENT_AXIS, OnSelchangeComboCurrentAxis)
#if 0
	ON_BN_CLICKED(IDC_BUTTON_MOTION_UP, OnButtonMotionUp)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_DOWN, OnButtonMotionDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_STOP, OnButtonMotionStop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KEYHOOK, OnKeyHook)
#else
	ON_BN_CLICKED(IDC_BUTTON_MOTION_STOP, OnButtonMotionStop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KEYHOOK, OnKeyHook)
	ON_MESSAGE(WM_BUTTON_EVENT, OnButtonEvent)
#endif
END_MESSAGE_MAP()

// ===========================================================================
// CJogOperationDlg message handlers
// ===========================================================================

BOOL CJogOperationDlg::Initialize(void)
{
	nexioInitialize(m_hWnd);

	nexaxInitialize();

	if ( nexaxIsInitialized(m_axis) )
		((CNextraApp*) AfxGetApp())->m_TrayIcon.SetIcon(IDR_MAINFRAME);

	return TRUE;
}

// ---------------------------------------------------------------------------
BOOL CJogOperationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_stcCurrentPos.SetBkColor(YELLOW_COLOR);

	m_cbCurrentAxis.SetCurSel(m_axis);

	HookKeySet();

	Initialize();

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------------------------
BOOL CJogOperationDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN &&
		 pMsg->hwnd != GetDlgItem(IDC_COMBO_CURRENT_AXIS)->GetSafeHwnd() ) {
		switch ( pMsg->wParam ) {
		case VK_UP:
			if ( nexioIsVacuumOff() && nexioIsStageLoadPos() && UpdateData() ) {;
				KillTimer(REFRESH_TIMER_ID);
				CCursor::BeginWaitCursor();
			OperationLogging("JOG OPERATION - VK_UP Key was pushed.");
			EquipmentStatusRun();
			nexaxNormalJog(m_axis, static_cast<short>(m_lJogSpeed));
				SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
			}
			return TRUE;
		case VK_DOWN:
			if ( UpdateData() ) {
				KillTimer(REFRESH_TIMER_ID);
				CCursor::BeginWaitCursor();
			OperationLogging("JOG OPERATION - VK_DOWN Key was pushed.");
			EquipmentStatusRun();
			nexaxNormalJog(m_axis, static_cast<short>(-m_lJogSpeed));
				SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
			}
			return TRUE;
		case VK_RETURN:
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnClose()
{
	OperationLogging("END Nextra Application");

	KillTimer(REFRESH_TIMER_ID);

	nexaxFinalize();
	nexioFinalize();

	CDialog::OnClose();
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent ) {
	case REFRESH_TIMER_ID:
		KillTimer(REFRESH_TIMER_ID);
		RefreshControl();
		EnableControl();
		SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnDestroy()
{
	HookKeyUnhook();

	CDialog::OnDestroy();
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnSelchangeComboCurrentAxis()
{
	int nSelect = m_cbCurrentAxis.GetCurSel();
	if ( nSelect != CB_ERR ) {
		m_axis = (unsigned short) nSelect;
		UINT nIDResource = nexaxIsInitialized(m_axis) ? IDR_MAINFRAME : IDI_OFFLINE;
		((CNextraApp*) AfxGetApp())->m_TrayIcon.SetIcon(nIDResource);
	}
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnOK()
{
	OperationLogging("JOG OPERATION - OK Button was pushed.");

	KillTimer(REFRESH_TIMER_ID);

	nexaxFinalize();
	nexioFinalize();

	CDialog::OnOK();
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnCancel()
{
	OperationLogging("JOG OPERATION - CANCEL Button was pushed.");

	KillTimer(REFRESH_TIMER_ID);

	nexaxFinalize();
	nexioFinalize();

	CDialog::OnCancel();
}

// ---------------------------------------------------------------------------
LRESULT CJogOperationDlg::OnKeyHook(WPARAM wParam, LPARAM lParam)
{
	switch ( wParam ) {
	case VK_UP:
	case VK_DOWN:
		if ( HIWORD(lParam) & KF_UP && CCursor::IsWaitCursor() ) {
			CCursor::EndWaitCursor();
			OperationLogging("JOG OPERATION - VK_UP or VK_DOWN Key was upped.");
			KillTimer(REFRESH_TIMER_ID);
			nexaxMotionStop(m_axis);
			SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
			EquipmentStatusRun(OFF);
		}
		break;
	default:
		break;
	}

	return 0L;
}

#if 0
// ---------------------------------------------------------------------------
void CJogOperationDlg::OnButtonMotionUp()
{
	OperationLogging("JOG OPERATION - MOTION UP Button was pushed.");

	if ( !nexioIsVacuumOff() || !nexioIsStageLoadPos() || !UpdateData() ) return;

	nexaxNormalJog(m_axis, m_lJogSpeed);
	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnButtonMotionDown()
{
	OperationLogging("JOG OPERATION - MOTION DOWN Button was pushed.");

	if ( !nexioIsStageLoadPos() || !UpdateData() ) return;

	nexaxNormalJog(m_axis, -m_lJogSpeed);
	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
}
#else
// ---------------------------------------------------------------------------
LRESULT CJogOperationDlg::OnButtonEvent(WPARAM wParam, LPARAM lParam)
{
	UINT nEvent = (UINT) wParam;
	UINT nID	= (UINT) lParam;

	switch ( nEvent ) {
	case CColorButton::EV_LBUTTONDOWN:
		EquipmentStatusRun();
		switch ( nID ) {
		case IDC_BUTTON_MOTION_UP:
			if ( /*nexioIsVacuumOff() && nexioIsStageLoadPos() && */UpdateData() ) {
				KillTimer(REFRESH_TIMER_ID);
			OperationLogging("JOG OPERATION - MOTION UP Button was pushed.");
			CCursor::BeginWaitCursor();
			nexaxNormalJog(m_axis, static_cast<short>(m_lJogSpeed));
				SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
			}
			break;
		case IDC_BUTTON_MOTION_DOWN:
			if ( nexioIsStageLoadPos() && UpdateData() ) {
				KillTimer(REFRESH_TIMER_ID);
			OperationLogging("JOG OPERATION - MOTION DOWN Button was pushed.");
			CCursor::BeginWaitCursor();
			nexaxNormalJog(m_axis, static_cast<short>(-m_lJogSpeed));
				SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
			}
			break;
		}
		break;
	case CColorButton::EV_LBUTTONUP:
		switch ( nID ) {
		case IDC_BUTTON_MOTION_UP:
		case IDC_BUTTON_MOTION_DOWN:
			OperationLogging("JOG OPERATION - MOTION UP OR DOWN Button was upped.");
			KillTimer(REFRESH_TIMER_ID);
			nexaxMotionStop(m_axis);
			CCursor::EndWaitCursor();
			SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
			EquipmentStatusRun(OFF);
			break;
		}
		break;
	case CColorButton::EV_LBUTTONDBLCLK:
	case CColorButton::EV_RBUTTONDOWN:
	case CColorButton::EV_RBUTTONUP:
	case CColorButton::EV_RBUTTONDBLCLK:
		break;
	}
	return 0L;
}
#endif

// ---------------------------------------------------------------------------
void CJogOperationDlg::OnButtonMotionStop()
{
	OperationLogging("JOG OPERATION - MOTION STOP Button was pushed.");
	KillTimer(REFRESH_TIMER_ID);
	nexaxMotionStop(m_axis);
	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
	EquipmentStatusRun(OFF);
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::RefreshControl(void)
{
	GetCurrPos();
}

// ---------------------------------------------------------------------------
void CJogOperationDlg::EnableControl(void)
{
	BOOL bEnable = nexaxIsIdle(m_axis) && nexioIsStageLoadPos() && nexaxIsReady(m_axis);
	EnableControlGroup(this, bEnable, IDC_BUTTON_MOTION_DOWN, -1);
	bEnable &= nexioIsVacuumOff();
	EnableControlGroup(this, bEnable, IDC_BUTTON_MOTION_UP, -1);
}
