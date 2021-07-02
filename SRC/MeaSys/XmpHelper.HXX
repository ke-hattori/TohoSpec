#pragma once

#include <stdlib.h>
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\ConfigFile.hxx"
#include "..\\..\\INC\\MeaSys.hxx"
#include "SrHead.h"
//#include "Mojiretsu.h"

#define XMPSTRINGLENMAX 				(20)							// ADAPからのRESULTデータのアイテム最大文字数
#define XMPREGISTARMAX					(256)							// 最大登録件数（＝parm.datのデータ個数）

#define MAXNAMELEN						(45)

class CXmpHelper
{
public:
	// constructor
	CXmpHelper() : m_iMaxCnt(0), m_pLayers(NULL)
	{
		::ZeroMemory(&m_nmdParams, sizeof(m_nmdParams));
	}

	// destructor
	virtual ~CXmpHelper()
	{
		if ( m_pLayers ) {
			delete [] m_pLayers;
			m_pLayers = NULL;
		}
	}

	// ストラテジー番号から該当のparms.datを取得する
	BOOL LoadParams(LPCTSTR pszStrategyEntry)
	{
		return LoadParams(GetStrategyNo(pszStrategyEntry));
	}

	BOOL LoadParams(int iStrategyNo)
	{
		TCHAR szPath[_MAX_PATH];

		m_iMaxCnt = 0;
		GetAdapParmsFilePath(szPath, iStrategyNo);
		return LoadAdapParamsFile(szPath);
	}

	// ファイル「analysis2.txt」に記述してあるコマンド "fitget(xxxx)" を取得する
	BOOL GetScriptFileDescriptDdeTextItem(LPTSTR pszDdeTextItem, int& iStartWaveLen, int& iEndWaveLen, int& iColorFilter, LPCTSTR pszStrategyEntry)
	{
		return GetScriptFileDescriptDdeTextItem(pszDdeTextItem, iStartWaveLen, iEndWaveLen, iColorFilter, GetStrategyNo(pszStrategyEntry));
	}

