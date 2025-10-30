// Procon.h : PROCON アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_PROCON_H__3671929E_8DEE_487B_B8B1_663322D16CF3__INCLUDED_)
#define AFX_PROCON_H__3671929E_8DEE_487B_B8B1_663322D16CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CProconApp:
// このクラスの動作の定義に関しては Procon.cpp ファイルを参照してください。
//

class CProconApp : public CWinApp
{
public:
	CProconApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CProconApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CProconApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PROCON_H__3671929E_8DEE_487B_B8B1_663322D16CF3__INCLUDED_)
