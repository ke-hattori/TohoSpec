#if !defined(AFX_SYSTEMOPERATIONSTRESSSETTINGDLG_H__DA6BBE2C_4E43_4B53_A3F1_EDD328AD2411__INCLUDED_)
#define AFX_SYSTEMOPERATIONSTRESSSETTINGDLG_H__DA6BBE2C_4E43_4B53_A3F1_EDD328AD2411__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationStressSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSystemOperationStressSettingDlg ダイアログ

class CSystemOperationStressSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationStressSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationStressSettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_STRESS_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_StressConfigButton;
// 2009.11.09 bagus Stress 追加変更 --{--
//	CShadeButtonST m_SampleElasticModulusSettingButton;
// 2009.11.09 bagus Stress 追加変更 --}--
	CShadeButtonST m_LineSectionSettingButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationStressSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationStressSettingDlg)
	afx_msg void OnStressConfigurationSettingButton();
	afx_msg void OnLineSectionSettingButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONSTRESSSETTINGDLG_H__DA6BBE2C_4E43_4B53_A3F1_EDD328AD2411__INCLUDED_)
