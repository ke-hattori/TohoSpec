#pragma warning (disable:4786)
#include <set>
#include <algorithm>
#include <active.hxx>
#define __NEXTRA_INTERNAL
#include <nexmisc.hxx>
#include <nexini.hxx>
#include <nexiobase.hxx>
#include <nexax.hxx>
#include <nexrc.hxx>
#include "coord.hpp"
#include "typea.hpp"

#ifdef _DEBUG
#pragma comment(lib, "dtnsactive.lib")
#pragma comment(lib, "dtnsnexif.lib")
#pragma comment(lib, "dtnsnexax.lib")
#pragma comment(lib, "dtnsnexio.lib")
#pragma comment(lib, "dtnsnexui.lib")
#pragma comment(lib, "dtnsnexini.lib")
#else
#pragma comment(lib, "tnsactive.lib")
#pragma comment(lib, "tnsnexif.lib")
#pragma comment(lib, "tnsnexax.lib")
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
	ST_INIT_SERVO_ON,			// servo on
	ST_INIT_MOVE_HOME,			// move home
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
#ifdef __NEXPROC_NO_PIN_UP_CHECK
	ST_LOAD_PARSE_NEXT,
	ST_LOAD_VAC_OFF,
	ST_LOAD_WAIT_VAC_OFF,
#endif

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

	// air float load
	ST_AFLOAD_MOVE_LOWER,
	ST_AFLOAD_CLAMP1,
	ST_AFLOAD_CLAMP2,
	ST_AFLOAD_UNCLAMP1,
	ST_AFLOAD_UNCLAMP2,
	ST_AFLOAD_PREP_UNCLAMP,
	ST_AFLOAD_VAC_ON,
	ST_AFLOAD_WAIT_VAC_ON,

	// air float control
	ST_AFLOAD_FLOAT_ON,
	ST_AFLOAD_FLOAT_OFF,

	// motion control
	ST_MOVE_UPPER,
	ST_MOVE_ALIGN,
	ST_MOVE_LOWER,
#ifdef __NEXPROC_NO_PIN_UP_CHECK
	ST_UPPER_PARSE_NEXT,
	ST_UPPER_VAC_OFF,
	ST_UPPER_WAIT_VAC_OFF,
	ST_ALIGN_PARSE_NEXT,
	ST_ALIGN_VAC_OFF,
	ST_ALIGN_WAIT_VAC_OFF,
#endif

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
	EV_DO_AFLOAD,		// glass air float load command
	EV_DO_UNLOAD,		// glass to be unloaded
	EV_DO_MOVE_UPPER,
	EV_DO_MOVE_ALIGN,
	EV_DO_MOVE_LOWER,
	EV_DO_CLAMP,
	EV_DO_UNCLAMP,

	// ADAX servo completion
	EV_ADAX_DONE,		// ADAX servo has completed last request
	EV_ADAX_ABORTED,	// ADAX servo failed

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
	{	ST_INIT,				EV_NEXT,			ST_INIT_SERVO_ON		},
	{	ST_INIT,				EV_ERROR,			ST_ERROR				},
	{	ST_INIT_SERVO_ON,		EV_NEXT,			ST_INIT_UNCLAMP2		},
	{	ST_INIT_SERVO_ON,		EV_ERROR,			ST_ERROR				},
	{	ST_INIT_UNCLAMP2,		EV_CLAMP_OK,		ST_INIT_UNCLAMP1		},
	{	ST_INIT_UNCLAMP2,		EV_SKIP,			ST_INIT_MOVE_HOME		},
	{	ST_INIT_UNCLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_INIT_UNCLAMP1,		EV_CLAMP_OK,		ST_INIT_MOVE_HOME		},
	{	ST_INIT_UNCLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_INIT_MOVE_HOME,		EV_ADAX_DONE,		ST_INIT_DONE			},
	{	ST_INIT_MOVE_HOME,		EV_DO_MOVE_LOWER,	ST_INIT_MOVE_LOWER		},
	{	ST_INIT_MOVE_HOME,		EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_INIT_MOVE_LOWER, 	EV_ADAX_DONE,		ST_INIT_DONE			},
	{	ST_INIT_MOVE_LOWER, 	EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_INIT_MOVE_LOWER, 	EV_ERROR,			ST_ERROR				},
	{	ST_INIT_DONE,			EV_NEXT,			ST_DONE 				},

	// origin return
	{	ST_IDLE,				EV_DO_ORT,			ST_ORT_UNCLAMP2 		},
	{	ST_ORT_UNCLAMP2,		EV_CLAMP_OK,		ST_ORT_UNCLAMP1 		},
	{	ST_ORT_UNCLAMP2,		EV_SKIP,			ST_ORT_MOVE_LOWER		},
	{	ST_ORT_UNCLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_ORT_UNCLAMP1,		EV_CLAMP_OK,		ST_ORT_MOVE_LOWER		},
	{	ST_ORT_UNCLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_ORT_MOVE_LOWER,		EV_ADAX_DONE,		ST_DONE 				},
	{	ST_ORT_MOVE_LOWER,		EV_SKIP,			ST_DONE 				},
	{	ST_ORT_MOVE_LOWER,		EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_ORT_MOVE_LOWER,		EV_ERROR,			ST_ERROR				},
	{	ST_ORT_MOVE_LOWER,		EV_CANCEL,			ST_CANCEL				},

	// load glass
