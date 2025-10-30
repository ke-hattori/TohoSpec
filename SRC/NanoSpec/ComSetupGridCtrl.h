// ComSetupGridCtrl.h : ヘッダー ファイル
//

#pragma once

// ##########################################################################
// ComSetupGridCtrl
// ##########################################################################

#include "GridCell.h"

class CComSetupGridCtrl : public CGridCtrl
{
public:
	BOOL GridInit(int iRows);
	void GridDisp(int row, int col, LPCTSTR pszText, DWORD TextStyle);
	void GridEditable(BOOL Editable = TRUE);
};
