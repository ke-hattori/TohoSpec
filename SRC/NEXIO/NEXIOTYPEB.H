#if !defined( _NEXIOTYPEB_H_ )
#define _NEXIOTYPEB_H_

#pragma once

#include <nexmisc.hxx>
#include <nexif.hxx>

#ifdef _DEBUG
#pragma comment(lib, "dtnsnexif.lib")
#else
#pragma comment(lib, "tnsnexif.lib")
#endif

using namespace nano;

typedef struct tagSTAGEDRIVER_YAXIS {
	unsigned char Port6Bit0			  : 1;
	unsigned char Port6Bit1			  : 1;
	unsigned char Port6Bit2			  : 1;
	unsigned char Port6Bit3			  : 1;
	unsigned char Port6Bit4			  : 1;
	unsigned char Port6Bit5			  : 1;
	unsigned char Port6Bit6			  : 1;
	unsigned char Port6Bit7			  : 1;

	unsigned char Port7Bit0			  : 1;
	unsigned char Port7Bit1			  : 1;
	unsigned char Port7Bit2			  : 1;
	unsigned char Port7Bit3			  : 1;
	unsigned char Port7Bit4			  : 1;
	unsigned char StageDriverYAxisAlarm0	  : 1;
	unsigned char StageDriverYAxisAlarm1	  : 1;
	unsigned char StageDriverYAxisAlarm2	  : 1;
} STAGEDRIVER_YAXIS;


