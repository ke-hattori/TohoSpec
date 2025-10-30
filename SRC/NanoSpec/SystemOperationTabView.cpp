// SystemOperationTabView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "SystemOperationCommonSettingDlg.h"
#include "SystemOperationSRSettingDlg.h"
//2009.08.16 bagus stress --{--
#include "SystemOperationStressSettingDlg.h"
//2009.08.16 bagus stress --}--
//2009.09.02 bagus se SE機能追加 --{--
#include "SystemOperationSeSettingDlg.h"
//2009.09.02 bagus se SE機能追加 --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
#include "SystemOperationCompEASESettingDlg.h"
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.07 bagus CTA 追加 --{--
#include "SystemOperationCTASettingDlg.h"
// 2009.10.07 bagus CTA 追加 --}--
// 2009.10.19 bagus MS 追加 --{--
#include "SystemOperationMsSettingDlg.h"
// 2009.10.19 bagus MS 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
#include "SystemOperationRSSettingDlg.h"
// 2009.11.04 bagus RS 追加 --}--
#include "SystemOperationTabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSystemOperationTabView
// #########################################################################

// =========================================================================
//
IMPLEMENT_DYNCREATE(CSystemOperationTabView, CNanoUI)

// =========================================================================
//
CSystemOperationTabView::CSystemOperationTabView()
	: CNanoUI(CSystemOperationTabView::IDD)
{
	//{{AFX_DATA_INIT(CSystemOperationTabView)
	//}}AFX_DATA_INIT
	memset(m_iHeadType, 0, sizeof(m_iHeadType));
	m_pSystemOperationCommonSettingDlg = NULL;
	m_pSystemOperationSrSettingDlg = NULL;
	//2009.08.16 bagus stress --{--
	m_pSystemOperationStressSettingDlg = NULL;
	//2009.08.16 bagus stress --}--
	//2009.09.02 bagus se SE機能追加 --{--
	m_pSystemOperationSeSettingDlg = NULL;
	//2009.09.02 bagus se SE機能追加 --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	m_pSystemOperationCompEASESettingDlg = NULL;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	m_pSystemOperationCTASettingDlg = NULL;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.19 bagus MS 追加 --{--
	m_pSystemOperationMsSettingDlg = NULL;
	// 2009.10.19 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	m_pSystemOperationRSSettingDlg = NULL;
	// 2009.11.04 bagus RS 追加 --}--
}

// =========================================================================
//
CSystemOperationTabView::~CSystemOperationTabView()
{
	if(m_pSystemOperationCommonSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationCommonSettingDlg->DestroyWindow();
		delete m_pSystemOperationCommonSettingDlg;
		m_pSystemOperationCommonSettingDlg = NULL;
	}

	if(m_pSystemOperationSrSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationSrSettingDlg->DestroyWindow();
		delete m_pSystemOperationSrSettingDlg;
		m_pSystemOperationSrSettingDlg = NULL;
	}
	//2009.08.16 bagus stress --{--
	if(m_pSystemOperationStressSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationStressSettingDlg->DestroyWindow();
		delete m_pSystemOperationStressSettingDlg;
		m_pSystemOperationStressSettingDlg = NULL;
	}
	//2009.08.16 bagus stress --}--
	//2009.09.02 bagus se SE機能追加 --{--
	if(m_pSystemOperationSeSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationSteSettingDlg->DestroyWindow();
		delete m_pSystemOperationSeSettingDlg;
		m_pSystemOperationSeSettingDlg = NULL;
	}
	//2009.09.02 bagus se SE機能追加 --}--

	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	if(m_pSystemOperationCompEASESettingDlg){
		delete m_pSystemOperationCompEASESettingDlg;
		m_pSystemOperationCompEASESettingDlg = NULL;
	}
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	if(m_pSystemOperationCTASettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationCTASettingDlg->DestroyWindow();
		delete m_pSystemOperationCTASettingDlg;
		m_pSystemOperationCTASettingDlg = NULL;
	}
	// 2009.10.07 bagus CTA 追加 --}--

	// 2009.10.19 bagus MS 追加 --{--
	if(m_pSystemOperationMsSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationSteSettingDlg->DestroyWindow();
		delete m_pSystemOperationMsSettingDlg;
		m_pSystemOperationMsSettingDlg = NULL;
	}
	// 2009.10.19 bagus MS 追加 --}--

	// 2009.11.04 bagus RS 追加 --{--
	if(m_pSystemOperationRSSettingDlg){
// デストラクトの中でDestroyWindowを呼び出さない
//		m_pSystemOperationRSSettingDlg->DestroyWindow();
		delete m_pSystemOperationRSSettingDlg;
		m_pSystemOperationRSSettingDlg = NULL;
	}
	// 2009.11.04 bagus RS 追加 --}--
}

// =========================================================================
//
void CSystemOperationTabView::DoDataExchange(CDataExchange* pDX)
{
	CNanoUI::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemOperationTabView)
	DDX_Control(pDX, IDC_SYSTEM_OPERATION_TAB, m_SystemOperationTab);
	//}}AFX_DATA_MAP
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSystemOperationTabView, CNanoUI)
	//{{AFX_MSG_MAP(CSystemOperationTabView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SYSTEM_OPERATION_TAB, OnSelchangeSystemOperationTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSystemOperationTabView 診断

