// SubstrateThicknessSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SubstrateThicknessSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SUB_THICKNESS_MIN		(300)			// unit:micro meter
#define SUB_THICKNESS_MAX		(1500)			// unit:micro meter
#define INVALID_REGION_MIN		(10)			// unit:micro meter
#define INVALID_REGION_MAX		(10000)			// unit:micro meter

/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessSettingDlg ダイアログ


CSubstrateThicknessSettingDlg::CSubstrateThicknessSettingDlg(BOOL bNew, CWnd* pParent /*=NULL*/)
	: CDialog(CSubstrateThicknessSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubstrateThicknessSettingDlg)
	//}}AFX_DATA_INIT
	m_bNew = bNew;
	::ZeroMemory(&m_SubstrateThickness, sizeof(m_SubstrateThickness));
}


void CSubstrateThicknessSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubstrateThicknessSettingDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_NO_EDIT, m_wndNoEdit);
	DDX_Control(pDX, IDC_SUBSTRATE_THICKNESS_EDIT, m_wndSubstrateThicknessEdit);
	DDX_Control(pDX, IDC_INVALID_REGION_EDIT, m_wndInvalidRegionEdit);
}


BEGIN_MESSAGE_MAP(CSubstrateThicknessSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSubstrateThicknessSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessSettingDlg メッセージ ハンドラ

BOOL CSubstrateThicknessSettingDlg::OnInitDialog()
{
	double dTemp;

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	// 入力文字数制限
	const int iLimitLength = 10;
	((CEdit*)GetDlgItem(IDC_SUBSTRATE_THICKNESS_EDIT))->SetLimitText(iLimitLength);
	((CEdit*)GetDlgItem(IDC_INVALID_REGION_EDIT))->SetLimitText(iLimitLength);

	// 入力制限（No）
	m_wndNoEdit.EnableWindow(FALSE);

	// データの設定
	CString strText;
	if(m_iNo == 0){
		strText = _T("Reference");
	}
	else{
		strText.Format("%d", m_iNo);
	}
	m_wndNoEdit.SetWindowText(strText);
	dTemp = MicroToMilli(m_SubstrateThickness.dThickness);
	strText.Format("%.2lf", dTemp);
	m_wndSubstrateThicknessEdit.SetWindowText(strText);
	dTemp = MicroToMilli(m_SubstrateThickness.dInvalidRegion);
	strText.Format("%.2lf", dTemp);
	m_wndInvalidRegionEdit.SetWindowText(strText);


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
double CSubstrateThicknessSettingDlg::MilliToMicro(double dMilli)
{
	return dMilli * 1000.0;
}

// =========================================================================
//
double CSubstrateThicknessSettingDlg::MicroToMilli(double dMicro)
{
	return dMicro / 1000.0;
}

// =========================================================================
//
void CSubstrateThicknessSettingDlg::OnOK()
{
	CString	strBuffer;
	CString	strFormat;
	CString strText;
	double dTempThickness;
	double dTempRegion;

	m_wndSubstrateThicknessEdit.GetWindowText(strText);
	dTempThickness = MilliToMicro(strtod(strText, NULL));
	m_wndInvalidRegionEdit.GetWindowText(strText);
	dTempRegion = MilliToMicro(strtod(strText, NULL));

	// check data
	if ( dTempThickness < SUB_THICKNESS_MIN || SUB_THICKNESS_MAX < dTempThickness ) {
		LoadStringML(IDS_SUBTHICK_OUT_OF_RANGE, strFormat, "Substrate thickness is out of range.\n(%.2lf - %.2lf)");
		strBuffer.Format((LPCTSTR)strFormat, MicroToMilli(SUB_THICKNESS_MIN), MicroToMilli(SUB_THICKNESS_MAX));
		AfxMessageBox(strBuffer);
		return;
	}
	if ( dTempRegion < INVALID_REGION_MIN || INVALID_REGION_MAX < dTempRegion ) {
		LoadStringML(IDS_INVALID_REGION_OUT_OF_RANGE, strFormat, "Invalid region is out of range.\n(%.2lf - %.2lf)");
		strBuffer.Format((LPCTSTR)strFormat, MicroToMilli(INVALID_REGION_MIN), MicroToMilli(INVALID_REGION_MAX));
		AfxMessageBox(strBuffer);
		return;
	}

	//
	m_SubstrateThickness.dThickness = dTempThickness;
	m_SubstrateThickness.dInvalidRegion = dTempRegion;
	m_SubstrateThickness.bUseData = TRUE;

	CDialog::OnOK();
}

// =========================================================================
//
void CSubstrateThicknessSettingDlg::OnCancel()
{
	CDialog::OnCancel();
}
