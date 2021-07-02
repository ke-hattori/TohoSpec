#if !defined(AFX_CHIEFTHREAD_H__11D35FEF_FC3D_4836_A6BB_A8BD15C71E35__INCLUDED_)
#define AFX_CHIEFTHREAD_H__11D35FEF_FC3D_4836_A6BB_A8BD15C71E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChiefThread.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

/////////////////////////////////////////////////////////////////////////////
// CChiefThread スレッド

class CChiefThread : public CWinThread
{
	DECLARE_DYNCREATE(CChiefThread)
public:
	CChiefThread(CWnd* pParent = NULL);		// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
	CFrameWnd* m_pcMainFrame;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefThread();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefThread)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIEFTHREAD_H__11D35FEF_FC3D_4836_A6BB_A8BD15C71E35__INCLUDED_)
