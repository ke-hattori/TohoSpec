#if !defined(AFX_USERSETTINGOPERATIONRSSETTINGDLG_H__C75982AF_16CB_4085_816B_23AA7A7D6FDE__INCLUDED_)
#define AFX_USERSETTINGOPERATIONRSSETTINGDLG_H__C75982AF_16CB_4085_816B_23AA7A7D6FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationRSSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationRSSettingDlg ダイアログ

class CUserSettingOperationRSSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationRSSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationRSSettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_RS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationRSSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	CShadeButtonST m_RecipeDefaultValueSettingButton;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationRSSettingDlg)
	afx_msg void OnRecipeDefaultValueSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGOPERATIONRSSETTINGDLG_H__C75982AF_16CB_4085_816B_23AA7A7D6FDE__INCLUDED_)
