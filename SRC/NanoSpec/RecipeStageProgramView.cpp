// Kawa Memo
// ��2007.1014_����|�C���g�͍��W�ϊ��v���O�����������̌�A�C�����鎖�B
//		void CRecipeStageProgramView::SetData();
//		void CRecipeStageProgramView::UpDate();



// RecipeStageProgramView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "PointSetupDispositionDlg.h"
#include "PointSetupLinesDlg.h"
#include "PointSetupRectangleDlg.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingDlg.h"
#include "BitmapStatic.h"
#include "DeskewDlg.h"
#include "SpecifyCoordinateDlg.h"
#include "SitePatternSetDlg.h"
#include "RecipeStageProgramView.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "../../INC/NSStage.hxx"

#undef SubclassWindow	// �o�O: C4003: SubclassWindow �}�N���̃p�����[�^�[�̕s�� http://support.microsoft.com/kb/150076/ja

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STAGE_OFFSET		15
#define RADIUS				1
#define ELLIPSE_RADIUS		2//4


// 2009.10.09 bagus StagePGM ���ʉ� --{--
#define	STAGE_PROGRAM_HEAD_TYPE		(99)
// 2009.10.09 bagus StagePGM ���ʉ� --}--

//#define POINTSETUPTEACHING_CAPTION		"POINT SETUP - TEACHING"
//#define POINTSETUPTEACHING_MESSAGE		"POINT SETUP - TEACHING"
//Saiki 20090528 Delete ----->
//#define POINTSETUPTEACHING_CAPTION		"�|�C���g�ݒ� - �e�B�[�`���O"
//#define POINTSETUPTEACHING_MESSAGE		"�|�C���g�ݒ� - �e�B�[�`���O"
//Saiki 20090528 Delete <-----

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

// ##########################################################################
// CRecipeStageProgramView
// ##########################################################################

// ==========================================================================
//
IMPLEMENT_DYNCREATE(CRecipeStageProgramView, CNanoRecipeUI)

// ==========================================================================
//
CRecipeStageProgramView::CRecipeStageProgramView()
	: m_iNumScans(0), CNanoRecipeUI(CRecipeStageProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeStageProgramView)
	//}}AFX_DATA_INIT

	m_CurrentCellID.col = -1;
	m_CurrentCellID.row = -1;

// Kojika 20090527 Change
	//m_strCaption = "STAGE PROGRAM";
	//m_strCaption = "�X�e�[�W�v���O����";
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_STAGE_PROGRAM, l_strBuffer, "STAGE PROGRAM");
	m_strCaption = l_strBuffer;
// Kojika 20090527 Change End

	::ZeroMemory(m_dPointBuff, sizeof(m_dPointBuff));
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	::ZeroMemory(m_OldScanPoint, sizeof(m_OldScanPoint));
	::ZeroMemory(m_SitePoint, sizeof(m_SitePoint));
	::ZeroMemory(m_OldSitePoint, sizeof(m_OldSitePoint));
}

// ==========================================================================
//
CRecipeStageProgramView::~CRecipeStageProgramView()
{
}

// ==========================================================================
//
void CRecipeStageProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeStageProgramView)
	DDX_Control(pDX, IDC_SAMPLE_POINT_MAP, m_SamplePointGraph);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Control(pDX, IDC_STAGE_PROGRAM_POINT_NUMBER, m_stcStageProgramNumScan);
	DDX_Control(pDX, IDC_SAMPLE_NAME, m_stcSampleName);
	DDX_Control(pDX, IDC_SAMPLE_SHAPE, m_stcSampleShape);
	DDX_Control(pDX, IDC_SAMPLE_THICKNESS, m_stcSampleThickness);
	DDX_Control(pDX, IDC_SAMPLE_SIZE_X, m_stcSampleSizeX);
	DDX_Control(pDX, IDC_SAMPLE_SIZE_Y, m_stcSampleSizeY);
	DDX_Control(pDX, IDC_SAMPLE_ORIGIN_X, m_stcSampleOriginX);
	DDX_Control(pDX, IDC_SAMPLE_ORIGIN_Y, m_stcSampleOriginY);
	DDX_Control(pDX, IDC_MEASUREMENT_POINT_GRID, m_MeasurementPointGrid);
	DDX_Control(pDX, IDC_DISPOSITION_BUTTON, m_DispositionButton);
	DDX_Control(pDX, IDC_TEACHING_BUTTON, m_TeachingButton);
	DDX_Control(pDX, IDC_ALL_CLEAR_BUTTON, m_AllClearButton);
	DDX_Control(pDX, IDC_DESKEW_BUTTON, m_DeskewButton);
	DDX_Control(pDX, IDC_SITE_PR_SET_BUTTON, m_SitePRSetButton);
	DDX_Control(pDX, IDC_SITE_PR_DEL_BUTTON, m_SitePRDelButton);
	DDX_Control(pDX, IDC_ADD_BUTTON, m_AddButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_REDRAW_POINT_MAP, m_RedrawPointButton);
	DDX_Control(pDX, IDC_STAGE_DESKEW_MODE, m_stcDeskewMode);
	DDX_Control(pDX, IDC_STAGE_SITEPATTERN_MODE, m_stcSitePatternMode);
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CRecipeStageProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeStageProgramView)
	ON_BN_CLICKED(IDC_DISPOSITION_BUTTON, OnDispositionButton)
	ON_BN_CLICKED(IDC_TEACHING_BUTTON, OnTeachingButton)
	ON_BN_CLICKED(IDC_ALL_CLEAR_BUTTON, OnAllClearButton)
	ON_BN_CLICKED(IDC_DESKEW_BUTTON, OnDeskewButton)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_REDRAW_POINT_MAP, OnRedrawPointMap)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SITE_PR_SET_BUTTON, OnSitePrSetButton)
	ON_BN_CLICKED(IDC_SITE_PR_DEL_BUTTON, OnSitePrDelButton)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_ENDLABELEDIT, IDC_MEASUREMENT_POINT_GRID, OnEndLabelEdit)
	ON_NOTIFY(GVN_SELCHANGED, IDC_MEASUREMENT_POINT_GRID, OnSelChanged)
	ON_NOTIFY(GVN_SELCHANGING, IDC_MEASUREMENT_POINT_GRID, OnSelChangeing)
END_MESSAGE_MAP()

// ==========================================================================
// CRecipeStageProgramView �f�f

#ifdef _DEBUG
// ==========================================================================
//
void CRecipeStageProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

// ==========================================================================
//
void CRecipeStageProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

// ==========================================================================
// CRecipeStageProgramView ���b�Z�[�W �n���h��

// ==========================================================================
//
void CRecipeStageProgramView::OnInitialUpdate()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	//���Ή��@�\HIDE
	GetDlgItem(IDC_AXIS_SYNCHRONOUS_CHECK)->ShowWindow(SW_HIDE);

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);

	if ( !LoadRecipeData() ) {
		UpDate();
		::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
		::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
// Kojika 20090527 Change
		//MessageBox("Can not open the sample type file.\nReturn to Stage Program List.", m_strCaption, MB_OK | MB_ICONSTOP);
		//MessageBox("�T���v���^�C�v�̃t�@�C�����J�����Ƃ��ł��܂���ł���.\n�X�e�[�W�v���O�����ɖ߂��ĉ�����", m_strCaption, MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_NOT_OPEN_SAMPLE_TYPE, l_strBuffer, "Can not open the sample type file.\nReturn to Stage Program List.");
		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
// Kojika 20090527 Change End
		m_pMainFrame->PostMessage(WM_EXE_WND_EXIT, 0, 0);
		return;
	}

	CNanoRecipeUI::OnInitialUpdate();

	///// Edit Tool Bar /////
	RECT rect;
	if(!m_wndEditBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndEditBar.LoadToolBar(IDR_EDIT_RECIPE_BAR)){
		TRACE0("Failed to create toolbar\n");
		return; 	// �쐬�Ɏ��s
	}

	GetClientRect(&rect);
	m_wndEditBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	/////�@Sample�p�^�[���F���Ȃ� /////
// 2009.12.03 K.Matsuo -->
//	if(!((m_SystemConfig.nPRMethod == 1) && (m_SystemConfig.bSamplePatternRec == TRUE))){
	if((m_SystemConfig.nPRMethod == 0) || (m_SystemConfig.bSamplePatternRec == FALSE)){
// 2009.12.03 K.Matsuo <--
		GetDlgItem(IDC_STAGE_DESKEW_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DESKEW_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DESKEW_GROUP)->ShowWindow(SW_HIDE);
	}

	/////�@�T�C�g�p�^�[���F���Ȃ� /////
// 2009.12.03 K.Matsuo -->
//	if(!((m_SystemConfig.nPRMethod == 1) && (m_SystemConfig.bSitePatternRec == TRUE))){
//2009.12.25 bagus --{--
//	if((m_SystemConfig.nPRMethod == 0) || (m_SystemConfig.bSamplePatternRec == FALSE)){
	if((m_SystemConfig.nPRMethod == 0) || (m_SystemConfig.bSamplePatternRec == FALSE)
	|| m_SystemConfig.bSitePatternRec == FALSE){
//2009.12.25 bagus --{--
// 2009.12.03 K.Matsuo <--
		GetDlgItem(IDC_STAGE_SITEPATTERN_MODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SITE_PR_SET_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SITE_PR_DEL_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SITE_PR_GROUP)->ShowWindow(SW_HIDE);
	}

	///// Static Control /////
	m_stcStageProgramName.SubclassWindow(GetDlgItem(IDC_STAGE_PROGRAM_NAME)->GetSafeHwnd());
	m_stcStageProgramName.SetBkColor(WATER_COLOR);
	m_stcStageProgramNumScan.SetBkColor(WATER_COLOR);
	m_stcSampleName.SetBkColor(WATER_COLOR);
	m_stcSampleShape.SetBkColor(WATER_COLOR);
	m_stcSampleThickness.SetBkColor(WATER_COLOR);
	m_stcSampleSizeX.SetBkColor(WATER_COLOR);
	m_stcSampleSizeY.SetBkColor(WATER_COLOR);
	m_stcSampleOriginX.SetBkColor(WATER_COLOR);
	m_stcSampleOriginY.SetBkColor(WATER_COLOR);
	m_stcDeskewMode.SetBkColor(WATER_COLOR);
	m_stcSitePatternMode.SetBkColor(WATER_COLOR);

	m_DispositionButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DispositionButton.DrawFlatFocus(TRUE);
	m_TeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TeachingButton.DrawFlatFocus(TRUE);
	m_AllClearButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllClearButton.DrawFlatFocus(TRUE);
	m_DeskewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeskewButton.DrawFlatFocus(TRUE);
	m_SitePRSetButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SitePRSetButton.DrawFlatFocus(TRUE);
	m_SitePRDelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_SitePRDelButton.DrawFlatFocus(TRUE);
	m_AddButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AddButton.DrawFlatFocus(TRUE);
	m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteButton.DrawFlatFocus(TRUE);
	m_RedrawPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RedrawPointButton.DrawFlatFocus(TRUE);

	///// Measurement Point Grid /////
	SamplePointGraph_Init();

	///// Measurement Point Grid /////
	m_MeasurementPointGrid.GridInit(m_iNumScans);
	m_MeasurementPointGrid.GridEditable();

	///// Axis Synchronous /////
	CheckDlgButton(IDC_AXIS_SYNCHRONOUS_CHECK, BST_UNCHECKED);

	///// Get Current Execute Window /////
	m_iWnd = m_pMainFrame->GetCurrExeWnd();

	///// Set Data /////
	SetData();

	///// copy Data Buff /////
	for ( int iPoint = 0; iPoint < m_StageProgInfoHdr.wNumScans; iPoint++ ) {
		m_dPointBuff[0][iPoint] = (m_ScanPoint[iPoint].lX) / MICROMETRE ;
		m_dPointBuff[1][iPoint] = (m_ScanPoint[iPoint].lY) / MICROMETRE ;
		m_dPointBuff[2][iPoint] = TRUE;
	}

	// �R�����g����������
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(RECIPE_COMMENT_LEN);

	char szFilePath[MAX_PATH + 1];
	char szFilePathBackUp[MAX_PATH + 1];
// 2009.06.10 K.Matsuo -->
	char szFilePathUntitled[MAX_PATH + 1];
// 2009.06.10 K.Matsuo <--

// 2009.06.10 K.Matsuo -->
	// �p�^�[���F���摜�i�V�K�쐬�j�̏ꍇ�̂��ݍ폜�iUntitled�A�^�C�g���Ȃ��̖��O�ŕۑ����ꂽ�f�[�^�j
	if ( m_nOpenMode == modeNew ) {
		_stprintf(szFilePathUntitled, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
		::DeleteFile(szFilePathUntitled);
		_stprintf(szFilePathUntitled, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
		::DeleteFile(szFilePathUntitled);
	}

	// �f�o�b�O���Ɋg���q�݂̂̂��݃t�@�C�����ł��Ă��܂��Ă���̂ŁA�O�̂��߂ɍ폜���������Ă���
	_stprintf(szFilePathUntitled, "%s.D1", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathUntitled);
	_stprintf(szFilePathUntitled, "%s.D2", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathUntitled);

// 2009.06.10 K.Matsuo <--

	// �p�^�[���F���摜�i���[�N�j�̂��ݍ폜
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathBackUp);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	::DeleteFile(szFilePathBackUp);

	// �p�^�[���F���摜�̑ޔ�
	_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);
	_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_StageProgInfoHdr.hdr.szName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);
}

