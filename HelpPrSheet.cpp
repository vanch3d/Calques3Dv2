// HelpPrSheet.cpp : implementation file
//

#include "stdafx.h"
#include "HelpPrSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLOATING_ARROW

/////////////////////////////////////////////////////////////////////////////
// CHelpPrSheet

IMPLEMENT_DYNAMIC(CHelpPrSheet, CPropertySheet)

CHelpPrSheet::CHelpPrSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CHelpPrSheet::CHelpPrSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CHelpPrSheet::~CHelpPrSheet()
{
}


BEGIN_MESSAGE_MAP(CHelpPrSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHelpPrSheet)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpPrSheet message handlers


/////////////////////////////////////////////////////////////////////////////
//	Name:	PreTranslateMessage
//	Desc:	We are filtering the special MFC 0x4d message which gets sent on
//			an F1 press.
//	Args:	(See MFC doc)
//	Ret:	TRUE if handled.
/////////////////////////////////////////////////////////////////////////////
BOOL CHelpPrSheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == 0x4d) {
		// Magical mystical MFC F1 Help msg!
		if (GetKeyState(VK_SHIFT) >= 0) {
			// Shift key not down
			// Supply general dialog level help
			OnCommandHelp(0, 0);
			
			return(TRUE);		// Eat it
		}
#ifdef FLOATING_ARROW
		else {
			// Use this if you want Shift+F1 to create the floating arrow instead
			SendMessage(WM_SYSCOMMAND, SC_CONTEXTHELP);
			return(TRUE);
		}
#endif FLOATING_ARROW
	}
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
//	Name:	OnInitDialog
//	Desc:	Just sets WS_EX_CONTEXTHELP so that we get the cute '?'.
/////////////////////////////////////////////////////////////////////////////
BOOL CHelpPrSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: Add your specialized code here
	ModifyStyleEx(0, WS_EX_CONTEXTHELP);
	
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
//	Name:	OnHelpInfo
//	Desc:	Supply control-level context Help.
//	Args:	(See MFC doc)
//	Ret:	TRUE if handled.
/////////////////////////////////////////////////////////////////////////////
BOOL CHelpPrSheet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("CHelpPrSheet::OnHelpInfo iCtrlId = %x hItemHandle = %x dwContextId = %x\n", pHelpInfo->iCtrlId, pHelpInfo->hItemHandle, pHelpInfo->dwContextId);
	
	if (pHelpInfo->iContextType == HELPINFO_WINDOW) {
		AfxGetApp()->WinHelp(pHelpInfo->dwContextId, HELP_CONTEXTPOPUP);
	}

	return(TRUE);
}
