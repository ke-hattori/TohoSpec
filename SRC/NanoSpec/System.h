//	System.h

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#ifndef _MASTER_
#define _EXTERN extern
#else
#define _EXTERN
#endif	// _MASTER_

#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\ConfigFile.hxx"
#include "..\\..\\INC\\RecipeFile.hxx"
#include "..\\..\\INC\\Utility.hxx"
#include "..\\..\\INC\\StageInt.hxx"
#include "..\\..\\INC\\StageErr.hxx"
#include "..\\..\\INC\\NSStage.hxx"
#include "..\\..\\INC\\AlarmIf.hxx"
#include "..\\..\\INC\\PifComm.hxx"
#include "..\\..\\INC\\PatternFile.hxx"
#include "GridCtrl.h"
#include "BtnST.h"
#include "BkDialogST.h"
#include "ShadeButtonST.h"
#include "CheckData.h"
#include "LogFile.h"
#include "StatusBarEx.h"
#include "ChiefExports.h"


/////////////////////////////////////////////////////////////////////////////
// LIB																	   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\LIB\\dTnsActive.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsConfigFile.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsRecipeFile.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsUtility.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsStage.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsMeaSys.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsHook.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsPatRec.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsOverlay.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsAlarmIf.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsPifComm.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsNexIo.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsNexIf.lib")
#pragma comment(lib, "..\\..\\LIB\\dtnsnexproc.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsDeskew.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsPatternFile.lib")
#pragma comment(lib, "..\\..\\LIB\\dTnsNSStage.lib")
#else
#pragma comment(lib, "..\\..\\LIB\\TnsActive.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsConfigFile.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsRecipeFile.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsUtility.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsStage.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsMeaSys.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsHook.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsPatRec.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsOverlay.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsAlarmIf.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsPifComm.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsNexIo.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsNexIf.lib")
#pragma comment(lib, "..\\..\\LIB\\Tnsnexproc.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsDeskew.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsPatternFile.lib")
#pragma comment(lib, "..\\..\\LIB\\TnsNSStage.lib")
#endif// _DEBUG

#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "msimg32.lib")

/////////////////////////////////////////////////////////////////////////////
// Define																   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
//#define CHIEF_USE_SLEEP		// ChiefのハードシミュレートモードでSleepを入れる場合は有効にする

/////////////////////////////////////////////////////////////////////////////
// Nanosepc Version Definition
#define SOFT_VERSION					"Ver.2.3.0"
/* added 2016.05.12 hmenjo 6500/TS3100 別 version ---------- { ---------- */
#define SOFT_VERSION_TS3100				"Ver.1.11.16"
/* added 2016.05.12 hmenjo 6500/TS3100 別 version ---------- } ---------- */

/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- { ---------- */
#define	SPLTR_SHRSCRL	/* 分割ウィンドウの共有スクロールモード	*/
/* added 2015.04.05 hmenjo FWXGA 対応2 ---------- } ---------- */
/////////////////////////////////////////////////////////////////////////////
// Window Message Definition
/*
// WM番号割当表
	WM_NANOEXE		(WM_APP + 0x0000)		// 2009.03.26 NanoSpec.exe
	WM_KEYHOOK	  	(WM_APP + 0x0100)		// 2009.03.26 KeyHook
	WM_USER_NEX		(WM_APP + 0x0200)		// 2009.03.26 Nextra
	WM_MEAS			(WM_APP + 0x0300)		// 2009.03.26 MeaSys（測定・解析関連）
	WM_PIFCOMM		(WM_APP + 0x0400)		// 2009.03.26 Pif（上位通信）
	WM_ALARMIF		(WM_APP + 0x0500)		// 2009.03.26 アラーム処理
	WM_DESKEWSEQ	(WM_APP + 0x0600)		// 2009.03.26 デスキュー
	WM_SITEPT		(WM_APP + 0x0700)		// 2009.05.28 サイトパターン
	WM_TITLE		(WM_APP + 0x0800)		// 2009.12.14 NanoSpecTitle
*/


/////////////////////////////////////////////////////////////////////////////
// Window Message Definition
// (0x0000 - 0x00FF)
#define WM_NANOEXE									(WM_APP + 0x0000)
#define WM_EXE_WND_ENTRY							(WM_NANOEXE + 0x00)
#define WM_EXE_WND_EXIT 							(WM_NANOEXE + 0x01)
#define WM_RECIPE_SAVE_AS							(WM_NANOEXE + 0x02)
#define WM_ALARM_HIS_ADD							(WM_NANOEXE + 0x03)
#define WM_MEAS_COMP_EVENT							(WM_NANOEXE + 0x10)
#define WM_MEAS_START_EVENT 						(WM_NANOEXE + 0x11)
#define WM_MEAS_ERROR_EVENT 						(WM_NANOEXE + 0x12)
#define WM_MANUAL_START_EVENT						(WM_NANOEXE + 0x13)
#define WM_RETURN_KEY_DOWN							(WM_NANOEXE + 0x20)
#define WM_GRID_KEY_DOWN							(WM_NANOEXE + 0x21)
#define WM_DO_MEASURE								(WM_NANOEXE + 0x30)
#define WM_DO_CANCEL								(WM_NANOEXE + 0x31)
#define WM_DO_PAUSE 								(WM_NANOEXE + 0x32)
#define WM_DO_RESUME								(WM_NANOEXE + 0x33)
#define WM_SELLENSCHANGE							(WM_NANOEXE + 0x40)
// 2009.11.16 K.Matsuo ヘッドオフセット対応 -->
#define WM_SELHEADTYPECHANGE						(WM_NANOEXE + 0x41)
// 2009.11.16 K.Matsuo ヘッドオフセット対応 <--
// 2013.02.22 bagus Substrate thickness setting -->
#define WM_SELSUBTHICKCHANGE						(WM_NANOEXE + 0x42)
// 2013.02.22 bagus Substrate thickness setting <--
#define WM_LINE_SELECT_CHANGE						(WM_NANOEXE + 0x50) //wparam: チェック状態, lparam: 未使用
#define WM_NANO_MEASURE_GRAPH_CALIBRATION_COMP		(WM_NANOEXE + 0x51) //キャリブレーション校正値設定完了
#define WM_CHIF_HEARTBEAT							(WM_NANOEXE + 0x52) // Chief 動作中表示用
#define WM_CHIF_SETTIME_FAIL						(WM_NANOEXE + 0x53) // 時刻設定失敗
#define WM_CHIF_RECV_PIFCMD 						(WM_NANOEXE + 0x54) // Pif コマンド受信を通知
#define WM_CHIF_UPDATESTATESDLG 					(WM_NANOEXE + 0x55) // ステータスモニタ更新
#define WM_CHIF_SENDTRACEDATA						(WM_NANOEXE + 0x56) // トレースデータ報告(送信)
#define WM_NANO_MEASURE_GRAPH_CALIBRATION_SET		(WM_NANOEXE + 0x57) //wparam: 設定ポイント, lparam: 未使用	キャリブレーション校正値1ポイント設定完了

#define WM_CHIF_PIFDIREFRESH						(WM_NANOEXE + 0x58) // Pif DI リフレッシュ
#define WM_CHIF_EQRUNCHECK							(WM_NANOEXE + 0x59) // EQ 動作中チェック
#define WM_CHIF_REPORTALARM 						(WM_NANOEXE + 0x5a) // Chief 用各種アラーム処理用
#define WM_CHIF_ERROR_NOTIFY						(WM_NANOEXE + 0x5b) // Chief エラー通知用(CMainFrame へ)
#define WM_CHIF_MOVE_SETPOS 						(WM_NANOEXE + 0x5c) // 汎用ポジション移動指令(Pif から)
#define WM_DISP_MOVE_SETPOS 						(WM_NANOEXE + 0x5d) // 汎用ポジション移動指令(画面から)
#define WM_CHIF_MOVE_SETPOS_END 					(WM_NANOEXE + 0x5e) // 汎用ポジション移動完了(画面へ)
#define WM_CHIF_STAGE_STOP							(WM_NANOEXE + 0x5f) // ステージ停止検出
#define WM_DISP_VACUUM_ONOFF						(WM_NANOEXE + 0x60) // バキュームON/OFF指令(画面から)
#define WM_CHIF_VACUUM_END							(WM_NANOEXE + 0x61) // バキューム指令完了(画面へ)
#define WM_CHIF_SET_RECIPE							(WM_NANOEXE + 0x62) // 着工レシピ通知：Chief WM_COPYDATA ハンドラ → Chief レシピ設定ハンドラ
#define WM_CHIF_AF_POPUP							(WM_NANOEXE + 0x63) // AF ポップアップ指令
#define WM_DISP_AF_POPUP_END						(WM_NANOEXE + 0x64) // AF ポップアップ終了
//#define	WM_MEAS_RECVDATA							(WM_NANOEXE + 0x65) // データ受信開始	ref.MeaSys.hxx
#define WM_CHIF_1POINT_END							(WM_NANOEXE + 0x66) // １ポイント測定終了
#define WM_DATA_1POINT_END							(WM_NANOEXE + 0x67) // １ポイント測定終了応答
#define WM_CHIF_MEAS_END							(WM_NANOEXE + 0x68) // １枚測定終了
#define WM_DATA_MEAS_END							(WM_NANOEXE + 0x69) // １枚測定終了応答
#define WM_DISP_START_AAF							(WM_NANOEXE + 0x6a) // AAF 開始
#define WM_DISP_START_SEQ							(WM_NANOEXE + 0x6b) // シーケンス測定開始
#define WM_DISP_START_1POINT						(WM_NANOEXE + 0x6c) // １ポイント測定開始
#define WM_DISP_PAUSE_SEQ							(WM_NANOEXE + 0x6d) // 測定ポーズボタン
#define WM_DISP_RESUME_SEQ							(WM_NANOEXE + 0x6e) // 測定レジュームボタン
#define WM_DISP_CANCEL_SEQ							(WM_NANOEXE + 0x6f) // 測定キャンセルボタン
#define WM_CHIF_AAF_END 							(WM_NANOEXE + 0x70) // AAF終了(画面へ)
#define WM_DISP_START_SRREF 						(WM_NANOEXE + 0x71) // リファレンス測定開始(画面から)
#define WM_CHIF_SRREF_END							(WM_NANOEXE + 0x72) // リファレンス測定終了(画面へ)
#define WM_CHIF_SHOWSW								(WM_NANOEXE + 0x73) // Chiefダイアログ表示制御
#define WM_CHIF_RESET_ALARM 						(WM_NANOEXE + 0x74) // 異常リセット
#define WM_CHIF_MEAS_START							(WM_NANOEXE + 0x75) // １枚測定開始(シーケンス測定開始)
#define WM_CHIF_NOTIFY_MEAS_PAUSE					(WM_NANOEXE + 0x76) // 測定ポーズを通知
// 2009.07.29 K.Matsuo -->
#define WM_MANUALSITE_POPUP_END						(WM_NANOEXE + 0x77) // マニュアルサイトパターンポップアップ終了
// 2009.07.29 K.Matsuo <--
/* added 2009.07.24 hmenjo ストレス ウィンドウメッセージ追加 ---------- { ---------- */
#define	WM_CHIF_LINE_START							(WM_NANOEXE + 0x78) /* Stress ライン動作開始	*/
//#define	WM_MEAS_LINE_END							(WM_NANOEXE + 0x79) /* Stress １ライン測定完了(測定モジュールが発行)	 WPARAM  =0:正常終了 ≠0:異常終了 */	ref.MeaSys.hxx
#define	WM_CHIF_LINE_END							(WM_NANOEXE + 0x7a) /* Stress １ライン測定完了(画面/データ処理モジュールへ)	*/
#define	WM_DATA_LINE_END							(WM_NANOEXE + 0x7b) /* Stress １ラインデータ処理完了(画面/データ処理モジュールが発行)	*/
#define	WM_CHIF_STRS_MEAS_END						(WM_NANOEXE + 0x7c) /* Stress １枚測定終了	*/
#define	WM_DATA_STRS_MEAS_END						(WM_NANOEXE + 0x7d) /* Stress １枚測定終了応答(画面/データ処理モジュールが発行)	*/
/* added 2009.07.24 hmenjo ストレス ウィンドウメッセージ追加 ---------- } ---------- */
// 2009.10.28 bagus 2point-distance --{--
#define WM_CHIF_DISTANCE_POPUP						(WM_NANOEXE + 0x7e) // 2点間距離 PointTeaching画面のポップアップ要求
#define WM_DISP_DISTANCE_POPUP_END					(WM_NANOEXE + 0x7f) // 2点間距離 PointTeaching画面のポップアップ終了通知
// 2009.10.28 bagus 2point-distance --}--
// 2009.11.03 bagus MS --{--
#define WM_CHIF_MS_POPUP							(WM_NANOEXE + 0x80)	// 顕微鏡測定 PointTeaching画面のポップアップ要求
#define WM_DISP_MS_POPUP_END						(WM_NANOEXE + 0x81)	// 顕微鏡測定 PointTeaching画面のポップアップ終了通知
// 2009.11.03 bagus MS --}--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
#define WM_DISP_CONFIRM_POPUP 						(WM_NANOEXE + 0x82) // T3100 リファレンス測定位置ユーザ確認要求
#define WM_DISP_CONFIRM_POPUP_END					(WM_NANOEXE + 0x83) // T3100 リファレンス測定位置ユーザ確認結果通知
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// bagus 2014.01.10 Add(Stage None対応) -->
#define WM_PIF_REPORT_SI_REF_COMP					(WM_NANOEXE + 0x84) // Stage None対応 リファンレンス取得完了報告
#define WM_PIF_REPORT_DARK_REF_COMP					(WM_NANOEXE + 0x85) // Stage None対応 ダークリファンレンス取得完了報告
// bagus 2014.01.10 Add(Stage None対応) <--
// WM_COPYDATA で使う識別番号(dwData)を定義します
#define WMCD_CHIF_RECIPE_NOTIFY 					1	// Pif からの着工レシピ通知：Chief 受信スレッド→ Chief ダイアログ
#define WMCD_CHIF_SET_RECIPE						2	// 着工レシピ指示：Chief ダイアログ → 画面モジュール
#define WMCD_CHIF_1POINT_END						3	// １ポイント測定終了：Chief → データ処理モジュール
#define WMCD_CHIF_MEAS_START						4	// １枚測定開始(シーケンス測定開始)：Chief → データ処理モジュール

#define WM_PROCESS_INIT 							(WM_NANOEXE + 0xA0)
#define WM_UNIT_INIT_RETRY							(WM_NANOEXE + 0xA1)
#define WM_EXE_DLG_DISPLAY							(WM_NANOEXE + 0xA2)

#define WM_DISPMENU_MEASUREMENT_START				(WM_NANOEXE + 0xB0) //
#define WM_DISPMENU_MANUAL_MEASUREMENT				(WM_NANOEXE + 0xB1) //
#define WM_DISPMENU_MEASUREMENT_CANCEL				(WM_NANOEXE + 0xB2) //
#define WM_DISPMENU_MEASUREMENT_PAUSE				(WM_NANOEXE + 0xB3) //
#define WM_DISPMENU_MEASUREMENT_RESUME				(WM_NANOEXE + 0xB4) //
#define WM_DISPMENU_REMEASUREMENT					(WM_NANOEXE + 0xB5) //
#define WM_DISPMENU_ADUJST_REMEASUREMENT			(WM_NANOEXE + 0xB6) //
#define WM_DISPMENU_SEQMEAS_EXIT					(WM_NANOEXE + 0xB7) //
#define WM_DISPMENU_MANUAL_MEASUREMENT_START		(WM_NANOEXE + 0xB8) //
#define WM_DISPMENU_MEASUREMENT_COMPLETE			(WM_NANOEXE + 0xB9) //
#define WM_DISPMENU_POINT_TEACHING					(WM_NANOEXE + 0xBA) //
#define WM_DISPMENU_MANUMEAS_EXIT					(WM_NANOEXE + 0xBB) //
#define WM_DISPMENU_HANDLER_LOAD_POSITION			(WM_NANOEXE + 0xBC) //
#define WM_DISPMENU_MANUAL_STAGE					(WM_NANOEXE + 0xBD) //
#define WM_DISPMENU_VACUUM							(WM_NANOEXE + 0xBE) //
#define WM_DISPMENU_REFERENCE						(WM_NANOEXE + 0xBF) //

#define WM_TESTMODE_ONEPOINT_END					(WM_NANOEXE + 0xC0) // テストモード1ポイント測定終了通知(TestMeasurementDlgへ通知)
#define WM_TESTMODE_REF_END 						(WM_NANOEXE + 0xC1) // テストモードリファレンス測定終了通知(TestMeasurementDlgへ通知)

#define WM_JOYSTICK 								(WM_NANOEXE + 0xD0)
#define WM_JOYSTICK_NG								(WM_NANOEXE + 0xD1)

#define WM_MESSAGE_SHOW 							(WM_NANOEXE + 0xE0)

#define WM_DEB_MESSAGE								(WM_NANOEXE + 0xF0) // デバック用

/////////////////////////////////////////////////////////////////////////////
// Window Message Definition
// (0x0600 - 0x06FF)
#define WM_DESKEWSEQ								(WM_APP + 0x0600)
#define WM_DESKEWSEQ_DESKEWMANUAL_START 			(WM_DESKEWSEQ + 0x00)	// マニュアルデスキュー開始
#define WM_DESKEWSEQ_DESKEWMANUAL_SITE1_OK			(WM_DESKEWSEQ + 0x01)	// マニュアルデスキュー終了 => DESKEWMANUAL SITE2へ
#define WM_DESKEWSEQ_DESKEWMANUAL_SITE2_OK			(WM_DESKEWSEQ + 0x02)	// マニュアルデスキュー終了 => DESKEW OK
#define WM_DESKEWSEQ_DESKEWMANUAL_CANCEL			(WM_DESKEWSEQ + 0x03)	// マニュアルデスキュー終了 => レシピ終了
#define WM_DESKEWSEQ_DESKEWMANUAL_NONE				(WM_DESKEWSEQ + 0x04)	// マニュアルデスキュー終了 => DESKEWなしで処理続行

/////////////////////////////////////////////////////////////////////////////
// Window Message Definition
// (0x0700 - 0x07FF)
#define WM_SITEPT									(WM_APP + 0x0700)
#define WM_EXECUTION_START							(WM_SITEPT + 0x01)
#define WM_EXECUTION_END							(WM_SITEPT + 0x02)

// 2009.05.16 myanagida -->
//#define WM_SITEPTSEQ
#define WM_SITEPTSEQ_SITEPTMANUAL_START 			(WM_SITEPT + 0x10)	// マニュアルサイトパターン開始
#define WM_SITEPTSEQ_SITEPTMANUAL_OK				(WM_SITEPT + 0x11)	// マニュアルサイトパターン終了
#define WM_SITEPTSEQ_SITEPTMANUAL_CANCEL			(WM_SITEPT + 0x12)	// マニュアルサイトパターン終了 => レシピ終了
#define WM_SITEPTSEQ_SITEPTMANUAL_NONE				(WM_SITEPT + 0x13)	// マニュアルサイトパターン終了 => サイトパターンなしで処理続行
// 2009.05.16 myanagida <--

/////////////////////////////////////////////////////////////////////////////
// main data base location
// Recipe Sr Measurement Parameters
#define MIN_TARGET_WAVELENGTH			380
// 2009.05.15 k-matsuo -->
//#define MAX_TARGET_WAVELENGTH			800
#define MAX_TARGET_WAVELENGTH			850			// スモールスポット対応。通常の最大値といえば、こちらを使用する
#define MAX_TARGET_WAVELENGTH_800		800			// 画面表示等で、800nmまでにしておきたい場合など、
													// 以前との互換性を考慮する際に使用する
// 2009.05.15 k-matsuo <--
#define MIN_ENHANCED_UV_WAVELENGTH		200
#define MIN_FILM_THICKNESS				1			//um
#define MAX_FILM_THICKNESS				10000		//um
#define MIN_INTEGRATION_TIME			0.01
#define MAX_INTEGRATION_TIME			25.00
#define MIN_GAMMA						0.1
#define MAX_GAMMA						10.0
#define MIN_LIFE_TIME					1.0
#define MAX_LIFE_TIME					10080.0 	//秒
#define CALC_TYPE_ITEM_MAX				2

// Config Setting Parameters
#define MIN_Z_ENCORDER					0.0
#define MAX_Z_ENCORDER					999999.0
#define MIN_SPEED						0.1
#define MAX_SPEED						500
#define MIN_UV_GAIN 					0.0
#define MAX_UV_GAIN 					100.0
#define MIN_FACTOR						0.0
#define MAX_FACTOR						100.0
#define MIN_MATCH_SCORE 				20
#define MAX_MATCH_SCORE 				100
#define MIN_VIEW_RANGE					0
#define MAX_VIEW_RANGE					640
#define MIN_VACCUM_TIMEOUT				10
#define MAX_VACCUM_TIMEOUT				10000
#define MIN_TURRET_ANGLE				0
#define MAX_TURRET_ANGLE				90

// 2009.11.10 bagus MS --{--
#define MICROSCOPE_ORIGIN_POSITION_NO	(0)
#define MICROSCOPE_UPPER_POSITION_NO	(63)
// 2009.11.10 bagus MS --}--

// 2009.09.28 bagus gantry --{--
#if 0
// 2009.09.19 bagus SE --{--
#define	MIN_WAITTIME_SHUTTER_MOVING		0.0
#define	MAX_WAITTIME_SHUTTER_MOVING		1000.0
#define	MIN_GANTRY_MEAS_POINT_NUM		0
#define	MAX_GANTRY_MEAS_POINT_NUM		1000
// 2009.09.19 bagus SE --}--
#endif
// 2009.09.28 bagus gantry --}--

//SetNanoOffsetCoordinate Mode Parameters
#define GENERAL_MODE					0		//ロード/汎用ポジションモード ： X,Y のみ
#define MEASURESEQUENCE_MODE			1		//測定ポイントモード ： X,Y,Z,メインレシピ名
#define REFERENCESEQUENCE_MODE			2		//リファレンス測定座標モード ： X,Y,Z
#define LENSSELECT_MODE 				10		//レンズ間オフセット補正座標モード ： X,Y,Z

/* added 2015.06.01 hmenjo XMP 表示/非表示２ ---------- { ---------- */
/*	XMP 表示チェック用タイマ定義	*/
#define	TMR_CHKXMPSHOW_ID	101
#define	TMR_CHKXMPSHOW_TIME	500
/* added 2015.06.01 hmenjo XMP 表示/非表示２ ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// Other Definition
#define TIME_FORMAT 					"%H:%M:%S"			// time format

/////////////////////////////////////////////////////////////////////////////
// Toolbarの高さ
#define TOOLBAR_HEIGHT					(20)

// 2009.09.11 bagus stress 修正 --{--
/////////////////////////////////////////////////////////////////////////////
// Section の最低距離
#define	SECTION_MIN_RANGE				(100 * MICROMETRE)
// 2009.09.11 bagus stress 修正 --}--

/////////////////////////////////////////////////////////////////////////////
// Color Definition
#define WHITE_COLOR 					RGB(255,255,255)
#define RED_COLOR						RGB(255,  0,  0)
#define GREEN_COLOR 					RGB(  0,255,  0)
#define BLUE_COLOR						RGB(  0,  0,255)
#define YELLOW_COLOR					RGB(255,255,  0)
#define MAGENTA_COLOR					RGB(255,  0,255)
#define CYAN_COLOR						RGB(  0,255,255)
#define EXIST_COLOR 					RGB(  0,128,255)
#define BUTTON_COLOR					GetSysColor(COLOR_BTNFACE)
#define HOST_MESSAGE_COLOR				RGB( 19,253,131)
#define GRAY_COLOR						RGB(128, 128, 128)
#define LIGHT_GRAY_COLOR				RGB(210,210,210)
#define DARK_GRAY_COLOR 				RGB(100,100,100)
#define SIGNAL_ON_COLOR 				RGB(155,155,255)
#define REFERENCE_COLOR 				RGB(104,235,141)
#define GLASS_EXIST_COLOR				RGB(  0,221,  0)
#define WATER_COLOR 					RGB(210,255,255)//(151,255,255)//RGB(  0,255,255)
#define GRID_CELL_COLOR 				RGB(0xFF, 0xFF, 0xE0)
#define LIGHT_BLUE_COLOR				RGB(  2,171,253)
#define BLACK_COLOR 					RGB(  0,  0,  0)
#define WAFER_COLOR 					RGB(210,255,255)
#define DARK_GREEN_COLOR				RGB(  0, 0xCC,0)
#define PURPLE_COLOR					RGB(153,  0,153)

/////////////////////////////////////////////////////////////////////////////
// ボタンの外観変更のパラメータの定義
#define BUTTON_NORMAL_SHADEID			CShadeButtonST::SHS_HBUMP
#define BUTTON_RADIO_SHADEID			CShadeButtonST::SHS_HARDBUMP
#define BUTTON_GRANULARITY				(5)
#define BUTTON_HIGHLIGHT				(20)
#define BUTTON_COLORING 				(15)
#define BUTTON_NORMAL_COLOR 			BLUE_COLOR
#define BUTTON_RADIO_ON_COLOR			GREEN_COLOR
#define BUTTON_RADIO_OFF_COLOR			WHITE_COLOR
#define BUTTON_START_COLOR				GREEN_COLOR

/////////////////////////////////////////////////////////////////////////////
// Buffer Length
#define STRFUNC_BUF_LEN 				600

/////////////////////////////////////////////////////////////////////////////
// USER_ACCOUNT
#define USER_ACCOUNT_TOHO_ENGINEER		"TOHO"
#define USER_ACCOUNT_TOHO_PASSWORD		"Mainte"

/////////////////////////////////////////////////////////////////////////////
// System Reserve
#define SYSTEM_RECIPE_BEGINNING_CHAR	'_' 	// アンダーバーから始まるレシピ名は、システム利用

#define TESTMODE_TEMP_RECIPE_NAME		"TestMode"						// テストモード用の一時作成レシピ名(（SYSTEM_RECIPE_BEGINNING_CHAR）を接頭語にして利用する)

//Saiki 20091211 Add ----->
// ==========================================================================
// File Retention
// ==========================================================================
#define FILE_RETENTION_MAX				(10)

//Saiki 20091211 Add <-----
/////////////////////////////////////////////////////////////////////////////
// Signal
//#define ON								(1)
//#define OFF								(0)

/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- { ---------- */
/////////////////////////////////////////////////////////////////////////////
//	FIT 値の表示仕様(scout 使用時)
//		0 : 小数点以下６桁  (VC 言語の標準仕様)
//		1 : 仮数部３桁，指数部３桁  (VC 言語の標準仕様)
//		2 : 仮数部２桁，指数部３桁  (VC 言語の標準仕様)
//		3 : 仮数部２桁，指数部２桁  (log10 を使って分解表示．計算誤差が不安)
//		上記以外：0 と同じ
#define	FIT_DISP_SCOUT	3
/* added 2014.11.22 hmenjo Fit 指数表示(2) ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// Structure															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
struct ONE_POINT_DATA
{
	BOOL Valid;
	STAGE_COORD_XYZ xyz;
	AF_VALID	afValid;												// AF VALIDデータ
	char szAfP502[2+1]; 												// P502送信用
	char szShowAfValid[SHOW_AF_VALID_LEN+1];							// 測定結果表示画面のFOCUS項目用
	double dData[ADAPRESULT_COLS_MAX];									// Colデータ(AFよりも後ろのデータ)
// 2009.09.30 K.Matsuo CommentOut -->
//	double dAnalysisData[SCANDATA_POINT_MAX];							// 反射率データ(379～799間を使用。それ以外は常に0)
//	double dAnalysisGenData[SCANDATA_POINT_MAX];						// XmpからのGenData(379～799間を使用。それ以外は常に0)
// 2009.09.30 K.Matsuo CommentOut <--
};
/* added 2009.07.23 hmenjo ストレス ライン数 定義２ ---------- { ---------- */
// 2009.08.05 K.Matsuo CommentOut -->
//struct STRESSRESULT {				/* ココはダミーです．		*/
//	long dummy;						/*	 「測定モジュール」で	*/
//};									/*		 定義されています．	*/
// 2009.08.05 K.Matsuo CommentOut <--
/* ストレス用測定結果データ構造*/
struct ONE_LINE_DATA_STRESS {
	STRESSRESULT		StressResult;					/* １ライン測定結果	*/
	STRESS_SECTION_POS	SecPos[STRESS_SECTIONS_MAX];	/* セクション位置情報 定義	*/
};
/* added 2009.07.23 hmenjo ストレス ライン数 定義２ ---------- } ---------- */


struct TEST_MODE_MEASRCP_CONDITION
{
	int 			iLens;				// 測定レンズ
	WORD			wOpticsFilterType;	// 光学フィルタ
	double			dIntegTime; 		// 測定時間
	WORD wStart;
	WORD wEnd;
};


#if 0 // 2014.04.04 bagus wavelength step modified -->
// struct TEST_MODE_DATA
// {
// 	TEST_MODE_MEASRCP_CONDITION MeasRcpCon;
// 	double dScanSampleData[SCANDATA_POINT_MAX]; 						//(379～799間を使用。それ以外は常に0)
// 	double dScan1stReferenceData[SCANDATA_POINT_MAX];					//(379～799間を使用。それ以外は常に0)
// 	double dScanDark_T1Data[SCANDATA_POINT_MAX];						//(379～799間を使用。それ以外は常に0)
// };
#else
struct TEST_MODE_DATA
{
	TEST_MODE_MEASRCP_CONDITION MeasRcpCon;
	double dMinTargetWave;
	double dMaxTargetWave;
	double dWavelengthStep;
	double dScanSampleData[SCANDATA_POINT_MAX]; 				 // 波長から配列インデックスを求める (波長 - dMinTargetWave) / dWavelengthStep
	double dScan1stReferenceData[SCANDATA_POINT_MAX];			 // 波長から配列インデックスを求める (波長 - dMinTargetWave) / dWavelengthStep
	double dScanDark_T1Data[SCANDATA_POINT_MAX];				 // 波長から配列インデックスを求める (波長 - dMinTargetWave) / dWavelengthStep
	// helper function.
	double GetScanSampleData(double dWavelength)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)dMinTargetWave) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)dMinTargetWave));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return 0.0;
		}

		return dScanSampleData[iIndex];
	}

	void SetScanSampleData(double dWavelength, double dData)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)dMinTargetWave) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)dMinTargetWave));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return;
		}

		dScanSampleData[iIndex] = dData;
	}

	double GetScan1stReferenceData(double dWavelength)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)dMinTargetWave) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)dMinTargetWave));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return 0.0;
		}

		return dScan1stReferenceData[iIndex];
	}

	void SetScan1stReferenceData(double dWavelength, double dData)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)dMinTargetWave) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)dMinTargetWave));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return;
		}

		dScan1stReferenceData[iIndex] = dData;
	}

	double GetScanDark_T1Data(double dWavelength)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)dMinTargetWave) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)dMinTargetWave));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return 0.0;
		}

		return dScanDark_T1Data[iIndex];
	}

	void SetScanDark_T1Data(double dWavelength, double dData)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)dMinTargetWave) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)dMinTargetWave));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return;
		}

		dScanDark_T1Data[iIndex] = dData;
	}
};
#endif // 2014.04.04 bagus wavelength step modified <--

