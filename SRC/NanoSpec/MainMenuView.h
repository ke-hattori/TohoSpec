#if !defined(AFX_MAINMENUVIEW_H__87441AF2_F73F_4959_8859_3A6131545077__INCLUDED_)
#define AFX_MAINMENUVIEW_H__87441AF2_F73F_4959_8859_3A6131545077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainMenuView.h : ï¿½wï¿½bï¿½_ï¿½[ ï¿½tï¿½@ï¿½Cï¿½ï¿½
//

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "NanoUI.h"

/////////////////////////////////////////////////////////////////////////////
// Define                                                                  //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
#define MAIN_MENU_VIEW_LABEL_MAX		(200)
#define MAIN_MENU_VIEW_BUTTON_MAX		(200)

/////////////////////////////////////////////////////////////////////////////
// CMainMenuView ï¿½tï¿½Hï¿½[ï¿½ï¿½ ï¿½rï¿½ï¿½ï¿½[                                           //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
class CMainMenuView : public CNanoUI
{
protected:
	CMainMenuView();			// ï¿½ï¿½ï¿½Iï¿½ï¿½ï¿½ï¿½ï¿½É�gï¿½pï¿½ï¿½ï¿½ï¿½ï¿½vï¿½ï¿½ï¿½eï¿½Nï¿½g ï¿½Rï¿½ï¿½ï¿½Xï¿½gï¿½ï¿½ï¿½Nï¿½^ï¿½B
	DECLARE_DYNCREATE(CMainMenuView)

// ï¿½tï¿½Hï¿½[ï¿½ï¿½ ï¿½fï¿½[ï¿½^
public:
	//{{AFX_DATA(CMainMenuView)
	enum { IDD = IDD_MAIN_MENU_FORM };
	//}}AFX_DATA

// ï¿½Aï¿½gï¿½ï¿½ï¿½rï¿½ï¿½ï¿½[ï¿½g
public:

// ï¿½Iï¿½yï¿½ï¿½ï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
public:
	///// Variable //////////////////////////////////////////////////////////
	CMainFrame* m_pMainFrame;
	CNanoSpecDoc* m_pDoc;

// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) -->
	SYSTEM_CONFIG m_SystemConfig;
// 2014.01.17 Bagus Add (Stage Noneï¿½Î‰ï¿½) <--

	CBrush m_Brush_UserName;
	CBrush m_Brush_HostMode;
	CBrush m_Brush_ProcessStatus;
	CBrush m_Brush_AlarmStatus;
	CBrush m_Brush_WarningStatus;

	int m_MainMenu;

	CShadeButtonST m_MainMenuMeasurementButton;
	CShadeButtonST m_MainMenuDataButton;
	CShadeButtonST m_MainMenuRecipeSettingButton;
	CShadeButtonST m_MainMenuAlarmHistoryButton;
	CShadeButtonST m_MainMenuLogOnButton;

	BOOL m_Disp[MAIN_MENU_VIEW_LABEL_MAX];
	BOOL m_Button[MAIN_MENU_VIEW_BUTTON_MAX];

	///// Function //////////////////////////////////////////////////////////
	void RefreshDlg(BOOL bInit = FALSE);
	void ControlColor(CDC* pDC, CBrush* pBrush, COLORREF Color);
	void ChangeHostMode(int iMode);
	void UpdateButtonStatus(int iMode);
	void SelectMainMenuButton(int Select);

	///// General ///////////////////////////////////////////////////////////
	inline BOOL CTRL_DISP(int No, int Data, int Init, const LPSTR Disp[2], UINT Id)
	{
		BOOL	bRet = FALSE;
		if(m_Disp[No] != Data || Init){
			CWnd* pWnd;
			pWnd = GetDlgItem(Id);
			if(pWnd){
				pWnd->SetWindowText(Disp[Data]);
				pWnd->RedrawWindow();
				// MainFrameMenuChange LogOn <-> LogOff //
				//if(Id == IDC_MAIN_MENU_LOG_ON_BUTTON){
					/*//CMenu* pMenu=AfxGetMainWnd()->GetMenu();
					CMenu* pMenu;
					pMenu= m_pMainFrame.GetMenu();
					pWnd = m_pMainFrame.GetMenuItemID(IDM_LOGON);
					pWnd->m_pMainFrame.SetWindowText(Disp[Data]);
					pWnd->m_pMainFrame.RedrawWindow();*/
				//}
				m_Disp[No] = Data;
				bRet = TRUE;
			}
		}
		return bRet;
	}

	inline void ENABLE_BUTTON(int No, int Flag, int Init, UINT Id){
		if(m_Button[No] != Flag || Init){
			CWnd* pWnd;
			pWnd = GetDlgItem(Id);
			if(pWnd){
				pWnd->EnableWindow(Flag);
				m_Button[No] = Flag;
			}
		}
	}

// ï¿½Iï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½h
	// ClassWizard ï¿½Í‰ï¿½ï¿½zï¿½Ö�ï¿½ï¿½ÌƒIï¿½[ï¿½oï¿½[ï¿½ï¿½ï¿½Cï¿½hï¿½ğ�¶�ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B
	//{{AFX_VIRTUAL(CMainMenuView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ï¿½Tï¿½|ï¿½[ï¿½g
	//}}AFX_VIRTUAL

// ï¿½Cï¿½ï¿½ï¿½vï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½eï¿½[ï¿½Vï¿½ï¿½ï¿½ï¿½
protected:
	virtual ~CMainMenuView();

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ê‚½ï¿½ï¿½ï¿½bï¿½Zï¿½[ï¿½W ï¿½}ï¿½bï¿½vï¿½Ö�ï¿½
	//{{AFX_MSG(CMainMenuView)
	afx_msg void OnMainMenuLogOnButton();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMainMenuDataButton();
	//}}AFX_MSG
	afx_msg void OnMainMenuButton(UINT nID);
	afx_msg LRESULT OnPifCommConnect(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ ï¿½Í‘Oï¿½sï¿½Ì’ï¿½ï¿½Oï¿½É’Ç‰ï¿½ï¿½Ì�éŒ¾ï¿½ï¿½}ï¿½ï¿½ï¿½ï¿½ï¿½Ü‚ï¿½ï¿½B

#endif // !defined(AFX_MAINMENUVIEW_H__87441AF2_F73F_4959_8859_3A6131545077__INCLUDED_)
