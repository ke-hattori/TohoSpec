// PointSetupTeachingMSDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingMSDlg.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg ダイアログ
/////////////////////////////////////////////////////////////////////////////
//
CPointSetupTeachingMSDlg::CPointSetupTeachingMSDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPointSetupTeachingMSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupTeachingMSDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_iTeachingMode = MODE_AUTO_MEAS;
	m_bMoveNext = FALSE;
// 2009.11.09 bagus MS 修正 --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS 修正 --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupTeachingMSDlg)
	DDX_Control(pDX, IDC_NEXT_POINT_BUTTON, m_NextPointButton);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CPointSetupTeachingMSDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPointSetupTeachingMSDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_HOME_POSITION_BUTTON, OnHomePositionButton)
	ON_CBN_SELENDOK(IDC_MICRO_SCOPE_FOCUS_POSITION, OnSelendokMicroScopeFocusPosition)
	ON_BN_CLICKED(IDC_NEXT_POINT_BUTTON, OnNextPointButton)
	//}}AFX_MSG_MAP
// 2009.11.09 bagus MS 修正 --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS 修正 --}--
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CPointSetupTeachingMSDlg::OnInitDialog()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);

	//Sampleセット
	SetFileName(m_StageProgInfoHdr.SampleInfo.szName);
	SetSampleMode(1);

	COverlayDialog::OnInitDialog();

	///// Next Point Button /////
	m_NextPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NextPointButton.DrawFlatFocus(TRUE);

	m_Z_UpButton.SetEnabled(FALSE);
	m_Z_DownButton.SetEnabled(FALSE);
	m_Z_HomePosButton.SetEnabled(FALSE);
	m_cbMotionElev.EnableWindow(FALSE);
//	m_btnAutoFocus.SetEnabled(FALSE);
	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE);
