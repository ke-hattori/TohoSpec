// RecipeStressStageProgramView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
//#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "RecipeStressStageProgramView.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingDlg.h"
#include "BitmapStatic.h"
#include "DeskewDlg.h"
#include "BtnSt.h"
#include "GridCellCheck.h"
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
#define ELLIPSE_RADIUS		2	//4

//Saiki 20110207 Delete ----->
//#define	PIN_INTERVAL		7
//Saiki 20110207 Delete <-----

// 2009.09.17 bagus stress Grid�C�� --{--
#define	DEFAULT_GRID_ROWS			30
// 2009.09.17 bagus stress Grid�C�� --}--

// 2009.10.09 bagus StagePGM ���ʉ� --{--
#define	STAGE_PROGRAM_HEAD_TYPE		(5)
// 2009.10.09 bagus StagePGM ���ʉ� --}--

// 2013.01.09 bagus -->
#define DIR_UNKNOWN    (0)
#define DIR_RIGHT      (1)
#define DIR_TOP        (2)
#define DIR_LEFT       (3)
#define DIR_BOTTOM     (4)
#define DIR_MAX        (5)
// 2013.01.09 bagus <--

/////////////////////////////////////////////////////////////////////////////
// CRecipeStressStageProgramView

IMPLEMENT_DYNCREATE(CRecipeStressStageProgramView, CNanoRecipeUI)

CRecipeStressStageProgramView::CRecipeStressStageProgramView()
	: CNanoRecipeUI(CRecipeStressStageProgramView::IDD)
{
	//{{AFX_DATA_INIT(CRecipeStressStageProgramView)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�̏�����������ǉ����܂�
	//}}AFX_DATA_INIT
	CString l_strBuffer;
	LoadStringML(IDS_CAPTION_STAGE_PROGRAM, l_strBuffer, "STAGE PROGRAM");
	m_strCaption = l_strBuffer;

	::ZeroMemory(m_dPointBuff, sizeof(m_dPointBuff));
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	::ZeroMemory(m_OldScanPoint, sizeof(m_OldScanPoint));
	::ZeroMemory(m_SitePoint, sizeof(m_SitePoint));
	::ZeroMemory(m_OldSitePoint, sizeof(m_OldSitePoint));

// 2009.09.10 bagus stress �C�� --{--
	m_bFirst = TRUE;
	memset(m_bUseLine, FALSE, sizeof(m_bUseLine));
// 2009.09.10 bagus stress �C�� --}--

// 2009.09.17 bagus stress Grid�C�� --{--
	m_nSysVScrollWidth = GetSystemMetrics(SM_CXVSCROLL);
// 2009.09.17 bagus stress Grid�C�� --}--
}

CRecipeStressStageProgramView::~CRecipeStressStageProgramView()
{
}

void CRecipeStressStageProgramView::DoDataExchange(CDataExchange* pDX)
{
	CNanoRecipeUI::DoDataExchange(pDX);
	//CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeStressStageProgramView)
	DDX_Control(pDX, IDC_SAMPLE_POINT_MAP, m_SamplePointGraph);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MEASUREMENT_LINE_SECTION_GRID, m_MeasurementPointGrid);
	DDX_Control(pDX, IDC_MEASUREMENT_POINT_GRID, m_MeasurementPointGrid2);
	DDX_Text(pDX, IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName, RECIPE_NAME_LEN + 1);
	DDX_Control(pDX, IDC_STAGE_PROGRAM_POINT_NUMBER, m_stcStageProgramNumScan);
	DDX_Control(pDX, IDC_STAGE_PROGRAM_LINE_SECTION, m_stcStageProgramLineSection);
	DDX_Control(pDX, IDC_SAMPLE_NAME, m_stcSampleName);
	DDX_Control(pDX, IDC_SAMPLE_SHAPE, m_stcSampleShape);
	DDX_Control(pDX, IDC_SAMPLE_THICKNESS, m_stcSampleThickness);
	DDX_Control(pDX, IDC_SAMPLE_SIZE_X, m_stcSampleSizeX);
	DDX_Control(pDX, IDC_SAMPLE_SIZE_Y, m_stcSampleSizeY);
	DDX_Control(pDX, IDC_SAMPLE_ORIGIN_X, m_stcSampleOriginX);
	DDX_Control(pDX, IDC_SAMPLE_ORIGIN_Y, m_stcSampleOriginY);
// 2009.09.04 bagus stress �f�X�L���[�폜 --{--
//	DDX_Control(pDX, IDC_STAGE_DESKEW_MODE, m_stcDeskewMode);
// 2009.09.04 bagus stress �f�X�L���[�폜 --}--
	DDX_Control(pDX, IDC_ALLCHECK_BUTTON, m_AllCheckButton);
	DDX_Control(pDX, IDC_ALL_CLEAR_BUTTON, m_AllClearButton);
	DDX_Control(pDX, IDC_TEACHING_BUTTON, m_TeachingButton);
	DDX_Control(pDX, IDC_REDRAW_POINT_MAP, m_RedrawPointButton);
// 2009.09.04 bagus stress �f�X�L���[�폜 --{--
//	DDX_Control(pDX, IDC_DESKEW_BUTTON, m_DeskewButton);
// 2009.09.04 bagus stress �f�X�L���[�폜 --}--
}


BEGIN_MESSAGE_MAP(CRecipeStressStageProgramView, CNanoRecipeUI)
	//{{AFX_MSG_MAP(CRecipeStressStageProgramView)
	ON_BN_CLICKED(IDC_ALLCHECK_BUTTON, OnAllcheckButton)
	ON_BN_CLICKED(IDC_ALL_CLEAR_BUTTON, OnAllClearButton)
	ON_BN_CLICKED(IDC_TEACHING_BUTTON, OnTeachingButton)
	ON_BN_CLICKED(IDC_REDRAW_POINT_MAP, OnRedrawPointMap)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_MATERIAL_ELASTIC_MODULUS, OnSelchangeMaterialElasticModulus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeStressStageProgramView �f�f

#ifdef _DEBUG
void CRecipeStressStageProgramView::AssertValid() const
{
	CNanoRecipeUI::AssertValid();
}

void CRecipeStressStageProgramView::Dump(CDumpContext& dc) const
{
	CNanoRecipeUI::Dump(dc);
}
#endif //_DEBUG

BOOL CRecipeStressStageProgramView::IsDataChanged()
{
	UpDate();

	//����|�C���g���܂łŕω����������ꍇ�͍X�V���ꂽ�Ƃ݂Ȃ�
	return ( memcmp(&m_StageProgInfoHdr, &m_OldStageProgInfoHdr, sizeof(m_StageProgInfoHdr)) != 0 ||
				memcmp(&m_StageStressProg, &m_OldStageStressProg, sizeof(m_StageStressProg)) != 0 ||
				memcmp(m_ScanPoint, m_OldScanPoint, (sizeof(m_ScanPoint) / SCAN_POINT_MAX * m_StageProgInfoHdr.wNumScans)) != 0 ||
				memcmp(m_SitePoint, m_OldSitePoint, (sizeof(m_SitePoint) / SCAN_POINT_MAX * m_StageProgInfoHdr.wNumScans)) != 0 );
}
BOOL CRecipeStressStageProgramView::SaveRecipeData()
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
		::DeleteFile(szNewFilePath);
		::CopyFile(szOriginalFilePath, szNewFilePath, FALSE);
		_stprintf(szOriginalFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_OldStageProgInfoHdr.hdr.szName);
		_stprintf(szNewFilePath, "%s%s.D2", g_szDb_Deskew_Img_Dir, m_szRecipeName);
		::DeleteFile(szNewFilePath);
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

	if ( !RecipeFile_SaveRecipe(&m_StageStressProg, m_StageProgInfoHdr.hdr.szName, RECIPE_FILE_STAGE_PROGRAM_STRESS) )
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
	::CopyMemory(&m_OldStageStressProg, &m_StageStressProg, sizeof(STAGE_PROG_STRESS));
	::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
	::CopyMemory(m_OldSitePoint, m_SitePoint, sizeof(m_SitePoint));

	CRect rect;
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetClientRect(rect);
	InvalidateRect(rect);

	return TRUE;
}

BOOL CRecipeStressStageProgramView::IsAccessPrivilege()
{
	///// User Access Privilege /////
	return m_pDoc->User_Access(ACCESS_PROGRAM);
}

BOOL CRecipeStressStageProgramView::CheckData()
{
// 2009.09.16 bagus stress �L�����C���� > 0 --{--
	if (m_StageStressProg.dwNumLsScans < 1) {
		CString	strBuffer;
		LoadStringML(IDS_INPUT_MEAS_LINE, strBuffer, "Measurement line is necessary by one.");
		AfxMessageBox(strBuffer);
		return ( FALSE );
	}
// 2009.09.16 bagus stress �L�����C���� > 0 --}--

// 2009.11.09 bagus Stress �ǉ��ύX --{--
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS);

	int nIndex = pCombo->GetCurSel();

	if (nIndex == CB_ERR) {
		CString	strBuffer;
		LoadStringML(IDS_STRESS_SELECT_MATERIAL_ELASTIC_MODULUS, strBuffer, "Please select material elastic modulus.");
		AfxMessageBox(strBuffer);
		return ( FALSE );
	}
// 2009.11.09 bagus Stress �ǉ��ύX --}--

	return TRUE;
}

int  CRecipeStressStageProgramView::SaveAsDlgInfo()
{
	return SAVE_AS_STAGE_PROGRAM;
}

void CRecipeStressStageProgramView::Discard()
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


