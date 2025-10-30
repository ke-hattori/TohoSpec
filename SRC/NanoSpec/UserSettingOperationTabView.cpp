// UserSettingOperationTabView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "UserSettingOperationCommonSettingDlg.h"
#include "UserSettingOperationSrSettingDlg.h"
//2009.09.03 bagus se --{--
#include "UserSettingOperationSeSettingDlg.h"
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
#include "UserSettingOperationCompEASESettingDlg.h"
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
#include "UserSettingOperationCTASettingDlg.h"
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
#include "UserSettingOperationMSSettingDlg.h"
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
#include "UserSettingOperationStressSettingDlg.h"
//2009.10.21 bagus Stress 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
#include "UserSettingOperationRSSettingDlg.h"
// 2009.11.04 bagus RS 追加 --}--
#include "UserSettingOperationTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CUserSettingOperationTabView

// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CUserSettingOperationTabView, CNanoUI)

// =========================================================================
//
CUserSettingOperationTabView::CUserSettingOperationTabView()
	: CNanoUI(CUserSettingOperationTabView::IDD)
{
	//{{AFX_DATA_INIT(CUserSettingOperationTabView)
	//}}AFX_DATA_INIT
	memset(m_iHeadType, 0, sizeof(m_iHeadType));
	m_pUserSettingOperationCommonSettingDlg = NULL;
	m_pUserSettingOperationSrSettingDlg = NULL;
//2009.09.03 bagus se --{--
	m_pUserSettingOperationSeSettingDlg = NULL;
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	m_pUserSettingOperationCompEASESettingDlg = NULL;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
	m_pUserSettingOperationCTASettingDlg = NULL;
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
	m_pUserSettingOperationMSSettingDlg = NULL;
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
	m_pUserSettingOperationStressSettingDlg = NULL;
//2009.10.21 bagus Stress 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	m_pUserSettingOperationRSSettingDlg = NULL;
// 2009.11.04 bagus RS 追加 --}--
}

// =========================================================================
//
CUserSettingOperationTabView::~CUserSettingOperationTabView()
{
	if(m_pUserSettingOperationCommonSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationCommonSettingDlg->DestroyWindow();
		delete m_pUserSettingOperationCommonSettingDlg;
		m_pUserSettingOperationCommonSettingDlg = NULL;
	}

	if(m_pUserSettingOperationSrSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationSrSettingDlg->DestroyWindow();
		delete m_pUserSettingOperationSrSettingDlg;
		m_pUserSettingOperationSrSettingDlg = NULL;
	}
	//2009.09.03 bagus se --{--
	if(m_pUserSettingOperationSeSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationSeSettingDlg->DestroyWindow();
		delete m_pUserSettingOperationSeSettingDlg;
		m_pUserSettingOperationSeSettingDlg = NULL;
	}
	//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	if(m_pUserSettingOperationCompEASESettingDlg){
		delete m_pUserSettingOperationCompEASESettingDlg;
		m_pUserSettingOperationCompEASESettingDlg = NULL;
	}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
	if(m_pUserSettingOperationCTASettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationCTASettingDlg->DestroyWindow();
		delete m_pUserSettingOperationCTASettingDlg;
		m_pUserSettingOperationCTASettingDlg = NULL;
	}
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
	if(m_pUserSettingOperationMSSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationMSSettingDlg->DestroyWindow();
		delete m_pUserSettingOperationMSSettingDlg;
		m_pUserSettingOperationMSSettingDlg = NULL;
	}
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
	if(m_pUserSettingOperationStressSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationStressSettingDlg->DestroyWindow();
		delete m_pUserSettingOperationStressSettingDlg;
		m_pUserSettingOperationStressSettingDlg = NULL;
	}
//2009.10.21 bagus Stress 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	if(m_pUserSettingOperationRSSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pUserSettingOperationRSSettingDlg->DestroyWindow();
		delete m_pUserSettingOperationRSSettingDlg;
		m_pUserSettingOperationRSSettingDlg = NULL;
	}
// 2009.11.04 bagus RS 追加 --}--
}