struct STATISTICS
{
	BOOL Valid;
	double dMinimum;
	double dMaximum;
	double dAverage;
	double dStandardDeviation;
	double dRange;

};

struct RCP_DATA
{

	char szRecipeName[RECIPE_NAME_LEN + 1]; 							// メインレシピ名
	//	MAIN_RCP_INFO_HDR hdr;
	MAIN_RCP_INFO MainRcpInfo;
	MEAS_PROG_INFO MeasProgInfo;
	STAGE_PROG_INFO_HDR StageProgInfoHdr;
	STAGE_COORD ScanPoint[SCAN_POINT_MAX];
	//2009.08.21 bagus stress --{--
	STAGE_PROG_STRESS	StressStageProgInfo;
	//2009.08.21 bagus stress --}--

};


struct MEASURED_DATA
{
	RCP_DATA rcp_data;													// レシピデータなど

	char szUserId[MAX_PATH + 1];										// ユーザID
	SYSTEMTIME syStartDateTime; 										// 測定開始日時(datファイル格納時に分解すること)
	SYSTEMTIME syEndDateTime;											// 測定終了日時(datファイル格納時に分解すること)
	char szSampleId[SAMPLE_ID_LEN + 1]; 								// サンプルID(HOSTより指示)
	//2009.10.13 bagus RS --{--
	//ロットID追加
	char szLotId[LOT_ID_LEN + 1];										// ロットID（RSのみ必須）
	//2009.10.13 bagus RS --}--
	char szUnit[DISP_UNIT_NAME_LEN + 1];								// 測定結果単位名
	int iUnit;															// 測定結果単位INDEX
	int iMeasureNum;													// 測定ポイント数

