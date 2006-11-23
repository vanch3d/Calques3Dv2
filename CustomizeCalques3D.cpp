// CustomizeCalques3D.cpp: implementation of the CCustomizeCalques3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "CustomizeCalques3D.h"

#include "prefs\Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomizeCalques3D

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// Default constructor
///
/// \param pWndParentFrame		Pointer to the parent frame window.
/// \param bAutoSetFromMenus	TRUE if the menu and keyboard pages are automatically filled from the application menus. 
/// \param uiFlags				Customization flags.
/// \param plistCustomPages		List of runtime classes of the custom property pages to be added to the customization dialog. 
///
//////////////////////////////////////////////////////////////////////
CCustomizeCalques3D::CCustomizeCalques3D(CFrameWnd* pWndParentFrame, 
						BOOL bAutoSetFromMenus,
						UINT uiFlags,
						CustomPageList* plistCustomPages):
	CBCGPToolbarCustomize(pWndParentFrame,bAutoSetFromMenus,uiFlags,NULL)
{
	CImageList pImageList;
	pImageList.Create(IDB_PREFS_ICONS,16,0,RGB (255, 0, 255));

	CBCGPToolBarImages	m_cImage;
	m_cImage.SetImageSize(CSize(16,16));
	m_cImage.SetTransparentColor(RGB(255,0,255));
	BOOL bRes = m_cImage.Load(IDB_PREFS_ICONS);


	//EnableStackedTabs(FALSE);
	for (int i=0;i<GetPageCount();i++)
	{
		CPropertyPage* pPage = GetPage(i);
		if (pPage) 
		{
			pPage->m_psp.dwFlags |= PSP_USEHICON ;
			pPage->m_psp.hIcon = pImageList.ExtractIcon(i);
		}
	}
}


//////////////////////////////////////////////////////////////////////
/// Perform the  initialisation of the customize dialog with Calques 3D specifics.
//////////////////////////////////////////////////////////////////////
void CCustomizeCalques3D::InitCalques3DPrefs()
{
	EnableUserDefinedToolbars ();

	//-----------------------------------
	// Add dropdown resources button:
	//-----------------------------------
	//AddButton (_T("Construction"), CBCGDropDownToolbarButton (_T("1 Point on ..."), &m_wndPointOnBar));
	//AddButton (_T("Construction"), CBCGDropDownToolbarButton (_T("1 Intersection ..."), &m_wndIntersectBar));
	//AddButton (_T("Construction"), CBCGDropDownToolbarButton (_T("1 Points ..."), &m_wndDivPointBar));

	RemoveButton(-1,ID_MACROS_USERS);

	//const CObList& lstTools = g_pUserToolsManager->GetUserTools ();
	int index=0;
	for (POSITION pos = TPref::TMacroList.GetHeadPosition (); pos != NULL;)
	{
		CMacroUserTool *pTool = (CMacroUserTool*) TPref::TMacroList.GetNext (pos);
		ASSERT_VALID (pTool);

		AddButton (_T("Macros"),
			CBCGPToolbarButton (ID_MACROS_USER1+index, CBCGPToolBar::GetDefaultImage(ID_MACROS_USER1), pTool->m_strLabel));
		index++;
	}
	//	AddButton (_T("zzzzzzzzzzz"),
	//		CBCGToolbarButton (ID_MACROS_USERS, CBCGToolBar::GetDefaultImage(ID_MACROS_USERS),_T("DDD")));


	//-------------------------
	// Setup Open button
	//-------------------------
	{	CMenu menu;
		menu.LoadMenu (IDR_OPEN);

		CMenu* pPopupMenu = menu.GetSubMenu (0);
		VERIFY (pPopupMenu != NULL);

		ReplaceButton (ID_FILE_OPEN, CBCGPToolbarMenuButton 
			(ID_FILE_OPEN, pPopupMenu->GetSafeHmenu (), 
			CImageHash::GetImageOfCommand (ID_FILE_OPEN, FALSE)));
	}
	//-------------------------
	// Setup undo/redo buttons:
	//-------------------------
	{	CMenu menu;
		menu.LoadMenu (IDR_UNDO);

		CMenu* pPopupMenu = menu.GetSubMenu (0);
		VERIFY (pPopupMenu != NULL);

		ReplaceButton (ID_EDIT_UNDO, CBCGPToolbarMenuButton 
			(ID_EDIT_UNDO, pPopupMenu->GetSafeHmenu (), 
			CImageHash::GetImageOfCommand (ID_EDIT_UNDO, FALSE)));

		ReplaceButton (ID_EDIT_REDO, CBCGPToolbarMenuButton 
			(ID_EDIT_REDO, pPopupMenu->GetSafeHmenu (), 
			CImageHash::GetImageOfCommand (ID_EDIT_REDO, FALSE)));
	}
}
