// Calques3D.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Calques3D.h"

//#include "MVDocTemplate.hpp"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "Calques3DDoc.h"

#include "ViewHisto.h"
#include "ViewUniv.h"
#include "ViewCalque.h"
#include "ViewGraph.h"
#include "ViewAnalytic.h"
#include "ViewDepend.h"
#include "TestView.h"

#include "Splash.h"

#include "AboutInfoDlg.h"

#include "Prefs\Prefs.h"

#include "BCGXMLSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Uncomment to use the BCG file dialog (with "recent" and "new document" tabs)
//#define OPENDLG_BCG

// Registry keys used for recovering customized aspects of Calques 3D
#define REG_SETTINGS	_T("Settings")
#define REG_SESSION		_T("Session")
#define REG_BUILD		_T("BuildDate")
#define REG_PROFILE		_T("Profile")
#define REG_PROFILEDES	_T("Description")

// Specify the base item for the registry
CString CCalques3DApp::g_strRegistryBase = REG_SETTINGS;

//////////////////////////////////////////////////////////////////////////////
/// The one and only CCalques3DApp object
CCalques3DApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCalques3DApp

IMPLEMENT_DYNAMIC(CCalques3DDocTemplate, CMultiDocTemplate)

BEGIN_MESSAGE_MAP(CCalques3DApp, CWinApp)
	//{{AFX_MSG_MAP(CCalques3DApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// Default construction
/// Initialise BCG's Resource Smart Update.
/////////////////////////////////////////////////////////////////////////////
CCalques3DApp::CCalques3DApp() : CBCGPWorkspace(TRUE)
{
}

/////////////////////////////////////////////////////////////////////////////
/// CCalques3DApp initialization.
/// 
/////////////////////////////////////////////////////////////////////////////
BOOL CCalques3DApp::InitInstance()
{
	//------------------------------------------------------------
	// Parse command line for standard shell commands, DDE, file open
	//------------------------------------------------------------
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//------------------------------------------------------------
	//Launch the splash window
	//------------------------------------------------------------
	CSplashWnd splash;
	BOOL bSplash = cmdInfo.m_bShowSplash;
	if (bSplash)
	{
		// only show splash if not embedded
		splash.Create(NULL);
		splash.ShowWindow(SW_SHOW);
		splash.UpdateWindow();
	}

	//------------------------------------------------------------
	// Localization: comment/uncomment for the relevant version
	//------------------------------------------------------------
    //m_hinstBCGCBRes = LoadLibrary (_T("BCGCBResESP.dll"));    // *** - Spanish
    //m_hinstBCGCBRes = LoadLibrary (_T("BCGCBResFRA.dll"));    // *** - French
    //m_hinstBCGCBRes = LoadLibrary (_T("BCGCBResPTG.dll"));    // *** - Portuguese
	//BCGCBSetResourceHandle (m_hinstBCGCBRes);

	//------------------------------------------------------------
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.
	//------------------------------------------------------------

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxInitRichEdit( );
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	//------------------------------------------------------------
	// Load the registry information
	// \todo SetRegistryKey() - Modify this string to be something more appropriate
	//------------------------------------------------------------
	SetRegistryKey(_T("CREDIT"));

	// Load standard INI file options (including MRU)
	LoadStdProfileSettings(6);  

	SetRegistryBase();
	//CString mstr1 = GetRegSectionPath();

	// For compatibility reasons, check the build date from the registry
	CString strRegDate = GetString(REG_BUILD, _T(""));
	CString strBuildDate = __DATE__;
	if (!strRegDate.IsEmpty() && strRegDate != strBuildDate)
	{
		MessageBox(NULL,
				_T("Due to major modifications in Calques 3D, the configuration saved\n"
				   "during the last session cannot be retrieved by this new version.\n"
				   "Calques 3D will be initialised with its default configuration.\n"
				   "This problem will not occur anymore after this launch."),
				_T("Warning !"),MB_ICONWARNING|MB_OK);
		WriteString(REG_BUILD,strBuildDate);
		CleanState(CCalques3DApp::g_strRegistryBase);
	}

	// If appropriate, load the information form the specify user profile
	SetRegistryBase (REG_SESSION);
	TPref::strProfile = GetSectionString(REG_PROFILE,REG_PROFILE, TPref::strProfile);
	SetRegistryBase (CCalques3DApp::g_strRegistryBase);
	if (!TPref::strProfile.IsEmpty())
	{
		// All registry read/write operations will be produced via CBCGXMLSettings gateway:
		CBCGPRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGXMLSettings));
		// Read settings:
		CBCGXMLSettings::ReadXMLFromFile (FALSE, TPref::strProfile);
	}
	else
		//CBCGRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGRegistry));
		CBCGPRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGPRegistry));

	//------------------------------------------------------------
	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	//------------------------------------------------------------

	//InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager();
	//InitSkinManager ();
	//GetSkinManager ()->EnableSkinsDownload (_T("http://www.bcgsoft.com/Skins"));
	//EnableTearOffMenus (NULL, ID_FREE_TEAROFF1, ID_FREE_TEAROFF2);

	// Enable user-defined tools. If you want allow more than 10 tools,
	// add tools entry to resources (ID_USER_TOOL11, ID_USER_TOOL12,...)
	EnableUserTools (ID_TOOLS_ENTRY, ID_USER_TOOL1, ID_USER_TOOL10);

	//------------------------------------------------------------
	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.
	//------------------------------------------------------------

	// UNIVERSE Template
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWUNIVERSE,
		RUNTIME_CLASS(CCalques3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewUniv));
	AddDocTemplate(pDocTemplate);

	// MACRO Template
	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWMACRO,
		RUNTIME_CLASS(CCalques3DMacroDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewUniv));
	AddDocTemplate(pDocTemplate);

	// HISTORY Template
	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWHISTORIQUE,
		RUNTIME_CLASS(CCalques3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewHisto));
	AddDocTemplate(pDocTemplate);

	// TRACING Template
	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWCALQUE,
		RUNTIME_CLASS(CCalques3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewCalque));
	AddDocTemplate(pDocTemplate);

	// GRAPH Template
	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWGRAPH,
		RUNTIME_CLASS(CCalques3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewGraph));
	AddDocTemplate(pDocTemplate);

	// MATHPAD Template
	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWANALYTIC,
		RUNTIME_CLASS(CCalques3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewAnalytic));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CCalques3DDocTemplate(
		IDR_VIEWDEPEND,
		RUNTIME_CLASS(CCalques3DDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewDepend));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//LoadCustomState ();

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	// close the splash screen
	if (cmdInfo.m_bShowSplash)
		splash.DestroyWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// CCalques3DApp exit cleanup