	char szTitleXyz[3][ADAPRESULTSTRINGLENMAX + 1]; 					// グリッドのタイトル(X,Y,Z)
	char szTitleAf[AF_HEADER_LEN + 1];									// グリッドのタイトル(AF)
	char szLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];		// グリッドのタイトル ←X,Y,Z,AFは含まず
	char szDispLabel[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];	//画面表示(データ保存)用タイトル ←X,Y,Z,AFは含まず
	char szLabelOri[ADAPRESULT_COLS_MAX][ADAPRESULTSTRINGLENMAX + 1];	// DDE受信のタイトル ←X,Y,Z,AFは含まず

	BOOL bLabelFlag;

	int iLabelCount;													// グリッドのタイトル数 ←X,Y,Z,AFは含まず
	int iDispLabelCount;			// グリッドのタイトル数(画面表示・保存用) ←X,Y,Z,AFは含まず
	ONE_POINT_DATA OnePointData[SCAN_POINT_MAX];						// 1ポイント分の測定データ
	STATISTICS Statistics[ADAPRESULT_COLS_MAX]; 						// 統計データ
	int iStatisticsValidPointCount; 									//統計データ有効Point数(統計データに加えるPoint数)
	TEST_MODE_DATA TestModeData[TESTDATA_POINT_MAX];					// テストモード用データ

	int iSetPointCount; 												// 格納済ポイント数
																		// (==有効データポイント数)…この数値分までが有効データとなるように記述すること！)
	int iShowCompletionPointList;										//画面に表示済みのポイント数(List用)
	int iShowCompletionPointGraph;										//画面に表示済みのポイント数(Graph用)

	//2009.08.17 bagus stress --{--
	BOOL			StressDataValid[STRESS_LINES_MAX];					// 測定処理の終わったデータが格納されているかのフラグ
	BOOL			StressDataRef[STRESS_LINES_MAX];					// リファレンス測定かどうか
	STRESSRESULT	StressData[STRESS_LINES_MAX];						//１ライン分のストレス測定データ
	int iShowCompletionStressLineList;									// 画面に表示済みのライン数(List用)
	int iShowCompletionStressLineGraph;									// 画面に表示済みのライン数(Graph用)
	STATISTICS		StressLineStatistics[STRESS_LINES_MAX];				// ストレスデータの測定統計情報
	STATISTICS		StressStatistics;									// 全ライン合計の統計情報
	//2009.08.17 bagus stress --}--

	//2009.10.28 bagus stress --{--
	STAGE_COORD		LastDistancePoint;									// 2点間距離の最終設定座標
	//2009.10.28 bagus stress --}--

	//2009.11.04 bagus CA --{--
	CTARESULT		ContactAngleData[SCAN_POINT_MAX];					// CA測定結果データ保存
	//2009.11.04 bagus CA --}--
	//2009.11.10 bagus RS --{--
	RESISTRESULT	ResistData[SCAN_POINT_MAX];							// 抵抗測定データ保存
	//2009.11.10 bagus RS --}--

};

//Saiki 20091211 Add ----->
// ==========================================================================
// Timer
// ==========================================================================
typedef struct{
	BOOL	Valid;
	DWORD	Start;
	DWORD	Estimate;
	DWORD	Value;
	BOOL	Up;
}TIMER_STATUS;
//Saiki 20091211 Add <-----

//2009.09.15 bagus se --{--
//SEの生データ1個分の構造体
typedef struct {
	BOOL bEb;															//Ebが存在するかどうか
	BOOL bValidModelData;												//モデルデータに入っている値が有効な値かどうか
	double dWaveLength;													//測定波長[nm]
	double dLightInputAngle;											//SE光源入射角度[deg]
	double dPsi;														//Psi
	double dDelta;														//Delta
	double dPsiModel;													//
	double dDeltaModel;													//
	double dPsiSD;														// Psi S.D
	double dDeltaSD;													// Delta S.D
	double dPsiSDModel;													// Psi S.D
	double dDeltaSDModel;												// Delta S.D
} SE_RAW_ONE_DATA;
//typedef CArray<SE_RAW_ONE_DATA,SE_RAW_ONE_DATA&>	CSERawDataList;		//データ読み出し時の保存List
typedef CMap<double,double&,SE_RAW_ONE_DATA,SE_RAW_ONE_DATA&> CSERawDataList;

/////////////////////////////////////////////////////////////////////////////
// Class Define 														   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
class CMainFrame;
class CNanoSpecDoc;
class CAlarm;
class CAlarmLogDlg;
class CInterlockDlg;
class CNanoSpecTitleDlg;
class CDebugDlg;
class CMessageDlg;
class CSitePatternEditDlg;
//Saiki 20091211 Add ----->
class CFileRetentionPeriod;
//Saiki 20091211 Add <-----
/////////////////////////////////////////////////////////////////////////////
// Class Pointer														   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

_EXTERN CAlarmLogDlg*					g_pAlarmLogDlg;
_EXTERN CInterlockDlg*					g_pInterlockDlg;
_EXTERN CMessageDlg*					g_pMessageDlg;
_EXTERN CSitePatternEditDlg*			g_pSitePatternEditDlg;
//Saiki 20091211 Add ----->
_EXTERN CFileRetentionPeriod*			g_pFileRetentionPeriod[FILE_RETENTION_MAX];
//Saiki 20091211 Add <-----
/////////////////////////////////////////////////////////////////////////////
// Variable 															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
// ==========================================================================
// Application Name Type
// ==========================================================================
_EXTERN long	g_lAppNameType;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

// ==========================================================================
// Path
// ==========================================================================
_EXTERN char	g_szMyPath[MAX_PATH];
_EXTERN char	g_szCurPath[MAX_PATH];
_EXTERN char	g_szBin_Dir[MAX_PATH];											// #define BIN_DIR
_EXTERN char	g_szLog_Dir[MAX_PATH];											// #define LOG_DIR
_EXTERN char	g_szCfg_Dir[MAX_PATH];											// #define CFG_DIR
_EXTERN char	g_szCfg_System_Dir[MAX_PATH];									// #define CFG_SYSTEM_DIR
_EXTERN char	g_szCfg_System_Sample_Dir[MAX_PATH];							// #define CFG_SYSTEM_SAMPLE_DIR
_EXTERN char	g_szCfg_System_Sr_Reference_Material_Dir[MAX_PATH]; 			// #define CFG_SYSTEM_SR_REFERENCE_MATERIAL
_EXTERN char	g_szCfg_User_Dir[MAX_PATH]; 									// #define CFG_USER_DIR
_EXTERN char	g_szCfg_User_User_Account_Dir[MAX_PATH];						// #define CFG_USER_USER_ACCOUNT_DIR
_EXTERN char	g_szDb_Dir[MAX_PATH];											// #define DB_DIR
_EXTERN char	g_szDb_Main_Recipe_Dir[MAX_PATH];								// #define DB_MAIN_RECIPE_DIR
_EXTERN char	g_szDb_Multi_Recipe_Dir[MAX_PATH];								// #define DB_MULTI_RECIPE_DIR
_EXTERN char	g_szDb_Measurement_Program_Dir[MAX_PATH];						// #define DB_MEASUREMENT_PROGRAM_DIR
_EXTERN char	g_szDb_Stage_Program_Dir[MAX_PATH]; 							// #define DB_STAGE_PROGRAM_DIR
_EXTERN char	g_szDb_Deskew_Img_Dir[MAX_PATH];								// #define DB_DESKEW_IMG_DIR
_EXTERN char	g_szDb_Pattern_Dir[MAX_PATH];									// #define DB_PATTERN_DIR
_EXTERN char	g_szDb_Pattern_Img_Dir[MAX_PATH];								// #define DB_PATTERN_IMG_DIR
_EXTERN char	g_szDb_Recalibration_Program_Dir[MAX_PATH]; 					// #define DB_RECALIBRATION_PROGRAM_DIR
_EXTERN char	g_szDb_Point_Deskew_Program_Dir[MAX_PATH];						// #define DB_POINT_DESKEW_PROGRAM_DIR
_EXTERN char	g_szResult_Dir[MAX_PATH];										// #define RESULT_DIR
_EXTERN char	g_szData_Dir[MAX_PATH]; 										// #define DATA_DIR							@@@
_EXTERN char	g_szData_MeasurementData_Dir[MAX_PATH]; 						// #define DATA_MEASUREMENTDATA_DIR
_EXTERN char	g_szData_Meas_Dat_Temp_Copy_Dir[MAX_PATH];						// #define MEAS_DAT_TEMP_COPY_PASS
_EXTERN char	g_szData_Measured_Data_Temp_Dir[MAX_PATH];						// #define MEASURED_DATA_TEMP_DIR
_EXTERN char	g_szData_Ref_Dir[MAX_PATH]; 									// #define REF_DIR
/* added 2009.07.31 hmenjo ストレス機能追加(9) ---------- { ---------- */
_EXTERN char	g_szData_StressRef_Dir[MAX_PATH];									  // #define REF_DIR
/* added 2009.07.31 hmenjo ストレス機能追加(9) ---------- } ---------- */
_EXTERN char	g_szLog_Pr_Dir[MAX_PATH];										// #define PR_DIR
//_EXTERN char	g_szDb_Pattern_Img_Temp_Dir[MAX_PATH];							// #define PATTERN_IMG_TEMP
//_EXTERN char	g_szDb_Pattern_Img_1_Dir[MAX_PATH]; 							// #define PATTERN_IMG_1
//_EXTERN char	g_szDb_Pattern_Img_2_Dir[MAX_PATH]; 							// #define PATTERN_IMG_2

// ==========================================================================
// NanoSpec.ini
// ==========================================================================

// --------------------------------------------------------------------------
// Hard Ware Simulation
_EXTERN BOOL	g_bHWS;

// --------------------------------------------------------------------------
// SrHeadSerial
_EXTERN char	g_szCommNo[MAX_PATH];
_EXTERN char	g_szBaudRate[MAX_PATH];
_EXTERN char	g_szParity[MAX_PATH];
_EXTERN char	g_szByteSize[MAX_PATH];
_EXTERN char	g_szStopBits[MAX_PATH];
_EXTERN char	g_szCcdDataTransferModeBinary[MAX_PATH];

// --------------------------------------------------------------------------
// StageConfigSettingDlg
_EXTERN BOOL	g_bHWStageSimulation;
_EXTERN long	g_lHWStageSizeX;
_EXTERN long	g_lHWStageSizeY;
_EXTERN long	g_lHWEdgeSizeX;
_EXTERN long	g_lHWEdgeSizeY;
_EXTERN long	g_lHWOriginOffsetX;
_EXTERN long	g_lHWOriginOffsetY;

// added hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- { ----------
// --------------------------------------------------------------------------
// EQ メンテナンス
_EXTERN BOOL	g_bIL_CycleStop;
// added hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- } ----------
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- { ----------
_EXTERN BOOL	g_bIL_DoorOpen;
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- } ----------
// added hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- { ----------
_EXTERN BOOL	g_bIL_SpeedDown;
// added hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- } ----------
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- { ---------- */
_EXTERN BOOL	g_bIL_EQPowerOff;
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- } ---------- */
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
_EXTERN BOOL	g_bIL_DoorOpenPause;
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- { ---------- */
// --------------------------------------------------------------------------
// サブコン電源再投入
_EXTERN long	g_lSBCPwr_OffTime;
_EXTERN long	g_lSBCPwr_WaitAfterOn;
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- } ---------- */

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// --------------------------------------------------------------------------
// モデルタイプ
_EXTERN long	g_lModelType;
// --------------------------------------------------------------------------
// ビデオ表示
_EXTERN long	g_lOVideoEnable;
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
_EXTERN char	g_szVideoFmtSize[128];
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
// 2013.11.08 Bagus Add (TohoSpec対応) <--
/* added 2014.05.22 hmenjo get Xmp Type ---------- { ---------- */
// --------------------------------------------------------------------------
// XMP タイプ
_EXTERN long	g_lXmpType;		/*	0 : jaw_adap.exe[WVASE]
									1 : SC_adap.exe[scout]
								*/
/* added 2014.05.22 hmenjo get Xmp Type ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// Constant Value														   //
//																		   //
/////////////////////////////////////////////////////////////////////////////
_EXTERN PROCESS_INFORMATION g_ProcessInfo;
// ==========================================================================
// Capture Boardの設定
enum CAPTURE_BORAD
{
	CAPTURE_BOARD_NONE = 0,
	CAPTURE_BOARD_DFGSV1,
	CAPTURE_BOARD_MAX,
};

const LPCSTR g_lpszCaptureBoard[] = {
	"None",
	"DFG/SV1",
};

// matsuhisa 20091209 modified >>>
// INC\Globals.hxx内で定義
// ==========================================================================
// Pattern Method (Image Board)の設定
#if 0
enum PR_METHOD
{
	PR_METHOD_NONE = 0,
// 2009.12.03 K.Matsuo -->
	PR_METHOD_MIL8_G,		// Geometric
	PR_METHOD_MIL8_M,		// ModelFinder
// 2009.12.03 K.Matsuo <--
	PR_METHOD_MAX,
};
#endif
// matsuhisa 20091209 modified <<<

const LPCSTR g_lpszPRMethod[] = {
	"None",
// 2009.12.03 K.Matsuo -->
	"Mil8.0 (G)",
	"Mil8.0 (M)",
// 2009.12.03 K.Matsuo <--
};

const LPCSTR g_lpszLanguage[] = {
	"English",
	"Japanese",
};

// ==========================================================================
// ステージタイプの設定
enum STAGE_TYPE
{
	STAGE_TYPE_NONE = 0,
	STAGE_TYPE_TOHO,
	STAGE_TYPE_NTN,
// added hmenjo 2009.05.15 新 Stage.dll 対応 ---------- { ----------
	STAGE_TYPE_SPT,		// ソディックステージ
// added hmenjo 2009.05.15 新 Stage.dll 対応 ---------- } ----------
	STAGE_TYPE_MAX,
};

const LPCSTR g_lpszStageType[] = {
	"None",
	"TOHO",
	"NTN",
// added hmenjo 2009.05.15 新 Stage.dll 対応 ---------- { ----------
	"SPT",		// ソディックステージ
// added hmenjo 2009.05.15 新 Stage.dll 対応 ---------- } ----------
};

// ==========================================================================
// ステージの中心(サブコンの中心)
enum STAGE_ORIGINAL_POSITION
{
	STAGE_ORIGINAL_POSITION_CENTER = 0,
	STAGE_ORIGINAL_POSITION_TOP_LEFT,
	STAGE_ORIGINAL_POSITION_TOP_RIGHT,
	STAGE_ORIGINAL_POSITION_BOTTOM_LEFT,
	STAGE_ORIGINAL_POSITION_BOTTOM_RIGHT,
	STAGE_ORIGINAL_POSITION_MAX,
};

const LPCSTR g_lpszStageOriginalPosition[] = {
	"Center",
	"Top Left",
	"Top Right",
	"Bottom Left",
	"Bottom Right",
};

// ==========================================================================
// 除震台の設定
enum VIBRATION_CONTROL_UNIT
{
	VIBRATION_CONTROL_UNIT_NONE = 0,
	VIBRATION_CONTROL_UNIT_TRUE,
	VIBRATION_CONTROL_UNIT_MAX,
};

const LPCSTR g_lpszVibrationControlUnit[] = {
	"None",
	"True",
};

// ==========================================================================
// Sample Lifter(アライメントピン)の設定
enum SAMPLE_LIFTER
{
	SAMPLE_LIFTER_NONE = 0,
	SAMPLE_LIFTER_TYPE1,
	SAMPLE_LIFTER_TYPE2,
	SAMPLE_LIFTER_MAX,
};

const LPCSTR g_lpszSampleLifter[] = {
	"None",
	"Type1",
	"Type2",
};

// ==========================================================================
// バキュームタイプの設定
enum VACUUM_TYPE
{
	VACUUM_TYPE_NONE = 0,
	VACUUM_TYPE_1,
	VACUUM_TYPE_2,
	VACUUM_TYPE_MAX,
};

const LPCSTR g_lpszVacuumType[] = {
	"None",
	"Type1",
	"Type2",
};

// 2009.10.29 bagus Jog 追加修正 --{--
// ==========================================================================
// バキュームタイプの設定
enum JOYSTICK_TYPE
{
	JOYSTICK_TYPE_NONE = 0,
	JOYSTICK_TYPE_XY,
	JOYSTICK_TYPE_XYZ,
	JOYSTICK_TYPE_MAX,
};

const LPCSTR g_lpszJoystickType[] = {
	"None",
	"XY",
	"XYZ",
};
// 2009.10.29 bagus Jog 追加修正 --}--

// ==========================================================================
// サブコンのナンバーの設定
/*enum SUB_CONTROLLER_NUMBER
{
	SUB_CONTROLLER_NUMBER_1 = 0,
	SUB_CONTROLLER_NUMBER_MAX,
};

const LPCSTR g_lpszSubControllerNumber[] = {
	"1",
};
*/

// ==========================================================================
// シグナルタワーの設定
enum SIGNAL_TOWER_LIGHT
{
	SIGNAL_TOWER_LIGHT_OFF,
	SIGNAL_TOWER_LIGHT_ON,
	SIGNAL_TOWER_LIGHT_BLINK,
	SIGNAL_TOWER_LIGHT_MAX,
};

const LPCSTR g_lpszSignalTowerLight[] = {
	"Off",
	"On",
	"Blink",
};

// ==========================================================================
// Buzzer
enum SIGNAL_TOWER_BUZZER
{
	SIGNAL_TOWER_BUZZER_OFF,
	SIGNAL_TOWER_BUZZER_SOUND1,
	SIGNAL_TOWER_BUZZER_SOUND2,
	SIGNAL_TOWER_BUZZER_MAX,
};

const LPCSTR g_lpszSignalTowerBuzzer[] = {
	"Off",
	"Sound1",
	"Sound2",
};

// ==========================================================================
// SRのヘッドタイプの設定
enum SR_HEAD_TYPE
{
	SR_HEAD_TYPE_NONE = 0,
	SR_HEAD_TYPE_LAH512,
	SR_HEAD_TYPE_LAH512_UV,
	SR_HEAD_TYPE_LAH512_THICK,
	SR_HEAD_TYPE_LAH1024_THICK,
	SR_HEAD_TYPE_LAH1024_SS,
	SR_HEAD_TYPE_MAX,
};

