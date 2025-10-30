#if !defined(AFX_USERSETTINGOPERATIONCTASETTINGDLG_H__548F2784_FAAF_4D1E_93A8_8BA0E8EDD855__INCLUDED_)
#define AFX_USERSETTINGOPERATIONCTASETTINGDLG_H__548F2784_FAAF_4D1E_93A8_8BA0E8EDD855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationCTASettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationCTASettingDlg ダイアログ

class CUserSettingOperationCTASettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationCTASettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationCTASettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_CTA_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationCTASettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	CShadeButtonST m_RecipeDefaultValueSettingButton;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationCTASettingDlg)
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

#endif // !defined(AFX_USERSETTINGOPERATIONCTASETTINGDLG_H__548F2784_FAAF_4D1E_93A8_8BA0E8EDD855__INCLUDED_)
