// SeRecipeDefaultValueSettingDlg.h : ヘッダー ファイル
//

#pragma once

// ##########################################################################
// CSeRecipeDefaultValueSettingDlg ダイアログ
// ##########################################################################

class CSeRecipeDefaultValueSettingDlg : public CDialog
{
// コンストラクション
public:
	CSeRecipeDefaultValueSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSeRecipeDefaultValueSettingDlg)
	enum { IDD = IDD_SR_RECIPE_DEFAULT_VALUE_SETTING_DLG };
	//}}AFX_DATA

//Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_MainRecipeButton;
	CShadeButtonST m_MeasurementProgramButton;
	int			   m_DefaultHeadType;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSeRecipeDefaultValueSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSeRecipeDefaultValueSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMainRecipeButton();
	afx_msg void OnMeasurementProgramButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
