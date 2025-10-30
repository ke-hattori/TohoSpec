/** CODE ********************************************************
 *                    Copyright (C) 1993 by Tencor Instruments
 *  ................. Source Code Control System (PVCS) keywords
 *
 *  NAME:   $Workfile: MOTFILE.C $
 *
 *  $Revision: 7 $
 *    $Author: Hmenjo $
 *      $Date: 02/01/16 15:43 $
 *   $Modtime: 02/01/16 15:42 $
 *       .Log:...see below...
 *  .............................................................
 *
 *  DESCRIPTION:
 *
 *  CONTENTS:
 *
 *  .....$Log: /ORCA3.2.2J/src/motsys/MOTFILE.C $
//
//  7     02/01/16 15:43 Hmenjo
//  日本語コメントを追加．
//
//  6     02/01/16 15:36 Hmenjo
//  日本語コメントを追加．
//
//  5     02/01/16 13:25 Hmenjo
//  日本語コメントを追加．
//
//  4     02/01/11 17:03 Hmenjo
//  日本語コメントを追加．
//
//  2     01/03/05 14:22 Hmenjo
//  $NoKeywords: $を追加．
//  コード内のバージョン情報を元に戻す．
//
//  1     00/11/01 9:22a Hmenjo
//  orca3.2.2の正常コンパイルのため．
//
//  $NoKeywords: $
//
//     Rev 1.4   17 Apr 1996 09:45:14   RLAMB
//  re-entered CIB interface changes
 *
 *     Rev 1.2   27 May 1994 10:05:50   Heinrich Gantenbein (HG)
 *  Increased PWM Filter value from 2 to 4.
 *
 *     Rev 1.1   23 Dec 1993 11:55:40   Peter F. Spicer (PFS)
 *
 *  removed use to TBPRO libs and dlls
 *
 *     Rev 1.0   30 Aug 1993 17:25:06   Lynn Chuu (LC)
 *  Initial revision.
 *
 ***************************************************************/
//static char ORCA_PVCSID[] = "$Header:   O:/orca/src/motsys/vcs/motfile.cv_   1.4   17 Apr 1996 09:45:14   RLAMB  $";

// ↓追加 by hmenjo
#include <windows.h>
#include "..\UPI_IRQDLL\tohoiodllexp.h"
#include <MotsysMsg.h>
#include <windowsx.h>
// ↑追加 by hmenjo

#include "motglob.h"
//#include <stdlib.h>
//#include <toolbox.h>
#include "motsdlg.h"


static MOT_DEF_PARAMS axis_default_data[axis_limit];

static MOT_PARAMS mot_data[axis_limit];

static char szCfgName[12] = "mot_cfg";

static char szAxisName[axis_limit][10];

static MOT_AXIS_DATA axis_mot_data[axis_limit];

static const BYTE pwm_filter_count = 4;

extern short alloc_axis;

extern WORD HCTL_read_delay;
extern WORD motion_base_port[2];


/*
 *	軸毎のモーションパラメタ書込み
 */
void WriteAxis(short axis)
{
	int fd;
	char  szPathName[128];
	LPMOT_DEF_PARAMS lpDef = &(axis_default_data[axis]);
	LPMOT_PARAMS lpMot = &(mot_data[axis]);

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szAxisName[axis]);	/* ReadConfig()で読出した値 */
	lstrcat(szPathName, ".dat");

	if ((fd = _lcreat(szPathName, 0)) >= 0) {
		/* ファイル「軸名.dat」が作成出来た */
		/* モーションパラメタを書込み */
		_lwrite(fd, (LPSTR) lpMot, sizeof(MOT_PARAMS));
		/* モーションデフォルトパラメタを書込み */
		_lwrite(fd, (LPSTR) lpDef, sizeof(MOT_DEF_PARAMS));
		_lclose(fd);
	}
}

/*
 *	軸毎のモーションパラメタを読出し
 */