	BOOL GetScriptFileDescriptDdeTextItem(LPTSTR pszDdeTextItem, int& iStartWaveLen, int& iEndWaveLen, int& iColorFilter, int iStrategyNo)
	{
		TCHAR szPath[_MAX_PATH];
		TCHAR szBuff[256];
		TCHAR szDelimiter[] = _T("=,");
		LPTSTR token;
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 -->
//		TCHAR szCommand[1024];
//		TCHAR szAdapRename[ADAPRESULTSTRINGLENMAX + 1];
//		TCHAR szAdapUnit[ADAPRESULTSTRINGLENMAX + 1];
//		LPTSTR pDest;
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 <--
// 2009.09.17 K.Matsuo 波長レンジ取得方法変更 -->
		BOOL bFoundNormalFit = FALSE;
// 2009.09.17 K.Matsuo 波長レンジ取得方法変更 <--
		if ( !LoadParams(iStrategyNo) )
			return FALSE;
		GetAdapScriptFilePath(szPath, iStrategyNo);

// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 -->
//		BOOL bFindFitGetCommand = FALSE;
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 <--
		const int nMaxSize = 1024;
		TCHAR szLine[nMaxSize];
		FILE* fp;

		// デフォルト値設定
		SR_CONFIG srConfig;
		ConfigFile_GetNanoSpecIni(&srConfig, CONFIG_FILE_SR_CONFIG);

		if ( srConfig.nHeadType == 5 ) {	/* SR_HEAD_TYPE_LAH1024_SS */ 
			iStartWaveLen = 380;
			iEndWaveLen = 850;
		}
		else {
			iStartWaveLen = 400;
			iEndWaveLen = 800;
		}

		iColorFilter = CSrHead::FILTER_OPEN;

		if ( (fp = fopen(szPath, _T("r"))) == NULL )
			return FALSE;
		while ( fgets(szLine, nMaxSize, fp) ) {
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 -->
//			if ( _tcsstr(szLine, _T("fitget(")) && !bFindFitGetCommand ) {
//				_tcscpy(szCommand, szLine); 							// ex. "fitget(Thick1=thick.1,Thick2=thick.2,MSE)"
//				bFindFitGetCommand = TRUE;
//			}
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 <--
// 2009.09.17 K.Matsuo 波長レンジ取得方法変更 -->
//			  if ( _tcsstr(szLine, _T("//nanostartwave=")) ) {
//				  _tcscpy(szBuff, &szLine[strlen(_T("//nanostartwave="))]);
//				  token = _tcstok(szBuff, _T(" \n"));
//				  iStartWaveLen = _ttoi(token);
//			  }
//			  if ( _tcsstr(szLine, _T("//nanoendwave=")) ) {
//				  _tcscpy(szBuff, &szLine[strlen(_T("//nanoendwave="))]);
//				  token = _tcstok(szBuff, _T(" \n"));
//				  iEndWaveLen = _ttoi(token);
//			  }
			if ( _tcsstr(szLine, _T("$2")) ) {
				// ノーマルFITの識別子発見
				bFoundNormalFit = TRUE;
			}
			if ( _tcsstr(szLine, _T("exprange(")) ) {
				// ノーマルFITの識別子発見済みであるか！？
				if ( bFoundNormalFit ) {
					_tcscpy(szBuff, &szLine[strlen(_T("exprange(wvlstart="))]);
					token = _tcstok(szBuff, _T(","));
					iStartWaveLen = _ttoi(token);
					token = _tcstok(NULL, _T("="));
//					ASSERT(strcmp(token, "wvlend") == 0);
					if ( strcmp(token, "wvlend") != 0 )
						return FALSE;
					token = _tcstok(NULL, _T(","));
					iEndWaveLen = _ttoi(token);
					bFoundNormalFit = FALSE;
				}
			}
// 2009.09.17 K.Matsuo 波長レンジ取得方法変更 <--
			if ( _tcsstr(szLine, _T("//nanofilter=")) ) {
				_tcscpy(szBuff, &szLine[strlen(_T("//nanofilter="))]);
				token = _tcstok(szBuff, _T(" \n"));
				LPCTSTR psz[] = {"", "open", "dark", "yellow", "orange", "pos3"};
				for ( int i = 0; i < sizeof(psz) / sizeof(psz[0] ); i++ ) {
					if ( _tcscmp(token, psz[i]) == 0 ) {
							iColorFilter = i;
							break;
					}
				}
			}
		}
		fclose(fp);

// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 -->
//		if ( !bFindFitGetCommand )
//			  return FALSE;
//
//		szBuff[0] = _TCHAR('\0');
//		token = _tcstok(&szCommand[strlen(_T("fitget("))], szDelimiter);
//		if ( GetAdapRename(szAdapRename, szAdapUnit, token) )
//			_tcscat(szBuff, szAdapRename);
//		else
//			_tcscat(szBuff, token);
//		if ( (token = _tcstok(NULL, szDelimiter)) == NULL ) {
//			pszDdeTextItem[0] = _TCHAR('\0');
//			return FALSE;
//		}
//		while ( (token = _tcstok(NULL, szDelimiter)) != NULL ) {
//			_tcscat(szBuff, _T(","));
//			if ( (pDest = _tcsstr(token, _T(")"))) != NULL ) {
//				  *pDest = _TCHAR('\0');
//				if ( GetAdapRename(szAdapRename, szAdapUnit, token) )
//					_tcscat(szBuff, szAdapRename);
//				else
//					_tcscat(szBuff, token);
//				_tcscpy(pszDdeTextItem, szBuff);
//				return TRUE;
//			  }
//
//			if ( GetAdapRename(szAdapRename, szAdapUnit, token) )
//				_tcscat(szBuff, szAdapRename);
//			else
//				_tcscat(szBuff, token);
//
//			if ( (token = _tcstok(NULL, szDelimiter)) == NULL ) {
//				pszDdeTextItem[0] = _TCHAR('\0');
//				return FALSE;
//			}
//		}
//
//		return FALSE;
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 <--

// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 -->
		// parms.datの順番でDdeTextが報告されてくる。fitgetの順番ではないので注意
		_tcscpy(szBuff, m_searchTbl[0].szAdapRename);
		for ( int i = 1; i < m_iMaxCnt; i++ ) {
			_tcscat(szBuff, _T(","));
			_tcscat(szBuff, m_searchTbl[i].szAdapRename);
		}

		_tcscpy(pszDdeTextItem, szBuff);

		return TRUE;
// 2009.06.05 K.Matsuo 多層膜時のADAP Dde通信での報告項目順の対応 <--
	}

