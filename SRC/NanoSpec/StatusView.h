#if !defined(AFX_STATUSVIEW_H__7E1464F6_72FD_48E4_89B4_DEFED6D33326__INCLUDED_)
#define AFX_STATUSVIEW_H__7E1464F6_72FD_48E4_89B4_DEFED6D33326__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CStatusView フォーム ビュー                                             //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

/////////////////////////////////////////////////////////////////////////////
//
class CStatusView : public CNanoUI
{
protected:
	CStatusView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CStatusView)

// フォーム データ
public:
	//{{AFX_DATA(CStatusView)
	enum { IDD = IDD_STATUS_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_AlarmResetButton;
	BOOL m_bOrder[6];
	CToolBar m_wndToolBar;
	RECT m_ListRect;

	void InitAlarmList();
	void AddAlarmList(int Index, ALARM_HIS* pAlarmHis);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStatusView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CStatusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStatusView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAlarmResetButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STATUSVIEW_H__7E1464F6_72FD_48E4_89B4_DEFED6D33326__INCLUDED_)
