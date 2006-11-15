//////////////////////////////////////////////////////////////////////
/// @file BCGPShapeButton.h
/// @brief interface for the CBCGPShapeButton class
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGPSHAPEBUTTON_H__FEA34C6D_5D34_461C_9403_95C23C0B6F4A__INCLUDED_)
#define AFX_BCGPSHAPEBUTTON_H__FEA34C6D_5D34_461C_9403_95C23C0B6F4A__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

class CBCGPShapePopup;

/////////////////////////////////////////////////////////////////////////////
/// CBCGPShapeButton window
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapeButton : public CBCGPButton
{
	friend class CBCGPShapeBar;

	DECLARE_DYNAMIC(CBCGPShapeButton)

// Construction
public:
	CBCGPShapeButton();

// Attributes
public:
	BOOL	m_bEnabledInCustomizeMode;	///< 
	BOOL	m_bAutoSetFocus;			///<

protected:
	int		m_nShapeType;				///<
	int		m_nShape;					///<
	int		m_nColumns;					///<

	CBCGPShapePopup*	m_pPopup;		///<

// Operations
public:
	int GetShape() const;
	void SetShape(int type, int index =-1/* automatic*/);

	void SetColumnsNumber (int nColumns)
	{
		m_nColumns = nColumns;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPShapeButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPShapeButton();
	virtual CSize SizeToContent (BOOL bCalcOnly = FALSE);

protected:
	virtual void UpdateShape (int color);
	virtual void OnFillBackground (CDC* pDC, const CRect& rectClient);
	virtual void OnDraw (CDC* pDC, const CRect& rect, UINT uiState);
	virtual void OnDrawBorder (CDC* pDC, CRect& rectClient, UINT uiState);
	virtual void OnDrawFocusRect (CDC* pDC, const CRect& rectClient);
	
	virtual void OnShowColorPopup ();

	BOOL IsDrawXPTheme () const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPShapeButton)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPSHAPEBUTTON_H__FEA34C6D_5D34_461C_9403_95C23C0B6F4A__INCLUDED_)
