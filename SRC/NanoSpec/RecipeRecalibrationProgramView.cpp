// RecipeRecalibrationProgramView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RecipeRecalibrationProgramView.h"
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int EditActual_IDs[] = {
	IDC_EDIT_ACTUAL1,
	IDC_EDIT_ACTUAL2,
	IDC_EDIT_ACTUAL3,
	IDC_EDIT_ACTUAL4,
	IDC_EDIT_ACTUAL5,
};

int EditMeasured_IDs[] = {
	IDC_EDIT_MEASURED1,
	IDC_EDIT_MEASURED2,
	IDC_EDIT_MEASURED3,
	IDC_EDIT_MEASURED4,
	IDC_EDIT_MEASURED5,
};

// #########################################################################
// CRecipeRecalibrationProgramView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CRecipeRecalibrationProgramView, CNanoRecipeUI)

// =========================================================================
//
CRecipeRecalibrationProgramView::CRecipeRecalibrationProgramView()
	: CNanoRecipeUI(CRecipeRecalibrationProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeRecalibrationProgramView)
	//}}AFX_DATA_INIT

// Kojika 20090527 Change
	//m_strCaption = "RECALIBRATION PROGRAM";
	//m_strCaption = "���L�����u���[�V���� �v���O����";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_RECALIBRATION_PROG, l_strBuffer, "RECALIBRATION PROGRAM");
	m_strCaption = l_strBuffer;
// Kojika 20090527 Change End
	::ZeroMemory(&m_RecalibProgInfo, sizeof(m_RecalibProgInfo));

	m_ValidNum = 0;
}

// =========================================================================
//
CRecipeRecalibrationProgramView::~CRecipeRecalibrationProgramView()
{
}

// =========================================================================
//
void CRecipeRecalibrationProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeRecalibrationProgramView)
	DDX_Control(pDX, IDC_RECALIB_GRAPH, m_RecalibGraph);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RECALIBRATION_GRID, m_RecalibrationGrid);
	DDX_Control(pDX, IDC_DRAW_RECALIB_GRAPH_BUTTON, m_DrawGraphButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeRecalibrationProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeRecalibrationProgramView)
	ON_BN_CLICKED(IDC_DRAW_RECALIB_GRAPH_BUTTON, OnDrawRecalibGraphButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CRecipeRecalibrationProgramView �f�f

#ifdef _DEBUG
// =========================================================================
//
void CRecipeRecalibrationProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

// =========================================================================
//
void CRecipeRecalibrationProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CRecipeRecalibrationProgramView ���b�Z�[�W �n���h��

// =========================================================================
//
void CRecipeRecalibrationProgramView::OnInitialUpdate()
{
	///// Load File /////
	LoadRecipeData();

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// �쐬�Ɏ��s
	}

	GetClientRect(&rect);
	m_wndToolBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Static Control /////
	m_stcRecalibrationProgramName.SubclassWindow(GetDlgItem(IDC_RECALIBRATION_PROGRAM_NAME)->GetSafeHwnd());
	m_stcRecalibrationProgramName.SetBkColor(WATER_COLOR);

	///// Recalibration Grid /////
	Graph_Init();
	RecalibrationGrid_Graph_Init();

	///// Control Initialize /////
	///// Deskew Execution Mode //////
	CComboBox* pCombo;
	pCombo = (CComboBox *)GetDlgItem(IDC_UNIT);
	pCombo->ResetContent();

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

//2009.09.02 bagus se SE�@�\�ǉ� --{--
//2009.09.01 bagus stress --{--
//	for ( int i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
//		// Kojika 20090528 Change
//		//pCombo->AddString(g_lpszRecalibrationProgramUnit[i]);
//		if(l_SystemConfig.nLanguage == 0){
//			pCombo->AddString(g_lpszRecalibrationProgramUnit_ENU[i]);
//		}else{
//			pCombo->AddString(g_lpszRecalibrationProgramUnit_JPN[i]);
//		}
//		// Kojika 20090528 Change End
//	}

//	if (m_RecalibProgInfo.wHeadType == HEAD_TYPE_STRESS) {
//		for ( int i = 0; i < STRESS_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
//			if(l_SystemConfig.nLanguage == 0){
//				pCombo->AddString(g_lpszStressRecalibrationProgramUnit_ENU[i]);
//			}else{
//				pCombo->AddString(g_lpszStressRecalibrationProgramUnit_JPN[i]);
//			}
//		}
//	} else {
//		for ( int i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
//			if(l_SystemConfig.nLanguage == 0){
//				pCombo->AddString(g_lpszRecalibrationProgramUnit_ENU[i]);
//			}else{
//				pCombo->AddString(g_lpszRecalibrationProgramUnit_JPN[i]);
//			}
//		}
//	}
//2009.09.01 bagus stress --}--
	int i;

	switch (m_RecalibProgInfo.wHeadType) {
	case HEAD_TYPE_SR:
// 2009.10.13 bagus Gantry �C�� --{--
#if 0
		for ( i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszRecalibrationProgramUnit_ENU[i]);
			}else{
				pCombo->AddString(g_lpszRecalibrationProgramUnit_JPN[i]);
			}
		}
#else
		switch (m_RecalibProgInfo.wScanType) {
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			for ( i = 0; i < GANTRY_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
				if(l_SystemConfig.nLanguage == 0){
					pCombo->AddString(g_lpszGantryRecalibrationProgramUnit_ENU[i]);
				}else{
					pCombo->AddString(g_lpszGantryRecalibrationProgramUnit_JPN[i]);
				}
			}
			break;
		default:
			for ( i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
				if(l_SystemConfig.nLanguage == 0){
					pCombo->AddString(g_lpszRecalibrationProgramUnit_ENU[i]);
				}else{
					pCombo->AddString(g_lpszRecalibrationProgramUnit_JPN[i]);
				}
			}
			break;
		}
#endif
// 2009.10.13 bagus Gantry �C�� --}--
		break;
	case HEAD_TYPE_STRESS:
		for ( i = 0; i < STRESS_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszStressRecalibrationProgramUnit_ENU[i]);
			}else{
				pCombo->AddString(g_lpszStressRecalibrationProgramUnit_JPN[i]);
			}
		}
		break;
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		for ( i = 0; i < SE_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszSeRecalibrationProgramUnit_ENU[i]);
			}else{
				pCombo->AddString(g_lpszSeRecalibrationProgramUnit_JPN[i]);
			}
		}
		break;
