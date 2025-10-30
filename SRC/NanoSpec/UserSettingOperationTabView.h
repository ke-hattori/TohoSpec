#if !defined(AFX_USERSETTINGOPERATIONTABVIEW_H__F7F01A60_52E3_46E2_A318_DC18847627BE__INCLUDED_)
#define AFX_USERSETTINGOPERATIONTABVIEW_H__F7F01A60_52E3_46E2_A318_DC18847627BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserSettingOperationTabView.h : ヘッダー ファイル
//

class CUserSettingOperationCommonSettingDlg;
class CUserSettingOperationSrSettingDlg;
//2009.09.03 bagus se --{--
class CUserSettingOperationSeSettingDlg;
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
class CUserSettingOperationCompEASESettingDlg;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
class CUserSettingOperationCTASettingDlg;
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
class CUserSettingOperationMSSettingDlg;
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
class CUserSettingOperationStressSettingDlg;
//2009.10.21 bagus Stress 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
class CUserSettingOperationRSSettingDlg;
// 2009.11.04 bagus RS 追加 --}--

// ##########################################################################
// CUserSettingOperationTabView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CUserSettingOperationTabView : public CNanoUI
{
protected:
	CUserSettingOperationTabView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CUserSettingOperationTabView)

// フォーム データ
public:
	//{{AFX_DATA(CUserSettingOperationTabView)
	enum { IDD = IDD_USER_SETTING_OPERATION_TAB_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	int m_iHeadType[HEAD_TYPE_MAX + 1];
	CTabCtrl m_UserSettingOperationTab;
	CUserSettingOperationCommonSettingDlg* m_pUserSettingOperationCommonSettingDlg;
	CUserSettingOperationSrSettingDlg* m_pUserSettingOperationSrSettingDlg;
//2009.09.03 bagus se --{--
	CUserSettingOperationSeSettingDlg* m_pUserSettingOperationSeSettingDlg;
//2009.09.03 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	CUserSettingOperationCompEASESettingDlg* m_pUserSettingOperationCompEASESettingDlg;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.08 bagus CTA 追加 --{--
	CUserSettingOperationCTASettingDlg* m_pUserSettingOperationCTASettingDlg;
// 2009.10.08 bagus CTA 追加 --}--
//2009.10.20 bagus MS 追加 --{--
	CUserSettingOperationMSSettingDlg* m_pUserSettingOperationMSSettingDlg;
//2009.10.20 bagus MS 追加 --}--
//2009.10.21 bagus Stress 追加 --{--
	CUserSettingOperationStressSettingDlg* m_pUserSettingOperationStressSettingDlg;
//2009.10.21 bagus Stress 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	CUserSettingOperationRSSettingDlg* m_pUserSettingOperationRSSettingDlg;
// 2009.11.04 bagus RS 追加 --}--

	void Tab_Init();
	void TabChange(int Index);
	void ShowDialog(int iIndex);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUserSettingOperationTabView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CUserSettingOperationTabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUserSettingOperationTabView)
	afx_msg void OnSelchangeUserSettingOperationTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_USERSETTINGOPERATIONTABVIEW_H__F7F01A60_52E3_46E2_A318_DC18847627BE__INCLUDED_)
