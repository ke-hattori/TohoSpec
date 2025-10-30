// NanoSpec.h : NANOSPEC アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_NANOSPEC_H__3ED5792D_CD32_4FF1_8BE2_910C169BFB06__INCLUDED_)
#define AFX_NANOSPEC_H__3ED5792D_CD32_4FF1_8BE2_910C169BFB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル
#include "System.h"

// 2009.11.24 K.Matsuo ヘッドオフセット対応＆マクロから関数化 -->
BOOL StageSelectLensNotify(UINT nLens);
BOOL SelectHeadTypeNotify(WORD wHeadType, BOOL bStageMove);
// 2009.11.24 K.Matsuo ヘッドオフセット対応＆マクロから関数化 <--
// 2013.02.22 bagus Substrate thickness setting -->
BOOL SelectSubThickNotifyIndex(int iIndex);
BOOL SelectSubThickNotify(double dThickness);
// 2013.02.22 bagus Substrate thickness setting <--

class CDebugDlg;

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecApp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

class CNanoSpecApp : public CWinApp
{
public:
	CNanoSpecApp();

//Operation
public:
	///// Variable //////////////////////////////////////////////////////////

	///// Function //////////////////////////////////////////////////////////
	void NextraExe(int Cmd);
	void NanoSpecTitle(int Cmd, int iParam = 0);
	CDebugDlg* m_pDebugDlg;

private:
/* added 2014.12.24 hmenjo 多重起動防止処理移動 ---------- { ---------- */
	HANDLE m_hMutex_Run;
/* added 2014.12.24 hmenjo 多重起動防止処理移動 ---------- } ---------- */
/* added 2014.11.22 hmenjo StartUpLogo 起動/停止 ---------- { ---------- */
	long StartUpLogo(long lCmd = 0);
/* added 2014.11.22 hmenjo StartUpLogo 起動/停止 ---------- } ---------- */
/* added 2010.11.16 hmenjo 起動時全 WVASE 終了 ---------- { ---------- */
	void ExitNanoProcesses(TCHAR (*ptszProcList)[][_MAX_FNAME]);
/* added 2010.11.16 hmenjo 起動時全 WVASE 終了 ---------- } ---------- */
/* added 2009.07.13 hmenjo 実行パス設定 ---------- { ---------- */
	int SetEnvNanoPath();
/* added 2009.07.13 hmenjo 実行パス設定 ---------- } ---------- */
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- { ---------- */
	DWORD m_dwProcIdNanoTitle;
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- } ---------- */
	void MakePath();
//	CView* m_pViews[MODE_NUM_MAX];
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoSpecApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CNanoSpecApp)
	afx_msg void OnAppAbout();
	afx_msg void OnDebug();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOSPEC_H__3ED5792D_CD32_4FF1_8BE2_910C169BFB06__INCLUDED_)
