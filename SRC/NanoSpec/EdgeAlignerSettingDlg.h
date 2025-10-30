#if !defined(AFX_EDGEALIGNERSETTINGDLG_H__55C222FB_FD80_48DE_BB81_A82BD8D632C9__INCLUDED_)
#define AFX_EDGEALIGNERSETTINGDLG_H__55C222FB_FD80_48DE_BB81_A82BD8D632C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"

// EdgeAlignerSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CEdgeAlignerSettingDlg ダイアログ
// ##########################################################################

class CEdgeAlignerSettingDlg : public CDialog
{
// コンストラクション
public:
	CEdgeAlignerSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
protected:
	//{{AFX_DATA(CEdgeAlignerSettingDlg)
	enum { IDD = IDD_EDGE_ALIGNER_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_StartSetupButton;
	CShadeButtonST m_EndSetupButton;
	CShadeButtonST m_SensorPositionScanButton;
	CColorStatic m_stcStartX;
	CColorStatic m_stcStartY;
	CColorStatic m_stcEndX;
	CColorStatic m_stcEndY;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEdgeAlignerSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CEdgeAlignerSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnStartSetupButton();
	afx_msg void OnEndSetupButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL CheckData();
	EDGE_ALIGNER m_EdgeAligner;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDGEALIGNERSETTINGDLG_H__55C222FB_FD80_48DE_BB81_A82BD8D632C9__INCLUDED_)
