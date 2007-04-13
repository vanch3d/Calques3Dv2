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
// MainFrm.cpp: implementation of the CMainFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Calques3D.h"

#include "MainFrm.h"

#include "Calques3DDoc.h"

#include "ViewHisto.h"
#include "ViewGraph.h"
#include "ViewUniv.h"
#include "ViewCalque.h"
#include "ViewAnalytic.h"
#include "ViewDepend.h"
#include "View3DRender.h"

#include "BCGPSliderButton.h"

#include "objects\Object3d.h"
//#include "objects\ObjectPropPage.h"

#include "prefs\PrefObjShape.h"
#include "prefs\PrefUniverse.h"
#include "prefs\PrefMacros.h"
#include "prefs\PrefSession.h"
#include "prefs\PrefMathPad.h"
#include "prefs\PrefDiscovery.h"

#include "prefs\preferencedialog.h"

#include "prefs\Prefs.h"

#include "prefs\UndoBar.h"

//#include "Splash.h"

#include "CustomizeCalques3D.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

const int  iMaxUserToolbars		= 15;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_UNIVERSE, OnViewUniverse)
	ON_COMMAND(ID_VIEW_HISTORIQUE, OnViewHistorique)
	ON_COMMAND(ID_VIEW_GRAPH, OnViewGraph)
	ON_COMMAND(ID_VIEW_ANALYTIC, OnViewAnalytic)
	ON_COMMAND(ID_VIEW_RENDERING, OnViewRendering)
	ON_COMMAND(ID_HELP_KEYBOARDMAP, OnHelpKeyboardmap)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullScreen)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	// Global help commands
	//ON_COMMAND(ID_VIEW_DEPENDLSIT, OnViewDepend)
	ON_COMMAND(ID_HELP_FINDER, CBCGPMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_CONTEXT_HELP, CBCGPMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CBCGPMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP_INDEX, CBCGPMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP, CBCGPMDIFrameWnd::OnHelp)
	ON_MESSAGE(WM_SHOWTASK_CTXTHELP, OnSetTaskMessageString)
	ON_MESSAGE(WM_SHOWOBJ_DESCRIPTION, OnSetObjectSelectionString)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND_EX_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)
	ON_REGISTERED_MESSAGE(BCGM_CUSTOMIZEHELP, OnHelpCustomizeToolbars)
	ON_COMMAND(ID_VIEW_OBJECT, OnViewObject)
	ON_COMMAND(ID_VIEW_CONSTRUCTION, OnViewConstruction)
	ON_COMMAND(ID_VIEW_VISUALISATION, OnViewVisualisation)
	ON_COMMAND(ID_VIEW_EXPLORATION, OnViewExploration)
	ON_COMMAND(ID_VIEW_PROPERTYBAR, OnViewPropertyBar)
	ON_COMMAND(ID_VIEW_DEPENDENTBAR, OnViewDependentBar)
	ON_COMMAND_RANGE(ID_VIEW_CALQUE,ID_VIEW_CALQUE4, OnViewCalque)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_CALQUE,ID_VIEW_CALQUE4, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, OnUpdateMode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UNIVERSE, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTORIQUE, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANALYTIC, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERING, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPH, OnUpdateViews)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECT, OnUpdateViewObject)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONSTRUCTION, OnUpdateViewConstruction)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VISUALISATION, OnUpdateViewVisualisation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EXPLORATION, OnUpdateViewExploration)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTYBAR, OnUpdateViewPropertyBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEPENDENTBAR, OnUpdateViewDependentBar)
//	ON_COMMAND(ID_VIEW_PLACEMENT_RESTORE, OnPlacementRestore)
//	ON_COMMAND(ID_VIEW_PLACEMENT_SAVE, OnPlacementSave)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_PLACEMENT_SAVE, OnUpdatePlacement)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_PLACEMENT_RESTORE, OnUpdatePlacement)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2005, OnAppLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_2000, ID_VIEW_APPLOOK_VS2005, OnUpdateAppLook)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_ICON,		// status icon
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROGRESS,	// progress bar
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	ID_INDICATOR_MODE,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_nAppLook = theApp.GetInt (_T("ApplicationLook"), ID_VIEW_APPLOOK_2003);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnAppLook (m_nAppLook);

	//CBCGToolBar::EnableQuickCustomization ();

	// Load toolbar user images:
	if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
	{
		TRACE(_T("Failed to load user images\n"));
		ASSERT (FALSE);
	}
	else
	{
		CBCGPToolBar::SetUserImages (&m_UserImages);
	}

	// TODO: Define your own basic commands. Be sure, that each pulldown 
	// menu have at least one basic command.

/*	CList<UINT, UINT>	lstBasicCommands;

	lstBasicCommands.AddTail (ID_VIEW_TOOLBARS);
	lstBasicCommands.AddTail (ID_FILE_NEW);
	lstBasicCommands.AddTail (ID_FILE_OPEN);
	lstBasicCommands.AddTail (ID_FILE_SAVE);
	lstBasicCommands.AddTail (ID_FILE_PRINT);
	lstBasicCommands.AddTail (ID_APP_EXIT);
	lstBasicCommands.AddTail (ID_EDIT_CUT);
	lstBasicCommands.AddTail (ID_EDIT_PASTE);
	lstBasicCommands.AddTail (ID_EDIT_UNDO);
	lstBasicCommands.AddTail (ID_APP_ABOUT);
	lstBasicCommands.AddTail (ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail (ID_VIEW_CUSTOMIZE);
	lstBasicCommands.AddTail (ID_WINDOW_TILE_HORZ);

	CBCGToolBar::SetBasicCommands (lstBasicCommands);*/
	
	//----------------------
	// Create dropdown toolbars
	//----------------------
	if (!m_wndPointOnBar.Create(this,
		WS_CHILD|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_POINTON_TB) || 
		!m_wndPointOnBar.LoadToolBar (IDR_POINTON_TB))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

