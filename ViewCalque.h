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
