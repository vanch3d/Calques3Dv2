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
// PrefMacros.cpp: implementation of the CPrefMacros class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefMacros.h"


#include "..\Calques3DDoc.h"

#include "Prefs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBCGPMacroList 

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////
CBCGPMacroList::CBCGPMacroList(CPrefMacros* pParent) : CBCGPEditListBox()
{
	m_pParent = pParent;
	m_bBrowse = FALSE;
}
 
/////////////////////////////////////////////////////////////////////////////
/// Used by the framework to receive notifications that a list box item is about to be 
/// removed from the list box. 
/// Remove the corresponding user macro from the list and from the interface.
///
/// @param iItem	The zero-based index of the item to remove
/// @return			TRUE is the item can be removed, FALSE otherwise
/////////////////////////////////////////////////////////////////////////////
BOOL CBCGPMacroList::OnBeforeRemoveItem (int iItem)
{
	CMacroUserTool* pTool = (CMacroUserTool*) GetItemData (iItem);
	ASSERT_VALID (pTool);

	POSITION pos1,pos2;
	
	for( pos1 = TPref::TMacroList.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		CMacroUserTool* pT1 = (CMacroUserTool*)TPref::TMacroList.GetNext( pos1 );
		if( pT1 == pTool)
		{
           CMacroUserTool *pa = (CMacroUserTool*)TPref::TMacroList.GetAt( pos2 ); // Save the old pointer for
                                  //deletion.
           TPref::TMacroList.RemoveAt( pos2 );
           delete pa; // Deletion avoids memory leak.
       }
   }
	//g_pUserToolsManager->RemoveTool (pTool);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the the label of an item has been edited.
/// Fix to prevent automatic updating of the item when the browse dialog box is opened.
/////////////////////////////////////////////////////////////////////////////
void CBCGPMacroList::OnEndEditLabel (LPCTSTR lpszLabel)
{
	if (m_bBrowse) return;
	CBCGPEditListBase::OnEndEditLabel (lpszLabel);
}

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when the user click on the browse button associated with a macro item.
/////////////////////////////////////////////////////////////////////////////
void CBCGPMacroList::OnBrowse ()
{
	int iSelItem = GetSelItem ();

	static char BASED_CODE szFilter[] = "Macro-construction (*.m3d)||";
	m_bBrowse=TRUE;
	
	CFileDialog mdlg(TRUE,"*.m3d","*.m3d",
			OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
			szFilter,
			this);
	//SetItemText(iSelItem,"< browse >");

	if (mdlg.DoModal () == IDOK)
	{
		CCalques3DMacroDoc mdoc;
		mdoc.OnOpenDocument(mdlg.GetPathName ());

		CString strMacroPath = mdlg.GetPathName ();
		CString strMacroName = mdoc.strObjectName;
		CString strMacroDef = mdoc.strObjectDef;

		CMacroUserTool* pTool = new CMacroUserTool();
		pTool->m_strDef = strMacroDef;
		pTool->m_strFile = strMacroPath;
		pTool->m_strLabel = strMacroName;
		TPref::TMacroList.AddTail(pTool);
		SetItemData (iSelItem, (DWORD) pTool);
		SetItemText(iSelItem,strMacroName);
		OnSelectionChanged ();
		m_bNewItem = false;
		m_bBrowse=FALSE;
	}
	else
	{	
		m_bBrowse=FALSE;
		OnEndEditLabel ("");
	}
}

/*void CBCGPMacroList::OnClickButton (int iButton)
{
	int iSelItem = GetSelItem ();
	UINT uiBtnID = GetButtonID (iButton);

	if (uiBtnID==BGCEDITLISTBOX_BTN_FOLDER_ID)
	{
	}
	else
	{
		UINT temp = m_uiStandardBtns;
		m_uiStandardBtns = 1;
		CBCGPEditListBase::OnClickButton (iButton);
		m_uiStandardBtns = temp;
	}

}*/

/////////////////////////////////////////////////////////////////////////////
/// Called by the framework when selection in the list control has changed. 
/// The interface in the Preference dialog is updated to reflect the information 
/// associated with the selected macro (ie name, file, description).
/////////////////////////////////////////////////////////////////////////////
void CBCGPMacroList::OnSelectionChanged ()
{
	int iSelItem = GetSelItem ();
	CMacroUserTool* pSelTool = (iSelItem < 0) ? 
		NULL : (CMacroUserTool*) GetItemData (iSelItem);

	if (pSelTool == NULL)
	{
		m_pParent->m_strMacroPath.Empty ();
		m_pParent->m_strMacroName.Empty ();
		m_pParent->m_strMacroDef.Empty ();
	}
	else
	{
		ASSERT_VALID (pSelTool);

		m_pParent->m_strMacroPath = pSelTool->m_strFile;
		m_pParent->m_strMacroName = pSelTool->m_strLabel;
		m_pParent->m_strMacroDef = pSelTool->m_strDef;
	}

	m_pParent->UpdateData (FALSE);
	m_pParent->EnableControls ();
}


/////////////////////////////////////////////////////////////////////////////
// CPrefMacros dialog

IMPLEMENT_DYNCREATE(CPrefMacros, CBCGPPropertyPage)

BEGIN_MESSAGE_MAP(CPrefMacros, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefMacros)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////
CPrefMacros::CPrefMacros():	
		CBCGPPropertyPage(CPrefMacros::IDD),
		m_wndMacroListBox (this)
{
	//{{AFX_DATA_INIT(CPrefMacros)
	m_strMacroPath = _T("");
	m_strMacroDef = _T("");
	m_strMacroName = _T("");
	m_bFreeLoad = TPref::bMacroLoading;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
/// Called by the framework to exchange and validate dialog data.
/////////////////////////////////////////////////////////////////////////////
void CPrefMacros::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefMacros)
	DDX_Control(pDX, IDC_PMAC_DEF, m_cDef);
	DDX_Control(pDX, IDC_PMAC_NAME, m_cName);
	DDX_Control(pDX, IDC_PMAC_PATH, m_cPath);
	DDX_Control(pDX, IDC_PMAC_LIST, m_wndMacroListBox);
	DDX_Text(pDX, IDC_PMAC_PATH, m_strMacroPath);
	DDX_Text(pDX, IDC_PMAC_DEF, m_strMacroDef);
	DDX_Text(pDX, IDC_PMAC_NAME, m_strMacroName);
	DDX_Check(pDX, IDC_PMAC_LOADMACRO, m_bFreeLoad);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		TPref::bMacroLoading = m_bFreeLoad;
	}
}