/*	int nb = m_wndPointOnBar.GetCount();
	for (int i=0;i<nb;i++)
	{
		CBCGToolbarButton* pBut = m_wndPointOnBar.GetButton(i);
		m_wndPointOnBar.OnSetDefaultButtonText(pBut);
		pBut->m_bText = TRUE;
	}*/

	if (!m_wndIntersectBar.Create(this,
		WS_CHILD|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_INTERSECT_TB) || 
		!m_wndIntersectBar.LoadToolBar (IDR_INTERSECT_TB))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	if (!m_wndDivPointBar.Create(this,
		WS_CHILD|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|
		CBRS_GRIPPER | CBRS_BORDER_3D,
		IDR_POINTDIV_TB) || 
		!m_wndDivPointBar.LoadToolBar (IDR_POINTDIV_TB))
	{
		TRACE0("Failed to create build toolbar\n");
		return FALSE;      // fail to create
	}

	//----------------------
	// Create Menu toolbar:
	//----------------------
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC); 
	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() & ~CBRS_GRIPPER);

	//----------------------
	// Create Status toolbar:
	//----------------------
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneStyle (0, SBPS_NOBORDERS);				// icon pane
	m_wndStatusBar.SetPaneStyle (1, SBPS_STRETCH | SBPS_NOBORDERS); // status line
	m_wndStatusBar.SetPaneStyle (2, SBPS_NOBORDERS);				// progress pane
	m_wndStatusBar.SetPaneWidth (2, 80);
	m_wndStatusBar.EnablePaneDoubleClick ();

	//----------------------
	// Create MainFrame toolbar:
	//----------------------
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);

	//----------------------
	// Create Format toolbar:
	//----------------------
	if (!m_wndTextBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_BORDER_3D
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1),
			ID_VIEW_FORMAT) ||
		!m_wndTextBar.LoadToolBar(IDR_FORMAT_TB))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strTextBarTitle;
	strTextBarTitle.LoadString (IDR_FORMAT_TB);
	m_wndTextBar.SetWindowText (strTextBarTitle);

	//----------------------
	// Create Objects toolbar:
	//----------------------
	if (!m_wndObjBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1),
			ID_VIEW_OBJECT) ||
		!m_wndObjBar.LoadToolBar(IDR_OBJECTS_TB))
	{
		TRACE0("Failed to create Objects Toolbar\n");
		return -1;      // fail to create
	}

	CString strObjectToolbarTitle;
	strObjectToolbarTitle.LoadString (IDR_OBJECTS_TB);
	m_wndObjBar.SetWindowText (strObjectToolbarTitle);

	//----------------------
	// Create Exploration toolbar:
	//----------------------
	if (!m_wndExplorBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1),
			ID_VIEW_EXPLORATION) ||
		!m_wndExplorBar.LoadToolBar(IDR_EXPLORATION_TB))
	{
		TRACE0("Failed to create Exploration Toolbar\n");
		return -1;      // fail to create
	}

	CString strExplorToolbarTitle;
	strExplorToolbarTitle.LoadString (IDR_EXPLORATION_TB);
	m_wndExplorBar.SetWindowText (strExplorToolbarTitle);

	//----------------------
	// Create Construction toolbar:
	//----------------------
	if (!m_wndConstrBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1),
			ID_VIEW_CONSTRUCTION) ||
		!m_wndConstrBar.LoadToolBar(IDR_CONSTRUCTION_TB))
	{
		TRACE0("Failed to create Construction Toolbar\n");
		return -1;      // fail to create
	}

	CString strConstrToolbarTitle;
	strConstrToolbarTitle.LoadString (IDR_CONSTRUCTION_TB);
	m_wndConstrBar.SetWindowText (strConstrToolbarTitle);

	//----------------------
	// Create Visualisation toolbar:
	//----------------------
	if (!m_wndVisuBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1, 1, 1, 1),
			ID_VIEW_VISUALISATION) ||
		!m_wndVisuBar.LoadToolBar(IDR_VISUALISATION_TB))
	{
		TRACE0("Failed to create Visualisation Toolbar\n");
		return -1;      // fail to create
	}

	CString strVisuToolbarTitle;
	strVisuToolbarTitle.LoadString (IDR_VISUALISATION_TB);
	m_wndVisuBar.SetWindowText (strVisuToolbarTitle);

	//----------------------
	// Create Property toolbar:
	//----------------------
	CString strPropertyTitle;
	strPropertyTitle.LoadString (IDR_PROPERTY_TB);
	if (!m_wndPropertyBar.Create (strPropertyTitle, this, CSize (200, 200),
		TRUE /* Has gripper */, ID_VIEW_PROPERTYBAR,
		WS_CHILD | WS_VISIBLE | CBRS_RIGHT|CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create workspace bar\n");
		return -1;      // fail to create
	}
	strPropertyTitle.LoadString (IDR_DEPENDENT_TB);
	if (!m_wndDependentBar.Create (strPropertyTitle, this, CSize (200, 200),
		TRUE /* Has gripper */, ID_VIEW_DEPENDENTBAR,
		WS_CHILD | WS_VISIBLE | CBRS_RIGHT|CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create workspace bar\n");
		return -1;      // fail to create
	}

	HICON hPropertyIcon = (HICON) ::LoadImage (::AfxGetResourceHandle (), 
		MAKEINTRESOURCE (IDR_PROPERTY_TB),
				IMAGE_ICON, ::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0);
	m_wndPropertyBar.SetIcon (hPropertyIcon, FALSE);
	hPropertyIcon = (HICON) ::LoadImage (::AfxGetResourceHandle (), 
		MAKEINTRESOURCE (IDR_DEPENDENT_TB),
				IMAGE_ICON, ::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0);
	m_wndDependentBar.SetIcon (hPropertyIcon, FALSE);


	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_RIGHT);

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndObjBar.EnableDocking(CBRS_ALIGN_ANY); 
	m_wndConstrBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndExplorBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndVisuBar.EnableDocking(CBRS_ALIGN_ANY); 
	m_wndPropertyBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDependentBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTextBar.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndTextBar);
	DockControlBar(&m_wndExplorBar);
	DockControlBarLeftOf(&m_wndToolBar,&m_wndTextBar);
	DockControlBarLeftOf(&m_wndConstrBar,&m_wndExplorBar);
	DockControlBarLeftOf(&m_wndObjBar,&m_wndConstrBar);
	DockControlBarLeftOf(&m_wndVisuBar,&m_wndObjBar);

	//DockControlBarLeftOf(&m_wndConstrBar,&m_wndToolBar);			///// NVL
	//DockControlBarLeftOf(&m_wndObjBar,&m_wndToolBar);			///// NVL
	//DockControlBarLeftOf(&m_wndExplorBar,&m_wndToolBar);			///// NVL
	//DockControlBarLeftOf(&m_wndVisuBar,&m_wndToolBar);			///// NVL
	DockControlBar(&m_wndPropertyBar);
	//DockControlBar(&m_wndDependentBar);
	//m_wndDependentBar.DockToWindow (&m_wndPropertyBar, CBRS_ALIGN_BOTTOM);
	CBCGPDockingControlBar* pTabbedBar = NULL; 
	m_wndDependentBar.AttachToTabWnd (&m_wndPropertyBar, DM_SHOW, FALSE, &pTabbedBar);



	//-----------------------
	// Reload toolbar images:
	//-----------------------
	CBCGPToolBar::ResetAllImages ();

	m_wndPointOnBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_POINTON_TB : IDR_POINTON_TB);
	m_wndIntersectBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_INTERSECT_TB : IDR_INTERSECT_TB);
	m_wndDivPointBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_POINTDIV_TB : IDR_POINTDIV_TB);
	m_wndToolBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_MAINFRAME24 : IDR_MAINFRAME);
	m_wndTextBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_FORMAT_TB24 : IDR_FORMAT_TB);
	m_wndObjBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_OBJECTS_TB : IDR_OBJECTS_TB);
	m_wndExplorBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_EXPLORATION_TB : IDR_EXPLORATION_TB);
	m_wndConstrBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_CONSTRUCTION_TB : IDR_CONSTRUCTION_TB);
	m_wndVisuBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_VISUALISATION_TB : IDR_VISUALISATION_TB);
	m_wndPropertyBar.m_wndToolBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_PROPERTY_TB24 : IDR_PROPERTY_TB);
	m_wndDependentBar.m_wndToolBar.LoadBitmap (theApp.m_bHiColorIcons ? IDR_PROPERTY_TB24 : IDR_PROPERTY_TB);

	//CBCGPToolBar::AddToolBarForImageCollection (IDR_MENU_IMAGES,theApp.m_bHiColorIcons ? IDB_MENUIMAGES24 : 0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_COMMANDS_TB,theApp.m_bHiColorIcons ? IDR_COMMANDS_TB24 : 0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_INTERSECT_TB,0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_POINTON_TB,0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_POINTDIV_TB,0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_GRAPH_TB,0);
	CBCGPToolBar::AddToolBarForImageCollection(IDR_VERIFY_TB,0);

	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,uiFirstUserToolBarId,uiLastUserToolBarId);

	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOW_MANAGER, TRUE);
	EnableFullScreenMode (ID_VIEW_FULLSCREEN);
	EnableFullScreenMainMenu(TRUE);

	//-----------------------------------
	// Create "File" menu logo resources:
	//-----------------------------------
	m_bmpVertCaption.LoadBitmap (IDB_MENU_LOGO);

	//----------------------
	// Create vertical font:
	//----------------------
	CFont font;
	if (font.CreateStockObject (DEFAULT_GUI_FONT))
	{
		LOGFONT lf;
		memset (&lf, 0, sizeof (LOGFONT));

		if (font.GetLogFont (&lf) != 0)
		{
			lf.lfOrientation = 900;
			lf.lfEscapement = 900;
			
			lf.lfHeight = -25;
			lf.lfWeight = 700;
			lf.lfItalic = FALSE;
			lf.lfQuality=PROOF_QUALITY;
			_tcscpy (lf.lfFaceName, _T("Arial"));

			m_fontVertCaption.CreateFontIndirect (&lf);
		}
	}

	// Setup undo actions:
/*	CUndoButton::m_lstActions.AddTail (_T("Item 1"));
	CUndoButton::m_lstActions.AddTail (_T("Item 2"));
	CUndoButton::m_lstActions.AddTail (_T("Item 3"));
	CUndoButton::m_lstActions.AddTail (_T("Item 4"));
	CUndoButton::m_lstActions.AddTail (_T("Item 5"));
	CUndoButton::m_lstActions.AddTail (_T("Item 6"));
	CUndoButton::m_lstActions.AddTail (_T("Item 7"));
	CUndoButton::m_lstActions.AddTail (_T("Item 8"));*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
//		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewOptions()
{
	CPreferenceDialog propSheet(this);
	propSheet.DoModal();
}

void CMainFrame::OnViewCustomize()
{
	


	CList<CRuntimeClass*,CRuntimeClass*> plistCustomPages;

	plistCustomPages.AddTail(RUNTIME_CLASS(CPrefSession));
	plistCustomPages.AddTail(RUNTIME_CLASS(CPrefObjShape));
	plistCustomPages.AddTail(RUNTIME_CLASS(CPrefUniverse));
	plistCustomPages.AddTail(RUNTIME_CLASS(CPrefMathPad));
	plistCustomPages.AddTail(RUNTIME_CLASS(CPrefMacros));
	//plistCustomPages.AddTail(RUNTIME_CLASS(CPrefDiscovery));

	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CCustomizeCalques3D* pDlgCust = new CCustomizeCalques3D (
		this
		,TRUE, // Automatic menus scaning 
		//BCGCUSTOMIZE_MENU_SHADOWS | 
		BCGCUSTOMIZE_TEXT_LABELS | 
		//BCGCUSTOMIZE_LOOK_2000 | 
		//BCGCUSTOMIZE_MENU_ANIMATIONS | 
		BCGCUSTOMIZE_CONTEXT_HELP 
		// | BCGCUSTOMIZE_SELECT_SKINS 
		,&plistCustomPages);


	pDlgCust->InitCalques3DPrefs();
	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (LOWORD (lp), HIWORD(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

void CMainFrame::OnToolsViewUserToolbar (UINT uiId)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		ASSERT (FALSE);
		return;
	}

	ShowControlBar (pUserToolBar, !(pUserToolBar->IsVisible()), FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
	CBCGPToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	pCmdUI->Enable ();
	pCmdUI->SetCheck (pUserToolBar->IsVisible());
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	UINT uiToolBarId = (UINT) wp;
	switch (uiToolBarId)
	{
	case IDR_MAINFRAME:
		{
			//-------------------------
			// Setup Open button
			//-------------------------
			CMenu menu;
			menu.LoadMenu (IDR_OPEN);

			CMenu* pPopupMenu = menu.GetSubMenu (0);
			VERIFY (pPopupMenu != NULL);

			m_wndToolBar.ReplaceButton (ID_FILE_OPEN, CBCGPToolbarMenuButton 
				(ID_FILE_OPEN, pPopupMenu->GetSafeHmenu (), 
				CImageHash::GetImageOfCommand (ID_FILE_OPEN, FALSE)));

		}
		{
			//-------------------------------------
			// Replace "Undo" button by menubutton:
			//-------------------------------------
			//m_wndToolBar.ReplaceButton (ID_EDIT_UNDO, CUndoButton (ID_EDIT_UNDO, "&Undo"));
			//m_wndToolBar.ReplaceButton (ID_EDIT_REDO, CRedoButton (ID_EDIT_REDO, "&Redo"));
			CMenu menu;
			menu.LoadMenu (IDR_UNDO);

			CMenu* pPopupMenu = menu.GetSubMenu (0);
			VERIFY (pPopupMenu != NULL);

			m_wndToolBar.ReplaceButton (ID_EDIT_UNDO, CBCGPToolbarMenuButton 
				(ID_EDIT_UNDO, pPopupMenu->GetSafeHmenu (), 
				CImageHash::GetImageOfCommand (ID_EDIT_UNDO, FALSE)));

			m_wndToolBar.ReplaceButton (ID_EDIT_REDO, CBCGPToolbarMenuButton 
				(ID_EDIT_REDO, pPopupMenu->GetSafeHmenu (), 
				CImageHash::GetImageOfCommand (ID_EDIT_REDO, FALSE)));
		}
		break;
	case IDR_VISUALISATION_TB:
		{
			//-------------------------
			// Setup Zoom button
			//-------------------------
			CMenu menu;
			menu.LoadMenu (IDR_ZOOM);

			CMenu* pPopupMenu = menu.GetSubMenu (0);
			VERIFY (pPopupMenu != NULL);

			m_wndVisuBar.ReplaceButton (ID_VISUALISATION_ZOOM, CBCGPToolbarMenuButton 
				(ID_VISUALISATION_ZOOM, pPopupMenu->GetSafeHmenu (), 
				CImageHash::GetImageOfCommand (ID_VISUALISATION_ZOOM, FALSE)));
		}
		{
			//-------------------------
			// Setup Referentiel button
			//-------------------------
			CMenu menu;
			menu.LoadMenu (IDR_REFERENTIEL);

			CMenu* pPopupMenu = menu.GetSubMenu (0);
			VERIFY (pPopupMenu != NULL);

			m_wndVisuBar.ReplaceButton (ID_VISUALISATION_REFERENTIAL, CBCGPToolbarMenuButton 
				(ID_VISUALISATION_REFERENTIAL, pPopupMenu->GetSafeHmenu (), 
				CImageHash::GetImageOfCommand (ID_VISUALISATION_REFERENTIAL, FALSE)));
		}
		break;
	case IDR_OBJECTS_TB:
		{
		}
		break;
	case IDR_CONSTRUCTION_TB:
		{
			//-----------------------------------
			// Replace dropdown resources button:
			//-----------------------------------
			m_wndConstrBar.ReplaceButton (ID_DUMMY_POINTON,
					CBCGPDropDownToolbarButton (_T("Add Resource"), &m_wndPointOnBar));
			m_wndConstrBar.ReplaceButton (ID_DUMMY_INTERS,
					CBCGPDropDownToolbarButton (_T("Add Resource"), &m_wndIntersectBar));
			m_wndConstrBar.ReplaceButton (ID_DUMMY_POINTDIV,
					CBCGPDropDownToolbarButton (_T("Add Resource"), &m_wndDivPointBar));
		}
	case IDR_UNIVERSEH_TB:
		{
			int r=0;
		}
		break;
	case IDR_EXPLORATION_TB:
		{
			int r=0;
		}
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnHelpCustomizeToolbars(WPARAM wp, LPARAM lp)
{
	int iPageNum = (int) wp;

	CBCGPToolbarCustomize* pDlg = (CBCGPToolbarCustomize*) lp;
	ASSERT_VALID (pDlg);

	// TODO: show help about page number iPageNum
	CPropertyPage* pPage = pDlg->GetPage(iPageNum);	
	if (pPage)
	{
		UINT nb = pPage->SendMessage(WM_HELPHITTEST,NULL,NULL);
		AfxGetApp()->WinHelp(nb);
	}

	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CBCGPPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
    CBCGPMDIFrameWnd::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

    if (pMenuPopup != NULL && pMenuPopup->GetParentButton() &&
		(pMenuPopup->GetParentButton()->m_nID == ID_EDIT_UNDO || 
		 pMenuPopup->GetParentButton()->m_nID == ID_EDIT_REDO))
		//pMenuPopup->GetMenuBar ()->CommandToIndex (ID_EDIT_UNDO_DUMMY) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		UINT dddd = pMenuPopup->GetParentButton()->m_nID;

		CMDIChildWnd*pChild = MDIGetActive();
		if (pChild) 
		{
		
			CCalques3DDoc *pDoc = DYNAMIC_DOWNCAST(CCalques3DDoc,pChild->GetActiveDocument());
			if (pDoc /*&& pDoc->m_nDocUndoState*/)
			{
				//CUndoButton::m_lstActions.RemoveAll();
				//CRedoButton::m_lstActions.RemoveAll();

				int nbUndo = 0;
				POSITION pos = NULL;
				
				if (dddd==ID_EDIT_UNDO)
				{
					nbUndo = pDoc->m_cNewUndoSet.GetCount();
					pos = pDoc->m_cNewUndoSet.GetHeadPosition ();
				}
				else
				{
					nbUndo = pDoc->m_cNewRedoSet.GetCount();
					pos = pDoc->m_cNewRedoSet.GetHeadPosition ();
				}
				BOOL bMore = (BOOL)(nbUndo>7);
				nbUndo = min(7,nbUndo);

				for (int i=0; pos != NULL && i <nbUndo;i++)
				{
					CUndoObject *pObj=NULL;
					if (dddd==ID_EDIT_UNDO)
						pObj = pDoc->m_cNewUndoSet.GetNext(pos);
					else
						pObj = pDoc->m_cNewRedoSet.GetNext(pos);

					if (!pObj) continue;
					
					CString mstr,strID;
					strID.Format(_T("%d - "),i+1);
					mstr = strID + pObj->GetUndoText();
					
					/*if (dddd==ID_EDIT_UNDO)
						CUndoButton::m_lstActions.AddTail (mstr);
					else
						CRedoButton::m_lstActions.AddTail (mstr);*/

					pMenuPopup->InsertItem (CBCGPToolbarMenuButton (
						ID_EDIT_UNDO_DUMMY, NULL, -1, mstr));
				}
				if (bMore)
				{
					CString mstr;
					mstr.LoadString(ID_EDIT_UNDO_MORE);
					pMenuPopup->InsertSeparator();
					pMenuPopup->InsertItem (CBCGPToolbarMenuButton (
						ID_EDIT_UNDO_DUMMY, NULL, -1,mstr));
				}


				//UINT nCmdID = pDoc->m_nUndoState + IDS_UNDO_NONE;
				//CString mstr;
				//mstr.LoadString(nCmdID);
				//pMenuPopup->InsertItem (CBCGToolbarMenuButton (
				//	ID_EDIT_UNDO_DUMMY, NULL, -1, mstr));
			}
		}
    }
    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VISUALISATION_POV_POV1) >= 0)
	{
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		int nb = pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VISUALISATION_POV_POV1);
		//pMenuPopup->RemoveItem (nb);

	}

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_MACROS_USERS) >= 0)
    {
		if (CBCGPToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		int rrr = pMenuPopup->GetMenuBar ()->CommandToIndex (ID_MACROS_USERS);
		pMenuPopup->RemoveItem(rrr);
		//pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_MACRO));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		// Replace toolbar dummy items to the user-defined toolbar names:
		//---------------------------------------------------------------
		int nbItem = (int) pPopup->GetMenuItemCount ();
		for (int i = 0; i < nbItem;)
		{
			UINT uiCmd = pPopup->GetMenuItemID (i);

			if (uiCmd >= ID_MACROS_USER1 && 
				uiCmd <= ID_MACROS_USER15)
			{
				//-------------------------------------------------------------------
				// "User toolbar" item. First check that toolbar number 'x' is exist:
				//-------------------------------------------------------------------
				POSITION pos = TPref::TMacroList.GetHeadPosition();
				int idx = 0;
				int nb = uiCmd - ID_MACROS_USER1;
				BOOL bFalse = FALSE;
				while (pos)
				{
				    CMacroUserTool* pToolBar = (CMacroUserTool*)TPref::TMacroList.GetNext( pos );
					if (idx==nb)
					{
						CString strToolbarName;
						strToolbarName = pToolBar->m_strLabel;

						pPopup->ModifyMenu (i ++, MF_BYPOSITION | MF_STRING, uiCmd, strToolbarName);
						bFalse = TRUE;

						pMenuPopup->InsertItem (CBCGPToolbarMenuButton (
								uiCmd, NULL, CBCGPToolBar::GetDefaultImage(ID_MACROS_USER1), strToolbarName));
						break;
					}
					idx++;
				}

		/*		CBCGToolBar* pToolBar = GetUserBarByIndex (uiCmd - uiViewUserToolbarCmdFirst);
				if (pToolBar != NULL)
				{
					ASSERT_VALID (pToolBar);

					//-----------------------------------------------------------
					// Modify the current menu item text to the toolbar title and
					// move next:
					//-----------------------------------------------------------
					CString strToolbarName;
					pToolBar->GetWindowText (strToolbarName);

					menu.ModifyMenu (i ++, MF_BYPOSITION | MF_STRING, uiCmd, strToolbarName);
				}
				else*/
				if (!bFalse)
				{
					pPopup->DeleteMenu (i, MF_BYPOSITION);
				}
			}
			else	// Not "user toolbar" item, move next
			{
				i ++;
			}
		}

		//pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

	int iIndex = -1;
	if (pMenuPopup && m_wndMenuBar.GetDroppedDownMenu (&iIndex) == 
		pMenuPopup->GetParentButton () &&
		iIndex == (m_wndMenuBar.GetSystemMenu () == NULL ? 0 : 1))	// "File" menu
	{
		pMenuPopup->EnableMenuLogo (40);
	}


	return TRUE;
}

void CMainFrame::OnViewObject()
{
	ShowControlBar (&m_wndObjBar,
					!(m_wndObjBar.IsVisible()),
					FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewObject(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndObjBar.IsVisible());
}

void CMainFrame::OnViewConstruction()
{
	ShowControlBar (&m_wndConstrBar,!(m_wndConstrBar.IsVisible()),FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewConstruction(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndConstrBar.IsVisible());
}

void CMainFrame::OnViewVisualisation()
{
	ShowControlBar (&m_wndVisuBar,!(m_wndVisuBar.IsVisible()),FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewVisualisation(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndVisuBar.IsVisible());
}

void CMainFrame::OnViewExploration()
{
	ShowControlBar (&m_wndExplorBar,
					!(m_wndExplorBar.IsVisible()),
					FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewExploration(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndExplorBar.IsVisible());
}

void CMainFrame::OnViewPropertyBar()
{
	ShowControlBar (&m_wndPropertyBar,
					!(m_wndPropertyBar.IsVisible()),
					FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewPropertyBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndPropertyBar.IsVisible());
}

void CMainFrame::OnViewDependentBar()
{
	ShowControlBar (&m_wndDependentBar,
					!(m_wndDependentBar.IsVisible()),
					FALSE,TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewDependentBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndDependentBar.IsVisible());
}


void CMainFrame::OnUpdateMode(CCmdUI *pCmdUI)
{
	BOOL bEnab=FALSE;
	CMDIChildWnd*pChild = NULL;
	CDocument* pDoc = NULL;
	
	if ((pChild = MDIGetActive()) && (pDoc = pChild->GetActiveDocument()))
	{
		bEnab = pDoc->IsModified();
	}

    pCmdUI->Enable(bEnab); 
}

//////////////////////////////////////////////////////////////////////
/// Method CMainFrame::OnSetTaskMessageString
/// @param wp	
/// @param wp	
/// @return 
//////////////////////////////////////////////////////////////////////
afx_msg LRESULT CMainFrame::OnSetTaskMessageString(WPARAM wp, LPARAM lp)
{
	CString strTaskHelp(*(CString*)wp);
	UINT nTaskID = (UINT)lp;
	//m_wndStatusBar.SetWindowText(mstr);
	m_wndStatusBar.SetPaneText(1,strTaskHelp);

	HICON icon = CBCGPToolBar::GetImages()->ExtractIcon(CImageHash::GetImageOfCommand (nTaskID));
	m_wndStatusBar.SetPaneIcon(0,icon);
	if (icon) ::DestroyIcon(icon);
	return 0L;
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	m_wndStatusBar.SetPaneIcon(0,(HICON)NULL);
	return CBCGPMDIFrameWnd::OnSetMessageString(wParam, lParam);
}

LRESULT CMainFrame::OnSetObjectSelectionString(WPARAM wParam, LPARAM lParam)
{
	CString strTaskHelp(*(CString*)wParam);
	m_wndStatusBar.SetPaneIcon(0,(HICON)NULL);
	m_wndStatusBar.SetPaneText(1,strTaskHelp);
	return 0L;
}


CFrameWnd* CMainFrame::LaunchView(UINT nID)
{
	CFrameWnd* pFrame =NULL;
	switch (nID)
	{
		case ID_VIEW_UNIVERSE:
			pFrame = LaunchUniverse();
			break;
		case ID_VIEW_HISTORIQUE:
			pFrame = LaunchViews(RUNTIME_CLASS(CViewHisto),IDR_VIEWHISTORIQUE);
			break;
		case ID_VIEW_GRAPH:
			pFrame = LaunchViews(RUNTIME_CLASS(CViewGraph),IDR_VIEWGRAPH);
			break;
		case ID_VIEW_ANALYTIC:
			pFrame = LaunchViews(RUNTIME_CLASS(CViewAnalytic),IDR_VIEWANALYTIC);
			break;
		case ID_VIEW_RENDERING:
			pFrame = LaunchViews(RUNTIME_CLASS(CView3DRender),IDR_VIEWRENDERING);
			break;
		case ID_VIEW_CALQUE:
		case ID_VIEW_CALQUE2:
		case ID_VIEW_CALQUE3:
		case ID_VIEW_CALQUE4:
			pFrame = LaunchCalques(nID);
			break;
	}
	return pFrame;
}

CFrameWnd* CMainFrame::LaunchViews(const CRuntimeClass* pClass,UINT nID)
{
	// TODO: Add your command handler code here
	CMDIChildWnd*pChild = MDIGetActive();
	if (!pChild) return NULL;
	
	CDocument* pDoc = pChild->GetActiveDocument();
	//CDocument* pDoc = GetDocument();
    if (!pDoc) return NULL; // only for views with document

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf(pClass)) 
		{
			pView->GetParentFrame()->ActivateFrame();
			return pView->GetParentFrame();
		}
	}

	POSITION pos2 = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos2 != NULL)
	{
		CDocTemplate* ptempl = AfxGetApp()->GetNextDocTemplate(pos2) ;
		if (ptempl)
		{
			CString rString;
			ptempl->GetDocString(rString,CDocTemplate::windowTitle);
			CString mstr;
			mstr.LoadString(nID);
			int nb = mstr.Find('\n');
			CString tt = mstr.Left(nb);

			if (tt == rString)
			{
				CMDIChildWnd* pNewFrame =
					(CMDIChildWnd*)(ptempl->CreateNewFrame(pDoc, NULL));
				if (pNewFrame == NULL) return NULL;  // not created
				ptempl->InitialUpdateFrame(pNewFrame, pDoc);
				return pNewFrame;
			}
		}
	}
	return NULL;
}

CFrameWnd*  CMainFrame::LaunchUniverse()
{
	// TODO: Add your command handler code here
	CMDIChildWnd*pChild = MDIGetActive();
	if (!pChild) return NULL;
	

	CDocument* pDoc = pChild->GetActiveDocument();
	//CDocument* pDoc = GetDocument();
    if (!pDoc) return NULL; // only for views with document

	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf( RUNTIME_CLASS(CViewUniv)) &&
			!pView->IsKindOf( RUNTIME_CLASS(CViewCalque))) 
		{
			pView->GetParentFrame()->ActivateFrame();
			return pView->GetParentFrame();
		}
	}

	POSITION pos2 = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos2 != NULL)
	{
		CDocTemplate* ptempl = AfxGetApp()->GetNextDocTemplate(pos2) ;
		if (ptempl)
		{
			CString rString;
			ptempl->GetDocString(rString,CDocTemplate::windowTitle);
			CString mstr;
			mstr.LoadString(IDR_VIEWUNIVERSE);
			int nb = mstr.Find('\n');
			CString tt = mstr.Left(nb);

			if (tt == rString)
			{
				CMDIChildWnd* pNewFrame =
					(CMDIChildWnd*)(ptempl->CreateNewFrame(pDoc, NULL));
				if (pNewFrame == NULL) return NULL;  // not created
				ptempl->InitialUpdateFrame(pNewFrame, pDoc);
				return pNewFrame;
			}
		}
	}
	return NULL;
}

