#include "stdafx.h"
#include <math.h>
#include "../../INC/Common.hxx"
#include "LogFile.h"
#include "ConfigFile.hxx"
#include "ScanRange.h"
#include "SrMeasure.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpec対応) <--

#define SCANDATADIRPATH 						CFG_SYSTEM_DIR
#define RECALFILENAME							_T("SystemSR.ini")

#define CCD_PIXEL_512					   (512)
#define CCD_PIXEL_1024					  (1024)

#define EXPOSURE_NUM					  (2400)

extern CLogFile* pLogFile;
extern BOOL bHwSimulation;
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
extern TCHAR g_tszProcDir[_MAX_PATH];		/* 呼出しプロセスのディレクトリ('\'付き)*/
extern TCHAR g_tszBaseDir[_MAX_PATH];		/* 基準ディレクトリ('\'付き)*/
extern void GetProcBaseDir(LPTSTR ptszProcDir, LPTSTR ptszBaseDir);
void AddAbsPath(LPTSTR ptszPath)
{
	TCHAR l_tszTempFName[_MAX_PATH + 2];
	memset(l_tszTempFName, 0, sizeof(l_tszTempFName));
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	_tcsncpy(l_tszTempFName, ptszPath, _MAX_PATH + 1);
	if (_MAX_PATH < (_tcslen(l_tszTempFName) + _tcslen(g_tszProcDir))) {
		_tcscpy(ptszPath, _T(""));
	} else {
		_stprintf(ptszPath, _T("%s%s"), g_tszProcDir, l_tszTempFName);
	}
}
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */

// --------------------------------------------------------------------------
// CSrMeasure
CSrMeasure::CSrMeasure()
{
	TRACE(_T("CSrMeasure::CSrMeasure()\n"));

	m_iCcdPixels = 0;
	::ZeroMemory(&m_SrConfig, sizeof(m_SrConfig));

	m_pSrHead = NULL;
	m_pCcdPosFitting = NULL;
	m_bRun = FALSE;
}

// --------------------------------------------------------------------------
// ~CSrMeasure
CSrMeasure::~CSrMeasure()
{
	TRACE(_T("CSrMeasure::~CSrMeasure()\n"));

}

// --------------------------------------------------------------------------
// InitInstance
BOOL CSrMeasure::InitInstance()
{
	TRACE(_T("CSrMeasure::InitInstance()\n"));

	char szVersion[256];
	char szSaveScanDataLog[32];

/* modified 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
//	  ::GetPrivateProfileString(INISECTION_CCDDATA, INIKEY_SAVESCANDATALOG, "FALSE", szSaveScanDataLog, sizeof(szSaveScanDataLog), CFG_DIR NANOSPEC_INIFILENAME);
/* modified 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ----------			  */
	TCHAR l_tszIniPath[_MAX_PATH];
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
// 2013.11.07 Bagus Mod (TohoSpec対応) -->
//	_stprintf(l_tszIniPath, _T("%s") CFG_DIR NANOSPEC_INIFILENAME, g_tszProcDir);

	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if(g_lAppNameType != APP_NAME_NANO){
		strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}
	_stprintf(l_tszIniPath, _T("%s%s%s"), CFG_DIR, g_tszProcDir, (LPCTSTR)strFilename);
// 2013.11.07 Bagus Mod (TohoSpec対応) <--
	::GetPrivateProfileString(INISECTION_CCDDATA, INIKEY_SAVESCANDATALOG, "FALSE", szSaveScanDataLog, sizeof(szSaveScanDataLog), l_tszIniPath);
/* modified 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */
	m_bSaveScanDataLog = ( strcmp(szSaveScanDataLog, "TRUE") == 0 ) ? TRUE : FALSE;

	ConfigFile_GetNanoSpecIni(&m_SrConfig, CONFIG_FILE_SR_CONFIG);

	// SRヘッドのインタフェースを選択する
	if ( bHwSimulation ) {
		m_pSrHead = new CSrHeadDesktop();
		pLogFile->Logging("Sr Head Type : CSrHeadDesktop");
	}
	else {
		if ( m_SrConfig.nHeadInterface == 1 ) {
			m_pSrHead = new CSrHeadSerial();
			pLogFile->Logging("Sr Head Type : CSrHeadSerial");
		}
		else {
			m_pSrHead = new CSrHeadParallel();
			pLogFile->Logging("Sr Head Type : CSrHeadParallel");
		}
	}

	// SRヘッドの初期化
	if ( !m_pSrHead->InitInstance() ) {
		pLogFile->Logging("Sr Head Initialize Error");
		pLogFile->Logging(m_pSrHead->GetLastError());
		return FALSE;
	}

	// SRヘッド（=NJPC025）のバージョン情報の取得
	if ( m_pSrHead->GetVersion(szVersion) ) {
		pLogFile->LoggingV("%s%s", "Sr Head Version : ", szVersion);
	}

	// SRヘッドの種類を選択する
	if ( m_SrConfig.nHeadType <=3 )	/* SR_HEAD_TYPE_LAH512, SR_HEAD_TYPE_LAH512_THICK, SR_HEAD_TYPE_LAH512_UV */
		m_iCcdPixels = CCD_PIXEL_512;
	else
		m_iCcdPixels = CCD_PIXEL_1024;


	// CCDキャリブレーションのデータ取得
	CCDCALIB cb;
	::ZeroMemory(&cb, sizeof(CCDCALIB));
	if ( !ReadCcdCalibDataFile(&cb) )
		return FALSE;

	// CCDキャリブレーションの作成
	m_pCcdPosFitting = new CCurveFitting(CCD_WCAL_MAX);
	if ( !m_pCcdPosFitting->SetParamData(cb) )
		return FALSE;

	return TRUE;
}

