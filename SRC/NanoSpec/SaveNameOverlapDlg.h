#if !defined(AFX_SAVENAMEOVERLAPDLG_H__C863F9C2_59F9_4D7F_9EC1_349A7EB22801__INCLUDED_)
#define AFX_SAVENAMEOVERLAPDLG_H__C863F9C2_59F9_4D7F_9EC1_349A7EB22801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveNameOverlapDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSaveNameOverlapDlg ダイアログ

class CSaveNameOverlapDlg : public CDialog
{
// コンストラクション
public:
	CSaveNameOverlapDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSaveNameOverlapDlg)
	enum { IDD = IDD_SAVE_NAME_OVERLAP };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


	char m_szSpecifiedFileName[128+1];


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSaveNameOverlapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSaveNameOverlapDlg)
	afx_msg void OnSave();
	afx_msg void OnCancel();
	afx_msg void OnSaveAs();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAVENAMEOVERLAPDLG_H__C863F9C2_59F9_4D7F_9EC1_349A7EB22801__INCLUDED_)
