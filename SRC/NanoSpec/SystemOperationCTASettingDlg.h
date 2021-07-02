#if !defined(AFX_SYSTEMOPERATIONCTASETTINGDLG_H__8114F314_7AEA_438F_8F9D_9F767D6B9AA8__INCLUDED_)
#define AFX_SYSTEMOPERATIONCTASETTINGDLG_H__8114F314_7AEA_438F_8F9D_9F767D6B9AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationCTASettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationCTASettingDlg ダイアログ

class CSystemOperationCTASettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationCTASettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationCTASettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_CTA_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_CtaConfigurationSettingButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationCTASettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationCTASettingDlg)
	afx_msg void OnCtaConfigurationSettingButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONCTASETTINGDLG_H__8114F314_7AEA_438F_8F9D_9F767D6B9AA8__INCLUDED_)
