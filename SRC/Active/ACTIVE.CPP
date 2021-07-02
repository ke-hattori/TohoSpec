/****************************************************************
 *		Copyright (C) 2003 by Tencor Instruments
 *
 *		Source Upgrade From VC1.52 to VC6.0
 *
 *		Updated by Chen-zm
 *
 *		Date 2003/02/28
 ****************************************************************/

// ===========================================================================
// Disable unnecessary warning messages
// ===========================================================================

// produced by inlined functions not used in a module
//     C4505: 'function_name': unreferenced local function has been removed
#pragma warning(disable: 4505)

// produced by /Ob2 option
//     C4711: function 'function_name' selected for automatic inline expansion
#pragma warning(disable: 4711)

// produced by "const Transition* const TransitionTable;"
//     C4512: 'function_name': assignment operator could not be generated
#pragma warning(disable: 4512)



// ============================================================================
// Include files
// ============================================================================

#include "active.hxx"

// ============================================================================
// Transition class implementation
// ============================================================================

// ----------------------------------------------------------------------------
// returns false if this is the last element in the transition table
// ----------------------------------------------------------------------------
int Transition::IsNotEndOfTable (void) const
{
	return OldState != EVENT_IGNORED;
}

// ----------------------------------------------------------------------------
// returns true if this transition matches the current state and the
// current event
// ----------------------------------------------------------------------------
int Transition::IsMatch (const int curr_state, const int event) const
{
	return curr_state == OldState && event == EventId;
}

// ----------------------------------------------------------------------------
// returns the new state for this transition
// ----------------------------------------------------------------------------
int Transition::GetNewState (void) const
{
	return NewState;
}



// ============================================================================
// cEventParams class implementation
// ============================================================================



// ============================================================================
// cActive class implementation
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
cActive::cActive
(
	const Transition* const transition_table,
	const int				initial_state		/*= 0*/
)
:	TransitionTable(transition_table),
	CurrState(initial_state)
{
}

// ----------------------------------------------------------------------------
// processes an event and call the virtual state processing function
// ----------------------------------------------------------------------------
void cActive::Event(const int event, const cEventParams* const params)
{
	const Transition* trans = &TransitionTable[0];

	while (trans->IsNotEndOfTable())
	{
		if (trans->IsMatch(CurrState, event))
		{
			EventInf.OldState = CurrState;
			EventInf.EventID = event;
			// set the new state
			CurrState = trans->GetNewState();

			// do virtual function call
			EnterState(params);
			break;
		}
		++trans;
	}
}

// ----------------------------------------------------------------------------
// generic do nothing state processing (no arguments)
// virtual function: can not be inlined
// ----------------------------------------------------------------------------
void cActive::EnterState (const cEventParams* const /*params*/)
{
	// empty (executed by generic do nothing states)
}
