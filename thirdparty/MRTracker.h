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
// MultiRectTracker.h: interface for the CMultiRectTracker class.
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
#if !defined(AFX_MULTIRECTTRACKER_H__62278817_5EF6_11D3_9F79_AE25E9FEAB06__INCLUDED_)
#define AFX_MULTIRECTTRACKER_H__62278817_5EF6_11D3_9F79_AE25E9FEAB06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Just because of the templates
#include <afxtempl.h>

class CText3D; 

////////////////////////////////////////////////////////////////////////////////
// A simple container for rect position, and a base class for the user-objects.
//
/*class CAnimObject
{
public:
    CAnimObject() { m_bResize = true; m_rectPosition.SetRectEmpty (); }
    CAnimObject( LPCRECT lpSrcRect ) { m_bResize = true; m_rectPosition = lpSrcRect;}
    ~CAnimObject() {;}

    LPCRECT GetRect () { return m_rectPosition;}
    void SetRect (LPCRECT lpSrcRect) { m_rectPosition = lpSrcRect;}

    bool m_bResize;
protected:
    CRect m_rectPosition;
};*/

////////////////////////////////////////////////////////////////////////////////
// This class use some of CRectTracker functions, (hit, draw, cursor...)
// to be compatible with the Microsoft tracker drawing.
//
class CMultiRectTracker : public CRectTracker 
{
// Constructors/Destructor
public:
	CMultiRectTracker();
	virtual ~CMultiRectTracker();

// API
public:
    // Very similar to CRectTracker functions
    void Draw(CDC* pDC);
	void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
	void SetTrackerView(CView *mv);
	int HitTest( CPoint point );
    int HitTestHandles(CPoint point);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest);
	BOOL Track( CWnd* pWnd, CPoint point,
                BOOL bAllowInvert = FALSE, CWnd* pWndClipTo = NULL );

    // Specific to the CMultiRectTracker
    void Add( CText3D* pObject );
    void Remove( CText3D* pObject );
    void RemoveAll();
    bool IsEmpty() { return (m_Objects.GetSize() == 0); }
	int	 GetSize() { return m_Objects.GetSize();};
	CText3D *GetAt(int i) { return m_Objects.GetAt(i);};

	void SetGridSize(CSize ms=CSize(-1,-1)) { m_szGrid = ms;};

// Internal functions
protected:
    // The most important function !
    BOOL MultiTrackHandle ( CWnd* pWnd, CPoint point, CWnd* pWndClipTo );
    
    void CopyPositions (); // Get a copy of the rectangles
    void UpdateObjects (); // Update the objects positions, from copied rectangles
    void ClearPositions (); // erase the rectangle's copy


// Internal datas
protected:
	CSize m_szGrid;
	CView *m_pView;
    CArray <CText3D*, CText3D*> m_Objects; // Array of objects pointers
    CText3D* m_pObject; // temporary pointer used if the hit is on an object.

    CArray <CRect*, CRect*> m_CopyPosition; // array of rectangles, objects position copy.
};

#endif // !defined(AFX_MULTIRECTTRACKER_H__62278817_5EF6_11D3_9F79_AE25E9FEAB06__INCLUDED_)
