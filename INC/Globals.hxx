#if !defined( _GLOBALS_HXX_ )
#define _GLOBALS_HXX_

#pragma once
//Saiki 20090525 Add ----->
// 2013.11.08 Bagus Delete (TohoSpec対応) -->
//#if !defined( ResourceLoadLibrary )
//#define ResourceLoadLibrary(langid, ResourceName) ::LoadLibrary(langid == LANGUAGE_ENGLISH ? ResourceName##"RCENU.dll" : ResourceName##"RCJPN.dll")
//#endif
//#if !defined( LoadStringML )
//#define LoadStringML(nID,Buffer,Default)				  \
//		{												  \
//			CString rString;							  \
//				if(rString.LoadString(nID) == FALSE ){	  \
//				Buffer = Default;						  \
//			}											  \
//			else{										  \
//				Buffer = rString;						  \
//			}											  \
//		  }
//#endif
// 2013.11.08 Bagus Delete (TohoSpec対応) <--
//Saiki 20090525 Add <-----
/////////////////////////////////////////////////////////////////////////////
// DEFINE                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// 2009.08.20 K.Matsuo ALID change (Ref. alarmIf.hxx) -->
#if 0
/* modified 2009.08.17 hmenjo ALID 定義を変更 ---------- { ---------- */
///* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- { ---------- */
//#define	ZPOSOFF_ALID		(43)	/* ストレス Z 位置オフ アラーム ID	*/
//#define	LDPOSSTAFAIL_ALID	(44)	/* ロードポジション移動開始失敗 アラーム ID	*/
//#define	LIFTERSTAFAIL_ALID	(45)	/* リフタ動作開始失敗 アラーム ID	*/
///* added 2009.08.17 hmenjo ストレス機能追加(37) ---------- } ---------- */
///* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- { ---------- */
//#define	HEPA_ALID			(42)	/* HEPA アラーム ID	*/
///* added 2009.07.31 hmenjo ストレス機能追加(3) ---------- } ---------- */
/* modified 2009.08.17 hmenjo ALID 定義を変更 ----------			  */
#define	ALID_HEPA			(42)	/* HEPA アラーム ID	*/
#define	ALID_ZPOSOFF		(43)	/* ストレス Z 位置オフ アラーム ID	*/
#define	ALID_LDPOSSTAFAIL	(44)	/* ロードポジション移動開始失敗 アラーム ID	*/
#define	ALID_LIFTERSTAFAIL	(45)	/* リフタ動作開始失敗 アラーム ID	*/
/* modified 2009.08.17 hmenjo ALID 定義を変更 ---------- } ---------- */
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- { ---------- */
#define	ALID_SAFETYPLUG		(46)	/* セーフティプラグ アラーム ID	*/
/* added 2009.08.17 hmenjo セーフティプラグ処理追加 ---------- } ---------- */
#endif // 0
// 2009.08.20 K.Matsuo ALID change <--

/* deleted 2009.09.29 hmenjo Stress Z 軸スキャン位置 ---------- { ---------- */
///* added 2009.08.04 hmenjo ストレス機能追加(19) ---------- { ---------- */
//// 2009.08.21 K.Matsuo -->
////#define	Z_RETRACT_POS	(1000000L)	/* [0.1um] Z 軸干渉外 位置	*/
//#define	Z_RETRACT_POS	(420000L)	/* [0.1um] Z 軸干渉外 位置	*/
//// 2009.08.21 K.Matsuo <--
///* added 2009.08.04 hmenjo ストレス機能追加(19) ---------- } ---------- */
/* deleted 2009.09.29 hmenjo Stress Z 軸スキャン位置 ---------- } ---------- */
/* added 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
#define	NO_COMPLETE_MSGTXT		/* 完了メッセージ表示しない(SetMessageText() 用)	*/
#define	NO_COMPLETE_MSG			/* 完了メッセージ表示しない(ロードポジション用)	*/
/* added 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */

// ==========================================================================
// 文字列長
// ==========================================================================
#define RECIPE_NAME_LEN						(24)
#define PATTERN_NAME_LEN					(24)
#define RECIPE_COMMENT_LEN					(80)
#define STRATEGY_NAME_LEN					(255)
#define COMPEASE_RECIPE_NAME_LEN			(255)
#define SPECIFIED_WAVELENGTH_MAX			(3)
#define USER_NAME_LEN						RECIPE_NAME_LEN					// UserAccountのUser名長さ
 //※ USER_NAME_LEN ≦ RECIPE_NAME_LENを保つこと! (『ユーザアカウント設定』関係で不都合が出る)
#define PASSWORD_LEN						(12)							// UserAccountのパスワードの長さ
#define DATE_LEN							(23)							// 日付の文字数
#define GENERAL_POSITION_COMMENT_LEN		(20)							// 汎用位置 コメント長さ
#define IP_ADDRESS_LEN						(15)							// IP Address[***.***.***.***]長さ
#define SAMPLE_ID_LEN						(32)							// サンプルID名の長さ
#define DISP_UNIT_NAME_LEN					(4)								// 測定結果単位名の長さ
#define MAINTENANCE_POSITION_COMMENT_LEN	(20)							// メンテナンス位置 コメント長さ
#define SHOW_AF_VALID_LEN					(5)								//測定結果表示画面のFOCUS項目の最大文字数
#define SAVE_FILE_PATH_NAME_LEN				(255)
#define AF_HEADER_LEN						(2)								//測定結果用のグリッドのタイトル(AF)
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
#define	MATERIAL_NAME_LEN					(24)							/* マテリアル名 最大文字数	*/
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
// 2009.10.08 bagus CTA 追加 --{--
#define LOT_ID_LEN							(32)							// ロットID名の長さ
// 2009.10.08 bagus CTA 追加 --}--


// ==========================================================================
// 最大数
// ==========================================================================
#define MAX_AXIS							(3)								// AXES DEFINITIONS
#define SCAN_POINT_MAX						(10000)							// 測定のポイント数 最大
/* added 2009.10.26 hmenjo CTA 最大ポイント数 ---------- { ---------- */
#define SCAN_POINT_CTA_MAX					(1000)							/* CTA 測定のポイント数 最大	*/
/* added 2009.10.26 hmenjo CTA 最大ポイント数 ---------- } ---------- */
#define TESTDATA_POINT_MAX					(100)							// テストモード最大測定データ数
#define ADAPRESULT_ROWS_MAX 				(SCAN_POINT_MAX)				// ADAP結果データ
#define ADAPRESULT_COLS_MAX 				(100)							// ADAP結果データ
#define MULTI_RCP_MAIN_RCP_MAX				(100)							// MultiRecipeのMainRecipe数 最大
#define GENERAL_POSITION_MAX				(10)							// 汎用位置数 最大
#define SR_FILTER_MAX						(3)								// SRフィルタ数 最大
#define SR_TURRET_RAY_MAX					(6)								// SRのターレット設定のRayの数 最大
#define SR_LENS_MAX							(4)								// SRのレンズの数 最大
#define SCANDATA_POINT_MAX					(1000)							// SCANDATA_POINT 最大
#define MAINTENANCE_POSITION_MAX			(4)								// メンテナンス位置数 最大
#define GRID_COL_MAX						(30)							//MeasurementGraphDlg,MeasurementListDlgのグリッド最大表示カラム数

#define SITE_PATTERN_MAX					(2)
/* added 2009.07.22 hmenjo ストレス ライン数 定義２ ---------- { ---------- */
#define	STRESS_LINES_MAX					(20)						/* ストレス 最大ライン数		*/
#define	STRESS_PINS_MAX						(20)						/* ストレス 最大ピン数			*/
#define	STRESS_SECTIONS_MAX					(20)						/* ストレス 最大セクション数	*/
#define	DEFL_CNT_MAX						(5000)						/* 変位データ 最大数			*/
#define	DYDX_CNT_MAX						(5000)						/* 傾きθデータ 最大数			*/
// 2009.08.05 K.Matsuo -->
//#define	DTHETADX_CNT_MAX					(5000)						/* dθ/dx（傾きθの傾き）= 1/R（曲率） 、R（曲率半径）データ 最大数	*/
#define	STRESS_CNT_MAX						(5000)						/* ストレスデータ 最大数 */
// 2009.08.05 K.Matsuo <--
/* added 2009.07.22 hmenjo ストレス ライン数 定義２ ---------- } ---------- */
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
#define	STRESS_MODULUS_MAX					(20)						/* 弾性係数テーブル 最大数		*/
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
//Saiki 20110207 Add ----->
#define	PIN_INTERVAL						(7)							/* ストレス 最大測定ポイント数	*/
//Saiki 20110207 Add <-----

// 2009.10.14 bagus Distance 追加 --{--
#define	DISTANCE_POINT_MAX						(5000)						/* 2点間データ 最大数 */
// 2009.10.14 bagus Distance 追加 --}--

// 2009.09.28 bagus gantry --{--
#define	MIN_WAITTIME_SHUTTER_MOVING		0.0
#define	MAX_WAITTIME_SHUTTER_MOVING		1000.0
//2009.12.07 bagus GTR --{--
//#define	MIN_GANTRY_MEAS_POINT_NUM		0
#define	MIN_GANTRY_MEAS_POINT_NUM		1
//2009.12.07 bagus GTR --}--
// 2009.10.27 bagus Gantry 追加修正 --{--
//#define	MAX_GANTRY_MEAS_POINT_NUM		20
#define	MAX_GANTRY_MEAS_POINT_NUM		25
// 2009.10.27 bagus Gantry 追加修正 --}--
// 2009.09.28 bagus gantry --}--

// 2009.10.19 bagus MS 追加 --{--
#define	MAX_MS_ROBO_CYLINDER_NUM			(6)
#define	MS_ROBO_CYLINDER_NAME_LEN			(16)
#define	MAX_MS_ROBO_CYLINDER_POSITION_NUM	(64)
// 2009.10.19 bagus MS 追加 --}--

// 2009.10.30 bagus CA 追加 --{--
#define	MIN_ZAXIS_POSITION				10000L
#define	MAX_ZAXIS_POSITION				50000L
// 2009.10.30 bagus CA 追加 --}--

// 2009.10.30 bagus MS 追加 --{--
#define	MIN_STD_ZAXIS_RETRACT_POS		10000L
#define	MAX_STD_ZAXIS_RETRACT_POS		50000L
// 2009.10.30 bagus MS 追加 --}--

// 2013.02.22 bagus Substrate thickness setting -->
#define	SUBSTRATE_THICKNESS_MAX				(51)	// 板厚テーブル 最大数（50件＋リファレンス）
// 2013.02.22 bagus Substrate thickness setting <--
/* added 2016.08.05 hmenjo レシピ編集 menu バグ ---------- { ---------- */
#define	RCP_PGM_NUM_MAX		10000	/* レシピ，プログラム 最大数	*/
/* added 2016.08.05 hmenjo レシピ編集 menu バグ ---------- } ---------- */

// ==========================================================================
// ディレクトリのパスの設定
#define BIN_DIR								"..\\BIN\\"
#define LOG_DIR								"..\\LOG\\"
#define CFG_DIR								"..\\CFG\\"
#define CFG_SYSTEM_DIR						CFG_DIR "SYSTEM\\"
#define CFG_SYSTEM_SAMPLE_DIR				CFG_SYSTEM_DIR "SAMPLE\\"					// 設定サンプルサイズテーブルパス名
#define CFG_SYSTEM_SR_REFERENCE_MATERIAL	CFG_SYSTEM_DIR "SR_ReferenceMaterial\\"		// 設定リファレンスストラテジーテーブルパス名
#define CFG_USER_DIR						CFG_DIR "USER\\"
#define CFG_USER_USER_ACCOUNT_DIR			CFG_USER_DIR "USERACCOUNT\\"
//2009.12.10 bagus Recicpe Backup --{--
#define DB_DIR_NAME							"DB"
#define DB_MAIN_RECIPE_DIR_NAME				"MAINRECIPE"
#define DB_MULTI_RECIPE_DIR_NAME			"MULTIRECIPE"
#define DB_STAGE_PROGRAM_DIR_NAME			"STAGEPGM"
#define DB_DESKEW_IMG_DIR_NAME				"DESKEW_IMG"
#define DB_PATTERN_DIR_NAME					"PATTERN"
#define DB_PATTERN_IMG_DIR_NAME				"PATTERN_IMG"
#define DB_RECALIBRATION_PROGRAM_DIR_NAME	"RECALIBRATIONPGM"
#define DB_POINT_DESKEW_PROGRAM_DIR_NAME	"POINTDESKEWPGM"

//2009.12.10 bagus Recicpe Backup --}--
#define DB_DIR								"..\\DB\\"
#define DB_MAIN_RECIPE_DIR					DB_DIR "MAINRECIPE\\"						// メインレシピパス名
#define DB_MULTI_RECIPE_DIR					DB_DIR "MULTIRECIPE\\"						// マルチレシピパス名
#define DB_MEASUREMENT_PROGRAM_DIR			DB_DIR "MEASUREMENTPGM\\"					// メジャーメントPGMパス名
#define DB_STAGE_PROGRAM_DIR				DB_DIR "STAGEPGM\\"							// ステージPGMパス名
#define DB_DESKEW_IMG_DIR					DB_DIR "DESKEW_IMG\\"						// デスキューPGMパス名
#define DB_PATTERN_DIR						DB_DIR "PATTERN\\"							// パターンファイルパス名
#define DB_PATTERN_IMG_DIR					DB_DIR "PATTERN_IMG\\"						// パターンイメージパス名
#define DB_RECALIBRATION_PROGRAM_DIR		DB_DIR "RECALIBRATIONPGM\\"					// リキャリブレーションPGMパス名
#define DB_POINT_DESKEW_PROGRAM_DIR			DB_DIR "POINTDESKEWPGM\\"					// ポイントデスキューパス名
#define RESULT_DIR							"..\\RESULT\\"								// 測定結果保存パス名
/* modified 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
//#define MEAS_DAT_TEMP_COPY_PASS 			"c:\\NanoSpec\\DATA\\MeasuredDataTemp\\"	//測定ポイントデータの仮格納場所
/* modified 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ----------			  */
#define MEAS_DAT_TEMP_COPY_PASS 			DATA_DIR _T("MeasuredDataTemp\\")	//測定ポイントデータの仮格納場所
/* modified 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
#define DATA_DIR				 			"..\\DATA\\"
#define DATA_MEASUREMENTDATA_DIR			DATA_DIR "MEASUREMENTDATA\\"				// CCDスキャンデータ測定のログ出力（NanoSpec.iniファイルへ指定した場合のオプション機能）
#define MEASURED_DATA_TEMP_DIR	 			"MeasuredDataTemp\\"						//測定ポイントデータの仮格納場所
#define REF_DIR								"ref\\"										//リファレンスデータ格納先
// Matsuhisa 2009.05.30 ----->
#define PR_DIR								LOG_DIR "PR\\"
// Matsuhisa 2009.05.30 ----->

// pattern recognition images
#define PATTERN_IMG_TEMP					 DB_PATTERN_IMG_DIR "temp.img"
#define PATTERN_IMG_1						 DB_PATTERN_IMG_DIR "site_1.img"
#define PATTERN_IMG_2						 DB_PATTERN_IMG_DIR "site_2.img"

// ==========================================================================
// ファイル名等
#define NANOSPEC_INIFILENAME				"NanoSpec.ini"								// NanoSpec Init File Name
#define SYSTEMCOMMON_INIFILENAME			"SYSTEM\\SystemCommon.ini"
#define SAVE_DEFAULT_NAME					""											//測定データ保存名をコモンダイアログで入力する時のデフォルト保存名
#define CIE_EXT								".cie"
#define DAT_EXT								".dat"
#define TXT_EXT								".txt"
#define BMP_EXT								".bmp"
#define MAINRECIPE_EXT						".mai"
#define MULTIRECIPE_EXT						".mul"
#define MEASUREMENTPGM_EXT					".mea"
#define STAGEPGM_EXT						".sta"
#define POINTDESKEWPGM_EXT					".poi"
#define RECALIBPGM_EXT						".rec"
#define REFERENCE_EXT						".ref"
#define INF_EXT								".inf"
#define PATTERN_EXT							".sit"
#define PATTERN_IMAGE_EXT					".spn"

#define COMMON_DEFAULT_NAME					"UserCommonDef"					// UserSettingのCommonに関する設定ファイル名
#define SR_HEAD_DEFAULT_NAME				"UserSRDef"						// UserSettingのSRに関する設定ファイル名
#define SE_HEAD_DEFAULT_NAME				"UserSEDef"						// UserSettingのSEに関する設定ファイル名
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
#define COMPEASE_HEAD_DEFAULT_NAME			"UserEaseDef"					// UserSettingのCompleteEASEに関する設定ファイル名
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//#define IRSE_HEAD_DEFAULT_NAME			"UserIRSEDef"					// UserSettingのIRSEに関する設定ファイル名
#define MS_HEAD_DEFAULT_NAME				"UserMSDef"						// UserSettingのMSに関する設定ファイル名
// 2009.10.19 bagus MS 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
//#define RES_HEAD_DEFAULT_NAME				"User4PPDef"					// UserSettingの4PPに関する設定ファイル名
#define RES_HEAD_DEFAULT_NAME				"UserRsDef"						// UserSettingの4PPに関する設定ファイル名
// 2009.11.04 bagus RS 追加 --}--
// 2009.10.08 bagus CTA 変更 --{--
//#define CTA_HEAD_DEFAULT_NAME				"UserCTADef"					// UserSettingのContactAngleに関する設定ファイル名
#define CTA_HEAD_DEFAULT_NAME				"UserCaDef"						// UserSettingのContactAngleに関する設定ファイル名
// 2009.10.08 bagus CTA 変更 --}--
//2009.10.21 bagus Stress 追加修正 --{--
//#define STRESS_HEAD_DEFAULT_NAME			"UserSTRESSDef"					// UserSettingのStressに関する設定ファイル名
#define STRESS_HEAD_DEFAULT_NAME			"UserStressDef"					// UserSettingのStressに関する設定ファイル名
//2009.10.21 bagus Stress 追加修正 --}--
#define STAGE_MAP_FILE_NAME					"StageMap"						// ZAxisのOffset値のファイル名
#define STAGE_LEVEL_CALIBRATION_FILE_NAME	"StageLevelCalibration"			// ZAxisのOffsetを求めるSrMainRecipeの保存ファイル名
#define USER_ACCOUNT_DAT_FILE_NAME			"UserAccount"					// Account.datのファイル名
#define SAMPLE_DAT_FILE_NAME				"Sample"						// sample.datのファイル名

/* modified 2009.06.15 hmenjo タイトルウィンドウキャプション変更 ---------- { ---------- */
//#define NANOSPEC_TITLE_WINDOW_NAME			"NanoSpec"
/* modified 2009.06.15 hmenjo タイトルウィンドウキャプション変更 ---------- 			 */
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//#define NANOSPEC_TITLE_WINDOW_NAME			"NanoSpec Initializing"
#define NANOSPEC_TITLE_WINDOW_NAME			"Initializing"
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
/* modified 2009.06.15 hmenjo タイトルウィンドウキャプション変更 ---------- } ---------- */
#define NANOSPEC_TITLE_EXE_NAME				"NanoSpecTitle.exe"

/* modified 2014.11.22 hmenjo TohoRecall へ変更 ---------- { ---------- */
//#define NANOMAP_WINDOW_NAME					"Nanometrics - Nanospec/AFT"
/* modified 2014.11.22 hmenjo TohoRecall へ変更 ----------              */
#define NANOMAP_WINDOW_NAME					_T("NanoMap32")
#define NANOMAP_WINDOW_NAME_NEW				_T("TohoRecall")
/* modified 2014.11.22 hmenjo TohoRecall へ変更 ---------- } ---------- */
#define NANOMAP_EXE_NAME					"NanoMap32.exe"
/* added 2014.11.22 hmenjo TohoRecall へ変更 ---------- { ---------- */
#define	NANOMAP_EXE_NAME_NEW				_T("TohoRecall.exe")
/* added 2014.11.22 hmenjo TohoRecall へ変更 ---------- } ---------- */

#define ADAP_WINDOW_NAME					"ADAP - Main Screen"

#define DESKEW_MANUAL_SEQ_WINDOW_NAME		"DeskewManualSeqWnd"
#define SITEPT_MANUAL_SEQ_WINDOW_NAME		"SitePtManualSeqWnd"

// ==========================================================================
// Mil
// ==========================================================================
typedef WORD RESERVED;

// ==========================================================================
// NanoSpec.ini
// ==========================================================================

// --------------------------------------------------------------------------
// Hard Ware Simulation
#define INISECTION_SIMULATION				"SIMULATION"
#define INIKEY_HWS							"HWSimulation"

// --------------------------------------------------------------------------
// SrCcdData
#define INISECTION_CCDDATA					"CCDDATA"
#define INIKEY_SAVESCANDATALOG				"SaveScanDataLog"

// --------------------------------------------------------------------------
// SrHeadSerial
#define INISECTION_HEADCOM					"HEADCOM"
#define INIKEY_COMMNO						"CommNo"
#define INIKEY_BAUDRATE 					"BaudRate"
#define INIKEY_PARITY						"Parity"
#define INIKEY_BYTESIZE 					"ByteSize"
#define INIKEY_STOPBITS 					"StopBits"
#define INIKEY_CCDDATATRANSFERMODEBINARY	"CcdDataTransferModeBinary"

// --------------------------------------------------------------------------
// StageConfigSettingDlg
#define INISECTION_STAGESIZE				"STAGESIZE"				//HWS時のサブコンからのデータ読み込みの代わり
#define INIKEY_HWSTAGESIMULATIN				"HWStageSimulation"
#define INIKEY_HWS_STAGESIZEX				"StageSizeX"
#define INIKEY_HWS_STAGESIZEY				"StageSizeY"
#define INIKEY_HWS_STAGEEDGESIZEX			"EdgeSizeX"
#define INIKEY_HWS_STAGEEDGESIZEY			"EdgeSizeY"
#define INIKEY_HWS_STAGEORIGINOFFSETX		"OriginOffsetX"
#define INIKEY_HWS_STAGEORIGINOFFSETY		"OriginOffsetY"

// added hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- { ----------
// --------------------------------------------------------------------------
// EQ メンテナンス
#define	INISECTION_EQMAINTE					_T("EQ Maintenance")
#define INIKEY_EQM_CYCLESTOP				_T("CycleStop")
// added hmenjo 2009.05.12 異常時サイクルストップ許可フラグ追加 ---------- } ----------
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- { ----------
#define INIKEY_EQM_DOOROPEN					_T("DoorOpen")
// added hmenjo 2009.05.12 ドア開検出許可フラグ追加 ---------- } ----------
// added hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- { ----------
#define INIKEY_EQM_SPEEDDOWN				_T("SpeedDown")
// added hmenjo 2009.05.12 速度変更許可フラグ追加 ---------- } ----------
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- { ---------- */
#define INIKEY_EQM_EQPOWEROFF				_T("EQPowerOff")
/* added 2009.05.26 hmenjo 装置電源オフ出力の有効/無効スイッチ追加 ---------- } ---------- */
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- { ---------- */
#define INIKEY_EQM_DOOROPENPAUSE			_T("DoorOpenPause")
/* added 2009.06.02 hmenjo ドア開で測定ポーズ ---------- } ---------- */

// 2009.08.28 bagus stress --{--
#define	STRESS_REFERENCE_NAME				_T("%StressReference%")
// 2009.08.28 bagus stress --}--
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- { ---------- */
// --------------------------------------------------------------------------
// サブコン電源再投入
#define	INISECTION_SUBCONPWR				_T("SubConPwr")
#define	INIKEY_SBCPWR_OFFTIME				_T("OffTime")
#define	INIKEY_SBCPWR_WAITAFTERON			_T("WaitAfterOn")
/* added 2013.06.04 hmenjo SubCon PwrOFF ---------- } ---------- */

// 2013.11.08 Bagus Add (TohoSpec対応) -->
// --------------------------------------------------------------------------
// モデルタイプ
#define	INISECTION_MODELTYPE				_T("ModelType")
#define	INIKEY_MODELTYPE_TYPE				_T("TYPE")

// --------------------------------------------------------------------------
// ビデオ表示
#define	INISECTION_OVIDEO				_T("OVideo")
#define	INIKEY_OVIDEO_ENABLE				_T("Enable")
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- { ---------- */
#define	INIKEY_OVIDEO_FORMATSIZE			_T("VideoFormatSize")
/* added 2015.12.03 hmenjo カメラビデオフォーマット設定化 ---------- } ---------- */
// 2013.11.08 Bagus Add (TohoSpec対応) <--

/////////////////////////////////////////////////////////////////////////////
// CONSTANT VALUE                                                          //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
// 2013.11.08 Bagus Add (TohoSpec対応) -->
enum APP_NAME_TYPE
{
	APP_NAME_NANO = 0,
	APP_NAME_TOHO,
	APP_NAME_MAX,
};

const LPCSTR g_lpszAppPrefix4[] = {
	"Nano",
	"Toho",
};

enum MODEL_TYPE
{
	MODEL_M6500 = 6500,
	MODEL_T3100 = 3100,
};
// 2013.11.08 Bagus Add (TohoSpec対応) <--

///// 言語の設定 ////////////////////////////////////////////////////////////
enum LANGUAGE
{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_JAPANESE,
	LANGUAGE_MAX,
};

// ##########################################################################
// MEASUREMENT PROGRAM
// ##########################################################################

// ==========================================================================
// カラーシステムタイプ
// ==========================================================================
typedef enum _COLOR_SYSTEM_TYPE
{
	COLORSYS_NTSC = 0,
	COLORSYS_PAL_SECM,
	COLORSYS_EBU,
	COLORSYS_SMPTE,
	COLORSYS_SPERSISTENCE,
	COLORSYS_LPERSISTENCE,
	COLORSYS_DELL,
	COLORSYS_MAX,
} COLOR_SYSTEM_TYPE;

// ==========================================================================
// イルミナントテーブル
// ==========================================================================
const LPCSTR ILLUMINANT_TABLE_ITEM[] = {
	"Illuminant_A",
	"Illuminant_D65",
	"SubIlluminant_C",
	"SubIlluminant_D50",
	"SubIlluminant_D55",
};

typedef enum _ILLUMINANT_TABLE
{
	ILLUMINANT_ILLUMINANT_A = 0,
	ILLUMINANT_ILLUMINANT_D65,
	ILLUMINANT_SUBILLUMINANT_C,
	ILLUMINANT_SUBILLUMINANT_D50,
	ILLUMINANT_SUBILLUMINANT_D55,
	ILLUMINANT_MAX,
} ILLUMINANT_TABLE;

// ==========================================================================
// 光学フィルタータイプ
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _OPTICAL_FILTER
{
	OPT_FILTER_UNKNOWN =0,
	OPT_FILTER_OPEN,
	OPT_FILTER_DARK,
	OPT_FILTER_POS1,
	OPT_FILTER_POS2,
	OPT_FILTER_POS3,
	OPT_FILTER_MAX,
} OPTICAL_FILTER;

// ==========================================================================
// ヘッドタイプ
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _HEAD_TYPE
{
	HEAD_TYPE_SR = 0,
	HEAD_TYPE_SE,
	HEAD_TYPE_IRSE,
	HEAD_TYPE_4PP,
	HEAD_TYPE_CTA,
	HEAD_TYPE_STRESS,
// 2009.10.19 bagus MS 追加 --{--
	HEAD_TYPE_MS,
// 2009.10.19 bagus MS 追加 --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	HEAD_TYPE_COMPEASE,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	HEAD_TYPE_MAX,
} HEAD_TYPE;

// 2009.10.09 bagus StagePGM 共通化 --{--
// ==========================================================================
// ステージプログラムタイプ
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _STAGE_PGM_TYPE
{
	STAGE_PGM_TYPE_STANDARD = 99,
	STAGE_PGM_TYPE_STRESS = 5,
	STAGE_PGM_TYPE_SR_GANTRY,			// 使用禁止(画面遷移用)
	STAGE_PGM_TYPE_SR_DISTANCE,			// 使用禁止(画面遷移用)
	STAGE_PGM_TYPE_MAX,
} STAGE_PGM_TYPE;

typedef enum _SCAN_STAGE_PGM_TYPE_STANDARD
{
	SCAN_TYPE_STANDARD = 0,
	SCAN_TYPE_STANDARD_MAX,
} SCAN_STAGE_PGM_TYPE_STANDARD;

typedef enum _SCAN_STAGE_PGM_TYPE_STRESS
{
	SCAN_TYPE_STRESS = 0,
	SCAN_TYPE_STRESS_MAX,
} SCAN_STAGE_PGM_TYPE_STRESS;

typedef enum _SCAN_STAGE_PGM_TYPE_SR_GANTRY
{
	SCAN_TYPE_SR_GANTRY = 3,
	SCAN_TYPE_SR_GANTRY_MAX,
} SCAN_STAGE_PGM_TYPE_SR_GANTRY;

typedef enum _SCAN_STAGE_PGM_TYPE_SR_DISTANCE
{
	SCAN_TYPE_SR_DISTANCE = 4,
	SCAN_TYPE_SR_DISTANCE_MAX,
} SCAN_STAGE_PGM_TYPE_SR_DISTANCE;
// 2009.10.09 bagus StagePGM 共通化 --}--

// ==========================================================================
// SRの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_SR
{
	MEAS_PROG_TYPE_SR_THICKNESS = 0,
	MEAS_PROG_TYPE_SR_REFLECTANCE,
	MEAS_PROG_TYPE_SR_TRANSMITTANCE,
// 2009.09.03 K.Matsuo -->
	MEAS_PROG_TYPE_SR_TRANSMITTANCE_G,				// 透過率、ステージがガントリータイプ
// 2009.09.03 K.Matsuo <--
// 2009.10.09 bagus ２点間 --{--
	MEAS_PROG_TYPE_SR_DISTANCE,
// 2009.10.09 bagus ２点間 --}--
	MEAS_PROG_TYPE_SR_REFLECTANCE_CIE,
	MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE,
	MEAS_PROG_TYPE_SR_OPTICAL_DENSITY,
	MEAS_PROG_TYPE_SR_MAX,
} MEAS_PROG_TYPE_SR;

// ==========================================================================
// SEの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_SE
{
	MEAS_PROG_TYPE_SE_THICKNESS = 0,
// 2009.09.25 K.Matsuo delete -->
//	MEAS_PROG_TYPE_SE_OPTICAL_CONSTANT,
//	MEAS_PROG_TYPE_SE_CALC_REFLECTANCE_TRANSMITTANCE,
//	MEAS_PROG_TYPE_SE_OPTICAL_COARSENESS,
//	MEAS_PROG_TYPE_SE_MIXTURE_MATERIAL,
//	MEAS_PROG_TYPE_SE_OPTICAL_BANDGAP,
//	MEAS_PROG_TYPE_SE_LTPS_PARAM,
// 2009.09.25 K.Matsuo delete <--
	MEAS_PROG_TYPE_SE_MAX,
} MEAS_PROG_TYPE_SE;

// 2009.10.19 bagus MS 追加 --{--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// ==========================================================================
// CompleteEASEの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_COMPEASE
{
	MEAS_PROG_TYPE_COMPEASE_THICKNESS = 0,
	MEAS_PROG_TYPE_COMPEASE_MAX,
} MEAS_PROG_TYPE_COMPEASE;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

#if 0
// ==========================================================================
// IRSEの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_IRSE
{
	MEAS_PROG_TYPE_IRSE_THICKNESS = 0,
	MEAS_PROG_TYPE_IRSE_OPTICAL_CONSTANT,
	MEAS_PROG_TYPE_IRSE_CALC_REFLECTANCE_TRANSMITTANCE,
	MEAS_PROG_TYPE_IRSE_OPTICAL_COARSENESS,
	MEAS_PROG_TYPE_IRSE_MIXTURE_MATERIAL,
	MEAS_PROG_TYPE_IRSE_OPTICAL_BANDGAP,
	MEAS_PROG_TYPE_IRSE_LTPS_PARAM,
	MEAS_PROG_TYPE_IRSE_MAX,
} MEAS_PROG_TYPE_IRSE;
#else
typedef enum _MEAS_PROG_TYPE_MS
{
	MEAS_PROG_TYPE_MS_MEAS = 0,
	MEAS_PROG_TYPE_MS_MAX,
} MEAS_PROG_TYPE_MS;
#endif
// 2009.10.19 bagus MS 追加 --}--

// ==========================================================================
// 4PPの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_4PP
{
	MEAS_PROG_TYPE_4PP_MEAS = 0,
	MEAS_PROG_TYPE_4PP_MAX,
} MEAS_PROG_TYPE_4PP;

// ==========================================================================
// Contact Angleの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_CTA
{
	MEAS_PROG_TYPE_CTA_MEAS = 0,
	MEAS_PROG_TYPE_CTA_MAX,
} MEAS_PROG_TYPE_CTA;

// ==========================================================================
// Stressの測定項目
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_PROG_TYPE_STRESS
{
	MEAS_PROG_TYPE_STRESS_MEAS = 0,
	MEAS_PROG_TYPE_STRESS_MAX,
} MEAS_PROG_TYPE_STRESS;

// ==========================================================================
// MeasurementListでのThickness測定の項目名
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_LIST_THICK
{
	MEAS_LIST_THICK_THICKNESS = 0,
	MEAS_LIST_THICK_FIT,
	MEAS_LIST_THICK_MAX,
} MEAS_LIST_THICK;

// ==========================================================================
// MeasurementListでのChromaticity測定の項目名
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_LIST_CIE
{
	MEAS_LIST_CIE_LARGE_X = 0,
	MEAS_LIST_CIE_LARGE_Y,
	MEAS_LIST_CIE_LARGE_Z,
	MEAS_LIST_CIE_SMALL_X,
	MEAS_LIST_CIE_SMALL_Y,
	MEAS_LIST_CIE_LARGE_L,
	MEAS_LIST_CIE_SMALL_A,
	MEAS_LIST_CIE_SMALL_B,
	MEAS_LIST_CIE_POINT_L,
	MEAS_LIST_CIE_POINT_A,
	MEAS_LIST_CIE_POINT_B,
	MEAS_LIST_CIE_APOST_U,
	MEAS_LIST_CIE_APOST_V,
	MEAS_LIST_CIE_POINT_U,
	MEAS_LIST_CIE_POINT_V,
	MEAS_LIST_CIE_MAX,
} MEAS_LIST_CIE;

// ==========================================================================
// MeasurementListでのPosition測定の項目名
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_LIST_POSITION
{
	MEAS_LIST_POSITION_NUM = 0,
	MEAS_LIST_POSITION_X,
	MEAS_LIST_POSITION_Y,
	MEAS_LIST_POSITION_Z,
	MEAS_LIST_POSITION_MAX,
} MEAS_LIST_POSITION;

// ==========================================================================
// MeasurementListでのLine/Section測定の項目名
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_LIST_STRESS_POSITION
{
	MEAS_LIST_STRESS_POSITION_NUM = 0,
//	MEAS_LIST_STRESS_POSITION_LINE,
//	MEAS_LIST_STRESS_POSITION_SECTION,
	MEAS_LIST_STRESS_POSITION_X_START,
	MEAS_LIST_STRESS_POSITION_X_END,
	MEAS_LIST_STRESS_POSITION_Y_POS,
	MEAS_LIST_STRESS_POSITION_STRESS,
	MEAS_LIST_STRESS_POSITION_X,
	MEAS_LIST_STRESS_POSITION_Y,
	MEAS_LIST_STRESS_POSITION_Z,
	MEAS_LIST_STRESS_POSITION_MAX,
} MEAS_LIST_STRESS_POSITION;

// ==========================================================================
// MeasurementListでのParameter測定の項目名
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _MEAS_LIST_PARAM
{
	MEAS_LIST_PARAM_MAXIM = 0,
	MEAS_LIST_PARAM_MINIM,
	MEAS_LIST_PARAM_RANGE,
	MEAS_LIST_PARAM_MEAS,
	MEAS_LIST_PARAM_SD,
	MEAS_LIST_PARAM_MAX,
} MEAS_LIST_PARAM;

// ##########################################################################
// Deskew Mode (文字列がNanoSpec\System.h内で定義してある)
// ##########################################################################
typedef enum _DESKEW_MODE
{
	DESKEW_MODE_NO_DESKEW = 0,												// correct with no deskew
	DESKEW_MODE_PATTERN_DESKEW,												// correct with pattern rec deskew
	DESKEW_MODE_MANUAL_DESKEW,												// correct with manual deskew
	DESKEW_MODE_MAX,
} DESKEW_MODE;

// ##########################################################################
// Deskew Fail Option (文字列がNanoSpec\System.h内で定義してある)
// ##########################################################################
typedef enum _DESKEW_FAIL_OPTION
{
	DESKEW_FAIL_OPTION_PROCEED_MEAS = 0,									// 強制測定（そのまま測定開始）
	DESKEW_FAIL_OPTION_CANCEL_RECIPE,										// 測定は行わず，レシピ終了
	DESKEW_FAIL_OPTION_MANUAL_DESKEW,										// Manual Deskewに移行する
// 2009.04.10 K.Matsuo Add -->
	DESKEW_FAIL_OPTION_PROCEED_MEAS_PMA,									// 強制測定（そのまま位置補正付き測定（PMA使用））
// 2009.04.10 K.Matsuo Add <--
	DESKEW_FAIL_OPTION_MAX,
} DESKEW_FAIL_OPTION;

// ###########################################################################
// MAIN RECPIE
// ###########################################################################
enum RECALIBRATION
{
	RECALIB_1 = 0,
	RECALIB_2,
	RECALIB_3,
	RECALIB_MAX,
};

// ##########################################################################
// Recipe，Program，UserAccount，Sampleの読み込みと保存の変数
// ##########################################################################
enum RECIPE_FILE_ITEM
{
	// Recipe Setting関連
	RECIPE_FILE_MAIN_RECIPE = 0,
	RECIPE_FILE_SR_MAIN_RECIPE,
	RECIPE_FILE_SE_MAIN_RECIPE,
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	RECIPE_FILE_COMPEASE_MAIN_RECIPE,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	RECIPE_FILE_IRSE_MAIN_RECIPE,
	RECIPE_FILE_MS_MAIN_RECIPE,
// 2009.10.19 bagus MS 追加 --}--
	RECIPE_FILE_4PP_MAIN_RECIPE,
	RECIPE_FILE_CTA_MAIN_RECIPE,
	RECIPE_FILE_STRESS_MAIN_RECIPE,
	RECIPE_FILE_MULTI_RECIPE,
	RECIPE_FILE_STAGE_PROGRAM,
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
	RECIPE_FILE_STAGE_PROGRAM_STRESS,
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */
	RECIPE_FILE_RECALIBRATION_PROGRAM,
	RECIPE_FILE_RECALIBRATION_PROGRAM_TWO,
	RECIPE_FILE_RECALIBRATION_PROGRAM_THREE,
	RECIPE_FILE_POINT_DESKEW_PROGRAM,
	RECIPE_FILE_MEASUREMENT_PROGRAM,
	RECIPE_FILE_SR_THICKNESS,
	RECIPE_FILE_SR_REFLECTANCE,
	RECIPE_FILE_SR_TRANSMITTANCE,
	RECIPE_FILE_SR_REFLECTANCE_CIE,
	RECIPE_FILE_SR_TRANSMITTANCE_CIE,
	RECIPE_FILE_SR_OPTICAL_DENSITY,
// 2009.09.04 K.Matsuo -->
	RECIPE_FILE_SE_THICKNESS,
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	RECIPE_FILE_COMPEASE_THICKNESS,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	RECIPE_FILE_4PP,
	// 2009.08.25 bagus stress --{--
	RECIPE_FILE_STRESS,
	// 2009.08.25 bagus stress --}--
	// 2009.10.07 bagus CTA 追加 --{--
	RECIPE_FILE_CTA,
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	RECIPE_FILE_SR_DISTANCE,
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	RECIPE_FILE_MS,
	//2009.10.20 bagus MS 追加 --}--
	// User Setting関連(UserAccountの設定)
	RECIPE_FILE_USER_ACCOUNT,
	// User Setting関連(デフォルト値の設定)
	RECIPE_FILE_DEF_SR_MAIN_RECIPE,
	RECIPE_FILE_DEF_SR_THICKNESS,
	RECIPE_FILE_DEF_SR_REFLECTANCE,
	RECIPE_FILE_DEF_SR_TRANSMITTANCE,
	RECIPE_FILE_DEF_SR_REFLECTANCE_CIE,
	RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE,
	RECIPE_FILE_DEF_SR_OPTICAL_DENSITY,
	RECIPE_FILE_DEF_4PP,
	// 2009.08.25 bagus stress --{--
	RECIPE_FILE_DEF_STRESS,
	RECIPE_FILE_DEF_STRESS_MAIN_RECIPE,
	// 2009.08.25 bagus stress --}--
	// 2009.09.07 bagus se --{--
	RECIPE_FILE_DEF_SE_THICKNESS,
	RECIPE_FILE_DEF_SE_MAIN_RECIPE,
	// 2009.09.07 bagus se --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	RECIPE_FILE_DEF_COMPEASE_THICKNESS,
	RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE,
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	RECIPE_FILE_DEF_CTA,
	RECIPE_FILE_DEF_CTA_MAIN_RECIPE,
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	RECIPE_FILE_DEF_SR_DISTANCE,
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	RECIPE_FILE_DEF_MS,
	RECIPE_FILE_DEF_MS_MAIN_RECIPE,
	//2009.10.20 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	RECIPE_FILE_DEF_4PP_MAIN_RECIPE,
	// 2009.11.04 bagus RS 追加 --}--
	// System関連(Sampleの設定)
	RECIPE_FILE_SAMPLE,
	// System関連(ZAxisOffsetの設定)
	RECIPE_FILE_Z_AXIS_OFFSET,
	// MeasurementData
	RECIPE_FILE_MEASUREMENT_DATA,
	RECIPE_FILE_STATISTICS_DATA,
	RECIPE_FILE_MEAS_DATA_SR_THICKNESS,
	RECIPE_FILE_MEAS_DATA_SR_REFLECTANCE,
	RECIPE_FILE_MEAS_DATA_SR_TRANSMITTANCE,
	RECIPE_FILE_MEAS_DATA_SR_REFLECTANCE_CIE,
	RECIPE_FILE_MEAS_DATA_SR_TRANSMITTANCE_CIE,
	RECIPE_FILE_MEAS_DATA_SR_OPTICAL_DENSITY,
	RECIPE_FILE_MEAS_DATA_4PP,
	RECIPE_FILE_STAT_DATA_SR_THICKNESS,
	RECIPE_FILE_STAT_DATA_SR_REFLECTANCE,
	RECIPE_FILE_STAT_DATA_SR_TRANSMITTANCE,
	RECIPE_FILE_STAT_DATA_SR_REFLECTANCE_CIE,
	RECIPE_FILE_STAT_DATA_SR_TRANSMITTANCE_CIE,
	RECIPE_FILE_STAT_DATA_SR_OPTICAL_DENSITY,
	RECIPE_FILE_STAT_DATA_4PP,
	RECIPE_FILE_MAX,
};

// ##########################################################################
// Iniファイルの設定
// ##########################################################################
enum NANOSPEC_INI_FILE
{
	SYSTEM_COMMON_INI_FILE = 0,
	SYSTEM_SR_INI_FILE,
	USER_SETTING_COMMON_INI_FILE,
	USER_SETTING_SR_INI_FILE,
	SYSTEM_STRESS_INI_FILE,			/* added 2009.07.22 hmenjo ストレス ライン数 定義  */
	USER_STRESS_MODULUS_INI_FILE,	/* 2009.11.09 bagus Stress 追加変更 */
	SYSTEM_SE_INI_FILE,				/* added 2009.08.25 hmenjo SE 構造体追加(１) */
	USER_SETTING_SE_INI_FILE,		/* added 2009.09.01 hmenjo SE 構造体追加(12) */
	SYSTEM_MS_INI_FILE,				/* 2009.10.19 bagus MS 追加 */
	SYSTEM_CA_INI_FILE,				/* 2009.10.30 bagus CA 追加 */
	SYSTEM_RS_INI_FILE,				/* 2009.11.04 bagus RS 追加 */
	SYSTEM_COMPEASE_INI_FILE,			/* 2013.02.04 bagus CompleteEASEヘッド追加 */
	USER_SETTING_COMPEASE_INI_FILE,			/* 2013.02.04 bagus CompleteEASEヘッド追加 */
	SUBSTRATE_THICKNESS_INI_FILE,			/* 2013.02.22 bagus Substrate thickness setting 追加 */
	NANOSPEC_INI_FILE_MAX,
};

const LPCSTR g_lpszNanoSpecIniFileName[] = {
	"SystemCommon.ini",
	"SystemSR.ini",
	"UserSettingCommon.ini",
	"UserSettingSr.ini",
	"SystemStress.ini",				/* added 2009.07.22 hmenjo ストレス ライン数 定義  */
	"UserStressElasticTable.ini",	/* 2009.11.09 bagus Stress 追加変更 */
	"SystemSE.ini",					/* added 2009.08.25 hmenjo SE 構造体追加(１) */
	"UserSettingSe.ini",			/* added 2009.09.01 hmenjo SE 構造体追加(12) */
	"SystemMS.ini",					/* 2009.10.19 bagus MS 追加 */
	"SystemCa.ini",					/* 2009.10.30 bagus CA 追加 */
	"SystemRS.ini",					/* 2009.11.04 bagus RS 追加 */
	"SystemCompEase.ini",			/* 2013.02.01 bagus CompleteEASE 追加 */
	"UserSettingEase.ini",			/* 2013.02.01 bagus CompleteEASE 追加 */
	"SubstrateThickness.ini"	/* 2013.02.22 bagus Substrate thickness setting 追加 */
};

// ##########################################################################
// Correct Angle
// (文字列がNanoSpec\System.h内で定義してある
// ##########################################################################
enum CORRECT_ANGLE
{
	CORRECT_ANGLE_0 = 0,
	CORRECT_ANGLE_90,
	CORRECT_ANGLE_MINUS_90,
	CORRECT_ANGLE_MAX
};

// ##########################################################################
// Speed Setup
// (文字列がNanoSpec\System.h内で定義してある)
// ##########################################################################
enum STAGE_MOTION_MODE
{
	STAGE_MOTION_LOW = 0,
	STAGE_MOTION_MID,
	STAGE_MOTION_HIGH,
	STAGE_MOTION_JOG_LOW,
	STAGE_MOTION_JOG_MID,
	STAGE_MOTION_JOG_HIGH,
	STAGE_MOTION_MAX,
};

// ##########################################################################
// SR CCD Array Setting
// ##########################################################################
enum SR_CCD_ARRAY_ITEM
{
	SR_CCD_ARRAY_ITEM1 = 0,
	SR_CCD_ARRAY_ITEM2,
	SR_CCD_ARRAY_ITEM3,
	SR_CCD_ARRAY_ITEM4,
	SR_CCD_ARRAY_ITEM5,
	SR_CCD_ARRAY_ITEM6,
	SR_CCD_ARRAY_ITEM7,
	SR_CCD_ARRAY_ITEM_MAX,
};

const LPCSTR g_lpszSrCcdArrayItem[] = {
	"253.652nm-Hg",
	"312.566nm-Hg",
	"365.015nm-Hg",
	"546.074nm-Hg",
	"587.092nm-Kr",
	"760.154nm-Kr",
	"785.482nm-Kr",
};
#define CCD_ARRAYT_STR_INIT { "253.652nm-Hg", "312.566nm-Hg", "365.015nm-Hg", "546.074nm-Hg", "587.092nm-Kr", "760.154nm-Kr", "785.482nm-Kr" }

// ##########################################################################
// SR Head カラーフィルタ(CCD シャッタ)の設定
// ##########################################################################
enum FILTER{			//FILTER_Status
	FILTER_OPEN = 1,	//default state
	FILTER_DARK,
	FILTER_POS1,		//Yellow
	FILTER_POS2,		//Orange
	FILTER_POS3,
};

// ###########################################################################
// どのデータを読み込みもしくは保存するかを決める変数
// ###########################################################################
enum CONFIG_FILE
{
	// User Setting - All
	CONFIG_FILE_USER_SETTING = 0,
	// User Setting - Common
	CONFIG_FILE_MANUAL_LOAD_POSITION,
	CONFIG_FILE_AUTO_LOGON_NAME,
	CONFIG_FILE_ACCESS_LEVEL,
	CONFIG_FILE_DELETE_DATA_PERIOD,
	CONFIG_FILE_DEFAULT_HEAD_TYPE,
	CONFIG_FILE_STAGE_MOTION_JOG,
	CONFIG_FILE_STAGE_MOTION_SPEED,
	CONFIG_FILE_MAINTENANCE_POSITION,
	CONFIG_FILE_MAINTENANCE_DATE,
	CONFIG_FILE_MAINTENANCE_LAMP,
	// User Setting - SR
	CONFIG_FILE_SR_PATTERN_REC,
	CONFIG_FILE_SR_REFERENCE_POSITION,
	CONFIG_FILE_SR_FIGURE_FORMAT,
	CONFIG_FILE_SR_AUTOFOCUS_RETRY,
/* added 2009.09.07 hmenjo 透過率 構造体追加(２) ---------- { ---------- */
	CONFIG_FILE_SR_TRANSMIT,
/* added 2009.09.07 hmenjo 透過率 構造体追加(２) ---------- } ---------- */
/* added 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
	/* User Setting - SE	*/
	CONFIG_FILE_SE_SETTING,
/* added 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */
//2009.09.07 bagus se --{--
	CONFIG_FILE_SE_FIGURE_FORMAT,
//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	CONFIG_FILE_COMPEASE_SETTING,
	CONFIG_FILE_COMPEASE_FIGURE_FORMAT,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.07 bagus CTA 追加 --{--
	CONFIG_FILE_CTA_SETTING,
// 2009.10.07 bagus CTA 追加 --}--
	// System - All
	CONFIG_FILE_SYSTEM,
	// System - Common
	CONFIG_FILE_SYSTEM_CONFIG,
	CONFIG_FILE_ORIGINAL_POSITION,
	CONFIG_FILE_STAGE_CONFIG,
	CONFIG_FILE_FOCUS_CONFIG,
	CONFIG_FILE_EDGE_ALIGNER,
	CONFIG_FILE_HANDLER_LOAD_POSITION,
	CONFIG_FILE_GENERAL_POSITION,
	CONFIG_FILE_SIGNAL_TOWER,
	CONFIG_FILE_HOST,
	CONFIG_FILE_COM,
	CONFIG_FILE_MULTI_RECIPE,
// 2013.01.11 bagus jog direction -->
	CONFIG_FILE_JOG_DIR,
// 2013.01.11 bagus jog direction <--
	// System - SR
	CONFIG_FILE_SR_CONFIG,
	CONFIG_FILE_SR_FILTER,
	CONFIG_FILE_SR_TURRET,
	CONFIG_FILE_SR_LENS_CENTER_OFFSET,
	CONFIG_FILE_SR_XMP,
	CONFIG_FILE_SR_VIEW_RANGE,
	CONFIG_FILE_SR_VIDEO_CALIBRATION,
	CONFIG_FILE_SR_CCD_ARRAY,
/* added 2009.07.22 hmenjo ストレス ライン数 定義 ---------- { ---------- */
	/* System - Stress	*/
	CONFIG_FILE_STRESS_CONFIG,
/* added 2009.07.22 hmenjo ストレス ライン数 定義 ---------- } ---------- */
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
	CONFIG_FILE_STRESS_MODULUS_CONFIG,
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
///* added 2009.08.25 hmenjo SE 構造体追加(１) ---------- { ---------- */
	/* System - SE	*/
//	CONFIG_FILE_SE_CONFIG,
///* added 2009.08.25 hmenjo SE 構造体追加(１) ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */
// 2009.10.07 bagus CTA 追加 --{--
//	CONFIG_FILE_CTA_CONFIG,
// 2009.10.07 bagus CTA 追加 --}--
	//2009.09.07 bagus se --{--
	// SE用XMP
	CONFIG_FILE_SE_XMP,
	//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	CONFIG_FILE_COMPEASE_CONFIG,
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
	CONFIG_FILE_MS_CONFIG,
// 2009.10.19 bagus MS 追加 --}--
// 2009.10.30 bagus CA 追加 --{--
	CONFIG_FILE_CA_CONFIG,
// 2009.10.30 bagus CA 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	CONFIG_FILE_RS_CONFIG,
// 2009.11.04 bagus RS 追加 --}--
// 2013.02.22 bagus Substrate thickness setting -->
	CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG,
// 2013.02.22 bagus Substrate thickness setting <--

	// Head Config
	CONFIG_FILE_HEAD_TYPE_CONFIG,
	//
	CONFIG_FILE_MAX,
};

// ##########################################################################
// Stage Original Position
// ##########################################################################
enum STAGE_ORIGIN_POSITION
{
	STAGE_ORIGIN_CENTER = 0,
	STAGE_ORIGIN_TOPLEFT,
	STAGE_ORIGIN_BOTTOMLEFT,
	STAGE_ORIGIN_BOTTOMRIGHT,
	STAGE_ORIGIN_TOPRIGHT,
};

// 2009.10.26 K.Matsuo GTr -->
// ##########################################################################
// SR MEASYS Status type
// ##########################################################################
enum MEASYS_STAT_TYPE
{
	MEASYS_DARK_T1 = 0, 				// ダーク		  (T1)(標準､CIE､OD共)
	MEASYS_STDR_T1	= 1,				// 1stリファレンス(T1)(標準､CIE､OD共)
	MEASYS_EXTR_T1	= 2,				// 2ndリファレンス(T1)(OD)
	MEASYS_DARK_T2	= 3,				// ダーク		  (T2)(OD)
	MEASYS_EXTR_T2	= 4,				// 2ndリファレンス(T2)(OD)
	MEASYS_SAMP		= 5,				// サンプル 	  (T1)(標準､CIE､OD2ndﾅｼ) or サンプル(T2)(OD2ndｱﾘ)
	MEASYS_ANALYSIS	= 6,				// 測定終了後の計算

	MEASYS_GTR_1	= 1,				// ガントリー透過率リファレンス（最大20点）// 0（Dark分）空けておく
	MEASYS_GTR_LAST	= MAX_GANTRY_MEAS_POINT_NUM,
	MEASYS_STAT_TYPE_MAX_NUM = MAX_GANTRY_MEAS_POINT_NUM + 1,
};
// 2009.10.26 K.Matsuo GTr <--

// ##########################################################################
// User Accountのアカウントレベルの設定
// (文字列がSystem.h内に定義してある)
// ##########################################################################

enum ACCOUNT_LEVEL
{
	ACCOUNT_LEVEL_ADMINISTRATOR = 0,
	ACCOUNT_LEVEL_ENGINEER,
	ACCOUNT_LEVEL_OPERATOR,
	ACCOUNT_LEVEL_MAX,
	ACCOUNT_LEVEL_LOGOFF, //ログオフ状態(ソフト使用者側から見えないアカウント種のため『ACCOUNT_LEVEL_MAX』外に定義)
};

// ##########################################################################
// User Access Item
// ##########################################################################
enum ACCESS_ITEM
{
	ACCESS_MEASUREMENT = 0,
	ACCESS_RECIPE,
	ACCESS_PROGRAM,
	ACCESS_DATA,
	ACCESS_USERSETTING,
	ACCESS_MAINTENANCE,
	ACCESS_SYSTEMCONFIG,
};

// 2009.11.04 bagus RS 追加 --{--
// ==========================================================================
// ヘッドタイプ
// (文字列がNanoSpec\System.h内で定義してある)
// ==========================================================================
typedef enum _PROBE_TYPE
{
	PROBE_TYPE_ASP = 0,
	PROBE_TYPE_ESP,
	PROBE_TYPE_PSP,
	PROBE_TYPE_QPP,
	PROBE_TYPE_NSCP,
	PROBE_TYPE_TFP,
	PROBE_TYPE_DMY1,
	PROBE_TYPE_DMY2,
	PROBE_TYPE_DMY3,
	PROBE_TYPE_EXT,
	PROBE_TYPE_MAX,
} PROBE_TYPE;
// 2009.11.04 bagus RS 追加 --}--


/////////////////////////////////////////////////////////////////////////////
// STRUCTURE                                                               //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

// ##########################################################################
// REFERENCE
// ##########################################################################
typedef struct _REF_OPT_DESC
{
	TCHAR	szRefMaterial[MAX_PATH + 1];
} REF_OPT_DESC;

typedef struct _REF_INFO_HDR
{
	BOOL		bWarnFlag;													// 警告フラグ
	double	dLifeTime;														// 寿命
} REF_INFO_HDR;

typedef struct _REF_INFO_STRUCT
{
	REF_INFO_HDR hdr;
	REF_OPT_DESC OptDesc;
} REF_INFO_STRUCT;

// ##########################################################################
// 2nd REFERENCE
// ##########################################################################
typedef struct _REF_2ND_INFO_STRUCT
{
	BOOL	bMeasure;
	double dIntegTime;
	TCHAR	szRefMaterial[MAX_PATH + 1];
} REF_2ND_INFO_STRUCT;

//
typedef struct _WAVELEN_RANGE
{
	WORD wStart;
	WORD wEnd;
} WAVELEN_RANGE;

// ##########################################################################
// CHROMATICITY
// ##########################################################################
typedef struct _CIE_OPT_DESC
{
//	WORD				wScanType;											// スキャンタイプ
	WORD				wColorSystem;										// カラーシステム
	WORD				wIlluminant;										// イルミナントテーブル
	double				dGammaCorrectFactor;								// ガンマ補正値
} CIE_OPT_DESC;

// ##########################################################################
// THICKNESS
// ##########################################################################
typedef struct _XMP_DESC
{
	TCHAR	szStrategyEntry[STRATEGY_NAME_LEN + 1];
	BOOL	bAddReflectanceMeasureFlag;
} XMP_DESC;

// ##########################################################################
// SR
// ##########################################################################
typedef struct _SR_SCAN_INFO
{
	int				iLens;													// 測定レンズ
	WORD			wOpticsFilterType;										// 光学フィルタ
	WAVELEN_RANGE	WavelenRange;											// 波長範囲
	double			dIntegTime;												// 測定時間
	int				iSpecificWavelen[SPECIFIED_WAVELENGTH_MAX];				// 指定波長
//	double			dNoiseFilter;											// ノイズフィルタ
	BOOL			bEnhancedUv;											// UVゲインの使用
	CIE_OPT_DESC	CIEOptDesc;												// 色度オプション
	XMP_DESC		XMPDesc;												// 膜厚測定
/* added 2009.07.23 hmenjo ストレス ライン数 定義２ ---------- { ---------- */
// 2009.10.22 bagus Stress 修正 --{--
//	long			dwCalcDataInterval;			/* 計算用データ間隔[mm]	CalculationDataInterval	*/
// 2009.10.22 bagus Stress 修正 --}--
/* added 2009.07.23 hmenjo ストレス ライン数 定義２ ---------- } ---------- */
} SR_SCAN_INFO;

// ##########################################################################
// RESISTANCE
// ##########################################################################
//typedef BOOL RS_ITEM;
typedef struct _RS_SCAN_INFO
{
// 2009.09.04 K.Matsuo -->
	int iLens;																// 測定レンズ
	WORD wOpticsFilterType;													// 光学フィルタ
// 2009.09.04 K.Matsuo <--
	long	lStartIdx;														// 開始指数
	WORD	wVoltageLimit;													// 最大電圧
	double dRsCorrectFactor;												// 抵抗率補正係数
	double dThickness;														// 膜厚
	WORD	wUnits;															// 膜厚単位
	double dScanTime;														// 測定時間
} RS_SCAN_INFO;

typedef struct _CA_SCAN_INFO
{
// 2009.09.04 K.Matsuo -->
	int iLens;																// 測定レンズ
	WORD wOpticsFilterType;													// 光学フィルタ
// 2009.09.04 K.Matsuo <--
// 2009.10.08 bagus CTA 追加 --{--
	BOOL	bContactAngle;
	BOOL	bRadius;
	BOOL	bLiquidVolume;
// 2009.10.08 bagus CTA 追加 --}--
} CA_SCAN_INFO;

// ##########################################################################
// SE
// ##########################################################################
typedef struct _SE_SCAN_INFO
{
// 2009.09.04 K.Matsuo -->
	int iLens;																// 測定レンズ
	WORD wOpticsFilterType;													// 光学フィルタ
// 2009.09.19 bagus SE --{--
	WAVELEN_RANGE	WavelenRange;											// 波長範囲
// 2009.09.19 bagus SE --}--
	TCHAR	szStrategyEntry[STRATEGY_NAME_LEN + 1];
// 2009.09.04 K.Matsuo <--
} SE_SCAN_INFO;

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// ##########################################################################
// CompleteEASE
// ##########################################################################
typedef struct _COMPEASE_SCAN_INFO
{
	int iLens;																// 測定レンズ
	WORD wOpticsFilterType;													// 光学フィルタ
	TCHAR	szCompEASERecipe[COMPEASE_RECIPE_NAME_LEN + 1];
	BOOL	bAutoSaveResultInCompEASE;
} COMPEASE_SCAN_INFO;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// 2009.10.19 bagus MS 追加 --{--
#if 0
typedef struct _IR_SCAN_INFO
{
// 2009.09.04 K.Matsuo -->
	int iLens;																// 測定レンズ
	WORD wOpticsFilterType;													// 光学フィルタ
// 2009.09.04 K.Matsuo <--
} IR_SCAN_INFO;
#else
typedef struct _MS_SCAN_INFO
{
	int iLens;																// 測定レンズ
	WORD wOpticsFilterType;													// 光学フィルタ
//2009.10.20 bagus MS 追加 --{--
	int iMagLensIndex;
//2009.10.20 bagus MS 追加 --}--
} MS_SCAN_INFO;
#endif
// 2009.10.19 bagus MS 追加 --}--

typedef struct _SCAN_INFO_HDR
{
	WORD wHeadType;															// ヘッドタイプ
	WORD wScanType;
} SCAN_INFO_HDR, *LPSCAN_INFO_HDR;

typedef struct _SCAN_PARAMS
{
	SCAN_INFO_HDR	hdr;
	union
	{
		SR_SCAN_INFO _SR;													// SR測定
		SE_SCAN_INFO _SE;													// SE測定
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		COMPEASE_SCAN_INFO _COMPEASE;										// CompEASE測定
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//		IR_SCAN_INFO _IR;													// IR測定
// 2009.10.19 bagus MS 追加 --}--
		RS_SCAN_INFO _RS;													// シート抵抗
		CA_SCAN_INFO _CA;													// コンタクトアングル
// 2009.10.19 bagus MS 追加 --{--
		MS_SCAN_INFO _MScope;												// MS測定
// 2009.10.19 bagus MS 追加 --}--
	};
} SCAN_PARAMS;

typedef struct _PROG_INFO_HDR
{
	TCHAR szName[RECIPE_NAME_LEN + 1];
	TCHAR szComment[RECIPE_COMMENT_LEN + 1];
} PROG_INFO_HDR;
typedef PROG_INFO_HDR ADMIN_INFO_HDR;

typedef struct _MEAS_PROG_INFO
{
	ADMIN_INFO_HDR		hdr;												// ヘッダ
	SCAN_PARAMS			ScanParams;
	REF_INFO_STRUCT		Ref;												// リファレンス
	REF_2ND_INFO_STRUCT Ref2nd;												// 2nd リファレンス
	_MEAS_PROG_INFO::_MEAS_PROG_INFO()
	{::ZeroMemory(this, sizeof(_MEAS_PROG_INFO));}
} MEAS_PROG_INFO, *LPMEAS_PROG_INFO;

// ==========================================================================
// Stage Configuration
// ==========================================================================
typedef struct _STAGE_CONFIG{
	int		MachineOrgSensorLoc;	// 1:Center, 2:Top Left, 3:Botton Left, 4:Botton Right, 5:Top Right
	int		OrgPos;		// 1:Center, 2:Top Left, 3:Botton Left, 4:Botton Right, 5:Top Right

	struct{
		int X;			// 1:Right Hand, 2:Upper, 3:Left Hand, 4:Lower
		int Y;			// 1:Right Hand, 2:Upper, 3:Left Hand, 4:Lower
	} Dir;

	struct{
		double dX;
		double dY;
	} Size;

	struct{
		double dX;
		double dY;
	} Edge;

	struct{
		long lX;
		long lY;
	} Origin;

	struct{
		double dLeftX;
		double dRightX;
		double dUpY;
		double dDownY;
	} StageMaxCoord;

// 2013.01.17 bagus rotate xy view -->
	int		RotateXyView;
// 2013.01.17 bagus rotate xy view <--

	_STAGE_CONFIG::_STAGE_CONFIG()
	{::ZeroMemory(this, sizeof(_STAGE_CONFIG));}
} STAGE_CONFIG;
// ##########################################################################
// STAGE PROGRAM
// ##########################################################################
// Sampleの構造体
typedef struct _SAMPLEINFO{
	TCHAR	szName[RECIPE_NAME_LEN + 1];
	UINT	nShape;
	double	dThickness;
	struct {
		double dx;
		double dy;
	} Size;
	struct {
		long lX;
		long lY;
	} Origin;
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
	TCHAR	tszElasticModulusName[MATERIAL_NAME_LEN + 1];	/* マテリアル名		ElasticModulusName	*/
	double	dElasticModulusValue;							/* 材質弾性係数		ElasticModulusValue	*/
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */

// 2009.12.01 bagus NANOMAP --{--
// ステージ設定を持つ。結果ファイル出力用
	STAGE_CONFIG	StageConfig;
// 2009.12.01 bagus NANOMAP --}--

	_SAMPLEINFO::_SAMPLEINFO()
	{::ZeroMemory(this, sizeof(_SAMPLEINFO));}
} SAMPLEINFO, *LPSAMPLEINFO;

typedef struct _STAGE_COORD
{
	long lX;
	long lY;
} STAGE_COORD, *LPSTAGE_COORD;

typedef struct _MX_COORD
{
	long lX;
	long lY;
} MX_COORD;

typedef struct _STAGE_COORD_XYZ
{
	long lX;
	long lY;
	long lZ;
} STAGE_COORD_XYZ, *LPSTAGE_COORD_XYZ;

typedef struct _DESKEW_RESULT
{
	double Score;															// score of the matching result
	double XPos;															// (x, y) position of match
	double YPos;															//
	double TargetScore;
} DESKEW_RESULT, *LPDESKEW_RESULT;

typedef struct _DESKEW_SITE
{
	BOOL		bDefined;													// site entered (manually) or taught (pat rec)
	STAGE_COORD Loc;
	WORD		wSize;
} DESKEW_SITE;

typedef DESKEW_SITE DESKEW_SITE_MANUAL;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
/* ストレス ライン/セクション(LS) 構造体 (ステージ PGM 用)*/
typedef struct _STRESS_SECTION_POS {				/* セクション位置情報 定義 ---------------------------------*/
	long	lScanStartPosX;								/* セクション開始 X 位置[um]	L%dS%d_ScanStart_X		*/
	long	lScanEndPosX;								/* セクション終了 X 位置[um]	L%dS%d_ScanEnd_X		*/
	long	lScanPosY;									/* セクション Y 位置[um]		L%dS%d_Y				*/
} STRESS_SECTION_POS, *LPSTRESS_SECTION_POS;
typedef const STRESS_SECTION_POS *LPCSTRESS_SECTION_POS;
/* deleted 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//typedef struct _STRESS_LINESECTION {				/* ラインセクション 定義 -----------------------------------*/
//	BOOL				bValidLine;						/* ラインの有効/無効			ValidLine%d				*/
//	STRESS_SECTION_POS	SectPos[STRESS_SECTIONS_MAX];	/* セクション位置情報 定義	*/
//} STRESS_LINESECTION, *LPSTRESS_LINESECTION;
//typedef const STRESS_LINESECTION *LPCSTRESS_LINESECTION;
/* deleted 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
typedef struct _STGPGM_STRESS_LINE {				/* ライン情報 定義 -----------------------------------------*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//	STRESS_LINESECTION	LineSec;						/* ラインセクション 定義	*/
//	BOOL	bScanValid[STRESS_SECTIONS_MAX];			/* セクションの有効/無効		L%dS%d_ScanValid		*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
	BOOL	bValidLine;									/* ラインの有効/無効			ValidLine%d				*/
	BOOL	bScanValid[STRESS_SECTIONS_MAX];			/* セクションの有効/無効		L%dS%d_ScanValid		*/
	STRESS_SECTION_POS	SectPos[STRESS_SECTIONS_MAX];	/* セクション位置情報 定義	*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
} STGPGM_STRESS_LINE, *LPSTGPGM_STRESS_LINE;
typedef const STGPGM_STRESS_LINE *LPCSTGPGM_STRESS_LINE;
typedef struct _STAGE_PROG_STRESS {					/* ステージ プログラム -------------------------------------*/
	DWORD				dwNumLsScans;					/* 全セクション数				NumLSScans	*/
/* modified 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
//	TCHAR				tszMateElastMod[256];			/* (不明：仮変数)				MaterialElasticModulus	*/
/* modified 2009.07.31 hmenjo ストレス ライン数 定義３ ----------			   */
	TCHAR	tszElasticModulusName[MATERIAL_NAME_LEN + 1];/* マテリアル名				ElasticModulusName		*/
	double	dElasticModulusValue;						/* 材質弾性係数					ElasticModulusValue		*/
/* modified 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
	STGPGM_STRESS_LINE	Line[STRESS_LINES_MAX];			/* ライン情報 定義	*/
} STAGE_PROG_STRESS, *LPSTAGE_PROG_STRESS;
typedef const STAGE_PROG_STRESS *LPCSTAGE_PROG_STRESS;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */

//Saiki 20110208 ----->
// ##########################################################################
// STRESS
// ##########################################################################
typedef struct _STRESS_PARAM_INFO
{
	BOOL bPriority;															//膜厚優先設定 FALSE(デフォルト):実測値 TRUE:固定値
	struct{
		double dPtThick[PIN_INTERVAL];
	} Line[STRESS_LINES_MAX];

} STRESS_PARAM_INFO, *LPSTRESS_PARAM_INFO;
//Saiki 20110208 <-----


typedef struct _STAGE_PROG_INFO_HDR
{
	ADMIN_INFO_HDR hdr;														// ヘッダ
	WORD			wNumScans;
	SAMPLEINFO		SampleInfo;
	WORD			wHeadType;												// ヘッドタイプ
	WORD			wScanType;												// スキャンタイプ
	int				iLens;													// レンズ
	UINT			nDeskewMode;											// Deskewの種類
	UINT			nDeskewFailOption;										// パターン認識失敗時の設定
	DESKEW_SITE		DeskewSite[2];											// パターン認識
	DESKEW_SITE_MANUAL DeskewSiteManual[2];									// Manual Deskew
	UINT			nSitePatternMode;										// SitePatternの使用／未使用
	UINT			nSitePrFailOption;										// サイトパターン失敗時の設定

	_STAGE_PROG_INFO_HDR::_STAGE_PROG_INFO_HDR()
	{::ZeroMemory(this, sizeof(_STAGE_PROG_INFO_HDR));}
} STAGE_PROG_INFO_HDR, *LPSTAGE_PROG_INFO_HDR;

// ##########################################################################
// RECALIBRATION PROGRAM
// ##########################################################################
#define RECALIB_MAX_ENTRY		(5)

typedef struct _RECALIB_ENTRY
{
	double dActual;
	double dMeasured;
} RECALIB_ENTRY;

typedef struct _RECALIB_PROG_INFO
{
	ADMIN_INFO_HDR hdr;														// ヘッダ
	WORD			wHeadType;												// ヘッドタイプ
	WORD			wScanType;												// スキャンタイプ
	WORD			wUnits;													// 単位
// 2009.09.15 K.Matsuo レキャリブレーションのマイナス値入力対応 -->
// 値が-1の場合あり。v2.10未満との互換性のため。
	int				iEntryNum;												// リキャリブレーション入力数
// 2009.09.15 K.Matsuo レキャリブレーションのマイナス値入力対応 <--
	RECALIB_ENTRY	RecalibEntry[RECALIB_MAX_ENTRY];						// リキャリブレーション
	_RECALIB_PROG_INFO::_RECALIB_PROG_INFO()
	{::ZeroMemory(this, sizeof(_RECALIB_PROG_INFO));}
} RECALIB_PROG_INFO, *LPRECALIB_PROG_INFO;

// ##########################################################################
// POINT DESKEW PROGRAM
// ##########################################################################
typedef struct _POINT_DESKEW_PROG_INFO
{
	ADMIN_INFO_HDR	hdr;												// ヘッダ
	WORD			wHeadType;											// ヘッドタイプ
	int				iLens;												// レンズ
	UINT			nDeskewMode;										// Deskewの種類
	UINT			nDeskewFailOption;									// パターン認識失敗時の設定
	DESKEW_SITE		DeskewSite;											// パターン認識
	_POINT_DESKEW_PROG_INFO::_POINT_DESKEW_PROG_INFO()
	{::ZeroMemory(this, sizeof(_POINT_DESKEW_PROG_INFO));}
} POINT_DESKEW_PROG_INFO, *LPPOINT_DESKEW_PROG_INFO;

// ##########################################################################
// MAIN RECPIE
// ##########################################################################

typedef struct _MAIN_RCP_INFO_HDR
{
	WORD	wHeadType;
	TCHAR	szMeas[RECIPE_NAME_LEN + 1];
	TCHAR	szStage[RECIPE_NAME_LEN + 1];
	TCHAR	szPointDeskew[RECIPE_NAME_LEN + 1];
	UINT	nPointManualAdjustment;
	BOOL	bSampleID;
	TCHAR	szSampleID[SAMPLE_ID_LEN + 1];
	BOOL	bSaveMeasResult;
	BOOL	bSaveOverwrite;
//2009.09.02 bagus se 機能拡張 --{--
	BOOL	bGraphAutoScale;
//2009.09.02 bagus se 機能拡張 --}--
//2009.12.17 bagus 保存ファイル名変更 --{--
	BOOL	bSaveNameWithDate;
//2009.12.17 bagus 保存ファイル名変更 --}--
} MAIN_RCP_INFO_HDR, *LPMAIN_RCP_INFO_HDR;

typedef struct _SR_MAIN_RCP_INFO
{
	int		RecalibItem[RECALIB_MAX];
	TCHAR	szRecalib[RECALIB_MAX][RECIPE_NAME_LEN + 1];
	UINT	nFocus;
	UINT	nAutoFocusFailOption;
	UINT	nFinalFocusFailDataValidOption;
	BOOL	bRefWithAF;
	UINT	nDefaultDisplay;
	BOOL	bMeasRef;
} SR_MAIN_RCP_INFO;

// 2009.09.16 K.Matsuo SE -->
//typedef struct _SE_MAIN_RCP_INFO
//{
//} SE_MAIN_RCP_INFO;
typedef SR_MAIN_RCP_INFO SE_MAIN_RCP_INFO;
// 2009.09.16 K.Matsuo SE <--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
typedef SR_MAIN_RCP_INFO COMPEASE_MAIN_RCP_INFO;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

/* modified 2009.09.29 hmenjo メインレシピ ヘッド毎情報合せ ---------- { ---------- */
//typedef struct _IR_MAIN_RCP_INFO
//{
//} IR_MAIN_RCP_INFO;
//
//typedef struct _RS_MAIN_RCP_INFO
//{
//} RS_MAIN_RCP_INFO;
//
//typedef struct _CA_MAIN_RCP_INFO
//{
//} CA_MAIN_RCP_INFO;
/* modified 2009.09.29 hmenjo メインレシピ ヘッド毎情報合せ ----------				*/
// 2009.10.19 bagus MS 追加 --{--
//typedef SR_MAIN_RCP_INFO IR_MAIN_RCP_INFO;
typedef SR_MAIN_RCP_INFO MS_MAIN_RCP_INFO;
// 2009.10.19 bagus MS 追加 --}--

typedef SR_MAIN_RCP_INFO RS_MAIN_RCP_INFO;

typedef SR_MAIN_RCP_INFO CA_MAIN_RCP_INFO;
/* modified 2009.09.29 hmenjo メインレシピ ヘッド毎情報合せ ---------- } ---------- */

typedef struct _MAIN_RCP_PARAM
{
	MAIN_RCP_INFO_HDR hdr;
	union
	{
		SR_MAIN_RCP_INFO _SR;												// SR測定
		SE_MAIN_RCP_INFO _SE;												// SE測定
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		COMPEASE_MAIN_RCP_INFO _COMPEASE;									// COMPEASE測定
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//		IR_MAIN_RCP_INFO _IR;												// IR測定
// 2009.10.19 bagus MS 追加 --}--
		RS_MAIN_RCP_INFO _RS;												// シート抵抗
		CA_MAIN_RCP_INFO _CA;												// コンタクトアングル
// 2009.10.19 bagus MS 追加 --{--
		MS_MAIN_RCP_INFO _MScope;											// MS測定
// 2009.10.19 bagus MS 追加 --}--
	};
} MAIN_RCP_PARAM;

typedef struct _MAIN_RCP_INFO
{
	ADMIN_INFO_HDR	hdr;
	MAIN_RCP_PARAM	MainRcpParam;

	_MAIN_RCP_INFO::_MAIN_RCP_INFO()
	{::ZeroMemory(this, sizeof(_MAIN_RCP_INFO));}
} MAIN_RCP_INFO, *LPMAIN_RCP_INFO;

// ##########################################################################
// MULTI RECPIE
// ##########################################################################
typedef struct _MULTI_RCP_MAIN_RCP_LIST
{
	TCHAR	szName[RECIPE_NAME_LEN + 1];

	_MULTI_RCP_MAIN_RCP_LIST::_MULTI_RCP_MAIN_RCP_LIST()
	{::ZeroMemory(this, sizeof(_MULTI_RCP_MAIN_RCP_LIST));}
} MULTI_RCP_MAIN_RCP_LIST, *LPMULTI_RCP_MAIN_RCP_LIST;

typedef struct _MULTI_RCP_INFO_HDR
{
	ADMIN_INFO_HDR hdr;
	WORD			wNumMainRecipe;

	_MULTI_RCP_INFO_HDR::_MULTI_RCP_INFO_HDR()
	{::ZeroMemory(this, sizeof(_MULTI_RCP_INFO_HDR));}
} MULTI_RCP_INFO_HDR;

/////-------->Recipeデータを取得し、関数にアクセスしてデータを読み出すため保存用の構造体を作成しない
// ##########################################################################
// MEASUREMENT RESULT DATA
// ##########################################################################
typedef struct _MEAS_THICKNESS_DATA
{
	STAGE_COORD		Position;
	double		dThicknessData;
	double		dFitData;
} MEAS_THICKNESS_DATA;

typedef struct _MEAS_REFLECT_DATA
{
	double		dWaveLengthData1;
	double		dWaveLengthData2;
	double		dWaveLengthData3;
} MEAS_REFLECT_DATA;

typedef struct _MEAS_CIE_DATA
{
	STAGE_COORD		Position;
	double		CieLarge_XData;
	double		CieLarge_YData;
	double		CieLarge_ZData;
	double		CieSmall_XData;
	double		CieSmall_YData;
	double		CieLarge_LData;
	double		CieSmall_AData;
	double		CieSmall_BData;
	double		CiePoint_LData;
	double		CiePoint_AData;
	double		CiePoint_BData;
	double		CieApost_UData;
	double		CieApost_VData;
	double		CiePoint_UData;
	double		CiePoint_VData;
} MEAS_CIE_DATA;

typedef struct _MEASUREMENT_DATA
{
	STAGE_COORD		Position;
	TCHAR			szName[RECIPE_NAME_LEN + 1];
	WORD			wHeadType;
	WORD			wScanType;
	TCHAR			szSampleName[RECIPE_NAME_LEN + 1];
	WORD			wNumScans;
	int				iSpecificWavelen[SPECIFIED_WAVELENGTH_MAX];
	double			dMaximumData;
	double			dMinimumData;
	double			dMeanData;
	double			dRangeData;
	double			dSdData;
	union
	{
		MEAS_THICKNESS_DATA Meas_ThicknessData;								// Thickness測定
		MEAS_REFLECT_DATA	Meas_ReflectData;								// Reflectance, Transmittance, OD測定
		MEAS_CIE_DATA		Meas_CieData;									// Chromacitity(Reflec,Trans)測定
	};

	_MEASUREMENT_DATA::_MEASUREMENT_DATA()
	{::ZeroMemory(this, sizeof(_MEASUREMENT_DATA));}
} MEASUREMENT_DATA;
/////<---------

// ##########################################################################
// 構造体(RECIPEFILE関係)
// Recipe，Program，UserAccount，Sampleの読み込み，保存を行うための構造体
// (Iniファイル形式)
// ##########################################################################
typedef struct _RECIPE_FILE{
	LPCSTR	section;
	LPCSTR	key;
	TCHAR	type;
	LPCSTR	def;
	void*	data;
} RECIPE_FILE;

// ##########################################################################
// Iniファイルの構造体
// ##########################################################################
typedef struct
{
	LPCSTR	section;
	LPCSTR	key;
	TCHAR	type;
	LPCSTR	def;
	void*	data;
} INIFILE;

// ##########################################################################
// レンズのオフセットの構造体
// ##########################################################################
typedef struct _SR_LENS_CENTER_OFFSET{
	int iBaseLens;
	struct{
		long lX;
		long lY;
		long lZ;	// 2013.02.22 bagus Substrate thickness setting
	} Lens[SR_LENS_MAX];
} SR_LENS_CENTER_OFFSET;

// ##########################################################################
// System Configuration
// ##########################################################################

// ==========================================================================
// SystemConfiguration
// ==========================================================================
typedef struct _HEAD_TYPE_CONFIG{
	BOOL	bSR;
	BOOL	bSE;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	BOOL	bCompEASE;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	BOOL	bIRSE;
// 2009.10.19 bagus MS 追加 --}--
	BOOL	bResist;
	BOOL	bCTA;
	BOOL	bStress;
// 2009.10.17 bagus MS 追加 --{--
	BOOL	bMS;
// 2009.10.17 bagus MS 追加 --}--
} HEAD_TYPE_CONFIG;

typedef struct _SYSTEM_CONFIG{
	HEAD_TYPE_CONFIG	HeadType;

	UINT	nStageType;
	UINT	nVibrationControlUnit;
	BOOL	bVibrationDownSensor;
	BOOL	bSampleExistenceSensor;
	UINT	nSampleLifter;
	BOOL	bSampleAligner;
	UINT	nVacuumType;
	int		iVacuumTimeout;
	double	dCorrectAngle;
	UINT	nCaptureBoard;
	UINT	nPRMethod;
	BOOL	bSamplePatternRec;
	BOOL	bSitePatternRec;
	BOOL	bHost;
	BOOL	bSignalTower;
	UINT	nLanguage;
	BOOL	bMultiRecipe;
	BOOL	bExtraViewCamera;
/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- { ---------- */
	BOOL	bUseLoadingShutter;
/* added 2009.06.16 hmenjo Loading Shutter 使用有無追加 ---------- } ---------- */
/* deleted 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- { ---------- */
///* added 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- { ---------- */
//	BOOL	bUseOptFilterMeasPgmAF;
///* added 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- } ---------- */
/* deleted 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- } ---------- */
// 2009.10.29 bagus Jog 追加修正 --{--
	UINT	nJoystickType;
// 2009.10.29 bagus Jog 追加修正 --}--
// 2009.12.10 bagus Recipe Backup --{--
	BOOL	bBackup;
// 2009.12.10 bagus Recipe Backup --}--
	BOOL	bDisableNextra;

	_SYSTEM_CONFIG::_SYSTEM_CONFIG()
	{::ZeroMemory(this, sizeof(_SYSTEM_CONFIG));}
} SYSTEM_CONFIG;

// ==========================================================================
// Original Position
// ==========================================================================
typedef struct _ORIGINAL_POSITION{
	WORD				wBaseHeadType;										// ヘッドタイプ
	STAGE_COORD_XYZ		LocSr;
	STAGE_COORD_XYZ		LocSe;
// 2009.10.19 bagus MS 追加 --{--
//	STAGE_COORD_XYZ		LocIrse;
// 2009.10.19 bagus MS 追加 --}--
	STAGE_COORD_XYZ		Loc4PP;
	STAGE_COORD_XYZ		LocContactAngle;
	STAGE_COORD_XYZ		LocStress;
// 2009.10.19 bagus MS 追加 --{--
	STAGE_COORD_XYZ		LocMs;
// 2009.10.19 bagus MS 追加 --}--

	_ORIGINAL_POSITION::_ORIGINAL_POSITION()
	{::ZeroMemory(this, sizeof(_ORIGINAL_POSITION));}
} ORIGINAL_POSITION;


// ==========================================================================
// Focus Configuration
// ==========================================================================
typedef struct _FOCUS_CONFIG{
	struct{
		BOOL	bValid;
		double	dDiv;
		double	dOffset;
	} Enc;
	BOOL	bOffsetCtrl;

	_FOCUS_CONFIG::_FOCUS_CONFIG()
	{::ZeroMemory(this, sizeof(_FOCUS_CONFIG));}
} FOCUS_CONFIG;

// ==========================================================================
// Edge Aligner
// ==========================================================================
typedef struct _EDGE_ALIGNER{
	STAGE_COORD		Start;
	STAGE_COORD		End;
	struct{
		STAGE_COORD		X[2];
		STAGE_COORD		Y[2];
	} Sensor;

	_EDGE_ALIGNER::_EDGE_ALIGNER()
	{::ZeroMemory(this, sizeof(_EDGE_ALIGNER));}
} EDGE_ALIGNER;

// ==========================================================================
// General Position
// ==========================================================================
typedef struct _GENERAL_POSITION{
	STAGE_COORD		Loc;
	TCHAR		szComment[GENERAL_POSITION_COMMENT_LEN + 1];

	_GENERAL_POSITION::_GENERAL_POSITION()
	{::ZeroMemory(this, sizeof(_GENERAL_POSITION));}
} GENERAL_POSITION;

// ==========================================================================
// Maintenance Position
// ==========================================================================
typedef struct _MAINTENANCE_POSITION{
	STAGE_COORD		Loc;
	TCHAR		szComment[MAINTENANCE_POSITION_COMMENT_LEN + 1];

	_MAINTENANCE_POSITION::_MAINTENANCE_POSITION()
	{::ZeroMemory(this, sizeof(_MAINTENANCE_POSITION));}
} MAINTENANCE_POSITION;

// ==========================================================================
// Signal Tower
// ==========================================================================
typedef struct _SIGNAL_TOWER_SETTING{
	int iRed;
	int iYellow;
	int iGreen;
	int iBuzzer;
} SIGNAL_TOWER_SETTING;

typedef struct _SIGNAL_TOWER{
	SIGNAL_TOWER_SETTING	Initialize;
	SIGNAL_TOWER_SETTING	Idle;
	SIGNAL_TOWER_SETTING	Run;
	SIGNAL_TOWER_SETTING	Alarm;

	_SIGNAL_TOWER::_SIGNAL_TOWER()
	{::ZeroMemory(this, sizeof(_SIGNAL_TOWER));}
} SIGNAL_TOWER;

/* added 2009.07.22 hmenjo ストレス ライン数 定義２/SystemStress.ini 追加２ ---------- { ---------- */
/* ##########################################################################
 * Stress Configuration (ストレス ライン/セクション 設定)
 * ####################################################################### */
typedef struct _STRESS_CFG_MEASPOS {				/* 膜厚測定位置情報 定義 -----------------------------------*/
	long	lX;											/* 膜厚測定位置 X[um]			L%dP%d_Thick_X			*/
	long	lY;											/* 膜厚測定位置 Y[um]			L%dP%d_Thick_Y			*/
} STRESS_CFG_MEASPOS, *LPSTRESS_CFG_MEASPOS;
typedef const STRESS_CFG_MEASPOS *LPCSTRESS_CFG_MEASPOS;
typedef struct _STRESS_CFG_PINPOS {					/* ピン位置情報 定義 ---------------------------------------*/
	long	lX;											/* ピン位置 X[um]				LiftPinLocL%dPin%d_X	*/
	long	lY;											/* ピン位置 Y[um]				LiftPinLocL%dPin%d_Y	*/
} STRESS_CFG_PINPOS, *LPSTRESS_CFG_PINPOS;
typedef const STRESS_CFG_PINPOS *LPCSTRESS_CFG_PINPOS;
typedef struct _STRESS_CFG_LINE {					/* ライン情報 定義 -----------------------------------------*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//	DWORD	dwLiftPinNumberOfLn;						/* ライン毎のピン数				LiftPinNumberOfL%d		*/
//	STRESS_CFG_PINPOS	PinPos[STRESS_PINS_MAX];		/* ピン位置情報 定義	*/
//	STRESS_CFG_MEASPOS	MeasPos[STRESS_PINS_MAX];		/* 膜厚測定位置情報 定義	*/
//	DWORD				dwSectionNum;					/* ライン毎のセクション数		L%dSecNum				*/
//	STRESS_LINESECTION	LineSec;						/* ラインセクション 定義	*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
	BOOL				bValidLine;						/* ラインの有効/無効			ValidLine%d				*/
	DWORD				dwLiftPinNumberOfLn;			/* ライン毎のピン数				LiftPinNumberOfL%d		*/
	STRESS_CFG_PINPOS	PinPos[STRESS_PINS_MAX];		/* ピン位置情報 定義	*/
	STRESS_CFG_MEASPOS	MeasPos[STRESS_PINS_MAX];		/* 膜厚測定位置情報 定義	*/
	DWORD				dwSectionNum;					/* ライン毎のセクション数		L%dSecNum				*/
	STRESS_SECTION_POS	SectPos[STRESS_SECTIONS_MAX];	/* セクション位置情報 定義	*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
} STRESS_CFG_LINE, *LPSTRESS_CFG_LINE;
typedef const STRESS_CFG_LINE *LPCSTRESS_CFG_LINE;
typedef struct _STRESS_CONFIG {						/* ストレス設定 --------------------------------------------*/
	DWORD	dwSamplingInterval;							/* サンプリングインターバル[us]	SamplingInterval		*/
	DWORD	dwDataIntegrationInterval;					/* データ蓄積間隔[サンプル数]	DataIntegrationInterval	*/
	DWORD	dwScanStartWaitTime;						/* HEPA 待ち時間[s]				ScanStartWaitTime		*/
	DWORD	dwScanSpeed;								/* 測定速度[mm/s]				ScanSpeed				*/
// K.Matsuo 2009.09.07 Change -->
	long	lScanStartPosX;								/* スキャン開始位置 X[um]		ScanStartPosition		*/
	long	lScanEndPosX;								/* スキャン終了位置 X[um]		ScanEndPosition			*/
	long	lLineStartPosX;								/* ライン移動開始位置 X[um]		LineMoveStartPosition	*/
	long	lLineEndPosX;								/* ライン移動終了位置 X[um]		LineMoveEndPosition		*/
// K.Matsuo 2009.09.07 Change <--
/* added 2009.09.29 hmenjo Stress Z 軸スキャン位置 ---------- { ---------- */
	long	lScanZAxisPosition;							/* Z 軸スキャン位置[0.1um]		ScanZAxisPosition		*/
/* added 2009.09.29 hmenjo Stress Z 軸スキャン位置 ---------- } ---------- */
	DWORD	dwShape;									/* 形状							Shape					*/
	struct {											/* サンプルサイズ情報	*/
		double	dx;										/*		サイズ X[um]			Size_X					*/
		double	dy;										/*		サイズ Y[um]			Size_Y					*/
	} Size;
	struct {											/* 原点位置情報	*/
		long	lX;										/*		X[um]					Origin_X				*/
		long	lY;										/*		Y[um]					Origin_Y				*/
	} Origin;
	long	lStressValidRange;							/* ストレス測定有効範囲[um]		StressValidRange		*/
	long	lInvalidRangeEdge;							/* サンプル端 無効領域[um]		InvalidRangeEdge		*/
	struct {											/* 膜厚測定座標オフセット*/
		long	lX;										/*		X[um]					ThickMeasPointOffset_X	*/
		long	lY;										/*		Y[um]					ThickMeasPointOffset_Y	*/
	} MeasOffset;
	DWORD	dwLiftPinNumberOfLine;						/* ライン数						LiftPinNumberOfLine		*/
	STRESS_CFG_LINE		Line[STRESS_LINES_MAX];			/* ライン情報 定義	*/
	//2009.12.21 bagus stress --{--
	TCHAR	szSampleName[RECIPE_NAME_LEN + 1];			// サンプル名
	//2009.12.21 bagus stress --}--
} STRESS_CONFIG, *LPSTRESS_CONFIG;
typedef const STRESS_CONFIG *LPCSTRESS_CONFIG;
/* added 2009.07.22 hmenjo ストレス ライン数 定義２/SystemStress.ini 追加２ ---------- } ---------- */

/* deleted 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
///* added 2009.08.25 hmenjo SE 構造体追加(１) ---------- { ---------- */
//// ##########################################################################
//// SE Configuration
//// ##########################################################################
//typedef struct _SE_CONFIG{
//	BOOL	bDoNotMoveShutter;				/* 通常/運用モード[TRUE：運用]		DoNotMoveShutter	*/
//
//	_SE_CONFIG::_SE_CONFIG()
//	{::ZeroMemory(this, sizeof(_SE_CONFIG));}
//} SE_CONFIG, *LPSE_CONFIG;
//typedef const SE_CONFIG *LPCSE_CONFIG;
///* added 2009.08.25 hmenjo SE 構造体追加(１) ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// ==========================================================================
// CompleteEASE Configuration
// =========================================================================
typedef struct _COMPEASE_CONFIG {
	BOOL	bDoNotMoveShutter;				/* 通常/運用モード[TRUE：運用]		DoNotMoveShutter	*/
	int	nAnalyzeTimeout;				/* EASE解析タイムアウト						*/
	TCHAR	szIpAddress[IP_ADDRESS_LEN+1];
	UINT	nPort;
	UINT	nAsyncPort;

	_COMPEASE_CONFIG::_COMPEASE_CONFIG()
	{::ZeroMemory(this, sizeof(_COMPEASE_CONFIG));}
} COMPEASE_CONFIG, *LPCOMPEASE_CONFIG;
typedef const COMPEASE_CONFIG *LPCCOMPEASE_CONFIG;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// 2009.10.19 bagus MS 追加 --{--
// ##########################################################################
// MS Configuration
// ##########################################################################
typedef struct _MS_ROBO_CYLINDER_POS{
	BOOL	bEnable;					// 使用/未使用
	TCHAR	szName[16];					// レンズ名
	UINT	nPosition;					// ティーチングポジション
	BOOL	bSensor;					// センサーOn/Off

	_MS_ROBO_CYLINDER_POS::_MS_ROBO_CYLINDER_POS()
	{::ZeroMemory(this, sizeof(_MS_ROBO_CYLINDER_POS));}
} MS_ROBO_CYLINDER_POS;

typedef struct _MS_CONFIG{
	MS_ROBO_CYLINDER_POS	RoboCylinder[MAX_MS_ROBO_CYLINDER_NUM];
// 2009.10.30 bagus MS 追加 --{--
	LONG	lStdZAxisRetractPos;		// Z軸退避位置座標
// 2009.10.30 bagus MS 追加 --}--

	_MS_CONFIG::_MS_CONFIG()
	{::ZeroMemory(this, sizeof(_MS_CONFIG));}
} MS_CONFIG, *LPMS_CONFIG;
// 2009.10.19 bagus MS 追加 --}--

// 2009.10.30 bagus CA 追加 --{--
// ##########################################################################
// CA Configuration
// ##########################################################################
typedef struct _CA_CONFIG{
	LONG	lCaStdZAxisRetractPos;				// Z軸退避位置座標

	_CA_CONFIG::_CA_CONFIG()
	{::ZeroMemory(this, sizeof(_CA_CONFIG));}
} CA_CONFIG, *LPCA_CONFIG;
// 2009.10.30 bagus CA 追加 --}--

// 2009.11.04 bagus RS 追加 --{--
typedef struct _RS_CONFIG{
	LONG	lRsStdZAxisRetractPos;				// Z軸退避位置座標
	UINT	nRsProbeType;						// プローブ

	_RS_CONFIG::_RS_CONFIG()
	{::ZeroMemory(this, sizeof(_RS_CONFIG));}
} RS_CONFIG, *LPRS_CONFIG;
// 2009.11.04 bagus RS 追加 --}--

// ##########################################################################
// SR Configuration
// ##########################################################################
typedef struct _SR_CONFIG{
	// Head
	UINT	nHeadType;
	double	dIntegrationTime;
	UINT	nHeadInterface;
	double	dSecondIntegrationTime;
// 2014.04.04 bagus wavelength step added -->
	UINT	nWavelengthStep;
// 2014.04.04 bagus wavelength step added <--

	// Option
	UINT	nTurret;
	BOOL	bAutoFocus;
	BOOL	bChooseAfFuncInReference;
	BOOL	bAutoFilter;
/* added 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- { ---------- */
	BOOL	bUseOptFilterMeasPgmAF;
/* added 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- } ---------- */
	BOOL	bTransmittance;
	BOOL	bBuiltInReference;
	BOOL	bCieMeasurement;
	BOOL	bOdMeasurement;
	BOOL	bUv;
	double	dUvGain;
	BOOL	bUvAutoShutter;
/* added 2009.09.07 hmenjo 透過率 構造体追加(１) ---------- { ---------- */
	int		iTrUseLensNo;				/* 透過率  使用レンズ番号		TrUseTurretNo				*/
	WORD	wTrShutterMoveWaitTime;		/* 透過率  シャッタ動作時間[ms]	TrShutterMoveWaitTime		*/
	BOOL	bGantryType;				/* 透過率  ガントリタイプ		GantryType					*/
	WORD	wGantryNumOfMeasPoint;		/* 透過率  光源数				GantryNumberOfMeasurePoint	*/
/* added 2009.09.07 hmenjo 透過率 構造体追加(１) ---------- } ---------- */
/* added 2009.09.08 hmenjo 透過率 構造体追加(４) ---------- { ---------- */
	int		iDefaultMeasLensNo;			/* デフォルト レンズ	DefaultMeasureTurretNo	*/
	WORD	wDefaultMeasFilter;			/* デフォルト フィルタ	DefaultMeasureFilter	*/
/* added 2009.09.08 hmenjo 透過率 構造体追加(４) ---------- } ---------- */
// 2009.10.13 bagus Distance 追加 --{--
	BOOL	bDistance;
// 2009.10.13 bagus Distance 追加 --}--

	_SR_CONFIG::_SR_CONFIG()
	{::ZeroMemory(this, sizeof(_SR_CONFIG));}
} SR_CONFIG;

/* added 2009.07.24 hmenjo ストレス ライン数 定義２ ---------- { ---------- */
/* ==========================================================================
 * ストレス 生データ 構造体
 * ========================================================================*/
typedef struct _STRESS_RAW_LINE {				/* ライン毎データ 定義	*/
	BOOL	bValidLine;								/* ラインの有効/無効	Line%dValidScan	*/
	struct {
		long	lScanPos;							/* 位置[um]				ScanPosition	*/
		long	lHeight;							/* 変位[um]				Height			*/
	} HeightInfo[DEFL_CNT_MAX];
} STRESS_RAW_LINE, *LPSTRESS_RAW_LINE;
typedef const STRESS_RAW_LINE *LPCSTRESS_RAW_LINE;
typedef struct _STRESS_REFERENCE {
	TCHAR	tszMainRecipeName[256];					/* メインレシピ名		Name			*/
	STRESS_RAW_LINE	Line[STRESS_LINES_MAX];			/* ライン毎データ 定義	*/
} STRESS_RAW_DATA, *LPSTRESS_RAW_DATA;
typedef const STRESS_RAW_DATA *LPCSTRESS_RAW_DATA;
/* added 2009.07.24 hmenjo ストレス ライン数 定義２ ---------- } ---------- */
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
/* ==========================================================================
 * ストレス サンプル材料弾性係数テーブル 構造体
 * ========================================================================*/
typedef struct _STRESS_MODULUS {						/* 弾性係数 定義	*/
	TCHAR	tszElasticModulusName[MATERIAL_NAME_LEN + 1];	/* マテリアル名		TableName%d		*/
	double	dElasticModulusValue;							/* 材質弾性係数		TableValue%d	*/
} STRESS_MODULUS, *LPSTRESS_MODULUS;
typedef const STRESS_MODULUS *LPCSTRESS_MODULUS;
typedef struct _STRESS_ELASTIC_TABLE {
	UINT			uiTableNum;								/* テーブル数		TableNumber		*/
	STRESS_MODULUS	ElasticModulus[STRESS_MODULUS_MAX];		/* 弾性係数 定義	*/
} STRESS_ELASTIC_TABLE, *LPSTRESS_ELASTIC_TABLE;
typedef const STRESS_ELASTIC_TABLE *LPCSTRESS_ELASTIC_TABLE;
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */

// ==========================================================================
// SR Filter
// ==========================================================================
typedef struct _SR_FILTER{
	BOOL			bEnable;
	TCHAR			szName[RECIPE_NAME_LEN + 1];
	WAVELEN_RANGE	Wavelength;

	_SR_FILTER::_SR_FILTER()
	{::ZeroMemory(this, sizeof(_SR_FILTER));}
} SR_FILTER;

// ==========================================================================
// SR Turret
// ==========================================================================
typedef struct _SR_TURRET{
	BOOL	bEnable;
	TCHAR	szName[RECIPE_NAME_LEN + 1];
	double	dXmpAngle;
/* added 2009.09.08 hmenjo 透過率 構造体追加(５) ---------- { ---------- */
	BOOL	bAnalysis;		/* TRUE で解析用	Turret%d_Analysis	*/
	int		iAFLens;		/* AF 時のレンズ	Turret%d_AFTurret	*/
/* added 2009.09.08 hmenjo 透過率 構造体追加(５) ---------- } ---------- */

	_SR_TURRET::_SR_TURRET()
	{::ZeroMemory(this, sizeof(_SR_TURRET));}
} SR_TURRET;

// ==========================================================================
// SR AutoFocus Retry
// ==========================================================================
typedef struct _SR_AUTOFOCUSRETRY{
	UINT	nTimes;

	_SR_AUTOFOCUSRETRY::_SR_AUTOFOCUSRETRY()
	{::ZeroMemory(this, sizeof(_SR_AUTOFOCUSRETRY));}
} _SR_AUTOFOCUSRETRY;

// ==========================================================================
// SR XMP
// ==========================================================================
typedef struct _SR_XMP{
	TCHAR	szStrategyListPath[256];
	TCHAR	szDataPath[256];
	TCHAR	szFittingDataPath[256];
	TCHAR	szAdapExePath[256];
	TCHAR	szWvaseWindowTitle[256];

	_SR_XMP::_SR_XMP()
	{::ZeroMemory(this, sizeof(_SR_XMP));}
} SR_XMP;
// ==========================================================================
// SE XMP
// ==========================================================================
//2009.09.07 bagus se --{--
// SE用XMPはSRと同等の情報を持つ
typedef SR_XMP SE_XMP;
//2009.09.07 bagus se --}--

// ==========================================================================
// SR View Range
// ==========================================================================
typedef struct _SR_VIEW_RANGE{
	long	lLeft;
	long	lTop;
	long	lWidth;
	long	lHeight;

	_SR_VIEW_RANGE::_SR_VIEW_RANGE()
	{::ZeroMemory(this, sizeof(_SR_VIEW_RANGE));}
} SR_VIEW_RANGE;

// ==========================================================================
// SR Video Calibration
// ==========================================================================
typedef struct _SR_VIDEO_CALIB{
	double	dXPixelToMicron;
	double	dYPixelToMicron;

	_SR_VIDEO_CALIB::_SR_VIDEO_CALIB()
	{::ZeroMemory(this, sizeof(_SR_VIDEO_CALIB));}
} SR_VIDEO_CALIB;

// ==========================================================================
// SR CCD Array
// ==========================================================================
typedef struct _SR_CCD_ARRAY{
	TCHAR	szItem[16 + 1];
	double	dData;

	_SR_CCD_ARRAY::_SR_CCD_ARRAY()
	{::ZeroMemory(this, sizeof(_SR_CCD_ARRAY));}
} SR_CCD_ARRAY;

#if 0 // 2014.04.04 bagus wavelength step modified -->
// // =========================================================================
// // SR MEASYS CCD Data Array
// typedef struct _SCANDATA{
// 	int iScanStart;
// 	int iScanEnd;
// 	double dScanData[SCANDATA_POINT_MAX]; // 例：添字の379には、380nmの光量データがあります
// } SCANDATA, *PSCANDATA;
//
// // =========================================================================
// // Analysis Data Array
// typedef struct _ANALYSISDATA{
// 	int iScanStart;
// 	int iScanEnd;
// 	double dAnalysisReflectanceData[SCANDATA_POINT_MAX]; // 例：添字の379には、380nmの反射率データがあります（SR膜厚測定（XMP使用）時、aft2adap.datの元データ）
// 	double dAnalysisFitData[SCANDATA_POINT_MAX];		 // 例：添字の379には、380nmのFitデータがあります（SR膜厚測定（XMP使用）時、gendata.datのデータを格納する）
// } ANALYSISDATA, *PANALYSISDATA;
//
#else
// =========================================================================
// SR MEASYS CCD Data Array
typedef struct _SCANDATA{
	int iScanStart;
	int iScanEnd;
	double dWavelengthStep;
	double dScanData[SCANDATA_POINT_MAX]; // 波長から配列インデックスを求める (波長 - iScanStart) / dWavelengthStep
	// helper function.
	double GetScanData(double dWavelength)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)iScanStart) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)iScanStart));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return 0.0;
		}

		return dScanData[iIndex];
	}

	void SetScanData(double dWavelength, double dData)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)iScanStart) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)iScanStart));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return;
		}

		dScanData[iIndex] = dData;
	}
} SCANDATA, *PSCANDATA;
#endif // 2014.04.04 bagus wavelength step modified <--

