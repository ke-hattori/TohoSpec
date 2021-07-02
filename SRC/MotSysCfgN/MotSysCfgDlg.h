// MotSysCfgDlg.h : ヘッダー ファイル
//

#if !defined(AFX_MOTSYSCFGDLG_H__CCEA29A7_9123_11D7_AE9A_0040266ED145__INCLUDED_)
#define AFX_MOTSYSCFGDLG_H__CCEA29A7_9123_11D7_AE9A_0040266ED145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
 *	マクロ定義
 */
#define	IDENT_CODE		"MotSysCfg"	// 識別コード(プロパティリストで使用)

/////////////////////////////////////////////////////////////////////////////
// CMotSysCfgDlg ダイアログ

class CMotCfg;
class CAxisDlg;
class CServoParam;

class CMotSysCfgDlg : public CDialog
{
// 構築
public:
	void EndServoParam();
	CServoParam* m_pCServoParam;
	CAxisDlg* m_pCAxisDlgBigX;
	CAxisDlg* m_pCAxisDlgBigY;
	CAxisDlg* m_pCAxisDlgZ;
	CAxisDlg* m_pCAxisDlgXslow;
	CAxisDlg* m_pCAxisDlgT;
	CAxisDlg* m_pCAxisDlgL;
	CAxisDlg* m_pCAxisDlgXfast;
	CAxisDlg* m_pCAxisDlgY;
	void EndAxisDlg(int AxisNo);
	HICON m_hMainIcon;
	void EndMotCfgDlg(void);
	CMotCfg* m_pCMotCfg;
	BOOL m_OnClose;
	CMotSysCfgDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMotSysCfgDlg)
	enum { IDD = IDD_MOTSYSCFG_DIALOG };
	BOOL	m_chkMotCfg;
	BOOL	m_chkBigX;
	BOOL	m_chkBigY;
	BOOL	m_chkLevel;
	BOOL	m_chkTheta;
	BOOL	m_chkXFast;
	BOOL	m_chkXSlow;
	BOOL	m_chkY;
	BOOL	m_chkZ;
	BOOL	m_chkServo;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotSysCfgDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMotSysCfgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnBtnAbout();
	afx_msg void OnChkMotcfg();
	afx_msg void OnChkBigx();
	afx_msg void OnChkBigy();
	afx_msg void OnChkZ();
	afx_msg void OnChkXslow();
	afx_msg void OnChkTheta();
	afx_msg void OnChkLevel();
	afx_msg void OnChkXfast();
	afx_msg void OnChkY();
	afx_msg void OnChkServo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTSYSCFGDLG_H__CCEA29A7_9123_11D7_AE9A_0040266ED145__INCLUDED_)
