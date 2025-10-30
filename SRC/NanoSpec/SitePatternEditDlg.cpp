// SitePatternEditDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
#include "ExecutionDlg.h"
#include "SitePatternEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WAIT_TIMER				(1)

// Callback�֐�
int CALLBACK CSitePatternEditDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BOOL bOrder = (BOOL)LOWORD(lParamSort);
	int iIndex = (int)HIWORD(lParamSort);

	CListCtrl* pList = (CListCtrl*)g_pSitePatternEditDlg->GetDlgItem(IDC_PATTERN_LIST);

	CString strBuffer1 = pList->GetItemText(lParam1, iIndex);
	CString strBuffer2 = pList->GetItemText(lParam2, iIndex);

	return bOrder ? strcmp(strBuffer2, strBuffer1) /*sort desc*/ : strcmp(strBuffer1, strBuffer2) /*sort asc*/;
}

/////////////////////////////////////////////////////////////////////////////
// CSitePatternEditDlg �_�C�A���O


CSitePatternEditDlg::CSitePatternEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSitePatternEditDlg::IDD, pParent)
{
	::ZeroMemory(m_bSort, sizeof(m_bSort));

	//{{AFX_DATA_INIT(CSitePatternEditDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}

CSitePatternEditDlg::~CSitePatternEditDlg()
{
	::DeleteFile(PATTERN_IMG_TEMP);
}

void CSitePatternEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSitePatternEditDlg)
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_DELETE_ALL_BUTTON, m_DeleteAllButton);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SITE_PR_IMG1, m_stcSitePrImg);
}


BEGIN_MESSAGE_MAP(CSitePatternEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSitePatternEditDlg)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_PATTERN_LIST, OnClickPatternList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PATTERN_LIST, OnColumnclickPatternList)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_DELETE_ALL_BUTTON, OnDeleteAllButton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EXECUTION_START, OnExecutionStart)
	ON_MESSAGE(WM_EXECUTION_END, OnExecutionEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSitePatternEditDlg ���b�Z�[�W �n���h��

BOOL CSitePatternEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	m_SelectItem = -1;
	m_SelectName = "";
	::ZeroMemory(&m_pPatSubInfo, sizeof(m_pPatSubInfo));

	CRect rect;

	//// Button Control /////
	switch( m_iMode ){
	case SITEPATTERN_MODE_DEL:
		m_DeleteButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_DeleteButton.DrawFlatFocus(TRUE);
		m_DeleteAllButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
		m_DeleteAllButton.DrawFlatFocus(TRUE);

		// Cancel�{�^����\�� �� Ok�{�^���ړ�
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

		GetDlgItem(IDCANCEL)->GetWindowRect(rect);
		ScreenToClient(rect);
		GetDlgItem(IDOK)->MoveWindow(rect);
		break;
	case SITEPATTERN_MODE_EDIT:
	default:
		GetDlgItem(IDC_DELETE_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DELETE_ALL_BUTTON)->ShowWindow(SW_HIDE);
		break;
	}
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Static Control /////
	m_lstCtrl.SubclassWindow(GetDlgItem(IDC_PATTERN_LIST)->GetSafeHwnd());
	m_lstCtrl.SetViewStyle();

	///// Lens Magnification Combo /////
	ConfigFile_GetNanoSpecIni(srTurret, CONFIG_FILE_SR_TURRET);

	///// Header Set /////
	List_HeaderSet();

	m_ExecuteSeq = 0;
	SetTimer(WAIT_TIMER, 100, NULL);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
				  // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

// ==========================================================================
//
void CSitePatternEditDlg::OnOK()
{
	CString strBuffer, strTitle;

	m_pDoc->OperationLogging("CSitePatternEditDlg::OK Button was push.");

	switch( m_iMode ){
	case SITEPATTERN_MODE_DEL:
		break;
	case SITEPATTERN_MODE_EDIT:
	default:
		if( m_SelectItem <= 0 ){
			LoadStringML(IDS_SELECT_PATTERN, strBuffer, "Please, Select Pattern Image");
			LoadStringML(IDS_SELECT_PATTERN_CONFIRM, strTitle, "SELECT PATTERN IMAGE CONFIRM");
			MessageBox(strBuffer, strTitle, MB_OK);
			return;
		}
		break;
	}
	CDialog::OnOK();
}

// ==========================================================================
//
void CSitePatternEditDlg::OnCancel()
{
	m_pDoc->OperationLogging("CSitePatternEditDlg::Cancel Button was push.");

	CDialog::OnCancel();
}

// ==========================================================================
//
void CSitePatternEditDlg::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent ) {
	case WAIT_TIMER:
		switch( m_ExecuteSeq ){
		case 0:
			::PostMessage(this->m_hWnd, WM_EXECUTION_START, 0L, 0L);
			m_ExecuteSeq ++;
			break;
		case 1:
			InitialDataSet();
			m_ExecuteSeq ++;
			break;
		case 2:
			::PostMessage(this->m_hWnd, WM_EXECUTION_END, 0L, 0L);
			m_ExecuteSeq ++;
			KillTimer(nIDEvent);
			break;
		}
		break;
	}
}

