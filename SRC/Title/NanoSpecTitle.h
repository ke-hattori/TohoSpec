// NanoSpecTitle.h : NANOSPECTITLE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_NANOSPECTITLE_H__97268DB5_7A04_42DE_89D2_4C031A9F9DF7__INCLUDED_)
#define AFX_NANOSPECTITLE_H__97268DB5_7A04_42DE_89D2_4C031A9F9DF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CNanoSpecTitleApp:
// このクラスの動作の定義に関しては NanoSpecTitle.cpp ファイルを参照してください。
//

class CNanoSpecTitleApp : public CWinApp
{
public:
	CNanoSpecTitleApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNanoSpecTitleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CNanoSpecTitleApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//		  この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- { ---------- */
	HANDLE m_hMutex_Run;
/* added 2009.06.05 hmenjo NanoSpecTitle 消えない対策 ---------- } ---------- */
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NANOSPECTITLE_H__97268DB5_7A04_42DE_89D2_4C031A9F9DF7__INCLUDED_)
