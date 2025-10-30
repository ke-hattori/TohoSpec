// AdditionalMeasurementDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "AdditionalMeasurementDlg.h"
#include "ManualMeasurementFormView.h"
#include "MeasurementTabView.h"
#include "..\\..\\INC\\NSStage.hxx"
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
#include "ChiefView.h"
#include "ChifTransiMaster.h"
extern CChiefView*	g_pcChiefView;	/* static 用クラスポインタ	*/
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */


#define ADDIMEAS_ENABLE_CHANGE_TIMER	1


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdditionalMeasurementDlg ダイアログ


CAdditionalMeasurementDlg::CAdditionalMeasurementDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CAdditionalMeasurementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiPositionSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

// 2009.10.30 bagus Stage-Sample 修正 --{--
	::ZeroMemory(m_szRecipeName, sizeof(m_szRecipeName));
	m_nSampleMode = 0;
// 2009.10.30 bagus Stage-Sample 修正 --}--
//2009.11.29 bagus GTR --{--
	m_bGTR = FALSE;
//2009.11.29 bagus GTR --}--
}


void CAdditionalMeasurementDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdditionalMeasurementDlg)
	DDX_Control(pDX, IDC_MEASUREMENT_BUTTON, m_MeasurementButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdditionalMeasurementDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CAdditionalMeasurementDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_MEAS_POINT, OnSelchangeComboMeasPoint)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MEASUREMENT_BUTTON, OnMeasurementButton)
	ON_MESSAGE(WM_TESTMODE_ONEPOINT_END, OnManuModeOnePointEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdditionalMeasurementDlg メッセージ ハンドラ
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAdditionalMeasurementDlg::OnInitDialog()
{
// 2009.10.30 bagus Stage-Sample 修正 --{--
	//Sampleセット
	if (m_nSampleMode == 1) {
		SetFileName(m_szRecipeName);
	}
	SetSampleMode(m_nSampleMode);
// 2009.10.30 bagus Stage-Sample 修正 --}--
	SYSTEM_CONFIG SystemConfig;
	LPCTSTR pszHeader[LANGUAGE_MAX][2] = {
		{_T("Point No"),_T("Position(mm)")},
		{_T("ポイントNo"),_T("位置(mm)")},
	};
	ConfigFile_GetNanoSpecIni(&SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

//	  PUSHBUTTON	  "MEASUREMENT",IDC_MEASUREMENT_BUTTON,306,366,85,28

//	CreateDialogItem("BUTTON", "MEASUREMENT",	BS_PUSHBUTTON,	CRect(CPoint(306,366), CSize(85,28)),	IDC_MEASUREMENT_BUTTON);
	//Saiki 20090526 Change ----->
	//CreateDialogItem("BUTTON", "測定",  BS_PUSHBUTTON,  CRect(CPoint(306,366), CSize(85,28)),   IDC_MEASUREMENT_BUTTON);
	CString strBuffer;
	LoadStringML(IDS_MEASUREMENT_BUTTON, strBuffer, "MEASUREMENT");
	CreateDialogItem("BUTTON", strBuffer,  BS_PUSHBUTTON,  CRect(CPoint(306,400), CSize(85,28)),   IDC_MEASUREMENT_BUTTON);
	//Saiki 20090526 Change <-----
	//2009.11.29 bagus GTR --{--
	LoadStringML(IDS_MEASUREMENT_BUTTON, strBuffer, "Point No.");
	//2010.01.15 bagus GTR --{--
	//CreateDialogItem("STATIC", _T("Point No."),  SS_CENTERIMAGE,	CRect(CPoint(306 ,366), CSize(50,14)),	 IDC_POINT_NO);
	CreateDialogItem("STATIC", pszHeader[SystemConfig.nLanguage][0],  SS_CENTERIMAGE,  CRect(CPoint(306 ,366), CSize(50,14)),	IDC_POINT_NO);
	//2010.01.15 bagus GTR --}--
	CreateDialogItem("COMBOBOX", _T("--"),	CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,	CRect(CPoint(336,366), CSize(45,104)),	 IDC_COMBO_MEAS_POINT);
	//2010.01.15 bagus GTR --{--
	//CreateDialogItem("STATIC", _T("Position(mm)"),  SS_CENTERIMAGE,  CRect(CPoint(400 ,366), CSize(50,14)),	IDC_POSITION_NO);
	CreateDialogItem("STATIC", pszHeader[SystemConfig.nLanguage][1],  SS_CENTERIMAGE,  CRect(CPoint(400 ,366), CSize(50,14)),	IDC_POSITION_NO);
	//2010.01.15 bagus GTR --}--
	CreateDialogItem("STATIC", _T("X"),  SS_CENTERIMAGE,  CRect(CPoint(450 ,366), CSize(20,14)),   IDC_POSITION_X1);
// 2010.01.21 bagus Gantry --{--
//	CreateDialogItem("STATIC", _T(""),	SS_SUNKEN | SS_RIGHT,  CRect(CPoint(460 ,366), CSize(60,14)),	IDC_MEAS_POINT_X);
	CreateDialogItem("STATIC", _T(""),	SS_CENTERIMAGE | SS_SUNKEN | SS_RIGHT,	CRect(CPoint(460 ,366), CSize(60,14)),	 IDC_MEAS_POINT_X);
// 2010.01.21 bagus Gantry --}--
	CreateDialogItem("STATIC", _T("Y"), SS_CENTERIMAGE,  CRect(CPoint(530,366), CSize(20,14)),	 IDC_POSITION_Y1);
// 2010.01.21 bagus Gantry --{--
//	CreateDialogItem("STATIC", _T(""),	SS_SUNKEN | SS_RIGHT,  CRect(CPoint(540,366), CSize(60,14)),   IDC_MEAS_POINT_Y);
	CreateDialogItem("STATIC", _T(""),	SS_CENTERIMAGE | SS_SUNKEN | SS_RIGHT,	CRect(CPoint(540,366), CSize(60,14)),	IDC_MEAS_POINT_Y);
// 2010.01.21 bagus Gantry --}--

// 2010.01.29 bagus Gantry --{--
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
// 2010.01.29 bagus Gantry --}--

	//2009.12.11 bagus 修正 --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//2009.12.11 bagus 修正 --{--
	if(m_bGTR){
		SR_CONFIG SrConfig;
		char szMeasPt[10+1];
		CString str;
		CSize sz;
		ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
		//pCombo->SetExtendedUI();
		//2009.12.11 bagus 修正 --{--
		//pCombo->AddString("<select>");
		pCombo->AddString(strSelectCaption);
		//2009.12.11 bagus 修正 --}--
// 2010.01.29 bagus Gantry --{--
#if 0
		for(int iMeasPt =1; iMeasPt <= SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
			_itoa(iMeasPt, szMeasPt, 10);
			pCombo->AddString(szMeasPt);
		}
#else
		RCP_DATA rcp_data;
		char szRecipeName[RECIPE_NAME_LEN + 1];
		STAGE_COORD tScanPoint[SCAN_POINT_MAX];
		SR_REFERENCE_POSITION SrReferencePosition;
		STAGE_PROG_INFO_HDR tStageProgInfoHdr;
		WORD wNumScans;
		int j;

		m_pDoc->GetRcpData(&rcp_data);
		::ZeroMemory(tScanPoint, sizeof(tScanPoint));

		strcpy(szRecipeName, rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);

		ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

		if ( !RecipeFile_LoadRecipe(&tStageProgInfoHdr, szRecipeName, RECIPE_FILE_STAGE_PROGRAM) ) {
			wNumScans = 0;
		} else {
			wNumScans = tStageProgInfoHdr.wNumScans;
		}
		if ( !RecipeFile_LoadPointList(tScanPoint, wNumScans, szRecipeName) ) {
			::ZeroMemory(tScanPoint, sizeof(tScanPoint));
		}

		for(int iMeasPt =1; iMeasPt <= SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
			for (j=0; j<wNumScans; j++) {
				if ((SrReferencePosition.LocGantryTransmitReference[iMeasPt-1].lX == tScanPoint[j].lX)
				 && (SrReferencePosition.LocGantryTransmitReference[iMeasPt-1].lY == tScanPoint[j].lY)) {
					_itoa(iMeasPt, szMeasPt, 10);
					pCombo->AddString(szMeasPt);
					break;
				}
			}
		}
#endif
// 2010.01.29 bagus Gantry --}--
		pCombo->SetCurSel(0);
		strBuffer.Format("%s","Position(mm)");
		GetDlgItemText(IDC_POSITION_NO,strBuffer);
		strBuffer.Format("%s","Point No");
		GetDlgItemText(IDC_POINT_NO,strBuffer);
		strBuffer.Format("%s","X");
		GetDlgItemText(IDC_POSITION_X1,strBuffer);
		strBuffer.Format("%s","Y");
		GetDlgItemText(IDC_POSITION_Y1,strBuffer);
		//コンボボックスを同期する
		CComboBox *pComboBase;
		CComboBox* pCombo2;
		pCombo2 = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
		pComboBase = (CComboBox *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_COMBO_MEAS_POINT);
		if(pComboBase && pCombo2){
			int iSelectMeasPt = pComboBase->GetCurSel();
			if(iSelectMeasPt != pCombo2->GetCurSel()){
				pCombo2->SetCurSel(iSelectMeasPt);
			}
			//リファレンスポジションのロード
			SR_REFERENCE_POSITION SrReferencePosition;
			ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

			SetPointData(SrReferencePosition,iSelectMeasPt);
		}
	}else{
		GetDlgItem(IDC_MEAS_POINT_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MEAS_POINT_X)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_Y1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_X1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POSITION_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_POINT_NO)->ShowWindow(SW_HIDE);
	}
	//2009.11.29 bagus GTR --}--

	COverlayDialog::OnInitDialog();

// 2010.01.29 bagus Gantry --{--
//	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
//	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
// 2010.01.29 bagus Gantry --}--

	m_pMainFrame->m_pManuMeaFormViewObj->m_pAddMeasDlgObj = this;

	///// Measurement Button /////
	m_MeasurementButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MeasurementButton.DrawFlatFocus(TRUE);

#if 0 // CANCELボタンを復活させた
	// Cancelボタン非表示 と Okボタン移動
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	CRect rect;
	GetDlgItem(IDCANCEL)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDOK)->MoveWindow(rect);
#endif

// 2009.10.29 bagus Jog 追加修正 --{--

	if (SystemConfig.nJoystickType == 0) {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	}
// 2009.10.29 bagus Jog 追加修正 --}--

	m_iProcessStatusOld = -1;
	m_iManuMeasModeOld = -1;

	SetTimer(ADDIMEAS_ENABLE_CHANGE_TIMER, 50, NULL);


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
//
void CAdditionalMeasurementDlg::OnOK()
{
	COverlayDialog::OnOK();

	// 現在の座標を引き渡す
	m_lX = m_CurrPos.lX;
	m_lY = m_CurrPos.lY;
	m_lZ = m_CurrPos.lZ;

}

/////////////////////////////////////////////////////////////////////////////
//
void CAdditionalMeasurementDlg::OnCancel()
{
	COverlayDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
void CAdditionalMeasurementDlg::OnTimer(UINT nIDEvent)
{
	int iProcessStatus = m_pDoc->GetProcessStatus();

	switch (nIDEvent)
	{
		case ADDIMEAS_ENABLE_CHANGE_TIMER:		// 50ms 定周期タイマ
			iProcessStatus = m_pDoc->GetProcessStatus();
			if(iProcessStatus != m_iProcessStatusOld)
			{

				if(iProcessStatus == PROCESS_DOWN) //ダウンからの復旧は想定外
				{
					ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
				}
#if 0
				if((iProcessStatus == PROCESS_WAIT)||(iProcessStatus == PROCESS_DOWN))
				{
					ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
				}
				else
				{
					ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);
				}
#endif

			}
			m_iProcessStatusOld = iProcessStatus;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
			this->ButtonEnableCheck();
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
			break;
	}



	int iManuMeasMode = m_pDoc->GetManuMeasMode();

	//[測定完了]ボタン押下時のボタン制限処理(マニュアル測定時のみ)
	if((m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)&&(iManuMeasMode != m_iManuMeasModeOld))
	{
		if(iManuMeasMode == MANUMODE_MEAS_COMP) //[測定完了]押下時
		{
			ButtonEnableChange(-1); //-1はswitch文にかからない数字
		}
	}
	//前回値更新
	m_iManuMeasModeOld = iManuMeasMode;



	COverlayDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
void CAdditionalMeasurementDlg::OnMeasurementButton()
{
	// TODO: この位置に特別な後処理を追加してください。

/* added 2009.11.27 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	this->m_bProgress1PMeas = TRUE;
/* added 2009.11.27 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

	COverlayDialog::SetOperationLog("MEASUREMENT Button was push.");

	// ここに追加測定の処理を入れる
	//2009.11.30 bagus --{--
	//ステージ移動中
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)){
		return;
	}
	//2009.11.30 bagus --}--

	//マニュアル測定モード設定
	m_pDoc->SetManuMeasMode(MANUMODE_ON_MEASURE); //測定中に設定


///ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);



/* modified 2009.11.27 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
//	m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas();
/* modified 2009.11.27 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ----------				*/
	if (TRUE != m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas()) {
		this->m_bProgress1PMeas = FALSE;
	}
/* modified 2009.11.27 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
}

// ==========================================================================
//測定中⇔測定終了 時のボタン有効/無効の切替え
void CAdditionalMeasurementDlg::ButtonEnableChange(int Select)
{

	if((m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)&&(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP)) //マニュアル測定 かつ [測定完了]押下時
	{
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]ボタン無効
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);					//[EXIT]ボタン有効
		return;
	}


	BOOL bIdle;
	bIdle = (Select == MEASUREMENT_MANUALMEASURE_E);


	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(bIdle); 		//[AUTO FOCUS]ボタン
	GetDlgItem(IDC_CAPTURE_BUTTON)->EnableWindow(bIdle);			//[CAPTURE]ボタン
	GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(bIdle);		//[SPECIFY COORD]ボタン
	GetDlgItem(IDC_JOG_SETTING_BUTTON)->EnableWindow(bIdle);		//[JOG SETTING]ボタン
	GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(bIdle);		//[MEASUREMENT]ボタン

	GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(bIdle); 		//MOTION MODE選択
	GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(bIdle);			//ELEV MOTION選択
	GetDlgItem(IDC_HEAD_TYPE)->EnableWindow(bIdle); 				//HEAD TYPE選択
	GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(bIdle);	//レンズ倍率選択
	GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(bIdle);  //カメラ選択

	GetDlgItem(IDOK)->EnableWindow(bIdle);							//[OK]ボタン
//	GetDlgItem(IDCANCEL)->EnableWindow(bIdle);						//[CANCEL]ボタン


	//ジョイスティック切替え(ソフト⇔ハード)ラジオボタン
	GetDlgItem(IDC_JOYSTICK1)->EnableWindow(bIdle);
	GetDlgItem(IDC_JOYSTICK2)->EnableWindow(bIdle);

	m_pMainFrame->m_ManualMeasMenuEnableFlg = bIdle;
}



// =========================================================================
//
void CAdditionalMeasurementDlg::SoftJoyStickXyzEnable(BOOL bEnable)
{
	//SoftJoyStick
	m_XY_UpButton.SetEnabled(bEnable);
	m_XY_DownButton.SetEnabled(bEnable);
	m_XY_RightButton.SetEnabled(bEnable);
	m_XY_LeftButton.SetEnabled(bEnable);
	m_Z_UpButton.SetEnabled(bEnable);
	m_Z_DownButton.SetEnabled(bEnable);
//Saiki 20091015 Add ----->
	m_Z_HomePosButton.SetEnabled(bEnable);
//Saiki 20091015 Add <-----
}



// ==========================================================================
//
LRESULT CAdditionalMeasurementDlg::OnManuModeOnePointEnd(WPARAM wparam, LPARAM lparam)
{
///	ButtonEnableChange(MEASUREMENT_MANUALMEASURE_E);
	return 0L;
}


void CAdditionalMeasurementDlg::OnDestroy()
{
	COverlayDialog::OnDestroy();

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	m_pMainFrame->m_pManuMeaFormViewObj->m_pAddMeasDlgObj = NULL;

}
void CAdditionalMeasurementDlg::OnSelchangeComboMeasPoint()
{
#if 0
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//ステージコンフィグのロード
	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	//リファレンスポジションのロード
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	iSelectMeasPt = pCombo->GetCurSel();
	if(0 < iSelectMeasPt && iSelectMeasPt <= SrConfig.wGantryNumOfMeasPoint){
		//XY座標の表示を変更
		SetPointData(SrReferencePosition, iSelectMeasPt);
		//カーソル番号に対応したステージ位置へ移動
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
		sampleCoord.lX = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX/* + m_StageProgInfoHdr.SampleInfo.Origin.lX*/;
		sampleCoord.lY = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY/* + m_StageProgInfoHdr.SampleInfo.Origin.lY*/;
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		//ボタン等の無効化
	}
#endif
	CComboBox *pComboBase;
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	pComboBase = (CComboBox *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_COMBO_MEAS_POINT);
	if(pComboBase && pCombo){
// 2010.01.29 bagus Gantry --{--
#if 0
		int iSelectMeasPt = pCombo->GetCurSel();
		if(iSelectMeasPt != pComboBase->GetCurSel()){
			pComboBase->SetCurSel(iSelectMeasPt);
			((CManualMeasurementFormView *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0)))->PostMessage(WM_COMMAND,(WPARAM)MAKEWPARAM(IDC_COMBO_MEAS_POINT,CBN_SELCHANGE),(LPARAM)pComboBase->GetSafeHwnd());
		}
#else
		int iSelectMeasPt;
		int iIndex;

		iIndex = pCombo->GetCurSel();

		RCP_DATA rcp_data;
		m_pDoc->GetRcpData(&rcp_data);
		if(m_bGTR){
			/* GTR の場合*/
			TCHAR l_tszGTRpos[32];
			this->GetDlgItemText(IDC_COMBO_MEAS_POINT, l_tszGTRpos, sizeof(TCHAR) * 32);
			iSelectMeasPt = _ttol(l_tszGTRpos);
		} else {
			iSelectMeasPt = pCombo->GetCurSel();
		}

		if(iIndex != pComboBase->GetCurSel()){
			pComboBase->SetCurSel(iIndex);
			((CManualMeasurementFormView *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0)))->PostMessage(WM_COMMAND,(WPARAM)MAKEWPARAM(IDC_COMBO_MEAS_POINT,CBN_SELCHANGE),(LPARAM)pComboBase->GetSafeHwnd());
		}
#endif
// 2010.01.29 bagus Gantry --}--
		SR_CONFIG SrConfig;
		ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

		//リファレンスポジションのロード
		SR_REFERENCE_POSITION SrReferencePosition;
		ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

		SetPointData(SrReferencePosition,iSelectMeasPt);
	}
}

