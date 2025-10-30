// NanoSpecTitleDlg.h : ヘッダー ファイル
//

#if !defined(AFX_NANOSPECTITLEDLG_H__78A5ECB9_F2D3_478E_B310_7A4954DEE765__INCLUDED_)
#define AFX_NANOSPECTITLEDLG_H__78A5ECB9_F2D3_478E_B310_7A4954DEE765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Define                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Color Definition
#define WHITE_COLOR						RGB(255,255,255)
#define RED_COLOR						RGB(255,  0,  0)
#define GREEN_COLOR						RGB(  0,255,  0)
#define BLUE_COLOR						RGB(  0,  0,255)
#define YELLOW_COLOR					RGB(255,255,  0)
#define MAGENTA_COLOR					RGB(255,  0,255)
#define CYAN_COLOR						RGB(  0,255,255)
#define EXIST_COLOR						RGB(  0,128,255)
#define BUTTON_COLOR					GetSysColor(COLOR_BTNFACE)
#define HOST_MESSAGE_COLOR				RGB( 19,253,131)
#define GRAY_COLOR						RGB(128, 128, 128)
#define LIGHT_GRAY_COLOR				RGB(210,210,210)
#define DARK_GRAY_COLOR					RGB(100,100,100)
#define SIGNAL_ON_COLOR					RGB(155,155,255)
#define REFERENCE_COLOR					RGB(104,235,141)
#define GLASS_EXIST_COLOR				RGB(  0,221,  0)
#define WATER_COLOR						RGB(210,255,255)//(151,255,255)//RGB(  0,255,255)
#define GRID_CELL_COLOR					RGB(0xFF, 0xFF, 0xE0)
#define LIGHT_BLUE_COLOR				RGB(  2,171,253)
#define BLACK_COLOR						RGB(  0,  0,  0)
#define WAFER_COLOR						RGB(210,255,255)
#define DARK_GREEN_COLOR				RGB(  0, 0xCC,0)
#define PURPLE_COLOR					RGB(153,  0,153)

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleDlg ダイアログ

class CNanoSpecTitleDlg : public CDialog
{
// 構築
public:
	CNanoSpecTitleDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CNanoSpecTitleDlg)
	enum { IDD = IDD_NANOSPECTITLE_DIALOG };
	CProgressCtrl	m_wndProgress;
	CStatic m_stcMessage2;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoSpecTitleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// Operation
public:
	///// Variable //////////////////////////////////////////////////////////
	CFont m_Font;
	CBrush m_Brush_Message;
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);

	///// Function //////////////////////////////////////////////////////////
	void FontCreation();

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNanoSpecTitleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnSetProgressPos(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetMessage2(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOSPECTITLEDLG_H__78A5ECB9_F2D3_478E_B310_7A4954DEE765__INCLUDED_)