BOOL ReadAxis(short axis)
{
	int		fd;
	char	szPathName[128];
	LPMOT_DEF_PARAMS	lpDef = &(axis_default_data[axis]);
	LPMOT_PARAMS		lpMot = &(mot_data[axis]);
	BOOL		l_rc = TRUE;

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szAxisName[axis]);	/* ReadConfig()で読出した値 */
	lstrcat(szPathName, ".dat");

	if ((fd = _lopen (szPathName, OF_READ)) >= 0) {
		/* ファイル「軸名.dat」がオープン出来た */
		/* モーションパラメタを読出し */
		_lread(fd, (LPSTR) lpMot, sizeof(MOT_PARAMS));
		/* モーションデフォルトパラメタを読出し */
		_lread(fd, (LPSTR) lpDef, sizeof(MOT_DEF_PARAMS));
		/* モーションデフォルトパラメタ内にモーションパラメタのアドレスをセット */
		lpDef->lpDefaultMotParams = lpMot;
		_lclose(fd);
	} else {
		// ファイルオープンエラー
		l_rc = FALSE;
	}
	/* 連続カウントのしきい値をセット */
	lpMot->byPwmFilterCount = pwm_filter_count;

	return l_rc;
}

/*
 *	mot_cfg.dat に書込む
 */
void WriteConfig(void)
{
	int		fd;
	char	szPathName[128];
	int		i;

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szCfgName);	/* 「mot_cfg」このファイルの上方にある */
	lstrcat(szPathName, ".dat");

	if ((fd = _lcreat(szPathName, 0)) >= 0) {
		/* ファイル「mot_cfg.dat」が作成出来た */
		/* motion_base_port[0],[1] を書込み */
		_lwrite(fd, (LPSTR) motion_base_port, 2 * sizeof(WORD));
		/* 軸数を書込み */
		_lwrite(fd, (LPSTR) &alloc_axis, sizeof(short));
		/* 軸毎の書込み */
		for (i = 0; i < alloc_axis; i++) {
			/* 軸名を書込み */
			_lwrite(fd, (LPSTR) szAxisName[i], 8);
			/* 軸データを書込み */
			_lwrite(fd, (LPSTR) &axis_mot_data[i], sizeof(MOT_AXIS_DATA));
		}
		_lclose(fd);
	}
}

/*
 *	mot_cfg.dat を読出す
 */
int ReadConfig(void)
{
	int		fd;
	char	szPathName[128];
	int		i, j;

	/* Windows ディレクトリのパスを取得 */
	GetWindowsDirectory(szPathName, 128);
	lstrcat(szPathName, "\\");
	lstrcat(szPathName, szCfgName);	/* 「mot_cfg」このファイルの上方にある */
	lstrcat(szPathName, ".dat");

	if ((fd = _lopen(szPathName, OF_READ)) >= 0) {
		/* ファイル「mot_cfg.dat」オープン出来た*/
		/* motion_base_port[0],[1]を設定 */
		_lread(fd, (LPSTR) motion_base_port, 2 * sizeof(WORD));
		/* 軸数 alloc_axis を設定 */
		_lread(fd, (LPSTR) &alloc_axis, sizeof(short));
		/* 軸毎のデータ読出し */
		for (i = 0; i < alloc_axis; i++) {
			/* 軸名を読出す[８文字](BIGX など) */
			_lread(fd, (LPSTR) szAxisName[i], 8);
			/* 軸名の終端に 0x00 を入れる */
			for (j = 0; j < 8; j++) {
				char c;
				if ((c = szAxisName[i][j]) == ' ' || c == '\0')
				   break;
			}
			szAxisName[i][j] = 0;
			/* 軸データを読出す */
			_lread(fd, (LPSTR) &axis_mot_data[i], sizeof(MOT_AXIS_DATA));
		}
		_lclose(fd);
	} else {
		// ファイルオープンエラー
		alloc_axis = 0;
	}

	return alloc_axis;
}

/*
 *	setup axis
 *	軸をセットアップ
 */