	// ADAPからのRESULTデータから取得したアイテム名をもとに、表示名と単位を取得する
	BOOL GetAdapRename(LPTSTR pszAdapRename, LPTSTR pszAdapUnit, LPCTSTR pszItemName)
	{
		for ( int i = 0; i < m_iMaxCnt; i++ ) {
			if ( _tcscmp(m_searchTbl[i].szAdapName, pszItemName) == 0 ) {
				_tcscpy(pszAdapRename, m_searchTbl[i].szAdapRename);
				_tcscpy(pszAdapUnit, m_searchTbl[i].szAdapUnit);
				return TRUE;
			}
		}
		return FALSE;
	}

// 2009.09.11 K.Matsuo SE -->
BOOL LoadNMD(LPCTSTR pszStrategyEntry)
{
	return LoadNMD(GetStrategyNo(pszStrategyEntry));
}

BOOL LoadNMD(int iStrategyNo)
{
	CString strBuffer;
	char szFilePath[MAX_PATH];
	char szDirPath[MAX_PATH];

	int nDummy;

	// CleanUp
	::ZeroMemory(&m_nmdParams, sizeof(m_nmdParams));
	if ( m_pLayers ) {
		delete [] m_pLayers;
		m_pLayers = NULL;
	}

	// 
	GetAdapStrategyFolderPath(szDirPath, iStrategyNo);
	sprintf(szFilePath, "%s%s", szDirPath, "\\analysis.nmd");

	TRY
	{
		CStdioFile stdioFile(szFilePath, CFile::modeRead);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nNumFilmLayers = atoi(strBuffer);

		m_pLayers = new FILM_LAYER_DATA[m_nmdParams.nNumFilmLayers + 1];

		//-------------------------------------------------
		// Get a pointer to the substrate film layer data.
		//-------------------------------------------------
		stdioFile.ReadString(strBuffer);
		strcpy(m_pLayers[0].strLayerFile, strBuffer.Mid(0, 13));

		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nLayerType = atoi(strBuffer);

		stdioFile.ReadString(strBuffer);
		strcpy(m_pLayers[0].strLayerDescript, strBuffer.Mid(0, MAXNAMELEN - 1));

		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nT_Var = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nNK_Var = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nNumNK_Vars = atoi(strBuffer);

		//-------------------------------------------------
		// store alloy parameters if material is an alloy.
		//-------------------------------------------------
		if ( m_pLayers[0].nLayerType == 3 ) {
			stdioFile.ReadString(strBuffer);
			m_pLayers[0].NomAlloy = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[0].MinAlloy = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[0].MaxAlloy = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[0].nGlobalAlloyFit = atoi(strBuffer);
		}
		
		//------------------------------------------------
		// Determine if this strategy uses the polarizer.
		//------------------------------------------------
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nGratingEnabled = atoi(strBuffer);
		if ( m_pLayers[0].nGratingEnabled < 0 )
			m_pLayers[0].nGratingEnabled = 0;
		if ( m_pLayers[0].nGratingEnabled )
			m_nmdParams.nGratingLayerCount = m_nmdParams.nGratingLayerCount + 1;

		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nGratingFillLayer = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nGratingSlices = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].fGratingSideWallAngle = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nGratingFitSideWallAngle = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nGratingHarmonics = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].fGratingPitch = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].fGratingLineWidth = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_pLayers[0].nGratingFitLineWidth = atoi(strBuffer);
		stdioFile.ReadString(strBuffer); // Dummy Read

		//--------------------------------
		// read parameters for each film.
		//--------------------------------
		for ( int nLayer = 1; nLayer < m_nmdParams.nNumFilmLayers + 1; nLayer++ ) {
			stdioFile.ReadString(strBuffer); // 128 bytes?
			strcpy(m_pLayers[nLayer].strLayerFile, strBuffer.Mid(0, 13));

			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nLayerType = atoi(strBuffer);

			stdioFile.ReadString(strBuffer); // 128 bytes?
			strcpy(m_pLayers[nLayer].strLayerDescript, strBuffer.Mid(0, MAXNAMELEN - 1)); //
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].LayerThick = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].LayerMinThick = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].LayerMaxThick = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nNumThickPts = atoi(strBuffer);

			//---------------------------------------------------
			// Convert thicknesses from nanometers to angrstoms
			//---------------------------------------------------
			m_pLayers[nLayer].LayerThick = m_pLayers[nLayer].LayerThick * 10;
			m_pLayers[nLayer].LayerMinThick = m_pLayers[nLayer].LayerMinThick * 10;
			m_pLayers[nLayer].LayerMaxThick = m_pLayers[nLayer].LayerMaxThick * 10;
			
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nT_Var = atoi(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nNK_Var = atoi(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nNumNK_Vars = atoi(strBuffer);
			
			//-------------------------------------------------
			// store alloy parameters if material is an alloy.
			//-------------------------------------------------
			
			if ( m_pLayers[nLayer].nLayerType == 3) {
				stdioFile.ReadString(strBuffer);
				m_pLayers[nLayer].NomAlloy = atof(strBuffer);
				stdioFile.ReadString(strBuffer);
				m_pLayers[nLayer].MinAlloy = atof(strBuffer);
				stdioFile.ReadString(strBuffer);
				m_pLayers[nLayer].MaxAlloy = atof(strBuffer);
				stdioFile.ReadString(strBuffer);
				m_pLayers[nLayer].nGlobalAlloyFit = atoi(strBuffer);
			}

			//------------------------------------
			// 09/14/2000 New grating parameters.
			//------------------------------------
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingEnabled = atoi(strBuffer);

			if (m_pLayers[nLayer].nGratingEnabled)
				m_nmdParams.nGratingLayerCount = m_nmdParams.nGratingLayerCount + 1;
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingFillLayer = atoi(strBuffer);

			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingSlices = atoi(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].fGratingSideWallAngle = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingFitSideWallAngle = atoi(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingHarmonics = atoi(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].fGratingPitch = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].fGratingLineWidth = atof(strBuffer);
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingFitLineWidth = atoi(strBuffer);
			stdioFile.ReadString(strBuffer); //Dummy Reading
		}

		// write advanced, acquisition, and searching parameters.
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nReflectWeight = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nFitGain = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nFitAOI = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobMSE_Cutoff = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nEnableGlobSearch = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nNonIdeality = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fMinWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fMaxWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.bHighAccuracy = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.Pol = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nCycles = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nEnableDB_Search = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nSearchDB_Always = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nNoModel = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nUseReflectData = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nNoM44_Data = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nAnalysisType = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nUseFTIR_Data = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nCheckPrevModel = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nCheckNomModel = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nUsePolarizer = atoi(strBuffer);
		if ( m_nmdParams.nUsePolarizer != 1 )
			m_nmdParams.nUsePolarizer = 0;

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nReturnTotalThick = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nScriptLocked = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nUseBurg = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		nDummy = atoi(strBuffer); // scab NumBurgPoles: not used.
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fF_Slope = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fF_Intercept = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nAdvProgNum = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fF_Index = atof(strBuffer);

		// the following 4 fields will be overwritten later by reding from another file
		// Peter 11-2-98

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nNK_Layers[0] = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fNK_WaveLengths[0] = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nNK_Layers[1] = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fNK_WaveLengths[1] = atof(strBuffer);
		
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fFTIR_MinWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fFTIR_MaxWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fFTIR_Resolution = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nFTIR_NumScans = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nIntegType = atoi(strBuffer);
		if ( m_nmdParams.nIntegType < 0 )
			m_nmdParams.nIntegType = 1; 			// default to short integration time.
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nDeriv_Freq = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fMSENormFactor = atof(strBuffer);

		stdioFile.ReadString(strBuffer);

		stdioFile.ReadString(strBuffer);
		m_nmdParams.fSE_MinWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fSE_MaxWaveLen = atof(strBuffer);

		// For frequency analysis.
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fSE_MinTau = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fSE_MaxTau = atof(strBuffer);

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nFitBandWidth = ( strBuffer == "True" ? 1 : 0 );

		stdioFile.ReadString(strBuffer);
		m_nmdParams.fBandWidth = atof(strBuffer);

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nPhotoResist = atoi(strBuffer); // use spectrometer filter.
		if ( m_nmdParams.nPhotoResist < 0 )
			m_nmdParams.nPhotoResist = 0;
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGlobal_R_Enabled = atoi(strBuffer);
		if ( m_nmdParams.nGlobal_R_Enabled < 0 )
			m_nmdParams.nGlobal_R_Enabled = 0;

		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobal_R_MinWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobal_R_MaxWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGlobal_SE_Enabled = atoi(strBuffer);
		if ( m_nmdParams.nGlobal_SE_Enabled < 0 )
			m_nmdParams.nGlobal_SE_Enabled = 0;
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobal_SE_MinWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobal_SE_MaxWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGlobal_FTIR_Enabled = atoi(strBuffer);
		if ( m_nmdParams.nGlobal_FTIR_Enabled < 0 )
			m_nmdParams.nGlobal_FTIR_Enabled = 0;
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobal_FTIR_MinWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGlobal_FTIR_MaxWaveLen = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nSR_PerformanceLevel = atoi(strBuffer);

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nSECloseShuttersDuringSR = atoi(strBuffer);
		if ( m_nmdParams.nSECloseShuttersDuringSR < 0 )
			m_nmdParams.nSECloseShuttersDuringSR = 0;
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nSEOpenUVShuttersDuringSE = atoi(strBuffer);
		if ( m_nmdParams.nSEOpenUVShuttersDuringSE < 0 )
			m_nmdParams.nSEOpenUVShuttersDuringSE = 0;

		stdioFile.ReadString(strBuffer);
		m_nmdParams.fGratingPitch = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGratingNumHarmonicsTE = atoi(strBuffer);
		if ( m_nmdParams.nGratingNumHarmonicsTE < 0 )
			m_nmdParams.nGratingNumHarmonicsTE = 0;

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGratingNumHarmonicsTM = atoi(strBuffer);
		if ( m_nmdParams.nGratingNumHarmonicsTM < 0 )
			m_nmdParams.nGratingNumHarmonicsTM = 0;

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGratingModel = atoi(strBuffer);

		//----------------------------------------------
		// Try to read the optical profiler parameters.
		//----------------------------------------------
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nReferenceBeam = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nMeasureBeam = atoi(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.dMetalReferenceN = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.dMetalReferenceK = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.dMetalMeasureN = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.dMetalMeasureK = atof(strBuffer);
		stdioFile.ReadString(strBuffer);
		m_nmdParams.nReflectanceScanCount = atoi(strBuffer);

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nProfilerMeasureMode = atoi(strBuffer); // if TheStrategy.Analysistype=6 (1 =regular, 2=relative, 3=both , -1 not in use
		if ( m_nmdParams.nProfilerMeasureMode <= 0 )
			m_nmdParams.nProfilerMeasureMode = 0;
		if ( m_nmdParams.nProfilerMeasureMode ) {
			m_nmdParams.nUseLaserProfiler = TRUE;
			if ( (m_nmdParams.nProfilerMeasureMode == 1) || (m_nmdParams.nProfilerMeasureMode == 3) )
				m_nmdParams.bExpectLaserProfilerGraph = TRUE;
		}

		//-------------------------------------------------------------
		// Parameters after this point are from a newer version of the
		// ANALYSIS.NMD file, and may not exist in older versions
		// of the file.
		//-------------------------------------------------------------
		if ( m_nmdParams.nAnalysisType == 6 ) {
// ファイル形式不明
//			ASSERT(FALSE);
			return FALSE;
//			  Input #fp, m_nmdParams.fProfilerRDishHighestPercent, m_nmdParams.fProfilerRDishLowestPercent
//			  for ( int i = 0; i < 10; i++ )
//				  Input #fp, m_nmdParams.nProfilerGroup(i)
//			  for ( i = 0; i < 10 )
//				  Input #fp, m_nmdParams.nProfilerReportGroup(i)
		}
		else {
			for ( int i = 0; i < 3; i++ )
				stdioFile.ReadString(strBuffer); //Dummy reading
		}
		//------------------------------------------------
		// Read the string for Material name 1, 2, and 3.
		//------------------------------------------------
		stdioFile.ReadString(strBuffer); //128bytes?

		for ( int i = 0; i < 3; i++ )
			stdioFile.ReadString(strBuffer); //Dummy Read

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nPolarizerProgramType = atoi(strBuffer); // 0 = TE, 1 = TM, 3 = multi-angle scan

		if ( m_nmdParams.nUsePolarizer ) {
			switch ( m_nmdParams.nPolarizerProgramType ) {
			case 2:
				m_nmdParams.nScanAngleCount = 2;
				break;
			case 3:
				m_nmdParams.nScanAngleCount = 3;
				break;
			default:
				m_nmdParams.nScanAngleCount = 1;
				break;
			}
			m_nmdParams.fTM_TE_AngleOffset = ( m_nmdParams.nPolarizerProgramType == 1 ? 90.0 : 0.0 );
		}

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nPolarizerLibrarySearch = atoi(strBuffer); // (0-off, 1-on)
		if ( m_nmdParams.nPolarizerLibrarySearch < 0 )
			m_nmdParams.nPolarizerLibrarySearch = 0;

		stdioFile.ReadString(strBuffer);
		if ( atoi(strBuffer) == 1 )
			m_nmdParams.bGratingFitPitch = TRUE;

		stdioFile.ReadString(strBuffer);
		if ( atoi(strBuffer) == 1 )
			m_nmdParams.nUseAperture = TRUE;			// 不明。VBで、intにTrueを代入している

		stdioFile.ReadString(strBuffer);
		m_nmdParams.nGratingDataType = atoi(strBuffer);

		//-------------------------------------------
		// Check for a possible end of the NMD file.
		//-------------------------------------------
		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		if ( nDummy < 0 )
			nDummy = 0;
		m_nmdParams.nScanQuadrants = nDummy;

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		if ( nDummy < 0 )
			nDummy = 0;
		m_nmdParams.nScansPerQuadrant = nDummy;

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.bPolarizerTrackWafer = (nDummy == 1);

		if ( m_nmdParams.nPolarizerProgramType == 4 ) {
			m_nmdParams.nScanAngleCount = m_nmdParams.nScanQuadrants * m_nmdParams.nScansPerQuadrant;
			if ( m_nmdParams.nScanAngleCount < 4 )
				return FALSE;
		}
		else if ( m_nmdParams.nUsePolarizer && ((m_nmdParams.nPolarizerProgramType >= 0) && (m_nmdParams.nPolarizerProgramType <= 3)) ) {
			m_nmdParams.bPolarizerTrackWafer = TRUE;
		}
		else {
			m_nmdParams.bPolarizerTrackWafer = FALSE;
		}
		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.bPolarisFitTE = (nDummy == 1);

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.bPolarisFitTM = (nDummy == 1);

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.bPolarisFitPsi = (nDummy == 1);

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.bPolarisFitDelta = (nDummy == 1);

		//---------------------------------------------
		// 01/02/02 Parse for possible new parameters.
		//---------------------------------------------

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.bGratingAutoSlicingEnabled = (nDummy == 1);

		stdioFile.ReadString(strBuffer);
		m_nmdParams.dGratingAutoSlicingMaxChange = 0.0;

		stdioFile.ReadString(strBuffer);
		m_nmdParams.dGratingAutoSlicingMaxSlices = atof(strBuffer);

		//-----------------------------------------
		// OCD Layer level parameters.
		// Up to 30 layer parameters are possible
		//-----------------------------------------
		for ( nLayer = 1; nLayer < m_nmdParams.nNumFilmLayers + 1; nLayer++ ) {
			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingGlobalFitLineWidthPoints = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingLayerType = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingSpacerMaterialLayer = nDummy;

			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].dGratingSpacerSideWallAngle = atof(strBuffer);

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].dGratingFitSpacerSideWallAngle = nDummy;

			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].dGratingSpacerWidth = atof(strBuffer);

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].dGratingFitSpacerWidth = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].dGratingCoupleSWA = nDummy;

			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].dGratingTopLineWidth = atof(strBuffer);

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingFitTopLineWidth = nDummy;
			stdioFile.ReadString(strBuffer);
			m_pLayers[nLayer].nGratingTopSpacerWidth = atof(strBuffer);

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingFitTopSpacerWidth = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingSideWallsVerticle = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingCoupleNK = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingCoupleLWtoPitch = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingCoupleSpacer = nDummy;

			stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
			m_pLayers[nLayer].nGratingControlType = nDummy;

			for ( int i = 0; i < 13; i++ ) {
				stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
				m_pLayers[nLayer].nNewReserved[i] = nDummy;   // reserved for future use.
			}
		}

		stdioFile.ReadString(strBuffer); nDummy = atoi(strBuffer);
		m_nmdParams.nReportPolyDensity = nDummy;

		for ( i = 0; i < 2; i++ ) {
			stdioFile.ReadString(strBuffer);
			m_nmdParams.dAbsoluteReflectanceWaveLength[i] = atof(strBuffer);
		}

		stdioFile.Close();

		//--------------------------------------------------------------------
		// set the default value to the third fastest performance level (0-6)
		//--------------------------------------------------------------------
		if ( m_nmdParams.nSR_PerformanceLevel < 0 )
			m_nmdParams.nSR_PerformanceLevel = 0;

		return TRUE;
	}
	CATCH(CFileException, e)
	{
//		CString strCause;
//		strCause.Format(pMojiretsu->LoadString(IDS_CAUSE_CODE), e->m_cause, e->m_strFileName);
//
//		switch ( e->m_cause )
//		{
//		case CFileException::fileNotFound :
//			strCause += pMojiretsu->LoadString(IDS_FILE_NOT_FOUND);
//			break;
//		case CFileException::badPath :
//			strCause += pMojiretsu->LoadString(IDS_BAD_PATH);
//			break;
//		default :
//			break;
//		}

//		MessageBox(NULL, strCause, NULL, MB_ICONERROR | MB_OK);

		return FALSE;
	}
	END_CATCH
}
// 2009.09.11 K.Matsuo SE <--

