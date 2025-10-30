// SelectHeadTypeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "SelectHeadTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #########################################################################
// CSelectHeadTypeDlg ダイアログ
// #########################################################################

// =========================================================================
//
CSelectHeadTypeDlg::CSelectHeadTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectHeadTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectHeadTypeDlg)
	//}}AFX_DATA_INIT

	m_iHeadType = HEAD_TYPE_SR;
}

// =========================================================================
//
void CSelectHeadTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectHeadTypeDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
}

// =========================================================================
//
BEGIN_MESSAGE_MAP(CSelectHeadTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectHeadTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// =========================================================================
// CSelectHeadTypeDlg メッセージ ハンドラ

// =========================================================================
//
BOOL CSelectHeadTypeDlg::OnInitDialog()
{
	// Kojika 20090601 Add
	CString l_strBuffer, l_strTitle;
	// Kojika 20090601 Add End

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);


	int iSelect = 0;
	CListBox* pList;
	HEAD_TYPE_CONFIG HeadType;
	DEFAULT_HEAD_TYPE DefaultHeadType;

	ConfigFile_GetNanoSpecIni(&HeadType, CONFIG_FILE_HEAD_TYPE_CONFIG);
	ConfigFile_GetNanoSpecIni(&DefaultHeadType, CONFIG_FILE_DEFAULT_HEAD_TYPE);
	m_iHeadType = DefaultHeadType.nHeadType;
	pList = (CListBox *)GetDlgItem(IDC_HEAD_TYPE_LIST);
	pList->ResetContent();

	// 選択されているヘッドのみ表示
	if(HeadType.bSR){
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SR]);
		if(m_iHeadType == HEAD_TYPE_SR){
			iSelect = pList->GetCount() - 1;
		}
	}
	if(HeadType.bSE){
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		if(HeadType.bCompEASE){
			pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_COMPEASE]);
		}
		else{
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_SE]);
		}
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		if(m_iHeadType == HEAD_TYPE_SE){
			iSelect = pList->GetCount() - 1;
		}
	}
// 2009.10.19 bagus MS 追加 --{--
#if 0
	if(HeadType.bIRSE){
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_IRSE]);
		if(m_iHeadType == HEAD_TYPE_IRSE){
			iSelect = pList->GetCount() - 1;
		}
	}
#endif
// 2009.10.19 bagus MS 追加 --}--
	if(HeadType.bResist){
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_4PP]);
		if(m_iHeadType == HEAD_TYPE_4PP){
			iSelect = pList->GetCount() - 1;
		}
	}
	if(HeadType.bCTA){
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_CTA]);
		if(m_iHeadType == HEAD_TYPE_CTA){
			iSelect = pList->GetCount() - 1;
		}
	}
	if(HeadType.bStress){
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_STRESS]);
		if(m_iHeadType == HEAD_TYPE_STRESS){
			iSelect = pList->GetCount() - 1;
		}
	}
// 2009.10.19 bagus MS 追加 --{--
	if(HeadType.bMS){
		pList->AddString(HEAD_TYPE_ITEM[HEAD_TYPE_MS]);
		if(m_iHeadType == HEAD_TYPE_MS){
			iSelect = pList->GetCount() - 1;
		}
	}
// 2009.10.19 bagus MS 追加 --}--

	if(pList->GetCount() <= 0){
		// Kojika 20090601 Change
		//MessageBox("Head Setting has not been set up.", "SELECT HEAD TYPE", MB_OK);
		//MessageBox("ヘッドタイプが設定されていません", "SELECT HEAD TYPE", MB_OK);
		LoadStringML(IDS_HEAD_SETTING_NOT_SETUP, l_strBuffer, "Head Setting has not been set up.");
		LoadStringML(IDS_TITLE_SELECT_HEAD_TYPE, l_strTitle, "SELECT HEAD TYPE");
		MessageBox(l_strBuffer, l_strTitle, MB_OK);
		// Kojika 20090601 Change End
		CDialog::OnCancel();
		return FALSE;
	}

	pList->SetCurSel(iSelect);

	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// =========================================================================
//
void CSelectHeadTypeDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	int iSelect, i;
	CListBox* pList;
	char szHeadType[256];

	pList = (CListBox *)GetDlgItem(IDC_HEAD_TYPE_LIST);
	iSelect =pList->GetCurSel();
	memset(szHeadType, 0, sizeof(szHeadType));

	// 選択されていない
	if(iSelect == LB_ERR){
		return;
	}

	pList->GetText(iSelect, szHeadType);
	for(i = 0; i < HEAD_TYPE_MAX; i++){
		if(strcmp(szHeadType, HEAD_TYPE_ITEM[i]) == 0){
			m_iHeadType = i;
			break;
		}
	}

	CDialog::OnOK();
}
