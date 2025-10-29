#include "stdafx.h"
#include "resource.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "Misc.h"
#include "SpecifyCoordinateDlg.h"
#include "JogSettingDlg.h"
#include "SpeedSettingDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\AlarmIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"
#include "SaveAsDlg.h"
#include "OverlayDialog.h"
// 2010.01.07 bagus Gantry --{--
// 2010.01.21 bagus Gantry --{--
//#include "ScanDataGraphDlg.h"
// 2010.01.21 bagus Gantry --}--
// 2010.01.07 bagus Gantry --}--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define GRID_INTERVAL (500)*MICROMETRE

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

COverlayStatic::COverlayStatic()
{
}

BEGIN_MESSAGE_MAP(COverlayStatic, CStatic)
	//{{AFX_MSG_MAP(COverlayStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ===========================================================================
// COverlayStatic message handlers
// ===========================================================================

void COverlayStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_LBUTTONDOWN, (WPARAM) nFlags, MAKELPARAM(point.x + m_rcOverlay.left, point.y + m_rcOverlay.top));

	CStatic::OnLButtonDown(nFlags, point);
}

void COverlayStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_LBUTTONUP, (WPARAM) nFlags, MAKELPARAM(point.x + m_rcOverlay.left, point.y + m_rcOverlay.top));

	CStatic::OnLButtonUp(nFlags, point);
}

void COverlayStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_MOUSEMOVE, (WPARAM) nFlags, MAKELPARAM(point.x + m_rcOverlay.left, point.y + m_rcOverlay.top));

	CStatic::OnMouseMove(nFlags, point);
}

void COverlayStatic::PreSubclassWindow()
{
	GetWindowRect(&m_rcOverlay);
	GetParent()->ScreenToClient(&m_rcOverlay);

	DWORD dwStyle = GetStyle();
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | SS_NOTIFY);

	OverlayAttach(m_hWnd);
	OverlayPreview(TRUE);
	OverlayDrawCenter();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	OverlaySelectChannel(pMainFrame->GetCameraSelect());

	CStatic::PreSubclassWindow();
}

// ==========================================================================
//
COverlayDialog::COverlayDialog(UINT uResourceID /*= IDD_OVERLAY_DLG*/, CWnd* pParent /*= NULL*/)
	: CDialog(uResourceID, pParent)
{
	m_sCaption = "";
	m_sMessage = "";
	m_sFileName = "";
	m_nSampleMode = 0;
	m_CurrPos.lX = 0;
	m_CurrPos.lY = 0;
	m_CurrPos.lZ = 0;
	m_CurrSamplePos.lX = 0;
	m_CurrSamplePos.lY = 0;
	m_CurrSamplePos.lZ = 0;
	m_lX = 0;
	m_lY = 0;
	m_lZ = 0;
// 2013.01.22 bagus stage origin offset -->
	m_CurrTeachPos.lX = 0;
	m_CurrTeachPos.lY = 0;
	m_CurrTeachPos.lZ = 0;
// 2013.01.22 bagus stage origin offset <--

// INS 2009.07.07 by kawamura
	m_StagePositionSetting = FALSE;
// INS 2009.07.07 by kawamura
// 2009.11.16 K.Matsuo �w�b�h�I�t�Z�b�g�Ή� -->
	m_bOriginalPositionSetting = FALSE;
// 2009.11.16 K.Matsuo �w�b�h�I�t�Z�b�g�Ή� <--

// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
	m_bbDC = NULL;
	m_bbBitmap = NULL;
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--
// 2009.05.28 K.Matsuo SPT -->
	m_bIsMoveAtSpeed = FALSE;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.05.28 K.Matsuo SPT <--

	m_joyContinue = FALSE;
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	m_bProgress1PMeas = FALSE;
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
// 2010.01.07 bagus Gantry --{--
	m_bGantry = FALSE;
// 2010.01.07 bagus Gantry --}--
// 2010.01.13 bagus Gantry --{--
	m_bUseCheckScan = FALSE;
// 2010.01.13 bagus Gantry --}--
}

// ==========================================================================
//
void COverlayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COverlayDialog)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STAGE_UP_BUTTON, m_XY_UpButton);
	DDX_Control(pDX, IDC_STAGE_DOWN_BUTTON, m_XY_DownButton);
	DDX_Control(pDX, IDC_STAGE_RIGHT_BUTTON, m_XY_RightButton);
	DDX_Control(pDX, IDC_STAGE_LEFT_BUTTON, m_XY_LeftButton);
	DDX_Control(pDX, IDC_ELEV_UP_BUTTON, m_Z_UpButton);
	DDX_Control(pDX, IDC_ELEV_DOWN_BUTTON, m_Z_DownButton);
	DDX_Control(pDX, IDC_AUTO_FOCUS_BUTTON, m_btnAutoFocus);
	DDX_Control(pDX, IDC_CAPTURE_BUTTON, m_btnCapture);
	DDX_Control(pDX, IDC_SPECIFY_COORD_BUTTON, m_btnSpecifyCoord);
	DDX_Control(pDX, IDC_JOG_SETTING_BUTTON, m_btnJogSetting);
	DDX_Control(pDX, IDC_HEAD_TYPE, m_cbHeadType);
	DDX_Control(pDX, IDC_LENS_MAGNIFICATION_COMBO, m_cbLens);
	DDX_Control(pDX, IDC_MEASUREMENT_TYPE_COMBO, m_cbCamera);
	DDX_Control(pDX, IDC_STAGE_MOTION_MODE, m_cbMotionStage);
	DDX_Control(pDX, IDC_ELEV_MOTION_MODE, m_cbMotionElev);
	DDX_Control(pDX, IDC_MESSAGE_LABEL, m_ctlMessage);
	DDX_Control(pDX, IDC_OVERLAY, m_stcOverlay);
	//Saiki 20090908 Add ----->
	DDX_Control(pDX, IDC_HOME_POSITION_BUTTON, m_Z_HomePosButton);
	//Saiki 20090908 Add <-----
	DDX_Control(pDX, IDC_JOYSTICK1, *m_joyStick1);
	DDX_Control(pDX, IDC_JOYSTICK2, *m_joyStick2);
	//}}AFX_DATA_MAP
// INS 2009.07.06 by kawamura
	DDX_Control(pDX, IDC_STAGE_MOVE, m_btnStageMove);
// INS 2009.07.06 by kawamura
// 2010.01.06 bagus Gantry --{--
	DDX_Control(pDX, IDC_CHECK_SCAN, m_btnCheckScan);
// 2010.01.06 bagus Gantry --}--
	DDX_Control(pDX, IDC_SUBTHICK_COMBO, m_cbSubThick);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(COverlayDialog, CDialog)
	//{{AFX_MSG_MAP(COverlayDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_AUTO_FOCUS_BUTTON, OnAutoFocus)
	ON_BN_CLICKED(IDC_CAPTURE_BUTTON, OnCapture)
	ON_BN_CLICKED(IDC_SPECIFY_COORD_BUTTON, OnSpecifyCoord)
	ON_BN_CLICKED(IDC_JOG_SETTING_BUTTON, OnJogSetting)
// INS 2009.07.06 by kawamura
	ON_BN_CLICKED(IDC_STAGE_MOVE, OnStageMove)
// INS 2009.07.06 by kawamura
// 2010.01.07 bagus Gantry --{--
	ON_BN_CLICKED(IDC_CHECK_SCAN, OnCheckScan)
// 2010.01.07 bagus Gantry --}--
//Saiki 20091016 Add ---->
	ON_BN_CLICKED(IDC_HOME_POSITION_BUTTON, OnHomePositionButton)
//Saiki 20091016 Add <----
// 2010.01.29 bagus Gantry --{--
	ON_WM_ACTIVATE()
// 2010.01.29 bagus Gantry --}--
	//}}AFX_MSG_MAP
// 2009.05.28 K.Matsuo SPT -->
	ON_MESSAGE(WM_KEYHOOK, OnKeyHook)
// 2009.05.28 K.Matsuo SPT -->
	ON_MESSAGE(WM_DO_STAGE_MOVE, OnDoStageMove)
	ON_MESSAGE(WM_STAGE_MOVE_DONE, OnStageMoveDone)
	ON_MESSAGE(WM_DO_ELEVATOR_MOVE, OnDoElevatorMove)
	ON_MESSAGE(WM_JOYSTICK, OnJoyStick)
END_MESSAGE_MAP()

// ##########################################################################
// COverlayDialog message handlers
// ##########################################################################

// ==========================================================================
//
BOOL COverlayDialog::OnInitDialog()
{
BOOL bJoyStk;
CString strBuffer;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	if( !m_joyContinue )
		// ��ʂ��\������鎞�̓f�t�H���g�̓\�t�gJoy�Ƃ���
		m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);

	// Radio�{�^���̐ݒ�
	m_joyStick1 = new CJoyStickRadioButton(JOY_SOFT_MODE);
	m_joyStick2 = new CJoyStickRadioButton(JOY_HARD_MODE);

// 2013.01.11 bagus jog direction -->
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_JogDirSetting, CONFIG_FILE_JOG_DIR);
// 2013.01.11 bagus jog direction <--

// INS 2009.07.06 by kawamura
	CreateDialogItem("BUTTON", "",				BS_GROUPBOX,	CRect(CPoint(296,358), CSize(154,50)),	IDC_STAGE_POSITION_GROUP);
	CreateDialogItem("STATIC", "X", 			SS_CENTERIMAGE, CRect(CPoint(304,370), CSize(8,12)),	IDC_STAGE_POSITION_X_LABEL);
	CreateDialogItem("STATIC", "Y", 			SS_CENTERIMAGE, CRect(CPoint(304,388), CSize(8,12)),	IDC_STAGE_POSITION_Y_LABEL);
// 2010.01.21 bagus Gantry --{--
#if 0
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,		CRect(CPoint(316,370), CSize(50,12)),	IDC_STAGE_POSITION_X);
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,		CRect(CPoint(316,388), CSize(50,12)),	IDC_STAGE_POSITION_Y);
#else
	CreateDialogItem("STATIC", "",				SS_CENTERIMAGE | SS_SUNKEN | SS_RIGHT,		CRect(CPoint(316,370), CSize(50,12)),	IDC_STAGE_POSITION_X);
	CreateDialogItem("STATIC", "",				SS_CENTERIMAGE | SS_SUNKEN | SS_RIGHT,		CRect(CPoint(316,388), CSize(50,12)),	IDC_STAGE_POSITION_Y);
#endif
// 2010.01.21 bagus Gantry --}--
	CreateDialogItem("STATIC", "(mm)", 			SS_CENTERIMAGE, CRect(CPoint(368,370), CSize(24,12)),	IDC_STAGE_POSITION_X_MM);
	CreateDialogItem("STATIC", "(mm)", 			SS_CENTERIMAGE, CRect(CPoint(368,388), CSize(24,12)),	IDC_STAGE_POSITION_Y_MM);
	LoadStringML(IDS_STAGE_MOVE, strBuffer, "MOVE");
	CreateDialogItem("BUTTON", strBuffer,		BS_PUSHBUTTON,	CRect(CPoint(394,372), CSize(48,26)),	IDC_STAGE_MOVE);

	TCHAR szBuffer[256];
	_stprintf(szBuffer, "%.3lf", (double)(m_lX)/MICROMETRE);
	SetDlgItemText(IDC_STAGE_POSITION_X, szBuffer);
	_stprintf(szBuffer, "%.3lf", (double)(m_lY)/MICROMETRE);
	SetDlgItemText(IDC_STAGE_POSITION_Y, szBuffer);
// INS 2009.07.06 by kawamura

// 2010.01.08 bagus Gantry --{--
#if 0
// 2010.01.06 bagus Gantry --{--
	CreateDialogItem("BUTTON", "",							BS_GROUPBOX,	CRect(CPoint(296,416), CSize(154,120)),	IDC_STAGE_REFERENCE_GROUP);

	CreateDialogItem("STATIC", "Integration Time (sec)", 	SS_CENTERIMAGE, CRect(CPoint(304,432), CSize(72,12)),	IDC_STAGE_REFERENCE_INTEGRATION_TIME_LABEL);
	CreateDialogItem("EDIT", "",							ES_AUTOHSCROLL | ES_RIGHT, CRect(CPoint(384,432), CSize(57,12)),	IDC_STAGE_REFERENCE_INTEGRATION_TIME,WS_EX_CLIENTEDGE);

	CreateDialogItem("STATIC", "Filter", 					SS_CENTERIMAGE, CRect(CPoint(304,456), CSize(72,12)),	IDC_STAGE_REFERENCE_FILTER_LABEL);
	CreateDialogItem("COMBOBOX", "",						CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,	CRect(CPoint(384,452), CSize(57,55)), IDC_STAGE_REFERENCE_FILTER);

	CreateDialogItem("STATIC", "Transmittance Shutter", 	SS_CENTERIMAGE, CRect(CPoint(304,476), CSize(72,12)),	IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_LABEL);
	CreateDialogItem("BUTTON", "Open",						WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON,	CRect(CPoint(380,476), CSize(28,11)),	IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1);
	CreateDialogItem("BUTTON", "Close",						WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,	CRect(CPoint(412,476), CSize(28,11)),	IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2);

	CreateDialogItem("BUTTON", "CHECK SCAN",				BS_PUSHBUTTON,	CRect(CPoint(348,496), CSize(96,26)),	IDC_CHECK_SCAN);
// 2010.01.06 bagus Gantry --}--
#else
	CreateDialogItem("BUTTON", "",							BS_GROUPBOX,	CRect(CPoint(296,416), CSize(154,120)),	IDC_STAGE_REFERENCE_GROUP);

	LoadStringML(IDS_STAGE_REFERENCE_INTEGRATION_TIME_LABEL, strBuffer, "Integration Time (sec)");
	CreateDialogItem("STATIC", strBuffer, 					SS_CENTERIMAGE, CRect(CPoint(304,432), CSize(72,12)),	IDC_STAGE_REFERENCE_INTEGRATION_TIME_LABEL);
	CreateDialogItem("EDIT", "",							ES_AUTOHSCROLL | ES_RIGHT, CRect(CPoint(380,432), CSize(28,12)),	IDC_STAGE_REFERENCE_INTEGRATION_TIME,WS_EX_CLIENTEDGE);

	LoadStringML(IDS_STAGE_REFERENCE_FILTER_LABEL, strBuffer, "Filter");
	CreateDialogItem("STATIC", strBuffer, 					SS_CENTERIMAGE, CRect(CPoint(304,452), CSize(72,12)),	IDC_STAGE_REFERENCE_FILTER_LABEL);
	CreateDialogItem("COMBOBOX", "",						CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,	CRect(CPoint(380,452), CSize(57,55)), IDC_STAGE_REFERENCE_FILTER);

	LoadStringML(IDS_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_LABEL, strBuffer, "Transmittance Shutter");
	CreateDialogItem("STATIC", strBuffer, 					SS_CENTERIMAGE, CRect(CPoint(304,476), CSize(72,12)),	IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_LABEL);
	LoadStringML(IDS_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1, strBuffer, "Open");
	CreateDialogItem("BUTTON", strBuffer,					WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON,	CRect(CPoint(380,476), CSize(28,11)),	IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1);
	LoadStringML(IDS_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2, strBuffer, "Close");
	CreateDialogItem("BUTTON", strBuffer,					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,	CRect(CPoint(412,476), CSize(28,11)),	IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2);

	LoadStringML(IDS_CHECK_SCAN, strBuffer, "CHECK SCAN");
	CreateDialogItem("BUTTON", strBuffer,					BS_PUSHBUTTON,	CRect(CPoint(396,496), CSize(48,26)),	IDC_CHECK_SCAN);
#endif
// 2010.01.08 bagus Gantry --}--

	CDialog::OnInitDialog();
	int i;

	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2010.01.06 bagus Gantry --{--
	//2010.01.13 bagus Gantry --{--
	//if (m_bGantry) {
	if (m_bUseCheckScan) {
	//2010.01.13 bagus Gantry --}--
		GetDlgItem(IDC_STAGE_REFERENCE_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_REFERENCE_INTEGRATION_TIME_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_REFERENCE_INTEGRATION_TIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_REFERENCE_FILTER_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STAGE_REFERENCE_FILTER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_SCAN)->ShowWindow(SW_SHOW);
		((CButton *)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2))->SetCheck(1);
		InitCombo_SrOpticalFilter();
		//2010.01.13 bagus GTR --{--
		SR_CONFIG l_SrConfig;
		memset(&l_SrConfig,NULL,sizeof(l_SrConfig));
		ConfigFile_GetNanoSpecIni(&l_SrConfig,CONFIG_FILE_SR_CONFIG);
		CString strIntegTime;
		strIntegTime.Format("%.2f",l_SrConfig.dIntegrationTime);
		SetDlgItemText(IDC_STAGE_REFERENCE_INTEGRATION_TIME,strIntegTime);
		//2010.01.13 bagus GTR --}--
		//2010.01.15 bagus GTR --{--
		//���ߗ����g�p����ꍇ�ɗL���ɂ���
		//if(m_bGantry){
		if(l_SrConfig.bTransmittance){
			GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_LABEL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2)->ShowWindow(SW_SHOW);
			//���݂̃V���b�^�[��Ԃɍ��킹�ă`�F�b�N�̃f�t�H���g��Ԃ�ύX����
			if(nexioIsTransmitLightSourcePower()
			&&
			(nexioIsTransmitLight1Condition()
			|| nexioIsTransmitLight2Condition()
			|| nexioIsTransmitLight3Condition())
			){
				((CButton*)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2))->SetCheck(FALSE);
			}else{
				((CButton*)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2))->SetCheck(TRUE);
			}
		}else{
			GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_LABEL)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2)->ShowWindow(SW_HIDE);
		}
		//2010.01.15 bagus GTR --}--
	} else {
		GetDlgItem(IDC_STAGE_REFERENCE_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_INTEGRATION_TIME_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_INTEGRATION_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_FILTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_SCAN)->ShowWindow(SW_HIDE);
	}
// 2010.01.06 bagus Gantry --}--

	// Windows Caption
	SetWindowText(m_sCaption);

	// Message
	m_ctlMessage.SetWindowText(m_sMessage);

	m_btnOk.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnOk.DrawFlatFocus(TRUE);

	m_btnCancel.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnCancel.DrawFlatFocus(TRUE);

	/// XY Up Button ///
	m_XY_UpButton.SetIcon(IDI_ARROW_ROUND_UP, (int) BTNST_AUTO_GRAY);
	m_XY_UpButton.DrawBorder(FALSE);
	m_XY_UpButton.SetParams(VK_UP, WM_DO_STAGE_MOVE);

	/// XY Down Button ///
	m_XY_DownButton.SetIcon(IDI_ARROW_ROUND_DOWN, (int) BTNST_AUTO_GRAY);
	m_XY_DownButton.DrawBorder(FALSE);
	m_XY_DownButton.SetParams(VK_DOWN, WM_DO_STAGE_MOVE);

	/// XY Right Button ///
	m_XY_RightButton.SetIcon(IDI_ARROW_ROUND_RIGHT, (int) BTNST_AUTO_GRAY);
	m_XY_RightButton.DrawBorder(FALSE);
	m_XY_RightButton.SetParams(VK_RIGHT, WM_DO_STAGE_MOVE);

	/// XY Left Button ///
	m_XY_LeftButton.SetIcon(IDI_ARROW_ROUND_LEFT, (int) BTNST_AUTO_GRAY);
	m_XY_LeftButton.DrawBorder(FALSE);
	m_XY_LeftButton.SetParams(VK_LEFT, WM_DO_STAGE_MOVE);

	/// Z Up Button ///
#if 0
	m_Z_UpButton.SetIcon(IDI_ARROW_ROUND_UP, (int) BTNST_AUTO_GRAY);
	m_Z_UpButton.DrawBorder(FALSE);
	m_Z_UpButton.SetParams(VK_UP, WM_DO_ELEVATOR_MOVE);
