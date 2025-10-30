// TestMeasurementGantryDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MeasurementGraphDlg.h"
#include "MeasurementGraphDlg2.h"
#include "MainFrm.h"
#include "TestMeasurementGantryDlg.h"
#include "ExecutionDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TESTMEASDLG_ENABLE_CHANGE_TIMER 1

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementGantryDlg �_�C�A���O


CTestMeasurementGantryDlg::CTestMeasurementGantryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMeasurementGantryDlg::IDD, pParent)
{
	m_strLens = "";
	m_strOpticalFilter = "";
	m_dIntegTime = 0;
	m_bIsMoveAtSpeed = FALSE;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
}


void CTestMeasurementGantryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestMeasurementGantryDlg)
	DDX_Control(pDX, IDC_MEASUREMENT_TAB, m_MeasurementTab);
	DDX_Control(pDX, IDC_REFERENCE_BUTTON, m_ReferenceButton);
	DDX_Control(pDX, IDC_ONE_POINT_REFERENCE_BUTTON, m_OnePointReferenceButton);
	DDX_Control(pDX, IDC_REFERENCE_CANCEL_BUTTON, m_ReferenceCancelButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_CBStringExact(pDX, IDC_SR_LENS, m_strLens);
	DDX_CBStringExact(pDX, IDC_SR_OPT_FILTER, m_strOpticalFilter);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MEAS_INTEGRATION_TIME, m_Meas.ScanParams._SR.dIntegTime);
	DDX_Text(pDX, IDC_WAVELENGTH_START, (short&)m_Meas.ScanParams._SR.WavelenRange.wStart);
	DDX_Text(pDX, IDC_WAVELENGTH_END,	(short&)m_Meas.ScanParams._SR.WavelenRange.wEnd);
	DDX_Control(pDX, IDC_MEAS_POINT_X, m_stcMeasurementPointX);
	DDX_Control(pDX, IDC_MEAS_POINT_Y, m_stcMeasurementPointY);
}


BEGIN_MESSAGE_MAP(CTestMeasurementGantryDlg, CDialog)
	//{{AFX_MSG_MAP(CTestMeasurementGantryDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MEASUREMENT_TAB, OnSelchangeMeasurementTab)
	ON_BN_CLICKED(IDC_REFERENCE_BUTTON, OnReferenceButton)
	ON_BN_CLICKED(IDC_ONE_POINT_REFERENCE_BUTTON, OnOnePointReferenceButton)
	ON_BN_CLICKED(IDC_REFERENCE_CANCEL_BUTTON, OnReferenceCancelButton)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_WAVELENGTH_START, OnKillfocusWavelengthStart)
	ON_EN_KILLFOCUS(IDC_MEAS_INTEGRATION_TIME, OnKillfocusMeasIntegrationTime)
	ON_EN_KILLFOCUS(IDC_WAVELENGTH_END, OnKillfocusWavelengthEnd)
	ON_CBN_SETFOCUS(IDC_SR_LENS, OnSetfocusSrLens)
	ON_CBN_SETFOCUS(IDC_SR_OPT_FILTER, OnSetfocusSrOptFilter)
	ON_CBN_KILLFOCUS(IDC_SR_OPT_FILTER, OnKillfocusSrOptFilter)
	ON_EN_SETFOCUS(IDC_MEAS_INTEGRATION_TIME, OnSetfocusMeasIntegrationTime)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_START, OnSetfocusWavelengthStart)
	ON_EN_SETFOCUS(IDC_WAVELENGTH_END, OnSetfocusWavelengthEnd)
	ON_CBN_KILLFOCUS(IDC_SR_LENS, OnKillfocusSrLens)
	ON_CBN_SELCHANGE(IDC_COMBO_MEAS_POINT, OnSelchangeComboMeasPoint)
	ON_CBN_SELCHANGE(IDC_SR_LENS, OnSelchangeSrLens)
	ON_CBN_SELCHANGE(IDC_SR_OPT_FILTER, OnSelchangeSrOptFilter)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_TESTMODE_ONEPOINT_END, OnTestModeOnePointEnd)
	ON_MESSAGE(WM_TESTMODE_REF_END, OnTestModeRefEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestMeasurementGantryDlg ���b�Z�[�W �n���h��
BOOL CTestMeasurementGantryDlg::OnInitDialog()
{
	char szMeasPt[16];

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();

	//
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_pMainFrame->m_pTestMeasGantryDlgObj = this;
	m_iConditionFlg = TESTMODE_INIT;	//�e�X�g���[�h�N������
	m_iConditionFlgOld = -1;
	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iProcessStatusOld = -1;

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);
	ConfigFile_GetNanoSpecIni(m_SrTurret, CONFIG_FILE_SR_TURRET);
	LoadRecipeData();
	m_Meas.ScanParams._SR.dIntegTime = m_dIntegTime;
	m_Meas.ScanParams._SR.WavelenRange.wStart = m_wStart;
	m_Meas.ScanParams._SR.WavelenRange.wEnd = m_wEnd;

	CDialog::OnInitDialog();

//	m_strLens = m_SrTurret[m_Meas.ScanParams._SR.iLens].szName;

	///// Get Recipe Data /////
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);

	///// Control Initialize /////
	InitCombo_Lens();
	if((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G)){
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
	else{
		InitCombo_SrOpticalFilter();
	}

	/// Reference Button ///
	m_ReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceButton.DrawFlatFocus(TRUE);

	/// 1Point Reference Button ///
	m_OnePointReferenceButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OnePointReferenceButton.DrawFlatFocus(TRUE);

	/// Reference Cancel Button ///
	m_ReferenceCancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceCancelButton.DrawFlatFocus(TRUE);

	/// Cancel Button ///
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	/// OK Button ///
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT,BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE   ||
		rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE ||
		rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G )
	{
		SetDlgItemInt(IDC_WAVELENGTH_START, MIN_TARGET_WAVELENGTH); //�g��(START)
		if ( m_SrConfig.nHeadType == SR_HEAD_TYPE_LAH1024_SS )
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH);
		else
			SetDlgItemInt(IDC_WAVELENGTH_END, MAX_TARGET_WAVELENGTH_800);
	}
	GetDlgItem(IDC_WAVELENGTH_START)->EnableWindow(FALSE);	//�g��(START)
	GetDlgItem(IDC_WAVELENGTH_END)->EnableWindow(FALSE);	//�g��(END)


	///// Grid Control Initialize /////
	MeasurementTab_Init();

	//Lens Disable
	//2010.01.09 bagus --{--
	//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);
	GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);
	//2010.01.09 bagus --}--

	//Gantry Number Of Measure Point Show & Initialize
	GetDlgItem(IDC_COMBO_MEAS_POINT)->ShowWindow(SW_SHOW);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	pCombo->AddString(strSelectCaption);
	for(int iMeasPt =1; iMeasPt <= m_SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
		_itoa(iMeasPt, szMeasPt, 10);
		pCombo->AddString(szMeasPt);
	}
	pCombo->SetCurSel(0);

	m_stcMeasurementPointX.SetBkColor(REFERENCE_COLOR);
	m_stcMeasurementPointY.SetBkColor(REFERENCE_COLOR);

	GetDlgItem(IDC_MEAS_POINT_X)->SetWindowText("-----");
	GetDlgItem(IDC_MEAS_POINT_Y)->SetWindowText("-----");

	m_AfExecuteFlg = FALSE;
	m_AfExecuteFlgOld = FALSE;
	m_iAlarmStatus = ALARM_NON;
	m_iAlarmStatusOld = ALARM_NON;

	SetTimer(TESTMEASDLG_ENABLE_CHANGE_TIMER, 50, NULL);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnDestroy()
{
BOOL result;

	if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageStop();
		}
	}
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		if( !m_pMainFrame->HardwareSimulation() ){
			result = StageElevatorStop();
		}
	}

	if( m_pMeasurementGraphDlg1 != NULL ){
		delete m_pMeasurementGraphDlg1;
		m_pMeasurementGraphDlg1 = NULL;
	}
	if( m_pMeasurementGraphDlg2 != NULL ){
		delete m_pMeasurementGraphDlg2;
		m_pMeasurementGraphDlg2 = NULL;
	}

	m_pDoc->SetMeasMode(MEASMODE_NOT_MEASURE); //���蒆�łȂ�
	m_pDoc->InitMeasData(); //�������[������

	DummyRcpDelete(); //�_�~�[���V�s�폜

	//�X�e�[�^�X�o�[�����N���A
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(0, "");

