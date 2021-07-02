#if !defined(AFX_SYSTEMCONFIGURATIONSETTINGDLG_H__57BB9D77_ED44_4B52_AE75_E663DB98D6B9__INCLUDED_)
#define AFX_SYSTEMCONFIGURATIONSETTINGDLG_H__57BB9D77_ED44_4B52_AE75_E663DB98D6B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemConfigurationSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CSystemConfigurationSettingDlg ダイアログ
// ##########################################################################

class CSystemConfigurationSettingDlg : public CDialog
{
// コンストラクション
public:
	CSystemConfigurationSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CSystemConfigurationSettingDlg)
	enum { IDD = IDD_SYSTEM_CONFIGURATION_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

// オーバーライド
	//{{AFX_VIRTUAL(CSystemConfigurationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CSystemConfigurationSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCaptureBoard();
	afx_msg void OnSelchangePRMethod();
	afx_msg void OnSelchangeLanguage();
	afx_msg void OnSelchangeStageType();
	afx_msg void OnSelchangeVibrationControlUnit();
	afx_msg void OnSelchangeSampleLifter();
	afx_msg void OnSelchangeVacuumType();
	afx_msg void OnSelchangeCorrectAngle();
	afx_msg void OnSelchangeJoystickType();
	afx_msg void OnCheckHeadTypeSe();
	afx_msg void OnCheckHeadTypeCompEASE();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitCombo_CaptureBoard();
	void InitCombo_PRMethod();
	void InitCombo_Language();
	void InitCombo_StageType();
	void InitCombo_StageVibrationControlUnit();
	void InitCombo_StageSampleLifter();
	void InitCombo_StageVacuumType();
	void InitCombo_StageCorrectAngle();
// 2009.10.29 bagus Jog 追加修正 --{--
	void InitCombo_StageJoystickType();
// 2009.10.29 bagus Jog 追加修正 --}--
	BOOL CheckData();

	SYSTEM_CONFIG m_SystemConfig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMCONFIGURATIONSETTINGDLG_H__57BB9D77_ED44_4B52_AE75_E663DB98D6B9__INCLUDED_)
