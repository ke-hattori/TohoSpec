// MotSysCfg.h : MOTSYSCFG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_MOTSYSCFG_H__CCEA29A5_9123_11D7_AE9A_0040266ED145__INCLUDED_)
#define AFX_MOTSYSCFG_H__CCEA29A5_9123_11D7_AE9A_0040266ED145__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMotSysCfgApp:
// このクラスの動作の定義に関しては MotSysCfg.cpp ファイルを参照してください。
//

class CMotSysCfgApp : public CWinApp
{
public:
	CMotSysCfgApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMotSysCfgApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMotSysCfgApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hMutex_Run;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MOTSYSCFG_H__CCEA29A5_9123_11D7_AE9A_0040266ED145__INCLUDED_)
