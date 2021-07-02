#ifndef _ALARM_IF_HXX_
#define _ALARM_IF_HXX_

#if !defined( _ALARMIFDLL_ )
	#define ALARMIFAPI __declspec(dllimport)
#else
	#define ALARMIFAPI __declspec(dllexport)
#endif

/////////////////////////////////////////////////////////////////////////////
// Window Message
#define WM_ALARMIF									(WM_APP + 0x0500)
#define WM_ALARMIF_SET								(WM_ALARMIF + 0x00)
#define WM_ALARMIF_RESET							(WM_ALARMIF + 0x01)


/////////////////////////////////////////////////////////////////////////////
// AlarmID (0001-4095)
#define NANOEXEALID									(0000)
#define NEXALID										(1000)	// See. NEXCOM.HXX and this file below.

// NANOEXEALID
#define ALID_EMOSwitchOn							(1 + NANOEXEALID)
#define ALID_DoorOpen								(2 + NANOEXEALID)
#define ALID_EquipmentPowerOff						(3 + NANOEXEALID)
#define ALID_RobotArmDetected						(4 + NANOEXEALID)
#define ALID_LifterLowestPositionSensorOff			(5 + NANOEXEALID)
#define ALID_WorkGuideOpenSensorOff					(6 + NANOEXEALID)
#define ALID_MaintenanceSwitchOn					(7 + NANOEXEALID)
#define ALID_AirPressureDown						(8 + NANOEXEALID)
#define ALID_VacuumSensorOff						(9 + NANOEXEALID)
#define ALID_Xmp_ADAP_StartingUpFailed				(10 + NANOEXEALID)
#define ALID_PatternRecognitionInitializeFailed		(11 + NANOEXEALID)
#define ALID_StageModuleInitializeFailed			(12 + NANOEXEALID)
#define ALID_MeasurementModuleInitializeFailed		(13 + NANOEXEALID)
#define ALID_MeasurementPointMoveFailed_PreMotion	(14 + NANOEXEALID)	// SEQ:PRE-MOTION
#define ALID_MeasurementPointMoveFailed				(15 + NANOEXEALID)
#define ALID_Z_AxisMoveFailed						(16 + NANOEXEALID)
#define ALID_MeasurementError						(17 + NANOEXEALID)
#define ALID_AnalysisError							(18 + NANOEXEALID)
#define ALID_ReferencePositionMoveFailed			(19 + NANOEXEALID)
#define ALID_ReferencePosition_Z_AxisMoveFailed		(20 + NANOEXEALID)
#define ALID_ReferenceFirstMeasurementFailed		(21 + NANOEXEALID)
#define ALID_ReferenceSecond_T1_Failed				(22 + NANOEXEALID)
#define ALID_ReferenceSecond_T2_Failed				(23 + NANOEXEALID)
#define ALID_DarkReferencePositionMoveFailed		(24 + NANOEXEALID)
#define ALID_DarkReferencePosition_Z_AxisMoveFailed	(25 + NANOEXEALID)
#define ALID_DarkReferenceMeasurement_T1_Failed		(26 + NANOEXEALID)
#define ALID_DarkReferenceMeasurement_T2_Failed		(27 + NANOEXEALID)
#define ALID_MainRecipeReadError					(28 + NANOEXEALID)
#define ALID_MeasurementProgramReadError			(29 + NANOEXEALID)
#define ALID_StageProgramReadError					(30 + NANOEXEALID)
#define ALID_SequenceMeasurementPointsSettingError	(31 + NANOEXEALID)
#define ALID_SequenceMeasurementPointReadError		(32 + NANOEXEALID)
#define ALID_RecipeReadError						(33 + NANOEXEALID)
#define ALID_ReferenceDataWriteFailed				(34 + NANOEXEALID)
#define ALID_ReferenceData_T1_WriteFailed			(35 + NANOEXEALID)
#define ALID_ReferenceData_T2_WriteFailed			(36 + NANOEXEALID)
#define ALID_ShutterCloseError						(37 + NANOEXEALID)
#define ALID_AllMeasurementPointsExecutingFlagInvalid	(38 + NANOEXEALID)
#define ALID_EquipmentPowerOffControlError			(39 + NANOEXEALID)
#define ALID_MaintenancePeriodElapsed				(40 + NANOEXEALID)
#define ALID_LampExchangePeriodElapsed				(41 + NANOEXEALID)
// 2009.08.20 K.Matsuo -->
#define	ALID_HepaStopFailed							(42 + NANOEXEALID)	/* HEPA アラーム ID	*/
#define	ALID_Z_PositionRetractFailed				(43 + NANOEXEALID)	/* ストレス Z 位置退避 アラーム ID	*/
#define	ALID_LoadPositionMoveStartFailed			(44 + NANOEXEALID)	/* ロードポジション移動開始失敗 アラーム ID	*/
#define	ALID_LifterMoveStartFailed					(45 + NANOEXEALID)	/* リフタ動作開始失敗 アラーム ID	*/
#define	ALID_SafetyPlugOpen							(46 + NANOEXEALID)	/* セーフティプラグ開 アラーム ID	*/
#define	ALID_MeasPauseError							(47 + NANOEXEALID)	/* 測定中断異常	*/
// 2009.08.20 K.Matsuo <--
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- { ---------- */
#define	ALID_CTA_IL_PI_On							(48 + NANOEXEALID)	/* CTA IL PI オフ検出	*/
#define	ALID_CTA_Setinf								(49 + NANOEXEALID)	/* CTA 情報設定エラー	*/
/* added 2009.10.30 hmenjo CTA CTAILPI 常時チェック ---------- } ---------- */
// 2009.11.03 bagus MS --{--
#define ALID_MS_IL_POS_ERROR						(50	+ NANOEXEALID)	/* マイクロスコープ 位置異常 */
// 2009.11.03 bagus MS --}--

#define ALID_MeasurementStartFailed					(51 + NANOEXEALID)
#define ALID_SubControllerReplayAbnormal			(52 + NANOEXEALID)
#define ALID_OneSampledMeasurementFailed			(53 + NANOEXEALID)
#define ALID_MeasurementFailed_ReMeasurement		(54 + NANOEXEALID)
#define ALID_ReferenceAcquisitionFailed				(55 + NANOEXEALID)
#define ALID_MeasurementFailed_ManualMeasurement	(56 + NANOEXEALID)
#define ALID_MeasurementFailed_TestMode				(57 + NANOEXEALID)
#define ALID_MeasurementResultRetentionFailed		(58 + NANOEXEALID)
#define ALID_MeasurementResultNameDuplication		(59 + NANOEXEALID)
#define ALID_StageError								(60 + NANOEXEALID)
#define ALID_StatusTransitionError					(61 + NANOEXEALID)
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
#define ALID_ChiefTransi_Failed_PostThreadMessage	(62 + NANOEXEALID)
#define ALID_ChiefTransi_BusyTransition				(63 + NANOEXEALID)
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
#define ALID_SelectHeadError						(69 + NANOEXEALID)	/* ヘッド選択エラー	*/
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
/* added 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- { ---------- */
#define ALID_GTR_Lamp_Error							(70 + NANOEXEALID)	/* GTR ランプエラー	*/
/* added 2009.11.20 hmenjo GTR ランプ初期化時 Close ---------- } ---------- */
#define ALID_StageShutterLowestPositionSensorOff	(71 + NANOEXEALID)
#define ALID_StageInActive							(72 + NANOEXEALID)
#define ALID_Z_AxisInActive							(73 + NANOEXEALID)
#define ALID_LifterInActive							(74 + NANOEXEALID)
#define ALID_WorkGuideInActive						(75 + NANOEXEALID)
#define ALID_TurretInActive							(76 + NANOEXEALID)
#define ALID_StageShutterInActive					(77 + NANOEXEALID)
// 2013.02.22 bagus Substrate thickness setting -->
#define ALID_SubstrateThicknessError				(78 + NANOEXEALID)
// 2013.02.22 bagus Substrate thickness setting <--

#define ALID_OperatorCall							(91 + NANOEXEALID)
#define ALID_AutoFocusRequired						(92 + NANOEXEALID)
#define ALID_StagePositioningRequired				(93 + NANOEXEALID)
#define ALID_FocusAndPositioningRequired			(94 + NANOEXEALID)
#define ALID_ManualDeskewRequired					(95 + NANOEXEALID)
#define ALID_ManualSitePatternRequired				(96 + NANOEXEALID)

#define ALID_EngineerMaintenanceSwitchOn			(101 + NANOEXEALID)

/* added 2009.11.06 K.Matsuo RS Seq Head Position 常時チェック ---------- { ---------- */
#define ALID_ResistUpperPositionSensorOff			(110 + NANOEXEALID)
#define ALID_ResistUpperPositionSensorError			(111 + NANOEXEALID)
/* added 2009.11.06 K.Matsuo RS Seq Head Position 常時チェック ---------- } ---------- */

//#define ALID_CTA_RESERVE120						(120 + NANOEXEALID)
// ～
//#define ALID_CTA_RESERVE189						(189 + NANOEXEALID)


// NEXALID
// See. NEXCOM.HXX and below.
#define ALID_SptAlarmStartNo						(20 + NEXALID)
#define ALID_SptStageWdError						(0 + ALID_SptAlarmStartNo)
#define ALID_SptStageNeedRebootPc					(1 + ALID_SptAlarmStartNo)
#define ALID_SptStageAlarm							(2 + ALID_SptAlarmStartNo)
#define ALID_SptStageInitCompFlagNotZero			(3 + ALID_SptAlarmStartNo)
#define ALID_SptStageDioError						(4 + ALID_SptAlarmStartNo)
#define ALID_SptStageServoOnError					(5 + ALID_SptAlarmStartNo)
#define ALID_SptStageEmergencyStop					(6 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommRead_SystemError			(7 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommRead_RetryOut1				(8 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommRead_RetryOut2				(9 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommRead2_SystemError			(10 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommRead2_RetryOut1			(11 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommRead2_RetryOut2			(12 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommWrite_SystemError			(13 + ALID_SptAlarmStartNo)
#define ALID_SptStageCommWrite_RetryOut				(14 + ALID_SptAlarmStartNo)
#define ALID_SptStageNotDefinedError				(15 + ALID_SptAlarmStartNo)
/* added 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- { ---------- */
#define ALID_SptStageMotionTimeout					(16 + ALID_SptAlarmStartNo)
/* added 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- } ---------- */

#define ALID_StageErrorStartNo						(40 + NEXALID)
#define ALID_StageOverTravelError					(0 + ALID_StageErrorStartNo)
#define ALID_StageUnknownError						(1 + ALID_StageErrorStartNo)

/////////////////////////////////////////////////////////////////////////////
// EXPORTED FUNCTION PROTOTYPES 										   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

ALARMIFAPI void __stdcall AlarmIf_Init(HWND hWnd);
ALARMIFAPI BOOL __stdcall AlarmIf_Set(int Id);
ALARMIFAPI BOOL __stdcall AlarmIf_Reset(int Id);

#endif // _ALARM_IF_HXX_
