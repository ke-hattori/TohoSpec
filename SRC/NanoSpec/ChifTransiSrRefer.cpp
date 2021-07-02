// ChifTransiSrRefer.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Active.hxx>
#include "ChifTransiSrRefer.h"
#include "ChiefExports.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "NanoSpecDoc.h"
#include "ChiefView.h"
#include "ChifTransiSeq.h"
#include "..\\..\\INC\\NSStage.hxx"
#include "ChifTransiAF.h"
#include "ChifTransiMaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *	マクロ定義
 */


/*
 *	構造体定義
 */


/*
 *	ローカルグローバル定義
 */
UINT WM_CHIF_TRREF_SETEVENT = RegisterWindowMessage("WM_CHIF_TRREF_SETEVENT");	// トランジションイベント発行用ウィンドウメッセージ
UINT WM_CHIF_TRREF_HEARTBEAT = RegisterWindowMessage("WM_CHIF_TRREF_HEARTBEAT");	// HeartBeat ウィンドウメッセージ


// ##########################################################################
// class CChiefTransiSrRefer : リファレンス測定 トランジション
// ##########################################################################

// =========================================================================//
//	トランジションテーブル
// =========================================================================//
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#if 0 
// const Transition TransTable_SrRefer[] =
// {
// 	//	OldState,				EventId,				NewState
//
// 	{	ST_SREF_INIT,			EV_SREF_DO_INIT,		ST_SREF_INIT			},	// 初期化処理を実行(すぐ EV_SREF_INIT_DONE で)
// 	{	ST_SREF_INIT,			EV_SREF_INIT_DONE,		ST_SREF_IDLE			},	// 初期化完了してアイドルへ
//
// 	{	ST_SREF_IDLE, 			EV_SREF_START,			ST_SREF_N_MOVE			},	// SR リファレンス位置へ移動完了待ち へ
// 																					//		移動処理前にリファレンス測定条件を判定．
// 	{	ST_SREF_N_MOVE, 		EV_SREF_IDLE,			ST_SREF_IDLE			},	// SR リファレンスが，不要
// 	{	ST_SREF_N_MOVE, 		EV_SREF_N_MOVE_FAIL,	ST_SREF_IDLE			},	// SR リファレンス位置へ移動開始失敗
// 	{	ST_SREF_N_MOVE, 		EV_SREF_N_MOVE_DONE,	ST_SREF_N_ZMOVE			},	// SR リファレンス位置へ移動完了，SR リファレンス位置Ｚへ移動完了待ち へ
// /* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- { ---------- */
// 	{	ST_SREF_N_MOVE, 		EV_SREF_N_MEAS_DONE,	ST_SREF_D_MOVE			},	// SR リファレンス測定パスで SR ダーク位置へ移動完了待ち へ
// /* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- } ---------- */
//
// 	{	ST_SREF_N_ZMOVE, 		EV_SREF_N_ZMOVE_FAIL,	ST_SREF_IDLE			},	// SR リファレンス位置Ｚへ移動開始失敗
// 	{	ST_SREF_N_ZMOVE, 		EV_SREF_N_ZMOVE_DONE,	ST_SREF_N_ZMOVE_DONE	},	// SR リファレンス位置Ｚへ移動完了，後処理
// 	{	ST_SREF_N_ZMOVE, 		EV_SREF_IDLE,			ST_SREF_IDLE			},	// キャンセル指令
//
// 	{	ST_SREF_N_ZMOVE_DONE, 	EV_SREF_AF,				ST_SREF_AF				},	// AF 有りで，AF 完了待ち へ
// 	{	ST_SREF_N_ZMOVE_DONE, 	EV_SREF_NOAF,			ST_SREF_N_MEAS			},	// AF 無しで，SR リファレンス測定完了待ち へ
// 	{	ST_SREF_N_ZMOVE_DONE, 	EV_SREF_IDLE,			ST_SREF_IDLE			},	// キャンセル指令
//
// 	{	ST_SREF_AF, 			EV_SREF_AF_DONE,		ST_SREF_AF_DONE			},	// AF 完了
// 	{	ST_SREF_AF, 			EV_SREF_IDLE,			ST_SREF_IDLE			},	// AF トランジションがビジーのため異常終了
// 	{	ST_SREF_AF_DONE, 		EV_SREF_AF_NG,			ST_SREF_IDLE			},	// NG では，完了処理実行して(すぐ EV_SREF_IDLE で)アイドルへ
// 	{	ST_SREF_AF_DONE, 		EV_SREF_AF_OK,			ST_SREF_N_MEAS			},	// OK では，で，SR リファレンス測定完了待ち へ
//
// 	{	ST_SREF_N_MEAS, 		EV_SREF_N_MEAS_ERR,		ST_SREF_IDLE			},	// SR リファレンス測定 異常終了でアイドルへ
// 	{	ST_SREF_N_MEAS, 		EV_SREF_START,			ST_SREF_N_MOVE			},	// SR リファレンス測定 完了，セカンドリファレンス有り
// 	{	ST_SREF_N_MEAS, 		EV_SREF_N_MEAS_DONE,	ST_SREF_D_MOVE			},	// SR リファレンス測定 完了で SR ダーク位置へ移動完了待ち へ
//
// 	{	ST_SREF_D_MOVE, 		EV_SREF_D_MOVE_DONE,	ST_SREF_D_ZMOVE			},	// SR ダーク位置へ移動完了，SR ダーク位置Ｚへ移動完了待ち へ
// 	{	ST_SREF_D_MOVE, 		EV_SREF_D_MOVE_FAIL,	ST_SREF_IDLE			},	// SR ダーク位置へ移動開始失敗
// 	{	ST_SREF_D_MOVE, 		EV_SREF_IDLE,			ST_SREF_IDLE			},	// キャンセル指令
//
// 	{	ST_SREF_D_ZMOVE, 		EV_SREF_D_ZMOVE_DONE,	ST_SREF_D_MEAS			},	// SR ダーク位置へ移動完了，SR ダーク測定完了待ち へ
// 	{	ST_SREF_D_ZMOVE, 		EV_SREF_D_ZMOVE_FAIL,	ST_SREF_IDLE			},	// SR ダーク位置Ｚへ移動開始失敗
// 	{	ST_SREF_D_ZMOVE, 		EV_SREF_IDLE,			ST_SREF_IDLE			},	// キャンセル指令
//
// 	{	ST_SREF_D_MEAS, 		EV_SREF_D_MEAS_ERR,		ST_SREF_IDLE			},	// SR ダーク測定 異常終了でアイドルへ
// 	{	ST_SREF_D_MEAS, 		EV_SREF_D_MEAS_DONE,	ST_SREF_IDLE			},	// SR ダーク測定 完了 アイドルへ
//
// 	// terminate the transition table
// 	END_OF_TRANSITION_TABLE
#else
const Transition TransTable_SrRefer[] =
{
	//	OldState,					EventId,								NewState

	{	ST_SREF_INIT,				EV_SREF_DO_INIT,						ST_SREF_INIT				},	// 初期化処理を実行(すぐ EV_SREF_INIT_DONE で)
	{	ST_SREF_INIT,				EV_SREF_INIT_DONE,						ST_SREF_IDLE				},	// 初期化完了してアイドルへ

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	{	ST_SREF_IDLE, 				EV_SREF_START,							ST_SREF_N_MOVE				},	// SR リファレンス位置へ移動完了待ち へ
	{	ST_SREF_IDLE, 				EV_SREF_START,							ST_SREF_N_PRECHECK_MOVE		},	//
// bagus add 2014.01.10 (Stage None対応) -->
	{	ST_SREF_IDLE,				EV_SREF_STD_START,						ST_SREF_STD_ZMOVE			},	// NanoSpec SR リファレンス位置Ｚへ移動完了待ち へ
	{	ST_SREF_IDLE,				EV_SREF_DRK_START,						ST_SREF_DRK_ZMOVE			},	// NanoSpec SR ダークリファレンス位置Ｚへ移動完了待ち へ
// bagus add 2014.01.10 (Stage None対応) <--
	{	ST_SREF_N_PRECHECK_MOVE,	EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM,		ST_SREF_N_MOVE				},	// NanoSpec
	{	ST_SREF_N_PRECHECK_MOVE,	EV_SREF_N_SIMULATION,					ST_SREF_N_MEAS				},	// NanoSpec
	{	ST_SREF_N_PRECHECK_MOVE,	EV_SREF_IDLE,							ST_SREF_IDLE				},	// NanoSpec
	{	ST_SREF_N_PRECHECK_MOVE,	EV_SREF_N_PRECHECK_NO_REF,				ST_SREF_NO_REF				},	// TohoSpec(Desktop type)
	{	ST_SREF_N_PRECHECK_MOVE,	EV_SREF_N_PRECHECK_NEED_CONFIRM_EXPIRE,	ST_SREF_N_PRE_CONFIRM_EXPIRE},	// TohoSpec(Desktop type)
	{	ST_SREF_N_PRECHECK_MOVE,	EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE,	ST_SREF_N_PRE_CONFIRM_MOVE	},	// TohoSpec(Desktop type)
	{	ST_SREF_NO_REF,				EV_SREF_N_SHOW_INFO_DLG_DONE,			ST_SREF_NO_REF_DONE			},	// TohoSpec(Desktop type)
	{	ST_SREF_NO_REF_DONE,		EV_SREF_IDLE,							ST_SREF_IDLE				},	// TohoSpec(Desktop type)
	{	ST_SREF_N_PRE_CONFIRM_EXPIRE,EV_SREF_N_SHOW_INFO_DLG_DONE,			ST_SREF_N_CONFIRM_EXPIRE	},	// TohoSpec(Desktop type)
	{	ST_SREF_N_CONFIRM_EXPIRE,	EV_SREF_N_CONFIRM_EXPIRE_YES,			ST_SREF_N_EXPIRE_YES		},	// TohoSpec(Desktop type)
	{	ST_SREF_N_CONFIRM_EXPIRE,	EV_SREF_N_CONFIRM_EXPIRE_NO,			ST_SREF_N_EXPIRE_NO			},	// TohoSpec(Desktop type)
	{	ST_SREF_N_EXPIRE_YES,		EV_SREF_N_CONFIRM_NEXT,					ST_SREF_N_CONFIRM_MOVE		},	// TohoSpec(Desktop type)
	{	ST_SREF_N_EXPIRE_NO,		EV_SREF_IDLE,							ST_SREF_IDLE				},	// TohoSpec(Desktop type)
	{	ST_SREF_N_PRE_CONFIRM_MOVE,	EV_SREF_N_SHOW_INFO_DLG_DONE,			ST_SREF_N_CONFIRM_MOVE		},	// TohoSpec(Desktop type)
	{	ST_SREF_N_CONFIRM_MOVE,		EV_SREF_N_CONFIRM_MOVE_OK,				ST_SREF_N_MOVE_OK			},	// TohoSpec(Desktop type)
	{	ST_SREF_N_CONFIRM_MOVE,		EV_SREF_N_CONFIRM_MOVE_CANCEL,			ST_SREF_N_MOVE_CANCEL		},	// TohoSpec(Desktop type)
	{	ST_SREF_N_MOVE_OK,			EV_SREF_N_CONFIRM_NEXT,					ST_SREF_N_MEAS				},	// TohoSpec(Desktop type)
	{	ST_SREF_N_MOVE_CANCEL,		EV_SREF_IDLE,							ST_SREF_IDLE				},	// TohoSpec(Desktop type)
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
																												//		移動処理前にリファレンス測定条件を判定．
	{	ST_SREF_N_MOVE, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// SR リファレンスが，不要
	{	ST_SREF_N_MOVE, 			EV_SREF_N_MOVE_FAIL,					ST_SREF_IDLE				},	// SR リファレンス位置へ移動開始失敗
	{	ST_SREF_N_MOVE, 			EV_SREF_N_MOVE_DONE,					ST_SREF_N_ZMOVE				},	// SR リファレンス位置へ移動完了，SR リファレンス位置Ｚへ移動完了待ち へ
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
// /* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- { ---------- */
// 	{	ST_SREF_N_MOVE, 			EV_SREF_N_MEAS_DONE,					ST_SREF_D_MOVE				},	// SR リファレンス測定パスで SR ダーク位置へ移動完了待ち へ
// /* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- } ---------- */
	{	ST_SREF_N_MOVE, 			EV_SREF_N_MEAS_DONE,					ST_SREF_D_PRECHECK_MOVE		},	// SR リファレンス測定パスで SR ダーク位置へ移動完了待ち へ
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	{	ST_SREF_N_ZMOVE, 			EV_SREF_N_ZMOVE_FAIL,					ST_SREF_IDLE				},	// SR リファレンス位置Ｚへ移動開始失敗
	{	ST_SREF_N_ZMOVE, 			EV_SREF_N_ZMOVE_DONE,					ST_SREF_N_ZMOVE_DONE		},	// SR リファレンス位置Ｚへ移動完了，後処理
	{	ST_SREF_N_ZMOVE, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_N_ZMOVE_DONE, 		EV_SREF_AF,								ST_SREF_AF					},	// AF 有りで，AF 完了待ち へ
	{	ST_SREF_N_ZMOVE_DONE, 		EV_SREF_NOAF,							ST_SREF_N_MEAS				},	// AF 無しで，SR リファレンス測定完了待ち へ
	{	ST_SREF_N_ZMOVE_DONE, 		EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_AF, 				EV_SREF_AF_DONE,						ST_SREF_AF_DONE				},	// AF 完了
	{	ST_SREF_AF, 				EV_SREF_IDLE,							ST_SREF_IDLE				},	// AF トランジションがビジーのため異常終了
	{	ST_SREF_AF_DONE, 			EV_SREF_AF_NG,							ST_SREF_IDLE				},	// NG では，完了処理実行して(すぐ EV_SREF_IDLE で)アイドルへ
	{	ST_SREF_AF_DONE, 			EV_SREF_AF_OK,							ST_SREF_N_MEAS				},	// OK では，で，SR リファレンス測定完了待ち へ

	{	ST_SREF_N_MEAS, 			EV_SREF_N_MEAS_ERR,						ST_SREF_IDLE				},	// SR リファレンス測定 異常終了でアイドルへ
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	{	ST_SREF_N_MEAS, 			EV_SREF_START,							ST_SREF_N_MOVE				},	// SR リファレンス測定 完了，セカンドリファレンス有り
//	{	ST_SREF_N_MEAS, 			EV_SREF_N_MEAS_DONE,					ST_SREF_D_MOVE				},	// SR リファレンス測定 完了で SR ダーク位置へ移動完了待ち へ
	{	ST_SREF_N_MEAS, 			EV_SREF_START,							ST_SREF_N_PRECHECK_MOVE		},	// SR リファレンス測定 完了，セカンドリファレンス有り
	{	ST_SREF_N_MEAS, 			EV_SREF_N_MEAS_DONE,					ST_SREF_D_PRECHECK_MOVE		},	// SR リファレンス測定 完了で SR ダーク位置へ移動完了待ち へ
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	{	ST_SREF_D_PRECHECK_MOVE,	EV_SREF_D_PRECHECK_MOVE_NO_CONFIRM,		ST_SREF_D_MOVE				},	// NanoSpec
	{	ST_SREF_D_PRECHECK_MOVE,	EV_SREF_D_SIMULATION,					ST_SREF_D_MEAS				},	// NanoSpec
	{	ST_SREF_D_PRECHECK_MOVE,	EV_SREF_D_PRECHECK_MOVE_CONFIRM_NEED,	ST_SREF_D_CONFIRM_MOVE		},	// TohoSpec(Desktop type)
	{	ST_SREF_D_CONFIRM_MOVE,		EV_SREF_D_CONFIRM_MOVE_OK,				ST_SREF_D_MOVE_OK			},	// TohoSpec(Desktop type)
	{	ST_SREF_D_CONFIRM_MOVE,		EV_SREF_D_CONFIRM_MOVE_CANCEL,			ST_SREF_D_MOVE_CANCEL		},	// TohoSpec(Desktop type)
	{	ST_SREF_D_MOVE_OK,			EV_SREF_D_CONFIRM_NEXT,					ST_SREF_D_MEAS				},	// TohoSpec(Desktop type)
	{	ST_SREF_D_MOVE_CANCEL,		EV_SREF_IDLE,							ST_SREF_IDLE				},	// TohoSpec(Desktop type)
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	{	ST_SREF_D_MOVE, 			EV_SREF_D_MOVE_DONE,					ST_SREF_D_ZMOVE				},	// SR ダーク位置へ移動完了，SR ダーク位置Ｚへ移動完了待ち へ
	{	ST_SREF_D_MOVE, 			EV_SREF_D_MOVE_FAIL,					ST_SREF_IDLE				},	// SR ダーク位置へ移動開始失敗
	{	ST_SREF_D_MOVE, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_D_ZMOVE, 			EV_SREF_D_ZMOVE_DONE,					ST_SREF_D_MEAS				},	// SR ダーク位置へ移動完了，SR ダーク測定完了待ち へ
	{	ST_SREF_D_ZMOVE, 			EV_SREF_D_ZMOVE_FAIL,					ST_SREF_IDLE				},	// SR ダーク位置Ｚへ移動開始失敗
	{	ST_SREF_D_ZMOVE, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_D_MEAS, 			EV_SREF_D_MEAS_ERR,						ST_SREF_IDLE				},	// SR ダーク測定 異常終了でアイドルへ
	{	ST_SREF_D_MEAS, 			EV_SREF_D_MEAS_DONE,					ST_SREF_IDLE				},	// SR ダーク測定 完了 アイドルへ

// bagus add 2014.01.10 (Stage None対応) -->
	{	ST_SREF_STD_ZMOVE, 			EV_SREF_STD_SIMULATION,					ST_SREF_STD_MEAS			},	//
	{	ST_SREF_STD_ZMOVE, 			EV_SREF_STD_ZMOVE_FAIL,					ST_SREF_IDLE				},	// SR リファレンス位置Ｚへ移動開始失敗
	{	ST_SREF_STD_ZMOVE, 			EV_SREF_STD_ZMOVE_DONE,					ST_SREF_STD_ZMOVE_DONE		},	// SR リファレンス位置Ｚへ移動完了，後処理
	{	ST_SREF_STD_ZMOVE, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_STD_ZMOVE_DONE, 	EV_SREF_STD_AF,							ST_SREF_STD_AF				},	// AF 有りで，AF 完了待ち へ
	{	ST_SREF_STD_ZMOVE_DONE, 	EV_SREF_STD_NOAF,						ST_SREF_STD_MEAS			},	// AF 無しで，SR リファレンス測定完了待ち へ
	{	ST_SREF_STD_ZMOVE_DONE, 	EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_STD_AF, 			EV_SREF_STD_AF_DONE,					ST_SREF_STD_AF_DONE			},	// AF 完了
	{	ST_SREF_STD_AF, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// AF トランジションがビジーのため異常終了
	{	ST_SREF_STD_AF_DONE, 		EV_SREF_STD_AF_NG,						ST_SREF_IDLE				},	// NG では，完了処理実行して(すぐ EV_SREF_IDLE で)アイドルへ
	{	ST_SREF_STD_AF_DONE, 		EV_SREF_STD_AF_OK,						ST_SREF_STD_MEAS			},	// OK では，で，SR リファレンス測定完了待ち へ

	{	ST_SREF_STD_MEAS, 			EV_SREF_STD_MEAS_ERR,					ST_SREF_IDLE				},	// SR リファレンス測定 異常終了でアイドルへ
	{	ST_SREF_STD_MEAS, 			EV_SREF_STD_MEAS_DONE,					ST_SREF_IDLE				},	// SR リファレンス測定 完了 アイドルへ

	{	ST_SREF_DRK_ZMOVE, 			EV_SREF_DRK_SIMULATION,					ST_SREF_DRK_MEAS			},	//
	{	ST_SREF_DRK_ZMOVE, 			EV_SREF_DRK_ZMOVE_DONE,					ST_SREF_DRK_MEAS			},	// SR ダーク位置へ移動完了，SR ダーク測定完了待ち へ
	{	ST_SREF_DRK_ZMOVE, 			EV_SREF_DRK_ZMOVE_FAIL,					ST_SREF_IDLE				},	// SR ダーク位置Ｚへ移動開始失敗
	{	ST_SREF_DRK_ZMOVE, 			EV_SREF_IDLE,							ST_SREF_IDLE				},	// キャンセル指令

	{	ST_SREF_DRK_MEAS, 			EV_SREF_DRK_MEAS_ERR,					ST_SREF_IDLE				},	// SR ダーク測定 異常終了でアイドルへ
	{	ST_SREF_DRK_MEAS, 			EV_SREF_DRK_MEAS_DONE,					ST_SREF_IDLE				},	// SR ダーク測定 完了 アイドルへ
// bagus add 2014.01.10 (Stage None対応) <--

	// terminate the transition table
	END_OF_TRANSITION_TABLE
};
#endif
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

// =========================================================================//
//	状態別処理関数テーブル
// =========================================================================//
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#if 0
// BEGIN_STATEPROC_MAP(CChiefTransiSrRefer)
// 	/*	0 */	ENTER_STATE(ST_SREF_INIT,			enter_ST_SREF_INIT),
// 	/*	1 */	ENTER_STATE(ST_SREF_IDLE,			enter_ST_SREF_IDLE),
// 	/*	2 */	ENTER_STATE(ST_SREF_N_MOVE,			enter_ST_SREF_N_MOVE),				// (WAIT 状態無しの場合有り)
// 	/*	3 */	ENTER_STATE(ST_SREF_N_ZMOVE,		enter_ST_SREF_N_ZMOVE),				// (WAIT 状態無し)
// 	/*	4 */	ENTER_STATE(ST_SREF_N_ZMOVE_DONE,	enter_ST_SREF_N_ZMOVE_DONE),		// (WAIT 状態無し)
// 	/*	5 */	ENTER_STATE(ST_SREF_AF,				enter_ST_SREF_AF),					// (WAIT 状態無しの場合有り)
// 	/*	6 */	ENTER_STATE(ST_SREF_AF_DONE,		enter_ST_SREF_AF_DONE),				// (WAIT 状態無し)
// 	/*	7 */	ENTER_STATE(ST_SREF_N_MEAS,			enter_ST_SREF_N_MEAS),				// (WAIT 状態無し)
// 	/*	8 */	ENTER_STATE(ST_SREF_D_MOVE,			enter_ST_SREF_D_MOVE),				// (WAIT 状態無しの場合有り)
// 	/*	9 */	ENTER_STATE(ST_SREF_D_ZMOVE,		enter_ST_SREF_D_ZMOVE),				// (WAIT 状態無し)
// 	/* 10 */	ENTER_STATE(ST_SREF_D_MEAS,			enter_ST_SREF_D_MEAS),				// (WAIT 状態無し)
//
// END_STATEPROC_MAP(CChiefTransiSrRefer)
#else
BEGIN_STATEPROC_MAP(CChiefTransiSrRefer)
	/*	0 */	ENTER_STATE(ST_SREF_INIT,				enter_ST_SREF_INIT),
	/*	1 */	ENTER_STATE(ST_SREF_IDLE,				enter_ST_SREF_IDLE),
	/*	2 */	ENTER_STATE(ST_SREF_N_PRECHECK_MOVE,	enter_ST_SREF_N_PRECHECK_MOVE),		//
	/*	3 */	ENTER_STATE(ST_SREF_NO_REF,				enter_ST_SREF_NO_REF),				//
	/*	4 */	ENTER_STATE(ST_SREF_NO_REF_DONE,		enter_ST_SREF_NO_REF_DONE),			//
	/*	5 */	ENTER_STATE(ST_SREF_N_PRE_CONFIRM_EXPIRE,enter_ST_SREF_N_PRE_CONFIRM_EXPIRE),	//
	/*	6 */	ENTER_STATE(ST_SREF_N_CONFIRM_EXPIRE,	enter_ST_SREF_N_CONFIRM_EXPIRE),	//
	/*	7 */	ENTER_STATE(ST_SREF_N_EXPIRE_YES,		enter_ST_SREF_N_EXPIRE_YES),		//
	/*	8 */	ENTER_STATE(ST_SREF_N_EXPIRE_NO,		enter_ST_SREF_N_EXPIRE_NO),			//
	/*	9 */	ENTER_STATE(ST_SREF_N_PRE_CONFIRM_MOVE,	enter_ST_SREF_N_PRE_CONFIRM_MOVE),	//
	/* 10 */	ENTER_STATE(ST_SREF_N_CONFIRM_MOVE,		enter_ST_SREF_N_CONFIRM_MOVE),		//
	/* 11 */	ENTER_STATE(ST_SREF_N_MOVE_OK,			enter_ST_SREF_N_MOVE_OK),			//
	/* 12 */	ENTER_STATE(ST_SREF_N_MOVE_CANCEL,		enter_ST_SREF_N_MOVE_CANCEL),		//
	/* 13 */	ENTER_STATE(ST_SREF_N_MOVE,				enter_ST_SREF_N_MOVE),				// (WAIT 状態無しの場合有り)
	/* 14 */	ENTER_STATE(ST_SREF_N_ZMOVE,			enter_ST_SREF_N_ZMOVE),				// (WAIT 状態無し)
	/* 15 */	ENTER_STATE(ST_SREF_N_ZMOVE_DONE,		enter_ST_SREF_N_ZMOVE_DONE),		// (WAIT 状態無し)
	/* 16 */	ENTER_STATE(ST_SREF_AF,					enter_ST_SREF_AF),					// (WAIT 状態無しの場合有り)
	/* 17 */	ENTER_STATE(ST_SREF_AF_DONE,			enter_ST_SREF_AF_DONE),				// (WAIT 状態無し)
	/* 18 */	ENTER_STATE(ST_SREF_N_MEAS,				enter_ST_SREF_N_MEAS),				// (WAIT 状態無し)
	/* 19 */	ENTER_STATE(ST_SREF_D_PRECHECK_MOVE,	enter_ST_SREF_D_PRECHECK_MOVE),		//
	/* 20 */	ENTER_STATE(ST_SREF_D_CONFIRM_MOVE,		enter_ST_SREF_D_CONFIRM_MOVE),		//
	/* 21 */	ENTER_STATE(ST_SREF_D_MOVE_OK,			enter_ST_SREF_D_MOVE_OK),			//
	/* 22 */	ENTER_STATE(ST_SREF_D_MOVE_CANCEL,		enter_ST_SREF_D_MOVE_CANCEL),		//
	/* 23 */	ENTER_STATE(ST_SREF_D_MOVE,				enter_ST_SREF_D_MOVE),				// (WAIT 状態無しの場合有り)
	/* 24 */	ENTER_STATE(ST_SREF_D_ZMOVE,			enter_ST_SREF_D_ZMOVE),				// (WAIT 状態無し)
	/* 25 */	ENTER_STATE(ST_SREF_D_MEAS,				enter_ST_SREF_D_MEAS),				// (WAIT 状態無し)
// bagus add 2014.01.10 (Stage None対応) -->
	/* 26 */	ENTER_STATE(ST_SREF_STD_ZMOVE,			enter_ST_SREF_STD_ZMOVE),			// (WAIT 状態無し)
	/* 27 */	ENTER_STATE(ST_SREF_STD_ZMOVE_DONE,		enter_ST_SREF_STD_ZMOVE_DONE),		// (WAIT 状態無し)
	/* 28 */	ENTER_STATE(ST_SREF_STD_AF,				enter_ST_SREF_STD_AF),				// (WAIT 状態無しの場合有り)
	/* 29 */	ENTER_STATE(ST_SREF_STD_AF_DONE,		enter_ST_SREF_STD_AF_DONE),			// (WAIT 状態無し)
	/* 30 */	ENTER_STATE(ST_SREF_STD_MEAS,			enter_ST_SREF_STD_MEAS),			// (WAIT 状態無し)
	/* 31 */	ENTER_STATE(ST_SREF_DRK_ZMOVE,			enter_ST_SREF_DRK_ZMOVE),			// (WAIT 状態無し)
	/* 32 */	ENTER_STATE(ST_SREF_DRK_MEAS,			enter_ST_SREF_DRK_MEAS),			// (WAIT 状態無し)
// bagus add 2014.01.10 (Stage None対応) <--

END_STATEPROC_MAP(CChiefTransiSrRefer)
#endif
// 2013.11.07 Bagus Mod (TohoSpec対応) <--


/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSrRefer

IMPLEMENT_DYNCREATE(CChiefTransiSrRefer, CWinThread)

CChiefTransiSrRefer::CChiefTransiSrRefer(CWnd* pParent /*=NULL*/) : cActive(TransTable_SrRefer)
{
	TRACE(_T("CChiefTransiSrRefer::CChiefTransiSrRefer() \n"));

	m_pcChiefView = (CView*) pParent;

	//	スレッドの起動待ち
	BOOL l_bThreadOK = CreateThread();
	if (0 != this->m_hThread) {
		CSingleLock	l_SingleLock(&m_cSyncEvent,TRUE);
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Started  CChiefTransiSrRefer."));

	//	オブジェクトの自動破棄を禁止
	m_bAutoDelete = FALSE;

}

CChiefTransiSrRefer::~CChiefTransiSrRefer()
{
	TRACE(_T("CChiefTransiSrRefer::~CChiefTransiSrRefer() \n"));

	//	スレッドの消滅待ち
	if (0 != this->m_hThread) {
		if (0 == m_bAutoDelete) {
			PostThreadMessage(WM_QUIT, 0, 0);
			::WaitForSingleObject(m_hThread, INFINITE);
			((CChiefView*) m_pcChiefView)->LogChief(_T("Ended    CChiefTransiSrRefer."));
		}
	}
}

BOOL CChiefTransiSrRefer::InitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiSrRefer::InitInstance() \n"));

	// コンストラクタの待ちを解除します
	//		つまり，スレッドの生成とスレッドクラスの構築の同期を取っています．
	m_cSyncEvent.SetEvent();

	// 初期状態処理用イベント発行
	TransiEvent(0, 0);

	// 変数初期化
	m_dwHeartBeat = 0;

	return TRUE;
}

int CChiefTransiSrRefer::ExitInstance()
{
	// TODO: この位置にスレッド単位の初期化コードを追加してください。

	TRACE(_T("CChiefTransiSrRefer::ExitInstance() \n"));

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CChiefTransiSrRefer, CWinThread)
	//{{AFX_MSG_MAP(CChiefTransiSrRefer)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加します。
	//}}AFX_MSG_MAP
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRREF_SETEVENT, OnTransiEvent)
	ON_REGISTERED_THREAD_MESSAGE(WM_CHIF_TRREF_HEARTBEAT, OnHeartBeat)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiSrRefer メッセージ ハンドラ

/*
 *	トランジションイベント発行 メッセージハンドラ
 */
void CChiefTransiSrRefer::OnTransiEvent(WPARAM wparam, LPARAM lparam)
{
	::Sleep(TRANSI_TIME);	// トランジション間タイミング調整(とりあえずだけど恒久かも・・)

	Event((int) wparam, (cEventParams*) lparam);

}

/*
 *	HeartBeat メッセージハンドラ
 */
void CChiefTransiSrRefer::OnHeartBeat(WPARAM wparam, LPARAM lparam)
{
	m_dwHeartBeat++;
}


/****************************************************************************
	その他関数
****************************************************************************/

/*
 *	トランジションイベント発行用関数
 */
void CChiefTransiSrRefer::TransiEvent(const int iEventNo, const cEventParams *const pEvParams)
{
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	PostThreadMessage(WM_CHIF_TRREF_SETEVENT, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	((CChiefView*) m_pcChiefView)->PostTransiEvent(this, WM_CHIF_TRREF_SETEVENT, _T("WM_CHIF_TRREF_SETEVENT"), 8, (WPARAM) iEventNo, (LPARAM) pEvParams);
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
}

/*
 *	HeartBeat 発行用関数
 */
void CChiefTransiSrRefer::HeartBeatPulse(void)
{
	PostThreadMessage(WM_CHIF_TRREF_HEARTBEAT, 0, 0);
}

/*
 *	現在の状態が「アイドル」かどうかをチェック
 *		戻り値：	true	アイドルである
 *					false	アイドルでない
 */
bool CChiefTransiSrRefer::IsIdle()
{
	bool	l_isRc;

	if (CurrState == ST_SREF_IDLE) {
		l_isRc = true;
	} else {
		l_isRc = false;
	}

	return l_isRc;
}

/*
 *	現在の状態を取得
 *		戻り値：	現在の状態
 */
int CChiefTransiSrRefer::GetCurrentState()
{
	return CurrState;
}

/*
 *	呼出元トランジションに完了を通知
 */
void CChiefTransiSrRefer::NotifyComplete(const cEventParams *const pEvParams)
{
/* added 2009.11.24 hmenjo 自動測定終了時は SR フィルタ OPEN ---------- { ---------- */
	MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN);
/* added 2009.11.24 hmenjo 自動測定終了時は SR フィルタ OPEN ---------- } ---------- */
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	/* 以前のヘッドに戻します．	*/
	((CChiefView*) m_pcChiefView)->SelectHead(m_wHeadPre, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
	switch (m_dwParentTransi) {
	case TRANSI_AF:
	case TRANSI_SR_REFER:
	case TRANSI_DESKEW:
	case TRANSI_SR_1POINT:
// bagus add 2014.01.10 (Stage None対応) -->
	case TRANSI_SR_STD_REFER:
// bagus add 2014.01.10 (Stage None対応) <--
	default:
		// 何もしません．
		break;
	case TRANSI_MASTER:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_UISREF_DONE, pEvParams);
		break;
// bagus add 2014.01.10 (Stage None対応) -->
	case TRANSI_MASTER_STD:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_STDREF_DONE, pEvParams);
		break;
	case TRANSI_MASTER_DRK:
		((CChiefTransiMaster*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiMaster)->TransiEvent(EV_MAS_DRKREF_DONE, pEvParams);
		break;
// bagus add 2014.01.10 (Stage None対応) <--
	case TRANSI_SEQ:
		((CChiefTransiSeq*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiSeq)->TransiEvent(EV_SEQ_REFER_DONE, pEvParams);
		break;
	}
}

/*
 *	SR リファレンス測定 トランジションをキャンセル終了させます．
 */
void CChiefTransiSrRefer::CancelSrRefer()
{
	m_bCancelReq = TRUE;
	// AF トランジションをキャンセル終了させます．
	if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->CancelAF();
	}
}

//--------------------------------------------------------------------------//
//	状態別処理関数群
//--------------------------------------------------------------------------//
/*
 *	初期状態
 */
void CChiefTransiSrRefer::enter_ST_SREF_INIT(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[0], (DWORD) pEvParams);

	// 初期化処理はありません．
}

