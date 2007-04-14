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
/// @file CustomizeCalques3D.h
/// @brief interface of the CCustomizeCalques3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CUSTOMIZECALQUES3D_H__D9625EE6_D8E8_4071_85CC_2BF330C3FECB__INCLUDED_)
#define AFX_CUSTOMIZECALQUES3D_H__D9625EE6_D8E8_4071_85CC_2BF330C3FECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// List of Custom pages to add in the Preference dialog box
/////////////////////////////////////////////////////////////////////////////
typedef CList <CRuntimeClass*, CRuntimeClass*> CustomPageList;

#define BCGCUSTOMIZE_DEFAULT	(BCGCUSTOMIZE_MENU_SHADOWS | BCGCUSTOMIZE_TEXT_LABELS | \
								 BCGCUSTOMIZE_LOOK_2000 | BCGCUSTOMIZE_MENU_ANIMATIONS)

/////////////////////////////////////////////////////////////////////////////
/// Main dialog for the customization of Calques3D GUI.
///
/// For the preferences of Calques 3D, see CPreferenceDialog.
/////////////////////////////////////////////////////////////////////////////
class CCustomizeCalques3D : public CBCGPToolbarCustomize  
{
// Construction
public:
	CCustomizeCalques3D(CFrameWnd* pWndParentFrame, 
						BOOL bAutoSetFromMenus = FALSE,
						UINT uiFlags = BCGCUSTOMIZE_DEFAULT,
						CustomPageList* plistCustomPages = NULL);

	void InitCalques3DPrefs();
};

#endif // !defined(AFX_CUSTOMIZECALQUES3D_H__D9625EE6_D8E8_4071_85CC_2BF330C3FECB__INCLUDED_)
