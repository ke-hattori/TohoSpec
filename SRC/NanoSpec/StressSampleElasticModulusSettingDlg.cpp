// StressSampleElasticModulusSettingDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
// 2009.09.04 bagus stress e+11変更 --{--
#include <math.h>
// 2009.09.04 bagus stress e+11変更 --}--
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "StressSampleElasticModulusSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStressSampleElasticModulusSettingDlg ダイアログ


CStressSampleElasticModulusSettingDlg::CStressSampleElasticModulusSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressSampleElasticModulusSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressSampleElasticModulusSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_bNewItem = FALSE;
}


void CStressSampleElasticModulusSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressSampleElasticModulusSettingDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CStressSampleElasticModulusSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CStressSampleElasticModulusSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressSampleElasticModulusSettingDlg メッセージ ハンドラ

void CStressSampleElasticModulusSettingDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください
	if (!CheckData())
		return;

	CString	sText;

	GetDlgItemText(IDC_MATERIAL_NAME, sText);
	sText.TrimLeft();
	sText.TrimRight();
	if (sText.GetLength() < 1) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:名称が空");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_MATERIAL_NAME, strBuffer, "Please input material name.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}
	strcpy(m_StressModulus.tszElasticModulusName, sText.GetBuffer(0));

	GetDlgItemText(IDC_MATERIAL_ELASTIC_MODULUS, sText);
	// 2009.09.04 bagus stress e+11変更 --{--
	//m_StressModulus.dElasticModulusValue = strtod(sText.GetBuffer(0), NULL);
	m_StressModulus.dElasticModulusValue = (strtod(sText.GetBuffer(0), NULL) * pow(10,11));
	// 2009.09.04 bagus stress e+11変更 --}--

	CDialog::OnOK();
}

BOOL CStressSampleElasticModulusSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Limit Text /////
	((CEdit*)GetDlgItem(IDC_MATERIAL_NAME))->SetLimitText(MATERIAL_NAME_LEN);

	///// Value /////
	CString	strBuffer;

	if (m_bNewItem) {
		strBuffer = "";
		SetDlgItemText(IDC_MATERIAL_NAME, strBuffer);
		SetDlgItemText(IDC_MATERIAL_ELASTIC_MODULUS, strBuffer);
	} else {
		strBuffer.Format("%s", m_StressModulus.tszElasticModulusName);
		SetDlgItemText(IDC_MATERIAL_NAME, strBuffer);
		// 2009.09.04 bagus stress e+11変更 --{--
		//strBuffer.Format("%e", m_StressModulus.dElasticModulusValue);
		strBuffer.Format("%f", m_StressModulus.dElasticModulusValue / pow(10,11));
		// 2009.09.04 bagus stress e+11変更 --}--
		SetDlgItemText(IDC_MATERIAL_ELASTIC_MODULUS, strBuffer);
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CStressSampleElasticModulusSettingDlg::CheckData()
{
	CString	sText;

	GetDlgItemText(IDC_MATERIAL_NAME, sText);
	sText.TrimLeft();
	sText.TrimRight();
	if (sText.GetLength() < 1) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:名称が空");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_INPUT_MATERIAL_NAME, strBuffer, "Please input material name.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return FALSE;
	}
	strcpy(m_StressModulus.tszElasticModulusName, sText.GetBuffer(0));

	GetDlgItemText(IDC_MATERIAL_ELASTIC_MODULUS, sText);
	if(CheckMinMaxDouble(sText, 0, 10000) != CHECK_DATA_OK){
		GetDlgItem(IDC_MATERIAL_ELASTIC_MODULUS)->SetFocus();
		return FALSE;
	}
	// 2009.09.04 bagus stress e+11変更 --{--
	//m_StressModulus.dElasticModulusValue = strtod(sText.GetBuffer(0), NULL);
	m_StressModulus.dElasticModulusValue = (strtod(sText.GetBuffer(0), NULL) * pow(10,11));
	// 2009.09.04 bagus stress e+11変更 --}--

	return TRUE;
}
