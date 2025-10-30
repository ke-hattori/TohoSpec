#if !defined(AFX_SAMPLEIDCONDLG_H__B2B858C8_C72B_43CF_B747_2459BF4DA80F__INCLUDED_)
#define AFX_SAMPLEIDCONDLG_H__B2B858C8_C72B_43CF_B747_2459BF4DA80F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SampleIdConDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSampleIdConDlg ダイアログ

class CSampleIdConDlg : public CDialog
{
// コンストラクション
public:
	CSampleIdConDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSampleIdConDlg)
	enum { IDD = IDD_SAMPLE_ID_CON_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSampleIdConDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSampleIdConDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAMPLEIDCONDLG_H__B2B858C8_C72B_43CF_B747_2459BF4DA80F__INCLUDED_)
