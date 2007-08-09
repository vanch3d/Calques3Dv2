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
/// @file WarningDialog.h
/// @brief interface of the CWarningDialog class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_)
#define AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IconCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// Implementation of the warning dialog box.
///
/////////////////////////////////////////////////////////////////////////////
class CWarningDialog : public CDialog
{
// Construction
public:
	CWarningDialog(CWnd* pParent = NULL);   // standard constructor

	//////////////////////////////////////////////////////////////////////
	/// Used to identify the nature of the message.
	//////////////////////////////////////////////////////////////////////
	enum TWarning {
			WARNING_MATHPAD	= 0		///< Document contains MathPad expression
			};

// Dialog Data
	//{{AFX_DATA(CWarningDialog)
	enum { IDD = IDD_MESSAGE };
	CStatic			m_wndTitle;		///< Control for the title of the warning
	CIconCtrl		m_wndIcon;		///< Control for the icon of the warning

	CString			m_strMessage;	///< The content of the warning message
	CString			m_strTitle;		///< The title of the warning
	BOOL			m_bDontAsk;		///< TRUE if the system won't ask the question again, FALSE otherwise
	//}}AFX_DATA
	TWarning		m_nMessage;		///< The nature of the warning

public:
	void	DoModeless(TWarning msg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWarningDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void PerformMessage(UINT action);
	// Generated message map functions
	//{{AFX_MSG(CWarningDialog)
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_)
