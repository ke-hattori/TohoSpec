#if !defined( _NEXIOTYPEA_H_ )
#define _NEXIOTYPEA_H_

#pragma once

#include <nexmisc.hxx>
#include <nexif.hxx>

#ifdef _DEBUG
#pragma comment(lib, "dtnsnexif.lib")
#else
#pragma comment(lib, "tnsnexif.lib")
#endif

using namespace nano;

typedef struct tagDINPTYPEA {
	union {
		basic_device buffer[EQPIOINP_BYTES];
		struct {
			struct {
				unsigned char EmergencyStop 			  : 1;
				unsigned char DoorInterlock 			  : 1;
				unsigned char EquipmentPower			  : 1;
#if defined( NEXIO_VER_210 )
				unsigned char SafetyPlug				  : 1;
				unsigned char HepaPower 				  : 1;
#else
				unsigned char Port0Bit3 				  : 1;
				unsigned char Port0Bit4 				  : 1;
#endif
				unsigned char Port0Bit5 				  : 1;
				unsigned char AirPressureLow			  : 1;
				unsigned char Movo2Alarm				  : 1;

				unsigned char Port1Bit0 				  : 1;
				unsigned char MaintenanceSwitch 		  : 1;
				unsigned char EngineerMaintenanceSwitch   : 1;
				unsigned char Port1Bit3 				  : 1;
				unsigned char Port1Bit4 				  : 1;
				unsigned char Port1Bit5 				  : 1;
#if defined( NEXIO_VER_210 )
				unsigned char StressScanStartPosX		  : 1;
				unsigned char StressScanEndPosX 		  : 1;
#else
				unsigned char Port1Bit6 				  : 1;
				unsigned char Port1Bit7 				  : 1;
#endif
				unsigned char ShutterOpen				  : 1;
				unsigned char ShutterTop1				  : 1;
				unsigned char ShutterBottom1			  : 1;
				unsigned char ShutterTop2				  : 1;
				unsigned char ShutterBottom2			  : 1;
				unsigned char RobotArm					  : 1;
				unsigned char Port2Bit6 				  : 1;
#if defined( NEXIO_VER_210 )
				unsigned char StressUnitAlarm			  : 1;
#else
				unsigned char Port2Bit7 				  : 1;
#endif
				unsigned char VacuumPressure1			  : 1;
				unsigned char VacuumPressure2			  : 1;
				unsigned char Port3Bit2 				  : 1;
#if defined( NEXIO_VER_210 )
				unsigned char GlassExistence1			  : 1;
				unsigned char GlassExistence2			  : 1;
#else
				unsigned char Port3Bit3 				  : 1;
				unsigned char GlassExistence			  : 1;
#endif
				unsigned char LoadPosX					  : 1;
				unsigned char LoadPosY					  : 1;
#if defined( NEXIO_VER_210 )
				unsigned char RefractPosZ				  : 1;
#else
				unsigned char Port3Bit7 				  : 1;
#endif
				struct {
					unsigned char On					  : 1;
					unsigned char Ready 				  : 1;
					unsigned char Alarm 				  : 1;
					unsigned char PosComp				  : 1;
					unsigned char UpperPos				  : 1;
					unsigned char AlignmentPos			  : 1;
					unsigned char LowerPos				  : 1;
					unsigned char DownPos				  : 1;
				} Servo[2];

				struct {
					union {
						unsigned short Guide;
						unsigned char  Block[2];
					};
				};
			} CN2;
			struct {
				unsigned char CAInterlock				  : 1;
				unsigned char Port0Bit1 				  : 1;
				unsigned char RSProbeHeadUpperPos		  : 1;
				unsigned char RSProbeHeadLowerPos		  : 1;
				unsigned char TransmitLightSourcePower	  : 1;
				unsigned char TransmitLight1Condition	  : 1;
				unsigned char TransmitLight2Condition	  : 1;
				unsigned char TransmitLight3Condition	  : 1;

				unsigned char MicroScopeModeSwitch		  : 1;
				unsigned char MicroScopeUpSwitch		  : 1;
				unsigned char MicroScopeDownSwitch		  : 1;
				unsigned char MicroScopeUpperPos		  : 1;
				unsigned char MicroScopeLowerPos1		  : 1;
				unsigned char MicroScopeLowerPos2		  : 1;
				unsigned char MicroScopeLensKind		  : 1;
				unsigned char Port1Bit7 				  : 1;

				struct {
					union {
						basic_device buffer;
						struct {
						unsigned char PM1				  : 1;
						unsigned char PM2				  : 1;
						unsigned char PM4				  : 1;
						unsigned char PM8				  : 1;
						unsigned char PM16				  : 1;
						unsigned char PM32				  : 1;
						unsigned char MOVE				  : 1;
						unsigned char MODES 			  : 1;

						unsigned char PZONE 			  : 1;
						unsigned char RMDS				  : 1;
						unsigned char HEND				  : 1;
						unsigned char PEND				  : 1;
						unsigned char SV				  : 1;
						unsigned char EMGS				  : 1;
						unsigned char ALM				  : 1;
						unsigned char Port3Bit7 		  : 1;
						};
					};
				} RoboCylinder;

				unsigned char Port4Bit0 				  : 1;
				unsigned char Port4Bit1 				  : 1;
				unsigned char Port4Bit2 				  : 1;
				unsigned char Port4Bit3 				  : 1;
				unsigned char Port4Bit4 				  : 1;
				unsigned char Port4Bit5 				  : 1;
				unsigned char Port4Bit6 				  : 1;
				unsigned char Port4Bit7 				  : 1;

				unsigned char Port5Bit0 				  : 1;
				unsigned char Port5Bit1 				  : 1;
				unsigned char Port5Bit2 				  : 1;
				unsigned char Port5Bit3 				  : 1;
				unsigned char Port5Bit4 				  : 1;
				unsigned char Port5Bit5 				  : 1;
				unsigned char Port5Bit6 				  : 1;
				unsigned char Port5Bit7 				  : 1;

				unsigned char Port6Bit0 				  : 1;
				unsigned char Port6Bit1 				  : 1;
				unsigned char Port6Bit2 				  : 1;
				unsigned char Port6Bit3 				  : 1;
				unsigned char Port6Bit4 				  : 1;
				unsigned char Port6Bit5 				  : 1;
				unsigned char Port6Bit6 				  : 1;
				unsigned char Port6Bit7 				  : 1;

				unsigned char Port7Bit0 				  : 1;
				unsigned char Port7Bit1 				  : 1;
				unsigned char Port7Bit2 				  : 1;
				unsigned char Port7Bit3 				  : 1;
				unsigned char Port7Bit4 				  : 1;
				unsigned char Port7Bit5 				  : 1;
				unsigned char Port7Bit6 				  : 1;
				unsigned char Port7Bit7 				  : 1;
			} CN3;
		};
	};
} DINPTYPEA, *PDINPTYPEA;

