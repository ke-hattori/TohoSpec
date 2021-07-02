#if !defined(AFX_MANUALLOADPOSITIONSETTINGDLG_H__527BD6D2_5178_47F2_BB28_0A047B28AE59__INCLUDED_)
#define AFX_MANUALLOADPOSITIONSETTINGDLG_H__527BD6D2_5178_47F2_BB28_0A047B28AE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"

// ManualLoadPositionSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CManualLoadPositionSettingDlg ダイアログ
// ##########################################################################

class CManualLoadPositionSettingDlg : public CDialog
{
// コンストラクション
public:
	CManualLoadPositionSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CManualLoadPositionSettingDlg)
	enum { IDD = IDD_MANUAL_LOAD_POSITION_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_ManualLoadPositionSetupButton;
	CColorStatic m_stcManualLoadPosX;
	CColorStatic m_stcManualLoadPosY;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CManualLoadPositionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CManualLoadPositionSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnManualLoadPositionSetupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MANUAL_LOAD_POSITION m_ManualLoadPosition;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MANUALLOADPOSITIONSETTINGDLG_H__527BD6D2_5178_47F2_BB28_0A047B28AE59__INCLUDED_)
