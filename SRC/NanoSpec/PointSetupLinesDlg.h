#if !defined(AFX_POINTSETUPLINESDLG_H__8E37063C_F425_4CB1_B7AF_EBC50B73CB1D__INCLUDED_)
#define AFX_POINTSETUPLINESDLG_H__8E37063C_F425_4CB1_B7AF_EBC50B73CB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupLinesDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CPointSetupLinesDlg ダイアログ
// ##########################################################################

class CPointSetupLinesDlg : public CDialog
{
// コンストラクション
public:
	CPointSetupLinesDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPointSetupLinesDlg)
	enum { IDD = IDD_POINT_SETUP_LINES_DLG };
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
	//{{AFX_VIRTUAL(CPointSetupLinesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupLinesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPLINESDLG_H__8E37063C_F425_4CB1_B7AF_EBC50B73CB1D__INCLUDED_)
