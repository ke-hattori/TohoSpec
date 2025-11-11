// PointSetupTeaching2PointDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <math.h>
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "PointSetupTeaching2PointDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeaching2PointDlg ダイアログ

CPointSetupTeaching2PointDlg::CPointSetupTeaching2PointDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPointSetupTeaching2PointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupTeaching2PointDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT


// 2009.10.24 bagus 2点間 修正 --{--
//	m_bTeachingMode = FALSE;

	m_iPosX = 0;
	m_iPosY = 0;
// 2009.10.24 bagus 2点間 修正 --}--

// 2009.10.28 bagus 2点間 修正 --{--
	m_b2nd = FALSE;
// 2009.10.28 bagus 2点間 修正 --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeaching2PointDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupTeaching2PointDlg)
	DDX_Control(pDX, IDC_NEXT_POINT_BUTTON, m_NextPointButton);
	DDX_Control(pDX, IDC_PREVIOUS_POINT_BUTTON, m_PreviousPointButton);
	DDX_Control(pDX, IDC_APPLY_BUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MOVE_POINT, m_MoveButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CPointSetupTeaching2PointDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPointSetupTeaching2PointDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_NEXT_POINT_BUTTON, OnNextPointButton)
	ON_BN_CLICKED(IDC_PREVIOUS_POINT_BUTTON, OnPreviousPointButton)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, OnApplyButton)
	ON_BN_CLICKED(IDC_HOME_POSITION_BUTTON, OnHomePositionButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MOVE_POINT, OnMoveButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeaching2PointDlg メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CPointSetupTeaching2PointDlg::OnInitDialog()
{
	//Sampleセット
	SetFileName(m_StageProgInfoHdr.SampleInfo.szName);
	SetSampleMode(1);

	COverlayDialog::OnInitDialog();

	///// Next Point Button /////
	m_NextPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NextPointButton.DrawFlatFocus(TRUE);

	///// Previous Point Button /////
	m_PreviousPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PreviousPointButton.DrawFlatFocus(TRUE);

	///// Apply Button /////
	m_ApplyButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ApplyButton.DrawFlatFocus(TRUE);

	///// Move Button /////
	m_MoveButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MoveButton.DrawFlatFocus(TRUE);

	// 測定ポイントを表示
// 2009.10.24 bagus 2点間 修正 --{--
//	if (m_bTeachingMode) {
	if (TRUE) {
// 2009.10.24 bagus 2点間 修正 --}--
		m_iCurrPoint = m_iNumScans == 0 ? 1 : m_iNumScans + 1;
		m_iCurrIndex = m_iNumScans == 0 ? 0 : m_iNumScans * 2;
		m_iMaxIndex = m_iCurrIndex - 1;
	} else {
		m_iCurrPoint = m_iNumScans == 0 ? 0 : m_iNumScans;
		m_iCurrIndex = m_iNumScans == 0 ? 0 : (m_iNumScans - 1) * 2;
		m_iMaxIndex = m_iCurrIndex;
	}

	SetPointData();

// 2009.10.24 bagus 2点間 修正 --{--
//	if (m_bTeachingMode) {
	if (TRUE) {
// 2009.10.24 bagus 2点間 修正 --}--
		GetDlgItem(IDC_POSITION_2ND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_X2_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_X2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_Y2_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_Y2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_2ND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DISTANCE_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DISTANCE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SYNCHRONOUS_CHECK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_APPLY_NEXT_CHECK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_APPLY_BUTTON)->ShowWindow(SW_SHOW);
	} else {
		GetDlgItem(IDC_POSITION_2ND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_X2_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_X2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_Y2_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_Y2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_2ND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DISTANCE_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DISTANCE)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_SYNCHRONOUS_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_APPLY_NEXT_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_APPLY_BUTTON)->ShowWindow(SW_HIDE);
	}

	m_iProcessStatusOld = -1;

	SetTimer(1, 50, NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeaching2PointDlg::OnOK()
{
//	COverlayDialog::SetOperationLog("OK Button was push.");
//
//	HookKeyUnhook();
//
//	CDialog::OnOK();

	COverlayDialog::OnOK();

	// TODO: この位置にその他の検証用のコードを追加してください
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeaching2PointDlg::OnCancel()
{
	COverlayDialog::OnCancel();

	// TODO: この位置に特別な後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CPointSetupTeaching2PointDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COverlayDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// Position 1st /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_1ST)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Point1st, REFERENCE_COLOR);
			return m_Brush_Point1st;
		}

		///// Position X1 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_X1)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_X1, REFERENCE_COLOR);
			return m_Brush_Position_X1;
		}

		///// Position Y1 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Y1)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_Y1, REFERENCE_COLOR);
			return m_Brush_Position_Y1;
		}

		///// Position 2nd /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_2ND)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Point2nd, REFERENCE_COLOR);
			return m_Brush_Point2nd;
		}

		///// Position X2 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_X2)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_X2, REFERENCE_COLOR);
			return m_Brush_Position_X2;
		}

		///// Position Y2 /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Y2)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_Y2, REFERENCE_COLOR);
			return m_Brush_Position_Y2;
		}

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
void CPointSetupTeaching2PointDlg::OnTimer(UINT nIDEvent)
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
void CPointSetupTeaching2PointDlg::OnNextPointButton()
{
	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

// 2009.10.24 bagus 2点間 修正 --{--
//	if (m_bTeachingMode) {
	if (TRUE) {
// 2009.10.24 bagus 2点間 修正 --}--
// 2009.10.24 bagus 2点間 修正 --{--
//		if ((m_iCurrPoint >= 1) && (m_iCurrIndex <= m_iNumScans * 2)){
		if ((m_iCurrPoint >= 1) && (m_iCurrIndex <= m_iMaxIndex)){
// 2009.10.24 bagus 2点間 修正 --}--
			m_iCurrIndex++;
// 2009.10.28 bagus 2点間 修正 --{--
#if 0
			if ((m_iCurrIndex % 2) == 0) {
				m_iCurrPoint++;
			}
#else
			if ((m_iCurrIndex % 2) == 0) {
				m_iCurrPoint++;
				m_b2nd = FALSE;
			} else {
				m_b2nd = TRUE;
			}
#endif
// 2009.10.28 bagus 2点間 修正 --}--
		} else {
			// Err
			return;
		}
	} else {
		if ((m_iCurrPoint >= 1) && (m_iCurrPoint < m_iNumScans)){
			m_iCurrIndex += 2;
			m_iCurrPoint++;
		} else {
			// Err
			return;
		}
	}
	SetPointData();

	// Pointが登録されている場合にはそのポイントに移動
	if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
		OnMoveButton();
	}
}

