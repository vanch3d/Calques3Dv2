// preferencedialog.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "..\resource.hm"
#include "preferencedialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BCGCBHELP_OFFSET	0x41000
static const int iButtonMargin = 8;

static const DWORD dwBCGResHelpIDs [] = 
{
	// Commands
	IDC_SHAPE_HIDDEN,						HIDC_SHAPE_HIDDEN,
	IDC_BCGBARRES_CATEGORY,						BCGCBHELP_OFFSET + IDC_BCGBARRES_CATEGORY,
	IDC_BCGBARRES_USER_TOOLS,					BCGCBHELP_OFFSET + IDC_BCGBARRES_USER_TOOLS,
	IDC_BCGBARRES_BUTTON_DESCR,					BCGCBHELP_OFFSET + IDC_BCGBARRES_BUTTON_DESCR,

};
/////////////////////////////////////////////////////////////////////////////
// CPreferenceDialog

IMPLEMENT_DYNAMIC(CPreferenceDialog, CBCGPPropertySheet)

CPreferenceDialog::CPreferenceDialog(CWnd* pParentWnd, UINT iSelectPage)
	:CBCGPPropertySheet(_T(""), pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_strCaption.LoadString(IDB_PREFC3D_ICONS);
	SetLook (CBCGPPropertySheet::PropSheetLook_OutlookBar);
	SetIconsList (IDB_PREFC3D_ICONS, 16);
	AddPage(&m_wndSession);
	AddPage(&m_wndShape);
	AddPage(&m_wndColours);
	AddPage(&m_wndUniverse);
	AddPage(&m_wndMathPad);
	AddPage(&m_wndMacros);


/*	SetLook (CBCGPPropertySheet::PropSheetLook_Tree, 150);
	SetIconsList (IDB_PREFC3D_ICONS, 16);

	CBCGPPropSheetCategory* pCat1 = AddTreeCategory (_T("Starting Calques 3D"), 0, 0);

	AddPageToTree (pCat1, &m_wndSession, -1, 5);

	CBCGPPropSheetCategory* pCat2 = AddTreeCategory (_T("Appearance"), 0, 0);

	AddPageToTree (pCat2, &m_wndShape, -1, 6);
	AddPageToTree (pCat2, &m_wndColours, -1, 1);

	CBCGPPropSheetCategory* pCat3 = AddTreeCategory (_T("Views"), 0, 0);

	AddPageToTree (pCat3, &m_wndUniverse, -1, 7);
	AddPageToTree (pCat3, &m_wndMathPad, -1, 8);

	CBCGPPropSheetCategory* pCat4 = AddTreeCategory (_T("Utilities"), 0, 0);
	AddPageToTree (pCat4, &m_wndMacros, -1, 9);*/
}

CPreferenceDialog::~CPreferenceDialog()
{
}


BEGIN_MESSAGE_MAP(CPreferenceDialog, CBCGPPropertySheet)
	//{{AFX_MSG_MAP(CPreferenceDialog)
	ON_WM_CONTEXTMENU()
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_WM_SIZE()
	ON_WM_HELPINFO()
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
	CBCGPWorkspace* g_pWorkspace=GetWorkspace();
		if (g_pWorkspace != NULL)
	{
		g_pWorkspace->OnAppContextHelp (pWnd, dwBCGResHelpIDs);
	}

}

