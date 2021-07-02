// eventpar.h: Headers for all the classes defines the event parameters
// for the finite state machine ( the cActive class ).

#ifndef EVENTPAR_H
#define EVENTPAR_H

#include <Active.hxx>	// finite state machine

//-----------------------------------------------------------------------------
//
// For use when passing parameters in Event()
//
class cStatusEventParams : public cEventParams
{
	public:
		LRESULT Status; // completion status

		cStatusEventParams (LRESULT InStatus)	// constructor
		{
			Status = InStatus;
		};
};

//-----------------------------------------------------------------------------
//
// For use when passing parameters in Event()
//
class cPointEventParams : public cEventParams
{
	public:
		POINT Point;	// a point as event parameter

		cPointEventParams (POINT InPoint)	// constructor
		{
			Point = InPoint;
		};
};

#endif

