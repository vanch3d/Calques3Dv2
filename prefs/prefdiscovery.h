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
/// @file prefdiscovery.h
/// @brief interface of the CPrefDiscovery class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFDISCOVERY_H__1E6CD9A0_C431_11D6_A2F8_CE7F861AE22B__INCLUDED_)
#define AFX_PREFDISCOVERY_H__1E6CD9A0_C431_11D6_A2F8_CE7F861AE22B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
