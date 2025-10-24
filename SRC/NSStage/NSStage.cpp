// NSStage.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include <process.h>

#include "../../INC/globals.hxx"
#include "../../INC/RecipeFile.hxx"
#include "../../INC/stageint.hxx"
#include "../../INC/Deskew.hxx"
#include "resource.h"
#include "StageCoord.hxx"
#include "Mojiretsu.h"

#define _NSSTAGETDLL_
#include "../../INC/NSStage.hxx"

// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
#define _MASTER_
#include "System.h"
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--

#ifndef _DEBUG
// Release
#pragma comment(lib, "TnsStage.lib")
#pragma comment(lib, "TnsDeskew.lib")
#else
// Debug
#pragma comment(lib, "dTnsStage.lib")
#pragma comment(lib, "dTnsDeskew.lib")
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �N���X�|�C���^
CMojiretsu* pMojiretsu;

// variables
static WORD wActiveHeadType;
static UINT nActiveSrLens;
static STAGE_COORD asyncStageMoveCoord;
static BOOL bShutDown;
static HANDLE hEvent;
static HANDLE hThread;
static UINT nThreadId;
static BOOL bThread;
static UINT nStageType;
static long lSpeedX;
static long lSpeedY;
// 2013.02.22 bagus Substrate thickness setting -->
static int iActiveSubstrateThickness = -1;
// 2013.02.22 bagus Substrate thickness setting <--

static AFX_EXTENSION_MODULE NSStageDLL = { NULL, NULL };

static UINT WINAPI NS_StageMoveThread(LPVOID lpData);

enum STAGE_TYPE
{
	STAGE_TYPE_NONE = 0,
	STAGE_TYPE_TOHO,
	STAGE_TYPE_NTN,
	STAGE_TYPE_SPT,		// �\�f�B�b�N�X�e�[�W
	STAGE_TYPE_MAX,
};



TCHAR g_tszProcDir[_MAX_PATH] = _T("");		/* �ďo���v���Z�X�̃f�B���N�g��('\'�t��)*/
TCHAR g_tszBaseDir[_MAX_PATH] = _T("");		/* ��f�B���N�g��('\'�t��)*/
void GetProcBaseDir(
		LPTSTR ptszProcDir,
		LPTSTR ptszBaseDir
	)
{
	if (0 == _tcscmp(ptszProcDir, _T(""))) {
		TCHAR l_tszProcessFName[_MAX_PATH];	/* �ďo���v���Z�X�̃t���p�X*/
		::GetModuleFileName(0, l_tszProcessFName, sizeof(l_tszProcessFName));
		TCHAR l_tszDrive[_MAX_DRIVE];
		TCHAR l_tszDir[_MAX_DIR];
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
		TCHAR l_tszFilename[_MAX_FNAME];
//		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, 0, 0);
		_tsplitpath(l_tszProcessFName, l_tszDrive, l_tszDir, l_tszFilename, 0);

		g_lAppNameType = APP_NAME_NANO; // default application name
		for(int i = 0; i < APP_NAME_MAX; i++){
			if(_tcsncmp(l_tszFilename, g_lpszAppPrefix4[i], 4) == 0){
				g_lAppNameType = i;
				break;
			}
		}
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
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


/* added 2014.12.22 hmenjo DLL ���d�N���h�~ ---------- { ---------- */
#include <DllMutex.hxx>
/* added 2014.12.22 hmenjo DLL ���d�N���h�~ ---------- } ---------- */
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
/* added 2014.12.22 hmenjo DLL ���d�N���h�~ ---------- { ---------- */
		if (FALSE == DllMutexCreate(_T("NSStage"))) {
			return TRUE;
		}
/* added 2014.12.22 hmenjo DLL ���d�N���h�~ ---------- } ---------- */
		TRACE0("NSSTAGE.DLL Initializing!\n");

		GetProcBaseDir(g_tszProcDir, g_tszBaseDir);

		if (!AfxInitExtensionModule(NSStageDLL, hInstance))
			return 0;

		new CDynLinkLibrary(NSStageDLL);

		bShutDown = FALSE;
		hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread = (HANDLE)_beginthreadex(NULL, 0, NS_StageMoveThread, NULL, 0, &nThreadId);

		pMojiretsu = new CMojiretsu();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("NSSTAGE.DLL Terminating!\n");
		AfxTermExtensionModule(NSStageDLL);

		bShutDown = TRUE;
		SetEvent(hEvent);

		if ( hThread ) {
			::WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = NULL;
		}

		if ( hEvent ) {
			CloseHandle(hEvent);
			hEvent = NULL;
		}

		if ( pMojiretsu ) {
			delete pMojiretsu;
			pMojiretsu = NULL;
		}
/* added 2014.12.22 hmenjo DLL ���d�N���h�~ ---------- { ---------- */
		DllMutexRelease();
/* added 2014.12.22 hmenjo DLL ���d�N���h�~ ---------- } ---------- */
	}
	return 1;
}