CFrameWnd*	CMainFrame::LaunchCalques(UINT id)
{
	CMDIChildWnd*pChild = MDIGetActive();
	if (!pChild) return NULL;
	
	CDocument* pDoc = pChild->GetActiveDocument();
    if (!pDoc) return NULL; 

	// Get the tracing number from the view command
	int nCalque = id - ID_VIEW_CALQUE + 1;

	// Look if the view is already open. If so, activate it.
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView->IsKindOf( RUNTIME_CLASS(CViewCalque))) 
		{
			CViewCalque *pClq = DYNAMIC_DOWNCAST(CViewCalque,pView);
			if (pClq && pClq->GetVisualParam()->nCalqueNum == nCalque)
			{
				pView->GetParentFrame()->ActivateFrame();
				return pView->GetParentFrame();
			}
		}
	}

	// Find the document template corresponding to the Tracing view
	POSITION pos2 = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos2 != NULL)
	{
		CDocTemplate* ptempl = AfxGetApp()->GetNextDocTemplate(pos2) ;
		if (!ptempl) continue;

		CString rString;
		ptempl->GetDocString(rString,CDocTemplate::windowTitle);
		CString mstr;
		mstr.LoadString(IDR_VIEWCALQUE);
		int nb = mstr.Find('\n');
		CString tt = mstr.Left(nb);
		if (tt != rString) continue;

		CMDIChildWnd* pNewFrame =
				(CMDIChildWnd*)(ptempl->CreateNewFrame(pDoc, NULL));
		if (pNewFrame == NULL) return NULL;  // not created

		// Initialise the Tracing attribute for the local visualisation parameter
		CView *pView = pNewFrame->GetActiveView();
		CViewCalque *pClq = DYNAMIC_DOWNCAST(CViewCalque,pView);
		if (pClq)
			pClq->GetVisualParam()->nCalqueNum = nCalque;
		ptempl->InitialUpdateFrame(pNewFrame, pDoc);
		pView = pNewFrame->GetActiveView();
		pClq = DYNAMIC_DOWNCAST(CViewCalque,pView);
		if (pClq)
			pClq->GetVisualParam()->nCalqueNum = nCalque;

		// Restore the previous window position
		CCalques3DDoc *pMyDoc= DYNAMIC_DOWNCAST(CCalques3DDoc,pDoc);
		CCalques3DDocTemplate *pMyTP= DYNAMIC_DOWNCAST(CCalques3DDocTemplate,ptempl);
		if (pMyDoc && pMyTP)
		{
			UINT nID = pMyTP->GetViewID();
			CWinPlacement mypl;
			BOOL bRet = pMyDoc->m_cWinPos.Lookup(nID,mypl);
			if (bRet)
			{
				mypl.m_pl.length = sizeof(WINDOWPLACEMENT);
				pNewFrame->SetWindowPlacement(&(mypl.m_pl));
			}

		}
		return pNewFrame;
	}
	return NULL;
}

