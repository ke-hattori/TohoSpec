#if !defined(AFX_SYSTEMTRAY_H__C347ED7F_4C88_4D38_B215_AAE785D95130__INCLUDED_)
#define AFX_SYSTEMTRAY_H__C347ED7F_4C88_4D38_B215_AAE785D95130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemTray.h : header file
//

//---------------------------------------------------------------------------
// CSystemTray window

class CSystemTray : public CWnd
{
// Construction
public:
	CSystemTray();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTray)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	BOOL SetNotificationWnd(CWnd* pNotifyWnd);
	BOOL SetIcon(UINT nIDResource);
	inline BOOL Create(UINT uCallbackMessage, UINT nIDTitle, HICON hIcon, UINT uID)
	{
		CString strTitle;
		strTitle.LoadString(nIDTitle);
	}
	BOOL Create(UINT uCallbackMessage, LPCTSTR lpszTip, HICON hIcon, UINT uID);
	virtual ~CSystemTray();

	// Generated message map functions
protected:
	NOTIFYICONDATA m_NotifyIconData;
	//{{AFX_MSG(CSystemTray)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	NEXINIPARAMS	m_params;
};

//---------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRAY_H__C347ED7F_4C88_4D38_B215_AAE785D95130__INCLUDED_)
