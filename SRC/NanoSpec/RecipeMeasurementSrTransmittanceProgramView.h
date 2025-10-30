#if !defined(AFX_RECIPEMEASUREMENTSRTRANSMITTANCEPROGRAMVIEW_H__D66A7569_094C_4AB4_B86E_6D8E966F828E__INCLUDED_)
#define AFX_RECIPEMEASUREMENTSRTRANSMITTANCEPROGRAMVIEW_H__D66A7569_094C_4AB4_B86E_6D8E966F828E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecipeMeasurementSrTransmittanceProgramView.h : ヘッダー ファイル
//

// ##########################################################################
// CRecipeMeasurementSrTransmittanceProgramView フォーム ビュー
// ##########################################################################

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "RecipeMeasurementSrReflectanceProgramView.h"

class CRecipeMeasurementSrTransmittanceProgramView : public CRecipeMeasurementSrReflectanceProgramView
{
protected:
	CRecipeMeasurementSrTransmittanceProgramView();			// 動的生成に使用されるプロテクト コンストラクタ。
	DECLARE_DYNCREATE(CRecipeMeasurementSrTransmittanceProgramView)

// インプリメンテーション
protected:
	virtual ~CRecipeMeasurementSrTransmittanceProgramView();
	virtual void LoadRecipeData();
	virtual BOOL SaveRecipeData();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RECIPEMEASUREMENTSRTRANSMITTANCEPROGRAMVIEW_H__D66A7569_094C_4AB4_B86E_6D8E966F828E__INCLUDED_)
