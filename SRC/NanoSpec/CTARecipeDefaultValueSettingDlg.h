// CTARecipeDefaultValueSettingDlg.h : ヘッダー ファイル
//

#pragma once

// ##########################################################################
// CCTARecipeDefaultValueSettingDlg ダイアログ
// ##########################################################################

class CCTARecipeDefaultValueSettingDlg : public CDialog
{
// コンストラクション
public:
	CCTARecipeDefaultValueSettingDlg(CWnd* pParent = NULL);  // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCTARecipeDefaultValueSettingDlg)
	enum { IDD = IDD_SR_RECIPE_DEFAULT_VALUE_SETTING_DLG };
	//}}AFX_DATA

//Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_MainRecipeButton;
	CShadeButtonST m_MeasurementProgramButton;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCTARecipeDefaultValueSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCTARecipeDefaultValueSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMainRecipeButton();
	afx_msg void OnMeasurementProgramButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
