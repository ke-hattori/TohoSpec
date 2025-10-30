// NanoSpec.cpp : ï¿½Aï¿½vï¿½ï¿½ï¿½Pï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½pï¿½Nï¿½ï¿½ï¿½Xï¿½Ì‹@ï¿½\ï¿½ï¿½`ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
//

#include "stdafx.h"
#define _MASTER_
#include "System.h"
#include "NanoSpec.h"
#include "NanoSpecDoc.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"
#include "NanoSpecView.h"
#include "AlarmLogDlg.h"
#include "DebugDlg.h"
#include "NanoSpecIni.h"
#include "..\\..\\INC\\Title.HXX"
#include "..\\..\\INC\\NEXIOBASE.HXX"
#include "..\\..\\INC\\NEXIF.HXX"
#include "InterlockDlg.h"
//Saiki 20090525 Add --->
#include <globals.hxx>
//Saiki 20090525 Add <---
#include "../../INC/NSStage.hxx"
#include "LensComboBox.h"
#include "MotionComboBox.h"

/* deleted 2009.07.16 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ï¿½í�œ ---------- { ---------- */
///* added 2009.07.13 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ---------- { ---------- */
///*
// *	ï¿½xï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½hï¿½Ì�İ’ï¿½
// */
//#pragma comment(lib, "delayimp.lib")
//#ifdef _DEBUG	/* ï¿½fï¿½oï¿½bï¿½Oï¿½rï¿½ï¿½ï¿½h -------------------------*/
//#pragma comment(linker, "/delayload:dTnsConfigFile.dll")
//#pragma comment(linker, "/delayload:dTnsRecipeFile.dll")
//#pragma comment(linker, "/delayload:dTnsStage.dll")
//#pragma comment(linker, "/delayload:dTnsMeaSys.dll")
//#pragma comment(linker, "/delayload:dTnsHook.dll")
//#pragma comment(linker, "/delayload:dTnsPatRec.dll")
//#pragma comment(linker, "/delayload:dTnsOverlay.dll")
//#pragma comment(linker, "/delayload:dTnsAlarmIf.dll")
//#pragma comment(linker, "/delayload:dTnsPifComm.dll")
//#pragma comment(linker, "/delayload:dTnsNexIo.dll")
//#pragma comment(linker, "/delayload:dTnsNexIf.dll")
//#pragma comment(linker, "/delayload:dTnsDeskew.dll")
//#pragma comment(linker, "/delayload:dTnsPatternFile.dll")
//#else			/* ï¿½ï¿½ï¿½ï¿½ï¿½[ï¿½Xï¿½rï¿½ï¿½ï¿½h--------------------------*/
//#pragma comment(linker, "/delayload:TnsConfigFile.dll")
//#pragma comment(linker, "/delayload:TnsRecipeFile.dll")
//#pragma comment(linker, "/delayload:TnsStage.dll")
//#pragma comment(linker, "/delayload:TnsMeaSys.dll")
//#pragma comment(linker, "/delayload:TnsHook.dll")
//#pragma comment(linker, "/delayload:TnsPatRec.dll")
//#pragma comment(linker, "/delayload:TnsOverlay.dll")
//#pragma comment(linker, "/delayload:TnsAlarmIf.dll")
//#pragma comment(linker, "/delayload:TnsPifComm.dll")
//#pragma comment(linker, "/delayload:TnsNexIo.dll")
//#pragma comment(linker, "/delayload:TnsNexIf.dll")
//#pragma comment(linker, "/delayload:TnsDeskew.dll")
//#pragma comment(linker, "/delayload:TnsPatternFile.dll")
//#endif
///* added 2009.07.13 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ---------- } ---------- */
/* deleted 2009.07.16 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ï¿½í�œ ---------- } ---------- */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL StageSelectLensNotify(UINT nLens)
{
	if (!NS_StageSelectLens(nLens))
		return FALSE;
	for (std::list<HWND>::iterator i = CLensComboBox::m_hWndList.begin(); i != CLensComboBox::m_hWndList.end(); i++)
		::PostMessage(*i, WM_SELLENSCHANGE, 0L, 0L);
	return TRUE;
}

BOOL SelectHeadTypeNotify(WORD wHeadType, BOOL bStageMove)
{
	if (!NS_SelectHeadType(wHeadType, bStageMove))
		return FALSE;
	for (std::list<HWND>::iterator i = CTypeComboBox::m_hWndList.begin(); i != CTypeComboBox::m_hWndList.end(); i++)
		::PostMessage(*i, WM_SELHEADTYPECHANGE, 0L, 0L);
	return TRUE;
}

// 2013.02.22 bagus Substrate thickness setting -->
BOOL SelectSubThickNotifyIndex(int iIndex)
{
	if (!NS_SelectSubstrateThickness(iIndex))
		return FALSE;
	for (std::list<HWND>::iterator i = CSubstrateThicknessComboBox::m_hWndList.begin(); i != CSubstrateThicknessComboBox::m_hWndList.end(); i++)
		::PostMessage(*i, WM_SELSUBTHICKCHANGE, 0L, 0L);
	return TRUE;
}

BOOL SelectSubThickNotify(double dThickness)
{
	SUBSTRATE_THICKNESS_TABLE substrateThicknessTable;
	ConfigFile_GetNanoSpecIni(&substrateThicknessTable, CONFIG_FILE_SUBSTRATE_THICKNESS_CONFIG);

	// ï¿½ÂŒï¿½ï¿½ï¿½ï¿½ï¿½Ô�ï¿½ï¿½ï¿½ï¿½tï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Bï¿½ï¿½ï¿½tï¿½@ï¿½ï¿½ï¿½ï¿½ï¿½Xï¿½Í‘Î�ÛŠO
	SUBSTRATE_THICKNESS substrateThickness;
	double dRoundThickness;

	// 1umï¿½Ì’Pï¿½Ê‚Í�lï¿½ÌŒÜ“ï¿½ï¿½ÅŠÛ‚ß‚Ä‘Î‰ï¿½ï¿½ï¿½ï¿½è‚¢ï¿½ï¿½ï¿½Ü‚ï¿½
	// (Ex.724umï¿½ï¿½720um)
	dRoundThickness = ((int)((dThickness + 5.0) / 10.0)) * 10.0;
	for (int i = 1; i < SUBSTRATE_THICKNESS_MAX; i++) {
		substrateThickness = substrateThicknessTable.SubstrateThickness[i];
		if (substrateThickness.bUseData) {
			if (dRoundThickness == substrateThickness.dThickness) {
				return SelectSubThickNotifyIndex(i);
			}
		}
	}

	return FALSE;	// not found.
}
// 2013.02.22 bagus Substrate thickness setting <--

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecApp                                                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CNanoSpecApp, CWinApp)
	//{{AFX_MSG_MAP(CNanoSpecApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDM_DEBUG, OnDebug)
	//}}AFX_MSG_MAP
	// ï¿½Wï¿½ï¿½ï¿½Ìƒtï¿½@ï¿½Cï¿½ï¿½ï¿½ï¿½{ï¿½hï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½g ï¿½Rï¿½}ï¿½ï¿½ï¿½h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ï¿½Wï¿½ï¿½ï¿½Ìˆï¿½ï¿½ï¿½Zï¿½bï¿½gï¿½Aï¿½bï¿½v ï¿½Rï¿½}ï¿½ï¿½ï¿½h
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecApp ï¿½Nï¿½ï¿½ï¿½Xï¿½Ì�\ï¿½z

/////////////////////////////////////////////////////////////////////////////
//
CNanoSpecApp::CNanoSpecApp()
{
	// TODO: ï¿½ï¿½ï¿½ÌˆÊ’uï¿½É�\ï¿½zï¿½pï¿½Rï¿½[ï¿½hï¿½ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B
	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ InitInstance ï¿½ï¿½ï¿½Ì�dï¿½vï¿½È�ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½×‚Ä‹Lï¿½qï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½B
/* added 2009.06.05 hmenjo NanoSpecTitle ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Î�ï¿½ ---------- { ---------- */
	m_dwProcIdNanoTitle = 0;
	/* added 2009.06.05 hmenjo NanoSpecTitle ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Î�ï¿½ ---------- } ---------- */
}

