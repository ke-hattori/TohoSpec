// RecipeThicknessCopyForm.cpp : implementation file
//

#include "stdafx.h"
#include "system.h"
#include "nanospec.h"
#include "RecipeThicknessCopyForm.h"
#include "RecipeThicknessSettingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecipeThicknessCopyForm dialog


CRecipeThicknessCopyForm::CRecipeThicknessCopyForm(CWnd* pParent /*=NULL*/)
	: CDialog(CRecipeThicknessCopyForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecipeThicknessCopyForm)
	m_dEditThicknessValue = 0.00;
	//}}AFX_DATA_INIT
}


void CRecipeThicknessCopyForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecipeThicknessCopyForm)
	DDX_Text(pDX, IDC_EDIT_THICKNESS_VALUE, m_dEditThicknessValue);
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDOK, m_OkButton);
    DDX_Control(pDX, IDCANCEL, m_CancelButton);
}


BEGIN_MESSAGE_MAP(CRecipeThicknessCopyForm, CDialog)
	//{{AFX_MSG_MAP(CRecipeThicknessCopyForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecipeThicknessCopyForm message handlers

BOOL CRecipeThicknessCopyForm::OnInitDialog()
{
	CDialog::OnInitDialog();

    ///// OK Button /////
    m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
    m_OkButton.DrawFlatFocus(TRUE);

    ///// Cancel Button /////
    m_CancelButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
    m_CancelButton.DrawFlatFocus(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecipeThicknessCopyForm::OnOK()
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CRecipeThicknessCopyForm::OnCancel()
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}
