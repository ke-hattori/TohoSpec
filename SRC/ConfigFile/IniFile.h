#ifndef _INIFILE_H_
#define _INIFILE_H_

void LoadNanoSpecIni(int iIniFile);
void SaveNanoSpecIni(int iIniFile);
void LoadAllNanoSpecIni();
void SaveAllNanoSpecIni();
void GetNanoSpecIni(void* pVoid, int iType);
void SetNanoSpecIni(void* pVoid, int iType);

#endif // _INIFILE_H_
