#if !defined(AFX_POINTSETUPDISPOSITIONDLG_H__0E47BF02_6B75_4F92_AF77_8D8341C7FA04__INCLUDED_)
#define AFX_POINTSETUPDISPOSITIONDLG_H__0E47BF02_6B75_4F92_AF77_8D8341C7FA04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointSetupDispositionDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CPointSetupDispositionDlg ダイアログ
// ##########################################################################

class CPointSetupDispositionDlg : public CDialog
{
// コンストラクション
public:
	CPointSetupDispositionDlg(int iNumScans = 0, double dSampleSizeX = 0, double dSampleSizeY = 0, int nOrgPos = 0,CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPointSetupDispositionDlg)
	enum { IDD = IDD_POINT_SETUP_DISPOSITION_DLG };
	//}}AFX_DATA

// Operation
public:
	CGridCtrl m_DispositionGrid;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	double m_dSampleSizeX;
	double m_dSampleSizeY;
	//2009.09.30 bagus se --{--
	int m_nOrgPos;
	//2009.09.30 bagus se --}--

//	STAGE_CONFIG m_StageConfig;
	long m_lStartPointX, m_lStartPointY;
	long m_lPitchX, m_lPitchY;
	int m_iCountX, m_iCountY;
	int m_iDrawShaped;
	int m_iNumScans;

	void GridDisp(CGridCtrl* pGrid, int row, int col, char* text, DWORD TextStyle);
	void GridEditable(CGridCtrl* pGrid, BOOL Editable);
	BOOL DispositionGrid_Init(CGridCtrl* pGrid);
	BOOL CheckData(CGridCtrl* pGrid);

	//2009.09.30 bagus se --{--
	//原点位置対応
	BOOL IsPosInStageSizeX(double dPos);
	BOOL IsPosInStageSizeY(double dPos);
	void GetPosRangeX(long *pMin,long *pMax);
	void GetPosRangeY(long *pMin,long *pMax);
	//2009.09.30 bagus se --}--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPointSetupDispositionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPointSetupDispositionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_POINTSETUPDISPOSITIONDLG_H__0E47BF02_6B75_4F92_AF77_8D8341C7FA04__INCLUDED_)
