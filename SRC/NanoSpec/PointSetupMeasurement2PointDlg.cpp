// PointSetupMeasurement2PointDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <math.h>
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "stdafx.h"
#include "nanospec.h"
#include "PointSetupMeasurement2PointDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#undef SubclassWindow	// バグ: C4003: SubclassWindow マクロのパラメーターの不足 http://support.microsoft.com/kb/150076/ja

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CROSS_MARK_LENGTH		(5)


/////////////////////////////////////////////////////////////////////////////
// CPointSetupMeasurement2PointDlg ダイアログ

CPointSetupMeasurement2PointDlg::CPointSetupMeasurement2PointDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPointSetupMeasurement2PointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupMeasurement2PointDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT


	m_iTeachingMode = MODE_AUTO_MEAS;

	m_iCurrPoint = 0;

	m_b2nd = FALSE;
	m_bComp = FALSE;

	m_bDrawCross = FALSE;

	m_bDraw2nd = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupMeasurement2PointDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupMeasurement2PointDlg)
	DDX_Control(pDX, IDC_MEAS_1ST_BUTTON, m_Meas1stButton);
	DDX_Control(pDX, IDC_MEAS_2ND_BUTTON, m_Meas2ndButton);
	DDX_Control(pDX, IDC_MEAS_COMP_BUTTON, m_MeasCompButton);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CPointSetupMeasurement2PointDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPointSetupMeasurement2PointDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_HOME_POSITION_BUTTON, OnHomePositionButton)
	ON_BN_CLICKED(IDC_MEAS_1ST_BUTTON, OnMeas1stButton)
	ON_BN_CLICKED(IDC_MEAS_2ND_BUTTON, OnMeas2ndButton)
	ON_BN_CLICKED(IDC_MEAS_COMP_BUTTON, OnMeasCompButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointSetupMeasurement2PointDlg メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CPointSetupMeasurement2PointDlg::OnInitDialog()
{
	//Sampleセット
	SetFileName(m_StageProgInfoHdr.SampleInfo.szName);
	SetSampleMode(1);

	COverlayDialog::OnInitDialog();

	///// Meas 1st Button /////
	m_Meas1stButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Meas1stButton.DrawFlatFocus(TRUE);

	///// Meas 2nd Button /////
	m_Meas2ndButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_Meas2ndButton.DrawFlatFocus(TRUE);

	///// Meas Comp Button /////
	m_MeasCompButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasCompButton.DrawFlatFocus(TRUE);

	m_l1stX = m_ScanPoint[(m_iCurrPoint - 1) * 2].lX;
	m_l1stY = m_ScanPoint[(m_iCurrPoint - 1) * 2].lY;
	m_l2ndX = m_ScanPoint[(m_iCurrPoint - 1) * 2 + 1].lX;
	m_l2ndY = m_ScanPoint[(m_iCurrPoint - 1) * 2 + 1].lY;

	switch ( m_iTeachingMode ) {
	case MODE_AUTO_MEAS:
		GetDlgItem(IDC_MEAS_1ST_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_2ND_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_COMP_BUTTON)->ShowWindow(SW_HIDE);
		//2009.11.29 bagus 2point-distance --{--
		//最初の位置にマークを置いておく
		m_iCrossX = (m_rcOverlay.right - m_rcOverlay.left) / 2;
		m_iCrossY = (m_rcOverlay.bottom - m_rcOverlay.left) / 2;
		if(m_b2nd)
			m_bDraw2nd = TRUE;
		m_bDrawCross = TRUE;
		//2009.11.29 bagus 2point-distance --}--
		break;
	case MODE_MANUAL_MEAS:
		GetDlgItem(IDC_MEAS_1ST_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEAS_2ND_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MEAS_COMP_BUTTON)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_MEAS_1ST_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_MEAS_2ND_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_MEAS_COMP_BUTTON)->EnableWindow(TRUE);
		//2009.11.29 bagus 2point-distance --{--
		//最初の位置にマークを置いておく
		m_iCrossX = (m_rcOverlay.right - m_rcOverlay.left) / 2;
		m_iCrossY = (m_rcOverlay.bottom - m_rcOverlay.left) / 2;
		if(m_b2nd)
			m_bDraw2nd = TRUE;
		m_bDrawCross = TRUE;
		//2009.11.29 bagus 2point-distance --}--
		break;
	default:
		break;
	}
	SetPointData();

	//2009.11.12 bagus 2point-distance --{--
	m_cbLens.EnableWindow(FALSE);
	m_cbHeadType.EnableWindow(FALSE);
	m_cbCamera.EnableWindow(FALSE);
	//2009.11.12 bagus 2point-distance --}--

	m_iProcessStatusOld = -1;

	if ((m_iTeachingMode == MODE_MANUAL_MEAS)){
		SetDlgItemText(IDC_POSITION_X1, "");
		SetDlgItemText(IDC_POSITION_Y1, "");
		SetDlgItemText(IDC_POSITION_X2, "");
		SetDlgItemText(IDC_POSITION_Y2, "");
// 2010.01.06 bagus Distance --{--
		SetDlgItemText(IDC_POSITION_X3, "");
		SetDlgItemText(IDC_POSITION_Y3, "");
// 2010.01.06 bagus Distance --}--
		SetDlgItemText(IDC_DISTANCE, "");
	}
	SetTimer(1, 50, NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupMeasurement2PointDlg::OnOK()
{
	if ((!m_bDrawCross) && (m_iTeachingMode == MODE_AUTO_MEAS))
		return;
	//2009.11.26 bagus 2point-Distance --{--
	if(m_b2nd && m_iTeachingMode == MODE_MANUAL_MEAS)
		return;
	//2009.11.26 bagus 2point-Distance --}--

	m_bComp = FALSE;

	COverlayDialog::OnOK();

	// TODO: この位置にその他の検証用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupMeasurement2PointDlg::OnCancel()
{
	COverlayDialog::OnCancel();

	// TODO: この位置に特別な後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CPointSetupMeasurement2PointDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COverlayDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// Point No. /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POINT_NO)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Point_No, REFERENCE_COLOR);
			return m_Brush_Point_No;
		}

		///// Position 1st /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_1ST)->GetSafeHwnd()){
			if (m_b2nd) {
				ControlColor(pDC, &m_Brush_Point1st, REFERENCE_COLOR);
			} else {
				ControlColor(pDC, &m_Brush_Point1st, YELLOW_COLOR);
			}
			return m_Brush_Point1st;
		}

		///// Position X1 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_X1)->GetSafeHwnd()){
			if (m_b2nd) {
				ControlColor(pDC, &m_Brush_Position_X1, REFERENCE_COLOR);
			} else {
				ControlColor(pDC, &m_Brush_Position_X1, YELLOW_COLOR);
			}
			return m_Brush_Position_X1;
		}

		///// Position Y1 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Y1)->GetSafeHwnd()){
			if (m_b2nd) {
				ControlColor(pDC, &m_Brush_Position_Y1, REFERENCE_COLOR);
			} else {
				ControlColor(pDC, &m_Brush_Position_Y1, YELLOW_COLOR);
			}
			return m_Brush_Position_Y1;
		}

		///// Position 2nd /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_2ND)->GetSafeHwnd()){
			if (m_b2nd) {
				ControlColor(pDC, &m_Brush_Point2nd, YELLOW_COLOR);
			} else {
				ControlColor(pDC, &m_Brush_Point2nd, REFERENCE_COLOR);
			}
			return m_Brush_Point2nd;
		}

		///// Position X2 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_X2)->GetSafeHwnd()){
			if (m_b2nd) {
				ControlColor(pDC, &m_Brush_Position_X2, YELLOW_COLOR);
			} else {
				ControlColor(pDC, &m_Brush_Position_X2, REFERENCE_COLOR);
			}
			return m_Brush_Position_X2;
		}

		///// Position Y2 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Y2)->GetSafeHwnd()){
			if (m_b2nd) {
				ControlColor(pDC, &m_Brush_Position_Y2, YELLOW_COLOR);
			} else {
				ControlColor(pDC, &m_Brush_Position_Y2, REFERENCE_COLOR);
			}
			return m_Brush_Position_Y2;
		}

