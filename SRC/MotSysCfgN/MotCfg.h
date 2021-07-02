#if !defined(AFX_MOTCFG_H__2D5C9826_338A_4E96_A117_899749E11EAB__INCLUDED_)
#define AFX_MOTCFG_H__2D5C9826_338A_4E96_A117_899749E11EAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotCfg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMotCfg ダイアログ

class CMotCfg : public CDialog
{
// コンストラクション
public:
	void GetData(void);
	void DispData(void);
	CWnd* m_pParent;
	CMotCfg(CWnd* pParent = NULL);	 // 標準のコンストラクタ
	BOOL Create(CWnd *pWnd);

// ダイアログ データ
	//{{AFX_DATA(CMotCfg)
	enum { IDD = IDD_MOTCFG_DLG };
	CString	m_BaseAddr1;
	CString	m_BaseAddr2;
	int		m_AxisNum;
	CString	m_AxisNameBigX;
	CString	m_AxisNameBigY;
	CString	m_AxisNameLevel;
	CString	m_AxisNameTheta;
	CString	m_AxisNameXFast;
	CString	m_AxisNameXSlow;
	CString	m_AxisNameY;
	CString	m_AxisNameZ;
	CString	m_HCTLPortBigX;
	CString	m_HCTLPortBigY;
	CString	m_HCTLPortLevel;
	CString	m_HCTLPortTheta;
	CString	m_HCTLPortXFast;
	CString	m_HCTLPortXSlow;
	CString	m_HCTLPortY;
	CString	m_HCTLPortZ;
	CString	m_HomeBitBigX;
	CString	m_HomeBitBigY;
	CString	m_HomeBitLevel;
	CString	m_HomeBitTheta;
	CString	m_HomeBitXFast;
	CString	m_HomeBitXSlow;
	CString	m_HomeBitY;
	CString	m_HomeBitZ;
	CString	m_HomeIntBitBigX;
	CString	m_HomeIntBitBigY;
	CString	m_HomeIntBitLevel;
	CString	m_HomeIntBitTheta;
	CString	m_HomeIntBitXFast;
	CString	m_HomeIntBitXSlow;
	CString	m_HomeIntBitY;
	CString	m_HomeIntBitZ;
	CString	m_HomeIntPortBigX;
	CString	m_HomeIntPortBigY;
	CString	m_HomeIntPortLevel;
	CString	m_HomeIntPortTheta;
	CString	m_HomeIntPortXFast;
	CString	m_HomeIntPortXSlow;
	CString	m_HomeIntPortY;
	CString	m_HomeIntPortZ;
	CString	m_HomePortBigX;
	CString	m_HomePortBigY;
	CString	m_HomePortLevel;
	CString	m_HomePortTheta;
	CString	m_HomePortXFast;
	CString	m_HomePortXSlow;
	CString	m_HomePortY;
	CString	m_HomePortZ;
	CString	m_StopBitBigX;
	CString	m_StopBitBigY;
	CString	m_StopBitLevel;
	CString	m_StopBitTheta;
	CString	m_StopBitXFast;
	CString	m_StopBitXSlow;
	CString	m_StopBitY;
	CString	m_StopBitZ;
	CString	m_UnusedBigX;
	CString	m_UnusedBigY;
	CString	m_UnusedLevel;
	CString	m_UnusedTheta;
	CString	m_UnusedXFast;
	CString	m_UnusedXSlow;
	CString	m_UnusedY;
	CString	m_UnusedZ;
	CString	m_StopPortBigX;
	CString	m_StopPortBigY;
	CString	m_StopPortLevel;
	CString	m_StopPortTheta;
	CString	m_StopPortXFast;
	CString	m_StopPortXSlow;
	CString	m_StopPortY;
	CString	m_StopPortZ;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMotCfg)
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

#endif // !defined(AFX_MOTCFG_H__2D5C9826_338A_4E96_A117_899749E11EAB__INCLUDED_)