typedef struct tagDOUTTYPEA {
	union {
		basic_device buffer[EQPIOOUT_BYTES];
		struct {
			struct {
				unsigned char EquipmentStatusRun		  : 1;
				unsigned char EquipmentPowerOFF 		  : 1;
// matsuhisa 2013.06.04 modified >>>
//				unsigned char Port0Bit2 					: 1;
				unsigned char SubConPowerOFF 			  : 1;
// matsuhisa 2013.06.04 modified <<<
				unsigned char Port0Bit3 				  : 1;
#if defined( NEXIO_VER_210 )
				unsigned char HepaPowerOFF				  : 1;
#else
				unsigned char Port0Bit4 				  : 1;
#endif
				unsigned char Port0Bit5 				  : 1;
				unsigned char Port0Bit6 				  : 1;
				unsigned char Port0Bit7 				  : 1;

				unsigned char Port1Bit0 				  : 1;
				unsigned char Port1Bit1 				  : 1;
				unsigned char Port1Bit2 				  : 1;
				unsigned char Port1Bit3 				  : 1;
				unsigned char Port1Bit4 				  : 1;
				unsigned char Port1Bit5 				  : 1;
				unsigned char Port1Bit6 				  : 1;
				unsigned char Port1Bit7 				  : 1;

				unsigned char ShutterOpen				  : 1;
				unsigned char ShutterClose				  : 1;
				unsigned char Port2Bit2 				  : 1;
				unsigned char Port2Bit3 				  : 1;
				unsigned char Port2Bit4 				  : 1;
				unsigned char Port2Bit5 				  : 1;
				unsigned char Port2Bit6 				  : 1;
				unsigned char Port2Bit7 				  : 1;

				unsigned char VacuumPressure1			  : 1;
				unsigned char VacuumPressure2			  : 1;
				unsigned char VacuumDestruct1			  : 1;
				unsigned char VacuumDestruct2			  : 1;
				unsigned char AirFloat1 				  : 1;
				unsigned char AirFloat2 				  : 1;
				unsigned char Port3Bit6 				  : 1;
				unsigned char Port3Bit7 				  : 1;

				struct {
					unsigned char On					  : 1;
					unsigned char AlarmReset			  : 1;
					unsigned char Brake 				  : 1;
					unsigned char Bit3					  : 1;
					unsigned char Bit4					  : 1;
					unsigned char Bit5					  : 1;
					unsigned char Bit6					  : 1;
					unsigned char Bit7					  : 1;
				} Servo[2];

				unsigned char GuideFwd1 				  : 1;
				unsigned char GuideRev1 				  : 1;
				unsigned char GuideFwd2 				  : 1;
				unsigned char GuideRev2 				  : 1;
				unsigned char Port6Bit4 				  : 1;
				unsigned char Port6Bit5 				  : 1;
				unsigned char Port6Bit6 				  : 1;
				unsigned char Port6Bit7 				  : 1;

#if defined( NEXIO_VER_210 )
				unsigned char StressMeasTriger			  : 1;
#else
				unsigned char Port7Bit0 				  : 1;
#endif
				unsigned char Port7Bit1 				  : 1;
				unsigned char Port7Bit2 				  : 1;
				unsigned char Port7Bit3 				  : 1;
				unsigned char Port7Bit4 				  : 1;
				unsigned char Port7Bit5 				  : 1;
				unsigned char Port7Bit6 				  : 1;
				unsigned char Port7Bit7 				  : 1;
			} CN2;

			struct {
				unsigned char CAInterlock				  : 1;
				unsigned char Port0Bit1 				  : 1;
				unsigned char RSProbeHeadUp 			  : 1;
				unsigned char RSProbeHeadDown			  : 1;
				unsigned char TransmitLightSource1Shutter : 1;
				unsigned char TransmitLightSource2Shutter : 1;
				unsigned char TransmitLightSource3Shutter : 1;
				unsigned char Port0Bit7 				  : 1;

				unsigned char MicroScapeModeLamp		  : 1;
				unsigned char Port1Bit1 				  : 1;
				unsigned char Port1Bit2 				  : 1;
				unsigned char Port1Bit3 				  : 1;
				unsigned char Port1Bit4 				  : 1;
				unsigned char Port1Bit5 				  : 1;
				unsigned char Port1Bit6 				  : 1;
				unsigned char Port1Bit7 				  : 1;

				struct {
					union {
						basic_device buffer;
						struct {
							unsigned char PC1			  : 1;
							unsigned char PC2			  : 1;
							unsigned char PC4			  : 1;
							unsigned char PC8			  : 1;
							unsigned char PC16			  : 1;
							unsigned char PC32			  : 1;
							unsigned char MODE			  : 1;
							unsigned char JISL			  : 1;

							unsigned char JOGP			  : 1;
							unsigned char JOGM			  : 1;
							unsigned char RMOD			  : 1;
							unsigned char HOME			  : 1;
							unsigned char STP			  : 1;
							unsigned char CSTR			  : 1;
							unsigned char RES			  : 1;
							unsigned char SON			  : 1;
						};
					};
				} RoboCylinder;

				unsigned char Port4Bit0 				  : 1;
				unsigned char Port4Bit1 				  : 1;
				unsigned char Port4Bit2 				  : 1;
				unsigned char Port4Bit3 				  : 1;
				unsigned char Port4Bit4 				  : 1;
				unsigned char Port4Bit5 				  : 1;
				unsigned char Port4Bit6 				  : 1;
				unsigned char Port4Bit7 				  : 1;

				unsigned char Port5Bit0 				  : 1;
				unsigned char Port5Bit1 				  : 1;
				unsigned char Port5Bit2 				  : 1;
				unsigned char Port5Bit3 				  : 1;
				unsigned char Port5Bit4 				  : 1;
				unsigned char Port5Bit5 				  : 1;
				unsigned char Port5Bit6 				  : 1;
				unsigned char Port5Bit7 				  : 1;

				unsigned char Port6Bit0 				  : 1;
				unsigned char Port6Bit1 				  : 1;
				unsigned char Port6Bit2 				  : 1;
				unsigned char Port6Bit3 				  : 1;
				unsigned char Port6Bit4 				  : 1;
				unsigned char Port6Bit5 				  : 1;
				unsigned char Port6Bit6 				  : 1;
				unsigned char Port6Bit7 				  : 1;

				unsigned char Port7Bit0 				  : 1;
				unsigned char Port7Bit1 				  : 1;
				unsigned char Port7Bit2 				  : 1;
				unsigned char Port7Bit3 				  : 1;
				unsigned char Port7Bit4 				  : 1;
				unsigned char Port7Bit5 				  : 1;
				unsigned char Port7Bit6 				  : 1;
				unsigned char Port7Bit7 				  : 1;
			} CN3;
		};
	};
} DOUTTYPEA, *PDOUTTYPEA;

