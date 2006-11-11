#if !defined(AFX_PREFMACROS_H__6E53D06A_064E_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PREFMACROS_H__6E53D06A_064E_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefMacros.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefMacros dialog
class CPrefMacros;

/////////////////////////////////////////////////////////////////////////////
/// CMacroList
///
/////////////////////////////////////////////////////////////////////////////
class CMacroList : public CBCGPEditListBox
{
public:
	CPrefMacros* m_pParent;

	CMacroList(CPrefMacros* pParent) : m_pParent (pParent) {}
		
	virtual void OnSelectionChanged ();
	virtual BOOL OnBeforeRemoveItem (int iItem);
	virtual void OnAfterAddItem (int iItem);
	virtual void OnAfterRenameItem (int iItem);
	virtual void OnAfterMoveItemUp (int iItem);
	virtual void OnAfterMoveItemDown (int iItem);


protected:
	virtual void CreateNewItem ();
};

class CMacroUserTool;

/////////////////////////////////////////////////////////////////////////////
/// CPrefMacros
///
/////////////////////////////////////////////////////////////////////////////
class CPrefMacros : public CBCGPPropertyPage
{
// Construction
public:
	DECLARE_DYNCREATE(CPrefMacros)
	CPrefMacros();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrefMacros)
	enum { IDD = IDD_PREF_MACROS_PAGE };
	CEdit	m_cDef;
	CEdit	m_cName;
	CEdit	m_cPath;
	CButton	m_cBrowse;
	CMacroList		m_cMacroList;
	CString	m_strMacroPath;
	CString	m_strMacroDef;
	CString	m_strMacroName;
	BOOL	m_bFreeLoad;
	//}}AFX_DATA

	CMacroUserTool*		m_pSelTool;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefMacros)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CMacroUserTool* CreateNewTool ();
	void EnableControls ();

protected:
	void OnUpdateTool();

	// Generated message map functions
	//{{AFX_MSG(CPrefMacros)
	virtual BOOL OnInitDialog();
	afx_msg void OnMacroBrowse();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFMACROS_H__6E53D06A_064E_11D5_A2FA_00D0B71C8709__INCLUDED_)
