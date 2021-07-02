// ViewWindowView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "Misc.h"
#include "ViewScreenDlg.h"
#include "ViewWindowView.h"
#include "OverlayDialog.h"
#include "ManualStageDlg.h"
#include "NEXIOBASE.HXX"
#include "..\\..\\INC\\NSStage.hxx"
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
#include "ChiefView.h"
#include "ChifTransiMaster.h"
extern CChiefView*	g_pcChiefView;	/* static 用クラスポインタ	*/
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// #########################################################################
// CViewWindowView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CViewWindowView, CNanoUI)

// =========================================================================
//
CViewWindowView::CViewWindowView()
	: CNanoUI(CViewWindowView::IDD)
{
	//{{AFX_DATA_INIT(CViewWindowView)
	//}}AFX_DATA_INIT

	FontCreation();
}

// =========================================================================
//
CViewWindowView::~CViewWindowView()
{
	m_Font.DeleteObject();

	if ( m_pdlgViewScreen != NULL ) {
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pdlgViewScreen->DestroyWindow();
		delete m_pdlgViewScreen;
		m_pdlgViewScreen = NULL;
	}
}

// =========================================================================
//
void CViewWindowView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewWindowView)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_AUTO_FOCUS_BUTTON, m_AutoFocusButton);
	//Saiki 20090908 Add ----->
	DDX_Control(pDX, IDC_H_POSITION_BUTTON, m_HomePositionButton);
	//Saiki 20090908 Add <-----
	DDX_Control(pDX, IDC_LENS_MAGNIFICATION_COMBO, m_cbLens);
	DDX_Control(pDX, IDC_MEASUREMENT_TYPE_COMBO, m_cbCamera);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CViewWindowView, CNanoUI)
	//{{AFX_MSG_MAP(CViewWindowView)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AUTO_FOCUS_BUTTON, OnAutoFocusButton)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_H_POSITION_BUTTON, OnHomePositionButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CViewWindowView 診断

#ifdef _DEBUG
// =========================================================================
//
void CViewWindowView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CViewWindowView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CViewWindowView メッセージ ハンドラ

// =========================================================================
//
void CViewWindowView::OnInitialUpdate()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	CNanoUI::OnInitialUpdate();

// 2014.01.17 Bagus Add (Stage None対応) -->
	//システムコンフィグ設定読み込み
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage None対応) <--

	OVLYMULTIWND ovlymw = { m_hWnd };
	GetDlgItem(IDC_CAMERA)->GetClientRect(&ovlymw.rc);
	//	クライアントエッジ境界線使用時のロジック
	OffsetRect(&ovlymw.rc, GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE));

// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if(g_lOVideoEnable == 1){
			OverlaySetMultipleWindow(&ovlymw);
		}
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		OverlaySetMultipleWindow(&ovlymw);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	m_rcCamera = ovlymw.rc;

// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_cbLens.ShowWindow(SW_HIDE);
		m_cbCamera.ShowWindow(SW_HIDE);
		m_AutoFocusButton.ShowWindow(SW_HIDE);
		m_HomePositionButton.ShowWindow(SW_HIDE);
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		///// Auto Focus Button /////
		m_AutoFocusButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_AutoFocusButton.DrawFlatFocus(TRUE);
		m_AutoFocusButton.SetFont(&m_Font);

		//Saiki 20090908 Add ----->
		///// Home Position Button /////
		m_HomePositionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_HomePositionButton.DrawFlatFocus(TRUE);
		m_HomePositionButton.SetFont(&m_Font);
		//Saiki 20090908 Add <-----
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	m_pdlgViewScreen = new CViewScreenDlg(this);
	m_pdlgViewScreen->Create(CViewScreenDlg::IDD);

	///// Select Lens /////
	int nSelect = 0;
	int nItemData = m_cbLens.GetItemData(nSelect);
//Okabe20090223Add--->
	m_cbLens.SetCurSel(nSelect);
//Okabe20090223Add<---

	// 2009.07.21 K.Matsuo NSSTAGE -->
//	StageSelectLens(nItemData);
	// 2009.07.21 K.Matsuo NSSTAGE <--

	SetTimer(1, 50, NULL);

}

// =========================================================================
//
void CViewWindowView::OnPaint()
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
}

// =========================================================================
//
void CViewWindowView::OnAutoFocusButton()
{
	CString l_strBuffer, l_strTitle, strMsg;

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;


	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
//	  CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
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
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
/* modified 2009.09.08 hmenjo AF 自動レンズ切替置換 ---------- { ---------- */
//	  if( StageDoAutoFocus()==FALSE ){
/* modified 2009.09.08 hmenjo AF 自動レンズ切替置換 ----------				*/
	if (FALSE == NS_StageDoAutoFocus()) {
/* modified 2009.09.08 hmenjo AF 自動レンズ切替置換 ---------- } ---------- */
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
		if( m_pDoc->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
			// MessageBoxを表示する
			LoadStringML(IDS_AUTO_FOCUS_FAIL, l_strBuffer, "Auto Focus Fail !!!.");
			LoadStringML(IDS_TITLE_AUTO_FOCUS, l_strTitle, "AUTO FOCUS");
			MessageBox(l_strBuffer, l_strTitle, MB_OK|MB_ICONWARNING);
		}
		else{
			// AutoFocus画面を表示する
			CManualStageDlg dlg;
			LoadStringML(IDS_AUTOFOCUS_CAPTION, l_strBuffer, "AUTO FOCUS");
			dlg.SetCaption(l_strBuffer);
			dlg.SetMessage(l_strBuffer);
			dlg.DoModal();
		}
	} else {
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
	}
	CCursor::EndWaitCursor();
}