void CMainFrame::OnUpdateViews(CCmdUI* pCmdUI) 
{
	CMDIChildWnd*pChild = MDIGetActive();
	if (!pChild) return;

	CDocument* pDoc = pChild->GetActiveDocument();
    if (!pDoc) return; // only for views with document

	//if (pCmdUI->m_nID==ID_VIEW_RENDERING)
	//	pCmdUI->Enable(FALSE);



	CView* pView=NULL;
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		BOOL bCheck = FALSE;
		pView = pDoc->GetNextView(pos);
		switch (pCmdUI->m_nID){
		case ID_VIEW_UNIVERSE:
			bCheck = (pView->IsKindOf(RUNTIME_CLASS(CViewUniv)) && 
						!pView->IsKindOf(RUNTIME_CLASS(CViewCalque)));
			break;
		case ID_VIEW_HISTORIQUE:
			bCheck = pView->IsKindOf(RUNTIME_CLASS(CViewHisto));
			break;
		case ID_VIEW_GRAPH:
			bCheck = pView->IsKindOf(RUNTIME_CLASS(CViewGraph));
			break;
		case ID_VIEW_ANALYTIC:
			bCheck = pView->IsKindOf(RUNTIME_CLASS(CViewAnalytic));
			break;
		case ID_VIEW_RENDERING:
			bCheck = pView->IsKindOf(RUNTIME_CLASS(CView3DRender));
			break;
		case ID_VIEW_CALQUE:
		case ID_VIEW_CALQUE2:
		case ID_VIEW_CALQUE3:
		case ID_VIEW_CALQUE4:
			{
				bCheck = pView->IsKindOf(RUNTIME_CLASS(CViewCalque));
				if (bCheck)
				{
					int nID = pCmdUI->m_nID - ID_VIEW_CALQUE +1;
					CViewCalque *pClq = DYNAMIC_DOWNCAST(CViewCalque,pView);
					bCheck = (pClq && pClq->GetVisualParam()->nCalqueNum == nID);
				}
			}
			break;
		}
		if (bCheck)
		{
			pCmdUI->SetCheck (TRUE);
			return;
		}
	}
}

