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
// CustomizeCalques3D.cpp: implementation of the CCustomizeCalques3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "CustomizeCalques3D.h"
#include "FormatToolBar.h"
#include "BCGPShapeMenuButton.h"

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
	pImageList.Create(IDB_PREFBCG_ICONS,16,0,RGB (255, 0, 255));

	CBCGPToolBarImages	m_cImage;
	m_cImage.SetImageSize(CSize(16,16));
	m_cImage.SetTransparentColor(RGB(255,0,255));
	BOOL bRes = m_cImage.Load(IDB_PREFBCG_ICONS);


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


	CString strTitle;
	strTitle.LoadString (IDR_FORMAT_TB);
	AddToolBar (strTitle, IDR_FORMAT_TB);

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

		CString strName;
		strName.LoadString(IDS_MACROS);
		AddButton (strName,CBCGPToolbarButton(
				ID_MACROS_USER1+index, 
				CBCGPToolBar::GetDefaultImage(ID_MACROS_USER1), 
				pTool->m_strLabel));
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
	//-------------------------
	// Setup undo/redo buttons:
	//-------------------------
	{	
		CBCGPToolbarFontCombo* pFontBtn = CFormatToolBar::CreateTextFontButton ();
		ReplaceButton (ID_FORMAT_TXTFONT, *pFontBtn); 
		delete pFontBtn;

		CBCGPToolbarFontSizeCombo* pFontSizeBtn = CFormatToolBar::CreateTextFontSizeButton ();
		ReplaceButton (ID_FORMAT_TXTSIZE, *pFontSizeBtn); 
		delete pFontSizeBtn;

		CBCGPColorMenuButton* pTextClrBtn = CFormatToolBar::CreateTextColorButton ();
		ReplaceButton (ID_FORMAT_TXTCOLOR, *pTextClrBtn);
		delete pTextClrBtn;

		CBCGPColorMenuButton* pObjClrBtn = CFormatToolBar::CreateObjectColorButton ();
		ReplaceButton (ID_FORMAT_OBJCOLOR, *pObjClrBtn);
		delete pObjClrBtn;

		ReplaceButton (ID_FORMAT_OBJSHAPE, CBCGPShapeMenuButton(ID_FORMAT_OBJSHAPE,1,1));
	}
}
