// TerminatorDlg.h : ヘッダー ファイル
//

#if !defined(AFX_TERMINATORDLG_H__C30A490B_E7A9_4DE4_B1FC_58D9FA611325__INCLUDED_)
#define AFX_TERMINATORDLG_H__C30A490B_E7A9_4DE4_B1FC_58D9FA611325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTerminatorDlg ダイアログ

class CTerminatorDlg : public CDialog
{
// 構築
public:
	CTerminatorDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

	void ListViewInit();
	void ListViewInsert();

	BOOL FindAndKillApp();
	BOOL KillProc(DWORD Pid);
	int GetList();
	void RestartWin(UINT param);
	BOOL OnShowcmdline();

	CString GGG[100];
	long	DDD[100];
	long	SaveDDD[100];
	int		MaxNum;

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
protected:
	void GetRootDirPath();
	void MakeProcessResetPath();
	void MakeBinPath();
	CString GetReleaseBinRelativePath();
	CString GetDebugBinRelativePath();

	CString m_strCurrentDirectory;
	CString m_strRootDirectory;
	CString m_strProcessResetDirPath;
	CString m_strForceRestartReleaseBatFilePath;
	CString m_strForceRestartDebugBatFilePath;
	CString m_strProcessListTxtFilePath;
	CString m_strNanoSpecReleaseExeFilePath;
	CString m_strNanoSpecDebugExeFilePath;

public:
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

// ダイアログ データ
	//{{AFX_DATA(CTerminatorDlg)
	enum { IDD = IDD_TERMINATOR_DIALOG };
	CListCtrl	m_List;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTerminatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTerminatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnForceRestart();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnForceKill();
	virtual void OnCancel();
	afx_msg void OnReAgentCD();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TERMINATORDLG_H__C30A490B_E7A9_4DE4_B1FC_58D9FA611325__INCLUDED_)
