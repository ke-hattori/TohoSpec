// SystemOperationSeSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
// 2009.09.17 bagus SE --{--
//#include "SrCofigurationSettingDlg.h"
#include "SeCofigurationSettingDlg.h"
// 2009.09.17 bagus SE --}--
#include "SeXmpSettingDlg.h"
#include "SystemOperationSeSettingDlg.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationSeSettingDlg ダイアログ
CSystemOperationSeSettingDlg::CSystemOperationSeSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemOperationSeSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemOperationSeSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

void CSystemOperationSeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemOperationSeSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SE_CONFIGURATION_SETTING_BUTTON, m_SeConfigurationSettingButton);
	DDX_Control(pDX, IDC_SE_XMP_SETTING_BUTTON, m_SeXmpSettingButton);
}

BEGIN_MESSAGE_MAP(CSystemOperationSeSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemOperationSeSettingDlg)
	ON_BN_CLICKED(IDC_SE_CONFIGURATION_SETTING_BUTTON, OnSeConfigurationSettingButton)
	ON_BN_CLICKED(IDC_SE_XMP_SETTING_BUTTON, OnSeXmpSettingButton)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationSeSettingDlg メッセージ ハンドラ

BOOL CSystemOperationSeSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// SR Configuration Setting Button /////
	m_SeConfigurationSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SeConfigurationSettingButton.DrawFlatFocus(TRUE);

	///// XMP Setting Button /////
	m_SeXmpSettingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SeXmpSettingButton.DrawFlatFocus(TRUE);

	SetTimer(1, 50, NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSystemOperationSeSettingDlg::OnSeConfigurationSettingButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CSeCofigurationSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

void CSystemOperationSeSettingDlg::OnSeXmpSettingButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CSeXmpSettingDlg dlg;
	ShowSettingDlg(&dlg);
}

BOOL CSystemOperationSeSettingDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSystemOperationSeSettingDlg::ShowSettingDlg(CDialog* pDlg)
{
	CString strBuffer, strTitle;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	if ( !pDoc->User_Access(ACCESS_SYSTEMCONFIG) ) {


		LoadStringML(IDS_USER_ACCESS_STOP_TEXT, strBuffer, "You don't have Access Privilege");
		LoadStringML(IDS_USER_ACCESS_STOP_CAPTION, strTitle, "ACCESS PRIVILEGE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	return ( pDlg->DoModal() == IDOK );
}

HBRUSH CSystemOperationSeSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSystemOperationSeSettingDlg::OnTimer(UINT nIDEvent)
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
