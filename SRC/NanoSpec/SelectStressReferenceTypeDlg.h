#if !defined(AFX_SELECTSTRESSREFERENCETYPEDLG_H__8DC41D0B_5E00_4C2E_A635_A490ED3D654E__INCLUDED_)
#define AFX_SELECTSTRESSREFERENCETYPEDLG_H__8DC41D0B_5E00_4C2E_A635_A490ED3D654E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStressReferenceTypeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSelectStressReferenceTypeDlg ダイアログ

class CSelectStressReferenceTypeDlg : public CDialog
{
// コンストラクション
public:
	CSelectStressReferenceTypeDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	int	m_SelectedIndex;

// ダイアログ データ
	//{{AFX_DATA(CSelectStressReferenceTypeDlg)
	enum { IDD = IDD_SELECT_STRESS_REFERENCE_TYP_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSelectStressReferenceTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelectStressReferenceTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ReferenceTypeCombo_Init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SELECTSTRESSREFERENCETYPEDLG_H__8DC41D0B_5E00_4C2E_A635_A490ED3D654E__INCLUDED_)
