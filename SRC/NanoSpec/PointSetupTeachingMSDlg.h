#if !defined(AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_)
#define AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupTeachingMSDlg.h : ï¿½wï¿½bï¿½_ï¿½[ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupTeachingMSDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O

class CPointSetupTeachingMSDlg : public COverlayDialog
{
// ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½
public:
	CPointSetupTeachingMSDlg(CWnd* pParent = NULL);   // ï¿½Wï¿½ï¿½ï¿½ÌƒRï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^

	int	m_iTeachingMode;			// ï¿½ï¿½ï¿½ï¿½/ï¿½è“®ï¿½ï¿½ï¿½[ï¿½h

public:
	typedef enum _TEACHING_MODE
	{
		MODE_AUTO_MEAS = 0,
		MODE_MANUAL_MEAS,
		MODE_MAX,
	} TEACHING_MODE;


// ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O ï¿½fï¿½[ï¿½^
	//{{AFX_DATA(CPointSetupTeachingMSDlg)
	enum { IDD = IDD_POINT_SETUP_TEACHING_MS_DLG };
		// ï¿½ï¿½ï¿½ï¿½: ClassWizard ï¿½Í‚ï¿½ï¿½ÌˆÊ’uï¿½Éƒfï¿½[ï¿½^ ï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//}}AFX_DATA

public:
	CShadeButtonST m_NextPointButton;

	int m_iCurrPoint;							// ï¿½ï¿½ï¿½Ý�Ý’è‚µï¿½æ‚¤ï¿½Æ‚ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½|ï¿½Cï¿½ï¿½ï¿½g
	BOOL m_bMoveNext;							// MoveNextï¿½ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½O

	long m_lX;
	long m_lY;
	long m_lZ;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;		// STAGE_PROG_INFO_HDRï¿½\ï¿½ï¿½ï¿½ï¿½

	int m_iProcessStatusOld;

// ï¿½Iï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½h
	// ClassWizard ï¿½Í‰ï¿½ï¿½zï¿½Ö�ï¿½ï¿½ÌƒIï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½hï¿½ð�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//{{AFX_VIRTUAL(CPointSetupTeachingMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ï¿½Tï¿½|ï¿½[ï¿½g
	//}}AFX_VIRTUAL

// ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
protected:

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½}ï¿½bï¿½vï¿½Ö�ï¿½
	//{{AFX_MSG(CPointSetupTeachingMSDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHomePositionButton();
	afx_msg void OnSelendokMicroScopeFocusPosition();
	afx_msg void OnNextPointButton();
	//}}AFX_MSG
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	afx_msg LRESULT OnNexResponse(WPARAM wparam, LPARAM lparam);
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
	DECLARE_MESSAGE_MAP()

	virtual void ButtonEnableCheck(void);
private:
	void InitCombo_Lens();
	void SoftJoyStickXyzEnable(BOOL bEnable);

	MS_CONFIG m_MsConfig;

// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --{--
	int		m_nSelLens;
	BOOL	m_bMSModeLampFlicker;
// 2009.11.09 bagus MS ï¿½Cï¿½ï¿½ --}--
// matsuhisa 2009.12.26 added >>>
	int		m_nLensSensorItem;
// matsuhisa 2009.12.26 added <<<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ ï¿½Í‘Oï¿½sï¿½Ì’ï¿½ï¿½Oï¿½É’Ç‰ï¿½ï¿½Ì�éŒ¾ï¿½ï¿½}ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B

#endif // !defined(AFX_POINTSETUPTEACHINGMSDLG_H__994E543D_5B7A_42F6_A5B7_02C833F14416__INCLUDED_)
