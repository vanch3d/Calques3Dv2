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
// PrefUniverse.cpp: implementation of the CPrefUniverse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "..\calques3d.h"
#include "PrefUniverse.h"

#include "Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int iListId = 1;

BEGIN_MESSAGE_MAP(CPOVList,CBCGPEditListBox)
	//ON_NOTIFY(LVN_KEYDOWN, iListId, OnKeyDown)
	ON_NOTIFY(NM_DBLCLK, iListId, OnDblclkList)
	//ON_NOTIFY(LVN_GETDISPINFO, iListId, OnGetdispinfo)
	//ON_NOTIFY(LVN_ENDLABELEDIT, iListId, OnEndLabelEdit)
	//ON_NOTIFY(LVN_ITEMCHANGED, iListId, OnItemChanged)
END_MESSAGE_MAP()

void CPOVList::OnDblclkList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	*pResult = 0;

	int iSelItem = GetSelItem ();

	if ((m_uiStandardBtns & BGCEDITLISTBOX_BTN_NEW) && iSelItem == -1)
	{
		CreateNewItem ();
		return;
	}

	if (iSelItem > 0)
	{
		EditItem (iSelItem);
	}
}

void CPOVList::OnSelectionChanged ()
{
	int iSelItem = GetSelItem ();

	int nb = GetButtonsCount();
	for (int i=1;i<nb;i++)
	{
		EnableButton(i,(BOOL)iSelItem);
	}


}

BOOL CPOVList::EditItem (int iIndex)
{
	BOOL bRes = FALSE;
	if (iIndex)
	{
		bRes = CBCGPEditListBox::EditItem (iIndex);
	}
	return bRes;
}

/////////////////////////////////////////////////////////////////////////////
// CPrefUniverse dialog
IMPLEMENT_DYNCREATE(CPrefUniverse, CBCGPPropertyPage)


CPrefUniverse::CPrefUniverse()
	:	CBCGPPropertyPage(CPrefUniverse::IDD)//,
		//m_cPOVList(this)
{	
	//{{AFX_DATA_INIT(CPrefUniverse)
	m_bMagnet = TPref::TUniv.bMagnet;
	m_bSynchro = TPref::TUniv.bSynchron;
	m_nRefInit = TPref::TUniv.nDefRep;
	m_bShowPolygon = !TPref::TUniv.bShowPolygon;
	//}}AFX_DATA_INIT
}


void CPrefUniverse::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefUniverse)
	DDX_Control(pDX, IDC_PUNIV_TRACE, m_wndTraceColor);
	DDX_Check(pDX, IDC_PUNIV_MAGNET, m_bMagnet);
	DDX_Check(pDX, IDC_PUNIV_SYNCHRO, m_bSynchro);
	DDX_CBIndex(pDX, IDC_PUNIV_REFINIT, m_nRefInit);
	DDX_Check(pDX, IDC_PUNIV_POLYGONS, m_bShowPolygon);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		TPref::TUniv.bMagnet = m_bMagnet;
		TPref::TUniv.bSynchron = m_bSynchro;
		TPref::TUniv.nDefRep = m_nRefInit;
		TPref::TUniv.bShowPolygon = !m_bShowPolygon;

		TPref::TUniv.clrTrace = m_wndTraceColor.GetColor();

	}
}


BEGIN_MESSAGE_MAP(CPrefUniverse, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefUniverse)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefUniverse message handlers

void CPrefUniverse::OnDestroy() 
{
	UpdateData();
	CBCGPPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CPrefUniverse::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	//-------------
	// Add buttons:
	//-------------
//	m_cPOVList.SetStandardButtons (BGCEDITLISTBOX_BTN_NEW|BGCEDITLISTBOX_BTN_DELETE);

	//------------
	// Fill tools:
	//------------
/*	int nb = TPref::TPOVList.GetCount();
	if (!nb)
	{
		m_cPOVList.AddItem(_T("< Default >"),NULL);
	}

	for(POSITION pos = TPref::TPOVList.GetHeadPosition(); pos != NULL; )
	{
		CMacroUserTool* pTool = (CMacroUserTool*)TPref::TPOVList.GetNext( pos );
		if (pTool)
			m_cPOVList.AddItem (pTool->m_strLabel, (DWORD) pTool);
	}*/

	// Initialize color names:
	int m_nNumColours2 = sizeof (TPref::crColours)/sizeof(TPref::ColourTableEntry);
	for (int i = 0; i < m_nNumColours2; i++)
	{
		CBCGPColorMenuButton::SetColorName (TPref::crColours[i].crColour, TPref::crColours[i].szName);
	}

	CString strDef,strOther,strDoc;
	strDef.LoadString(PREF_NAME_CLRAUTO);
	strOther.LoadString(PREF_NAME_CLROTHER);
	strDoc.LoadString(PREF_NAME_CLRDOC);
	//m_wndTraceColor.SetPalette(&m_palColorPicker);
	//m_wndTraceColor.EnableAutomaticButton (strDef, m_clrDefObj);
	m_wndTraceColor.EnableOtherButton (strOther);
	m_wndTraceColor.SetColor (TPref::TUniv.clrTrace);
	m_wndTraceColor.SetColumnsNumber (8);
	m_wndTraceColor.m_bEnabledInCustomizeMode = TRUE;
	//m_wndTraceColor.SetDocumentColors(strDoc,TPref::docColors);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPrefUniverse::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