// =========================================================================
// Analysis Data Array
typedef struct _ANALYSISDATA{
	int iScanStart;
	int iScanEnd;
	double dWavelengthStep;
	double dAnalysisReflectanceData[SCANDATA_POINT_MAX]; // 波長から配列インデックスを求める (波長 - iScanStart) / dWavelengthStep
	double dAnalysisFitData[SCANDATA_POINT_MAX];		 // 波長から配列インデックスを求める (波長 - iScanStart) / dWavelengthStep
	// helper function.
	double GetAnalysisReflectanceData(double dWavelength)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)iScanStart) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)iScanStart));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return 0.0;
		}

		return dAnalysisReflectanceData[iIndex];
	}

	void SetAnalysisReflectanceData(double dWavelength, double dData)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)iScanStart) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)iScanStart));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return;
		}

		dAnalysisReflectanceData[iIndex] = dData;
	}

	double GetAnalysisFitData(double dWavelength)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)iScanStart) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)iScanStart));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return 0.0;
		}

		return dAnalysisFitData[iIndex];
	}

	void SetAnalysisFitData(double dWavelength, double dData)
	{
		int iIndex;

		if(dWavelengthStep == 0.5){
			iIndex = (int)((dWavelength - (double)iScanStart) * 2.0);
		}
		else{
			iIndex = (int)((dWavelength - (double)iScanStart));
		}

		if(iIndex < 0 || SCANDATA_POINT_MAX - 1 < iIndex){
			// out of range error.
			return;
		}

		dAnalysisFitData[iIndex] = dData;
	}
} ANALYSISDATA, *PANALYSISDATA;