/*
 *	アイドル
 */
void CChiefTransiSrRefer::enter_ST_SREF_IDLE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[1], (DWORD) pEvParams);

	m_bCancelReq = FALSE;
	// WAIT 状態にするため処理はありません．
}

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#if 0
// /*
//  *	SR リファレンス位置へ移動完了待ち(WAIT 状態無しの場合有り)
//  */
// void CChiefTransiSrRefer::enter_ST_SREF_N_MOVE(const cEventParams* const pEvParams)
// /* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
// /*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
// /* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
// {
// 	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[2], (DWORD) pEvParams);
//
// 	// 初回呼出かどうか判定 ---------------------------------------------------
// 	if (0 != pEvParams) {
// 		// 初回呼出し -------------------------------------
// 		// 呼出元トランジションを保存
// /* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
// //		m_dwParentTransi = (DWORD) pEvParams;
// /* modified 2009.09.09 hmenjo 引数変更 ----------			   */
// 		m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
// /* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
// 		// 複数回測定用カウンタを１にリセット
// 		m_dwRefCounter = 1;
// /* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
// 		/* SR ヘッドにします．	*/
// 		m_wHeadPre = NS_GetCurrentHeadType();
// 		((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
// /* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
// // 2013.02.22 bagus Substrate thickness setting -->
// 		if (((CChiefView*) m_pcChiefView)->SelectSubstrateThickIndex(0)) {	// 板厚設定には、リファレンスを指定する
// 			((CChiefView*) m_pcChiefView)->LogChief(_T("Select Reference Sub Thickness failed."));
// 		}
// // 2013.02.22 bagus Substrate thickness setting <--
// 	} else {
// 		// 複数回呼出し -----------------------------------
// 		// 複数回測定用カウンタ＋１
// 		m_dwRefCounter++;
// 	}
//
// /* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
// 	LPMAIN_RCP_INFO		l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
// 	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// /* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
//
// /* added 2009.10.29 hmenjo CTA リファレンス無し ---------- { ---------- */
// 	/* SE，CTA の場合はリファレンス不要	*/
// 	if ((HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
// 	 || (HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
// 	 || (HEAD_TYPE_SE == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) 
// // 2013.02.01 bagus CompleteEASEヘッド追加 -->
// 	 || (HEAD_TYPE_COMPEASE == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) ) {
// // 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 		TransiEvent(EV_SREF_IDLE);	/* トランジション更新	*/
// 		NotifyComplete((cEventParams*) TR_SREF_OK);	/* 呼出元トランジションに完了を通知	*/
// 		return;
// 	}
// /* added 2009.10.29 hmenjo CTA リファレンス無し ---------- } ---------- */
//
// 	// 2013.02.25 bagus -->
// 	// 測定開始前にシャッターを閉じる処理を追加。
// 	// CompleteEASE PC側で手動シャッター開されたまま、SR測定開始された場合を考慮する。
// 	if (((CMainFrame*)(((CChiefView*) m_pcChiefView)->m_pcMainFrame))->m_SystemConfig.HeadType.bCompEASE) {
// 		MEAS_CompEASEHead_CloseLampShutter();
// 	}
// 	// 2013.02.25 bagus <--
//
// 	// ハードシミュレートモード判定 -------------------------------------------
// 	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
// 		// ハードシミュレートモードだったので，正常終了とします．
// 		// 2008.12.30 matsuo add ----->
// /* modified 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- { ---------- */
// //		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 1st (Normal) Reference."));
// //		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// //		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_N_1ST_MEASING);	// Status Bar
// //		MEAS_1stReference(l_pMeasProgInfo);
// //#ifdef CHIEF_USE_SLEEP
// //		::Sleep(1000);
// //#endif
// /* modified 2009.09.09 hmenjo 透過率 ガントリでダークのみ ----------			  */
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
// //		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
// 		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 		 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G != l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
// 			/* 通常透過率のみ	*/
// 			((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 1st (Normal) Reference."));
// 			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_N_1ST_MEASING);	// Status Bar
// 			MEAS_1stReference(l_pMeasProgInfo);
// #ifdef CHIEF_USE_SLEEP
// 			::Sleep(1000);
// #endif
// 		}
// /* modified 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- } ---------- */
// 		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring Dark T1 Reference."));
// 		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_D_T1_MEASING);	// Status Bar
// 		MEAS_Dark_T1(l_pMeasProgInfo);
// 		TCHAR	l_szMainRcpName[256];
// 		((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// 		MEAS_WriteGTrRefFile(l_szMainRcpName);
// 		// 2008.12.30 matsuo add <-----
// #ifdef CHIEF_USE_SLEEP
// 		::Sleep(5000);
// #endif
// #ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
// 		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_D_T1_COMPLETE);	// Status Bar
// #else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
// 		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
// #endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
// 		// トランジション更新
// 		TransiEvent(EV_SREF_IDLE);
// 		// 呼出元トランジションに完了を通知
// 		NotifyComplete((cEventParams*) TR_SREF_OK);
// 		return;
// 	}
//
// 	// 以降は実機の場合です ---------------------------------------------------
// 	BOOL l_bDoRefMeas = FALSE;
// 	TCHAR	l_szMainRcpName[256];
// 	if (0 != pEvParams) {
// 		// リファレンス設定を取得 ---------------------------------------------
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
// //		LPMAIN_RCP_INFO		l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
// //		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
// 		m_bMeasRef = l_pMainRcpInfo->MainRcpParam._SR.bMeasRef;		// 更新フラグ(TRUE:毎回更新，FALSE:有効期限チェックによる)
// 		m_bWarnFlag = l_pMeasProgInfo->Ref.hdr.bWarnFlag;			// 有効期限チェックフラグ(0:チェックしない，1:チェックする)
// 		m_dLifeTime = l_pMeasProgInfo->Ref.hdr.dLifeTime;			// リファレンス更新間隔[ms](有効期限)
// 		m_bRef2ndMeasure = l_pMeasProgInfo->Ref2nd.bMeasure;		// セカンドリファレンス測定 要/不要
// 		m_wScanType = l_pMeasProgInfo->ScanParams.hdr.wScanType;	// 測定項目
// /* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- { ---------- */
// 		if (
// 			(HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 		 && (
// 				(MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType)
// 			 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
// 				)
// 			) {
// 			/* 透過率の場合はセカンドリファレンスは無しにします．	*/
// 			m_bRef2ndMeasure = FALSE;
// 		}
// /* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- } ---------- */
// 		// 有効期限を確認 -----------------------------------------------------
// 		// カレントメインレシピ名を取得
// 		((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// /* modified 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- { ---------- */
// //		BOOL l_bExpire = FALSE;
// //		if (0 == m_bMeasRef) {
// //			// 有効期限外で更新の設定だった
// //			if (0 != MEAS_CheckRefFileElapsedTimeOut(l_szMainRcpName, m_dLifeTime)) {
// //				// 有効期限切れだった
// //				l_bExpire = TRUE;
// //			}
// //		}
// //		// 設定からリファレンスの取得が必要かどうか判定 -----------------------
// //		if ((0 != m_bMeasRef) || ((0 != m_bWarnFlag) && (0 != l_bExpire))) {
// //			// 必要
// //			//		(「毎回更新」か，「有効期限チェックする」で「有効期限切れ」の場合)
// //			l_bDoRefMeas = TRUE;
// //		} else {
// //			// 不要
// //			// さらにリファレンスデータファイルの有無で必要かどうかを判定します．
// //			if (0 != m_bRef2ndMeasure) {
// //				// セカンドリファレンス必要
// //				if ((0 == MEAS_ReadRefFile2ndRefT1(l_szMainRcpName))
// //				 || (0 == MEAS_ReadRefFile2ndRefT2(l_szMainRcpName))) {
// //					// データファイル(「セカンド リファレンス T1 データ」と「セカンド リファレンス T2 データ」)が無かった
// //					l_bDoRefMeas = TRUE;
// //				} else {
// //					// データファイルが有った
// //					l_bDoRefMeas = FALSE;
// //				}
// //			} else {
// //				// セカンドリファレンス不要
// //				if (0 == MEAS_ReadRefFile(l_szMainRcpName)) {
// //					// データファイル(「ファースト リファレンスデータ」)が無かった
// //					l_bDoRefMeas = TRUE;
// //				} else {
// //					// データファイルが有った
// //					l_bDoRefMeas = FALSE;
// //				}
// //			}
// //		}
// // 2009.10.09 K.Matsuo -->
// 		if (0 != m_bMeasRef) {
// 			// 必要
// 			l_bDoRefMeas = TRUE;
// 		} else {
// // 2009.10.09 K.Matsuo <--
// /* modified 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- 			 */
// 			int l_iRefData = ((CChiefView*) m_pcChiefView)->CheckReferenceData(l_szMainRcpName, m_dLifeTime, m_bRef2ndMeasure);
// 			if (
// /* modified 2009.11.11 hmenjo Refer 期限チェックミス修正 ---------- { ---------- */
// //				((1 == l_iRefData) && (0 != m_bWarnFlag))	/* 有効期限切れで，有効期限切れチェックする設定の場合	*/
// //			 || (2 <= l_iRefData)	/* ファイルが存在しない	*/
// /* modified 2009.11.11 hmenjo Refer 期限チェックミス修正 ---------- 			 */
// 				((2 == l_iRefData) && (0 != m_bWarnFlag))	/* 有効期限切れで，有効期限切れチェックする設定の場合	*/
// 			 || (1 == l_iRefData)	/* ファイルが存在しない	*/
// 			 || (2 < l_iRefData)	/* その他の異常	*/
// /* modified 2009.11.11 hmenjo Refer 期限チェックミス修正 ---------- } ---------- */
// 				) {
// 				l_bDoRefMeas = TRUE;
// 			}
// 		}
// /* modified 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- } ---------- */
// 	}
//
// /* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
// 	if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
// 		/* ガントリ透過率では設定によらず必ず実施します．	*/
// 		l_bDoRefMeas = TRUE;
// 	}
// /* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
//
// 	// リファレンス測定位置への移動処理 ---------------------------------------
// 	DWORD l_dwErrCode = TR_SREF_OK;
// 	int l_iEventNext;
// 	if (TR_SREF_OK == l_dwErrCode) {
// 		if (((0 == pEvParams) && (0 != m_bRef2ndMeasure)) || (0 != l_bDoRefMeas)) {
// 			// 取得必要 ---------------------------------------------
// /* added 2009.07.02 hmenjo リファレンス用ターレット&フィルタ切替え移動 ---------- { ---------- */
// 			// ターレット制御(測定用：測定プログラムの Lens)
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
// //			LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
// 			((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
// 			// カラーフィルタ制御(リファレンス測定用：測定プログラムの OpticalFilter)
// 			if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
// 				// 異常時でも無視します．
// 				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSrRefer::enter_ST_SREF_N_MOVE()."));
// 			}
// /* added 2009.07.02 hmenjo リファレンス用ターレット&フィルタ切替え移動 ---------- } ---------- */
// 			// リファレンス測定位置を取得
// 			SR_REFERENCE_POSITION	l_SrReferencePosition;
// 			ConfigFile_GetNanoSpecIni(&l_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);
// 			// ダークリファレンス測定位置
// 			m_DRefPos.lX = l_SrReferencePosition.LocDarkReference.lX;
// 			m_DRefPos.lY = l_SrReferencePosition.LocDarkReference.lY;
// 			m_DRefPos.lZ = l_SrReferencePosition.LocDarkReference.lZ;
// 			// (ノーマル)リファレンス測定位置
// 			switch (m_wScanType) {
// 			case MEAS_PROG_TYPE_SR_THICKNESS:			// 膜厚
// 			case MEAS_PROG_TYPE_SR_REFLECTANCE:			// 反射率
// 			case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:		// 反射色度
// 			default:
// 				m_NRefPos.lX = l_SrReferencePosition.LocReflectReference.lX;
// 				m_NRefPos.lY = l_SrReferencePosition.LocReflectReference.lY;
// 				m_NRefPos.lZ = l_SrReferencePosition.LocReflectReference.lZ;
// 				break;
// 			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:		// 透過率
// /* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- { ---------- */
// 			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:		/* ガントリ透過率	*/
// /* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- } ---------- */
// 			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:	// 透過色度
// 				m_NRefPos.lX = l_SrReferencePosition.LocTransmitAbsoluteReference.lX;
// 				m_NRefPos.lY = l_SrReferencePosition.LocTransmitAbsoluteReference.lY;
// 				m_NRefPos.lZ = l_SrReferencePosition.LocTransmitAbsoluteReference.lZ;
// 				break;
// 			case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:		// 光学濃度
// 				switch (m_dwRefCounter) {
// 				case 1:		// １回目
// 					m_NRefPos.lX = l_SrReferencePosition.LocTransmitFirstReference.lX;
// 					m_NRefPos.lY = l_SrReferencePosition.LocTransmitFirstReference.lY;
// 					m_NRefPos.lZ = l_SrReferencePosition.LocTransmitFirstReference.lZ;
// 					break;
// 				case 2:		// ２回目
// 				default:
// 					m_NRefPos.lX = l_SrReferencePosition.LocTransmitSecondReference.lX;
// 					m_NRefPos.lY = l_SrReferencePosition.LocTransmitSecondReference.lY;
// 					m_NRefPos.lZ = l_SrReferencePosition.LocTransmitSecondReference.lZ;
// 					break;
// 				}
// 				break;
// 			}
// #ifndef CHIEF_STAGE_ON
// 			// 補正
// 			STAGE_COORD_XYZ l_LocXYZ;
// 			l_LocXYZ.lX = m_NRefPos.lX; l_LocXYZ.lY = m_NRefPos.lY; l_LocXYZ.lZ = m_NRefPos.lZ;
// 			NS_ConvertToStageMoveCoord(&l_LocXYZ);		// (ノーマル)リファレンス
// 			m_NRefPos.lX = l_LocXYZ.lX; m_NRefPos.lY = l_LocXYZ.lY; m_NRefPos.lZ = l_LocXYZ.lZ;
// 			l_LocXYZ.lX = m_DRefPos.lX; l_LocXYZ.lY = m_DRefPos.lY; l_LocXYZ.lZ = m_DRefPos.lZ;
// 			NS_ConvertToStageMoveCoord(&l_LocXYZ);		// ダークリファレンス
// 			m_DRefPos.lX = l_LocXYZ.lX; m_DRefPos.lY = l_LocXYZ.lY; m_DRefPos.lZ = l_LocXYZ.lZ;
// #endif
// /* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- { ---------- */
// 			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
// 				/* ダークリファレンスへ(ノーマルリファレンスはパスします)	*/
// 				/* トランジション更新	*/
// 				m_dwAFValid = 0;	/* AF 無し	*/
// 				TransiEvent(EV_SREF_N_MEAS_DONE);
// 				return;
// 			}
// /* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- } ---------- */
// 			// XY 移動実行 ------------------------------------------
// 			STAGE_COORD l_SrStageCoord;
// 			l_SrStageCoord.lX = m_NRefPos.lX;
// 			l_SrStageCoord.lY = m_NRefPos.lY;
//
// 			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_STAGE_MOVING);	// Status Bar
// 			((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to (Normal) Reference position."));
// #ifndef CHIEF_STAGE_ON
// //			if (0 == StageMoveAbsoluteEx(&l_SrStageCoord, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
// 			if (0 == StageMoveAbsolute(&l_SrStageCoord)) {
// #else
// 			if (0 == NS_StageMoveAbsoluteEx(&l_SrStageCoord)) {
// #endif
// 				// 移動開始失敗
// 				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_ReferencePositionMoveFailed));
// 				l_iEventNext = EV_SREF_N_MOVE_FAIL;
// 				l_dwErrCode = TR_SREF_N_MOVESTART_FAIL;
// 			} else {
// 				// 移動開始成功
// 				((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 21;
// 				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
// 				// WAIT 状態にするためトランジションを更新しません．
// 			}
// 		} else {
// // 2009.09.29 K.Matsuo GTr -->
// 			// （実機から）取得不要
// 			// その代わり、リファレンスデータファイルを読み込む
// 			MEAS_ReadRefFile(l_szMainRcpName);
// 			if (0 != m_bRef2ndMeasure) {
// 				MEAS_ReadRefFile2ndRefT1(l_szMainRcpName);
// 				MEAS_ReadRefFile2ndRefT2(l_szMainRcpName);
// 			}
// // 2009.09.29 K.Matsuo GTr <--
// 			l_iEventNext = EV_SREF_IDLE;
// 		}
// 	} else {
// 		// 異常発生していた
// 		l_iEventNext = EV_SREF_N_MOVE_FAIL;
// 	}
//
// 	if ((0 != l_dwErrCode) || ((0 == l_bDoRefMeas) && (m_dwRefCounter < 2))) {
// 		// トランジション更新
// 		TransiEvent(l_iEventNext);
// 		// 呼出元トランジションに完了を通知
// 		NotifyComplete((cEventParams*) l_dwErrCode);
// 	}
// }
#else
/*
 *	SR リファレンス位置へ移動完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_PRECHECK_MOVE(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[2], (DWORD) pEvParams);

	int l_iEventNext;

// bagus add 2014.01.10 (Stage None対応) -->
		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
// bagus add 2014.01.10 (Stage None対応) <--

	// 初回呼出かどうか判定 ---------------------------------------------------
	if (0 != pEvParams) {
		// 初回呼出し -------------------------------------
		// 呼出元トランジションを保存
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		m_dwParentTransi = (DWORD) pEvParams;
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		m_dwParentTransi = LOBYTE(LOWORD(pEvParams));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */
		// 複数回測定用カウンタを１にリセット
		m_dwRefCounter = 1;
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
		/* SR ヘッドにします．	*/
		m_wHeadPre = NS_GetCurrentHeadType();
		((CChiefView*) m_pcChiefView)->SelectHead(HEAD_TYPE_SR, FALSE);
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
// 2013.11.07 Bagus Add (TohoSpec対応) -->

		if(g_lModelType != MODEL_T3100){
// 2013.11.07 Bagus Add (TohoSpec対応) <--
// bagus Mod 2014.01.10 (Stage None対応) -->
			if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
// 2013.02.22 bagus Substrate thickness setting -->
				if (((CChiefView*) m_pcChiefView)->SelectSubstrateThickIndex(0)) {	// 板厚設定には、リファレンスを指定する
					((CChiefView*) m_pcChiefView)->LogChief(_T("Select Reference Sub Thickness failed."));
				}
			}
