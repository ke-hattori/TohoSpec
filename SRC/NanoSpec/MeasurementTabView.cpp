// MeasurementTabView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MeasurementListDlg.h"
#include "MeasurementGraphDlg.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "MeasurementTabView.h"
#include "NanoSpecDoc.h"
#include "SaveNameOverlapDlg.h"
#include "shlwapi.h"
#include "ManualMeasurementFormView.h"
#include "..\\..\\INC\\NSStage.hxx"
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
#include <Dlgs.h>
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
//2009.12.23 bagus --{--
#include "MeasurementProcessView.h"
//2009.12.23 bagus --}--
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
#include "ManuReMeasReCalcStatDlg.h"
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2009.10.23 bagus Stress �C�� --{--
#define	COLUMN_LENGTH		(12)
// 2009.10.23 bagus Stress �C�� --}--

// #########################################################################
// CMeasurementTabView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CMeasurementTabView, CNanoUI)

// =========================================================================
//
CMeasurementTabView::CMeasurementTabView()
	: CNanoUI(CMeasurementTabView::IDD)
{
	//{{AFX_DATA_INIT(CMeasurementTabView)
	//}}AFX_DATA_INIT
	m_pMeasurementListDlg = NULL;
	m_pMeasurementGraphDlg = NULL;

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;

}

// =========================================================================
//
CMeasurementTabView::~CMeasurementTabView()
{


	if ( m_pMeasurementListDlg != NULL ) {
// �f�X�g���N�g�̒���DestroyWindow���Ăяo���Ȃ�
//		m_pMeasurementListDlg->DestroyWindow();
		delete m_pMeasurementListDlg;
		m_pMeasurementListDlg = NULL;
	}

	if ( m_pMeasurementGraphDlg != NULL ) {
// �f�X�g���N�g�̒���DestroyWindow���Ăяo���Ȃ�
//		m_pMeasurementGraphDlg->DestroyWindow();
		delete m_pMeasurementGraphDlg;
		m_pMeasurementGraphDlg = NULL;
	}
}

// =========================================================================
//
void CMeasurementTabView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementTabView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MEASUREMENT_TAB, m_MeasurementTab);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CMeasurementTabView, CNanoUI)
	//{{AFX_MSG_MAP(CMeasurementTabView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MEASUREMENT_TAB, OnSelchangeMeasurementTab)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_RESULT_DATA, OnUpdateSaveResultData)
	ON_COMMAND(IDM_SAVE_RESULT_DATA, OnSaveResultData)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_SAVE_AS_RESULT_DATA, OnSaveAs)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_AS_RESULT_DATA, OnUpdateSaveAs)
	//}}AFX_MSG_MAP

//	ON_MESSAGE(WM_MEAS_COMP_EVENT, SetMeasurementData)
	ON_MESSAGE(WM_MEAS_START_EVENT, MeasurementStart)
	ON_MESSAGE(WM_MEAS_ERROR_EVENT, MeasurementPointError)
	ON_MESSAGE(WM_MANUAL_START_EVENT, ManualMeasurementStart)

END_MESSAGE_MAP()

// =========================================================================
// CMeasurementTabView ���b�Z�[�W �n���h��

// =========================================================================
//
void CMeasurementTabView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

// 2014.01.17 Bagus Add (Stage None�Ή�) -->
	//�V�X�e���R���t�B�O�ݒ�ǂݍ���
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2014.01.17 Bagus Add (Stage None�Ή�) <--

	///// Edit Tool Bar /////
	if(!m_wndMeasurementBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndMeasurementBar.LoadToolBar(IDR_MEASUREMENT_BAR)){
		TRACE0("Failed to create toolbar\n");
		return;		// �쐬�Ɏ��s
	}

	RECT rect;
	GetClientRect(&rect);
	m_wndMeasurementBar.SetWindowPos(&wndTop, 0, 0, rect.right, TOOLBAR_HEIGHT, SWP_NOMOVE);

	///// Get Selected Recipe Name /////
	//2009.09.29 bagus se --{-- del
	//m_pMainFrame->GetSelectListName(m_szName);
	//2009.09.29 bagus se --{--

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
	this->MRCS_Initialize();
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

	///// Load Data /////
	LoadData();

	///// Tab	/////
	Tab_Init();

/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- { ---------- */
	RECT l_rectThisView;	this->GetWindowRect(&l_rectThisView);
	RECT l_rectMeasTab;		m_MeasurementTab.GetWindowRect(&l_rectMeasTab);
	if ((g_lModelType == MODEL_T3100) && (l_rectThisView.bottom < l_rectMeasTab.bottom)) {
		this->SetScrollSizes(MM_TEXT, CSize(0, 0));
		/* �^�u�̒�ӂ��ړ�	*/
		m_MeasurementTab.SetWindowPos(	0,
							0, 0,
							l_rectMeasTab.right - l_rectMeasTab.left,
							l_rectThisView.bottom - l_rectMeasTab.top,
							SWP_NOZORDER | SWP_NOMOVE);
	}
/* added 2015.04.05 hmenjo FWXGA �Ή�2 ---------- } ---------- */

	///// List	/////
	int dx = 5;
	int dy = 24;
	m_pMeasurementListDlg = new CMeasurementListDlg();
	m_pMeasurementListDlg->Create(CMeasurementListDlg::IDD, &m_MeasurementTab);
	m_pMeasurementListDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Graph /////
//	m_pMeasurementGraphDlg = new CMeasurementGraphDlg();
	if( m_pDoc->GetDispStatus() == MAIN_MENU_MANUAL_MEASUREMENT ){
		// �}�j���A������
		m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_MANUAL);
	}
	else{
		// �ʏ푪��
		m_pMeasurementGraphDlg = new CMeasurementGraphDlg(GRAPHDLG_MODE_SEQ);
	}
	m_pMeasurementGraphDlg->Create(CMeasurementGraphDlg::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Set Current Selection Item (Default Select Tab) ////
	int Index;
	Index = m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay;
	//2009.10.13 bagus CTA --{--
	//�O���t�^�u�̂Ȃ����V�s�͔O�̂��߂ɋ����I��LIST�^�u���f�t�H���g�ɕύX����
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
	//2009.11.03 bagus MS --{--
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
	//2009.11.03 bagus MS --}--
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE)
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE)
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	){
		Index = 0;
	}
	//2009.10.13 bagus CTA --}--
	TabChange(Index);
	m_MeasurementTab.SetCurSel(Index);


	m_bToolbarSaveCompFlg = TRUE; //���肪�s���Ȃ������ꍇ���l�����A�����l��TRUE(�ۑ���)�Ƃ���B
	m_bExitSaveCompFlg = TRUE; //���肪�s���Ȃ������ꍇ���l�����A�����l��TRUE(�ۑ���)�Ƃ���B
	m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
	m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

	m_iProcessStatusOld = -1;
	m_iHostModeOld = -1;
	m_iManuMeasModeOld = -1;

	SetTimer(TABVIEW_ENABLE_CHANGE_TIMER, 50, NULL);
	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE)  //�ʏ푪��E�����W���[
	{
		ButtonEnableChange(MEAS_TAB_VIEW_CREATE);
	}
	m_pMainFrame->m_pMeaTabViewObj = this;

	::SetEvent(m_pMainFrame->m_hEvTabView);


	//2009.09.29 bagus se --{--
	//2009.11.30 bagus �C�� --{--
	//SR�łȂ��Ƃ��蓮����̏ꍇ�ɂ͂�����Processing�Ɉڍs����
	//if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR){
	//2009.11.30 bagus �C�� --}--
		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)  //�}�j���A������
		{
			m_pDoc->SetProcessStatus(PROCESS_PROC);
			//2009.10.30 bagus 2point-distance
			//2�_�ԋ����̏ꍇ�ɂ̓��t�@�����X���g�p���Ȃ��̂Ŏ擾���Ȃ�
			if(
			//2009.11.30 bagus �C�� --{--
			//m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE
			(m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR &&
				m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_DISTANCE)
			//2009.11.30 bagus �C�� --}--
			//2009.11.03 bagus MS --{--
			&& m_MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_MS
			//2009.11.03 bagus MS --}--
			//2010.01.15 bagus GTR --{--
			//�K���g�����ߗ��̏ꍇ�ɂ̓��t�@�����X�����̃^�C�~���O�ŏ������Ȃ�
			&& (m_MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_SR &&
				m_MeasProgInfo.ScanParams.hdr.wScanType != MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)
			//2010.01.15 bagus GTR --}--
			){
				m_pMainFrame->m_pManuMeaFormViewObj->GetReferenceManuStart();
			}else{
				//2009.12.29 bagus �C�� --{--
				//�蓮����͌��XGetReferenceManuStart()�ő���J�n�������Z�b�g���邪
				//�����ɗ���ꍇ�ɂ̓��t�@�����X���擾���Ȃ��̂ł����ŃZ�b�g����B
				///// ����J�n�����擾 /////
				SYSTEMTIME syStartDateTime;
				GetLocalTime(&syStartDateTime);
				m_pDoc->SetStartDateTime(&syStartDateTime);
				//2009.12.29 bagus �C�� --}--
			}
			//2�_�ԋ����̏ꍇ�ɂ̓��t�@�����X���g�p���Ȃ��̂Ŏ擾���Ȃ�
		}
	//}
	//2009.09.29 bagus se --}--

}

// =========================================================================
//
void CMeasurementTabView::OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index = m_MeasurementTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CMeasurementTabView::Tab_Init()
{
//	m_MeasurementTab.InsertItem(0, "List");
//	m_MeasurementTab.InsertItem(1, "Graph");
	//Saiki 20090529 Change ----->
	//m_MeasurementTab.InsertItem(0, "���X�g");
	//m_MeasurementTab.InsertItem(1, "�O���t");
	CString strBuffer;
	LoadStringML(IDS_TAB_LIST, strBuffer, "List");
	m_MeasurementTab.InsertItem(0, strBuffer);
	//2009.10.13 bagus CTA --{--
	//�O���t�^�u�̂Ȃ����V�s�ɑΉ�
	if(m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP
	//2009.11.03 bagus MS --{--
	|| m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS
	//2009.11.03 bagus MS --}--
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR && m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE)
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	|| (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE)
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--
	){
	}else{
		LoadStringML(IDS_TAB_GRAPH, strBuffer, "Graph");
		m_MeasurementTab.InsertItem(1, strBuffer);
	}
	//2009.10.13 bagus CTA --}--
	//Saiki 20090529 Change <-----
}

// =========================================================================
//
void CMeasurementTabView::TabChange(int Index)
{
	m_MeasurementTab.HighlightItem(0, FALSE);
	m_MeasurementTab.HighlightItem(1, FALSE);
	m_MeasurementTab.HighlightItem(Index, TRUE);

	///// Hide All Dialog /////
	m_pMeasurementListDlg->ShowWindow(SW_HIDE);
	m_pMeasurementGraphDlg->ShowWindow(SW_HIDE);

	///// Show Dialog /////
	switch(Index){
	case 0:
		m_pMeasurementListDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pMeasurementGraphDlg->ShowWindow(SW_SHOW);
		break;
	}
}

// =========================================================================
//
void CMeasurementTabView::LoadData()
{
	//2009.09.29 bagus se --{--
	RCP_DATA rcp_data;

	m_pDoc->GetRcpData(&rcp_data);
	m_MainRcpInfo = rcp_data.MainRcpInfo;
	m_MeasProgInfo = rcp_data.MeasProgInfo;
	m_StageProgInfoHdr = rcp_data.StageProgInfoHdr;
	memcpy(m_ScanPoint,rcp_data.ScanPoint,sizeof(m_ScanPoint));
	strcpy(m_szName,m_MainRcpInfo.hdr.szName);
	//RecipeFile_LoadRecipe(&m_MainRcpInfo, m_szName, RECIPE_FILE_MAIN_RECIPE);
	//RecipeFile_LoadRecipe(&m_MeasProgInfo, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
	//RecipeFile_LoadRecipe(&m_StageProgInfoHdr, m_MainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
	//RecipeFile_LoadPointList(m_ScanPoint, m_StageProgInfoHdr.wNumScans, m_MainRcpInfo.MainRcpParam.hdr.szStage);
	//2009.09.29 bagus se --}--

}

// =========================================================================
//
LRESULT CMeasurementTabView::SetMeasurementData(WPARAM wparam, LPARAM lparam)
{
//���g�p
//	m_pMeasurementListDlg->PostMessage(WM_MEAS_COMP_EVENT, wparam, lparam);
//	m_pMeasurementGraphDlg->PostMessage(WM_MEAS_COMP_EVENT, wparam, lparam);
	return 0L;
}

// =========================================================================
//
LRESULT CMeasurementTabView::MeasurementStart(WPARAM wparam/*Point Number*/, LPARAM lparam)
{
	m_pMeasurementListDlg->StartMeasurementInit(wparam);
	return 0L;
}

// =========================================================================
//
LRESULT CMeasurementTabView::MeasurementPointError(WPARAM wparam/*Point Number*/, LPARAM lparam)
{
	m_pMeasurementListDlg->MeasurementError(wparam);
	return 0L;
}

// =========================================================================
//
LRESULT CMeasurementTabView::ManualMeasurementStart(WPARAM wparam, LPARAM lparam)
{
	STAGE_COORD pPosition;
	STAGE_COORD_XYZ CurrPos;

	NS_StageGetPos(&CurrPos);

	pPosition.lX = CurrPos.lX;
	pPosition.lY = CurrPos.lY;

	//���݂̃X�e�[�W���W���擾
	//�}�j���A�����͑��薈�ɍ��W���|�C���g�O���t�֒ǉ�����K�v������B
	//m_pMeasurementListDlg->DrawPointMarker(pPosition.lX, pPosition.lY, MEASUREMENT_STATES_START);
	return 0L;
}





//===========================================================================================
//===========================================================================================
//===========================================================================================
/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConAddRevi
// Purpose		: ��������Ȃǂ̕ۑ�(�ǉ��E�C��)
// �߂�l:	1:����
//			2:���s(�ۑ����Ȃ�)
//			3:���s(�ۑ��L�����Z��)
//�@�@�@�@ 0:���s
int CMeasurementTabView::SaveMeasConAddRevi(LPCTSTR pszMainRcpName, int iSaveWay)
{
	char szSampleId[SAMPLE_ID_LEN + 1];
	//2009.10.13 bagus RS --{--
	//���b�gID�̒ǉ�
	char szLotId[LOT_ID_LEN + 1];
	//2009.10.13 bagus RS --}--
	char szSaveFullFilePath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szSaveFullDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szRefFilePath[MAX_PATH];
	char szToFilePath[MAX_PATH];
	char szFullPathFrom[SAVE_FILE_PATH_NAME_LEN+1]; //��΃p�X�ɕϊ������p�X
	char szFullPathTo[SAVE_FILE_PATH_NAME_LEN+1];//��΃p�X�ɕϊ������p�X
	char szCopyFrom[SAVE_FILE_PATH_NAME_LEN+1];
	char szCopyTo[SAVE_FILE_PATH_NAME_LEN+1];
	static char szMoveFrom[SAVE_FILE_PATH_NAME_LEN+1];
	static char szMoveTo[SAVE_FILE_PATH_NAME_LEN+1];
	char szDeleteDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDeleteFilePath[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDirName[SAVE_FILE_PATH_NAME_LEN + 1];
	char szDirName2[SAVE_FILE_PATH_NAME_LEN + 1]; //�f�B���N�g����(�t�@�C��������g���q������������)
	char szReNameDir[SAVE_FILE_PATH_NAME_LEN + 1];
	char szMessage[255+1];
	RCP_DATA rcp_data;
	BOOL bSampleID;
	BOOL bSaveMeasResult;
	BOOL bSaveOverwrite;
	BOOL bFileNameExistFlg = FALSE; //�ۑ����悤�Ƃ����t�@�C���������݂��邩�̃t���O(TRUE:���݁AFALSE:�Ȃ�)
	int iRtn=0; //���b�Z�[�W�{�b�N�X�߂�l�p
	//2009.10.17 bagus stress --{--
	char szCheckLetters[] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//2009.10.17 bagus stress --{--

	memset(szSampleId, 0, sizeof(szSampleId));
	//2009.10.13 bagus RS --{--
	//���b�gID�̒ǉ�
	memset(szLotId, 0, sizeof(szLotId));
	//2009.10.13 bagus RS --}--
	memset(szSaveFullFilePath, 0, sizeof(szSaveFullFilePath));
	memset(szSaveFullDirPath, 0, sizeof(szSaveFullDirPath));
	memset(szRefFilePath, 0, sizeof(szRefFilePath));
	memset(szToFilePath, 0, sizeof(szToFilePath));
	memset(szFullPathFrom, 0, sizeof(szFullPathFrom));
	memset(szFullPathTo, 0, sizeof(szFullPathTo));
	memset(szCopyFrom, 0, sizeof(szCopyFrom));
	memset(szCopyTo, 0, sizeof(szCopyTo));
	memset(szMoveTo, 0, sizeof(szMoveTo));
	memset(szMoveTo, 0, sizeof(szMoveTo));
	memset(szDeleteDirPath, 0, sizeof(szDeleteDirPath));
	memset(szDeleteFilePath, 0, sizeof(szDeleteFilePath));
	memset(szDirName, 0, sizeof(szDirName));
	memset(szDirName2, 0, sizeof(szDirName2));
	memset(szReNameDir, 0, sizeof(szReNameDir));
	memset(szMessage, 0, sizeof(szMessage));
	memset(&rcp_data, 0, sizeof(rcp_data));

	m_pDoc->GetSampleId(szSampleId); //�T���v��ID�̎擾
	//2009.10.13 bagus RS --{--
	//���b�gID�̒ǉ�
	m_pDoc->GetLotId(szLotId); 	//���b�gID�̎擾
	//2009.10.13 bagus RS --}--
	m_pDoc->GetRcpData(&rcp_data);

	bSampleID = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSampleID; //�T���v��ID���g�p�H
	bSaveMeasResult = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult; //�Z�[�u�t�@�C�����ɃT���v��ID���g�p?(bSampleID==TRUE���̂ݗL��)
	bSaveOverwrite = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite; //�Z�[�u�t�@�C���������t�@�C�����������ꍇ�A�㏑���H
	//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --{--
	BOOL bSaveNameWithDate = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate; //�t�@�C���ɓ�����ǉ�����
	//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --}--
	//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --{--
	if(m_pDoc->GetHostMode() == HOST_REMOTE){
		//�����[�g���͏�Ɉȉ��̃I�v�V������L���ɂ���
		bSampleID = TRUE;
		bSaveMeasResult = TRUE;
		bSaveOverwrite = TRUE;
		bSaveNameWithDate = TRUE;	//�A���X�g���X����̏ꍇ�ɂ͎g�p�Ƃ���TRUE�ł����Ă������͕t���Ȃ�
	}
	//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --}--


	//2009.08.28 bagus stress --{--
	//���ɕۑ����ׂ��A�Ԃ̌���
	//z�܂ōs������Z���㏑��
	int nNextSaveNo = strlen(szCheckLetters)-1;
	//2009.12.21 bagus �C�� --}--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if(!m_pDoc->IsStressRefereneMeasuremnt()){
			char szCheckDir[MAX_PATH+1];
			//2009.10.17 bagus stress --{--
			//for(int nCheckNo = 1;nCheckNo < INT_MAX;nCheckNo++){
			//	sprintf(szCheckDir,"%s\\%s\\%s_%d%s",g_szResult_Dir,szSampleId,pszMainRcpName,nCheckNo,DAT_EXT);
//2009.12.21 bagus �C�� --{--
//			for(int nCheckNo = 0;nCheckNo < strlen(szCheckDir);nCheckNo++){
			for(int nCheckNo = 0;nCheckNo < strlen(szCheckLetters);nCheckNo++){
//2009.12.21 bagus �C�� --}--
				sprintf(szCheckDir,"%s\\%s\\%s_%c%s",g_szResult_Dir,szSampleId,pszMainRcpName,szCheckLetters[nCheckNo],DAT_EXT);
			//2009.10.17 bagus stress --}--
				if(!FileOrDirExists(szCheckDir)){
					nNextSaveNo = nCheckNo;
					break;
				}
			}
		}else{
			//2009.09.17 bagus stress --{--
			//pszMainRcpName = STRESS_REFERENCE_NAME;
			//2009.09.17 bagus stress --}--
		}
		if(iSaveWay == MEASUREMENT_TOOLBAR_SAVE
		|| iSaveWay == MEASUREMENT_TOOLBAR_SAVE_AS){
			iSaveWay = MEASUREMENT_EXIT_SAVE;
		}
		//�X�g���X�̏ꍇ�̓t�@�C�����𕷂��K�v���Ȃ�
		bSaveMeasResult = TRUE;
		//2009.09.15 bagus stress --{--
		//�X�g���X�̃��t�@�����X�͏�ɏ㏑��
		if(m_pDoc->IsStressRefereneMeasuremnt()){
			bSaveOverwrite = TRUE;
		}
		//2009.09.15 bagus stress --}--
	}
	//2009.08.28 bagus stress --}--