int ConfigureAxes(void)
{
	int i;
	LPMOT_DEF_PARAMS lpDef;
	LPMOT_PARAMS lpMot;

	// Win32 版では未使用
	/* モーション割込ベクタ
	 *		割込番号が８以下	割込番号＋0x08
	 *					超		割込番号＋0x68
	 * モーション割込マスクビット_b7_________________________________b0_
	 *		割込番号が８以下	|__8_|__7_|__6_|__5_|__4_|__3_|__2_|__1_|
	 *					超		|_16_|_15_|_14_|_13_|_12_|_11_|_10_|__9_|
	 * モーション割込マスクレジスタ(8259)
	 *		割込番号が８以下	0x21
	 *					超		0xa1
	 */
	MOTION_INT_VECTOR = (wIrq > 8) ? wIrq + 0x70 - 0x08 : wIrq + 0x08;
	// Win32 版では未使用
	/* モーション割込マスクビット */
	MOTION_INT_MASK = (wIrq > 8) ? 1 << (wIrq - 8) : 1 << wIrq;
	// Win32 版では未使用
	/* モーション割込マスクレジスタ(8259) */
	MOTION_INT_8259_MASK_REG = (wIrq > 8) ? 0xA1 : 0x21;

	// 8255,8253,8259，各軸のモーションデータを初期化，モーション割込を初期化
	motion_initialize_data(0);
	for (i = 0; i < alloc_axis; i++) {
		// 軸毎に HCTL 等の制御アドレス情報を設定
		motion_add_axis((short) i, &axis_mot_data[i]);

		lpDef = &(axis_default_data[i]);	/* モーションデフォルトパラメタ */
		lpMot = &(mot_data[i]);				/* モーションパラメタ */
		// モータシステム初期化
		motion_initialize((short) i, lpDef, lpMot);
	}
	return alloc_axis;
}


static char sz_byte[]   = "0x%02X";
static char sz_word[]   = "0x%04X";
static char sz_short[]  = "%d";
static char sz_long[]   = "%ld";
static char sz_cvtbuf[20];
static char* npszEnd;

static int nChoice;


#define wid(id) GetDlgItem (hdlg, id)

/*
 *	以下は windowsx.h で定義されている．
 *	Static_SetText() -> SetWindowText()
 *	Static_GetText() -> GetWindowText()
 */

#define Static_SetHexWord(hwnd, wVal) \
   wsprintf (sz_cvtbuf, sz_word, wVal); \
   Static_SetText (hwnd, sz_cvtbuf)

#define Static_SetHexByte(hwnd, wVal) \
   wsprintf (sz_cvtbuf, sz_byte, wVal); \
   Static_SetText (hwnd, sz_cvtbuf)

#define Static_SetShort(hwnd, nVal) \
   wsprintf (sz_cvtbuf, sz_short, nVal); \
   Static_SetText (hwnd, sz_cvtbuf)

#define Static_SetLong(hwnd, lVal) \
   wsprintf (sz_cvtbuf, sz_long, lVal); \
   Static_SetText (hwnd, sz_cvtbuf)


#define Static_GetHexWord(hwnd) \
   Static_GetText (hwnd, sz_cvtbuf, 20), \
   (WORD)strtol (sz_cvtbuf, &npszEnd, 16)

#define Static_GetHexByte(hwnd) \
   Static_GetText (hwnd, sz_cvtbuf, 20), \
   (BYTE)strtol (sz_cvtbuf, &npszEnd, 16)

#define Static_GetShort(hwnd) \
   Static_GetText (hwnd, sz_cvtbuf, 20), \
   (short)strtol (sz_cvtbuf, &npszEnd, 10)

#define Static_GetLong(hwnd) \
   Static_GetText (hwnd, sz_cvtbuf, 20), \
   strtol (sz_cvtbuf, &npszEnd, 10)


