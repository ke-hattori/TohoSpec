// SrReferencePositionSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "SrReferencePositionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ==========================================================================
// Define																   //
//																		   //
// ==========================================================================
//#define DARK_REFERENCE_POSITION_SETTING_CAPTION			"DARK REFERENCE POSITION SETTING"
//#define DARK_REFERENCE_POSITION_SETTING_MESSAGE			"DARK REFERENCE POSITION SETTING"
//#define REFLECT_ABSOLUTE_REFERENCE_POSITION_CAPTION		"REFLECT ABSOLUTE REFERENCE POSITION SETTING"
//#define REFLECT_ABSOLUTE_REFERENCE_POSITION_MESSAGE		"REFLECT ABSOLUTE REFERENCE POSITION SETTING"
//#define TRANSMIT_ABSOLUTE_REFERENCE_POSITION_CAPTION	"TRANSMIT ABSOLUTE REFERENCE POSITION SETTING"
//#define TRANSMIT_ABSOLUTE_REFERENCE_POSITION_MESSAGE	"TRANSMIT ABSOLUTE REFERENCE POSITION SETTING"
//#define TRANSMIT_1ST_REFERENCE_POSITION_CAPTION			"TRANSMIT 1ST REFERENCE POSITION SETTING"
//#define TRANSMIT_1ST_REFERENCE_POSITION_MESSAGE			"TRANSMIT 1ST REFERENCE POSITION SETTING"
//#define TRANSMIT_2ND_REFERENCE_POSITION_CAPTION			"TRANSMIT 2ND REFERENCE POSITION SETTING"
//#define TRANSMIT_2ND_REFERENCE_POSITION_MESSAGE			"TRANSMIT 2ND REFERENCE POSITION SETTING"
//#define DARK_REFERENCE_POSITION_SETTING_CAPTION		  "ダークリファレンス位置設定"
//#define DARK_REFERENCE_POSITION_SETTING_MESSAGE		  "ダークリファレンス位置設定"
//#define REFLECT_ABSOLUTE_REFERENCE_POSITION_CAPTION	  "反射絶対リファレンス位置設定"
//#define REFLECT_ABSOLUTE_REFERENCE_POSITION_MESSAGE	  "反射絶対リファレンス位置設定"
//#define TRANSMIT_ABSOLUTE_REFERENCE_POSITION_CAPTION	  "透過絶対リファレンス位置設定"
//#define TRANSMIT_ABSOLUTE_REFERENCE_POSITION_MESSAGE	  "透過絶対リファレンス位置設定"
//#define TRANSMIT_1ST_REFERENCE_POSITION_CAPTION		  "第１透過リファレンス位置設定"
//#define TRANSMIT_1ST_REFERENCE_POSITION_MESSAGE		  "第１透過リファレンス位置設定"
//#define TRANSMIT_2ND_REFERENCE_POSITION_CAPTION		  "第２透過リファレンス位置設定"
//#define TRANSMIT_2ND_REFERENCE_POSITION_MESSAGE		  "第２透過リファレンス位置設定"

