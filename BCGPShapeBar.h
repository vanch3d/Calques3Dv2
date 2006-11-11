// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

#if !defined(AFX_BCGPSHAPEBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
#define AFX_BCGPSHAPEBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPColorBar.h : header file
//

//#include "BCGCBPro.h"
//#include "BCGPPopupMenuBar.h"

class CBCGPShapeButton;
class CBCGPPropList;

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeBar window

class CBCGPShapeBar : public CBCGPPopupMenuBar
{
	friend class CShapePopup;
	//friend class CBCGPColorMenuButton;
	friend class CBCGPShapeButton;
	friend class CShapeToolbarPopup;
	friend class CBCGPShapePopup;
	//friend class CBCGPImageEditDlg;
	//friend class CBCGPColorProp;

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
			CPalette*		pPalette = NULL
		);

protected:
	CBCGPShapeBar(int type,
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
	int			m_Type;
    int							m_nNumColumns;
    int							m_nNumRowsHorz;
    int							m_nNumColumnsVert;
	int							m_nShape;
	CSize						m_BoxSize;
	int							m_nRowHeight;
	int							m_nShapeSelected;	// (-1) - auto
	UINT						m_nCommandID;
	BOOL						m_bIsEnabled;
	BOOL						m_bIsTearOff;
	CBCGPShapeButton*			m_pParentBtn;
	//CBCGPPropList*				m_pWndPropList;
	int							m_nHorzOffset;
	int							m_nVertOffset;
	BOOL						m_bInternal;	// Used in customization
	int							m_nVertMargin;
	int							m_nHorzMargin;

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPSHAPEBAR_H__9B759C19_6B1C_48C4_A884_C0DEDD60DD77__INCLUDED_)
