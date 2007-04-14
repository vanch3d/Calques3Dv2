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
/// @file PreferenceDialog.h
/// @brief interface of the CPreferenceDialog class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_)
#define AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrefSession.h"
#include "PrefObjShape.h"
#include "PrefObjColor.h"
#include "PrefMacros.h"
#include "PrefUniverse.h"
#include "PrefMathPad.h"

//////////////////////////////////////////////////////////////////////
/// Main dialog for Calques 3D preferences
///
/// For the customization of Calques 3D GUI, see CCustomizeCalques3D.
//////////////////////////////////////////////////////////////////////
class CPreferenceDialog : public CBCGPPropertySheet
{
	DECLARE_DYNAMIC(CPreferenceDialog)

// Construction
public:
	CPreferenceDialog(CWnd* pParentWnd, UINT iSelectPage = 0);

// Attributes
public:
	CPrefSession	m_wndSession;		///< Control for the Session page
	CPrefUniverse	m_wndUniverse;		///< Control for the Universe page
	CPrefObjShape	m_wndShape;			///< Control for the Object Appearance page
	CPrefObjColor	m_wndColours;		///< Control for the Object Appearance page
	CPrefMathPad	m_wndMathPad;		///< Control for the MathPad page
	CPrefMacros		m_wndMacros;		///< Control for the Macros page

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferenceDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreferenceDialog();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreferenceDialog)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_)