// 2013.02.22 bagus Substrate thickness setting <--
// bagus Mod 2014.01.10 (Stage None対応) <--
// 2013.11.07 Bagus Add (TohoSpec対応) -->
		}
// 2013.11.07 Bagus Add (TohoSpec対応) <--
	} else {
		// 複数回呼出し -----------------------------------
		// 複数回測定用カウンタ＋１
		m_dwRefCounter++;
	}

/* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
	LPMAIN_RCP_INFO		l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */

/* added 2009.10.29 hmenjo CTA リファレンス無し ---------- { ---------- */
	/* SE，CTA の場合はリファレンス不要	*/
	if ((HEAD_TYPE_CTA == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
	 || (HEAD_TYPE_4PP == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType)
	 || (HEAD_TYPE_SE == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) 
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	 || (HEAD_TYPE_COMPEASE == l_pMainRcpInfo->MainRcpParam.hdr.wHeadType) ) {
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		TransiEvent(EV_SREF_IDLE);	/* トランジション更新	*/
		NotifyComplete((cEventParams*) TR_SREF_OK);	/* 呼出元トランジションに完了を通知	*/
		return;
	}
/* added 2009.10.29 hmenjo CTA リファレンス無し ---------- } ---------- */

	// 2013.02.25 bagus -->
	// 測定開始前にシャッターを閉じる処理を追加。
	// CompleteEASE PC側で手動シャッター開されたまま、SR測定開始された場合を考慮する。
	if (((CMainFrame*)(((CChiefView*) m_pcChiefView)->m_pcMainFrame))->m_SystemConfig.HeadType.bCompEASE) {
		MEAS_CompEASEHead_CloseLampShutter();
	}
	// 2013.02.25 bagus <--

// 2013.11.07 Bagus Delete (TohoSpec対応) -->
#if 0 
// 	// ハードシミュレートモード判定 -------------------------------------------
// 	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
// 		// ハードシミュレートモードだったので，正常終了とします．
// 		// 2008.12.30 matsuo add ----->
// /* modified 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- { ---------- */
// //		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 1st (Normal) Reference."));
// //		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// //		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_N_1ST_MEASING);	// Status Bar
// //		MEAS_1stReference(l_pMeasProgInfo);
// //#ifdef CHIEF_USE_SLEEP
// //		::Sleep(1000);
// //#endif
// /* modified 2009.09.09 hmenjo 透過率 ガントリでダークのみ ----------			  */
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
// //		LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
// /* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
// 		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
// 		 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G != l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
// 			/* 通常透過率のみ	*/
// 			((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 1st (Normal) Reference."));
// 			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_N_1ST_MEASING);	// Status Bar
// 			MEAS_1stReference(l_pMeasProgInfo);
// #ifdef CHIEF_USE_SLEEP
// 			::Sleep(1000);
// #endif
// 		}
// /* modified 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- } ---------- */
// 		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring Dark T1 Reference."));
// 		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_D_T1_MEASING);	// Status Bar
// 		MEAS_Dark_T1(l_pMeasProgInfo);
// 		TCHAR	l_szMainRcpName[256];
// 		((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
// 		MEAS_WriteGTrRefFile(l_szMainRcpName);
// 		// 2008.12.30 matsuo add <-----
// #ifdef CHIEF_USE_SLEEP
// 		::Sleep(5000);
// #endif
// #ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
// 		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_D_T1_COMPLETE);	// Status Bar
// #else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
// 		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
// #endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
// 		// トランジション更新
// 		TransiEvent(EV_SREF_IDLE);
// 		// 呼出元トランジションに完了を通知
// 		NotifyComplete((cEventParams*) TR_SREF_OK);
// 		return;
// 	}
#endif
// 2013.11.07 Bagus Delete (TohoSpec対応) <--

	// 以降は実機の場合です ---------------------------------------------------
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	BOOL l_bDoRefMeas = FALSE;
	m_bDoRefMeas = FALSE;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	TCHAR	l_szMainRcpName[256];
	if (0 != pEvParams) {
		// リファレンス設定を取得 ---------------------------------------------
/* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
//		LPMAIN_RCP_INFO		l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
//		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
		m_bMeasRef = l_pMainRcpInfo->MainRcpParam._SR.bMeasRef;		// 更新フラグ(TRUE:毎回更新，FALSE:有効期限チェックによる)
		m_bWarnFlag = l_pMeasProgInfo->Ref.hdr.bWarnFlag;			// 有効期限チェックフラグ(0:チェックしない，1:チェックする)
		m_dLifeTime = l_pMeasProgInfo->Ref.hdr.dLifeTime;			// リファレンス更新間隔[ms](有効期限)
		m_bRef2ndMeasure = l_pMeasProgInfo->Ref2nd.bMeasure;		// セカンドリファレンス測定 要/不要
		m_wScanType = l_pMeasProgInfo->ScanParams.hdr.wScanType;	// 測定項目
/* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- { ---------- */
		if (
			(HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		 && (
				(MEAS_PROG_TYPE_SR_TRANSMITTANCE == l_pMeasProgInfo->ScanParams.hdr.wScanType)
			 || (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)
				)
			) {
			/* 透過率の場合はセカンドリファレンスは無しにします．	*/
			m_bRef2ndMeasure = FALSE;
		}
/* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- } ---------- */
		// 有効期限を確認 -----------------------------------------------------
		// カレントメインレシピ名を取得
		((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
/* modified 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- { ---------- */
//		BOOL l_bExpire = FALSE;
//		if (0 == m_bMeasRef) {
//			// 有効期限外で更新の設定だった
//			if (0 != MEAS_CheckRefFileElapsedTimeOut(l_szMainRcpName, m_dLifeTime)) {
//				// 有効期限切れだった
//				l_bExpire = TRUE;
//			}
//		}
//		// 設定からリファレンスの取得が必要かどうか判定 -----------------------
//		if ((0 != m_bMeasRef) || ((0 != m_bWarnFlag) && (0 != l_bExpire))) {
//			// 必要
//			//		(「毎回更新」か，「有効期限チェックする」で「有効期限切れ」の場合)
//			l_bDoRefMeas = TRUE;
//		} else {
//			// 不要
//			// さらにリファレンスデータファイルの有無で必要かどうかを判定します．
//			if (0 != m_bRef2ndMeasure) {
//				// セカンドリファレンス必要
//				if ((0 == MEAS_ReadRefFile2ndRefT1(l_szMainRcpName))
//				 || (0 == MEAS_ReadRefFile2ndRefT2(l_szMainRcpName))) {
//					// データファイル(「セカンド リファレンス T1 データ」と「セカンド リファレンス T2 データ」)が無かった
//					l_bDoRefMeas = TRUE;
//				} else {
//					// データファイルが有った
//					l_bDoRefMeas = FALSE;
//				}
//			} else {
//				// セカンドリファレンス不要
//				if (0 == MEAS_ReadRefFile(l_szMainRcpName)) {
//					// データファイル(「ファースト リファレンスデータ」)が無かった
//					l_bDoRefMeas = TRUE;
//				} else {
//					// データファイルが有った
//					l_bDoRefMeas = FALSE;
//				}
//			}
//		}
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
		if(g_lModelType != MODEL_T3100){
// 2009.10.09 K.Matsuo -->
			if (0 != m_bMeasRef) {
				// 必要
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//				l_bDoRefMeas = TRUE;
				m_bDoRefMeas = TRUE;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			} else {
// 2009.10.09 K.Matsuo <--
/* modified 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- 			 */
				int l_iRefData = ((CChiefView*) m_pcChiefView)->CheckReferenceData(l_szMainRcpName, m_dLifeTime, m_bRef2ndMeasure);
				if (
/* modified 2009.11.11 hmenjo Refer 期限チェックミス修正 ---------- { ---------- */
//					((1 == l_iRefData) && (0 != m_bWarnFlag))	/* 有効期限切れで，有効期限切れチェックする設定の場合	*/
//				 || (2 <= l_iRefData)	/* ファイルが存在しない	*/
/* modified 2009.11.11 hmenjo Refer 期限チェックミス修正 ---------- 			 */
					((2 == l_iRefData) && (0 != m_bWarnFlag))	/* 有効期限切れで，有効期限切れチェックする設定の場合	*/
				 || (1 == l_iRefData)	/* ファイルが存在しない	*/
				 || (2 < l_iRefData)	/* その他の異常	*/
/* modified 2009.11.11 hmenjo Refer 期限チェックミス修正 ---------- } ---------- */
					) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//					l_bDoRefMeas = TRUE;
					m_bDoRefMeas = TRUE;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
				}
			}
/* modified 2009.09.10 hmenjo リファレンスデータチェック関数追加 ---------- } ---------- */
			// ハードシミュレートモード判定 -------------------------------------------
			if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
				m_dwAFValid = 0;	/* AF 無し	*/
//				l_iEventNext = EV_SREF_N_SIMULATION;
				if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
					l_iEventNext = EV_SREF_N_SIMULATION;
				}
				else{
					if (m_dwParentTransi == TRANSI_SEQ){
						l_iEventNext = EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM;
					}
					else{
						l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE;
					}
				}
			}
			else{
// bagus add 2014.01.10 (Stage None対応) -->
//				l_iEventNext = EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM;
				if (l_SystemConfig.nStageType != STAGE_TYPE_NONE) {
					l_iEventNext = EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM;
				}
				else
				{
					if (m_dwParentTransi == TRANSI_SEQ){
						l_iEventNext = EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM;
					}
					else{
						l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE;
					}
				}
// bagus add 2014.01.10 (Stage None対応) <--
			}
		}
		else{
			m_dwAFValid = 0;	/* AF 無し	*/

			if (0 == m_bMeasRef) {
				// 有効期限を確認 -----------------------------------------------------
				// カレントメインレシピ名を取得
				((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
				int l_iRefData = ((CChiefView*) m_pcChiefView)->CheckReferenceData(l_szMainRcpName, m_dLifeTime, m_bRef2ndMeasure);
				switch(l_iRefData){
				case 0:
					if(((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetMeasMode() != MEASMODE_TEST){
						l_iEventNext = EV_SREF_N_PRECHECK_NO_REF;
					}
					else{
						// Testモードは有効期限に関係なくリファレンスチェックを実行する
						l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE;
					}
					break;
				case 1:
					l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE;
					break;
				case 2:
				default:
					if(((CNanoSpecDoc*) ((CChiefView*) m_pcChiefView)->m_pcNanoSpecDoc)->GetMeasMode() != MEASMODE_TEST){
						l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_EXPIRE;
					}
					else{
						// Testモードは有効期限に関係なくリファレンスチェックを実行する
						l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE;
					}
					break;
				}
			}
			else{
				l_iEventNext = EV_SREF_N_PRECHECK_NEED_CONFIRM_MOVE;
			}
		}
	}
	else{
// bagus add 2014.01.10 (Stage None対応) -->
//		l_iEventNext = EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM;
		if (((CMainFrame*)(((CChiefView*) m_pcChiefView)->m_pcMainFrame))->m_SystemConfig.nStageType != STAGE_TYPE_NONE) {
			l_iEventNext = EV_SREF_N_PRECHECK_MOVE_NO_CONFIRM;
		}
		else
		{
			l_iEventNext = EV_SREF_N_MOVE_DONE;
		}
// bagus add 2014.01.10 (Stage None対応) <--
	}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

/* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
	if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
	 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
		/* ガントリ透過率では設定によらず必ず実施します．	*/
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		l_bDoRefMeas = TRUE;
		m_bDoRefMeas = TRUE;
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	}
/* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */

	// トランジション更新
	TransiEvent(l_iEventNext, pEvParams);
}

/*
 *	SR ターレット（レンズ）／フィルタ手動合わせ（ユーザー確認）
 */
void CChiefTransiSrRefer::enter_ST_SREF_NO_REF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[3], (DWORD) pEvParams);

	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_DISP_CONFIRM_POPUP, 0, MAKELPARAM(l_pMeasProgInfo->ScanParams._SR.iLens, l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType));

	// WAIT 状態にするためトランジションを更新しません．
//	TransiEvent(EV_SREF_N_SHOW_INFO_DLG_DONE);
}

