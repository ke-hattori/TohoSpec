#if !defined( _NEXINI_HXX_ )
#define _NEXINI_HXX_

#pragma once

#include <nexmisc.hxx>
#include <profile.hxx>

using namespace nano;

#ifdef __NEXTRA_INTERNAL

//enum { LANGUAGE_ENGLISH = 0, LANGUAGE_JAPANESE };
// matsuhisa 2011.09.13 modified >>>
//enum { NEXTRA_TYPEA = 0, NEXTRA_TYPEB, NEXTRA_TYPE_MAX };
// 2013.11.07 Bagus Mod (TohoSpec‘Î‰ž) -->
//Saiki 20130812 Modified Ver.4.7.6----->
//enum { NEXTRA_TYPEA = 0, NEXTRA_TYPEB, NEXTRA_TYPEC, NEXTRA_TYPED, NEXTRA_TYPE_MAX };
//Saiki 20130812 Modified Ver.4.7.6<-----
enum { NEXTRA_TYPEA = 0, NEXTRA_TYPEB, NEXTRA_TYPEC, NEXTRA_TYPED, NEXTRA_TYPEE, NEXTRA_TYPE_MAX };
// 2013.11.07 Bagus Mod (TohoSpec‘Î‰ž) <--
// matsuhisa 2011.09.13 modified <<<
enum { CYLINDER_2POS = 2, CYLINDER_3POS };

#pragma pack(push, 2)
// Parameter Ini File
typedef struct tagNEXINIPARAMS {
	struct {
		unsigned short	Language;
		unsigned char	Type;
		unsigned char	Mode;
		BOOL			VacuumFeedback;
		BOOL			ConfirmOperator;
		BOOL			RobotArmInterlock;
		int 			CmdShow;
// matsuhisa 2011.11.12 added >>>
		int				FirstValve;
// matsuhisa 2011.11.12 added <<<
	} System;
	struct dio_config_t Dio;
	struct sio_config_t Sio[2];
	struct {
		unsigned long Homing;
		unsigned long Origin;
		unsigned long TeachLoc;
		unsigned long WorkGuide;
		unsigned long VacuumOn;
		unsigned long VacuumOff;
		unsigned long ShutterOpen;
		unsigned long ShutterClose;
	} MotionTimeouts;
	struct {
		unsigned long WorkGuideOpen;
		unsigned long WorkGuideClose;
		unsigned long VacuumDestruct;
		unsigned long ServoOn;
	} MotionDelay;
	struct {
		unsigned char HardwareSimulation;
		unsigned long Resolution;
		unsigned long TimeoutsT1;
		unsigned long TimeoutsT2;
		unsigned long TimeoutsT3;
	} ADAX;
	struct {
		unsigned char  Cylinder;
		BOOL		   InitialClose;
	} Shutter;
	struct {
		unsigned char InitialPosNo;
		struct {
			unsigned long ServoOn;
			unsigned long Homing;
			unsigned long TeachPos;
		} Timeouts;
	} RoboCylinder;
	struct {
		BOOL Shutter;
		BOOL AirFloat;
		BOOL RoboCylinder;
	} Options;
	struct {
		int ExistCount;
	} GlassExist;
//Saiki 20130306 Modified ----->
	struct {
		int VacCount;
	} VacuumPressure;
//Saiki 20130306 Modified <-----
} NEXINIPARAMS, *PNEXINIPARAMS;
#pragma pack(pop)

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NEXINIDLL_ )
	#define NEXINIAPI __declspec(dllimport)
#else
	#define NEXINIAPI __declspec(dllexport)
#endif

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
NEXINIAPI void __stdcall nexiniGetParameter(PNEXINIPARAMS pParams);
NEXINIAPI void __stdcall nexiniSetParameter(PNEXINIPARAMS pParams);

#ifdef __cplusplus
}
#endif

#endif // __NEXTRA_INTERNAL

#endif // _NEXINI_HXX_
