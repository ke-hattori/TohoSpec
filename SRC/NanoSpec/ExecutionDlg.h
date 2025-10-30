#if !defined(AFX_EXECUTIONDLG_H__F159916F_2468_48F8_BE21_F1ADAC0061FE__INCLUDED_)
#define AFX_EXECUTIONDLG_H__F159916F_2468_48F8_BE21_F1ADAC0061FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExecutionDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CExecutionDlg ダイアログ

class CExecutionDlg : public CDialog
{
// コンストラクション
public:
	CExecutionDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual ~CExecutionDlg();

// ダイアログ データ
	//{{AFX_DATA(CExecutionDlg)
	enum { IDD = IDD_EXECUTION_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
public:
	///// Variable /////
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	CFont m_Font;

	char m_szCaption[MAX_PATH + 1];
	char m_szMessage[MAX_PATH + 1];

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	///// Function /////
	void FontCreation();
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);
	void SetCaption(LPCTSTR lpszCaption);
	void SetMessage(LPCTSTR lpszMessage);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CExecutionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CExecutionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXECUTIONDLG_H__F159916F_2468_48F8_BE21_F1ADAC0061FE__INCLUDED_)