// ==========================================================================
//
void CRecipeStageProgramView::OnDispositionButton()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	CCellID CellID;
	int iRow;
	char szBuff[256];
	int iCountX, iCountY;
	int iNumCount = 0;
	int iAddCount = 0;
	long lX, lY;

	UpDate();

	//2009.09.30 bagus se --{--
	//���_�ʒu�Ή�
	//CPointSetupDispositionDlg dlg(m_iNumScans, m_StageProgInfoHdr.SampleInfo.Size.dx, m_StageProgInfoHdr.SampleInfo.Size.dy);
	CPointSetupDispositionDlg dlg(m_iNumScans, m_StageProgInfoHdr.SampleInfo.Size.dx, m_StageProgInfoHdr.SampleInfo.Size.dy,m_StageConfig.OrgPos);
	//2009.09.30 bagus se --}--

	if(m_iNumScans >= SCAN_POINT_MAX){
// Kojika 20090527 Change
		//sprintf(szBuff, "The Maximum Number which can be Registered is %d.", SCAN_POINT_MAX);
		//sprintf(szBuff, "���͂ł���ő吔�� %d �ł�", SCAN_POINT_MAX);
	LoadStringML(IDS_MAX_NUM_REGISTER, l_strBuffer, "The Maximum Number which can be Registered is %d.");
	sprintf(szBuff, (LPCSTR)l_strBuffer, SCAN_POINT_MAX);
// Kojika 20090527 Change End
		MessageBox(szBuff, m_strCaption, MB_OK|MB_ICONSTOP);
		return;
	}

	CellID = m_MeasurementPointGrid.GetFocusCell();
	iRow = CellID.row;

	if(dlg.DoModal() == IDCANCEL){
		return;
	}

	//m_MeasurementPointGrid.GridInit(m_iNumScans);
	m_MeasurementPointGrid.GridInit((dlg.m_iCountX * dlg.m_iCountY) + m_iNumScans);
//	ClearMeasurementPoint();

	//�r���ɑ}���ǉ��̏ꍇ�A�}���ʒu�ȉ��̃f�[�^���ړ�������
	if(( iRow != -1 ) && ( iRow <= m_iNumScans )){
		if(dlg.m_iDrawShaped == STAGE_DRAW_SHAPED_LINE){
			iAddCount = dlg.m_iCountX;
		}
		else{
			iAddCount = dlg.m_iCountX * dlg.m_iCountY;
		}
		for(iCountX = m_iNumScans; iCountX > iRow -1; iCountX--){
			m_dPointBuff[0][iAddCount + iCountX - 1] = m_dPointBuff[0][iCountX -1];
			m_dPointBuff[1][iAddCount + iCountX - 1] = m_dPointBuff[1][iCountX -1];
			m_dPointBuff[2][iAddCount + iCountX - 1] = m_dPointBuff[2][iCountX -1];

			///// X /////
			sprintf(szBuff, "%.3lf", m_dPointBuff[0][iAddCount + iCountX - 1]);
			m_MeasurementPointGrid.GridDisp(iAddCount + iCountX, 1, szBuff, DT_RIGHT);

			///// Y /////
			sprintf(szBuff, "%.3lf", m_dPointBuff[1][iAddCount + iCountX - 1]);
			m_MeasurementPointGrid.GridDisp(iAddCount + iCountX, 2, szBuff, DT_RIGHT);
		}
		for(iCountX = 1; iCountX < iRow; iCountX++){
			///// X /////
			sprintf(szBuff, "%.3lf", m_dPointBuff[0][iCountX -1]);
			m_MeasurementPointGrid.GridDisp(iCountX, 1, szBuff, DT_RIGHT);

			///// Y /////
			sprintf(szBuff, "%.3lf", m_dPointBuff[1][iCountX -1]);
			m_MeasurementPointGrid.GridDisp(iCountX, 2, szBuff, DT_RIGHT);
		}
	}
	else{
		//�����f�[�^�̍ĕ`��
		for(int iSetRow = 0 ; iSetRow < m_iNumScans ; iSetRow++){
			///// X /////
			sprintf(szBuff, "%.3lf", m_dPointBuff[0][iSetRow]);
			m_MeasurementPointGrid.GridDisp(iSetRow + 1, 1, szBuff, DT_RIGHT);

			///// Y /////
			sprintf(szBuff, "%.3lf", m_dPointBuff[1][iSetRow]);
			m_MeasurementPointGrid.GridDisp(iSetRow + 1, 2, szBuff, DT_RIGHT);
		}
	}

	//Row���I������Ă��ȏꍇ�́A�����f�[�^�̌�Ƀf�[�^��ǉ����܂��B
	if(dlg.m_iDrawShaped == STAGE_DRAW_SHAPED_LINE){
		for(iCountX = 0; iCountX < dlg.m_iCountX; iCountX++){
			lX = dlg.m_lStartPointX + dlg.m_lPitchX * iCountX;
			lY = dlg.m_lStartPointY + dlg.m_lPitchY * iCountX;

			iAddCount = iRow == -1 ? m_iNumScans: iRow + iCountX - 1;

			///// X /////
			sprintf(szBuff, "%.3lf", ((double)lX / MICROMETRE) );
			m_MeasurementPointGrid.GridDisp(iAddCount + 1, 1, szBuff, DT_RIGHT);

			///// Y /////
			sprintf(szBuff, "%.3lf", ((double)lY / MICROMETRE) );
			m_MeasurementPointGrid.GridDisp(iAddCount + 1, 2, szBuff, DT_RIGHT);

			///// Draw Point Mark /////
			m_dPointBuff[0][iAddCount] = ((double)lX / MICROMETRE);
			m_dPointBuff[1][iAddCount] = ((double)lY / MICROMETRE);
			m_dPointBuff[2][iAddCount] = TRUE;

			m_iNumScans++;
		}
	}

	else{
		//�f�[�^�̒ǉ�
		int m_iTmpNumScans = m_iNumScans;
		for(iCountY = 0 ; iCountY < dlg.m_iCountY; iCountY++){
			lY = dlg.m_lStartPointY + dlg.m_lPitchY * iCountY;
			//����
			if ((iCountY % 2) == 0){
				for(iCountX = 0 ; iCountX < dlg.m_iCountX ; iCountX++){
					lX = dlg.m_lStartPointX + dlg.m_lPitchX * iCountX;

					iNumCount = iCountY * dlg.m_iCountX + iCountX;
					//�ŏI�s�ɒǉ��Fm_iTmpNumScans + iNumCount �@�@�I���s�ɒǉ�: iRow + iNumCount - 1
					iAddCount = iRow == -1 ? m_iTmpNumScans + iNumCount : iRow + iNumCount - 1;

					///// X /////
					sprintf(szBuff, "%.3lf", ((double)lX / MICROMETRE ));
					m_MeasurementPointGrid.GridDisp(iAddCount + 1, 1, szBuff, DT_RIGHT);

					///// Y /////
					sprintf(szBuff, "%.3lf", ((double)lY / MICROMETRE ));
					m_MeasurementPointGrid.GridDisp(iAddCount + 1, 2, szBuff, DT_RIGHT);

					///// Draw Point Mark /////
					m_dPointBuff[0][iAddCount] = ((double)lX / MICROMETRE) ;
					m_dPointBuff[1][iAddCount] = ((double)lY / MICROMETRE) ;
					m_dPointBuff[2][iAddCount] = TRUE;
					m_iNumScans++;
				}
			}
			//�~��
			else{
				for(iCountX = dlg.m_iCountX -1 ; iCountX >= 0  ; iCountX--){
					lX = dlg.m_lStartPointX + dlg.m_lPitchX * iCountX;

					iNumCount = iCountY * dlg.m_iCountX + (dlg.m_iCountX - iCountX -1);
					iAddCount = iRow == -1 ? m_iTmpNumScans + iNumCount : iRow + iNumCount - 1;
					///// X /////
					sprintf(szBuff, "%.3lf", ((double)lX / MICROMETRE) );
					m_MeasurementPointGrid.GridDisp(iAddCount + 1, 1, szBuff, DT_RIGHT);

					///// Y /////
					sprintf(szBuff, "%.3lf", ((double)lY / MICROMETRE) );
					m_MeasurementPointGrid.GridDisp(iAddCount + 1, 2, szBuff, DT_RIGHT);

					///// Draw Point Mark /////
					m_dPointBuff[0][iAddCount] = ((double)lX / MICROMETRE) ;
					m_dPointBuff[1][iAddCount] = ((double)lY / MICROMETRE) ;
					m_dPointBuff[2][iAddCount] = TRUE;

					m_iNumScans++;
				}
			}

		}
	}

	///// Data Update /////
	//m_iNumScans++;
	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;
	for(int i = 0 ; i < m_iNumScans + 1; i++){
		if ( m_dPointBuff[2][i] != FALSE ) {
			m_ScanPoint[i].lX = static_cast<long>((m_dPointBuff[0][i]) * MICROMETRE);
			m_ScanPoint[i].lY = static_cast<long>((m_dPointBuff[1][i]) * MICROMETRE);
		}
	}
	SamplePointGraph_DataSet();
}

// ==========================================================================
//
void CRecipeStageProgramView::OnTeachingButton()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	char szBuff[256];

	if(m_iNumScans >= SCAN_POINT_MAX){
// Kojika 20090527 Change
		//sprintf(szBuff, "The Maximum Number which can be Registered is %d.", SCAN_POINT_MAX);
		//sprintf(szBuff, "���͂ł���ő吔�� %d �ł�", SCAN_POINT_MAX);
	LoadStringML(IDS_MAX_NUM_REGISTER, l_strBuffer, "The Maximum Number which can be Registered is %d.");
	sprintf(szBuff, (LPCSTR)l_strBuffer, SCAN_POINT_MAX);
// Kojika 20090527 Change End
		MessageBox(szBuff, m_strCaption, MB_OK | MB_ICONSTOP);
		return;
	}

// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
	CString strMsg;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVO�A���[�� 				���k
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ���C���^�[���b�N������
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	CPointSetupTeachingDlg dlg;

//	char szBuff[256];

// Goto test add --->
	UpDate();
	dlg.m_iNumScans = m_iNumScans;
	memcpy(&dlg.m_StageProgInfoHdr, &m_StageProgInfoHdr, sizeof(dlg.m_StageProgInfoHdr));
	memcpy(dlg.m_ScanPoint, m_ScanPoint, sizeof(dlg.m_ScanPoint));
// Goto test add <---

// 2013.02.22 bagus Substrate thickness setting -->
	BOOL bSelected;
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	bSelected = SelectSubThickNotify(m_StageProgInfoHdr.SampleInfo.dThickness);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);

	if ( !bSelected ) {
		AlarmIf_Set(ALID_SubstrateThicknessError);
	}
// 2013.02.22 bagus Substrate thickness setting <--

	//dlg.SetFileName(&m_StageProgInfoHdr);
	LoadStringML(IDS_POINTSETUPTEACHING_CAPTION, l_strBuffer, "POINT SETUP - TEACHING");
	dlg.SetCaption(l_strBuffer);

	LoadStringML(IDS_POINTSETUPTEACHING_MESSAGE, l_strBuffer, "POINT SETUP - TEACHING");
	dlg.SetMessage(l_strBuffer);

	if(dlg.DoModal() == IDOK){
		m_iNumScans = dlg.m_iNumScans;
		memcpy(m_ScanPoint, dlg.m_ScanPoint, sizeof(m_ScanPoint));
		SetData();

		m_iNumScans++;
	}

	UpDate();
	SamplePointGraph_DataSet();
}

// ==========================================================================
//
void CRecipeStageProgramView::OnAllClearButton()
{
// Kojika 20090527 Add
	CString l_strBuffer, l_strTitle;
// Kojika 20090527 Add End

// Kojika 20090527 Change
	//if ( MessageBox("Do you want all clear ?", "ALL CLEAR", MB_ICONQUESTION | MB_OKCANCEL) != IDOK )
	//if ( MessageBox("�S�Ă̑���|�C���g���������Ă��X�����ł����H", "�S�����m�F", MB_ICONQUESTION | MB_OKCANCEL) != IDOK )
	LoadStringML(IDS_CLEAR_ALL_MEASERE_POINT, l_strBuffer, "Do you want all measured point clear ?");
	LoadStringML(IDS_TITLE_ALL_CANCELLATION, l_strTitle, "ALL CANCELLATION CONFIRMATION");
	if ( MessageBox(l_strBuffer, l_strTitle, MB_ICONQUESTION | MB_OKCANCEL) != IDOK )
// Kojika 20090527 Change End
		return;

	ClearMeasurementPoint();
	::ZeroMemory(m_dPointBuff, sizeof(m_dPointBuff));
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));

	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;
	m_MeasurementPointGrid.GridInit(m_iNumScans);
	//SamplePointGraph_Init();
	m_SamplePointGraph.Reset();

	Invalidate(TRUE);
}

