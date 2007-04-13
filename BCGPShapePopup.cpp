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
// BCGPShapePopup.cpp: implementation of the CBCGPShapePopup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCGPShapePopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapePopup

IMPLEMENT_DYNAMIC(CBCGPShapePopup, CBCGPPopupMenu)

BEGIN_MESSAGE_MAP(CBCGPShapePopup, CBCGPPopupMenu)
	//{{AFX_MSG_MAP(CBCGPShapePopup)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBCGPShapePopup::~CBCGPShapePopup()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPShapePopup message handlers

int CBCGPShapePopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPToolBar::IsCustomizeMode () && !m_bEnabledInCustomizeMode)
	{
		// Don't show color popup in cistomization mode
		return -1;
	}

	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD toolbarStyle = dwDefaultToolbarStyle;
	if (m_AnimationType != NO_ANIMATION && !CBCGPToolBar::IsCustomizeMode ())
	{
		toolbarStyle &= ~WS_VISIBLE;
	}

	if (!m_wndColorBar.Create (this, toolbarStyle | CBRS_TOOLTIPS | CBRS_FLYBY, 1))
	{
		TRACE(_T ("Can't create popup menu bar\n"));
		return -1;
	}

	CWnd* pWndParent = GetParent ();
	ASSERT_VALID (pWndParent);

	m_wndColorBar.SetOwner (pWndParent);
	m_wndColorBar.SetBarStyle(m_wndColorBar.GetBarStyle() | CBRS_TOOLTIPS);

	ActivatePopupMenu (BCGCBProGetTopLevelFrame (pWndParent), this);
	RecalcLayout ();
	return 0;
}
//*************************************************************************************
CBCGPControlBar* CBCGPShapePopup::CreateTearOffBar (CFrameWnd* pWndMain, UINT uiID, LPCTSTR lpszName)
{
	ASSERT_VALID (pWndMain);
	ASSERT (lpszName != NULL);
	ASSERT (uiID != 0);

	CBCGPColorMenuButton* pColorMenuButton = 
		DYNAMIC_DOWNCAST (CBCGPColorMenuButton, GetParentButton ());
	if (pColorMenuButton == NULL)
	{
		ASSERT (FALSE);
		return NULL;
	}

	CBCGPShapeBar* pColorBar = new CBCGPShapeBar (m_wndColorBar, pColorMenuButton->m_nID);

	if (!pColorBar->Create (pWndMain,
		dwDefaultToolbarStyle,
		uiID))
	{
		TRACE0 ("Failed to create a new toolbar!\n");
		delete pColorBar;
		return NULL;
	}

	pColorBar->SetWindowText (lpszName);
	pColorBar->SetBarStyle (pColorBar->GetBarStyle () |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	pColorBar->EnableDocking (CBRS_ALIGN_ANY);

	return pColorBar;
}

