#if !defined(AFX_POINTSETUPDISPOSITION2POINTDLG_H__2444AD56_0D65_4D64_9E2B_526BEF7EBD7C__INCLUDED_)
#define AFX_POINTSETUPDISPOSITION2POINTDLG_H__2444AD56_0D65_4D64_9E2B_526BEF7EBD7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupDisposition2PointDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPointSetupDisposition2PointDlg ダイアログ

class CPointSetupDisposition2PointDlg : public CDialog
{
// コンストラクション
public:
	CPointSetupDisposition2PointDlg(int iNumScans = 0, double dSampleSizeX = 0, double dSampleSizeY = 0, int nOrgPos = 0,CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPointSetupDisposition2PointDlg)
	enum { IDD = IDD_POINT_SETUP_DISPOSITION_2POINT_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
public:
	CGridCtrl m_DispositionGrid;
	CGridCtrl m_DispositionGrid2;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	double m_dSampleSizeX;
	double m_dSampleSizeY;
	int m_nOrgPos;

	long m_lStartPointX1, m_lStartPointY1;
	long m_lPitchX1, m_lPitchY1;
	long m_lStartPointX2, m_lStartPointY2;
	long m_lPitchX2, m_lPitchY2;
	int m_iCountX, m_iCountY;
	int m_iDrawShaped;
	int m_iNumScans;

	void GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle);
	void GridEditable(CGridCtrl* pGrid, BOOL Editable);
	BOOL DispositionGrid_Init();
	BOOL DispositionGrid2_Init();

	BOOL CheckData();

	//原点位置対応
	BOOL IsPosInStageSizeX(double dPos);
	BOOL IsPosInStageSizeY(double dPos);
	void GetPosRangeX(long *pMin,long *pMax);
	void GetPosRangeY(long *pMin,long *pMax);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupDisposition2PointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupDisposition2PointDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPDISPOSITION2POINTDLG_H__2444AD56_0D65_4D64_9E2B_526BEF7EBD7C__INCLUDED_)
