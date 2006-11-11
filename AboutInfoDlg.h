#if !defined(AFX_ABOUTINFODLG_H__17174020_E8A3_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ABOUTINFODLG_H__17174020_E8A3_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutInfoDlg.h : header file
//

#include "CreditsCtrl.h"	// Added by ClassView

#include "IconCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// ProjectRCVersion: Reading the VERSIONINFO resource.
///
/////////////////////////////////////////////////////////////////////////////
class ProjectRCVersion {
public:
	ProjectRCVersion ();
	virtual ~ProjectRCVersion ();

	BOOL GetProductName (LPSTR &prodName);
	BOOL GetProductVersion (LPSTR &prodVersion);
	BOOL GetCopyright (LPSTR &copyright);
	BOOL GetDebug (LPSTR &debug);

protected:
    LPBYTE      TransBlock;
    void FAR    *FVData;

private:
    // Don't allow this object to be copied.
    ProjectRCVersion (const ProjectRCVersion &);
    ProjectRCVersion & operator =(const ProjectRCVersion &);
};

/////////////////////////////////////////////////////////////////////////////
/// CAboutCreditPage
///
/////////////////////////////////////////////////////////////////////////////
class CAboutCreditPage : public CPropertyPage
{
// Construction
public:
	CAboutCreditPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutCreditPage)
	enum { IDD = IDD_ABOUTCREDIT };
	//}}AFX_DATA

	CCreditsCtrl m_wndCredits;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutCreditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutCreditPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CAboutInfoPage
///
/////////////////////////////////////////////////////////////////////////////
class CAboutInfoPage : public CPropertyPage
{
// Construction
public:
	CAboutInfoPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutInfoPage)
	enum { IDD = IDD_ABOUTPROP };
	CBCGPURLLinkButton	m_cURL;
	CBCGPURLLinkButton	m_cMail;
	CStatic	m_cVersion;
	CStatic	m_cInfo;
	CStatic	m_cDebug;
	CStatic	m_cCopyright;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutInfoPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutInfoPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// CAboutCalquePage
///
/////////////////////////////////////////////////////////////////////////////
class CAboutCalquePage : public CPropertyPage
{
// Construction
public:
	CAboutCalquePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutCalquePage)
	CIconCtrl		m_wndIcon;
	enum { IDD = IDD_ABOUTCALQUE };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutCalquePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutCalquePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// CAboutDlg
///
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CAboutDlg)

// Construction
public:
	CAboutDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAboutDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
protected:
	CImageList m_imageTab;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAboutDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTINFODLG_H__17174020_E8A3_11D4_A2FA_00D0B71C8709__INCLUDED_)
