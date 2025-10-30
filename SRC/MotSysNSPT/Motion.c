/** CODE ********************************************************
 *                        Copyright (C) 1993 by Tencor Instruments
 *  ................. Source Code Control System (PVCS) keywords
 *
 *  NAME:   $Workfile: MOTION.C $
 *
 *  $Revision: 11 $
 *   $Author: Hmenjo $
 *      $Date: 02/01/16 15:36 $
 *  $Modtime: 02/01/16 15:19 $
 *       .Log:...see below...
 *  .............................................................
 *
 *  DESCRIPTION:
 *
 *  CONTENTS:
 *
 *  .....$Log: /ORCA3.2.2J/src/motsys/MOTION.C $
//
//  11    02/01/16 15:36 Hmenjo
//  日本語コメントを追加．
//
//  2     01/03/05 14:23 Hmenjo
//  $NoKeywords: $を追加．
//  コード内のバージョン情報を元に戻す．
//
//  1     00/11/01 9:22a Hmenjo
//  orca3.2.2の正常コンパイルのため．
//
//  $NoKeywords: $
//
 *
 ***************************************************************/

#include <windows.h>
#include "..\\..\\inc\\MotsysMsg.h"
#include "CtrlCmd.h"
#define	MOTION_EXT_ON
#include "MotionDef.h"
#include "Motion.h"
#include <stdio.h>
#include <tchar.h>

#include "FileNameDef.h"


static void	set_current_position(short, long);
static long	get_current_position(short);
static int	motion_command_valid(short);
static void	local_emergency_stop(short axis);


static long  get_current_position(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	long	position;

	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		position = CC_get_true_position(CC_CnvAxisToSPT(axis));
		if (motion_data[axis].bInvertedSense) {
			position = -position;
		}
		break;
	default:
		position = 0;
		break;
	}

	return position;
}

void  send_motion_message(
		short axis,
		short type
	)
{
	HWND hWndSend;

	if ((0 <= axis) && (axis < AXIS_NUM)) {
		hWndSend = motion_data[axis].hWnd;
	} else {
		hWndSend = motion_data[AXIS_BIG_X].hWnd;
	}

	axis = ConvAxis(axis);

	if ((-1 <= axis) && (axis < AXIS_NUM) && (0 != hWndSend)) {
		PostMessage(
				hWndSend,
				WM_MOTIONMESSAGE,
				(WPARAM) type,
				MAKELPARAM(axis, 0)
			);
	}
}

// コマンドを実行可能かどうかチェック
static int	motion_command_valid(
		short axis
	)
{
	if ((-1 == axis) && (AXIS_ALL <= axis)) {
		return MS_PARAMETER_ERROR;
	}
	if (0 != motion_data[axis].uninitialized) {
		return MS_AXIS_UNINITIALIZED;
	}
	if (0 == (g_CC_AxisInfo[CC_CnvAxisToSPT(axis)].Status & 0x0008)) {
		return MS_AXIS_NOT_HOME;
	}
	if (0 != CC_IsJoyMode()) {
		return MS_JOYMODE;
	}

	return MS_NO_ERROR;
}

static void  local_emergency_stop(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		CC_local_emergency_stop(CC_CnvAxisToSPT(axis));
		break;
	default:
		break;
	}
}

int motion_set_response_window(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T
									-1(AXIS_ALL)	：全軸	*/
		HWND hWnd
	)
{
	int i;

	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else if (0 == g_CC_bInitialize) {
			return MS_INITIALIZING;
		} else if (0 == hWnd) {
			return MS_PARAMETER_ERROR;
		} else {
			motion_data[axis].hWnd = hWnd;
			motion_data[axis].uninitialized = FALSE;
		}
		break;
	case AXIS_ALL:
		if (0 == g_CC_bInitialize) {
			return MS_INITIALIZING;
		} else if (0 == hWnd) {
			return MS_PARAMETER_ERROR;
		}
		for (i = 0; i < AXIS_NUM; i++) {
			switch (i) {
			case AXIS_BIG_X:
			case AXIS_BIG_Y:
			case AXIS_Z:
			case AXIS_T:
				if (0 != motion_data[i].bEnable) {
					motion_data[i].hWnd = hWnd;
					motion_data[i].uninitialized = FALSE;
				}
				break;
			}
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}

	return MS_NO_ERROR;
}

