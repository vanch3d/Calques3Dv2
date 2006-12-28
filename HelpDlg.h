#if !defined(AFX_HELPDLG_H__8DD10B76_82F9_11D3_9343_00105AE2D16E__INCLUDED_)
#define AFX_HELPDLG_H__8DD10B76_82F9_11D3_9343_00105AE2D16E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDlg.h : header file
//

//////////////////////////////////////////////////////////////////////////////
/// CHelpDlg dialog
/// 	This class can be used as a base class for all dialogs that wish to use
/// 	context-sensitive Help. It will properly route the context Help options
/// 	to the main app's WinHelp implementation. It has the following features:
/// 	1) F1 will result in the display of Help for the whole dialog.
/// 	2) Shift + F1 will result in context Help for the control that currently
/// 	   has focus. Note that there is an alternative here. If you want 
/// 		Shift + F1 to be equivalent to clicking the '?' button, ie, you want
/// 		the floating arrow to appear, just define the symbol FLOATING_ARROW
/// 		in the .cpp file.
/// 	3) If you place a Help button with ID = IDHELP, it will display Help for
/// 	   the whole dialog.
/// 
/// 	All you need to do is the following:
/// 	1) Build your dialog as you normally would.
/// 	2) If you want context-sensitive Help, make sure you check the Context
/// 	   Help check box in the More Styles tab of the resource editor.
/// 	3) For any controls that you want context-sensitive Help, make sure you
/// 	   check the Help ID check box in the General tab of the resource editor.
/// 	4) If you want a Help button in your dialog, put one there with the ID
/// 	   = IDHELP.
/// 	5) Use Class Wizard and derive from CDialog.
/// 	6) Edit the .h and .cpp files and make CHelpDlg the base class instead
/// 	   of CDialog. Note you must replace ALL refs to CDialog with CHelpDlg.
/// 	7) Enjoy!
/// 
/// 	Note that this presumes you have selected the Context-Sensitive Help option
/// 	when you created your project. This will do the right thing about creating
/// 	the resource.hm and YourProject.hm files.
//////////////////////////////////////////////////////////////////////////////
class CHelpDlg : public CDialog
{
// Construction
public:
	CHelpDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHelpDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHelpDlg)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	afx_msg void OnHelpButton();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPDLG_H__8DD10B76_82F9_11D3_9343_00105AE2D16E__INCLUDED_)