#ifndef __NEXPROC_NO_PIN_UP_CHECK
	{	ST_IDLE,				EV_DO_LOAD, 		ST_LOAD_MOVE_ALIGN		},
#else
	{	ST_IDLE,				EV_DO_LOAD, 		ST_LOAD_PARSE_NEXT		},
	{	ST_LOAD_PARSE_NEXT, 	EV_NEXT,			ST_LOAD_VAC_OFF 		},
	{	ST_LOAD_PARSE_NEXT, 	EV_SKIP,			ST_LOAD_MOVE_ALIGN		},
	{	ST_LOAD_VAC_OFF,		EV_NEXT,			ST_LOAD_WAIT_VAC_OFF	},
	{	ST_LOAD_VAC_OFF,		EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_WAIT_VAC_OFF,	EV_VAC_OK,			ST_LOAD_MOVE_ALIGN		},
	{	ST_LOAD_WAIT_VAC_OFF,	EV_CANCEL,			ST_CANCEL				},
	{	ST_LOAD_WAIT_VAC_OFF,	EV_VAC_ABORTED, 	ST_ABORT				},
#endif
	{	ST_LOAD_MOVE_ALIGN, 	EV_ADAX_DONE,		ST_LOAD_CLAMP1			},
	{	ST_LOAD_MOVE_ALIGN, 	EV_ADAX_ABORTED,	ST_ABORT				},
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
	{	ST_LOAD_MOVE_LOWER, 	EV_ADAX_DONE,		ST_LOAD_WAIT_VAC_ON 	},
	{	ST_LOAD_MOVE_LOWER, 	EV_ADAX_ABORTED,	ST_ABORT				},
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
	{	ST_UNLOAD_MOVE_ALIGN,	EV_ADAX_DONE,		ST_UNLOAD_CLAMP1		},
	{	ST_UNLOAD_MOVE_ALIGN,	EV_ADAX_ABORTED,	ST_ABORT				},
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
	{	ST_UNLOAD_MOVE_UPPER,	EV_ADAX_DONE,		ST_DONE,				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_ERROR,			ST_ERROR				},
	{	ST_UNLOAD_MOVE_UPPER,	EV_CANCEL,			ST_CANCEL				},

	// air load glass
	{	ST_IDLE,				EV_DO_AFLOAD,		ST_AFLOAD_MOVE_LOWER	},
	{	ST_AFLOAD_MOVE_LOWER,	EV_ADAX_DONE,		ST_AFLOAD_FLOAT_ON		},
	{	ST_AFLOAD_MOVE_LOWER,	EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_AFLOAD_MOVE_LOWER,	EV_ERROR,			ST_ERROR				},
	{	ST_AFLOAD_MOVE_LOWER,	EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_FLOAT_ON, 	EV_NEXT,			ST_AFLOAD_CLAMP1		},
	{	ST_AFLOAD_FLOAT_ON, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_CLAMP1,		EV_CLAMP_OK,		ST_AFLOAD_CLAMP2		},
	{	ST_AFLOAD_CLAMP1,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_AFLOAD_CLAMP1,		EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_CLAMP2,		EV_CLAMP_OK,		ST_AFLOAD_PREP_UNCLAMP	},
	{	ST_AFLOAD_CLAMP2,		EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_CLAMP2,		EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_AFLOAD_PREP_UNCLAMP, EV_NEXT,			ST_AFLOAD_FLOAT_OFF 	},
	{	ST_AFLOAD_PREP_UNCLAMP, EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_FLOAT_OFF,	EV_NEXT,			ST_AFLOAD_VAC_ON		},
	{	ST_AFLOAD_FLOAT_OFF,	EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_VAC_ON,		EV_NEXT,			ST_AFLOAD_UNCLAMP2		},
	{	ST_AFLOAD_VAC_ON,		EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_UNCLAMP2, 	EV_CLAMP_OK,		ST_AFLOAD_UNCLAMP1		},
	{	ST_AFLOAD_UNCLAMP2, 	EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_AFLOAD_UNCLAMP2, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_UNCLAMP1, 	EV_CLAMP_OK,		ST_LOAD_WAIT_VAC_ON 	},
	{	ST_AFLOAD_UNCLAMP1, 	EV_CLAMP_ABORTED,	ST_ABORT				},
	{	ST_AFLOAD_UNCLAMP1, 	EV_CANCEL,			ST_CANCEL				},
	{	ST_AFLOAD_WAIT_VAC_ON,	EV_VAC_OK,			ST_DONE 				},
	{	ST_AFLOAD_WAIT_VAC_ON,	EV_VAC_ABORTED, 	ST_ABORT				},
	{	ST_AFLOAD_WAIT_VAC_ON,	EV_CANCEL,			ST_CANCEL				},

	// move to upper
