#if !defined(AFX_VIEW3DRENDER_H__B2DB87D7_9EE1_4190_96A2_D923EB8198F0__INCLUDED_)
#define AFX_VIEW3DRENDER_H__B2DB87D7_9EE1_4190_96A2_D923EB8198F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View3DRender.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView3DRender view

class CView3DRender : public CView
{
protected:
	CView3DRender();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView3DRender)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView3DRender)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CView3DRender();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CView3DRender)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW3DRENDER_H__B2DB87D7_9EE1_4190_96A2_D923EB8198F0__INCLUDED_)
