/** INCLUDE *****************************************************
 *                    Copyright (C) 1993 by Tencor Instruments
 *  ................. Source Code Control System (PVCS) keywords
 *
 *  $Header: /ORCA3.2.2J/inc/MOTSYS.H 5     02/10/08 9:52 Hmenjo $
 *
 *  $Revision: 5 $
 *    $Author: Hmenjo $
 *      $Date: 02/10/08 9:52 $
 *   $Modtime: 02/09/20 10:43 $
 *       .Log:...see below...
 *  .............................................................
 *
 *  DESCRIPTION:
 *
 *  CONTENTS:
 *
 *  .....$Log: /ORCA3.2.2J/inc/MOTSYS.H $
//
//  5     02/10/08 9:52 Hmenjo
//  日本語コメントを追加．
//
//  4     02/01/18 16:40 Hmenjo
//  コメントのネストミスを修正．
//
//  3     02/01/11 16:22 Hmenjo
//  日本語コメントを追加．
//
//  1     00/11/01 9:21a Hmenjo
//  orca3.2.2の正常コンパイルのため．
//
 *
 ***************************************************************/
/* change FILENAME to your h file name, excluding file extention */

#if !defined(MOTSYS_INC)
#define MOTSYS_INC

#if defined(__cplusplus)
extern "C" {
#endif


// Any App or Dll which used this driver must include the following
// fragment in their .def file
/*
   IMPORTS
      USER.OPENDRIVER
      USER.CLOSEDRIVER
      USER.SENDDRIVERMESSAGE
*/

/* このメッセージはモーションイベントのために MotsysSetWindow により
   登録されたウィンドウに送られる． */

// for Win32 #define WM_MOTIONMESSAGE	WM_USER + 50




/* この typedef は MotsysMoveAtSpeed で使われる */




/* 以下はドライバに送られるメッセージを定義する． */
/* それらに加えるのが #define である． */
/* 最初の３つは便利にするため用意される． */
//
/* system.ini の[drivers]セクションにあると仮定する． */
// motion=c:\drivers\motsys.drv
//        (or whatever)
//
/* 通常の動作がいくぶんこのように見える */
//
/*
	HDRVR hdrv = MotsysOpen ("motion");
	int num_axes;

   .
   .

	MotsysConfigure (hdrv, hWnd);	  設定ダイアログボックスに表示するために
									   ドライバに知らせる
   .
   .

	num_axes = MotsysInitialize (hdrv);		現在の設定をロードするため
	for (i=0;i<num_axes;i++)
	{
		   違う軸が違うウィンドウに応答するかもしれない
		MotsysSetWindow (hdrv, axis, hWndResponse[i]);
	}
   .
   .

	// do some motions
   .
   .

	MotsysClose (hdrv);

*/

// 指定された軸を使用を宣言し，(軸毎に)応答メッセージ送信先ウィンドウハンドルを設定します．
//		axis		軸番号
//		hwnd		応答メッセージ送信先ウィンドウハンドル
#define MotsysSetWindow(hdrv, axis, hwnd) ((int) ::SendMessage(hdrv, MOTDRV_SETWINDOW, MAKEWPARAM(axis, 0), (LPARAM) hwnd))

// 指定された軸の SoftHome を設定します．
//		axis		軸番号
//		home		SoftHome 位置
#define MotsysSetSoftHome(hdrv, axis, home) ((int) ::SendMessage(hdrv, MOTDRV_SETSOFTHOME, MAKEWPARAM(axis, 0), (LPARAM) home))

// 指定された軸を停止します．
//		axis		軸番号
#define MotsysEmergencyStop(hdrv, axis) ((int) ::SendMessage(hdrv, MOTDRV_EMERGENCYSTOP, MAKEWPARAM(axis, 0), 0L))

// 指定された軸を，指定された速度で移動します．
//		axis		軸番号
//		velocity	速度		＋：＋方向へ移動，－：－方向へ移動
//		mode		(未使用)
#define MotsysMoveAtSpeed(hdrv, axis, velocity, mode) ((int) ::SendMessage(hdrv, MOTDRV_MOVEATSPEED, MAKEWPARAM(axis, velocity), (LPARAM) mode))

// 指定された軸を，指定された速度で，指定された位置へ移動します．
//		axis		軸番号
//		position	位置
//		velocity	速度
#define MotsysMoveToPosition(hdrv, axis, position, velocity) ((int) ::SendMessage(hdrv, MOTDRV_MOVETOPOSITION, MAKEWPARAM(axis, velocity), (LPARAM) position))

// 指定された軸の位置を返します．
//		axis		軸番号
//		p_position	(未使用)
//		戻り値		現在位置
#define MotsysGetPosition(hdrv, axis) ((long) ::SendMessage(hdrv, MOTDRV_GETPOSITION, MAKEWPARAM(axis, 0), 0L))

// 指定された軸を HP へ移動します．
//		原点復帰が，まだであれば原点復帰を行ないます(HP へは移動しません)．
//		axis		軸
#define MotsysGoHome(hdrv, axis) ((int) ::SendMessage(hdrv, MOTDRV_GOHOME, MAKEWPARAM(axis, 0), 0L))

// 指定された軸を，指定された速度で，指定された位置へ移動します．						これは使わないことにします．
//		axis		軸番号
//		velocity	速度
//		destination	位置
#define MotsysMoveAtSpeedToDestination(hdrv, axis, velocity, destination) ((int) ::SendMessage(hdrv, MOTDRV_MOVEATSPEEDTODEST, MAKEWPARAM(axis, velocity), (LPARAM) destination))

// 指定された軸が動作中かを返します．
//		axis		軸番号
//		戻り値		MS_NO_ERROR で停止中
#define MotsysIsStopped(hdrv, axis) ((int) ::SendMessage(hdrv, MOTDRV_ISSTOPPED, MAKEWPARAM(axis, 0), 0L))

// JOYSTICK モードを返します．
//		戻り値		MS_JOYMODE で使用中
#define MotsysIsJoyMode(hdrv) ((int) ::SendMessage(hdrv, MOTDRV_ISJOYMODE, 0, 0))

// JOYSTICK モードを設定します．
//		Mode		0：未使用，1：使用
//		SpeedSel	1：速度１，2：速度２，3：速度３
#define MotsysSetJoyMode(hdrv, Mode, SpeedSel) ((int) ::SendMessage(hdrv, MOTDRV_SETJOYMODE, (WPARAM) Mode, (LPARAM) SpeedSel))

// 最高速度を取得します．
//		axis		軸番号
//		戻り値		最高速度
#define MotsysGetMaxSpeed(hdrv, axis) ((int) ::SendMessage(hdrv, MOTDRV_GETMAXSPEED, MAKEWPARAM(axis, 0), 0L))

// 軸の稼動範囲(距離)を取得します．
//		axis		軸番号
//		戻り値		サイズ
#define MotsysGetMotionSize(hdrv, axis) ((int) ::SendMessage(hdrv, MOTDRV_GETMOTIONSIZE, MAKEWPARAM(axis, 0), 0L))

// 軸の電子ギアを取得します．
//		axis		軸番号
//		戻り値		分母：LOWORD()，分子：HIWORD()
#define MotsysGetElectronicGear(hdrv, axis) ((int) ::SendMessage(hdrv, MOTDRV_GETELECTRONICGEAR, MAKEWPARAM(axis, 0), 0L))

// 軸のソフトリミットを読出します．
//		axis		軸番号
//		signflag	正負の符号(0≦：＋側，＜0：－側)
//		戻り値		リミット値
#define MotsysGetSoftLimit(hdrv, axis, signflag) ((int) ::SendMessage(hdrv, MOTDRV_GETSOFTLIMIT, MAKEWPARAM(axis, signflag), 0L))

// ジョイスティックの速度を設定します．
//		axis		軸番号
//		SpeedSel	1：速度１，2：速度２，3：速度３
//		velocity	速度
//		戻り値		MS_NO_ERROR で設定成功
#define MotsysSetJoySpeed(hdrv, axis, SpeedSel, velocity) ((int) ::SendMessage(hdrv, MOTDRV_SETJOYSPEED, MAKEWPARAM(axis, velocity), SpeedSel))

#if defined(__cplusplus)
}
#endif

#endif

