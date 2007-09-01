// POVUserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "calques3d.h"
#include "POVUserDialog.h"
#include "prefs\Prefs.h"
#include "prefs\POVTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOVUserDialog dialog
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
	int nb = GetCount();

	if ((m_uiStandardBtns & BGCEDITLISTBOX_BTN_NEW) && iSelItem == -1 && nb<5)
	{
		CreateNewItem ();
		return;
	}

	if (iSelItem > 0)
	{
		EditItem (iSelItem);
	}
}

BOOL CPOVList::OnBeforeRemoveItem (int iItem)	
{	
	CPOVUserTool* pSelTool = (CPOVUserTool*) GetItemData (iItem);
	if (pSelTool)
	{
		for (POSITION pos = m_pParent->m_pObjList->GetHeadPosition (); pos != NULL;)
		{
			POSITION locpos = pos;
			CPOVUserTool *pObj = m_pParent->m_pObjList->GetNext (pos);
			if (!pObj) continue;
			if (pObj==pSelTool) 
			{
				m_pParent->m_pObjList->RemoveAt(locpos);
				break;
			}
		}
		delete pSelTool;
		SetItemData(iItem,(DWORD)NULL);
	}
	return TRUE;	
}
	
void CPOVList::OnAfterAddItem (int iItem) 
{
	CString str = GetItemText(iItem);
	CPOVUserTool *tool = new CPOVUserTool();
	tool->m_bCanRemove = TRUE;
	tool->m_strLabel = str;
	tool->m_projParam = m_pParent->m_projParam;
	SetItemData(iItem,(DWORD)tool);
	m_pParent->m_pObjList->AddHead(tool);
	m_pParent->ShowProperty(tool);

}

void CPOVList::OnSelectionChanged ()
{
	int iSelItem = GetSelItem ();

	int nb = GetButtonsCount();
	int nbitem = GetCount();
	EnableButton(0,iSelItem>1);
	for (int i=1;i<nb-1;i++)
	{
		EnableButton(i,(BOOL)iSelItem>0);
	}
	EnableButton(nb-1,nbitem<5);

	CPOVUserTool* pSelTool = (iSelItem < 0) ? 
		NULL : (CPOVUserTool*) GetItemData (iSelItem);
	m_pParent->ShowProperty(pSelTool);
}

void CPOVList::OnAfterMoveItemUp (int iItem)
{
	OnSelectionChanged ();
}

void CPOVList::OnAfterMoveItemDown (int /*iItem*/)
{
	OnSelectionChanged ();
}

void CPOVList::OnAfterRenameItem (int /*iItem*/)
{
	OnSelectionChanged ();
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
// CPOVUserDialog dialog


CPOVUserDialog::CPOVUserDialog(CWnd* pParent /*=NULL*/)
	:	CDialog(CPOVUserDialog::IDD, pParent),
		m_wndPOVListBox(this)
{
	m_pObjList = NULL;
	//{{AFX_DATA_INIT(CPOVUserDialog)
	m_strTheta = _T("");
	m_strPhi = _T("");
	//}}AFX_DATA_INIT
}


void CPOVUserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOVUserDialog)
	DDX_Control(pDX, IDC_POV_LIST, m_wndPOVListBox);
	DDX_Text(pDX, IDC_POV_THETA, m_strTheta);
	DDX_Text(pDX, IDC_POV_PHI, m_strPhi);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		int nb = m_wndPOVListBox.GetCount();
		m_pObjList->RemoveAll();
		for (int i=0;i<nb;i++)
		{
			CPOVUserTool *pObj = (CPOVUserTool *)m_wndPOVListBox.GetItemData(i);
			CString str = m_wndPOVListBox.GetItemText(i);
			if (!pObj) continue;
			pObj->m_strLabel = str;
			m_pObjList->AddTail(pObj);
		}
	}
}


BEGIN_MESSAGE_MAP(CPOVUserDialog, CDialog)
	//{{AFX_MSG_MAP(CPOVUserDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOVUserDialog message handlers

BOOL CPOVUserDialog::OnInitDialog() 
{

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UINT nStdBtn = BGCEDITLISTBOX_BTN_UP | BGCEDITLISTBOX_BTN_DOWN;
	m_wndPOVListBox.SetStandardButtons (nStdBtn);
	m_wndPOVListBox.AddButton(IDB_LIST_DELITEM,"Remove this point of view",VK_DELETE,0,BGCEDITLISTBOX_BTN_DELETE_ID);
	m_wndPOVListBox.AddButton(IDB_LIST_ADDITEM,"Save the current point of view",VK_INSERT,0,BGCEDITLISTBOX_BTN_NEW_ID);
	nStdBtn |= BGCEDITLISTBOX_BTN_NEW | BGCEDITLISTBOX_BTN_DELETE;
	m_wndPOVListBox.SetStandardBtns(nStdBtn);
	//m_wndPOVListBox.EnableBrowseButton ();
	m_wndPOVListBox.SetGrayDisabledButtons();
	
	//------------
	// Fill tools:
	//------------
	if (m_pObjList==NULL) return FALSE;
// 	for(POSITION pos = TPref::TMacroList.GetHeadPosition(); pos != NULL; )
// 	{
// 		CMacroUserTool* pTool = (CMacroUserTool*)TPref::TMacroList.GetNext( pos );
// 		if (pTool)
// 			m_wndMacroListBox.AddItem (pTool->m_strLabel, (DWORD) pTool);
// 	}

	for (POSITION pos = m_pObjList->GetHeadPosition (); pos != NULL;)
	{
		CPOVUserTool *pObj = m_pObjList->GetNext (pos);
		if (pObj) m_wndPOVListBox.AddItem(pObj->m_strLabel,(DWORD)pObj);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPOVUserDialog::OnOK()
{
	CDialog::OnOK();
}

void CPOVUserDialog::OnCancel()
{
	UpdateData(TRUE);
	CDialog::OnCancel();
}

void CPOVUserDialog::ShowProperty(CPOVUserTool* prop)
{
	m_strPhi = m_strTheta = _T("");
	if (prop!=NULL)
	{
		SProjection proj = prop->m_projParam;
		m_strTheta.Format(_T("%.2f"),proj.theta);
		m_strPhi.Format(_T("%.2f"),proj.phi);
	}

	UpdateData(FALSE);
}
