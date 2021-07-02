#if !defined( _NEXIOTYPED_H_ )
#define _NEXIOTYPED_H_

#pragma once

using namespace nano;

class cDioTypeD : public cDioTypeC
{
public:
	cDioTypeD () {}

	// NANOSPEC DI INTERFACE
	virtual BOOL IsSafetyPlug (void) { return TRUE; }
	virtual BOOL IsShutterOpen (void) { return FALSE; }
	virtual BOOL IsShutterClose (void) { return TRUE; }
	virtual BOOL IsStageLoadPos (void) { return m_inp->LoadPosX && m_inp->LoadPosY; }

	virtual BOOL IsPinUpperPos (void)
	{
		return FALSE;
	}
	virtual BOOL IsPinAlignmentPos (void)
	{
		return FALSE;
	}
	virtual BOOL IsPinDownPos (void)
	{
		return TRUE;
	}
	virtual BOOL IsWorkGuideOpen (void)
	{
		return TRUE;
	}
	virtual BOOL IsWorkGuideClose (void)
	{
		return FALSE;
	}
	virtual BOOL IsRobotArmDetect			 (void) { return TRUE; }

	// NEXTRA DI INTERFACE
	virtual BOOL IsWorkGuideOpenEx	(unsigned short num) { return TRUE; }
	virtual BOOL IsWorkGuideCloseEx (unsigned short num) { return FALSE; }

};

#endif // _NEXIOTYPED_H_
