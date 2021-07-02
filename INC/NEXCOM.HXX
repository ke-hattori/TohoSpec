#if !defined( _NEXCOM_HXX_ )
#define _NEXCOM_HXX_

#pragma once

#include <globals.hxx>

#ifdef __NEXTRA_INTERNAL

// #########################################################################
// DEFINITIONS
// #########################################################################

#define NEXTRA_SOFT_VERSION 	"Ver1.00"

#define T1_TIMEOUT				(500L)
#define T2_TIMEOUT				(2000L)
#define T3_TIMEOUT				(5000L)

#define CRCHAR					TCHAR('\r') 	/* carriage return */
#define LFCHAR					TCHAR('\n') 	/* line feed */
#define NULLCHAR				TCHAR('\0')
#define SPACECHAR				TCHAR(' ')
#define TABCHAR 				TCHAR('\t')
#define DQUOTECHAR				TCHAR('\"')
#define SLASHCHAR				TCHAR('\\')
#define COLONCHAR				TCHAR(':')
#define COMMACHAR				TCHAR(',')
#define DOTCHAR 				TCHAR('.')
#define STXCHAR 				TCHAR('\x02')
#define ACKCHAR 				TCHAR('\x06')
#define NAKCHAR 				TCHAR('\x15')

#define CR						TEXT("\r")
#define LF						TEXT("\n")
#define CRLF					TEXT("\r\n")
#define SPACE					TEXT(" ")
#define TAB 					TEXT("\t")
#define SLASH					TEXT("\\")
#define BLANK					TEXT("")
#define STX 					TEXT("\x02")
#define ACK 					TEXT("\x06")
#define NAK 					TEXT("\x15")
#define LBRACKET				TEXT("[")
#define RBRACKET				TEXT("]")

#define CRLF_LENGTH 			(2)

#define MAX_BUFFER_LENGTH		(16)

#define SERR_SUCCESS			(0L)
#define SERR_INVALID_HANDLE 	(1L)
#define SERROR_READ_FAULT		(2L)
#define SERR_TIMEOUT			(3L)
#define SERR_BUFFER_OVERFLOW	(4L)
#define SERR_BAD_FORMAT 		(5L)
#define SERR_WRITE_FAULT		(6L)
#define SERR_WAIT_FAILURE		(7L)

#define NEXTRA_LOGFILE_NAME 	LOG_DIR "NEXTRA.LOG"
#define NEXADAX_LOGFILE_NAME	LOG_DIR "NEXADAX.LOG"
#define NEXSIO_LOGFILE_NAME 	LOG_DIR "NEXSIO.LOG"
#define NEXIO_LOGFILE_NAME		LOG_DIR "NEXIO.LOG"
#define NEXPROC_LOGFILE_NAME	LOG_DIR "NEXPROC.LOG"
#define NEXINI_INIFILE_NAME 	CFG_DIR "NEXINI.INI"
#define NEXALARM_LOGFILE_NAME	LOG_DIR "NEXALARM.LOG"
#define NEXALARM_INIFILE_NAME	CFG_DIR "ALARMLIST.INI"
#define VACUUM_CFGFILE_NAME 	CFG_DIR "VACCFG.DAT"
#define CLAMP_CFGFILE_NAME		CFG_DIR "CLAMPCFG.DAT"
#define NEXIO_TXTFILE_NAME		CFG_DIR "NEXIO.TXT"

#ifndef countof
	#define countof(array) (sizeof(array)/sizeof(array[0]))
#endif

// ===========================================================================
// ALARM DEFINITIONS
// ===========================================================================
#define NEXALID 							(1000)

#define NEXALID_DioOpenError				(0	+ NEXALID)
#define NEXALID_EmergencyStop				(1	+ NEXALID)
#define NEXALID_MainPowerOnError			(2	+ NEXALID)
#define NEXALID_RcInitializeFailure 		(7	+ NEXALID)
#define NEXALID_RcMotionTimeouts			(8	+ NEXALID)
#define NEXALID_RcMotionFailure 			(9	+ NEXALID)
#define NEXALID_ClampTimeouts				(10 + NEXALID)
#define NEXALID_StageIsNotLoadPosError		(11 + NEXALID)
#define NEXALID_VacuumIsNotOffError 		(12 + NEXALID)
#define NEXALID_GlassIsExistsError			(13 + NEXALID)
#define NEXALID_VacuumOnTimeouts			(14 + NEXALID)
#define NEXALID_VacuumOffTimeouts			(15 + NEXALID)
#define NEXALID_RobotArmDetected			(16 + NEXALID)
#define NEXALID_ShutterOpenTimeouts 		(17 + NEXALID)
#define NEXALID_ShutterCloseTimeouts		(18 + NEXALID)
#define NEXALID_ServoAlarm					(50 + NEXALID)
#define NEXALID_ServoOnFailure				(51 + NEXALID)
#define NEXALID_ServoInitializeFailure		(52 + NEXALID)
#define NEXALID_PortInitializeFailure		(53 + NEXALID)
#define NEXALID_CommunicationFailure		(54 + NEXALID)
#define NEXALID_EstabCommunicationError 	(55 + NEXALID)
#define NEXALID_ServoNotReady				(56 + NEXALID)
#define NEXALID_ServoNotInitialized 		(57 + NEXALID)
#define NEXALID_MotionTimeouts				(58 + NEXALID)

#endif // __NEXTRA_INTERNAL

#endif // _NEXCOM_HXX_