// ==========================================================================
//
void CRecipeStageProgramView::OnDeskewButton()
{
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
	CString strMsg;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVO�A���[�� 				���k
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ���C���^�[���b�N������
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	CDeskewDlg dlg(&m_StageProgInfoHdr);
	if( dlg.DoModal() == IDOK ){
		SetData();
	}
}

// ==========================================================================
//
void CRecipeStageProgramView::OnAddButton()
{
// Kojika 20090527 Add
	CString l_strBuffer;
// Kojika 20090527 Add End

	CCellID CellID;
	int iRow;
	char szGridText[1024];
	char szBuff[256];
	//char *stopstring;
	int Valid_X;
	int Valid_Y;
	int i;

	if(m_iNumScans >= SCAN_POINT_MAX){
// Kojika 20090527 Change
		//sprintf(szBuff, "The Maximum Number which can be Registered is %d.", SCAN_POINT_MAX);
		//sprintf(szBuff, "���͂ł���ő吔�� %d �ł�", SCAN_POINT_MAX);
	LoadStringML(IDS_MAX_NUM_REGISTER, l_strBuffer, "The Maximum Number which can be Registered is %d.");
	sprintf(szBuff, (LPCSTR)l_strBuffer, SCAN_POINT_MAX);
// Kojika 20090527 Change End
		MessageBox(szBuff, m_strCaption, MB_OK | MB_ICONSTOP);
		return;
	}

// 2013.01.09 bagus -->
//	CSpecifyCoordinateDlg dlg(RECIPE_STAGE_WND, m_StageProgInfoHdr.SampleInfo.Size.dx, m_StageProgInfoHdr.SampleInfo.Size.dy);

	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
	}

	CSpecifyCoordinateDlg dlg(RECIPE_STAGE_WND, dSampleSizeX, dSampleSizeY);
// 2013.01.09 bagus <--

	CellID = m_MeasurementPointGrid.GetFocusCell();
	iRow = CellID.row;
	dlg.m_nSampleMode =1;

	if(dlg.DoModal() != IDOK){
		return;
	}

	::ZeroMemory(m_dPointBuff, sizeof(m_dPointBuff));

	///// Refresh Data /////
	m_iNumScans = 0;
	memset(m_ScanPoint, 0, sizeof(m_ScanPoint));
	for(i = 0 ; i < SCAN_POINT_MAX ; i++){
		if(i == (iRow - 1)){
			m_dPointBuff[0][i] = (double)dlg.m_lX / MICROMETRE;
			m_dPointBuff[1][i] = (double)dlg.m_lY / MICROMETRE;
		}
		Valid_X = Valid_Y = 0;
		///// X /////
		strcpy(szGridText, m_MeasurementPointGrid.GetItemText(i + 1, 1));
		Valid_X = strlen(szGridText);
		if(Valid_X){
			if(i >= (iRow - 1) && iRow != -1){
				m_dPointBuff[0][i+1] = atof(szGridText);
				//m_dPointBuff[0][i + 1] = strtol(szGridText, &stopstring, 10);
			}
			else{
				m_dPointBuff[0][i] = atof(szGridText);
				//m_dPointBuff[0][i] = strtol(szGridText, &stopstring, 10);
			}
		}

		///// Y /////
		strcpy(szGridText, m_MeasurementPointGrid.GetItemText(i + 1, 2));
		Valid_Y = strlen(szGridText);
		if(Valid_Y){
			if(i >= (iRow - 1) && iRow != -1){
				m_dPointBuff[1][i+1] = atof(szGridText);
				//m_dPointBuff[1][i + 1] = strtol(szGridText, &stopstring, 10);
			}
			else{
				m_dPointBuff[1][i] = atof(szGridText);
				//m_dPointBuff[1][i] = strtol(szGridText, &stopstring, 10);
			}
		}

		///// Judgement /////
		if(Valid_X && Valid_Y){
			m_dPointBuff[2][i] = TRUE;
			m_iNumScans++;
		}
		else{
			m_dPointBuff[2][m_iNumScans] = TRUE;
			break;
		}
	}

	///// Measurement Point Grid Add Cell /////
	m_MeasurementPointGrid.GridInit(m_iNumScans+1);

	///// Refresh /////
	for(i = 0 ; i < m_iNumScans + 1; i++){
		sprintf(szBuff, "%.3lf", m_dPointBuff[0][i]);
		m_MeasurementPointGrid.GridDisp(i + 1, 1, szBuff, DT_RIGHT);

		sprintf(szBuff, "%.3lf", m_dPointBuff[1][i]);
		m_MeasurementPointGrid.GridDisp(i + 1, 2, szBuff, DT_RIGHT);
	}

	///// �V�KPoint�ǉ� /////
	if(iRow == -1){
		m_dPointBuff[0][m_iNumScans] = (double)dlg.m_lX / MICROMETRE;;
		sprintf(szBuff, "%.3lf", m_dPointBuff[0][m_iNumScans]);
		m_MeasurementPointGrid.GridDisp(m_iNumScans + 1, 1, szBuff, DT_RIGHT);

		m_dPointBuff[1][m_iNumScans] = (double)dlg.m_lY / MICROMETRE;
		sprintf(szBuff, "%.3lf", m_dPointBuff[1][m_iNumScans]);
		m_MeasurementPointGrid.GridDisp(m_iNumScans + 1, 2, szBuff, DT_RIGHT);

		m_dPointBuff[2][m_iNumScans] = TRUE;
	}

	///// Data Update /////
	m_iNumScans++;
	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;
	for(i = 0 ; i < m_iNumScans + 1; i++){
		if ( m_dPointBuff[2][i] != FALSE ) {
			m_ScanPoint[i].lX = static_cast<long>(m_dPointBuff[0][i] * MICROMETRE);
			m_ScanPoint[i].lY = static_cast<long>(m_dPointBuff[1][i] * MICROMETRE);
		}
	}
	SamplePointGraph_DataSet();

	Invalidate(TRUE);
}

// ==========================================================================
//
void CRecipeStageProgramView::OnDeleteButton()
{
// Kojika 20090527 Add
	CString l_strBuffer, strBuffer1, strBuffer2;
// Kojika 20090527 Add End

	int iRow;
	char szBuff[256];
	char szGridText[1024];
	int Valid_X;
	int Valid_Y;
	int i;

	///// SelectPoint /////
	//CCellID CellID;
	//CellID = m_MeasurementPointGrid.GetFocusCell();
	//iRow = CellID.row;
	iRow = m_CurrentCellID.row;
	//NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	//iRow=pItem->iRow;

	if(iRow <= 0){
		//MessageBox("Choose the Data to Delete.", m_strCaption, MB_OK | MB_ICONEXCLAMATION);
		//Saiki 20090528 Change ----->
		//MessageBox("�폜����f�[�^��I��ł�������", m_strCaption, MB_OK | MB_ICONEXCLAMATION);
		LoadStringML(IDS_CHOOSE_DELETE_DATA, strBuffer1, "Choose the Data to Delete.");
		MessageBox(strBuffer1, m_strCaption, MB_OK | MB_ICONEXCLAMATION);
		//Saiki 20090528 Change <-----
		return;
	}
	//sprintf(szBuff, "%s%d%s", "Is the Point Data of No. ", iRow, " deleted?");
	//Saiki 20090528 Change ----->
	//sprintf(szBuff, "%s%d%s", "No.", iRow, " �̃|�C���g�f�[�^���폜���܂���d?");
	LoadStringML(IDS_POINT_DATA_NO, strBuffer1, "Is the Point Data of No. ");
	LoadStringML(IDS_DELETE_POINT_DATA, strBuffer2, " deleted?");
	sprintf(szBuff, "%s%d%s", (LPCSTR)strBuffer1, iRow, (LPCSTR)strBuffer2);
	//Saiki 20090528 Change <-----
	if( MessageBox(szBuff, m_strCaption, MB_YESNO | MB_ICONQUESTION) !=IDYES ) {
		return;
	}

	if ( m_MeasurementPointGrid.GetItemText(iRow, 1).IsEmpty() || m_MeasurementPointGrid.GetItemText(iRow, 2).IsEmpty() ) {
		m_MeasurementPointGrid.GridDisp(iRow, 1, "", DT_RIGHT);
		m_MeasurementPointGrid.GridDisp(iRow, 2, "", DT_RIGHT);
		return;
	}

	///// Delete Row /////
	::ZeroMemory(m_dPointBuff, sizeof(m_dPointBuff));

	///// Refresh Data /////
	m_iNumScans = 0;
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	for ( int i = 0 ; i < SCAN_POINT_MAX ; i++ ) {
		Valid_X = Valid_Y = 0;
		///// X /////
		strcpy(szGridText, m_MeasurementPointGrid.GetItemText(i + 1, 1));
		Valid_X = strlen(szGridText);
		if(Valid_X){
			if(i > (iRow -1)){
				m_dPointBuff[0][i-1] = atof(szGridText);
				//m_dPointBuff[0][i-1] = strtol(szGridText, &stopstring, 10);
			}
			else{
				m_dPointBuff[0][i] = atof(szGridText);
				//m_dPointBuff[0][i] = strtol(szGridText, &stopstring, 10);
			}
		}

		///// Y /////
		strcpy(szGridText, m_MeasurementPointGrid.GetItemText(i + 1, 2));
		Valid_Y = strlen(szGridText);
		if(Valid_Y){
			if(i > (iRow -1)){
				m_dPointBuff[1][i-1] = atof(szGridText);
				//m_dPointBuff[1][i -1] = strtol(szGridText, &stopstring, 10);
			}
			else{
				m_dPointBuff[1][i] = atof(szGridText);
				//m_dPointBuff[1][i] = strtol(szGridText, &stopstring, 10);
			}
		}

		///// Judgement /////
		if(Valid_X && Valid_Y){
			m_dPointBuff[2][i] = TRUE;
			m_iNumScans++;
		}
		else{
			m_dPointBuff[0][m_iNumScans -1] = 0;
			m_dPointBuff[1][m_iNumScans -1] = 0;
			m_dPointBuff[2][m_iNumScans -1] = FALSE;
			break;
		}
	}

	m_iNumScans = m_iNumScans - 1;

	///// Measurement Point Grid Delete Cell /////
	m_MeasurementPointGrid.GridInit(m_iNumScans);

	///// Refresh /////
	for ( i = 0; i < m_iNumScans; i++ ) {
		sprintf(szBuff, "%.3lf", m_dPointBuff[0][i]);
		m_MeasurementPointGrid.GridDisp(i + 1, 1, szBuff, DT_RIGHT);

		sprintf(szBuff, "%.3lf", m_dPointBuff[1][i]);
		m_MeasurementPointGrid.GridDisp(i + 1, 2, szBuff, DT_RIGHT);
	}

	m_MeasurementPointGrid.GridDisp(m_iNumScans + 1, 1, "", DT_RIGHT);
	m_MeasurementPointGrid.GridDisp(m_iNumScans + 1, 2, "", DT_RIGHT);

	///// Data Update /////
	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;
	for(i = 0 ; i < m_iNumScans + 1; i++){
		if ( m_dPointBuff[2][i] != FALSE ) {
			m_ScanPoint[i].lX = static_cast<long>((m_dPointBuff[0][i]) * MICROMETRE);
			m_ScanPoint[i].lY = static_cast<long>((m_dPointBuff[1][i]) * MICROMETRE);
		}
	}
	SamplePointGraph_DataSet();

	Invalidate(TRUE);
}

// ==========================================================================
//
void CRecipeStageProgramView::ClearMeasurementPoint()
{
	for ( int iRow = 1; iRow < m_MeasurementPointGrid.GetRowCount(); iRow++ ) {
		///// X /////
		m_MeasurementPointGrid.GridDisp(iRow, 1, "", DT_RIGHT);
		///// Y /////
		m_MeasurementPointGrid.GridDisp(iRow, 2, "", DT_RIGHT);
	}

	m_iNumScans = 0;
}

// ==========================================================================
//
BOOL CRecipeStageProgramView::LoadRecipeData()
{
	TCHAR szSampleTypeName[RECIPE_NAME_LEN + 1];
	SAMPLEINFO SampleInfo;

// 2009.09.29 bagus SE --{--
// 2009.10.09 bagus StagePGM ���ʉ� --{--
//	WORD	wHeadType;
// 2009.10.09 bagus StagePGM ���ʉ� --}--
// 2009.09.29 bagus SE --}--

	// �V�K�쐬�̏ꍇ
	if ( m_nOpenMode == modeNew ) {
		///// Get Selected Sample Type	Name /////
		m_pMainFrame->GetSelectSampleTypeName(szSampleTypeName);

// 2009.09.29 bagus SE --{--
// 2009.10.09 bagus StagePGM ���ʉ� --{--
//		wHeadType = (WORD)pMainFrame->GetSelectHeadType();
// 2009.10.09 bagus StagePGM ���ʉ� --}--
// 2009.09.29 bagus SE --}--

		//	///// Sample /////
		if ( !RecipeFile_LoadRecipe(&SampleInfo, szSampleTypeName, RECIPE_FILE_SAMPLE) ) {
			memset(&m_StageProgInfoHdr, 0, sizeof(m_StageProgInfoHdr));
			memset(m_ScanPoint, 0, sizeof(m_ScanPoint));
			return FALSE;
		}
		else {
			strcpy(m_StageProgInfoHdr.SampleInfo.szName, SampleInfo.szName);
			m_StageProgInfoHdr.SampleInfo.nShape = SampleInfo.nShape;
			m_StageProgInfoHdr.SampleInfo.dThickness = SampleInfo.dThickness;
			m_StageProgInfoHdr.SampleInfo.Size.dx = SampleInfo.Size.dx;
			m_StageProgInfoHdr.SampleInfo.Size.dy = SampleInfo.Size.dy;
			m_StageProgInfoHdr.SampleInfo.Origin.lX = SampleInfo.Origin.lX;
			m_StageProgInfoHdr.SampleInfo.Origin.lY = SampleInfo.Origin.lY;

			///// Site Pattern /////
			::ZeroMemory(m_SitePoint, sizeof(m_SitePoint));
		}

// 2009.09.29 bagus SE --{--
// 2009.10.09 bagus StagePGM ���ʉ� --{--
//		m_StageProgInfoHdr.wHeadType = wHeadType;
// 2009.10.09 bagus StagePGM ���ʉ� --}--
// 2009.09.29 bagus SE --}--
// 2009.12.14 bagus NANOMAP �ǉ� --{--
		m_StageProgInfoHdr.SampleInfo.StageConfig = m_StageConfig;
// 2009.12.14 bagus NANOMAP �ǉ� --}--
	}
	else{
		///// Stage Program Header /////
		if ( !RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_szRecipeName, RECIPE_FILE_STAGE_PROGRAM) ) {
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
// 2009.06.10 K.Matsuo -->
			::ZeroMemory(&m_OldStageProgInfoHdr, sizeof(m_OldStageProgInfoHdr));
// 2009.06.10 K.Matsuo <--
		}
		else{
// 2009.12.02 K.Matsuo SampleOrigin -->
			SAMPLEINFO sampleInfo;
			::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
			RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
			m_StageProgInfoHdr.SampleInfo.Origin.lX = sampleInfo.Origin.lX;
			m_StageProgInfoHdr.SampleInfo.Origin.lY = sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
			::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
			m_iNumScans = (int)m_StageProgInfoHdr.wNumScans;
		}

		///// Measurement Point /////
		if ( !RecipeFile_LoadPointList(m_ScanPoint, m_iNumScans, m_szRecipeName) ) {
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
			::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
		}
		else{
			::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
		}

		///// Sample Name /////
		strcpy(szSampleTypeName, m_StageProgInfoHdr.SampleInfo.szName);

		///// Site Pattern /////
		::ZeroMemory(m_SitePoint, sizeof(m_SitePoint));
// 2009.12.03 K.Matsuo -->
//		if((m_SystemConfig.nPRMethod == 1) && (m_SystemConfig.bSitePatternRec == TRUE)){
		if((m_SystemConfig.nPRMethod > 0) && (m_SystemConfig.bSitePatternRec == TRUE)){
// 2009.12.03 K.Matsuo <--
			/////�@�T�C�g�p�^�[���F������ /////
			m_wNumSite = m_iNumScans;
			if( !PatternFile_LoadPointList(m_SitePoint, &m_wNumSite, m_szRecipeName) ){
			}
			else{
				::CopyMemory(m_OldSitePoint, m_SitePoint, sizeof(m_SitePoint));
			}
		}
	}

// 2009.10.09 bagus StagePGM ���ʉ� --{--
	m_StageProgInfoHdr.wHeadType = STAGE_PROGRAM_HEAD_TYPE;
	m_StageProgInfoHdr.wScanType = SCAN_TYPE_STANDARD;
// 2009.10.09 bagus StagePGM ���ʉ� --}--
// 2009.10.13 bagus StagePGM �C�� --{--
	m_OldStageProgInfoHdr.wHeadType = STAGE_PROGRAM_HEAD_TYPE;
	m_OldStageProgInfoHdr.wScanType = SCAN_TYPE_STANDARD;
// 2009.10.13 bagus StagePGM �C�� --}--

	if ( m_nOpenMode == modeNew ) {
		//strcpy(m_StageProgInfoHdr.hdr.szName, "Untitled");
		//Saiki 20090528 Change ----->
		//strcpy(m_StageProgInfoHdr.hdr.szName, "�^�C�g���Ȃ�");
		CString strBuffer;
		LoadStringML(IDS_UNTITLE_NAME, strBuffer, "Untitled");
		strcpy(m_StageProgInfoHdr.hdr.szName, strBuffer);
// 2009.06.11 K.Matsuo -->
		strcpy(m_OldStageProgInfoHdr.hdr.szName, strBuffer);
// 2009.06.11 K.Matsuo <--
		//Saiki 20090528 Change <-----
	}

	return TRUE;
}

