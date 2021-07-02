// SYSCFGDLG.CPP : implementation file
//

#include "stdafx.h"
#include "common.hpp"
#include "syscfgdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ===========================================================================
// CSystemConfigurationDlg dialog
// ===========================================================================

CSystemConfigurationDlg::CSystemConfigurationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemConfigurationDlg::IDD, pParent)
{
	nexiniGetParameter(&m_params);
	//{{AFX_DATA_INIT(CSystemConfigurationDlg)
	//}}AFX_DATA_INIT
}

void CSystemConfigurationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemConfigurationDlg)
	DDX_Text(pDX, IDC_TIMEOUTS_HOMING, m_params.MotionTimeouts.Homing);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.Homing, 0, 90999);
	DDX_Text(pDX, IDC_TIMEOUTS_ORT, m_params.MotionTimeouts.Origin);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.Origin, 0, 99099);
	DDX_Text(pDX, IDC_TIMEOUTS_TEACH_LOC, m_params.MotionTimeouts.TeachLoc);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.TeachLoc, 0, 99099);
	DDX_Text(pDX, IDC_TIMEOUTS_CLAMP, m_params.MotionTimeouts.WorkGuide);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.WorkGuide, 0, 99099);
	DDX_Text(pDX, IDC_TIMEOUTS_VAC_ON, m_params.MotionTimeouts.VacuumOn);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.VacuumOn, 0, 99099);
	DDX_Text(pDX, IDC_TIMEOUTS_VAC_OFF, m_params.MotionTimeouts.VacuumOff);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.VacuumOff, 0, 99099);
	DDX_Text(pDX, IDC_TIMEOUTS_SHUTTER_OPEN, m_params.MotionTimeouts.ShutterOpen);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.ShutterOpen, 0, 99099);
	DDX_Text(pDX, IDC_TIMEOUTS_SHUTTER_CLOSE, m_params.MotionTimeouts.ShutterClose);
	DDV_MinMaxLong(pDX, m_params.MotionTimeouts.ShutterClose, 0, 99099);
	DDX_Text(pDX, IDC_DELAY_CLAMP, m_params.MotionDelay.WorkGuideOpen);
	DDV_MinMaxLong(pDX, m_params.MotionDelay.WorkGuideOpen, 0, 99099);
	DDX_Text(pDX, IDC_DELAY_UNCLAMP, m_params.MotionDelay.WorkGuideClose);
	DDV_MinMaxLong(pDX, m_params.MotionDelay.WorkGuideClose, 0, 99099);
	DDX_Text(pDX, IDC_DELAY_VAC_DEST, m_params.MotionDelay.VacuumDestruct);
	DDV_MinMaxLong(pDX, m_params.MotionDelay.VacuumDestruct, 0, 99099);
	DDX_Check(pDX, IDC_ENABLE_VACUUM_FEEDBACK, m_params.System.VacuumFeedback);
	DDX_Check(pDX, IDC_ENABLE_CONFIRM_OPERATOR, m_params.System.ConfirmOperator);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSystemConfigurationDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemConfigurationDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ===========================================================================
// CSystemConfigurationDlg message handlers
// ===========================================================================

// ---------------------------------------------------------------------------
BOOL CSystemConfigurationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ---------------------------------------------------------------------------
void CSystemConfigurationDlg::OnCancel()
{
	OperationLogging("SYSTEM CONFIGURATION - CANCEL Button was pushed.");

	CDialog::OnCancel();
}

// ---------------------------------------------------------------------------
void CSystemConfigurationDlg::OnOK()
{
	OperationLogging("SYSTEM CONFIGURATION - OK Button was pushed.");

	if ( UpdateData() ) {
		if( IDOK == MyMessageBox( NULL, IDS_SYSTEM_CONFIG_TEXT, IDS_SYSTEM_CONFIG_TITLE,
						MB_ICONQUESTION | MB_OKCANCEL ) ) {
			nexiniSetParameter(&m_params);
		}
		else {
			return;
		}
	}

	CDialog::OnOK();
}
