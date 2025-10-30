#if !defined(AFX_MEASUREMENTTABVIEW_H__A3A8BC63_850A_4C0A_A286_593E4F63680D__INCLUDED_)
#define AFX_MEASUREMENTTABVIEW_H__A3A8BC63_850A_4C0A_A286_593E4F63680D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeasurementTabView.h : ï¿½wï¿½bï¿½_ï¿½[ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

class CMeasurementListDlg;
class CMeasurementGraphDlg;
class CMeasurementMapDlg;

// ##########################################################################
// CMeasurementTabView ï¿½tï¿½Hï¿½[ï¿½ï¿½ ï¿½rï¿½ï¿½ï¿½[
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
	CMeasurementTabView();			// ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ÉŽgï¿½pï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½eï¿½Nï¿½g ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^ï¿½B
	DECLARE_DYNCREATE(CMeasurementTabView)

// ï¿½tï¿½Hï¿½[ï¿½ï¿½ ï¿½fï¿½[ï¿½^
public:
	//{{AFX_DATA(CMeasurementTabView)
	enum { IDD = IDD_MEASUREMENT_TAB_FORM };
	//}}AFX_DATA

// ï¿½Aï¿½gï¿½ï¿½ï¿½rï¿½ï¿½ï¿½[ï¿½g
public:

// ï¿½Iï¿½yï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
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

// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--

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
	const char * strtail( const char *string );
	int isJMS( const char *str, int nPos );
	void ButtonEnableChange(int Select);
	int FolderCopy( LPCTSTR szFrom, LPCTSTR szTo );
	int MakeDirectory(char* dir);
	//2009.08.28 bagus stress --{--
	//2009.08.28 bagus stress --}--
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
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
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */


// ï¿½Iï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½h
	// ClassWizard ï¿½Í‰ï¿½ï¿½zï¿½Ö�ï¿½ï¿½ÌƒIï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½hï¿½ð�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//{{AFX_VIRTUAL(CMeasurementTabView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ï¿½Tï¿½|ï¿½[ï¿½g
	//}}AFX_VIRTUAL

// ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
//protected:
public:
	virtual ~CMeasurementTabView();

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½}ï¿½bï¿½vï¿½Ö�ï¿½
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
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½ÖŽ~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- { ---------- */
	void OnSaveAsMain(int iSaveWay);
/* added 2009.06.16 hmenjo ï¿½gï¿½pï¿½ÖŽ~ï¿½ï¿½ï¿½ï¿½ï¿½`ï¿½Fï¿½bï¿½N ---------- } ---------- */
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- { ---------- */
	void MRCS_Initialize();
	WORD m_MRCS_wShowCompletionPointSnapL;
	WORD m_MRCS_wShowCompletionPointSnapG;
	WORD m_MRCS_wReMeasureNo;	/* ï¿½Ä‘ï¿½ï¿½ï¿½ No.	*/
	WORD m_MRCS_wIncludeColorSetNo;	/* ï¿½wï¿½ï¿½sï¿½Fï¿½Ô�ï¿½	*/
	BOOL m_MRCS_bIncludeColorSetList;
	BOOL m_MRCS_bIncludeColorSetGraph;
	BOOL MRCS_CheckPermission(DWORD dwMode = 0);
	BOOL MRCS_CheckRunPermission();
	DWORD m_MRCS_dwSettings[SCAN_POINT_MAX + 1];	/* ï¿½Ý’ï¿½oï¿½bï¿½tï¿½@	*/
	void MRCS_SettingSet(long lIndex, WORD wSetting);
	void MRCS_GridLineColorSet(long lIndex, COLORREF cr = CLR_DEFAULT);
	WORD MRCS_DeleteCountGet();
	WORD MRCS_DeleteCountInc(WORD wCount = 1);
	WORD MRCS_DeleteCountDec(WORD wCount = 1);
	WORD MRCS_IncludeCountInc(WORD wCount = 1);
	WORD MRCS_IncludeCountDec(WORD wCount = 1);
/* added 2016.05.05 hmenjo ï¿½è“® ï¿½Ä‘ï¿½ï¿½ï¿½/ï¿½wï¿½è“�ï¿½v ---------- } ---------- */
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ ï¿½Í‘Oï¿½sï¿½Ì’ï¿½ï¿½Oï¿½É’Ç‰ï¿½ï¿½Ì�éŒ¾ï¿½ï¿½}ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B

#endif // !defined(AFX_MEASUREMENTTABVIEW_H__A3A8BC63_850A_4C0A_A286_593E4F63680D__INCLUDED_)
