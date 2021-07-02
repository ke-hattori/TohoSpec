#if !defined( _MANUALDESKEWDLG_H_ )
#define _MANUALDESKEWDLG_H_

#pragma once

#include "EventPar.h"
#include "OverlayDialog.h"
//===========================================================================
// class : CManualDeskewDlg
//===========================================================================

class CManualDeskewDlg : public COverlayDialog, cActive
{
// コンストラクション
public:
	CManualDeskewDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	CManualDeskewDlg(DESKEW_SITE_MANUAL* pDeskewSiteManual, CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CManualDeskewDlg)
	//}}AFX_DATA

// Operation
public:
	DESKEW_SITE_MANUAL* m_pDeskewSiteManual;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CManualDeskewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CManualDeskewDlg)
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LRESULT OnStageMoveDone(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	// state processing functions
	//
	void enter_ST_INIT		(const cEventParams* const);
	void enter_ST_MOVE_TO_DESKEW_SITE(const cEventParams* const);
	void enter_ST_FOCUS		(const cEventParams* const);
	void enter_ST_START		(const cEventParams* const);
	void enter_ST_STORE_SITE (const cEventParams* const evp);
	void enter_ST_STAGE_MOVED(const cEventParams* const);
	DECLARE_STATEPROC_MAP(CManualDeskewDlg);

	POINT m_ptTeach;
};
#endif // !defined(_MANUALDESKEWDLG_H_)