typedef struct tagDINPTYPEB {
	union {
		basic_device buffer[EQPIOINP_BYTES];
		struct {
			unsigned char EmergencyStop 			  : 1;
			unsigned char DoorInterlock 			  : 1;
			unsigned char EquipmentPower			  : 1;
			unsigned char Port0Bit3 				  : 1;
			unsigned char Port0Bit4 				  : 1;
			unsigned char Port0Bit5 				  : 1;
			unsigned char AirPressureLow			  : 1;
// 2013.01.10 bagus stage driver alarm io -->
//			unsigned char Port0Bit7 				  : 1;
			unsigned char StageAlarm 				  : 1;
// 2013.01.10 bagus stage driver alarm io <--

			unsigned char Port1Bit0 				  : 1;
			unsigned char MaintenanceSwitch 		  : 1;
			unsigned char EngineerMaintenanceSwitch   : 1;
			unsigned char Port1Bit3 				  : 1;
//Saiki 20130306 Modified ----->
//			unsigned char Port1Bit4 				  : 1;
//			unsigned char Port1Bit5 				  : 1;
//			unsigned char Port1Bit6 				  : 1;
//			unsigned char Port1Bit7 				  : 1;
			unsigned char TransmitLightSourcePower	  : 1;
			unsigned char TransmitLight1Condition 	  : 1;
			unsigned char TransmitLight2Condition 	  : 1;
			unsigned char TransmitLight3Condition 	  : 1;
//Saiki 20130306 Modified <-----

// matsuhisa 2011.11.11 modified >>>
//			unsigned char Port2Bit0 				  : 1;
//			unsigned char Port2Bit1 				  : 1;
//			unsigned char Port2Bit2 				  : 1;
			unsigned char ShutterOpen				  : 1;
			unsigned char ShutterCloseCylinder		  : 1;
			unsigned char ShutterOpenCylinder		  : 1;
// matsuhisa 2011.11.11 modified <<<
			unsigned char Port2Bit3 				  : 1;
			unsigned char Port2Bit4 				  : 1;
			unsigned char RobotArm					  : 1;
//Saiki 20130306 Modified ----->
//			unsigned char Port2Bit6 				  : 1;
//			unsigned char Port2Bit7 				  : 1;
			unsigned char RSProbeHeadUpperPos		  : 1;
			unsigned char RSProbeHeadLowerPos		  : 1;
//Saiki 20130306 Modified <-----

//Saiki 20130306 Modified ----->
//			unsigned char VacuumPressure			  : 1;
//			unsigned char Port3Bit1 				  : 1;
			unsigned char VacuumPressure1			  : 1;
			unsigned char VacuumPressure2			  : 1;
//Saiki 20130306 Modified <-----
			unsigned char Port3Bit2 				  : 1;
			unsigned char GlassExistence1			  : 1;
			unsigned char GlassExistence2			  : 1;
			unsigned char LoadPosX					  : 1;
			unsigned char LoadPosY					  : 1;
//Saiki 20130306 Modified ----->
//			unsigned char Port3Bit7 				  : 1;
			unsigned char RefractPosZ 				  : 1;
//Saiki 20130306 Modified <-----

			unsigned char LifterCylinder1UpperPos	  : 1;
			unsigned char LifterCylinder1LowerPos	  : 1;
			unsigned char LifterCylinder2UpperPos	  : 1;
			unsigned char LifterCylinder2LowerPos	  : 1;
// matsuhisa 2011.09.13 modified >>>
//			unsigned char Port4Bit4 				  : 1;
//			unsigned char Port4Bit5 				  : 1;
//			unsigned char Port4Bit6 				  : 1;
//			unsigned char Port4Bit7 				  : 1;
			unsigned char LifterCylinder3UpperPos	  : 1;
			unsigned char LifterCylinder3LowerPos	  : 1;
			unsigned char LifterCylinder4UpperPos	  : 1;
			unsigned char LifterCylinder4LowerPos	  : 1;
// matsuhisa 2011.09.13 modified <<<

// 2013.01.10 bagus stage driver alarm io -->
// 			unsigned char Port5Bit0 				  : 1;
// 			unsigned char Port5Bit1 				  : 1;
// 			unsigned char Port5Bit2 				  : 1;
// 			unsigned char Port5Bit3 				  : 1;
// 			unsigned char Port5Bit4 				  : 1;
// 			unsigned char Port5Bit5 				  : 1;
			unsigned char StageDriverX1AxisAlarm0	  : 1;
			unsigned char StageDriverX1AxisAlarm1	  : 1;
			unsigned char StageDriverX1AxisAlarm2	  : 1;
			unsigned char StageDriverX2AxisAlarm0 	  : 1;
			unsigned char StageDriverX2AxisAlarm1	  : 1;
			unsigned char StageDriverX2AxisAlarm2	  : 1;
// 2013.01.10 bagus stage driver alarm io <--
			unsigned char Port5Bit6 				  : 1;
			unsigned char Port5Bit7 				  : 1;

// 2013.01.10 bagus stage driver alarm io -->
//			unsigned short Guide;
			union {
				unsigned short Guide;
				STAGEDRIVER_YAXIS YAxis;
			};
// 2013.01.10 bagus stage driver alarm io <--
		};
	};
} DINPTYPEB, *PDINPTYPEB;

