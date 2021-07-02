// SystemOperationStressSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SystemOperationStressSettingDlg.h"
#include "StressCofigurationSettingDlg.h"
#include "StressSampleElasticModulusSettingDlg.h"
// 2009.11.09 bagus Stress 追加変更 --{--
#include "StressElasticModulusListDlg.h"
// 2009.11.09 bagus Stress 追加変更 --}--
#include "StressSelectSampleTypeDlg.h"
#include "StressLineSectionSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationStressSettingDlg ダイアログ


CSystemOperationStressSettingDlg::CSystemOperationStressSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemOperationStressSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemOperationStressSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSystemOperationStressSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemOperationStressSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STRESS_CONFIGURATION_SETTING_BUTTON, m_StressConfigButton);
// 2009.11.09 bagus Stress 追加変更 --{--
//	DDX_Control(pDX, IDC_SAMPLE_ELASTICMODULUS_SETTING_BUTTON, m_SampleElasticModulusSettingButton);
// 2009.11.09 bagus Stress 追加変更 --}--
	DDX_Control(pDX, IDC_LINE_SECTION_SETTING_BUTTON, m_LineSectionSettingButton);
}


BEGIN_MESSAGE_MAP(CSystemOperationStressSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemOperationStressSettingDlg)
	ON_BN_CLICKED(IDC_STRESS_CONFIGURATION_SETTING_BUTTON, OnStressConfigurationSettingButton)
	ON_BN_CLICKED(IDC_LINE_SECTION_SETTING_BUTTON, OnLineSectionSettingButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationStressSettingDlg メッセージ ハンドラ

void CSystemOperationStressSettingDlg::OnStressConfigurationSettingButton()
{
	CStressCofigurationSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

//DEL void CSystemOperationStressSettingDlg::OnSampleElasticmodulusSettingButton()
//DEL {
//DEL 	CStressElasticModulusListDlg dlg;
//DEL 	ShowSettingDlg(&dlg);
//DEL }

void CSystemOperationStressSettingDlg::OnLineSectionSettingButton()
{
	CStressSelectSampleTypeDlg dlg;
	if(!ShowSettingDlg(&dlg)){
		return;
	}
	CStressLineSectionSettingDlg dlg2;

	strcpy(dlg2.m_szName, dlg.m_szName);

	ShowSettingDlg(&dlg2);

}

BOOL CSystemOperationStressSettingDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSystemOperationStressSettingDlg::ShowSettingDlg(CDialog *pDlg)
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

BOOL CSystemOperationStressSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// Stress Config Button /////
	m_StressConfigButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_StressConfigButton.DrawFlatFocus(TRUE);

// 2009.11.09 bagus Stress 追加変更 --{--
	///// Sample Elastic Modulus Setting Button /////
//	m_SampleElasticModulusSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
//	m_SampleElasticModulusSettingButton.DrawFlatFocus(TRUE);
// 2009.11.09 bagus Stress 追加変更 --}--

	///// Line Section Setting Button /////
	m_LineSectionSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_LineSectionSettingButton.DrawFlatFocus(TRUE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}
