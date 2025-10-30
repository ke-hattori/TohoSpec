#if !defined(AFX_CHIFTRANSIDESKEW_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
#define AFX_CHIFTRANSIDESKEW_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChifTransiDeskew.h : ヘッダー ファイル
//

#include <afxmt.h>	// CEvent と CSingleLock で必要です

// 2009.03.18 K.Matsuo Add -->
class CDeskewSeq;
// 2009.03.18 K.Matsuo Add <--

//--------------------------------------------------------------------------//
//	状態定義
//--------------------------------------------------------------------------//
#define	_ST_DSKW_DISP(x)	_T(#x)
#define	ST_DSKW_DISP(x)	_ST_DSKW_DISP(x)
#define	ST_DSKW_(x)		ST_DSKW_##x
// 状態の名称を定義→
//		ここを追加削除した場合は ST_xxx_n の番号に対応した
//		下記の ST_xxx_n と ST_xxx_DISP_n も合わせてください．
#define	ST_DSKW_0		ST_DSKW_(INIT)			// 初期状態
#define	ST_DSKW_1		ST_DSKW_(IDLE)			// アイドル
#define	ST_DSKW_2		ST_DSKW_(START)			// 開始前処理							(WAIT 状態無し)
#define	ST_DSKW_3		ST_DSKW_(AF)			// AF 完了待ち
#define	ST_DSKW_4		ST_DSKW_(AF_DONE)		// AF 完了								(WAIT 状態無し)
#define	ST_DSKW_5		ST_DSKW_(AUTO)			// オートデスキュー完了待ち
#define	ST_DSKW_6		ST_DSKW_(AUTO_OK)		// オートデスキュー OK 完了				(WAIT 状態無し)
#define	ST_DSKW_7		ST_DSKW_(AUTO_PROCEED)	// オートデスキュー失敗のまま続行		(WAIT 状態無し)
#define	ST_DSKW_8		ST_DSKW_(AUTO_CANCEL)	// オートデスキューレシピ終了			(WAIT 状態無し)
// 2009.04.10 K.Matsuo Change -->
#define	ST_DSKW_9		ST_DSKW_(AUTO_PROCEED_PMA)	// オートデスキュー失敗のまま続行・位置補正付き測定		(WAIT 状態無し)
#define	ST_DSKW_10		ST_DSKW_(MANU)			// マニュアルデスキュー完了待ち
#define	ST_DSKW_11		ST_DSKW_(MANU_OK)		// マニュアルデスキュー OK 完了			(WAIT 状態無し)
#define	ST_DSKW_12		ST_DSKW_(MANU_CANCEL)	// マニュアルデスキューレシピ終了		(WAIT 状態無し)
#define	ST_DSKW_13		ST_DSKW_(MANU_NONE)		// マニュアルデスキューなしで続行		(WAIT 状態無し)
// 2009.04.10 K.Matsuo Change <--
// 状態の名称を定義←
enum STATES_DESKEW
{
	// 構築時は０になっています．
	// ので，最初は０として定義された状態です．
	// しかし，０の状態に対応する関数は自動的に実行されませんので，
	// 必要があれば InitInstance() などから実行する必要があります．

	// ここを変更した場合は直後にあるステータス文字列の定義も合わせてください．

	ST_DSKW_0 = 0,
	ST_DSKW_1,
	ST_DSKW_2,
	ST_DSKW_3,
	ST_DSKW_4,
	ST_DSKW_5,
	ST_DSKW_6,
	ST_DSKW_7,
	ST_DSKW_8,
	ST_DSKW_9,
	ST_DSKW_10,
	ST_DSKW_11,
	ST_DSKW_12,
// 2009.04.10 K.Matsuo Add -->
	ST_DSKW_13,
// 2009.04.10 K.Matsuo Add <--
};
const LPSTR STATES_DESKEW_DISP[] = {
	ST_DSKW_DISP(ST_DSKW_0),
	ST_DSKW_DISP(ST_DSKW_1),
	ST_DSKW_DISP(ST_DSKW_2),
	ST_DSKW_DISP(ST_DSKW_3),
	ST_DSKW_DISP(ST_DSKW_4),
	ST_DSKW_DISP(ST_DSKW_5),
	ST_DSKW_DISP(ST_DSKW_6),
	ST_DSKW_DISP(ST_DSKW_7),
	ST_DSKW_DISP(ST_DSKW_8),
	ST_DSKW_DISP(ST_DSKW_9),
	ST_DSKW_DISP(ST_DSKW_10),
	ST_DSKW_DISP(ST_DSKW_11),
	ST_DSKW_DISP(ST_DSKW_12),
// 2009.04.10 K.Matsuo Add -->
	ST_DSKW_DISP(ST_DSKW_13),
// 2009.04.10 K.Matsuo Add <--
} ;

