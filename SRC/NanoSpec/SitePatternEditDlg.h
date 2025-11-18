#if !defined(AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_)
#define AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SitePatternEditDlg.h : ヘッダー ファイル
//
#include "NanoListCtrl.h"
#include "ColorStatic.h"
#include "BitmapStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSitePatternEditDlg ダイアログ

class CSitePatternEditDlg : public CDialog
{
// コンストラクション
public:
	CSitePatternEditDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ
	~CSitePatternEditDlg();
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

// ダイアログ データ
	//{{AFX_DATA(CSitePatternEditDlg)
	enum { IDD = IDD_SITE_PATTERN_EDIT_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

	CBitmapStatic m_stcSitePrImg;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_DeleteAllButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CNanoListCtrl m_lstCtrl;
	CListCtrl& rfListCtrl() {return m_lstCtrl;}

	int		m_iMode;
	int		m_SelectItem;
	CString m_SelectName;
	SPR_SUB_INFO m_pPatSubInfo;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSitePatternEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSitePatternEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickPatternList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickPatternList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteButton();
	afx_msg void OnDeleteAllButton();
	//}}AFX_MSG
	LRESULT OnExecutionStart(WPARAM wparam, LPARAM lparam);
	LRESULT OnExecutionEnd(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()

	virtual void List_HeaderSet();
	virtual void List_DataSet();
	virtual void List_Add(const char* Name);
	virtual void SortColumn(int iCol);
	virtual void DispPattern();
	virtual void SetCursorPosition(int iItem);
	virtual void InitialDataSet();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;


private:
	void RenumberItem();

	SR_TURRET srTurret[SR_LENS_MAX];
	BOOL	m_bSort[256];

	int m_ExecuteSeq;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SITEPATTERNEDITDLG_H__F296AB64_F0CB_4C9B_8383_2E63251E2546__INCLUDED_)