const LPCSTR g_lpszSrHeadType[] = {
	"None",
	"LAH512",
	"LAH512(UV)",
	"LAH512(THICK)",
	"LAH1024(THICK)",
	"LAH1024(SS)",
};

// ==========================================================================
// SRのヘッドインターフェイスの設定
enum SR_HEAD_INTERFACE
{
	SR_HEAD_INTERFACE_NONE = 0,
	SR_HEAD_INTERFACE_SERIAL,
	SR_HEAD_INTERFACE_PARALLEL,
	SR_HEAD_INTERFACE_MAX,
};

const LPCSTR g_lpszSrHeadInterface[] = {
	"None",
	"NJPC025-Serial(COMA)",
	"NJPC025-Parallel(ISA)",
};

// 2014.04.04 bagus wavelength step added -->
// ==========================================================================
// SR 波長ステップの設定
enum SR_WAVELENGTH_STEP
{
	SR_WAVELENGTH_STEP_1_NM = 0,
	SR_WAVELENGTH_STEP_05_NM,
	SR_WAVELENGTH_STEP_MAX,
};

const LPCSTR g_lpszSrWavelengthStep[] = {
	"1nm",
	"0.5nm",
};
// 2014.04.04 bagus wavelength step added <--

// ==========================================================================
// SRのターレットの設定
enum SR_TURRET_NAME
{
	SR_TURRET_NONE = 0,
	SR_TURRET_LINER,
	SR_TURRET_OLYMPUS,
	SR_TURRET_MAX,
};

const LPCSTR g_lpszSrTurret[] = {
	"None",
	"Liner Type",
	"Olympus",

};

// ==========================================================================
// 測定データを削除する期間の設定
enum DELETE_DATA_PERIOD
{
	DELETE_DATA_PERIOD_NONE = 0,
	DELETE_DATA_PERIOD_ONE_MONTH,
	DELETE_DATA_PERIOD_THREE_MONTHS,
	DELETE_DATA_PERIOD_SIX_MONTHS,
	DELETE_DATA_PERIOD_ONE_YEAR,
	DELETE_DATA_PERIOD_ONE_HALF_YEARS,
	DELETE_DATA_PERIOD_TWO_YEARS,
	DELETE_DATA_PERIOD_TWO_HALF_YEARS,
	DELETE_DATA_PERIOD_THREE_YEARS,
	DELETE_DATA_PERIOD_THREE_HALF_YEARS,
	DELETE_DATA_PERIOD_FOUR_YEARS,
	DELETE_DATA_PERIOD_FOUR_HALF_YEARS,
	DELETE_DATA_PERIOD_FIVE_YEARS,
	DELETE_DATA_PERIOD_MAX,
};
// Kojika 20090528 Change
const LPCSTR g_lpszDeleteDataPeriod_JPN[] =
{
	"期限無し",
	"1 ヶ月",
	"3 ヶ月",
	"6 ヶ月",
	"1 年",
	"1.5 年",
	"2 年",
	"2.5 年",
	"3 年",
	"3.5 年",
	"4 年",
	"4.5 年",
	"5 年",
};
const LPCSTR g_lpszDeleteDataPeriod_ENU[] =
{
	"None",
	"1 month ago",
	"3 months ago",
	"6 months ago",
	"1 year ago",
	"1.5 years ago",
	"2 years ago",
	"2.5 years ago",
	"3 years ago",
	"3.5 years ago",
	"4 years ago",
	"4.5 years ago",
	"5 years ago",
};
// Kojika 20090528 Change End

// ==========================================================================
// User Setting pattern Rec Num Layer
// User SettingのSRのパターン認識のパターンを探すために
// どのくらい周りを検索するかを設定
enum SR_USER_SETTING_PATTERN_REC_NUM_LAYERS
{
	SR_USER_SETTING_PATTERN_REC_NUM_LAYERS_NONE = 0,
	SR_USER_SETTING_PATTERN_REC_NUM_LAYERS1,
	SR_USER_SETTING_PATTERN_REC_NUM_LAYERS2,
	SR_USER_SETTING_PATTERN_REC_NUM_LAYERS3,
	SR_USER_SETTING_PATTERN_REC_NUM_LAYERS4,
	SR_USER_SETTING_PATTERN_REC_NUM_LAYERS_MAX,
};

// Kojika 20090528 Change
const LPCSTR g_lpszSrUserSettingPatternRecNumLayers_JPN[] = {
	"無し",
	"1 (8 サイト)",
	"2 (24 サイト)",
	"3 (48 サイト)",
	"4 (80 サイト)",
};
const LPCSTR g_lpszSrUserSettingPatternRecNumLayers_ENU[] = {
	"None",
	"1 (8 Sites)",
	"2 (24 Sites)",
	"3 (48 Sites)",
	"4 (80 Sites)",
};
#define PATREC_LAYERS_STR_INIT_ENU { "None", "1 (8 Sites)", "2 (24 Sites)", "3 (48 Sites)", "4 (80 Sites)" }
#define PATREC_LAYERS_STR_INIT_JPN { "無し", "1 (8 サイト)", "2 (24 サイト)", "3 (48 サイト)", "4 (80 サイト)" }
// Kojika 20090527 Change End


// ==========================================================================
// 測定結果のフォーマット(小数点第何位まで有効にするか)の設定
enum FIGURE_FORMAT
{
	FIGURE_FORMAT_0 = 0,
	FIGURE_FORMAT_1,
	FIGURE_FORMAT_2,
	FIGURE_FORMAT_3,
	FIGURE_FORMAT_4,
	FIGURE_FORMAT_MAX,
};

const LPCSTR g_lpszFigureFormat[] = {
	"#",
	"#.#",
	"#.##",
	"#.###",
	"#.####",
};

#define FIGURE_FORMAT_STR_INIT { "#", "#.#", "#.##", "#.###", "#.####" }

// ==========================================================================
// 測定結果の単位の設定
enum FIGURE_UNIT_SR_THICKNESS
{
	FIGURE_UNIT_SR_THICKNESS_MICRON =0,
	FIGURE_UNIT_SR_THICKNESS_ANGSTROM,
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
	FIGURE_UNIT_SR_THICKNESS_UM,
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */
	FIGURE_UNIT_SR_THICKNESS_MAX,
};

const LPCSTR g_lpszFigureUnitSrThickness[] = {
	"nm",
	"A",
/* added 2014.05.22 hmenjo thick unit um ---------- { ---------- */
	"um",
/* added 2014.05.22 hmenjo thick unit um ---------- } ---------- */
};

#define FIGURE_UNIT_STR_INIT { "nm","A"  }

//2009.09.07 bagus se --{--
// ==========================================================================
// 測定結果の単位の設定
enum FIGURE_UNIT_SE_THICKNESS
{
	FIGURE_UNIT_SE_THICKNESS_ANGSTROM = 0,
	FIGURE_UNIT_SE_THICKNESS_MICRON,
	FIGURE_UNIT_SE_THICKNESS_MAX,
};

const LPCSTR g_lpszFigureUnitSeThickness[] = {
	"A",
	"nm",
};

#define FIGURE_UNIT_SE_STR_INIT { "A","nm"	}
//2009.09.07 bagus se --}--

// ==========================================================================
// Sample設定のSample Shape
enum SAMPLE_SHAPE
{
	SAMPLE_SHAPE_RECTANGLE = 0,
	SAMPLE_SHAPE_MAX,
};
// Kojika 20090528 Change
const LPCSTR g_lpszSampleShape_ENU[] = {
	"Rectangle",
};
const LPCSTR g_lpszSampleShape_JPN[] = {
	"矩形",
};
#define SAMPLE_SHAPE_STR_INIT_ENU { "Rectangle" }
#define SAMPLE_SHAPE_STR_INIT_JPN { "矩形" }
// Kojika 20090528 Change End

// ==========================================================================
// Lens Maginification
// k-matsuo ヘッドごとにレンズ設定があるので、意味がない。20080229
//enum LENS_MAGNIFICATION
//{
//	LENS_5 = 0,
//	LENS_10,
//	NONE,
//	LENS_50,
//	LENS_MAX,
//};
//
//const LPCSTR g_szLensMagnificationItem[] = {
//	"X5",
//	"X10",
//	"NONE",
//	"X50",
//};

// ==========================================================================
// View Window Camera Type
enum VIEW_WINDOW_MEASUREMENT_TYPE
{
	VIEW_WINDOW_MEAS_TYPE_SR_REFLECTANCE = 0,
	VIEW_WINDOW_MEAS_TYPE_SR_TRANSMITTANCE,
	VIEW_WINDOW_MEAS_TYPE_SE,
// 2009.10.19 bagus MS 追加 --{--
//	VIEW_WINDOW_MEAS_TYPE_IRSE,
	VIEW_WINDOW_MEAS_TYPE_MS,
// 2009.10.19 bagus MS 追加 --}--
	VIEW_WINDOW_MEAS_TYPE_VIEW,
	VIEW_WINDOW_MEAS_TYPE_MAX,
};

// 2009.12.04 K.Matsuo Change -->
const LPCSTR g_szViewWindowMeasurementTypeItem_ENU[] = {
	"SR Reflectance",
	"View",
	"SR Transmittance",
};
const LPCSTR g_szViewWindowMeasurementTypeItem_JPN[] = {
	"SR 反射率",
	"ビュー",
	"SR 透過率",
};
// 2009.12.04 K.Matsuo Change <--

// ==========================================================================
// Stage Motion (enumがGlobals.hxx内で定義してある)
const LPCSTR g_lpszStageMotion[] = {
	//"High",
	//"Middle",
	//"Low",
	"高速",
	"中速",
	"低速",
	"ジョグ(低)",
	"ジョグ(中)",
	"ジョグ(高)",
};
/*
//#define MOTION_MODE_STR_INIT { "LOW", "MID", "HIGH"}
#define MOTION_MODE_STR_INIT { "低速", "中速", "高速"}
#define MOTION_MODE_STAGE_STR_INIT { "低速", "中速", "高速", "ジョグ(低)", "ジョグ(中)", "ジョグ(高)" }
#define MOTION_MODE_ELEVATOR_STR_INIT { "低速", "中速", "高速", "ジョグ" }
// 2009.05.28 K.Matsuo SPT -->
//#define MOTION_MODE_STR_INIT { "LOW", "MID", "HIGH", "JOG(L)", "JOG(M)", "JOG(H)" }
//#define MOTION_MODE_STAGE_STR_INIT { "低速", "中速", "高速", "ジョグ(低)", "ジョグ(中)", "ジョグ(高)" }
//#define MOTION_MODE_ELEVATOR_STR_INIT { "ジョグ(低)", "ジョグ(中)", "ジョグ(高)"}
// 2009.05.28 K.Matsuo SPT <--
*/
const LPCSTR g_lpszStageMotion_JPN[] = {
	"高速",
	"中速",
	"低速",
};
const LPCSTR g_lpszStageMotion_ENU[] = {
	"Fast",
	"Middle",
	"Low",
};
#define MOTION_MODE_STR_INIT_JPN { "低速", "中速", "高速", "ジョグ(低)", "ジョグ(中)", "ジョグ(高)" }
#define MOTION_MODE_STR_INIT_ENU { "LOW", "MID", "FAST", "JOG(LOW)", "JOG(MIDDLE)", "JOG(FAST)" }
#define MOTION_MODE_STAGE_STR_INIT_JPN { "低速", "中速", "高速", "ジョグ(低)", "ジョグ(中)", "ジョグ(高)" }
#define MOTION_MODE_STAGE_STR_INIT_ENU { "LOW", "MIDDLE", "FAST", "JOG(LOW)", "JOG(MIDDLE)", "JOG(FAST)" }
#define MOTION_MODE_ELEVATOR_STR_INIT_JPN { "ジョグ(低)", "ジョグ(中)", "ジョグ(高)" }
#define MOTION_MODE_ELEVATOR_STR_INIT_ENU { "JOG(LOW)", "JOG(MIDDLE)", "JOG(FAST)" }

// ==========================================================================
// Correct Angle (enumがConfigFile.hxx内で定義してある
const LPCSTR g_lpszCorrectAngle[] = {
	"0",
	"90",
	"-90",
};

// ==========================================================================
// User Accountのアカウントレベルの設定
//(enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR g_lpszAccountLevel_JPN[] = {
	"管理者",
	"エンジニア",
	"オペレータ",
};
const LPCSTR g_lpszAccountLevel_ENU[] = {
	"Administrator",
	"Engineer",
	"Operator",
};
#define ACCOUNT_LEVEL_STR_INIT_ENU { "Administrator", "Engineer", "Operator" }
#define ACCOUNT_LEVEL_STR_INIT_JPN { "管理者", "エンジニア", "オペレータ" }
// Kojika 20090528 Change End

// ==========================================================================
// MainRecipeのPoint Manual Adjustmentの設定
enum POINT_MANUAL_ADJUSTMENT
{
	POINT_MANUAL_ADJUSTMENT_NONE = 0,
//	  POINT_MANUAL_ADJUSTMENT_ONLY_FIRST_POINT,
//	  POINT_MANUAL_ADJUSTMENT_ALL_POINT_STAGE_PROGRAM_POINT,
	POINT_MANUAL_ADJUSTMENT_ALL_POINT_EVERY_ADJUSTMENT,
	POINT_MANUAL_ADJUSTMENT_MAX,
};

//Saiki 20090527 Change ----->
const LPCSTR g_lpszPointManualAdjustment_ENU[] = {
	"None",
	"All Point(Manual Adjustment)",
};
const LPCSTR g_lpszPointManualAdjustment_JPN[] = {
	"補正しない",
//	  "最初の測定時のみ補正",
//	  "全ての測定点を補正(ステージプログラム単位で補正)",
//	  "全ての測定点を補正(各点独立で補正)",
	"測定点を手動補正",
//Saiki 20090527 Change <-----
};

// ==========================================================================
// MainRecipeのSampleID Inputの設定
/*enum SAMPLE_ID_INPUT
{
	SAMPLE_ID_INPUT_NONE = 0,
	SAMPLE_ID_INPUT_SAMPLE_EXCHANGE,
	SAMPLE_ID_INPUT_PROGRAM_START,
	SAMPLE_ID_INPUT_MAX,
};

const LPCSTR g_lpszSampleIdInput[] = {
	"None",
	"At Sample Exchange",
	"At Program Start",
};
*/

// ==========================================================================
// MainRecipeのSRの場合のフォーカスの設定
enum SR_FOCUS
{
	SR_FOCUS_NONE = 0,
	SR_FOCUS_ONLY_FIRST_POINT,
	SR_FOCUS_ALL_POINT,
	SR_FOCUS_MAX,
};
//Saiki 20090527 Change ----->
const LPCSTR g_lpszSrFocus_ENU[] = {
	"None",
	"Only The First Point",
	"All Measurement Points",
};
const LPCSTR g_lpszSrFocus_JPN[] = {
	"フォーカスしない",
	"最初の測定点のみ行う",
	"全ての測定点で行う",
};
//Saiki 20090527 Change <-----

// ==========================================================================
// MainRecipeのSRの場合のフォーカス失敗時の設定
enum AFOCUS_FAIL_OPTION
{
	AFOCUS_FAIL_OPTION_AUTO_RETRY_MANUAL_FOCUS = 0, 	// 規定回数リトライ + Manual Deskewに移行する
	AFOCUS_FAIL_OPTION_AUTO_RETRY_PROCEED,				// 規定回数リトライ + そのまま測定
	AFOCUS_FAIL_OPTION_AUTO_RETRY_CANCEL_RECIPE,		// 規定回数リトライ + 測定は行わず，レシピ終了
	AFOCUS_FAIL_OPTION_MANUAL_FOCUS,					// Manual Deskewに移行する
	AFOCUS_FAIL_OPTION_PROCEED, 						// そのまま測定
	AFOCUS_FAIL_OPTION_CANCEL_RECIPE,					// 測定は行わず，レシピ終了
	AFOCUS_FAIL_OPTION_MAX,
};

//Saiki 20090527 Change ----->
const LPCSTR g_lpszSrFocusIfFail_ENU[] = {
	"Auto Retry + Manual Focus",
	"Auto Retry + Proceed Measurement",
	"Auto Retry + Cancel Recipe",
	"Manual Focus",
	"Proceed Measurement",
	"Cancel Recipe"
};
const LPCSTR g_lpszSrFocusIfFail_JPN[] = {
	"自動リトライ + 手動フォーカス",
	"自動リトライ + 強制測定",
	"自動リトライ + レシピキャンセル",
	"手動フォーカス",
	"強制測定",
	"レシピキャンセル"
};
//Saiki 20090527 Change <-----
// ==========================================================================
// MainRecipeのSRの場合のフォーカス失敗のデータの有効性の設定
enum FANAL_FOCUS_FAIL_DATA_VALID_OPTION
{
	FANAL_FOCUS_FAIL_DATA_VALID_OPTION_VALID = 0,		// 最終的にフォーカスが失敗であった時、測定データを有効とする（統計データに加える）
	FANAL_FOCUS_FAIL_DATA_VALID_OPTION_INVALID, 		// 最終的にフォーカスが失敗であった時、測定データを無効とする（統計データに加えない）
	FANAL_FOCUS_FAIL_DATA_VALID_OPTION_MAX,
};