// =========================================================================
//
void CSitePatternEditDlg::List_HeaderSet()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_PATTERN_LIST);
	LV_COLUMN listcol;
	int i;
	DWORD dwStyle = 0;
	int ItemMax;
	//Saiki 20090527 Add ----->
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	//Saiki 20090527 Add <-----

	int Width[] = {
		0,		// (0) Index
		240,	// (1) Pattern Name
//		70,		// (2) Lens
	};

	LPTSTR Item_JPN[] = {
		"",
		"�p�^�[����",
//		"�����Y",
	};
	LPTSTR Item_ENU[] = {
		"",
		"Pattern Name",
//		"Lens",
	};

	int Fmt[] = {
		LVCFMT_LEFT,
		LVCFMT_LEFT,
//		LVCFMT_CENTER,
	};

	ItemMax = sizeof(Width) / sizeof(int);

	///// Modify Style /////
	dwStyle = 0;
	dwStyle |= LVS_SHOWSELALWAYS;
	dwStyle |= LVS_SINGLESEL;
	pList->ModifyStyle(0, dwStyle);

	///// Extended Style /////
	dwStyle = 0;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	pList->SetExtendedStyle(dwStyle);

	listcol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for(i = 0 ; i < ItemMax ; i++){
		listcol.fmt = Fmt[i];
		listcol.cx = Width[i];
		//Saiki 20090601 Change ----->
		//listcol.pszText = Item[i];
		if(l_SystemConfig.nLanguage == 0){
			listcol.pszText = Item_ENU[i];
		}
		else{
			listcol.pszText = Item_JPN[i];
		}
		//Saiki 20090601 Change <-----
		pList->InsertColumn(i,&listcol);
	}

}

// =========================================================================
//
void CSitePatternEditDlg::List_DataSet()
{
	CString strBuf;

	rfListCtrl().DeleteAllItems();

	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	char szFilePath[MAX_PATH];
	BOOL bLoop, bRet;
	SYSTEMTIME lastWriteSystemTime;

	sprintf(szDirPath, "%s%s", g_szDb_Pattern_Img_Dir, "*.SPN" );
	bLoop = Finder.FindFile(szDirPath);
	while(bLoop){
		bLoop = Finder.FindNextFile();
		if(Finder.IsDots())
			continue;

	// �t�@�C�������擾
	strBuf = Finder.GetFileTitle();

	sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Dir, (LPCSTR)strBuf, DAT_EXT);
		bRet = PatternFile_ExistPatternFilePath(szFilePath, &lastWriteSystemTime);
		if( bRet ){
			// �T�u���t�@�C�������݂���ꍇ�̂ݕ\���ΏۂƂ���
			List_Add(strBuf);
		}
	}

}