#ifndef __NEXPROC_NO_PIN_UP_CHECK
	{	ST_IDLE,				EV_DO_MOVE_UPPER,	ST_MOVE_UPPER			},
#else
	{	ST_IDLE,				EV_DO_MOVE_UPPER,	ST_UPPER_PARSE_NEXT 	},
	{	ST_UPPER_PARSE_NEXT,	EV_NEXT,			ST_UPPER_VAC_OFF		},
	{	ST_UPPER_PARSE_NEXT,	EV_SKIP,			ST_MOVE_UPPER			},
	{	ST_UPPER_VAC_OFF,		EV_NEXT,			ST_UPPER_WAIT_VAC_OFF	},
	{	ST_UPPER_VAC_OFF,		EV_CANCEL,			ST_CANCEL				},
	{	ST_UPPER_WAIT_VAC_OFF,	EV_VAC_OK,			ST_MOVE_UPPER			},
	{	ST_UPPER_WAIT_VAC_OFF,	EV_CANCEL,			ST_CANCEL				},
	{	ST_UPPER_WAIT_VAC_OFF,	EV_VAC_ABORTED, 	ST_ABORT				},
#endif
	{	ST_MOVE_UPPER,			EV_ADAX_DONE,		ST_DONE 				},
	{	ST_MOVE_UPPER,			EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_MOVE_UPPER,			EV_ERROR,			ST_ERROR				},
	{	ST_MOVE_UPPER,			EV_CANCEL,			ST_CANCEL				},

	// move to alignment
#ifndef __NEXPROC_NO_PIN_UP_CHECK
	{	ST_IDLE,				EV_DO_MOVE_ALIGN,	ST_MOVE_ALIGN			},
#else
	{	ST_IDLE,				EV_DO_MOVE_ALIGN,	ST_ALIGN_PARSE_NEXT 	},
	{	ST_ALIGN_PARSE_NEXT,	EV_NEXT,			ST_ALIGN_VAC_OFF		},
	{	ST_ALIGN_PARSE_NEXT,	EV_SKIP,			ST_MOVE_ALIGN			},
	{	ST_ALIGN_VAC_OFF,		EV_NEXT,			ST_ALIGN_WAIT_VAC_OFF	},
	{	ST_ALIGN_VAC_OFF,		EV_CANCEL,			ST_CANCEL				},
	{	ST_ALIGN_WAIT_VAC_OFF,	EV_VAC_OK,			ST_MOVE_ALIGN			},
	{	ST_ALIGN_WAIT_VAC_OFF,	EV_CANCEL,			ST_CANCEL				},
	{	ST_ALIGN_WAIT_VAC_OFF,	EV_VAC_ABORTED, 	ST_ABORT				},