// 2010.02.01 K.Matsuo ASSERT ������� -->
	m_pMainFrame->m_pTestMeasGantryDlgObj = NULL;
// 2010.02.01 K.Matsuo ASSERT ������� <--

	CDialog::OnDestroy();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnOK()
{
	CString strMsg;

	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
	}

	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlag�`�F�b�N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ���C���^�[���b�N������


	CString csBuff;

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_dIntegTime = atof(csBuff);

//	HookKeyUnhook();

	///// �w��t�H���_���݊m�F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ����f�[�^���t�H���_���c���Ă�����폜 /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}

	CDialog::OnOK();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnCancel()
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

//	HookKeyUnhook();

	///// �w��t�H���_���݊m�F /////
	char szDelDir[MAX_PATH];
	strcpy(szDelDir, g_szData_Measured_Data_Temp_Dir);
	BOOL bDirExist = m_pMainFrame->FileOrDirExists(szDelDir);

	///// ����f�[�^���t�H���_���c���Ă�����폜 /////
	if(bDirExist == TRUE){
		m_pMainFrame->DeleteDirectory(_T(szDelDir));
	}

	CDialog::OnCancel();
}

// =========================================================================
//
void CTestMeasurementGantryDlg::MeasurementTab_Init()
{
	///// Tab	/////
	CString strBuf;

	LoadStringML(IDS_CAPTION_RAW_DATA, strBuf, "Raw Data");
	m_MeasurementTab.InsertItem(0, strBuf); //Raw Data

	///// List	/////
	int dx = 5;
	int dy = 24;

	///// Graph(�ʏ�) /////
	m_pMeasurementGraphDlg1 = new CMeasurementGraphDlg2(GRAPHDLG_MODE_TEST_RAW);
	m_pMeasurementGraphDlg1->Create(CMeasurementGraphDlg2::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg1->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Graph(RAW) /////
	m_pMeasurementGraphDlg2 = new CMeasurementGraphDlg2(GRAPHDLG_MODE_TEST_RAW);
	m_pMeasurementGraphDlg2->Create(CMeasurementGraphDlg2::IDD, &m_MeasurementTab);
	m_pMeasurementGraphDlg2->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	///// Set Current Selection Item (Default Select Tab) ////
	int Index;
//	Index = m_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay;
	Index = 0;
	TabChange(Index);
	m_MeasurementTab.SetCurSel(Index);

}

// =========================================================================
//
void CTestMeasurementGantryDlg::TabChange(int Index)
{
	m_MeasurementTab.HighlightItem(0, FALSE);
//	m_MeasurementTab.HighlightItem(1, FALSE);
//	m_MeasurementTab.HighlightItem(Index, TRUE);
	m_MeasurementTab.HighlightItem(0, TRUE);

	///// Hide All Dialog /////
	m_pMeasurementGraphDlg1->ShowWindow(SW_HIDE);
	m_pMeasurementGraphDlg2->ShowWindow(SW_HIDE);

	///// Show Dialog /////
//	switch(Index){
//	case 0:
		m_pMeasurementGraphDlg1->ShowWindow(SW_SHOW);
//		break;
//	case 1:
//		m_pMeasurementGraphDlg2->ShowWindow(SW_SHOW);
//		break;
//	}
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index = m_MeasurementTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CTestMeasurementGantryDlg::LoadRecipeData()
{

	//���V�s�Ǎ���

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	RecipeFile_LoadRecipe(&m_MainRcpInfo, rcp_data.szRecipeName, RECIPE_FILE_MAIN_RECIPE); //
	RecipeFile_LoadRecipe(&m_Meas, m_MainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM);
}

// =========================================================================
//�����Y �R���{�C�j�V��������
//
void CTestMeasurementGantryDlg::InitCombo_Lens()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	pCombo->ResetContent();

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
		if ( m_SrTurret[i].bEnable ) {
			nIndex = pCombo->AddString(m_SrTurret[i].szName);
			pCombo->SetItemData(nIndex, i);
		}
	}

	nIndex = pCombo->FindStringExact(-1, m_strLens);
	pCombo->SetCurSel(nIndex);

	if ( nIndex == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nIndex);
}

// =========================================================================
// �����Y �R���{�I����
//
void CTestMeasurementGantryDlg::OnSelchangeSrLens()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	CString strMsg;

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}

	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return;
		}  // �X�e�[�W���쒆
	}
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ���C���^�[���b�N������

	UpdateData(TRUE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_LENS);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.iLens = pCombo->GetItemData(nSelect);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//�捞�ݏ���(Detector Condition)�ύX��

	UpdateData(FALSE);
}

// =========================================================================
//
void CTestMeasurementGantryDlg::InitCombo_SrOpticalFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);

	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	//�I�[�g�t�B���^����̏ꍇ
	if (m_SrConfig.bAutoFilter){
		pCombo->EnableWindow(FALSE);
		pCombo->ResetContent();

		SR_FILTER srFilter[SR_FILTER_MAX];
		ConfigFile_GetNanoSpecIni(srFilter, CONFIG_FILE_SR_FILTER);

		int nIndex;
		int iFilterIndex;
		for ( int iOptItem = 0; iOptItem < OPT_FILTER_MAX; iOptItem++ ) {
			switch ( iOptItem ) {
			case OPT_FILTER_UNKNOWN:
				break;
			case OPT_FILTER_OPEN:
				if(l_SystemConfig.nLanguage == 0){
					nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM_ENU[iOptItem]);
				}else{
					nIndex = pCombo->AddString(OPTICAL_FILTER_TYPE_ITEM_JPN[iOptItem]);
				}
				pCombo->SetItemData(nIndex, iOptItem);
				break;
			case OPT_FILTER_DARK:
				break;
			case OPT_FILTER_POS1:
			case OPT_FILTER_POS2:
			case OPT_FILTER_POS3:
				iFilterIndex = iOptItem - 3;
				if ( srFilter[iFilterIndex].bEnable ) {
					nIndex = pCombo->AddString(srFilter[iFilterIndex].szName);
					pCombo->SetItemData(nIndex, iOptItem);
				}
				break;
			default:
				break;
			}
		}

//		LPCTSTR pszFilterName[] = {OPTICAL_FILTER_TYPE_ITEM[0], OPTICAL_FILTER_TYPE_ITEM[1], OPTICAL_FILTER_TYPE_ITEM[2], srFilter[0].szName, srFilter[1].szName, srFilter[2].szName};

		nIndex = pCombo->FindStringExact(-1, m_strOpticalFilter);
		pCombo->SetCurSel(nIndex);
	}

	//�I�[�g�t�B���^�Ȃ��̏ꍇ
	else{
		m_Meas.ScanParams._SR.wOpticsFilterType = 0;
		GetDlgItem(IDC_SR_OPT_FILTER_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SR_OPT_FILTER)->ShowWindow(SW_HIDE);
	}
}

// =========================================================================
// �I�v�e�B�J���t�B���^�[ �R���{�I����
//
void CTestMeasurementGantryDlg::OnSelchangeSrOptFilter()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SR_OPT_FILTER);
	int nSelect = pCombo->GetCurSel();
	if ( nSelect == CB_ERR )
		return;
	m_Meas.ScanParams._SR.wOpticsFilterType = static_cast<long>(pCombo->GetItemData(nSelect));

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_AFTER;	//�捞�ݏ���(Detector Condition)�ύX��

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnReferenceButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	CString strMsg, strTitle;

	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //�����e�i���X�E�X�C�b�`�E�I��
	}

	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlag�`�F�b�N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ���C���^�[���b�N������


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		// �K��JoyStick�̏�Ԃ�߂�����
//		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	CString strBuffer;

	m_pDoc->GetRcpData(&rcp_data);

	///// Clear Reference /////
	ClearReference(rcp_data.szRecipeName);


	//�����Y�ݒ�
	UINT uiLens = m_Meas.ScanParams._SR.iLens;
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(uiLens);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	//�����W���[�����g�v���O�������C��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
	{
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_REFLECTANCE);
			break;
		case MEAS_PROG_TYPE_SR_THICKNESS:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_THICKNESS);
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
			break;
		default:
			break;
	}


	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		// �K��JoyStick�̏�Ԃ�߂�����
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, 0);

	///// �`�[�t�̌��� /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
		// �K��JoyStick�̏�Ԃ�߂�����
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	CExecutionDlg dlg;
//	dlg.SetCaption(EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	dlg.SetCaption(strBuffer);
	LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
	dlg.SetMessage(strBuffer);
	dlg.DoModal();

	m_iConditionFlg = TESTMODE_REF_AFTER;	//���t�@�����X�����(��̃��[�_���̃_�C�A���O�{�b�N�X�����t�@�����X����I���܂Ŕ����Ȃ�����)

	return;

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnOnePointReferenceButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	CString strMsg, strTitle;

	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
		if( nexioIsMaintenanceSwitch() != ON ) {AlarmIf_Set(ALID_MaintenanceSwitchOn); return;} //�����e�i���X�E�X�C�b�`�E�I��
	}

	// �eI/O�`�F�b�N
	if(m_pDoc->IsInterLock() == TRUE){return;}
	if( nexioIsMovo2Alarm() 		!= OFF) {AlarmIf_Set(ALID_StageError); return;} // �X�e�[�W�G���[
	if(m_pDoc->CheckUnitStatus() == TRUE){return;}

	//ActuateFlag�`�F�b�N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && m_pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return;
	} //�X�e�[�W�����쒆�ł�
	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	// ���C���^�[���b�N������


	if(CheckData() == FALSE) return;

	if( !m_pDoc->JoyStickChangeDisable() ){
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		// �K��JoyStick�̏�Ԃ�߂�����
//		m_pDoc->JoyStickStatusRestore();
		return;
	}


	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	CString strBuffer;

	m_pDoc->GetRcpData(&rcp_data);

	///// Clear Reference /////
	ClearReference(rcp_data.szRecipeName);


	//�����Y�ݒ�
	UINT uiLens = m_Meas.ScanParams._SR.iLens;
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
	StageSelectLensNotify(uiLens);
	m_pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);

	//�����W���[�����g�v���O�������C��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
	{
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_REFLECTANCE);
			break;
		case MEAS_PROG_TYPE_SR_THICKNESS:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_THICKNESS);
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
			break;
		default:
			break;
	}


	///// Start Reference /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_ReferenceAcquisitionFailed);
		// �K��JoyStick�̏�Ԃ�߂�����
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