#else
	m_Z_UpButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Z_UpButton.DrawFlatFocus(TRUE);
	m_Z_UpButton.SetParams(VK_UP, WM_DO_ELEVATOR_MOVE);
#endif

	/// Z Down Button ///
#if 0
	m_Z_DownButton.SetIcon(IDI_ARROW_ROUND_DOWN, (int) BTNST_AUTO_GRAY);
	m_Z_DownButton.DrawBorder(FALSE);
	m_Z_DownButton.SetParams(VK_DOWN, WM_DO_ELEVATOR_MOVE);
#else
	m_Z_DownButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Z_DownButton.DrawFlatFocus(TRUE);
	m_Z_DownButton.SetParams(VK_DOWN, WM_DO_ELEVATOR_MOVE);
#endif

//Saiki 20091015 Add ----->
	/// Z Home Position Button
#if 0

#else
	m_Z_HomePosButton.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Z_HomePosButton.DrawFlatFocus(TRUE);
#endif

//Saiki 20091015 Add <-----

	m_btnAutoFocus.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnAutoFocus.DrawFlatFocus(TRUE);

	m_btnCapture.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnCapture.DrawFlatFocus(TRUE);

	m_btnSpecifyCoord.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnSpecifyCoord.DrawFlatFocus(TRUE);

	m_btnJogSetting.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnJogSetting.DrawFlatFocus(TRUE);

// 2010.01.06 bagus Gantry --{--
	m_btnCheckScan.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_btnCheckScan.DrawFlatFocus(TRUE);
// 2010.01.06 bagus Gantry --}--

// INS 2009.07.06 by kawamura
	// ���W���Z�b�g����Ă��鎞�̂ݗL��
	if( m_StagePositionSetting ){
		m_btnStageMove.SetShade(BUTTON_RADIO_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_btnStageMove.DrawFlatFocus(TRUE);
	}
	else{
		GetDlgItem(IDC_STAGE_POSITION_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_POSITION_X_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_POSITION_Y_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_POSITION_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_POSITION_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_POSITION_X_MM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_POSITION_Y_MM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STAGE_MOVE)->ShowWindow(SW_HIDE);
	}
// INS 2009.07.06 by kawamura

// 2009.11.16 K.Matsuo �w�b�h�I�t�Z�b�g�Ή� -->
	m_cbHeadType.EnableWindow(m_bOriginalPositionSetting);
// 2009.11.16 K.Matsuo �w�b�h�I�t�Z�b�g�Ή� <--

	// �L���v�`���[��ʂ�\��
//	OverlayAttach(m_hWnd);
//	OverlayPreview(TRUE);
//	OverlayDrawCenter();
//	OverlaySetWindowPos(OVERLAY_VIEW_OFFSET_X, OVERLAY_VIEW_OFFSET_Y);

	m_crBkColor = OverlayGetBkColor();

	GetDlgItem(IDC_OVERLAY)->GetWindowRect(&m_rcOverlay);
	ScreenToClient(&m_rcOverlay);

	//
	if(!m_pMainFrame->HardwareSimulation()){
		StageGetOriginPos(&m_OriginPos);
	}

	///// Load Stage Config File /////
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	m_iOrgPos		 = m_StageConfig.OrgPos;
	m_dStageSizeX	 = m_StageConfig.Size.dX;
	m_dStageSizeY	 = m_StageConfig.Size.dY;
	m_dStageMaxSizeX = m_StageConfig.Size.dX;
	m_dStageMaxSizeY = m_StageConfig.Size.dY;
	m_StageOriginCoord.lX  = m_StageConfig.Origin.lX;
	m_StageOriginCoord.lY  = m_StageConfig.Origin.lY;

	// �T���v���̎擾
	GetSampleData();

	// �X�e�[�W����{�^���̃}�E�X���� ���܂��͋֎~
	BOOL bStageOpeButtonEnable;
	bStageOpeButtonEnable = (m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE);
	m_XY_UpButton.SetEnabled(bStageOpeButtonEnable);
	m_XY_DownButton.SetEnabled(bStageOpeButtonEnable);
	m_XY_RightButton.SetEnabled(bStageOpeButtonEnable);
	m_XY_LeftButton.SetEnabled(bStageOpeButtonEnable);

	// �W���C�X�e�B�b�N�L���^����
	if( m_pMainFrame->GetJoyStickMode() != JOY_SOFT_MODE ){
		// �W���C�X�e�B�b�N����
		bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
		if( bJoyStk ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
	}
	else{
		// �W���C�X�e�B�b�N�֎~
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}

// 2009.10.29 bagus Jog �ǉ��C�� --{--
	if (m_SystemConfig.nJoystickType == 0) {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	}
// 2009.10.29 bagus Jog �ǉ��C�� --}--

	SetTimer(REFRESH_TIMER_ID, REFRESH_TIMER_ELAPSE, NULL);
	SetTimer(OVERLAY_BUTTON_ENABLECHANGE_TIMER, 50, NULL);
	SetTimer(CHECK_ABORT_TIMER, 100, NULL);

	///// Get Video Calib /////
	ConfigFile_GetNanoSpecIni(m_SrVideoCalib, CONFIG_FILE_SR_VIDEO_CALIBRATION);
	for(i = 0 ; i < SR_LENS_MAX ; i++){
		if(m_SrVideoCalib[i].dXPixelToMicron <= 0) m_SrVideoCalib[i].dXPixelToMicron = 1;
		if(m_SrVideoCalib[i].dYPixelToMicron <= 0) m_SrVideoCalib[i].dYPixelToMicron = 1;
	}

	m_XY_ButtonUseHistry = 0;		// XY Button Use ?
	m_Z_ButtonUseHistry  = 0;		// Z Button Use ?
	m_AutoFocusUseHistry = 0;		// AutoFocus Use ?
	m_AutoFocusResult	 = 0;		// AutoFocus Result OK ? or NG ?

// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
	CreateStageMapBackBuffer();
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--

// 2009.05.28 K.Matsuo SPT -->
	HookKeySet();
// 2009.05.28 K.Matsuo SPT <--

// 2009.04.10 K.Matsuo Add -->
	//�����[�g���̓I�y���[�^�[�R�[��
	if(m_pDoc->GetHostMode() == HOST_REMOTE)
		AlarmIf_Set(ALID_OperatorCall);
// 2009.04.10 K.Matsuo Add <--

/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
#if 1
	if (HEAD_TYPE_MS == NS_GetCurrentHeadType()) {
		this->GetDlgItem(IDC_ELEV_UP_BUTTON)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_HOME_POSITION_BUTTON)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_ELEV_DOWN_BUTTON)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);
	}
#endif
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */

// 2010.01.21 bagus Gantry --{--
//	m_ScanDataGraphDlg.Create(CScanDataGraphDlg::IDD, this);
// 2010.02.01 bagus Gantry --{--
//	m_ScanDataGraphDlg.Create(CScanDataGraphDlg::IDD, NULL);
	m_ScanDataGraphDlg.Create(CScanDataGraphDlg::IDD, GetParent());
// 2010.02.01 bagus Gantry --}--
// 2010.01.21 bagus Gantry --}--

//Saiki 20130123 add --->
	if (m_SystemConfig.nJoystickType != 0) {
		if ( m_pDoc->Rap_IsDoorInterlock() == ON ) {		/* �h�A�C���^�[���b�N			��L */
			if ( m_joyStick1->GetCheck() ) {
				// �����I�Ƀ\�t�gJoy�֐؂�ւ���
				m_pMainFrame->SetJoyStickMode(JOY_HARD_MODE);
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
					m_joyStick1->UnChecked();
					m_joyStick2->Checked();
					m_joyStick2->SetFocus();
				}
				else{
					m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
					m_pDoc->ChangeJoyStick(FALSE);
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
					m_joyStick1->Checked();
					m_joyStick2->UnChecked();
					m_joyStick1->SetFocus();
				}
			}
		}
	}

//Saiki 20130123 add <---

// 2013.02.22 bagus Substrate thickness setting -->
	if ( m_nSampleMode != 0 ) {
		m_cbSubThick.EnableWindow(FALSE);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	return TRUE;
}

// ==========================================================================
// ��ʉE���̗̈�ɃR���g���[���𓮓I�쐬���邽�߂̊֐�
void COverlayDialog::CreateDialogItem(LPCTSTR lpClassName, LPCTSTR lpszCaption, DWORD dwStyle, RECT rect, UINT nID, DWORD dwStyleEx /* = 0 */)
{
	HWND hWnd;

	::MapDialogRect(m_hWnd, &rect);
	//2010.01.07 bagus �g���E�B���h�E�X�^�C�����Z�b�g�ł���悤�ɏC�� --{--
	//hWnd = ::CreateWindow(lpClassName, lpszCaption, WS_CHILD | WS_VISIBLE | dwStyle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, m_hWnd, (HMENU)nID, NULL, NULL);
	hWnd = ::CreateWindowEx(dwStyleEx,lpClassName, lpszCaption, WS_CHILD | WS_VISIBLE | dwStyle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, m_hWnd, (HMENU)nID, NULL, NULL);
	//2010.01.07 bagus --}--
	::SendMessage(hWnd, WM_SETFONT, ::SendMessage(m_hWnd, WM_GETFONT, 0L, 0L), 0L);
}

// ==========================================================================
//
void COverlayDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

}

// ==========================================================================
//
void COverlayDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawStageView();
}

// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
// ==========================================================================
//
void COverlayDialog::CreateStageMapBackBuffer()
{
	// �X�e�[�W�}�b�v�̌Œ蕔���̍쐬�i�o�b�N�o�b�t�@�j

	///// Region Map /////
	CRect StageRect;
	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
	CDC* pDC = pPictBox->GetDC();
	pPictBox->GetClientRect(&StageRect);

	m_bbDC = new CDC();
	m_bbDC->CreateCompatibleDC(pDC);
	m_bbBitmap = new CBitmap();
	m_bbBitmap->CreateCompatibleBitmap(pDC,StageRect.Width(),StageRect.Height());
	m_bbDC->SetBkMode(OPAQUE);
	m_bbDC->SelectObject(m_bbBitmap);
	pPictBox->ReleaseDC(pDC);

	// Draw Stage View
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
	/* �w�b�h�I�t�Z�b�g�l��Ǐo���܂��D	*/
	NS_GetHeadTypeCoordLoc(NS_GetCurrentHeadType(), &m_HeadOffset);
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
	SetupStageMap();
	SetupSampleMap();
	DrawGrid();
}
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--

// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
// ==========================================================================
//
void COverlayDialog::DrawStageView()
{
	// �X�e�[�W�}�b�v�̌Œ蕔���̃r�b�g�}�b�v���R�s�[
	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
	CDC* pDC = pPictBox->GetDC();
	CRect drawRect(m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX, m_lDrawEndStagePosY);
	pDC->BitBlt(drawRect.left,drawRect.top,drawRect.Width(),drawRect.Height(),m_bbDC,drawRect.left,drawRect.top,SRCCOPY);
	pPictBox->ReleaseDC(pDC);

	// �X�e�[�W�}�b�v�Ƀ|�W�V�����}�[�J�[��`��
	// Reset the stage position marker
	UpdateStagePosMarker();
}
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--

// ==========================================================================
//
HBRUSH COverlayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// Message Label /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_MESSAGE_LABEL)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_MessageLabel, REFERENCE_COLOR);
			return m_Brush_MessageLabel;
		}
		///// Stage /////
		///// Pos X /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_COORD_X)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Coord_X, REFERENCE_COLOR);
			return m_Brush_Coord_X;
		}
		///// Pos Y /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_COORD_Y)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Coord_Y, REFERENCE_COLOR);
			return m_Brush_Coord_Y;
		}
		///// Pos Z /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_COORD_Z)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Coord_Z, REFERENCE_COLOR);
			return m_Brush_Coord_Z;
		}
		///// Sample /////
		///// Pos X /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_SAMPLE_X)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Sample_X, REFERENCE_COLOR);
			return m_Brush_Sample_X;
		}
		///// Pos Y /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_SAMPLE_Y)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Sample_Y, REFERENCE_COLOR);
			return m_Brush_Sample_Y;
		}
		///// Pos Z /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_SAMPLE_Z)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Sample_Z, REFERENCE_COLOR);
			return m_Brush_Sample_Z;
		}

		///// Stage Pos X /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_STAGE_POSITION_X)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_StagePosition_X, REFERENCE_COLOR);
			return m_Brush_StagePosition_X;
		}
		///// Stage Pos Y /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_STAGE_POSITION_Y)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_StagePosition_Y, REFERENCE_COLOR);
			return m_Brush_StagePosition_Y;
		}
		break;
	}

	return hbr;
}

/// ==========================================================================
//
void COverlayDialog::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}

// ==========================================================================
//
void COverlayDialog::OnTimer(UINT nIDEvent)
{

	switch ( nIDEvent ) {
	case REFRESH_TIMER_ID:
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
		{
			/* �w�b�h���ؑւ���ꂽ��T���v���̕\�����w�b�h�I�t�Z�b�g�ɍ��킹�ăV�t�g�����ĕ\�����邽��
				�X�e�[�W�}�b�v���ĕ`�悵�܂��D	*/
			static WORD ls_wHeadPre = NS_GetCurrentHeadType();
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//			WORD l_wHead = NS_GetCurrentHeadType();
//			if (ls_wHeadPre != l_wHead) {
//				NS_GetHeadTypeCoordLoc(l_wHead, &m_HeadOffset);
//				ls_wHeadPre = l_wHead;
//				SetupStageMap();
//				SetupSampleMap();
//				DrawGrid();
//				this->OnPaint();
//			}
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
			if (TRUE == m_cbHeadType.m_bSelChanged) {
				m_cbHeadType.m_bSelChanged = FALSE;
				WORD l_wHead = NS_GetCurrentHeadType();
				if (ls_wHeadPre != l_wHead) {
					NS_GetHeadTypeCoordLoc(l_wHead, &m_HeadOffset);
					ls_wHeadPre = l_wHead;
					SetupStageMap();
					SetupSampleMap();
					DrawGrid();
					this->OnPaint();
				}
			}
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
		}
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
		Refresh();
		break;

	case OVERLAY_BUTTON_ENABLECHANGE_TIMER:
		ButtonEnableCheck();
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ -->
		JoyStickEnableCheck();
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ <--
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
		if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* �h�A�C���^�[���b�N			���g */
			if (0 != m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
				/* XY �X�e�[�W���쒆�ł��D	*/
				StageStop();
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
			}
		}
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
		break;

	case CHECK_ABORT_TIMER:
		if ( m_pDoc->GetProcessStatus() == PROCESS_ABRT ) {
			KillTimer(nIDEvent);
			OnCancel();
		}
		break;

	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

// ==========================================================================
//
void COverlayDialog::Refresh(void)
{
	TCHAR szBuff[256];

	if(!m_pMainFrame->HardwareSimulation()){
		NS_StageGetPos(&m_CurrPos);
// 2013.01.22 bagus stage origin offset -->
		STAGE_COORD_XYZ coordTemp;
		coordTemp = m_CurrPos;
		NS_ConvertToStageMoveCoord(&coordTemp);
		m_CurrTeachPos = coordTemp;
// 2013.01.22 bagus stage origin offset -->
	}

	if ( m_CurrPos.lX == m_old_CurrPos.lX && m_CurrPos.lY == m_old_CurrPos.lY && m_CurrPos.lZ == m_old_CurrPos.lZ &&
		m_CurrSamplePos.lX == m_old_CurrSamplePos.lX && m_CurrSamplePos.lY == m_old_CurrSamplePos.lY && m_CurrSamplePos.lZ == m_old_CurrSamplePos.lZ ) return;

	///// Stage /////
	///// X Axis /////
	if(m_CurrPos.lX != m_old_CurrPos.lX){
		sprintf(szBuff, "%.3lf", (double)(m_CurrPos.lX)/MICROMETRE);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//		SetDlgItemText(IDC_COORD_X, szBuff);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
		if (TRUE != this->m_bProgress1PMeas) {SetDlgItemText(IDC_COORD_X, szBuff);}
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	}
	///// Y Axis /////
	if(m_CurrPos.lY != m_old_CurrPos.lY){
		sprintf(szBuff, "%.3lf", (double)(m_CurrPos.lY)/MICROMETRE);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//		SetDlgItemText(IDC_COORD_Y, szBuff);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
		if (TRUE != this->m_bProgress1PMeas) {SetDlgItemText(IDC_COORD_Y, szBuff);}
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	}
	///// Z Axis /////
	if(m_CurrPos.lZ != m_old_CurrPos.lZ){
		sprintf(szBuff, "%.3lf", (double)(m_CurrPos.lZ)/MICROMETRE/10);
		SetDlgItemText(IDC_COORD_Z, szBuff);
	}

	///// Renew /////
	m_old_CurrPos.lX = m_CurrPos.lX;
	m_old_CurrPos.lY = m_CurrPos.lY;
	m_old_CurrPos.lZ = m_CurrPos.lZ;

	///// Sample /////
	if( m_sFileName == "" ){
		///// X Axis /////
		///// Y Axis /////
		///// Z Axis /////
		sprintf(szBuff, "%s", "  -----  ");
		SetDlgItemText(IDC_SAMPLE_X, szBuff);
		SetDlgItemText(IDC_SAMPLE_Y, szBuff);
		SetDlgItemText(IDC_SAMPLE_Z, szBuff);
	}
	else{
		///// X Axis /////
		//20091109 Change ----->
		//m_CurrSamplePos.lX = m_CurrPos.lX - m_SampleOriginCoord.lX;
		m_CurrSamplePos.lX = m_CurrPos.lX - m_SampleOriginCoord.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
		//20091109 Change <-----
		if(m_CurrSamplePos.lX != m_old_CurrSamplePos.lX){
			sprintf(szBuff, "%.3lf", (double)(m_CurrSamplePos.lX)/MICROMETRE);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//			SetDlgItemText(IDC_SAMPLE_X, szBuff);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
			if (TRUE != this->m_bProgress1PMeas) {SetDlgItemText(IDC_SAMPLE_X, szBuff);}
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
		}
		///// Y Axis /////
		//20091109 Change ----->
		//m_CurrSamplePos.lY = m_CurrPos.lY - m_SampleOriginCoord.lY;
		m_CurrSamplePos.lY = m_CurrPos.lY - m_SampleOriginCoord.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));
		//20091109 Change <-----
		if(m_CurrSamplePos.lY != m_old_CurrSamplePos.lY){
			sprintf(szBuff, "%.3lf", (double)(m_CurrSamplePos.lY)/MICROMETRE);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//			SetDlgItemText(IDC_SAMPLE_Y, szBuff);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
			if (TRUE != this->m_bProgress1PMeas) {SetDlgItemText(IDC_SAMPLE_Y, szBuff);}
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
		}
		///// Z Axis /////
		m_CurrSamplePos.lZ = m_CurrPos.lZ;
		if(m_CurrSamplePos.lZ != m_old_CurrSamplePos.lZ){
			sprintf(szBuff, "%.3lf", (double)(m_CurrSamplePos.lZ)/MICROMETRE/10);
			SetDlgItemText(IDC_SAMPLE_Z, szBuff);
		}
	}

	///// Renew /////
	m_old_CurrSamplePos.lX = m_CurrSamplePos.lX;
	m_old_CurrSamplePos.lY = m_CurrSamplePos.lY;
	m_old_CurrSamplePos.lZ = m_CurrSamplePos.lZ;

	// Reset the stage position marker
//	PostMessage(WM_PAINT, 0, 0);
	DrawStageView();
}

// ==========================================================================
//
void COverlayDialog::OnMouseMove(UINT nFlags, CPoint point)
{
// 2009.11.24 bagus Overlay �C�� --{--
//	if ( m_rcStage.PtInRect(point) )
	if (( m_rcStage.PtInRect(point) ) || ( m_rcOverlay.PtInRect(point) ))
// 2009.11.24 bagus Overlay �C�� --{--
		SetCursor (AfxGetApp()->LoadCursor(IDC_CROSS_1));
	else
		SetCursor(LoadCursor(NULL, IDC_ARROW)); // assumes arrow is default

	CDialog::OnMouseMove(nFlags, point);
}

