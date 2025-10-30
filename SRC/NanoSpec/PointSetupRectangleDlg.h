#if !defined(AFX_POINTSETUPRECTANGLEDLG_H__16CBA8B9_4E0E_4B11_B178_F962FBFBE5F7__INCLUDED_)
#define AFX_POINTSETUPRECTANGLEDLG_H__16CBA8B9_4E0E_4B11_B178_F962FBFBE5F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupRectangleDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CPointSetupRectangleDlg ダイアログ
// ##########################################################################

class CPointSetupRectangleDlg : public CDialog
{
// コンストラクション
public:
	CPointSetupRectangleDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPointSetupRectangleDlg)
	enum { IDD = IDD_POINT_SETUP_RECTANGLE_DLG };
	//}}AFX_DATA

// Operation
public:
	CGridCtrl m_PointListGrid;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	void GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle);
	void GridEditable(CGridCtrl* pGrid, BOOL Editable);
	BOOL PointListGrid_Init(CGridCtrl* pGrid);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupRectangleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupRectangleDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPRECTANGLEDLG_H__16CBA8B9_4E0E_4B11_B178_F962FBFBE5F7__INCLUDED_)