//---------------------------------------------------------------------------
// NS_StageInitialize
BOOL NSSTAGEAPI NS_StageInitialize()
{
	TRACE(_T("NS_StageInitialize()\n"));

	// Load System Config /////
	SYSTEM_CONFIG systemConfig;
	ConfigFile_GetNanoSpecIni(&systemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	nStageType = systemConfig.nStageType;

#ifdef _DEBUG
	HANDLE hModule = ResourceLoadLibrary(systemConfig.nLanguage, "dTnsNSStage");	// Macro : ResourceLoadLibrary // ref. globals.hxx
#else
	HANDLE hModule = ResourceLoadLibrary(systemConfig.nLanguage, "TnsNSStage");	// Macro : ResourceLoadLibrary // ref. globals.hxx
#endif
//	AfxSetResourceHandle((HINSTANCE) hModule);

	pMojiretsu->SetResourceHandle((HINSTANCE) hModule);

	// �X�e�[�W���SPT�i�\�f�B�b�N�X�e�[�W�j
	// ���W�W���Ƒ��x�ݒ�
	if (STAGE_TYPE_SPT == nStageType) {
		/*	���t�B�[�h�o�b�N����\�F156.25[nm]=156250[pm] */
		LONG64 l_l64FactorBase = 156250;	/* [pm] */
		long l_lEleGear;
		StageGetElectronicGear(X, &l_lEleGear);
		LONG64 l_lEleGearXA = HIWORD(l_lEleGear);	/* A�F���q */
		LONG64 l_lEleGearXB = LOWORD(l_lEleGear);	/* B�F���� */
		StageGetElectronicGear(Y, &l_lEleGear);
		LONG64 l_lEleGearYA = HIWORD(l_lEleGear);	/* A�F���q */
		LONG64 l_lEleGearYB = LOWORD(l_lEleGear);	/* B�F���� */
		/* �P�ʂ�Ǐo���܂��D(���l�P�ʂ�[um]�ł�) */
		TCHAR l_tszTemp[128];
		double l_dTemp;
		char szFilePath[MAX_PATH];
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) -->
//		sprintf(szFilePath, "%s%s%s", g_tszProcDir, CFG_DIR, NANOSPEC_INIFILENAME);
		CString strFilename;

		strFilename = NANOSPEC_INIFILENAME;
		if(g_lAppNameType != APP_NAME_NANO){
			strFilename.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
		}
		sprintf(szFilePath, "%s%s%s", g_tszProcDir, CFG_DIR, strFilename);
// 2013.11.07 Bagus Mod (TohoSpec�Ή�) <--
		::GetPrivateProfileString(_T("StageCoord"), _T("Xunit"), _T("+1.0"), l_tszTemp, sizeof(l_tszTemp), szFilePath);
		l_dTemp = _tcstod(l_tszTemp, 0);
		LONG64 l_l64Xunit = (LONG64) l_dTemp * 1000000; /* [pm] */
		LONG64 l_l64XunitABS = (l_l64Xunit < 0)? l_l64Xunit * -1 : l_l64Xunit;	/* [pm] */
		::GetPrivateProfileString(_T("StageCoord"), _T("Yunit"), _T("+1.0"), l_tszTemp, sizeof(l_tszTemp), szFilePath);
		l_dTemp = _tcstod(l_tszTemp, 0);
		LONG64 l_l64Yunit = (LONG64) l_dTemp * 1000000; /* [pm] */
		LONG64 l_l64YunitABS = (l_l64Yunit < 0)? l_l64Yunit * -1 : l_l64Yunit;	/* [pm] */
		LONG64 l_l64unitMAX = 1000000000;	/* [pm] */
		LONG64 l_l64unitMIN = 5000; /* [pm] */
		if (
			((0 == l_l64Xunit) || (l_l64XunitABS < l_l64unitMIN) || (l_l64unitMAX < l_l64XunitABS))
		 || ((0 == l_l64Yunit) || (l_l64YunitABS < l_l64unitMIN) || (l_l64unitMAX < l_l64YunitABS))
			) {
			MessageBox(NULL, pMojiretsu->LoadString(IDS_STAGE_UNIT_PARAM_SET_ERROR), "NanoSpec", MB_OK | MB_ICONSTOP);
		}
		// �P�ʌW�����v�Z���܂��D([count/(��L�P��)])
		LONG64 l_l64FactorX = (l_l64Xunit * l_lEleGearXB) / (l_l64FactorBase * l_lEleGearXA);
		LONG64 l_l64FactorY = (l_l64Yunit * l_lEleGearYB) / (l_l64FactorBase * l_lEleGearYA);
		long l_lFactorX = (long) l_l64FactorX;
		long l_lFactorY = (long) l_l64FactorY;
		StageSetFactor(X, l_lFactorX);
		StageSetFactor(Y, l_lFactorY);
		// �W�����x��Ǐo���܂��D(�P�ʂ�[(��L�P��)/ms]�ł�)
		::GetPrivateProfileString(_T("StageCoord"), _T("Xspeed"), _T("+400"), l_tszTemp, sizeof(l_tszTemp), szFilePath);
		l_dTemp = _tcstod(l_tszTemp, 0);
		lSpeedX = (long) l_dTemp;
		::GetPrivateProfileString(_T("StageCoord"), _T("Yspeed"), _T("+400"), l_tszTemp, sizeof(l_tszTemp), szFilePath);
		l_dTemp = _tcstod(l_tszTemp, 0);
		lSpeedY = (long) l_dTemp;
		if (
			((lSpeedX <= 0) || (400 < lSpeedX))
		 || ((lSpeedY <= 0) || (400 < lSpeedY))
			) {
			MessageBox(NULL, pMojiretsu->LoadString(IDS_STAGE_SPEED_PARAM_SET_ERROR), "NanoSpec", MB_OK | MB_ICONSTOP);
		}
		StageSetLocalSpeed(lSpeedX, lSpeedY);
	}

	// Load Lens Offset Config /////
	SR_LENS_CENTER_OFFSET srLensOffset;
	ConfigFile_GetNanoSpecIni(&srLensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);

	// �ŏ��̃w�b�h�^�C�v��ݒ�
	// �Ƃ肠�����f�t�H���g SR�w�b�h�Ƃ���
	wActiveHeadType = HEAD_TYPE_SR;

	// Turret Backlash ����
	StageSelectLens(1);
	StageSelectLens(0);

	// �ŏ��̃����Y����
	StageSelectLens(srLensOffset.iBaseLens);
	nActiveSrLens = srLensOffset.iBaseLens;

// 2013.02.22 bagus Substrate thickness setting -->
	iActiveSubstrateThickness = -1;		// ���I��
// 2013.02.22 bagus Substrate thickness setting <--

	return TRUE;
}

