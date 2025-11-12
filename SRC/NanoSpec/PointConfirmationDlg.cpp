// PointConfirmationDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "PointConfirmationDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//Saiki 20110207 Delete ----->
//#define	PIN_INTERVAL		7
//Saiki 20110207 Delete <-----

/////////////////////////////////////////////////////////////////////////////
// CPointConfirmationDlg ダイアログ
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CPointConfirmationDlg::CPointConfirmationDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPointConfirmationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointConfirmationDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_wHeadType = HEAD_TYPE_SR;
	m_lYPoint = 0;
	m_nSelectedPoint = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointConfirmationDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointConfirmationDlg)
	DDX_Control(pDX, IDC_NEXT_POINT_BUTTON, m_NextPointButton);
	DDX_Control(pDX, IDC_PREVIOUS_POINT_BUTTON, m_PreviousPointButton);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MOVE_POINT, m_MoveButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CPointConfirmationDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPointConfirmationDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_NEXT_POINT_BUTTON, OnNextPointButton)
	ON_BN_CLICKED(IDC_PREVIOUS_POINT_BUTTON, OnPreviousPointButton)
	ON_BN_CLICKED(IDC_VALID_MEASUREMENT_CHECK, OnCheckValidPoint)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MOVE_POINT, OnMoveButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointConfirmationDlg メッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CPointConfirmationDlg::OnInitDialog()
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

	///// Move Button /////
	m_MoveButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MoveButton.DrawFlatFocus(TRUE);

	// 測定ポイントを表示
	if ((m_nSelectedPoint >= 1) && (m_nSelectedPoint <= m_iNumScans)){
		m_iCurrPoint = m_nSelectedPoint;
	} else {
		m_iCurrPoint = 1;
	}

	SetPointData();

	m_iProcessStatusOld = -1;

	SetTimer(1, 50, NULL);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointConfirmationDlg::OnOK()
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
void CPointConfirmationDlg::OnCancel()
{
//	COverlayDialog::SetOperationLog("OK Button was push.");
//
//	HookKeyUnhook();
//
//	CDialog::OnCancel();

	COverlayDialog::OnCancel();

	// TODO: この位置に特別な後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CPointConfirmationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COverlayDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// Position X /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_X)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_X, REFERENCE_COLOR);
			return m_Brush_Position_X;
		}

		///// Position Y /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Y)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_Y, REFERENCE_COLOR);
			return m_Brush_Position_Y;
		}

		break;
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointConfirmationDlg::OnTimer(UINT nIDEvent)
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
void CPointConfirmationDlg::OnNextPointButton()
{
	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

	if ((m_iCurrPoint >= 1) && (m_iCurrPoint < m_iNumScans)){
		m_iCurrPoint++;
		SetPointData();
	} else {
		// Err
		return;
	}

	// Pointが登録されている場合にはそのポイントに移動
	if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
		OnMoveButton();
	}
}

// ==========================================================================
// Name：		OnPreviousPointButton
void CPointConfirmationDlg::OnPreviousPointButton()
{
	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

	if ((m_iCurrPoint > 1) && (m_iCurrPoint < m_iNumScans + 1)){
		m_iCurrPoint--;
		SetPointData();
	} else {
		// Err
		return;
	}

	// そのポイントに移動
	if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
		OnMoveButton();
	}
}

// ==========================================================================
// Name：		SetPointData
void CPointConfirmationDlg::SetPointData()
{
	CString csPointNo, csX, csY;

	if(m_iCurrPoint == 0 || m_iCurrPoint > m_iNumScans){
		csX.Format("");
		csY.Format("");
	}
	else{
		// X
		csX.Format("%.3lf", (double)(m_ScanPoint[m_iCurrPoint - 1].lX) / MICROMETRE);
		// Y
		csY.Format("%.3lf", (double)(m_ScanPoint[m_iCurrPoint -1].lY) / MICROMETRE);
	}
	// Point No.
	csPointNo.Format("%d", m_iCurrPoint);

	SetDlgItemText(IDC_POINT_NO, csPointNo);
	SetDlgItemText(IDC_POSITION_X, csX);
	SetDlgItemText(IDC_POSITION_Y, csY);

	CheckDlgButton(IDC_VALID_MEASUREMENT_CHECK, m_ValidPoint[m_iCurrPoint -1]);
}

// ==========================================================================
// Name：		OnMoveButton
void CPointConfirmationDlg::OnMoveButton()
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

	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

	SetPointData();

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	STAGE_COORD sampleCoord;
// 2009.12.02 K.Matsuo SampleOrigin -->
//	sampleCoord.lX = m_ScanPoint[m_iCurrPoint - 1].lX + m_StageProgInfoHdr.SampleInfo.Origin.lX;
//	sampleCoord.lY = m_ScanPoint[m_iCurrPoint - 1].lY + m_StageProgInfoHdr.SampleInfo.Origin.lY;
	SAMPLEINFO sampleInfo;
	::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
	RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
	sampleCoord.lX = m_ScanPoint[m_iCurrPoint - 1].lX + sampleInfo.Origin.lX;
	sampleCoord.lY = m_ScanPoint[m_iCurrPoint - 1].lY + sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
	NS_StageMoveAbsolute(&sampleCoord);
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}


// =========================================================================
//
void CPointConfirmationDlg::SoftJoyStickXyzEnable(BOOL bEnable)
{
	//SoftJoyStick
	m_XY_UpButton.SetEnabled(bEnable);
	m_XY_DownButton.SetEnabled(bEnable);
	m_XY_RightButton.SetEnabled(bEnable);
	m_XY_LeftButton.SetEnabled(bEnable);
	m_Z_UpButton.SetEnabled(bEnable);
	m_Z_DownButton.SetEnabled(bEnable);
}

BOOL CPointConfirmationDlg::CheckInputCurrPoint()
{
	CString	sCurrPoint;
	DWORD	dwCurrPoint;

	GetDlgItemText(IDC_POINT_NO, sCurrPoint);
	dwCurrPoint = strtol(sCurrPoint, NULL, 10);

	if ((dwCurrPoint >= 1) && (dwCurrPoint < (DWORD)(m_iNumScans + 1))){
		m_iCurrPoint = dwCurrPoint;
	} else {
		// Err
		return ( FALSE );
	}

	return ( TRUE );
}

void CPointConfirmationDlg::OnCheckValidPoint()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	m_ValidPoint[m_iCurrPoint -1] = (IsDlgButtonChecked(IDC_VALID_MEASUREMENT_CHECK) == BST_CHECKED);
}