// ==========================================================================
//
void CRecipeStageProgramView::SetData()
{
	char szBuff[256];

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	///// Name /////
	SetDlgItemText(IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName);

	///// Sample - Name /////
	SetDlgItemText(IDC_SAMPLE_NAME, m_StageProgInfoHdr.SampleInfo.szName);

	///// Sample - Shape /////
	m_StageProgInfoHdr.SampleInfo.nShape = m_StageProgInfoHdr.SampleInfo.nShape >= SAMPLE_SHAPE_MAX ? 0 : m_StageProgInfoHdr.SampleInfo.nShape;
	// Kojika 20090528 Change
	//SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape[m_StageProgInfoHdr.SampleInfo.nShape]);
	if(l_SystemConfig.nLanguage == 0){
		SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape_ENU[m_StageProgInfoHdr.SampleInfo.nShape]);
	}
	else{
		SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape_JPN[m_StageProgInfoHdr.SampleInfo.nShape]);
	}
	// Kojika 20090528 Change End

	///// Sample - Thickness /////
	sprintf(szBuff, "%.3lf", m_StageProgInfoHdr.SampleInfo.dThickness /MICROMETRE);
	SetDlgItemText(IDC_SAMPLE_THICKNESS, szBuff);

	///// Sample - Size X /////
	sprintf(szBuff, "%.3lf", m_StageProgInfoHdr.SampleInfo.Size.dx /MICROMETRE);
	SetDlgItemText(IDC_SAMPLE_SIZE_X, szBuff);

	///// Sample - Size Y /////
	sprintf(szBuff, "%.3lf", m_StageProgInfoHdr.SampleInfo.Size.dy /MICROMETRE);
	SetDlgItemText(IDC_SAMPLE_SIZE_Y, szBuff);

	///// Sample - Origin X /////
	sprintf(szBuff, "%.3lf", ((double)m_StageProgInfoHdr.SampleInfo.Origin.lX /MICROMETRE));
	SetDlgItemText(IDC_SAMPLE_ORIGIN_X, szBuff);

	///// Sample - Origin Y /////
	sprintf(szBuff, "%.3lf", ((double)m_StageProgInfoHdr.SampleInfo.Origin.lY /MICROMETRE));
	SetDlgItemText(IDC_SAMPLE_ORIGIN_Y, szBuff);

	///// Point Number /////
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);

	///// Point Coord /////
	// SAMPLE���_����̑��΍��W�Ƃ���
	for ( int i = 0 ; i < m_iNumScans ; i++ ) {
		// X
		sprintf(szBuff, "%.3lf", ((double)m_ScanPoint[i].lX) / MICROMETRE );
		m_MeasurementPointGrid.GridDisp(i + 1, 1, szBuff, DT_RIGHT);

		// Y
		sprintf(szBuff, "%.3lf", ((double)m_ScanPoint[i].lY) / MICROMETRE );
		m_MeasurementPointGrid.GridDisp(i + 1, 2, szBuff, DT_RIGHT);
	}

	///// Comment /////
	SetDlgItemText(IDC_COMMENT, m_StageProgInfoHdr.hdr.szComment);

	///// Deskew Mode /////
	// Kojika 20090528 Change
	//strcpy(szBuff, g_lpszDeskewMode[m_StageProgInfoHdr.nDeskewMode]);
	if(l_SystemConfig.nLanguage == 0){
		strcpy(szBuff, g_lpszDeskewMode_ENU[m_StageProgInfoHdr.nDeskewMode]);
	}
	else{
		strcpy(szBuff, g_lpszDeskewMode_JPN[m_StageProgInfoHdr.nDeskewMode]);
	}
	// Kojika 20090528 Change End
	SetDlgItemText(IDC_STAGE_DESKEW_MODE, szBuff);

	///// SitePattern Mode /////
	// Kojika 20090528 Change
	//strcpy(szBuff, g_lpszSitePatternMode[m_StageProgInfoHdr.nSitePatternMode]);
	if(l_SystemConfig.nLanguage == 0){
		strcpy(szBuff, g_lpszSitePatternMode_ENU[m_StageProgInfoHdr.nSitePatternMode]);
	}
	else{
		strcpy(szBuff, g_lpszSitePatternMode_JPN[m_StageProgInfoHdr.nSitePatternMode]);
	}
	// Kojika 20090528 Change End
	SetDlgItemText(IDC_STAGE_SITEPATTERN_MODE, szBuff);
}

