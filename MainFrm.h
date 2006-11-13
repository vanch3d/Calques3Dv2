//////////////////////////////////////////////////////////////////////
/// @file MainFrm.h
/// @brief interface of the CMainFrame class
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MAINFRM_H__1B2AE4A8_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_MAINFRM_H__1B2AE4A8_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "PropertyBar.h"

/////////////////////////////////////////////////////////////////////////////
/// CMainFrame
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

	CBCGPToolBar			m_wndVisuBar;		///< Control for the Visualisation toolbar
	CBCGPToolBar			m_wndObjBar;		///< Control for the Object toolbar
	CBCGPToolBar			m_wndConstrBar;		///< Control for the Construction toolbar
	CBCGPToolBar			m_wndExplorBar;		///< Control for the Exploration toolbar

	CBCGPDropDownToolBar	m_wndPointOnBar;	///< Control for the "Point on" dropdown menu
	CBCGPDropDownToolBar	m_wndIntersectBar;	///< Control for the "Intersection" dropdown menu
	CBCGPDropDownToolBar	m_wndDivPointBar;	///< Control for the "Various Point" dropdown menu

	CPropertyBar			m_wndPropertyBar;	///< Control for the modeless object properties dialog

	CBCGPToolBarImages		m_UserImages;		///< List of user-defined images (for configuring the interface)

	CFont					m_fontVertCaption;	///< Font for the vertical caption in the File menu
	CBitmap					m_bmpVertCaption;	///< Logo for the vertical caption in the File menu

	UINT					m_nAppLook;			///< Current look-and-feel of the application

// Operations
public:
	CPropertyBar*		GetPropertyBar() { return  &m_wndPropertyBar;};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


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
	//}}AFX_MSG
	afx_msg void OnViewDepend();
	afx_msg void OnViewCustomize();
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
	afx_msg void OnViewCalque(UINT id);
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
	afx_msg void OnWindowManager();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);

//	afx_msg void OnPlacementRestore();
//	afx_msg void OnPlacementSave();
//	afx_msg void OnUpdatePlacement(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	
	void LaunchViews(const CRuntimeClass* pClass,UINT nID);
	virtual BOOL OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup);
	virtual void OnDrawMenuLogo (CDC* pDC, CBCGPPopupMenu* pMenu, const CRect& rectLogo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1B2AE4A8_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
