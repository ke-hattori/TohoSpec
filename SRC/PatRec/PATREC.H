#if !defined( _PATREC_H_ )
#define _PATRECDLL_

#pragma once

#include "..\\..\\INC\\patrec.hxx"
#include "..\\..\\INC\\overlay.hxx"
#include "..\\..\\INC\\configfile.hxx"

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\LIB\\dtnsoverlay.lib")
#pragma comment(lib, "..\\..\\LIB\\dtnsconfigfile.lib")
#else
#pragma comment(lib, "..\\..\\LIB\\tnsoverlay.lib")
#pragma comment(lib, "..\\..\\LIB\\tnsconfigfile.lib")
#endif

#ifndef countof
	#define countof(array) (sizeof(array)/sizeof(array[0]))
#endif

class CPatRec
{
public:
	CPatRec() : m_lpBuffer(NULL) { ConfigFile_GetNanoSpecIni(&m_patternRec, CONFIG_FILE_SR_PATTERN_REC); };
	virtual ~CPatRec() {};

	virtual BOOL Teach(LPRECT lpRect) { return FALSE; }
	virtual BOOL SaveModel(PSTR pszModelName) { return FALSE; }
	virtual BOOL GetModel(PSTR pszModelName) { return FALSE; }
	virtual BOOL Match(LPDESKEW_RESULT lpDeskewResult, BOOL bSaveLayer, int iCurrLayer) { return FALSE; };
	virtual BOOL DrawModel(HWND hWnd, WORD wSite, PSTR pszModelName) { return FALSE; }
	virtual BOOL RestoreModel(PSTR pszModelName) { return FALSE; }

	virtual BOOL Init() { return FALSE; }
	virtual BOOL End() { return FALSE; }

protected:
	inline void Halt(void) {
		//OverlayPreview(FALSE);
	}
	inline void Continuous(void) {
		//OverlayPreview(TRUE);
	}
#if !defined( __MYGRAYSCALE )
	LPVOID m_lpBuffer;
#else
	LPBYTE m_lpBuffer;
	LPBYTE m_lpTempBuffer;
#endif
	SR_PATTERN_REC m_patternRec;
};

class CPatRecMil80 : public CPatRec
{
public:
	CPatRecMil80();
	virtual ~CPatRecMil80();

	virtual BOOL Teach(LPRECT lpRect);
	virtual BOOL SaveModel(PSTR pszModelName);
	virtual BOOL GetModel(PSTR pszModelName);
	virtual BOOL Match(LPDESKEW_RESULT lpDeskewResult, BOOL bSaveLayer, int iCurrLayer);

	virtual BOOL DrawModel(HWND hWnd, WORD wSite, PSTR pszModelName);
	virtual BOOL RestoreModel(PSTR pszModelName);

	virtual BOOL Init(void);
	virtual BOOL End(void);

// matsuhisa 2009.12.09 modified >>>
//private:
protected:
// matsuhisa 2009.12.09 modified <<<
	BOOL ImageGrab(void);
	BOOL SaveLayer(BOOL bSaveLayer, int iCurrLayer, int iRepeatNum);

	MIL_ID	MilApplication;
	MIL_ID	MilSystem;		// system identifier
	MIL_ID	MilDisplay;
	MIL_ID	MilImage;		// image buffer identifier
	MIL_ID	MilSubImage;	// image in which model will be searched
	MIL_ID	MilGrayImage;
	MIL_ID	MilModel;		// model identifier
	MIL_ID	MilResult;		// result identifier
	MIL_ID	MilDeskewSite[DESKEW_SITE_MAX];
	MIL_ID	MilGraphic;
};

// matsuhisa 2009.12.09 added >>>
class GeometricModelFinder : public CPatRecMil80
{
public:
	GeometricModelFinder();
	virtual ~GeometricModelFinder();

	virtual BOOL Teach(LPRECT lpRect);
	virtual BOOL GetModel(PSTR pszModelName);
	virtual BOOL Match(LPDESKEW_RESULT lpDeskewResult, BOOL bSaveLayer, int iCurrLayer);

	virtual BOOL End(void);
};

class CPatMatchRec : public CPatRecMil80
{
public:
	CPatMatchRec();
	virtual ~CPatMatchRec();

	virtual BOOL Teach(LPRECT lpRect);
	virtual BOOL GetModel(PSTR pszModelName);
	virtual BOOL Match(LPDESKEW_RESULT lpDeskewResult, BOOL bSaveLayer, int iCurrLayer);

	virtual BOOL End(void);
};
// matsuhisa 2009.12.09 added <<<
#endif // _PATREC_H_
