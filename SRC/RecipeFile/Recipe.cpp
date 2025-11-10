#include "stdafx.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\RecipeFile.hxx"
#include "Recipe.h"
#include "..\..\INC\ts_secure_crt.h"

#define BUFFER_LEN 		(1024)

/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
#include "..\\..\\INC\\ConfigFile.hxx"
#ifdef _DEBUG
	#pragma comment(lib, "..\\..\\LIB\\dTnsConfigFile.lib")
#else
	#pragma comment(lib, "..\\..\\LIB\\TnsConfigFile.lib")
#endif
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */
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

MAIN_RCP_INFO g_MainRcpInfo;
MULTI_RCP_INFO_HDR g_MultiRcpInfoHdr;
MEAS_PROG_INFO g_MeasProgInfo;
STAGE_PROG_INFO_HDR g_StageProgInfoHdr;
RECALIB_PROG_INFO g_RecalibProgInfo;
POINT_DESKEW_PROG_INFO g_PointDeskewProgInfo;
SAMPLEINFO g_SampleInfo;
USER_ACCOUNT g_UserAccount;
MEASUREMENT_DATA g_MeasurementData;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
STAGE_PROG_STRESS	lg_StageProgStress;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */
//2009.12.10 bagus Recipe Backup --{--
char g_szBackupPath[MAX_PATH+1];
BOOL g_bUseBackup;
//2009.12.10 bagus Recipe Backup --}--
//Saiki 20110208 Add ----->
STRESS_PARAM_INFO lg_StressParamInfo;
#define	LnPtThickness	_T("L%dP%d_Thickness")
#define StressParamSec	_T("STRESS PARAMETER")
#define PriorityKey		_T("PRIORITY")

//Saiki 20110208 Add <-----
// 2009.11.04 bagus RS 追加 --{--
// ==========================================================================
// 測定結果の単位の設定 (NanoSpec\System.h と同じ)
enum RESISTANCE_UNIT_4PP
{
	RESISTANCE_UNIT_MILL = 0,
	RESISTANCE_UNIT_MICRO,
	RESISTANCE_UNIT_NANO,
	RESISTANCE_UNIT_ANGSTROM,
	RESISTANCE_UNIT_MAX,
};

const LPCSTR RESISTANCE_UNIT_ITEM[] = {
	"mm",
	"um",
	"nm",
	"A",
};
// 2009.11.04 bagus RS 追加 --}--

// MainRecipeファイルのヘッドタイプを読み込むためのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE MainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"Name",						'C',		"",			g_MainRcpInfo.hdr.szName											},
	{"",						"HeadType",					'W',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.wHeadType							},
};

// SRのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",		'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]				},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]				},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]				},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1]					},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2]					},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3]					},
	{"",						"Focus",					'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.nFocus								},
	{"",						"AutoFocusFailOption",		'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption				},
	{"",						"FanalFocusFailDataValidOption",'I',	"0",		&g_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption		},
	{"",						"DefaultDisplay",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay						},
	{"",						"MeasurementReference",		'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam._SR.bMeasRef							},
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._SR.bRefWithAF							},
//2009.09.02 bagus se 機能拡張 --{--
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.09.02 bagus se 機能拡張 --}--
//2009.12.17 bagus 保存ファイル名変更 --{--
	{"",						"SaveNameWithDate",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
//2009.12.17 bagus 保存ファイル名変更 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
};

// SEのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SeMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",		'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
// 2009.09.29 bagus SE --{--
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SE.RecalibItem[RECALIB_1]				},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SE.RecalibItem[RECALIB_2]				},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SE.RecalibItem[RECALIB_3]				},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SE.szRecalib[RECALIB_1]					},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SE.szRecalib[RECALIB_2]					},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SE.szRecalib[RECALIB_3]					},
	{"",						"Focus",					'I',		"0",		&g_MainRcpInfo.MainRcpParam._SE.nFocus								},
	{"",						"AutoFocusFailOption",		'I',		"0",		&g_MainRcpInfo.MainRcpParam._SE.nAutoFocusFailOption				},
	{"",						"FanalFocusFailDataValidOption",'I',	"0",		&g_MainRcpInfo.MainRcpParam._SE.nFinalFocusFailDataValidOption		},
	{"",						"DefaultDisplay",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SE.nDefaultDisplay						},
	{"",						"MeasurementReference",		'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam._SE.bMeasRef							},
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._SE.bRefWithAF							},
// 2009.09.29 bagus SE --}--
//2009.09.02 bagus se 機能拡張 --{--
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.09.02 bagus se 機能拡張 --}--
//2009.12.17 bagus 保存ファイル名変更 --{--
	{"",						"SaveNameWithDate",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
//2009.12.17 bagus 保存ファイル名変更 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
};

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
// CompleteEASEのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE CompEASEMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",				'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szStage								},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.RecalibItem[RECALIB_1]				},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.RecalibItem[RECALIB_2]				},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.RecalibItem[RECALIB_3]				},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._COMPEASE.szRecalib[RECALIB_1]					},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._COMPEASE.szRecalib[RECALIB_2]					},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._COMPEASE.szRecalib[RECALIB_3]					},
	{"",						"Focus",					'I',		"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.nFocus								},
	{"",						"AutoFocusFailOption",		'I',		"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.nAutoFocusFailOption				},
	{"",						"FanalFocusFailDataValidOption",'I',	"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.nFinalFocusFailDataValidOption		},
	{"",						"DefaultDisplay",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._COMPEASE.nDefaultDisplay						},
	{"",						"MeasurementReference",		'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam._COMPEASE.bMeasRef							},
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._COMPEASE.bRefWithAF							},
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
	{"",						"SaveNameWithDate",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
};
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

// 2009.10.19 bagus MS 追加 --{--
#if 0
// IRSEのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE IrseMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",				'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
//2009.09.02 bagus se 機能拡張 --{--
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.09.02 bagus se 機能拡張 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
};
#else
// MSのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE MicroScopeMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",		'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._MScope.RecalibItem[RECALIB_1]			},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._MScope.RecalibItem[RECALIB_2]			},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._MScope.RecalibItem[RECALIB_3]			},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._MScope.szRecalib[RECALIB_1]				},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._MScope.szRecalib[RECALIB_2]				},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._MScope.szRecalib[RECALIB_3]				},
	{"",						"Focus",					'I',		"0",		&g_MainRcpInfo.MainRcpParam._MScope.nFocus							},
	{"",						"AutoFocusFailOption",		'I',		"0",		&g_MainRcpInfo.MainRcpParam._MScope.nAutoFocusFailOption			},
	{"",						"FanalFocusFailDataValidOption",'I',	"0",		&g_MainRcpInfo.MainRcpParam._MScope.nFinalFocusFailDataValidOption	},
	{"",						"DefaultDisplay",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._MScope.nDefaultDisplay					},
	{"",						"MeasurementReference",		'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam._MScope.bMeasRef						},
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._MScope.bRefWithAF						},
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.12.17 bagus 保存ファイル名変更 --{--
	{"",						"SaveNameWithDate",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
//2009.12.17 bagus 保存ファイル名変更 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
};
#endif
// 2009.10.19 bagus MS 追加 --}--

// 4PPのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE ResistMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",		'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
//2009.09.02 bagus se 機能拡張 --{--
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.09.02 bagus se 機能拡張 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
//2009.12.07 bagus CTA 修正 --{--
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._RS.bRefWithAF						},
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._RS.RecalibItem[RECALIB_1]			},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._RS.RecalibItem[RECALIB_2]			},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._RS.RecalibItem[RECALIB_3]			},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._RS.szRecalib[RECALIB_1]				},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._RS.szRecalib[RECALIB_2]				},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._RS.szRecalib[RECALIB_3]				},
//2009.12.07 bagus CTA 修正 --}--
//2009.12.17 bagus 保存ファイル名変更 --{--
	{"",						"SaveNameWithDate",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
//2009.12.17 bagus 保存ファイル名変更 --}--
};

// Contact AngleのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE ContactAngleMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",		'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"DeskewProgram",			'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szDeskew								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
// 2009.10.08 bagus CTA 追加 --{--
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._CA.RecalibItem[RECALIB_1]				},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._CA.RecalibItem[RECALIB_2]				},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._CA.RecalibItem[RECALIB_3]				},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._CA.szRecalib[RECALIB_1]					},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._CA.szRecalib[RECALIB_2]					},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._CA.szRecalib[RECALIB_3]					},
	{"",						"Focus",					'I',		"0",		&g_MainRcpInfo.MainRcpParam._CA.nFocus								},
	{"",						"AutoFocusFailOption",		'I',		"0",		&g_MainRcpInfo.MainRcpParam._CA.nAutoFocusFailOption				},
	{"",						"FanalFocusFailDataValidOption",'I',	"0",		&g_MainRcpInfo.MainRcpParam._CA.nFinalFocusFailDataValidOption		},
	{"",						"DefaultDisplay",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._CA.nDefaultDisplay						},
	{"",						"MeasurementReference",		'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam._CA.bMeasRef							},
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._CA.bRefWithAF							},
// 2009.10.08 bagus CTA 追加 --}--
//2009.09.02 bagus se 機能拡張 --{--
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.09.02 bagus se 機能拡張 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
//2009.12.17 bagus 保存ファイル名変更 --{--
	{"",						"SaveNameWithDate",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
//2009.12.17 bagus 保存ファイル名変更 --}--
};

// StressのMainRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE StressMainRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MainRecipe",				"MeasurementProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szMeas								},
	{"",						"StageProgram",		'C',		"",					g_MainRcpInfo.MainRcpParam.hdr.szStage								},
