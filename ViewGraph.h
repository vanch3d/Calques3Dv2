//////////////////////////////////////////////////////////////////////
/// @file ViewGraph.h
/// @brief interface of the CViewGraph class.
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VIEWGRAPH_H__38064EB4_D667_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWGRAPH_H__38064EB4_D667_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
/// Display the dependency graph of the construction.
///
/////////////////////////////////////////////////////////////////////////////
class CViewGraph : public CScrollView
{
protected:
	CViewGraph();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewGraph)

// Attributes
public:
	CMap<int,int,CPoint,CPoint>		m_cRectPos;		///< Initial positions for objects (based on depth and index in dependence list)
	CFont m_font;
	CPoint	m_oldP;
	CObject3D*	m_Obj;
	CImageList	 m_ImageList;
	CxObject3DSet	m_ObjList;
	int				m_nShowCmd;

private:
	CToolTipCtrl	m_wndToolTip;			///< Reimplementation of the tooltip

// Operations
public:
	BOOL OnSelectObject(CObject3D *pObj);
	CCalques3DDoc* GetDocument();

	CPoint AddObject(CObject3D *pObj);
	CObject3D *GetObjectAt(CObject3D *pObj,CPoint point);

	virtual void OnDrawGraph(CDC* pDC);      // overridden to draw this view

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewGraph)
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
	virtual ~CViewGraph();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewGraph)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnGraphShowCmd(UINT);
	afx_msg void OnUpdateGraphShow(CCmdUI* pCmdUI);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnObjectProperty();
	afx_msg void OnUpdateObjectProperty(CCmdUI* pCmdUI);
	LRESULT OnUpdateObjTooltip(WPARAM wp, LPARAM lp=0);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Calques3DView.cpp
inline CCalques3DDoc* CViewGraph::GetDocument()
   { return (CCalques3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWGRAPH_H__38064EB4_D667_11D4_A2FA_00D0B71C8709__INCLUDED_)