#ifdef _DEBUG
// =========================================================================
//
void CSystemOperationTabView::AssertValid() const
{
	CNanoUI::AssertValid();
}

// =========================================================================
//
void CSystemOperationTabView::Dump(CDumpContext& dc) const
{
	CNanoUI::Dump(dc);
}
#endif //_DEBUG

// =========================================================================
// CSystemOperationTabView メッセージ ハンドラ

// =========================================================================
//
void CSystemOperationTabView::OnInitialUpdate()
{
	CNanoUI::OnInitialUpdate();

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	// タブの初期化
	Tab_Init();
}

// =========================================================================
//
void CSystemOperationTabView::Tab_Init()
{
	int Index;
	int dx = 5;
	int dy = 24;
	int iTabRowCount, iTabItemCount = 0;
	HEAD_TYPE_CONFIG HeadType;

	// Kojika 20090603 Add
	CString l_strBuffer;
	// Kojika 20090603 Add End

	ConfigFile_GetNanoSpecIni(&HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);

	m_SystemOperationTab.DeleteAllItems();
	memset(m_iHeadType, 0, sizeof(m_iHeadType));
	// Common
	// Kojika 20090603 Change
	//m_SystemOperationTab.InsertItem(iTabItemCount, "COMMON");
	LoadStringML(IDS_COMMON_TAB, l_strBuffer, "COMMON");
	m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
	iTabItemCount++;
	// SR
	if(HeadType.bSR){
		//m_SystemOperationTab.InsertItem(iTabItemCount, "SR");
		LoadStringML(IDS_SR_TAB, l_strBuffer, "SR");
		m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
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
				m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
				m_iHeadType[iTabItemCount] = HEAD_TYPE_COMPEASE;
			}
			else{
			//m_SystemOperationTab.InsertItem(iTabItemCount, "SE");
			LoadStringML(IDS_SE_TAB, l_strBuffer, "SE");
			m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_SE;
			}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
			iTabItemCount++;
		}
// 2009.10.19 bagus MS 追加 --{--
#if 0
		// IRSE
		if(HeadType.bIRSE){
			//m_SystemOperationTab.InsertItem(iTabItemCount, "IRSE");
			LoadStringML(IDS_IRSE_TAB, l_strBuffer, "IRSE");
			m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_IRSE;
			iTabItemCount++;
		}
#endif
// 2009.10.19 bagus MS 追加 --}--
		// 4PP
		if(HeadType.bResist){
			//m_SystemOperationTab.InsertItem(iTabItemCount, "4PP");
// 2009.11.04 bagus RS 追加 --{--
//			LoadStringML(IDS_4PP_TAB, l_strBuffer, "4PP");
			LoadStringML(IDS_4PP_TAB, l_strBuffer, "RS");
// 2009.11.04 bagus RS 追加 --}--
			m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_4PP;
			iTabItemCount++;
		}
		// Contact Angle
		if(HeadType.bCTA){
			//m_SystemOperationTab.InsertItem(iTabItemCount, "CA");
			LoadStringML(IDS_CTA_TAB, l_strBuffer, "CA");
			m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_CTA;
			iTabItemCount++;
		}
		// Stress
		if(HeadType.bStress){
			//m_SystemOperationTab.InsertItem(iTabItemCount, "Stress");
			LoadStringML(IDS_STRESS_TAB, l_strBuffer, "Stress");
			m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_STRESS;
			iTabItemCount++;
		}
		// Kojika 20090603 Change End