//	���t�@�����X�|�W�V�������擾����
	int nRefPos;

	nRefPos = ((CComboBox *)GetDlgItem(IDC_COMBO_MEAS_POINT))->GetCurSel();
	if(nRefPos <= 0){
		//���t�@�����X�|�W�V�������I������Ă��Ȃ�
		LoadStringML(IDS_WARNING_NO_REF_POS, strMsg, "Please Select Reference Position.");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);
		return;
	}

	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_SRREF, (WPARAM)rcp_data.szRecipeName, nRefPos);

	///// �`�[�t�̌��� /////
	LoadStringML(IDS_TESTMODE_REF_SCAN_START, strBuffer, "Measurement Start Test Mode Reference");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, strBuffer);

	if(bResult == FALSE){
		// �K��JoyStick�̏�Ԃ�߂�����
//		m_pDoc->JoyStickStatusRestore();
		return;
	}

	m_pDoc->OperationLogging("AUTO FOCUS Button was push.");

	m_pDoc->SetProcessStatus(PROCESS_PROC);

	CExecutionDlg dlg;
//	dlg.SetCaption(EXECUTION_CAPTION);
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	dlg.SetCaption(strBuffer);
	LoadStringML(IDS_MESUREMENT_REF, strBuffer, "Reference Measurement...");
	dlg.SetMessage(strBuffer);
	dlg.DoModal();

	m_iConditionFlg = TESTMODE_REF_AFTER;	//���t�@�����X�����(��̃��[�_���̃_�C�A���O�{�b�N�X�����t�@�����X����I���܂Ŕ����Ȃ�����)

	return;

}

