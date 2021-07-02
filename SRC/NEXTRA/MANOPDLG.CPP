// MANOPDLG.CPP : implementation file
//

#include "stdafx.h"
#include "common.hpp"
#include "nextra.h"
#include "informdlg.h"
#include "manopdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CManualOperationDlg dialog
// ===========================================================================

CManualOperationDlg::CManualOperationDlg(CWnd* pParent /*=NULL*/)
	: m_lightMode(nexioMS_ModeLamp), m_kill(FALSE), CDialog(CManualOperationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualOperationDlg)
	//}}AFX_DATA_INIT
	nexiniGetParameter(&m_params);
}

void CManualOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualOperationDlg)
	DDX_Control(pDX, IDC_BUTTON_OPEN_SHUTTER, m_btnOpenShutter);
	DDX_Control(pDX, IDC_BUTTON_CLOSE_SHUTTER, m_btnCloseShutter);
	DDX_Control(pDX, IDC_COMBO_RC_MODE, m_cbRcMode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CManualOperationDlg, CDialog)
	//{{AFX_MSG_MAP(CManualOperationDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_UPPER_POS, OnButtonMoveToUpperPos)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_ALIGNMENT_POS, OnButtonMoveToAlignmentPos)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_LOWER_POS, OnButtonMoveToLowerPos)
	ON_BN_CLICKED(IDC_BUTTON_START_LOAD, OnButtonStartLoad)
	ON_BN_CLICKED(IDC_BUTTON_START_UNLOAD, OnButtonStartUnload)
	ON_BN_CLICKED(IDC_BUTTON_VACUUM_ON, OnButtonVacuumOn)
	ON_BN_CLICKED(IDC_BUTTON_VACUUM_OFF, OnButtonVacuumOff)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_WORK_GUIDE, OnButtonOpenWorkGuide)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_WORK_GUIDE, OnButtonCloseWorkGuide)
	ON_BN_CLICKED(IDC_BUTTON_AIR_FLOAT_ON, OnButtonAirFloatOn)
	ON_BN_CLICKED(IDC_BUTTON_AIR_FLOAT_OFF, OnButtonAirFloatOff)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SHUTTER, OnButtonOpenShutter)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_SHUTTER, OnButtonCloseShutter)
	ON_CBN_SELCHANGE(IDC_COMBO_RC_MODE, OnSelchangeComboRcMode)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
	ON_MESSAGE(WM_BUTTON_EVENT, OnButtonEvent)
END_MESSAGE_MAP()

