#if !defined( _NEXIO_BASE_HXX_ )
#define _NEXIO_BASE_HXX_

#pragma once

#include <nexmisc.hxx>
#include <nexini.hxx>
#include <nexif.hxx>
#include <apidio.h>

#pragma comment(lib, "apidio.lib")
#ifdef _DEBUG
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsnexini.lib")
#endif


#ifdef __NEXTRA_INTERNAL

// #########################################################################
// DEFINITIONS
// #########################################################################
#define EQPIOINP_BYTES		   (24)
#define EQPIOOUT_BYTES		   (24)
#define EQPIOINP_KERNELSTR	   TEXT("EQPIOINP_KERNELSTR")
#define EQPIOOUT_KERNELSTR	   TEXT("EQPIOOUT_KERNELSTR")

#define NEXIO_MAX_SLOTS_NUM    (3)

#define NEXIO_VER_210		   (210)
//#define NEXIO_VER_200 		 (200)
//#define NEXIO_VER_100 		 (100)

const BYTE	ON	= 1;		   /* signal on */
const BYTE	OFF = 0;		   /* signal off */

#if !defined( _DEVPRTF )
#define _DEVPRTF(arg) (arg ? TCHAR('1') : TCHAR('.'))
#endif

typedef BOOL (WINAPI* NEXIOINPPOCTYPEPTR)(void);
typedef void (WINAPI* NEXIOOUTPOCTYPEPTR)(BOOL);

