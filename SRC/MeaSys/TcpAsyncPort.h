#pragma once

#include "Winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncPort スレッド

class CTcpAsyncPort : public CWinThread
{
	DECLARE_DYNCREATE(CTcpAsyncPort)
protected:
	CTcpAsyncPort();           // 動的生成に使用されるプロテクト コンストラクタ

// アトリビュート
public:

// オペレーション
public:
	virtual ~CTcpAsyncPort();
	void Quit(void){m_Run = FALSE;};

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTcpAsyncPort)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTcpAsyncPort)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BOOL m_Run;
};
