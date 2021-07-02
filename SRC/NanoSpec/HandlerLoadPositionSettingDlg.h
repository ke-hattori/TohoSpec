#if !defined(AFX_HANDLERLOADPOSITIONSETTINGDLG_H__7F065C25_8FA2_4F43_90B7_1CFD7FCADC89__INCLUDED_)
#define AFX_HANDLERLOADPOSITIONSETTINGDLG_H__7F065C25_8FA2_4F43_90B7_1CFD7FCADC89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorStatic.h"

// HandlerLoadPositionSettingDlg.h : ヘッダー ファイル
//

// ##########################################################################
// CHandlerLoadPositionSettingDlg ダイアログ
// ##########################################################################

class CHandlerLoadPositionSettingDlg : public CDialog
{
// コンストラクション
public:
	CHandlerLoadPositionSettingDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

protected:
// ダイアログ データ
	//{{AFX_DATA(CHandlerLoadPositionSettingDlg)
	enum { IDD = IDD_HANDLER_LOAD_POSITION_SETTING_DLG };
	//}}AFX_DATA
	CShadeButtonST m_OkButton;
	CShadeButtonST m_CancelButton;
	CShadeButtonST m_HandlerLoadPositionSetupButton;
	CColorStatic m_stcHandlerLoadPosX;
	CColorStatic m_stcHandlerLoadPosY;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHandlerLoadPositionSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//{{AFX_MSG(CHandlerLoadPositionSettingDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHandlerLoadPositionSetupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HANDLER_LOAD_POSITION m_HandlerLoadPosition;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HANDLERLOADPOSITIONSETTINGDLG_H__7F065C25_8FA2_4F43_90B7_1CFD7FCADC89__INCLUDED_)
