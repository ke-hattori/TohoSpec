// LensComboBox.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include <windowsx.h>
#include <globals.hxx>
#include <configfile.hxx>
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "Misc.h"
#include "LensComboBox.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\AlarmIf.hxx"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ##########################################################################
// CLensComboBox
// ##########################################################################
std::list<HWND> CLensComboBox::m_hWndList;

CLensComboBox::CLensComboBox()
{
}

CLensComboBox::~CLensComboBox()
{
	if ( !m_hWndList.empty() )
		m_hWndList.pop_back();
}

BEGIN_MESSAGE_MAP(CLensComboBox, CComboBox)
	//{{AFX_MSG_MAP(CLensComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELLENSCHANGE, OnSelectLensNotify)
END_MESSAGE_MAP()


LRESULT CLensComboBox::OnSelectLensNotify(WPARAM wParam, LPARAM lParam)
{
	int iCurrLens = StageGetCurLens();
	int nCount = static_cast<int>(GetCount());

	if( iCurrLens < 0 || iCurrLens > nCount ) iCurrLens = 0;

//	if ( GetItemData(GetCurSel()) == iCurrLens )
//		return 0L;

	for ( int i = 0; i < nCount; i++ ) {
		if ( GetItemData(i) == iCurrLens ) {
			SetCurSel(i);
			break;
		}
	}
	return 0L;
}

void CLensComboBox::PreSubclassWindow()
{
	m_hWndList.push_back(m_hWnd);

	///// Lens Magnification Combo /////
	SR_TURRET srTurret[SR_LENS_MAX];
	ConfigFile_GetNanoSpecIni(srTurret, CONFIG_FILE_SR_TURRET);

	int nIndex;
	for ( int i = 0; i < SR_LENS_MAX; i++ ) {
		if ( srTurret[i].bEnable ) {
			nIndex = AddString(srTurret[i].szName);
			SetItemData(nIndex, i);
		}
	}
	::PostMessage(m_hWnd, WM_SELLENSCHANGE, 0L, 0L);
	CComboBox::PreSubclassWindow();
}

BOOL CLensComboBox::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	//Saiki 20090603 Add ----->
	CString strMsg, strTitle;
	//Saiki 20090603 Add <-----

	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckIL() )	  return TRUE;
			if( !pDoc->JoyStickChangeDisable() ){
				//Saiki 20090603 Change ----->
				//MessageBox(	"���쒆�ł�",	"�x��", MB_OK);
				LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
				MessageBox(strMsg, strTitle, MB_OK);
				//Saiki 20090603 Change <-----
				// �K��JoyStick�̏�Ԃ�߂�����
				pDoc->JoyStickStatusRestore();
				return TRUE;
			}
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// �_�u���N���b�N���Ɍ듮�삷�鎖������̂Ŕr������
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

BOOL CLensComboBox::CheckIL()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	//�����[�g���͉������Ȃ�
	if(pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// �eI/O�`�F�b�N
	/*//Saiki 20090520 Change ----->*/
	if(pDoc->IsInterLock() == TRUE){return FALSE;}
	if(pDoc->CheckUnitStatus() == TRUE){return FALSE;}
	/*//Saiki 20090520 Change <-----*/
	// �V���b�^�[�ȊO�������Ă����瓮��֎~
	if( pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090530 Change ----->
		if( pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			pDoc->MessageStringIf_Set(strMsg);
			return FALSE;
		}	// �X�e�[�W���쒆
		//Saiki 20090530 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(pDoc->CheckActiveFlag() == TRUE){return FALSE;}
	/*//Saiki 20090520 Change <-----*/
	// ���C���^�[���b�N������

	return TRUE;
}

void CLensComboBox::OnSelchange()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	// �I���J�[�\���̓���
	int nSelect = GetCurSel();
	for ( std::list<HWND>::iterator i = m_hWndList.begin(); i != m_hWndList.end(); i++ )
//		::PostMessage(*i, WM_SELLENSCHANGE, 0L, 0L);
		ComboBox_SetCurSel(*i, nSelect);
	// �����Y�ύX
	int nItemData = GetItemData(nSelect);
	CCursor::BeginWaitCursor();
	pDoc->ActuateFlagsSet(ACTUATE_TURRET, TRUE);
// 2013.02.22 bagus Substrate thickness setting -->
	pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
// 2013.02.22 bagus Substrate thickness setting <--
	NS_StageSelectLens(nItemData);
// 2013.02.22 bagus Substrate thickness setting -->
	pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
// 2013.02.22 bagus Substrate thickness setting <--
	pDoc->ActuateFlagsSet(ACTUATE_TURRET, FALSE);
	CCursor::EndWaitCursor();

	// �K��JoyStick�̏�Ԃ�߂�����
	pDoc->JoyStickStatusRestore();
}

