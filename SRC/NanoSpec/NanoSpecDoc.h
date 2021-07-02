/////////////////////////////////////////////////////////////////////////////
// NanoSpecDoc.h : CNanoSpecDoc クラスの宣言およびインターフェイスの定義   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NANOSPECDOC_H__0D7A1318_6079_48DF_BA64_850B8EACFF94__INCLUDED_)
#define AFX_NANOSPECDOC_H__0D7A1318_6079_48DF_BA64_850B8EACFF94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Definition															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
#define OPERATION_LOG_NAME		"Operation"
#define OPERATION_LOG_SIZE		(1000000)
#define OPERATION_LOG_MAX		(30)
// hmenjo ChiefLog -->
#define CHIEF_LOG_NAME		"Chief"
#define CHIEF_LOG_SIZE		(1000000)
#define CHIEF_LOG_MAX		(30)
// hmenjo ChiefLog <--
// 2013.01.10 bagus stage driver alarm io -->
#define STAGE_ERROR_LOG_NAME	"StageError"
#define STAGE_ERROR_LOG_SIZE	(1000000)
#define STAGE_ERROR_LOG_MAX		(30)
// 2013.01.10 bagus stage driver alarm io <--
//Saiki 20100517 Add ----->
#define FRETEN_LOG_NAME		"FileRetention"
#define FRETEN_LOG_SIZE		(1000000)
#define FRETEN_LOG_MAX		(30)
//Saiki 20100517 Add <-----
#define ALARM_HIS_MAX			(1000)

#define ALARM_LOG_DATE_LEN		(10)			// 2008.08.04
#define ALARM_LOG_TIME_LEN		(8) 			// 23:46:12
#define ALARM_LOG_LEVEN_LEN 	(5) 			// "ALARM", "WARN"
#define ALARM_LOG_SET_LEN		(3) 			// "SET", "RST"
#define ALARM_LOG_ID_LEN		(4) 			// "0001"
#define ALARM_LOG_TEXT_LEN		(80)

/////////////////////////////////////////////////////////////////////////////
// Structure															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
typedef struct{
	char Date[ALARM_LOG_DATE_LEN + 1];
	char Time[ALARM_LOG_TIME_LEN + 1];
	char Level[ALARM_LOG_LEVEN_LEN + 1];
	char Set[ALARM_LOG_SET_LEN + 1];
	char Id[ALARM_LOG_ID_LEN + 1];
	char Text[ALARM_LOG_TEXT_LEN + 1];
}ALARM_HIS;

/*
 *	動作中フラグ群の構造体
 */
typedef struct {
	BOOL	bXYStage;		// XY ステージ
	BOOL	bZAxis; 		// Z 軸
	BOOL	bPin;			// ピン
	BOOL	bShutter;		// シャッタ
	BOOL	bWorkGuide; 	// ワークガイド
	BOOL	bTurret;		// ターレット
// 2009.08.28 K.Matsuo -->
	BOOL	bHead;			// ヘッド
// 2009.08.28 K.Matsuo <--
// 2009.11.10 bagus MS --{--
	BOOL	bMS;			// 顕微鏡
// 2009.11.10 bagus MS --}--
} ACTUATE_FLAGS;

/////////////////////////////////////////////////////////////////////////////
//
class CNanoSpecDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CNanoSpecDoc();
	DECLARE_DYNCREATE(CNanoSpecDoc)

// アトリビュート
public:

