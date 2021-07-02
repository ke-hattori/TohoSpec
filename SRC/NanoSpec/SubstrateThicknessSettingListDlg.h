#pragma once

// SubstrateThicknessSettingListDlg.h : ヘッダー ファイル
//

#include "NanoListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessSettingListDlg ダイアログ

class CSubstrateThicknessSettingListDlg : public CDialog
{
// コンストラクション
public:
	CSubstrateThicknessSettingListDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	///// Variable /////
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_NewButton;
	CShadeButtonST m_EditButton;
	CShadeButtonST m_DeleteButton;
	CShadeButtonST m_ReferenceEditButton;
	CShadeButtonST m_ReferenceDeleteButton;
	CNanoListCtrl m_SubstrateThicknessListCtrl;
	CNanoListCtrl m_ReferenceThicknessListCtrl;

	SUBSTRATE_THICKNESS_TABLE m_SubstrateThicknessTable;

// ダイアログ データ
	//{{AFX_DATA(CSubstrateThicknessSettingListDlg)
	enum { IDD = IDD_SUBSTRATE_THICKNESS_SETTING_LIST_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSubstrateThicknessSettingListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void SubstrateThicknessList_Init();
	void ReferenceThicknessList_Init();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSubstrateThicknessSettingListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnNewButton();
	afx_msg void OnEditButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnReferenceEditButton();
	afx_msg void OnReferenceDeleteButton();
	afx_msg void OnDblclkSubstrateThicknessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkReferenceThicknessList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateListCtrl(CNanoListCtrl* pListCtrl, int iConfigDataIndex);
	void UpDate(BOOL bValid);
	double MilliToMicro(double dMilli);
	double MicroToMilli(double dMicro);
	void EditSubstrateThickness();
	void EditReferenceThickness();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
};
