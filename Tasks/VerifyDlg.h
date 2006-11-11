#if !defined(AFX_VERIFYDLG_H__B4086545_D393_11D5_A2FB_00D0B71C8709__INCLUDED_)
#define AFX_VERIFYDLG_H__B4086545_D393_11D5_A2FB_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerifyDlg.h : header file
//
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
