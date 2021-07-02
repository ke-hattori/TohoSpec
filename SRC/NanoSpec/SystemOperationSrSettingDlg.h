#if !defined(AFX_SYSTEMOPERATIONSRSETTINGDLG_H__E31FE4EA_40E2_4A30_81E8_02DE4792D094__INCLUDED_)
#define AFX_SYSTEMOPERATIONSRSETTINGDLG_H__E31FE4EA_40E2_4A30_81E8_02DE4792D094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationSrSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSystemOperationSrSettingDlg ダイアログ
// ##########################################################################

class CSystemOperationSrSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemOperationSrSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSystemOperationSrSettingDlg)
	enum { IDD = IDD_SYSTEM_OPERATION_SR_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_SrConfigurationSettingButton;
	CShadeButtonST m_FilterSettingButton;
	CShadeButtonST m_TurretSettingButton;
	CShadeButtonST m_XmpSettingButton;
	CShadeButtonST m_ViewRangeSettingButton;
	CShadeButtonST m_LensSettingButton;
	CShadeButtonST m_VideoCalibSettingButton;
	CShadeButtonST m_CcdArraySettingButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationSrSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationSrSettingDlg)
	afx_msg void OnSrConfigurationSettingButton();
	afx_msg void OnFilterSettingButton();
	afx_msg void OnTurretSettingButton();
	afx_msg void OnXmpSettingButton();
	afx_msg void OnViewRangeSettingButton();
	afx_msg void OnLensSettingButton();
	afx_msg void OnCcdArraySettingButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnVideoCalibSettingButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ShowSettingDlg(CDialog* pDlg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONSRSETTINGDLG_H__E31FE4EA_40E2_4A30_81E8_02DE4792D094__INCLUDED_)
