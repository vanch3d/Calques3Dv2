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
/// @file BCGPSliderButton.h
/// @brief interface of the CBCGPSliderToolbar and associated classes.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGPSLIDERBUTTON_H__7E838796_4EA9_11D3_A72E_0090274409AC__INCLUDED_)
#define AFX_BCGPSLIDERBUTTON_H__7E838796_4EA9_11D3_A72E_0090274409AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBCGPSliderButton;

/////////////////////////////////////////////////////////////////////////////
/// CBCGPSliderToolbar window
/////////////////////////////////////////////////////////////////////////////
class CBCGPSliderToolbar : public CBCGPToolBar
{
// Construction
public:
	UINT m_nAlign;
	CBCGPSliderToolbar();
	virtual BOOL CreateEx(CWnd* pParentWnd,UINT align=CBRS_ALIGN_TOP,UINT nID = AFX_IDW_TOOLBAR);

protected:
	virtual BOOL AllowShowOnList () const		{	return FALSE;	}

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPSliderToolbar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGPSliderCtrl window
/////////////////////////////////////////////////////////////////////////////
class CBCGPSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CBCGPSliderCtrl(CBCGPSliderButton& btnSlider);

// Attributes
public:
	CBCGPSliderButton& m_btnSlider;
	CBCGPToolBarImages m_Images;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPSliderCtrl)
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPSliderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPSliderCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnPaint();
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CBCGPSliderButton window
/////////////////////////////////////////////////////////////////////////////
class CBCGPSliderButton : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CBCGPSliderButton)

public:
	CBCGPSliderButton();
	CBCGPSliderButton (UINT uiId,
			int iImage = -1,
			DWORD dwStyle = 0,
			int iWidth = 0);

	virtual ~CBCGPSliderButton();

	void SetRange (int iMin, int iMax);
	void SetValue (int iValue, BOOL bNotify = TRUE);

	int GetPos (UINT uiCmd);

// Overrides:
	virtual void Serialize(CArchive& ar);
	virtual void CopyFrom (const CBCGPToolbarButton& src);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual void OnMove ();
	virtual void OnSize (int iSize);
	virtual HWND GetHwnd ()
	{	
		return m_wndSlider.GetSafeHwnd ();
	}

	virtual BOOL CanBeStretched () const
	{	
		return TRUE;	
	}

	virtual BOOL HaveHotBorder () const
	{
		return FALSE;
	}


// Attributes:
public:
	CBCGPSliderCtrl	m_wndSlider;
	CBCGPToolBar*	m_pToolBar;
	int				m_iWidth;
	DWORD			m_dwStyle;
	int				m_nMin;
	int				m_nMax;
	int				m_nValue;
};

#endif // !defined(AFX_BCGPSLIDERBUTTON_H__7E838796_4EA9_11D3_A72E_0090274409AC__INCLUDED_)