// #########################################################################
// CSrReferencePositionSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSrReferencePositionSettingDlg::CSrReferencePositionSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSrReferencePositionSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSrReferencePositionSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSrReferencePositionSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DARK_REFERENCE_POSITION_X, m_stcDarkRefPos_X);
	DDX_Control(pDX, IDC_DARK_REFERENCE_POSITION_Y, m_stcDarkRefPos_Y);
	DDX_Control(pDX, IDC_DARK_REFERENCE_POSITION_Z, m_stcDarkRefPos_Z);
	DDX_Control(pDX, IDC_REFLECT_REFERENCE_POSITION_X, m_stcReflectRefPos_X);
	DDX_Control(pDX, IDC_REFLECT_REFERENCE_POSITION_Y, m_stcReflectRefPos_Y);
	DDX_Control(pDX, IDC_REFLECT_REFERENCE_POSITION_Z, m_stcReflectRefPos_Z);
	DDX_Control(pDX, IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X, m_stcTransmitAbsoluteRefPos_X);
	DDX_Control(pDX, IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y, m_stcTransmitAbsoluteRefPos_Y);
	DDX_Control(pDX, IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z, m_stcTransmitAbsoluteRefPos_Z);
	DDX_Control(pDX, IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X, m_stcTransmitFirstRefPos_X);
	DDX_Control(pDX, IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y, m_stcTransmitFirstRefPos_Y);
	DDX_Control(pDX, IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z, m_stcTransmitFirstRefPos_Z);
	DDX_Control(pDX, IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X, m_stcTransmitSecondRefPos_X);
	DDX_Control(pDX, IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y, m_stcTransmitSecondRefPos_Y);
	DDX_Control(pDX, IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z, m_stcTransmitSecondRefPos_Z);
	DDX_Control(pDX, IDC_DARK_REFERENCE_SETUP_BUTTON, m_DarkReferenceSetupButton);
	DDX_Control(pDX, IDC_REFLECT_REFERENCE_SETUP_BUTTON, m_ReflectReferenceSetupButton);
	DDX_Control(pDX, IDC_TRANSMIT_ABSOLUTE_REFERENCE_SETUP_BUTTON, m_TransmitAbsoluteReferenceSetupButton);
	DDX_Control(pDX, IDC_TRANSMIT_FIRST_REFERENCE_SETUP_BUTTON, m_TransmitFirstReferenceSetupButton);
	DDX_Control(pDX, IDC_TRANSMIT_SECOND_REFERENCE_SETUP_BUTTON, m_TransmitSecondReferenceSetupButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
//2009.09.28 bagus gantry --{--
	DDX_Control(pDX, IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_X, m_stcGantryTransmitRefPos_X);
	DDX_Control(pDX, IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Y, m_stcGantryTransmitRefPos_Y);
	DDX_Control(pDX, IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Z, m_stcGantryTransmitRefPos_Z);
	DDX_Control(pDX, IDC_GANTRY_TRANSMIT_REFERENCE_SETUP_BUTTON, m_GantryTransmitReferenceSetupButton);
//2009.09.28 bagus gantry --}--
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSrReferencePositionSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSrReferencePositionSettingDlg)
	ON_BN_CLICKED(IDC_DARK_REFERENCE_SETUP_BUTTON, OnDarkReferenceSetupButton)
	ON_BN_CLICKED(IDC_REFLECT_REFERENCE_SETUP_BUTTON, OnReflectReferenceSetupButton)
	ON_BN_CLICKED(IDC_TRANSMIT_ABSOLUTE_REFERENCE_SETUP_BUTTON, OnTransmitAbsoluteReferenceSetupButton)
	ON_BN_CLICKED(IDC_TRANSMIT_FIRST_REFERENCE_SETUP_BUTTON, OnTransmitFirstReferenceSetupButton)
	ON_BN_CLICKED(IDC_TRANSMIT_SECOND_REFERENCE_SETUP_BUTTON, OnTransmitSecondReferenceSetupButton)
	ON_CBN_SELCHANGE(IDC_POSITION_NO, OnSelchangePositionNo)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_GANTRY_TRANSMIT_REFERENCE_SETUP_BUTTON, OnGantryTransmitReferenceSetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSrReferencePositionSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSrReferencePositionSettingDlg::OnInitDialog()
{
	//未対応機能HIDE
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_GRPBOX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_SETUP_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X_LABEL)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_GRPBOX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_FIRST_REFERENCE_SETUP_BUTTON)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_GRPBOX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TRANSMIT_SECOND_REFERENCE_SETUP_BUTTON)->ShowWindow(SW_HIDE);


	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// 動的サイズ・配置変更 /////
//2009.09.28 bagus gantry --{--
#if 0
	GetDlgItem(IDOK)->SetWindowPos(&wndTop,260,170,0,0,SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop,365,170,0,0,SWP_NOSIZE);
	CSrReferencePositionSettingDlg::SetWindowPos(&wndTop,0,0,470,233,SWP_NOMOVE);
