#if !defined(AFX_CHIFTRANSIAF_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
#define AFX_CHIFTRANSIAF_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiAF.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

//--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------//
#define	_ST_AAF_DISP(x)	_T(#x)
#define	ST_AAF_DISP(x)	_ST_AAF_DISP(x)
#define	ST_AAF_(x)		ST_AAF_##x
// 状態の名称を定義→
//		ここを追加削除した場合は ST_xxx_n の番号に対応した
//		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．
#define	ST_AAF_0		ST_AAF_(INIT)			// 初期状態
#define	ST_AAF_1		ST_AAF_(IDLE)			// アイドル
#define	ST_AAF_2		ST_AAF_(AF)				// AF 完了待ち							(WAIT 状態無し)
#define	ST_AAF_3		ST_AAF_(POPUP)			// ポップアップ終了待ち
#define	ST_AAF_4		ST_AAF_(POPUP_RETRY)	// ポップアップ終了リトライボタン		(WAIT 状態無し)
#define	ST_AAF_5		ST_AAF_(POPUP_IGNORE)	// ポップアップ終了 AF 無視ボタン		(WAIT 状態無し)
#define	ST_AAF_6		ST_AAF_(POPUP_SKIP)		// ポップアップ終了スキップボタン		(WAIT 状態無し)
#define	ST_AAF_7		ST_AAF_(POPUP_CANCEL)	// ポップアップ終了キャンセルボタン		(WAIT 状態無し)
// 状態の名称を定義←
enum STATES_AAF
{
	// 構築時は０になっています．
	// ので，最初は０として定義された状態です．
	// しかし，０の状態に対応する関数は自動的に実行されませんので，
	// 必要があれば InitInstance() などから実行する必要があります．

	// ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．

	ST_AAF_0 = 0,
	ST_AAF_1,
	ST_AAF_2,
	ST_AAF_3,
	ST_AAF_4,
	ST_AAF_5,
	ST_AAF_6,
	ST_AAF_7,
};
const LPSTR STATES_AAF_DISP[] = {
	ST_AAF_DISP(ST_AAF_0),
	ST_AAF_DISP(ST_AAF_1),
	ST_AAF_DISP(ST_AAF_2),
	ST_AAF_DISP(ST_AAF_3),
	ST_AAF_DISP(ST_AAF_4),
	ST_AAF_DISP(ST_AAF_5),
	ST_AAF_DISP(ST_AAF_6),
	ST_AAF_DISP(ST_AAF_7),
};

//--------------------------------------------------------------------------//
//	イベント定義
//--------------------------------------------------------------------------//
enum EVENTS_AAF
{
	EV_AAF_DO_INIT = 0,			// 初期化開始
	EV_AAF_INIT_DONE,			// 初期化終了
	EV_AAF_IDLE,				// アイドルへ
	EV_AAF_START,				// オートフォーカス開始
	EV_AAF_OK,					// AF OK
	EV_AAF_RO_PROCEED,			// AF NG リトライオーバ自動強制測定
	EV_AAF_RO_SKIP,				// AF NG リトライオーバ自動スキップ
	EV_AAF_RO_CANCEL,			// AF NG リトライオーバキャンセル
	EV_AAF_RO_POPUP,			// AF NG リトライオーバポップアップ
	EV_AAF_POPUP_RETRY,			// ポップアップ リトライボタン
	EV_AAF_POPUP_IGNORE,		// ポップアップ AF 無視ボタン
	EV_AAF_POPUP_SKIP,			// ポップアップ スキップボタン
	EV_AAF_POPUP_CANCEL,		// ポップアップ キャンセルボタン
};

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiAF スレッド

class CChiefTransiAF : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiAF)
public:
	CChiefTransiAF(CWnd* pParent = NULL);	// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void CancelAF(void);
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefTransiAF)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefTransiAF();

	// 状態別の処理関数
	void enter_ST_AAF_INIT(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_IDLE(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_AF(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_POPUP(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_POPUP_RETRY(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_POPUP_IGNORE(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_POPUP_SKIP(const cEventParams* const pEvParams = 0);
	void enter_ST_AAF_POPUP_CANCEL(const cEventParams* const pEvParams = 0);

	DECLARE_STATEPROC_MAP(CChiefTransiAF);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefTransiAF)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCancelReq;
	DWORD m_dwAfMode;
	void NotifyComplete(const cEventParams* const pEvParams = 0);
	DWORD m_dwAFValid;
	DWORD m_dwParentTransi;
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFTRANSIAF_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
