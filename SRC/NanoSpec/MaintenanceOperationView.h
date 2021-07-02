#if !defined(AFX_MAINTENANCEOPERATIONVIEW_H__8E4BA0E2_71F5_4342_9494_86227ADA30D1__INCLUDED_)
#define AFX_MAINTENANCEOPERATIONVIEW_H__8E4BA0E2_71F5_4342_9494_86227ADA30D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaintenanceOperationView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMaintenanceOperationView フォーム ビュー                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

/////////////////////////////////////////////////////////////////////////////
//
class CMaintenanceOperationView : public CNanoUI
{
protected:
	CMaintenanceOperationView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMaintenanceOperationView)

// フォーム データ
public:
	//{{AFX_DATA(CMaintenanceOperationView)
	enum { IDD = IDD_MAINTENANCE_OPERATION_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CShadeButtonST m_MotionButton;
	CShadeButtonST m_DioMonitorButton;
	CShadeButtonST m_TransitionMonitorButton;
	CShadeButtonST m_MaintenanceDateButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMaintenanceOperationView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CMaintenanceOperationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMaintenanceOperationView)
	afx_msg void OnMotionButton();
	afx_msg void OnDioMonitorButton();
	afx_msg void OnTransitionMonitorButton();
	afx_msg void OnMaintenanceDateButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL CheckUserAccessPrivilege();

// 2014.01.17 Bagus Add (Stage None対応) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage None対応) <--
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINTENANCEOPERATIONVIEW_H__8E4BA0E2_71F5_4342_9494_86227ADA30D1__INCLUDED_)
