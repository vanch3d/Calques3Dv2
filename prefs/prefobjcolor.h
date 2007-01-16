#if !defined(AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_)
#define AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// prefobjcolor.h : header file
//
#include "..\GridCtrl\GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
/// CPrefObjColor
///
/////////////////////////////////////////////////////////////////////////////
class CPrefObjColor : public CBCGPPropertyPage
{
	DECLARE_DYNCREATE(CPrefObjColor)

// Construction
public:
	CPrefObjColor();
	~CPrefObjColor();

// Dialog Data
	//{{AFX_DATA(CPrefObjColor)
	enum { IDD = IDD_PREF_COLORS_PAGE };
	BOOL	m_bGrayHidden;
	//}}AFX_DATA

private:
	CGridCtrl	m_cColorlist;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPrefObjColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void FillColorList();

	// Generated message map functions
	//{{AFX_MSG(CPrefObjColor)
	virtual BOOL OnInitDialog();
	afx_msg void OnGrayHidden();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFOBJCOLOR_H__4F83D53C_8DED_42A8_9D79_FBAE6709BD4F__INCLUDED_)
