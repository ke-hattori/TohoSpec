#if !defined(AFX_MANUALMEASUREMENTPROCESSVIEW_H__190B03C7_C0D5_4415_85C7_A851CDAF37EA__INCLUDED_)
#define AFX_MANUALMEASUREMENTPROCESSVIEW_H__190B03C7_C0D5_4415_85C7_A851CDAF37EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualMeasurementProcessView.h : ヘッダー ファイル
//

// ##########################################################################
// CManualMeasurementProcessView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CManualMeasurementProcessView : public CNanoUI
{
protected:
	CManualMeasurementProcessView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CManualMeasurementProcessView)

// フォーム データ
public:
	//{{AFX_DATA(CManualMeasurementProcessView)
	enum { IDD = IDD_MANUAL_MEASUREMENT_CANCEL_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_MeasurementCancelButton;
	CShadeButtonST m_MeasurementPauseButton;
	CShadeButtonST m_RemeasurementButton;
	CShadeButtonST m_ValuesToDisplayButton;
	CShadeButtonST m_ExitButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CManualMeasurementProcessView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CManualMeasurementProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CManualMeasurementProcessView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasurementCancelButton();
	afx_msg void OnMeasurementPauseButton();
	afx_msg void OnExitButton();
	afx_msg void OnRemeasurementButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MANUALMEASUREMENTPROCESSVIEW_H__190B03C7_C0D5_4415_85C7_A851CDAF37EA__INCLUDED_)
