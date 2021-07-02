#if !defined(AFX_USERSETTINGOPERATIONSESETTINGDLG_H__2DBEA1CB_C87C_4CDC_B6A4_BA999A48F168__INCLUDED_)
#define AFX_USERSETTINGOPERATIONSESETTINGDLG_H__2DBEA1CB_C87C_4CDC_B6A4_BA999A48F168__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationSeSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationSeSettingDlg ダイアログ

class CUserSettingOperationSeSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationSeSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationSeSettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_SE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationSeSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	CShadeButtonST m_RecipeDefaultValueSettingButton;
	CShadeButtonST m_FigureFormatSettingButton;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationSeSettingDlg)
	afx_msg void OnRecipeDefaultValueSettingButton();
	afx_msg void OnFigureFormatSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGOPERATIONSESETTINGDLG_H__2DBEA1CB_C87C_4CDC_B6A4_BA999A48F168__INCLUDED_)
