#pragma once

// 2009.09.29 K.Matsuo ../NanoSpec/SubTransition.cppから引越し＆リネーム -->

class CSrRefFile
{
public:
	static int CheckRefFileElapsedTimeOut(LPCTSTR pszMainRecipeName, double dLifeTime);
	static BOOL ReadRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL ReadGTrRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName, int iCoordRefPos);
	static BOOL ReadRefFile2ndRefT1(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL ReadRefFile2ndRefT2(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL WriteRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL WriteGTrRefFile(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL WriteRefFile2ndRefT1(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL WriteRefFile2ndRefT2(SCANDATA* pScanData, LPCTSTR pszMainRecipeName);
	static BOOL Is2ndRefT1FileExist(LPCTSTR pszMainRecipeName);
	static BOOL Is2ndRefT2FileExist(LPCTSTR pszMainRecipeName);
};

// 2009.09.29 K.Matsuo ../NanoSpec/SubTransition.cppから引越し＆リネーム <--