// 2013.02.22 bagus Substrate thickness setting -->
// ==========================================================================
// Substrate Thickness
typedef struct _SUBSTRATE_THICKNESS {
	double	dThickness;					// 板厚
	double	dInvalidRegion;					// 無効領域
	BOOL	bUseData;
} SUBSTRATE_THICKNESS, *LPSUBSTRATE_THICKNESS;
typedef const SUBSTRATE_THICKNESS *LPCSUBSTRATE_THICKNESS;
typedef struct _SUBSTRATE_THICKNESS_TABLE {
	SUBSTRATE_THICKNESS	SubstrateThickness[SUBSTRATE_THICKNESS_MAX];
} SUBSTRATE_THICKNESS_TABLE, *LPSUBSTRATE_THICKNESS_TABLE;
typedef const SUBSTRATE_THICKNESS_TABLE *LPCSUBSTRATE_THICKNESS_TABLE;
// 2013.02.22 bagus Substrate thickness setting <--

// ##########################################################################
// User Setting
// ##########################################################################

// ==========================================================================
// Access Level
// ==========================================================================
typedef struct _ACCESS_LEVEL{
	BOOL bMeasurement;
	BOOL bRecipe;
	BOOL bProgram;
	BOOL bData;
	BOOL bUserSetting;
	BOOL bMaintenance;
} ACCESS_LEVEL;