class cDioTypeA : public cAbstractDio<DINPTYPEA, DOUTTYPEA>
{
public:
	cDioTypeA () {}

	// NANOSPEC DI INTERFACE
	virtual BOOL IsEmergencyStop			 (void) { return m_inp->CN2.EmergencyStop; }
	virtual BOOL IsDoorInterlock			 (void) { return m_inp->CN2.DoorInterlock; }
	virtual BOOL IsEquipmentPower			 (void) { return m_inp->CN2.EquipmentPower; }
	virtual BOOL IsSafetyPlug				 (void) { return m_inp->CN2.SafetyPlug; }
	virtual BOOL IsHepaPower				 (void) { return m_inp->CN2.HepaPower; }
	virtual BOOL IsAirPressureLevelLow		 (void) { return m_inp->CN2.AirPressureLow; }
	virtual BOOL IsMovo2Alarm				 (void) { return m_inp->CN2.Movo2Alarm; }
	virtual BOOL IsMaintenanceSwitch		 (void) { return m_inp->CN2.MaintenanceSwitch; }
	virtual BOOL IsEngineerMaintenanceSwitch (void) { return m_inp->CN2.EngineerMaintenanceSwitch; }
	virtual BOOL IsStressScanStartPosX		 (void) { return m_inp->CN2.StressScanStartPosX; }
	virtual BOOL IsStressScanEndPosX		 (void) { return m_inp->CN2.StressScanEndPosX; }
	virtual BOOL IsShutterOpen				 (void) { return m_inp->CN2.ShutterOpen && m_inp->CN2.ShutterBottom1; }
	virtual BOOL IsShutterClose 			 (void) { return m_inp->CN2.ShutterTop1; }
	virtual BOOL IsRobotArmDetect			 (void) { return m_inp->CN2.RobotArm; }
	virtual BOOL IsStressUnitAlarm			 (void) { return m_inp->CN2.StressUnitAlarm; }
	virtual BOOL IsPinUpperPos				 (void) { return m_inp->CN2.Servo[0].UpperPos && m_inp->CN2.Servo[1].UpperPos; }
	virtual BOOL IsPinAlignmentPos			 (void) { return m_inp->CN2.Servo[0].AlignmentPos && m_inp->CN2.Servo[1].AlignmentPos; }
	virtual BOOL IsPinDownPos				 (void) { return m_inp->CN2.Servo[0].DownPos && m_inp->CN2.Servo[1].DownPos; }
	virtual BOOL IsWorkGuideOpen			 (void) { return m_inp->CN2.Guide == m_clamp.open; }
	virtual BOOL IsWorkGuideClose			 (void) { return m_inp->CN2.Guide == m_clamp.close; }
#if defined( NEXIO_VER_210 )
//Saiki 20130116 Change ----->
//	virtual BOOL IsGlassExist				 (void) { return m_inp->CN2.GlassExistence1 && m_inp->CN2.GlassExistence2; }
	virtual BOOL IsGlassExist				 (void)
	{
		// Ý’è“Ç‚Ýž‚Ý
		nexiniGetParameter(&m_params);

		switch(m_params.GlassExist.ExistCount){
			case 1:
				return m_inp->CN2.GlassExistence1;
			case 2:
				return m_inp->CN2.GlassExistence1 && m_inp->CN2.GlassExistence2;
			default:
				return 0;
		}
	}
//Saiki 20130116 Change <-----
#else
	virtual BOOL IsGlassExist				 (void) { return m_inp->CN2.GlassExistence; }
#endif
	virtual BOOL IsStageLoadPos 			 (void) { return m_inp->CN2.LoadPosX && m_inp->CN2.LoadPosY; }
	virtual BOOL IsVacuumOn 				 (void) { return IsVacuumOnEx(); }
	virtual BOOL IsVacuumOff				 (void) { return IsVacuumOffEx(); }
	virtual BOOL IsRetractPosZ				 (void) { return m_inp->CN2.RefractPosZ; }