#else
	SR_CONFIG SrConfig;

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_POSITION_NO);
	int	i;
	CString	strBuffer;

	memset(&SrConfig, 0, sizeof(SrConfig));
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	if (SrConfig.wGantryNumOfMeasPoint <= 0) {
		GetDlgItem(IDC_POSITION_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_SETUP_BUTTON)->EnableWindow(FALSE);
	}

	if (!SrConfig.bTransmittance || !SrConfig.bGantryType) {
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_GRPBOX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_NO_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_X_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Y_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Z_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Z)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GANTRY_TRANSMIT_REFERENCE_SETUP_BUTTON)->ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->SetWindowPos(&wndTop,260,170,0,0,SWP_NOSIZE);
		GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop,365,170,0,0,SWP_NOSIZE);
		CSrReferencePositionSettingDlg::SetWindowPos(&wndTop,0,0,470,233,SWP_NOMOVE);
	} else {
		GetDlgItem(IDOK)->SetWindowPos(&wndTop,478,200,0,0,SWP_NOSIZE);
		GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop,583,200,0,0,SWP_NOSIZE);
		CSrReferencePositionSettingDlg::SetWindowPos(&wndTop,0,0,688,263,SWP_NOMOVE);
	}

	///// Combo Box Initialize /////
	pCombo->ResetContent();
	for (i=1; i<=SrConfig.wGantryNumOfMeasPoint; i++) {
		strBuffer.Format("%d", i);
		pCombo->AddString(strBuffer);
	}
	pCombo->SetCurSel(0);
#endif
//2009.09.28 bagus gantry --}--

	///// Static Control /////
	m_stcDarkRefPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcDarkRefPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcDarkRefPos_Z.SetBkColor(REFERENCE_COLOR);
	m_stcReflectRefPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcReflectRefPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcReflectRefPos_Z.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitAbsoluteRefPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitAbsoluteRefPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitAbsoluteRefPos_Z.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitFirstRefPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitFirstRefPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitFirstRefPos_Z.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitSecondRefPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitSecondRefPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcTransmitSecondRefPos_Z.SetBkColor(REFERENCE_COLOR);
//2009.09.28 bagus gantry --{--
	m_stcGantryTransmitRefPos_X.SetBkColor(REFERENCE_COLOR);
	m_stcGantryTransmitRefPos_Y.SetBkColor(REFERENCE_COLOR);
	m_stcGantryTransmitRefPos_Z.SetBkColor(REFERENCE_COLOR);
//2009.09.28 bagus gantry --}--

	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);
	m_DarkReferenceSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DarkReferenceSetupButton.DrawFlatFocus(TRUE);
	m_ReflectReferenceSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReflectReferenceSetupButton.DrawFlatFocus(TRUE);
	m_TransmitAbsoluteReferenceSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TransmitAbsoluteReferenceSetupButton.DrawFlatFocus(TRUE);
	m_TransmitFirstReferenceSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TransmitFirstReferenceSetupButton.DrawFlatFocus(TRUE);
	m_TransmitSecondReferenceSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TransmitSecondReferenceSetupButton.DrawFlatFocus(TRUE);
//2009.09.28 bagus gantry --{--
	m_GantryTransmitReferenceSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GantryTransmitReferenceSetupButton.DrawFlatFocus(TRUE);
//2009.09.28 bagus gantry --}--


//2009.09.28 bagus gantry --{--
#if 0
	char szTemp[256];
	SR_REFERENCE_POSITION SrReferencePosition;

	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	// Dark Reference Position
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocDarkReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_DARK_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocDarkReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_DARK_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocDarkReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_DARK_REFERENCE_POSITION_Z, szTemp);

	// Reflect Reference Position
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocReflectReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocReflectReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocReflectReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Z, szTemp);

	// Tramsmit Absolute Reference Position
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitAbsoluteReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitAbsoluteReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitAbsoluteReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z, szTemp);

	// Tramsmit First Reference Position
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitFirstReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitFirstReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitFirstReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z, szTemp);

	// Tramsmit Second Reference Position
	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitSecondReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitSecondReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(SrReferencePosition.LocTransmitSecondReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z, szTemp);
