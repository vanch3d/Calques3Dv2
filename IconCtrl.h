#if !defined(AFX_ICONCTRL_H__674347E9_6B9E_44A6_BE1D_D342C6A7E98B__INCLUDED_)
#define AFX_ICONCTRL_H__674347E9_6B9E_44A6_BE1D_D342C6A7E98B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconCtrl.h : header file
//

#define ICONCTRL_CLASSNAME    _T("MFCIconCtrl")  // Window class name

/////////////////////////////////////////////////////////////////////////////
/// CIconCtrl
/////////////////////////////////////////////////////////////////////////////
class CIconCtrl : public CWnd
{
	DECLARE_DYNCREATE(CIconCtrl)

// Construction
public:
	CIconCtrl();
	void SetImage(int nb) {m_nImage = nb;};

// Attributes
public:
	int					m_nImage;	///< Zero-based index of the image to display
	CBCGPToolBarImages	m_cImage;	///< List of images

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconCtrl)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIconCtrl();
    BOOL RegisterWindowClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIconCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONCTRL_H__674347E9_6B9E_44A6_BE1D_D342C6A7E98B__INCLUDED_)
