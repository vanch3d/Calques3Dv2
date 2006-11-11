#if !defined(AFX_PREFOBJSHAPE_H__07937C27_D590_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFOBJSHAPE_H__07937C27_D590_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefObjShape.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefObjShape dialog
#include "..\GridCtrl\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// CPrefObjShape
///
/////////////////////////////////////////////////////////////////////////////
class CPrefObjShape : public CBCGPPropertyPage
{
// Construction
public:
	CPrefObjShape();   // standard constructor
	DECLARE_DYNCREATE(CPrefObjShape)

// Dialog Data
	//{{AFX_DATA(CPrefObjShape)
	enum { IDD = IDD_PREF_SHAPE_PAGE };
	//CComboBox	m_cRetro;
	//CComboBox	m_cFree;
	//CComboBox	m_cCstr;
	//CComboBox	m_cDep;
	//CSpinButtonCtrl	m_cSpinSize;
	BOOL	m_bGrayHidden;
	int		m_idxFree;
	int		m_idxCstr;
	int		m_idxDep;
	int		m_idxRetro;
	int		m_nPtSize;
	//}}AFX_DATA
	CGridCtrl	m_cColorlist;

	CStatic			m_wndPropListLocation;
	CBCGPPropList	m_wndProp;

	void FillColorList();

	void FillPoint();
	void FillLocus();
	void FillSphere();
	void FillLine();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefObjShape)
	public:
	virtual void OnOK();
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefObjShape)
	virtual BOOL OnInitDialog();
	afx_msg void OnGrayHidden();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFOBJSHAPE_H__07937C27_D590_11D4_A2FA_00D0B71C8709__INCLUDED_)