/////////////////////////////////////////////////////////////////////////////
// ï¿½Bï¿½ï¿½ï¿½ CNanoSpecApp ï¿½Iï¿½uï¿½Wï¿½Fï¿½Nï¿½g

CNanoSpecApp theApp;

/* added 2010.11.16 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½S WVASE ï¿½Iï¿½ï¿½ ---------- { ---------- */
#include <tlhelp32.h>
void CNanoSpecApp::ExitNanoProcesses(TCHAR(*ptszProcList)[][_MAX_FNAME])
{
	//	TCHAR (*l_ptszProcList)[][_MAX_FNAME] = (TCHAR(*)[][_MAX_FNAME]) pvProcList;
	//	l_ptszProcList = (TCHAR(*)[][_MAX_FNAME]) pvProcList;
		/* ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ï¿½ï¿½Xï¿½gï¿½É‚ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½È‚ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	HANDLE l_hSnapProc = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (0 != l_hSnapProc) {
		BOOL l_bClose;		/* WM_CLOSE ï¿½Ì�ï¿½ï¿½ï¿½	*/
		long li;			/* ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ï¿½ï¿½Xï¿½gï¿½zï¿½ï¿½Ì—vï¿½fï¿½Ô�ï¿½	*/
		HANDLE l_hProc;		/* ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½nï¿½ï¿½ï¿½hï¿½ï¿½	*/
		HWND l_hWnd;		/* ï¿½Tï¿½ï¿½ï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½nï¿½ï¿½ï¿½hï¿½ï¿½	*/
		DWORD l_dwProcID;	/* ï¿½Tï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½X ID	*/
		DWORD l_dwExitCode;	/* ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½Ì�Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½	*/
		DWORD l_dwTimeSta;	/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½pï¿½Jï¿½nï¿½ï¿½ï¿½ï¿½	*/
		PROCESSENTRY32 l_ProcEntry32;
		BOOL l_bRc = ::Process32First(l_hSnapProc, &l_ProcEntry32);
		while (0 != l_bRc) {
			li = 0;
			while (0 != _tcsicmp((*ptszProcList)[li], _T(""))) {
				if (0 == _tcsicmp(l_ProcEntry32.szExeFile, (*ptszProcList)[li])) {
					/* ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ï¿½ï¿½Xï¿½gï¿½É‚ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
					l_bClose = FALSE;
					l_hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, l_ProcEntry32.th32ProcessID);
					/* ï¿½ï¿½ï¿½Ìƒvï¿½ï¿½ï¿½Zï¿½X ID ï¿½ï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½nï¿½ï¿½ï¿½hï¿½ï¿½ï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½é‚½ï¿½ß‚É‘Sï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½Tï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
/* modified 2013.06.12 hmenjo ExitNanoProcesses ---------- { ---------- */
//					l_hWnd = ::GetTopWindow(0);
//					while (0 != l_hWnd) {
//						::GetWindowThreadProcessId(l_hWnd, &l_dwProcID);
//						if (l_dwProcID == l_ProcEntry32.th32ProcessID) {
//							/* ï¿½Tï¿½ï¿½ï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Ìƒvï¿½ï¿½ï¿½Zï¿½X ID ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
//							::SendMessage(l_hWnd, WM_CLOSE, 0, 0);
//							/* ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½Ì�Iï¿½ï¿½ï¿½mï¿½Fï¿½Å‚ï¿½ï¿½D	*/
//							if (0 != l_hProc) {
//								l_dwTimeSta = ::GetTickCount();
//								do {
//									if (FALSE == ::GetExitCodeProcess(l_hProc, &l_dwExitCode)) {
//										break;
//									} else {
//										if (STILL_ACTIVE == l_dwExitCode) {
//											::Sleep(100);
//										} else {
//											l_bClose = TRUE;
//											break;
//										}
//									}
//								} while ((::GetTickCount() - l_dwTimeSta) <= 5000);
//							}
//							break;
//						}
//						l_hWnd = ::GetNextWindow(l_hWnd, GW_HWNDNEXT);
//					}
/* modified 2013.06.12 hmenjo ExitNanoProcesses ----------              */
					HWND l_hwndTop[2][16];	memset(l_hwndTop, 0, sizeof(l_hwndTop));
					HWND l_hwndOwn[16];	memset(l_hwndOwn, 0, sizeof(l_hwndOwn));
					l_hWnd = ::GetTopWindow(0);
					int l_iTop = 0;
					int l_iOwn = 0;
					while (0 != l_hWnd) {
						::GetWindowThreadProcessId(l_hWnd, &l_dwProcID);
						if (l_dwProcID == l_ProcEntry32.th32ProcessID) {
							/* ï¿½Tï¿½ï¿½ï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Ìƒvï¿½ï¿½ï¿½Zï¿½X ID ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½ï¿½D	*/
							l_hwndTop[0][l_iTop] = l_hWnd;
							l_hwndTop[1][l_iTop] = ::GetWindow(l_hWnd, GW_OWNER);
							if (0 != l_hwndTop[1][l_iTop]) {
								l_hwndOwn[l_iOwn] = l_hwndTop[1][l_iTop];
								l_iOwn++;
							}
							l_iTop++;
						}
						l_hWnd = ::GetNextWindow(l_hWnd, GW_HWNDNEXT);
					}
					if (0 < l_iOwn) {
						int l_iO;
						int l_iT;
						/* ï¿½Iï¿½[ï¿½iï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½[ï¿½iï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½oï¿½^ï¿½ï¿½ï¿½ï¿½í�œï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
						for (l_iO = 0; l_iO < l_iOwn; l_iO++) {
							for (l_iT = 0; l_iT < l_iTop; l_iT++) {
								if ((l_hwndOwn[l_iO] == l_hwndTop[0][l_iT]) &&
									(0 != l_hwndTop[1][l_iT])) {
									break;
								}
							}
							if (l_iT < l_iTop) {
								l_hwndOwn[l_iO] = 0;
							}
						}
						/* ï¿½cï¿½ï¿½ï¿½ï¿½ï¿½Iï¿½[ï¿½iï¿½ï¿½ï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Æ‚ï¿½ï¿½ï¿½ WM_CLOSE ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
						for (l_iO = 0; l_iO < l_iOwn; l_iO++) {
							//							if (0 != l_hwndOwn[l_iO]) {
							::PostMessage(l_hwndOwn[l_iO], WM_CLOSE, 0, 0);
							//								break;
							//							}
						}
					}
					l_dwTimeSta = ::GetTickCount();
					do {
						if (FALSE == ::GetExitCodeProcess(l_hProc, &l_dwExitCode)) {
							break;
						}
						else {
							if (STILL_ACTIVE == l_dwExitCode) {
								::Sleep(100);
							}
							else {
								l_bClose = TRUE;
								break;
							}
						}
					} while ((::GetTickCount() - l_dwTimeSta) <= 5000);
					/* modified 2013.06.12 hmenjo ExitNanoProcesses ---------- } ---------- */
					if (FALSE == l_bClose) {
						/* WM_CLOSE ï¿½Å�Iï¿½ï¿½ï¿½oï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½Ì‚Å‹ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
						/*		ï¿½ï¿½ï¿½é‚¢ï¿½ÍƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½D	*/
						if (0 != l_hProc) {
							if (FALSE != ::TerminateProcess(l_hProc, 0)) {
								l_dwTimeSta = ::GetTickCount();
								do {
									if (FALSE == ::GetExitCodeProcess(l_hProc, &l_dwExitCode)) {
										break;
									}
									else {
										if (STILL_ACTIVE == l_dwExitCode) {
											::Sleep(100);
										}
										else {
											l_bClose = TRUE;
											break;
										}
									}
								} while ((::GetTickCount() - l_dwTimeSta) <= 5000);
								if (FALSE == l_bClose) {
									/* ï¿½Iï¿½ï¿½ï¿½oï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½Å‚ï¿½ï¿½ï¿½ï¿½D	*/
								}
							}
						}
					}
					::CloseHandle(l_hProc);
					break;
				}
				li++;
			}
			l_bRc = ::Process32Next(l_hSnapProc, &l_ProcEntry32);
		}
	}
}
/* added 2010.11.16 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½S WVASE ï¿½Iï¿½ï¿½ ---------- } ---------- */

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecApp ï¿½Nï¿½ï¿½ï¿½Xï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½
BOOL CNanoSpecApp::InitInstance()
{
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
	g_lAppNameType = APP_NAME_NANO; // default application name
	for (int i = 0; i < APP_NAME_MAX; i++) {
		if (_tcsncmp(m_pszExeName, g_lpszAppPrefix4[i], 4) == 0) {
			g_lAppNameType = i;
			break;
		}
	}
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	/* added 2014.12.24 hmenjo ï¿½ï¿½ï¿½dï¿½Nï¿½ï¿½ï¿½hï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½Ú“ï¿½ ---------- { ---------- */
	m_hMutex_Run = ::CreateMutex(NULL, TRUE, m_pszExeName);
	if ((0 == m_hMutex_Run) ||
		(::GetLastError() == ERROR_ALREADY_EXISTS)) {
		TCHAR l_tszGetCaption[MAX_PATH];
		TCHAR l_tszCaptionMain[MAX_PATH];
		BOOL l_bFound = FALSE;
		int i;
		CWnd* l_pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while ((0 != l_pPrevWnd) && (FALSE == l_bFound)) {
			memset(l_tszGetCaption, 0, sizeof(l_tszGetCaption));
			l_pPrevWnd->GetWindowText(l_tszGetCaption, (sizeof(l_tszGetCaption) / sizeof(TCHAR)) - 1);
			for (i = 0; i < APP_NAME_MAX; i++) {
				_stprintf(l_tszCaptionMain, _T("%sSpec -"), g_lpszAppPrefix4[i]);
				if (_tcslen(l_tszCaptionMain) < _tcslen(l_tszGetCaption)) {
					l_tszGetCaption[_tcslen(l_tszCaptionMain)] = 0;
				}
				if (0 == _tcscmp(l_tszCaptionMain, l_tszGetCaption)) {
					l_bFound = TRUE;
					if (l_pPrevWnd->IsIconic()) {
						l_pPrevWnd->ShowWindow(SW_RESTORE);
					}

					l_pPrevWnd->SetForegroundWindow();
					l_pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
					break;
				}
			}
			l_pPrevWnd = l_pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		return FALSE;
	}
	/* added 2014.12.24 hmenjo ï¿½ï¿½ï¿½dï¿½Nï¿½ï¿½ï¿½hï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½Ú“ï¿½ ---------- } ---------- */

	MakePath();
	/* deleted 2009.07.16 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ï¿½í�œ ---------- { ---------- */
	///* added 2009.07.13 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ---------- { ---------- */
	//	SetEnvNanoPath();
	///* added 2009.07.13 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ---------- } ---------- */
	/* deleted 2009.07.16 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ï¿½í�œ ---------- } ---------- */
	/* added 2010.11.16 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½S WVASE ï¿½Iï¿½ï¿½ ---------- { ---------- */
	TCHAR l_tszProcList1[][_MAX_FNAME] = {
			_T("jaw_adap.exe"),
			// 2014.03.08 bagus SCOUT added -->
						_T("SC_adap.exe"),
						// 2014.03.08 bagus SCOUT added <--
									0
	};
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
	// 	TCHAR l_tszProcList2[][_MAX_FNAME] = {
	// 			_T("wvase32.exe"),
	// 			_T("nextra.exe"),
	// 			_T("nanopifsock.exe"),
	// 			0
	// 		};
	TCHAR l_tszProcList2[4][_MAX_FNAME];
	CString strBuffer;

	_tcscpy(l_tszProcList2[0], _T("wvase32.exe"));
	_tcscpy(l_tszProcList2[1], _T("nextra.exe"));
	// 2014.03.08 bagus SCOUT added -->
	_tcscpy(l_tszProcList2[2], _T("SCOUT.exe"));
	// 2014.03.08 bagus SCOUT added <--
	strBuffer = _T("NanoPifSock.exe");
	if (g_lAppNameType != APP_NAME_NANO) {
		strBuffer.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}
	_tcscpy(l_tszProcList2[2], strBuffer);
	_tcscpy(l_tszProcList2[3], "");
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	this->ExitNanoProcesses((TCHAR(*)[][_MAX_FNAME]) & l_tszProcList1);
	this->ExitNanoProcesses((TCHAR(*)[][_MAX_FNAME]) & l_tszProcList2);
	/* added 2010.11.16 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½S WVASE ï¿½Iï¿½ï¿½ ---------- } ---------- */

		//Saiki 20090525 Add ----->
	// 2013.11.07 Bagus Delete (TohoSpecï¿½Î‰ï¿½) -->
	//	CString strBuffer;
	// 2013.11.07 Bagus Delete (TohoSpecï¿½Î‰ï¿½) <--
	SYSTEM_CONFIG l_SystemConfig;

	///// NanoSpec.iniï¿½ÌƒOï¿½ï¿½ï¿½[ï¿½oï¿½ï¿½ï¿½Ï�ï¿½ï¿½Ì�ï¿½ï¿½ï¿½ï¿½ï¿½ /////
	ConfigFile_LoadAllNanoSpecIni();
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	HANDLE m_hModule;
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
#ifdef _DEBUG
	strBuffer.Format("dtns%sspec", g_lpszAppPrefix4[g_lAppNameType]);
#else
	strBuffer.Format("tns%sspec", g_lpszAppPrefix4[g_lAppNameType]);
#endif
	m_hModule = ResourceLoadLibrary(l_SystemConfig.nLanguage, strBuffer);
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	AfxSetResourceHandle((HINSTANCE)m_hModule);
	//Saiki 20090525 Add <-----
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm && !AfxOleInit()) {
		//Saiki 20090525 Change ----->
	//		AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
	//		  AfxMessageBox(_T("OLE ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½É�ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½. OLEï¿½ï¿½ï¿½Cï¿½uï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½oï¿½[ï¿½Wï¿½ï¿½ï¿½ï¿½ï¿½Å‚ï¿½ï¿½é‚©ï¿½mï¿½Fï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"));
		LoadStringML(IDS_OLE_INITIALIZATION_FAILED, strBuffer, "OLE initialization failed. Make sure that the OLE libraries are the correct version");
		AfxMessageBox(strBuffer);
	}
	//Saiki 20090525 Change <-----

	if (!AfxSocketInit()) {
		//Kojika 20090525 Change
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		LoadStringML(IDS_SOCKETS_INIT_FAILED, strBuffer, "Windows socket initialization failed.");
		AfxMessageBox(strBuffer);
		//Kojika 20090525 Change End
		return FALSE;
	}

	AfxEnableControlContainer();

	/* added 2014.12.24 hmenjo ï¿½ï¿½ï¿½dï¿½Nï¿½ï¿½ï¿½hï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½Ú“ï¿½ ---------- { ---------- */
	//	// 2ï¿½dï¿½Nï¿½ï¿½ï¿½Ì–hï¿½~
	//	::CreateMutex(NULL, TRUE, m_pszExeName);
	//	if( ::GetLastError() == ERROR_ALREADY_EXISTS)
	//	{
	//		CWnd* pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
	//		while(pPrevWnd)
	//		{
	//			if(::GetProp(pPrevWnd->GetSafeHwnd(), m_pszExeName))			// ::SetProp
	//			{
	//				if(pPrevWnd->IsIconic())
	//					pPrevWnd->ShowWindow(SW_RESTORE);
	//
	//				pPrevWnd->SetForegroundWindow();
	//				pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
	//			}
	//			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
	//		}
	//		return FALSE;
	//	}
	/* added 2014.12.24 hmenjo ï¿½ï¿½ï¿½dï¿½Nï¿½ï¿½ï¿½hï¿½~ï¿½ï¿½ï¿½ï¿½ï¿½Ú“ï¿½ ---------- } ---------- */

#ifdef _AFXDLL
	// Enable3dControls(); 	// No longer needed in modern MFC
#else
	// Enable3dControlsStatic();	// No longer needed in modern MFC
#endif

	//First free the string allocated by MFC at CWinApp startup.
	//The string is allocated before InitInstance is called.
	free((void*)m_pszProfileName);

	//Change the name of the .INI file.
	//The CWinApp destructor will free the memory.
	char szFilePath[MAX_PATH];
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
	//	sprintf(szFilePath, "%s%s", g_szCfg_Dir, NANOSPEC_INIFILENAME);
	CString strFilename;

	strFilename = NANOSPEC_INIFILENAME;
	if (g_lAppNameType != APP_NAME_NANO) {
		strBuffer.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
	}
	sprintf(szFilePath, "%s%s", g_szCfg_Dir, (LPCSTR)strFilename);
	// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
	m_pszProfileName = _tcsdup(szFilePath);

	// ï¿½Aï¿½vï¿½ï¿½ï¿½Pï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ï¿½pï¿½Ìƒhï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½g ï¿½eï¿½ï¿½ï¿½vï¿½ï¿½ï¿½[ï¿½gï¿½ï¿½oï¿½^ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½Bï¿½hï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½g ï¿½eï¿½ï¿½ï¿½vï¿½ï¿½ï¿½[ï¿½g
	//	ï¿½Íƒhï¿½Lï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½gï¿½Aï¿½tï¿½ï¿½ï¿½[ï¿½ï¿½ ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½Æƒrï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é‚½ï¿½ß‚É‹@ï¿½\ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		//		IDR_MAINFRAME,
		IDR_MEASUREMENT_MENU,
		RUNTIME_CLASS(CNanoSpecDoc),
		RUNTIME_CLASS(CMainFrame),		// ï¿½ï¿½ï¿½Cï¿½ï¿½ SDI ï¿½tï¿½ï¿½ï¿½[ï¿½ï¿½ ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½E
		RUNTIME_CLASS(CNanoSpecView));
	AddDocTemplate(pDocTemplate);

	///// NanoSpec INI //////
	CNanoSpecIni NanoSpecIni;

	///// Start NanoSpec Title /////
	NanoSpecTitle(NANOSPEC_TITLE_START);

	///// Start Nextra.exe /////