#else
	memset(&m_SrReferencePosition, 0, sizeof(m_SrReferencePosition));
	ConfigFile_GetNanoSpecIni(&m_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	// Dark Reference Position
	ShowLocDarkReference();

	// Reflect Reference Position
	ShowLocReflectReference();

	// Tramsmit Absolute Reference Position
	ShowLocTransmitAbsoluteReference();

	// Tramsmit First Reference Position
	ShowLocTransmitFirstReference();

	// Tramsmit Second Reference Position
	ShowLocTransmitSecondReference();

	// Gantry Transmit Reference Position
	ShowLocGantryTransmitReference(0);
#endif
//2009.09.28 bagus gantry --}--

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::OnOK()
{
//2009.09.28 bagus gantry --{--
#if 0
	CString csTemp;
	SR_REFERENCE_POSITION SrReferencePosition;

	///// Save /////
	// Dark Reference Position
	// X
	GetDlgItemText(IDC_DARK_REFERENCE_POSITION_X, csTemp);
	SrReferencePosition.LocDarkReference.lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_DARK_REFERENCE_POSITION_Y, csTemp);
	SrReferencePosition.LocDarkReference.lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_DARK_REFERENCE_POSITION_Z, csTemp);
	SrReferencePosition.LocDarkReference.lZ = (long)(atof(csTemp) * MICROMETRE);

	// Reflect Reference Position
	// X
	GetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_X, csTemp);
	SrReferencePosition.LocReflectReference.lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Y, csTemp);
	SrReferencePosition.LocReflectReference.lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Z, csTemp);
	SrReferencePosition.LocReflectReference.lZ = (long)(atof(csTemp) * MICROMETRE);

	// Transmit Absolute Reference Position
	// X
	GetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X, csTemp);
	SrReferencePosition.LocTransmitAbsoluteReference.lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y, csTemp);
	SrReferencePosition.LocTransmitAbsoluteReference.lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z, csTemp);
	SrReferencePosition.LocTransmitAbsoluteReference.lZ = (long)(atof(csTemp) * MICROMETRE);

	// Transmit First Reference Position
	// X
	GetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X, csTemp);
	SrReferencePosition.LocTransmitFirstReference.lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y, csTemp);
	SrReferencePosition.LocTransmitFirstReference.lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z, csTemp);
	SrReferencePosition.LocTransmitFirstReference.lZ = (long)(atof(csTemp) * MICROMETRE);

	// Transmit Second Reference Position
	// X
	GetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X, csTemp);
	SrReferencePosition.LocTransmitSecondReference.lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y, csTemp);
	SrReferencePosition.LocTransmitSecondReference.lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z, csTemp);
	SrReferencePosition.LocTransmitSecondReference.lZ = (long)(atof(csTemp) * MICROMETRE);

	ConfigFile_SetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);
#else
	///// Save /////
	ConfigFile_SetNanoSpecIni(&m_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);
	ConfigFile_SaveNanoSpecIni(USER_SETTING_SR_INI_FILE);
#endif
//2009.09.28 bagus gantry --}--
	CDialog::OnOK();
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::OnDarkReferenceSetupButton()
{
//2009.09.28 bagus gantry --{--
#if 0
	char szTemp[256];
	CString csTemp;
	COverlayDialog dlgOverlay;
	//Saiki 20090529 Add ----->
	CString strCap, strMsg;
	//Saiki 20090529 Add <-----

	// X
	GetDlgItemText(IDC_DARK_REFERENCE_POSITION_X, csTemp);
	dlgOverlay.m_lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_DARK_REFERENCE_POSITION_Y, csTemp);
	dlgOverlay.m_lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_DARK_REFERENCE_POSITION_Z, csTemp);
	dlgOverlay.m_lZ = (long)(atof(csTemp) * MICROMETRE);
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(DARK_REFERENCE_POSITION_SETTING_CAPTION);
	//dlgOverlay.SetMessage(DARK_REFERENCE_POSITION_SETTING_MESSAGE);
	LoadStringML(IDS_DARK_REFERENCE_POSITION_SETTING_CAPTION, strCap, "DARK REFERENCE POSITION SETTING");
	LoadStringML(IDS_DARK_REFERENCE_POSITION_SETTING_MESSAGE, strMsg, "DARK REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);
	//Saiki 20090529 Change <-----
	if(dlgOverlay.DoModal() == IDOK){
		// X
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lX)/MICROMETRE);
		SetDlgItemText(IDC_DARK_REFERENCE_POSITION_X, szTemp);
		// Y
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lY)/MICROMETRE);
		SetDlgItemText(IDC_DARK_REFERENCE_POSITION_Y, szTemp);
		// Z
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lZ)/MICROMETRE);
		SetDlgItemText(IDC_DARK_REFERENCE_POSITION_Z, szTemp);
	}
