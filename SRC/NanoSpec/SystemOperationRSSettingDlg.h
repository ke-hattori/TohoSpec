#if !defined(AFX_SYSTEMOPERATIONRSSETTINGDLG_H__283894D4_62D1_4F0D_8693_2781C771A097__INCLUDED_)
#define AFX_SYSTEMOPERATIONRSSETTINGDLG_H__283894D4_62D1_4F0D_8693_2781C771A097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationRSSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationRSSettingDlg ダイアログ

class CSystemOperationRSSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationRSSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationRSSettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_RS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_RsConfigurationSettingButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationRSSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationRSSettingDlg)
	afx_msg void OnRsConfigurationSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONRSSETTINGDLG_H__283894D4_62D1_4F0D_8693_2781C771A097__INCLUDED_)