protected:
	int GetStrategyNo(LPCTSTR pszStrategyEntry)
	{
// 2009.09.17 K.Matsuo ストラテジー№取得 Bug Fix 対応 -->
//		  const int nStrategyNoLen = 2;
//		  TCHAR szStrategyNo[nStrategyNoLen + 1];
//
//		  _tcsncpy(szStrategyNo, pszStrategyEntry, nStrategyNoLen);   //頭2文字の番号を取得
//		  szStrategyNo[nStrategyNoLen] = _TCHAR('\0');
//		  return _ttoi(szStrategyNo);
		return _ttoi(pszStrategyEntry);
// 2009.09.17 K.Matsuo ストラテジー№取得 Bug Fix 対応 <--
	}

	void GetAdapParmsFilePath(LPTSTR pszPath, int iStrategyNo)
	{
		TCHAR szPath_buffer[_MAX_PATH];

		GetAdapStrategyFolderPath(szPath_buffer, iStrategyNo);
		_stprintf(pszPath, _T("%s%s"), szPath_buffer, _T("\\parms.dat"));
	}

	void GetAdapScriptFilePath(LPTSTR pszPath, int iStrategyNo)
	{
		TCHAR szPath_buffer[_MAX_PATH];

		GetAdapStrategyFolderPath(szPath_buffer, iStrategyNo);
		_stprintf(pszPath, _T("%s%s"), szPath_buffer, _T("\\analysis2.txt"));
	}

	void GetAdapStrategyFolderPath(LPTSTR pszFolderPath, int iStrategyNo)
	{
		// ConfigファイルからADAP ANALYSISのフォルダパス取得
		SR_XMP srXmp;
		TCHAR szPath_buffer[_MAX_PATH];
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFilename[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];

		ConfigFile_GetNanoSpecIni(&srXmp, CONFIG_FILE_SR_XMP);
		_splitpath(srXmp.szStrategyListPath, szDrive, szDir, szFilename, szExt);
		_stprintf(szPath_buffer, _T("%s%s%s%d"), szDrive, szDir, _T("as"), iStrategyNo);
		_tcscpy(pszFolderPath, szPath_buffer);
	}

	BOOL LoadAdapParamsFile(LPCTSTR pszPath)
	{
		// ファイル「parms.dat」から表示名取得
		const int nMaxSize = 256;
		TCHAR szLine[nMaxSize];
		TCHAR szBuff[nMaxSize];

		FILE* fp;
		TCHAR* token;
		int iParmsLineCnt;

		if ( (fp = fopen(pszPath, _T("r"))) == NULL )
			return FALSE;
		try
		{
			// テキスト1行目にデータ個数が書かれている
			if ( fgets(szLine, nMaxSize, fp) == NULL )
				throw _T("Text Fortmat Error");
			iParmsLineCnt = _ttoi(szLine);
			if ( iParmsLineCnt > XMPREGISTARMAX )
				throw _T("Item Data Count Over");
			// テキスト2行目以降の処理
			while ( fgets(szLine, nMaxSize, fp) != NULL )
			{
				_tcscpy(szBuff, szLine);
				// ここ,****,****
				if ( (token = _tcstok(szBuff, ",")) == NULL )
					throw _T("Text Fortmat Error");
				_tcscpy(m_searchTbl[m_iMaxCnt].szAdapName, token);
				// ****,ここ,****
				if ( (token = _tcstok(NULL, ",")) == NULL )
					throw _T("Text Fortmat Error");
				_tcscpy(m_searchTbl[m_iMaxCnt].szAdapRename, token);
				// ****,****,ここ
				if ( (token = _tcstok(NULL, "\n")) == NULL )
					throw _T("Text Fortmat Error");
				_tcscpy(m_searchTbl[m_iMaxCnt].szAdapUnit, token);
				m_iMaxCnt++;
			}
		}
		catch(TCHAR* e)
		{
			TRACE(e);
			fclose(fp);
			return FALSE;
		}
		fclose(fp);
		// テキスト1行目の個数データと読込み件数が同じであるかチェック
		if ( m_iMaxCnt != iParmsLineCnt )
			return FALSE;
		return TRUE;
	}

	BOOL GetCommandFitGet(LPTSTR pszCommand, LPCTSTR pszPath)
	{
		BOOL bRet = FALSE;

		const int nMaxSize = 1024;
		TCHAR szLine[nMaxSize];
		FILE* fp;

		if ( (fp = fopen(pszPath, _T("r"))) == NULL )
			return FALSE;
		while ( fgets(szLine, nMaxSize, fp) ) {
			if ( _tcsstr(szLine, _T("fitget(")) ) {
				_tcscpy(pszCommand, szLine);
				bRet = TRUE;
				break;
			}
		}
		fclose(fp);

		return bRet;
	}

	typedef struct _ADAPPARAMS {
		TCHAR szAdapName[XMPSTRINGLENMAX + 1];
		TCHAR szAdapRename[XMPSTRINGLENMAX + 1];
		TCHAR szAdapUnit[XMPSTRINGLENMAX + 1];
	} ADAPPARAMS, *PADAPPARAMS;

	ADAPPARAMS m_searchTbl[XMPREGISTARMAX];
	int m_iMaxCnt;