// =========================================================================
//
void CViewWindowView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
// 2013.11.07 Bagus Add (TohoSpec対応) -->
// 2014.01.17 Bagus Mod (Stage None対応) -->
//	if(g_lModelType == MODEL_T3100){
	if(m_SystemConfig.nStageType == STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		if(g_lOVideoEnable == 1){
			if ( m_rcCamera.PtInRect(point) ) {
				m_pdlgViewScreen->ShowWindow(SW_HIDE);
				m_pdlgViewScreen->ShowWindow(SW_SHOWNORMAL);
			}
		}
	}
	else{
// 2013.11.07 Bagus Add (TohoSpec対応) <--
		if ( m_rcCamera.PtInRect(point) ) {
			m_pdlgViewScreen->ShowWindow(SW_HIDE);
			m_pdlgViewScreen->ShowWindow(SW_SHOWNORMAL);
		}
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	CNanoUI::OnLButtonDblClk(nFlags, point);
}

// =========================================================================
//
HBRUSH CViewWindowView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoUI::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	if(m_pDoc->GetProcessStatus() == PROCESS_INIT) return hbr;

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

// =========================================================================
//
void CViewWindowView::OnTimer(UINT nIDEvent)
{
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//// 2009.12.04 K.Matsuo Change -->
//	BOOL bEnable = ( m_pDoc->GetProcessStatus() == PROCESS_WAIT );
//	m_cbLens.EnableWindow(bEnable);
//	m_cbCamera.EnableWindow(bEnable);
//	m_AutoFocusButton.EnableWindow(bEnable);
//	m_HomePositionButton.EnableWindow(bEnable);
//// 2009.12.04 K.Matsuo Change <--
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
	BOOL bEnable = (m_pDoc->GetProcessStatus() == PROCESS_WAIT) && (m_pDoc->GetHostMode() == HOST_LOCAL);
	BOOL l_bNoActuate =	TRUE;
	DWORD l_dwAllAct = m_pDoc->ActuateFlagsGetAll();
	if (
		0 != (l_dwAllAct & (
							ACTUATE_XYSTAGE
						  | ACTUATE_ZAXIS
						  | ACTUATE_PIN
						  | ACTUATE_WORKGUIDE
							))
		) {
		l_bNoActuate = FALSE;
	}
	BOOL l_bNoProgressTrMas = TRUE;
	if (0 != g_pcChiefView) {
		if (0 != g_pcChiefView->m_pcChiefTransiMaster) {
			if (false == ((CChiefTransiMaster*) g_pcChiefView->m_pcChiefTransiMaster)->IsIdle()) {
				l_bNoProgressTrMas = FALSE;
			}
		}
	}
	BOOL l_bCtrlEna;
	if (
		(TRUE == bEnable)
	 && (TRUE == l_bNoActuate)
	 && (TRUE == l_bNoProgressTrMas)
		) {
		l_bCtrlEna = TRUE;
	} else {
		l_bCtrlEna = FALSE;
	}
#if 0
	/* コッチではどこにあるか判らない同様な処理のコンフリクトで異常動作になるため採用しません．	*/
	static BOOL ls_bCtrlEnaPre = l_bCtrlEna;	/* 前回値	*/
	if (ls_bCtrlEnaPre != l_bCtrlEna) {
		ls_bCtrlEnaPre = l_bCtrlEna;
		/* 変化有り	*/
		m_cbLens.EnableWindow(l_bCtrlEna);
		m_cbCamera.EnableWindow(l_bCtrlEna);
		m_AutoFocusButton.EnableWindow(l_bCtrlEna);
		m_HomePositionButton.EnableWindow(l_bCtrlEna);
	}
#else
// 2014.01.17 Bagus Mod (Stage None対応) -->
// 2013.11.07 Bagus Add (TohoSpec対応) -->
//	if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None対応) <--
		m_cbLens.EnableWindow(l_bCtrlEna);
		m_cbCamera.EnableWindow(l_bCtrlEna);
		m_AutoFocusButton.EnableWindow(l_bCtrlEna);
		m_HomePositionButton.EnableWindow(l_bCtrlEna);
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
#endif
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

	if ( m_pDoc->GetProcessStatus() == PROCESS_INIT )
		return;

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

	CNanoUI::OnTimer(nIDEvent);
}

// =========================================================================
//
void CViewWindowView::OnHomePositionButton()
{
// 2010.02.03 K.Matsuo PIN UPPER インターロックチェック漏れ対応 -->
	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	CString strMsg;

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();

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
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //ステージが動作中です
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ▲インターロック条件▲
// 2010.02.03 K.Matsuo PIN UPPER インターロックチェック漏れ対応 <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z 軸の初期化停止位置
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z 軸の初期化停止位置ファクタ

	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != pMainFrame->HardwareSimulation()) {
		// ハードシミュレートモードだったので，測定完了待ち へ
	}
	else {
		// 実機の場合 ---------------------------------------------------------
		// AF 要/不要判定
		// 不要
			// (全点 AF 無しの場合)Z 軸移動が必要かチェック
		// 必要(仕様上必要なら)
		// Ｚ軸移動
		//TCHAR l_szMainRcpName[256];
		//((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// 初期化停止位置[pulse]を[0.1um]に変換します
		LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
		l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// 初期化停止位置[pulse]÷ファクタ[pulse/mm]
		l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]に変換する
		long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

		NS_ConvertToStageMoveCoord(&l_lZ);		// 補正

		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
#if 0	//----- Kawashima 2008.12.01 ----->
//				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
		pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// 移動開始失敗
		} else {
			// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);	 // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
	}
}