typedef struct tagDOUTTYPEB {
	union {
		basic_device buffer[EQPIOOUT_BYTES];
		struct {
			unsigned char EquipmentStatusRun	: 1;
			unsigned char Port0Bit1 			: 1;
// matsuhisa 2013.06.04 modified >>>
//			unsigned char Port0Bit2 			: 1;
			unsigned char SubConPowerOFF 		: 1;
// matsuhisa 2013.06.04 modified <<<
			unsigned char Port0Bit3 			: 1;
			unsigned char Port0Bit4 			: 1;
			unsigned char Port0Bit5 			: 1;
			unsigned char Port0Bit6 			: 1;
			unsigned char Port0Bit7 			: 1;

			unsigned char Port1Bit0 			: 1;
			unsigned char Port1Bit1 			: 1;
			unsigned char Port1Bit2 			: 1;
			unsigned char Port1Bit3 			: 1;
			unsigned char Port1Bit4 			: 1;
//Saiki 20130306 Modified ----->
//			unsigned char Port1Bit5 			: 1;
//			unsigned char Port1Bit6 			: 1;
//			unsigned char Port1Bit7 			: 1;
			unsigned char TransmitLightSource1Shutter : 1;
			unsigned char TransmitLightSource2Shutter : 1;
			unsigned char TransmitLightSource3Shutter : 1;
//Saiki 20130306 Modified <-----

// matsuhisa 2011.11.11 modified >>>
//			unsigned char Port2Bit0 			: 1;
//			unsigned char Port2Bit1 			: 1;
			unsigned char ShutterOpen 			: 1;
			unsigned char ShutterClose 			: 1;
// matsuhisa 2011.11.11 modified <<<
			unsigned char Port2Bit2 			: 1;
			unsigned char Port2Bit3 			: 1;
			unsigned char Port2Bit4 			: 1;
			unsigned char Port2Bit5 			: 1;
//Saiki 20130306 Modified ----->
//			unsigned char Port2Bit6 			: 1;
//			unsigned char Port2Bit7 			: 1;
			unsigned char RSProbeHeadUp			: 1;
			unsigned char RSProbeHeadDown		: 1;
//Saiki 20130306 Modified <-----

//Saiki 20130306 Modified ----->
//			unsigned char VacuumPressure		: 1;
//			unsigned char Port3Bit1 			: 1;
//			unsigned char VacuumDestruct		: 1;
//			unsigned char Port3Bit3 			: 1;
			unsigned char VacuumPressure1		: 1;
			unsigned char VacuumPressure2		: 1;
			unsigned char VacuumDestruct1		: 1;
			unsigned char VacuumDestruct2		: 1;
//Saiki 20130306 Modified <-----
			unsigned char Port3Bit4 			: 1;
			unsigned char Port3Bit5 			: 1;
			unsigned char Port3Bit6 			: 1;
			unsigned char Port3Bit7 			: 1;

			unsigned char LifterCylinder1Up 	: 1;
			unsigned char LifterCylinder1Down	: 1;
			unsigned char LifterCylinder2Up 	: 1;
			unsigned char LifterCylinder2Down	: 1;
			unsigned char Port4Bit4 			: 1;
			unsigned char Port4Bit5 			: 1;
			unsigned char Port4Bit6 			: 1;
			unsigned char Port4Bit7 			: 1;

			unsigned char Port5Bit0 			: 1;
			unsigned char Port5Bit1 			: 1;
			unsigned char Port5Bit2 			: 1;
			unsigned char Port5Bit3 			: 1;
			unsigned char Port5Bit4 			: 1;
			unsigned char Port5Bit5 			: 1;
			unsigned char Port5Bit6 			: 1;
			unsigned char Port5Bit7 			: 1;

			unsigned char GuideFwd1 			: 1;
			unsigned char GuideRev1 			: 1;
			unsigned char GuideFwd2 			: 1;
			unsigned char GuideRev2 			: 1;
			unsigned char Port6Bit4 			: 1;
			unsigned char Port6Bit5 			: 1;
			unsigned char Port6Bit6 			: 1;
			unsigned char Port6Bit7 			: 1;

			unsigned char Port7Bit0 			: 1;
			unsigned char Port7Bit1 			: 1;
			unsigned char Port7Bit2 			: 1;
			unsigned char Port7Bit3 			: 1;
			unsigned char Port7Bit4 			: 1;
			unsigned char Port7Bit5 			: 1;
			unsigned char Port7Bit6 			: 1;
			unsigned char Port7Bit7 			: 1;
		};
	};
} DOUTTYPEB, *PDOUTTYPEB;

class cDioTypeB : public cAbstractDio<DINPTYPEB, DOUTTYPEB>
{
public:
	cDioTypeB () {}

