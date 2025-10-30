#if !defined(AFX_STRESSCOFIGURATIONSETTINGDLG_H__1CFA46E8_8BCC_4741_A483_7EE259D654C4__INCLUDED_)
#define AFX_STRESSCOFIGURATIONSETTINGDLG_H__1CFA46E8_8BCC_4741_A483_7EE259D654C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StressCofigurationSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CStressCofigurationSettingDlg ダイアログ

class CStressCofigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CStressCofigurationSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	STRESS_CONFIG m_StressConfig;

	BOOL CheckData();
	void UpDate(BOOL);

// ダイアログ データ
	//{{AFX_DATA(CStressCofigurationSettingDlg)
	enum { IDD = IDD_STRESS_CONFIGURATION_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStressCofigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL ShowSettingDlg(CDialog* pDlg);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStressCofigurationSettingDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRESSCOFIGURATIONSETTINGDLG_H__1CFA46E8_8BCC_4741_A483_7EE259D654C4__INCLUDED_)