// ==========================================================================
//
void CTestMeasurementGantryDlg::OnReferenceCancelButton()
{
	//���蒆�̂݃L�����Z�������s����
//	if(m_pDoc->GetProcessStatus() == PROCESS_PROC){
		AfxGetMainWnd()->PostMessage(WM_DO_CANCEL, 0, 0);
	//}
}

// ==========================================================================
//
void CTestMeasurementGantryDlg::StartTestModeMeas()
{

	RCP_DATA rcp_data;
	int ChiefRet = 0;
	BOOL bResult = FALSE;
	// Kojika 20090529 Change
	CString l_strBuffer, l_strTitle;
	//char* pszCaption = "�e�X�g���[�h����J�n";
	// Kojika 20090529 Change End

	m_pDoc->GetRcpData(&rcp_data);


	//���萔����
	if(m_pDoc->GetStoreCount() >= TESTDATA_POINT_MAX)
	{
		char szMsgBuff[255+1];
		memset(szMsgBuff, 0, sizeof(szMsgBuff));
		// Kojika 20090529 Change
		//sprintf(szMsgBuff, "����f�[�^�� %d �𒴂��Ă��܂�!", TESTDATA_POINT_MAX);
		LoadStringML(IDS_MEASURE_DATA_NUMBER_EXCEED, l_strBuffer, "Measurement data of number %d is exceeded.");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		sprintf(szMsgBuff, l_strBuffer, TESTDATA_POINT_MAX);
		MessageBox(szMsgBuff, l_strTitle, MB_OK);
		// Kojika 20090529 Change End
		return;
	}


	//�����W���[�����g�v���O�������C��
	switch(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType)
	{
		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_REFLECTANCE);
			break;
		case MEAS_PROG_TYPE_SR_THICKNESS:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_THICKNESS);
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			MakeTempSrMeasurementRecipe(m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEAS_PROG_TYPE_SR_TRANSMITTANCE_G);
			break;
		default:
			break;
	}


	///// �`�[�t�֑���J�n���b�Z�[�W�𑗐M /////
	HWND l_hChiefDlg = ::FindWindow(0, CHIEF_DLG_CAPTION);
	if(l_hChiefDlg == NULL){
		AlarmIf_Set(ALID_MeasurementFailed_TestMode);
		return;
	}