// ==========================================================================
// Default Head Type
// ==========================================================================
typedef struct _DEFAULT_HEAD_TYPE{
	UINT	nHeadType;
	UINT	nMeasType;

	_DEFAULT_HEAD_TYPE::_DEFAULT_HEAD_TYPE()
	{::ZeroMemory(this, sizeof(_DEFAULT_HEAD_TYPE));}
} DEFAULT_HEAD_TYPE;

// ==========================================================================
// SR Pattern Rec
// ==========================================================================
typedef struct _SR_PATTERN_REC{
	UINT	nNumLayers;
	double	dLowestMatchScore;
	double	dStopGropingScore;

	// Matsuhisa 2009.05.30 ----->
	struct {
		BOOL bEnable;
		long XRad;
		long YRad;
	} Mask;
	// Matsuhisa 2009.05.30 <-----

	_SR_PATTERN_REC::_SR_PATTERN_REC()
	{::ZeroMemory(this, sizeof(_SR_PATTERN_REC));}
} SR_PATTERN_REC;

// ==========================================================================
// SR Reference Position
// ==========================================================================
typedef struct _SR_REFERENCE_POSITION{
	STAGE_COORD_XYZ LocDarkReference;
	STAGE_COORD_XYZ LocReflectReference;
	STAGE_COORD_XYZ LocTransmitAbsoluteReference;
	STAGE_COORD_XYZ LocTransmitFirstReference;
	STAGE_COORD_XYZ LocTransmitSecondReference;
//2009.09.28 bagus gantry --{--
	STAGE_COORD_XYZ LocGantryTransmitReference[MAX_GANTRY_MEAS_POINT_NUM];
//2009.09.28 bagus gantry --}--

	_SR_REFERENCE_POSITION::_SR_REFERENCE_POSITION()
	{::ZeroMemory(this, sizeof(_SR_REFERENCE_POSITION));}
} SR_REFERENCE_POSITION;

