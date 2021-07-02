// NEXTRADLG.H : header file
//

#if !defined(_NEXTRADLG_H_)
#define _NEXTRADLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "informdlg.h"

// ===========================================================================
// CNextraDlg dialog
// ===========================================================================

class CNextraDlg : public CDialog
{
// Construction
public:
	CNextraDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNextraDlg)
	enum { IDD = IDD_NEXTRA_DIALOG };
	CListCtrl m_listAlarm;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNextraDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNextraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnQuit();
	afx_msg void OnButtonTeachOperation();
	afx_msg void OnButtonManualOperation();
	afx_msg void OnButtonOnDioOperation();
	afx_msg void OnButtonSystemConfiguration();
	afx_msg void OnButtonChangePassword();
	afx_msg void OnButtonAlarmHistory();
	afx_msg void OnButtonAlarmReset();
	//}}AFX_MSG
	LRESULT OnNexInitialize(WPARAM wp, LPARAM lp);
	LRESULT OnNexReturnToOrigin(WPARAM wp, LPARAM lp);
	LRESULT OnNexLoad(WPARAM wp, LPARAM lp);
	LRESULT OnNexUnload(WPARAM wp, LPARAM lp);
	LRESULT OnNexMoveToUpper(WPARAM wp, LPARAM lp);
	LRESULT OnNexMoveToAlignment(WPARAM wp, LPARAM lp);
	LRESULT OnNexMoveToLower(WPARAM wp, LPARAM lp);
	LRESULT OnNexOpenWorkGuide(WPARAM wp, LPARAM lp);
	LRESULT OnNexCloseWorkGuide(WPARAM wp, LPARAM lp);
	LRESULT OnNexOpenShutter(WPARAM wp, LPARAM lp);
	LRESULT OnNexCloseShutter(WPARAM wp, LPARAM lp);
	LRESULT OnNexVacuumOn(WPARAM wp, LPARAM lp);
	LRESULT OnNexVacuumOff(WPARAM wp, LPARAM lp);
	LRESULT OnNexAirFloatOn(WPARAM wp, LPARAM lp);
	LRESULT OnNexAirFloatOff(WPARAM wp, LPARAM lp);
	LRESULT OnNexCancel(WPARAM wp, LPARAM lp);
	LRESULT OnNexAlarmAllReset(WPARAM wp, LPARAM lp);
	LRESULT OnNexAlarmSet(WPARAM wp, LPARAM lp);
	LRESULT OnNexAlarmReset(WPARAM wp, LPARAM lp);
	LRESULT OnNexRC_MoveToTeachPos(WPARAM wp, LPARAM lp);
	LRESULT OnNexRC_JogPlus(WPARAM wp, LPARAM lp);
	LRESULT OnNexRC_JogMinus(WPARAM wp, LPARAM lp);
	LRESULT OnNexRC_JogStop(WPARAM wp, LPARAM lp);
	LRESULT OnNexRC_SelectMode(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

private:
	inline BOOL IsHardwareSimulation (void) const { return m_params.ADAX.HardwareSimulation; }
	inline void AlarmReset (void)
	{
		if ( m_listAlarm.GetItemCount() > 0 ) {
			m_listAlarm.DeleteAllItems();
			m_alarm.Reset();
		}
	}

	enum { NEXSW_SHOW = 0, NEXSW_MINIMIZE, NEXSW_HIDE };

	BOOL Initialize (void);

	CDialog*	 m_pdlgManOp;

	NEXINIPARAMS m_params;
	cAlarm		 m_alarm;
};

#endif // !defined(_NEXTRADLG_H_)