//Saiki 20090527 Change ----->
const LPCSTR g_lpszSrFocusIfFailDataValid_ENU[] = {
	"Valid",
	"Invalid",
};
const LPCSTR g_lpszSrFocusIfFailDataValid_JPN[] = {
	"含む",
	"含まない",
};
//Saiki 20090527 Change <-----
// ==========================================================================
// MainRecipeのSRの場合のデフォルトディスプレイの設定
enum SR_DEFAULT_DISPLAY
{
	SR_DEFAULT_DISPLAY_LIST = 0,
	SR_DEFAULT_DISPLAY_GRAPH,
	//SR_DEFAULT_DISPLAY_DISABLE,
	SR_DEFAULT_DISPLAY_MAX,
};

//Saiki 20090527 Change ----->
const LPCSTR g_lpszSrDefaultDisplay_ENU[] = {
	"List",
	"Graph",
	//"Disable",
};
const LPCSTR g_lpszSrDefaultDisplay_JPN[] = {
	"リスト",
	"グラフ",
	//"Disable",
};
//Saiki 20090527 Change <-----
// ==========================================================================
// Recalibration Programの単位の設定
enum RECALIBRATION_PROGRAM_UNIT
{
	RECALIBRATION_PROGRAM_UNIT_NONE = 0,
	RECALIBRATION_PROGRAM_UNIT_ANGSTROM,
	RECALIBRATION_PROGRAM_UNIT_MICRON,
	RECALIBRATION_PROGRAM_UNIT_PERCENT,
	RECALIBRATION_PROGRAM_UNIT_MAX,
};

// Kojika 20090528 Change
const LPCSTR g_lpszRecalibrationProgramUnit_JPN[] = {
	"無し",
	"A",
	"nm",
	"%",
};
const LPCSTR g_lpszRecalibrationProgramUnit_ENU[] = {
	"None",
	"A",
	"nm",
	"%",
};
// Kojika 20090528 Change End
//2009.09.01 bagus stress --{--
enum STRESS_RECALIBRATION_PROGRAM_UNIT
{
	STRESS_RECALIBRATION_PROGRAM_UNIT_MPA = 0,
	STRESS_RECALIBRATION_PROGRAM_UNIT_MAX,
};

const LPCSTR g_lpszStressRecalibrationProgramUnit_JPN[] = {
	"MPa",
};
const LPCSTR g_lpszStressRecalibrationProgramUnit_ENU[] = {
	"MPa",
};
//2009.09.01 bagus stress --}--

//2009.09.02 bagus se SE機能追加 --{--
enum SE_RECALIBRATION_PROGRAM_UNIT
{
	SE_RECALIBRATION_PROGRAM_UNIT_NONE = 0,
	SE_RECALIBRATION_PROGRAM_UNIT_ANGSTROM,
	SE_RECALIBRATION_PROGRAM_UNIT_MICRON,
	SE_RECALIBRATION_PROGRAM_UNIT_ELECTRONVOLT,
	SE_RECALIBRATION_PROGRAM_UNIT_MAX,
};

const LPCSTR g_lpszSeRecalibrationProgramUnit_JPN[] = {
	"無し",
	"A",
	"nm",
	"eV",
};
const LPCSTR g_lpszSeRecalibrationProgramUnit_ENU[] = {
	"None",
	"A",
	"nm",
	"eV",
};
//2009.09.02 bagus se SE機能追加 --}--

// 2009.10.08 bagus CTA 追加 --{--
enum CTA_RECALIBRATION_PROGRAM_UNIT
{
	CTA_RECALIBRATION_PROGRAM_UNIT_DEGREE = 0,
	CTA_RECALIBRATION_PROGRAM_UNIT_MAX,
};

const LPCSTR g_lpszCTARecalibrationProgramUnit_JPN[] = {
	"deg",
};
const LPCSTR g_lpszCTARecalibrationProgramUnit_ENU[] = {
	"deg",
};
// 2009.10.08 bagus CTA 追加 --}--

// 2009.10.13 bagus Gantry 修正 --{--
enum GANTRY_RECALIBRATION_PROGRAM_UNIT
{
	GANTRY_RECALIBRATION_PROGRAM_UNIT_PERCENT = 0,
	GANTRY_RECALIBRATION_PROGRAM_UNIT_MAX,
};

const LPCSTR g_lpszGantryRecalibrationProgramUnit_JPN[] = {
	"%",
};
const LPCSTR g_lpszGantryRecalibrationProgramUnit_ENU[] = {
	"%",
};
// 2009.10.13 bagus Gantry 修正 --}--

// ==========================================================================
// Deskew Mode (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR g_lpszDeskewMode_JPN[] = {
	"デスキューを使用しない",
	"パターン認識 デスキュー",
	"マニュアルデスキュー",
};
const LPCSTR g_lpszDeskewMode_ENU[] = {
	"No Deskew",
	"Pattern Rec. Deskew",
	"Manual Deskew",
};
// Kojika 20090528 Change End

// ==========================================================================
// Deskew Fail Option (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR g_lpszDeskewFailOption_JPN[] = {
	//"Proceed Measurement",
	//"Cancel Recipe",
	//"Manual Deskew",
// 2009.04.10 K.Matsuo Add -->
	//"Proceed Measurement With Point Manual Measurement",
// 2009.04.10 K.Matsuo Add <--
// 2009.04.10 K.Matsuo Change -->
//	"測定開始",
	"強制測定",
// 2009.04.10 K.Matsuo Change <--
	"レシピキャンセル",
	"マニュアルデスキュー",
// 2009.04.10 K.Matsuo Add -->
	"位置補正付き測定（PMAを使用）",
// 2009.04.10 K.Matsuo Add -->
};
const LPCSTR g_lpszDeskewFailOption_ENU[] = {
	//"Proceed Measurement",
	//"Cancel Recipe",
	//"Manual Deskew",
// 2009.04.10 K.Matsuo Add -->
	//"Proceed Measurement With Point Manual Measurement",
// 2009.04.10 K.Matsuo Add <--
// 2009.04.10 K.Matsuo Change -->
//	"測定開始",
	"Proceed Measurement",
// 2009.04.10 K.Matsuo Change <--
	"Cancel Recipe",
	"Manual Deskew",
// 2009.04.10 K.Matsuo Add -->
	"Proceed Measurement With Point Manual Measurement",
// 2009.04.10 K.Matsuo Add -->
};
// Kojika 20090528 Change End

// ==========================================================================
// 数値入力でステージ移動を行うときの絶対移動か相対移動の設定
enum SPECIFY_COORDINATE
{
	SPECIFY_COORDINATE_STAGE = 0,
	SPECIFY_COORDINATE_SAMPLE,
//	SPECIFY_COORDINATE_INCREMENT,	// 今回は使わないので削除しておく
	SPECIFY_COORDINATE_MAX,
};

//Kojika 20090528 Change
const LPCSTR g_lpszSpecifyCoordinate_JPN[] = {
	"ステージ座標",
	"サンプル座標",
//	"Increment",					// 今回は使わないので削除しておく
};
const LPCSTR g_lpszSpecifyCoordinate_ENU[] = {
	"Stage Coordinate",
	"Sample Coordiinate",
//	"Increment",					// 今回は使わないので削除しておく
};
// Kojika 20090528 Change End

// ==========================================================================
// Host Mode
enum CONTROL_MODE
{
	HOST_LOCAL = 0,
	HOST_REMOTE,
};
//Saiki 20090527 Change ----->
const LPSTR HOST_MODE_DISP_ENU[] = {
	"LOCAL",
	"REMOTE",
} ;
const LPSTR HOST_MODE_DISP_JPN[] = {
	"ローカル",
	"リモート",
} ;
//Saiki 20090527 Change <-----

const COLORREF HOST_MODE_COLOR[] = {
	YELLOW_COLOR,
	GREEN_COLOR,
};

// ==========================================================================
// Vacuum Status
enum VACUUM_STATUS
{
	VACUUM_EMPTY = 0,
	VACUUM_NG,
	VACUUM_OK,
};

// ==========================================================================
// Vacuum Sensor
enum VACUUM_SENSOR
{
	VACUUM_SENSOR_OFF = 0,
	VACUUM_SENSOR_ON,
};

// Kojika 20090528 Change
const LPSTR VACUUM_SENSOR_DISP_JPN[] = {
	"バキューム オフ",
	"バキューム オン",
} ;
const LPSTR VACUUM_SENSOR_DISP_ENU[] = {
	"VACUUM OFF",
	"VACUUM ON",
} ;
// Kojika 20090528 Change End

const COLORREF VACUUM_SENSOR_TEXT_COLOR[] = {
	BLACK_COLOR,
	WHITE_COLOR,
};

const COLORREF VACUUM_SENSOR_COLOR[] = {
	DARK_GRAY_COLOR,
	LIGHT_BLUE_COLOR,
};

// ==========================================================================
// Sample Sensor
enum GLASS_SENSOR
{
	SAMPLE_SENSOR_OFF = 0,
	SAMPLE_SENSOR_ON,
};

// Kojika 20090528 Change
const LPSTR SAMPLE_SENSOR_DISP_JPN[] = {
	"サンプル 無",
	"サンプル 有",
} ;
const LPSTR SAMPLE_SENSOR_DISP_ENU[] = {
	"SAMPLE NONE",
	"SAMPLE EXIST",
} ;
// Kojika 20090528 Change End

const COLORREF SAMPLE_SENSOR_TEXT_COLOR[] = {
	BLACK_COLOR,
	WHITE_COLOR,
};

const COLORREF SAMPLE_SENSOR_COLOR[] = {
	DARK_GRAY_COLOR,
	LIGHT_BLUE_COLOR,
};

// ==========================================================================
// Load Position Sensor
enum LOAD_POSITION_SENSOR
{
	LOAD_POSITION_SENSOR_OFF = 0,
	LOAD_POSITION_SENSOR_ON,
};

//Kojika 20090528 Change
const LPSTR LOAD_POSITION_SENSOR_DISP_JPN[] = {
/* modified 2009.07.03 hmenjo ロードポジション文字 ---------- { ---------- */
//	"ロードポジション外",
/* modified 2009.07.03 hmenjo ロードポジション文字 ----------			   */
	"ロードポジション",
/* modified 2009.07.03 hmenjo ロードポジション文字 ---------- } ---------- */
	"ロードポジション",
};
const LPSTR LOAD_POSITION_SENSOR_DISP_ENU[] = {
/* modified 2009.07.03 hmenjo ロードポジション文字 ---------- { ---------- */
//	"NO LOAD POS.",
/* modified 2009.07.03 hmenjo ロードポジション文字 ----------			   */
	"LOAD POSITION",
/* modified 2009.07.03 hmenjo ロードポジション文字 ---------- } ---------- */
	"LOAD POSITION",
};
// Kojika 20090528 Change End

const COLORREF LOAD_POSITION_SENSOR_TEXT_COLOR[] = {
	BLACK_COLOR,
	WHITE_COLOR,
};

const COLORREF LOAD_POSITION_SENSOR_COLOR[] = {
	DARK_GRAY_COLOR,
	LIGHT_BLUE_COLOR,
};

// ==========================================================================
// Process Status
enum PROCESS_STATUS
{
	PROCESS_INIT = 0,
	PROCESS_DOWN,
	PROCESS_WAIT,
	PROCESS_PROC,
	PROCESS_ABRT,
	PROCESS_COMP,
	PROCESS_MAX,
};

//Saiki 20090527 Change ----->
const LPSTR PROCESS_STATUS_DISP_ENU[] = {
	"INIT",
	"DOWN",
	"WAIT",
	"PROCESSING",
	"ABORT",
	"COMPLETE",
} ;
const LPSTR PROCESS_STATUS_DISP_JPN[] = {
	"初期化中",
	"ダウン",
	"ウェイト",
	"プロセス中",
	"アボート",
	"コンプリート",
} ;

//Saiki 20090527 Change <-----

const COLORREF PROCESS_STATUS_COLOR[] = {
	WHITE_COLOR,
	RED_COLOR,
	YELLOW_COLOR,
	GREEN_COLOR,
	MAGENTA_COLOR,
	LIGHT_BLUE_COLOR,
};

// ==========================================================================
// Alarm Status
enum {
	ALARM_NON = 0,
	ALARM_EXIST,
};
//Saiki 20090527 Change ----->
const LPSTR ALARM_STATUS_DISP_ENU[] = {
	"ALARM",
	"ALARM",
} ;
const LPSTR ALARM_STATUS_DISP_JPN[] = {
	"異常",
	"異常",
} ;

//Saiki 20090527 Change <-----
const COLORREF ALARM_STATUS_COLOR[] = {
	DARK_GRAY_COLOR,
	RED_COLOR,
};

// ==========================================================================
// Warning Status
enum {
	WARNING_NON = 0,
	WARNING_EXIST,
};
//Saiki 20090527 Change ----->
const LPSTR WARNING_STATUS_DISP_ENU[] = {
	"WARNING",
	"WARNING",
} ;
const LPSTR WARNING_STATUS_DISP_JPN[] = {
	"警告",
	"警告",
} ;
//Saiki 20090527 Change <-----
const COLORREF WARNING_STATUS_COLOR[] = {
	DARK_GRAY_COLOR,
	YELLOW_COLOR,
};

// ==========================================================================
// Display Status (Main Menu Status)
enum DISPLAY_STATUS //MAIN_MENU_STATUS
{
	MAIN_MENU_MEASUREMENT = 0,
	MAIN_MENU_DATA,
	MAIN_MENU_RECIPE_SETTING,
	MAIN_MENU_ALARM_HISTORY,
	MAIN_MENU_USER_SETTING,
	MAIN_MENU_MAINTENANCE,
	MAIN_MENU_SYSTEM,
	MAIN_MENU_MANUAL_MEASUREMENT,
};

// ==========================================================================
// Measurement Mode
enum MEASUREMENT_RECIPE
{
	MEASUREMENT_MAIN_RECIPE = 0,
	MEASUREMENT_MULTI_RECIPE,
};

// ==========================================================================
// Recipe Setting Mode
enum RECIPE_SETTING_MODE
{
	RECIPE_SETTING_MAIN_RECIPE = 0, 				// Recipe Setting Main Recipe
	RECIPE_SETTING_MULTI_RECIPE,					// Recipe Setting Multi Recipe
	RECIPE_SETTING_MEASUREMENT_PROGRAM, 			// Recipe Setting Measurement Program
	RECIPE_SETTING_STAGE_PROGRAM,					// Recipe Setting Stage Program
	RECIPE_SETTING_RECALIBRATION_PROGRAM,			// Recipe Setting Recalib Program
	RECIPE_SETTING_POINT_DESKEW_PROGRAM,			// Recipe Setting Point Deskew Program
	RECIPE_SETTING_NUM_MAX,
};

// ==========================================================================
// Recipe Save / Discard
//レシピ(プログラム)の終了，保存するかそのまま終了するかを決める変数の定義
enum{
	MODIFIED_SAVE = 0,
	MODIFIED_DISCARD,
};

// ==========================================================================
// Save As
// SaveAsDlgでどのレシピ(プログラム)のSaveAsなのかを決める変数の定義
enum{
	SAVE_AS_MAIN_RECIPE = 0,
	SAVE_AS_MULTI_RECIPE,
	SAVE_AS_MEASUREMENT_PROGRAM,
	SAVE_AS_STAGE_PROGRAM,
	SAVE_AS_RECALIBRATION_PROGRAM,
	SAVE_AS_POINT_DESKEW_PROGRAM,
	SAVE_AS_USER_ACCOUNT,
	SAVE_AS_SAMPLE,
	SAVE_AS_SITE_PATTERN,
	SAVE_AS_SITE_PATTERN_IMG,
	SAVE_AS_OVERLAY_CAPTURE,
	SAVE_AS_MAX,
};

// ==========================================================================
// Operation Panel
enum OPERATION_PANEL
{
	OP_MEASUREMENT = 0, 							// Measurement
	OP_MEASUREMENT_PROCESS, 						// Measurement Process
	OP_MANUAL_MEASUREMENT_PROCESS,					// Manual Measurement Process
	OP_RECIPE_SETTING,								// Recipe Setting
	OP_DATA,										// Measurement Data
	OP_DATA_REVIEW, 								// Data Review List
	OP_USER_SETTING,								// User Setting
	OP_MAINTENANCE, 								// Maintenance
	OP_SYSTEM,										// System
	OP_ALARM_HISTORY,								// Alarm History
	OP_MANUAL_MEASUREMENT_FORM, 					// Manual Measurement Form
	OP_NUM_MAX,
};

