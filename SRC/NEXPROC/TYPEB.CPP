#pragma warning (disable:4786)
#include <set>
#include <algorithm>
#include <active.hxx>
#define __NEXTRA_INTERNAL
#include <nexmisc.hxx>
#include <nexini.hxx>
#include <nexiobase.hxx>
#include <nexrc.hxx>
#include "coord.hpp"
#include "typeb.hpp"

#ifdef _DEBUG
#pragma comment(lib, "dtnsactive.lib")
#pragma comment(lib, "dtnsnexif.lib")
#pragma comment(lib, "dtnsnexio.lib")
#pragma comment(lib, "dtnsnexui.lib")
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsactive.lib")
#pragma comment(lib, "tnsnexif.lib")
#pragma comment(lib, "tnsnexio.lib")
#pragma comment(lib, "tnsnexui.lib")
#pragma comment(lib, "tnsnexini.lib")
#endif

using namespace std;
using namespace nano;

// ---------------------------------------------------------------------------
// State Definitions
// ---------------------------------------------------------------------------
enum TYPEA_STATES
{
	// idle state
	ST_IDLE,					// no requests pending

	// initialize the nextra
	ST_INIT,					// initialize nextra
	ST_INIT_MOVE_LOWER,
	ST_INIT_UNCLAMP1,			// unclamp
	ST_INIT_UNCLAMP2,			// unclamp
	ST_INIT_DONE,

	// origin return
	ST_ORT_MOVE_LOWER,
	ST_ORT_UNCLAMP1,
	ST_ORT_UNCLAMP2,

	// load glass
	ST_LOAD_MOVE_ALIGN,
	ST_LOAD_MOVE_LOWER,
	ST_LOAD_CLAMP1,
	ST_LOAD_CLAMP2,
	ST_LOAD_UNCLAMP1,
	ST_LOAD_UNCLAMP2,
	ST_LOAD_PREP_UNCLAMP,
	ST_LOAD_VAC_ON,
	ST_LOAD_WAIT_VAC_ON,

	// unload glass
	ST_UNLOAD_MOVE_UPPER,
	ST_UNLOAD_MOVE_ALIGN,
	ST_UNLOAD_CLAMP1,
	ST_UNLOAD_CLAMP2,
	ST_UNLOAD_UNCLAMP1,
	ST_UNLOAD_UNCLAMP2,
	ST_UNLOAD_PREP_UNCLAMP,
	ST_UNLOAD_VAC_OFF,
	ST_UNLOAD_WAIT_VAC_OFF,

	// motion control
	ST_MOVE_UPPER,
	ST_MOVE_ALIGN,
	ST_MOVE_LOWER,

	// clamp control
	ST_CLAMP1,
	ST_CLAMP2,
	ST_UNCLAMP1,
	ST_UNCLAMP2,
	ST_PREP_UNCLAMP,

	// vacuum control
	ST_VAC_ON,
	ST_VAC_OFF,
	ST_WAIT_VAC_ON,
	ST_WAIT_VAC_OFF,

	ST_ERROR,					// tell cliend, requset ERROR
	ST_CANCEL,

	// termination of current request
	ST_DONE,					// tell client, request DONE
	ST_ABORT,					// tell client, request ABORTED
};

// ---------------------------------------------------------------------------
// Event Definitions
// ---------------------------------------------------------------------------
enum TYPEA_EVENTS
{
	// client requests
	EV_DO_INIT, 		// initialization
	EV_DO_ORT,			// origin return
	EV_DO_LOAD, 		// glass load command
	EV_DO_UNLOAD,		// glass to be unloaded
	EV_DO_MOVE_UPPER,
	EV_DO_MOVE_ALIGN,
	EV_DO_MOVE_LOWER,
	EV_DO_CLAMP,
	EV_DO_UNCLAMP,

	// Pin completion
	EV_PIN_DONE,		// Pin has completed last request
	EV_PIN_ABORTED, 	// Pin servo failed

	// events from clamp
	EV_CLAMP_OK,		// request action done
	EV_CLAMP_ABORTED,	// request failed

	EV_VAC_OK,
	EV_VAC_ABORTED,

