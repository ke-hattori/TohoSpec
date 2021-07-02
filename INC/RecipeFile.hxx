#pragma once

#include <Globals.hxx>

// dllimport / dllexport
#if !defined(_RECIPEFILEDLL_)
#define RECIPEFILEAPI __declspec(dllimport)
#else
#define RECIPEFILEAPI __declspec(dllexport)
#endif

extern "C" void RECIPEFILEAPI RecipeFile_Lock();
extern "C" void RECIPEFILEAPI RecipeFile_Unlock();

extern "C" BOOL RECIPEFILEAPI RecipeFile_LoadRecipe(LPVOID pVoid, LPCSTR lpszName, int iType);
extern "C" BOOL RECIPEFILEAPI RecipeFile_SaveRecipe(LPCVOID pVoid, LPCSTR lpszName, int iType);
extern "C" BOOL RECIPEFILEAPI RecipeFile_LoadMainRecipeList(MULTI_RCP_MAIN_RCP_LIST* pMainRcpList, WORD wNumMainRecipe, LPCSTR lpszName);
extern "C" BOOL RECIPEFILEAPI RecipeFile_SaveMainRecipeList(const MULTI_RCP_MAIN_RCP_LIST* pMainRcpList, WORD wNumMainRecipe, LPCSTR lpszName);
extern "C" BOOL RECIPEFILEAPI RecipeFile_LoadPointList(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
extern "C" BOOL RECIPEFILEAPI RecipeFile_SavePointList(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
// 2009.10.15 bagus Distance ’Ç‰Á --{--
extern "C" BOOL RECIPEFILEAPI RecipeFile_Load2PointList(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
extern "C" BOOL RECIPEFILEAPI RecipeFile_Save2PointList(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName);
// 2009.10.15 bagus Distance ’Ç‰Á --}--
extern "C" BOOL RECIPEFILEAPI RecipeFile_SaveCurrRecipeInfo(LPCSTR lpszName,  int = 0);

extern "C" BOOL RECIPEFILEAPI RecipeFile_DeleteRecipe(LPCSTR lpszName, int iType);
extern "C" BOOL RECIPEFILEAPI RecipeFile_ExistRecipe(LPCSTR lpszName, SYSTEMTIME* lpLastWriteSystemTime, int iType);

// 2009.12.10 bagus Recipe Backup --{--
extern "C" BOOL RECIPEFILEAPI RecipeFile_SetBackupPath(LPCSTR lpszBackupPath,BOOL bUse);
extern "C" BOOL RECIPEFILEAPI RecipeFile_BackupAllRecipe();
extern "C" BOOL RECIPEFILEAPI RecipeFile_RestoreAllRecipe();
// 2009.12.10 bagus Recipe Backup --}--
//Saiki 20110208 Add ----->
extern "C" BOOL RECIPEFILEAPI RecipeFile_SetStressParam(LPVOID pVoid, LPCSTR lpszName, int iType);
extern "C" BOOL RECIPEFILEAPI RecipeFile_LoadStressParam(LPVOID pVoid, LPCSTR lpszName, int iType);
//Saiki 20110208 Add <-----
