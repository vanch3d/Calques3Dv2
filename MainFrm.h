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
/// @file MainFrm.h
/// Interface of the CMainFrame class.
///
/// $Date: 2007-11-11 11:10:32+00 $
/// $Revision: 1.12 $
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MAINFRM_H__1B2AE4A8_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_MAINFRM_H__1B2AE4A8_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "PropertyBar.h"
#include "FormatToolBar.h"
#include "ParamGeoBar.h"

/////////////////////////////////////////////////////////////////////////////
/// The MDI main frame window of the application.
///
/////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
protected: 
	CBCGPMenuBar			m_wndMenuBar;		///< Control for the menu bar
	CBCGPToolBar			m_wndToolBar;		///< Control for the main toolbar
	CBCGPStatusBar			m_wndStatusBar;		///< Control for the status bar

	CFormatToolBar			m_wndTextBar;		///< Control for the Format toolbar
	CBCGPToolBar			m_wndVisuBar;		///< Control for the Visualisation toolbar
	CBCGPToolBar			m_wndObjBar;		///< Control for the Object toolbar
	CBCGPToolBar			m_wndConstrBar;		///< Control for the Construction toolbar
	CBCGPToolBar			m_wndExplorBar;		///< Control for the Exploration toolbar

	CBCGPDropDownToolBar	m_wndPointOnBar;	///< Control for the "Point on" dropdown menu
	CBCGPDropDownToolBar	m_wndIntersectBar;	///< Control for the "Intersection" dropdown menu
	CBCGPDropDownToolBar	m_wndDivPointBar;	///< Control for the "Various Point" dropdown menu

	CPropertyBar			m_wndPropertyBar;	///< Control for the modeless object properties dialog
	CDependentBar			m_wndDependentBar;	///< Control for the modeless object properties dialog
	CParamGeoBar			m_wndParamGeoBar;	///< Control for the modeless ParamGeo interface

	CBCGPToolBarImages		m_UserImages;		///< List of user-defined images (for configuring the interface)

	CFont					m_fontVertCaption;	///< Font for the vertical caption in the File menu
	CBitmap					m_bmpVertCaption;	///< Logo for the vertical caption in the File menu

	UINT					m_nAppLook;			///< Current look-and-feel of the application

public:
	CComPtr<ISensor>		m_g3DSensor;
	CComPtr<IKeyboard>		m_g3DKeyboard;

// Operations
public:
	/////////////////////////////////////////////////////////////////////////////
	/// Access to the Property bar
	/////////////////////////////////////////////////////////////////////////////
	CPropertyBar*		GetPropertyBar() { return  &m_wndPropertyBar;};
	/////////////////////////////////////////////////////////////////////////////
	/// Access to the Dependent bar
	/////////////////////////////////////////////////////////////////////////////
	CPropertyBar*		GetDependentBar() { return  &m_wndDependentBar;};
	/////////////////////////////////////////////////////////////////////////////
	/// Access to the ParamGeo bar
	/////////////////////////////////////////////////////////////////////////////
	CBCGPDockingControlBar*		GetParamGeoBar() { return  &m_wndParamGeoBar;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	virtual void OnDrawMenuLogo (CDC* pDC, CBCGPPopupMenu* pMenu, const CRect& rectLogo);

public:
	CFrameWnd* LaunchView(CDocument* pDoc,UINT nID);
	void		SetWarningMsg(BOOL act=FALSE);
private:
	CFrameWnd*  LaunchViews(CDocument* pDoc,const CRuntimeClass* pClass,UINT nID);
	CFrameWnd*  LaunchUniverse(CDocument* pDoc);
	CFrameWnd*	LaunchCalques(CDocument* pDoc,UINT nID);

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewUniverse();
	afx_msg void OnViewHistorique();
	afx_msg void OnViewGraph();
	afx_msg void OnViewAnalytic();
	afx_msg void OnViewRendering();
	afx_msg void OnHelpKeyboardmap();
	afx_msg void OnViewFullScreen();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnViewDepend();
	afx_msg void OnViewCustomize();
	afx_msg void OnViewOptions();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg LRESULT OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp);
	afx_msg void OnUpdateViews(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMode(CCmdUI *pCmdUI);
	afx_msg LRESULT OnSetTaskMessageString(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetObjectSelectionString(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewObject();
	afx_msg void OnUpdateViewObject(CCmdUI* pCmdUI);
	afx_msg void OnViewConstruction();
	afx_msg void OnUpdateViewConstruction(CCmdUI* pCmdUI);
	afx_msg void OnViewVisualisation();
	afx_msg void OnUpdateViewVisualisation(CCmdUI* pCmdUI);
	afx_msg void OnViewExploration();
	afx_msg void OnUpdateViewExploration(CCmdUI* pCmdUI);
	afx_msg void OnViewPropertyBar();
	afx_msg void OnUpdateViewPropertyBar(CCmdUI* pCmdUI);
	afx_msg void OnViewDependentBar();
	afx_msg void OnUpdateViewDependentBar(CCmdUI* pCmdUI);
	afx_msg void OnViewParamGeoBar();
	afx_msg void OnUpdateViewParamGeoBar(CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCalque(UINT id);

//	afx_msg void OnPlacementRestore();
//	afx_msg void OnPlacementSave();
//	afx_msg void OnUpdatePlacement(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1B2AE4A8_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
