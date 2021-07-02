#ifndef _CONFIG_FILE_HXX_
#define _CONFIG_FILE_HXX_

#include "..\\..\\INC\\Globals.hxx"

#ifndef CONFAPI
#define CONFAPI 	__declspec(dllexport)
#endif	// CONFAPI

/////////////////////////////////////////////////////////////////////////////
// Function Definition                                                     //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

void CONFAPI ConfigFile_LoadNanoSpecIni(int iIniFile);
void CONFAPI ConfigFile_SaveNanoSpecIni(int iIniFile);
void CONFAPI ConfigFile_LoadAllNanoSpecIni();
void CONFAPI ConfigFile_SaveAllNanoSpecIni();
void CONFAPI ConfigFile_GetNanoSpecIni(void* pVoid, int iType);
void CONFAPI ConfigFile_SetNanoSpecIni(void* pVoid, int iType);

#ifdef __cplusplus
} /* End of extern "C" { */
#endif


#endif // _CONFIG_FILE_HXX_
