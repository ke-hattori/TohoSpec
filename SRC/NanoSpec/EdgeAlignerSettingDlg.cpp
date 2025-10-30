// EdgeAlignerSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "EdgeAlignerSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CEdgeAlignerSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CEdgeAlignerSettingDlg::CEdgeAlignerSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEdgeAlignerSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdgeAlignerSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CEdgeAlignerSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdgeAlignerSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_X_ALIGNER1_X, m_EdgeAligner.Sensor.X[0].lX);
	DDX_Text(pDX, IDC_X_ALIGNER1_Y, m_EdgeAligner.Sensor.X[0].lY);
	DDX_Text(pDX, IDC_X_ALIGNER2_X, m_EdgeAligner.Sensor.X[1].lX);
	DDX_Text(pDX, IDC_X_ALIGNER2_Y, m_EdgeAligner.Sensor.X[1].lY);
	DDX_Text(pDX, IDC_Y_ALIGNER1_X, m_EdgeAligner.Sensor.Y[0].lX);
	DDX_Text(pDX, IDC_Y_ALIGNER1_Y, m_EdgeAligner.Sensor.Y[0].lY);
	DDX_Text(pDX, IDC_Y_ALIGNER2_X, m_EdgeAligner.Sensor.Y[1].lX);
	DDX_Text(pDX, IDC_Y_ALIGNER2_Y, m_EdgeAligner.Sensor.Y[1].lY);
	DDX_Text(pDX, IDC_START_X, m_EdgeAligner.Start.lX);
	DDX_Text(pDX, IDC_START_Y, m_EdgeAligner.Start.lY);
	DDX_Text(pDX, IDC_END_X, m_EdgeAligner.End.lX);
	DDX_Text(pDX, IDC_END_Y, m_EdgeAligner.End.lY);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_START_SETUP_BUTTON, m_StartSetupButton);
	DDX_Control(pDX, IDC_END_SETUP_BUTTON, m_EndSetupButton);
	DDX_Control(pDX, IDC_SENSOR_POSITION_SCAN_BUTTON, m_SensorPositionScanButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CEdgeAlignerSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CEdgeAlignerSettingDlg)
	ON_BN_CLICKED(IDC_START_SETUP_BUTTON, OnStartSetupButton)
	ON_BN_CLICKED(IDC_END_SETUP_BUTTON, OnEndSetupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CEdgeAlignerSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CEdgeAlignerSettingDlg::OnInitDialog()
{
	////// Load File /////
	ConfigFile_GetNanoSpecIni(&m_EdgeAligner, CONFIG_FILE_EDGE_ALIGNER);

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Start Setup Button /////
	m_StartSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_StartSetupButton.DrawFlatFocus(TRUE);

	///// End Setup Button /////
	m_EndSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_EndSetupButton.DrawFlatFocus(TRUE);

	///// Sensor Position Scan Button /////
	m_SensorPositionScanButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SensorPositionScanButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_stcStartX.SubclassWindow(GetDlgItem(IDC_START_X)->GetSafeHwnd());
	m_stcStartX.SetBkColor(REFERENCE_COLOR);
	m_stcStartY.SubclassWindow(GetDlgItem(IDC_START_Y)->GetSafeHwnd());
	m_stcStartY.SetBkColor(REFERENCE_COLOR);
	m_stcEndX.SubclassWindow(GetDlgItem(IDC_END_X)->GetSafeHwnd());
	m_stcEndX.SetBkColor(REFERENCE_COLOR);
	m_stcEndY.SubclassWindow(GetDlgItem(IDC_END_Y)->GetSafeHwnd());
	m_stcEndY.SetBkColor(REFERENCE_COLOR);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CEdgeAlignerSettingDlg::OnOK()
{
	///// Save /////
	if ( !CheckData() )
		return;

	ConfigFile_SetNanoSpecIni(&m_EdgeAligner, CONFIG_FILE_EDGE_ALIGNER);
	ConfigFile_SaveNanoSpecIni(SYSTEM_COMMON_INI_FILE);

	CDialog::OnOK();
}

// =========================================================================
//
void CEdgeAlignerSettingDlg::OnStartSetupButton()
{
	if(UpdateData(TRUE) == FALSE) return;

	COverlayDialog dlgOverlay;

	dlgOverlay.m_lX = m_EdgeAligner.Start.lX;
	dlgOverlay.m_lY = m_EdgeAligner.Start.lY;

	if ( dlgOverlay.DoModal() == IDOK ) {
		m_EdgeAligner.Start.lX = dlgOverlay.m_lX;
		m_EdgeAligner.Start.lY = dlgOverlay.m_lY;
	}

	UpdateData(FALSE);
}

// =========================================================================
//
void CEdgeAlignerSettingDlg::OnEndSetupButton()
{
	if(UpdateData(TRUE) == FALSE) return;

	COverlayDialog dlgOverlay;

	dlgOverlay.m_lX = m_EdgeAligner.End.lX;
	dlgOverlay.m_lY = m_EdgeAligner.End.lY;

	if ( dlgOverlay.DoModal() == IDOK ) {
		m_EdgeAligner.End.lX = dlgOverlay.m_lX;
		m_EdgeAligner.End.lY = dlgOverlay.m_lY;
	}

	UpdateData(FALSE);
}
// =========================================================================
//
BOOL CEdgeAlignerSettingDlg::CheckData()
{
	if(UpdateData(TRUE) == FALSE) return FALSE;

	CString strBuffer;
	long lMin = 0;
	long lMax = 999999;

	// X Aligner1
	GetDlgItemText(IDC_X_ALIGNER1_X, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_X_ALIGNER1_X)->SetFocus();
		return FALSE;
	}

	GetDlgItemText(IDC_X_ALIGNER1_Y, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_X_ALIGNER1_Y)->SetFocus();
		return FALSE;
	}

	// X Aligner2
	GetDlgItemText(IDC_X_ALIGNER2_X, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_X_ALIGNER2_X)->SetFocus();
		return FALSE;
	}

	GetDlgItemText(IDC_X_ALIGNER2_Y, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_X_ALIGNER2_Y)->SetFocus();
		return FALSE;
	}

	// Y Aligner1
	GetDlgItemText(IDC_Y_ALIGNER1_X, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_Y_ALIGNER1_X)->SetFocus();
		return FALSE;
	}

	GetDlgItemText(IDC_Y_ALIGNER1_Y, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_Y_ALIGNER1_Y)->SetFocus();
		return FALSE;
	}

	// Y Aligner2
	GetDlgItemText(IDC_Y_ALIGNER2_X, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_Y_ALIGNER2_X)->SetFocus();
		return FALSE;
	}

	GetDlgItemText(IDC_Y_ALIGNER2_Y, strBuffer);
	if(CheckMinMaxLong(strBuffer, lMin, lMax) != CHECK_DATA_OK){
		GetDlgItem(IDC_Y_ALIGNER2_Y)->SetFocus();
		return FALSE;
	}


	return TRUE;
}