//=========================================================
//[�ۑ���p�X�擾]==========================================
	switch(iSaveWay)
	{
		case MEASUREMENT_EXIT_SAVE:

			if((m_pDoc->GetHostMode() != HOST_LOCAL)&&(bSaveOverwrite == FALSE)) //�����[�g ���� �㏑���m�F���K�v�ȏꍇ
			{
				//2009.08.28 bagus stress --{--
				//�X�g���X�̃f�B���N�g���\���ɑΉ�
				//�����[�g���̓t�@�C���d�����s����
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
					if(m_pDoc->IsStressRefereneMeasuremnt()){
						sprintf(szSaveFullFilePath, "%s%s\\%s%s", g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s", g_szResult_Dir , szSampleId,STRESS_REFERENCE_NAME  );
					}else{
						//2009.10.17 bagus stress --{--
						//sprintf(szSaveFullFilePath, "%s%s\\%s_%d%s", g_szResult_Dir , szSampleId ,pszMainRcpName, nNextSaveNo, DAT_EXT); //
						//sprintf(szSaveFullDirPath, "%s%s\\%s_%d", g_szResult_Dir , szSampleId,pszMainRcpName, nNextSaveNo );
						sprintf(szSaveFullFilePath, "%s%s\\%s_%c%s", g_szResult_Dir , szSampleId ,pszMainRcpName, szCheckLetters[nNextSaveNo], DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s_%c", g_szResult_Dir , szSampleId,pszMainRcpName, szCheckLetters[nNextSaveNo] );
						//2009.10.17 bagus stress --{--
					}
				}else{
					//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --{--
					if(!bSaveNameWithDate){
						//���C�����V�s�̐ݒ�ɂ��ۑ����̃f�B���N�g���ƌ��ʃt�@�C���ɑ���J�n������t��������
						sprintf(szSaveFullFilePath, "%s%s%s", g_szResult_Dir , szSampleId , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s", g_szResult_Dir , szSampleId);
					}else{
						//YYYYMMDD_hhmmss => 15
						char szNameWithDate[15+1];
						SYSTEMTIME syStartDateTime;
						m_pDoc->GetStartDateTime(&syStartDateTime);
						memset(szNameWithDate,NULL,sizeof(szNameWithDate));
						_snprintf(szNameWithDate,15,"%04d%02d%02d_%02d%02d%02d",
								syStartDateTime.wYear,
								syStartDateTime.wMonth,
								syStartDateTime.wDay,
								syStartDateTime.wHour,
								syStartDateTime.wMinute,
								syStartDateTime.wSecond
							);
						sprintf(szSaveFullFilePath, "%s%s_%s%s", g_szResult_Dir , szSampleId , szNameWithDate , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s_%s", g_szResult_Dir , szSampleId , szNameWithDate);
					}
					//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --}--
				}
				//2009.08.28 bagus stress --}--
				if(( FileOrDirExists(szSaveFullFilePath)==TRUE ) //�t�@�C���d���L��
				||( FileOrDirExists(szSaveFullDirPath)==TRUE )) //�f�B���N�g���d���L��
				{
					AlarmIf_Set(ALID_MeasurementResultNameDuplication); //���茋�ʕۑ����d��(�����[�g��)

					int iRtnOverlap = SaveOverlap(szSaveFullFilePath);
					if(iRtnOverlap != -1) //-1�͏������s
					{
//						return iRtnOverlap;
						return 2; //�����[�g���͑���I����ɑ����ʂɂƂǂ܂�ƃ}�Y�C�̂ŁA����L�����Z���ł��ۑ������Ƀ��V�s�I����ʂɔ�����B
					}
				}
			}
// 2009.12.17 bagus Stress --{--
#if 0
			else if(((bSampleID == TRUE)&&(bSaveMeasResult == TRUE))
					||((m_pDoc->GetHostMode() != HOST_LOCAL)&&(bSaveOverwrite == TRUE)))
#else
			else if(((bSampleID == TRUE)&&(bSaveMeasResult == TRUE))
					||((m_pDoc->GetHostMode() != HOST_LOCAL)))
#endif
// 2009.12.17 bagus Stress --}--
			{
				//2009.08.28 bagus stress --{--
				//sprintf(szSaveFullFilePath, "%s%s%s", g_szResult_Dir , szSampleId , DAT_EXT); //
				if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
					if(m_pDoc->IsStressRefereneMeasuremnt()){
						sprintf(szSaveFullFilePath, "%s%s\\%s%s", g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s", g_szResult_Dir , szSampleId,STRESS_REFERENCE_NAME  );
					}else{
						//2009.10.17 bagus stress --{--
						//sprintf(szSaveFullFilePath, "%s%s\\%s_%d%s", g_szResult_Dir , szSampleId ,pszMainRcpName, nNextSaveNo, DAT_EXT); //
						//sprintf(szSaveFullDirPath, "%s%s\\%s_%d", g_szResult_Dir , szSampleId,pszMainRcpName, nNextSaveNo );
						sprintf(szSaveFullFilePath, "%s%s\\%s_%c%s", g_szResult_Dir , szSampleId ,pszMainRcpName, szCheckLetters[nNextSaveNo], DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s\\%s_%c", g_szResult_Dir , szSampleId,pszMainRcpName, szCheckLetters[nNextSaveNo] );
						//2009.10.17 bagus stress --{--
					}
				}else{
					//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --{--
					if(!bSaveNameWithDate){
						//���C�����V�s�̐ݒ�ɂ��ۑ����̃f�B���N�g���ƌ��ʃt�@�C���ɑ���J�n������t��������
						sprintf(szSaveFullFilePath, "%s%s%s", g_szResult_Dir , szSampleId , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s", g_szResult_Dir , szSampleId);
					}else{
						//YYYYMMDD_hhmmss => 15
						char szNameWithDate[15+1];
						SYSTEMTIME syStartDateTime;
						m_pDoc->GetStartDateTime(&syStartDateTime);
						memset(szNameWithDate,NULL,sizeof(szNameWithDate));
						_snprintf(szNameWithDate,15,"%04d%02d%02d_%02d%02d%02d",
								syStartDateTime.wYear,
								syStartDateTime.wMonth,
								syStartDateTime.wDay,
								syStartDateTime.wHour,
								syStartDateTime.wMinute,
								syStartDateTime.wSecond
							);
						sprintf(szSaveFullFilePath, "%s%s_%s%s", g_szResult_Dir , szSampleId , szNameWithDate , DAT_EXT); //
						sprintf(szSaveFullDirPath, "%s%s_%s", g_szResult_Dir , szSampleId , szNameWithDate);
					}
					//2009.12.17 bagus �ۑ��t�@�C�����̕ύX --}--
				}
				//2009.08.28 bagus stress --}--
			}
			else
			{
				if(!GetMeasSaveFileName(szSaveFullFilePath))
				{
					return 3;

				}
			}
			break;

		case MEASUREMENT_TOOLBAR_SAVE:
			if(!GetMeasSaveFileName(szSaveFullFilePath))
			{
				return 3;

			}
			break;
		//Saiki 20090401 Add ----->
		case MEASUREMENT_TOOLBAR_SAVE_AS:
			if(bSaveOverwrite)
				bSaveOverwrite = FALSE;
			if(!GetMeasSaveFileName(szSaveFullFilePath))
			{
				return 3;

			}
			break;
		//Saiki 20090401 Add <-----
		default:
			//�����Ȃ�
			break;
	}


//=========================================================================
//[�ۑ���t�@�C�������d�����Ă����ꍇ�A�����ɂ��Ώۃt�@�C��(�t�H���_)���폜]==
	//�ۑ���t�@�C���������ɑ��݂��Ă����ꍇ�̏���
	memset(szSaveFullDirPath, 0, sizeof(szSaveFullDirPath));
	_tsplitpath(szSaveFullFilePath, NULL, szDirName, szDirName2, NULL);
	sprintf(szSaveFullDirPath, "%s%s", szDirName, szDirName2); //
	if(( FileOrDirExists(szSaveFullFilePath)==TRUE ) //�t�@�C���d���L��
	||( FileOrDirExists(szSaveFullDirPath)==TRUE )) //�f�B���N�g���d���L��
	{
		bFileNameExistFlg = TRUE; //�ۑ��\��t�@�C�����d��
		sprintf(szDeleteFilePath, "%s", szSaveFullFilePath); //�폜�Ώۃt�@�C���p�X�����쐬
		sprintf(szDeleteDirPath, "%s", szSaveFullDirPath); //�폜�Ώۃt�H���_�p�X�����쐬
	}
	else
	{
		//�d���i�V
		bFileNameExistFlg = FALSE; //
	}


	//�ۑ��t�@�C�����d�����Ă����ꍇ�̏㏑��(���m�ɂ͍폜+�ړ�)�̉�����
	if(bFileNameExistFlg == TRUE) //�ۑ����ɏd������
	{
/* modified 2016.08.31 hmenjo ���ʃf�[�^����ɍ폜 ---------- { ---------- */
//		if((m_pDoc->GetHostMode() == HOST_LOCAL)
//		&&(bSampleID == TRUE)&&(bSaveMeasResult == TRUE)&&(bSaveOverwrite == FALSE))
//		{
//			int iRtnOverlap = SaveOverlap(szSaveFullFilePath);
//			if(iRtnOverlap != -1) //-1�͏������s
//			{
//				return iRtnOverlap;
//			}
//		}
//
//		//�㏑���̏����Ƃ��āA�Ώۃt�@�C��(�t�H���_���폜)
//		remove(szDeleteFilePath); //�Ώۃt�@�C���폜
//		//�����̎��_�ł́A�d���̓t�@�C�����݂̂Ŕ��f���Ă���̂ŁA�f�B���N�g���폜�͒��O�ő��݂��m�F����K�v������B
//		if(FileOrDirExists(szDeleteDirPath)==TRUE)
//		{
//			m_pMainFrame->DeleteDirectory(_T(szDeleteDirPath)); //�Ώۃf�B���N�g�����ċA�I�ɍ폜
//		}
/* modified 2016.08.31 hmenjo ���ʃf�[�^����ɍ폜 ----------              */
		int l_iRtnOverlap = 0;
		if (
			(m_pDoc->GetHostMode() == HOST_LOCAL) &&	/* ���[�J���ł��邱��	*/
			(bSampleID == TRUE) &&						/* ���C�����V�s�Łu�T���v��ID���g�p�v���I��	*/
			(bSaveMeasResult == TRUE) &&				/* ���C�����V�s�Łu�T���v��ID���Ŏ����ۑ�����v���I��	*/
			(bSaveOverwrite == FALSE)					/* ���C�����V�s�Łu�㏑���ۑ�����v���I�t	*/
		) {
			/* �㏑���m�F�_�C�A���O	*/
			l_iRtnOverlap = SaveOverlap(szSaveFullFilePath);
			if (-1 == l_iRtnOverlap) {
				/* �㏑���ۑ�(SAVE)�ŏ������s	*/
				/*	�㏑���O�̍폜�����͎��� if (-1 == l_iRtnOverlap) �u���b�N�ł��D	*/
			} else if (-2 == l_iRtnOverlap) {
				/* ���O��t���ĕۑ�(SAVE AS)�ŏ������s	*/
				/*	szSaveFullFilePath �Ŏw�肳�ꂽ���ʃf�[�^�����݂���ꍇ������܂��̂ŁC
					���݂���ꍇ�͍폜���܂��D
					���̏ꍇ�CszSaveFullFilePath ����Ώۃf�B���N�g���𐶐����܂��D
					�Ώۂ����ʃf�[�^�p�̃f�B���N�g���ł��̂ŁC���̃t�H�[�}�b�g��
					�f�[�^�̑��݂͎d�l�゠�肦�Ȃ�����ł��D	*/
				if (0xffffffff != ::GetFileAttributes(szSaveFullFilePath)) {
					/* ���݂����̂ō폜���Ă����܂�	*/
					remove(szSaveFullFilePath);
					char l_szDeleteDirPath[SAVE_FILE_PATH_NAME_LEN + 1];
					strcpy(l_szDeleteDirPath, szSaveFullFilePath);
					if (0 != strrchr(l_szDeleteDirPath, _T('.'))) {
						*(strrchr(l_szDeleteDirPath, _T('.'))) = 0x00;
						if (FALSE != FileOrDirExists(l_szDeleteDirPath)) {
							/* �Ώۃf�B���N�g�����ċA�I�ɍ폜	*/
							m_pMainFrame->DeleteDirectory(_T(l_szDeleteDirPath));
						}
					}
				}
			} else {
				/* �ۑ����Ȃ�(CANCEL)	*/
				return l_iRtnOverlap;
			}
		} else {
			l_iRtnOverlap = -1;
		}
		if (-1 == l_iRtnOverlap)  {
			/* �㏑���̏����Ƃ��āC�Ώۃt�@�C��(�t�H���_���폜)	*/
			remove(szDeleteFilePath); /* �Ώۃt�@�C���폜	*/
			/*	�����̎��_�ł́C�d���̓t�@�C�����݂̂Ŕ��f���Ă���̂ŁC
				�f�B���N�g���폜�͒��O�ő��݂��m�F����K�v������D	*/
			if (FileOrDirExists(szDeleteDirPath) == TRUE) {
				m_pMainFrame->DeleteDirectory(_T(szDeleteDirPath));	/* �Ώۃf�B���N�g�����ċA�I�ɍ폜	*/
			}
		}
/* modified 2016.08.31 hmenjo ���ʃf�[�^����ɍ폜 ---------- } ---------- */
	}

//==========================================================
//[���t�H���_�Ɉꎞ�ۑ�]=====================================
	//���t�@�����X�t�@�C���R�s�[
	sprintf(szRefFilePath, "%s%s%s", g_szData_Ref_Dir, /*szRecipeName*/pszMainRcpName, DAT_EXT);
	sprintf(szToFilePath, "%s%s%s", g_szData_Measured_Data_Temp_Dir, pszMainRcpName, REFERENCE_EXT); //hibino [��]���茋�ʕۑ����Ƀ��t�@�����X�t�@�C�����̊g���q��.ref�ɕύX(�R�s�[����.dat�̂܂�)
	CopyFile(szRefFilePath, szToFilePath, FALSE); 						// �����t�@�C���������Ă��A�㏑������

	//2009.09.01 bagus stress --{--
	//�X�g���X�̃��t�@�����X���������ꍇ�ɂ�����R�s�[����
	//StressRefRaw.dat -> ref\StressRef\SampleID.dat
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		sprintf(szRefFilePath, "%s%s%s",g_szData_Measured_Data_Temp_Dir ,_T("StressRefRawData"), DAT_EXT);
		sprintf(szToFilePath, "%s%s%s", g_szData_StressRef_Dir,szSampleId , DAT_EXT);
		CopyFile(szRefFilePath, szToFilePath, FALSE);
	}

	//2009.09.01 bagus stress --}--

	//���V�s�f�[�^���t�@�C���ɕۑ�(����O�ɕ�������̂̂�)
	if(m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) //�}�j���A������ȊO
	{
		RecipeFile_SaveCurrRecipeInfo(pszMainRcpName);
	}
	else //�}�j���A������
	{
		RecipeFile_SaveCurrRecipeInfo(pszMainRcpName, 1);
	}

	//�f�[�^������
	SaveMeasConRevi(pszMainRcpName);

	//�f�[�^�ǋL
	SaveMeasConAdd(pszMainRcpName); //����f�[�^�ȂǒǋL

//==========================================================
//[�t�@�C��rename��move(���K�̏ꏊ�֊i�[)]====================
	//�t�@�C��rename��move(���K�̏ꏊ�֊i�[)

	_tsplitpath(szSaveFullFilePath, NULL, szDirName, szReNameDir, NULL); //�p�X�𕪉�


	//�t�@�C��move&rename
	sprintf(szMoveFrom, "%s%s%s", g_szData_Measured_Data_Temp_Dir, pszMainRcpName, DAT_EXT);
	sprintf(szMoveTo, "%s", szSaveFullFilePath);
	MoveFile(szMoveFrom, szMoveTo ); //Move(Copy�ł͂Ȃ�)

	//2010.01.13 bagus 2point --{--
	//2�_�ԋ����̏ꍇ�ɂ̓t�H���_�����K�v���Ȃ��̂ł�����Return
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		return 1;
	}
	//2010.01.13 bagus 2point --}--
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_COMPEASE){
		return 1;
	}
	// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--

	//��RecipeFile_SaveCurrRecipeInfo()��2����s�����[Point]���ǋL����Ă��܂��̂ŁA���move���K�v
	//���t�H���_�͎c���Ă����Ȃ���2��ڂ̃Z�[�u�̎��ɍ��邩��copy�ɂ���B
	//�t�H���_copy&rename
	sprintf(szCopyFrom, "%s%s", g_szData_Measured_Data_Temp_Dir, "*");
	sprintf(szCopyTo, "%s%s%s", szDirName, szReNameDir, "\\");
	_fullpath(szFullPathFrom, (const char *)szCopyFrom, sizeof(szFullPathFrom)); //��΃p�X�ɕϊ�
	_fullpath(szFullPathTo, (const char *)szCopyTo, sizeof(szFullPathTo)); //��΃p�X�ɕϊ�
	MakeDirectory(szFullPathTo); //�󂯑��f�B���N�g���쐬
	//2009.09.17 bagus stress --{--
	//�X�g���X�̃��t�@�����X�t�@�C�����R�s�[����
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if(m_pDoc->IsStressRefereneMeasuremnt()){
			sprintf(szRefFilePath, "%s%s%s",g_szData_Measured_Data_Temp_Dir,pszMainRcpName, DAT_EXT);
			//sprintf(szToFilePath, "%s%s%s", g_szData_Measured_Data_Temp_Dir,STRESS_REFERENCE_NAME, DAT_EXT);
			sprintf(szToFilePath, "%s%s\\%s%s",g_szResult_Dir , szSampleId ,STRESS_REFERENCE_NAME, DAT_EXT);
			MoveFileEx(szRefFilePath, szToFilePath,MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED);
		}
	}
	//2009.09.17 bagus stress --}--
	int iCpChk = FolderCopy(szFullPathFrom, szFullPathTo); //�R�s�[(Move�ł͂Ȃ�)


	//�ŏI���茋�ʕۑ�����DATA��t�H���_�Ɋi�[
	// write NanoSpec ini file
	char szLastSaveInfFilePath[_MAX_PATH];
	memset(szLastSaveInfFilePath, 0, sizeof(szLastSaveInfFilePath));
	char szMoveToFullPath[SAVE_FILE_PATH_NAME_LEN+1];
	memset(szMoveToFullPath, 0, sizeof(szMoveToFullPath));
	_fullpath(szMoveToFullPath, (const char *)szMoveTo, sizeof(szMoveToFullPath)); //��΃p�X�ɕϊ�

/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� NanoSpec.exe ---------- { ----------	*/
//	sprintf(szLastSaveInfFilePath, "%s%s%s", DATA_DIR, "LastSaveInf", INF_EXT);
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� NanoSpec.exe ----------				*/
	_stprintf(szLastSaveInfFilePath, _T("%sLastSaveInf") _T(INF_EXT), g_szData_Dir);
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� NanoSpec.exe ---------- } ----------	*/
	::WritePrivateProfileString("LASTSAVEINF",
								"MeasuredDataPath",
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� NanoSpec.exe ---------- { ----------	*/
//								szMoveTo,
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� NanoSpec.exe ----------				*/
								szMoveToFullPath,
/* modified 2009.07.07 hmenjo dll ���΃p�X�Ή� NanoSpec.exe ---------- } ----------	*/
								szLastSaveInfFilePath);


	return (int)((0 == iCpChk)? 1 : 0);
}


/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConRevi
// Purpose		: ���������K�v�ȏꍇ�g�p
// Parameters	: pszMainRcpName  ---> �ǋL�Ώۃt�@�C����(�p�X)
void CMeasurementTabView::SaveMeasConRevi(LPCTSTR pszMainRcpName)
{

//�f�[�^������
	int iDummyStartWaveLen;
	int iDummyEndWaveLen;
	int iDummyOpticalFilter;
	CString strBuffer;
	CStringArray arrayStringItem;
	int iItemNo[3];
	char szFilePath[MAX_PATH + 1];
	RCP_DATA rcp_data;
	TCHAR szDdeTextItem[1024];
	char *token;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
	int iRecalibMax;
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--

/* added 2015.06.01 hmenjo ���˗� NONE �o�O ---------- { ---------- */
	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
/* added 2015.06.01 hmenjo ���˗� NONE �o�O ---------- } ---------- */
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	if ( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS ) {
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� -->
		if ( rcp_data.MeasProgInfo.ScanParams.hdr.wHeadType == HEAD_TYPE_COMPEASE ) {
			iRecalibMax = sizeof(g_lpszCompEASERecalibItem_ENU) / sizeof(g_lpszCompEASERecalibItem_ENU[0]);
			for ( int i = 0; i < iRecalibMax; i++ ) {
				arrayStringItem.Add(g_lpszCompEASERecalibItem_ENU[i]);
			}
		}
		else {
	// �X�g���e�W�[���̑��荀�ڃC���f�b�N�X��ǉ�
		arrayStringItem.Add("None");
		if ( MEAS_GetAdapScriptFileDescriptDdeTextItem(szDdeTextItem, iDummyStartWaveLen, iDummyEndWaveLen, iDummyOpticalFilter, rcp_data.MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry) ){
			token = strtok(szDdeTextItem, _T(","));
			while ( token ) {
				arrayStringItem.Add(token);
				token = strtok(NULL, _T(","));
			}
		}

			iRecalibMax = arrayStringItem.GetSize();
		}
// 2013.02.01 bagus CompleteEASE�w�b�h�ǉ� <--

		// SR��MainRecipeRevi�t�@�C���̃Z�N�V�����C�L�[�C�f�t�H���g�C�ϐ��̐ݒ�
		LPCTSTR pszKey[] =	{ "FirstRecalItem", "SecondRecalItem",	"ThirdRecalItem"};
		iItemNo[0] = rcp_data.MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1];
		iItemNo[1] = rcp_data.MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2];
		iItemNo[2] = rcp_data.MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3];

		for ( int i = 0; i < 3; i++ ) {
			if ( iItemNo[i] < iRecalibMax )
				strBuffer = arrayStringItem.GetAt(iItemNo[i]);
			else
				strBuffer.Format("%d ;*****RecalibItem IndexOver", iItemNo[i]);

			// write NanoSpec ini file
/* deleted 2015.06.01 hmenjo ���˗� NONE �o�O ---------- { ---------- */
//			sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
/* deleted 2015.06.01 hmenjo ���˗� NONE �o�O ---------- } ---------- */
			::WritePrivateProfileString("MainRecipe",
										pszKey[i],
										strBuffer,
										szFilePath);
		}
	}


