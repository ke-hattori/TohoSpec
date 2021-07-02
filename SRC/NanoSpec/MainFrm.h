/////////////////////////////////////////////////////////////////////////////
// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします //
//																		   //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__59E4B529_2C83_4278_BA0F_4B0C5A99BCE2__INCLUDED_)
#define AFX_MAINFRM_H__59E4B529_2C83_4278_BA0F_4B0C5A99BCE2__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "active.hxx"
#include "mysplitterwnd.h"
#include "MainFrmMailRecvThread.h"
#include "SharedMemory.h"
#include "ChiefExports.h"
#include "LensComboBox.h"

class CReferenceExecutionDlg;


class CMeasurementTabView; //#include "MeasurementTabView.h"
class CMeasurementOperationView; //#include "MeasurementOperationView.h"
class CManualMeasurementFormView; //#include "ManualMeasurementFormView.h"
class CTestMeasurementDlg; //#include "TestMeasurementDlg.h"
// 2010.01.06 bagus Gantry --{--
class CTestMeasurementGantryDlg; //#include "TestMeasurementGantryDlg.h"
// 2010.01.06 bagus Gantry --}--
class CMaintenanceNoticeDlg; //#include "MaintenanceNoticeDlg.h"


/////////////////////////////////////////////////////////////////////////////
// Define																   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
#define MAIN_FRAME_LABEL_MAX		(200)
#define MAIN_FRAME_BUTTON_MAX		(200)

