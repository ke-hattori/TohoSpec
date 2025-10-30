#if !defined(AFX_DELETEDATAPERIODSETTINGDLG_H__39D5E95A_7582_47EF_98EE_35FA60AEDBA3__INCLUDED_)
#define AFX_DELETEDATAPERIODSETTINGDLG_H__39D5E95A_7582_47EF_98EE_35FA60AEDBA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeleteDataPeriodSettingDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"

// ##########################################################################
// CDeleteDataPeriodSettingDlg ダイアログ
// ##########################################################################

class CDeleteDataPeriodSettingDlg : public CDialog
{
// コンストラクション
public:
	CDeleteDataPeriodSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDeleteDataPeriodSettingDlg)
	enum { IDD = IDD_DELETE_DATA_PERIOD_SETTING_DLG };
	//}}AFX_DATA
protected:
	CColorStatic m_stcDeleteDataDate;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_DataDelButton;

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDeleteDataPeriodSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDeleteDataPeriodSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDeleteDataPeriod();
	virtual void OnOK();
	afx_msg void OnDataDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void InitCombo_DeleteDataPeriod();

	int m_iDeleteDataPeriod;
	CString m_strDeleteDataDate;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DELETEDATAPERIODSETTINGDLG_H__39D5E95A_7582_47EF_98EE_35FA60AEDBA3__INCLUDED_)
