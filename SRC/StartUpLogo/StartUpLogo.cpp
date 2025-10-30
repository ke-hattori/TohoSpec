// StartUpLogo.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <tchar.h>
#include <stdio.h>

/*
 *	マクロ
 */
#define	DISPTIME_DEFAULT	(2000)
#define	BMP_NG_WIDTH	(200)
#define	BMP_NG_HEIGHT	(100)
#define	TMR_INTVL_ID	(101)
#define	TMR_INTVL_TIME	(100)
#define	TMR_INTVL_ID2	(102)
#define	TMR_INTVL_TIME2	(500)

/*
 *	関数プロトタイプ
 */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM SetWindowClass(HINSTANCE);
BOOL CreateWin(HINSTANCE, int);
BOOL InitInstance(LPTSTR lpCmdLine);
void ExitInstance(void);
void DrawBitmap(HDC, HBITMAP, LPBITMAP);
void LoadBmp(LPCTSTR);

/*
 *	AnimateWindow()
 */
extern "C" BOOL WINAPI AnimateWindow(HWND hWnd, DWORD dwTime, DWORD dwFlags);
#define AW_HOR_POSITIVE		0x00000001
#define AW_HOR_NEGATIVE		0x00000002
#define AW_VER_POSITIVE		0x00000004
#define AW_VER_NEGATIVE		0x00000008
#define AW_CENTER			0x00000010
#define AW_HIDE				0x00010000
#define AW_ACTIVATE			0x00020000
#define AW_SLIDE			0x00040000
#define AW_BLEND			0x00080000

/*
 *	構造体
 */
typedef struct {			/* コマンドラインパラメタ	*/
	long	lWzo;				/* ウィンドウ Z オーダ	*/
	RECT	rectPos;			/* 表示位置＆サイズ	*/
	long	lAnimate;			/* アニメ	*/
	DWORD	dwDispTime;			/* 表示時間	*/
} CMDLINE, *LPCMDLINE;
typedef struct {			/* 設定(ini)ファイルパラメタ	*/
	struct {					/* [WINDOW]	*/
		long	lZorder;			/* ZORDER	*/
		HWND	hWnd;				/* HWMD	*/
		RECT	rectPos;			/* left,top,right,bottom	*/
		DWORD	dwDispTime;			/* DISPTIME	*/
	} Window;
	struct {						/* [RESOURCE]	*/
		TCHAR	tszPathIco[MAX_PATH];	/* ICON アイコンのパス(相対パス指定可能)	*/
		TCHAR	tszPathBmp[MAX_PATH];	/* BMP ビットマップのパス(相対パス指定可能) */
	} Resource;
	struct {					/* [ANIMATE]	*/
		long	lNum;				/* NUM 処理数(最大99)	*/
		struct {
			long	lMode;			/* MODEnn アニメ種類	*/
			DWORD	dwTime;			/* TIMEnn 時間[ms]，あるいは，フレーム数	*/
		} MdTm[100];
	} Animate;
} INIPARAMS, *LPINIPARAMS;


/*
 *	変数
 */
HINSTANCE g_hInstance;
TCHAR g_tszClassName[] = _T("StartUpLogo");		/* ウィンドウクラス，キャプション	*/
HANDLE g_hMutex_Run;	/* 多重起動防止用	*/
TCHAR g_tszPathExe[MAX_PATH];		/* exe のフルパス	*/
TCHAR g_tszPathBase[MAX_PATH];		/* exe のフルパスの拡張子無し	*/
TCHAR g_tszDirExe[MAX_PATH];		/* exe のディレクトリ	*/
TCHAR g_tszPathIni[MAX_PATH];		/* ini フルパス	*/
TCHAR g_tszPathIco[MAX_PATH];		/* アイコンファイルのフルパス	*/
TCHAR g_tszPathBmp[MAX_PATH];		/* ビットマップファイルのフルパス	*/
HBITMAP g_hBmp;
HICON g_hIcon32;
HICON g_hIcon16;
HCURSOR g_hCursor;
BITMAP g_BmpInfo;
CMDLINE g_CmdLine;			/* コマンドラインパラメタ	*/
INIPARAMS g_IniParams;		/* ini ファイルパラメタ	*/
long g_lAnimeCnt;
int g_iPrc;
BOOL g_bWM_CLOSE_No1st;