/////////////////////////////////////////////////////////////////////////////
/// Initialise the content of the dialog box. 
/////////////////////////////////////////////////////////////////////////////
BOOL CPrefMacros::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	//CBCGPButton::EnableWinXPTheme ();
	
	//-------------
	// Add buttons:
	//-------------
	//m_wndMacroListBox.AddButton(IDB_LIST_ADDITEM,"Add",0,0,BGCEDITLISTBOX_BTN_NEW_ID);
	//m_wndMacroListBox.AddButton(IDB_LIST_DELITEM,"Remove",0,0,BGCEDITLISTBOX_BTN_DELETE_ID);
	//m_wndMacroListBox.AddButton(IDB_LIST_BROWSE,"Browse",0,0,BGCEDITLISTBOX_BTN_FOLDER_ID);
	m_wndMacroListBox.SetStandardButtons (BGCEDITLISTBOX_BTN_NEW | BGCEDITLISTBOX_BTN_DELETE);
	m_wndMacroListBox.EnableBrowseButton ();
	m_wndMacroListBox.SetGrayDisabledButtons();

	//------------
	// Fill tools:
	//------------
	for(POSITION pos = TPref::TMacroList.GetHeadPosition(); pos != NULL; )
	{
		CMacroUserTool* pTool = (CMacroUserTool*)TPref::TMacroList.GetNext( pos );
		if (pTool)
			m_wndMacroListBox.AddItem (pTool->m_strLabel, (DWORD) pTool);
	}

	EnableControls ();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
/// Enable/disable the edit controls, depending in the selection of an item in the
/// user macro list box.
/////////////////////////////////////////////////////////////////////////////
void CPrefMacros::EnableControls ()
{
	BOOL bEnableItemProps = (m_wndMacroListBox.GetSelItem () >= 0);

	m_cPath.EnableWindow (bEnableItemProps);
	m_cName.EnableWindow (bEnableItemProps);
	m_cDef.EnableWindow (bEnableItemProps);
}

/////////////////////////////////////////////////////////////////////////////
/// Called when the user clicks the OK button. Update the data before closing.
/////////////////////////////////////////////////////////////////////////////
void CPrefMacros::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData ();
	CBCGPPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
/// Called when the dialog box is being destroyed. Update the data before closing.
/////////////////////////////////////////////////////////////////////////////
void CPrefMacros::OnDestroy() 
{
	UpdateData ();
	CBCGPPropertyPage::OnDestroy();
}
