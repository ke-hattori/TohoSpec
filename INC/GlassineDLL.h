// GlassineDLL.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

typedef struct tagKBDLLHOOKSTRUCT {
	DWORD vkCode;
	DWORD scanCode;
	DWORD flags;
	DWORD time;
	DWORD dwExtraInfo;
}KBDLLHOOKSTRUCT, FAR *LPKBDLLHOOK ;

class __declspec(dllexport) KeyLock	// KeyHookクラスを宣言しエクスポート
{

public:

	static HHOOK g_hHook;

	BOOL Set();
	BOOL Release();

////////////////////////////////////////////////////////////
   enum
   {
	  TASKMGR  = 0x01,	//disable (Ctrl+Alt+Del)
	  TASKKEYS = 0x02,	//disable (Alt-TAB, etc)
	  TASKBAR  = 0x04,	//disable Taskbar
	  ALL=0xFFFF		//disable all
   };

   static void Disable(DWORD dwItem,BOOL bDisable,BOOL bBeep=FALSE);

   static BOOL IsTaskMgrDisabled();
   static BOOL IsTaskBarDisabled();
   static BOOL AreTaskKeysDisabled()
   {
	return AreTaskKeysDisabled();
   }
////////////////////////////////////////////////////////////

};