int APIENTRY _tWinMain(
						HINSTANCE	hInstance,
						HINSTANCE	hPrevInstance,
						LPTSTR		lpCmdLine,
						int			nCmdShow)
{
 	// TODO: この位置にコードを記述してください。

	BOOL l_bRet = FALSE;
	BOOL l_bInitERR = FALSE;
	MSG l_msg;
	int l_iRc = 0;

	g_hInstance = hInstance;

	/* 初期化	*/
	if (FALSE != InitInstance(lpCmdLine)) {
		/* ウィンドウクラスの登録	*/
		if (0 != SetWindowClass(g_hInstance)) {
			/* ウィンドウの生成	*/
			if (FALSE != CreateWin(g_hInstance, nCmdShow)) {
				l_bInitERR = FALSE;
				/* メッセージループ	*/
				while (0 != (l_bRet = ::GetMessage(&l_msg, NULL, 0, 0))) {
					if (l_bRet == -1) {
						break;
					} else {
						::TranslateMessage(&l_msg);
						::DispatchMessage(&l_msg);
					}
				}
			} else {
				l_bInitERR = TRUE;
			}
		} else {
			l_bInitERR = TRUE;
		}
	} else {
		l_bInitERR = TRUE;
	}

	/* 終了処理	*/
	ExitInstance();

	if (FALSE == l_bInitERR) {
		l_iRc = (int) l_msg.wParam;
	} else {
		l_iRc = 0;
	}

	return l_iRc;
}

/*
 *	ウィンドウクラスの登録
 */
ATOM SetWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX l_wc;
	l_wc.cbSize = sizeof(WNDCLASSEX);
	l_wc.style = CS_HREDRAW | CS_VREDRAW;
	l_wc.lpfnWndProc = WndProc;						/* プロシージャ名	*/
	l_wc.cbClsExtra = 0;
	l_wc.cbWndExtra = 0;
	l_wc.hInstance = hInstance;						/* インスタンス	*/
	l_wc.hIcon = (0 != g_hIcon32)? g_hIcon32 : 0;	/* アイコン標準	*/
	l_wc.hCursor = (0 != g_hCursor)? g_hCursor : 0;	/* カーソル	*/
	l_wc.hbrBackground = (HBRUSH) ::GetStockObject(WHITE_BRUSH);
	l_wc.lpszMenuName = 0;							/* メニュー名	*/
	l_wc.lpszClassName = (LPCTSTR) g_tszClassName;	/* ウィンドウクラス名	*/
	l_wc.hIconSm = (0 != g_hIcon16)? g_hIcon16 : 0;	/* アイコン小	*/

    return (::RegisterClassEx(&l_wc));
}

/*
 *	ウィンドウの生成
 */
BOOL CreateWin(HINSTANCE hInstance, int nCmdShow)
{
	HWND l_hWnd;

	l_hWnd = ::CreateWindow(
					g_tszClassName,			/* 登録されているウィンドウクラス名	*/
					g_tszClassName,			/* キャプション	*/
//					WS_OVERLAPPEDWINDOW,	/* ウィンドウの種類	*/
					WS_POPUP,				/* ウィンドウの種類	*/
					g_IniParams.Window.rectPos.left,		/* X 座標	*/
					g_IniParams.Window.rectPos.top,			/* Y 座標	*/
					g_IniParams.Window.rectPos.right - g_IniParams.Window.rectPos.left + 1,	/* 幅	*/
					g_IniParams.Window.rectPos.bottom - g_IniParams.Window.rectPos.top + 1,	/* 高さ	*/
					NULL,					/* 親ウィンドウのハンドル．親を作るときは NULL	*/
					NULL,					/* メニューハンドル．クラスメニューを使うときは NULL	*/
					hInstance,				/* インスタンスハンドル	*/
					NULL					/* ウィンドウ作成データ	*/
				);
	if (0 == l_hWnd) {
        return FALSE;
	}
//	::ShowWindow(l_hWnd, nCmdShow);
	::ShowWindow(l_hWnd, SW_SHOWNORMAL);	/* 通常表示固定	*/
	::UpdateWindow(l_hWnd);

	return TRUE;
}

