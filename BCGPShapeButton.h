//////////////////////////////////////////////////////////////////////
// Calques 3D - a 3D Dynamic Geometry Learning Environment
// Copyright (c) 1997-2007 Nicolas Van Labeke
//////////////////////////////////////////////////////////////////////
// This file is part of Calques 3D.
// 
// Calques 3D is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.
// 
// Calques 3D is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License 
// along with Calques 3D; if not, write to The Free Software Foundation, Inc., 
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
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
