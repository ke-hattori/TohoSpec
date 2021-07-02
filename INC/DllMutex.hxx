/*
 *	DLL の多重起動防止用関数群
 */
HANDLE lg_hDllMutex_Run = 0;
//#define	DLLMUTEX_DEB
BOOL DllMutexCreate(LPCTSTR pDllName)
{
	BOOL l_bRc = TRUE;

	/* 呼出し側プロセスのフルパスを取得	*/
	TCHAR l_tszProcPathName[MAX_PATH];
	::GetModuleFileName(::GetModuleHandle(0), l_tszProcPathName, (sizeof(l_tszProcPathName) / sizeof(TCHAR)) - 1);
	/* CreateMutex() の lpName に'\'が存在するとエラーになるのでファイル名のみを抽出	*/
	TCHAR *l_ptszProcFileName = _tcsrchr(l_tszProcPathName, _T('\\')) + 1;
	/* この dll の特定の名称を追加	*/
	TCHAR l_tszMutexName[MAX_PATH] = _T("");
	if (0 != pDllName) {
		_tcscpy(l_tszMutexName, pDllName);
	}
	_tcscat(l_tszMutexName, _T(":"));
	_tcscat(l_tszMutexName, l_ptszProcFileName);
	HANDLE l_hDllMutex_Run = ::CreateMutex(0, TRUE, l_tszMutexName);
	if (0 == l_hDllMutex_Run) {
#ifdef DLLMUTEX_DEB
		_tcscat(l_tszMutexName, _T("\n0 == lg_hDllMutex_Run"));
		::MessageBox(0, l_tszMutexName, _T("DllMutexCreate()"), MB_OK | MB_SYSTEMMODAL);
#endif
		l_bRc = FALSE;
	}
	if (::GetLastError() == ERROR_ALREADY_EXISTS) {
#ifdef DLLMUTEX_DEB
		_tcscat(l_tszMutexName, _T("\nERROR_ALREADY_EXISTS"));
		::MessageBox(0, l_tszMutexName, _T("DllMutexCreate()"), MB_OK | MB_SYSTEMMODAL);
#endif
		l_bRc = FALSE;
	}
	if (FALSE != l_bRc) {
		lg_hDllMutex_Run = l_hDllMutex_Run;
#ifdef DLLMUTEX_DEB
		_tcscat(l_tszMutexName, _T("\nOK."));
		::MessageBox(0, l_tszMutexName, _T("DllMutexCreate()"), MB_OK | MB_SYSTEMMODAL);
#endif
	}

	return l_bRc;
}
BOOL DllMutexRelease()
{
	BOOL l_bRc = TRUE;

	if (0 != lg_hDllMutex_Run) {
		if (FALSE == ::ReleaseMutex(lg_hDllMutex_Run)) {
			l_bRc = FALSE;
		}
		if (FALSE == ::CloseHandle(lg_hDllMutex_Run)) {
			l_bRc = FALSE;
		}
	}

	return l_bRc;
}
