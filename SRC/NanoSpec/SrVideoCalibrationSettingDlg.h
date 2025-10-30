#if !defined(AFX_SRVIDEOCALIBRATIONSETTINGDLG_H__7CB23C2A_B954_475F_92AD_47C40478FA15__INCLUDED_)
#define AFX_SRVIDEOCALIBRATIONSETTINGDLG_H__7CB23C2A_B954_475F_92AD_47C40478FA15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SrVideoCalibrationSettingDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"

// ##########################################################################
// CSrVideoCalibrationSettingDlg ダイアログ
// ##########################################################################

class CSrVideoCalibrationSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrVideoCalibrationSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrVideoCalibrationSettingDlg)
	enum { IDD = IDD_SR_VIDEO_CALIBRATION_SETTING_DLG };
	//}}AFX_DATA

//opration
public:
	CColorStatic m_stcLens1_X, m_stcLens1_Y;
	CColorStatic m_stcLens2_X, m_stcLens2_Y;
	CColorStatic m_stcLens3_X, m_stcLens3_Y;
	CColorStatic m_stcLens4_X, m_stcLens4_Y;
	CShadeButtonST m_Lens1SetupButton;
	CShadeButtonST m_Lens2SetupButton;
	CShadeButtonST m_Lens3SetupButton;
	CShadeButtonST m_Lens4SetupButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	SR_VIDEO_CALIB m_SrVideoCalib[SR_LENS_MAX];
	SR_TURRET m_SrTurret[SR_LENS_MAX];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrVideoCalibrationSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL
	void DDX_CustomLensNameText(CDataExchange* pDX, int nIdcName, int nIdcSetupButton, BOOL bTurretEnable, LPCTSTR pszTurretName);
	void DDX_CustomLensXyText(CDataExchange* pDX, int nIDC, double value);

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrVideoCalibrationSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLens1SetupButton();
	afx_msg void OnLens2SetupButton();
	afx_msg void OnLens3SetupButton();
	afx_msg void OnLens4SetupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void LensSetupButton(UINT nID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRVIDEOCALIBRATIONSETTINGDLG_H__7CB23C2A_B954_475F_92AD_47C40478FA15__INCLUDED_)
