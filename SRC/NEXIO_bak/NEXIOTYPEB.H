#if !defined( _NEXIOTYPEB_H_ )
#define _NEXIOTYPEB_H_

#pragma once

using namespace nano;

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
			unsigned char Port0Bit7 				  : 1;

			unsigned char Port1Bit0 				  : 1;
			unsigned char MaintenanceSwitch 		  : 1;
			unsigned char EngineerMaintenanceSwitch   : 1;
			unsigned char Port1Bit3 				  : 1;
			unsigned char Port1Bit4 				  : 1;
			unsigned char Port1Bit5 				  : 1;
			unsigned char Port1Bit6 				  : 1;
			unsigned char Port1Bit7 				  : 1;

			unsigned char Port2Bit0 				  : 1;
			unsigned char Port2Bit1 				  : 1;
			unsigned char Port2Bit2 				  : 1;
			unsigned char Port2Bit3 				  : 1;
			unsigned char Port2Bit4 				  : 1;
			unsigned char RobotArm					  : 1;
			unsigned char Port2Bit6 				  : 1;
			unsigned char Port2Bit7 				  : 1;

			unsigned char VacuumPressure			  : 1;
			unsigned char Port3Bit1 				  : 1;
			unsigned char Port3Bit2 				  : 1;
			unsigned char GlassExistence1			  : 1;
			unsigned char GlassExistence2			  : 1;
			unsigned char LoadPosX					  : 1;
			unsigned char LoadPosY					  : 1;
			unsigned char Port3Bit7 				  : 1;

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

			unsigned char Port5Bit0 				  : 1;
			unsigned char Port5Bit1 				  : 1;
			unsigned char Port5Bit2 				  : 1;
			unsigned char Port5Bit3 				  : 1;
			unsigned char Port5Bit4 				  : 1;
			unsigned char Port5Bit5 				  : 1;
			unsigned char Port5Bit6 				  : 1;
			unsigned char Port5Bit7 				  : 1;

			unsigned short Guide;
		};
	};
} DINPTYPEB, *PDINPTYPEB;

typedef struct tagDOUTTYPEB {
	union {
		basic_device buffer[EQPIOOUT_BYTES];
		struct {
			unsigned char EquipmentStatusRun	: 1;
			unsigned char Port0Bit1 			: 1;
			unsigned char Port0Bit2 			: 1;
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
			unsigned char Port1Bit5 			: 1;
			unsigned char Port1Bit6 			: 1;
			unsigned char Port1Bit7 			: 1;

			unsigned char Port2Bit0 			: 1;
			unsigned char Port2Bit1 			: 1;
			unsigned char Port2Bit2 			: 1;
			unsigned char Port2Bit3 			: 1;
			unsigned char Port2Bit4 			: 1;
			unsigned char Port2Bit5 			: 1;
			unsigned char Port2Bit6 			: 1;
			unsigned char Port2Bit7 			: 1;

			unsigned char VacuumPressure		: 1;
			unsigned char Port3Bit1 			: 1;
			unsigned char VacuumDestruct		: 1;
			unsigned char Port3Bit3 			: 1;
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
	virtual BOOL IsWorkGuideOpen			 (void) { return m_inp->Guide == m_clamp.open; }
	virtual BOOL IsWorkGuideClose			 (void) { return m_inp->Guide == m_clamp.close; }
	virtual BOOL IsGlassExist				 (void) { return m_inp->GlassExistence1 && m_inp->GlassExistence2; }
	virtual BOOL IsStageLoadPos 			 (void) { return m_inp->LoadPosX && m_inp->LoadPosY; }
	virtual BOOL IsVacuumOn 				 (void) { return IsVacuumOnEx(); }
	virtual BOOL IsVacuumOff				 (void) { return IsVacuumOffEx(); }
	virtual BOOL IsRetractZPos				 (void) { return m_inp->Port3Bit7; }	/*hdebdeb*/

	// NANOSPEC DO INTERFACE
	virtual BOOL VacuumStatus (void) { return m_out->VacuumPressure; }

	virtual void EquipmentStatusRun (BOOL bOn) { m_out->EquipmentStatusRun = bOn; }

	// NEXTRA DI INTERFACE
	virtual BOOL IsWorkGuideOpenEx	(unsigned short num) { return (m_inp->Guide & m_clamp[num].open)  == m_clamp[num].open; }
	virtual BOOL IsWorkGuideCloseEx (unsigned short num) { return (m_inp->Guide & m_clamp[num].close) == m_clamp[num].close; }

	virtual BOOL IsVacuumOnEx (void)
	{
		if ( !m_out->VacuumPressure )
			return FALSE;

		if ( m_out->VacuumPressure ) {
			if ( !m_inp->VacuumPressure )
				return FALSE;
		}
		return TRUE;
	}
	virtual BOOL IsVacuumOffEx (void) { return !m_inp->VacuumPressure; }

	// NEXTRA DO INTERFACE
	virtual void VacuumOnEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
			m_out->VacuumPressure = bOn;
			break;
		case VACUUM_SOLENOID_VALVE2:
			break;
		}
	}
	virtual void VacuumDestructEx (unsigned short valve, BOOL bOn)
	{
		switch ( valve ) {
		case VACUUM_SOLENOID_VALVE1:
			m_out->VacuumDestruct = bOn;
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
};

#endif // _NEXIOTYPEB_H_
