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
/// @file SuppressDlg.h
/// @brief interface of the CSuppressDlg class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SUPPRESSDLG_H__294E9963_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_SUPPRESSDLG_H__294E9963_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\HelpDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSuppressDlg dialog

/////////////////////////////////////////////////////////////////////////////
/// CSuppressDlg
///
/////////////////////////////////////////////////////////////////////////////
class CSuppressDlg : public CHelpDlg
{
// Construction
public:
    BOOL m_bShwoList;

    void ShowDependList(BOOL bShow=TRUE);
    CSuppressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSuppressDlg)
    enum { IDD = IDD_SUPPRESS_DLG };
    CListCtrl   m_cDelList;
    //}}AFX_DATA
    CxObject3DSet *m_pList;
    CImageList   m_ImageList;


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSuppressDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSuppressDlg)
    afx_msg void OnShowList();
    afx_msg void OnDeleteItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeleteItemChanging(NMHDR* pNMHDR, LRESULT* pResult);
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPPRESSDLG_H__294E9963_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_)
