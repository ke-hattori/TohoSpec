#if !defined(AFX_LOGOFFDLG_H__154054F4_FD92_442B_9733_8FA2B5B5B745__INCLUDED_)
#define AFX_LOGOFFDLG_H__154054F4_FD92_442B_9733_8FA2B5B5B745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogOffDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CLogOffDlg ダイアログ
// ##########################################################################

class CLogOffDlg : public CDialog
{
// コンストラクション
public:
	CLogOffDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CLogOffDlg)
	enum { IDD = IDD_LOG_OFF_DLG };
	//}}AFX_DATA

//Opearation
public:
	CShadeButtonST m_OkButton, m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CLogOffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CLogOffDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_LOGOFFDLG_H__154054F4_FD92_442B_9733_8FA2B5B5B745__INCLUDED_)
