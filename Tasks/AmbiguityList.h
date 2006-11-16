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
class CAmbiguityList : public CBCGPToolBar
{
// Construction
public:
	CAmbiguityList(const char far* title, CWnd* parent);
	CAmbiguityList(UINT sid, CWnd* parent);
	virtual ~CAmbiguityList();

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
	int Execute();
	void AddHelper(int n,CString mstr);

	/////////////////////////////////////////////////////////////////////////////
	/// Override for special enabling/disabling of user-interface items associated with the toolbar
	/// @param bDisableIfNoHndler	Specifies whether the user-interface item should be disabled 
	///								if there is no handler defined in message map 
	/////////////////////////////////////////////////////////////////////////////
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)	
	{		
		CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*) GetOwner (), bDisableIfNoHndler);
	}

	/////////////////////////////////////////////////////////////////////////////
	/// Called by the framework to receive permission from a CBCGPBaseToolBar-derived object 
	/// to display (or not) the object in the list box on the toolbar customization page.   
	/////////////////////////////////////////////////////////////////////////////
	virtual BOOL AllowShowOnList () const		{	return FALSE;	}

	// Generated message map functions
protected:
	//{{AFX_MSG(CAmbiguityList)
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMBIGUITYLIST_H__B5365C53_4E02_4D4E_A3EE_5591B58A2771__INCLUDED_)