#define DEFINE_NEXIOPROCTYPE(ret, name, arg) \
	typedef ret (WINAPI* fn##name) arg; \
	static fn##name pfn##name = NULL;

#define BEGIN_NEXIOPROCMAP()

#define NEXIOPROCMAP_ENTRY(module, name) \
	pfn##name = (fn##name) GetProcAddress(module, #name); \
	if ( pfn##name == NULL ) { \
		 _RPTF1(_CRT_WARN, "GetProcAddress is failed - %s", #name); \
		 FreeLibrary(module); \
		 return FALSE; \
	}

#define END_NEXIOPROCMAP()

#define GUIDEOP_MK			   0xAAAA  // mask for work guide open bits:
#define GUIDECL_MK			   0x5555  // mask for work guide open bits:
#define GUIDEOPEX_MK		   0x00AA  // mask for work guide openex bits:
#define GUIDECLEX_MK		   0x0055  // mask for work guide closeex bits:

enum
{
	VACUUM_SOLENOID_VALVE1 = 0,
	VACUUM_SOLENOID_VALVE2,
	VACUUM_SOLENOID_VALVE_MAX
};

namespace nano
{

using namespace std;

class basic_device
{
public:
	unsigned char* operator& () { return &byte; }
	operator unsigned char*  () { return &byte; }

	friend ostream& operator << (ostream& os, basic_device& rhs)
	{
		os << _DEVPRTF(rhs.bit0)
		   << _DEVPRTF(rhs.bit1)
		   << _DEVPRTF(rhs.bit2)
		   << _DEVPRTF(rhs.bit3)
		   << _DEVPRTF(rhs.bit4)
		   << _DEVPRTF(rhs.bit5)
		   << _DEVPRTF(rhs.bit6)
		   << _DEVPRTF(rhs.bit7);

		return os;
	}

	union {
		unsigned char byte;
		struct {
			unsigned char bit0 : 1;
			unsigned char bit1 : 1;
			unsigned char bit2 : 1;
			unsigned char bit3 : 1;
			unsigned char bit4 : 1;
			unsigned char bit5 : 1;
			unsigned char bit6 : 1;
			unsigned char bit7 : 1;
		};
	};
};

class cDioInterface
{
public:
	// NEXTRA DI INTERFACE
	virtual BOOL IsEmergencyStop			   (void) { return FALSE; }
	virtual BOOL IsDoorInterlock			   (void) { return FALSE; }
	virtual BOOL IsEquipmentPower			   (void) { return FALSE; }
	virtual BOOL IsSafetyPlug				   (void) { return FALSE; }
	virtual BOOL IsHepaPower				   (void) { return FALSE; }
	virtual BOOL IsAirPressureLevelLow		   (void) { return FALSE; }
	virtual BOOL IsMovo2Alarm				   (void) { return FALSE; }
	virtual BOOL IsMaintenanceSwitch		   (void) { return FALSE; }
	virtual BOOL IsEngineerMaintenanceSwitch   (void) { return FALSE; }
	virtual BOOL IsStressScanStartPosX		   (void) { return FALSE; }
	virtual BOOL IsStressScanEndPosX		   (void) { return FALSE; }
	virtual BOOL IsShutterOpen				   (void) { return FALSE; }
	virtual BOOL IsShutterClose 			   (void) { return FALSE; }
	virtual BOOL IsRobotArmDetect			   (void) { return FALSE; }
	virtual BOOL IsStressUnitAlarm			   (void) { return FALSE; }
	virtual BOOL IsVacuumOn 				   (void) { return FALSE; }
	virtual BOOL IsVacuumOff				   (void) { return FALSE; }
	virtual BOOL IsGlassExist				   (void) { return FALSE; }
	virtual BOOL IsPinUpperPos				   (void) { return FALSE; }
	virtual BOOL IsPinAlignmentPos			   (void) { return FALSE; }
	virtual BOOL IsPinDownPos				   (void) { return FALSE; }
	virtual BOOL IsWorkGuideOpen			   (void) { return FALSE; }
	virtual BOOL IsWorkGuideClose			   (void) { return FALSE; }
	virtual BOOL IsStageLoadPos 			   (void) { return FALSE; }
	virtual BOOL IsVacuumOnEx				   (void) { return FALSE; }
	virtual BOOL IsVacuumOffEx				   (void) { return FALSE; }
	virtual BOOL IsRetractPosZ				   (void) { return FALSE; }

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

// 2013.01.10 bagus stage driver alarm io -->
	virtual BOOL IsStageAlarm				(void) 				 { return FALSE; }
	virtual BOOL IsStageDriverX1AxisAlarm	(unsigned short num) { return FALSE; }
	virtual BOOL IsStageDriverX2AxisAlarm	(unsigned short num) { return FALSE; }
	virtual BOOL IsStageDriverYAxisAlarm	(unsigned short num) { return FALSE; }
// 2013.01.10 bagus stage driver alarm io <--

	// NEXTRA DO INTERFACE
	virtual BOOL VacuumStatus		(void) { return FALSE; }
	virtual BOOL AirFloatStatus 	(void) { return FALSE; }
	virtual BOOL HepaStatus 		(void) { return FALSE; }

	virtual void EquipmentStatusRun (BOOL bOn) {}
	virtual void EquipmentPowerOFF	(BOOL bOn) {}
// matsuhisa 2013.06.04 added >>>
	virtual void SubConPowerOFF		(BOOL bOn) {}
// matsuhisa 2013.06.04 added <<<
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
	virtual BOOL IsCA_Interlock (void) { return FALSE; }

	virtual void CA_Interlock (BOOL bOn) {}

	/* Resistance Sheet(4PP) */
	virtual BOOL IsRS_ProbeHeadUpperPos (void) { return FALSE; }
	virtual BOOL IsRS_ProbeHeadLowerPos (void) { return FALSE; }


	virtual void RS_ProbeHeadUp   (BOOL bOn) {}
	virtual void RS_ProbeHeadDown (BOOL bOn) {}

	/* Transmittance */
	virtual BOOL IsTransmitLightSourcePower (void) { return FALSE; }
	virtual BOOL IsTransmitLight1Condition (void) { return FALSE; }
	virtual BOOL IsTransmitLight2Condition (void) { return FALSE; }
	virtual BOOL IsTransmitLight3Condition (void) { return FALSE; }

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
	virtual unsigned char RC_GetCurrentMode (void) { return RCOPMODE_UNKNOWN; }
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

	cDioInterface* GetDioInterface (void) { return this; }
};

template <class T1, class T2>
class cAbstractDio : public cDioInterface
{
public:
	cAbstractDio () : m_inp(EQPIOINP_KERNELSTR), m_out(EQPIOOUT_KERNELSTR) { nexiniGetParameter(&m_params); }

	shared_memory<T1> m_inp;
	shared_memory<T2> m_out;

	struct clamp_data_t {
		ULONGLONG open;
		ULONGLONG close;
	};
	typedef std::vector<clamp_data_t> clamp_list;

protected:
	class cClamp : public clamp_list
	{
	public:
		cClamp (void) { Initialize(); }
		void Initialize (void)
		{
			open = close = 0L;
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexiobase.hxx ---------- { ---------- */
//			  std::ifstream ifs(CLAMP_CFGFILE_NAME);
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexiobase.hxx ---------- 			 */
			TCHAR l_tszProcessFPath[_MAX_PATH]; /* 呼出しプロセスのフルパス*/
			::GetModuleFileName(0, l_tszProcessFPath, sizeof(l_tszProcessFPath));
			TCHAR l_tszDrive[_MAX_DRIVE];
			TCHAR l_tszDir[_MAX_DIR];
			_tsplitpath(l_tszProcessFPath, l_tszDrive, l_tszDir, 0, 0);
			TCHAR l_tszProcDir[_MAX_PATH];
			_stprintf(l_tszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
			if (0 != _tcslen(l_tszProcDir)) {
				if (_T('\\') != l_tszProcDir[_tcslen(l_tszProcDir) - 1]) {
					_tcscat(l_tszProcDir, _T("\\"));
				}
			}
			TCHAR l_tszClampCfgFPath[_MAX_PATH];
			_stprintf(l_tszClampCfgFPath, _T("%s") _T(CLAMP_CFGFILE_NAME), l_tszProcDir);
			std::ifstream ifs(l_tszClampCfgFPath);
/* modified 2009.07.07 hmenjo dll 相対パス対応 nexiobase.hxx ---------- } ---------- */
			if ( !ifs.is_open() ) {
				struct clamp_data_t data = { GUIDEOPEX_MK, GUIDECLEX_MK };
				push_back(data);
				data.open <<= 8; data.close <<= 8;
				push_back(data);
				open = GUIDEOP_MK; close = GUIDECL_MK;
			}
			std::string line;
			while ( std::getline(ifs, line) ) {
				std::istringstream iss(line);
				std::string token;
				unsigned long block;
				if ( std::getline(iss, token, COMMACHAR) )
					block = atol(token.c_str());
				if ( size() < block ) resize(block);
				if ( std::getline(iss, token, COMMACHAR) )
					at(block - 1).open = hatol(token.c_str());
				if ( std::getline(iss, token, COMMACHAR) )
					at(block - 1).close = hatol(token.c_str());
#if 0
				open  |= (at(block - 1).open  << (8 * (block - 1)));
				close |= (at(block - 1).close << (8 * (block - 1)));
#else
				open  |= at(block - 1).open;
				close |= at(block - 1).close;
#endif
			}
		}
		ULONGLONG open, close;
	private:
		long hatol (const char* s)
		{
			long retval = 0;
			BOOL first	= FALSE;

			struct hex_dec_t {
				char ch;
				long value;
			};
			static const hex_dec_t table [16] =
			{
				{ '0',	0 },	{ '1',	1 },	{ '2',	2 },	{ '3',	3 },
				{ '4',	4 },	{ '5',	5 },	{ '6',	6 },	{ '7',	7 },
				{ '8',	8 },	{ '9',	9 },	{ 'A', 10 },	{ 'B', 11 },
				{ 'C', 12 },	{ 'D', 13 },	{ 'E', 14 },	{ 'F', 15 }
			};

			char* str = _strupr(_strdup(s));
			char* pch = str;

			if ( *pch == '0' && *(pch + 1) == 'X' )
				pch += 2;

			while ( *pch != NULLCHAR ) {
				BOOL found = FALSE;
				for ( int i = 0; i < countof(table); i++ ) {
					if ( *pch == table[i].ch ) {
						if ( !first )
							retval <<= 4;
						retval |= table[i].value;
						found = TRUE;
						break;
					}
				}
				if ( !found ) break;
				pch++;
				first = FALSE;
			}
			free(str);
			return retval;
		}
	} m_clamp;
	NEXINIPARAMS  m_params;

};

}; // namespace nano

struct	device_status_t {
	unsigned char raw;
	unsigned char up;
	unsigned char down;
	unsigned char old;
};

class device_status : protected device_status_t
{
public:
	device_status () { basic_interface::zeromemory(&raw); }
	device_status& operator = (const unsigned char value) { return detect(value); }

	inline bool is_on	(void) const { return raw	  == ON; }
	inline bool is_off	(void) const { return raw	  != ON; }
	inline bool is_up	(void) const { return up	  == ON; }
	inline bool is_down (void) const { return down	  == ON; }

	inline device_status& detect (void)
	{
		up	 = (raw | old) ^ old;
		down = (raw | old) ^ raw;
		old  = raw;
		return *this;
	}
	inline device_status& detect (unsigned char value)
	{
		raw = value;
		return detect();
	}
};

class device_list : public std::vector<device_status>
{
public:
	device_list () {}
	device_list (size_type n) : std::vector<device_status>(n) {}
	inline void for_each (void) { for ( iterator iter(begin()); iter != end(); iter++ ) iter->detect(); }
};

#endif // __NEXTRA_INTERNAL

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined( _NEXIODLL_ )
	#define NEXIOAPI __declspec(dllimport)
#else
	#define NEXIOAPI __declspec(dllexport)
#endif

// ====================================================================
// EXPORTED FUNCTION PROTOTYPES
// ====================================================================
NEXIOAPI BOOL __stdcall nexioInitialize(HWND owner);
NEXIOAPI void __stdcall nexioFinalize(void);

NEXIOAPI BOOL __stdcall nexioMonitor(int X, int Y);

NEXIOAPI BOOL __stdcall nexioIsEmergencyStop(void);
NEXIOAPI BOOL __stdcall nexioIsDoorInterlock(void);
NEXIOAPI BOOL __stdcall nexioIsEquipmentPower(void);
NEXIOAPI BOOL __stdcall nexioIsSafetyPlug(void);
NEXIOAPI BOOL __stdcall nexioIsHepaPower(void);
NEXIOAPI BOOL __stdcall nexioIsAirPressureLevelLow(void);
NEXIOAPI BOOL __stdcall nexioIsMovo2Alarm(void);
NEXIOAPI BOOL __stdcall nexioIsMaintenanceSwitch(void);
NEXIOAPI BOOL __stdcall nexioIsEngineerMaintenanceSwitch(void);
NEXIOAPI BOOL __stdcall nexioIsStressScanStartPosX(void);
NEXIOAPI BOOL __stdcall nexioIsStressScanEndPosX(void);
NEXIOAPI BOOL __stdcall nexioIsShutterOpen(void);
NEXIOAPI BOOL __stdcall nexioIsShutterClose(void);
NEXIOAPI BOOL __stdcall nexioIsRobotArmDetect(void);
NEXIOAPI BOOL __stdcall nexioIsStressUnitAlarm(void);
NEXIOAPI BOOL __stdcall nexioIsVacuumOn(void);
NEXIOAPI BOOL __stdcall nexioIsVacuumOff(void);
NEXIOAPI BOOL __stdcall nexioIsGlassExist(void);
NEXIOAPI BOOL __stdcall nexioIsStageLoadPos(void);
NEXIOAPI BOOL __stdcall nexioIsPinUpperPos(void);
NEXIOAPI BOOL __stdcall nexioIsPinAlignmentPos(void);
NEXIOAPI BOOL __stdcall nexioIsPinDownPos(void);
NEXIOAPI BOOL __stdcall nexioIsWorkGuideOpen(void);
NEXIOAPI BOOL __stdcall nexioIsWorkGuideClose(void);
NEXIOAPI BOOL __stdcall nexioIsRetractPosZ(void);

NEXIOAPI BOOL __stdcall nexioIsVacuumStatus(void);
NEXIOAPI BOOL __stdcall nexioIsAirFloatStatus(void);
NEXIOAPI BOOL __stdcall nexioIsHepaStatus(void);

NEXIOAPI void __stdcall nexioEquipmentStatusRun(BOOL bOn);
NEXIOAPI void __stdcall nexioEquipmentPowerOFF(BOOL bOn);
// matsuhisa 2013.06.04 added >>>
NEXIOAPI void __stdcall nexioSubConPowerOFF(BOOL bOn);
// matsuhisa 2013.06.04 added <<<
NEXIOAPI void __stdcall nexioHepaPowerOFF(BOOL bOn);
NEXIOAPI void __stdcall nexioStressMeasTriger(BOOL bOn);

NEXIOAPI void __stdcall nexioStressMeasTriger(BOOL bOn);

#ifdef __NEXTRA_INTERNAL
NEXIOAPI BOOL __stdcall nexioIsPinUpperPosEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsServoOnEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsServoReadyEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsServoAlarmEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsPinPosCompEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsPinAlignmentPosEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsPinLowerPosEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsPinDownPosEx(unsigned short axis);
NEXIOAPI BOOL __stdcall nexioIsWorkGuideOpenEx(unsigned short num);
NEXIOAPI BOOL __stdcall nexioIsWorkGuideCloseEx(unsigned short num);
NEXIOAPI BOOL __stdcall nexioIsVacuumOnEx(void);
NEXIOAPI BOOL __stdcall nexioIsVacuumOffEx(void);

// 2013.01.10 bagus stage driver alarm io -->
NEXIOAPI BOOL __stdcall nexioIsStageAlarm(void);
NEXIOAPI BOOL __stdcall nexioIsStageDriverX1AxisAlarm(unsigned short num);
NEXIOAPI BOOL __stdcall nexioIsStageDriverX2AxisAlarm(unsigned short num);
NEXIOAPI BOOL __stdcall nexioIsStageDriverYAxisAlarm(unsigned short num);
// 2013.01.10 bagus stage driver alarm io <--

NEXIOAPI void __stdcall nexioShutterOpenEx(BOOL bOn);
NEXIOAPI void __stdcall nexioShutterCloseEx(BOOL bOn);
NEXIOAPI void __stdcall nexioVacuumOnEx(unsigned short valve, BOOL bOn);
NEXIOAPI void __stdcall nexioVacuumDestructEx(unsigned short valve, BOOL bOn);
NEXIOAPI void __stdcall nexioAirFloatEx(unsigned short valve, BOOL bOn);
NEXIOAPI void __stdcall nexioServoOnEx(unsigned short axis, BOOL bOn);
NEXIOAPI void __stdcall nexioServoAlarmResetEx(unsigned short axis, BOOL bOn);
NEXIOAPI void __stdcall nexioServoBrakeEx(unsigned short axis, BOOL bOn);
NEXIOAPI void __stdcall nexioPinUpEx(unsigned short num, BOOL bOn);
NEXIOAPI void __stdcall nexioPinAlignmentEx(unsigned short num, BOOL bOn);
NEXIOAPI void __stdcall nexioPinDownEx(unsigned short num, BOOL bOn);
NEXIOAPI void __stdcall nexioWorkGuideOpenEx(unsigned short num, BOOL bOn);
NEXIOAPI void __stdcall nexioWorkGuideCloseEx(unsigned short num, BOOL bOn);

/**********************************************************************
 * Contact Angle                                                      *
**********************************************************************/
NEXIOAPI BOOL __stdcall nexioIsCA_Interlock(void);

NEXIOAPI void __stdcall nexioCA_Interlock(BOOL bOn);

/**********************************************************************
 * Resistance Sheet(4PP)                                              *
**********************************************************************/
NEXIOAPI BOOL __stdcall nexioIsRS_ProbeHeadUpperPos(void);
NEXIOAPI BOOL __stdcall nexioIsRS_ProbeHeadLowerPos(void);

NEXIOAPI void __stdcall nexioRS_ProbeHeadUp(BOOL bOn);
NEXIOAPI void __stdcall nexioRS_ProbeHeadDown(BOOL bOn);

/**********************************************************************
 * Transmittance                                                      *
**********************************************************************/
NEXIOAPI BOOL __stdcall nexioIsTransmitLightSourcePower(void);
NEXIOAPI BOOL __stdcall nexioIsTransmitLight1Condition(void);
NEXIOAPI BOOL __stdcall nexioIsTransmitLight2Condition(void);
NEXIOAPI BOOL __stdcall nexioIsTransmitLight3Condition(void);

NEXIOAPI void __stdcall nexioTransmitLightSource1Shutter(BOOL bOn);
NEXIOAPI void __stdcall nexioTransmitLightSource2Shutter(BOOL bOn);
NEXIOAPI void __stdcall nexioTransmitLightSource3Shutter(BOOL bOn);

/**********************************************************************
 * Micro Scope                                                        *
**********************************************************************/
NEXIOAPI BOOL __stdcall nexioIsMS_ModeSwitch(void);
NEXIOAPI BOOL __stdcall nexioIsMS_UpSwitch(void);
NEXIOAPI BOOL __stdcall nexioIsMS_DownSwitch(void);
NEXIOAPI BOOL __stdcall nexioIsMS_UpperPos(void);
NEXIOAPI BOOL __stdcall nexioIsMS_LowerPos1(void);
NEXIOAPI BOOL __stdcall nexioIsMS_LowerPos2(void);
// 2009.11.09 bagus MS 修正 --{--
//NEXIOAPI BOOL __stdcall nexioIsMS_LensKend(void);
NEXIOAPI BOOL __stdcall nexioIsMS_LensKind(void);
// 2009.11.09 bagus MS 修正 --}--

NEXIOAPI void __stdcall nexioMS_ModeLamp(BOOL bOn);

/**********************************************************************
 * Robo Cylinder                                                      *
**********************************************************************/
NEXIOAPI unsigned char __stdcall nexioRC_GetCurrentMode(void);
NEXIOAPI BOOL __stdcall nexioRC_GetJISL(void);
NEXIOAPI BOOL __stdcall nexioRC_GetJOGP(void);
NEXIOAPI BOOL __stdcall nexioRC_GetJOGM(void);
NEXIOAPI unsigned char __stdcall nexioRC_PM(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PM1(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PM2(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PM4(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PM8(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PM16(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PM32(void);
NEXIOAPI BOOL __stdcall nexioIsRC_MOVE(void);
NEXIOAPI BOOL __stdcall nexioIsRC_MODES(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PZONE(void);
NEXIOAPI BOOL __stdcall nexioIsRC_RMDS(void);
NEXIOAPI BOOL __stdcall nexioIsRC_HEND(void);
NEXIOAPI BOOL __stdcall nexioIsRC_PEND(void);
NEXIOAPI BOOL __stdcall nexioIsRC_WEND(void);
NEXIOAPI BOOL __stdcall nexioIsRC_SV(void);
NEXIOAPI BOOL __stdcall nexioIsRC_EMGS(void);
NEXIOAPI BOOL __stdcall nexioIsRC_ALM(void);

NEXIOAPI void __stdcall nexioRC_PC(unsigned char val);
NEXIOAPI void __stdcall nexioRC_PC1(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_PC2(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_PC4(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_PC8(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_PC16(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_PC32(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_MODE(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_JISL(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_JOGP(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_JOGM(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_RMOD(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_HOME(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_STP(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_CSTR(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_PWRT(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_RES(BOOL bOn);
NEXIOAPI void __stdcall nexioRC_SON(BOOL bOn);

#endif // __NEXTRA_INTERNAL

#ifdef __cplusplus
}
#endif

#endif // _NEXIO_BASE_HXX_