void CLensComboBox::OnSelendcancel()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	// �K��JoyStick�̏�Ԃ�߂�����
	pDoc->JoyStickStatusRestore();
}

// ##########################################################################
// CCameraComboBox
// ##########################################################################
std::list<HWND> CCameraComboBox::m_hWndList;

CCameraComboBox::CCameraComboBox()
{
}

CCameraComboBox::~CCameraComboBox()
{
	if ( !m_hWndList.empty() )
		m_hWndList.pop_back();
}

BEGIN_MESSAGE_MAP(CCameraComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCameraComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCameraComboBox::PreSubclassWindow()
{
	m_hWndList.push_back(m_hWnd);

	///// Camera Magnification Combo /////
	SYSTEM_CONFIG SystemConfig;
	ConfigFile_GetNanoSpecIni(&SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	HEAD_TYPE_CONFIG HeadType;
	ConfigFile_GetNanoSpecIni(&HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);
	SR_CONFIG SrConfig;
	ConfigFile_GetNanoSpecIni(&SrConfig, CONFIG_FILE_SR_CONFIG);

	ResetContent();

	///// �V�X�e���R���t�B�O�Őݒ肳��Ă���w�b�h�̂ݒǉ� /////
	int nIndex;
	int iChannelNo = 1;
	if(SystemConfig.nLanguage == LANGUAGE_ENGLISH){
		if(HeadType.bSR){
			nIndex = AddString(g_szViewWindowMeasurementTypeItem_ENU[0]);
			SetItemData(nIndex, iChannelNo);
		}
		iChannelNo++;
		if(SystemConfig.bExtraViewCamera){
			nIndex = AddString(g_szViewWindowMeasurementTypeItem_ENU[1]);
			SetItemData(nIndex, iChannelNo);
		}
		iChannelNo++;
		if(HeadType.bSR && SrConfig.bTransmittance && !SrConfig.bGantryType){
			nIndex = AddString(g_szViewWindowMeasurementTypeItem_ENU[2]);
			SetItemData(nIndex, iChannelNo);
		}
		iChannelNo++;
	}
	else{
		if(HeadType.bSR){
			nIndex = AddString(g_szViewWindowMeasurementTypeItem_JPN[0]);
			SetItemData(nIndex, iChannelNo);
		}
		iChannelNo++;
		if(SystemConfig.bExtraViewCamera){
			nIndex = AddString(g_szViewWindowMeasurementTypeItem_JPN[1]);
			SetItemData(nIndex, iChannelNo);
		}
		iChannelNo++;
		if(HeadType.bSR && SrConfig.bTransmittance && !SrConfig.bGantryType){
			nIndex = AddString(g_szViewWindowMeasurementTypeItem_JPN[2]);
			SetItemData(nIndex, iChannelNo);
		}
		iChannelNo++;
	}

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	OverlaySelectChannel(pMainFrame->GetCameraSelect());

	int nCount = static_cast<int>(GetCount());
	for ( int i = 0; i < nCount; i++ ) {
		if ( GetItemData(i) == pMainFrame->GetCameraSelect() ) {
			SetCurSel(i);
			break;
		}
	}

	CComboBox::PreSubclassWindow();
}

BOOL CCameraComboBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckIL() )	return TRUE;
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// �_�u���N���b�N���Ɍ듮�삷�鎖������̂Ŕr������
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

BOOL CCameraComboBox::CheckIL()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* pDoc = (CNanoSpecDoc*)pMainFrame->GetActiveDocument();

	//�����[�g���͉������Ȃ�
	if(pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ���C���^�[���b�N������
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// ���������e�i���X
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// �����e�i���X�ُ�
	}
	else{												// �ʏ퓮��
	}
	// ���C���^�[���b�N������

	return TRUE;
}

void CCameraComboBox::OnSelchange()
{
	int nSelect = GetCurSel();

	// �I���J�[�\���̓���
	for ( std::list<HWND>::iterator i = m_hWndList.begin(); i != m_hWndList.end(); i++ )
		ComboBox_SetCurSel(*i, nSelect);

	//�O���o�[�{�[�h���̓`�����l���ύX�iCh1,Ch2�j
	int iChannelNo = GetItemData(nSelect);
	OverlaySelectChannel(iChannelNo);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetCameraSelect(iChannelNo);
}