BOOL CPreferenceDialog::OnInitDialog() 
{
	BOOL bResult = CBCGPPropertySheet::OnInitDialog();
	ModifyStyleEx(0, WS_EX_CONTEXTHELP);
	
	CRect rectClient;	// Client area rectangle
	GetClientRect (&rectClient);

	//----------------------
	// Hide "OK" button:
	//----------------------
	CWnd *pWndOK = GetDlgItem (IDOK);
	if (pWndOK == NULL)
	{
		return bResult;
	}

	pWndOK->ShowWindow (SW_HIDE);
	pWndOK->EnableWindow (FALSE);

	//----------------------
	// Show "Cancel" button:
	//----------------------
	CWnd *pWndCancel = GetDlgItem (IDCANCEL);
	if (pWndCancel == NULL)
	{
		return bResult;
	}

	pWndCancel->ShowWindow (SW_SHOW);
	pWndCancel->EnableWindow ();

	CRect rectClientCancel;
	pWndCancel->GetClientRect (&rectClientCancel);
	pWndCancel->MapWindowPoints (this, &rectClientCancel);

	//-------------------------------
	// Enlarge property sheet window:
	//-------------------------------
	CRect rectWnd;
	GetWindowRect(rectWnd);	

	SetWindowPos(NULL, 0, 0,
		rectWnd.Width (),
		rectWnd.Height () + iButtonMargin,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	//-------------------------------------------------
	// Move "Cancel" button to the right bottom corner:
	//-------------------------------------------------
	pWndCancel->SetWindowPos (NULL, 
		rectClient.right - rectClientCancel.Width () - iButtonMargin,
		rectClientCancel.top + iButtonMargin / 2,
		0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	//---------------------------------------------------
	// Change "Cancel" button's style to "DEFPUSHBUTTON":
	//---------------------------------------------------
	CWnd *pWndOk = GetDlgItem (IDOK);
	if (pWndOk != NULL)
	{
		pWndOk->ModifyStyle (BS_DEFPUSHBUTTON, 0);
	}

	pWndCancel->ModifyStyle (0, BS_DEFPUSHBUTTON);

	//--------------------------------------------------------
	// Replace "Cancel" text to "Close" 
	// (CPropertyPage::CancelToClose method does nothing in a 
	// modeless property sheet):
	//--------------------------------------------------------
	CString strCloseText;
	
	{
		//CBCGLocalResource locaRes;
		strCloseText.LoadString (IDS_BCGBARRES_CLOSE);
	}

	pWndCancel->SetWindowText (strCloseText);

	//------------------------
	// Adjust the Help button:
	//------------------------
	CButton *pWndHelp = (CButton*) GetDlgItem (IDHELP);
	if (pWndHelp == NULL)
	{
		return bResult;
	}

	{
		pWndHelp->ShowWindow (SW_SHOW);
		pWndHelp->EnableWindow ();

		//-----------------------
		// Set Help button image:
		//-----------------------
		LPCTSTR lpszResourceName = MAKEINTRESOURCE (IDB_BCGBARRES_HELP);
		ASSERT(lpszResourceName != NULL);

		HBITMAP hbmpHelp = (HBITMAP) ::LoadImage (
			AfxFindResourceHandle (lpszResourceName, RT_BITMAP),
			lpszResourceName,
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
		ASSERT (hbmpHelp != NULL);

		pWndHelp->ModifyStyle (0, BS_BITMAP);
		pWndHelp->SetBitmap (hbmpHelp);

		//-------------------------------------------------
		// Move "Help" button to the left bottom corner and
		// adjust its size by the bitmap size:
		//-------------------------------------------------
		BITMAP bmp;
		::GetObject (hbmpHelp, sizeof (BITMAP), &bmp);
		
		pWndHelp->SetWindowPos (NULL, 
			rectClient.left + iButtonMargin,
			rectClientCancel.top,
			bmp.bmWidth + iButtonMargin, bmp.bmHeight + iButtonMargin,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	return bResult;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreferenceDialog::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPPropertySheet::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CPreferenceDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		AfxGetApp()->WinHelp(pHelpInfo->dwContextId, HELP_CONTEXTPOPUP);
		return TRUE;
	}

	return CBCGPPropertySheet::OnHelpInfo(pHelpInfo);
}

BOOL CPreferenceDialog::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == 0x4d) {
		// Magical mystical MFC F1 Help msg!
		if (GetKeyState(VK_SHIFT) >= 0) {
			// Shift key not down
			// Supply general dialog level help
			OnCommandHelp(0, 0);
			
			return(TRUE);		// Eat it
		}
		else {
			// Use this if you want Shift+F1 to create the floating arrow instead
			SendMessage(WM_SYSCOMMAND, SC_CONTEXTHELP);
			return(TRUE);
		}
	}	
	if (pMsg->message == WM_RBUTTONUP || pMsg->message == WM_CONTEXTMENU)
	{

		CPropertyPage* ppage = GetActivePage();
		CBCGPBaseTabWnd* ptab = m_wndOutlookBar.GetUnderlinedWindow ();
		if  (pMsg->hwnd == m_wndPane1.GetSafeHwnd() || pMsg->hwnd == ptab->GetSafeHwnd())
			return(TRUE);		// Eat it
	}
	return CBCGPPropertySheet::PreTranslateMessage(pMsg);
}