// =========================================================================
//
void CSitePatternEditDlg::List_Add(const char* Name)
{
	LV_ITEM listitem;
	::ZeroMemory( &listitem, sizeof(listitem) );

	listitem.iItem = rfListCtrl().GetItemCount();
	listitem.mask = LVIF_TEXT;

	///// Index /////
	listitem.iSubItem = 0;
	listitem.pszText = "";
	int item = rfListCtrl().InsertItem(&listitem);

	///// Name /////
	listitem.iSubItem = 1;
	listitem.pszText = (LPTSTR)Name;
	rfListCtrl().SetItem(&listitem);

	rfListCtrl().SetItemState(item, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// =========================================================================
//
void CSitePatternEditDlg::SortColumn(int iCol)
{
	RenumberItem();
	m_lstCtrl.SortItems(CompareFunc, MAKELPARAM((WORD)m_bSort[iCol], WORD(iCol)));
//	m_bSort[iCol] = !m_bSort[iCol];
}

// =========================================================================
//
void CSitePatternEditDlg::RenumberItem()
{
	LV_ITEM listitem;

	for ( int i = 0; i < rfListCtrl().GetItemCount(); i++ ) {
		listitem.iItem = i;
		listitem.iSubItem = 0;
		listitem.mask = LVIF_PARAM;
		listitem.lParam = i;
		m_lstCtrl.SetItem(&listitem);
	}
}

// ==========================================================================
//
void CSitePatternEditDlg::OnClickPatternList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nIndex = pNMListView->iItem;

	m_SelectItem = nIndex + 1;
	DispPattern();
	
	*pResult = 0;
}

// ==========================================================================
//
void CSitePatternEditDlg::DispPattern()
{
	TCHAR	szFileName[MAX_PATH];
	TCHAR	szFilePath[MAX_PATH];
	TCHAR	szBuffer[256];
	BOOL	bRet;
	SYSTEMTIME lastWriteSystemTime;

	UpdateData();

	SetDlgItemText(IDC_LENS1, "");
	SetDlgItemText(IDC_OFFSET_X1, "");
	SetDlgItemText(IDC_OFFSET_Y1, "");

	::ZeroMemory(szFileName, sizeof(szFileName));
	::ZeroMemory(szFilePath, sizeof(szFilePath));
	::DeleteFile(PATTERN_IMG_TEMP);

	m_stcSitePrImg.Invalidate(TRUE);

	if( m_SelectItem > 0 ){
		// �p�^�[����
		m_SelectName = rfListCtrl().GetItemText(m_SelectItem-1, 1);

//		PatternFile_MakePatternImageFilePath(szFilePath, m_SelectName, m_SelectLens);
	sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, (LPCSTR)m_SelectName, PATTERN_IMAGE_EXT);

		bRet = PatternFile_ExistPatternFilePath(szFilePath, &lastWriteSystemTime);
		if( bRet ){
			// Pattern�L��
			::CopyFile(szFilePath, PATTERN_IMG_TEMP, FALSE);
			// �I�t�Z�b�g�����擾
			PatternFile_GetSubInfo(m_SelectName, &m_pPatSubInfo);
			// �����Y
			SetDlgItemText(IDC_LENS1, srTurret[m_pPatSubInfo.iLens].szName);
			// �I�t�Z�b�gX
			_stprintf(szBuffer, "%.3lf", (double)(m_pPatSubInfo.ptOffset.x)/MICROMETRE);
			SetDlgItemText(IDC_OFFSET_X1, szBuffer);
			// �I�t�Z�b�gY
			_stprintf(szBuffer, "%.3lf", (double)(m_pPatSubInfo.ptOffset.y)/MICROMETRE);
			SetDlgItemText(IDC_OFFSET_Y1, szBuffer);
		}

		SetCursorPosition(m_SelectItem-1);
	}
	m_stcSitePrImg.SetBitmap(PATTERN_IMG_TEMP);

	UpdateData(FALSE);
}