/////////////////////////////////////////////////////////////////////////////
// CRecipeStressStageProgramView ���b�Z�[�W �n���h��
void CRecipeStressStageProgramView::OnInitialUpdate()
{
	m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.09.09 bagus stress �C�� --{--
	memset(&m_StageConfig,NULL,sizeof(m_StageConfig));
// 2009.09.09 bagus stress �C�� --}--
	ConfigFile_GetNanoSpecIni(&m_StageConfig, CONFIG_FILE_STAGE_CONFIG);
	memset(&m_StressConfig, 0, sizeof(m_StressConfig));
	ConfigFile_GetNanoSpecIni(&m_StressConfig, CONFIG_FILE_STRESS_CONFIG);

	if ( !LoadRecipeData() ) {
		UpDate();
		::CopyMemory(&m_OldStageProgInfoHdr, &m_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
		::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
		::CopyMemory(&m_OldStageStressProg, &m_StageStressProg, sizeof(STAGE_PROG_STRESS));
//		LoadStringML(IDS_NOT_OPEN_SAMPLE_TYPE, l_strBuffer, "Can not open the sample type file.\nReturn to Stage Program List.");
//		MessageBox(l_strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
//		pMainFrame->PostMessage(WM_EXE_WND_EXIT, 0, 0);
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

	///// Static Control /////
	m_stcStageProgramName.SubclassWindow(GetDlgItem(IDC_STAGE_PROGRAM_NAME)->GetSafeHwnd());
	m_stcStageProgramName.SetBkColor(WATER_COLOR);
	m_stcStageProgramNumScan.SetBkColor(WATER_COLOR);
	m_stcStageProgramLineSection.SetBkColor(WATER_COLOR);
	m_stcSampleName.SetBkColor(WATER_COLOR);
	m_stcSampleShape.SetBkColor(WATER_COLOR);
	m_stcSampleThickness.SetBkColor(WATER_COLOR);
	m_stcSampleSizeX.SetBkColor(WATER_COLOR);
	m_stcSampleSizeY.SetBkColor(WATER_COLOR);
	m_stcSampleOriginX.SetBkColor(WATER_COLOR);
	m_stcSampleOriginY.SetBkColor(WATER_COLOR);
// 2009.09.04 bagus stress �f�X�L���[�폜 --{--
//	m_stcDeskewMode.SetBkColor(WATER_COLOR);
// 2009.09.04 bagus stress �f�X�L���[�폜 --}--

	m_AllCheckButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllCheckButton.DrawFlatFocus(TRUE);
	m_AllClearButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_AllClearButton.DrawFlatFocus(TRUE);
	m_TeachingButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_TeachingButton.DrawFlatFocus(TRUE);
	m_RedrawPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_RedrawPointButton.DrawFlatFocus(TRUE);
// 2009.09.04 bagus stress �f�X�L���[�폜 --{--
//	m_DeskewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
//	m_DeskewButton.DrawFlatFocus(TRUE);
// 2009.09.04 bagus stress �f�X�L���[�폜 --}--

// 2009.11.09 bagus Stress �ǉ��ύX --{--
	memset(&m_ElasticTable, 0, sizeof(m_ElasticTable));
	ConfigFile_GetNanoSpecIni(&m_ElasticTable, CONFIG_FILE_STRESS_MODULUS_CONFIG);

	InitCombo_MaterialElasticModulus();
// 2009.11.09 bagus Stress �ǉ��ύX --}--

	///// Measurement Point Grid /////
	m_SamplePointGraph.SetLineStatusMode(TRUE);
	SamplePointGraph_Init();

	///// Set Data /////
	SetData();

	SetMeasurementLineSectionData();
	SetMeasurementPointData();

// 2009.09.09 bagus stress �C�� --{--
//	UpDate();
	SamplePointGraph_DataSet();
// 2009.09.09 bagus stress �C�� --}--

	SetTimer(1,100,NULL);
}


void CRecipeStressStageProgramView::OnAllcheckButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	int	i, j;

	for (i=0; i<STRESS_LINES_MAX; i++) {
		for (j=0; j<STRESS_SECTIONS_MAX; j++) {
			m_StageStressProg.Line[i].bScanValid[j] = TRUE;
		}
	}

//	UpDate();
	SetMeasurementLineSectionData();

// 2009.09.07 bagus stress Line/Section���C�� --{--
#if 0
	m_StageStressProg.dwNumLsScans = CheckLineSectionNum();

	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageStressProg.dwNumLsScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_LINE_SECTION, szBuff);
#else
	CheckMeasurementLineSectionData();
#endif
// 2009.09.07 bagus stress Line/Section���C�� --}--
//2009.09.15 bagus stress --{--
	m_SamplePointGraph.Invalidate();
	Invalidate();
//2009.09.15 bagus stress --}--
}

void CRecipeStressStageProgramView::OnAllClearButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	int	i, j;

	for (i=0; i<STRESS_LINES_MAX; i++) {
		for (j=0; j<STRESS_SECTIONS_MAX; j++) {
			m_StageStressProg.Line[i].bScanValid[j] = FALSE;
		}
	}

//	UpDate();
	SetMeasurementLineSectionData();

// 2009.09.07 bagus stress Line/Section���C�� --{--
#if 0
	m_StageStressProg.dwNumLsScans = CheckLineSectionNum();

	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageStressProg.dwNumLsScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_LINE_SECTION, szBuff);
#else
	CheckMeasurementLineSectionData();
#endif
// 2009.09.07 bagus stress Line/Section���C�� --}--
//2009.09.15 bagus stress --{--
	m_SamplePointGraph.Invalidate();
	Invalidate();
//2009.09.15 bagus stress --}--
}

void CRecipeStressStageProgramView::OnTeachingButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString l_strBuffer;

	char szBuff[256];

	if(m_iNumScans >= SCAN_POINT_MAX){
		LoadStringML(IDS_MAX_NUM_REGISTER, l_strBuffer, "The Maximum Number which can be Registered is %d.");
		sprintf(szBuff, l_strBuffer, SCAN_POINT_MAX);
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

// 2009.09.14 bagus stress Stress�Ή� --{--
	CCellID celSel = m_MeasurementPointGrid2.GetFocusCell();
	int	FixRow = 1;

	dlg.m_wHeadType = HEAD_TYPE_STRESS;
	dlg.m_nSelectedPoint = celSel.row - FixRow + 1;
// 2009.09.14 bagus stress Stress�Ή� --}--

	if(dlg.DoModal() == IDOK){
		m_iNumScans = dlg.m_iNumScans;
		memcpy(m_ScanPoint, dlg.m_ScanPoint, sizeof(m_ScanPoint));
		SetData();

		m_iNumScans++;
	}

	UpDate();
	SamplePointGraph_DataSet();
}

void CRecipeStressStageProgramView::OnRedrawPointMap()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

// 2009.09.10 bagus stress �C�� --{--
// debug
//	UpDate();
// 2009.09.10 bagus stress �C�� --}--
	SamplePointGraph_DataSet();
}

// 2009.09.04 bagus stress �f�X�L���[�폜 --{--
/*
void CRecipeStressStageProgramView::OnDeskewButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CDeskewDlg dlg(&m_StageProgInfoHdr);
	if( dlg.DoModal() == IDOK ){
		SetData();
	}
}
*/
// 2009.09.04 bagus stress �f�X�L���[�폜 --}--

void CRecipeStressStageProgramView::MeasurementLineSectionListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 4; //�L��Col��

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
//	int Rows = 100 + FixRows;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid�C�� --{--
//	int row,col;
//	int ItemWidth = 70;
	int	col;
// 2009.09.17 bagus stress Grid�C�� --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"LS No.",
		"X Start",
		"X End",
		"Y",
		"Valid",
	};
	char* ColItem_JPN[]={
		"LS No.",
		"X Start",
		"X End",
		"Y",
		"Valid",
	};

	///// Grid Initialize /////
	m_MeasurementPointGrid.SetEditable(FALSE);
	m_MeasurementPointGrid.EnableDragAndDrop(FALSE);
	m_MeasurementPointGrid.SetTextBkColor(WHITE_COLOR);
	m_MeasurementPointGrid.SetRowResize(0);

	//2009.09.15 bagus stress --{--
	//m_MeasurementPointGrid.EnableSelection(FALSE);
	m_MeasurementPointGrid.EnableSelection(TRUE);
	//m_MeasurementPointGrid.SetListMode(TRUE);
	m_MeasurementPointGrid.SetListMode(FALSE);
	//m_MeasurementPointGrid.SetTrackFocusCell(TRUE);
	m_MeasurementPointGrid.SetTrackFocusCell(FALSE);
	//m_MeasurementPointGrid.SetFrameFocusCell(TRUE);
	m_MeasurementPointGrid.SetFrameFocusCell(FALSE);
	//m_MeasurementPointGrid.SetFixedColumnSelection(FALSE);
	m_MeasurementPointGrid.SetFixedColumnSelection(TRUE);
	//m_MeasurementPointGrid.SetFixedRowSelection(FALSE);
	m_MeasurementPointGrid.SetFixedRowSelection(TRUE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_MeasurementPointGrid.SetRowCount(Rows);
		m_MeasurementPointGrid.SetColumnCount(Cols);
		m_MeasurementPointGrid.SetFixedRowCount(FixRows);
		m_MeasurementPointGrid.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid�C�� --{--
	CWnd* GridBox = GetDlgItem(IDC_MEASUREMENT_LINE_SECTION_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid�C�� --}--

	// Row 0 Height
	m_MeasurementPointGrid.SetRowHeight(0, 18);

// 2009.09.17 bagus stress Grid�C�� --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_MeasurementPointGrid.GetRowCount() ; row++){
		m_MeasurementPointGrid.SetRowHeight(row, 18);
	}
#endif
// 2009.09.17 bagus stress Grid�C�� --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid�C�� --{--
//	m_MeasurementPointGrid.SetColumnWidth(0, 52);	// No.
	m_MeasurementPointGrid.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid�C�� --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_MeasurementPointGrid.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid�C�� --{--
//		m_MeasurementPointGrid.SetColumnWidth(col, ItemWidth);
		m_MeasurementPointGrid.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid�C�� --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_MeasurementPointGrid.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_MeasurementPointGrid.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid�C�� --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_MeasurementPointGrid.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("LxP%d", row);
		m_MeasurementPointGrid.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid�C�� --}--
}