/////////////////////////////////////////////////////////////////////////////
// CMainFrame															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CFrameWnd , public cActive
{

protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	virtual ~CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// アトリビュート
public:
// オペレーション
public:
	///// Variable //////////////////////////////////////////////////////////
	SYSTEM_CONFIG m_SystemConfig;
// 2009.05.15 k-matsuo -->
	SR_CONFIG m_srConfig;
// 2009.05.15 k-matsuo <--

	CNanoSpecApp* m_pApp;
	CNanoSpecDoc* m_pDoc;

	CWinThread* m_pcChiefThread;	// Chief スレッドクラスポインタ

	int m_View2ID;
	int m_View1ID;
	CView* m_pView1;
	CMySplitterWnd m_wndSplitter;
	CMySplitterWnd m_wndSplitter1;
	CMySplitterWnd m_wndSplitter2;
	CStatusBarEx m_wndStatusBar;

	BOOL m_Disp[MAIN_FRAME_LABEL_MAX];
	BOOL m_Button[MAIN_FRAME_BUTTON_MAX];

	CToolBar m_wndToolBar;
	CToolBar m_wndNanoSpecBar;
	CView* m_pCurrExeView;
	int m_iCurrMode;
	int m_iCurrExeWnd;

	int m_iSplit_OpePanelHeight;
	int m_iSelectHeadType, m_iSelectMeasType;
	CMenu m_NewMenu;
	BOOL m_bNew;
	BOOL m_bDefaultSetting;
	char m_szSelectListName[50][RECIPE_NAME_LEN + 1];
	char m_szSampleTypeName[RECIPE_NAME_LEN + 1];
	BOOL m_bTransmittance;
	BOOL m_bAutoFocusInProgress;
	CString m_strEqStatus;
	CAlarm* m_pAlarm;
	int m_iNowPoint;
	int m_iNowStressLine;

	long m_DbgCount;			// for debug

	///// Status Bar Monitor /////
	BOOL m_bVacuumSensor;
	BOOL m_bVacuumValve;
	BOOL m_bSampleSensor;
	BOOL m_bStageLoadPos;
	BOOL m_Old_bVacuumSensor;
	BOOL m_Old_bVacuumValve;
	BOOL m_Old_bSampleSensor;
	BOOL m_Old_bStageLoadPos;

	BOOL m_bInitRetry;

	CMeasurementTabView* m_pMeaTabViewObj; //CMeasurementTabViewのオブジェクトのポインタ格納用
	CMeasurementOperationView* m_pMeaOpeViewObj; //CMeasurementOperationViewのオブジェクトのポインタ格納用
	CManualMeasurementFormView* m_pManuMeaFormViewObj; //CManualMeasurementFormViewのオブジェクトのポインタ格納用
	CTestMeasurementDlg* m_pTestMeasDlgObj; //CTestMeasurementDlgのオブジェクトのポインタ格納用
// 2010.01.06 bagus Gantry --{--
	CTestMeasurementGantryDlg* m_pTestMeasGantryDlgObj; //CTestMeasurementGantryDlgのオブジェクトのポインタ格納用
// 2010.01.06 bagus Gantry --}--
	BOOL m_MeasMenuEnableFlg; //主に、測定中に制限が必要なもの用
	BOOL m_MeasMenuEnableFlg2; //主に、測定画面に滞在中に制限が必要なもの用(測定, レシピ設定, ユーザ設定, メンテナンス用)
	BOOL m_ManualMeasMenuEnableFlg;
	HANDLE m_hEvTabView;

	CMaintenanceNoticeDlg* m_pMntNoticeDlg;
	BOOL m_bOrgSystemColorChangeExecFlg;

	int	m_nMotionStage;			// XYの設定値
	int m_nMotionElev;			// Zの設定値
	int m_nCameraSelect;		// カメラ選択
	int m_nJoyStickMode;		// ジョイスティック選択

	///// Function //////////////////////////////////////////////////////////
	void ProcessQuit();
	BOOL HardwareSimulation();
	void ChangeHostMode(int iMode);
	void ChangeVacuumStatus();
	void ChangeEqStatus();
	void ChangeOperationPanel(int iMode, int iEditMode = 0 /* EDIT_MAIN_RECIPE */);
	void ChangeExeWnd(int iExeWndMode);
	BOOL CheckSelectMainRecipe();
	void ChangeMainMenuStatus();
	void GetSelectListName(LPTSTR pszName);
	void SetSelectListName(LPCTSTR pszName);
	void GetSelectSampleTypeName(LPTSTR pszName);
	void SetSelectSampleTypeName(LPCTSTR pszName);
	int GetSelectHeadType();
	void SetSelectHeadType(int iHeadType);
	int GetSelectMeasurementType();
	void SetSelectMeasurementType(int iMeasType);
	BOOL GetMethodOfMeasurement();
	BOOL MenuExit();
	int GetCurrOperationPanel();
	int GetCurrExeWnd();
	CView* GetCurrentExeView();
	BOOL GetOrginalSystemColor();
	BOOL SetOrginalSystemColor();
	void PutStatusBarUpdate(UINT nIDFind, LPCTSTR str);

	void RefreshDlg(BOOL bInit = FALSE);
	void GlassDataStatistics(STATISTICS* Statistics, int iLabelCount, UINT nFinalFocusFailDataValidOption/*, AF_VALID afValid*/);
	//2009.11.04 bagus CA,RS --{--
	//ADAP関連とそうでないものを共通して取れるようにする。
	int ScanDataLabelGet(WORD wHeadType,char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid);
	int ScanDataLabelGet_ADAP(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid);
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	int ScanDataLabelGet_COMPEASE(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid);
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	int ScanDataLabelGet_CA(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid);
	int ScanDataLabelGet_RS(char szLabel[][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[][ADAPRESULTSTRINGLENMAX + 1], double dData[ADAPRESULT_COLS_MAX], int iNowPoint, AF_VALID afValid);
	//2009.11.04 bagus CA,RS --}--
	void CMainFrame::MeasPointDatFileTempCopy(int iNowPoint);
	int isJMS( const char *str, int nPos );
	void AfValidResultJudge(AF_VALID afValid, char* szShowAfValid, char* szAfP502);
	BOOL JudgeChiefResult(int Value, LPCTSTR lpszCaption);

	BOOL FileOrDirExists(LPCTSTR pszFileName);
	BOOL DeleteDirectory(LPCTSTR lpPathName);
	void GetStrategyhead(const char* szStrategyEntry, char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], int *iDispLabelCount);
	int MeasEnd(); //
	//2009.08.27 bagus stress --{--
	BOOL OutputStressData(LPCTSTR lpszOutputPath,LPCTSTR lpszRecipeName,BOOL bRef);
	BOOL OutputStressRawData(LPCTSTR lpszOutputPath,LPCTSTR lpszRecipeName,BOOL bRef);
	BOOL OutputStressLSRawData(LPCTSTR lpszOutputPath,LPCTSTR lpszRecipeName,int nLine,int nSection,BOOL bRef);
	//2009.08.27 bagus stress --{--
	//2009.08.31 bagus stress --{--
	void CalcStressLineStatitcs(STRESSRESULT *pResult,STATISTICS *pStat,BOOL *pScanValid);
	void CalcStressStatistics(STATISTICS *pStat,RCP_DATA *pRcp);
	//2009.08.31 bagus stress --}--
	//2009.10.28 bagus 2point-distance --{--
	double Calc2PointDistance(long lStartX,long lStartY,long lEndX,long lEndY);
	void DistanceDataStatistics(STATISTICS* Statistics);
	//2009.10.28 bagus 2point-distance --}--

	void TestModeTempRcpDelete();
	void MaintenanceDlgOpen();
	void ChangeMaintenanceModeColor(int iMode);
	void OldMeasDataDelete();
	BOOL GetDeleteDate(SYSTEMTIME *stDeleteBaseTime);
	void DeleteOldFile(const SYSTEMTIME stDeleteBaseTime);
	void WatchDoc();
	//Saiki 20090411 Add ----->
	static BOOL CALLBACK QuitExternalApp(HWND hWnd, LPARAM lParam);
	void MessageShow(CString strBuffer);
	//Saiki 20090411 Add <-----
	void DoEvents(void);
/* added 2014.11.22 hmenjo TohoRecall へ変更 ---------- { ---------- */
	void OnMenuDataFromExt();
/* added 2014.11.22 hmenjo TohoRecall へ変更 ---------- } ---------- */

	//2009.09.15 baugs se --{--
	//SE生データファイルの取得
	void SplitString(CString *pStrData,CStringArray *pStrArray,TCHAR chSplitter,BOOL bTrim = FALSE);
	BOOL LoadSERawData(LPCTSTR lpszFilePath,CSERawDataList *pList,BOOL bRawData);
	BOOL LoadSERawDataByPointNo(int nPointNo,CSERawDataList *pList);
	//2009.09.15 baugs se --}--
	//2009.11.04 bagus CA --{--
	BOOL GetCTAMeasureResult(CTARESULT *pResult);
	//2009.11.04 bagus CA --{--
	//2009.11.10 bagus RS --{--
	BOOL GetResistMeasureResult(RESISTRESULT *pResult);
	//2009.11.10 bagus RS --}--

	///// General ///////////////////////////////////////////////////////////
	inline BOOL CTRL_DISP(int No, int Data, int Init, const LPSTR Disp[2], UINT Id)
	{
		BOOL	bRet = FALSE;
		if(m_Disp[No] != Data || Init){
			CWnd* pWnd;
			pWnd = GetDlgItem(Id);
			if(pWnd){
				pWnd->SetWindowText(Disp[Data]);
				pWnd->RedrawWindow();
				m_Disp[No] = Data;
				bRet = TRUE;
			}
		}
		return bRet;
	}

	inline void ENABLE_BUTTON(int No, int Flag, int Init, UINT Id){
		if(m_Button[No] != Flag || Init){
			CWnd* pWnd;
			pWnd = GetDlgItem(Id);
			if(pWnd){
				pWnd->EnableWindow(Flag);
				m_Button[No] = Flag;
			}
		}
	}

	// XYステージ速度選択
	inline int GetMotionStage() { return m_nMotionStage; }
	inline void SetMotionStage(int nSelect){ m_nMotionStage = nSelect; }
	// Z軸速度選択
	inline int GetMotionElev() { return m_nMotionElev; }
	inline void SetMotionElev(int nSelect){ m_nMotionElev = nSelect; }
	// カメラ選択
	inline int GetCameraSelect() { return m_nCameraSelect; }
	inline void SetCameraSelect(int nSelect){ m_nCameraSelect = nSelect; }
	// ジョイスティック選択
	inline int GetJoyStickMode() { return m_nJoyStickMode; }
	inline void SetJoyStickMode(int nMode){ m_nJoyStickMode = nMode; }

// Test --->
	void EndSrMeasurement(BOOL bResult);
	void EndSrFocus(BOOL bResult);
// Test <---

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// 生成されたメッセージ マップ関数
protected:

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMeasurementStart();
	afx_msg void OnMenuMeasurement();
	afx_msg void OnMenuEditRecipe();
	afx_msg void OnMenuData();
	afx_msg void OnMenuUserSetting();
	afx_msg void OnMenuMaintenance();
	afx_msg void OnMenuSystem();
	afx_msg void OnMenuLocal();
	afx_msg void OnMenuRemote();
	afx_msg void OnMenuExit();
	afx_msg void OnUpdateMenuSave(CCmdUI* pCmdUI);
	afx_msg void OnAutoFocus();
	afx_msg void OnClose();
	afx_msg void OnAppExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateMenuUserSetting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuMaintenance(CCmdUI* pCmdUI);
	afx_msg void OnMenuLogon();
	afx_msg void OnMenuAlarm();
	afx_msg void OnUpdateMenuData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystem(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnUpdateRemote(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLocal(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnUpdateMeasurement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRecipeSetting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLogon(CCmdUI* pCmdUI);
	afx_msg void OnSysColorChange();
	afx_msg void OnUpdateMeasurementStart(CCmdUI* pCmdUI);
	afx_msg void OnHandlerLoadPosition();
	afx_msg void OnUpdateHandlerLoadPosition(CCmdUI* pCmdUI);
	afx_msg void OnManualStage();
	afx_msg void OnUpdateManualStage(CCmdUI* pCmdUI);
	afx_msg void OnVacuum();
	afx_msg void OnUpdateVacuum(CCmdUI* pCmdUI);
	afx_msg void OnReference();
	afx_msg void OnUpdateReference(CCmdUI* pCmdUI);
	afx_msg void OnMeasurementCancel();
	afx_msg void OnUpdateMeasurementCancel(CCmdUI* pCmdUI);
	afx_msg void OnMeasurementPause();
	afx_msg void OnUpdateMeasurementPause(CCmdUI* pCmdUI);
	afx_msg void OnRemeasurement();
	afx_msg void OnUpdateRemeasurement(CCmdUI* pCmdUI);
	afx_msg void OnManualMeasurementStart();
	afx_msg void OnUpdateManualMeasurementStart(CCmdUI* pCmdUI);
	afx_msg void OnMeasurementComplete();
	afx_msg void OnUpdateMeasurementComplete(CCmdUI* pCmdUI);
	afx_msg void OnPointTeaching();
	afx_msg void OnUpdatePointTeaching(CCmdUI* pCmdUI);
	afx_msg void OnMeasurementResume();
	afx_msg void OnUpdateMeasurementResume(CCmdUI* pCmdUI);
	afx_msg void OnManualMeasurement();
	afx_msg void OnUpdateManualMeasurement(CCmdUI* pCmdUI);
	afx_msg void OnSeqmeasExit();
	afx_msg void OnUpdateSeqmeasExit(CCmdUI* pCmdUI);
	afx_msg void OnManumeasExit();
	afx_msg void OnUpdateManumeasExit(CCmdUI* pCmdUI);
	afx_msg void OnAdujstRemeasurement();
	afx_msg void OnUpdateAdujstRemeasurement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
/* added 2016.05.14 hmenjo 手動 menu バグ ---------- { ---------- */
	afx_msg void OnSaveAs();
	afx_msg void OnUpdateSaveAs(CCmdUI* pCmdUI);
/* added 2016.05.14 hmenjo 手動 menu バグ ---------- } ---------- */
/* added 2016.08.05 hmenjo レシピ編集 menu バグ ---------- { ---------- */
	afx_msg void OnNew();
	afx_msg void OnUpdateNew(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
/* added 2016.08.05 hmenjo レシピ編集 menu バグ ---------- } ---------- */
/* added 2016.08.12 hmenjo レシピ編集 menu バグ2 ---------- { ---------- */
	afx_msg void OnRecipeSave();
	afx_msg void OnRecipeSaveAs();
	afx_msg void OnUpdateRecipeSaveAs(CCmdUI* pCmdUI);
/* added 2016.08.12 hmenjo レシピ編集 menu バグ2 ---------- } ---------- */
	//}}AFX_MSG

	afx_msg LRESULT OnExeWndExit(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDoMeasure(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDoCancel(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDoPause(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDoResume(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnAlarmIf_Set(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnAlarmIf_Reset(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetMeasurementData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnScanDataStockProc(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqStart(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnOneSheetEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMoveSetposEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnVacuumEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnAfPopup(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSrrefEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnAafEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnManualDeskewDlgPopup(WPARAM wparam, LPARAM lparam);
// 2009.05.16 myanagida -->
	afx_msg LRESULT OnManualSitePtDlgPopup(WPARAM wparam, LPARAM lparam);
// 2009.05.16 myanagida <--
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	afx_msg LRESULT OnNotifyMeasPause(WPARAM wparam, LPARAM lparam);
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */

	afx_msg LRESULT OnProcessInit(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnUnitInitRetry(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnExeDlgDisplay(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnMessageShow(WPARAM wparam, LPARAM lparam);

	afx_msg LRESULT OnDebMessageRecieve(WPARAM wparam, LPARAM lparam);

	//2009.07.27 bagus stress ストレス対応 --{--
	afx_msg LRESULT OnLineEnd(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnLineStart(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnStrsMeasEnd(WPARAM wparam,LPARAM lparam);
	//2009.07.27 bagus stress ストレス対応 --}--
	//2009.10.28 bagus 2point-distance --{--
	afx_msg LRESULT OnDistancePopup(WPARAM wparam, LPARAM lparam);
	//2009.10.28 bagus 2point-distance --}--
	//2009.11.03 bagus MS --{--
	afx_msg LRESULT OnMSPopup(WPARAM wparam, LPARAM lparam);
	//2009.11.03 bagus MS --}--
	// 2013.11.07 Bagus Add (TohoSpec対応) -->
	afx_msg LRESULT OnConfirmPopup(WPARAM wparam, LPARAM lparam);
	// 2013.11.07 Bagus Add (TohoSpec対応) <--
	// bagus 2014.01.10 Add(Stage None対応) -->
	afx_msg LRESULT OnCompleteSiReferenceDoneReport(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnCompleteDarkReferenceDoneReport(WPARAM wparam, LPARAM lparam);
	// bagus 2014.01.10 Add(Stage None対応) <--

	DECLARE_MESSAGE_MAP()

protected:
	void enter_ST_IDLE(const cEventParams* const);
	void enter_ST_INIT(const cEventParams* const);
	void enter_ST_SHUTDOWN(const cEventParams* const);
	void enter_ST_DONE(const cEventParams* const);
	void enter_ST_ABORT(const cEventParams* const);
	void enter_ST_RETRY(const cEventParams* const);
	DECLARE_STATEPROC_MAP(CMainFrame);

private:
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	BOOL m_bClosedPauseMsgBox;
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
	void ChangeMenu();
	void SetWindowTitle();
	int SelectListNameIndex();

	double m_dHWStageSizeX;
	double m_dHWStageSizeY;
	char m_szMainRecipeName[RECIPE_NAME_LEN + 1];
	MAIN_RCP_INFO m_MainRcpInfo;
	STAGE_PROG_INFO_HDR m_StageProgInfoHdr;
	CMainFrmMailRecvThread* m_pMainFrmMailRecvThread;
	COLORREF m_OrgSystemColor[50];
	BOOL m_bGetColorInf;
	CLensComboBox m_cbLens;

	///// Maintenance Date /////
	void MaintenanceCheck();
	MAINTENANCE_DATE m_pMaintenanceDate;
	MAINTENANCE_LAMP m_pMaintenanceLamp;
	COleDateTime mNextDate;					// 次回メンテナンス日時
	COleDateTimeSpan m_pPassageHour;		// 経過時間
	COleDateTimeSpan m_100ms;

// 2009.05.15 k-matsuo -->
	int m_iMaxWaveLength;
// 2009.05.15 k-matsuo <--

// 2009.11.25 K.Matsuo GTr Reference -->
	BOOL m_bGTRref;
	int m_iGTRrefNowPointNo;
// 2009.11.25 K.Matsuo GTr Reference <--
/* deleted 2010.10.19 hmenjo DioLog.exe 起動しなくする ---------- { ---------- */
//    PROCESS_INFORMATION m_prInfo;
/* deleted 2010.10.19 hmenjo DioLog.exe 起動しなくする ---------- } ---------- */
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__59E4B529_2C83_4278_BA0F_4B0C5A99BCE2__INCLUDED_)
