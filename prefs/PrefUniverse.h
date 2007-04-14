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
/// @file PrefUniverse.h
/// @brief interface of the CPrefUniverse class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFUNIVERSE_H__F4121A44_E592_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFUNIVERSE_H__F4121A44_E592_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