#else
	COverlayDialog dlgOverlay;
	CString strCap, strMsg;

	dlgOverlay.m_lX = m_SrReferencePosition.LocDarkReference.lX;
	dlgOverlay.m_lY = m_SrReferencePosition.LocDarkReference.lY;
	dlgOverlay.m_lZ = m_SrReferencePosition.LocDarkReference.lZ;
	dlgOverlay.m_StagePositionSetting = TRUE;

	LoadStringML(IDS_DARK_REFERENCE_POSITION_SETTING_CAPTION, strCap, "DARK REFERENCE POSITION SETTING");
	LoadStringML(IDS_DARK_REFERENCE_POSITION_SETTING_MESSAGE, strMsg, "DARK REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);

// 2010.01.13 bagus Gantry --{--
	dlgOverlay.m_bUseCheckScan = TRUE;
// 2010.01.13 bagus Gantry --}--

	if(dlgOverlay.DoModal() == IDOK){
		m_SrReferencePosition.LocDarkReference.lX = dlgOverlay.m_lX;
		m_SrReferencePosition.LocDarkReference.lY = dlgOverlay.m_lY;
		m_SrReferencePosition.LocDarkReference.lZ = dlgOverlay.m_lZ;
	}

	ShowLocDarkReference();
#endif
//2009.09.28 bagus gantry --}--
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::OnReflectReferenceSetupButton()
{
//2009.09.28 bagus gantry --{--
#if 0
	char szTemp[256];
	CString csTemp;
	COverlayDialog dlgOverlay;
	//Saiki 20090529 Add ----->
	CString strCap, strMsg;
	//Saiki 20090529 Add <-----

	// X
	GetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_X, csTemp);
	dlgOverlay.m_lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Y, csTemp);
	dlgOverlay.m_lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Z, csTemp);
	dlgOverlay.m_lZ = (long)(atof(csTemp) * MICROMETRE);
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(REFLECT_ABSOLUTE_REFERENCE_POSITION_CAPTION);
	//dlgOverlay.SetMessage(REFLECT_ABSOLUTE_REFERENCE_POSITION_MESSAGE);
	LoadStringML(IDS_REFLECT_ABSOLUTE_REFERENCE_POSITION_CAPTION, strCap, "REFLECT ABSOLUTE REFERENCE POSITION SETTING");
	LoadStringML(IDS_REFLECT_ABSOLUTE_REFERENCE_POSITION_MESSAGE, strMsg, "REFLECT ABSOLUTE REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);
	//Saiki 20090529 Change <-----
	if(dlgOverlay.DoModal() == IDOK){
		// X
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lX)/MICROMETRE);
		SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_X, szTemp);
		// Y
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lY)/MICROMETRE);
		SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Y, szTemp);
		// Z
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lZ)/MICROMETRE);
		SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Z, szTemp);
	}
#else
	COverlayDialog dlgOverlay;
	CString strCap, strMsg;

	dlgOverlay.m_lX = m_SrReferencePosition.LocReflectReference.lX;
	dlgOverlay.m_lY = m_SrReferencePosition.LocReflectReference.lY;
	dlgOverlay.m_lZ = m_SrReferencePosition.LocReflectReference.lZ;
	dlgOverlay.m_StagePositionSetting = TRUE;

	LoadStringML(IDS_REFLECT_ABSOLUTE_REFERENCE_POSITION_CAPTION, strCap, "REFLECT ABSOLUTE REFERENCE POSITION SETTING");
	LoadStringML(IDS_REFLECT_ABSOLUTE_REFERENCE_POSITION_MESSAGE, strMsg, "REFLECT ABSOLUTE REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);

// 2010.01.13 bagus Gantry --{--
	dlgOverlay.m_bUseCheckScan = TRUE;
// 2010.01.13 bagus Gantry --}--

	if(dlgOverlay.DoModal() == IDOK){
		m_SrReferencePosition.LocReflectReference.lX = dlgOverlay.m_lX;
		m_SrReferencePosition.LocReflectReference.lY = dlgOverlay.m_lY;
		m_SrReferencePosition.LocReflectReference.lZ = dlgOverlay.m_lZ;
	}

	ShowLocReflectReference();
#endif
//2009.09.28 bagus gantry --}--
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::OnTransmitAbsoluteReferenceSetupButton()
{
//2009.09.28 bagus gantry --{--
#if 0
	char szTemp[256];
	CString csTemp;
	COverlayDialog dlgOverlay;
	//Saiki 20090529 Add ----->
	CString strCap, strMsg;
	//Saiki 20090529 Add <-----

	// X
	GetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X, csTemp);
	dlgOverlay.m_lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y, csTemp);
	dlgOverlay.m_lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z, csTemp);
	dlgOverlay.m_lZ = (long)(atof(csTemp) * MICROMETRE);
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(TRANSMIT_ABSOLUTE_REFERENCE_POSITION_CAPTION);
	//dlgOverlay.SetMessage(TRANSMIT_ABSOLUTE_REFERENCE_POSITION_MESSAGE);
	LoadStringML(IDS_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_CAPTION, strCap, "TRANSMIT ABSOLUTE REFERENCE POSITION SETTING");
	LoadStringML(IDS_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_MESSAGE, strMsg, "TRANSMIT ABSOLUTE REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);
	//Saiki 20090529 Change <-----

	if(dlgOverlay.DoModal() == IDOK){
		// X
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lX)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X, szTemp);
		// Y
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lY)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y, szTemp);
		// Z
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lZ)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z, szTemp);
	}
