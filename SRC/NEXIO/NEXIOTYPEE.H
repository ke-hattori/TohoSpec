#if !defined( _NEXIOTYPEE_H_ )
#define _NEXIOTYPEE_H_

#pragma once

using namespace nano;

class cDioTypeE : public cDioInterface
{
public:
	// NEXTRA DI INTERFACE
	virtual BOOL IsEmergencyStop			   (void) { return TRUE; }
	virtual BOOL IsDoorInterlock			   (void) { return TRUE; }
	virtual BOOL IsEquipmentPower			   (void) { return TRUE; }
	virtual BOOL IsSafetyPlug				   (void) { return TRUE; }
	virtual BOOL IsHepaPower				   (void) { return FALSE; }
	virtual BOOL IsAirPressureLevelLow		   (void) { return TRUE; }
	virtual BOOL IsMovo2Alarm				   (void) { return FALSE; }
	virtual BOOL IsMaintenanceSwitch		   (void) { return TRUE; }
	virtual BOOL IsEngineerMaintenanceSwitch   (void) { return FALSE; }
	virtual BOOL IsStressScanStartPosX		   (void) { return TRUE; }
	virtual BOOL IsStressScanEndPosX		   (void) { return TRUE; }
	virtual BOOL IsShutterOpen				   (void) { return FALSE; }
	virtual BOOL IsShutterClose 			   (void) { return TRUE; }
	virtual BOOL IsRobotArmDetect			   (void) { return TRUE; }
	virtual BOOL IsStressUnitAlarm			   (void) { return TRUE; }
	virtual BOOL IsVacuumOn 				   (void) { return TRUE; }
	virtual BOOL IsVacuumOff				   (void) { return FALSE; }
	virtual BOOL IsGlassExist				   (void) { return TRUE; }
	virtual BOOL IsPinUpperPos				   (void) { return FALSE; }
	virtual BOOL IsPinAlignmentPos			   (void) { return FALSE; }
	virtual BOOL IsPinDownPos				   (void) { return TRUE; }
	virtual BOOL IsWorkGuideOpen			   (void) { return TRUE; }
	virtual BOOL IsWorkGuideClose			   (void) { return FALSE; }
	virtual BOOL IsStageLoadPos 			   (void) { return TRUE; }
	virtual BOOL IsVacuumOnEx				   (void) { return FALSE; }
	virtual BOOL IsVacuumOffEx				   (void) { return FALSE; }
	virtual BOOL IsRetractPosZ				   (void) { return TRUE; }

	virtual BOOL IsServoOnEx		 (unsigned short axis) { return FALSE; }
	virtual BOOL IsServoReadyEx 	 (unsigned short axis) { return FALSE; }
	virtual BOOL IsServoAlarmEx 	 (unsigned short axis) { return FALSE; }
	virtual BOOL IsPinPosCompEx 	 (unsigned short axis) { return FALSE; }
	virtual BOOL IsPinUpperPosEx	 (unsigned short axis) { return FALSE; }
	virtual BOOL IsPinAlignmentPosEx (unsigned short axis) { return FALSE; }
	virtual BOOL IsPinLowerPosEx	 (unsigned short axis) { return FALSE; }
	virtual BOOL IsPinDownPosEx 	 (unsigned short axis) { return FALSE; }
	virtual BOOL IsWorkGuideOpenEx	 (unsigned short num)  { return FALSE; }
	virtual BOOL IsWorkGuideCloseEx  (unsigned short num)  { return FALSE; }

	virtual BOOL IsStageAlarm				(void) 				 { return FALSE; }
	virtual BOOL IsStageDriverX1AxisAlarm	(unsigned short num) { return FALSE; }
	virtual BOOL IsStageDriverX2AxisAlarm	(unsigned short num) { return FALSE; }
	virtual BOOL IsStageDriverYAxisAlarm	(unsigned short num) { return FALSE; }

	// NEXTRA DO INTERFACE
	virtual BOOL VacuumStatus		(void) { return FALSE; }
	virtual BOOL AirFloatStatus 	(void) { return FALSE; }
	virtual BOOL HepaStatus 		(void) { return TRUE; }

	virtual void EquipmentStatusRun (BOOL bOn) {}
	virtual void EquipmentPowerOFF	(BOOL bOn) {}
	virtual void SubConPowerOFF		(BOOL bOn) {}
	virtual void HepaPowerOFF		(BOOL bOn) {}
	virtual void StressMeasTriger	(BOOL bOn) {}

	virtual void ShutterOpenEx		 (BOOL bOn) {}
	virtual void ShutterCloseEx 	 (BOOL bOn) {}
	virtual void VacuumOnEx 		 (unsigned short valve, BOOL bOn) {}
	virtual void VacuumDestructEx	 (unsigned short valve, BOOL bOn) {}
	virtual void AirFloatEx 		 (unsigned short valve, BOOL bOn) {}
	virtual void ServoOnEx			 (unsigned short axis, BOOL bOn) {}
	virtual void ServoBrakeEx		 (unsigned short axis, BOOL bOn) {}