	// NANOSPEC DO INTERFACE
	virtual BOOL VacuumStatus	(void) { return m_out->CN2.VacuumPressure1 || m_out->CN2.VacuumPressure2; }
	virtual BOOL AirFloatStatus (void) { return m_out->CN2.AirFloat1	   || m_out->CN2.AirFloat2; }
	virtual BOOL HepaStatus 	(void) { return m_out->CN2.HepaPowerOFF ^ m_inp->CN2.HepaPower; }

	virtual void EquipmentStatusRun (BOOL bOn) { m_out->CN2.EquipmentStatusRun = bOn; }
	virtual void EquipmentPowerOFF	(BOOL bOn) { m_out->CN2.EquipmentPowerOFF  = bOn; }
// matsuhisa 2013.06.04 added >>>
	virtual void SubConPowerOFF		(BOOL bOn) { m_out->CN2.SubConPowerOFF     = bOn; }
// matsuhisa 2013.06.04 added <<<
	virtual void HepaPowerOFF		(BOOL bOn) { m_out->CN2.HepaPowerOFF	   = bOn; }
	virtual void StressMeasTriger	(BOOL bOn) { m_out->CN2.StressMeasTriger   = bOn; }

	// NEXTRA DI INTERFACE
	virtual BOOL IsServoOnEx		 (unsigned short axis) { return m_inp->CN2.Servo[axis].On; }
	virtual BOOL IsServoReadyEx 	 (unsigned short axis) { return m_inp->CN2.Servo[axis].Ready; }
	virtual BOOL IsServoAlarmEx 	 (unsigned short axis) { return !m_inp->CN2.Servo[axis].Alarm; }
	virtual BOOL IsPinPosCompEx 	 (unsigned short axis) { return m_inp->CN2.Servo[axis].PosComp; }
	virtual BOOL IsPinUpperPosEx	 (unsigned short axis) { return m_inp->CN2.Servo[axis].UpperPos && m_inp->CN2.Servo[axis].PosComp; }
	virtual BOOL IsPinAlignmentPosEx (unsigned short axis) { return m_inp->CN2.Servo[axis].AlignmentPos && IsPinPosCompEx(axis); }
	virtual BOOL IsPinLowerPosEx	 (unsigned short axis) { return m_inp->CN2.Servo[axis].LowerPos 	&& IsPinPosCompEx(axis); }
	virtual BOOL IsPinDownPosEx 	 (unsigned short axis) { return m_inp->CN2.Servo[axis].DownPos; }
	virtual BOOL IsWorkGuideOpenEx	 (unsigned short num)  { return (m_inp->CN2.Guide & m_clamp[num].open)	== m_clamp[num].open; }
	virtual BOOL IsWorkGuideCloseEx  (unsigned short num)  { return (m_inp->CN2.Guide & m_clamp[num].close) == m_clamp[num].close; }