// --------------------------------------------------------------------------
// ExitInstance
void CSrMeasure::ExitInstance()
{
	TRACE(_T("CSrMeasure::ExitInstance()\n"));

	if ( m_pSrHead ) {
		m_pSrHead->ExitInstance();
		delete m_pSrHead;
		m_pSrHead = NULL;
	}

	if ( m_pCcdPosFitting ) {
		delete m_pCcdPosFitting;
		m_pCcdPosFitting = NULL;
	}
}

// --------------------------------------------------------------------------
// Measure
BOOL CSrMeasure::Measure(SCANDATA scanData[], int ccdData[], int iMeaSysStatus, const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CSrMeasure::Measure()\n"));

	const SR_SCAN_INFO& srScanInfo = pMeasProgInfo->ScanParams._SR;
	int* iCcdDataArray = ccdData;

	CScanRange scanRange(pMeasProgInfo);

	SCANDATA* pScanData = &(scanData[iMeaSysStatus]);
	::ZeroMemory(pScanData, sizeof(SCANDATA));

	double dIntegTime = GetIntegTime(iMeaSysStatus, pMeasProgInfo);
	BOOL bDark = ( iMeaSysStatus == MEASYS_DARK_T1 || iMeaSysStatus == MEASYS_DARK_T2 );

	m_bRun = TRUE;

	// UvMeasure or VisMeasure or Both!?	// @@@ シャッター開閉は、本対応で、chiefが担当になりそう
	// 測定波長の開始・終了を設定
	if ( scanRange.IsUvMeasure() ) {
		// UvMeasure
		CloseTransShutter(FALSE);												// Transmission Light Off
		if ( bDark )
			 CloseUvShutter();
		else
			 OpenUvShutter();

		double exValueUV;
		if ( srScanInfo.bEnhancedUv )
			exValueUV = dIntegTime * m_SrConfig.dUvGain;
		else
			exValueUV = dIntegTime;

// 2010.03.09 K.Matsuo -->
		pLogFile->Logging("Sr Head Measure Uv CcdScan Start");
// 2010.03.09 K.Matsuo <--

		if ( CcdScan(iCcdDataArray, exValueUV, TRUE) ) {
			GetScanData(pScanData->dScanData, CScanRange::MIN_UV_SCAN, CScanRange::MIN_VS_SCANL, iCcdDataArray);
			UvFinally();
// 2010.03.09 K.Matsuo -->
			pLogFile->Logging("Sr Head Measure Uv CcdScan Normal End");
// 2010.03.09 K.Matsuo <--
		}
		else {
			// CcdScan Error
			UvFinally();
// 2010.03.09 K.Matsuo -->
			pLogFile->Logging("Sr Head Measure Uv CcdScan Abnormal End");
// 2010.03.09 K.Matsuo <--
			goto ABEND;
		}

		// Both UvMeasure and VisMeasure!?
		if ( scanRange.IsVisMeasure() )
			;
		else
			goto NORMALEND;
	}

	// VisMeasure
//	switch ( pMeasProgInfo->ScanParams.hdr.wScanType )
//	{
//	case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
//	case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
//	case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
//		ChangeCcdShutter(CSrHead::FILTER_DARK);
//		if ( bDark )
//			CloseTransShutter();										// Transmission Light Off
//		else
//			OpenTransShutter(); 										// Transmission Light On
//		break;
//	default:
//		ChangeCcdShutter(srScanInfo.wOpticsFilterType);
//		break;
//	}

// 2010.03.09 K.Matsuo -->
	pLogFile->Logging("Sr Head Measure Vis CcdScan Start");
// 2010.03.09 K.Matsuo <--

	if ( CcdScan(iCcdDataArray, dIntegTime) ) {
// 2009.05.18 K.Matsuo Change -->
//				GetScanData(pScanData->dScanData, CScanRange::MIN_VS_SCANL, CScanRange::MAX_CS_SCAN, iCcdDataArray);
		GetScanData(pScanData->dScanData, CScanRange::MIN_VS_SCANL, scanRange.GetScanEndRange(), iCcdDataArray);
// 2009.05.18 K.Matsuo Change <--
//		VisFinally();
// 2010.03.09 K.Matsuo -->
		pLogFile->Logging("Sr Head Measure Vis CcdScan Normal End");
// 2010.03.09 K.Matsuo <--
		goto NORMALEND;
	}
	else {
		// CcdScan Error
//		VisFinally();
// 2010.03.09 K.Matsuo -->
		pLogFile->Logging("Sr Head Measure Vis CcdScan Abnormal End");
// 2010.03.09 K.Matsuo <--
		goto ABEND;
	}


NORMALEND:
	pScanData->iScanStart = scanRange.GetScanStartRange();
	pScanData->iScanEnd = scanRange.GetScanEndRange();
// 2014.04.04 bagus wavelength step added -->
	switch(m_SrConfig.nWavelengthStep){
	case 1: /* SR_WAVELENGTH_STEP_05_NM */
		pScanData->dWavelengthStep = 0.5;
		break;
	case 0: /* SR_WAVELENGTH_STEP_1_NM */
	default:
		pScanData->dWavelengthStep = 1.0;
		break;
	}
// 2014.04.04 bagus wavelength step added <--
	if ( m_bSaveScanDataLog )
		MakeScanDataLog(iCcdDataArray, pScanData->dScanData, pMeasProgInfo->ScanParams.hdr.wScanType, iMeaSysStatus);
	m_bRun = FALSE;
	return TRUE;

ABEND:
	m_bRun = FALSE;
	return FALSE;
}

