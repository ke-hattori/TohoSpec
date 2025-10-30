// Pattern.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\PatternFile.hxx"
#include "Pattern.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// ファイルが存在するかを確認する関数（存在する場合、あわせて最終更新日時を設定する）
BOOL FILEEXIST(LPCTSTR pszFileName, SYSTEMTIME* lpLastWriteSystemTime)
{
	WIN32_FILE_ATTRIBUTE_DATA win32Data;
	FILETIME localFileTime;

	if ( !GetFileAttributesEx(pszFileName, GetFileExInfoStandard, &win32Data) )
		return FALSE;
	::FileTimeToLocalFileTime(&win32Data.ftLastWriteTime, &localFileTime);
	::FileTimeToSystemTime(&localFileTime, lpLastWriteSystemTime);
	return TRUE;
}

// PatternFileのStageProgramのセクション名
char g_lpszPatternFileStageProgramSection[] = "StageProgram";

// PatternFileのStageProgramのポイント数
char g_lpszPatternFileStageProgramPointNum[] = "NumScans";

// PatternFileのPointのリストのセクション名
char g_lpszPatternFilePointListSection[] = "Point";

//2009.12.10 bagus Recipe Backup --{--
char g_szBackupPath[MAX_PATH+1];
BOOL g_bUseBackup;
//2009.12.10 bagus Recipe Backup --}--

