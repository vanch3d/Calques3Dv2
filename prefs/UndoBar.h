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
/// @file UndoBar.h
/// @brief interface of the CUndoBar class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_UNDOBAR_H__F88FEBBF_A4D0_4BEC_80DE_0CC1D1C8B073__INCLUDED_)
#define AFX_UNDOBAR_H__F88FEBBF_A4D0_4BEC_80DE_0CC1D1C8B073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUndoButton;
class CUndoBar;


/////////////////////////////////////////////////////////////////////////////
/// CUndoListBox
///
/////////////////////////////////////////////////////////////////////////////
class CUndoListBox : public CListBox
{
// Construction
public:
	CUndoListBox(CUndoBar& bar);

// Attributes
protected:
	CUndoBar&	m_Bar;

// Operations
public:
	UINT		m_resID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUndoListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUndoListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CUndoBar
///
/////////////////////////////////////////////////////////////////////////////
class CUndoBar : public CBCGPPopupMenuBar
{
	DECLARE_SERIAL(CUndoBar)

// Construction
public:
	CUndoBar();

// Attributes
protected:
	CUndoListBox	m_wndList;
	CRect			m_rectLabel;
	int				m_nLabelHeight;
	CString			m_strLabel;

// Operations
public:
	void DoUndo ();
	void SetLabel (const CString& strLabel);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoBar)
	//}}AFX_VIRTUAL

	virtual void OnFillBackground (CDC* pDC);
	virtual CSize CalcSize (BOOL bVertDock);

// Implementation
public:
	virtual ~CUndoBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUndoBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CUndoButton* GetParentButton () const;
};

/////////////////////////////////////////////////////////////////////////////
/// CUndoMenu
///
/////////////////////////////////////////////////////////////////////////////
class CUndoMenu : public CBCGPPopupMenu
{
	DECLARE_SERIAL(CUndoMenu)

	virtual CBCGPPopupMenuBar* GetMenuBar ()
	{
		return &m_wndUndoBar;
	}

	CUndoBar	m_wndUndoBar;

    // Generated message map functions
protected:
    //{{AFX_MSG(CUndoMenu)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CUndoButton
///
/////////////////////////////////////////////////////////////////////////////
class CUndoButton : public CBCGPToolbarMenuButton
{
	friend class CUndoBar;

	DECLARE_SERIAL(CUndoButton)

public:
	CUndoButton()
	{
	}

	CUndoButton(UINT uiCmdID, LPCTSTR lpszText) :
		CBCGPToolbarMenuButton (uiCmdID, NULL,
			CImageHash::GetImageOfCommand (uiCmdID, FALSE), lpszText)
	{
	}

	static CStringList	m_lstActions;
	static int			m_nSelNum;

	static int GetSelNum ()
	{
		return m_nSelNum;
	}

	static void ResetSelNum ()
	{
		m_nSelNum = 0;
	}

protected:
	virtual CBCGPPopupMenu* CreatePopupMenu ()
	{
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return NULL;
		}

		return new CUndoMenu;
	}

	virtual BOOL IsEmptyMenuAllowed () const
	{
		return TRUE;
	}

	virtual void OnChangeParentWnd (CWnd* pWndParent)
	{
		CBCGPToolbarMenuButton::OnChangeParentWnd (pWndParent);
		m_bDrawDownArrow = TRUE;
	}
};

//////////////////////////////////////////////////////////////////////////////
// CRedoButton

/*class CRedoButton : public CUndoButton
{
	DECLARE_SERIAL(CRedoButton)

public:
	CRedoButton() : CUndoButton()
	{
	}

	CRedoButton(UINT uiCmdID, LPCTSTR lpszText) : CUndoButton(uiCmdID, lpszText)
	{
	}
};*/


/////////////////////////////////////////////////////////////////////////////
/// CRedoButton
///
/////////////////////////////////////////////////////////////////////////////
class CRedoButton : public CBCGPToolbarMenuButton
{
	friend class CUndoBar;

	DECLARE_SERIAL(CRedoButton)

public:
	CRedoButton()
	{
	}

	CRedoButton(UINT uiCmdID, LPCTSTR lpszText) :
		CBCGPToolbarMenuButton (uiCmdID, NULL,
			CImageHash::GetImageOfCommand (uiCmdID, FALSE), lpszText)
	{
	}

	static CStringList	m_lstActions;
	static int			m_nSelNum;

	static int GetSelNum ()
	{
		return m_nSelNum;
	}

	static void ResetSelNum ()
	{
		m_nSelNum = 0;
	}

protected:
	virtual CBCGPPopupMenu* CreatePopupMenu ()
	{
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return NULL;
		}

		return new CUndoMenu;
	}

	virtual BOOL IsEmptyMenuAllowed () const
	{
		return TRUE;
	}

	virtual void OnChangeParentWnd (CWnd* pWndParent)
	{
		CBCGPToolbarMenuButton::OnChangeParentWnd (pWndParent);
		m_bDrawDownArrow = TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOBAR_H__F88FEBBF_A4D0_4BEC_80DE_0CC1D1C8B073__INCLUDED_)
