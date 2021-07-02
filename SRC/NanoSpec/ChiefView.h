// ChiefView.h : ヘッダー ファイル
//

#pragma once

#define CHIEF_REMEASURE_ON
#define CHIEF_STAGE_ON

/*
 *	構造体定義
 */
// Chief ダイアログの表示方法(設定ファイルから読込み)
typedef struct {
	DWORD	dwType; 	// ダイアログ表示方法
	DWORD	dwDelay;	// ダイアログ表示ディレイ [s]
	DWORD	dwShowSWprc;
} CHIEF_DLGSHOWSW;
// DI 割付
typedef struct {
	BOOL	bEMOStop;			// 非常停止
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
	BOOL	bSafetyPlug;		/* ドア セーフティプラグ	*/
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
	BOOL	bDoorInterlock; 	// ドア インタロック
	BOOL	bEQPower;			// 装置パワーオン
//	BOOL	bAlignmentPowerOn;	// アライメントパワーオン	これは削除されました(2008.10.27)．
	BOOL	bMaintenanceSW; 	// メンテナンス SW オン
	BOOL	bTHMaintenanceSW;	// 東朋メンテナンス SW オン
	BOOL	bAirPressureLow;	// エア圧力低下
	BOOL	bGlassExist;		// 基板在荷
	BOOL	bLoadPos;			// ロードポジション
	BOOL	bShutterOpen;		// シャッタ 開
	BOOL	bShutterClose;		// シャッタ 閉
	BOOL	bRobotArm;			// ロボットアーム検出
	BOOL	bVacuumPressure1;	// バキュームセンサ1
//	BOOL	bVacuumPressure2;	// バキュームセンサ2	これは削除されました(2008.10.27)．
	BOOL	bPinUp; 			// ピン アップ
	BOOL	bPinDown;			// ピン ダウン
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- { ---------- */
	BOOL	bPinAlign;			// ピン アライメント
/* added 2009.08.20 hmenjo ストレス機能追加(46) ---------- } ---------- */
	BOOL	bWorkGuideOpen; 	// ワークガイド 開
	BOOL	bWorkGuideClose;	// ワークガイド 閉
// 2013.01.10 bagus stage driver alarm io -->
	BOOL	bStageAlarm;		// ステージアラーム
	BOOL	bX1AxisAlarm[3];	// Stage Driver X1軸アラーム
	BOOL	bX2AxisAlarm[3];	// Stage Driver X2軸アラーム
	BOOL	bYAxisAlarm[3];		// Stage Driver Y軸アラーム
// 2013.01.10 bagus stage driver alarm io <--
} CHIEF_DI_INFO;
// レシピ情報
typedef struct {
	LPVOID	pMainRcpInfo;
	LPVOID	pMeasProgInfo;
	LPVOID	pStageProgInfoHdr;
	LPVOID	pScanPoint;
// 2009.05.17 myanagida -->
	LPVOID	pSitePattern;
// 2009.05.17 myanagida <--
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- { ---------- */
	LPVOID	pStageProgStress;
/* added 2009.08.03 hmenjo ストレス機能追加(13) ---------- } ---------- */
//Saiki 20110308 Add ----->
	LPVOID	pStressParamInfo;
//Saiki 20110308 Add <-----
} CHIEF_RECIPES;
// 入力無効スイッチ
typedef struct {
	BOOL	bRobotArm;
	BOOL	bPinDown;
	BOOL	bAirPressureLow;
/* added 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- { ---------- */
	BOOL	bPinShutterILInvalid;
/* added 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- } ---------- */
} DIO_IGNORE_SW;
// アラーム記憶フラグ
typedef struct {
	BOOL	bEMOStopOn;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
	BOOL	bSafetyPlugOn;
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
	BOOL	bDoorInterlockOn;
	BOOL	bEQPowerOn;
	BOOL	bShutterCloseOff;
	BOOL	bRobotArmOn;
	BOOL	bMaintenanceSWOn;
	BOOL	bAirPressureLowOn;
	BOOL	bVacuumPressure1Off;
	BOOL	bPinDownOff;
	BOOL	bWorkGuideOpenOff;
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- { ---------- */
	BOOL	bRetractZPosOff;
/* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- } ---------- */
// 2013.01.10 bagus stage driver alarm io -->
	BOOL	bStageAlarmOn;
	BOOL	bStageOverTravelOn;
	BOOL	bStageUnknownOn;
// 2013.01.10 bagus stage driver alarm io <--
} ALARM_FLAGS;
// 測定開始前ヘッド揺れ収束時間待ち
typedef struct {		// 時刻は ::GetTickCount() の値です．
	DWORD	dwWaitTimeSetting;		// 設定値(ini ファイルから取得)[ms]
	DWORD	dwStartTime;		// ステージ停止時刻[ms]
	DWORD	dwEndTime;			// 測定/AF 開始タイミング時刻[ms]
} WAIT_MEAS_TIME;

/////////////////////////////////////////////////////////////////////////////
// CChiefView ダイアログ

class CChiefView : public CFormView
{
protected:
	CChiefView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CChiefView)

// コンストラクション
public:
/* added 2013.02.01 hmenjo PreAF ---------- { ---------- */
	long m_lPreAFafterPreMove;
/* added 2013.02.01 hmenjo PreAF ---------- } ---------- */
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
	void PostTransiEvent(CWinThread* pcThread, UINT uiMsg, TCHAR* ptszMsg, BYTE byTransiKind, WPARAM wParam, LPARAM lParam);
	BOOL IsBusyTransi(BYTE byTransiKindDst, BYTE byTransiKindSrc);
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
/* added 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- { ---------- */
	DIO_IGNORE_SW m_DioIgnoreSW;
/* added 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- } ---------- */
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- { ---------- */
	HEAD_TYPE_CONFIG	m_EnableHead;
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- } ---------- */
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- { ---------- */
	BOOL IsCtaILPI(void);
/* added 2009.10.29 hmenjo CTA Seq CTAILPI チェック ---------- } ---------- */
/* added 2009.11.06 K.Matsuo Resist Seq チェック ---------- { ---------- */
	int CheckResistIL(void);
/* added 2009.11.06 K.Matsuo Resist Seq チェック ---------- } ---------- */
// 2009.11.12 bagus MS --{--
	BOOL IsMSILPI(void);
// 2009.11.12 bagus MS --}--
/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
	BOOL GetMainRcpInfHead(LPMAIN_RCP_INFO pMainRcpInfo, WORD wHeadType, LPHEAD_MAIN_RCP_INFO pHeadMainRcpInf);
/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */
/* added 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- { ---------- */
	int CheckReferenceData(TCHAR* ptszMainRcpName, double dLifeTime, BOOL bRef2ndMeasure = FALSE);
/* added 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- } ---------- */
/* added 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- { ---------- */
	int GetMeasLens(WORD wHeadType = HEAD_TYPE_SR);
/* added 2009.09.07 hmenjo 測定 PGM レンズ読出し ---------- } ---------- */
/* added 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- { ---------- */
	WORD GetMeasFilter(WORD wHeadType = HEAD_TYPE_SR);
/* added 2009.09.07 hmenjo 測定 PGM フィルタ読出し ---------- } ---------- */
/* added 2009.08.25 hmenjo SE ランプフィルタ制御関数 ---------- { ---------- */
	BOOL LampFilterR(WORD wFilter = 0, WORD wMode = 0);
/* added 2009.08.25 hmenjo SE ランプフィルタ制御関数 ---------- } ---------- */
/* added 2009.08.06 hmenjo ストレス機能追加(31) ---------- { ---------- */
	BOOL IsHWS();
/* added 2009.08.06 hmenjo ストレス機能追加(31) ---------- } ---------- */
/* added 2009.08.03 hmenjo ストレス機能追加(18) ---------- { ---------- */
	DWORD GetNextLineNo(DWORD* pdwLineNo);
/* added 2009.08.03 hmenjo ストレス機能追加(18) ---------- } ---------- */
/* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- { ---------- */
	DWORD m_dwPinMoveState;
/* added 2009.08.03 hmenjo ストレス機能追加(17) ---------- } ---------- */
/* added 2009.07.30 hmenjo ストレス機能追加(3) ---------- { ---------- */
	BOOL HepaOnOff(BOOL bOn, char* pcHepaAlarmLevel = 0);
/* added 2009.07.30 hmenjo ストレス機能追加(3) ---------- } ---------- */
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
	void SentMeasPauseCheck(int iReason);
	void SentMeasPause(int iReason, BOOL bRelease = FALSE);
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
	void LogChief_Transi(TCHAR* ptszTransiState, DWORD dwParam);
	void LogChief_WinMsg(TCHAR* ptszMsgName, WPARAM wparam, LPARAM lparam);
	void LogChief(TCHAR* pszLogText);
// 2013.01.10 bagus stage driver alarm io -->
	void LogStageError(TCHAR* pszLogText);
	void OutputStageError(int iDioAxisAlarm, int iAxis);
// 2013.01.10 bagus stage driver alarm io <--
	DWORD GetNextPointNo(DWORD* pdwPointNo);
	void WaitTimeBeforeMeas(DWORD dwMode);
// 2009.09.25 K.Matsuo CTA -->
	TCHAR m_szLotID[256];
// 2009.09.25 K.Matsuo CTA <--
	TCHAR m_szSampleID[256];

	void CheckDIO_Running(void);
	BOOL CheckDIO_IsAirPressureLowON(void);
// 2013.01.10 bagus stage driver alarm io -->
	int CheckDIO_IsX1AxisAlarm(void);
	int CheckDIO_IsX2AxisAlarm(void);
	int CheckDIO_IsYAxisAlarm(void);
// 2013.01.10 bagus stage driver alarm io <--
	BOOL CheckDIO_IsPinDownOFF(void);
	BOOL CheckDIO_IsRobotArmON(void);
	DWORD CheckDIO_IsEMO(void);
	BOOL HeadTypeCheck(WORD wHeadType);
	void ActuateFlagsSet(ACTUATE_FLAGS_SEL ActFgSel, BOOL bFlag);
	CHIEF_DI_INFO m_DiInfo;
	void ReportCancelComplete(int iResult);
	int ProcStatusGet(void);
	void ProcStatusSet(int iStatus);
	void PrepareToDestroy(void);
	BOOL m_bReqCancelComplete;
	BOOL m_bGotRecipeFromPif;
	BOOL SelectLens(UINT uiLens);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	BOOL SelectHead(WORD wHeadType, BOOL bStageMove);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
// 2013.02.22 bagus Substrate thickness setting -->
	BOOL SelectSubstrateThickIndex(int iIndex);
	BOOL SelectSubstrateThick(double dVal);
// 2013.02.22 bagus Substrate thickness setting <--
	BOOL m_bPreMoveSW;
	CHIEF_RECIPES m_ChiefRecipes;
	DWORD RecipesGet(TCHAR* pszMainRecipName, DWORD dwMode);
	DWORD m_dwStageMoveState;
	void GetCurrentMainRecipeName(TCHAR* pszCurrentMainRecipeName);
// 2009.11.30 K.Matsuo -->
	LPCTSTR GetMainRecipeNamePtr() {return m_szMainRecipeName;}
// 2009.11.30 K.Matsuo <--

	CWinThread* m_pcChiefTransiAF;
	CWinThread* m_pcChiefTransiDeskew;
	CWinThread* m_pcChiefTransiMaster;
	CWinThread* m_pcChiefTransiSeq;
	CWinThread* m_pcChiefTransiSr1Point;
	CWinThread* m_pcChiefTransiSrRefer;
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- { ---------- */
	CWinThread* m_pcChiefTransiStress;
/* added 2009.07.30 hmenjo ストレス機能追加(2) ---------- } ---------- */
	DWORD m_dwModuleState_Stage;
	DWORD m_dwModuleState_SR_Meas;
	HANDLE m_hEvTrMaster;
	CDocument* m_pcNanoSpecDoc;
	CFrameWnd* m_pcMainFrame;
//	CChiefView(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CChiefView)
	enum { IDD = IDD_CHIEF_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

	// マルチメディアタイマ コールバック定義
	static void CALLBACK MMTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	static UINT m_uiMMTimerID[16];

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual void PostNcDestroy();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefView)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void OnBtnHide();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnChkTop();
	afx_msg void OnBtnTest();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	afx_msg LRESULT OnHeartBeat(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnNextraResponse(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDispRecvPifCmd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnUpdateStatesOnDlg(WPARAM wparam, LPARAM lparam);
// 2009.02.05 K.Matsuo delete -->
//	afx_msg LRESULT OnSendTraceData(WPARAM wparam, LPARAM lparam);
// 2009.02.05 K.Matsuo delete <--
	afx_msg LRESULT OnPifDiRefresh(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnEQRunCheck(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnReportAlarms(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMoveSetPosFromPif(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMoveSetPosFromDisp(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStageStop(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnVacuumFromDisp(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetRecipe(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnAFPopupEnd(WPARAM wparam, LPARAM lparam);
// 2009.07.29 K.Matsuo -->
	afx_msg LRESULT OnManualSitePtPopupEnd(WPARAM wparam, LPARAM lparam);
// 2009.07.29 K.Matsuo <--
	afx_msg LRESULT OnMeasRecvData(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnData1PointEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataMeasEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartAAF(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartDispSeq(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStart1Point(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqPause(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqResume(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSeqCancel(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnStartSrRefer(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDlgShowSW(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnResetAlarm(WPARAM wparam, LPARAM lparam);
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- { ---------- */
	afx_msg LRESULT OnMeasLineEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataLineEnd(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDataStressMeasEnd(WPARAM wparam, LPARAM lparam);
/* added 2009.08.05 hmenjo ストレス機能追加(25) ---------- } ---------- */
//2009.10.28 bagus 2point-distance --{--
	afx_msg LRESULT OnDistancePopupEnd(WPARAM wparam, LPARAM lparam);
//2009.10.28 bagus 2point-distance --}--
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- { ---------- */
	afx_msg LRESULT OnMeasAlarmCTA(WPARAM wparam, LPARAM lparam);
/* added 2009.10.30 hmenjo CTA アラームハンドラ ---------- } ---------- */
//2009.11.03 bagus MS --{--
	afx_msg LRESULT OnMSPopupEnd(WPARAM wparam,LPARAM lparam);
//2009.11.03 bagus MS --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	afx_msg LRESULT OnMeasCompEASEStatus(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMeasCompEASEError(WPARAM wparam, LPARAM lparam);
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
	afx_msg LRESULT OnConfirmPopupEnd(WPARAM wparam, LPARAM lparam);
// 2013.11.07 Bagus Add (TohoSpec対応) <--

	DECLARE_MESSAGE_MAP()
private:
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- { ---------- */
	int CtaReset(DWORD dwMode = 0);
/* added 2009.11.06 hmenjo CTA アラームリセット時異常リセット ---------- } ---------- */
	DWORD m_bCheckEqPowerOFF;
	void EqPowerOffPulse(void);
	WAIT_MEAS_TIME m_WaitMeasTime;
	BOOL m_bSpeedLimiterOFF;
	CShadeButtonST m_HideButton;
	CShadeButtonST m_TopButton;
	CShadeButtonST m_TestButton;

	ALARM_FLAGS m_AlarmFlags;
	void CancelSeqForAlarm(void);
	void StopAxisAllEMO(void);
/* deleted 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- { ---------- */
//	DIO_IGNORE_SW m_DioIgnoreSW;
/* deleted 2009.11.27 hmenjo Pif ピン&シャッタ 同時動作 ---------- } ---------- */
	DWORD RecipesGetCheck(TCHAR *pszMainRecipeName, DWORD dwMode);
	void RecipesMalloc(BOOL bAlloc);
	DWORD m_dwModuleState_Pif;
	DWORD m_dwModuleState_Nextra;
	TCHAR m_szMainRecipeName[256];
	BOOL m_bVacuumOnFromDisp;
	BOOL m_bVacuumOffFromDisp;
	void IsStageIdle(void);
	void TransitionsEnd(void);
	DWORD TransitionsStart(void);
	BOOL m_bEQRunPrev;
	void GetDiInfo(CHIEF_DI_INFO* pDiInfo);
// 2009.02.05 K.Matsuo delete -->
//	BOOL m_bTraceDataTimer;
//	BOOL StartTimerTraceData(UINT uiTraceDataPeriod);
//	UINT m_uiTraceDataPeriod;
// 2009.02.05 K.Matsuo delete <--
	CWinThread* m_pcChiefRcvMailThread;
	CHIEF_DLGSHOWSW m_DlgShowSW;
	DWORD m_dwHeartBeatCnt;
/* added 2009.05.27 hmenjo SPT アラームの検出 ---------- { ---------- */
	void SPTAlarmDetector(void);
/* added 2009.05.27 hmenjo SPT アラームの検出 ---------- } ---------- */
};
