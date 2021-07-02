#if !defined(AFX_SAMPLEIDDLG_H__87F8F600_67FC_45E3_AFD4_E7EB3C479F6A__INCLUDED_)
#define AFX_SAMPLEIDDLG_H__87F8F600_67FC_45E3_AFD4_E7EB3C479F6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SampleIdDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSampleIdDlg ダイアログ

class CSampleIdDlg : public CDialog
{
// コンストラクション
public:
	CSampleIdDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// 2009.10.08 bagus CTA 追加 --{--
	BOOL	m_bUseLotID;
// 2009.10.08 bagus CTA 追加 --}--
// 2009.10.22 bagus Stress 修正 --{--
	BOOL	m_bUseReferenceList;
// 2009.10.22 bagus Stress 修正 --}--

// ダイアログ データ
	//{{AFX_DATA(CSampleIdDlg)
	enum { IDD = IDD_SAMPLE_ID_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
public:
	///// Variable /////
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	CShadeButtonST m_OkButton, m_CancelButton;
// 2009.10.22 bagus Stress 修正 --{--
	CShadeButtonST m_ListButton;
// 2009.10.22 bagus Stress 修正 --}--

private:
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
	TCHAR m_tszSampleID[SAMPLE_ID_LEN + 1];
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
// 2009.10.08 bagus CTA 追加 --{--
	TCHAR m_tszLotID[LOT_ID_LEN + 1];
// 2009.10.08 bagus CTA 追加 --}--
	RCP_DATA m_rcp_data;

public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSampleIdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSampleIdDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnUpdateSampleIdEdit();
	afx_msg void OnUpdateLotIdEdit();
	afx_msg void OnList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAMPLEIDDLG_H__87F8F600_67FC_45E3_AFD4_E7EB3C479F6A__INCLUDED_)