void CMainFrame::OnViewFullScreen()
{
	CMDIChildWnd* pChild = MDIGetActive();

	BOOL isFS = IsFullScreen();

	if (!isFS) SetRedraw(FALSE);
	if (pChild) pChild->ShowWindow(SW_MAXIMIZE);

	
	const CObList& gAllToolbars = CBCGPToolBar::GetAllToolbars ();
	POSITION pos;
	if (!isFS) for (pos = gAllToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (pos);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);
			CBCGPMDIChildWnd *pWnd = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd,pToolBar->GetParent());
			if (pWnd)
				pWnd->ShowControlBar (pToolBar, FALSE,FALSE, FALSE);
		}
	}
	ShowFullScreen ();
	if (!isFS) SetRedraw(TRUE);
	if (!isFS) for (pos = gAllToolbars.GetHeadPosition (); pos != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (pos);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);
			CBCGPMDIChildWnd *pWnd = DYNAMIC_DOWNCAST(CBCGPMDIChildWnd,pToolBar->GetParent());
			if (pWnd)
				pWnd->ShowControlBar (pToolBar, TRUE,FALSE, FALSE);
		}
	}
}

void CMainFrame::OnViewUniverse() 
{
	LaunchUniverse();
}

void CMainFrame::OnViewCalque(UINT id) 
{
	LaunchCalques(id);
}