//---------------------------------------------------------------------------
// GetIntegTime
double CSrMeasure::GetIntegTime(int iMeaSysStatus, const MEAS_PROG_INFO* pMeasProgInfo)
{
	double dIntegTime;

	switch ( iMeaSysStatus ) {
	case MEASYS_DARK_T1:
	case MEASYS_STDR_T1:
	case MEASYS_EXTR_T1:
		dIntegTime = pMeasProgInfo->ScanParams._SR.dIntegTime;
		break;
	case MEASYS_DARK_T2:
	case MEASYS_EXTR_T2:
		dIntegTime = pMeasProgInfo->Ref2nd.dIntegTime;
		break;
	case MEASYS_SAMP:
		if ( !pMeasProgInfo->Ref2nd.bMeasure )
			dIntegTime = pMeasProgInfo->ScanParams._SR.dIntegTime;
		else
			dIntegTime = pMeasProgInfo->Ref2nd.dIntegTime;
		break;
	default:
		ASSERT( FALSE );
		break;
	}

	return dIntegTime;
}

//---------------------------------------------------------------------------
// UvFinally										/// @@@ シャッター開閉は、本対応で、chiefが担当になりそう
void CSrMeasure::UvFinally()
{
	CloseUvShutter();
}

//---------------------------------------------------------------------------
// VisFinally
void CSrMeasure::VisFinally()
{
//	switch ( pMeasProgInfo->ScanParams.hdr.wScanType )
//	{
//	case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
//	case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
//	case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
//		CloseTransShutter(FALSE);
//		ChangeCcdShutter(CSrHead::FILTER_OPEN);
//		break;
//	default:
//		break;
//	}
}

//---------------------------------------------------------------------------
// ChangeCcdShutter
BOOL CSrMeasure::ChangeCcdShutter(WORD wFilter)
{
	TRACE(_T("CSrMeasure::ChangeCcdShutter()\n"));

	if ( !m_SrConfig.bAutoFilter )
		return FALSE;
	return m_pSrHead->ChangeCcdShutter(wFilter);
}

