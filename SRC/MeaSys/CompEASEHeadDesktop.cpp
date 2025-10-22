// CompEASEHeadDesktop.cpp : Defines the CompEASEHeadDesktop routines.
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include "../../INC/Globals.hxx"
#include "../../INC/Common.hxx"
#include "../../INC/MeaSys.hxx"
#include "resource.h"
#include "LogFile.h"
#include "Mojiretsu.h"
#include "CompEASEHead.h"

#define BUFFER_LEN			(1024 * 4)

extern CLogFile* pLogFile;
extern CMojiretsu* pMojiretsu;
extern HWND g_hNotifyWnd;

//---------------------------------------------------------------------------
// InitInstance
BOOL CCompEASEHeadDesktop::InitInstance()
{
	TRACE(_T("CCompEASEHeadDesktop::InitInstance()\n"));

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CCompEASEHeadDesktop::ExitInstance()
{
	TRACE(_T("CCompEASEHeadDesktop::ExitInstance()\n"));
}

//---------------------------------------------------------------------------
// Ready
BOOL CCompEASEHeadDesktop::Ready()
{
	return TRUE;
}

//---------------------------------------------------------------------------
// Status
int CCompEASEHeadDesktop::Status()
{
	return TRUE;
}

//---------------------------------------------------------------------------
// OpenShutter
BOOL CCompEASEHeadDesktop::OpenShutter()
{
	return TRUE;
}

//---------------------------------------------------------------------------
// CloseShutter
BOOL CCompEASEHeadDesktop::CloseShutter()
{
	return TRUE;
}

//---------------------------------------------------------------------------
// ListRecipes
BOOL CCompEASEHeadDesktop::ListRecipes(LPTSTR pszListRecipes)
{
	CStringArray strListRecipes;
	CString strBuffer;

	if ( pszListRecipes == NULL ) {
		return FALSE;
	}

	strListRecipes.RemoveAll();
	int i;

	for ( i = 0; i < 100; i++ ) {
		if ( i == 0 ) {
			strBuffer.Format("TestRecipe%d.recipe", i + 1);
		}
		else {
			strBuffer.Format("/TestRecipe%d.recipe", i + 1);
		}
		strListRecipes.Add(strBuffer);
	}

	pszListRecipes[0] = '\0';
	for ( i = 0; i < strListRecipes.GetSize(); i++ ) {
		_tcscat(pszListRecipes, strListRecipes[i]);
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// RunRecipe
BOOL CCompEASEHeadDesktop::RunRecipe(LPCTSTR pszRecipeId, LPTSTR pszMeasResult)
{
// 	if ( g_hNotifyWnd )
// 		::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);

	LPCTSTR pszExample = "MSE = 661.034,Roughness = 27.04 } 10.458 nm,Thickness # 2 = 67.77 } 10.700 nm,Einf = 2.236 } 13.2609,Amp1 = 154.4219 } 92.60231,Br1 = 1.587 } 2.2363,Eo1 = 3.034 } 0.3289,Amp2 = 9.461936 } 202.9963749,Br2 = 0.6418 } 4.76635,En2 = 7.281 } 30.8794,Resistivity (OhmEcm)3 = 1000.000000 } 2.1192E+11,Scat. Time (fs)3 = 376.671 } 1.6939E+10,Thickness # 1 = 6260.44 } 10.171 nm,N1 = 1.46791,K1 = 0.00000";

	_tcscpy(pszMeasResult, pszExample);

	return TRUE;
}
