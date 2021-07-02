#if !defined(_ALHISDLG_H_)
#define _ALHISDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlHisDlg.h : header file
//

// ----------------------------------------------------------------------------
// CAlarmHistoryDlg dialog

class CAlarmHistoryDlg : public CDialog
{
// Construction
public:
	CAlarmHistoryDlg( CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlarmHistoryDlg)
	enum { IDD = IDD_ALARM_HISTORY_DIALOG };
	CListCtrl m_listAlarm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarmHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlarmHistoryDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void InitializeList (void);
	void UpdateList (void);
};

#endif // !defined(_ALHISDLG_H_)
