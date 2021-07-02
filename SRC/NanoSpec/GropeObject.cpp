// GropeObject.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "GropeObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////  CGropeObject Class ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// This CGropeObject class defines how the groping is done surrounding an
// area by returning the amount and the direction of the next movement in
// both x and y directions. The unit of the amount returned is in terms of
// one half-screen size.
//
// Groping is done one layer at a time, from inner layer to outer layer
// First do 1st layer, site number 1 to 8; then 2nd layer, site number 1 to 16;
// and so forth..., until you reach the last layer. Always call "Finished()"
// to find out if the groping is done before the call to "GetNextMove()".
//
// Within each layer, the groping is done clockwise from 1 to n as indicated
// in the following graph. The directions and the xy coordinate used in the
// class is also shown below.
//
//
//                  01--2--3--4--5                                           y
//                   \           |                  UP                      /|\
//                  16\ 1--2--3  6                  /|\                      |
//                   | \ \    |  |                   |                       |
//                  15  8 \   4  7                   |                       |
//                   |  |     |  |       LEFT <------|-----> RIGHT           |---------->x
//                  14  7--6--5  8                   |
//                   |           |                   |
//                  13--12-11-10-9                  \|/
//                                                 DOWN
//
//



// class constructor
CGropeObject::CGropeObject()
{
	// init members such that Finish() will return FALSE
	m_NumLayers = 0;
	m_CurrLayer = 1;
}

CGropeObject::CGropeObject(WORD NumLayers)
{
	// set the number of layers
	SetNumLayers(NumLayers);
}


// Set the number of layers we want to grope on. Should be called
// before each grope to initialize the whole class. It is also assumed
// that the current stage position is at the center of the grope area.
void CGropeObject::SetNumLayers(WORD NumLayers)
{
	if( NumLayers <= 0 )			// illegal
	{
		m_NumLayers = 0;
		m_CurrLayer = 1;
		return;
	}

	m_NumLayers 	= NumLayers;	// init number of layers

	// init data members such that the first move is to move to the
	// first site of the first layer.
	m_CurrDirection = UP_LEFT;
	m_CurrLayer 	= 1;
	m_NextSite		= 1;
}

// Get the direction and amount (in terms of half of the overlay view)
// of the next move in both x and y directions.
BOOL CGropeObject::GetNextMove(short* dx, short* dy)
{
	if( Finished() )
	{	// all the moves have been finished, no more move
		*dx = *dy = 0;
		return FALSE;
	}

	// return the amount and direction of the next move
	// base on the current moving direction
	switch (m_CurrDirection)
	{
		case UP_LEFT:
			*dx = -1;  *dy = m_CurrLayer == 1 ? 1 : 2;

			// there is only one move in this direction in each layer
			// after this move, chang to the next direction
			m_CurrDirection++;
			m_NextSite = 0;
			break;

		case RIGHT:
			*dx = 1;   *dy = 0;
			break;

		case DOWN:
			*dx = 0;   *dy = -1;
			break;

		case LEFT:
			*dx = -1;  *dy = 0;
			break;

		case UP:
			*dx = 0;   *dy = 1;
			break;
	}

	// now update the data members to define the movement after this
	// next movement so the next call to this function will get that.
	if( m_CurrDirection == UP )
	{
		// this is the direction that's going to finish this current layer
		// first check if the current site is the last site in this layer
		if( m_NextSite < 2 * m_CurrLayer - 1 )
		{
			// haven't finished all the sites yet, go to next site
			m_NextSite++;
		}
		else
		{
			// finished all the sites in this layer, go to next layer
			// init the corresponding data members
			m_CurrLayer++;
			m_CurrDirection = UP_LEFT;
			m_NextSite		= 0;
		}
	}
	else
	{
		// just need to check if all the sites in the current direction is
		// finished, if not, just increase the site number; if yes, go to
		// the next direction, and init the site number to 1.
		if( m_NextSite < 2 * m_CurrLayer )
		{
			m_NextSite++;
		}
		else
		{
			m_CurrDirection++;
			m_NextSite = 1;
		}
	}

	return TRUE;
}

