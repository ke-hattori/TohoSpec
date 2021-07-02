// Terminator.h : TERMINATOR アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_TERMINATOR_H__80603DD4_45CB_41E3_9837_D2A33AABE374__INCLUDED_)
#define AFX_TERMINATOR_H__80603DD4_45CB_41E3_9837_D2A33AABE374__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTerminatorApp:
// このクラスの動作の定義に関しては Terminator.cpp ファイルを参照してください。
//

class CTerminatorApp : public CWinApp
{
public:
	CTerminatorApp();
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTerminatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CTerminatorApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TERMINATOR_H__80603DD4_45CB_41E3_9837_D2A33AABE374__INCLUDED_)