// 2009.10.19 bagus MS 追加 --{--
		// MS
		if(HeadType.bMS){
			LoadStringML(IDS_MS_TAB, l_strBuffer, "MS");
			m_SystemOperationTab.InsertItem(iTabItemCount, l_strBuffer);
			m_iHeadType[iTabItemCount] = HEAD_TYPE_MS;
			iTabItemCount++;
		}
// 2009.10.19 bagus MS 追加 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Del (TohoSpec対応) <--

	iTabRowCount = m_SystemOperationTab.GetRowCount();
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
	m_SystemOperationTab.SetWindowPos(&wndTop, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);

	///// Common /////
	if(m_pSystemOperationCommonSettingDlg == NULL){
		m_pSystemOperationCommonSettingDlg = new CSystemOperationCommonSettingDlg();
		m_pSystemOperationCommonSettingDlg->Create(CSystemOperationCommonSettingDlg::IDD, &m_SystemOperationTab);
	}
	m_pSystemOperationCommonSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

	///// SR /////
	if(m_pSystemOperationSrSettingDlg == NULL){
		m_pSystemOperationSrSettingDlg = new CSystemOperationSrSettingDlg();
		m_pSystemOperationSrSettingDlg->Create(CSystemOperationSrSettingDlg::IDD, &m_SystemOperationTab);
	}
	m_pSystemOperationSrSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) <--
		//2009.08.16 bagus stress --{--
		///// Stress /////
		if(m_pSystemOperationStressSettingDlg == NULL){
			m_pSystemOperationStressSettingDlg = new CSystemOperationStressSettingDlg();
			m_pSystemOperationStressSettingDlg->Create(CSystemOperationStressSettingDlg::IDD, &m_SystemOperationTab);
		}
		m_pSystemOperationStressSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//2009.08.16 bagus stress --}--

		//2009.09.02 bagus se SE機能追加 --{--
		///// SE /////
		if(m_pSystemOperationSeSettingDlg == NULL){
			m_pSystemOperationSeSettingDlg = new CSystemOperationSeSettingDlg();
			m_pSystemOperationSeSettingDlg->Create(CSystemOperationSeSettingDlg::IDD, &m_SystemOperationTab);
		}
		m_pSystemOperationSeSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		//2009.09.02 bagus se SE機能追加 --}--

		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if(m_pSystemOperationCompEASESettingDlg == NULL){
			m_pSystemOperationCompEASESettingDlg = new CSystemOperationCompEASESettingDlg();
			m_pSystemOperationCompEASESettingDlg->Create(CSystemOperationCompEASESettingDlg::IDD, &m_SystemOperationTab);
		}
		m_pSystemOperationCompEASESettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--

		// 2009.10.07 bagus CTA 追加 --{--
		///// CTA /////
		if(m_pSystemOperationCTASettingDlg == NULL){
			m_pSystemOperationCTASettingDlg = new CSystemOperationCTASettingDlg();
			m_pSystemOperationCTASettingDlg->Create(CSystemOperationCTASettingDlg::IDD, &m_SystemOperationTab);
		}
		m_pSystemOperationCTASettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2009.10.07 bagus CTA 追加 --}--

		// 2009.10.19 bagus MS 追加 --{--
		if(m_pSystemOperationMsSettingDlg == NULL){
			m_pSystemOperationMsSettingDlg = new CSystemOperationMsSettingDlg();
			m_pSystemOperationMsSettingDlg->Create(CSystemOperationMsSettingDlg::IDD, &m_SystemOperationTab);
		}
		m_pSystemOperationMsSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2009.10.19 bagus MS 追加 --}--

		// 2009.11.04 bagus RS 追加 --{--
		if(m_pSystemOperationRSSettingDlg == NULL){
			m_pSystemOperationRSSettingDlg = new CSystemOperationRSSettingDlg();
			m_pSystemOperationRSSettingDlg->Create(CSystemOperationRSSettingDlg::IDD, &m_SystemOperationTab);
		}
		m_pSystemOperationRSSettingDlg->SetWindowPos(&wndTop, dx, dy, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		// 2009.11.04 bagus RS 追加 --}--
// 2013.11.08 Bagus Del (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Del (TohoSpec対応) <--

	///// Set Current Selection Item /////
	Index = m_SystemOperationTab.SetCurSel(0);
	TabChange(Index);
}

