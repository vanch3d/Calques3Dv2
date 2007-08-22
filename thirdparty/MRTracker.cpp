////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1999 by Thierry Maurel
// All rights reserved
//
// Distribute freely, except: don't remove my name from the source or
// documentation (don't take credit for my work), mark your changes (don't
// get me blamed for your possible bugs), don't alter or remove this
// notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc., and
// I'll try to keep a version up to date.  I can be reached as follows:
//    tmaurel@caramail.com
//
////////////////////////////////////////////////////////////////////////////////
// Version : 1.0                       * Author : T.Maurel
// Date    : 30.08.99
//
// MultiRectTracker.cpp: implementation of the CMultiRectTracker class.
//
//////////////////////////////////////////////////////////////////////
//
// This class is useful for vector editors, or similar applications
// where the user can select or move some objects on the screen.
// Anywhere you use CRectTracker, you can use this extended version,
// with the same options for drawing (handle size, style...).
// The principle is contains in a loop : for each object, update the 
// m_rect and call the CRectTracker corresponding function.
// The m_rect is not used in other cases, so there is no potential pb.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\calques3d.h"

#include "..\objects\text3d.h"
#include "MRTracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
//
CMultiRectTracker::CMultiRectTracker()
{
	SetGridSize();
	m_pView=0;
    m_nHandleSize = 4;
    m_nStyle = dottedLine | resizeOutside;
}

//////////////////////////////////////////////////////////////////////
//
//
CMultiRectTracker::~CMultiRectTracker()
{
    m_Objects.RemoveAll ();
    ClearPositions ();
}

void CMultiRectTracker::SetTrackerView(CView *mv)
{
	m_pView = mv;
}

//////////////////////////////////////////////////////////////////////
// API
//////////////////////////////////////////////////////////////////////
// Object pointers management, very simple.
//////////////////////////////////////////////////////////////////////
//
//
void CMultiRectTracker::Add( CText3D* pObject )
{
    m_Objects.Add (pObject);
    m_pObject = pObject;
}

void CMultiRectTracker::Remove( CText3D* pObject )
{
	if (!pObject) return;

    for (int index = 0; index < m_Objects.GetSize(); index++)
	{
		CText3D *pObj = m_Objects.GetAt(index);
		if (pObj == pObject)
		{
			m_Objects.RemoveAt(index);
			break;
		}
	}
}



//////////////////////////////////////////////////////////////////////
//
//
void CMultiRectTracker::RemoveAll ()
{
    m_Objects.RemoveAll();
    m_rect.SetRectEmpty();
}


//////////////////////////////////////////////////////////////////////
// Do the whole work, decide if the user moves objects or extends only one.
//
BOOL CMultiRectTracker::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert, CWnd* pWndClipTo)
{
	// If the array is empty, do nothing...
    if (IsEmpty())
        return FALSE;

    // perform hit testing on the handles
	int nHandle = HitTestHandles(point);
	if (nHandle == hitNothing)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

    if (nHandle == hitMiddle) {
        // The selected objects will be moved on the screen
        return MultiTrackHandle (pWnd, point, pWndClipTo);

    } else {
        // Update the only one object selected,
        // and call CRectTracker::TrackHandle
		//if (!m_pObject->m_bResize) return false;

		CClientDC dc(m_pView);
		m_pView->OnPrepareDC(&dc);

        m_Objects.RemoveAll();
        m_Objects.Add(m_pObject);
        m_rect.CopyRect (m_pObject->rActZone);
        // otherwise, call helper function to do the tracking
	    //dc.DPtoLP(&m_rect);
		m_bAllowInvert = bAllowInvert;
	    BOOL bRet = TrackHandle(nHandle, m_pView, point, pWndClipTo);
	    //dc.LPtoDP(&m_rect);
		if (m_szGrid != CSize(-1,-1))
		{
			int r=0;
		}
        if (bRet)
            m_pObject->rActZone  = m_rect;
        return bRet;
    }
}

//////////////////////////////////////////////////////////////////////
// Very classic drawing function
//
void CMultiRectTracker::Draw(CDC* pDC)
{
	//CClientDC dc(m_pView);
	//m_pView->OnPrepareDC(&dc);
    for (int index = 0; index < m_Objects.GetSize(); index++) {
        m_rect.CopyRect(m_Objects.GetAt(index)->rActZone);
		pDC->LPtoDP(&m_rect);

        CRectTracker::Draw(pDC);
    }
}



