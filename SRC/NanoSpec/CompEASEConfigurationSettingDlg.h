#pragma once

// CompEASEConfigurationSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CCompEASEConfigurationSettingDlg ダイアログ

class CCompEASEConfigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CCompEASEConfigurationSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCompEASEConfigurationSettingDlg)
	enum { IDD = IDD_COMPEASE_CONFIGURATION_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCompEASEConfigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCompEASEConfigurationSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	COMPEASE_CONFIG m_CompEaseConfig;
};
