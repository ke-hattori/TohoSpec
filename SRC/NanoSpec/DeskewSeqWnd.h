// DeskewSeqWnd.h : header file
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDeskewManualSeqWnd dialog

class CDeskewManualSeqWnd : public CWnd
{
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeskewManualSeqWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDeskewManualSeqWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnDeskewManualSite1OkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeskewManualSite2OkEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeskewManualCancelEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeskewManualNoneEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
