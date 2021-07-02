/////////////////////////////////////////////////////////////////////////////
// StatusBarEx.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// StatusBarEx

class CStatusBarEx : public CStatusBar
{
public:
	typedef struct
	{
		int				nIndex;			// ペインの左端からのインデックス.
		UINT			uStrId;			// ペインの文字列ID.
		COLORREF		TextColor;		// ペインのテキスト色.
		COLORREF		BackColor;		// ペインの背景色.
		CString			sText;			// ペインのテキスト.
	}	PANE_DATA;

				CStatusBarEx();
	virtual		~CStatusBarEx();
	int			Initialize( UINT , COLORREF TextColor, COLORREF BackColor);
	int			Initialize( UINT strId, int bDisable = FALSE);
	void		ChangeColor( UINT strId, COLORREF TextColor, COLORREF BackColor, int bUpdate = TRUE);
	void		ChangeTextColor( UINT strId, COLORREF TextColor, int bUpdate = TRUE);
	void		ChangeBackColor( UINT strId, COLORREF BackColor, int bUpdate = TRUE);
	void		ChangeText( UINT strId, LPCTSTR pszNewText, int bUpdate = TRUE);

public:
	int lInitPane( UINT strId, COLORREF TextColor, COLORREF BackColor);
	CTypedPtrArray<CPtrArray,PANE_DATA*>	m_Panes;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StatusBarEx)
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDIS);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(StatusBarEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int			lFindBufferByIndex( int index);
	int			lFindBufferByStrId( UINT strId);
};
