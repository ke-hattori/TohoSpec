#if !defined(AFX_DESKEWDLG_H__7A53405E_9370_4F3E_81EB_3844C875C1F5__INCLUDED_)
#define AFX_DESKEWDLG_H__7A53405E_9370_4F3E_81EB_3844C875C1F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeskewDlg.h : ヘッダー ファイル
//
#include "..\\..\\INC\\PatRec.hxx"
#include "ColorStatic.h"

// ##########################################################################
// CDeskewDlg ダイアログ
// ##########################################################################

class CDeskewDlg : public CDialog
{
// コンストラクション
public:
	CDeskewDlg(STAGE_PROG_INFO_HDR* pStageProgInfoHdr, CWnd* pParent = NULL); // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDeskewDlg)
	enum { IDD = IDD_DESKEW_DLG };
	//}}AFX_DATA

//Operation
public:
	STAGE_PROG_INFO_HDR* m_pStageProgInfoHdr;
	STAGE_PROG_INFO_HDR* pStageProgInfoHdr;
	STAGE_PROG_INFO_HDR m_OldStageProgInfoHdr;
	CColorStatic m_stcAutoDeskew1_X;
	CColorStatic m_stcAutoDeskew1_Y;
	CColorStatic m_stcAutoDeskew2_X;
	CColorStatic m_stcAutoDeskew2_Y;
	CColorStatic m_stcManualDeskew1_X;
	CColorStatic m_stcManualDeskew1_Y;
	CColorStatic m_stcManualDeskew2_X;
	CColorStatic m_stcManualDeskew2_Y;
	CShadeButtonST m_AutoDeskew1SetupButton;
	CShadeButtonST m_AutoDeskew2SetupButton;
	CShadeButtonST m_ManualDeskew1SetupButton;
	CShadeButtonST m_ManualDeskew2SetupButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDeskewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL
	void DDX_CustomLocText(CDataExchange* pDX, int nIDC, long& value);
	void DDV_CustomComboExecutionModeCheck(CDataExchange* pDX);
	void DDV_CustomComboFailOptionCheck(CDataExchange* pDX);
	void DDV_CustomAutoDeskewLensCheck(CDataExchange* pDX);
// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDeskewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAutoDeskew1SetupButton();
	afx_msg void OnAutoDeskew2SetupButton();
	afx_msg void OnManualDeskew1SetupButton();
	afx_msg void OnManualDeskew2SetupButton();
	afx_msg void OnSelchangeDeskewExecutionMode();
	afx_msg void OnSelchangeDeskewFailOption();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CBitmapStatic m_stcDeskewImg[DESKEW_SITE_MAX];

private:
	void DrawModel(WORD wSite);
	CBitmap m_bmpDeskewSite[DESKEW_SITE_MAX];
	int m_iAutoDeskewLensCheck[DESKEW_SITE_MAX];

// 2013.02.22 bagus Substrate thickness setting -->
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
// 2013.02.22 bagus Substrate thickness setting <--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DESKEWDLG_H__7A53405E_9370_4F3E_81EB_3844C875C1F5__INCLUDED_)