/*
 *	SR ターレット（レンズ）／フィルタ手動合わせ（ユーザー確認）完了
 */
void CChiefTransiSrRefer::enter_ST_SREF_NO_REF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[4], (DWORD) pEvParams);

	// トランジション更新
	TransiEvent(EV_SREF_IDLE);
	// 呼出元トランジションに完了を通知
	NotifyComplete((cEventParams*) TR_SREF_OK);
}

/*
 *	SR ターレット（レンズ）／フィルタ手動合わせ（ユーザー確認）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_PRE_CONFIRM_EXPIRE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[5], (DWORD) pEvParams);

	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_DISP_CONFIRM_POPUP, 0, MAKELPARAM(l_pMeasProgInfo->ScanParams._SR.iLens, l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType));

	// WAIT 状態にするためトランジションを更新しません．
//	TransiEvent(EV_SREF_N_SHOW_INFO_DLG_DONE);
}

/*
 *	SR リファレンス有効期限確認(WAIT 状態)
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_CONFIRM_EXPIRE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[6], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_DISP_CONFIRM_POPUP, 1, 0);

	// WAIT 状態にするためトランジションを更新しません．
//	TransiEvent(EV_SREF_N_CONFIRM_EXPIRE_YES);
//	TransiEvent(EV_SREF_N_CONFIRM_EXPIRE_NO);
}

/*
 *	SR リファレンス有効期限確認結果（ユーザー確認結果YES）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_EXPIRE_YES(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[7], (DWORD) pEvParams);

	m_bDoRefMeas = TRUE;

	TransiEvent(EV_SREF_N_CONFIRM_NEXT);
}

/*
 *	SR リファレンス有効期限確認結果（ユーザー確認結果NO・リファレンス取得不要）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_EXPIRE_NO(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[8], (DWORD) pEvParams);

	m_bDoRefMeas = FALSE;

	TransiEvent(EV_SREF_IDLE);	/* トランジション更新	*/
	NotifyComplete((cEventParams*) TR_SREF_OK);	/* 呼出元トランジションに完了を通知	*/
}

