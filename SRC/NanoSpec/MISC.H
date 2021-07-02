#if !defined( _MISC_H_ )
#define _MISC_H_

#include <process.h>
#include <common.hxx>
#include "ShadeButtonST.h"

#define WM_DO_STAGE_MOVE		(WM_USER + 0x0010)
#define WM_STAGE_MOVE_DONE		(WM_USER + 0x0011)
#define WM_DO_ELEVATOR_MOVE 	(WM_USER + 0x0020)

// #########################################################################
// FUNCTION
// #########################################################################


// #########################################################################
// MACRO
// #########################################################################
#define NanoAddComboBoxString(ID, String, Count, Select) \
{														 \
	PCSTR pszString[] = String; 						 \
	CComboBox* pComboBox = (CComboBox*) GetDlgItem(ID);  \
	if ( pComboBox != NULL ) {							 \
		pComboBox->ResetContent();						 \
		for ( int index = 0; index < Count; index++ ) {  \
			pComboBox->AddString(pszString[index]); 	 \
		}												 \
		pComboBox->SetCurSel(Select);					 \
	}													 \
}
class CCursor
{
public:

	static void BeginWaitCursor(BOOL bShowWaitCursor = TRUE) {
		m_bWaitCursor = TRUE;
		m_bShowWaitCursor = bShowWaitCursor;

		if ( m_bShowWaitCursor ) {
			AfxGetApp()->m_pMainWnd->SetCapture();
			AfxGetApp()->BeginWaitCursor();
		}
	}
	static void EndWaitCursor(void) {
		m_bWaitCursor = FALSE;

		if ( m_bShowWaitCursor ) {
			AfxGetApp()->EndWaitCursor();
			ReleaseCapture();
		}
	}
private:

public:
	static BOOL m_bWaitCursor;
	static BOOL m_bShowWaitCursor;
};

class CMotionButton : public CShadeButtonST
{
public:
	CMotionButton()
		: m_nKey(0L), m_nMsg(0L) {};
	CMotionButton(UINT nKey, STAGE_MOTION_MODE MotionMode)
		: m_nKey(nKey), m_nMsg(0L) {};

	inline void SetParams(UINT nKey, UINT nMsg) { m_nKey = nKey; m_nMsg = nMsg; }
	inline BOOL PostMotionMessage(void)
	{
		if ( m_nMsg != 0L ) {
			return GetParent()->PostMessage(m_nMsg, m_nKey, 0L);
		}
		return FALSE;
	}

	inline void SetEnabled(BOOL flag) { m_nMode = flag; };
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- { ---------- */
	inline BOOL GetEnabled(void) {return m_nMode;};
/* added 2009.12.09 hmenjo 測定 Seq は指定ヘッドで動作(追加改造) ---------- } ---------- */

	//{{AFX_VIRTUAL(CMotionButton)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CMotionButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT m_nKey;
	UINT m_nMsg;
	BOOL m_nMode;
};

#endif // _MISC_H_
