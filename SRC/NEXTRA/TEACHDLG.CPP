// TEACHDLG.CPP : implementation file
//

#include "stdafx.h"
#include <hook.hxx>
#include "common.hpp"
#include "informdlg.h"
#include "teachdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CTeachLocationDlg dialog
// ===========================================================================

CTeachLocationDlg::CTeachLocationDlg(CWnd* pParent /*=NULL*/)
	: m_axis(nexax_axis_1), m_pdlgInform(NULL), CDialog(CTeachLocationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachLocationDlg)
	m_lJogSpeed = 100L;
	m_lJogDistance = 1L;
   //}}AFX_DATA_INIT
	new_motparams.loc.speed[loc_upper].low			  = 1;
	new_motparams.loc.speed[loc_upper].high 		  = 1;
	new_motparams.loc.speed[loc_alignment].low		  = 1;
	new_motparams.loc.speed[loc_alignment].high 	  = 1;
	new_motparams.loc.speed[loc_lower].low			  = 1;
	new_motparams.loc.speed[loc_lower].high 		  = 1;
	new_motparams.loc.speed[loc_alignment_decel].low  = 1;
	new_motparams.loc.acc							  = 0.f;
	new_motparams.loc.dec							  = 0.f;
	new_motparams.home.speed.low					  = 1;
	new_motparams.home.speed.high					  = 1;
	new_motparams.home.acc							  = 0.f;
	new_motparams.home.dec							  = 0.f;
	new_motparams.jog_speed.value					  = 1;

	nexiniGetParameter(&m_params);
}

void CTeachLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachLocationDlg)
	DDX_Control(pDX, IDC_COMBO_CURRENT_AXIS, m_cbCurrentAxis);
	DDX_Control(pDX, IDC_UPPER_POS, m_stcUpperPos);
	DDX_Control(pDX, IDC_ALIGNMENT_POS, m_stcAlignmentPos);
	DDX_Control(pDX, IDC_LOWER_POS, m_stcLowerPos);
	DDX_Control(pDX, IDC_ALIGNMENT_DECEL_POS, m_stcAlignmentDecelPos);
	DDX_Control(pDX, IDC_CURRENT_POS, m_stcCurrentPos);
	DDX_Control(pDX, IDC_ARROW1, m_stcArrow1);
	DDX_Control(pDX, IDC_ARROW2, m_stcArrow2);
	DDX_Control(pDX, IDC_ARROW3, m_stcArrow3);
	DDX_Control(pDX, IDC_ARROW4, m_stcArrow4);
	DDX_Control(pDX, IDC_ARROW5, m_stcArrow5);
	DDX_Control(pDX, IDC_ARROW6, m_stcArrow6);
	DDX_Text(pDX, IDC_EDIT_JOG_SPEED, m_lJogSpeed);
	DDV_MinMaxLong(pDX, m_lJogSpeed, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_JOG_DISTANCE, m_lJogDistance);
	DDV_MinMaxLong(pDX, m_lJogDistance, 1, 500);
	DDX_Text(pDX, IDC_EDIT_UPPER_POS, new_motparams.loc.pos[loc_upper]);
	DDX_Text(pDX, IDC_EDIT_ALIGNMENT_POS, new_motparams.loc.pos[loc_alignment]);
	DDX_Text(pDX, IDC_EDIT_LOWER_POS, new_motparams.loc.pos[loc_lower]);
	DDX_Text(pDX, IDC_EDIT_ALIGNMENT_DECEL_POS, new_motparams.loc.pos[loc_alignment_decel]);
	DDX_Text(pDX, IDC_EDIT_UPPER_POS_SPEED1, new_motparams.loc.speed[loc_upper].high);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_upper].high, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_UPPER_POS_SPEED2, new_motparams.loc.speed[loc_upper].low);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_upper].low, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_ALIGNMENT_POS_SPEED1, new_motparams.loc.speed[loc_alignment].high);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_alignment].high, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_ALIGNMENT_POS_SPEED2, new_motparams.loc.speed[loc_alignment].low);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_alignment].low, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_LOWER_POS_SPEED1, new_motparams.loc.speed[loc_lower].high);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_lower].high, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_LOWER_POS_SPEED2, new_motparams.loc.speed[loc_lower].low);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_lower].low, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_ALIGNMENT_POS_DECEL_SPEED, new_motparams.loc.speed[loc_alignment_decel].high);
	DDV_MinMaxInt(pDX, new_motparams.loc.speed[loc_alignment_decel].high, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_TEACH_LOCATION_ACCEL_TIME, new_motparams.loc.acc);
	DDV_MinMaxFloat(pDX, new_motparams.loc.acc, 0.f, 99.99f);
	DDX_Text(pDX, IDC_EDIT_TEACH_LOCATION_DECEL_TIME, new_motparams.loc.dec);
	DDV_MinMaxFloat(pDX, new_motparams.loc.dec, 0.f, 99.99f);
	DDX_Text(pDX, IDC_EDIT_HOMING_FWD_SPEED, new_motparams.home.speed.high);
	DDV_MinMaxInt(pDX, new_motparams.home.speed.high, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_HOMING_REV_SPEED, new_motparams.home.speed.low);
	DDV_MinMaxInt(pDX, new_motparams.home.speed.low, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_HOMING_ACCEL_TIME, new_motparams.home.acc);
	DDV_MinMaxFloat(pDX, new_motparams.home.acc, 0.f, 99.99f);
	DDX_Text(pDX, IDC_EDIT_HOMING_DECEL_TIME, new_motparams.home.dec);
	DDV_MinMaxFloat(pDX, new_motparams.home.dec, 0., 99.99f);
	DDX_Text(pDX, IDC_EDIT_PULSE_JOG_SPEED, new_motparams.jog_speed.value);
	DDV_MinMaxInt(pDX, new_motparams.jog_speed.value, 1, 5000);


	if ( pDX->m_bSaveAndValidate ) {
		if ( new_motparams.loc.pos[loc_upper] <= new_motparams.loc.pos[loc_alignment] ) {
			MyMessageBox(NULL, IDS_UPARAMS_UNAVAILABLE, IDS_MOTPARAMS_TITLE, MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
			pDX->Fail();
		}
		else
		if ( new_motparams.loc.pos[loc_alignment] <= new_motparams.loc.pos[loc_alignment_decel] ) {
			MyMessageBox(NULL, IDS_APARAMS_UNAVAILABLE, IDS_MOTPARAMS_TITLE, MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
			pDX->Fail();
		}
		else
		if ( new_motparams.loc.pos[loc_lower] >= new_motparams.loc.pos[loc_alignment_decel] ) {
			MyMessageBox(NULL, IDS_LPARAMS_UNAVAILABLE, IDS_MOTPARAMS_TITLE, MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
			pDX->Fail();
		}
	}
	else {
		TCHAR szText[256];
		sprintf(szText, "%+011ld", new_motparams.loc.pos[loc_upper]);
		m_stcUpperPos.SetWindowText(szText);
		sprintf(szText, "%+011ld", new_motparams.loc.pos[loc_alignment]);
		m_stcAlignmentPos.SetWindowText(szText);
		sprintf(szText, "%+011ld", new_motparams.loc.pos[loc_lower]);
		m_stcLowerPos.SetWindowText(szText);
		sprintf(szText, "%+011ld", new_motparams.loc.pos[loc_alignment_decel]);
		m_stcAlignmentDecelPos.SetWindowText(szText);
	}

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTeachLocationDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachLocationDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENT_AXIS, OnSelchangeComboCurrentAxis)
	ON_BN_CLICKED(IDC_BUTTON_UPPER_POS_SET, OnButtonUpperPosSet)
	ON_BN_CLICKED(IDC_BUTTON_ALIGNMENT_POS_SET, OnButtonAlignmentPosSet)
	ON_BN_CLICKED(IDC_BUTTON_LOWER_POS_SET, OnButtonLowerPosSet)
	ON_BN_CLICKED(IDC_BUTTON_ALIGNMENT_DECEL_POS_SET, OnButtonAlignmentDecelPosSet)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_PARAMS_SAVE, OnButtonMotionParamsSave)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_PARAMS_RELOAD, OnButtonMotionParamsReload)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_UP, OnButtonMotionUp)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_DOWN, OnButtonMotionDown)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_STOP, OnButtonMotionStop)
	ON_BN_CLICKED(IDC_RADIO_JOG_DISTANCE, OnRadioJogDistance)
	ON_BN_CLICKED(IDC_RADIO_JOG_SPEED, OnRadioJogSpeed)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KEYHOOK, OnKeyHook)