#else
	COverlayDialog dlgOverlay;
	CString strCap, strMsg;

	dlgOverlay.m_lX = m_SrReferencePosition.LocTransmitAbsoluteReference.lX;
	dlgOverlay.m_lY = m_SrReferencePosition.LocTransmitAbsoluteReference.lY;
	dlgOverlay.m_lZ = m_SrReferencePosition.LocTransmitAbsoluteReference.lZ;
	dlgOverlay.m_StagePositionSetting = TRUE;

	LoadStringML(IDS_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_CAPTION, strCap, "TRANSMIT ABSOLUTE REFERENCE POSITION SETTING");
	LoadStringML(IDS_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_MESSAGE, strMsg, "TRANSMIT ABSOLUTE REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);

// 2010.01.13 bagus Gantry --{--
	dlgOverlay.m_bUseCheckScan = TRUE;
// 2010.01.13 bagus Gantry --}--

	if(dlgOverlay.DoModal() == IDOK){
		m_SrReferencePosition.LocTransmitAbsoluteReference.lX = dlgOverlay.m_lX;
		m_SrReferencePosition.LocTransmitAbsoluteReference.lY = dlgOverlay.m_lY;
		m_SrReferencePosition.LocTransmitAbsoluteReference.lZ = dlgOverlay.m_lZ;
	}

	ShowLocTransmitAbsoluteReference();
#endif
//2009.09.28 bagus gantry --}--
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::OnTransmitFirstReferenceSetupButton()
{
//2009.09.28 bagus gantry --{--
#if 0
	char szTemp[256];
	CString csTemp;
	COverlayDialog dlgOverlay;
	//Saiki 20090529 Add ----->
	CString strCap, strMsg;
	//Saiki 20090529 Add <-----

	// X
	GetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X, csTemp);
	dlgOverlay.m_lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y, csTemp);
	dlgOverlay.m_lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z, csTemp);
	dlgOverlay.m_lZ = (long)(atof(csTemp) * MICROMETRE);
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(TRANSMIT_1ST_REFERENCE_POSITION_CAPTION);
	//dlgOverlay.SetMessage(TRANSMIT_1ST_REFERENCE_POSITION_MESSAGE);
	LoadStringML(IDS_TRANSMIT_1ST_REFERENCE_POSITION_CAPTION, strCap, "TRANSMIT 1ST REFERENCE POSITION SETTING");
	LoadStringML(IDS_TRANSMIT_1ST_REFERENCE_POSITION_MESSAGE, strMsg, "TRANSMIT 1ST REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);
	//Saiki 20090529 Change <-----

	if(dlgOverlay.DoModal() == IDOK){
		// X
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lX)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X, szTemp);
		// Y
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lY)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y, szTemp);
		// Z
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lZ)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z, szTemp);
	}
#else
	COverlayDialog dlgOverlay;
	CString strCap, strMsg;

	dlgOverlay.m_lX = m_SrReferencePosition.LocTransmitFirstReference.lX;
	dlgOverlay.m_lY = m_SrReferencePosition.LocTransmitFirstReference.lY;
	dlgOverlay.m_lZ = m_SrReferencePosition.LocTransmitFirstReference.lZ;
	dlgOverlay.m_StagePositionSetting = TRUE;

	LoadStringML(IDS_TRANSMIT_1ST_REFERENCE_POSITION_CAPTION, strCap, "TRANSMIT 1ST REFERENCE POSITION SETTING");
	LoadStringML(IDS_TRANSMIT_1ST_REFERENCE_POSITION_MESSAGE, strMsg, "TRANSMIT 1ST REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);

// 2010.01.13 bagus Gantry --{--
	dlgOverlay.m_bUseCheckScan = TRUE;
// 2010.01.13 bagus Gantry --}--

	if(dlgOverlay.DoModal() == IDOK){
		m_SrReferencePosition.LocTransmitFirstReference.lX = dlgOverlay.m_lX;
		m_SrReferencePosition.LocTransmitFirstReference.lY = dlgOverlay.m_lY;
		m_SrReferencePosition.LocTransmitFirstReference.lZ = dlgOverlay.m_lZ;
	}

	ShowLocTransmitFirstReference();
#endif
//2009.09.28 bagus gantry --}--
}