// ==========================================================================
//
void CRecipeStageProgramView::UpDate()
{
	char szGridText[1024];
	//char *stopstring;
	CString strBuffer;
	int Valid_X;
	int Valid_Y;
	BOOL bShape = FALSE;
	int i;

// 2009.10.24 bagus 2�_�� �C�� --{--
	long lTmp;
// 2009.10.24 bagus 2�_�� �C�� --}--

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	///// Name /////
	GetDlgItemText(IDC_STAGE_PROGRAM_NAME, strBuffer);
	strBuffer.Insert(RECIPE_NAME_LEN, '\0');
	strcpy(m_StageProgInfoHdr.hdr.szName, strBuffer);

	///// Sample Name /////
	GetDlgItemText(IDC_SAMPLE_NAME, strBuffer);
	strBuffer.Insert(RECIPE_NAME_LEN, '\0');
	strcpy(m_StageProgInfoHdr.SampleInfo.szName, strBuffer);

	///// Sample -Shape /////
	GetDlgItemText(IDC_SAMPLE_SHAPE, strBuffer);
	for ( int i = 0 ; i < SAMPLE_SHAPE_MAX ; i ++ ) {
		// Kojika 20090528 Change
		//if ( strBuffer == g_lpszSampleShape[i] ) {
		//	m_StageProgInfoHdr.SampleInfo.nShape = i;
		//	bShape = TRUE;
		//}
		if(l_SystemConfig.nLanguage == 0){
			if ( strBuffer == g_lpszSampleShape_ENU[i] ) {
				m_StageProgInfoHdr.SampleInfo.nShape = i;
				bShape = TRUE;
			}
		}
		else{
			if ( strBuffer == g_lpszSampleShape_JPN[i] ) {
				m_StageProgInfoHdr.SampleInfo.nShape = i;
				bShape = TRUE;
			}
		}
		// Kojika 20090528 Change End
	}
	if ( !bShape ) {
		//MessageBox("The Shape not Existing is Inputted.", m_strCaption, MB_OK | MB_ICONSTOP);
		//Saiki 20090528 Change ----->
		//MessageBox("���݂��Ȃ��T���v��������͂���Ă��܂�", m_strCaption, MB_OK | MB_ICONSTOP);
		CString strBuffer;
		LoadStringML(IDS_SHAPE_NOT_EXIST_INP, strBuffer, "The Shape not Existing is Inputted.");
		MessageBox(strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		//Saiki 20090528 Change <-----
	}

	///// Thickness /////
	GetDlgItemText(IDC_SAMPLE_THICKNESS, strBuffer);
	m_StageProgInfoHdr.SampleInfo.dThickness = atof(strBuffer) * MICROMETRE;

	///// Sample - Size X /////
	GetDlgItemText(IDC_SAMPLE_SIZE_X, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Size.dx = atof(strBuffer) * MICROMETRE;

	///// Sample - Size Y /////
	GetDlgItemText(IDC_SAMPLE_SIZE_Y, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Size.dy = atof(strBuffer) * MICROMETRE;

	///// Sample - Origin X /////
	GetDlgItemText(IDC_SAMPLE_ORIGIN_X, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Origin.lX = static_cast<long>(atof(strBuffer) * MICROMETRE);

	///// Sample - Origin Y /////
	GetDlgItemText(IDC_SAMPLE_ORIGIN_Y, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Origin.lY = static_cast<long>(atof(strBuffer) * MICROMETRE);

	///// Point /////
	m_iNumScans = 0;
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	for ( i = 0 ; i < SCAN_POINT_MAX ; i++ ) {
		Valid_X = Valid_Y = 0;
		///// X /////
		strcpy(szGridText, m_MeasurementPointGrid.GetItemText(i + 1, 1));
		Valid_X = strlen(szGridText);
		if ( Valid_X ) {
// 2009.10.24 bagus 2�_�� �C�� --{--
//			m_ScanPoint[i].lX = static_cast<long>(atof(szGridText) * MICROMETRE);
			lTmp = static_cast<long>(atof(szGridText) * MICROMETRE);
			if (!CheckValidPointX(lTmp))
				break;
			m_ScanPoint[i].lX = lTmp;
// 2009.10.24 bagus 2�_�� �C�� --}--
			//m_ScanPoint[i].lX = static_cast<long>((strtol(szGridText, &stopstring, 10)) * MICROMETRE);

			///// Refresh X /////
			strBuffer.Format("%.3lf", ((double)m_ScanPoint[i].lX) / MICROMETRE );
			m_MeasurementPointGrid.GridDisp(i + 1, 1, strBuffer, DT_RIGHT);
		}

		///// Y /////
		strcpy(szGridText, m_MeasurementPointGrid.GetItemText(i + 1, 2));
		Valid_Y = strlen(szGridText);
		if ( Valid_Y ) {
// 2009.10.24 bagus 2�_�� �C�� --{--
//			m_ScanPoint[i].lY = static_cast<long>(atof(szGridText) * MICROMETRE);
			lTmp = static_cast<long>(atof(szGridText) * MICROMETRE);
			if (!CheckValidPointY(lTmp))
				break;
			m_ScanPoint[i].lY = lTmp;
// 2009.10.24 bagus 2�_�� �C�� --}--
			//m_ScanPoint[i].lY = static_cast<long>((strtol(szGridText, &stopstring, 10)) * MICROMETRE);

			///// Refresh Y /////
			strBuffer.Format("%.3lf", ((double)m_ScanPoint[i].lY) / MICROMETRE );
			m_MeasurementPointGrid.GridDisp(i + 1, 2, strBuffer, DT_RIGHT);
		}

		///// Judgement /////
		if ( Valid_X && Valid_Y )
			m_iNumScans++;
		else
			break;
	}

	///// Clear Remaining Grids /////
	for ( ; i < m_MeasurementPointGrid.GetRowCount(); i++ ) {
		m_MeasurementPointGrid.GridDisp(i + 1, 1, "", DT_RIGHT);
		m_MeasurementPointGrid.GridDisp(i + 1, 2, "", DT_RIGHT);
	}

	///// Point Count /////
	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;

	///// Comment /////
	GetDlgItemText(IDC_COMMENT, m_StageProgInfoHdr.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// ==========================================================================
//
BOOL CRecipeStageProgramView::SaveRecipeData()
{
	char szFilePath[MAX_PATH + 1];
	char szFilePathBackUp[MAX_PATH + 1];
	char szOriginalFilePath[MAX_PATH + 1];
	char szNewFilePath[MAX_PATH + 1];

	// �V�������V�s����ݒ�iSave, SaveAs���p�j
	strcpy(m_StageProgInfoHdr.hdr.szName, m_szRecipeName);

	if ( strcmp(m_OldStageProgInfoHdr.hdr.szName, m_szRecipeName) != 0 ) {
		// �V�������V�s����ݒ�iSaveAs��p�j

		// �����V�s�̃p�^�[���F���摜��V�������V�s���ō쐬
		// �i�p�^�[���F�����ҏW�r�������m��Ȃ��̂ŁA���V�s�ҏW���̓��e�ō쐬����j
		_stprintf(szOriginalFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		_stprintf(szNewFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_szRecipeName);
// 2009.06.12 K.Matsuo -->
		::DeleteFile(szNewFilePath);
// 2009.06.12 K.Matsuo <--
		::CopyFile(szOriginalFilePath, szNewFilePath, FALSE);
		_stprintf(szOriginalFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		_stprintf(szNewFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_szRecipeName);
// 2009.06.12 K.Matsuo -->
		::DeleteFile(szNewFilePath);
// 2009.06.12 K.Matsuo <--
		::CopyFile(szOriginalFilePath, szNewFilePath, FALSE);

		// �����V�s�̃p�^�[���F���摜�i�ޔ��j�̖߂�
		// �i�p�^�[���F�����ҏW�r�������m��Ȃ��̂ŁA���V�s�ҏW�O�̏�Ԃ܂Ŗ߂��j
		_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
		_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		::CopyFile(szFilePathBackUp, szFilePath, FALSE);
		_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
		_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		::CopyFile(szFilePathBackUp, szFilePath, FALSE);
	}

	// �p�^�[���F���摜�̃o�b�N�A�b�v���쐬
	// �i�����A���̌�̑���ŁA�p�^�[���F���ҏW�˕ۑ��L�����Z�������ꍇ�A���̃o�b�N�A�b�v���_�̓��e�ŏ����߂��j
	_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_szRecipeName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);
	_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_szRecipeName);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	::CopyFile(szFilePath, szFilePathBackUp, FALSE);

	if ( !RecipeFile_SaveRecipe(&m_StageProgInfoHdr, m_StageProgInfoHdr.hdr.szName, RECIPE_FILE_STAGE_PROGRAM) )
		return FALSE;

	if ( !RecipeFile_SavePointList(m_ScanPoint, m_StageProgInfoHdr.wNumScans, m_StageProgInfoHdr.hdr.szName) )
		return FALSE;

	if ( !PatternFile_SavePointList(m_SitePoint, m_StageProgInfoHdr.wNumScans, m_StageProgInfoHdr.hdr.szName) )
		return FALSE;

	//2009.12.10 bagus Recipe Backup --{--
	USER_DATA l_UserData;
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);
	ConfigFile_GetNanoSpecIni(&l_UserData,CONFIG_FILE_USER_SETTING);
	if(l_SystemConfig.bBackup){
		if(l_UserData.BackupData.bAutoBackup && strlen(l_UserData.BackupData.BackupPath) > 0 ){
			//�����o�b�N�A�b�v���L���Ȃ̂ł����ŉ摜�t�@�C�����o�b�N�A�b�v����
			TCHAR fullPath[MAX_PATH];
			CString strSourceImage;
			CString strBackupPath;
			BOOL bCreateDir = TRUE;
			_fullpath(fullPath,l_UserData.BackupData.BackupPath,sizeof(fullPath));
			_tcscat(fullPath,"\\" DB_DIR_NAME "\\" DB_DESKEW_IMG_DIR_NAME "\\" );
			for(int n = 0;n < static_cast<int>(strlen(fullPath));n++){
				if(n > 0 && fullPath[n] == '\\' && fullPath[n-1] != ':'){
					fullPath[n] = '\0';
					bCreateDir = CreateDirectory(fullPath,NULL);
					if(!bCreateDir){
						if(GetLastError() == ERROR_ALREADY_EXISTS){
							bCreateDir = TRUE;
						}
					}
					fullPath[n] = '\\';
					if(!bCreateDir)
						break;
				}
			}
			if(bCreateDir){
				strSourceImage.Format("%s%s.D1",g_szDb_Deskew_Img_Dir,m_szRecipeName);
				strBackupPath.Format("%s%s.D1",fullPath,m_szRecipeName);
				CopyFile(szFilePath,strBackupPath.GetBuffer(0),FALSE);
				strSourceImage.Format("%s%s.D2",g_szDb_Deskew_Img_Dir,m_szRecipeName);
				strBackupPath.Format("%s%s.D2",fullPath,m_szRecipeName);
				CopyFile(szFilePath,strBackupPath.GetBuffer(0),FALSE);
			}
		}
	}
	//2009.12.10 bagus Recipe Backup --}--

	SetDlgItemText(IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName);

	///// Point Number /////
	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);

	::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
	::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
	::CopyMemory(m_OldSitePoint, m_SitePoint, sizeof(m_SitePoint));

// 2009.06.12 K.Matsuo -->
	CRect rect;
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetClientRect(rect);
	InvalidateRect(rect);
// 2009.06.12 K.Matsuo <--

	return TRUE;
}

// ==========================================================================
//
BOOL CRecipeStageProgramView::CheckData()
{
	TCHAR szBuff[256];
//	double dMinX, dMaxX, dMinY, dMaxY;
	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;
	//Saiki 20090528 Add ----->
	CString strBuffer;
	//Saiki 20090528 Add <-----

	UpDate();

// 2009.06.12 K.Matsuo -->
	if ( m_iNumScans <= 0 ) {
		LoadStringML(IDS_NO_MEASUREMENT_POINT, strBuffer, "There is no specification measurement point.");
		MessageBox(strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
		return FALSE;
	}
// 2009.06.12 K.Matsuo <--

	///// �X�e�[�W�̑傫���Ń��~�b�g�l��ݒ� /////
//	dMaxX = (m_StageProgInfoHdr.SampleInfo.Size.dx/2);			// (um)
//	dMinX = -dMaxX;
//	dMaxY = (m_StageProgInfoHdr.SampleInfo.Size.dy/2);			// (um)
//	dMinY = -dMaxY;

	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
	}

	switch ( m_StageConfig.OrgPos ) {
	case 1: // ���S
		coordLeftTop.lX = static_cast<long>(dSampleSizeX / 2 * -1);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY / 2);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX / 2);
		coordRightBottom.lY = static_cast<long>(dSampleSizeY / 2 * -1);
		break;
	case 2: // ����
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX);
		coordRightBottom.lY = static_cast<long>(dSampleSizeY * -1);
		break;
	case 3: // ����
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX);
		coordRightBottom.lY = 0;
		break;
	case 4: // �E��
		coordLeftTop.lX = static_cast<long>(dSampleSizeX * -1);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = 0;
		break;
	case 5: // �E��
		coordLeftTop.lX = static_cast<long>(dSampleSizeX * -1);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = static_cast<long>(dSampleSizeY * -1);
		break;
	}

	// �X�e�[�W�R���t�B�O�̎��ݒ�ƍ��킹��
	NS_ConvertToStageConfigXyAxis(&coordLeftTop);
	NS_ConvertToStageConfigXyAxis(&coordRightBottom);

	///// �O���b�h�̒l���擾 /////
	for ( int i = 0; i < m_iNumScans; i++ ) {
		///// X /////
//		if ( !(m_ScanPoint[i].lX >= dMinX && m_ScanPoint[i].lX <= dMaxX) ) {
		if ( !(m_ScanPoint[i].lX >= min(coordLeftTop.lX, coordRightBottom.lX) && m_ScanPoint[i].lX <= max(coordLeftTop.lX, coordRightBottom.lX)) ) {
			//sprintf(szBuff, "POINT = %d : X coordinate is out of limit.", i + 1);
			//Saiki 20090528 Change ----->
			//sprintf(szBuff, "�|�C���g = %d : X ���W���͈͊O�ł�", i + 1);
		LoadStringML(IDS_X_COORDINATE_OUT_LIMIT, strBuffer, "POINT = %d : X coordinate is out of limit.");
		sprintf(szBuff, (LPCSTR)strBuffer, i + 1);
			//Saiki 20090528 Change <-----
			MessageBox(szBuff, m_strCaption, MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		///// Y /////
//		if ( !(m_ScanPoint[i].lY >= dMinY && m_ScanPoint[i].lY <= dMaxY) ) {
		if ( !(m_ScanPoint[i].lY >= min(coordLeftTop.lY, coordRightBottom.lY) && m_ScanPoint[i].lY <= max(coordLeftTop.lY, coordRightBottom.lY)) ) {
			//sprintf(szBuff, "POINT = %d : Y coordinate is out of limit.", i + 1);
			//Saiki 20090528 Change ----->
			//sprintf(szBuff, "�|�C���g = %d : Y ���W���͈͊O�ł�", i + 1);
		LoadStringML(IDS_Y_COORDINATE_OUT_LIMIT, strBuffer, "POINT = %d : Y coordinate is out of limit.");
		sprintf(szBuff, (LPCSTR)strBuffer, i + 1);
			//Saiki 20090528 Change <-----
			MessageBox(szBuff, m_strCaption, MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}

	return TRUE;
}

// ==========================================================================
//
BOOL CRecipeStageProgramView::IsDataChanged()
{
	UpDate();

	//����|�C���g���܂łŕω����������ꍇ�͍X�V���ꂽ�Ƃ݂Ȃ�
	return ( memcmp(&m_StageProgInfoHdr, &m_OldStageProgInfoHdr, sizeof(m_StageProgInfoHdr)) != 0 ||
				memcmp(m_ScanPoint, m_OldScanPoint, (sizeof(m_ScanPoint) / 10000 * m_StageProgInfoHdr.wNumScans)) != 0 ||
				memcmp(m_SitePoint, m_OldSitePoint, (sizeof(m_SitePoint) / 10000 * m_StageProgInfoHdr.wNumScans)) != 0 );
}

// ==========================================================================
//
void CRecipeStageProgramView::OnPaint()
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	long lDrawStageSizeX, lDrawStageSizeY;
	CRect StageRect;
//	CRect SampleRect;

	///// Region Map /////
	CWnd* pPictBox = GetDlgItem(IDC_MEASUREMENT_MAP);
	CDC* pDC = pPictBox->GetDC();
	pPictBox->GetClientRect(&StageRect);

	///// Draw Stage Color /////
	CBrush BrushStage(BLACK_COLOR);
	m_oldBrush = pDC->SelectObject(&BrushStage);
	CPen PenStage(PS_SOLID, 1, BLACK_COLOR);
	m_oldPen = pDC->SelectObject(&PenStage);

	///// Stage Rect Offset /////
	long StageRectLeft = StageRect.left + STAGE_OFFSET;
	long StageRectRight = StageRect.right - STAGE_OFFSET;
	long StageRectTop = StageRect.top + STAGE_OFFSET;
	long StageRectBottom = StageRect.bottom - STAGE_OFFSET;

	///// �X�e�[�W�L���͈� /////
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
// 2013.01.17 bagus rotate xy view <--
// 	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
// 	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

	double dStageSizeX;
	double dStageSizeY;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		break;
	case 90:
	case 270:
		dStageSizeX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		dStageSizeY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		break;
	}
// 2013.01.17 bagus rotate xy view <--

	if((dStageSizeX / dStageSizeY) > ((double)(StageRectRight - StageRectLeft) / (double)(StageRectBottom - StageRectTop))) {
		lDrawStageSizeX =(StageRectRight - StageRectLeft);
//		  lDrawStageSizeY = static_cast<long>(((StageRectRight - StageRectLeft) * dStageMaxSizeY / dStageMaxSizeX));
		lDrawStageSizeY = static_cast<long>(((StageRectRight - StageRectLeft) * dStageSizeY / dStageSizeX));

		m_dDrawStartStagePosX = StageRectLeft;
		m_dDrawStartStagePosY = StageRectTop + ((StageRectBottom - StageRectTop - lDrawStageSizeY) / 2);
		m_dDrawEndStagePosX = StageRectRight;
		m_dDrawEndStagePosY = m_dDrawStartStagePosY + lDrawStageSizeY;
	}
	else{
		lDrawStageSizeX = static_cast<long>(((StageRectBottom - StageRectTop) * dStageSizeX / dStageSizeY));
		lDrawStageSizeY = static_cast<long>((StageRectBottom - StageRectTop));

		m_dDrawStartStagePosX = StageRectLeft + ((StageRectRight - StageRectLeft - lDrawStageSizeX) / 2);
		m_dDrawStartStagePosY = StageRectTop;
		m_dDrawEndStagePosX = m_dDrawStartStagePosX + lDrawStageSizeX;
		m_dDrawEndStagePosY = StageRectBottom;
	}

	pDC->Rectangle(CRect(static_cast<int>(m_dDrawStartStagePosX), static_cast<int>(m_dDrawStartStagePosY), static_cast<int>(m_dDrawEndStagePosX), static_cast<int>(m_dDrawEndStagePosY)));
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);

	///// Draw Sample /////
	DrawSample(pDC, m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

	///// Draw Origin Point /////
// 2013.01.09 bagus -->
//	DrawOriginMaker(m_StageConfig.OrgPos);
// 2013.01.09 bagus <--

	///// Draw AxisCoordinate /////
	DrawAxisCoordinate(m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

	SamplePointGraph_DataSet();

// �`��p���b�Z�[�W�Ƃ��� CNanoRecipeUI::OnPaint() ���Ăяo���Ă͂����܂���
	ReleaseDC(pDC);
}

// 2013.01.09 bagus -->
// �����W�iSTAGEAPI�p�j�ɕϊ�
void CRecipeStageProgramView::ToXyAxisCoord(STAGE_COORD* lpPoint)
{
	// X: RIGHT
	if ( m_StageConfig.Dir.X == DIR_RIGHT ) {
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: LEFT
	if ( m_StageConfig.Dir.X == DIR_LEFT ) {
		lpPoint->lX *= -1;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_TOP:
			break;
		case DIR_BOTTOM:
			lpPoint->lY *= -1;
			break;
		default:
			break;
		}
	}

	// X: TOP
	if ( m_StageConfig.Dir.X == DIR_TOP ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lY = ptTemp.lX;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_RIGHT:
			lpPoint->lX = ptTemp.lY;
			break;
		case DIR_LEFT:
			lpPoint->lX = ptTemp.lY * -1;
			break;
		default:
			break;
		}
	}

	// X: BOTTOM
	if ( m_StageConfig.Dir.X == DIR_BOTTOM ) {
		STAGE_COORD ptTemp;

		ptTemp = *lpPoint;

		lpPoint->lY = ptTemp.lX * -1;
		switch ( m_StageConfig.Dir.Y ) {
		case DIR_RIGHT:
			lpPoint->lX = ptTemp.lY;
			break;
		case DIR_LEFT:
			lpPoint->lX = ptTemp.lY * -1;
			break;
		default:
			break;
		}
	}
}
// 2013.01.09 bagus <--

// ==========================================================================
//
void CRecipeStageProgramView::DrawSample(CDC* pDC, int iOrg, int DirX, int DirY)
{
	WINDOWPLACEMENT Stageplacement, Sampleplacement;

	///// Region Map /////
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetWindowPlacement(&Stageplacement);

	///// �X�e�[�W�L���͈�(�����l�p�͂��̃T�C�Y�ŕ`�悳��Ă���) /////
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
// 2013.01.17 bagus rotate xy view <--
// 	double dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
// 	double dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);

	double dStageSizeX;
	double dStageSizeY;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		dStageSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		dStageSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		break;
	case 90:
	case 270:
		dStageSizeX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
		dStageSizeY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
		break;
	}
// 2013.01.17 bagus rotate xy view <--

// 2013.01.09 bagus -->
	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
	}

// 2013.01.17 bagus rotate xy view <--
	double dTemp;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		break;
	case 90:
	case 270:
		dTemp = dSampleSizeX;
		dSampleSizeX = dSampleSizeY;
		dSampleSizeY = dTemp;
		break;
	}
// 2013.01.17 bagus rotate xy view <--

	///// �@�T���v���T�C�Y���X�e�[�W�̑傫���Ƃ菬�������`�F�b�N /////
// 	if ((m_StageProgInfoHdr.SampleInfo.Size.dx > dStageSizeX) || (m_StageProgInfoHdr.SampleInfo.Size.dy > dStageSizeY))
// 		return;
	if ((dSampleSizeX > dStageSizeX) || (dSampleSizeY > dStageSizeY))
		return;

	//�X�e�[�W��Origin Position���X�e�[�W�k�����ɍ��킹�����W�ɕϊ�

// 2009.05.25 K.Matsuo StageConfigSetting SPT -->

	// StageConfigSetting��m_StageConfig.Origin�̎戵���ɂ��Đ������Ă���

	//	 �C���O�FMachineOrgSensorLoc �i���W�I�{�^���ō����Ȃǂ��w��j�̊�ʒu����̃I�t�Z�b�g�l�B��ʒu�ɂ��A�I�t�Z�b�g�l�̉��Z�E���Z���s��
	//	 �C����FMachineOrgSensorLoc�́A�p�~�B������i�Œ�j�Ƃ��A�}�V�[���X�e�[�W���_���P�[�V�����̍��W�ʒu�Ƃ��ė��p����
// 2009.05.25 K.Matsuo StageConfigSetting SPT <--

	///// �T���v���`��T�C�Y�̌v�Z�i�X�e�[�W�`��T�C�Y�΂���T���v���`��T�C�Y���v�Z�j /////
// 	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * m_StageProgInfoHdr.SampleInfo.Size.dx / dStageMaxSizeX;
// 	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * m_StageProgInfoHdr.SampleInfo.Size.dy / dStageMaxSizeY;
	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * dSampleSizeX / dStageSizeX;
	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * dSampleSizeY / dStageSizeY;
// 2013.01.09 bagus <--

	///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l /////
	double dTotalOrgOffsetPosX, dTotalOrgOffsetPosY;

	///// �T���v���̕`��ʒu /////
	double dtop, dbottom, dright, dleft;

// 2013.01.09 bagus -->
	STAGE_COORD XyAxisCoord;

	XyAxisCoord.lX = m_StageProgInfoHdr.SampleInfo.Origin.lX;
	XyAxisCoord.lY = m_StageProgInfoHdr.SampleInfo.Origin.lY;
	ToXyAxisCoord(&XyAxisCoord);
// 2013.01.09 bagus <--

	//Sample�`��ʒu�ݒ�
	//�l�p�`��T���v���̂ݑΉ�
	switch(m_StageProgInfoHdr.SampleInfo.nShape){
	case SAMPLE_SHAPE_RECTANGLE:		//Sample shaped RectAngle
		switch(iOrg){
		case 1: ///// �Z���^�[�	//X���F��+�@Y���F��+�̂ݑΉ�
		default:
			///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l���v�Z /////
			//dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			//dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			//dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
			//dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2013.01.09 bagus -->
// 			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2013.01.17 bagus rotate xy view <--
// 			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (XyAxisCoord.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (XyAxisCoord.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 90:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 180:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 270:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--

			///// �T���v���`��ʒu���v�Z /////
			//���
			dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX / 2);
			dright	= dleft + dDrawSampleSizeX;
			//���
			dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + (dDrawSampleSizeY / 2);
			dtop	= dbottom - dDrawSampleSizeY;
			break;

		case 2: /////LeftTop
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
			///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l���v�Z /////
// 2009.09.11 bagus stress �C�� --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			//dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			//dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->
// 			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2013.01.17 bagus rotate xy view <--
// 			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 90:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 180:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 270:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
// 2013.01.17 bagus rotate xy view -->
// 			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
// 			dright	= dleft + dDrawSampleSizeX;
// 			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
// 			dbottom	= dtop + dDrawSampleSizeY;
			//���㌴�_�ƂȂ�悤�ɍ���i�`��J�n�ʒu�j���Z�o
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 90:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - dDrawSampleSizeX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 180:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 270:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
			break;
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
		case 3: /////LeftBottom
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
			///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l���v�Z /////
// 2009.09.11 bagus stress �C�� --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			//dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			//dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->
// 			dTotalOrgOffsetPosX = ((m_StageProgInfoHdr.SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX);
// 			dTotalOrgOffsetPosY = ((m_StageProgInfoHdr.SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY);
// 2013.01.17 bagus rotate xy view -->
// 			dTotalOrgOffsetPosX = ((XyAxisCoord.lX - (m_StageConfig.StageMaxCoord.dLeftX - (XyAxisCoord.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX);
// 			dTotalOrgOffsetPosY = ((XyAxisCoord.lY - (m_StageConfig.StageMaxCoord.dDownY - (XyAxisCoord.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY);
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 90:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 180:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 270:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
// 2013.01.17 bagus rotate xy view -->
// 			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
// 			dright	= dleft + dDrawSampleSizeX;// - dTotalOrgOffsetPosX;
// 			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
// 			dtop	= dbottom - dDrawSampleSizeY;// + dTotalOrgOffsetPosY;
			//�������_�ƂȂ�悤�ɍ���i�`��J�n�ʒu�j���Z�o
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 90:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 180:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 270:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX);
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
			break;
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
		case 4: /////RightBottom
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
			///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l���v�Z /////
// 2009.09.11 bagus stress �C�� --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			//dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			//dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->
// 			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2013.01.17 bagus rotate xy view -->
// 			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 90:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 180:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 270:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
// 2013.01.17 bagus rotate xy view -->
// 			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
// 			dleft	= dright - dDrawSampleSizeX;
// 			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
// 			dtop	= dbottom - dDrawSampleSizeY;
			//�E�����_�ƂȂ�悤�ɍ���i�`��J�n�ʒu�j���Z�o
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX);
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 90:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 180:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX + dDrawSampleSizeX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 270:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX);
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
			break;
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
		case 5: /////RightTop
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
			///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l���v�Z /////
