#if !defined(AFX_STRESSSELECTSAMPLETYPEDLG_H__E21ADD78_80AB_48C8_8E04_0809ACB63C0F__INCLUDED_)
#define AFX_STRESSSELECTSAMPLETYPEDLG_H__E21ADD78_80AB_48C8_8E04_0809ACB63C0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StressSelectSampleTypeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CStressSelectSampleTypeDlg ダイアログ

class CStressSelectSampleTypeDlg : public CDialog
{
// コンストラクション
public:
	CStressSelectSampleTypeDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	char m_szName[RECIPE_NAME_LEN + 1];

// ダイアログ データ
	//{{AFX_DATA(CStressSelectSampleTypeDlg)
	enum { IDD = IDD_STRESS_SELECT_SAMPLE_TYPE };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStressSelectSampleTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStressSelectSampleTypeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL SampleTypeCombo_Init();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRESSSELECTSAMPLETYPEDLG_H__E21ADD78_80AB_48C8_8E04_0809ACB63C0F__INCLUDED_)