	virtual BOOL IsVacuumOnEx (void)
	{
		if ( !m_out->CN2.VacuumPressure1 && !m_out->CN2.VacuumPressure2 )
			return FALSE;

		if ( m_out->CN2.VacuumPressure1 ) {
			if ( !m_inp->CN2.VacuumPressure1 )
				return FALSE;
		}
		if ( m_out->CN2.VacuumPressure2 ) {
			if ( !m_inp->CN2.VacuumPressure2 )
				return FALSE;
		}
		return TRUE;
	}
	virtual BOOL IsVacuumOffEx (void) { return !m_inp->CN2.VacuumPressure1 && !m_inp->CN2.VacuumPressure2; }

	// NEXTRA DO INTERFACE
	virtual void VacuumOnEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
			m_out->CN2.VacuumPressure1 = bOn;
			break;
		case VACUUM_SOLENOID_VALVE2:
			m_out->CN2.VacuumPressure2 = bOn;
			break;
		}
	}
	virtual void VacuumDestructEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
			m_out->CN2.VacuumDestruct1 = bOn;
			break;
		case VACUUM_SOLENOID_VALVE2:
			m_out->CN2.VacuumDestruct2 = bOn;
			break;
		}
	}
	virtual void AirFloatEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
			m_out->CN2.AirFloat1 = bOn;
			break;
		case VACUUM_SOLENOID_VALVE2:
			m_out->CN2.AirFloat2 = bOn;
			break;
		}
	}

	virtual void ShutterOpenEx		(BOOL bOn)						{ m_out->CN2.ShutterOpen = bOn; }
	virtual void ShutterCloseEx 	(BOOL bOn)						{ m_out->CN2.ShutterClose = bOn; }
	virtual void ServoOnEx			(unsigned short axis, BOOL bOn) { m_out->CN2.Servo[axis].On = bOn; }
	virtual void ServoAlarmResetEx	(unsigned short axis, BOOL bOn) { m_out->CN2.Servo[axis].AlarmReset = bOn; }
	virtual void ServoBrakeEx		(unsigned short axis, BOOL bOn) { m_out->CN2.Servo[axis].Brake = bOn; }
	virtual void WorkGuideOpenEx	(unsigned short num, BOOL bOn)	{ if ( num == 0 ) m_out->CN2.GuideRev1 = bOn; else m_out->CN2.GuideRev2 = bOn; }
	virtual void WorkGuideCloseEx	(unsigned short num, BOOL bOn)	{ if ( num == 0 ) m_out->CN2.GuideFwd1 = bOn; else m_out->CN2.GuideFwd2 = bOn; }

	/* Contact Angle */
	virtual BOOL IsCA_Interlock (void) { return m_inp->CN3.CAInterlock; }

	virtual void CA_Interlock (BOOL bOn) { m_out->CN3.CAInterlock = bOn; }

	/* Resistance Sheet(4PP) */
	virtual BOOL IsRS_ProbeHeadUpperPos (void) { return m_inp->CN3.RSProbeHeadUpperPos; }
	virtual BOOL IsRS_ProbeHeadLowerPos (void) { return m_inp->CN3.RSProbeHeadLowerPos; }


	virtual void RS_ProbeHeadUp   (BOOL bOn) { m_out->CN3.RSProbeHeadUp = bOn; }
	virtual void RS_ProbeHeadDown (BOOL bOn) { m_out->CN3.RSProbeHeadDown = bOn; }

	/* Transmittance */
	virtual BOOL IsTransmitLightSourcePower (void) { return m_inp->CN3.TransmitLightSourcePower; }
	virtual BOOL IsTransmitLight1Condition	(void) { return m_inp->CN3.TransmitLight1Condition; }
	virtual BOOL IsTransmitLight2Condition	(void) { return m_inp->CN3.TransmitLight2Condition; }
	virtual BOOL IsTransmitLight3Condition	(void) { return m_inp->CN3.TransmitLight3Condition; }

	virtual void TransmitLightSource1Shutter (BOOL bOn) { m_out->CN3.TransmitLightSource1Shutter = bOn; }
	virtual void TransmitLightSource2Shutter (BOOL bOn) { m_out->CN3.TransmitLightSource2Shutter = bOn; }
	virtual void TransmitLightSource3Shutter (BOOL bOn) { m_out->CN3.TransmitLightSource3Shutter = bOn; }

	/* Micro Scope */
	virtual BOOL IsMS_ModeSwitch (void) { return m_inp->CN3.MicroScopeModeSwitch; }
	virtual BOOL IsMS_UpSwitch	 (void) { return m_inp->CN3.MicroScopeUpSwitch; }
	virtual BOOL IsMS_DownSwitch (void) { return m_inp->CN3.MicroScopeDownSwitch; }
	virtual BOOL IsMS_UpperPos	 (void) { return m_inp->CN3.MicroScopeUpperPos; }
	virtual BOOL IsMS_LowerPos1  (void) { return m_inp->CN3.MicroScopeLowerPos1; }
	virtual BOOL IsMS_LowerPos2  (void) { return m_inp->CN3.MicroScopeLowerPos2; }
	virtual BOOL IsMS_LensKind	 (void) { return m_inp->CN3.MicroScopeLensKind; }

	virtual void MS_ModeLamp (BOOL bOn) { m_out->CN3.MicroScapeModeLamp = bOn; }

	/* Robo Cylinder */
	virtual unsigned char RC_GetCurrentMode (void)
	{
		unsigned char retval = RCOPMODE_UNKNOWN;

		if ( IsRC_MODES() ) {
			retval = m_out->CN3.RoboCylinder.JISL ? RCOPMODE_INCHING : RCOPMODE_JOG;
		}
		else {
			retval = RCOPMODE_NORMAL;
		}
		return retval;
	}
	virtual BOOL RC_GetJISL (void) { return m_out->CN3.RoboCylinder.JISL; }
	virtual BOOL RC_GetJOGP (void) { return m_out->CN3.RoboCylinder.JOGP; }
	virtual BOOL RC_GetJOGM (void) { return m_out->CN3.RoboCylinder.JOGM; }

	virtual BOOL RC_PM (unsigned char* val) { return *val = m_inp->CN3.RoboCylinder.buffer.byte & 0xF; }
	virtual BOOL IsRC_PM1	(void) { return m_inp->CN3.RoboCylinder.PM1; }
	virtual BOOL IsRC_PM2	(void) { return m_inp->CN3.RoboCylinder.PM2; }
	virtual BOOL IsRC_PM4	(void) { return m_inp->CN3.RoboCylinder.PM4; }
	virtual BOOL IsRC_PM8	(void) { return m_inp->CN3.RoboCylinder.PM8; }
	virtual BOOL IsRC_PM16	(void) { return m_inp->CN3.RoboCylinder.PM16; }
	virtual BOOL IsRC_PM32	(void) { return m_inp->CN3.RoboCylinder.PM32; }
	virtual BOOL IsRC_MOVE	(void) { return m_inp->CN3.RoboCylinder.MOVE; }
	virtual BOOL IsRC_MODES (void) { return m_inp->CN3.RoboCylinder.MODES; }
	virtual BOOL IsRC_PZONE (void) { return m_inp->CN3.RoboCylinder.PZONE; }
	virtual BOOL IsRC_RMDS	(void) { return m_inp->CN3.RoboCylinder.RMDS; }
	virtual BOOL IsRC_HEND	(void) { return m_inp->CN3.RoboCylinder.HEND; }
	virtual BOOL IsRC_PEND	(void) { return m_inp->CN3.RoboCylinder.PEND; }
	virtual BOOL IsRC_WEND	(void) { return m_inp->CN3.RoboCylinder.PEND; }
	virtual BOOL IsRC_SV	(void) { return m_inp->CN3.RoboCylinder.SV; }
	virtual BOOL IsRC_EMGS	(void) { return m_inp->CN3.RoboCylinder.EMGS; }
	virtual BOOL IsRC_ALM	(void) { return m_inp->CN3.RoboCylinder.ALM; }

	virtual void RC_PC (unsigned char val) { m_out->CN3.RoboCylinder.buffer.byte = ((m_out->CN3.RoboCylinder.buffer.byte & 0xC0) | val); }
	virtual void RC_PC1  (BOOL bOn) { m_out->CN3.RoboCylinder.PC1  = bOn; }
	virtual void RC_PC2  (BOOL bOn) { m_out->CN3.RoboCylinder.PC2  = bOn; }
	virtual void RC_PC4  (BOOL bOn) { m_out->CN3.RoboCylinder.PC4  = bOn; }
	virtual void RC_PC8  (BOOL bOn) { m_out->CN3.RoboCylinder.PC8  = bOn; }
	virtual void RC_PC16 (BOOL bOn) { m_out->CN3.RoboCylinder.PC16 = bOn; }
	virtual void RC_PC32 (BOOL bOn) { m_out->CN3.RoboCylinder.PC32 = bOn; }
	virtual void RC_MODE (BOOL bOn) { m_out->CN3.RoboCylinder.MODE = bOn; }
	virtual void RC_JISL (BOOL bOn) { m_out->CN3.RoboCylinder.JISL = bOn; }
	virtual void RC_JOGP (BOOL bOn) { m_out->CN3.RoboCylinder.JOGP = bOn; }
	virtual void RC_JOGM (BOOL bOn) { m_out->CN3.RoboCylinder.JOGM = bOn; }
	virtual void RC_RMOD (BOOL bOn) { m_out->CN3.RoboCylinder.RMOD = bOn; }
	virtual void RC_HOME (BOOL bOn) { m_out->CN3.RoboCylinder.HOME = bOn; }
	virtual void RC_STP  (BOOL bOn) { m_out->CN3.RoboCylinder.STP  = bOn; }
	virtual void RC_CSTR (BOOL bOn) { m_out->CN3.RoboCylinder.CSTR = bOn; }
	virtual void RC_RES  (BOOL bOn) { m_out->CN3.RoboCylinder.RES  = bOn; }
	virtual void RC_SON  (BOOL bOn) { m_out->CN3.RoboCylinder.SON  = bOn; }

private:
	NEXINIPARAMS	params;
};

#endif // _NEXIOTYPEA_H_
