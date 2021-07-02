#include "stdafx.h"
#include "../../INC/ConfigFile.hxx"
#include "ScanRange.h"

// --------------------------------------------------------------------------
// CScanRange
CScanRange::CScanRange(const MEAS_PROG_INFO* pMeasProgInfo)
	: m_iScanStart(0), m_iScanEnd(0), m_bUvMeasure(FALSE), m_bVisMeasure(FALSE)
{
	TRACE(_T("CScanRange::CScanRange()\n"));

	const SR_SCAN_INFO& srScanInfo = pMeasProgInfo->ScanParams._SR;

	SR_CONFIG srConfig;
	ConfigFile_GetNanoSpecIni(&srConfig, CONFIG_FILE_SR_CONFIG);

	// UvMeasure  指定波長がMIN_VS_SCANL（可視光線下限）未満
	// VisMeasure 指定波長がMIN_VS_SCANL（可視光線下限）より大きい
	if ( IsDiscrete(pMeasProgInfo->ScanParams.hdr.wScanType) ) {
		for ( int i = 0; i < SPECIFIED_WAVELENGTH_MAX; i++ ) {
			if ( srScanInfo.iSpecificWavelen[i] < MIN_UV_SCAN )
				continue;
			if ( srScanInfo.iSpecificWavelen[i] < MIN_VS_SCANL )
				m_bUvMeasure = TRUE;
			if( srScanInfo.iSpecificWavelen[i] > MIN_VS_SCANL )
				m_bVisMeasure = TRUE;
		}
	}
	else {
		if ( srScanInfo.WavelenRange.wStart < MIN_VS_SCANL )
			m_bUvMeasure = TRUE;
		if ( srScanInfo.WavelenRange.wEnd > MIN_VS_SCANL )
			m_bVisMeasure = TRUE;
	}

	// 測定波長の開始・終了を設定
	if ( m_bUvMeasure ) {
		m_iScanStart = MIN_UV_SCAN;
		if ( !m_bVisMeasure )
			m_iScanEnd = MIN_VS_SCANL;
		else
			m_iScanEnd = MAX_CS_SCAN;
	}
	else {
		m_iScanStart = MIN_VS_SCANL;
		if ( srConfig.nHeadType == 5 )	/* SR_HEAD_TYPE_LAH1024_SS */
			m_iScanEnd = MAX_SS_SCAN;
		else
			m_iScanEnd = MAX_CS_SCAN;
	}
}

// --------------------------------------------------------------------------
// ~CScanRange
CScanRange::~CScanRange()
{
	TRACE(_T("CScanRange::~CScanRange()\n"));
}

//---------------------------------------------------------------------------
// IsDiscrete
BOOL CScanRange::IsDiscrete(WORD wScanType)
{
	BOOL bRet = FALSE;

	switch ( wScanType )
	{
//	case MEAS_PROG_TYPE_SR_THICKNESS:
	case MEAS_PROG_TYPE_SR_REFLECTANCE:
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE:
	case MEAS_PROG_TYPE_SR_TRANSMITTANCE_G:
		bRet = TRUE;
		break;
	}
	return bRet;
}

