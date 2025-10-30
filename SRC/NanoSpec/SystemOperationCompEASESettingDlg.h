#pragma once

// SystemOperationCompEASESettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationCompEASESettingDlg ダイアログ

class CSystemOperationCompEASESettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationCompEASESettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationCompEASESettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_COMPEASE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_EASEConfigurationSettingButton;
	CShadeButtonST m_EASEIPAddressSettingButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationCompEASESettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationCompEASESettingDlg)
	afx_msg void OnEASEConfigurationSettingButton();
	afx_msg void OnEASEIPAddressSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};