/*
 *	SR ターレット（レンズ）／フィルタ手動合わせ（ユーザー確認）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_PRE_CONFIRM_MOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[9], (DWORD) pEvParams);

	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_DISP_CONFIRM_POPUP, 0, MAKELPARAM(l_pMeasProgInfo->ScanParams._SR.iLens, l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType));

	// WAIT 状態にするためトランジションを更新しません．
//	TransiEvent(EV_SREF_N_SHOW_INFO_DLG_DONE);
}

/*
 *	SR リファレンス位置への移動（ユーザー確認・WAIT 状態）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_CONFIRM_MOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[10], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_DISP_CONFIRM_POPUP, 10, 0);

	// WAIT 状態にするためトランジションを更新しません．
//	TransiEvent(EV_SREF_N_CONFIRM_MOVE_OK);
//	TransiEvent(EV_SREF_N_CONFIRM_MOVE_CANCEL);
}

/*
 *	SR リファレンス位置への移動結果（ユーザー確認結果OK）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_MOVE_OK(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[11], (DWORD) pEvParams);

	TransiEvent(EV_SREF_N_CONFIRM_NEXT);
}

/*
 *	SR リファレンス位置への移動結果（ユーザー確認結果Cancel）
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_MOVE_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[12], (DWORD) pEvParams);

	TransiEvent(EV_SREF_IDLE);	/* トランジション更新	*/
	NotifyComplete((cEventParams*) TR_SREF_CANCEL);	/* 呼出元トランジションに完了を通知	*/
}

/*
 *	SR リファレンス位置へ移動完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_MOVE(const cEventParams* const pEvParams)
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- { ---------- */
/*				LOBYTE(LOWORD(pEvParams))：呼出元トランジション
/* added 2009.09.09 hmenjo 引数コメント追加 ---------- } ---------- */
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[2], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[13], (DWORD) pEvParams);

/* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
	LPMAIN_RCP_INFO		l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* added 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */

	// 以降は実機の場合です ---------------------------------------------------
	TCHAR	l_szMainRcpName[256];

	// カレントメインレシピ名を取得
	((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);

	// リファレンス測定位置への移動処理 ---------------------------------------
	DWORD l_dwErrCode = TR_SREF_OK;
	int l_iEventNext;
	if (TR_SREF_OK == l_dwErrCode) {
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		if (((0 == pEvParams) && (0 != m_bRef2ndMeasure)) || (0 != l_bDoRefMeas)) {
		if (((0 == pEvParams) && (0 != m_bRef2ndMeasure)) || (0 != m_bDoRefMeas)) {
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
			// 取得必要 ---------------------------------------------
/* added 2009.07.02 hmenjo リファレンス用ターレット&フィルタ切替え移動 ---------- { ---------- */
			// ターレット制御(測定用：測定プログラムの Lens)
/* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- { ---------- */
//			LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
/* deleted 2009.09.11 hmenjo SrRef ガントリ透過率で必ず実施 ---------- } ---------- */
			((CChiefView*) m_pcChiefView)->SelectLens(l_pMeasProgInfo->ScanParams._SR.iLens);
			// カラーフィルタ制御(リファレンス測定用：測定プログラムの OpticalFilter)
			if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
				// 異常時でも無視します．
				((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSrRefer::enter_ST_SREF_N_MOVE()."));
			}
/* added 2009.07.02 hmenjo リファレンス用ターレット&フィルタ切替え移動 ---------- } ---------- */
			// リファレンス測定位置を取得
			SR_REFERENCE_POSITION	l_SrReferencePosition;
			ConfigFile_GetNanoSpecIni(&l_SrReferencePosition, CONFIG_FILE_SR_REFERENCE_POSITION);
			// ダークリファレンス測定位置
			m_DRefPos.lX = l_SrReferencePosition.LocDarkReference.lX;
			m_DRefPos.lY = l_SrReferencePosition.LocDarkReference.lY;
			m_DRefPos.lZ = l_SrReferencePosition.LocDarkReference.lZ;
			// (ノーマル)リファレンス測定位置
			switch (m_wScanType) {
			case MEAS_PROG_TYPE_SR_THICKNESS:			// 膜厚
			case MEAS_PROG_TYPE_SR_REFLECTANCE:			// 反射率
			case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:		// 反射色度
			default:
				m_NRefPos.lX = l_SrReferencePosition.LocReflectReference.lX;
				m_NRefPos.lY = l_SrReferencePosition.LocReflectReference.lY;
				m_NRefPos.lZ = l_SrReferencePosition.LocReflectReference.lZ;
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:		// 透過率
/* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- { ---------- */
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:		/* ガントリ透過率	*/
/* added 2009.09.09 hmenjo 透過率 測定位置(座標) ---------- } ---------- */
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:	// 透過色度
				m_NRefPos.lX = l_SrReferencePosition.LocTransmitAbsoluteReference.lX;
				m_NRefPos.lY = l_SrReferencePosition.LocTransmitAbsoluteReference.lY;
				m_NRefPos.lZ = l_SrReferencePosition.LocTransmitAbsoluteReference.lZ;
				break;
			case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:		// 光学濃度
				switch (m_dwRefCounter) {
				case 1:		// １回目
					m_NRefPos.lX = l_SrReferencePosition.LocTransmitFirstReference.lX;
					m_NRefPos.lY = l_SrReferencePosition.LocTransmitFirstReference.lY;
					m_NRefPos.lZ = l_SrReferencePosition.LocTransmitFirstReference.lZ;
					break;
				case 2:		// ２回目
				default:
					m_NRefPos.lX = l_SrReferencePosition.LocTransmitSecondReference.lX;
					m_NRefPos.lY = l_SrReferencePosition.LocTransmitSecondReference.lY;
					m_NRefPos.lZ = l_SrReferencePosition.LocTransmitSecondReference.lZ;
					break;
				}
				break;
			}
#ifndef CHIEF_STAGE_ON
			// 補正
			STAGE_COORD_XYZ l_LocXYZ;
			l_LocXYZ.lX = m_NRefPos.lX; l_LocXYZ.lY = m_NRefPos.lY; l_LocXYZ.lZ = m_NRefPos.lZ;
			NS_ConvertToStageMoveCoord(&l_LocXYZ);		// (ノーマル)リファレンス
			m_NRefPos.lX = l_LocXYZ.lX; m_NRefPos.lY = l_LocXYZ.lY; m_NRefPos.lZ = l_LocXYZ.lZ;
			l_LocXYZ.lX = m_DRefPos.lX; l_LocXYZ.lY = m_DRefPos.lY; l_LocXYZ.lZ = m_DRefPos.lZ;
			NS_ConvertToStageMoveCoord(&l_LocXYZ);		// ダークリファレンス
			m_DRefPos.lX = l_LocXYZ.lX; m_DRefPos.lY = l_LocXYZ.lY; m_DRefPos.lZ = l_LocXYZ.lZ;
#endif
/* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- { ---------- */
			if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
			 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
				/* ダークリファレンスへ(ノーマルリファレンスはパスします)	*/
				/* トランジション更新	*/
				m_dwAFValid = 0;	/* AF 無し	*/
				TransiEvent(EV_SREF_N_MEAS_DONE);
				return;
			}
/* added 2009.09.09 hmenjo 透過率 ガントリでダークのみ ---------- } ---------- */
			// XY 移動実行 ------------------------------------------
			STAGE_COORD l_SrStageCoord;
			l_SrStageCoord.lX = m_NRefPos.lX;
			l_SrStageCoord.lY = m_NRefPos.lY;

			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_STAGE_MOVING);	// Status Bar
			((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to (Normal) Reference position."));
#ifndef CHIEF_STAGE_ON
//			if (0 == StageMoveAbsoluteEx(&l_SrStageCoord, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
			if (0 == StageMoveAbsolute(&l_SrStageCoord)) {
#else
			if (0 == NS_StageMoveAbsoluteEx(&l_SrStageCoord)) {
#endif
				// 移動開始失敗
				((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_ReferencePositionMoveFailed));
				l_iEventNext = EV_SREF_N_MOVE_FAIL;
				l_dwErrCode = TR_SREF_N_MOVESTART_FAIL;
			} else {
				// 移動開始成功
				((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 21;
				((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
				// WAIT 状態にするためトランジションを更新しません．
			}
		} else {
// 2009.09.29 K.Matsuo GTr -->
			// （実機から）取得不要
			// その代わり、リファレンスデータファイルを読み込む
			MEAS_ReadRefFile(l_szMainRcpName);
			if (0 != m_bRef2ndMeasure) {
				MEAS_ReadRefFile2ndRefT1(l_szMainRcpName);
				MEAS_ReadRefFile2ndRefT2(l_szMainRcpName);
			}
// 2009.09.29 K.Matsuo GTr <--
			l_iEventNext = EV_SREF_IDLE;
		}
	} else {
		// 異常発生していた
		l_iEventNext = EV_SREF_N_MOVE_FAIL;
	}

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	if ((0 != l_dwErrCode) || ((0 == l_bDoRefMeas) && (m_dwRefCounter < 2))) {
	if ((0 != l_dwErrCode) || ((0 == m_bDoRefMeas) && (m_dwRefCounter < 2))) {
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		// トランジション更新
		TransiEvent(l_iEventNext);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) l_dwErrCode);
	}
}
#endif
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

/*
 *	SR リファレンス位置Ｚへ移動完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_ZMOVE(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[3], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[14], (DWORD) pEvParams);

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

#if 0	//----- Kawashima 2008.12.01 ----->
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン

	TransiEvent(EV_SREF_N_ZMOVE_DONE);

	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#else	//----- Kawashima 2008.12.01 -----
#if 1	// Z 軸の位置はリファレンス位置の設定から取得せずサブコンパラメタの SETIP を使います．
	// Z 軸移動処理実行
	EEPROMDATA	l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z 軸の初期化停止位置 取得
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu;	// Z 軸の初期化停止位置ファクタ 取得
	// 初期化停止位置[pulse]を[0.1um]に変換します
	LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
	l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// 初期化停止位置[pulse]÷ファクタ[pulse/mm]
	l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]に変換する
	long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_Z_MOVING);	// Status Bar
	if (0 == StageElevatorMove(l_lZ)) {
#else	// Z 軸の位置はリファレンス位置の設定から取得せずサブコンパラメタの SETIP を使います．
	// Z 軸移動処理実行
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_Z_MOVING);	// Status Bar
	if (0 == StageElevatorMove(m_NRefPos.lZ)) {
#endif	// Z 軸の位置はリファレンス位置の設定から取得せずサブコンパラメタの SETIP を使います．
		// 移動開始失敗
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_ReferencePosition_Z_AxisMoveFailed));
		// トランジション更新
		TransiEvent(EV_SREF_N_ZMOVE_FAIL);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_N_ZMOVESTART_FAIL);
	} else {
		// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_Z_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to (Normal) Reference position."));
		// トランジション更新
		TransiEvent(EV_SREF_N_ZMOVE_DONE);
	}
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#endif	//----- Kawashima 2008.12.01 <-----
}

/*
 *	SR リファレンス位置Ｚへ移動完了(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_ZMOVE_DONE(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[4], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[15], (DWORD) pEvParams);

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

	// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_SWAYSETTLING);	// Status Bar
	((CChiefView*) m_pcChiefView)->WaitTimeBeforeMeas(1);

	// AF する/しないの設定
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	BOOL	l_bAF;
	if (0 != l_pMainRcpInfo->MainRcpParam._SR.bRefWithAF) {
		l_bAF = TRUE;
	} else {
		l_bAF = FALSE;
	}

	// AF 要/不要判定
	if (0 == l_bAF) {
		// 不要
		m_dwAFValid = 0;	// AF 無し
		// トランジション更新
		TransiEvent(EV_SREF_NOAF);
	} else {
		// 必要
		// 次のトランジションで AF 指令を発行しています．
		// トランジション更新
		TransiEvent(EV_SREF_AF);
	}
}

/*
 *	AF 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSrRefer::enter_ST_SREF_AF(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[5], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[16], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(3, 8)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSrRefer::enter_ST_SREF_AF() - AF Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// AF トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_TRAAF_BUSY);
	} else {
		// カラーフィルタ制御(デスキュー用：OPEN にします)
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- { ---------- */
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- { ---------- */
////		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ----------			   */
//		WORD l_wFilter;
//		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_SystemConfig.bUseOptFilterMeasPgmAF) {
//			l_wFilter = FILTER_OPEN;
//		} else {
//			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
//			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//		}
//		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- } ---------- */
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ----------			  */
		WORD l_wFilter;
		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_srConfig.bUseOptFilterMeasPgmAF) {
			l_wFilter = FILTER_OPEN;
		} else {
			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
		}
		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- } ---------- */
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSrRefer::enter_ST_SREF_AF()."));
		}
		// AF 発行
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_FOCUSING);	// Status Bar
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(TRANSI_SR_REFER, 0));
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_SR_REFER, 0), 0));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */

		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	AF 完了(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_AF_DONE(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[6], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[17], (DWORD) pEvParams);

	AF_VALID	l_AFValid;

	// AF 結果判定
	int l_iEventNext;
	l_AFValid.dwValid = (DWORD) pEvParams;
	m_dwAFValid = l_AFValid.dwValid;
#if 0	// AF 結果判定を変更 -------- { --------
	switch (l_AFValid.ucResultAF) {
	case 0:		// AF 無し
	case 1:		// AF 正常
	case 2:		// 自動強制測定
		l_iEventNext = EV_SREF_AF_OK;
		break;
	case 4:		// AF 失敗(元ポップアップ)
		switch (l_AFValid.ucResultPopup) {
		case 2:		// 閉じる(OK)(元 AF 無視)ボタン
			l_iEventNext = EV_SREF_AF_OK;
			break;
		case 0:		// ポップアップ無し
		case 1:		// リトライボタン
		case 3:		// スキップボタン
		case 4:		// キャンセルボタン
		default:
			l_iEventNext = EV_SREF_AF_NG;
			break;
		}
		break;
	case 3:		// 自動スキップ
	case 5:		// レシピ終了
	default:
		l_iEventNext = EV_SREF_AF_NG;
		break;
	}
#else	// AF 結果判定を変更 --------
	switch (l_AFValid.ucResultPopup) {
	case 0:		// ポップアップ無し
	case 1:		// リトライボタン
		switch(l_AFValid.ucResultAF) {
		case 0:		// AF 無し
		case 1:		// AF 正常
		case 2:		// 自動強制測定
			l_iEventNext = EV_SREF_AF_OK;
			break;
		case 3:		// 自動スキップ
		case 4:		// AF 失敗(ココでこの状態は存在しないはず)
		case 5:		// レシピ終了
		default:
			l_iEventNext = EV_SREF_AF_NG;
			break;
		}
		break;
	case 2:		// 閉じる(OK)(元 AF 無視)ボタン
		l_iEventNext = EV_SREF_AF_OK;
		break;
	case 3:		// スキップボタン
	case 4:		// キャンセルボタン(リファレンス測定では NG としてキャンセルさせます)
	default:
		l_iEventNext = EV_SREF_AF_NG;
		break;
	}
#endif	// AF 結果判定を変更 -------- } --------

	if (EV_SREF_AF_NG != l_iEventNext) {
		// カラーフィルタ制御(測定用：測定プログラムの OpticalFilter)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
		if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSrRefer::enter_ST_SREF_AF_DONE()."));
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元トランジションに完了を通知
	if (EV_SREF_AF_NG == l_iEventNext) {
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_FOCUS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		NotifyComplete((cEventParams*) TR_SREF_AF_NG);
	}
}

/*
 *	SR リファレンス測定完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_N_MEAS(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[7], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[18], (DWORD) pEvParams);

	// ファースト/セカンド リファレンス測定判定
	DWORD l_dwALID = 0;
	int l_iEventNext;
	DWORD l_dwErrCode = TR_SREF_OK;
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
	if (1 == m_dwRefCounter) {
		// ファースト リファレンス測定だった ------------------------------
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN, 1);	/* TR OPEN，SR CLOSE	*/
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- } ---------- */
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_1ST_MEASING);	// Status Bar
		// ハードシミュレートモード判定 -------------------------------------------
	 	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_N_1ST_MEASING);	// Status Bar
		}
		else{
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_1ST_MEASING);	// Status Bar
		}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 1st (Normal) Reference."));
		if (0 == MEAS_1stReference(l_pMeasProgInfo)) {
			// 異常終了 -----------------------------------------
			l_dwALID = ALID_ReferenceFirstMeasurementFailed;
			l_dwErrCode = TR_SREF_N_1STMEAS_FAIL;
			l_iEventNext = EV_SREF_N_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_1ST_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		} else {
			// 正常終了 -----------------------------------------
			// セカンド リファレンス要/不要判定
			if (0 != m_bRef2ndMeasure) {
				// セカンド リファレンス要
				l_iEventNext = EV_SREF_START;
			} else {
				// セカンド リファレンス不要
				l_iEventNext = EV_SREF_N_MEAS_DONE;
			}
		}
	} else {
		// セカンド リファレンス測定だった --------------------------------
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_2ND_MEASING);	// Status Bar
		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 2nd (Normal) T1/T2 Reference."));
		if (0 == MEAS_2ndReference_T1(l_pMeasProgInfo)) {
			// 異常終了	-----------------------------------------
			l_dwALID = ALID_ReferenceSecond_T1_Failed;
			l_dwErrCode = TR_SREF_N_2NDMEAS_T1_FAIL;
			l_iEventNext = EV_SREF_N_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_2ND_T1_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		} else if (0 == MEAS_2ndReference_T2(l_pMeasProgInfo)) {
			// 異常終了	-----------------------------------------
			l_dwALID = ALID_ReferenceSecond_T2_Failed;
			l_dwErrCode = TR_SREF_N_2NDMEAS_T2_FAIL;
			l_iEventNext = EV_SREF_N_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_2ND_T2_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		} else {
			// 正常終了 -----------------------------------------
			l_iEventNext = EV_SREF_N_MEAS_DONE;
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元トランジションに完了を通知
	if (TR_SREF_OK != l_dwErrCode) {
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* TR CLOSE，SR OPEN	*/
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, l_dwALID));
		NotifyComplete((cEventParams*) l_dwErrCode);
	}
// 2013.12.16 Bagus Mod (TohoSpec対応) -->
	else{
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
	}
