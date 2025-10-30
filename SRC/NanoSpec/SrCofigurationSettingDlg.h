#if !defined(AFX_SRCOFIGURATIONSETTINGDLG_H__2DE0B9E1_5307_4975_875A_D446DD5E6C8E__INCLUDED_)
#define AFX_SRCOFIGURATIONSETTINGDLG_H__2DE0B9E1_5307_4975_875A_D446DD5E6C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrCofigurationSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSrCofigurationSettingDlg ダイアログ
// ##########################################################################

class CSrCofigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrCofigurationSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrCofigurationSettingDlg)
	enum { IDD = IDD_SR_CONFIGURATION_SETTING_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	SR_CONFIG m_SrConfig;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrCofigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrCofigurationSettingDlg)
	afx_msg void OnCheckUv();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckOdMeasurement();
	afx_msg void OnCheckAutoFocus();
	afx_msg void OnCheckAutoFilter();
	afx_msg void OnCheckTransmittance();
	afx_msg void OnCheckGantryType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRCOFIGURATIONSETTINGDLG_H__2DE0B9E1_5307_4975_875A_D446DD5E6C8E__INCLUDED_)
