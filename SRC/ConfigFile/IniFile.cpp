#include "stdafx.h"
// 2013.11.07 Bagus Mod (TohoSpecｿｽﾎ会ｿｽ) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpecｿｽﾎ会ｿｽ) <--
#include <windows.h>
#include <string>
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\StageInt.hxx"
#include "..\\..\\INC\\ConfigFile.hxx"
#include "IniFile.h"
#include "..\..\INC\ts_secure_crt.h"
/* added 2009.07.07 hmenjo dll ｿｽｿｽｿｽﾎパｿｽXｿｽﾎ会ｿｽ ConfigFile.dll ---------- { ---------- */
#include <tchar.h>
extern TCHAR g_tszProcDir[_MAX_PATH];		/* ｿｽﾄ出ｿｽｿｽｿｽvｿｽｿｽｿｽZｿｽXｿｽﾌデｿｽBｿｽｿｽｿｽNｿｽgｿｽｿｽ('\'ｿｽtｿｽｿｽ)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* ｿｽ準ｿｽfｿｽBｿｽｿｽｿｽNｿｽgｿｽｿｽ('\'ｿｽtｿｽｿｽ)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
/* added 2009.07.07 hmenjo dll ｿｽｿｽｿｽﾎパｿｽXｿｽﾎ会ｿｽ ConfigFile.dll ---------- } ---------- */

USER_DATA			UserData;
NANOSPEC_SYSTEM_DATA NanoSysData;

//Saiki 20110207 Delete ----->
//#define	PIN_INTERVAL		7
//Saiki 20110207 Delete <-----

// 2009.09.29 bagus SE --{--
static double lg_TrShutterMoveWaitTime;
// 2009.09.29 bagus SE --}--

// << ini file >>
// [Section Name]
// (key name)=***
// (key name)=***
//	.
//	.
//
// <data type>
// 'b' = boolean
// 'B' = BYTE
// 'S' = short
// 'I' = integer
// 'L' = long
// 'F' = float
// 'D' = double
// 'W' = WORD
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --{--
// 'E' = XXXe+YYY
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --}--
// 'C' = character (ｿｽﾅ托ｿｽｿｽ256)

// 2009.05.14 k-matsuo -->
// '#' = double		// data type 'D'ｿｽﾌ撰ｿｽｿｽxｿｽgｿｽｿｽ	double ｿｽｿｽｿｽx10ｿｽｿｽｿｽﾅ保托ｿｽ
// 2009.05.14 k-matsuo <--

