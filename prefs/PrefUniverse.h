#if !defined(AFX_PREFUNIVERSE_H__F4121A44_E592_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFUNIVERSE_H__F4121A44_E592_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefUniverse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefUniverse dialog
class CPrefUniverse;

/////////////////////////////////////////////////////////////////////////////
/// CPOVList
///
/////////////////////////////////////////////////////////////////////////////
class CPOVList : public CBCGPEditListBox
{
public:
	CPrefUniverse* m_pParent;

	CPOVList(CPrefUniverse* pParent=NULL) : m_pParent (pParent) {};

	virtual void OnSelectionChanged ();
	virtual BOOL EditItem (int iIndex);

protected:
	afx_msg void OnDblclkList (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// CPrefUniverse
///
/////////////////////////////////////////////////////////////////////////////
class CPrefUniverse : public CBCGPPropertyPage
{
// Construction
public:
	CPrefUniverse();   // standard constructor
	DECLARE_DYNCREATE(CPrefUniverse)

// Dialog Data
	//{{AFX_DATA(CPrefUniverse)
	enum { IDD = IDD_PREF_UNIVERSE_PAGE };
	CBCGPColorButton		m_wndTraceColor;
//	CPOVList			m_cPOVList;
	BOOL	m_bMagnet;
	BOOL	m_bSynchro;
	int		m_nRefInit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefUniverse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefUniverse)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFUNIVERSE_H__F4121A44_E592_11D4_A2FA_00D0B71C8709__INCLUDED_)
