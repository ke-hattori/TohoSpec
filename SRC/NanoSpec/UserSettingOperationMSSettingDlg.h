#if !defined(AFX_USERSETTINGOPERATIONMSSETTINGDLG_H__FE6A3B2D_2368_48F0_9E56_AEF10EBE5F00__INCLUDED_)
#define AFX_USERSETTINGOPERATIONMSSETTINGDLG_H__FE6A3B2D_2368_48F0_9E56_AEF10EBE5F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationMSSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationMSSettingDlg ダイアログ

class CUserSettingOperationMSSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationMSSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationMSSettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_MS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationMSSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	CShadeButtonST m_RecipeDefaultValueSettingButton;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationMSSettingDlg)
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

#endif // !defined(AFX_USERSETTINGOPERATIONMSSETTINGDLG_H__FE6A3B2D_2368_48F0_9E56_AEF10EBE5F00__INCLUDED_)
