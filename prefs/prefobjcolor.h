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
/// @file prefobjcolor.h
/// @brief interface of the CPrefObjColor class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_)
#define AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\GridCtrl\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// CPrefObjColor
///
/////////////////////////////////////////////////////////////////////////////
class CPrefObjColor : public CBCGPPropertyPage
{
	DECLARE_DYNCREATE(CPrefObjColor)

// Construction
public:
	CPrefObjColor();
	~CPrefObjColor();

// Dialog Data
	//{{AFX_DATA(CPrefObjColor)
	enum { IDD = IDD_PREF_COLORS_PAGE };
	BOOL	m_bGrayHidden;
	//}}AFX_DATA

private:
	CGridCtrl	m_cColorlist;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefObjColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillColorList();

	// Generated message map functions
	//{{AFX_MSG(CPrefObjColor)
	virtual BOOL OnInitDialog();
	afx_msg void OnGrayHidden();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_)