// ==========================================================================
//
void COverlayDialog::OnDestroy()
{
BOOL bJoyStk;
BOOL result;

#if 0
	if( m_pMainFrame->GetJoyStickMode()!=0 ){
		// �n�[�h�W���C�X�e�B�b�N
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
#else
	if( !m_joyContinue ){
		// �I������ꍇ�́A�����I�Ƀ\�t�gJoy�֐؂�ւ���
		m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
#endif

	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageStop();
// DEL 2009.06.01
//			if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
		}
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageElevatorStop();
// DEL 2009.06.01
//			if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
		}
	}

	if( m_joyStick1 != NULL ){
		delete m_joyStick1;
		m_joyStick1 = NULL;
	}
	if( m_joyStick2 != NULL ){
		delete m_joyStick2;
		m_joyStick2 = NULL;
	}

// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
	if ( m_bbDC ) {
		m_bbDC->DeleteDC();
		delete m_bbDC;
		m_bbDC = NULL;
	}
	if ( m_bbBitmap ) {
		m_bbBitmap->DeleteObject();
		delete m_bbBitmap;
		m_bbBitmap = NULL;
	}
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--

// 2009.05.28 K.Matsuo SPT -->
	HookKeyUnhook();
// 2009.05.28 K.Matsuo SPT <--

	CDialog::OnDestroy();
}

// ==========================================================================
// Name�F		OnLButtonUp
void COverlayDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if(m_XY_UpButton.m_bPush		||
		m_XY_DownButton.m_bPush 	||
		m_XY_RightButton.m_bPush	||
		m_XY_LeftButton.m_bPush)
	{
//		TRACE("CPointSetupTeachingDlg::OnLButtonUp\n");
	}
	else if(m_Z_UpButton.m_bPush	||
			m_Z_DownButton.m_bPush)
	{
//		TRACE("CPointSetupTeachingDlg::OnLButtonUp - Z\n");
	}

	CDialog::OnLButtonUp(nFlags, point);
}

// ==========================================================================
//
void COverlayDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
STAGE_COORD TeachPos;
BOOL bJoyStk=TRUE;
BOOL result;
	//Saiki 20090530 Add ----->
	CString strMsg, strTitle;
	//Saiki 20090530 Add <-----
	if ( CCursor::m_bWaitCursor ) return;

//2009.12.15 bagus MS --{--
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
//2009.12.15 bagus MS --{--
	//���������L���łȂ��Ƃ��ɂ͂Ȃɂ������Ȃ�
	if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		//���쒆�͏���
		nexioMS_ModeLamp(FALSE);
	}
//2009.12.15 bagus MS --}--
//2009.12.15 bagus MS --}--

/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	if (TRUE == this->m_bProgress1PMeas) {
		return;
	}
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */

	if ( m_rcOverlay.PtInRect(point) ) {		// �摜�N���b�N�E�Z���^�[�ړ�

		// ���C���^�[���b�N������
		if( !OnLButtonDown_CheckIL() )			return;
		// ���C���^�[���b�N������

			if( !m_pDoc->JoyStickChangeDisable() ){
				//Saiki 20090601 Change ----->
				//MessageBox(	"���쒆�ł�",	"�x��", MB_OK);
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
				MessageBox(strMsg, strTitle, MB_OK);
				//Saiki 20090601 Change <-----
				// �K��JoyStick�̏�Ԃ�߂�����
				m_pDoc->JoyStickStatusRestore();
				return;
			}
		{
			ClientToStage(&point, &TeachPos);	// convert client point to chuck point

			CCursor::BeginWaitCursor();
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			if(!m_pMainFrame->HardwareSimulation()){
// 2013.01.09 bagus -->
//				result = StageMoveAbsolute(&TeachPos);
				result = NS_StageMoveAbsolute(&TeachPos);
// 2013.01.09 bagus <--
// DEL 2009.06.01
//				if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			CCursor::EndWaitCursor();
		}
		{	// �K��JoyStick�̏�Ԃ�߂�����
			m_pDoc->JoyStickStatusRestore();
		}
	}
	else if ( m_rcStage.PtInRect(point) ) { 	// �X�e�[�W�}�b�v�E�N���b�N�ړ�

		// ���C���^�[���b�N������
		if( !OnLButtonDown_CheckIL() )			return;
		// ���C���^�[���b�N������

			if( !m_pDoc->JoyStickChangeDisable() ){
				//Saiki 20090601 Change ----->
				//MessageBox(	"���쒆�ł�",	"�x��", MB_OK);
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
				MessageBox(strMsg, strTitle, MB_OK);
				//Saiki 20090601 Change <-----
				// �K��JoyStick�̏�Ԃ�߂�����
				m_pDoc->JoyStickStatusRestore();
				return;
			}
		{

// 2013.01.17 bagus rotate xy view -->
// 			///// �X�e�[�W�L���͈� /////
// 			double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
// 			double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
//
// 			long xfract = point.x - m_lAbsolutePos.x;
// 			long yfract = point.y - m_lAbsolutePos.y;
// 			//����
// 			TeachPos.lX = (dStageSizeX * xfract / m_rcStage.Width()) + (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
// 			TeachPos.lY = (dStageSizeY * -yfract / m_rcStage.Height()) + (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));

			///// �X�e�[�W�L���͈� /////
			double dStageSizeX;
			double dStageSizeY;

			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			case 180:
			default:
				dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
				dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
				break;
			case 90:
			case 270:
				dStageSizeX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
				dStageSizeY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
				break;
			}

			long xfract;
			long yfract;

			xfract = point.x - m_lAbsolutePos.x;
			yfract = point.y - m_lAbsolutePos.y;

			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				//����
				TeachPos.lX = (dStageSizeX * xfract / m_rcStage.Width()) + (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
				TeachPos.lY = (dStageSizeY * -yfract / m_rcStage.Height()) + (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));
				break;
			case 90:
				//�����ɂ́A0�x�̂Ƃ��̉E��
				TeachPos.lX = (dStageSizeY * -yfract / m_rcStage.Height()) + (-m_StageConfig.StageMaxCoord.dRightX - (m_StageConfig.Origin.lX * (-1)));
				TeachPos.lX = TeachPos.lX * -1;
				TeachPos.lY = (dStageSizeX * xfract / m_rcStage.Width()) + (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));
				break;
			case 180:
				//�����ɂ́A0�x�̂Ƃ��̉E��
				TeachPos.lX = (dStageSizeX * xfract / m_rcStage.Width()) + (-m_StageConfig.StageMaxCoord.dRightX - (m_StageConfig.Origin.lX * (-1)));
				TeachPos.lX = TeachPos.lX * -1;
				TeachPos.lY = (dStageSizeY * -yfract / m_rcStage.Height()) + (-m_StageConfig.StageMaxCoord.dUpY - (m_StageConfig.Origin.lY * (-1)));
				TeachPos.lY = TeachPos.lY * -1;
				break;
			case 270:
				//�����ɂ́A0�x�̂Ƃ��̍���
				TeachPos.lX = (dStageSizeY * -yfract / m_rcStage.Height()) + (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
				TeachPos.lY = (dStageSizeX * xfract / m_rcStage.Width()) + (-m_StageConfig.StageMaxCoord.dUpY - (m_StageConfig.Origin.lY * (-1)));
				TeachPos.lY = TeachPos.lY * -1;
				break;
			}
// 2013.01.17 bagus rotate xy view <--

			CCursor::BeginWaitCursor();
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			if(!m_pMainFrame->HardwareSimulation()){
				result = StageMoveAbsolute(&TeachPos);
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			CCursor::EndWaitCursor();

			// �n�[�h�V�~�����[�V�����Ȃ�
			if( m_pMainFrame->HardwareSimulation() ){
				m_CurrPos.lX=TeachPos.lX;
				m_CurrPos.lY=TeachPos.lY;
				// 2013.01.09 bagus -->
				m_CurrTeachPos = m_CurrPos;
				NS_ConvertToStageGetPosCoord(&m_CurrPos);
				// 2013.01.09 bagus <--
			}
		}
		{	// �K��JoyStick�̏�Ԃ�߂�����
			m_pDoc->JoyStickStatusRestore();
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

// ==========================================================================
// ���{�^���������ꂽ���̃C���^�[���b�N�̊m�F
BOOL COverlayDialog::OnLButtonDown_CheckIL()
{
	//Saiki 20090608 Add ----->
	CString strMsg;
	//Saiki 20090608 Add <-----
	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){			// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// �����e�i���X�ُ�
	}
	else{													// �ʏ퓮��
		if( nexioIsMaintenanceSwitch() != ON ){ 	AlarmIf_Set(ALID_MaintenanceSwitchOn); 	return FALSE;}	// �����e�i���X�E�X�C�b�`�E�I��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock()		== TRUE){							return FALSE;}
	if( nexioIsMovo2Alarm() 		!= OFF ){	AlarmIf_Set(ALID_StageError);		return FALSE;}	// MOVO�A���[�� ���k
	if(m_pDoc->CheckUnitStatus()	== TRUE){							return FALSE;}
	//Saiki 20090608 Change ----->
	if( m_pMainFrame->GetJoyStickMode()==JOY_SOFT_MODE && m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){	// �\�t�g�W���C
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return FALSE;
	}  // �X�e�[�W���쒆
	//Saiki 20090608 Change <-----
	if(m_pDoc->CheckActiveFlag() == TRUE){								return FALSE;}
	// ���C���^�[���b�N������

	return TRUE;
}

// ==========================================================================
//
void COverlayDialog::DrawLocationMarker(CONST POINT* lpPoint, BOOL bDrawColor)
{
	// if bDrawColor = TRUE, use blue, otherwise use overlay color
	COLORREF crDrawColor = bDrawColor ? RGB (0, 0, 255) : m_crBkColor;

	short w = 8;		// draw width

	POINT ptScreen = *lpPoint;	// make a copy of the input

	// draw a cross at the overlay center
	OverlayDrawLine(crDrawColor,
		ptScreen.x - w, ptScreen.y, ptScreen.x + w, ptScreen.y);
	OverlayDrawLine(crDrawColor,
		ptScreen.x, ptScreen.y - w, ptScreen.x, ptScreen.y + w);

	// incase the marker overlayed with the overlay center
	if ( !bDrawColor)
		OverlayDrawCenter();
}

// // ==========================================================================
// //
// void COverlayDialog::SetupStageMap(void)
// {
// 	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
// 	CRect StageRect;
// 	double dStageSizeX, dStageSizeY;
//
// 	///// Region Map /////
// 	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
// //	CDC* pDC = pPictBox->GetDC();
// 	CDC* pDC = m_bbDC;
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--
// 	pPictBox->GetClientRect(&StageRect);
//
// //	TRACE("COverlayDialog::SetupStageMap StageRect Befor %ld, %ld, %ld, %ld \n", StageRect.left, StageRect.top, StageRect.right, StageRect.bottom);
//
// 	///// Stage Rect Offset /////
// 	CPoint iPos;
// 	iPos = StageRect.CenterPoint();
// //	TRACE("COverlayDialog::SetupStageMap StageCenter %ld, %ld \n", iPos.x, iPos.y);
// 	m_iMaxSizeX = iPos.x - STAGE_MAP_OFFSET_X;
// 	m_iMaxSizeY = iPos.y - STAGE_MAP_OFFSET_Y;
//
// //	long StageRectLeft	 = iPos.x - m_iMaxSizeX;
// //	long StageRectTop	 = iPos.y - m_iMaxSizeY;
// //	long StageRectRight  = iPos.x + m_iMaxSizeX;
// //	long StageRectBottom = iPos.y + m_iMaxSizeY;
//
// //	TRACE("COverlayDialog::SetupStageMap StageRect After %ld, %ld, %ld, %ld \n", StageRectLeft, StageRectTop, StageRectRight, StageRectBottom);
//
// 	///// �X�e�[�W�L���͈� /////
// 	dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
// 	dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
//
//
// 	///// �X�e�[�W�S�͈� /////
// //	if((m_dStageMaxSizeX / m_dStageMaxSizeY) > (m_iMaxSizeX / m_iMaxSizeY)) {
// //		m_iMaxSizeY = m_iMaxSizeX * m_dStageMaxSizeY / m_dStageMaxSizeX;
// //	}
// //	else{
// //		m_iMaxSizeX = m_iMaxSizeY * m_dStageMaxSizeX / m_dStageMaxSizeY;
// //	}
// 	if((dStageSizeX / dStageSizeY) > (m_iMaxSizeX / m_iMaxSizeY)) {
// 		m_iMaxSizeY = m_iMaxSizeX * dStageSizeY / dStageSizeX;
// 	}
// 	else{
// 		m_iMaxSizeX = m_iMaxSizeY * dStageSizeX / dStageSizeY;
// 	}
// 	m_lDrawStartStagePosX = iPos.x - m_iMaxSizeX;
// 	m_lDrawStartStagePosY = iPos.y - m_iMaxSizeY;
// 	m_lDrawEndStagePosX   = iPos.x + m_iMaxSizeX;
// 	m_lDrawEndStagePosY   = iPos.y + m_iMaxSizeY;
//
// 	m_dStageMaxMap = CRect(m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX, m_lDrawEndStagePosY);
// //	TRACE("COverlayDialog::SetupStageMap MaxStage %ld, %ld, %ld, %ld \n", m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX, m_lDrawEndStagePosY);
//
// 	///// Draw Stage Color /////
// 	CBrush BrushStage1(BUTTON_COLOR);
// 	m_oldBrush = pDC->SelectObject(&BrushStage1);
// 	CPen PenStage1(PS_SOLID, 1, BUTTON_COLOR);
// 	m_oldPen = pDC->SelectObject(&PenStage1);
// 	pDC->Rectangle(m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX+1, m_lDrawEndStagePosY+1);
//
// //	///// �X�e�[�W�L���͈� /////
// //	dStageSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
// //	dStageSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
//
// 	m_iStageSizeX = m_iMaxSizeX;// * dStageSizeX / m_dStageMaxSizeX;
// 	m_iStageSizeY = m_iMaxSizeY;// * dStageSizeY / m_dStageMaxSizeY;
//
// 	long lDrawStartStagePosX = iPos.x - m_iStageSizeX;
// 	long lDrawStartStagePosY = iPos.y - m_iStageSizeY;
// 	long lDrawEndStagePosX	 = iPos.x + m_iStageSizeX;
// 	long lDrawEndStagePosY	 = iPos.y + m_iStageSizeY;
//
// 	m_dStageMap = CRect(lDrawStartStagePosX, lDrawStartStagePosY, lDrawEndStagePosX, lDrawEndStagePosY);
// //	TRACE("COverlayDialog::SetupStageMap Satge %ld, %ld, %ld, %ld \n", lDrawStartStagePosX, lDrawStartStagePosY, lDrawEndStagePosX, lDrawEndStagePosY);
//
// 	///// Draw Stage Color /////
// 	CBrush BrushStage2(BLACK_COLOR);
// 	pDC->SelectObject(&BrushStage2);
// 	CPen PenStage2(PS_SOLID, 1, BLACK_COLOR);
// 	pDC->SelectObject(&PenStage2);
// 	pDC->Rectangle(lDrawStartStagePosX, lDrawStartStagePosY, lDrawEndStagePosX+1, lDrawEndStagePosY+1);
//
// 	pDC->SelectObject(m_oldPen);
// 	pDC->SelectObject(m_oldBrush);
//
// 	// �����̃��W�b�N�ɍ��킹���ނ��߂ɍ��W��^����
// 	pPictBox->GetWindowRect(&m_rcStage);
// 	ScreenToClient(&m_rcStage);
//
// 	long lx = iPos.x - m_iStageSizeX;
// 	long ly = iPos.y - m_iStageSizeY;
//
// 	m_rcStage.DeflateRect( lx, ly);
// //	TRACE("COverlayDialog::SetupStageMap m_rcStage %ld, %ld, %ld, %ld \n", m_rcStage.left, m_rcStage.top, m_rcStage.right, m_rcStage.bottom);
//
// 	x_pixs_um = m_iMaxSizeX * 2 / dStageSizeX;
// 	y_pixs_um = m_iMaxSizeY * 2 / dStageSizeY;
//
// // 2009.05.25 K.Matsuo StageConfigSetting SPT -->
// 	// �I�t�Z�b�g���ꂽ���S���W�i�΃X�e�[�W�j
// 	// StageConfigSetting��m_StageConfig.Origin�̎戵���ɂ��Đ������Ă���
// 	//	 �C���O�FMachineOrgSensorLoc �i���W�I�{�^���ō����Ȃǂ��w��j�̊�ʒu����̃I�t�Z�b�g�l�B��ʒu�ɂ��A�I�t�Z�b�g�l�̉��Z�E���Z���s��
// 	//	 �C����FMachineOrgSensorLoc�́A�p�~�B������i�Œ�j�Ƃ��A�}�V�[���X�e�[�W���_���P�[�V�����̍��W�ʒu�Ƃ��ė��p����
// 	m_lCenterPos.x = iPos.x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
// 	m_lCenterPos.y = iPos.y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
//
// //	TRACE("COverlayDialog::SetupStageMap m_lCenterPos %ld, %ld \n", m_lCenterPos.x, m_lCenterPos.y);
//
// 	// �I�t�Z�b�g���ꂽ���S���W�i�΃E�B���h�j
// 	m_lAbsolutePos.x = m_rcStage.CenterPoint().x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
// 	m_lAbsolutePos.y = m_rcStage.CenterPoint().y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
//
// //	TRACE("COverlayDialog::SetupStageMap m_lAbsolutePos %ld, %ld \n", m_lAbsolutePos.x, m_lAbsolutePos.y);
// // 2009.05.25 K.Matsuo StageConfigSetting SPT <--
//
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
// //	pPictBox->ReleaseDC(pDC);
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--
// }

// ==========================================================================
//
void COverlayDialog::SetupStageMap(void)
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	CRect StageRect;
	double dStageSizeX, dStageSizeY;

	///// Region Map /////
	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
	CDC* pDC = m_bbDC;
	pPictBox->GetClientRect(&StageRect);

	///// Stage Rect Offset /////
	CPoint iPos;
	iPos = StageRect.CenterPoint();

	m_iMaxSizeX = iPos.x - STAGE_MAP_OFFSET_X;
	m_iMaxSizeY = iPos.y - STAGE_MAP_OFFSET_Y;

	///// �X�e�[�W�L���͈� /////
// 2013.01.17 bagus rotate xy view -->
// 	dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
// 	dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		break;
	case 90:
	case 270:
		dStageSizeX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		dStageSizeY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		break;
	}
// 2013.01.17 bagus rotate xy view <--

	///// �X�e�[�W�S�͈� /////
	if((dStageSizeX / dStageSizeY) > ((double)m_iMaxSizeX / (double)m_iMaxSizeY)) {
		m_iMaxSizeY = m_iMaxSizeX * dStageSizeY / dStageSizeX;
	}
	else{
		m_iMaxSizeX = m_iMaxSizeY * dStageSizeX / dStageSizeY;
	}
	m_lDrawStartStagePosX = iPos.x - m_iMaxSizeX;
	m_lDrawStartStagePosY = iPos.y - m_iMaxSizeY;
	m_lDrawEndStagePosX   = iPos.x + m_iMaxSizeX;
	m_lDrawEndStagePosY   = iPos.y + m_iMaxSizeY;

	m_dStageMaxMap = CRect(m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX, m_lDrawEndStagePosY);

	///// Draw Stage Color /////
	CBrush BrushStage1(BUTTON_COLOR);
	m_oldBrush = pDC->SelectObject(&BrushStage1);
	CPen PenStage1(PS_SOLID, 1, BUTTON_COLOR);
	m_oldPen = pDC->SelectObject(&PenStage1);
	pDC->Rectangle(m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX+1, m_lDrawEndStagePosY+1);

	///// �X�e�[�W�L���͈� /////
	m_iStageSizeX = m_iMaxSizeX;// * dStageSizeX / m_dStageMaxSizeX;
	m_iStageSizeY = m_iMaxSizeY;// * dStageSizeY / m_dStageMaxSizeY;

	long lDrawStartStagePosX = iPos.x - m_iStageSizeX;
	long lDrawStartStagePosY = iPos.y - m_iStageSizeY;
	long lDrawEndStagePosX	 = iPos.x + m_iStageSizeX;
	long lDrawEndStagePosY	 = iPos.y + m_iStageSizeY;

	m_dStageMap = CRect(lDrawStartStagePosX, lDrawStartStagePosY, lDrawEndStagePosX, lDrawEndStagePosY);

	///// Draw Stage Color /////
	CBrush BrushStage2(BLACK_COLOR);
	pDC->SelectObject(&BrushStage2);
	CPen PenStage2(PS_SOLID, 1, BLACK_COLOR);
	pDC->SelectObject(&PenStage2);
	pDC->Rectangle(lDrawStartStagePosX, lDrawStartStagePosY, lDrawEndStagePosX+1, lDrawEndStagePosY+1);

	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);

	// �����̃��W�b�N�ɍ��킹���ނ��߂ɍ��W��^����
	pPictBox->GetWindowRect(&m_rcStage);
	ScreenToClient(&m_rcStage);

	long lx = iPos.x - m_iStageSizeX;
	long ly = iPos.y - m_iStageSizeY;

	m_rcStage.DeflateRect( lx, ly);

	x_pixs_um = m_iMaxSizeX * 2 / dStageSizeX;
	y_pixs_um = m_iMaxSizeY * 2 / dStageSizeY;

