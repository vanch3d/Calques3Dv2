#if !defined(AFX_SUPPRESSDLG_H__294E9963_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_SUPPRESSDLG_H__294E9963_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SuppressDlg.h : header file
//

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
