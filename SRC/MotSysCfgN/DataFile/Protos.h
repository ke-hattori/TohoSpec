/** INCLUDE *****************************************************
 *                    Copyright (C) 1993 by Tencor Instruments
 *  ................. Source Code Control System (PVCS) keywords
 *
 *  NAME:   $Workfile: PROTOS.H $
 *
 *  $Revision: 2 $
 *    $Author: Hmenjo $
 *      $Date: 01/03/05 14:26 $
 *   $Modtime: 01/03/05 14:26 $
 *       .Log:...see below...
 *  .............................................................
 *
 *  DESCRIPTION:
 *
 *  CONTENTS:
 *
 *  .....$Log: /ORCA3.2.2J/src/motsys/PROTOS.H $
//
//  2     01/03/05 14:26 Hmenjo
//  $NoKeywords$を追加．
//  コード内のバージョン情報を元に戻す．
//
//  1     00/11/01 9:22a Hmenjo
//  orca3.2.2の正常コンパイルのため．
//
//  $NoKeywords: $
//
//     Rev 1.2   12 Apr 1996 10:56:44   RLAMB
//  fixed problems in CIB interrupt code
 *
 *     Rev 1.1   01 Apr 1994 09:40:40   Lynn Chuu (LC)
 *  Added motion_accurate_move.
 *
 *     Rev 1.0   30 Aug 1993 17:25:42   Lynn Chuu (LC)
 *  Initial revision.
 *
 ***************************************************************/
/* change FILENAME to your h file name, excluding file extention */
//static char ORCA_PVCSID_PROTOS_H[] = "$Header:   O:/orca/src/motsys/vcs/protos.hv_   1.2   12 Apr 1996 10:56:44   RLAMB  $";


extern WORD MOTION_INT_VECTOR;
extern WORD MOTION_INT_MASK;
extern WORD MOTION_INT_8259_MASK_REG;
extern WORD wIrq;


typedef enum on_or_offtag {
	off,
	on
} on_or_off;

typedef struct MOT_MSGtag {
	short	mmType;
	short	nAxis;		// the axis on which motion event occurred
						//	= -1 if motion failure by red button
	short	nAxisPwm;	// pwm value that caused motion failure
						// 0 - if timer int or red button motor failure
} MOT_MSG;

//typedef MOT_MSG __far* LPMOT_MSG;
typedef MOT_MSG *LPMOT_MSG;
//typedef WORD __far* LPMOT_DATA;
typedef WORD *LPMOT_DATA;


/* Defined here are the structures which contain the motor control parameters
   required for control and tracking of the operation of each axis of motion.
   These parameters are divided into two groups:

         axis definition - those that define the physical aspects of the axis.
                           These are fixed values that will not change during
                           operation of the axis.  They can only be changed at
                           axis configuration time (set in motor_initialize).
         axis operation - those that define the current mode of operation of
                           the axis.  These values may change due to
                           variations in the mechanics of the axis and the
                           operation being performed.  They can be changed
                           at any time by the controlling program (using
                           motor_set_parameters).  The motion system has the
                           capability to update these values in non-volatile
                           memory (NVM) as they change during operation.
*/

#pragma pack(push, 1)
typedef struct MOT_PARAMStag {
	short	nLowGain;			// K value during trapezoidal moves
	short	nHighGain;			// K value for position hold after moves
	short	nZero;
	short	nPole;				// A, B - values for the digital filter
	short	nSampleTime;		// T - the filter's sample period (HCTL reg R0F)
	BYTE	byPwmMax;			// pwm safety limit to trigger motor overload
	BYTE	byPwmFilterCount;	// consecutive counts > limit to trigger failure
	short	nMode;				// operation mode of the axis - values are
	LONG	lDestination;		// destination location for the axis
	short	nMaxVelocity;		// max velocity for trapezoidal moves
	short	nIntegVelocity;		// command (integral) velocity
	short	nPropVelocity;		// command (proportional) velocity
	short	nAcceleration;		// current acceleration
} MOT_PARAMS;
#pragma pack(pop)

//typedef MOT_PARAMS __far* LPMOT_PARAMS;
typedef MOT_PARAMS *LPMOT_PARAMS;


/***************************************************************************
*
*    default motion parameters - these are sent to the motor system
*    initialization routine
*
***************************************************************************/

