#if !defined(AFX_SRREFERENCEPOSITIONSETTINGDLG_H__D0AF6CD9_66B7_4FB2_85E5_E9F84FD74DFA__INCLUDED_)
#define AFX_SRREFERENCEPOSITIONSETTINGDLG_H__D0AF6CD9_66B7_4FB2_85E5_E9F84FD74DFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrReferencePositionSettingDlg.h : ヘッダー ファイル
//

#include "ColorStatic.h"

// ##########################################################################
// CSrReferencePositionSettingDlg ダイアログ
// ##########################################################################

class CSrReferencePositionSettingDlg : public CDialog
{
// コンストラクション
public:
	CSrReferencePositionSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSrReferencePositionSettingDlg)
	enum { IDD = IDD_SR_REFERENCE_POSITION_SETTING_DLG };
	//}}AFX_DATA

//Operation
public:
	CColorStatic m_stcDarkRefPos_X, m_stcDarkRefPos_Y, m_stcDarkRefPos_Z;
	CColorStatic m_stcReflectRefPos_X, m_stcReflectRefPos_Y, m_stcReflectRefPos_Z;
	CColorStatic m_stcTransmitAbsoluteRefPos_X, m_stcTransmitAbsoluteRefPos_Y, m_stcTransmitAbsoluteRefPos_Z;
	CColorStatic m_stcTransmitFirstRefPos_X, m_stcTransmitFirstRefPos_Y, m_stcTransmitFirstRefPos_Z;
	CColorStatic m_stcTransmitSecondRefPos_X, m_stcTransmitSecondRefPos_Y, m_stcTransmitSecondRefPos_Z;
	CShadeButtonST m_DarkReferenceSetupButton;
	CShadeButtonST m_ReflectReferenceSetupButton;
	CShadeButtonST m_TransmitAbsoluteReferenceSetupButton;
	CShadeButtonST m_TransmitFirstReferenceSetupButton;
	CShadeButtonST m_TransmitSecondReferenceSetupButton;
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
//2009.09.28 bagus gantry --{--
	CColorStatic m_stcGantryTransmitRefPos_X, m_stcGantryTransmitRefPos_Y, m_stcGantryTransmitRefPos_Z;
	CShadeButtonST m_GantryTransmitReferenceSetupButton;
//2009.09.28 bagus gantry --}--

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrReferencePositionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSrReferencePositionSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDarkReferenceSetupButton();
	afx_msg void OnReflectReferenceSetupButton();
	afx_msg void OnTransmitAbsoluteReferenceSetupButton();
	afx_msg void OnTransmitFirstReferenceSetupButton();
	afx_msg void OnTransmitSecondReferenceSetupButton();
//2009.09.28 bagus gantry --{--
	afx_msg void OnSelchangePositionNo();
	afx_msg void OnGantryTransmitReferenceSetupButton();
//2009.09.28 bagus gantry --}--
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
//2009.09.28 bagus gantry --{--
	SR_REFERENCE_POSITION m_SrReferencePosition;

	void ShowLocDarkReference();
	void ShowLocReflectReference();
	void ShowLocTransmitAbsoluteReference();
	void ShowLocTransmitFirstReference();
	void ShowLocTransmitSecondReference();
	void ShowLocGantryTransmitReference(int nIndex);
//2009.09.28 bagus gantry --}--

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRREFERENCEPOSITIONSETTINGDLG_H__D0AF6CD9_66B7_4FB2_85E5_E9F84FD74DFA__INCLUDED_)
