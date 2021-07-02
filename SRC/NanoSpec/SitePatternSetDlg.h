#if !defined(AFX_SITEPATTERNSETDLG_H__96349D3B_FBFF_4D54_A674_3F0FE9EED193__INCLUDED_)
#define AFX_SITEPATTERNSETDLG_H__96349D3B_FBFF_4D54_A674_3F0FE9EED193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"
#include "GridCellCheck.h"

/////////////////////////////////////////////////////////////////////////////
// CSitePatternSetDlg ダイアログ

class CSitePatternSetDlg : public CDialog
{
// コンストラクション
public:
	CSitePatternSetDlg(STAGE_PROG_INFO_HDR* pStageProgInfoHdr, CWnd* pParent = NULL); // 標準のコンストラクタ
	~CSitePatternSetDlg();

	// StageProgramとのデータの引渡し
	STAGE_PROG_INFO_HDR* m_pStageProgInfoHdr;
	STAGE_PROG_INFO_HDR m_OldStageProgInfoHdr;
	STAGE_COORD		m_ScanPoint[SCAN_POINT_MAX];
	SITE_PATTERN	m_SitePoint[SCAN_POINT_MAX];
	int				m_iNumScans;

// ダイアログ データ
	//{{AFX_DATA(CSitePatternSetDlg)
	enum { IDD = IDD_SITE_PATTERN_SET_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

	CColorStatic m_stcStageProgramName;
	CColorStatic m_stcLens1;
	CColorStatic m_stcOffsetX1;
	CColorStatic m_stcOffsetY1;
	CColorStatic m_stcLens2;
	CColorStatic m_stcOffsetX2;
	CColorStatic m_stcOffsetY2;
	CShadeButtonST m_Pattern1SetButton;
	CShadeButtonST m_Pattern2SetButton;
	CShadeButtonST m_PatternChgButton;
	CShadeButtonST m_PatternAllChgButton;
	CShadeButtonST m_PatternDelButton;
	CShadeButtonST m_PatternAllDelButton;
	CShadeButtonST m_AllEnableButton;
	CShadeButtonST m_AllDisableButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	CGridCtrl m_MeasurementPointGrid;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSitePatternSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSitePatternSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPattern1SetupButton();
	afx_msg void OnPattern2SetupButton();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeSitePrExecutionMode();
	afx_msg void OnSelchangeSitePrFailOption();
	afx_msg void OnPatternChgButton();
	afx_msg void OnPatternAllChgButton();
	afx_msg void OnPatternDelButton();
	afx_msg void OnPatternAllDelButton();
	afx_msg void OnAllEnableButton();
	afx_msg void OnAllDisableButton();
	//}}AFX_MSG
	afx_msg void OnSelChangeing(NMHDR *pNotifyStruct, LRESULT* /*pResult*/);
	DECLARE_MESSAGE_MAP()

	void PatternSetUp(int nSite);
	BOOL MeasurementPointGrid_Init(int iRows);
	void GridDisp(int row, int col, LPCTSTR pszText, DWORD TextStyle);
	void GridEditable(BOOL Editable = TRUE);
	void SetData();
	void DispPattern();
	void OperationEnable(BOOL bFlg);
	void SetCursor(CCellID cell);
	int  CheckSelectItem();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	CBitmapStatic m_stcSitePrImg[SITE_PATTERN_MAX];
	CCellID m_CurrentCellID;
	int		m_SelectMode;
	int		m_FailMode;
	CString	m_szLens1;
	CString m_szOffsetX1;
	CString m_szOffsetY1;
	CString	m_szLens2;
	CString m_szOffsetX2;
	CString m_szOffsetY2;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SITEPATTERNSETDLG_H__96349D3B_FBFF_4D54_A674_3F0FE9EED193__INCLUDED_)
