// RecipeStageProgramMeasurementPointMap.h : ヘッダー ファイル
//

#pragma once

// ##########################################################################
// RecipeStageProgramMeasurementPointMap
// ##########################################################################

#include "GridCell.h"

class CRecipeStageProgramMeasurementPointGrid : public CGridCtrl
{
public:
	BOOL GridInit(int iRows);
	void GridDisp(int row, int col, LPCTSTR pszText, DWORD TextStyle);
	void GridEditable(BOOL Editable = TRUE);
};
