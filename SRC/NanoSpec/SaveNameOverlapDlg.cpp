// SaveNameOverlapDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "SaveNameOverlapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveNameOverlapDlg ダイアログ


CSaveNameOverlapDlg::CSaveNameOverlapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveNameOverlapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveNameOverlapDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	memset(m_szSpecifiedFileName, 0, sizeof(m_szSpecifiedFileName));

}


void CSaveNameOverlapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveNameOverlapDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveNameOverlapDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveNameOverlapDlg)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_SAVE_AS, OnSaveAs)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveNameOverlapDlg メッセージ ハンドラ


BOOL CSaveNameOverlapDlg::OnInitDialog()
{
	// Kojika 20090601 Add
	CString l_strBuffer;
	// Kojika 20090601 Add End

	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);

	char l_szMessage[1024+1];
	memset(l_szMessage, 0, sizeof(l_szMessage));
	// Kojika 20090601 Change
/*	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, "\n already exists, Do you want to replace it?"); */
/*	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, "\nこのファイルは既に存在します。 上書きしますか？"); */
//	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, "\nこのファイルは既に存在します。 上書きしますか？\n(保存に数十秒かかる場合があります。)");
	LoadStringML(IDS_FILE_ALREADY_EXIST, l_strBuffer, "\n already exists, Do you want to replace it?");
	sprintf(l_szMessage, "%s%s", m_szSpecifiedFileName, (LPCTSTR)l_strBuffer);
	// Kojika 20090601 Change End
	GetDlgItem(IDC_SAVE_NAME_OVERLAP_TEXT)->SetWindowText(l_szMessage);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



void CSaveNameOverlapDlg::OnSave()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EndDialog(SAVEOVERLAP_SAVE); //上書き保存(SAVE)
}

void CSaveNameOverlapDlg::OnCancel()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EndDialog(SAVEOVERLAP_NOSAVE); //保存しない(CANCEL)
}

void CSaveNameOverlapDlg::OnSaveAs()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EndDialog(SAVEOVERLAP_SAVEAS); //名前を付けて保存(SAVE AS)
}
