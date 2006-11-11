//////////////////////////////////////////////////////////////////////
/// @file ChildFrm.h
/// @brief interface of the CChildFrame class
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CHILDFRM_H__1B2AE4AA_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_CHILDFRM_H__1B2AE4AA_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
/// CChildFrame
///
/////////////////////////////////////////////////////////////////////////////
class CChildFrame : public CBCGPMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__1B2AE4AA_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
