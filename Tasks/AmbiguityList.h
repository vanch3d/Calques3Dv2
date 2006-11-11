//////////////////////////////////////////////////////////////////////
/// @file AmbiguityList.h
/// @brief interface of the CAmbiguityList class.
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_AMBIGUITYLIST_H__B5365C53_4E02_4D4E_A3EE_5591B58A2771__INCLUDED_)
#define AFX_AMBIGUITYLIST_H__B5365C53_4E02_4D4E_A3EE_5591B58A2771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// A custom-made popup menu containing the name of objects in an ambiguous designation.
///
/// \todo The popup menu is in fact generic. It may be a good idea to merge it with the 
/// list of selected objects and get rid of the item-by-item construction of the menu items.
/////////////////////////////////////////////////////////////////////////////
class CAmbiguityList : public CWnd
{
// Construction
public:
	CAmbiguityList();
	CAmbiguityList(const char far* title, CWnd* parent);
	CAmbiguityList(UINT sid, CWnd* parent);

// Attributes
private:
	CStringArray	m_cHelpString;
	CWnd*			m_wndParent;
	CMenu			m_wndPopup;
	int				m_nCount;
	int				m_nIndex;

// Operations
public:
	int AddString(CString str,BOOL dis = FALSE,BOOL checked = FALSE);
	int AddString(UINT sid=0,BOOL dis = FALSE,BOOL checked = FALSE);
	//int AddString(CBitmap& bmp,BOOL dis = FALSE,BOOL checked = FALSE);
	//int AddString(HMENU hpopup,CString str,BOOL dis = FALSE,BOOL checked = FALSE);
	int Execute();   // returns pick index (1 based), 0 if cancelled
	void AddHelper(int n,CString mstr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAmbiguityList)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAmbiguityList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAmbiguityList)
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBIGUITYLIST_H__B5365C53_4E02_4D4E_A3EE_5591B58A2771__INCLUDED_)