// ==========================================================================
// Main Frame Right Pane
enum EXE_WND
{
	MEAS_WND,										// Measurement
	MANUAL_RECIPE_WND,								// 測定画面(ZAxisOffset時)
	MANUAL_MEAS_WND,								// Manual Measurement
	RECIPE_MAIN_LIST_WND,							// Main Recipe List
	RECIPE_MULTI_LIST_WND,							// Multi Recipe List
	RECIPE_MEAS_LIST_WND,							// Measurement Program List
	RECIPE_STAGE_LIST_WND,							// Stage Program List
	RECIPE_RECALIBRATION_LIST_WND,					// Recalibration Program List
	RECIPE_POINT_DESKEW_LIST_WND,					// Point Deskew Program List
	RECIPE_MAIN_SR_WND, 							// Recipe Setting Main SR
	RECIPE_MAIN_SE_WND, 							// Recipe Setting Main SE
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	RECIPE_MAIN_COMPEASE_WND, 						// Recipe Setting Main COMPEASE
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	RECIPE_MAIN_IRSE_WND,							// Recipe Setting Main IRSE
	RECIPE_MAIN_MS_WND,								// Recipe Setting Main MicroScope
// 2009.10.19 bagus MS 追加 --}--
	RECIPE_MAIN_STRESS_WND, 						// Recipe Setting Main STRESS
	RECIPE_MAIN_4PP_WND,							// Recipe Setting Main 4PP
	RECIPE_MAIN_CTA_WND,							// Recipe Setting Main CTA
	RECIPE_MULTI_WND,								// Recipe Setting Multi Recipe
	RECIPE_MEAS_SR_THICKNESS_WND,					// Recipe Setting Measurement Program SR Thickness
	RECIPE_MEAS_SR_REFLECTANCE_WND, 				// Recipe Setting Measurement Program SR Reflectance
	RECIPE_MEAS_SR_TRANSMITTANCE_WND,				// Recipe Setting Measurement Program SR Transmittance
// 2010.01.07 bagus Gantry --{--
	RECIPE_MEAS_SR_GANTRY_WND, 						// Recipe Setting Measurement Program SR Gantry
// 2010.01.07 bagus Gantry --}--
	RECIPE_MEAS_SR_CIE_REFLECTANCE_WND, 			// Recipe Setting Measurement Program SR Reflectance CIE
	RECIPE_MEAS_SR_CIE_TRANSMITTANCE_WND,			// Recipe Setting Measurement Program SR Transmittance CIE
	RECIPE_MEAS_SR_OD_WND,							// Recipe Setting Measurement Program SR Optical Density
	RECIPE_MEAS_SE_THICKNESS_WND,					// Recipe Setting Measurement Program SE Thickness
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	RECIPE_MEAS_COMPEASE_THICKNESS_WND, 			// Recipe Setting Measurement Program CompleteEASE Thickness
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	RECIPE_MEAS_IRSE_THICKNESS_WND, 				// Recipe Setting Measurement Program IRSE Thickness
	RECIPE_MEAS_MS_WND,								// Recipe Setting Measurement Program Micro Scope
// 2009.10.19 bagus MS 追加 --}--
	RECIPE_MEAS_STRESS_WND, 						// Recipe Setting Measurement Program STRESS
	RECIPE_MEAS_4PP_WND,							// Recipe Setting Measurement Program 4PP
	RECIPE_MEAS_CTA_WND,							// Recipe Setting Measurement Program ContactAngle
	// 2009.10.13 bagus Distance 追加 --{--
	RECIPE_MEAS_SR_DISTANCE_WND,					// Recipe Setting Measurement Program Distance
	// 2009.10.13 bagus Distance 追加 --}--
	RECIPE_STAGE_WND,								// Recipe Setting Stage Program
	//2009.08.25 bagus stress --{--
	RECIPE_STAGE_STRESS_WND,						// Recipe Setting Stage Program STRESS
	//2009.08.25 bagus stress --}--
	// 2009.10.02 bagus Gantry --{--
	RECIPE_STAGE_GANTRY_WND,						// Recipe Setting Stage Program Gantry
	// 2009.10.02 bagus Gantry --}--
	// 2009.10.13 bagus Distance 追加 --{--
	RECIPE_STAGE_SR_DISTANCE_WND,					// Recipe Setting Stage Program Distance
	// 2009.10.13 bagus Distance 追加 --}--
	RECIPE_RECALIBRATION_WND,						// Recipe Setting Recalibration Program
	RECIPE_POINT_DESKEW_WND,						// Recipe Setting Point Deskew Program
	DATA_LIST_WND,									// Data List
	DATA_SR_WND,									// Data SR Review
	DATA_SE_WND,									// Data SE Review
// 2009.10.19 bagus MS 追加 --{--
//	DATA_IRSE_WND,									// Data IRSE Review
	DATA_MS_WND,									// Data MS Review
// 2009.10.19 bagus MS 追加 --}--
	DATA_STRESS_WND,								// Data STRESS Review
	DATA_4PP_WND,									// Data 4PP Review
	DATA_CONTACT_ANGLE_WND, 						// Data CTA Review
	USERCONF_WND,									// User Setting Wnd
	USERCONF_DEF_MAIN_RECIPE_WND,					// SRのMainRecipeのデフォルト値設定
	USERCONF_DEF_SR_THICKNESS_WND,					// SRのThickness測定のデフォルト値設定
	USERCONF_DEF_SR_REFLECTANCE_WND,				// SRのReflectance測定のデフォルト値設定
	USERCONF_DEF_SR_TRANSMITTANCE_WND,				// SRのTransmittance測定のデフォルト値設定
	USERCONF_DEF_SR_CIE_REFLECTANCE_WND,			// SRのReflectance CIE測定のデフォルト値設定
	USERCONF_DEF_SR_CIE_TRANSMITTANCE_WND,			// SRのTransmittance CIE測定のデフォルト値設定
	USERCONF_DEF_SR_OD_WND, 						// SRのOptical Density測定のデフォルト値設定
	USERCONF_DEF_SR_STRESS_WND, 					// SRのStress測定のデフォルト値設定
	USERCONF_DEF_SR_4PP_WND,						// SRの4PP測定のデフォルト値設定
	USERCONF_DEF_SR_CTA_WND,						// SRのContactAngle測定のデフォルト値設定
	//2009.09.07 bagus se --{--
	USERCONF_DEF_SE_THICKNESS_WND,					// SEのThickness測定のデフォルト値設定
	//2009.09.07 bagus se --}--
	// 2009.09.29 bagus SE --{--
	USERCONF_DEF_SE_MAIN_RECIPE_WND,				// SEのMainRecipeのデフォルト値設定
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	USERCONF_DEF_COMPEASE_THICKNESS_WND,			// CompleteEASEのThickness測定のデフォルト値設定
	USERCONF_DEF_COMPEASE_MAIN_RECIPE_WND,			// CompleteEASEのMainRecipeのデフォルト値設定
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.08 bagus CTA 追加 --{--
	USERCONF_DEF_CTA_MAIN_RECIPE_WND,				// ContactAngleのMainRecipeのデフォルト値設定
	USERCONF_DEF_CTA_WND,							// ContactAngle測定のデフォルト値設定
	// 2009.10.08 bagus CTA 追加 --}--
	// 2009.10.13 bagus Distance 追加 --{--
	USERCONF_DEF_SR_DISTANCE_WND,					// SRのDistance測定のデフォルト値設定
	// 2009.10.13 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	USERCONF_DEF_MS_MAIN_RECIPE_WND,				// MicroScopeのMainRecipeのデフォルト値設定
	USERCONF_DEF_MS_WND,							// MicroScope測定のデフォルト値設定
	//2009.10.20 bagus MS 追加 --}--
	//2009.10.21 bagus Stress 追加 --{--
	USERCONF_DEF_STRESS_MAIN_RECIPE_WND,			// StressのMainRecipeのデフォルト値設定
	USERCONF_DEF_STRESS_WND,						// Stress測定のデフォルト値設定
	//2009.10.21 bagus Stress 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	USERCONF_DEF_4PP_MAIN_RECIPE_WND,				// ResistanceのMainRecipeのデフォルト値設定
	USERCONF_DEF_4PP_WND,							// Resistance測定のデフォルト値設定
	// 2009.11.04 bagus RS 追加 --}--
	MAINTE_WND, 									// Maintenance Wnd
	SYSCONF_WND,									// System Wnd
	SYSCONF_Z_AXIS_OFFSET_MEAS_WND, 				// 測定画面(ZAxisOffset時)
	ALARM_HISTORY_LIST_WND, 						// Alarm History List
	MANUAL_MEAS_FORM_WND,							// Manual Measurement Form
	EXE_WND_MAX,
};

// ==========================================================================
// SR Measurement Type
// (enumがGlobals.hxx内で定義してある)
//Kojika 20090528 Change
const LPCSTR MEAS_PROG_TYPE_SR_ITEM_JPN[] = {
	"膜厚測定 (n,k)",
	"反射率測定",
	"透過率測定",
// 2009.09.03 K.Matsuo -->
	"透過率測定 (ガントリー）",
// 2009.09.03 K.Matsuo <--
// 2009.10.09 bagus ２点間 --{--
	"2点間距離測定",
// 2009.10.09 bagus ２点間 --}--
	"色度測定 (反射率)",
	"色度測定 (透過率)",
	"光学濃度測定",
};
const LPCSTR MEAS_PROG_TYPE_SR_ITEM_ENU[] = {
	"Thickness (n,k)",
	"Reflectance",
	"Transmittance",
// 2009.09.03 K.Matsuo -->
	"Transmittance (Gantry)",
// 2009.09.03 K.Matsuo <--
// 2009.10.09 bagus ２点間 --{--
	"Distance",
// 2009.10.09 bagus ２点間 --}--
	"Chromaticity (Reflectance)",
	"Chromaticity (Transmittance)",
	"Optical Density",
};
// Kojika 20090528 Change End

// ==========================================================================
// SE Measurement Type
// (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR MEAS_PROG_TYPE_SE_ITEM_JPN[] = {
//2009.10.21 bagus Stress 修正 --{--
//	"膜厚測定",
	"膜厚測定 (n,k,Eg)",
//2009.10.21 bagus Stress 修正 --}--
// 2009.09.25 K.Matsuo delete -->
//	"光学定数(n,k)測定",
//	"反射率,透過率計算(%T, %R)",
//	"光学粗さ測定",
//	"混在マテリアルの凝縮",
//	"光学バンドギャップa-Si測定",
//	"LTPS用結晶化度関連パラメータ測定",
// 2009.09.25 K.Matsuo delete <--
};
const LPCSTR MEAS_PROG_TYPE_SE_ITEM_ENU[] = {
//2009.10.21 bagus Stress 修正 --{--
//	"Thickness",
	"Thickness (n,k,Eg)",
//2009.10.21 bagus Stress 修正 --}--
// 2009.09.25 K.Matsuo delete -->
//	"光学定数(n,k)測定",
//	"反射率,透過率計算(%T, %R)",
//	"光学粗さ測定",
//	"混在マテリアルの凝縮",
//	"光学バンドギャップa-Si測定",
//	"LTPS用結晶化度関連パラメータ測定",
// 2009.09.25 K.Matsuo delete <--
};
// Kojika 20090528 Change End

// 2009.10.19 bagus MS 追加 --{--
#if 0
// ==========================================================================
// IRSE Measurement Type
// (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR MEAS_PROG_TYPE_IRSE_ITEM_JPN[] = {
	"膜厚測定",
	"光学定数(n,k)測定",
	"反射率,透過率計算(%T, %R)",
	"光学粗さ測定",
	"混在マテリアルの凝縮",
	"光学バンドギャップa-Si測定",
	"LTPS用結晶化度関連パラメータ測定",
};
const LPCSTR MEAS_PROG_TYPE_IRSE_ITEM_ENU[] = {
	"膜厚測定",
	"光学定数(n,k)測定",
	"反射率,透過率計算(%T, %R)",
	"光学粗さ測定",
	"混在マテリアルの凝縮",
	"光学バンドギャップa-Si測定",
	"LTPS用結晶化度関連パラメータ測定",
};
// Kojika 20090528 Change End
#else
const LPCSTR MEAS_PROG_TYPE_MS_ITEM_JPN[] = {
//2009.10.21 bagus MS 追加修正 --{--
//	"顕微鏡",
	"マイクロスコープ測定",
//2009.10.21 bagus MS 追加修正 --}--
};
const LPCSTR MEAS_PROG_TYPE_MS_ITEM_ENU[] = {
	"Micro Scope",
};
#endif
// 2009.10.19 bagus MS 追加 --}--


// ==========================================================================
// ContactAngle Measurement Type
// (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change

const LPCSTR MEAS_PROG_TYPE_CONTACT_ANGLE_ITEM_JPN[] = {
	"コンタクトアングル測定",
};
const LPCSTR MEAS_PROG_TYPE_CONTACT_ANGLE_ITEM_ENU[] = {
//2009.10.21 bagus Stress 修正 --{--
//	"Contact Angle Measurement",
	"Contact Angle",
//2009.10.21 bagus Stress 修正 --}--
};
// Kojika 20090528 Change End

// ==========================================================================
// Resistance Measurement Type
// (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR MEAS_PROG_TYPE_4PP_ITEM_JPN[] = {
//2009.10.21 bagus Stress 修正 --{--
//	"シート抵抗測定",
	"シート抵抗",
//2009.10.21 bagus Stress 修正 --}--
};
const LPCSTR MEAS_PROG_TYPE_4PP_ITEM_ENU[] = {
//2009.10.21 bagus Stress 修正 --{--
//	"Sheet Resistance Measurement",
	"Sheet Resistance",
//2009.10.21 bagus Stress 修正 --}--
};
// Kojika 20090528 Change End

// ==========================================================================
// STRESS Measurement Type
// (enumがGlobals.hxx内で定義してある)
// Kojika 20090528 Change
const LPCSTR MEAS_PROG_TYPE_STRESS_ITEM_JPN[] = {
	"応力測定",
};
const LPCSTR MEAS_PROG_TYPE_STRESS_ITEM_ENU[] = {
//2009.10.21 bagus Stress 修正 --{--
//	"STRESS Measurement",
	"Stress",
//2009.10.21 bagus Stress 修正 --}--
};// Kojika 20090528 Change End

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// ==========================================================================
// CompEASE Measurement Type
// (enumがGlobals.hxx内で定義してある)
const LPCSTR MEAS_PROG_TYPE_COMPEASE_ITEM_JPN[] = {
	"膜厚測定 (n,k,Eg)",
};
const LPCSTR MEAS_PROG_TYPE_COMPEASE_ITEM_ENU[] = {
	"Thickness (n,k,Eg)",
};
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// ==========================================================================
// Head Type
// (enumがGlobals.hxx内で定義してある)
const LPCSTR HEAD_TYPE_ITEM[] = {
	"SR",
	"SE",
	"IRSE",
// 2009.11.04 bagus RS 追加 --{--
//	"4PP",
	"RESISTANCE",
// 2009.11.04 bagus RS 追加 --}--
//2009.10.21 bagus CA 追加修正 --{--
//2009.12.07 bagus CA 修正 --{--
//	"CTA",
//	"CA",
	"CONTACT ANGLE",
//2009.12.07 bagus CA 修正 --}--
//2009.10.21 bagus CA 追加修正 --}--
	"STRESS",
// 2009.10.19 bagus MS 追加 --{--
// 2009.12.07 bagus MS 変更 --{--
//	"MS",
	"MICRO SCOPE",
// 2009.12.07 bagus MS 変更 --}--
// 2009.10.19 bagus MS 追加 --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	"EASE",
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
};

// 2009.12.09 bagus --{--
const LPCSTR HEAD_TYPE_ITEM_SHORT[] = {
	"SR",
	"SE",
	"IRSE",
	"RS",
	"CA",
	"STRESS",
	"MS",
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	"EASE",
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
};
// 2009.12.09 bagus --}--

// 2009.10.09 bagus StagePGM 共通化 --{--
// ==========================================================================
// StagePGM Type
// (enumがGlobals.hxx内で定義してある)
/*
const LPCSTR STAGE_PGM_TYPE_ITEM[] = {
	"STANDARD",
	"STRESS",
	"SR Gantry Transmittance",
	"2 Point Distance",
};
*/
//2009.12.07 bagus 修正 --{--
const LPCSTR STAGE_PGM_TYPE_ITEM_ENU[] = {
	"STANDARD",
	"STRESS",
	"SR Gantry Transmittance",
	"2 Point Distance",
};
const LPCSTR STAGE_PGM_TYPE_ITEM_JPN[] = {
	_T("標準"),
	_T("応力"),
	_T("ガントリー透過率"),
	_T("2点間距離"),
};
//2009.12.07 bagus 修正 --}--
// 2009.10.09 bagus StagePGM 共通化 --}--

// ==========================================================================
// Optical Filter Type
// (enumがGlobals.hxx内で定義してある)
//	Kojika 20090528 Change

const LPCSTR OPTICAL_FILTER_TYPE_ITEM_JPN[] = {
	"フィルタ無",
	"オープン", 				// 名称を「None」から「Open」に変更
	"ダーク",
	"イエローフィルタ", 	// POS1
	"オレンジフィルタ", 	// POS2
	"Pos3",
};
const LPCSTR OPTICAL_FILTER_TYPE_ITEM_ENU[] = {
	"Unknown",
	"Open", 				// 名称を「None」から「Open」に変更
	"Dark",
	"Yellow Filter",		// POS1
	"Orange Filter",		// POS2
	"Pos3",
};
// Kojika 20090528 Change End

// ==========================================================================
// Color System Type
//(enumがGlobals.hxx内で定義してある)
const LPCSTR COLORSYSTEMS_TYPE_ITEM[] = {
	"NTSC",
	"PAL_SECAM",
	"EBU",
	"SMPTE",
	"Short Persistance",
	"Long Persistance",
	"Dell",
};

