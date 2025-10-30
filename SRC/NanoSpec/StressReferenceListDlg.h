#if !defined(AFX_STRESSREFERENCELISTDLG_H__78D6A810_14DB_4FCB_B284_002700699563__INCLUDED_)
#define AFX_STRESSREFERENCELISTDLG_H__78D6A810_14DB_4FCB_B284_002700699563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StressReferenceListDlg.h : ヘッダー ファイル
//

#include "NanoListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CStressReferenceListDlg ダイアログ

class CStressReferenceListDlg : public CDialog
{
// コンストラクション
public:
	CStressReferenceListDlg(CWnd* pParent = NULL);	 // 標準のコンストラクタ

	CShadeButtonST m_CancelButton;
	CShadeButtonST m_DeleteReferenceButton;

	CNanoListCtrl m_StressReferenceListCtrl;

// 2009.10.22 bagus Stress 修正 --{--
	CShadeButtonST m_OkButton;
	BOOL	m_bListMode;
	CString	m_strSampleID;
// 2009.10.22 bagus Stress 修正 --}--

// ダイアログ データ
	//{{AFX_DATA(CStressReferenceListDlg)
	enum { IDD = IDD_STRESS_REFERENCE_LIST_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStressReferenceListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStressReferenceListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeleteReferenceButton();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkReferenceList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	SYSTEM_CONFIG m_SystemConfig;
	BOOL DeleteStressReference();
	void SetStressReferenceListData();
	void StressReferenceList_Init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRESSREFERENCELISTDLG_H__78D6A810_14DB_4FCB_B284_002700699563__INCLUDED_)
