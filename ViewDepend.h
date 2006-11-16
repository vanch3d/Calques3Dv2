#if !defined(AFX_VIEWDEPEND_H__8B4631C0_2365_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWDEPEND_H__8B4631C0_2365_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDepend.h : header file
//

////////////////////////////////////////////////////////////////////////////
// Display the dependency list of the selected object.
//
// \deprecated This view was originally used for debugging purpose and is not part of the
// standard Calques 3D release.
/////////////////////////////////////////////////////////////////////////////
class CViewDepend : public CListView
{
protected:
	CViewDepend();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDepend)

// Attributes
public:

// Operations
public:
	CCalques3DDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDepend)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewDepend();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewDepend)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Calques3DView.cpp
inline CCalques3DDoc* CViewDepend::GetDocument()
   { return (CCalques3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDEPEND_H__8B4631C0_2365_11D5_A2FA_00D0B71C8709__INCLUDED_)
