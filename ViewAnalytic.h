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
/// @file ViewAnalytic.h
/// @brief interface of the CViewAnalytic class.
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VIEWANALYTIC_H__9DBCACD6_F378_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWANALYTIC_H__9DBCACD6_F378_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAnalytic.h : header file
//

#include "thirdparty/MRTracker.h"

class CEquation3D;
//class MTParser;
/////////////////////////////////////////////////////////////////////////////
/// Display all the analytic elements of the construction in a notebook-type environment.
///
/////////////////////////////////////////////////////////////////////////////
class CViewAnalytic : public CScrollView
{
protected:
	CViewAnalytic();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewAnalytic)

// Attributes
public:
	CxObject3DSet	m_cSelObjectSet;
	CEquation3D		*m_pSelObject;
	CPoint			m_oldP,
					m_ptSelStart;

	BOOL			m_bSelection;
	CRect			m_rSelect;
	CRect			m_rViewSize;
	CRect			m_rMargin;
		
	CToolTipCtrl	myToolTip;
	CMultiRectTracker	m_wndMultiTrack;

	CEdit *			m_pEdit;
	CDialog*		m_pDialog;

private:
	BOOL			m_bIsEditMode;
	//MTParser *m_pParser;
	CBCGPToolBarImages	m_bmpBack;

// Operations
public:
	CCalques3DDoc* GetDocument();

	void OnDrawAnalytic(CDC* pDC);      // overridden to draw this view

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAnalytic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewAnalytic();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewAnalytic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateAddComment(CCmdUI* pCmdUI);
	afx_msg void OnAddComment();
	afx_msg void OnAddEquation();
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditSelectAll();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFormat(CCmdUI* pCmdUI);
	afx_msg void OnFormat(UINT);
	afx_msg void OnFormat();
	LRESULT OnUpdateObjTooltip(WPARAM wp, LPARAM lp=0);
	afx_msg LRESULT OnEndEdit(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Calques3DView.cpp
inline CCalques3DDoc* CViewAnalytic::GetDocument()
   { return (CCalques3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWANALYTIC_H__9DBCACD6_F378_11D4_A2FA_00D0B71C8709__INCLUDED_)