// 2013.01.17 bagus rotate xy view -->
//	// �I�t�Z�b�g���ꂽ���S���W�i�΃X�e�[�W�j
//	m_lCenterPos.x = iPos.x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
//	m_lCenterPos.y = iPos.y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
//
//	// �I�t�Z�b�g���ꂽ���S���W�i�΃E�B���h�j
//	m_lAbsolutePos.x = m_rcStage.CenterPoint().x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
//	m_lAbsolutePos.y = m_rcStage.CenterPoint().y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));

	// �I�t�Z�b�g���ꂽ���S���W�i�΃X�e�[�W�j
	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		m_lCenterPos.x = iPos.x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
		m_lCenterPos.y = iPos.y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
		break;
	case 90:
		m_lCenterPos.x = iPos.x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lY - dStageSizeX/2)/(dStageSizeX/2));
		m_lCenterPos.y = iPos.y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lX - dStageSizeY/2)/(dStageSizeY/2));
		break;
	case 180:
		m_lCenterPos.x = iPos.x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
		m_lCenterPos.y = iPos.y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
		break;
	case 270:
		m_lCenterPos.x = iPos.x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lY - dStageSizeX/2)/(dStageSizeX/2));
		m_lCenterPos.y = iPos.y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lX - dStageSizeY/2)/(dStageSizeY/2));
		break;
	}

	// �I�t�Z�b�g���ꂽ���S���W�i�΃E�B���h�j
	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		m_lAbsolutePos.x = m_rcStage.CenterPoint().x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
		m_lAbsolutePos.y = m_rcStage.CenterPoint().y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
		break;
	case 90:
		m_lAbsolutePos.x = m_rcStage.CenterPoint().x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lY - dStageSizeX/2)/(dStageSizeX/2));
		m_lAbsolutePos.y = m_rcStage.CenterPoint().y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lX - dStageSizeY/2)/(dStageSizeY/2));
		break;
	case 180:
		m_lAbsolutePos.x = m_rcStage.CenterPoint().x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lX - dStageSizeX/2)/(dStageSizeX/2));
		m_lAbsolutePos.y = m_rcStage.CenterPoint().y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lY - dStageSizeY/2)/(dStageSizeY/2));
		break;
	case 270:
		m_lAbsolutePos.x = m_rcStage.CenterPoint().x + (long)(m_iMaxSizeX * (m_StageOriginCoord.lY - dStageSizeX/2)/(dStageSizeX/2));
		m_lAbsolutePos.y = m_rcStage.CenterPoint().y - (long)(m_iMaxSizeY * (m_StageOriginCoord.lX - dStageSizeY/2)/(dStageSizeY/2));
		break;
	}
// 2013.01.17 bagus rotate xy view <--
}

// ==========================================================================
//
void COverlayDialog::DrawGrid(void)
{
	///// Region Map /////
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
//	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
//	CDC* pDC = pPictBox->GetDC();
	CDC* pDC = m_bbDC;
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--

	DrawCenterMarker(&m_dStageMap, pDC);

// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
//	pPictBox->ReleaseDC(pDC);
// 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--
}

// ==========================================================================
//
void COverlayDialog::DrawCenterMarker(LPCRECT lpRect, CDC* pDC)
{
	// Draw Grid
	CRect rcClient;
	int iIntervalX = 50, iIntervalY = 50;;

//	iIntervalX = (int)(GRID_INTERVAL * x_pixs_um);
//	iIntervalY = (int)(GRID_INTERVAL * y_pixs_um);

	rcClient.CopyRect(lpRect);
	rcClient.left = m_lCenterPos.x;
	//DrawGridLine(X, 1, 0, &rcClient, pDC);
	DrawGridLine(X, 1, iIntervalX, &rcClient, pDC);
	rcClient.CopyRect(lpRect);
	rcClient.right = m_lCenterPos.x;
	DrawGridLine(X, -1, iIntervalX, &rcClient, pDC);

	rcClient.CopyRect(lpRect);
	rcClient.bottom = m_lCenterPos.y;
	DrawGridLine(Y, 1, iIntervalY, &rcClient, pDC);
	rcClient.CopyRect(lpRect);
	rcClient.top = m_lCenterPos.y;
	DrawGridLine(Y, -1, iIntervalY, &rcClient, pDC);


	// Draw a circle that represents the center of the wafer
	//
	CPen pen;
	pen.CreatePen(PS_SOLID, 0, RGB (0, 255, 0));
	CPen *pOldPen = pDC->SelectObject(&pen);

// ���S�j��
//	  long StageCenterX = m_lCenterPos.x - ((m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1))) * x_pixs_um);
//	  long StageCenterY = m_lCenterPos.y + ((m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1))) * y_pixs_um);
// �X�e�[�W���_�j��
/* modified 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
//	  long StageCenterX = m_lCenterPos.x + ((m_StageConfig.Origin.lX * (-1)) * x_pixs_um)- ((m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1))) * x_pixs_um);
//	  long StageCenterY = m_lCenterPos.y - ((m_StageConfig.Origin.lY * (-1)) * y_pixs_um)+ ((m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1))) * y_pixs_um);
/* modified 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ----------				*/
	STAGE_COORD l_StgCfgOrg;
	l_StgCfgOrg.lX = m_StageConfig.Origin.lX - m_HeadOffset.lX;
	l_StgCfgOrg.lY = m_StageConfig.Origin.lY - m_HeadOffset.lY;
// 2013.01.17 bagus rotate xy view <--
// 	long StageCenterX = m_lCenterPos.x + ((m_StageConfig.Origin.lX * (-1)) * x_pixs_um) - ((m_StageConfig.StageMaxCoord.dLeftX - (l_StgCfgOrg.lX * (-1))) * x_pixs_um);
// 	long StageCenterY = m_lCenterPos.y - ((m_StageConfig.Origin.lY * (-1)) * y_pixs_um) + ((m_StageConfig.StageMaxCoord.dDownY - (l_StgCfgOrg.lY * (-1))) * y_pixs_um);

	long StageCenterX;
	long StageCenterY;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		//����
		StageCenterX = m_lCenterPos.x + ((m_StageConfig.Origin.lX * (-1)) * x_pixs_um) - ((m_StageConfig.StageMaxCoord.dLeftX - (l_StgCfgOrg.lX * (-1))) * x_pixs_um);
		StageCenterY = m_lCenterPos.y - ((m_StageConfig.Origin.lY * (-1)) * y_pixs_um) + ((m_StageConfig.StageMaxCoord.dDownY - (l_StgCfgOrg.lY * (-1))) * y_pixs_um);
		break;
	case 90:
		//�����ɂ́A0�x�̂Ƃ��̉E��
		StageCenterX = m_lCenterPos.x + ((m_StageConfig.Origin.lY * (-1)) * x_pixs_um) - ((m_StageConfig.StageMaxCoord.dDownY - (l_StgCfgOrg.lY * (-1))) * x_pixs_um);
		StageCenterY = m_lCenterPos.y + ((m_StageConfig.Origin.lX * (-1)) * y_pixs_um) + ((-m_StageConfig.StageMaxCoord.dRightX - (l_StgCfgOrg.lX * (-1))) * y_pixs_um);
		break;
	case 180:
		//�����ɂ́A0�x�̂Ƃ��̉E��
		StageCenterX = m_lCenterPos.x - ((m_StageConfig.Origin.lX * (-1)) * x_pixs_um) - ((-m_StageConfig.StageMaxCoord.dRightX - (l_StgCfgOrg.lX * (-1))) * x_pixs_um);
		StageCenterY = m_lCenterPos.y + ((m_StageConfig.Origin.lY * (-1)) * y_pixs_um) + ((-m_StageConfig.StageMaxCoord.dUpY - (l_StgCfgOrg.lY * (-1))) * y_pixs_um);
		break;
	case 270:
		//�����ɂ́A0�x�̂Ƃ��̍���
		StageCenterX = m_lCenterPos.x - ((m_StageConfig.Origin.lY * (-1)) * x_pixs_um) - ((-m_StageConfig.StageMaxCoord.dUpY - (l_StgCfgOrg.lY * (-1))) * x_pixs_um);
		StageCenterY = m_lCenterPos.y - ((m_StageConfig.Origin.lX * (-1)) * y_pixs_um) + ((m_StageConfig.StageMaxCoord.dLeftX - (l_StgCfgOrg.lX * (-1))) * y_pixs_um);
		break;
	}
// 2013.01.17 bagus rotate xy view -->
/* modified 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */

	// Draw two cross dashed lines on wafer
	short length = 8;
	if( TRUE){
	}
//	  DrawDashLine(X, m_lCenterPos.x, +1, length, lpRect->right,  m_lCenterPos.y, 4, pDC);
//	  DrawDashLine(X, m_lCenterPos.x, -1, length, lpRect->left,   m_lCenterPos.y, 4, pDC);
//	  DrawDashLine(Y, m_lCenterPos.y, +1, length, lpRect->bottom, m_lCenterPos.x, 4, pDC);
//	  DrawDashLine(Y, m_lCenterPos.y, -1, length, lpRect->top,	  m_lCenterPos.x, 4, pDC);
	DrawDashLine(X, StageCenterX, +1, length, lpRect->right,  StageCenterY, 4, pDC);
	DrawDashLine(X, StageCenterX, -1, length, lpRect->left,   StageCenterY, 4, pDC);
	DrawDashLine(Y, StageCenterY, +1, length, lpRect->bottom, StageCenterX, 4, pDC);
	DrawDashLine(Y, StageCenterY, -1, length, lpRect->top,	  StageCenterX, 4, pDC);

	pDC->SelectObject(pOldPen);

}

// ==========================================================================
//
void COverlayDialog::DrawGridLine(WORD wAxis, int iInc, int iInterval, LPCRECT lpRect, CDC* pDC)
{
	int iBound;

	// Draw a circle that represents the center of the wafer
	//
	CPen pen(PS_SOLID, 0, WHITE_COLOR);
	CPen *pOldPen = pDC->SelectObject(&pen);

	if ( wAxis == X ) { 							// draw the line along x axis
		//�X�e�[�W���S��Grid��`��
		DrawDashLine(Y, lpRect->top, 1, 1, lpRect->bottom, lpRect->left, 1, pDC);
		iBound = iInc > 0 ? lpRect->left + iInterval : lpRect->right - iInterval;
		while ( iInc > 0 ? iBound <= lpRect->right :iBound >= lpRect->left ) {
			DrawDashLine(Y, lpRect->top, 1, 1, lpRect->bottom, iBound, 1, pDC);
//			TRACE("COverlayDialog::DrawGridLine Y= %d, %d, %d \n", lpRect->top, lpRect->bottom, iBound);
			iBound = iInc > 0 ? iBound + iInterval : iBound - iInterval;
		}
	}
	else {
		//�X�e�[�W���S��Grid��`��
		DrawDashLine(X, lpRect->left, 1, 1, lpRect->right, lpRect->bottom, 1, pDC);
		iBound = iInc > 0 ? lpRect->bottom - iInterval : lpRect->top + iInterval;
		while ( iInc > 0 ? iBound >= lpRect->top : iBound <= lpRect->bottom ) {
			DrawDashLine(X, lpRect->left, 1, 1, lpRect->right, iBound, 1, pDC);
//			TRACE("COverlayDialog::DrawGridLine X= %d, %d, %d \n", lpRect->left, lpRect->right, iBound);
			iBound = iInc > 0 ? iBound - iInterval : iBound + iInterval;
		}
	}
	pDC->SelectObject(pOldPen);
}

// ==========================================================================
//
void COverlayDialog::DrawDashLine( short Axis, int Start, short Inc, short Length, int Bound, int OtherAxisVal, int iEmptySpace, CDC *pDC )
{
	int End = Start + (Length - iEmptySpace / 2) * Inc;

	DrawOneLine(Axis, Start, End, OtherAxisVal, pDC);
	Start = End + iEmptySpace * Inc;

	BOOL Continue = Inc > 0 ? Start <= Bound : Start >= Bound;
	while ( Continue ) {
		End = Start + (Length * 2 - iEmptySpace) * Inc;
		if ( Inc > 0 )
			End = End < Bound ? End : Bound;
		else
			End = End > Bound ? End : Bound;

		DrawOneLine(Axis, Start, End, OtherAxisVal, pDC);
		Start = End + iEmptySpace * Inc;

		Continue = Inc > 0 ? Start <= Bound : Start >= Bound;
	}
}

// ==========================================================================
//
void COverlayDialog::DrawOneLine(short Axis, int Start, int End, int OtherAxisVal, CDC *pDC)
{
	if ( Axis == X ) {	// draw the line along x axis
		pDC->MoveTo(Start, OtherAxisVal);
		pDC->LineTo(End,	OtherAxisVal);
	}
	else {				// draw the line along y axis
		pDC->MoveTo(OtherAxisVal, Start);
		pDC->LineTo(OtherAxisVal, End);
	}
}

// ==========================================================================
//
void COverlayDialog::UpdateStagePosMarker ()
{
	if ( x_pixs_um == 0 || y_pixs_um == 0 )
		return; 									// error cannot be handled

	int x, y;
	///// �X�e�[�W�L���͈� /////

// 2013.01.09 bagus -->
//	// �����X�e�[�W��̌��݈ʒu�ŕ\�����邽�߂ɁA�ϊ�����
	STAGE_COORD_XYZ coordTemp;
	coordTemp = m_CurrPos;
	NS_ConvertToStageMoveCoord(&coordTemp);
// 2013.01.09 bagus <--

//	x = m_lCenterPos.x + (int)(m_CurrPos.lX * x_pixs_um + (double)0.5);
//	y = m_lCenterPos.y - (int)(m_CurrPos.lY * y_pixs_um + (double)0.5);
	//����

// 2013.01.17 bagus rotate xy view -->
// 	x = m_lCenterPos.x + (int)(coordTemp.lX * x_pixs_um) - ((m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1))) * x_pixs_um);
// 	y = m_lCenterPos.y - (int)(coordTemp.lY * y_pixs_um) + ((m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1))) * y_pixs_um);

	x = m_lCenterPos.x;
	y = m_lCenterPos.y;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		//����
		x = x + (int)(coordTemp.lX * x_pixs_um) - ((m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1))) * x_pixs_um);
		y = y - (int)(coordTemp.lY * y_pixs_um) + ((m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1))) * y_pixs_um); 
		break;
	case 90:
		//�����ɂ́A0�x�̂Ƃ��̉E��
		x = x + (int)(coordTemp.lY * x_pixs_um) - ((m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1))) * x_pixs_um);
		y = y + (int)(coordTemp.lX * y_pixs_um) + ((-m_StageConfig.StageMaxCoord.dRightX - (m_StageConfig.Origin.lX * (-1))) * y_pixs_um);
		break;
	case 180:
		//�����ɂ́A0�x�̂Ƃ��̉E��
		x = x - (int)(coordTemp.lX * x_pixs_um) - ((-m_StageConfig.StageMaxCoord.dRightX - (m_StageConfig.Origin.lX * (-1))) * x_pixs_um);
		y = y + (int)(coordTemp.lY * y_pixs_um) + ((-m_StageConfig.StageMaxCoord.dUpY - (m_StageConfig.Origin.lY * (-1))) * y_pixs_um); 
		break;
	case 270:
		//�����ɂ́A0�x�̂Ƃ��̍���
		x = x - (int)(coordTemp.lY * x_pixs_um) - ((-m_StageConfig.StageMaxCoord.dUpY - (m_StageConfig.Origin.lY * (-1))) * x_pixs_um);
		y = y - (int)(coordTemp.lX * y_pixs_um) + ((m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1))) * y_pixs_um);
		break;
	}
// 2013.01.17 bagus rotate xy view <--

	m_ptStageMarker.x = x; m_ptStageMarker.y = y;	// update position

//	TRACE("COverlayDialog::UpdateStagePosMarker %d, %d \n", m_ptStageMarker.x, m_ptStageMarker.y);

/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	if (TRUE == this->m_bProgress1PMeas) {
		m_ptStageMarker = m_ptOldStageMarker;
	}
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
	DrawMarker();									// draw new

	m_ptOldStageMarker = m_ptStageMarker;
}

// ==========================================================================
//	COverlayDialog::DrawMarker ()
//
//	Description:
//
void COverlayDialog::DrawMarker()
{
	///// Region Map /////
	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
	CDC* pDC = pPictBox->GetDC();

// 2009.06.11 K.Matsuo -->
	CRect drawRect(m_lDrawStartStagePosX, m_lDrawStartStagePosY, m_lDrawEndStagePosX, m_lDrawEndStagePosY);
	CRgn rgnClip;
	rgnClip.CreateRectRgn(drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
	pDC->SelectClipRgn(&rgnClip);
// 2009.06.11 K.Matsuo <--

	CPen pen;
//	pen.CreatePen(PS_SOLID, 0, BLUE_COLOR);
	pen.CreatePen(PS_SOLID, 0, RED_COLOR);
	CPen *pOldPen = pDC->SelectObject(&pen);

//	CBrush brush (BLUE_COLOR);						// define fill color
	CBrush brush (RED_COLOR);						// define fill color
	CBrush *OldBrush = pDC->SelectObject(&brush);		// make active

//	TRACE("COverlayDialog::DrawMarker %d, %d, %d, %d \n", m_dStageMap.left, m_dStageMap.top, m_dStageMap.right, m_dStageMap.bottom);

#define D 2
//	if ( m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x - D, m_ptStageMarker.y - D)) &&
//			m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x - D, m_ptStageMarker.y + D)) &&
//			m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x + D, m_ptStageMarker.y - D)) &&
//			m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x + D, m_ptStageMarker.y + D)) )
//	{
//		pDC->Ellipse ( m_ptStageMarker.x - D, m_ptStageMarker.y - D,
//						m_ptStageMarker.x + D+1, m_ptStageMarker.y + D+1 );
//	}
	if ( m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x, m_ptStageMarker.y)) &&
			m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x, m_ptStageMarker.y)) &&
			m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x, m_ptStageMarker.y)) &&
			m_dStageMap.PtInRect (CPoint(m_ptStageMarker.x, m_ptStageMarker.y)) )
	{
		pDC->Ellipse ( m_ptStageMarker.x - D, m_ptStageMarker.y - D,
						m_ptStageMarker.x + D+1, m_ptStageMarker.y + D+1 );
	}
#undef D