// =========================================================================
//
void CUserSettingOperationTabView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserSettingOperationTabView)
	DDX_Control(pDX, IDC_USER_SETTING_OPERATION_TAB, m_UserSettingOperationTab);
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CUserSettingOperationTabView, CNanoUI)
	//{{AFX_MSG_MAP(CUserSettingOperationTabView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_USER_SETTING_OPERATION_TAB, OnSelchangeUserSettingOperationTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CUserSettingOperationTabView 診断

#ifdef _DEBUG
// =========================================================================
//
void CUserSettingOperationTabView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CUserSettingOperationTabView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CUserSettingOperationTabView メッセージ ハンドラ

// =========================================================================
//
void CUserSettingOperationTabView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	Tab_Init();
}

// =========================================================================
//
void CUserSettingOperationTabView::Tab_Init()
{
	int Index;
	int dx = 5;
	int dy = 24;
	int iTabRowCount, iTabItemCount = 0;
	HEAD_TYPE_CONFIG HeadType;

	// Kojika 20090529 Add
	CString l_strBuffer;
	// Kojika 20090529 Add End

	ConfigFile_GetNanoSpecIni(&HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);

	m_UserSettingOperationTab.DeleteAllItems();
	memset(m_iHeadType, 0, sizeof(m_iHeadType));
	///// TAB Item /////
	// Common
	// Kojika 20090529 Change
	//m_UserSettingOperationTab.InsertItem(iTabItemCount, "COMMON");
	//m_UserSettingOperationTab.InsertItem(iTabItemCount, "共通項目");
	LoadStringML(IDS_COMMON_TAB, l_strBuffer, "COMMON");
	m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
	// Kojika 20090529 Change End
	iTabItemCount++;
	// SR
	if(HeadType.bSR){
		// Kojika 20090603 Change
		//m_UserSettingOperationTab.InsertItem(iTabItemCount, "SR");
		LoadStringML(IDS_SR_TAB, l_strBuffer, "SR");
		m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
		// Kojika 20090603 Change End
		m_iHeadType[iTabItemCount] = HEAD_TYPE_SR;
		iTabItemCount++;
	}
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		// SE
		if(HeadType.bSE){
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
			if(HeadType.bCompEASE){
				LoadStringML(IDS_COMPEASE_TAB, l_strBuffer, "EASE");
				m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
				m_iHeadType[iTabItemCount] = HEAD_TYPE_COMPEASE;
			}
			else {
			// Kojika 20090603 Change
			//m_UserSettingOperationTab.InsertItem(iTabItemCount, "SE");
			LoadStringML(IDS_SE_TAB, l_strBuffer, "SE");
			m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			// Kojika 20090603 Change End
			m_iHeadType[iTabItemCount] = HEAD_TYPE_SE;
			}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			iTabItemCount++;
		}
// 2009.10.19 bagus MS 追加 --{--
#if 0
		// IRSE
		if(HeadType.bIRSE){
			// Kojika 20090603 Change
			//m_UserSettingOperationTab.InsertItem(iTabItemCount, "IRSE");
			LoadStringML(IDS_IRSE_TAB, l_strBuffer, "IRSE");
			m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			// Kojika 20090603 Change End
			m_iHeadType[iTabItemCount] = HEAD_TYPE_IRSE;
			iTabItemCount++;
		}
#endif
// 2009.10.19 bagus MS 追加 --}--
		// 4PP
		if(HeadType.bResist){
			// Kojika 20090603 Change
			//m_UserSettingOperationTab.InsertItem(iTabItemCount, "4PP");
// 2009.11.04 bagus RS 追加 --{--
//			LoadStringML(IDS_4PP_TAB, l_strBuffer, "4PP");
			LoadStringML(IDS_4PP_TAB, l_strBuffer, "RS");
// 2009.11.04 bagus RS 追加 --}--
			m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			// Kojika 20090603 Change End
			m_iHeadType[iTabItemCount] = HEAD_TYPE_4PP;
			iTabItemCount++;
		}
		// Contact Angle
		if(HeadType.bCTA){
			// Kojika 20090603 Change
			//m_UserSettingOperationTab.InsertItem(iTabItemCount, "CA");
			LoadStringML(IDS_CTA_TAB, l_strBuffer, "CA");
			m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			// Kojika 20090603 Change End
			m_iHeadType[iTabItemCount] = HEAD_TYPE_CTA;
			iTabItemCount++;
		}
		// Stress
		if(HeadType.bStress){
			// Kojika 20090603 Change
			//m_UserSettingOperationTab.InsertItem(iTabItemCount, "Stress");
			LoadStringML(IDS_STRESS_TAB, l_strBuffer, "Stress");
			m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			// Kojika 20090603 Change End
			m_iHeadType[iTabItemCount] = HEAD_TYPE_STRESS;
			iTabItemCount++;
		}
// 2009.10.19 bagus MS 追加 --{--
		// MS
		if(HeadType.bMS){
			LoadStringML(IDS_MS_TAB, l_strBuffer, "MS");
			m_UserSettingOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_MS;
			iTabItemCount++;
		}
// 2009.10.19 bagus MS 追加 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	iTabRowCount = m_UserSettingOperationTab.GetRowCount();
	// タブの行数に応じてダイアログの表示位置を変更する必要がある
	switch(iTabRowCount){
	case 2:
		dy = 39;
		break;
	case 3:
		dy = 54;
		break;
	case 1:
	default:
		dy = 24;
		break;
	}

	// Tab レクトサイズの設定
	CRect rc;
	GetClientRect(rc);
	m_UserSettingOperationTab.SetWindowPos(&wndTop, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);

	///// Common /////
	if(m_pUserSettingOperationCommonSettingDlg == NULL){
		m_pUserSettingOperationCommonSettingDlg = new CUserSettingOperationCommonSettingDlg();
		m_pUserSettingOperationCommonSettingDlg->Create(CUserSettingOperationCommonSettingDlg::IDD, &m_UserSettingOperationTab);
	}
	m_pUserSettingOperationCommonSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
	///// SR /////
	if(m_pUserSettingOperationSrSettingDlg == NULL){
		m_pUserSettingOperationSrSettingDlg = new CUserSettingOperationSrSettingDlg();
		m_pUserSettingOperationSrSettingDlg->Create(CUserSettingOperationSrSettingDlg::IDD, &m_UserSettingOperationTab);
	}
	m_pUserSettingOperationSrSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		//2009.09.04 bagus se --{--
		///// SE /////
		if(m_pUserSettingOperationSeSettingDlg == NULL){
			m_pUserSettingOperationSeSettingDlg = new CUserSettingOperationSeSettingDlg();
			m_pUserSettingOperationSeSettingDlg->Create(CUserSettingOperationSeSettingDlg::IDD, &m_UserSettingOperationTab);
		}
		m_pUserSettingOperationSeSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//2009.09.04 bagus se --}--
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		///// CompleteEASE /////
		if(m_pUserSettingOperationCompEASESettingDlg == NULL){
			m_pUserSettingOperationCompEASESettingDlg = new CUserSettingOperationCompEASESettingDlg();
			m_pUserSettingOperationCompEASESettingDlg->Create(CUserSettingOperationCompEASESettingDlg::IDD, &m_UserSettingOperationTab);
		}
		m_pUserSettingOperationCompEASESettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		// 2009.10.08 bagus CTA 追加 --{--
		///// ContactAngle /////
		if(m_pUserSettingOperationCTASettingDlg == NULL){
			m_pUserSettingOperationCTASettingDlg = new CUserSettingOperationCTASettingDlg();
			m_pUserSettingOperationCTASettingDlg->Create(CUserSettingOperationCTASettingDlg::IDD, &m_UserSettingOperationTab);
		}
		m_pUserSettingOperationCTASettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2009.10.08 bagus CTA 追加 --}--
		//2009.10.20 bagus MS 追加 --{--
		///// MicroScope /////
		if(m_pUserSettingOperationMSSettingDlg == NULL){
			m_pUserSettingOperationMSSettingDlg = new CUserSettingOperationMSSettingDlg();
			m_pUserSettingOperationMSSettingDlg->Create(CUserSettingOperationMSSettingDlg::IDD, &m_UserSettingOperationTab);
		}
		m_pUserSettingOperationMSSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//2009.10.20 bagus MS 追加 --}--
		//2009.10.21 bagus Stress 追加 --{--
		///// Stress /////
		if(m_pUserSettingOperationStressSettingDlg == NULL){
			m_pUserSettingOperationStressSettingDlg = new CUserSettingOperationStressSettingDlg();
			m_pUserSettingOperationStressSettingDlg->Create(CUserSettingOperationStressSettingDlg::IDD, &m_UserSettingOperationTab);
		}
		m_pUserSettingOperationStressSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//2009.10.21 bagus Stress 追加 --}--
		// 2009.11.04 bagus RS 追加 --{--
		///// Resistance /////
		if(m_pUserSettingOperationRSSettingDlg == NULL){
			m_pUserSettingOperationRSSettingDlg = new CUserSettingOperationRSSettingDlg();
			m_pUserSettingOperationRSSettingDlg->Create(CUserSettingOperationRSSettingDlg::IDD, &m_UserSettingOperationTab);
		}
		m_pUserSettingOperationRSSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2009.11.04 bagus RS 追加 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Set Current Selection Item /////
	Index = m_UserSettingOperationTab.SetCurSel(0);
	TabChange(Index);
}

