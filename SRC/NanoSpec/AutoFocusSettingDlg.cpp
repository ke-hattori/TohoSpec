// AutoFocusSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "AutoFocusSettingDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoFocusSettingDlg ダイアログ


CAutoFocusSettingDlg::CAutoFocusSettingDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CAutoFocusSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoFocusSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

//2009.09.24 bagus stress 機能追加 --{--
	m_lX = 0;
	m_lY = 0;
	m_lZ = 0;

	m_wHeadType = HEAD_TYPE_SR;
//2009.09.24 bagus stress 機能追加 --}--
}


void CAutoFocusSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoFocusSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SETTING_POINT_BUTTON, m_SettingPointButton);
}


BEGIN_MESSAGE_MAP(CAutoFocusSettingDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CAutoFocusSettingDlg)
	ON_BN_CLICKED(IDC_SETTING_POINT_BUTTON, OnSettingPointButton)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoFocusSettingDlg メッセージ ハンドラ
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAutoFocusSettingDlg::OnInitDialog()
{
	//Saiki 20091016 Add ----->
	CString strBuffer;
	LoadStringML(IDS_SETTING_POINT_BUTTON, strBuffer, "SETTING POINT");
	CreateDialogItem("BUTTON", strBuffer, BS_PUSHBUTTON,  CRect(CPoint(300,388), CSize(72,24)),   IDC_SETTING_POINT_BUTTON);

	LoadStringML(IDS_ORIGIN_POS, strBuffer, "Position(mm)");
	CreateDialogItem("STATIC", strBuffer,		SS_CENTERIMAGE, CRect(CPoint(300,364), CSize(44,12)),	IDC_SETTING_POINT_LABEL);

	CreateDialogItem("STATIC", "X", 			SS_CENTERIMAGE, CRect(CPoint(351,364), CSize(8,12)),	IDC_SETTING_POINT_LABEL_X);
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,	CRect(CPoint(361,364), CSize(50,12)),	IDC_POSITION_X);

	CreateDialogItem("STATIC", "Y", 			SS_CENTERIMAGE, CRect(CPoint(421,364), CSize(8,12)),	IDC_SETTING_POINT_LABEL_Y);
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,	CRect(CPoint(431,364), CSize(50,12)),	IDC_POSITION_Y);

	CreateDialogItem("STATIC", "Z", 			SS_CENTERIMAGE, CRect(CPoint(492,364), CSize(8,12)),	IDC_SETTING_POINT_LABEL_Z);
	CreateDialogItem("STATIC", "",				SS_SUNKEN | SS_RIGHT,	CRect(CPoint(502,364), CSize(50,12)),	IDC_POSITION_Z);

	//Saiki 20091016 Add <-----

//2009.09.24 bagus stress 機能追加 --{--
	//Sampleセット
	SetFileName(m_StageProgInfoHdr.SampleInfo.szName);
	SetSampleMode(1);

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_StageProgInfoHdr.SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

//	RCP_DATA rcp_data;
//	m_pDoc->GetRcpData(&rcp_data);
//	m_wHeadType = rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType;
//2009.09.24 bagus stress 機能追加 --}--
	COverlayDialog::OnInitDialog();

//2009.09.24 bagus stress 機能追加 --{--
	///// SettingPoint Button /////
	m_SettingPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SettingPointButton.DrawFlatFocus(TRUE);

//2009.11.30 bagus stress --{--
//	if (m_wHeadType == HEAD_TYPE_STRESS) {
//2009.11.30 bagus stress --}--
	if (m_StageProgInfoHdr.wHeadType == STAGE_PGM_TYPE_STRESS){
		NS_StageGetPos(&m_CurrPos);
		m_lX = m_CurrPos.lX;
		m_lY = m_CurrPos.lY;
		m_lZ = m_CurrPos.lZ;

		SetPointData();

		GetDlgItem(IDC_SETTING_POINT_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SETTING_POINT_LABEL_X)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SETTING_POINT_LABEL_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SETTING_POINT_LABEL_Z)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_X)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_POSITION_Z)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SETTING_POINT_BUTTON)->ShowWindow(SW_SHOW);
	} else {
		GetDlgItem(IDC_SETTING_POINT_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SETTING_POINT_LABEL_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SETTING_POINT_LABEL_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SETTING_POINT_LABEL_Z)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_Z)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SETTING_POINT_BUTTON)->ShowWindow(SW_HIDE);
	}
