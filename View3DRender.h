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
/// @file View3DRender.h
/// @brief interface of the CView3DRender class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VIEW3DRENDER_H__B2DB87D7_9EE1_4190_96A2_D923EB8198F0__INCLUDED_)
#define AFX_VIEW3DRENDER_H__B2DB87D7_9EE1_4190_96A2_D923EB8198F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGLTools\GLEnabledView.h"
#include "OGLTools/BallController.h"

class CRenderPropDlg;

#define NBVIEWPARAMS 12

/////////////////////////////////////////////////////////////////////////////
/// Display the construction in real 3D, using OpenGL.
///
/////////////////////////////////////////////////////////////////////////////
class CView3DRender : public CGLEnabledView
{
protected:
	CView3DRender();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView3DRender)

// Attributes
public:
	CPoint MouseDownPoint;
	CBallController trackball;
	CRenderPropDlg* m_pDlg;
    int   m_LightParam[NBVIEWPARAMS];  // Graphics dimension (along X-axis)
// Operations
public:
	CCalques3DDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView3DRender)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnCreateGL();
	void OnDrawGL();
	void GetLightParams(int *pPos);			// Get lighting parameters
	void SetLightParam (short lp, int nPos);	// Set lighting parameters
	void SetLight();
protected:
	virtual ~CView3DRender();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void OnSizeGL(int cx, int cy);
	//{{AFX_MSG(CView3DRender)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in Calques3DView.cpp
inline CCalques3DDoc* CView3DRender::GetDocument()
   { return (CCalques3DDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////
/// Dialog for the OpenGL rendering attributes.
///
/////////////////////////////////////////////////////////////////////////////
class CRenderPropDlg : public CDialog
{
public:
	CView3DRender *m_pView;
	int m_Pos[NBVIEWPARAMS];

	CRenderPropDlg(CView3DRender* p);
	int GetSliderNum(HWND hwnd, UINT& nID);


	//{{AFX_DATA(CRenderPropDlg)
	enum { IDD = IDD_RENDERER_PROP };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CRenderPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CRenderPropDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW3DRENDER_H__B2DB87D7_9EE1_4190_96A2_D923EB8198F0__INCLUDED_)