	virtual void ServoAlarmResetEx	(unsigned short axis, BOOL bOn) {}
	virtual void PinUpEx			(unsigned short num, BOOL bOn)	{}
	virtual void PinAlignmentEx 	(unsigned short num, BOOL bOn)	{}
	virtual void PinDownEx			(unsigned short num, BOOL bOn)	{}
	virtual void WorkGuideOpenEx	(unsigned short num, BOOL bOn)	{}
	virtual void WorkGuideCloseEx	(unsigned short num, BOOL bOn)	{}


	/* Contact Angle */
	virtual BOOL IsCA_Interlock (void) { return TRUE; }

	virtual void CA_Interlock (BOOL bOn) {}

	/* Resistance Sheet(4PP) */
	virtual BOOL IsRS_ProbeHeadUpperPos (void) { return TRUE; }
	virtual BOOL IsRS_ProbeHeadLowerPos (void) { return FALSE; }


	virtual void RS_ProbeHeadUp   (BOOL bOn) {}
	virtual void RS_ProbeHeadDown (BOOL bOn) {}

	/* Transmittance */
	virtual BOOL IsTransmitLightSourcePower (void) { return TRUE; }
	virtual BOOL IsTransmitLight1Condition (void) { return TRUE; }
	virtual BOOL IsTransmitLight2Condition (void) { return TRUE; }
	virtual BOOL IsTransmitLight3Condition (void) { return TRUE; }

	virtual void TransmitLightSource1Shutter (BOOL bOn) {}
	virtual void TransmitLightSource2Shutter (BOOL bOn) {}
	virtual void TransmitLightSource3Shutter (BOOL bOn) {}

	/* Micro Scope */
	virtual BOOL IsMS_ModeSwitch (void) { return FALSE; }
	virtual BOOL IsMS_UpSwitch	 (void) { return FALSE; }
	virtual BOOL IsMS_DownSwitch (void) { return FALSE; }
	virtual BOOL IsMS_UpperPos	 (void) { return FALSE; }
	virtual BOOL IsMS_LowerPos1  (void) { return FALSE; }
	virtual BOOL IsMS_LowerPos2  (void) { return FALSE; }
	virtual BOOL IsMS_LensKind	 (void) { return FALSE; }

	virtual void MS_ModeLamp (BOOL bOn) {}

	/* Robo Cylinder */
	virtual unsigned char RC_GetCurrentMode (void) { return RCOPMODE_NORMAL; }
	virtual BOOL RC_GetJISL (void) { return FALSE; }
	virtual BOOL RC_GetJOGP (void) { return FALSE; }
	virtual BOOL RC_GetJOGM (void) { return FALSE; }
	virtual unsigned char RC_PM (void) { return 0; }
	virtual BOOL IsRC_PM1	(void) { return FALSE; }
	virtual BOOL IsRC_PM2	(void) { return FALSE; }
	virtual BOOL IsRC_PM4	(void) { return FALSE; }
	virtual BOOL IsRC_PM8	(void) { return FALSE; }
	virtual BOOL IsRC_PM16	(void) { return FALSE; }
	virtual BOOL IsRC_PM32	(void) { return FALSE; }
	virtual BOOL IsRC_MOVE	(void) { return FALSE; }
	virtual BOOL IsRC_MODES (void) { return FALSE; }
	virtual BOOL IsRC_PZONE (void) { return FALSE; }
	virtual BOOL IsRC_RMDS	(void) { return FALSE; }
	virtual BOOL IsRC_HEND	(void) { return FALSE; }
	virtual BOOL IsRC_PEND	(void) { return FALSE; }
	virtual BOOL IsRC_WEND	(void) { return FALSE; }
	virtual BOOL IsRC_SV	(void) { return FALSE; }
	virtual BOOL IsRC_EMGS	(void) { return FALSE; }
	virtual BOOL IsRC_ALM	(void) { return FALSE; }

	virtual void RC_PC (unsigned char val) {}
	virtual void RC_PC1  (BOOL bOn) {}
	virtual void RC_PC2  (BOOL bOn) {}
	virtual void RC_PC4  (BOOL bOn) {}
	virtual void RC_PC8  (BOOL bOn) {}
	virtual void RC_PC16 (BOOL bOn) {}
	virtual void RC_PC32 (BOOL bOn) {}
	virtual void RC_MODE (BOOL bOn) {}
	virtual void RC_JISL (BOOL bOn) {}
	virtual void RC_JOGP (BOOL bOn) {}
	virtual void RC_JOGM (BOOL bOn) {}
	virtual void RC_RMOD (BOOL bOn) {}
	virtual void RC_HOME (BOOL bOn) {}
	virtual void RC_STP  (BOOL bOn) {}
	virtual void RC_CSTR (BOOL bOn) {}
	virtual void RC_PWRT (BOOL bOn) {}
	virtual void RC_RES  (BOOL bOn) {}
	virtual void RC_SON  (BOOL bOn) {}
};

#endif // _NEXIOTYPEE_H_