	// NANOSPEC DI INTERFACE
	virtual BOOL IsEmergencyStop			 (void) { return m_inp->EmergencyStop; }
	virtual BOOL IsDoorInterlock			 (void) { return m_inp->DoorInterlock; }
	virtual BOOL IsEquipmentPower			 (void) { return m_inp->EquipmentPower; }
	virtual BOOL IsAirPressureLevelLow		 (void) { return m_inp->AirPressureLow; }
	virtual BOOL IsMaintenanceSwitch		 (void) { return m_inp->MaintenanceSwitch; }
	virtual BOOL IsEngineerMaintenanceSwitch (void) { return m_inp->EngineerMaintenanceSwitch; }
	virtual BOOL IsShutterClose 			 (void) { return !m_params.Options.Shutter; }
	virtual BOOL IsRobotArmDetect			 (void) { return m_inp->RobotArm; }
	virtual BOOL IsPinUpperPos				 (void) { return m_inp->LifterCylinder1UpperPos && m_inp->LifterCylinder2UpperPos; }
	virtual BOOL IsPinAlignmentPos			 (void) { return m_inp->LifterCylinder1UpperPos && m_inp->LifterCylinder2LowerPos; }
	virtual BOOL IsPinDownPos				 (void) { return m_inp->LifterCylinder1LowerPos && m_inp->LifterCylinder2LowerPos; }

// 2013.01.10 bagus stage driver alarm io -->

// 	virtual BOOL IsWorkGuideOpen			 (void) { return m_inp->Guide == m_clamp.open; }
// 	virtual BOOL IsWorkGuideClose			 (void) { return m_inp->Guide == m_clamp.close; }
	virtual BOOL IsWorkGuideOpen (void)
	{
		unsigned short Guide;

		Guide = m_inp->Guide;

		// mask stage driver Y axis alarm
		STAGEDRIVER_YAXIS* pYAxis = reinterpret_cast<STAGEDRIVER_YAXIS*>(&Guide);
		pYAxis->StageDriverYAxisAlarm0 = 0;
		pYAxis->StageDriverYAxisAlarm1 = 0;
		pYAxis->StageDriverYAxisAlarm2 = 0;

		return Guide == m_clamp.open;
	}

	virtual BOOL IsWorkGuideClose (void)
	{
		unsigned short Guide;

		Guide = m_inp->Guide;

		// mask stage driver Y axis alarm
		STAGEDRIVER_YAXIS* pYAxis = reinterpret_cast<STAGEDRIVER_YAXIS*>(&Guide);
		pYAxis->StageDriverYAxisAlarm0 = 0;
		pYAxis->StageDriverYAxisAlarm1 = 0;
		pYAxis->StageDriverYAxisAlarm2 = 0;

		return Guide == m_clamp.close;
	}
// 2013.01.10 bagus stage driver alarm io <--

//Saiki 20130116 Change ----->
//	virtual BOOL IsGlassExist				 (void) { return m_inp->GlassExistence1 && m_inp->GlassExistence2; }
	virtual BOOL IsGlassExist				 (void)
	{
		// Ý’è“Ç‚Ýž‚Ý
		nexiniGetParameter(&m_params);

		switch(m_params.GlassExist.ExistCount){
			case 1:
				return m_inp->GlassExistence1;
			case 2:
				return m_inp->GlassExistence1 && m_inp->GlassExistence2;
			default:
				return 0;
		}
	}
//Saiki 20130116 Change <-----
	virtual BOOL IsStageLoadPos 			 (void) { return m_inp->LoadPosX && m_inp->LoadPosY; }
	virtual BOOL IsVacuumOn 				 (void) { return IsVacuumOnEx(); }
	virtual BOOL IsVacuumOff				 (void) { return IsVacuumOffEx(); }
	virtual BOOL IsRetractZPos				 (void) { return m_inp->RefractPosZ; }	/*hdebdeb*/

// 2013.01.10 bagus stage driver alarm io -->
	virtual BOOL IsStageAlarm				 (void) { return m_inp->StageAlarm; }
	virtual BOOL IsStageDriverX1AxisAlarm (unsigned short num)
	{
		switch ( num ) {
		case 0:
			return m_inp->StageDriverX1AxisAlarm0;
			break;
		case 1:
			return m_inp->StageDriverX1AxisAlarm1;
			break;
		case 2:
			return m_inp->StageDriverX1AxisAlarm2;
			break;
		}

		return FALSE;
	}

