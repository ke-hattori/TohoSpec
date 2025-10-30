#if !defined(AFX_DATALISTVIEW_H__75EA8A1A_46C0_4AEF_A095_AFC8BF6FFE2C__INCLUDED_)
#define AFX_DATALISTVIEW_H__75EA8A1A_46C0_4AEF_A095_AFC8BF6FFE2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListView.h : ヘッダー ファイル
//

// ##########################################################################
// CDataListView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoListUI.h"

class CDataListView : public CNanoListUI
{
protected:
	CDataListView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CDataListView();
	DECLARE_DYNCREATE(CDataListView)

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRecipeMainListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	///// Functions /////
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void List_HeaderSet();
	virtual void List_DataSet();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDataListView)
	afx_msg void OnClickDataList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDataList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnReturnDataList(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

private:
	CToolBar m_wndDataBar;
	void List_Add(char* Name, char* Head, char* MeasItem, char* SampleId, int Point, char* Date);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DATALISTVIEW_H__75EA8A1A_46C0_4AEF_A095_AFC8BF6FFE2C__INCLUDED_)
