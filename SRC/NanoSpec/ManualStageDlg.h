#if !defined(AFX_MANUALSTAGEDLG_H__140DC5E9_5096_43EF_9E4D_D65024F214B7__INCLUDED_)
#define AFX_MANUALSTAGEDLG_H__140DC5E9_5096_43EF_9E4D_D65024F214B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualStageDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CManualStageDlg ダイアログ

class CManualStageDlg : public COverlayDialog
{
// コンストラクション
public:
	CManualStageDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

// 2009.10.05 bagus SE --{--
	char m_szRecipeName[RECIPE_NAME_LEN + 1];
	int m_nSampleMode;
// 2009.10.05 bagus SE --}--

// ダイアログ データ
	//{{AFX_DATA(CManualStageDlg)
	enum { IDD = IDD_OVERLAY_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CManualStageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CManualStageDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MANUALSTAGEDLG_H__140DC5E9_5096_43EF_9E4D_D65024F214B7__INCLUDED_)
