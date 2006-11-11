#if !defined(AFX_PREFSESSION_H__6F760580_16F2_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFSESSION_H__6F760580_16F2_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefSession.h : header file
//

////////////////////////////////////////////////////////////////////////////
// CSaveProfileDlg dialog

/////////////////////////////////////////////////////////////////////////////
/// CSaveProfileDlg
///
/////////////////////////////////////////////////////////////////////////////
class CSaveProfileDlg : public CDialog
{
// Construction
public:
	CSaveProfileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveProfileDlg)
	enum { IDD = IDD_SAVE_PROFILE_DLG };
	CButton		m_wndOK;
	CListBox	m_lstProfiles;
	CString		m_strProfileName;
	CString		m_strCurSelProfileName;
	//}}AFX_DATA

	CString GetProfileName () const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveProfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveProfileDlg)
	afx_msg void OnSelchangeListProfiles();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditProfileName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CPrefSession dialog

/////////////////////////////////////////////////////////////////////////////
/// CPrefSession
///
/////////////////////////////////////////////////////////////////////////////
class CPrefSession : public CBCGPPropertyPage
{
// Construction
public:
	CPrefSession();   // standard constructor
	DECLARE_DYNCREATE(CPrefSession)

// Dialog Data
	//{{AFX_DATA(CPrefSession)
	enum { IDD = IDD_PREF_SESSION_PAGE };
	CEdit	m_cSessionDescr;
	CListBox	m_cStartList;
	CString		m_strStart;
	//}}AFX_DATA

	CStringArray	m_lPathFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefSession)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	UpdateProfileList();
	// Generated message map functions
	//{{AFX_MSG(CPrefSession)
	afx_msg void OnDestroy();
	afx_msg void OnSaveAs();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditSession();
	afx_msg void OnSelchangeReglist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSESSION_H__6F760580_16F2_11D5_A2FA_00D0B71C8709__INCLUDED_)
