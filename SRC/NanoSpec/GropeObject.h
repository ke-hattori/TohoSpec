// GropeObject.h : ヘッダー ファイル
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// define the groping directions in turn
const WORD UP_LEFT = 0;
const WORD RIGHT   = 1;
const WORD DOWN    = 2;
const WORD LEFT    = 3;
const WORD UP	   = 4;

class CGropeObject
{
// define the groping directions in turn

// private data members
private:
	WORD  m_NumLayers;		 // total number of layers to grope
	WORD  m_CurrDirection;	 // current moving direction
	WORD  m_NextSite;		 // the next site number to move to (in current direction)
	WORD  m_CurrLayer;		 // the current groping layer

// public interfaces
public:
	 CGropeObject ();	// constructor
	 CGropeObject (WORD NumLayers); 	// constructor

	 void SetNumLayers(WORD NumLayers);
	 BOOL GetNextMove(short* dx, short* dy);

	 // Check when to stop the move by calling this function. The move is
	 // done when we are already at a layer higher than what is required.
	 BOOL Finished()
	 {	return m_CurrLayer > m_NumLayers ? TRUE : FALSE; }

	 UINT GetCurrLayer()
	 {	return m_CurrLayer; }
};