// ==========================================================================
// 測定結果の単位の設定
// 2009.11.04 bagus RS 追加 --{--
#if 0
enum RESISTANCE_UNIT_4PP
{
	RESISTANCE_UNIT_ANGSTROM = 0,
	RESISTANCE_UNIT_NANO,
	RESISTANCE_UNIT_MICRO,
	RESISTANCE_UNIT_MILL,
	RESISTANCE_UNIT_MAX,
};

const LPCSTR RESISTANCE_UNIT_ITEM[] = {
	"A",
	"nm",
	"um",
	"mm",
};
#else
enum RESISTANCE_UNIT_4PP
{
	RESISTANCE_UNIT_MILL = 0,
	RESISTANCE_UNIT_MICRO,
	RESISTANCE_UNIT_NANO,
	RESISTANCE_UNIT_ANGSTROM,
	RESISTANCE_UNIT_MAX,
};

const LPCSTR RESISTANCE_UNIT_ITEM[] = {
	"mm",
	"um",
	"nm",
	"A",
};
#endif
// 2009.11.04 bagus RS 追加 --}--

// ==========================================================================
// Voltage
enum RESISTANCE_LIMIT_VOLTAGE
{
	RESISTANCE_LIMIT_VOLTAGE_LOW = 0,
	RESISTANCE_LIMIT_VOLTAGE_HIGH,
	RESISTANCE_LIMIT_VOLTAGE_MAX,
};

const LPCSTR RESISTANCE_LIMIT_VOLTAGE_ITEM[] = {
	"10V",
	"90V",
};

// ==========================================================================
// オートフォーカス判定リスト 項目名
const LPCSTR MEAS_LIST_AF_CHECK_ITEM[] = {
	"Focus",
};

// ==========================================================================
// Thickness(n,k)測定結果リスト 項目名
const LPCSTR MEAS_LIST_THICK_ITEM[] = {
	"Thickness",
	"Fit",
};

// ==========================================================================
// CIE測定結果リスト 項目名
const LPCSTR MEAS_LIST_CIE_ITEM[] = {
	"CIE_X",
	"CIE_Y",
	"CIE_Z",
	"CIE_x",
	"CIE_y",
	"CIE_L",
	"CIE_a",
	"CIE_b",
	"CIE_L*",
	"CIE_a*",
	"CIE_b*",
	"CIE_u'",
	"CIE_v'",
	"CIE_u*",
	"CIE_v*",
};

// ==========================================================================
// 測定ポジション位置結果 項目名
const LPCSTR MEAS_LIST_POSITION_ITEM[] = {
	"No.",
	"X",
	"Y",
	"Z",
};
//2009.11.26 bagus 2point-distance --{--
const LPCSTR MEAS_LIST_DISTANCE_POSITION_ITEM[] = {
	"X1",
	"Y1",
	"X2",
	"Y2",
};
//2009.11.26 bagus 2point-distance --}--
// 2009.08.21 bagus stress --{--
// ==========================================================================
// 測定ポジション位置結果 項目名(ストレス測定用)
const LPCSTR MEAS_LIST_STRESS_POSITION_ITEM[] = {
	"No.",
	"X-Sta[mm]",
	"X-End[mm]",
	"Y[mm]",
	"Stress",
	"X",
	"Y",
	"Z",
};
// 2009.08.21 bagus stress --}--

// 2009.11.04 bagus CA --{--
// ==========================================================================
// 測定ポジション位置結果 項目名(コンタクトアングル測定用)
const LPCSTR MEAS_LIST_CA_ITEM[] = {
	"CA[deg]",
	"Rad[um]",
	"Liquid[uL]",
};
// 2009.11.04 bagus CA --}--
// 2009.11.04 bagus RS --{--
// ==========================================================================
// 測定ポジション位置結果 項目名(コンタクトアングル測定用)
const LPCSTR MEAS_LIST_RS_ITEM[] = {
	"RS[OHM]",
	"SurRes",
	"VolRes",
	"Conduct",
};
// 2009.11.04 bagus RS --}--

// ==========================================================================
// 測定パラメータ項目名
// Kojika 20090528 Change
const LPCSTR MEAS_LIST_PARAM_ITEM_JPN[] = {
	"Max",
	"Min",
	"Range",
	"Mean",
	"S.D.",
};
const LPCSTR MEAS_LIST_PARAM_ITEM_ENU[] = {
	"Max",
	"Min",
	"Range",
	"Mean",
	"S.D.",
};// Kojika 20090528 Change End

// ==========================================================================
// テストモード測定結果用ヘッダー項目名(検知条件のみ)
//※ここの項目名は英語固定(日本語化不要)
const LPCSTR TEST_MODE_CON_ITEM[] = {
	"Lens",
	"Filter",
	//Saiki 20090610 Change ----->
	//"Expose[sec]",
	"Integ.[sec]",
	//Saiki 20090610 Change <-----
};
//2010.01.11 bagus GTR --{--
//Gantry透過率のテストモードヘッダ
//※ここの項目名は英語固定(日本語化不要)
const LPCSTR TEST_MODE_SR_GANTRY_CON_ITEM[] = {
	"Lens",
	"Integ.[sec]",
};
//2010.01.11 bagus GTR --}--

// ==========================================================================
// Measurement States
enum MEASUREMENT_STATES
{
	MEASUREMENT_STATES_IDLE = 0,
	MEASUREMENT_STATES_START,
	MEASUREMENT_STATES_SCAN,
	MEASUREMENT_STATES_END,
	MEASUREMENT_STATES_ERROR,
	MEASUREMENT_STATES_SELECT,
	MEASUREMENT_STATES_MAX,
};


// ==========================================================================
// Measurement Data Save の方法
enum
{
	MEASUREMENT_EXIT_SAVE = 0,
	MEASUREMENT_TOOLBAR_SAVE,
	//Saiki 20090401 Add ----->
	MEASUREMENT_TOOLBAR_SAVE_AS,
	//Saiki 20090401 Add <-----
};


// ==========================================================================
// Recipeステージプログラム描画図形
enum STAGE_DRAW_SHAPED
{
	STAGE_DRAW_SHAPED_RECTANGLE = 0,
	STAGE_DRAW_SHAPED_LINE,
	STAGE_DRAW_SHAPED_MAX,
};

//	Kojika 20090528 Change
const LPCSTR STAGE_DRAW_SHAPED_ITEM_JPN[] = {
	"ボックス",
	"ライン",
};
const LPCSTR STAGE_DRAW_SHAPED_ITEM_ENU[] = {
	"Rectangle",
	"Line",
};
// Kojika 20090528 Change End

// ==========================================================================
// LOG ON
enum {
	LOG_OFF = 0,
	LOG_ON,
};

//Saiki 20090527 Change ----->
const LPSTR LOG_ON_STATUS_DISP_ENU[] = {
	"LOG OFF",
	"LOG ON",
} ;
const LPSTR LOG_ON_STATUS_DISP_JPN[] = {
	"ログオフ",
	"ログオン",
} ;
//Saiki 20090527 Change <-----


// ==========================================================================
//MeasurementOperationView内のボタン有効/無効の切替え用
enum SAMPLE_BUTTONENABLE
{
	HANDLER_LOAD_POSITION_S = 0,
	HANDLER_LOAD_POSITION_E,
	SAMPLE_LOAD_S,
	SAMPLE_LOAD_E,
	SAMPLE_UNLOAD_S,
	SAMPLE_UNLOAD_E,
	MEAS_OPE_ALL_DISABLE,
	MEAS_OPE_ALL_ENABLE,
};

// ==========================================================================
//測定中⇔測定終了 時のボタン有効/無効の切替え用
//シーケンス測定用(リメジャー含む), マニュアル測定, テストモード開始
enum
{
	MEASUREMENT_SEQ_S = 0,		//リメジャーメント含む
	MEASUREMENT_SEQ_E,			//リメジャーメント含む
	MEASUREMENT_MANUALMEASURE_S,
	MEASUREMENT_MANUALMEASURE_E,
	MEASUREMENT_MANUALMEASURE_COMP,
	MEASUREMENT_TEST_S,
	MEASUREMENT_TEST_E,
// 2009.10.30 bagus 2点間 修正 --{--
	MEASUREMENT_MANUAL_DISTANCE_S,
	MEASUREMENT_MANUAL_DISTANCE_E,
	MEASUREMENT_MANUAL_DISTANCE_COMP,
// 2009.10.30 bagus 2点間 修正 --}--
	MEAS_TAB_VIEW_CREATE,
	MEAS_TAB_VIEW_DELETE,
	MANU_MEAS_FORM_CREATE,
	MANU_MEAS_FORM_DELETE,
};


//AF VALID情報(測定結果画面表示用)
const LPCSTR g_lpszAfValidResultShow[] = {
// 2009.10.23 bagus Stress 修正 --{--
//	"-----", //オートフォーカスなし
	"  ---", //オートフォーカスなし
// 2009.10.23 bagus Stress 修正 --}--
	"G(AF)", //オートフォーカス正常(Auto Focus Good)
	"N(AF)", //オートフォーカス異常(Auto Focus NG)
	"G(MZ)", //Zマニュアル操作アリ(成功or失敗の概念ナシ)(Manual Z move Good)
	"G(MF)", //マニュアルフォーカス操作成功(Manual Focus Good)
	"N(MF)", //マニュアルフォーカス操作失敗(Manual Focus NG)
	"C(MC)", //ポップアップ→キャンセル(Manual Cancel)
	"C(AC)", //レシピ終了(Auto Cancel)
	"-(--)", //想定外
};

//AF VALID情報(1ポイント測定終了応答時上位へ送信用)
const LPCSTR g_lpszAfValidResultP502[] = {
	"NO", //オートフォーカスなし
	"AF", //オートフォーカス正常(Auto Focus Good)
	"NG", //オートフォーカス異常(Auto Focus NG)
	"MZ", //Zマニュアル操作アリ(成功or失敗の概念ナシ)(Manual Z move Good)
	"MF", //マニュアルフォーカス操作成功(Manual Focus Good)
	"NG", //マニュアルフォーカス操作失敗(Manual Focus NG)
	"MC", //ポップアップ→キャンセル(Manual Cancel)
	"AC", //レシピ終了(Auto Cancel)
	"NG", //想定外
};


// ==========================================================================
//単位桁数表示調整(主にThick用)(g_dUnitConvert[]と対応)
const LPCSTR g_lpszThickUnitDecimalPoint[] = {
	".5", //um
	".2", //nm
	".1", //A
/* added 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
	".0", /* 整数部のみ	*/
/* added 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */
};

// ==========================================================================
//単位変更係数(測定結果表示用)(g_lpszFormat[]・g_lpszThickUnitDecimalPoint[]と対応)
const double g_dUnitConvert[] = {
	0.001, //um
	1,	   //nm
	10,    //A
	//2009.09.20 bagus se  --{--
	1,		//eV
	//2009.09.20 bagus se  --}--
};


// ==========================================================================
//表示単位(XYZ用)(g_lpszXyzDecimalPoint[]・g_dXyzUnitConvertCoefficient[]と対応)
const LPCSTR g_lpszXyzUnit[] = {
	"[mm]", //mm (←固定(シャープ仕様))
	"[um]", //um
};

// ==========================================================================
//単位桁数調整(XYZ用)(g_dXyzUnitConvertCoefficient[]と対応)
const LPCSTR g_lpszXyzDecimalPoint[] = {
	".3",	//mm
	".0",	//um
};

//#define MICROMETRE (1000L)
// ==========================================================================
//単位変更係数(XYZ用)(g_lpszXyzDecimalPoint[]と対応)
const double g_dXyzUnitConvertCoefficient[] = {
//	0.001, //mm
(double)1/MICROMETRE, //mm
	1,	   //um
};

// ==========================================================================
//単位桁数調整(汎用)
const LPCSTR g_lpszDecimalPoint[] = {
	".0",	//-
	".1",	//-
	".2",	//%値系, FIT
	".3",	//-
	".4",	//N,K値系
/* added 2014.05.22 hmenjo scout fit .6f ---------- { ---------- */
	".5",	//-
	".6",	//FIT for scout
/* added 2014.05.22 hmenjo scout fit .6f ---------- } ---------- */
};



// ==========================================================================
//測定モード
enum{
	MEASMODE_NOT_MEASURE = 0,	//測定中でない(測定結果画面のWAIT時は含まない)
	MEASMODE_SEQ_OR_REMEASURE,	//通常測定orリメジャー
	MEASMODE_MANUALMEASURE, 	//マニュアル測定
	MEASMODE_TEST,				//テストモード
//Saiki 20130123 add --->
	MEASMODE_REFERENCE,			//リファレンス測定モード
//Saiki 20130123 add <---
};


// ==========================================================================
//シーケンス測定モードの状態(シーケンス測定モード専用)
enum{
	SEQ_NOT_MEASURE = 0,	//測定中でない(測定結果画面のWAIT時含む)
	SEQ_ON_NORMALMEASURE,	 //通常測定中
	SEQ_ON_REMEASURE,		//リメジャー中
};


// ==========================================================================
//マニュアル測定モードの状態(マニュアル測定モード専用)
enum{
	MANUMODE_INIT = 0,		//マニュアルモード起動直後(リファレンス測定中含む)(測定を一回もやっていない状態)
	MANUMODE_ON_MEASURE,	//測定中(1回目の測定押した時～[測定完了]を押す前までの間)
	MANUMODE_MEAS_COMP, 	//測定終了([測定完了]押下後)
	MANUMODE_NOT,			//マニュアル測定モードでない
};


// ==========================================================================
//テストモードの状態(テストモード専用)
enum{
	TESTMODE_INIT = 0,				//テストモード起動直後
	TESTMODE_CONDITION_CANGE_EDIT,	//取込み条件(Detector Condition)編集中
	TESTMODE_CONDITION_CANGE,		//
	TESTMODE_CONDITION_CANGE_AFTER, //取込み条件(Detector Condition)変更後
	TESTMODE_MEAS_AFTER,			//測定後
	TESTMODE_REF_AFTER, 			//リファレンス測定後
};


// ==========================================================================
//CMeasurementGraphDlgクラス用途
enum{
	GRAPHDLG_MODE_SEQ = 0,		//通常測定用(リメジャー含む)
	GRAPHDLG_MODE_MANUAL,		//マニュアル測定用
	GRAPHDLG_MODE_TEST_NOMAL,	//テストモード用(通常データ)
	GRAPHDLG_MODE_TEST_RAW, 	//テストモード用(RAWデータ)
};


// ==========================================================================
//初期設定の保存名が重複した時の動作判定フラグ用(リモート時のみ使用)
enum
{
	SAVEOVERLAP_SAVE = 0,	//上書き保存(SAVE)
	SAVEOVERLAP_NOSAVE, 	//保存しない(CANCEL)
	SAVEOVERLAP_SAVEAS, 	//名前を付けて保存(SAVE AS)
};


// ==========================================================================
// 動作中フラグ群のアクセス用識別定数
// CNanoSpecDoc::ActuateFlagsSet()，CNanoSpecDoc::ActuateFlagsGet() 関数で使います．
enum ACTUATE_FLAGS_SEL
{
#if 0	// ビット割付に変更します
	ACTUATE_XYSTAGE = 0,	// XY ステージ
	ACTUATE_ZAXIS,			// Z 軸
	ACTUATE_PIN,			// ピン
	ACTUATE_SHUTTER,		// シャッタ
	ACTUATE_WORKGUIDE,		// ワークガイド
	ACTUATE_MAX,			// (使用禁止)
#else	// ビット割付に変更します
	ACTUATE_NON 		= 0x00000000,	// なし
	ACTUATE_XYSTAGE 	= 0x00000001,	// XY ステージ
	ACTUATE_ZAXIS		= 0x00000002,	// Z 軸
	ACTUATE_PIN 		= 0x00000004,	// ピン
	ACTUATE_SHUTTER 	= 0x00000008,	// シャッタ
	ACTUATE_WORKGUIDE	= 0x00000010,	// ワークガイド
	ACTUATE_TURRET		= 0x00000020,	// ターレット
// 2009.11.02 bagus MS 追加 --{--
	ACTUATE_MICROSCOPE	= 0x00000040,	// 顕微鏡
// 2009.11.02 bagus MS 追加 --}--
	ACTUATE_MAX 		= 0xffffffff,	// (使用禁止)
#endif	// ビット割付に変更します
};

enum ACTUATE_MODE_SEL
{
	ACTUATE_MODE_A = 0, 	// Mode A
	ACTUATE_MODE_B, 		// Mode B
	ACTUATE_MODE_C, 		// Mode C
	ACTUATE_MODE_MAX,		// (使用禁止)
};

