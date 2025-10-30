#if !defined(AFX_ORIGINALPOSITIONSETTINGDLG_H__43170470_6CB8_4688_8DB2_0D24EC1F2B66__INCLUDED_)
#define AFX_ORIGINALPOSITIONSETTINGDLG_H__43170470_6CB8_4688_8DB2_0D24EC1F2B66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OriginalPositionSettingDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"

// ##########################################################################
// COriginalPositionSettingDlg ダイアログ
// ##########################################################################

class COriginalPositionSettingDlg : public CDialog
{
// コンストラクション
public:
	COriginalPositionSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(COriginalPositionSettingDlg)
	enum { IDD = IDD_ORIGINAL_POSITION_SETTING_DLG };
	//}}AFX_DATA

// Operation
public:
// 2009.09.17 bagus SE --{--
	CColorStatic m_stcPos_X, m_stcPos_Y, m_stcPos_Z;
// 2009.09.17 bagus SE --}--

	CColorStatic m_stcSR_X, m_stcSR_Y, m_stcSR_Z;
	CColorStatic m_stcSE_X, m_stcSE_Y, m_stcSE_Z;
	CColorStatic m_stc4PP_X, m_stc4PP_Y, m_stc4PP_Z;
	CColorStatic m_stcCONTACT_ANGLE_X, m_stcCONTACT_ANGLE_Y, m_stcCONTACT_ANGLE_Z;
	CColorStatic m_stcSTRESS_X, m_stcSTRESS_Y, m_stcSTRESS_Z;
// 2009.10.19 bagus MS 追加 --{--
//	CColorStatic m_stcIRSE_X, m_stcIRSE_Y, m_stcIRSE_Z;
	CColorStatic m_stcMS_X, m_stcMS_Y, m_stcMS_Z;
// 2009.10.19 bagus MS 追加 --}--

	CShadeButtonST m_SrSetupButton;
	CShadeButtonST m_SeSetupButton;
	CShadeButtonST m_4PPSetupButton;
	CShadeButtonST m_ContactAngleSetupButton;
	CShadeButtonST m_StressSetupButton;
	CShadeButtonST m_IrseSetupButton;

	CShadeButtonST m_AllClearButton;
// 2009.09.17 bagus SE --{--
	CShadeButtonST m_CheckButton;
	CShadeButtonST m_ApplyButton;
// 2009.09.17 bagus SE --}--

	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;

	HEAD_TYPE_CONFIG m_HeadType;
	ORIGINAL_POSITION m_OriginalPosition;
	int m_iBaseHeadType;
	BOOL m_bBaseHeadEnable;

	BOOL Init();
	void UpDate(BOOL);
	void EnableSetButtons(BOOL);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COriginalPositionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COriginalPositionSettingDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSrSetupButton();
	afx_msg void OnSeSetupButton();
	afx_msg void OnMsSetupButton();
	afx_msg void On4PPSetupButton();
	afx_msg void OnContactAngleSetupButton();
	afx_msg void OnStressSetupButton();
	afx_msg void OnAllClear();
	afx_msg void OnSelchangeBaseHeadType();
// 2009.09.17 bagus SE --{--
	afx_msg void OnCheck();
	afx_msg void OnApply();
// 2009.09.17 bagus SE --}--
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// 2009.09.17 bagus SE --{--
private:
	void UpDateBasePos();

	STAGE_COORD_XYZ	m_BasePos;
// 2009.09.17 bagus SE --}--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ORIGINALPOSITIONSETTINGDLG_H__43170470_6CB8_4688_8DB2_0D24EC1F2B66__INCLUDED_)
