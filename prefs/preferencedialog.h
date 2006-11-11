#if !defined(AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_)
#define AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// preferencedialog.h : header file
//

#include "PrefSession.h"
#include "PrefObjShape.h"
#include "PrefMacros.h"
#include "PrefUniverse.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog

class CPreferenceDialog : public CBCGPPropertySheet
{
	DECLARE_DYNAMIC(CPreferenceDialog)

// Construction
public:
	CPreferenceDialog(CWnd* pParentWnd, UINT iSelectPage = 0);

// Attributes
public:
	CPrefSession		m_wndPage1;		///< Reference to the Land page of the dialog
	CPrefUniverse	m_wndPage2;		///< Reference to the Income page of the dialog
	CPrefObjShape		m_wndPage3;		///< Reference to the Troops page of the dialog
	CPrefMacros	m_wndPage4;		///< Reference to the Territory page of the dialog

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferenceDialog)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_)
