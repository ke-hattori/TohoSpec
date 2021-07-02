#if !defined(AFX_MEASUREMENTTABVIEW_H__A3A8BC63_850A_4C0A_A286_593E4F63680D__INCLUDED_)
#define AFX_MEASUREMENTTABVIEW_H__A3A8BC63_850A_4C0A_A286_593E4F63680D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementTabView.h : ヘッダー ファイル
//

class CMeasurementListDlg;
class CMeasurementGraphDlg;
class CMeasurementMapDlg;

// ##########################################################################
// CMeasurementTabView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"


#define SHOW_GRAPH_TIMER			(1)
#define SHOW_LIST_TIMER				(2)
#define TABVIEW_ENABLE_CHANGE_TIMER	(3)


class CMeasurementTabView : public CNanoUI
{
public:
//protected:
	CMeasurementTabView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CMeasurementTabView)

// フォーム データ
public:
	//{{AFX_DATA(CMeasurementTabView)
	enum { IDD = IDD_MEASUREMENT_TAB_FORM };
	//}}AFX_DATA

// アトリビュート
public:

// オペレーション
public:
	CToolBar m_wndMeasurementBar;
	CTabCtrl m_MeasurementTab;
	CMeasurementListDlg* m_pMeasurementListDlg;
	CMeasurementGraphDlg* m_pMeasurementGraphDlg;
	MAIN_RCP_INFO m_MainRcpInfo;
	MEAS_PROG_INFO m_MeasProgInfo;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	STAGE_COORD m_ScanPoint[SCAN_POINT_MAX];
	MEASUREMENT_DATA m_MeasurementData;
	BOOL m_bToolbarSaveCompFlg;
	BOOL m_bExitSaveCompFlg;
	int m_iProcessStatusOld;
	int m_iHostModeOld;
	int m_iManuMeasModeOld;

// 2014.01.17 Bagus Add (Stage None対応) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage None対応) <--

	void MeasurementListGrid_Init();
	char m_szName[RECIPE_NAME_LEN + 1];

	void Tab_Init();
	void TabChange(int Index);
	void LoadData();

	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

	int SaveMeasConAddRevi(LPCTSTR pszMainRcpName, int iSaveWay);
	void SaveMeasConRevi(LPCTSTR pszMainRcpName);
	void SaveMeasConRevi2(RECIPE_FILE* pDesc, int iDescCount, LPCTSTR pszMainRcpName);
	void SaveMeasConAdd(LPCTSTR pszMainRcpName);
	BOOL ZeroBungUp(char* czPoint, int iPoint);
	BOOL GetMeasSaveFileName(char* szSaveFilePath);
	int SaveOverlap(char* szSaveFullFilePath);
	BOOL FileOrDirExists(LPCTSTR pszFileName);
	char * GetFileName( const char *PathName );
	char * strtail( const char *string );
	int isJMS( const char *str, int nPos );
	void ButtonEnableChange(int Select);
	int FolderCopy( LPCTSTR szFrom, LPCTSTR szTo );
	int MakeDirectory(char* dir);
	//2009.08.28 bagus stress --{--
	//2009.08.28 bagus stress --}--
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
	BOOL MRCS_RDblClk(POINT ptGrid, CGridCtrl* pclsGrid = 0);
	WORD MRCS_SettingGet(long lIndex);
	WORD MRCS_SettingStatusGet(long lIndex);
	void MRCS_SettingStatusSet(long lIndex, WORD lStatus);
	DWORD MRCS_SettingAllGet(long lIndex);
	WORD MRCS_IncludeCountGet();
	WORD MRCS_IncludeAdd(long lIndex);
	WORD MRCS_ReMeasureNoGet();
	void MRCS_ReMeasureNoClr();
	void MRCS_IncludeColorSetNoSet(WORD wNo);
	WORD MRCS_IncludeColorSetNoGet();
	void MRCS_IncludeColorSet(WORD wNo, BOOL bList, BOOL bGraph);
	BOOL MRCS_ShowCompletionPointSnap(LPWORD pwList = 0, LPWORD pwGraph = 0);
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMeasurementTabView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
//protected:
public:
	virtual ~CMeasurementTabView();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMeasurementTabView)
	afx_msg void OnSelchangeMeasurementTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSaveResultData(CCmdUI* pCmdUI);
	afx_msg void OnSaveResultData();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSaveAs();
	afx_msg void OnUpdateSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT SetMeasurementData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT MeasurementStart(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT MeasurementPointError(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT ManualMeasurementStart(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
private:
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- { ---------- */
	void OnSaveAsMain(int iSaveWay);
/* added 2009.06.16 hmenjo 使用禁止文字チェック ---------- } ---------- */
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- { ---------- */
	void MRCS_Initialize();
	WORD m_MRCS_wShowCompletionPointSnapL;
	WORD m_MRCS_wShowCompletionPointSnapG;
	WORD m_MRCS_wReMeasureNo;	/* 再測定 No.	*/
	WORD m_MRCS_wIncludeColorSetNo;	/* 指定行色番号	*/
	BOOL m_MRCS_bIncludeColorSetList;
	BOOL m_MRCS_bIncludeColorSetGraph;
	BOOL MRCS_CheckPermission(DWORD dwMode = 0);
	BOOL MRCS_CheckRunPermission();
	DWORD m_MRCS_dwSettings[SCAN_POINT_MAX + 1];	/* 設定バッファ	*/
	void MRCS_SettingSet(long lIndex, WORD wSetting);
	void MRCS_GridLineColorSet(long lIndex, COLORREF cr = CLR_DEFAULT);
	WORD MRCS_DeleteCountGet();
	WORD MRCS_DeleteCountInc(WORD wCount = 1);
	WORD MRCS_DeleteCountDec(WORD wCount = 1);
	WORD MRCS_IncludeCountInc(WORD wCount = 1);
	WORD MRCS_IncludeCountDec(WORD wCount = 1);
/* added 2016.05.05 hmenjo 手動 再測定/指定統計 ---------- } ---------- */
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEASUREMENTTABVIEW_H__A3A8BC63_850A_4C0A_A286_593E4F63680D__INCLUDED_)
