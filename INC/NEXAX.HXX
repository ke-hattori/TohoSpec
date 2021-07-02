#if !defined( _NEXAX_HXX_ )
#define _NEXAX_HXX_

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NEXAXDLL_ )
	#define NEXAXAPI __declspec(dllimport)
#else
	#define NEXAXAPI __declspec(dllexport)
#endif

enum nexax_axis_enum
{
	nexax_axis_1 = 0,
	nexax_axis_2,
	nexax_axis_max,
};

enum
{
	FA22_PLS = 0,
	FA22_Pro,
	FA22_oP,
	FA22_Unknown
};

enum motion_direction_enum {
	motion_dir_forward = 0,
	motion_dir_reverse,
	motion_dir_max
};

enum location_enum {
	loc_unknown = -1,
	loc_upper = 0,
	loc_alignment = 1,
	loc_lower = 2,
	loc_alignment_decel = 3,
	loc_max
};

enum remote_command_enum {
	rcmd_teach_loc = 0,
	rcmd_home,
	rcmd_softhome,
	rcmd_normal_jog,
	rcmd_motion_stop,
	rcmd_params_item,
	rcmd_curr_pos,
	rcmd_eeprom_data,
	rcmd_alarm_reset
};

enum eeprom_params_enum {
	eeprom_upper_pos = 0,
	eeprom_alignment_pos = 1,
	eeprom_lower_pos = 2,
	eeprom_alignment_decel_pos = 3,
	eeprom_upper_speed,
	eeprom_alignment_speed,
	eeprom_lower_speed,
	eeprom_alignment_decel_speed,
	eeprom_teach_accel_decel,
	eeprom_home_speed,
	eeprom_home_accel_decel,
	eeprom_softhome_speed,
	eeprom_detect_range,
	eeprom_jog_speed,
	eeprom_alarm_id,
	eeprom_io_monitor,
	eeprom_status,
	eeprom_max
};

static const char* eeprom_params_names [] = {
	"upper position",
	"alignment position",
	"lower position",
	"alignment decel position",
	"upper position speed",
	"alignment position speed",
	"lower position speed",
	"decel for alignment pos speed",
	"move absolute accel and decel time",
	"homing speed",
	"homing accel and decel time",
	"soft home position speed",
	"detect range",
	"pulse jogging speed",
	"alarm id",
	"i/o monitor",
	"servo status",
};

struct adax_data_t {
	union {
		long	  value;
		struct {
			short low;
			short high;
		};
	};
};

struct motion_params_t {
	struct {
		long		pos[loc_max];
		adax_data_t speed[loc_max];
		float		acc;
		float		dec;
	} loc;
	struct {
		adax_data_t speed;
		float		acc;
		float		dec;
	} home;
	adax_data_t 	softhome_speed;
	short			detect_range;
	adax_data_t 	jog_speed;
};

#define NEXAXALARMOFFSETAXIS2					(50)

#if !defined( NEXAXALARMSET )
#define nexaxAlarmSet(alid, axis) \
{ \
	switch ( axis ) { \
	case nexax_axis_1: \
		nexifAlarmSet(alid); \
		break; \
	case nexax_axis_2: \
		nexifAlarmSet(alid + NEXAXALARMOFFSETAXIS2); \
		break; \
	default: \
		break; \
	} \
}
#endif
// =========================================================================
// NEXAX ERROR CODE
// =========================================================================
#define NEXAXERR_NONE							(0x00)
#define NEXAXERR_CANCELED						(0x01)
#define NEXAXERR_MOTION_TIMEOUTS				(0x02)
#define NEXAXERR_NOT_INITIALIZED				(0x03)
#define NEXAXERR_NOT_READY						(0x04)
#define NEXAXERR_INVALID_PARAMETER				(0x05)
#define NEXAXERR_INITIALIZE_FAILURE 			(0x06)
#define NEXAXERR_COMMUNICATION_FAILURE			(0x07)
#define NEXAXERR_MOTION_FAILURE 				(0x08)

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
NEXAXAPI unsigned long __stdcall nexaxInitialize(void);
NEXAXAPI unsigned long __stdcall nexaxFinalize(void);
NEXAXAPI unsigned long __stdcall nexaxCancel(void);
NEXAXAPI unsigned long __stdcall nexaxWaitForMotionFinished(unsigned short axis);
NEXAXAPI unsigned long __stdcall nexaxWaitForHomeMotionFinished(unsigned short axis);
NEXAXAPI unsigned long __stdcall nexaxMoveToUpperPos(unsigned short axis, BOOL wait = FALSE);
NEXAXAPI unsigned long __stdcall nexaxMoveToAlignmentPos(unsigned short axis, BOOL wait = FALSE);
NEXAXAPI unsigned long __stdcall nexaxMoveToLowerPos(unsigned short axis, BOOL wait = FALSE);
NEXAXAPI unsigned long __stdcall nexaxMoveToHomePos(unsigned short axis);
NEXAXAPI unsigned long __stdcall nexaxMoveToSoftHomePos(unsigned short axis);
NEXAXAPI unsigned long __stdcall nexaxNormalJog(unsigned short axis, short speed);
NEXAXAPI unsigned long __stdcall nexaxPulseJog(unsigned short axis, long pls);
NEXAXAPI unsigned long __stdcall nexaxMotionStop(unsigned short axis);

NEXAXAPI unsigned long __stdcall nexaxGetCurrPos(unsigned short axis, long& pos);
NEXAXAPI unsigned long __stdcall nexaxUploadMotionParams(unsigned short axis, motion_params_t* params);
NEXAXAPI unsigned long __stdcall nexaxDownloadMotionParams(unsigned short axis, motion_params_t* params);

NEXAXAPI BOOL __stdcall nexaxIsInitialized(unsigned short axis);
NEXAXAPI BOOL __stdcall nexaxIsIdle(unsigned short axis);
NEXAXAPI BOOL __stdcall nexaxIsReady(unsigned short axis);

#ifdef __cplusplus
}
#endif

#endif // _NEXAX_HXX_