// =========================================================================
//
void CSrReferencePositionSettingDlg::OnTransmitSecondReferenceSetupButton()
{
//2009.09.28 bagus gantry --{--
#if 0
	char szTemp[256];
	CString csTemp;
	COverlayDialog dlgOverlay;
	//Saiki 20090529 Add ----->
	CString strCap, strMsg;
	//Saiki 20090529 Add <-----

	// X
	GetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X, csTemp);
	dlgOverlay.m_lX = (long)(atof(csTemp) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y, csTemp);
	dlgOverlay.m_lY = (long)(atof(csTemp) * MICROMETRE);
	// Z
	GetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z, csTemp);
	dlgOverlay.m_lZ = (long)(atof(csTemp) * MICROMETRE);
// INS 2009.07.07 by kawamura
	dlgOverlay.m_StagePositionSetting = TRUE;
// INS 2009.07.07 by kawamura

	//Saiki 20090529 Change ----->
	//dlgOverlay.SetCaption(TRANSMIT_2ND_REFERENCE_POSITION_CAPTION);
	//dlgOverlay.SetMessage(TRANSMIT_2ND_REFERENCE_POSITION_MESSAGE);
	LoadStringML(IDS_TRANSMIT_2ND_REFERENCE_POSITION_CAPTION, strCap, "TRANSMIT 2ND REFERENCE POSITION SETTING");
	LoadStringML(IDS_TRANSMIT_2ND_REFERENCE_POSITION_MESSAGE, strMsg, "TRANSMIT 2ND REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);
	//Saiki 20090529 Change <-----

	if(dlgOverlay.DoModal() == IDOK){
		// X
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lX)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X, szTemp);
		// Y
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lY)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y, szTemp);
		// Z
		memset(szTemp, 0, sizeof(szTemp));
		sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(dlgOverlay.m_lZ)/MICROMETRE);
		SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z, szTemp);
	}
#else
	COverlayDialog dlgOverlay;
	CString strCap, strMsg;

	dlgOverlay.m_lX = m_SrReferencePosition.LocTransmitSecondReference.lX;
	dlgOverlay.m_lY = m_SrReferencePosition.LocTransmitSecondReference.lY;
	dlgOverlay.m_lZ = m_SrReferencePosition.LocTransmitSecondReference.lZ;
	dlgOverlay.m_StagePositionSetting = TRUE;

	LoadStringML(IDS_TRANSMIT_2ND_REFERENCE_POSITION_CAPTION, strCap, "TRANSMIT 2ND REFERENCE POSITION SETTING");
	LoadStringML(IDS_TRANSMIT_2ND_REFERENCE_POSITION_MESSAGE, strMsg, "TRANSMIT 2ND REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);

// 2010.01.13 bagus Gantry --{--
	dlgOverlay.m_bUseCheckScan = TRUE;
// 2010.01.13 bagus Gantry --}--

	if(dlgOverlay.DoModal() == IDOK){
		m_SrReferencePosition.LocTransmitSecondReference.lX = dlgOverlay.m_lX;
		m_SrReferencePosition.LocTransmitSecondReference.lY = dlgOverlay.m_lY;
		m_SrReferencePosition.LocTransmitSecondReference.lZ = dlgOverlay.m_lZ;
	}

	ShowLocTransmitSecondReference();
#endif
//2009.09.28 bagus gantry --{--
}

//2009.09.28 bagus gantry --{--
void CSrReferencePositionSettingDlg::OnSelchangePositionNo()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_POSITION_NO);
	int	nIndex = pCombo->GetCurSel();

	ShowLocGantryTransmitReference(nIndex);
}

