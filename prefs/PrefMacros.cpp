// PrefMacros.cpp : implementation file
//

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


// "BCGToolsPage.cpp"
/////////////////////////////////////////////////////////////////////////////
// CPrefMacros dialog
BOOL CMacroList::OnBeforeRemoveItem (int iItem)
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
	m_pParent->m_pSelTool = NULL;
	return TRUE;
}
//***************************************************************************
void CMacroList::OnAfterAddItem (int iItem)
{
	CMacroUserTool* pTool = new CMacroUserTool();
	//pTool->m_strLabel = GetItemText (iItem);
	TPref::TMacroList.AddTail(pTool);
	SetItemData (iItem, (DWORD) pTool);

	OnSelectionChanged ();
}
//***************************************************************************
void CMacroList::OnAfterRenameItem (int iItem)
{
	CMacroUserTool* pTool = (CMacroUserTool*) GetItemData (iItem);
	ASSERT_VALID (pTool);

	//pTool->m_strLabel = GetItemText (iItem);
}
//***************************************************************************
void CMacroList::OnAfterMoveItemUp (int iItem)
{
	CMacroUserTool* pTool = (CMacroUserTool*) GetItemData (iItem);
	ASSERT_VALID (pTool);

//	g_pUserToolsManager->MoveToolUp (pTool);
}
//***************************************************************************
void CMacroList::OnAfterMoveItemDown (int iItem)
{
	CMacroUserTool* pTool = (CMacroUserTool*) GetItemData (iItem);
	ASSERT_VALID (pTool);
//
//	g_pUserToolsManager->MoveToolDown (pTool);
}
//**************************************************************************
void CMacroList::OnSelectionChanged ()
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

	//ASSERT_VALID (m_pParent->m_pParentSheet);
	//m_pParent->m_pParentSheet->OnBeforeChangeTool (m_pParent->m_pSelTool);

	m_pParent->m_pSelTool = pSelTool;
	m_pParent->UpdateData (FALSE);

	m_pParent->EnableControls ();

//	m_pParent->m_pParentSheet->OnAfterChangeTool (m_pParent->m_pSelTool);*/
}

void CMacroList::CreateNewItem ()
{
	int iLastItem = AddItem (_T("new macro"));
	ASSERT (iLastItem >= 0);

	m_bNewItem = TRUE;
	EditItem (iLastItem);
}


/////////////////////////////////////////////////////////////////////////////
// CPrefMacros dialog
IMPLEMENT_DYNCREATE(CPrefMacros, CBCGPPropertyPage)


CPrefMacros::CPrefMacros()
	:	CBCGPPropertyPage(CPrefMacros::IDD),
		m_cMacroList (this)
{
	//{{AFX_DATA_INIT(CPrefMacros)
	m_strMacroPath = _T("");
	m_strMacroDef = _T("");
	m_strMacroName = _T("");
	m_bFreeLoad = TPref::bMacroLoading;
	//}}AFX_DATA_INIT

	m_pSelTool = NULL;
}


void CPrefMacros::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefMacros)
	DDX_Control(pDX, IDC_PMAC_DEF, m_cDef);
	DDX_Control(pDX, IDC_PMAC_NAME, m_cName);
	DDX_Control(pDX, IDC_PMAC_PATH, m_cPath);
	DDX_Control(pDX, IDC_PMAC_BROWSE, m_cBrowse);
	DDX_Control(pDX, IDC_PMAC_LIST, m_cMacroList);
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


BEGIN_MESSAGE_MAP(CPrefMacros, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefMacros)
	ON_BN_CLICKED(IDC_PMAC_BROWSE, OnMacroBrowse)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefMacros message handlers

BOOL CPrefMacros::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();
	
	//-------------
	// Add buttons:
	//-------------
	m_cMacroList.SetStandardButtons (BGCEDITLISTBOX_BTN_NEW | BGCEDITLISTBOX_BTN_DELETE);

	//------------
	// Fill tools:
	//------------
	for(POSITION pos = TPref::TMacroList.GetHeadPosition(); pos != NULL; )
	{
		CMacroUserTool* pTool = (CMacroUserTool*)TPref::TMacroList.GetNext( pos );
		if (pTool)
			m_cMacroList.AddItem (pTool->m_strLabel, (DWORD) pTool);
	}

	EnableControls ();	// By Andreas Roth

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefMacros::EnableControls ()
{
	BOOL bEnableItemProps = (m_cMacroList.GetSelItem () >= 0);

	m_cPath.EnableWindow (bEnableItemProps);
	m_cName.EnableWindow (bEnableItemProps);
	m_cDef.EnableWindow (bEnableItemProps);
	m_cBrowse.EnableWindow (bEnableItemProps);
}

void CPrefMacros::OnUpdateTool() 
{
	UpdateData ();

	int iSelItem = m_cMacroList.GetSelItem ();
	CMacroUserTool* pSelTool = (iSelItem >= 0) ?
		(CMacroUserTool*) m_cMacroList.GetItemData (iSelItem) : NULL;

	if (pSelTool == NULL)
	{
		m_strMacroPath.Empty ();
		m_strMacroName.Empty ();
		m_strMacroDef.Empty ();

		UpdateData (FALSE);
	}
	else
	{
		ASSERT_VALID (pSelTool);

		pSelTool->m_strFile = m_strMacroPath;
		pSelTool->m_strLabel = m_strMacroName;
		pSelTool->m_strDef = m_strMacroDef;
	}

	EnableControls ();	// By Andreas Roth
}


void CPrefMacros::OnMacroBrowse() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Macro-construction (*.m3d)||";

	//CFileDialog dlg (TRUE, g_pUserToolsManager->GetDefExt (), NULL, 0,
	//	g_pUserToolsManager->GetFilter (), this);
	
	CFileDialog mdlg(TRUE,"*.m3d","*.m3d",
			OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,
			szFilter,
			this);

	if (mdlg.DoModal () == IDOK)
	{
		CCalques3DMacroDoc mdoc;
		mdoc.OnOpenDocument(mdlg.GetPathName ());

		m_strMacroPath = mdlg.GetPathName ();
		m_strMacroName = mdoc.strObjectName;
		m_strMacroDef = mdoc.strObjectDef;
		UpdateData (FALSE);
		OnUpdateTool();
	}
}

CMacroUserTool* CPrefMacros::CreateNewTool ()
{
	//ASSERT_VALID (m_pParentSheet);

	const int nMaxTools = 10;//g_pUserToolsManager->GetMaxTools ();

	if (TPref::TMacroList.GetCount () == nMaxTools)
	{
		//CBCGLocalResource locaRes;
		
		//CString strError;
		//strError.Format (IDS_BCGBARRES_TOO_MANY_TOOLS_FMT, nMaxTools);
		//MessageBox (strError);
		return NULL;
	}

	//CBCGUserTool* pTool = g_pUserToolsManager->CreateNewTool ();
	//ASSERT_VALID (pTool);
	CMacroUserTool* pTool = new CMacroUserTool();

	return pTool;
}

void CPrefMacros::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnUpdateTool();
	CBCGPPropertyPage::OnOK();
}

void CPrefMacros::OnDestroy() 
{
	OnUpdateTool();
	CBCGPPropertyPage::OnDestroy();
}
