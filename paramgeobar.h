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
/// @file ParamGeoBar.h
/// Interface for the classes used in defining the ParamGeo3D user interface.
///
/// $Date$
/// $Revision$
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PARAMGEOBAR_H__CEF67ED0_2817_4D3F_B912_3D6BFB71E15C__INCLUDED_)
#define AFX_PARAMGEOBAR_H__CEF67ED0_2817_4D3F_B912_3D6BFB71E15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CParamGeoEditCtrl : public CBCGPEditCtrl
{
// Construction
public:
	CParamGeoEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamGeoEditCtrl)
	//}}AFX_VIRTUAL
	
// Implementation
public:
	// Tooltip Support
	virtual BOOL OnGetTipText (CString& strTipString);

// Destruction
	virtual ~CParamGeoEditCtrl();

protected:
	// IntelliSense Support

	// Generated message map functions
protected:
	//{{AFX_MSG(CParamGeoEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CMapStringToString m_mTipString;
};

/////////////////////////////////////////////////////////////////////////////
/// Redefinition of the toolbar in the ParamGeo3D widget.
///
/////////////////////////////////////////////////////////////////////////////
class CParamGeoToolBar : public CBCGPToolBar
{
public: 
	// All commands should be routed via owner window, not via frame:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) 
	{ 
		CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*) GetOwner (), bDisableIfNoHndler);
	}

	// Toolbar should't appear on the customization list
	virtual BOOL AllowShowOnList () const		{	return FALSE;	}
	virtual void AdjustLayout ();
};

/////////////////////////////////////////////////////////////////////////////
/// The container for the ParamGeo3D user interface.
///
/////////////////////////////////////////////////////////////////////////////
class CControlsContainer : public CWnd
{
// Construction
public:
	CControlsContainer();

// Attributes
public:
#ifdef _C3D_PARAMGEO
    MKernelVector		kv;  /* Maple kernel handle */
#endif
	CBCGPEditCtrl		m_wndInputCtrl;
	CBCGPEditCtrl		m_wndOutputCtrl;
	CParamGeoToolBar	m_wndToolBar;
	CBCGPStatusBar		m_wndStatusBar;

	CButton			m_wndButton1;
	CButton			m_wndButton2;
	CProgressCtrl	m_wndProgress;
	CFont			m_Font;
	int				m_nTextHeight;

// Operations
public:
	void Layout ();

public:
	static void textCallBack( void *data, int tag, char *output );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlsContainer)
	public:
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlsContainer();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlsContainer)
	afx_msg void OnParamGeoStart();
	afx_msg void OnShowExample();
	afx_msg void OnClearExample();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// The toolbar supporting the ParamGeo3D routines
///
/////////////////////////////////////////////////////////////////////////////
class CParamGeoBar: public CBCGPDockingControlBar
{
public:
	CParamGeoBar();

// Attributes
public:
	CControlsContainer	m_wndContainer;
	CParamGeoToolBar	m_wndToolBar;
	CBCGPTabWnd			m_wndTabs;
	CParamGeoEditCtrl	m_wndSymbolicCtrl;

// Operations
public:
	void AdjustLayout ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamGeoBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CParamGeoBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CParamGeoBar)
	afx_msg void OnParamGeoStart();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// Redefinition of a toolbar button for the loaded Calques 3D document.
/// 
///
/////////////////////////////////////////////////////////////////////////////
class CParamGeoBarLabel : public CBCGPToolbarButton  
{
	DECLARE_SERIAL(CParamGeoBarLabel)
public:
	CParamGeoBarLabel(UINT uiID = 0, LPCTSTR lpszText = NULL);
	virtual ~CParamGeoBarLabel();

	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
						 BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,						
						 BOOL bHighlight = FALSE,						
						 BOOL bDrawBorder = TRUE, 
						 BOOL bGrayDisabledButtons = TRUE);   	

	virtual BOOL IsEditable () const	
	{
		return FALSE;
	}

	virtual SIZE OnCalculateSize(CDC*pDC,const CSize&sizeDefault,BOOL bHorz);

};

#endif // !defined(AFX_PARAMGEOBAR_H__CEF67ED0_2817_4D3F_B912_3D6BFB71E15C__INCLUDED_)
