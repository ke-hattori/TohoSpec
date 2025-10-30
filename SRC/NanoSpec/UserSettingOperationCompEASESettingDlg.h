#pragma once

// UserSettingOperationCompEASESettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationCompEASESettingDlg ダイアログ

class CUserSettingOperationCompEASESettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationCompEASESettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationCompEASESettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_COMPEASE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationCompEASESettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	CShadeButtonST m_RecipeDefaultValueSettingButton;
	CShadeButtonST m_FigureFormatSettingButton;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationCompEASESettingDlg)
	afx_msg void OnRecipeDefaultValueSettingButton();
	afx_msg void OnFigureFormatSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};
