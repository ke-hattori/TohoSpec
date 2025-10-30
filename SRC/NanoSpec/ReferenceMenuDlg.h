#if !defined(AFX_REFERENCEMENUDLG_H__4E5238F4_995E_4DFA_9D0A_0261370D6B5F__INCLUDED_)
#define AFX_REFERENCEMENUDLG_H__4E5238F4_995E_4DFA_9D0A_0261370D6B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReferenceMenuDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"

// ##########################################################################
// CReferenceMenuDlg ダイアログ
// ##########################################################################

class CReferenceMenuDlg : public CDialog
{
// コンストラクション
public:
	CReferenceMenuDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CReferenceMenuDlg)
	enum { IDD = IDD_REFERENCE_MENU_DLG };
	//}}AFX_DATA

//Operation
public:
	///// Variable /////
	CColorStatic m_stcRecipeName;
	CColorStatic m_stcHeadType;
	CColorStatic m_stcMeasurementItem;
	CShadeButtonST m_GetReferenceButton;
	CShadeButtonST m_ClearReferenceButton;
	CShadeButtonST m_CancelButton;

	///// Function /////
	void ClearReference();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CReferenceMenuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CReferenceMenuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClearReferenceButton();
	afx_msg void OnGetReferenceButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_REFERENCEMENUDLG_H__4E5238F4_995E_4DFA_9D0A_0261370D6B5F__INCLUDED_)
