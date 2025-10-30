#if !defined(AFX_STRESSELASTICMODULUSLISTDLG_H__56B332C4_A0B1_4605_8E95_BACC2E74B714__INCLUDED_)
#define AFX_STRESSELASTICMODULUSLISTDLG_H__56B332C4_A0B1_4605_8E95_BACC2E74B714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StressElasticModulusListDlg.h : ヘッダー ファイル
//

#include "NanoListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CStressElasticModulusListDlg ダイアログ

class CStressElasticModulusListDlg : public CDialog
{
// コンストラクション
public:
	CStressElasticModulusListDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CShadeButtonST m_OkButton;
	CShadeButtonST m_NewButton;
	CShadeButtonST m_EditButton;
	CShadeButtonST m_DeleteButton;

	CNanoListCtrl m_ElasticModulusListCtrl;

	STRESS_ELASTIC_TABLE	m_ElasticTable;

// ダイアログ データ
	//{{AFX_DATA(CStressElasticModulusListDlg)
	enum { IDD = IDD_STRESS_SAMPLE_ELASTICMODULUS_LIST_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA



	BOOL CheckData();
	void UpDate(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CStressElasticModulusListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void ElasticModulusList_Init();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CStressElasticModulusListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNewButton();
	afx_msg void OnEditButton();
	afx_msg void OnDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CheckSameName(TCHAR *tszName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STRESSELASTICMODULUSLISTDLG_H__56B332C4_A0B1_4605_8E95_BACC2E74B714__INCLUDED_)