// 2009.06.11 K.Matsuo -->
	pDC->SelectClipRgn(NULL);
	rgnClip.DeleteObject();
// 2009.06.11 K.Matsuo <--

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(OldBrush);

	pPictBox->ReleaseDC(pDC);
}

// ==========================================================================
// ClientToStage
void COverlayDialog::ClientToStage(LPPOINT lpPoint, STAGE_COORD* pStage)
{
	POINT ptScreen = *lpPoint;	// make a copy of the input

	// map center from screen (0, 0) to screen(m_OverlayCenter.x, m_OverlayCenter.y)
	ptScreen.x -= m_rcOverlay.CenterPoint().x;
	ptScreen.y -= m_rcOverlay.CenterPoint().y;

	ptScreen.y *= -1;							// now invert the y axis

	// Now the point we have is in the logical coordinate: center as overlay center
	// and x point to the right and y point to the top. For stage, the overlay center
	// is the current stage position. The Distance between the logical point and the
	// overlay center multiply by video calibration factor plus the current stage position
	// will give us the stage coordinate of the input screen point
	PixelToMicron(&ptScreen, pStage);
}

// 2013.01.17 bagus rotate xy view -->
// ==========================================================================
//
long COverlayDialog::RotateXyAxisDir(long lDir)
{
	long lRet;

	lRet = lDir;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
		break;
	case 90:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_TOP:
			lRet = DIR_RIGHT;
			break;
		case DIR_LEFT:
			lRet = DIR_TOP;
			break;
		case DIR_BOTTOM:
			lRet = DIR_LEFT;
			break;
		default:
			break;
		}
		break;
	case 180:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_LEFT;
			break;
		case DIR_TOP:
			lRet = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			lRet = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			lRet = DIR_TOP;
			break;
		default:
			break;
		}
		break;
	case 270:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_TOP;
			break;
		case DIR_TOP:
			lRet = DIR_LEFT;
			break;
		case DIR_LEFT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_BOTTOM:
			lRet = DIR_RIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return lRet;
}
// 2013.01.17 bagus rotate xy view <--

// 2013.01.09 bagus -->
// �N���C�A���g�X�e�[�W���W�iUI��ʗp�j�ɕϊ�
// �J�����́AStageConfig�Őݒ肳�ꂽView�����Őݒu����Ă���
void COverlayDialog::ToClientXyStageCoord(STAGE_COORD* lpPoint)
{
// 2013.01.17 bagus rotate xy view -->
	long lDirX = RotateXyAxisDir(m_StageConfig.Dir.X);
	long lDirY = RotateXyAxisDir(m_StageConfig.Dir.Y);
// 2013.01.17 bagus rotate xy view <--

	// X: RIGHT
	if ( lDirX == DIR_RIGHT ) {
		switch ( lDirY ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: LEFT
	if ( lDirX == DIR_LEFT ) {
		lpPoint->lX *= -1;
		switch ( lDirY ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: TOP
	if ( lDirX == DIR_TOP ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lX = ptTemp.lY;
		switch ( lDirY ) {
		case DIR_RIGHT:
			lpPoint->lY = ptTemp.lX;
			break;
		case DIR_LEFT:
			lpPoint->lY = ptTemp.lX * -1;
			break;
		default:
			break;
		}
	}

	// X: BOTTOM
	if ( lDirX == DIR_BOTTOM ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lX = ptTemp.lY * -1;
		switch ( lDirY ) {
		case DIR_RIGHT:
			lpPoint->lY = ptTemp.lX;
			break;
		case DIR_LEFT:
			lpPoint->lY = ptTemp.lX * -1;
			break;
		default:
			break;
		}
	}
}

// �����W�iSTAGEAPI�p�j�ɕϊ�
void COverlayDialog::ToXyAxisCoord(STAGE_COORD* lpPoint)
{
	// X: RIGHT
	if ( m_StageConfig.Dir.X == DIR_RIGHT ) {
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: LEFT
	if ( m_StageConfig.Dir.X == DIR_LEFT ) {
		lpPoint->lX *= -1;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: TOP
	if ( m_StageConfig.Dir.X == DIR_TOP ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lY = ptTemp.lX;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_RIGHT:
			lpPoint->lX = ptTemp.lY;
			break;
		case DIR_LEFT:
			lpPoint->lX = ptTemp.lY * -1;
			break;
		default:
			break;
		}
	}

	// X: BOTTOM
	if ( m_StageConfig.Dir.X == DIR_BOTTOM ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lY = ptTemp.lX * -1;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_RIGHT:
			lpPoint->lX = ptTemp.lY;
			break;
		case DIR_LEFT:
			lpPoint->lX = ptTemp.lY * -1;
			break;
		default:
			break;
		}
	}
}
// 2013.01.09 bagus <--

// 2013.01.09 bagus -->
// ==========================================================================
// PixelToMicron
// void COverlayDialog::PixelToMicron(LPPOINT lpPoint, STAGE_COORD* pStage)
// {
// 	STAGE_COORD CurrPos;
// 	int SelTurret;
// 	double dx;
// 	double dy;
//
// 	StageGetPos(&CurrPos);
//
// 	///// get the video calibration factor /////
// 	SelTurret = StageGetCurLens();
// 	if(SelTurret < 0 || SelTurret >= SR_LENS_MAX) SelTurret = 0;
// 	dx = m_SrVideoCalib[SelTurret].dXPixelToMicron;
// 	dy = m_SrVideoCalib[SelTurret].dYPixelToMicron;
//
// 	///// calculate the absolute position of each point on stage /////
// 	pStage->lX = long(lpPoint->x * dx + (lpPoint->x >= 0 ? 0.5 : -0.5)) + CurrPos.lX;
// 	pStage->lY = long(lpPoint->y * dy + (lpPoint->y >= 0 ? 0.5 : -0.5)) + CurrPos.lY;
// }
void COverlayDialog::PixelToMicron(LPPOINT lpPoint, STAGE_COORD* pStage)
{
	STAGE_COORD CurrPos;
	int SelTurret;
	double dx;
	double dy;

	NS_StageGetPos(&CurrPos);

	///// get the video calibration factor /////
	SelTurret = StageGetCurLens();
	if(SelTurret < 0 || SelTurret >= SR_LENS_MAX) SelTurret = 0;
	dx = m_SrVideoCalib[SelTurret].dXPixelToMicron;
	dy = m_SrVideoCalib[SelTurret].dYPixelToMicron;

// 	///// calculate the absolute position of each point on stage /////
// 	pStage->lX = long(lpPoint->x * dx + (lpPoint->x >= 0 ? 0.5 : -0.5)) + CurrPos.lX;
// 	pStage->lY = long(lpPoint->y * dy + (lpPoint->y >= 0 ? 0.5 : -0.5)) + CurrPos.lY;

	STAGE_COORD XyViewNsClientCoord;

	XyViewNsClientCoord.lX = long(lpPoint->x * dx + (lpPoint->x >= 0 ? 0.5 : -0.5));
	XyViewNsClientCoord.lY = long(lpPoint->y * dy + (lpPoint->y >= 0 ? 0.5 : -0.5));

	ToClientXyStageCoord(&XyViewNsClientCoord);

	pStage->lX = XyViewNsClientCoord.lX + CurrPos.lX;
	pStage->lY = XyViewNsClientCoord.lY + CurrPos.lY;
}
// 2013.01.09 bagus <--

// =========================================================================
// StageToClient
void COverlayDialog::StageToClient(STAGE_COORD* pStage, LPPOINT lpPoint)
{
	// first convert logical coordinate to screen coordinate
	MicronToPixel(pStage, lpPoint);

	lpPoint->y *= -1;		// first invert the y axis

	// map logical center from screen(m_OverlayCenter.x, m_OverlayCenter.y) to screen (0, 0)
	// here we need to make sure that after the addition, the point values are still
	// within the interger range.
	long point_x = (long) m_rcOverlay.CenterPoint().x + lpPoint->x;
	long point_y = (long) m_rcOverlay.CenterPoint().y + lpPoint->y;

	point_x = point_x > INT_MAX ? INT_MAX : point_x;
	point_y = point_y > INT_MAX ? INT_MAX : point_y;

	lpPoint->x = point_x < INT_MIN ? INT_MIN : int(point_x);
	lpPoint->y = point_y < INT_MIN ? INT_MIN : int(point_y);

	lpPoint->x -= m_rcOverlay.left;
	lpPoint->y -= m_rcOverlay.top;
}

// // ==========================================================================
// // MicronToPixel
// void COverlayDialog::MicronToPixel(STAGE_COORD* pLogical, LPPOINT lpPoint)
// {
// 	STAGE_COORD CurrPos;
// 	int SelTurret;
// 	double dx;
// 	double dy;
//
// 	StageGetPos(&CurrPos);
//
// 	///// get the video calibration factor /////
// 	SelTurret = StageGetCurLens();
// 	if(SelTurret < 0 || SelTurret >= SR_LENS_MAX) SelTurret = 0;
// 	dx = m_SrVideoCalib[SelTurret].dXPixelToMicron;
// 	dy = m_SrVideoCalib[SelTurret].dYPixelToMicron;
//
// 	///// calculate the pixel value relative to overlay center /////
// 	double pilX = (pLogical->lX - CurrPos.lX) / dx;
// 	double pilY = (pLogical->lY - CurrPos.lY) / dy;
//
// 	///// we need to convert the double to int, so add 0.5 to round to the nearest int /////
// 	pilX = pilX >= 0 ? pilX + 0.5 : pilX - 0.5;
// 	pilY = pilY >= 0 ? pilY + 0.5 : pilY - 0.5;
//
// 	// since the x and y in POINT struct are integers, we need to make sure that
// 	// the converted value doesn't exceed the boundary of integer. It happens
// 	// when the stage value is too large (also depends on video calibration factor).
// 	pilX = pilX > INT_MAX ? INT_MAX : pilX;
// 	pilY = pilY > INT_MAX ? INT_MAX : pilY;
//
// 	lpPoint->x = int ( pilX < INT_MIN ? INT_MIN : pilX );
// 	lpPoint->y = int ( pilY < INT_MIN ? INT_MIN : pilY );
// }
//
// ==========================================================================
// MicronToPixel
void COverlayDialog::MicronToPixel(STAGE_COORD* pLogical, LPPOINT lpPoint)
{
	STAGE_COORD CurrPos;
	int SelTurret;
	double dx;
	double dy;

	NS_StageGetPos(&CurrPos);

	///// get the video calibration factor /////
	SelTurret = StageGetCurLens();
	if(SelTurret < 0 || SelTurret >= SR_LENS_MAX) SelTurret = 0;
	dx = m_SrVideoCalib[SelTurret].dXPixelToMicron;
	dy = m_SrVideoCalib[SelTurret].dYPixelToMicron;

	STAGE_COORD XyAxisCoord;

	XyAxisCoord.lX = pLogical->lX - CurrPos.lX;
	XyAxisCoord.lY = pLogical->lY - CurrPos.lY;

	ToXyAxisCoord(&XyAxisCoord);

	///// calculate the pixel value relative to overlay center /////
	double pilX = XyAxisCoord.lX / dx;
	double pilY = XyAxisCoord.lY / dy;

	///// we need to convert the double to int, so add 0.5 to round to the nearest int /////
	pilX = pilX >= 0 ? pilX + 0.5 : pilX - 0.5;
	pilY = pilY >= 0 ? pilY + 0.5 : pilY - 0.5;

	// since the x and y in POINT struct are integers, we need to make sure that
	// the converted value doesn't exceed the boundary of integer. It happens
	// when the stage value is too large (also depends on video calibration factor).
	pilX = pilX > INT_MAX ? INT_MAX : pilX;
	pilY = pilY > INT_MAX ? INT_MAX : pilY;

	lpPoint->x = int ( pilX < INT_MIN ? INT_MIN : pilX );
	lpPoint->y = int ( pilY < INT_MIN ? INT_MIN : pilY );
}

// 2009.05.28 K.Matsuo SPT -->
// ==========================================================================
//
LRESULT COverlayDialog::OnKeyHook(WPARAM wParam, LPARAM lParam)
{
	switch ( wParam ) {
	case VK_RIGHT:
	case VK_LEFT:
	case VK_UP:
	case VK_DOWN:
		if ( HIWORD(lParam) & KF_UP ) {
			CCursor::EndWaitCursor();
//			StageStop();
			PostMessage(WM_STAGE_MOVE_DONE, 0L, 0L);
		}
		break;
	default:
		return 0L;
	}

	return 0L;
}
// 2009.05.28 K.Matsuo SPT <--

// ==========================================================================
//
void COverlayDialog::OnOK()
{
	SetOperationLog("OK Button was push.");
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----
// 2009.05.16 k-matsuo
// �����[�g����m_lX, m_lY�̒l���ݒ肳�ꂸ��EndDialog�����o�O�Ή�
// �z�X�g���[�h�Ɋ֌W�Ȃ��C���^�[���b�N�������݂�悤�ɂ���
//
//	//�����[�g���͉������Ȃ�
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ���C���^�[���b�N������
/* modified 2009.06.18 hmenjo �I�[�o���C DLG OK �{�^���ُ� MSG ---------- } ---------- */
	/*//Saiki 20090520 Change <-----*/
	// �ǂꂩ�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	 // �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

	m_lX = m_CurrPos.lX;
	m_lY = m_CurrPos.lY;
// 2013.02.22 bagus Substrate thickness setting -->
	m_lZ = m_CurrPos.lZ;
// 2013.02.22 bagus Substrate thickness setting <--
// 2013.01.22 bagus stage origin offset -->
	m_lTeachPosX = m_CurrTeachPos.lX;
	m_lTeachPosY = m_CurrTeachPos.lY;
// 2013.01.22 bagus stage origin offset <--

	CDialog::OnOK();
}

// ==========================================================================
//
void COverlayDialog::OnCancel()
{
	SetOperationLog("CANCEL Button was push.");

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// ���C���^�[���b�N������

	CDialog::OnCancel();
}

// ==========================================================================
// �\�t�g�W���C�X�e�B�b�N�ɂ��蓮����i�w�x���j
// ==========================================================================
LRESULT COverlayDialog::OnDoStageMove(WPARAM wParam, LPARAM lParam)
{
STAGE_COORD Pos;
BOOL result;
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----
// 2009.05.28 K.Matsuo SPT -->
	if ( CCursor::m_bWaitCursor ) return 0L;
// 2009.05.28 K.Matsuo SPT <--

// 2009.05.16 k-matsuo
// �����[�g���ɑ���ł��Ȃ��o�O�Ή�
// �z�X�g���[�h�Ɋ֌W�Ȃ��C���^�[���b�N�������݂�悤�ɂ���
//
//	//�����[�g���͉������Ȃ�
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return 0L;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return 0L;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return 0L;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return 0L;} // MOVO�A���[�� 				���k
	if(m_pDoc->CheckUnitStatus() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return 0L;
		}	// �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/

	// ���C���^�[���b�N������

	short sign = 1;
	WORD wAxis;

	switch(wParam){
	case VK_DOWN:
		sign *= -1;
	case VK_UP:
		wAxis = Y;
		break;
	case VK_LEFT:
		sign *= -1;
	case VK_RIGHT:
		wAxis = X;
		break;
	default:
		return 0L;
	}

// 2013.01.11 bagus jog direction -->
	if(m_JogDirSetting.bSwapXyDir){
		if(wAxis == X){
			wAxis = Y;
		}
		else{
			wAxis = X;
		}
	}

	if(m_JogDirSetting.bChangeXSign){
		if(wAxis == X){
			sign *= -1;
		}
	}

	if(m_JogDirSetting.bChangeYSign){
		if(wAxis == Y){
			sign *= -1;
		}
	}
// 2013.01.11 bagus jog direction <--

	SetOperationLog("XY Stage Button was push.");

	m_XY_ButtonUseHistry = 1;		// XY Button Use

// 2009.05.28 K.Matsuo SPT -->
	UINT nMode = m_cbMotionStage.GetMode();

	CCursor::BeginWaitCursor();

	int MotionJog = GetMotionJog(wAxis, nMode);

	///// Load File /////
	STAGE_MOTION_SPEED_DATA l_StageMotionSpeedData[MAX_AXIS];
	ConfigFile_GetNanoSpecIni(&l_StageMotionSpeedData[0], CONFIG_FILE_STAGE_MOTION_SPEED);

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);

	if(!m_pMainFrame->HardwareSimulation()){
		do{
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if(m_pDoc->IsInterLock() == TRUE)
				break;
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			switch ( m_SystemConfig.nStageType ) {
			case STAGE_TYPE_SPT:
				if(nMode < STAGE_MOTION_JOG_LOW){
					// �ᑬ�E�����E����
					if ( StageIsIdle() ) {
						StageMoveAtSpeedEx(wAxis, (double)(MotionJog * sign)/MICROMETRE, FALSE);
						m_bIsMoveAtSpeed = TRUE;
						if ( lParam == 1 ) {
							// �L�[�{�[�h����́AWM_STAGE_MOVE_DONE�̎�M�Œ�~����
							return 0L;
						}
					}
				}else{
					// �W���O�i��j�E�W���O�i���j�E�W���O�i���j
					Pos.lX = Pos.lY = 0;
					switch ( wAxis ) {
					case X:
						Pos.lX = MotionJog * sign;
						break;
					case Y:
						Pos.lY = MotionJog * sign;
						break;
					}
					result = StageMoveRelativeAtSpeed(&Pos, (double)l_StageMotionSpeedData[wAxis].Speed[nMode-3]/MICROMETRE);
// DEL 2009.06.01
//					if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
				}
				break;
			default:
// 2009.07.15 K.Matsuo -->
				switch ( wAxis ) {
				case X:
					Pos.lX = MotionJog * sign;
					break;
				case Y:
					Pos.lY = MotionJog * sign;
					break;
				}
				result = StageMoveRelative(&Pos);
// 2009.07.15 K.Matsuo <--
// DEL 2009.06.01
//				if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
				break;
			}
			Sleep(200);
		}while(::GetAsyncKeyState(VK_LBUTTON) < 0);

		if ( m_SystemConfig.nStageType == STAGE_TYPE_SPT ) {
			if ( m_bIsMoveAtSpeed ) {
				// �}�E�X����́A�����Œ�~������
				StageStop();
				m_bIsMoveAtSpeed = FALSE;
			}
		}
	}
	else{
		switch(wAxis){
		case X:
			m_CurrPos.lX += MotionJog * sign;
			break;
		case Y:
			m_CurrPos.lY += MotionJog * sign;
			break;
		}
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	CCursor::EndWaitCursor();

// 2009.05.28 K.Matsuo SPT <--

	return 0L;
}

// ==========================================================================
//
LRESULT COverlayDialog::OnStageMoveDone(WPARAM wParam, LPARAM lParam)
{
// 2009.05.28 K.Matsuo SPT -->
//BOOL result;

	if ( m_SystemConfig.nStageType != STAGE_TYPE_SPT ) return 0L;
	if ( m_cbMotionStage.GetMode() >= STAGE_MOTION_JOG_LOW ) return 0L;
	if ( m_pMainFrame->HardwareSimulation() ) return 0L;
//	if ( m_pMainFrame->GetJoyStickMode() != 0 ) return 0L;

	if ( m_bIsMoveAtSpeed ) {
		StageStop();
		m_bIsMoveAtSpeed = FALSE;
//		 if( result ){
//			TRACE("COverlayDialog::OnStageMoveDone StageStop\n");
//		  }
//		  else{
//			  AlarmIf_Set(ALID_SubControllerReplayAbnormal);
//		}
	}
// 2009.05.28 K.Matsuo SPT <--

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);

	CCursor::EndWaitCursor();

	return 0L;
}

// ==========================================================================
// �\�t�g�W���C�X�e�B�b�N�ɂ��蓮����i�y���j
// ==========================================================================
LRESULT COverlayDialog::OnDoElevatorMove(WPARAM wParam, LPARAM lParam)
{
BOOL result;
short sign = 1;
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----
// 2009.05.16 k-matsuo
// �����[�g���ɑ���ł��Ȃ��o�O�Ή�
// �z�X�g���[�h�Ɋ֌W�Ȃ��C���^�[���b�N�������݂�悤�ɂ���
//
//	//�����[�g���͉������Ȃ�
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return 0L;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return 0L;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return 0L;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// �V���b�^�[�ȊO�������Ă����瓮��֎~

	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return 0L;
		}	// �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return 0L;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

	switch ( wParam ) {
	case VK_DOWN:
		sign *= -1;
	case VK_UP:
		break;
	default:
		return 0L;
	}

	SetOperationLog("Z Axis Button was push.");

	m_Z_ButtonUseHistry  = 1;		// Z Button Use
	m_AutoFocusUseHistry = 0;		// AutoFocus Not Use

// 2009.05.28 K.Matsuo SPT -->
	UINT nElevMode = m_cbMotionElev.GetElevMode();

	CCursor::BeginWaitCursor();

	int MotionJog = GetMotionJog(Z, nElevMode);

	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);

	if(!m_pMainFrame->HardwareSimulation()){
		do{
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			if(m_pDoc->IsInterLock() == TRUE)
				break;
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
			result = StageElevatorMoveRelative(MotionJog * sign);
// DEL 2009.06.01
//			if( !result ) AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
			Sleep(200);
		}while( ::GetAsyncKeyState(VK_LBUTTON) < 0);
	}
	else{
		m_CurrPos.lZ += MotionJog * sign;
	}

	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	CCursor::EndWaitCursor();

// 2009.05.28 K.Matsuo SPT <--

	return 0L;
}

// ==========================================================================
//
BOOL COverlayDialog::PreTranslateMessage(MSG* pMsg)
{
// 2009.05.28 K.Matsuo SPT -->
//	if( pMsg->hwnd == m_hWnd ){
	if ( pMsg->message == WM_KEYDOWN ) {
		switch ( pMsg->wParam ) {
		case VK_DOWN:
		case VK_UP:
		case VK_LEFT:
		case VK_RIGHT:
			// H/W�����Ȃ�
			if( m_pMainFrame->GetJoyStickMode() == 0 ){
//				if ( m_SystemConfig.nStageType == STAGE_TYPE_SPT ) {
//					if ( !CCursor::m_bWaitCursor ) {
//						CCursor::BeginWaitCursor();
//						PostMessage(WM_DO_STAGE_MOVE, pMsg->wParam, 0L);
						PostMessage(WM_DO_STAGE_MOVE, pMsg->wParam, 1L);
						TRACE("COverlayDialog::PreTranslateMessage WM_KEYDOWN\n");
//					}
					return TRUE;
//				}
			}
			break;
		default:
			break;
		}
	}
//	  else if ( pMsg->message == WM_KEYUP ) {
//		  switch ( pMsg->wParam ) {
//		  case VK_DOWN:
//		  case VK_UP:
//		  case VK_LEFT:
//		  case VK_RIGHT:
//			  // H/W�����Ȃ�
//			  if( m_pMainFrame->GetJoyStickMode() == 0 ){
//				  PostMessage(WM_STAGE_MOVE_DONE, pMsg->wParam, 0L);
//				TRACE("COverlayDialog::PreTranslateMessage WM_KEYUP\n");
//				  return TRUE;
//			  }
//			  break;
//		  default:
//			  break;
//		  }
//	  }
//	}
// 2009.05.28 K.Matsuo SPT <--

	return CDialog::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
void COverlayDialog::OnSpecifyCoord()
{

CSpecifyCoordinateDlg dlg;

STAGE_COORD Position;
BOOL bJoyStk=FALSE;
BOOL result;
	//Saiki 20090530 Add ----->
	CString strMsg, strTitle;
	//Saiki 20090530 Add <-----



	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){	// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{											// �ʏ퓮��
		if( nexioIsMaintenanceSwitch() != ON ){ 	AlarmIf_Set(ALID_MaintenanceSwitchOn); 	return;}	// �����e�i���X�E�X�C�b�`�E�I��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock()		== TRUE){							return;}
	if( nexioIsMovo2Alarm() 		!= OFF ){		AlarmIf_Set(ALID_StageError);	return;}	// MOVO�A���[�� ���k
	if(m_pDoc->CheckUnitStatus()	== TRUE){							return;}
	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){								return;}
	// ���C���^�[���b�N������

	if( !m_pDoc->JoyStickChangeDisable() ){
		//Saiki 20090603 Change ----->
		//MessageBox( "���쒆�ł�", "�x��", MB_OK);
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		//Saiki 20090603 Change <-----
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	SetOperationLog("COORDINATE Button was push.");
	TRACE("COverlayDialog::OnSpecifyCoord()�֐��� m_nSampleMode:%d\n", m_nSampleMode);
	dlg.m_nSampleMode = m_nSampleMode;
//OkabeAdd20090612--->
	if ( m_nSampleMode != 0 ){
		dlg.m_dSampleSizeX = m_dSampleSizeX;
		dlg.m_dSampleSizeY = m_dSampleSizeY;
	}
//OkabeAdd20090612<---
	if( dlg.DoModal() == IDOK ){
		switch ( dlg.m_iMoveMode ) {
		case SPECIFY_COORDINATE_STAGE:

			Position.lX = dlg.m_lX;
			Position.lY = dlg.m_lY;
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
			if(!m_pMainFrame->HardwareSimulation()){
				result = NS_StageMoveAbsolute(&Position);
			}
			else{
				m_CurrPos.lX = Position.lX;
				m_CurrPos.lY = Position.lY;
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			break;

		case SPECIFY_COORDINATE_SAMPLE:

			if (!(m_nSampleMode ==0)){
				Position.lX = dlg.m_lX + m_SampleOriginCoord.lX + (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
				Position.lY = dlg.m_lY + m_SampleOriginCoord.lY + (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				if(!m_pMainFrame->HardwareSimulation()){
					result = NS_StageMoveAbsolute(&Position);
				}
				else{
					m_CurrPos.lX = Position.lX;
					m_CurrPos.lY = Position.lY;
				}
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
			break;
		default:
			break;
		}
	}
	{	// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
	}

}

// ==========================================================================
//
void COverlayDialog::OnJogSetting()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	//Saiki 20090601 Change ----->
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) )	{
		LoadStringML(IDS_Z_AXIS_WORKING, strMsg, "Z Axis is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // �y�����쒆
	//Saiki 20090601 Change <-----
	// ���C���^�[���b�N������

	SetOperationLog("JOG SETTING Button was push.");

	if(m_SystemConfig.nStageType == STAGE_TYPE_SPT){
		CSpeedSettingDlg dlg;

		if( dlg.DoModal() == IDOK ){
			///// Low X /////
			m_StageMotionSpeedData[X].Speed[STAGE_MOTION_LOW] = dlg.m_StageMotionSpeedData[X].Speed[STAGE_MOTION_LOW];
			///// Low Y /////
			m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_LOW] = dlg.m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_LOW];
			///// Mid X /////
			m_StageMotionSpeedData[X].Speed[STAGE_MOTION_MID] = dlg.m_StageMotionSpeedData[X].Speed[STAGE_MOTION_MID];
			///// Mid Y /////
			m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_MID] = dlg.m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_MID];
			///// High X /////
			m_StageMotionSpeedData[X].Speed[STAGE_MOTION_HIGH] = dlg.m_StageMotionSpeedData[X].Speed[STAGE_MOTION_HIGH];
			///// High Y /////
			m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_HIGH] = dlg.m_StageMotionSpeedData[Y].Speed[STAGE_MOTION_HIGH];
			///// Jog X /////
			m_StageMotionSpeedData[X].Jog = dlg.m_StageMotionSpeedData[X].Jog;
			///// Jog Y /////
			m_StageMotionSpeedData[Y].Jog = dlg.m_StageMotionSpeedData[Y].Jog;
		}
	}else{
		CJogSettingDlg dlg;

		if( dlg.DoModal() == IDOK ){
			///// Low X /////
			m_StageMotionJogData[X].Jog[STAGE_MOTION_LOW] = dlg.m_StageMotionJogData[X].Jog[STAGE_MOTION_LOW];
			///// Low Y /////
			m_StageMotionJogData[Y].Jog[STAGE_MOTION_LOW] = dlg.m_StageMotionJogData[Y].Jog[STAGE_MOTION_LOW];
			///// Mid X /////
			m_StageMotionJogData[X].Jog[STAGE_MOTION_MID] = dlg.m_StageMotionJogData[X].Jog[STAGE_MOTION_MID];
			///// Mid Y /////
			m_StageMotionJogData[Y].Jog[STAGE_MOTION_MID] = dlg.m_StageMotionJogData[Y].Jog[STAGE_MOTION_MID];
			///// High X /////
			m_StageMotionJogData[X].Jog[STAGE_MOTION_HIGH] = dlg.m_StageMotionJogData[X].Jog[STAGE_MOTION_HIGH];
			///// High Y /////
			m_StageMotionJogData[Y].Jog[STAGE_MOTION_HIGH] = dlg.m_StageMotionJogData[Y].Jog[STAGE_MOTION_HIGH];
		}
	}
}