#pragma pack(push, 1)
typedef struct MOT_DEF_PARAMStag {
	BYTE	byStatusReg;		// status reg value for HCTL chip
	BYTE	byUnused;			// needed for alignment
	LONG	lMinusLimit;
	LONG	lPlusLimit;			// from Home
// BOOL は int で定義されている
// Win32 では int は 32 ビットなので 16 ビットに変更する
	short	bInvertedSense;		// true -> motions & positions inverted
	short	bForwardForHome;	// true -> go forward to home if home sensor is
//	BOOL	bInvertedSense;		// true -> motions & positions inverted
//	BOOL	bForwardForHome;	// true -> go forward to home if home sensor is
								//	 on low side of home
	short	nHomeVelocity;		// high velocity to do a home move
	short	nHomeAccel;			// acceleration to do a home move
	LPMOT_PARAMS	lpDefaultMotParams;
} MOT_DEF_PARAMS;
#pragma pack(pop)

//typedef MOT_DEF_PARAMS __far* LPMOT_DEF_PARAMS;
typedef MOT_DEF_PARAMS *LPMOT_DEF_PARAMS;



typedef struct MOT_AXIS_DATAtag {
	WORD	wHctlPort;		// HCTL アドレス
	WORD	wHomePort;		// 原点センサアドレス
	WORD	wHomeIntPort;	// 原点センサ割込アドレス
	WORD	wXstopPort;		// 停止指令アドレス
	BYTE	byHomeMask;		// 原点センサビット位置
	BYTE	byHomeIntMask;	// 原点センサ割込ビット
	BYTE	byXstopMask;	// 停止指令ビット
	BYTE	byUnused;		// 未使用
} MOT_AXIS_DATA;
//typedef MOT_AXIS_DATA __far* LPMOT_AXIS_DATA;
typedef MOT_AXIS_DATA *LPMOT_AXIS_DATA;


#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************
*
*       motion_set_parameters
*
*       will set the motion parameters for the requested
*       axis to the values specified in params.  params is assumed to point to
*       a table in non volatile memory (NVM).  This pointer is saved for later
*       updates of motor system values in case a power loss is encountered.
*       If a power loss occurs, these saved values can then be restored with
*       this call and the operation resumed using motor_restart.  A null params
*       value will leave the parameters unchanged and disable further automatic
*       updates of those parameters in NVM.  A value of 0 for axis is invalid.
*
****************************************************************************/
int motion_set_parameters(
		short	axis,
		LPMOT_PARAMS p_params
	);

#ifdef __cplusplus
}
#endif

/***************************************************************************
*
*      motion_initialize
*
*      this routine will initialize the motor system for the axis specified.
*      Motor parameters will be set to the values specified in params.
*      home_reached will be cleared and idle mode entered for the axis.
*      A value of -1 for axis will initialize all axes to the same values.
*      Note: this routine must be called prior to any others at power_up or
*            while axis is in motor failure safety mode.
*
***************************************************************************/
int motion_initialize(
		short	axis,
		LPMOT_DEF_PARAMS	p_default_params,
		LPMOT_PARAMS		p_default_mot_p
	);

int motion_add_axis(
		short	axis,
//		MOT_AXIS_DATA __far* p_axis_data);
		MOT_AXIS_DATA	*p_axis_data
	);

void motion_terminate(void);

void motion_initialize_data(WORD timer_delay);

WORD motion_get_home_velocity(short axis);

void motion_set_home_velocity(short axis, short velocity);

void motion_enable_pwm_monitor(short axis, short mot_frac);

void do_motion_isr(void);

#ifdef __cplusplus
extern "C" {
#endif

short motion_move_accurate(short axis, long destination);
int motion_aux_control(short axis, on_or_off flag, BYTE bit_id);
void motion_unmask_motion_int(void);
void motion_mask_motion_int(void);
int motion_lock(short axis, HWND hWnd);
BOOL motion_in_home_flag(short axis);
int  motion_is_stopped(short axis);
int motion_unlock(short axis, HWND hWnd);
int motion_acquire_data(short axis, short time, LPWORD *p_data);
int motion_speed_dest_reached(short axis);
int motion_move_at_speed_to_dest(short axis, long destination, short velocity);
int motion_move_pwm(short axis, short pwm);
int motion_set_acceleration(short axis, short acceleration);
int motion_go_home(short axis);
int motion_get_position(short axis, long *p_position);
int motion_move_distance(short axis, long distance, short velocity);
int motion_move_to_position(short axis, long position, short velocity);
int motion_move_at_speed(short axis, short velocity, VEL_MODE mode);
int motion_emergency_stop(short axis);
int motion_resume(short axis);
int motion_stop(short axis);
int motion_set_soft_home(short axis, long soft_home);
int motion_set_response_window(short axis, HWND hWnd);

#ifdef __cplusplus
}
#endif

extern void DoMIIsr(void);