int  motion_is_stopped(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	if (0 != CC_IsJoyMode()) {
		return MS_JOYMODE;
	}

	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else if (0 != motion_data[axis].uninitialized) {
			return MS_AXIS_UNINITIALIZED;
		} else {
			if (
				(0 == g_CC_Axis_prc[CC_CnvAxisToSPT(axis)])
			 && (0 == g_CC_Axis_mode[CC_CnvAxisToSPT(axis)])
			 && (0 == g_CC_AxisInfo[CC_CnvAxisToSPT(axis)].Mode)
			 && (0 == CC_IsJoyMode())
				) {
				return MS_NO_ERROR;
			} else {
				return MS_FUNCTION_BUSY;
			}
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
}

int motion_is_joymode(void)
{
	if (0 == g_CC_bInitialize) {
		return MS_AXIS_UNINITIALIZED;
	}

	if (0 != CC_IsJoyMode()) {
		return MS_JOYMODE;
	} else {
		return MS_NO_ERROR;
	}
}

int motion_set_joymode(int iMode, int iSpeedSel)
{
	return CC_SetJoyMode(iMode, iSpeedSel);
}

int motion_emergency_stop(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T
									-1(AXIS_ALL)	：全軸	*/
	)
{
	int i;

	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			// 停止系処理なのでコマンドチェックはしていません．
			local_emergency_stop(axis);
		}
		break;
	case AXIS_ALL:
		for (i = 0; i < AXIS_NUM; i++) {
			switch (i) {
			case AXIS_BIG_X:
			case AXIS_BIG_Y:
			case AXIS_Z:
			case AXIS_T:
				if (0 != motion_data[i].bEnable) {
					// 停止系処理なのでコマンドチェックはしていません．
					local_emergency_stop((short) i);
				}
				break;
			}
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}

	return MS_NO_ERROR;
}


int motion_move_at_speed(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
		short velocity
//		VEL_MODE mode	// 未使用[2009.03.18]
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			// コマンド実行可能かどうかチェック
			int mot_status = motion_command_valid(axis);
			if (
				(mot_status == MS_NO_ERROR)
				) {
				// エラーなし，か，サーボ移動中でビジーなら
				if (0 != motion_data[axis].bInvertedSense) {
					velocity = -velocity;
				}
				mot_status = CC_motion_move_at_speed(CC_CnvAxisToSPT(axis), velocity);
			}
			return mot_status;
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
}

int motion_move_at_speed_to_dest(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
		long  destination,
		short velocity
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			// コマンド実行可能かどうかチェック
			int mot_status = motion_command_valid(axis);
			if (
				(mot_status == MS_NO_ERROR)
				) {
				// エラーなし
				if (0 != motion_data[axis].bInvertedSense) {
					destination = -destination;
				}
				mot_status = CC_motion_move_at_speed_to_dest(CC_CnvAxisToSPT(axis), destination, velocity);
			}
			return mot_status;
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
}

int motion_move_to_position(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
		long position,
		short velocity
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			// コマンド実行可能かどうかチェック
			int mot_status = motion_command_valid(axis);
			if (
				(mot_status == MS_NO_ERROR)
				) {
				// エラーなし
				if (0 != motion_data[axis].bInvertedSense) {
					position = -position;
				}
				mot_status = CC_motion_move_to_position(CC_CnvAxisToSPT(axis), position, velocity);
			}
			return mot_status;
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
}

long motion_get_position(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_GETPOS_PARAMERROR;
		} else if (0 != motion_data[axis].uninitialized) {
			return MS_GETPOS_AXIS_UNINIT;
		} else if (0 == (g_CC_AxisInfo[axis].Status & 0x0008)) {
			return MS_GETPOS_AXIS_NOTHOME;
		} else {
			return get_current_position(axis);
		}
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}
}

int motion_go_home(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	int mot_status;

	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			mot_status = motion_command_valid(axis);
			if (
				(mot_status == MS_NO_ERROR)
			 || (mot_status == MS_AXIS_NOT_HOME)
				) {
				mot_status = CC_motion_go_home(CC_CnvAxisToSPT(axis));
			}
			return mot_status;
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}

	return MS_NO_ERROR;
}

// 軸の HP として新しい論理値をセットする．
int motion_set_soft_home(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
		long  soft_home
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			// コマンド実行可能かどうかチェック
			int mot_status = motion_command_valid(axis);
			if (
				(mot_status == MS_NO_ERROR)
			 || (mot_status == MS_AXIS_NOT_HOME)
				) {
				mot_status = CC_motion_set_soft_home(CC_CnvAxisToSPT(axis), soft_home);
				if (MS_NO_ERROR == mot_status) {
					motion_data[axis].soft_home = soft_home;
				}
			}
			return mot_status;
		}
		break;
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
}

/*
 *	最高速度を取得
 */
int motion_get_maxspeed(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_GETPOS_PARAMERROR;
		} else if (0 != motion_data[axis].uninitialized) {
			return MS_GETPOS_AXIS_UNINIT;
		} else {
			return CC_GetMaxSpeed(CC_CnvAxisToSPT(axis));
		}
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}
}

/*
 *	軸の稼動範囲(距離)を取得
 */
int motion_get_motionsize(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_GETPOS_PARAMERROR;
		} else if (0 != motion_data[axis].uninitialized) {
			return MS_GETPOS_AXIS_UNINIT;
		} else {
			return CC_GetMotionSize(CC_CnvAxisToSPT(axis));
		}
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}
}

/*
 *	軸の電子ギアを取得
 */
