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
// ChildFrm.cpp: implementation of the CChildFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Calques3D.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "Calques3DDoc.h"
#include "ViewUniv.h"
#include "ViewHisto.h"
#include "ViewCalque.h"
#include "ViewGraph.h"
#include "ViewAnalytic.h"
#include "View3DRender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CBCGPMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CBCGPMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CBCGPMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CBCGPMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	//CMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);
	CString wStrng;
	//GetWindowText(wStrng);
	CView *pView = GetActiveView();
	CViewCalque *pClq = NULL;
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	CDocument *pDoc = pView->GetDocument();
	wStrng = pDoc->GetTitle();
	if (pClq = DYNAMIC_DOWNCAST(CViewCalque,pView))
	{
		int nID = pClq->GetVisualParam()->nCalqueNum;
		CString mstr,mstr2;
		mstr.LoadString(IDR_VIEWCALQUE);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		if (nID)
			mstr2.Format(_T("%s %d - %s"),tt,nID,wStrng);
		else
			mstr2.Format(_T("%s - %s"),tt,wStrng);
		//SetWindowText(mstr2);
		AfxSetWindowText(m_hWnd, mstr2);

	}
   else if (DYNAMIC_DOWNCAST(CViewUniv,pView))
	{
		CString mstr;
		mstr.LoadString(IDR_VIEWUNIVERSE);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		wStrng = tt + _T(" - ") + wStrng;
		//SetWindowText(wStrng);
		AfxSetWindowText(m_hWnd, wStrng);
	}
	else if (DYNAMIC_DOWNCAST(CViewHisto,pView))
	{
		CString mstr;
		mstr.LoadString(IDR_VIEWHISTORIQUE);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		wStrng = tt + _T(" - ") + wStrng;
		//SetWindowText(wStrng);
		AfxSetWindowText(m_hWnd, wStrng);
	}
	else if (DYNAMIC_DOWNCAST(CViewGraph,pView))
	{
		CString mstr;
		mstr.LoadString(IDR_VIEWGRAPH);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		wStrng = tt + _T(" - ") + wStrng;
		//SetWindowText(wStrng);
		AfxSetWindowText(m_hWnd, wStrng);
	}
	else if (DYNAMIC_DOWNCAST(CViewAnalytic,pView))
	{
		CString mstr;
		mstr.LoadString(IDR_VIEWANALYTIC);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		wStrng = tt + _T(" - ") + wStrng;
		//SetWindowText(wStrng);
		AfxSetWindowText(m_hWnd, wStrng);
	}
	else if (DYNAMIC_DOWNCAST(CView3DRender,pView))
	{
		CString mstr;
		mstr.LoadString(IDR_VIEWRENDERING);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		wStrng = tt + _T(" - ") + wStrng;
		//SetWindowText(wStrng);
		AfxSetWindowText(m_hWnd, wStrng);
	}
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CBCGPMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
    // TODO: Add your message handler code here
    CView *pView = GetActiveView( ) ;
    if (!pView) return;

	
    CCalques3DDoc *pMDoc = DYNAMIC_DOWNCAST( CCalques3DDoc, pView->GetDocument());
    CMainFrame *pWnd = DYNAMIC_DOWNCAST (CMainFrame, AfxGetMainWnd());
	
    if (!pWnd) return;

    if (!pActivateWnd)
    {
        pWnd->GetPropertyBar()->EnableBar(FALSE);
        pWnd->GetDependentBar()->EnableBar(FALSE);
        //pWnd->m_wndWorkSpace.UpdateDocument();
		pWnd->SetWarningMsg(FALSE);
    }
    else if (bActivate)// && pMDoc)
    {
        pWnd->GetPropertyBar()->EnableBar(TRUE);
        pWnd->GetDependentBar()->EnableBar(TRUE);
        //pWnd->m_wndWorkSpace.UpdateFromDoc(pMDoc);
 		BOOL warning = (pMDoc) ? pMDoc->IsMathPadUsed() : FALSE;
		pWnd->SetWarningMsg(warning);
   }
	else
    {
        pWnd->GetPropertyBar()->EnableBar(FALSE);
        pWnd->GetDependentBar()->EnableBar(FALSE);
        //pWnd->m_wndWorkSpace.UpdateDocument();
		pWnd->SetWarningMsg(FALSE);
    }	
}
