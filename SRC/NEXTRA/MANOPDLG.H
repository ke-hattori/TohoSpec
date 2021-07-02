// MANOPDLG.H : header file
//

#if !defined(_MANOPDLG_H_)
#define _MANOPDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ===========================================================================
// CManualOperationDlg dialog
// ===========================================================================

class CManualOperationDlg : public CDialog
{
// Construction
public:
	CManualOperationDlg(CWnd* pParent = NULL);	 // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualOperationDlg)
	enum { IDD = IDD_MANUAL_OPERATION_DIALOG };
	CComboBox	 m_cbRcMode;
	CColorButton m_btnOpenShutter;
	CColorButton m_btnCloseShutter;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualOperationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CManualOperationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnButtonMoveToUpperPos();
	afx_msg void OnButtonMoveToAlignmentPos();
	afx_msg void OnButtonMoveToLowerPos();
	afx_msg void OnButtonStartLoad();
	afx_msg void OnButtonStartUnload();
	afx_msg void OnButtonVacuumOn();
	afx_msg void OnButtonVacuumOff();
	afx_msg void OnButtonOpenWorkGuide();
	afx_msg void OnButtonCloseWorkGuide();
	afx_msg void OnButtonAirFloatOn();
	afx_msg void OnButtonAirFloatOff();
	afx_msg void OnButtonOpenShutter();
	afx_msg void OnButtonCloseShutter();
	afx_msg void OnSelchangeComboRcMode();
	//}}AFX_MSG
	LRESULT OnNexResponse (WPARAM wp, LPARAM lp);
	LRESULT OnButtonEvent (WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

	static unsigned int __stdcall start_address (void* arglist) { return reinterpret_cast<CManualOperationDlg*>(arglist)->start_address(); }
	virtual unsigned int start_address (void);

	class cLigtCtrl
	{
	public:
		cLigtCtrl(NEXIOOUTPOCTYPEPTR _f)
			: f(_f), m_flicker(FALSE) {}
	public:
		void Update (void)
		{
			if ( m_flicker && m_timer.is_timeout() ) {
				f(m_flag);
				m_flag = !m_flag;
				m_timer.reset();
			}
		}
		void TurnOff (void)
		{
			m_flicker = FALSE;
			f(m_flag = FALSE);
		}
		void Light (void)
		{
			m_flicker = FALSE;
			f(m_flag = TRUE);
		}
		void FlickerLow (void)
		{
			m_flicker = TRUE;
			m_timer.set(500);
		}
		void FlickerHigh (void)
		{
			m_flicker = TRUE;
			m_timer.set(1000);
		}
	private:
		NEXIOOUTPOCTYPEPTR f;
		BOOL		 m_flicker;
		BOOL		 m_flag;
		basic_timer m_timer;
	};

	cLigtCtrl m_lightMode;

private:
	void RefreshControl (void);
	void EnableControl (void);

	basic_thread m_thread;
	BOOL		 m_kill;

	NEXINIPARAMS m_params;
};

#endif // !defined(_MANOPDLG_H_)
