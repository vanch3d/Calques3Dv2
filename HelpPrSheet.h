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
/// @file HelpPrSheet.h
/// @brief interface of the CHelpPrSheet class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_HELPPRSHEET_H__8DD10B78_82F9_11D3_9343_00105AE2D16E__INCLUDED_)
#define AFX_HELPPRSHEET_H__8DD10B78_82F9_11D3_9343_00105AE2D16E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////
/// CHelpPrSheet
///	
/// 	This class can be used as a base class for all dialogs that wish to use
/// 	context-sensitive Help. It will properly route the context Help options
/// 	to the main app's WinHelp implementation. It has the following features:
/// 	1) F1 will result in the display of Help for the whole dialog.
/// 	2) Shift + F1 will result in context Help for the control that currently
/// 	   has focus. Note that there is an alternative here. If you want 
/// 		Shift + F1 to be equivalent to clicking the '?' button, ie, you want
/// 		the floating arrow to appear, just define the symbol FLOATING_ARROW
/// 		in the .cpp file.
/// 
/// 	All you need to do is the following:
/// 	1) Build your Property Pages as you normally would.
/// 	2) If you want context-sensitive Help, make sure you check the Context
/// 	   Help check box in the More Styles tab of the resource editor.
/// 	3) For any controls that you want context-sensitive Help, make sure you
/// 	   check the Help ID check box in the General tab of the resource editor.
/// 	4) Use Class Wizard and derive from CPropertyPage as you normally would.
/// 	5) Use Class Wizard and create a new class derived from CPropertySheet.
/// 	6) Edit the .h and .cpp files and make CHelpPrSheet the base class instead
/// 	   of CPropertySheet. Note you must replace ALL refs to CPropertySheet
/// 		with CHelpPrSheet.
/// 	7) Enjoy!
/// 
/// 	Note that this presumes you have selected the Context-Sensitive Help option
/// 	when you created your project. This will do the right thing about creating
/// 	the resource.hm and YourProject.hm files.
//////////////////////////////////////////////////////////////////////////////
class CHelpPrSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CHelpPrSheet)

// Construction
public:
	CHelpPrSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CHelpPrSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpPrSheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHelpPrSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpPrSheet)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPPRSHEET_H__8DD10B78_82F9_11D3_9343_00105AE2D16E__INCLUDED_)
