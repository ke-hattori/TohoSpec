#pragma once

#include "PifSockSocket.h"

class CPifMessageBehavior
{
public:
	static void AddRequMessage(LPCTSTR psz)
	{
		CPifSockSocket::AddRequMessage(psz);
	}

	static void AddRespMessage(LPCTSTR psz)
	{
		CPifSockSocket::AddRespMessage(psz);
	}
};