// 2013.12.16 Bagus Mod (TohoSpec対応) <--
}

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
/*
 *	SR ダーク・移動判定(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_PRECHECK_MOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[19], (DWORD) pEvParams);

	int l_iEventNext;

// bagus Mod 2014.01.10 (Stage None対応) -->
	if(g_lModelType != MODEL_T3100){
		SYSTEM_CONFIG l_SystemConfig;
		ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

		// ハードシミュレートモード判定 -------------------------------------------
		if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
//			l_iEventNext = EV_SREF_D_SIMULATION;
			if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
				l_iEventNext = EV_SREF_D_SIMULATION;
			}
			else{
				if (m_dwParentTransi == TRANSI_SEQ){
					l_iEventNext = EV_SREF_D_PRECHECK_MOVE_NO_CONFIRM;
				}
				else{
					l_iEventNext = EV_SREF_D_PRECHECK_MOVE_CONFIRM_NEED;
				}
			}
		}
		else{
//			l_iEventNext = EV_SREF_D_PRECHECK_MOVE_NO_CONFIRM;
			if(l_SystemConfig.nStageType != STAGE_TYPE_NONE){
				l_iEventNext = EV_SREF_D_PRECHECK_MOVE_NO_CONFIRM;
			}
			else{
				if (m_dwParentTransi == TRANSI_SEQ){
					l_iEventNext = EV_SREF_D_PRECHECK_MOVE_NO_CONFIRM;
				}
				else{
					l_iEventNext = EV_SREF_D_PRECHECK_MOVE_CONFIRM_NEED;
				}
			}
		}
	}
	else{
		l_iEventNext = EV_SREF_D_PRECHECK_MOVE_CONFIRM_NEED;
	}
// bagus Mod 2014.01.10 (Stage None対応) <--

	TransiEvent(l_iEventNext);
}

/*
 *	SR ダーク位置への移動（ユーザー確認・WAIT 状態）
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_CONFIRM_MOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[20], (DWORD) pEvParams);

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->PostMessage(WM_DISP_CONFIRM_POPUP, 11, 0);

	// WAIT 状態にするためトランジションを更新しません．
//	TransiEvent(EV_SREF_D_CONFIRM_MOVE_OK);
//	TransiEvent(EV_SREF_D_CONFIRM_MOVE_CANCEL);
}

/*
 *	SR ダーク位置への移動結果（ユーザー確認結果OK）
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_MOVE_OK(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[21], (DWORD) pEvParams);

	TransiEvent(EV_SREF_D_CONFIRM_NEXT);
}

/*
 *	SR ダーク位置への移動結果（ユーザー確認結果CANCEL）
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_MOVE_CANCEL(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[22], (DWORD) pEvParams);

	TransiEvent(EV_SREF_IDLE);	/* トランジション更新	*/
	NotifyComplete((cEventParams*) TR_SREF_CANCEL);	/* 呼出元トランジションに完了を通知	*/
}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

/*
 *	SR ダーク位置へ移動完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_MOVE(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[8], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[23], (DWORD) pEvParams);

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

	// XY 軸移動処理実行
	STAGE_COORD l_SrStageCoord;
	l_SrStageCoord.lX = m_DRefPos.lX;
	l_SrStageCoord.lY = m_DRefPos.lY;

	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_STAGE_MOVING);	// Status Bar
	((CChiefView*) m_pcChiefView)->LogChief(_T("Stage is moving to Dark Reference position."));
#ifndef CHIEF_STAGE_ON
//	if (0 == StageMoveAbsoluteEx(&l_SrStageCoord, CHIEF_STGMVABS_MODE)) {		Stage.dll が対応するまでは下記の呼出し方法になります．
	if (0 == StageMoveAbsolute(&l_SrStageCoord)) {
#else
	if (0 == NS_StageMoveAbsoluteEx(&l_SrStageCoord)) {
#endif
		// 移動開始失敗
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_DarkReferencePositionMoveFailed));
		// トランジション更新
		TransiEvent(EV_SREF_D_MOVE_FAIL);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_D_MOVESTART_FAIL);
	} else {
		// 移動開始成功
		((CChiefView*) m_pcChiefView)->m_dwStageMoveState = 22;
		((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_XYSTAGE, TRUE);		// 動作中フラグ(XY ステージ)をオン
		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	SR ダーク位置Ｚへ移動完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_ZMOVE(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[9], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[24], (DWORD) pEvParams);

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

#if 1	// ダーク時は Z 軸を移動しません．(ノーマルの位置のままとします)
	TransiEvent(EV_SREF_D_ZMOVE_DONE);
#else	// ダーク時は Z 軸を移動しません．(ノーマルの位置のままとします)
#if 0	//----- Kawashima 2008.12.01 ----->
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン

	TransiEvent(EV_SREF_D_ZMOVE_DONE);

	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#else	//----- Kawashima 2008.12.01 -----
	// Z 軸移動処理実行
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_Z_MOVING);	// Status Bar
	if (0 == StageElevatorMove(m_DRefPos.lZ)) {
		// 移動開始失敗
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_DarkReferencePosition_Z_AxisMoveFailed));
		// トランジション更新
		TransiEvent(EV_SREF_D_ZMOVE_FAIL);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_D_ZMOVESTART_FAIL);
	} else {
		// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_Z_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to Dark Reference position."));
		// トランジション更新
		TransiEvent(EV_SREF_D_ZMOVE_DONE);
	}
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#endif	//----- Kawashima 2008.12.01 <-----
#endif	// ダーク時は Z 軸を移動しません．(ノーマルの位置のままとします)
}

/*
 *	SR ダーク測定完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_D_MEAS(const cEventParams* const pEvParams)
{
//	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[10], (DWORD) pEvParams);
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[25], (DWORD) pEvParams);

	// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_SWAYSETTLING);	// Status Bar
	((CChiefView*) m_pcChiefView)->WaitTimeBeforeMeas(1);

	DWORD l_dwALID = 0;
	int l_iEventNext;
	DWORD l_dwErrCode = TR_SREF_OK;
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
	// ダークリファレンス T1 測定 -----------------------------------------
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 3);	/* TR CLOSE，SR CLOSE	*/
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T1_MEASING);	// Status Bar
	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_D_T1_MEASING);	// Status Bar
	}
	else{
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T1_MEASING);	// Status Bar
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring Dark T1 Reference."));
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	if (0 == MEAS_Dark_T1(l_pMeasProgInfo)) {
		// 異常終了
		l_dwALID = ALID_DarkReferenceMeasurement_T1_Failed;
		l_dwErrCode = TR_SREF_D_MEAS_T1_FAIL;
		l_iEventNext = EV_SREF_D_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T1_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
	}
	// セカンド リファレンス要/不要判定 -----------------------------------
	if (0 != m_bRef2ndMeasure) {
		// セカンドリファレンス必要
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T2_MEASING);	// Status Bar
		((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring Dark T2 Reference."));
		if (0 == MEAS_Dark_T2(l_pMeasProgInfo)) {
			// 異常終了
			l_dwALID = ALID_DarkReferenceMeasurement_T2_Failed;
			l_dwErrCode = TR_SREF_D_MEAS_T2_FAIL;
			l_iEventNext = EV_SREF_D_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T2_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
			((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		}
	}
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* TR CLOSE，SR OPEN	*/
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
	// リファレンスデータ保存 ---------------------------------------------
	if (TR_SREF_OK == l_dwErrCode) {
		// 測定正常
		l_iEventNext = EV_SREF_D_MEAS_DONE;
		// カレントメインレシピ名を取得
		TCHAR	l_szMainRcpName[256];
		((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// ファースト リファレンスデータ書込み
// 2009.09.29 K.Matsuo GTr -->
//		if (0 == MEAS_WriteRefFile(l_szMainRcpName)) {
		BOOL l_bRet;
		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
			l_bRet = MEAS_WriteGTrRefFile(l_szMainRcpName);
		}
		else {
			l_bRet = MEAS_WriteRefFile(l_szMainRcpName);
		}
		if (0 == l_bRet) {
// 2009.09.29 K.Matsuo GTr <--
			// 書込み失敗
			l_dwALID = ALID_ReferenceDataWriteFailed;
			l_dwErrCode = TR_SREF_REFDATA_WR_ERR;
			l_iEventNext = EV_SREF_D_MEAS_ERR;
		} else {
			// セカンド リファレンス要/不要判定 ----------------------
			if (0 != m_bRef2ndMeasure) {
				// セカンド リファレンス必要設定 ----------------
				// セカンド リファレンス T1 データ書込み
				if (0 == MEAS_WriteRefFile2ndRefT1(l_szMainRcpName)) {
					// 書込み失敗
					l_dwALID = ALID_ReferenceData_T1_WriteFailed;
					l_dwErrCode = TR_SREF_REFDATAT1_WR_ERR;
					l_iEventNext = EV_SREF_D_MEAS_ERR;
				} else {
					// セカンド リファレンス T2 データ書込み
					if (0 == MEAS_WriteRefFile2ndRefT2(l_szMainRcpName)) {
						// 書込み失敗
						l_dwALID = ALID_ReferenceData_T2_WriteFailed;
						l_dwErrCode = TR_SREF_REFDATAT2_WR_ERR;
						l_iEventNext = EV_SREF_D_MEAS_ERR;
					} else {
						// 書込み成功
						l_iEventNext = EV_SREF_D_MEAS_DONE;
					}
				}
			} else {
				// セカンド リファレンス不要設定 ----------------
				l_iEventNext = EV_SREF_D_MEAS_DONE;
			}
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元トランジションに完了を通知
	if (0 != l_dwErrCode) {
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, l_dwALID));
		NotifyComplete((cEventParams*) l_dwErrCode);	// エラー時
	} else {
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_COMPLETED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		NotifyComplete((cEventParams*) m_dwAFValid);	// 正常時
	}
}

// bagus 2014.01.10 Add (Stage None対応) -->
/*
 *	SR リファレンス位置Ｚへ移動完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_STD_ZMOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[26], (DWORD) pEvParams);

	m_dwParentTransi = LOBYTE(LOWORD(pEvParams));

	// ハードシミュレートモード判定 -------------------------------------------
 	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		TransiEvent(EV_SREF_STD_SIMULATION);
		return;
	}

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

#if 0	//----- Kawashima 2008.12.01 ----->
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン

	TransiEvent(EV_SREF_STD_ZMOVE_DONE);

	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#else	//----- Kawashima 2008.12.01 -----
#if 1	// Z 軸の位置はリファレンス位置の設定から取得せずサブコンパラメタの SETIP を使います．
	// Z 軸移動処理実行
	EEPROMDATA	l_E2pRomParams;
	StageGetEEPromData(&l_E2pRomParams);
	long l_lZPos = l_E2pRomParams.AutoFocus.InitPos;	// Z 軸の初期化停止位置 取得
	long l_lZPosPpU = l_E2pRomParams.AutoFocus.ppu;	// Z 軸の初期化停止位置ファクタ 取得
	// 初期化停止位置[pulse]を[0.1um]に変換します
	LONG64 l_l64_Z = ((LONG64) l_lZPos) * ((LONG64) 1000000);
	l_l64_Z = l_l64_Z / ((LONG64) l_lZPosPpU);	// 初期化停止位置[pulse]÷ファクタ[pulse/mm]
	l_l64_Z = l_l64_Z * ((LONG64) 10000);	// [0.1um]に変換する
	long l_lZ = (long) (l_l64_Z / ((LONG64) 1000000));
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_Z_MOVING);	// Status Bar
	if (0 == StageElevatorMove(l_lZ)) {
#else	// Z 軸の位置はリファレンス位置の設定から取得せずサブコンパラメタの SETIP を使います．
	// Z 軸移動処理実行
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_Z_MOVING);	// Status Bar
	if (0 == StageElevatorMove(m_NRefPos.lZ)) {
#endif	// Z 軸の位置はリファレンス位置の設定から取得せずサブコンパラメタの SETIP を使います．
		// 移動開始失敗
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_ReferencePosition_Z_AxisMoveFailed));
		// トランジション更新
		TransiEvent(EV_SREF_STD_ZMOVE_FAIL);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_N_ZMOVESTART_FAIL);
	} else {
		// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_Z_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to (Normal) Reference position."));
		// トランジション更新
		TransiEvent(EV_SREF_STD_ZMOVE_DONE);
	}
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#endif	//----- Kawashima 2008.12.01 <-----
}

/*
 *	SR リファレンス位置Ｚへ移動完了(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_STD_ZMOVE_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[27], (DWORD) pEvParams);

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

	// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_SWAYSETTLING);	// Status Bar
	((CChiefView*) m_pcChiefView)->WaitTimeBeforeMeas(1);

	// AF する/しないの設定
	LPMAIN_RCP_INFO l_pMainRcpInfo = (LPMAIN_RCP_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMainRcpInfo);
	BOOL	l_bAF;
	if (0 != l_pMainRcpInfo->MainRcpParam._SR.bRefWithAF) {
		l_bAF = TRUE;
	} else {
		l_bAF = FALSE;
	}

	// AF 要/不要判定
	if (0 == l_bAF) {
		// 不要
		m_dwAFValid = 0;	// AF 無し
		// トランジション更新
		TransiEvent(EV_SREF_STD_NOAF);
	} else {
		// 必要
		// 次のトランジションで AF 指令を発行しています．
		// トランジション更新
		TransiEvent(EV_SREF_STD_AF);
	}
}

/*
 *	AF 完了待ち(WAIT 状態無しの場合有り)
 */
void CChiefTransiSrRefer::enter_ST_SREF_STD_AF(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[28], (DWORD) pEvParams);

/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- { ---------- */
//	if (false == ((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->IsIdle()) {
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ----------              */
	if (FALSE != ((CChiefView*) m_pcChiefView)->IsBusyTransi(3, 8)) {
		((CChiefView*) m_pcChiefView)->LogChief(_T("CChiefTransiSrRefer::enter_ST_SREF_AF() - AF Seq is busy."));
/* modified 2012.01.23 hmenjo [７]勝手アボート対策 ---------- } ---------- */
		// AF トランジションがビジーでした．
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_TRAAF_BUSY);
	} else {
		// カラーフィルタ制御(デスキュー用：OPEN にします)
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- { ---------- */
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- { ---------- */
////		if (0 == MEAS_SrHead_ChangeCcdShutter(FILTER_OPEN)) {
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ----------			   */
//		WORD l_wFilter;
//		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_SystemConfig.bUseOptFilterMeasPgmAF) {
//			l_wFilter = FILTER_OPEN;
//		} else {
//			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
//			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
//		}
//		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
///* modified 2009.07.02 hmenjo Seq 測定時フィルタ固定機能 ---------- } ---------- */
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ----------			  */
		WORD l_wFilter;
		if (0 == ((CMainFrame*) ((CChiefView*) m_pcChiefView)->m_pcMainFrame)->m_srConfig.bUseOptFilterMeasPgmAF) {
			l_wFilter = FILTER_OPEN;
		} else {
			LPMEAS_PROG_INFO l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
			l_wFilter = l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType;
		}
		if (0 == MEAS_SrHead_ChangeCcdShutter(l_wFilter)) {
/* modified 2009.07.03 hmenjo Seq 測定時フィルタ固定機能 訂正 ---------- } ---------- */
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSrRefer::enter_ST_SREF_STD_AF()."));
		}
		// AF 発行
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_FOCUSING);	// Status Bar
/* modified 2009.09.09 hmenjo 引数変更 ---------- { ---------- */
//		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(TRANSI_SR_STD_REFER, 0));
/* modified 2009.09.09 hmenjo 引数変更 ----------			   */
		((CChiefTransiAF*) ((CChiefView*) m_pcChiefView)->m_pcChiefTransiAF)->TransiEvent(EV_AAF_START, (cEventParams*) MAKELONG(MAKEWORD(TRANSI_SR_STD_REFER, 0), 0));
/* modified 2009.09.09 hmenjo 引数変更 ---------- } ---------- */

		// WAIT 状態にするためトランジションを更新しません．
	}
}

/*
 *	AF 完了(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_STD_AF_DONE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[29], (DWORD) pEvParams);

	AF_VALID	l_AFValid;

	// AF 結果判定
	int l_iEventNext;
	l_AFValid.dwValid = (DWORD) pEvParams;
	m_dwAFValid = l_AFValid.dwValid;
#if 0	// AF 結果判定を変更 -------- { --------
	switch (l_AFValid.ucResultAF) {
	case 0:		// AF 無し
	case 1:		// AF 正常
	case 2:		// 自動強制測定
		l_iEventNext = EV_SREF_STD_AF_OK;
		break;
	case 4:		// AF 失敗(元ポップアップ)
		switch (l_AFValid.ucResultPopup) {
		case 2:		// 閉じる(OK)(元 AF 無視)ボタン
			l_iEventNext = EV_SREF_STD_AF_OK;
			break;
		case 0:		// ポップアップ無し
		case 1:		// リトライボタン
		case 3:		// スキップボタン
		case 4:		// キャンセルボタン
		default:
			l_iEventNext = EV_SREF_STD_AF_NG;
			break;
		}
		break;
	case 3:		// 自動スキップ
	case 5:		// レシピ終了
	default:
		l_iEventNext = EV_SREF_STD_AF_NG;
		break;
	}
#else	// AF 結果判定を変更 --------
	switch (l_AFValid.ucResultPopup) {
	case 0:		// ポップアップ無し
	case 1:		// リトライボタン
		switch(l_AFValid.ucResultAF) {
		case 0:		// AF 無し
		case 1:		// AF 正常
		case 2:		// 自動強制測定
			l_iEventNext = EV_SREF_STD_AF_OK;
			break;
		case 3:		// 自動スキップ
		case 4:		// AF 失敗(ココでこの状態は存在しないはず)
		case 5:		// レシピ終了
		default:
			l_iEventNext = EV_SREF_STD_AF_NG;
			break;
		}
		break;
	case 2:		// 閉じる(OK)(元 AF 無視)ボタン
		l_iEventNext = EV_SREF_STD_AF_OK;
		break;
	case 3:		// スキップボタン
	case 4:		// キャンセルボタン(リファレンス測定では NG としてキャンセルさせます)
	default:
		l_iEventNext = EV_SREF_STD_AF_NG;
		break;
	}
#endif	// AF 結果判定を変更 -------- } --------

	if (EV_SREF_STD_AF_NG != l_iEventNext) {
		// カラーフィルタ制御(測定用：測定プログラムの OpticalFilter)
		LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
		if (0 == MEAS_SrHead_ChangeCcdShutter(l_pMeasProgInfo->ScanParams._SR.wOpticsFilterType)) {
			// 異常時でも無視します．
			((CChiefView*) m_pcChiefView)->LogChief(_T("Failed to MEAS_SrHead_ChangeCcdShutter() in CChiefTransiSrRefer::enter_ST_SREF_AF_DONE()."));
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元トランジションに完了を通知
	if (EV_SREF_STD_AF_NG == l_iEventNext) {
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_FOCUS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		NotifyComplete((cEventParams*) TR_SREF_AF_NG);
	}
}

/*
 *	SR リファレンス測定完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_STD_MEAS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[30], (DWORD) pEvParams);

	DWORD l_dwALID = 0;
	int l_iEventNext;
	DWORD l_dwErrCode = TR_SREF_OK;
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);

/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_OPEN, 1);	/* TR OPEN，SR CLOSE	*/
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- } ---------- */
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_1ST_MEASING);	// Status Bar
	// ハードシミュレートモード判定 -------------------------------------------
 	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_N_1ST_MEASING);	// Status Bar
	}
	else{
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_1ST_MEASING);	// Status Bar
	}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring 1st (Normal) Reference."));
	if (0 == MEAS_1stReference(l_pMeasProgInfo)) {
		// 異常終了 -----------------------------------------
		l_dwALID = ALID_ReferenceFirstMeasurementFailed;
		l_dwErrCode = TR_SREF_N_1STMEAS_FAIL;
		l_iEventNext = EV_SREF_STD_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_N_1ST_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
	} else {
		// 正常終了 -----------------------------------------
		l_iEventNext = EV_SREF_STD_MEAS_DONE;
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元トランジションに完了を通知
	if (TR_SREF_OK != l_dwErrCode) {
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* TR CLOSE，SR OPEN	*/
/* added 2009.09.09 hmenjo 透過率 ランプ制御 REF(９) ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, l_dwALID));

		NotifyComplete((cEventParams*) l_dwErrCode);
	}
// 2013.12.16 Bagus Mod (TohoSpec対応) -->
	else{
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar

//		NotifyComplete((cEventParams*) m_dwAFValid);	// 正常時
		NotifyComplete((cEventParams*) TR_SREF_OK);		// 正常時
	}
// 2013.12.16 Bagus Mod (TohoSpec対応) <--
}

/*
 *	SR ダーク位置Ｚへ移動完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_DRK_ZMOVE(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[31], (DWORD) pEvParams);

	m_dwParentTransi = LOBYTE(LOWORD(pEvParams));

	// ハードシミュレートモード判定 -------------------------------------------
 	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		TransiEvent(EV_SREF_DRK_SIMULATION);
		return;
	}

#if 1	// キャンセル対応改造
	if (0 != m_bCancelReq) {
		m_bCancelReq = FALSE;
		// トランジション更新
		TransiEvent(EV_SREF_IDLE);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_CANCEL);
		return;
	}
#endif

#if 1	// ダーク時は Z 軸を移動しません．(ノーマルの位置のままとします)
	TransiEvent(EV_SREF_DRK_ZMOVE_DONE);
#else	// ダーク時は Z 軸を移動しません．(ノーマルの位置のままとします)
#if 0	//----- Kawashima 2008.12.01 ----->
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン

	TransiEvent(EV_SREF_DRK_ZMOVE_DONE);

	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#else	//----- Kawashima 2008.12.01 -----
	// Z 軸移動処理実行
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, TRUE);		// 動作中フラグ(Z 軸)をオン
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_Z_MOVING);	// Status Bar
	if (0 == StageElevatorMove(m_DRefPos.lZ)) {
		// 移動開始失敗
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, ALID_DarkReferencePosition_Z_AxisMoveFailed));
		// トランジション更新
		TransiEvent(EV_SREF_DRK_ZMOVE_FAIL);
		// 呼出元トランジションに完了を通知
		NotifyComplete((cEventParams*) TR_SREF_D_ZMOVESTART_FAIL);
	} else {
		// 移動完了
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_Z_MOVED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
		((CChiefView*) m_pcChiefView)->LogChief(_T("Z axis was moved to Dark Reference position."));
		// トランジション更新
		TransiEvent(EV_SREF_DRK_ZMOVE_DONE);
	}
	((CChiefView*) m_pcChiefView)->ActuateFlagsSet(ACTUATE_ZAXIS, FALSE);		// 動作中フラグ(Z 軸)をオフ
#endif	//----- Kawashima 2008.12.01 <-----
#endif	// ダーク時は Z 軸を移動しません．(ノーマルの位置のままとします)
}

/*
 *	SR ダーク測定完了待ち(WAIT 状態無し)
 */
void CChiefTransiSrRefer::enter_ST_SREF_DRK_MEAS(const cEventParams* const pEvParams)
{
	((CChiefView*) m_pcChiefView)->LogChief_Transi(STATES_SR_REFER_DISP[32], (DWORD) pEvParams);

	// 測定開始前のヘッド揺れ収束待ち(2008.12.18 追加)
	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_SWAYSETTLING);	// Status Bar
	((CChiefView*) m_pcChiefView)->WaitTimeBeforeMeas(1);

	DWORD l_dwALID = 0;
	int l_iEventNext;
	DWORD l_dwErrCode = TR_SREF_OK;
	LPMEAS_PROG_INFO	l_pMeasProgInfo = (LPMEAS_PROG_INFO) (((CChiefView*) m_pcChiefView)->m_ChiefRecipes.pMeasProgInfo);
	// ダークリファレンス T1 測定 -----------------------------------------
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 3);	/* TR CLOSE，SR CLOSE	*/
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T1_MEASING);	// Status Bar
	// ハードシミュレートモード判定 -------------------------------------------
	if (0 != ((CMainFrame*) (((CChiefView*) m_pcChiefView)->m_pcMainFrame))->HardwareSimulation()) {
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_HWS_D_T1_MEASING);	// Status Bar
	}
	else{
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T1_MEASING);	// Status Bar
	}
	((CChiefView*) m_pcChiefView)->LogChief(_T("Measuring Dark T1 Reference."));
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	if (0 == MEAS_Dark_T1(l_pMeasProgInfo)) {
		// 異常終了
		l_dwALID = ALID_DarkReferenceMeasurement_T1_Failed;
		l_dwErrCode = TR_SREF_D_MEAS_T1_FAIL;
		l_iEventNext = EV_SREF_DRK_MEAS_ERR;
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_D_T1_MEAS_FAILED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
	}

