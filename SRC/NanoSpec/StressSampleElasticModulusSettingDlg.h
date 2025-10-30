#if !defined(AFX_STRESSSAMPLEELASTICMODULUSSETTINGDLG_H__E20D4EF5_896B_4EAE_B4C0_BDF7A0B59D82__INCLUDED_)
#define AFX_STRESSSAMPLEELASTICMODULUSSETTINGDLG_H__E20D4EF5_896B_4EAE_B4C0_BDF7A0B59D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StressSampleElasticModulusSettingDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CStressSampleElasticModulusSettingDlg ダイアログ

class CStressSampleElasticModulusSettingDlg : public CDialog
{
// コンストラクション
public:
	BOOL m_bNewItem;
	STRESS_MODULUS m_StressModulus;

	CStressSampleElasticModulusSettingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// ダイアログ データ
	//{{AFX_DATA(CStressSampleElasticModulusSettingDlg)
	enum { IDD = IDD_STRESS_SAMPLE_ELASTICMODULUS_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStressSampleElasticModulusSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStressSampleElasticModulusSettingDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CheckData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRESSSAMPLEELASTICMODULUSSETTINGDLG_H__E20D4EF5_896B_4EAE_B4C0_BDF7A0B59D82__INCLUDED_)
