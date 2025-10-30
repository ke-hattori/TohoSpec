// SystemOperationCommonSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "StageConfigSettingDlg.h"
#include "OriginalPositionSettingDlg.h"
#include "HandlerLoadPositionSettingDlg.h"
#include "GeneralPositionSettingDlg.h"
#include "SampleListDlg.h"
#include "FocusConfigurationSettingDlg.h"
#include "EdgeAlignerSettingDlg.h"
#include "ZAxisOffsetSettingDlg.h"
#include "SignalTowerSettingDlg.h"
#include "SystemConfigurationSettingDlg.h"
#include "SystemOperationCommonSettingDlg.h"
#include "HostSetupDlg.h"
#include "ComSetupDlg.h"
#include "JogDirSettingDlg.h"
#include "SubstrateThicknessSettingListDlg.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSystemOperationCommonSettingDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSystemOperationCommonSettingDlg::CSystemOperationCommonSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemOperationCommonSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemOperationCommonSettingDlg)
	//}}AFX_DATA_INIT
}


// =========================================================================
//
void CSystemOperationCommonSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemOperationCommonSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STAGE_COORDINATE_SETTING_BUTTON, m_StageCoordinateSettingButton);
	DDX_Control(pDX, IDC_ORIGINAL_POSITION_SETTING_BUTTON, m_OriginalPositionSettingButton);
	DDX_Control(pDX, IDC_HANDLER_LOAD_POSITION_SETTING_BUTTON, m_HandlerLoadPositionSettingButton);
	DDX_Control(pDX, IDC_GENERAL_POSITION_SETTING_BUTTON, m_GeneralPositionSettingButton);
	DDX_Control(pDX, IDC_SAMPLE_SETTING_BUTTON, m_SampleSettingButton);
	DDX_Control(pDX, IDC_SUBSTRATE_THICKNESS_SETTING_LIST_BUTTON, m_SubstrateThicknessSettingListButton);
	DDX_Control(pDX, IDC_EDGE_ALIGNER_SETTING_BUTTON, m_EdgeAlignerSettingButton);
	DDX_Control(pDX, IDC_ZAXIS_OFFSET_SETTING_BUTTON, m_ZAxisOffsetSettingButton);
	DDX_Control(pDX, IDC_HOST_SETTING_BUTTON, m_HostSettingButton);
	DDX_Control(pDX, IDC_COM_SETTING_BUTTON, m_ComSettingButton);
	DDX_Control(pDX, IDC_SIGNAL_TOWER_SETTING_BUTTON, m_SignalTowerSettingButton);
	DDX_Control(pDX, IDC_SYSTEM_CONFIGURATION_SETTING_BUTTON, m_SystemConfigurationSettingButton);
	DDX_Control(pDX, IDC_JOG_DIRECTION_SETTING_BUTTON, m_JogDirectionSettingButton);
}


