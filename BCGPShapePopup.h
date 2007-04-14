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
/// @file BCGPShapePopup.h
/// @brief interface for the CBCGPShapePopup class
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGCPSHAPEPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_)
#define AFX_BCGCPSHAPEPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "BCGPPopupMenu.h"
#include "BCGPShapeBar.h"

class CBCGPControlBar;
class CBCGPPropList;

/////////////////////////////////////////////////////////////////////////////
/// CBCGPShapePopup
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapePopup : public CBCGPPopupMenu
{
	friend class CBCGPShapeButton;
	//friend class CBCGPColorProp;
	//friend class CBCGPColorMenuButton;

	DECLARE_DYNAMIC(CBCGPShapePopup)

// Construction
public:
    CBCGPShapePopup (CBCGPShapeButton* pParentBtn,int type,int nShape,int nColumns)
		: m_wndColorBar (type, nShape, nColumns,-1, -1,(UINT)-1,pParentBtn)
	{
		m_bEnabledInCustomizeMode = FALSE;
	}

	virtual ~CBCGPShapePopup();

// Attributes
protected:
	CBCGPShapeBar	m_wndColorBar;
	BOOL			m_bEnabledInCustomizeMode;

protected:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBCGPShapePopup)
	//}}AFX_VIRTUAL

	virtual CBCGPPopupMenuBar* GetMenuBar ()
	{
		return &m_wndColorBar;
	}

	virtual CBCGPControlBar* CreateTearOffBar (CFrameWnd* pWndMain, UINT uiID, LPCTSTR lpszName);

    // Generated message map functions
protected:
    //{{AFX_MSG(CBCGPShapePopup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGCPSHAPEPOPUP_H__6ABB7DC6_CCA2_4B65_9269_3F41D3F21D3D__INCLUDED_)
