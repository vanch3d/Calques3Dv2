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
/// @file VerifyDlg.h
/// @brief interface of the CVerifyDlg class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VERIFYDLG_H__B4086545_D393_11D5_A2FB_00D0B71C8709__INCLUDED_)
#define AFX_VERIFYDLG_H__B4086545_D393_11D5_A2FB_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObject3D;
class CPoint3D;


#include "..\Objects\Vector4.h"
#include "..\HelpDlg.h"
#include "..\GridCtrl\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// CVerifyDlg
///
/////////////////////////////////////////////////////////////////////////////
class CVerifyDlg : public CHelpDlg
{
// Construction
public:
	CVerifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVerifyDlg)
	enum { IDD = IDD_VERIFY_DLG };
	//CListCtrl	m_cTargetList;
	CBCGPButton 	m_cShowCE;
	CStatic	m_cPropIcon;
	CString	m_strProp;
	CString	m_strResult;
	//}}AFX_DATA
	CImageList	m_pPropImageList;
	CImageList	m_pObjImageList;
	CImageList	m_pBtnImageList;
	UINT		m_nTask;
	CPoint3D*	m_pObjFree;
	CVector4	m_vNewPt;
	CVector4	m_vOldPt;
	BOOL		m_bIsCE;
	BOOL		m_bShowCE;
	CView*		m_pView;

	CGridCtrl	m_cColorlist;


	void AddTarget(CObject3D *pObj);
	void ClearTarget();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVerifyDlg)
	afx_msg void OnShowExample();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERIFYDLG_H__B4086545_D393_11D5_A2FB_00D0B71C8709__INCLUDED_)
