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
/// @file FormatToolBar.h
/// @brief interface of the CFormatToolBar class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_FORMATTOOLBAR_H__B7AF4660_2F25_4521_A10D_1D175146F444__INCLUDED_)
#define AFX_FORMATTOOLBAR_H__B7AF4660_2F25_4521_A10D_1D175146F444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormatToolBar.h : header file

class CObject3D;

/////////////////////////////////////////////////////////////////////////////
/// CFormatToolBar window
/////////////////////////////////////////////////////////////////////////////
class CFormatToolBar : public CBCGPToolBar
{
	DECLARE_SERIAL(CFormatToolBar)

// Construction
public:
	CFormatToolBar();

// Attributes
public:
	static CBCGPToolbarFontCombo*		CreateTextFontButton ();
	static CBCGPToolbarFontSizeCombo*	CreateTextFontSizeButton ();
	static CBCGPColorMenuButton*		CreateTextColorButton ();
	static CBCGPColorMenuButton*		CreateObjectColorButton ();
	static void OnUpdateCmdUI(CObject3D *pObj);

	static const CBCGPFontDesc* GetFontByCmdID (UINT uiCmdID);
	static int		GetFontSizeByCmdID (UINT uiCmdID);

private:
	static CPalette	m_palObjectColorPicker;	///< Palette for color picker

// Operations
public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormatToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFormatToolBar();

	// Generated message map functions
protected:
	virtual void OnReset ();

	//{{AFX_MSG(CFormatToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMATTOOLBAR_H__B7AF4660_2F25_4521_A10D_1D175146F444__INCLUDED_)
