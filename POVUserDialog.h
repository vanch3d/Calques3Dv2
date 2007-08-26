#if !defined(AFX_POVUSERDIALOG_H__CC17383D_0C9F_40D3_B495_C2018660D375__INCLUDED_)
#define AFX_POVUSERDIALOG_H__CC17383D_0C9F_40D3_B495_C2018660D375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// POVUserDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPOVUserDialog dialog

class CPOVUserDialog : public CDialog
{
// Construction
public:
	CPOVUserDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPOVUserDialog)
	enum { IDD = IDD_POV_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPOVUserDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPOVUserDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POVUSERDIALOG_H__CC17383D_0C9F_40D3_B495_C2018660D375__INCLUDED_)
