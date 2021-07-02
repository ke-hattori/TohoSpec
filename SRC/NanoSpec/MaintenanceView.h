#if !defined(AFX_MAINTENANCEVIEW_H__ED80408E_8F2F_4DFF_9C8C_99ABFD7A61E7__INCLUDED_)
#define AFX_MAINTENANCEVIEW_H__ED80408E_8F2F_4DFF_9C8C_99ABFD7A61E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenanceView.h : ヘッダー ファイル
//

// ##########################################################################
// CMaintenanceView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CMaintenanceView : public CNanoUI
{
protected:
	CMaintenanceView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMaintenanceView)

// フォーム データ
public:
	//{{AFX_DATA(CMaintenanceView)
	enum { IDD = IDD_MAINTENANCE_FORM };
	//}}AFX_DATA

// アトリビュート
public:
	CBrush m_Brush_BackColor;

// オペレーション
public:
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMaintenanceView)
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
	public:
	virtual void OnInitialUpdate();
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CMaintenanceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMaintenanceView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINTENANCEVIEW_H__ED80408E_8F2F_4DFF_9C8C_99ABFD7A61E7__INCLUDED_)
