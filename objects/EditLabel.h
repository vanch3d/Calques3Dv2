//////////////////////////////////////////////////////////////////////
// Calques 3D - a 3D Dynamic Geometry Learning Environment
// Copyright (c) 1997-2007 Nicolas Van Labeke
//////////////////////////////////////////////////////////////////////
// This file is part of Calques 3D.
// 
// Calques 3D is free software; you can redistribute it and/or modify it 
// under the terms of the GNU General Public License as published by 
// the Free Software Foundation; either version 2 of the License, or 
// (at your option) any later version.
// 
// Calques 3D is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License 
// along with Calques 3D; if not, write to The Free Software Foundation, Inc., 
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA 
//////////////////////////////////////////////////////////////////////
/// @file EditLabel.h
/// @brief interface of the CEditLabel class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_EDITLABEL_H__3E5E88A0_39E4_4AFD_9A88_C125DDCCEF84__INCLUDED_)
#define AFX_EDITLABEL_H__3E5E88A0_39E4_4AFD_9A88_C125DDCCEF84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