// =========================================================================
//
void CUserSettingOperationTabView::OnSelchangeUserSettingOperationTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index;

	Index = m_UserSettingOperationTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CUserSettingOperationTabView::TabChange(int Index)
{
	int i;
	///// TAB Item Highlight /////
	for(i = 0; i < m_UserSettingOperationTab.GetItemCount(); i++)
		m_UserSettingOperationTab.HighlightItem(i, FALSE);

	m_UserSettingOperationTab.HighlightItem(Index, TRUE);

	///// Hide All Dialog /////
	m_pUserSettingOperationCommonSettingDlg->ShowWindow(SW_HIDE);
	m_pUserSettingOperationSrSettingDlg->ShowWindow(SW_HIDE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
//2009.09.03 bagus se --{--
		m_pUserSettingOperationSeSettingDlg->ShowWindow(SW_HIDE);
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		m_pUserSettingOperationCompEASESettingDlg->ShowWindow(SW_HIDE);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
		m_pUserSettingOperationCTASettingDlg->ShowWindow(SW_HIDE);
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
		m_pUserSettingOperationMSSettingDlg->ShowWindow(SW_HIDE);
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
		m_pUserSettingOperationStressSettingDlg->ShowWindow(SW_HIDE);
//2009.10.21 bagus Stress 追加 --}--
//2009.11.09 bagus RS 修正 --{--
		m_pUserSettingOperationRSSettingDlg->ShowWindow(SW_HIDE);
//2009.11.09 bagus RS 修正 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Show Dialog /////
	switch(Index){
	case 0:
		m_pUserSettingOperationCommonSettingDlg->ShowWindow(SW_SHOW);
		break;
	default:
		ShowDialog(Index);
		break;
	}
}

// =========================================================================
//
void CUserSettingOperationTabView::ShowDialog(int iIndex)
{
	switch(m_iHeadType[iIndex]){
	case HEAD_TYPE_SR:
		m_pUserSettingOperationSrSettingDlg->ShowWindow(SW_SHOW);
		break;
////2009.09.03 bagus se --{--
	case HEAD_TYPE_SE:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pUserSettingOperationSeSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pUserSettingOperationSeSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pUserSettingOperationCompEASESettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pUserSettingOperationCompEASESettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
	case HEAD_TYPE_CTA:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pUserSettingOperationCTASettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pUserSettingOperationCTASettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
	case HEAD_TYPE_MS:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pUserSettingOperationMSSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pUserSettingOperationMSSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
	case HEAD_TYPE_STRESS:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pUserSettingOperationStressSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pUserSettingOperationStressSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
//2009.10.21 bagus Stress 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	case HEAD_TYPE_4PP:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pUserSettingOperationRSSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pUserSettingOperationRSSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2009.11.04 bagus RS 追加 --}--
	default:
		break;
	}
}