// 2009.09.11 K.Matsuo SE -->
	typedef struct _FILM_LAYER_DATA {
		char strLayerFile[13 + 1];				//
		int nLayerType; 						//
		int nLayerFlag; 						// Standard layer measurement flags.
		int nLayerMat;							// Standard material file number.
		char strLayerDescript[MAXNAMELEN + 1];	// Material file description
		double LayerThick;						// Thickness of layer.
		double LayerMinThick;					// Minimum thickness of layer.
		double LayerMaxThick;					// Maximum thickness of layer.
		int nNumThickPts;						//
		int nT_Var; 							// Fit for thickness for this layer.
		int nNK_Var;							//
		int nNumNK_Vars;						//
		double NMult;							// N multiplier for each layer.
		double KMult;							// K multiplier for each layer.
		double NomAlloy;						// Nominal alloy fraction.
		double MinAlloy;						// Minimum alloy fraction.
		double MaxAlloy;						// Maximum alloy fraction.
		int nGlobalAlloyFit;					//
		double Actual[9];						// Recal Actual values for the layer.
		double Measured[9]; 					// Recal Measured values for the layer.
		int nGratingEnabled;
		int nGratingFillBelow;
		int nGratingFillLayer;
		int nGratingSlices;
		double fGratingSideWallAngle;
		int nGratingFitSideWallAngle;
		int nGratingHarmonics;
		double fGratingPitch;					// spacing between features (microns).
		double fGratingLineWidth;
		int nGratingFitLineWidth;
		int nReserved[1];						// reserved for future use.

		// The CD graph parameters are determined after parsing the PARMS.DAT file.
		int nCDProfileBaseLineFieldID;
		int nCDProfileHeightFieldID;
		int nCDProfileSideWallAngleFieldID;

		// 01/02/02 New OCD layer parameters
		int nGratingGlobalFitLineWidthPoints;
		int nGratingLayerType;
		int nGratingSpacerMaterialLayer;
		double dGratingSpacerSideWallAngle;
		int dGratingFitSpacerSideWallAngle;
		double dGratingSpacerWidth;
		int dGratingFitSpacerWidth;
		int dGratingCoupleSWA;
		double dGratingTopLineWidth;
		int nGratingFitTopLineWidth;
		double nGratingTopSpacerWidth;
		int nGratingFitTopSpacerWidth;
		int nGratingSideWallsVerticle;
		int nGratingCoupleNK;
		int nGratingCoupleLWtoPitch;
		int nGratingCoupleSpacer;
		int nGratingControlType;
		int nNewReserved[13];					// reserved for future use.
	} FILM_LAYER_DATA;

	FILM_LAYER_DATA* m_pLayers;


	typedef struct _NMD_FMT {
		int nNumFilmLayers;

		int nReflectWeight; 			// Weighting factor for reflectance v. SE
		int nFitGain;					// Whether to fit gain flag.
		int nFitAOI;					// Whether to fit AOI flag.
		double fGlobMSE_Cutoff; 		// Global MSE cutoff value.
		double nEnableGlobSearch;		// Enable global search cutoff.
		int nNonIdeality;				// Non ideality value.
		double fMinWaveLen; 			// Minimum spectral wave length.
		double fMaxWaveLen; 			// Maximum spectral wave length.
		double fFTIR_MinWaveLen;		// Minimum spectral wave length.
		double fFTIR_MaxWaveLen;		// Maximum spectral wave length.
		double fFTIR_Resolution;		// Maximum spectral wave length.
		int nFTIR_NumScans; 			//
		BOOL bHighAccuracy; 			//
		double Pol; 					// change it from INT to float to fit in ADAP
		int nCycles;					//
		int nEnableDB_Search;			// Enable Data Base search flag.
		int nSearchDB_Always;			// Search Data Base always flag.
		int nNoModel;					// No Model flag.
		int nUseReflectData;			//
		int nNoM44_Data;				// Flag for no SE data in program.
		int nUseFTIR_Data;				// Flag for no FTIR data in program.
		int nUseNIR_Data;				// Flag for no NIT data in program.
		int nAnalysisType;				// Flag for no XMO analysis type.
		int nNK_Invert; 				// Flag for whether N & K will be inverted.
		int nNK_InvertLayer;			// Layer whose N & K will be inverted.
		int nCheckPrevModel;			// Check previous model flag.
		int nCheckNomModel; 			// Check nominal model flag.
		int nEnablePeakFinder;			// Enable peak finder flag.
		int nReturnTotalThick;			// Return total thickness flag.
		int nScriptLocked;
		int nUseBurg;
		double fF_Slope;
		double fF_Intercept;
		int nAdvProgNum;
		double fF_Index;
		int nNK_Layers[3];				// Layer for first N & K to fit.
		double fNK_WaveLengths[3];		// Wave length for first N & K to fit.
		int nIntegType;
		int nNormalizeMSE;
		int nDeriv_Freq;
		int nPhotoResist;
		double fMSENormFactor;
		double fSE_MinWaveLen;
		double fSE_MaxWaveLen;
		double fSE_MinTau;
		double fSE_MaxTau;
		int nFitBandWidth;
		double fBandWidth;
		int nGlobal_R_Enabled;
		double fGlobal_R_MinWaveLen;
		double fGlobal_R_MaxWaveLen;
		int nGlobal_SE_Enabled;
		double fGlobal_SE_MinWaveLen;
		double fGlobal_SE_MaxWaveLen;
		int nGlobal_FTIR_Enabled;
		double fGlobal_FTIR_MinWaveLen;
		double fGlobal_FTIR_MaxWaveLen;
		int nSR_PerformanceLevel;		// Number of scans to avereage for the reflectometer.
		int nSECloseShuttersDuringSR;
		int nSEOpenUVShuttersDuringSE;

		double fGratingPitch;
		int nGratingNumHarmonicsTE;
		int nGratingNumHarmonicsTM;

		int nGratingModel;
		int nReferenceBeam;
		int nMeasureBeam;
		double dMetalReferenceN;
		double dMetalReferenceK;
		double dMetalMeasureN;
		double dMetalMeasureK;
		int nReflectanceScanCount;
		int nPolarizerProgramType;

		//----------------------------------------
		// More optical profiler film parameters.
		//----------------------------------------

		int nProfilerMeasureMode;
		double fProfilerRDishHighestPercent;
		double fProfilerRDishLowestPercent;
		int nProfilerGroup[10];
		int nProfilerReportGroup[10];
		int nPolarizerLibrarySearch;
		BOOL bPolarizerTrackWafer;		// Matt's s parameter.
		int nScanQuadrants; 			// Matt's q parameter.
		int nScansPerQuadrant;			// Matt's r parameter.
		int nUseAperture;
		int nGratingDataType;

		//---------------------------------------------
		// The number of multi-angle polarizer scans.
		// 1 (o degrees or 3( 0, 45, and 90 degrees ).
		//---------------------------------------------
		int nScanAngleCount;

		//	nUseOpticalProfiler;
		int nUseLaserProfiler;
		BOOL bExpectLaserProfilerGraph;
		int nUsePolarizer;

		BOOL bGratingFitPitch;
		BOOL bPolarisFitTE;
		BOOL bPolarisFitTM;
		BOOL bPolarisFitPsi;
		BOOL bPolarisFitDelta;

		//-----------------------
		// CD Profile parameters
		//-----------------------
		int nGratingLayerCount;

		double fTM_TE_AngleOffset;

		//------------------------------------------
		// 01/02/02 More parameters, mostly for OCD
		//------------------------------------------

		BOOL bGratingAutoSlicingEnabled;
		double dGratingAutoSlicingMaxChange;
		double dGratingAutoSlicingMaxSlices;
		int nReportPolyDensity;
		double dAbsoluteReflectanceWaveLength[2];

		//-------------------------------
		// Data from the Parms.dat file.
		//-------------------------------

		int nNumFilmParameters;
		int nNumFilmParamsParsed;

	} NMD_FMT;

public:
	NMD_FMT m_nmdParams;
// 2009.09.11 K.Matsuo SE <--
};
