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
/// @file ViewUniv.h
/// @brief interface of the CViewUniv class.
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VIEWUNIV_H__1B2AE4DA_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWUNIV_H__1B2AE4DA_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCGPSliderButton.h"

class CVisualParam;
class CTask;

/////////////////////////////////////////////////////////////////////////////
/// Display the complete construction in a 2½D environment.
///
/// This is the main interface for creating, visualizing and manipulation geometrical constructions
/// in Calques 3D.
/////////////////////////////////////////////////////////////////////////////
class CViewUniv : public CView
{
protected:
	CViewUniv();					// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewUniv)

// Attributes
private:
	CVisualParam*	m_pVisParam;		///< Pointer to the local visualization parameters
	CTask*			m_pCurrentTask;		///< Pointer to the current activated task
	CToolTipCtrl	m_wndToolTip;			///< Reimplementation of the tooltip

	CBCGPSliderToolbar	m_wndHorizBar;	///< Control for the horizontal slider
	CBCGPSliderToolbar	m_wndVertBar;	///< Control for the vertical slider

	CObject3D*		pSelectObj;			///< Pointer to the object currently selected

// Operations
public:
	CCalques3DDoc* GetDocument();

	CObject3D*		GetSelectedObject();
	CVisualParam*	GetVisualParam();
	void			SetVisualParam(int VP);
	void			SetFixedProjection(BOOL bFixed = TRUE);
	void			SetSliderPosition(UINT sliderID,int pos);
	void			OnMouse3D(UINT);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewUniv)
	public:
	virtual void	OnInitialUpdate();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void	OnDraw(CDC* pDC);
	virtual void	OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void	OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL	OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

// Implementation
protected:
	virtual ~CViewUniv();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL IsWindowActivated();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewUniv)
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg void	OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void	OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void	OnSetFocus(CWnd* pOldWnd);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL	OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void	OnUpdateToggleMarks(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateVisualisationReferential(CCmdUI* pCmdUI);
	afx_msg void	OnToggleMarks();
	afx_msg void	OnEditUndo();
	afx_msg void	OnEditRedo();
	afx_msg void	OnEditCopy();
	afx_msg void	OnChangePointsOfView();
	afx_msg void	OnMacrosSave();
	//}}AFX_MSG
	afx_msg void	OnFilePrintPreview();
	afx_msg void	OnMacrosInfo();
	afx_msg void	OnShowHidden();
	afx_msg void	OnChangePOV(UINT);
	afx_msg void	OnChangeZoom(UINT);
	afx_msg LRESULT OnChangeZoom(WPARAM,LPARAM);
	afx_msg void	OnChangePerspective(UINT);
	afx_msg void	OnChangeReferential(UINT);
	afx_msg void	OnStartTask(UINT);
	afx_msg BOOL	OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void	OnRotateHor();
	afx_msg void	OnRotateVer();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	afx_msg void	OnUpdateMacrosInfo(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateTasks(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateReferential(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateZoom(CCmdUI* pCmdUI);
	afx_msg void	OnUpdatePOV(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateSliders(CCmdUI* pCmdUI);
	afx_msg void	OnUpdatePerspective(CCmdUI* pCmdUI);
	LRESULT			OnUpdateObjTooltip(WPARAM wp, LPARAM lp=0);
	afx_msg void	OnUpdateFormat(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateShowHidden(CCmdUI* pCmdUI);
	afx_msg void	OnFormat(UINT);
	afx_msg void	OnFormat();

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Calques3DView.cpp
inline CCalques3DDoc* CViewUniv::GetDocument()
   { return (CCalques3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWUNIV_H__1B2AE4DA_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
