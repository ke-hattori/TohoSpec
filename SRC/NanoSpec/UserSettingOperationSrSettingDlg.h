#if !defined(AFX_USERSETTINGOPERATIONSRSETTINGDLG_H__94CF4FE1_2403_4E52_A8E7_67457D859AF6__INCLUDED_)
#define AFX_USERSETTINGOPERATIONSRSETTINGDLG_H__94CF4FE1_2403_4E52_A8E7_67457D859AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationSrSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CUserSettingOperationSrSettingDlg ダイアログ
// ##########################################################################

class CUserSettingOperationSrSettingDlg : public CDialog
{
// コンストラクション
public:
	CUserSettingOperationSrSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CUserSettingOperationSrSettingDlg)
	enum { IDD = IDD_USER_SETTING_OPERATION_SR_DLG };
	//}}AFX_DATA

protected:
	CShadeButtonST m_PatternRecSettingButton;
	CShadeButtonST m_ReferencePositionSettingButton;
	CShadeButtonST m_RecipeDefaultValueSettingButton;
	CShadeButtonST m_FigureFormatSettingButton;
	CShadeButtonST m_FocusRetryTimesSettingButton;
	CShadeButtonST m_DeleteSitePRImageButton;
// 2009.09.30 bagus Gantry --{--
	CShadeButtonST m_TransmittanceSettingButton;
// 2009.09.30 bagus Gantry --}--

	SR_CONFIG m_SrConfig;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationSrSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationSrSettingDlg)
	afx_msg void OnPatternRecSettingButton();
	afx_msg void OnReferencePositionSettingButton();
	afx_msg void OnRecipeDefaultValueSettingButton();
	afx_msg void OnFigureFormatSettingButton();
	afx_msg void OnFocusRetryTimesSettingButton();
	afx_msg void OnDeleteSitePRImageButton();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTransmittanceSettingButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGOPERATIONSRSETTINGDLG_H__94CF4FE1_2403_4E52_A8E7_67457D859AF6__INCLUDED_)
