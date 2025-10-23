#include "stdafx.h"
#include <time.h>
#include <FLOAT.H>
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\MeaSys.hxx"
#include "SrRefFile.h"

extern TCHAR g_tszData_Ref_Dir[_MAX_PATH];

// ==========================================================================
// Define                                                                  //
//                                                                         //
// ==========================================================================

#define DATAFILENAME2NDREFT1			"_2ndRefT1"
#define DATAFILENAME2NDREFT2			"_2ndRefT2"

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::ReadRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
// TRUE  : 読込成功
// FALSE : 読込失敗
{
	// リファレンスデータファイルを読込む
	//
	// (* sample *)
	// Start=380
	// End=800
	// nm	Dark	 Reference
	//	  1 5555.555 5555.555
	// ...
	// 1000 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 256;
	TCHAR szLine[nMaxSize];

	sprintf(szPath, "%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "r")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark = &pScanData[MEASYS_DARK_T1];
	PSCANDATA pCSRef  = &pScanData[MEASYS_STDR_T1];
	// ヘッダ情報（1行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanStart = pCSRef->iScanStart = atoi(&szLine[strlen("Start=")]);
	// ヘッダ情報（2行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanEnd = pCSRef->iScanEnd = atoi(&szLine[strlen("End=")]);
	// ヘッダ情報（3行目）
	_fgetts(szLine, nMaxSize, fp);
	// 光量（4行目以降）
// 2014.04.04 bagus wavelength step modified -->
// 	int i = 0;
// 	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
// 	{
// 		TCHAR buf[nMaxSize];
// 		strcpy(buf, szLine);
// 		TCHAR* tokenWave = strtok(buf, " ");
// 		tokenWave = strtok(NULL, " ");
// 		pCSDark->dScanData[i] = atof(tokenWave);					// ダークデータの取得
// 		tokenWave = strtok(NULL, " ");
// 		pCSRef->dScanData[i] = atof(tokenWave); 					// リファレンスデータの取得
// 		i++;
// 	}

	double d = pCSDark->dWavelengthStep;
	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
	{
		TCHAR buf[nMaxSize];
		strcpy(buf, szLine);
		TCHAR* tokenWave = strtok(buf, " ");
		tokenWave = strtok(NULL, " ");
		pCSDark->SetScanData(d, atof(tokenWave));					// ダークデータの取得
		tokenWave = strtok(NULL, " ");
		pCSRef->SetScanData(d, atof(tokenWave)); 					// リファレンスデータの取得
		d += pCSDark->dWavelengthStep;
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);

	return TRUE;
}

// 2009.09.29 K.Matsuo GTr -->
/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::ReadGTrRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName, int iCoordRefPos)
// TRUE  : 読込成功
// FALSE : 読込失敗
{
	// ガントリー透過率用・リファレンスデータファイルを読込む
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     Ref1     ...
    //    1 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 1024;
	TCHAR szLine[nMaxSize];

	sprintf(szPath, "%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "r")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark = &pScanData[MEASYS_DARK_T1];
	PSCANDATA pCSRef = &pScanData[MEASYS_STDR_T1];
	// ヘッダ情報（1行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanStart = pCSRef->iScanStart = atoi(&szLine[strlen("Start=")]);
	// ヘッダ情報（2行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanEnd = pCSRef->iScanEnd = atoi(&szLine[strlen("End=")]);
	// ヘッダ情報（3行目）
	_fgetts(szLine, nMaxSize, fp);
	// 光量（4行目以降）
// 2014.04.04 bagus wavelength step modified -->
// 	int i = 0;
// 	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
// 	{
// 		TCHAR buf[nMaxSize];
// 		strcpy(buf, szLine);
// 		TCHAR* tokenWave = strtok(buf, " ");
// 		tokenWave = strtok(NULL, " ");
// 		pCSDark->dScanData[i] = atof(tokenWave);					// ダークデータの取得
//
// 		for ( int j = 1; j <= iCoordRefPos; j++ ) {
// 			tokenWave = strtok(NULL, " ");
// 			if ( !tokenWave )
// 				return FALSE;
// 		}
//
// 		pCSRef->dScanData[i] = atof(tokenWave); 				   // リファレンスデータの取得
// 		i++;
// 	}

	double d = pCSDark->dWavelengthStep;
	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
	{
		TCHAR buf[nMaxSize];
		strcpy(buf, szLine);
		TCHAR* tokenWave = strtok(buf, " ");
		tokenWave = strtok(NULL, " ");
		pCSDark->SetScanData(d, atof(tokenWave));					// ダークデータの取得

		for ( int j = 1; j <= iCoordRefPos; j++ ) {
			tokenWave = strtok(NULL, " ");
			if ( !tokenWave )
				return FALSE;
		}

		pCSRef->SetScanData(d, atof(tokenWave)); 				   // リファレンスデータの取得
		d += pCSDark->dWavelengthStep;
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);

	return TRUE;
}
// 2009.09.29 K.Matsuo GTr <--

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::ReadRefFile2ndRefT1(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
// TRUE  : 読込成功
// FALSE : 読込失敗
{
	// リファレンスデータファイルを読込む
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     1stReference 2ndReference
    //    1 5555.555 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 256;
	TCHAR szLine[nMaxSize];

	sprintf(szPath, "%s%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DATAFILENAME2NDREFT1, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "r")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark	= &pScanData[MEASYS_DARK_T1];
	PSCANDATA pCS1stRef = &pScanData[MEASYS_STDR_T1];
	PSCANDATA pCS2ndRef = &pScanData[MEASYS_EXTR_T1];
	// ヘッダ情報（1行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanStart = pCS1stRef->iScanStart = pCS2ndRef->iScanStart = atoi(&szLine[strlen("Start=")]);
	// ヘッダ情報（2行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanEnd = pCS1stRef->iScanEnd = pCS2ndRef->iScanEnd = atoi(&szLine[strlen("End=")]);
	// ヘッダ情報（3行目）
	_fgetts(szLine, nMaxSize, fp);
	// 光量（4行目以降）
// 2014.04.04 bagus wavelength step modified -->
// 	int i = 0;
// 	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
// 	{
// 		TCHAR buf[nMaxSize];
// 		strcpy(buf, szLine);
// 		TCHAR* tokenWave = strtok(buf, " ");
// 		tokenWave = strtok(NULL, " ");
// 		pCSDark->dScanData[i] = atof(tokenWave);					// ダークT1データの取得
// 		tokenWave = strtok(NULL, " ");
// 		pCS1stRef->dScanData[i] = atof(tokenWave);					// 1stリファレンスT1データの取得
// 		tokenWave = strtok(NULL, " ");
// 		pCS2ndRef->dScanData[i] = atof(tokenWave);					// 2stリファレンスT1データの取得
// 		i++;
// 	}

	double d = pCSDark->dWavelengthStep;
	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
	{
		TCHAR buf[nMaxSize];
		strcpy(buf, szLine);
		TCHAR* tokenWave = strtok(buf, " ");
		tokenWave = strtok(NULL, " ");
		pCSDark->SetScanData(d, atof(tokenWave));					// ダークT1データの取得
		tokenWave = strtok(NULL, " ");
		pCS1stRef->SetScanData(d, atof(tokenWave));					// 1stリファレンスT1データの取得
		tokenWave = strtok(NULL, " ");
		pCS2ndRef->SetScanData(d, atof(tokenWave));					// 2stリファレンスT1データの取得
		d += pCSDark->dWavelengthStep;
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::ReadRefFile2ndRefT2(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
// TRUE  : 読込成功
// FALSE : 読込失敗
{
	// リファレンスデータファイルを読込む
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     2ndReference
    //    1 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 256;
	TCHAR szLine[nMaxSize];

	sprintf(szPath, "%s%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DATAFILENAME2NDREFT2, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "r")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark	= &pScanData[MEASYS_DARK_T2];
	PSCANDATA pCS2ndRef = &pScanData[MEASYS_EXTR_T2];
	// ヘッダ情報（1行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanStart = pCS2ndRef->iScanStart = atoi(&szLine[strlen("Start=")]);
	// ヘッダ情報（2行目）
	_fgetts(szLine, nMaxSize, fp);
	pCSDark->iScanEnd = pCS2ndRef->iScanEnd = atoi(&szLine[strlen("End=")]);
	// ヘッダ情報（3行目）
	_fgetts(szLine, nMaxSize, fp);
	// 光量（4行目以降）
// 2014.04.04 bagus wavelength step modified -->
// 	int i = 0;
// 	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
// 	{
// 		TCHAR buf[nMaxSize];
// 		strcpy(buf, szLine);
// 		TCHAR* tokenWave = strtok(buf, " ");
// 		tokenWave = strtok(NULL, " ");
// 		pCSDark->dScanData[i] = atof(tokenWave);					// ダークT2データの取得
// 		tokenWave = strtok(NULL, " ");
// 		pCS2ndRef->dScanData[i] = atof(tokenWave);					// 2ndリファレンスT2データの取得
// 		i++;
// 	}

	double d = pCSDark->dWavelengthStep;
	while ( _fgetts(szLine, nMaxSize, fp) != NULL )
	{
		TCHAR buf[nMaxSize];
		strcpy(buf, szLine);
		TCHAR* tokenWave = strtok(buf, " ");
		tokenWave = strtok(NULL, " ");
		pCSDark->SetScanData(d, atof(tokenWave));					// ダークT2データの取得
		tokenWave = strtok(NULL, " ");
		pCS2ndRef->SetScanData(d, atof(tokenWave));					// 2ndリファレンスT2データの取得
		d += pCSDark->dWavelengthStep;
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::WriteRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
{
	// リファレンスデータファイルに書込み
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     Reference
    //    1 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 256;

	sprintf(szPath, "%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "w")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark = &pScanData[MEASYS_DARK_T1];
	PSCANDATA pCSRef  = &pScanData[MEASYS_STDR_T1];
	fprintf(fp, "Start=%3d\n", pCSDark->iScanStart);				// ヘッダ情報（1行目）
	fprintf(fp, "End=%3d\n", pCSDark->iScanEnd);					// ヘッダ情報（2行目）
	fputs("nm   Dark     Reference\n", fp); 						// ヘッダ情報（3行目）

// 2014.04.04 bagus wavelength step modified -->
//	for ( int i = 0; i < SCANDATA_POINT_MAX; i++ )
//		fprintf(fp, "%4d %lf %lf\n", i+1, pCSDark->dScanData[i], pCSRef->dScanData[i]); 		// 光量（4行目以降）

	if(pCSDark->dWavelengthStep == 0.5){
		for ( double d = 0.5; d <= SCANDATA_POINT_MAX; d += 0.5 ) {
			fprintf(fp, "%4.1lf %lf %lf\n", d, pCSDark->GetScanData(d), pCSRef->GetScanData(d)); 	// 光量（4行目以降）
		}
	}
	else{
		for ( int i = 1; i <= SCANDATA_POINT_MAX; i++ ) {
			fprintf(fp, "%4d %lf %lf\n", i, pCSDark->GetScanData(i), pCSRef->GetScanData(i)); 	// 光量（4行目以降）
		}
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);
	return TRUE;
}

// 2009.09.29 K.Matsuo GTr -->
/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::WriteGTrRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
{
	// ガントリー透過率用・リファレンスデータファイルに書込み
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     Ref1     ...
    //    1 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 1024;
	TCHAR szHeader[nMaxSize];
	TCHAR szData[nMaxSize];
	TCHAR szBuff[nMaxSize];

	sprintf(szPath, "%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "w")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark = &pScanData[MEASYS_DARK_T1];
	PSCANDATA pCSRef = &pScanData[MEASYS_GTR_1];

	fprintf(fp, "Start=%3d\n", pCSDark->iScanStart);				// ヘッダ情報（1行目）
	fprintf(fp, "End=%3d\n", pCSDark->iScanEnd);					// ヘッダ情報（2行目）

	strcpy(szHeader, "nm   Dark "); 								// ヘッダ情報（3行目）
	for ( int i = 0; i < MEASYS_GTR_LAST; i++ ) {
		sprintf(szBuff, "    Ref%02d", i + 1);
		strcat(szHeader, szBuff);
	}
	strcat(szHeader, "\n");
	fputs(szHeader, fp);

// 2014.04.04 bagus wavelength step modified -->
// 	for ( i = 0; i < SCANDATA_POINT_MAX; i++ ) {					// 光量（4行目以降）
// 		sprintf(szData, "%4d %lf", i+1, pCSDark->dScanData[i]);
// 		pCSRef = &pScanData[MEASYS_GTR_1];
// 		for ( int j = 0; j < MEASYS_GTR_LAST; j++ ) {
// 			sprintf(szBuff, " %lf", (pCSRef++)->dScanData[i]);
// 			strcat(szData, szBuff);
// 		}
// 		strcat(szData, "\n");
// 		fputs(szData, fp);
// 	}

	if(pCSDark->dWavelengthStep == 0.5){
		for ( double d = 0.5; d <= SCANDATA_POINT_MAX; d += 0.5 ) {
			sprintf(szData, "%4.1lf %lf", d, pCSDark->GetScanData(d));
			pCSRef = &pScanData[MEASYS_GTR_1];
			for ( int j = 0; j < MEASYS_GTR_LAST; j++ ) {
				sprintf(szBuff, " %lf", (pCSRef++)->GetScanData(d));
				strcat(szData, szBuff);
			}
			strcat(szData, "\n");
			fputs(szData, fp);
		}
	}
	else{
		for ( i = 1; i <= SCANDATA_POINT_MAX; i++ ) {					// 光量（4行目以降）
			sprintf(szData, "%4d %lf", i, pCSDark->GetScanData(i));
			pCSRef = &pScanData[MEASYS_GTR_1];
			for ( int j = 0; j < MEASYS_GTR_LAST; j++ ) {
				sprintf(szBuff, " %lf", (pCSRef++)->GetScanData(i));
				strcat(szData, szBuff);
			}
			strcat(szData, "\n");
			fputs(szData, fp);
		}
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);

	return TRUE;
}
// 2009.09.29 K.Matsuo GTr <--

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::WriteRefFile2ndRefT1(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
{
	// リファレンスデータファイルに書込み
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     1stReference 2ndReference
    //    1 5555.555 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 256;

	sprintf(szPath, "%s%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DATAFILENAME2NDREFT1, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "w")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark	= &pScanData[MEASYS_DARK_T1];
	PSCANDATA pCS1stRef = &pScanData[MEASYS_STDR_T1];
	PSCANDATA pCS2ndRef = &pScanData[MEASYS_EXTR_T1];
	fprintf(fp, "Start=%3d\n", pCSDark->iScanStart);				// ヘッダ情報（1行目）
	fprintf(fp, "End=%3d\n", pCSDark->iScanEnd);					// ヘッダ情報（2行目）
	fputs("nm   Dark     1stReference 2ndReference\n", fp); 		// ヘッダ情報（3行目）
// 2014.04.04 bagus wavelength step modified -->
// 	for ( int i = 0; i < SCANDATA_POINT_MAX; i++ )
// 		fprintf(fp, "%4d %lf %lf %lf\n", i+1, pCSDark->dScanData[i], pCS1stRef->dScanData[i], pCS2ndRef->dScanData[i]); 		// 光量（4行目以降）

	if(pCSDark->dWavelengthStep == 0.5){
		for ( double d = 0.5; d <= SCANDATA_POINT_MAX; d += 0.5 ) {
			fprintf(fp, "%4.1lf %lf %lf %lf\n", d, pCSDark->GetScanData(d), pCS1stRef->GetScanData(d), pCS2ndRef->GetScanData(d)); 	// 光量（4行目以降）
		}
	}
	else{
		for ( int i = 1; i <= SCANDATA_POINT_MAX; i++ ) {
			fprintf(fp, "%4d %lf %lf %lf\n", i, pCSDark->GetScanData(i), pCS1stRef->GetScanData(i), pCS2ndRef->GetScanData(i)); 	// 光量（4行目以降）
		}
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::WriteRefFile2ndRefT2(SCANDATA* pScanData, LPCTSTR pszMainRecipeName)
{
	// リファレンスデータファイルに書込み
	//
	// (* sample *)
	// Start=380
	// End=800
    // nm   Dark     2ndReference
    //    1 5555.555 5555.555
    // ...
    // 1000 5555.555 5555.555

	TCHAR szPath[MAX_PATH+1];
	const int nMaxSize = 256;

	sprintf(szPath, "%s%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DATAFILENAME2NDREFT2, DAT_EXT);
	FILE* fp;
	if ( (fp = fopen(szPath, "w")) == NULL )				// リファレンスデータファイル OPEN
		return FALSE;

	PSCANDATA pCSDark	= &pScanData[MEASYS_DARK_T2];
	PSCANDATA pCS2ndRef = &pScanData[MEASYS_EXTR_T2];
	fprintf(fp, "Start=%3d\n", pCSDark->iScanStart);				// ヘッダ情報（1行目）
	fprintf(fp, "End=%3d\n", pCSDark->iScanEnd);					// ヘッダ情報（2行目）
	fputs("nm   Dark     2ndReference\n", fp);						// ヘッダ情報（3行目）
// 2014.04.04 bagus wavelength step modified -->
// 	for ( int i = 0; i < SCANDATA_POINT_MAX; i++ )
// 		fprintf(fp, "%4d %lf %lf\n", i+1, pCSDark->dScanData[i],  pCS2ndRef->dScanData[i]); 		// 光量（4行目以降）

	if(pCSDark->dWavelengthStep == 0.5){
		for ( double d = 0.5; d <= SCANDATA_POINT_MAX; d += 0.5 ) {
			fprintf(fp, "%4.1lf %lf %lf\n", d, pCSDark->GetScanData(d),  pCS2ndRef->GetScanData(d)); 	// 光量（4行目以降）
		}
	}
	else{
		for ( int i = 1; i <= SCANDATA_POINT_MAX; i++ ) {
			fprintf(fp, "%4d %lf %lf\n", i, pCSDark->GetScanData(i),  pCS2ndRef->GetScanData(i)); 		// 光量（4行目以降）
		}
	}
// 2014.04.04 bagus wavelength step modified <--
	fclose(fp);
	return TRUE;
}


int CSrRefFile::CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime,struct tm &tmFileTime)
// 0 : 正常（期限内有効）
// 1 : エラー（ファイルが存在しない）
// 2 : エラー（期限切れ）
{
	const int AVAILABLE 	= 0;
	const int ERR_NOEXIST	= 1;
	const int ERR_EXPIRED 	= 2;
// 2009.09.10 K.Matsuo <--

	//Saiki 20090603 Change ----->
	CString strMsg, strTitle;
	//Saiki 20090603 Change <-----
	// 期限切れチェック
	TCHAR szPath[MAX_PATH+1];
	sprintf(szPath, "%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DAT_EXT);
	HANDLE hFile = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	// ファイルハンドル取得
	if ( hFile == INVALID_HANDLE_VALUE )
		return ERR_NOEXIST;    										// ファイルがない場合、期限切れとする

	FILETIME ftFileTime;
	FILETIME ftLocalFileTime;
	SYSTEMTIME stLocalFileTime;
	SYSTEMTIME stLocalTime; 										// ローカル時刻

	GetFileTime(hFile, NULL, NULL, &ftFileTime);					// ファイル最終更新日時
	CloseHandle(hFile);
	FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime); 		// ファイル最終更新日時（ローカル時刻）
	FileTimeToSystemTime(&ftLocalFileTime, &stLocalFileTime);		// ファイル最終更新日時（ローカル時刻）SYSTEMTIME型
	GetLocalTime(&stLocalTime); 									// 現在時刻（ローカル時刻）

	struct tm tmCurrTime;
	time_t timeFileTime;
	time_t timeCurrTime;
	ZeroMemory(&tmFileTime, sizeof(tmFileTime));
	tmFileTime.tm_year = stLocalFileTime.wYear - 1900;				// 1900 年からの年
	tmFileTime.tm_mon  = stLocalFileTime.wMonth - 1;				// 1 月からの月数 (0 ～ 11)
	tmFileTime.tm_mday = stLocalFileTime.wDay;
	tmFileTime.tm_hour = stLocalFileTime.wHour;
	tmFileTime.tm_min  = stLocalFileTime.wMinute;
	tmFileTime.tm_sec  = stLocalFileTime.wSecond;
	//Saiki 20090603 Change ----->
	//if ( (timeFileTime = mktime(&tmFileTime)) == (time_t)-1 )
	//	  MessageBox(NULL, "mktime(&tmFileTime) の実行に失敗しました", "NanoSpec", MB_OK);
//	LoadStringML(IDS_TMFILETIMR_FAILED, strMsg, "Failed to execute the mktime(&tmFileTime)");
//	LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//	if ( (timeFileTime = mktime(&tmFileTime)) == (time_t)-1 ){
//		MessageBox(NULL, strMsg, strTitle, MB_OK);
//	}
	//Saiki 20090603 Change <-----
	timeFileTime = mktime(&tmFileTime);

	ZeroMemory(&tmCurrTime, sizeof(tmCurrTime));
	tmCurrTime.tm_year = stLocalTime.wYear - 1900;					// 1900 年からの年
	tmCurrTime.tm_mon  = stLocalTime.wMonth - 1;					// 1 月からの月数 (0 ～ 11)
	tmCurrTime.tm_mday = stLocalTime.wDay;
	tmCurrTime.tm_hour = stLocalTime.wHour;
	tmCurrTime.tm_min  = stLocalTime.wMinute;
	tmCurrTime.tm_sec  = stLocalTime.wSecond;
	//Saiki 20090603 Change ----->
	//if ( (timeCurrTime = mktime(&tmCurrTime)) == (time_t)-1 )
	//	  MessageBox(NULL, "mktime(&tmCurrTime) の実行に失敗しました", "NanoSpec", MB_OK);
//	LoadStringML(IDS_TMCURRTIME_FAILED, strMsg, "Failed to execute the mktime(&tmCurrTime)");
//	LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//	if ( (timeCurrTime = mktime(&tmCurrTime)) == (time_t)-1 ){
//		MessageBox(NULL, strMsg, strTitle, MB_OK);
//	}
	//Saiki 20090603 Change <-----
	timeCurrTime = mktime(&tmCurrTime);

	double dElapsedTime;
	if ( timeFileTime == (time_t)-1 || timeCurrTime == (time_t)-1 )
		dElapsedTime = DBL_MAX;		// 万が一、時刻取得を失敗した場合は、期限切れにする
	else
		dElapsedTime = difftime(timeCurrTime, timeFileTime);

	return ( ( dElapsedTime > dLifeTime * 60 ) ? ERR_EXPIRED : AVAILABLE ); // 秒数比較
}

/////////////////////////////////////////////////////////////////////////////
//
// 2009.09.10 K.Matsuo -->
//BOOL CSrRefFile::CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime)
// TRUE : 期限切れ
int CSrRefFile::CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime)
// 0 : 正常（期限内有効）
// 1 : エラー（ファイルが存在しない）
// 2 : エラー（期限切れ）
{
	const int AVAILABLE 	= 0;
	const int ERR_NOEXIST	= 1;
	const int ERR_EXPIRED 	= 2;
// 2009.09.10 K.Matsuo <--

	//Saiki 20090603 Change ----->
	CString strMsg, strTitle;
	//Saiki 20090603 Change <-----
	// 期限切れチェック
	TCHAR szPath[MAX_PATH+1];
	sprintf(szPath, "%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DAT_EXT);
	HANDLE hFile = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);	// ファイルハンドル取得
	if ( hFile == INVALID_HANDLE_VALUE )
		return ERR_NOEXIST;    										// ファイルがない場合、期限切れとする

	FILETIME ftFileTime;
	FILETIME ftLocalFileTime;
	SYSTEMTIME stLocalFileTime;
	SYSTEMTIME stLocalTime; 										// ローカル時刻

	GetFileTime(hFile, NULL, NULL, &ftFileTime);					// ファイル最終更新日時
	CloseHandle(hFile);
	FileTimeToLocalFileTime(&ftFileTime, &ftLocalFileTime); 		// ファイル最終更新日時（ローカル時刻）
	FileTimeToSystemTime(&ftLocalFileTime, &stLocalFileTime);		// ファイル最終更新日時（ローカル時刻）SYSTEMTIME型
	GetLocalTime(&stLocalTime); 									// 現在時刻（ローカル時刻）

	struct tm tmFileTime;
	struct tm tmCurrTime;
	time_t timeFileTime;
	time_t timeCurrTime;
	ZeroMemory(&tmFileTime, sizeof(tmFileTime));
	tmFileTime.tm_year = stLocalFileTime.wYear - 1900;				// 1900 年からの年
	tmFileTime.tm_mon  = stLocalFileTime.wMonth - 1;				// 1 月からの月数 (0 ～ 11)
	tmFileTime.tm_mday = stLocalFileTime.wDay;
	tmFileTime.tm_hour = stLocalFileTime.wHour;
	tmFileTime.tm_min  = stLocalFileTime.wMinute;
	tmFileTime.tm_sec  = stLocalFileTime.wSecond;
	//Saiki 20090603 Change ----->
	//if ( (timeFileTime = mktime(&tmFileTime)) == (time_t)-1 )
	//	  MessageBox(NULL, "mktime(&tmFileTime) の実行に失敗しました", "NanoSpec", MB_OK);
//	LoadStringML(IDS_TMFILETIMR_FAILED, strMsg, "Failed to execute the mktime(&tmFileTime)");
//	LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//	if ( (timeFileTime = mktime(&tmFileTime)) == (time_t)-1 ){
//		MessageBox(NULL, strMsg, strTitle, MB_OK);
//	}
	//Saiki 20090603 Change <-----
	timeFileTime = mktime(&tmFileTime);

	ZeroMemory(&tmCurrTime, sizeof(tmCurrTime));
	tmCurrTime.tm_year = stLocalTime.wYear - 1900;					// 1900 年からの年
	tmCurrTime.tm_mon  = stLocalTime.wMonth - 1;					// 1 月からの月数 (0 ～ 11)
	tmCurrTime.tm_mday = stLocalTime.wDay;
	tmCurrTime.tm_hour = stLocalTime.wHour;
	tmCurrTime.tm_min  = stLocalTime.wMinute;
	tmCurrTime.tm_sec  = stLocalTime.wSecond;
	//Saiki 20090603 Change ----->
	//if ( (timeCurrTime = mktime(&tmCurrTime)) == (time_t)-1 )
	//	  MessageBox(NULL, "mktime(&tmCurrTime) の実行に失敗しました", "NanoSpec", MB_OK);
//	LoadStringML(IDS_TMCURRTIME_FAILED, strMsg, "Failed to execute the mktime(&tmCurrTime)");
//	LoadStringML(IDS_TITLE_NANOSPEC, strTitle, "NanoSpec");
//	if ( (timeCurrTime = mktime(&tmCurrTime)) == (time_t)-1 ){
//		MessageBox(NULL, strMsg, strTitle, MB_OK);
//	}
	//Saiki 20090603 Change <-----
	timeCurrTime = mktime(&tmCurrTime);

	double dElapsedTime;
	if ( timeFileTime == (time_t)-1 || timeCurrTime == (time_t)-1 )
		dElapsedTime = DBL_MAX;		// 万が一、時刻取得を失敗した場合は、期限切れにする
	else
		dElapsedTime = difftime(timeCurrTime, timeFileTime);

	return ( ( dElapsedTime > dLifeTime * 60 ) ? ERR_EXPIRED : AVAILABLE ); // 秒数比較
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::Is2ndRefT1FileExist(LPCTSTR pszMainRecipeName)
{
	TCHAR szFilePath[MAX_PATH+1];

	sprintf(szFilePath, "%s%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DATAFILENAME2NDREFT1, DAT_EXT);
	return ::GetFileAttributes(szFilePath) != 0xFFFFFFFF;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL CSrRefFile::Is2ndRefT2FileExist(LPCTSTR pszMainRecipeName)
{
	TCHAR szFilePath[MAX_PATH+1];

	sprintf(szFilePath, "%s%s%s%s", g_tszData_Ref_Dir, pszMainRecipeName, DATAFILENAME2NDREFT2, DAT_EXT);
	return ::GetFileAttributes(szFilePath) != 0xFFFFFFFF;
}
