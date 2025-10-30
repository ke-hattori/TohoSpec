// SampleIdDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "SampleIdDlg.h"
#include "NanoSpecDoc.h"
#include "MainFrm.h"
// 2009.10.22 bagus Stress 修正 --{--
#include "StressReferenceListDlg.h"
// 2009.10.22 bagus Stress 修正 --}--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleIdDlg ダイアログ

/////////////////////////////////////////////////////////////////////////////
//
CSampleIdDlg::CSampleIdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleIdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleIdDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_pMainFrame	= NULL;
	m_pDoc			= NULL;
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
	_tcscpy(m_tszSampleID, _T(""));
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */

// 2009.10.08 bagus CTA 追加 --{--
	m_bUseLotID = FALSE;
// 2009.10.08 bagus CTA 追加 --}--

// 2009.10.22 bagus Stress 修正 --{--
	m_bUseReferenceList = FALSE;
// 2009.10.22 bagus Stress 修正 --}--
}

/////////////////////////////////////////////////////////////////////////////
//
void CSampleIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleIdDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
// 2009.10.22 bagus Stress 修正 --{--
	DDX_Control(pDX, IDC_LIST, m_ListButton);
// 2009.10.22 bagus Stress 修正 --}--
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CSampleIdDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleIdDlg)
	ON_EN_UPDATE(IDC_SAMPLE_ID_EDIT, OnUpdateSampleIdEdit)
	ON_EN_UPDATE(IDC_LOT_ID_EDIT, OnUpdateLotIdEdit)
	ON_BN_CLICKED(IDC_LIST, OnList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSampleIdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMainFrame = (CMainFrame *)AfxGetMainWnd();
	m_pDoc = (CNanoSpecDoc*)m_pMainFrame->GetActiveDocument();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	///// Cancel Button /////
	m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_CancelButton.DrawFlatFocus(TRUE);

// 2009.10.22 bagus Stress 修正 --{--
	///// List Button /////
	m_ListButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_ListButton.DrawFlatFocus(TRUE);

	if (m_bUseReferenceList) {
		GetDlgItem(IDC_LIST)->ShowWindow(SW_SHOW);
	} else {
		GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);
	}
// 2009.10.22 bagus Stress 修正 --}--

/* added 2014.05.22 hmenjo 手動時 lot ID 不要 ---------- { ---------- */
	CString l_strMsg;
	switch (g_lModelType) {
	case MODEL_T3100:
		this->GetDlgItem(IDC_LOT_ID_EDIT)->ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_STATIC_LOTID)->ShowWindow(SW_HIDE);
		LoadStringML(IDS_SAMPLE_ID_DLG, l_strMsg, "SAMPLE ID");
		this->SetWindowText(l_strMsg);
		break;
	}
/* added 2014.05.22 hmenjo 手動時 lot ID 不要 ---------- } ---------- */

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
					// 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/////////////////////////////////////////////////////////////////////////////
// CSampleIdDlg メッセージ ハンドラ

/////////////////////////////////////////////////////////////////////////////
//
void CSampleIdDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	char szSampleId[SAMPLE_ID_LEN + 1];
	memset(szSampleId, 0, sizeof(szSampleId));

	CString sSampId;
	CEdit* SampIdEdit = (CEdit*)GetDlgItem(IDC_SAMPLE_ID_EDIT);
	SampIdEdit->GetWindowText(sSampId);

	sSampId.TrimLeft();
	sSampId.TrimRight();
	//Saiki 20090601 Add ----->
	CString strBuffer, strTitle;
	//Saiki 20090601 Add <-----

	if((sSampId == "")||(sSampId.GetLength() > SAMPLE_ID_LEN))
	{
		//MessageBox("The input value is either blank or number of characters is over the limited.", "Confirmation", MB_OK); //入力値が空欄か文字数オーバーです。
		//Saiki 20090601 Change ----->
		//MessageBox("入力値が空欄 もしくは 入力文字数オーバーです", "Confirmation", MB_OK); //入力値が空欄か文字数オーバーです。
		LoadStringML(IDS_INPUT_VALUE_CHARA_OVER, strBuffer, "The input value is either blank or number of characters is over the limited.");
		LoadStringML(IDS_TITLE_CONFIRMATION, strTitle, "Confirmation");
		MessageBox(strBuffer, strTitle, MB_OK); //入力値が空欄か文字数オーバーです。
		//Saiki 20090601 Change <-----
		GetDlgItem(IDC_SAMPLE_ID_EDIT)->SetFocus();
		return;
	}
	strcpy( szSampleId, sSampId );

// 2009.10.08 bagus CTA 追加 --{--
	char szLotId[LOT_ID_LEN + 1];
	memset(szLotId, 0, sizeof(szLotId));

	CString sLotId;
	CEdit* LotIdEdit = (CEdit*)GetDlgItem(IDC_LOT_ID_EDIT);
	LotIdEdit->GetWindowText(sLotId);

	sLotId.TrimLeft();
	sLotId.TrimRight();

	if ((m_bUseLotID) && ((sLotId == "")||(sLotId.GetLength() > LOT_ID_LEN)))
	{
		LoadStringML(IDS_INPUT_VALUE_CHARA_OVER, strBuffer, "The input value is either blank or number of characters is over the limited.");
		LoadStringML(IDS_TITLE_CONFIRMATION, strTitle, "Confirmation");
		MessageBox(strBuffer, strTitle, MB_OK); //入力値が空欄か文字数オーバーです。
		GetDlgItem(IDC_LOT_ID_EDIT)->SetFocus();
		return;
	}
	strcpy( szLotId, sLotId );
