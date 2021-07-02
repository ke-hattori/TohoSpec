// RecipePointDeskewProgramView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RecipePointDeskewProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CRecipePointDeskewProgramView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CRecipePointDeskewProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipePointDeskewProgramView::CRecipePointDeskewProgramView()
	: CNanoRecipeUI(CRecipePointDeskewProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipePointDeskewProgramView)
	//}}AFX_DATA_INIT

// Kojika 20090527 Change
//	m_strCaption = "POINT DESKEW PROGRAM";
	//Saiki 20090410 Change ----->
	//m_strCaption = "ポイント 傾き補正 プログラム";
	//m_strCaption = "ポイント デスキュー プログラム";
	//Saiki 20090410 Change <-----
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_POINT_DESKEW, l_strBuffer, "POINT DESKEW PROGRAM");
	m_strCaption = l_strBuffer;
// Kojika 20090527 Change End
	::ZeroMemory(&m_PointDeskewProgInfo, sizeof(m_PointDeskewProgInfo));
	m_bDeskewDefined = FALSE;
}

// =========================================================================
//
CRecipePointDeskewProgramView::~CRecipePointDeskewProgramView()
{
}

// =========================================================================
//
void CRecipePointDeskewProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipePointDeskewProgramView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AUTO_DESKEW_SETUP_BUTTON, m_AutoDeskewSetupButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CRecipePointDeskewProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipePointDeskewProgramView)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_AUTO_DESKEW_SETUP_BUTTON, OnAutoDeskewSetupButton)
	ON_CBN_SELCHANGE(IDC_DESKEW_EXECUTION_MODE, OnSelchangeDeskewExecutionMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CRecipePointDeskewProgramView 診断

#ifdef _DEBUG
// =========================================================================
//
void CRecipePointDeskewProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

// =========================================================================
//
void CRecipePointDeskewProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CRecipePointDeskewProgramView メッセージ ハンドラ

// =========================================================================
//
void CRecipePointDeskewProgramView::OnInitialUpdate()
{
	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	///// Load File /////
	LoadRecipeData();

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// 作成に失敗
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Auto Deskew Setup Button /////
	m_AutoDeskewSetupButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AutoDeskewSetupButton.DrawFlatFocus(TRUE);

	///// Control Initialize /////
	///// Deskew Execution Mode //////
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_EXECUTION_MODE);
	pCombo->ResetContent();

	for ( int i = 1; i < DESKEW_MODE_MAX; i++ ){	 //It excepts "No Deskew"
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszDeskewMode[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszDeskewMode_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszDeskewMode_JPN[i]);
		}
		// Kojika 20090528 Change End
	}

	///// Deskew Fail Option /////
	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_FAIL_OPTION);
	pCombo->ResetContent();
	for ( i = 0; i < DESKEW_FAIL_OPTION_MAX; i++ ){
		// Kojika 20090528 Change
		//pCombo->AddString(g_lpszDeskewFailOption[i]);
		if(l_SystemConfig.nLanguage == 0){
			pCombo->AddString(g_lpszDeskewFailOption_ENU[i]);
		}
		else{
			pCombo->AddString(g_lpszDeskewFailOption_JPN[i]);
		}
		// Kojika 20090528 Change End
	}

	// コメント文字数制限
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// Set Data /////
	SetData();
	OnSelchangeDeskewExecutionMode();
}

// =========================================================================
//
HBRUSH CRecipePointDeskewProgramView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CNanoRecipeUI::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
		///// Name /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POINT_DESKEW_PROGRAM_NAME)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_PointDeskewProgramName, WATER_COLOR);
			return m_Brush_PointDeskewProgramName;
		}

		///// Auto Deskew X /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_AUTO_DESKEW_X)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_AutoDeskew_X, REFERENCE_COLOR);
			return m_Brush_AutoDeskew_X;
		}

		///// Auto Deskew Y /////
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_AUTO_DESKEW_Y)->GetSafeHwnd()){
			ControlColor(pDC, &m_Brush_AutoDeskew_Y, REFERENCE_COLOR);
			return m_Brush_AutoDeskew_Y;
		}
		break;
	}

	return hbr;
}

// =========================================================================
//
void CRecipePointDeskewProgramView::ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color)
{
	pBrush->DeleteObject();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(Color);
	pBrush->CreateSolidBrush(Color);
}

