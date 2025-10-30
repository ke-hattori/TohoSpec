#if !defined(AFX_DATAREVIEWVIEW_H__979C0BF5_AAC2_48CC_A507_44544595B6D5__INCLUDED_)
#define AFX_DATAREVIEWVIEW_H__979C0BF5_AAC2_48CC_A507_44544595B6D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataReviewView.h : ヘッダー ファイル
//

class CMeasurementListDlg;
class CMeasurementGraphDlg;
// ##########################################################################
// CDataReviewView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CDataReviewView : public CNanoUI
{
protected:
	CDataReviewView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CDataReviewView)

// フォーム データ
public:
	//{{AFX_DATA(CDataReviewView)
	enum { IDD = IDD_DATA_REVIEW_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CToolBar m_wndDataReviewBar;
	CTabCtrl m_DataReviewTab;
	CMeasurementListDlg* m_pMeasurementListDlg;
	CMeasurementGraphDlg* m_pMeasurementGraphDlg;

	void Tab_Init();
	void TabChange(int Index);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDataReviewView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CDataReviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDataReviewView)
	//}}AFX_MSG
	afx_msg void OnSelchangeDataReviewTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DATAREVIEWVIEW_H__979C0BF5_AAC2_48CC_A507_44544595B6D5__INCLUDED_)