//---------------------------------------------------------------------------
// OpenUvShutter
BOOL CSrMeasure::OpenUvShutter()
{
	TRACE(_T("CSrMeasure::OpenUvShutter()\n"));

	const double dOpenDelay  = 0.8; //'[SEC]                                    // 動作完了を信じて、待つしかない

	if ( !m_SrConfig.bUv || !m_SrConfig.bUvAutoShutter )
		return FALSE;
	if ( !m_pSrHead->OpenUvShutter() )
		return FALSE;
	CTimer timer;
	timer.Restart(dOpenDelay);
	while ( !timer.IsTimeout() )
	{
		::Sleep(10);
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// CloseUvShutter
BOOL CSrMeasure::CloseUvShutter()
{
	TRACE(_T("CSrMeasure::CloseUvShutter()\n"));

	const double dCloseDelay = 0.5; //'[SEC]                                    // 動作完了を信じて、待つしかない

	if ( !m_SrConfig.bUv || !m_SrConfig.bUvAutoShutter )
		return FALSE;
	if ( !m_pSrHead->CloseUvShutter() )
		return FALSE;
	CTimer timer;
	timer.Restart(dCloseDelay);
	while ( !timer.IsTimeout() )
	{
		::Sleep(10);
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// OpenTransShutter
BOOL CSrMeasure::OpenTransShutter()
{
	TRACE(_T("CSrMeasure::OpenTransShutter()\n"));

	const double dOpenDelay = 0.5; //'[SEC]                                     // 動作完了を信じて、待つしかない

	if ( !m_SrConfig.bTransmittance )
		return FALSE;
	if ( !m_pSrHead->OpenTransShutter() )
		return FALSE;
	CTimer timer;
	timer.Restart(dOpenDelay);
	while ( !timer.IsTimeout() )
	{
		::Sleep(10);
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// CloseTransShutter
BOOL CSrMeasure::CloseTransShutter(BOOL bWait/*=TRUE*/)
{
	TRACE(_T("CSrMeasure::CloseTransShutter()\n"));

	const double dCloseDelay = 0.5; //'[SEC]                                    // 動作完了を信じて、待つしかない

	if ( !m_SrConfig.bTransmittance )
		return FALSE;
	if ( !m_pSrHead->CloseTransShutter() )
		return FALSE;
	if ( !bWait )
		return TRUE;
	CTimer timer;
	timer.Restart(dCloseDelay);
	while ( !timer.IsTimeout() )
	{
		::Sleep(10);
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// CcdScan
BOOL CSrMeasure::CcdScan(int iCcdDataArray[], double dIntegTime, BOOL bProhibitNotify/*=FALSE*/)
{
	TRACE(_T("CSrMeasure::CcdScan()\n"));

	// iCcdDataArray[]にCcdScan結果を編集する
	int iExposure;
	iExposure = (int)(dIntegTime * EXPOSURE_NUM);
/* added 2016.02.24 hmenjo バッチ処理 ---------- { ---------- */
	if (1 == g_lBatchFlag) {
		/* バッチ要求フラグを２にします．	*/
		g_lBatchFlag = 2;
		pLogFile->Logging("CSrHeadDesktop::CcdScan() for Batch.");
		CSrHeadDesktop l_pSrHeadDesktop;
		return l_pSrHeadDesktop.CcdScan(iCcdDataArray, m_iCcdPixels, iExposure, 1, bProhibitNotify);
	} else {
		/* バッチ要求フラグをクリア	*/
		g_lBatchFlag = 0;
	}
/* added 2016.02.24 hmenjo バッチ処理 ---------- } ---------- */
	return m_pSrHead->CcdScan(iCcdDataArray, m_iCcdPixels, iExposure, 1, bProhibitNotify);
}

// 2014.04.04 bagus wavelength step modified -->
//---------------------------------------------------------------------------
// GetScanData
void CSrMeasure::GetScanData(double* DstArray, int MinTRWave, int MaxTRWave, const int iCcdDataArray[])
{
	TRACE(_T("CSrMeasure::GetScanData()\n"));

	switch(m_SrConfig.nWavelengthStep){
	case 1: /* SR_WAVELENGTH_STEP_05_NM */
		GetScanData_Step_05nm(DstArray, MinTRWave, MaxTRWave, iCcdDataArray);
		break;
	case 0: /* SR_WAVELENGTH_STEP_1_NM */
	default:
		GetScanData_Step_1nm(DstArray, MinTRWave, MaxTRWave, iCcdDataArray);
		break;
	}
}

//---------------------------------------------------------------------------
// GetScanData_Step_1nm
void CSrMeasure::GetScanData_Step_1nm(double* DstArray, int MinTRWave, int MaxTRWave, const int iCcdDataArray[])
{
	TRACE(_T("CSrMeasure::GetScanData_Step_1nm()\n"));

	// 測定波長全域の光量算出関数
	// 測定波長から実際の波長ごとに対応した光量を算出（1nm毎）
	// ・測定波長に対する実際のディテクタ位置は、波長iPix = m_ccdPosFitting.GetFitValue(double(p))で求める
	// ・実際の波長の光量は、2点線形加重算出法で求める
	//	 （3点比率で求め加重した2点算出し、その2点の線形比率で加重する）
	int iPix;
	double dPix;
	double ddPix;
	double ddm;
	double ddp;
	double dData;
	int iIndex;

	for ( int p = MinTRWave; p < MaxTRWave+1; p++ ) {
		dPix = m_pCcdPosFitting->GetFitValue(double(p));
		iPix = (int)dPix;
// 加重平均計算の関係で、インデックス参照が不正な位置になるので、端の１つ内側までしか利用しないように修正
//		  if ( iPix <= 0 )
//			  dData = iCcdDataArray[0];
//		  else if ( iPix >= m_iCcdPixels )
//			  dData = iCcdDataArray[m_iCcdPixels-1];
		if ( iPix <= 1 )
			dData = iCcdDataArray[1];
		else if ( iPix >= m_iCcdPixels -1 )
			dData = iCcdDataArray[m_iCcdPixels-2];
		else
		{
			ddPix = dPix - (double)iPix;
			ddm = iCcdDataArray[iPix-1] * 0.5 + (iCcdDataArray[iPix] + iCcdDataArray[iPix-2]) * 0.25;
			ddp = iCcdDataArray[iPix] * 0.5 + (iCcdDataArray[iPix-1] + iCcdDataArray[iPix+1]) * 0.25;
			dData = ddm * (1.0 - ddPix) + ddp * ddPix;
		}

		iIndex = p - MinTRWave;
		DstArray[iIndex] = dData;
	}
}

//---------------------------------------------------------------------------
// GetScanData_Step_05nm
void CSrMeasure::GetScanData_Step_05nm(double* DstArray, int MinTRWave, int MaxTRWave, const int iCcdDataArray[])
{
	TRACE(_T("CSrMeasure::GetScanData_Step_05nm()\n"));

	// 測定波長全域の光量算出関数
	// 測定波長から実際の波長ごとに対応した光量を算出（0.5nm毎）
	// ・測定波長に対する実際のディテクタ位置は、波長iPix = m_ccdPosFitting.GetFitValue(double(p))で求める
	// ・実際の波長の光量は、2点線形加重算出法で求める
	//	 （3点比率で求め加重した2点算出し、その2点の線形比率で加重する）
	int iPix;
	double dPix;
	double ddPix;
	double ddm;
	double ddp;
	double dData;
	int iIndex;

	double p = (double)MinTRWave;
	while ( p <= (double)MaxTRWave ) {
		dPix = m_pCcdPosFitting->GetFitValue(p);
		iPix = (int)dPix;
// 加重平均計算の関係で、インデックス参照が不正な位置になるので、端の１つ内側までしか利用しないように修正
//		  if ( iPix <= 0 )
//			  dData = iCcdDataArray[0];
//		  else if ( iPix >= m_iCcdPixels )
//			  dData = iCcdDataArray[m_iCcdPixels-1];
		if ( iPix <= 1 )
			dData = iCcdDataArray[1];
		else if ( iPix >= m_iCcdPixels -1 )
			dData = iCcdDataArray[m_iCcdPixels-2];
		else
		{
			ddPix = dPix - (double)iPix;
			ddm = iCcdDataArray[iPix-1] * 0.5 + (iCcdDataArray[iPix] + iCcdDataArray[iPix-2]) * 0.25;
			ddp = iCcdDataArray[iPix] * 0.5 + (iCcdDataArray[iPix-1] + iCcdDataArray[iPix+1]) * 0.25;
			dData = ddm * (1.0 - ddPix) + ddp * ddPix;
		}

		iIndex = (int)((p - (double)MinTRWave) * 2);
		DstArray[iIndex] = dData;

		p += 0.5;
	}
}
// 2014.04.04 bagus wavelength step modified <--

//---------------------------------------------------------------------------
// IsDiscrete
BOOL CSrMeasure::IsDiscrete(WORD wScanType)
{
	TRACE(_T("CSrMeasure::IsDiscrete(WORD wScanType)\n"));

	// VB名称そのまま。測定波長が飛び石指定の意味でしょうか
	BOOL bRet = FALSE;
	switch ( wScanType )
	{
//	case MEAS_PROG_TYPE_SR_THICKNESS:
	case MEAS_PROG_TYPE_SR_REFLECTANCE:
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
		bRet = TRUE;
		break;
	}
	return bRet;
}

//---------------------------------------------------------------------------
// ReadCcdCalibDataFile
BOOL CSrMeasure::ReadCcdCalibDataFile(CCDCALIB* cb)
{
	TRACE(_T("CSrMeasure::ReadCcdCalibDataFile()\n"));
#if 0
	// CCD キャリブレーションファイルを読込む
	//
	// (* sample *)
	// "ccd.ini"
	// 48.3742395602167 	  253.652nm-Hg
	// 97.7766436282545 	  312.566nm-Hg
	// 141.427878567949 	  365.015nm-Hg
	// 289.786262163892 	  546.074nm-Hg
	// 323.006821509451 	  587.092nm-Kr
	// 462.947067711502 	  760.154nm-Kr
	// 483.565708423033 	  785.482nm-Kr

	TCHAR szFilename[_MAX_PATH];
	const int nMaxSize = 128;
	TCHAR szLine[nMaxSize];

	cb->datanum = 0;

	FILE* fp;

	_stprintf(szFilename, _T("%s%s"), CFG_SYSTEM_DIR, RECALFILENAME);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
	AddAbsPath(szFilename);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */
	if ( (fp = fopen(szFilename, _T("r"))) == NULL )
		return FALSE;

	while ( _fgetts(szLine, nMaxSize, fp) != NULL && cb->datanum < CALIBFILEMAXLINE )
	{
		TCHAR buf[nMaxSize];
		_tcscpy(buf, szLine);
		TCHAR* szBuff = _tcstok(buf, _T(" "));
		cb->value[cb->datanum] = atof(szBuff);

		TCHAR* tokenWave = _tcsstr(szLine, _T(" "));
		if ( tokenWave == NULL) {
			fclose(fp);
			return FALSE;												// フォーマットエラー
		}
		while ( *tokenWave == _TCHAR(' ') ) 							// スペース文字を読み飛ばす
			tokenWave = _tcsinc(tokenWave);

		TCHAR* psz = _tcsstr(szLine, _T("nm")); 						// 波長が、nmの単位付きで書かれている場合、NULLにする
		if ( psz != NULL )
			*psz = NULL;
		cb->point[cb->datanum] = atof(tokenWave);
		cb->datanum++;
	}
	fclose(fp);
	return TRUE;
#endif

	// [CCD ARRAY]
	// CCDArray1_Item=253.652nm-Hg
	// CCDArray1_Data=48.3742395602167
	// CCDArray2_Item=312.566nm-Hg
	// CCDArray2_Data=97.7766436282545
	// CCDArray3_Item=365.015nm-Hg
	// CCDArray3_Data=141.427878567949
	// CCDArray4_Item=546.074nm-Hg
	// CCDArray4_Data=289.786262163892
	// CCDArray5_Item=587.092nm-Kr
	// CCDArray5_Data=323.006821509451
	// CCDArray6_Item=760.154nm-Kr
	// CCDArray6_Data=462.947067711502
	// CCDArray7_Item=785.482nm-Kr
	// CCDArray7_Data=483.565708423033
/* added 2024.01.08 hmenjo CCD ARRAY 追加 ---------- { ---------- */
	// CCDArray8_Item=907.973nm
	// CCDArray8_Data=600.0000000000
/* added 2024.01.08 hmenjo CCD ARRAY 追加 ---------- } ---------- */

	TCHAR szFilename[_MAX_PATH];
	const int nMaxSize = 128;
	TCHAR szBuff[nMaxSize];
	TCHAR* pszIniSection = "CCD ARRAY";
	TCHAR* psz;

	_stprintf(szFilename, _T("%s%s"), CFG_SYSTEM_DIR, RECALFILENAME);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
	AddAbsPath(szFilename);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */

	cb->datanum = 0;

	if ( m_SrConfig.nHeadType == 1 ||
		 m_SrConfig.nHeadType == 2 )	/* SR_HEAD_TYPE_LAH512, SR_HEAD_TYPE_LAH512_UV */
	{
		// MAX SCAN RANGE が800nmのHEADのはずなので、253.652nm-HgからCCDキャリブレーションがされているはず
		::GetPrivateProfileString(pszIniSection, "CCDArray1_Item", _T("253.652nm-Hg"), szBuff, sizeof(szBuff), szFilename);
		psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
		if ( psz != NULL )
			*psz = NULL;
		cb->point[cb->datanum] = atof(szBuff);
		::GetPrivateProfileString(pszIniSection, "CCDArray1_Data", _T("48.3742395602167"), szBuff, sizeof(szBuff), szFilename);
		cb->value[cb->datanum] = atof(szBuff);
		cb->datanum++;
	}

	::GetPrivateProfileString(pszIniSection, "CCDArray2_Item", _T("312.566nm-Hg"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray2_Data", _T("97.7766436282545"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;

	::GetPrivateProfileString(pszIniSection, "CCDArray3_Item", _T("365.015nm-Hg"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray3_Data", _T("141.427878567949"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;

	::GetPrivateProfileString(pszIniSection, "CCDArray4_Item", _T("546.074nm-Hg"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray4_Data", _T("289.786262163892"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;

	::GetPrivateProfileString(pszIniSection, "CCDArray5_Item", _T("587.092nm-Kr"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray5_Data", _T("323.006821509451"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;

	::GetPrivateProfileString(pszIniSection, "CCDArray6_Item", _T("760.154nm-Kr"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray6_Data", _T("462.947067711502"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;

	::GetPrivateProfileString(pszIniSection, "CCDArray7_Item", _T("785.482nm-Kr"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray7_Data", _T("483.565708423033"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;
/* added 2024.01.08 hmenjo CCD ARRAY 追加 ---------- { ---------- */

	::GetPrivateProfileString(pszIniSection, "CCDArray8_Item", _T("907.973nm"), szBuff, sizeof(szBuff), szFilename);
	psz = _tcsstr(szBuff, _T("nm"));						 // 波長が、nmの単位付きで書かれている場合、NULLにする
	if ( psz != NULL )
		*psz = NULL;
	cb->point[cb->datanum] = atof(szBuff);
	::GetPrivateProfileString(pszIniSection, "CCDArray8_Data", _T("600.0000000000"), szBuff, sizeof(szBuff), szFilename);
	cb->value[cb->datanum] = atof(szBuff);
	cb->datanum++;
/* added 2024.01.08 hmenjo CCD ARRAY 追加 ---------- } ---------- */

	return TRUE;
}

//---------------------------------------------------------------------------
// MakeScanDataLog
void CSrMeasure::MakeScanDataLog(const int iCcdDataArray[], const double* DstArray, WORD wScanType, int iMeaSysStatus)
{
	TRACE(_T("CSrMeasure::MakeScanDataLog()\n"));

	LPCTSTR MEAS_PROG_TYPE_SR_ITEM[] = {
		_T("Thickness (n,k)"),
		_T("Reflectance"),
		_T("Transmittance"),
		_T("Chromaticity (Reflectance)"),
		_T("Chromaticity (Transmittance)"),
		_T("Optical Density"),
	};

	LPCTSTR pszComment[] = {
		_T("Dark_T1"),
		_T("1stReference_T1"),
		_T("2ndReference_T1"),
		_T("Dark_T2"),
		_T("2ndReference_T2"),
		_T("Sample")
	};

	TCHAR szFilename[_MAX_PATH];
	FILE* fp;

	// CCDスキャンデータログを作成する
	GetScanDataFilename(szFilename);
	if ( (fp = fopen(szFilename, _T("a"))) == NULL )
		return;

	_ftprintf(fp, _T("MTYPE: %s\n"), MEAS_PROG_TYPE_SR_ITEM[wScanType]);				// ヘッダ情報（測定種類）
	_ftprintf(fp, _T("PHASE: %s\n"), pszComment[iMeaSysStatus]);						// ヘッダ情報（DARK処理、REFERENCE処理、SAMPLE処理）
	for ( int iPixIdx = 0; iPixIdx < m_iCcdPixels; iPixIdx++ ) {						// CCD情報
		_ftprintf(fp, _T("%3d%s%5d\n"), iPixIdx+1, _T(" "), iCcdDataArray[iPixIdx]);
	}

	_ftprintf(fp, _T("\n\n"));
	_ftprintf(fp, _T("below [nm]\n"));													// 計算結果
	for ( int i = 0; i < SCANDATA_POINT_MAX; i++ ) {
		_ftprintf(fp, _T("%4d%s%lf\n"), i+1, _T(" "), DstArray[i]);
	}
	fclose(fp);
}

//---------------------------------------------------------------------------
// GetScanDataFilename
void CSrMeasure::GetScanDataFilename(LPTSTR pszFilename)
{
	TRACE(_T("CSrMeasure::GetScanDataFilename()\n"));

	// CCDスキャンデータログの格納フォルダを作成（日付）
	// CCDスキャンデータログの"パス名＋ファイル名.txt"を返却
	TCHAR szDate[] = _T("yyyymmdd");
	TCHAR szTime[] = _T("hhmmss");
	TCHAR szDateDirPath[_MAX_PATH];

/* modified 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
//	  SECURITY_ATTRIBUTES sa;
//	  sa.lpSecurityDescriptor = NULL;
//	  ::CreateDirectory(DATA_MEASUREMENTDATA_DIR, &sa);
/* modified 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ----------			  */
	GetProcBaseDir(g_tszProcDir, g_tszBaseDir);
	TCHAR l_tszMeasDataDir[_MAX_PATH];
	_stprintf(l_tszMeasDataDir, _T("%s") DATA_MEASUREMENTDATA_DIR, g_tszProcDir);
	SECURITY_ATTRIBUTES sa;
	sa.lpSecurityDescriptor = NULL;
	::CreateDirectory(l_tszMeasDataDir, &sa);
/* modified 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */

	SYSTEMTIME systime;
	::GetLocalTime(&systime);

	_stprintf(szDate, _T("%04d%02d%02d"), systime.wYear, systime.wMonth, systime.wDay);
	_stprintf(szTime, _T("%02d%02d%02d"), systime.wHour, systime.wMinute, systime.wSecond);

	_stprintf(szDateDirPath, _T("%s%s%s"), DATA_MEASUREMENTDATA_DIR, szDate, _T("\\"));
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- { ---------- */
	AddAbsPath(szDateDirPath);
/* added 2009.07.07 hmenjo dll 相対パス対応 MeaSys.dll ---------- } ---------- */
	::CreateDirectory(szDateDirPath, &sa);

	_stprintf(pszFilename, _T("%s%s%s"), szDateDirPath, szTime, TXT_EXT);
}

// --------------------------------------------------------------------------
// GTrReferecneMeasure
BOOL CSrMeasure::GTrReferecneMeasure(SCANDATA scanData[], int ccdData[], int iMeaSysStatus, const MEAS_PROG_INFO* pMeasProgInfo)
{
	TRACE(_T("CSrMeasure::GTrReferecneMeasure()\n"));

	// iMeaSysStatus特殊化

	const SR_SCAN_INFO& srScanInfo = pMeasProgInfo->ScanParams._SR;
	int* iCcdDataArray = ccdData;

	CScanRange scanRange(pMeasProgInfo);

	SCANDATA* pScanData = &(scanData[iMeaSysStatus]);
	::ZeroMemory(pScanData, sizeof(SCANDATA));

	double dIntegTime = pMeasProgInfo->ScanParams._SR.dIntegTime;

	m_bRun = TRUE;

	if ( CcdScan(iCcdDataArray, dIntegTime) ) {
		GetScanData(pScanData->dScanData, CScanRange::MIN_VS_SCANL, scanRange.GetScanEndRange(), iCcdDataArray);
		pScanData->iScanStart = scanRange.GetScanStartRange();
		pScanData->iScanEnd = scanRange.GetScanEndRange();
// 2014.04.04 bagus wavelength step added -->
		switch(m_SrConfig.nWavelengthStep){
		case 1: /* SR_WAVELENGTH_STEP_05_NM */
			pScanData->dWavelengthStep = 0.5;
			break;
		case 0: /* SR_WAVELENGTH_STEP_1_NM */
		default:
			pScanData->dWavelengthStep = 1.0;
			break;
		}
// 2014.04.04 bagus wavelength step added <--
		m_bRun = FALSE;
		return TRUE;
	}
	else {
		// CcdScan Error
		m_bRun = FALSE;
		return FALSE;
	}
}

int CSrMeasure::GetWaveLengthStepFromConfig()
{
	return m_SrConfig.nWavelengthStep;
}