	virtual BOOL IsStageDriverX2AxisAlarm (unsigned short num)
	{
		switch ( num ) {
		case 0:
			return m_inp->StageDriverX2AxisAlarm0;
			break;
		case 1:
			return m_inp->StageDriverX2AxisAlarm1;
			break;
		case 2:
			return m_inp->StageDriverX2AxisAlarm2;
			break;
		}

		return FALSE;
	}

	virtual BOOL IsStageDriverYAxisAlarm (unsigned short num)
	{
		switch ( num ) {
		case 0:
			return m_inp->YAxis.StageDriverYAxisAlarm0;
			break;
		case 1:
			return m_inp->YAxis.StageDriverYAxisAlarm1;
			break;
		case 2:
			return m_inp->YAxis.StageDriverYAxisAlarm2;
			break;
		}

		return FALSE;
	}
// 2013.01.10 bagus stage driver alarm io <--

	// NANOSPEC DO INTERFACE
//Saiki 20130306 Modified ----->
//	virtual BOOL VacuumStatus (void) { return m_out->VacuumPressure; }
	virtual BOOL VacuumStatus (void)
	{
		// Ý’è“Ç‚Ýž‚Ý
		nexiniGetParameter(&m_params);

		switch(m_params.VacuumPressure.VacCount){
			case 1:
				return m_out->VacuumPressure1;
			case 2:
				return m_out->VacuumPressure1 || m_out->VacuumPressure2;
			default:
				return FALSE;
		}
	}

//Saiki 20130306 Modified <-----
// matsuhisa 2013.06.04 added >>>
	virtual void SubConPowerOFF		(BOOL bOn) { m_out->SubConPowerOFF = bOn; }
// matsuhisa 2013.06.04 added <<<

	virtual void EquipmentStatusRun (BOOL bOn) { m_out->EquipmentStatusRun = bOn; }

	// NEXTRA DI INTERFACE
	virtual BOOL IsWorkGuideOpenEx	(unsigned short num) { return (m_inp->Guide & m_clamp[num].open)  == m_clamp[num].open; }
	virtual BOOL IsWorkGuideCloseEx (unsigned short num) { return (m_inp->Guide & m_clamp[num].close) == m_clamp[num].close; }

	virtual BOOL IsVacuumOnEx (void)
	{
//Saiki 20130306 Modified ----->
//		if ( !m_out->VacuumPressure )
//			return FALSE;
//
//		if ( m_out->VacuumPressure ) {
//			if ( !m_inp->VacuumPressure )
//				return FALSE;
//		}
//		return TRUE;

		// Ý’è“Ç‚Ýž‚Ý
		nexiniGetParameter(&m_params);

		if(m_params.VacuumPressure.VacCount == 1){
			if ( !m_out->VacuumPressure1 )
				return FALSE;

			if ( m_out->VacuumPressure1 ) {
				if ( !m_inp->VacuumPressure1 )
					return FALSE;
			}
			return TRUE;
		}
		else if(m_params.VacuumPressure.VacCount == 2){
			if ( !m_out->VacuumPressure1 && !m_out->VacuumPressure2 )
				return FALSE;
			if ( m_out->VacuumPressure1 ) {
				if ( !m_inp->VacuumPressure1 )
					return FALSE;
			}
			if ( m_out->VacuumPressure2 ) {
				if ( !m_inp->VacuumPressure2 )
					return FALSE;
			}
			return TRUE;
		}
		else if(m_params.VacuumPressure.VacCount == 0){
			return TRUE;
		}
		else{
			return FALSE;
		}
//Saiki 20130306 Modified <-----
	}
//Saiki 20130306 Modified ----->
//	virtual BOOL IsVacuumOffEx (void) { return !m_inp->VacuumPressure; }
	virtual BOOL IsVacuumOffEx (void)
	{
		// Ý’è“Ç‚Ýž‚Ý
		nexiniGetParameter(&m_params);

		switch(m_params.VacuumPressure.VacCount){
			case 0:
				return TRUE;
			case 1:
				return !m_inp->VacuumPressure1;
			case 2:
				return !m_inp->VacuumPressure1 && !m_inp->VacuumPressure2;
			default:
				return FALSE;
		}
	}
//Saiki 20130306 Modified <-----