// 2009.09.11 bagus stress �C�� --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --{--
//			dTotalOrgOffsetPosX = (m_SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			//dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			//dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2013.01.09 bagus -->
// 			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
// 2013.01.17 bagus rotate xy view -->
// 			dTotalOrgOffsetPosX = (XyAxisCoord.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
// 			dTotalOrgOffsetPosY = (XyAxisCoord.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 90:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 180:
				dTotalOrgOffsetPosX = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosY;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			case 270:
				dTotalOrgOffsetPosX = XyAxisCoord.lY - m_StageConfig.StageMaxCoord.dDownY;
				dTotalOrgOffsetPosX = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - dTotalOrgOffsetPosX;
				dTotalOrgOffsetPosX *= (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageSizeX;
				dTotalOrgOffsetPosY = XyAxisCoord.lX - m_StageConfig.StageMaxCoord.dLeftX;
				dTotalOrgOffsetPosY *= (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
// 2013.01.17 bagus rotate xy view -->
// 			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
// 			dleft	= dright - dDrawSampleSizeX;
// 			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
// 			dbottom	= dtop + dDrawSampleSizeY;
			//�E�㌴�_�ƂȂ�悤�ɍ���i�`��J�n�ʒu�j���Z�o
			switch ( m_StageConfig.RotateXyView ) {
			case 0:
			default:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX);
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 90:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX - (dDrawSampleSizeX);
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY - dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 180:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX + dDrawSampleSizeX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			case 270:
				dleft	= m_dDrawStartStagePosX + dTotalOrgOffsetPosX;
				dright	= dleft + dDrawSampleSizeX;
				dbottom = m_dDrawEndStagePosY - dTotalOrgOffsetPosY + dDrawSampleSizeY;
				dtop	= dbottom - dDrawSampleSizeY;
				break;
			}
// 2013.01.17 bagus rotate xy view <--
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
			break;
		}

		if ((m_dDrawStartStagePosX <= dleft) && (dright <= m_dDrawEndStagePosX) && (m_dDrawStartStagePosY <= dtop) && (dbottom <= m_dDrawEndStagePosY)){
			Sampleplacement.rcNormalPosition.left = static_cast<long>(Stageplacement.rcNormalPosition.left + dleft);
			Sampleplacement.rcNormalPosition.right = static_cast<long>(Stageplacement.rcNormalPosition.left + dright);
			Sampleplacement.rcNormalPosition.top = static_cast<long>(Stageplacement.rcNormalPosition.top  + dtop);
			Sampleplacement.rcNormalPosition.bottom = static_cast<long>(Stageplacement.rcNormalPosition.top  + dbottom);
			//�l�pSample�`��
			m_SamplePointGraph.SetWindowPlacement(&Sampleplacement);
			m_SamplePointGraph.ShowWindow(SW_SHOW);
		}
		else{
			m_SamplePointGraph.ShowWindow(SW_HIDE);
		}

	}
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);
}

// 2013.01.17 bagus rotate xy view -->
// ==========================================================================
//
long CRecipeStageProgramView::RotateXyAxisDir(long lDir)
{
	long lRet;

	lRet = lDir;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
		break;
	case 90:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_TOP:
			lRet = DIR_RIGHT;
			break;
		case DIR_LEFT:
			lRet = DIR_TOP;
			break;
		case DIR_BOTTOM:
			lRet = DIR_LEFT;
			break;
		default:
			break;
		}
		break;
	case 180:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_LEFT;
			break;
		case DIR_TOP:
			lRet = DIR_BOTTOM;
			break;
		case DIR_LEFT:
			lRet = DIR_RIGHT;
			break;
		case DIR_BOTTOM:
			lRet = DIR_TOP;
			break;
		default:
			break;
		}
		break;
	case 270:
		switch ( lDir ) {
		case DIR_RIGHT:
			lRet = DIR_TOP;
			break;
		case DIR_TOP:
			lRet = DIR_LEFT;
			break;
		case DIR_LEFT:
			lRet = DIR_BOTTOM;
			break;
		case DIR_BOTTOM:
			lRet = DIR_RIGHT;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return lRet;
}
// 2013.01.17 bagus rotate xy view <--

