// ReferenceMenuDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "ReferenceMenuDlg.h"
#include "RecipeMainListView.h"
#include "ExecutionDlg.h"
#include "NEXIOBASE.HXX"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ##########################################################################
// CReferenceMenuDlg ダイアログ
// ##########################################################################

// ==========================================================================
//
CReferenceMenuDlg::CReferenceMenuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReferenceMenuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReferenceMenuDlg)
	//}}AFX_DATA_INIT
}

// ==========================================================================
//
void CReferenceMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReferenceMenuDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RECIPE_NAME, m_stcRecipeName);
	DDX_Control(pDX, IDC_HEAD_TYPE, m_stcHeadType);
	DDX_Control(pDX, IDC_MEAS_ITEM, m_stcMeasurementItem);
	DDX_Control(pDX, IDC_GET_REFERENCE_BUTTON, m_GetReferenceButton);
	DDX_Control(pDX, IDC_CLEAR_REFERENCE_BUTTON, m_ClearReferenceButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CReferenceMenuDlg, CDialog)
	//{{AFX_MSG_MAP(CReferenceMenuDlg)
	ON_BN_CLICKED(IDC_CLEAR_REFERENCE_BUTTON, OnClearReferenceButton)
	ON_BN_CLICKED(IDC_GET_REFERENCE_BUTTON, OnGetReferenceButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ==========================================================================
// CReferenceMenuDlg メッセージ ハンドラ



// ==========================================================================
//
BOOL CReferenceMenuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	///// Static Control /////
	m_stcRecipeName.SetBkColor(WATER_COLOR);
	m_stcHeadType.SetBkColor(WATER_COLOR);
	m_stcMeasurementItem.SetBkColor(WATER_COLOR);
	m_GetReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_GetReferenceButton.DrawFlatFocus(TRUE);
	m_ClearReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ClearReferenceButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// 選択中のメインレシピ名を表示 /////
	char szRecipeName[RECIPE_NAME_LEN + 1];
	memset(szRecipeName, 0, sizeof(szRecipeName));
	((CRecipeMainListView *)/*pMainFrame*/((CMainFrame*)AfxGetMainWnd())->m_wndSplitter2.GetPane(0,0))->RecipeStart(szRecipeName);
	SetDlgItemText(IDC_RECIPE_NAME, szRecipeName);

	///// 選択中のHead＆MeasItemを表示 /////
	char szHead[255+1];
	char szScan[255+1];
	memset(szHead, 0, sizeof(szHead));
	memset(szScan, 0, sizeof(szScan));

	((CRecipeMainListView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitter2.GetPane(0,0))->RecipeHead(szHead, szScan);
	SetDlgItemText(IDC_HEAD_TYPE, szHead);
	SetDlgItemText(IDC_MEAS_ITEM, szScan);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
//
void CReferenceMenuDlg::OnClearReferenceButton()
{
	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	// Kojika 20090529 Add
	//if(MessageBox("リファレンスをクリアしますか？", "REFERENCE", MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
	LoadStringML(IDS_CLEAR_REFERENCE, l_strBuffer, "Do you want to clear Reference ?");
	LoadStringML(IDS_TITLE_REFERENCE, l_strTitle, "REFERENCE");
	if(MessageBox(l_strBuffer, l_strTitle, MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
	// Kojika 20090529 Add End

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
	// ▲インターロック条件▲


	ClearReference();
}

// ==========================================================================
//
void CReferenceMenuDlg::ClearReference()
{
	char szFilePath[MAX_PATH];
	CString sRecipeName;
	char szRecipeName[RECIPE_NAME_LEN + 1];

	memset(szFilePath, 0, sizeof(szFilePath));
	memset(szRecipeName, 0, sizeof(szRecipeName));

	GetDlgItemText(IDC_RECIPE_NAME, sRecipeName);

	strcpy(szRecipeName, sRecipeName);

	sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, szRecipeName, DAT_EXT);

	GetFileAttributes(szFilePath);

	DeleteFile(szFilePath);
}

// ==========================================================================
//
void CReferenceMenuDlg::OnGetReferenceButton()
{
	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	CString sRecipeName = "";
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	//char* pszCaption = "REFERENCE";
	//char* pszCaption = "リファレンス";
	static char szRecipeName[MAX_PATH];
	//Saiki 20090528 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090528 Add <-----


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
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //メンテナンス・スイッチ・オン
	}

	// 各I/Oチェック
	/*Saiki 20090520 Change ----->*/
	if(pDoc->IsInterLock() == TRUE){return;}
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	if( ((CNanoSpecDoc*) ((CMainFrame*) AfxGetMainWnd())->m_pDoc)->Rap_IsAirPressureLowON(1)!= OFF) {AlarmIf_Set(ALID_AirPressureDown); return;}	// エア圧力低下
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // ステージエラー
	if(pDoc->CheckUnitStatus() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/

	//ActuateFlagチェック
	//Saiki 20090531 Change ----->
	if(pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strBuffer, "Stage is moving.");
		pDoc->MessageStringIf_Set(strBuffer);
		return;
	} //ステージが動作中です
	//Saiki 20090531 Change <-----
	/*//Saiki 20090520 Change ----->*/
	if(pDoc->CheckActiveFlag() == TRUE){return;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲


	//if(MessageBox("Do you want to get Reference ?", "REFERENCE", MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
	//Saiki 20090528 Change ----->
	//if(MessageBox("リファレンスを取得しますか？", "REFERENCE", MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
	LoadStringML(IDS_GET_REFERENCE, strBuffer, "Do you want to get Reference ?");
	LoadStringML(IDS_TITLE_REFERENCE, strTitle, "REFERENCE");
	if(MessageBox(strBuffer, strTitle, MB_OKCANCEL|MB_ICONQUESTION) != IDOK) return;
	//Saiki 20090528 Change <-----

	pDoc->OperationLogging("Measurement Operation - REFERENCE BUTTON was pushed.");

	///// Clear Reference /////
	ClearReference();

	///// Get Recipe Name /////
	GetDlgItemText(IDC_RECIPE_NAME, sRecipeName);

	if(sRecipeName == ""){
		//Saiki 20090528 Change ----->
		//MessageBox("メインレシピが選択されていません", pszCaption, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_NOT_SELECT_MAIN_RECIPE, strBuffer, "Have not Selected a Recipe");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);
		//Saiki 20090528 Change <-----
		return;
	}

	memset(szRecipeName, 0, sizeof(szRecipeName));
	strcpy(szRecipeName, sRecipeName.GetBuffer(0));

	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementStartFailed);
		return;
	}


	//レンズ設定
	MAIN_RCP_INFO l_MainRcpInfo;
	MEAS_PROG_INFO l_Meas;
	memset(&l_MainRcpInfo, 0, sizeof(l_MainRcpInfo));
	memset(&l_Meas, 0, sizeof(l_Meas));
	RecipeFile_LoadRecipe(&l_MainRcpInfo, szRecipeName, RECIPE_FILE_MAIN_RECIPE); //
	RecipeFile_LoadRecipe(&l_Meas, l_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);


	pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	UINT uiLens = l_Meas.ScanParams._SR.iLens;
	StageSelectLensNotify(uiLens);
	pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)szRecipeName, 0);

	///// チーフの結果 /////
	//Saiki 20090528 Change ----->
	//bResult = pMainFrame->JudgeChiefResult(ChiefRet, pszCaption);
	bResult = pMainFrame->JudgeChiefResult(ChiefRet, strTitle);
	//Saiki 20090528 Change <-----

	if(bResult == FALSE){
		pDoc->OperationLogging("Measurement Operation - REFERENCE BUTTON - ABORT : Chief Result is Cancel.");
		return;
	}

	pDoc->OperationLogging("Measurement Operation - REFERENCE BUTTON - Reference was initiated.");

	pDoc->SetProcessStatus(PROCESS_PROC);
//Saiki 20130123 add --->
	pDoc->SetMeasMode(MEASMODE_REFERENCE); //リファレンス測定モード
//Saiki 20130123 add <---
	CExecutionDlg dlg;
	//Saiki 20090128 Change ----->
//	dlg.SetCaption(EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	dlg.SetCaption(strBuffer);
	//Saiki 20090128 Change <-----
	//dlg.SetMessage("Reference Executing...");
	//Saiki 20090528 Change ----->
	//dlg.SetMessage("リファレンス 測定中...");
	LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Executing...");
	dlg.SetMessage(strBuffer);
	//Saiki 20090528 Change <-----
	dlg.DoModal(); //CMainFrame::OnSrrefEnd()から消去する。
}