void CMainFrame::OnViewAnalytic() 
{
	LaunchViews(RUNTIME_CLASS(CViewAnalytic),IDR_VIEWANALYTIC);
}

void CMainFrame::OnViewDepend() 
{
	LaunchViews(RUNTIME_CLASS(CViewDepend),IDR_VIEWDEPEND);
}

void CMainFrame::OnViewGraph() 
{
	LaunchViews(RUNTIME_CLASS(CViewGraph),IDR_VIEWGRAPH);
}

void CMainFrame::OnViewHistorique() 
{
	LaunchViews(RUNTIME_CLASS(CViewHisto),IDR_VIEWHISTORIQUE);
}

void CMainFrame::OnViewRendering() 
{
	LaunchViews(RUNTIME_CLASS(CView3DRender),IDR_VIEWRENDERING);
}

void CMainFrame::OnDrawMenuLogo (CDC* pDC, CBCGPPopupMenu* pMenu, const CRect& rectLogo)
{
	ASSERT_VALID (pDC);

	//------------------------------------------------------
	// Fill menu side logo by gradient color:
	// (this code was copied from BCGSizingControlBar.cpp
	// Copyright (C) 1998, 1999 by Cristi Posea)
	//------------------------------------------------------

    // this will make 2^6 = 64 fountain steps
    int nShift = 6;
    int nSteps = 1 << nShift;

	COLORREF clrCptn = RGB (0, 128, 128);
	COLORREF clrCptnRight = RGB (0, 0, 128);

    for (int i = 0; i < nSteps; i++)
    {
        // do a little alpha blending
        BYTE bR = (BYTE) ((GetRValue(clrCptn) * (nSteps - i) +
                   GetRValue(clrCptnRight) * i) >> nShift);
        BYTE bG = (BYTE) ((GetGValue(clrCptn) * (nSteps - i) +
                   GetGValue(clrCptnRight) * i) >> nShift);
        BYTE bB = (BYTE) ((GetBValue(clrCptn) * (nSteps - i) +
                   GetBValue(clrCptnRight) * i) >> nShift);

		CBrush br (RGB(bR, bG, bB));

        // then paint with the resulting color
        CRect r2 = rectLogo;

		r2.bottom = rectLogo.bottom - 
               ((i * rectLogo.Height()) >> nShift);
        r2.top = rectLogo.bottom - 
            (((i + 1) * rectLogo.Height()) >> nShift);
        if (r2.Height() > 0)
           pDC->FillRect(r2, &br);
    }

	pDC->Draw3dRect (rectLogo, ::GetSysColor (COLOR_3DLIGHT), ::GetSysColor (COLOR_3DSHADOW));

    int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
    COLORREF clrOldText = pDC->SetTextColor (RGB (255, 255, 255));

    CFont* pOldFont = pDC->SelectObject (&m_fontVertCaption);
    CString sTitle;
	sTitle.LoadString(AFX_IDS_APP_TITLE);

    CPoint ptOrg = CPoint(rectLogo.left + 5, rectLogo.bottom - 10);

    pDC->ExtTextOut (ptOrg.x, ptOrg.y,
        ETO_CLIPPED, rectLogo, sTitle, NULL);

    pDC->SelectObject(pOldFont);
    pDC->SetBkMode(nOldBkMode);
    pDC->SetTextColor(clrOldText);

	//-----------
	// Draw logo:
	//-----------
	pDC->DrawState (CPoint (rectLogo.left + 4, rectLogo.top + 10),
					CSize (32,32), &m_bmpVertCaption, 0);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	return CBCGPMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

void CMainFrame::OnHelpKeyboardmap() 
{
	// TODO: Add your command handler code here
	CBCGPKeyMapDlg dlg (this, TRUE /* Enable Print */);
	dlg.DoModal ();
}

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

void CMainFrame::OnUpdateAppLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio (m_nAppLook == pCmdUI->m_nID);
}



