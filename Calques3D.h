//////////////////////////////////////////////////////////////////////
/// @file Calques3D.h
/// @brief main header file for the CALQUES 3D application.
///
/// 
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CALQUES3D_H__1B2AE4A4_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_CALQUES3D_H__1B2AE4A4_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
/// @name Notification Messages
/// These messages are send by the framework to all the views currently open in the workspace
/// to notify them of the occurence of a particular event so that, if relevant, they could handle it. 
/////////////////////////////////////////////////////////////////////////////
//@{

/// Notify the update of the contextual help associated with a task.
/// This message is sent every time the current task (in the Universe view) needs to update its contextual help, 
/// usually in the status bar of the application.
///
/// @param WPARAM The string containing the description (task + step) of the current task (could be NULL).
/// @param LPARAM The command ID (UINT) of the current task - to retrieve its icon (could be 0).
///
/// @see CTask::GetContextualHelp(), CMainFrame::OnSetTaskMessageString()
#define WM_SHOWTASK_CTXTHELP	(WM_APP + 10)

/// Notify the update of the view's tooltip when the cursor is hovering over an object (or several). 
/// This message is sent
///
/// @param WPARAM The list (CxObject3DSet) of all objects under the cursor (could be NULL).
/// @param LPARAM TRUE if the tooltip is to be displayed, FALSE otherwise.
#define WM_SHOWOBJ_TOOLTIPS		(WM_APP + 11)

/// Notify the update of the description of the hovered object.
///
/// @param WPARAM The string containing the description of the object (could be NULL).
/// @param LPARAM The identifier of the object - to retrieve its icon (could be 0).
#define WM_SHOWOBJ_DESCRIPTION	(WM_APP + 12)

/// Notify the manual modification of the zoom factor in the Universe view.
///
/// @param WPARAM An integer (UINT) defining the percentage of the zoom factor (cannot be 0).
#define WM_UPDATEZOOM_MANUAL	(WM_APP + 15)

#define WM_UPDATEOBJ_ADD		(WM_APP + 20)		///< An object has been added to the document
#define WM_UPDATEOBJ_MOD		(WM_APP + 21)		///< The properties of an object have been modified
#define WM_UPDATEOBJ_MOV		(WM_APP + 22)		///< An object has been moved in one of the view
#define WM_UPDATEOBJ_DEL		(WM_APP + 23)		///< An object has been deleted from the document
#define WM_UPDATEOBJ_SEL		(WM_APP + 24)		///< An object has been selected in one of the view
#define WM_UPDATEOBJ_EXT		(WM_APP + 25)		///< An object has been extracted in a tracing view
#define WM_UPDATEOBJ_RED		(WM_APP + 26)		///< An object has been redefined

#define WM_UPDATEOBJ_DEP		(WM_APP + 30)		///< The dependence list of an object is recomputed
//@}


/////////////////////////////////////////////////////////////////////////////
/// Extension of the CMultiDocTemplate for accessing view's ID and Runtime class.
/////////////////////////////////////////////////////////////////////////////
class CCalques3DDocTemplate : public CMultiDocTemplate
{
public:
	CCalques3DDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) :
			CMultiDocTemplate(nIDResource,pDocClass,pFrameClass, pViewClass)
			{
			}
	
	CRuntimeClass* GetViewRT(){return m_pViewClass;};
	UINT		   GetViewID(){return m_nIDResource;};


	DECLARE_DYNAMIC(CCalques3DDocTemplate)
};

/////////////////////////////////////////////////////////////////////////////
/// Main class for the Calques 3D application.
/////////////////////////////////////////////////////////////////////////////
class CCalques3DApp : public CWinApp,
					  public CBCGPWorkspace
{
private:
	HINSTANCE		m_hinstC3DRes;		///< Handle for Calques3D resources (NOT YET USED)
	HINSTANCE		m_hinstBCGCBRes;	///< Handle for BCGContrlBar resources (for i18n purpose)

public:
	CCalques3DApp();

	// Override from CBCGWorkspace
	virtual	void OnClosingMainFrame (CBCGPFrameImpl* pFrameImpl);
	virtual void PreLoadState();
	virtual void LoadCustomState ();
	virtual void SaveCustomState ();

	static CString g_strRegistryBase;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalques3DApp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCalques3DApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


extern CCalques3DApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALQUES3D_H__1B2AE4A4_D0DE_11D4_A2FA_00D0B71C8709__INCLUDED_)