//===================================================
	// write NanoSpec ini file
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)  //�}�j���A������
	{
		::WritePrivateProfileString("MainRecipe",
									"StageProgram",
									"None",
									szFilePath);
	}

}

/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConRevi2
// Purpose		: ini�t�@�C�������o��
// Parameters	: pDesc 		  --->
//				  iDescCount	  ---> �������݃L�[��
//				  pszMainRcpName  ---> �ǋL�Ώۃt�@�C����(�p�X)
void CMeasurementTabView::SaveMeasConRevi2(RECIPE_FILE* pDesc, int iDescCount, LPCTSTR pszMainRcpName)
{
	int iIndex/*, iDescCount*/;
	LPCSTR section;
//	char buff[RECIPE_COMMENT_LEN + 1];
	char buff[1024 + 1];
	char szFilePath[_MAX_PATH];


	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);


	for(iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if (strlen((pDesc + iIndex)->section) > 0)
			section = (pDesc + iIndex)->section;

		// set file data
		memset(buff, 0, sizeof(buff));
		switch((pDesc + iIndex)->type){
		case 'b':
			strcpy(buff, (*(BOOL*)(pDesc + iIndex)->data)? "TRUE" : "FALSE");
			break;
		case 'B':
			_itoa(*(BYTE*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'S':
			_itoa(*(short*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'I':
			_itoa(*(int*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'L':
			_ltoa(*(long*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'F':
			sprintf(buff, "%.3f", *(float*)(pDesc + iIndex)->data);
			break;
		case 'D':
			sprintf(buff, "%.3lf", *(double*)(pDesc + iIndex)->data);
			break;
		case 'W':
			sprintf(buff, "%d", *(WORD*)(pDesc + iIndex)->data);
			break;
		case 'C':
//			strncpy(buff, (char*)(pDesc + iIndex)->data, RECIPE_COMMENT_LEN);
			strncpy(buff, (char*)(pDesc + iIndex)->data, sizeof(buff));
			break;
		default:
			break;
		}

		// write NanoSpec ini file
		::WritePrivateProfileString(section,
									(pDesc + iIndex)->key,
									buff,
									/*p*/szFilePath);
	}

	return;
}



/////////////////////////////////////////////////////////////////////////////
// Name			: SaveMeasConAdd
// Purpose		: �ǋL���K�v�ȏꍇ�g�p(��ɑ���f�[�^�p)
// Parameters	: pDesc				--->
//				  iDescCount		---> �������݃L�[��
//				  pszMainRcpName	---> �ǋL�Ώۃt�@�C����(�p�X)
void CMeasurementTabView::SaveMeasConAdd(LPCTSTR pszMainRcpName)
{

//
	BOOL bCheck = FALSE;

	char szUserId[MAX_PATH + 1];									//���[�UID
	SYSTEMTIME syStartDateTime;										//����J�n����
	SYSTEMTIME syEndDateTime;										//����I������
	char szSampleId[SAMPLE_ID_LEN + 1];								//	�T���v��ID
	//2009.10.13 bagus RS --{--
	char szLotId[LOT_ID_LEN + 1];									//	���b�gID
	//2009.10.13 bagus RS --}--
	char szUnit[DISP_UNIT_NAME_LEN + 1];							//	���茋�ʒP�ʖ�
	int iSetPointCount;												//�i�[�σ|�C���g��
	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1] = {0};			//���x��(X,Y,Z) �c���g�p(�֐��̈����ɕK�v�Ȃ̂ł��邾��)
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];
	double dData[ADAPRESULT_COLS_MAX];								//Col�f�[�^(AF�������̃f�[�^)
	char szFilePath[_MAX_PATH];
	RCP_DATA rcp_data;

// 2009.10.23 bagus Stress �C�� --{--
	int	nColumnLen;
	char szTmp[255+1];
	char szTmp2[255+1];
	int	nKeyLen;
// 2009.10.23 bagus Stress �C�� --}--

/* added 2017.04.15 hmenjo �����\���� ---------- { ---------- */
	int l_iUnitIndexThickInt = -1;		/* -1 �͐����\�������Ȃ�	*/
	SR_FIGURE_FORMAT l_SrFigureFormat;
	ConfigFile_GetNanoSpecIni(&l_SrFigureFormat,
					CONFIG_FILE_SR_FIGURE_FORMAT);
	if (0 != l_SrFigureFormat.iThicknessIntOnly) {
		l_iUnitIndexThickInt = 3;		/* -1 �ȊO�̓C���f�b�N�X�l	*/
	}
/* added 2017.04.15 hmenjo �����\���� ---------- } ---------- */

//������
	memset(szUserId, 0, sizeof(szUserId));
	memset(&syStartDateTime, 0, sizeof(syStartDateTime));
	memset(&syEndDateTime, 0 , sizeof(syEndDateTime));
	memset(szSampleId, 0, sizeof(szSampleId));
	//2009.10.13 bagus RS --{--
	//���b�gID�̒ǉ�
	memset(szLotId, 0, sizeof(szLotId));
	//2009.10.13 bagus RS --}--
	memset(szUnit, 0, sizeof(szUnit));
	iSetPointCount = 0;
	memset(szDispLabel, 0, sizeof(szDispLabel));
	memset(dData ,0 ,sizeof(dData));
	memset(szFilePath, 0, sizeof(szFilePath));


//�f�[�^���W
	m_pDoc->GetUserId(szUserId);	//���[�UID�̎擾
	m_pDoc->GetStartDateTime(&syStartDateTime);	//	����J�n�����̎擾
	m_pDoc->GetEndDateTime(&syEndDateTime);	//	����I�������̎擾
	m_pDoc->GetSampleId(szSampleId);	//	�T���v��ID�̎擾
	//2009.10.13 bagus RS --{--
	//���b�gID�̒ǉ�
	m_pDoc->GetLotId(szLotId);		//	���b�gID�̎擾
	//2009.10.13 bagus RS --}--
	m_pDoc->GetUnitName(szUnit);	//	���茋�ʒP�ʖ��̎擾
	iSetPointCount = m_pDoc->GetStoreCount();	//�i�[�ό����̎擾
	m_pDoc->GetDispLabel(szDispLabel);
	sprintf(szFilePath, "%s%s%s", g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);

	//2009.09.01 bagus stress --{--
	m_pDoc->GetRcpData(&rcp_data);
	//2009.09.01 bagus stress --}--

	//2009.10.28 bagus 2point-distance --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		iSetPointCount /= 2;
	}
	//2009.10.28 bagus 2point-distance --}--
//�t�@�C���ɒǋL

	char szSection[16 + 1];
	char szStartDate[255 + 1];
	char szStartTime[255 + 1];
	char szEndDate[255 + 1];
	char szEndTime[255 + 1];
	//2009.09.01 bagus stress --{--
	//���ڂ�������̂Ńo�b�t�@�𑝂₷
	//char szFullLabel[1024+1];
	char szFullLabel[2048+1];
	//2009.09.01 bagus stress --}--
	char szPointNum[255 + 1]; //�|�C���g��
	char szPointKey[255 + 1]; //�|�C���g���L�[��
	char szPointData[1024+1];
	char szTempFormat[32+1];
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
	char szTempFormatThickSD[32+1];
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
	ONE_POINT_DATA OnePointData;
	int iWritePoint = 0;
	//2009.09.01 bagus stress --{--
	int iLSScansNum = 0;
	char szStressUnit[32+1];
	//2009.09.01 bagus stress --}--
	int i,j;

	memset(szSection, 0, sizeof(szSection));
	memset(szStartDate, 0 ,sizeof(szStartDate));
	memset(szStartTime, 0, sizeof(szStartTime));
	memset(szEndDate, 0, sizeof(szEndDate));
	memset(szEndTime, 0, sizeof(szEndTime));
	memset(szFullLabel, 0, sizeof(szFullLabel));
	memset(szPointNum, 0, sizeof(szPointNum));
	memset(szPointKey, 0, sizeof(szPointKey));
	memset(szPointData, 0, sizeof(szPointData));
	memset(szTempFormat, 0, sizeof(szTempFormat));
	memset(&OnePointData, 0, sizeof(OnePointData));



//����J�n(�I��)����
	sprintf(szStartDate, "%04d/%02d/%02d", syStartDateTime.wYear,syStartDateTime.wMonth,syStartDateTime.wDay); //����J�n���t
	sprintf(szStartTime, "%02d:%02d:%02d.%03d", syStartDateTime.wHour,syStartDateTime.wMinute,syStartDateTime.wSecond,syStartDateTime.wMilliseconds); //����J�n����
	sprintf(szEndDate, "%04d/%02d/%02d", syEndDateTime.wYear,syEndDateTime.wMonth,syEndDateTime.wDay); //����I�����t
	sprintf(szEndTime, "%02d:%02d:%02d.%03d", syEndDateTime.wHour,syEndDateTime.wMinute,syEndDateTime.wSecond,syEndDateTime.wMilliseconds); //����I������

	iWritePoint = 0;
	iWritePoint += sprintf(szFullLabel + iWritePoint, "%s",  "    "); ////�u�����N*4
//2009.09.01 bagus stress --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		//SR����
	//�w�b�_1(X,Y,Z��)
		for(i=1; i<4+4; i++)
		{
			char szXyz[255+1];
			memset(szXyz, 0, sizeof(szXyz));

//2009.10.20 bagus Stress �C�� --{--
#if 0
			if(i == 4)
				sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "MPa"); 		//�X�g���X��MPa
			else
				sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
#else
			if (i == 4) {
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "[MPa]"); 		//�X�g���X��MPa
				sprintf(szTmp, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], "[MPa]"); 		//�X�g���X��MPa
				// 2009.10.23 bagus Stress �C�� --}--
			} else if (i > 4) {
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				sprintf(szTmp, "%s%s", MEAS_LIST_STRESS_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				// 2009.10.23 bagus Stress �C�� --}--
			} else {
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szXyz, "     %s", MEAS_LIST_STRESS_POSITION_ITEM[i]); //g_lpszXyzUnit[0]��mm
				sprintf(szTmp, "%s", MEAS_LIST_STRESS_POSITION_ITEM[i]); //g_lpszXyzUnit[0]��mm
				// 2009.10.23 bagus Stress �C�� --}--
			}
			// 2009.10.23 bagus Stress �C�� --{--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp);
			sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
			// 2009.10.23 bagus Stress �C�� --}--
#endif
//2009.10.20 bagus Stress �C�� --}--
			if(i==1)
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, "\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
			else
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
		}
	//�w�b�_2(AF���ǋL)
		// 2009.10.23 bagus Stress �C�� --{--
		//iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",	MEAS_LIST_AF_CHECK_ITEM[0]);
		nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress �C�� --}--
	}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
		for(i=0; i<4; i++)
		{
			char szXyz[255+1];
			memset(szXyz, 0, sizeof(szXyz));
			sprintf(szTmp, "%s%s", MEAS_LIST_POSITION_ITEM[i % 2 + 1], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
			nColumnLen = COLUMN_LENGTH - strlen(szTmp);
			sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
//2009.12.10 bagus �C�� --{--
//			if(i==1)
			if(i==0)
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, "\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
			else
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szXyz); ////X,Y,Z
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
//2009.12.10 bagus �C�� --{--
		}

	// 2009.10.23 bagus Stress �C�� --}--
	//2009.10.28 bagus 2point-Distance --{--
	}else{
		//SR�Ȃ�
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Add (Stage None�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Add (Stage None�Ή�) <--
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
			//�w�b�_1(X,Y,Z��)
			for(i=1; i<4; i++)
			{
				char szXyz[255+1];
				memset(szXyz, 0, sizeof(szXyz));
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szXyz, "     %s%s", MEAS_LIST_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				sprintf(szTmp, "%s%s", MEAS_LIST_POSITION_ITEM[i], g_lpszXyzUnit[0]); //g_lpszXyzUnit[0]��mm
				nColumnLen = COLUMN_LENGTH - strlen(szTmp);
				sprintf(szXyz, "%*s%s", nColumnLen, " ", szTmp);
				// 2009.10.23 bagus Stress �C�� --}--
				if(i==1)
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szFullLabel + iWritePoint, "\"%s\"",  szXyz); ////X,Y,Z
					iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
				else
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szXyz); ////X,Y,Z
					iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szXyz); ////X,Y,Z
// 2009.12.15 bagus NANOMAP Mod <--
			}
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
		}
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
//�w�b�_2(AF���ǋL)
// 2009.12.17 bagus CA --{--
#if 0
		// 2009.10.23 bagus Stress �C�� --{--
		//iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",	MEAS_LIST_AF_CHECK_ITEM[0]);
		nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress �C�� --}--

#else
		if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP) {
			// RS�͏o�͂��Ȃ�
		} else if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_CTA) {
			// CA�͏o�͂��Ȃ�
		} else {
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
//			if(g_lModelType != MODEL_T3100){
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
				nColumnLen = COLUMN_LENGTH - strlen(MEAS_LIST_AF_CHECK_ITEM[0]);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", MEAS_LIST_AF_CHECK_ITEM[0]);
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
			}
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
		}
#endif
// 2009.12.17 bagus CA --}--
	}
//2009.09.01 bagus stress --}--
//�w�b�_3(�X�g���e�W���ǋL)

	//2009.11.17 bagus RS --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
		nColumnLen = COLUMN_LENGTH - strlen(_T("DATA"));
		sprintf(szTmp, "%*s%s", nColumnLen, " ", _T("DATA"));
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
		iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
	}
	//2009.11.17 bagus RS --}--

	for(i=0; szDispLabel[i][0] != '\0'; i++)
	{
// 2009.10.23 bagus Stress �C�� --{--
#if 0
		if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 10.10s\"",	szDispLabel[i]); //
		}
		else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K�l�n
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 8.8s\"",  szDispLabel[i]); //
		}
		else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%�n, Fit
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 8.8s\"",  szDispLabel[i]); //
		}
		else //���̑�
		{
			iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"% 9.9s\"",  szDispLabel[i]); //
		}
#else
		nColumnLen = COLUMN_LENGTH - strlen(szDispLabel[i]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szDispLabel[i]);
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
//		if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
		if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
	// 2009.12.15 bagus NANOMAP Mod -->
	//		iWritePoint += sprintf(szFullLabel + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
	// 2009.12.15 bagus NANOMAP Mod <--
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
		}
		else{
			if(i == 0){
				iWritePoint += sprintf(szFullLabel + iWritePoint, " \"%s\"",  szTmp);
			}
			else{
				iWritePoint += sprintf(szFullLabel + iWritePoint, ", \"%s\"",  szTmp);
			}
		}
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
#endif
// 2009.10.23 bagus Stress �C�� --}--
	}

	//DAT�t�@�C���������ݏ���
	int iDescCount;
	RECIPE_FILE* pDesc = NULL;
	BOOL bSaveJudge = 0;

	//2009.09.01 bagus stress --{--
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		iLSScansNum = m_pDoc->GetMeasuredStressLSNum();
		strcpy(szStressUnit,"MPa");
	}else{
		iLSScansNum = 0;
		strcpy(szStressUnit,"");
	}
	//2009.09.01 bagus stress --}--

//=============================================================================

