// GlassineDlg.h : ヘッダー ファイル
//

#if !defined(AFX_GLASSINEDLG_H__6F3ECD2A_1BB1_4180_8B4F_209815542A9A__INCLUDED_)
#define AFX_GLASSINEDLG_H__6F3ECD2A_1BB1_4180_8B4F_209815542A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef BOOL __stdcall SETLAYER( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags );
/////////////////////////////////////////////////////////////////////////////
// CGlassineDlg ダイアログ

class CGlassineDlg : public CDialog
{
// 構築
public:
	CGlassineDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CGlassineDlg)
	enum { IDD = IDD_GLASSINE_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CGlassineDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CGlassineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL SetLayered(HWND hWnd,int Pos);
	BOOL OnShowcmdline();
	void ResetWinPos();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_GLASSINEDLG_H__6F3ECD2A_1BB1_4180_8B4F_209815542A9A__INCLUDED_)
