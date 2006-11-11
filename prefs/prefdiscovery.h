#if !defined(AFX_PREFDISCOVERY_H__1E6CD9A0_C431_11D6_A2F8_CE7F861AE22B__INCLUDED_)
#define AFX_PREFDISCOVERY_H__1E6CD9A0_C431_11D6_A2F8_CE7F861AE22B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefDiscovery.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefDiscovery dialog
/////////////////////////////////////////////////////////////////////////////
/// CPrefCAS
///
/////////////////////////////////////////////////////////////////////////////
class CPrefCAS : public CObject
{
public:
	CPrefCAS();

	CString	m_strName;
	BOOL	m_bFound;
	CString	m_strVersion;
	CString	m_strPath;
	CString	m_strRuntime;
	CString	m_strScript;
};

/////////////////////////////////////////////////////////////////////////////
/// CPrefDiscovery
///
/////////////////////////////////////////////////////////////////////////////
class CPrefDiscovery : public CPropertyPage
{
// Construction
public:
	CPrefDiscovery();   // standard constructor
	~CPrefDiscovery();

	DECLARE_DYNCREATE(CPrefDiscovery)

// Dialog Data
	//{{AFX_DATA(CPrefDiscovery)
	enum { IDD = IDD_PREF_DISCOVERY_PAGE };
	CComboBox	m_wndCAS;
	int		m_nCAS;
	CString	m_strRuntime;
	CString	m_strScript;
	CString	m_strVersion;
	//}}AFX_DATA

	typedef CTypedPtrArray<CObArray,CPrefCAS*>	CCASList;
	CCASList	m_cCASList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefDiscovery)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefDiscovery)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBrowseScript();
	afx_msg void OnBrowseCmdLine();
	afx_msg void OnChangeCAS();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFDISCOVERY_H__1E6CD9A0_C431_11D6_A2F8_CE7F861AE22B__INCLUDED_)
