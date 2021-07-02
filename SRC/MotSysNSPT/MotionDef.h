
/*
 *	motion_data の構造定義
 */

typedef struct {
	BOOL			bEnable;			// 軸 有効フラグ
	HWND			hWnd;				// 応答送信先のウィンドウハンドル
	BOOL			uninitialized;		// 未初期化フラグ
	BOOL			bInvertedSense;		// 移動方向反転フラグ
	long			soft_home;			// ソフト HP
} MOT_DATA;
