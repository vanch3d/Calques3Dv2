#if !defined(AFX_MACROINFODLG_H__6E53D060_064E_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_MACROINFODLG_H__6E53D060_064E_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacroInfoDlg.h : header file
//

#include "HelpDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CMacroInfoDlg dialog

/////////////////////////////////////////////////////////////////////////////
/// CMacroInfoDlg
///
/////////////////////////////////////////////////////////////////////////////
class CMacroInfoDlg : public CHelpDlg
{
// Construction
public:
	CMacroInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMacroInfoDlg)
	enum { IDD = IDD_MACROINFO_DLG };
	CButton	m_wndSaveAs;
	CString	m_strDef;
	CString	m_strHelp;
	CString	m_strName;
	CString	m_strPath;
	//}}AFX_DATA
	BOOL	m_bModify;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacroInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMacroInfoDlg)
	afx_msg void OnSaveAs();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACROINFODLG_H__6E53D060_064E_11D5_A2FA_00D0B71C8709__INCLUDED_)
