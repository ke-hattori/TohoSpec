
/*
 *	Motion System Driver 用 ウィンドウメッセージ
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MotMsgON

		UINT MOTDRV_SHOWINDOW			= RegisterWindowMessage("MOTDRV_SHOWINDOW");	/*	ウィンドウを表示にする */
		UINT MOTDRV_HIDEINDOW			= RegisterWindowMessage("MOTDRV_HIDEINDOW");	/*	ウィンドウを非表示にする */
// 下記のメッセージは "motsys.h" を参照のこと
		UINT WM_MOTIONMESSAGE			= RegisterWindowMessage("WM_MOTIONMESSAGE");
		UINT MOTDRV_SETWINDOW			= RegisterWindowMessage("MOTDRV_SETWINDOW");
		UINT MOTDRV_SETSOFTHOME			= RegisterWindowMessage("MOTDRV_SETSOFTHOME");
		UINT MOTDRV_EMERGENCYSTOP		= RegisterWindowMessage("MOTDRV_EMERGENCYSTOP");
		UINT MOTDRV_MOVEATSPEED			= RegisterWindowMessage("MOTDRV_MOVEATSPEED");
		UINT MOTDRV_MOVETOPOSITION		= RegisterWindowMessage("MOTDRV_MOVETOPOSITION");
		UINT MOTDRV_GETPOSITION			= RegisterWindowMessage("MOTDRV_GETPOSITION");
		UINT MOTDRV_GOHOME				= RegisterWindowMessage("MOTDRV_GOHOME");
		UINT MOTDRV_MOVEATSPEEDTODEST	= RegisterWindowMessage("MOTDRV_MOVEATSPEEDTODEST");
		UINT MOTDRV_ISSTOPPED			= RegisterWindowMessage("MOTDRV_ISSTOPPED");
		UINT MOTDRV_ISJOYMODE			= RegisterWindowMessage("MOTDRV_ISJOYMODE");
		UINT MOTDRV_SETJOYMODE			= RegisterWindowMessage("MOTDRV_SETJOYMODE");
		UINT MOTDRV_GETMAXSPEED			= RegisterWindowMessage("MOTDRV_GETMAXSPEED");
		UINT MOTDRV_GETMOTIONSIZE		= RegisterWindowMessage("MOTDRV_GETMOTIONSIZE");
		UINT MOTDRV_GETELECTRONICGEAR	= RegisterWindowMessage("MOTDRV_GETELECTRONICGEAR");
		UINT MOTDRV_GETSOFTLIMIT		= RegisterWindowMessage("MOTDRV_GETSOFTLIMIT");
		UINT MOTDRV_SETJOYSPEED			= RegisterWindowMessage("MOTDRV_SETJOYSPEED");
#else

extern	UINT MOTDRV_SHOWINDOW;				/*	ウィンドウを表示にする */
extern	UINT MOTDRV_HIDEINDOW;				/*	ウィンドウを非表示にする */
// 下記のメッセージは "motsys.h" を参照のこと
extern	UINT WM_MOTIONMESSAGE;
extern	UINT MOTDRV_SETWINDOW;
extern	UINT MOTDRV_SETSOFTHOME;
extern	UINT MOTDRV_EMERGENCYSTOP;
extern	UINT MOTDRV_MOVEATSPEED;
extern	UINT MOTDRV_MOVETOPOSITION;
extern	UINT MOTDRV_GETPOSITION;
extern	UINT MOTDRV_GOHOME;
extern	UINT MOTDRV_MOVEATSPEEDTODEST;
extern	UINT MOTDRV_ISSTOPPED;
extern	UINT MOTDRV_ISJOYMODE;
extern	UINT MOTDRV_SETJOYMODE;
extern	UINT MOTDRV_GETMAXSPEED;
extern	UINT MOTDRV_GETMOTIONSIZE;
extern	UINT MOTDRV_GETELECTRONICGEAR;
extern	UINT MOTDRV_GETSOFTLIMIT;
extern	UINT MOTDRV_SETJOYSPEED;

#endif

#ifdef __cplusplus
}
#endif


/* ８軸以上はありません */
#define	AXIS_BIG_X		0	// BigX
#define	AXIS_BIG_Y		1	// BigY
#define	AXIS_Z			2	// Z
#define	AXIS_SLOW_X		3	// Slow X
#define	AXIS_T			4	// Theta
#define	AXIS_L			5	// Level
#define	AXIS_FAST_X		6	// Fast X
#define	AXIS_ALL		-1	// 全軸指定
#define	AXIS_NUM		8	// 軸数


/* wParam は以下の１つである */
#define  MM_DONE		   20	// 軸の正常停止
#define  MM_STOPPED 	   21	// 軸の強制停止(エラー等ではありません)
#define  MM_FAILURE 	   22	// 軸の異常終了
/* added 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- { ---------- */
#define  MM_TIMEOUT 	   23	/* 軸の移動タイムアウト	*/
/* added 2009.11.11 hmenjo MotSys 軸タイムアウト ---------- } ---------- */


/*
 *	リターンコード
 */
#define MS_NO_ERROR				0			/* 要求されたとおりの実行だった */
#define MS_NO_FUNCTION			1			/* システムに実装されていない機能 */
#define MS_NO_HARDWARE			2			/* 想定したハードウェアが実装されていない */
#define MS_PARAMETER_ERROR		5			/* パラメタ範囲か値のエラー */
#define MS_FUNCTION_BUSY		6			/* 許可されない呼出し - ビジー */
#define MS_INSUFFICIENT_MEMORY	7			/* 呼出しでのメモリ確保が失敗(未使用) */
#define MS_AXIS_NOT_HOME		8			/* 原点復帰済でないと呼出しが無効 */
#define MS_AXIS_UNINITIALIZED	11			/* 初期化されていない */
#define MS_AXIS_LOCKED			14			/* ロックされている，						(未使用)
											   軸がロックされているのに動作要求，
											   不適当なタスクからアンロックされた */
#define	MS_INITIALIZING			16			// 現在初期化中です
#define	MS_JOYMODE				17			// JOYSTICK モード中です
#define	MS_GETPOS_PARAMERROR	0x7fffffff	// パラメタエラー(MOTDRV_GETPOSITION 用)
#define	MS_GETPOS_AXIS_UNINIT	0x7ffffffe	// 未初期化(MOTDRV_GETPOSITION 用)
#define	MS_GETPOS_AXIS_NOTHOME	0x7ffffffd	// 未原点復帰(MOTDRV_GETPOSITION 用)
