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
/// @file BCGPShapeMenuButton.h
/// @brief interface for the CBCGPShapeMenuButton class
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BCGPSHAPEMENUBUTTON_H__BCABDD04_841E_4B6A_8F61_E7EBE08212D4__INCLUDED_)
#define AFX_BCGPSHAPEMENUBUTTON_H__BCABDD04_841E_4B6A_8F61_E7EBE08212D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPShapeMenuButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapeMenuButton window

/////////////////////////////////////////////////////////////////////////////
/// CBCGPShapeMenuButton
/////////////////////////////////////////////////////////////////////////////
class CBCGPShapeMenuButton : public CBCGPToolbarMenuButton
{
	DECLARE_SERIAL(CBCGPShapeMenuButton)
// Construction
public:
	CBCGPShapeMenuButton();
	CBCGPShapeMenuButton(UINT uiCmdID,int type,int shape);

// Attributes
public:
	int		m_nShapeType;				///<
	int		m_nShape;					///<

	static CMap<UINT,UINT,CPoint, CPoint> m_ShapesByID;
	static CPoint GetShapeByCmdID (UINT uiCmdID);

// Operations
public:
	int GetShape() const;
	void SetShape(int type, int index =-1/* automatic*/);

// Implementation
public:
	virtual ~CBCGPShapeMenuButton();
	virtual void OnChangeParentWnd (CWnd* pWndParent);
	virtual SIZE OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz);
	virtual void OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages,
						BOOL bHorz = TRUE, BOOL bCustomizeMode = FALSE,
						BOOL bHighlight = FALSE,
						BOOL bDrawBorder = TRUE,
						BOOL bGrayDisabledButtons = TRUE);

protected:
		virtual CBCGPPopupMenu* CreatePopupMenu ();
	virtual int OnDrawOnCustomizeList (CDC* pDC, const CRect& rect, BOOL bSelected);
	virtual BOOL IsEmptyMenuAllowed () const
	{
		return TRUE;
	}

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPSHAPEMENUBUTTON_H__BCABDD04_841E_4B6A_8F61_E7EBE08212D4__INCLUDED_)
