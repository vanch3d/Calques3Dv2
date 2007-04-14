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
/// @file IconCtrl.h
/// @brief interface of the CIconCtrl class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_ICONCTRL_H__674347E9_6B9E_44A6_BE1D_D342C6A7E98B__INCLUDED_)
#define AFX_ICONCTRL_H__674347E9_6B9E_44A6_BE1D_D342C6A7E98B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ICONCTRL_CLASSNAME    _T("MFCIconCtrl")  // Window class name

/////////////////////////////////////////////////////////////////////////////
/// CIconCtrl
/////////////////////////////////////////////////////////////////////////////
class CIconCtrl : public CWnd
{
	DECLARE_DYNCREATE(CIconCtrl)

// Construction
public:
	CIconCtrl();
	void SetImage(int nb) {m_nImage = nb;};

// Attributes
public:
	int					m_nImage;	///< Zero-based index of the image to display
	CBCGPToolBarImages	m_cImage;	///< List of images

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconCtrl)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconCtrl();
    BOOL RegisterWindowClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONCTRL_H__674347E9_6B9E_44A6_BE1D_D342C6A7E98B__INCLUDED_)
