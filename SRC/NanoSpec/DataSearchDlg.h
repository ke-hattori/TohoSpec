#if !defined(AFX_DATASEARCHDLG_H__84729FAC_254D_4527_AD08_BDDCA6D5D297__INCLUDED_)
#define AFX_DATASEARCHDLG_H__84729FAC_254D_4527_AD08_BDDCA6D5D297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSearchDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CDataSearchDlg ダイアログ
// ##########################################################################

class CDataSearchDlg : public CDialog
{
// コンストラクション
public:
	CDataSearchDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDataSearchDlg)
	enum { IDD = IDD_DATA_SEARCH_DLG };
	//}}AFX_DATA

//Operations

public:
	CShadeButtonST m_SearchButton, m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDataSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDataSearchDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DATASEARCHDLG_H__84729FAC_254D_4527_AD08_BDDCA6D5D297__INCLUDED_)
