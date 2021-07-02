#if !defined(AFX_SPECIFYCOORDINATEDLG_H__338366B1_58AC_4D1B_9D5A_230A36A95A3C__INCLUDED_)
#define AFX_SPECIFYCOORDINATEDLG_H__338366B1_58AC_4D1B_9D5A_230A36A95A3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecifyCoordinateDlg.h : ヘッダー ファイル
//
#define DEFAULT_WND		0
// ##########################################################################
// CSpecifyCoordinateDlg ダイアログ
// ##########################################################################

class CSpecifyCoordinateDlg : public CDialog
{
// コンストラクション
public:
	CSpecifyCoordinateDlg(int iType = DEFAULT_WND, double dSampleSizeX = 0, double dSampleSizeY = 0, CWnd* pParent = NULL); // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSpecifyCoordinateDlg)
	enum { IDD = IDD_SPECIFY_COORDINATE_DLG };
	//}}AFX_DATA

// Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	int m_iType;
	double m_dSampleSizeX;
	double m_dSampleSizeY;
	long m_lX, m_lY;
	int m_iMoveMode;
	int m_nSampleMode;

	BOOL CheckData();
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSpecifyCoordinateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSpecifyCoordinateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCoordinate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetOperationLog(LPCTSTR pszLog);

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	STAGE_CONFIG m_StageConfig;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPECIFYCOORDINATEDLG_H__338366B1_58AC_4D1B_9D5A_230A36A95A3C__INCLUDED_)