/////////////////////////////////////////////////////////////////////////////
int CCalques3DApp::ExitInstance() 
{
	BCGCBProCleanUp();
	//FreeLibrary (m_hinstBCGCBRes);
	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CCalques3DApp message handlers

/////////////////////////////////////////////////////////////////////////////
/// Create a new empty document
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	//if (m_pDocManager != NULL)
	//	m_pDocManager->OnFileNew();
	if (!m_pDocManager) return;

	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
	pTemplate->OpenDocumentFile(NULL);
}

/////////////////////////////////////////////////////////////////////////////
/// Open an existing document
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	//CWinApp::OnFileOpen();

	BOOL bOpenFileDialog = TRUE;
	DWORD lFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	UINT nIDSTitle = AFX_IDS_OPENFILE;
	CString fileName;
	CDocTemplate* pTemplate = NULL;


	CString title;
	VERIFY(title.LoadString(nIDSTitle));

#ifdef OPENDLG_BCG
	CBCGPFileDialog dlgFile (title, bOpenFileDialog);

	////*************************
	CBitmap bmpLogo;
	bmpLogo.LoadBitmap (IDB_LOGO);
	//dlgFile.SetLogoBitmap (&bmpLogo);

	CImageList newItemImages;
	newItemImages.Create (32, 32, ILC_MASK, 0, 1);
	newItemImages.Add (AfxGetApp ()->LoadIcon (IDR_VIEWUNIVERSE));
	newItemImages.Add (AfxGetApp ()->LoadIcon (IDR_VIEWMACRO));

	dlgFile.SetNewImagesList (&newItemImages);

	CString strItem;
	strItem.LoadString (IDR_VIEWUNIVERSE);
	dlgFile.AddNewItem (strItem.SpanExcluding (_T("\n")), 0);

	strItem.LoadString (IDR_VIEWMACRO);
	dlgFile.AddNewItem (strItem.SpanExcluding (_T("\n")), 1);

	HICON hiconOpenDlg = AfxGetApp ()->LoadIcon (IDR_MAINFRAME);
	dlgFile.SetDlgIcon (hiconOpenDlg);
	////************************* 
