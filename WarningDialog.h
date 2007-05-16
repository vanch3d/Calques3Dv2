#if !defined(AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_)
#define AFX_WARNINGDIALOG_H__3A6353D4_2689_48C5_B5B8_68FFF7F508B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WarningDialog.h : header file
//
#include "IconCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog dialog

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
	CStatic	m_wndTitle;
	CIconCtrl		m_wndIcon;
	BOOL			m_bDontAsk;
	CString			m_strMessage;
	CString	m_strTitle;
	//}}AFX_DATA
	TWarning	m_nMessage;

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
