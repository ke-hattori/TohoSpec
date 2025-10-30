#if !defined(AFX_SYSTEMOPERATIONSESETTINGDLG_H__ECD420D8_4D59_473F_8E05_40D0B5F1867A__INCLUDED_)
#define AFX_SYSTEMOPERATIONSESETTINGDLG_H__ECD420D8_4D59_473F_8E05_40D0B5F1867A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationSeSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationSeSettingDlg ダイアログ

class CSystemOperationSeSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationSeSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationSeSettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_SE_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_SeConfigurationSettingButton;
	CShadeButtonST m_SeXmpSettingButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationSeSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationSeSettingDlg)
	afx_msg void OnSeConfigurationSettingButton();
	afx_msg void OnSeXmpSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONSESETTINGDLG_H__ECD420D8_4D59_473F_8E05_40D0B5F1867A__INCLUDED_)
