#if !defined(AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_)
#define AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// preferencedialog.h : header file
//

#include "PrefSession.h"
#include "PrefObjShape.h"
#include "PrefObjColor.h"
#include "PrefMacros.h"
#include "PrefUniverse.h"
#include "PrefMathPad.h"

//////////////////////////////////////////////////////////////////////
/// Main dialog for Calques 3D preferences
///
/// For the customization of Calques 3D GUI, see CCustomizeCalques3D.
//////////////////////////////////////////////////////////////////////
class CPreferenceDialog : public CBCGPPropertySheet
{
	DECLARE_DYNAMIC(CPreferenceDialog)

// Construction
public:
	CPreferenceDialog(CWnd* pParentWnd, UINT iSelectPage = 0);

// Attributes
public:
	CPrefSession	m_wndSession;		///< Control for the Session page
	CPrefUniverse	m_wndUniverse;		///< Control for the Universe page
	CPrefObjShape	m_wndShape;			///< Control for the Object Appearance page
	CPrefObjColor	m_wndColours;		///< Control for the Object Appearance page
	CPrefMathPad	m_wndMathPad;		///< Control for the MathPad page
	CPrefMacros		m_wndMacros;		///< Control for the Macros page

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferenceDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCEDIALOG_H__92DC2D0A_AC12_4FC5_A852_3207C558E700__INCLUDED_)