//---------------------------------------------------------------------------
// NS_StageGetPos
void NSSTAGEAPI NS_StageGetPos(STAGE_COORD_XYZ* pCoordXyz)
{
	StageGetPos(pCoordXyz);

	NS_ConvertToStageGetPosCoord(pCoordXyz);
}

//---------------------------------------------------------------------------
// NS_StageGetPos
void NSSTAGEAPI NS_StageGetPos(STAGE_COORD* pCoordXy)
{
	STAGE_COORD_XYZ coordXyz;

	NS_StageGetPos(&coordXyz);

	pCoordXy->lX = coordXyz.lX;
	pCoordXy->lY = coordXyz.lY;
}

//---------------------------------------------------------------------------
// NS_SampleCoordStageGetPos
void NSSTAGEAPI NS_SampleCoordStageGetPos(STAGE_COORD_XYZ* pCoordXyz, const STAGE_COORD* pSampleOriginCoordXy)
{
	STAGE_COORD_XYZ coordXyz;

	NS_StageGetPos(&coordXyz);

	// �I�t�Z�b�g�i�T���v�����W�j�l�����Z����
	coordXyz.lX -= pSampleOriginCoordXy->lX;
	coordXyz.lY -= pSampleOriginCoordXy->lY;
	*pCoordXyz = coordXyz;
}

//---------------------------------------------------------------------------
// NS_SampleCoordStageGetPos
void NSSTAGEAPI NS_SampleCoordStageGetPos(STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy)
{
	STAGE_COORD_XYZ coordXyz;

	NS_SampleCoordStageGetPos(&coordXyz, pSampleOriginCoordXy);

	pCoordXy->lX = coordXyz.lX;
	pCoordXy->lY = coordXyz.lY;
}

//---------------------------------------------------------------------------
// NS_StageMoveAbsolute
BOOL NSSTAGEAPI NS_StageMoveAbsolute(const STAGE_COORD* pCoordXy)
{
	STAGE_COORD convCoord;
	convCoord = *pCoordXy;

	NS_ConvertToStageMoveCoord(&convCoord);

	return StageMoveAbsolute(&convCoord);
}

//---------------------------------------------------------------------------
// NS_SampleCoordStageMoveAbsolute
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsolute(const STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy, int iMeasHead /* = -1*/)
{
	STAGE_COORD headOffset;
	STAGE_COORD convCoord;
	STAGE_COORD deskewCoord;

	if ( iMeasHead == -1 ) {
		iMeasHead = wActiveHeadType;
	}
	NS_GetDiffHeadTypeOffset(iMeasHead, &headOffset);

	convCoord.lX = pCoordXy->lX + headOffset.lX;
	convCoord.lY = pCoordXy->lY + headOffset.lY;

	NS_ConvertToStageMoveCoord(&convCoord, pSampleOriginCoordXy);

	DeskewCorrectToCurrent(convCoord, deskewCoord);

	return StageMoveAbsolute(&deskewCoord);
}

//---------------------------------------------------------------------------
// NS_SampleCoordStageMoveAbsolute
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsolute(const STAGE_COORD* pCoordXy, LPCTSTR pszCurrentMainRecipeName, int iMeasHead /* = -1*/)
{
	STAGE_COORD headOffset;
	STAGE_COORD convCoord;
	STAGE_COORD deskewCoord;

	if ( iMeasHead == -1 ) {
		iMeasHead = wActiveHeadType;
	}
	NS_GetDiffHeadTypeOffset(iMeasHead, &headOffset);

	convCoord.lX = pCoordXy->lX + headOffset.lX;
	convCoord.lY = pCoordXy->lY + headOffset.lY;

	NS_ConvertToStageMoveCoord(&convCoord, pszCurrentMainRecipeName);

	DeskewCorrectToCurrent(convCoord, deskewCoord);

	return StageMoveAbsolute(&deskewCoord);
}

//---------------------------------------------------------------------------
// NS_StageMoveRelative
BOOL NSSTAGEAPI NS_StageMoveRelative(const STAGE_COORD* pCoordXy)
{
	STAGE_COORD convCoord;

	NS_StageGetPos(&convCoord);
	convCoord.lX += pCoordXy->lX;
	convCoord.lY += pCoordXy->lY;

	return NS_StageMoveAbsolute(&convCoord);
}

//---------------------------------------------------------------------------
// NS_GetHeadType
WORD NSSTAGEAPI NS_GetCurrentHeadType()
{
	return wActiveHeadType;
}

