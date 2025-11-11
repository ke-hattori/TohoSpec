// MotionComboBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MotionComboBox.h"
#include "..\\..\\INC\\NexIOBASE.hxx"
#include "..\\..\\INC\\AlarmIf.hxx"
#include <windowsx.h>
#include "MainFrm.h"
#include "Misc.h"
#include "..\\..\\INC\\NSStage.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeComboBox
std::list<HWND> CTypeComboBox::m_hWndList;

CTypeComboBox::CTypeComboBox()
{
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	m_bSelChanged = FALSE;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
}

CTypeComboBox::~CTypeComboBox()
{
	if ( !m_hWndList.empty() )
		m_hWndList.pop_back();
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CTypeComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	ON_MESSAGE(WM_SELHEADTYPECHANGE, OnSelectHeadTypeNotify)
END_MESSAGE_MAP()

// ==========================================================================
//
LRESULT CTypeComboBox::OnSelectHeadTypeNotify(WPARAM wParam, LPARAM lParam)
{
	int nHeadType = NS_GetCurrentHeadType();
	int nCount = GetCount();

	for ( int i = 0; i < nCount; i++ ) {
		if ( GetItemData(i) == nHeadType ) {
			SetCurSel(i);
			break;
		}
	}
	return 0L;
}

// ==========================================================================
//
void CTypeComboBox::PreSubclassWindow()
{
	m_hWndList.push_back(m_hWnd);

	ResetContent();

	int iSel;

	ConfigFile_GetNanoSpecIni(&m_HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);
	// 選択されているヘッドのみ表示
	if(m_HeadType.bSR){
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SR]);
		SetItemData(iSel, HEAD_TYPE_SR);
	}
	if(m_HeadType.bSE){
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if(m_HeadType.bCompEASE) {
			iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_COMPEASE]);
		}
		else {
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SE]);
		}
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		SetItemData(iSel, HEAD_TYPE_SE);
	}
// 2009.10.19 bagus MS 追加 --{--
#if 0
	if(m_HeadType.bIRSE){
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_IRSE]);
		SetItemData(iSel, HEAD_TYPE_IRSE);
	}
#endif
// 2009.10.19 bagus MS 追加 --}--
	if(m_HeadType.bResist){
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_4PP]);
		SetItemData(iSel, HEAD_TYPE_4PP);
	}
	if(m_HeadType.bCTA){
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_CTA]);
		SetItemData(iSel, HEAD_TYPE_CTA);
	}
	if(m_HeadType.bStress){
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_STRESS]);
		SetItemData(iSel, HEAD_TYPE_STRESS);
	}
// 2009.10.19 bagus MS 追加 --{--
	if(m_HeadType.bMS){
		iSel = AddString(HEAD_TYPE_ITEM[HEAD_TYPE_MS]);
		SetItemData(iSel, HEAD_TYPE_MS);
	}
// 2009.10.19 bagus MS 追加 --}--

	::PostMessage(m_hWnd, WM_SELHEADTYPECHANGE, 0L, 0L);
	CComboBox::PreSubclassWindow();
}

// ==========================================================================
//
BOOL CTypeComboBox::PreTranslateMessage(MSG* pMsg)
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	CString strMsg, strTitle;

	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckIL() )	return TRUE;
			if( !m_pDoc->JoyStickChangeDisable() ){
				LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
				LoadStringML(IDS_TITLE_WARNING, strTitle, "WARNING");
				MessageBox(strMsg, strTitle, MB_OK);
				m_pDoc->JoyStickStatusRestore();
				return TRUE;
			}
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// ダブルクリック時に誤動作する事があるので排除する
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
BOOL CTypeComboBox::CheckIL()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// メンテナンス異常
	}
	else{												// 通常動作
		if( nexioIsMaintenanceSwitch() != ON ){ 	AlarmIf_Set(ALID_MaintenanceSwitchOn); 	return FALSE;}	// メンテナンス・スイッチ・オン
	}
	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return FALSE;}
	if( nexioIsMovo2Alarm() 	!= OFF){	AlarmIf_Set(ALID_StageError);	return FALSE;}	// MOVOアラーム 				＝Ｌ
	if(m_pDoc->CheckUnitStatus() == TRUE){return FALSE;}
	/*//Saiki 20090520 Change <-----*/
	// シャッター以外が動いていたら動作禁止
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return FALSE;
		}	 // ステージ動作中
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return FALSE;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲

	return TRUE;
}