void CMainFrame::OnAppLook(UINT id)
{
	CBCGPDockManager::SetDockMode (DT_SMART);

	m_nAppLook = id;

	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;
	switch (m_nAppLook)
	{
	case ID_VIEW_APPLOOK_2000:
		// enable Office 2000 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager));
		break;

	case ID_VIEW_APPLOOK_XP:
		// enable Office XP look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		// enable Windows XP look (in other OS Office XP look will be used):
		CBCGPWinXPVisualManager::m_b3DTabsXPTheme = TRUE;
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPWinXPVisualManager));
		CBCGPDockManager::SetDockMode (DT_SMART);
		((CBCGPWinXPVisualManager*)CBCGPVisualManager::GetInstance ())->SetOfficeStyleMenus ();
		break;

	case ID_VIEW_APPLOOK_2003:
		// enable Office 2003 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2003));
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS2005:
		// enable VS.NET 2005 look:
		CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManagerVS2005));
		CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D_ROUNDED;
		CBCGPDockManager::SetDockMode (DT_SMART);
		break;

	default:
		ASSERT (FALSE);
	}

	//m_wndPropertyBar.RecalcLayout ();

	CBCGPDockManager* pDockManager = GetDockManager ();
	if (pDockManager != NULL)
	{
		ASSERT_VALID (pDockManager);
		pDockManager->AdjustBarFrames ();
	}

	CWindowDC dc (NULL);
	theApp.m_bHiColorIcons = dc.GetDeviceCaps (BITSPIXEL) >= 16;

	if (globalData.bIsWindows9x && dc.GetDeviceCaps (BITSPIXEL) == 16)
	{
		theApp.m_bHiColorIcons = FALSE;
	}

	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	theApp.WriteInt (_T("ApplicationLook"), m_nAppLook);
}
