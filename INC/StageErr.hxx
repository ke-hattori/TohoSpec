#if !defined( _STAGEERR_HXX_ )
#define _STAGEERR_HXX_

#define STAGE_ERR_INVALID_DATA					(-4)
#define STAGE_ERR_INVALID_PORT					(-201)
#define STAGE_ERR_COMMUNICATION_FAILURE 		(-207)
#define STAGE_ERR_STAGE_INIT_FAILURE			(-251)
#define STAGE_ERR_STAGE_NOT_FOUND				(-252)
#define STAGE_ERR_INVALID_STAGE 				(-253)
#define STAGE_ERR_XSENSOR						(-254)
#define STAGE_ERR_YSENSOR						(-255)
#define STAGE_ERR_MOTION_ABORTED				(-256)
#define STAGE_ERR_MOTION_FAILURE				(-257)
#define STAGE_ERR_AUTOFOCUS_INIT_FAILURE		(-281)
#define STAGE_ERR_AUTOFOCUS_NOT_FOUND			(-282)
#define STAGE_ERR_INVALID_AUTOFOCUS 			(-283)
#define STAGE_ERR_AUTOFOCUS_MOTION_FAILURE		(-284)
#define STAGE_ERR_AUTOFOCUS_MOTION_ABORTED		(-285)
#define STAGE_ERR_ZAXIS_MOTION_FAILURE			(-286)
#define STAGE_ERR_INVALID_AUTOFOCUSTURRET		(-287)
#define STAGE_ERR_COVER_OPENED					(-288)
#define STAGE_ERR_TURRET_INIT_FAILURE			(-291)
#define STAGE_ERR_TURRET_NOT_FOUND				(-292)
#define STAGE_ERR_AUTOFOCUSTURRET_INIT_FAILURE	(-293)
#define STAGE_ERR_EDGE_ALIGNER_FAILURE			(-314)
#define STAGE_ERR_ALIGNER_NOT_FOUND 			(-315)

#define STAGE_ERR_INVALID_ACCESS				IOBASE_ERR_INVALID_ACCESS
#define STAGE_ERR_NONE							IOBASE_ERR_NONE

#if 1	// added hmenjo 20090417 ---------- { -----
#define	STAGE_ERR_MS_BASE						(1000)
#define	STAGE_ERR_MS_NONE_MOTSYS				(-(STAGE_ERR_MS_BASE +	 1))	// MotSys が見つかりません．
#define	STAGE_ERR_MS_NONE_MOTTSK				(-(STAGE_ERR_MS_BASE +	 2))	// MotTsk が見つかりません．
#define	STAGE_ERR_MS_NOT_SERVO_ON				(-(STAGE_ERR_MS_BASE +	 3))	// サーボオンしていません．
#define	STAGE_ERR_MS_INIT_TIMEOUT				(-(STAGE_ERR_MS_BASE +	 4))	// 初期化完了待ちでタイムアウト
#define	STAGE_ERR_MS_MOTION_TIMEOUT				(-(STAGE_ERR_MS_BASE +	 5))	// 移動完了待ちでタイムアウト
#define	STAGE_ERR_MS_MOTION_FORCE				(-(STAGE_ERR_MS_BASE +	 6))	// 移動完了待ちで強制停止
#define	STAGE_ERR_MS_MOTION_FAIL				(-(STAGE_ERR_MS_BASE +	 7))	// 移動完了待ちで異常停止
#define	STAGE_ERR_MS_NO_FUNCTION				(-(STAGE_ERR_MS_BASE + 101))	/* システムに実装されていない機能 */
#define	STAGE_ERR_MS_NO_HARDWARE				(-(STAGE_ERR_MS_BASE + 102))	// 想定したハードウェアが実装されていない
#define	STAGE_ERR_MS_PARAMETER_ERROR			(-(STAGE_ERR_MS_BASE + 105))	// パラメタ範囲か値のエラー
#define	STAGE_ERR_MS_FUNCTION_BUSY				(-(STAGE_ERR_MS_BASE + 106))	// 許可されない呼出し - ビジー
#define	STAGE_ERR_MS_INSUFFICIENT_MEMORY		(-(STAGE_ERR_MS_BASE + 107))	// 呼出しでのメモリ確保が失敗				(未使用)
#define	STAGE_ERR_MS_AXIS_NOT_HOME				(-(STAGE_ERR_MS_BASE + 108))	// 原点復帰済でないと呼出しが無効
#define	STAGE_ERR_MS_AXIS_UNINITIALIZED			(-(STAGE_ERR_MS_BASE + 111))	// 初期化されていない
#define	STAGE_ERR_MS_AXIS_LOCKED				(-(STAGE_ERR_MS_BASE + 114))	// ロックされている，						(未使用)
#define	STAGE_ERR_MS_INITIALIZING				(-(STAGE_ERR_MS_BASE + 116))	// 現在初期化中です
#define	STAGE_ERR_MS_JOYMODE					(-(STAGE_ERR_MS_BASE + 117))	// JOYSTICK モード中です
#define	STAGE_ERR_MS_GETPOS_PARAMERROR			(0x7fffffff)					// パラメタエラー(MOTDRV_GETPOSITION 用)
#define	STAGE_ERR_MS_GETPOS_AXIS_UNINIT			(0x7ffffffe)					// 未初期化(MOTDRV_GETPOSITION 用)
#define	STAGE_ERR_MS_GETPOS_AXIS_NOTHOME		(0x7ffffffd)					// 未原点復帰(MOTDRV_GETPOSITION 用)
#define	STAGE_ERR_MS_GETPOS_NO_FUNCTION			(0x7ffffffc)					/* システムに実装されていない機能(MOTDRV_GETPOSITION 用) */
#define	STAGE_ERR_MS_UNDEFINE_ERR				(-(STAGE_ERR_MS_BASE + 999))	// 未定義のエラーです．
#endif	// added hmenjo 20090417 ---------- } -----

#endif // _STAGEERR_HXX_
