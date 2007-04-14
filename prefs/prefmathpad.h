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
/// @file PrefMathPad.h
/// @brief interface of the CPrefMathPad class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PREFMATHPAD_H__ECD2C221_F248_11D5_A2F5_E887FB711228__INCLUDED_)
#define AFX_PREFMATHPAD_H__ECD2C221_F248_11D5_A2F5_E887FB711228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPrefMathPad dialog

/////////////////////////////////////////////////////////////////////////////
/// CPrefMathPad
///
/////////////////////////////////////////////////////////////////////////////
class CPrefMathPad : public CBCGPPropertyPage
{
// Construction
public:
	CPrefMathPad();   // standard constructor
	DECLARE_DYNCREATE(CPrefMathPad)

// Dialog Data
	//{{AFX_DATA(CPrefMathPad)
	enum { IDD = IDD_PREF_MATHPAD_PAGE };
	CComboBox	m_cUnitLinear;
	int		m_bFraction;
	CString	m_strScale;
	int		m_nUnitAngle;
	int		m_nUnitLinear;
	int		m_nPrecFraction;
	int		m_nPrecReal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefMathPad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefMathPad)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnMathpadReal();
	afx_msg void OnMathpadFraction();
	afx_msg void OnChangeMathpadPrv();
	afx_msg void OnChangeMathpadPfr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFMATHPAD_H__ECD2C221_F248_11D5_A2F5_E887FB711228__INCLUDED_)
