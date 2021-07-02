#if !defined(AFX_CTACOFIGURATIONSETTINGDLG_H__3A7DE5C1_E3FB_4141_8A34_66AA1A1275C8__INCLUDED_)
#define AFX_CTACOFIGURATIONSETTINGDLG_H__3A7DE5C1_E3FB_4141_8A34_66AA1A1275C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTACofigurationSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CCTACofigurationSettingDlg ダイアログ

class CCTACofigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CCTACofigurationSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCTACofigurationSettingDlg)
	enum { IDD = IDD_CTA_CONFIGURATION_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CA_CONFIG m_CaConfig;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCTACofigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCTACofigurationSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CTACOFIGURATIONSETTINGDLG_H__3A7DE5C1_E3FB_4141_8A34_66AA1A1275C8__INCLUDED_)
