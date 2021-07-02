// VacuumChangeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "VacuumChangeDlg.h"
#include "..\\..\\INC\\NEXIF.HXX"
#include "..\\..\\INC\\NEXIOBASE.HXX"
#include "NanoSpecDoc.h"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVacuumChangeDlg ダイアログ                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CVacuumChangeDlg::CVacuumChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVacuumChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVacuumChangeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CVacuumChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVacuumChangeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_VACUUM_ON_BUTTON, m_VacuumOnButton);
	DDX_Control(pDX, IDC_VACUUM_OFF_BUTTON, m_VacuumOffButton);
}


/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CVacuumChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CVacuumChangeDlg)
	ON_BN_CLICKED(IDC_VACUUM_ON_BUTTON, OnVacuumOnButton)
	ON_BN_CLICKED(IDC_VACUUM_OFF_BUTTON, OnVacuumOffButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVacuumChangeDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CVacuumChangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// Vacuum ON Button /////
	m_VacuumOnButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_VacuumOnButton.DrawFlatFocus(TRUE);

	///// Vacuum OFF Button /////
	m_VacuumOffButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_VacuumOffButton.DrawFlatFocus(TRUE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CVacuumChangeDlg::OnVacuumOnButton()
{

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	//Saiki 20090601 Add ----->
	CString strMsg;
	//Saiki 20090601 Add <-----

	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(pDoc->IsInterLock() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	//Saiki 20090601 Change ----->
	if( nexioIsPinDownPos() 		!= ON ) {
		LoadStringML(IDS_LIFTER_NOT_LOWER, strMsg, "Sample Lifter is not Lowest Position.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // ピン下端外れ
	if( nexioIsWorkGuideClose() 	!= OFF) {
		LoadStringML(IDS_CLAMP_NOT_CLOSED, strMsg, "Work Guide is not Close Position.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // ワークガイドClose位置
	if( nexioIsWorkGuideOpen()		!= ON ) {
		LoadStringML(IDS_CLAMP_NOT_OPEND, strMsg, "Work Guide is not Open Position.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // ワークガイドOpen位置
	//Saiki 20090601 Change <-----

	//ActuateFlagチェック
	//Saiki 20090601 Change ----->
	if(pDoc->ActuateFlagsGet(ACTUATE_PIN)){
		LoadStringML(IDS_LIFTER_WORKING, strMsg, "Sample Lifter is moving.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	} //リフターが動作中です
	if(pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE)){
		LoadStringML(IDS_CLAMP_WORKING, strMsg, "Work Guide is moving.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ワークガイドが動作中です
	//Saiki 20090601 Change <-----
	// ▲インターロック条件▲


	nexifVacuumOn(NULL);
	EndDialog(1);
}

/////////////////////////////////////////////////////////////////////////////
//
void CVacuumChangeDlg::OnVacuumOffButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	//Saiki 20090601 Add ----->
	CString strMsg;
	//Saiki 20090601 Add <-----

	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
		  この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(pDoc->IsInterLock() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	//Saiki 20090601 Change ----->
	if( nexioIsWorkGuideClose() 	!= OFF) {
		LoadStringML(IDS_CLAMP_NOT_CLOSED, strMsg, "Work Guide is not Close Position.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // ワークガイドClose位置
	if( nexioIsWorkGuideOpen()		!= ON ) {
		LoadStringML(IDS_CLAMP_NOT_OPEND, strMsg, "Work Guide is not Open Position.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	}  // ワークガイドOpen位置

	//Saiki 20090601 Change <-----

	//ActuateFlagチェック
	//Saiki 20090601 Change ----->
	if(pDoc->ActuateFlagsGet(ACTUATE_PIN)){
		LoadStringML(IDS_LIFTER_WORKING, strMsg, "Sample Lifter is moving.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	} //リフターが動作中です
	if(pDoc->ActuateFlagsGet(ACTUATE_WORKGUIDE)){
		LoadStringML(IDS_CLAMP_WORKING, strMsg, "Work Guide is moving.");
		pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ワークガイドが動作中です
	//Saiki 20090601 Change <-----
	// ▲インターロック条件▲


	nexifVacuumOff(NULL);
	EndDialog(1);
}