//////////////////////////////////////////////////////////////////////
// Hit functions
//////////////////////////////////////////////////////////////////////
//
//
int CMultiRectTracker::HitTestHandles(CPoint point) 
{
    int iHit = hitNothing;
    for (int index = 0; index < m_Objects.GetSize(); index++) {
        m_rect.CopyRect(m_Objects.GetAt(index)->rActZone);
        iHit = CRectTracker::HitTestHandles(point);
        if (iHit != hitNothing) {
            m_pObject = m_Objects.GetAt(index);
            return iHit;
        }
    }
    return hitNothing;
}


//////////////////////////////////////////////////////////////////////
//
//
int CMultiRectTracker::HitTest(CPoint point)
{
    int iHit = hitNothing;
    for (int index = 0; index < m_Objects.GetSize(); index++) {
        m_rect.CopyRect(m_Objects.GetAt(index)->rActZone);
        iHit = CRectTracker::HitTest(point);
        if (iHit != hitNothing) {
            m_pObject = m_Objects.GetAt(index);
            return iHit;
        }
    }
    return hitNothing;
}


//////////////////////////////////////////////////////////////////////
// Ask each objects to get a cursor change or not
//
BOOL CMultiRectTracker::SetCursor(CWnd *pWnd, UINT nHitTest)
{
	CClientDC dc(m_pView);
	m_pView->OnPrepareDC(&dc);
    CRect rectSave = m_rect;
    for (int index = 0; index < m_Objects.GetSize(); index++) {
        m_rect.CopyRect(m_Objects.GetAt(index)->rActZone);
		dc.LPtoDP(&m_rect);
        if (CRectTracker::SetCursor(pWnd, nHitTest)) {
            m_rect = rectSave;
            return TRUE;
        }
    }
    m_rect = rectSave;
    return FALSE;
}


//////////////////////////////////////////////////////////////////////
// Internal functions
//////////////////////////////////////////////////////////////////////
// Inspirated by CRectTarcker::TrackHandle
//
BOOL CMultiRectTracker::MultiTrackHandle (CWnd* pWnd, CPoint point, CWnd* pWndClipTo)
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();

    CPoint oldPoint = point;

    // We work on the rects copies, not the objects
    CopyPositions ();

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pWndClipTo->GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ASSERT_VALID(pDrawDC);

	CRect rectOld;
	BOOL bMoved = FALSE;

    int index;
    CRect* pRect;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
            
			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);