//	{"",						"PointDeskewProgram",		'C',		"",			g_MainRcpInfo.MainRcpParam.hdr.szPointDeskew						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
	{"",						"PointManualAdjustment",	'I',		"0",		&g_MainRcpInfo.MainRcpParam.hdr.nPointManualAdjustment				},
	{"",						"SampleID",					'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSampleID							},
	{"",						"SaveMeasurementResult",	'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveMeasResult						},
	{"",						"SaveOverwrite",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveOverwrite						},
	{"",						"FirstRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1]				},
	{"",						"SecondRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2]				},
	{"",						"ThirdRecalItem",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3]				},
	{"",						"FirstRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1]					},
	{"",						"SecondRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2]					},
	{"",						"ThirdRecalibration",		'C',		"",			g_MainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3]					},
	{"",						"Focus",					'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.nFocus								},
	{"",						"AutoFocusFailOption",		'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.nAutoFocusFailOption				},
	{"",						"FanalFocusFailDataValidOption",'I',	"0",		&g_MainRcpInfo.MainRcpParam._SR.nFinalFocusFailDataValidOption		},
	{"",						"DefaultDisplay",			'I',		"0",		&g_MainRcpInfo.MainRcpParam._SR.nDefaultDisplay						},
	{"",						"MeasurementReference",		'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam._SR.bMeasRef							},
	{"",						"ReferenceWithAF",			'b',		"TRUE",		&g_MainRcpInfo.MainRcpParam._SR.bRefWithAF							},
//2009.09.02 bagus se 機能拡張 --{--
	{"",						"GraphAutoScale",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bGraphAutoScale						},
//2009.09.02 bagus se 機能拡張 --}--
	{"",						"Comment",					'C',		"",			g_MainRcpInfo.hdr.szComment											},
//2009.12.17 bagus 保存ファイル名変更 --{--
	{"",						"SaveNameWithDate",			'b',		"FALSE",	&g_MainRcpInfo.MainRcpParam.hdr.bSaveNameWithDate					},
//2009.12.17 bagus 保存ファイル名変更 --}--
};

// MultiRecipeファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE MultiRecipeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MultiRecipe",				"Name",						'C',		"",			g_MultiRcpInfoHdr.hdr.szName										},
	{"",						"NumMainRecipe",			'W',		"0",		&g_MultiRcpInfoHdr.wNumMainRecipe									},
	{"",						"Comment",					'C',		"",			g_MultiRcpInfoHdr.hdr.szComment										},
};

// MeasurementProgramファイルのHeadType，ScanTypeを読み込むためのセクション，キー．デフォルト，変数の設定
const RECIPE_FILE MeasurementProgramDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MeasurementProgram",		"Name",						'C',		"",			g_MeasProgInfo.hdr.szName											},
	{"",						"HeadType",					'W',		"0",		&g_MeasProgInfo.ScanParams.hdr.wHeadType							},
	{"",						"ScanType",					'W',		"0",		&g_MeasProgInfo.ScanParams.hdr.wScanType							},
};

// SRのThickness(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrThicknessDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SrThickness",				"StartWaveLength",			'W',		"400",		&g_MeasProgInfo.ScanParams._SR.WavelenRange.wStart					},
	{"",						"EndWaveLength",			'W',		"800",		&g_MeasProgInfo.ScanParams._SR.WavelenRange.wEnd					},
	{"",						"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"IntergrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"OpticalFilter",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.wOpticsFilterType					},
	{"",						"EnhancedUvGain",			'b',		"FALSE",	&g_MeasProgInfo.ScanParams._SR.bEnhancedUv							},
	{"",						"Strategy",					'C',		"0",		g_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry				},
	{"",						"AddReflectanceMeasureFlag",'b',		"FALSE",	&g_MeasProgInfo.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag	},
	{"",						"FirstWaveLength",			'I',		"488",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[0]					},
	{"",						"SecondWaveLength",			'I',		"546",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[1]					},
	{"",						"ThirdWaveLength",			'I',		"633",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[2]					},
	{"",						"LifeTimeWaringFlag",		'b',		"FALSE",	&g_MeasProgInfo.Ref.hdr.bWarnFlag									},
	{"",						"LifeTime",					'D',		"1.0",		&g_MeasProgInfo.Ref.hdr.dLifeTime									},
	{"",						"ReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// SRのReflectance(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrReflectDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SrReflect",				"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"IntegrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"OpticalFilter",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.wOpticsFilterType					},
	{"",						"EnhancedUvGain",			'b',		"FALSE",	&g_MeasProgInfo.ScanParams._SR.bEnhancedUv							},
	{"",						"FirstWaveLength",			'I',		"488",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[0]					},
	{"",						"SecondWaveLength",			'I',		"546",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[1]					},
	{"",						"ThirdWaveLength",			'I',		"633",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[2]					},
	{"",						"LifeTimeWaringFlag",		'b',		"FALSE",	&g_MeasProgInfo.Ref.hdr.bWarnFlag									},
	{"",						"LifeTime",					'D',		"1.0",		&g_MeasProgInfo.Ref.hdr.dLifeTime									},
	{"",						"ReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// SRのTransmittance(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrTransmitDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
// 2009.10.27 bagus Gantry 追加修正 --{--
//	{"SrTransmit",				"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"SrGantryTransmit",		"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
// 2009.10.27 bagus Gantry 追加修正 --}--
	{"",						"IntegrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"OpticalFilter",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.wOpticsFilterType					},
	{"",						"EnhancedUvGain",			'b',		"FALSE",	&g_MeasProgInfo.ScanParams._SR.bEnhancedUv							},
	{"",						"FirstWaveLength",			'I',		"488",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[0]					},
	{"",						"SecondWaveLength",			'I',		"546",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[1]					},
	{"",						"ThirdWaveLength",			'I',		"633",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[2]					},
	{"",						"LifeTimeWaringFlag",		'b',		"FALSE",	&g_MeasProgInfo.Ref.hdr.bWarnFlag									},
	{"",						"LifeTime",					'D',		"1.0",		&g_MeasProgInfo.Ref.hdr.dLifeTime									},
	{"",						"ReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// SRのReflectanceCIE(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrReflectCieDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SrReflectCie",			"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"IntegrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"ColorSystems",				'W',		"0",		&g_MeasProgInfo.ScanParams._SR.CIEOptDesc.wColorSystem				},
	{"",						"IlluminantTable",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.CIEOptDesc.wIlluminant				},
	{"",						"Gamma",					'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.CIEOptDesc.dGammaCorrectFactor		},
	{"",						"ReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// SRのTransmittanceCIE(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrTransmitCieDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SrTransmitCie",			"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"IntegrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"ColorSystems",				'W',		"0",		&g_MeasProgInfo.ScanParams._SR.CIEOptDesc.wColorSystem				},
	{"",						"IlluminantTable",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.CIEOptDesc.wIlluminant				},
	{"",						"Gamma",					'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.CIEOptDesc.dGammaCorrectFactor		},
	{"",						"ReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// SRのOptical Density(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrOdDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SrOpticalDensity",		"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"IntegrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"FirstWaveLength",			'I',		"488",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[0]					},
	{"",						"SecondWaveLength",			'I',		"546",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[1]					},
	{"",						"ThirdWaveLength",			'I',		"633",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[2]					},
	{"",						"1stReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Measurement2ndReference",	'b',		"FALSE",	&g_MeasProgInfo.Ref2nd.bMeasure										},
	{"",						"2ndIntegrationTime",		'D',		"1.0",		&g_MeasProgInfo.Ref2nd.dIntegTime									},
	{"",						"2ndReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref2nd.szRefMaterial									},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// 2009.10.14 bagus Distance 追加 --{--
// SRのDistance(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SrDistanceDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SrDistance",				"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"OpticalFilter",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.wOpticsFilterType					},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
// 2009.10.14 bagus Distance 追加 --}--

// 4PP(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
// 2009.11.04 bagus RS 追加 --{--
#if 0
const RECIPE_FILE ResistDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
//	{"4PP",						"StartIndex",				'L',		"5",		&g_MeasProgInfo.ScanParams._RS.lStartIdx							},
	{"",						"LimitVoltage",				'W',		"0",		&g_MeasProgInfo.ScanParams._RS.wVoltageLimit						},
	{"",						"RCF",						'D',		"4.532",	&g_MeasProgInfo.ScanParams._RS.dRsCorrectFactor						},
	{"",						"Thickness",				'D',		"1.0",		&g_MeasProgInfo.ScanParams._RS.dThickness							},
	{"",						"ThicknessUnit",			'W',		"0",		&g_MeasProgInfo.ScanParams._RS.wUnits								},
	{"",						"ScanTime",					'D',		"2.0",		&g_MeasProgInfo.ScanParams._RS.dScanTime							},
// 2009.09.04 K.Matsuo -->
	{"",						"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._RS.iLens									},
	{"",						"OpticalFilter",			'W',		"1",		&g_MeasProgInfo.ScanParams._RS.wOpticsFilterType						},
// 2009.09.04 K.Matsuo <--
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
#else
const RECIPE_FILE ResistDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
//	{"RS",						"RsStartRange",				'L',		"0",		&g_MeasProgInfo.ScanParams._RS.lStartIdx							},
//	{"",						"RsLimitVol",				'W',		"10",		&g_MeasProgInfo.ScanParams._RS.wVoltageLimit						},
	{"RS",						"RsLimitVol",				'W',		"10",		&g_MeasProgInfo.ScanParams._RS.wVoltageLimit						},
	{"",						"RsRCF",					'D',		"4.532",	&g_MeasProgInfo.ScanParams._RS.dRsCorrectFactor						},
	{"",						"RsThick",					'D',		"0",		&g_MeasProgInfo.ScanParams._RS.dThickness							},
//	{"",						"RsThickUnit",				'W',		"0",		&g_MeasProgInfo.ScanParams._RS.wUnits								},
	{"",						"RsMeasWaitTime",			'D',		"0",		&g_MeasProgInfo.ScanParams._RS.dScanTime							},
	{"",						"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._RS.iLens								},
	{"",						"OpticalFilter",			'W',		"0",		&g_MeasProgInfo.ScanParams._RS.wOpticsFilterType					},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
#endif
// 2009.11.04 bagus RS 追加 --}--

// 2009.08.24 bagus stress	--{--
const RECIPE_FILE StressDesc[] =
{
//	Section Name				Key Name					Data Type	Default		Variable Pointer
// 2009.10.22 bagus Stress 修正 --{--
//	{"Stress",					"CalculationDataIntereval",	'L',		"10",		&g_MeasProgInfo.ScanParams._SR.dwCalcDataInterval					},
// 2009.10.22 bagus Stress 修正 --}--
	{"SrThickness",				"StartWaveLength",			'W',		"400",		&g_MeasProgInfo.ScanParams._SR.WavelenRange.wStart					},
	{"",						"EndWaveLength",			'W',		"800",		&g_MeasProgInfo.ScanParams._SR.WavelenRange.wEnd					},
	{"",						"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SR.iLens								},
	{"",						"IntergrationTime",			'D',		"1.0",		&g_MeasProgInfo.ScanParams._SR.dIntegTime							},
	{"",						"OpticalFilter",			'W',		"0",		&g_MeasProgInfo.ScanParams._SR.wOpticsFilterType					},
	{"",						"EnhancedUvGain",			'b',		"FALSE",	&g_MeasProgInfo.ScanParams._SR.bEnhancedUv							},
	{"",						"Strategy",					'C',		"0",		g_MeasProgInfo.ScanParams._SR.XMPDesc.szStrategyEntry				},
	{"",						"AddReflectanceMeasureFlag",'b',		"FALSE",	&g_MeasProgInfo.ScanParams._SR.XMPDesc.bAddReflectanceMeasureFlag	},
	{"",						"FirstWaveLength",			'I',		"488",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[0]					},
	{"",						"SecondWaveLength",			'I',		"546",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[1]					},
	{"",						"ThirdWaveLength",			'I',		"633",		&g_MeasProgInfo.ScanParams._SR.iSpecificWavelen[2]					},
	{"",						"LifeTimeWaringFlag",		'b',		"FALSE",	&g_MeasProgInfo.Ref.hdr.bWarnFlag									},
	{"",						"LifeTime",					'D',		"1.0",		&g_MeasProgInfo.Ref.hdr.dLifeTime									},
	{"",						"ReferenceStrategy",		'C',		"",			g_MeasProgInfo.Ref.OptDesc.szRefMaterial							},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};

// 2009.08.24 bagus stress	--}--

// 2009.10.08 bagus CTA 追加 --{--
const RECIPE_FILE ContactAngleDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"CA",						"MeasCA",					'b',		"FALSE",	&g_MeasProgInfo.ScanParams._CA.bContactAngle						},
	{"",						"MeasRedius",				'b',		"FALSE",	&g_MeasProgInfo.ScanParams._CA.bRadius								},
	{"",						"MeasLiquid",				'b',		"FALSE",	&g_MeasProgInfo.ScanParams._CA.bLiquidVolume						},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
// 2009.10.08 bagus CTA 追加 --}--

// 2009.09.04 K.Matsuo -->
// SE(MeasurementProgram)ファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SeThicknessDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"SE",						"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._SE.iLens									},
	{"",						"OpticalFilter",			'W',		"1",		&g_MeasProgInfo.ScanParams._SE.wOpticsFilterType						},
	{"",						"Strategy",					'C',		"",			g_MeasProgInfo.ScanParams._SE.szStrategyEntry						},
// 2009.09.19 bagus SE --{--
	{"",						"StartWaveLength",			'W',		"400",		&g_MeasProgInfo.ScanParams._SE.WavelenRange.wStart					},
	{"",						"EndWaveLength",			'W',		"800",		&g_MeasProgInfo.ScanParams._SE.WavelenRange.wEnd					},
// 2009.09.19 bagus SE --}--
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
// 2009.09.04 K.Matsuo <--

// 2013.02.01 bagus CompleteEASEヘッド追加 -->
const RECIPE_FILE CompEASEThicknessDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"EASE",					"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._COMPEASE.iLens							},
	{"",						"OpticalFilter",			'W',		"1",		&g_MeasProgInfo.ScanParams._COMPEASE.wOpticsFilterType				},
	{"",						"CompleteEASE_Recipe",		'C',		"",			g_MeasProgInfo.ScanParams._COMPEASE.szCompEASERecipe				},
	{"",						"CompleteEASE_ResultAutoSave",	'b',	"TRUE",		&g_MeasProgInfo.ScanParams._COMPEASE.bAutoSaveResultInCompEASE		},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
// 2013.02.01 bagus CompleteEASEヘッド追加 <--

//2009.10.20 bagus MS 追加 --{--
const RECIPE_FILE MicroScopeDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"MS",						"Lens",						'I',		"0",		&g_MeasProgInfo.ScanParams._MScope.iLens							},
	{"",						"OpticalFilter",			'W',		"1",		&g_MeasProgInfo.ScanParams._MScope.wOpticsFilterType				},
	{"",						"MagLensIndex",				'I',		"0",		&g_MeasProgInfo.ScanParams._MScope.iMagLensIndex					},
	{"",						"Comment",					'C',		"",			g_MeasProgInfo.hdr.szComment										},
};
//2009.10.20 bagus MS 追加 --}--

// StageProgramファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE StageProgramDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"StageProgram",			"Name",						'C',		"",			g_StageProgInfoHdr.hdr.szName										},
	{"",						"NumScans",					'W',		"0",		&g_StageProgInfoHdr.wNumScans										},
	{"",						"SampleName",				'C',		"",			g_StageProgInfoHdr.SampleInfo.szName								},
	{"",						"SampleShape",				'I',		"0",		&g_StageProgInfoHdr.SampleInfo.nShape								},
	{"",						"SampleThickness",			'D',		"1.0",		&g_StageProgInfoHdr.SampleInfo.dThickness							},
	{"",						"SampleSize_X",				'D',		"100.0",	&g_StageProgInfoHdr.SampleInfo.Size.dx								},
	{"",						"SampleSize_Y",				'D',		"100.0",	&g_StageProgInfoHdr.SampleInfo.Size.dy								},
	{"",						"SampleOrigin_X",			'L',		"0",		&g_StageProgInfoHdr.SampleInfo.Origin.lX							},
	{"",						"SampleOrigin_Y",			'L',		"0",		&g_StageProgInfoHdr.SampleInfo.Origin.lY							},
// 2009.12.01 bagus NANOMAP --{--
	{"",						"MachineOrgSensorLoc",			 'I',	 "3",		&g_StageProgInfoHdr.SampleInfo.StageConfig.MachineOrgSensorLoc				   },
	{"",						 "OriginPosition",				 'I',	 "1",		&g_StageProgInfoHdr.SampleInfo.StageConfig.OrgPos							   },
	{"",						 "CoordinateDirection_X",		 'I',	 "1",		&g_StageProgInfoHdr.SampleInfo.StageConfig.Dir.X							   },
	{"",						 "CoordinateDirection_Y",		 'I',	 "2",		&g_StageProgInfoHdr.SampleInfo.StageConfig.Dir.Y							   },
	//2009.12.03 bagus 修正 --{--
	//{"",						   "Size_X",					   'D',    "900",	  &g_StageProgInfoHdr.SampleInfo.StageConfig.Size.dX							 },
	//{"",						   "Size_Y",					   'D',    "900",	  &g_StageProgInfoHdr.SampleInfo.StageConfig.Size.dY							 },
	//{"",						   "Edge_X",					   'D',    "0",		&g_StageProgInfoHdr.SampleInfo.StageConfig.Edge.dX								},
	//{"",						   "Edge_Y",					   'D',    "0",		&g_StageProgInfoHdr.SampleInfo.StageConfig.Edge.dY								},
	//{"",						   "OriginCoordinate_X",		   'L',    "0", 	  &g_StageProgInfoHdr.SampleInfo.StageConfig.Origin.lX							 },
	//{"",						   "OriginCoordinate_Y",		   'L',    "0", 	  &g_StageProgInfoHdr.SampleInfo.StageConfig.Origin.lY							 },
	//{"",						   "StageMaxCoordLeft_X",		   'D',    "0", 	  &g_StageProgInfoHdr.SampleInfo.StageConfig.StageMaxCoord.dLeftX				 },
	//{"",						   "StageMaxCoordRight_X",		   'D',    "0", 	  &g_StageProgInfoHdr.SampleInfo.StageConfig.StageMaxCoord.dRightX				 },
	//{"",						   "StageMaxCoordUp_Y", 		   'D',    "0", 	  &g_StageProgInfoHdr.SampleInfo.StageConfig.StageMaxCoord.dUpY 				 },
	//{"",						   "StageMaxCoordDown_Y",		   'D',    "0", 	  &g_StageProgInfoHdr.SampleInfo.StageConfig.StageMaxCoord.dDownY				 },
	//2009.12.03 bagus 修正 --}--
// 2009.12.01 bagus NANOMAP --{--
	{"",						"HeadType",					'W',		"0",		&g_StageProgInfoHdr.wHeadType										},
	{"",						"ScanType",					'W',		"0",		&g_StageProgInfoHdr.wScanType										},
	{"",						"Lens",						'I',		"0",		&g_StageProgInfoHdr.iLens											},
	{"",						"DeskewMode",				'I',		"0",		&g_StageProgInfoHdr.nDeskewMode										},
	{"",						"DeskewFailOption",			'I',		"0",		&g_StageProgInfoHdr.nDeskewFailOption								},
	{"",						"AutoDeskew1Position_bDefined",	'b',	"FALSE",	&g_StageProgInfoHdr.DeskewSite[0].bDefined							},
	{"",						"AutoDeskew1Position_X",	'L',		"0",		&g_StageProgInfoHdr.DeskewSite[0].Loc.lX							},
	{"",						"AutoDeskew1Position_Y",	'L',		"0",		&g_StageProgInfoHdr.DeskewSite[0].Loc.lY							},
	{"",						"AutoDeskew1Size",			'W',		"0",		&g_StageProgInfoHdr.DeskewSite[0].wSize								},
	{"",						"AutoDeskew2Position_bDefined",	'b',	"FALSE",	&g_StageProgInfoHdr.DeskewSite[1].bDefined							},
	{"",						"AutoDeskew2Position_X",	'L',		"0",		&g_StageProgInfoHdr.DeskewSite[1].Loc.lX							},
	{"",						"AutoDeskew2Position_Y",	'L',		"0",		&g_StageProgInfoHdr.DeskewSite[1].Loc.lY							},
	{"",						"AutoDeskew2Size",			'W',		"0",		&g_StageProgInfoHdr.DeskewSite[1].wSize								},
	{"",						"ManualDeskew1Position_bDefined",'b',	"FALSE",	&g_StageProgInfoHdr.DeskewSiteManual[0].bDefined					},
	{"",						"ManualDeskew1Position_X",	'L',		"0",		&g_StageProgInfoHdr.DeskewSiteManual[0].Loc.lX						},
	{"",						"ManualDeskew1Position_Y",	'L',		"0",		&g_StageProgInfoHdr.DeskewSiteManual[0].Loc.lY						},
	{"",						"ManualDeskew2Position_bDefined",'b',	"FALSE",	&g_StageProgInfoHdr.DeskewSiteManual[1].bDefined					},
	{"",						"ManualDeskew2Position_X",	'L',		"0",		&g_StageProgInfoHdr.DeskewSiteManual[1].Loc.lX						},
	{"",						"ManualDeskew2Position_Y",	'L',		"0",		&g_StageProgInfoHdr.DeskewSiteManual[1].Loc.lY						},
	{"",						"SitePatternMode",			'I',		"0",		&g_StageProgInfoHdr.nSitePatternMode								},
	{"",						"SitePrFailOption",			'I',		"0",		&g_StageProgInfoHdr.nSitePrFailOption								},
	{"",						"Comment",					'C',		"",			g_StageProgInfoHdr.hdr.szComment									},
//2009.08.31 bagus stress --{--
	{"",						"ElasticModulusName",		'C',		"",			g_StageProgInfoHdr.SampleInfo.tszElasticModulusName					},
	{"",						"ElasticModulusValue",		'D',		"0.0",		&g_StageProgInfoHdr.SampleInfo.dElasticModulusValue					},
//2009.08.31 bagus stress --}--
};

// RecalibrationProgramファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE RecalibrationProgramDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"RecalibrationProgram",	"Name",						'C',		"",			g_RecalibProgInfo.hdr.szName										},
	{"",						"HeadType",					'W',		"0",		&g_RecalibProgInfo.wHeadType										},
	{"",						"ScanType",					'W',		"0",		&g_RecalibProgInfo.wScanType										},
	{"",						"Unit",						'W',		"0",		&g_RecalibProgInfo.wUnits											},
// 2009.09.15 K.Matsuo レキャリブレーションのマイナス値入力対応 -->
// 初期値が-1は、v2.10未満との互換性のため。
	{"",						"EntryNum",					'I',		"-1",		&g_RecalibProgInfo.iEntryNum										},
// 2009.09.15 K.Matsuo レキャリブレーションのマイナス値入力対応 <--
	{"",						"Actual1",					'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[0].dActual							},
	{"",						"Measured1",				'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[0].dMeasured						},
	{"",						"Actual2",					'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[1].dActual							},
	{"",						"Measured2",				'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[1].dMeasured						},
	{"",						"Actual3",					'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[2].dActual							},
	{"",						"Measured3",				'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[2].dMeasured						},
	{"",						"Actual4",					'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[3].dActual							},
	{"",						"Measured4",				'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[3].dMeasured						},
	{"",						"Actual5",					'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[4].dActual							},
	{"",						"Measured5",				'D',		"0.0",		&g_RecalibProgInfo.RecalibEntry[4].dMeasured						},
	{"",						"Comment",					'C',		"",			g_RecalibProgInfo.hdr.szComment										},
};

// PointDeskewProgramファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE PointDeskewProgramDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"PointDeskewProgram",		"Name",						'C',		"",			g_PointDeskewProgInfo.hdr.szName									},
	{"",						"HeadType",					'W',		"0",		&g_PointDeskewProgInfo.wHeadType									},
	{"",						"Lens",						'I',		"0",		&g_PointDeskewProgInfo.iLens										},
	{"",						"DeskewMode",				'I',		"0",		&g_PointDeskewProgInfo.nDeskewMode									},
	{"",						"DeskewFailOption",			'I',		"0",		&g_PointDeskewProgInfo.nDeskewFailOption							},
	{"",						"AutoDeskewPosition_X",		'L',		"0",		&g_PointDeskewProgInfo.DeskewSite.Loc.lX							},
	{"",						"AutoDeskewPosition_Y",		'L',		"0",		&g_PointDeskewProgInfo.DeskewSite.Loc.lY							},
	{"",						"AutoDeskewSize",			'W',		"0",		&g_PointDeskewProgInfo.DeskewSite.wSize								},
	{"",						"Comment",					'C',		"",			g_PointDeskewProgInfo.hdr.szComment									},
};

// Sampleファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE SampleDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"Sample",					"Name",						'C',		"",			g_SampleInfo.szName													},
	{"",						"Shape",					'I',		"0",		&g_SampleInfo.nShape												},
	{"",						"Thickness",				'D',		"1000",		&g_SampleInfo.dThickness/*um*/										},
	{"",						"Size_X",					'D',		"100000",	&g_SampleInfo.Size.dx/*um*/											},
	{"",						"Size_Y",					'D',		"100000",	&g_SampleInfo.Size.dy/*um*/											},
	{"",						"Origin_X",					'L',		"0",		&g_SampleInfo.Origin.lX/*um*/										},
	{"",						"Origin_Y",					'L',		"0",		&g_SampleInfo.Origin.lY	/*um*/										},
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
// 2009.11.09 bagus Stress 追加変更 --{--
//	{"",						"ElasticModulusName",		'C',		"",			g_SampleInfo.tszElasticModulusName									},
//	{"",						"ElasticModulusValue",		'D',		"0.0",		&g_SampleInfo.dElasticModulusValue									},
// 2009.11.09 bagus Stress 追加変更 --}--
/* added 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
// 2009.12.01 bagus NANOMAP --{--
	{"",						"MachineOrgSensorLoc",			 'I',	 "3",					 &g_SampleInfo.StageConfig.MachineOrgSensorLoc								 },
	{"",						 "OriginPosition",				 'I',	 "1",					 &g_SampleInfo.StageConfig.OrgPos											 },
	{"",						 "CoordinateDirection_X",		 'I',	 "1",					 &g_SampleInfo.StageConfig.Dir.X											 },
	{"",						 "CoordinateDirection_Y",		 'I',	 "2",					 &g_SampleInfo.StageConfig.Dir.Y											 },
	//2009.12.03 bagus 修正 --{--
	//{"",						   "Size_X",					   'D',    "900",				   &g_SampleInfo.StageConfig.Size.dX										   },
	//{"",						   "Size_Y",					   'D',    "900",				   &g_SampleInfo.StageConfig.Size.dY										   },
	//{"",						   "Edge_X",					   'D',    "0",					&g_SampleInfo.StageConfig.Edge.dX										   },
	//{"",						   "Edge_Y",					   'D',    "0",			 		&g_SampleInfo.StageConfig.Edge.dY										   },
	//{"",						   "OriginCoordinate_X",		   'L',    "0", 				   &g_SampleInfo.StageConfig.Origin.lX										   },
	//{"",						   "OriginCoordinate_Y",		   'L',    "0", 				   &g_SampleInfo.StageConfig.Origin.lY										   },
	//{"",						   "StageMaxCoordLeft_X",		   'D',    "0", 				   &g_SampleInfo.StageConfig.StageMaxCoord.dLeftX							   },
	//{"",						   "StageMaxCoordRight_X",		   'D',    "0", 				   &g_SampleInfo.StageConfig.StageMaxCoord.dRightX							   },
	//{"",						   "StageMaxCoordUp_Y", 		   'D',    "0", 				   &g_SampleInfo.StageConfig.StageMaxCoord.dUpY 							   },
	//{"",						   "StageMaxCoordDown_Y",		   'D',    "0", 				   &g_SampleInfo.StageConfig.StageMaxCoord.dDownY							   },
	//2009.12.03 bagus 修正 --}--
// 2009.12.01 bagus NANOMAP --{--
};

// UserAccountファイルのセクション，キー，デフォルト，変数の設定
const RECIPE_FILE UserAccountDesc[] =
{
//	Section Name				Key Name					Data Type	Defaule		Variable Pointer
	{"UserAccount",				"Name",						'C',		"",			g_UserAccount.szName												},
	{"",						"AccountLevel",				'I',		"0",		&g_UserAccount.nAccountLevel										},
	{"",						"EnablePassword",			'b',		"FALSE",	&g_UserAccount.bEnablePassword										},
	{"",						"Password",					'C',		"",			&g_UserAccount.szPassword											},
};
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
/* Stage PGM for ストレス ファイルのセクション，キー，デフォルト，変数の設定 */
DWORD lg_dwDummy;
#define	LS_VALIDLINEn		_T("ValidLine%d")
#define	LS_LnSnSCANVALID	_T("L%dS%d_ScanValid")
#define	LS_LnSnSCANSTART_X	_T("L%dS%d_ScanStart_X")
#define	LS_LnSnSCANEND_X	_T("L%dS%d_ScanEnd_X")
#define	LS_LnSnSCANY		_T("L%dS%d_Y")
/* 以降の定義は使いません．(直接読込みのためです) */
const RECIPE_FILE StageProgramStressDesc[] =
{
	/*Section Name				Key Name					Data Type	Defaule		Variable Pointer	*/
	{"StageProgram",			"NumLSScans",				'L',		"1",		g_UserAccount.szName												},
	{"",						"MaterialElasticModulus",	'C',		"",			&g_UserAccount.nAccountLevel										},
	{"LS",						LS_VALIDLINEn,				'b',		"FALSE",	&g_UserAccount.bEnablePassword										},
	{"",						LS_LnSnSCANVALID,			'b',		"FALSE",	&g_UserAccount.szPassword											},
	{"",						LS_LnSnSCANSTART_X,			'L',		"0",		&g_UserAccount.szPassword											},
	{"",						LS_LnSnSCANEND_X,			'L',		"0",		&g_UserAccount.szPassword											},
	{"",						LS_LnSnSCANY,				'L',		"0",		&g_UserAccount.szPassword											},
	{"",						"dmy",						'L',		"9",		&lg_dwDummy															},
};
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */

//// MeasurementDataファイルのセクション，キー，デフォルト，変数の設定
//const RECIPE_FILE MeasurementDataDesc[] =
//{
////	Section Name				Key Name					Data Type	Defaule		Variable Pointer
//	{"MeasurementData",			"Name",						'C',		"",			g_MeasurementData.szName											},
//	{"",						"HeadType",					'W',		"0",		&g_MeasurementData.wHeadType										},
//	{"",						"ScanType",					'W',		"0",		&g_MeasurementData.wScanType										},
//	{"",						"SampleName",				'C',		"",			&g_MeasurementData.szSampleName										},
//	{"",						"NumScans",					'W',		"0",		&g_MeasurementData.wNumScans										},
//	{"",						"FirstWaveLength",			'I',		"400",		&g_MeasurementData.iSpecificWavelen[0]								},
//	{"",						"SecondWaveLength",			'I',		"500",		&g_MeasurementData.iSpecificWavelen[1]								},
//	{"",						"ThirdWaveLength",			'I',		"600",		&g_MeasurementData.iSpecificWavelen[2]								},
//	{"",						"Max.",						'D',		"0",		&g_MeasurementData.dMaximumData										},
//	{"",						"Min.",						'D',		"0",		&g_MeasurementData.dMinimumData										},
//	{"",						"Mean",						'D',		"0",		&g_MeasurementData.dMeanData										},
//	{"",						"Range",					'D',		"0",		&g_MeasurementData.dRangeData										},
//	{"",						"S.D.",						'D',		"0",		&g_MeasurementData.dSdData											},
//};

// MultiRecipeのMainRecipeのリストのセクション名
char g_lpszMultiRcpMainRcpListSection[] = "[MainRecipe]";
// StageProgramのPointのリストのセクション名
char g_lpszStageProgPointListSection[] = "[Point]";
//// MeasurementDataの測定結果リストのセクション名
//char g_lpszMeasurementDataListSection[] = "[MeasurementDataList]";
//// StatisticsListの測定結果リストのセクション名
//char g_lpszStatisticsListSection[] = "[StatisticsList]";

/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
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
		_tcscpy_s(ptszPath, _MAX_PATH, _T(""));
	} else {
		_stprintf_s(ptszPath, _MAX_PATH, _T("%s%s"), g_tszProcDir, l_tszTempFName);
	}
}
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleの読み込み
// Parameters : pVoid	 ---> データを保存する構造体のポインタ
//				lpszName ---> ファイル名
//				iType	 ---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadRecipe(LPVOID pVoid, LPCSTR lpszName, int iType)
{
	int iIndex, iDescCount;
	LPCSTR section;
	char buff[BUFFER_LEN + 1], szFilePath[_MAX_PATH];
	RECIPE_FILE* pDesc = NULL;
	SYSTEMTIME lastWriteSystemTime;

	ClearMemory();

	// HeadType，ScanTypeを読み込む
	if(!LoadHeadTypeAndScanType(&iType, lpszName)){
			return FALSE;
	}

	switch(iType){
	// Recipe Setting関連
	case RECIPE_FILE_SR_MAIN_RECIPE:
		iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
	case RECIPE_FILE_SE_MAIN_RECIPE:
		iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_MAIN_RECIPE:
		iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case RECIPE_FILE_IRSE_MAIN_RECIPE:
		iDescCount = sizeof(IrseMainRecipeDesc) / sizeof(IrseMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, IrseMainRecipeDesc, sizeof(IrseMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
#else
	case RECIPE_FILE_MS_MAIN_RECIPE:
		iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case RECIPE_FILE_4PP_MAIN_RECIPE:
		iDescCount = sizeof(ResistMainRecipeDesc) / sizeof(ResistMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistMainRecipeDesc, sizeof(ResistMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
	case RECIPE_FILE_CTA_MAIN_RECIPE:
		iDescCount = sizeof(ContactAngleMainRecipeDesc) / sizeof(ContactAngleMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleMainRecipeDesc, sizeof(ContactAngleMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(StressMainRecipeDesc) / sizeof(StressMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressMainRecipeDesc, sizeof(StressMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
	case RECIPE_FILE_MULTI_RECIPE:
		iDescCount = sizeof(MultiRecipeDesc) / sizeof(MultiRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MultiRecipeDesc, sizeof(MultiRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MULTI_RECIPE_DIR "%s" MULTIRECIPE_EXT, lpszName);
		break;
	case RECIPE_FILE_STAGE_PROGRAM:
		iDescCount = sizeof(StageProgramDesc) / sizeof(StageProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StageProgramDesc, sizeof(StageProgramDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, lpszName);
		break;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
	case RECIPE_FILE_STAGE_PROGRAM_STRESS:
		sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, lpszName);
		break;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */
	case RECIPE_FILE_RECALIBRATION_PROGRAM:
		iDescCount = sizeof(RecalibrationProgramDesc) / sizeof(RecalibrationProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, RecalibrationProgramDesc, sizeof(RecalibrationProgramDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_RECALIBRATION_PROGRAM_DIR "%s" RECALIBPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_POINT_DESKEW_PROGRAM:
		iDescCount = sizeof(PointDeskewProgramDesc) / sizeof(PointDeskewProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, PointDeskewProgramDesc, sizeof(PointDeskewProgramDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_POINT_DESKEW_PROGRAM_DIR "%s" POINTDESKEWPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_SR_THICKNESS:
		iDescCount = sizeof(SrThicknessDesc) / sizeof(SrThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrThicknessDesc, sizeof(SrThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_SR_REFLECTANCE:
		iDescCount = sizeof(SrReflectDesc) / sizeof(SrReflectDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectDesc, sizeof(SrReflectDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_SR_TRANSMITTANCE:
		iDescCount = sizeof(SrTransmitDesc) / sizeof(SrTransmitDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitDesc, sizeof(SrTransmitDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_SR_REFLECTANCE_CIE:
		iDescCount = sizeof(SrReflectCieDesc) / sizeof(SrReflectCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectCieDesc, sizeof(SrReflectCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_SR_TRANSMITTANCE_CIE:
		iDescCount = sizeof(SrTransmitCieDesc) / sizeof(SrTransmitCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitCieDesc, sizeof(SrTransmitCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	case RECIPE_FILE_SR_OPTICAL_DENSITY:
		iDescCount = sizeof(SrOdDesc) / sizeof(SrOdDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrOdDesc, sizeof(SrOdDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
// 2009.09.04 K.Matsuo -->
	case RECIPE_FILE_SE_THICKNESS:
		iDescCount = sizeof(SeThicknessDesc) / sizeof(SeThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeThicknessDesc, sizeof(SeThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_THICKNESS:
		iDescCount = sizeof(CompEASEThicknessDesc) / sizeof(CompEASEThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEThicknessDesc, sizeof(CompEASEThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	//2009.09.01 bagus stress --{--
	case RECIPE_FILE_STRESS:
		iDescCount = sizeof(StressDesc) / sizeof(StressDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressDesc, sizeof(StressDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	//2009.09.01 bagus stress --}--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_CTA:
		iDescCount = sizeof(ContactAngleDesc) / sizeof(ContactAngleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleDesc, sizeof(ContactAngleDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_SR_DISTANCE:
		iDescCount = sizeof(SrDistanceDesc) / sizeof(SrDistanceDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrDistanceDesc, sizeof(SrDistanceDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_MS:
		iDescCount = sizeof(MicroScopeDesc) / sizeof(MicroScopeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeDesc, sizeof(MicroScopeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	//2009.10.20 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	case RECIPE_FILE_4PP:
		iDescCount = sizeof(ResistDesc) / sizeof(ResistDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistDesc, sizeof(ResistDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		break;
	// 2009.11.04 bagus RS 追加 --}--
	// User Setting関連 (User Accountの設定)
	case RECIPE_FILE_USER_ACCOUNT:
		iDescCount = sizeof(UserAccountDesc) / sizeof(UserAccountDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, UserAccountDesc, sizeof(UserAccountDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_USER_ACCOUNT_DIR "%s" DAT_EXT, lpszName);
		break;
	// User Setting関連 (デフォルト値の設定)
	case RECIPE_FILE_DEF_SR_MAIN_RECIPE:
		iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SR_THICKNESS:
		iDescCount = sizeof(SrThicknessDesc) / sizeof(SrThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrThicknessDesc, sizeof(SrThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE:
		iDescCount = sizeof(SrReflectDesc) / sizeof(SrReflectDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectDesc, sizeof(SrReflectDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE:
		iDescCount = sizeof(SrTransmitDesc) / sizeof(SrTransmitDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitDesc, sizeof(SrTransmitDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE_CIE:
		iDescCount = sizeof(SrReflectCieDesc) / sizeof(SrReflectCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectCieDesc, sizeof(SrReflectCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE:
		iDescCount = sizeof(SrTransmitCieDesc) / sizeof(SrTransmitCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitCieDesc, sizeof(SrTransmitCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SR_OPTICAL_DENSITY:
		iDescCount = sizeof(SrOdDesc) / sizeof(SrOdDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrOdDesc, sizeof(SrOdDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2009.11.04 bagus RS 追加 --{--
	case RECIPE_FILE_DEF_4PP_MAIN_RECIPE:
		iDescCount = sizeof(ResistMainRecipeDesc) / sizeof(ResistMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistMainRecipeDesc, sizeof(ResistMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2009.11.04 bagus RS 追加 --}--
	case RECIPE_FILE_DEF_4PP:
		iDescCount = sizeof(ResistDesc) / sizeof(ResistDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistDesc, sizeof(ResistDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	//2009.08.25 bagus stress --{--
	case RECIPE_FILE_DEF_STRESS:
		iDescCount = sizeof(StressDesc) / sizeof(StressDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressDesc, sizeof(StressDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	//2009.08.25 bagus stress --}--
	// 2009.09.29 bagus Stress --{--
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(StressMainRecipeDesc) / sizeof(StressMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressMainRecipeDesc, sizeof(StressMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2009.09.29 bagus Stress --}--
	// 2009.09.29 bagus SE --{--
	case RECIPE_FILE_DEF_SE_MAIN_RECIPE:
		iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_SE_THICKNESS:
		iDescCount = sizeof(SeThicknessDesc) / sizeof(SeThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeThicknessDesc, sizeof(SeThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE:
		iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_COMPEASE_THICKNESS:
		iDescCount = sizeof(CompEASEThicknessDesc) / sizeof(CompEASEThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEThicknessDesc, sizeof(CompEASEThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_DEF_CTA_MAIN_RECIPE:
		iDescCount = sizeof(ContactAngleMainRecipeDesc) / sizeof(ContactAngleMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleMainRecipeDesc, sizeof(ContactAngleMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_CTA:
		iDescCount = sizeof(ContactAngleDesc) / sizeof(ContactAngleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleDesc, sizeof(ContactAngleDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_DEF_SR_DISTANCE:
		iDescCount = sizeof(SrDistanceDesc) / sizeof(SrDistanceDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrDistanceDesc, sizeof(SrDistanceDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_DEF_MS_MAIN_RECIPE:
		iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_MS:
		iDescCount = sizeof(MicroScopeDesc) / sizeof(MicroScopeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeDesc, sizeof(MicroScopeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	//2009.10.20 bagus MS 追加 --}--
	// System関連(Sampleの設定)
	case RECIPE_FILE_SAMPLE:
		iDescCount = sizeof(SampleDesc) / sizeof(SampleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SampleDesc, sizeof(SampleDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_SAMPLE_DIR "%s" DAT_EXT, lpszName);
		break;
	// System関連(ZAxisOffsetの設定)
	case RECIPE_FILE_Z_AXIS_OFFSET:
		switch(g_MainRcpInfo.MainRcpParam.hdr.wHeadType){
		case HEAD_TYPE_SR:
			iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
		case HEAD_TYPE_SE:
			iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			iDescCount = sizeof(IrseMainRecipeDesc) / sizeof(IrseMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, IrseMainRecipeDesc, sizeof(IrseMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
#else
		case HEAD_TYPE_MS:
			iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		default:
			return FALSE;
			break;
		}
		break;
	default:
		return FALSE;
		break;
	}
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */

	// ファイルが存在しない場合
	if(!FILEEXIST(szFilePath, &lastWriteSystemTime)){
		if(pDesc != NULL){
			delete [] pDesc;
			pDesc = NULL;
		}
		return FALSE;
	}
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
	if (RECIPE_FILE_STAGE_PROGRAM_STRESS == iType) {
		memset(&lg_StageProgStress, 0, sizeof(lg_StageProgStress));
		/* ストレスの LS 定義の読出し	*/
		lg_StageProgStress.dwNumLsScans = ::GetPrivateProfileInt(_T("StageProgram"), _T("NumLSScans"), 0, szFilePath);
/* modified 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
//		::GetPrivateProfileString(
//				_T("StageProgram"),
//				_T("MaterialElasticModulus"),
//				_T(""),
//				lg_StageProgStress.tszMateElastMod,
//				sizeof(lg_StageProgStress.tszMateElastMod),
//				szFilePath
//			);
/* modified 2009.07.31 hmenjo ストレス ライン数 定義３ ----------			   */
		::GetPrivateProfileString(
				_T("StageProgram"),
				_T("ElasticModulusName"),
				_T(""),
				lg_StageProgStress.tszElasticModulusName,
				sizeof(lg_StageProgStress.tszElasticModulusName),
				szFilePath
			);
		TCHAR l_tszTemp[64];
		::GetPrivateProfileString(
				_T("StageProgram"),
				_T("ElasticModulusValue"),
				_T("0.0"),
				l_tszTemp,
				sizeof(l_tszTemp),
				szFilePath
			);
		lg_StageProgStress.dElasticModulusValue = _tcstod(l_tszTemp, 0);
/* modified 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
		TCHAR l_tszSec[] = _T("LS");
		TCHAR l_tszKey[255];
/* deleted 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- { ---------- */
//		TCHAR l_tszTemp[64];
/* deleted 2009.07.31 hmenjo ストレス ライン数 定義３ ---------- } ---------- */
		/* ストレス設定を読出し：ライン数とライン毎セクション数	*/
		STRESS_CONFIG l_StressConfig;
		ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
		DWORD l_dwLineNum = l_StressConfig.dwLiftPinNumberOfLine;
		if (STRESS_LINES_MAX < l_dwLineNum) {
			l_dwLineNum = STRESS_LINES_MAX;
		}
		DWORD l_dwSectionNum[STRESS_LINES_MAX];
		memset(l_dwSectionNum, 0, sizeof(l_dwSectionNum));
  DWORD iLine;
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			l_dwSectionNum[iLine] = l_StressConfig.Line[iLine].dwSectionNum;
			if (STRESS_SECTIONS_MAX < l_dwSectionNum[iLine]) {
				l_dwSectionNum[iLine] = STRESS_SECTIONS_MAX;
			}
		}
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			/* ライン有効/無効を読出し	*/
			_stprintf(l_tszKey, LS_VALIDLINEn, iLine + 1);
			::GetPrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), l_tszTemp, sizeof(l_tszTemp), szFilePath);
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//			lg_StageProgStress.Line[iLine].LineSec.bValidLine = (0 == _tcscmp(l_tszTemp, _T("TRUE")))? TRUE : FALSE;
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
			lg_StageProgStress.Line[iLine].bValidLine = (0 == _tcscmp(l_tszTemp, _T("TRUE")))? TRUE : FALSE;
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
			for (DWORD iSec = 0; iSec < l_dwSectionNum[iLine]; iSec++) {
				/* セクション有効/無効を読出し	*/
				_stprintf(l_tszKey, LS_LnSnSCANVALID, iLine + 1, iSec + 1);
				::GetPrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), l_tszTemp, sizeof(l_tszTemp), szFilePath);
				lg_StageProgStress.Line[iLine].bScanValid[iSec] = (0 == _tcscmp(l_tszTemp, _T("TRUE")))? TRUE : FALSE;
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//				if ((0 != lg_StageProgStress.Line[iLine].LineSec.bValidLine)
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
				if ((0 != lg_StageProgStress.Line[iLine].bValidLine)
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
				 && (0 != lg_StageProgStress.Line[iLine].bScanValid[iSec])) {
					/* セクション位置を読出し	*/
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
//					lg_StageProgStress.Line[iLine].LineSec.SectPos[iSec].lScanStartPosX = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
//					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
//					lg_StageProgStress.Line[iLine].LineSec.SectPos[iSec].lScanEndPosX = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
//					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
//					lg_StageProgStress.Line[iLine].LineSec.SectPos[iSec].lScanPosY = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanStartPosX = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanEndPosX = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanPosY = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
				} else {
					/* ライン無効，あるいは，セクション無効	*/
					lg_StageProgStress.Line[iLine].bScanValid[iSec] = FALSE;
// 2009.09.09 bagus stress 修正 --{--
#if 0
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- { ---------- */
//					lg_StageProgStress.Line[iLine].LineSec.SectPos[iSec].lScanStartPosX = 0;
//					lg_StageProgStress.Line[iLine].LineSec.SectPos[iSec].lScanEndPosX = 0;
//					lg_StageProgStress.Line[iLine].LineSec.SectPos[iSec].lScanPosY = 0;
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ----------			  */
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanStartPosX = 0;
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanEndPosX = 0;
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanPosY = 0;
/* modified 2009.08.18 hmenjo STRESS_LINESECTION 削除 ---------- } ---------- */
#else
					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanStartPosX = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanEndPosX = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
					lg_StageProgStress.Line[iLine].SectPos[iSec].lScanPosY = ::GetPrivateProfileInt(l_tszSec, l_tszKey, 0, szFilePath);
#endif
// 2009.09.09 bagus stress 修正 --}--
				}
			}
		}
		memcpy(pVoid, &lg_StageProgStress, sizeof(STAGE_PROG_STRESS));
		return TRUE;
	}
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */

// 2009.11.04 bagus RS 追加 --{--
	if ((iType == RECIPE_FILE_4PP) || (iType == RECIPE_FILE_DEF_4PP)) {
		TCHAR l_tszTemp[64];

		::GetPrivateProfileString(
				_T("RS"),
				_T("RsStartRange"),
				_T(""),
				l_tszTemp,
				sizeof(l_tszTemp),
				szFilePath
			);
		g_MeasProgInfo.ScanParams._RS.lStartIdx = strtol(l_tszTemp, NULL, 10) + 3;
		if ((g_MeasProgInfo.ScanParams._RS.lStartIdx < 0) || (g_MeasProgInfo.ScanParams._RS.lStartIdx > 10)) {
			g_MeasProgInfo.ScanParams._RS.lStartIdx = 0;
		}

		::GetPrivateProfileString(
				_T("RS"),
				_T("RsThickUnit"),
				_T(""),
				l_tszTemp,
				sizeof(l_tszTemp),
				szFilePath
			);
		g_MeasProgInfo.ScanParams._RS.wUnits = 0;
		for (int nRSUnit = 0; nRSUnit < RESISTANCE_UNIT_MAX; nRSUnit++) {
			if (strcmp(l_tszTemp, RESISTANCE_UNIT_ITEM[nRSUnit]) == 0) {
				g_MeasProgInfo.ScanParams._RS.wUnits = nRSUnit;
				break;
			}
		}
	}
// 2009.11.04 bagus RS 追加 --}--

	// データの読み込み
	for(iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if(strlen((pDesc + iIndex)->section) > 0)
			section = (pDesc + iIndex)->section;

		// get file data
		memset(buff, 0, sizeof(buff));
		::GetPrivateProfileString(section,
								(pDesc + iIndex)->key,
								(pDesc + iIndex)->def,
								buff,
								sizeof(buff),
								szFilePath);

		// set ini file data to variable
		switch((pDesc + iIndex)->type){
		case 'b':
			*(BOOL*)(pDesc + iIndex)->data = (strcmp(buff, "TRUE") == 0)? TRUE : FALSE;
			break;
		case 'B':
			*(BYTE*)(pDesc + iIndex)->data = (BYTE)atoi(buff);
			break;
		case 'S':
			*(short*)(pDesc + iIndex)->data = (short)atoi(buff);
			break;
		case 'I':
			*(int*)(pDesc + iIndex)->data = (int)atoi(buff);
			break;
		case 'L':
			*(long*)(pDesc + iIndex)->data = (long)atol(buff);
			break;
		case 'F':
			*(float*)(pDesc + iIndex)->data = (float)atof(buff);
			break;
		case 'D':
			*(double*)(pDesc + iIndex)->data = (double)atof(buff);
			break;
		case 'W':
			*(WORD*)(pDesc + iIndex)->data = (WORD)atoi(buff);
			break;
		case 'C':
			strcpy_s((char*)(pDesc + iIndex)->data, 256, buff);
			break;
		default:
			(void*)(pDesc + iIndex)->data = NULL;
			break;
		}
	}

	if(pDesc != NULL){
		delete [] pDesc;
		pDesc = NULL;
	}

	// 読み込んだデータをコピー
	switch(iType){
	case RECIPE_FILE_SR_MAIN_RECIPE:
	case RECIPE_FILE_SE_MAIN_RECIPE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_MAIN_RECIPE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case RECIPE_FILE_IRSE_MAIN_RECIPE:
	case RECIPE_FILE_MS_MAIN_RECIPE:
// 2009.10.19 bagus MS 追加 --}--
	case RECIPE_FILE_4PP_MAIN_RECIPE:
	case RECIPE_FILE_CTA_MAIN_RECIPE:
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
	case RECIPE_FILE_DEF_SR_MAIN_RECIPE:
	// 2009.09.29 bagus Stress --{--
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
	// 2009.09.29 bagus Stress --}--
	// 2009.09.29 bagus SE --{--
	case RECIPE_FILE_DEF_SE_MAIN_RECIPE:
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE:
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_DEF_CTA_MAIN_RECIPE:
	// 2009.10.07 bagus CTA 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_DEF_MS_MAIN_RECIPE:
	//2009.10.20 bagus MS 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	case RECIPE_FILE_DEF_4PP_MAIN_RECIPE:
	// 2009.11.04 bagus RS 追加 --}--
	case RECIPE_FILE_Z_AXIS_OFFSET:
		memcpy(pVoid, &g_MainRcpInfo, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_MULTI_RECIPE:
		memcpy(pVoid, &g_MultiRcpInfoHdr, sizeof(MULTI_RCP_INFO_HDR));
		break;
	case RECIPE_FILE_STAGE_PROGRAM:
		memcpy(pVoid, &g_StageProgInfoHdr, sizeof(STAGE_PROG_INFO_HDR));
		break;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- { ---------- */
	case RECIPE_FILE_STAGE_PROGRAM_STRESS:
		sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, lpszName);
		break;
/* added 2009.07.22 hmenjo ストレス ステージ PGM 読出追加 ---------- } ---------- */
	case RECIPE_FILE_RECALIBRATION_PROGRAM:
		memcpy(pVoid, &g_RecalibProgInfo, sizeof(RECALIB_PROG_INFO));
		break;
	case RECIPE_FILE_POINT_DESKEW_PROGRAM:
		memcpy(pVoid, &g_PointDeskewProgInfo, sizeof(POINT_DESKEW_PROG_INFO));
		break;
	case RECIPE_FILE_SR_THICKNESS:
	case RECIPE_FILE_SR_REFLECTANCE:
	case RECIPE_FILE_SR_TRANSMITTANCE:
	case RECIPE_FILE_SR_REFLECTANCE_CIE:
	case RECIPE_FILE_SR_TRANSMITTANCE_CIE:
	case RECIPE_FILE_SR_OPTICAL_DENSITY:
// 2009.09.04 K.Matsuo -->
	case RECIPE_FILE_SE_THICKNESS:
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_THICKNESS:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case RECIPE_FILE_4PP:
	//2009.08.25 bagus stress --{--
	case RECIPE_FILE_STRESS:
	//2009.08.25 bagus stress --{--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_CTA:
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_SR_DISTANCE:
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_MS:
	//2009.10.20 bagus MS 追加 --}--
	case RECIPE_FILE_DEF_SR_THICKNESS:
	case RECIPE_FILE_DEF_SR_REFLECTANCE:
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE:
	case RECIPE_FILE_DEF_SR_REFLECTANCE_CIE:
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE:
	case RECIPE_FILE_DEF_SR_OPTICAL_DENSITY:
	case RECIPE_FILE_DEF_4PP:
	//2009.08.25 bagus stress --{--
	case RECIPE_FILE_DEF_STRESS:
	//2009.08.25 bagus stress --}--
	// 2009.09.29 bagus SE --{--
	case RECIPE_FILE_DEF_SE_THICKNESS:
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_DEF_COMPEASE_THICKNESS:
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_DEF_CTA:
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_DEF_SR_DISTANCE:
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_DEF_MS:
	//2009.10.20 bagus MS 追加 --}--
		memcpy(pVoid, &g_MeasProgInfo, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_SAMPLE:
		memcpy(pVoid, &g_SampleInfo, sizeof(SAMPLEINFO));
		break;
	case RECIPE_FILE_USER_ACCOUNT:
		memcpy(pVoid, &g_UserAccount, sizeof(USER_ACCOUNT));
		break;
//	case RECIPE_FILE_MEASUREMENT_DATA:
//		memcpy(pVoid, &g_MeasurementData, sizeof(MEASUREMENT_DATA));
//		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : SaveRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleの保存
// Parameters : pVoid	 ---> データを保存する構造体のポインタ
//				lpszName ---> ファイル名
//				iType	 ---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SaveRecipe(LPCVOID pVoid, LPCSTR lpszName, int iType)
{
	int iIndex, iDescCount;
	LPCSTR section;
	char buff[BUFFER_LEN + 1], szFilePath[_MAX_PATH];
	RECIPE_FILE* pDesc = NULL;
	SYSTEMTIME lastWriteSystemTime;

	ClearMemory();

	switch(iType){
	// Recipe Setting関連
	case RECIPE_FILE_SR_MAIN_RECIPE:
		iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_SE_MAIN_RECIPE:
		iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_MAIN_RECIPE:
		iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case RECIPE_FILE_IRSE_MAIN_RECIPE:
		iDescCount = sizeof(IrseMainRecipeDesc) / sizeof(IrseMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, IrseMainRecipeDesc, sizeof(IrseMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
#else
	case RECIPE_FILE_MS_MAIN_RECIPE:
		iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case RECIPE_FILE_4PP_MAIN_RECIPE:
		iDescCount = sizeof(ResistMainRecipeDesc) / sizeof(ResistMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistMainRecipeDesc, sizeof(ResistMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_CTA_MAIN_RECIPE:
		iDescCount = sizeof(ContactAngleMainRecipeDesc) / sizeof(ContactAngleMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleMainRecipeDesc, sizeof(ContactAngleMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(StressMainRecipeDesc) / sizeof(StressMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressMainRecipeDesc, sizeof(StressMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_MULTI_RECIPE:
		iDescCount = sizeof(MultiRecipeDesc) / sizeof(MultiRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MultiRecipeDesc, sizeof(MultiRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MULTI_RECIPE_DIR "%s" MULTIRECIPE_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MultiRcpInfoHdr, pVoid, sizeof(MULTI_RCP_INFO_HDR));
		break;
	case RECIPE_FILE_STAGE_PROGRAM:
		iDescCount = sizeof(StageProgramDesc) / sizeof(StageProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StageProgramDesc, sizeof(StageProgramDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_StageProgInfoHdr, pVoid, sizeof(STAGE_PROG_INFO_HDR));
		break;
//2009.08.31 bagus stress --{--
	case RECIPE_FILE_STAGE_PROGRAM_STRESS:
		sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, lpszName);
		break;
//2009.08.31 bagus stress --}--
	case RECIPE_FILE_RECALIBRATION_PROGRAM:
		iDescCount = sizeof(RecalibrationProgramDesc) / sizeof(RecalibrationProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, RecalibrationProgramDesc, sizeof(RecalibrationProgramDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_RECALIBRATION_PROGRAM_DIR "%s" RECALIBPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_RecalibProgInfo, pVoid, sizeof(RECALIB_PROG_INFO));
		break;
	case RECIPE_FILE_POINT_DESKEW_PROGRAM:
		iDescCount = sizeof(PointDeskewProgramDesc) / sizeof(PointDeskewProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, PointDeskewProgramDesc, sizeof(PointDeskewProgramDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_POINT_DESKEW_PROGRAM_DIR "%s" POINTDESKEWPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_PointDeskewProgInfo, pVoid, sizeof(POINT_DESKEW_PROG_INFO));
		break;
	case RECIPE_FILE_SR_THICKNESS:
		iDescCount = sizeof(SrThicknessDesc) / sizeof(SrThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrThicknessDesc, sizeof(SrThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_SR_REFLECTANCE:
		iDescCount = sizeof(SrReflectDesc) / sizeof(SrReflectDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectDesc, sizeof(SrReflectDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_SR_TRANSMITTANCE:
		iDescCount = sizeof(SrTransmitDesc) / sizeof(SrTransmitDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitDesc, sizeof(SrTransmitDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_SR_REFLECTANCE_CIE:
		iDescCount = sizeof(SrReflectCieDesc) / sizeof(SrReflectCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectCieDesc, sizeof(SrReflectCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_SR_TRANSMITTANCE_CIE:
		iDescCount = sizeof(SrTransmitCieDesc) / sizeof(SrTransmitCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitCieDesc, sizeof(SrTransmitCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_SR_OPTICAL_DENSITY:
		iDescCount = sizeof(SrOdDesc) / sizeof(SrOdDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrOdDesc, sizeof(SrOdDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
// 2009.09.04 K.Matsuo -->
	case RECIPE_FILE_SE_THICKNESS:
		iDescCount = sizeof(SeThicknessDesc) / sizeof(SeThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeThicknessDesc, sizeof(SeThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
// 2009.09.04 K.Matsuo <--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_THICKNESS:
		iDescCount = sizeof(CompEASEThicknessDesc) / sizeof(CompEASEThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEThicknessDesc, sizeof(CompEASEThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case RECIPE_FILE_4PP:
		iDescCount = sizeof(ResistDesc) / sizeof(ResistDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistDesc, sizeof(ResistDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_STRESS:
		iDescCount = sizeof(StressDesc) / sizeof(StressDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressDesc, sizeof(StressDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_CTA:
		iDescCount = sizeof(ContactAngleDesc) / sizeof(ContactAngleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleDesc, sizeof(ContactAngleDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_SR_DISTANCE:
		iDescCount = sizeof(SrDistanceDesc) / sizeof(SrDistanceDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrDistanceDesc, sizeof(SrDistanceDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_MS:
		iDescCount = sizeof(MicroScopeDesc) / sizeof(MicroScopeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeDesc, sizeof(MicroScopeDesc));
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	//2009.10.20 bagus MS 追加 --}--
	// User Setting関連(UserAccountの設定)
	case RECIPE_FILE_USER_ACCOUNT:
		iDescCount = sizeof(UserAccountDesc) / sizeof(UserAccountDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, UserAccountDesc, sizeof(UserAccountDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_USER_ACCOUNT_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_UserAccount, pVoid, sizeof(USER_ACCOUNT));
		break;
	// User Setting関連(デフォルト値の設定)
	case RECIPE_FILE_DEF_SR_MAIN_RECIPE:
		iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_DEF_SR_THICKNESS:
		iDescCount = sizeof(SrThicknessDesc) / sizeof(SrThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrThicknessDesc, sizeof(SrThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE:
		iDescCount = sizeof(SrReflectDesc) / sizeof(SrReflectDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectDesc, sizeof(SrReflectDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE:
		iDescCount = sizeof(SrTransmitDesc) / sizeof(SrTransmitDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitDesc, sizeof(SrTransmitDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE_CIE:
		iDescCount = sizeof(SrReflectCieDesc) / sizeof(SrReflectCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectCieDesc, sizeof(SrReflectCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE:
		iDescCount = sizeof(SrTransmitCieDesc) / sizeof(SrTransmitCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitCieDesc, sizeof(SrTransmitCieDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_DEF_SR_OPTICAL_DENSITY:
		iDescCount = sizeof(SrOdDesc) / sizeof(SrOdDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrOdDesc, sizeof(SrOdDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.11.04 bagus RS 追加 --{--
	case RECIPE_FILE_DEF_4PP_MAIN_RECIPE:
		iDescCount = sizeof(ResistMainRecipeDesc) / sizeof(ResistMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistMainRecipeDesc, sizeof(ResistMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	// 2009.11.04 bagus RS 追加 --}--
	case RECIPE_FILE_DEF_4PP:
		iDescCount = sizeof(ResistDesc) / sizeof(ResistDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistDesc, sizeof(ResistDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	case RECIPE_FILE_DEF_STRESS:
		iDescCount = sizeof(StressDesc) / sizeof(StressDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressDesc, sizeof(StressDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.09.29 bagus Stress --{--
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(StressMainRecipeDesc) / sizeof(StressMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressMainRecipeDesc, sizeof(StressMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	// 2009.09.29 bagus Stress --}--
	// 2009.09.29 bagus SE --{--
	case RECIPE_FILE_DEF_SE_MAIN_RECIPE:
		iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_DEF_SE_THICKNESS:
		iDescCount = sizeof(SeThicknessDesc) / sizeof(SeThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeThicknessDesc, sizeof(SeThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE:
		iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_DEF_COMPEASE_THICKNESS:
		iDescCount = sizeof(CompEASEThicknessDesc) / sizeof(CompEASEThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEThicknessDesc, sizeof(CompEASEThicknessDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_DEF_CTA_MAIN_RECIPE:
		iDescCount = sizeof(ContactAngleMainRecipeDesc) / sizeof(ContactAngleMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleMainRecipeDesc, sizeof(ContactAngleMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_DEF_CTA:
		iDescCount = sizeof(ContactAngleDesc) / sizeof(ContactAngleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleDesc, sizeof(ContactAngleDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_DEF_SR_DISTANCE:
		iDescCount = sizeof(SrDistanceDesc) / sizeof(SrDistanceDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrDistanceDesc, sizeof(SrDistanceDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_DEF_MS_MAIN_RECIPE:
		iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		break;
	case RECIPE_FILE_DEF_MS:
		iDescCount = sizeof(MicroScopeDesc) / sizeof(MicroScopeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeDesc, sizeof(MicroScopeDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_MeasProgInfo, pVoid, sizeof(MEAS_PROG_INFO));
		break;
	//2009.10.20 bagus MS 追加 --}--
	// System関連(Sampleの設定)
	case RECIPE_FILE_SAMPLE:
		iDescCount = sizeof(SampleDesc) / sizeof(SampleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SampleDesc, sizeof(SampleDesc));
		sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_SAMPLE_DIR "%s" DAT_EXT, lpszName);
		// 保存内容をコピー
		memcpy(&g_SampleInfo, pVoid, sizeof(SAMPLEINFO));
		break;
	// System関連(ZAxisOffsetの設定)
	case RECIPE_FILE_Z_AXIS_OFFSET:
		// 保存内容をコピー
		memcpy(&g_MainRcpInfo, pVoid, sizeof(MAIN_RCP_INFO));
		switch(g_MainRcpInfo.MainRcpParam.hdr.wHeadType){
		case HEAD_TYPE_SR:
			iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
		case HEAD_TYPE_SE:
			iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			iDescCount = sizeof(IrseMainRecipeDesc) / sizeof(IrseMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, IrseMainRecipeDesc, sizeof(IrseMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
#else
		case HEAD_TYPE_MS:
			iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
			sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, lpszName);
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		default:
			return FALSE;
			break;
		}
		break;
	default:
		return FALSE;
		break;
	}
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */


//2009.08.31 bagus stress --{--
	if (RECIPE_FILE_STAGE_PROGRAM_STRESS == iType) {
		STAGE_PROG_STRESS *l_pStageProgStress = (STAGE_PROG_STRESS *)pVoid;
		/* ストレスの LS 定義の読出し	*/
		::WritePrivateProfileString(
				_T("StageProgram"),
				_T("ElasticModulusName"),
				l_pStageProgStress->tszElasticModulusName,
				szFilePath
			);
		TCHAR l_tszTemp[64];
// 2009.11.09 bagus Stress 追加変更 --{--
//		_stprintf_s(l_tszTemp, _countof(l_tszTemp), "%.3f", l_pStageProgStress->dElasticModulusValue);
		_stprintf_s(l_tszTemp, _countof(l_tszTemp), "%e", l_pStageProgStress->dElasticModulusValue);
// 2009.11.09 bagus Stress 追加変更 --}--
		::WritePrivateProfileString(
				_T("StageProgram"),
				_T("ElasticModulusValue"),
				l_tszTemp,
				szFilePath
			);
// 2009.09.08 bagus stress 修正 --{--
		_stprintf_s(l_tszTemp, _countof(l_tszTemp), "%ld", l_pStageProgStress->dwNumLsScans);
		::WritePrivateProfileString(
				_T("StageProgram"),
				_T("NumLSScans"),
				l_tszTemp,
				szFilePath
			);
// 2009.09.08 bagus stress 修正 --}--
//		lg_StageProgStress.dElasticModulusValue = _tcstod(l_tszTemp, 0);
		TCHAR l_tszSec[] = _T("LS");
		TCHAR l_tszKey[255];
		/* ストレス設定を読出し：ライン数とライン毎セクション数	*/
		STRESS_CONFIG l_StressConfig;
		ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
		DWORD l_dwLineNum = l_StressConfig.dwLiftPinNumberOfLine;
		if (STRESS_LINES_MAX < l_dwLineNum) {
			l_dwLineNum = STRESS_LINES_MAX;
		}
		DWORD l_dwSectionNum[STRESS_LINES_MAX];
		memset(l_dwSectionNum, 0, sizeof(l_dwSectionNum));
  DWORD iLine;
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			l_dwSectionNum[iLine] = l_StressConfig.Line[iLine].dwSectionNum;
			if (STRESS_SECTIONS_MAX < l_dwSectionNum[iLine]) {
				l_dwSectionNum[iLine] = STRESS_SECTIONS_MAX;
			}
		}
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			/* ライン有効/無効を読出し	*/
			_stprintf(l_tszKey, LS_VALIDLINEn, iLine + 1);
			_stprintf(l_tszTemp,_T("%s"),l_pStageProgStress->Line[iLine].bValidLine ? _T("TRUE") : _T("FALSE"));
			::WritePrivateProfileString(l_tszSec, l_tszKey, l_tszTemp, szFilePath);
//2009.09.24 bagus stress --{--
// LineのValidがFALSEの場合にはセクションの情報を書き込まない
			for (DWORD iSec = 0; iSec < l_dwSectionNum[iLine]; iSec++) {
//			for (DWORD iSec = 0; iSec < l_dwSectionNum[iLine] && l_pStageProgStress->Line[iLine].bValidLine ; iSec++) {
//2009.09.24 bagus stress --}--
				/* セクション有効/無効を読出し	*/
				_stprintf(l_tszKey, LS_LnSnSCANVALID, iLine + 1, iSec + 1);
// 2009.09.10 bagus stress 修正 --{--
//				if ((0 != l_pStageProgStress->Line[iLine].bValidLine)
//				 && (0 != l_pStageProgStress->Line[iLine].bScanValid[iSec])) {
				if (0 != l_pStageProgStress->Line[iLine].bValidLine) {
				 	if (0 != l_pStageProgStress->Line[iLine].bScanValid[iSec]) {
					WritePrivateProfileString(l_tszSec, l_tszKey, _T("TRUE"), szFilePath);
					} else {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), szFilePath);
						//2009.09.24 bagus stress --{--
						//Valid=FALSEの場合は書き込まない
						continue;
						//2009.09.24 bagus stress --}--
					}
// 2009.09.10 bagus stress 修正 --}--
					/* セクション位置を読出し	*/
					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanStartPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanEndPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanPosY );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath);
				} else {
// 2009.09.09 bagus stress 修正 --{--
#if 0
					WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), szFilePath);
					WritePrivateProfileString(l_tszSec, l_tszKey,_T("0"),szFilePath);
					WritePrivateProfileString(l_tszSec, l_tszKey,_T("0"),szFilePath);
					WritePrivateProfileString(l_tszSec, l_tszKey,_T("0"),szFilePath);
#else
// 2009.09.10 bagus stress 修正 --{--
//					WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), szFilePath);
				 	if (0 != l_pStageProgStress->Line[iLine].bScanValid[iSec]) {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("TRUE"), szFilePath);
					} else {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), szFilePath);
						//2009.09.24 bagus stress --{--
						//Valid=FALSEの場合は書き込まない
						continue;
						//2009.09.24 bagus stress --}--
					}
// 2009.09.10 bagus stress 修正 --}--
					/* セクション位置を読出し	*/
					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanStartPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanEndPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanPosY );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath);
#endif
// 2009.09.09 bagus stress 修正 --}--
				}
			}
		}
		return TRUE;
	}
//2009.08.31 bagus stress --}--



	// ファイルが存在する場合，いったん削除する(ただしデフォルト値設定の場合には削除しない)
//2009.10.20 bagus MS 追加 --{--
#if 0
	if(FILEEXIST(szFilePath, &lastWriteSystemTime)		&&
		(iType != RECIPE_FILE_DEF_SR_MAIN_RECIPE)		&&
		(iType != RECIPE_FILE_DEF_SR_THICKNESS)			&&
		(iType != RECIPE_FILE_DEF_SR_REFLECTANCE)		&&
		(iType != RECIPE_FILE_DEF_SR_TRANSMITTANCE)		&&
		(iType != RECIPE_FILE_DEF_SR_REFLECTANCE_CIE)	&&
		(iType != RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE)	&&
		(iType != RECIPE_FILE_DEF_SR_OPTICAL_DENSITY)	&&
		(iType != RECIPE_FILE_DEF_4PP)					&&
		// 2009.09.29 bagus Stress --{--
		//2009.08.25 bagus stress --{--
		//(iType != RECIPE_FILE_DEF_STRESS)){
		//2009.08.25 bagus stress --}--
		(iType != RECIPE_FILE_DEF_STRESS)				&&
		(iType != RECIPE_FILE_DEF_STRESS_MAIN_RECIPE)	&&
		// 2009.09.29 bagus Stress --}--
		// 2009.09.29 bagus SE --{--
		(iType != RECIPE_FILE_DEF_SE_THICKNESS)			&&
		// 2009.09.29 bagus SE --}--
		// 2009.10.07 bagus CTA 追加 --{--
//		(iType != RECIPE_FILE_DEF_SE_MAIN_RECIPE)){
		(iType != RECIPE_FILE_DEF_SE_MAIN_RECIPE)		&&
		(iType != RECIPE_FILE_DEF_CTA)					&&
		// 2009.10.14 bagus Distance 追加 --{--
//		(iType != RECIPE_FILE_DEF_CTA_MAIN_RECIPE)){
		(iType != RECIPE_FILE_DEF_CTA_MAIN_RECIPE)		&&
		(iType != RECIPE_FILE_DEF_SR_DISTANCE)){
		// 2009.10.14 bagus Distance 追加 --}--
		// 2009.10.07 bagus CTA 追加 --}--
		::DeleteFile(szFilePath);
#else
	if (FILEEXIST(szFilePath, &lastWriteSystemTime)) {
		switch ( iType ) {
		case RECIPE_FILE_DEF_SR_MAIN_RECIPE:
		case RECIPE_FILE_DEF_SR_THICKNESS:
		case RECIPE_FILE_DEF_SR_REFLECTANCE:
		case RECIPE_FILE_DEF_SR_TRANSMITTANCE:
		case RECIPE_FILE_DEF_SR_REFLECTANCE_CIE:
		case RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE:
		case RECIPE_FILE_DEF_SR_OPTICAL_DENSITY:
		case RECIPE_FILE_DEF_4PP:
		// 2009.11.04 bagus RS 追加 --{--
		case RECIPE_FILE_DEF_4PP_MAIN_RECIPE:
		// 2009.11.04 bagus RS 追加 --}--
		case RECIPE_FILE_DEF_STRESS:
		case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		case RECIPE_FILE_DEF_SE_THICKNESS:
		case RECIPE_FILE_DEF_SE_MAIN_RECIPE:
		// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case RECIPE_FILE_DEF_COMPEASE_THICKNESS:
		case RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE:
		// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		case RECIPE_FILE_DEF_CTA:
		case RECIPE_FILE_DEF_CTA_MAIN_RECIPE:
		case RECIPE_FILE_DEF_SR_DISTANCE:
		case RECIPE_FILE_DEF_MS:
		case RECIPE_FILE_DEF_MS_MAIN_RECIPE:
			break;
		default:
			::DeleteFile(szFilePath);
			break;
		}
#endif
//2009.10.20 bagus MS 追加 --}--
	}

// 2009.11.04 bagus RS 追加 --{--
	if ((iType == RECIPE_FILE_4PP) || (iType == RECIPE_FILE_DEF_4PP)) {
		TCHAR l_tszTemp[64];

		_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%+d"), g_MeasProgInfo.ScanParams._RS.lStartIdx - 3 );
		WritePrivateProfileString(_T("RS"), _T("RsStartRange"), l_tszTemp, szFilePath);

		if ((g_MeasProgInfo.ScanParams._RS.wUnits < 0) || (g_MeasProgInfo.ScanParams._RS.wUnits >= RESISTANCE_UNIT_MAX)) {
			strcpy_s(l_tszTemp, sizeof(l_tszTemp), "");
		} else {
			strcpy_s(l_tszTemp, sizeof(l_tszTemp), RESISTANCE_UNIT_ITEM[g_MeasProgInfo.ScanParams._RS.wUnits]);
		}
		WritePrivateProfileString(_T("RS"), _T("RsThickUnit"), l_tszTemp, szFilePath);
	}
// 2009.11.04 bagus RS 追加 --}--

	// Name，HeadType，ScanTypeを書き込む
	if(!SaveHeadTypeAndScanType(iType, lpszName)){
		if(pDesc != NULL){
			delete [] pDesc;
			pDesc = NULL;
		}
	}

	for(iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if (strlen((pDesc + iIndex)->section) > 0)
			section = (pDesc + iIndex)->section;

		// set file data
		memset(buff, 0, sizeof(buff));
		switch((pDesc + iIndex)->type){
		case 'b':
			strcpy(buff, (*(BOOL*)(pDesc + iIndex)->data)? "TRUE" : "FALSE");
			break;
		case 'B':
			_itoa(*(BYTE*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'S':
			_itoa(*(short*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'I':
			_itoa(*(int*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'L':
			_ltoa(*(long*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'F':
			sprintf(buff, "%.3f", *(float*)(pDesc + iIndex)->data);
			break;
		case 'D':
			sprintf(buff, "%.3lf", *(double*)(pDesc + iIndex)->data);
			break;
		case 'W':
			sprintf(buff, "%d", *(WORD*)(pDesc + iIndex)->data);
			break;
		case 'C':
			strncpy(buff, (char*)(pDesc + iIndex)->data, BUFFER_LEN);
			break;
		default:
			break;
		}

		// write NanoSpec ini file
		::WritePrivateProfileString(section,
									(pDesc + iIndex)->key,
									buff,
									szFilePath);
	}

	if(pDesc != NULL){
		delete [] pDesc;
		pDesc = NULL;
	}

	//2009.12.10 bagus Recipe Backup --{--
	BackupRecipe(szFilePath);
	//2009.12.10 bagus Recipe Backup --}--

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadMainRecipeList
// Purpose	  : MultiRecipeのMainRecipeListの読み込み
// Parameters : pMainRcpList   ---> MainRecipeListの配列
//				wNumMainRecipe ---> 読み込むMainRecipe数
//				lpszName	   ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadMainRecipeList(MULTI_RCP_MAIN_RCP_LIST* pMainRcpList,
	WORD wNumMainRecipe, LPCSTR lpszName)
{
	SYSTEMTIME lastWriteSystemTime;

	// ファイルが存在しない場合
	if(!ExistRecipe(lpszName, &lastWriteSystemTime, RECIPE_FILE_MULTI_RECIPE)){
		return FALSE;
	}

	CString strBuff;
	BOOL bFind = FALSE;

	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, DB_MULTI_RECIPE_DIR "%s" MULTIRECIPE_EXT, lpszName);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */

	TRY
	{
		CStdioFile file(szFilePath, CFile::modeRead);
		// セクションの検索
		while(file.ReadString(strBuff)){
			strBuff.Replace('\r', '\0');
			strBuff.Replace('\n', '\0');
			if ( strBuff == g_lpszMultiRcpMainRcpListSection )
			{
				bFind = TRUE;
				break;
			}
		}
		if(!bFind){
			file.Close();
			return FALSE;
		}
		for(int i = 0; i < (int)wNumMainRecipe; i++){
			if(!file.ReadString(strBuff)){
				file.Close();
				return FALSE;
			}
			strBuff.Replace('\r', '\0');
			strBuff.Replace('\n', '\0');
			strcpy((pMainRcpList + i)->szName, strBuff);
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
// Name 	  : SaveMainRecipeList
// Purpose	  : MultiRecipeのMainRecipeListの保存
// Parameters : pMainRcpList   ---> MainRecipeListの配列
//				wNumMainRecipe ---> 保存するMainRecipe数
//				lpszName	   ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SaveMainRecipeList(const MULTI_RCP_MAIN_RCP_LIST* pMainRcpList,
	WORD wNumMainRecipe, LPCSTR lpszName)
{
	char szFilePath[_MAX_PATH], Buff[256];
	sprintf_s(szFilePath, _MAX_PATH, DB_MULTI_RECIPE_DIR "%s" MULTIRECIPE_EXT, lpszName);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */

	TRY
	{
		CStdioFile file(szFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
		file.SeekToEnd();
		// セクション名を書き込む
		sprintf_s(Buff, sizeof(Buff), "%s\n", g_lpszMultiRcpMainRcpListSection);
		file.WriteString(Buff);

		// MainRecipeの名前を書き込む
		for ( int i = 0; i < (int)wNumMainRecipe; i++ )
		{
			sprintf(Buff, "%s\n", (pMainRcpList + i)->szName);
			file.WriteString(Buff);
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
// Name 	  : LoadPointList
// Purpose	  : StageProgramの測定ポイントリストの読み込み
// Parameters : pPoint		---> STAGE_COORDの配列
//				wNumScans	---> 読み込む測定ポイント数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadPointList(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, pszFileName);		// ステージPGM
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
	return LoadPointList2(pPoint, wNumScans, szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadPointList2
// Purpose	  : StageProgramの測定ポイントリストの読み込み
// Parameters : pPoint	  ---> STAGE_COORDの配列
//				wNumScans ---> 読み込む測定ポイント数
//				pszFilePath ---> ファイルパス名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadPointList2(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath)
{
	CString strBuff;
	BOOL bFind = FALSE;
	int i, iColonIndex, iCommaIndex;
	SYSTEMTIME lastWriteSystemTime;

	// ファイルが存在しない場合
	if ( !FILEEXIST(pszFilePath, &lastWriteSystemTime) )
		return FALSE;

	TRY
	{
		CStdioFile file(pszFilePath, CFile::modeRead);
		while(file.ReadString(strBuff)){
			strBuff.Replace('\r', '\0');
			strBuff.Replace('\n', '\0');
			if ( strBuff == g_lpszStageProgPointListSection )
			{
				bFind = TRUE;
				break;
			}
		}

		if(!bFind){
			file.Close();
			return FALSE;
		}

		for(i = 0; i < (int)wNumScans; i++){
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
			iColonIndex = strBuff.Find(':');
			iCommaIndex = strBuff.Find(',');
			(pPoint + i)->lX = atol(strBuff.Mid(iColonIndex + 1, iCommaIndex - (iColonIndex + 1)));
			(pPoint + i)->lY = atol(strBuff.Mid(iCommaIndex + 1, strlen(strBuff) - (iCommaIndex + 1)));
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
// Purpose	  : StageProgramの測定ポイントリストの保存
// Parameters : pPoint		---> STAGE_COORDの配列
//				wNumScans	---> 保存する測定ポイント数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SavePointList(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, pszFileName);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
	return SavePointList2(pPoint, wNumScans, szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : SavePointList2
// Purpose	  : StageProgramの測定ポイントリストの保存
// Parameters : pPoint		---> STAGE_COORDの配列
//				wNumScans	---> 保存する測定ポイント数
//				pszFilePath ---> ファイルパス名
//				bDoBackup	---> バックアップ処理を実行するかどうか
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SavePointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath,BOOL bDoBackup /* = TRUE */)
{
	char Buff[256];
	TRY
	{
		CStdioFile file(pszFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
		file.SeekToEnd();
		// セクション名を書き込む
		sprintf_s(Buff, sizeof(Buff), "%s\n", g_lpszStageProgPointListSection);
		file.WriteString(Buff);

		// ポイントを書き込む
		for(int i = 0; i < (int)wNumScans; i++){
			sprintf(Buff, "%d:%ld,%ld\n", i + 1, (pPoint + i)->lX, (pPoint + i)->lY);
			file.WriteString(Buff);
		}
		file.WriteString("\n");
		file.Close();
		//return TRUE;
	}
	CATCH( CFileException, e )
	{
		return FALSE;
	}
	END_CATCH

	//2010.03.09 15.24 Recicpe Backup 修正 --{--
	if(bDoBackup){
		//2009.12.10 bagus Recipe Backup --{--
		BackupRecipe(pszFilePath);
		//2009.12.10 bagus Recipe Backup --}--
	}
	//2010.03.09 15.24 Recicpe Backup 修正 --}--

	return TRUE;
}

// 2009.10.15 bagus Distance 追加 --{--
BOOL Load2PointList(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, pszFileName);		// ステージPGM
	AddAbsPath(szFilePath);
	return Load2PointList2(pPoint, wNumScans, szFilePath);
}

BOOL Load2PointList2(STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath)
{
	CString strBuff;
	BOOL bFind = FALSE;
	int i, iColonIndex, iComma1Index, iComma2Index, iComma3Index;
	SYSTEMTIME lastWriteSystemTime;

	int	nIndex;

	// ファイルが存在しない場合
	if ( !FILEEXIST(pszFilePath, &lastWriteSystemTime) )
		return FALSE;

	TRY
	{
		CStdioFile file(pszFilePath, CFile::modeRead);
		while(file.ReadString(strBuff)){
			strBuff.Replace('\r', '\0');
			strBuff.Replace('\n', '\0');
			if ( strBuff == g_lpszStageProgPointListSection )
			{
				bFind = TRUE;
				break;
			}
		}

		if(!bFind){
			file.Close();
			return FALSE;
		}

		nIndex = 0;
		for (i=0; i<(int)wNumScans; i++) {
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
			iColonIndex = strBuff.Find(':');
			iComma1Index = strBuff.Find(',');
			iComma2Index = strBuff.Find(',', iComma1Index + 1);
			iComma3Index = strBuff.Find(',', iComma2Index + 1);
			(pPoint + nIndex)->lX = atol(strBuff.Mid(iColonIndex + 1, iComma1Index - (iColonIndex + 1)));
			(pPoint + nIndex)->lY = atol(strBuff.Mid(iComma1Index + 1, iComma2Index - (iComma1Index + 1)));
			nIndex++;
			(pPoint + nIndex)->lX = atol(strBuff.Mid(iComma2Index + 1, iComma3Index - (iComma2Index + 1)));
			(pPoint + nIndex)->lY = atol(strBuff.Mid(iComma3Index + 1, strlen(strBuff) - (iComma3Index + 1)));
			nIndex++;
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

BOOL Save2PointList(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, pszFileName);
	AddAbsPath(szFilePath);
	return Save2PointList2(pPoint, wNumScans, szFilePath);
}


// 2010.03.09 bagus Recipe Backup 修正 --{--
//BOOL Save2PointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath)
BOOL Save2PointList2(const STAGE_COORD* pPoint, WORD wNumScans, LPCSTR pszFilePath,BOOL bDoBackup /*= TRUE */)
// 2010.03.09 bagus Recipe Backup 修正 --}--
{
	char Buff[256];
	int	nIndex;

	TRY
	{
		CStdioFile file(pszFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
		file.SeekToEnd();
		// セクション名を書き込む
		sprintf_s(Buff, sizeof(Buff), "%s\n", g_lpszStageProgPointListSection);
		file.WriteString(Buff);

		// ポイントを書き込む
		nIndex = 0;
		for(int i = 0; i < (int)wNumScans; i++){
			sprintf(Buff, "%d:%ld,%ld,%ld,%ld\n", i + 1, (pPoint + nIndex)->lX, (pPoint + nIndex)->lY, (pPoint + nIndex + 1)->lX, (pPoint + nIndex + 1)->lY);
			file.WriteString(Buff);
			nIndex += 2;
		}
		file.WriteString("\n");
		file.Close();
		//return TRUE;
	}
	CATCH( CFileException, e )
	{
		return FALSE;
	}
	END_CATCH

	// 2010.03.09 bagus Recipe Backup 修正 --}--
	if(bDoBackup){
		//2009.12.10 bagus Recipe Backup --{--
		BackupRecipe(pszFilePath);
		//2009.12.10 bagus Recipe Backup --}--
	}
	// 2010.03.09 bagus Recipe Backup 修正 --}--
	return TRUE;
}
// 2009.10.15 bagus Distance 追加 --}--

/////////////////////////////////////////////////////////////////////////////
// Name 	  : MakeRecipeFilePath
// Purpose	  : ファイル名とレシピタイプからファイルパス名を生成
// Parameters : pszFilePath ---> ファイルパス名
//				pszFileName ---> ファイル名
//				iType		---> どのファイル(MainReicpe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 成功
//				FALSE ---> 失敗
BOOL MakeRecipeFilePath(LPSTR pszFilePath, LPCSTR pszFileName, int iType)
{
	switch(iType)
	{
	case RECIPE_FILE_MAIN_RECIPE:
		sprintf(pszFilePath, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, pszFileName);
		break;
	case RECIPE_FILE_MULTI_RECIPE:
		sprintf(pszFilePath, DB_MULTI_RECIPE_DIR "%s" MULTIRECIPE_EXT, pszFileName);
		break;
	case RECIPE_FILE_MEASUREMENT_PROGRAM:
		sprintf(pszFilePath, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, pszFileName);
		break;
	case RECIPE_FILE_STAGE_PROGRAM:
		sprintf(pszFilePath, DB_STAGE_PROGRAM_DIR "%s" STAGEPGM_EXT, pszFileName);
		break;
	case RECIPE_FILE_RECALIBRATION_PROGRAM:
		sprintf(pszFilePath, DB_RECALIBRATION_PROGRAM_DIR "%s" RECALIBPGM_EXT, pszFileName);
		break;
	case RECIPE_FILE_POINT_DESKEW_PROGRAM:
		sprintf(pszFilePath, DB_POINT_DESKEW_PROGRAM_DIR "%s" POINTDESKEWPGM_EXT, pszFileName);
		break;
	case RECIPE_FILE_SAMPLE:
		sprintf(pszFilePath, CFG_SYSTEM_SAMPLE_DIR "%s" DAT_EXT, pszFileName);
		break;
	case RECIPE_FILE_USER_ACCOUNT:
		sprintf(pszFilePath, CFG_USER_USER_ACCOUNT_DIR "%s" DAT_EXT, pszFileName);
		break;
	default:
		return FALSE;
		break;
	}
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(pszFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : DeleteRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleの削除
// Parameters : pszFileName ---> ファイル名
//				iType		---> どのファイル(MainReicpe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 削除成功
//				FALSE ---> 削除失敗
BOOL DeleteRecipe(LPCSTR pszFileName, int iType)
{
	char szFilePath[_MAX_PATH];

	if ( !MakeRecipeFilePath(szFilePath, pszFileName, iType) )
		return FALSE;

	//2009.12.25 bagus Recipe Backup --{--
	DeleteBackupRecipe(szFilePath);
	//2009.12.25 bagus Recipe Backup --}--
	return ::DeleteFile(szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : ExistRecipe
// Purpose	  : Recipe，Program，UserAccount，Sampleが存在するかを確認
// Parameters : pszFileName ---> ファイル名
//				lpLastWriteSystemTime ---> ファイルの最終更新日時
//				iType		---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 存在する
//				FALSE ---> 存在しない
BOOL ExistRecipe(LPCSTR pszFileName, SYSTEMTIME* lpLastWriteSystemTime, int iType)
{
	char szFilePath[_MAX_PATH];

	if ( !MakeRecipeFilePath(szFilePath, pszFileName, iType) )
		return FALSE;

	return FILEEXIST(szFilePath, lpLastWriteSystemTime);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadHeadTypeAndScanType
// Purpose	  : Name，HeadType，ScanTypeの読み込み(MainRecipe，MeasurementProgram関連のみ)
// Parameters : piType		---> どのファイル(MainRecipe等)なのかを決める変数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadHeadTypeAndScanType(int* piType, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];
	switch(*piType){
	case RECIPE_FILE_MAIN_RECIPE:
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, pszFileName);
		break;
	case RECIPE_FILE_MEASUREMENT_PROGRAM:
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, pszFileName);
		break;
	case RECIPE_FILE_Z_AXIS_OFFSET:
		sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, pszFileName);
		break;
	default:
		break;
	}

/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
	return LoadHeadTypeAndScanType2(piType, szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : LoadHeadTypeAndScanType2
// Purpose	  : Name，HeadType，ScanTypeの読み込み(MainRecipe，MeasurementProgram関連のみ)
// Parameters : piType		---> どのファイル(MainRecipe等)なのかを決める変数
//				pszFilePath ---> ファイルパス名
//
// Returns	  : TRUE  ---> 読み込み成功
//				FALSE ---> 読み込み失敗
BOOL LoadHeadTypeAndScanType2(int* piType, LPCSTR pszFilePath)
{
	int iIndex, iDescCount;
	LPCSTR section;
	char buff[BUFFER_LEN + 1];
	RECIPE_FILE* pDesc = NULL;
	SYSTEMTIME lastWriteSystemTime;

	switch(*piType){
	case RECIPE_FILE_MAIN_RECIPE:
		iDescCount = sizeof(MainRecipeDesc) / sizeof(MainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MainRecipeDesc, sizeof(MainRecipeDesc));
		break;
	case RECIPE_FILE_MEASUREMENT_PROGRAM:
		iDescCount = sizeof(MeasurementProgramDesc) / sizeof(MeasurementProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MeasurementProgramDesc, sizeof(MeasurementProgramDesc));
		break;
	case RECIPE_FILE_Z_AXIS_OFFSET:
		iDescCount = sizeof(MainRecipeDesc) / sizeof(MainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MainRecipeDesc, sizeof(MainRecipeDesc));
		break;
	// デフォルト値設定(ファイルには保存してないので直接入れ込む)
	case RECIPE_FILE_DEF_SR_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SR;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SR_THICKNESS:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_THICKNESS;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
// 2009.10.27 bagus Gantry 追加修正 --{--
//		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE_G;
// 2009.10.27 bagus Gantry 追加修正 --}--
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE_CIE:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_REFLECTANCE_CIE;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SR_OPTICAL_DENSITY:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_OPTICAL_DENSITY;
		return TRUE;
		break;
// 2009.09.29 bagus Stress --{--
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_STRESS:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_STRESS;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_STRESS_MEAS;
		return TRUE;
		break;
// 2009.09.29 bagus Stress --}--
// 2009.09.29 bagus SE --{--
	case RECIPE_FILE_DEF_SE_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_SE;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_SE_THICKNESS:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SE;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SE_THICKNESS;
		return TRUE;
		break;
// 2009.09.29 bagus SE --}--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_COMPEASE;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_COMPEASE_THICKNESS:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_COMPEASE;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_COMPEASE_THICKNESS;
		return TRUE;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_DEF_CTA_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_CTA;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_CTA:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_CTA;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_CTA_MEAS;
		return TRUE;
		break;
// 2009.10.07 bagus CTA 追加 --}--
// 2009.11.04 bagus RS 追加 --{--
	case RECIPE_FILE_DEF_4PP_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_4PP;
		return TRUE;
		break;
// 2009.11.04 bagus RS 追加 --}--
	case RECIPE_FILE_DEF_4PP:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_4PP;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_4PP_MEAS;
		return TRUE;
		break;
// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_DEF_SR_DISTANCE:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_SR;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_SR_DISTANCE;
		return TRUE;
		break;
// 2009.10.14 bagus Distance 追加 --}--
//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_DEF_MS_MAIN_RECIPE:
		g_MainRcpInfo.MainRcpParam.hdr.wHeadType = HEAD_TYPE_MS;
		return TRUE;
		break;
	case RECIPE_FILE_DEF_MS:
		g_MeasProgInfo.ScanParams.hdr.wHeadType = HEAD_TYPE_MS;
		g_MeasProgInfo.ScanParams.hdr.wScanType = MEAS_PROG_TYPE_MS_MEAS;
		return TRUE;
		break;
//2009.10.20 bagus MS 追加 --}--
	// MainRecipe，MeasurementProgram以外は処理なし
	default:
		return TRUE;
		break;
	}

	// ファイルが存在しない場合
	if(!FILEEXIST(pszFilePath, &lastWriteSystemTime)){
		if(pDesc != NULL){
			delete [] pDesc;
			pDesc = NULL;
		}
		return FALSE;
	}

	// データの読み込み
	for(iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if(strlen((pDesc + iIndex)->section) > 0)
			section = (pDesc + iIndex)->section;

		// get file data
		memset(buff, 0, sizeof(buff));
		::GetPrivateProfileString(section,
								(pDesc + iIndex)->key,
								(pDesc + iIndex)->def,
								buff,
								sizeof(buff),
								pszFilePath);

		// set ini file data to variable
		switch((pDesc + iIndex)->type){
		case 'W':
			*(WORD*)(pDesc + iIndex)->data = (WORD)atoi(buff);
			break;
		case 'C':
			strcpy_s((char*)(pDesc + iIndex)->data, 256, buff);
			break;
		default:
			(void*)(pDesc + iIndex)->data = NULL;
			break;
		}
	}

	if(pDesc != NULL){
		delete [] pDesc;
		pDesc = NULL;
	}

	if(*piType == RECIPE_FILE_MAIN_RECIPE){
		switch(g_MainRcpInfo.MainRcpParam.hdr.wHeadType){
		case HEAD_TYPE_SR:
			*piType = RECIPE_FILE_SR_MAIN_RECIPE;
			break;
		case HEAD_TYPE_SE:
			*piType = RECIPE_FILE_SE_MAIN_RECIPE;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			*piType = RECIPE_FILE_COMPEASE_MAIN_RECIPE;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			*piType = RECIPE_FILE_IRSE_MAIN_RECIPE;
			break;
#else
		case HEAD_TYPE_MS:
			*piType = RECIPE_FILE_MS_MAIN_RECIPE;
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			*piType = RECIPE_FILE_4PP_MAIN_RECIPE;
			break;
		case HEAD_TYPE_CTA:
			*piType = RECIPE_FILE_CTA_MAIN_RECIPE;
			break;
		case HEAD_TYPE_STRESS:
			*piType = RECIPE_FILE_STRESS_MAIN_RECIPE;
			break;
		default:
			return FALSE;
			break;
		}
	}
	else if(*piType == RECIPE_FILE_MEASUREMENT_PROGRAM){
		switch(g_MeasProgInfo.ScanParams.hdr.wHeadType){
		case HEAD_TYPE_SR:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_SR_THICKNESS:
				*piType = RECIPE_FILE_SR_THICKNESS;
				break;
			case MEAS_PROG_TYPE_SR_REFLECTANCE:
				*piType = RECIPE_FILE_SR_REFLECTANCE;
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
// 2009.10.27 bagus Gantry 追加修正 --{--
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
// 2009.10.27 bagus Gantry 追加修正 --}--
				*piType = RECIPE_FILE_SR_TRANSMITTANCE;
				break;
			// 2009.10.14 bagus Distance 追加 --{--
			case MEAS_PROG_TYPE_SR_DISTANCE:
				*piType = RECIPE_FILE_SR_DISTANCE;
				break;
			// 2009.10.14 bagus Distance 追加 --}--
			case MEAS_PROG_TYPE_SR_REFLECTANCE_CIE:
				*piType = RECIPE_FILE_SR_REFLECTANCE_CIE;
				break;
			case MEAS_PROG_TYPE_SR_TRANSMITTANCE_CIE:
				*piType = RECIPE_FILE_SR_TRANSMITTANCE_CIE;
				break;
			case MEAS_PROG_TYPE_SR_OPTICAL_DENSITY:
				*piType = RECIPE_FILE_SR_OPTICAL_DENSITY;
				break;
			default:
				return FALSE;
				break;
			}
			break;
// 2009.09.04 K.Matsuo -->
		case HEAD_TYPE_SE:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_SE_THICKNESS:
				*piType = RECIPE_FILE_SE_THICKNESS;
				break;
			default:
				return FALSE;
				break;
			}
			break;
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_COMPEASE_THICKNESS:
				*piType = RECIPE_FILE_COMPEASE_THICKNESS;
				break;
			default:
				return FALSE;
				break;
			}
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
		case HEAD_TYPE_4PP:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_4PP_MEAS:
				*piType = RECIPE_FILE_4PP;
				break;
			default:
				return FALSE;
				break;
			}
			break;
		//2009.07.30 bagus stress --{--
		case HEAD_TYPE_STRESS:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_STRESS_MEAS:
				//*piType = RECIPE_FILE_SR_THICKNESS;				////@@@@@@ダミー！
				*piType = RECIPE_FILE_STRESS;				////@@@@@@ダミー！
				break;
			default:
				return FALSE;
				break;
			}
			break;
		//2009.07.30 bagus stress --}--
		// 2009.10.07 bagus CTA 追加 --{--
		case HEAD_TYPE_CTA:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_CTA_MEAS:
				*piType = RECIPE_FILE_CTA;
				break;
			default:
				return FALSE;
				break;
			}
			break;
		// 2009.10.07 bagus CTA 追加 --}--
		//2009.10.20 bagus MS 追加 --{--
		case HEAD_TYPE_MS:
			switch(g_MeasProgInfo.ScanParams.hdr.wScanType){
			case MEAS_PROG_TYPE_MS_MEAS:
				*piType = RECIPE_FILE_MS;
				break;
			default:
				return FALSE;
				break;
			}
			break;
		//2009.10.20 bagus MS 追加 --}--
		default:
			return FALSE;
			break;
		}
	}
	else if(*piType == RECIPE_FILE_Z_AXIS_OFFSET){
		return TRUE;
	}
	else{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : SaveHeadTypeAndScanType
// Purpose	  : Name，HeadType，ScanTypeの保存(MainRecipe，MeasurementProgram関連のみ)
// Parameters : iType		---> どのファイル(MainRecipe等)なのかを決める変数
//				pszFileName ---> ファイル名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SaveHeadTypeAndScanType(int iType, LPCSTR pszFileName)
{
	char szFilePath[_MAX_PATH];

	switch(iType){
	case RECIPE_FILE_SR_MAIN_RECIPE:
	case RECIPE_FILE_SE_MAIN_RECIPE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_MAIN_RECIPE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case RECIPE_FILE_IRSE_MAIN_RECIPE:
	case RECIPE_FILE_MS_MAIN_RECIPE:
// 2009.10.19 bagus MS 追加 --}--
	case RECIPE_FILE_4PP_MAIN_RECIPE:
	case RECIPE_FILE_CTA_MAIN_RECIPE:
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, pszFileName);
		break;
	case RECIPE_FILE_Z_AXIS_OFFSET:
		sprintf_s(szFilePath, _MAX_PATH, CFG_SYSTEM_DIR "%s" DAT_EXT, pszFileName);
		break;
	case RECIPE_FILE_SR_THICKNESS:
	case RECIPE_FILE_SR_REFLECTANCE:
	case RECIPE_FILE_SR_TRANSMITTANCE:
	case RECIPE_FILE_SR_REFLECTANCE_CIE:
	case RECIPE_FILE_SR_TRANSMITTANCE_CIE:
	case RECIPE_FILE_SR_OPTICAL_DENSITY:
// 2009.09.04 K.Matsuo -->
	case RECIPE_FILE_SE_THICKNESS:
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_THICKNESS:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case RECIPE_FILE_4PP:
	//2009.08.25 bagus stress --{--
	case RECIPE_FILE_STRESS:
	//2009.08.25 bagus stress --}--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_CTA:
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_SR_DISTANCE:
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_MS:
	//2009.10.20 bagus MS 追加 --}--
		sprintf_s(szFilePath, _MAX_PATH, DB_MEASUREMENT_PROGRAM_DIR "%s" MEASUREMENTPGM_EXT, pszFileName);
		break;
	// MainRecipe，MeasurementProgram以外は処理なし(デフォルト値設定の場合も処理なし)
	default:
		return TRUE;
		break;
	}

/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
	AddAbsPath(szFilePath);
/* added 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
	return SaveHeadTypeAndScanType2(iType, szFilePath);
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : SaveHeadTypeAndScanType2
// Purpose	  : Name，HeadType，ScanTypeの保存(MainRecipe，MeasurementProgram関連のみ)
// Parameters : iType		---> どのファイル(MainRecipe等)なのかを決める変数
//				pszFilePath ---> ファイルパス名
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SaveHeadTypeAndScanType2(int iType, LPCSTR pszFilePath)
{
	int iDescCount;
	LPCSTR section;
	char buff[BUFFER_LEN + 1];
	RECIPE_FILE* pDesc = NULL;

	switch(iType){
	case RECIPE_FILE_SR_MAIN_RECIPE:
	case RECIPE_FILE_SE_MAIN_RECIPE:
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_MAIN_RECIPE:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
//	case RECIPE_FILE_IRSE_MAIN_RECIPE:
	case RECIPE_FILE_MS_MAIN_RECIPE:
// 2009.10.19 bagus MS 追加 --}--
	case RECIPE_FILE_4PP_MAIN_RECIPE:
	case RECIPE_FILE_CTA_MAIN_RECIPE:
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(MainRecipeDesc) / sizeof(MainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MainRecipeDesc, sizeof(MainRecipeDesc));
		break;
	case RECIPE_FILE_Z_AXIS_OFFSET:
		iDescCount = sizeof(MainRecipeDesc) / sizeof(MainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MainRecipeDesc, sizeof(MainRecipeDesc));
		break;
	case RECIPE_FILE_SR_THICKNESS:
	case RECIPE_FILE_SR_REFLECTANCE:
	case RECIPE_FILE_SR_TRANSMITTANCE:
	case RECIPE_FILE_SR_REFLECTANCE_CIE:
	case RECIPE_FILE_SR_TRANSMITTANCE_CIE:
	case RECIPE_FILE_SR_OPTICAL_DENSITY:
// 2009.09.04 K.Matsuo -->
	case RECIPE_FILE_SE_THICKNESS:
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_THICKNESS:
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case RECIPE_FILE_4PP:
	//2009.08.25 bagus stress --{--
	case RECIPE_FILE_STRESS:
	//2009.08.25 bagus stress --}--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_CTA:
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_SR_DISTANCE:
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_MS:
	//2009.10.20 bagus MS 追加 --}--
		iDescCount = sizeof(MeasurementProgramDesc) / sizeof(MeasurementProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MeasurementProgramDesc, sizeof(MeasurementProgramDesc));
		break;
	// MainRecipe，MeasurementProgram以外は処理なし(デフォルト値設定の場合も処理なし)
	default:
		return TRUE;
		break;
	}

	for(int iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if (strlen((pDesc + iIndex)->section) > 0)
			section = (pDesc + iIndex)->section;

		// set file data
		memset(buff, 0, sizeof(buff));
		switch((pDesc + iIndex)->type){
		case 'W':
			sprintf(buff, "%d", *(WORD*)(pDesc + iIndex)->data);
			break;
		case 'C':
			strncpy(buff, (char*)(pDesc + iIndex)->data, BUFFER_LEN);
			break;
		default:
			break;
		}

		// write NanoSpec ini file
		::WritePrivateProfileString(section,
									(pDesc + iIndex)->key,
									buff,
									pszFilePath);
	}

// 破棄し忘れ
	if(pDesc != NULL){
		delete [] pDesc;
		pDesc = NULL;
	}
// 破棄し忘れ

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name 	  : ClearMemory
// Purpose	  : 読み込みに使用するグローバル変数のクリア(初期化)
// Parameters : なし
//
// Returns	  : なし
void ClearMemory()
{
	memset(&g_MainRcpInfo, 0, sizeof(MAIN_RCP_INFO));
	memset(&g_MultiRcpInfoHdr, 0, sizeof(MULTI_RCP_INFO_HDR));
	memset(&g_MeasProgInfo, 0, sizeof(MEAS_PROG_INFO));
	memset(&g_StageProgInfoHdr, 0, sizeof(STAGE_PROG_INFO_HDR));
	memset(&g_RecalibProgInfo, 0, sizeof(RECALIB_PROG_INFO));
	memset(&g_PointDeskewProgInfo, 0, sizeof(POINT_DESKEW_PROG_INFO));
	memset(&g_SampleInfo, 0, sizeof(SAMPLEINFO));
	memset(&g_UserAccount, 0, sizeof(USER_ACCOUNT));
}


// =========================================================================
// Name 	  : SaveCurrRecipeInfo
// Purpose	  : Recipe，Program，UserAccount，Sampleの保存
// Parameters : pszMainRcpName ---> メインレシピ名
//			  : iMeasType(デフォルト引数) ---> 測定タイプ(マニュアル測定かそうでないかを識別するためのもの) (デフォルト値は0)
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SaveCurrRecipeInfo(LPCSTR pszMainRcpName, int iMeasType)
{
	MAIN_RCP_INFO mainRcpInfo;
	MEAS_PROG_INFO measProgInfo;
	STAGE_PROG_INFO_HDR stageProgInfoHdr;
	RECALIB_PROG_INFO recalibProgInfo;
	POINT_DESKEW_PROG_INFO pointDeskewProgInfo;
	STAGE_COORD point[SCAN_POINT_MAX];
	ZeroMemory(point, sizeof(point));
	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, MEAS_DAT_TEMP_COPY_PASS "%s" DAT_EXT, pszMainRcpName);
/* modified 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- { ---------- */
//	CreateDirectory(MEAS_DAT_TEMP_COPY_PASS, NULL);
/* modified 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ----------			  */
	AddAbsPath(szFilePath);
	TCHAR l_tszTempFName[_MAX_PATH];
	_stprintf(l_tszTempFName, _T("%s") MEAS_DAT_TEMP_COPY_PASS, g_tszProcDir);
	CreateDirectory(l_tszTempFName, NULL);
/* modified 2009.07.07 hmenjo dll 相対パス対応 RecipeFile.dll ---------- } ---------- */
//2009.09.17 bagus stress --{--
	STAGE_PROG_STRESS stressStageProgInfo;
//2009.09.17 bagus stress --}--

// MAIN
	if ( !LoadRecipe(&mainRcpInfo, pszMainRcpName, RECIPE_FILE_MAIN_RECIPE) )
		return FALSE;
	if ( !SaveCurrRecipeInfo(pszMainRcpName, szFilePath, RECIPE_FILE_MAIN_RECIPE) )
		return FALSE;
// MEASUREMENT
	if ( LoadRecipe(&measProgInfo, mainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_MEASUREMENT_PROGRAM) )
		SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam.hdr.szMeas, szFilePath, RECIPE_FILE_MEASUREMENT_PROGRAM);

	//2009.09.17 bagus stress --{--
	if(mainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		if ( LoadRecipe(&measProgInfo, mainRcpInfo.MainRcpParam.hdr.szMeas, RECIPE_FILE_SR_THICKNESS) )
			SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam.hdr.szMeas, szFilePath, RECIPE_FILE_SR_THICKNESS);

	}
	//2009.09.17 bagus stress --}--
	if(iMeasType != 1) //マニュアル測定以外
	{
		//マニュアル測定は、[StageProgram][Point]は不要。
	// STAGE
		if ( LoadRecipe(&stageProgInfoHdr, mainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM) )
			SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam.hdr.szStage, szFilePath, RECIPE_FILE_STAGE_PROGRAM);

	// POINT LIST
		//2009.10.28 bagus 2point-Distance --{--
		if( stageProgInfoHdr.wHeadType == HEAD_TYPE_SR
		&&	stageProgInfoHdr.wScanType == SCAN_TYPE_SR_DISTANCE){
			if ( stageProgInfoHdr.wNumScans && Load2PointList(&point[0], stageProgInfoHdr.wNumScans, mainRcpInfo.MainRcpParam.hdr.szStage) )
			//2010.03.09 bagus Recipe Backup 修正 --{--
			//SaveCurrRecipeInfoの場合にはDBフォルダへのレシピの保存ではなく結果ファイルの作成なのでバックアップ処理は動かさない
			//Save2PointList2(&point[0], stageProgInfoHdr.wNumScans, szFilePath);
			Save2PointList2(&point[0], stageProgInfoHdr.wNumScans, szFilePath,FALSE);
			//2010.03.09 bagus Recipe Backup 修正 --}--
		}else{
		if ( stageProgInfoHdr.wNumScans && LoadPointList(&point[0], stageProgInfoHdr.wNumScans, mainRcpInfo.MainRcpParam.hdr.szStage) )
			//2010.03.09 bagus Recipe Backup 修正 --{--
			//SaveCurrRecipeInfoの場合にはDBフォルダへのレシピの保存ではなく結果ファイルの作成なのでバックアップ処理は動かさない
			//SavePointList2(&point[0], stageProgInfoHdr.wNumScans, szFilePath);
			SavePointList2(&point[0], stageProgInfoHdr.wNumScans, szFilePath,FALSE);
			//2010.03.09 bagus Recipe Backup 修正 --}--
		}
		//2009.10.28 bagus 2point-Distance --}--
	}
	//2009.09.17 bagus stress --{--
	if(mainRcpInfo.MainRcpParam.hdr.wHeadType == HEAD_TYPE_STRESS){
		LoadRecipe(&stressStageProgInfo,mainRcpInfo.MainRcpParam.hdr.szStage, RECIPE_FILE_STAGE_PROGRAM_STRESS);
		SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam.hdr.szStage, szFilePath, RECIPE_FILE_STAGE_PROGRAM_STRESS);
	}
	//2009.09.17 bagus stress --}--

// POINT DESKEW
	if ( LoadRecipe(&pointDeskewProgInfo, mainRcpInfo.MainRcpParam.hdr.szPointDeskew, RECIPE_FILE_POINT_DESKEW_PROGRAM) )
		SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam.hdr.szPointDeskew, szFilePath, RECIPE_FILE_POINT_DESKEW_PROGRAM);

// RECALIBRATION
	if ( strcmp(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], "None") != 0 && mainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_1] != -1)
	{
		if ( !LoadRecipe(&recalibProgInfo, mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], RECIPE_FILE_RECALIBRATION_PROGRAM) )
			return FALSE;
		if ( !SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_1], szFilePath, RECIPE_FILE_RECALIBRATION_PROGRAM) )
			return FALSE;
	}
	if ( strcmp(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], "None") != 0 && mainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_2] != -1)
	{
		if ( !LoadRecipe(&recalibProgInfo, mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], RECIPE_FILE_RECALIBRATION_PROGRAM) )
			return FALSE;
//		if ( !SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], szFilePath, RECIPE_FILE_RECALIBRATION_PROGRAM) )
		if ( !SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_2], szFilePath, RECIPE_FILE_RECALIBRATION_PROGRAM_TWO) )
			return FALSE;
	}
	if ( strcmp(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], "None") != 0 && mainRcpInfo.MainRcpParam._SR.RecalibItem[RECALIB_3] != -1)
	{
		if ( !LoadRecipe(&recalibProgInfo, mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], RECIPE_FILE_RECALIBRATION_PROGRAM) )
			return FALSE;
//		if ( !SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], szFilePath, RECIPE_FILE_RECALIBRATION_PROGRAM) )
		if ( !SaveCurrRecipeInfo(mainRcpInfo.MainRcpParam._SR.szRecalib[RECALIB_3], szFilePath, RECIPE_FILE_RECALIBRATION_PROGRAM_THREE) )
			return FALSE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Name 	  : SaveCurrRecipeInfo
// Purpose	  : Recipe，Program，UserAccount，Sampleの保存
// Parameters : pszRcpName	---> レシピ名
//				pszFilePath ---> ファイルパス名
//				iType		---> どのファイル(MainRecipe等)なのかを決める変数
//
// Returns	  : TRUE  ---> 保存成功
//				FALSE ---> 保存失敗
BOOL SaveCurrRecipeInfo(LPCSTR pszRcpName, LPCSTR pszFilePath, int iType)
{
	int iIndex, iDescCount;
	int iTypeBuff;
	LPCSTR section;
	char buff[BUFFER_LEN + 1];
	RECIPE_FILE* pDesc = NULL;

	iTypeBuff = iType;
	// HeadType，ScanTypeを読み込む
	if(!LoadHeadTypeAndScanType(&iTypeBuff, pszRcpName)){
		return FALSE;
	}

	switch(iTypeBuff){
	// Recipe Setting関連
	case RECIPE_FILE_SR_MAIN_RECIPE:
		iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
		break;
	case RECIPE_FILE_SE_MAIN_RECIPE:
		iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_MAIN_RECIPE:
		iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case RECIPE_FILE_IRSE_MAIN_RECIPE:
		iDescCount = sizeof(IrseMainRecipeDesc) / sizeof(IrseMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, IrseMainRecipeDesc, sizeof(IrseMainRecipeDesc));
		break;
#else
	case RECIPE_FILE_MS_MAIN_RECIPE:
		iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case RECIPE_FILE_4PP_MAIN_RECIPE:
		iDescCount = sizeof(ResistMainRecipeDesc) / sizeof(ResistMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistMainRecipeDesc, sizeof(ResistMainRecipeDesc));
		break;
	case RECIPE_FILE_CTA_MAIN_RECIPE:
		iDescCount = sizeof(ContactAngleMainRecipeDesc) / sizeof(ContactAngleMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleMainRecipeDesc, sizeof(ContactAngleMainRecipeDesc));
		break;
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(StressMainRecipeDesc) / sizeof(StressMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressMainRecipeDesc, sizeof(StressMainRecipeDesc));
		break;
	case RECIPE_FILE_MULTI_RECIPE:
		iDescCount = sizeof(MultiRecipeDesc) / sizeof(MultiRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MultiRecipeDesc, sizeof(MultiRecipeDesc));
		break;
	case RECIPE_FILE_STAGE_PROGRAM:
		iDescCount = sizeof(StageProgramDesc) / sizeof(StageProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StageProgramDesc, sizeof(StageProgramDesc));
		break;
	//2009.09.17 bagus stress --{--
	case RECIPE_FILE_STAGE_PROGRAM_STRESS:
		break;
	//2009.09.17 bagus stress --}--
	case RECIPE_FILE_RECALIBRATION_PROGRAM:
	case RECIPE_FILE_RECALIBRATION_PROGRAM_TWO:
	case RECIPE_FILE_RECALIBRATION_PROGRAM_THREE:
		iDescCount = sizeof(RecalibrationProgramDesc) / sizeof(RecalibrationProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, RecalibrationProgramDesc, sizeof(RecalibrationProgramDesc));
		break;
	case RECIPE_FILE_POINT_DESKEW_PROGRAM:
		iDescCount = sizeof(PointDeskewProgramDesc) / sizeof(PointDeskewProgramDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, PointDeskewProgramDesc, sizeof(PointDeskewProgramDesc));
		break;
	case RECIPE_FILE_SR_THICKNESS:
		iDescCount = sizeof(SrThicknessDesc) / sizeof(SrThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrThicknessDesc, sizeof(SrThicknessDesc));
		break;
	case RECIPE_FILE_SR_REFLECTANCE:
		iDescCount = sizeof(SrReflectDesc) / sizeof(SrReflectDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectDesc, sizeof(SrReflectDesc));
		break;
	case RECIPE_FILE_SR_TRANSMITTANCE:
		iDescCount = sizeof(SrTransmitDesc) / sizeof(SrTransmitDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitDesc, sizeof(SrTransmitDesc));
		break;
	case RECIPE_FILE_SR_REFLECTANCE_CIE:
		iDescCount = sizeof(SrReflectCieDesc) / sizeof(SrReflectCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectCieDesc, sizeof(SrReflectCieDesc));
		break;
	case RECIPE_FILE_SR_TRANSMITTANCE_CIE:
		iDescCount = sizeof(SrTransmitCieDesc) / sizeof(SrTransmitCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitCieDesc, sizeof(SrTransmitCieDesc));
		break;
	case RECIPE_FILE_SR_OPTICAL_DENSITY:
		iDescCount = sizeof(SrOdDesc) / sizeof(SrOdDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrOdDesc, sizeof(SrOdDesc));
		break;
// 2009.09.04 K.Matsuo -->
	case RECIPE_FILE_SE_THICKNESS:
		iDescCount = sizeof(SeThicknessDesc) / sizeof(SeThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeThicknessDesc, sizeof(SeThicknessDesc));
		break;
// 2009.09.04 K.Matsuo <--
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_COMPEASE_THICKNESS:
		iDescCount = sizeof(CompEASEThicknessDesc) / sizeof(CompEASEThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEThicknessDesc, sizeof(CompEASEThicknessDesc));
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	case RECIPE_FILE_4PP:
		iDescCount = sizeof(ResistDesc) / sizeof(ResistDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistDesc, sizeof(ResistDesc));
		break;
	case RECIPE_FILE_CTA:
		iDescCount = sizeof(ContactAngleDesc) / sizeof(ContactAngleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleDesc, sizeof(ContactAngleDesc));
		break;
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_SR_DISTANCE:
		iDescCount = sizeof(SrDistanceDesc) / sizeof(SrDistanceDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrDistanceDesc, sizeof(SrDistanceDesc));
		break;
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_MS:
		iDescCount = sizeof(MicroScopeDesc) / sizeof(MicroScopeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeDesc, sizeof(MicroScopeDesc));
		break;
	//2009.10.20 bagus MS 追加 --}--
	// User Setting関連(UserAccountの設定)
	case RECIPE_FILE_USER_ACCOUNT:
		iDescCount = sizeof(UserAccountDesc) / sizeof(UserAccountDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, UserAccountDesc, sizeof(UserAccountDesc));
		break;
	// User Setting関連(デフォルト値の設定)
	case RECIPE_FILE_DEF_SR_MAIN_RECIPE:
		iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
		break;
	case RECIPE_FILE_DEF_SR_THICKNESS:
		iDescCount = sizeof(SrThicknessDesc) / sizeof(SrThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrThicknessDesc, sizeof(SrThicknessDesc));
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE:
		iDescCount = sizeof(SrReflectDesc) / sizeof(SrReflectDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectDesc, sizeof(SrReflectDesc));
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE:
		iDescCount = sizeof(SrTransmitDesc) / sizeof(SrTransmitDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitDesc, sizeof(SrTransmitDesc));
		break;
	case RECIPE_FILE_DEF_SR_REFLECTANCE_CIE:
		iDescCount = sizeof(SrReflectCieDesc) / sizeof(SrReflectCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrReflectCieDesc, sizeof(SrReflectCieDesc));
		break;
	case RECIPE_FILE_DEF_SR_TRANSMITTANCE_CIE:
		iDescCount = sizeof(SrTransmitCieDesc) / sizeof(SrTransmitCieDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrTransmitCieDesc, sizeof(SrTransmitCieDesc));
		break;
	case RECIPE_FILE_DEF_SR_OPTICAL_DENSITY:
		iDescCount = sizeof(SrOdDesc) / sizeof(SrOdDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrOdDesc, sizeof(SrOdDesc));
		break;
	// 2009.09.29 bagus Stress --{--
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		iDescCount = sizeof(StressMainRecipeDesc) / sizeof(StressMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressMainRecipeDesc, sizeof(StressMainRecipeDesc));
		break;
	case RECIPE_FILE_DEF_STRESS:
		iDescCount = sizeof(StressDesc) / sizeof(StressDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, StressDesc, sizeof(StressDesc));
		break;
	// 2009.09.29 bagus Stress --}--
	// 2009.09.29 bagus SE --{--
	case RECIPE_FILE_DEF_SE_MAIN_RECIPE:
		iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
		break;
	case RECIPE_FILE_DEF_SE_THICKNESS:
		iDescCount = sizeof(SeThicknessDesc) / sizeof(SeThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SeThicknessDesc, sizeof(SeThicknessDesc));
		break;
	// 2009.09.29 bagus SE --}--
	// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case RECIPE_FILE_DEF_COMPEASE_MAIN_RECIPE:
		iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
		break;
	case RECIPE_FILE_DEF_COMPEASE_THICKNESS:
		iDescCount = sizeof(CompEASEThicknessDesc) / sizeof(CompEASEThicknessDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, CompEASEThicknessDesc, sizeof(CompEASEThicknessDesc));
		break;
	// 2013.02.01 bagus CompleteEASEヘッド追加 <--
	// 2009.10.07 bagus CTA 追加 --{--
	case RECIPE_FILE_DEF_CTA_MAIN_RECIPE:
		iDescCount = sizeof(ContactAngleMainRecipeDesc) / sizeof(ContactAngleMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleMainRecipeDesc, sizeof(ContactAngleMainRecipeDesc));
		break;
	case RECIPE_FILE_DEF_CTA:
		iDescCount = sizeof(ContactAngleDesc) / sizeof(ContactAngleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ContactAngleDesc, sizeof(ContactAngleDesc));
		break;
	// 2009.10.07 bagus CTA 追加 --}--
	// 2009.11.04 bagus RS 追加 --{--
	case RECIPE_FILE_DEF_4PP_MAIN_RECIPE:
		iDescCount = sizeof(ResistMainRecipeDesc) / sizeof(ResistMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistMainRecipeDesc, sizeof(ResistMainRecipeDesc));
		break;
	// 2009.11.04 bagus RS 追加 --}--
	case RECIPE_FILE_DEF_4PP:
		iDescCount = sizeof(ResistDesc) / sizeof(ResistDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, ResistDesc, sizeof(ResistDesc));
		break;
	// 2009.10.14 bagus Distance 追加 --{--
	case RECIPE_FILE_DEF_SR_DISTANCE:
		iDescCount = sizeof(SrDistanceDesc) / sizeof(SrDistanceDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SrDistanceDesc, sizeof(SrDistanceDesc));
		break;
	// 2009.10.14 bagus Distance 追加 --}--
	//2009.10.20 bagus MS 追加 --{--
	case RECIPE_FILE_DEF_MS_MAIN_RECIPE:
		iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
		break;
	case RECIPE_FILE_DEF_MS:
		iDescCount = sizeof(MicroScopeDesc) / sizeof(MicroScopeDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, MicroScopeDesc, sizeof(MicroScopeDesc));
		break;
	//2009.10.20 bagus MS 追加 --}--
	// System関連(Sampleの設定)
	case RECIPE_FILE_SAMPLE:
		iDescCount = sizeof(SampleDesc) / sizeof(SampleDesc[0]);
		pDesc = new RECIPE_FILE [iDescCount];
		memcpy(pDesc, SampleDesc, sizeof(SampleDesc));
		break;
	// System関連(ZAxisOffsetの設定)
	case RECIPE_FILE_Z_AXIS_OFFSET:
		switch(g_MainRcpInfo.MainRcpParam.hdr.wHeadType){
		case HEAD_TYPE_SR:
			iDescCount = sizeof(SrMainRecipeDesc) / sizeof(SrMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SrMainRecipeDesc, sizeof(SrMainRecipeDesc));
			break;
		case HEAD_TYPE_SE:
			iDescCount = sizeof(SeMainRecipeDesc) / sizeof(SeMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, SeMainRecipeDesc, sizeof(SeMainRecipeDesc));
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			iDescCount = sizeof(CompEASEMainRecipeDesc) / sizeof(CompEASEMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, CompEASEMainRecipeDesc, sizeof(CompEASEMainRecipeDesc));
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			iDescCount = sizeof(IrseMainRecipeDesc) / sizeof(IrseMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, IrseMainRecipeDesc, sizeof(IrseMainRecipeDesc));
			break;
#else
		case HEAD_TYPE_MS:
			iDescCount = sizeof(MicroScopeMainRecipeDesc) / sizeof(MicroScopeMainRecipeDesc[0]);
			pDesc = new RECIPE_FILE [iDescCount];
			memcpy(pDesc, MicroScopeMainRecipeDesc, sizeof(MicroScopeMainRecipeDesc));
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		default:
			return FALSE;
			break;
		}
		break;
	default:
		return FALSE;
		break;
	}

	// Name，HeadType，ScanTypeを書き込む
	if(!SaveHeadTypeAndScanType2(iTypeBuff, pszFilePath)){
		if(pDesc != NULL){
			delete [] pDesc;
			pDesc = NULL;
		}
	}

//2009.08.31 bagus stress --{--
	if (RECIPE_FILE_STAGE_PROGRAM_STRESS == iType) {
		STAGE_PROG_STRESS *l_pStageProgStress = (STAGE_PROG_STRESS *)&lg_StageProgStress;
		/* ストレスの LS 定義の読出し	*/
		::WritePrivateProfileString(
				_T("StageProgram"),
				_T("ElasticModulusName"),
				l_pStageProgStress->tszElasticModulusName,
				pszFilePath
			);
		TCHAR l_tszTemp[64];
// 2009.11.09 bagus Stress 追加変更 --{--
//		_stprintf_s(l_tszTemp, _countof(l_tszTemp), "%.3f", l_pStageProgStress->dElasticModulusValue);
		_stprintf_s(l_tszTemp, _countof(l_tszTemp), "%e", l_pStageProgStress->dElasticModulusValue);
// 2009.11.09 bagus Stress 追加変更 --}--
		::WritePrivateProfileString(
				_T("StageProgram"),
				_T("ElasticModulusValue"),
				l_tszTemp,
				pszFilePath
			);
// 2009.09.08 bagus stress 修正 --{--
		_stprintf_s(l_tszTemp, _countof(l_tszTemp), "%ld", l_pStageProgStress->dwNumLsScans);
		::WritePrivateProfileString(
				_T("StageProgram"),
				_T("NumLSScans"),
				l_tszTemp,
				pszFilePath
			);
// 2009.09.08 bagus stress 修正 --}--
//		lg_StageProgStress.dElasticModulusValue = _tcstod(l_tszTemp, 0);
		TCHAR l_tszSec[] = _T("LS");
		TCHAR l_tszKey[255];
		/* ストレス設定を読出し：ライン数とライン毎セクション数	*/
		STRESS_CONFIG l_StressConfig;
		ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
		DWORD l_dwLineNum = l_StressConfig.dwLiftPinNumberOfLine;
		if (STRESS_LINES_MAX < l_dwLineNum) {
			l_dwLineNum = STRESS_LINES_MAX;
		}
		DWORD l_dwSectionNum[STRESS_LINES_MAX];
		memset(l_dwSectionNum, 0, sizeof(l_dwSectionNum));
  DWORD iLine;
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			l_dwSectionNum[iLine] = l_StressConfig.Line[iLine].dwSectionNum;
			if (STRESS_SECTIONS_MAX < l_dwSectionNum[iLine]) {
				l_dwSectionNum[iLine] = STRESS_SECTIONS_MAX;
			}
		}
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			/* ライン有効/無効を読出し	*/
			_stprintf(l_tszKey, LS_VALIDLINEn, iLine + 1);
			_stprintf(l_tszTemp,_T("%s"),l_pStageProgStress->Line[iLine].bValidLine ? _T("TRUE") : _T("FALSE"));
			::WritePrivateProfileString(l_tszSec, l_tszKey, l_tszTemp, pszFilePath);
			for (DWORD iSec = 0; iSec < l_dwSectionNum[iLine]; iSec++) {
				/* セクション有効/無効を読出し	*/
				_stprintf(l_tszKey, LS_LnSnSCANVALID, iLine + 1, iSec + 1);
// 2009.09.10 bagus stress 修正 --{--
//				if ((0 != l_pStageProgStress->Line[iLine].bValidLine)
//				 && (0 != l_pStageProgStress->Line[iLine].bScanValid[iSec])) {
				if (0 != l_pStageProgStress->Line[iLine].bValidLine) {
				 	if (0 != l_pStageProgStress->Line[iLine].bScanValid[iSec]) {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("TRUE"), pszFilePath);
					} else {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), pszFilePath);
// 2009.09.24 bagus stress --{--
						continue;
// 2009.09.24 bagus stress --}--
					}
// 2009.09.10 bagus stress 修正 --}--
					/* セクション位置を読出し	*/
					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanStartPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,pszFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanEndPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,pszFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanPosY );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,pszFilePath);
				} else {
// 2009.09.09 bagus stress 修正 --{--
#if 0
					WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), pszFilePath);
					WritePrivateProfileString(l_tszSec, l_tszKey,_T("0"),pszFilePath);
					WritePrivateProfileString(l_tszSec, l_tszKey,_T("0"),pszFilePath);
					WritePrivateProfileString(l_tszSec, l_tszKey,_T("0"),pszFilePath);
#else
// 2009.09.10 bagus stress 修正 --{--
//					WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), szFilePath);
				 	if (0 != l_pStageProgStress->Line[iLine].bScanValid[iSec]) {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("TRUE"), pszFilePath);
					} else {
						WritePrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), pszFilePath);
// 2009.09.24 bagus stress --{--
						continue;
// 2009.09.24 bagus stress --}--

					}
// 2009.09.10 bagus stress 修正 --}--
					/* セクション位置を読出し	*/
					_stprintf(l_tszKey, LS_LnSnSCANSTART_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanStartPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,pszFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANEND_X, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanEndPosX );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,pszFilePath);
					_stprintf(l_tszKey, LS_LnSnSCANY, iLine + 1, iSec + 1);
					_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%ld"), l_pStageProgStress->Line[iLine].SectPos[iSec].lScanPosY );
					WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,pszFilePath);
#endif
// 2009.09.09 bagus stress 修正 --}--
				}
			}
		}
		return TRUE;
	}
//2009.08.31 bagus stress --}--
	char szSectionBuff[255];
	memset(szSectionBuff, 0, sizeof(szSectionBuff));

	for(iIndex = 0; iIndex < iDescCount; iIndex++){
		// get section name
		if (strlen((pDesc + iIndex)->section) > 0)
//			section = (pDesc + iIndex)->section;
		{
			if(iTypeBuff == RECIPE_FILE_RECALIBRATION_PROGRAM)
			{
				sprintf(szSectionBuff, "%s%s", (pDesc + iIndex)->section, "1");
				section = szSectionBuff;
			}
			else if(iTypeBuff == RECIPE_FILE_RECALIBRATION_PROGRAM_TWO)
			{
				sprintf(szSectionBuff, "%s%s", (pDesc + iIndex)->section, "2");
				section = szSectionBuff;
			}
			else if(iTypeBuff == RECIPE_FILE_RECALIBRATION_PROGRAM_THREE)
			{
				sprintf(szSectionBuff, "%s%s", (pDesc + iIndex)->section, "3");
				section = szSectionBuff;
			}
			else
			{
				section = (pDesc + iIndex)->section;
			}
		}


		// set file data
		memset(buff, 0, sizeof(buff));
		switch((pDesc + iIndex)->type){
		case 'b':
			strcpy(buff, (*(BOOL*)(pDesc + iIndex)->data)? "TRUE" : "FALSE");
			break;
		case 'B':
			_itoa(*(BYTE*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'S':
			_itoa(*(short*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'I':
			_itoa(*(int*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'L':
			_ltoa(*(long*)(pDesc + iIndex)->data, buff, 10);
			break;
		case 'F':
			sprintf(buff, "%.3f", *(float*)(pDesc + iIndex)->data);
			break;
		case 'D':
			sprintf(buff, "%.3lf", *(double*)(pDesc + iIndex)->data);
			break;
		case 'W':
			sprintf(buff, "%d", *(WORD*)(pDesc + iIndex)->data);
			break;
		case 'C':
			strncpy(buff, (char*)(pDesc + iIndex)->data, BUFFER_LEN);
			break;
		default:
			break;
		}

		// write NanoSpec ini file
		::WritePrivateProfileString(section,
									(pDesc + iIndex)->key,
									buff,
									pszFilePath);
	}

	if(pDesc != NULL){
		delete [] pDesc;
		pDesc = NULL;
	}

	return TRUE;
}
//2009.12.10 bagus Recipe Backup --{--
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

	strcpy_s(szBuff, sizeof(szBuff), lpszDir);
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
// ==========================================================================
// 関数名：BOOL DeleteDirectory(LPCTSTR lpPathName)
// 引　数：lpPathName　削除するディレクトリのパス名が入ったNULLで終わる文字列。
// 戻り値：関数が成功すると、0以外の値が返り、関数が失敗すると、0が返ります。
// 解　説：ディレクトリを丸ごと削除する。
BOOL DeleteDirectory(LPCTSTR lpPathName)
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

		//2010.01.19 bagus Recipe Backup --{--
		//テストモードのレシピはバックアップしない
		if(szFile[0] == '_'){
			return TRUE;
		}
		//2010.01.19 bagus Recipe Backup --}--

		//DB\からのディレクトリパスを取得
		pSubDirPos = strstr(szDir,DB_DIR);
		if(pSubDirPos == NULL){
			pSubDirPos = ".";
		}else{
			pSubDirPos = pSubDirPos + strlen(DB_DIR);
		}
		//2009.12.25 bagus Recipe Backup --{--
		wsprintf_s(szToPath, sizeof(szToPath), "%s\\%s", g_szBackupPath,DB_DIR_NAME);
		DWORD dwRet = GetFileAttributes(szToPath);
		if(dwRet == (DWORD)-1){
			//DBディレクトリがなければフルバックアップ
			return BackupAllRecipe();

		}
		//2009.12.25 bagus Recipe Backup --}--
		//バックアップ先のディレクトリを作成しておく
		wsprintf_s(szToPath, sizeof(szToPath), "%s\\%s\\%s", g_szBackupPath,DB_DIR_NAME,pSubDirPos);
		//2009.12.25 bagus Recipe Backup --{--
		DeleteDirectory(szToPath);
		//2009.12.25 bagus Recipe Backup --}--
		MakeDirectory(szToPath);
//2009.12.25 bagus Recipe Backup --{--
//		wsprintf_s(szToPath, sizeof(szToPath), "%s\\%s\\%s\\%s%s", g_szBackupPath,DB_DIR_NAME,pSubDirPos,szFile,szExt);
//		bRet = CopyFile(lpszSourcePath,szToPath,FALSE);
		wsprintf_s(szFromPath, sizeof(szFromPath), "%s\\%s\\%s", szDrive,szDir,"*");
		bRet = CopyDirectory(szFromPath,szToPath);
//2009.12.25 bagus Recipe Backup --}--
	}

	return TRUE;
}
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
		wsprintf_s(szToPath, sizeof(szToPath), "%s\\%s\\%s\\%s%s", g_szBackupPath,DB_DIR_NAME,pSubDirPos,szFile,szExt);
		bRet = DeleteFile(szToPath);
	}

	return TRUE;


}
//2009.12.25 bagus Recipe Backup --}--
BOOL BackupAllRecipe()
{
	//DB_DIR以下を指定されたバックアップパスに丸ごと保存します。
	//この時に、もともとバックアップパスに存在していたフォルダとファイルは削除されます。
	DWORD dwRet;
	CString strPath;

	//バックアップパスが指定されていないのでなにもしない
	if(strlen(g_szBackupPath) == 0)
		return TRUE;

	strPath = g_szBackupPath;
	strPath += "\\" DB_DIR_NAME;

	//バックアップパスに存在する既存のファイルとディレクトリを削除する
	//dwRet = GetFileAttributes(g_szBackupPath);
	dwRet = GetFileAttributes(strPath.GetBuffer(0));
	if(dwRet != (DWORD)-1 && (dwRet & FILE_ATTRIBUTE_DIRECTORY) != 0){
		//2009.12.21 bagus Recipe Backup --{--
		//if(!DeleteDirectory(g_szBackupPath))
		if(!DeleteDirectory(strPath.GetBuffer(0)))
			return FALSE;
		//2009.12.21 bagus Recipe Backup --}--
	}
	//バックアップ用のディレクトリを作成する
	//if(!MakeDirectory(g_szBackupPath))
	if(!MakeDirectory(strPath.GetBuffer(0)))
		return FALSE;

	//全ファイルをコピーする
	return CopyDirectory(DB_DIR "*",strPath.GetBuffer(0)) == 0;
}
BOOL RestoreAllRecipe()
{
	//DB_DIR以下に指定されたバックアップパスから丸ごとコピーします
	//この時に、もともとDB_DIRに存在していたフォルダとファイルは削除されます。
	DWORD dwRet;
	CString strPath;
	//2009.12.25 bagus --{--
	CString strTo;
	BOOL bRet = TRUE;
	char *dirNames[] = {
		DB_MAIN_RECIPE_DIR_NAME,
		DB_MULTI_RECIPE_DIR_NAME,
		DB_STAGE_PROGRAM_DIR_NAME,
		DB_DESKEW_IMG_DIR_NAME,
		DB_PATTERN_DIR_NAME,
		DB_PATTERN_IMG_DIR_NAME,
		DB_RECALIBRATION_PROGRAM_DIR_NAME,
		DB_POINT_DESKEW_PROGRAM_DIR_NAME
	};
	char *pDirName;
	//2009.12.25 bagus --}--

	//バックアップパスが指定されていないのでなにもしない
	if(strlen(g_szBackupPath) == 0)
		return TRUE;


	for(int n = 0;n < sizeof(dirNames)/sizeof(dirNames[0]);n++){
		pDirName = dirNames[n];
		//全ファイルをコピーする
		strPath = g_szBackupPath;
		strPath = strPath + "\\" DB_DIR_NAME;
		strPath = strPath + "\\";
		strPath = strPath + pDirName;

		//コピー元のディレクトリが存在しない場合にはなにもしない
		dwRet = GetFileAttributes(strPath.GetBuffer(0));

		if(dwRet == (DWORD)-1 || (dwRet & FILE_ATTRIBUTE_DIRECTORY) == 0){
			//return FALSE;
			continue;
		}

		//バックアップパスに存在する既存のファイルとディレクトリを削除する
		strTo = DB_DIR;
		strTo = strTo + "\\";
		strTo = strTo + pDirName;
		dwRet = GetFileAttributes(strTo.GetBuffer(0));
		if(dwRet != (DWORD)-1 && (dwRet & FILE_ATTRIBUTE_DIRECTORY) != 0){
			if(!DeleteDirectory(strTo.GetBuffer(0)))
				return FALSE;
		}
		if(!MakeDirectory(strTo.GetBuffer(0)))
			return FALSE;

		strPath += "\\*";
		bRet = CopyDirectory(strPath.GetBuffer(0),strTo.GetBuffer(0)) == 0;
		if(!bRet)
			break;
	}
	return bRet;
}
//2009.12.10 bagus Recipe Backup --}--
//Saiki 20110208 Add ----->
BOOL SetStressParam(LPVOID pVoid, LPCSTR lpszName, int iType)
{
	char szFilePath[_MAX_PATH];
	/* 応力測定の使用する膜厚値のタイプを取得 */
	TCHAR l_tszSec[64] = StressParamSec;
	TCHAR l_tszKey[64] = PriorityKey;
	TCHAR l_tszTemp[64];

	ClearMemory();

	switch(iType){
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		//memcpy(pVoid, &lg_StressParamInfo, sizeof(STRESS_PARAM_INFO));
		memcpy(&lg_StressParamInfo, pVoid, sizeof(STRESS_PARAM_INFO));
		break;
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		//memcpy(pVoid, &lg_StressParamInfo, sizeof(STRESS_PARAM_INFO));
		memcpy(&lg_StressParamInfo, pVoid, sizeof(STRESS_PARAM_INFO));
		break;
	default:
		return FALSE;
		break;
	}

	AddAbsPath(szFilePath);

	STRESS_PARAM_INFO *l_pStressParamInfo = (STRESS_PARAM_INFO *)pVoid;
	/* ストレスの LS 定義の書込み	*/
	_stprintf(l_tszTemp,_T("%s"),lg_StressParamInfo.bPriority ? _T("TRUE") : _T("FALSE"));
	if(FALSE == ::WritePrivateProfileString(l_tszSec, l_tszKey, l_tszTemp, szFilePath))
		return FALSE;

	/* ストレス設定を読出し：ライン数とライン毎セクション数	*/
	STRESS_CONFIG l_StressConfig;
	ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
	DWORD l_dwLineNum = l_StressConfig.dwLiftPinNumberOfLine;
	if (STRESS_LINES_MAX < l_dwLineNum) {
		l_dwLineNum = STRESS_LINES_MAX;
	}
 DWORD iLine;
	for (iLine = 0; iLine < l_dwLineNum; iLine++) {
		for (DWORD iPoint = 0; iPoint < PIN_INTERVAL; iPoint++){
			/* セクション位置を読出し	*/
			_stprintf(l_tszKey, LnPtThickness, iLine + 1, iPoint + 1);
			_stprintf_s(l_tszTemp, _countof(l_tszTemp), _T("%.3f"), lg_StressParamInfo.Line[iLine].dPtThick[iPoint]);
			if(0 == WritePrivateProfileString(l_tszSec, l_tszKey,l_tszTemp,szFilePath))
				return FALSE;
		}
	}
//	BackupRecipe(szFilePath);
	return TRUE;
}

BOOL LoadStressParam(LPVOID pVoid, LPCSTR lpszName, int iType)
{
	char szFilePath[_MAX_PATH];
	SYSTEMTIME lastWriteSystemTime;

	ClearMemory();
	memset(&lg_StressParamInfo, NULL, sizeof(STRESS_PARAM_INFO));

//	sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
	switch(iType){
	case RECIPE_FILE_STRESS_MAIN_RECIPE:
		sprintf_s(szFilePath, _MAX_PATH, DB_MAIN_RECIPE_DIR "%s" MAINRECIPE_EXT, lpszName);
		break;
	case RECIPE_FILE_DEF_STRESS_MAIN_RECIPE:
		sprintf_s(szFilePath, _MAX_PATH, CFG_USER_DIR "%s" DAT_EXT, lpszName);
		break;
	default:
		return FALSE;
		break;
	}

	AddAbsPath(szFilePath);
	// ファイルが存在しない場合
	if(!FILEEXIST(szFilePath, &lastWriteSystemTime)){
		return FALSE;
	}

	/* 応力測定の使用する膜厚値のタイプを取得 */
	TCHAR l_tszSec[64] = StressParamSec;
	TCHAR l_tszKey[64] = PriorityKey;
	TCHAR l_tszTemp[64];

	if(0 == ::GetPrivateProfileString(l_tszSec, l_tszKey, _T("FALSE"), l_tszTemp, sizeof(l_tszTemp), szFilePath))
		return FALSE;

	lg_StressParamInfo.bPriority = (0 == _tcscmp(l_tszTemp, _T("TRUE")))? TRUE : FALSE;

	//if(lg_StressParamInfo.bPriority == TRUE)
	//{
		/* ストレス設定を読出し：ライン数	*/
		STRESS_CONFIG l_StressConfig;
		ConfigFile_GetNanoSpecIni(&l_StressConfig, CONFIG_FILE_STRESS_CONFIG);
		DWORD l_dwLineNum = l_StressConfig.dwLiftPinNumberOfLine;
		if (STRESS_LINES_MAX < l_dwLineNum) {
			l_dwLineNum = STRESS_LINES_MAX;
		}
  DWORD iLine;
		for (iLine = 0; iLine < l_dwLineNum; iLine++) {
			for (DWORD iPoint = 0; iPoint < PIN_INTERVAL; iPoint++) {
				/* 固定膜厚値を読出し	*/
				_stprintf(l_tszKey, LnPtThickness, iLine + 1, iPoint + 1);
				if(0 == ::GetPrivateProfileString(l_tszSec, l_tszKey, _T("0.0"), l_tszTemp, sizeof(l_tszTemp), szFilePath))
					return FALSE;
				lg_StressParamInfo.Line[iLine].dPtThick[iPoint] = atof(l_tszTemp);
			}
		}
	//}
	memcpy(pVoid, &lg_StressParamInfo, sizeof(STRESS_PARAM_INFO));
	return TRUE;
}

//Saiki 20110208 Add <-----