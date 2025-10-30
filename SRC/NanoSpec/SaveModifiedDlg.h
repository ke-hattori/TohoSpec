#if !defined(AFX_SAVEMODIFIEDDLG_H__DF403955_4ABA_41CB_83AB_11A99D20EE67__INCLUDED_)
#define AFX_SAVEMODIFIEDDLG_H__DF403955_4ABA_41CB_83AB_11A99D20EE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveModifiedDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSaveModifiedDlg ダイアログ
// ##########################################################################

class CSaveModifiedDlg : public CDialog
{
// コンストラクション
public:
	CSaveModifiedDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	CSaveModifiedDlg(LPCTSTR pszCaption, CWnd* pParent = NULL);

// ダイアログ データ
	//{{AFX_DATA(CSaveModifiedDlg)
	enum { IDD = IDD_SAVE_MODIFIED_DLG };
	//}}AFX_DATA

// Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CString m_strModified;
	int m_iModified;

// オーバーライド
	//{{AFX_VIRTUAL(CSaveModifiedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSaveModifiedDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAVEMODIFIEDDLG_H__DF403955_4ABA_41CB_83AB_11A99D20EE67__INCLUDED_)