// ==========================================================================
// SR 表示フォーマット
// ==========================================================================
typedef struct SR_AUTOFOCUS_RETRY{
	UINT nTimes;

	SR_AUTOFOCUS_RETRY::SR_AUTOFOCUS_RETRY()
	{::ZeroMemory(this, sizeof(SR_AUTOFOCUS_RETRY));}
} SR_AUTOFOCUS_RETRY;

// ==========================================================================
// SR オートフォーカスリトライ
// =========================================================================
typedef struct _SR_FIGURE_FORMAT{
	UINT nThicknessFormat;
	UINT nThicknessUnit;
	UINT nReflectanceFormat;
	UINT nTransmittanceFormat;
	UINT nChromaticityFormat;
	UINT nOpticalDensityFormat;
/* added 2017.04.15 hmenjo 整数表示化 ---------- { ---------- */
	int iThicknessIntOnly;
/* added 2017.04.15 hmenjo 整数表示化 ---------- } ---------- */

	_SR_FIGURE_FORMAT::_SR_FIGURE_FORMAT()
	{::ZeroMemory(this, sizeof(_SR_FIGURE_FORMAT));}
} SR_FIGURE_FORMAT;
//2009.09.07 bagus se --{--
// ==========================================================================
// SE 測定単位構造体
// =========================================================================
typedef struct _SE_FIGURE_FORMAT{
	UINT nThicknessFormat;
	UINT nThicknessUnit;
	_SE_FIGURE_FORMAT::_SE_FIGURE_FORMAT()
	{::ZeroMemory(this, sizeof(_SE_FIGURE_FORMAT));}
} SE_FIGURE_FORMAT;
//2009.09.07 bagus se --}--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
typedef SE_FIGURE_FORMAT COMPEASE_FIGURE_FORMAT;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