// ==========================================================================
// Name：		SetPointData
void CAdditionalMeasurementDlg::SetPointData(SR_REFERENCE_POSITION SrReferencePosition, int iSelectMeasPt)
{
	CString csPointNo, csX, csY;

	// X
	csX.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX) / MICROMETRE);
	// Y
	csY.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY) / MICROMETRE);

	SetDlgItemText(IDC_MEAS_POINT_X, csX);
	SetDlgItemText(IDC_MEAS_POINT_Y, csY);
}

HBRUSH CAdditionalMeasurementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COverlayDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_MEAS_POINT_X)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_PositionX, REFERENCE_COLOR);
			return m_Brush_PositionX;
		}
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_MEAS_POINT_Y)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_PositionY, REFERENCE_COLOR);
			return m_Brush_PositionY;
		}
		break;
	}

	return hbr;
}

/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
/* 動作系コントロールの Enable/Disable	*/
void CAdditionalMeasurementDlg::ButtonEnableCheck(void)
{
	static bool ls_bTrMasIdlePre = ((CChiefTransiMaster*) g_pcChiefView->m_pcChiefTransiMaster)->IsIdle();
	static struct {
		BOOL	l_bCtrlEna;
		int		l_bCtrlID;
	} l_MvCtrls[] = {
		TRUE,	IDC_MEASUREMENT_BUTTON,			/* 01:測定				*/
		TRUE,	IDC_HEAD_TYPE,					/* 02:ヘッドタイプ		*/
		TRUE,	IDC_LENS_MAGNIFICATION_COMBO,	/* 03:レンズ			*/
		TRUE,	IDC_AUTO_FOCUS_BUTTON,			/* 04:オートフォーカス	*/
		TRUE,	IDC_SPECIFY_COORD_BUTTON,		/* 05:座標入力			*/
		TRUE,	IDC_ELEV_UP_BUTTON,				/* 06:Z 軸 W.D.近		*/
		TRUE,	IDC_HOME_POSITION_BUTTON,		/* 07:Z 軸 H.ポジション	*/
		TRUE,	IDC_ELEV_DOWN_BUTTON,			/* 08:Z 軸 W.D.遠		*/
		TRUE,	IDC_STAGE_UP_BUTTON,			/* 09:XY ↑				*/
		TRUE,	IDC_STAGE_DOWN_BUTTON,			/* 10:XY ↓				*/
		TRUE,	IDC_STAGE_RIGHT_BUTTON,			/* 11:XY →				*/
		TRUE,	IDC_STAGE_LEFT_BUTTON,			/* 12:XY ←				*/
		TRUE,	IDC_JOYSTICK1,					/* 13:ソフトジョイ		*/
		TRUE,	IDC_JOYSTICK2,					/* 14:ジョイスティック	*/
		TRUE,	IDOK,							/* 15:OK				*/
		TRUE,	-1,
	};
	bool l_bTrMasIdle = ((CChiefTransiMaster*) g_pcChiefView->m_pcChiefTransiMaster)->IsIdle();
	if (ls_bTrMasIdlePre != l_bTrMasIdle) {
		/* １ポイント測定のアイドル状態変化有り	*/
		ls_bTrMasIdlePre = l_bTrMasIdle;
		int i = 0;
		if (true == l_bTrMasIdle) {
			/* Idle になった．	*/
			this->m_bProgress1PMeas = FALSE;
			while (-1 != l_MvCtrls[i].l_bCtrlID) {
#if 0
				this->GetDlgItem(l_MvCtrls[i].l_bCtrlID)->EnableWindow(l_MvCtrls[i].l_bCtrlEna);
#else
				if (IDC_STAGE_UP_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					m_XY_UpButton.SetEnabled(l_MvCtrls[i].l_bCtrlEna);
				} else if (IDC_STAGE_DOWN_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					m_XY_DownButton.SetEnabled(l_MvCtrls[i].l_bCtrlEna);
				} else if (IDC_STAGE_RIGHT_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					m_XY_RightButton.SetEnabled(l_MvCtrls[i].l_bCtrlEna);
				} else if (IDC_STAGE_LEFT_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					m_XY_LeftButton.SetEnabled(l_MvCtrls[i].l_bCtrlEna);
				} else {
					this->GetDlgItem(l_MvCtrls[i].l_bCtrlID)->EnableWindow(l_MvCtrls[i].l_bCtrlEna);
				}
#endif
				i++;
			}
		} else {
			/* ビジーになった．	*/
			this->m_bProgress1PMeas = TRUE;
			while (-1 != l_MvCtrls[i].l_bCtrlID) {
#if 0
				l_MvCtrls[i].l_bCtrlEna = this->GetDlgItem(l_MvCtrls[i].l_bCtrlID)->IsWindowEnabled();
				this->GetDlgItem(l_MvCtrls[i].l_bCtrlID)->EnableWindow(FALSE);
#else
				if (IDC_STAGE_UP_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					l_MvCtrls[i].l_bCtrlEna = m_XY_UpButton.GetEnabled();
					m_XY_UpButton.SetEnabled(FALSE);
				} else if (IDC_STAGE_DOWN_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					l_MvCtrls[i].l_bCtrlEna = m_XY_DownButton.GetEnabled();
					m_XY_DownButton.SetEnabled(FALSE);
				} else if (IDC_STAGE_RIGHT_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					l_MvCtrls[i].l_bCtrlEna = m_XY_RightButton.GetEnabled();
					m_XY_RightButton.SetEnabled(FALSE);
				} else if (IDC_STAGE_LEFT_BUTTON == l_MvCtrls[i].l_bCtrlID) {
					l_MvCtrls[i].l_bCtrlEna = m_XY_LeftButton.GetEnabled();
					m_XY_LeftButton.SetEnabled(FALSE);
				} else {
					l_MvCtrls[i].l_bCtrlEna = this->GetDlgItem(l_MvCtrls[i].l_bCtrlID)->IsWindowEnabled();
					this->GetDlgItem(l_MvCtrls[i].l_bCtrlID)->EnableWindow(FALSE);
				}
#endif
				i++;
			}
		}
	} else {
		/* １ポイント測定のアイドル状態変化無し	*/
		if (true == l_bTrMasIdle) {
			/* アイドル中	*/
			COverlayDialog::ButtonEnableCheck();
		}
	}
}
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
