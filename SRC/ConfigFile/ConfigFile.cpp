// ConfigFile.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//
#include "stdafx.h"
#include <windows.h>
#include <globals.hxx>
#include <configfile.hxx>
#include "inifile.h"
/* added 2009.07.07 hmenjo dll 相対パス対応 ConfigFile.dll ---------- { ---------- */
#include <stdlib.h>
#include <tchar.h>
#include <stdio.h>
/* added 2009.07.07 hmenjo dll 相対パス対応 ConfigFile.dll ---------- } ---------- */

// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#define _MASTER_
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

/* added 2009.07.07 hmenjo dll 相対パス対応 ConfigFile.dll ---------- { ---------- */
TCHAR g_tszProcDir[_MAX_PATH] = _T("");		/* 呼出しプロセスのディレクトリ('\'付き)*/
TCHAR g_tszBaseDir[_MAX_PATH] = _T("");		/* 基準ディレクトリ('\'付き)*/
void GetProcBaseDir(
		LPTSTR ptszProcDir,
		LPTSTR ptszBaseDir
	)
{
	if (0 == _tcscmp(ptszProcDir, _T(""))) {
		TCHAR l_tszProcessFName[_MAX_PATH];	/* 呼出しプロセスのフルパス*/
		::GetModuleFileName(0, l_tszProcessFName, sizeof(l_tszProcessFName));
		TCHAR l_tszDrive[_MAX_DRIVE];
		TCHAR l_tszDir[_MAX_DIR];
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
		TCHAR l_tszFilename[_MAX_FNAME];
		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, l_tszFilename, 0);

		g_lAppNameType = APP_NAME_NANO; // default application name
		for(int i = 0; i < APP_NAME_MAX; i++){
			if(_tcsncmp(l_tszFilename, g_lpszAppPrefix4[i], 4) == 0){
				g_lAppNameType = i;
				break;
			}
		}
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		_stprintf(ptszProcDir, _T("%s%s"), l_tszDrive, l_tszDir);
		if (0 != _tcslen(ptszProcDir)) {
			if (_T('\\') != ptszProcDir[_tcslen(ptszProcDir) - 1]) {
				_tcscat(ptszProcDir, _T("\\"));
			}
		}
	}
	if ((0 != _tcscmp(ptszProcDir, _T("")))
	 && (0 == _tcscmp(ptszBaseDir, _T("")))) {
		TCHAR l_tszProcDirUp[_MAX_PATH];
		_stprintf(l_tszProcDirUp, _T("%s.."), ptszProcDir);
		_tfullpath(ptszBaseDir, l_tszProcDirUp, sizeof(g_tszBaseDir));
		if ((0 != _tcslen(ptszBaseDir)) && (_T('\\') != ptszBaseDir[_tcslen(ptszBaseDir) - 1])) {
			_tcscat(ptszBaseDir, _T("\\"));
		}
	}
}
/* added 2009.07.07 hmenjo dll 相対パス対応 ConfigFile.dll ---------- } ---------- */
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
	TCHAR l_tszIniPath[_MAX_PATH];
	char szFilename[_MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("ConfigFile"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		// Matsuhisa 2009.05.30 ----->
		ConfigFile_LoadAllNanoSpecIni();
		// Matsuhisa 2009.05.30 <-----
/* added 2009.07.07 hmenjo dll 相対パス対応 ConfigFile.dll ---------- { ---------- */
		GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
/* added 2009.07.07 hmenjo dll 相対パス対応 ConfigFile.dll ---------- } ---------- */
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
		strcpy(szFilename, NANOSPEC_INIFILENAME);
		if(g_lAppNameType != APP_NAME_NANO){
			char* p;
			char szRemainChars[_MAX_PATH];
			p = strstr(szFilename, g_lpszAppPrefix4[APP_NAME_NANO]);
			if(p != NULL){
				strcpy(szRemainChars, p + strlen(g_lpszAppPrefix4[APP_NAME_NANO])); // copy chars part of after replace position.

				*p = '\0';
				strcat(szFilename, g_lpszAppPrefix4[g_lAppNameType]);  // concate replace chars.
				strcat(szFilename, szRemainChars); // concate copy chars part of after replace position.
			}
		}
		_stprintf(l_tszIniPath, _T("%s%s%s"), g_tszProcDir, CFG_DIR, szFilename);
		g_lModelType = ::GetPrivateProfileInt(INISECTION_MODELTYPE, INIKEY_MODELTYPE_TYPE, MODEL_M6500, l_tszIniPath);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL 多重起動防止 ---------- } ---------- */
		break;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_LoadNanoSpecIni
// Purpose    : Iniファイルの内容の読み込み
// Parameters : iIniFile ---> どのIniファイルなのかを決める変数
//
// Returns    : なし
void CONFAPI ConfigFile_LoadNanoSpecIni(int iIniFile)
{
	LoadNanoSpecIni(iIniFile);
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_SaveNanoSpecIni
// Purpose    : Iniファイルの保存
// Parameters : iIniFile ---> どのIniファイルなのかを決める変数
//
// Returns    : なし
void CONFAPI ConfigFile_SaveNanoSpecIni(int iIniFile)
{
	SaveNanoSpecIni(iIniFile);
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_LoadAllNanoSpecIni
// Purpose    : 全てのIniファイルの内容の読み込み
// Parameters : なし
//
// Returns    : なし
void CONFAPI ConfigFile_LoadAllNanoSpecIni()
{
	LoadAllNanoSpecIni();
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_SaveAllNanoSpecIni
// Purpose    : 全てのIniファイルの保存
// Parameters : なし
//
// Returns    : なし
void CONFAPI ConfigFile_SaveAllNanoSpecIni()
{
	SaveAllNanoSpecIni();
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_GetNanoSpecIni
// Purpose    : Iniファイルの設定の読み込み(メモリ)
// Parameters : pVoid ---> データを読み込む構造体等のポインタ
//              iType ---> どのデータを読み込むかを決める変数
//
// Returns    : なし
void CONFAPI ConfigFile_GetNanoSpecIni(void* pVoid, int iType)
{
	GetNanoSpecIni(pVoid, iType);
}

/////////////////////////////////////////////////////////////////////////////
// Name       : ConfigFile_SetNanoSpecIni
// Purpose    : Iniファイルの設定の保存(メモリ)
// Parameters : pVoid ---> 保存するデータの構造体等のポインタ
//              iType ---> どのデータを保存するかを決める変数
//
// Returns    : なし
void CONFAPI ConfigFile_SetNanoSpecIni(void* pVoid, int iType)
{
	SetNanoSpecIni(pVoid, iType);
}
