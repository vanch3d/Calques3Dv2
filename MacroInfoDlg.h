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
/// @file MacroInfoDlg.h
/// @brief interface of the CMacroInfoDlg class.
///
///
//////////////////////////////////////////////////////////////////////
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
