#if !defined(AFX_USERACCOUNTLISTDLG_H__C7387FF5_80DC_4C46_BF0E_DF772BEBDD7F__INCLUDED_)
#define AFX_USERACCOUNTLISTDLG_H__C7387FF5_80DC_4C46_BF0E_DF772BEBDD7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"
#include "NanoListCtrl.h"

// UserAccountListDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CUserAccountListDlg ダイアログ
// ##########################################################################

class CUserAccountListDlg : public CDialog
{
// コンストラクション
public:
	CUserAccountListDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
	virtual ~CUserAccountListDlg();

// ダイアログ データ
	//{{AFX_DATA(CUserAccountListDlg)
	enum { IDD = IDD_USER_ACCOUNT_LIST_DLG };
	//}}AFX_DATA

//Opration
public:
	CColorStatic m_stcUserName;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_NewButton;
	CShadeButtonST m_EditButton;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_SetButton;
	CShadeButtonST m_ResetButton;
	CNanoListCtrl m_lstCtrl;

	void UserAccountList_HeaderSet();
	void UserAccountList_DataSet();
	void UserAccountList_Add(char* lpszName, int iAccountLevell, char* lpszDate);
	void EditUserAccount(BOOL bNew);
	void AutoLogon_Init();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserAccountListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserAccountListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewButton();
	afx_msg void OnEditButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnSetButton();
	afx_msg void OnResetButton();
	afx_msg void OnDblclkUserAccountList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickUserAccountList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERACCOUNTLISTDLG_H__C7387FF5_80DC_4C46_BF0E_DF772BEBDD7F__INCLUDED_)
