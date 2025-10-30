#if !defined(AFX_ALARMHISTORYLISTVIEW_H__F8F9045E_BD7B_4F66_AB28_9A91B9E869E0__INCLUDED_)
#define AFX_ALARMHISTORYLISTVIEW_H__F8F9045E_BD7B_4F66_AB28_9A91B9E869E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlarmHistoryListView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAlarmHistoryListView フォーム ビュー                                   //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoListUI.h"

/////////////////////////////////////////////////////////////////////////////
//
class CAlarmHistoryListView : public CNanoListUI
{
protected:
	CAlarmHistoryListView();			// 動的生成に使用されるプロテクト コンストラクタ。
	virtual ~CAlarmHistoryListView();
	DECLARE_DYNCREATE(CAlarmHistoryListView)

// Operation
public:
	///// Variable /////
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	///// Functions /////
	virtual void List_HeaderSet();
	virtual void List_DataSet();
	void List_Disp(ALARM_HIS* pAlarmHis);
	void List_Add(ALARM_HIS* pAlarmHis);
	void SetCursorToLast();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAlarmHistoryListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAlarmHistoryListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnclickRecipeMainList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	afx_msg LRESULT OnAlarmHis_Add(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALARMHISTORYLISTVIEW_H__F8F9045E_BD7B_4F66_AB28_9A91B9E869E0__INCLUDED_)
