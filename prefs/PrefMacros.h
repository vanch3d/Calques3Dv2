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
/// @file prefmacros.h
/// @brief interface of the CPrefMacros class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFMACROS_H__6E53D06A_064E_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFMACROS_H__6E53D06A_064E_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#define BGCEDITLISTBOX_BTN_FOLDER_ID		(UINT)(-15)

class CPrefMacros;
class CMacroUserTool;

/////////////////////////////////////////////////////////////////////////////
/// Override of the CBCGPEditListBox in order to deal with user macro items.
///
/////////////////////////////////////////////////////////////////////////////
class CBCGPMacroList : public CBCGPEditListBox
{
public:
	CPrefMacros* m_pParent;			///< A Pointer to the parent dialog box.
	BOOL m_bBrowse;					///< TRUE if the browse dialog is launched, FALSE otherwise

	CBCGPMacroList(CPrefMacros* pParent);
		
	//virtual void OnClickButton (int iButton);
	virtual BOOL OnBeforeRemoveItem (int iItem);
	virtual void OnEndEditLabel (LPCTSTR lpszLabel);
	virtual void OnSelectionChanged ();
	virtual void OnBrowse ();
};


/////////////////////////////////////////////////////////////////////////////
/// CPrefMacros
///
/////////////////////////////////////////////////////////////////////////////
class CPrefMacros : public CBCGPPropertyPage
{
// Construction
public:
	DECLARE_DYNCREATE(CPrefMacros)
	CPrefMacros();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrefMacros)
	enum { IDD = IDD_PREF_MACROS_PAGE };
	CBCGPMacroList		m_wndMacroListBox;		///< The control for the macro item list
	CEdit	m_cDef;
	CEdit	m_cName;
	CEdit	m_cPath;
	CString	m_strMacroPath;
	CString	m_strMacroDef;
	CString	m_strMacroName;
	BOOL	m_bFreeLoad;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefMacros)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void EnableControls ();

protected:
	// Generated message map functions
	//{{AFX_MSG(CPrefMacros)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFMACROS_H__6E53D06A_064E_11D5_A2FA_00D0B71C8709__INCLUDED_)
