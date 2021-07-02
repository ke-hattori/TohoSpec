#if !defined(AFX_SRAUTOFOCUSRETRYTIMESDLG_H__D225B603_797D_42CB_8FCF_24AE1170D94B__INCLUDED_)
#define AFX_SRAUTOFOCUSRETRYTIMESDLG_H__D225B603_797D_42CB_8FCF_24AE1170D94B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrAutoFocusRetryTimesDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// SrAutoFocusRetryTimesDlg ダイアログ

class SrAutoFocusRetryTimesDlg : public CDialog
{
// コンストラクション
public:
	SrAutoFocusRetryTimesDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(SrAutoFocusRetryTimesDlg)
	enum { IDD = IDD_SR_AUTOFOCUS_RETRY_TIMES_DLG };
	UINT	m_RetryTimes;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(SrAutoFocusRetryTimesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(SrAutoFocusRetryTimesDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	BOOL CheckData();

	SR_AUTOFOCUS_RETRY m_SrAutoFocusRetry;
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRAUTOFOCUSRETRYTIMESDLG_H__D225B603_797D_42CB_8FCF_24AE1170D94B__INCLUDED_)