// =========================================================================
//
BEGIN_MESSAGE_MAP(CSystemOperationCommonSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemOperationCommonSettingDlg)
	ON_BN_CLICKED(IDC_ORIGINAL_POSITION_SETTING_BUTTON, OnOriginalPositionSettingButton)
	ON_BN_CLICKED(IDC_HANDLER_LOAD_POSITION_SETTING_BUTTON, OnHandlerLoadPositionSettingButton)
	ON_BN_CLICKED(IDC_GENERAL_POSITION_SETTING_BUTTON, OnGeneralPositionSettingButton)
	ON_BN_CLICKED(IDC_SAMPLE_SETTING_BUTTON, OnSampleSettingButton)
	ON_BN_CLICKED(IDC_SUBSTRATE_THICKNESS_SETTING_LIST_BUTTON, OnSubstrateThicknessSettingListButton)
	ON_BN_CLICKED(IDC_EDGE_ALIGNER_SETTING_BUTTON, OnEdgeAlignerSettingButton)
	ON_BN_CLICKED(IDC_ZAXIS_OFFSET_SETTING_BUTTON, OnZAxisOffsetSettingButton)
	ON_BN_CLICKED(IDC_HOST_SETTING_BUTTON, OnHostSettingButton)
	ON_BN_CLICKED(IDC_COM_SETTING_BUTTON, OnComSettingButton)
	ON_BN_CLICKED(IDC_SIGNAL_TOWER_SETTING_BUTTON, OnSignalTowerSettingButton)
	ON_BN_CLICKED(IDC_STAGE_CONFIG_SETTING_BUTTON, OnStageConfigSettingButton)
	ON_BN_CLICKED(IDC_SYSTEM_CONFIGURATION_SETTING_BUTTON, OnSystemConfigurationSettingButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_JOG_DIRECTION_SETTING_BUTTON, OnJogDirectionSettingButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSystemOperationCommonSettingDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSystemOperationCommonSettingDlg::OnInitDialog()
{
// 2013.02.15 bagus Substrate thickness setting -->
// 	//未対応機能HIDE
// 	GetDlgItem(IDC_FOCUS_CONFIGURATION_SETTING_BUTTON)->ShowWindow(SW_HIDE);
// 2013.02.15 bagus Substrate thickness setting <--
	//Saiki 20090527 Delete ---->
	//GetDlgItem(IDC_GENERAL_POSITION_SETTING_BUTTON)->ShowWindow(SW_HIDE);
	//Saiki 20090527 Delete <----
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	//ボタン系（配置、表示有無、色等）のイニシャル処理
	InitButton();

	SetTimer(1, 50, NULL);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnSystemConfigurationSettingButton()
{
	CSystemConfigurationSettingDlg dlg;
	ShowSettingDlg(&dlg);
	InitButton();
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnOriginalPositionSettingButton()
{
	COriginalPositionSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnStageConfigSettingButton()
{
	CStageConfigSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnHandlerLoadPositionSettingButton()
{
	CHandlerLoadPositionSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnGeneralPositionSettingButton()
{
	CGeneralPositionSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnSampleSettingButton()
{
	CSampleListDlg dlg;
	ShowSettingDlg(&dlg);
}

// 2013.02.15 bagus Substrate thickness setting -->
// // =========================================================================
// //
// void CSystemOperationCommonSettingDlg::OnFocusConfigurationSettingButton()
// {
// 	CFocusConfigurationSettingDlg dlg;
// 	ShowSettingDlg(&dlg);
// }
// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnSubstrateThicknessSettingListButton()
{
	CSubstrateThicknessSettingListDlg dlg;
	ShowSettingDlg(&dlg);
}
// 2013.02.15 bagus Substrate thickness setting <--

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnEdgeAlignerSettingButton()
{
	CEdgeAlignerSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnZAxisOffsetSettingButton()
{
	// ヘッドはSR, SE, IRSEの順で決めてしまう
	// 通常の測定と同じ処理を行い，Z_AxisData.dat，Z_AxisData.txtを作成する

	CZAxisOffsetSettingDlg dlg;
	if ( ShowSettingDlg(&dlg) ) {
		CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
		pMainFrame->ChangeOperationPanel(OP_MEASUREMENT_PROCESS);
		pMainFrame->ChangeExeWnd(SYSCONF_Z_AXIS_OFFSET_MEAS_WND);
	}
}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnHostSettingButton()
{
	CHostSetupDlg dlg;
	ShowSettingDlg(&dlg);

}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnComSettingButton()
{
	CComSetupDlg dlg;
	ShowSettingDlg(&dlg);

}

// =========================================================================
//
void CSystemOperationCommonSettingDlg::OnSignalTowerSettingButton()
{
	CSignalTowerSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

// =========================================================================
//
BOOL CSystemOperationCommonSettingDlg::PreTranslateMessage(MSG* pMsg)
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
BOOL CSystemOperationCommonSettingDlg::ShowSettingDlg(CDialog* pDlg)
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


void CSystemOperationCommonSettingDlg::InitButton()
{
	///// Load System Config File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// System Configuration Setting Button /////
	m_SystemConfigurationSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SystemConfigurationSettingButton.DrawFlatFocus(TRUE);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_StageCoordinateSettingButton.ShowWindow(SW_HIDE);
		m_OriginalPositionSettingButton.ShowWindow(SW_HIDE);
		m_HandlerLoadPositionSettingButton.ShowWindow(SW_HIDE);
		m_GeneralPositionSettingButton.ShowWindow(SW_HIDE);
		m_SampleSettingButton.ShowWindow(SW_HIDE);
		m_SubstrateThicknessSettingListButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// Stage Coordinate Setting Button /////
		m_StageCoordinateSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_StageCoordinateSettingButton.DrawFlatFocus(TRUE);

		///// Original Position Setting Button /////
		m_OriginalPositionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_OriginalPositionSettingButton.DrawFlatFocus(TRUE);

		///// Handler Load Position Setting Button /////
		m_HandlerLoadPositionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_HandlerLoadPositionSettingButton.DrawFlatFocus(TRUE);

		///// General Position Setting Button /////
		m_GeneralPositionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_GeneralPositionSettingButton.DrawFlatFocus(TRUE);

		///// Sample Setting Button /////
		m_SampleSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_SampleSettingButton.DrawFlatFocus(TRUE);

// 2013.02.15 bagus Substrate thickness setting -->
		///// Substrate Thickness Setting List Button /////
		m_SubstrateThicknessSettingListButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_SubstrateThicknessSettingListButton.DrawFlatFocus(TRUE);
// 2013.02.15 bagus Substrate thickness setting <--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Edge Aligner Setting Button /////
	m_EdgeAlignerSettingButton.ShowWindow(SW_HIDE);
	//m_EdgeAlignerSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
	//	BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	//m_EdgeAlignerSettingButton.DrawFlatFocus(TRUE);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_ZAxisOffsetSettingButton.ShowWindow(SW_HIDE);
		m_HostSettingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		///// ZAxis Offset Setting Button /////
		m_ZAxisOffsetSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_ZAxisOffsetSettingButton.DrawFlatFocus(TRUE);

		///// Host Setting Button /////
		if (m_SystemConfig.bHost){
			m_HostSettingButton.ShowWindow(SW_SHOWDEFAULT);
			m_HostSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
				BUTTON_COLORING, BUTTON_NORMAL_COLOR);
			m_HostSettingButton.DrawFlatFocus(TRUE);
		}
		else{
			m_HostSettingButton.ShowWindow(SW_HIDE);
		}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Com Setting Button /////
	m_ComSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ComSettingButton.DrawFlatFocus(TRUE);

	///// Signal Tower Setting Button /////
	if (m_SystemConfig.bSignalTower){
		m_SignalTowerSettingButton.ShowWindow(SW_SHOWDEFAULT);
		m_SignalTowerSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_SignalTowerSettingButton.DrawFlatFocus(TRUE);
	}
	else{
		m_SignalTowerSettingButton.ShowWindow(SW_HIDE);
	}

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType == MODEL_T3100){
		m_JogDirectionSettingButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.08 Bagus Add (TohoSpec対応) <--
// 2013.01.11 bagus jog direction -->
		///// Jog Direction Setting Button /////
		m_JogDirectionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
			BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_JogDirectionSettingButton.DrawFlatFocus(TRUE);
// 2013.01.11 bagus jog direction -->
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--
}

HBRUSH CSystemOperationCommonSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSystemOperationCommonSettingDlg::OnTimer(UINT nIDEvent)
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

// 2013.01.11 bagus jog direction -->
void CSystemOperationCommonSettingDlg::OnJogDirectionSettingButton() 
{
	CJogDirSettingDlg dlg;

	ShowSettingDlg(&dlg);
}
// 2013.01.11 bagus jog direction <--