// ==========================================================================
//
void CTypeComboBox::OnSelchange()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// 選択カーソルの同期
	int nSelect = GetCurSel();
	for ( std::list<HWND>::iterator i = m_hWndList.begin(); i != m_hWndList.end(); i++ )
//		::PostMessage(*i, WM_SELHEADTYPECHANGE, 0L, 0L);
		ComboBox_SetCurSel(*i, nSelect);
	// ヘッドタイプ変更
	int nItemData = GetItemData(nSelect);
	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
	NS_SelectHeadType(nItemData, TRUE);
	m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
	CCursor::EndWaitCursor();

	// 必ずJoyStickの状態を戻すこと
	m_pDoc->JoyStickStatusRestore();
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	m_bSelChanged = TRUE;
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */
}

// ==========================================================================
//
void CTypeComboBox::OnSelendcancel()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// 必ずJoyStickの状態を戻すこと
	m_pDoc->JoyStickStatusRestore();
}


/////////////////////////////////////////////////////////////////////////////
// CMotionComboBox
std::list<HWND> CMotionComboBox::m_hWndList;

CMotionComboBox::CMotionComboBox()
{
}

CMotionComboBox::~CMotionComboBox()
{
	if ( !m_hWndList.empty() )
		m_hWndList.pop_back();
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CMotionComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, CMotionComboBox::OnSelchange)
END_MESSAGE_MAP()

// ==========================================================================
//
void CMotionComboBox::PreSubclassWindow()
{
	m_hWndList.push_back(m_hWnd);

	ResetContent();

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig1, CONFIG_FILE_SYSTEM_CONFIG);

	int nIndex;

	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	// Kojika 20090528 Change
	//PCSTR pszString[] = MOTION_MODE_STAGE_STR_INIT;
	//if(m_SystemConfig1.nStageType == STAGE_TYPE_SPT){
	//	  for ( int index = 0; index < STAGE_MOTION_MAX; index++ ){
	//		  nIndex = AddString(pszString[index]);
	//		  SetItemData(nIndex, index);
	//	  }
	//}else{
	//	for ( int index = 0; index < (STAGE_MOTION_MAX-3); index++ ){
	//		nIndex = AddString(pszString[index]);
	//		SetItemData(nIndex, index);
	//	}
	//}

	if(l_SystemConfig.nLanguage == 0){
		PCSTR pszString[] = MOTION_MODE_STAGE_STR_INIT_ENU;
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		if(m_SystemConfig1.nStageType == STAGE_TYPE_SPT){
//			for ( int index = 0; index < STAGE_MOTION_MAX; index++ ){
//				nIndex = AddString(pszString[index]);
//				SetItemData(nIndex, index);
//			}
//		}
//		else{
// 2013.11.08 Bagus Del (TohoSpec対応) <--
			for ( int index = 0; index < (STAGE_MOTION_MAX-3); index++ ){
				nIndex = AddString(pszString[index]);
				SetItemData(nIndex, index);
			}
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		}
// 2013.11.08 Bagus Del (TohoSpec対応) <--
	}
	else{
		PCSTR pszString[] = MOTION_MODE_STAGE_STR_INIT_JPN;
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		if(m_SystemConfig1.nStageType == STAGE_TYPE_SPT){
//			for ( int index = 0; index < STAGE_MOTION_MAX; index++ ){
//				nIndex = AddString(pszString[index]);
//				SetItemData(nIndex, index);
//			}
//		}else{
// 2013.11.08 Bagus Del (TohoSpec対応) <--
			for ( int index = 0; index < (STAGE_MOTION_MAX-3); index++ ){
				nIndex = AddString(pszString[index]);
				SetItemData(nIndex, index);
			}
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		}
// 2013.11.08 Bagus Del (TohoSpec対応) <--
	}
	// Kojika 20090528 Change End

	SetCurSel(GetMode());
}