//---------------------------------------------------------------------------
// NS_SelectHeadType
BOOL NSSTAGEAPI NS_SelectHeadType(WORD wHeadType, BOOL bStageMove)
{
	if ( wActiveHeadType == wHeadType )
		return TRUE;

	STAGE_COORD coord;

	if ( bStageMove ) {
		NS_GetDiffHeadTypeOffset(wHeadType, &coord);
		NS_StageMoveRelative(&coord);
	}

	// ���݂̃w�b�h�^�C�v��ۑ����Ă���
	wActiveHeadType = wHeadType;

	return TRUE;
}

// 2013.02.22 bagus Substrate thickness setting -->
//---------------------------------------------------------------------------
// NS_StageSelectLens
// BOOL NSSTAGEAPI NS_StageSelectLens(UINT nLens)
// {
// 	// �����Y�ؑւ��ŃY��������������A�A�T�[�g�����B
// 	// �ǂ����̃^�C�~���O�ŁAStageSelectLens()�����ڌĂ΂�Ă���\��������
// 	ASSERT(INT_MIN == StageGetCurLens() || nActiveSrLens == (UINT)StageGetCurLens());
//
// 	if ( nLens == nActiveSrLens )
// 		return TRUE;
//
// 	STAGE_COORD activeLensPosXy;
// 	STAGE_COORD newLensPosXy;
// 	STAGE_COORD coordPosXy;
//
// 	NS_GetSrLensOffsetCoord(nActiveSrLens, &activeLensPosXy);
// 	NS_GetSrLensOffsetCoord(nLens, &newLensPosXy);
// 	coordPosXy.lX = -activeLensPosXy.lX + newLensPosXy.lX;
// 	coordPosXy.lY = -activeLensPosXy.lY + newLensPosXy.lY;
//
// 	// �X�e�[�W�I�t�Z�b�g����
// 	NS_StageMoveRelative(&coordPosXy);
//
// 	// ���̃����Y��STAGE.DLL�T�u�R���^�[���b�g�ύX����
// 	StageSelectLens(nLens);
//
// 	// ���݂̃����Y����ۑ����Ă���
// 	nActiveSrLens = nLens;
//
// 	return TRUE;
// }
BOOL NSSTAGEAPI NS_StageSelectLens(UINT nLens)
{
	// �����Y�ؑւ��ŃY��������������A�A�T�[�g�����B
	// �ǂ����̃^�C�~���O�ŁAStageSelectLens()�����ڌĂ΂�Ă���\��������
	ASSERT(INT_MIN == StageGetCurLens() || nActiveSrLens == (UINT)StageGetCurLens());

	if ( nLens == nActiveSrLens )
		return TRUE;

	STAGE_COORD_XYZ activeLensPosXyz;
	STAGE_COORD_XYZ newLensPosXyz;
	STAGE_COORD coordPosXy;
	long lPosZ;

	NS_GetSrLensOffsetCoord(nActiveSrLens, &activeLensPosXyz);
	NS_GetSrLensOffsetCoord(nLens, &newLensPosXyz);
	coordPosXy.lX = -activeLensPosXyz.lX + newLensPosXyz.lX;
	coordPosXy.lY = -activeLensPosXyz.lY + newLensPosXyz.lY;

	// �X�e�[�W�I�t�Z�b�g����
	NS_StageMoveRelative(&coordPosXy);

	// ���̃����Y��STAGE.DLL�T�u�R���^�[���b�g�ύX����
	StageSelectLens(nLens);

	// Z �I�t�Z�b�g
	lPosZ = -activeLensPosXyz.lZ + newLensPosXyz.lZ;
	StageSetZLensOffset(lPosZ);

	// ���݂̃����Y����ۑ����Ă���
	nActiveSrLens = nLens;

	return TRUE;
}
// 2013.02.22 bagus Substrate thickness setting <--

