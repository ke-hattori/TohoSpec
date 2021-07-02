// GlassineDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Glassine.h"
#include "GlassineDlg.h"
#include "GlassineDLL.h"
// Goto add 2007.0409 --->
//#include <LogManager.hxx>
// Goto add 2007.0409 <---

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define WM_USER_HOTKEY_EXIT		1101
//#define _WIN32_WINNT			0x0500
//#define WS_EX_LAYERED	0x00080000
//#define LWA_COLORKEY	0x00000001
//#define LWA_ALPHA		0x00000002L

KeyLock TT;
int Rate=5;
SETLAYER	*SetLayeredWindowAttributes;

/////////////////////////////////////////////////////////////////////////////
// CGlassineDlg ダイアログ

CGlassineDlg::CGlassineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlassineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGlassineDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlassineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlassineDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGlassineDlg, CDialog)
	//{{AFX_MSG_MAP(CGlassineDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlassineDlg メッセージ ハンドラ

BOOL CGlassineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// ウィンドウを最前面にする
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

//////////////////////////////////////////////////////////////
	if(OnShowcmdline())
	{
		SetLayered(this->GetSafeHwnd(),Rate);
		ResetWinPos();
		::SetForegroundWindow(this->GetSafeHwnd());
		TT.Set();
	}
	else
	{
		OnCancel();
	}
//////////////////////////////////////////////////////////////

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CGlassineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CGlassineDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/////////////////////////////////////////////////////////
BOOL CGlassineDlg::SetLayered(HWND hWnd,int Pos)
{
	HINSTANCE	hDllInst;

	hDllInst = LoadLibrary("user32.dll");
	if( hDllInst == NULL ){
//		MessageBox( "USER32.DLLが読み込めませんでした。","Error", MB_OK|MB_ICONSTOP );
//		return( FALSE );
		ASSERT(FALSE);
	}
	GetModuleHandle("user32.dll");
	SetLayeredWindowAttributes =
		(SETLAYER *)GetProcAddress( hDllInst, "SetLayeredWindowAttributes" );
	if( SetLayeredWindowAttributes == NULL ){
//		MessageBox( "SetLayeredWindowAttributes()関数のポインタが取得できませんでした。Windows2000ではない可能性があります。","Error", MB_OK );
//		return( FALSE );
		ASSERT(FALSE);
	}

	long lStyle;

	lStyle = GetWindowLong( hWnd, GWL_EXSTYLE );
	lStyle |= 0x00080000;
	SetWindowLong( hWnd, GWL_EXSTYLE, lStyle );

// Goto change 2007.0703 --->
//	SetLayeredWindowAttributes(hWnd, 0, Pos, 2 );
	SetLayeredWindowAttributes(hWnd, 0, 255 * 30 / 100, 2);
// Goto change 2007.0703 <---
	FreeLibrary( hDllInst );
   return TRUE;
}

///////////////////////////////////////////////////////////////
BOOL CGlassineDlg::OnShowcmdline()
{
  int intArgC;
  intArgC = __argc;

  if(!(intArgC==2))
  {
	return FALSE;
  }
  int res = atoi(& (__argv[1])[2]);
  switch(res)
  {
	  case 1:
		  Rate = 5;
		  break;
	  case 2:
		  Rate = 75;
		  break;
	  case 3:
		  Rate = 125;
		  break;
	  case 4:
		  Rate = 175;
		  break;
	  case 5:
		  Rate = 255;
		  break;
	  default:
		  return FALSE;
  }
	return TRUE;
}

////////////////////////////////////////////////////////////////
void CGlassineDlg::ResetWinPos()
{
	HWND hwnd = this->GetSafeHwnd();
	int width  = GetSystemMetrics (SM_CXSCREEN);		// width of screen
	int height = GetSystemMetrics (SM_CYSCREEN);		// height

	::SetWindowPos(
	hwnd,
	0,
	-100,						//Left
	-100,						//Top
	width+200,					//Width
	height+200,					//Height
	SWP_NOACTIVATE |
	SWP_NOZORDER);

}

////////////////////////////////////////////////////////////////
void CGlassineDlg::OnCancel()
{
// Goto add 2007.0409 --->
//	  OperationLogging(_T("Push [InterLock] DLG - [Cancel] button"));
// Goto add 2007.0409 <---
	TT.Release();

// 2009.06.08 K.Matsuo -->
	// Screen InterlockをALT+F6で、中央のダイアログが消えずに残ってしまうことへの対応
	// Findwindowの名前検索では、大文字・小文字は区別されないので、
	// 同名だったGlassineDlg名を変更しました
	CWnd* pWnd = CWnd::FindWindow(NULL, "INTERLOCK");
	if ( pWnd )
		pWnd->ShowWindow(SW_HIDE);
// 2009.06.08 K.Matsuo <--
	CDialog::OnCancel();
}

BOOL CGlassineDlg::PreTranslateMessage(MSG* pMsg)
{
	BOOL SA = FALSE;
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if(pMsg->wParam == VK_F6)
			OnCancel();
	}
	return CDialog::PreTranslateMessage(pMsg);

}