// =========================================================================
//
void CSystemOperationTabView::OnSelchangeSystemOperationTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int Index;

	Index = m_SystemOperationTab.GetCurSel();

	TabChange(Index);

	*pResult = 0;
}

// =========================================================================
//
void CSystemOperationTabView::TabChange(int Index)
{
	int i;

	for(i = 0; i < m_SystemOperationTab.GetItemCount(); i++)
		m_SystemOperationTab.HighlightItem(i, FALSE);

	m_SystemOperationTab.HighlightItem(Index, TRUE);

	///// Hide All Dialog /////
	m_pSystemOperationCommonSettingDlg->ShowWindow(SW_HIDE);
	m_pSystemOperationSrSettingDlg->ShowWindow(SW_HIDE);
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	if(g_lModelType != MODEL_T3100){
// 2013.11.08 Bagus Add (TohoSpec対応) -->
		m_pSystemOperationStressSettingDlg->ShowWindow(SW_HIDE);
		//2009.09.02 bagus se SE機能追加 --{--
		m_pSystemOperationSeSettingDlg->ShowWindow(SW_HIDE);
		//2009.09.02 bagus se SE機能追加 --}--
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		m_pSystemOperationCompEASESettingDlg->ShowWindow(SW_HIDE);
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		// 2009.10.07 bagus CTA 追加 --{--
		m_pSystemOperationCTASettingDlg->ShowWindow(SW_HIDE);
		// 2009.10.07 bagus CTA 追加 --}--
		// 2009.10.19 bagus MS 追加 --{--
		m_pSystemOperationMsSettingDlg->ShowWindow(SW_HIDE);
		// 2009.10.19 bagus MS 追加 --}--
		// 2009.11.04 bagus RS 追加 --{--
		m_pSystemOperationRSSettingDlg->ShowWindow(SW_HIDE);
		// 2009.11.04 bagus RS 追加 --}--
// 2013.11.08 Bagus Add (TohoSpec対応) -->
	}
// 2013.11.08 Bagus Add (TohoSpec対応) <--

	///// Show Dialog /////
	switch(Index){
	case 0:
		m_pSystemOperationCommonSettingDlg->ShowWindow(SW_SHOW);
		break;
	default:
		ShowDialog(Index);
		break;
	}
}

// =========================================================================
//
void CSystemOperationTabView::ShowDialog(int iIndex)
{
	switch(m_iHeadType[iIndex]){
	case HEAD_TYPE_SR:
		m_pSystemOperationSrSettingDlg->ShowWindow(SW_SHOW);
		break;
	case HEAD_TYPE_STRESS:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pSystemOperationStressSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pSystemOperationStressSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
//2009.09.02 bagus se SE機能追加 --{--
	case HEAD_TYPE_SE:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pSystemOperationSeSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pSystemOperationSeSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
//2009.09.02 bagus se SE機能追加 --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pSystemOperationCompEASESettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pSystemOperationCompEASESettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.07 bagus CTA 追加 --{--
	case HEAD_TYPE_CTA:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pSystemOperationCTASettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pSystemOperationCTASettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2009.10.07 bagus CTA 追加 --}--
// 2009.10.19 bagus MS 追加 --{--
	case HEAD_TYPE_MS:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pSystemOperationMsSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pSystemOperationMsSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2009.10.19 bagus MS 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	case HEAD_TYPE_4PP:
// 2013.11.08 Bagus Mod (TohoSpec対応) -->
//		m_pSystemOperationRSSettingDlg->ShowWindow(SW_SHOW);
		if(g_lModelType != MODEL_T3100){
			m_pSystemOperationRSSettingDlg->ShowWindow(SW_SHOW);
		}
// 2013.11.08 Bagus Mod (TohoSpec対応) <--
		break;
// 2009.11.04 bagus RS 追加 --}--
	default:
		break;
	}
}

