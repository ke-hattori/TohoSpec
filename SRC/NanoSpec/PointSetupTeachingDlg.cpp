// PointSetupTeachingDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingDlg.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\NexIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Saiki 20110207 Delete ----->
//#define	PIN_INTERVAL		7
//Saiki 20110207 Delete <-----

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingDlg �_�C�A���O
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
CPointSetupTeachingDlg::CPointSetupTeachingDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPointSetupTeachingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupTeachingDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT

// 2009.09.14 bagus stress Stress�Ή� --{--
	m_wHeadType = HEAD_TYPE_SR;
	m_lYPoint = 0;
	m_nSelectedPoint = 0;
// 2009.09.14 bagus stress Stress�Ή� --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupTeachingDlg)
	DDX_Control(pDX, IDC_NEXT_POINT_BUTTON, m_NextPointButton);
	DDX_Control(pDX, IDC_PREVIOUS_POINT_BUTTON, m_PreviousPointButton);
	DDX_Control(pDX, IDC_APPLY_BUTTON, m_ApplyButton);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MOVE_POINT, m_MoveButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CPointSetupTeachingDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPointSetupTeachingDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_NEXT_POINT_BUTTON, OnNextPointButton)
	ON_BN_CLICKED(IDC_PREVIOUS_POINT_BUTTON, OnPreviousPointButton)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, OnApplyButton)
	ON_BN_CLICKED(IDC_HOME_POSITION_BUTTON, OnHomePositionButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MOVE_POINT, OnMoveButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingDlg ���b�Z�[�W �n���h��
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CPointSetupTeachingDlg::OnInitDialog()
{
	//Sample�Z�b�g
	SetFileName(m_StageProgInfoHdr.SampleInfo.szName);
	SetSampleMode(1);

	COverlayDialog::OnInitDialog();

	///// Next Point Button /////
	m_NextPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NextPointButton.DrawFlatFocus(TRUE);

	///// Previous Point Button /////
	m_PreviousPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_PreviousPointButton.DrawFlatFocus(TRUE);

	///// Apply Button /////
	m_ApplyButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ApplyButton.DrawFlatFocus(TRUE);

	///// Move Button /////
	m_MoveButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_MoveButton.DrawFlatFocus(TRUE);

	// ����|�C���g��\��
// 2009.09.14 bagus stress Stress�Ή� --{--
//	m_iCurrPoint = m_iNumScans == 0 ? 1 : m_iNumScans + 1;

	switch (m_wHeadType) {
	case HEAD_TYPE_STRESS:
		if ((m_nSelectedPoint >= 1) && (m_nSelectedPoint <= m_iNumScans)){
			m_iCurrPoint = m_nSelectedPoint;
		} else {
			m_iCurrPoint = 1;
		}
		break;
	default:
		m_iCurrPoint = m_iNumScans == 0 ? 1 : m_iNumScans + 1;
		break;
	}
// 2009.09.14 bagus stress Stress�Ή� --}--
	SetPointData();

	m_iProcessStatusOld = -1;

	SetTimer(1, 50, NULL);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingDlg::OnOK()
{
//	COverlayDialog::SetOperationLog("OK Button was push.");
//
//	HookKeyUnhook();
//
//	CDialog::OnOK();

	COverlayDialog::OnOK();

	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingDlg::OnCancel()
{
//	COverlayDialog::SetOperationLog("OK Button was push.");
//
//	HookKeyUnhook();
//
//	CDialog::OnCancel();

	COverlayDialog::OnCancel();

	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
//
HBRUSH CPointSetupTeachingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = COverlayDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch(nCtlColor){
	case CTLCOLOR_STATIC:
// 2009.09.14 bagus stress Stress�Ή� --{--
//		///// Point No. /////
//		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_POINT_NO)->GetSafeHwnd()){
//			ControlColor(pDC, &m_Brush_PointNo, WATER_COLOR);
//			return m_Brush_PointNo;
//		}
// 2009.09.14 bagus stress Stress�Ή� --}--

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

		break;
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingDlg::OnTimer(UINT nIDEvent)
{
//�͑������
//�{�^���̗L��/�����̐ؑւ���ActuateFlags�������ɂ��č쐬�����肢���܂��B
#if 0 //ProcessStatus���g���Ă���̂ŊԈႢ
	int iProcessStatus = m_pDoc->GetProcessStatus();

	//�\�t�g�W���C�X�e�B�b�N����
	if(iProcessStatus != m_iProcessStatusOld)
	{
		if( iProcessStatus != PROCESS_WAIT )
		{
			SoftJoyStickXyzEnable(FALSE); //����֎~
		}
		else
		{
			SoftJoyStickXyzEnable(TRUE); //���싖��
		}
	}
	m_iProcessStatusOld = iProcessStatus;
#endif

	COverlayDialog::OnTimer(nIDEvent);
}

// ==========================================================================
// Name�F		OnNextPointButton
void CPointSetupTeachingDlg::OnNextPointButton()
{
// 2009.09.14 bagus stress Stress�Ή� --{--
//	if(m_iCurrPoint <= m_iNumScans){
//		m_iCurrPoint++;
//		SetPointData();
//	}

	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

	switch (m_wHeadType) {
	case HEAD_TYPE_STRESS:
		if ((m_iCurrPoint >= 1) && (m_iCurrPoint < m_iNumScans)){
			m_iCurrPoint++;
			SetPointData();
		} else {
			// Err
			return;
		}
		break;
	default:
		if ((m_iCurrPoint >= 1) && (m_iCurrPoint <= m_iNumScans)){
			m_iCurrPoint++;
			SetPointData();
		} else {
			// Err
			return;
		}
		break;
	}
// 2009.09.14 bagus stress Stress�Ή� --}--

	// Point���o�^����Ă���ꍇ�ɂ͂��̃|�C���g�Ɉړ�
	if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
		OnMoveButton();
	}
}

// ==========================================================================
// Name�F		OnPreviousPointButton
void CPointSetupTeachingDlg::OnPreviousPointButton()
{

// 2009.09.14 bagus stress Stress�Ή� --{--
//	if(m_iCurrPoint > 1){
//		m_iCurrPoint--;
//		SetPointData();
//
//		// ���̃|�C���g�Ɉړ�
//		if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
//			OnMoveButton();
//		}
//	}

	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

	switch (m_wHeadType) {
	case HEAD_TYPE_STRESS:
		if ((m_iCurrPoint > 1) && (m_iCurrPoint < m_iNumScans + 1)){
			m_iCurrPoint--;
			SetPointData();
		} else {
			// Err
			return;
		}
		break;
	default:
		if ((m_iCurrPoint > 1) && (m_iCurrPoint <= m_iNumScans + 1)){
			m_iCurrPoint--;
			SetPointData();
		} else {
			// Err
			return;
		}
		break;
	}

	// ���̃|�C���g�Ɉړ�
	if(IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK) == TRUE){
		OnMoveButton();
	}
// 2009.09.14 bagus stress Stress�Ή� --}--
}

// ==========================================================================
// Name�F		OnApplyButton
void CPointSetupTeachingDlg::OnApplyButton()
{
	CString csBuff;

// 2009.09.14 bagus stress Stress�Ή� --{--
//	if(m_iCurrPoint == 0){
//		m_iCurrPoint = 1;
//	}

	if (!CheckInputCurrPoint()) {
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, csBuff, "It specifies outside the range.");
		AfxMessageBox(csBuff);
		return;
	}
// 2009.09.14 bagus stress Stress�Ή� --}--

// 2009.09.14 bagus stress Stress�Ή� --{--
#if 0
	// X
	GetDlgItemText(IDC_SAMPLE_X, csBuff);
	m_ScanPoint[m_iCurrPoint - 1].lX = (double)(atof(csBuff) * MICROMETRE);
	// Y
	GetDlgItemText(IDC_SAMPLE_Y, csBuff);
	m_ScanPoint[m_iCurrPoint - 1].lY = (double)(atof(csBuff) * MICROMETRE);

	m_iNumScans = m_iCurrPoint > m_iNumScans ? m_iNumScans + 1 : m_iNumScans;

	// ���̃|�C���g��Teaching�Ɉړ�
	if(IsDlgButtonChecked(IDC_APPLY_NEXT_CHECK) == TRUE){
		m_iCurrPoint++;
/* added 2009.07.16 hmenjo �|�C���g�Z�b�g�{�^���ł����ړ����� ---------- { ---------- */
		if (TRUE == this->IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK)) {
			this->OnMoveButton();
		}
/* added 2009.07.16 hmenjo �|�C���g�Z�b�g�{�^���ł����ړ����� ---------- } ---------- */
	}
#else
	long	lScanPointX, lScanPointY;
	int		nCurrPoint, nCheckPoint;

	nCurrPoint = m_iCurrPoint - 1;

	// ���W�̃`�F�b�N���s��
	switch (m_wHeadType) {
	case HEAD_TYPE_STRESS:
		GetDlgItemText(IDC_SAMPLE_X, csBuff);
		lScanPointX = static_cast<long>((double)(atof(csBuff) * MICROMETRE));
		GetDlgItemText(IDC_SAMPLE_Y, csBuff);
		lScanPointY = static_cast<long>((double)(atof(csBuff) * MICROMETRE));

		// X���̃`�F�b�N
		nCheckPoint = nCurrPoint % PIN_INTERVAL;
		switch (nCheckPoint) {
		case 0:
			if (m_ScanPoint[nCurrPoint + 1].lX < lScanPointX) {
				LoadStringML(IDS_OVERLAY_MOVE_X_AXIS, csBuff, "The value of X axis is outside an effective range.");
				AfxMessageBox(csBuff);
				return;
			}
			break;
		case 1:
			if (lScanPointX < m_ScanPoint[nCurrPoint - 1].lX) {
				LoadStringML(IDS_OVERLAY_MOVE_X_AXIS, csBuff, "The value of X axis is outside an effective range.");
				AfxMessageBox(csBuff);
				return;
			}
			break;
		case PIN_INTERVAL - 2:
			if (lScanPointX > m_ScanPoint[nCurrPoint + 1].lX) {
				LoadStringML(IDS_OVERLAY_MOVE_X_AXIS, csBuff, "The value of X axis is outside an effective range.");
				AfxMessageBox(csBuff);
				return;
			}
			break;
		case PIN_INTERVAL - 1:
			if (m_ScanPoint[nCurrPoint - 1].lX > lScanPointX) {
				LoadStringML(IDS_OVERLAY_MOVE_X_AXIS, csBuff, "The value of X axis is outside an effective range.");
				AfxMessageBox(csBuff);
				return;
			}
			break;
		default:
			break;
		}

		// Y���̈ړ��͋֎~
		if (m_ScanPoint[nCurrPoint].lY != lScanPointY) {
			LoadStringML(IDS_OVERLAY_MOVE_Y_AXIS, csBuff, "The value of Y axis has been changed.");
			AfxMessageBox(csBuff);
			return;
		}

		// X
		m_ScanPoint[nCurrPoint].lX = lScanPointX;
		// Y
//		m_ScanPoint[nCurrPoint].lY = lScanPointY;
		break;
	default:
		// X
		GetDlgItemText(IDC_SAMPLE_X, csBuff);
		m_ScanPoint[nCurrPoint].lX = static_cast<long>((double)(atof(csBuff) * MICROMETRE));
		// Y
		GetDlgItemText(IDC_SAMPLE_Y, csBuff);
		m_ScanPoint[nCurrPoint].lY = static_cast<long>((double)(atof(csBuff) * MICROMETRE));
		break;
	}

	// ���|�C���g�����C���N�������g
	switch (m_wHeadType) {
	case HEAD_TYPE_STRESS:
		break;
	default:
		m_iNumScans = m_iCurrPoint > m_iNumScans ? m_iNumScans + 1 : m_iNumScans;
		break;
	}

	// ���̃|�C���g��Teaching�Ɉړ�
	if(IsDlgButtonChecked(IDC_APPLY_NEXT_CHECK) == TRUE){
		switch (m_wHeadType) {
		case HEAD_TYPE_STRESS:
			if ((m_iCurrPoint >= 1) && (m_iCurrPoint < m_iNumScans)){
				m_iCurrPoint++;
			} else {
				// Err
//				return;
			}
			break;
		default:
			m_iCurrPoint++;
			break;
		}
		if (TRUE == this->IsDlgButtonChecked(IDC_SYNCHRONOUS_CHECK)) {
			this->OnMoveButton();
		}
	}
#endif
// 2009.09.14 bagus stress Stress�Ή� --}--

	// �|�C���g�ݒ�
	SetPointData();

}

// ==========================================================================
// Name�F		SetPointData
void CPointSetupTeachingDlg::SetPointData()
{
	CString csPointNo, csX, csY;

	if(m_iCurrPoint == 0 || m_iCurrPoint > m_iNumScans){
		csX.Format("");
		csY.Format("");
	}
	else{
		// X
		csX.Format("%.3lf", (double)(m_ScanPoint[m_iCurrPoint - 1].lX) / MICROMETRE);
		// Y
		csY.Format("%.3lf", (double)(m_ScanPoint[m_iCurrPoint -1].lY) / MICROMETRE);
	}
	// Point No.
	csPointNo.Format("%d", m_iCurrPoint);

	SetDlgItemText(IDC_POINT_NO, csPointNo);
	SetDlgItemText(IDC_POSITION_X, csX);
	SetDlgItemText(IDC_POSITION_Y, csY);
}

// ==========================================================================
// Name�F		OnMoveButton
void CPointSetupTeachingDlg::OnMoveButton()
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

// 2009.09.14 bagus stress Stress�Ή� --{--
#if 0
	if(m_iCurrPoint <= m_iNumScans){
		m_iCurrPoint;
		SetPointData();
	}

	// Point���o�^����Ă���ꍇ�ɂ͂��̃|�C���g�Ɉړ�
	if(m_iCurrPoint <= m_iNumScans){
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		STAGE_COORD sampleCoord;
		sampleCoord.lX = m_ScanPoint[m_iCurrPoint - 1].lX + m_StageProgInfoHdr.SampleInfo.Origin.lX;
		sampleCoord.lY = m_ScanPoint[m_iCurrPoint - 1].lY + m_StageProgInfoHdr.SampleInfo.Origin.lY;
		NS_StageMoveAbsolute(&sampleCoord);
		m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	}
#else
	if (!CheckInputCurrPoint()) {
		CString	strBuffer;
		LoadStringML(IDS_OVERLAY_INPUT_POINT_NO, strBuffer, "It specifies outside the range.");
		AfxMessageBox(strBuffer);
		return;
	}

	SetPointData();

	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	STAGE_COORD sampleCoord;
// 2009.12.02 K.Matsuo SampleOrigin -->
//	sampleCoord.lX = m_ScanPoint[m_iCurrPoint - 1].lX + m_StageProgInfoHdr.SampleInfo.Origin.lX;
//	sampleCoord.lY = m_ScanPoint[m_iCurrPoint - 1].lY + m_StageProgInfoHdr.SampleInfo.Origin.lY;
	SAMPLEINFO sampleInfo;
	::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
	RecipeFile_LoadRecipe(&sampleInfo, m_StageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
	sampleCoord.lX = m_ScanPoint[m_iCurrPoint - 1].lX + sampleInfo.Origin.lX;
	sampleCoord.lY = m_ScanPoint[m_iCurrPoint - 1].lY + sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
	NS_StageMoveAbsolute(&sampleCoord);
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
#endif
// 2009.09.14 bagus stress Stress�Ή� --}--
}


// =========================================================================
//
void CPointSetupTeachingDlg::SoftJoyStickXyzEnable(BOOL bEnable)
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

void CPointSetupTeachingDlg::OnHomePositionButton()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
// 2010.02.03 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� -->
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
// 2010.02.03 K.Matsuo PIN UPPER �C���^�[���b�N�`�F�b�N�R��Ή� <--

	BOOL l_bAF = TRUE;
	EEPROMDATA l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z ���̏�������~�ʒu
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu; // Z ���̏�������~�ʒu�t�@�N�^

	// �n�[�h�V�~�����[�g���[�h���� -------------------------------------------
	if (0 != m_pMainFrame->HardwareSimulation()) {
		// �n�[�h�V�~�����[�g���[�h�������̂ŁC���芮���҂� ��
	}
	else {
		// ���@�̏ꍇ ---------------------------------------------------------
		// AF �v/�s�v����
		// �s�v
			// (�S�_ AF �����̏ꍇ)Z ���ړ����K�v���`�F�b�N
		// �K�v(�d�l��K�v�Ȃ�)
		// �y���ړ�
		//TCHAR l_szMainRcpName[256];
		//((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// ��������~�ʒu[pulse]��[0.1um]�ɕϊ����܂�
		LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
		l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// ��������~�ʒu[pulse]���t�@�N�^[pulse/mm]
		l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]�ɕϊ�����
		long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));

		NS_ConvertToStageMoveCoord(&l_lZ);		// �␳

		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// ���쒆�t���O(Z ��)���I��
#if 0	//----- Kawashima 2008.12.01 ----->
//				TransiEvent(EV_SR1P_ZMOVE_DONE);
#else	//----- Kawashima 2008.12.01 -----
		m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVING);  // Status Bar
		if (0 == StageElevatorMove(l_lZ)) {
			// �ړ��J�n���s
		} else {
			// �ړ�����
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ---------- { ---------- */
			m_pMainFrame->SetMessageText(IDS_CHIF_SR1P_Z_MOVED);   // Status Bar
#else						/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ----------			   */
			m_pMainFrame->SetMessageText(_T(" ")); // Status Bar
#endif						/* modified 2009.08.05 hmenjo �������b�Z�[�W�\�����Ȃ� ---------- } ---------- */
		}
#endif	//----- Kawashima 2008.12.01 <-----
		m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// ���쒆�t���O(Z ��)���I�t
	}
}

// 2009.09.14 bagus stress Stress�Ή� --{--
BOOL CPointSetupTeachingDlg::CheckInputCurrPoint()
{
	CString	sCurrPoint;
	DWORD	dwCurrPoint;

	GetDlgItemText(IDC_POINT_NO, sCurrPoint);
	dwCurrPoint = strtol(sCurrPoint, NULL, 10);

	switch (m_wHeadType) {
	case HEAD_TYPE_STRESS:
		if ((dwCurrPoint >= 1) && (dwCurrPoint < static_cast<DWORD>(m_iNumScans + 1))){
			m_iCurrPoint = dwCurrPoint;
		} else {
			// Err
			return ( FALSE );
		}
	break;
	default:
		if ((dwCurrPoint >= 1) && (dwCurrPoint <= static_cast<DWORD>(m_iNumScans + 1))){
			m_iCurrPoint = dwCurrPoint;
		} else {
			// Err
			return ( FALSE );
		}
		break;
	}

	return ( TRUE );
}
// 2009.09.14 bagus stress Stress�Ή� --}--
// 2009.11.26 bagus �S�� �C�� --{--
void CPointSetupTeachingDlg::ButtonEnableCheck()
{
	COverlayDialog::ButtonEnableCheck();

	//�X�e�[�W�ړ����͈ړ������Ȃ�
	BOOL bNextPrevButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	GetDlgItem(IDC_NEXT_POINT_BUTTON)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_PREVIOUS_POINT_BUTTON)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_MOVE_POINT)->EnableWindow(bNextPrevButtonEnable);
	GetDlgItem(IDC_APPLY_BUTTON)->EnableWindow(bNextPrevButtonEnable);

}
// 2009.11.26 bagus �S�� �C�� --}--