// ==========================================================================
//
void CSitePatternEditDlg::SetCursorPosition(int iItem)
{
	rfListCtrl().SetSelectionMark(iItem);
	rfListCtrl().SetItemState(iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

// ==========================================================================
//
void CSitePatternEditDlg::InitialDataSet()
{
	///// Data Set /////
	List_DataSet();

	int nItem = m_lstCtrl.GetItemCount();
	if( nItem > 0 ) m_SelectItem = 1;

	///// Sort /////
	if( nItem >= 2 ) SortColumn(1);

	///// Pattern Display /////
	DispPattern();
}

// ==========================================================================
//
//void CSitePatternEditDlg::OnExecutionStart()
LRESULT CSitePatternEditDlg::OnExecutionStart(WPARAM wparam, LPARAM lparam)
{
//	MessageBox(	"�p�^�[���t�@�C���������Ǝ��Ԃ������邱�Ƃ�����܂�\n���΂炭���҂�������...",
//				"�p�^�[���ǂݍ��ݏ���",
//				MB_OK);
	CExecutionDlg dlg;
//	dlg.SetCaption(IDS_EXECUTION_CAPTION);
	CString strBuffer;
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	dlg.SetCaption(strBuffer);
	//Saiki 20090601 Change ----->
	//dlg.SetMessage("�p�^�[���t�@�C���������Ǝ��Ԃ������邱�Ƃ�����܂�\n���΂炭���҂�������...");
	LoadStringML(IDS_MANY_PAT_FILE_SOME_TIME, strBuffer, "If you have many Pattern Files, it may take some time \n Please wait ...");
	dlg.SetMessage(strBuffer);
	//Saiki 20090601 Change <-----
	dlg.DoModal();

	return 0;
}

// ==========================================================================
//
//void CSitePatternEditDlg::OnExecutionEnd()
LRESULT CSitePatternEditDlg::OnExecutionEnd(WPARAM wparam, LPARAM lparam)
{
	HWND hWnd;
	CString strBuffer;
	LoadStringML(IDS_EXECUTION_CAPTION, strBuffer, "EXECUTION");
	hWnd = ::FindWindow(NULL, strBuffer);
	if(hWnd != NULL)
	{
		::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}

	return 0;
}

// =========================================================================
//
void CSitePatternEditDlg::OnColumnclickPatternList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_bSort[pNMListView->iSubItem] = !m_bSort[pNMListView->iSubItem];
	SortColumn(pNMListView->iSubItem);

	*pResult = 0;
}

// ==========================================================================
//
void CSitePatternEditDlg::OnDeleteButton()
{
	TCHAR	szFilePath[MAX_PATH];
	CString strBuffer, strTitle;

	if( m_SelectItem <= 0 ){
		LoadStringML(IDS_SELECT_PATTERN, strBuffer, "Please, Select Pattern Image");
		LoadStringML(IDS_SELECT_PATTERN_CONFIRM, strTitle, "SELECT PATTERN IMAGE CONFIRM");
		MessageBox(strBuffer, strTitle, MB_OK);
		return;
	}

	LoadStringML(IDS_DELETE_PATTERN, strBuffer, "Would you Delete Pattern Image?");
	LoadStringML(IDS_DELETE_PATTERN_CONFIRM, strTitle, "DELETE PATTERN IMAGE CONFIRM");
	if ( MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL) == IDOK ){
	// �p�^�[����
	m_SelectName = rfListCtrl().GetItemText(m_SelectItem-1, 1);
	sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, (LPCSTR)m_SelectName, PATTERN_IMAGE_EXT);
	::DeleteFile(szFilePath);
	sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Dir, (LPCSTR)m_SelectName, DAT_EXT);
		::DeleteFile(szFilePath);

		InitialDataSet();
	}

}

// ==========================================================================
//
void CSitePatternEditDlg::OnDeleteAllButton()
{
	CString strBuffer, strTitle;
	CString strBuf;
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	char szFilePath[MAX_PATH];
	BOOL bLoop;


	LoadStringML(IDS_DELETE_ALL_PATTERN, strBuffer, "Would you Delete All Pattern Image?");
	LoadStringML(IDS_DELETE_ALL_PATTERN_CONFIRM, strTitle, "DELETE ALL PATTERN IMAGE CONFIRM");
	if ( MessageBox(strBuffer, strTitle, MB_ICONQUESTION | MB_OKCANCEL) == IDOK ){

		sprintf(szDirPath, "%s%s", g_szDb_Pattern_Img_Dir, "*.SPN" );
		bLoop = Finder.FindFile(szDirPath);
		while(bLoop){
			bLoop = Finder.FindNextFile();
			if(Finder.IsDots())
				continue;

		// �t�@�C�������擾
		strBuf = Finder.GetFileTitle();
		sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, (LPCSTR)strBuf, PATTERN_IMAGE_EXT);
			::DeleteFile(szFilePath);
//			sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Dir, strBuf, DAT_EXT);
//			::DeleteFile(szFilePath);

		}

		sprintf(szDirPath, "%s%s", g_szDb_Pattern_Dir, "*.dat" );
		bLoop = Finder.FindFile(szDirPath);
		while(bLoop){
			bLoop = Finder.FindNextFile();
			if(Finder.IsDots())
				continue;

		// �t�@�C�������擾
		strBuf = Finder.GetFileTitle();
//			sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Img_Dir, strBuf, PATTERN_IMAGE_EXT);
//			::DeleteFile(szFilePath);
		sprintf(szFilePath, "%s%s%s", g_szDb_Pattern_Dir, (LPCSTR)strBuf, DAT_EXT);
			::DeleteFile(szFilePath);

		}

		InitialDataSet();
	}
}
