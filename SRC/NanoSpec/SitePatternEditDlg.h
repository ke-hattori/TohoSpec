#if !defined(AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_)
#define AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SitePatternEditDlg.h : ï¿½wï¿½bï¿½_ï¿½[ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//
#include "NanoListCtrl.h"
#include "ColorStatic.h"
#include "BitmapStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSitePatternEditDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O

class CSitePatternEditDlg : public CDialog
{
// ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½Vï¿½ï¿½ï¿½ï¿½
public:
	CSitePatternEditDlg(CWnd* pParent = NULL);	 // ï¿½Wï¿½ï¿½ï¿½ÌƒRï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^
	~CSitePatternEditDlg();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

// ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O ï¿½fï¿½[ï¿½^
	//{{AFX_DATA(CSitePatternEditDlg)
	enum { IDD = IDD_SITE_PATTERN_EDIT_DLG };
		// ï¿½ï¿½ï¿½ï¿½: ClassWizard ï¿½Í‚ï¿½ï¿½ÌˆÊ’uï¿½Éƒfï¿½[ï¿½^ ï¿½ï¿½ï¿½ï¿½ï¿½oï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//}}AFX_DATA

	CBitmapStatic m_stcSitePrImg;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_DeleteAllButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CNanoListCtrl m_lstCtrl;
	CListCtrl& rfListCtrl() {return m_lstCtrl;}

	int		m_iMode;
	int		m_SelectItem;
	CString m_SelectName;
	SPR_SUB_INFO m_pPatSubInfo;

// ï¿½Iï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½h
	// ClassWizard ï¿½Í‰ï¿½ï¿½zï¿½Ö�ï¿½ï¿½ÌƒIï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½hï¿½ğ�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//{{AFX_VIRTUAL(CSitePatternEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ï¿½Tï¿½|ï¿½[ï¿½g
	//}}AFX_VIRTUAL

// ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
protected:

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½}ï¿½bï¿½vï¿½Ö�ï¿½
	//{{AFX_MSG(CSitePatternEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickPatternList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickPatternList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteButton();
	afx_msg void OnDeleteAllButton();
	//}}AFX_MSG
	LRESULT OnExecutionStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnExecutionEnd(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual void List_Add(const char* Name);
	virtual void SortColumn(int iCol);
	virtual void DispPattern();
	virtual void SetCursorPosition(int iItem);
	virtual void InitialDataSet();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;


private:
	void RenumberItem();

	SR_TURRET srTurret[SR_LENS_MAX];
	BOOL	m_bSort[256];

	int m_ExecuteSeq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ ï¿½Í‘Oï¿½sï¿½Ì’ï¿½ï¿½Oï¿½É’Ç‰ï¿½ï¿½Ì�éŒ¾ï¿½ï¿½}ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B

#endif // !defined(AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_)