// SystemCommon.iniｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ
const INIFILE SystemCommonIniDesc[] =
{
//	Section Name					Key Name						Type	Default					Variable Pointer
// System Config
	{"SYSTEM CONFIG",				"SR",							'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bSR										},
	{"",							"SE",							'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bSE										},
// 2013.02.01 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	{"",							"COMPEASE",						'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bCompEASE								},
// 2013.02.01 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
//	{"",							"IRSE",							'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bIRSE									},
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
//	{"",							"4PP",							'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bResist									},
	{"",							"Resistance",					'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bResist									},
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--
	{"",							"ContactAngle",					'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bCTA										},
	{"",							"Stress",						'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bStress									},
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	{"",							"MS",							'b',	"FALSE",				&NanoSysData.SystemConfig.HeadType.bMS										},
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--
	{"",							"StageType",					'I',	"0",					&NanoSysData.SystemConfig.nStageType										},
	{"",							"VibrationControlUnit",			'I',	"0",					&NanoSysData.SystemConfig.nVibrationControlUnit								},
	{"",							"VibrationDownSensor",			'b',	"FALSE",				&NanoSysData.SystemConfig.bVibrationDownSensor								},
	{"",							"SampleExistenceSensor",		'b',	"FALSE",				&NanoSysData.SystemConfig.bSampleExistenceSensor							},
	{"",							"SampleLifter",					'I',	"0",					&NanoSysData.SystemConfig.nSampleLifter										},
	{"",							"SampleAligner",				'b',	"FALSE",				&NanoSysData.SystemConfig.bSampleAligner									},
	{"",							"VacuumType",					'I',	"0",					&NanoSysData.SystemConfig.nVacuumType										},
	{"",							"VacuumTimeout",				'I',	"5000",					&NanoSysData.SystemConfig.iVacuumTimeout									},
	{"",							"Correct Angle",				'D',	"0",					&NanoSysData.SystemConfig.dCorrectAngle										},
	{"",							"Capture Board",				'I',	"0",					&NanoSysData.SystemConfig.nCaptureBoard										},
	{"",							"Pattern Rec Method",			'I',	"0",					&NanoSysData.SystemConfig.nPRMethod											},
	{"",							"Sample Pattern Rec",			'b',	"FALSE",				&NanoSysData.SystemConfig.bSamplePatternRec									},
	{"",							"Site Pattern Rec",				'b',	"FALSE",				&NanoSysData.SystemConfig.bSitePatternRec									},
	{"",							"Host",							'b',	"FALSE",				&NanoSysData.SystemConfig.bHost												},
	{"",							"SignalTower",					'b',	"FALSE",				&NanoSysData.SystemConfig.bSignalTower										},
	{"",							"Language",						'I',	"0",					&NanoSysData.SystemConfig.nLanguage											},
	{"",							"Multi Recipe",					'b',	"FALSE",				&NanoSysData.SystemConfig.bMultiRecipe										},
	{"",							"Extra View Camera",			'b',	"FALSE",				&NanoSysData.SystemConfig.bExtraViewCamera									},
/* added 2009.06.16 hmenjo Loading Shutter ｿｽgｿｽpｿｽLｿｽｿｽｿｽﾇ会ｿｽ ---------- { ---------- */
	{"",							"UseLoadingShutter",			'b',	"FALSE",				&NanoSysData.SystemConfig.bUseLoadingShutter								},
/* added 2009.06.16 hmenjo Loading Shutter ｿｽgｿｽpｿｽLｿｽｿｽｿｽﾇ会ｿｽ ---------- } ---------- */
/* deleted 2009.07.03 hmenjo Seq ｿｽｿｽｿｽ闔橸ｿｽtｿｽBｿｽｿｽｿｽ^ｿｽﾅ抵ｿｽ@ｿｽ\ ｿｽｿｽｿｽｿｽ ---------- { ---------- */
///* added 2009.07.02 hmenjo Seq ｿｽｿｽｿｽ闔橸ｿｽtｿｽBｿｽｿｽｿｽ^ｿｽﾅ抵ｿｽ@ｿｽ\ ---------- { ---------- */
//	{"",							"UseOptFilterMeasPgmAF",		'b',	"FALSE",				&NanoSysData.SystemConfig.bUseOptFilterMeasPgmAF							},
///* added 2009.07.02 hmenjo Seq ｿｽｿｽｿｽ闔橸ｿｽtｿｽBｿｽｿｽｿｽ^ｿｽﾅ抵ｿｽ@ｿｽ\ ---------- } ---------- */
/* deleted 2009.07.03 hmenjo Seq ｿｽｿｽｿｽ闔橸ｿｽtｿｽBｿｽｿｽｿｽ^ｿｽﾅ抵ｿｽ@ｿｽ\ ｿｽｿｽｿｽｿｽ ---------- } ---------- */
// 2009.10.29 bagus Jog ｿｽﾇ会ｿｽｿｽCｿｽｿｽ --{--
	{"",							"JoystickType",					'I',	"1",					&NanoSysData.SystemConfig.nJoystickType										},
// 2009.10.29 bagus Jog ｿｽﾇ会ｿｽｿｽCｿｽｿｽ --}--
// 2009.12.10 bagus Recipe Backup --{--
	{"",							"Backup",						'b',	"FALSE",				&NanoSysData.SystemConfig.bBackup											},
// 2009.12.10 bagus Recipe Backup --}--
	{"",							"DisableNextra",				'b',	"FALSE",				&NanoSysData.SystemConfig.bDisableNextra									},


// Original Position
	{"ORIGINAL POSITION",			"BaseHeadType",					'W',	"0",					&NanoSysData.OriginalPosition.wBaseHeadType									},
	{"",							"SR_X",							'L',	"0",					&NanoSysData.OriginalPosition.LocSr.lX										},
	{"",							"SR_Y",							'L',	"0",					&NanoSysData.OriginalPosition.LocSr.lY										},
	{"",							"SR_Z",							'L',	"0",					&NanoSysData.OriginalPosition.LocSr.lZ										},
	{"",							"SE_X",							'L',	"0",					&NanoSysData.OriginalPosition.LocSe.lX										},
	{"",							"SE_Y",							'L',	"0",					&NanoSysData.OriginalPosition.LocSe.lY										},
	{"",							"SE_Z",							'L',	"0",					&NanoSysData.OriginalPosition.LocSe.lZ										},
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
#if 0
	{"",							"IRSE_X",						'L',	"0",					&NanoSysData.OriginalPosition.LocIrse.lX									},
	{"",							"IRSE_Y",						'L',	"0",					&NanoSysData.OriginalPosition.LocIrse.lY									},
	{"",							"IRSE_Z",						'L',	"0",					&NanoSysData.OriginalPosition.LocIrse.lZ									},
#else
	{"",							"MS_X",						'L',	"0",					&NanoSysData.OriginalPosition.LocMs.lX											},
	{"",							"MS_Y",						'L',	"0",					&NanoSysData.OriginalPosition.LocMs.lY											},
	{"",							"MS_Z",						'L',	"0",					&NanoSysData.OriginalPosition.LocMs.lZ											},
#endif
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
//	{"",							"4PP_X",						'L',	"0",					&NanoSysData.OriginalPosition.Loc4PP.lX										},
//	{"",							"4PP_Y",						'L',	"0",					&NanoSysData.OriginalPosition.Loc4PP.lY										},
//	{"",							"4PP_Z",						'L',	"0",					&NanoSysData.OriginalPosition.Loc4PP.lZ										},
	{"",							"RS_X",							'L',	"0",					&NanoSysData.OriginalPosition.Loc4PP.lX										},
	{"",							"RS_Y",							'L',	"0",					&NanoSysData.OriginalPosition.Loc4PP.lY										},
	{"",							"RS_Z",							'L',	"0",					&NanoSysData.OriginalPosition.Loc4PP.lZ										},
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--
// 2009.10.07 bagus CTA ｿｽLｿｽ[ｿｽｿｽｿｽﾌ変更 --{--
//	{"",							"ContactAngle_X",				'L',	"0",					&NanoSysData.OriginalPosition.LocContactAngle.lX							},
//	{"",							"ContactAngle_Y",				'L',	"0",					&NanoSysData.OriginalPosition.LocContactAngle.lY							},
//	{"",							"ContactAngle_Z",				'L',	"0",					&NanoSysData.OriginalPosition.LocContactAngle.lZ							},
	{"",							"CA_X",							'L',	"0",					&NanoSysData.OriginalPosition.LocContactAngle.lX							},
	{"",							"CA_Y",							'L',	"0",					&NanoSysData.OriginalPosition.LocContactAngle.lY							},
	{"",							"CA_Z",							'L',	"0",					&NanoSysData.OriginalPosition.LocContactAngle.lZ							},
// 2009.10.07 bagus CTA --}--
	{"",							"Stress_X",						'L',	"0",					&NanoSysData.OriginalPosition.LocStress.lX									},
	{"",							"Stress_Y",						'L',	"0",					&NanoSysData.OriginalPosition.LocStress.lY									},
	{"",							"Stress_Z",						'L',	"0",					&NanoSysData.OriginalPosition.LocStress.lZ									},

// Stage Config Setting
	{"STAGE CONFIG",				"MachineOrgSensorLoc",			'I',	"3",					&NanoSysData.StageConfig.MachineOrgSensorLoc							   },
	{"",							"OriginPosition",				'I',	"1",					&NanoSysData.StageConfig.OrgPos 										   },
	{"",							"CoordinateDirection_X",		'I',	"1",					&NanoSysData.StageConfig.Dir.X											   },
	{"",							"CoordinateDirection_Y",		'I',	"2",					&NanoSysData.StageConfig.Dir.Y											   },
	{"",							"Size_X",						'D',	"900",					&NanoSysData.StageConfig.Size.dX										   },
	{"",							"Size_Y",						'D',	"900",					&NanoSysData.StageConfig.Size.dY										   },
	{"",							"Edge_X",						'D',	"0",					&NanoSysData.StageConfig.Edge.dX										   },
	{"",							"Edge_Y",						'D',	"0",					&NanoSysData.StageConfig.Edge.dY										   },
	{"",							"OriginCoordinate_X",			'L',	"0",					&NanoSysData.StageConfig.Origin.lX										   },
	{"",							"OriginCoordinate_Y",			'L',	"0",					&NanoSysData.StageConfig.Origin.lY										   },
	{"",							"StageMaxCoordLeft_X",			'D',	"0",					&NanoSysData.StageConfig.StageMaxCoord.dLeftX							   },
	{"",							"StageMaxCoordRight_X", 		'D',	"0",					&NanoSysData.StageConfig.StageMaxCoord.dRightX							   },
	{"",							"StageMaxCoordUp_Y",			'D',	"0",					&NanoSysData.StageConfig.StageMaxCoord.dUpY 							   },
	{"",							"StageMaxCoordDown_Y",			'D',	"0",					&NanoSysData.StageConfig.StageMaxCoord.dDownY							   },
// 2013.01.17 bagus rotate xy view -->
	{"",							"RotateXyView",					'I',	"0",					&NanoSysData.StageConfig.RotateXyView									   },
// 2013.01.17 bagus rotate xy view <--

// Focus Configuration
	{"FOCUS CONFIGURATION", 		"Encoder",						'b',	"FALSE",				&NanoSysData.FocusConfig.Enc.bValid 									   },
	{"",							"EncoderDivValue",				'D',	"0",					&NanoSysData.FocusConfig.Enc.dDiv										   },
	{"",							"EncoderOffset",				'D',	"0",					&NanoSysData.FocusConfig.Enc.dOffset									   },
	{"",							"OffsetControl",				'b',	"FALSE",				&NanoSysData.FocusConfig.bOffsetCtrl									   },

// Edge Aligner
	{"EDGE ALIGNER",				"Start_X",						'L',	"0",					&NanoSysData.EdgeAligner.Start.lX										   },
	{"",							"Start_Y",						'L',	"0",					&NanoSysData.EdgeAligner.Start.lY										   },
	{"",							"End_X",						'L',	"0",					&NanoSysData.EdgeAligner.End.lX 										   },
	{"",							"End_Y",						'L',	"0",					&NanoSysData.EdgeAligner.End.lY 										   },
	{"",							"SensorX1_X",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.X[0].lX 								   },
	{"",							"SensorX1_Y",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.X[0].lY 								   },
	{"",							"SensorX2_X",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.X[1].lX 								   },
	{"",							"SensorX2_Y",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.X[1].lY 								   },
	{"",							"SensorY1_X",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.Y[0].lX 								   },
	{"",							"SensorY1_Y",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.Y[0].lY 								   },
	{"",							"SensorY2_X",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.Y[1].lX 								   },
	{"",							"SensorY2_Y",					'L',	"0",					&NanoSysData.EdgeAligner.Sensor.Y[1].lY 								   },

// Handler Load Position
	{"HANDLER LOAD POSITION",		"X",							'L',	"0",					&NanoSysData.HandlerLoadPosition.lX 									   },
	{"",							"Y",							'L',	"0",					&NanoSysData.HandlerLoadPosition.lY 									   },

// General Position
	{"GENERAL POSITION",			"X1",							'L',	"0",					&NanoSysData.GeneralPosition[0].Loc.lX									   },
	{"",							"Y1",							'L',	"0",					&NanoSysData.GeneralPosition[0].Loc.lY									   },
	{"",							"Comment1", 					'C',	"", 					NanoSysData.GeneralPosition[0].szComment								   },
	{"",							"X2",							'L',	"0",					&NanoSysData.GeneralPosition[1].Loc.lX									   },
	{"",							"Y2",							'L',	"0",					&NanoSysData.GeneralPosition[1].Loc.lY									   },
	{"",							"Comment2", 					'C',	"", 					NanoSysData.GeneralPosition[1].szComment								   },
	{"",							"X3",							'L',	"0",					&NanoSysData.GeneralPosition[2].Loc.lX									   },
	{"",							"Y3",							'L',	"0",					&NanoSysData.GeneralPosition[2].Loc.lY									   },
	{"",							"Comment3", 					'C',	"", 					NanoSysData.GeneralPosition[2].szComment								   },
	{"",							"X4",							'L',	"0",					&NanoSysData.GeneralPosition[3].Loc.lX									   },
	{"",							"Y4",							'L',	"0",					&NanoSysData.GeneralPosition[3].Loc.lY									   },
	{"",							"Comment4", 					'C',	"", 					NanoSysData.GeneralPosition[3].szComment								   },
	{"",							"X5",							'L',	"0",					&NanoSysData.GeneralPosition[4].Loc.lX									   },
	{"",							"Y5",							'L',	"0",					&NanoSysData.GeneralPosition[4].Loc.lY									   },
	{"",							"Comment5", 					'C',	"", 					NanoSysData.GeneralPosition[4].szComment								   },
	{"",							"X6",							'L',	"0",					&NanoSysData.GeneralPosition[5].Loc.lX									   },
	{"",							"Y6",							'L',	"0",					&NanoSysData.GeneralPosition[5].Loc.lY									   },
	{"",							"Comment6", 					'C',	"", 					NanoSysData.GeneralPosition[5].szComment								   },
	{"",							"X7",							'L',	"0",					&NanoSysData.GeneralPosition[6].Loc.lX									   },
	{"",							"Y7",							'L',	"0",					&NanoSysData.GeneralPosition[6].Loc.lY									   },
	{"",							"Comment7", 					'C',	"", 					NanoSysData.GeneralPosition[6].szComment								   },
	{"",							"X8",							'L',	"0",					&NanoSysData.GeneralPosition[7].Loc.lX									   },
	{"",							"Y8",							'L',	"0",					&NanoSysData.GeneralPosition[7].Loc.lY									   },
	{"",							"Comment8", 					'C',	"", 					NanoSysData.GeneralPosition[7].szComment								   },
	{"",							"X9",							'L',	"0",					&NanoSysData.GeneralPosition[8].Loc.lX									   },
	{"",							"Y9",							'L',	"0",					&NanoSysData.GeneralPosition[8].Loc.lY									   },
	{"",							"Comment9", 					'C',	"", 					NanoSysData.GeneralPosition[8].szComment								   },
	{"",							"X10",							'L',	"0",					&NanoSysData.GeneralPosition[9].Loc.lX									   },
	{"",							"Y10",							'L',	"0",					&NanoSysData.GeneralPosition[9].Loc.lY									   },
	{"",							"Comment10",					'C',	"", 					NanoSysData.GeneralPosition[9].szComment								   },

// Signal Tower
	{"SIGNAL TOWER",				"InitializeRed",				'I',	"0",					&NanoSysData.SignalTower.Initialize.iRed								   },
	{"",							"InitializeYellow", 			'I',	"0",					&NanoSysData.SignalTower.Initialize.iYellow 							   },
	{"",							"InitializeGreen",				'I',	"0",					&NanoSysData.SignalTower.Initialize.iGreen								   },
	{"",							"InitializeBuzzer", 			'I',	"0",					&NanoSysData.SignalTower.Initialize.iBuzzer 							   },
	{"",							"IdleRed",						'I',	"0",					&NanoSysData.SignalTower.Idle.iRed										   },
	{"",							"IdleYellow",					'I',	"0",					&NanoSysData.SignalTower.Idle.iYellow									   },
	{"",							"IdleGreen",					'I',	"0",					&NanoSysData.SignalTower.Idle.iGreen									   },
	{"",							"IdleBuzzer",					'I',	"0",					&NanoSysData.SignalTower.Idle.iBuzzer									   },
	{"",							"RuiRed",						'I',	"0",					&NanoSysData.SignalTower.Run.iRed										   },
	{"",							"RuiYellow",					'I',	"0",					&NanoSysData.SignalTower.Run.iYellow									   },
	{"",							"RuiGreen", 					'I',	"0",					&NanoSysData.SignalTower.Run.iGreen 									   },
	{"",							"RuiBuzzer",					'I',	"0",					&NanoSysData.SignalTower.Run.iBuzzer									   },
	{"",							"AlarmRed", 					'I',	"0",					&NanoSysData.SignalTower.Alarm.iRed 									   },
	{"",							"AlarmYellow",					'I',	"0",					&NanoSysData.SignalTower.Alarm.iYellow									   },
	{"",							"AlarmGreen",					'I',	"0",					&NanoSysData.SignalTower.Alarm.iGreen									   },
	{"",							"AlarmBuzzer",					'I',	"0",					&NanoSysData.SignalTower.Alarm.iBuzzer									   },

// Host IP Address(My Address to connection CIM-PC)
	{"NANOPIFSOCK",					"SocketAddress",				'C',	"0.0.0.0",				&NanoSysData.HostAddress.szMyIpAddress									   },
	{"",							"Port", 						'I',	"5001", 				&NanoSysData.HostAddress.nPort											   },
	{"",							"NanoPifSockWindowTitle",		'C',	"NanoPifSock",			NanoSysData.HostAddress.szNanoPifSockWindowTitle						   },

// Com Setting
	{"HEADCOM_SR",					"CommNo",						'C',	"COM3", 			 	&NanoSysData.ComSetting.SrComPort.szCommNo								   },
	{"",							"BaudRate", 	  				'C',	"230400",	  		 	&NanoSysData.ComSetting.SrComPort.szBaudRate				   			   },
	{"",							"Parity",						'C',	"N",	 			 	&NanoSysData.ComSetting.SrComPort.szParity						   	  	   },
	{"",							"ByteSize", 	  				'C',	"8",	 			 	&NanoSysData.ComSetting.SrComPort.szByteSize						   	   },
	{"",							"StopBits", 	  				'C',	"1",   		  		 	&NanoSysData.ComSetting.SrComPort.szStopBits						   	   },
	{"HEADCOM_RS",					"CommNo",						'C',	"COM7", 			 	&NanoSysData.ComSetting.RsComPort.szCommNo							   	   },
	{"",							"BaudRate", 	  				'C',	"19200",	 		 	&NanoSysData.ComSetting.RsComPort.szBaudRate						   	   },
	{"",							"Parity",						'C',	"N",	 			 	&NanoSysData.ComSetting.RsComPort.szParity							   	   },
	{"",							"ByteSize", 	  				'C',	"8",	 			 	&NanoSysData.ComSetting.RsComPort.szByteSize						   	   },
	{"",							"StopBits", 	  				'C',	"1",   		  		 	&NanoSysData.ComSetting.RsComPort.szStopBits						   	   },
	{"HEADCOM_CA",					"CommNo",						'C',	"COM4", 			 	&NanoSysData.ComSetting.CaComPort.szCommNo							   	   },
	{"",							"BaudRate", 	  				'C',	"9600", 			 	&NanoSysData.ComSetting.CaComPort.szBaudRate						   	   },
	{"",							"Parity",						'C',	"N",	 			 	&NanoSysData.ComSetting.CaComPort.szParity							   	   },
	{"",							"ByteSize", 	  				'C',	"8",	 			 	&NanoSysData.ComSetting.CaComPort.szByteSize						   	   },
	{"",							"StopBits", 	  				'C',	"1",   		  		 	&NanoSysData.ComSetting.CaComPort.szStopBits						   	   },
	{"HEADCOM_STRESS",				"CommNo",						'C',	"COM4", 			 	&NanoSysData.ComSetting.StressComPort.szCommNo							   },
	{"",							"BaudRate", 	  				'C',	"115200",	  		 	&NanoSysData.ComSetting.StressComPort.szBaudRate						   },
	{"",							"Parity",						'C',	"N",	 			 	&NanoSysData.ComSetting.StressComPort.szParity							   },
	{"",							"ByteSize", 	  				'C',	"8",	 			 	&NanoSysData.ComSetting.StressComPort.szByteSize						   },
	{"",							"StopBits", 	  				'C',	"1",   		  		 	&NanoSysData.ComSetting.StressComPort.szStopBits						   },

// 2013.01.11 bagus jog direction -->
	{"JOG DIRECTION",				"SwapXyDir", 					'b',	"FALSE",				&NanoSysData.JogDirSetting.bSwapXyDir									   },
	{"",							"ChangeXSign", 					'b',	"FALSE",				&NanoSysData.JogDirSetting.bChangeXSign									   },
	{"",							"ChangeYSign", 					'b',	"FALSE",				&NanoSysData.JogDirSetting.bChangeYSign									   },
// 2013.01.11 bagus jog direction <--
};


// SystemSR.iniｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ
const INIFILE SystemSrIniDesc[] =
{
//	Section Name					Key Name						Type	Default 				Variable Pointer
	{"CONFIG",						"HeadType", 					'I',	"0",					&NanoSysData.SrConfig.nHeadType 										   },
	{"",							"IntergrationTime", 			'D',	"1",					&NanoSysData.SrConfig.dIntegrationTime									   },
	{"",							"HeadInterface",				'I',	"0",					&NanoSysData.SrConfig.nHeadInterface									   },
	{"",							"SecondIntergrationTime",		'D',	"1",					&NanoSysData.SrConfig.dSecondIntegrationTime							   },
// 2014.04.04 bagus wavelength step added -->
	{"",							"WavelengthStepMode",			'I',	"0",					&NanoSysData.SrConfig.nWavelengthStep									   },
// 2014.04.04 bagus wavelength step added <--
	{"",							"Turret",						'I',	"0",					&NanoSysData.SrConfig.nTurret											   },
	{"",							"AutoFocus",					'b',	"FALSE",				&NanoSysData.SrConfig.bAutoFocus										   },
	{"",							"ChooseAfFuncInReference",		'b',	"FALSE",				&NanoSysData.SrConfig.bChooseAfFuncInReference							   },
	{"",							"AutoFilter",					'b',	"FALSE",				&NanoSysData.SrConfig.bAutoFilter										   },
/* added 2009.07.03 hmenjo Seq ｿｽｿｽｿｽ闔橸ｿｽtｿｽBｿｽｿｽｿｽ^ｿｽﾅ抵ｿｽ@ｿｽ\ ｿｽｿｽｿｽｿｽ ---------- { ---------- */
	{"",							"UseOptFilterMeasPgmAF",		'b',	"FALSE",				&NanoSysData.SrConfig.bUseOptFilterMeasPgmAF							   },
/* added 2009.07.03 hmenjo Seq ｿｽｿｽｿｽ闔橸ｿｽtｿｽBｿｽｿｽｿｽ^ｿｽﾅ抵ｿｽ@ｿｽ\ ｿｽｿｽｿｽｿｽ ---------- } ---------- */
	{"",							"Transmittance",				'b',	"FALSE",				&NanoSysData.SrConfig.bTransmittance									   },
	{"",							"Built-inReference",			'b',	"FALSE",				&NanoSysData.SrConfig.bBuiltInReference 								   },
	{"",							"CIEMeasurement",				'b',	"FALSE",				&NanoSysData.SrConfig.bCieMeasurement									   },
	{"",							"ODMeasurement",				'b',	"FALSE",				&NanoSysData.SrConfig.bOdMeasurement									   },
	{"",							"UV",							'b',	"FALSE",				&NanoSysData.SrConfig.bUv												   },
	{"",							"UVGain",						'D',	"1",					&NanoSysData.SrConfig.dUvGain											   },
	{"",							"UVAutoShutter",				'b',	"FALSE",				&NanoSysData.SrConfig.bUvAutoShutter									   },
/* added 2009.09.07 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- { ---------- */
	{"",							"TrUseTurretNo",				'W',	"0",					&NanoSysData.SrConfig.iTrUseLensNo											},
// 2009.09.29 bagus SE --{--
//	{"",							"TrShutterMoveWaitTime",		'W',	"0",					&NanoSysData.SrConfig.wTrShutterMoveWaitTime								},
	{"",							"TrShutterMoveWaitTime",		'D',	"0.5",					&lg_TrShutterMoveWaitTime													},
// 2009.09.29 bagus SE --}--
	{"",							"GantryType",					'b',	"FALSE",				&NanoSysData.SrConfig.bGantryType											},
	{"",							"GantryNumberOfMeasurePoint",	'W',	"0",					&NanoSysData.SrConfig.wGantryNumOfMeasPoint									},
/* added 2009.09.07 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- } ---------- */
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽS) ---------- { ---------- */
	{"",							"DefaultMeasureTurretNo",		'W',	"0",					&NanoSysData.SrConfig.iDefaultMeasLensNo									},
	{"",							"DefaultMeasureFilter",			'W',	"0",					&NanoSysData.SrConfig.wDefaultMeasFilter									},
// 2009.10.13 bagus Distance ｿｽﾇ会ｿｽ --{--
	{"",							"Distance",						'b',	"FALSE",				&NanoSysData.SrConfig.bDistance												},
// 2009.10.13 bagus Distance ｿｽﾇ会ｿｽ --}--
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽS) ---------- } ---------- */
	{"FILTER",						"Filter1_Enable",				'b',	"FALSE",				&NanoSysData.SrFilter[0].bEnable										   },
	{"",							"Filter1_Name", 				'C',	"", 					NanoSysData.SrFilter[0].szName											   },
	{"",							"Filter1_WavelengthStart",		'W',	"380",					&NanoSysData.SrFilter[0].Wavelength.wStart								   },
	{"",							"Filter1_WavelengthEnd",		'W',	"900",					&NanoSysData.SrFilter[0].Wavelength.wEnd								   },
	{"",							"Filter2_Enable",				'b',	"FALSE",				&NanoSysData.SrFilter[1].bEnable										   },
	{"",							"Filter2_Name", 				'C',	"", 					NanoSysData.SrFilter[1].szName											   },
	{"",							"Filter2_WavelengthStart",		'W',	"380",					&NanoSysData.SrFilter[1].Wavelength.wStart								   },
	{"",							"Filter2_WavelengthEnd",		'W',	"900",					&NanoSysData.SrFilter[1].Wavelength.wEnd								   },
	{"",							"Filter3_Enable",				'b',	"FALSE",				&NanoSysData.SrFilter[2].bEnable										   },
	{"",							"Filter3_Name", 				'C',	"", 					NanoSysData.SrFilter[2].szName											   },
	{"",							"Filter3_WavelengthStart",		'W',	"380",					&NanoSysData.SrFilter[2].Wavelength.wStart								   },
	{"",							"Filter3_WavelengthEnd",		'W',	"900",					&NanoSysData.SrFilter[2].Wavelength.wEnd								   },
	{"TURRET",						"Turret1_Enable",				'b',	"FALSE",				&NanoSysData.SrTurret[0].bEnable										   },
	{"",							"Turret1_Name", 				'C',	"", 					NanoSysData.SrTurret[0].szName											   },
	{"",							"Turret1_XMPAngle", 			'D',	"0",					&NanoSysData.SrTurret[0].dXmpAngle										   },
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- { ---------- */
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- { ---------- */
//	{"",							"Turret1_Analysis",				'b',	"FALSE",				&NanoSysData.SrTurret[0].bAnalysis											},
//	{"",							"Turret1_AFTurret",				'W',	"FALSE",				&NanoSysData.SrTurret[0].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ----------			   */
	{"",							"Turret1_Analysis",				'b',	"TRUE",					&NanoSysData.SrTurret[0].bAnalysis											},
	{"",							"Turret1_AFTurret",				'W',	"9999",					&NanoSysData.SrTurret[0].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- } ---------- */
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- } ---------- */
	{"",							"Turret2_Enable",				'b',	"FALSE",				&NanoSysData.SrTurret[1].bEnable										   },
	{"",							"Turret2_Name", 				'C',	"", 					NanoSysData.SrTurret[1].szName											   },
	{"",							"Turret2_XMPAngle", 			'D',	"0",					&NanoSysData.SrTurret[1].dXmpAngle										   },
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- { ---------- */
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- { ---------- */
//	{"",							"Turret2_Analysis",				'b',	"FALSE",				&NanoSysData.SrTurret[1].bAnalysis											},
//	{"",							"Turret2_AFTurret",				'W',	"FALSE",				&NanoSysData.SrTurret[1].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ----------			   */
	{"",							"Turret2_Analysis",				'b',	"TRUE",					&NanoSysData.SrTurret[1].bAnalysis											},
	{"",							"Turret2_AFTurret",				'W',	"9999",					&NanoSysData.SrTurret[1].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- } ---------- */
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- } ---------- */
	{"",							"Turret3_Enable",				'b',	"FALSE",				&NanoSysData.SrTurret[2].bEnable										   },
	{"",							"Turret3_Name", 				'C',	"", 					NanoSysData.SrTurret[2].szName											   },
	{"",							"Turret3_XMPAngle", 			'D',	"0",					&NanoSysData.SrTurret[2].dXmpAngle										   },
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- { ---------- */
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- { ---------- */
//	{"",							"Turret3_Analysis",				'b',	"FALSE",				&NanoSysData.SrTurret[2].bAnalysis											},
//	{"",							"Turret3_AFTurret",				'W',	"FALSE",				&NanoSysData.SrTurret[2].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ----------			   */
	{"",							"Turret3_Analysis",				'b',	"TRUE",					&NanoSysData.SrTurret[2].bAnalysis											},
	{"",							"Turret3_AFTurret",				'W',	"9999",					&NanoSysData.SrTurret[2].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- } ---------- */
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- } ---------- */
	{"",							"Turret4_Enable",				'b',	"FALSE",				&NanoSysData.SrTurret[3].bEnable										   },
	{"",							"Turret4_Name", 				'C',	"", 					NanoSysData.SrTurret[3].szName											   },
	{"",							"Turret4_XMPAngle", 			'D',	"0",					&NanoSysData.SrTurret[3].dXmpAngle										   },
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- { ---------- */
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- { ---------- */
//	{"",							"Turret4_Analysis",				'b',	"FALSE",				&NanoSysData.SrTurret[3].bAnalysis											},
//	{"",							"Turret4_AFTurret",				'W',	"FALSE",				&NanoSysData.SrTurret[3].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ----------			   */
	{"",							"Turret4_Analysis",				'b',	"TRUE",					&NanoSysData.SrTurret[3].bAnalysis											},
	{"",							"Turret4_AFTurret",				'W',	"9999",					&NanoSysData.SrTurret[3].iAFLens											},
/* modified 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- } ---------- */
/* added 2009.09.08 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽT) ---------- } ---------- */
	{"LENS CENTER OFFSET",			"BaseLens", 					'I',	"0",					&NanoSysData.SrLensCenterOffset.iBaseLens								   },
	{"",							"Lens1_CenterOffset_X", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[0].lX								   },
	{"",							"Lens1_CenterOffset_Y", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[0].lY								   },
// 2013.02.22 bagus Substrate thickness setting -->
	{"",							"Lens1_CenterOffset_Z", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[0].lZ								   },
// 2013.02.22 bagus Substrate thickness setting <--
	{"",							"Lens2_CenterOffset_X", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[1].lX								   },
	{"",							"Lens2_CenterOffset_Y", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[1].lY								   },
// 2013.02.22 bagus Substrate thickness setting -->
	{"",							"Lens2_CenterOffset_Z", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[1].lZ								   },
// 2013.02.22 bagus Substrate thickness setting <--
	{"",							"Lens3_CenterOffset_X", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[2].lX								   },
	{"",							"Lens3_CenterOffset_Y", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[2].lY								   },
// 2013.02.22 bagus Substrate thickness setting -->
	{"",							"Lens3_CenterOffset_Z", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[2].lZ								   },
// 2013.02.22 bagus Substrate thickness setting <--
	{"",							"Lens4_CenterOffset_X", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[3].lX								   },
	{"",							"Lens4_CenterOffset_Y", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[3].lY								   },
// 2013.02.22 bagus Substrate thickness setting -->
	{"",							"Lens4_CenterOffset_Z", 		'L',	"0",					&NanoSysData.SrLensCenterOffset.Lens[3].lZ								   },
// 2013.02.22 bagus Substrate thickness setting <--
	{"XMP", 						"StrategyListPath", 			'C',	"c:\\wvase32\\adap\\analysis\\strategy.lst",			NanoSysData.SrXmp.szStrategyListPath	   },
	{"",							"DataPath", 					'C',	"c:\\wvase32\\adap\\aft2adap.dat",						NanoSysData.SrXmp.szDataPath			   },
	{"",							"FittingDataPath",				'C',	"c:\\wvase32\\adap\\gendata.dat",						NanoSysData.SrXmp.szFittingDataPath 	   },
	{"",							"ADAPExePath",					'C',	"c:\\wvase32\\adap\\jaw_adap.exe",						NanoSysData.SrXmp.szAdapExePath 		   },
	{"",							"WVASEWindowTitle", 			'C',	"V.A.S.E. for Windows, Port #2910",						NanoSysData.SrXmp.szWvaseWindowTitle	   },
	{"VIEW RANGE",					"Left", 						'L',	"0",					&NanoSysData.SrViewRange.lLeft											   },
	{"",							"Top",							'L',	"0",					&NanoSysData.SrViewRange.lTop											   },
	{"",							"Width",						'L',	"640",					&NanoSysData.SrViewRange.lWidth 										   },
	{"",							"Height",						'L',	"480",					&NanoSysData.SrViewRange.lHeight										   },
	{"VIDEO CALIB", 				"Lens1_XPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[0].dXPixelToMicron							   },
	{"",							"Lens1_YPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[0].dYPixelToMicron							   },
	{"",							"Lens2_XPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[1].dXPixelToMicron							   },
	{"",							"Lens2_YPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[1].dYPixelToMicron							   },
	{"",							"Lens3_XPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[2].dXPixelToMicron							   },
	{"",							"Lens3_YPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[2].dYPixelToMicron							   },
	{"",							"Lens4_XPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[3].dXPixelToMicron							   },
	{"",							"Lens4_YPixelToMicron", 		'D',	"0",					&NanoSysData.SrVideoCalib[3].dYPixelToMicron							   },
#if 0
	{"CCD ARRAY",					"CCDArray1_Item",				'C',	g_lpszSrCcdArrayItem[0],NanoSysData.SrCcdArray[0].szItem										   },
	{"",							"CCDArray1_Data",				'D',	"0",					&NanoSysData.SrCcdArray[0].dData										   },
	{"",							"CCDArray2_Item",				'C',	g_lpszSrCcdArrayItem[1],NanoSysData.SrCcdArray[1].szItem										   },
	{"",							"CCDArray2_Data",				'D',	"0",					&NanoSysData.SrCcdArray[1].dData										   },
	{"",							"CCDArray3_Item",				'C',	g_lpszSrCcdArrayItem[2],NanoSysData.SrCcdArray[2].szItem										   },
	{"",							"CCDArray3_Data",				'D',	"0",					&NanoSysData.SrCcdArray[2].dData										   },
	{"",							"CCDArray4_Item",				'C',	g_lpszSrCcdArrayItem[3],NanoSysData.SrCcdArray[3].szItem										   },
	{"",							"CCDArray4_Data",				'D',	"0",					&NanoSysData.SrCcdArray[3].dData										   },
	{"",							"CCDArray5_Item",				'C',	g_lpszSrCcdArrayItem[4],NanoSysData.SrCcdArray[4].szItem										   },
	{"",							"CCDArray5_Data",				'D',	"0",					&NanoSysData.SrCcdArray[4].dData										   },
	{"",							"CCDArray6_Item",				'C',	g_lpszSrCcdArrayItem[5],NanoSysData.SrCcdArray[5].szItem										   },
	{"",							"CCDArray6_Data",				'D',	"0",					&NanoSysData.SrCcdArray[5].dData										   },
	{"",							"CCDArray7_Item",				'C',	g_lpszSrCcdArrayItem[6],NanoSysData.SrCcdArray[6].szItem										   },
	{"",							"CCDArray7_Data",				'D',	"0",					&NanoSysData.SrCcdArray[6].dData										   },
#endif // 0
// 2009.05.14 k-matsuo -->
	{"CCD ARRAY",					"CCDArray1_Item",				'C',	g_lpszSrCcdArrayItem[0],NanoSysData.SrCcdArray[0].szItem										   },
	{"",							"CCDArray1_Data",				'#',	"0",					&NanoSysData.SrCcdArray[0].dData										   },
	{"",							"CCDArray2_Item",				'C',	g_lpszSrCcdArrayItem[1],NanoSysData.SrCcdArray[1].szItem										   },
	{"",							"CCDArray2_Data",				'#',	"0",					&NanoSysData.SrCcdArray[1].dData										   },
	{"",							"CCDArray3_Item",				'C',	g_lpszSrCcdArrayItem[2],NanoSysData.SrCcdArray[2].szItem										   },
	{"",							"CCDArray3_Data",				'#',	"0",					&NanoSysData.SrCcdArray[2].dData										   },
	{"",							"CCDArray4_Item",				'C',	g_lpszSrCcdArrayItem[3],NanoSysData.SrCcdArray[3].szItem										   },
	{"",							"CCDArray4_Data",				'#',	"0",					&NanoSysData.SrCcdArray[3].dData										   },
	{"",							"CCDArray5_Item",				'C',	g_lpszSrCcdArrayItem[4],NanoSysData.SrCcdArray[4].szItem										   },
	{"",							"CCDArray5_Data",				'#',	"0",					&NanoSysData.SrCcdArray[4].dData										   },
	{"",							"CCDArray6_Item",				'C',	g_lpszSrCcdArrayItem[5],NanoSysData.SrCcdArray[5].szItem										   },
	{"",							"CCDArray6_Data",				'#',	"0",					&NanoSysData.SrCcdArray[5].dData										   },
	{"",							"CCDArray7_Item",				'C',	g_lpszSrCcdArrayItem[6],NanoSysData.SrCcdArray[6].szItem										   },
	{"",							"CCDArray7_Data",				'#',	"0",					&NanoSysData.SrCcdArray[6].dData										   },
// 2009.05.14 k-matsuo <--
/* added 2024.01.08 hmenjo CCD ARRAY ｿｽﾇ会ｿｽ ---------- { ---------- */
	{"",							"CCDArray8_Item",				'C',	g_lpszSrCcdArrayItem[7],NanoSysData.SrCcdArray[7].szItem										   },
	{"",							"CCDArray8_Data",				'#',	"0",					&NanoSysData.SrCcdArray[7].dData										   },
/* added 2024.01.08 hmenjo CCD ARRAY ｿｽﾇ会ｿｽ ---------- } ---------- */
};

// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
// SystemCompEase.iniｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ
const INIFILE SystemCompEaseIniDesc[] =
{
//	Section Name					Key Name						Type	Default 				Variable Pointer
	{"CONFIG",						"EaseAnalyzeTimeout", 			'I',	"120",					&NanoSysData.CompEASEConfig.nAnalyzeTimeout 							   },
	{"COMPEASE IP",					"Address", 						'C',	"127.0.0.1",			NanoSysData.CompEASEConfig.szIpAddress 									   },
	{"",							"Port", 						'I',	"4444",					&NanoSysData.CompEASEConfig.nPort 										   },
	{"",							"AsyncPort", 					'I',	"4445",					&NanoSysData.CompEASEConfig.nAsyncPort 									   },
};
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--

/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ`ｿｽQ/SystemStress.ini ｿｽﾇ会ｿｽｿｽQ ---------- { ---------- */
/* SystemStress.iniｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ */
DWORD lg_dwDummy;
#define	LINEnSECNUM	_T("L%dSecNum")
const INIFILE SystemStressIniDesc[] =
{
	/*Section Name					Key Name						Type	Default					Variable Pointer */
	{"CONFIG",						"SamplingInterval",				'L',	"1000",					&NanoSysData.StressConfig.dwSamplingInterval			},
	{"",							"DataIntegrationInterval",		'L',	"1",					&NanoSysData.StressConfig.dwDataIntegrationInterval		},
	{"",							"ScanStartWaitTime",			'L',	"0",					&NanoSysData.StressConfig.dwScanStartWaitTime			},
	{"",							"ScanSpeed",					'L',	"100",					&NanoSysData.StressConfig.dwScanSpeed					},
// K.Matsuo 2009.09.07 -->
	{"",							"ScanStartPosition",			'L',	"10000",					&NanoSysData.StressConfig.lScanStartPosX				},
	{"",							"ScanEndPosition",				'L',	"2490000",				&NanoSysData.StressConfig.lScanEndPosX					},
// K.Matsuo 2009.09.07 <--
	{"",							"LineMoveStartPosition",		'L',	"5000",					&NanoSysData.StressConfig.lLineStartPosX				},
	{"",							"LineMoveEndPosition",			'L',	"2495000",				&NanoSysData.StressConfig.lLineEndPosX					},
/* added 2009.09.29 hmenjo Stress Z ｿｽｿｽｿｽXｿｽLｿｽｿｽｿｽｿｽｿｽﾊ置 ---------- { ---------- */
	{"",							"ScanZAxisPosition",			'L',	"420000",				&NanoSysData.StressConfig.lScanZAxisPosition			},
/* added 2009.09.29 hmenjo Stress Z ｿｽｿｽｿｽXｿｽLｿｽｿｽｿｽｿｽｿｽﾊ置 ---------- } ---------- */
	{"LINE SECTION",				"LiftPinNumberOfLine",			'L',	"1",					&NanoSysData.StressConfig.dwLiftPinNumberOfLine			},
	{"",							"Shape",						'L',	"0",					&NanoSysData.StressConfig.dwShape						},
	{"",							"Size_X",						'D',	"2500000",				&NanoSysData.StressConfig.Size.dx						},
	{"",							"Size_Y",						'D',	"2200000",				&NanoSysData.StressConfig.Size.dy						},
	{"",							"Origin_X",						'L',	"0",					&NanoSysData.StressConfig.Origin.lX						},
	{"",							"Origin_Y",						'L',	"0",					&NanoSysData.StressConfig.Origin.lY						},
	{"",							"StressValidRange",				'L',	"50000",				&NanoSysData.StressConfig.lStressValidRange				},
	{"",							"InvalidRangeEdge",				'L',	"0",					&NanoSysData.StressConfig.lInvalidRangeEdge				},
	{"",							"ThickMeasPointOffset_X",		'L',	"0",					&NanoSysData.StressConfig.MeasOffset.lX					},
	{"",							"ThickMeasPointOffset_Y",		'L',	"0",					&NanoSysData.StressConfig.MeasOffset.lY					},
//2009.12.21 bagus stress --{--
	{"",							"Sample Name",					'C',	"",						&NanoSysData.StressConfig.szSampleName					},
//2009.12.21 bagus stress --}--
//	{"",							LINEnSECNUM,					'L',	"0",					&NanoSysData.StressConfig.Line[0].dwSectionNum			},
//2009.12.21 bagus stress --{--
//	{"",							"dmy",							'L',	"9",					&lg_dwDummy												},
//2009.12.21 bagus stress --}--

};
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ`ｿｽQ/SystemStress.ini ｿｽﾇ会ｿｽｿｽQ ---------- } ---------- */
//2009.08.24 bagus stress --{--
#define	TABLENAME_N		_T("TableName%d")
#define	TABLEVALUE_N	_T("TableValue%d")
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//const INIFILE SystemStressModulusIniDesc[] =
const INIFILE UserStressModulusIniDesc[] =
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
{
	/*Section Name					Key Name						Type	Default					Variable Pointer */
	{"ElasticModulus",				"TableNumber",					'L',	"0",					&NanoSysData.StressElasticTable.uiTableNum										},
	{"",							TABLENAME_N,					'C',	"0",					&NanoSysData.StressElasticTable.ElasticModulus[0].tszElasticModulusName			},
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --{--
//	{"",							TABLEVALUE_N,					'D',	"0",					&NanoSysData.StressElasticTable.ElasticModulus[0].dElasticModulusValue			},
	{"",							TABLEVALUE_N,					'E',	"0",					&NanoSysData.StressElasticTable.ElasticModulus[0].dElasticModulusValue			},
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --}--
// 2009.12.21 bagus stres --{--
//	{"",							"dmy",							'L',	"0",					&lg_dwDummy																		},
// 2009.12.21 bagus stres --}--
};
//2009.08.24 bagus stress --}--

//2009.09.07 bagus se --{--
// SEｿｽpXMPｿｽﾌゑｿｽｿｽﾟに抵ｿｽ`ｿｽｿｽｿｽｿｽ
/* deleted 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- { ---------- */
///* added 2009.08.25 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- { ---------- */
const INIFILE SystemSeIniDesc[] =
{
	/*Section Name					Key Name						Type	Default											Variable Pointer */
	//{"CONFIG",					"DoNotMoveShutter",				'b',	"FALSE",										&NanoSysData.SeConfig.bDoNotMoveShutter		},
	{"XMP",							"StrategyListPath",				'C',	"c:\\wvase32\\adap\\analysis\\strategy.lst",	NanoSysData.SeXmp.szStrategyListPath		},
	{"",							"DataPath",						'C',	"c:\\wvase32\\adap\\sedata.dat",				NanoSysData.SeXmp.szDataPath				},
	{"",							"FittingDataPath",				'C',	"c:\\wvase32\\adap\\gendata.dat",				NanoSysData.SeXmp.szFittingDataPath			},
	{"",							"ADAPExePath",					'C',	"c:\\wvase32\\adap\\jaw_adap.exe",				NanoSysData.SeXmp.szAdapExePath				},
	{"",							"WVASEWindowTitle",				'C',	"V.A.S.E. for Windows, Port #6520",				NanoSysData.SeXmp.szWvaseWindowTitle		},
};
///* added 2009.08.25 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- } ---------- */
//2009.09.07 bagus se --}--

// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
const INIFILE SystemMsIniDesc[] =
{
	/*Section Name					Key Name						Type	Default						Variable Pointer */
	{"CONFIG",						"MsStdZAxisRetractPos",			'L',	"100000",					&NanoSysData.MsConfig.lStdZAxisRetractPos		},
};
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--

// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --{--
const INIFILE SystemCaIniDesc[] =
{
	/*Section Name					Key Name						Type	Default						Variable Pointer */
	{"CONFIG",						"CaZAxisPosition",				'L',	"100000",					&NanoSysData.CaConfig.lCaStdZAxisRetractPos			},
};
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --}--

// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
const INIFILE SystemRsIniDesc[] =
{
	/*Section Name					Key Name						Type	Default						Variable Pointer */
	{"CONFIG",						"RsZAxisPosition",				'L',	"100000",					&NanoSysData.RsConfig.lRsStdZAxisRetractPos			},
	{"",							"RsProbeType",					'I',	"5",						&NanoSysData.RsConfig.nRsProbeType					},
};
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--

// UserSettingCommon.iniｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ
const INIFILE UserSettingCommonIniDesc[] =
{
//	Section Name					Key Name						Type	Default 				Variable Pointer
	{"MANUAL LOAD POSITION",		"X",							'L',	"0",					&UserData.ManualLoadPosition.lX 								   },
	{"",							"Y",							'L',	"0",					&UserData.ManualLoadPosition.lY 								   },
	{"AUTO LOGON",					"UserName", 					'C',	"", 					UserData.szAutoLogonName										   },
	{"ACCESS LEVEL(ADMINISTRATOR)", "Measurement",					'b',	"TRUE", 				&UserData.AccessLevel[ACCOUNT_LEVEL_ADMINISTRATOR].bMeasurement    },
	{"",							"Recipe",						'b',	"TRUE", 				&UserData.AccessLevel[ACCOUNT_LEVEL_ADMINISTRATOR].bRecipe		   },
	{"",							"Program",						'b',	"TRUE", 				&UserData.AccessLevel[ACCOUNT_LEVEL_ADMINISTRATOR].bProgram 	   },
	{"",							"Data", 						'b',	"TRUE", 				&UserData.AccessLevel[ACCOUNT_LEVEL_ADMINISTRATOR].bData		   },
	{"",							"UserSetting",					'b',	"TRUE", 				&UserData.AccessLevel[ACCOUNT_LEVEL_ADMINISTRATOR].bUserSetting    },
	{"",							"Maintenance",					'b',	"TRUE", 				&UserData.AccessLevel[ACCOUNT_LEVEL_ADMINISTRATOR].bMaintenance    },
	{"ACCESS LEVEL(ENGINEER)",		"Measurement",					'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_ENGINEER].bMeasurement		   },
	{"",							"Recipe",						'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_ENGINEER].bRecipe			   },
	{"",							"Program",						'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_ENGINEER].bProgram			   },
	{"",							"Data", 						'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_ENGINEER].bData 			   },
	{"",							"UserSetting",					'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_ENGINEER].bUserSetting		   },
	{"",							"Maintenance",					'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_ENGINEER].bMaintenance		   },
	{"ACCESS LEVEL(OPERATOR)",		"Measurement",					'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_OPERATOR].bMeasurement		   },
	{"",							"Recipe",						'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_OPERATOR].bRecipe			   },
	{"",							"Program",						'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_OPERATOR].bProgram			   },
	{"",							"Data", 						'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_OPERATOR].bData 			   },
	{"",							"UserSetting",					'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_OPERATOR].bUserSetting		   },
	{"",							"Maintenance",					'b',	"FALSE",				&UserData.AccessLevel[ACCOUNT_LEVEL_OPERATOR].bMaintenance		   },
	{"DELETE DATA PERIOD",			"DeleteDataPeriod", 			'I',	"0",					&UserData.iDeleteDataPeriod 									   },
	{"DEFAULT HEAD TYPE",			"HeadType", 					'I',	"0",					&UserData.DefaultHeadType.nHeadType 							   },
	{"",							"MeasurementType",				'I',	"0",					&UserData.DefaultHeadType.nMeasType 							   },
	{"STAGE MOTION JOG DATA",		"JogLowX",						'I',	"100",					&UserData.StageMotionJogData[X].Jog[STAGE_MOTION_LOW]			   },
	{"",							"JogLowY",						'I',	"100",					&UserData.StageMotionJogData[Y].Jog[STAGE_MOTION_LOW]			   },
	{"",							"JogLowZ",						'I',	"10",					&UserData.StageMotionJogData[Z].Jog[STAGE_MOTION_LOW]			   },
	{"",							"JogMiddleX",					'I',	"500",					&UserData.StageMotionJogData[X].Jog[STAGE_MOTION_MID]			   },
	{"",							"JogMiddleY",					'I',	"500",					&UserData.StageMotionJogData[Y].Jog[STAGE_MOTION_MID]			   },
	{"",							"JogMiddleZ",					'I',	"20",					&UserData.StageMotionJogData[Z].Jog[STAGE_MOTION_MID]			   },
	{"",							"JogHighX", 					'I',	"1000", 				&UserData.StageMotionJogData[X].Jog[STAGE_MOTION_HIGH]			   },
	{"",							"JogHighY", 					'I',	"1000", 				&UserData.StageMotionJogData[Y].Jog[STAGE_MOTION_HIGH]			   },
	{"",							"JogHighZ", 					'I',	"40",					&UserData.StageMotionJogData[Z].Jog[STAGE_MOTION_HIGH]			   },
	{"STAGE MOTION SPEED DATA", 	"SpeedLowX",					  'I',	  "100",				  &UserData.StageMotionSpeedData[X].Speed[STAGE_MOTION_LOW] 	   },
	{"",							"SpeedLowY",					  'I',	  "100",				  &UserData.StageMotionSpeedData[Y].Speed[STAGE_MOTION_LOW] 	   },
	{"",							"SpeedLowZ",					  'I',	  "10", 				  &UserData.StageMotionSpeedData[Z].Speed[STAGE_MOTION_LOW] 	   },
	{"",							"SpeedMiddleX", 				  'I',	  "500",				  &UserData.StageMotionSpeedData[X].Speed[STAGE_MOTION_MID] 	   },
	{"",							"SpeedMiddleY", 				  'I',	  "500",				  &UserData.StageMotionSpeedData[Y].Speed[STAGE_MOTION_MID] 	   },
	{"",							"SpeedMiddleZ", 				  'I',	  "20", 				  &UserData.StageMotionSpeedData[Z].Speed[STAGE_MOTION_MID] 	   },
	{"",							"SpeedHighX",					  'I',	  "1000",				  &UserData.StageMotionSpeedData[X].Speed[STAGE_MOTION_HIGH]	   },
	{"",							"SpeedHighY",					  'I',	  "1000",				  &UserData.StageMotionSpeedData[Y].Speed[STAGE_MOTION_HIGH]	   },
	{"",							"SpeedHighZ",					  'I',	  "40", 				  &UserData.StageMotionSpeedData[Z].Speed[STAGE_MOTION_HIGH]	   },
	{"",							"JogX", 						  'I',	  "1000",				  &UserData.StageMotionSpeedData[X].Jog							   },
	{"",							"JogY", 						  'I',	  "1000",				  &UserData.StageMotionSpeedData[Y].Jog 						   },
	{"",							"JogZ", 						  'I',	  "40", 				  &UserData.StageMotionSpeedData[Z].Jog 						   },
// Maintenance Position
	{"MAINTENANCE POSITION",		"X1",							'L',	"0",					&UserData.MaintenancePosition[0].Loc.lX 						   },
	{"",							"Y1",							'L',	"0",					&UserData.MaintenancePosition[0].Loc.lY 						   },
	{"",							"Comment1", 					'C',	"", 					UserData.MaintenancePosition[0].szComment						   },
	{"",							"X2",							'L',	"0",					&UserData.MaintenancePosition[1].Loc.lX 						   },
	{"",							"Y2",							'L',	"0",					&UserData.MaintenancePosition[1].Loc.lY 						   },
	{"",							"Comment2", 					'C',	"", 					UserData.MaintenancePosition[1].szComment						   },
	{"",							"X3",							'L',	"0",					&UserData.MaintenancePosition[2].Loc.lX 						   },
	{"",							"Y3",							'L',	"0",					&UserData.MaintenancePosition[2].Loc.lY 						   },
	{"",							"Comment3", 					'C',	"", 					UserData.MaintenancePosition[2].szComment						   },
	{"",							"X4",							'L',	"0",					&UserData.MaintenancePosition[3].Loc.lX 						   },
	{"",							"Y4",							'L',	"0",					&UserData.MaintenancePosition[3].Loc.lY 						   },
	{"",							"Comment4", 					'C',	"", 					UserData.MaintenancePosition[3].szComment						   },
// Maintenance Date
	{"MAINTENANCE DATE",			"Date", 						'C',	"", 					UserData.MaintenanceDate.MaintenanceDate						   },
	{"",							"Next", 						'C',	"", 					UserData.MaintenanceDate.NextMaintenanceDate					   },
	{"",							"SPAN",							'I',	"0",					&UserData.MaintenanceDate.nDaysSpan 							   },
	{"",							"ALARM",						'I',	"0",					&UserData.MaintenanceDate.nAlarmFlag							   },
// Maintenance Hour
	{"MAINTENANCE LAMP",			"Date", 						'C',	"", 					UserData.MaintenanceLamp.MaintenanceDate						   },
	{"",							"Hour", 						'D',	"", 					&UserData.MaintenanceLamp.MaintenanceHour						   },
	{"",							"SPAN",							'I',	"0",					&UserData.MaintenanceLamp.nHourSpan 							   },
	{"",							"ALARM",						'I',	"0",					&UserData.MaintenanceLamp.nAlarmFlag							   },
// Recipe Backup
	{"BACKUP DATA",					"AutoBackup",					'b',	"FALSE",				&UserData.BackupData.bAutoBackup									},
	{"",							"BackupPath",					'C',	"",						UserData.BackupData.BackupPath										},
};

//// UserSettingCommon.iniｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ
const INIFILE UserSettingSrIniDesc[] =
{
//	Section Name					Key Name						Type	Default 				Variable Pointer
	{"PATTERN REC", 				"NumLayers",					'I',	"0",					&UserData.SrPatternRec.nNumLayers								   },
	{"",							"LowestMatchScore", 			'D',	"40",					&UserData.SrPatternRec.dLowestMatchScore						   },
	{"",							"StopGropingScore", 			'D',	"65",					&UserData.SrPatternRec.dStopGropingScore						   },
// Matsuhisa 2009.05.30 ----->
	{"",							"MaskEnable",					'b',	"FALSE",				&UserData.SrPatternRec.Mask.bEnable 							   },
	{"",							"XRad", 						'L',	"2",					&UserData.SrPatternRec.Mask.XRad								   },
	{"",							"YRad", 						'L',	"2",					&UserData.SrPatternRec.Mask.YRad								   },
// Matsuhisa 2009.05.30 ----->
	{"REFERENCE POSITION",			"DarkReference_X",				'L',	"0",					&UserData.SrReferencePosition.LocDarkReference.lX				   },
	{"",							"DarkReference_Y",				'L',	"0",					&UserData.SrReferencePosition.LocDarkReference.lY				   },
	{"",							"DarkReference_Z",				'L',	"0",					&UserData.SrReferencePosition.LocDarkReference.lZ				   },
	{"",							"ReflectReference_X",			'L',	"0",					&UserData.SrReferencePosition.LocReflectReference.lX			   },
	{"",							"ReflectReference_Y",			'L',	"0",					&UserData.SrReferencePosition.LocReflectReference.lY			   },
	{"",							"ReflectReference_Z",			'L',	"0",					&UserData.SrReferencePosition.LocReflectReference.lZ			   },
//	  {"",							  "TransmitAbsoluteReference_X",  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitAbsoluteReference.lX 	 },
//	  {"",							  "TransmitAbsokuteReference_Y",  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitAbsoluteReference.lY 	 },
//	  {"",							  "TransmitAbsokuteReference_Z",  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitAbsoluteReference.lZ 	 },
//	  {"",							  "TransmitFirstReference_X",	  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitFirstReference.lX		 },
//	  {"",							  "TransmitFirstReference_Y",	  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitFirstReference.lY		 },
//	  {"",							  "TransmitFirstReference_Z",	  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitFirstReference.lZ		 },
//	  {"",							  "TransmitSecondReference_X",	  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitSecondReference.lX		 },
//	  {"",							  "TransmitSecondReference_Y",	  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitSecondReference.lY		 },
//	  {"",							  "TransmitSecondReference_Z",	  'L',	  "0",					  &UserData.SrReferencePosition.LocTransmitSecondReference.lZ		 },
//	  {"FIGURE FORMAT", 			  "ThicknessFormat",			  'I',	  "4",					  &UserData.SrFigureFormat.nThicknessFormat 						 },
	{"FIGURE FORMAT",				"ThicknessUnit",				'I',	"0",					&UserData.SrFigureFormat.nThicknessUnit 						   },
//	  {"",							  "ThicknessUnit",				  'I',	  "0",					  &UserData.SrFigureFormat.nThicknessUnit							 },
//	  {"",							  "ReflectanceFormat",			  'I',	  "4",					  &UserData.SrFigureFormat.nReflectanceFormat						 },
//	  {"",							  "TransmittanceFormat",		  'I',	  "4",					  &UserData.SrFigureFormat.nTransmittanceFormat 					 },
//	  {"",							  "ChromaticityFormat", 		  'I',	  "4",					  &UserData.SrFigureFormat.nChromaticityFormat						 },
//	  {"",							  "OpticalDensityFormat",		  'I',	  "4",					  &UserData.SrFigureFormat.nOpticalDensityFormat					 },
/* added 2017.04.15 hmenjo ｿｽｿｽｿｽｿｽｿｽ\ｿｽｿｽｿｽｿｽ ---------- { ---------- */
	  {"",							  "ThicknessIntOnly",			  'I',	  "0",					&UserData.SrFigureFormat.iThicknessIntOnly							},
/* added 2017.04.15 hmenjo ｿｽｿｽｿｽｿｽｿｽ\ｿｽｿｽｿｽｿｽ ---------- } ---------- */
	{"AUTOFOCUS RETRY", 			"AutoFocusRetryTimes",			'I',	"1",					&UserData.SrAutoFocusRetry.nTimes		  						   },
/* added 2009.09.07 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽQ) ---------- { ---------- */
	{"TRANSMITTANCE SETTING",		"DoNotCheckMeasPoint",			'b',	"FALSE",				&UserData.SrTransmittance.bDoNotCheckMeasPoint						},
/* added 2009.09.07 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽQ) ---------- } ---------- */

};

/* added 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- { ---------- */
/* UserSettingSe.ini ｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ	*/
const INIFILE UserSettingSeIniDesc[] =
{
	/*Section Name					Key Name						Type	Default					Variable Pointer */
/* modified 2009.09.14 hmenjo SE ｿｽｿｽｿｽｿｽ SR ｿｽｿｽｿｽｿｽｿｽv OPEN ｿｽﾅよい ---------- { ---------- */
//	{"SE SETTING",					"DoNotMoveShutter",				'b',	"FALSE",				&UserData.SeSetting.bDoNotMoveShutter					},
/* modified 2009.09.14 hmenjo SE ｿｽｿｽｿｽｿｽ SR ｿｽｿｽｿｽｿｽｿｽv OPEN ｿｽﾅよい ----------				*/
	{"SE SETTING",					"DoNotMoveShutter",				'b',	"TRUE",					&UserData.SeSetting.bDoNotMoveShutter					},
/* modified 2009.09.14 hmenjo SE ｿｽｿｽｿｽｿｽ SR ｿｽｿｽｿｽｿｽｿｽv OPEN ｿｽﾅよい ---------- } ---------- */
// 2009.09.19 bagus SE --{--
	{"FIGURE FORMAT",				"SeUnit",						'I',	"0",					&UserData.SeFigureFormat.nThicknessUnit					},
// 2009.09.19 bagus SE --}--
};
/* added 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- } ---------- */

// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
/* UserSettingEase.ini ｿｽﾌセｿｽNｿｽVｿｽｿｽｿｽｿｽｿｽCｿｽLｿｽ[ｿｽCｿｽfｿｽtｿｽHｿｽｿｽｿｽgｿｽCｿｽﾏ撰ｿｽｿｽﾌ設抵ｿｽ	*/
const INIFILE UserSettingCompEASEIniDesc[] =
{
	/*Section Name					Key Name						Type	Default					Variable Pointer */
	{"FIGURE FORMAT",				"EaseUnit",						'I',	"0",					&UserData.CompEASEFigureFormat.nThicknessUnit			},
};
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--

// 2013.02.22 bagus Substrate Thickness --{--
#define	SUBSTRATE_THICKNESS_N	_T("SubstrateThickness%d")
#define	INVALID_REGION_N		_T("InvalidRegion%d")
#define	USE_DATA_N				_T("UseData%d")
const INIFILE SubstrateThicknessIniDesc[] =
{
	/*Section Name					Key Name						Type	Default					Variable Pointer */
	{"SubstrateThickness",			SUBSTRATE_THICKNESS_N,			'D',	"0",					&NanoSysData.SubstrateThicknessTable.SubstrateThickness[0].dThickness			},
	{"",							INVALID_REGION_N,				'D',	"0",					&NanoSysData.SubstrateThicknessTable.SubstrateThickness[0].dInvalidRegion		},
	{"",							USE_DATA_N,						'b',	"FALSE",				&NanoSysData.SubstrateThicknessTable.SubstrateThickness[0].bUseData				},
};
// 2013.02.22 bagus Substrate Thickness --{--

/////////////////////////////////////////////////////////////////////////////
// Name       : LoadNanoSpecIni
// Purpose    : Iniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾌ難ｿｽｿｽeｿｽﾌ読み搾ｿｽｿｽｿｽ
// Parameters : iIniFile ---> ｿｽﾇゑｿｽIniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾈのゑｿｽｿｽｿｽｿｽｿｽｿｽﾟゑｿｽﾏ撰ｿｽ
//
// Returns    : ｿｽﾈゑｿｽ
void LoadNanoSpecIni(int iIniFile)
{
	int index, iIniDescCount;
	LPCSTR section;
	char buff[256], szIniFilePath[_MAX_PATH];
	INIFILE* pIniDesc = NULL;

	memset(szIniFilePath, 0, sizeof(szIniFilePath));
	switch(iIniFile){
	case SYSTEM_COMMON_INI_FILE:
		iIniDescCount = sizeof(SystemCommonIniDesc) / sizeof(SystemCommonIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemCommonIniDesc, sizeof(SystemCommonIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
	case SYSTEM_SR_INI_FILE:
		iIniDescCount = sizeof(SystemSrIniDesc) / sizeof(SystemSrIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemSrIniDesc, sizeof(SystemSrIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
//2009.09.08 bagus se --{--
//SRｿｽpXMPｿｽﾌゑｿｽｿｽﾟに抵ｿｽ`ｿｽｿｽｿｽｿｽ
/* deleted 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- { ---------- */
///* added 2009.08.25 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- { ---------- */
	case SYSTEM_SE_INI_FILE:
		iIniDescCount = sizeof(SystemSeIniDesc) / sizeof(SystemSeIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemSeIniDesc, sizeof(SystemSeIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
///* added 2009.08.25 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- } ---------- */
//2009.09.08 bagus se --}--
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	case SYSTEM_MS_INI_FILE:
		iIniDescCount = sizeof(SystemMsIniDesc) / sizeof(SystemMsIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemMsIniDesc, sizeof(SystemMsIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --{--
	case SYSTEM_CA_INI_FILE:
		iIniDescCount = sizeof(SystemCaIniDesc) / sizeof(SystemCaIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemCaIniDesc, sizeof(SystemCaIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --}--
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
	case SYSTEM_RS_INI_FILE:
		iIniDescCount = sizeof(SystemRsIniDesc) / sizeof(SystemRsIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemRsIniDesc, sizeof(SystemRsIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--
	case USER_SETTING_COMMON_INI_FILE:
		iIniDescCount = sizeof(UserSettingCommonIniDesc) / sizeof(UserSettingCommonIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingCommonIniDesc, sizeof(UserSettingCommonIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
	case USER_SETTING_SR_INI_FILE:
		iIniDescCount = sizeof(UserSettingSrIniDesc) / sizeof(UserSettingSrIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingSrIniDesc, sizeof(UserSettingSrIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
/* added 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- { ---------- */
	case USER_SETTING_SE_INI_FILE:
		iIniDescCount = sizeof(UserSettingSeIniDesc) / sizeof(UserSettingSeIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingSeIniDesc, sizeof(UserSettingSeIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
/* added 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- } ---------- */
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case USER_SETTING_COMPEASE_INI_FILE:
		iIniDescCount = sizeof(UserSettingCompEASEIniDesc) / sizeof(UserSettingCompEASEIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingCompEASEIniDesc, sizeof(UserSettingCompEASEIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
	case SYSTEM_STRESS_INI_FILE:
		iIniDescCount = sizeof(SystemStressIniDesc) / sizeof(SystemStressIniDesc[0]);
		pIniDesc = new INIFILE[iIniDescCount];
		memcpy(pIniDesc, SystemStressIniDesc, sizeof(SystemStressIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
//2009.08.24 bagus stress --{--
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//	case SYSTEM_STRESS_MODULUS_INI_FILE:
	case USER_STRESS_MODULUS_INI_FILE:
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//		iIniDescCount = sizeof(SystemStressModulusIniDesc) / sizeof(SystemStressModulusIniDesc[0]);
		iIniDescCount = sizeof(UserStressModulusIniDesc) / sizeof(UserStressModulusIniDesc[0]);
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
		pIniDesc = new INIFILE[iIniDescCount];
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//		memcpy(pIniDesc, SystemStressModulusIniDesc, sizeof(SystemStressModulusIniDesc));
		memcpy(pIniDesc, UserStressModulusIniDesc, sizeof(UserStressModulusIniDesc));
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
		// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
		//sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
		break;
//2009.08.24 bagus stress --}--
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case SYSTEM_COMPEASE_INI_FILE:
		iIniDescCount = sizeof(SystemCompEaseIniDesc) / sizeof(SystemCompEaseIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemCompEaseIniDesc, sizeof(SystemCompEaseIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--
// 2013.02.22 bagus Substrate thickness setting -->
	case SUBSTRATE_THICKNESS_INI_FILE:
		iIniDescCount = sizeof(SubstrateThicknessIniDesc) / sizeof(SubstrateThicknessIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SubstrateThicknessIniDesc, sizeof(SubstrateThicknessIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2013.02.22 bagus Substrate thickness setting <--
	default:
		return;
		break;
	}
/* added 2009.07.07 hmenjo dll ｿｽｿｽｿｽﾎパｿｽXｿｽﾎ会ｿｽ ConfigFile.dll ---------- { ---------- */
	TCHAR l_tszTempFName[_MAX_PATH];
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	_tcscpy_s(l_tszTempFName, _MAX_PATH, szIniFilePath);
	_stprintf_s(szIniFilePath, _MAX_PATH, _T("%s%s"), g_tszProcDir, l_tszTempFName);
/* added 2009.07.07 hmenjo dll ｿｽｿｽｿｽﾎパｿｽXｿｽﾎ会ｿｽ ConfigFile.dll ---------- } ---------- */

//2009.08.27 bagus stress --{--
	if(iIniFile == SYSTEM_STRESS_INI_FILE){
		int nIniStressValidLineNum = 0;
		int nIniStressLine;
		int nIniStressPin;
		int nIniStressSection;
		for(nIniStressLine = 0;nIniStressLine < STRESS_LINES_MAX;nIniStressLine++){
			int nIniStressValidSectionNum = 0;
			int nIniStressValidPinNum = 0;
			char szBuf[1024+1];
			section = "LINE SECTION";
			sprintf_s(szBuf, sizeof(szBuf), "ValidLine%d", nIniStressLine+1);
			NanoSysData.StressConfig.Line[nIniStressLine].bValidLine = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
			sprintf_s(szBuf, sizeof(szBuf), "LiftPinNumberOfL%d", nIniStressLine+1);
			NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
// 2009.09.17 bagus SE --{--
//			NanoSysData.StressConfig.Line[nIniStressLine].dwSectionNum = NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn -1;
			if (NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn < 1) {
				NanoSysData.StressConfig.Line[nIniStressLine].dwSectionNum = 0;
			} else {
				NanoSysData.StressConfig.Line[nIniStressLine].dwSectionNum = NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn -1;
			}
// 2009.09.17 bagus SE --}--
// 2009.10.17 bagus Stress --{--
#if 0
			for(nIniStressPin = 0;nIniStressPin < STRESS_PINS_MAX;nIniStressPin++){
				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_X", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lX = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_Y", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lY = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_X", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lX = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_Y", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lY = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
			}
#else
			for(nIniStressPin = 0;nIniStressPin < STRESS_PINS_MAX;nIniStressPin++){
				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_X", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lX = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_Y", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lY = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
			}
			for(nIniStressPin = 0;nIniStressPin < PIN_INTERVAL;nIniStressPin++){
				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_X", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lX = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_Y", nIniStressLine+1,nIniStressPin+1);
				NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lY = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
			}
#endif
// 2009.10.17 bagus Stress --}--
			for(nIniStressSection = 0;nIniStressSection < STRESS_SECTIONS_MAX;nIniStressSection++){
				sprintf_s(szBuf, sizeof(szBuf), "L%dS%d_ScanStart_X", nIniStressLine+1,nIniStressSection+1);
				NanoSysData.StressConfig.Line[nIniStressLine].SectPos[nIniStressSection].lScanStartPosX = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "L%dS%d_ScanEnd_X", nIniStressLine+1,nIniStressSection+1);
				NanoSysData.StressConfig.Line[nIniStressLine].SectPos[nIniStressSection].lScanEndPosX = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
				sprintf_s(szBuf, sizeof(szBuf), "L%dS%d_ScanPos_Y", nIniStressLine+1,nIniStressSection+1);
				NanoSysData.StressConfig.Line[nIniStressLine].SectPos[nIniStressSection].lScanPosY = ::GetPrivateProfileInt(section,
																szBuf,
																0,
																szIniFilePath);
			}
		}

	}
//2009.08.27 --}--

// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	if(iIniFile == SYSTEM_MS_INI_FILE){
		int	nIndex;
		char szMsKey[1024+1];

		section = "ROBO CYLINDER POSITION";
		for (nIndex=0; nIndex<MAX_MS_ROBO_CYLINDER_NUM; nIndex++) {
			sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Enable", nIndex+1);
			::GetPrivateProfileString(section,
										szMsKey,
										"FALSE",
										buff,
										sizeof(buff),
										szIniFilePath);
			NanoSysData.MsConfig.RoboCylinder[nIndex].bEnable = (strcmp(buff, "TRUE") == 0)? TRUE: FALSE;

			sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Name", nIndex+1);
			::GetPrivateProfileString(section,
										szMsKey,
										"",
										NanoSysData.MsConfig.RoboCylinder[nIndex].szName,
										sizeof(NanoSysData.MsConfig.RoboCylinder[nIndex].szName),
										szIniFilePath);

			sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Position", nIndex+1);
			NanoSysData.MsConfig.RoboCylinder[nIndex].nPosition = ::GetPrivateProfileInt(section, szMsKey, 1, szIniFilePath);

			sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Sensor", nIndex+1);
			::GetPrivateProfileString(section,
										szMsKey,
										"FALSE",
										buff,
										sizeof(buff),
										szIniFilePath);
			NanoSysData.MsConfig.RoboCylinder[nIndex].bSensor = (strcmp(buff, "TRUE") == 0)? TRUE: FALSE;
		}
	}
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--

/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
	int l_iLineCount = 0;
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
//2009.08.24 bagus stress --{--
// Stress ElasticModulus ｿｽﾎ会ｿｽ
	int l_iTableNameCount = 0;
	int l_iTableValueCount = 0;
//2009.08.24 bagus stress --}--
// 2013.02.22 bagus Substrate thickness setting -->
	int l_iThicknessCount = 0;
	int l_iInvalidRegionCount = 0;
	int l_iUseDataCount = 0;
// 2013.02.22 bagus Substrate thickness setting <--

// 2013.02.22 bagus Substrate thickness setting -->
	if(SUBSTRATE_THICKNESS_INI_FILE != iIniFile){
// 2013.02.22 bagus Substrate thickness setting -->
	for(index = 0; index < iIniDescCount; index++){
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
		if (0 != l_iLineCount) {
			if (0 != index) {
				index--;
			}
		}
#if 1
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
		// get section name
		if(strlen((pIniDesc + index)->section) > 0)
			section = (pIniDesc + index)->section;

		// get ini file data
		memset(buff, NULL, sizeof(buff));
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
				/*	ｿｽﾛ托ｿｽｿｽﾌ関撰ｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽ鼾ｿｽﾍ，ｿｽﾅ大ラｿｽCｿｽｿｽｿｽｿｽｿｽ超ゑｿｽｿｽｿｽｿｽｿｽｿｽCｿｽｿｽｿｽﾌ抵ｿｽ`ｿｽｿｽ ini ｿｽtｿｽ@ｿｽCｿｽｿｽｿｽｿｽ
					ｿｽｿｽｿｽoｿｽｿｽｿｽﾈゑｿｽｿｽ謔､ｿｽﾉゑｿｽｿｽﾈゑｿｽｿｽｿｽﾎなゑｿｽﾈゑｿｽｿｽｿｽｿｽﾟ，ｿｽRｿｽRｿｽﾆ難ｿｽｿｽlｿｽﾈ擾ｿｽｿｽｿｽｿｽﾉはゑｿｽｿｽﾈゑｿｽｿｽｿｽｿｽﾆ．*/
		TCHAR l_tszKey[256];
		LPCTSTR l_ptszKeyOrg = nullptr;
		if (0 == l_iLineCount) {
			if ((SYSTEM_STRESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, LINEnSECNUM))) {
				l_ptszKeyOrg = (pIniDesc + index)->key;
				l_iLineCount = STRESS_LINES_MAX;
			}
		}
		if (0 < l_iLineCount) {
			if (l_ptszKeyOrg) {
			(pIniDesc + index)->key = l_ptszKeyOrg;
			}
			if ((SYSTEM_STRESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, LINEnSECNUM))) {
				_stprintf_s(l_tszKey, _countof(l_tszKey), LINEnSECNUM, STRESS_LINES_MAX - l_iLineCount + 1);
				(pIniDesc + index)->key = l_tszKey;
				(pIniDesc + index)->data = &NanoSysData.StressConfig.Line[STRESS_LINES_MAX - l_iLineCount].dwSectionNum;
				l_iLineCount--;
			}
		}
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
#else
	//2009.08.27 bagus stress --{--
	//ｿｽfｿｽ[ｿｽ^ｿｽｿｽｿｽｿｽｿｽｿｽｿｽﾈるたｿｽﾟゑｿｽｿｽﾌ擾ｿｽｿｽｿｽｿｽﾍゑｿｽﾟゑｿｽ
		TCHAR l_tszKey[256];
		LPCTSTR l_ptszKeyOrg = nullptr;
	//2009.08.27 bagus stress --}--
#endif
//2009.08.24 bagus stress --{--
// Stress ElasticModulus ｿｽﾎ会ｿｽ
		if (0 != l_iTableNameCount) {
			if (0 != index) {
				index--;
			}
		}

		// get ini file data
		memset(buff, NULL, sizeof(buff));
		if (0 == l_iTableNameCount) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				l_ptszKeyOrg = (pIniDesc + index)->key;
				l_iTableNameCount = STRESS_MODULUS_MAX;
			}
		}
		if (0 < l_iTableNameCount) {
			if (l_ptszKeyOrg) {
			(pIniDesc + index)->key = l_ptszKeyOrg;
			}
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				_stprintf_s(l_tszKey, _countof(l_tszKey), TABLENAME_N, STRESS_MODULUS_MAX - l_iTableNameCount + 1);
				(pIniDesc + index)->key = l_tszKey;
				(pIniDesc + index)->data = &NanoSysData.StressElasticTable.ElasticModulus[STRESS_MODULUS_MAX - l_iTableNameCount].tszElasticModulusName;
				l_iTableNameCount--;
			}
		}
		if (0 != l_iTableValueCount) {
			if (0 != index) {
				index--;
			}
		}
		if (0 == l_iTableValueCount) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				l_ptszKeyOrg = (pIniDesc + index)->key;
				l_iTableValueCount = STRESS_MODULUS_MAX;
			}
		}
		if (0 < l_iTableValueCount) {
			if (l_ptszKeyOrg) {
			(pIniDesc + index)->key = l_ptszKeyOrg;
			}
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				_stprintf_s(l_tszKey, _countof(l_tszKey), TABLEVALUE_N, STRESS_MODULUS_MAX - l_iTableValueCount + 1);
				(pIniDesc + index)->key = l_tszKey;
				(pIniDesc + index)->data = &NanoSysData.StressElasticTable.ElasticModulus[STRESS_MODULUS_MAX - l_iTableValueCount].dElasticModulusValue;
				l_iTableValueCount--;
			}
		}

//2009.08.24 bagus stress --}--

		::GetPrivateProfileString(section,
								(pIniDesc + index)->key,
								(pIniDesc + index)->def,
								buff,
								sizeof(buff),
								szIniFilePath);

		// set ini file data to variable
		switch((pIniDesc + index)->type){
		case 'b':
			*(BOOL*)(pIniDesc + index)->data = (strcmp(buff, "TRUE") == 0)? TRUE: FALSE;
			break;
		case 'B':
			*(BYTE*)(pIniDesc + index)->data = (BYTE)atoi(buff);
			break;
		case 'S':
			*(short*)(pIniDesc + index)->data = (short)atoi(buff);
			break;
		case 'I':
			*(int*)(pIniDesc + index)->data = (int)atoi(buff);
			break;
		case 'L':
			*(long*)(pIniDesc + index)->data = (long)atol(buff);
			break;
		case 'F':
			*(float*)(pIniDesc + index)->data = (float)atof(buff);
			break;
		case 'D':
			*(double*)(pIniDesc + index)->data = (double)atof(buff);
			break;
// 2009.05.14 k-matsuo -->
		case '#':
			*(double*)(pIniDesc + index)->data = (double)atof(buff);
			break;
// 2009.05.14 k-matsuo <--
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --{--
		case 'E':
			*(double*)(pIniDesc + index)->data = strtod(buff, NULL);
			break;
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --}--
		case 'W':
			*(WORD*)(pIniDesc + index)->data = (WORD)atoi(buff);
			break;
		case 'C':
			strcpy_s((char*)(pIniDesc + index)->data, 256, buff);
			break;
		default:
			(void*)(pIniDesc + index)->data = NULL;
			break;
		}
	}
// 2013.02.22 bagus Substrate thickness setting -->
	}
	else{
		TCHAR l_tszKey[256];
		LPCTSTR l_ptszKeyOrg = nullptr;

		for(index = 0; index < iIniDescCount; index++){
			// get section name
			if(strlen((pIniDesc + index)->section) > 0)
				section = (pIniDesc + index)->section;

			for(int i = 0; i < SUBSTRATE_THICKNESS_MAX; i++){

				// get ini file data
				memset(buff, NULL, sizeof(buff));
				if (0 == l_iThicknessCount) {
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, SUBSTRATE_THICKNESS_N))) {
						l_ptszKeyOrg = (pIniDesc + index)->key;
						l_iThicknessCount = SUBSTRATE_THICKNESS_MAX;
					}
				}
				if (0 < l_iThicknessCount) {
					if (l_ptszKeyOrg) {
					(pIniDesc + index)->key = l_ptszKeyOrg;
					}
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, SUBSTRATE_THICKNESS_N))) {
						_stprintf_s(l_tszKey, _countof(l_tszKey), SUBSTRATE_THICKNESS_N, SUBSTRATE_THICKNESS_MAX - l_iThicknessCount);
						(pIniDesc + index)->key = l_tszKey;
						(pIniDesc + index)->data = &NanoSysData.SubstrateThicknessTable.SubstrateThickness[SUBSTRATE_THICKNESS_MAX - l_iThicknessCount].dThickness;
						l_iThicknessCount--;
					}
				}
				if (0 == l_iInvalidRegionCount) {
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, INVALID_REGION_N))) {
						l_ptszKeyOrg = (pIniDesc + index)->key;
						l_iInvalidRegionCount = SUBSTRATE_THICKNESS_MAX;
					}
				}
				if (0 < l_iInvalidRegionCount) {
					if (l_ptszKeyOrg) {
					(pIniDesc + index)->key = l_ptszKeyOrg;
					}
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, INVALID_REGION_N))) {
						_stprintf_s(l_tszKey, _countof(l_tszKey), INVALID_REGION_N, SUBSTRATE_THICKNESS_MAX - l_iInvalidRegionCount);
						(pIniDesc + index)->key = l_tszKey;
						(pIniDesc + index)->data = &NanoSysData.SubstrateThicknessTable.SubstrateThickness[SUBSTRATE_THICKNESS_MAX - l_iInvalidRegionCount].dInvalidRegion;
						l_iInvalidRegionCount--;
					}
				}
				if (0 == l_iUseDataCount) {
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, USE_DATA_N))) {
						l_ptszKeyOrg = (pIniDesc + index)->key;
						l_iUseDataCount = SUBSTRATE_THICKNESS_MAX;
					}
				}
				if (0 < l_iUseDataCount) {
					if (l_ptszKeyOrg) {
					(pIniDesc + index)->key = l_ptszKeyOrg;
					}
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, USE_DATA_N))) {
						_stprintf_s(l_tszKey, _countof(l_tszKey), USE_DATA_N, SUBSTRATE_THICKNESS_MAX - l_iUseDataCount);
						(pIniDesc + index)->key = l_tszKey;
						(pIniDesc + index)->data = &NanoSysData.SubstrateThicknessTable.SubstrateThickness[SUBSTRATE_THICKNESS_MAX - l_iUseDataCount].bUseData;
						l_iUseDataCount--;
					}
				}

				::GetPrivateProfileString(section,
										(pIniDesc + index)->key,
										(pIniDesc + index)->def,
										buff,
										sizeof(buff),
										szIniFilePath);

				// set ini file data to variable
				switch((pIniDesc + index)->type){
				case 'b':
					*(BOOL*)(pIniDesc + index)->data = (strcmp(buff, "TRUE") == 0)? TRUE: FALSE;
					break;
				case 'B':
					*(BYTE*)(pIniDesc + index)->data = (BYTE)atoi(buff);
					break;
				case 'S':
					*(short*)(pIniDesc + index)->data = (short)atoi(buff);
					break;
				case 'I':
					*(int*)(pIniDesc + index)->data = (int)atoi(buff);
					break;
				case 'L':
					*(long*)(pIniDesc + index)->data = (long)atol(buff);
					break;
				case 'F':
					*(float*)(pIniDesc + index)->data = (float)atof(buff);
					break;
				case 'D':
					*(double*)(pIniDesc + index)->data = (double)atof(buff);
					break;
// 2009.05.14 k-matsuo -->
				case '#':
					*(double*)(pIniDesc + index)->data = (double)atof(buff);
					break;
// 2009.05.14 k-matsuo <--
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --{--
				case 'E':
					*(double*)(pIniDesc + index)->data = strtod(buff, NULL);
					break;
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --}--
				case 'W':
					*(WORD*)(pIniDesc + index)->data = (WORD)atoi(buff);
					break;
				case 'C':
					strcpy_s((char*)(pIniDesc + index)->data, 256, buff);
					break;
				default:
					(void*)(pIniDesc + index)->data = NULL;
					break;
				}
			}
		}
	}
// 2013.02.22 bagus Substrate thickness setting <--

//2009.09.28 bagus gantry --{--
	if (iIniFile == USER_SETTING_SR_INI_FILE) {
		int	i;
		TCHAR l_tszKey[255];

		for (i=0; i<MAX_GANTRY_MEAS_POINT_NUM; i++) {
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_X", i + 1);
			UserData.SrReferencePosition.LocGantryTransmitReference[i].lX = ::GetPrivateProfileInt(_T("REFERENCE POSITION"), l_tszKey, 0, szIniFilePath);
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_Y", i + 1);
			UserData.SrReferencePosition.LocGantryTransmitReference[i].lY = ::GetPrivateProfileInt(_T("REFERENCE POSITION"), l_tszKey, 0, szIniFilePath);
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_Z", i + 1);
			UserData.SrReferencePosition.LocGantryTransmitReference[i].lZ = ::GetPrivateProfileInt(_T("REFERENCE POSITION"), l_tszKey, 0, szIniFilePath);
		}
	}
//2009.09.28 bagus gantry --}--

// 2009.09.29 bagus SE --{--
	if (iIniFile == SYSTEM_SR_INI_FILE) {
		NanoSysData.SrConfig.wTrShutterMoveWaitTime = lg_TrShutterMoveWaitTime * 1000;
	}
// 2009.09.29 bagus SE --}--

	if(pIniDesc != NULL){
		delete [] pIniDesc;
		pIniDesc = NULL;
	}
/* added 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- { ---------- */
	if (SYSTEM_SR_INI_FILE == iIniFile) {
//		for (int iLenss = 1; iLenss <= SR_LENS_MAX; iLenss++) {
		/*	SR_LENS_MAX ｿｽﾍ「5ｿｽvｿｽｿｽｿｽｿｽｿｽﾇ，ｿｽﾇ搾ｿｽｿｽﾝゑｿｽｿｽS(0ｿｽ`3)ｿｽﾜでゑｿｽｿｽｿｽｿｽﾎ会ｿｽｿｽｿｽｿｽﾄゑｿｽｿｽﾈゑｿｽｿｽﾌゑｿｽ
			ｿｽu5ｿｽvｿｽﾉゑｿｽｿｽﾄゑｿｽｿｽｿｽｿｽﾜゑｿｽｿｽD	*/
		for (int iLenss = 1; iLenss <= 4; iLenss++) {
			if (SR_LENS_MAX < NanoSysData.SrTurret[iLenss - 1].iAFLens) {
				NanoSysData.SrTurret[iLenss - 1].iAFLens = iLenss - 1;
			}
		}
	}
/* added 2009.09.29 hmenjo Auto Focus Turret ｿｽfｿｽtｿｽHｿｽｿｽｿｽg ---------- } ---------- */
// 2013.11.14 Bagus Add (TohoSpecｿｽﾎ会ｿｽ) -->
	if(g_lModelType == MODEL_T3100){
		if (iIniFile == SYSTEM_COMMON_INI_FILE){
			// System Config
			NanoSysData.SystemConfig.HeadType.bSE			= FALSE;
			NanoSysData.SystemConfig.HeadType.bCompEASE		= FALSE;
			NanoSysData.SystemConfig.HeadType.bResist		= FALSE;
			NanoSysData.SystemConfig.HeadType.bCTA			= FALSE;
			NanoSysData.SystemConfig.HeadType.bStress		= FALSE;
			NanoSysData.SystemConfig.HeadType.bMS			= FALSE;
			NanoSysData.SystemConfig.nCaptureBoard			= 0;
			NanoSysData.SystemConfig.nPRMethod				= 0;
			NanoSysData.SystemConfig.bSamplePatternRec		= FALSE;
			NanoSysData.SystemConfig.bSitePatternRec		= FALSE;
			NanoSysData.SystemConfig.bHost					= FALSE;
			NanoSysData.SystemConfig.bSignalTower			= FALSE;
			NanoSysData.SystemConfig.bDisableNextra			= TRUE;	// disable NEXTRA.exe
			NanoSysData.SystemConfig.nStageType				= 0;
			NanoSysData.SystemConfig.nJoystickType			= 0;
			NanoSysData.SystemConfig.nVibrationControlUnit	= 0;
			NanoSysData.SystemConfig.bVibrationDownSensor	= FALSE;
			NanoSysData.SystemConfig.bSampleExistenceSensor	= FALSE;
			NanoSysData.SystemConfig.nSampleLifter			= 0;
			NanoSysData.SystemConfig.bSampleAligner			= FALSE;
			NanoSysData.SystemConfig.nVacuumType			= 0;
			NanoSysData.SystemConfig.iVacuumTimeout			= 0;
			NanoSysData.SystemConfig.dCorrectAngle			= 0.000;
			NanoSysData.SystemConfig.bMultiRecipe			= FALSE;
			NanoSysData.SystemConfig.bExtraViewCamera		= FALSE;
			NanoSysData.SystemConfig.bUseLoadingShutter		= FALSE;
		}
		if (iIniFile == SYSTEM_SR_INI_FILE) {
			// SR Config
			NanoSysData.SrConfig.nTurret					= 0;
			NanoSysData.SrConfig.bAutoFocus					= FALSE;
			NanoSysData.SrConfig.bChooseAfFuncInReference	= FALSE;
			NanoSysData.SrConfig.bAutoFilter				= FALSE;
			NanoSysData.SrConfig.bUseOptFilterMeasPgmAF		= FALSE;
			NanoSysData.SrConfig.bBuiltInReference 			= FALSE;
			NanoSysData.SrConfig.bUv						= FALSE;
			NanoSysData.SrConfig.bUvAutoShutter				= FALSE;
			NanoSysData.SrConfig.dUvGain					= 0.000;
			NanoSysData.SrConfig.bTransmittance				= FALSE;
			NanoSysData.SrConfig.iTrUseLensNo				= 0;	// Turret No:1
			NanoSysData.SrConfig.wTrShutterMoveWaitTime		= 0.000;
			NanoSysData.SrConfig.bGantryType				= FALSE;
			NanoSysData.SrConfig.wGantryNumOfMeasPoint		= 1;
			NanoSysData.SrConfig.bDistance					= FALSE;

			// Turret
			NanoSysData.SrTurret[0].iAFLens					= 0;	// Turret No:1
			NanoSysData.SrTurret[1].iAFLens					= 0;	// Turret No:1
			NanoSysData.SrTurret[2].iAFLens					= 0;	// Turret No:1
			NanoSysData.SrTurret[3].iAFLens					= 0;	// Turret No:1
		}
	}
// 2013.11.14 Bagus Add (TohoSpecｿｽﾎ会ｿｽ) <--
}

/////////////////////////////////////////////////////////////////////////////
// Name       : SaveNanoSpecIni
// Purpose    : Iniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾌ保托ｿｽ
// Parameters : iIniFile ---> ｿｽﾇゑｿｽIniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾈのゑｿｽｿｽｿｽｿｽｿｽｿｽﾟゑｿｽﾏ撰ｿｽ
//
// Returns    : ｿｽﾈゑｿｽ
void SaveNanoSpecIni(int iIniFile)
{
	int index, iIniDescCount;
	LPCSTR section;
	char buff[256], szIniFilePath[_MAX_PATH];
	INIFILE* pIniDesc = NULL;

	memset(szIniFilePath, 0, sizeof(szIniFilePath));
	switch(iIniFile){
	case SYSTEM_COMMON_INI_FILE:
		iIniDescCount = sizeof(SystemCommonIniDesc) / sizeof(SystemCommonIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemCommonIniDesc, sizeof(SystemCommonIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
	case SYSTEM_SR_INI_FILE:
		iIniDescCount = sizeof(SystemSrIniDesc) / sizeof(SystemSrIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemSrIniDesc, sizeof(SystemSrIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
// 2009.09.29 bagus SE --{--
		if (iIniFile == SYSTEM_SR_INI_FILE) {
//			lg_TrShutterMoveWaitTime = (long)((((double)(NanoSysData.SrConfig.wTrShutterMoveWaitTime) / 1000) * 1000 + 500) / 1000);
//			lg_TrShutterMoveWaitTime = (double)((((double)(NanoSysData.SrConfig.wTrShutterMoveWaitTime) / 1000) * 1000 + 500) / 1000);
			lg_TrShutterMoveWaitTime = (double)(NanoSysData.SrConfig.wTrShutterMoveWaitTime / 1000.0);
		}
// 2009.09.29 bagus SE --}--
		break;
	//2009.09.07 bagus se --{--
	case SYSTEM_SE_INI_FILE:
		iIniDescCount = sizeof(SystemSeIniDesc) / sizeof(SystemSeIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemSeIniDesc, sizeof(SystemSeIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
	//2009.09.07 bagus se --}--
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	case SYSTEM_MS_INI_FILE:
		iIniDescCount = sizeof(SystemMsIniDesc) / sizeof(SystemMsIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemMsIniDesc, sizeof(SystemMsIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --{--
	case SYSTEM_CA_INI_FILE:
		iIniDescCount = sizeof(SystemCaIniDesc) / sizeof(SystemCaIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemCaIniDesc, sizeof(SystemCaIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --}--
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
	case SYSTEM_RS_INI_FILE:
		iIniDescCount = sizeof(SystemRsIniDesc) / sizeof(SystemRsIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemRsIniDesc, sizeof(SystemRsIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--
	case USER_SETTING_COMMON_INI_FILE:
		iIniDescCount = sizeof(UserSettingCommonIniDesc) / sizeof(UserSettingCommonIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingCommonIniDesc, sizeof(UserSettingCommonIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
	case USER_SETTING_SR_INI_FILE:
		iIniDescCount = sizeof(UserSettingSrIniDesc) / sizeof(UserSettingSrIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingSrIniDesc, sizeof(UserSettingSrIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.09.19 bagus SE --{--
	case USER_SETTING_SE_INI_FILE:
		iIniDescCount = sizeof(UserSettingSeIniDesc) / sizeof(UserSettingSeIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingSeIniDesc, sizeof(UserSettingSeIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2009.09.19 bagus SE --}--
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case USER_SETTING_COMPEASE_INI_FILE:
		iIniDescCount = sizeof(UserSettingCompEASEIniDesc) / sizeof(UserSettingCompEASEIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, UserSettingCompEASEIniDesc, sizeof(UserSettingCompEASEIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	//2009.08.24 bagus --{--
	case SYSTEM_STRESS_INI_FILE:
		iIniDescCount = sizeof(SystemStressIniDesc) / sizeof(SystemStressIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemStressIniDesc, sizeof(SystemStressIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		//2009.12.23 bagus stress ｿｽCｿｽｿｽ --{--
		//ｿｽｿｽUｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽｿｽB
		//ｿｽｿｽｿｽｿｽｿｽLSｿｽﾌ設抵ｿｽﾌ昔の値ｿｽｿｽｿｽcｿｽｿｽﾈゑｿｽｿｽ謔､ｿｽﾉゑｿｽｿｽ驍ｽｿｽﾟでゑｿｽｿｽB
		DeleteFile(szIniFilePath);
		//2009.12.23 bagus stress ｿｽCｿｽｿｽ --}--
		break;
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//	case SYSTEM_STRESS_MODULUS_INI_FILE:
	case USER_STRESS_MODULUS_INI_FILE:
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//		iIniDescCount = sizeof(SystemStressModulusIniDesc) / sizeof(SystemStressModulusIniDesc[0]);
		iIniDescCount = sizeof(UserStressModulusIniDesc) / sizeof(UserStressModulusIniDesc[0]);
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
		pIniDesc = new INIFILE [iIniDescCount];
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//		memcpy(pIniDesc, SystemStressModulusIniDesc, sizeof(SystemStressModulusIniDesc));
		memcpy(pIniDesc, UserStressModulusIniDesc, sizeof(UserStressModulusIniDesc));
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
		// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
		//sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_USER_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
		break;
	//2009.08.24 bagus --}--
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case SYSTEM_COMPEASE_INI_FILE:
		iIniDescCount = sizeof(SystemCompEaseIniDesc) / sizeof(SystemCompEaseIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SystemCompEaseIniDesc, sizeof(SystemCompEaseIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--
// 2013.02.22 bagus Substrate thickness setting -->
	case SUBSTRATE_THICKNESS_INI_FILE:
		iIniDescCount = sizeof(SubstrateThicknessIniDesc) / sizeof(SubstrateThicknessIniDesc[0]);
		pIniDesc = new INIFILE [iIniDescCount];
		memcpy(pIniDesc, SubstrateThicknessIniDesc, sizeof(SubstrateThicknessIniDesc));
		sprintf_s(szIniFilePath, _MAX_PATH, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[iIniFile]);
		break;
// 2013.02.22 bagus Substrate thickness setting <--
	default:
		return;
		break;
	}
/* added 2009.07.07 hmenjo dll ｿｽｿｽｿｽﾎパｿｽXｿｽﾎ会ｿｽ ConfigFile.dll ---------- { ---------- */
	TCHAR l_tszTempFName[_MAX_PATH];
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	_tcscpy_s(l_tszTempFName, _MAX_PATH, szIniFilePath);
	_stprintf_s(szIniFilePath, _MAX_PATH, _T("%s%s"), g_tszProcDir, l_tszTempFName);
/* added 2009.07.07 hmenjo dll ｿｽｿｽｿｽﾎパｿｽXｿｽﾎ会ｿｽ ConfigFile.dll ---------- } ---------- */


//2009.08.27 bagus stress --{--
	if(iIniFile == SYSTEM_STRESS_INI_FILE){
		int nIniStressValidLineNum = 0;
		int nIniStressLine;
		int nIniStressPin;
		int nIniStressSection;
// 2009.09.17 bagus Stress --{--
//		for(nIniStressLine = 0;nIniStressLine < STRESS_LINES_MAX;nIniStressLine++){
		for(nIniStressLine = 0;nIniStressLine < static_cast<int>(NanoSysData.StressConfig.dwLiftPinNumberOfLine);nIniStressLine++){
// 2009.09.17 bagus Stress --}--
			int nIniStressValidSectionNum = 0;
			int nIniStressValidPinNum = 0;
			char szBuf[1024+1];
			section = "LINE SECTION";
			sprintf_s(szBuf, sizeof(szBuf), "ValidLine%d", nIniStressLine+1);
//			_stprintf_s(buff, _countof(buff), _T("%s"), NanoSysData.StressConfig.Line[nIniStressLine].bValidLine ? "TRUE" : "FALSE");
			_stprintf_s(buff, _countof(buff), _T("%s"), NanoSysData.StressConfig.Line[nIniStressLine].bValidLine ? "1" : "0");
			WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

			sprintf_s(szBuf, sizeof(szBuf), "LiftPinNumberOfL%d", nIniStressLine+1);
			_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn);
			WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

// 2009.10.07 bagus Stress --{--
#if 0
// 2009.09.17 bagus SE --{--
//			for(nIniStressPin = 0;nIniStressPin < STRESS_PINS_MAX;nIniStressPin++){
			for(nIniStressPin = 0;nIniStressPin < static_cast<int>(NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn);nIniStressPin++){
// 2009.09.17 bagus SE --}--
				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_X", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lX);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_Y", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lY);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_X", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lX);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_Y", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lY);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);
		}
#else
			for (nIniStressPin=0; nIniStressPin<static_cast<int>(NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn); nIniStressPin++) {
				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_X", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lX);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "LiftPinLocL%dPin%d_Y", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].PinPos[nIniStressPin].lY);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);
			}

			for (nIniStressPin=0; nIniStressPin<PIN_INTERVAL; nIniStressPin++) {
				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_X", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lX);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "L%dP%d_Thick_Y", nIniStressLine+1,nIniStressPin+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].MeasPos[nIniStressPin].lY);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);
			}
#endif
// 2009.10.07 bagus Stress --}--
// 2009.09.17 bagus SE --{--
//			for(nIniStressSection = 0;nIniStressSection < STRESS_SECTIONS_MAX;nIniStressSection++){
			if (NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn < 1)
				continue;

			for(nIniStressSection = 0;nIniStressSection < NanoSysData.StressConfig.Line[nIniStressLine].dwLiftPinNumberOfLn - 1;nIniStressSection++){
// 2009.09.17 bagus SE --}--
				sprintf_s(szBuf, sizeof(szBuf), "L%dS%d_ScanStart_X", nIniStressLine+1,nIniStressSection+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].SectPos[nIniStressSection].lScanStartPosX);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "L%dS%d_ScanEnd_X", nIniStressLine+1,nIniStressSection+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].SectPos[nIniStressSection].lScanEndPosX);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);

				sprintf_s(szBuf, sizeof(szBuf), "L%dS%d_ScanPos_Y", nIniStressLine+1,nIniStressSection+1);
				_stprintf_s(buff, _countof(buff), _T("%ld"), NanoSysData.StressConfig.Line[nIniStressLine].SectPos[nIniStressSection].lScanPosY);
				WritePrivateProfileString(section,szBuf,buff,szIniFilePath);
			}
		}
	}
//2009.08.27 --}--

// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	if(iIniFile == SYSTEM_MS_INI_FILE){
		int	nIndex;
		char szMsKey[1024+1];

		section = "ROBO CYLINDER POSITION";
		for (nIndex=0; nIndex<MAX_MS_ROBO_CYLINDER_NUM; nIndex++) {
			sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Enable", nIndex+1);
			_stprintf_s(buff, _countof(buff), _T("%s"), NanoSysData.MsConfig.RoboCylinder[nIndex].bEnable ? "TRUE" : "FALSE");
			WritePrivateProfileString(section, szMsKey, buff, szIniFilePath);

			if (NanoSysData.MsConfig.RoboCylinder[nIndex].bEnable) {
				sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Name", nIndex+1);
				WritePrivateProfileString(section, szMsKey, NanoSysData.MsConfig.RoboCylinder[nIndex].szName, szIniFilePath);

				sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Position", nIndex+1);
				_stprintf_s(buff, _countof(buff), _T("%d"), NanoSysData.MsConfig.RoboCylinder[nIndex].nPosition);
				WritePrivateProfileString(section, szMsKey, buff, szIniFilePath);

				sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Sensor", nIndex+1);
				_stprintf_s(buff, _countof(buff), _T("%s"), NanoSysData.MsConfig.RoboCylinder[nIndex].bSensor ? "TRUE" : "FALSE");
				WritePrivateProfileString(section, szMsKey, buff, szIniFilePath);
			} else {
				sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Name", nIndex+1);
				WritePrivateProfileString(section, szMsKey, "", szIniFilePath);

				sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Position", nIndex+1);
				WritePrivateProfileString(section, szMsKey, "", szIniFilePath);

				sprintf_s(szMsKey, sizeof(szMsKey), "Index%d_Sensor", nIndex+1);
				WritePrivateProfileString(section, szMsKey, "FALSE", szIniFilePath);
			}
		}
	}
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--

/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
	int l_iLineCount = 0;
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
//2009.08.24 bagus stress --{--
// Stress ElasticModulus ｿｽﾎ会ｿｽ
	int l_iTableNameCount = 0;
	int l_iTableValueCount = 0;
//2009.08.24 bagus stress --}--
// 2013.02.22 bagus Substrate thickness setting -->
	int l_iThicknessCount = 0;
	int l_iInvalidRegionCount = 0;
	int l_iUseDataCount = 0;
// 2013.02.22 bagus Substrate thickness setting <--

// 2013.02.22 bagus Substrate thickness setting -->
	if(SUBSTRATE_THICKNESS_INI_FILE != iIniFile){
// 2013.02.22 bagus Substrate thickness setting <--
	for(index = 0; index < iIniDescCount; index++){
		// get section name
		if (strlen((pIniDesc + index)->section) > 0)
			section = (pIniDesc + index)->section;
#if 1
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
		if (0 != l_iLineCount) {
			if (0 != index) {
				index--;
			}
		}
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
		// get section name
		if(strlen((pIniDesc + index)->section) > 0)
			section = (pIniDesc + index)->section;

		// get ini file data
		memset(buff, NULL, sizeof(buff));
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
		TCHAR l_tszKey[256];
		LPCTSTR l_ptszKeyOrg = nullptr;
		if (0 == l_iLineCount) {
			if ((SYSTEM_STRESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, LINEnSECNUM))) {
				l_ptszKeyOrg = (pIniDesc + index)->key;
				l_iLineCount = NanoSysData.StressConfig.dwLiftPinNumberOfLine;
			}
		}
		if (0 < l_iLineCount) {
			if (l_ptszKeyOrg) {
			(pIniDesc + index)->key = l_ptszKeyOrg;
			}
			if ((SYSTEM_STRESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, LINEnSECNUM))) {
				_stprintf_s(l_tszKey, _countof(l_tszKey), LINEnSECNUM, NanoSysData.StressConfig.dwLiftPinNumberOfLine - l_iLineCount + 1);
				(pIniDesc + index)->key = l_tszKey;
				(pIniDesc + index)->data = &NanoSysData.StressConfig.Line[STRESS_LINES_MAX - l_iLineCount].dwSectionNum;
				l_iLineCount--;
			}
		}
#else
		TCHAR l_tszKey[256];
		LPCTSTR l_ptszKeyOrg = nullptr;
#endif
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
//2009.08.24 bagus stress --{--
// Stress ElasticModulus ｿｽﾎ会ｿｽ
		if (0 != l_iTableNameCount) {
			if (0 != index) {
				index--;
			}
		}

		// get ini file data
		memset(buff, NULL, sizeof(buff));
		if (0 == l_iTableNameCount) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				l_ptszKeyOrg = (pIniDesc + index)->key;
				l_iTableNameCount = NanoSysData.StressElasticTable.uiTableNum;
			}
		}
		if (0 < l_iTableNameCount) {
			if (l_ptszKeyOrg) {
			(pIniDesc + index)->key = l_ptszKeyOrg;
			}
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLENAME_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				_stprintf_s(l_tszKey, _countof(l_tszKey), TABLENAME_N, NanoSysData.StressElasticTable.uiTableNum - l_iTableNameCount + 1);
				(pIniDesc + index)->key = l_tszKey;
				(pIniDesc + index)->data = &NanoSysData.StressElasticTable.ElasticModulus[NanoSysData.StressElasticTable.uiTableNum - l_iTableNameCount].tszElasticModulusName;
				l_iTableNameCount--;
			}
		}
		if (0 != l_iTableValueCount) {
			if (0 != index) {
				index--;
			}
		}
		if (0 == l_iTableValueCount) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				l_ptszKeyOrg = (pIniDesc + index)->key;
				l_iTableValueCount = NanoSysData.StressElasticTable.uiTableNum;
			}
		}
		if (0 < l_iTableValueCount) {
			if (l_ptszKeyOrg) {
			(pIniDesc + index)->key = l_ptszKeyOrg;
			}
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --{--
//			if ((SYSTEM_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
			if ((USER_STRESS_MODULUS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, TABLEVALUE_N))) {
// 2009.11.09 bagus Stress ｿｽﾇ会ｿｽｿｽﾏ更 --}--
				_stprintf_s(l_tszKey, _countof(l_tszKey), TABLEVALUE_N, NanoSysData.StressElasticTable.uiTableNum - l_iTableValueCount + 1);
				(pIniDesc + index)->key = l_tszKey;
				(pIniDesc + index)->data = &NanoSysData.StressElasticTable.ElasticModulus[NanoSysData.StressElasticTable.uiTableNum - l_iTableValueCount].dElasticModulusValue;
				l_iTableValueCount--;
			}
		}

//2009.08.24 bagus stress --}--

		// set ini file data
		memset(buff, NULL, sizeof(buff));
		switch((pIniDesc + index)->type){
		case 'b':
			strcpy(buff, (*(BOOL*)(pIniDesc + index)->data)? "TRUE": "FALSE");
			break;
		case 'B':
			_itoa(*(BYTE*)(pIniDesc + index)->data, buff, 10);
			break;
		case 'S':
			_itoa(*(short*)(pIniDesc + index)->data, buff, 10);
			break;
		case 'I':
			_itoa(*(int*)(pIniDesc + index)->data, buff, 10);
			break;
		case 'L':
			_ltoa(*(long*)(pIniDesc + index)->data, buff, 10);
			break;
		case 'F':
			sprintf(buff, "%.3f", *(float*)(pIniDesc + index)->data);
			break;
		case 'D':
			sprintf(buff, "%.3lf", *(double*)(pIniDesc + index)->data);
			break;
// 2009.05.14 k-matsuo -->
		case '#':
			sprintf(buff, "%.10lf", *(double*)(pIniDesc + index)->data);
			break;
// 2009.05.14 k-matsuo <--
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --{--
		case 'E':
			sprintf(buff, "%e", *(double*)(pIniDesc + index)->data);
			break;
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --}--
		case 'W':
			sprintf(buff, "%d", *(WORD*)(pIniDesc + index)->data);
			break;
		case 'C':
			strncpy(buff, (char*)(pIniDesc + index)->data, sizeof(buff));
			break;
		default:
			break;
		}

		// write NanoSpec ini file
		::WritePrivateProfileString(section,
									(pIniDesc + index)->key,
									buff,
									szIniFilePath);
	}
// 2013.02.22 bagus Substrate thickness setting -->
	}
	else{
		TCHAR l_tszKey[256];
		LPCTSTR l_ptszKeyOrg = nullptr;
		for(index = 0; index < iIniDescCount; index++){
			// get section name
			if (strlen((pIniDesc + index)->section) > 0)
				section = (pIniDesc + index)->section;

			for(int i = 0; i < SUBSTRATE_THICKNESS_MAX; i++){

				// get ini file data
				if (0 == l_iThicknessCount) {
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, SUBSTRATE_THICKNESS_N))) {
						l_ptszKeyOrg = (pIniDesc + index)->key;
						l_iThicknessCount = SUBSTRATE_THICKNESS_MAX;
					}
				}
				if (0 < l_iThicknessCount) {
					if (l_ptszKeyOrg) {
					(pIniDesc + index)->key = l_ptszKeyOrg;
					}
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, SUBSTRATE_THICKNESS_N))) {
						_stprintf_s(l_tszKey, _countof(l_tszKey), SUBSTRATE_THICKNESS_N, SUBSTRATE_THICKNESS_MAX - l_iThicknessCount);
						(pIniDesc + index)->key = l_tszKey;
						(pIniDesc + index)->data = &NanoSysData.SubstrateThicknessTable.SubstrateThickness[SUBSTRATE_THICKNESS_MAX - l_iThicknessCount].dThickness;
						l_iThicknessCount--;
					}
				}
				if (0 == l_iInvalidRegionCount) {
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, INVALID_REGION_N))) {
						l_ptszKeyOrg = (pIniDesc + index)->key;
						l_iInvalidRegionCount = SUBSTRATE_THICKNESS_MAX;
					}
				}
				if (0 < l_iInvalidRegionCount) {
					if (l_ptszKeyOrg) {
					(pIniDesc + index)->key = l_ptszKeyOrg;
					}
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, INVALID_REGION_N))) {
						_stprintf_s(l_tszKey, _countof(l_tszKey), INVALID_REGION_N, SUBSTRATE_THICKNESS_MAX - l_iInvalidRegionCount);
						(pIniDesc + index)->key = l_tszKey;
						(pIniDesc + index)->data = &NanoSysData.SubstrateThicknessTable.SubstrateThickness[SUBSTRATE_THICKNESS_MAX - l_iInvalidRegionCount].dInvalidRegion;
						l_iInvalidRegionCount--;
					}
				}
				if (0 == l_iUseDataCount) {
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, USE_DATA_N))) {
						l_ptszKeyOrg = (pIniDesc + index)->key;
						l_iUseDataCount = SUBSTRATE_THICKNESS_MAX;
					}
				}
				if (0 < l_iUseDataCount) {
					if (l_ptszKeyOrg) {
					(pIniDesc + index)->key = l_ptszKeyOrg;
					}
					if ((SUBSTRATE_THICKNESS_INI_FILE == iIniFile) && (0 == _tcscmp((pIniDesc + index)->key, USE_DATA_N))) {
						_stprintf_s(l_tszKey, _countof(l_tszKey), USE_DATA_N, SUBSTRATE_THICKNESS_MAX - l_iUseDataCount);
						(pIniDesc + index)->key = l_tszKey;
						(pIniDesc + index)->data = &NanoSysData.SubstrateThicknessTable.SubstrateThickness[SUBSTRATE_THICKNESS_MAX - l_iUseDataCount].bUseData;
						l_iUseDataCount--;
					}
				}

				// set ini file data
				memset(buff, NULL, sizeof(buff));
				switch((pIniDesc + index)->type){
				case 'b':
					strcpy(buff, (*(BOOL*)(pIniDesc + index)->data)? "TRUE": "FALSE");
					break;
				case 'B':
					_itoa(*(BYTE*)(pIniDesc + index)->data, buff, 10);
					break;
				case 'S':
					_itoa(*(short*)(pIniDesc + index)->data, buff, 10);
					break;
				case 'I':
					_itoa(*(int*)(pIniDesc + index)->data, buff, 10);
					break;
				case 'L':
					_ltoa(*(long*)(pIniDesc + index)->data, buff, 10);
					break;
				case 'F':
					sprintf(buff, "%.3f", *(float*)(pIniDesc + index)->data);
					break;
				case 'D':
					sprintf(buff, "%.3lf", *(double*)(pIniDesc + index)->data);
					break;
// 2009.05.14 k-matsuo -->
				case '#':
					sprintf(buff, "%.10lf", *(double*)(pIniDesc + index)->data);
					break;
// 2009.05.14 k-matsuo <--
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --{--
				case 'E':
					sprintf(buff, "%e", *(double*)(pIniDesc + index)->data);
					break;
// 2009.09.04 bagus stress e+11ｿｽﾏ更 --}--
				case 'W':
					sprintf(buff, "%d", *(WORD*)(pIniDesc + index)->data);
					break;
				case 'C':
					strncpy(buff, (char*)(pIniDesc + index)->data, sizeof(buff));
					break;
				default:
					break;
				}

				// write NanoSpec ini file
				::WritePrivateProfileString(section,
											(pIniDesc + index)->key,
											buff,
											szIniFilePath);
			}
		}
	}
// 2013.02.22 bagus Substrate thickness setting <--

//2009.09.28 bagus gantry --{--
	if (iIniFile == USER_SETTING_SR_INI_FILE) {
		int	i;
		TCHAR l_tszKey[255];
		TCHAR l_tszTemp[255];

		char szSrIniFilePath[_MAX_PATH];
		int	nGantryNum;

		sprintf(szSrIniFilePath, "%s%s", CFG_SYSTEM_DIR, g_lpszNanoSpecIniFileName[SYSTEM_SR_INI_FILE]);
		nGantryNum = ::GetPrivateProfileInt(_T("CONFIG"), "GantryNumberOfMeasurePoint", 0, szSrIniFilePath);

//		for (i=0; i<MAX_GANTRY_MEAS_POINT_NUM; i++) {
		for (i=0; i<nGantryNum; i++) {
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_X", i + 1);
			_stprintf(l_tszTemp, _T("%ld"), UserData.SrReferencePosition.LocGantryTransmitReference[i].lX);
			WritePrivateProfileString(_T("REFERENCE POSITION"), l_tszKey, l_tszTemp, szIniFilePath);
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_Y", i + 1);
			_stprintf(l_tszTemp, _T("%ld"), UserData.SrReferencePosition.LocGantryTransmitReference[i].lY);
			WritePrivateProfileString(_T("REFERENCE POSITION"), l_tszKey, l_tszTemp, szIniFilePath);
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_Z", i + 1);
			_stprintf(l_tszTemp, _T("%ld"), UserData.SrReferencePosition.LocGantryTransmitReference[i].lZ);
			WritePrivateProfileString(_T("REFERENCE POSITION"), l_tszKey, l_tszTemp, szIniFilePath);
		}
		for (i=nGantryNum; i<MAX_GANTRY_MEAS_POINT_NUM; i++) {
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_X", i + 1);
			WritePrivateProfileString(_T("REFERENCE POSITION"), l_tszKey, _T("0"), szIniFilePath);
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_Y", i + 1);
			WritePrivateProfileString(_T("REFERENCE POSITION"), l_tszKey, _T("0"), szIniFilePath);
			_stprintf_s(l_tszKey, _countof(l_tszKey), "GTR%d_Reference_Z", i + 1);
			WritePrivateProfileString(_T("REFERENCE POSITION"), l_tszKey, _T("0"), szIniFilePath);
		}
	}
//2009.09.28 bagus gantry --}--

	if(pIniDesc != NULL){
		delete [] pIniDesc;
		pIniDesc = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Name       : LoadAllNanoSpecIni
// Purpose    : ｿｽSｿｽﾄゑｿｽIniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾌ難ｿｽｿｽeｿｽﾌ読み搾ｿｽｿｽｿｽ
// Parameters : ｿｽﾈゑｿｽ
//
// Returns    : ｿｽﾈゑｿｽ
void LoadAllNanoSpecIni()
{
	int i;
	for(i = 0; i < NANOSPEC_INI_FILE_MAX; i++){
		LoadNanoSpecIni(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Name       : SaveAllNanoSpecIni
// Purpose    : ｿｽSｿｽﾄゑｿｽIniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾌ保托ｿｽ
// Parameters : ｿｽﾈゑｿｽ
//
// Returns    : ｿｽﾈゑｿｽ
void SaveAllNanoSpecIni()
{
	int i;
	for(i = 0; i < NANOSPEC_INI_FILE_MAX; i++){
		SaveNanoSpecIni(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_GetNanoSpecIni
// Purpose    : Iniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾌ設抵ｿｽﾌ読み搾ｿｽｿｽｿｽ(ｿｽｿｽｿｽｿｽｿｽｿｽ)
// Parameters : pVoid ---> ｿｽfｿｽ[ｿｽ^ｿｽｿｽﾇみ搾ｿｽｿｽﾞ構ｿｽｿｽｿｽﾌ難ｿｽｿｽﾌポｿｽCｿｽｿｽｿｽ^
//              iType ---> ｿｽﾇのデｿｽ[ｿｽ^ｿｽｿｽﾇみ搾ｿｽｿｽﾞゑｿｽｿｽｿｽｿｽｿｽｿｽﾟゑｿｽﾏ撰ｿｽ
//
// Returns    : ｿｽﾈゑｿｽ
void GetNanoSpecIni(void* pVoid, int iType)
{
	switch(iType){

	// User Setting - All
	case CONFIG_FILE_USER_SETTING:
		memcpy(pVoid, &UserData, sizeof(USER_DATA));
		break;

	// User Setting - Common
	case CONFIG_FILE_MANUAL_LOAD_POSITION:
		memcpy(pVoid, &UserData.ManualLoadPosition, sizeof(MANUAL_LOAD_POSITION));
		break;
	case CONFIG_FILE_AUTO_LOGON_NAME:
		memcpy(pVoid, UserData.szAutoLogonName, USER_NAME_LEN + 1);
		break;
	case CONFIG_FILE_ACCESS_LEVEL:
		memcpy(pVoid, UserData.AccessLevel, sizeof(ACCESS_LEVEL) * ACCOUNT_LEVEL_MAX);
		break;
	case CONFIG_FILE_DELETE_DATA_PERIOD:
		memcpy(pVoid, &UserData.iDeleteDataPeriod, sizeof(int));
		break;
	case CONFIG_FILE_DEFAULT_HEAD_TYPE:
		memcpy(pVoid, &UserData.DefaultHeadType, sizeof(DEFAULT_HEAD_TYPE));
		break;
	case CONFIG_FILE_STAGE_MOTION_JOG:
		memcpy(pVoid, &UserData.StageMotionJogData[0], sizeof(STAGE_MOTION_JOG_DATA) * MAX_AXIS);
		break;
	case CONFIG_FILE_STAGE_MOTION_SPEED:
		memcpy(pVoid, &UserData.StageMotionSpeedData[0], sizeof(STAGE_MOTION_SPEED_DATA) * MAX_AXIS);
		break;
	case CONFIG_FILE_MAINTENANCE_POSITION:
		memcpy(pVoid, UserData.MaintenancePosition, sizeof(MAINTENANCE_POSITION) * MAINTENANCE_POSITION_MAX);
		break;
	case CONFIG_FILE_MAINTENANCE_DATE:
		memcpy(pVoid, &UserData.MaintenanceDate, sizeof(MAINTENANCE_DATE));
		break;
	case CONFIG_FILE_MAINTENANCE_LAMP:
		memcpy(pVoid, &UserData.MaintenanceLamp, sizeof(MAINTENANCE_LAMP));
		break;

	// User Setting - SR
	case CONFIG_FILE_SR_PATTERN_REC:
		memcpy(pVoid, &UserData.SrPatternRec, sizeof(SR_PATTERN_REC));
		break;
	case CONFIG_FILE_SR_REFERENCE_POSITION:
		memcpy(pVoid, &UserData.SrReferencePosition, sizeof(SR_REFERENCE_POSITION));
		break;
	case CONFIG_FILE_SR_FIGURE_FORMAT:
		memcpy(pVoid, &UserData.SrFigureFormat, sizeof(SR_FIGURE_FORMAT));
		break;
	case CONFIG_FILE_SR_AUTOFOCUS_RETRY:
		memcpy(pVoid, &UserData.SrAutoFocusRetry, sizeof(SR_AUTOFOCUS_RETRY));
		break;
/* added 2009.09.07 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽQ) ---------- { ---------- */
	case CONFIG_FILE_SR_TRANSMIT:
		memcpy(pVoid, &UserData.SrTransmittance, sizeof(SR_TRANSMIT));
		break;
/* added 2009.09.07 hmenjo ｿｽｿｽｿｽﾟ暦ｿｽ ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽQ) ---------- } ---------- */

/* added 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- { ---------- */
	/* User Setting - SE	*/
	case CONFIG_FILE_SE_SETTING:
		memcpy(pVoid, &UserData.SeSetting, sizeof(SE_SETTING));
		break;
/* added 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- } ---------- */
	//2009.09.07 bagus se --{--
	case CONFIG_FILE_SE_FIGURE_FORMAT:
		memcpy(pVoid, &UserData.SeFigureFormat, sizeof(SE_FIGURE_FORMAT));
		break;
	//2009.09.07 bagus se --}--
// 2013.02.01 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case CONFIG_FILE_COMPEASE_SETTING:
		memcpy(pVoid, &UserData.CompEASESetting, sizeof(COMPEASE_SETTING));
		break;
	case CONFIG_FILE_COMPEASE_FIGURE_FORMAT:
		memcpy(pVoid, &UserData.CompEASEFigureFormat, sizeof(COMPEASE_FIGURE_FORMAT));
		break;
// 2013.02.01 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--

	// System - All
	case CONFIG_FILE_SYSTEM:
		memcpy(pVoid, &NanoSysData, sizeof(NANOSPEC_SYSTEM_DATA));
		break;

	// System - Common
	case CONFIG_FILE_SYSTEM_CONFIG:
		memcpy(pVoid, &NanoSysData.SystemConfig, sizeof(SYSTEM_CONFIG));
		break;
	case CONFIG_FILE_ORIGINAL_POSITION:
		memcpy(pVoid, &NanoSysData.OriginalPosition, sizeof(ORIGINAL_POSITION));
		break;
	case CONFIG_FILE_STAGE_CONFIG:
		memcpy(pVoid, &NanoSysData.StageConfig, sizeof(STAGE_CONFIG));
		break;
	case CONFIG_FILE_FOCUS_CONFIG:
		memcpy(pVoid, &NanoSysData.FocusConfig, sizeof(FOCUS_CONFIG));
		break;
	case CONFIG_FILE_EDGE_ALIGNER:
		memcpy(pVoid, &NanoSysData.EdgeAligner, sizeof(EDGE_ALIGNER));
		break;
	case CONFIG_FILE_HANDLER_LOAD_POSITION:
		memcpy(pVoid, &NanoSysData.HandlerLoadPosition, sizeof(HANDLER_LOAD_POSITION));
		break;
	case CONFIG_FILE_GENERAL_POSITION:
		memcpy(pVoid, NanoSysData.GeneralPosition, sizeof(GENERAL_POSITION) * GENERAL_POSITION_MAX);
		break;
	case CONFIG_FILE_SIGNAL_TOWER:
		memcpy(pVoid, &NanoSysData.SignalTower, sizeof(SIGNAL_TOWER));
		break;
	case CONFIG_FILE_HOST:
		memcpy(pVoid, &NanoSysData.HostAddress, sizeof(HOST_ADDRESS));
		break;
	case CONFIG_FILE_COM:
		memcpy(pVoid, &NanoSysData.ComSetting, sizeof(COM_SETTING));
		break;
	case CONFIG_FILE_MULTI_RECIPE:
		break;
// 2013.01.11 bagus jog direction -->
	case CONFIG_FILE_JOG_DIR:
		memcpy(pVoid, &NanoSysData.JogDirSetting, sizeof(JOG_DIR_SETTING));
		break;
// 2013.01.11 bagus jog direction <--

	// System - SR
	case CONFIG_FILE_SR_CONFIG:
		memcpy(pVoid, &NanoSysData.SrConfig, sizeof(SR_CONFIG));
		break;
	case CONFIG_FILE_SR_FILTER:
		memcpy(pVoid, NanoSysData.SrFilter, sizeof(SR_FILTER) * SR_FILTER_MAX);
		break;
	case CONFIG_FILE_SR_TURRET:
		memcpy(pVoid, NanoSysData.SrTurret, sizeof(SR_TURRET) * SR_LENS_MAX);
		break;
	case CONFIG_FILE_SR_LENS_CENTER_OFFSET:
		memcpy(pVoid, &NanoSysData.SrLensCenterOffset, sizeof(SR_LENS_CENTER_OFFSET));
		break;
	case CONFIG_FILE_SR_XMP:
		memcpy(pVoid, &NanoSysData.SrXmp, sizeof(SR_XMP));
		break;
	case CONFIG_FILE_SR_VIEW_RANGE:
		memcpy(pVoid, &NanoSysData.SrViewRange, sizeof(SR_VIEW_RANGE));
		break;
	case CONFIG_FILE_SR_VIDEO_CALIBRATION:
		memcpy(pVoid, NanoSysData.SrVideoCalib, sizeof(SR_VIDEO_CALIB) * SR_LENS_MAX);
		break;
	case CONFIG_FILE_SR_CCD_ARRAY:
		memcpy(pVoid, NanoSysData.SrCcdArray, sizeof(SR_CCD_ARRAY) * SR_CCD_ARRAY_ITEM_MAX);
		break;
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- { ---------- */
	case CONFIG_FILE_STRESS_CONFIG:
		memcpy(pVoid, &NanoSysData.StressConfig, sizeof(STRESS_CONFIG));
		break;
/* added 2009.07.22 hmenjo ｿｽXｿｽgｿｽｿｽｿｽX ｿｽｿｽｿｽCｿｽｿｽｿｽｿｽ ｿｽｿｽ` ---------- } ---------- */
	case CONFIG_FILE_STRESS_MODULUS_CONFIG:
		memcpy(pVoid, &NanoSysData.StressElasticTable, sizeof(STRESS_ELASTIC_TABLE));
		break;
/* deleted 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- { ---------- */
///* added 2009.08.25 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- { ---------- */
//	/* System - SE	*/
//	case CONFIG_FILE_SE_CONFIG:
//		memcpy(pVoid, &NanoSysData.SeConfig, sizeof(SE_CONFIG));
//		break;
///* added 2009.08.25 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(ｿｽP) ---------- } ---------- */
/* deleted 2009.09.01 hmenjo SE ｿｽ\ｿｽｿｽｿｽﾌ追会ｿｽ(12) ---------- } ---------- */
	//2009.09.07 bagus se --{--
	case CONFIG_FILE_SE_XMP:
		memcpy(pVoid, &NanoSysData.SeXmp, sizeof(SE_XMP));
		break;
	//2009.09.07 bagus se --}--

// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	case CONFIG_FILE_MS_CONFIG:
		memcpy(pVoid, &NanoSysData.MsConfig, sizeof(MS_CONFIG));
		break;
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--

// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --{--
	case CONFIG_FILE_CA_CONFIG:
		memcpy(pVoid, &NanoSysData.CaConfig, sizeof(CA_CONFIG));
		break;
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --}--
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
	case CONFIG_FILE_RS_CONFIG:
		memcpy(pVoid, &NanoSysData.RsConfig, sizeof(RS_CONFIG));
		break;
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case CONFIG_FILE_COMPEASE_CONFIG:
		memcpy(pVoid, &NanoSysData.CompEASEConfig, sizeof(COMPEASE_CONFIG));
		break;
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--
// 2013.02.22 bagus Substrate thickness setting -->
	case CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG:
		memcpy(pVoid, &NanoSysData.SubstrateThicknessTable, sizeof(SUBSTRATE_THICKNESS_TABLE));
		break;
// 2013.02.22 bagus Substrate thickness setting <--

	// Head Config
	case CONFIG_FILE_HEAD_TYPE_CONFIG:
		memcpy(pVoid, &NanoSysData.SystemConfig.HeadType, sizeof(HEAD_TYPE_CONFIG));
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_SetNanoSpecIni
// Purpose    : Iniｿｽtｿｽ@ｿｽCｿｽｿｽｿｽﾌ設抵ｿｽﾌ保托ｿｽ(ｿｽｿｽｿｽｿｽｿｽｿｽ)
// Parameters : pVoid ---> ｿｽﾛ托ｿｽｿｽｿｽｿｽｿｽfｿｽ[ｿｽ^ｿｽﾌ構ｿｽｿｽｿｽﾌ難ｿｽｿｽﾌポｿｽCｿｽｿｽｿｽ^
//              iType ---> ｿｽﾇのデｿｽ[ｿｽ^ｿｽｿｽﾛ托ｿｽｿｽｿｽｿｽ驍ｩｿｽｿｽｿｽｿｽｿｽﾟゑｿｽﾏ撰ｿｽ
//
// Returns    : ｿｽﾈゑｿｽ
void SetNanoSpecIni(void* pVoid, int iType)
{
	switch(iType){

	// User Setting - All
	case CONFIG_FILE_USER_SETTING:
		memcpy(&UserData, pVoid, sizeof(USER_DATA));
		break;

	// User Setting - Common
	case CONFIG_FILE_MANUAL_LOAD_POSITION:
		memcpy(&UserData.ManualLoadPosition, pVoid, sizeof(MANUAL_LOAD_POSITION));
		break;
	case CONFIG_FILE_AUTO_LOGON_NAME:
		memcpy(UserData.szAutoLogonName, pVoid, USER_NAME_LEN + 1);
		break;
	case CONFIG_FILE_ACCESS_LEVEL:
		memcpy(UserData.AccessLevel, pVoid, sizeof(ACCESS_LEVEL) * ACCOUNT_LEVEL_MAX);
		break;
	case CONFIG_FILE_DELETE_DATA_PERIOD:
		memcpy(&UserData.iDeleteDataPeriod, pVoid, sizeof(int));
		break;
	case CONFIG_FILE_DEFAULT_HEAD_TYPE:
		memcpy(&UserData.DefaultHeadType, pVoid, sizeof(DEFAULT_HEAD_TYPE));
		break;
	case CONFIG_FILE_STAGE_MOTION_JOG:
		memcpy(&UserData.StageMotionJogData[0], pVoid, sizeof(STAGE_MOTION_JOG_DATA) * MAX_AXIS);
		break;
	case CONFIG_FILE_STAGE_MOTION_SPEED:
		memcpy(&UserData.StageMotionSpeedData[0], pVoid, sizeof(STAGE_MOTION_SPEED_DATA) * MAX_AXIS);
		break;
	case CONFIG_FILE_MAINTENANCE_POSITION:
		memcpy(UserData.MaintenancePosition, pVoid, sizeof(MAINTENANCE_POSITION) * MAINTENANCE_POSITION_MAX);
		break;
	case CONFIG_FILE_MAINTENANCE_DATE:
		memcpy(&UserData.MaintenanceDate, pVoid, sizeof(MAINTENANCE_DATE));
		break;
	case CONFIG_FILE_MAINTENANCE_LAMP:
		memcpy(&UserData.MaintenanceLamp, pVoid, sizeof(MAINTENANCE_LAMP));
		break;

	// User Setting - SR
	case CONFIG_FILE_SR_PATTERN_REC:
		memcpy(&UserData.SrPatternRec, pVoid, sizeof(SR_PATTERN_REC));
		break;
	case CONFIG_FILE_SR_REFERENCE_POSITION:
		memcpy(&UserData.SrReferencePosition, pVoid, sizeof(SR_REFERENCE_POSITION));
		break;
	case CONFIG_FILE_SR_FIGURE_FORMAT:
		memcpy(&UserData.SrFigureFormat, pVoid, sizeof(SR_FIGURE_FORMAT));
		break;
	case CONFIG_FILE_SR_AUTOFOCUS_RETRY:
		memcpy(&UserData.SrAutoFocusRetry, pVoid, sizeof(SR_AUTOFOCUS_RETRY));
		break;
	// 2009.09.30 bagus Gantry --{--
	case CONFIG_FILE_SR_TRANSMIT:
		memcpy(&UserData.SrTransmittance, pVoid, sizeof(SR_TRANSMIT));
		break;
	// 2009.09.30 bagus Gantry --}--
	/* User Setting - SE	*/
	//2009.09.07 bagus se --{--
	case CONFIG_FILE_SE_SETTING:
		//memcpy(pVoid, &UserData.SeSetting, sizeof(SE_SETTING));
		memcpy(&UserData.SeSetting, pVoid, sizeof(SE_SETTING));
		break;
	case CONFIG_FILE_SE_FIGURE_FORMAT:
	// bug fix 2013.02.01 -->
	    // memcpy(&UserData.SrFigureFormat, pVoid, sizeof(SE_FIGURE_FORMAT));
		memcpy(&UserData.SeFigureFormat, pVoid, sizeof(SE_FIGURE_FORMAT));
	// bug fix 2013.02.01 <--
		break;
	//2009.09.07 bagus se --}--
	// 2013.02.01 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case CONFIG_FILE_COMPEASE_SETTING:
		memcpy(&UserData.CompEASESetting, pVoid, sizeof(COMPEASE_SETTING));
		break;
	case CONFIG_FILE_COMPEASE_FIGURE_FORMAT:
		memcpy(&UserData.CompEASEFigureFormat, pVoid, sizeof(COMPEASE_FIGURE_FORMAT));
		break;
	// 2013.02.01 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--

	// System - All
	case CONFIG_FILE_SYSTEM:
		memcpy(&NanoSysData, pVoid, sizeof(NANOSPEC_SYSTEM_DATA));
		break;

	// System - Common
	case CONFIG_FILE_SYSTEM_CONFIG:
		memcpy(&NanoSysData.SystemConfig, pVoid, sizeof(SYSTEM_CONFIG));
		break;
	case CONFIG_FILE_ORIGINAL_POSITION:
		memcpy(&NanoSysData.OriginalPosition, pVoid, sizeof(ORIGINAL_POSITION));
		break;
	case CONFIG_FILE_STAGE_CONFIG:
		memcpy(&NanoSysData.StageConfig, pVoid, sizeof(STAGE_CONFIG));
		break;
	case CONFIG_FILE_FOCUS_CONFIG:
		memcpy(&NanoSysData.FocusConfig, pVoid, sizeof(FOCUS_CONFIG));
		break;
	case CONFIG_FILE_EDGE_ALIGNER:
		memcpy(&NanoSysData.EdgeAligner, pVoid, sizeof(EDGE_ALIGNER));
		break;
	case CONFIG_FILE_HANDLER_LOAD_POSITION:
		memcpy(&NanoSysData.HandlerLoadPosition, pVoid, sizeof(HANDLER_LOAD_POSITION));
		break;
	case CONFIG_FILE_GENERAL_POSITION:
		memcpy(NanoSysData.GeneralPosition, pVoid, sizeof(GENERAL_POSITION) * GENERAL_POSITION_MAX);
		break;
	case CONFIG_FILE_SIGNAL_TOWER:
		memcpy(&NanoSysData.SignalTower, pVoid, sizeof(SIGNAL_TOWER));
		break;
	case CONFIG_FILE_HOST:
		memcpy(&NanoSysData.HostAddress, pVoid, sizeof(HOST_ADDRESS));
		break;
	case CONFIG_FILE_COM:
		memcpy(&NanoSysData.ComSetting, pVoid, sizeof(COM_SETTING));
		break;
	case CONFIG_FILE_MULTI_RECIPE:
		break;
// 2013.01.11 bagus jog direction -->
	case CONFIG_FILE_JOG_DIR:
		memcpy(&NanoSysData.JogDirSetting, pVoid, sizeof(JOG_DIR_SETTING));
		break;
// 2013.01.11 bagus jog direction <--

	// System - SR
	case CONFIG_FILE_SR_CONFIG:
		memcpy(&NanoSysData.SrConfig, pVoid, sizeof(SR_CONFIG));
		break;
	case CONFIG_FILE_SR_FILTER:
		memcpy(NanoSysData.SrFilter, pVoid, sizeof(SR_FILTER) * SR_FILTER_MAX);
		break;
	case CONFIG_FILE_SR_TURRET:
		memcpy(NanoSysData.SrTurret, pVoid, sizeof(SR_TURRET) * SR_LENS_MAX);
		break;
	case CONFIG_FILE_SR_LENS_CENTER_OFFSET:
		memcpy(&NanoSysData.SrLensCenterOffset, pVoid, sizeof(SR_LENS_CENTER_OFFSET));
		break;
	case CONFIG_FILE_SR_XMP:
		memcpy(&NanoSysData.SrXmp, pVoid, sizeof(SR_XMP));
		break;
	case CONFIG_FILE_SR_VIEW_RANGE:
		memcpy(&NanoSysData.SrViewRange, pVoid, sizeof(SR_VIEW_RANGE));
		break;
	case CONFIG_FILE_SR_VIDEO_CALIBRATION:
		memcpy(NanoSysData.SrVideoCalib, pVoid, sizeof(SR_VIDEO_CALIB) * SR_LENS_MAX);
		break;
	case CONFIG_FILE_SR_CCD_ARRAY:
		memcpy(NanoSysData.SrCcdArray, pVoid, sizeof(SR_CCD_ARRAY) * SR_CCD_ARRAY_ITEM_MAX);
		break;
	//2009.08.24 bagus stress --{--
	case CONFIG_FILE_STRESS_CONFIG:
		memcpy(&NanoSysData.StressConfig, pVoid, sizeof(STRESS_CONFIG));
		break;
	case CONFIG_FILE_STRESS_MODULUS_CONFIG:
		memcpy(&NanoSysData.StressElasticTable, pVoid, sizeof(STRESS_ELASTIC_TABLE));
		break;
	//2009.08.24 bagus stress --}--
	//2009.09.07 bagus se --{--
	case CONFIG_FILE_SE_XMP:
		memcpy(&NanoSysData.SeXmp, pVoid, sizeof(SE_XMP));
		break;
	//2009.09.07 bagus se --}--
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --{--
	case CONFIG_FILE_MS_CONFIG:
		memcpy(&NanoSysData.MsConfig, pVoid, sizeof(MS_CONFIG));
		break;
// 2009.10.19 bagus MS ｿｽﾇ会ｿｽ --}--
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --{--
	case CONFIG_FILE_CA_CONFIG:
		memcpy(&NanoSysData.CaConfig, pVoid, sizeof(CA_CONFIG));
		break;
// 2009.10.30 bagus CA ｿｽﾇ会ｿｽ --}--
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --{--
	case CONFIG_FILE_RS_CONFIG:
		memcpy(&NanoSysData.RsConfig, pVoid, sizeof(RS_CONFIG));
		break;
// 2009.11.04 bagus RS ｿｽﾇ会ｿｽ --}--
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ -->
	case CONFIG_FILE_COMPEASE_CONFIG:
		memcpy(&NanoSysData.CompEASEConfig, pVoid, sizeof(COMPEASE_CONFIG));
		break;
// 2013.02.04 bagus CompleteEASEｿｽwｿｽbｿｽhｿｽﾇ会ｿｽ <--
// 2013.02.22 bagus Substrate thickness setting -->
	case CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG:
		memcpy(&NanoSysData.SubstrateThicknessTable, pVoid, sizeof(SUBSTRATE_THICKNESS_TABLE));
		break;
// 2013.02.22 bagus Substrate thickness setting <--
	default:
		break;
	}
}