// 2009.12.29 K.Matsuo Nextra.exeï¿½ï¿½ï¿½cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ -->
	NextraExe(NEXTRA_EXE_QUIT);
	// 2009.12.29 K.Matsuo Nextra.exeï¿½ï¿½ï¿½cï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½ï¿½ï¿½Aï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ <--
	if (!l_SystemConfig.bDisableNextra) {
		NextraExe(NEXTRA_EXE_START);
	}

	// DDEï¿½Afile open ï¿½È‚Ç•Wï¿½ï¿½ï¿½ÌƒVï¿½Fï¿½ï¿½ ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ÌƒRï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í‚ï¿½ï¿½Ü‚ï¿½ï¿½B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½Åƒfï¿½Bï¿½Xï¿½pï¿½bï¿½` ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½wï¿½è‚µï¿½Ü‚ï¿½ï¿½B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// MAIN MENU LOGON ï¿½ï¿½\ï¿½ï¿½ //
	CWnd* pMain = AfxGetMainWnd();
	CMenu* pMenu = pMain->GetMenu();
	pMenu->DeleteMenu(IDM_LOGON, MF_BYCOMMAND);
	// 2013.11.12 Bagus Add (TohoSpecï¿½Î‰ï¿½) -->
	// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
	//	if(g_lModelType == MODEL_T3100){
	if (l_SystemConfig.nStageType == STAGE_TYPE_NONE) {
		// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--
		CMenu* pSubMenu = pMenu->GetSubMenu(0);
		/* modified 2014.05.22 hmenjo ï¿½è“®ï¿½ï¿½ NanoMap ---------- { ---------- */
		//		pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
		/* modified 2014.05.22 hmenjo ï¿½è“®ï¿½ï¿½ NanoMap ----------              */
		if (g_lModelType != MODEL_T3100) {
			pSubMenu->DeleteMenu(IDM_DATA, MF_BYCOMMAND);
		}
		/* modified 2014.05.22 hmenjo ï¿½è“®ï¿½ï¿½ NanoMap ---------- } ---------- */

		pSubMenu = pMenu->GetSubMenu(1);

		CString str;
		pSubMenu->GetMenuString(IDM_MEASUREMENT_START, str, MF_BYCOMMAND);
		pSubMenu->ModifyMenu(IDM_MANUAL_MEASUREMENT, MF_BYCOMMAND, IDM_MANUAL_MEASUREMENT, str);

		pSubMenu->DeleteMenu(IDM_MEASUREMENT_START, MF_BYCOMMAND);
		pSubMenu->DeleteMenu(IDM_HANDLER_LOAD_POSITION, MF_BYCOMMAND);
		pSubMenu->DeleteMenu(IDM_REFERENCE, MF_BYCOMMAND);
		pSubMenu->DeleteMenu(IDM_VACUUM, MF_BYCOMMAND);
		pSubMenu->DeleteMenu(IDM_MANUAL_STAGE, MF_BYCOMMAND);

		pSubMenu->DeleteMenu(1, MF_BYPOSITION);
	}
	// 2013.11.12 Bagus Add (TohoSpecï¿½Î‰ï¿½) <--

		//ï¿½Vï¿½Xï¿½eï¿½ï¿½ï¿½Rï¿½ï¿½ï¿½tï¿½Bï¿½Oï¿½İ’ï¿½Ç‚İ�ï¿½ï¿½ï¿½
	SYSTEM_CONFIG systemConfig;
	ConfigFile_GetNanoSpecIni(&systemConfig, CONFIG_FILE_SYSTEM_CONFIG);
	if (!systemConfig.bHost) {
		pMenu->DeleteMenu(2, MF_BYPOSITION);
	}
	m_pMainWnd->DrawMenuBar();
	/* added 2014.05.22 hmenjo get Xmp Type ---------- { ---------- */
		/* get adap exe path	*/
	SR_XMP l_srXmp;
	ConfigFile_GetNanoSpecIni(&l_srXmp, CONFIG_FILE_SR_XMP);
	TCHAR l_tszAdapPath[MAX_PATH];
	if (l_srXmp.szAdapExePath[0] != '\0') {
		_tcscpy(l_tszAdapPath, l_srXmp.szAdapExePath);
	}
	else {
		_tcscpy(l_tszAdapPath, _T("C:\\WVASE32\\Adap\\jaw_adap.exe"));
	}
	_tcslwr(l_tszAdapPath);
	if (0 != _tcsstr(l_tszAdapPath, _T("sc_adap.exe"))) {
		g_lXmpType = 1;		/* 1ï¿½FSC_adap.exe[scout]	*/
	}
	else {
		g_lXmpType = 0;		/* 0ï¿½Fjaw_adap.exe[WVASE]	*/
	}
	/* added 2014.05.22 hmenjo get Xmp Type ---------- } ---------- */

		///// Alarm History Log Dlg /////
	g_pAlarmLogDlg = new CAlarmLogDlg();
	g_pAlarmLogDlg->Create(CAlarmLogDlg::IDD);

	///// Interlock Dlg /////
	g_pInterlockDlg = new CInterlockDlg();
	g_pInterlockDlg->Create(CInterlockDlg::IDD);

	///// Debug Dlg /////
	m_pDebugDlg = new CDebugDlg;
	m_pDebugDlg->Create(CDebugDlg::IDD, NULL);

	// ï¿½ï¿½ï¿½Cï¿½ï¿½ ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½Ì‚Å�Aï¿½\ï¿½ï¿½ï¿½Æ�Xï¿½Vï¿½ï¿½ï¿½sï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//m_pMainWnd->ShowWindow(m_nCmdShow);