void CSrReferencePositionSettingDlg::OnGantryTransmitReferenceSetupButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	COverlayDialog dlgOverlay;
	CString strCap, strMsg;

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_POSITION_NO);
	int	nIndex = pCombo->GetCurSel();

	if (nIndex == CB_ERR)
		return;

	if (nIndex >= MAX_GANTRY_MEAS_POINT_NUM)
		return;

	dlgOverlay.m_lX = m_SrReferencePosition.LocGantryTransmitReference[nIndex].lX;
	dlgOverlay.m_lY = m_SrReferencePosition.LocGantryTransmitReference[nIndex].lY;
	dlgOverlay.m_lZ = m_SrReferencePosition.LocGantryTransmitReference[nIndex].lZ;
	dlgOverlay.m_StagePositionSetting = TRUE;

	LoadStringML(IDS_GANTRY_TRANSMIT_REFERENCE_POSITION_CAPTION, strCap, "GANTRY TRANSMIT REFERENCE POSITION SETTING");
	LoadStringML(IDS_GANTRY_TRANSMIT_REFERENCE_POSITION_MESSAGE, strMsg, "GANTRY TRANSMIT REFERENCE POSITION SETTING");
	dlgOverlay.SetCaption(strCap);
	dlgOverlay.SetMessage(strMsg);

// 2010.01.07 bagus Gantry --{--
	dlgOverlay.m_bGantry = TRUE;
// 2010.01.07 bagus Gantry --}--
// 2010.01.13 bagus Gantry --{--
	dlgOverlay.m_bUseCheckScan = TRUE;
// 2010.01.13 bagus Gantry --}--

	if(dlgOverlay.DoModal() == IDOK){
		m_SrReferencePosition.LocGantryTransmitReference[nIndex].lX = dlgOverlay.m_lX;
		m_SrReferencePosition.LocGantryTransmitReference[nIndex].lY = dlgOverlay.m_lY;
		m_SrReferencePosition.LocGantryTransmitReference[nIndex].lZ = dlgOverlay.m_lZ;
	}

	ShowLocGantryTransmitReference(nIndex);
}

void CSrReferencePositionSettingDlg::ShowLocDarkReference()
{
	char szTemp[256];

	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocDarkReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_DARK_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocDarkReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_DARK_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocDarkReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_DARK_REFERENCE_POSITION_Z, szTemp);
}

void CSrReferencePositionSettingDlg::ShowLocReflectReference()
{
	char szTemp[256];

	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocReflectReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocReflectReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocReflectReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_REFLECT_REFERENCE_POSITION_Z, szTemp);
}

void CSrReferencePositionSettingDlg::ShowLocTransmitAbsoluteReference()
{
	char szTemp[256];

	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitAbsoluteReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitAbsoluteReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitAbsoluteReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_ABSOLUTE_REFERENCE_POSITION_Z, szTemp);
}

void CSrReferencePositionSettingDlg::ShowLocTransmitFirstReference()
{
	char szTemp[256];

	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitFirstReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitFirstReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitFirstReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_FIRST_REFERENCE_POSITION_Z, szTemp);
}

void CSrReferencePositionSettingDlg::ShowLocTransmitSecondReference()
{
	char szTemp[256];

	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitSecondReference.lX)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitSecondReference.lY)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocTransmitSecondReference.lZ)/MICROMETRE);
	SetDlgItemText(IDC_TRANSMIT_SECOND_REFERENCE_POSITION_Z, szTemp);
}

void CSrReferencePositionSettingDlg::ShowLocGantryTransmitReference(int nIndex)
{
	char szTemp[256];

	if (nIndex < 0)
		return;

	if (nIndex >= MAX_GANTRY_MEAS_POINT_NUM)
		return;

	// X
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocGantryTransmitReference[nIndex].lX)/MICROMETRE);
	SetDlgItemText(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_X, szTemp);
	// Y
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocGantryTransmitReference[nIndex].lY)/MICROMETRE);
	SetDlgItemText(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Y, szTemp);
	// Z
	memset(szTemp, 0, sizeof(szTemp));
	sprintf_s(szTemp, _countof(szTemp), "%.3lf", (double)(m_SrReferencePosition.LocGantryTransmitReference[nIndex].lZ)/MICROMETRE);
	SetDlgItemText(IDC_GANTRY_TRANSMIT_REFERENCE_POSITION_Z, szTemp);
}
//2009.09.28 bagus gantry --}--