/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- { ---------- */
	((CChiefView*) m_pcChiefView)->LampFilterR(FILTER_DARK, 1);	/* TR CLOSE，SR OPEN	*/
/* added 2009.09.08 hmenjo 透過率 ランプ制御 REF(８) ---------- } ---------- */
	// リファレンスデータ保存 ---------------------------------------------
	if (TR_SREF_OK == l_dwErrCode) {
		// 測定正常
		l_iEventNext = EV_SREF_DRK_MEAS_DONE;
		// カレントメインレシピ名を取得
		TCHAR	l_szMainRcpName[256];
		((CChiefView*) m_pcChiefView)->GetCurrentMainRecipeName(l_szMainRcpName);
		// ファースト リファレンスデータ書込み
// 2009.09.29 K.Matsuo GTr -->
//		if (0 == MEAS_WriteRefFile(l_szMainRcpName)) {
		BOOL l_bRet;
		if ((HEAD_TYPE_SR == l_pMeasProgInfo->ScanParams.hdr.wHeadType)
		 && (MEAS_PROG_TYPE_SR_TRANSMITTANCE_G == l_pMeasProgInfo->ScanParams.hdr.wScanType)) {
			l_bRet = MEAS_WriteGTrRefFile(l_szMainRcpName);
		}
		else {
			l_bRet = MEAS_WriteRefFile(l_szMainRcpName);
		}
		if (0 == l_bRet) {
// 2009.09.29 K.Matsuo GTr <--
			// 書込み失敗
			l_dwALID = ALID_ReferenceDataWriteFailed;
			l_dwErrCode = TR_SREF_REFDATA_WR_ERR;
			l_iEventNext = EV_SREF_DRK_MEAS_ERR;
		} else {
			// セカンド リファレンス要/不要判定 ----------------------
			if (0 != m_bRef2ndMeasure) {
				// セカンド リファレンス必要設定 ----------------
				// セカンド リファレンス T1 データ書込み
				if (0 == MEAS_WriteRefFile2ndRefT1(l_szMainRcpName)) {
					// 書込み失敗
					l_dwALID = ALID_ReferenceData_T1_WriteFailed;
					l_dwErrCode = TR_SREF_REFDATAT1_WR_ERR;
					l_iEventNext = EV_SREF_DRK_MEAS_ERR;
				} else {
					// セカンド リファレンス T2 データ書込み
					if (0 == MEAS_WriteRefFile2ndRefT2(l_szMainRcpName)) {
						// 書込み失敗
						l_dwALID = ALID_ReferenceData_T2_WriteFailed;
						l_dwErrCode = TR_SREF_REFDATAT2_WR_ERR;
						l_iEventNext = EV_SREF_DRK_MEAS_ERR;
					} else {
						// 書込み成功
						l_iEventNext = EV_SREF_DRK_MEAS_DONE;
					}
				}
			} else {
				// セカンド リファレンス不要設定 ----------------
				l_iEventNext = EV_SREF_DRK_MEAS_DONE;
			}
		}
	}

	// トランジション更新 -----------------------------------------------------
	TransiEvent(l_iEventNext);

	// 呼出元トランジションに完了を通知
	if (0 != l_dwErrCode) {
		((CChiefView*) m_pcChiefView)->PostMessage(WM_CHIF_REPORTALARM, MAKEWPARAM(MAKEWORD(CHRAMTD_REP_ALARM, CHRANFY_NOTIFY_ON), MAKEWORD(8, CHRAMSG_OK)), MAKELPARAM(0, l_dwALID));
		NotifyComplete((cEventParams*) l_dwErrCode);	// エラー時
	} else {
#ifndef NO_COMPLETE_MSGTXT	/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- { ---------- */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(IDS_CHIF_SREF_COMPLETED);	// Status Bar
#else						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ----------			   */
		((CChiefView*) m_pcChiefView)->m_pcMainFrame->SetMessageText(_T(" "));	// Status Bar
#endif						/* modified 2009.08.05 hmenjo 完了メッセージ表示しない ---------- } ---------- */
//		NotifyComplete((cEventParams*) m_dwAFValid);	// 正常時
		NotifyComplete((cEventParams*) TR_SREF_OK);	// 正常時
	}
}

// bagus 2014.01.10 Add (Stage None対応) <--
