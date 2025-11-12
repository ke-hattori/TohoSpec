// StressElasticModulusListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
// 2009.09.04 bagus stress e+11変更 --{--
#include <math.h>
// 2009.09.04 bagus stress e+11変更 --}--
#include "StressElasticModulusListDlg.h"
#include "StressSampleElasticModulusSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStressElasticModulusListDlg ダイアログ


CStressElasticModulusListDlg::CStressElasticModulusListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressElasticModulusListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressElasticModulusListDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CStressElasticModulusListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressElasticModulusListDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_NEW_BUTTON, m_NewButton);
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);

	DDX_Control(pDX, IDC_MODULUS_LIST, m_ElasticModulusListCtrl);
}


BEGIN_MESSAGE_MAP(CStressElasticModulusListDlg, CDialog)
	//{{AFX_MSG_MAP(CStressElasticModulusListDlg)
	ON_BN_CLICKED(IDC_NEW_BUTTON, OnNewButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressElasticModulusListDlg メッセージ ハンドラ

BOOL CStressElasticModulusListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// New Button /////
	m_NewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_NewButton.DrawFlatFocus(TRUE);

	///// Edit Button /////
	m_EditButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_EditButton.DrawFlatFocus(TRUE);

	///// Delete Button /////
	m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteButton.DrawFlatFocus(TRUE);

	memset(&m_ElasticTable, 0, sizeof(m_ElasticTable));
	ConfigFile_GetNanoSpecIni(&m_ElasticTable, CONFIG_FILE_STRESS_MODULUS_CONFIG);

	///// Init List /////
	ElasticModulusList_Init();

	///// Set Data /////
	UpDate(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CStressElasticModulusListDlg::OnOK()
{
	if(!CheckData())	return ;

	UpDate(TRUE);

	ConfigFile_SetNanoSpecIni(&m_ElasticTable, CONFIG_FILE_STRESS_MODULUS_CONFIG);
// 2009.11.09 bagus Stress 追加変更 --{--
//	ConfigFile_SaveNanoSpecIni(SYSTEM_STRESS_MODULUS_INI_FILE);
	ConfigFile_SaveNanoSpecIni(USER_STRESS_MODULUS_INI_FILE);
// 2009.11.09 bagus Stress 追加変更 --}--

	CDialog::OnOK();
}
BOOL CStressElasticModulusListDlg::CheckData()
{
	UpdateData(TRUE);

	return TRUE;
}

void CStressElasticModulusListDlg::UpDate(BOOL bValid)
{
	LV_ITEM listitem;
	int item;
	TCHAR tszText[MATERIAL_NAME_LEN + 1];
	CString strBuffer;
	int i;

	if(bValid){
		//構造体へデータをセットする
		m_ElasticTable.uiTableNum = m_ElasticModulusListCtrl.GetItemCount();
		for (i=0; (unsigned int)i<m_ElasticTable.uiTableNum; i++) {
			m_ElasticModulusListCtrl.GetItemText(i, 0, tszText, MATERIAL_NAME_LEN + 1);
			strcpy(m_ElasticTable.ElasticModulus[i].tszElasticModulusName, tszText);

			strBuffer = m_ElasticModulusListCtrl.GetItemText(i, 1);
			// 2009.09.04 bagus stress e+11変更 --{--
			//m_ElasticTable.ElasticModulus[i].dElasticModulusValue = strtod(strBuffer.GetBuffer(0), NULL);
			m_ElasticTable.ElasticModulus[i].dElasticModulusValue = (strtod(strBuffer.GetBuffer(0), NULL) * pow(10,11));
			// 2009.09.04 bagus stress e+11変更 --}--
		}
	}else{
		//構造体にあるデータを画面にセットする
		for (i=0; (unsigned int)i<m_ElasticTable.uiTableNum; i++) {
			listitem.iItem = m_ElasticModulusListCtrl.GetItemCount();
			listitem.mask = LVIF_TEXT;

			///// Name /////
			listitem.iSubItem = 0;
			listitem.pszText = m_ElasticTable.ElasticModulus[i].tszElasticModulusName;
			item = m_ElasticModulusListCtrl.InsertItem(&listitem);

			///// Elastic Modulus /////
			listitem.iSubItem = 1;
			// 2009.09.04 bagus stress e+11変更 --{--
			//sprintf(listitem.pszText, "%e", m_ElasticTable.ElasticModulus[i].dElasticModulusValue);
			sprintf(listitem.pszText, "%f", m_ElasticTable.ElasticModulus[i].dElasticModulusValue / pow(10,11));
			// 2009.09.04 bagus stress e+11変更 --}--
			m_ElasticModulusListCtrl.SetItem(&listitem);
		}
	}
}

void CStressElasticModulusListDlg::OnNewButton()
{
	LV_ITEM listitem;
	int item;

	CStressSampleElasticModulusSettingDlg dlg;

	if (m_ElasticModulusListCtrl.GetItemCount() >= STRESS_MODULUS_MAX) {
// 2009.09.09 bagus stress StringTable使用 --{--
//		AfxMessageBox("Err:上限オーバー");
		CString	strBuffer;
		LoadStringML(IDS_STRESS_MODULUS_MAX_OVER, strBuffer, "Over Stress modulus max num.");
		AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
		return;
	}

	dlg.m_bNewItem = TRUE;
	sprintf(dlg.m_StressModulus.tszElasticModulusName, "");
	dlg.m_StressModulus.dElasticModulusValue = 0;
	if(dlg.DoModal() == IDOK){
		// 重複チェック
		if (CheckSameName(dlg.m_StressModulus.tszElasticModulusName)) {
// 2009.09.09 bagus stress StringTable使用 --{--
//			AfxMessageBox("Err:重複エラー");
			CString	strBuffer;
			LoadStringML(IDS_STRESS_MODULUS_SAME_NAME, strBuffer, "Stress modulus already exists.");
			AfxMessageBox(strBuffer);
// 2009.09.09 bagus stress StringTable使用 --}--
			return;
		}

		listitem.iItem = m_ElasticModulusListCtrl.GetItemCount();
		listitem.mask = LVIF_TEXT;

		///// Name /////
		listitem.iSubItem = 0;
		listitem.pszText = dlg.m_StressModulus.tszElasticModulusName;
		item = m_ElasticModulusListCtrl.InsertItem(&listitem);

		///// Elastic Modulus /////
		listitem.iSubItem = 1;
		// 2009.09.04 bagus stress e+11変更 --{--
		//sprintf(listitem.pszText, "%e", dlg.m_StressModulus.dElasticModulusValue);
		sprintf(listitem.pszText, "%f", dlg.m_StressModulus.dElasticModulusValue / pow(10,11));
		// 2009.09.04 bagus stress e+11変更 --}--
		m_ElasticModulusListCtrl.SetItem(&listitem);
	}
}

void CStressElasticModulusListDlg::OnEditButton()
{
	int	SelectedIndex;

	SelectedIndex = m_ElasticModulusListCtrl.GetSelectionMark();
	if (SelectedIndex < 0) {
		// 選択されていない
		return;
	}

	TCHAR tszText[MATERIAL_NAME_LEN + 1];
	CString strBuffer;
	CStressSampleElasticModulusSettingDlg dlg;

	dlg.m_bNewItem = FALSE;
	m_ElasticModulusListCtrl.GetItemText(SelectedIndex, 0, tszText, MATERIAL_NAME_LEN + 1);
	strcpy(dlg.m_StressModulus.tszElasticModulusName, tszText);
	strBuffer = m_ElasticModulusListCtrl.GetItemText(SelectedIndex, 1);
	dlg.m_StressModulus.dElasticModulusValue = strtod(strBuffer.GetBuffer(0), NULL);
	if(dlg.DoModal() == IDOK){
		// 重複チェック
//		if (CheckSameName(dlg.m_StressModulus.tszElasticModulusName)) {
//			AfxMessageBox("Err:重複エラー");
//			return;
//		}

		m_ElasticModulusListCtrl.SetItemText(SelectedIndex, 0, dlg.m_StressModulus.tszElasticModulusName);
		// 2009.09.04 bagus stress e+11変更 --{--
		//strBuffer.Format("%e", dlg.m_StressModulus.dElasticModulusValue);
		strBuffer.Format("%f", dlg.m_StressModulus.dElasticModulusValue / pow(10,11));
		// 2009.09.04 bagus stress e+11変更 --}--
		m_ElasticModulusListCtrl.SetItemText(SelectedIndex, 1, strBuffer.GetBuffer(0));
	}
}

void CStressElasticModulusListDlg::OnDeleteButton()
{
	int	SelectedIndex;

	SelectedIndex = m_ElasticModulusListCtrl.GetSelectionMark();
	if (SelectedIndex < 0) {
		// 選択されていない
		return;
	}

	m_ElasticModulusListCtrl.DeleteItem(SelectedIndex);
}

void CStressElasticModulusListDlg::ElasticModulusList_Init()
{
	LV_COLUMN listcol;
	int ItemMax;
	SYSTEM_CONFIG l_SystemConfig;

// 2009.09.04 bagus stress e+11変更 --{--
//	int Width[] = {
//		135,	 // (0) Name
//		135,	 // (1) Elastic Modulus
//	};
	int Width[] = {
		120,	 // (0) Name
		150,	 // (1) Elastic Modulus
	};
// 2009.09.04 bagus stress e+11変更 --}--

// 2009.09.04 bagus stress 単位変更 --{--
	LPTSTR pszItemJPN[] = {
		"材質名",
//		"Elastic Modulus [Pa]",
		"弾性係数 [E+11 Pa]",
	};
	LPTSTR pszItemENU[] = {
		"Name",
//		"Elastic Modulus [Pa]",
		"Elastic Modulus [E+11 Pa]",
	};
// 2009.09.04 bagus stress 単位変更 --}--

//	int Fmt[] = { LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };
	int Fmt[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT };

	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);

	ItemMax = sizeof(Width) / sizeof(int);

	m_ElasticModulusListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(int i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		if(l_SystemConfig.nLanguage == 0){
			listcol.pszText = pszItemENU[i];
		}else{
			listcol.pszText = pszItemJPN[i];
		}
		m_ElasticModulusListCtrl.InsertColumn(i,&listcol);
	}

	m_ElasticModulusListCtrl.DeleteAllItems();
}

BOOL CStressElasticModulusListDlg::CheckSameName(TCHAR *tszName)
{
	int	itemNum;
	int	i;

	CString	strChkName;
	CString	strTarName;

	strChkName.Format("%s", tszName);

	itemNum = m_ElasticModulusListCtrl.GetItemCount();
	for (i=0; i<itemNum; i++) {
		strTarName = m_ElasticModulusListCtrl.GetItemText(i, 0);
		if (strChkName == strTarName) {
			return ( TRUE );
		}
	}

	return ( FALSE );
}