// オペレーション
public:
	///// Variable //////////////////////////////////////////////////////////
	CLogFile m_OperationLogFile;
	ALARM_HIS m_AlarmHis[ALARM_HIS_MAX];
	int m_AlarmHis_Count;

	///// Mode /////
	int m_iHostMode;

	/////  Status /////
	int m_iPifCommConnectStatus;
	int m_iProcessStatus;
	int m_iVacuumStatus;
	int m_iAlarmStatus;
	int m_iWarningStatus;
	int m_iDispStatus;
	BOOL m_iInitFinished;
	BOOL m_bJoyStickSelectEnabled;		// 現在のハードJoyStick許可／禁止の状態
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- { ---------- */
	long m_lZ_AF;
/* added 2009.12.02 hmenjo 測定 Seq は指定ヘッドで動作(画面) ---------- } ---------- */

	///// User /////
	USER_ACCOUNT m_User;

	MAINTENANCE_DATE m_gMaintenanceDate;
	MAINTENANCE_LAMP m_gMaintenanceLamp;
	COleDateTimeSpan m_gPassageHour;		// 経過時間

private:
	MEASURED_DATA* m_pMeasureddata;
	int m_iLastMeasPoint;		//最終測定点(通常測定時:測定ポイント数, リメジャーモード時:再測定ポイントの一番大きい点)
	int m_iCurrentSetPoint; //一番最後にデータを格納(書換え)したポイント番号(通常測定時はGetStoreCount()で得られる値と同じ)
	int m_iSeqMeasMode; //シーケンス測定モード(測定中でない/通常測定中/リメジャー中)
	int m_iMeasMode;	//測定モード(測定中でない/通常測定中orリメジャー中/マニュアル測定中/テストモード中)
	int m_iManuMeasMode;	//マニュアル測定モードの状態
	int m_iReMeasNumScans;	//リメジャー時の測定ポイント数
	int m_iReMeasurePointCnt; //リメジャー時の測定済ポイント数
	BOOL m_bPauseFlg;	//ポーズ判定フラグ(TRUE:ポーズ FALSE:それ以外)
	// 2009.08.17 bagus stress --{--
	int m_iLastMeasStressLine;			//ストレス測定の最終測定ストレスライン
	int m_iCurrentSetStressLine;		//一番最後にデータを格納したストレスライン
	int m_iCurrentMeasStressLine;
	// 2009.08.17 bagus stress --{--

// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	BOOL m_bSetSampleIdFromUI;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

public:
	char m_szName[RECIPE_NAME_LEN + 1];

	///// Function //////////////////////////////////////////////////////////
/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
	long GetExpDouble(double dData);
/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */
/* added 2009.09.10 hmenjo ガントリ透過率用ランプチェック ---------- { ---------- */
	BOOL IsGTRLampOn(void);
/* added 2009.09.10 hmenjo ガントリ透過率用ランプチェック ---------- } ---------- */
/* added 2009.09.07 hmenjo 透過率 ランプ制御 ---------- { ---------- */
	BOOL SetTransmittanceLamp(WORD wFilter = FILTER_DARK);
/* added 2009.09.07 hmenjo 透過率 ランプ制御 ---------- } ---------- */
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- { ---------- */
	BOOL HepaOnOff(BOOL bOn, char* pcHepaAlarmLevel = 0);
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- } ---------- */
/* added 2009.07.30 hmenjo ストレス機能追加(4) ---------- { ---------- */
	BOOL GetAlarmInf(int iAlId, int* piAlCd, char* pcLevel, TCHAR* ptszAlText, int iTextBufLen);
/* added 2009.07.30 hmenjo ストレス機能追加(4) ---------- } ---------- */
/* added 2009.07.23 hmenjo スキャンタイプチェック関数追加 ---------- { ---------- */
	BOOL IsValidScanType(WORD wScanType, WORD wHeadType);
/* added 2009.07.23 hmenjo スキャンタイプチェック関数追加 ---------- } ---------- */
/* added 2009.07.23 hmenjo ヘッドタイプチェック関数追加 ---------- { ---------- */
	BOOL IsValidHeadType(WORD wHeadType);
/* added 2009.07.23 hmenjo ヘッドタイプチェック関数追加 ---------- } ---------- */
/* added hmenjo 2009.05.20 ドアインタロック無効化対応 -------- { -------- */
	BOOL Rap_IsDoorInterlock();
/* added hmenjo 2009.05.20 ドアインタロック無効化対応 -------- } -------- */
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- { -------- */
	BOOL Rap_IsAirPressureLowON(BOOL bIgnore = 0);
/* added hmenjo 2009.05.20 エア圧力低下検出のラッパ関数 -------- } -------- */
	void StageSpeedLimiter(BOOL bLimiter);
// hmenjo ChiefLog -->
	void ChiefLogging(TCHAR* pszLogText);
// hmenjo ChiefLog <--
// 2013.01.10 bagus stage driver alarm io -->
	void StageErrorLogging(TCHAR* pszLogText);
// 2013.01.10 bagus stage driver alarm io <--
	void OperationLogging(char* lpszLogText);
	void MakeNanospecDir();
	int MakeDirectory(char* lpszDir);
	void AddTime(char* lpszBuff);
	//Saiki 20100517 Add ----->
	void FileRetentionLogging(char* lpszLogText);
	//Saiki 20100517 Add <-----
    /* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- { -------- */
//	/* ジョイスティック切替用 */
//	BOOL ChangeJoyStick(BOOL flag);
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- 		   */
	/* ジョイスティック切替用 */
	BOOL ChangeJoyStick(BOOL flag, int iSpeedSel = 3);
/* modified hmenjo 2009.05.20 SPT 用 JoyStick 対応 -------- } -------- */
//	// 動作判定関数
//	BOOL ActuateEnableCheck(ACTUATE_FLAGS_SEL ActFgSel, ACTUATE_MODE_SEL ActMode);
	// メッセージをセットする
	//Saiki 20090530 Change ----->
	//void MessageIf_Set(int m_MsgID);
	void MessageStringIf_Set(CString strBuffer);
	//Saiki 20090530 Change <-----
// 2009.10.09 bagus StagePGM 共通化 --{--
	int GetStagePGM(int nHeadType, int nScanType);
	int GetStagePGMItem(int nHeadType, int nScanType);
	BOOL IsValidStagePGM(int nHeadType, int nScanType, int nStagePGMType);
// 2009.10.09 bagus StagePGM 共通化 --}--

	void InitMeasData(void);
	void GetMeasDataAll(MEASURED_DATA* meadat);
	void SetMeasDataAll(MEASURED_DATA* meadat);
	void GetRcpData(RCP_DATA* rcp_data);
	void SetRcpData(RCP_DATA* rcp_data);
	BOOL GetOnePointAndStatistics(ONE_POINT_DATA *OnePointData, STATISTICS *Statistics, int iPoint); //非推奨
	BOOL SetOnePointAndStatistics(ONE_POINT_DATA *OnePointData, STATISTICS *Statistics, int iPoint); //非推奨
	BOOL GetOnePointData(ONE_POINT_DATA *OnePointData, int iPoint);
	BOOL SetOnePointData(ONE_POINT_DATA *OnePointData, int iPoint);
	BOOL SetTestModeData(TEST_MODE_DATA *TestModeData, int iPoint);
	BOOL GetStatisticsData(STATISTICS *Statistics);
	BOOL SetStatisticsData(STATISTICS *Statistics);
	int GetStatisticsValidPointCount(void);
	void SetStatisticsValidPointCount(int iStatisticsValidPointCount);
	//2009.08.20 bagus stress --{--
	BOOL SetOneStressLineData(STRESSRESULT *OneStressLineData,int iPoint);
	BOOL GetOneStressLineData(STRESSRESULT *OneStressLineData,int iPoint);
	//2009.08.20 bagus stress --}--
	//2009.11.04 bagus CA --{--
	BOOL SetOneContactAngleData(CTARESULT *OneCTAResult,int iPoint);
	BOOL GetOneContactAngleData(CTARESULT *OneCTAResult,int iPoint);
	//2009.11.04 bagus CA --}--
	//2009.11.10 bagus RS --{--
	BOOL SetOneResistData(RESISTRESULT *OneResistResult,int iPoint);
	BOOL GetOneResistData(RESISTRESULT *OneResistResult,int iPoint);
	//2009.11.10 bagus RS --}--

// 2009.09.30 K.Matsuo Delete -->
//	double NanoSpecDoc_GetAnalysisData(int iCnt, int iWave);
//	double NanoSpecDoc_GetAnalysisGenData(int iCnt, int iWave);
// 2009.09.30 K.Matsuo Delete <--
// 2014.04.03 bagus wavelength step modified -->
// 	double NanoSpecDoc_GetScanSampleData(int iCnt, int iWave);
// 	double NanoSpecDoc_GetScan1stReferenceData(int iCnt, int iWave);
// 	double NanoSpecDoc_GetScanDark_T1Data(int iCnt, int iWave);
	double NanoSpecDoc_GetScanSampleData(int iCnt, double dWave);
	double NanoSpecDoc_GetScan1stReferenceData(int iCnt, double dWave);
	double NanoSpecDoc_GetScanDark_T1Data(int iCnt, double dWave);
// 2014.04.03 bagus wavelength step modified <--
	void GetMeasRcpCon(int iCnt, TEST_MODE_MEASRCP_CONDITION* MeasRcpCon);
	void GetTestModeData(TEST_MODE_DATA* TestModeData, int iCnt);
	void GetOnepointDdata(int iPoint, double* dData);
	void GetLabel(char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1], char szTitleAf[AF_HEADER_LEN + 1], char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], int *iLabelCount);
	void GetLabelOri(char pszLabelOri[][ADAPRESULTSTRINGLENMAX + 1]);
	void SetLabel(char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1], char szTitleAf[AF_HEADER_LEN + 1], char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1], int iLabelCount);
	void GetDispLabel(char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1]);
	void SetDispLabel(char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1]);
	int GetDispLabelCount(void);
	void SetDispLabelCount(int iDispLabelCount);
	int GetStoreCount(void);
	void SetStoreCount(int iSetPointCount);
	int GetMeasurePointCount(void);
	void SetMeasurePointCount(int iSetPointCount);
	void GetUserId(char szUserId[50+1]);
	void SetUserId(char szUserId[50+1]);
	void GetStartDateTime(SYSTEMTIME* syStartDateTime);
	void SetStartDateTime(SYSTEMTIME* syStartDateTime);
	void GetEndDateTime(SYSTEMTIME* syEndDateTime);
	void SetEndDateTime(SYSTEMTIME* syEndDateTime);
	void GetSampleId(char szSampleId[SAMPLE_ID_LEN + 1]);
	void SetSampleId(char szSampleId[SAMPLE_ID_LEN + 1]);
	//2009.10.13 bagus 4PP --{--
	//ロットID追加
	void GetLotId(char szLotID[LOT_ID_LEN + 1]);
	void SetLotId(char szLotID[LOT_ID_LEN + 1]);
	//2009.10.13 bagus 4PP --}--
	void GetUnitName(char szUnit[DISP_UNIT_NAME_LEN + 1]);
	void SetUnitName(char szUnit[DISP_UNIT_NAME_LEN + 1]);
	int GetUnitIndex(void);
	void SetUnitIndex(int iUnit);
	int GetShowCompPointList(void);
	void SetShowCompPointList(int iShowCompletionPointList);
	int GetShowCompPointGraph(void);
	void SetShowCompPointGraph(int iShowCompletionPointGraph);


	int GetLastMeasPoint(void);
	void SetLastMeasPoint(int iLastMeasPoint);
	int GetCurrentSetPoint(void);
	void SetCurrentSetPoint(int iCurrentSetPoint);
	//2009.08.22 bagus stress --{--
	BOOL GetOneStressLineDataValid(int iLineNo);
	void SetOneStressLineDataValid(int iLineNo,BOOL bValid);
	BOOL GetOneStressLineDataRef(int iLineNo);
	void SetOneStressLineDataRef(int iLineNo,BOOL bRef);
	BOOL IsStressRefereneMeasuremnt();
	//2009.08.22 bagus stress --}--
	//2009.08.30 bagus stress --{--
	int GetCurrentMeasureStressLine();
	void SetCurrentMeasureStressLine(int nLineNo);
	//2009.08.30 bagus stress --}--
	//2009.08.31 bagus stress --{--
	void GetStressLineStatisticsData(int iLineNo,STATISTICS *pStat);
	void SetStressLineStatisticsData(int iLineNo,STATISTICS *pStat);
	void GetStressStatisticsData(STATISTICS *pStat);
	void SetStressStatisticsData(STATISTICS *pStat);
	//2009.08.31 bagus stress --}--
	//2009.09.01 bagus stress --{--
	int GetMeasuredStressLSNum();
	//2009.09.01 bagus stress --}--
	//2009.09.02 bagus stress --{--
	int GetShowCompStressList(void);
	void SetShowCompStressList(int iShowCompletionPointList);
	int GetShowCompStressGraph(void);
	void SetShowCompStressGraph(int iShowCompletionPointGraph);
	//2009.09.02 bagus stress --}--
	//2009.10.28 bagus 2point-Distance --{--
	STAGE_COORD GetLastDistancePoint();
	void SetLastDistancePoint(STAGE_COORD tCoord);
	//2009.10.28 bagus 2point-Distance --}--

	int GetSeqMeasMode(void);
	void SetSeqMeasMode(int iSeqMeasMode);
	int GetMeasMode(void);
	void SetMeasMode(int iMeasMode);
	int GetManuMeasMode(void);
	void SetManuMeasMode(int iManuMeasMode);
	int GetReMeasNumScans(void);
	void SetReMeasNumScans(int iReMeasNumScans);
	int GetReMeasurePointCnt(void);
	void SetReMeasurePointCnt(int iReMeasurePointCnt);
	BOOL GetPauseFlg(void);
	void SetPauseFlg(BOOL bPauseFlg);
	BOOL GetLabelFlag(){ return m_pMeasureddata->bLabelFlag; };
	void SetLabelFlag(BOOL bLabelFlag){ m_pMeasureddata->bLabelFlag = bLabelFlag; };
	void SetInitFinished(BOOL bFlag){m_iInitFinished = bFlag;};
	BOOL GetInitFinished(){ return m_iInitFinished; };
	/*//Saiki 20090520 Add ----->*/
	BOOL IsInterLock(void);
	//2009.12.25 bagus 修正 --{--
	BOOL IsInterLockForMScope(void);
	//2009.12.25 bagus 修正 --}--
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ---------- { ---------- */
//	BOOL CheckUnitStatus(void);
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ----------			   */
	BOOL CheckUnitStatus(BOOL bEnablePinAlign = FALSE);
/* modified 2009.08.20 hmenjo Pin Aling 位置有効化 ---------- } ---------- */
	BOOL CheckActiveFlag(void);
	/*//Saiki 20090520 Add <-----*/

	BOOL JoyStickChangeDisable();		// ハードJoyを一時的に禁止にする
	BOOL JoyStickStatusRestore();		// ハードJoyの状態を元に戻す

	///// Alarm History /////
	void AlarmHis_Restore(char* Path, char* filename,int LogMax,int HisMax);
	void AlarmHis_StrToHis(char* pszStr, ALARM_HIS* pAlarmHis);
	int AlarmHis_Add(int Index, ALARM_HIS* pAlarmHis);
	int AlarmHis_GetCount(){return m_AlarmHis_Count;};
	BOOL AlarmHis_Get(int Index, ALARM_HIS* pAlarmHis);

	///// User /////
	BOOL User_Is();
	void User_Entry(USER_ACCOUNT User);
	void User_Remove();
	char* User_GetName(){ return m_User.szName; };
	int User_GetLevel(){ return m_User.nAccountLevel; };
	BOOL User_Access(int iAccessItem);

	///// Mode /////
	void SetHostMode(int Mode){ m_iHostMode = Mode; };
	int GetHostMode(){ return m_iHostMode; };

	///// PifCommConnectStatus /////
	void SetPifCommConnectStatus(int Status){ m_iPifCommConnectStatus = Status; };
	int GetPifCommConnectStatus(){ return m_iPifCommConnectStatus; };

	///// Process Status /////
	void SetProcessStatus(int Status){ m_iProcessStatus = Status; };
	int GetProcessStatus(){ return m_iProcessStatus; };

	///// Vacuum Status /////
	void SetVacuumStatus(int Status){ m_iVacuumStatus = Status; };
	int GetVacuumStatus(){ return m_iVacuumStatus; };

	///// Alarm status /////
	void SetAlarmStatus(int Status){ m_iAlarmStatus = Status; };
	int GetAlarmStatus(){ return m_iAlarmStatus; };

	///// Warning Status /////
	void SetWarningStatus(int Status){ m_iWarningStatus = Status; };
	int GetWarningStatus(){ return m_iWarningStatus; };

	///// 画面ステータス /////
	void SetDispStatus(int Status) { m_iDispStatus = Status ;};
	int GetDispStatus(){ return m_iDispStatus; };

	///// PatRec 動作中フラグ /////
	void SetPatRecFlag(BOOL bFlag);
	BOOL GetPatRecFlag(void);

	///// JoyStick切替許可／禁止 /////
	void SetJoyStickSelectEnabled(BOOL bFlg) { m_bJoyStickSelectEnabled = bFlg; };
	BOOL GetJoyStickSelectEnabled() { return m_bJoyStickSelectEnabled; };

	///// メンテナンスデータ /////
	void SetMaintenanceDate(MAINTENANCE_DATE m_pMaintenanceDate){ m_gMaintenanceDate = m_pMaintenanceDate; };
	MAINTENANCE_DATE GetMaintenanceDate(){ return m_gMaintenanceDate; };
	void SetMaintenanceLamp(MAINTENANCE_LAMP m_pMaintenanceLamp){ m_gMaintenanceLamp = m_pMaintenanceLamp; };
	MAINTENANCE_LAMP GetMaintenanceLamp(){ return m_gMaintenanceLamp; };
	void SetPassageHour(double Hour){ m_gPassageHour.m_span = Hour; };
	double GetPassageHour(){ return m_gPassageHour.m_span; };

	//2009.11.04 bagus CA --{--
	//手動測定時の準備/後始末処理
	BOOL PrepareManualCAMeasure();
	BOOL CleanupManualCAMeasure();
	//2009.11.04 bagus CA --}--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	void SetSampleIdFromUI(BOOL bSet);
	BOOL IsSetSampleIdFromUI();
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoSpecDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	BOOL m_bDoPointMeasFlag[SCAN_POINT_MAX];
// 2009.02.05 K.Matsuo delete -->
//	UINT GetTraceDataPeriod(void) {return 0; /* ダミーリターンです */};
// 2009.02.05 K.Matsuo delete <--

	///// 動作中フラグ群アクセス関数 /////

	DWORD CNanoSpecDoc::ActuateFlagsGetAll();

	BOOL ActuateFlagsGet(ACTUATE_FLAGS_SEL ActFgSel);
	void ActuateFlagsSet(ACTUATE_FLAGS_SEL ActFgSel, BOOL bFlag);
	virtual ~CNanoSpecDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CNanoSpecDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
// hmenjo ChiefLog -->
	CLogFile m_ChiefLogFile;
// hmenjo ChiefLog <--
	ACTUATE_FLAGS m_ActuateFlags;
	BOOL m_bPatRecFlag;

	// ジョイスティック切替え用
	BOOL m_ActualJoyStickFlag;
//Saiki 20100517 Add ----->
	CLogFile m_FRetentionLogFile;
//Saiki 20100517 Add <-----
// 2013.01.10 bagus stage driver alarm io -->
	CLogFile m_StageErrorLogFile;
// 2013.01.10 bagus stage driver alarm io <--
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOSPECDOC_H__0D7A1318_6079_48DF_BA64_850B8EACFF94__INCLUDED_)