/* added 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
typedef struct _SE_SETTING {
	BOOL	bDoNotMoveShutter;				/* 通常/運用モード[TRUE：運用]		DoNotMoveShutter	*/

	_SE_SETTING::_SE_SETTING()
	{::ZeroMemory(this, sizeof(_SE_SETTING));}
} SE_SETTING, *LPSE_SETTING;
typedef const SE_SETTING *LPCSE_SETTING;
/* added 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
typedef SE_SETTING COMPEASE_SETTING;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

/* added 2009.09.07 hmenjo 透過率 構造体追加(２) ---------- { ---------- */
typedef struct _SR_TRANSMIT {
	BOOL	bDoNotCheckMeasPoint;			/* 通常/運用モード[TRUE：運用]		DoNotCheckMeasPoint	*/

	_SR_TRANSMIT::_SR_TRANSMIT()
	{::ZeroMemory(this, sizeof(_SR_TRANSMIT));}
} SR_TRANSMIT, *LPSR_TRANSMIT;
typedef const SR_TRANSMIT *LPCSR_TRANSMIT;
/* added 2009.09.07 hmenjo 透過率 構造体追加(２) ---------- } ---------- */

// ##########################################################################
// XYStage, ZAxisの移動速度の設定
// ##########################################################################
typedef struct tagSTAGE_MOTION_JOG_DATA {
	int Jog[3];			// low, mid, high
} STAGE_MOTION_JOG_DATA, *PSTAGE_MOTION_JOG_DATA;

