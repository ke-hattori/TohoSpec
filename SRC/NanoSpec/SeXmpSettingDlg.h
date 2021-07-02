#if !defined(AFX_SeXmpSettingDlg_H__2EAC0FE9_83C2_408A_9587_B467A885B5A6__INCLUDED_)
#define AFX_SeXmpSettingDlg_H__2EAC0FE9_83C2_408A_9587_B467A885B5A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeXmpSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSeXmpSettingDlg ダイアログ
// ##########################################################################

class CSeXmpSettingDlg : public CDialog
{
// コンストラクション
public:
	CSeXmpSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CSeXmpSettingDlg)
	enum { IDD = IDD_SE_XMP_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_StrategyListPathButton;
	CShadeButtonST m_DataPathButton;
	CShadeButtonST m_FittingDataPathButton;
	CShadeButtonST m_WvaseExePathButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSeXmpSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSeXmpSettingDlg)
	afx_msg void OnStrategyListPathButton();
	afx_msg void OnDataPathButton();
	afx_msg void OnFittingDataPathButton();
	afx_msg void OnWvaseExePathButton();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL CheckData();

	SR_XMP m_SrXmp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SeXmpSettingDlg_H__2EAC0FE9_83C2_408A_9587_B467A885B5A6__INCLUDED_)
