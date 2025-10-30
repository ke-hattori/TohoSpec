// ManuReMeasReCalcStatDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "ManuReMeasReCalcStatDlg.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int lg_iID[16] = {
		IDC_RADIO_MRCS_1,	/* bit 0	*/
		IDC_RADIO_MRCS_2,	/* bit 1	*/
		IDC_RADIO_MRCS_3,	/* bit 2	*/
		IDC_RADIO_MRCS_4,	/* bit 3	*/
		IDC_RADIO_MRCS_5,	/* bit 4	*/
		-1,					/* bit 5	*/
		-1,					/* bit 6	*/
		-1,					/* bit 7	*/
		-1,					/* bit 8	*/
		-1,					/* bit 9	*/
		-1,					/* bit 10	*/
		-1,					/* bit 11	*/
		IDC_RADIO_MRCS_13,	/* bit 12	*/
		IDC_RADIO_MRCS_14,	/* bit 13	*/
		-1,					/* bit 14	*/
		-1,					/* bit 15	*/
	};

/////////////////////////////////////////////////////////////////////////////
// CManuReMeasReCalcStatDlg ダイアログ


CManuReMeasReCalcStatDlg::CManuReMeasReCalcStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManuReMeasReCalcStatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManuReMeasReCalcStatDlg)
	//}}AFX_DATA_INIT
	m_wClickLine = 0;
	m_wEnableSelect = 0;
}


void CManuReMeasReCalcStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManuReMeasReCalcStatDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManuReMeasReCalcStatDlg, CDialog)
	//{{AFX_MSG_MAP(CManuReMeasReCalcStatDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_MRCS_RUN, OnBtnMRCS_Run)
	ON_BN_CLICKED(IDC_BTN_MRCS_CANCEL, OnBtnMRCS_Cancel)
	ON_BN_CLICKED(IDC_RADIO_MRCS_1, OnRadioMRCS_1)
	ON_BN_CLICKED(IDC_RADIO_MRCS_2, OnRadioMRCS_2)
	ON_BN_CLICKED(IDC_RADIO_MRCS_3, OnRadioMRCS_3)
	ON_BN_CLICKED(IDC_RADIO_MRCS_4, OnRadioMRCS_4)
	ON_BN_CLICKED(IDC_RADIO_MRCS_5, OnRadioMRCS_5)
	ON_BN_CLICKED(IDC_RADIO_MRCS_13, OnRadioMRCS_13)
	ON_BN_CLICKED(IDC_RADIO_MRCS_14, OnRadioMRCS_14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManuReMeasReCalcStatDlg メッセージ ハンドラ

BOOL CManuReMeasReCalcStatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	m_wSelectNo = 0xffff;
	CString l_strClickLine;
	l_strClickLine.Format(_T("%d"), m_wClickLine);
	this->SetDlgItemText(IDC_STATIC_MRCS_LINE , l_strClickLine);
	BOOL l_bDefFirst = FALSE;
	WORD l_wBitPos;
	for (int i = 0; i < (sizeof(lg_iID) / sizeof(lg_iID[0])); i++) {
		if (-1 != lg_iID[i]) {
			l_wBitPos = 0x0001 << i;
			if (0 != (m_wEnableSelect & l_wBitPos)) {
				this->GetDlgItem(lg_iID[i])->EnableWindow(TRUE);
				if (FALSE == l_bDefFirst) {
					/* 最初のイネーブル指定のみオン状態にします．	*/
					l_bDefFirst = TRUE;
					((CButton*) (this->GetDlgItem(lg_iID[i])))->SetCheck(1);
					this->SetRunButtonText(i + 1);
				}
			} else {
				this->GetDlgItem(lg_iID[i])->EnableWindow(FALSE);
			}
		}
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CManuReMeasReCalcStatDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください

	/* "リターン"キーで終了するのを回避するため削除	*/
//	CDialog::OnOK();
}

void CManuReMeasReCalcStatDlg::OnCancel()
{
	// TODO: この位置に特別な後処理を追加してください。

	/* "ESC"キーで終了するのを回避するため削除	*/
//	CDialog::OnCancel();
}

void CManuReMeasReCalcStatDlg::OnClose()
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	CDialog::OnClose();
}

void CManuReMeasReCalcStatDlg::OnBtnMRCS_Run()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CString l_strConfirm;
	LoadStringML(IDS_MRCS_CONFIRM, l_strConfirm, "Are you sure ??");
//	CString l_strCaption;
//	this->GetWindowText(l_strCaption);
	if (IDYES != AfxMessageBox(l_strConfirm, MB_YESNO | MB_ICONQUESTION)) {
		return;
	}

	for (int i = 0; i < (sizeof(lg_iID) / sizeof(lg_iID[0])); i++) {
		if (-1 != lg_iID[i]) {
			if (0 != ((CButton*) (this->GetDlgItem(lg_iID[i])))->GetCheck()) {
				/* 最初のオンのみ取得します．	*/
				m_wSelectNo = i + 1;
				break;
			}
		}
	}

	CDialog::OnOK();
}

void CManuReMeasReCalcStatDlg::OnBtnMRCS_Cancel()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CDialog::OnCancel();
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_1()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(1);
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_2()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(2);
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_3()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(3);
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_4()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(4);
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_5()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(5);
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_13()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(13);
}

void CManuReMeasReCalcStatDlg::OnRadioMRCS_14()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	this->SetRunButtonText(14);
}

BOOL CManuReMeasReCalcStatDlg::ParamSet(LPARAM lParam/* = 0*/)
{
	m_wClickLine = HIWORD(lParam);
	m_wEnableSelect = LOWORD(lParam);

	if (0 == m_wClickLine) {
		return FALSE;
	}
	if (0 == m_wEnableSelect) {
		return FALSE;
	}

	return TRUE;
}

BOOL CManuReMeasReCalcStatDlg::ParamGet(LPARAM *plParam/* = 0*/)
{
	if (0 == plParam) {
		return FALSE;
	}

	*plParam = MAKELPARAM(m_wSelectNo, m_wClickLine);

	return TRUE;
}

void CManuReMeasReCalcStatDlg::SetRunButtonText(long lNo)
{
	CString l_strRunButton;
	switch (lNo) {
	case 1:		LoadStringML(IDS_MRCS_INCLUDE,		l_strRunButton, "Include");	break;
	case 2:		LoadStringML(IDS_MRCS_EXCLUDE,		l_strRunButton, "Exclude");	break;
	case 3:		LoadStringML(IDS_MRCS_DELETE,		l_strRunButton, "Delete");	break;
	case 4:		LoadStringML(IDS_MRCS_RESTORE,		l_strRunButton, "Restore");	break;
	case 5:		LoadStringML(IDS_MRCS_REMEASURE,	l_strRunButton, "Re-Measure");	break;
	case 13:	LoadStringML(IDS_MRCS_ALLRESTORE,	l_strRunButton, "All Restore");	break;
	case 14:	LoadStringML(IDS_MRCS_ALLRELEASE,	l_strRunButton, "All Release");	break;
	default:
		return;
		break;
	}
	this->SetDlgItemText(IDC_BTN_MRCS_RUN, l_strRunButton);
}
