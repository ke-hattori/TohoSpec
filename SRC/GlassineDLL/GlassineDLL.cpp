// GlassineDLL.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//
#define _WIN32_WINNT 0x0400
#include "stdafx.h"
#include "GlassineDLL.h"

#pragma data_seg( ".KeyHookData" )
HHOOK	KeyLock::g_hHook = NULL;
#pragma data_seg()

#define HKCU HKEY_CURRENT_USER
#define LLKHF_ALTDOWN		 0x00000020
#define LLKHF_UP			 0x00000080

LPCTSTR KEY_DisableTaskMgr =
   "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
LPCTSTR VAL_DisableTaskMgr = "DisableTaskMgr";

HINSTANCE g_hDllInst = NULL;

LRESULT LowLevelKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	static BOOL fShiftPressed = FALSE;

	BOOL fHandled = FALSE;

	if (nCode == HC_ACTION)
	{
		KBDLLHOOKSTRUCT *pkbdllhook = (KBDLLHOOKSTRUCT *)lParam;

		switch (wParam)
		{
			case WM_SYSKEYDOWN:
				switch (pkbdllhook->vkCode)
				{
					case VK_LSHIFT:
					case VK_RSHIFT:				//Shift キー
					{
						fHandled = TRUE;
						break;
					}
					case VK_TAB:				//Alt + Tab キー
					{
						if (pkbdllhook->flags & LLKHF_ALTDOWN)
						{
							fHandled = TRUE;
						}
						break;
					}
					case VK_ESCAPE:				//Esc キー
					{
						if (pkbdllhook->flags & LLKHF_ALTDOWN)
						{
							fHandled = TRUE;
						}
						break;
					}
					case VK_F4:					//Alt+F4 キー
					{
						if (pkbdllhook->flags & LLKHF_ALTDOWN)
						{
							fHandled = TRUE;
						}
						break;
					}
//					  case VK_F6:					//Alt+F6でLock終了
//					  {
//						HWND hWndLock = ::FindWindow(NULL, "InterLock");
//						if(hWndLock)
//							SendMessage(hWndLock,WM_CLOSE,NULL,NULL);
//						else
//						{
//							KeyLock TT;
//							TT.Release();
//						}
//						  break;
//					  }
					case VK_DELETE:
					{
						fHandled = TRUE;
					}
						break;
				}

				break;

			case WM_KEYUP:
			case WM_KEYDOWN:
					fHandled = TRUE;
					break;
			case WM_SYSKEYUP:
				switch (pkbdllhook->vkCode)
				{
					case VK_LMENU:
					case VK_RMENU:
					{
						break;
					}
					case VK_LSHIFT:
					case VK_RSHIFT:
					{
						fShiftPressed = FALSE;
						break;
					}
				}

				break;
		}
	}

	return (fHandled ? TRUE : CallNextHookEx(KeyLock::g_hHook, nCode, wParam, lParam));
}


extern "C"
BOOL APIENTRY DllMain( HINSTANCE hInstance, DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	g_hDllInst = hInstance;
	return TRUE;
}


BOOL KeyLock::Set()
{
	//::MessageBox(NULL, "Set-In", "GlassineDLL", MB_ICONWARNING);
	g_hHook = ::SetWindowsHookEx(13,(HOOKPROC)LowLevelKeyboardProc,g_hDllInst,0);

	KeyLock::Disable(ALL,TRUE,FALSE);
	if(g_hHook == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL KeyLock::Release()
{
	BOOL bRes = TRUE;

	if(g_hHook != NULL)
	{
		bRes = ::UnhookWindowsHookEx( g_hHook );
		g_hHook = NULL;
		//::MessageBox(NULL, "Release", "GlassineDLL", MB_ICONWARNING);
	}

	//Taskbarを有効に戻す
	KeyLock::Disable(ALL,FALSE,FALSE);
	return bRes;
}

void KeyLock::Disable(DWORD dwFlags, BOOL bDisable, BOOL bBeep)
{

   // (Ctrl+Alt+Del)
   if (dwFlags & TASKMGR)
   {
	  HKEY hk;
	  if (RegOpenKey(HKCU, KEY_DisableTaskMgr,&hk)!=ERROR_SUCCESS)
		 RegCreateKey(HKCU, KEY_DisableTaskMgr, &hk);

	  if (bDisable)
	  { //disable TaskManager： set policy = 1
		 DWORD val=1;
		 RegSetValueEx(hk, VAL_DisableTaskMgr, NULL,
			REG_DWORD, (BYTE*)&val, sizeof(val));
	  }
	  else
		{ //enable TaskManager
		 RegDeleteValue(hk,VAL_DisableTaskMgr);
		}
	  }

   if (dwFlags & TASKBAR)
   {
	  HWND hwnd = FindWindow("Shell_traywnd", NULL);
	  EnableWindow(hwnd, !bDisable);
   }

}