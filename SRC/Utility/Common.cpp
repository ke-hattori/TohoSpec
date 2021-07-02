#define WIN32_LEAN_AND_MEAN

//===========================================================================
// Common.cpp (Common.hxx)
//===========================================================================
#include <common.hxx>

// =========================================================================
// NameÅF DoEvents
void DoEvents(void)
{
	MSG msg;

	// dispatch window messages for our client
	while ( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
// =========================================================================
// NameÅF Delay
void Delay(DWORD dwDuration)
{
	DWORD dwStart, dwNow, dwElapsed;

	// wait until finished
	dwStart   = ::GetTickCount();
	dwElapsed = 0L;
	while ( dwElapsed < dwDuration ) {
		DoEvents();

		dwNow = ::GetTickCount();
		if ( dwNow < dwStart )
			dwElapsed += dwNow;
		else
			dwElapsed += (dwNow - dwStart);
		dwStart = dwNow;
	}
}
// =========================================================================
// NameÅF WaitWithMessageLoop
BOOL WaitWithMessageLoop(HANDLE hEvent, DWORD dwTimeout)
{
	DWORD dwRet;

	DWORD dwMaxTick = GetTickCount() + dwTimeout;

	while ( TRUE ) {
		// wait for event or message, if it's a message, process it and return to waiting state
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwMaxTick - GetTickCount(), QS_ALLINPUT);
		if ( dwRet == WAIT_OBJECT_0 ) {
			return TRUE;
		}
		else if ( dwRet == WAIT_OBJECT_0 + 1 ) {
			DoEvents();
		}
		else {
			return FALSE;
		}
	}
}