#else 
	CFileDialog dlgFile(bOpenFileDialog);
#endif

	dlgFile.m_ofn.Flags |= lFlags;
	CString strFilter;
	CString strDefault;

	POSITION pos = GetFirstDocTemplatePosition();
	BOOL bFirst = TRUE;
	while (pos != NULL)
	{
		CDocTemplate *pTemplate = GetNextDocTemplate(pos);
		CString strFilterExt, strFilterName;
		CString* pstrDefaultExt = bFirst ? &strDefault : NULL;

		if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
				!strFilterExt.IsEmpty() &&
		    pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
				!strFilterName.IsEmpty())
		{
			// a file based document template - add to filter list
			ASSERT(strFilterExt[0] == '.');
			if (pstrDefaultExt != NULL)
			{
				// set the default extension
				*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
				dlgFile.m_ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
				dlgFile.m_ofn.nFilterIndex = dlgFile.m_ofn.nMaxCustFilter + 1;  // 1 based number
			}

			// add to filter
			strFilter += strFilterName;
			ASSERT(!strFilter.IsEmpty());  // must have a file type name
			strFilter += (TCHAR)'\0';  // next string please
			strFilter += (TCHAR)'*';
			strFilter += strFilterExt;
			strFilter += (TCHAR)'\0';  // next string please
			dlgFile.m_ofn.nMaxCustFilter++;
		}
		bFirst = FALSE;
	}

		// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();

#ifdef OPENDLG_BCG
	////*************************
	::DestroyIcon (hiconOpenDlg);
	////************************* 