#if 1
// ADD 2009.06.02 H.Kawamura
// �ꎞ�I�Ƀn�[�hJoyStick���E��
	if( !m_pDoc->JoyStickChangeDisable() ){
		CString strMsg, strTitle;
		LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
		LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
		MessageBox(strMsg, strTitle, MB_OK);

		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();

		return;
	}
// ADD 2009.06.02 H.Kawamura
#endif

/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- { ---------- */
	STAGE_COORD_XYZ l_CurrPosXYZ;
	NS_StageGetPos(&l_CurrPosXYZ);
	m_pDoc->m_lZ_AF = l_CurrPosXYZ.lZ;
/* added 2009.12.02 hmenjo ���� Seq �͎w��w�b�h�œ���(���) ---------- } ---------- */
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ---------- { ---------- */
//	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM)rcp_data.szRecipeName, 0); //1�|�C���g����
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ----------			   */
	ChiefRet = ::SendMessage(l_hChiefDlg, WM_DISP_START_1POINT, (WPARAM) rcp_data.szRecipeName, MAKELPARAM(1, 0)); //1�|�C���g����
/* modified 2009.11.27 hmenjo �P�|�C���g����ő���|�C���g�ԍ� ---------- } ---------- */

	///// �`�[�t�̌��� /////
	// Kojika 20090529 Change
	LoadStringML(IDS_TESTMODE_MEASURE_START, l_strBuffer, "Test Mode Measurement Start");
	bResult = m_pMainFrame->JudgeChiefResult(ChiefRet, l_strBuffer);
	// Kojika 20090529 Change End

	if(bResult == FALSE){
		m_pDoc->OperationLogging("Measurement Operation - TEST MODE MEASUREMENT BUTTON - ABORT : Chief Result is Measurement Cancel.");

#if 1
// ADD 2009.06.02 H.Kawamura
		// �K��JoyStick�̏�Ԃ�߂�����
		m_pDoc->JoyStickStatusRestore();
// ADD 2009.06.02 H.Kawamura
#endif

		return;
	}

	m_pDoc->SetProcessStatus(PROCESS_PROC);

#if 0
// DEL 2009.06.02 H.Kawamura
// �ꎞ�I�Ƀn�[�hJoyStick���E�������ɒu��������
	// H/W�L���Ȃ�
	if( m_pMainFrame->GetJoyStickMode() != 0 ){
		BOOL bJoyStk= m_pDoc->ChangeJoyStick(FALSE); //�W���C�X�e�B�b�N���g�p�s�ɐݒ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		TRACE("CTestMeasurementGantryDlg::OnMeasurementButton()�֐��� ChangeJoyStick()�̖߂�l:%d\n", bJoyStk);
	}
// DEL 2009.06.02 H.Kawamura
#endif

	m_iConditionFlg = TESTMODE_MEAS_AFTER; //�����
}

// ==========================================================================
//
void CTestMeasurementGantryDlg::ClearReference(char* szRecipeName)
{
	char szFilePath[MAX_PATH];
	memset(szFilePath, 0, sizeof(szFilePath));

	sprintf(szFilePath, "%s%s%s", g_szData_Ref_Dir, szRecipeName, DAT_EXT);
	DeleteFile(szFilePath);
}



// ==========================================================================
//
LRESULT CTestMeasurementGantryDlg::OnTestModeOnePointEnd(WPARAM wParam, LPARAM lParam)
{
//�e�X�g���[�h1�|�C���g����I��������

	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);

	return 0L;
}

// ==========================================================================
//
LRESULT CTestMeasurementGantryDlg::OnTestModeRefEnd(WPARAM wParam, LPARAM lParam)
{
//�e�X�g���[�h1�|�C���g����I��������
	//2010.01.15 bagus --{--
	//�e�X�g���[�h�Ń_�[�N���t�@�����X�̊m��^�C�~���O�����̃^�C�~���O�Ȃ̂ł����ł�����x�X�V������
	RCP_DATA rcp_data;
	m_pDoc->GetRcpData(&rcp_data);

	if(rcp_data.MainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_SR
	&& rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G
	&& m_pDoc->GetMeasMode() == MEASMODE_TEST){
		m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
		m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_ONEPOINT_END, wParam, lParam);
	}
	//2010.01.15 bagus --}--
	m_pMeasurementGraphDlg1->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);
	m_pMeasurementGraphDlg2->PostMessage(WM_TESTMODE_REF_END, wParam, lParam);

	Invalidate();
	return 0L;
}


