#if !defined(AFX_TESTMEASUREMENTOPERATIONVIEW_H__454291BE_54DB_4317_B8CD_A39615CDF249__INCLUDED_)
#define AFX_TESTMEASUREMENTOPERATIONVIEW_H__454291BE_54DB_4317_B8CD_A39615CDF249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestMeasurementOperationView.h : ヘッダー ファイル
//

// ##########################################################################
// CTestMeasurementOperationView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CTestMeasurementOperationView : public CNanoUI
{
protected:
	CTestMeasurementOperationView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CTestMeasurementOperationView)

// フォーム データ
public:
	//{{AFX_DATA(CTestMeasurementOperationView)
	enum { IDD = IDD_TEST_MEASUREMENT_OPERATION_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_MeasurementButton;
	CShadeButtonST m_RemeasurementButton;
	CShadeButtonST m_GetReferenceButton;
	CShadeButtonST m_HandlerLoadPositionButton;
	CShadeButtonST m_ManualLoadPositionButton;
	CShadeButtonST m_VacuumButton;
	CShadeButtonST m_GlassLifterButton;
	CShadeButtonST m_DataSaveButton;
	CShadeButtonST m_DataLoadButton;
	CShadeButtonST m_ExitButton;

	///// Functions /////


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTestMeasurementOperationView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CTestMeasurementOperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTestMeasurementOperationView)
	afx_msg void OnExitButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TESTMEASUREMENTOPERATIONVIEW_H__454291BE_54DB_4317_B8CD_A39615CDF249__INCLUDED_)
