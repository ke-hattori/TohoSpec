// MessageDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nanospec.h"
#include "MainFrm.h"
#include "MessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg ダイアログ


CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_MessageMax = 0;
}


void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageDlg)
	DDX_Control(pDX, IDC_MESSAGE_STRING, m_ctlMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg メッセージ ハンドラ

// ==========================================================================
//
BOOL CMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ShowWindow(SW_HIDE);		// 起動時は非表示

	// StringListの読み込み
	MessageListLoading();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
				  // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// ==========================================================================
//
void CMessageDlg::MessageListLoading()
{
	int pos, posCR;
	int len, pt;
	CStdioFile File;
	CString sBuff;
	char pstr[3];
	char szFilePath[MAX_PATH];

	pstr[0] = 0x5c;	pstr[1] = 0x6e;	pstr[2] = 0;

	///// Load System Config File /////
	SYSTEM_CONFIG m_SystemConfig;
	ConfigFile_GetNanoSpecIni(&m_SystemConfig, CONFIG_FILE_SYSTEM_CONFIG);

	if( m_SystemConfig.nLanguage == 1 ){	// 日本語
		// recipe data file open
		sprintf(szFilePath, "%s%s", g_szCfg_Dir, MESSAGE_FILE_NAME_J);
		if(!File.Open(szFilePath, CFile::modeRead | CFile::typeText)){
			return;
		}
	}
	else{									// 英語
		// recipe data file open
		sprintf(szFilePath, "%s%s", g_szCfg_Dir, MESSAGE_FILE_NAME_E);
		if(!File.Open(szFilePath, CFile::modeRead | CFile::typeText)){
			return;
		}
	}

	while(File.ReadString(sBuff) && m_MessageMax < MESSAGE_LIST_MAX){

		pos = 0;

		///// Invalidate /////
		if(sBuff == ""){
			continue;
		}

		if(*sBuff.GetBuffer(0) == '/'){
			continue;
		}

		///// ALID /////
		m_MessageList[m_MessageMax].Id = atoi(sBuff.GetBuffer(0));

		///// Text /////
		pos = sBuff.Find(",", pos) + 1;
		len = 0; pt = 0;
		while( 1 ){
			if( (posCR = sBuff.Find(pstr, pos)) != -1 ){
				len = posCR - pos;
				strcpy(&m_MessageList[m_MessageMax].Text[pt], sBuff.Mid(pos, len).GetBuffer(0));
				pos += len;		pt += len;
				m_MessageList[m_MessageMax].Text[pt] = '\r';
				pos ++;			pt ++;
				m_MessageList[m_MessageMax].Text[pt] = '\n';
				pos ++;			pt ++;
			}
			else{
				strcpy(&m_MessageList[m_MessageMax].Text[pt], sBuff.Mid(pos, 80).GetBuffer(0));
				break;
			}
		}

		///// Next Alarm /////
		m_MessageMax++;
	}

	File.Close();
}

// ==========================================================================
//
void CMessageDlg::SetCaption(LPCTSTR pszCaption)
{
	SetWindowText(pszCaption);
}

// ==========================================================================
//
void CMessageDlg::SetMessage(int Id)
{
	int Index = MessageSearchIndex(Id);

	if( Index != -1 ){
		m_ctlMessage.SetWindowText(m_MessageList[Index].Text);
		CenterWindow();
	}
}

//Saiki 20090530 Add ----->
// ==========================================================================
//
void CMessageDlg::SetMessageString(CString strBuffer)
{
	m_ctlMessage.SetWindowText(strBuffer);
	CenterWindow();
}
//Saiki 20090530 Add <-----

// ==========================================================================
//
int CMessageDlg::MessageSearchIndex(int Id)
{
	int Index;

	for(Index = 0; Index < m_MessageMax; Index++){
		if(m_MessageList[Index].Id == Id) return Index;
	}

	return -1;
}