#endif

	fileName.ReleaseBuffer();
	if (nResult == IDOK)
	{
#ifdef OPENDLG_BCG
		 ////*************************
		int nNewItem = -1;
		switch (dlgFile.GetPage()){
		case CBCGPFileDialog::BCGFileNew :
			nNewItem = dlgFile.GetNewItemIndex();
			break;
		case CBCGPFileDialog::BCGFileOpen :
			break;
		case CBCGPFileDialog::BCGFileRecent :
			fileName = dlgFile.GetRecentFilePath();
			break;
		default:
			return;
		}
		if (nNewItem !=-1)
		{
			POSITION pos = GetFirstDocTemplatePosition();
			CDocTemplate *pTemplate2 = NULL;
			int nb =-1;
			while (pos != NULL && nb != nNewItem)
			{
				pTemplate2 = GetNextDocTemplate(pos);
				nb++;
			}
			if (pTemplate2)
				pTemplate2->OpenDocumentFile(NULL);

		}
		else
			OpenDocumentFile(fileName);
		////************************* 
#else
		OpenDocumentFile(fileName);
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////
/// Called when the main frame of the application is closed.
/// Save the current state of the application into the registry
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::OnClosingMainFrame (CBCGPFrameImpl* pFrame)
{
	// Defaults to automatically saving state.
	SaveState(0, pFrame);
}


/////////////////////////////////////////////////////////////////////////////
/// Load Calques 3D specific preferences
/// See TPref for the definition of these preferences
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::LoadCustomState ()
{
	//SetRegistryBase (REG_SESSION);
	//TPref::strProfile = GetSectionString(REG_PROFILE,REG_PROFILE, TPref::strProfile);
	//TPref::strProfileDesc = GetSectionString(REG_PROFILE,REG_PROFILEDES, TPref::strProfileDesc);

	//SetRegistryBase (CCalques3DApp::g_strRegistryBase);
	TPref::DefRep = GetSectionInt(_T("Universe"),_T("DefRep"), TPref::DefRep);
	TPref::Magnet = GetSectionInt(_T("Universe"),_T("Magnet"), TPref::Magnet);
	TPref::Synchron = GetSectionInt(_T("Universe"),_T("Synchron"), TPref::Synchron);
	TPref::GrayedHidden = GetSectionInt(_T("Universe"),_T("GrayHidden"), TPref::GrayedHidden);
	TPref::bMacroLoading = GetSectionInt(_T("Macro"),_T("UserLoading"), TPref::bMacroLoading);
	BOOL brest = GetSectionObject(_T("Macro"),_T("UserMacros"),TPref::TMacroList);
	//CString mstr = TPref::strProfile ;
	//TPref::strProfile = mstr;

	// Call the default initialisation of the preferences
	TPref::DefaultInit();
}

/////////////////////////////////////////////////////////////////////////////
/// Save Calques 3D specific preferences
/// See TPref for the definition of these preferences
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::SaveCustomState ()
{
	//SetRegistryBase (REG_SESSION);
	//WriteSectionString(REG_PROFILE,REG_PROFILE, TPref::strProfile);
	//WriteSectionString(REG_PROFILE,REG_PROFILEDES, TPref::strProfileDesc);

	SetRegistryBase (CCalques3DApp::g_strRegistryBase);
	WriteSectionInt(_T("Universe"),_T("DefRep"), TPref::DefRep);
	WriteSectionInt(_T("Universe"),_T("Magnet"), TPref::Magnet);
	WriteSectionInt(_T("Universe"),_T("Synchron"), TPref::Synchron);
	WriteSectionInt(_T("Universe"),_T("GrayHidden"), TPref::GrayedHidden);
	//WriteInt (_T("GradientCaption"), CBCGSizingControlBar::IsCaptionGradient ());
	//WriteInt (_T("TabFlatBorders"), m_bTabFlatBorders);

	WriteSectionInt(_T("Macro"),_T("UserLoading"), TPref::bMacroLoading);
	WriteSectionObject(_T("Macro"),_T("UserMacros"),TPref::TMacroList);
	POSITION pos = TPref::TMacroList.GetHeadPosition();
	while( pos != NULL )
	{
		delete TPref::TMacroList.GetNext( pos );
	}
	TPref::TMacroList.RemoveAll();

	//CBCGRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGRegistry));
	SetRegistryBase (REG_SESSION);
	WriteSectionString(REG_PROFILE,REG_PROFILE, TPref::strProfile);
	WriteSectionString(REG_PROFILE,REG_PROFILEDES, TPref::strProfileDesc);
	SetRegistryBase (CCalques3DApp::g_strRegistryBase);
}


/////////////////////////////////////////////////////////////////////////////
/// Called when Calques 3D current state is loaded from the registry.
/// Initialise the various managers with the relevant information.
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::PreLoadState ()
{
	CMenu menu;
	CString mstr;

	// Initialise the mouse manager for the given views
	//GetMouseManager()->AddView (IDR_CALQUETYPE, _T("My view"), IDR_CALQUETYPE);
	//GetMouseManager()->AddView (IDR_VIEWUNIVERSE, _T("Universe"), IDR_VIEWUNIVERSE);

	// Initialise the various contextual menus
	menu.LoadMenu(IDR_POPUP_EXTRACT);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_EXTRACT);
	
	menu.LoadMenu(IDR_POPUP_MOVE);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_MOVE);
	
	menu.LoadMenu(IDR_POPUP_DEFAULT);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_DEFAULT);
	
	menu.LoadMenu(IDR_POPUP_PROJECTION);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_PROJECTION);
	
	menu.LoadMenu(IDR_POPUP_SHAPE);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_SHAPE);

	menu.LoadMenu(IDR_POPUP_VERIFY);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_VERIFY);

	menu.LoadMenu(IDR_POPUP_MEASURE);
	menu.GetMenuString(0,mstr,MF_BYPOSITION);
	menu.Detach();
	GetContextMenuManager()->AddMenu (mstr, IDR_POPUP_MEASURE);
}


BOOL CCalques3DApp::PreTranslateMessage(MSG* pMsg)
{
	return CWinApp::PreTranslateMessage(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
/// Calles to initialised and launch the "About" dialogue.
/// See CAboutInfoPage, CAboutCreditPage and CAboutCalquePage
/////////////////////////////////////////////////////////////////////////////
void CCalques3DApp::OnAppAbout()
{
	//CAboutDlg aboutDlg;
	//aboutDlg.DoModal();

	CAboutDlg aboutDlg(IDR_ABOUTDLG);

	CAboutInfoPage		m_pInfoDlg;
	CAboutCreditPage	m_pCreditDlg;
	CAboutCalquePage	m_pCalqueDlg;

	aboutDlg.AddPage(&m_pCalqueDlg);
	aboutDlg.AddPage(&m_pInfoDlg);
	aboutDlg.AddPage(&m_pCreditDlg);
	aboutDlg.DoModal();
}