BOOL InitInstance(LPTSTR lpCmdLine)
{
	/* 二重起動を防止	*/
	g_hMutex_Run = 0;
	g_hMutex_Run = ::CreateMutex(NULL, TRUE, g_tszClassName);
	if ((g_hMutex_Run == NULL) || (::GetLastError() == ERROR_ALREADY_EXISTS)) {
		g_hMutex_Run = 0;
		return	FALSE;
	}

	/* 各種ファイルパスの構築	*/
	if (0 == ::GetModuleFileName(0, g_tszPathExe, sizeof(g_tszPathExe) / sizeof(TCHAR) - 1)) {
		return FALSE;
	}
	_tcscpy(g_tszPathBase, g_tszPathExe);
//	g_tszPathBase[_tcslen(g_tszPathBase) - 4] = 0;
	*(_tcsrchr(g_tszPathBase, _T('.'))) = 0;
	_tcscpy(g_tszDirExe, g_tszPathExe);
	*(_tcsrchr(g_tszDirExe, _T('\\')) + 1) = 0;
	_stprintf(g_tszPathIni, _T("%s.ini"), g_tszPathBase);
	_stprintf(g_tszPathIco, _T("%s.ico"), g_tszPathBase);
	_stprintf(g_tszPathBmp, _T("%s.bmp"), g_tszPathBase);

	/* コマンドパラメタの判定	*/
	if (0 != lpCmdLine) {
		TCHAR* l_ptszTmp = 0;
		TCHAR l_tszTmp[256];
		int i;
		/* ウィンドウ Z オーダ	WZO<param>	*/
		l_ptszTmp = _tcsstr(lpCmdLine, _T("WZO"));
		if (0 == l_ptszTmp) {
			/* WZO 指定無し	*/
			g_CmdLine.lWzo = 0;
		} else {
			l_ptszTmp += _tcslen(_T("WZO"));
			memset(l_tszTmp, 0, sizeof(l_tszTmp));
			for (i = 0; i < ((sizeof(l_tszTmp) / sizeof(TCHAR)) - 1); i++) {
				if ((*l_ptszTmp == _T('\0')) || (*l_ptszTmp == _T(' '))) {
					break;
				}
				l_tszTmp[i] = *l_ptszTmp++;
			}
			if			(0 == _tcscmp(l_tszTmp, _T("TOPMOST"))) {
				/* 最前面	*/
				g_CmdLine.lWzo = 1;
			} else if	(0 == _tcscmp(l_tszTmp, _T("TOPMOST2"))) {
				/* 最前面２	*/
				g_CmdLine.lWzo = 2;
			} else if	(0 == _tcscmp(l_tszTmp, _T("NOZO"))) {
				/* 変更無し	*/
				g_CmdLine.lWzo = 0;
			} else {
				/* ウィンドウハンドル	*/
				for (i = 0; i < ((int) _tcslen(l_tszTmp)); i++) {
					if (0 == _istxdigit(l_tszTmp[i])) {
						break;
					}
				}
				if (((int) _tcslen(l_tszTmp)) <= i) {
					/* 正常	*/
					g_CmdLine.lWzo = _tcstoul(l_tszTmp, 0, 16);
				} else {
					/* 異常(変更無し)	*/
					g_CmdLine.lWzo = 0;
				}
			}
		}
		/* アニメ				ANIMATE<param>	*/
		l_ptszTmp = _tcsstr(lpCmdLine, _T("ANIMATE"));
		if (0 == l_ptszTmp) {
			/* ANIMATE 指定無し	*/
			g_CmdLine.lAnimate = 0;
		} else {
			l_ptszTmp += _tcslen(_T("ANIMATE"));
			memset(l_tszTmp, 0, sizeof(l_tszTmp));
			for (i = 0; i < ((sizeof(l_tszTmp) / sizeof(TCHAR)) - 1); i++) {
				if ((*l_ptszTmp == _T('\0')) || (*l_ptszTmp == _T(' '))) {
					break;
				}
				l_tszTmp[i] = *l_ptszTmp++;
			}
			if (0 == _tcscmp(l_tszTmp, _T("1"))) {
				/* デフォルトアニメ	*/
				g_CmdLine.lAnimate = 1;
			} else {
				/* アニメ無し	*/
				g_CmdLine.lAnimate = 0;
			}
		}
		/* 表示時間				DISPTIME<param>	*/
		l_ptszTmp = _tcsstr(lpCmdLine, _T("DISPTIME"));
		if (0 == l_ptszTmp) {
			/* DISPTIME 指定無し	*/
			g_CmdLine.dwDispTime = DISPTIME_DEFAULT;
		} else {
			l_ptszTmp += _tcslen(_T("DISPTIME"));
			memset(l_tszTmp, 0, sizeof(l_tszTmp));
			for (i = 0; i < ((sizeof(l_tszTmp) / sizeof(TCHAR)) - 1); i++) {
				if ((*l_ptszTmp == _T('\0')) || (*l_ptszTmp == _T(' '))) {
					break;
				}
				l_tszTmp[i] = *l_ptszTmp++;
			}
			for (i = 0; i < ((int) _tcslen(l_tszTmp)); i++) {
				if (0 == _istdigit(l_tszTmp[i])) {
					break;
				}
			}
			if (((int) _tcslen(l_tszTmp)) <= i) {
				/* 正常	*/
				g_CmdLine.dwDispTime = _tcstoul(l_tszTmp, 0, 10);
			} else {
				/* 異常(変更無し)	*/
				g_CmdLine.dwDispTime = DISPTIME_DEFAULT;
			}
		}
		/* 表示位置＆サイズ		RECT<left>,<top>,<right>,<bottom>	*/
		l_ptszTmp = _tcsstr(lpCmdLine, _T("RECT"));
		if (0 == l_ptszTmp) {
			/* RECT 指定無し	*/
			g_CmdLine.rectPos.left = -1;
			g_CmdLine.rectPos.top = -1;
			g_CmdLine.rectPos.right = -1;
			g_CmdLine.rectPos.bottom = -1;
		} else {
			l_ptszTmp += _tcslen(_T("RECT"));
			memset(l_tszTmp, 0, sizeof(l_tszTmp));
			for (i = 0; i < ((sizeof(l_tszTmp) / sizeof(TCHAR)) - 1); i++) {
				if ((*l_ptszTmp == _T('\0')) || (*l_ptszTmp == _T(' '))) {
					break;
				}
				l_tszTmp[i] = *l_ptszTmp++;
			}
			int l_iCommaCnt = 0;
			for (i = 0; i < ((int) _tcslen(l_tszTmp)); i++) {
				if ((0 == _istdigit(l_tszTmp[i])) && (_T(',') != l_tszTmp[i]) && (_T('-') != l_tszTmp[i])) {
					break;
				}
				if (_T(',') == l_tszTmp[i]) {
					l_iCommaCnt++;
				}
			}
			if ((i < ((int) _tcslen(l_tszTmp))) || (3 != l_iCommaCnt)) {
				/* 異常	*/
				g_CmdLine.rectPos.left = -1;
				g_CmdLine.rectPos.top = -1;
				g_CmdLine.rectPos.right = -1;
				g_CmdLine.rectPos.bottom = -1;
			} else {
				/* 正常	*/
				TCHAR *l_ptszTmpS = &(l_tszTmp[0]);
				TCHAR *l_ptszTmpN;

				for (i = 0; i < 4; i++) {
					if (*l_ptszTmpS == _T(',')) {
						switch (i) {
						case 0:		g_CmdLine.rectPos.left = -1;	break;
						case 1:		g_CmdLine.rectPos.top = -1;		break;
						case 2:		g_CmdLine.rectPos.right = -1;	break;
						default:	g_CmdLine.rectPos.bottom = -1;	break;
						}
						l_ptszTmpS++;
					} else {
						if (i < 3) {
							l_ptszTmpN = l_ptszTmpS;
							while (1) {
								if (*l_ptszTmpN == _T(',')) {
									*l_ptszTmpN = 0;
									l_ptszTmpN++;
									break;
								}
								l_ptszTmpN++;
							}
						}
						long l_lData = _tcstoul(l_ptszTmpS, 0, 10);
						switch (i) {
						case 0:		g_CmdLine.rectPos.left = l_lData;	break;
						case 1:		g_CmdLine.rectPos.top = l_lData;	break;
						case 2:		g_CmdLine.rectPos.right = l_lData;	break;
						default:	g_CmdLine.rectPos.bottom = l_lData;	break;
						}
						l_ptszTmpS = l_ptszTmpN;
					}
				}
			}
			if (g_CmdLine.rectPos.left < 0) {
				g_CmdLine.rectPos.right = -1;
			}
			if (g_CmdLine.rectPos.top < 0) {
				g_CmdLine.rectPos.bottom = -1;
			}
		}
	}

	/* デスクトップのサイズ	*/
	POINT l_ptDeskTop;
	l_ptDeskTop.x = ::GetSystemMetrics(SM_CXSCREEN);
	l_ptDeskTop.y = ::GetSystemMetrics(SM_CYSCREEN);

	/* 設定(ini)ファイルの読込み	*/
	memset(&g_IniParams, 0, sizeof(g_IniParams));
	DWORD l_dwRc = ::GetFileAttributes(g_tszPathIni);
	switch (l_dwRc) {
	case -1:
	case FILE_ATTRIBUTE_DIRECTORY:
		/* ini ファイル無し	*/
		/* ウィンドウ Z オーダ	*/
		switch (g_CmdLine.lWzo) {
		case 0:		/* 変更無し	*/
			g_IniParams.Window.lZorder = 0;
			g_IniParams.Window.hWnd = (HWND) -1;
			break;
		case 1:		/* 最前面	*/
			g_IniParams.Window.lZorder = 1;
			g_IniParams.Window.hWnd = (HWND) -1;
			break;
		case 2:		/* 最前面２	*/
			g_IniParams.Window.lZorder = 2;
			g_IniParams.Window.hWnd = (HWND) -1;
			break;
		default:	/* ウィンドウハンドル	*/
			g_IniParams.Window.lZorder = 3;
			g_IniParams.Window.hWnd = (HWND) g_CmdLine.lWzo;
			break;
		}
		/* ビットマップのロード	*/
		LoadBmp(g_tszPathBmp);
		/* 表示位置＆サイズ	*/
		if (g_CmdLine.rectPos.left < 0) {
			g_IniParams.Window.rectPos.left = (l_ptDeskTop.x - g_BmpInfo.bmWidth) / 2;
		} else {
			g_IniParams.Window.rectPos.left = g_CmdLine.rectPos.left;
		}
		if (g_CmdLine.rectPos.top < 0) {
			g_IniParams.Window.rectPos.top = (l_ptDeskTop.y - g_BmpInfo.bmHeight) / 2;
		} else {
			g_IniParams.Window.rectPos.top = g_CmdLine.rectPos.top;
		}
		if (g_CmdLine.rectPos.right < 0) {
			g_IniParams.Window.rectPos.right = g_IniParams.Window.rectPos.left + g_BmpInfo.bmWidth - 1;
		} else {
			g_IniParams.Window.rectPos.right = g_CmdLine.rectPos.right;
		}
		if (g_CmdLine.rectPos.bottom < 0) {
			g_IniParams.Window.rectPos.bottom = g_IniParams.Window.rectPos.top + g_BmpInfo.bmHeight - 1;
		} else {
			g_IniParams.Window.rectPos.bottom = g_CmdLine.rectPos.bottom;
		}
		/* アニメ	*/
		if (1 == g_CmdLine.lAnimate) {
			/* デフォルトアニメ	*/
			g_IniParams.Animate.lNum = 0;
		} else {
			/* アニメ無し(デフォルト)	*/
			g_IniParams.Animate.lNum = -1;
		}
		/* 表示時間	*/
		g_IniParams.Window.dwDispTime = g_CmdLine.dwDispTime;
		/* アイコンファイル	*/
		_tcscpy(g_IniParams.Resource.tszPathIco, g_tszPathIco);
		/* ビットマップファイル	*/
		_tcscpy(g_IniParams.Resource.tszPathBmp, g_tszPathBmp);
		break;
	default:
		/* ini ファイル有り	*/
		TCHAR l_tszSec[32];
		TCHAR l_tszTmp[MAX_PATH];
		long l_lTmp;
		_tcscpy(l_tszSec, _T("RESOURCE"));	/* リソース	*/
		/* アイコンのパス	*/
		::GetPrivateProfileString(l_tszSec, _T("ICON"), _T(""), l_tszTmp, MAX_PATH - 1, g_tszPathIni);
		if (l_tszTmp[1] == _T(':')) {
			_tcscpy(g_IniParams.Resource.tszPathIco, l_tszTmp);
		} else {
			_stprintf(g_IniParams.Resource.tszPathIco, _T("%s%s"), g_tszDirExe, l_tszTmp);
		}
		/* ビットマップのパス	*/
		::GetPrivateProfileString(l_tszSec, _T("BMP"), _T(""), l_tszTmp, MAX_PATH - 1, g_tszPathIni);
		if (l_tszTmp[1] == _T(':')) {
			_tcscpy(g_IniParams.Resource.tszPathBmp, l_tszTmp);
		} else {
			_stprintf(g_IniParams.Resource.tszPathBmp, _T("%s%s"), g_tszDirExe, l_tszTmp);
		}
		_tcscpy(l_tszSec, _T("WINDOW"));	/* ウィンドウ	*/
		/* ZORDER	*/
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("ZORDER"), 0, g_tszPathIni);
		if ((l_lTmp < 0) || (4 <= l_lTmp)) {
			g_IniParams.Window.lZorder = 0;
		} else {
			g_IniParams.Window.lZorder = l_lTmp;
		}
		/* ウィンドウハンドル	*/
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("HWMD"), -1, g_tszPathIni);
		if (l_lTmp < 3) {
			l_lTmp = -1;
		}
		if ((3 == g_IniParams.Window.lZorder) && (-1 == l_lTmp)) {
			g_IniParams.Window.lZorder = 0;
		}
		/* ビットマップのロード	*/
		LoadBmp(g_IniParams.Resource.tszPathBmp);
		/* 表示位置＆サイズ	*/
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("LEFT"), -1, g_tszPathIni);
		g_IniParams.Window.rectPos.left = l_lTmp;
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("TOP"), -1, g_tszPathIni);
		g_IniParams.Window.rectPos.top = l_lTmp;
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("RIGHT"), -1, g_tszPathIni);
		g_IniParams.Window.rectPos.right = l_lTmp;
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("BOTTOM"), -1, g_tszPathIni);
		g_IniParams.Window.rectPos.bottom = l_lTmp;
		if (g_IniParams.Window.rectPos.left < 0) {
			g_IniParams.Window.rectPos.right = -1;
		}
		if (g_IniParams.Window.rectPos.top < 0) {
			g_IniParams.Window.rectPos.bottom = -1;
		}
		if (g_IniParams.Window.rectPos.left < 0) {
			g_IniParams.Window.rectPos.left = (l_ptDeskTop.x - g_BmpInfo.bmWidth) / 2;
		}
		if (g_IniParams.Window.rectPos.top < 0) {
			g_IniParams.Window.rectPos.top = (l_ptDeskTop.x - g_BmpInfo.bmHeight) / 2;
		}
		if (g_IniParams.Window.rectPos.right < 0) {
			g_IniParams.Window.rectPos.right = g_IniParams.Window.rectPos.left + g_BmpInfo.bmWidth - 1;
		}
		if (g_IniParams.Window.rectPos.bottom < 0) {
			g_IniParams.Window.rectPos.bottom = g_IniParams.Window.rectPos.top + g_BmpInfo.bmHeight - 1;
		}
		/* 表示時間	*/
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("DISPTIME"), 2000, g_tszPathIni);
		g_IniParams.Window.dwDispTime = l_lTmp;
		_tcscpy(l_tszSec, _T("ANIMATE"));	/* アニメ	*/
		/* 処理数，アニメ種類，時間	*/
		l_lTmp = ::GetPrivateProfileInt(l_tszSec, _T("NUM"), -1, g_tszPathIni);
		if (99 < l_lTmp) {
			g_IniParams.Animate.lNum = 99;
		} else {
			g_IniParams.Animate.lNum = l_lTmp;
		}
		if (0 < g_IniParams.Animate.lNum) {
			TCHAR l_tszNum[32];
			TCHAR l_tszModeBase[8] = _T("Mode");
			TCHAR l_tszTimeBase[8] = _T("Time");
			TCHAR l_tszMode[8];
			TCHAR l_tszTime[8];
			for (int i = 0; i <= g_IniParams.Animate.lNum; i++) {
				_stprintf(l_tszNum, _T("%02d"), i);
				_tcscpy(l_tszMode, l_tszModeBase);
				_tcscat(l_tszMode, l_tszNum);
				_tcscpy(l_tszTime, l_tszTimeBase);
				_tcscat(l_tszTime, l_tszNum);
				if (0 == i) {
					l_lTmp = ::GetPrivateProfileInt(l_tszSec, l_tszMode, AW_CENTER | AW_HIDE, g_tszPathIni);
				} else {
					l_lTmp = ::GetPrivateProfileInt(l_tszSec, l_tszMode, AW_CENTER, g_tszPathIni);
				}
				g_IniParams.Animate.MdTm[i].lMode = l_lTmp;
				l_lTmp = ::GetPrivateProfileInt(l_tszSec, l_tszTime, 500, g_tszPathIni);
				g_IniParams.Animate.MdTm[i].dwTime = l_lTmp;
			}
		}
		break;
	}
	if (0 == g_IniParams.Animate.lNum) {
		/* デフォルトアニメ	*/
		g_IniParams.Animate.lNum = 4;
//		g_IniParams.Animate.MdTm[0].lMode = AW_CENTER | AW_HIDE;
		g_IniParams.Animate.MdTm[0].lMode = AW_BLEND | AW_HIDE;
		g_IniParams.Animate.MdTm[0].dwTime = 3000;
		g_IniParams.Animate.MdTm[1].lMode = AW_CENTER;
		g_IniParams.Animate.MdTm[1].dwTime = 1000;
		g_IniParams.Animate.MdTm[2].lMode = -100;
		g_IniParams.Animate.MdTm[2].dwTime = 1000;
		g_IniParams.Animate.MdTm[3].lMode = AW_CENTER | AW_HIDE;
		g_IniParams.Animate.MdTm[3].dwTime = 1000;
		g_IniParams.Animate.MdTm[4].lMode = AW_BLEND;
		g_IniParams.Animate.MdTm[4].dwTime = 3000;
	}

	/* アイコンのロード	*/
	g_hIcon32 = 0;
	g_hIcon32 = (HICON) ::LoadImage(
							NULL,
							g_IniParams.Resource.tszPathIco,
							IMAGE_ICON,
							32, 32,
							LR_LOADFROMFILE
						);
	g_hIcon16 = 0;
	g_hIcon16 = (HICON) ::LoadImage(
							NULL,
							g_IniParams.Resource.tszPathIco,
							IMAGE_ICON,
							16, 16,
							LR_LOADFROMFILE
						);
	/* カーソルの設定	*/
	g_hCursor = 0;
	g_hCursor = (HCURSOR) LoadImage(
								NULL,
								MAKEINTRESOURCE(OCR_NORMAL),
								IMAGE_CURSOR,
								0, 0,
								LR_DEFAULTSIZE | LR_SHARED
							);

	return TRUE;
}