///MeasuredData�����o��1(�|�C���g���f�[�^������)
	const RECIPE_FILE SrMeasuredDataDesc[] =
	{
		//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
		{"MeasuredData",	"UserID",		'C',		"0",		szUserId			},
		{"",				"StartDate",	'C',		"0",		szStartDate 		},
		{"",				"StartTime",	'C',		"0",		szStartTime 		},
		{"",				"EndDate",		'C',		"0",		szEndDate			},
		{"",				"EndTime",		'C',		"0",		szEndTime			},
		{"",				"SampleID", 	'C',		"0",		szSampleId			},
//2009.10.13 bagus RS --{--
//���b�gID�̒ǉ�
		{"",				"LotID", 		'C',		"0",		szLotId			  	},
//2009.10.13 bagus RS --}--
		{"",				"Unit", 		'C',		"0",		szUnit				},
		{"",				"MeasuredNum",	'I',		"0",		&iSetPointCount 	},
//2009.09.01 bagus stress --{--
//�X�g���X�p�̍��ڂ̒ǉ�
		{"",				"MeasuredLSScanNum",	'I',  "0",		&iLSScansNum 		},
		{"",				"StressUnit",			'C',  "",		szStressUnit 		},
//2009.09.01 bagus stress --}--
		{"",				"Header",		'C',		"0",		szFullLabel 		},
	};
	//SrMeasuredDataDesc[]�̏�������
	iDescCount = sizeof(SrMeasuredDataDesc) / sizeof(SrMeasuredDataDesc[0]);
	pDesc = new RECIPE_FILE [iDescCount];
	memcpy(pDesc, SrMeasuredDataDesc, sizeof(SrMeasuredDataDesc));

	SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

	if ( pDesc ) {
		delete [] pDesc;
		pDesc = NULL;
	}

//=============================================================================


///MeasuredData�����o��2(�|�C���g���f�[�^�̂�)
	for(i=0; i<iSetPointCount; i++)
	{
		ONE_POINT_DATA OnePointData2;
		double dDistance;
	//2009.10.28 bagus 2point-Distance --{--
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE){
			m_pDoc->GetOnePointData(&OnePointData, 2 * i+1);		//�n�_
			m_pDoc->GetOnePointData(&OnePointData2, 2 * i+2);		//�I�_
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			nKeyLen = strlen(szPointKey);
			iWritePoint = 0;
			//Start X
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Start Y
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Start X
			sprintf(szTmp2, "%+.3lf",  OnePointData2.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Start Y
			sprintf(szTmp2, "%+.3lf",  OnePointData2.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//Distance
			dDistance = m_pMainFrame->Calc2PointDistance(OnePointData.xyz.lX,OnePointData.xyz.lY,OnePointData2.xyz.lX,OnePointData2.xyz.lY);
			sprintf(szTmp2, "%+.3lf",  dDistance * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//2009.11.26 bagus 2point-distance --{--
			//dx,dy���ڂ̒ǉ�
			//dx
			//2009.11.29 bagus 2point-distance --{--
			//sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lX - OnePointData2.xyz.lX) * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf",  fabs((OnePointData.xyz.lX - OnePointData2.xyz.lX) * g_dXyzUnitConvertCoefficient[0])); //X
			//2009.11.29 bagus 2point-distance --}--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//dy
			//2009.11.29 bagus 2point-distance --{--
			//sprintf(szTmp2, "%+.3lf", (OnePointData.xyz.lY - OnePointData2.xyz.lY) * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf", fabs((OnePointData.xyz.lY - OnePointData2.xyz.lY) * g_dXyzUnitConvertCoefficient[0])); //X
			//2009.11.29 bagus 2point-distance --}--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			//2009.11.26 bagus 2point-distance --}--


	//2009.10.28 bagus 2point-Distance --}--
	//2009.11.17 bagus RS --{--
		}else if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_4PP){
			m_pDoc->GetOnePointData(&OnePointData, i+1);
			m_pDoc->GetOnepointDdata(i+1, dData);
			//�L�[
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			// 2009.10.23 bagus Stress �C�� --{--
			nKeyLen = strlen(szPointKey);
			// 2009.10.23 bagus Stress �C�� --}--
			iWritePoint = 0;

			//X,Y,Z��
			// 2009.10.23 bagus Stress �C�� --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, "\"% +10.3lf\"",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress �C�� --}--

			// 2009.10.23 bagus Stress �C�� --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
			sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress �C�� --}--

			// 2009.10.23 bagus Stress �C�� --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10 ); //Z
			sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10); //Z
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress �C�� --}--

// 2009.12.17 bagus RS --{--
#if 0
			//AF���ǋL
			// 2009.10.23 bagus Stress �C�� --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",	OnePointData.szShowAfValid); //AF
			sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress �C�� --}--
#endif
// 2009.12.17 bagus RS --}--

			{
				int nRSDataCol;
				RESISTRESULT ResistOnePointData;
				BOOL bResitValidData = FALSE;
				CString strBuffer;
				m_pDoc->GetOneResistData(&ResistOnePointData,i+1);

				switch(ResistOnePointData.chDataValid){
				case _T('N'):
					bResitValidData = TRUE;
					strBuffer.Format(_T("%s"),_T("VALID"));
					break;
				case _T('O'):
					bResitValidData = FALSE;
					strBuffer.Format(_T("%s"),_T("OV.RG"));
					break;
				case _T('L'):
					bResitValidData = FALSE;
					strBuffer.Format(_T("%s"),_T("OV.LD"));
					break;
				default:
					bResitValidData = FALSE;
					strBuffer.Format(_T("%s"),_T("UNKNOWN"));
					break;
				}
				nColumnLen = COLUMN_LENGTH - strBuffer.GetLength();
				sprintf(szTmp, "%*s%s", nColumnLen, " ", strBuffer.GetBuffer(0));
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--

				for(nRSDataCol = 0;nRSDataCol < 4;nRSDataCol++){
					//MeasurementData
					switch(nRSDataCol){
					//2009.11.17 RS --{--
					case 0:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szResistance : _T("")); 		break;
					case 1:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szSurfaceResistivity : _T(""));	break;
					case 2:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szVolumeResistivity : _T(""));	break;
					case 3:	strBuffer.Format("%s",bResitValidData ? ResistOnePointData.szConductivity : _T(""));		break;
					//2009.11.17 RS --}--
					}
					nColumnLen = COLUMN_LENGTH - strBuffer.GetLength();
					sprintf(szTmp, "%*s%s", nColumnLen, " ", strBuffer.GetBuffer(0));
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				}
			}


	//2009.11.17 bagus RS --}--
		}else{
			m_pDoc->GetOnePointData(&OnePointData, i+1);
			m_pDoc->GetOnepointDdata(i+1, dData);
		//�L�[
			ZeroBungUp(szPointNum, i+1);
			sprintf(szPointKey, "%s%s", "Point", szPointNum);
			// 2009.10.23 bagus Stress �C�� --{--
			nKeyLen = strlen(szPointKey);
			// 2009.10.23 bagus Stress �C�� --}--
			iWritePoint = 0;
		//2009.09.01 bagus stress --{--
		//�X�g���X�p�ɍ��ڂ𖄂߂�
			if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
				for(int nSpacer = 0;nSpacer < 4;nSpacer++){
// 2009.10.22 bagus Stress �C�� --{--
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"        --\"");	//
					if (nSpacer == 0) {
						// 2009.10.23 bagus Stress �C�� --{--
						//iWritePoint += sprintf(szPointData + iWritePoint, "\"       ---\"");	//
						sprintf(szTmp2, "---");
						nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
						sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//						iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
						iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
						// 2009.10.23 bagus Stress �C�� --}--
					} else {
						// 2009.10.23 bagus Stress �C�� --{--
						//iWritePoint += sprintf(szPointData + iWritePoint, ",\"       ---\"");	//
						sprintf(szTmp2, "---");
						nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
						sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//						iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
						iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
						// 2009.10.23 bagus Stress �C�� --}--
					}
// 2009.10.22 bagus Stress �C�� --}--
				}
				iWritePoint += sprintf(szPointData + iWritePoint, ",");	//
			}
	//2009.09.01 bagus stress --}--

// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
//			if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
			if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
				//X,Y,Z��
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, "\"% +10.3lf\"",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
				sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lX * g_dXyzUnitConvertCoefficient[0]); //X
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--

				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
				sprintf(szTmp2, "%+.3lf",  OnePointData.xyz.lY * g_dXyzUnitConvertCoefficient[0]); //Y
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--

				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"% +10.3lf\"",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10 ); //Z
				sprintf(szTmp2, "%+.3lf",  (OnePointData.xyz.lZ * g_dXyzUnitConvertCoefficient[0])/10); //Z
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--

				//AF���ǋL
// 2009.12.17 bagus CA --{--
#if 0
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",	OnePointData.szShowAfValid); //AF
				sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--
#else
				if (rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType != HEAD_TYPE_CTA) {
					sprintf(szTmp2, "%s",  OnePointData.szShowAfValid); //AF
					nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
					sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				}
#endif
// 2009.12.17 bagus CA --}--
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
			}
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--

			int iUnitIndex = 0;
			int iUnit = m_pDoc->GetUnitIndex();
			if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
			else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
			if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
				iUnitIndex = 0;
			}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */

			//�X�g���e�W��
			for(j=0; szDispLabel[j][0] != '\0'; j++)
			{
				char szDecimalPointBuff[255+1];
				memset(szDecimalPointBuff, 0, sizeof(szDecimalPointBuff));
				if(strstr(szDispLabel[j], "Thick") != NULL) //Thick*
				{
/* modified 2017.04.15 hmenjo �����\���� ---------- { ---------- */
//					// 2009.10.23 bagus Stress �C�� --{--
//					//sprintf(szTempFormat, ",%s 10%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //�I��Unit�ŕϓ�
//					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
//					// 2009.10.23 bagus Stress �C�� --}--
/* modified 2017.04.15 hmenjo �����\���� ----------              */
					if (l_iUnitIndexThickInt < 0) {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
					} else {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "lf");
					}
/* modified 2017.04.15 hmenjo �����\���� ---------- } ---------- */
				}
				else if((szDispLabel[j][0]=='N')||(szDispLabel[j][0]=='K')) //N,K�l�n
				{
					// 2009.10.23 bagus Stress �C�� --{--
					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[4], "lf\""); //[.####]�Œ�
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[4], "lf"); //[.####]�Œ�
					// 2009.10.23 bagus Stress �C�� --}--
				}
				else if(strstr(szDispLabel[j], "%") != NULL) //%�n
				{
					// 2009.10.23 bagus Stress �C�� --{--
					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]�Œ�
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]�Œ�
					// 2009.10.23 bagus Stress �C�� --}--
				}
				else if(strstr(szDispLabel[j], "Fit") != NULL) //Fit
				{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//					// 2009.10.23 bagus Stress �C�� --{--
//					//sprintf(szTempFormat, ",%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]�Œ�
//					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]�Œ�
//					// 2009.10.23 bagus Stress �C�� --}--
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
					if (1 == g_lXmpType) {
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- { ---------- */
////						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf"); //[.##]�Œ�
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ----------              */
//						strcpy(szTempFormat, "%.3e");
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
#if FIT_DISP_SCOUT == 3
						strcpy(szTempFormat, "");
#elif FIT_DISP_SCOUT == 2
						strcpy(szTempFormat, "%.2e");
#elif FIT_DISP_SCOUT == 1
						strcpy(szTempFormat, "%.3e");
#else
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf"); //[.##]�Œ�
#endif
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
					} else {
						sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf"); //[.##]�Œ�
					}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
				}
				else //���̑�
				{
					// 2009.10.23 bagus Stress �C�� --{--
					//sprintf(szTempFormat, ",%s 9%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //�I��Unit�ŕϓ�
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
					// 2009.10.23 bagus Stress �C�� --}--
				}

				if(strstr(szDispLabel[j], "Thick") != NULL) //���x������Thick*
				{
					// 2009.10.23 bagus Stress �C�� --{--
					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j] * g_dUnitConvert[iUnitIndex]); //
					sprintf(szTmp2, szTempFormat,  dData[j] * g_dUnitConvert[iUnitIndex]);
					// 2009.10.23 bagus Stress �C�� --}--
				}
				else if(strstr(szDispLabel[j], "%") != NULL) //���x������%�n
				{
					// 2009.10.23 bagus Stress �C�� --{--
					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j] * 100); //
					sprintf(szTmp2, szTempFormat,  dData[j] * 100);
					// 2009.10.23 bagus Stress �C�� --}--
				}
				else //
				{
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
//					// 2009.10.23 bagus Stress �C�� --{--
//					//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dData[j]); //
//					sprintf(szTmp2, szTempFormat,  dData[j]);
//					// 2009.10.23 bagus Stress �C�� --}--
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
					if (0 == strlen(szTempFormat)) {
						long l_lExp = m_pDoc->GetExpDouble(dData[j]);
						sprintf(szTmp2, "%.2fe%+03d",  dData[j] / pow(10, l_lExp), l_lExp);
					} else {
						sprintf(szTmp2, szTempFormat,  dData[j]);
					}
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
				}

				// 2009.10.23 bagus Stress �C�� --{--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2014.01.17 Bagus Mod (Stage None�Ή�) -->
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
//				if(g_lModelType != MODEL_T3100){
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
				if(m_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2014.01.17 Bagus Mod (Stage None�Ή�) <--
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
// 2013.12.19 Bagus Add (TohoSpec�Ή�) -->
				}
				else{
					if(j == 0){
						iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
					}
					else{
						iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
					}
				}
// 2013.12.19 Bagus Add (TohoSpec�Ή�) <--
				// 2009.10.23 bagus Stress �C�� --}--
			}
		//2009.10.28 bagus 2point-Distance --{--
		}
		//2009.10.28 bagus 2point-Distance --}--

		const RECIPE_FILE SrPointDataDesc[] =
		{
			//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
			{"MeasuredData",	szPointKey, 	'C',		"0",		szPointData 	},
		};

		//SrPointDataDesc[]�̏�������
		iDescCount = sizeof(SrPointDataDesc) / sizeof(SrPointDataDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrPointDataDesc, sizeof(SrPointDataDesc));

		SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

		if ( pDesc ) {
			delete [] pDesc;
			pDesc = NULL;
		}

	}

	//2009.09.01 bagus stress --{--
	//�X�g���X�̏ꍇ��LS�̃f�[�^���o�͂���
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		char szLS[16+1];
		STAGE_PROG_STRESS& stressStage = rcp_data.StressStageProgInfo;

		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			if(!m_pDoc->GetOneStressLineDataValid(nLine+1)){
				continue;
			}

			for(int nSection = 0;nSection < STRESS_SECTIONS_MAX;nSection++){
				memset(szLS,NULL,sizeof(szLS));
				memset(szPointData,NULL,sizeof(szPointData));
				iWritePoint = 0;
				//�L���łȂ��Z�N�V�����͂Ȃɂ����Ȃ�
				if(!stressStage.Line[nLine].bScanValid[nSection]){
					continue;
				}
			//LmSn
				sprintf(szLS,"L%dS%d",nLine+1,nSection+1);
			//X-Start
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%ld\"",stressStage.Line[nLine].SectPos[nSection].lScanStartPosX);
				nKeyLen = 10 - strlen(szLS);
				if (nKeyLen < 0)
					nKeyLen = 0;
				//2009.12.24 bagus stress --{--
				//sprintf(szTmp2, "%ld", stressStage.Line[nLine].SectPos[nSection].lScanStartPosX);
				sprintf(szTmp2, "%.4f", stressStage.Line[nLine].SectPos[nSection].lScanStartPosX / (double)MICROMETRE);
				//2009.12.24 bagus stress --}--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, "%*s\"%s\"", nKeyLen, " ", szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, "%*s\"%s\"", nKeyLen+1, " ", szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--
			//X-End
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%ld\"",stressStage.Line[nLine].SectPos[nSection].lScanEndPosX);
				//2009.12.24 bagus stress --{--
				//sprintf(szTmp2, "%ld", stressStage.Line[nLine].SectPos[nSection].lScanEndPosX);
				sprintf(szTmp2, "%.4f", stressStage.Line[nLine].SectPos[nSection].lScanEndPosX / (double)MICROMETRE);
				//2009.12.24 bagus stress --}--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--
			//Y
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, ",\"%ld\"",stressStage.Line[nLine].SectPos[nSection].lScanPosY);
				//2009.12.24 bagus stress --{--
				//sprintf(szTmp2, "%ld", stressStage.Line[nLine].SectPos[nSection].lScanPosY);
				sprintf(szTmp2, "%.4f", stressStage.Line[nLine].SectPos[nSection].lScanPosY / (double)MICROMETRE);
				//2009.12.24 bagus stress --}--
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--
			//Stress
				STRESSRESULT tStressResult;
				m_pDoc->GetOneStressLineData(&tStressResult,nLine+1);
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint,",\"%.3lf\"",tStressResult.dStressSection[nSection]);
				sprintf(szTmp2, "%.3lf", tStressResult.dStressSection[nSection]);
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--
				//�X�g���X���ڂɖ��֌W�ȍ��ڂ𖄂߂�
// 2009.10.23 bagus Stress �C�� --{--
#if 0
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint,",\"      ---\"");
				// 2009.10.23 bagus Stress �C�� --}--
#else
				sprintf(szTmp2, "---");
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
	#if 0
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
// 2009.12.14 bagus Stress �C�� --{--
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
// 2009.12.14 bagus Stress �C�� --}--
	#else
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
				iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
	#endif
// 2009.12.15 bagus NANOMAP Mod <--
#endif
// 2009.10.23 bagus Stress �C�� --}--
				//
				const RECIPE_FILE StressLSDataDesc[] =
				{
					//Section Name		Key Name		Data Type	Defaule 	Variable Pointer
					{"MeasuredData",	szLS,	  'C',		  "0",		  szPointData	  },
				};

				iDescCount = sizeof(StressLSDataDesc) / sizeof(StressLSDataDesc[0]);
				pDesc = new RECIPE_FILE [iDescCount];
				memcpy(pDesc, StressLSDataDesc, sizeof(StressLSDataDesc));

				SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

				if ( pDesc ) {
					delete [] pDesc;
					pDesc = NULL;
				}
			}

		}
	}
	//2009.09.01 bagus stress --}--


	//���s(Point*****�̍��ڌ��1�s���s��ǉ�)
	FILE *fp;
	if ((fp = fopen(szFilePath, "a")) == NULL)
	{
		//�t�@�C���I�[�v�������s�����ꍇ�A���s����ꂸ���s
	}
	else
	{
		putc('\n', fp);
		//2009.08.28 bagus stress --{--
		fclose(fp);
		//2009.08.28 bagus stress --}--
	}
	//2009.08.28 bagus stress --{--
	//���s�����̂ł����fclose���Ȃ�
	//fclose(fp);
	//2009.08.28 bagus stress --}--


//========================================
///MeasuredData�����o��3(���v�f�[�^�̂�)


	char szStrategyLabel[1024+1];
	char szValidMesuredNum[1024+1];
	char szHeader[1024+1];
	char szMax[1024+1];
	char szMin[1024+1];
	char szRange[1024+1];
	char szMean[1024+1];
	char szSd[1024+1];
	memset(szStrategyLabel, 0, sizeof(szStrategyLabel));
	memset(szValidMesuredNum, 0, sizeof(szValidMesuredNum));
	memset(szHeader, 0, sizeof(szHeader));
	memset(szMax, 0, sizeof(szMax));
	memset(szMin, 0, sizeof(szMin));
	memset(szRange, 0, sizeof(szRange));
	memset(szMean, 0, sizeof(szMean));
	memset(szSd, 0, sizeof(szSd));

// 2009.10.23 bagus Stress �C�� --{--
	BOOL bWriteHead = FALSE;
// 2009.10.23 bagus Stress �C�� --}--

	//���v�f�[�^�L��Point��, �X�g���e�W�B�w�b�_
	iWritePoint = 0;
// 2009.10.23 bagus Stress �C�� --{--
	nKeyLen = 10 - strlen("Header");
	if (nKeyLen < 0) {
		nKeyLen = 0;
	} else {
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "%*s", nKeyLen, " ");
	}
