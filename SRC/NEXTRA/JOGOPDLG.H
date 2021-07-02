// JOGOPDLG.H : header file
//

#if !defined(_JOGOPDLG_H_)
#define _JOGOPDLG_H_
#include <afxmt.h>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ===========================================================================
// CJogOperationDlg dialog
// ===========================================================================

class CJogOperationDlg : public CDialog
{
// Construction
public:
	CJogOperationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJogOperationDlg)
	enum { IDD = IDD_JOG_OPERATION_DIALOG };
	CComboBox	 m_cbCurrentAxis;
#if 1
	CColorButton m_btnMotionUp;
	CColorButton m_btnMotionDown;
#endif
	long		 m_lJogSpeed;
	CColorStatic m_stcCurrentPos;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJogOperationDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJogOperationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnSelchangeComboCurrentAxis();
#if 0
	afx_msg void OnButtonMotionUp();
	afx_msg void OnButtonMotionDown();
	afx_msg void OnButtonMotionStop();
	//}}AFX_MSG
	LRESULT OnKeyHook (WPARAM wp, LPARAM lp);
#else
	afx_msg void OnButtonMotionUp();
	afx_msg void OnButtonMotionDown();
	afx_msg void OnButtonMotionStop();
	//}}AFX_MSG
	LRESULT OnKeyHook (WPARAM wp, LPARAM lp);
	LRESULT OnButtonEvent (WPARAM wp, LPARAM lp);
#endif
	DECLARE_MESSAGE_MAP()

private:
	BOOL Initialize (void);
	void RefreshControl (void);
	void EnableControl (void);

	inline void GetCurrPos (void)
	{
		long lPos;
		CString strText;
		if ( nexaxIsInitialized(m_axis) ) {
			nexaxGetCurrPos(m_axis, lPos);
			strText.Format("%+011ld", lPos);
		}
		else {
			strText.LoadString(IDS_UNKNOWN_CURRENT_POSITION);
		}
	m_stcCurrentPos.SetWindowText(strText);
	}
	unsigned short	 m_axis;
	NEXINIPARAMS	 m_params;
};

#endif // !defined(_JOGOPDLG_H_)
