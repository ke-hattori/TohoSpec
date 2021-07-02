// SitePtSeqWnd.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSitePtManualSeqWnd dialog

class CSitePtManualSeqWnd : public CWnd
{
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSitePtManualSeqWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSitePtManualSeqWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnSitePtManualOkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSitePtManualCancelEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSitePtManualNoneEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