void ExitInstance()
{
	if (0 != g_hMutex_Run) {
		::ReleaseMutex(g_hMutex_Run);
		::CloseHandle(g_hMutex_Run);
	}
	if (0 != g_hIcon32) {
		::DestroyIcon(g_hIcon32);
	}
	if (0 != g_hIcon16) {
		::DestroyIcon(g_hIcon16);
	}
	if (0 != g_hBmp) {
		::DeleteObject(g_hBmp);
	}
	if (0 != g_hCursor) {
		::DestroyCursor(g_hCursor);
	}
}

/*
 *	ウィンドウプロシージャ
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC l_hDc;
	PAINTSTRUCT l_PaintStruct;
	static DWORD ls_dwCnt = 0;
	static long ls_lCmd = 0;

	switch (msg) {
	case WM_CREATE:
		{
			/* ツールウィンドウにすることでタスクバーに表示しない	*/
			long l_lExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			l_lExStyle |= WS_EX_TOOLWINDOW;
			::SetWindowLong(hWnd, GWL_EXSTYLE, l_lExStyle);
			/* タイトルバー(キャプション)を消す
			   枠を消す	*/
			long l_lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			l_lStyle &= ~WS_CAPTION;
			l_lStyle &= ~WS_THICKFRAME;
			::SetWindowLong(hWnd, GWL_STYLE, l_lStyle);
			/* Z オーダ	*/
			HWND l_hWnd = (HWND) -1;
			HWND l_hWndInsertAfter = 0;
			switch (g_IniParams.Window.lZorder) {
			case 1:		/* 最前面	*/
				l_hWnd = hWnd;
				l_hWndInsertAfter = HWND_TOPMOST;
				break;
			case 2:		/* 最前面２	*/
				l_hWnd = hWnd;
				l_hWndInsertAfter = HWND_TOPMOST;
				break;
			case 3:		/* ウィンドウハンドル	*/
				l_hWnd = g_IniParams.Window.hWnd;
				l_hWndInsertAfter = hWnd;
				break;
			case 0:		/* 変更無し	*/
			default:	/* デフォルト	*/
				break;
			}
			if (((HWND) -1) != l_hWnd) {
				BOOL l_bRc = ::SetWindowPos(
									l_hWnd,
									l_hWndInsertAfter,
									0, 0,
									0, 0,
									SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
								);
			}
		}
		/* 起動時のアニメ	*/
		g_lAnimeCnt = 0;
		if (g_IniParams.Animate.lNum <= 0) {
			/* 何もしない	*/
		} else {
			g_lAnimeCnt = 1;	/* 初回アニメ	*/
			AnimateWindow(	hWnd,
							g_IniParams.Animate.MdTm[g_lAnimeCnt].dwTime,
							g_IniParams.Animate.MdTm[g_lAnimeCnt].lMode);
		}
		::SetForegroundWindow(hWnd);
		/* タイマ開始	*/
		g_iPrc = 0;
		g_bWM_CLOSE_No1st = FALSE;
		::SetTimer(hWnd, TMR_INTVL_ID, TMR_INTVL_TIME, 0);
		::SetTimer(hWnd, TMR_INTVL_ID2, TMR_INTVL_TIME2, 0);
		break;
	case WM_TIMER:
		switch (wParam) {
		case TMR_INTVL_ID2:
			::KillTimer(hWnd, TMR_INTVL_ID2);
			/* 最前面２	*/
			if (2 == g_IniParams.Window.lZorder) {
				BOOL l_bRc = ::SetWindowPos(
									hWnd,
									HWND_TOPMOST,
									0, 0,
									0, 0,
									SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
								);
			}
			::SetTimer(hWnd, TMR_INTVL_ID2, TMR_INTVL_TIME2, 0);
			break;
		case TMR_INTVL_ID:
			::KillTimer(hWnd, TMR_INTVL_ID);
			/* アニメ判定	*/
			if (0 < g_IniParams.Animate.lNum) {
				/* アニメ有り	*/
				switch (g_iPrc) {
				case 0:		/* 次コマンド処理	*/
					g_lAnimeCnt++;	/* コマンドポインタ更新	*/
					if (g_lAnimeCnt <= g_IniParams.Animate.lNum) {
						/* コマンドポインタが設定数以内	*/
						if (g_IniParams.Animate.MdTm[g_lAnimeCnt].lMode < 0) {
							/* 拡張コマンド	*/
							ls_lCmd = g_IniParams.Animate.MdTm[g_lAnimeCnt].lMode * -1;
							if ((0 <= ls_lCmd) && (ls_lCmd <= 99)) {
								/* コマンド番号にジャンプ	*/
								if (g_IniParams.Animate.lNum < ls_lCmd) {
									ls_lCmd = g_IniParams.Animate.lNum;
								}
								g_lAnimeCnt = ls_lCmd - 1;	/* 指定コマンド番号−１ をセット	*/
							} else {
								ls_dwCnt = 0;
								g_iPrc = 2;	/* ウェイトへ	*/
							}
						} else {
							/* 標準コマンド(アニメコマンド)	*/
							AnimateWindow(	hWnd,
											g_IniParams.Animate.MdTm[g_lAnimeCnt].dwTime,
											g_IniParams.Animate.MdTm[g_lAnimeCnt].lMode);
						}
					} else {
						/* コマンド数分処理終了	*/
						g_iPrc = 1;	/* 表示時間待ちへ	*/
						ls_dwCnt = 0;
					}
					::SetTimer(hWnd, TMR_INTVL_ID, TMR_INTVL_TIME, 0);
					break;
				case 1:		/* 表示時間待ちへ	*/
					if (0 < g_IniParams.Window.dwDispTime) {
						ls_dwCnt++;
						if (g_IniParams.Window.dwDispTime < ((DWORD) (ls_dwCnt * TMR_INTVL_TIME))) {
							g_iPrc = 98;	/* 終了用アニメ実行へ	*/
						}
					} else {
						if (FALSE != g_bWM_CLOSE_No1st) {
							g_iPrc = 98;	/* 終了用アニメ実行へ	*/
						}
					}
					::SetTimer(hWnd, TMR_INTVL_ID, TMR_INTVL_TIME, 0);
					break;
				case 2:		/* ウェイト	*/
					ls_dwCnt++;
					if (g_IniParams.Animate.MdTm[g_lAnimeCnt].dwTime < ((DWORD) (ls_dwCnt * TMR_INTVL_TIME))) {
						g_iPrc = 0;	/* 次のコマンドへ	*/
					}
					::SetTimer(hWnd, TMR_INTVL_ID, TMR_INTVL_TIME, 0);
					break;
				case 98:	/* 終了用アニメ実行	*/
					AnimateWindow(	hWnd,
									g_IniParams.Animate.MdTm[0].dwTime,
									g_IniParams.Animate.MdTm[0].lMode);
					g_iPrc = 99;	/* WM_CLOSE 送信へ	*/
					::SetTimer(hWnd, TMR_INTVL_ID, TMR_INTVL_TIME, 0);
					break;
				case 99:	/* WM_CLOSE 送信	*/
					::PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				}
			} else {
				/* アニメ無し	*/
				if (0 < g_IniParams.Window.dwDispTime) {
					/* 表示時間	*/
					ls_dwCnt++;
					if (g_IniParams.Window.dwDispTime < ((DWORD) (ls_dwCnt * TMR_INTVL_TIME))) {
						::PostMessage(hWnd, WM_CLOSE, 0, 0);
					} else {
						::SetTimer(hWnd, TMR_INTVL_ID, TMR_INTVL_TIME, 0);
					}
				}
			}
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (0 < g_IniParams.Animate.lNum) {
			AnimateWindow(hWnd, 100, AW_CENTER);
		}
		if (0 < g_IniParams.Window.dwDispTime) {
			::KillTimer(hWnd, TMR_INTVL_ID);
			::PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	case WM_PRINTCLIENT:
		l_hDc = (HDC) wParam;
		if (0 != g_hBmp) {
			DrawBitmap(l_hDc, g_hBmp, &g_BmpInfo);
		}
		break;
	case WM_PAINT:
		l_hDc = ::BeginPaint(hWnd, &l_PaintStruct);
		if (0 != g_hBmp) {
			DrawBitmap(l_hDc, g_hBmp, &g_BmpInfo);
		}
		::EndPaint(hWnd, &l_PaintStruct);
		break;
	case WM_CLOSE:
		{
			BOOL l_bClose = FALSE;
			if (g_IniParams.Animate.lNum < 0) {
				/* アニメ無し	*/
				l_bClose = TRUE;
			} else {
				/* アニメ有り	*/
				if (99 <= g_iPrc) {
					l_bClose = TRUE;
				} else {
					if (FALSE != g_bWM_CLOSE_No1st) {
						l_bClose = TRUE;
					} else {
						g_bWM_CLOSE_No1st = TRUE;
					}
				}
			}
			if (FALSE != l_bClose) {
				::KillTimer(hWnd, TMR_INTVL_ID);
				::KillTimer(hWnd, TMR_INTVL_ID2);
				::DestroyWindow(hWnd);
			}
		}
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

/*
 *	ビットマップ描画
 */
void DrawBitmap(HDC hDc, HBITMAP hBmp, LPBITMAP pBmpInfo)
{
	HDC l_hDcMem;

	l_hDcMem = ::CreateCompatibleDC(hDc);
	::SelectObject(l_hDcMem, hBmp);
	::BitBlt(hDc, 0, 0, pBmpInfo->bmWidth, pBmpInfo->bmHeight, l_hDcMem, 0, 0, SRCCOPY);
	::DeleteDC(l_hDcMem);
}

/*
 *	ビットマップロード
 */
void LoadBmp(LPCTSTR ptszBitmapFile)
{
		g_hBmp = 0;
		g_hBmp = (HBITMAP) ::LoadImage(
								NULL,
								ptszBitmapFile,
								IMAGE_BITMAP,
								0, 0,
								LR_LOADFROMFILE
							);
		if (0 != g_hBmp) {
			::GetObject(g_hBmp, (int) sizeof(BITMAP), &g_BmpInfo);
		} else {
			g_BmpInfo.bmWidth = BMP_NG_WIDTH;
			g_BmpInfo.bmHeight = BMP_NG_HEIGHT;
		}
}
