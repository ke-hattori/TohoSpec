#if !defined(AFX_GENERALPOSITIONSETTINGDLG_H__58DCDA12_3562_4CEC_ADD7_D1DA857C120E__INCLUDED_)
#define AFX_GENERALPOSITIONSETTINGDLG_H__58DCDA12_3562_4CEC_ADD7_D1DA857C120E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"

// GeneralPositionSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CGeneralPositionSettingDlg ダイアログ
// ##########################################################################

class CGeneralPositionSettingDlg : public CDialog
{
// コンストラクション
public:
	CGeneralPositionSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CGeneralPositionSettingDlg)
	enum { IDD = IDD_GENERAL_POSITION_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_GeneralPositionSetupButton;
	CColorStatic m_stcGeneralPosX;
	CColorStatic m_stcGeneralPosY;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CGeneralPositionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CGeneralPositionSettingDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGeneralPositionNo();
	afx_msg void OnGeneralPositionSetupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	GENERAL_POSITION m_TempGeneralPosition[GENERAL_POSITION_MAX];

private:
	void InitCombo_GeneralPosNo();

	int m_iCurrNo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_GENERALPOSITIONSETTINGDLG_H__58DCDA12_3562_4CEC_ADD7_D1DA857C120E__INCLUDED_)