// ==========================================================================
//
BOOL CMotionComboBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckIL() )	return TRUE;
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// ダブルクリック時に誤動作する事があるので排除する
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
BOOL CMotionComboBox::CheckIL()
{
	//Saiki 20090530 Add ----->
	CString strMsg;
	//Saiki 20090530 Add <-----

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// メンテナンス異常
	}
	else{												// 通常動作
	}
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090531 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return FALSE;
		}	 // ステージ動作中
		//Saiki 20090531 Change <-----
	}
	// ▲インターロック条件▲

	return TRUE;
}

// ==========================================================================
//
void CMotionComboBox::OnSelchange()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();

	int nSelect = GetCurSel();
	if ( nSelect != CB_ERR ) {
		m_nMode = nSelect;
	}
	for ( std::list<HWND>::iterator i = m_hWndList.begin(); i != m_hWndList.end(); i++ )
		ComboBox_SetCurSel(*i, nSelect);

	CCursor::BeginWaitCursor();
	m_pMainFrame->SetMotionStage(nSelect);
	CCursor::EndWaitCursor();
}

// ==========================================================================
//
UINT CMotionComboBox::GetMode()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_nMode = m_pMainFrame->GetMotionStage();
	return m_nMode;
}

/////////////////////////////////////////////////////////////////////////////
//
std::list<HWND> CMotionElevComboBox::m_hWndListElev;

CMotionElevComboBox::CMotionElevComboBox()
{
}

CMotionElevComboBox::~CMotionElevComboBox()
{
	if ( !m_hWndListElev.empty() )
		m_hWndListElev.pop_back();
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CMotionElevComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, CMotionElevComboBox::OnSelchange)
END_MESSAGE_MAP()

// ==========================================================================
//
void CMotionElevComboBox::PreSubclassWindow()
{
	m_hWndListElev.push_back(m_hWnd);

	ResetContent();

	///// Load File /////
	ConfigFile_GetNanoSpecIni(&m_SystemConfig2, CONFIG_FILE_SYSTEM_CONFIG);

	int nIndex;
	// Kojika 20090528 Add
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	// Kojika 20090528 Add End

	// Kojika 20090528 Change
	//PCSTR pszString[] = MOTION_MODE_ELEVATOR_STR_INIT;
	//if(m_SystemConfig2.nStageType == STAGE_TYPE_SPT){
	//	for ( int index = 0; index < STAGE_MOTION_MAX-2; index++ ){
	//		nIndex = AddString(pszString[index]);
	//		SetItemData(nIndex, index);
	//	}
	//}else{
	//	for ( int index = 0; index < (STAGE_MOTION_MAX-3); index++ ){
	//	  nIndex = AddString(pszString[index]);
	//	  SetItemData(nIndex, index);
	//	  }
	//}

	if(l_SystemConfig.nLanguage == 0){
		PCSTR pszString[] = MOTION_MODE_ELEVATOR_STR_INIT_ENU;
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		if(m_SystemConfig2.nStageType == STAGE_TYPE_SPT){
//			for ( int index = 0; index < STAGE_MOTION_MAX-2; index++ ){
//				nIndex = AddString(pszString[index]);
//				SetItemData(nIndex, index);
//	}
//		}else{
// 2013.11.08 Bagus Del (TohoSpec対応) <--
			for ( int index = 0; index < (STAGE_MOTION_MAX-3); index++ ){
				nIndex = AddString(pszString[index]);
				SetItemData(nIndex, index);
			}
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		}
// 2013.11.08 Bagus Del (TohoSpec対応) <--
	}
	else{
		PCSTR pszString[] = MOTION_MODE_ELEVATOR_STR_INIT_JPN;
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		if(m_SystemConfig2.nStageType == STAGE_TYPE_SPT){
//			for ( int index = 0; index < STAGE_MOTION_MAX-2; index++ ){
//				nIndex = AddString(pszString[index]);
//				SetItemData(nIndex, index);
//			}
//		}
//		else{
// 2013.11.08 Bagus Del (TohoSpec対応) <--
			for ( int index = 0; index < (STAGE_MOTION_MAX-3); index++ ){
				nIndex = AddString(pszString[index]);
				SetItemData(nIndex, index);
			}
// 2013.11.08 Bagus Del (TohoSpec対応) -->
//		}
// 2013.11.08 Bagus Del (TohoSpec対応) <--
	}
	// Kojika 20090528 Change End

	SetCurSel(GetElevMode());
}

// ==========================================================================
//
BOOL CMotionElevComboBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckIL() )	return TRUE;
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// ダブルクリック時に誤動作する事があるので排除する
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
BOOL CMotionElevComboBox::CheckIL()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();
	//Saiki 20090601 Add ----->
	CString strMsg;
	//Saiki 20090601 Add <-----
	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// メンテナンス異常
	}
	else{												// 通常動作
	}
	//Saiki 20090601 Change ----->
	if( m_pDoc->ActuateFlagsGet(ACTUATE_ZAXIS) ){
		LoadStringML(IDS_Z_AXIS_WORKING, strMsg, "Z Axis is moving.");
		m_pDoc->MessageStringIf_Set(strMsg);
		return FALSE;
	}	 // Ｚ軸動作中
	//Saiki 20090601 Change <-----
	// ▲インターロック条件▲

	return TRUE;
}

