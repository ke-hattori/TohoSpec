// win.h : header file
/****************************************************************
 *		Copyright (C) 2003 by Tencor Instruments
 *
 *		Source Upgrade From VC1.52 to VC6.0
 *
 *		Updated by Ouyang-jc
 *
 *		Date:	2003/03/26
 ****************************************************************/

/*
 *	マクロ定義
 */
#define	IDENT_CODE		"MotTsk2"	// 識別コード(プロパティリストで使用)

/////////////////////////////////////////////////////////////////////////////
// cMotTskWnd frame

class cMotTskWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(cMotTskWnd)
public:
	cMotTskWnd();			// protected constructor used by dynamic creation
	~cMotTskWnd();
// Attributes
private:
	BOOL m_bCloseReq;
	CString m_strMMessage[AXIS_NUM]; // string for detailing motion message
	long m_Pos[AXIS_NUM];

public:

// Operations
public:

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(cMotTskWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	// non class-wizard message handler
	afx_msg LONG MotionMessage(UINT pfsuint, LONG pfslong);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