// 2009.10.08 bagus CTA �ǉ� --{--
	case HEAD_TYPE_CTA:
		for ( i = 0; i < CTA_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ){
			if(l_SystemConfig.nLanguage == 0){
				pCombo->AddString(g_lpszCTARecalibrationProgramUnit_ENU[i]);
			}else{
				pCombo->AddString(g_lpszCTARecalibrationProgramUnit_JPN[i]);
			}
		}
		break;
// 2009.10.08 bagus CTA �ǉ� --}--
	default:
		break;
	}
//2009.09.02 bagus se SE�@�\�ǉ� --}--

// 2009.10.13 bagus Gantry �C�� --{--
#if 0
// 2009.10.02 bagus Gantry --{--
	switch (m_RecalibProgInfo.wScanType) {
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
		pCombo->SetCurSel(3);		// %���w��
		pCombo->EnableWindow(FALSE);
		break;
	default:
		break;
	}
// 2009.10.02 bagus Gantry --}--
#endif
// 2009.10.13 bagus Gantry �C�� --}--

	/// �O���t�`�� Button ///
	m_DrawGraphButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,
		BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DrawGraphButton.DrawFlatFocus(TRUE);


	// �R�����g����������
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	///// SetData /////
	SetData();

	//2009.09.01 bagus stress  --{--
	//2009.12.07 bagus �C�� --{--
	//�V�K�̎��ɂ͂Ȃɂ����Ȃ�
	if ( m_nOpenMode != modeNew ) {
		OnDrawRecalibGraphButton();
	}
	//2009.12.07 bagus �C�� --}--
	//2009.09.01 bagus stress  --}--

/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectComment;	this->GetDlgItem(IDC_COMMENT)->GetWindowRect(&l_rectComment);
	RECT l_rectThisDlg = {0, 0, 605, 551};
	((CDialog*) this)->MapDialogRect(&l_rectThisDlg);
	long l_lViewBottom = l_rectThisView.top + (l_rectThisDlg.bottom - l_rectThisDlg.top + 1);
	long l_lSpaceCommentBottom = l_lViewBottom - l_rectComment.bottom;
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom <= (l_rectComment.bottom + l_lSpaceCommentBottom))) {
		long l_ldeltaY = l_rectThisView.bottom - l_lSpaceCommentBottom - (l_rectComment.bottom - l_rectComment.top);
		l_ldeltaY = l_rectComment.top - l_ldeltaY;
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		POINT l_ptStart = {0, 0};
		/* �R�����g����	*/
		RECT l_rectCommentStatic;	this->GetDlgItem(IDC_COMMENT_STATIC)->GetWindowRect(&l_rectCommentStatic);
		l_ptStart.x = l_rectCommentStatic.left;
		l_ptStart.y = l_rectCommentStatic.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT_STATIC)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		/* �R�����g��	*/
		l_ptStart.x = l_rectComment.left;
		l_ptStart.y = l_rectComment.top - l_ldeltaY;
		this->ScreenToClient(&l_ptStart);
		this->GetDlgItem(IDC_COMMENT)->SetWindowPos(0, l_ptStart.x, l_ptStart.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */
}

