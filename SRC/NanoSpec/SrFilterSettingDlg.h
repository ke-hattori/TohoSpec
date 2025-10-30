#if !defined(AFX_SRFILTERSETTINGDLG_H__511115DB_9E6E_4CE6_903D_5365A8A7B46C__INCLUDED_)
#define AFX_SRFILTERSETTINGDLG_H__511115DB_9E6E_4CE6_903D_5365A8A7B46C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrFilterSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSrFilterSettingDlg ダイアログ
// ##########################################################################

class CSrFilterSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrFilterSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrFilterSettingDlg)
	enum { IDD = IDD_SR_FILTER_SETTING_DLG };
	//}}AFX_DATA

//Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	SR_FILTER m_SrFilter[SR_FILTER_MAX];
	CString m_strName;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrFilterSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrFilterSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckFilterEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRFILTERSETTINGDLG_H__511115DB_9E6E_4CE6_903D_5365A8A7B46C__INCLUDED_)