// ==========================================================================
//
void CMotionElevComboBox::OnSelchange()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();

	int nSelect = GetCurSel();
	if ( nSelect != CB_ERR ) {
		m_nElevMode = nSelect;
	}
	for ( std::list<HWND>::iterator i = m_hWndListElev.begin(); i != m_hWndListElev.end(); i++ )
		ComboBox_SetCurSel(*i, nSelect);

	CCursor::BeginWaitCursor();
	m_pMainFrame->SetMotionElev(nSelect);
	CCursor::EndWaitCursor();
}

// ==========================================================================
//
UINT CMotionElevComboBox::GetElevMode()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	m_nElevMode = m_pMainFrame->GetMotionElev();
	return m_nElevMode;
}

/////////////////////////////////////////////////////////////////////////////
//
//std::list<HWND> CJoyStickRadioButton::m_hWndList[JOY_RADIO_MAX];
std::list<HWND> CJoyStickRadioButton::m_hWndList[JOY_MODE_MAX];

// ==========================================================================
// 引数：indexはRadioボタンのグループ内の番号
// ==========================================================================
CJoyStickRadioButton::CJoyStickRadioButton(int index)
{
	m_nIndex = index;
}

CJoyStickRadioButton::~CJoyStickRadioButton()
{
	if ( !m_hWndList[m_nIndex].empty() )
		m_hWndList[m_nIndex].pop_back();
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CJoyStickRadioButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, CJoyStickRadioButton::OnJoyStick)
END_MESSAGE_MAP()

// ==========================================================================
//
void CJoyStickRadioButton::PreSubclassWindow()
{
	m_hWndList[m_nIndex].push_back(m_hWnd);

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	int m_nMode = m_pMainFrame->GetJoyStickMode();
	if( m_nMode == m_nIndex ){
		::SendMessage( m_hWnd, BM_SETCHECK, BST_CHECKED, 0);
	}
	else{
		::SendMessage( m_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
	}
}

// ==========================================================================
//
BOOL CJoyStickRadioButton::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckChangeMode() )	return TRUE;
			if( !CheckIL() )			return TRUE;
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// ダブルクリック時に誤動作する事があるので排除する
			return TRUE;
		}
	}
	return CButton::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
BOOL CJoyStickRadioButton::CheckChangeMode()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// 切替禁止なら処理しない
	if( !m_pDoc->GetJoyStickSelectEnabled() ) return FALSE;

	return TRUE;
}

