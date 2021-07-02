#if !defined(AFX_SYSTEMOPERATIONTABVIEW_H__9D738A6D_2932_41EA_B9BD_9F397D101D33__INCLUDED_)
#define AFX_SYSTEMOPERATIONTABVIEW_H__9D738A6D_2932_41EA_B9BD_9F397D101D33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemOperationTabView.h : ヘッダー ファイル
//

class CSystemOperationCommonSettingDlg;
class CSystemOperationSrSettingDlg;
class CSystemOperationStressSettingDlg;
//2009.09.02 bagus se SE機能追加 --{--
class CSystemOperationSeSettingDlg;
//2009.09.02 bagus se SE機能追加 --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
class CSystemOperationCompEASESettingDlg;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.07 bagus CTA 追加 --{--
class CSystemOperationCTASettingDlg;
// 2009.10.07 bagus CTA 追加 --}--
// 2009.10.19 bagus MS 追加 --{--
class CSystemOperationMsSettingDlg;
// 2009.10.19 bagus MS 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
class CSystemOperationRSSettingDlg;
// 2009.11.04 bagus RS 追加 --}--


// ##########################################################################
// CSystemOperationTabView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

class CSystemOperationTabView : public CNanoUI
{
protected:
	CSystemOperationTabView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CSystemOperationTabView)

// フォーム データ
public:
	//{{AFX_DATA(CSystemOperationTabView)
	enum { IDD = IDD_SYSTEM_OPERATION_TAB_FORM };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	int m_iHeadType[HEAD_TYPE_MAX + 1];
	CTabCtrl m_SystemOperationTab;
	CSystemOperationCommonSettingDlg* m_pSystemOperationCommonSettingDlg;
	CSystemOperationSrSettingDlg* m_pSystemOperationSrSettingDlg;
	CSystemOperationStressSettingDlg* m_pSystemOperationStressSettingDlg;
	//2009.09.02 bagus se SE機能追加 --{--
	CSystemOperationSeSettingDlg* m_pSystemOperationSeSettingDlg;
	//2009.09.02 bagus se SE機能追加 --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	CSystemOperationCompEASESettingDlg* m_pSystemOperationCompEASESettingDlg;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	CSystemOperationCTASettingDlg* m_pSystemOperationCTASettingDlg;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.19 bagus MS 追加 --{--
	CSystemOperationMsSettingDlg* m_pSystemOperationMsSettingDlg;
	// 2009.10.19 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	CSystemOperationRSSettingDlg* m_pSystemOperationRSSettingDlg;
	// 2009.11.04 bagus RS 追加 --}--

	void Tab_Init();
	void TabChange(int Index);
	void ShowDialog(int iIndex);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSystemOperationTabView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CSystemOperationTabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSystemOperationTabView)
	afx_msg void OnSelchangeSystemOperationTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SYSTEMOPERATIONTABVIEW_H__9D738A6D_2932_41EA_B9BD_9F397D101D33__INCLUDED_)
