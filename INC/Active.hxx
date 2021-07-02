/****************************************************************
 *		Copyright (C) 2003 by Tencor Instruments
 *
 *		Source Upgrade From VC1.52 to VC6.0 
 *		
 *		Updated by Chen-zm
 *		
 *		Date 2003/02/28
 ****************************************************************/

#ifndef ACTIVE_HXX
#define ACTIVE_HXX

// ***************************************************************************
// ***************************************************************************
// This file describes the following architectural classes, based on 
// Schlaer/Mellor OOA/OOD:
// - Transition
// - Active (Active Instance Superclass)
//
// The FSM assumes that all unspecified transitions mean event ignored.
// ***************************************************************************
// ***************************************************************************



// ===========================================================================
// magic number to terminate the transition tables
// ===========================================================================

const int EVENT_IGNORED 			= -1;
#define   END_OF_TRANSITION_TABLE	{EVENT_IGNORED,EVENT_IGNORED,EVENT_IGNORED}



// ===========================================================================
// Transition: one entry in a transition table
// ===========================================================================
// 
// NOTE: Transition tables need to look as follows:
//		 const Transition MyTransTable[] =
//		 {
//			 //  OldState,			 EventId,			 NewState
//			 {	 ST_IDLE,			 EV_DO_INIT,		 ST_INIT			},
//			 ...
//	  
//			 // terminate the transition table
//			 END_OF_TRANSITION_TABLE
//		 };

class Transition
{
	public:

		// triplet specifying legal transitions (public for simple init)
		int 		OldState;
		int 		EventId;
		int 		NewState;
		
		// returns false if this is the last element in the transition table
		// (see also END_OF_TABLE macro above)
		int IsNotEndOfTable (void) const;
		
		// returns true if this transition matches the current state and the
		// current event
		int IsMatch (const int curr_state, const int event) const;
		
		// returns the new state for this transition
		int GetNewState (void) const;
};


// ===========================================================================
// Base Class for all event parameters for active classes
// ===========================================================================

class cEventParams
{
};



// ===========================================================================
// Base Class for all active classes (classes with a non-trivial life cycle)
// ===========================================================================

typedef struct {
	int		OldState;
	int		EventID;
} EVENT_INFO, *LPEVENT_INFO;

class cActive
{
	// NOTE: No objects of type Active can be created. It is used
	//		 exclusively to derive classes.
	
	private:
		
		// pointer to private transition table
		const Transition* const TransitionTable;
		
		
	protected:
	
		// current state data member
		int 	CurrState;
		// event information
		EVENT_INFO	EventInf;

		// constructs the base class, needs a pointer to the first element in
		// the transition table and the initial state of the FSM
		cActive
		(
			const Transition* const 	transition_table,
			const int					initial_state		= 0
		);
		
		// generic do nothing state processing (no arguments)
		// virtual function: can not be inlined
		virtual void EnterState (const cEventParams* const params);
		
		
	public:
	
		// processes an event and call the virtual state processing function
		void Event
		(
			const int					event,
			const cEventParams* const	params		= 0
		);
};