// ==========================================================================
// Name：		OnPreviousPointButton
void CPointSetupTeaching2PointDlg::OnPreviousPointButton()
{
	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}


// 2009.10.24 bagus 2点間 修正 --{--
//	if (m_bTeachingMode) {
	if (TRUE) {
// 2009.10.24 bagus 2点間 修正 --}--
		if ((m_iCurrIndex > 0) && (m_iCurrPoint <= m_iNumScans + 1)){
			m_iCurrIndex--;
// 2009.10.28 bagus 2点間 修正 --{--
#if 0
			if ((m_iCurrIndex % 2) != 0) {
				m_iCurrPoint--;
			}
#else
			if ((m_iCurrIndex % 2) != 0) {
				m_iCurrPoint--;
				m_b2nd = TRUE;
			} else {
				m_b2nd = FALSE;
			}
#endif
// 2009.10.28 bagus 2点間 修正 --}--
		} else {
			// Err
			return;
		}
	} else {
		if ((m_iCurrPoint > 1) && (m_iCurrPoint <= m_iNumScans + 1)){
			m_iCurrIndex -= 2;
			m_iCurrPoint--;
		} else {
			// Err
			return;
		}
	}
	SetPointData();

	// そのポイントに移動
	if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
		OnMoveButton();
	}
}

// ==========================================================================
// Name：		OnApplyButton
void CPointSetupTeaching2PointDlg::OnApplyButton()
{
	CString csBuff;

	if (!CheckInputCurrPoint()) {
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, csBuff, "It specifies outside the range.");
		AfxMessageBox(csBuff);
		return;
	}

	// X
	GetDlgItemText(IDC_SAMPLE_X, csBuff);
	m_ScanPoint[m_iCurrIndex].lX = (double)(atof(csBuff) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_SAMPLE_Y, csBuff);
	m_ScanPoint[m_iCurrIndex].lY = (double)(atof(csBuff) * MICROMETRE);