//	m_btnCapture.SetEnabled(FALSE);
	GetDlgItem(IDC_CAPTURE_BUTTON)->EnableWindow(FALSE);
	m_cbLens.EnableWindow(FALSE);
	//2009.11.12 bagus MS --{--
	m_cbHeadType.EnableWindow(FALSE);
	m_cbCamera.EnableWindow(FALSE);
	//2009.11.12 bagus MS --}--

	///// Lens Combo /////
	InitCombo_Lens();
	OnSelendokMicroScopeFocusPosition();

	if (m_iTeachingMode == MODE_AUTO_MEAS) {
		GetDlgItem(IDC_NEXT_POINT_BUTTON)->ShowWindow(SW_SHOW);
	} else {
		GetDlgItem(IDC_NEXT_POINT_BUTTON)->ShowWindow(SW_HIDE);
	}

	SetTimer(1, 50, NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::OnOK()
{
// 2009.11.09 bagus MS 修正 --{--
	if ( CCursor::m_bWaitCursor ) return;
// 2009.11.09 bagus MS 修正 --}--

	COverlayDialog::OnOK();

	// TODO: この位置にその他の検証用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::OnCancel()
{
// 2009.11.09 bagus MS 修正 --{--
	if ( CCursor::m_bWaitCursor ) return;
// 2009.11.09 bagus MS 修正 --}--

	COverlayDialog::OnCancel();

	// TODO: この位置に特別な後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::OnTimer(UINT nIDEvent)
{
//河村さんへ
//ボタンの有効/無効の切替えをActuateFlagsを条件にして作成をお願いします。
#if 0 //ProcessStatusを使っているので間違い
	int iProcessStatus = m_pDoc->GetProcessStatus();

	//ソフトジョイスティック制限
	if(iProcessStatus != m_iProcessStatusOld)
	{
		if( iProcessStatus != PROCESS_WAIT )
		{
			SoftJoyStickXyzEnable(FALSE); //操作禁止
		}
		else
		{
			SoftJoyStickXyzEnable(TRUE); //操作許可
		}
	}
	m_iProcessStatusOld = iProcessStatus;
#endif

	COverlayDialog::OnTimer(nIDEvent);
}

// ==========================================================================
// Name：		OnNextPointButton
void CPointSetupTeachingMSDlg::OnNextPointButton()
{
// 2009.11.09 bagus MS 修正 --{--
	if ( CCursor::m_bWaitCursor ) return;
// 2009.11.09 bagus MS 修正 --}--

	m_bMoveNext = TRUE;

	COverlayDialog::OnOK();
}

// =========================================================================
//
void CPointSetupTeachingMSDlg::SoftJoyStickXyzEnable(BOOL bEnable)
{
	//SoftJoyStick
	m_XY_UpButton.SetEnabled(bEnable);
	m_XY_DownButton.SetEnabled(bEnable);
	m_XY_RightButton.SetEnabled(bEnable);
	m_XY_LeftButton.SetEnabled(bEnable);
//	m_Z_UpButton.SetEnabled(FALSE);
//	m_Z_DownButton.SetEnabled(FALSE);
//	m_Z_HomePosButton.SetEnabled(FALSE);
}

void CPointSetupTeachingMSDlg::OnHomePositionButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
// 2010.02.03 K.Matsuo PIN UPPER インターロックチェック漏れ対応 -->
	CString strMsg;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // メンテナンス異常
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVOアラーム 				＝Ｌ
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// シャッター以外が動いていたら動作禁止
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// ステージ動作中
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ▲インターロック条件▲
// 2010.02.03 K.Matsuo PIN UPPER インターロックチェック漏れ対応 <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z 軸の初期化停止位置
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z 軸の初期化停止位置ファクタ

	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != m_pMainFrame->HardwareSimulation()) {
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
		m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// 移動開始失敗
		} else {
			// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);   // Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			m_pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
	}
}

// =========================================================================
//レンズ コンボイニシャル処理
//
void CPointSetupTeachingMSDlg::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	pCombo->ResetContent();

	int nIndex;
//2010.01.19 bagus MS --{--
	m_nLensSensorItem = -1;
//2010.01.19 bagus MS --}--
	for ( int i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++ ) {
		if (m_MsConfig.RoboCylinder[i].bEnable ) {
			nIndex = pCombo->AddString(m_MsConfig.RoboCylinder[i].szName);
			pCombo->SetItemData(nIndex, i);
// matsuhisa 2009.12.26 added >>>
			if (m_MsConfig.RoboCylinder[i].bSensor) {
				m_nLensSensorItem = i;
			}
// matsuhisa 2009.12.26 added <<<
		}
	}

	//2009.12.25 bagus MS --{--
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(0 <= rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex
	&& rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex < pCombo->GetCount()){
		pCombo->SetCurSel(rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex);
		m_nSelLens = rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex;
	}else{
		pCombo->SetCurSel(0);
		m_nSelLens = 0;
	}
	//2009.12.25 bagus MS --}--
}

// =========================================================================
// レンズ切り替え 移動
//
void CPointSetupTeachingMSDlg::OnSelendokMicroScopeFocusPosition()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	int	nIndex = pCombo->GetCurSel();
	int	nItemData = pCombo->GetItemData(nIndex);

	int	nPosNo = m_MsConfig.RoboCylinder[nItemData].nPosition;

	CString l_strBuffer, l_strTitle, strMsg;
