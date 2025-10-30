#if !defined(AFX_VIEWSCREENDLG_H__ED9C765E_FD23_442F_841B_56EF811F2183__INCLUDED_)
#define AFX_VIEWSCREENDLG_H__ED9C765E_FD23_442F_841B_56EF811F2183__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewScreenDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CViewScreenDlg ダイアログ
// ##########################################################################

class CViewScreenDlg : public CDialog
{
// コンストラクション
public:
	CViewScreenDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CViewScreenDlg)
	enum { IDD = IDD_VIEW_SCREEN_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;

// オーバーライド
	//{{AFX_VIRTUAL(CViewScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CViewScreenDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VIEWSCREENDLG_H__ED9C765E_FD23_442F_841B_56EF811F2183__INCLUDED_)
