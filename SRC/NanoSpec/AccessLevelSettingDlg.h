#if !defined(AFX_ACCESSLEVELSETTINGDLG_H__263669BD_8F7F_4EAD_A4EA_74A92A2D1D13__INCLUDED_)
#define AFX_ACCESSLEVELSETTINGDLG_H__263669BD_8F7F_4EAD_A4EA_74A92A2D1D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccessLevelSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CAccessLevelSettingDlg ダイアログ
// ##########################################################################

class CAccessLevelSettingDlg : public CDialog
{
// コンストラクション
public:
	CAccessLevelSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CAccessLevelSettingDlg)
	enum { IDD = IDD_ACCESS_LEVEL_SETTING_DLG };
	//}}AFX_DATA
protected:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	//{{AFX_VIRTUAL(CAccessLevelSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAccessLevelSettingDlg)
	afx_msg BOOL OnInitDialog();
	afx_msg void OnSelchangeAccountLevel();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	ACCESS_LEVEL m_AccessLevel[ACCOUNT_LEVEL_MAX];

private:
	void UpdateCheckAccessLevel(int iAccountLevel);

	int m_iAccountLevel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ACCESSLEVELSETTINGDLG_H__263669BD_8F7F_4EAD_A4EA_74A92A2D1D13__INCLUDED_)