// ==========================================================================
//
void COverlayDialog::OnAutoFocus()
{
// 2009.05.16 k-matsuo
// �����[�g���ɑ���ł��Ȃ��o�O�Ή�
// �z�X�g���[�h�Ɋ֌W�Ȃ��C���^�[���b�N�������݂�悤�ɂ���
//
//	//�����[�g���͉������Ȃ�
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	//Saiki 20090530 Add ----->
	CString strMsg, strTitle;
	//Saiki 20090530 Add <-----
	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// �ǂꂩ�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	//Saiki 20090601 Change ----->
	if( m_pDoc->ActuateFlagsGet(ACTUATE_SHUTTER) )	{
		LoadStringML(IDS_LOAD_SHUTTER_WORKING, strMsg, "Sample Loading Shutter is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // �V���b�^�[���쒆
	//Saiki 20090601 Change <-----
	// ���C���^�[���b�N������

//	if ( !CCursor::m_bWaitCursor ) {

		m_AutoFocusUseHistry = 1;		// AutoFocus Use
		m_Z_ButtonUseHistry  = 0;		// Z Button Not Use

		CCursor::BeginWaitCursor();
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
		if(!m_pMainFrame->HardwareSimulation()){
/* modified 2009.09.08 hmenjo AF ���������Y�ؑ֒u�� ---------- { ---------- */
//			if( StageDoAutoFocus()==FALSE ){
/* modified 2009.09.08 hmenjo AF ���������Y�ؑ֒u�� ----------				*/
			if (FALSE == NS_StageDoAutoFocus()) {
/* modified 2009.09.08 hmenjo AF ���������Y�ؑ֒u�� ---------- } ---------- */
				m_AutoFocusResult	 = 1;	// AutoFocus Result NG
				//Saiki 20090603 Change ----->
//				MessageBox("Auto Focus Fail !!!.", "AUTO FOCUS", MB_OK|MB_ICONWARNING);
				//MessageBox("�I�[�g�t�H�[�J�X���s !!!.", "AUTO FOCUS", MB_OK|MB_ICONWARNING);
				LoadStringML(IDS_AUTOFOCUS_FAILED, strMsg, "Auto Focus Fail !!!.");
				LoadStringML(IDS_TITLE_AUTO_FOCUS, strTitle, "AUTO FOCUS");
				MessageBox(strMsg, strTitle, MB_OK|MB_ICONWARNING);
				//Saiki 20090603 Change <-----
			}
			else{
				m_AutoFocusResult	 = 0;	// AutoFocus Result OK
				SetOperationLog("AUTO FOCUS Button was push.");
			}
		}
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
		CCursor::EndWaitCursor();

//	}
}

// ==========================================================================
//
void COverlayDialog::OnCapture()
{
	RECT rc;
	HDC hDC, hDCCompat;
	HBITMAP hBitmap, hBitmapPrev;
	BITMAPINFO *pbmi;
	void *pbits;
	BITMAPFILEHEADER bmfh;
	HANDLE hfile;
	DWORD tmp;
	//Saiki 20090601 Add ----->
	CString strMsg;
	//Saiki 20090601 Add <-----

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090601 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
		//Saiki 20090601 Change <-----
	}
	//Saiki 20090601 Change ----->
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS)){
		LoadStringML(IDS_Z_AXIS_WORKING, strMsg, "Z Axis is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // �y�����쒆
	//Saiki 20090601 Change <-----
	// ���C���^�[���b�N������

//	CCursor::BeginWaitCursor();

	SetOperationLog("CAPTURE Button was push.");

	OverlayPreview(FALSE);		// ��U��ʂ�����

	OverlayPreview(TRUE);		// ��ʂ��ĕ`�悷��

	//�t�@�C�������擾����
	CString m_csFileName;

#if 1
	CSaveAsDlg SaveDlg(SAVE_AS_OVERLAY_CAPTURE);

	if ( SaveDlg.DoModal() == IDOK ) {
		CCursor::BeginWaitCursor();

		m_csFileName = SaveDlg.m_szFilePath;
#else
//	CFileDialog cDlg( FALSE, "bmp", "*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR );
//	if( cDlg.DoModal() == IDOK )
//	{
//		m_csFileName = cDlg.GetPathName();
#endif
		hfile = CreateFile(m_csFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
									CREATE_ALWAYS, 0, NULL);

		Sleep(1000);			 // �����҂�

		//�r�b�g�}�b�v�C���t�H���쐬����B
		HWND hWnd = GetDlgItem(IDC_OVERLAY)->m_hWnd;
		hDC = ::GetDC(hWnd);
		GetDlgItem(IDC_OVERLAY)->GetWindowRect(&rc);
		ScreenToClient(&rc);

		pbmi = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256];
		ZeroMemory(&pbmi->bmiHeader, sizeof(pbmi->bmiHeader));
		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbmi->bmiHeader.biWidth = rc.right-rc.left;
		pbmi->bmiHeader.biHeight = rc.bottom-rc.top;
		pbmi->bmiHeader.biBitCount = GetDeviceCaps(hDC, BITSPIXEL);
		pbmi->bmiHeader.biPlanes = 1;
		pbmi->bmiHeader.biSizeImage =
				(((pbmi->bmiHeader.biWidth
					* pbmi->bmiHeader.biBitCount+31)>>3)	//�r�b�g���o�C�g�B
						& 0xfffffffc)						//�S�o�C�g���E�B
							* pbmi->bmiHeader.biHeight;
		//DDB ���쐬���ăE�C���h�E����r�b�g�}�b�v���R�s�[�B
		hBitmap = CreateCompatibleBitmap(hDC, rc.right-rc.left, rc.bottom-rc.top);
		hDCCompat = CreateCompatibleDC(hDC);
		hBitmapPrev = (HBITMAP)SelectObject(hDCCompat, hBitmap);
		BitBlt(hDCCompat, 0, 0, pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight,
					hDC, rc.left-OVERLAY_VIEW_OFFSET_X-1, rc.top-OVERLAY_VIEW_OFFSET_Y-1, SRCCOPY);
		SelectObject(hDCCompat, hBitmapPrev);
		//�����������蓖�Ăăr�b�g�}�b�v����f�[�^���擾�B
		pbits = new BYTE[pbmi->bmiHeader.biSizeImage];
		GetDIBits(hDC, hBitmap, 0, pbmi->bmiHeader.biHeight, pbits, pbmi, DIB_RGB_COLORS);
		::ReleaseDC(hWnd, hDC);

		//�r�b�g�}�b�v�t�@�C���w�b�_�[���쐬�B
		bmfh.bfType = 'B'+('M'<<8);
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
		if (pbmi->bmiHeader.biBitCount <= 8) {
			bmfh.bfOffBits += sizeof(COLORREF) * (1 << pbmi->bmiHeader.biBitCount);
		}
		bmfh.bfSize = bmfh.bfOffBits+pbmi->bmiHeader.biSizeImage;

		//�f�[�^���t�@�C���փZ�[�u�B
		WriteFile(hfile, &bmfh, sizeof(bmfh), &tmp, NULL);
		WriteFile(hfile, &pbmi->bmiHeader, sizeof(pbmi->bmiHeader), &tmp, NULL);
		if (pbmi->bmiHeader.biBitCount <= 8) {
			WriteFile(hfile,
						pbmi->bmiColors,
						sizeof(COLORREF) * (1 << pbmi->bmiHeader.biBitCount),
						&tmp, NULL);
		}
		SetFilePointer(hfile, bmfh.bfOffBits, NULL, FILE_BEGIN);
		WriteFile(hfile, pbits, pbmi->bmiHeader.biSizeImage, &tmp, NULL);
		CloseHandle(hfile);

		//�Еt��
		delete[] (BYTE*)pbmi;
		delete[] (BYTE*)pbits;

		CCursor::EndWaitCursor();
	}

	OverlayDrawCenter();		// �O���b�h���ĕ`�悷��

//	CCursor::EndWaitCursor();
}

// ==========================================================================
//
void COverlayDialog::SetCaption(LPCTSTR pszCaption)
{
	m_sCaption = pszCaption;
}

// ==========================================================================
//
void COverlayDialog::SetMessage(LPCTSTR pszMessage)
{
	m_sMessage = pszMessage;
}

// ==========================================================================
//
void COverlayDialog::SetFileName(LPCTSTR pszFileName)
{
	m_sFileName = pszFileName;
}

// =========================================================================
//
void COverlayDialog::GetSampleData()
{
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	BOOL bLoop;

	m_iSampleMax = 0;
	if( m_nSampleMode==0 ){
		sprintf(szDirPath, "%s*%s", g_szCfg_System_Sample_Dir, DAT_EXT);
		bLoop = Finder.FindFile(szDirPath);
		while(bLoop){
			bLoop = Finder.FindNextFile();
			if(Finder.IsDots())
				continue;

		// �t�@�C�������擾
		sprintf(m_sSampleName[m_iSampleMax++], "%s", (LPCSTR)Finder.GetFileTitle());

		}
	}
}

// ==========================================================================
//
void COverlayDialog::SetupSampleMap(void)
{
	SAMPLEINFO	SampleInfo;
	int 		i;

	if ( m_nSampleMode == 1 ){
		///// Sample /////
		if ( !RecipeFile_LoadRecipe(&SampleInfo, m_sFileName, RECIPE_FILE_SAMPLE) ) {
		}
		else {
			// �T���v���̕`��
			// 2013.01.09 bagus -->
			// m_dSampleSizeX	 = SampleInfo.Size.dx;
			// m_dSampleSizeY	 = SampleInfo.Size.dy;
			if ( m_StageConfig.Dir.X == DIR_RIGHT ||
				m_StageConfig.Dir.X == DIR_LEFT ) {
			    m_dSampleSizeX	 = SampleInfo.Size.dx;
			    m_dSampleSizeY	 = SampleInfo.Size.dy;
			}
			else {
			    m_dSampleSizeX	 = SampleInfo.Size.dy;
			    m_dSampleSizeY	 = SampleInfo.Size.dx;
			}
			// 2013.01.09 bagus <--
//			  m_SampleOriginCoord.lX = SampleInfo.Origin.lX;
//			  m_SampleOriginCoord.lY = SampleInfo.Origin.lY;
			// 2013.01.09 bagus -->
			m_SampleOriginCoord.lX = SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
			m_SampleOriginCoord.lY = SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));

			STAGE_COORD XyAxisCoord;
			STAGE_COORD MapDrawSampleOriginCoord;

			XyAxisCoord.lX = SampleInfo.Origin.lX;
			XyAxisCoord.lY = SampleInfo.Origin.lY;
			ToXyAxisCoord(&XyAxisCoord);

			MapDrawSampleOriginCoord.lX = XyAxisCoord.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
			MapDrawSampleOriginCoord.lY = XyAxisCoord.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));
			// 2013.01.09 bagus <--
			DrawSampleMap(MapDrawSampleOriginCoord.lX, MapDrawSampleOriginCoord.lY);
		}
	}
	else{
		for( i=0; i<m_iSampleMax; i++ ){
			///// Sample /////
			if ( !RecipeFile_LoadRecipe(&SampleInfo, m_sSampleName[i], RECIPE_FILE_SAMPLE) ) {
			}
			else {
				// �T���v���̕`��
				// 2013.01.09 bagus -->
//				m_dSampleSizeX	 = SampleInfo.Size.dx;
//				m_dSampleSizeY	 = SampleInfo.Size.dy;
				if ( m_StageConfig.Dir.X == DIR_RIGHT ||
					m_StageConfig.Dir.X == DIR_LEFT ) {
					m_dSampleSizeX	 = SampleInfo.Size.dx;
					m_dSampleSizeY	 = SampleInfo.Size.dy;
				}
				else {
					m_dSampleSizeX	 = SampleInfo.Size.dy;
					m_dSampleSizeY	 = SampleInfo.Size.dx;
				}
				m_SampleOriginCoord.lX = SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
				m_SampleOriginCoord.lY = SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));

				STAGE_COORD XyAxisCoord;
				STAGE_COORD MapDrawSampleOriginCoord;

				XyAxisCoord.lX = SampleInfo.Origin.lX;
				XyAxisCoord.lY = SampleInfo.Origin.lY;
				ToXyAxisCoord(&XyAxisCoord);

				MapDrawSampleOriginCoord.lX = XyAxisCoord.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)));
				MapDrawSampleOriginCoord.lY = XyAxisCoord.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)));
				// 2013.01.09 bagus <--
				DrawSampleMap(MapDrawSampleOriginCoord.lX, MapDrawSampleOriginCoord.lY);
			}
		}
	}

}

