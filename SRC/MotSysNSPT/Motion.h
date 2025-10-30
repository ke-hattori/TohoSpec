
/*
 *	Motion.c ‚ÌŠO•”’è‹`
 */

#ifdef MOTION_EXT_ON
#define	_MOT_EXT
#else
#define	_MOT_EXT	extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

int motion_set_response_window(short axis, HWND hWnd);
int motion_set_soft_home(short axis, long soft_home);
int motion_emergency_stop(short axis);
int motion_is_stopped(short axis);
int motion_move_at_speed(short axis, short velocity);
int motion_move_to_position(short axis, long position, short velocity);
long motion_get_position(short axis);
int motion_go_home(short axis);
int motion_move_at_speed_to_dest(short axis, long destination, short velocity);
short ConvAxis(short sAxis_org);
void InitMotionData(void);
int motion_is_joymode(void);
int motion_set_joymode(int iMode, int iSpeedSel);
int motion_get_maxspeed(short axis);
int motion_get_motionsize(short axis);
int motion_get_electronicgear(short axis);
int motion_get_softlimit(short axis, short signflag);
int motion_set_joyspeed(short axis, int iSpeedSel, short velocity);

void send_motion_message(short axis, short type);


_MOT_EXT	MOT_DATA motion_data[AXIS_NUM];
_MOT_EXT	int g_iDlgCtrlID[50][AXIS_NUM + 1];

#ifdef __cplusplus
}
#endif