// ==========================================================================
//
BOOL CJoyStickRadioButton::CheckIL()
{
	//Saiki 20090601 Add ----->
	CString strMsg;
	//Saiki 20090601 Add <-----
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// メンテナンス異常
	}
	else{												// 通常動作
	}
	// 各I/Oチェック
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->IsInterLock() == TRUE){return FALSE;}
	/*//Saiki 20090520 Change <-----*/
	if( m_pMainFrame->GetJoyStickMode()==0 ){			// 現在はソフトジョイ（⇒ハード）
		/*//Saiki 20090520 Change ----->*/
		if(m_pDoc->CheckUnitStatus() == TRUE){return FALSE;}
		/*//Saiki 20090520 Change <-----*/
	}
	else{												// 現在はハードジョイ（⇒ソフト）
	}
	// シャッター以外が動いていたら動作禁止
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		//Saiki 20090531 Change ----->
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return FALSE;
		}	 // ステージ動作中
		//Saiki 20090531 Change <-----
	}
	/*//Saiki 20090520 Change ----->*/
	if(m_pDoc->CheckActiveFlag() == TRUE){return FALSE;}
	/*//Saiki 20090520 Change <-----*/
	// ▲インターロック条件▲

	return TRUE;
}

// ==========================================================================
//
void CJoyStickRadioButton::OnJoyStick()
{
BOOL bJoyStk=FALSE;

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	int m_nMode = m_pMainFrame->GetJoyStickMode();

	// 変更がなければ処理しない
	if( m_nMode == m_nIndex ) return;

	// ジョイスティック切替
	if( m_nMode == 0 ){ 								// 現在はソフトジョイ
		// ジョイスティック許可
		bJoyStk = m_pDoc->ChangeJoyStick(TRUE);
		if( bJoyStk ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);
		}
	}
	else{												// 現在はハードジョイ
		// ジョイスティック禁止
		bJoyStk = m_pDoc->ChangeJoyStick(FALSE);
		if( bJoyStk ){
			m_pDoc->ActuateFlagsSet(ACTUATE_XYSTAGE, FALSE);
		}
	}

	if( bJoyStk != TRUE ){
//		MessageBox("ジョイスティック切替失敗 !!!.", "JOYSTICK", MB_OK|MB_ICONWARNING);
//		GetParent()->PostMessage(WM_JOYSTICK_NG, 0L, 0L);
// DEL 2009.06.01
//		AlarmIf_Set(ALID_SubControllerReplayAbnormal);
// DEL 2009.06.01
// 2009.08.07 K.Matsuo Bug Fix SPTステージ減速停止中にJOYSTICK変更 -->
		GetParent()->PostMessage(WM_JOYSTICK, 0L, 0L);
// 2009.08.07 K.Matsuo Bug Fix SPTステージ減速停止中にJOYSTICK変更 <--
		return;
	}