// 2009.10.23 bagus Stress �C�� --}--
	//2009.09.01 bagus stress --{--
	//�X�g���X�̓��v���p�w�b�h
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		// 2009.10.23 bagus Stress �C�� --{--
		//iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"Stress[MPa]\"");
		sprintf(szTmp2, "%s", "Stress[MPa]");
		nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"%s\"",  szTmp);
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
		// 2009.10.23 bagus Stress �C�� --}--
		for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
			STGPGM_STRESS_LINE& LineInfo = rcp_data.StressStageProgInfo.Line[nLine];
			if(LineInfo.bValidLine){
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szStrategyLabel + iWritePoint,",\"Stress L%d\"",nLine+1);
				sprintf(szTmp2, "Stress L%d", nLine+1);
				nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
				sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//				iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",\"%s\"",  szTmp);
				iWritePoint += sprintf(szStrategyLabel + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
				// 2009.10.23 bagus Stress �C�� --}--
			}
		}
// 2009.10.23 bagus Stress �C�� --{--
		bWriteHead = TRUE;
// 2009.10.23 bagus Stress �C�� --}--
	}
	//2009.09.01 bagus stress --}--
	for(i=0; szDispLabel[i][0] != '\0'; i++)
	{
// 2009.10.23 bagus Stress �C�� --{--
#if 0
		//2009.09.01 bagus stress --{--
		//if(i != 0)
		if(iWritePoint != 0)
		//2009.09.01 bagus stress --}--
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",");
		}
#else
		if (bWriteHead) {
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, ",");
		} else {
			//
		}
#endif
// 2009.10.23 bagus Stress �C�� --}--
// 2009.10.23 bagus Stress �C�� --{--
#if 0
		if(strstr(szDispLabel[i], "Thick") != NULL) //Thick*
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 10.10s\"",  szDispLabel[i]); //
		}
		else if((szDispLabel[i][0]=='N')||(szDispLabel[i][0]=='K')) //N,K�l�n
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 8.8s\"",  szDispLabel[i]); //
		}
		else if((strstr(szDispLabel[i], "%") != NULL)||(strstr(szDispLabel[i], "Fit") != NULL)) //%�n, Fit
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 8.8s\"",  szDispLabel[i]); //
		}
		else //���̑�
		{
			iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"% 9.9s\"",  szDispLabel[i]); //
		}
#else
		nColumnLen = COLUMN_LENGTH - strlen(szDispLabel[i]);
		sprintf(szTmp, "%*s%s", nColumnLen, " ", szDispLabel[i]);
// 2009.12.15 bagus NANOMAP Mod -->
//		iWritePoint += sprintf(szStrategyLabel + iWritePoint, "\"%s\"",  szTmp);
		iWritePoint += sprintf(szStrategyLabel + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
#endif
// 2009.10.23 bagus Stress �C�� --}--
// 2009.10.23 bagus Stress �C�� --{--
		bWriteHead = TRUE;
// 2009.10.23 bagus Stress �C�� --}--
	}

	int iStatisticsValidPointCount = m_pDoc->GetStatisticsValidPointCount();
	const RECIPE_FILE SrStatisticsHeaderDesc[] = //���v�f�[�^�L�����A�X�g���e�W�B�w�b�_
	{
		//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
		{"STATISTICS",		"ValidMesuredNum",			'I',		"0",		&iStatisticsValidPointCount },
		{"",				"Header",					'C',		"0",		szStrategyLabel 							},
	};

	//SrStatisticsHeaderDesc[]�̏�������
	iDescCount = sizeof(SrStatisticsHeaderDesc) / sizeof(SrStatisticsHeaderDesc[0]);
	pDesc = new RECIPE_FILE [iDescCount];
	memcpy(pDesc, SrStatisticsHeaderDesc, sizeof(SrStatisticsHeaderDesc));

	SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

	if ( pDesc ) {
		delete [] pDesc;
		pDesc = NULL;
	}

	int iUnitIndex = 0;
	int iUnit = m_pDoc->GetUnitIndex();
	if((iUnit >= 0) && (iUnit < FIGURE_UNIT_SR_THICKNESS_MAX)){iUnitIndex = iUnit + 1;}
	else{iUnitIndex = 1;}
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
	if (iUnit == FIGURE_UNIT_SR_THICKNESS_UM) {
		iUnitIndex = 0;
	}
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */
	memset(szTempFormat, 0, sizeof(szTempFormat));
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
	memset(szTempFormatThickSD, 0, sizeof(szTempFormatThickSD));
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */

// 2009.10.23 bagus Stress �C�� --{--
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.23 bagus Stress �C�� --}--

	for(i=0; i<5; i++)
	{
		iWritePoint = 0;
// 2009.10.23 bagus Stress �C�� --{--
		bWriteHead = FALSE;
		if(l_SystemConfig.nLanguage == 0){
			nKeyLen = 10 - strlen(MEAS_LIST_PARAM_ITEM_ENU[i]);
		} else {
			nKeyLen = 10 - strlen(MEAS_LIST_PARAM_ITEM_JPN[i]);
		}

		if (nKeyLen < 0) {
			nKeyLen = 0;
		} else {
			iWritePoint += sprintf(szPointData + iWritePoint, "%*s", nKeyLen, " ");
		}
// 2009.10.23 bagus Stress �C�� --}--
		int iWritePointSpaceBung;
		iWritePointSpaceBung=0;

		//2009.09.01 bagus stress --{--
		//�X�g���X�̏ꍇ�ɂ̓X�g���X�p�̍��ڂ̓��v����ǉ�����
		if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
			STATISTICS l_StressStat;
			m_pDoc->GetStressStatisticsData(&l_StressStat);
			double dStressData;
			if(i==0)		dStressData = l_StressStat.dMaximum;
			else if(i==1)	dStressData = l_StressStat.dMinimum;
			else if(i==2)	dStressData = l_StressStat.dRange;
			else if(i==3)	dStressData = l_StressStat.dAverage;
			else/*if(i==4)*/dStressData = l_StressStat.dStandardDeviation;
			// 2009.10.23 bagus Stress �C�� --{--
			//iWritePoint += sprintf(szPointData + iWritePoint, "\"%.3lf\"",dStressData);
			sprintf(szTmp2, "%.3lf", dStressData);
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress �C�� --}--

			for(int nLine = 0;nLine < STRESS_LINES_MAX;nLine++){
				STGPGM_STRESS_LINE& LineInfo = rcp_data.StressStageProgInfo.Line[nLine];
				if(LineInfo.bValidLine){
					m_pDoc->GetStressLineStatisticsData(nLine+1,&l_StressStat);
					double dStressData;
					if(i==0)		dStressData = l_StressStat.dMaximum;
					else if(i==1)	dStressData = l_StressStat.dMinimum;
					else if(i==2)	dStressData = l_StressStat.dRange;
					else if(i==3)	dStressData = l_StressStat.dAverage;
					else/*if(i==4)*/dStressData = l_StressStat.dStandardDeviation;
					// 2009.10.23 bagus Stress �C�� --{--
					//iWritePoint += sprintf(szPointData + iWritePoint,",\"%.3lf\"",dStressData);
					sprintf(szTmp2, "%.3lf", dStressData);
					nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
					sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//					iWritePoint += sprintf(szPointData + iWritePoint, ",\"%s\"",  szTmp);
					iWritePoint += sprintf(szPointData + iWritePoint, ", \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
					// 2009.10.23 bagus Stress �C�� --}--
				}
			}
// 2009.10.23 bagus Stress �C�� --{--
			bWriteHead = TRUE;
// 2009.10.23 bagus Stress �C�� --}--
		}
		//2009.09.01 bagus stress --}--

		//Max,Min,Range,Mean,S.D.
		//�X�g���e�W��
		for(j=0; szDispLabel[j][0] != '\0'; j++)
		{
			if(strstr(szDispLabel[j], "Thick") != NULL) //Thick*
			{
/* modified 2017.04.15 hmenjo �����\���� ---------- { ---------- */
//				// 2009.10.23 bagus Stress �C�� --{--
//				//sprintf(szTempFormat, "%s 10%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //�I��Unit�ŕϓ�
//				sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
//				// 2009.10.23 bagus Stress �C�� --}--
/* modified 2017.04.15 hmenjo �����\���� ----------              */
				if (l_iUnitIndexThickInt < 0) {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
				} else {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[l_iUnitIndexThickInt], "lf");
				}
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
				sprintf(szTempFormatThickSD, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
/* added 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
/* modified 2017.04.15 hmenjo �����\���� ---------- } ---------- */
			}
			else if((szDispLabel[j][0]=='N')||(szDispLabel[j][0]=='K')) //N,K�l�n
			{
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[4], "lf\""); //[.####]�Œ�
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[4], "lf");
				// 2009.10.23 bagus Stress �C�� --}--
			}
			else if(strstr(szDispLabel[j], "%") != NULL) //%�n
			{
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]�Œ�
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
				// 2009.10.23 bagus Stress �C�� --}--
			}
			else if(strstr(szDispLabel[j], "Fit") != NULL) //Fit
			{
/* modified 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
//				// 2009.10.23 bagus Stress �C�� --{--
//				//sprintf(szTempFormat, "%s 8%s%s", "\"%", g_lpszDecimalPoint[2], "lf\""); //[.##]�Œ�
//				sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
//				// 2009.10.23 bagus Stress �C�� --}--
/* modified 2014.05.22 hmenjo scout fit .6f ----------              */
				if (1 == g_lXmpType) {
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- { ---------- */
////					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf");
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ----------              */
//					strcpy(szTempFormat, "%.3e");
///* modified 2014.10.27 hmenjo Fit �w���\�� %.3e ---------- } ---------- */
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
#if FIT_DISP_SCOUT == 3
					strcpy(szTempFormat, "");
#elif FIT_DISP_SCOUT == 2
					strcpy(szTempFormat, "%.2e");
#elif FIT_DISP_SCOUT == 1
					strcpy(szTempFormat, "%.3e");
#else
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[6], "lf");
#endif
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
				} else {
					sprintf(szTempFormat, "%s%s%s", "%", g_lpszDecimalPoint[2], "lf");
				}
/* modified 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
			}
			else //���̑�
			{
				// 2009.10.23 bagus Stress �C�� --{--
				//sprintf(szTempFormat, "%s 9%s%s", "\"%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf\""); //�I��Unit�ŕϓ�
				sprintf(szTempFormat, "%s%s%s", "%", g_lpszThickUnitDecimalPoint[iUnitIndex], "lf"); //�I��Unit�ŕϓ�
				// 2009.10.23 bagus Stress �C�� --}--
			}

			STATISTICS Statistics[ADAPRESULT_COLS_MAX];
			memset(Statistics, 0, sizeof(Statistics));
			m_pDoc->GetStatisticsData(Statistics);
			double dStatisticsData;
			if(i==0)		dStatisticsData = Statistics[j].dMaximum;
			else if(i==1)	dStatisticsData = Statistics[j].dMinimum;
			else if(i==2)	dStatisticsData = Statistics[j].dRange;
			else if(i==3)	dStatisticsData = Statistics[j].dAverage;
			else/*if(i==4)*/dStatisticsData = Statistics[j].dStandardDeviation;

// 2009.10.23 bagus Stress �C�� --{--
#if 0
			//�l�ߕ�
			char szSpaceBung[16+1];
			memset(szSpaceBung, 0, sizeof(szSpaceBung));
			//2009.09.01 bagus stress --{--
			//if(j == 0)
			if(iWritePoint == 0)
			//2009.09.01 bagus stress --}--
			{
				//Saiki 20090603 Change ----->
				//int iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM[i]); //6��"Header"�̕�����(STATISTICS�̃L�[�̍Œ�����)
				int iSpaceBungCnt;
				SYSTEM_CONFIG l_SystemConfig;
				ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
				if(l_SystemConfig.nLanguage == 0){
						iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM_ENU[i]); //6��"Header"�̕�����(STATISTICS�̃L�[�̍Œ�����)
				}
				else{
					iSpaceBungCnt = 6 - strlen(MEAS_LIST_PARAM_ITEM_JPN[i]); //6��"Header"�̕�����(STATISTICS�̃L�[�̍Œ�����)
				}
				//Saiki 20090603 Change <-----
				for(int k=0; k < iSpaceBungCnt; k++)
				{
					iWritePointSpaceBung += sprintf(szSpaceBung + iWritePointSpaceBung, " ");
				}
				iWritePoint += sprintf(szPointData + iWritePoint, szSpaceBung); //
			}
#endif
// 2009.10.23 bagus Stress �C�� --}--
// 2009.10.23 bagus Stress �C�� --{--
#if 0
			//2009.09.01 bagus stress --{--
			//if(j != 0)
			if(iWritePoint != 0)
			//2009.09.01 bagus stress --}--
			{
				iWritePoint += sprintf(szPointData + iWritePoint, ",");
			}
#else
			if (bWriteHead) {
				iWritePoint += sprintf(szPointData + iWritePoint, ",");
			} else {
				//
			}
#endif
// 2009.10.23 bagus Stress �C�� --}--
			if(strstr(szDispLabel[j], "Thick") != NULL) //���x������Thick*
			{
/* modified 2017.04.29 hmenjo �����\���� SD ���O ---------- { ---------- */
//				// 2009.10.23 bagus Stress �C�� --{--
//				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData/*dData[j]*/ * g_dUnitConvert[iUnitIndex]); //
//				sprintf(szTmp2, szTempFormat,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
//				// 2009.10.23 bagus Stress �C�� --}--
/* modified 2017.04.29 hmenjo �����\���� SD ���O ----------              */
				if (i == 4) {
					/* SD �l	*/
					sprintf(szTmp2, szTempFormatThickSD,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
				} else {
					/* SD �l�ȊO	*/
					sprintf(szTmp2, szTempFormat,  dStatisticsData * g_dUnitConvert[iUnitIndex]);
				}
/* modified 2017.04.29 hmenjo �����\���� SD ���O ---------- } ---------- */
			}
			else if(strstr(szDispLabel[j], "%") != NULL) //���x������%�n
			{
				// 2009.10.23 bagus Stress �C�� --{--
				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData * 100); //
				sprintf(szTmp2, szTempFormat,  dStatisticsData * 100);
				// 2009.10.23 bagus Stress �C�� --}--
			}
			else //
			{
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- { ---------- */
//				// 2009.10.23 bagus Stress �C�� --{--
//				//iWritePoint += sprintf(szPointData + iWritePoint, szTempFormat, dStatisticsData/*dData[j]*/); //
//				sprintf(szTmp2, szTempFormat,  dStatisticsData);
//				// 2009.10.23 bagus Stress �C�� --}--
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ----------              */
				if (0 == strlen(szTempFormat)) {
					long l_lExp = m_pDoc->GetExpDouble(dStatisticsData);
					sprintf(szTmp2, "%.2fe%+03d",  dStatisticsData / pow(10, l_lExp), l_lExp);
				} else {
					sprintf(szTmp2, szTempFormat,  dStatisticsData);
				}
/* modified 2014.11.22 hmenjo Fit �w���\��(2) ---------- } ---------- */
			}

			// 2009.10.23 bagus Stress �C�� --{--
			nColumnLen = COLUMN_LENGTH - strlen(szTmp2);
			sprintf(szTmp, "%*s%s", nColumnLen, " ", szTmp2);
// 2009.12.15 bagus NANOMAP Mod -->
//			iWritePoint += sprintf(szPointData + iWritePoint, "\"%s\"",  szTmp);
			iWritePoint += sprintf(szPointData + iWritePoint, " \"%s\"",  szTmp);
// 2009.12.15 bagus NANOMAP Mod <--
			// 2009.10.23 bagus Stress �C�� --}--

// 2009.10.23 bagus Stress �C�� --{--
			bWriteHead = TRUE;
// 2009.10.23 bagus Stress �C�� --}--
		}

//Saiki 20090603 Change ----->
// 2009.10.23 bagus Stress �C�� --{--
//		SYSTEM_CONFIG l_SystemConfig;
//		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// 2009.10.23 bagus Stress �C�� --}--
		if(l_SystemConfig.nLanguage == 0){

			const RECIPE_FILE SrStatisticsDataDesc_ENU[] =
			{
				//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
				{"STATISTICS",		/*szMeasListParamItem*/MEAS_LIST_PARAM_ITEM_ENU[i], 'C',		"0",		szPointData 	},
			};
			//SrStatisticsDataDesc[]�̏�������
			iDescCount = sizeof(SrStatisticsDataDesc_ENU) / sizeof(SrStatisticsDataDesc_ENU[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrStatisticsDataDesc_ENU, sizeof(SrStatisticsDataDesc_ENU));
		}
		else{
			const RECIPE_FILE SrStatisticsDataDesc_JPN[] =
		{
			//Section Name		Key Name					Data Type	Defaule 	Variable Pointer
				{"STATISTICS",		/*szMeasListParamItem*/MEAS_LIST_PARAM_ITEM_JPN[i], 'C',		"0",		szPointData 	},
		};
		//SrStatisticsDataDesc[]�̏�������
			iDescCount = sizeof(SrStatisticsDataDesc_JPN) / sizeof(SrStatisticsDataDesc_JPN[0]);
		pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrStatisticsDataDesc_JPN, sizeof(SrStatisticsDataDesc_JPN));
		}
//Saiki 20090603 Change <-----

		SaveMeasConRevi2(pDesc, iDescCount, pszMainRcpName); //

		if(pDesc != NULL){
			delete [] pDesc;
			pDesc = NULL;
		}

	}

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
	/* �g�p���ʃf�[�^�o��*/
	if ((0 == this->MRCS_IncludeCountGet()) &&
		(0 == this->MRCS_DeleteCountGet())) {
		/* �w��s�C�폜�ςݍs���Ƃ��ɂO�̏ꍇ�͏]���̎d�l�ł��D	*/
	} else {
		CString l_strPoints = _T("");
		CString l_strTmp;
		BOOL l_bUse = FALSE;
		int l_iprevUse = 0;
		for (i = 1; i <= iSetPointCount; i++) {
			if (0 == (MRCS_SettingStatusGet(i) & 0x4000)) {
				/* Valid = FALSE �łȂ�	*/
				if (0 < this->MRCS_IncludeCountGet()) {
					/* �w��s�݂�	*/
					if (1 == this->MRCS_SettingGet(i)) {
						l_bUse = TRUE;
					}
				} else {
					/* �w��s����	*/
					if (2 != this->MRCS_SettingGet(i)) {
						l_bUse = TRUE;
					}
				}
			}
			if (FALSE != l_bUse) {
				/* �g�p	*/
				l_bUse = FALSE;
				if (0 < l_strPoints.GetLength()) {
					/* �Q��ڈȍ~	*/
					if (l_iprevUse == i - 1) {
						/* �A��	*/
						if (_T('-') != l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
							l_strPoints += _T("-");
						}
						if (i == iSetPointCount) {
							/* �ŏI�f�[�^	*/
							l_strTmp.Format(_T("%d"), iSetPointCount);
							l_strPoints += l_strTmp;
						}
					} else {
						/* ��є�	*/
						if (_T('-') == l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
							l_strTmp.Format(_T("%d,%d"), l_iprevUse, i);
						} else {
							l_strTmp.Format(_T(",%d"), i);
						}
						l_strPoints += l_strTmp;
					}
				} else {
					/* ����	*/
					l_strPoints.Format(_T("%d"), i);
				}
				l_iprevUse = i;
			} else {
				/* ���g�p	*/
				if (i == iSetPointCount) {
					/* �ŏI�f�[�^	*/
					if (_T('-') == l_strPoints.GetAt(l_strPoints.GetLength() - 1)) {
						l_strTmp.Format(_T("%d"), l_iprevUse);
						l_strPoints += l_strTmp;
					}
				}
			}
		}
		TCHAR l_tszFilePath[_MAX_PATH];
		_stprintf(l_tszFilePath, _T("%s%s%s"), g_szData_Meas_Dat_Temp_Copy_Dir, pszMainRcpName, DAT_EXT);
		::WritePrivateProfileString(_T("STATISTICS"), _T("Points"), l_strPoints, l_tszFilePath);
	}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

	return;
}



