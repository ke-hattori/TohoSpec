#ifndef _BITMAPSTATIC_H_
#define _BITMAPSTATIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ##########################################################################
// class : CBitmapStatic
// ##########################################################################
class CBitmapStatic : public CStatic
{
// コンストラクション
public:
	CBitmapStatic();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockST)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CBitmapStatic();

	BOOL SetBitmap(PCSTR pszFilePath, CScrollBar* pScrollBar = NULL);
	inline void DeleteObject(BOOL bErase = TRUE) {
		if ( IsValid() ) {
			m_bmImage.DeleteObject();
		}
		ShowScrollBar(SB_BOTH, FALSE);
	}
	inline BOOL IsValid(void) { return m_bmImage.GetSafeHandle() != NULL; }

protected:
	//{{AFX_MSG(CClockST)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void DrawBitmap(CDC* pDC);
	CRect	m_rcClient;
	CBitmap m_bmImage;
	CSize	m_sizeBitmap;
	CPoint	m_ptImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BITMAPSTATIC_H__22341131_66CF_4286_B4F3_EBACCFE9C008__INCLUDED_)
