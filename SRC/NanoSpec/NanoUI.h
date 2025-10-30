// NanoUI.h : ヘッダー ファイル
//

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "stdafx.h"
#include "System.h"
#include "NanoSpec.h"
#include "MySplitterWnd.h"
#include "MainFrm.h"


// =========================================================================
//
class CNanoUI : public CFormView
{
	DECLARE_DYNAMIC(CNanoUI)

public:
	CNanoUI(LPCTSTR lpszTemplateName);
	CNanoUI(UINT nIDTemplate);
	virtual ~CNanoUI();
	virtual BOOL Exit();

protected:
	virtual void FontCreation(int nHeight= 12);

public:
	CFont m_Font;
};
