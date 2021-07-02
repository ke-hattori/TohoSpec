#pragma once

#include "Resource.h"
#include "MailThread.h"

class CPifMessageBehavior
{
public:
	static void AddRequMessage(LPCTSTR psz)
	{
		CMailThread::AddSendMessage(psz);
	}

	static void AddRespMessage(LPCTSTR psz)
	{
		CMailThread::AddSendMessage(psz);
	}
};
