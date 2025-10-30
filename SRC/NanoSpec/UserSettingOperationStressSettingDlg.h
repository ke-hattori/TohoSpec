#if !defined(AFX_USERSETTINGOPERATIONSTRESSSETTINGDLG_H__D0FFDF00_9511_4975_ACBA_E22BCC72F021__INCLUDED_)
#define AFX_USERSETTINGOPERATIONSTRESSSETTINGDLG_H__D0FFDF00_9511_4975_ACBA_E22BCC72F021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationStressSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserSettingOperationStressSettingDlg ダイアログ

class CUserSettingOperationStressSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationStressSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationStressSettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_STRESS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationStressSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	CShadeButtonST m_RecipeDefaultValueSettingButton;
// 2009.11.09 bagus Stress 追加変更 --{--
	CShadeButtonST m_SampleElasticModulusSettingButton;
// 2009.11.09 bagus Stress 追加変更 --}--

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationStressSettingDlg)
	afx_msg void OnRecipeDefaultValueSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSampleElasticmodulusSettingButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGOPERATIONSTRESSSETTINGDLG_H__D0FFDF00_9511_4975_ACBA_E22BCC72F021__INCLUDED_)