//	for( int index=0; index<JOY_RADIO_MAX; index++ ){
	for( int index=0; index<JOY_MODE_MAX; index++ ){
		// 元の選択ボタンのチェックを外す
		if( index == m_nMode ){
			for ( std::list<HWND>::iterator i = m_hWndList[index].begin(); i != m_hWndList[index].end(); i++ )
				if( *i != m_hWnd ) ::SendMessage( *i, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		// 新しい選択ボタンをチェックする
		if( index == m_nIndex ){
			for ( std::list<HWND>::iterator i = m_hWndList[index].begin(); i != m_hWndList[index].end(); i++ )
				if( *i != m_hWnd ) ::SendMessage( *i, BM_SETCHECK, BST_CHECKED, 0);
		}
	}

	m_pMainFrame->SetJoyStickMode( m_nIndex );

	GetParent()->PostMessage(WM_JOYSTICK, 0L, 0L);
}

// ==========================================================================
//
void CJoyStickRadioButton::Checked()
{
	::SendMessage( m_hWnd, BM_SETCHECK, BST_CHECKED, 0);
}

// ==========================================================================
//
void CJoyStickRadioButton::UnChecked()
{
	::SendMessage( m_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
}

// 2013.02.22 bagus Substrate thickness setting -->
/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessComboBox
std::list<HWND> CSubstrateThicknessComboBox::m_hWndList;

CSubstrateThicknessComboBox::CSubstrateThicknessComboBox()
{
}

CSubstrateThicknessComboBox::~CSubstrateThicknessComboBox()
{
	if ( !m_hWndList.empty() )
		m_hWndList.pop_back();
}

// ==========================================================================
//
BEGIN_MESSAGE_MAP(CSubstrateThicknessComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, CSubstrateThicknessComboBox::OnSelchange)
	ON_MESSAGE(WM_SELSUBTHICKCHANGE, OnSelectSubstrateThicknessNotify)
END_MESSAGE_MAP()

// ==========================================================================
//
LRESULT CSubstrateThicknessComboBox::OnSelectSubstrateThicknessNotify(WPARAM wParam, LPARAM lParam)
{
	int iIndex = NS_GetCurrentSubstrateThickness();
	int nCount = GetCount();

	for ( int i = 0; i < nCount; i++ ) {
		if ( GetItemData(i) == iIndex ) {
			SetCurSel(i);
			break;
		}
	}
	return 0L;
}

// =========================================================================
//
double CSubstrateThicknessComboBox::MilliToMicro(double dMilli)
{
	return dMilli * 1000.0;
}

// =========================================================================
//
double CSubstrateThicknessComboBox::MicroToMilli(double dMicro)
{
	return dMicro / 1000.0;
}

// ==========================================================================
//
void CSubstrateThicknessComboBox::PreSubclassWindow()
{
	char szBuff[256];
	int iSel;
	double dTemp;
	SUBSTRATE_THICKNESS substrateThickness;

	m_hWndList.push_back(m_hWnd);

	ResetContent();

	///// Load File /////
	SUBSTRATE_THICKNESS_TABLE substrateThicknessTable;
	ConfigFile_GetNanoSpecIni(&substrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);

	for ( int i = 0; i < SUBSTRATE_THICKNESS_MAX; i++) {
		substrateThickness = substrateThicknessTable.SubstrateThickness[i];
		if ( substrateThickness.bUseData ) {
			if ( i == 0 ) {
				strcpy(szBuff, "Reference");
			}
			else {
				dTemp = MicroToMilli(substrateThickness.dThickness);
				sprintf(szBuff, "%.2lfmm", dTemp);
			}
			iSel = AddString(szBuff);
			SetItemData(iSel, i);
		}
	}

	::PostMessage(m_hWnd, WM_SELSUBTHICKCHANGE, 0L, 0L);
	CComboBox::PreSubclassWindow();
}

// ==========================================================================
//
BOOL CSubstrateThicknessComboBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->hwnd == m_hWnd ){
		if( pMsg->message == WM_LBUTTONDOWN ){
			if( !CheckIL() )	return TRUE;
		}
		else if( pMsg->message == WM_LBUTTONDBLCLK ){
			// ダブルクリック時に誤動作する事があるので排除する
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

// ==========================================================================
//
BOOL CSubstrateThicknessComboBox::CheckIL()
{
	CString strMsg;

	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	//リモート時は何もしない
	if(m_pDoc->GetHostMode() == HOST_REMOTE) return FALSE;

	// ▼インターロック条件▼
	if( nexioIsEngineerMaintenanceSwitch()==ON ){		// 東朋メンテナンス
		if( nexioIsMaintenanceSwitch() != OFF ){	AlarmIf_Set(ALID_EngineerMaintenanceSwitchOn);	return FALSE;}	// メンテナンス異常
	}
	else{												// 通常動作
	}
	if( m_pMainFrame->GetJoyStickMode()==0 ){
		if( m_pDoc->ActuateFlagsGet(ACTUATE_XYSTAGE) ){
			LoadStringML(IDS_STAGE_WORKING, strMsg, "Stage is moving.");
			m_pDoc->MessageStringIf_Set(strMsg);
			return FALSE;
		}	 // ステージ動作中
	}
	// ▲インターロック条件▲

	return TRUE;
}

// ==========================================================================
//
void CSubstrateThicknessComboBox::OnSelchange()
{
	CMainFrame* m_pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CNanoSpecDoc* m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	// 選択カーソルの同期
	int nSelect = GetCurSel();
	for ( std::list<HWND>::iterator i = m_hWndList.begin(); i != m_hWndList.end(); i++ ) {
		ComboBox_SetCurSel(*i, nSelect);
	}

	// 板厚登録
	int nItemData = GetItemData(nSelect);
	CCursor::BeginWaitCursor();
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);
	NS_SelectSubstrateThickness(nItemData);
	m_pDoc->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);
	CCursor::EndWaitCursor();
}
// 2013.02.22 bagus Substrate thickness setting <--
