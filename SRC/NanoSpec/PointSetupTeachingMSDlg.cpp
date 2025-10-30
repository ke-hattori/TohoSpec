// PointSetupTeachingMSDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "OverlayDialog.h"
#include "PointSetupTeachingMSDlg.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "NEXIOBASE.HXX"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg �_�C�A���O
/////////////////////////////////////////////////////////////////////////////
//
CPointSetupTeachingMSDlg::CPointSetupTeachingMSDlg(CWnd* pParent /*=NULL*/)
	: COverlayDialog(CPointSetupTeachingMSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointSetupTeachingMSDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT

	m_iTeachingMode = MODE_AUTO_MEAS;
	m_bMoveNext = FALSE;
// 2009.11.09 bagus MS �C�� --{--
	m_nSelLens = 0;
// 2009.11.09 bagus MS �C�� --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::DoDataExchange(CDataExchange* pDX)
{
	COverlayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointSetupTeachingMSDlg)
	DDX_Control(pDX, IDC_NEXT_POINT_BUTTON, m_NextPointButton);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CPointSetupTeachingMSDlg, COverlayDialog)
	//{{AFX_MSG_MAP(CPointSetupTeachingMSDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_HOME_POSITION_BUTTON, OnHomePositionButton)
	ON_CBN_SELENDOK(IDC_MICRO_SCOPE_FOCUS_POSITION, OnSelendokMicroScopeFocusPosition)
	ON_BN_CLICKED(IDC_NEXT_POINT_BUTTON, OnNextPointButton)
	//}}AFX_MSG_MAP
// 2009.11.09 bagus MS �C�� --{--
	ON_MESSAGE(WM_NEX_RESPONSE, OnNexResponse)