// // ==========================================================================
// //
// // 2013.01.09 bagus -->
// // void COverlayDialog::DrawSampleMap(void)
// void COverlayDialog::DrawSampleMap(long lMapDrawSampleOriginCoordX, long lMapDrawSampleOriginCoordY)
// // 2013.01.09 bagus <--
// {
// 	///// �X�e�[�W�L���͈� /////
// 	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
// 	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
//
// 	// �T���v���̃T�C�Y
// 	long lDrawSampleSizeX = m_iMaxSizeX * m_dSampleSizeX / dStageSizeX;
// 	long lDrawSampleSizeY = m_iMaxSizeY * m_dSampleSizeY / dStageSizeY;
// //	TRACE("COverlayDialog::DrawSampleMap SampleSize = %ld, %ld \n", lDrawSampleSizeX, lDrawSampleSizeY);
//
// 	// �X�e�[�W���_����̈ړ���
// /* modified 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
// //	  long lX_Offset = m_iMaxSizeX * 2 * m_SampleOriginCoord.lX / dStageSizeX;
// //	  long lY_Offset = m_iMaxSizeY * 2 * m_SampleOriginCoord.lY / dStageSizeY;
// /* modified 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ----------				*/
// // 2013.01.09 bagus -->
// // 	long lX_Offset = m_iMaxSizeX * 2 * (m_SampleOriginCoord.lX + m_HeadOffset.lX) / dStageSizeX;
// // 	long lY_Offset = m_iMaxSizeY * 2 * (m_SampleOriginCoord.lY + m_HeadOffset.lY) / dStageSizeY;
// 	long lX_Offset = m_iMaxSizeX * 2 * (lMapDrawSampleOriginCoordX + m_HeadOffset.lX) / dStageSizeX;
// 	long lY_Offset = m_iMaxSizeY * 2 * (lMapDrawSampleOriginCoordY + m_HeadOffset.lY) / dStageSizeY;
// // 2013.01.09 bagus <--
// /* modified 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
//
// 	// �����X�e�[�W�̎������i�E�����F+X�A������F+Y�j�ƍ��킹��
// // 2013.01.09 bagus -->
// //	NS_ConvertToStageMoveXyAxis(&lX_Offset, &lY_Offset);
// // 2013.01.09 bagus <--
//
// 	long SampleRectLeft;
// 	long SampleRectTop;
// 	long SampleRectRight;
// 	long SampleRectBottom;
//
// // 2013.01.09 bagus -->
// // 	switch ( m_StageConfig.OrgPos ) {
// // 	case 1: // ���S
// // 		// �T���v���̎l�_
// // 		SampleRectLeft	 = m_lCenterPos.x - lDrawSampleSizeX + lX_Offset;
// // 		SampleRectTop	 = m_lCenterPos.y - lDrawSampleSizeY - lY_Offset;
// // 		SampleRectRight  = m_lCenterPos.x + lDrawSampleSizeX + lX_Offset;
// // 		SampleRectBottom = m_lCenterPos.y + lDrawSampleSizeY - lY_Offset;
// // 		break;
// // 	case 2: // ����
// // 		SampleRectLeft	 = m_lDrawStartStagePosX + 1 + lX_Offset;
// // 		SampleRectTop	 = m_lDrawStartStagePosY + 1 - lY_Offset;
// // 		SampleRectRight  = SampleRectLeft 	+ lDrawSampleSizeX * 2;
// // 		SampleRectBottom = SampleRectTop 	+ lDrawSampleSizeY * 2;
// // 		break;
// // 	case 3: // ����
// // 		SampleRectLeft 	 = m_lDrawStartStagePosX + 1 + lX_Offset;
// // 		SampleRectBottom = m_lDrawEndStagePosY	 - 1 - lY_Offset;
// // 		SampleRectRight  = SampleRectLeft 	+ lDrawSampleSizeX * 2;
// // 		SampleRectTop	 = SampleRectBottom	- lDrawSampleSizeY * 2;
// // 		break;
// // 	case 4: // �E��
// // 		SampleRectRight  = m_lDrawEndStagePosX	 - 1 + lX_Offset;
// // 		SampleRectBottom = m_lDrawEndStagePosY	 - 1 - lY_Offset;
// // 		SampleRectLeft 	 = SampleRectRight	- lDrawSampleSizeX * 2;
// // 		SampleRectTop	 = SampleRectBottom	- lDrawSampleSizeY * 2;
// // 		break;
// // 	case 5: // �E��
// // 		SampleRectRight  = m_lDrawEndStagePosX	 - 1 + lX_Offset;
// // 		SampleRectTop	 = m_lDrawStartStagePosY + 1 - lY_Offset;
// // 		SampleRectLeft 	 = SampleRectRight - lDrawSampleSizeX * 2;
// // 		SampleRectBottom = SampleRectTop   + lDrawSampleSizeY * 2;
// // 		break;
// // 	}
// 	switch ( m_StageConfig.OrgPos ) {
// 	case 1: // ���S
// 		// �T���v���̎l�_
// 		SampleRectLeft	 = m_lCenterPos.x - lDrawSampleSizeX + lX_Offset;
// 		SampleRectTop	 = m_lCenterPos.y - lDrawSampleSizeY - lY_Offset;
// 		SampleRectRight  = m_lCenterPos.x + lDrawSampleSizeX + lX_Offset;
// 		SampleRectBottom = m_lCenterPos.y + lDrawSampleSizeY - lY_Offset;
// 		break;
// 	case 2: // ����
// 		SampleRectLeft 	 = m_lDrawStartStagePosX + 1 + lX_Offset;
// 		SampleRectBottom = m_lDrawEndStagePosY	 - 1 - lY_Offset + (lDrawSampleSizeY * 2) - 1;
// 		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
// 		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 		break;
// 	case 3: // ����
// 		SampleRectLeft 	 = m_lDrawStartStagePosX + 1 + lX_Offset;
// 		SampleRectBottom = m_lDrawEndStagePosY	 - 1 - lY_Offset;
// 		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
// 		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 		break;
// 	case 4: // �E��
// 		SampleRectLeft 	 = m_lDrawStartStagePosX + 1 + lX_Offset - (lDrawSampleSizeX * 2) + 1;
// 		SampleRectBottom = m_lDrawEndStagePosY	 - 1 - lY_Offset;
// 		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
// 		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 		break;
// 	case 5: // �E��
// 		SampleRectLeft 	 = m_lDrawStartStagePosX + 1 + lX_Offset - (lDrawSampleSizeX * 2) + 1;
// 		SampleRectBottom = m_lDrawEndStagePosY	 - 1 - lY_Offset + (lDrawSampleSizeY * 2) - 1;
// 		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
// 		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 		break;
// 	}
// // 2013.01.09 bagus <--
//
//
// 	// �X�e�[�W���͂ݏo���ꍇ�͕`�悵�Ȃ�
// 	if( SampleRectLeft		< m_lDrawStartStagePosX ||
// 		SampleRectTop		< m_lDrawStartStagePosY ||
// 		SampleRectRight 	> m_lDrawEndStagePosX	||
// 		SampleRectBottom	> m_lDrawEndStagePosY	)	return;
//
// 	///// Region Map /////
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
// //	CWnd* pPictBox = GetDlgItem(IDC_STAGE_MAP);
// //	CDC* pDC = pPictBox->GetDC();
// 	CDC* pDC = m_bbDC;
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--
//
// 	// �T���v���̕`��
// 	CRect SampleRect;
// 	SampleRect = CRect(SampleRectLeft, SampleRectTop, SampleRectRight, SampleRectBottom);
// //	TRACE("COverlayDialog::DrawSampleMap Befor %ld, %ld, %ld, %ld \n", SampleRectLeft, SampleRectTop, SampleRectRight, SampleRectBottom);
//
// 	///// Draw Stage Color /////
// 	CBrush BrushStage(WATER_COLOR);
// 	m_oldBrush = pDC->SelectObject(&BrushStage);
// 	CPen PenStage(PS_SOLID, 1, WATER_COLOR);
// 	m_oldPen = pDC->SelectObject(&PenStage);
// 	switch ( m_StageConfig.OrgPos ) {
// 	case 1: // ���S
// 		pDC->Rectangle(SampleRectLeft, SampleRectTop, SampleRectRight+1, SampleRectBottom+1);
// 		break;
// 	default:
// 		pDC->Rectangle(SampleRectLeft, SampleRectTop, SampleRectRight, SampleRectBottom);
// 		break;
// 	}
//
// 	pDC->SelectObject(m_oldPen);
// 	pDC->SelectObject(m_oldBrush);
//
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O -->
// //	pPictBox->ReleaseDC(pDC);
// // 2009.05.27 K.Matsuo �_�u���o�b�t�@�����O <--
// }

// ==========================================================================
//
void COverlayDialog::DrawSampleMap(long lMapDrawSampleOriginCoordX, long lMapDrawSampleOriginCoordY)
{
	///// �X�e�[�W�L���͈� /////
	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

	// �T���v���̃T�C�Y
// 2013.01.17 bagus rotate xy view -->
// 	long lDrawSampleSizeX = m_iMaxSizeX * m_dSampleSizeX / dStageSizeX;
// 	long lDrawSampleSizeY = m_iMaxSizeY * m_dSampleSizeY / dStageSizeY;

	long lDrawSampleSizeX;
	long lDrawSampleSizeY;

	if ( m_StageConfig.RotateXyView == 0 ||
		 m_StageConfig.RotateXyView == 180 ) {
		lDrawSampleSizeX = m_iMaxSizeX * m_dSampleSizeX / dStageSizeX;
		lDrawSampleSizeY = m_iMaxSizeY * m_dSampleSizeY / dStageSizeY;
	}
	else {
		lDrawSampleSizeX = m_iMaxSizeX * m_dSampleSizeY / dStageSizeY;
		lDrawSampleSizeY = m_iMaxSizeY * m_dSampleSizeX / dStageSizeX;
	}

	// �X�e�[�W���_����̈ړ���
// 	long lX_Offset = m_iMaxSizeX * 2 * (lMapDrawSampleOriginCoordX + m_HeadOffset.lX) / dStageSizeX;
// 	long lY_Offset = m_iMaxSizeY * 2 * (lMapDrawSampleOriginCoordY + m_HeadOffset.lY) / dStageSizeY;

	long lX_Offset;
	long lY_Offset;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		lX_Offset = m_iMaxSizeX * 2 * (lMapDrawSampleOriginCoordX + m_HeadOffset.lX) / dStageSizeX;
		lY_Offset = m_iMaxSizeY * 2 * (lMapDrawSampleOriginCoordY + m_HeadOffset.lY) / dStageSizeY;
		break;
	case 90:
		lX_Offset = m_iMaxSizeX * 2 * (lMapDrawSampleOriginCoordY + m_HeadOffset.lY) / dStageSizeY;
		lY_Offset = m_iMaxSizeY * 2 * (lMapDrawSampleOriginCoordX + m_HeadOffset.lX) / dStageSizeX;
		lY_Offset = m_iMaxSizeY * 2 - lY_Offset;
		break;
	case 180:
		lX_Offset = m_iMaxSizeX * 2 * (lMapDrawSampleOriginCoordX + m_HeadOffset.lX) / dStageSizeX;
		lX_Offset = m_iMaxSizeX * 2 - lX_Offset;
		lY_Offset = m_iMaxSizeY * 2 * (lMapDrawSampleOriginCoordY + m_HeadOffset.lY) / dStageSizeY;
		lY_Offset = m_iMaxSizeY * 2 - lY_Offset;
		break;
	case 270:
		lX_Offset = m_iMaxSizeX * 2 * (lMapDrawSampleOriginCoordY + m_HeadOffset.lY) / dStageSizeY;
		lX_Offset = m_iMaxSizeX * 2 - lX_Offset;
		lY_Offset = m_iMaxSizeY * 2 * (lMapDrawSampleOriginCoordX + m_HeadOffset.lX) / dStageSizeX;
		break;
	}
// 2013.01.17 bagus rotate xy view <--

	long SampleRectLeft;
	long SampleRectTop;
	long SampleRectRight;
	long SampleRectBottom;

	switch ( m_StageConfig.OrgPos ) {
	case 1: // ���S
		// �T���v���̎l�_
		SampleRectLeft	 = m_lCenterPos.x - lDrawSampleSizeX + lX_Offset;
		SampleRectTop	 = m_lCenterPos.y - lDrawSampleSizeY - lY_Offset;
		SampleRectRight  = m_lCenterPos.x + lDrawSampleSizeX + lX_Offset;
		SampleRectBottom = m_lCenterPos.y + lDrawSampleSizeY - lY_Offset;
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			break;
		case 180:
			break;
		case 270:
			break;
		}
		break;
// 2013.01.17 bagus rotate xy view <--
	case 2: // ����
// 2013.01.17 bagus rotate xy view -->
		SampleRectLeft 	 = m_lCenterPos.x + lX_Offset;
		SampleRectTop	 = m_lCenterPos.y - lY_Offset;
		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
		SampleRectBottom = SampleRectTop    + (lDrawSampleSizeY * 2);
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			SampleRectLeft -= 2;
			SampleRectRight -= 2;
			break;
		case 90:
			SampleRectTop += 2;
			SampleRectBottom += 2;
			SampleRectLeft -= (lDrawSampleSizeX * 2);
			SampleRectRight -= (lDrawSampleSizeX * 2);
			break;
		case 180:
			SampleRectTop -= (lDrawSampleSizeY * 2);
			SampleRectLeft -= (lDrawSampleSizeX * 2) + 2;
			SampleRectBottom -= (lDrawSampleSizeY * 2);
			SampleRectRight -= (lDrawSampleSizeX * 2) + 2;
			break;
		case 270:
			SampleRectTop -= (lDrawSampleSizeY * 2) - 2;
			SampleRectBottom -= (lDrawSampleSizeY * 2) - 2;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 3: // ����
		SampleRectLeft 	 = m_lCenterPos.x + lX_Offset;
		SampleRectBottom = m_lCenterPos.y - lY_Offset;
		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			SampleRectLeft -= 2;
			SampleRectRight -= 2;
			break;
		case 90:
			SampleRectTop += (lDrawSampleSizeY * 2) + 2;
			SampleRectBottom += (lDrawSampleSizeY * 2) + 2;
			break;
		case 180:
			SampleRectTop += (lDrawSampleSizeY * 2);
			SampleRectLeft -= (lDrawSampleSizeX * 2) + 2;
			SampleRectBottom += (lDrawSampleSizeY * 2);
			SampleRectRight -= (lDrawSampleSizeX * 2) + 2;
			break;
		case 270:
			SampleRectTop += 2;
			SampleRectBottom += 2;
			SampleRectLeft -= (lDrawSampleSizeX * 2);
			SampleRectRight -= (lDrawSampleSizeX * 2);
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 4: // �E��
		SampleRectLeft 	 = m_lCenterPos.x + lX_Offset - (lDrawSampleSizeX * 2);
		SampleRectBottom = m_lCenterPos.y - lY_Offset;
		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			SampleRectLeft -= 2;
			SampleRectRight -= 2;
			break;
		case 90:
			SampleRectTop += 2;
			SampleRectBottom += 2;
			SampleRectLeft += (lDrawSampleSizeX * 2);
			SampleRectRight += (lDrawSampleSizeX * 2);
			break;
		case 180:
			SampleRectLeft += (lDrawSampleSizeX * 2) - 2;
			SampleRectTop += (lDrawSampleSizeY * 2);
			SampleRectRight += (lDrawSampleSizeX * 2) - 2;
			SampleRectBottom += (lDrawSampleSizeY * 2);
			break;
		case 270:
			SampleRectTop += (lDrawSampleSizeY * 2) + 2;
			SampleRectBottom += (lDrawSampleSizeY * 2) + 2;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 5: // �E��
		SampleRectLeft 	 = m_lCenterPos.x + lX_Offset - (lDrawSampleSizeX * 2);
		SampleRectBottom = m_lCenterPos.y - lY_Offset + (lDrawSampleSizeY * 2);
		SampleRectRight  = SampleRectLeft 	+ (lDrawSampleSizeX * 2);
		SampleRectTop	 = SampleRectBottom	- (lDrawSampleSizeY * 2);
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			SampleRectLeft -= 2;
			SampleRectRight -= 2;
			break;
		case 90:
			SampleRectTop -= (lDrawSampleSizeY * 2) - 2;
			SampleRectBottom -= (lDrawSampleSizeY * 2) - 2;
			break;
		case 180:
			SampleRectLeft += (lDrawSampleSizeX * 2) - 2;
			SampleRectTop -= (lDrawSampleSizeY * 2);
			SampleRectRight += (lDrawSampleSizeX * 2) - 2;
			SampleRectBottom -= (lDrawSampleSizeY * 2);
			break;
		case 270:
			SampleRectTop += 2;
			SampleRectBottom += 2;
			SampleRectLeft += (lDrawSampleSizeX * 2);
			SampleRectRight += (lDrawSampleSizeX * 2);
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	}

	// �X�e�[�W���͂ݏo���ꍇ�͕`�悵�Ȃ�
	if( SampleRectLeft		< m_lDrawStartStagePosX - 2 ||
		SampleRectTop		< m_lDrawStartStagePosY - 2 ||
		SampleRectRight 	> m_lDrawEndStagePosX + 2	||
		SampleRectBottom	> m_lDrawEndStagePosY + 2	)	return;

	///// Region Map /////
	CDC* pDC = m_bbDC;

	// �T���v���̕`��
	CRect SampleRect;
	SampleRect = CRect(SampleRectLeft, SampleRectTop, SampleRectRight, SampleRectBottom);

	///// Draw Stage Color /////
	CBrush BrushStage(WATER_COLOR);
	m_oldBrush = pDC->SelectObject(&BrushStage);
	CPen PenStage(PS_SOLID, 1, WATER_COLOR);
	m_oldPen = pDC->SelectObject(&PenStage);
	switch ( m_StageConfig.OrgPos ) {
	case 1: // ���S
		pDC->Rectangle(SampleRectLeft, SampleRectTop, SampleRectRight+1, SampleRectBottom+1);
		break;
	default:
		pDC->Rectangle(SampleRectLeft, SampleRectTop, SampleRectRight, SampleRectBottom);
		break;
	}

	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);
}