END_MESSAGE_MAP()

// ===========================================================================
// CTeachLocationDlg message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
BOOL CTeachLocationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_stcUpperPos.SetBkColor(GREEN_COLOR);
	m_stcAlignmentPos.SetBkColor(GREEN_COLOR);
	m_stcLowerPos.SetBkColor(GREEN_COLOR);
	m_stcAlignmentDecelPos.SetBkColor(GREEN_COLOR);
	m_stcCurrentPos.SetBkColor(YELLOW_COLOR);

	m_stcArrow1.SetIcon(IDI_ARROW_ICON);
	m_stcArrow2.SetIcon(IDI_ARROW_ICON);
	m_stcArrow3.SetIcon(IDI_ARROW_ICON);
	m_stcArrow4.SetIcon(IDI_ARROW_ICON);
	m_stcArrow5.SetIcon(IDI_ARROW_ICON);
	m_stcArrow6.SetIcon(IDI_ARROW_ICON);

	CheckDlgButton(IDC_RADIO_JOG_DISTANCE, BST_CHECKED);
	SetDlgItemText(IDC_JOG_UNIT, "(mm)");

	m_cbCurrentAxis.SetCurSel(m_axis);

	HookKeySet();

	if ( !ReloadMotionParams() ) {
		EndDialog(IDCANCEL);
		return FALSE;
	}
	UpdateData(FALSE);

	RefreshControl();

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------------------------
BOOL CTeachLocationDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN &&
		 pMsg->hwnd != GetDlgItem(IDC_COMBO_CURRENT_AXIS)->GetSafeHwnd() ) {
		if ( IsDlgButtonChecked(IDC_RADIO_JOG_SPEED) ) {
			switch ( pMsg->wParam ) {
			case VK_UP:
			if ( nexioIsVacuumOff() && nexioIsStageLoadPos() && UpdateData() ) {;
				CCursor::BeginWaitCursor();
				nexaxNormalJog(m_axis, static_cast<short>(m_lJogSpeed));
					OperationLogging("TEACH LOCATION - VK_UP Key was pushed.");
				}
				return TRUE;
		case VK_DOWN:
			if ( nexioIsStageLoadPos() && UpdateData() ) {
				CCursor::BeginWaitCursor();
				nexaxNormalJog(m_axis, static_cast<short>(-m_lJogSpeed));
					OperationLogging("TEACH LOCATION - VK_DOWN Key was pushed.");
				}
				return TRUE;
			case VK_RETURN:
				return FALSE;
			default:
				break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnTimer(UINT nIDEvent)
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
void CTeachLocationDlg::OnDestroy()
{
	if ( m_pdlgInform ) {
		m_pdlgInform->DestroyWindow();
		delete m_pdlgInform;
	}

	HookKeyUnhook();

	CDialog::OnDestroy();
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnSelchangeComboCurrentAxis()
{
	KillTimer(REFRESH_TIMER_ID);

	int nSelect = m_cbCurrentAxis.GetCurSel();
	if ( nSelect != CB_ERR ) {
		m_axis = (unsigned short) nSelect;
	}

	if ( UpdateData() ) {

		if ( basic_interface::comparememory(&new_motparams, &old_motparams) != 0 ) {
			if( IDOK == MyMessageBox( m_hWnd, IDS_MOTION_PARAMS_SAVED, IDS_MOTPARAMS_TITLE,
							MB_ICONQUESTION | MB_OKCANCEL ) ) {
				SaveMotionParams();
			}
		}
		ReloadMotionParams();
		UpdateData(FALSE);
	}
	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonUpperPosSet()
{
	OperationLogging("TEACH LOCATION - UPPER POS SET Button was pushed.");

	nexaxGetCurrPos(m_axis, new_motparams.loc.pos[loc_upper]);

	UpdateData(FALSE);
}
// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonAlignmentPosSet()
{
	OperationLogging("TEACH LOCATION - ALIGNMENT POS SET Button was pushed.");

	nexaxGetCurrPos(m_axis, new_motparams.loc.pos[loc_alignment]);

	UpdateData(FALSE);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonLowerPosSet()
{
	OperationLogging("TEACH LOCATION - LOWER POS SET Button was pushed.");

	nexaxGetCurrPos(m_axis, new_motparams.loc.pos[loc_lower]);

	UpdateData(FALSE);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonAlignmentDecelPosSet()
{
	OperationLogging("TEACH LOCATION - ALIGNMENT DECEL POS SET Button was pushed.");

	nexaxGetCurrPos(m_axis, new_motparams.loc.pos[loc_alignment_decel]);

	UpdateData(FALSE);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonMotionParamsSave()
{
	OperationLogging("TEACH LOCATION - MOTION PARAMS SAVE Button was pushed.");

	KillTimer(REFRESH_TIMER_ID);

	if ( !UpdateData() ) return;

	SaveMotionParams();

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonMotionParamsReload()
{
	OperationLogging("TEACH LOCATION - MOTION PARAMS RELOAD Button was pushed.");

	KillTimer(REFRESH_TIMER_ID);

	ReloadMotionParams();

	UpdateData(FALSE);

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
}

// ---------------------------------------------------------------------------
LRESULT CTeachLocationDlg::OnKeyHook(WPARAM wParam, LPARAM lParam)
{
	switch ( wParam ) {
	case VK_UP:
	case VK_DOWN:
		if ( HIWORD(lParam) & KF_UP && CCursor::IsWaitCursor() ) {
			CCursor::EndWaitCursor();
			nexaxMotionStop(m_axis);
			OperationLogging("TEACH LOCATION - VK_UP or VK_DOWN Key was upped.");
		}
		break;
	default:
		break;
	}

	return 0L;
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonMotionUp()
{
	OperationLogging("TEACH LOCATION - MOTION UP Button was pushed.");

	if ( !nexioIsVacuumOff() || !nexioIsStageLoadPos() || !UpdateData() ) return;

	if ( IsDlgButtonChecked(IDC_RADIO_JOG_SPEED) )
		nexaxNormalJog(m_axis, static_cast<short>(m_lJogSpeed));
	else
	if ( IsDlgButtonChecked(IDC_RADIO_JOG_DISTANCE) )
		nexaxPulseJog(m_axis, m_params.ADAX.Resolution * m_lJogDistance);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonMotionDown()
{
	OperationLogging("TEACH LOCATION - MOTION DOWN Button was pushed.");

	if ( !nexioIsStageLoadPos() || !UpdateData() ) return;

	if ( IsDlgButtonChecked(IDC_RADIO_JOG_SPEED) )
		nexaxNormalJog(m_axis, static_cast<short>(-m_lJogSpeed));
	else
	if ( IsDlgButtonChecked(IDC_RADIO_JOG_DISTANCE) )
		nexaxPulseJog(m_axis, m_params.ADAX.Resolution * -m_lJogDistance);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnButtonMotionStop()
{
	OperationLogging("TEACH LOCATION - MOTION STOP Button was pushed.");

	nexaxMotionStop(m_axis);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnRadioJogDistance()
{
	OperationLogging("TEACH LOCATION - JOG DISTANCE Radio Button was pushed.");

	SetDlgItemText(IDC_JOG_UNIT, "(mm)");
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnRadioJogSpeed()
{
	OperationLogging("TEACH LOCATION - JOG SPEED Radio Button was pushed.");

	SetDlgItemText(IDC_JOG_UNIT, "(min-1)");
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::EnableControl(void)
{
	BOOL bEnable = nexaxIsIdle(m_axis) && nexioIsStageLoadPos() && nexaxIsReady(m_axis);
	EnableControlGroup ( this, bEnable,
						 IDC_COMBO_CURRENT_AXIS,
						 IDC_BUTTON_MOTION_DOWN,
						 IDC_BUTTON_MOTION_PARAMS_SAVE,
						 IDC_BUTTON_MOTION_PARAMS_RELOAD,
						 -1 );
	bEnable = nexioIsVacuumOff();
	EnableControlGroup(this, bEnable, IDC_BUTTON_MOTION_UP, -1);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::RefreshControl(void)
{
	long lPos;
	nexaxGetCurrPos(m_axis, lPos);

	TCHAR szText[256];
	sprintf(szText, "%+011ld", lPos);
	m_stcCurrentPos.SetWindowText(szText);
}

// ---------------------------------------------------------------------------
void CTeachLocationDlg::OnCancel()
{
	CDialog::OnCancel();
}
