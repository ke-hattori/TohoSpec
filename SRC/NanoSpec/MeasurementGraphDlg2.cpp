// MeasurementGraphDlg2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "MeasurementGraphDlg.h"
#include "MeasurementGraphDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CMeasurementGraphDlg2 ダイアログ


CMeasurementGraphDlg2::CMeasurementGraphDlg2(int m_mode, CWnd* pParent /*=NULL*/)
	: CMeasurementGraphDlg(m_mode, CMeasurementGraphDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasurementGraphDlg2)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT


}


void CMeasurementGraphDlg2::DoDataExchange(CDataExchange* pDX)
{
	CMeasurementGraphDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasurementGraphDlg2)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasurementGraphDlg2, CMeasurementGraphDlg)
	//{{AFX_MSG_MAP(CMeasurementGraphDlg2)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasurementGraphDlg2 メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
BOOL CMeasurementGraphDlg2::OnInitDialog()
{

	CMeasurementGraphDlg::OnInitDialog();



//	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
//	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

//	m_pDoc->GetRcpData(&m_rcp_data);

/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	if (m_pDoc->GetMeasMode() == MEASMODE_TEST) {
		CWnd* l_pcwndParent = this->GetParent();			// CTabCtrl
		RECT l_rectParentAR;	l_pcwndParent->GetWindowRect(&l_rectParentAR);
		/* 親ウィンドウをタブと決め付けた処理です．	*/
		((CTabCtrl*) l_pcwndParent)->AdjustRect(FALSE, &l_rectParentAR);
		RECT l_rectThisDlg;	this->GetWindowRect(&l_rectThisDlg);
		if ((g_lModelType == MODEL_T3100) &&
			((l_rectParentAR.bottom - l_rectParentAR.top) < (l_rectThisDlg.bottom - l_rectThisDlg.top))) {
			/* オフセット計算	*/
			long l_lDeltaY = (l_rectThisDlg.bottom - l_rectThisDlg.top) - (l_rectParentAR.bottom - l_rectParentAR.top);
			/* テスト測定の場合に移動サイズが合わないための対策です．	*/
			l_lDeltaY += 5;
			this->ChgSizeCtrl(-l_lDeltaY);
		}
	}
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

