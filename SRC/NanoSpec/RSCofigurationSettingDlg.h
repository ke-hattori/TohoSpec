#if !defined(AFX_RSCOFIGURATIONSETTINGDLG_H__BB167165_656C_4DF4_9C09_6A2EF073081C__INCLUDED_)
#define AFX_RSCOFIGURATIONSETTINGDLG_H__BB167165_656C_4DF4_9C09_6A2EF073081C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RSCofigurationSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRSCofigurationSettingDlg ダイアログ

class CRSCofigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CRSCofigurationSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRSCofigurationSettingDlg)
	enum { IDD = IDD_RS_CONFIGURATION_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	RS_CONFIG m_RsConfig;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRSCofigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRSCofigurationSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitCombo_ProbeType();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RSCOFIGURATIONSETTINGDLG_H__BB167165_656C_4DF4_9C09_6A2EF073081C__INCLUDED_)