void CRecipeStressStageProgramView::MeasurementPointListGrid_Init()
{
	GV_ITEM Item;

	int iDispCol = 2; //�L��Col��

	int FixRows = 1;
	int FixCols = 1;
	int Cols = iDispCol + FixCols;
//	int Rows = 100 + FixRows;
	int Rows = FixRows;
// 2009.09.17 bagus stress Grid�C�� --{--
//	int row,col;
//	int ItemWidth = 70;
	int	col;
// 2009.09.17 bagus stress Grid�C�� --}--
	DWORD dwTextStyle;

	char* ColItem_ENU[]={
		"No.",
		"X",
		"Y",
	};
	char* ColItem_JPN[]={
		"No.",
		"X",
		"Y",
	};

	///// Grid Initialize /////
	m_MeasurementPointGrid2.SetEditable(FALSE);
	m_MeasurementPointGrid2.EnableDragAndDrop(FALSE);
	m_MeasurementPointGrid2.SetTextBkColor(WHITE_COLOR);
	m_MeasurementPointGrid2.SetRowResize(0);

	//2009.09.15 bagus stress --{--
	//m_MeasurementPointGrid2.EnableSelection(FALSE);
	//m_MeasurementPointGrid2.SetTrackFocusCell(TRUE);
	//m_MeasurementPointGrid2.SetFrameFocusCell(TRUE);
	//m_MeasurementPointGrid2.SetFixedColumnSelection(FALSE);
	//m_MeasurementPointGrid2.SetFixedRowSelection(FALSE);

	m_MeasurementPointGrid2.EnableSelection(TRUE);
	m_MeasurementPointGrid2.SetTrackFocusCell(FALSE);
	m_MeasurementPointGrid2.SetFrameFocusCell(FALSE);
	m_MeasurementPointGrid2.SetFixedColumnSelection(TRUE);
	m_MeasurementPointGrid2.SetFixedRowSelection(TRUE);
	m_MeasurementPointGrid2.SetListMode(FALSE);
	//2009.09.15 bagus stress --}--

	TRY{
		m_MeasurementPointGrid2.SetRowCount(Rows);
		m_MeasurementPointGrid2.SetColumnCount(Cols);
		m_MeasurementPointGrid2.SetFixedRowCount(FixRows);
		m_MeasurementPointGrid2.SetFixedColumnCount(FixCols);
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

// 2009.09.17 bagus stress Grid�C�� --{--
	CWnd* GridBox = GetDlgItem(IDC_MEASUREMENT_POINT_GRID);
	CDC* pDC = GridBox->GetDC();
	CRect GridRect;
	GridBox->GetClientRect(&GridRect);

	ReleaseDC(pDC);

	int	nCellsWidth = GridRect.Width() - m_nSysVScrollWidth;
	int	nColWidth = nCellsWidth / Cols;
	int	nFixColWidth = nCellsWidth - (nColWidth * iDispCol);
// 2009.09.17 bagus stress Grid�C�� --}--

	// Row 0 Height
	m_MeasurementPointGrid2.SetRowHeight(0, 18);

// 2009.09.17 bagus stress Grid�C�� --{--
#if 0
	// Row 1 -> RowCount Height
	for(row = 1 ; row < m_MeasurementPointGrid2.GetRowCount() ; row++){
		m_MeasurementPointGrid2.SetRowHeight(row, 18);
	}
#endif
// 2009.09.17 bagus stress Grid�C�� --}--

	// Col 0 Width
// 2009.09.17 bagus stress Grid�C�� --{--
//	m_MeasurementPointGrid2.SetColumnWidth(0, 45);	// No.
	m_MeasurementPointGrid2.SetColumnWidth(0, nFixColWidth);	// No.
// 2009.09.17 bagus stress Grid�C�� --}--

	// Col 1 -> ColumnCount Width
	for (col = 1 ; col < m_MeasurementPointGrid2.GetColumnCount(); col++)
	{
// 2009.09.17 bagus stress Grid�C�� --{--
//		m_MeasurementPointGrid2.SetColumnWidth(col, ItemWidth);
		m_MeasurementPointGrid2.SetColumnWidth(col, nColWidth);
// 2009.09.17 bagus stress Grid�C�� --}--
	}

	///// Title Column /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(col = 0; col < m_MeasurementPointGrid2.GetColumnCount(); col++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = 0;
		Item.col = col;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("%s",ColItem_ENU[col]);
		m_MeasurementPointGrid2.SetItem(&Item);
	}

// 2009.09.17 bagus stress Grid�C�� --{--
#if 0
	///// Title Row /////
	dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	dwTextStyle |= DT_END_ELLIPSIS;

	for(row = 1; row < m_MeasurementPointGrid2.GetRowCount(); row++){
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = row;
		Item.col = 0;
		Item.nFormat = dwTextStyle;
		Item.strText.Format("LxP%d", row);
		m_MeasurementPointGrid2.SetItem(&Item);
	}
#endif
// 2009.09.17 bagus stress Grid�C�� --}--
}

void CRecipeStressStageProgramView::SetMeasurementLineSectionData()
{
	MeasurementLineSectionListGrid_Init();

	GV_ITEM Item;

	int	i ,j;

	int FixRows = 1;
	int FixCols = 1;
	int	row;
//	int	Rows;
	DWORD	dwTextStyle;
	int nRowCount = 0;

	for(i = 0;i < STRESS_LINES_MAX;i++){
		if (!m_StageStressProg.Line[i].bValidLine) {
			continue;
		}
		nRowCount += m_StressConfig.Line[i].dwSectionNum;
	}

	TRY{
		//m_MeasurementPointGrid.SetRowCount(m_StageProgInfoHdr.wNumScans + FixRows);
// 2009.09.17 bagus stress Grid�C�� --{--
//		m_MeasurementPointGrid.SetRowCount(nRowCount + FixRows);
		if ((nRowCount + FixRows) < DEFAULT_GRID_ROWS) {
			m_MeasurementPointGrid.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_MeasurementPointGrid.SetRowCount(nRowCount + FixRows);
		}
// 2009.09.17 bagus stress Grid�C�� --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	row = FixRows;
#if 0
	for(i=FixRows; i<m_StageProgInfoHdr.wNumScans+FixRows; i++){
//		if (!m_StageStressProg.Line[i - FixRows].bValidLine) {
//			continue;
//		}
		Rows = m_StressConfig.Line[i - FixRows].dwSectionNum;

		for (j=row; j<row+Rows; j++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 0;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("L%dS%d", i, j - row + 1);
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StageStressProg.Line[i - FixRows].SectPos[j - row].lScanStartPosX / MICROMETRE));			// �P�ʕϊ�(um->mm)
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StageStressProg.Line[i - FixRows].SectPos[j - row].lScanEndPosX / MICROMETRE));			// �P�ʕϊ�(um->mm)
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 3;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("%.3f", (double)(m_StageStressProg.Line[i - FixRows].SectPos[j - row].lScanPosY / MICROMETRE));			// �P�ʕϊ�(um->mm)
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = j;
			Item.col = 4;
			Item.nFormat = dwTextStyle;
			if (m_StageStressProg.Line[i - FixRows].bScanValid[j - row]) {
				Item.strText.Format("1");
			} else {
				Item.strText.Format("0");
			}
			m_MeasurementPointGrid.SetItem(&Item);
		}

		row += Rows;
	}