// 2009.11.09 bagus MS �C�� --}--
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg ���b�Z�[�W �n���h��
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CPointSetupTeachingMSDlg::OnInitDialog()
{
	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_MsConfig, CONFIG_FILE_MS_CONFIG);

	//Sample�Z�b�g
	SetFileName(m_StageProgInfoHdr.SampleInfo.szName);
	SetSampleMode(1);

	COverlayDialog::OnInitDialog();

	///// Next Point Button /////
	m_NextPointButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NextPointButton.DrawFlatFocus(TRUE);

	m_Z_UpButton.SetEnabled(FALSE);
	m_Z_DownButton.SetEnabled(FALSE);
	m_Z_HomePosButton.SetEnabled(FALSE);
	m_cbMotionElev.EnableWindow(FALSE);
//	m_btnAutoFocus.SetEnabled(FALSE);
	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(FALSE);
//	m_btnCapture.SetEnabled(FALSE);
	GetDlgItem(IDC_CAPTURE_BUTTON)->EnableWindow(FALSE);
	m_cbLens.EnableWindow(FALSE);
	//2009.11.12 bagus MS --{--
	m_cbHeadType.EnableWindow(FALSE);
	m_cbCamera.EnableWindow(FALSE);
	//2009.11.12 bagus MS --}--

	///// Lens Combo /////
	InitCombo_Lens();
	OnSelendokMicroScopeFocusPosition();

	if (m_iTeachingMode == MODE_AUTO_MEAS) {
		GetDlgItem(IDC_NEXT_POINT_BUTTON)->ShowWindow(SW_SHOW);
	} else {
		GetDlgItem(IDC_NEXT_POINT_BUTTON)->ShowWindow(SW_HIDE);
	}

	SetTimer(1, 50, NULL);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
					// ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::OnOK()
{
// 2009.11.09 bagus MS �C�� --{--
	if ( CCursor::m_bWaitCursor ) return;
// 2009.11.09 bagus MS �C�� --}--

	COverlayDialog::OnOK();

	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::OnCancel()
{
// 2009.11.09 bagus MS �C�� --{--
	if ( CCursor::m_bWaitCursor ) return;
// 2009.11.09 bagus MS �C�� --}--

	COverlayDialog::OnCancel();

	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
//
void CPointSetupTeachingMSDlg::OnTimer(UINT nIDEvent)
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
void CPointSetupTeachingMSDlg::OnNextPointButton()
{
// 2009.11.09 bagus MS �C�� --{--
	if ( CCursor::m_bWaitCursor ) return;
// 2009.11.09 bagus MS �C�� --}--

	m_bMoveNext = TRUE;

	COverlayDialog::OnOK();
}

// =========================================================================
//
void CPointSetupTeachingMSDlg::SoftJoyStickXyzEnable(BOOL bEnable)
{
	//SoftJoyStick
	m_XY_UpButton.SetEnabled(bEnable);
	m_XY_DownButton.SetEnabled(bEnable);
	m_XY_RightButton.SetEnabled(bEnable);
	m_XY_LeftButton.SetEnabled(bEnable);
//	m_Z_UpButton.SetEnabled(FALSE);
//	m_Z_DownButton.SetEnabled(FALSE);
//	m_Z_HomePosButton.SetEnabled(FALSE);
}

void CPointSetupTeachingMSDlg::OnHomePositionButton()
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

// =========================================================================
//�����Y �R���{�C�j�V��������
//
void CPointSetupTeachingMSDlg::InitCombo_Lens()
{
	CString l_strBuffer, l_strTitle;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	pCombo->ResetContent();

	int nIndex;
//2010.01.19 bagus MS --{--
	m_nLensSensorItem = -1;
//2010.01.19 bagus MS --}--
	for ( int i = 0; i < MAX_MS_ROBO_CYLINDER_NUM; i++ ) {
		if (m_MsConfig.RoboCylinder[i].bEnable ) {
			nIndex = pCombo->AddString(m_MsConfig.RoboCylinder[i].szName);
			pCombo->SetItemData(nIndex, i);
// matsuhisa 2009.12.26 added >>>
			if (m_MsConfig.RoboCylinder[i].bSensor) {
				m_nLensSensorItem = i;
			}
// matsuhisa 2009.12.26 added <<<
		}
	}

	//2009.12.25 bagus MS --{--
	RCP_DATA rcp;
	m_pDoc->GetRcpData(&rcp);
	if(0 <= rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex
	&& rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex < pCombo->GetCount()){
		pCombo->SetCurSel(rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex);
		m_nSelLens = rcp.MeasProgInfo.ScanParams._MScope.iMagLensIndex;
	}else{
		pCombo->SetCurSel(0);
		m_nSelLens = 0;
	}
	//2009.12.25 bagus MS --}--
}

// =========================================================================
// �����Y�؂�ւ� �ړ�
//
void CPointSetupTeachingMSDlg::OnSelendokMicroScopeFocusPosition()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_MICRO_SCOPE_FOCUS_POSITION);
	int	nIndex = pCombo->GetCurSel();
	int	nItemData = pCombo->GetItemData(nIndex);

	int	nPosNo = m_MsConfig.RoboCylinder[nItemData].nPosition;

	CString l_strBuffer, l_strTitle, strMsg;
// matsuhisa 2009.12.26 added >>>
	if ( !nexioIsMS_LensKind() ) {
//2010.01.19 bagus MS --{--
//		  if ( nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
		if (m_nLensSensorItem >= 0 &&  nPosNo == m_MsConfig.RoboCylinder[m_nLensSensorItem].nPosition ) {
//2010.01.19 bagus MS --}--
			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
			m_pDoc->MessageStringIf_Set(strMsg);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
	}
// matsuhisa 2009.12.26 added <<<

// 2009.11.09 bagus MS �C�� --{--
	if ( CCursor::m_bWaitCursor ) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--

	//�����[�g���͉������Ȃ�
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
// 2009.11.09 bagus MS �C�� --{--
//	if(m_pDoc->GetHostMode() == HOST_REMOTE) return;
	if (m_pDoc->GetHostMode() == HOST_REMOTE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--

	CMainFrame* pMainFrame = (CMainFrame *)AfxGetMainWnd();
	// ���C���^�[���b�N������
	//�����e�i���X���[�h�`�F�b�N(2���������쐧��)
	if( nexioIsEngineerMaintenanceSwitch()==ON )// ���������e�i���X���[�h
	{
// 2009.11.09 bagus MS �C�� --{--
//		if( nexioIsMaintenanceSwitch() != OFF ) {AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn); return;} //�����e�i���X���[�h�g�ݍ��킹�G���[
		if ( nexioIsMaintenanceSwitch() != OFF ) {
			AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);
			pCombo->SetCurSel(m_nSelLens);
			return;
		}
// 2009.11.09 bagus MS �C�� --}--
		/*���������e���[�h�̎��́A�K�������e�i���X���[�h(L)�ƂȂ�
			���̏ꍇ�A2����������\ */
	}
	else //���������e�i���X���[�h�ȊO
	{
	}

	// �eI/O�`�F�b�N
// 2009.11.09 bagus MS �C�� --{--
//	if(m_pDoc->IsInterLock() == TRUE){return;}
//	if(m_pDoc->CheckUnitStatus() == TRUE){return;}
// matsuhisa 2009.12.26 modified >>>
//	if (m_pDoc->IsInterLock() == TRUE) {
	if (m_pDoc->IsInterLockForMScope() == TRUE) {
// matsuhisa 2009.12.26 modified <<<
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
	if (m_pDoc->CheckUnitStatus() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--

	//ActuateFlag�`�F�b�N
	if(m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) && pMainFrame->GetJoyStickMode()==0){
		LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
// 2009.11.09 bagus MS �C�� --{--
//		return;
		pCombo->SetCurSel(m_nSelLens);
		return;
// 2009.11.09 bagus MS �C�� --}--
	} //�X�e�[�W�����쒆�ł�
// 2009.11.09 bagus MS �C�� --{--
//	if(m_pDoc->CheckActiveFlag() == TRUE){return;}
	if (m_pDoc->CheckActiveFlag() == TRUE) {
		pCombo->SetCurSel(m_nSelLens);
		return;
	}
// 2009.11.09 bagus MS �C�� --}--
	// ���C���^�[���b�N������

// 2009.11.09 bagus MS �C�� --{--
#if 0
	// Z�����싖�����v OFF
	nexioMS_ModeLamp(FALSE);

	// �ʒu����
	nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo);

	// Z�����싖�����v ON
	nexioMS_ModeLamp(TRUE);
#else
	if (!m_pDoc->ActuateFlagsGet(ACTUATE_MICROSCOPE)) {
// matsuhisa 2009.12.26 deleted >>>
//		if (m_MsConfig.RoboCylinder[nItemData].bSensor == nexioIsMS_LensKind()) {
// matsuhisa 2009.12.26 deleted <<<
			// ���̈ړ����͂̎�t���֎~����
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_JOYSTICK_WORKING, strMsg, "JoyStick is Working");
				LoadStringML(IDS_TITLE_WARNING, l_strTitle, "Warning");
				MessageBox(strMsg, l_strTitle, MB_OK);
				// �K��JoyStick�̏�Ԃ�߂�����
				m_pDoc->JoyStickStatusRestore();
				pCombo->SetCurSel(m_nSelLens);
				return;
			}

			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,TRUE);

			// Mode�؂�ւ�
			unsigned char oldMode = nexioRC_GetCurrentMode();
			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, RCOPMODE_NORMAL);
				MSG 	msg;

				//��������������܂ő҂�
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			// �ʒu����
			if(nexifRC_MoveToTeachPos(this->m_hWnd, nPosNo)){
				// ActuateFlag���Z�b�g����
				m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, TRUE);
				MSG 	msg;

				//��������������܂ő҂�
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}

			if(oldMode != RCOPMODE_NORMAL){
				nexifRC_SelectMode(this->m_hWnd, oldMode);
				MSG 	msg;

				//��������������܂ő҂�
				while (1){
					if(::PeekMessage(&msg, NULL, WM_NEX_RESPONSE, WM_NEX_RESPONSE, PM_REMOVE)){
						::TranslateMessage(&msg);
						::DispatchMessage(&msg);
						break;
					}else{
						Sleep(10);
					}
				}
			}
			m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE,FALSE);