//	  m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->ShowWindow(SW_MINIMIZE);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O                                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½O ï¿½fï¿½[ï¿½^
		//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Operation

public:
	CShadeButtonST m_OkButton;

	// ClassWizard ï¿½ï¿½ï¿½zï¿½Ö�ï¿½ï¿½ÌƒIï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½hï¿½ğ�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ï¿½ÌƒTï¿½|ï¿½[ï¿½g
	//}}AFX_VIRTUAL

// ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
//
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDOK, m_OkButton);
}

/////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ï¿½_ï¿½Cï¿½Aï¿½ï¿½ï¿½Oï¿½ï¿½ï¿½ï¿½ï¿½sï¿½ï¿½ï¿½é‚½ï¿½ß‚ÌƒAï¿½vï¿½ï¿½ï¿½Pï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½ ï¿½Rï¿½}ï¿½ï¿½ï¿½h

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecApp ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½                                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///// OK Button /////
	m_OkButton.SetShade(BUTTON_NORMAL_SHADEID, BUTTON_GRANULARITY, BUTTON_HIGHLIGHT, BUTTON_COLORING, BUTTON_NORMAL_COLOR);
	m_OkButton.DrawFlatFocus(TRUE);

	char szBuff[100];

	/* modified 2016.05.12 hmenjo 6500/TS3100 ï¿½ï¿½ version ---------- { ---------- */
	//	sprintf(szBuff, "%s (%s %s)", SOFT_VERSION, __DATE__, __TIME__);
	/* modified 2016.05.12 hmenjo 6500/TS3100 ï¿½ï¿½ version ----------              */
	char l_szVersion[128];
	switch (g_lModelType) {
	case MODEL_T3100:	strcpy(l_szVersion, SOFT_VERSION_TS3100);	break;
	default:			strcpy(l_szVersion, SOFT_VERSION);			break;
	}
	sprintf(szBuff, "%s (%s %s)", l_szVersion, __DATE__, __TIME__);
	/* modified 2016.05.12 hmenjo 6500/TS3100 ï¿½ï¿½ version ---------- } ---------- */
	SetDlgItemText(IDC_VERSION, szBuff);

	return TRUE;	// ï¿½Rï¿½ï¿½ï¿½gï¿½ï¿½ï¿½[ï¿½ï¿½ï¿½Éƒtï¿½Hï¿½[ï¿½Jï¿½Xï¿½ï¿½İ’è‚µï¿½È‚ï¿½ï¿½Æ‚ï¿½ï¿½Aï¿½ß‚ï¿½lï¿½ï¿½ TRUE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
	// ï¿½ï¿½O: OCX ï¿½vï¿½ï¿½ï¿½pï¿½eï¿½B ï¿½yï¿½[ï¿½Wï¿½Ì–ß‚ï¿½lï¿½ï¿½ FALSE ï¿½Æ‚È‚ï¿½Ü‚ï¿½
}

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecApp ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½nï¿½ï¿½ï¿½hï¿½ï¿½                                        //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
BOOL CNanoSpecApp::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message) {
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_ESCAPE) {
			int i = 0;
		}
		break;
	}
	return CWinApp::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecApp::OnDebug()
{
	m_pDebugDlg->ShowWindow(SW_HIDE);
	m_pDebugDlg->ShowWindow(SW_SHOWNORMAL);
}

