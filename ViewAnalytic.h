#if !defined(AFX_VIEWANALYTIC_H__9DBCACD6_F378_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWANALYTIC_H__9DBCACD6_F378_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewAnalytic.h : header file
//

class CEquation3D;

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
	CToolTipCtrl	myToolTip;

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
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddComment(CCmdUI* pCmdUI);
	afx_msg void OnAddComment();
	afx_msg void OnAddEquation();
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	LRESULT OnUpdateObjTooltip(WPARAM wp, LPARAM lp=0);
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