// =========================================================================
//�_�~�[���W���[�����g�v���O�������㏑��
BOOL CTestMeasurementGantryDlg::MakeTempSrMeasurementRecipe(LPCTSTR pszRecipeName, int iRecipeTipe)
{
	switch(iRecipeTipe)
	{
		case MEAS_PROG_TYPE_SR_THICKNESS:
			if ( !RecipeFile_SaveRecipe(&m_Meas, pszRecipeName, RECIPE_FILE_SR_THICKNESS) )
			{
				return FALSE;
			}
			break;

		case MEAS_PROG_TYPE_SR_REFLECTANCE:
			if ( !RecipeFile_SaveRecipe(&m_Meas, pszRecipeName, RECIPE_FILE_SR_REFLECTANCE) )
			{
				return FALSE;
			}
			break;
		case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
			if ( !RecipeFile_SaveRecipe(&m_Meas, pszRecipeName, RECIPE_FILE_SR_TRANSMITTANCE) )
			{
				return FALSE;
			}
			break;

		default:
				return FALSE;
			break;
	}
	return TRUE;
}

// =========================================================================
//�_�~�[���V�s�폜
BOOL CTestMeasurementGantryDlg::DummyRcpDelete()
{
	char szDeleteRcpPathMain[255 + 1];
	char szDeleteRcpPathMeas[255 + 1];
	char szDeleteRcpPathRef[255 + 1];
	char szDeleteRcpPathStage[255 + 1];
	memset(szDeleteRcpPathMain, 0, sizeof(szDeleteRcpPathMain));
	memset(szDeleteRcpPathMeas, 0, sizeof(szDeleteRcpPathMeas));
	memset(szDeleteRcpPathRef, 0, sizeof(szDeleteRcpPathRef));
	memset(szDeleteRcpPathStage, 0, sizeof(szDeleteRcpPathStage));

	sprintf(szDeleteRcpPathMain, "%s%s%s",
			g_szDb_Main_Recipe_Dir, m_MainRcpInfo.hdr.szName, MAINRECIPE_EXT);
	sprintf(szDeleteRcpPathMeas, "%s%s%s",
			g_szDb_Measurement_Program_Dir, m_MainRcpInfo.MainRcpParam.hdr.szMeas, MEASUREMENTPGM_EXT);
	sprintf(szDeleteRcpPathRef, "%s%s%s",
		g_szData_Ref_Dir, m_MainRcpInfo.hdr.szName, DAT_EXT);
	sprintf(szDeleteRcpPathStage, "%s%s%s",
			g_szDb_Stage_Program_Dir, m_MainRcpInfo.MainRcpParam.hdr.szStage, STAGEPGM_EXT);


	BOOL bMainDel = DeleteFile( szDeleteRcpPathMain );
	BOOL bMeasDel = DeleteFile( szDeleteRcpPathMeas );
	BOOL bRefDel = DeleteFile( szDeleteRcpPathRef );
	BOOL bStageDel = DeleteFile( szDeleteRcpPathStage );


	if((bMainDel == FALSE)||(bMeasDel == FALSE)||(bRefDel == FALSE))
	{
		return FALSE;
	}

	return TRUE;
}


void CTestMeasurementGantryDlg::OnTimer(UINT nIDEvent)
{
BOOL bJoyStk;

	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);


	m_iProcessStatus = m_pDoc->GetProcessStatus();
	m_iAlarmStatus = m_pDoc->GetAlarmStatus();

// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ -->
//	JoyStickEnableCheck();
// 2009.06.05 K.Matsuo �h�A�C���^�[���b�N���o����JoyStick������ <--
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->
	if ( m_pDoc->Rap_IsDoorInterlock() != ON ) {		/* �h�A�C���^�[���b�N			���g */
		if (0 != m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)) {
			/* XY �X�e�[�W���쒆�ł��D	*/
			StageStop();
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);	/* ���쒆�t���O(XY �X�e�[�W)���I�t	*/
		}
	}
// 2009.08.04 K.Matsuo PKI LBUTTONDOWN DoorOpen -->

	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld))
	{
//================
		//�n�[�h�W���C�X�e�B�b�N���쐧��(�A���[�����̂�)
		if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_EXIST))
		{
			// H/W�L���Ȃ�
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
				m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
			}
		}
		else if((m_iAlarmStatus != m_iAlarmStatusOld)&&(m_iAlarmStatus != ALARM_NON))
		{
			// H/W�L���Ȃ�
			if( m_pMainFrame->GetJoyStickMode() != 0 ){
				bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
				if( bJoyStk == TRUE ){
					m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
				}
			}
		}