#else
	//for(i=FixRows; i<m_StageProgInfoHdr.wNumScans+FixRows; i++){
	int ls_rows = FixRows ;
	for(i = 0;i < STRESS_LINES_MAX;i++){
		//2009.09.19 bagus stress --{--
		//if (!m_StageStressProg.Line[i].bValidLine) {
		if(!m_StressConfig.Line[i].bValidLine){
		//2009.09.19 bagus stress --}--
			continue;
		}

		for (j=0; j<m_StressConfig.Line[i].dwSectionNum; j++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 0;
			Item.nFormat = dwTextStyle;
			Item.strText.Format("L%dS%d", i+1, j+1);
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			//2009.09.24 bagus stress --{--
			//Item.strText.Format("%.3f", (double)(m_StageStressProg.Line[i].SectPos[j].lScanStartPosX / MICROMETRE));			// �P�ʕϊ�(um->mm)
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].SectPos[j].lScanStartPosX / MICROMETRE));			// �P�ʕϊ�(um->mm)
			//2009.09.24 bagus stress --}--
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			//2009.09.24 bagus stress --{--
			//Item.strText.Format("%.3f", (double)(m_StageStressProg.Line[i].SectPos[j].lScanEndPosX / MICROMETRE));			// �P�ʕϊ�(um->mm)
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].SectPos[j].lScanEndPosX / MICROMETRE));			// �P�ʕϊ�(um->mm)
			//2009.09.24 bagus stress --}--
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 3;
			Item.nFormat = dwTextStyle;
			//2009.09.24 bagus stress --{--
			//Item.strText.Format("%.3f", (double)(m_StageStressProg.Line[i].SectPos[j].lScanPosY / MICROMETRE));			// �P�ʕϊ�(um->mm)
			Item.strText.Format("%.3f", (double)(m_StressConfig.Line[i].SectPos[j].lScanPosY / MICROMETRE));			// �P�ʕϊ�(um->mm)
			//2009.09.24 bagus stress --}--
			m_MeasurementPointGrid.SetItem(&Item);

			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 4;
			Item.nFormat = dwTextStyle;
			m_MeasurementPointGrid.SetCellType(ls_rows,4,RUNTIME_CLASS(CGridCellCheck));
			if (m_StageStressProg.Line[i].bScanValid[j]) {
				//Item.strText.Format("1");
				((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->SetCheck(TRUE);
			} else {
				//Item.strText.Format("0");
				((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->SetCheck(FALSE);
			}
			//m_MeasurementPointGrid.SetItem(&Item);
			ls_rows++;
		}
	}
#endif
}

void CRecipeStressStageProgramView::SetMeasurementPointData()
{
// 2009.09.10 bagus stress �C�� --{--
//	MeasurementPointListGrid_Init();
// 2009.09.10 bagus stress �C�� --}--

	GV_ITEM Item;

	int	i ,j;

// 2009.09.10 bagus stress �C�� --{--
	int		LineNum = 0;
	BOOL	bUseLine[STRESS_LINES_MAX];

	memset(bUseLine, FALSE, sizeof(bUseLine));

	for (i=0; i<STRESS_LINES_MAX; i++) {
		//2009.09.19 bagus stress --{--
		//if (!m_StageStressProg.Line[i].bValidLine){
		if (!m_StressConfig.Line[i].bValidLine){
			bUseLine[i] = FALSE;
			continue;
		}
		//2009.09.19 bagus stress --}--

		bUseLine[i] = FALSE;
		for (j=0; j<m_StressConfig.Line[i].dwSectionNum; j++) {
			//2009.09.19 bagus stress --{--
			if (m_StageStressProg.Line[i].bScanValid[j]) {
			//2009.09.19 bagus stress --}--
				bUseLine[i] = TRUE;
				break;
			}
		}

		if (bUseLine[i])
			LineNum++;
	}

	BOOL	bReturn = TRUE;
	for (i=0; i<STRESS_LINES_MAX; i++) {
		if (m_bUseLine[i] != bUseLine[i]) {
			bReturn = FALSE;
			break;
		}
	}

	if (m_bFirst) {
		m_bFirst = FALSE;
	} else {
		if (bReturn) {
			return;
		}
	}

	//
	BOOL bOldUseLine[STRESS_LINES_MAX];
	for (i=0; i<STRESS_LINES_MAX; i++) {
		bOldUseLine[i] = m_bUseLine[i];
		m_bUseLine[i] = bUseLine[i];
	}
// 2009.09.10 bagus stress �C�� --}--

// 2009.09.10 bagus stress �C�� --{--
	MeasurementPointListGrid_Init();
// 2009.09.10 bagus stress �C�� --}--

	int FixRows = 1;
	int FixCols = 1;
	int	nIndex;
// 2009.09.10 bagus stress �C�� --{--
	int	nRowIndex;
// 2009.09.10 bagus stress �C�� --}--
	DWORD	dwTextStyle;

	TRY{
// 2009.09.10 bagus stress �C�� --{--
//		m_MeasurementPointGrid2.SetRowCount((m_StressConfig.dwLiftPinNumberOfLine * PIN_INTERVAL) + FixRows);
// 2009.09.17 bagus stress Grid�C�� --{--
//		m_MeasurementPointGrid2.SetRowCount(LineNum * PIN_INTERVAL + FixRows);
		if ((LineNum * PIN_INTERVAL + FixRows) < DEFAULT_GRID_ROWS) {
			m_MeasurementPointGrid2.SetRowCount(DEFAULT_GRID_ROWS);
		} else {
			m_MeasurementPointGrid2.SetRowCount(LineNum * PIN_INTERVAL + FixRows);
		}
// 2009.09.17 bagus stress Grid�C�� --}--
// 2009.09.10 bagus stress �C�� --}--
	}
	CATCH(CMemoryException, e){
		e->ReportError();
		e->Delete();
		return;
	}
	END_CATCH

	nIndex = 0;
// 2009.09.10 bagus stress �C�� --{--
	nRowIndex = 0;
//	for(i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++){
	int nNewLines = 0;
	for(i=0; i<STRESS_LINES_MAX; i++){
// 2009.10.19 bagus Stress �C�� --{--
#if 0
		if (!m_StageStressProg.Line[i].bValidLine) {
			nIndex += PIN_INTERVAL;
			continue;
		}
#endif
// 2009.10.19 bagus Stress �C�� --}--

		if (!bUseLine[i]) {
			if(bOldUseLine[i])
				nIndex += PIN_INTERVAL;
			continue;
		}
// 2009.09.10 bagus stress �C�� --}--
		for (j=0; j<PIN_INTERVAL; j++) {
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 2009.09.10 bagus stress �C�� --{--
//			Item.row = j + (i * PIN_INTERVAL) + FixRows;
			Item.row = nRowIndex + FixRows;
// 2009.09.10 bagus stress �C�� --}--
			Item.col = 0;
			Item.nFormat = dwTextStyle;
//			Item.strText.Format("L%dP%d", i+1, j+1);
//			Item.strText.Format("%d", nIndex + 1);
			Item.strText.Format("%d", nRowIndex + FixRows);
			m_MeasurementPointGrid2.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 2009.09.10 bagus stress �C�� --{--
//			Item.row = j + (i * PIN_INTERVAL) + FixRows;
			Item.row = nRowIndex + FixRows;
// 2009.09.10 bagus stress �C�� --}--
			Item.col = 1;
			Item.nFormat = dwTextStyle;
			//if(nIndex < m_StageProgInfoHdr.wNumScans){
			if(bOldUseLine[i]){
				Item.strText.Format("%.3f", (double)(m_ScanPoint[nIndex].lX / MICROMETRE));		// �P�ʕϊ�(um->mm)
			}else{
				//Item.strText.Format("%.3f", (double)(m_ScanPoint[nIndex].lX / MICROMETRE));		// �P�ʕϊ�(um->mm)
				Item.strText.Format("%.3f",	(double)(m_StressConfig.Line[i].MeasPos[j].lX / MICROMETRE));
			}
			m_MeasurementPointGrid2.SetItem(&Item);

			dwTextStyle = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 2009.09.10 bagus stress �C�� --{--
//			Item.row = j + (i * PIN_INTERVAL) + FixRows;
			Item.row = nRowIndex + FixRows;
// 2009.09.10 bagus stress �C�� --}--
			Item.col = 2;
			Item.nFormat = dwTextStyle;
			//Item.strText.Format("%.3f", (double)(m_ScanPoint[nIndex].lY / MICROMETRE));		// �P�ʕϊ�(um->mm)
			if(bOldUseLine[i]){
				Item.strText.Format("%.3f", (double)(m_ScanPoint[nIndex].lY / MICROMETRE));		// �P�ʕϊ�(um->mm)
			}else{
				//Item.strText.Format("%.3f", (double)(m_ScanPoint[nIndex].lY / MICROMETRE));		// �P�ʕϊ�(um->mm)
				Item.strText.Format("%.3f",	(double)(m_StressConfig.Line[i].MeasPos[j].lY / MICROMETRE));
			}
			m_MeasurementPointGrid2.SetItem(&Item);

// 2009.09.16 bagus stress �C�� --{--
#if 0
			nIndex++;
			if(bOldUseLine[i]){
				//nNewLines++;
				//nIndex++;
			}
#else
			if(bOldUseLine[i]){
				nIndex++;
			}
#endif
// 2009.09.16 bagus stress �C�� --}--

// 2009.09.10 bagus stress �C�� --{--
			nRowIndex++;
// 2009.09.10 bagus stress �C�� --}--
		}
		//if(!bOldUseLine[i]){
		//	nNewLines++;
		//}
	}

// 2009.09.10 bagus stress �C�� --{--
// debug
	UpDate();

	SamplePointGraph_DataSet();
// 2009.09.10 bagus stress �C�� --}--
}

// ==========================================================================
//
void CRecipeStressStageProgramView::SetData()
{
	char szBuff[256];

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	///// Name /////
	SetDlgItemText(IDC_STAGE_PROGRAM_NAME, m_StageProgInfoHdr.hdr.szName);

	///// Sample - Name /////
	SetDlgItemText(IDC_SAMPLE_NAME, m_StageProgInfoHdr.SampleInfo.szName);

	///// Sample - Shape /////
	m_StageProgInfoHdr.SampleInfo.nShape = m_StageProgInfoHdr.SampleInfo.nShape >= SAMPLE_SHAPE_MAX ? 0 : m_StageProgInfoHdr.SampleInfo.nShape;
	if(l_SystemConfig.nLanguage == 0){
		SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape_ENU[m_StageProgInfoHdr.SampleInfo.nShape]);
	}
	else{
		SetDlgItemText(IDC_SAMPLE_SHAPE, g_lpszSampleShape_JPN[m_StageProgInfoHdr.SampleInfo.nShape]);
	}

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

	///// Line / Section /////
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
// 2009.09.07 bagus stress Line/Section���C�� --{--
//	SetDlgItemText(IDC_STAGE_PROGRAM_LINE_SECTION, szBuff);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);
// 2009.09.07 bagus stress Line/Section���C�� --}--

	///// Point Number /////
	sprintf(szBuff, "%d", m_StageStressProg.dwNumLsScans);
// 2009.09.07 bagus stress Line/Section���C�� --{--
//	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);
	SetDlgItemText(IDC_STAGE_PROGRAM_LINE_SECTION, szBuff);
// 2009.09.07 bagus stress Line/Section���C�� --}--

	///// Point Coord /////
	// SAMPLE���_����̑��΍��W�Ƃ���
	for ( int i = 0 ; i < m_iNumScans ; i++ ) {
		// X
		sprintf(szBuff, "%.3lf", ((double)m_ScanPoint[i].lX) / MICROMETRE );
		m_MeasurementPointGrid2.GridDisp(i + 1, 1, szBuff, DT_RIGHT);

		// Y
		sprintf(szBuff, "%.3lf", ((double)m_ScanPoint[i].lY) / MICROMETRE );
		m_MeasurementPointGrid2.GridDisp(i + 1, 2, szBuff, DT_RIGHT);
	}

	///// Comment /////
	SetDlgItemText(IDC_COMMENT, m_StageProgInfoHdr.hdr.szComment);

// 2009.09.04 bagus stress �f�X�L���[�폜 --{--
	///// Deskew Mode /////
//	if(l_SystemConfig.nLanguage == 0){
//		strcpy(szBuff, g_lpszDeskewMode_ENU[m_StageProgInfoHdr.nDeskewMode]);
//	}
//	else{
//		strcpy(szBuff, g_lpszDeskewMode_JPN[m_StageProgInfoHdr.nDeskewMode]);
//	}
//	SetDlgItemText(IDC_STAGE_DESKEW_MODE, szBuff);
// 2009.09.04 bagus stress �f�X�L���[�폜 --}--
}

// ==========================================================================
//
void CRecipeStressStageProgramView::UpDate()
{
	char szGridText[1024];
	//char *stopstring;
	CString strBuffer;
	int Valid_X;
	int Valid_Y;
	BOOL bShape = FALSE;

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

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
	}
	if ( !bShape ) {
		CString strBuffer;
		LoadStringML(IDS_SHAPE_NOT_EXIST_INP, strBuffer, "The Shape not Existing is Inputted.");
		MessageBox(strBuffer, m_strCaption, MB_OK | MB_ICONSTOP);
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
	m_StageProgInfoHdr.SampleInfo.Origin.lX = atof(strBuffer) * MICROMETRE;

	///// Sample - Origin Y /////
	GetDlgItemText(IDC_SAMPLE_ORIGIN_Y, strBuffer);
	m_StageProgInfoHdr.SampleInfo.Origin.lY = atof(strBuffer) * MICROMETRE;

	///// Point /////
	m_iNumScans = 0;
// 2009.09.10 bagus stress �C�� --{--
//	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
// 2009.09.10 bagus stress �C�� --}--
	for ( i = 0 ; i < SCAN_POINT_MAX ; i++ ) {
		Valid_X = Valid_Y = 0;
		///// X /////
		strcpy(szGridText, m_MeasurementPointGrid2.GetItemText(i + 1, 1));
		Valid_X = strlen(szGridText);
		if ( Valid_X ) {
			m_ScanPoint[i].lX = atof(szGridText) * MICROMETRE;
			//m_ScanPoint[i].lX = (strtol(szGridText, &stopstring, 10)) * MICROMETRE;

			///// Refresh X /////
			strBuffer.Format("%.3lf", ((double)m_ScanPoint[i].lX) / MICROMETRE );
			m_MeasurementPointGrid2.GridDisp(i + 1, 1, strBuffer, DT_RIGHT);
		}

		///// Y /////
		strcpy(szGridText, m_MeasurementPointGrid2.GetItemText(i + 1, 2));
		Valid_Y = strlen(szGridText);
		if ( Valid_Y ) {
			m_ScanPoint[i].lY = atof(szGridText) * MICROMETRE;
			//m_ScanPoint[i].lY = (strtol(szGridText, &stopstring, 10)) * MICROMETRE;

			///// Refresh Y /////
			strBuffer.Format("%.3lf", ((double)m_ScanPoint[i].lY) / MICROMETRE );
			m_MeasurementPointGrid2.GridDisp(i + 1, 2, strBuffer, DT_RIGHT);
		}

		///// Judgement /////
		if ( Valid_X && Valid_Y )
			m_iNumScans++;
		else
			break;
	}

	///// Clear Remaining Grids /////
	for ( ; i < m_MeasurementPointGrid.GetRowCount(); i++ ) {
		m_MeasurementPointGrid2.GridDisp(i + 1, 1, "", DT_RIGHT);
		m_MeasurementPointGrid2.GridDisp(i + 1, 2, "", DT_RIGHT);
	}

	///// Point Count /////
	m_StageProgInfoHdr.wNumScans = (WORD)m_iNumScans;

	///// LS Valid /////
	int ls_rows = 1;
	BOOL bValidLine;
	for(int n = 0;n < STRESS_LINES_MAX;n++){
		bValidLine = FALSE;
		//2009.09.19 bagus stress --{--
		//if (!m_StageStressProg.Line[n].bValidLine) {
		if(!m_StressConfig.Line[n].bValidLine){
			m_StageStressProg.Line[n].bValidLine = FALSE;
		//2009.09.19 bagus stress --}--
			continue;
		}
		for (int j=0; j<m_StressConfig.Line[n].dwSectionNum; j++) {
			//if(m_LineCheckBox.CheckBox[ls_rows-1]->GetCheck()){
			//2009.09.15 bagus stress --{--
			//if(((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows-1,4))->GetCheck()){
			if(((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->GetCheck()){
			//2009.09.15 bagus stress --}--
				m_StageStressProg.Line[n].bScanValid[j] = TRUE;
				//2009.09.19 bagus stress --{--
				bValidLine = TRUE;
				//2009.09.19 bagus stress --}--
				//2009.09.24 bagus stress --{--
				m_StageStressProg.Line[n].SectPos[j] = m_StressConfig.Line[n].SectPos[j];
				//2009.09.24 bagus stress --}--
			}else{
				m_StageStressProg.Line[n].bScanValid[j] = FALSE;
				//2009.09.24 bagus stress --{--
				m_StageStressProg.Line[n].SectPos[j] = m_StressConfig.Line[n].SectPos[j];
				//2009.09.24 bagus stress --}--
			}
			/*
			GV_ITEM Item;
			DWORD dwTextStyle;
			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 4;
			Item.nFormat = dwTextStyle;
			if (m_StageStressProg.Line[i].bScanValid[j]) {
//				Item.strText.Format("1");
				((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->SetCheck(TRUE);
			} else {
//				Item.strText.Format("0");
				((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->SetCheck(FALSE);
			}
			m_MeasurementPointGrid.SetItem(&Item);
			*/
			ls_rows++;
		}
		//2009.09.19 bagu stress  --{--
		m_StageStressProg.Line[n].bValidLine = bValidLine;
		//2009.09.19 bagu stress  --}--
	}

	///// Comment /////
	GetDlgItemText(IDC_COMMENT, m_StageProgInfoHdr.hdr.szComment, RECIPE_COMMENT_LEN + 1);
}

// ==========================================================================
//
BOOL CRecipeStressStageProgramView::LoadRecipeData()
{
	TCHAR szSampleTypeName[RECIPE_NAME_LEN + 1];
	SAMPLEINFO 		SampleInfo;

	int	i, j, k;

	// �V�K�쐬�̏ꍇ
	if ( m_nOpenMode == modeNew ) {
		///// Get Selected Sample Type	Name /////
		m_pMainFrame->GetSelectSampleTypeName(szSampleTypeName);

		///// Sample /////
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
			m_StageProgInfoHdr.SampleInfo.dElasticModulusValue = SampleInfo.dElasticModulusValue;
			_tcscpy(m_StageProgInfoHdr.SampleInfo.tszElasticModulusName,SampleInfo.tszElasticModulusName);

			int	NumOfSections = 0;
			for (i=0; i<STRESS_LINES_MAX; i++) {
//				if ((m_StressConfig.Line[i].dwLiftPinNumberOfLn > 0) && (IsValidLine(i))) {
				if (m_StressConfig.Line[i].dwLiftPinNumberOfLn > 0) {
					if (m_StressConfig.Line[i].bValidLine) {
						NumOfSections += m_StressConfig.Line[i].dwSectionNum;
					}
				}
			}
			m_StageProgInfoHdr.wNumScans = NumOfSections;

			///// Site Pattern /////
			::ZeroMemory(m_SitePoint, sizeof(m_SitePoint));
		}

// 2009.12.14 bagus NANOMAP �ǉ� --{--
		m_StageProgInfoHdr.SampleInfo.StageConfig = m_StageConfig;
// 2009.12.14 bagus NANOMAP �ǉ� --}--

		k = 0;
		for (i=0; i<STRESS_LINES_MAX; i++) {
			for (j=0; j<PIN_INTERVAL; j++) {
				m_ScanPoint[k].lX = m_StressConfig.Line[i].MeasPos[j].lX;
				m_ScanPoint[k].lY = m_StressConfig.Line[i].MeasPos[j].lY;
				k++;
			}
		}

		::ZeroMemory(&m_StageStressProg, sizeof(m_StageStressProg));
		::ZeroMemory(&m_OldStageStressProg, sizeof(m_OldStageStressProg));

		for (i=0; i<STRESS_LINES_MAX; i++) {
			m_StageStressProg.Line[i].bValidLine = m_StressConfig.Line[i].bValidLine;
			for (j=0; j<STRESS_SECTIONS_MAX; j++) {
				m_StageStressProg.Line[i].SectPos[j].lScanStartPosX = m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				m_StageStressProg.Line[i].SectPos[j].lScanEndPosX = m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				m_StageStressProg.Line[i].SectPos[j].lScanPosY = m_StressConfig.Line[i].SectPos[j].lScanPosY;
				m_StageStressProg.Line[i].bScanValid[j] = TRUE;
			}
		}
		m_StageStressProg.dElasticModulusValue = SampleInfo.dElasticModulusValue;
		_tcscpy(m_StageStressProg.tszElasticModulusName,SampleInfo.tszElasticModulusName);
	}
	else{
		///// Stage Program Header /////
		if ( !RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_szRecipeName, RECIPE_FILE_STAGE_PROGRAM) ) {
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
			::ZeroMemory(&m_OldStageProgInfoHdr, sizeof(m_OldStageProgInfoHdr));
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
// 2009.09.11 bagus stress �C�� --{--
//		if ( !RecipeFile_LoadPointList(m_ScanPoint, m_iNumScans, m_szRecipeName) ) {
		if (!LoadScanPoint()) {
// 2009.09.11 bagus stress �C�� --}--
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

		///// Stress Program Header /////
		if ( !RecipeFile_LoadRecipe(&m_StageStressProg, m_szRecipeName, RECIPE_FILE_STAGE_PROGRAM_STRESS) ) {
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageStressProg, sizeof(m_StageStressProg));
			::ZeroMemory(&m_OldStageStressProg, sizeof(m_OldStageStressProg));
		}
		else{
			::CopyMemory(&m_OldStageStressProg, &m_StageStressProg, sizeof(m_StageStressProg));
//			m_iNumScans = (int)m_StageStressProg.wNumScans;
		}

		///// Measurement Point /////
// 2009.09.11 bagus stress �C�� --{--
//		if ( !RecipeFile_LoadPointList(m_ScanPoint, m_iNumScans, m_szRecipeName) ) {
		if (!LoadScanPoint()) {
// 2009.09.11 bagus stress �C�� --}--
			m_nOpenMode = modeNew;
			::ZeroMemory(&m_StageProgInfoHdr, sizeof(m_StageProgInfoHdr));
			::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
		}
		else{
			::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
		}
	}

	if ( m_nOpenMode == modeNew ) {
		CString strBuffer;
		LoadStringML(IDS_UNTITLE_NAME, strBuffer, "Untitled");
		strcpy(m_StageProgInfoHdr.hdr.szName, strBuffer);
		strcpy(m_OldStageProgInfoHdr.hdr.szName, strBuffer);
	}

// 2009.10.09 bagus StagePGM ���ʉ� --{--
//	m_StageProgInfoHdr.wHeadType = HEAD_TYPE_STRESS;
	m_StageProgInfoHdr.wHeadType = STAGE_PROGRAM_HEAD_TYPE;
	m_StageProgInfoHdr.wScanType = SCAN_TYPE_STRESS;
// 2009.10.09 bagus StagePGM ���ʉ� --}--
// 2009.10.13 bagus StagePGM �C�� --{--
	m_OldStageProgInfoHdr.wHeadType = STAGE_PROGRAM_HEAD_TYPE;
	m_OldStageProgInfoHdr.wScanType = SCAN_TYPE_STRESS;
// 2009.10.13 bagus StagePGM �C�� --}--

	return TRUE;
}

// =========================================================================
//
void CRecipeStressStageProgramView::SamplePointGraph_Init()
{
	CString strBuffer;
	CNanoPointGraph::GraphConfig tConfig;

	m_SamplePointGraph.ShowWindow(SW_HIDE);

	////Point Graph Data Reset
// 2009.09.11 bagus stress �C�� --{--
//	m_SamplePointGraph.Reset();
//	m_SamplePointGraph.ResetSection();
	m_SamplePointGraph.ResetAll();
// 2009.09.11 bagus stress �C�� --}--

	////Back Color
	tConfig.BackColor = WAFER_COLOR;
	////Edge Color
	tConfig.EdgeColor[CNanoPointGraph::EDGE_LOWER] = WHITE_COLOR;
	tConfig.EdgeColor[CNanoPointGraph::EDGE_UPPER] = RGB(0x00,0x00,0x00);
	////Status Color
	tConfig.StatusColor[CNanoPointGraph::NOT_MEASURED] = RGB(0x00,0x00,0xFF);
	tConfig.StatusColor[CNanoPointGraph::MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.StatusColor[CNanoPointGraph::SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.StatusColor[CNanoPointGraph::MEASURED] = RGB(0x77,0x77,0x77);
	////Section Color
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_NOT_MEASURED] = RGB(0xFF,0xD7,0x00);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_MEASURING] = RGB(0x00,0xFF,0x55);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_SELECTED] = RGB(0xFF,0x00,0x00);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_MEASURED] = RGB(0x77,0x77,0x77);
	tConfig.SectionStatusColor[CNanoPointGraph::SECTION_NOT_USED] = WAFER_COLOR;
	//2009.09.10 bagus stress --{--
	tConfig.PinColor = RGB(0xFF,0x00,0xFF);
	//2009.09.10 bagus stress --}--

	// 2013.01.09 bagus -->
	tConfig.OriginColor = GREEN_COLOR;
	// 2013.01.09 bagus -->

	////CNanoPointGraph�N���X���T�C�Y(Sample Size)
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//	tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
//	tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	if (((m_StageConfig.Dir.X == 2) || (m_StageConfig.Dir.X == 4)) && ((m_StageConfig.Dir.Y == 1) || (m_StageConfig.Dir.Y == 3))) {
		// X<->Y����ւ�
		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dx;
		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else if (((m_StageConfig.Dir.X == 1) || (m_StageConfig.Dir.X == 3)) && ((m_StageConfig.Dir.Y == 2) || (m_StageConfig.Dir.Y == 4))) {
		// X-Y���̂܂�
		tConfig.SampleSizeX = m_StageProgInfoHdr.SampleInfo.Size.dx;
		tConfig.SampleSizeY = m_StageProgInfoHdr.SampleInfo.Size.dy;
	} else {
		// ���ݒ肪��������
	}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
	m_SamplePointGraph.SetConfig(&tConfig);

	SamplePointGraph_DataSet();
}

// =========================================================================
//
void CRecipeStressStageProgramView::SamplePointGraph_DataSet()
{
	////Point Graph Data Reset
// 2009.09.11 bagus stress �C�� --{--
//	m_SamplePointGraph.Reset();
//	m_SamplePointGraph.ResetSection();
	m_SamplePointGraph.ResetAll();
// 2009.09.11 bagus stress �C�� --}--

	CNanoPointGraph::LSData tLineData;

	int i, j;
	int ls_rows;

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
		break;
	case 2:
		dOffSetValueX = 0;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
//		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dOffSetValueY = 0;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		break;
	case 3:
		dOffSetValueX = 0;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
//		dOffSetValueY = 0;
		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		break;
	case 4:
		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
//		dOffSetValueY = 0;
		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		break;
	case 5:
		dOffSetValueX = m_StageProgInfoHdr.SampleInfo.Size.dx;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --{--
//		dOffSetValueY = m_StageProgInfoHdr.SampleInfo.Size.dy;
		dOffSetValueY = 0;
// 2009.09.08 bagus stress ���_�ʒu�Ή� --}--
		break;
	}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

// 2013.01.09 bagus -->
	if ( m_StageConfig.Dir.X == DIR_RIGHT ||
		 m_StageConfig.Dir.X == DIR_LEFT ) {
		m_SamplePointGraph.SetOriginPointData(dOffSetValueX, dOffSetValueY);
	}
	else {
		m_SamplePointGraph.SetOriginPointData(dOffSetValueY, dOffSetValueX);
	}
// 2013.01.09 bagus <--

// 2009.09.17 bagus stress Grid�C�� --{--
	int nRowCount = 0;

	for(i = 0;i < STRESS_LINES_MAX;i++){
		if (!m_StageStressProg.Line[i].bValidLine) {
			continue;
		}
		nRowCount += m_StressConfig.Line[i].dwSectionNum;
	}
// 2009.09.17 bagus stress Grid�C�� --}--

	ls_rows = 1;
	for (i=0; i<m_StressConfig.dwLiftPinNumberOfLine; i++) {
		if(i >= STRESS_PINS_MAX){
			break;
		}

//2009.09.10 bagus stress --{--
// Pin Data
		for(j = 0; j<m_StressConfig.Line[i].dwLiftPinNumberOfLn;j++){
			long lPinX,lPinY;
			lPinX = lPinY = 0L;
			switch (m_StageConfig.Dir.X) {
			case 1:		// X:��
			default:
				lPinX = dOffSetValueX + m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 2:		// X:��
				lPinY = dOffSetValueX - m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 3:		// X:��
				lPinX = dOffSetValueX - m_StressConfig.Line[i].PinPos[j].lX;
				break;
			case 4:		// X:��
				lPinY = dOffSetValueX + m_StressConfig.Line[i].PinPos[j].lX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1:		// Y:��
				lPinX = dOffSetValueY + m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 2:		// Y:��
			default:
				lPinY = dOffSetValueY - m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 3:		// Y:��
				lPinX = dOffSetValueY - m_StressConfig.Line[i].PinPos[j].lY;
				break;
			case 4:		// Y:��
				lPinY = dOffSetValueY + m_StressConfig.Line[i].PinPos[j].lY;
				break;
			}
			m_SamplePointGraph.AddPinData(lPinX,lPinY);
		}
//2009.09.10 bagus stress --}--
		for (j=0; j<m_StressConfig.Line[i].dwSectionNum; j++) {
// 2009.09.10 bagus stress �C�� --{--
//			if (m_StageStressProg.Line[i].bValidLine) {
//				tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;
//			} else {
//				tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
//			}
// 2009.09.10 bagus stress �C�� --}--

			if(j >= STRESS_SECTIONS_MAX){
				break;
			}
			//2009.09.15 bagus stress --{--
			//if (m_StageStressProg.Line[i].bScanValid[j]) {
			if(ls_rows < m_MeasurementPointGrid.GetRowCount()){
				//2009.09.19 bagus stress --{--
				//if(m_StageStressProg.Line[i].bValidLine){
				if(m_StressConfig.Line[i].bValidLine){
				//2009.09.19 bagus stress --}--
// 2009.09.17 bagus stress Grid�C�� --{--
#if 0
					if (((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->GetCheck()){
						tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;
					} else {
						tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
					}
#else
					if (m_MeasurementPointGrid.GetCell(ls_rows,4)->IsKindOf(RUNTIME_CLASS(CGridCellCheck))) {
						if (((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->GetCheck()){
							tLineData.Status = CNanoPointGraph::SECTION_NOT_MEASURED;
						} else {
							tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
						}
					} else {
						tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
					}
#endif
// 2009.09.17 bagus stress Grid�C�� --}--
					ls_rows++;
				}else{
					tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
				}
			}else{
				tLineData.Status = CNanoPointGraph::SECTION_NOT_USED;
			}
			//2009.09.15 bagus stress --}--

// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//			tLineData.StartX = m_StressConfig.Line[i].SectPos[j].lScanStartPosX + dOffSetValueX;
//			tLineData.StartY = -(m_StressConfig.Line[i].SectPos[j].lScanPosY - dOffSetValueY);
//			tLineData.EndX = m_StressConfig.Line[i].SectPos[j].lScanEndPosX + dOffSetValueX;
//			tLineData.EndY = -(m_StressConfig.Line[i].SectPos[j].lScanPosY - dOffSetValueY);
			switch (m_StageConfig.Dir.X) {
			case 1:		// X:��
			default:
				tLineData.StartX = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndX = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 2:		// X:��
				tLineData.StartY = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndY = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 3:		// X:��
				tLineData.StartX = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndX = dOffSetValueX - m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			case 4:		// X:��
				tLineData.StartY = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanStartPosX;
				tLineData.EndY = dOffSetValueX + m_StressConfig.Line[i].SectPos[j].lScanEndPosX;
				break;
			}

			switch (m_StageConfig.Dir.Y) {
			case 1:		// Y:��
				tLineData.StartX = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndX = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 2:		// Y:��
			default:
				tLineData.StartY = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndY = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 3:		// Y:��
				tLineData.StartX = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndX = dOffSetValueY - m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			case 4:		// Y:��
				tLineData.StartY = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				tLineData.EndY = dOffSetValueY + m_StressConfig.Line[i].SectPos[j].lScanPosY;
				break;
			}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--

			m_SamplePointGraph.AddLineData(&tLineData);
		}
	}

	double Scan_PointIncOffsetX,  Scan_PointIncOffsetY;
	for(int iPoint= 0; iPoint<m_StageProgInfoHdr.wNumScans; iPoint++){
// 2009.09.10 bagus stress �C�� --{--
		if (!m_bUseLine)
			continue;
// 2009.09.10 bagus stress �C�� --}--
// 2009.09.07 bagus stress ���_�ʒu�Ή� --{--
//		Scan_PointIncOffsetX = m_ScanPoint[iPoint].lX + dOffSetValueX;
//		Scan_PointIncOffsetY = -(m_ScanPoint[iPoint].lY - dOffSetValueY);
		switch (m_StageConfig.Dir.X) {
		case 1:		// X:��
		default:
			Scan_PointIncOffsetX = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		case 2:		// X:��
			Scan_PointIncOffsetY = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 3:		// X:��
			Scan_PointIncOffsetX = dOffSetValueX - m_ScanPoint[iPoint].lX;
			break;
		case 4:		// X:��
			Scan_PointIncOffsetY = dOffSetValueX + m_ScanPoint[iPoint].lX;
			break;
		}

		switch (m_StageConfig.Dir.Y) {
		case 1:		// Y:��
			Scan_PointIncOffsetX = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		case 2:		// Y:��
		default:
			Scan_PointIncOffsetY = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 3:		// Y:��
			Scan_PointIncOffsetX = dOffSetValueY - m_ScanPoint[iPoint].lY;
			break;
		case 4:		// Y:��
			Scan_PointIncOffsetY = dOffSetValueY + m_ScanPoint[iPoint].lY;
			break;
		}
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
		m_SamplePointGraph.AddPoint(Scan_PointIncOffsetX, Scan_PointIncOffsetY);
	}


	///// Point Number /////
	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);
}


void CRecipeStressStageProgramView::OnPaint()
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	long lDrawStageSizeX, lDrawStageSizeY;
	CRect StageRect;
	CRect SampleRect;

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
// 2009.12.09 bagus --{--
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
// 2009.12.09 bagus --}--

	if((dStageMaxSizeX / dStageMaxSizeY) > ((StageRectRight - StageRectLeft) / (StageRectBottom - StageRectTop))) {
		lDrawStageSizeX =(StageRectRight - StageRectLeft);
		lDrawStageSizeY = ((StageRectRight - StageRectLeft) * dStageMaxSizeY / dStageMaxSizeX);

		m_dDrawStartStagePosX = StageRectLeft;
		m_dDrawStartStagePosY = StageRectTop + ((StageRectBottom - StageRectTop - lDrawStageSizeY) / 2);
		m_dDrawEndStagePosX = StageRectRight;
		m_dDrawEndStagePosY = m_dDrawStartStagePosY + lDrawStageSizeY;
	}
	else{
		lDrawStageSizeX = ((StageRectBottom - StageRectTop) * dStageMaxSizeX / dStageMaxSizeY);
		lDrawStageSizeY = (StageRectBottom - StageRectTop);

		m_dDrawStartStagePosX = StageRectLeft + ((StageRectRight - StageRectLeft - lDrawStageSizeX) / 2);
		m_dDrawStartStagePosY = StageRectTop;
		m_dDrawEndStagePosX = m_dDrawStartStagePosX + lDrawStageSizeX;
		m_dDrawEndStagePosY = StageRectBottom;
	}

	pDC->Rectangle(CRect(m_dDrawStartStagePosX, m_dDrawStartStagePosY, m_dDrawEndStagePosX, m_dDrawEndStagePosY));
	pDC->SelectObject(m_oldPen);
	pDC->SelectObject(m_oldBrush);

	///// Draw Sample /////
	DrawSample(pDC, m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

// 2013.01.09 bagus -->
// 	///// Draw Origin Point /////
// 	DrawOriginMaker(m_StageConfig.OrgPos);
// 2013.01.09 bagus <--

	///// Draw AxisCoordinate /////
	DrawAxisCoordinate(m_StageConfig.OrgPos, m_StageConfig.Dir.X, m_StageConfig.Dir.Y);

	SamplePointGraph_DataSet();

	// �`��p���b�Z�[�W�Ƃ��� CNanoRecipeUI::OnPaint() ���Ăяo���Ă͂����܂���
	ReleaseDC(pDC);
}

// ==========================================================================
//
void CRecipeStressStageProgramView::DrawSample(CDC* pDC, int iOrg, int DirX, int DirY)
{
	WINDOWPLACEMENT Stageplacement, Sampleplacement;

	///// Region Map /////
	GetDlgItem(IDC_MEASUREMENT_MAP)->GetWindowPlacement(&Stageplacement);

	///// �X�e�[�W�L���͈�(�����l�p�͂��̃T�C�Y�ŕ`�悳��Ă���) /////
// 2009.12.09 bagus --{--
//	double dStageMaxSizeX = m_StageConfig.Size.dX - (2 * m_StageConfig.Edge.dX);
//	double dStageMaxSizeY = m_StageConfig.Size.dY - (2 * m_StageConfig.Edge.dY);
	double dStageMaxSizeX = (m_StageConfig.StageMaxCoord.dRightX - m_StageConfig.StageMaxCoord.dLeftX) - (2 * m_StageConfig.Edge.dX);
	double dStageMaxSizeY = (m_StageConfig.StageMaxCoord.dUpY - m_StageConfig.StageMaxCoord.dDownY) - (2 * m_StageConfig.Edge.dY);
// 2009.12.09 bagus --}--

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

	///// �@�T���v���T�C�Y���X�e�[�W�̑傫���Ƃ菬�������`�F�b�N /////
// 	if ((m_StageProgInfoHdr.SampleInfo.Size.dx > dStageMaxSizeX) || (m_StageProgInfoHdr.SampleInfo.Size.dy > dStageMaxSizeY))
// 		return;
	if ((dSampleSizeX > dStageMaxSizeX) || (dSampleSizeY > dStageMaxSizeY))
		return;
// 2013.01.09 bagus <--

	//�X�e�[�W��Origin Position���X�e�[�W�k�����ɍ��킹�����W�ɕϊ�

	///// �T���v���`��T�C�Y�̌v�Z�i�X�e�[�W�`��T�C�Y�΂���T���v���`��T�C�Y���v�Z�j /////
	double dDrawSampleSizeX =(m_dDrawEndStagePosX - m_dDrawStartStagePosX) * m_StageProgInfoHdr.SampleInfo.Size.dx / dStageMaxSizeX;
	double dDrawSampleSizeY =(m_dDrawEndStagePosY - m_dDrawStartStagePosY) * m_StageProgInfoHdr.SampleInfo.Size.dy / dStageMaxSizeY;

	///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l /////
	double dTotalOrgOffsetPosX, dTotalOrgOffsetPosY;

	///// �T���v���̕`��ʒu /////
	double dtop, dbottom, dright, dleft;

	//Sample�`��ʒu�ݒ�
	//�l�p�`��T���v���̂ݑΉ�
	switch(m_StageProgInfoHdr.SampleInfo.nShape){
	case SAMPLE_SHAPE_RECTANGLE:		//Sample shaped RectAngle
		switch(iOrg){
		case 1: ///// �Z���^�[�	//X���F��+�@Y���F��+�̂ݑΉ�
		default:
			///// �X�e�[�W�ƃT���v���̌��_�I�t�Z�b�g�̍��v�l���v�Z /////
// 2009.12.09 bagus --{--
//			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX - m_StageConfig.Edge.dX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX)) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY - m_StageConfig.Edge.dY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY)) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = ((m_StageConfig.Origin.lX) + (m_StageProgInfoHdr.SampleInfo.Origin.lX) - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = ((m_StageConfig.Origin.lY) + (m_StageProgInfoHdr.SampleInfo.Origin.lY) - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.12.09 bagus --}--

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
			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom	= dtop + dDrawSampleSizeY;
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
// 2009.12.09 bagus --{--
//			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
//			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
			dTotalOrgOffsetPosX = ((m_StageProgInfoHdr.SampleInfo.Origin.lX - (m_StageConfig.StageMaxCoord.dLeftX - (m_StageConfig.Origin.lX * (-1)))) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX);
			dTotalOrgOffsetPosY = ((m_StageProgInfoHdr.SampleInfo.Origin.lY - (m_StageConfig.StageMaxCoord.dDownY - (m_StageConfig.Origin.lY * (-1)))) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY);
// 2009.12.09 bagus --}--
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
			dleft	= m_dDrawStartStagePosX + 1 + dTotalOrgOffsetPosX;
			dright	= dleft + dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
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
			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dbottom = m_dDrawEndStagePosY - 1 - dTotalOrgOffsetPosY;
			dtop	= dbottom - dDrawSampleSizeY;
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
			dTotalOrgOffsetPosX = (m_StageProgInfoHdr.SampleInfo.Origin.lX) * (m_dDrawEndStagePosX - m_dDrawStartStagePosX) / dStageMaxSizeX;
			dTotalOrgOffsetPosY = (m_StageProgInfoHdr.SampleInfo.Origin.lY) * (m_dDrawEndStagePosY - m_dDrawStartStagePosY) / dStageMaxSizeY;
// 2009.10.05 bagus SE --}--
// 2009.09.11 bagus stress �C�� --}--

			///// �T���v���`��ʒu���v�Z /////
			dright	= m_dDrawEndStagePosX - 1 - dTotalOrgOffsetPosX;
			dleft	= dright - dDrawSampleSizeX;
			dtop = m_dDrawStartStagePosY + 1 + dTotalOrgOffsetPosY;
			dbottom	= dtop + dDrawSampleSizeY;
// 2009.09.07 bagus stress ���_�ʒu�Ή� --}--
			break;
		}

		if ((m_dDrawStartStagePosX <= dleft) && (dright <= m_dDrawEndStagePosX) && (m_dDrawStartStagePosY <= dtop) && (dbottom <= m_dDrawEndStagePosY)){
			Sampleplacement.rcNormalPosition.left	= Stageplacement.rcNormalPosition.left + dleft;
			Sampleplacement.rcNormalPosition.right	= Stageplacement.rcNormalPosition.left + dright;
			Sampleplacement.rcNormalPosition.top	= Stageplacement.rcNormalPosition.top  + dtop;
			Sampleplacement.rcNormalPosition.bottom = Stageplacement.rcNormalPosition.top  + dbottom;
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

// ==========================================================================
//
void CRecipeStressStageProgramView::DrawAxisCoordinate(int iOrgPos, int DirX, int DirY)
{
#define ARROW_LENGTH	18
#define ARROW_SIDE	6

	POINT PT[3];
	CString csBuff;
	CString strBuffer;

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
	if( pDC->SelectObject( CFont::FromHandle((HFONT) GetStockObject(ANSI_FIXED_FONT) ) ) == NULL ){
		LoadStringML(IDS_ERROR_SELECT_OBJECT, strBuffer, "Error in SelectObject");
		MessageBox(strBuffer);
	}

	///// �w�i�F�𓧖��ɐݒ� /////
	pDC->SetBkMode(TRANSPARENT);

	///// Draw Center Position /////
	long CenterPosX = StageRectLeft + ((StageRectRight - StageRectLeft) / 2);
	long CenterPosY = StageRectTop + ((StageRectBottom - StageRectTop) / 2);

	pDC->SelectObject(&BrushArrow);
	pDC->SelectObject(&PenArrow);
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
// void CRecipeStressStageProgramView::DrawOriginMaker(int Origin)
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

// 2009.09.04 bagus stress Esc������ --{--
BOOL CRecipeStressStageProgramView::PreTranslateMessage(MSG* pMsg)
{
	/* ���b�Z�[�W�Ώۂ̃E�B���h�E�n���h������R���g���[�� ID ���擾���܂��D	*/
	switch(pMsg->message){
	case WM_KEYDOWN:
		switch(pMsg->wParam){
		case VK_ESCAPE:
			break;
		}
		break;
	}

	return CNanoRecipeUI::PreTranslateMessage(pMsg);
}
// 2009.09.04 bagus stress Esc������ --}--

// 2009.09.07 bagus stress Line/Section���C�� --{--
WORD CRecipeStressStageProgramView::CheckLineSectionNum()
{
	WORD	wRet = 0;

	int	FixRows = 1;

	GV_ITEM	Item;
	int		i;
	BOOL	bValid;

	int	nRowNum;
	nRowNum = m_MeasurementPointGrid.GetRowCount();

	for (i=FixRows; i<nRowNum; i++) {
		Item.mask = GVIF_TEXT;
		Item.row = i;
		Item.col = 4;
		//m_MeasurementPointGrid.GetItem(&Item);
		//bValid = atoi(Item.strText);
// 2009.09.17 bagus stress Grid�C�� --{--
#if 0
		bValid = ((CGridCellCheck *)m_MeasurementPointGrid.GetCell(i,4))->GetCheck();
		if (bValid) {
			wRet++;
		}
#else

		if (m_MeasurementPointGrid.GetCell(i,4)->IsKindOf(RUNTIME_CLASS(CGridCellCheck))) {
			bValid = ((CGridCellCheck *)m_MeasurementPointGrid.GetCell(i,4))->GetCheck();
			if (bValid)
				wRet++;
		}
#endif
// 2009.09.17 bagus stress Grid�C�� --}--
	}

	return wRet;
}
// 2009.09.07 bagus stress Line/Section���C�� --}--


void CRecipeStressStageProgramView::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	CheckMeasurementLineSectionData();

	CNanoRecipeUI::OnTimer(nIDEvent);
}

BOOL CRecipeStressStageProgramView::CheckMeasurementLineSectionData()
{
// 2009.09.17 bagus stress Grid�C�� --{--
//	int	LineCols, LineRows;

//	LineCols = m_MeasurementPointGrid.GetColumnCount();
//	LineRows = m_MeasurementPointGrid.GetRowCount();
// 2009.09.17 bagus stress Grid�C�� --}--

	GV_ITEM Item;

	int	i ,j;

	int FixRows = 1;
	int FixCols = 1;
	DWORD	dwTextStyle;
	int nRowCount = 0;
	long	lLineValid;

	long	lValid = 0;

// 2009.09.10 bagus stress �C�� --{--
	long	NumOfSections = 0;
	BOOL	bUseLine;
	BOOL	bCheckChanged = FALSE;
// 2009.09.10 bagus stress �C�� --}--

	int ls_rows = FixRows ;
	for(i = 0;i < STRESS_LINES_MAX;i++){
		//2009.09.19 bagus stress --{--
		//if (!m_StageStressProg.Line[i].bValidLine) {
		if (!m_StressConfig.Line[i].bValidLine) {
		//2009.09.19 bagus stress --{--
			continue;
		}

// 2009.09.10 bagus stress �C�� --{--
		bUseLine = FALSE;
// 2009.09.10 bagus stress �C�� --}--
		for (j=0; j<m_StressConfig.Line[i].dwSectionNum; j++) {
			Item.mask = GVIF_TEXT;
			Item.row = ls_rows;
			Item.col = 4;
			m_MeasurementPointGrid.GetItem(&Item);
#if 1
//			lLineValid = m_LineCheckBox.CheckBox[ls_rows-1]->GetCheck();
			lLineValid = ((CGridCellCheck *)m_MeasurementPointGrid.GetCell(ls_rows,4))->GetCheck();
#else
			lLineValid = strtol(Item.strText, NULL, 10);
#endif

			dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dwTextStyle |= DT_END_ELLIPSIS;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = ls_rows;
			Item.col = 4;
			Item.nFormat = dwTextStyle;
			if (lLineValid <= 0) {
//				if(Item.strText == "1"){	bCheckChanged = TRUE; }
				if(m_StageStressProg.Line[i].bScanValid[j]){	bCheckChanged = TRUE; }
				m_StageStressProg.Line[i].bScanValid[j] = FALSE;
//				Item.strText.Format("0");
//				m_MeasurementPointGrid.SetItem(&Item);
			} else {
//				if(Item.strText == "0"){	bCheckChanged = TRUE; }
				if(!m_StageStressProg.Line[i].bScanValid[j]){	bCheckChanged = TRUE; }
				m_StageStressProg.Line[i].bScanValid[j] = TRUE;
//				Item.strText.Format("1");
//				m_MeasurementPointGrid.SetItem(&Item);

				lValid++;
// 2009.09.10 bagus stress �C�� --{--
				bUseLine = TRUE;
// 2009.09.10 bagus stress �C�� --}--
			}
			m_StageStressProg.Line[i].bValidLine = bUseLine;

			ls_rows++;
		}
// 2009.09.10 bagus stress �C�� --{--
		if (bUseLine)
			NumOfSections++;
// 2009.09.10 bagus stress �C�� --}--
	}

//	m_StageStressProg.dwNumLsScans = CheckLineSectionNum();
	m_StageStressProg.dwNumLsScans = lValid;
	TCHAR szBuff[256];
	sprintf(szBuff, "%d", m_StageStressProg.dwNumLsScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_LINE_SECTION, szBuff);

// 2009.09.10 bagus stress �C�� --{--
//	m_iNumScans = NumOfSections * PIN_INTERVAL;
//	m_StageProgInfoHdr.wNumScans = m_iNumScans;
	m_StageProgInfoHdr.wNumScans = NumOfSections * PIN_INTERVAL;
	sprintf(szBuff, "%d", m_StageProgInfoHdr.wNumScans);
	SetDlgItemText(IDC_STAGE_PROGRAM_POINT_NUMBER, szBuff);

	SetMeasurementPointData();
// 2009.09.10 bagus stress �C�� --}--

// 2009.09.13 bagus stress --{--
// �\�����X�V
	if(bCheckChanged){
		m_SamplePointGraph.Invalidate();
		Invalidate();
	}
// 2009.09.13 bagus stress --}--

	return ( TRUE );
}

// 2009.09.11 bagus stress �C�� --{--
BOOL CRecipeStressStageProgramView::LoadScanPoint()
{
	STRESS_CONFIG	StressConfig;
	STAGE_COORD		ScanPoint1[SCAN_POINT_MAX];
	STAGE_COORD		ScanPoint2[SCAN_POINT_MAX];
	STAGE_COORD		ScanPoint3[SCAN_POINT_MAX];

	int	i, j, k;

	::ZeroMemory(ScanPoint1, sizeof(ScanPoint1));
	::ZeroMemory(ScanPoint2, sizeof(ScanPoint2));
	::ZeroMemory(ScanPoint3, sizeof(ScanPoint3));

	///// from Stress Config /////
	memset(&StressConfig, 0, sizeof(StressConfig));
	ConfigFile_GetNanoSpecIni(&StressConfig, CONFIG_FILE_STRESS_CONFIG);

	k = 0;
	for (i=0; i<STRESS_LINES_MAX; i++) {
		for (j=0; j<PIN_INTERVAL; j++) {
			ScanPoint1[k].lX = StressConfig.Line[i].MeasPos[j].lX;
			ScanPoint1[k].lY = StressConfig.Line[i].MeasPos[j].lY;
			k++;
		}
	}

	///// from Load File /////
	if ( !RecipeFile_LoadPointList(ScanPoint2, m_iNumScans, m_szRecipeName) ) {
//		::ZeroMemory(ScanPoint2, sizeof(ScanPoint2));
		::CopyMemory(m_ScanPoint, ScanPoint1, sizeof(ScanPoint1));
		return ( FALSE );
	}

	//
	::CopyMemory(m_OldScanPoint, ScanPoint2, sizeof(m_ScanPoint));
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	::CopyMemory(m_ScanPoint, ScanPoint2, sizeof(ScanPoint2));
	return TRUE;

	long	lYPos1, lYPos2, lYPosBack;
	BOOL	bExist;
	int		nIndex;

	nIndex = 0;
	for (i=0; i<SCAN_POINT_MAX; i++) {
		lYPos2 = ScanPoint2[i].lY;
		if (i == 0) {
			lYPosBack = lYPos2;
		} else {
			if (lYPosBack == lYPos2) {
				continue;
			} else {
				lYPosBack = lYPos2;
			}
		}

		bExist = FALSE;
		for (j=0; j<SCAN_POINT_MAX; j++) {
			lYPos1 = ScanPoint1[j].lY;
			if (lYPos2 == lYPos1) {
				bExist = TRUE;
				break;
			}
		}

		if (!bExist) {
			for (k=0; k<SCAN_POINT_MAX; k++) {
				if (lYPos2 == ScanPoint1[k].lY) {
					ScanPoint3[nIndex].lX = ScanPoint1[k].lX;
					ScanPoint3[nIndex].lY = ScanPoint1[k].lY;
					nIndex++;
				}
			}
		} else {
			for (k=0; k<SCAN_POINT_MAX; k++) {
				if (lYPos2 == ScanPoint2[k].lY) {
					ScanPoint3[nIndex].lX = ScanPoint2[k].lX;
					ScanPoint3[nIndex].lY = ScanPoint2[k].lY;
					nIndex++;
				}
			}
		}
	}

	::CopyMemory(m_OldScanPoint, m_ScanPoint, sizeof(m_ScanPoint));
	::ZeroMemory(m_ScanPoint, sizeof(m_ScanPoint));
	::CopyMemory(m_ScanPoint, ScanPoint3, sizeof(ScanPoint3));

	return ( TRUE );
}
// 2009.09.11 bagus stress �C�� --}--

// 2009.11.09 bagus Stress �ǉ��ύX --{--
void CRecipeStressStageProgramView::InitCombo_MaterialElasticModulus()
{
	int	i;
	CString	strItem;
	CString	strSample;

	strSample.Format("%e %s", m_StageStressProg.dElasticModulusValue, m_StageStressProg.tszElasticModulusName);

	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS);
	pCombo->ResetContent();
	for (i=0; i<m_ElasticTable.uiTableNum; i++) {
		strItem.Format("%e %s", m_ElasticTable.ElasticModulus[i].dElasticModulusValue, m_ElasticTable.ElasticModulus[i].tszElasticModulusName);
		pCombo->AddString(strItem);
	}
	int nIndex = pCombo->FindStringExact(-1, strSample);
	pCombo->SetCurSel(nIndex);
}
// 2009.11.09 bagus Stress �ǉ��ύX --}--

void CRecipeStressStageProgramView::OnSelchangeMaterialElasticModulus()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CComboBox* pCombo;
	pCombo = (CComboBox*)GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS);

	int nIndex = pCombo->GetCurSel();

	strcpy(m_StageStressProg.tszElasticModulusName, m_ElasticTable.ElasticModulus[nIndex].tszElasticModulusName);
	m_StageStressProg.dElasticModulusValue = m_ElasticTable.ElasticModulus[nIndex].dElasticModulusValue;
}