int motion_get_electronicgear(
		short axis		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
		if (0 == motion_data[axis].bEnable) {
			return MS_GETPOS_PARAMERROR;
		} else {
			return CC_GetElectronicGear(CC_CnvAxisToSPT(axis));
		}
		break;
	case AXIS_Z:
	case AXIS_T:
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}
}

/*
 *	軸のソフトリミット＋側を取得
 */
int motion_get_softlimit(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY
									２(AXIS_Z)		：Z
									４(AXIS_T)		：T	*/
		short signflag	/* 符号		≦０：＋側取得
									＜０：−側取得*/
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
	case AXIS_Z:
	case AXIS_T:
		if (0 == motion_data[axis].bEnable) {
			return MS_GETPOS_PARAMERROR;
		} else if (0 != motion_data[axis].uninitialized) {
			return MS_GETPOS_AXIS_UNINIT;
		} else {
			return CC_GetSoftLimit(CC_CnvAxisToSPT(axis), signflag);
		}
		break;
	default:
		return MS_GETPOS_PARAMERROR;
		break;
	}
}

/*
 *	ジョイスティックの速度を設定します．
 */
int motion_set_joyspeed(
		short axis,		/* 軸指定	０(AXIS_BIG_X)	：BigX
									１(AXIS_BIG_Y)	：BigY*/
		int iSpeedSel,
		short velocity
	)
{
	switch (axis) {
	case AXIS_BIG_X:
	case AXIS_BIG_Y:
		if (0 == motion_data[axis].bEnable) {
			return MS_PARAMETER_ERROR;
		} else {
			int mot_status;
			mot_status = CC_SetJoySpeed(CC_CnvAxisToSPT(axis), iSpeedSel, velocity);
			return mot_status;
		}
		break;
	case AXIS_Z:
	case AXIS_T:
	default:
		return MS_PARAMETER_ERROR;
		break;
	}
}

/*
 *	設定による軸(BigX/BigY)の入替え
 */
short ConvAxis(short sAxis_org)
{

	short l_sAxis;
	TCHAR l_tszText[256];
	int  l_ichange;

	GetPrivateProfileString(_T("Axis"), _T("ChangeAxisEnable"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	l_ichange = _ttoi(l_tszText);

	l_sAxis = sAxis_org;

	if(0 != l_ichange) {
		switch(l_sAxis) {
			case AXIS_BIG_X:
				l_sAxis = AXIS_BIG_Y;
				break;
			case AXIS_BIG_Y:
				l_sAxis = AXIS_BIG_X;
				break;
			default:
				break;
		}
	}

	return l_sAxis;
}

/*
 *	motion_data を初期化
 */
void InitMotionData(void)
{
	int i;
	TCHAR l_tszText[256];

	for (i = 0; i < AXIS_NUM; i++) {
		motion_data[i].bEnable = FALSE;
		motion_data[i].hWnd = 0;
		motion_data[i].uninitialized = TRUE;
		motion_data[i].bInvertedSense = FALSE;
		motion_data[i].soft_home = 0;
	}

	// X 軸は必ず使用
	motion_data[AXIS_BIG_X].bEnable = TRUE;
	// Y 軸は必ず使用
	motion_data[AXIS_BIG_Y].bEnable = TRUE;
	// Z 軸の使用指定を設定
	GetPrivateProfileString(_T("Axis"), _T("EnableAxisZ"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	motion_data[AXIS_Z].bEnable = (0 == _tcscmp(l_tszText, _T("1")))? TRUE : FALSE;
	// T 軸の使用指定を設定
	GetPrivateProfileString(_T("Axis"), _T("EnableAxisT"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	motion_data[AXIS_T].bEnable = (0 == _tcscmp(l_tszText, _T("1")))? TRUE : FALSE;

	// X 軸の移動方向反転を設定
	GetPrivateProfileString(_T("Axis"), _T("InvertedSenseX"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	motion_data[AXIS_BIG_X].bInvertedSense = (0 == _tcscmp(l_tszText, _T("1")))? TRUE : FALSE;
	// Y 軸の移動方向反転を設定
	GetPrivateProfileString(_T("Axis"), _T("InvertedSenseY"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	motion_data[AXIS_BIG_Y].bInvertedSense = (0 == _tcscmp(l_tszText, _T("1")))? TRUE : FALSE;
	// Z 軸の移動方向反転を設定
	GetPrivateProfileString(_T("Axis"), _T("InvertedSenseZ"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	motion_data[AXIS_Z].bInvertedSense = (0 == _tcscmp(l_tszText, _T("1")))? TRUE : FALSE;
	// T 軸の移動方向反転を設定
	GetPrivateProfileString(_T("Axis"), _T("InvertedSenseT"), _T("0"), l_tszText, sizeof(l_tszText), SERVO_INI);
	motion_data[AXIS_T].bInvertedSense = (0 == _tcscmp(l_tszText, _T("1")))? TRUE : FALSE;
}