static void pascal SetParams(
		HWND hdlg,
		int	index
	)
{
	LPMOT_DEF_PARAMS	lpMdp;
	LPMOT_PARAMS		lpMp;
	LPMOT_AXIS_DATA		lpMad;

	Static_SetHexWord(wid(IDD_MOT_BASE0), motion_base_port[0]);
	Static_SetHexWord(wid(IDD_MOT_BASE1), motion_base_port[1]);
	Static_SetShort(wid(IDD_IRQ), wIrq);

	lpMad = &(axis_mot_data[index]);
	lpMdp = &(axis_default_data[index]);
	lpMp  = &(mot_data[index]);

	Static_SetHexWord(wid(IDD_HCTL_PORT),		lpMad->wHctlPort);
	Static_SetHexWord(wid(IDD_HOME_PORT),		lpMad->wHomePort);
	Static_SetHexWord(wid(IDD_HOME_INT_PORT),	lpMad->wHomeIntPort);
	Static_SetHexWord(wid(IDD_X_STOP_PORT), 	lpMad->wXstopPort);
	Static_SetHexByte(wid(IDD_HOME_MASK),		lpMad->byHomeMask);
	Static_SetHexByte(wid(IDD_HOME_INT_MASK),	lpMad->byHomeIntMask);
	Static_SetHexByte(wid(IDD_X_STOP_MASK), 	lpMad->byXstopMask);

	Static_SetLong(wid(IDD_MINUS_LIMIT),		lpMdp->lMinusLimit);
	Static_SetLong(wid(IDD_PLUS_LIMIT), 		lpMdp->lPlusLimit);
	Button_SetCheck(wid(IDD_INVERTED_SENSE),	lpMdp->bInvertedSense);
	Button_SetCheck(wid(IDD_FORWARD_FOR_HOME),	lpMdp->bForwardForHome);
	Static_SetShort(wid(IDD_HOME_VELOCITY), 	lpMdp->nHomeVelocity);
	Static_SetShort(wid(IDD_HOME_ACCEL),		lpMdp->nHomeAccel);

	Static_SetShort(wid(IDD_LOW_GAIN),			lpMp->nLowGain);
	Static_SetShort(wid(IDD_HIGH_GAIN), 		lpMp->nHighGain);
	Static_SetShort(wid(IDD_ZERO),				lpMp->nZero);
	Static_SetShort(wid(IDD_SAMPLE_TIME),		lpMp->nSampleTime);
	Static_SetShort(wid(IDD_PWM_MAX),			lpMp->byPwmMax);
	Static_SetShort(wid(IDD_PWM_FILTER_COUNT),	lpMp->byPwmFilterCount);
}

static void pascal GetParams(
		HWND hdlg,
		int  index
	)
{
	LPMOT_DEF_PARAMS	lpMdp;
	LPMOT_PARAMS		lpMp;
	LPMOT_AXIS_DATA		lpMad;

	motion_base_port[0] = Static_GetHexWord(wid(IDD_MOT_BASE0));
	motion_base_port[1] = Static_GetHexWord(wid(IDD_MOT_BASE1));
	wIrq                = Static_GetShort(wid(IDD_IRQ));

	lpMad = &(axis_mot_data[index]);
	lpMdp = &(axis_default_data[index]);
	lpMp  = &(mot_data[index]);

	lpMad->wHctlPort       = Static_GetHexWord(wid(IDD_HCTL_PORT));
	lpMad->wHomePort       = Static_GetHexWord(wid(IDD_HOME_PORT));
	lpMad->wHomeIntPort    = Static_GetHexWord(wid(IDD_HOME_INT_PORT));
	lpMad->wXstopPort      = Static_GetHexWord(wid(IDD_X_STOP_PORT));
	lpMad->byHomeMask      = Static_GetHexByte(wid(IDD_HOME_MASK));
	lpMad->byHomeIntMask   = Static_GetHexByte(wid(IDD_HOME_INT_MASK));
	lpMad->byXstopMask     = Static_GetHexByte(wid(IDD_X_STOP_MASK));

	lpMdp->lMinusLimit     = Static_GetLong(wid(IDD_MINUS_LIMIT));
	lpMdp->lPlusLimit      = Static_GetLong(wid(IDD_PLUS_LIMIT));
	lpMdp->bInvertedSense  = Button_GetCheck(wid(IDD_INVERTED_SENSE));
	lpMdp->bForwardForHome = Button_GetCheck(wid(IDD_FORWARD_FOR_HOME));
	lpMdp->nHomeVelocity   = Static_GetShort(wid(IDD_HOME_VELOCITY));
	lpMdp->nHomeAccel      = Static_GetShort(wid(IDD_HOME_ACCEL));

	lpMp->nLowGain         = Static_GetShort(wid(IDD_LOW_GAIN));
	lpMp->nHighGain        = Static_GetShort(wid(IDD_HIGH_GAIN));
	lpMp->nZero            = Static_GetShort(wid(IDD_ZERO));
	lpMp->nSampleTime      = Static_GetShort(wid(IDD_SAMPLE_TIME));
	lpMp->byPwmMax         = Static_GetShort(wid(IDD_PWM_MAX));
	lpMp->byPwmFilterCount = Static_GetShort(wid(IDD_PWM_FILTER_COUNT));
}