/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadPointList
// Purpose	  : PatternFileのパターンリストの読み込み
// Parameters : pPoint		---> SITE_PATTERNの配列
//				pNumScans	---> 読み込む測定ポイント数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadPointList(SITE_PATTERN* pPoint, WORD* pNumScans, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	MakePatternFilePath(szFilePath, pszFileName);
//	sprintf(szFilePath, DB_PATTERN_DIR "%s" PATTERN_EXT, pszFileName);		// ステージPGM
	return LoadPointList2(pPoint, pNumScans, szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadPointList2
// Purpose	  : PatternFileのパターンリストの読み込み
// Parameters : pPoint	  ---> SITE_PATTERNの配列
//				pNumScans ---> 読み込む測定ポイント数
//								NULLを指定された場合はファイル内に
//								登録されている測定ポイント数
//				pszFilePath ---> ファイルパス名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadPointList2(SITE_PATTERN* pPoint, WORD* pNumScans, LPCSTR pszFilePath)
{
	CHAR	szBuf[RECIPE_COMMENT_LEN + 1];
	char	szSection[256];
	CString strBuff;
	BOOL	bFind = FALSE;
	int		pos1, pos2;
	SYSTEMTIME lastWriteSystemTime;

	// ファイルが存在しない場合
	if ( !FILEEXIST(pszFilePath, &lastWriteSystemTime) )
		return FALSE;

	TRY
	{
		if( *pNumScans == NULL ){
			// 読み込み数が指定されていない場合
			memset( szBuf, '\0', sizeof(szBuf) );
			::GetPrivateProfileString((LPCTSTR)g_lpszPatternFileStageProgramSection,
									(LPCTSTR)g_lpszPatternFileStageProgramPointNum,
									"0",
									szBuf,
									sizeof(szBuf),
									pszFilePath);
			*pNumScans = (WORD)atoi(szBuf);
		}
		CStdioFile file(pszFilePath, CFile::modeRead);
		// セクション[Point]を探す
		memset( szSection, '\0', sizeof(szSection) );
		sprintf( szSection, "[%s]", g_lpszPatternFilePointListSection );
		while(file.ReadString(strBuff)){
			strBuff.Replace('\r', '\0');
			strBuff.Replace('\n', '\0');
			if ( strBuff == szSection )
			{
				bFind = TRUE;
				break;
			}
		}

		if(!bFind){
			file.Close();
			return FALSE;
		}

		for(int i = 0; i < *pNumScans; i++){
			if(!file.ReadString(strBuff)){
				file.Close();
				return FALSE;
			}
			if( strcmp(strBuff, "") == 0 ){
				file.Close();
				return FALSE;
			}

			strBuff.Replace('\r', '\0');
			strBuff.Replace('\n', '\0');
			// Enable
			pos1 = strBuff.Find(':') + 1;
			pos2 = strBuff.Find(',', pos1) + 1;
			(pPoint + i)->bEnable = atoi(strBuff.Mid(pos1, pos2 - pos1 - 1));
			// bDefined1
			pos1 = pos2;
			pos2 = strBuff.Find(',', pos1) + 1;
			(pPoint + i)->PatternInfo[0].bDefined = atoi(strBuff.Mid(pos1, pos2 - pos1 - 1));
			// Pattern1
			pos1 = pos2;
			pos2 = strBuff.Find(',', pos1) + 1;
			strcpy((pPoint + i)->PatternInfo[0].szSitePatternName, strBuff.Mid(pos1, pos2 - pos1 - 1));
			// Lens1
			pos1 = pos2;
			pos2 = strBuff.Find(',', pos1) + 1;
			(pPoint + i)->PatternInfo[0].iLens = atoi(strBuff.Mid(pos1, pos2 - pos1 - 1));
			// bDefined2
			pos1 = pos2;
			pos2 = strBuff.Find(',', pos1) + 1;
			(pPoint + i)->PatternInfo[1].bDefined = atoi(strBuff.Mid(pos1, pos2 - pos1 - 1));
			// Pattern2
			pos1 = pos2;
			pos2 = strBuff.Find(',', pos1) + 1;
			strcpy((pPoint + i)->PatternInfo[1].szSitePatternName, strBuff.Mid(pos1, pos2 - pos1 - 1));
			// Lens2
			(pPoint + i)->PatternInfo[1].iLens = atoi(strBuff.Mid(pos2));
		}
		file.Close();
		return TRUE;
	}
	CATCH( CFileException, e )
	{
		return FALSE;
	}
	END_CATCH
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : SavePointList
// Purpose	  : PatternFileのパターンリストの保存
// Parameters : pPoint		---> SITE_PATTERNの配列
//				wNumScans	---> 保存する測定ポイント数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SavePointList(const SITE_PATTERN* pPoint, WORD wNumScans, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	MakePatternFilePath(szFilePath, pszFileName);
//	sprintf(szFilePath, DB_PATTERN_DIR "%s" PATTERN_EXT, pszFileName);
	return SavePointList2(pPoint, wNumScans, szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : SavePointList2
// Purpose	  : PatternFileのパターンリストの保存
// Parameters : pPoint		---> SITE_PATTERNの配列
//				wNumScans	---> 保存する測定ポイント数
//				pszFilePath ---> ファイルパス名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SavePointList2(const SITE_PATTERN* pPoint, WORD wNumScans, LPCSTR pszFilePath)
{
	SYSTEMTIME lastWriteSystemTime;
	char Buff[256];
	TRY
	{
		// ポイント数をチェックする
		if( wNumScans == NULL ) return FALSE;

		// ファイルが存在する場合は削除する
		if ( FILEEXIST(pszFilePath, &lastWriteSystemTime) )
			::DeleteFile(pszFilePath);

		CStdioFile file(pszFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
		file.SeekToEnd();

		// セクション名[StageProgram]を書き込む
		sprintf(Buff, "[%s]\n", g_lpszPatternFileStageProgramSection);
		file.WriteString(Buff);

		// ポイント数を書き込む
		sprintf(Buff, "%s=%d\n", g_lpszPatternFileStageProgramPointNum, wNumScans);
		file.WriteString(Buff);

		// セクション名[Point]を書き込む
		sprintf(Buff, "[%s]\n", g_lpszPatternFilePointListSection);
		file.WriteString(Buff);

		// ポイントを書き込む
		for(int i = 0; i < (int)wNumScans; i++){
			sprintf(Buff, "%d:%d,%d,%s,%d,%d,%s,%d\n" , i + 1
												, (pPoint + i)->bEnable
												, (pPoint + i)->PatternInfo[0].bDefined
												, (pPoint + i)->PatternInfo[0].szSitePatternName
												, (pPoint + i)->PatternInfo[0].iLens
												, (pPoint + i)->PatternInfo[1].bDefined
												, (pPoint + i)->PatternInfo[1].szSitePatternName
												, (pPoint + i)->PatternInfo[1].iLens);
			file.WriteString(Buff);
		}
		file.WriteString("\n");
		file.Close();
		//2009.12.10 bagus Recipe Backup --{--
		BackupRecipe(pszFilePath);
		//2009.12.10 bagus Recipe Backup --}--
		return TRUE;
	}
	CATCH( CFileException, e )
	{
		return FALSE;
	}
	END_CATCH
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : MakePatternFilePath
// Purpose	  : ファイル名とレシピタイプからファイルパス名を生成
// Parameters : pszFilePath ---> ファイルパス名
//				pszFileName ---> ファイル名
//				iType		---> どのファイル(MainReicpe等)なのかを決める変数
//				iOption1 ---> オプションデータ１
//
void MakePatternFilePath(LPSTR pszFilePath, LPCSTR pszFileName)
{
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
//	sprintf(pszFilePath, DB_PATTERN_DIR "%s" PATTERN_EXT, pszFileName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ----------			   */
	_stprintf(pszFilePath, _T("%s%s%s%s"), g_tszProcDir, _T(DB_PATTERN_DIR), pszFileName, _T(PATTERN_EXT));
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : MakePatternImageFilePath
// Purpose	  : ファイル名とレシピタイプからファイルパス名を生成
// Parameters : pszFilePath ---> ファイルパス名
//				pszFileName ---> ファイル名
//				iOption1 ---> オプションデータ１
//
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- { ---------- */
//void MakePatternImageFilePath(LPSTR pszFilePath, LPCSTR pszFileName, int iLens)
//{
//	sprintf(pszFilePath, DB_PATTERN_IMG_DIR "%s.SP%d" , pszFileName, iLens);
//}
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ----------			  */
void MakePatternImageFilePath(LPTSTR ptszFilePath, LPCTSTR ptszFileName)
{
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
//	_stprintf(ptszFilePath, DB_PATTERN_IMG_DIR _T("%s%s") , ptszFileName, _T(PATTERN_IMAGE_EXT));
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ----------			   */
	_stprintf(ptszFilePath, _T("%s%s%s%s") , g_tszProcDir, _T(DB_PATTERN_IMG_DIR), ptszFileName, _T(PATTERN_IMAGE_EXT));
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */
}
/* modified 2009.07.01 hmenjo イメージファイル拡張子 .spn ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// Name 	  : GetPatternFileInfo
// Purpose	  : ファイルパス名からファイル名とレンズ倍率を生成
// Parameters : pszFilePath ---> ファイルパス名
//				pszFileName ---> ファイル名
//				iLens ---> レンズ倍率
//
void GetPatternFileInfo(LPCSTR pszFilePath, LPSTR pszFileName, int* iLens)
{
	CString strBuf;
	int pos=0;

	strBuf = pszFilePath;
	pos = strBuf.Find('.', pos);

	strcpy(pszFileName, strBuf.Left(pos));
	*iLens = atoi( strBuf.Mid(pos+3));
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : ExistPatternFilePath
// Purpose	  : ファイルパス名が存在するかを確認
// Parameters : pszFilePath ---> ファイルパス名
//				lpLastWriteSystemTime ---> ファイルの最終更新日時
//
// Returns	  : TRUE  ---> 存在する
//				FALSE ---> 存在しない
BOOL ExistPatternFilePath(LPCSTR pszFilePath, SYSTEMTIME* lpLastWriteSystemTime)
{
	return FILEEXIST(pszFilePath, lpLastWriteSystemTime);
}

/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- { ---------- */
#define	SPR_PROF_SECTION	_T("SPR_INF")	/* SPR パターン情報ファイルのセクション名 */
#define	SPR_PROF_KEY_X		_T("OffsetX")	/* SPT オフセット X 軸値のキー名 */
#define	SPR_PROF_KEY_Y		_T("OffsetY")	/* SPT オフセット Y 軸値のキー名 */
#define	SPR_PROF_KEY_LENS	_T("Lens")		/* SPT レンズ情報のキー名 */
/////////////////////////////////////////////////////////////////////////////
/* Name 	  : SetSubInfo
// Purpose	  : SPR パターンサブ情報をファイルに書込みます．
// Parameters : LPCTSTR ptszPattern			---> パターン名
//				LPCSPR_SUB_INFO pPatSubInfo	---> サブ情報
//
// Returns	  : TRUE  ---> 書込み成功
//				FALSE ---> 書込み失敗									*/
BOOL SetSubInfo(LPCTSTR ptszPatName, LPCSPR_SUB_INFO pPatSubInfo)
{
	/* パターンサブ情報ファイルパスを作成 */
	if (0 == _tcscmp(ptszPatName, _T(""))) {
		return FALSE;
	}
	TCHAR l_tszFilePath[_MAX_PATH];
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
//	_stprintf(l_tszFilePath, _T("%s%s.dat"), _T(DB_PATTERN_DIR), ptszPatName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ----------			   */
	_stprintf(l_tszFilePath, _T("%s%s%s.dat"), g_tszProcDir, _T(DB_PATTERN_DIR), ptszPatName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */
	/* 書込み */
	TCHAR l_tszTemp[64];
	BOOL l_bRet;
	l_bRet = ::WritePrivateProfileString(SPR_PROF_SECTION, SPR_PROF_KEY_X, _ltot(pPatSubInfo->ptOffset.x, l_tszTemp, 10), l_tszFilePath);
	if (0 == l_bRet) {
		return FALSE;
	}
	l_bRet = ::WritePrivateProfileString(SPR_PROF_SECTION, SPR_PROF_KEY_Y, _ltot(pPatSubInfo->ptOffset.y, l_tszTemp, 10), l_tszFilePath);
	if (0 == l_bRet) {
		return FALSE;
	}
	l_bRet = ::WritePrivateProfileString(SPR_PROF_SECTION, SPR_PROF_KEY_LENS, _ltot(pPatSubInfo->iLens, l_tszTemp, 10), l_tszFilePath);
	if (0 == l_bRet) {
		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
/* Name 	  : GetSubInfo
// Purpose	  : SPR パターンサブ情報をファイルに書込みます．
// Parameters : LPCTSTR ptszPattern		---> パターン名
//				LPCSPR_SUB_INFO pPatSubInfo	---> サブ情報
//
// Returns	  : TRUE  ---> 読込み成功
//				FALSE ---> 読込み失敗									*/
BOOL GetSubInfo(LPCTSTR ptszPatName, LPSPR_SUB_INFO pPatSubInfo)
{
	BOOL l_bRet = TRUE;
	/* パターンサブ情報ファイルパスを作成 */
	if (0 == _tcscmp(ptszPatName, _T(""))) {
		l_bRet = FALSE;
	}
	TCHAR l_tszFilePath[_MAX_PATH];
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
//	_stprintf(l_tszFilePath, _T("%s%s.dat"), _T(DB_PATTERN_DIR), ptszPatName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ----------			   */
	_stprintf(l_tszFilePath, _T("%s%s%s.dat"), g_tszProcDir, _T(DB_PATTERN_DIR), ptszPatName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */
	/* 読込み */
	UINT l_uiX;
	if (TRUE == l_bRet) {
		if ((0 == (l_uiX = ::GetPrivateProfileInt(SPR_PROF_SECTION, SPR_PROF_KEY_X, 0, l_tszFilePath)))
		 && (1 == ::GetPrivateProfileInt(SPR_PROF_SECTION, SPR_PROF_KEY_X, 1, l_tszFilePath))) {
			l_bRet = FALSE;
		}
	}
	UINT l_uiY;
	if (TRUE == l_bRet) {
		if ((0 == (l_uiY = ::GetPrivateProfileInt(SPR_PROF_SECTION, SPR_PROF_KEY_Y, 0, l_tszFilePath)))
		 && (1 == ::GetPrivateProfileInt(SPR_PROF_SECTION, SPR_PROF_KEY_Y, 1, l_tszFilePath))) {
			l_bRet = FALSE;
		}
	}
	UINT l_uiLens;
	if (TRUE == l_bRet) {
		if ((0 == (l_uiLens = ::GetPrivateProfileInt(SPR_PROF_SECTION, SPR_PROF_KEY_LENS, 0, l_tszFilePath)))
		 && (1 == ::GetPrivateProfileInt(SPR_PROF_SECTION, SPR_PROF_KEY_LENS, 1, l_tszFilePath))) {
			l_bRet = FALSE;
		}
	}

	if (TRUE == l_bRet) {
		pPatSubInfo->ptOffset.x = l_uiX;
		pPatSubInfo->ptOffset.y = l_uiY;
		pPatSubInfo->iLens = l_uiLens;
	} else {
		pPatSubInfo->ptOffset.x = 0;
		pPatSubInfo->ptOffset.y = 0;
		pPatSubInfo->iLens = 0;
	}

	return l_bRet;
}
/////////////////////////////////////////////////////////////////////////////
/* Name 	  : RemoveSubInfo
// Purpose	  : SPR パターンサブ情報ファイルを削除します．
// Parameters : LPCTSTR ptszPattern ---> パターン名
//
// Returns	  : TRUE  ---> 削除成功
//				FALSE ---> 削除失敗									*/
BOOL RemoveSubInfo(LPCTSTR ptszPatName)
{
	/* パターンサブ情報ファイルパスを作成 */
	if (0 == _tcscmp(ptszPatName, _T(""))) {
		return FALSE;
	}
	TCHAR l_tszFilePath[_MAX_PATH];
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- { ---------- */
//	_stprintf(l_tszFilePath, _T("%s%s.dat"), _T(DB_PATTERN_DIR), ptszPatName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ----------			   */
	_stprintf(l_tszFilePath, _T("%s%s%s.dat"), g_tszProcDir, _T(DB_PATTERN_DIR), ptszPatName);
/* modified 2009.07.01 hmenjo PatternFile 相対パス対応 ---------- } ---------- */
	/* 削除 */
	int l_iRet = _tremove(l_tszFilePath);
	if (0 != l_iRet) {
		return FALSE;
	}

	return TRUE;
}
/* added 2009.06.22 hmenjo SPR オフセット読込み/書込み機能追加 ---------- } ---------- */
// 2009.12.10 bagus Recipe Backup --{--
// ==========================================================================
// 関数名：BOOL DeleteDirectory(LPCTSTR lpPathName)
// 引　数：lpPathName　削除するディレクトリのパス名が入ったNULLで終わる文字列。
// 戻り値：関数が成功すると、0以外の値が返り、関数が失敗すると、0が返ります。
// 解　説：ディレクトリを丸ごと削除する。
static BOOL DeleteDirectory(LPCTSTR lpPathName)
{
	CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	if(fnd.FindFile(strPathName, 0)){
		int i = 1;
		while(i){
			i = fnd.FindNextFile();

			// ファイル名が"."か".."の場合は次を検索
			if(fnd.IsDots())
				continue;

			// 削除するファイル名取得
			// GetFilePath()にはバグがあり正確に取得できない場合があるので使わない
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// フォルダだった場合、再帰呼び出しでそのフォルダを削除
			if(fnd.IsDirectory())
				DeleteDirectory(strDeleteFile);

			// ファイルの削除
			else
				::DeleteFile(strDeleteFile);
		}
		fnd.Close();

		// フォルダの削除
		return ::RemoveDirectory(lpPathName);
	}
	return FALSE;
}
static BOOL CopyDirectory(LPCTSTR lpszSource,LPCTSTR lpszDest)
{
	SHFILEOPSTRUCT tSHFile;
	char szFrom[MAX_PATH];
	char szTo[MAX_PATH];

	_fullpath(szFrom, (const char *)lpszSource, sizeof(szFrom)); //絶対パスに変換
	_fullpath(szTo, (const char *)lpszDest, sizeof(szTo)); //絶対パスに変換

	ZeroMemory(&tSHFile, sizeof(SHFILEOPSTRUCT));

	CString from = szFrom;
	CString to = szTo;

	tSHFile.hwnd = NULL;
	tSHFile.wFunc = FO_COPY;
	//2009.12.17 bagus 修正 --{--
	//tSHFile.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SIMPLEPROGRESS ;
	tSHFile.fFlags = FOF_NOCONFIRMMKDIR | FOF_NOERRORUI ;
	//2009.12.17 bagus 修正 --}--

	tSHFile.fAnyOperationsAborted = TRUE;
	tSHFile.hNameMappings = NULL;
	//2009.12.17 bagus 修正 --{--
	//tSHFile.lpszProgressTitle = NULL;
	tSHFile.lpszProgressTitle = "Copy";
	//2009.12.17 bagus 修正 --}--

	from += "0";
	to += "0";
	from.SetAt( from.GetLength()-1, NULL );
	to.SetAt( to.GetLength()-1, NULL );

	tSHFile.pFrom = (LPCTSTR)from;
	tSHFile.pTo = (LPCTSTR)to;

	return SHFileOperation( &tSHFile ); // 実行
}
static int MakeDirectory(char* lpszDir)
{
	char szBuff[256];
	int i;
	int iRet;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = NULL;

	if((iRet = CreateDirectory(lpszDir, &sa)) == TRUE)
		return 1;
	else{
		//既に存在している場合にはそこでOKとする
		if(GetLastError() == ERROR_ALREADY_EXISTS){
			return 1;
		}
	}

	for(i = strlen(lpszDir); i > 0; i--){
		if (lpszDir[i] == '\\' || lpszDir[i] == '/')
			break;
	}

	if(i <= 0)
		return 0;

	strcpy(szBuff, lpszDir);
	szBuff[i] = NULL;
	//if(iRet = MakeDirectory(szBuff))
	//	return (iRet = CreateDirectory(lpszDir, &sa)) == TRUE ? 1 : 0;
	if(iRet = MakeDirectory(szBuff)){
		iRet = CreateDirectory(lpszDir, &sa);
		if(iRet){
			return 1;
		}else{
			//既に存在している場合にはそこでOKとする
			if(GetLastError() == ERROR_ALREADY_EXISTS){
				return 1;
			}
		}
	}

	return 0;
}
BOOL SetBackupPath(LPCTSTR lpszBackupPath,BOOL bUse)
{
	ASSERT(lpszBackupPath != NULL);
	strncpy(g_szBackupPath,lpszBackupPath,MAX_PATH);
	g_bUseBackup = bUse;

	return TRUE;
}
BOOL BackupRecipe(LPCTSTR lpszSourcePath)
{
	ASSERT(lpszSourcePath != NULL);
	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szFile[MAX_PATH];
	char szExt[MAX_PATH];
	char szToPath[MAX_PATH];
	char szFromPath[MAX_PATH];
	BOOL bRet = TRUE;
	char *pSubDirPos;

	_splitpath(lpszSourcePath,szDrive,szDir,szFile,szExt);
	if(g_bUseBackup){
		//DB\からのディレクトリパスを取得
		pSubDirPos = strstr(szDir,DB_DIR);
		if(pSubDirPos == NULL){
			pSubDirPos = ".";
		}else{
			pSubDirPos = pSubDirPos + strlen(DB_DIR);
		}
		//バックアップ先のディレクトリを作成しておく
		wsprintf(szToPath,"%s\\%s\\%s",g_szBackupPath,DB_DIR_NAME,pSubDirPos);
		//2009.12.25 bagus Recipe Backup --{--
		DeleteDirectory(szToPath);
		//2009.12.25 bagus Recipe Backup --}--
		MakeDirectory(szToPath);
//2009.12.25 bagus Recipe Backup --{--
//		wsprintf(szToPath,"%s\\%s\\%s\\%s%s",g_szBackupPath,DB_DIR_NAME,pSubDirPos,szFile,szExt);
//		bRet = CopyFile(lpszSourcePath,szToPath,FALSE);
		wsprintf(szFromPath,"%s\\%s\\%s",szDrive,szDir,"*");
		bRet = CopyDirectory(szFromPath,szToPath);
//2009.12.25 bagus Recipe Backup --}--
	}

	return TRUE;
}
// 2009.12.10 bagus Recipe Backup --}--
//2009.12.25 bagus Recipe Backup --{--
BOOL DeleteBackupRecipe(LPCTSTR lpszSourcePath)
{
	ASSERT(lpszSourcePath != NULL);
	char szDrive[MAX_PATH];
	char szDir[MAX_PATH];
	char szFile[MAX_PATH];
	char szExt[MAX_PATH];
	char szToPath[MAX_PATH];
	BOOL bRet = TRUE;
	char *pSubDirPos;

	_splitpath(lpszSourcePath,szDrive,szDir,szFile,szExt);
	if(g_bUseBackup){
		//DB\からのディレクトリパスを取得
		pSubDirPos = strstr(szDir,DB_DIR);
		if(pSubDirPos == NULL){
			pSubDirPos = ".";
		}else{
			pSubDirPos = pSubDirPos + strlen(DB_DIR);
		}
		//削除する
		wsprintf(szToPath,"%s\\%s\\%s\\%s%s",g_szBackupPath,DB_DIR_NAME,pSubDirPos,szFile,szExt);
		bRet = DeleteFile(szToPath);
	}

	return TRUE;


}
//2009.12.25 bagus Recipe Backup --}--
