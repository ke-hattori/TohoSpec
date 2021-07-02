#include "stdafx.h"
#include "Misc.h"

BOOL CCursor::m_bShowWaitCursor = FALSE;
BOOL CCursor::m_bWaitCursor = FALSE;

BEGIN_MESSAGE_MAP(CMotionButton, CShadeButtonST)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CMotionButton::PreSubclassWindow()
{
	SetEnabled(TRUE);

	CShadeButtonST::PreSubclassWindow();
}

void CMotionButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_nMode ){
//	if ( !CCursor::m_bWaitCursor ) {
//		CCursor::BeginWaitCursor();
		PostMotionMessage();
//	}

	CShadeButtonST::OnLButtonDown(nFlags, point);
	}
}

void CMotionButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_nMode ){
//	CCursor::EndWaitCursor();
//	StageStop();

	CShadeButtonST::OnLButtonUp(nFlags, point);
	}
}

void CMotionButton::OnKillFocus(CWnd* pNewWnd)
{
	if( m_nMode ){
	CShadeButtonST::OnKillFocus(pNewWnd);

//	if ( CCursor::m_bWaitCursor ) {
//		StageStop();
//	}
	}
}

// #########################################################################
// FUNCTION
// #########################################################################
