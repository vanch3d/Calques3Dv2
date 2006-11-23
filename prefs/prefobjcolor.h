#if !defined(AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_)
#define AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// prefobjcolor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefObjColor dialog

class CPrefObjColor : public CPropertyPage
{
	DECLARE_DYNCREATE(CPrefObjColor)

// Construction
public:
	CPrefObjColor();
	~CPrefObjColor();

// Dialog Data
	//{{AFX_DATA(CPrefObjColor)
	enum { IDD = IDD_PREF_COLORS_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefObjColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPrefObjColor)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_)