typedef struct tagSTAGE_MOTION_SPEED_DATA {
	int Speed[3];			// low, mid, high
	int Jog;
} STAGE_MOTION_SPEED_DATA, *PSTAGE_MOTION_SPEED_DATA;

typedef STAGE_COORD		MANUAL_LOAD_POSITION;
typedef STAGE_COORD		HANDLER_LOAD_POSITION;

// ##########################################################################
// メンテナンスの設定
// ##########################################################################
typedef struct tagMAINTENANCE_DATE {
	TCHAR	MaintenanceDate[20];
	TCHAR	NextMaintenanceDate[20];
	UINT	nDaysSpan;
	BOOL	nAlarmFlag;
} MAINTENANCE_DATE;

typedef struct tagMAINTENANCE_LAMP {
	TCHAR	MaintenanceDate[20];
	double	MaintenanceHour;
	UINT	nHourSpan;
	BOOL	nAlarmFlag;
} MAINTENANCE_LAMP;

// ##########################################################################
// レシピバックアップの設定
// ##########################################################################
typedef struct tagBACKUP_SETTING {
	BOOL bAutoBackup;
	TCHAR BackupPath[MAX_PATH];
} BACKUP_SETTING;

// ##########################################################################
// User Setting関連
// ##########################################################################

// ==========================================================================
// USER DATA
// ==========================================================================
typedef struct tagUSER_DATA {
	// Common
	MANUAL_LOAD_POSITION	ManualLoadPosition;
	TCHAR					szAutoLogonName[USER_NAME_LEN + 1];
	ACCESS_LEVEL			AccessLevel[ACCOUNT_LEVEL_MAX];
	int						iDeleteDataPeriod;
	DEFAULT_HEAD_TYPE		DefaultHeadType;
	STAGE_MOTION_JOG_DATA	StageMotionJogData[MAX_AXIS];
	STAGE_MOTION_SPEED_DATA	StageMotionSpeedData[MAX_AXIS];
	MAINTENANCE_POSITION	MaintenancePosition[MAINTENANCE_POSITION_MAX];
	MAINTENANCE_DATE		MaintenanceDate;
	MAINTENANCE_LAMP		MaintenanceLamp;

	// SR
	SR_PATTERN_REC			SrPatternRec;
	SR_REFERENCE_POSITION	SrReferencePosition;
	SR_FIGURE_FORMAT		SrFigureFormat;
	SR_AUTOFOCUS_RETRY		SrAutoFocusRetry;
/* added 2009.09.07 hmenjo 透過率 構造体追加(２) ---------- { ---------- */
	SR_TRANSMIT				SrTransmittance;
/* added 2009.09.07 hmenjo 透過率 構造体追加(２) ---------- } ---------- */
/* added 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
	/* SE	*/
	SE_SETTING				SeSetting;
/* added 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */
	//2009.09.07 bagus se --{--
	SE_FIGURE_FORMAT		SeFigureFormat;
	//2009.09.07 bagus se --}--

// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// CompEASE
	COMPEASE_SETTING		CompEASESetting;
	COMPEASE_FIGURE_FORMAT	CompEASEFigureFormat;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	//2009.12.10 bagus Recipe Backup --{--
	BACKUP_SETTING				BackupData;
	//2009.12.10 bagus Recipe Backup --}--
} USER_DATA;

