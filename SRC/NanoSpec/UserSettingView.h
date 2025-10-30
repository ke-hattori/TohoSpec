#if !defined(AFX_USERSETTINGVIEW_H__7C405C3D_CD6B_4748_B993_F8506CA7D77C__INCLUDED_)
#define AFX_USERSETTINGVIEW_H__7C405C3D_CD6B_4748_B993_F8506CA7D77C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingView.h : ヘッダー ファイル
//

// ##########################################################################
// CUserSettingView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CUserSettingView : public CNanoUI
{
protected:
	CUserSettingView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CUserSettingView)

// フォーム データ
public:
	//{{AFX_DATA(CUserSettingView)
	enum { IDD = IDD_USER_SETTING_FORM };
	//}}AFX_DATA

// アトリビュート
public:
	CBrush m_Brush_BackColor;
	CBrush m_Brush_UserSettingBackGround;

// オペレーション
public:
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CUserSettingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGVIEW_H__7C405C3D_CD6B_4748_B993_F8506CA7D77C__INCLUDED_)