static BOOL Config_InitDialog(
		HWND   hdlg,
		HWND   hwndFocus,
		LPARAM lParam
	)

{
	int i;
	HWND hwndCtl;
	char szNum[2];

	hwndCtl = wid(IDD_AXIS_NAME);
	for (i = 0; i < alloc_axis; i++) {
		ComboBox_AddString (hwndCtl, szAxisName[i]);
	}
	for (i = alloc_axis; i < 8; i++) {
		ComboBox_AddString(hwndCtl, "<unused>");
	}
	ComboBox_SetCurSel(hwndCtl, nChoice = 0);
	ComboBox_LimitText(hwndCtl, 8);

	hwndCtl = wid(IDD_AXIS_NUMBER);
	szNum[1] = 0;

	for (i=0;i<8;i++) {
		szNum[0] = '0' + i;
		ComboBox_AddString(hwndCtl, szNum);
	}
	ComboBox_SetCurSel(hwndCtl, nChoice);

	SetParams(hdlg, nChoice);

	return TRUE;
}

// この関数は Win32 版では未使用になった
static void Config_Command(
		HWND hdlg,
		int id,
		HWND hwndCtl,
		UINT codeNotify
	)
{
	switch (codeNotify) {
	case CBN_EDITCHANGE:
		if (id == IDD_AXIS_NAME) {
			ComboBox_GetText (hwndCtl, szAxisName[nChoice], 10);
		}
		break;
	case CBN_SELCHANGE:
		if (id == IDD_AXIS_NUMBER) {
			nChoice = ComboBox_GetCurSel(hwndCtl);
			ComboBox_SetCurSel(wid(IDD_AXIS_NAME), nChoice);
			ReadConfig();
			ReadAxis(nChoice);
			SetParams(hdlg, nChoice);
			if (nChoice >= alloc_axis) {
				SetFocus(wid(IDD_AXIS_NAME));
			}
		} else if (id == IDD_AXIS_NAME) {
			nChoice = ComboBox_GetCurSel(hwndCtl);
			ComboBox_SetCurSel(wid(IDD_AXIS_NUMBER), nChoice);
			ReadConfig();
			ReadAxis(nChoice);
			SetParams(hdlg, nChoice);
		}
		break;
	case BN_CLICKED:
		if (id == IDD_OK) {
			GetParams(hdlg, nChoice);
			WriteAxis(nChoice);
			WriteConfig();
		} else {
			EndDialog(hdlg, FALSE);
		}
		break;
	default:
		break;
	}
}


#if 0	// ↓以下は不要
BOOL _export CALLBACK ConfigDlg(
		HWND hdlg,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	)
{
	switch (msg) {
	case WM_INITDIALOG:
		// DlgBoxCenter (hdlg);
		Config_InitDialog(hdlg, (HWND)wParam, lParam);
		break;
	case WM_COMMAND:
		Config_Command(hdlg, (int) wParam,
					(HWND) LOWORD(lParam), (UINT) HIWORD(lParam));
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void DisplayConfiguration(HWND hwndParent)
{
	extern HINSTANCE hInstLib;
	DialogBox (hInstLib, "CONFIG", hwndParent, ConfigDlg);
}
#endif	// ↑以上は不要