// ==========================================================================
// HOST_ADDRESS (My IP Address to connection CIM-PC.)
// ==========================================================================
typedef struct _HOST_ADDRESS{
	TCHAR	szMyIpAddress[IP_ADDRESS_LEN+1];
	UINT	nPort;
	TCHAR	szNanoPifSockWindowTitle[256];
} HOST_ADDRESS;

// ==========================================================================
// COM_SETTING
// ==========================================================================
typedef struct _COM_PORT{
	TCHAR	szCommNo[16];
	TCHAR	szBaudRate[16];
	TCHAR	szParity[16];
	TCHAR	szByteSize[16];
	TCHAR	szStopBits[16];
} COM_PORT;

typedef struct _COM_SETTING{
	COM_PORT SrComPort;
	COM_PORT RsComPort;
	COM_PORT CaComPort;
	COM_PORT StressComPort;
} COM_SETTING;

// 2013.01.11 bagus jog direction -->
// ==========================================================================
// JOG DIRECTION_SETTING
// ==========================================================================
typedef struct _JOG_DIR_SETTING{
	BOOL	bSwapXyDir;
	BOOL	bChangeXSign;
	BOOL	bChangeYSign;
} JOG_DIR_SETTING;
// 2013.01.11 bagus jog direction -->

// ##########################################################################
// System Setting関連
// ##########################################################################

// ==========================================================================
// SYSTEM DATA
// ==========================================================================
typedef struct tagNANOSPEC_SYSTEM_DATA {
	// Common
	SYSTEM_CONFIG			SystemConfig;
	ORIGINAL_POSITION		OriginalPosition;
	STAGE_CONFIG			StageConfig;
	FOCUS_CONFIG			FocusConfig;
	EDGE_ALIGNER			EdgeAligner;
	HANDLER_LOAD_POSITION	HandlerLoadPosition;
	GENERAL_POSITION		GeneralPosition[GENERAL_POSITION_MAX];
	SIGNAL_TOWER			SignalTower;
	HOST_ADDRESS			HostAddress;
	COM_SETTING				ComSetting;
// 2013.01.11 bagus jog direction -->
	JOG_DIR_SETTING			JogDirSetting;
// 2013.01.11 bagus jog direction -->

	// SR
	SR_CONFIG				SrConfig;
	SR_FILTER				SrFilter[SR_FILTER_MAX];
	SR_TURRET				SrTurret[SR_LENS_MAX];
	SR_LENS_CENTER_OFFSET	SrLensCenterOffset;
	SR_XMP					SrXmp;
	SR_VIEW_RANGE			SrViewRange;
	SR_VIDEO_CALIB			SrVideoCalib[SR_LENS_MAX];
	SR_CCD_ARRAY			SrCcdArray[SR_CCD_ARRAY_ITEM_MAX];
/* added 2009.07.22 hmenjo ストレス ライン数 定義 ---------- { ---------- */
	/* Stress	*/
	STRESS_CONFIG			StressConfig;
/* added 2009.07.22 hmenjo ストレス ライン数 定義 ---------- } ---------- */
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
	STRESS_ELASTIC_TABLE	StressElasticTable;
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE 構造体追加(12) ---------- { ---------- */
///* added 2009.08.25 hmenjo SE 構造体追加(１) ---------- { ---------- */
//	/* SE	*/
//	SE_CONFIG				SeConfig;
///* added 2009.08.25 hmenjo SE 構造体追加(１) ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE 構造体追加(12) ---------- } ---------- */
	//2009.09.07 bagus se --{--
	// SE用XMP構造体の追加
	SE_XMP					SeXmp;
	//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	COMPEASE_CONFIG			CompEASEConfig;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
	MS_CONFIG				MsConfig;
// 2009.10.19 bagus MS 追加 --}--
// 2009.10.30 bagus CA 追加 --{--
	CA_CONFIG				CaConfig;
// 2009.10.30 bagus CA 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	RS_CONFIG				RsConfig;
// 2009.11.04 bagus RS 追加 --}--
// 2013.02.22 bagus Substrate thickness setting -->
	SUBSTRATE_THICKNESS_TABLE	SubstrateThicknessTable;
// 2013.02.22 bagus Substrate thickness setting <--
} NANOSPEC_SYSTEM_DATA, *LPNANOSPEC_SYSTEM_DATA;

// ==========================================================================
// SYSTEM COMMON
// ==========================================================================
typedef struct tagSYSTEM_COMMON_DATA {
	SYSTEM_CONFIG			SystemConfig;
	ORIGINAL_POSITION		OriginalPosition;
	STAGE_CONFIG			StageConfig;
	FOCUS_CONFIG			FocusConfig;
	EDGE_ALIGNER			EdgeAligner;
	HANDLER_LOAD_POSITION	HandlerLoadPosition;
	GENERAL_POSITION		GeneralPosition[GENERAL_POSITION_MAX];
	SIGNAL_TOWER			SignalTower;
	HOST_ADDRESS			HostAddress;
} SYSTEM_COMMON_DATA;

// ==========================================================================
// SYSTEM SR
// ==========================================================================
typedef struct tagSYSTEM_SR_DATA {
	SR_CONFIG				Config;
	SR_FILTER				Filter[SR_FILTER_MAX];
	SR_TURRET				Turret[SR_LENS_MAX];
	SR_LENS_CENTER_OFFSET	LensCenterOffset;
	SR_XMP					XMP;
	SR_VIEW_RANGE			ViewRange;
	SR_VIDEO_CALIB			VideoCalib[SR_LENS_MAX];
	SR_CCD_ARRAY			CCDArray[SR_CCD_ARRAY_ITEM_MAX];
} SYSTEM_SR_DATA;

// ###########################################################################
// UserAccountの構造体
// ###########################################################################
typedef struct _USER_ACCOUNT{
	TCHAR szName[USER_NAME_LEN + 1];
	UINT nAccountLevel;
	BOOL bEnablePassword;
	TCHAR szPassword[PASSWORD_LEN + 1];

	_USER_ACCOUNT::_USER_ACCOUNT()
	{::ZeroMemory(this, sizeof(_USER_ACCOUNT));}
} USER_ACCOUNT, *LPUSER_ACCOUNT;

// ==========================================================================
// SitePattern関連
// ==========================================================================
// CPatRecDeskewDlgのモード
#define	DESKEW_MODE		0
#define	SITEPR_MODE		1

// CSitePatternEditDlgのモード
#define SITEPATTERN_MODE_EDIT	0
#define SITEPATTERN_MODE_DEL	1

typedef enum _SITE_PR_MODE
{
	SITE_PR_MODE_NO_PR = 0,												// correct with no pattern recognition
	SITE_PR_MODE_USE_PR,												// correct with use pattern recognition
	SITE_PR_MODE_MAX,
} SITE_PR_MODE;

typedef enum _SITE_PR_FAIL_OPTION
{
	SITE_PR_FAIL_OPTION_FORCE = 0,
	SITE_PR_FAIL_OPTION_CANCEL,
	SITE_PR_FAIL_OPTION_MANUAL,
	SITE_PR_FAIL_OPTION_PRE_FORCE,
	SITE_PR_FAIL_OPTION_PRE_MANUAL,
	SITE_PR_FAIL_OPTION_MAX,
} SITE_PR_FAIL_OPTION;

/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
typedef struct tagSPR_SUB_INFO {
	int		iLens;					/* レンズ設定		*/
	POINT	ptOffset;				/* X/Y オフセット	*/
} SPR_SUB_INFO, *LPSPR_SUB_INFO;
typedef const SPR_SUB_INFO *LPCSPR_SUB_INFO;
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */

typedef struct _PATTERN_INFO{
	BOOL	bDefined;													// site entered (manually) or taught (pat rec)
	TCHAR	szSitePatternName[PATTERN_NAME_LEN + 1];					// パターン名
	int		iLens;														// レンズ
} PATTERN_INFO;

typedef struct _SITE_PATTERN{
	BOOL			bEnable;
	PATTERN_INFO	PatternInfo[2];										// パターン情報
} SITE_PATTERN, *LPSITE_PATTERN;

typedef DESKEW_SITE SITEPT_SITE_MANUAL;

// matsuhisa 20091209 added >>>
enum PR_METHOD
{
	PR_METHOD_NONE = 0,
// 2009.12.03 K.Matsuo -->
	PR_METHOD_MIL8_G,		// GMF(Geometric Model Finder)
	PR_METHOD_MIL8_M,		// PMR(Pattern matching recognition)
// 2009.12.03 K.Matsuo <--
	PR_METHOD_MAX,
};
// matsuhisa 20091209 added <<<<
/* added 2016.08.26 hmenjo ini ファイルログ ---------- { ---------- */
#define  htestLog(htl_bFirst, htl_szLogMsg)	\
{								\
	HANDLE l_htl_hMutex = CreateMutex(0, FALSE, "htestLog");	\
	if (0 != l_htl_hMutex) {	\
		WaitForSingleObject(l_htl_hMutex, INFINITE);	\
	}	\
	\
	char l_htl_szLogFile[MAX_PATH];	\
	sprintf(l_htl_szLogFile, "%s%s", LOG_DIR, "htestLog.log");	\
	if (1 == htl_bFirst) {			\
		/* ログを削除	*/		\
		remove(l_htl_szLogFile);	\
	}												\
	UINT l_htl_uiPos = GetPrivateProfileInt("Ctrl", "Pos", 0, l_htl_szLogFile);	\
	l_htl_uiPos++;	\
	char l_htl_szPos[32];	\
	sprintf(l_htl_szPos, "%d", l_htl_uiPos);	\
	WritePrivateProfileString("Ctrl", "Pos", l_htl_szPos, l_htl_szLogFile);	\
	SYSTEMTIME l_htl_st;	\
	GetLocalTime(&l_htl_st);	\
	char l_htl_szDateTime[32];	\
	sprintf(l_htl_szDateTime, "%04d.%02d.%02d %02d:%02d:%02d.%03d - ", l_htl_st.wYear, l_htl_st.wMonth, l_htl_st.wDay, l_htl_st.wHour, l_htl_st.wMinute, l_htl_st.wSecond, l_htl_st.wMilliseconds);	\
	char l_htl_szLogMsg[256];	\
	sprintf(l_htl_szLogMsg, "%s%s", l_htl_szDateTime, htl_szLogMsg);	\
	char l_htl_szKey[16];	\
	sprintf(l_htl_szKey, "%08d", l_htl_uiPos);	\
	WritePrivateProfileString("Log", l_htl_szKey, l_htl_szLogMsg, l_htl_szLogFile);	\
	\
	if (0 != l_htl_hMutex) {	\
		ReleaseMutex(l_htl_hMutex);	\
		if (2 == htl_bFirst) {	\
			CloseHandle(l_htl_hMutex);	\
		}	\
	}	\
}
/* added 2016.08.26 hmenjo ini ファイルログ ---------- } ---------- */
#endif // _GLOBALS_HXX_