	// internal requests
	EV_NEXT,			// get next wafer
	EV_SKIP,			// skip
	EV_DONE,			// fall-thru state
	EV_ERROR,			// error
	EV_CANCEL,
	EV_ABORT,			// failure, user selected abort
};

// ---------------------------------------------------------------------------
// State-Event-Table Definitions
// ---------------------------------------------------------------------------
const Transition TypeATransTable[] =
{
	//	OldState				EventId 			NewState

	// initialize the servo
	{	ST_IDLE,				EV_DO_INIT, 		ST_INIT 				},
	{	ST_INIT,				EV_NEXT,			ST_INIT_UNCLAMP2		},
	{	ST_INIT,				EV_ERROR,			ST_ERROR				},
	{	ST_INIT_UNCLAMP2,		EV_CLAMP_OK,		ST_INIT_UNCLAMP1		},
	{	ST_INIT_UNCLAMP2,		EV_SKIP,			ST_MOVE_LOWER			},
	{	ST_INIT_UNCLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_INIT_UNCLAMP1,		EV_CLAMP_OK,		ST_INIT_MOVE_LOWER		},
	{	ST_INIT_UNCLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_INIT_MOVE_LOWER, 	EV_PIN_DONE,		ST_INIT_DONE			},
	{	ST_INIT_MOVE_LOWER, 	EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_INIT_MOVE_LOWER, 	EV_ERROR,			ST_ERROR				},
	{	ST_INIT_DONE,			EV_NEXT,			ST_DONE 				},

	// origin return
	{	ST_IDLE,				EV_DO_ORT,			ST_ORT_UNCLAMP2 		},
	{	ST_ORT_UNCLAMP2,		EV_CLAMP_OK,		ST_ORT_UNCLAMP1 		},
	{	ST_ORT_UNCLAMP2,		EV_SKIP,			ST_ORT_MOVE_LOWER		},
	{	ST_ORT_UNCLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_ORT_UNCLAMP1,		EV_CLAMP_OK,		ST_ORT_MOVE_LOWER		},
	{	ST_ORT_UNCLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_ORT_MOVE_LOWER,		EV_PIN_DONE,		ST_DONE 				},
	{	ST_ORT_MOVE_LOWER,		EV_SKIP,			ST_DONE 				},
	{	ST_ORT_MOVE_LOWER,		EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_ORT_MOVE_LOWER,		EV_ERROR,			ST_ERROR				},
	{	ST_ORT_MOVE_LOWER,		EV_CANCEL,			ST_CANCEL				},

	// load glass
	{	ST_IDLE,				EV_DO_LOAD, 		ST_LOAD_MOVE_ALIGN		},
	{	ST_LOAD_MOVE_ALIGN, 	EV_PIN_DONE,		ST_LOAD_CLAMP1			},
	{	ST_LOAD_MOVE_ALIGN, 	EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_LOAD_MOVE_ALIGN, 	EV_ERROR,			ST_ERROR				},
	{	ST_LOAD_MOVE_ALIGN, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_CLAMP1, 		EV_CLAMP_OK,		ST_LOAD_CLAMP2			},
	{	ST_LOAD_CLAMP1, 		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_LOAD_CLAMP2, 		EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_CLAMP2, 		EV_CLAMP_OK,		ST_LOAD_PREP_UNCLAMP	},
	{	ST_LOAD_PREP_UNCLAMP,	EV_NEXT,			ST_LOAD_UNCLAMP2		},
	{	ST_LOAD_PREP_UNCLAMP,	EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_UNCLAMP2,		EV_CLAMP_OK,		ST_LOAD_UNCLAMP1		},
	{	ST_LOAD_UNCLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_LOAD_UNCLAMP2,		EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_UNCLAMP1,		EV_CLAMP_OK,		ST_LOAD_VAC_ON			},
	{	ST_LOAD_UNCLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_LOAD_UNCLAMP1,		EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_VAC_ON, 		EV_NEXT,			ST_LOAD_MOVE_LOWER		},
	{	ST_LOAD_VAC_ON, 		EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_MOVE_LOWER, 	EV_PIN_DONE,		ST_LOAD_WAIT_VAC_ON 	},
	{	ST_LOAD_MOVE_LOWER, 	EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_LOAD_MOVE_LOWER, 	EV_ERROR,			ST_ERROR				},
	{	ST_LOAD_MOVE_LOWER, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_WAIT_VAC_ON,	EV_VAC_OK,			ST_DONE 				},
	{	ST_LOAD_WAIT_VAC_ON,	EV_VAC_ABORTED, 	ST_ABORT				},
	{	ST_LOAD_WAIT_VAC_ON,	EV_CANCEL,			ST_CANCEL				},

	// unload glass
	{	ST_IDLE,				EV_DO_UNLOAD,		ST_UNLOAD_VAC_OFF		},
	{	ST_UNLOAD_VAC_OFF,		EV_NEXT,			ST_UNLOAD_WAIT_VAC_OFF	},
	{	ST_UNLOAD_VAC_OFF,		EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_WAIT_VAC_OFF, EV_VAC_OK,			ST_UNLOAD_MOVE_ALIGN	},
	{	ST_UNLOAD_WAIT_VAC_OFF, EV_VAC_ABORTED, 	ST_ABORT				},
	{	ST_UNLOAD_WAIT_VAC_OFF, EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_MOVE_ALIGN,	EV_PIN_DONE,		ST_UNLOAD_CLAMP1		},
	{	ST_UNLOAD_MOVE_ALIGN,	EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_UNLOAD_MOVE_ALIGN,	EV_ERROR,			ST_ERROR				},
	{	ST_UNLOAD_MOVE_ALIGN,	EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_CLAMP1,		EV_CLAMP_OK,		ST_UNLOAD_CLAMP2		},
	{	ST_UNLOAD_CLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_UNLOAD_CLAMP1,		EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_CLAMP2,		EV_CLAMP_OK,		ST_UNLOAD_PREP_UNCLAMP	},
	{	ST_UNLOAD_CLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_UNLOAD_CLAMP2,		EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_PREP_UNCLAMP, EV_NEXT,			ST_UNLOAD_UNCLAMP2		},
	{	ST_UNLOAD_PREP_UNCLAMP, EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_UNCLAMP2, 	EV_CLAMP_OK,		ST_UNLOAD_UNCLAMP1		},
	{	ST_UNLOAD_UNCLAMP2, 	EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_UNLOAD_UNCLAMP2, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_UNCLAMP1, 	EV_CLAMP_OK,		ST_UNLOAD_MOVE_UPPER	},
	{	ST_UNLOAD_UNCLAMP1, 	EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_UNLOAD_UNCLAMP1, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_PIN_DONE,		ST_DONE,				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_ERROR,			ST_ERROR				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_CANCEL,			ST_CANCEL				},

	// move to upper
	{	ST_IDLE,				EV_DO_MOVE_UPPER,	ST_MOVE_UPPER			},
	{	ST_MOVE_UPPER,			EV_PIN_DONE,		ST_DONE 				},
	{	ST_MOVE_UPPER,			EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_MOVE_UPPER,			EV_ERROR,			ST_ERROR				},
	{	ST_MOVE_UPPER,			EV_CANCEL,			ST_CANCEL				},

	// move to alignment
	{	ST_IDLE,				EV_DO_MOVE_ALIGN,	ST_MOVE_ALIGN			},
	{	ST_MOVE_ALIGN,			EV_PIN_DONE,		ST_DONE 				},
	{	ST_MOVE_ALIGN,			EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_MOVE_ALIGN,			EV_ERROR,			ST_ERROR				},
	{	ST_MOVE_ALIGN,			EV_CANCEL,			ST_CANCEL				},

	// move to lower
	{	ST_IDLE,				EV_DO_MOVE_LOWER,	ST_MOVE_LOWER			},
	{	ST_MOVE_LOWER,			EV_PIN_DONE,		ST_DONE 				},
	{	ST_MOVE_LOWER,			EV_PIN_ABORTED, 	ST_ABORT				},
	{	ST_MOVE_LOWER,			EV_ERROR,			ST_ERROR				},
	{	ST_MOVE_LOWER,			EV_CANCEL,			ST_CANCEL				},

	// clamp
	{	ST_IDLE,				EV_DO_CLAMP,		ST_CLAMP1				},
	{	ST_CLAMP1,				EV_CLAMP_OK,		ST_CLAMP2				},
	{	ST_CLAMP1,				EV_SKIP,			ST_DONE 				},
	{	ST_CLAMP1,				EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_CLAMP1,				EV_ERROR,			ST_ERROR				},
	{	ST_CLAMP1,				EV_CANCEL,			ST_CANCEL				},
	{	ST_CLAMP2,				EV_CLAMP_OK,		ST_DONE 				},
	{	ST_CLAMP2,				EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_CLAMP2,				EV_ERROR,			ST_ERROR				},
	{	ST_CLAMP2,				EV_CANCEL,			ST_CANCEL				},

	// unclamp
	{	ST_IDLE,				EV_DO_UNCLAMP,		ST_UNCLAMP2 			},
	{	ST_UNCLAMP2,			EV_CLAMP_OK,		ST_UNCLAMP1 			},
	{	ST_UNCLAMP2,			EV_SKIP,			ST_DONE 				},
	{	ST_UNCLAMP2,			EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_UNCLAMP2,			EV_CANCEL,			ST_CANCEL				},
	{	ST_UNCLAMP1,			EV_CLAMP_OK,		ST_DONE 				},
	{	ST_UNCLAMP1,			EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_UNCLAMP1,			EV_CANCEL,			ST_CANCEL				},

	// error handling
	{	ST_IDLE,				EV_ERROR,			ST_ERROR				},
	{	ST_ERROR,				EV_DONE,			ST_IDLE 				},
	{	ST_CANCEL,				EV_DONE,			ST_IDLE 				},

	// termination of current request
	{	ST_DONE,				EV_DONE,			ST_IDLE 				},
	{	ST_ABORT,				EV_DONE,			ST_IDLE 				},

	// -----------------------------------------------------------------------
	// terminate the transition table
	// -----------------------------------------------------------------------
	END_OF_TRANSITION_TABLE
};

