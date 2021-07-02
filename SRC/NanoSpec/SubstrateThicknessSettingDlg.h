#if !defined(AFX_SUBSTRATETHICKNESSSETTINGDLG_H__FBBA317A_3ACE_4815_8EC3_745B61E9A8E6__INCLUDED_)
#define AFX_SUBSTRATETHICKNESSSETTINGDLG_H__FBBA317A_3ACE_4815_8EC3_745B61E9A8E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubstrateThicknessSettingDlg.h : ヘッダー ファイル
//

#include "NanoListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSubstrateThicknessSettingDlg ダイアログ

class CSubstrateThicknessSettingDlg : public CDialog
{
// コンストラクション
public:
	CSubstrateThicknessSettingDlg(BOOL bNew, CWnd* pParent = NULL);   // 標準のコンストラクタ

	///// Variable /////
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CEdit m_wndNoEdit;
	CEdit m_wndSubstrateThicknessEdit;
	CEdit m_wndInvalidRegionEdit;
	int m_iNo;
	SUBSTRATE_THICKNESS m_SubstrateThickness;

// ダイアログ データ
	//{{AFX_DATA(CSubstrateThicknessSettingDlg)
	enum { IDD = IDD_SUBSTRATE_THICKNESS_SETTING_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSubstrateThicknessSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	double MilliToMicro(double dMilli);
	double MicroToMilli(double dMicro);
	void SubstrateThicknessList_Init();
	void ReferenceThicknessList_Init();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSubstrateThicknessSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	BOOL m_bNew;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SUBSTRATETHICKNESSSETTINGDLG_H__FBBA317A_3ACE_4815_8EC3_745B61E9A8E6__INCLUDED_)