// matsuhisa 2009.12.26 added >>>
	if ( !nexioIsMS_LensKind() ) {
//2010.01.19 bagus MS --{--
//		  if ( nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
		if (m_nLensSensorItem >= 0 &&  nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
//2010.01.19 bagus MS --}--
			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
			m_pDoc->MessageStringIf_Set(strMsg);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
	}
// matsuhisa 2009.12.26 added <<<

// 2009.11.09 bagus MS 修正 --{--
	if ( CCursor::m_bWaitCursor ) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
// 2009.11.09 bagus MS 修正 --{--
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ▼インターロック条件▼
	//メンテナンスモードチェック(2軸同時動作制限)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// 東朋メンテナンスモード
	{
// 2009.11.09 bagus MS 修正 --{--
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //メンテナンスモード組み合わせエラー
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
// 2009.11.09 bagus MS 修正 --}--
		/*東朋メンテモードの時は、必ずメンテナンスモード(L)となる
			この場合、2軸同時動作可能 */
	}
	else //東朋メンテナンスモード以外
	{
	}

	// 各I/Oチェック
// 2009.11.09 bagus MS 修正 --{--
//	if(m_pDoc->IsInterLock() == TRUE){return;}
//	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
// matsuhisa 2009.12.26 modified >>>
//	if (m_pDoc->IsInterLock() == TRUE) {
	if (m_pDoc->IsInterLockForMScope() == TRUE) {
// matsuhisa 2009.12.26 modified <<<
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
	if (m_pDoc->CheckUnitStatus() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--

	//ActuateFlagチェック
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
// 2009.11.09 bagus MS 修正 --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS 修正 --}--
	} //ステージが動作中です
// 2009.11.09 bagus MS 修正 --{--
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS 修正 --}--
	// ▲インターロック条件▲

// 2009.11.09 bagus MS 修正 --{--
#if 0
	// Z軸動作許可ランプ OFF
	nexioMS_ModeLamp(FALSE);

	// 位置決め
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

	// Z軸動作許可ランプ ON
	nexioMS_ModeLamp(TRUE);
#else
	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
// matsuhisa 2009.12.26 deleted >>>
//		if (m_MsConfig.RoboCylinder[nItemData].bSensor == nexioIsMS_LensKind()) {
// matsuhisa 2009.12.26 deleted <<<
			// 他の移動入力の受付を禁止する
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, l_strTitle, "Warning");
				MessageBox(strMsg, l_strTitle, MB_OK);
				// 必ずJoyStickの状態を戻すこと
				m_pDoc->JoyStickStatusRestore();
				pCombo->SetCurSel(m_nSelLens);
				return;
			}

			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);

			// Mode切り替え
			unsigned char oldMode = nexioRC_GetCurrentMode();
			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
				MSG 	msg;

				//応答を処理するまで待つ
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			// 位置決め
			if(nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo)){
				// ActuateFlagをセットする
				m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
				MSG 	msg;

				//応答を処理するまで待つ
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, oldMode);
				MSG 	msg;

				//応答を処理するまで待つ
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,FALSE);

// matsuhisa 2009.12.26 deleted >>>
//		} else {
//			// 異常
//			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
//			m_pDoc->MessageStringIf_Set(strMsg);
//			pCombo->SetCurSel(m_nSelLens);
//		}
// matsuhisa 2009.12.26 deleted <<<
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS 修正 --}--
}

// 2009.11.09 bagus MS 修正 --{--
void CPointSetupTeachingMSDlg::OnNexResponse(WPARAM wparam, LPARAM lparam)
{
	switch ( wparam ) {
	case nexRC_MoveToTeachPos:
	case nexRC_JogPlus:
	case nexRC_JogMinus:
	case nexRC_JogStop:
	case nexRC_SelectMode:
		m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
		break;
	default:
		break;
	}
}
// 2009.11.09 bagus MS 修正 --}--
// 2009.11.26 bagus 全般 修正 --{--
void CPointSetupTeachingMSDlg::ButtonEnableCheck()
{
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//	COverlayDialog::ButtonEnableCheck();
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
	/* XY軸ボタン	*/
	BOOL bXyButtonEnable = ( m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	m_XY_UpButton.SetEnabled(bXyButtonEnable);
	m_XY_DownButton.SetEnabled(bXyButtonEnable);
	m_XY_RightButton.SetEnabled(bXyButtonEnable);
	m_XY_LeftButton.SetEnabled(bXyButtonEnable);

//	/* Z軸ボタン	*/
//	BOOL bZButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) );
//	m_Z_UpButton.SetEnabled(bZButtonEnable);
//	m_Z_DownButton.SetEnabled(bZButtonEnable);
//	  m_Z_HomePosButton.SetEnabled(bZButtonEnable);

//	/* AutoFocusボタン	*/
//	BOOL bAFButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) );
//	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(bAFButtonEnable);

	/* 座標入力ボタン	*/
	BOOL bCoordButtonEnable = ( m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ||
								m_pMainFrame->GetJoyStickMode() != JOY_SOFT_MODE );
	GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(bCoordButtonEnable);
/* modified 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

	//ステージ移動中は移動させない
	BOOL bNextPrevButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	GetDlgItem(IDC_NEXT_POINT_BUTTON)->EnableWindow(bNextPrevButtonEnable);

}
// 2009.11.26 bagus 全般 修正 --}--
