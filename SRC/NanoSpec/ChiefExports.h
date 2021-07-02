#pragma once

/*-------------------------------------------------------------------
 *	Chief 外部モジュールのための定義ファイル
 *-----------------------------------------------------------------*/

#define	CHIEF_INI	_T("Chief.ini")	// Chief 用設定ファイル名
#define CHIEF_DLG_CAPTION	_T("TRANSITION MONITOR")	// Chief ダイアログのキャプション
//#define ADAP_EXE_PATH	_T("C:\\WVASE32\\Adap\\jaw_adap.exe")	// XMP パス
#define	CHIEF_TRMAS_INIT_TIME	40000	// [ms] マスタトランジション初期化待ち時間
#define	CHIEF_TRMAS_END_TIME	20000	// [ms] マスタトランジション終了待ち時間
#define	TIMER_DELAY_COMPLETE	2000	// Complete ディレイタイマ値[ms]
#define	TIMER_DELAY_ABORT		2000	// Abort ディレイタイマ値[ms]
#define CIE_MINWAVE				(380)	// CIE/OD値最小波長		(SubTransition.cpp から移動)
#define CIE_MAXWAVE				(780)	// CIE/OD値最大波長		(SubTransition.cpp から移動)
#define	TRANSI_TIME				10		// トランジション間タイミング調整(とりあえずだけど恒久かも・・)
#define	CHIEF_STGMVABS_MODE		TRUE	// StageMoveAbsoluteEx() の待ちモード指定(TRUE：待つ，FALSE：待たない)
#define	TIMER_EQPWOFF_TIMEOUT	10000	// 装置電源オフ監視時間[ms]
// トランジション ID 定義
enum TRANSI_ID {
	TRANSI_MASTER = 1,		// MAS	マスタ トランジション
	TRANSI_SEQ,				// SEQ	シーケンス測定 トランジション
	TRANSI_SR_REFER,		// SREF リファレンス測定 トランジション
	TRANSI_DESKEW,			// DSKW デスキュー トランジション
	TRANSI_AF,				// AAF	AF トランジション
	TRANSI_SR_1POINT,		// SR1P １ポイント測定 トランジション
/* added 2009.07.31 hmenjo ストレス機能追加(9) ---------- { ---------- */
	TRANSI_STRESS_SEQ,		/* STRS	ストレス測定 トランジション	*/
/* added 2009.07.31 hmenjo ストレス機能追加(9) ---------- } ---------- */
// bagus add 2014.01.10 -->
	TRANSI_MASTER_STD,		// MAS_STD	マスタスタンダード トランジション
	TRANSI_MASTER_DRK,		// MAS_DRK	マスタダーク トランジション
// bagus add 2014.01.10 <--
	TRANSI_SR_STD_REFER,	// SREF_STD スタンダードリファレンス測定 トランジション
};
// AF VALID データ定義
#if 0
typedef union {
	DWORD	dwValid;
	struct {
		BYTE	ucRetry;				// AF リトライ回数
		BYTE	ucResultAF;				// AF 結果				(0:AF 無し，1:AF 正常，2:自動強制測定，3:自動スキップ，4:AF 失敗，5:レシピ終了)
		BYTE	ucResultPopup;			// ポップアップ結果		(0:ポップアップ無し，1:リトライボタン，2:閉じる(OK)ボタン，3:スキップボタン，4:キャンセルボタン)
										//			ポップアップがあった場合は，≠０になります．
		BYTE	bit1ManuXY		: 1;	// XY マニュアル操作	(1:操作有り)
		BYTE	bit1ManuZ		: 1;	// Z マニュアル操作		(1:操作有り)
		BYTE	bit1ManuAF		: 1;	// AF マニュアル操作	(1:操作有り)
		BYTE	bit1ManuAfRslt	: 1;	// AF マニュアル操作結果(1:マニュアル AF 失敗)
		BYTE	bit3Reserve		: 3;	// (予備)
		BYTE	bit1Err			: 1;	// (予約：使用禁止)(このビットが１の場合，このビット以外のビットはエラーステータスを示す場合があります)
	};
} AF_VALID;
#else
typedef union {			// こちらが最新です．
	DWORD	dwValid;
	struct {
		BYTE	ucRetry;				// AF リトライ回数
		BYTE	ucResultAF		: 3;	// AF 結果				(0:AF 無し，1:AF 正常，2:自動強制測定，3:自動スキップ，4:AF 失敗，5:レシピ終了)
		BYTE	ucResultPopup	: 3;	// ポップアップ結果		(0:ポップアップ無し，1:リトライボタン，2:閉じる(OK)ボタン，3:スキップボタン，4:キャンセルボタン)
										//			ポップアップがあった場合は，≠０になります．
		BYTE	bit2PopupKind	: 2;	// ポップアップ種類		(0:AF のみ，1:PMA のみ，2:AF & PMA)
		BYTE	bitReserve16_23	: 8;	// (予備)
		BYTE	bit1ManuXY		: 1;	// XY マニュアル操作	(1:操作有り)
		BYTE	bit1ManuZ		: 1;	// Z マニュアル操作		(1:操作有り)
		BYTE	bit1ManuAF		: 1;	// AF マニュアル操作	(1:操作有り)
		BYTE	bit1ManuAfRslt	: 1;	// AF マニュアル操作結果(1:マニュアル AF 失敗)
		BYTE	bitReserve28_30	: 3;	// (予備)
		BYTE	bit1Err			: 1;	// (予約：使用禁止)(このビットが１の場合，このビット以外のビットはエラーステータスを示す場合があります)
	};
} AF_VALID;
#endif
// １ポイント測定終了(WM_COPYDATA：WMCD_CHIF_1POINT_END)データ定義
typedef struct {
		DWORD	dwPointNo;		// 終了ポイント番号
		DWORD	dwAFValid;		// AF_VALID データ
		long	lX;				// X 軸位置
		long	lY;				// Y 軸位置
		long	lZ;				// Z 軸位置
} MEAS_1POINT_RESULT;
// 測定開始通知(WM_COPYDATA：WMCD_CHIF_MEAS_START)データ定義
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
enum MEASMODE_ID {
	MEASMODE_NORM = 0,	/* 通常測定(SR，SE，透過率)	*/
	MEASMODE_GTRREF,	/* ガントリ透過率リファレンス測定	*/
//2009.11.03 bagus 2poin-distance 削除 --{--
//	MEASMODE_DISTANCE,	/* 2点間距離 */
//2009.11.03 bagus 2poin-distance 削除 --}--
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- { ---------- */
	MEASMODE_CTA,		/* CTA 測定	*/
/* added 2009.10.29 hmenjo CTA Seq API 追加 ---------- } ---------- */
//2009.11.03 bagus MS 追加 --{--
	MEASMODE_MS,
//2009.11.03 bagus MS 追加 --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	MEASMODE_COMPEASE,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
};
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
typedef struct {
	BOOL	bGetSamplID;		// FALSE：サンプル ID の指定は無い(画面からのシーケンス測定開始のため)
								// TRUE	：サンプル ID の指定が有る(Pif からのシーケンス測定開始)
	TCHAR	szSampleID[256];	// サンプル ID 文字列
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- { ---------- */
	TCHAR	tszLotID[256];		/* ロット ID 文字列 (bGetSamplID に依る)	*/
/* added 2009.10.29 hmenjo CTA ロット ID 対応 ---------- } ---------- */
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- { ---------- */
	MEASMODE_ID	enMeasMode;		/* 測定モード MEASMODE_ID	*/
/* added 2009.09.09 hmenjo 透過率 Seq リファレンスモード(16) ---------- } ---------- */
} SEQ_MEAS_START;

/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- { ---------- */
typedef struct {
	UINT nFocus;
	UINT nAutoFocusFailOption;
} HEAD_MAIN_RCP_INFO, *LPHEAD_MAIN_RCP_INFO;
typedef const HEAD_MAIN_RCP_INFO *LPCHEAD_MAIN_RCP_INFO;
/* added 2009.09.29 hmenjo Head 毎 Main Rcp Inf 読出し ---------- } ---------- */

/* added 2009.10.15 hmenjo Stage PGM 共通化対応 ヘッドタイプ書換 ---------- { ---------- */
/* ヘッドタイプ STD 変換テーブル	*/
const WORD HEADTYPE_STD_TBL[] = {
	HEAD_TYPE_SR,
	HEAD_TYPE_SE,
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	HEAD_TYPE_COMPEASE,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	HEAD_TYPE_IRSE,
// 2009.10.19 bagus MS 追加 --}--
	HEAD_TYPE_4PP,
	HEAD_TYPE_CTA,
// 2009.10.19 bagus MS 追加 --{--
	HEAD_TYPE_MS,
// 2009.10.19 bagus MS 追加 --}--
	-1,
};
/* added 2009.10.15 hmenjo Stage PGM 共通化対応 ヘッドタイプ書換 ---------- } ---------- */

/*
 *	各種アラーム報告用定義
 */
/*			パラメタ説明
 *				wparam
 *						[１]：LOBYTE(LOWORD(wparam))	報告方法
 *															メッセージボックスの表示のみ
 *															アラーム報告のみ
 *															両方
 *						[２]：HIBYTE(LOWORD(wparam))	親通知(CMainFrame)
 *															しない
 *															する(その後の処理は親次第です)
 *						[３]：LOBYTE(HIWORD(wparam))	メッセージボックスのタイトル番号
 *						[４]：HIBYTE(HIWORD(wparam))	メッセージボックスのボタン表示
 *															OK
 *															YES/NO(YES の場合は親通知時のアプリ終了パラメタを TRUE にする)
 *				lparam
 *						[５]：LOWORD(lparam)			メッセージボックスのテキスト番号
 *						[６]：HIWORD(lparam)			アラーム報告の ALID
 *			指定方法
 *				PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD([１], [２]), MAKEWORD([３], [４])), MAKELPARAM([５], [６]));
 */
enum CHIEF_REP_ALARM_METHOD {		// [１]報告方法
	CHRAMTD_NONE = 0,					// (何もしない)
	CHRAMTD_MSGBOX,						// メッセージボックスの表示のみ
	CHRAMTD_REP_ALARM,					// アラーム報告のみ
	CHRAMTD_BOTH,						// 両方
};
enum CHIEF_REP_ALARM_NOTIFY {		// [２]親通知
	CHRANFY_NOTIFY_OFF = 0,				// しない
	CHRANFY_NOTIFY_ON,					// する(その後の処理は親次第です)
};
enum CHIEF_REP_ALARM_MSGBUTTON {	// [４]メッセージボックスのボタン表示
	CHRAMSG_OK = 0,						// OK
	CHRAMSG_YESNO,						// YES/NO(YES の場合は親通知時のアプリ終了パラメタを TRUE にする)
};
const LPSTR CHIEF_REP_ALARM_MSGTITLE[] = {		// [３]メッセージボックスのタイトル
	/*	0 */ _T("NanoSpec Chief CChiefThread"),
	/*	1 */ _T("NanoSpec Chief CChiefView"),
	/*	2 */ _T("NanoSpec Chief CChiefRcvMailThread"),
	/*	3 */ _T("NanoSpec Chief CChiefTransiAF"),
	/*	4 */ _T("NanoSpec Chief CChiefTransiDeskew"),
	/*	5 */ _T("NanoSpec Chief CChiefTransiMaster"),
	/*	6 */ _T("NanoSpec Chief CChiefTransiSeq"),
	/*	7 */ _T("NanoSpec Chief CChiefTransiSr1Point"),
	/*	8 */ _T("NanoSpec Chief CChiefTransiSrRefer"),
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
	/*	9 */ _T("NanoSpec Chief CChiefTransiStress"),
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
	/*	0 */ _T("NONE"),
};
const LPSTR CHIEF_REP_ALARM_MSGTEXT[] = {		// [５]メッセージボックスのテキスト
	/*	0 */ _T("Failed to Start timer(SetTimer()) for Trace Data. \nTerminate this application ???"),
	/*	1 */ _T("Failed to Kill timer(KillTimer()) for Trace Data. \nTerminate this application ???"),
	/*	2 */ _T("Failed to Start Timer(SetTimer()) to Autohide Chief Dialog. \nTerminate this application ???"),
	/*	3 */ _T("Failed to Kill Timer(KillTimer()) to Autohide Chief Dialog. \nTerminate this application ???"),
	/*	4 */ _T("Failed to Start Thread for Master Transition (CChiefTransiMaster). \nTerminate this application ???"),
	/*	5 */ _T("Failed to Start Thread for Sequence Transition (CChiefTransiSeq). \nTerminate this application ???"),
	/*	6 */ _T("Failed to Start Thread for SR 1 Point Transition (CChiefTransiSr1Point). \nTerminate this application ???"),
	/*	7 */ _T("Failed to Start Thread for SR Reference Transition (CChiefTransiSrRefer). \nTerminate this application ???"),
	/*	8 */ _T("Failed to Start Thread for Deskew Transition (CChiefTransiDeskew). \nTerminate this application ???"),
	/*	9 */ _T("Failed to Start Thread for AF Transition (CChiefTransiAF). \nTerminate this application ???"),
	/* 10 */ _T("Failed to Start Thread for Receive Mail (CChiefRcvMailThread). \nTerminate this application ???"),
	/* 11 */ _T("Failed to Start timer(SetTimer() : 10ms period). \nTerminate this application ???"),
	/* 12 */ _T("Failed to Start timer(SetTimer() : 50ms period). \nTerminate this application ???"),
	/* 13 */ _T("Failed to Start timer(SetTimer() : 100ms period). \nTerminate this application ???"),
	/* 14 */ _T("Failed to Start timer(SetTimer() : 1s period). \nTerminate this application ???"),
	/* 15 */ _T("Failed to Start Multimedia timer(::timeSetEvent()). \nTerminate this application ???"),
	/* 16 */ _T("Received undefined command from Pif."),
	/* 17 */ _T("Failed to create Event for Master Transition initialized. \nTerminate this application ???"),
	/* 18 */ _T("Failed to initialize Master Transition. \nTerminate this application ???"),
	/* 19 */ _T("Failed to start process (CreateProcess) Adap. \nTerminate this application ???"),
	/* 20 */ _T("PatternRec Initialize Error. \nTerminate this application ???"),
	/* 21 */ _T("Stage Initialize Error. \nTerminate this application ???"),
	/* 22 */ _T("Measys Initialize Error. \nTerminate this application ???"),
	/* 23 */ _T("Failed to terminate Master Transition."),
	/* 24 */ _T("Failed to allocate memory for Main Recipe. \nTerminate this application."),
	/* 25 */ _T("Failed to allocate memory for Measurement Program. \nTerminate this application."),
	/* 26 */ _T("Failed to allocate memory for Stage Program. \nTerminate this application."),
	/* 27 */ _T("Failed to allocate memory for Scan Points. \nTerminate this application."),
	/* 28 */ _T("Failed to read Recipe (Main Recipe)."),
	/* 29 */ _T("Failed to read Recipe (Measurement Program)."),
	/* 30 */ _T("Failed to read Recipe (Stage Program)."),
	/* 31 */ _T("Failed to read Recipe (Out of Range for number of measurement points)."),
	/* 32 */ _T("Failed to read Recipe (Measurement points)."),
	/* 33 */ _T("Failed to read Recipe (others)."),
	/* 34 */ _T("Failed to Start timer(SetTimer() for EQ Power OFF timeout). \nTerminate this application ???"),
	/* 35 */ _T("Failed to Kill timer(KillTimer() for EQ Power OFF timeout). \nTerminate this application ???"),
// 2009.05.17 myanagida -->
	/* 36 */ _T("Failed to allocate memory for SitePattern table. \nTerminate this application."),
// 2009.05.17 myanagida <--
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- { ---------- */
	/* 37 */ _T("Failed to Start timer(SetTimer()) for HEPA stop wait. \nTerminate this application ???"),
	/* 38 */ _T("Failed to Kill timer(KillTimer()) for HEPA stop wait. \nTerminate this application ???"),
/* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- } ---------- */
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
	/* 39 */ _T("Failed to TransiEvent(PostThreadMessage). \nTerminate this application ???"),
	/* 40 */ _T("Transition busy. \nTerminate this application ???"),
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
	/*	0 */ _T("NONE"),	//	0：
};

//Saiki 20090602 Delete ----->
/*
 *	以下は MainFrame のステータスバーへ表示するテキスト定義です．
 *		将来，リソースの String table のリソース ID になる可能性があるため，
 *		似たような名前で定義します．
 */
/*//// Chief ダイアログでの制御
//#define	IDS_CHIF_MLDPOS_MOVING				_T("マニュアルロードポジションへ移動中・・・")
////#define IDS_CHIF_MLDPOS_MOVED				_T("マニュアルロードポジションへ移動完了．")
//#define	IDS_CHIF_MLDPOS_MOVED				_T("")
//#define	IDS_CHIF_HLDPOS_MOVING				_T("ハンドラロードポジションへ移動中・・・")
////#define IDS_CHIF_HLDPOS_MOVED				_T("ハンドラロードポジションへ移動完了．")
//#define	IDS_CHIF_HLDPOS_MOVED				_T("")
//#define	IDS_CHIF_GENPOS_MOVING				_T("汎用ポジションへ移動中・・・")
////#define IDS_CHIF_GENPOS_MOVED				_T("汎用ポジションへ移動完了・・・")
//#define	IDS_CHIF_GENPOS_MOVED				_T("")
//#define	IDS_CHIF_HWS_MLDPOS_MOVING			_T("[ハードシミュレートモード]マニュアルロードポジションへ移動中・・・")
////#define IDS_CHIF_HWS_MLDPOS_MOVED			_T("[ハードシミュレートモード]マニュアルロードポジションへ移動完了．")
//#define	IDS_CHIF_HWS_MLDPOS_MOVED			_T("")
//#define	IDS_CHIF_HWS_HLDPOS_MOVING			_T("[ハードシミュレートモード]ハンドラロードポジションへ移動中・・・")
////#define IDS_CHIF_HWS_HLDPOS_MOVED			_T("[ハードシミュレートモード]ハンドラロードポジションへ移動完了．")
//#define	IDS_CHIF_HWS_HLDPOS_MOVED			_T("")
//#define	IDS_CHIF_HWS_GENPOS_MOVING			_T("[ハードシミュレートモード]汎用ポジションへ移動中・・・")
////#define IDS_CHIF_HWS_GENPOS_MOVED			_T("[ハードシミュレートモード]汎用ポジションへ移動完了・・・")
//#define	IDS_CHIF_HWS_GENPOS_MOVED			_T("")
//// リファレンス測定
//#define IDS_CHIF_SREF_HWS_N_1ST_MEASING		_T("リファレンス測定：[ハードシミュレートモード](ノーマル)1stリファレンス測定中・・・")
//#define IDS_CHIF_SREF_HWS_D_T1_MEASING		_T("リファレンス測定：[ハードシミュレートモード]ダークリファレンスT1測定中・・・")
//#define IDS_CHIF_SREF_HWS_D_T1_COMPLETE		_T("リファレンス測定：[ハードシミュレートモード]ダークリファレンスT1測定完了．")
//#define IDS_CHIF_SREF_N_STAGE_MOVING		_T("リファレンス測定：(ノーマル)リファレンス測定位置(ＸＹ)へ移動中・・・")
//#define IDS_CHIF_SREF_N_STAGE_MOVED			_T("リファレンス測定：(ノーマル)リファレンス測定位置(ＸＹ)へ移動完了．")
//#define	IDS_CHIF_SREF_N_Z_MOVING			_T("リファレンス測定：(ノーマル)リファレンス測定位置(Ｚ)へ移動中・・・")
//#define	IDS_CHIF_SREF_N_Z_MOVED 			_T("リファレンス測定：(ノーマル)リファレンス測定位置(Ｚ)へ移動完了．")
//#define	IDS_CHIF_SREF_N_SWAYSETTLING		_T("リファレンス測定：(ノーマル)リファレンス：ヘッド揺れ収束待ち・・・")
//#define	IDS_CHIF_SREF_N_FOCUSING			_T("リファレンス測定：(ノーマル)リファレンス：フォーカス中・・・")
//#define	IDS_CHIF_SREF_N_FOCUS_FAILED		_T("リファレンス測定：(ノーマル)リファレンス：フォーカス失敗．")
//#define	IDS_CHIF_SREF_N_1ST_MEASING 		_T("リファレンス測定：(ノーマル)1stリファレンス測定中・・・")
//#define	IDS_CHIF_SREF_N_1ST_MEAS_FAILED 	_T("リファレンス測定：(ノーマル)1stリファレンス測定失敗．")
//#define	IDS_CHIF_SREF_N_2ND_MEASING 		_T("リファレンス測定：(ノーマル)2ndリファレンス測定中・・・")
//#define	IDS_CHIF_SREF_N_2ND_T1_MEAS_FAILED	_T("リファレンス測定：(ノーマル)2ndリファレンスT1測定失敗．")
//#define	IDS_CHIF_SREF_N_2ND_T2_MEAS_FAILED	_T("リファレンス測定：(ノーマル)2ndリファレンスT2測定失敗．")
//#define	IDS_CHIF_SREF_D_STAGE_MOVING		_T("リファレンス測定：ダークリファレンス測定位置(ＸＹ)へ移動中・・・")
//#define	IDS_CHIF_SREF_D_STAGE_MOVED 		_T("リファレンス測定：ダークリファレンス測定位置(ＸＹ)へ移動完了．")
//#define	IDS_CHIF_SREF_D_Z_MOVING			_T("リファレンス測定：ダークリファレンス測定位置(Ｚ)へ移動中・・・")
//#define	IDS_CHIF_SREF_D_Z_MOVED 			_T("リファレンス測定：ダークリファレンス測定位置(Ｚ)へ移動完了．")
//#define	IDS_CHIF_SREF_D_SWAYSETTLING		_T("リファレンス測定：ダークリファレンス：ヘッド揺れ収束待ち・・・")
//#define	IDS_CHIF_SREF_D_T1_MEASING			_T("リファレンス測定：ダークリファレンスT1測定中・・・")
//#define	IDS_CHIF_SREF_D_T1_MEAS_FAILED		_T("リファレンス測定：ダークリファレンスT1測定失敗．")
//#define	IDS_CHIF_SREF_D_T2_MEASING			_T("リファレンス測定：ダークリファレンスT2測定中・・・")
//#define	IDS_CHIF_SREF_D_T2_MEAS_FAILED		_T("リファレンス測定：ダークリファレンスT2測定失敗．")
//#define	IDS_CHIF_SREF_COMPLETED 			_T("リファレンス測定：完了．")
//// デスキュー
//#define	IDS_CHIF_DSKW_HWS_DESKEWING 		_T("デスキュー：[ハードシミュレートモード]デスキュー中・・・")
//#define	IDS_CHIF_DSKW_HWS_DESKEWED			_T("デスキュー：[ハードシミュレートモード]デスキュー完了．")
//#define	IDS_CHIF_DSKW_FOCUSING				_T("デスキュー：フォーカス中・・・")
//#define	IDS_CHIF_DSKW_FOCUS_CANCEL			_T("デスキュー：フォーカス キャンセル．")
//#define	IDS_CHIF_DSKW_AUTO_ING				_T("デスキュー：オートデスキュー中・・・")
//#define	IDS_CHIF_DSKW_AUTO_COMPLETE 		_T("デスキュー：オートデスキュー完了．")
//#define	IDS_CHIF_DSKW_AUTO_FAILED			_T("デスキュー：オートデスキュー失敗．")
//#define	IDS_CHIF_DSKW_AUTO_CANCEL			_T("デスキュー：オートデスキュー キャンセル．")
//#define	IDS_CHIF_DSKW_MANU_ING				_T("デスキュー：マニュアルデスキュー中・・・")
//#define	IDS_CHIF_DSKW_MANU_COMPLETE 		_T("デスキュー：マニュアルデスキュー完了．")
//#define	IDS_CHIF_DSKW_MANU_CANCEL			_T("デスキュー：マニュアルデスキュー キャンセル．")
//#define	IDS_CHIF_DSKW_MANU_IGNORE			_T("デスキュー：マニュアルデスキュー無効．")
//// １ポイント測定
//#define	IDS_CHIF_SR1P_Z_MOVING				_T("ポイント測定：測定位置(Ｚ)へ移動中・・・")
//#define	IDS_CHIF_SR1P_Z_MOVED				_T("ポイント測定：測定位置(Ｚ)へ移動完了．")
//#define	IDS_CHIF_SR1P_FOCUSING				_T("ポイント測定：フォーカス中・・・")
//#define	IDS_CHIF_SR1P_FOCUS_ABORTED 		_T("ポイント測定：フォーカス中断．")
//#define	IDS_CHIF_SR1P_HWS_MEASING			_T("ポイント測定：[ハードシミュレートモード]測定中・・・")
//#define	IDS_CHIF_SR1P_MEASING				_T("ポイント測定：測定中・・・")
//#define	IDS_CHIF_SR1P_MEAS_FAILED			_T("ポイント測定：測定失敗．")
//#define	IDS_CHIF_SR1P_ANALYZING 			_T("ポイント測定：解析中・・・")
//#define	IDS_CHIF_SR1P_ANALYZE_FAILED		_T("ポイント測定：解析失敗．")
//#define	IDS_CHIF_SR1P_DATA_PROCING			_T("ポイント測定：データ処理中・・・")
//#define	IDS_CHIF_SR1P_DATA_PROC_COMPLETE	_T("ポイント測定：データ処理完了．")
//// 2009.05.11 K.Matsuo SitePR -->
//// サイトパターン
//#define	IDS_CHIF_SITEPR_AUTO_ING			_T("サイトパターン：オートサイトパターン中・・・")
//#define	IDS_CHIF_SITEPR_AUTO_COMPLETE		_T("サイトパターン：オートサイトパターン完了．")
//#define	IDS_CHIF_SITEPR_AUTO_FAILED 		_T("サイトパターン：オートサイトパターン失敗．")
//#define	IDS_CHIF_SITEPR_AUTO_CANCEL 		_T("サイトパターン：オートサイトパターン キャンセル．")
//#define	IDS_CHIF_SITEPR_MANU_ING			_T("サイトパターン：マニュアルサイトパターン中・・・")
//#define	IDS_CHIF_SITEPR_MANU_COMPLETE		_T("サイトパターン：マニュアルサイトパターン完了．")
//#define	IDS_CHIF_SITEPR_MANU_CANCEL 		_T("サイトパターン：マニュアルサイトパターン キャンセル．")
//// 2009.05.11 K.Matsuo SitePR <--
//// シーケンス測定
//#define	IDS_CHIF_SEQ_STAGE_PREMOVING		_T("シーケンス測定：測定位置(ＸＹ)へ先行移動中・・・")
//#define IDS_CHIF_SEQ_STAGE_MOVED			_T("シーケンス測定：測定位置(ＸＹ)へ移動完了．")
//#define IDS_CHIF_SEQ_STAGE_MOVING 		_T("シーケンス測定：測定位置(ＸＹ)へ移動中・・・")
//#define IDS_CHIF_SEQ_SWAYSETTLING 		_T("シーケンス測定：ヘッド揺れ収束待ち・・・")
//#define IDS_CHIF_SEQ_DATA_PROCING 		_T("シーケンス測定：データ処理中・・・")
//#define IDS_CHIF_SEQ_DATA_PROC_COMPLETE		_T("シーケンス測定：データ処理完了．")
//#define IDS_CHIF_SEQ_CANCELING				_T("シーケンス測定：キャンセル停止中・・・")
//#define IDS_CHIF_SEQ_CANCELED 			_T("シーケンス測定：キャンセル完了．")
//#define IDS_CHIF_SEQ_PAUSING				_T("シーケンス測定：一時停止中(ポーズ)・・・")
//Saiki 20090602 Delete*/

/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
/*
 *	内部ウィンドウメッセージチェック用 定義
 */
#define	CHIEF_TREVENT_RETRY_COUNT		10		/* WM_CHIF_TRxxxx_SETEVENT リトライ回数	*/
#define	CHIEF_TREVENT_RETRY_INTERVAL	50		/* WM_CHIF_TRxxxx_SETEVENT リトライ間隔[ms]	*/
#define	CHIEF_CHECK_BUSY_TIME			1000	/* ビジーチェック時間[ms]	*/
#define	CHIEF_CHECK_BUSY_INTERVAL		100		/* ビジーチェック間隔[ms]	*/
/* added 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