// 2009.10.08 bagus CTA 追加 --}--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	m_pDoc->SetSampleIdFromUI(TRUE);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	m_pDoc->SetSampleId(szSampleId);
// 2009.10.08 bagus CTA 追加 --{--
	m_pDoc->SetLotId(szLotId);
// 2009.10.08 bagus CTA 追加 --}--

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
//
void CSampleIdDlg::OnCancel()
{

	CDialog::OnCancel();
}

/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
void CSampleIdDlg::OnUpdateSampleIdEdit()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO: Add your control notification handler code here

	int l_iSelStart, l_SelEnd;
	((CEdit*) this->GetDlgItem(IDC_SAMPLE_ID_EDIT))->GetSel(l_iSelStart, l_SelEnd);
	TCHAR l_tszSampleID[SAMPLE_ID_LEN + 1];
	this->GetDlgItemText(IDC_SAMPLE_ID_EDIT, l_tszSampleID, sizeof(l_tszSampleID));
	if ((0 == _tcscmp(l_tszSampleID, _T(""))) || (0 != CheckName(l_tszSampleID, _tcslen(l_tszSampleID)))) {
		/* 禁止文字は無かった．*/
		_tcscpy(m_tszSampleID, l_tszSampleID);
	} else {
		/* 禁止文字が有った．*/
		this->SetDlgItemText(IDC_SAMPLE_ID_EDIT, m_tszSampleID);
		int l_idLen = _tcslen(l_tszSampleID) - _tcslen(m_tszSampleID);
		if (l_idLen < 0) {l_idLen *= -1;}
		switch (l_idLen) {
		case 0:
			break;
		case 1:
			if (0 < l_iSelStart) {l_iSelStart--;}
			if (0 < l_SelEnd) {l_SelEnd--;}
			break;
		case 2:
			if (2 <= l_iSelStart) {l_iSelStart -= 2;}
			if (2 <= l_SelEnd) {l_SelEnd -= 2;}
			break;
		default:
			l_iSelStart = -1;
			l_SelEnd = -1;
			break;
		}
		((CEdit*) this->GetDlgItem(IDC_SAMPLE_ID_EDIT))->SetSel(l_iSelStart, l_SelEnd, FALSE);
	}
}
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */

// 2009.10.08 bagus CTA 追加 --{--
void CSampleIdDlg::OnUpdateLotIdEdit()
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_UPDATE フラグ付きで EM_SETEVENTMASK
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	int l_iSelStart, l_SelEnd;
	((CEdit*) this->GetDlgItem(IDC_LOT_ID_EDIT))->GetSel(l_iSelStart, l_SelEnd);
	TCHAR l_tszLotID[LOT_ID_LEN + 1];
	this->GetDlgItemText(IDC_LOT_ID_EDIT, l_tszLotID, sizeof(l_tszLotID));
	if ((m_bUseLotID) && (0 == _tcscmp(l_tszLotID, _T("")))) {
		/* LotIDを使用するため、空文字禁止 */
	} else if ((0 == _tcscmp(l_tszLotID, _T(""))) || (0 != CheckName(l_tszLotID, _tcslen(l_tszLotID)))) {
		/* 禁止文字は無かった．*/
		_tcscpy(m_tszLotID, l_tszLotID);
	} else {
		/* 禁止文字が有った．*/
		this->SetDlgItemText(IDC_LOT_ID_EDIT, m_tszLotID);
		int l_idLen = _tcslen(l_tszLotID) - _tcslen(m_tszLotID);
		if (l_idLen < 0) {l_idLen *= -1;}
		switch (l_idLen) {
		case 0:
			break;
		case 1:
			if (0 < l_iSelStart) {l_iSelStart--;}
			if (0 < l_SelEnd) {l_SelEnd--;}
			break;
		case 2:
			if (2 <= l_iSelStart) {l_iSelStart -= 2;}
			if (2 <= l_SelEnd) {l_SelEnd -= 2;}
			break;
		default:
			l_iSelStart = -1;
			l_SelEnd = -1;
			break;
		}
		((CEdit*) this->GetDlgItem(IDC_LOT_ID_EDIT))->SetSel(l_iSelStart, l_SelEnd, FALSE);
	}
}
// 2009.10.08 bagus CTA 追加 --}--

// 2009.10.22 bagus Stress 追加 --{--
void CSampleIdDlg::OnList()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CStressReferenceListDlg	dlg;

	dlg.m_bListMode = TRUE;
	if (dlg.DoModal() == IDOK) {
		SetDlgItemText(IDC_SAMPLE_ID_EDIT, dlg.m_strSampleID);
	}
}
// 2009.10.22 bagus Stress 追加 --}--