#endif
	{	ST_MOVE_ALIGN,			EV_ADAX_DONE,		ST_DONE 				},
	{	ST_MOVE_ALIGN,			EV_ADAX_ABORTED,	ST_ABORT				},
	{	ST_MOVE_ALIGN,			EV_ERROR,			ST_ERROR				},
	{	ST_MOVE_ALIGN,			EV_CANCEL,			ST_CANCEL				},

	// move to lower
	{	ST_IDLE,				EV_DO_MOVE_LOWER,	ST_MOVE_LOWER			},
	{	ST_MOVE_LOWER,			EV_ADAX_DONE,		ST_DONE 				},
	{	ST_MOVE_LOWER,			EV_ADAX_ABORTED,	ST_ABORT				},
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
BEGIN_STATEPROC_MAPEX(cTypeA)

	// idle state
	ENTER_STATE(ST_IDLE, enter_ST_IDLE),

	// initialize the resistor
	ENTER_STATE(ST_INIT, enter_ST_INIT),
	ENTER_STATE(ST_INIT_SERVO_ON, enter_ST_INIT_SERVO_ON),
	ENTER_STATE(ST_INIT_MOVE_HOME, enter_ST_INIT_MOVE_HOME),
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
#ifdef __NEXPROC_NO_PIN_UP_CHECK
	ENTER_STATE(ST_LOAD_PARSE_NEXT, enter_ST_PARSE_NEXT),
	ENTER_STATE(ST_LOAD_VAC_OFF, enter_ST_VAC_OFF),
	ENTER_STATE(ST_LOAD_WAIT_VAC_OFF, enter_ST_WAIT_VAC_OFF),
#endif

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

	// air float load
	ENTER_STATE(ST_AFLOAD_MOVE_LOWER, enter_ST_MOVE_LOWER),
	ENTER_STATE(ST_AFLOAD_CLAMP1, enter_ST_CLAMP1),
	ENTER_STATE(ST_AFLOAD_CLAMP2, enter_ST_CLAMP2),
	ENTER_STATE(ST_AFLOAD_CLAMP1, enter_ST_UNCLAMP1),
	ENTER_STATE(ST_AFLOAD_CLAMP2, enter_ST_UNCLAMP2),
	ENTER_STATE(ST_AFLOAD_PREP_UNCLAMP, enter_ST_PREP_UNCLAMP),
	ENTER_STATE(ST_AFLOAD_VAC_ON, enter_ST_VAC_ON),
	ENTER_STATE(ST_AFLOAD_WAIT_VAC_ON, enter_ST_WAIT_VAC_ON),

	// air float control
	ENTER_STATE(ST_AFLOAD_FLOAT_ON, enter_ST_AFLOAD_FLOAT_ON),
	ENTER_STATE(ST_AFLOAD_FLOAT_OFF, enter_ST_AFLOAD_FLOAT_OFF),

	// motion control
	ENTER_STATE(ST_MOVE_UPPER, enter_ST_MOVE_UPPER),
	ENTER_STATE(ST_MOVE_ALIGN, enter_ST_MOVE_ALIGN),
	ENTER_STATE(ST_MOVE_LOWER, enter_ST_MOVE_LOWER),
#ifdef __NEXPROC_NO_PIN_UP_CHECK
	ENTER_STATE(ST_UPPER_PARSE_NEXT, enter_ST_PARSE_NEXT),
	ENTER_STATE(ST_UPPER_VAC_OFF, enter_ST_VAC_OFF),
	ENTER_STATE(ST_UPPER_WAIT_VAC_OFF, enter_ST_WAIT_VAC_OFF),
	ENTER_STATE(ST_ALIGN_PARSE_NEXT, enter_ST_PARSE_NEXT),
	ENTER_STATE(ST_ALIGN_VAC_OFF, enter_ST_VAC_OFF),
	ENTER_STATE(ST_ALIGN_WAIT_VAC_OFF, enter_ST_WAIT_VAC_OFF),
#endif

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

END_STATEPROC_MAPEX(cTypeA)

// ===========================================================================
// Constructor/destructor
// ===========================================================================

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
cTypeA::cTypeA(void) : cCoord(), cActive(TypeATransTable)
{
	m_list.push_back(m_vacuum = (cVacuum*) new cVacuumTypeA());
	m_list.push_back(m_airfloat = (cAirFloat*) new cAirFloatTypeA());
}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
cTypeA::~cTypeA(void)
{
}

// ===========================================================================
// Helper functions for common processing
// ===========================================================================
LRESULT cTypeA::Initialize(void)
{
	m_lfs << arrange << datetime << "Initialize" << std::endl;

	if ( m_params.Options.RoboCylinder ) {
		if ( !m_robo->Initialize() ) {
			nexifAlarmSet(NEXALID_RcInitializeFailure);
			return 0L;
		}
	}

	nexaxInitialize();

	m_lfs << arrange << datetime << "EV_DO_INIT" << std::endl;
	Event(EV_DO_INIT);

	// 初期化完了待ち
	while ( !IsIdle() ) {
		basic_timer::do_events(1L);
	}
	return IsInitialized() ? 1L : 0L;
}