//2009.09.24 bagus stress 機能追加 --}--

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CAutoFocusSettingDlg::OnOK()
{
//2009.09.24 bagus stress 機能追加 --{--
	CString strMsg, strTitle;

	if (m_wHeadType == HEAD_TYPE_STRESS) {
		// Y軸のチェックを行う
		NS_StageGetPos(&m_CurrPos);
		if (m_lY != m_CurrPos.lY) {
			// Y軸の移動は禁止
			LoadStringML(IDS_OVERLAY_MOVE_Y_AXIS, strMsg, "The value of Y axis has been changed.");
			LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
			MessageBox(strMsg, strTitle, MB_OK);
			return;
		}
	} else {
		// Y軸のチェックは不要
	}
//2009.09.24 bagus stress 機能追加 --}--

	COverlayDialog::OnOK();

	//チーフへメッセージを投げる
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		// AFポップアップ終了応答
		::PostMessage(l_hChiefDlg, WM_DISP_AF_POPUP_END, 1, MAKELPARAM(MAKEWORD(m_XY_ButtonUseHistry,m_Z_ButtonUseHistry),MAKEWORD(m_AutoFocusUseHistry,m_AutoFocusResult)));
	} else {
		//エラー処理記述すること！
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CAutoFocusSettingDlg::OnCancel()
{
	COverlayDialog::OnCancel();

	//チーフへメッセージを投げる
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if (0 != l_hChiefDlg) {
		// AFポップアップ終了応答
		::PostMessage(l_hChiefDlg, WM_DISP_AF_POPUP_END, 3, MAKELPARAM(MAKEWORD(m_XY_ButtonUseHistry,m_Z_ButtonUseHistry),MAKEWORD(m_AutoFocusUseHistry,m_AutoFocusResult)));
	} else {
		//エラー処理記述すること！
	}
}

void CAutoFocusSettingDlg::OnSettingPointButton()
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

// 2009.12.02 K.Matsuo SampleOrigin -->
	SAMPLEINFO sampleInfo;
// 2009.12.02 K.Matsuo SampleOrigin <--

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	STAGE_COORD sampleCoord;
// 2009.12.02 K.Matsuo SampleOrigin -->
//	sampleCoord.lX = m_lX + m_StageProgInfoHdr.SampleInfo.Origin.lX;
//	sampleCoord.lY = m_lY + m_StageProgInfoHdr.SampleInfo.Origin.lY;
	::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
	RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
	sampleCoord.lX = m_lX + sampleInfo.Origin.lX;
	sampleCoord.lY = m_lY + sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
	NS_StageMoveAbsolute(&sampleCoord);
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
}

HBRUSH CAutoFocusSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COverlayDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

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

//2009.09.24 bagus stress 機能追加 --{--
		///// Position Z /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POSITION_Z)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_Position_Z, REFERENCE_COLOR);
			return m_Brush_Position_Z;
		}
//2009.09.24 bagus stress 機能追加 --}--

		break;
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

//2009.09.24 bagus stress 機能追加 --{--
void CAutoFocusSettingDlg::SetPointData()
{
	CString csPointNo, csX, csY, csZ;

	// X
	csX.Format("%.3lf", m_lX / (double)(MICROMETRE));
	// Y
	csY.Format("%.3lf", m_lY / (double)(MICROMETRE));
	// Z
	csZ.Format("%.3lf", m_lZ / (double)(MICROMETRE) / 10.0);

	SetDlgItemText(IDC_POSITION_X, csX);
	SetDlgItemText(IDC_POSITION_Y, csY);
	SetDlgItemText(IDC_POSITION_Z, csZ);
}

//2009.09.24 bagus stress 機能追加 --}--