// ===========================================================================
// CManualOperationDlg message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
BOOL CManualOperationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);

	EnableControlGroup(this, m_params.Options.Shutter,
		IDC_BUTTON_OPEN_SHUTTER,
		IDC_BUTTON_CLOSE_SHUTTER,
		IDC_RADIO_OPEN_SHUTTER,
		IDC_RADIO_CLOSE_SHUTTER,
		-1);

	EnableControlGroup(this, m_params.Options.AirFloat,
		IDC_BUTTON_AIR_FLOAT_ON,
		IDC_BUTTON_AIR_FLOAT_OFF,
		IDC_RADIO_AIR_FLOAT_ON,
		IDC_RADIO_AIR_FLOAT_OFF,
		-1);

	EnableControlGroup(this, m_params.Options.RoboCylinder,
		IDC_COMBO_RC_MODE,
		-1);

	if ( m_params.Options.RoboCylinder ) {
		m_cbRcMode.SetCurSel(nexioIsRC_MODES());
	}
	if ( !((CNextraApp*) AfxGetApp())->IsHostOption() ) {
		m_thread.begin(start_address, this);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent ) {
	case REFRESH_TIMER_ID:
		RefreshControl();
		EnableControl();
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnCancel()
{
	OperationLogging("MANUAL OPERATION - CANCEL Button was pushed.");

	nexifCancel(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnDestroy()
{
	m_lightMode.TurnOff();

	m_kill = TRUE;
	m_thread.finalize();

	CDialog::OnDestroy();
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::RefreshControl(void)
{
	CheckDlgButton(IDC_RADIO_MOVE_TO_UPPER_POS, nexioIsPinUpperPos());
	CheckDlgButton(IDC_RADIO_MOVE_TO_ALIGNMENT_POS, nexioIsPinAlignmentPos());
	CheckDlgButton(IDC_RADIO_MOVE_TO_LOWER_POS, nexioIsPinDownPos());
	CheckDlgButton(IDC_RADIO_VACUUM_ON, nexioIsVacuumOn());
	CheckDlgButton(IDC_RADIO_VACUUM_OFF, nexioIsVacuumOff());
	CheckDlgButton(IDC_RADIO_AIR_FLOAT_ON, nexioIsAirFloatStatus());
	CheckDlgButton(IDC_RADIO_AIR_FLOAT_OFF, !nexioIsAirFloatStatus());
	CheckDlgButton(IDC_RADIO_OPEN_WORK_GUIDE, nexioIsWorkGuideOpen());
	CheckDlgButton(IDC_RADIO_CLOSE_WORK_GUIDE, nexioIsWorkGuideClose());
	if ( !m_params.Options.Shutter ) return;
	CheckDlgButton(IDC_RADIO_OPEN_SHUTTER, nexioIsShutterOpen());
	CheckDlgButton(IDC_RADIO_CLOSE_SHUTTER, nexioIsShutterClose());
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::EnableControl(void)
{
	EnableControlGroup(this, nexprocIsIdle(),
		IDC_BUTTON_MOVE_TO_UPPER_POS, IDC_BUTTON_MOVE_TO_ALIGNMENT_POS, IDC_BUTTON_MOVE_TO_LOWER_POS,
		IDC_BUTTON_START_LOAD,IDC_BUTTON_START_UNLOAD,
		IDC_BUTTON_VACUUM_ON, IDC_BUTTON_VACUUM_OFF,
		IDC_BUTTON_OPEN_WORK_GUIDE,
		IDC_BUTTON_CLOSE_WORK_GUIDE,
		IDC_BUTTON_AIR_FLOAT_ON,
		IDC_BUTTON_AIR_FLOAT_OFF,
		-1);
	if ( !m_params.Options.Shutter ) return;
	EnableControlGroup(this, nexprocIsIdle(),
		IDC_BUTTON_OPEN_SHUTTER,
		IDC_BUTTON_CLOSE_SHUTTER,
		-1);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonMoveToUpperPos()
{
	OperationLogging("MANUAL OPERATION - MOVE TO UPPER POS Button was pushed.");

	EquipmentStatusRun();

	nexifMoveToUpper(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonMoveToAlignmentPos()
{
	OperationLogging("MANUAL OPERATION - MOVE TO ALIGNMENT POS Button was pushed.");

	EquipmentStatusRun();

	nexifMoveToAlignment(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonMoveToLowerPos()
{
	OperationLogging("MANUAL OPERATION - MOVE TO LOWER POS Button was pushed.");

	EquipmentStatusRun();

	nexifMoveToLower(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonStartLoad()
{
	OperationLogging("MANUAL OPERATION - START LOAD Button was pushed.");

	EquipmentStatusRun();

	nexifLoad(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonStartUnload()
{
	OperationLogging("MANUAL OPERATION - START UNLOAD Button was pushed.");

	EquipmentStatusRun();

	nexifUnload(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonVacuumOn()
{
	OperationLogging("MANUAL OPERATION - VACUUM ON Button was pushed.");

	nexifVacuumOn(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonVacuumOff()
{
	OperationLogging("MANUAL OPERATION - VACUUM OFF Button was pushed.");

	nexifVacuumOff(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonOpenWorkGuide()
{
	OperationLogging("MANUAL OPERATION - OPEN WORK GUIDE Button was pushed.");

	EquipmentStatusRun();

	nexifOpenWorkGuide(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonCloseWorkGuide()
{
	OperationLogging("MANUAL OPERATION - CLOSE WORK GUIDE Button was pushed.");

	EquipmentStatusRun();

	nexifCloseWorkGuide(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonAirFloatOn()
{
	OperationLogging("MANUAL OPERATION - AIR FLOAT ON Button was pushed.");

	nexifAirFloatOn(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonAirFloatOff()
{
	OperationLogging("MANUAL OPERATION - AIR FLOAT OFF Button was pushed.");

	nexifAirFloatOff(m_hWnd);
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonOpenShutter()
{
	OperationLogging("MANUAL OPERATION - OPEN SHUTTER Button was pushed.");
}

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnButtonCloseShutter()
{
	OperationLogging("MANUAL OPERATION - CLOSE SHUTTER Button was pushed.");
}

// ---------------------------------------------------------------------------
LRESULT CManualOperationDlg::OnNexResponse(WPARAM wp, LPARAM lp)
{
	switch ( wp ) {
	case nexInitialize:
	case nexReturnToOrigin:
	case nexLoad:
	case nexUnload:
	case nexMoveToUpper:
	case nexMoveToAlignment:
	case nexMoveToLower:
	case nexOpenWorkGuide:
	case nexCloseWorkGuide:
	case nexOpenShutter:
	case nexCloseShutter:
		EquipmentStatusRun(OFF);
		break;
	case nexRC_MoveToTeachPos:
		EquipmentStatusRun(OFF);
		CCursor::EndWaitCursor();
		break;
	case nexRC_JogPlus:
	case nexRC_JogMinus:
		EquipmentStatusRun(OFF);
		break;
	case nexRC_SelectMode:
		m_cbRcMode.SetCurSel(nexioIsRC_MODES());
		CCursor::EndWaitCursor();
		switch ( nexioRC_GetCurrentMode() ) {
		case RCOPMODE_JOG:			// ジョグ
			m_lightMode.Light();
			break;
		case RCOPMODE_INCHING:		// インチング
			m_lightMode.FlickerLow();
			break;
		case RCOPMODE_UNKNOWN:		// 不明
		case RCOPMODE_NORMAL:		// 通常
		default:
			m_lightMode.TurnOff();
			break;
		}
		break;
	default:
		break;
	}
	return 0L;
}

// ---------------------------------------------------------------------------
LRESULT CManualOperationDlg::OnButtonEvent(WPARAM wParam, LPARAM lParam)
{
	UINT nEvent = (UINT) wParam;
	UINT nID	= (UINT) lParam;

	switch ( nEvent ) {
	case CColorButton::EV_LBUTTONDOWN:
		EquipmentStatusRun();
		switch ( nID ) {
		case IDC_BUTTON_OPEN_SHUTTER:
			nexifOpenShutter(m_hWnd);
			break;
		case IDC_BUTTON_CLOSE_SHUTTER:
			nexifCloseShutter(m_hWnd);
			break;
		}
		break;
	case CColorButton::EV_LBUTTONUP:
		switch ( nID ) {
		case IDC_BUTTON_OPEN_SHUTTER:
		case IDC_BUTTON_CLOSE_SHUTTER:
			nexifCancel(m_hWnd);
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

// ---------------------------------------------------------------------------
void CManualOperationDlg::OnSelchangeComboRcMode()
{
	CCursor::BeginWaitCursor();

	int nSelect = m_cbRcMode.GetCurSel();
	if ( nSelect != CB_ERR ) {
		nexifRC_SelectMode(m_hWnd, nSelect == 0 ? RCOPMODE_NORMAL : nexioRC_GetJISL() ? RCOPMODE_INCHING : RCOPMODE_JOG);
	}
}

// ---------------------------------------------------------------------------
unsigned int CManualOperationDlg::start_address (void)
{
	static device_status modebtn, upbtn, downbtn;

	while ( !m_kill ) {
		if ( modebtn.detect(nexioIsMS_ModeSwitch()).is_down() ) {
			switch ( nexioRC_GetCurrentMode() ) {
			case RCOPMODE_JOG:			// ジョグ
				CCursor::BeginWaitCursor();
				nexifRC_SelectMode(m_hWnd, RCOPMODE_INCHING);
				break;
			case RCOPMODE_INCHING:		// インチング
				nexifRC_SelectMode(m_hWnd, RCOPMODE_JOG);
				break;
			case RCOPMODE_UNKNOWN:		// 不明
			case RCOPMODE_NORMAL:		// 通常
			default:
				break;
			}
		}
		switch ( nexioRC_GetCurrentMode() ) {
		case RCOPMODE_JOG:			// ジョグ
		case RCOPMODE_INCHING:		// インチング
			upbtn.detect(nexioIsMS_UpSwitch());
			downbtn.detect(nexioIsMS_DownSwitch());
			// 同時押し
			if ( upbtn.is_up() && downbtn.is_up() ) {
				break;
			}
			// 上ボタン押し
			if ( nexioIsMS_UpSwitch() ) {
				if ( !nexioIsMS_DownSwitch() && !nexioRC_GetJOGM() ) {
					CCursor::BeginWaitCursor();
					nexifRC_JogMinus(m_hWnd);
				}
			}
			else
			// 下ボタン押し
			if ( nexioIsMS_DownSwitch() ) {
				if ( !nexioIsMS_UpSwitch() && !nexioRC_GetJOGP() ) {
					CCursor::BeginWaitCursor();
					nexifRC_JogPlus(m_hWnd);
				}
			}
			else {
				// ジョグ運転中なら停止
				if ( nexioRC_GetJOGP() || nexioRC_GetJOGM() ) {
					nexifRC_JogStop(m_hWnd);
					CCursor::EndWaitCursor();
				}
			}
			break;
		case RCOPMODE_UNKNOWN:		// 不明
		case RCOPMODE_NORMAL:		// 通常
		default:
			break;
		}
		m_lightMode.Update();
		basic_timer::do_events(1L);
	}

	return 0L;
}
