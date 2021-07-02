#if !defined(AFX_MSCOFIGURATIONSETTINGDLG_H__2AF926E1_2A8B_449A_BA8C_CD0508BAC3B2__INCLUDED_)
#define AFX_MSCOFIGURATIONSETTINGDLG_H__2AF926E1_2A8B_449A_BA8C_CD0508BAC3B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsCofigurationSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMsCofigurationSettingDlg ダイアログ

class CMsCofigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CMsCofigurationSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMsCofigurationSettingDlg)
	enum { IDD = IDD_MS_CONFIGURATION_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	MS_CONFIG m_MsConfig;

	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMsCofigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMsCofigurationSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MSCOFIGURATIONSETTINGDLG_H__2AF926E1_2A8B_449A_BA8C_CD0508BAC3B2__INCLUDED_)