/////////////////////////////////////////////////////////////////////////////
//���l�̑O��0��⊮���A5���̐����ɂ���B
BOOL CMeasurementTabView::ZeroBungUp(char* czPoint, int iPoint)
{
	char czTemp[5+1];
	memset(czTemp, 0, sizeof(czTemp));

	int i=0;
	int iCnt= 5; //�O��'0'��⊮���鐔(5�ŏ�����)

	if((iPoint <= 99999) && (iPoint > 0)) //5���܂ŉ�
	{
		_itoa(iPoint, czTemp, 10);

		for(i=0; czTemp[i]!='\0'; i++)
		{iCnt--;}

		for(i=0; i<5+1; i++)
		{
			if(i < iCnt){czPoint[i]='0';}
			else		{czPoint[i] = czTemp[i-iCnt];}
		}
		return TRUE;
	}

	czPoint[0] = '\0';
	return FALSE; //�����܂œ��B�����珈�����s

}

/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
TCHAR lg_tszDirSelect[_MAX_PATH * 2];	/* �I������Ă���f�B���N�g�� */
TCHAR lg_tszEditText[_MAX_PATH];	/* ���͒��̃t�@�C�����̈ꎞ�L�� */
TCHAR lg_tszErrMsg[256];	/* �G���[���b�Z�[�W������ */
TCHAR lg_tszErrMsgTitle[256];	/* �G���[���b�Z�[�W�^�C�g�������� */
UINT CALLBACK GetSaveFNHookProc(
		HWND hwndDlg,	/* �R�����_�C�A���O�̃R���g���[���̃n���h�� */
		UINT uiMsg,		/* ���b�Z�[�W */
		WPARAM wParam,	/*	*/
		LPARAM lParam	/*	*/
	)
{
	static int ls_iDirChgPrc = 0;

	switch (uiMsg) {
	case WM_NOTIFY:
		{
			LPNMHDR l_pNMHdr = (LPNMHDR) lParam;
			LPOFNOTIFY l_pOpenFileNameNotify = (LPOFNOTIFY) lParam;
			TCHAR l_tszInvalid[64] = _T("!\"'*+,./:;<=>?@[]^`|~$&()\\");	/* �g�p�֎~���� */
			TCHAR l_tszInvalid2[64] = _T("!\"'*+,./:;<=>?@[]^`|~$&()");	/* �g�p�֎~����'\'�͋����܂� */
			switch (l_pNMHdr->code) {
			case CDN_INITDONE:
				::EnableWindow(::GetDlgItem(l_pNMHdr->hwndFrom, cmb1), FALSE);	/* �ꏊ�R���{���f�B�Z�[�u�� */
				::EnableWindow(::GetDlgItem(l_pNMHdr->hwndFrom, cmb2), FALSE);	/* ��ރR���{���f�B�Z�[�u�� */
				_tcscpy(lg_tszDirSelect, l_pOpenFileNameNotify->lpOFN->lpstrInitialDir);
				/* �f�X�N�g�b�v�̒����ɕ\�����܂��D */
				{
					int l_iLeft = ::GetSystemMetrics(SM_CXFULLSCREEN);
					int l_iTop = ::GetSystemMetrics(SM_CYFULLSCREEN);
					RECT l_rectDlg = {0};
					::GetWindowRect(::GetParent(hwndDlg), &l_rectDlg);
					l_iLeft = (l_iLeft / 2) - ((l_rectDlg.right - l_rectDlg.left) / 2);
					l_iTop = (l_iTop / 2) - ((l_rectDlg.bottom - l_rectDlg.top) / 2);
					::SetWindowPos(::GetParent(hwndDlg), 0, l_iLeft, l_iTop, 0, 0, SWP_NOSIZE);
				}
				break;
			case CDN_FOLDERCHANGE:	/* �f�B���N�g�����ύX����܂����D */
				{
					/* �ύX���ꂽ�f�B���N�g�����擾 */
					TCHAR l_tszDirChanged[_MAX_DIR * 2];
					memset(l_tszDirChanged, 0, sizeof(l_tszDirChanged));
					CommDlg_OpenSave_GetFolderPath(l_pNMHdr->hwndFrom, l_tszDirChanged, sizeof(l_tszDirChanged));
					switch (ls_iDirChgPrc) {
					case 0:		/* ���������� */
						ls_iDirChgPrc = 1;
						break;
					case 1:		/* �f�B���N�g�����ɖ����ȕ������������`�F�b�N */
								/* �ƌ��ʃf�B���N�g���ȉ����ǂ������`�F�b�N */
								/*	(�����ȃf�B���N�g���ł͂Ȃ��Ė����ȕ����̃`�F�b�N�݂̂ł�)*/
						{
							TCHAR l_tszDirChgd[_MAX_PATH * 2];
							_tcscpy(l_tszDirChgd, l_tszDirChanged);
							l_tszDirChgd[_tcslen(l_pOpenFileNameNotify->lpOFN->lpstrInitialDir)] = 0;
							if (
								(0 != _tcsicmp(l_tszDirChgd, l_pOpenFileNameNotify->lpOFN->lpstrInitialDir))
								|| (0 != _tcspbrk(&(l_tszDirChanged[_tcslen(l_pOpenFileNameNotify->lpOFN->lpstrInitialDir) + 1]), l_tszInvalid2))
								) {
								/* �����ȃf�B���N�g���Ȃ̂ŁC�O�̃f�B���N�g���ɖ߂��܂��D*/
								/* ���͒��̃t�@�C�������ꎞ�L�� */
								CommDlg_OpenSave_GetSpec(l_pNMHdr->hwndFrom, lg_tszEditText, sizeof(lg_tszEditText));
								/* �O�̃f�B���N�g���� EDIT �R���g���[���ɃZ�b�g */
								::SendDlgItemMessage(l_pNMHdr->hwndFrom, edt1, WM_SETTEXT, 0, (LPARAM) lg_tszDirSelect);
								/* OK �{�^�����N���b�N���� */
								::PostMessage(l_pNMHdr->hwndFrom, WM_COMMAND, (WPARAM) IDOK, (LPARAM) 0);
								ls_iDirChgPrc = 2;
							} else {
								_tcscpy(lg_tszDirSelect, l_tszDirChanged);
							}
						}
						break;
					default:	/* �f�B���N�g���߂��҂� */
						if (0 == _tcsicmp(l_tszDirChanged, lg_tszDirSelect)) {
							/* ���͒��̃t�@�C������ EDIT �R���g���[���ɖ߂��܂� */
							::SendDlgItemMessage(l_pNMHdr->hwndFrom, edt1, WM_SETTEXT, 0, (LPARAM) lg_tszEditText);
							ls_iDirChgPrc = 1;
						}
						break;
					}
				}
				break;
			case CDN_FILEOK:	/* OK �{�^����������܂����D */
				{				/*		EDIT �R���g���[������̂Ƃ��̓R�R�ɗ��܂���D */
					/* �t�@�C������(�p�X������)�擾 */
					TCHAR l_tszFile[_MAX_FNAME * 2];
					CommDlg_OpenSave_GetSpec(l_pNMHdr->hwndFrom, l_tszFile, sizeof(l_tszFile));
					/* �g���q�̗L���𔻒f���Ă���΍폜���܂��D */
					TCHAR l_tszExt[_MAX_EXT] = _T(".");
					_tcscat(l_tszExt, l_pOpenFileNameNotify->lpOFN->lpstrDefExt);
					if (0 == _tcsicmp(&(l_tszFile[_tcslen(l_tszFile) - _tcslen(l_tszExt)]), l_tszExt)) {
						/* �w�肳�ꂽ�g���q�����͂���Ă����̂ŁC�폜���܂��D */
						l_tszFile[_tcslen(l_tszFile) - _tcslen(l_tszExt)] = 0;
					}
					if (0 != _tcspbrk(l_tszFile, l_tszInvalid)) {
						/* ������������ */
						/* �G���[���b�Z�[�W�{�b�N�X��\�����܂��D */
						::MessageBox(0, lg_tszErrMsg, lg_tszErrMsgTitle, MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
						/* �_�C�A���O����Ȃ��悤�ɂ��܂��D */
						::SetWindowLong(hwndDlg, DWL_MSGRESULT, 1);
						return 1;
					}
				}
				break;
			}
		}
		break;
	}

	return 0;
}
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */

//�t�@�C���������ĕۑ�
BOOL CMeasurementTabView::GetMeasSaveFileName(char* szThisInputFliePass)
{
// Kojika 20090526 Add
	CString strBuffer1, strBuffer2;
// Kojika 20090526 Add End

//	char szThisInputFliePass[SAVE_FILE_PATH_NAME_LEN+1];
//	memset(szThisInputFliePass, 0, sizeof(szThisInputFliePass));

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	BOOL bSaveOverwrite = rcp_data.MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite;

/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
	CString l_strErrMsg = _T("���ʃf�[�^���ɖ����ȕ���������܂��D");
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
//	CString l_strErrMsgTitle = _T("NanoSpec");
	CString l_strErrMsgTitle;
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
	LoadStringML(IDS_RSLTNM_ERRMSG, l_strErrMsg, _T("Invalid character(s) in Result data name."));
	LoadStringML(IDS_TITLE_NANOSPEC, l_strErrMsgTitle, _T("NanoSpec"));
	_tcscpy(lg_tszErrMsg, l_strErrMsg);
	_tcscpy(lg_tszErrMsgTitle, l_strErrMsgTitle);
	/* �R�����t�@�C���_�C�A���O�̏��� */
	TCHAR l_tszResultFile[_MAX_PATH * 2] = _T(SAVE_DEFAULT_NAME);	/* ���ʃf�B���N�g�����t�@�C���� */
	TCHAR l_tszResultFileTitle[_MAX_FNAME];	/* ���ʃt�@�C����(�p�X����) */
	TCHAR l_tszDefaultDir[_MAX_PATH * 2];	/* ����(�f�t�H���g)�f�B���N�g�� */
	_tcscpy(l_tszDefaultDir, g_szResult_Dir);
	if (_T('\\') == l_tszDefaultDir[_tcslen(g_szResult_Dir) - 1]) {
		/* �Ō��'\'������΍폜���܂��D */
		l_tszDefaultDir[_tcslen(g_szResult_Dir) - 1] = 0;
	}
	OPENFILENAME l_ofn;
	memset(&l_ofn, 0, sizeof(OPENFILENAME));
	l_ofn.lStructSize = sizeof(OPENFILENAME);
	l_ofn.hwndOwner = this->m_hWnd;
	l_ofn.lpstrFilter = _T("DAT Files (*.dat)\0*.dat\0\0");
	l_ofn.nFilterIndex = 0;
	l_ofn.lpstrFile = l_tszResultFile;
	l_ofn.nMaxFile = sizeof(l_tszResultFile);
	l_ofn.lpstrFileTitle = l_tszResultFileTitle;
	l_ofn.nMaxFileTitle = sizeof(l_tszResultFileTitle);
	l_ofn.lpstrInitialDir = l_tszDefaultDir;
	l_ofn.Flags = 0
				| OFN_NOCHANGEDIR
				| OFN_EXPLORER
				| OFN_ENABLEHOOK
				| OFN_HIDEREADONLY
				| 0;
	if ((m_pDoc->GetHostMode() == HOST_LOCAL) && (bSaveOverwrite == FALSE)) {
		/* ���[�J���� ���� �㏑���m�F���K�v */
		l_ofn.Flags |= OFN_OVERWRITEPROMPT; /* �㏑���m�F */
	}
	l_ofn.lpstrDefExt = _T("dat");
	l_ofn.lpfnHook = GetSaveFNHookProc;
	BOOL l_bRet = ::GetSaveFileName(&l_ofn);
	if (0 == l_bRet) {
		return FALSE;
	}
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ----------			   */
//	  CFileDialog dlg( FALSE,
//					  "*.*",
//					  SAVE_DEFAULT_NAME,
//					  OFN_HIDEREADONLY | OFN_NOCHANGEDIR,	  // OFN_NOCHANGEDIR �J�����g�f�B���N�g�����Y����΍�BCFileDialog�̒��O�̃J�����g�f�B���N�g���ɖ߂�
//					  "DAT File (*.dat)|*.dat||",
//					  this);
//
//	  if((m_pDoc->GetHostMode() == HOST_LOCAL)&&(bSaveOverwrite == FALSE)) //���[�J���� ���� �㏑���m�F���K�v
//	  {
//		  dlg.m_ofn.Flags |= OFN_OVERWRITEPROMPT; //�㏑���m�F
//	  }
//
//	  dlg.m_ofn.lpstrInitialDir = g_szResult_Dir;
//
//	  if(dlg.DoModal() != IDOK)
//	  {
//		  return FALSE;
//	  }
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */

/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
//	  if(dlg.GetFileTitle().GetLength() > SAMPLE_ID_LEN)
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ----------			   */
	TCHAR l_tszResultFName[_MAX_FNAME];
	_tcscpy(l_tszResultFName, l_tszResultFileTitle);
	TCHAR *l_pDotPos = _tcsrchr(l_tszResultFName, _T('.'));
	if (0 != l_pDotPos) {
		*l_pDotPos = 0;
	}
	if(_tcslen(l_tszResultFName) > SAMPLE_ID_LEN)
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
	{
// Kojika 20090526 Change
//		MessageBox("�ۑ��������߂��܂�", "����f�[�^�ۑ�", MB_OK);
		LoadStringML(IDS_NAME_TOO_LONG, strBuffer1, "Name is Too Long.");
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change
		return FALSE;
	}

/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
//	  if(dlg.GetPathName().GetLength() > SAVE_FILE_PATH_NAME_LEN)
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ----------			   */
	if(_tcslen(l_tszResultFile) > SAVE_FILE_PATH_NAME_LEN)
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
	{
// Kojika 20090526 Change
//		MessageBox("Path is Too Long.", "Measurement Data Save", MB_OK);
//		MessageBox("�ۑ���p�X�����߂��܂�", "����f�[�^�ۑ�", MB_OK);
		LoadStringML(IDS_PATH_TOO_LONG, strBuffer1, "Path is Too Long.");
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(strBuffer1, strBuffer2, MB_OK);
// Kojika 20090526 Change
		return FALSE;
	}

/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
//	  strcpy(szThisInputFliePass, dlg.GetPathName());
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ----------			   */
	_tcscpy(szThisInputFliePass, l_tszResultFile);
/* modified 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */


	char szFullPath[MAX_PATH];
//	memset(szFullPath, 0, sizeof(szFullPath));
//	_fullpath(szFullPath, (const char *)RESULT_DIR, sizeof(szFullPath));
	strcpy(szFullPath, g_szResult_Dir);
	if(StrStrI(szThisInputFliePass, szFullPath)==NULL)
	{
		char szMessage[1024+1];
		memset(szMessage, 0, sizeof(szMessage));
// Kojika 20090526 Change
//		sprintf(szMessage, "%s%s", szFullPath, " �z���ȊO�ɂ͕ۑ����邱�Ƃ��ł��܂���B\n�ēx�A�ۑ����Ȃ����Ă��������B");
//		MessageBox(szMessage, "����f�[�^�ۑ�", MB_OK);
//		sprintf(szMessage, " %s �z���ȊO�ɂ͕ۑ����邱�Ƃ��ł��܂���B\n�ēx�A�ۑ����Ȃ����Ă��������B", szFullPath);
		LoadStringML(IDS_NOT_POSSIBLE_SAVE_BESIDE_BELOW, strBuffer1, "It isn't possible to save it besides below %s. \nPlease save it again.");
		sprintf(szMessage, strBuffer1, szFullPath);
		LoadStringML(IDS_TITLE_MEASURE_DATA_SAVE, strBuffer2, "Measurement Data Save");
		MessageBox(szMessage, strBuffer2, MB_OK);
// Kojika 20090526 Change End
		return FALSE;
	}

	return TRUE; //�����܂ŗ����Ȃ琳��
}



/////////////////////////////////////////////////////////////////////////////
//�㏑���m�F�_�C�A���O���o��
int CMeasurementTabView::SaveOverlap(char* szSaveFullFilePath)
{
	int iSaveNameOverlap;
	CSaveNameOverlapDlg SaveDlg;
	strcpy(SaveDlg.m_szSpecifiedFileName, szSaveFullFilePath); //�d���t�@�C�������_�C�A���O�ɓn��
	iSaveNameOverlap = SaveDlg.DoModal();

	if(iSaveNameOverlap == SAVEOVERLAP_SAVE)		   //�㏑���ۑ�(SAVE)
	{
		//�������s
	}
	else if(iSaveNameOverlap == SAVEOVERLAP_NOSAVE)    //�ۑ����Ȃ�(CANCEL)
	{
		//
		return 2;	//2:���s(�ۑ����Ȃ�)
	}
	else if(iSaveNameOverlap == SAVEOVERLAP_SAVEAS)    //���O��t���ĕۑ�(SAVE AS)
	{
		//���O��t���ĕۑ�(�����̑I���ɂ���Ắw�ۑ����Ȃ��x)
		if(!GetMeasSaveFileName(szSaveFullFilePath))
		{
			return 3;	//3:���s(�ۑ��L�����Z��)
/* added 2016.08.31 hmenjo ���ʃf�[�^����ɍ폜 ---------- { ---------- */
		} else {
			/* ���O��t���ĕۑ�(SAVE AS)�ŏ������s	*/
			return -2;
/* added 2016.08.31 hmenjo ���ʃf�[�^����ɍ폜 ---------- } ---------- */
		}
	}

	return -1; //�������s
}



/////////////////////////////////////////////////////////////////////////////
//�t�@�C�����݃`�F�b�N
BOOL CMeasurementTabView::FileOrDirExists(LPCTSTR pszFileName)
{
	return (BOOL)( GetFileAttributes(pszFileName) != 0xffffffff );
}



/////////////////////////////////////////////////////////////////////////////
//�p�X������A�t�@�C�����̊J�n�ʒu���擾����
//�p�X���̖�������p�X���Ō�� '\' �܂ł��A�t�@�C�����Ɣ��f����
char * CMeasurementTabView::GetFileName( const char *PathName )
{
	for( const char *p = strtail(PathName); p >= PathName; p-- ) // ������̍Ōォ��ŏ��܂�
	{
		if ( ('\\'==*p) && !isJMS(PathName,p-PathName) )
			return (char*)(p+1);
	}
	return (char *)PathName; // ������� '\' �͊܂܂�Ă��Ȃ�����
}//GetFileName


/////////////////////////////////////////////////////////////////////////////
//�����񖖔��̕��� '\0' �̈ʒu��Ԃ�
const char * CMeasurementTabView::strtail( const char *string )
{
	return strchr( string, '\0' );
}//strtail


/////////////////////////////////////////////////////////////////////////////
//�����񒆂̂���P�o�C�g�ɂ��āA�P�o�C�g�������Q�o�C�g�����̈ꕔ�ł��邩�𔻒肷��B
//*********************************************************
// ������ str �� str[nPos] �ɂ��āA
//	 �O �c�c �P�o�C�g����
//	 �P �c�c �Q�o�C�g�����̈ꕔ�i��P�o�C�g�j
//	 �Q �c�c �Q�o�C�g�����̈ꕔ�i��Q�o�C�g�j
// �̂����ꂩ��Ԃ��B
//*********************************************************
#define jms1(c) (((0x81 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0x9F)) || ((0xE0 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC) ))
#define jms2(c) ((0x7F != (unsigned char)(c)) && (0x40 <= ((unsigned char)(c))) && (((unsigned char)(c)) <= 0xFC))
int CMeasurementTabView::isJMS( const char *str, int nPos )
{
	int i;
	int state; // { 0, 1, 2 }

	// 0 = �P�o�C�g����
	// 1 = �Q�o�C�g�����̑�P�o�C�g
	// 2 = �Q�o�C�g�����̑�Q�o�C�g
	state = 0;
	for( i = 0; str[i] != '\0'; i++ )
	{
		if		( ( state == 0 ) && ( jms1( str[i] ) ) ) state = 1; // 0 -> 1
		else if ( ( state == 1 ) && ( jms2( str[i] ) ) ) state = 2; // 1 -> 2
		else if ( ( state == 2 ) && ( jms1( str[i] ) ) ) state = 1; // 2 -> 1
		else											 state = 0; // 2 -> 0, ���̑�

		// str[nPos] �ł̏�Ԃ�Ԃ��B
		if ( i == nPos ) return state;
	}
	return 0;
}//isJMS



void CMeasurementTabView::OnUpdateSaveResultData(CCmdUI* pCmdUI)
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

	//2009.11.03 bagus MS --{--
	//����������͕ۑ������Ȃ�
	{
		RCP_DATA rcp;
		m_pDoc->GetRcpData(&rcp);
		if(rcp.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
			pCmdUI->Enable(FALSE);
			return ;
		}
	}
	//2009.11.03 bagus MS --}--


	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A�����胂�[�h
	{
		if(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP)
		{
			pCmdUI->Enable(TRUE);
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}
	else //�ʏ푪�胂�[�h(�����W���[�܂�)
	{

		if(m_pDoc->GetProcessStatus() == PROCESS_WAIT)
		{
			pCmdUI->Enable(TRUE);
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}

}


void CMeasurementTabView::OnSaveResultData()
{
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
	OnSaveAsMain(MEASUREMENT_TOOLBAR_SAVE);
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
/* deleted 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
//	  // TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
//
////�c�[���o�[�̃Z�[�u�{�^���������̏���
//
//// Kojika 20090526 Add
//	  CString strBuffer, strTitle;
//// Kojika 20090526 Add End
//
////  BOOL bCheck;
//int iCheck=0;
//	  RCP_DATA rcp_data;
//	  memset(&rcp_data, 0, sizeof(rcp_data));
//	  m_pDoc->GetRcpData(&rcp_data);
//
//	  iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_TOOLBAR_SAVE);
//
//	  //Saiki 20090406 Change ----->
////	if(iCheck != 1)
////	{
//////		MessageBox("Save UnCompleted.", "", MB_OK); //�Z�[�u���s
////		MessageBox("�ۑ����s", "NanoSpec", MB_OK); //�Z�[�u���s
////	}
////  else
////  {
////	  m_bToolbarSaveCompFlg = TRUE; //�c�[���o�[�ŕۑ���
////  }
//	  if(iCheck != 1 && iCheck != 3){ //�ۑ����s
//		  AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //���茋�ʕۑ����s
//		  if( m_pDoc->GetHostMode() == HOST_LOCAL ){
//// Kojika 20090526 Change
////		  MessageBox("Save UnCompleted.", "NanoSpec", MB_OK); //�Z�[�u���s
////		  MessageBox("�ۑ��Ɏ��s���܂���", "NanoSpec", MB_OK); //�Z�[�u���s
//			  LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
//			  LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//			  MessageBox(strBuffer, strTitle, MB_OK); //�Z�[�u���s
//// Kojika 20090526 Change END
//		  }
//	  }
//	  else if(iCheck == 3){//�ۑ��L�����Z��
//		   //�ۑ��L�����Z���Ȃ̂ŉ������Ȃ�
//	  }
//	  else{//�ۑ�����
//		  m_bToolbarSaveCompFlg = TRUE; //�c�[���o�[�ŕۑ���
//	  }
//	  //Saiki 20090406 Change <-----
//
/* deleted 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
}





// ==========================================================================
//���蒆�̑���I�� ���̃{�^���L��/�����̐ؑւ�
//�ʏ푪��E�����W���[��p�B �}�j���A�����胂�[�h�ł̐ؑւ���ManualMeasurementFormView.cpp�ɋL�q
void CMeasurementTabView::ButtonEnableChange(int Select)
{
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);
	WORD wNumScans;


	//�����[�g���͑����Ԃɂ�����炸�A�{�^������������������
	if(m_pDoc->GetHostMode() == HOST_REMOTE) //�����[�g��
	{
		if(m_pDoc->GetMeasMode() == MEASMODE_SEQ_OR_REMEASURE) //�ʏ푪��or�����W���[
		{
			//IDD_MAIN_MENU_FORM���R���g���[���̖�����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE); //[LOG ON]�{�^������

			//IDD_MEASUREMENT_PROCESS_FORM�R���g���[���̖�����
//			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_CANCEL_BUTTON)->EnableWindow(FALSE); 	//[���蒆�~]�{�^������
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->EnableWindow(FALSE);		//[���蒆�f(�ĊJ)]�{�^������
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[REMEASUREMENT]�{�^������
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);	//[�ʒu�␳��đ���]�{�^������
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]�{�^������

			//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//�����Y�{���I�𖳌�
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�J�����I�𖳌�
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]�{�^������
			//Saiki 20090410 Add ----->
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�w�b�h�֖ؑ���
			//Saiki 20090410 Add <-----
			//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
			m_pMainFrame->m_MeasMenuEnableFlg = FALSE;
		}
		return;
	}


	switch(Select){
	case MEASUREMENT_SEQ_S:

		//IDD_MEASUREMENT_PROCESS_FORM�R���g���[���̖�����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[REMEASUREMENT]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(FALSE);	//[�ʒu�␳��đ���]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(FALSE);					//[EXIT]�{�^������

		//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(FALSE);	//�����Y�{���I�𖳌�
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�J�����I�𖳌�
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE); 		//[AUTO FOCUS]�{�^������
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�w�b�h�֖ؑ���
		//Saiki 20090410 Add <-----

		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
		m_pMainFrame->m_MeasMenuEnableFlg = FALSE;

		break;


	case MEASUREMENT_SEQ_E:

		//IDD_MEASUREMENT_PROCESS_FORM�R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_CANCEL_BUTTON)->EnableWindow(TRUE);	//[���蒆�~]�{�^���L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_PAUSE_BUTTON)->EnableWindow(TRUE);	//[���蒆�f(�ĊJ)]�{�^���L��
		//2009.10.28 bagus 2point-distance --{--
		wNumScans = rcp_data.StageProgInfoHdr.wNumScans;
		if(rcp_data.StageProgInfoHdr.wHeadType == HEAD_TYPE_SR
		&& rcp_data.StageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE){
			wNumScans *= 2;
		}
		//if(rcp_data.StageProgInfoHdr.wNumScans == m_pDoc->GetStoreCount()) //���萔���\�葪��ɓ��B���Ă���
		if(wNumScans == m_pDoc->GetStoreCount()) //���萔���\�葪��ɓ��B���Ă���
		//2009.10.28 bagus 2point-distance --}--
		{
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_REMEASUREMENT_BUTTON)->EnableWindow(TRUE);		//[REMEASUREMENT]�{�^���L��
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_ADUJST_REMEASUREMENT_BUTTON)->EnableWindow(TRUE);//[�ʒu�␳��đ���]�{�^���L��
		}
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);				//[EXIT]�{�^���L��
		//2009.12.20 bagus �C�� --{--
		((CMeasurementProcessView *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0)))->ChangeOperationEnable(FALSE);
		//2009.12.20 bagus �C�� --}--

		//IDD_VIEW_WINDOW_FORM���R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(TRUE);	//�����Y�{���I��L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE); 	//�J�����I��L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(TRUE);			//[AUTO FOCUS]�{�^���L��
		//Saiki 20090410 Add ----->
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(TRUE);	//�w�b�h�ؑ֗L��
		//Saiki 20090410 Add <-----

		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̗L�����t���O����
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;


//	  case MEASUREMENT_TEST_S:
//		  break;

//	  case MEASUREMENT_TEST_E:
//		  break;


	case MEAS_TAB_VIEW_CREATE:
		//IDD_MAIN_MENU_FORM���R���g���[���̖�����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(FALSE);		//[MEASUREMENT]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(FALSE);	//[RECIPE]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_LOG_ON_BUTTON)->EnableWindow(FALSE);		//[LOG ON]�{�^������

		if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE) //�}�j���A������
		{
			//IDD_VIEW_WINDOW_FORM���R���g���[���̖�����
			(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(2, 0))->GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(FALSE);	//�J�����I�𖳌�
		}


		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̖������t���O����
		m_pMainFrame->m_MeasMenuEnableFlg2 = FALSE;

		break;


	case MEAS_TAB_VIEW_DELETE:
		//IDD_MAIN_MENU_FORM���R���g���[���̗L����
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_MEASUREMENT_BUTTON)->EnableWindow(TRUE);	//[MEASUREMENT]�{�^���L��
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(0, 0))->GetDlgItem(IDC_MAIN_MENU_RECIPE_SETTING_BUTTON)->EnableWindow(TRUE);//[RECIPE]�{�^���L��

		//���j���[�o�[��(IDR_MEASUREMENT_MENU)�̗L�����t���O����
		m_pMainFrame->m_MeasMenuEnableFlg2 = TRUE;
		m_pMainFrame->m_MeasMenuEnableFlg = TRUE;

		break;
	}

}



void CMeasurementTabView::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	int iProcessStatus = m_pDoc->GetProcessStatus();
	int iHostMode = m_pDoc->GetHostMode();
	int iMeasMode = m_pDoc->GetMeasMode();	//���胂�[�h�擾
	int iManuMeasMode = m_pDoc->GetManuMeasMode();

	if((iProcessStatus != m_iProcessStatusOld)||(iHostMode != m_iHostModeOld))
	{
		switch (nIDEvent)
		{
			case TABVIEW_ENABLE_CHANGE_TIMER:		// 50ms ������^�C�}

//				int iMeasMode = m_pDoc->GetMeasMode();	//���胂�[�h�擾
				if(iMeasMode == MEASMODE_SEQ_OR_REMEASURE) //�ʏ푪��or�����W���[
				{

					if((iProcessStatus == PROCESS_WAIT)||(iProcessStatus == PROCESS_DOWN))
					{
						ButtonEnableChange(MEASUREMENT_SEQ_E);
					}
					else
					{
						ButtonEnableChange(MEASUREMENT_SEQ_S);
					}
				}
				else if(iMeasMode == MEASMODE_MANUALMEASURE) //�}�j���A������
				{
					//�}�j���A������̃{�^������́AManualMeasurementFormView.cpp�ɋL�q

				}
//			  else if(iMeasMode == MEASMODE_TEST) //�e�X�g���[�h
//			  {
//
//			  }
				else
				{
					//�����Ȃ�
				}

				break;
		}
	}

	//�O��l�X�V
	m_iProcessStatusOld = iProcessStatus;
	m_iHostModeOld = iHostMode;



	//�O��l�X�V
	m_iManuMeasModeOld = iManuMeasMode;


	CNanoUI::OnTimer(nIDEvent);
}



int CMeasurementTabView::FolderCopy(LPCTSTR szFrom, LPCTSTR szTo )
{
	SHFILEOPSTRUCT tSHFile;

	ZeroMemory(&tSHFile, sizeof(SHFILEOPSTRUCT));

	CString from = szFrom;
	CString to = szTo;

	tSHFile.hwnd = m_hWnd;
	tSHFile.wFunc = FO_COPY;
	tSHFile.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SIMPLEPROGRESS ;

	tSHFile.fAnyOperationsAborted = TRUE;
	tSHFile.hNameMappings = NULL;
	tSHFile.lpszProgressTitle = NULL;

	from += "0";
	to += "0";
	from.SetAt( from.GetLength()-1, NULL );
	to.SetAt( to.GetLength()-1, NULL );

	tSHFile.pFrom = (LPCTSTR)from;
	tSHFile.pTo = (LPCTSTR)to;

	return SHFileOperation( &tSHFile ); // ���s

}



/////////////////////////////////////////////////////////////////////////////
// Make Directory
int CMeasurementTabView::MakeDirectory(char* dir)
{
	char buff[256];
	int i;
	SECURITY_ATTRIBUTES sa;
	int ret;

	sa.lpSecurityDescriptor = NULL;

	if((ret = CreateDirectory(dir,&sa)) == TRUE) return 1;
	for(i=strlen(dir);i>0;i--){
		if(dir[i] == '\\' || dir[i] == '/'){
			break;
		}
	}
	if(i <= 0) return 0;
	strcpy(buff,dir);
	buff[i]=NULL;
	if(ret = MakeDirectory(buff)){
		return (ret = CreateDirectory(dir,&sa)) == TRUE ? 1 : 0;
	}
	return 0;
}



void CMeasurementTabView::OnDestroy()
{
	CNanoUI::OnDestroy();

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	//�X�e�[�^�X�o�[�����N���A
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

}

/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
void CMeasurementTabView::OnSaveAsMain(int iSaveWay)
{
	CString strBuffer, strTitle;

	int iCheck=0;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	//2009.11.03 bagus MS --{--
	//����������̏ꍇ�ɂ͕ۑ������Ȃ�
	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS){
		iCheck = 1;
	}else{
		iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, iSaveWay);
	}
	//2009.11.03 bagus MS --}--


	if(iCheck != 1 && iCheck != 3){ //�ۑ����s
		AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //���茋�ʕۑ����s
		if( m_pDoc->GetHostMode() == HOST_LOCAL ){
			LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
			LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
			MessageBox(strBuffer, strTitle, MB_OK); //�Z�[�u���s
		}
	}
	else if(iCheck == 3){//�ۑ��L�����Z��
			//�ۑ��L�����Z���Ȃ̂ŉ������Ȃ�
	}
	else{//�ۑ�����
		m_bToolbarSaveCompFlg = TRUE; //�c�[���o�[�ŕۑ���
	}
}
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
void CMeasurementTabView::OnSaveAs()
{
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
	OnSaveAsMain(MEASUREMENT_TOOLBAR_SAVE_AS);
/* added 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
/* deleted 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- { ---------- */
//	  //�c�[���o�[�̃Z�[�u�{�^���������Ɠ��l�̏���
//
//// Kojika 20090526 Add
//	  CString strBuffer, strTitle;
//// Kojika 20090526 Add End
//
//	  int iCheck=0;
//	  RCP_DATA rcp_data;
//	  memset(&rcp_data, 0, sizeof(rcp_data));
//	  m_pDoc->GetRcpData(&rcp_data);
//
//	  iCheck = SaveMeasConAddRevi(rcp_data.szRecipeName, (int)MEASUREMENT_TOOLBAR_SAVE_AS);
//
//	  //Saiki 20090406 Change ----->
////	if(iCheck != 1){
////		MessageBox("�ۑ����s", "NanoSpec", MB_OK); //�Z�[�u���s
////	}
////	else{
////	  m_bToolbarSaveCompFlg = TRUE; //�c�[���o�[�ŕۑ���
////  }
////  }
//	  if(iCheck != 1 && iCheck != 3){ //�ۑ����s
//		  AlarmIf_Set(ALID_MeasurementResultRetentionFailed); //���茋�ʕۑ����s
//		  if( m_pDoc->GetHostMode() == HOST_LOCAL ){
//// Kojika 20090526 Change
////				  MessageBox("Save UnCompleted.", "NanoSpec", MB_OK); //�Z�[�u���s
////		  MessageBox("�ۑ��Ɏ��s���܂���", "NanoSpec", MB_OK); //�Z�[�u���s
//			  LoadStringML(IDS_SAVE_UNCOMPLETED, strBuffer, "Save UnCompleted.");
//			  LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec")
//			  MessageBox(strBuffer, strTitle, MB_OK); //�Z�[�u���s
//// Kojika 20090526 Change END
//		  }
//	  }
//	  else if(iCheck == 3){//�ۑ��L�����Z��
//		   //�ۑ��L�����Z���Ȃ̂ŉ������Ȃ�
//	  }
//	  else{//�ۑ�����
//		  m_bToolbarSaveCompFlg = TRUE; //�c�[���o�[�ŕۑ���
//	  }
//	  //Saiki 20090406 Change <-----
/* deleted 2009.06.16 hmenjo �g�p�֎~�����`�F�b�N ---------- } ---------- */
}

void CMeasurementTabView::OnUpdateSaveAs(CCmdUI* pCmdUI)
{
	if(m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE){ //�}�j���A�����胂�[�h
		if(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP){
			pCmdUI->Enable(TRUE);
		}
		else{
			pCmdUI->Enable(FALSE);
		}
	}
	else{ //�ʏ푪�胂�[�h(�����W���[�܂�)

		if(m_pDoc->GetProcessStatus() == PROCESS_WAIT){
			pCmdUI->Enable(TRUE);
		}
		else{
			pCmdUI->Enable(FALSE);
		}
	}
}

/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
/*
 *	������
 */
void CMeasurementTabView::MRCS_Initialize()
{
	memset(m_MRCS_dwSettings, 0, sizeof(m_MRCS_dwSettings));
	m_MRCS_wReMeasureNo = 0;
	m_MRCS_wIncludeColorSetNo = 0;
	m_MRCS_bIncludeColorSetList = FALSE;
	m_MRCS_bIncludeColorSetGraph = FALSE;
	m_MRCS_wShowCompletionPointSnapL = 0;
	m_MRCS_wShowCompletionPointSnapG = 0;
}
/*
 *	�@�\�̎��s�\�������`�F�b�N���܂��D
 *		DWORD dwMode
 *				���O�F�S�`�F�b�N
 *				���P�F�đ��莞�p�`�F�b�N
 *				��L�ȊO�F���O�Ɠ���
 */
BOOL CMeasurementTabView::MRCS_CheckPermission(DWORD dwMode/* = 0*/)
{
	BOOL l_bRc = FALSE;

	/* �蓮���[�h�ȊO�͏��O	*/
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		return FALSE;
	}

	/* �X�g���X����͏��O	*/
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS) {
		return FALSE;
	}

	/* ����������͏��O	*/
	if (m_MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_MS) {
		return FALSE;
	}

	/* SR_DISTANCE �͏��O	*/
	if (m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_DISTANCE) {
		return FALSE;
	}

	/* GTR �͍đ���̂ݏ��O	*/
	if ((dwMode == 1) &&
		(m_MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)) {
		return FALSE;
	}

	return TRUE;
}
/*
 *	�@�\�̉E�_�u���N���b�N�̎��s�\�������`�F�b�N���܂��D
 */
BOOL CMeasurementTabView::MRCS_CheckRunPermission()
{
	BOOL l_bRc = FALSE;

	/* �蓮���[�h�ȊO�͏��O	*/
	if (m_pDoc->GetMeasMode() != MEASMODE_MANUALMEASURE) {
		return FALSE;
	}
/* added 2016.06.02 hmenjo �蓮 �đ���/�w�蓝�v ---------- { ---------- */
	/* �蓮���[�h�ő��芮���͏��O	*/
	if (m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP) {
		return FALSE;
	}
/* added 2016.06.02 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */

	if (FALSE != m_pMainFrame->m_ManualMeasMenuEnableFlg) {
		l_bRc = TRUE;
	}

	return l_bRc;
}
/*
 *	�E�_�u���N���b�N����
 */
BOOL CMeasurementTabView::MRCS_RDblClk(POINT ptGrid, CGridCtrl* pclsGrid/* = 0*/)
{
	BOOL l_bRc = FALSE;
	int i;

	if (0 == pclsGrid) {
		return FALSE;
	}

	CGridCtrl* l_pclsGrid = pclsGrid;

	if (FALSE != this->MRCS_CheckRunPermission()) {
		if (FALSE != this->MRCS_CheckPermission()) {
			CCellID l_clsCellID = l_pclsGrid->GetCellFromPt(ptGrid, FALSE);
			if (FALSE != l_clsCellID.IsValid()) {
				/* �N���b�N�����Z���͗L��	*/
				int l_iMeasCount = m_pDoc->GetMeasurePointCount();
				if (l_clsCellID.row <= l_iMeasCount) {
					/* �N���b�N�s No. �����萔�ȓ�	*/
					WORD l_wClickLine = l_clsCellID.row;
					/* �L���ȍs�ł̑I����Ԃ̍s�����擾	*/
//	�e�X�g�p		int l_iSelCnt = m_MeasurementListGrid.GetSelectedCount();
//	�e�X�g�p		if (FALSE != m_MeasurementListGrid.IsCellSelected(l_clsCellID)) {
					int l_iSelCount = 0;
					BOOL l_bIncludedInSelected = FALSE;	/* �I���s���Ɏw��s���݂�t���O	*/
					for (i = 1; i <= l_iMeasCount; i++) {
						if (FALSE != l_pclsGrid->IsCellSelected(i, l_clsCellID.col)) {
							/* �I���s	*/
							l_iSelCount++;
							if (1 == this->MRCS_SettingGet(i)) {
								l_bIncludedInSelected = TRUE;
							}
							m_MRCS_dwSettings[i] |= 0x80000000;
						} else {
							/* ��I���s	*/
							m_MRCS_dwSettings[i] &= 0x7fffffff;
						}
					}
					/* �\�ȑ���̔���	*/
					DWORD l_dwSecondCheckID = 0;	/* �������� ID	*/
					switch (l_iSelCount) {
					case 0:		/* �I����Ԗ���	*/
						l_dwSecondCheckID = 2;
						break;
					case 1:		/* �P�s�I��	*/
						if (FALSE != l_pclsGrid->IsCellSelected(l_clsCellID)) {
							/* �N���b�N�s���I�����	*/
							l_dwSecondCheckID = 3;
						} else {
							/* �N���b�N�s����I�����	*/
							l_dwSecondCheckID = 2;
						}
						break;
					default:	/* �����s�I��	*/
						if (FALSE != l_pclsGrid->IsCellSelected(l_clsCellID)) {
							/* �N���b�N�s���I�����	*/
							l_dwSecondCheckID = 1;
						} else {
							/* �N���b�N�s����I�����	*/
							l_dwSecondCheckID = 2;
						}
						break;
					}
					WORD l_wEnableSelect = 0;
					switch (l_dwSecondCheckID) {
					case 1:
						if (FALSE != l_bIncludedInSelected) {
							/* �I����Ԃ̍s�Ɏw��s���݂�	*/
							l_wEnableSelect |= 0x0001;	/* �w��s	*/
							l_wEnableSelect |= 0x0002;	/* �w�����	*/
						} else {
							/* �I����Ԃ̍s�Ɏw��s������	*/
							l_wEnableSelect |= 0x0001;	/* �w��s	*/
						}
						break;
					case 2:
						if (2 != this->MRCS_SettingGet(l_clsCellID.row)) {
							/* �N���b�N�s���폜�ςݍs�łȂ�	*/
							if (1 < (l_iMeasCount - MRCS_DeleteCountGet())) {
								/* ��폜�ςݍs���Q�ȏ�̏ꍇ�̂ݍ폜�\�ł��D	*/
								l_wEnableSelect |= 0x0004;	/* �폜	*/
							}
							if (FALSE != this->MRCS_CheckPermission(1)) {
								l_wEnableSelect |= 0x0010;	/* �đ���	*/
							}
						} else {
							/* �N���b�N�s���폜�ςݍs�ł���	*/
							l_wEnableSelect |= 0x0008;	/* �폜����	*/
							if (FALSE != this->MRCS_CheckPermission(1)) {
								l_wEnableSelect |= 0x0010;	/* �đ���	*/
							}
						}
						break;
					case 3:
						if ((1 != this->MRCS_SettingGet(l_clsCellID.row)) &&
							(2 != this->MRCS_SettingGet(l_clsCellID.row))) {
							/* �N���b�N�s���w��s�łȂ��C�폜�ςݍs�ł��Ȃ�	*/
							l_wEnableSelect |= 0x0001;	/* �w��s	*/
						} else if (1 == this->MRCS_SettingGet(l_clsCellID.row)) {
							/* �N���b�N�s���w��s�ł���	*/
							l_wEnableSelect |= 0x0002;	/* �w�����	*/
						}
						break;
					default:
						/* �������Ȃ�	*/
						break;
					}
					if (0 < MRCS_DeleteCountGet()) {
						/* �폜�ςݍs���݂�	*/
						l_wEnableSelect |= 0x1000;	/* �폜�ςݍs�݂�	*/
					}
					if (0 < MRCS_IncludeCountGet()) {
						/* �w��s���݂�	*/
						l_wEnableSelect |= 0x2000;	/* �w��s�݂�	*/
					}
					if (0 != l_wEnableSelect) {
						/* �����ʂ�\��	*/
						l_bRc = TRUE;
						CManuReMeasReCalcStatDlg l_dlgMRCS;
						l_dlgMRCS.ParamSet(MAKELPARAM(l_wEnableSelect, l_wClickLine));
						if (IDOK == l_dlgMRCS.DoModal()) {
							/* OK	*/
							DWORD l_dwRslt;
							l_dlgMRCS.ParamGet((LPARAM*) &l_dwRslt);
							WORD l_wSelectNo = LOWORD(l_dwRslt);
							BOOL l_bCalcStat = FALSE;	/* ���v�v�Z�K�v	*/
							switch (l_wSelectNo) {
							case 1:		/* �w��s	*/
							case 2:		/* �w�����	*/
								l_bCalcStat = TRUE;
								for (i = 1; i <= l_iMeasCount; i++) {
									if (0 != (m_MRCS_dwSettings[i] & 0x80000000)) {
										/* �I���s	*/
										switch (l_wSelectNo) {
										case 1:		/* �w��s	*/
											if (2 != this->MRCS_SettingGet(i)) {
												/* �폜�ςݍs�ȊO	*/
												this->MRCS_SettingSet(i, 1);
												this->MRCS_GridLineColorSet(i, CYAN_COLOR);
												this->MRCS_IncludeCountInc();
											}
											break;
										case 2:		/* �w�����	*/
											if (1 == this->MRCS_SettingGet(i)) {
												/* �w��s	*/
												this->MRCS_SettingSet(i, 0);
												this->MRCS_GridLineColorSet(i, CLR_DEFAULT);
												this->MRCS_IncludeCountDec();
											}
											break;
										}
									}
								}
								break;
							case 3:		/* �폜	*/
								l_bCalcStat = TRUE;
								if (1 == this->MRCS_SettingGet(l_wClickLine)) {
									/* �w��s�������ꍇ�̓f�N�������g���܂��D	*/
									this->MRCS_IncludeCountDec();
								}
								this->MRCS_SettingSet(l_wClickLine, 2);
								this->MRCS_GridLineColorSet(l_wClickLine, MAGENTA_COLOR);
								this->MRCS_DeleteCountInc();
								break;
							case 4:		/* �폜����	*/
								if (0 == MRCS_IncludeCountGet()) {
									/* �w��s�������ꍇ�̂ݓ��v�v�Z���܂��D	*/
									l_bCalcStat = TRUE;
								}
								this->MRCS_SettingSet(l_wClickLine, 0);
								this->MRCS_GridLineColorSet(l_wClickLine, CLR_DEFAULT);
								this->MRCS_DeleteCountDec();
								break;
							case 5:		/* �đ���	*/
								m_MRCS_wReMeasureNo = l_wClickLine;
								if (2 == this->MRCS_SettingGet(m_MRCS_wReMeasureNo)) {
									/* �폜�ςݍs�ł���	*/
									this->MRCS_DeleteCountDec();
									this->MRCS_SettingSet(m_MRCS_wReMeasureNo, 0);
									this->MRCS_GridLineColorSet(m_MRCS_wReMeasureNo, CLR_DEFAULT);
								}
								m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas();
								break;
							case 13:	/* ���ׂĂ̍폜�f�[�^�𕜊�	*/
							case 14:	/* ���ׂĂ̎w��s������	*/
								{
									WORD l_wDiffPrm;
									switch (l_wSelectNo) {
									case 13:	/* ���ׂĂ̍폜�f�[�^�𕜊�	*/
										if (0 == MRCS_IncludeCountGet()) {
											/* �w��s�������ꍇ�̂ݓ��v�v�Z���܂��D	*/
											l_bCalcStat = TRUE;
										}
										l_wDiffPrm = 2;
										m_MRCS_dwSettings[0] &= 0xffff0000;
										break;
									case 14:	/* ���ׂĂ̎w��s������	*/
										l_bCalcStat = TRUE;
										l_wDiffPrm = 1;
										m_MRCS_dwSettings[0] &= 0x0000ffff;
										break;
									}
									for (i = 1; i <= l_iMeasCount; i++) {
										if (l_wDiffPrm == this->MRCS_SettingGet(i)) {
											this->MRCS_SettingSet(i, 0);
											this->MRCS_GridLineColorSet(i, CLR_DEFAULT);
										}
									}
								}
								break;
							default:
								break;
							}
							/* �I����Ԃ�����	*/
							for (i = 1; i <= SCAN_POINT_MAX; i++) {
								m_MRCS_dwSettings[i] &= 0x7fffffff;
							}
							m_pMeasurementListDlg->MeasurementListGrid_UnSelectAll();
							m_pMeasurementGraphDlg->MeasurementListGrid_UnSelectAll();
//	�I����Ԃɂ͂��Ȃ����Ƃɂ��܂��D
//							if (5 == l_wSelectNo) {
//								/* �đ����͂��̍s��I����Ԃɂ��܂��D	*/
//								m_MRCS_dwSettings[l_wClickLine] |= 0x80000000;
//							}
							/* ���v�v�Z	*/
							if (FALSE != l_bCalcStat) {
								STATISTICS l_Statistics[ADAPRESULT_COLS_MAX];	/* ���v�f�[�^	*/
								memset(l_Statistics, 0, sizeof(l_Statistics));
								/* ���v�v�Z	*/
								m_pMainFrame->GlassDataStatistics(
										l_Statistics,
										m_pDoc->GetDispLabelCount(),
										m_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption
									);
								/* ���v�f�[�^�ۑ�	*/
								m_pDoc->SetStatisticsData(l_Statistics);
								/* ���v�f�[�^�\��	*/
//	�s�v�ł��D							m_pDoc->SetShowCompPointGraph(l_iMeasCount - 1);
								/*		���v�f�[�^�̕\���� CMeasurementListDlg �݂̂ōs���Ă��邽��
										CMeasurementGraphDlg ���͕s�v�ł��D	*/
								m_pDoc->SetShowCompPointList(l_iMeasCount - 1);
							}
						} else {
							/* Cancel	*/
							/*	�����͉�������܂���D	*/
						}
					}
				}
			}
		}
	}

	return l_bRc;
}
/*
 *	�ݒ�o�b�t�@����ݒ�̂ݎ��o��
 */
WORD CMeasurementTabView::MRCS_SettingGet(long lIndex)
{
	return LOWORD(m_MRCS_dwSettings[lIndex]);
}
/*
 *	�ݒ�o�b�t�@�ɐݒ��������
 */
void CMeasurementTabView::MRCS_SettingSet(long lIndex, WORD wSetting)
{
	WORD l_wStatus = HIWORD(m_MRCS_dwSettings[lIndex]);
	m_MRCS_dwSettings[lIndex] = (DWORD) MAKELONG(wSetting, l_wStatus);
}
/*
 *	�ݒ�o�b�t�@�����Ԃ̂ݎ��o��
 */
WORD CMeasurementTabView::MRCS_SettingStatusGet(long lIndex)
{
	return HIWORD(m_MRCS_dwSettings[lIndex]);
}
/*
 *	�ݒ�o�b�t�@�ɏ�Ԃ̂ݏ�����
 */
void CMeasurementTabView::MRCS_SettingStatusSet(long lIndex, WORD wStatus)
{
	WORD l_wSetting = LOWORD(m_MRCS_dwSettings[lIndex]);
	m_MRCS_dwSettings[lIndex] = (DWORD) MAKELONG(l_wSetting, wStatus);
}
/*
 *	�ݒ�o�b�t�@����S�������o��
 */
DWORD CMeasurementTabView::MRCS_SettingAllGet(long lIndex)
{
	return m_MRCS_dwSettings[lIndex];
}
/*
 *	�w�胊�X�g�s�̐F�ݒ�
 */
void CMeasurementTabView::MRCS_GridLineColorSet(long lIndex, COLORREF cr/* = CLR_DEFAULT*/)
{
	/* CMeasurementListDlg	*/
	for (int i = 1; i < m_pMeasurementListDlg->m_MeasurementListGrid.GetColumnCount(); i++) {
		m_pMeasurementListDlg->m_MeasurementListGrid.SetItemBkColour(lIndex, i, cr);
	}
	/* CMeasurementGraphDlg	*/
	for (i = 1; i < m_pMeasurementGraphDlg->m_MeasurementListGrid.GetColumnCount(); i++) {
		m_pMeasurementGraphDlg->m_MeasurementListGrid.SetItemBkColour(lIndex, i, cr);
	}
}
/*
 *	�폜�ςݍs���J�E���^���擾
 */
WORD CMeasurementTabView::MRCS_DeleteCountGet()
{
	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	�폜�ςݍs���J�E���^�����Z
 */
WORD CMeasurementTabView::MRCS_DeleteCountInc(WORD wCount/* = 1*/)
{
	WORD l_wCount = LOWORD(m_MRCS_dwSettings[0]);
	l_wCount += wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(l_wCount, HIWORD(m_MRCS_dwSettings[0]));

	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	�폜�ςݍs���J�E���^�����Z
 */
WORD CMeasurementTabView::MRCS_DeleteCountDec(WORD wCount/* = 1*/)
{
	WORD l_wCount = LOWORD(m_MRCS_dwSettings[0]);
	l_wCount -= wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(l_wCount, HIWORD(m_MRCS_dwSettings[0]));

	return LOWORD(m_MRCS_dwSettings[0]);
}
/*
 *	�w��s���J�E���^���擾
 */
WORD CMeasurementTabView::MRCS_IncludeCountGet()
{
	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	�w��s���J�E���^�����Z
 */
WORD CMeasurementTabView::MRCS_IncludeCountInc(WORD wCount/* = 1*/)
{
	WORD l_wCount = HIWORD(m_MRCS_dwSettings[0]);
	l_wCount += wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(LOWORD(m_MRCS_dwSettings[0]), l_wCount);

	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	�w��s���J�E���^�����Z
 */
WORD CMeasurementTabView::MRCS_IncludeCountDec(WORD wCount/* = 1*/)
{
	WORD l_wCount = HIWORD(m_MRCS_dwSettings[0]);
	l_wCount -= wCount;
	m_MRCS_dwSettings[0] = (DWORD) MAKELONG(LOWORD(m_MRCS_dwSettings[0]), l_wCount);

	return HIWORD(m_MRCS_dwSettings[0]);
}
/*
 *	�w��s��ǉ�
 */
WORD CMeasurementTabView::MRCS_IncludeAdd(long lIndex)
{
	WORD l_wRc = this->MRCS_IncludeCountGet();

	if (0 == this->MRCS_SettingGet(lIndex)) {
		/* �w�肪���������ꍇ�̂ݐݒ肵�܂��D	*/
		this->MRCS_SettingSet(lIndex, 1);
		l_wRc = this->MRCS_IncludeCountInc();
	}

	return l_wRc;
}
/*
 *	�đ��� No. �̎擾
 */
WORD CMeasurementTabView::MRCS_ReMeasureNoGet()
{
	return m_MRCS_wReMeasureNo;
}
/*
 *	�đ��� No. ���N���A
 */
void CMeasurementTabView::MRCS_ReMeasureNoClr()
{
	m_MRCS_wReMeasureNo = 0;
}
/*
 *	�w��s�F�ԍ��̐ݒ�
 */
void CMeasurementTabView::MRCS_IncludeColorSetNoSet(WORD wNo)
{
	m_MRCS_wIncludeColorSetNo = wNo;
}
/*
 *	�w��s�F�ԍ��̎擾
 */
WORD CMeasurementTabView::MRCS_IncludeColorSetNoGet()
{
	return m_MRCS_wIncludeColorSetNo;
}
/*
 *	�w��s�F�ݒ�
 */
#define	MRCSREDRAWMETHOD	1	/* �ĕ`����@	*/
void CMeasurementTabView::MRCS_IncludeColorSet(WORD wNo, BOOL bList, BOOL bGraph)
{
	if (0 == wNo) {
		m_MRCS_wIncludeColorSetNo = 0;
	}
	if ((FALSE == bList) && (FALSE == bGraph)) {
		m_MRCS_bIncludeColorSetList = FALSE;
		m_MRCS_bIncludeColorSetGraph = FALSE;
	}
	if ((0 == wNo) ||
		((FALSE == bList) && (FALSE == bGraph))) {
		return;
	}

	if (FALSE != bList) {
		m_MRCS_bIncludeColorSetList = TRUE;
	}
	if (FALSE != bGraph) {
		m_MRCS_bIncludeColorSetGraph = TRUE;
	}
	if ((FALSE != m_MRCS_bIncludeColorSetList) &&
		(FALSE != m_MRCS_bIncludeColorSetGraph)) {
		/* �����ݒ芮��	*/
		WORD l_wSetNo = m_MRCS_wIncludeColorSetNo;
		m_MRCS_bIncludeColorSetList = FALSE;
		m_MRCS_bIncludeColorSetGraph = FALSE;
		m_MRCS_wIncludeColorSetNo = 0;
		this->MRCS_GridLineColorSet(l_wSetNo, CYAN_COLOR);
		/*	�F�ݒ��̔��f���C�����ɂ���܂���ł����̂ŁC
			�ĕ`��(�H)���������Ă��܂��D	*/
		/* �O���b�h�\���X�V	*/
#if		MRCSREDRAWMETHOD == 1
		m_pMeasurementListDlg->m_MeasurementListGrid.Refresh();
		m_pMeasurementGraphDlg->m_MeasurementListGrid.Refresh();
#elif	MRCSREDRAWMETHOD == 2
		m_pMeasurementListDlg->m_MeasurementListGrid.RedrawRow(lIndex);
		m_pMeasurementGraphDlg->m_MeasurementListGrid.RedrawRow(lIndex);
#elif	MRCSREDRAWMETHOD == 3
		m_pMeasurementListDlg->m_MeasurementListGrid.EnsureVisible(lIndex, 1);
		m_pMeasurementGraphDlg->m_MeasurementListGrid.EnsureVisible(lIndex, 1);
#endif
	}
}
/*
 *	���ʃ��X�g�̕\���ς݃|�C���g�̋L���C�擾�C�N���A
 */
BOOL CMeasurementTabView::MRCS_ShowCompletionPointSnap(LPWORD pwList/* = 0*/, LPWORD pwGraph/* = 0*/)
{
	BOOL l_bRc = FALSE;

	if ((0 == pwList) && (0 == pwGraph)) {
		/* �N���A	*/
		m_MRCS_wShowCompletionPointSnapL = 0;
		m_MRCS_wShowCompletionPointSnapG = 0;
	} else
	if ((0 != pwList) && (0 != pwGraph)) {
		/* �L��	*/
		m_MRCS_wShowCompletionPointSnapL = m_pDoc->GetShowCompPointList();
		m_MRCS_wShowCompletionPointSnapG = m_pDoc->GetShowCompPointGraph();
		*pwList = m_MRCS_wShowCompletionPointSnapL;
		*pwGraph = m_MRCS_wShowCompletionPointSnapG;
		l_bRc = TRUE;
	} else
	if (0 != pwList) {
		/* CMeasurementListDlg ���擾	*/
		*pwList = m_MRCS_wShowCompletionPointSnapL;
		m_MRCS_wShowCompletionPointSnapL = 0;
		l_bRc = TRUE;
	} else
	if (0 != pwGraph) {
		/* CMeasurementGraphDlg ���擾	*/
		*pwGraph = m_MRCS_wShowCompletionPointSnapG;
		m_MRCS_wShowCompletionPointSnapG = 0;
		l_bRc = TRUE;
	}

	return l_bRc;
}
/* added 2016.05.05 hmenjo �蓮 �đ���/�w�蓝�v ---------- } ---------- */
