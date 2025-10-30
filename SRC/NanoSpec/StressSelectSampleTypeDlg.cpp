// StressSelectSampleTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "StressSelectSampleTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStressSelectSampleTypeDlg ダイアログ


CStressSelectSampleTypeDlg::CStressSelectSampleTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStressSelectSampleTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStressSelectSampleTypeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CStressSelectSampleTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStressSelectSampleTypeDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CStressSelectSampleTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CStressSelectSampleTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStressSelectSampleTypeDlg メッセージ ハンドラ

void CStressSelectSampleTypeDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_STRESS_SELECT_SAMPLE_TYPE);
	int iSelect = pCombo->GetCurSel();

	if(iSelect == CB_ERR){
		return;
	}

	memset(m_szName, 0, sizeof(m_szName));
	pCombo->GetLBText(iSelect, m_szName);

	CDialog::OnOK();
}

BOOL CStressSelectSampleTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

	///// Sample Type Combo /////
	if(!SampleTypeCombo_Init()){
		EndDialog(IDCANCEL);
		return FALSE;
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CStressSelectSampleTypeDlg::SampleTypeCombo_Init()
{
	CComboBox* pCombo = (CComboBox *)GetDlgItem(IDC_STRESS_SELECT_SAMPLE_TYPE);
	CFileFind Finder;
	char szDirPath[_MAX_PATH];
	CString csName;
	BOOL bLoop;
	CString strBuffer, strTitle;

	pCombo->ResetContent();

	sprintf(szDirPath, "%s*%s", g_szCfg_System_Sample_Dir, DAT_EXT);
	bLoop = Finder.FindFile(szDirPath);

	while(bLoop){
		bLoop = Finder.FindNextFile();

		if(Finder.IsDots()){
			continue;
		}

		csName = Finder.GetFileTitle();
		csName.Insert(RECIPE_NAME_LEN, '\0');

		pCombo->AddString(csName);
	}

	if(pCombo->GetCount() <= 0){
		LoadStringML(IDS_SAMPLE_SETTING, strBuffer, "Please Sample Setting");
		LoadStringML(IDS_SELECT_SAMPLE_TYPE, strTitle, "SELECT SAMPLE TYPE");
		MessageBox(strBuffer, strTitle, MB_OK | MB_ICONSTOP);

		return FALSE;
	}

	pCombo->SetCurSel(0);

	return TRUE;
}