/*			{
				CSize szDelta(0,0);
				CPoint currPt((int)(short)LOWORD(msg.lParam),(int)(short)HIWORD(msg.lParam));

                pRect = m_CopyPosition.GetAt(0);
				szDelta = currPt - pRect->TopLeft();

				oldPoint = m_rect.TopLeft();

				for (index = 0; index < m_CopyPosition.GetSize(); index++) {
	                pRect = m_CopyPosition.GetAt(index);
	                m_rect.CopyRect(pRect);

					rectOld = m_rect;
					/// NVL
					//CPoint currPt((int)(short)LOWORD(msg.lParam),(int)(short)HIWORD(msg.lParam));
					m_rect.left += szDelta.cx;
					m_rect.top += szDelta.cy;

					if (m_szGrid != CSize(-1,-1))
					{
						int r=0;
						m_rect.left = (m_rect.left / m_szGrid.cx)*m_szGrid.cx;
						m_rect.top = (m_rect.top / m_szGrid.cy)*m_szGrid.cy;
					}

	                m_rect.right = m_rect.left + rectOld.Width();
					m_rect.bottom = m_rect.top + rectOld.Height();
					//oldPoint = m_rect.TopLeft();

				    if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
					{
						if (bMoved)
						{
						    m_bErase = TRUE;
							DrawTrackerRect(&rectOld, pWndClipTo, pDrawDC, pWnd);
						}
						if (!m_bFinalErase)
						    bMoved = TRUE;
				    }
				    if (!rectOld.EqualRect(&m_rect) && !m_bFinalErase)
				    {
					    m_bErase = FALSE;
						DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
					}

					pRect->CopyRect(m_rect);
				}
            }
            oldPoint = msg.lParam;
		    if (m_bFinalErase)
			    goto ExitLoop;*/
            for (index = 0; index < m_CopyPosition.GetSize(); index++) {
                pRect = m_CopyPosition.GetAt(index);
                m_rect.CopyRect(pRect);

			    rectOld = m_rect;
				/// NVL
				CPoint currPt((int)(short)LOWORD(msg.lParam),(int)(short)HIWORD(msg.lParam));
				m_rect.left += currPt.x - oldPoint.x;
                m_rect.top += currPt.y - oldPoint.y;

/*				if (m_szGrid != CSize(-1,-1))
				{
					int r=0;
					m_rect.left = (m_rect.left / m_szGrid.cx)*m_szGrid.cx;
					m_rect.top = (m_rect.top / m_szGrid.cy)*m_szGrid.cy;
				}*/

                m_rect.right = m_rect.left + rectOld.Width();
				m_rect.bottom = m_rect.top + rectOld.Height();
				//oldPoint = m_rect.TopLeft();

			    if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			    {
				    if (bMoved)
				    {
					    m_bErase = TRUE;
					    DrawTrackerRect(&rectOld, pWndClipTo, pDrawDC, pWnd);
				    }
				    if (!m_bFinalErase)
					    bMoved = TRUE;
			    }
			    if (!rectOld.EqualRect(&m_rect) && !m_bFinalErase)
			    {
				    m_bErase = FALSE;
				    DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
			    }

                pRect->CopyRect(m_rect);

            }
            oldPoint = msg.lParam;
		    if (m_bFinalErase)
			    goto ExitLoop;

			break;

		// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
                for (int index = 0; index < m_CopyPosition.GetSize(); index++) {
                    m_rect.CopyRect(m_CopyPosition.GetAt(index));
				    m_bErase = m_bFinalErase = TRUE;
					//dc.LPtoDP(&m_rect);
				    DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
                }
			}
            //RestoreRectangles ( m_rects );
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pWndClipTo != NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// update object's rect pos in case bMoved is TRUE
	if (bMoved)
		UpdateObjects ();

	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return bMoved;
}


//////////////////////////////////////////////////////////////////////
// Saved rectangle management
//////////////////////////////////////////////////////////////////////
//
//
void CMultiRectTracker::CopyPositions ()
{
    ClearPositions ();

    for (int index = 0; index < m_Objects.GetSize(); index++)
	{
		CRect mrect2 = m_Objects.GetAt(index)->rActZone;
		CRect *mrect = new CRect(mrect2);
        m_CopyPosition.Add (mrect);
	}
}

//////////////////////////////////////////////////////////////////////
//
//
void CMultiRectTracker::UpdateObjects()
{
	CClientDC dc(m_pView);
	m_pView->OnPrepareDC(&dc);
    ASSERT (m_Objects.GetSize() == m_CopyPosition.GetSize());
    for (int index = 0; index < m_Objects.GetSize(); index++)
	{
		CRect trect = m_CopyPosition.GetAt(index);
		dc.DPtoLP(&trect);
		CText3D *pObj = m_Objects.GetAt(index);
	//	if (pObj && (pObj->m_nRelatedID == -1 || (pObj->m_nRelatedID != -1 && m_Objects.GetSize() == 1)))
		{
			pObj->rActZone =trect;
		}
	}

    ClearPositions ();
}

//////////////////////////////////////////////////////////////////////
//
//
void CMultiRectTracker::ClearPositions ()
{
    for (int index = 0; index < m_CopyPosition.GetSize(); index++)
        delete m_CopyPosition.GetAt(index);
    m_CopyPosition.RemoveAll();
}


void CMultiRectTracker::DrawTrackerRect(
	LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CClientDC dc(m_pView);
	m_pView->OnPrepareDC(&dc);

	CRect rect = *lpRect;
	dc.DPtoLP(&rect);
	rect.NormalizeRect();

	// convert to client coordinates
	if (pWndClipTo != NULL)
	{
		pWnd->ClientToScreen(&rect);
		pWndClipTo->ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase)
	{
		// otherwise, size depends on the style
		if (m_nStyle & hatchedBorder)
		{
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		}
		else
		{
			size.cx = 1;
			size.cy = 1;
		}
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
		pDC->DrawDragRect(rect, size, m_rectLast, m_sizeLast);

	// remember last rectangles
	m_rectLast = rect;
	m_sizeLast = size;
}
