#if !defined(AFX_SAMPLELISTDLG_H__CB601C4E_2B9F_426B_823A_F474ED65B8D2__INCLUDED_)
#define AFX_SAMPLELISTDLG_H__CB601C4E_2B9F_426B_823A_F474ED65B8D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NanoListCtrl.h"

// SampleListDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSampleListDlg ダイアログ
// ##########################################################################

class CSampleListDlg : public CDialog
{
// コンストラクション
public:
	CSampleListDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	virtual ~CSampleListDlg();

	static int CALLBACK CSampleListDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParam3);

protected:
// ダイアログ データ
	//{{AFX_DATA(CSampleListDlg)
	enum { IDD = IDD_SAMPLE_LIST_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_NewButton;
	CShadeButtonST m_EditButton;
	CShadeButtonST m_DeleteButton;
	CNanoListCtrl m_lstCtrl;

	void SampleList_HeaderSet();
	void SampleList_DataSet();
	// 2009.08.26 bagus stress --{--
	//void SampleList_Add(char* lpszName, int iShape, double dSizeX, double dSizeY, double dThickness, char* lpszDate);
	void SampleList_Add(char* lpszName, int iShape, double dSizeX, double dSizeY, double dThickness, double dElasticModulusValue, char* lpszDate);
	// 2009.08.26 bagus stress --}--
	void EditSample(BOOL bNew);
	void Sort(int col);
	void RenumberItem();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSampleListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSampleListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewButton();
	afx_msg void OnEditButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnDblclkSampleList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickSampleList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickSampleList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_iCol;
	BOOL m_bOrder[6];
	// 2009.08.26 bagus stress --{--
	SYSTEM_CONFIG m_SystemConfig;
	// 2009.08.26 bagus stress --}--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SAMPLELISTDLG_H__CB601C4E_2B9F_426B_823A_F474ED65B8D2__INCLUDED_)