// matsuhisa 2009.12.26 deleted >>>
//		} else {
//			// �ُ�
//			LoadStringML(IDS_MICROSOFT_LENS_MISMATCH, strMsg, "Microscope Lens kind is mismatch.");
//			m_pDoc->MessageStringIf_Set(strMsg);
//			pCombo->SetCurSel(m_nSelLens);
//		}
// matsuhisa 2009.12.26 deleted <<<
	}
#endif

	m_nSelLens = nIndex;
// 2009.11.09 bagus MS �C�� --}--
}

// 2009.11.09 bagus MS �C�� --{--
LRESULT CPointSetupTeachingMSDlg::OnNexResponse(WPARAM wparam, LPARAM lparam)
{
	switch ( wparam ) {
	case nexRC_MoveToTeachPos:
	case nexRC_JogPlus:
	case nexRC_JogMinus:
	case nexRC_JogStop:
	case nexRC_SelectMode:
		m_pDoc->ActuateFlagsSet(ACTUATE_MICROSCOPE, FALSE);
		break;
	default:
		break;
	}
	return 0;
}
// 2009.11.09 bagus MS �C�� --}--
// 2009.11.26 bagus �S�� �C�� --{--
void CPointSetupTeachingMSDlg::ButtonEnableCheck()
{
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- { ---------- */
//	COverlayDialog::ButtonEnableCheck();
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ----------				*/
	/* XY���{�^��	*/
	BOOL bXyButtonEnable = ( m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	m_XY_UpButton.SetEnabled(bXyButtonEnable);
	m_XY_DownButton.SetEnabled(bXyButtonEnable);
	m_XY_RightButton.SetEnabled(bXyButtonEnable);
	m_XY_LeftButton.SetEnabled(bXyButtonEnable);

//	/* Z���{�^��	*/
//	BOOL bZButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) );
//	m_Z_UpButton.SetEnabled(bZButtonEnable);
//	m_Z_DownButton.SetEnabled(bZButtonEnable);
//	  m_Z_HomePosButton.SetEnabled(bZButtonEnable);

//	/* AutoFocus�{�^��	*/
//	BOOL bAFButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) );
//	GetDlgItem(IDC_AUTO_FOCUS_BUTTON)->EnableWindow(bAFButtonEnable);

	/* ���W���̓{�^��	*/
	BOOL bCoordButtonEnable = ( m_pMainFrame->GetJoyStickMode() == JOY_SOFT_MODE && !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ||
								m_pMainFrame->GetJoyStickMode() != JOY_SOFT_MODE );
	GetDlgItem(IDC_SPECIFY_COORD_BUTTON)->EnableWindow(bCoordButtonEnable);
/* modified 2009.12.09 hmenjo ���� Seq �͎w��w�b�h�œ���(�ǉ�����) ---------- } ---------- */

	//�X�e�[�W�ړ����͈ړ������Ȃ�
	BOOL bNextPrevButtonEnable = ( !m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) );
	GetDlgItem(IDC_NEXT_POINT_BUTTON)->EnableWindow(bNextPrevButtonEnable);

}
// 2009.11.26 bagus �S�� �C�� --}--
