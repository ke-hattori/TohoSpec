#if !defined(AFX_USERACCOUNTSETTINGDLG_H__334C0329_04B0_4C6A_8207_3F652216146A__INCLUDED_)
#define AFX_USERACCOUNTSETTINGDLG_H__334C0329_04B0_4C6A_8207_3F652216146A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// UserAccountSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CUserAccountSettingDlg ダイアログ                                       //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

class CUserAccountSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserAccountSettingDlg(BOOL bNew, char* lpszName = NULL, CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CUserAccountSettingDlg)
	enum { IDD = IDD_USER_ACCOUNT_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserAccountSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserAccountSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAccountLevel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitCombo_AccountLevel();
	BOOL CheckData();

	USER_ACCOUNT m_UserAccount;
	ACCESS_LEVEL m_AccessLevel[ACCOUNT_LEVEL_MAX];
	BOOL m_bNew;
	CString m_strConfirmPassword;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERACCOUNTSETTINGDLG_H__334C0329_04B0_4C6A_8207_3F652216146A__INCLUDED_)