//--------------------------------------------------------------------------//
//	イベント定義
//--------------------------------------------------------------------------//
enum EVENTS_DESKEW
{
	EV_DSKW_DO_INIT = 0,		// 初期化開始
	EV_DSKW_INIT_DONE,			// 初期化終了
	EV_DSKW_IDLE,				// アイドルへ
	EV_DSKW_START,				// デスキュー開始
	EV_DSKW_MANU,				// マニュアルデスキュー開始
	EV_DSKW_AF,					// AF 開始
	EV_DSKW_AF_DONE,			// AF 完了
	EV_DSKW_AF_OK,				// AF OK
	EV_DSKW_AF_NG,				// AF NG
	EV_DSKW_AF_CANCEL,			// AF ポップアップ キャンセルボタン
// 2009.03.11 K.Matsuo Add -->
	EV_DSKW_AUTO,				// オートデスキュー開始
// 2009.03.11 K.Matsuo Add <--
	EV_DSKW_AUTO_OK,			// オートデスキュー OK 完了
	EV_DSKW_AUTO_PROCEED,		// オートデスキュー NG(続行)
	EV_DSKW_AUTO_CANCEL,		// オートデスキュー NG(レシピ終了)
	EV_DSKW_AUTO_MANU,			// オートデスキュー NG(マニュアルデスキュー)
// 2009.04.10 K.Matsuo Add -->
	EV_DSKW_AUTO_PROCEED_PMA,	// オートデスキュー NG(続行・位置補正付き測定)
// 2009.04.10 K.Matsuo Add -->
	EV_DSKW_MANU_OK,			// マニュアルデスキュー OK 完了
	EV_DSKW_MANU_CANCEL,		// マニュアルデスキューキャンセル
	EV_DSKW_MANU_NONE,			// マニュアルデスキューしない
};

/*
 *	マクロ定義
 */
// デスキュー終了ステータス
#define	TR_DSKW_OK				0			// デスキュー OK
#define	TR_DSKW_TRAF_BUSY		0xffff0401	// AF トランジションがビジーでした
#define	TR_DSKW_AF_CANCEL		0xffff0402	// AF ポップアップでキャンセルされた
#define	TR_DSKW_AUTO_PROCEED	0xffff0411	// オートデスキュー デスキュー失敗のまま続行
#define	TR_DSKW_AUTO_CANCEL		0xffff0412	// オートデスキュー レシピ終了(キャンセル)
#define	TR_DSKW_MANU_PROCEED	0xffff0421	// マニュアルデスキュー デスキュー無しで続行
#define	TR_DSKW_MANU_CANCEL		0xffff0422	// マニュアルデスキュー レシピ終了(キャンセル)

/////////////////////////////////////////////////////////////////////////////
// CChiefTransiDeskew スレッド

class CChiefTransiDeskew : public CWinThread, public cActive
{
	DECLARE_DYNCREATE(CChiefTransiDeskew)
public:
	CChiefTransiDeskew(CWnd* pParent = NULL);	// 動的生成に使用されるプロテクト コンストラクタ
protected:

// アトリビュート
public:

// オペレーション
public:
	DWORD m_dwHeartBeat;
	void HeartBeatPulse(void);
	void CancelDeskew(void);
	void TransiEvent(const int iEventNo, const cEventParams* const pEvParams = 0);
	int GetCurrentState(void);
	bool IsIdle();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChiefTransiDeskew)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CChiefTransiDeskew();

	// 状態別の処理関数
	void enter_ST_DSKW_INIT(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_IDLE(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_START(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_AF(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_AF_DONE(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_AUTO(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_AUTO_OK(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_AUTO_PROCEED(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_AUTO_CANCEL(const cEventParams* const pEvParams = 0);
// 2009.04.10 K.Matsuo Add -->
	void enter_ST_DSKW_AUTO_PROCEED_PMA(const cEventParams* const pEvParams = 0);
// 2009.04.10 K.Matsuo Add <--
	void enter_ST_DSKW_MANU(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_MANU_OK(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_MANU_CANCEL(const cEventParams* const pEvParams = 0);
	void enter_ST_DSKW_MANU_NONE(const cEventParams* const pEvParams = 0);

	DECLARE_STATEPROC_MAP(CChiefTransiDeskew);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CChiefTransiDeskew)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	afx_msg void OnTransiEvent(WPARAM wparam, LPARAM lparam);
	afx_msg void OnHeartBeat(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
private:
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- { ---------- */
	void NotifyComplete(const cEventParams *const pEvParams);
	WORD m_wHeadPre;
/* added 2009.11.26 hmenjo 測定 Seq は指定ヘッドで動作 ---------- } ---------- */
	CView* m_pcChiefView;
	CEvent m_cSyncEvent;
// 2009.03.18 K.Matsuo Add -->
	CDeskewSeq* m_pDeskewSeq;
// 2009.03.18 K.Matsuo Add <--
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CHIFTRANSIDESKEW_H__44228793_B4D9_4104_BE5A_A9C693AC1349__INCLUDED_)