// =========================================================================
//
void CRecipePointDeskewProgramView::OnSelchangeDeskewExecutionMode()
{
	CComboBox* pCombo;

	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_EXECUTION_MODE);
	switch ( pCombo->GetCurSel() + 1 ) {
	case DESKEW_MODE_MANUAL_DESKEW:
		GetDlgItem(IDC_AUTO_DESKEW_SETUP_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DESKEW_FAIL_OPTION)->EnableWindow(FALSE);
		break;
	case DESKEW_MODE_PATTERN_DESKEW:
		GetDlgItem(IDC_AUTO_DESKEW_SETUP_BUTTON)->EnableWindow();
		GetDlgItem(IDC_DESKEW_FAIL_OPTION)->EnableWindow();
		break;
	}
}

// =========================================================================
//
void CRecipePointDeskewProgramView::OnAutoDeskewSetupButton()
{
/*
	char szBuff[256];
	CPatRecDeskewDlg dlgPatRecDeskew(&m_pStageProgInfoHdr->DeskewSite[0]);
	// X
	GetDlgItemText(IDC_AUTO_DESKEW_X, szBuff, sizeof(szBuff));
	dlg.m_lX = atol(szBuff);
	// Y
	GetDlgItemText(IDC_AUTO_DESKEW_Y, szBuff, sizeof(szBuff));
	dlg.m_lY = atol(szBuff);

	if ( dlg.DoModal() == IDOK ) {
		// X
		sprintf(szBuff, "%ld", dlg.m_lX);
		SetDlgItemText(IDC_AUTO_DESKEW_X, szBuff);
		// Y
		sprintf(szBuff, "%ld", dlg.m_lY);
		SetDlgItemText(IDC_AUTO_DESKEW_Y, szBuff);

		m_bDeskewDefined = TRUE;
	}
*/
}

// =========================================================================
//
void CRecipePointDeskewProgramView::LoadRecipeData()
{
// Kojika 20090529 Add
	CString l_strBuffer;
// Kojika 20090529 Add End

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// 新規作成の場合
	if ( m_nOpenMode == modeNew ) {
		m_PointDeskewProgInfo.wHeadType = (WORD)pMainFrame->GetSelectHeadType();
		m_PointDeskewProgInfo.nDeskewMode = DESKEW_MODE_MANUAL_DESKEW;
	}
	else {
		// ロード失敗時には新規作成とみなす
		if ( !RecipeFile_LoadRecipe(&m_PointDeskewProgInfo, m_szRecipeName, RECIPE_FILE_POINT_DESKEW_PROGRAM) ) {
			m_nOpenMode = modeNew;
			m_PointDeskewProgInfo.wHeadType = (WORD)pMainFrame->GetSelectHeadType();
			m_PointDeskewProgInfo.nDeskewMode = DESKEW_MODE_MANUAL_DESKEW;
		}
	}

	if ( m_PointDeskewProgInfo.nDeskewMode == DESKEW_MODE_NO_DESKEW ) { //If DeskewMode is 0(No Deskew), must change 1(Manual Deskew)
		m_PointDeskewProgInfo.nDeskewMode = DESKEW_MODE_MANUAL_DESKEW;
	}
	::CopyMemory(&m_OldPointDeskewProgInfo, &m_PointDeskewProgInfo, sizeof(POINT_DESKEW_PROG_INFO));

	m_bDeskewDefined = ( m_PointDeskewProgInfo.nDeskewMode == DESKEW_MODE_PATTERN_DESKEW );

	if ( m_nOpenMode == modeNew ) {
// Kojika 20090529 Change
//		strcpy(m_PointDeskewProgInfo.hdr.szName, "Untitled");
//		strcpy(m_PointDeskewProgInfo.hdr.szName, "タイトルなし");
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_PointDeskewProgInfo.hdr.szName, l_strBuffer);
// Kojika 20090529 Change End
	}
}

