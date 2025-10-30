#if !defined(AFX_SPECIFYCOORDINATE2POINTDLG_H__5DB5EE72_42E3_4BC8_AA17_C199BFE59EDB__INCLUDED_)
#define AFX_SPECIFYCOORDINATE2POINTDLG_H__5DB5EE72_42E3_4BC8_AA17_C199BFE59EDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecifyCoordinate2PointDlg.h : ヘッダー ファイル
//
#define DEFAULT_WND		0
/////////////////////////////////////////////////////////////////////////////
// CSpecifyCoordinate2PointDlg ダイアログ

class CSpecifyCoordinate2PointDlg : public CDialog
{
// コンストラクション
public:
	CSpecifyCoordinate2PointDlg(int iType = DEFAULT_WND, double dSampleSizeX = 0, double dSampleSizeY = 0, CWnd* pParent = NULL); // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSpecifyCoordinate2PointDlg)
	enum { IDD = IDD_SPECIFY_COORDINATE_2POINT_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// Operation
public:
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	int m_iType;
	double m_dSampleSizeX;
	double m_dSampleSizeY;
	long m_lX1, m_lY1;
	long m_lX2, m_lY2;
	int m_iMoveMode;
	int m_nSampleMode;

	BOOL CheckData();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSpecifyCoordinate2PointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSpecifyCoordinate2PointDlg)
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

#endif // !defined(AFX_SPECIFYCOORDINATE2POINTDLG_H__5DB5EE72_42E3_4BC8_AA17_C199BFE59EDB__INCLUDED_)