void cTypeA::Finalize(void)
{
	cCoord::Finalize();

	nexaxFinalize();
}

// ---------------------------------------------------------------------------
// イベント解析
// ---------------------------------------------------------------------------
void cTypeA::ParseEvent(void)
{
	m_lfs << arrange << datetime << "ParseEvent(" << setw(2) << setfill('0') << client.event << ")" << std::endl;

	ready();

	switch ( client.event ) {
	case nexReturnToOrigin:
		m_lfs << arrange << datetime << "EV_DO_ORT" << std::endl;
		Event(EV_DO_ORT);
		break;
	case nexLoad:
#ifndef __NEXPROC_NO_PIN_UP_CHECK
		if ( nexioIsPinUpperPos() ) {
			m_lfs << arrange << datetime << "EV_DO_LOAD" << std::endl;
			Event(EV_DO_LOAD);
		}
		else {
			m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
			Event(EV_ERROR);
		}
#else
		m_lfs << arrange << datetime << "EV_DO_LOAD" << std::endl;
		Event(EV_DO_LOAD);
#endif
		break;
	case nexUnload:
		if ( nexioIsPinDownPos() ) {
			if ( m_params.System.RobotArmInterlock ) {
				if ( !nexioIsRobotArmDetect() ) {
					nexifAlarmSet(NEXALID_RobotArmDetected);
					m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
					Event(EV_ERROR);
					break;
				}
			}
			m_lfs << arrange << datetime << "EV_DO_UNLOAD" << std::endl;
			Event(EV_DO_UNLOAD);
		}
		else {
			m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
			Event(EV_ERROR);
		}
		break;
	case nexMoveToUpper:
		if ( nexioIsGlassExist() ) {		// 在荷
			if ( nexioIsPinDownPosEx(nexax_axis_1) || nexioIsPinDownPosEx(nexax_axis_2) ) {
				nexifAlarmSet(NEXALID_GlassIsExistsError);
				m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
				Event(EV_ERROR);
				break;
			}
		}
		else
		if ( m_params.System.RobotArmInterlock ) {
			if ( !nexioIsRobotArmDetect() ) {
				nexifAlarmSet(NEXALID_RobotArmDetected);
				m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
				Event(EV_ERROR);
				break;
			}
		}
		m_lfs << arrange << datetime << "EV_DO_MOVE_UPPER" << std::endl;
		Event(EV_DO_MOVE_UPPER);
		break;
	case nexMoveToAlignment:
		m_lfs << arrange << datetime << "EV_DO_MOVE_ALIGN" << std::endl;
		Event(EV_DO_MOVE_ALIGN);
		break;
	case nexMoveToLower:
		m_lfs << arrange << datetime << "EV_DO_MOVE_LOWER" << std::endl;
		Event(EV_DO_MOVE_LOWER);
		break;
	case nexOpenWorkGuide:
		m_lfs << arrange << datetime << "EV_DO_UNCLAMP" << std::endl;
		Event(EV_DO_UNCLAMP);
		break;
	case nexCloseWorkGuide:
		m_lfs << arrange << datetime << "EV_DO_CLAMP" << std::endl;
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
void cTypeA::EmergencyStop(void)
{
	nexaxFinalize();

	cCoord::EmergencyStop();
}

// ---------------------------------------------------------------------------
// 装置状態監視スレッド
// ---------------------------------------------------------------------------
unsigned int cTypeA::start_address (void)
{
	static device_list	 servo(nexax_axis_max);
	static device_status emergency;
	static device_status eq_power;

	while ( is_loop() ) {
		// 非常停止
		if ( emergency.detect(nexioIsEmergencyStop()).is_down() ||
			 eq_power.detect(nexioIsEquipmentPower()).is_down() ) {
			m_lfs << arrange << datetime << "EmergencyStop" << std::endl;
			EmergencyStop();
		}
		// 装置主電源
		for ( unsigned short axis = nexax_axis_1; axis < nexax_axis_max; axis++ ) {
			if ( servo[axis].detect(nexioIsServoOnEx(axis)).is_down() ) {
				m_lfs << arrange << datetime << "PowerDown" << std::endl;
				nexaxAlarmSet(NEXALID_MainPowerOnError, axis);
			}
		}
		Sleep(1);
	}

	return 0L;
}

// ---------------------------------------------------------------------------
BOOL cTypeA::IsIdle(void) const
{
	return CurrState == ST_IDLE;
}

// ---------------------------------------------------------------------------
// 動作完了待ち
// ---------------------------------------------------------------------------
void cTypeA::WaitForMotionFinished (void)
{
	unsigned long retval[nexax_axis_max];
	retval[nexax_axis_1] = nexaxWaitForMotionFinished(nexax_axis_1);
	retval[nexax_axis_2] = nexaxWaitForMotionFinished(nexax_axis_2);

	if ( retval[nexax_axis_1] == NEXAXERR_CANCELED ||
		 retval[nexax_axis_2] == NEXAXERR_CANCELED ) {
		m_lfs << arrange << datetime << "EV_CANCEL" << std::endl;
		Event(EV_CANCEL);
	}
	else
	if ( retval[nexax_axis_1] == NEXAXERR_NONE &&
		 retval[nexax_axis_2] == NEXAXERR_NONE ) {
		m_lfs << arrange << datetime << "EV_ADAX_DONE" << std::endl;
		Event(EV_ADAX_DONE);
	}
	else {
		m_lfs << arrange << datetime << "EV_ADAX_ABORTED" << std::endl;
		Event(EV_ADAX_ABORTED);
	}
}

// ===========================================================================
// State processing
// ===========================================================================

// ---------------------------------------------------------------------------
// ホストへ完了報告及びステータスアイドル移行
// ---------------------------------------------------------------------------
void cTypeA::enter_ST_IDLE(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_IDLE" << std::endl;

	PostClientMessage();
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_INIT(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_INIT" << std::endl;

	// check whether the servo is ready
	if ( IsHardwareSimulation() || IsServoReady() ) {
		m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
		Event(EV_NEXT);
	}
	else {
		Event(EV_ERROR);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_INIT_SERVO_ON(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_INIT_SERVO_ON" << std::endl;

	ServoOn(ON);

	basic_timer wait;

	// サーボON検出まで遅延
	wait.delay(m_params.MotionDelay.ServoOn);

	// check whether the servo is ready
	if ( !IsExistServoAlarm() || IsHardwareSimulation() ) {
		// サーボブレーキ解除
		ServoBrake(ON);
		m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
		Event(EV_NEXT);
	}
	else {
		// サーボON失敗
		for ( unsigned short axis = nexax_axis_1; axis < nexax_axis_max; axis++ ) {
			if ( nexioIsServoAlarmEx(axis) )
				nexaxAlarmSet(NEXALID_ServoOnFailure, axis);
		}
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
		Event(EV_ERROR);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_INIT_MOVE_HOME(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_INIT_MOVE_HOME" << std::endl;

	// すでに初期化済
	if ( IsInitialized() ) {
		m_lfs << arrange << datetime << "EV_DO_MOVE_LOWER" << std::endl;
		Event(EV_DO_MOVE_LOWER);
	}
	else {
		// 原点復帰開始
		if ( !MoveToHomePos() ) {
			 m_lfs << arrange << datetime << "EV_ADAX_ABORTED" << std::endl;
			 Event(EV_ADAX_ABORTED);
		}
		else {
			// 原点復帰完了待ち
			unsigned long retval[nexax_axis_max];
			retval[nexax_axis_1] = nexaxWaitForHomeMotionFinished(nexax_axis_1);
			retval[nexax_axis_2] = nexaxWaitForHomeMotionFinished(nexax_axis_2);

			if ( retval[nexax_axis_1] == NEXAXERR_NONE &&
				 retval[nexax_axis_2] == NEXAXERR_NONE ) {
				m_lfs << arrange << datetime << "EV_ADAX_DONE" << std::endl;
				Event(EV_ADAX_DONE);
			}
			else {
				m_lfs << arrange << datetime << "EV_ADAX_ABORTED" << std::endl;
				Event(EV_ADAX_ABORTED);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_INIT_DONE(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_INIT_DONE" << std::endl;

	m_initialized = TRUE;

	m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_MOVE_UPPER(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_MOVE_UPPER" << std::endl;

	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
		Event(EV_ERROR);
	}
	else
	if ( !nexioIsVacuumOff() ) {		  // 吸着確認
		nexifAlarmSet(NEXALID_VacuumIsNotOffError);
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
		Event(EV_ERROR);
	}
	else
	if ( !MoveToUpperPos() ) {			// 上昇端移動開始
		 m_lfs << arrange << datetime << "EV_ADAX_ABORTED" << std::endl;
		 Event(EV_ADAX_ABORTED);
	}
	else {
		 WaitForMotionFinished();		// 動作完了待ち
	 }
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_MOVE_ALIGN(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_MOVE_ALIGN" << std::endl;

	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
		Event(EV_ERROR);
	}
	else
	if ( !nexioIsVacuumOff() ) {		  // 吸着確認
		nexifAlarmSet(NEXALID_VacuumIsNotOffError);
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
		Event(EV_ERROR);
	}
	else
	if ( !MoveToAlignmentPos() ) {		// アライメント移動開始
		 m_lfs << arrange << datetime << "EV_ADAX_ABORTED" << std::endl;
		 Event(EV_ADAX_ABORTED);
	}
	else {
		 WaitForMotionFinished();		// 動作完了待ち
	 }
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_MOVE_LOWER(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_MOVE_LOWER" << std::endl;

	if ( !MoveToLowerPos() ) {			// 下降端移動開始
		//Alarm
		 m_lfs << arrange << datetime << "EV_ADAX_ABORTED" << std::endl;
		 Event(EV_ADAX_ABORTED);
	}
	else {
		 WaitForMotionFinished();		// 動作完了待ち
	 }
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_AFLOAD_FLOAT_ON(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_AFLOAD_FLOAT_ON" << std::endl;

	m_airfloat->ChuckValve();

	m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_AFLOAD_FLOAT_OFF(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_AFLOAD_FLOAT_OFF" << std::endl;

	m_airfloat->UnchuckValve();

	m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_CLAMP1(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_CLAMP1" << std::endl;

	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
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
		m_lfs << arrange << datetime << "EV_CLAMP_OK" << std::endl;
		Event(EV_CLAMP_OK);
	}
	else {
		m_lfs << arrange << datetime << "EV_CLAMP_ABORTED" << std::endl;
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_CLAMP2(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "EV_CLAMP_ABORTED" << std::endl;

	if ( !nexioIsStageLoadPos() ) { 	// ロードポジション確認
		nexifAlarmSet(NEXALID_StageIsNotLoadPosError);
		m_lfs << arrange << datetime << "EV_ERROR" << std::endl;
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
		m_lfs << arrange << datetime << "EV_CLAMP_OK" << std::endl;
		Event(EV_CLAMP_OK);
	}
	else {
		m_lfs << arrange << datetime << "EV_CLAMP_ABORTED" << std::endl;
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_UNCLAMP1(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_UNCLAMP1" << std::endl;

	if ( DoClampWaitForFinished (		// アンクランプ開始・完了待ち
			 nexioWorkGuideOpenEx,
			 nexioIsWorkGuideOpenEx,
// matsuhisa 2011.11.12 modified >>>
//			 0,
			 m_params.System.FirstValve,
// matsuhisa 2011.11.12 modified <<<
			 m_params.MotionTimeouts.WorkGuide ) ) {
		m_lfs << arrange << datetime << "EV_CLAMP_OK" << std::endl;
		Event(EV_CLAMP_OK);
	}
	else {
		m_lfs << arrange << datetime << "EV_CLAMP_ABORTED" << std::endl;
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_UNCLAMP2(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_UNCLAMP2" << std::endl;

	if ( DoClampWaitForFinished (		// アンクランプ開始・完了待ち
			 nexioWorkGuideOpenEx,
			 nexioIsWorkGuideOpenEx,
// matsuhisa 2011.11.12 modified >>>
//			 1,
			 m_params.System.FirstValve ^ 1,
// matsuhisa 2011.11.12 modified <<<
			 m_params.MotionTimeouts.WorkGuide ) ) {
		m_lfs << arrange << datetime << "EV_CLAMP_OK" << std::endl;
		Event(EV_CLAMP_OK);
	}
	else {
		m_lfs << arrange << datetime << "EV_CLAMP_ABORTED" << std::endl;
		Event(EV_CLAMP_ABORTED);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_PREP_UNCLAMP(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_PREP_UNCLAMP" << std::endl;

	// アンクランプ開始まで遅延
	basic_timer timer;
	timer.delay(m_params.MotionDelay.WorkGuideOpen);

	if ( is_cancel() ) {
		m_lfs << arrange << datetime << "EV_CANCEL" << std::endl;
		Event(EV_CANCEL);
	}
	else {
		m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
		Event(EV_NEXT);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_VAC_ON(const cEventParams* const evp)
{
	m_lfs << arrange << datetime << "enter_ST_VAC_ON" << std::endl;

	m_vacuum->ChuckValve(FALSE);

	m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_VAC_OFF(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_VAC_OFF" << std::endl;

	m_vacuum->UnchuckValve(FALSE);

	m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
	Event(EV_NEXT);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_WAIT_VAC_ON(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_WAIT_VAC_ON" << std::endl;

	if ( is_cancel() ) {
		m_lfs << arrange << datetime << "EV_CANCEL" << std::endl;
		Event(EV_CANCEL);
		return;
	}

	if ( m_params.System.VacuumFeedback ) {
		basic_timer wait(m_params.MotionTimeouts.VacuumOn);
		while (1) {
			if ( nexioIsVacuumOn() ) {
				m_lfs << arrange << datetime << "EV_VAC_OK" << std::endl;
				Event(EV_VAC_OK);
				break;
			}
			else
			if ( wait.is_timeout() ) {
				nexifAlarmSet(NEXALID_VacuumOnTimeouts);
				m_lfs << arrange << datetime << "EV_VAC_ABORTED" << std::endl;
				Event(EV_VAC_ABORTED);
				break;
			}
			else
			if ( is_cancel() ) {
				m_lfs << arrange << datetime << "EV_CANCEL" << std::endl;
				Event(EV_CANCEL);
				break;
			}
			basic_timer::do_events(1L);
		}
	}
	else {
		m_lfs << arrange << datetime << "EV_VAC_OK" << std::endl;
		Event(EV_VAC_OK);
	}
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_WAIT_VAC_OFF(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_WAIT_VAC_OFF" << std::endl;

	if ( is_cancel() ) {
		m_lfs << arrange << datetime << "EV_CANCEL" << std::endl;
		Event(EV_CANCEL);
		return;
	}

	basic_timer wait(m_params.MotionTimeouts.VacuumOff);
	while (1) {
		if ( nexioIsVacuumOff() ) {
			m_lfs << arrange << datetime << "EV_VAC_OK" << std::endl;
			Event(EV_VAC_OK);
			break;
		}
		else
		if ( wait.is_timeout() ) {
			nexifAlarmSet(NEXALID_VacuumOffTimeouts);
			m_lfs << arrange << datetime << "EV_VAC_ABORTED" << std::endl;
			Event(EV_VAC_ABORTED);
			break;
		}
		else
		if ( is_cancel() ) {
			m_lfs << arrange << datetime << "EV_CANCEL" << std::endl;
			Event(EV_CANCEL);
			break;
		}
		basic_timer::do_events(1L);
	}
}

#ifdef __NEXPROC_NO_PIN_UP_CHECK
// ---------------------------------------------------------------------------
void cTypeA::enter_ST_PARSE_NEXT(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_PARSE_NEXT" << std::endl;

	switch ( client.event ) {
	case nexReturnToOrigin:
		break;
	case nexLoad:
	case nexMoveToUpper:
	case nexMoveToAlignment:
		if ( nexioIsPinDownPos() || !nexioIsVacuumOff() ) {
			m_lfs << arrange << datetime << "EV_NEXT" << std::endl;
			Event(EV_NEXT);
		}
		else {
			m_lfs << arrange << datetime << "EV_SKIP" << std::endl;
			Event(EV_SKIP);
		}
		break;
	case nexUnload:
		break;
	case nexMoveToLower:
		break;
	case nexOpenWorkGuide:
		break;
	case nexCloseWorkGuide:
		break;
	case nexInitialize:
	default:
		break;
	}
}
#endif

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_ERROR(const cEventParams* const evp)
{
	m_lfs << arrange << datetime << "enter_ST_ERROR" << std::endl;

	m_lfs << arrange << datetime << "EV_DONE" << std::endl;
	Event(EV_DONE);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_CANCEL(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_CANCEL" << std::endl;

	m_lfs << arrange << datetime << "EV_DONE" << std::endl;
	Event(EV_DONE);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_DONE(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_DONE" << std::endl;

	client.result = TRUE;

	m_lfs << arrange << datetime << "EV_DONE" << std::endl;
	Event(EV_DONE);
}

// ---------------------------------------------------------------------------
void cTypeA::enter_ST_ABORT(const cEventParams* const /*params*/)
{
	m_lfs << arrange << datetime << "enter_ST_ABORT" << std::endl;

	m_lfs << arrange << datetime << "EV_DONE" << std::endl;
	Event(EV_DONE);
}