// =========================================================================
//
void CRecipePointDeskewProgramView::SetData()
{
	CComboBox* pCombo;
	CString strBuffer;

	///// Name /////
	SetDlgItemText(IDC_POINT_DESKEW_PROGRAM_NAME, m_PointDeskewProgInfo.hdr.szName);

	// Deskew Execution Mode
	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_EXECUTION_MODE);
	pCombo->SetCurSel(m_PointDeskewProgInfo.nDeskewMode - 1);
	// Deskew Fail Option
	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_FAIL_OPTION);
	pCombo->SetCurSel(m_PointDeskewProgInfo.nDeskewFailOption);

	// Auto Deskew Position X
	strBuffer.Format("%ld", m_PointDeskewProgInfo.DeskewSite.Loc.lX);
	SetDlgItemText(IDC_AUTO_DESKEW_X, strBuffer);
	// Auto Deskew Position Y
	strBuffer.Format("%ld", m_PointDeskewProgInfo.DeskewSite.Loc.lY);
	SetDlgItemText(IDC_AUTO_DESKEW_Y, strBuffer);

	///// Comment /////
	SetDlgItemText(IDC_COMMENT, m_PointDeskewProgInfo.hdr.szComment);
}

// =========================================================================
//
void CRecipePointDeskewProgramView::UpDate()
{
	CComboBox* pCombo;
	CString strBuffer;

	///// Name /////
	GetDlgItemText(IDC_POINT_DESKEW_PROGRAM_NAME, strBuffer);
	strBuffer.Insert(RECIPE_NAME_LEN, '\0');
	strcpy(m_PointDeskewProgInfo.hdr.szName, strBuffer);

	// Deskew Execution Mode
	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_EXECUTION_MODE);
	m_PointDeskewProgInfo.nDeskewMode = pCombo->GetCurSel() + 1;
	// Deskew Fail Option
	pCombo = (CComboBox *)GetDlgItem(IDC_DESKEW_FAIL_OPTION);
	m_PointDeskewProgInfo.nDeskewFailOption = pCombo->GetCurSel();

	// Auto Deskew Position X
	GetDlgItemText(IDC_AUTO_DESKEW_X, strBuffer);
	m_PointDeskewProgInfo.DeskewSite.Loc.lX = atol(strBuffer);

	// Auto Deskew Position Y
	GetDlgItemText(IDC_AUTO_DESKEW_Y, strBuffer);
	m_PointDeskewProgInfo.DeskewSite.Loc.lY = atol(strBuffer);

	///// Comment /////
	GetDlgItemText(IDC_COMMENT, m_PointDeskewProgInfo.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// =========================================================================
//
BOOL CRecipePointDeskewProgramView::IsDataChanged()
{
	UpDate();

	return memcmp(&m_OldPointDeskewProgInfo, &m_PointDeskewProgInfo, sizeof(POINT_DESKEW_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipePointDeskewProgramView::SaveRecipeData()
{
	// 新しいレシピ名を設定（Save, SaveAs兼用）
	strcpy(m_PointDeskewProgInfo.hdr.szName, m_szRecipeName);
	if ( !RecipeFile_SaveRecipe(&m_PointDeskewProgInfo, m_PointDeskewProgInfo.hdr.szName, RECIPE_FILE_POINT_DESKEW_PROGRAM) )
		return FALSE;

	SetDlgItemText(IDC_POINT_DESKEW_PROGRAM_NAME, m_PointDeskewProgInfo.hdr.szName);
	::CopyMemory(&m_OldPointDeskewProgInfo, &m_PointDeskewProgInfo, sizeof(POINT_DESKEW_PROG_INFO));

	m_bDeskewDefined = ( m_PointDeskewProgInfo.nDeskewMode == DESKEW_MODE_PATTERN_DESKEW );

	return TRUE;
}

// =========================================================================
//
BOOL CRecipePointDeskewProgramView::CheckData()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	UpDate();

	// それぞれのモードにおいてDeskewが設定されているかチェック
	if ( m_PointDeskewProgInfo.nDeskewMode == DESKEW_MODE_PATTERN_DESKEW && !m_bDeskewDefined ) {
// Kojika 20090527 Change
////	MessageBox("Please Teach Auto Deskew", m_strCaption, MB_OK | MB_ICONSTOP);
//		//Saiki 20090410 Change ----->
//		//MessageBox("自動傾き補正をティーチングして下さい", m_strCaption, MB_OK | MB_ICONSTOP);
//		MessageBox("オート デスキューをティーチングして下さい", m_strCaption, MB_OK | MB_ICONSTOP);
//		//Saiki 20090410 Change <-----
		LoadStringML(IDS_TEACH_AUTO_DESKEW, l_strBuffer, "Please Teach Auto Deskew");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
		return FALSE;
	}

	return TRUE;
}

// =========================================================================
//
BOOL CRecipePointDeskewProgramView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_PROGRAM);
}

// =========================================================================
//
int CRecipePointDeskewProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_POINT_DESKEW_PROGRAM;
}
