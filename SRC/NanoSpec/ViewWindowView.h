#if !defined(AFX_VIEWWINDOWVIEW_H__6B834F51_6519_46C5_BEF9_C2CEE04ABEAB__INCLUDED_)
#define AFX_VIEWWINDOWVIEW_H__6B834F51_6519_46C5_BEF9_C2CEE04ABEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewWindowView.h : ヘッダー ファイル
//
class CViewScreenDlg;

// ##########################################################################
// CViewWindowView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\\..\\INC\\Globals.hxx"
#include "LensComboBox.h"
#include "NanoUI.h"

class CViewWindowView : public CNanoUI
{
protected:
	CViewWindowView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CViewWindowView)

// フォーム データ
protected:
	//{{AFX_DATA(CViewWindowView)
	enum { IDD = IDD_VIEW_WINDOW_FORM };
	//}}AFX_DATA

	CShadeButtonST m_AutoFocusButton;
	//Saiki 20090908 Add ----->
	CShadeButtonST m_HomePositionButton;
	//Saiki 20090908 Add <-----

	CLensComboBox m_cbLens;
	CCameraComboBox m_cbCamera;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CViewWindowView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CViewWindowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CViewWindowView)
	afx_msg void OnPaint();
	afx_msg void OnAutoFocusButton();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHomePositionButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
// 2014.01.17 Bagus Add (Stage None対応) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage None対応) <--
	CNanoSpecDoc* m_pDoc;
	CViewScreenDlg* m_pdlgViewScreen;
	CRect m_rcCamera;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VIEWWINDOWVIEW_H__6B834F51_6519_46C5_BEF9_C2CEE04ABEAB__INCLUDED_)
