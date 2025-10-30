// SystemOperationSrSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SrCofigurationSettingDlg.h"
#include "SrFilterSettingDlg.h"
#include "SrTurretSettingDlg.h"
#include "SrXmpSettingDlg.h"
#include "SrViewRangeSettingDlg.h"
#include "SrLensCenterOffsetSetingDlg.h"
#include "SrVideoCalibrationSettingDlg.h"
#include "SrCcdArraySettingDlg.h"
#include "SystemOperationSrSettingDlg.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSystemOperationSrSettingDlg ダイアログ

// =========================================================================
//
CSystemOperationSrSettingDlg::CSystemOperationSrSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemOperationSrSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemOperationSrSettingDlg)
	//}}AFX_DATA_INIT
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemOperationSrSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SR_CONFIGURATION_SETTING_BUTTON, m_SrConfigurationSettingButton);
	DDX_Control(pDX, IDC_FILTER_SETTING_BUTTON, m_FilterSettingButton);
	DDX_Control(pDX, IDC_TURRET_SETTING_BUTTON, m_TurretSettingButton);
	DDX_Control(pDX, IDC_XMP_SETTING_BUTTON, m_XmpSettingButton);
	DDX_Control(pDX, IDC_VIEW_RANGE_SETTING_BUTTON, m_ViewRangeSettingButton);
	DDX_Control(pDX, IDC_LENS_SETTING_BUTTON, m_LensSettingButton);
	DDX_Control(pDX, IDC_VIDEO_CALIB_SETTING_BUTTON, m_VideoCalibSettingButton);
	DDX_Control(pDX, IDC_CCD_ARRAY_SETTING_BUTTON, m_CcdArraySettingButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSystemOperationSrSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemOperationSrSettingDlg)
	ON_BN_CLICKED(IDC_SR_CONFIGURATION_SETTING_BUTTON, OnSrConfigurationSettingButton)
	ON_BN_CLICKED(IDC_FILTER_SETTING_BUTTON, OnFilterSettingButton)
	ON_BN_CLICKED(IDC_TURRET_SETTING_BUTTON, OnTurretSettingButton)
	ON_BN_CLICKED(IDC_XMP_SETTING_BUTTON, OnXmpSettingButton)
	ON_BN_CLICKED(IDC_VIEW_RANGE_SETTING_BUTTON, OnViewRangeSettingButton)
	ON_BN_CLICKED(IDC_LENS_SETTING_BUTTON, OnLensSettingButton)
	ON_BN_CLICKED(IDC_CCD_ARRAY_SETTING_BUTTON, OnCcdArraySettingButton)
	ON_BN_CLICKED(IDC_VIDEO_CALIB_SETTING_BUTTON, OnVideoCalibSettingButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSystemOperationSrSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSystemOperationSrSettingDlg::OnInitDialog()
{
	//未対応機能HIDE
	GetDlgItem(IDC_VIEW_RANGE_SETTING_BUTTON)->ShowWindow(SW_HIDE);

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// SR Configuration Setting Button /////
	m_SrConfigurationSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SrConfigurationSettingButton.DrawFlatFocus(TRUE);

	///// Filter Setting Button /////
	m_FilterSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_FilterSettingButton.DrawFlatFocus(TRUE);

	///// Turret Setting Button /////
	m_TurretSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TurretSettingButton.DrawFlatFocus(TRUE);

	///// XMP Setting Button /////
	m_XmpSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_XmpSettingButton.DrawFlatFocus(TRUE);

	///// View Range Setting Button /////
	m_ViewRangeSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ViewRangeSettingButton.DrawFlatFocus(TRUE);

	///// Lens Setting Button /////
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//	m_LensSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
//		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
//	m_LensSettingButton.DrawFlatFocus(TRUE);
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_LENS_SETTING_BUTTON)->ShowWindow(SW_HIDE);
	}
	else{
		m_LensSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_LensSettingButton.DrawFlatFocus(TRUE);
	}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--

	///// Video Calibration Setting Button /////
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//	m_VideoCalibSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
//		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
//	m_VideoCalibSettingButton.DrawFlatFocus(TRUE);
	if(g_lModelType == MODEL_T3100){
		GetDlgItem(IDC_VIDEO_CALIB_SETTING_BUTTON)->ShowWindow(SW_HIDE);
	}
	else{
		m_VideoCalibSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_VideoCalibSettingButton.DrawFlatFocus(TRUE);
	}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--

	///// CCD Array Setting Button /////
	m_CcdArraySettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CcdArraySettingButton.DrawFlatFocus(TRUE);

	SetTimer(1, 50, NULL);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
BOOL CSystemOperationSrSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	switch(pMsg->message){
	case WM_KEYDOWN:
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		break;
	case WM_SYSKEYDOWN:
		if(pMsg->wParam == VK_F4)
			return TRUE;
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnSrConfigurationSettingButton()
{
	CSrCofigurationSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnFilterSettingButton()
{
	CSrFilterSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnTurretSettingButton()
{
	CSrTurretSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnXmpSettingButton()
{
	CSrXmpSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnViewRangeSettingButton()
{
	CSrViewRangeSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnLensSettingButton()
{
	CSrLensCenterOffsetSetingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnVideoCalibSettingButton()
{
	CSrVideoCalibrationSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationSrSettingDlg::OnCcdArraySettingButton()
{
	CSrCcdArraySettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
BOOL CSystemOperationSrSettingDlg::ShowSettingDlg(CDialog* pDlg)
{
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( !pDoc->User_Access(ACCESS_SYSTEMCONFIG) ) {
//Saiki 20090128 Change ----->
//		MessageBox(USER_ACCESS_STOP_TEXT, USER_ACCESS_STOP_CAPTION, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
//Saiki 20090128 Change <-----
		return FALSE;
	}

	return ( pDlg->DoModal() == IDOK );
}

HBRUSH CSystemOperationSrSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

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

void CSystemOperationSrSettingDlg::OnTimer(UINT nIDEvent)
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

	CDialog::OnTimer(nIDEvent);
}
