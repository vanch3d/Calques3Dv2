#if !defined(AFX_EDITLABEL_H__3E5E88A0_39E4_4AFD_9A88_C125DDCCEF84__INCLUDED_)
#define AFX_EDITLABEL_H__3E5E88A0_39E4_4AFD_9A88_C125DDCCEF84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLabel.h : header file
//

class CObject3D;
class CxObject3DSet;

/////////////////////////////////////////////////////////////////////////////
/// In-place editor for the labels and text objects.
///
/////////////////////////////////////////////////////////////////////////////
class CEditLabel : public CEdit
{
// Construction
public:
	CEditLabel(CWnd* pParent,CPoint topleft,CObject3D* pObj,CxObject3DSet* pSet);

// Attributes
public:
	CPoint	m_ptAnchor;
    CRect   m_Rect;
    CRect   m_InitRect;
	CObject3D*		m_pObj;
	CxObject3DSet*	m_pSet;
	CWnd*	pOwn;

// Operations
public:
     void EndEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditLabel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void PostNcDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditLabel)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLABEL_H__3E5E88A0_39E4_4AFD_9A88_C125DDCCEF84__INCLUDED_)
