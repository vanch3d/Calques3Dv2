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
/// @file AboutInfoDlg.h
/// @brief interface of the CAboutCreditPage class.
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_ABOUTINFODLG_H__17174020_E8A3_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_ABOUTINFODLG_H__17174020_E8A3_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CreditsCtrl.h"	// Added by ClassView

#include "IconCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// CAboutCreditPage
///
/////////////////////////////////////////////////////////////////////////////
class CAboutCreditPage : public CPropertyPage
{
// Construction
public:
	CAboutCreditPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutCreditPage)
	enum { IDD = IDD_ABOUTCREDIT };
	//}}AFX_DATA

	CCreditsCtrl m_wndCredits;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutCreditPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutCreditPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CAboutInfoPage
///
/////////////////////////////////////////////////////////////////////////////
class CAboutInfoPage : public CPropertyPage
{
// Construction
public:
	CAboutInfoPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutInfoPage)
	enum { IDD = IDD_ABOUTPROP };
	CBCGPURLLinkButton	m_cSupport;
	CBCGPURLLinkButton	m_cURL;
	CBCGPURLLinkButton	m_cMail;
	CStatic				m_cVersion;
	CStatic				m_cInfo;
	CStatic				m_cDebug;
	CStatic				m_cCopyright;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutInfoPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutInfoPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// CAboutCalquePage
///
/////////////////////////////////////////////////////////////////////////////
class CAboutCalquePage : public CPropertyPage
{
// Construction
public:
	CAboutCalquePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutCalquePage)
	CIconCtrl		m_wndIcon;
//	CBCGPButton		m_wndff;
	enum { IDD = IDD_ABOUTCALQUE };
	//}}AFX_DATA
//	CBCGPToolBarImages m_cImages;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutCalquePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAboutCalquePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
/// CAboutDlg
///
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CAboutDlg)

// Construction
public:
	CAboutDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAboutDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
protected:
	CImageList m_imageTab;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAboutDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTINFODLG_H__17174020_E8A3_11D4_A2FA_00D0B71C8709__INCLUDED_)
