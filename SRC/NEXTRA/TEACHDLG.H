// TEACHDLG.H : header file
//

#if !defined(_TEACHDLG_H_)
#define _TEACHDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ===========================================================================
// CTeachLocationDlg dialog
// ===========================================================================
class CNextraDlg;
class CTeachLocationDlg : public CDialog
{
// Construction
public:
	CTeachLocationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachLocationDlg)
	enum { IDD = IDD_TEACH_LOCATION_DIALOG };
	CComboBox	 m_cbCurrentAxis;
	long		 m_lJogSpeed;
	long		 m_lJogDistance;
	CColorStatic m_stcUpperPos;
	CColorStatic m_stcAlignmentPos;
	CColorStatic m_stcLowerPos;
	CColorStatic m_stcAlignmentDecelPos;
	CColorStatic m_stcCurrentPos;
	CColorStatic m_stcArrow1;
	CColorStatic m_stcArrow2;
	CColorStatic m_stcArrow3;
	CColorStatic m_stcArrow4;
	CColorStatic m_stcArrow5;
	CColorStatic m_stcArrow6;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachLocationDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTeachLocationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboCurrentAxis();
	afx_msg void OnButtonUpperPosSet();
	afx_msg void OnButtonAlignmentPosSet();
	afx_msg void OnButtonLowerPosSet();
	afx_msg void OnButtonAlignmentDecelPosSet();
	afx_msg void OnButtonMotionParamsSave();
	afx_msg void OnButtonMotionParamsReload();
	afx_msg void OnButtonMotionUp();
	afx_msg void OnButtonMotionDown();
	afx_msg void OnButtonMotionStop();
	afx_msg void OnRadioJogDistance();
	afx_msg void OnRadioJogSpeed();
	//}}AFX_MSG
	LRESULT OnKeyHook (WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

private:
	inline void ShowInformation(int nCmdShow, UINT nID = 0L)
	{
		if ( nCmdShow == SW_SHOW) {
			if ( m_pdlgInform )  {
				m_pdlgInform->DestroyWindow();
				delete m_pdlgInform;
				m_pdlgInform = NULL;
			}
			m_pdlgInform = new CInformationDlg();
			m_pdlgInform->m_strText.LoadString(nID);
			m_pdlgInform->m_strTitle.LoadString(IDS_MOTPARAMS_TITLE);
			m_pdlgInform->Create(CInformationDlg::IDD);
			// get the current window position
			CRect ClientRect;
			m_pdlgInform->GetClientRect(ClientRect);
			POINT ptScreen;
			ptScreen.x = (GetSystemMetrics(SM_CXSCREEN) / 2) - ClientRect.right / 2;
			ptScreen.y = (GetSystemMetrics(SM_CYSCREEN) / 2) + ClientRect.bottom;
			m_pdlgInform->SetWindowPos(&CWnd::wndTopMost, ptScreen.x, ptScreen.y, 0, 0, SWP_NOSIZE);

		}
		ASSERT(m_pdlgInform != NULL);
		m_pdlgInform->ShowWindow(nCmdShow);
	}
	inline BOOL SaveMotionParams (void)
	{
		ShowInformation(SW_SHOW, IDS_MOTPARAMS_SAVING);
		// íºê⁄ì¸óÕÇ≥ÇÍÇΩéû
		basic_interface::copymemory(&old_motparams, &new_motparams);
		if ( nexaxDownloadMotionParams(m_axis, &new_motparams) == NEXAXERR_NONE ) {
			ShowInformation(SW_HIDE);
			return TRUE;
		}
		ShowInformation(SW_HIDE);
		MyMessageBox(NULL, IDS_MOTPARAMS_SAVE_FAILED, IDS_MOTPARAMS_TITLE,
			MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
		return FALSE;

	}
	inline BOOL ReloadMotionParams (void)
	{
		ShowInformation(SW_SHOW, IDS_MOTPARAMS_LOADING);
		if ( nexaxUploadMotionParams(m_axis, &old_motparams) == NEXAXERR_NONE ) {
			basic_interface::copymemory(&new_motparams, &old_motparams);
			ShowInformation(SW_HIDE);
			return TRUE;
		}
		ShowInformation(SW_HIDE);
		MyMessageBox(NULL, IDS_MOTPARAMS_LOAD_FAILED, IDS_MOTPARAMS_TITLE,
			MB_OK | MB_SYSTEMMODAL | MB_ICONSTOP);
		return FALSE;
	}

	void EnableControl (void);
	void RefreshControl (void);

	motion_params_t new_motparams;
	motion_params_t old_motparams;

	unsigned short	m_axis;
	NEXINIPARAMS	m_params;
	CInformationDlg* m_pdlgInform;
};

#endif // !defined(_TEACHDLG_H_)