	// NEXTRA DO INTERFACE
	virtual void VacuumOnEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
//Saiki 20130306 Modified ----->
//			m_out->VacuumPressure = bOn;
			switch(m_params.VacuumPressure.VacCount){
				case 0:
					break;
				case 1:
					m_out->VacuumPressure1 = bOn;
					break;
				case 2:
					m_out->VacuumPressure1 = m_out->VacuumPressure2 = bOn;
				default:
					break;
			}
//Saiki 20130306 Modified <-----
			break;
		case VACUUM_SOLENOID_VALVE2:
			break;
		}
	}
	virtual void VacuumDestructEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
//Saiki 20130306 Modified ----->
//			m_out->VacuumDestruct = bOn;
			m_out->VacuumDestruct1 = bOn;
			switch(m_params.VacuumPressure.VacCount){
				case 0:
					break;
				case 1:
					m_out->VacuumDestruct1 = bOn;
					break;
				case 2:
					m_out->VacuumDestruct1 = m_out->VacuumDestruct2 = bOn;
				default:
					break;
			}
//Saiki 20130306 Modified <-----
			break;
		case VACUUM_SOLENOID_VALVE2:
			break;
		}
	}

	virtual void PinUpEx		  (unsigned short num, BOOL bOn) { m_out->LifterCylinder1Up   = m_out->LifterCylinder2Up   = bOn; }
	virtual void PinAlignmentEx   (unsigned short num, BOOL bOn) { m_out->LifterCylinder1Up   = m_out->LifterCylinder2Down = bOn; }
	virtual void PinDownEx		  (unsigned short num, BOOL bOn) { m_out->LifterCylinder1Down = m_out->LifterCylinder2Down = bOn; }
	virtual void WorkGuideOpenEx  (unsigned short num, BOOL bOn) { if ( num == 0 ) m_out->GuideRev1 = bOn; else m_out->GuideRev2 = bOn; }
	virtual void WorkGuideCloseEx (unsigned short num, BOOL bOn) { if ( num == 0 ) m_out->GuideFwd1 = bOn; else m_out->GuideFwd2 = bOn; }

//Saiki 20130306 Modified ----->
	/* Transmittance */
	virtual BOOL IsTransmitLightSourcePower (void) { return m_inp->TransmitLightSourcePower; }
	virtual BOOL IsTransmitLight1Condition	(void) { return m_inp->TransmitLight1Condition; }
	virtual BOOL IsTransmitLight2Condition	(void) { return m_inp->TransmitLight2Condition; }
	virtual BOOL IsTransmitLight3Condition	(void) { return m_inp->TransmitLight3Condition; }

	virtual void TransmitLightSource1Shutter (BOOL bOn) { m_out->TransmitLightSource1Shutter = bOn; }
	virtual void TransmitLightSource2Shutter (BOOL bOn) { m_out->TransmitLightSource2Shutter = bOn; }
	virtual void TransmitLightSource3Shutter (BOOL bOn) { m_out->TransmitLightSource3Shutter = bOn; }

	/* Resistance Sheet(4PP) */
	virtual BOOL IsRS_ProbeHeadUpperPos (void) { return m_inp->RSProbeHeadUpperPos; }
	virtual BOOL IsRS_ProbeHeadLowerPos (void) { return m_inp->RSProbeHeadLowerPos; }

	virtual void RS_ProbeHeadUp   (BOOL bOn) { m_out->RSProbeHeadUp = bOn; }
	virtual void RS_ProbeHeadDown (BOOL bOn) { m_out->RSProbeHeadDown = bOn; }

	virtual BOOL IsRetractPosZ			(void) { return m_inp->RefractPosZ; }
//Saiki 20130306 Modified <-----
private:
	NEXINIPARAMS	params;
};

#endif // _NEXIOTYPEB_H_