// =========================================================================
//
BOOL CRecipeRecalibrationProgramView::RecalibrationGrid_Graph_Init()
{
// Kojika 20090527 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// Kojika 20090527 Add End

	///// Grid Initialize /////
	GV_ITEM Item;
	int Cols = 3;
	int Rows = 5 + 1;
	int FixRows = 1;
	int FixCols = 1;
	int row,col;
	DWORD dwTextStyle;

// Kojika 20090527 Change
//	char* ColItem[]={
//		//"No",
//		//"Actual",
//		//"Measured",
		//"No",
//		"��l",
//		"�����l",
//	};
	char* ColItemJPN[]={
		"No",
		"��l",
		"�����l",
	};
	char* ColItemENU[]={
		"No",
		"Actual",
		"Measured",
	};
// Kojika 20090527 Change End

	///// Grid Initialize /////
	m_RecalibrationGrid.SetListMode(FALSE);
	m_RecalibrationGrid.EnableDragAndDrop(FALSE);
	m_RecalibrationGrid.SetTextBkColor(WHITE_COLOR);
	m_RecalibrationGrid.SetRowResize(0);
	m_RecalibrationGrid.SetEditable(TRUE);				// <--- When Edit is TRUE
	m_RecalibrationGrid.EnableSelection(TRUE);			// <--- When Edit is TRUE
	m_RecalibrationGrid.SetTrackFocusCell(FALSE);		// <--- When Edit is TRUE
	m_RecalibrationGrid.SetFrameFocusCell(FALSE);		// <--- When Edit is TRUE

	TRY {
		m_RecalibrationGrid.SetRowCount(Rows);
		m_RecalibrationGrid.SetColumnCount(Cols);
		m_RecalibrationGrid.SetFixedRowCount(FixRows);
		m_RecalibrationGrid.SetFixedColumnCount(FixCols);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	END_CATCH

	// Make cell 1,1 read-only
	//m_RecalibrationGrid.SetItemState(1,1, m_RecalibrationGrid.GetItemState(1,1) | GVIS_READONLY);

	//BOOL TitleTips = TRUE;
	//m_RecalibrationGrid.EnableTitleTips(TitleTips);

	///// Grid Size Initialize /////
	//Saiki 20090129 Change ----->
//	  m_RecalibrationGrid.SetRowHeight(0, 18);
	m_RecalibrationGrid.SetRowHeight(0, 20);
	//Saiki 20090129 Change <-----
	m_RecalibrationGrid.SetColumnWidth(0, 40);

	for (row = 1; row < m_RecalibrationGrid.GetRowCount(); row++){
		m_RecalibrationGrid.SetRowHeight(row, 18);
	}

	m_RecalibrationGrid.SetColumnWidth(1, 70);
	m_RecalibrationGrid.SetColumnWidth(2, 70);

	///// Font Initialize /////
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);

	for (row = 0; row < m_RecalibrationGrid.GetRowCount(); row++){
		for (col = 0; col < m_RecalibrationGrid.GetColumnCount(); col++){
			m_RecalibrationGrid.SetItemFont(row,col,&LogFont);
		}
	}

	///// Title Column /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_RecalibrationGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
// Kojika 20090527 Change
		//Item.strText.Format(_T("%s"),ColItem[col]);
		if(l_SystemConfig.nLanguage == 0){
			Item.strText.Format(_T("%s"),ColItemENU[col]);
		}
		else{
			Item.strText.Format(_T("%s"),ColItemJPN[col]);
		}
// Kojika 20090527 Change End
		m_RecalibrationGrid.SetItem(&Item);
	}

	///// Title Row /////
	//dwTextStyle = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_RecalibrationGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format(_T("%d"), row);
		m_RecalibrationGrid.SetItem(&Item);
	}

	///// �O���t�f�[�^�N���A /////
	m_RecalibGraph.ClearGraphData();

	///// Actual /////
	dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_RecalibrationGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 1;
		Item.nFormat = dwTextStyle;

		//2009.09.01 bagus stress --{--
		//�X�g���X��0�ȉ������e����
		BOOL bStop = FALSE;
		if(m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS){
			//Actual Data
			//if(m_RecalibProgInfo.RecalibEntry[row - 1].dActual <= 0)
			//	return FALSE;
			//2009.12.07 bagus STRESS --{--
			if(m_nOpenMode != modeNew){
				if(m_RecalibProgInfo.RecalibEntry[row - 1].dActual > 0){
					Item.strText.Format(_T("%.3lf"), m_RecalibProgInfo.RecalibEntry[row - 1].dActual);
				} else {
					Item.strText.Format("%s", "");
				}
			}else{
				Item.strText.Format("%s", "");
			}
			//2009.12.07 bagus STRESS --}--
			///// �O���b�h�f�[�^�Z�b�g /////
			m_RecalibrationGrid.SetItem(&Item);

			//Measured Data
			Item.col = 2;
			//if(m_RecalibProgInfo.RecalibEntry[row - 1].dMeasured <= 0)
			//	return FALSE;

			if(m_nOpenMode != modeNew){
				if(m_RecalibProgInfo.RecalibEntry[row - 1].dMeasured > 0){
					Item.strText.Format(_T("%.3lf"), m_RecalibProgInfo.RecalibEntry[row - 1].dMeasured );
				} else {
					Item.strText.Format("%s", "");
				}
			}else{
				Item.strText.Format("%s", "");
			}
			///// �O���b�h�f�[�^�Z�b�g /////
			m_RecalibrationGrid.SetItem(&Item);
		}else{
			if(row > 1){
				if(m_RecalibProgInfo.RecalibEntry[row - 2].dActual >= m_RecalibProgInfo.RecalibEntry[row - 1].dActual
				|| m_RecalibProgInfo.RecalibEntry[row - 2].dMeasured >= m_RecalibProgInfo.RecalibEntry[row - 1].dMeasured){
					bStop = TRUE;
				}
			}
			if(bStop || m_nOpenMode == modeNew){
				Item.strText.Format("%s", "");
				m_RecalibrationGrid.SetItem(&Item);
				Item.col = 2;
				Item.strText.Format("%s", "");
				m_RecalibrationGrid.SetItem(&Item);
			}else{
				Item.strText.Format(_T("%.3lf"), m_RecalibProgInfo.RecalibEntry[row - 1].dActual);
				m_RecalibrationGrid.SetItem(&Item);
				Item.col = 2;
				Item.strText.Format(_T("%.3lf"), m_RecalibProgInfo.RecalibEntry[row - 1].dMeasured );
				m_RecalibrationGrid.SetItem(&Item);
			}
		}
		//2009.09.01 bagus stress --{--

		///// �O���t�f�[�^�Z�b�g /////
		//2009.09.01 bagus stress --{--
		//if(m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS){
		//if((m_RecalibProgInfo.RecalibEntry[row-1].dActual > 0) && (m_RecalibProgInfo.RecalibEntry[row-1].dMeasured > 0))
		//		m_RecalibGraph.AddLineData(0, m_RecalibProgInfo.RecalibEntry[row - 1].dActual, m_RecalibProgInfo.RecalibEntry[row - 1].dMeasured);
		//}else{
		//	OnDrawRecalibGraphButton();
		//}
		//2009.09.01 bagus stress --}--
	}

	return TRUE;
}

// =========================================================================
//
void CRecipeRecalibrationProgramView::LoadRecipeData()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// �V�K�쐬�̏ꍇ
	if ( m_nOpenMode == modeNew ) {
		m_RecalibProgInfo.wHeadType = (WORD)pMainFrame->GetSelectHeadType();
		m_RecalibProgInfo.wScanType = (WORD)pMainFrame->GetSelectMeasurementType();
	}
	else {
		// ���[�h���s���ɂ͐V�K�쐬�Ƃ݂Ȃ�
		if ( !RecipeFile_LoadRecipe(&m_RecalibProgInfo, m_szRecipeName, RECIPE_FILE_RECALIBRATION_PROGRAM) ) {
			m_nOpenMode = modeNew;
			m_RecalibProgInfo.wHeadType = (WORD)pMainFrame->GetSelectHeadType();
			m_RecalibProgInfo.wScanType = (WORD)pMainFrame->GetSelectMeasurementType();
		}
	}
	::CopyMemory(&m_OldRecalibProgInfo, &m_RecalibProgInfo, sizeof(RECALIB_PROG_INFO));

	if ( m_nOpenMode == modeNew ) {
// Kojika 20090527 Change
		//strcpy(m_RecalibProgInfo.hdr.szName, "Untitled");
		//strcpy(m_RecalibProgInfo.hdr.szName, "�^�C�g���Ȃ�");
		LoadStringML(IDS_UNTITLE, l_strBuffer, "Untitled");
		strcpy(m_RecalibProgInfo.hdr.szName, l_strBuffer);
// Kojika 20090527 Change End
	}
}

