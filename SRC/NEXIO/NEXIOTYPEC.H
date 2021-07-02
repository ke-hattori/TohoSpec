#if !defined( _NEXIOTYPEC_H_ )
#define _NEXIOTYPEC_H_

#pragma once

using namespace nano;

class cDioTypeC : public cDioTypeB
{
public:
	cDioTypeC () {}

	// NANOSPEC DI INTERFACE
	virtual BOOL IsSafetyPlug (void) { return TRUE; }
	virtual BOOL IsShutterOpen (void) { return m_inp->ShutterOpen && m_inp->ShutterOpenCylinder; }
	virtual BOOL IsShutterClose (void) { return m_inp->ShutterCloseCylinder; }
	virtual BOOL IsStageLoadPos (void) { return m_inp->LoadPosX; }

	virtual BOOL IsPinUpperPos (void)
	{
		return m_inp->LifterCylinder1UpperPos && m_inp->LifterCylinder2UpperPos && m_inp->LifterCylinder3UpperPos && m_inp->LifterCylinder4UpperPos;
	}
	virtual BOOL IsPinAlignmentPos (void)
	{
		return m_inp->LifterCylinder3UpperPos && m_inp->LifterCylinder4UpperPos && m_inp->LifterCylinder1LowerPos && m_inp->LifterCylinder2LowerPos;
	}
	virtual BOOL IsPinDownPos (void)
	{
		return m_inp->LifterCylinder1LowerPos && m_inp->LifterCylinder2LowerPos && m_inp->LifterCylinder3LowerPos && m_inp->LifterCylinder4LowerPos;
	}

	virtual void PinAlignmentEx (unsigned short num, BOOL bOn) { m_out->LifterCylinder1Down   = m_out->LifterCylinder2Up = bOn; }
	virtual void ShutterOpenEx (BOOL bOn) { m_out->ShutterOpen = bOn; }
	virtual void ShutterCloseEx (BOOL bOn) { m_out->ShutterClose = bOn; }

};

#endif // _NEXIOTYPEC_H_
