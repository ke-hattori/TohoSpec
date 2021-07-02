#if !defined(AFX_DATALISTOPERATIONVIEW_H__F1EBF391_83B2_435C_9445_8F366C256CB5__INCLUDED_)
#define AFX_DATALISTOPERATIONVIEW_H__F1EBF391_83B2_435C_9445_8F366C256CB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListOperationView.h : ヘッダー ファイル
//

// ##########################################################################
// CDataListOperationView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CDataListOperationView : public CNanoUI
{
protected:
	CDataListOperationView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CDataListOperationView)

// フォーム データ
public:
	//{{AFX_DATA(CDataListOperationView)
	enum { IDD = IDD_DATA_LIST_OPERATION_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_ReviewButton;
	CShadeButtonST m_SearchButton;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_ImportButton;
	CShadeButtonST m_ExportButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDataListOperationView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDataListOperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDataListOperationView)
	afx_msg void OnDataSearchButton();
	afx_msg void OnDataReviewButton();
	afx_msg void OnDataExportButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL IsAccessPrivilege();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DATALISTOPERATIONVIEW_H__F1EBF391_83B2_435C_9445_8F366C256CB5__INCLUDED_)