// ==========================================================================
//
void CRecipeStageProgramView::DrawAxisCoordinate(int iOrgPos, int DirX, int DirY)
{
#define ARROW_LENGTH	18
#define ARROW_SIDE	6

	POINT PT[3];
	CString csBuff;
	//Saiki 20090528 Add ----->
	CString strBuffer;
	//Saiki 20090528 Add <-----

	CWnd* CoordBox = GetDlgItem(IDC_COORDINATE);
	CDC* pDC = CoordBox->GetDC();
	CRect StageRect;
	CoordBox->GetClientRect(&StageRect);

	CBrush BrushStage(WAFER_COLOR);
	CBrush BrushArrow(BLACK_COLOR);
	CBrush BrushOrg(GREEN_COLOR);
	CPen PenStage(PS_SOLID, 1, WAFER_COLOR);
	CPen PenArrow(PS_SOLID, 1, BLACK_COLOR);
	CPen PenOrg(PS_SOLID, 1, GREEN_COLOR);

	CBrush* oldBrush = pDC->SelectObject(&BrushStage);
	CPen* oldPen = pDC->SelectObject(&PenStage);

	long StageRectLeft = StageRect.left + STAGE_OFFSET;
	long StageRectRight = StageRect.right - STAGE_OFFSET;
	long StageRectTop = StageRect.top + STAGE_OFFSET;
	long StageRectBottom = StageRect.bottom - STAGE_OFFSET;

	///// Draw Stage /////
	pDC->Rectangle(CRect(StageRectLeft, StageRectTop, StageRectRight , StageRectBottom));

	///// Font Setting /////
	TEXTMETRIC tm;
	GetTextMetrics(pDC->GetSafeHdc(), &tm);
	//Saiki 20090528 Change ----->
	//if( pDC->SelectObject( CFont::FromHandle((HFONT) GetStockObject(ANSI_FIXED_FONT) ) ) == NULL )
	//	//MessageBox("Error in SelectObject");
	//	MessageBox("�I�������I�u�W�F�N�g���G���[�ł�");
	if( pDC->SelectObject( CFont::FromHandle((HFONT) GetStockObject(ANSI_FIXED_FONT) ) ) == NULL ){
		//MessageBox("Error in SelectObject");
		LoadStringML(IDS_ERROR_SELECT_OBJECT, strBuffer, "Error in SelectObject");
		MessageBox(strBuffer);
	}
	//Saiki 20090528 Change <-----

	///// �w�i�F�𓧖��ɐݒ� /////
	pDC->SetBkMode(TRANSPARENT);

	///// Draw Center Position /////
	long CenterPosX = StageRectLeft + ((StageRectRight - StageRectLeft) / 2);
	long CenterPosY = StageRectTop + ((StageRectBottom - StageRectTop) / 2);

	pDC->SelectObject(&BrushArrow);
	pDC->SelectObject(&PenArrow);

// 2013.01.17 bagus rotate xy view -->
	DirX = RotateXyAxisDir(DirX);
	DirY = RotateXyAxisDir(DirY);
// 2013.01.17 bagus rotate xy view <--

	///// Draw Arrows /////
	if(iOrgPos == 1){ //�\����`��
		pDC->MoveTo(CenterPosX - ARROW_LENGTH, CenterPosY);
		pDC->LineTo(CenterPosX + ARROW_LENGTH, CenterPosY);
		pDC->MoveTo(CenterPosX, CenterPosY + ARROW_LENGTH);
		pDC->LineTo(CenterPosX, CenterPosY - ARROW_LENGTH);
	}

	if(DirX == 1 || DirY == 1){ //�E����
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX + ARROW_LENGTH, CenterPosY);
		PT[0].x = CenterPosX + ARROW_LENGTH;
		PT[0].y = CenterPosY;
		PT[1].x = CenterPosX + ARROW_LENGTH - ARROW_SIDE;
		PT[1].y = CenterPosY + ARROW_SIDE/2;
		PT[2].x = CenterPosX + ARROW_LENGTH - ARROW_SIDE;
		PT[2].y = CenterPosY - ARROW_SIDE/2;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 1)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x + 5, PT[0].y - ARROW_SIDE, csBuff);
	}

	if(DirX == 2 || DirY == 2){ //�����
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX, CenterPosY - ARROW_LENGTH);
		PT[0].x = CenterPosX;
		PT[0].y = CenterPosY - ARROW_LENGTH;
		PT[1].x = CenterPosX + ARROW_SIDE/2;
		PT[1].y = CenterPosY - ARROW_LENGTH + ARROW_SIDE;
		PT[2].x = CenterPosX - ARROW_SIDE/2;
		PT[2].y = CenterPosY - ARROW_LENGTH + ARROW_SIDE;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 2)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x - ARROW_SIDE, PT[0].y - 15, csBuff);
	}

	if(DirX == 3 || DirY == 3){ //������
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX - ARROW_LENGTH, CenterPosY);
		PT[0].x = CenterPosX - ARROW_LENGTH;
		PT[0].y = CenterPosY;
		PT[1].x = CenterPosX - ARROW_LENGTH + ARROW_SIDE;
		PT[1].y = CenterPosY - ARROW_SIDE/2;
		PT[2].x = CenterPosX - ARROW_LENGTH + ARROW_SIDE;
		PT[2].y = CenterPosY + ARROW_SIDE/2;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 3)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x - 20, PT[0].y - ARROW_SIDE, csBuff);
	}
	if(DirX == 4 || DirY == 4){ //������
		pDC->MoveTo(CenterPosX, CenterPosY);
		pDC->LineTo(CenterPosX, CenterPosY + ARROW_LENGTH);
		PT[0].x = CenterPosX;
		PT[0].y = CenterPosY + ARROW_LENGTH;
		PT[1].x = CenterPosX - ARROW_SIDE/2;
		PT[1].y = CenterPosY + ARROW_LENGTH - ARROW_SIDE;
		PT[2].x = CenterPosX + ARROW_SIDE/2;
		PT[2].y = CenterPosY + ARROW_LENGTH - ARROW_SIDE;
		pDC->SetPolyFillMode(WINDING );
		pDC->Polygon( PT, 3 );
		if(DirX == 4)
			csBuff.Format("%s", "+X");
		else
			csBuff.Format("%s", "+Y");
		pDC->TextOut(PT[0].x - ARROW_SIDE, PT[0].y + 5, csBuff);
	}

	pDC->SelectObject(&BrushOrg);
	pDC->SelectObject(&PenOrg);
	pDC->Ellipse((CenterPosX - ELLIPSE_RADIUS), (CenterPosY - ELLIPSE_RADIUS), (CenterPosX + ELLIPSE_RADIUS), (CenterPosY + ELLIPSE_RADIUS));

	///// Put back color /////
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);

	ReleaseDC(pDC);
}

// 2013.01.09 bagus -->
// // ==========================================================================
// //
// void CRecipeStageProgramView::DrawOriginMaker(int Origin)
// {
// 	long OriginX, OriginY;
//
// 	CWnd* pPictBox = GetDlgItem(IDC_MEASUREMENT_MAP);
// 	CDC* pDC = pPictBox->GetDC();
// 	CRect StageRect;
// 	pPictBox->GetClientRect(&StageRect);
//
// 	CBrush BrushRed(GREEN_COLOR);
// 	CPen PenRed(PS_SOLID, 1, GREEN_COLOR);
// 	pDC->SelectObject(&BrushRed);
// 	pDC->SelectObject(&PenRed);
//
// 	///// �X�e�[�W�L���͈� /////
// 	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
// 	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
//
// 	///// Draw point /////
// 	switch(Origin){
// 	case 1:
// 	default:
// 		///// Center Position /////
// 		OriginX = m_dDrawStartStagePosX + (m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
// 		OriginY = m_dDrawStartStagePosY + (m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 		break;
//
// 	case 2: /////LeftTop
// 	case 3: /////LeftBottom
// 	case 4: /////RightBottom
// 	case 5: /////RightTop
// 		break;
// 	}
// 	//Draw Point
// 	pDC->Ellipse((OriginX - ELLIPSE_RADIUS), (OriginY - ELLIPSE_RADIUS), (OriginX + ELLIPSE_RADIUS), (OriginY + ELLIPSE_RADIUS));
// 	pDC->SelectObject(m_oldPen);
// 	pDC->SelectObject(m_oldBrush);
//
// 	ReleaseDC(pDC);
// }
// 2013.01.09 bagus <--

// ==========================================================================
//
BOOL CRecipeStageProgramView::PreTranslateMessage(MSG* pMsg)
{
/* modified 2009.06.16 hmenjo Stage PGM Edit Delete �L�[�s��΍� ---------- { ---------- */
//	CCellID CellID;
//	int iRow;
//
//	CellID = m_MeasurementPointGrid.GetFocusCell();
//	iRow = CellID.row;
//	if ( iRow == -1 ) {
//		if ( pMsg->message == WM_KEYDOWN )
//		{
//			switch(pMsg->wParam) {
//			case VK_DELETE:
//				OnDeleteButton();
//				return TRUE;
//			}
//		}
//	}
/* modified 2009.06.16 hmenjo Stage PGM Edit Delete �L�[�s��΍� ----------			   */
	/* ���b�Z�[�W�Ώۂ̃E�B���h�E�n���h������R���g���[�� ID ���擾���܂��D	*/
	int l_iCtrlID = ::GetDlgCtrlID(pMsg->hwnd);
	/* �O���b�h�R���g���[���̂݁uDelete�v�L�[���������܂��D	*/
	switch (l_iCtrlID) {
	case IDC_MEASUREMENT_POINT_GRID:
		if ((WM_KEYDOWN == pMsg->message) && (VK_DELETE == pMsg->wParam)) {
			if (m_MeasurementPointGrid.GetFocusCell().row == -1) {
				OnDeleteButton();
				return TRUE;
			}
		}
		break;
	}
/* modified 2009.06.16 hmenjo Stage PGM Edit Delete �L�[�s��΍� ---------- } ---------- */
	return CNanoRecipeUI::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
void CRecipeStageProgramView::OnEndLabelEdit(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	CCellID CellID;
	int iRow;
	CString strGridData1;
	CString strGridData2;

	CellID = m_MeasurementPointGrid.GetFocusCell();
	iRow = CellID.row;
	if ( iRow != -1 ) {
		strGridData1 = m_MeasurementPointGrid.GetItemText(iRow, 1);
		strGridData2 = m_MeasurementPointGrid.GetItemText(iRow, 2);
		if ( !strGridData1.IsEmpty() && !strGridData2.IsEmpty() ) {
			m_dPointBuff[0][iRow - 1] = atof(strGridData1);
			m_dPointBuff[1][iRow - 1] = atof(strGridData2);
			m_dPointBuff[2][iRow - 1] = TRUE;
			m_iNumScans = 0;
			for ( int i = 0; i < SCAN_POINT_MAX ; i++) {
				if ( m_dPointBuff[2][i] == TRUE ) {
					m_iNumScans++;
				}
			}
			Invalidate(TRUE);
		}
	}
}

// =========================================================================
// OnSelChangeing IDC_MEASUREMENT_POINT_GRID�I���s�̃X�L�����|�C���g�ɂ��āA�}�b�v�̃X�L�����|�C���g��I��F�ɐݒ肷��
void CRecipeStageProgramView::OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	///// Draw SelectPoint /////
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;

	if ( !m_MeasurementPointGrid.IsValid(CellID) )
		return;

	//PointGraph�I���|�C���g�F�ύX
	m_SamplePointGraph.Select(CellID.row-1);

	//���݂̑I�𒆃Z��
	m_CurrentCellID = CellID;
}

// ==========================================================================
// OnSelChanged IDC_MEASUREMENT_POINT_GRID�I���s�̃X�L�����ҏW������
void CRecipeStageProgramView::OnSelChanged(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	/*
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID CellID;
	CellID.row=pItem->iRow;
	CellID.col=pItem->iColumn;

	if ( !m_MeasurementPointGrid.IsValid(CellID) )
		return;

	//���݂̑I�𒆃Z��
	m_CurrentCellID = CellID;
	*/
}

// =========================================================================
//
BOOL CRecipeStageProgramView::IsAccessPrivilege()
{
	///// User Access Privilege /////
	return m_pDoc->User_Access(ACCESS_PROGRAM);
}

