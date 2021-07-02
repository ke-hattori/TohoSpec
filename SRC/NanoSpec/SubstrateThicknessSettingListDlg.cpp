// SubstrateThicknessSettingListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "SubstrateThicknessSettingDlg.h"
#include "SubstrateThicknessSettingListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessSettingListDlg ダイアログ


CSubstrateThicknessSettingListDlg::CSubstrateThicknessSettingListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubstrateThicknessSettingListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubstrateThicknessSettingListDlg)
	//}}AFX_DATA_INIT
}


void CSubstrateThicknessSettingListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubstrateThicknessSettingListDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_NEW_BUTTON, m_NewButton);
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_REFERENCE_EDIT_BUTTON, m_ReferenceEditButton);
	DDX_Control(pDX, IDC_REFERENCE_DELETE_BUTTON, m_ReferenceDeleteButton);
	DDX_Control(pDX, IDC_SUBSTRATE_THICKNESS_LIST, m_SubstrateThicknessListCtrl);
	DDX_Control(pDX, IDC_REFERENCE_THICKNESS_LIST, m_ReferenceThicknessListCtrl);
}


BEGIN_MESSAGE_MAP(CSubstrateThicknessSettingListDlg, CDialog)
	//{{AFX_MSG_MAP(CSubstrateThicknessSettingListDlg)
	ON_BN_CLICKED(IDC_NEW_BUTTON, OnNewButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_REFERENCE_EDIT_BUTTON, OnReferenceEditButton)
	ON_BN_CLICKED(IDC_REFERENCE_DELETE_BUTTON, OnReferenceDeleteButton)
	ON_NOTIFY(NM_DBLCLK, IDC_SUBSTRATE_THICKNESS_LIST, OnDblclkSubstrateThicknessList)
	ON_NOTIFY(NM_DBLCLK, IDC_REFERENCE_THICKNESS_LIST, OnDblclkReferenceThicknessList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessSettingListDlg メッセージ ハンドラ

BOOL CSubstrateThicknessSettingListDlg::OnInitDialog() 
{
	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

 	///// Get Data /////
 	ConfigFile_GetNanoSpecIni(&m_SubstrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);

	CDialog::OnInitDialog();

	///// New Button /////
//	m_NewButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
//	m_NewButton.DrawFlatFocus(TRUE);
	m_NewButton.EnableWindow(FALSE);
	m_NewButton.ShowWindow(SW_HIDE);

	///// Edit Button /////
	m_EditButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_EditButton.DrawFlatFocus(TRUE);

	///// Delete Button /////
	m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_DeleteButton.DrawFlatFocus(TRUE);

	///// Reference Edit Button /////
	m_ReferenceEditButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceEditButton.DrawFlatFocus(TRUE);

	///// Reference Delete Button /////
	m_ReferenceDeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ReferenceDeleteButton.DrawFlatFocus(TRUE);

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Init List /////
	SubstrateThicknessList_Init();
	ReferenceThicknessList_Init();

	UpDate(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::SubstrateThicknessList_Init()
{
	LV_COLUMN listcol;
	int ItemMax;
	SYSTEM_CONFIG l_SystemConfig;

	int Width[] = {
		60,		 // (0) No.
		150,	 // (1) Substrate Thickness
		150,	 // (2) Invalid Region
	};

	LPTSTR pszItemJPN[] = {
		"No.",
		"板厚",
		"無効領域",
	};
	LPTSTR pszItemENU[] = {
		"No.",
		"Substrate Thickness",
		"Invalid Region",
	};

	int Fmt[] = { LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_CENTER};

	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);

	ItemMax = sizeof(Width) / sizeof(Width[0]);

	m_SubstrateThicknessListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for ( int i = 0 ; i < ItemMax; i++ ) {
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		if (l_SystemConfig.nLanguage == 0) {
			listcol.pszText = pszItemENU[i];
		}
		else {
			listcol.pszText = pszItemJPN[i];
		}
		m_SubstrateThicknessListCtrl.InsertColumn(i, &listcol);
	}

	m_SubstrateThicknessListCtrl.DeleteAllItems();
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::ReferenceThicknessList_Init()
{
	LV_COLUMN listcol;
	int ItemMax;
	SYSTEM_CONFIG l_SystemConfig;

	int Width[] = {
		60,		 // (0) No.
		150,	 // (1) Substrate Thickness
		150,	 // (2) Invalid Region
	};

	LPTSTR pszItemJPN[] = {
		"No.",
		"板厚",
		"無効領域",
	};
	LPTSTR pszItemENU[] = {
		"No.",
		"Substrate Thickness",
		"Invalid Region",
	};

	int Fmt[] = { LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_CENTER};

	ConfigFile_GetNanoSpecIni(&l_SystemConfig,CONFIG_FILE_SYSTEM_CONFIG);

	ItemMax = sizeof(Width) / sizeof(Width[0]);

	m_ReferenceThicknessListCtrl.SetViewStyle();

	///// List Setting /////
	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for ( int i = 0 ; i < ItemMax; i++ ) {
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		if (l_SystemConfig.nLanguage == 0) {
			listcol.pszText = pszItemENU[i];
		}
		else {
			listcol.pszText = pszItemJPN[i];
		}
		m_ReferenceThicknessListCtrl.InsertColumn(i, &listcol);
	}

	m_ReferenceThicknessListCtrl.DeleteAllItems();
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::UpdateListCtrl(CNanoListCtrl* pListCtrl, int iIndex)
{
	LV_ITEM listitem;
	CString strBuffer;
	double dTemp;

	listitem.iItem = pListCtrl->GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// No /////
	listitem.iSubItem = 0;
	if ( iIndex == 0 ) {
		listitem.pszText = _T("Reference");
	}
	else {
		strBuffer.Format("%d", iIndex);
		listitem.pszText = strBuffer.GetBuffer(0);
	}
	pListCtrl->InsertItem(&listitem);

	///// Substrate Thickness /////
	listitem.iSubItem = 1;
	if(m_SubstrateThicknessTable.SubstrateThickness[iIndex].bUseData){
		dTemp = MicroToMilli(m_SubstrateThicknessTable.SubstrateThickness[iIndex].dThickness);
		strBuffer.Format("%.2lf", dTemp);
		listitem.pszText = strBuffer.GetBuffer(0);
	}
	else{
		listitem.pszText = _T("-");
	}
	pListCtrl->SetItem(&listitem);

	///// Invalid Region /////
	listitem.iSubItem = 2;
	if(m_SubstrateThicknessTable.SubstrateThickness[iIndex].bUseData){
		dTemp = MicroToMilli(m_SubstrateThicknessTable.SubstrateThickness[iIndex].dInvalidRegion);
		strBuffer.Format("%.2lf", dTemp);
		listitem.pszText = strBuffer.GetBuffer(0);
	}
	else{
		listitem.pszText = _T("-");
	}
	pListCtrl->SetItem(&listitem);
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::UpDate(BOOL bValid)
{
	if(bValid){
		//構造体へデータをセットするのは、個別入力ダイアログ画面での入力完了時点
	}
	else{
		//構造体にあるデータを画面にセットする
		UpdateListCtrl(&m_ReferenceThicknessListCtrl, 0);				// Index 0は、リファレンスの板厚

		for (int i = 1; i < SUBSTRATE_THICKNESS_MAX; i++) {				// Index 1以降は、サンプルの板厚
			UpdateListCtrl(&m_SubstrateThicknessListCtrl, i);
		}
	}
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnOK() 
{
	CDialog::OnOK();
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnCancel() 
{
	CDialog::OnCancel();
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnNewButton() 
{
}

// =========================================================================
//
double CSubstrateThicknessSettingListDlg::MilliToMicro(double dMilli)
{
	return dMilli * 1000.0;
}

// =========================================================================
//
double CSubstrateThicknessSettingListDlg::MicroToMilli(double dMicro)
{
	return dMicro / 1000.0;
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::EditSubstrateThickness() 
{
	CString strBuffer;
	int	iSelectedIndex;
	int	ConfigDataIndex;
	double dTemp;

	iSelectedIndex = m_SubstrateThicknessListCtrl.GetSelectionMark();
	if (iSelectedIndex < 0) {
		// 選択されていない
		return;
	}

	// Index 0:リファレンス分のズレを加算しておく
	ConfigDataIndex = iSelectedIndex + 1;

	CSubstrateThicknessSettingDlg dlg(FALSE);

	dlg.m_iNo = iSelectedIndex + 1;
	if( m_SubstrateThicknessTable.SubstrateThickness[ConfigDataIndex].bUseData ) {
		dlg.m_SubstrateThickness = m_SubstrateThicknessTable.SubstrateThickness[ConfigDataIndex];
	}

	if(dlg.DoModal() == IDOK){
		// iniファイルに保存
		m_SubstrateThicknessTable.SubstrateThickness[ConfigDataIndex] = dlg.m_SubstrateThickness;
		ConfigFile_SetNanoSpecIni(&m_SubstrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);
		ConfigFile_SaveNanoSpecIni(SUBSTRATE_THICKNESS_INI_FILE);

		// 画面表示更新
		dTemp = MicroToMilli(m_SubstrateThicknessTable.SubstrateThickness[ConfigDataIndex].dThickness);
		strBuffer.Format("%.2lf", dTemp);
		m_SubstrateThicknessListCtrl.SetItemText(iSelectedIndex, 1, strBuffer);
		dTemp = MicroToMilli(m_SubstrateThicknessTable.SubstrateThickness[ConfigDataIndex].dInvalidRegion);
		strBuffer.Format("%.2lf", dTemp);
		m_SubstrateThicknessListCtrl.SetItemText(iSelectedIndex, 2, strBuffer);
	}
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnEditButton() 
{
	EditSubstrateThickness();
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnDeleteButton() 
{
	int	SelectedIndex;
	int ConfigDataIndex;

	SelectedIndex = m_SubstrateThicknessListCtrl.GetSelectionMark();
	if (SelectedIndex < 0) {
		// 選択されていない
		return;
	}

	CString strBuffer;
	CString strListCtrlText;
	CString strCaption;
	CString strMessage;
	int iRet;

	strListCtrlText = m_SubstrateThicknessListCtrl.GetItemText(SelectedIndex, 0);

	LoadStringML(IDS_WANT_DELETE, strBuffer, "Do you want to Delete ""%s""?");
	strMessage.Format(strBuffer, (LPCTSTR)strListCtrlText);

	LoadStringML(IDS_TITLE_SUBTHICK_LIST, strBuffer, "SUBSTRATE THICKNESS LIST");
	strCaption = strBuffer;

	iRet = MessageBox(strMessage, strCaption, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2);
	if (iRet == IDCANCEL ) {
		return;
	}

	// Index 0:リファレンス分のズレを加算しておく
	ConfigDataIndex = SelectedIndex + 1;

	// iniファイル書き込み
	SUBSTRATE_THICKNESS substrateThickness;
	::ZeroMemory(&substrateThickness, sizeof(substrateThickness));
	m_SubstrateThicknessTable.SubstrateThickness[ConfigDataIndex] = substrateThickness;
	ConfigFile_SetNanoSpecIni(&m_SubstrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SUBSTRATE_THICKNESS_INI_FILE);

	// 画面表示更新
	m_SubstrateThicknessListCtrl.SetItemText(SelectedIndex, 1, _T("-"));
	m_SubstrateThicknessListCtrl.SetItemText(SelectedIndex, 2, _T("-"));
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnDblclkSubstrateThicknessList(NMHDR* pNMHDR, LRESULT* pResult)
{
	EditSubstrateThickness();

	*pResult = 0;
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::EditReferenceThickness() 
{
	CString strBuffer;
	CSubstrateThicknessSettingDlg dlg(FALSE);
	double dTemp;

	dlg.m_iNo = 0;
	if( m_SubstrateThicknessTable.SubstrateThickness[0].bUseData ) {
		dlg.m_SubstrateThickness = m_SubstrateThicknessTable.SubstrateThickness[0];
	}

	if(dlg.DoModal() == IDOK){
		// iniファイルに保存
		m_SubstrateThicknessTable.SubstrateThickness[0] = dlg.m_SubstrateThickness;
		ConfigFile_SetNanoSpecIni(&m_SubstrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);
		ConfigFile_SaveNanoSpecIni(SUBSTRATE_THICKNESS_INI_FILE);

		// 画面表示更新
		dTemp = MicroToMilli(m_SubstrateThicknessTable.SubstrateThickness[0].dThickness);
		strBuffer.Format("%.2lf", dTemp);
		m_ReferenceThicknessListCtrl.SetItemText(0, 1, strBuffer);
		dTemp = MicroToMilli(m_SubstrateThicknessTable.SubstrateThickness[0].dInvalidRegion);
		strBuffer.Format("%.2lf", dTemp);
		m_ReferenceThicknessListCtrl.SetItemText(0, 2, strBuffer);
	}
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnReferenceEditButton() 
{
	EditReferenceThickness();
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnReferenceDeleteButton() 
{
	CString strBuffer;
	CString strListCtrlText;
	CString strCaption;
	CString strMessage;
	int iRet;

	strListCtrlText = m_ReferenceThicknessListCtrl.GetItemText(0, 0);

	LoadStringML(IDS_WANT_DELETE, strBuffer, "Do you want to Delete ""%s""?");
	strMessage.Format(strBuffer, (LPCTSTR)strListCtrlText);

	LoadStringML(IDS_TITLE_SUBTHICK_REF_LIST, strBuffer, "REFERENCE THICKNESS LIST");
	strCaption = strBuffer;

	iRet = MessageBox(strMessage, strCaption, MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2);
	if (iRet == IDCANCEL ) {
		return;
	}

	// iniファイル書き込み
	SUBSTRATE_THICKNESS substrateThickness;
	::ZeroMemory(&substrateThickness, sizeof(substrateThickness));
	m_SubstrateThicknessTable.SubstrateThickness[0] = substrateThickness;
	ConfigFile_SetNanoSpecIni(&m_SubstrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);
	ConfigFile_SaveNanoSpecIni(SUBSTRATE_THICKNESS_INI_FILE);

	// 画面表示更新
	m_ReferenceThicknessListCtrl.SetItemText(0, 1, _T("-"));
	m_ReferenceThicknessListCtrl.SetItemText(0, 2, _T("-"));
}

// =========================================================================
//
void CSubstrateThicknessSettingListDlg::OnDblclkReferenceThicknessList(NMHDR* pNMHDR, LRESULT* pResult)
{
	EditReferenceThickness();

	*pResult = 0;
}
