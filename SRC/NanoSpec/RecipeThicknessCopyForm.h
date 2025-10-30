#if !defined(AFX_RECIPETHICKNESSCOPYFORM_H__F12DE0A3_33B6_4A7D_8D47_7F0482D719F7__INCLUDED_)
#define AFX_RECIPETHICKNESSCOPYFORM_H__F12DE0A3_33B6_4A7D_8D47_7F0482D719F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeThicknessCopyForm.h : header file
//
#include "ColorStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CRecipeThicknessCopyForm dialog

class CRecipeThicknessCopyForm : public CDialog
{
// Construction
public:
	CRecipeThicknessCopyForm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecipeThicknessCopyForm)
	enum { IDD = IDD_RECIPE_THICK_COPY_FORM };
	double	m_dEditThicknessValue;
	//}}AFX_DATA

protected:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecipeThicknessCopyForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecipeThicknessCopyForm)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECIPETHICKNESSCOPYFORM_H__F12DE0A3_33B6_4A7D_8D47_7F0482D719F7__INCLUDED_)