// =========================================================================
//
void COverlayDialog::SetOperationLog(LPCTSTR pszLog)
{
	char strmsg[1024];

	// ���샍�O���擾
	memset( strmsg, 0, sizeof(strmsg));
	strcpy( strmsg, "[");
	::GetWindowText(this->m_hWnd, &strmsg[1], 1000);
	_tcscat( strmsg, "] ");
	_tcscat( strmsg, pszLog);

	m_pDoc->OperationLogging(strmsg);
}

// =========================================================================
//
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
//void COverlayDialog::OnJoyStick()
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ----------			   */
LRESULT COverlayDialog::OnJoyStick(WPARAM wparam, LPARAM lparam)
/* modified 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- } ---------- */
{
	// �W���C�X�e�B�b�N�L���^����
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		m_joyStick1->UnChecked();
		m_joyStick2->Checked();
		// �W���C�X�e�B�b�N����
		m_XY_UpButton.SetEnabled(FALSE);
		m_XY_DownButton.SetEnabled(FALSE);
		m_XY_RightButton.SetEnabled(FALSE);
		m_XY_LeftButton.SetEnabled(FALSE);
	}
	else{
		m_joyStick1->Checked();
		m_joyStick2->UnChecked();
		// �W���C�X�e�B�b�N�֎~
		m_XY_UpButton.SetEnabled(TRUE);
		m_XY_DownButton.SetEnabled(TRUE);
		m_XY_RightButton.SetEnabled(TRUE);
		m_XY_LeftButton.SetEnabled(TRUE);
	}
/* added 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- { ---------- */
	return 0;
/* added 2009.06.15 hmenjo ���[�U��`���b�Z�[�W�n���h�� IF �C�� ---------- } ---------- */
}

// =========================================================================
//
void COverlayDialog::ButtonEnableCheck()
{
#if 1	/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	/* �ǂ��ɂ��邩����Ȃ����l�ȏ����̃R���t���N�g�ňُ퓮��ɂȂ邽�ߕύX���܂���D	*/
	// XY���{�^��
	BOOL bXyButtonEnable = ( m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	m_XY_UpButton.SetEnabled(bXyButtonEnable);
	m_XY_DownButton.SetEnabled(bXyButtonEnable);
	m_XY_RightButton.SetEnabled(bXyButtonEnable);
	m_XY_LeftButton.SetEnabled(bXyButtonEnable);

	// Z���{�^��
	BOOL bZButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) );
	m_Z_UpButton.SetEnabled(bZButtonEnable);
	m_Z_DownButton.SetEnabled(bZButtonEnable);
	m_Z_HomePosButton.SetEnabled(bZButtonEnable);

	// AutoFocus�{�^��
	BOOL bAFButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) );
	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(bAFButtonEnable);

	// ���W���̓{�^��
	BOOL bCoordButtonEnable = ( m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ||
								m_pMainFrame->GetJoyStickMode() != JOY_SOFT_MODE );
	GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(bCoordButtonEnable);
#else	/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
	/* XY���{�^�� ------------------------------------------------ */
	BOOL bXyButtonEnable =	(m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE
						 && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE));
	static BOOL ls_bXyButtonEnablePre = bXyButtonEnable;			/* �O��l	*/
	static BOOL ls_bXYUBtnEnaBack = m_XY_UpButton.GetEnabled();		/* �O��l	*/
	static BOOL ls_bXYDBtnEnaBack = m_XY_DownButton.GetEnabled();	/* �O��l	*/
	static BOOL ls_bXYRBtnEnaBack = m_XY_RightButton.GetEnabled();	/* �O��l	*/
	static BOOL ls_bXYLBtnEnaBack = m_XY_LeftButton.GetEnabled();	/* �O��l	*/
	if (ls_bXyButtonEnablePre != bXyButtonEnable) {
		/* �ω��L��	*/
		if (TRUE == bXyButtonEnable) {
			m_XY_UpButton.SetEnabled(ls_bXYUBtnEnaBack);
			m_XY_DownButton.SetEnabled(ls_bXYDBtnEnaBack);
			m_XY_RightButton.SetEnabled(ls_bXYRBtnEnaBack);
			m_XY_LeftButton.SetEnabled(ls_bXYLBtnEnaBack);
		} else {
			ls_bXYUBtnEnaBack = m_XY_UpButton.GetEnabled();
			ls_bXYDBtnEnaBack = m_XY_DownButton.GetEnabled();
			ls_bXYRBtnEnaBack = m_XY_RightButton.GetEnabled();
			ls_bXYLBtnEnaBack = m_XY_LeftButton.GetEnabled();
			m_XY_UpButton.SetEnabled(FALSE);
			m_XY_DownButton.SetEnabled(FALSE);
			m_XY_RightButton.SetEnabled(FALSE);
			m_XY_LeftButton.SetEnabled(FALSE);
		}
	}

	/* Z���{�^�� ------------------------------------------------- */
	BOOL bZButtonEnable = (!m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS));
	static BOOL ls_bZButtonEnablePre = bZButtonEnable;	/* �O��l	*/
	static BOOL ls_bZUBtnEnaBack = this->GetDlgItem(IDC_ELEV_UP_BUTTON)->IsWindowEnabled();			/* �O��l	*/
	static BOOL ls_bZHBtnEnaBack = this->GetDlgItem(IDC_HOME_POSITION_BUTTON)->IsWindowEnabled();	/* �O��l	*/
	static BOOL ls_bZDBtnEnaBack = this->GetDlgItem(IDC_ELEV_DOWN_BUTTON)->IsWindowEnabled();		/* �O��l	*/
	if (ls_bZButtonEnablePre != bZButtonEnable) {
		/* �ω��L��	*/
		if (TRUE == bZButtonEnable) {
			this->GetDlgItem(IDC_ELEV_UP_BUTTON)->EnableWindow(ls_bZUBtnEnaBack);
			this->GetDlgItem(IDC_HOME_POSITION_BUTTON)->EnableWindow(ls_bZHBtnEnaBack);
			this->GetDlgItem(IDC_ELEV_DOWN_BUTTON)->EnableWindow(ls_bZDBtnEnaBack);
		} else {
			ls_bZUBtnEnaBack = this->GetDlgItem(IDC_ELEV_UP_BUTTON)->IsWindowEnabled();
			ls_bZHBtnEnaBack = this->GetDlgItem(IDC_HOME_POSITION_BUTTON)->IsWindowEnabled();
			ls_bZDBtnEnaBack = this->GetDlgItem(IDC_ELEV_DOWN_BUTTON)->IsWindowEnabled();
			this->GetDlgItem(IDC_ELEV_UP_BUTTON)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_HOME_POSITION_BUTTON)->EnableWindow(FALSE);
			this->GetDlgItem(IDC_ELEV_DOWN_BUTTON)->EnableWindow(FALSE);
		}
	}

	/* AutoFocus�{�^�� ------------------------------------------- */
	BOOL bAFButtonEnable = (!m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS));
	static BOOL ls_bAFButtonEnablePre = bAFButtonEnable;	/* �O��l	*/
	static BOOL ls_bAfBtnEnaBack = this->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->IsWindowEnabled();	/* �O��l	*/
	if (ls_bAFButtonEnablePre != bAFButtonEnable) {
		/* �ω��L��	*/
		if (TRUE == bAFButtonEnable) {
			this->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(ls_bAfBtnEnaBack);
		} else {
			ls_bAfBtnEnaBack = this->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->IsWindowEnabled();
			this->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE);
		}
	}

	/* ���W���̓{�^�� -------------------------------------------- */
	BOOL bCoordButtonEnable =	(
								m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE
							 && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)
							 || m_pMainFrame->GetJoyStickMode() != JOY_SOFT_MODE
									);
	static BOOL ls_bCoordButtonEnablePre = bCoordButtonEnable;	/* �O��l	*/
	static BOOL ls_bCordBtnEnaBack = this->GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->IsWindowEnabled();	/* �O��l	*/
	if (ls_bCoordButtonEnablePre != bCoordButtonEnable) {
		/* �ω��L��	*/
		if (TRUE == bCoordButtonEnable) {
			this->GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(ls_bCordBtnEnaBack);
		} else {
			ls_bCordBtnEnaBack = this->GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->IsWindowEnabled();
			this->GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(FALSE);
		}
	}
#endif	/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */

}

// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ -->
// =========================================================================
//
void COverlayDialog::JoyStickEnableCheck()
{
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* �h�A�C���^�[���b�N			���g */
		if ( m_joyStick2->GetCheck() ) {
			// �����I�Ƀ\�t�gJoy�֐؂�ւ���
			m_pMainFrame->SetJoyStickMode(JOY_SOFT_MODE);
			m_pDoc->ChangeJoyStick(FALSE);
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			m_joyStick1->Checked();
			m_joyStick2->UnChecked();
			m_joyStick1->SetFocus();
		}
	}
}
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ <--

// =========================================================================
//
void COverlayDialog::OnStageMove()
{
STAGE_COORD		Position;
CString strMsg, strTitle;


	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){	// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{											// �ʏ퓮��
		if( nexioIsMaintenanceSwitch() != ON ){ 	AlarmIf_Set(ALID_MaintenanceSwitchOn); 	return;}	// �����e�i���X�E�X�C�b�`�E�I��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock()		== TRUE){							return;}
	if( nexioIsMovo2Alarm() 		!= OFF ){		AlarmIf_Set(ALID_StageError);	return;}	// MOVO�A���[�� ���k
	if(m_pDoc->CheckUnitStatus()	== TRUE){							return;}
	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){								return;}
	// ���C���^�[���b�N������

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "Warning");
		MessageBox(strMsg, strTitle, MB_OK);
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
		return;
	}

	SetOperationLog("MOVE Button was push.");

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	if(!m_pMainFrame->HardwareSimulation()){
		Position.lX = m_lX;
		Position.lY = m_lY;

		if( CheckStageMoveEnable(&Position) ){
			// �L���͈͂Ȃ�ړ�������
			NS_StageMoveAbsolute(&Position);
		}
	}
	else{
		Position.lX = m_lX;
		Position.lY = m_lY;

		if( CheckStageMoveEnable(&Position) ){
			// �L���͈͂Ȃ�ړ�������
			m_CurrPos.lX = Position.lX;
			m_CurrPos.lY = Position.lY;
		}
	}
// 2009.07.10 K.Matsuo BugFix -->
//	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
// 2009.07.10 K.Matsuo BugFix <--

	{	// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
	}
}

// 2010.01.07 bagus Gantry --{--
// =========================================================================
//
void COverlayDialog::OnCheckScan()
{
// 2010.01.21 bagus Gantry --{--
#if 0
	CScanDataGraphDlg	dlg;
	CString strIntegrationTime;
	SR_CONFIG l_SrConfig;

	ConfigFile_GetNanoSpecIni(&l_SrConfig,CONFIG_FILE_SR_CONFIG);
	dlg.m_iStartWaveLength = MIN_TARGET_WAVELENGTH;
	if ( l_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		dlg.m_iEndWaveLength = MAX_TARGET_WAVELENGTH;
	else
		dlg.m_iEndWaveLength = MAX_TARGET_WAVELENGTH_800;
	dlg.m_nOpticalFilter = ((CComboBox *)GetDlgItem(IDC_STAGE_REFERENCE_FILTER))->GetCurSel();
	dlg.m_bTransmittanceShutterOpen = ((CButton *)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1))->GetCheck();
	GetDlgItemText(IDC_STAGE_REFERENCE_INTEGRATION_TIME,strIntegrationTime);
	dlg.m_dIntegrationTime = atof(strIntegrationTime.GetBuffer(0));
	//2010.01.13 bagus GTR --{--
	//if(dlg.m_nIntegrationTime <= 0.0){
	if(dlg.m_dIntegrationTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < dlg.m_dIntegrationTime){
	//2010.01.13 bagus GTR --}--
		CString l_strBuffer;
		CString strBuffer;
		//2010.01.13 bagus �C�� --{--
		//LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.1f - %.1f)");
		LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.2f - %.2f)");
		//2010.01.13 bagus �C�� --}--
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, "CHECK SCAN", MB_OK | MB_ICONSTOP);
		return;
	}

	dlg.DoModal();
#else
	CString strIntegrationTime;
	SR_CONFIG l_SrConfig;

	ConfigFile_GetNanoSpecIni(&l_SrConfig,CONFIG_FILE_SR_CONFIG);
	m_ScanDataGraphDlg.m_iStartWaveLength = MIN_TARGET_WAVELENGTH;
	if ( l_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
		m_ScanDataGraphDlg.m_iEndWaveLength = MAX_TARGET_WAVELENGTH;
	else
		m_ScanDataGraphDlg.m_iEndWaveLength = MAX_TARGET_WAVELENGTH_800;
	m_ScanDataGraphDlg.m_nOpticalFilter = ((CComboBox *)GetDlgItem(IDC_STAGE_REFERENCE_FILTER))->GetCurSel();
	m_ScanDataGraphDlg.m_bTransmittanceShutterOpen = ((CButton *)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1))->GetCheck();
	GetDlgItemText(IDC_STAGE_REFERENCE_INTEGRATION_TIME,strIntegrationTime);
	m_ScanDataGraphDlg.m_dIntegrationTime = atof(strIntegrationTime.GetBuffer(0));

	if(m_ScanDataGraphDlg.m_dIntegrationTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_ScanDataGraphDlg.m_dIntegrationTime){
		CString l_strBuffer;
		CString strBuffer;
		LoadStringML(IDS_INTEGTIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.2f - %.2f)");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, "CHECK SCAN", MB_OK | MB_ICONSTOP);
		return;
	}

	m_ScanDataGraphDlg.InitDialogData();

	m_ScanDataGraphDlg.ScanData();

//	m_ScanDataGraphDlg.SetWindowPos( AfxGetMainWnd(), 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE ) ;
	m_ScanDataGraphDlg.ShowWindow( SW_NORMAL );
#endif
// 2010.01.21 bagus Gantry --}--
}
// 2010.01.07 bagus Gantry --}--

// =========================================================================
//
BOOL COverlayDialog::CheckStageMoveEnable(STAGE_COORD* pos)
{
CString strMsg;

	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;

	coordLeftTop.lX = m_StageConfig.StageMaxCoord.dLeftX;
	coordLeftTop.lY = m_StageConfig.StageMaxCoord.dUpY;
	coordRightBottom.lX = m_StageConfig.StageMaxCoord.dRightX;
	coordRightBottom.lY = m_StageConfig.StageMaxCoord.dDownY;

	NS_ConvertToStageGetPosCoord(&coordLeftTop);
	NS_ConvertToStageGetPosCoord(&coordRightBottom);

	///// Stage Origin X /////
	if(( pos->lX < min(coordLeftTop.lX, coordRightBottom.lX) ) || ( max(coordLeftTop.lX, coordRightBottom.lX) < pos->lX )){
		LoadStringML(IDS_X_AXIS_ORIGIN_OUT_RANGE, strMsg, "X axis origin coordinate is out of range the stage.");
		MessageBox(strMsg, m_sCaption, MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	///// Stage Origin Y /////
	if(( pos->lY < min(coordLeftTop.lY, coordRightBottom.lY) ) || ( max(coordLeftTop.lY, coordRightBottom.lY) < pos->lY )){
		LoadStringML(IDS_Y_AXIS_ORIGIN_OUT_RANGE, strMsg, "Y axis origin coordinate is out of range the stage.");
		MessageBox(strMsg, m_sCaption, MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	return TRUE;
}


// =========================================================================
//
void COverlayDialog::OnHomePositionButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
// 2010.02.03 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
	CString strMsg;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVO�A���[�� 				���k
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ���C���^�[���b�N������
// 2010.02.03 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z ���̏�������~�ʒu
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z ���̏�������~�ʒu�t�@�N�^

	// �n�[�h�V�~�����[�g���[�h���� -------------------------------------------
	if (0 != m_pMainFrame->HardwareSimulation()) {
		// �n�[�h�V�~�����[�g���[�h�������̂ŁC���芮���҂� ��
	}
	else {
		// ���@�̏ꍇ ---------------------------------------------------------
		// AF �v/�s�v����
		// �s�v
			// (�S�_ AF �����̏ꍇ)Z ���ړ����K�v���`�F�b�N
		// �K�v(�d�l��K�v�Ȃ�)
		// �y���ړ�
		//TCHAR l_szMainRcpName[256];
		//((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// ��������~�ʒu[pulse]��[0.1um]�ɕϊ����܂�
		LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
		l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// ��������~�ʒu[pulse]���t�@�N�^[pulse/mm]
		l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]�ɕϊ�����
		long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

		NS_ConvertToStageMoveCoord(&l_lZ);		// �␳

		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ���쒆�t���O(Z ��)���I��
#if 0	//----- Kawashima 2008.12.01 ----->
//				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
		m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// �ړ��J�n���s
		} else {
			// �ړ�����
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ---------- { ---------- */
			m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);   // Status Bar
#else						/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ----------			   */
			m_pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ---------- } ---------- */
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
	}
}
// 2010.01.07 bagus Gantry Transmittance --{--
void COverlayDialog::InitCombo_SrOpticalFilter()
{

	CString l_strBuffer, l_strTitle;
	SR_FILTER l_srFilter[SR_FILTER_MAX];

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_STAGE_REFERENCE_FILTER);
	ConfigFile_GetNanoSpecIni(l_srFilter, CONFIG_FILE_SR_FILTER);

	pCombo->ResetContent();

	int nIndex;
	int iFilterIndex;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	LPCTSTR pszFilterName[LANGUAGE_MAX][OPT_FILTER_MAX] = {
		{OPTICAL_FILTER_TYPE_ITEM_ENU[0], OPTICAL_FILTER_TYPE_ITEM_ENU[1], OPTICAL_FILTER_TYPE_ITEM_ENU[2], l_srFilter[0].szName, l_srFilter[1].szName, l_srFilter[2].szName},	// Enu
		{OPTICAL_FILTER_TYPE_ITEM_JPN[0], OPTICAL_FILTER_TYPE_ITEM_JPN[1], OPTICAL_FILTER_TYPE_ITEM_JPN[2], l_srFilter[0].szName, l_srFilter[1].szName, l_srFilter[2].szName},	// Jpn
	};

	for ( int iOptItem = 0; iOptItem < OPT_FILTER_MAX; iOptItem++ ) {
		switch ( iOptItem ) {
		case OPT_FILTER_UNKNOWN:
			break;
		case OPT_FILTER_OPEN:
			nIndex = pCombo->AddString(pszFilterName[l_SystemConfig.nLanguage][iOptItem]);
			pCombo->SetItemData(nIndex, iOptItem);
			break;
		case OPT_FILTER_DARK:
			break;
		case OPT_FILTER_POS1:
		case OPT_FILTER_POS2:
		case OPT_FILTER_POS3:
			iFilterIndex = iOptItem - 3;
			if ( l_srFilter[iFilterIndex].bEnable ) {
				nIndex = pCombo->AddString(l_srFilter[iFilterIndex].szName);
				pCombo->SetItemData(nIndex, iOptItem);
			}
			break;
		default:
			break;
		}
	}

	pCombo->SetCurSel(0);

}
void COverlayDialog::OnSelchangeSrOptFilter()
{
	CComboBox *pCombo;

	pCombo = (CComboBox *)GetDlgItem(IDC_STAGE_REFERENCE_FILTER);
	if(!pCombo)
		return;
	MEAS_SrHead_ChangeCcdShutter(pCombo->GetCurSel());

}

void COverlayDialog::OnCheckTransmittanceShutter()
{
	CButton *pButton;
	BOOL bRet;

	pButton = (CButton *)GetDlgItem(IDC_STAGE_REFERENCE_TRANSMITTANCE_SHUTTER_1);
	if(!pButton)
		return;
	//���ߗ������V���b�^�[�̊J��
	if(pButton->GetCheck()){
		bRet = m_pDoc->SetTransmittanceLamp(FILTER_OPEN);
	}else{
		bRet = m_pDoc->SetTransmittanceLamp(FILTER_DARK);
	}
}
// 2010.01.07 bagus Gantry Transmittance --}--

// 2010.01.29 bagus Gantry --{--
void COverlayDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

}
// 2010.01.29 bagus Gantry --}--
