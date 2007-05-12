#if !defined(AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_)
#define AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WarningDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog dialog

class CWarningDialog : public CDialog
{
// Construction
public:
	CWarningDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWarningDialog)
	enum { IDD = IDD_MESSAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarningDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWarningDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_)
