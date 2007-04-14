//////////////////////////////////////////////////////////////////////
// Calques 3D - a 3D Dynamic Geometry Learning Environment
// Copyright (c) 1997-2007 Nicolas Van Labeke
//////////////////////////////////////////////////////////////////////
// This file is part of Calques 3D.
// 
// Calques 3D is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.
// 
// Calques 3D is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License 
// along with Calques 3D; if not, write to The Free Software Foundation, Inc., 
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
//////////////////////////////////////////////////////////////////////
/// @file PrefSession.h
/// @brief interface of the CSaveProfileDlg class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFSESSION_H__6F760580_16F2_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFSESSION_H__6F760580_16F2_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