// 画面ステータス(モード)の画面表示用文字列定義
// 2010.01.19 K.Matsuo -->
////Saiki 20090527 Change ----->
//const LPSTR DISPMODE_STATUS_DISP_ENU[] = {
//	_T("Measurement"),
//	_T("Recipe"),
//	_T("Data"),
//	_T("User Setting"),
//	_T("Maintenance"),
//	_T("System Setting"),
//} ;
//const LPSTR DISPMODE_STATUS_DISP_JPN[] = {
//	_T("測定"),
//	_T("レシピ"),
//	_T("データ"),
//	_T("ユーザ設定"),
//	_T("メンテナンス"),
//	_T("システム設定"),
//} ;
//
////Saiki 20090527 Change <-----
const LPSTR DISPMODE_STATUS_DISP_ENU[] = {
	_T("Measurement"),
	_T("Data"),					// Dataは、存在しない
	_T("Recipe"),
	_T("Alarm"),				// Alarmは、存在しない
	_T("User Setting"),
	_T("Maintenance"),
	_T("System Setting"),
	_T("Manu Measurement"),
} ;
const LPSTR DISPMODE_STATUS_DISP_JPN[] = {
	_T("測定"),
	_T("データ"),				// データは、存在しない
	_T("レシピ"),
	_T("アラーム"),				// アラームは、存在しない
	_T("ユーザ設定"),
	_T("メンテナンス"),
	_T("システム設定"),
	_T("手動測定"),
} ;
// 2010.01.19 K.Matsuo -->

// 2009.11.04 bagus RS 追加 --{--
// ==========================================================================
// Probe Type
// (enumがGlobals.hxx内で定義してある)
const LPCSTR PROBE_TYPE_ITEM[] = {
	"ASP",			// 0
	"ESP",			// 1
	"PSP",			// 2
	"QPP",			// 3
	"NSCP",			// 4
	"TFP",			// 5
	"",				// 6
	"",				// 7
	"",				// 8
	"EXT",			// 9
};
// 2009.11.04 bagus RS 追加 --}--


// ==========================================================================
//
enum NANOSPEC_TITLE
{
	NANOSPEC_TITLE_START = 0,
	NANOSPEC_TITLE_SHOW,
	NANOSPEC_TITLE_HIDE,
// 2009.09.24 K.Matsuo -->
	NANOSPEC_TITLE_PROGRESS,
// 2009.09.24 K.Matsuo <--
// 2009.12.05 K.Matsuo -->
	NANOSPEC_TITLE_MESSAGE2,
// 2009.12.05 K.Matsuo <--
	NANOSPEC_TITLE_CLOSE,
};

// ==========================================================================
//
enum NEXTRA_EXE
{
	NEXTRA_EXE_START = 0,
	NEXTRA_EXE_SHOW,
	NEXTRA_EXE_HIDE,
	NEXTRA_EXE_QUIT,
};

// ==========================================================================
// メンテナンス（校正）定数
enum MAINTENANCE_DAYS
{
	DAYS_90 = 0,			// 90日
	DAYS_180,				// 180日
	DAYS_270,				// 270日
	DAYS_365,				// 365日
	DAYS_MAX,				// (使用禁止)
};
//Saiki 20090410 Change ----->

//#define MAINTENANCE_DAYS_STR { "90days", "180days", "270days", "365days" }
//#define MAINTENANCE_DAYS_STR { "90日", "180日", "270日", "365日" }
#define MAINTENANCE_DAYS_STR_JPN { "90日", "180日", "270日", "365日" }
#define MAINTENANCE_DAYS_STR_ENU { "90days", "180days", "270days", "365days" }
// Kojika 20090528 Change End
// ==========================================================================
// メンテナンス（Lamp）定数
enum MAINTENANCE_LIFETIME
{
	HOURS_500 = 0,			// 500h
	HOURS_1000, 			// 1000h
	HOURS_1500, 			// 1500h
	HOURS_2000, 			// 2000h
	HOURS_2500, 			// 2500h
	HOURS_3000, 			// 3000h
	HOURS_MAX,				// (使用禁止)
};

// Kojika 20090528 Change
//#define MAINTENANCE_LIFETIME_STR { "500h", "1000h", "1500h", "2000h", "2500h", "3000h" }
//#define MAINTENANCE_LIFETIME_STR { "500時間", "1000時間", "1500時間", "2000時間", "2500時間", "3000時間" }
//Saiki 20090410 Change <-----

#define MAINTENANCE_LIFETIME_STR_JPN { "500時間", "1000時間", "1500時間", "2000時間", "2500時間", "3000時間" }
#define MAINTENANCE_LIFETIME_STR_ENU { "500h", "1000h", "1500h", "2000h", "2500h", "3000h" }
// Kojika 20090528 Change End
#define MAINTENANCE_LIFETIME_INIT { 500L, 1000L, 1500L, 2000L, 2500L, 3000L }

// ==========================================================================
// JoyStick Mode
enum
{
	JOY_SOFT_MODE = 0,		// ソフトJoyモード
	JOY_HARD_MODE,			// ハードJoyモード
	JOY_MODE_MAX,
};

/////////////////////////////////////////////////////////////////////////////
// Functions															   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
/////////////////////////////////////////////////////////////////////////////
// ResourceLoadLibrary
#ifdef _MASTER_
HMODULE ResourceLoadLibrary(int langid, LPCTSTR lpszResourceName)
{
	TCHAR l_szResourceFullName[_MAX_FNAME];

	_tcscpy(l_szResourceFullName, lpszResourceName);
	if(langid == LANGUAGE_ENGLISH){
		_tcscat(l_szResourceFullName, "RCENU.dll");
	}
	else{
		_tcscat(l_szResourceFullName, "RCJPN.dll");
	}

	return ::LoadLibrary(l_szResourceFullName);
}
#else
extern HMODULE ResourceLoadLibrary(int langid, LPCTSTR lpszResourceName);
#endif

/////////////////////////////////////////////////////////////////////////////
// LoadStringML
#ifdef _MASTER_
void LoadStringML(int nID, CString& strBuffer, LPCTSTR lpszDefault)
{
	CString strResourceString;

	if(strResourceString.LoadString(nID) == FALSE){
		strResourceString = lpszDefault;
	}

	if(g_lAppNameType != APP_NAME_NANO){
		strResourceString.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}

	strBuffer = strResourceString;
}
#else
extern void LoadStringML(int nID, CString& strBuffer, LPCTSTR lpszDefault);
#endif

/////////////////////////////////////////////////////////////////////////////
// LoadStringML_org
#ifdef _MASTER_
void LoadStringML_org(int nID, CString& strBuffer, LPCTSTR lpszDefault)
{
	CString rString;

	if(rString.LoadString(nID) == FALSE){
		strBuffer = lpszDefault;
	}
	else{
		strBuffer = rString;
	}
}
#else
extern void LoadStringML_org(int nID, CString& strBuffer, LPCTSTR lpszDefault);
#endif
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

/////////////////////////////////////////////////////////////////////////////
// CListCtrl List_FindItem
#ifdef _MASTER_
int List_FindItem(CListCtrl *pList, int col, char* str)
{
	char Buff[256];
	int nListMax;
	int nIndex = 0;
	int retv = -1;

	nListMax = pList->GetItemCount();

	if(nListMax <= 0) return retv;

	if(str[0] == '\0') return retv;

	for(nIndex = 0 ; nIndex < nListMax ; nIndex++){
		pList->GetItemText(nIndex, col, Buff, sizeof(Buff));
		if(strcmp(str, Buff) == 0){
			break;
		}
	}

	if(nIndex >= nListMax) return retv;

	retv = nIndex;

	return retv;
}
#else
extern int List_FindItem(CListCtrl *pList, int col, char* str);
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrl List_SetCursorPosition
#ifdef _MASTER_
int List_SetCursorPosition(CListCtrl *pList, int col, char* str)
{
	char Buff[256];
	int nListMax;
	int nIndex = 0;
	int retv = -1;

	nListMax = pList->GetItemCount();

	if(nListMax <= 0) return retv;

	if(str[0] != '\0'){
		for(nIndex = 0 ; nIndex < nListMax ; nIndex++){
			pList->GetItemText(nIndex, col, Buff, sizeof(Buff));
			if(strcmp(str, Buff) == 0){
				break;
			}
		}

		if(nIndex >= nListMax) nIndex = 0;
	}

	retv = pList->SetSelectionMark(nIndex);
	pList->SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	retv = nIndex;

	return retv;
}
#else
extern int List_SetCursorPosition(CListCtrl *pList, int col, char* str);
#endif

/////////////////////////////////////////////////////////////////////////////
// strbuff
#ifdef _MASTER_
LPSTR strbuff(LPCSTR str, int size)
{
	static char buff[STRFUNC_BUF_LEN+1];
	int len;

	ASSERT(size >= 0 && size <= STRFUNC_BUF_LEN);

	memset(buff, 0, sizeof(buff));
	memset(buff, ' ', size);
	if((len = strlen(str)) > size) len = size;
	memcpy(buff, str, len);

	return buff;
}
#else
extern LPSTR strbuff(LPCSTR str, int size);
#endif

/////////////////////////////////////////////////////////////////////////////
// strspace
#ifdef _MASTER_
LPSTR strspace(int size)
{
	static char buff[STRFUNC_BUF_LEN+1];
	int index;

	ASSERT(size >= 0 && size <= STRFUNC_BUF_LEN);

	memset(buff, NULL, sizeof(buff));

	for (index = 0; index < size; index++)
		buff[index] = ' ';

	return buff;
}
#else
extern LPSTR strspace(int size);
#endif

/////////////////////////////////////////////////////////////////////////////
// strtrim
#ifdef _MASTER_
LPSTR strtrim(LPSTR str)
{
	static char buff[STRFUNC_BUF_LEN+1];
	CString string;

	memset(buff, NULL, sizeof(buff));
	string = str;
	string.TrimLeft();
	string.TrimRight();
	strncpy(buff, string.GetBuffer(0), STRFUNC_BUF_LEN);

	return buff;
}
#else
extern LPSTR strtrim(LPSTR str);
#endif

/////////////////////////////////////////////////////////////////////////////
// strcut
#ifdef _MASTER_
LPSTR strcut(LPSTR str, int first, int count)
{
	ASSERT(first >= 0);
	ASSERT(count >= 0);

	static char buff[STRFUNC_BUF_LEN+1];

	memset(buff, NULL, sizeof(buff));
	memcpy(buff, &str[first], count);

	return buff;
}
#else
extern LPSTR strcut(LPSTR str, int first, int count);
#endif

/////////////////////////////////////////////////////////////////////////////
// strfloat
#ifdef _MASTER_
LPSTR strfloat(LPSTR format, float fdata)
{
	static char buff[STRFUNC_BUF_LEN+1];

	sprintf(buff, format, fdata);

	return buff;
}
#else
extern LPSTR strfloat(LPSTR format, float fdata);
#endif

/////////////////////////////////////////////////////////////////////////////
// strint
#ifdef _MASTER_
LPSTR strint(LPSTR format, int idata)
{
	static char buff[STRFUNC_BUF_LEN+1];

	sprintf(buff, format, idata);

	return buff;
}
#else
extern LPSTR strint(LPSTR format, int idata);
#endif

/////////////////////////////////////////////////////////////////////////////
// strlong
#ifdef _MASTER_
LPSTR strlong(LPSTR format, long ldata)
{
	static char buff[STRFUNC_BUF_LEN+1];

	sprintf(buff, format, ldata);

	return buff;
}
#else
extern LPSTR strlong(LPSTR format, long ldata);
#endif

/////////////////////////////////////////////////////////////////////////////
// getbcd
#ifdef _MASTER_
int bcd_to_int(BYTE bcd)
{
	int data = 0;

	data  = ((bcd & 0xf0) >> 4) * 10;
	data += (bcd & 0x0f);

	return data;
}
#else
extern int bcd_to_int(BYTE bcd);
#endif

/////////////////////////////////////////////////////////////////////////////
// setbcd
#ifdef _MASTER_
BYTE int_to_bcd(int data)
{
	BYTE bcd;

	bcd = (BYTE)((data / 10) << 4) + (BYTE)(data % 10);

	return bcd;
}
#else
extern BYTE int_to_bcd(int data);
#endif

/////////////////////////////////////////////////////////////////////////////
// svrdata
#ifdef _MASTER_
LPSTR svrdata(LPSTR data, char element, int size, LPSTR last)
{
	static char buff[STRFUNC_BUF_LEN+1];
	int len, index, count;

	ASSERT(size < (int)(sizeof(buff)-strlen(last)));

	memset(buff, 0, sizeof(buff));
	len = strlen(data);

	index = 0;
	for (count = 0; count < size; count++)
	{
		if (count < (size-len))
			buff[count] = element;
		else
			buff[count] = data[index++];
	}
	strcat(buff, last);

	return buff;
}
#else
extern LPSTR svrdata(LPSTR data, char element, int size, LPSTR last);
#endif

/////////////////////////////////////////////////////////////////////////////
/// Checkf (for debug)
inline void Checkf(char* pFormat, ...)
{
	HWND	hWnd=FindWindow("CheckListClass", NULL);
	COPYDATASTRUCT	Cds;
	va_list 	pArgp;
	char		Buffer[MAX_PATH]	;

	if(hWnd == NULL)
		return ;
	va_start(pArgp, pFormat);
	vsprintf(Buffer, pFormat, pArgp) ;
	TRACE("%s\n",Buffer);
	Cds.dwData = 0;
	Cds.lpData = (void *)Buffer;
	Cds.cbData = lstrlen(Buffer) + 1;
	SendMessage(hWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&Cds);
}

/////////////////////////////////////////////////////////////////////////////
//
inline void ToTimeStr( time_t tTime, char* szTime )
{
	struct tm*		tBlock;
	tBlock = localtime( &tTime );
	sprintf( szTime, "%04d%02d%02d%02d%02d%02d",
									(tBlock->tm_year+1900),
									tBlock->tm_mon+1,
									tBlock->tm_mday,
									tBlock->tm_hour,
									tBlock->tm_min,
									tBlock->tm_sec );
}

/////////////////////////////////////////////////////////////////////////////
//
inline void ToTimeStr2( time_t tTime, char* szTime )
{
	struct tm*		tBlock;
	tBlock = localtime( &tTime );
	sprintf( szTime, "%02d%02d%02d%02d%02d%02d",
									(tBlock->tm_year+1900) % 100,
									tBlock->tm_mon+1,
									tBlock->tm_mday,
									tBlock->tm_hour,
									tBlock->tm_min,
									tBlock->tm_sec );
}

/////////////////////////////////////////////////////////////////////////////
//
inline void ToTimeStr3( time_t tTime, char* szTime )
{
	struct tm*		tBlock;
	tBlock = localtime( &tTime );
	sprintf( szTime, "%04d.%02d.%02d %02d:%02d:%02d",
									(tBlock->tm_year+1900),
									tBlock->tm_mon+1,
									tBlock->tm_mday,
									tBlock->tm_hour,
									tBlock->tm_min,
									tBlock->tm_sec );
}

// ==========================================================================
// SitePattern Mode
// Kojika 20090528 Change
const LPCSTR g_lpszSitePatternMode_JPN[] = {
	"サイト補正を使用しない",
	"サイト補正を使用する",
};
const LPCSTR g_lpszSitePatternMode_ENU[] = {
	"No Site Pattern Rec.",
	"Site Pattern Rec.",
};
// Kojika 20090528 Change End
// ==========================================================================
// SitePattern Fail Option
// Kojika 20090528 Change
const LPCSTR g_lpszSitePatternFailOption_JPN[] = {
	"強制測定",
	"レシピキャンセル",
	"マニュアル位置決め",
	"前ステップ位置での強制測定",
	"前ステップ位置でのマニュアル位置決め",
};
const LPCSTR g_lpszSitePatternFailOption_ENU[] = {
	"Proceed Measurement",
	"Cancel Recipe",
	"Manual Adjustment",
	"Proceed Measurement in the Prev. Step.",
	"Manual Adjustment in the Prev. Step.",
};
// Kojika 20090528 Change End

// 2009.12.08 bagus --{--
// ==========================================================================
// MotionDlg Pos Button Text
const LPCSTR g_lpszMotionPos_JPN[] = {
	"ポジション1",
	"ポジション2",
	"ポジション3",
	"ポジション4",
};

const LPCSTR g_lpszMotionPos_ENU[] = {
	"Maintenance1",
	"Maintenance2",
	"Maintenance3",
	"Maintenance4",
};
// 2009.12.08 bagus --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// ==========================================================================
// RecipeMainSrView CompEASE Recalib Items
const LPCSTR g_lpszCompEASERecalibItem_JPN[] = {
	"無し",
	"Thick1",
	"Thick2",
	"Thick3",
	"Thick4",
	"Thick5",
	"Thick6",
	"Thick7",
	"Thick8",
	"N1",
	"N2",
	"N3",
	"N4",
	"N5",
	"N6",
	"N7",
	"N8",
	"K1",
	"K2",
	"K3",
	"K4",
	"K5",
	"K6",
	"K7",
	"K8",
	"Eg1",
	"Eg2",
	"Eg3",
	"Eg4",
	"Eg5",
	"Eg6",
	"Eg7",
	"Eg8",
};

const LPCSTR g_lpszCompEASERecalibItem_ENU[] = {
	"None",
	"Thick1",
	"Thick2",
	"Thick3",
	"Thick4",
	"Thick5",
	"Thick6",
	"Thick7",
	"Thick8",
	"N1",
	"N2",
	"N3",
	"N4",
	"N5",
	"N6",
	"N7",
	"N8",
	"K1",
	"K2",
	"K3",
	"K4",
	"K5",
	"K6",
	"K7",
	"K8",
	"Eg1",
	"Eg2",
	"Eg3",
	"Eg4",
	"Eg5",
	"Eg6",
	"Eg7",
	"Eg8",
};
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
#endif	// _SYSTEM_H_
