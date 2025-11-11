// AdditionalMeasurementDlg.cpp : �C���v�������e�[�V���� �t�@�C��
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
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
#include "ChiefView.h"
#include "ChifTransiMaster.h"
extern CChiefView*	g_pcChiefView;	/* static �p�N���X�|�C���^	*/
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */


#define ADDIMEAS_ENABLE_CHANGE_TIMER	1


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdditionalMeasurementDlg �_�C�A���O


CAdditionalMeasurementDlg::CAdditionalMeasurementDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CAdditionalMeasurementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiPositionSettingDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT

// 2009.10.30 bagus Stage-Sample �C�� --{--
	::ZeroMemory(m_szRecipeName, sizeof(m_szRecipeName));
	m_nSampleMode = 0;
// 2009.10.30 bagus Stage-Sample �C�� --}--
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
// CAdditionalMeasurementDlg ���b�Z�[�W �n���h��
//																		   //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAdditionalMeasurementDlg::OnInitDialog()
{
// 2009.10.30 bagus Stage-Sample �C�� --{--
	//Sample�Z�b�g
	if (m_nSampleMode == 1) {
		SetFileName(m_szRecipeName);
	}
	SetSampleMode(m_nSampleMode);
// 2009.10.30 bagus Stage-Sample �C�� --}--
	SYSTEM_CONFIG SystemConfig;
	LPCTSTR pszHeader[LANGUAGE_MAX][2] = {
		{_T("Point No"),_T("Position(mm)")},
		{_T("�|�C���gNo"),_T("�ʒu(mm)")},
	};
	ConfigFile_GetNanoSpecIni(&SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

//	  PUSHBUTTON	  "MEASUREMENT",IDC_MEASUREMENT_BUTTON,306,366,85,28

//	CreateDialogItem("BUTTON", "MEASUREMENT",	BS_PUSHBUTTON,	CRect(CPoint(306,366), CSize(85,28)),	IDC_MEASUREMENT_BUTTON);
	//Saiki 20090526 Change ----->
	//CreateDialogItem("BUTTON", "����",  BS_PUSHBUTTON,  CRect(CPoint(306,366), CSize(85,28)),   IDC_MEASUREMENT_BUTTON);
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

	//2009.12.11 bagus �C�� --{--
	CString strSelectCaption;
	LoadStringML(IDS_MS_COMBO_SELECT_CAPTION,strSelectCaption,"<select>");
	//2009.12.11 bagus �C�� --{--
	if(m_bGTR){
		SR_CONFIG SrConfig;
		char szMeasPt[10+1];
		CString str;
		CSize sz;
		ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
		//pCombo->SetExtendedUI();
		//2009.12.11 bagus �C�� --{--
		//pCombo->AddString("<select>");
		pCombo->AddString(strSelectCaption);
		//2009.12.11 bagus �C�� --}--
// 2010.01.29 bagus Gantry --{--
#if 0
		for(int iMeasPt =1; iMeasPt <= SrConfig.wGantryNumOfMeasPoint; iMeasPt++){
			itoa(iMeasPt, szMeasPt, 10);
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

		strcpy_s(szRecipeName, sizeof(szRecipeName), rcp_data.MainRcpInfo.MainRcpParam.hdr.szStage);

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
					itoa(iMeasPt, szMeasPt, 10);
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
		//�R���{�{�b�N�X�𓯊�����
		CComboBox *pComboBase;
		CComboBox* pCombo2;
		pCombo2 = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
		pComboBase = (CComboBox *)(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_COMBO_MEAS_POINT);
		if(pComboBase && pCombo2){
			int iSelectMeasPt = pComboBase->GetCurSel();
			if(iSelectMeasPt != pCombo2->GetCurSel()){
				pCombo2->SetCurSel(iSelectMeasPt);
			}
			//���t�@�����X�|�W�V�����̃��[�h
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

#if 0 // CANCEL�{�^���𕜊�������
	// Cancel�{�^����\�� �� Ok�{�^���ړ�
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	CRect rect;
	GetDlgItem(IDCANCEL)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDOK)->MoveWindow(rect);
#endif

// 2009.10.29 bagus Jog �ǉ��C�� --{--

	if (SystemConfig.nJoystickType == 0) {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_HIDE);
	} else {
		GetDlgItem(IDC_JOYSTICK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JOYSTICK2)->ShowWindow(SW_SHOW);
	}
// 2009.10.29 bagus Jog �ǉ��C�� --}--

	m_iProcessStatusOld = -1;
	m_iManuMeasModeOld = -1;

	SetTimer(ADDIMEAS_ENABLE_CHANGE_TIMER, 50, NULL);


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/////////////////////////////////////////////////////////////////////////////
//
void CAdditionalMeasurementDlg::OnOK()
{
	COverlayDialog::OnOK();

	// ���݂̍��W�������n��
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
		case ADDIMEAS_ENABLE_CHANGE_TIMER:		// 50ms ������^�C�}
			iProcessStatus = m_pDoc->GetProcessStatus();
			if(iProcessStatus != m_iProcessStatusOld)
			{

				if(iProcessStatus == PROCESS_DOWN) //�_�E������̕����͑z��O
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
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
			this->ButtonEnableCheck();
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
			break;
	}



	int iManuMeasMode = m_pDoc->GetManuMeasMode();

	//[���芮��]�{�^���������̃{�^����������(�}�j���A�����莞�̂�)
	if((m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)&&(iManuMeasMode != m_iManuMeasModeOld))
	{
		if(iManuMeasMode == MANUMODE_MEAS_COMP) //[���芮��]������
		{
			ButtonEnableChange(-1); //-1��switch���ɂ�����Ȃ�����
		}
	}
	//�O��l�X�V
	m_iManuMeasModeOld = iManuMeasMode;



	COverlayDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
//
void CAdditionalMeasurementDlg::OnMeasurementButton()
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

/* added 2009.11.27 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
	this->m_bProgress1PMeas = TRUE;
/* added 2009.11.27 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */

	COverlayDialog::SetOperationLog("MEASUREMENT Button was push.");

	// �����ɒǉ�����̏���������
	//2009.11.30 bagus --{--
	//�X�e�[�W�ړ���
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE)){
		return;
	}
	//2009.11.30 bagus --}--

	//�}�j���A�����胂�[�h�ݒ�
	m_pDoc->SetManuMeasMode(MANUMODE_ON_MEASURE); //���蒆�ɐݒ�


///ButtonEnableChange(MEASUREMENT_MANUALMEASURE_S);



/* modified 2009.11.27 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//	m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas();
/* modified 2009.11.27 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
	if (TRUE != m_pMainFrame->m_pManuMeaFormViewObj->ManuMeas()) {
		this->m_bProgress1PMeas = FALSE;
	}
/* modified 2009.11.27 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
}

// ==========================================================================
//���蒆�̑���I�� ���̃{�^���L��/�����̐ؑւ�
void CAdditionalMeasurementDlg::ButtonEnableChange(int Select)
{

	if((m_pDoc->GetMeasMode() == MEASMODE_MANUALMEASURE)&&(m_pDoc->GetManuMeasMode() == MANUMODE_MEAS_COMP)) //�}�j���A������ ���� [���芮��]������
	{
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(FALSE);			//[MEASUREMENT]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_MEASUREMENT_COMPLETE_BUTTON)->EnableWindow(FALSE);	//[MEASUREMENT COMPLETE]�{�^������
		(((CMainFrame*)AfxGetMainWnd())->m_wndSplitter1.GetPane(1, 0))->GetDlgItem(IDC_EXIT_BUTTON)->EnableWindow(TRUE);					//[EXIT]�{�^���L��
		return;
	}


	BOOL bIdle;
	bIdle = (Select == MEASUREMENT_MANUALMEASURE_E);


	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(bIdle); 		//[AUTO FOCUS]�{�^��
	GetDlgItem(IDC_CAPTURE_BUTTON)->EnableWindow(bIdle);			//[CAPTURE]�{�^��
	GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(bIdle);		//[SPECIFY COORD]�{�^��
	GetDlgItem(IDC_JOG_SETTING_BUTTON)->EnableWindow(bIdle);		//[JOG SETTING]�{�^��
	GetDlgItem(IDC_MEASUREMENT_BUTTON)->EnableWindow(bIdle);		//[MEASUREMENT]�{�^��

	GetDlgItem(IDC_STAGE_MOTION_MODE)->EnableWindow(bIdle); 		//MOTION MODE�I��
	GetDlgItem(IDC_ELEV_MOTION_MODE)->EnableWindow(bIdle);			//ELEV MOTION�I��
	GetDlgItem(IDC_HEAD_TYPE)->EnableWindow(bIdle); 				//HEAD TYPE�I��
	GetDlgItem(IDC_LENS_MAGNIFICATION_COMBO)->EnableWindow(bIdle);	//�����Y�{���I��
	GetDlgItem(IDC_MEASUREMENT_TYPE_COMBO)->EnableWindow(bIdle);  //�J�����I��

	GetDlgItem(IDOK)->EnableWindow(bIdle);							//[OK]�{�^��
//	GetDlgItem(IDCANCEL)->EnableWindow(bIdle);						//[CANCEL]�{�^��


	//�W���C�X�e�B�b�N�ؑւ�(�\�t�g�̃n�[�h)���W�I�{�^��
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

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	m_pMainFrame->m_pManuMeaFormViewObj->m_pAddMeasDlgObj = NULL;

}
void CAdditionalMeasurementDlg::OnSelchangeComboMeasPoint()
{
#if 0
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	//�X�e�[�W�R���t�B�O�̃��[�h
	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	//���t�@�����X�|�W�V�����̃��[�h
	SR_REFERENCE_POSITION SrReferencePosition;
	ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

	int iSelectMeasPt;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_MEAS_POINT);
	iSelectMeasPt = pCombo->GetCurSel();
	if(0 < iSelectMeasPt && iSelectMeasPt <= SrConfig.wGantryNumOfMeasPoint){
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
			/* GTR �̏ꍇ*/
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

		//���t�@�����X�|�W�V�����̃��[�h
		SR_REFERENCE_POSITION SrReferencePosition;
		ConfigFile_GetNanoSpecIni(&SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);

		SetPointData(SrReferencePosition,iSelectMeasPt);
	}
}