// =========================================================================
//
void CRecipeRecalibrationProgramView::SetData()
{
	CComboBox* pCombo;
	CString csBuff;

	///// Name /////
	SetDlgItemText(IDC_RECALIBRATION_PROGRAM_NAME, m_RecalibProgInfo.hdr.szName);

	// Recalibration Unit
	pCombo = (CComboBox *)GetDlgItem(IDC_UNIT);
// 2009.10.02 bagus Gantry --{--
#if 0
	//2009.09.01 bagus stress --{--
	if(m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS){
		pCombo->SetCurSel(m_RecalibProgInfo.wUnits);
	}else{
		//�X�g���XMPa�Œ�
		pCombo->SetCurSel(0);
	}
	//2009.09.01 bagus stress --}--
#else
	switch (m_RecalibProgInfo.wHeadType) {
	case HEAD_TYPE_SR:
		switch (m_RecalibProgInfo.wScanType) {
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.10.13 bagus Gantry �C�� --{--
//			pCombo->SetCurSel(3);
			pCombo->SetCurSel(0);
// 2009.10.13 bagus Gantry �C�� --}--
			break;
		default:
			pCombo->SetCurSel(m_RecalibProgInfo.wUnits);
			break;
		}
		break;
	case HEAD_TYPE_STRESS:
		pCombo->SetCurSel(0);
		break;
// 2009.10.08 bagus CTA �ǉ� --{--
	case HEAD_TYPE_CTA:
		pCombo->SetCurSel(0);
		break;
// 2009.10.08 bagus CTA �ǉ� --}--
	default:
		pCombo->SetCurSel(m_RecalibProgInfo.wUnits);
		break;
	}
#endif
// 2009.10.02 bagus Gantry --}--

	///// Comment /////
	SetDlgItemText(IDC_COMMENT, m_RecalibProgInfo.hdr.szComment);
}

// =========================================================================
//
void CRecipeRecalibrationProgramView::UpDate()
{
	CString strBuffer;
	CString csComment;

//2009.09.01 bagus stress --{--
	CString strBuffer1;
	CString strBuffer2;
//2009.09.01 bagus stress --}--

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	///// Name /////
	GetDlgItemText(IDC_RECALIBRATION_PROGRAM_NAME, strBuffer);
	strBuffer.Insert(RECIPE_NAME_LEN, '\0');
	strcpy(m_RecalibProgInfo.hdr.szName, strBuffer);

	// Unit
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_UNIT);
	pCombo->GetLBText(pCombo->GetCurSel(), strBuffer);
// 2009.10.22 bagus Stress �C�� --{--
#if 0
	for ( int i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
		// Kojika 20090528 Change
/*
		if ( strcmp(g_lpszRecalibrationProgramUnit[i], strBuffer) == 0 ) {
			m_RecalibProgInfo.wUnits = (WORD)i;
			break;
		}
*/
// 2009.09.19 bagus SE --{--
#if 0
		//2009.09.01 bagus stress --{--
		if(m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS){
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}else{
				if ( strcmp(g_lpszRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}
		}else{
			//�X�g���X��1�Œ�
			m_RecalibProgInfo.wUnits = (WORD)1;
		}
		//2009.09.01 bagus stress --}--
#else
		switch ( m_RecalibProgInfo.wHeadType ) {
		case HEAD_TYPE_STRESS:
			m_RecalibProgInfo.wUnits = (WORD)1;
			break;
// 2009.10.08 bagus CTA �ǉ� --{--
		case HEAD_TYPE_CTA:
			m_RecalibProgInfo.wUnits = (WORD)1;
			break;
// 2009.10.08 bagus CTA �ǉ� --}--
		case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszSeRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}else{
				if ( strcmp(g_lpszSeRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}
			break;
// 2009.10.13 bagus Gantry �C�� --{--
		case HEAD_TYPE_SR:
			switch (m_RecalibProgInfo.wScanType) {
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
				if(l_SystemConfig.nLanguage == 0){
					if ( strcmp(g_lpszGantryRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)1;
						break;
					}
				}else{
					if ( strcmp(g_lpszGantryRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)1;
						break;
					}
				}
				break;
			default:
				if(l_SystemConfig.nLanguage == 0){
					if ( strcmp(g_lpszRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)i;
						break;
					}
				}else{
					if ( strcmp(g_lpszRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)i;
						break;
					}
				}
				break;
			}
			break;
// 2009.10.13 bagus Gantry �C�� --}--
		default:
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}else{
				if ( strcmp(g_lpszRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}
			break;
		}
#endif
// 2009.09.19 bagus SE --}--
		// Kojika 20090528 Change End
	}
#else
	int	i;

	switch ( m_RecalibProgInfo.wHeadType ) {
	case HEAD_TYPE_STRESS:
		for ( i = 0; i < STRESS_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszStressRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)1;
					break;
				}
			}else{
				if ( strcmp(g_lpszStressRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)1;
					break;
				}
			}
		}
		break;
	case HEAD_TYPE_CTA:
		for ( i = 0; i < CTA_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszCTARecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)1;
					break;
				}
			}else{
				if ( strcmp(g_lpszCTARecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)1;
					break;
				}
			}
		}
		break;
	case HEAD_TYPE_SE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
		for ( i = 0; i < SE_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszSeRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}else{
				if ( strcmp(g_lpszSeRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}
		}
		break;
	case HEAD_TYPE_SR:
		switch (m_RecalibProgInfo.wScanType) {
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			for ( i = 0; i < GANTRY_RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
				if(l_SystemConfig.nLanguage == 0){
					if ( strcmp(g_lpszGantryRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)1;
						break;
					}
				}else{
					if ( strcmp(g_lpszGantryRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)1;
						break;
					}
				}
			}
			break;
		default:
			for ( i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
				if(l_SystemConfig.nLanguage == 0){
					if ( strcmp(g_lpszRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)i;
						break;
					}
				}else{
					if ( strcmp(g_lpszRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
						m_RecalibProgInfo.wUnits = (WORD)i;
						break;
					}
				}
			}
			break;
		}
		break;
	default:
		for ( i = 0; i < RECALIBRATION_PROGRAM_UNIT_MAX; i++ ) {
			if(l_SystemConfig.nLanguage == 0){
				if ( strcmp(g_lpszRecalibrationProgramUnit_ENU[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}else{
				if ( strcmp(g_lpszRecalibrationProgramUnit_JPN[i], strBuffer) == 0 ) {
					m_RecalibProgInfo.wUnits = (WORD)i;
					break;
				}
			}
		}
		break;
	}
#endif
// 2009.10.22 bagus Stress �C�� --}--

//2009.09.01 bagus stress --{--
	m_ValidNum = 0;
//2009.09.01 bagus stress --}--

	// �O���b�h�̒l���擾
	for ( int iRow = 1; iRow < m_RecalibrationGrid.GetRowCount(); iRow++ ) {
//2009.09.01 bagus stress --{--
//		// Actual
//		strBuffer = m_RecalibrationGrid.GetItemText(iRow, 1);
//		m_RecalibProgInfo.RecalibEntry[iRow - 1].dActual = atof(strBuffer);
//		// Measured
//		strBuffer = m_RecalibrationGrid.GetItemText(iRow, 2);
//		m_RecalibProgInfo.RecalibEntry[iRow - 1].dMeasured = atof(strBuffer);
		// Actual
		strBuffer1 = m_RecalibrationGrid.GetItemText(iRow, 1);
		m_RecalibProgInfo.RecalibEntry[iRow - 1].dActual = atof(strBuffer1);
		// Measured
		strBuffer2 = m_RecalibrationGrid.GetItemText(iRow, 2);
		m_RecalibProgInfo.RecalibEntry[iRow - 1].dMeasured = atof(strBuffer2);

		strBuffer1.TrimLeft();
		strBuffer1.TrimRight();
		strBuffer2.TrimLeft();
		strBuffer2.TrimRight();

		if ((!strBuffer1.IsEmpty()) && (!strBuffer2.IsEmpty())) {
			m_ValidNum++;
		}
//2009.09.01 bagus stress --}--
	}

// 2009.09.15 K.Matsuo �o�^���`�F�b�N -->
	m_RecalibProgInfo.iEntryNum = m_ValidNum;
// 2009.09.15 K.Matsuo �o�^���`�F�b�N <--

	///// Comment /////
	GetDlgItemText(IDC_COMMENT, m_RecalibProgInfo.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// =========================================================================
//
BOOL CRecipeRecalibrationProgramView::IsDataChanged()
{
	UpDate();

	return memcmp(&m_OldRecalibProgInfo, &m_RecalibProgInfo, sizeof(RECALIB_PROG_INFO)) != 0;
}

// =========================================================================
//
BOOL CRecipeRecalibrationProgramView::SaveRecipeData()
{
// 2009.10.13 bagus Gantry �C�� --{--
#if 0
// 2009.10.02 bagus Gantry --{--
	switch (m_RecalibProgInfo.wHeadType) {
	case HEAD_TYPE_SR:
		switch (m_RecalibProgInfo.wScanType) {
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			m_RecalibProgInfo.wUnits = 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
// 2009.10.02 bagus Gantry --}--
#endif
// 2009.10.13 bagus Gantry �C�� --}--

	// �V�������V�s����ݒ�iSave, SaveAs���p�j
	strcpy(m_RecalibProgInfo.hdr.szName, m_szRecipeName);
	if ( !RecipeFile_SaveRecipe(&m_RecalibProgInfo, m_RecalibProgInfo.hdr.szName, RECIPE_FILE_RECALIBRATION_PROGRAM) )
		return FALSE;

	SetDlgItemText(IDC_RECALIBRATION_PROGRAM_NAME, m_RecalibProgInfo.hdr.szName);
	::CopyMemory(&m_OldRecalibProgInfo, &m_RecalibProgInfo, sizeof(RECALIB_PROG_INFO));
	return TRUE;
}

// =========================================================================
//
BOOL CRecipeRecalibrationProgramView::CheckData()
{
// Kojika 20090527 Add
	CString l_strBuffer;
	CString l_strBuffer2;
// Kojika 20090527 Add End

	double dTemp;
	CString strBuffer;
	CString csCmpBuff;
	int iRow;

	UpDate();

	// �O���b�h�̒l���擾
	// Actual
	BOOL IsVacantCell=FALSE;
	for ( iRow = 1; iRow < m_RecalibrationGrid.GetRowCount(); iRow++ ) {
		strBuffer = m_RecalibrationGrid.GetItemText(iRow, 1);
		if ((IsVacantCell==TRUE) && (strBuffer != "")){
// Kojika 20090527 Change
			//MessageBox("There is the vacant cell during 1st actual value and last actual value.", m_strCaption, MB_OK | MB_ICONSTOP);
			//MessageBox("��P��l�ƍŏI��l�̊Ԃɓ��͂���Ă��Ȃ��Z��������܂�.", m_strCaption, MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_VACANT_CELL_DURING_ACTUAL, l_strBuffer, "There is the vacant cell during 1st actual value and last actual value.");
			MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
			return FALSE;
		}
		//2009.09.01 bagus stress --{--
		//if ( iRow == 1 && ( strBuffer == "" || atof(strBuffer) <= 0 ) ) {
		if ( iRow == 1 && ( strBuffer == "" || ((m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS) && (atof(strBuffer) <= 0))) ) {
		//2009.09.01 bagus stress --}--
// Kojika 20090527 Change
			//MessageBox("1st actual value is inaccurate.", m_strCaption, MB_OK | MB_ICONSTOP);
			//MessageBox("����l���s���ł�", m_strCaption, MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_FIRST_ACTUAL_INACCURATE, l_strBuffer, "1st actual value is inaccurate.");
			MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
			return FALSE;
		}
		if ( iRow != 1 ) {
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- { ---------- */
//			//2009.09.01 bagus stress --{--
//			//if ( dTemp > atof(strBuffer) || atof(strBuffer) <= 0 ) {
//			if ( dTemp > atof(strBuffer) || ((m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS) && (atof(strBuffer) <= 0 )) ) {
//			//2009.09.01 bagus stress --}--
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ----------              */
			if (
				((m_RecalibProgInfo.wHeadType == HEAD_TYPE_STRESS) && (dTemp > atof(strBuffer))) ||
				((m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS) && (atof(strBuffer) <= 0))
				) {
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- } ---------- */
				if ( strBuffer != "" ) {
// Kojika 20090527 Change
					//MessageBox("Entering value is not ascent.", m_strCaption, MB_OK | MB_ICONSTOP);
					//MessageBox("�X�����㏸�����ɂȂ�悤�ɓ��͂��Ă�������", m_strCaption, MB_OK | MB_ICONSTOP);
					LoadStringML(IDS_ENTER_VALUE_NOT_ASCENT, l_strBuffer, "Entering value is not ascent.");
					MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
					return FALSE;
				}
				else{
					IsVacantCell=TRUE;
				}
			}
			csCmpBuff = m_RecalibrationGrid.GetItemText(iRow, 2);
			if ( strBuffer == _TEXT("") && csCmpBuff != "") {
// Kojika 20090527 Change
				//MessageBox("There is a vacant cell.", m_strCaption, MB_OK | MB_ICONSTOP);
				//MessageBox("���͂���Ă��Ȃ��Z��������܂�", m_strCaption, MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_VACANT_CELL, l_strBuffer, "There is a vacant cell.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
				return FALSE;
			}

		}
		dTemp = atof(strBuffer);
	}

	// Measured
	IsVacantCell=FALSE;
	for ( iRow = 1; iRow < m_RecalibrationGrid.GetRowCount(); iRow++ ) {
		strBuffer = m_RecalibrationGrid.GetItemText(iRow, 2);
		if ((IsVacantCell==TRUE) && (strBuffer != "")){
// Kojika 20090527 Change
			//MessageBox("There is a vacant cell during 1st measured value and last measured value.", m_strCaption, MB_OK | MB_ICONSTOP);
			//MessageBox("��P�����l�ƍŏI�����l�̊Ԃɓ��͂���Ă��Ȃ��Z��������܂�", m_strCaption, MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_VACANT_CELL_DURING_MEASURED, l_strBuffer, "There is a vacant cell during 1st measured value and last measured value.");
			MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
			return FALSE;
		}

		//2009.09.01 bagus stress --{--
		//if ( iRow == 1 && ( strBuffer == "" || atof(strBuffer) <= 0 ) ) {
		if ( iRow == 1 && ( strBuffer == "" || ((m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS) && (atof(strBuffer) <= 0 ))) ) {
		//2009.09.01 bagus stress --}--
// Kojika 20090527 Change
			//MessageBox("1st measured value is inaccurate.", m_strCaption, MB_OK | MB_ICONSTOP);
			//MessageBox("�������l���s���ł�", m_strCaption, MB_OK | MB_ICONSTOP);
			LoadStringML(IDS_FIRST_MEASURED_INACCURATE, l_strBuffer, "1st measured value is inaccurate.");
			MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
			return FALSE;
		}
		if ( iRow != 1 ) {
			//2009.09.01 bagus stress --{--
			//if ( dTemp > atof(strBuffer) || atof(strBuffer) <= 0 ) {
			if ( dTemp > atof(strBuffer) || ((m_RecalibProgInfo.wHeadType != HEAD_TYPE_STRESS) && (atof(strBuffer) <= 0)) ) {
			//2009.09.01 bagus stress --}--
				if ( strBuffer != "" ) {
// Kojika 20090527 Change
					//MessageBox("Entering value is not ascent.", m_strCaption, MB_OK | MB_ICONSTOP);
					//MessageBox("�X�����㏸�����ɂȂ�悤�ɓ��͂��Ă�������", m_strCaption, MB_OK | MB_ICONSTOP);
					LoadStringML(IDS_ENTER_VALUE_NOT_ASCENT, l_strBuffer, "Entering value is not ascent.");
					MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
					return FALSE;
				}
				else{
					IsVacantCell=TRUE;
				}
			}
			csCmpBuff = m_RecalibrationGrid.GetItemText(iRow, 1);
			if ( strBuffer == "" && csCmpBuff != "" ) {
// Kojika 20090527 Change
				//MessageBox("There is a vacant cell.", m_strCaption, MB_OK | MB_ICONSTOP);
				//MessageBox("���͂���Ă��Ȃ��Z��������܂�", m_strCaption, MB_OK | MB_ICONSTOP);
				LoadStringML(IDS_VACANT_CELL, l_strBuffer, "There is a vacant cell.");
				MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
				return FALSE;
			}
		}
		dTemp = atof(strBuffer);
	}

// 2009.09.15 K.Matsuo �o�^���`�F�b�N -->
	int iMinCount;
	if (m_RecalibProgInfo.wHeadType == HEAD_TYPE_STRESS) {
		// �}�C�i�X�l���͋��e
		iMinCount = 2;
	}
	else {
		// �v���X�l���͂̂�
		iMinCount = 1;
	}

	if ( m_ValidNum < iMinCount ) {
		LoadStringML(IDS_RECALIB_DATA_COUNT, l_strBuffer, "Enter %d data or more.");
		l_strBuffer2.Format(l_strBuffer, iMinCount);
		MessageBox(l_strBuffer2, m_strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
// 2009.09.15 K.Matsuo �o�^���`�F�b�N <--

	return TRUE;
}

// =========================================================================
//
BOOL CRecipeRecalibrationProgramView::IsAccessPrivilege()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	///// User Access Privilege /////
	return pDoc->User_Access(ACCESS_PROGRAM);
}

// =========================================================================
//
int CRecipeRecalibrationProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_RECALIBRATION_PROGRAM;
}

void CRecipeRecalibrationProgramView::OnDrawRecalibGraphButton()
{
	if(!CheckData()) return;

	///// SetData /////
	SetData();

	Graph_Init();

	//// Graph Data All Clear ////
	m_RecalibGraph.ClearGraphData();

	//// Get Data And Set Graph Data ////
	// �O���b�h�̒l���擾
	//2009.09.01 bagus stress --{--
	//for ( int i = 0; i < 5; i++ ) {
	for ( int i = 0; i < m_ValidNum; i++ ) {
	//2009.09.01 bagus stress --}--
		//2009.09.01 bagus stress --{--
		//if((m_RecalibProgInfo.RecalibEntry[i].dActual> 0) && (m_RecalibProgInfo.RecalibEntry[i].dMeasured> 0))
		//	m_RecalibGraph.AddLineData(0, m_RecalibProgInfo.RecalibEntry[i].dActual, m_RecalibProgInfo.RecalibEntry[i].dMeasured);
		if (m_RecalibProgInfo.wHeadType == HEAD_TYPE_STRESS) {
			m_RecalibGraph.AddLineData(0, m_RecalibProgInfo.RecalibEntry[i].dActual, m_RecalibProgInfo.RecalibEntry[i].dMeasured);
		} else {
			if (i == 0) {
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- { ---------- */
//				m_RecalibGraph.AddLineData(0, 0, 0);
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ----------              */
				if (m_ValidNum < 2) {
					m_RecalibGraph.AddLineData(0, 0, 0);
				} else if (m_RecalibProgInfo.RecalibEntry[0].dActual > m_RecalibProgInfo.RecalibEntry[1].dActual) {
/* modified 2015.12.06 hmenjo �O���t�o�O�C�� ---------- { ---------- */
//					/* ��l�̃I�t�Z�b�g�P���Q�̕ω����O�����̏ꍇ��
//						��l�̃I�t�Z�b�g�O���I�t�Z�b�g�P�Ɠ����l�ɂ��܂�	*/
//					m_RecalibGraph.AddLineData(0, m_RecalibProgInfo.RecalibEntry[0].dActual, 0);
/* modified 2015.12.06 hmenjo �O���t�o�O�C�� ----------              */
					/* ��l�̃I�t�Z�b�g�P���Q�̕ω����O�����̏ꍇ��
						��l�̃I�t�Z�b�g�O���I�t�Z�b�g�P���Q�̌X���ŋ��߂܂��D	*/
					double l_dX1 = m_RecalibProgInfo.RecalibEntry[0].dMeasured;
					double l_dX2 = m_RecalibProgInfo.RecalibEntry[1].dMeasured;
					double l_dY1 = m_RecalibProgInfo.RecalibEntry[0].dActual;
					double l_dY2 = m_RecalibProgInfo.RecalibEntry[1].dActual;
					double l_dDx = l_dX2 - l_dX1;
					double l_dDy = l_dY2 - l_dY1;
					double l_dYY = l_dY1 + (0.0 - l_dX1) * l_dDy / l_dDx;
					if (DBL_MAX < l_dYY) {
						l_dYY = DBL_MAX;
					}
					m_RecalibGraph.AddLineData(0, l_dYY, 0);
/* modified 2015.12.06 hmenjo �O���t�o�O�C�� ---------- } ---------- */
				} else {
					m_RecalibGraph.AddLineData(0, 0, 0);
				}
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- } ---------- */
			}
			if((m_RecalibProgInfo.RecalibEntry[i].dActual> 0) && (m_RecalibProgInfo.RecalibEntry[i].dMeasured> 0))
				m_RecalibGraph.AddLineData(0, m_RecalibProgInfo.RecalibEntry[i].dActual, m_RecalibProgInfo.RecalibEntry[i].dMeasured);
		}
		//2009.09.01 bagus stress --}--
	}

/*
		CString strBuffer;

		///// Set Wavelength at Cell /////
		strBuffer.Format("%ld", iRow);
		m_LightIntensityListGrid.SetItemText(iWave - m_iStartWaveLength + 1, 0, strBuffer);
		dwTextStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
		m_LightIntensityListGrid.SetItemFormat(iWave - m_iStartWaveLength + 1, 0, dwTextStyle);

		dwTextStyle = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;

		///// Set Measured Data Value at Cell /////
		dLength = MEAS_GetAnalysisData(iPointIndex, iWave);
		strBuffer.Format("%lf", dLength);
		m_LightIntensityListGrid.SetItemText(iWave - m_iStartWaveLength + 1, 1, strBuffer);
		m_LightIntensityListGrid.SetItemFormat(iWave - m_iStartWaveLength + 1, 1, dwTextStyle);
		m_RecalibGraph.AddLineData(0, (double)atof(strBuffer));

		///// Set XMP Gen Data Value at Cell /////
		dLength = MEAS_GetAnalysisGenData(iPointIndex, iWave);
		strBuffer.Format("%lf", dLength);
		m_LightIntensityListGrid.SetItemText(iWave - m_iStartWaveLength + 1, 2, strBuffer);
		m_LightIntensityListGrid.SetItemFormat(iWave - m_iStartWaveLength + 1, 2, dwTextStyle);
		m_RecalibGraph.AddLineData(1, (double)atof(strBuffer));
	}
	m_LightIntensityListGrid.Invalidate();
	*/
	m_RecalibGraph.Invalidate();

}

// =========================================================================
//
void CRecipeRecalibrationProgramView::Graph_Frame_Init( int dX_Min/*=0*/, int dX_Max/*=100*/, int dY_Min/*=0*/, int dY_Max/*=100*/, int iPointNum )
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	CNanoMeasureGraph::GraphConfig tGraphConfig;
	CNanoMeasureGraph::LineConfig tLineConfig;

	memset(&tGraphConfig,NULL,sizeof(tGraphConfig));
	tGraphConfig.UseLineSelect = TRUE;
// Kojika 20090527 Change
	//strcpy(tGraphConfig.UpperTitle,"RECALIBRATION GRAPH");
	//strcpy(tGraphConfig.UpperTitle,"���L�����u���[�V���� �O���t");
	LoadStringML(IDS_TITLE_RECALIB_GRAPH, l_strBuffer, "RECALIBRATION GRAPH");
	strcpy(tGraphConfig.UpperTitle, l_strBuffer);
// Kojika 20090527 Change End
	strcpy(tGraphConfig.LowerTitle,"");
	tGraphConfig.CanvasMargin = 7;	//�O���t�`��]��

///// �ݒ�͈͂̐ݒ� /////
// Kojika 20090527 Change
	//strcpy(tGraphConfig.Y_AxisTitle,"Measured Data");
	//strcpy(tGraphConfig.X_AxisTitle,"Actual Data");
	//strcpy(tGraphConfig.Y_AxisTitle,"�����l�f�[�^");
	//strcpy(tGraphConfig.X_AxisTitle,"��l�f�[�^");
	LoadStringML(IDS_TITLE_MEASURED_DATA, l_strBuffer, "Measured Data");
	strcpy(tGraphConfig.Y_AxisTitle, l_strBuffer);
	LoadStringML(IDS_TITLE_ACTUAL_DATA, l_strBuffer, "Actual Data");
	strcpy(tGraphConfig.X_AxisTitle, l_strBuffer);
// Kojika 20090527 Change End
	tGraphConfig.X_Max = dX_Max;
	tGraphConfig.Y_Max = dY_Max;
	tGraphConfig.X_Origin = dX_Min;
	tGraphConfig.Y_Origin = dY_Min;
	tGraphConfig.X_Step = (int)(dX_Max/5);
	tGraphConfig.Y_Step = (int)(dY_Max/5);

	//2009.09.01 bagus stress --{--
	//tGraphConfig.PointNum = iPointNum;
	tGraphConfig.PointNum = m_ValidNum;
	//2009.09.01 bagus stress --}--
	tGraphConfig.CanvasBackColor = RGB(0xFF,0xFF,0xFF);
	tGraphConfig.GraphBackColor = RGB(0xFF,0xFF,0xFF);//RGB(0x00,0xAA,0xAA);
	tGraphConfig.GraphBorderColor = RGB(0x00,0x00,0x00);
	tGraphConfig.GraphStepLineColor = RGB(0xAA,0xAA,0xAA);
	tGraphConfig.TitleTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.UnitTextColor = RGB(0x00,0x00,0x00);
	tGraphConfig.SelectingCalibrationColor= RGB(0xFF, 0x00, 0x00);
	tGraphConfig.SelectedCalibrationColor = RGB(0x00, 0x00, 0xFF);
	tGraphConfig.UseCalibration = FALSE;
	//tGraphConfig.CalibrationLineNum = 7;
	m_RecalibGraph.SetGraphConfig(tGraphConfig);

	////SetLine Condition ////
	for(int i = 0;i < NANO_MEASURE_MAX_GRAPH_LINE;i++){
		memset(&tLineConfig,NULL,sizeof(tLineConfig));

		if(i==0){
			tLineConfig.UseFlag = TRUE;
			tLineConfig.Visible = TRUE;
			//tLineConfig.DataIndex = i;
			tLineConfig.Color = RGB(255,0,0);
			// =PS_SOLID:����
			// PS_DASH:�j��
			// PS_DOT:�_��
			// PS_DASHDOT:�P�_����
			// PS_DASHDOTDOT:�Q�_����
			tLineConfig.PenStyle = PS_SOLID;
			//wsprintf(tLineConfig.LineName,"Measured Data ");

		}
		else{
			tLineConfig.UseFlag = FALSE;
			tLineConfig.Visible = FALSE;
		}

		m_RecalibGraph.SetLineConfig(i,tLineConfig);
		m_RecalibGraph.ClearGraphData();
	}
}


// =========================================================================
//
void CRecipeRecalibrationProgramView::Graph_Init()
{
	///// �O���t�f�[�^�N���A /////
	m_RecalibGraph.ClearGraphData();

	///// �ő�E�ŏ��l�Z�b�g /////
	//2009.09.01 bagus stress --{--
//	double dActDataMax=0;
//	double dMeaDataMax=0;
	double dActDataMax=DBL_MIN;
	double dMeaDataMax=DBL_MIN;
	double dActDataMin=DBL_MAX;
	double dMeaDataMin=DBL_MAX;
	//2009.09.01 bagus stress --}--
	int iPointNum=0;

	//2009.09.01 bagus stress --{--
//	for(int i = 0; i < 5; i++){
//		///// Actual /////
//		if(m_RecalibProgInfo.RecalibEntry[i].dActual > 0){
//			if ( dActDataMax < m_RecalibProgInfo.RecalibEntry[i].dActual )
//				dActDataMax = m_RecalibProgInfo.RecalibEntry[i].dActual;
//		}
//		///// Measured /////
//		if(m_RecalibProgInfo.RecalibEntry[i].dMeasured > 0){
//			if ( dMeaDataMax < m_RecalibProgInfo.RecalibEntry[i].dMeasured )
//				dMeaDataMax = m_RecalibProgInfo.RecalibEntry[i].dMeasured;
//		}
//		if((m_RecalibProgInfo.RecalibEntry[i].dActual <= 0) && (m_RecalibProgInfo.RecalibEntry[i].dMeasured <= 0))
//			break;
//		else
//			iPointNum++;
//	}

	if (m_RecalibProgInfo.wHeadType == HEAD_TYPE_STRESS) {
		for(int i = 0; i < m_ValidNum; i++){
			///// Actual /////
			if ( dActDataMax < m_RecalibProgInfo.RecalibEntry[i].dActual )
				dActDataMax = m_RecalibProgInfo.RecalibEntry[i].dActual;
			if ( dActDataMin > m_RecalibProgInfo.RecalibEntry[i].dActual )
				dActDataMin = m_RecalibProgInfo.RecalibEntry[i].dActual;
			///// Measured /////
			if ( dMeaDataMax < m_RecalibProgInfo.RecalibEntry[i].dMeasured )
				dMeaDataMax = m_RecalibProgInfo.RecalibEntry[i].dMeasured;
			if ( dMeaDataMin > m_RecalibProgInfo.RecalibEntry[i].dMeasured )
				dMeaDataMin = m_RecalibProgInfo.RecalibEntry[i].dMeasured;

			iPointNum++;
		}
	} else {
		dActDataMax=0;
		dMeaDataMax=0;
		for(int i = 0; i < m_ValidNum; i++){
			///// Actual /////
			if(m_RecalibProgInfo.RecalibEntry[i].dActual > 0){
				if ( dActDataMax < m_RecalibProgInfo.RecalibEntry[i].dActual )
					dActDataMax = m_RecalibProgInfo.RecalibEntry[i].dActual;
			}
/* added 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- { ---------- */
			if (2 <= m_ValidNum) {
				if (m_RecalibProgInfo.RecalibEntry[0].dActual > m_RecalibProgInfo.RecalibEntry[1].dActual) {
					if (m_RecalibProgInfo.RecalibEntry[i].dActual <= dActDataMin) {
						dActDataMin = m_RecalibProgInfo.RecalibEntry[i].dActual;
					}
				}
			}
/* added 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- } ---------- */
			///// Measured /////
			if(m_RecalibProgInfo.RecalibEntry[i].dMeasured > 0){
				if ( dMeaDataMax < m_RecalibProgInfo.RecalibEntry[i].dMeasured )
					dMeaDataMax = m_RecalibProgInfo.RecalibEntry[i].dMeasured;
			}
			if((m_RecalibProgInfo.RecalibEntry[i].dActual <= 0) && (m_RecalibProgInfo.RecalibEntry[i].dMeasured <= 0))
				break;
			else
				iPointNum++;
		}
	}
	//2009.09.01 bagus stress --}--

	//// �O���t�t���[�������ݒ� ////
	//if ((dMeaDataMax != 0) && (dActDataMax != 0)){
		int TmpdActDataMax;
		int TmpdMeaDataMax;

		if((dActDataMax-(int)dActDataMax)!=0)
			TmpdActDataMax = (int)dActDataMax + 1;
		else
			TmpdActDataMax = (int)dActDataMax;

		if((dMeaDataMax-(int)dMeaDataMax)!=0)
			TmpdMeaDataMax = (int)dMeaDataMax + 1;
		else
			TmpdMeaDataMax = (int)dMeaDataMax;

		//2009.09.01 bagus stress --{--

		int TmpdActDataMin;
		int TmpdMeaDataMin;

		if((dActDataMin-(int)dActDataMin)!=0)
			TmpdActDataMin = (int)dActDataMin - 1;
		else
			TmpdActDataMin = (int)dActDataMin;

		if((dMeaDataMin-(int)dMeaDataMin)!=0)
			TmpdMeaDataMin = (int)dMeaDataMin - 1;
		else
			TmpdMeaDataMin = (int)dMeaDataMin;

		//Graph_Frame_Init(0, TmpdActDataMax, 0, TmpdMeaDataMax, iPointNum);
		if (m_RecalibProgInfo.wHeadType == HEAD_TYPE_STRESS) {
			Graph_Frame_Init(TmpdActDataMin, TmpdActDataMax, TmpdMeaDataMin, TmpdMeaDataMax, iPointNum);
		} else {
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- { ---------- */
//			Graph_Frame_Init(0, TmpdActDataMax, 0, TmpdMeaDataMax, iPointNum);
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ----------              */
			if ((2 <= m_ValidNum) &&
				(m_RecalibProgInfo.RecalibEntry[0].dActual > m_RecalibProgInfo.RecalibEntry[1].dActual)) {
				Graph_Frame_Init(TmpdActDataMin,	TmpdActDataMax,	0,	TmpdMeaDataMax,	iPointNum);
			} else {
				Graph_Frame_Init(0,					TmpdActDataMax,	0,	TmpdMeaDataMax,	iPointNum);
			}
/* modified 2015.12.06 hmenjo ���J���u�t���֑Ή� ---------- } ---------- */
		}
		//2009.09.01 bagus stress --}--
	/*}
	else{
		Graph_Frame_Init();
	}
*/
	return;
}