// 2010.01.06 bagus Distance --{--
		///// Position X3 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_X3)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_X3, REFERENCE_COLOR);
			return m_Brush_Position_X3;
		}

		///// Position Y3 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Y3)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_Y3, REFERENCE_COLOR);
			return m_Brush_Position_Y3;
		}
// 2010.01.06 bagus Distance --}--

		///// Distance /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_DISTANCE)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Distance, REFERENCE_COLOR);
			return m_Brush_Distance;
		}

		break;
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupMeasurement2PointDlg::OnTimer(UINT nIDEvent)
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
	//2009.11.29 bagus 2point-Distance --{--
	//座標が移動した際にマーク表示座標を再計算させる
	if(m_bDrawCross){
		AdjustCrossPointPos();
	}
	//2009.11.29 bagus 2point-Distance --}--
}

// ==========================================================================
// Name：		SetPointData
void CPointSetupMeasurement2PointDlg::SetPointData()
{
	CString csPointNo, csX1, csY1, csX2, csY2, csDistance;
// 2010.01.06 bagus Distance --{--
	CString csX3, csY3;
// 2010.01.06 bagus Distance --}--
	double	dDistance;

	// X1
	csX1.Format("%.3lf", (double)(m_l1stX) / MICROMETRE);
	// Y1
	csY1.Format("%.3lf", (double)(m_l1stY) / MICROMETRE);

	//2009.11.12 bagus 2point-distance --{--
	//if ((m_iTeachingMode == MODE_MANUAL_MEAS) && (!m_bDraw2nd)) {
	if (!m_bDraw2nd) {
	//2009.11.12 bagus 2point-distance --}--
		csX2 = "";
		csY2 = "";
		csDistance = "";
	} else {
		// X2
		csX2.Format("%.3lf", (double)(m_l2ndX) / MICROMETRE);
		// Y2
		csY2.Format("%.3lf", (double)(m_l2ndY) / MICROMETRE);

// 2010.01.06 bagus Distance --{--
		// X3
		csX3.Format("%.3lf", (double)(abs(m_l2ndX - m_l1stX)) / MICROMETRE);
		// Y3
		csY3.Format("%.3lf", (double)(abs(m_l2ndY - m_l1stY)) / MICROMETRE);
// 2010.01.06 bagus Distance --}--

		// Distance
		dDistance = (double)m_pMainFrame->Calc2PointDistance(m_l1stX, m_l1stY, m_l2ndX, m_l2ndY) / MICROMETRE;
		csDistance.Format("%.3lf", dDistance);
	}

	// Point No.
	csPointNo.Format("%d", m_iCurrPoint);

	SetDlgItemText(IDC_POINT_NO, csPointNo);
	SetDlgItemText(IDC_POSITION_X1, csX1);
	SetDlgItemText(IDC_POSITION_Y1, csY1);
	SetDlgItemText(IDC_POSITION_X2, csX2);
	SetDlgItemText(IDC_POSITION_Y2, csY2);
// 2010.01.06 bagus Distance --{--
	SetDlgItemText(IDC_POSITION_X3, csX3);
	SetDlgItemText(IDC_POSITION_Y3, csY3);
// 2010.01.06 bagus Distance --}--
	SetDlgItemText(IDC_DISTANCE, csDistance);
}

