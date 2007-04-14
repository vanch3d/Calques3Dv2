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
/// @file PropertyBar.h
/// @brief interface for the CPropertyBar and CDependentBar classes
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PROPERTYBAR_H__1AADA4F6_2728_4C39_B844_3B0AD1B1B9D7__INCLUDED_)
#define AFX_PROPERTYBAR_H__1AADA4F6_2728_4C39_B844_3B0AD1B1B9D7__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

class CObject3D;
class CText3D;

/////////////////////////////////////////////////////////////////////////////
/// Private toolbar used in the CPropertyBar class
///
/////////////////////////////////////////////////////////////////////////////
class CPropertiesToolBar : public CBCGPToolBar
{
public:	
	/////////////////////////////////////////////////////////////////////////////
	/// Override for special enabling/disabling of user-interface items associated with the toolbar
	/// @param bDisableIfNoHndler	Specifies whether the user-interface item should be disabled 
	///								if there is no handler defined in message map 
	/////////////////////////////////////////////////////////////////////////////
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)	
	{		
		CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*) GetOwner (), bDisableIfNoHndler);
	}

	/////////////////////////////////////////////////////////////////////////////
	/// Called by the framework to receive permission from a CBCGPBaseToolBar-derived object 
	/// to display (or not) the object in the list box on the toolbar customization page.   
	/////////////////////////////////////////////////////////////////////////////
	virtual BOOL AllowShowOnList () const		{	return FALSE;	}
};

/////////////////////////////////////////////////////////////////////////////
/// CPropertyBar
///
/////////////////////////////////////////////////////////////////////////////
class CPropertyBar : public CBCGPDockingControlBar
{
public:
	CPropertyBar();
	void AdjustLayout ();
	void OnChangeVisualStyle ();

// Attributes
public:
	CPropertiesToolBar	m_wndToolBar;		///< The local toolbar associated with the control
protected:
	CBCGPPropList		m_wndProp;			///< The property list of the control
	CPalette			m_palColorPicker;	///< The local colour palette used by color properties
	CImageList			m_pImgList;			///< The list of icons used for CObject3D properties
	CObject3D*			m_pSelObj;			///< Current selected object

// Operations
protected:
	CBCGPProp* GetObjectAppearance(CObject3D* pObj);
	CBCGPProp* GetTextAppearance(CText3D* pObj);

public:
    void EnableBar(BOOL bOpen=TRUE);
	virtual void AddProperties(CObject3D* pObj);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertyBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpand();
	afx_msg void OnUpdateExpand(CCmdUI* pCmdUI);
	afx_msg void OnSorting();
	afx_msg void OnUpdateSorting(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnPropertyChanged (WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// CDependentBar
///
/////////////////////////////////////////////////////////////////////////////
class CDependentBar : public CPropertyBar
{
public:
	CDependentBar();
	virtual void AddProperties(CObject3D* pObj);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDependentBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDependentBar();

protected:
	//{{AFX_MSG(CDependentBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateExpand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSorting(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_PROPERTYBAR_H__1AADA4F6_2728_4C39_B844_3B0AD1B1B9D7__INCLUDED_)
