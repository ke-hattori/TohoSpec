#if !defined(AFX_FOCUSCONFIGURATIONSETTINGDLG_H__2E85C759_81C4_4CD3_B934_32D870D4D0A2__INCLUDED_)
#define AFX_FOCUSCONFIGURATIONSETTINGDLG_H__2E85C759_81C4_4CD3_B934_32D870D4D0A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FocusConfigurationSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CFocusConfigurationSettingDlg ダイアログ
// ##########################################################################

class CFocusConfigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CFocusConfigurationSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CFocusConfigurationSettingDlg)
	enum { IDD = IDD_FOCUS_CONFIGURATION_SETTING_DLG };
	FOCUS_CONFIG m_FocusConfig;
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	//{{AFX_VIRTUAL(CFocusConfigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CFocusConfigurationSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckZEncoder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL CheckData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FOCUSCONFIGURATIONSETTINGDLG_H__2E85C759_81C4_4CD3_B934_32D870D4D0A2__INCLUDED_)