// =========================================================================
//
void CPointSetupMeasurement2PointDlg::SoftJoyStickXyzEnable(BOOL bEnable)
{
	//SoftJoyStick
	m_XY_UpButton.SetEnabled(bEnable);
	m_XY_DownButton.SetEnabled(bEnable);
	m_XY_RightButton.SetEnabled(bEnable);
	m_XY_LeftButton.SetEnabled(bEnable);
	m_Z_UpButton.SetEnabled(bEnable);
	m_Z_DownButton.SetEnabled(bEnable);
	m_Z_HomePosButton.SetEnabled(bEnable);
}

void CPointSetupMeasurement2PointDlg::OnHomePositionButton()
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

void CPointSetupMeasurement2PointDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	STAGE_COORD TeachPos;

	CString strMsg, strTitle;


	if ( m_rcOverlay.PtInRect(point) && (GetKeyState(VK_SHIFT) < 0)) {
		if ( CCursor::m_bWaitCursor ) return;
		COverlayDialog::ClientToStage(&point, &TeachPos);
		NS_ConvertToStageGetPosCoord(&TeachPos);

		//2009.11.10 bagus 2point-distance --{--
		//物理座標から論理座標へ変換する
		//StageLogicalToPhysical(&TeachPos,&MarkPos);
		//2009.11.10 bagus 2point-distance --}--

		m_bDrawCross = TRUE;

		if (m_b2nd)
			m_bDraw2nd = TRUE;

		ClientToScreen(&point);
		GetDlgItem(IDC_OVERLAY)->ScreenToClient(&point);

		m_iCrossX = point.x;
		m_iCrossY = point.y;

		CalcPointData(TeachPos.lX, TeachPos.lY);

		SetPointData();

		Invalidate(TRUE);

		return;
	}

	COverlayDialog::OnLButtonDown(nFlags, point);
}

