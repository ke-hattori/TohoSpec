// MotSysDef.h : MOTSYS アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_MOTSYS_H__ABC015F8_34CD_435C_9866_D0AFD61A64C1__INCLUDED_)
#define AFX_MOTSYS_H__ABC015F8_34CD_435C_9866_D0AFD61A64C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMotSysApp:
// このクラスの動作の定義に関しては MotSys.cpp ファイルを参照してください。
//

class CMotSysApp : public CWinApp
{
public:
	HANDLE m_hMutex_Run;
	CMotSysApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotSysApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMotSysApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//		  この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTSYS_H__ABC015F8_34CD_435C_9866_D0AFD61A64C1__INCLUDED_)
