//////////////////////////////////////////////////////////////////////
/// @file SplashScreen.h
/// @brief interface of the CBigIcon and CSplashScreen classes.
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SPLASHSCREEN_H__CCE6F7C5_6997_4F43_BAFF_4850B7A42857__INCLUDED_)
#define AFX_SPLASHSCREEN_H__CCE6F7C5_6997_4F43_BAFF_4850B7A42857__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashScreen.h : header file
//

#include "IconCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// CBigIcon
///
/////////////////////////////////////////////////////////////////////////////
class CBigIcon : public CButton
{
// Attributes
public:
	CBitmap m_bitmap;
	CSize m_sizeBitmap;

// Operations
public:
	void SizeToContent();

// Implementation
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//{{AFX_MSG(CBigIcon)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen dialog

class CSplashScreen : public CDialog
{
// Construction
public:
	CSplashScreen();   // standard constructor
	BOOL Create(CWnd* pParent);

// Dialog Data
	//{{AFX_DATA(CSplashScreen)
	CIconCtrl		m_wndIcon;
	enum { IDD = IDD_SPLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashScreen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBigIcon m_icon; // self-draw button

	// Generated message map functions
	//{{AFX_MSG(CSplashScreen)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHSCREEN_H__CCE6F7C5_6997_4F43_BAFF_4850B7A42857__INCLUDED_)