//================
		//�R���g���[����Enable����(�P�ƃI�[�g�t�H�[�J�X���쎞�ȊO)
		switch(nIDEvent)
		{
			case  TESTMEASDLG_ENABLE_CHANGE_TIMER:

				if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_INIT)||(m_iConditionFlg == TESTMODE_CONDITION_CANGE_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //�wWAIT�x ���� (�w�e�X�g���[�h�N������x �܂��� �w�捞�ݏ���(Detector Condition)�ύX��x) ���� �w�A���[���Ȃ��x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//�����Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//���t�@�����X
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(TRUE);		//1Point���t�@�����X
					GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(TRUE);		//���t�@�����XCancel

					//�I��
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]�{�^��
				}
				else if(((m_iProcessStatus == PROCESS_WAIT) && ((m_iConditionFlg == TESTMODE_REF_AFTER)||(m_iConditionFlg == TESTMODE_MEAS_AFTER)) )
					&&(m_iAlarmStatus == ALARM_NON))
				{ //�wWAIT�x ���� (�w���t�@�����X�����x �܂��� �w�����x) ���� �w�A���[���Ȃ��x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//�����Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(TRUE);		//���t�@�����X
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(TRUE);		//1Point���t�@�����X
					GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(TRUE);		//���t�@�����XCancel

					//�I��
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]�{�^��
				}
				else if((m_iProcessStatus == PROCESS_DOWN) || (m_iAlarmStatus != ALARM_NON))
				{ //�wDOWN�x �܂��� �w�A���[���������x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				  //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(TRUE);				//�����Y
					}
					//Saiki 20090908 Change <-----
					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(TRUE);			//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(TRUE);	//���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//���t�@�����X
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(FALSE);		//1Point���t�@�����X
					//2010.01.13 bagus GTR --{--
					//GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(FALSE);		//���t�@�����XCancel
					//2010.01.13 bagus GTR --}--

					//�I��
					GetDlgItem(IDOK)->EnableWindow(TRUE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(TRUE);					//[CANCEL]�{�^��
				}
				else if((m_iProcessStatus == PROCESS_INIT)
					||(m_iProcessStatus == PROCESS_PROC)
					||(m_iProcessStatus == PROCESS_ABRT)
					||(m_iProcessStatus == PROCESS_COMP))
				{ //�wINIT�x�܂��́wPROC�x�܂��́wABRT�x�܂��́wCOMP�x
					//�捞�ݏ���
					//Saiki 20090908 Change ----->
					//GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE); 			   //�����Y
					if(!((rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE) || (rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_TRANSMITTANCE_G))){
						GetDlgItem(IDC_SR_LENS)->EnableWindow(FALSE);				 //�����Y
					}
					//Saiki 20090908 Change <-----

					if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
					{
						GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(FALSE); 		//���w�t�B���^�[
					}
					GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(FALSE); //���莞��

					//����n
					GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(FALSE);		//���t�@�����X
					GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(FALSE);		//1Point���t�@�����X
					//2010.01.13 bagus GTR --{--
					//GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(FALSE);		//���t�@�����XCancel
					//2010.01.13 bagus GTR --}--

					//�I��
					GetDlgItem(IDOK)->EnableWindow(FALSE);						//[OK]�{�^��
					GetDlgItem(IDCANCEL)->EnableWindow(FALSE);					//[CANCEL]�{�^��
				}
				else
				{
					//�R�R�ɂ͂��Ȃ��͂�
				}

				break;
		}
	}

//================

	//�R���g���[����Enable����(�P�ƃI�[�g�t�H�[�J�X���쎞)
	if(m_AfExecuteFlg != m_AfExecuteFlgOld)
	{
		//�捞�ݏ���
		GetDlgItem(IDC_SR_LENS)->EnableWindow(!m_AfExecuteFlg); 				//�����Y
		if( rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_REFLECTANCE )
		{
			GetDlgItem(IDC_SR_OPT_FILTER)->EnableWindow(!m_AfExecuteFlg);		//���w�t�B���^�[
		}
		GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->EnableWindow(!m_AfExecuteFlg);	//���莞��

		//����n
		GetDlgItem(IDC_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//���t�@�����X
		GetDlgItem(IDC_ONE_POINT_REFERENCE_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//1Point���t�@�����X
		GetDlgItem(IDC_REFERENCE_CANCEL_BUTTON)->EnableWindow(!m_AfExecuteFlg);		//���t�@�����XCancel

		//�I��
		GetDlgItem(IDOK)->EnableWindow(!m_AfExecuteFlg);						//[OK]�{�^��
		GetDlgItem(IDCANCEL)->EnableWindow(!m_AfExecuteFlg);					//[CANCEL]�{�^��
	}


#if 0 //ProcessStatus��ActuateFlags�𕹗p
//�捞�ݏ����ҏW���A�E�F�C�g�ȊO�A�A���[���������AAF���s�� �̓{�^�������ɂ���B
	//�\�t�g�W���C�X�e�B�b�N����
	if((m_iConditionFlg != m_iConditionFlgOld)||(m_iProcessStatus != m_iProcessStatusOld)||(m_iAlarmStatus != m_iAlarmStatusOld)||(m_AfExecuteFlg != m_AfExecuteFlgOld))
	{
		if((m_iConditionFlg == TESTMODE_CONDITION_CANGE_EDIT) || ( m_iProcessStatus != PROCESS_WAIT ) || (m_iAlarmStatus == ALARM_EXIST) || (m_AfExecuteFlg == TRUE))
		{
			SoftJoyStickXyzEnable(FALSE); //����֎~

			//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(FALSE);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(FALSE);
		}
		else
		{
			SoftJoyStickXyzEnable(TRUE); //���싖��

			//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
			GetDlgItem(IDC_JOYSTICK1)->EnableWindow(TRUE);
			GetDlgItem(IDC_JOYSTICK2)->EnableWindow(TRUE);
		}
	}
#endif

//================
	//�O��l�X�V
	m_iConditionFlgOld = m_iConditionFlg;
	m_iProcessStatusOld = m_iProcessStatus;
	m_iAlarmStatusOld = m_iAlarmStatus;
	m_AfExecuteFlgOld = m_AfExecuteFlg;

//================

	CDialog::OnTimer(nIDEvent);
}



void CTestMeasurementGantryDlg::OnSetfocusSrLens()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementGantryDlg::OnKillfocusSrLens()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//�捞�ݏ���(Detector Condition)�ύX��
}

