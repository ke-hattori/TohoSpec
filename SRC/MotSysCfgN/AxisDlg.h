#if !defined(AFX_AXISDLG_H__B7E0D97A_140A_40FA_8064_4596A42E0B79__INCLUDED_)
#define AFX_AXISDLG_H__B7E0D97A_140A_40FA_8064_4596A42E0B79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxisDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAxisDlg ダイアログ

class CAxisDlg : public CDialog
{
// コンストラクション
public:
	void GetData(void);
	void DispData(void);
	int m_AxisNo;
	BOOL Create(CWnd *pWnd, int AxisNo);
	CWnd* m_pParent;
	CAxisDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CAxisDlg)
	enum { IDD = IDD_AXIS_DLG };
	short	m_Acceleration;
	CString	m_DefaultMotParams;
	long	m_Destination;
	short	m_FWD_Home;
	BYTE	m_HCTLStatusReg;
	short	m_HighGain;
	short	m_HomeAccel;
	short	m_HomeVelocity;
	short	m_IntegVelocity;
	short	m_InvertedSense;
	short	m_LowGain;
	short	m_MaxVelocity;
	long	m_MinusLimit;
	short	m_Mode;
	long	m_PlusLimit;
	short	m_Pole;
	short	m_PropVelocity;
	BYTE	m_PwmFilterCount;
	BYTE	m_PwmMax;
	short	m_SampleTime;
	BYTE	m_Unused;
	short	m_Zero;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAxisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAxisDlg)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBtnLoad();
	afx_msg void OnBtnSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_AXISDLG_H__B7E0D97A_140A_40FA_8064_4596A42E0B79__INCLUDED_)