//---------------------------------------------------------------------------
// NS_StageDoAutoFocus
BOOL NS_StageDoAutoFocus()
{
	// AF ���������Y�ؑ֕t��
	// AF �O�Ɍ��݂̃����Y��Ǐo���C
	// ���̃����Y�ł� AF �� SystemSR.ini �� AF �p�����Y�̎w�肪����Ă���ꍇ��
	// AF �O�ɂ��̃����Y�ɐؑւ� AF �I����C�����Y�����ɖ߂��܂��D

	BOOL bRet;

	// Sr Turret Config /////
	SR_TURRET srTurret[SR_LENS_MAX];
	ConfigFile_GetNanoSpecIni(&srTurret, CONFIG_FILE_SR_TURRET);

	// �����Y�� AF �p�ɐؑւ�
	StageSelectLens(srTurret[nActiveSrLens].iAFLens);	// �����Y�I�t�Z�b�g�ړ��Ȃ��̃����Y�ؑցi���x�d���j
	// AF ���s
	bRet = StageDoAutoFocus();
	// �����Y��߂��܂��i�����Y�ؑւ��̃Y�������j
	StageSelectLens(nActiveSrLens);						// �����Y�I�t�Z�b�g�ړ��Ȃ��̃����Y�ؑցi���x�d���j

	return bRet;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageGetPosCoord
void NSSTAGEAPI NS_ConvertToStageGetPosCoord(STAGE_COORD_XYZ* pCoordXyz)
{
	STAGE_COORD_XYZ coordXyz;
	coordXyz = *pCoordXyz;

	// StageApi���W����StageConfig���W�ɕϊ�
	CStageCoord convCoord(coordXyz.lX, coordXyz.lY);
	convCoord.ToUiDisplayCoord();

	// �I�t�Z�b�g�i�w�b�h�E�����Y�j�l�����Z����
	STAGE_COORD_XYZ headLensOffset;
	NS_GetCurrOffsetStageCoord(&headLensOffset);
	convCoord.lX -= headLensOffset.lX;
	convCoord.lY -= headLensOffset.lY;
	coordXyz.lZ -= headLensOffset.lZ;

	pCoordXyz->lX = convCoord.lX;
	pCoordXyz->lY = convCoord.lY;
	pCoordXyz->lZ = coordXyz.lZ;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageGetPosCoord
void NSSTAGEAPI NS_ConvertToStageGetPosCoord(STAGE_COORD* pCoordXy)
{
	STAGE_COORD_XYZ coordXyz;
	coordXyz.lX = pCoordXy->lX;
	coordXyz.lY = pCoordXy->lY;
	coordXyz.lZ = 0L;

	NS_ConvertToStageGetPosCoord(&coordXyz);

	pCoordXy->lX = coordXyz.lX;
	pCoordXy->lY = coordXyz.lY;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveCoord
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD_XYZ* pCoordXyz, const STAGE_COORD* pSampleOriginCoordXy)
{
	CStageCoord convCoord(pCoordXyz->lX, pCoordXyz->lY);

	// �I�t�Z�b�g�i�w�b�h�E�����Y�j�l�����Z����
	STAGE_COORD_XYZ headLensOffset;
	NS_GetCurrOffsetStageCoord(&headLensOffset);
	convCoord.lX += headLensOffset.lX;
	convCoord.lY += headLensOffset.lY;

	// �I�t�Z�b�g�i�T���v�����W�j�l�����Z����
	convCoord += *pSampleOriginCoordXy;

	// StageConfig���W����StageApi���W�ɕϊ�
	convCoord.ToApiCoord();

	pCoordXyz->lX = convCoord.lX;
	pCoordXyz->lY = convCoord.lY;
	pCoordXyz->lZ += headLensOffset.lZ;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveCoord
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy)
{
	STAGE_COORD_XYZ coordXyz;
	coordXyz.lX = pCoordXy->lX;
	coordXyz.lY = pCoordXy->lY;
	coordXyz.lZ = 0L;

	NS_ConvertToStageMoveCoord(&coordXyz, pSampleOriginCoordXy);

	pCoordXy->lX = coordXyz.lX;
	pCoordXy->lY = coordXyz.lY;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveCoord
void NSSTAGEAPI NS_ConvertToStageMoveCoord(long* plZ)
{
	STAGE_COORD_XYZ coordXyz;
	coordXyz.lX = 0L;
	coordXyz.lY = 0L;
	coordXyz.lZ = *plZ;

	NS_ConvertToStageMoveCoord(&coordXyz);

	*plZ = coordXyz.lZ;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveCoord
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD_XYZ* pCoordXyz, LPCTSTR pszCurrentMainRecipeName /*=NULL*/)
{
	MAIN_RCP_INFO mainRcpInfo;
	STAGE_PROG_INFO_HDR stageProgInfoHdr;
	STAGE_COORD originCoord;
// 2009.12.02 K.Matsuo SampleOrigin -->
	SAMPLEINFO sampleInfo;
// 2009.12.02 K.Matsuo SampleOrigin <--

	if ( pszCurrentMainRecipeName != NULL ) {
		///// Load Recipe /////
		RecipeFile_LoadRecipe(&mainRcpInfo, pszCurrentMainRecipeName, RECIPE_FILE_MAIN_RECIPE);
		RecipeFile_LoadRecipe(&stageProgInfoHdr, mainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM);
// 2009.12.02 K.Matsuo SampleOrigin -->
//		originCoord.lX = stageProgInfoHdr.SampleInfo.Origin.lX;
//		originCoord.lY = stageProgInfoHdr.SampleInfo.Origin.lY;
		::ZeroMemory(&sampleInfo, sizeof(SAMPLEINFO));
		RecipeFile_LoadRecipe(&sampleInfo, stageProgInfoHdr.SampleInfo.szName, RECIPE_FILE_SAMPLE);
		originCoord.lX = sampleInfo.Origin.lX;
		originCoord.lY = sampleInfo.Origin.lY;
// 2009.12.02 K.Matsuo SampleOrigin <--
	}
	else {
		originCoord.lX = 0L;
		originCoord.lY = 0L;
	}

	NS_ConvertToStageMoveCoord(pCoordXyz, &originCoord);
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveCoord
void NSSTAGEAPI NS_ConvertToStageMoveCoord(STAGE_COORD* pCoordXy, LPCTSTR pszCurrentMainRecipeName /*=NULL*/)
{
	STAGE_COORD_XYZ coordXyz;
	coordXyz.lX = pCoordXy->lX;
	coordXyz.lY = pCoordXy->lY;
	coordXyz.lZ = 0L;

	NS_ConvertToStageMoveCoord(&coordXyz, pszCurrentMainRecipeName);

	pCoordXy->lX = coordXyz.lX;
	pCoordXy->lY = coordXyz.lY;
}

//---------------------------------------------------------------------------
// NS_ConvertToXyAxisCoord
void NSSTAGEAPI NS_ConvertToXyAxisCoord(STAGE_COORD* pCoordXy)
{
	CStageCoord convCoord(pCoordXy);

	// StageConfig���W����StageApi���W�ɕϊ�
	convCoord.ToApiCoord();
	*pCoordXy = convCoord;
}

//---------------------------------------------------------------------------
// NS_ConvertToXyAxisCoord
void NSSTAGEAPI NS_ConvertToXyAxisCoord(long* plX, long* plY)
{
	STAGE_COORD coordXy;
	coordXy.lX = *plX;
	coordXy.lY = *plY;

	NS_ConvertToXyAxisCoord(&coordXy);

	*plX = coordXy.lX;
	*plY = coordXy.lY;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageConfigXyAxis
void NSSTAGEAPI NS_ConvertToStageConfigXyAxis(STAGE_COORD* pCoordXy)
{
	CStageCoord convCoord(pCoordXy);

	convCoord.ToClientXyStageCoord();
	*pCoordXy = convCoord;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageConfigXyAxis
void NSSTAGEAPI NS_ConvertToStageConfigXyAxis(long* plX, long* plY)
{
	STAGE_COORD coordXy;
	coordXy.lX = *plX;
	coordXy.lY = *plY;

	NS_ConvertToStageConfigXyAxis(&coordXy);

	*plX = coordXy.lX;
	*plY = coordXy.lY;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveXyAxis
void NSSTAGEAPI NS_ConvertToStageMoveXyAxis(STAGE_COORD* pCoordXy)
{
	CStageCoord convCoord(pCoordXy);

	convCoord.ToXyAxisCoord();
	*pCoordXy = convCoord;
}

//---------------------------------------------------------------------------
// NS_ConvertToStageMoveXyAxis
void NSSTAGEAPI NS_ConvertToStageMoveXyAxis(long* plX, long* plY)
{
	STAGE_COORD coordXy;
	coordXy.lX = *plX;
	coordXy.lY = *plY;

	NS_ConvertToStageMoveXyAxis(&coordXy);

	*plX = coordXy.lX;
	*plY = coordXy.lY;
}



//---------------------------------------------------------------------------
// NS_GetDiffHeadTypeOffset
void NSSTAGEAPI NS_GetDiffHeadTypeOffset(WORD wHeadType, STAGE_COORD* pCoordXy)
{
	if ( wActiveHeadType == wHeadType ) {
		pCoordXy->lX = 0L;
		pCoordXy->lY = 0L;
		return;
	}

	// �I�t�Z�b�g�i�J�����g�����Z�A�V�����Z�j
	STAGE_COORD_XYZ activeHeadPosXyz;
	STAGE_COORD_XYZ newHeadPosXyz;
	STAGE_COORD_XYZ headOffset;

	NS_GetHeadTypeCoordLoc(wActiveHeadType, &activeHeadPosXyz);
	NS_GetHeadTypeCoordLoc(wHeadType, &newHeadPosXyz);
	headOffset.lX = -activeHeadPosXyz.lX + newHeadPosXyz.lX;
	headOffset.lY = -activeHeadPosXyz.lY + newHeadPosXyz.lY;
	headOffset.lZ = -activeHeadPosXyz.lZ + newHeadPosXyz.lZ;

	pCoordXy->lX = headOffset.lX;
	pCoordXy->lY = headOffset.lY;
}

// 2013.02.22 bagus Substrate thickness setting -->
//---------------------------------------------------------------------------
// NS_GetCurrOffsetStageCoord
// void NSSTAGEAPI NS_GetCurrOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz)
// {
// 	// �w�b�h�I�t�Z�b�g�l��ݒ肷��
// 	STAGE_COORD_XYZ headOffset;
// 	NS_GetCurrHeadTypeOffsetStageCoord(&headOffset);
// 	pCoordXyz->lX = headOffset.lX;
// 	pCoordXyz->lY = headOffset.lY;
// 	pCoordXyz->lZ = headOffset.lZ;
//
// 	STAGE_COORD srLensOffset;
// 	NS_GetCurrSrLensOffsetStageCoord(&srLensOffset);
// 	pCoordXyz->lX += srLensOffset.lX;
// 	pCoordXyz->lY += srLensOffset.lY;
// }
void NSSTAGEAPI NS_GetCurrOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz)
{
	// �w�b�h�I�t�Z�b�g�l��ݒ肷��
	STAGE_COORD_XYZ headOffset;
	NS_GetCurrHeadTypeOffsetStageCoord(&headOffset);
	pCoordXyz->lX = headOffset.lX;
	pCoordXyz->lY = headOffset.lY;
	pCoordXyz->lZ = headOffset.lZ;

	if ( wActiveHeadType == HEAD_TYPE_SR ) {
		STAGE_COORD_XYZ srLensOffset;
	NS_GetCurrSrLensOffsetStageCoord(&srLensOffset);
	pCoordXyz->lX += srLensOffset.lX;
	pCoordXyz->lY += srLensOffset.lY;
		pCoordXyz->lZ += srLensOffset.lZ;
	}
}
// 2013.02.22 bagus Substrate thickness setting <--

//---------------------------------------------------------------------------
// NS_GetCurrHeadTypeOffsetStageCoord
void NSSTAGEAPI NS_GetCurrHeadTypeOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz)
{
	NS_GetHeadTypeCoordLoc(wActiveHeadType, pCoordXyz);
}

// 2013.02.22 bagus Substrate thickness setting -->
//---------------------------------------------------------------------------
// NS_GetCurrSrLensOffsetStageCoord
// void NSSTAGEAPI NS_GetCurrSrLensOffsetStageCoord(STAGE_COORD* pCoord)
// {
// //	// Sr�w�b�h�ȊO�ŃR�[��������A�A�T�[�g�𔭐�������B
// //	ASSERT(wActiveHeadType == HEAD_TYPE_SR);
//
// 	// �����Y�ؑւ��ŃY��������������A�A�T�[�g�����B
// 	// �ǂ����̃^�C�~���O�ŁAStageSelectLens()�����ڌĂ΂�Ă���\��������
// //	ASSERT(INT_MIN == StageGetCurLens() || nActiveSrLens == (UINT)StageGetCurLens());
//
// 	NS_GetSrLensOffsetCoord(nActiveSrLens, pCoord);
// }
void NSSTAGEAPI NS_GetCurrSrLensOffsetStageCoord(STAGE_COORD_XYZ* pCoordXyz)
{
	NS_GetSrLensOffsetCoord(nActiveSrLens, pCoordXyz);
}
// 2013.02.22 bagus Substrate thickness setting <--

//---------------------------------------------------------------------------
// NS_GetHeadTypeCoordLoc
void NSSTAGEAPI NS_GetHeadTypeCoordLoc(WORD wHeadType, STAGE_COORD_XYZ* pCoordXyz)
{
	// Caution!!
	// Get Absolute Machine Coord Location. Not Offset Coord.

	STAGE_COORD_XYZ coordXyz;

	// Load Stage Config /////
	ORIGINAL_POSITION headPos;
	ConfigFile_GetNanoSpecIni(&headPos, CONFIG_FILE_ORIGINAL_POSITION);

	switch ( wHeadType ) {
	case HEAD_TYPE_SR:
		coordXyz = headPos.LocSr;
		break;
	case HEAD_TYPE_SE:
// 2013.02.21 bagus CompleteEASE�w�b�h�ǉ� -->
	case HEAD_TYPE_COMPEASE:
// 2013.02.21 bagus CompleteEASE�w�b�h�ǉ� <--
		coordXyz = headPos.LocSe;
		break;
// 2009.10.19 bagus MS �ǉ� --{--
#if 0
	case HEAD_TYPE_IRSE:
		coordXyz = headPos.LocIrse;
		break;
#else
	case HEAD_TYPE_MS:
		coordXyz = headPos.LocMs;
		break;
#endif
// 2009.10.19 bagus MS �ǉ� --}--
	case HEAD_TYPE_4PP:
		coordXyz = headPos.Loc4PP;
		break;
	case HEAD_TYPE_CTA:
		coordXyz = headPos.LocContactAngle;
		break;
	case HEAD_TYPE_STRESS:
		coordXyz = headPos.LocStress;
		break;
	}

	*pCoordXyz = coordXyz;
}

// 2013.02.22 bagus Substrate thickness setting -->
//---------------------------------------------------------------------------
// NS_GetSrLensOffsetCoord
// void NSSTAGEAPI NS_GetSrLensOffsetCoord(UINT nLens, STAGE_COORD* pCoordXy)
// {
// 	// Caution!!
// 	// Get Offset Coord. Not Absolute Machine Coord Location.
//
// 	STAGE_COORD coordXy;
//
// 	// Load Stage Config /////
// 	SR_LENS_CENTER_OFFSET srLensOffset;
// 	ConfigFile_GetNanoSpecIni(&srLensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);
//
// 	coordXy.lX = srLensOffset.Lens[nLens].lX - srLensOffset.Lens[srLensOffset.iBaseLens].lX;
// 	coordXy.lY = srLensOffset.Lens[nLens].lY - srLensOffset.Lens[srLensOffset.iBaseLens].lY;
//
// 	*pCoordXy = coordXy;
// }
void NSSTAGEAPI NS_GetSrLensOffsetCoord(UINT nLens, STAGE_COORD_XYZ* pCoordXyz)
{
	// Caution!!
	// Get Offset Coord. Not Absolute Machine Coord Location.

	STAGE_COORD_XYZ coordXyz;

	// Load Stage Config /////
	SR_LENS_CENTER_OFFSET srLensOffset;
	ConfigFile_GetNanoSpecIni(&srLensOffset, CONFIG_FILE_SR_LENS_CENTER_OFFSET);

	coordXyz.lX = srLensOffset.Lens[nLens].lX - srLensOffset.Lens[srLensOffset.iBaseLens].lX;
	coordXyz.lY = srLensOffset.Lens[nLens].lY - srLensOffset.Lens[srLensOffset.iBaseLens].lY;
	coordXyz.lZ = srLensOffset.Lens[nLens].lZ - srLensOffset.Lens[srLensOffset.iBaseLens].lZ;

	*pCoordXyz = coordXyz;
}
// 2013.02.22 bagus Substrate thickness setting <--


//---------------------------------------------------------------------------
// NS_StageMoveAbsoluteEx
BOOL NSSTAGEAPI NS_StageMoveAbsoluteEx(const STAGE_COORD* pCoordXy)
{
	STAGE_COORD convCoord;

	convCoord.lX = pCoordXy->lX;
	convCoord.lY = pCoordXy->lY;
	NS_ConvertToStageMoveCoord(&convCoord);

	return NS_StageMoveAbsoluteAsync(&convCoord);
}

//---------------------------------------------------------------------------
// NS_SampleCoordStageMoveAbsoluteEx
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsoluteEx(const STAGE_COORD* pCoordXy, const STAGE_COORD* pSampleOriginCoordXy, int iMeasHead /* = -1*/)
{
	STAGE_COORD headOffset;
	STAGE_COORD convCoord;
	STAGE_COORD deskewCoord;

	if ( iMeasHead == -1 ) {
		iMeasHead = wActiveHeadType;
	}
	NS_GetDiffHeadTypeOffset(iMeasHead, &headOffset);

	convCoord.lX = pCoordXy->lX + headOffset.lX;
	convCoord.lY = pCoordXy->lY + headOffset.lY;
	NS_ConvertToStageMoveCoord(&convCoord, pSampleOriginCoordXy);

	DeskewCorrectToCurrent(convCoord, deskewCoord);

	return NS_StageMoveAbsoluteAsync(&deskewCoord);
}

//---------------------------------------------------------------------------
// NS_SampleCoordStageMoveAbsoluteEx
BOOL NSSTAGEAPI NS_SampleCoordStageMoveAbsoluteEx(const STAGE_COORD* pCoordXy, LPCTSTR pszCurrentMainRecipeName, int iMeasHead /* = -1*/)
{
	STAGE_COORD headOffset;
	STAGE_COORD convCoord;
	STAGE_COORD deskewCoord;

	if ( iMeasHead == -1 ) {
		iMeasHead = wActiveHeadType;
	}
	NS_GetDiffHeadTypeOffset(iMeasHead, &headOffset);

	convCoord.lX = pCoordXy->lX + headOffset.lX;
	convCoord.lY = pCoordXy->lY + headOffset.lY;
	NS_ConvertToStageMoveCoord(&convCoord, pszCurrentMainRecipeName);

	DeskewCorrectToCurrent(convCoord, deskewCoord);

	return NS_StageMoveAbsoluteAsync(&deskewCoord);
}

//---------------------------------------------------------------------------
// NS_StageMoveAbsoluteAsync
BOOL NSSTAGEAPI NS_StageMoveAbsoluteAsync(const STAGE_COORD* pStageMoveCoordXy)
{
	if ( !NS_IsIdleStageMoveAsync() )
		return FALSE;

	asyncStageMoveCoord = *pStageMoveCoordXy;
	bThread = TRUE;
	SetEvent(hEvent);

	return TRUE;
}

//---------------------------------------------------------------------------
// NS_StageMoveThread
UINT WINAPI NS_StageMoveThread(LPVOID lpData)
{
	while  ( 1 ) {
		::WaitForSingleObject(hEvent, INFINITE);
		if ( bShutDown ) {
			bThread = FALSE;
			break;
		}
		StageMoveAbsolute(&asyncStageMoveCoord);
		bThread = FALSE;
	}

	return 0;
}

//---------------------------------------------------------------------------
// NS_IsIdleStageMoveAsync
BOOL NSSTAGEAPI NS_IsIdleStageMoveAsync()
{
	BOOL bRet;

	switch ( nStageType ) {
	case STAGE_TYPE_SPT:
		bRet = (bThread == FALSE && StageIsIdle());
		break;
	default:
		bRet = (bThread == FALSE);
		break;
	}

	return bRet;
}

//---------------------------------------------------------------------------
// NS_SetStageSpeed
BOOL NSSTAGEAPI NS_SetStageSpeed(long lSpeed)
{
	if ( STAGE_TYPE_SPT != nStageType )
		return FALSE;

	StageSetLocalSpeed(lSpeed, lSpeed);

	return TRUE;
}

//---------------------------------------------------------------------------
// NS_RestoreStageSpeed
BOOL NSSTAGEAPI NS_RestoreStageSpeed()
{
	if ( STAGE_TYPE_SPT != nStageType )
		return FALSE;

	StageSetLocalSpeed(lSpeedX, lSpeedY);

	return TRUE;
}

// 2013.02.22 bagus Substrate thickness setting -->
//---------------------------------------------------------------------------
// NS_SelectSubstrateThickness
BOOL NSSTAGEAPI NS_SelectSubstrateThickness(int iIndex)
{
	TRACE1(_T("NS_SelectSubstrateThickness() : %d\n"), iIndex);

	if ( iActiveSubstrateThickness == iIndex )
		return TRUE;

	//
	if ( iIndex < 0 || SUBSTRATE_THICKNESS_MAX <= iIndex ) {
		return FALSE;
	}

	SUBSTRATE_THICKNESS_TABLE substrateThicknessTable;
	ConfigFile_GetNanoSpecIni(&substrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);

	SUBSTRATE_THICKNESS substrateThickness;
	substrateThickness = substrateThicknessTable.SubstrateThickness[iIndex];

	if ( !substrateThickness.bUseData ) {
		return FALSE;
	}

	//
	StageSetZSubThickOffset((long)substrateThickness.dThickness);

	//
	StageSetAFInvalidRegion((long)substrateThickness.dInvalidRegion);

	// ���݂̃C���f�b�N�X��ۑ����Ă���
	iActiveSubstrateThickness = iIndex;

	return TRUE;
}

//---------------------------------------------------------------------------
// NS_GetCurrentSubstrateThickness
int NSSTAGEAPI NS_GetCurrentSubstrateThickness()
{
	return iActiveSubstrateThickness;
}
// 2013.02.22 bagus Substrate thickness setting <--