void CPointSetupMeasurement2PointDlg::OnPaint()
{
	COverlayDialog::OnPaint();
	return;
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	COverlayDialog::OnPaint();

	if (!m_bDrawCross)
		return;

	CWnd* VideoScreen = GetDlgItem(IDC_OVERLAY);
	CDC* pDC = VideoScreen->GetDC();
	CRect rect;
	VideoScreen->GetClientRect(&rect);

	CPen* pOldPen;
	CPen cLinePen(PS_SOLID, 1, RED_COLOR);

	pOldPen = pDC->SelectObject(&cLinePen);

	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH * 2, m_iCrossY);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH * 2, m_iCrossY);
	pDC->MoveTo(m_iCrossX, m_iCrossY - CROSS_MARK_LENGTH * 2);
	pDC->LineTo(m_iCrossX, m_iCrossY + CROSS_MARK_LENGTH * 2);

	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);
	pDC->MoveTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);

	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);

	pDC->SelectObject(&pOldPen);

	ReleaseDC(pDC);

	// 描画用メッセージとして COverlayDialog::OnPaint() を呼び出してはいけません
}

void CPointSetupMeasurement2PointDlg::CalcPointData(long lX, long lY)
{
//debug ステージ座標を計算する

	if (m_b2nd) {
		m_l2ndX = lX;
		m_l2ndY = lY;
	} else {
		m_l1stX = lX;
		m_l1stY = lY;
	}
}

void CPointSetupMeasurement2PointDlg::OnMeas1stButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (!m_bDrawCross)
		return;

	GetDlgItem(IDC_MEAS_1ST_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_MEAS_2ND_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_MEAS_COMP_BUTTON)->EnableWindow(TRUE);

	m_bDrawCross = FALSE;

	m_b2nd = TRUE;
	Invalidate(TRUE);
}

void CPointSetupMeasurement2PointDlg::OnMeas2ndButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if (!m_bDrawCross)
		return;

	GetDlgItem(IDC_MEAS_1ST_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_MEAS_2ND_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_MEAS_COMP_BUTTON)->EnableWindow(TRUE);

	m_bDrawCross = FALSE;

	m_iCurrPoint++;
	m_b2nd = FALSE;
	if(m_iTeachingMode == MODE_MANUAL_MEAS){
		// 測定ポイント番号と VALID データをデータ処理へ通知
		MEAS_1POINT_RESULT l_Meas1PointResult;
		STAGE_COORD tCoord;
		l_Meas1PointResult.dwPointNo = 0;
		l_Meas1PointResult.dwAFValid = 0;
		l_Meas1PointResult.lX = m_l1stX;
		l_Meas1PointResult.lY = m_l1stY;
		l_Meas1PointResult.lZ = 0;
		COPYDATASTRUCT l_CopyData;
		l_CopyData.dwData = WMCD_CHIF_1POINT_END;
		l_CopyData.cbData = sizeof(MEAS_1POINT_RESULT);
		l_CopyData.lpData = &l_Meas1PointResult;
		tCoord.lX = m_l1stX;
		tCoord.lY = m_l1stY;
		m_pDoc->SetLastDistancePoint(tCoord);
		m_pMainFrame->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);

		l_Meas1PointResult.dwPointNo = 0;
		l_Meas1PointResult.dwAFValid = 0;
		l_Meas1PointResult.lX = m_l2ndX;
		l_Meas1PointResult.lY = m_l2ndY;
		l_Meas1PointResult.lZ = 0;
		l_CopyData.dwData = WMCD_CHIF_1POINT_END;
		l_CopyData.cbData = sizeof(MEAS_1POINT_RESULT);
		l_CopyData.lpData = &l_Meas1PointResult;
		tCoord.lX = m_l2ndX;
		tCoord.lY = m_l2ndY;
		m_pDoc->SetLastDistancePoint(tCoord);
		m_pMainFrame->SendMessage(WM_COPYDATA, 0, (LPARAM) &l_CopyData);
	}
	//2009.11.17 bagus 2point-distance --{--
	//2nd のボタンの後はクリアする
	// Point No.
	CString csPointNo;
	csPointNo.Format("%d", m_iCurrPoint);

	SetDlgItemText(IDC_POINT_NO,csPointNo);
	SetDlgItemText(IDC_POSITION_X1, _T(""));
	SetDlgItemText(IDC_POSITION_Y1, _T(""));
	SetDlgItemText(IDC_POSITION_X2, _T(""));
	SetDlgItemText(IDC_POSITION_Y2, _T(""));
