#if !defined(AFX_DEBUGDLG_H__4868D821_E9BC_47B3_BF35_C7C18C3F912F__INCLUDED_)
#define AFX_DEBUGDLG_H__4868D821_E9BC_47B3_BF35_C7C18C3F912F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDlg.h : ヘッダー ファイル
//
#ifdef REFRESH_TIMER_ID
	#undef REFRESH_TIMER_ID
#endif
#define REFRESH_TIMER_ID		(1)

class CMainFrame;
class CNanoSpecDoc;
class CView;

// ##########################################################################
// CDebugDlg ダイアログ
// これは削除すべきクラス
// ##########################################################################

class CDebugDlg : public CDialog
{
// コンストラクション
public:
	CDebugDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDebugDlg)
	enum { IDD = IDD_DEBUG_DLG };
	//}}AFX_DATA
	int m_iPoint;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDebugDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// Operation
	///// Variable //////////////////////////////////////////////////////////
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;
	CView* m_pView;

	///// Function //////////////////////////////////////////////////////////


// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDebugDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnSendMeasurementData();
	afx_msg void OnSendStartStates();
	afx_msg void OnSendErrorStates();
	afx_msg void OnSendManualMeasurement();
	afx_msg void OnDebAlarmSetButton();
	afx_msg void OnDebAlarmResetButton();
	afx_msg void OnDebGetPaneButton();
	afx_msg void OnSelchangeDebProcessStatusCombo();
	afx_msg void OnDebPifcommBtnP304();
	afx_msg void OnDebPifcommBtnP305();
	afx_msg void OnDebPifcommBtnP307();
	afx_msg void OnDebPifcommBtnP502();
	afx_msg void OnDebPifcommBtnP503();
	afx_msg void OnDebPifcommBtnP504();
	afx_msg void OnDebNexioMonitor();
	afx_msg void OnHibinoDebug1();
	afx_msg void OnDebManualStageButton2();
	afx_msg void OnDebMesSend();
	afx_msg void OnDebMesCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDebPatternLoad();
	afx_msg void OnDebPatternSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_loop;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DEBUGDLG_H__4868D821_E9BC_47B3_BF35_C7C18C3F912F__INCLUDED_)