// 2009.10.24 bagus 2点間 修正 --{--
//	m_iNumScans = m_iCurrPoint > m_iNumScans ? m_iNumScans + 1 : m_iNumScans;
	if ((m_iCurrIndex % 2) == 0) {
		// 1stの時は増やさない
		m_iMaxIndex++;
	} else {
		// 2ndの時に増やす
//		m_iNumScans = m_iCurrPoint > m_iNumScans ? m_iNumScans + 1 : m_iNumScans;
		if (m_iCurrPoint > m_iNumScans) {
			m_iNumScans++;
			m_iMaxIndex++;
		}
	}
// 2009.10.24 bagus 2点間 修正 --}--

	// 次のポイントのTeachingに移動
	if(IsDlgButtonChecked(IDC_APPLY_NEXT_CHECK) == TRUE){
// 2009.10.24 bagus 2点間 修正 --{--
//		if (m_bTeachingMode) {
		if (TRUE) {
// 2009.10.24 bagus 2点間 修正 --}--
// 2009.10.24 bagus 2点間 修正 --{--
//			if ((m_iCurrPoint >= 1) && (m_iCurrIndex <= m_iNumScans * 2)){
			if ((m_iCurrPoint >= 1) && (m_iCurrIndex <= m_iMaxIndex)){
// 2009.10.24 bagus 2点間 修正 --}--
				m_iCurrIndex++;
// 2009.10.28 bagus 2点間 修正 --{--
#if 0
				if ((m_iCurrIndex % 2) == 0) {
					m_iCurrPoint++;
				}
#else
				if ((m_iCurrIndex % 2) == 0) {
					m_iCurrPoint++;
					m_b2nd = FALSE;
				} else {
					m_b2nd = TRUE;
				}
#endif
// 2009.10.28 bagus 2点間 修正 --}--
			} else {
				// Err
				return;
			}
		} else {
// 2009.10.24 bagus 2点間 修正 --{--
#if 0
			if ((m_iCurrPoint >= 1) && (m_iCurrPoint < m_iNumScans)){
				m_iCurrIndex += 2;
				m_iCurrPoint++;
			} else {
				// Err
				return;
			}
#endif
// 2009.10.24 bagus 2点間 修正 --}--
		}

		if (TRUE == this->IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK)) {
			this->OnMoveButton();
		}
	}

	// ポイント設定
	SetPointData();

}

// ==========================================================================
// Name：		SetPointData
void CPointSetupTeaching2PointDlg::SetPointData()
{
	CString csPointNo, csX1, csY1, csX2, csY2, csDistance;
	double	dX, dY, dDistance;

// 2009.10.24 bagus 2点間 修正 --{--
//	if(m_iCurrPoint == 0 || m_iCurrPoint > m_iNumScans){
	if(m_iCurrPoint == 0 || m_iCurrIndex > m_iMaxIndex){
// 2009.10.24 bagus 2点間 修正 --}--
		csX1.Format("");
		csY1.Format("");
		csX2.Format("");
		csY2.Format("");
		csDistance.Format("");
	}
	else{
		// X1
		csX1.Format("%.3lf", (double)(m_ScanPoint[m_iCurrIndex].lX) / MICROMETRE);
		// Y1
		csY1.Format("%.3lf", (double)(m_ScanPoint[m_iCurrIndex].lY) / MICROMETRE);
		// X2
		csX2.Format("%.3lf", (double)(m_ScanPoint[m_iCurrIndex + 1].lX) / MICROMETRE);
		// Y2
		csY2.Format("%.3lf", (double)(m_ScanPoint[m_iCurrIndex + 1].lY) / MICROMETRE);

// 2009.10.24 bagus 2点間 修正 --{--
//		if (!m_bTeachingMode) {
		if (FALSE) {
// 2009.10.24 bagus 2点間 修正 --}--
			dX = ((double)(m_ScanPoint[m_iCurrIndex + 1].lX) / MICROMETRE) - ((double)(m_ScanPoint[m_iCurrIndex].lX) / MICROMETRE);
			dY = ((double)(m_ScanPoint[m_iCurrIndex + 1].lY) / MICROMETRE) - ((double)(m_ScanPoint[m_iCurrIndex].lY) / MICROMETRE);
			dDistance = sqrt(pow(dX, 2.0) + pow(dY, 2.0));
			csDistance.Format("%.3lf", dDistance);
		}
	}
	// Point No.
	csPointNo.Format("%d", m_iCurrPoint);

	SetDlgItemText(IDC_POINT_NO, csPointNo);
	SetDlgItemText(IDC_POSITION_X1, csX1);
	SetDlgItemText(IDC_POSITION_Y1, csY1);
	SetDlgItemText(IDC_POSITION_X2, csX2);
	SetDlgItemText(IDC_POSITION_Y2, csY2);
// 2009.10.24 bagus 2点間 修正 --{--
//	if (!m_bTeachingMode) {
	if (FALSE) {
// 2009.10.24 bagus 2点間 修正 --}--
		SetDlgItemText(IDC_DISTANCE, csDistance);
		SetDlgItemText(IDC_POSITION_1ST, "1st");
		SetDlgItemText(IDC_POSITION_2ND, "2nd");
	} else {
		if ((m_iCurrIndex % 2) ==0) {
			SetDlgItemText(IDC_POSITION_1ST, "1st");
		} else {
			SetDlgItemText(IDC_POSITION_1ST, "2nd");
		}
	}
}

