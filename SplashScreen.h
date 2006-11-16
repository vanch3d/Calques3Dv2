#if !defined(AFX_SPLASHSCREEN_H__CCE6F7C5_6997_4F43_BAFF_4850B7A42857__INCLUDED_)
#define AFX_SPLASHSCREEN_H__CCE6F7C5_6997_4F43_BAFF_4850B7A42857__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashScreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen dialog

class CSplashScreen : public CDialog
{
// Construction
public:
	CSplashScreen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashScreen)
	enum { IDD = IDD_SPLASH1 };
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

	// Generated message map functions
	//{{AFX_MSG(CSplashScreen)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHSCREEN_H__CCE6F7C5_6997_4F43_BAFF_4850B7A42857__INCLUDED_)
