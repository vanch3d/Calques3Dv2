//////////////////////////////////////////////////////////////////////
/// @file ViewHisto.h
/// @brief interface of the CViewHisto class.
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VIEWHISTO_H__1B2AE4DB_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_VIEWHISTO_H__1B2AE4DB_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewHisto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
/// Display the history of the construction in a list-like fashion.
///
/////////////////////////////////////////////////////////////////////////////
class CViewHisto : public CTreeView//CListView
{
protected:
	CViewHisto();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewHisto)

// Attributes
public:

protected:
	CImageList		m_ImageList;	///< List of icons for drawing the objects
	CImageList		m_treeicon;		///< List of icons for the status indicators
	BOOL			m_bRefit;		///< TRUE for updating the tree control, FALSE for blocking it

private:
	CRect rcBounds;
	int m_nCharWidth;
	int m_nRowHeight;
	int m_nRowsPerPage;
	HANDLE hDIB;
	WINDOWPLACEMENT WndPlace;
// Operations
public:
	CCalques3DDoc* GetDocument();
	HANDLE DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHisto)
	public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewHisto();

	void ExpandBranch( HTREEITEM hti );
	void CollapseBranch( HTREEITEM hti );
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CString OnExportSymbolic();
	CString OnExportDot();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewHisto)
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnExpandAll();
	afx_msg void OnCollapseAll();
	afx_msg void OnHistoryExport();
	afx_msg void OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Calques3DView.cpp
inline CCalques3DDoc* CViewHisto::GetDocument()
   { return (CCalques3DDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWHISTO_H__1B2AE4DB_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
