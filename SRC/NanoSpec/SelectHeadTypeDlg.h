#if !defined(AFX_SELECTHEADTYPEDLG_H__654F6818_687C_47E2_A40A_DA568D3F0C6B__INCLUDED_)
#define AFX_SELECTHEADTYPEDLG_H__654F6818_687C_47E2_A40A_DA568D3F0C6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectHeadTypeDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSelectHeadTypeDlg ダイアログ
// ##########################################################################

class CSelectHeadTypeDlg : public CDialog
{
// コンストラクション
public:
	CSelectHeadTypeDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSelectHeadTypeDlg)
	enum { IDD = IDD_SELECT_HEAD_TYPE_DLG };
	//}}AFX_DATA

//Opration
public:
	CShadeButtonST m_OkButton, m_CancelButton;
	int m_iHeadType;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSelectHeadTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelectHeadTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SELECTHEADTYPEDLG_H__654F6818_687C_47E2_A40A_DA568D3F0C6B__INCLUDED_)
