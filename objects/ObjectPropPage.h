#if !defined(AFX_OBJECTPROPPAGE_H__294E9964_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_OBJECTPROPPAGE_H__294E9964_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjectPropPage.h : header file


#include "..\BCGPShapeButton.h"
#include "ComboShape.h"

/////////////////////////////////////////////////////////////////////////////
/// CObjectPropPage dialog
/////////////////////////////////////////////////////////////////////////////
class CObjectPropPage : public CPropertyPage
{
// Construction
public:
	CObjectPropPage();   // standard constructor
	DECLARE_DYNCREATE(CObjectPropPage)

// Dialog Data
	//{{AFX_DATA(CObjectPropPage)
	enum { IDD = IDD_OBJECTPROP_PAGE };
	CButton				m_cHidden;
	CBCGPColorButton	m_cColor;
	CBCGPShapeButton	m_cShape;
	int					m_bHidden;
	BOOL				m_bMarked;
	CString				m_strName;
	CSpinButtonCtrl		m_wndLocusSpin;
	CEdit				m_wndLocus;
	CStatic				m_wndLocusTitle;
	int					m_nLocus;
	//}}AFX_DATA
	//CStringArray	m_cStrShape;
	CPalette		m_palColorPicker;	// Palette for color picker
	COLORREF		m_clrObj,
					m_clrDefObj;
	BOOL			m_bChanged;
	BOOL			m_bIsLocus;
	CString			m_strDefName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeObjName();
	afx_msg void OnSetObjColor();
	afx_msg void OnHiddenObject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CObjectTextPage dialog
/////////////////////////////////////////////////////////////////////////////
class CObjectTextPage : public CPropertyPage
{
// Construction
public:
	CObjectTextPage();   // standard constructor
	DECLARE_DYNCREATE(CObjectTextPage)

// Dialog Data
	//{{AFX_DATA(CObjectTextPage)
	enum { IDD = IDD_OBJECTTEXT_PAGE };
	CEdit	m_cEquation;
	CString		m_strText;
	CString		m_strEqu;
	CBCGPMenuButton	m_cInsertVar;
	CString		m_strVarName;
	BOOL		m_bMarked;
	//}}AFX_DATA
	unsigned long	m_bMod;
	LOGFONT		m_lFont;
	COLORREF	m_clrFont;
	CMenu		m_menu;
	int			m_nSelD,m_nSelF;
	CxObject3DSet	m_pSource;
	//CStrMap		m_pVarMap;
	//double		m_pValue;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectTextPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectTextPage)
	afx_msg void OnChangeEquation();
	afx_msg void OnSetFont();
	virtual BOOL OnInitDialog();
	afx_msg void OnVarMenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/// CObjectInfoPage dialog
/////////////////////////////////////////////////////////////////////////////
class CObjectInfoPage : public CPropertyPage
{
// Construction
public:
	CObjectInfoPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectInfoPage)
	enum { IDD = IDD_OBJECTINFO_PAGE };
	CEdit	m_cHelp;
	CEdit	m_cDef;
	CString	m_strObjDef;
	CString	m_strObjHelp;
	//}}AFX_DATA
	BOOL	m_bModif;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectInfoPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectInfoPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////
/// CObjectDepPage dialog
//////////////////////////////////////////////////////////////////////
class CObjectDepPage : public CPropertyPage
{
// Construction
public:
	CObjectDepPage();   // standard constructor
	CObjectDepPage(UINT nID);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectDepPage)
	enum { IDD = IDD_OBJECTDEP_PAGE };
	CListCtrl	m_cDepList;
	//}}AFX_DATA
	CxObject3DSet *m_pList;
	CImageList	 m_ImageList;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectDepPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	afx_msg void OnViewIcon();
	afx_msg void OnViewList();

	// Generated message map functions
	//{{AFX_MSG(CObjectDepPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSwitchMode();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CObjectCpntPage : public CObjectDepPage
{
// Construction
public:
	CObjectCpntPage();   // standard constructor
// Dialog Data
	//{{AFX_DATA(CObjectCpntPage)
	enum { IDD = IDD_OBJECTCPT_PAGE };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectCpntPage)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CObjectCpntPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTPROPPAGE_H__294E9964_D40C_11D4_A2FA_00D0B71C8709__INCLUDED_)
