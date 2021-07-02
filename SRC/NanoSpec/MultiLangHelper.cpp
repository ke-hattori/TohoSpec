// MultiLangHelper.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "System.h"
#include "MultiLangHelper.h"

// =========================================================================
//
CMultiLangHelper::CMultiLangHelper()
{
	SYSTEM_CONFIG l_SystemConfig;
	ConfigFile_GetNanoSpecIni(&l_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	m_nLanguage = l_SystemConfig.nLanguage;
}

CMultiLangHelper::~CMultiLangHelper()
{
}

CString& CMultiLangHelper::operator[](int index)
{
	return GetItemText(index);
}


// =========================================================================
//
CMLScanType::CMLScanType(int iHeadType)
	: m_iHeadType(iHeadType)
{
}

CMLScanType::~CMLScanType()
{
}

int CMLScanType::GetItemCount()
{
	int iMax;

	switch(m_iHeadType){
	case HEAD_TYPE_SR:
		iMax = MEAS_PROG_TYPE_SR_MAX;
		break;
	case HEAD_TYPE_SE:
		iMax = MEAS_PROG_TYPE_SE_MAX;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
	case HEAD_TYPE_COMPEASE:
		iMax = MEAS_PROG_TYPE_COMPEASE_MAX;
		break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
	case HEAD_TYPE_IRSE:
		iMax = MEAS_PROG_TYPE_IRSE_MAX;
		break;
#else
	case HEAD_TYPE_MS:
		iMax = MEAS_PROG_TYPE_MS_MAX;
		break;
#endif
// 2009.10.19 bagus MS 追加 --}--
	case HEAD_TYPE_4PP:
		iMax = MEAS_PROG_TYPE_4PP_MAX;
		break;
	case HEAD_TYPE_CTA:
		iMax = MEAS_PROG_TYPE_CTA_MAX;
		break;
	case HEAD_TYPE_STRESS:
		iMax = MEAS_PROG_TYPE_STRESS_MAX;
		break;
	default:
		iMax = 0;
		break;
	}

	return iMax;
}

CString& CMLScanType::GetItemText(int index)
{
	const LPCTSTR* pszItem;

	if ( index >= GetItemCount() ) {
		ASSERT(FALSE);
		m_string.Empty();
		return m_string;
	}

	if ( m_nLanguage == LANGUAGE_ENGLISH ) {
		switch(m_iHeadType){
		case HEAD_TYPE_SR:
			pszItem = MEAS_PROG_TYPE_SR_ITEM_ENU;
			break;
		case HEAD_TYPE_SE:
			pszItem = MEAS_PROG_TYPE_SE_ITEM_ENU;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			pszItem = MEAS_PROG_TYPE_COMPEASE_ITEM_ENU;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			pszItem = MEAS_PROG_TYPE_IRSE_ITEM_ENU;
			break;
#else
		case HEAD_TYPE_MS:
			pszItem = MEAS_PROG_TYPE_MS_ITEM_ENU;
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			pszItem = MEAS_PROG_TYPE_4PP_ITEM_ENU;
			break;
		case HEAD_TYPE_CTA:
			pszItem = MEAS_PROG_TYPE_CONTACT_ANGLE_ITEM_ENU;
			break;
		case HEAD_TYPE_STRESS:
			pszItem = MEAS_PROG_TYPE_STRESS_ITEM_ENU;
			break;
		default:
			pszItem = NULL;
			break;
		}
	}
	else{
		switch(m_iHeadType){
		case HEAD_TYPE_SR:
			pszItem = MEAS_PROG_TYPE_SR_ITEM_JPN;
			break;
		case HEAD_TYPE_SE:
			pszItem = MEAS_PROG_TYPE_SE_ITEM_JPN;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 -->
		case HEAD_TYPE_COMPEASE:
			pszItem = MEAS_PROG_TYPE_COMPEASE_ITEM_JPN;
			break;
// 2013.02.01 bagus CompleteEASEヘッド追加 <--
// 2009.10.19 bagus MS 追加 --{--
#if 0
		case HEAD_TYPE_IRSE:
			pszItem = MEAS_PROG_TYPE_IRSE_ITEM_JPN;
			break;
#else
		case HEAD_TYPE_MS:
			pszItem = MEAS_PROG_TYPE_MS_ITEM_JPN;
			break;
#endif
// 2009.10.19 bagus MS 追加 --}--
		case HEAD_TYPE_4PP:
			pszItem = MEAS_PROG_TYPE_4PP_ITEM_JPN;
			break;
		case HEAD_TYPE_CTA:
			pszItem = MEAS_PROG_TYPE_CONTACT_ANGLE_ITEM_JPN;
			break;
		case HEAD_TYPE_STRESS:
			pszItem = MEAS_PROG_TYPE_STRESS_ITEM_JPN;
			break;
		default:
			pszItem = NULL;
			break;
		}
	}

	if ( pszItem ) {
		m_string = pszItem[index];
	}
	else {
		ASSERT(FALSE);
		m_string.Empty();
	}

	return m_string;
}


// =========================================================================
//