// ---------------------------------------------------------------------------
// State-Processing-Table Definitions
// ---------------------------------------------------------------------------
BEGIN_STATEPROC_MAPEX(cTypeB)

	// idle state
	ENTER_STATE(ST_IDLE, enter_ST_IDLE),

	// initialize the resistor
	ENTER_STATE(ST_INIT, enter_ST_INIT),
	ENTER_STATE(ST_INIT_MOVE_LOWER, enter_ST_MOVE_LOWER),
	ENTER_STATE(ST_INIT_UNCLAMP1, enter_ST_UNCLAMP1),
	ENTER_STATE(ST_INIT_UNCLAMP2, enter_ST_UNCLAMP2),
	ENTER_STATE(ST_INIT_DONE, enter_ST_INIT_DONE),

	// origin return
	ENTER_STATE(ST_ORT_MOVE_LOWER, enter_ST_MOVE_LOWER),
	ENTER_STATE(ST_ORT_UNCLAMP1, enter_ST_UNCLAMP1),
	ENTER_STATE(ST_ORT_UNCLAMP2, enter_ST_UNCLAMP2),

	// load glass
	ENTER_STATE(ST_LOAD_MOVE_ALIGN, enter_ST_MOVE_ALIGN),
	ENTER_STATE(ST_LOAD_MOVE_LOWER, enter_ST_MOVE_LOWER),
	ENTER_STATE(ST_LOAD_CLAMP1, enter_ST_CLAMP1),
	ENTER_STATE(ST_LOAD_CLAMP2, enter_ST_CLAMP2),
	ENTER_STATE(ST_LOAD_UNCLAMP1, enter_ST_UNCLAMP1),
	ENTER_STATE(ST_LOAD_UNCLAMP2, enter_ST_UNCLAMP2),
	ENTER_STATE(ST_LOAD_PREP_UNCLAMP, enter_ST_PREP_UNCLAMP),
	ENTER_STATE(ST_LOAD_VAC_ON, enter_ST_VAC_ON),
	ENTER_STATE(ST_LOAD_WAIT_VAC_ON, enter_ST_WAIT_VAC_ON),

	// unload glass
	ENTER_STATE(ST_UNLOAD_MOVE_UPPER, enter_ST_MOVE_UPPER),
	ENTER_STATE(ST_UNLOAD_MOVE_ALIGN, enter_ST_MOVE_ALIGN),
	ENTER_STATE(ST_UNLOAD_CLAMP1, enter_ST_CLAMP1),
	ENTER_STATE(ST_UNLOAD_CLAMP2, enter_ST_CLAMP2),
	ENTER_STATE(ST_UNLOAD_UNCLAMP1, enter_ST_UNCLAMP1),
	ENTER_STATE(ST_UNLOAD_UNCLAMP2, enter_ST_UNCLAMP2),
	ENTER_STATE(ST_UNLOAD_PREP_UNCLAMP, enter_ST_PREP_UNCLAMP),
	ENTER_STATE(ST_UNLOAD_VAC_OFF, enter_ST_VAC_OFF),
	ENTER_STATE(ST_UNLOAD_WAIT_VAC_OFF, enter_ST_WAIT_VAC_OFF),

	// motion control
	ENTER_STATE(ST_MOVE_UPPER, enter_ST_MOVE_UPPER),
	ENTER_STATE(ST_MOVE_ALIGN, enter_ST_MOVE_ALIGN),
	ENTER_STATE(ST_MOVE_LOWER, enter_ST_MOVE_LOWER),

	// clamp / unclamp
	ENTER_STATE(ST_CLAMP1, enter_ST_CLAMP1),
	ENTER_STATE(ST_CLAMP2, enter_ST_CLAMP2),
	ENTER_STATE(ST_UNCLAMP1, enter_ST_UNCLAMP1),
	ENTER_STATE(ST_UNCLAMP2, enter_ST_UNCLAMP2),
	ENTER_STATE(ST_PREP_UNCLAMP, enter_ST_PREP_UNCLAMP),

	// vacuum control
	ENTER_STATE(ST_VAC_ON, enter_ST_VAC_ON),
	ENTER_STATE(ST_VAC_OFF, enter_ST_VAC_OFF),
	ENTER_STATE(ST_WAIT_VAC_ON, enter_ST_WAIT_VAC_ON),
	ENTER_STATE(ST_WAIT_VAC_OFF, enter_ST_WAIT_VAC_OFF),

	// error handling
	ENTER_STATE(ST_ERROR, enter_ST_ERROR),
	ENTER_STATE(ST_CANCEL, enter_ST_CANCEL),

	// termination of current request
	ENTER_STATE(ST_DONE, enter_ST_DONE),
	ENTER_STATE(ST_ABORT, enter_ST_ABORT),

