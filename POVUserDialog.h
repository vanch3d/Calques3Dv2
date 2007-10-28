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
/// @file POVUserDialog.h
/// Interface for the user-defined Points of View associated classes.
//
/// $Date: 2007-10-28 12:00:15+00 $
/// $Revision: 1.3 $
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_POVUSERDIALOG_H__CC17383D_0C9F_40D3_B495_C2018660D375__INCLUDED_)
#define AFX_POVUSERDIALOG_H__CC17383D_0C9F_40D3_B495_C2018660D375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "objects/Vector4.h"

class CPOVUserDialog;
class CxPOVObjectSet;
class CPOVUserTool;

/////////////////////////////////////////////////////////////////////////////
/// CPOVList
///
/////////////////////////////////////////////////////////////////////////////
class CPOVList : public CBCGPEditListBox
{
public:
	CPOVUserDialog* m_pParent;

	CPOVList(CPOVUserDialog* pParent=NULL) : m_pParent (pParent) {};

	virtual void OnSelectionChanged ();
	virtual BOOL OnBeforeRemoveItem (int /*iItem*/);
	virtual void OnAfterAddItem (int /*iItem*/);
	virtual void OnAfterMoveItemUp (int /*iItem*/);
	virtual void OnAfterMoveItemDown (int /*iItem*/);
	virtual void OnAfterRenameItem (int /*iItem*/);
	virtual BOOL EditItem (int iIndex);

	UINT GetStandardBtns() { return m_uiStandardBtns;};
	void SetStandardBtns(UINT stdBtns) { m_uiStandardBtns = stdBtns;};

protected:
	afx_msg void OnDblclkList (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CPOVList
///
/////////////////////////////////////////////////////////////////////////////
class CPOVUserDialog : public CDialog
{
// Construction
public:
	CPOVUserDialog(CWnd* pParent = NULL);   // standard constructor
	CxPOVObjectSet* m_pObjList;
	SProjection 	m_projParam;
// Dialog Data
	//{{AFX_DATA(CPOVUserDialog)
	enum { IDD = IDD_POV_DLG };
	CPOVList	m_wndPOVListBox;		///< The control for the macro item list
	CString		m_strTheta;
	CString		m_strPhi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPOVUserDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetPOVList(CxPOVObjectSet* mylist) {m_pObjList = mylist;};
	void SetProjParam(SProjection myParam) {m_projParam = myParam;};
	void ShowProperty(CPOVUserTool* prop);
// Implementation
protected:
	virtual void OnOK();
	virtual void OnCancel();


	// Generated message map functions
	//{{AFX_MSG(CPOVUserDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POVUSERDIALOG_H__CC17383D_0C9F_40D3_B495_C2018660D375__INCLUDED_)