// 2010.01.06 bagus Distance --{--
	SetDlgItemText(IDC_POSITION_X3, _T(""));
	SetDlgItemText(IDC_POSITION_Y3, _T(""));
// 2010.01.06 bagus Distance --}--
	SetDlgItemText(IDC_DISTANCE, _T(""));

	Invalidate(TRUE);
}

void CPointSetupMeasurement2PointDlg::OnMeasCompButton()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_bComp = TRUE;

	COverlayDialog::OnOK();
}
void CPointSetupMeasurement2PointDlg::Refresh()
{
	COverlayDialog::Refresh();

	if (!m_bDrawCross)
		return;

	//2009.11.30 bagus 2point-distance --{--
	//マークを範囲外の位置に描画しないようにする。
	//2009.11.30 bagus 2point-distance --}--

	CWnd* VideoScreen = GetDlgItem(IDC_OVERLAY);
	CDC* pDC = VideoScreen->GetDC();
	CRect rect;
	CRgn clip;
	VideoScreen->GetClientRect(&rect);

	CPen* pOldPen;
	CPen cLinePen(PS_SOLID, 1, RED_COLOR);

	pOldPen = pDC->SelectObject(&cLinePen);
	clip.CreateRectRgnIndirect(rect);
	pDC->SelectClipRgn(&clip);

	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH * 2, m_iCrossY);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH * 2, m_iCrossY);
	pDC->MoveTo(m_iCrossX, m_iCrossY - CROSS_MARK_LENGTH * 2);
	pDC->LineTo(m_iCrossX, m_iCrossY + CROSS_MARK_LENGTH * 2);

	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);
	pDC->MoveTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);

	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY - CROSS_MARK_LENGTH);
	pDC->MoveTo(m_iCrossX - CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);
	pDC->LineTo(m_iCrossX + CROSS_MARK_LENGTH, m_iCrossY + CROSS_MARK_LENGTH);

	pDC->SelectObject(&pOldPen);
	pDC->SelectClipRgn(NULL);

	ReleaseDC(pDC);

}
//2009.11.29 bagus 2point-distance --{--
//マーク表示位置調整用関数
// 現在のマーク座標を元に再計算する。
void CPointSetupMeasurement2PointDlg::AdjustCrossPointPos()
{
	STAGE_COORD pos;
	POINT newCrossPoint;

	if(!m_b2nd){
		pos.lX = m_l1stX;
		pos.lY = m_l1stY;
	}else{
		pos.lX = m_l2ndX;
		pos.lY = m_l2ndY;
	}

	NS_ConvertToStageMoveCoord(&pos);
	COverlayDialog::StageToClient(&pos,&newCrossPoint);

	m_iCrossX = newCrossPoint.x;
	m_iCrossY = newCrossPoint.y;
}
//2009.11.29 bagus 2point-distance --}--