/////////////////////////////////////////////////////////////////////////////
//
int CNanoSpecApp::ExitInstance()
{
	/* ï¿½ï¿½dï¿½Nï¿½ï¿½ï¿½Ì–hï¿½~ï¿½Ì‚ï¿½ï¿½ß‚ï¿½ Mutex ï¿½Jï¿½ï¿½	*/
	if (0 != m_hMutex_Run) {
		::ReleaseMutex(m_hMutex_Run);
		::CloseHandle(m_hMutex_Run);
	}


	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecApp::NextraExe(int Cmd)
{
	HWND hWnd;
	BOOL bRet;
	char szFilePath[MAX_PATH];

	hWnd = ::FindWindow(NULL, NEXTRA_WINDOW_NAME);

	switch (Cmd) {
	case NEXTRA_EXE_START:
		if (hWnd == NULL) {
			STARTUPINFO StartInfo;
			PROCESS_INFORMATION ProcessInfo;
			ZeroMemory(&StartInfo, sizeof(StartInfo));
			ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
			StartInfo.wShowWindow = SW_SHOWNORMAL;
			StartInfo.cb = sizeof(StartInfo);
			sprintf(szFilePath, "%s%s%s", g_szBin_Dir, NEXTRA_EXE_NAME, " /H");
			int ret = CreateProcess(NULL,
				szFilePath, 					// BIN_DIR "\\" NEXTRA_EXE_NAME " /H",
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				".",
				&StartInfo,
				&ProcessInfo
			);
		}
		break;
	case NEXTRA_EXE_SHOW:
		if (hWnd != NULL) {
			bRet = ::ShowWindow(hWnd, SW_SHOWNORMAL);
		}
		break;
	case NEXTRA_EXE_HIDE:
		if (hWnd != NULL) {
			bRet = ::ShowWindow(hWnd, SW_HIDE);
		}
		break;
	case NEXTRA_EXE_QUIT:
		if (hWnd != NULL) {
			// 2009.12.29 K.Matsuo Nextra.exe ï¿½Iï¿½ï¿½ï¿½Ò‚ï¿½ï¿½Î�ï¿½ -->
			bRet = ::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			if (0 != bRet) {
				DWORD l_dwStartTime = ::GetTickCount();
				DWORD l_dwEndTime, l_dwElpsTime;
				while (1) {
					hWnd = ::FindWindow(NULL, NEXTRA_WINDOW_NAME);
					if (hWnd == NULL) {
						// ï¿½ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½
						break;
					}
					l_dwEndTime = ::GetTickCount();
					if (l_dwEndTime < l_dwStartTime) {
						l_dwElpsTime = l_dwEndTime + (ULONG_MAX - l_dwStartTime) + 1;
					}
					else {
						l_dwElpsTime = l_dwEndTime - l_dwStartTime;
					}
					if (30000 <= l_dwElpsTime) {
						// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
						break;
					}
					::Sleep(100);
				}
			}
			// 2009.12.29 K.Matsuo Nextra.exe ï¿½Iï¿½ï¿½ï¿½Ò‚ï¿½ï¿½Î�ï¿½ <--
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CNanoSpecApp::NanoSpecTitle(int Cmd, int iParam/*=0*/)
{
	HWND hWnd;
	BOOL bRet;
	char szFilePath[MAX_PATH];

	hWnd = ::FindWindow(NULL, NANOSPEC_TITLE_WINDOW_NAME);

	switch (Cmd) {
	case NANOSPEC_TITLE_START:
		if (hWnd == NULL) {
			/* added 2014.11.22 hmenjo StartUpLogo ï¿½Nï¿½ï¿½/ï¿½ï¿½~ ---------- { ---------- */
			this->StartUpLogo(1);	/* ï¿½Nï¿½ï¿½	*/
			/* added 2014.11.22 hmenjo StartUpLogo ï¿½Nï¿½ï¿½/ï¿½ï¿½~ ---------- } ---------- */
			STARTUPINFO StartInfo;
			PROCESS_INFORMATION ProcessInfo;
			ZeroMemory(&StartInfo, sizeof(StartInfo));
			ZeroMemory(&ProcessInfo, sizeof(ProcessInfo));
			StartInfo.wShowWindow = SW_SHOWNORMAL;
			StartInfo.cb = sizeof(StartInfo);
			// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) -->
			//			sprintf(szFilePath, "%s%s", g_szBin_Dir, NANOSPEC_TITLE_EXE_NAME);
			CString strBuffer;
			strBuffer = NANOSPEC_TITLE_EXE_NAME;
			if (g_lAppNameType != APP_NAME_NANO) {
				strBuffer.Replace(g_lpszAppPrefix4[APP_NAME_NANO], g_lpszAppPrefix4[g_lAppNameType]);
			}
			sprintf(szFilePath, "%s%s", g_szBin_Dir, (LPCTSTR)strBuffer);
			// 2013.11.07 Bagus Mod (TohoSpecï¿½Î‰ï¿½) <--
			int ret = CreateProcess(NULL,
				szFilePath, 					// BIN_DIR "\\" NANOSPEC_TITLE_EXE_NAME,
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				".",
				&StartInfo,
				&ProcessInfo
			);
			/* added 2009.06.05 hmenjo NanoSpecTitle ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Î�ï¿½ ---------- { ---------- */
			m_dwProcIdNanoTitle = ProcessInfo.dwProcessId;
			/* deleted 2009.06.15 hmenjo ï¿½^ï¿½Cï¿½gï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌƒSï¿½~ï¿½í�œ ---------- { ---------- */
			//			ProcessInfo.hProcess;
			/* deleted 2009.06.15 hmenjo ï¿½^ï¿½Cï¿½gï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌƒSï¿½~ï¿½í�œ ---------- } ---------- */
			if (0 != ret) {
				/* ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ÍƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Â‚Ì‚ï¿½
					ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‚ï¿½ï¿½C 30[s]ï¿½Ô‘Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
				DWORD l_dwStartTime = ::GetTickCount();
				DWORD l_dwEndTime, l_dwElpsTime;
				while (0 == ::FindWindow(NULL, NANOSPEC_TITLE_WINDOW_NAME)) {
					/* deleted 2009.06.15 hmenjo ï¿½^ï¿½Cï¿½gï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌƒSï¿½~ï¿½í�œ ---------- { ---------- */
					//					::Sleep(200);
					/* deleted 2009.06.15 hmenjo ï¿½^ï¿½Cï¿½gï¿½ï¿½ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½\ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÌƒSï¿½~ï¿½í�œ ---------- } ---------- */
					l_dwEndTime = ::GetTickCount();
					if (l_dwEndTime < l_dwStartTime) {
						l_dwElpsTime = l_dwEndTime + (ULONG_MAX - l_dwStartTime) + 1;
					}
					else {
						l_dwElpsTime = l_dwEndTime - l_dwStartTime;
					}
					if (30000 <= l_dwElpsTime) {
						// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
						break;
					}
				}
				::Sleep(200);
			}
			/* added 2009.06.05 hmenjo NanoSpecTitle ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Î�ï¿½ ---------- } ---------- */
		}
		break;
	case NANOSPEC_TITLE_SHOW:
		if (hWnd != NULL) {
			bRet = ::ShowWindow(hWnd, SW_SHOWNORMAL);
		}
		else {
			NanoSpecTitle(NANOSPEC_TITLE_START);
		}
		break;
	case NANOSPEC_TITLE_HIDE:
		if (hWnd != NULL) {
			bRet = ::ShowWindow(hWnd, SW_HIDE);
		}
		break;
		// 2009.09.24 K.Matsuo -->
	case NANOSPEC_TITLE_PROGRESS:
		if (hWnd != NULL) {
			bRet = ::PostMessage(hWnd, WM_SET_PROGRESS, iParam, 0L);
		}
		break;
		// 2009.09.24 K.Matsuo <--
		// 2009.12.05 K.Matsuo -->
	case NANOSPEC_TITLE_MESSAGE2:
		if (hWnd != NULL) {
			bRet = ::PostMessage(hWnd, WM_SET_MESSAGE2, iParam, 0L);
		}
		break;
		// 2009.12.05 K.Matsuo <--
	case NANOSPEC_TITLE_CLOSE:
		if (hWnd != NULL) {
			/* added 2014.11.22 hmenjo StartUpLogo ï¿½Nï¿½ï¿½/ï¿½ï¿½~ ---------- { ---------- */
			this->StartUpLogo(2);	/* ï¿½ï¿½~	*/
			/* added 2014.11.22 hmenjo StartUpLogo ï¿½Nï¿½ï¿½/ï¿½ï¿½~ ---------- } ---------- */
			bRet = ::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
			/* added 2009.06.05 hmenjo NanoSpecTitle ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Î�ï¿½ ---------- { ---------- */
			if (0 != bRet) {
				/* ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ÍƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Â‚Ì‚ï¿½
					ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½é‚©ï¿½C 30[s]ï¿½Ô‘Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
				DWORD l_dwStartTime = ::GetTickCount();
				DWORD l_dwEndTime, l_dwElpsTime;
				while (0 != ::GetProcessVersion(m_dwProcIdNanoTitle)) {
					l_dwEndTime = ::GetTickCount();
					if (l_dwEndTime < l_dwStartTime) {
						l_dwElpsTime = l_dwEndTime + (ULONG_MAX - l_dwStartTime) + 1;
					}
					else {
						l_dwElpsTime = l_dwEndTime - l_dwStartTime;
					}
					if (30000 <= l_dwElpsTime) {
						// ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D
						break;
					}
					/* added 2016.08.26 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½}ï¿½ï¿½ï¿½Î�ï¿½ ---------- { ---------- */
										/*	CNanoSpecTitleDlg ï¿½ï¿½ï¿½ï¿½ SetForegroundWindow() ï¿½ï¿½
											SetWindowPos() ï¿½É‚ï¿½ï¿½ OS ï¿½ï¿½ï¿½ï¿½ÌƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½
											ï¿½~ï¿½Ü‚ï¿½È‚ï¿½ï¿½æ‚¤ï¿½É‚ï¿½ï¿½é‚½ï¿½ß�D	*/
					MSG l_msg;
					while (FALSE != ::PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE)) {
						::TranslateMessage(&l_msg);
						::DispatchMessage(&l_msg);
					}
					/* added 2016.08.26 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½}ï¿½ï¿½ï¿½Î�ï¿½ ---------- } ---------- */
				}
			}
			/* added 2009.06.05 hmenjo NanoSpecTitle ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½ï¿½Î�ï¿½ ---------- } ---------- */
		}
		break;
	}
}

// ==========================================================================
//
void CNanoSpecApp::MakePath()
{
	char* ptr;
	const char* cptr;
	memset(g_szCurPath, 0, sizeof(g_szCurPath));
	memset(g_szMyPath, 0, sizeof(g_szMyPath));
	DWORD dRet = GetCurrentDirectory(MAX_PATH, g_szCurPath);
	strcpy(g_szMyPath, g_szCurPath);
	//ptr = strrchr(g_szMyPath, '\\');
	//if (ptr) *ptr = '\0';

	///// BIN_DIR /////
	strcpy(g_szBin_Dir, g_szMyPath);
	cptr = strchr(BIN_DIR, '\\');
	if (cptr) strcat(g_szBin_Dir, cptr);

	///// LOG_DIR /////
	strcpy(g_szLog_Dir, g_szMyPath);
	cptr = strchr(LOG_DIR, '\\');
	if (cptr) strcat(g_szLog_Dir, cptr);

	///// PR_DIR /////
	strcpy(g_szLog_Pr_Dir, g_szLog_Dir);
	strcat(g_szLog_Pr_Dir, "PR\\");

	///// CFG_DIR /////
	strcpy(g_szCfg_Dir, g_szMyPath);
	cptr = strchr(CFG_DIR, '\\');
	if (cptr) strcat(g_szCfg_Dir, cptr);

	///// CFG_SYSTEM_DIR /////
	strcpy(g_szCfg_System_Dir, g_szCfg_Dir);
	strcat(g_szCfg_System_Dir, "SYSTEM\\");

	///// CFG_SYSTEM_SAMPLE_DIR /////
	strcpy(g_szCfg_System_Sample_Dir, g_szCfg_System_Dir);
	strcat(g_szCfg_System_Sample_Dir, "SAMPLE\\");

	///// CFG_SYSTEM_SR_REFERENCE_MATERIAL /////
	strcpy(g_szCfg_System_Sr_Reference_Material_Dir, g_szCfg_System_Dir);
	strcat(g_szCfg_System_Sr_Reference_Material_Dir, "SR_ReferenceMaterial\\");

	///// CFG_USER_DIR /////
	strcpy(g_szCfg_User_Dir, g_szCfg_Dir);
	strcat(g_szCfg_User_Dir, "USER\\");

	///// CFG_USER_USER_ACCOUNT_DIR /////
	strcpy(g_szCfg_User_User_Account_Dir, g_szCfg_User_Dir);
	strcat(g_szCfg_User_User_Account_Dir, "USERACCOUNT\\");

	///// DB_DIR /////
	strcpy(g_szDb_Dir, g_szMyPath);
	cptr = strchr(DB_DIR, '\\');
	if (cptr) strcat(g_szDb_Dir, cptr);

	///// DB_MAIN_RECIPE_DIR /////
	strcpy(g_szDb_Main_Recipe_Dir, g_szDb_Dir);
	strcat(g_szDb_Main_Recipe_Dir, "MAINRECIPE\\");

	///// DB_MULTI_RECIPE_DIR /////
	strcpy(g_szDb_Multi_Recipe_Dir, g_szDb_Dir);
	strcat(g_szDb_Multi_Recipe_Dir, "MULTIRECIPE\\");

	///// DB_MEASUREMENT_PROGRAM_DIR /////
	strcpy(g_szDb_Measurement_Program_Dir, g_szDb_Dir);
	strcat(g_szDb_Measurement_Program_Dir, "MEASUREMENTPGM\\");

	///// DB_STAGE_PROGRAM_DIR /////
	strcpy(g_szDb_Stage_Program_Dir, g_szDb_Dir);
	strcat(g_szDb_Stage_Program_Dir, "STAGEPGM\\");

	///// DB_DESKEW_IMG_DIR /////
	strcpy(g_szDb_Deskew_Img_Dir, g_szDb_Dir);
	strcat(g_szDb_Deskew_Img_Dir, "DESKEW_IMG\\");

	///// DB_PATTERN_DIR /////
	strcpy(g_szDb_Pattern_Dir, g_szDb_Dir);
	strcat(g_szDb_Pattern_Dir, "PATTERN\\");

	///// DB_PATTERN_IMG_DIR /////
	strcpy(g_szDb_Pattern_Img_Dir, g_szDb_Dir);
	strcat(g_szDb_Pattern_Img_Dir, "PATTERN_IMG\\");

	///// DB_RECALIBRATION_PROGRAM_DIR /////
	strcpy(g_szDb_Recalibration_Program_Dir, g_szDb_Dir);
	strcat(g_szDb_Recalibration_Program_Dir, "RECALIBRATIONPGM\\");

	///// DB_POINT_DESKEW_PROGRAM_DIR /////
	strcpy(g_szDb_Point_Deskew_Program_Dir, g_szDb_Dir);
	strcat(g_szDb_Point_Deskew_Program_Dir, "POINTDESKEWPGM\\");

	///// RESULT_DIR /////
	strcpy(g_szResult_Dir, g_szMyPath);
	cptr = strchr(RESULT_DIR, '\\');
	if (cptr) strcat(g_szResult_Dir, cptr);

	///// DATA_DIR /////
	strcpy(g_szData_Dir, g_szMyPath);
	cptr = strchr(DATA_DIR, '\\');
	if (cptr) strcat(g_szData_Dir, cptr);

	///// DATA_MEASUREMENTDATA_DIR /////
	strcpy(g_szData_MeasurementData_Dir, g_szData_Dir);
	strcat(g_szData_MeasurementData_Dir, "MEASUREMENTDATA\\");

	///// MEAS_DAT_TEMP_COPY_PASS /////
	strcpy(g_szData_Meas_Dat_Temp_Copy_Dir, g_szData_Dir);
	strcat(g_szData_Meas_Dat_Temp_Copy_Dir, "MeasuredDataTemp\\");

	///// MEASURED_DATA_TEMP_DIR /////
	strcpy(g_szData_Measured_Data_Temp_Dir, g_szData_Dir);
	strcat(g_szData_Measured_Data_Temp_Dir, "MeasuredDataTemp\\");

	///// REF_DIR /////
	strcpy(g_szData_Ref_Dir, g_szData_Dir);
	strcat(g_szData_Ref_Dir, "ref\\");
	/* added 2009.07.31 hmenjo ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½@ï¿½\ï¿½Ç‰ï¿½(9) ---------- { ---------- */
	_tcscpy(g_szData_StressRef_Dir, g_szData_Ref_Dir);
	_tcscat(g_szData_StressRef_Dir, _T("StressRef\\"));
	/* added 2009.07.31 hmenjo ï¿½Xï¿½gï¿½ï¿½ï¿½Xï¿½@ï¿½\ï¿½Ç‰ï¿½(9) ---------- } ---------- */
}

/* added 2009.07.13 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ---------- { ---------- */
/*	NanoSpec ï¿½pï¿½Ì�ï¿½ï¿½sï¿½pï¿½Xï¿½ï¿½İ’è‚µï¿½Ü‚ï¿½ï¿½D	*/
int CNanoSpecApp::SetEnvNanoPath()
{
	/* ï¿½î�€ï¿½fï¿½Bï¿½ï¿½ï¿½Nï¿½gï¿½ï¿½ï¿½ğ“±�o	*/
	TCHAR l_tszTemp[_MAX_PATH];
	if (0 == ::GetModuleFileName(0, l_tszTemp, sizeof(l_tszTemp))) {
		return -1;
	}
	TCHAR l_tszBaseDrv[_MAX_DRIVE];
	TCHAR l_tszBaseDir[_MAX_DIR];
	_tsplitpath(l_tszTemp, l_tszBaseDrv, l_tszBaseDir, 0, 0);
	_stprintf(l_tszTemp, _T("%s%s.."), l_tszBaseDrv, l_tszBaseDir);
	TCHAR l_tszBasePath[_MAX_PATH];
	if (0 == _tfullpath(l_tszBasePath, l_tszTemp, sizeof(l_tszBasePath))) {
		return -2;
	}
	if (_T('\\') != l_tszBasePath[_tcslen(l_tszBasePath) - 1]) {
		_tcscat(l_tszBasePath, _T("\\"));	/* ï¿½ÅŒï¿½ï¿½'\'ï¿½ï¿½Ç‰ï¿½	*/
	}
	/* ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½ï¿½ï¿½ì�¬ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	TCHAR l_tszPathBin[_MAX_PATH];								/* BIN		*/
	_stprintf(l_tszPathBin, _T("%sBIN"), l_tszBasePath);
	TCHAR l_tszPathBinDeb[_MAX_PATH];							/* BIN_DEB	*/
	_stprintf(l_tszPathBinDeb, _T("%sBIN_DEB"), l_tszBasePath);
	TCHAR l_tszPathDll[_MAX_PATH];								/* DLL		*/
	_stprintf(l_tszPathDll, _T("%sDLL"), l_tszBasePath);
	/* ï¿½Â‹ï¿½ï¿½Ï�ï¿½ï¿½Ì�ï¿½ï¿½sï¿½pï¿½Xï¿½Ì•ï¿½ï¿½ï¿½ï¿½ï¿½Tï¿½Cï¿½Yï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	DWORD l_dwEnvPathReqLen = ::GetEnvironmentVariable(_T("path"), 0, 0);
	if (0 == l_dwEnvPathReqLen) {
		return -3;
	}
	/* ï¿½Â‹ï¿½ï¿½Ï�ï¿½ï¿½Ì�ï¿½ï¿½sï¿½pï¿½Xï¿½æ“¾ï¿½pï¿½Ì—Ìˆï¿½ï¿½ï¿½mï¿½Û‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	int l_iAddLen = _tcslen(l_tszPathBin) + _tcslen(l_tszPathBinDeb) + _tcslen(l_tszPathDll) + (sizeof(TCHAR) * 3);
	l_dwEnvPathReqLen += l_iAddLen + (sizeof(TCHAR) * 4);
	TCHAR* l_ptszEnvPath = (TCHAR*)malloc(l_dwEnvPathReqLen);
	if (0 == l_ptszEnvPath) {
		return -4;
	}
	/* ï¿½Â‹ï¿½ï¿½Ï�ï¿½ï¿½Ì�ï¿½ï¿½sï¿½pï¿½Xï¿½ï¿½ï¿½æ“¾ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	memset(l_ptszEnvPath, 0, l_dwEnvPathReqLen);
	DWORD l_dwEnvPathLen = ::GetEnvironmentVariable(_T("path"), l_ptszEnvPath, l_dwEnvPathReqLen);
	_tcslwr(l_ptszEnvPath);
	/* Nano ï¿½Ì�ï¿½ï¿½sï¿½pï¿½Xï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î’Ç‰ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
	TCHAR l_tszAddPath[_MAX_PATH] = _T("");
	TCHAR* l_ptszFind;
	_tcslwr(l_tszPathBin);
	l_ptszFind = _tcsstr(l_ptszEnvPath, l_tszPathBin);
	if ((0 == l_ptszFind)
		|| ((0 != l_ptszFind)
			&& (_T(';') != *(l_ptszFind + _tcslen(l_tszPathBin)))
			&& (0 != *(l_ptszFind + _tcslen(l_tszPathBin))))) {
		_tcscat(l_tszAddPath, _T(";")); _tcscat(l_tszAddPath, l_tszPathBin);
	}
	_tcslwr(l_tszPathBinDeb);
	l_ptszFind = _tcsstr(l_ptszEnvPath, l_tszPathBinDeb);
	if ((0 == l_ptszFind)
		|| ((0 != l_ptszFind)
			&& (_T(';') != *(l_ptszFind + _tcslen(l_tszPathBinDeb)))
			&& (0 != *(l_ptszFind + _tcslen(l_tszPathBinDeb))))) {
		_tcscat(l_tszAddPath, _T(";")); _tcscat(l_tszAddPath, l_tszPathBinDeb);
	}
	_tcslwr(l_tszPathDll);
	l_ptszFind = _tcsstr(l_ptszEnvPath, l_tszPathDll);
	if ((0 == l_ptszFind)
		|| ((0 != l_ptszFind)
			&& (_T(';') != *(l_ptszFind + _tcslen(l_tszPathDll)))
			&& (0 != *(l_ptszFind + _tcslen(l_tszPathDll))))) {
		_tcscat(l_tszAddPath, _T(";")); _tcscat(l_tszAddPath, l_tszPathDll);
	}
	if (0 == _tcscmp(l_tszAddPath, _T(""))) {
		/* ï¿½ï¿½ï¿½×‚Ä‘ï¿½ï¿½İ‚ï¿½ï¿½ï¿½ï¿½Ì‚Å’Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½ï¿½D	*/
	}
	else {
		/* ï¿½Ç‰ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½Ì‚ÅŠÂ‹ï¿½ï¿½Ï�ï¿½ï¿½Ì�ï¿½ï¿½sï¿½pï¿½Xï¿½É’Ç‰ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
		_tcscat(l_ptszEnvPath, l_tszAddPath);
		::SetEnvironmentVariable(_T("path"), l_ptszEnvPath);
	}

	free(l_ptszEnvPath);

	return 0;
}
/* added 2009.07.13 hmenjo ï¿½ï¿½ï¿½sï¿½pï¿½Xï¿½İ’ï¿½ ---------- } ---------- */

/* added 2014.11.22 hmenjo StartUpLogo ï¿½Nï¿½ï¿½/ï¿½ï¿½~ ---------- { ---------- */
#define	SUL_CAPTION	_T("StartUpLogo")
long CNanoSpecApp::StartUpLogo(long lCmd/* = 0*/)
{
	long l_lRc = 0;
	TCHAR l_tszCaption[256] = SUL_CAPTION;
	HWND l_hWnd = ::FindWindow(NULL, l_tszCaption);
	static DWORD ls_dwProcessId = 0;

	long l_lCmd = lCmd;

	switch (l_lCmd) {
	case 1:		/* ï¿½Jï¿½n(CreateProcess)	*/
		if (l_hWnd == NULL) {
			STARTUPINFO l_StartInfo;
			PROCESS_INFORMATION l_ProcessInfo;
			memset(&l_StartInfo, 0, sizeof(l_StartInfo));
			memset(&l_ProcessInfo, 0, sizeof(l_ProcessInfo));
			l_StartInfo.cb = sizeof(l_StartInfo);
			l_StartInfo.wShowWindow = SW_SHOWNORMAL;
			TCHAR l_tszPathExe[MAX_PATH];
			_stprintf(l_tszPathExe, _T("%s%s.exe"), g_szBin_Dir, l_tszCaption);
			//			TCHAR l_tszParam[] = _T("ANIMATE1 WZOTOPMOST DISPTIME0 RECT-1,550,-1,-1");	/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½@ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^	*/
			/* modified 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- { ---------- */
			//			TCHAR l_tszParam[] = _T("ANIMATE1 WZOTOPMOST2 DISPTIME0 RECT-1,550,-1,-1");	/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½@ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^	*/
			/* modified 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ----------              */
			TCHAR l_tszParamFmt[] = _T("ANIMATE1 WZOTOPMOST2 DISPTIME0 RECT-1,%d,-1,-1");	/* ï¿½Rï¿½}ï¿½ï¿½ï¿½hï¿½ï¿½ï¿½Cï¿½ï¿½ï¿½@ï¿½pï¿½ï¿½ï¿½ï¿½ï¿½^	*/
			TCHAR l_tszParam[256] = _T("");
			RECT l_rectDesktop;
			CWnd::GetDesktopWindow()->GetClientRect(&l_rectDesktop);
			long l_lHPos = 550;
			if (l_rectDesktop.bottom < 1024) {
				l_lHPos = 370;
			}
			_stprintf(l_tszParam, l_tszParamFmt, l_lHPos);
			/* modified 2015.04.05 hmenjo FWXGA ï¿½Î‰ï¿½2 ---------- } ---------- */
			if (0 < _tcslen(l_tszParam)) {
				_tcscat(l_tszPathExe, _T(" "));
				_tcscat(l_tszPathExe, l_tszParam);
			}
			BOOL l_bRc = ::CreateProcess(NULL,
				l_tszPathExe,
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				_T("."),
				&l_StartInfo,
				&l_ProcessInfo
			);
			ls_dwProcessId = 0;
			if (FALSE != l_bRc) {
				ls_dwProcessId = l_ProcessInfo.dwProcessId;
				/* ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ÍƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Â‚Ì‚ï¿½
					ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚Å‚ï¿½ï¿½C 30[s]ï¿½Ô‘Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
				DWORD l_dwStartTime = ::GetTickCount();
				while (0 == ::FindWindow(NULL, l_tszCaption)) {
					if (30000 <= (::GetTickCount() - l_dwStartTime)) {
						/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
						l_lRc = -11;
						break;
					}
				}
				::Sleep(200);
			}
			else {
				/* CreateProcess() ï¿½Gï¿½ï¿½ï¿½[	*/
				l_lRc = -12;
			}
		}
		break;
	case 2:		/* ï¿½Iï¿½ï¿½(WM_CLOSE)	*/
		if ((l_hWnd != NULL) && (0 != ls_dwProcessId)) {
			BOOL l_bRc = ::PostMessage(l_hWnd, WM_CLOSE, NULL, NULL);
			if (FALSE != l_bRc) {
				/* ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½Zï¿½Xï¿½ÍƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½Â‚Ì‚ï¿½
					ï¿½Eï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä‚ï¿½ï¿½È‚ï¿½ï¿½ï¿½ï¿½Æ‚ï¿½ï¿½mï¿½Fï¿½ï¿½ï¿½é‚©ï¿½C 30[s]ï¿½Ô‘Ò‚ï¿½ï¿½Ü‚ï¿½ï¿½D*/
				DWORD l_dwStartTime = ::GetTickCount();
				DWORD l_dwPV = 0;
				while (0 != (l_dwPV = ::GetProcessVersion(ls_dwProcessId))) {
					if (30000 <= (::GetTickCount() - l_dwStartTime)) {
						/* ï¿½^ï¿½Cï¿½ï¿½ï¿½Aï¿½Eï¿½gï¿½É‚ï¿½ï¿½Ü‚ï¿½ï¿½D	*/
						l_lRc = -21;
						/* added 2016.09.02 hmenjo StartUpLogo ï¿½ï¿½ï¿½ï¿½ ---------- { ---------- */
						::PostMessage(l_hWnd, WM_CLOSE, NULL, NULL);
						/* added 2016.09.02 hmenjo StartUpLogo ï¿½ï¿½ï¿½ï¿½ ---------- } ---------- */
						break;
					}
					/* added 2016.08.26 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½}ï¿½ï¿½ï¿½Î�ï¿½ ---------- { ---------- */
										/*	CNanoSpecTitleDlg ï¿½ï¿½ï¿½ï¿½ SetForegroundWindow() ï¿½ï¿½
											SetWindowPos() ï¿½É‚ï¿½ï¿½ OS ï¿½ï¿½ï¿½ï¿½ÌƒEï¿½Bï¿½ï¿½ï¿½hï¿½Eï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½Wï¿½ï¿½
											ï¿½~ï¿½Ü‚ï¿½È‚ï¿½ï¿½æ‚¤ï¿½É‚ï¿½ï¿½é‚½ï¿½ß�D	*/
					MSG l_msg;
					while (FALSE != ::PeekMessage(&l_msg, NULL, 0, 0, PM_REMOVE)) {
						::TranslateMessage(&l_msg);
						::DispatchMessage(&l_msg);
					}
					/* added 2016.08.26 hmenjo ï¿½Nï¿½ï¿½ï¿½ï¿½ï¿½_ï¿½ï¿½ï¿½}ï¿½ï¿½ï¿½Î�ï¿½ ---------- } ---------- */
				}
				if (0 == l_dwPV) {
					ls_dwProcessId = 0;
				}
			}
			else {
				/* PostMessage() ï¿½Gï¿½ï¿½ï¿½[	*/
				l_lRc = -22;
			}
		}
		break;
	default:
		/* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È‚ï¿½	*/
		break;
	}

	return l_lRc;
}
/* added 2014.11.22 hmenjo StartUpLogo ï¿½Nï¿½ï¿½/ï¿½ï¿½~ ---------- } ---------- */
