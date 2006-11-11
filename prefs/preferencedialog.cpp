// preferencedialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "preferencedialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog

IMPLEMENT_DYNAMIC(CPreferenceDialog, CBCGPPropertySheet)

CPreferenceDialog::CPreferenceDialog(CWnd* pParentWnd, UINT iSelectPage)
	:CBCGPPropertySheet("dfdfd", pParentWnd, iSelectPage)
{
/*	SetLook (CBCGPPropertySheet::PropSheetLook_OutlookBar);
	SetIconsList (IDB_PREFS_ICONS, 16);
	AddPage(&m_wndPage1);
	AddPage(&m_wndPage2);
	AddPage(&m_wndPage3);
	AddPage(&m_wndPage4);*/

		SetLook (CBCGPPropertySheet::PropSheetLook_Tree, 150 /* Tree control width */);
	SetIconsList (IDB_PREFS_ICONS, 16 /* Image width */);

	CBCGPPropSheetCategory* pCat1 = AddTreeCategory (_T("Environment"), 0, 1);

	AddPageToTree (pCat1, &m_wndPage1, -1, 2);
	AddPageToTree (pCat1, &m_wndPage2, -1, 2);

	CBCGPPropSheetCategory* pCat2 = AddTreeCategory (_T("Source Control"), 0, 1);

	AddPageToTree (pCat2, &m_wndPage3, -1, 2);
	AddPageToTree (pCat2, &m_wndPage4, -1, 2);
}

CPreferenceDialog::~CPreferenceDialog()
{
}


BEGIN_MESSAGE_MAP(CPreferenceDialog, CBCGPPropertySheet)
	//{{AFX_MSG_MAP(CPreferenceDialog)
	ON_WM_CONTEXTMENU()
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog message handlers

LRESULT CPreferenceDialog::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (LOWORD (lp), HIWORD(lp));

	return 0;
}


void CPreferenceDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}

BOOL CPreferenceDialog::OnInitDialog() 
{
	CBCGPPropertySheet::OnInitDialog();
	
	ModifyStyleEx(0, WS_EX_CONTEXTHELP);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