// ===========================================================================
// State Processing Map helpers macros
// ===========================================================================
//
// NOTE 1: The virtual function EnterState can be used to do all state related
//		   processing. Use a switch(CurrState) to selected what state you are
//		   processing.
//
// NOTE 2: The following macros simplify the state processing dispatch in the
//		   same way the MFC message maps simplify the standard switch
//		   statements in the WINPROC function.
//
// NOTE 3: If you are using these macros, do not declare or implement the
//		   virtual function EnterState. The macros will do it.
//
// NOTE 4: If your cActive derived class is also derived from a second class
//		   (multiple inheritance), then warning C4204 is produced erroneously:
//		   "nonstandard extension used : non-constant aggregate initializer"
//		   Solutions:
//		   1) Do NOT use multiple inheritance and these macros
//		   2) insert "#pragma warning(disable: 4204)" before 
//			  BEGIN_STATEPROC_MAP(...) and
//			  insert "#pragma warning(default: 4204)" after
//			  END_STATEPROC_MAP(...)
//
// USEAGE:
//
//		xxx.hpp:
//			class Y: public cActive
//			{
//				... // standard class stuff
//
//				private:
//					// state processing functions
//					void EnterStateIdle (const cEventParams* const params);
//					void EnterStateStart (const cEventParams* const params);
//					void EnterStateNext (const cEventParams* const params);
//					...
//				
//				DECLARE_STATEPROC_MAP(Y)
//			};
//
//		xxx.cpp:
//			// Y is the class name
//			// pragma only required for Y, if Y is multiple inheritance
//			#pragma warning(disable: 4204) // see NOTE 4 above
//			BEGIN_STATEPROC_MAP(Y)
//				ENTER_STATE(ST_IDLE, EnterStateIdle),
//				ENTER_STATE(ST_START, EnterStateStart),
//				ENTER_STATE(ST_WAIT, NULL), // state without state processing
//				ENTER_STATE(ST_NEXT, EnterStateNext),
//				...
//			END_STATEPROC_MAP(Y)
//			// pragma only required for Y, if Y is multiple inheritance
//			#pragma warning(default: 4204) // see NOTE 4 above
//
//			// Constructor
//			Y::Y : cActive(MyTransTable, ST_IDLE)
//			{
//				// check the consistency of the STATEPROC_MAP (Debug version)
//				ASSERT(CheckStateProcMap());
//
//				// do specific init
//			}

// ----------------------------------------------------------------------------
// Macro DECLARE_STATEPROC_MAP
// ----------------------------------------------------------------------------
#define DECLARE_STATEPROC_MAP(class_name) \
private: \
	typedef void (class_name::*P_STATE_PROC)(const cEventParams* const); \
	class STATE_PROC_DEF \
	{ \
	public: \
		int 			state; \
		char*			state_descr; \
		P_STATE_PROC	proc; \
	}; \
	static STATE_PROC_DEF	StateProcMap[]; \
	static int				nStateProcMap; \
protected: \
	virtual void EnterState (const cEventParams* const params); \
	int CheckStateProcMap(void);

// ----------------------------------------------------------------------------
// Macro BEGIN_STATEPROC_MAP (debug and release versions)
// ----------------------------------------------------------------------------
#ifdef _DEBUG
#define BEGIN_STATEPROC_MAP(class_name) \
void class_name::EnterState (const cEventParams* const params) \
{ \
	if (CurrState >= 0 && CurrState < nStateProcMap) \
	{ \
		if (StateProcMap[CurrState].state_descr != NULL) \
		{ \
			OutputDebugString(#class_name); \
			OutputDebugString(StateProcMap[CurrState].state_descr); \
		} \
		if (StateProcMap[CurrState].proc != NULL) \
			(this->*(StateProcMap[CurrState].proc))(params); \
	} \
} \
class_name::STATE_PROC_DEF class_name::StateProcMap[] = \
{
#else
#define BEGIN_STATEPROC_MAP(class_name) \
void class_name::EnterState (const cEventParams* const params) \
{ \
	if (CurrState >= 0 && CurrState < nStateProcMap) \
	{ \
		if (StateProcMap[CurrState].proc != NULL) \
			(this->*(StateProcMap[CurrState].proc))(params); \
	} \
} \
class_name::STATE_PROC_DEF class_name::StateProcMap[] = \
{
#endif

// ----------------------------------------------------------------------------
// Macro ENTER_STATE
// ----------------------------------------------------------------------------
#define ENTER_STATE(state, proc) { state, "::" #state ": " #proc "\n", proc }

// ----------------------------------------------------------------------------
// Macro END_STATEPROC_MAP
// ----------------------------------------------------------------------------
#define END_STATEPROC_MAP(class_name) \
}; \
int class_name::nStateProcMap = sizeof(StateProcMap) / sizeof(StateProcMap[0]); \
int class_name::CheckStateProcMap(void) \
{ \
	for (int i = 0; i < nStateProcMap; ++i) \
	{ \
		if (i != StateProcMap[i].state) \
			return 0; \
	} \
	return 1; \
}


#endif
