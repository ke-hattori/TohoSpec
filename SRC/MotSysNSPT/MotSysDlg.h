// MotSysDlg.h : ヘッダー ファイル
//

#if !defined(AFX_MOTSYSDLG_H__5817BA20_B6FA_45E3_B048_A4FA69263851__INCLUDED_)
#define AFX_MOTSYSDLG_H__5817BA20_B6FA_45E3_B048_A4FA69263851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
 *	マクロ定義
 */
#define	IDENT_CODE		"MotSys"	// 識別コード(プロパティリストで使用)
#define	MSD_MESSAGEBOX_TITLE	_T("Motion System Driver Error")

class CSio;
class CLogFile;

/////////////////////////////////////////////////////////////////////////////
// CMotSysDlg ダイアログ

class CMotSysDlg : public CDialog
{
// 構築

// Matsuhisa 20040304 Add -->
protected:
public:
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- { ---------- */
	BOOL m_bDioLog;
	void DioLogging(DWORD dwDi, DWORD dwDo);
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- } ---------- */
	BOOL m_bOnClosing;
	HICON m_hMainIcon;
// Matsuhisa 20040302 Add -->
	int m_AutoHide;
	long m_dPos[3][AXIS_NUM + 1];
	void DispdPos(WORD axis, long d_Pos);
// Matsuhisa 20040302 Add <--
	void CnvWordToBin(WORD WordData, TCHAR *ptszBinary, WORD mode);
	void CloseObjects(void);
	void ShowAlarm(WORD Mode, WORD Axis, WORD Code1, WORD Code2);
	BOOL m_OnClose;
	CLogFile* m_pComLogFile;
	BOOL InitializeCommLog(void);
	BOOL InitializeComm(void);
	CSio* m_pSio;
	LPVOID m_SharedMapView;
	HANDLE m_hSharedMap;
	void ShowLastError(DWORD code, TCHAR *ptszErrMsg);
	void ChgToPriorityMsg(DWORD code, TCHAR *ptszmsg);
	LRESULT MotsysProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MotsysDriverProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	CMotSysDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMotSysDlg)
	enum { IDD = IDD_MOTSYS_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotSysDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMotSysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnAbout();
	afx_msg LRESULT OnBtnHide(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBTNServoOFF();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnBTNORGtest();
	afx_msg void OnBTNServoON();
	afx_msg void OnBTNORGtestY();
	afx_msg void OnBTNORGtestZ();
	afx_msg void OnBTNORGtestT();
	afx_msg void OnBTNABStestX();
	afx_msg void OnBTNABStestY();
	afx_msg void OnBTNABStestZ();
	afx_msg void OnBTNABStestT();
	afx_msg void OnBTNINCtestX();
	afx_msg void OnBTNINCtestY();
	afx_msg void OnBTNINCtestZ();
	afx_msg void OnBTNINCtestT();
	afx_msg void OnBTNHPtestX();
	afx_msg void OnBTNHPtestY();
	afx_msg void OnBTNHPtestZ();
	afx_msg void OnBTNHPtestT();
	afx_msg void OnBTNABStestX2();
	afx_msg void OnBTNABStestY2();
	afx_msg void OnBTNABStestZ2();
	afx_msg void OnBTNABStestT2();
	afx_msg void OnBTNINCtestX2();
	afx_msg void OnBTNINCtestY2();
	afx_msg void OnBTNINCtestZ2();
	afx_msg void OnBTNINCtestT2();
// Matsuhisa 20040302 Add -->
	afx_msg void OnBtnServoParam();
	afx_msg void OnBtnDPosPeakClearX();
	afx_msg void OnBtnDPosPeakClearYM();
	afx_msg void OnBtnDPosPeakClearYS();
	afx_msg void OnBtnDPosPeakClearZ();
	afx_msg void OnBtnDPosPeakClearT();
// Matsuhisa 20040302 Add <--
	//}}AFX_MSG
	afx_msg LRESULT OnMotsysShowWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Setwindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Setsofthome(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Emergencystop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Moveatspeed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Movetoposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Getposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Gohome(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Moveatspeedtodest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_Isstopped(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_IsJoyMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_SetJoyMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_GetMaxSpeed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_GetMotionSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_GetElectronicGear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_GetSoftLimit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMotdrv_SetJoySpeed(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowAlarm(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnServoInitComp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- { ---------- */
	CLogFile* m_pcDioLog;
/* added 2009.12.14 hmenjo MotSys DIO ログ追加 ---------- } ---------- */
	void AlarmLogging(TCHAR* ptszAlarmLog);
	CLogFile* m_pAlarmLogFile;
	void EnableDlgCtrl(void);
	BOOL m_bDIO_Opned;
	BOOL m_bInitFailed;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTSYSDLG_H__5817BA20_B6FA_45E3_B048_A4FA69263851__INCLUDED_)