// ==========================================================================
// Name�F		SetPointData
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

/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
/* ����n�R���g���[���� Enable/Disable	*/
void CAdditionalMeasurementDlg::ButtonEnableCheck(void)
{
	static bool ls_bTrMasIdlePre = ((CChiefTransiMaster*) g_pcChiefView->m_pcChiefTransiMaster)->IsIdle();
	static struct {
		BOOL	l_bCtrlEna;
		int		l_bCtrlID;
	} l_MvCtrls[] = {
		TRUE,	IDC_MEASUREMENT_BUTTON,			/* 01:����				*/
		TRUE,	IDC_HEAD_TYPE,					/* 02:�w�b�h�^�C�v		*/
		TRUE,	IDC_LENS_MAGNIFICATION_COMBO,	/* 03:�����Y			*/
		TRUE,	IDC_AUTO_FOCUS_BUTTON,			/* 04:�I�[�g�t�H�[�J�X	*/
		TRUE,	IDC_SPECIFY_COORD_BUTTON,		/* 05:���W����			*/
		TRUE,	IDC_ELEV_UP_BUTTON,				/* 06:Z �� W.D.��		*/
		TRUE,	IDC_HOME_POSITION_BUTTON,		/* 07:Z �� H.�|�W�V����	*/
		TRUE,	IDC_ELEV_DOWN_BUTTON,			/* 08:Z �� W.D.��		*/
		TRUE,	IDC_STAGE_UP_BUTTON,			/* 09:XY ��				*/
		TRUE,	IDC_STAGE_DOWN_BUTTON,			/* 10:XY ��				*/
		TRUE,	IDC_STAGE_RIGHT_BUTTON,			/* 11:XY ��				*/
		TRUE,	IDC_STAGE_LEFT_BUTTON,			/* 12:XY ��				*/
		TRUE,	IDC_JOYSTICK1,					/* 13:�\�t�g�W���C		*/
		TRUE,	IDC_JOYSTICK2,					/* 14:�W���C�X�e�B�b�N	*/
		TRUE,	IDOK,							/* 15:OK				*/
		TRUE,	-1,
	};
	bool l_bTrMasIdle = ((CChiefTransiMaster*) g_pcChiefView->m_pcChiefTransiMaster)->IsIdle();
	if (ls_bTrMasIdlePre != l_bTrMasIdle) {
		/* �P�|�C���g����̃A�C�h����ԕω��L��	*/
		ls_bTrMasIdlePre = l_bTrMasIdle;
		int i = 0;
		if (true == l_bTrMasIdle) {
			/* Idle �ɂȂ����D	*/
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
			/* �r�W�[�ɂȂ����D	*/
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
		/* �P�|�C���g����̃A�C�h����ԕω�����	*/
		if (true == l_bTrMasIdle) {
			/* �A�C�h����	*/
			COverlayDialog::ButtonEnableCheck();
		}
	}
}
/* added 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */
