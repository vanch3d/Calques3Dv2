//////////////////////////////////////////////////////////////////////
/// @file BCGPShapeBar.h
/// @brief interface for the CBCGPShapeBar class
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGPSHAPEBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
#define AFX_BCGPSHAPEBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif 

class CBCGPShapeButton;
class CBCGPPropList;

/////////////////////////////////////////////////////////////////////////////
/// Provides the functionality of the regular docking control bar specially designed  
/// to handle various shape selection issues in the document/application. 
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapeBar : public CBCGPPopupMenuBar
{
	friend class CShapePopup;
	friend class CBCGPShapeButton;
	friend class CBCGPCShapeToolbarButton;
	friend class CBCGPShapePopup;

	DECLARE_SERIAL(CBCGPShapeBar)

// Construction
public:
	CBCGPShapeBar();

	virtual BOOL Create(
			CWnd*		pParentWnd,
			DWORD		dwStyle,
			UINT		nID,
			CPalette*	pPalette = NULL,	// Palette color, NULL - standard 20 colors
			int			nColumns = 0,		// Number of columns, -1 - default
			int			nRowsDockHorz = 0,	// Number of rows for horz. docking
			int			nColDockVert = 0);	// Number of columns for vert. docking

	virtual BOOL CreateControl (
			CWnd*			pParentWnd,
			const CRect&	rect,
			UINT			nID,
			int				nColumns = -1,
			CPalette*		pPalette = NULL);

protected:
	CBCGPShapeBar(
			int	type,
			int nShape,
			int nColumns,
			int	nRowsDockHorz,
			int	nColDockVert,
			UINT nCommandID,
			CBCGPShapeButton* pParentBtn);

	CBCGPShapeBar (CBCGPShapeBar& src, UINT uiCommandID);

// Operations:
public:
	void ContextToSize (BOOL bSquareButtons = TRUE,
						BOOL bCenterButtons = TRUE);

// Attributes
public:
	void SetShape (int shape);
	int GetShape() const
	{
		return m_nShapeSelected;
	}

	UINT GetCommandID () const
	{
		return m_nCommandID;
	}

	void SetCommandID (UINT nCommandID)
	{
		m_nCommandID = nCommandID;
	}

	BOOL IsTearOff () const
	{
		return m_bIsTearOff;
	}

	void SetVertMargin (int nVertMargin);
	void SetHorzMargin (int nHorzMargin);

	int GetVertMargin () const
	{
		return m_nVertMargin;
	}

	int GetHorzMargin ()
	{
		return m_nHorzMargin;
	}

protected:
	int		m_Type;
    int		m_nNumColumns;
    int		m_nNumRowsHorz;
    int		m_nNumColumnsVert;
	int		m_nShape;
	CSize	m_BoxSize;
	int		m_nRowHeight;
	int		m_nShapeSelected;	// (-1) - auto
	UINT	m_nCommandID;
	BOOL	m_bIsEnabled;
	BOOL	m_bIsTearOff;
	int		m_nHorzOffset;
	int		m_nVertOffset;
	BOOL	m_bInternal;	// Used in customization
	int		m_nVertMargin;
	int		m_nHorzMargin;

	CBCGPShapeButton*	m_pParentBtn;
	//CBCGPPropList*		m_pWndPropList;

// Overrides
	virtual BOOL OnSendCommand (const CBCGPToolbarButton* pButton);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual void AdjustLocations ();
	virtual CSize CalcSize (BOOL bVertDock);
	virtual void DoPaint(CDC* pDC);
	virtual BOOL OnKey (UINT nChar);

	virtual void Serialize (CArchive& ar);
	virtual void ShowCommandMessageString (UINT uiCmdId);

	virtual BOOL AllowChangeTextLabels () const
	{
		return FALSE;
	}

	virtual BOOL AllowShowOnList () const
	{	
		return TRUE;	
	}

	virtual void Rebuild ();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPShapeBar)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPShapeBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPShapeBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()

	CSize GetColorGridSize (BOOL bVertDock) const;
	int GetExtraHeight (int nNumColumns) const;
};


/////////////////////////////////////////////////////////////////////////////
/// CBCGPCShapeToolbarButton 
/////////////////////////////////////////////////////////////////////////////
class CBCGPCShapeToolbarButton : public CBCGPToolbarButton  
{
	friend class CBCGPShapeBar;

	DECLARE_SERIAL(CBCGPCShapeToolbarButton)

protected:
	CBCGPCShapeToolbarButton(int type=0,int index=0, 
				BOOL bIsAutomatic = FALSE, 
				LPCTSTR lpszColorName = NULL, BOOL bHighlight = FALSE);

// Overrides
	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						BOOL bHighlight = FALSE,
						BOOL bDrawBorder = TRUE,
						BOOL bGrayDisabledButtons = TRUE);

	virtual BOOL OnToolHitTest(const CWnd* pWnd, TOOLINFO* pTI);
	virtual void OnChangeParentWnd (CWnd* pWndParent);

// Attributes
protected:
	int m_Type;
	int m_nShape;
	BOOL			m_bHighlight;
	BOOL			m_bIsAutomatic;
	CBCGPShapeBar*	m_pParentBar;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPSHAPEBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