END_STATEPROC_MAPEX(cTypeB)

// ===========================================================================
// Constructor/destructor
// ===========================================================================

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
cTypeB::cTypeB(void) : cCoord(), cActive(TypeATransTable)
{
	m_list.push_back(m_vacuum = (cVacuum*) new cVacuumTypeB());
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
cTypeB::~cTypeB(void)
{
}

// ===========================================================================
// Helper functions for common processing
// ===========================================================================
LRESULT cTypeB::Initialize(void)
{
	Event(EV_DO_INIT);

	// 初期化完了待ち
	while ( !IsIdle() ) {
		basic_timer::do_events(1L);
	}
	return IsInitialized() ? 1L : 0L;
}

void cTypeB::Finalize(void)
{
	cCoord::Finalize();
}

// ---------------------------------------------------------------------------
// イベント解析
// ---------------------------------------------------------------------------
void cTypeB::ParseEvent(void)
{
	ready();

	switch ( client.event ) {
	case nexReturnToOrigin:
		Event(EV_DO_ORT);
		break;
	case nexLoad:
		if ( nexioIsPinUpperPos() )
			Event(EV_DO_LOAD);
		else
			Event(EV_ERROR);
		break;
	case nexUnload:
		if ( nexioIsPinDownPos() ) {
			if ( m_params.System.RobotArmInterlock ) {
				if ( !nexioIsRobotArmDetect() ) {
					nexifAlarmSet(NEXALID_RobotArmDetected);
					Event(EV_ERROR);
					break;
				}
			}
			Event(EV_DO_UNLOAD);
		}
		else
			Event(EV_ERROR);
		break;
	case nexMoveToUpper:
		if ( nexioIsGlassExist() ) {		// 在荷
			if ( nexioIsPinDownPos()) {
				nexifAlarmSet(NEXALID_GlassIsExistsError);
				Event(EV_ERROR);
				break;
			}
		}
		else
		if ( m_params.System.RobotArmInterlock ) {
			if ( !nexioIsRobotArmDetect() ) {
				nexifAlarmSet(NEXALID_RobotArmDetected);
				Event(EV_ERROR);
				break;
			}
		}
		Event(EV_DO_MOVE_UPPER);
		break;
	case nexMoveToAlignment:
		Event(EV_DO_MOVE_ALIGN);
		break;
	case nexMoveToLower:
		Event(EV_DO_MOVE_LOWER);
		break;
	case nexOpenWorkGuide:
		Event(EV_DO_UNCLAMP);
		break;
	case nexCloseWorkGuide:
		Event(EV_DO_CLAMP);
		break;
	case nexInitialize:
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
// 非常停止処理
// ---------------------------------------------------------------------------
void cTypeB::EmergencyStop(void)
{
	cCoord::EmergencyStop();
}

// ---------------------------------------------------------------------------
// 装置状態監視スレッド
// ---------------------------------------------------------------------------
unsigned int cTypeB::start_address (void)
{
	static device_status emergency;
	static device_status eq_power;

	while ( is_loop() ) {
		// 非常停止
		if ( emergency.detect(nexioIsEmergencyStop()).is_down() ||
			 eq_power.detect(nexioIsEquipmentPower()).is_down() ) {
			EmergencyStop();
		}
		// 装置主電源
		Sleep(1);
	}

	return 0L;
}

// ---------------------------------------------------------------------------
BOOL cTypeB::IsIdle(void) const
{
	return CurrState == ST_IDLE;
}

// ===========================================================================
// State processing
// ===========================================================================

// ---------------------------------------------------------------------------
// ホストへ完了報告及びステータスアイドル移行
// ---------------------------------------------------------------------------
void cTypeB::enter_ST_IDLE(const cEventParams* const /*params*/)
{
	PostClientMessage();
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_INIT(const cEventParams* const /*params*/)
{
	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_INIT_DONE(const cEventParams* const /*params*/)
{
	m_initialized = TRUE;

	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_MOVE_UPPER(const cEventParams* const /*params*/)
{
	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		Event(EV_ERROR);
	}
	else
	if ( !nexioIsVacuumOff() ) {		  // 吸着確認
		nexifAlarmSet(NEXALID_VacuumIsNotOffError);
		Event(EV_ERROR);
	}
	else
	if ( !MoveToUpperPos() ) {			// 上昇端移動開始
		Event(EV_PIN_ABORTED);
	}
	else {
		Event(EV_PIN_DONE);
	 }
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_MOVE_ALIGN(const cEventParams* const /*params*/)
{
	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		Event(EV_ERROR);
	}
	else
	if ( !nexioIsVacuumOff() ) {		  // 吸着確認
		nexifAlarmSet(NEXALID_VacuumIsNotOffError);
		Event(EV_ERROR);
	}
	else
	if ( !MoveToAlignmentPos() ) {		// アライメント移動開始
		Event(EV_PIN_ABORTED);
	}
	else {
		Event(EV_PIN_DONE);
	 }
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_MOVE_LOWER(const cEventParams* const /*params*/)
{
	if ( !MoveToLowerPos() ) {			// 下降端移動開始
		//Alarm
		Event(EV_PIN_ABORTED);
	}
	else {
		Event(EV_PIN_DONE);
	 }
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_CLAMP1(const cEventParams* const /*params*/)
{
	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		Event(EV_ERROR);
	}
	else
	if ( DoClampWaitForFinished (		// クランプ開始・完了待ち
			 nexioWorkGuideCloseEx,
			 nexioIsWorkGuideCloseEx,
// matsuhisa 2011.11.12 modified >>>
//			 0,
			 m_params.System.FirstValve,
// matsuhisa 2011.11.12 modified <<<
			 m_params.MotionTimeouts.WorkGuide ) ) {
		Event(EV_CLAMP_OK);
	}
	else {
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_CLAMP2(const cEventParams* const /*params*/)
{
	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		Event(EV_ERROR);
	}
	else

	if ( DoClampWaitForFinished (		// クランプ開始・完了待ち
			 nexioWorkGuideCloseEx,
			 nexioIsWorkGuideCloseEx,
// matsuhisa 2011.11.12 modified >>>
//			 1,
			 m_params.System.FirstValve ^ 1,
// matsuhisa 2011.11.12 modified <<<
			 m_params.MotionTimeouts.WorkGuide ) ) {
		Event(EV_CLAMP_OK);
	}
	else {
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_UNCLAMP1(const cEventParams* const /*params*/)
{
	if ( DoClampWaitForFinished (		// アンクランプ開始・完了待ち
			 nexioWorkGuideOpenEx,
			 nexioIsWorkGuideOpenEx,
// matsuhisa 2011.11.12 modified >>>
//			 0,
			 m_params.System.FirstValve,
// matsuhisa 2011.11.12 modified <<<
			 m_params.MotionTimeouts.WorkGuide ) ) {
		Event(EV_CLAMP_OK);
	}
	else {
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_UNCLAMP2(const cEventParams* const /*params*/)
{
	if ( DoClampWaitForFinished (		// アンクランプ開始・完了待ち
			 nexioWorkGuideOpenEx,
			 nexioIsWorkGuideOpenEx,
// matsuhisa 2011.11.12 modified >>>
//			 1,
			 m_params.System.FirstValve ^ 1,
// matsuhisa 2011.11.12 modified <<<
			 m_params.MotionTimeouts.WorkGuide ) ) {
		Event(EV_CLAMP_OK);
	}
	else {
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_PREP_UNCLAMP(const cEventParams* const /*params*/)
{
	// アンクランプ開始まで遅延
	basic_timer timer;
	timer.delay(m_params.MotionDelay.WorkGuideOpen);

	if ( is_cancel() ) {
		Event(EV_CANCEL);
	}
	else {
		Event(EV_NEXT);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_VAC_ON(const cEventParams* const evp)
{
	m_vacuum->ChuckValve(FALSE);

	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_VAC_OFF(const cEventParams* const /*params*/)
{
	m_vacuum->UnchuckValve(FALSE);

	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_WAIT_VAC_ON(const cEventParams* const /*params*/)
{
	if ( is_cancel() ) {
		Event(EV_CANCEL);
		return;
	}

	if ( m_params.System.VacuumFeedback ) {
		basic_timer wait(m_params.MotionTimeouts.VacuumOn);
		while (1) {
			if ( nexioIsVacuumOn() ) {
				Event(EV_VAC_OK);
				break;
			}
			else
			if ( wait.is_timeout() ) {
				nexifAlarmSet(NEXALID_VacuumOnTimeouts);
				Event(EV_VAC_ABORTED);
				break;
			}
			else
			if ( is_cancel() ) {
				Event(EV_CANCEL);
				break;
			}
			basic_timer::do_events(1L);
		}
	}
	else {
		Event(EV_VAC_OK);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_WAIT_VAC_OFF(const cEventParams* const /*params*/)
{
	if ( is_cancel() ) {
		Event(EV_CANCEL);
		return;
	}

	basic_timer wait(m_params.MotionTimeouts.VacuumOff);
	while (1) {
		if ( nexioIsVacuumOff() ) {
			Event(EV_VAC_OK);
			break;
		}
		else
		if ( wait.is_timeout() ) {
			nexifAlarmSet(NEXALID_VacuumOffTimeouts);
			Event(EV_VAC_ABORTED);
			break;
		}
		else
		if ( is_cancel() ) {
			Event(EV_CANCEL);
			break;
		}
		basic_timer::do_events(1L);
	}
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_ERROR(const cEventParams* const evp)
{
	Event(EV_DONE);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_CANCEL(const cEventParams* const /*params*/)
{
	Event(EV_DONE);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_DONE(const cEventParams* const /*params*/)
{
	client.result = TRUE;

	Event(EV_DONE);
}

// ---------------------------------------------------------------------------
void cTypeB::enter_ST_ABORT(const cEventParams* const /*params*/)
{
	Event(EV_DONE);
}