void CTestMeasurementGantryDlg::OnSetfocusSrOptFilter()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementGantryDlg::OnKillfocusSrOptFilter()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//�捞�ݏ���(Detector Condition)�ύX��
}

void CTestMeasurementGantryDlg::OnSetfocusMeasIntegrationTime()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementGantryDlg::OnKillfocusMeasIntegrationTime()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));
	m_pDoc->GetRcpData(&rcp_data);

	GetDlgItemText(IDC_MEAS_INTEGRATION_TIME, csBuff);
	m_Meas.ScanParams._SR.dIntegTime = atof(csBuff);

	m_pDoc->SetRcpData(&rcp_data);

	m_iConditionFlg = TESTMODE_CONDITION_CANGE/*_Ed*/;	//�捞�ݏ���(Detector Condition)�ύX��

}

void CTestMeasurementGantryDlg::OnSetfocusWavelengthStart()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementGantryDlg::OnKillfocusWavelengthStart()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
	{
		m_pDoc->GetRcpData(&rcp_data);
		GetDlgItemText(IDC_WAVELENGTH_START, csBuff);
		rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wStart = m_Meas.ScanParams._SR.WavelenRange.wStart = atoi(csBuff);
		m_pDoc->SetRcpData(&rcp_data);
	}
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //�捞�ݏ���(Detector Condition)�ύX��

}

void CTestMeasurementGantryDlg::OnSetfocusWavelengthEnd()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	m_iConditionFlg = TESTMODE_CONDITION_CANGE_EDIT;	//�捞�ݏ���(Detector Condition)�ҏW��
}

void CTestMeasurementGantryDlg::OnKillfocusWavelengthEnd()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString csBuff;
	RCP_DATA rcp_data;
	memset(&rcp_data, 0, sizeof(rcp_data));

	if(rcp_data.MeasProgInfo.ScanParams.hdr.wScanType == MEAS_PROG_TYPE_SR_THICKNESS)
	{
		m_pDoc->GetRcpData(&rcp_data);
		GetDlgItemText(IDC_WAVELENGTH_END, csBuff);
		rcp_data.MeasProgInfo.ScanParams._SR.WavelenRange.wEnd = m_Meas.ScanParams._SR.WavelenRange.wEnd = atoi(csBuff);
		m_pDoc->SetRcpData(&rcp_data);
	}
	m_iConditionFlg = TESTMODE_CONDITION_CANGE; //�捞�ݏ���(Detector Condition)�ύX��
}

// =========================================================================
//
BOOL CTestMeasurementGantryDlg::CheckData()
{
	CString strBuffer;

	// Kojika 20090529 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090529 Add End

	if(UpdateData(TRUE) == 0) return FALSE;

	///// Integration Time /////
	if ( m_Meas.ScanParams._SR.dIntegTime < MIN_INTEGRATION_TIME || MAX_INTEGRATION_TIME < m_Meas.ScanParams._SR.dIntegTime )
	{
		// Kojika 20090529 Change
//		strBuffer.Format("Integration Time is out of range.\n(%.3f - %.3f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		//strBuffer.Format("���莞�Ԃ��͈͊O�ɐݒ肳��Ă��܂�\n(%.3f - %.3f)", MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		//MessageBox(strBuffer, "NanoSpec", MB_OK | MB_ICONSTOP);
		LoadStringML(IDS_INTEGRA_TIME_OUT_RANGE, l_strBuffer, "Integration Time is out of range.\n(%.3f - %.3f)");
		LoadStringML(IDS_TITLE_NANOSPEC, l_strTitle, "NanoSpec");
		strBuffer.Format(l_strBuffer, MIN_INTEGRATION_TIME, MAX_INTEGRATION_TIME);
		MessageBox(strBuffer, l_strTitle, MB_OK | MB_ICONSTOP);
		// Kojika 20090529 Change End
		GetDlgItem(IDC_MEAS_INTEGRATION_TIME)->SetFocus();
		return FALSE;
	}
	return TRUE;
}

// =========================================================================
//
void CTestMeasurementGantryDlg::OnSelchangeComboMeasPoint()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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

	//���t�@�����X�|�W�V�����̃��[�h
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	iSelectMeasPt = pCombo->GetCurSel();
	if(0 < iSelectMeasPt && iSelectMeasPt <= m_SrConfig.wGantryNumOfMeasPoint){
		//XY���W�̕\����ύX
		SetPointData(SrReferencePosition, iSelectMeasPt);
		//�J�[�\���ԍ��ɑΉ������X�e�[�W�ʒu�ֈړ�
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
		sampleCoord.lX = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX/* + m_StageProgInfoHdr.SampleInfo.Origin.lX*/;
		sampleCoord.lY = SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY/* + m_StageProgInfoHdr.SampleInfo.Origin.lY*/;
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		//�{�^�����̖�����
	}
}
// ==========================================================================
// Name�F		SetPointData
void CTestMeasurementGantryDlg::SetPointData(SR_REFERENCE_POSITION SrReferencePosition, int iSelectMeasPt)
{
	CString csPointNo, csX, csY;

	// X
	csX.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lX) / MICROMETRE);
	// Y
	csY.Format("%.3lf", (double)(SrReferencePosition.LocGantryTransmitReference[iSelectMeasPt - 1].lY) / MICROMETRE);

	SetDlgItemText(IDC_MEAS_POINT_X, csX);
	SetDlgItemText(IDC_MEAS_POINT_Y, csY);
}
