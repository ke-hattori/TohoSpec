// MotSysDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_MOTSYSDLG_H__5817BA20_B6FA_45E3_B048_A4FA69263851__INCLUDED_)
#define AFX_MOTSYSDLG_H__5817BA20_B6FA_45E3_B048_A4FA69263851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
 *	�}�N����`
 */
#define	IDENT_CODE		"MotSys"	// ���ʃR�[�h(�v���p�e�B���X�g�Ŏg�p)
#define	MSD_MESSAGEBOX_TITLE	_T("Motion System Driver Error")

class CSio;
class CLogFile;

/////////////////////////////////////////////////////////////////////////////
// CMotSysDlg �_�C�A���O

class CMotSysDlg : public CDialog
{
// �\�z

// Matsuhisa 20040304 Add -->
protected:
public:
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
	BOOL m_bDioLog;
	void DioLogging(DWORD dwDi, DWORD dwDo);
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */
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
	CMotSysDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMotSysDlg)
	enum { IDD = IDD_MOTSYS_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMotSysDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMotSysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnAbout();
	afx_msg void OnBtnHide();
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
	afx_msg LRESULT OnMotsysHideWindow(WPARAM wParam, LPARAM lParam);
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
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
	CLogFile* m_pcDioLog;
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */
	void AlarmLogging(TCHAR* ptszAlarmLog);
	CLogFile* m_pAlarmLogFile;
	void EnableDlgCtrl(void);
	BOOL m_bDIO_Opned;
	BOOL m_bInitFailed;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MOTSYSDLG_H__5817BA20_B6FA_45E3_B048_A4FA69263851__INCLUDED_)
