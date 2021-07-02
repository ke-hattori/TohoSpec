#if !defined(AFX_SYSTEMOPERATIONMSSETTINGDLG_H__54FFD1AC_B51D_4FE6_A9A2_A5353CC3FA53__INCLUDED_)
#define AFX_SYSTEMOPERATIONMSSETTINGDLG_H__54FFD1AC_B51D_4FE6_A9A2_A5353CC3FA53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationMsSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationMsSettingDlg ダイアログ

class CSystemOperationMsSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationMsSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationMsSettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_MS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_MsConfigurationSettingButton;
	CShadeButtonST m_MsLensSettingButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationMsSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationMsSettingDlg)
	afx_msg void OnMsConfigurationSettingButton();
	afx_msg void OnMsRoboCylinderSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONMSSETTINGDLG_H__54FFD1AC_B51D_4FE6_A9A2_A5353CC3FA53__INCLUDED_)
