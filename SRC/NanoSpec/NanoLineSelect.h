#if !defined(AFX_NANOLINESELECT_H__5D5F54C7_D960_461F_82DB_650C1B9818F3__INCLUDED_)
#define AFX_NANOLINESELECT_H__5D5F54C7_D960_461F_82DB_650C1B9818F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NanoLineSelect.h : ヘッダー ファイル
//
#include <afxwin.h> 		// MFC のコアおよび標準コンポーネント
#include <afxext.h> 		// MFC の拡張部分
#include <afxdisp.h>		// MFC のオートメーション クラス

//CheckBox用 定数
#define CHECK_BOX_WIDTH			(12)
#define GUIDE_LINE_LENGTH		(24)
#define CHECK_BOX_OFFSET		(2)

/////////////////////////////////////////////////////////////////////////////
// CNanoLineSelect ウィンドウ

class CNanoLineSelect : public CButton
{
// コンストラクション
public:
	CNanoLineSelect();

// アトリビュート
public:
	void SetDisplaySetting(COLORREF BackColor,COLORREF LineColor,int PenStyle);

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoLineSelect)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	SIZE GetIdealDispSize();
	void SetCheck(int nChecked);
	int GetCheck();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual ~CNanoLineSelect();


	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CNanoLineSelect)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void ClearArea(CDC *pDC,RECT *pScreenSize);
	void DrawCheckBoxState(CDC *pDC,RECT *pScreenSize);
	void DrawPenGuidence(CDC *pDC,RECT *pScreenSize);
	void DrawLineText(CDC *pDC,RECT *pScreenSize);

	SIZE GetTextSize(CDC *pDC, CFont *pFont, CString strText);

	COLORREF	m_LineColor;
	COLORREF	m_BackColor;
	int			m_PenStyle;
	int			m_Checked;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOLINESELECT_H__5D5F54C7_D960_461F_82DB_650C1B9818F3__INCLUDED_)
