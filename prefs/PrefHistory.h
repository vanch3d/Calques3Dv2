#if !defined(AFX_PREFHISTORY_H__AB60439D_5522_41B8_8745_ACB8700E1C52__INCLUDED_)
#define AFX_PREFHISTORY_H__AB60439D_5522_41B8_8745_ACB8700E1C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefHistory dialog

class CPrefHistory : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefHistory)

// Construction
public:
	CPrefHistory();
	~CPrefHistory();

// Dialog Data
	//{{AFX_DATA(CPrefHistory)
	enum { IDD = IDD_PREF_HISTORY_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPrefHistory)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFHISTORY_H__AB60439D_5522_41B8_8745_ACB8700E1C52__INCLUDED_)