// ==========================================================================
// Name：		OnMoveButton
void CPointSetupTeaching2PointDlg::OnMoveButton()
{
// 2010.02.02 K.Matsuo PIN UPPER インターロックチェック漏れ対応 -->
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
// 2010.02.02 K.Matsuo PIN UPPER インターロックチェック漏れ対応 <--

	if(m_iCurrPoint <= m_iNumScans){
		SetPointData();
	}

	// Pointが登録されている場合にはそのポイントに移動
	if(m_iCurrPoint <= m_iNumScans){
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
// 2009.12.02 K.Matsuo SampleOrigin -->
//		sampleCoord.lX = m_ScanPoint[m_iCurrIndex].lX + m_StageProgInfoHdr.SampleInfo.Origin.lX;
//		sampleCoord.lY = m_ScanPoint[m_iCurrIndex].lY + m_StageProgInfoHdr.SampleInfo.Origin.lY;
		SAMPLEINFO sampleInfo;
		::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
		RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
		sampleCoord.lX = m_ScanPoint[m_iCurrIndex].lX + sampleInfo.Origin.lX;
		sampleCoord.lY = m_ScanPoint[m_iCurrIndex].lY + sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
}


// =========================================================================
//
void CPointSetupTeaching2PointDlg::SoftJoyStickXyzEnable(BOOL bEnable)
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

void CPointSetupTeaching2PointDlg::OnHomePositionButton()
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

BOOL CPointSetupTeaching2PointDlg::CheckInputCurrPoint()
{
	CString	sCurrPoint;
	DWORD	dwCurrPoint;

	GetDlgItemText(IDC_POINT_NO, sCurrPoint);
	dwCurrPoint = strtol(sCurrPoint, NULL, 10);

	if ((dwCurrPoint >= 1) && (dwCurrPoint <= m_iNumScans + 1)){
		m_iCurrPoint = dwCurrPoint;
// 2009.10.24 bagus 2点間 修正 --{--
		m_iCurrIndex = (dwCurrPoint - 1) * 2;
// 2009.10.24 bagus 2点間 修正 --}--
// 2009.10.28 bagus 2点間 修正 --{--
		if (m_b2nd)
			m_iCurrIndex++;
// 2009.10.28 bagus 2点間 修正 --}--
	} else {
		// Err
		return ( FALSE );
	}

	return ( TRUE );
}
// 2009.11.26 bagus 全般 修正 --{--
void CPointSetupTeaching2PointDlg::ButtonEnableCheck()
{
	COverlayDialog::ButtonEnableCheck();

	//ステージ移動中は移動させない
	BOOL bNextPrevButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	GetDlgItem(IDC_NEXT_POINT_BUTTON)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_PREVIOUS_POINT_BUTTON)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_MOVE_POINT)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_POINT_NO)->EnableWindow(bNextPrevButtonEnable);

}
// 2009.11.26 bagus 全般 修正 --}--
