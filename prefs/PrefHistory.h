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
/// @file PrefHistory.h
/// @brief interface of the CPrefHistory class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFHISTORY_H__AB60439D_5522_41B8_8745_ACB8700E1C52__INCLUDED_)
#define AFX_PREFHISTORY_H__AB60439D_5522_41B8_8745_ACB8700E1C52__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// CPrefHistory dialog
///
/////////////////////////////////////////////////////////////////////////////
class CPrefHistory : public CBCGPPropertyPage
{
	DECLARE_DYNCREATE(CPrefHistory)

// Construction
public:
	CPrefHistory();
	~CPrefHistory();

// Dialog Data
	//{{AFX_DATA(CPrefHistory)
	enum { IDD = IDD_PREF_HISTORY_PAGE };
	BOOL	m_bShowHidden;
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
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFHISTORY_H__AB60439D_5522_41B8_8745_ACB8700E1C52__INCLUDED_)
