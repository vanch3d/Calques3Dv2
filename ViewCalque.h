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
/// @file ViewCalque.h
/// @brief interface of the CViewCalque class.
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VIEWCALQUE_H__ACACFDF0_D297_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWCALQUE_H__ACACFDF0_D297_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

/////////////////////////////////////////////////////////////////////////////
/// Display a subset of the construction in a view similar to the universe.
///
/// This view has all the features of the Universe view (from which it inherits) except that no
/// construction is allowed. 
/// 
///
/////////////////////////////////////////////////////////////////////////////
class CViewCalque : public CViewUniv
{
protected:
	CViewCalque();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewCalque)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCalque)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewCalque();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewCalque)
	//}}AFX_MSG
	afx_msg void OnUpdateTasks(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCALQUE_H__ACACFDF0_D297_11D4_A2FA_00D0B71C8709__INCLUDED_)