// =========================================================================
// �f�[�^�j������CleanUp����
void CRecipeStageProgramView::Discard()
{
	char szFilePath[MAX_PATH + 1];
	char szFilePathBackUp[MAX_PATH + 1];

	// �p�^�[���F���摜�i�ޔ��j�̖߂�
	_stprintf(szFilePathBackUp, "%s_BackUp.D1", g_szDb_Deskew_Img_Dir);
	_stprintf(szFilePath, "%s%s.D1", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
	::CopyFile(szFilePathBackUp, szFilePath, FALSE);
	_stprintf(szFilePathBackUp, "%s_BackUp.D2", g_szDb_Deskew_Img_Dir);
	_stprintf(szFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
	::CopyFile(szFilePathBackUp, szFilePath, FALSE);
}

// =========================================================================
//
int CRecipeStageProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_STAGE_PROGRAM;
}

// =========================================================================
//
void CRecipeStageProgramView::SamplePointGraph_Init()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	m_SamplePointGraph.ShowWindow(SW_HIDE);

	////Point Graph Data Reset
	m_SamplePointGraph.Reset();

	////Back Color
	tConfig.BackColor = WAFER_COLOR;//WATER_COLOR;//WHITE_COLOR;
	////Edge Color
	tConfig.EdgeColor[CNanoPointGraph::EDGE_LOWER] = WHITE_COLOR;
	tConfig.EdgeColor[CNanoPointGraph::EDGE_UPPER] = RGB(0x00,0x00,0x00);
	////Status Color
	tConfig.StatusColor[CNanoPointGraph::NOT_MEASURED] = RGB(0x00,0x00,0xFF);
	tConfig.StatusColor[CNanoPointGraph::MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.StatusColor[CNanoPointGraph::SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.StatusColor[CNanoPointGraph::MEASURED] = RGB(0x77,0x77,0x77);

	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////CNanoPointGraph�N���X���T�C�Y(Sample Size)
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//	tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
//	tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
// 		// X<->Y����ւ�
// 		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
// 		// X-Y���̂܂�
// 		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 	} else {
// 		// ���ݒ肪��������
// 	}
// 2013.01.17 bagus rotate xy view -->
	double dSampleSizeX;
	double dSampleSizeY;

	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->Y����ւ�
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-Y���̂܂�
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else {
		// ���ݒ肪��������
	}

	double dTemp;

	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	case 180:
	default:
		break;
	case 90:
	case 270:
		dTemp = dSampleSizeX;
		dSampleSizeX = dSampleSizeY;
		dSampleSizeY = dTemp;
		break;
	}

	tConfig.SampleSizeX = dSampleSizeX;
	tConfig.SampleSizeY = dSampleSizeY;
// 2013.01.17 bagus rotate xy view <--

// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
	m_SamplePointGraph.SetConfig(&tConfig);

	SamplePointGraph_DataSet();
}

// =========================================================================
//
void CRecipeStageProgramView::SamplePointGraph_DataSet()
{
	////Point Graph Data Reset
	m_SamplePointGraph.Reset();

	////Point Data
	//�Z���^�[���_�̏ꍇ
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//	double dOffSetValueX = (m_StageProgInfoHdr.SampleInfo.Size.dx/2);
//	double dOffSetValueY = (m_StageProgInfoHdr.SampleInfo.Size.dy/2);
	double dOffSetValueX;
	double dOffSetValueY;
	switch (m_StageConfig.OrgPos) {
	case 1:
	default:
		dOffSetValueX = (m_StageProgInfoHdr.SampleInfo.Size.dx/2);
		dOffSetValueY = (m_StageProgInfoHdr.SampleInfo.Size.dy/2);
// 2013.01.17 bagus rotate xy view -->
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			break;
		case 180:
			break;
		case 270:
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 2:		// Left & Top
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 		dOffSetValueY = 0;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			break;
		case 90:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 180:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 270:
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 3:		// Left & Bottom
// 2013.01.17 bagus rotate xy view -->
//		dOffSetValueX = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = 0;
// 		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 90:
			break;
		case 180:
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 270:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 4:		// Right & Bottom
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = 0;
// 		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 90:
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 180:
			break;
		case 270:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	case 5:		// Right & Top
// 2013.01.17 bagus rotate xy view -->
// 		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
// //		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 		dOffSetValueY = 0;
// // 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		dOffSetValueX = 0;
		dOffSetValueY = 0;
		switch ( m_StageConfig.RotateXyView ) {
		case 0:
		default:
 			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 90:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			dOffSetValueY += m_StageProgInfoHdr.SampleInfo.Size.dy;
			break;
		case 180:
			dOffSetValueX += m_StageProgInfoHdr.SampleInfo.Size.dx;
			break;
		case 270:
			break;
		}
// 2013.01.17 bagus rotate xy view <--
		break;
	}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

// 2013.01.09 bagus -->
	double dOriginPointDataX;
	double dOriginPointDataY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dOriginPointDataX = dOffSetValueX;
		dOriginPointDataY = dOffSetValueY;
	}
	else {
		dOriginPointDataX = dOffSetValueY;
		dOriginPointDataY = dOffSetValueX;
	}

// 2013.01.17 bagus rotate xy view <--
//	m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOffSetValueX), static_cast<long>(dOffSetValueY));
	switch ( m_StageConfig.RotateXyView ) {
	case 0:
	default:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataX), static_cast<long>(dOriginPointDataY));
		break;
	case 90:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataY), static_cast<long>(dOriginPointDataX));
		dOffSetValueX = dOriginPointDataY;
		dOffSetValueY = dOriginPointDataX;
		break;
	case 180:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataX), static_cast<long>(dOriginPointDataY));
		break;
	case 270:
		m_SamplePointGraph.SetOriginPointData(static_cast<long>(dOriginPointDataY), static_cast<long>(dOriginPointDataX));
		dOffSetValueX = dOriginPointDataY;
		dOffSetValueY = dOriginPointDataX;
		break;
	}
// 2013.01.17 bagus rotate xy view <--
// 2013.01.09 bagus <--

	double Scan_PointIncOffsetX;
	double Scan_PointIncOffsetY;
// 2013.01.17 bagus rotate xy view -->
	double dScanPointX;
	double dScanPointY;

	long lDirX = RotateXyAxisDir(m_StageConfig.Dir.X);
	long lDirY = RotateXyAxisDir(m_StageConfig.Dir.Y);
// 2013.01.17 bagus rotate xy view <--

	for(int iPoint= 0; iPoint<m_StageProgInfoHdr.wNumScans; iPoint++){
// 2013.01.17 bagus rotate xy view -->
// // 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
// //		Scan_PointIncOffsetX = m_ScanPoint[iPoint].lX + dOffSetValueX;
// //		Scan_PointIncOffsetY = -(m_ScanPoint[iPoint].lY - dOffSetValueY);
// 		switch (m_StageConfig.Dir.X) {
// 		case 1:		// X:��
// 		default:
// 			Scan_PointIncOffsetX = dOffSetValueX + m_ScanPoint[iPoint].lX;
// 			break;
// 		case 2:		// X:��
// 			Scan_PointIncOffsetY = dOffSetValueX - m_ScanPoint[iPoint].lX;
// 			break;
// 		case 3:		// X:��
// 			Scan_PointIncOffsetX = dOffSetValueX - m_ScanPoint[iPoint].lX;
// 			break;
// 		case 4:		// X:��
// 			Scan_PointIncOffsetY = dOffSetValueX + m_ScanPoint[iPoint].lX;
// 			break;
// 		}
//
// 		switch (m_StageConfig.Dir.Y) {
// 		case 1:		// Y:��
// 			Scan_PointIncOffsetX = dOffSetValueY + m_ScanPoint[iPoint].lY;
// 			break;
// 		case 2:		// Y:��
// 		default:
// 			Scan_PointIncOffsetY = dOffSetValueY - m_ScanPoint[iPoint].lY;
// 			break;
// 		case 3:		// Y:��
// 			Scan_PointIncOffsetX = dOffSetValueY - m_ScanPoint[iPoint].lY;
// 			break;
// 		case 4:		// Y:��
// 			Scan_PointIncOffsetY = dOffSetValueY + m_ScanPoint[iPoint].lY;
// 			break;
// 		}
// // 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

		dScanPointX = m_ScanPoint[iPoint].lX;
		dScanPointY = m_ScanPoint[iPoint].lY;

		switch (lDirX) {
		case 1:		// X:��
		default:
			Scan_PointIncOffsetX = dOffSetValueX + dScanPointX;
			break;
		case 2:		// X:��
			Scan_PointIncOffsetY = dOffSetValueX - dScanPointX;
			break;
		case 3:		// X:��
			Scan_PointIncOffsetX = dOffSetValueX - dScanPointX;
			break;
		case 4:		// X:��
			Scan_PointIncOffsetY = dOffSetValueX + dScanPointX;
			break;
		}

		switch (lDirY) {
		case 1:		// Y:��
			Scan_PointIncOffsetX = dOffSetValueY + dScanPointY;
			break;
		case 2:		// Y:��
		default:
			Scan_PointIncOffsetY = dOffSetValueY - dScanPointY;
			break;
		case 3:		// Y:��
			Scan_PointIncOffsetX = dOffSetValueY - dScanPointY;
			break;
		case 4:		// Y:��
			Scan_PointIncOffsetY = dOffSetValueY + dScanPointY;
			break;
		}
// 2013.01.17 bagus rotate xy view <--

		m_SamplePointGraph.AddPoint(static_cast<long>(Scan_PointIncOffsetX), static_cast<long>(Scan_PointIncOffsetY));
	}

	///// Point Number /////
	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);
}

void CRecipeStageProgramView::OnRedrawPointMap()
{
	UpDate();
/*
	///// Data Update /////
	for(int i = 0 ; i < m_iNumScans + 1; i++){
		if ( m_dPointBuff[2][i] != FALSE ) {
			m_ScanPoint[i].lX = static_cast<long>((m_dPointBuff[0][i]) * MICROMETRE);
			m_ScanPoint[i].lY = static_cast<long>((m_dPointBuff[1][i]) * MICROMETRE);
		}
	}
*/
	SamplePointGraph_DataSet();

}

void CRecipeStageProgramView::OnSitePrSetButton()
{
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
	CString strMsg;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;} // �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return;} // MOVO�A���[�� 				���k
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}	// �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}

	// ���C���^�[���b�N������
// 2010.02.02 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	if ( !CheckData() )
		return;

	CSitePatternSetDlg dlg(&m_StageProgInfoHdr);

	::CopyMemory(dlg.m_ScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
	::CopyMemory(dlg.m_SitePoint, m_SitePoint, sizeof(m_SitePoint));
	dlg.m_iNumScans = m_iNumScans;
	if( dlg.DoModal() == IDOK ){
		::CopyMemory(m_ScanPoint, dlg.m_ScanPoint, sizeof(m_ScanPoint));
		::CopyMemory(m_SitePoint, dlg.m_SitePoint, sizeof(m_SitePoint));
		SetData();
	}
}

void CRecipeStageProgramView::OnSitePrDelButton()
{
}

// 2013.01.09 bagus -->
// 2009.10.24 bagus 2�_�� �C�� --{--
BOOL CRecipeStageProgramView::CheckValidPointX(long lPointX)
{
// 	long lMax, lMin;
// 	BOOL bRet = FALSE;
//
// 	switch (m_StageConfig.OrgPos) {
// 	case 1:		// Center
// 	default:
// 		lMax = m_StageProgInfoHdr.SampleInfo.Size.dx / 2;
// 		lMin = -lMax;
// 		break;
// 	case 2:		// Left & Top
// 	case 3:		// Left & Bottom
// 		lMax = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 		lMin = 0;
// 		break;
// 	case 4:		// Right & Bottom
// 	case 5:		// Right & Top
// 		lMax = 0;
// 		lMin = -m_StageProgInfoHdr.SampleInfo.Size.dx;
// 		break;
// 	}
//
// 	if ((lMin <= lPointX) && (lPointX <= lMax))
// 		bRet = TRUE;
//
// 	return ( bRet );

	CString csBuff;
	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;

	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
	}

	switch ( m_StageConfig.OrgPos ) {
	case 1: // ���S
		coordLeftTop.lX = static_cast<long>(dSampleSizeX / 2 * -1);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY / 2);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX / 2);
		coordRightBottom.lY = static_cast<long>(dSampleSizeY / 2 * -1);
		break;
	case 2: // ����
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX);
		coordRightBottom.lY = static_cast<long>(dSampleSizeY * -1);
		break;
	case 3: // ����
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX);
		coordRightBottom.lY = 0;
		break;
	case 4: // �E��
		coordLeftTop.lX = static_cast<long>(dSampleSizeX * -1);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = 0;
		break;
	case 5: // �E��
		coordLeftTop.lX = static_cast<long>(dSampleSizeX * -1);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = static_cast<long>(dSampleSizeY * -1);
		break;
	}

	// �X�e�[�W�R���t�B�O�̎��ݒ�ƍ��킹��
	NS_ConvertToStageConfigXyAxis(&coordLeftTop);
	NS_ConvertToStageConfigXyAxis(&coordRightBottom);

	///// X /////
	if(lPointX < min(coordLeftTop.lX, coordRightBottom.lX)){
		return FALSE;
	}
	if(max(coordLeftTop.lX, coordRightBottom.lX) < lPointX){
		return FALSE;
	}

	return TRUE;
}

BOOL CRecipeStageProgramView::CheckValidPointY(long lPointY)
{
// 	long lMax, lMin;
// 	BOOL bRet = FALSE;
//
// 	switch (m_StageConfig.OrgPos) {
// 	case 1:		// Center
// 	default:
// 		lMax = m_StageProgInfoHdr.SampleInfo.Size.dy / 2;
// 		lMin = -lMax;
// 		break;
// 	case 2:		// Left & Top
// 	case 5:		// Right & Top
// 		lMax = 0;
// 		lMin = -m_StageProgInfoHdr.SampleInfo.Size.dy;
// 		break;
// 	case 3:		// Left & Bottom
// 	case 4:		// Right & Bottom
// 		lMax = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 		lMin = 0;
// 		break;
// 	}
//
// 	if ((lMin <= lPointY) && (lPointY <= lMax))
// 		bRet = TRUE;
//
// 	return ( bRet );

	CString csBuff;
	STAGE_COORD coordLeftTop;
	STAGE_COORD coordRightBottom;

	double dSampleSizeX;
	double dSampleSizeY;

	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	}
	else {
		dSampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dSampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
	}

	switch ( m_StageConfig.OrgPos ) {
	case 1: // ���S
		coordLeftTop.lX = static_cast<long>(dSampleSizeX / 2 * -1);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY / 2);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX / 2);
		coordRightBottom.lY = static_cast<long>(dSampleSizeY / 2 * -1);
		break;
	case 2: // ����
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX);
		coordRightBottom.lY = static_cast<long>(dSampleSizeY * -1);
		break;
	case 3: // ����
		coordLeftTop.lX = static_cast<long>(0);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY);
		coordRightBottom.lX = static_cast<long>(dSampleSizeX);
		coordRightBottom.lY = 0;
		break;
	case 4: // �E��
		coordLeftTop.lX = static_cast<long>(dSampleSizeX * -1);
		coordLeftTop.lY = static_cast<long>(dSampleSizeY);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = 0;
		break;
	case 5: // �E��
		coordLeftTop.lX = static_cast<long>(dSampleSizeX * -1);
		coordLeftTop.lY = static_cast<long>(0);
		coordRightBottom.lX = 0;
		coordRightBottom.lY = static_cast<long>(dSampleSizeY * -1);
		break;
	}

	// �X�e�[�W�R���t�B�O�̎��ݒ�ƍ��킹��
	NS_ConvertToStageConfigXyAxis(&coordLeftTop);
	NS_ConvertToStageConfigXyAxis(&coordRightBottom);

	///// Y /////
	if(lPointY < min(coordLeftTop.lY, coordRightBottom.lY)){
		return FALSE;
	}
	if(max(coordLeftTop.lY, coordRightBottom.lY) < lPointY){
		return FALSE;
	}

	return TRUE;
}
// 2009.10.24 bagus 2�_�� �C�� --}--
// 2013.01.09 bagus <--
