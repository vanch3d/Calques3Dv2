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
// HelpDlg.cpp: implementation of the CHelpDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg dialog


CHelpDlg::CHelpDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CHelpDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CHelpDlg)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDHELP, OnHelpButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDlg message handlers


/////////////////////////////////////////////////////////////////////////////
//	Name:	PreTranslateMessage
//	Desc:	We are filtering the special MFC 0x4d message which gets sent on
//			an F1 press.
//	Args:	(See MFC doc)
//	Ret:	TRUE if handled.
/////////////////////////////////////////////////////////////////////////////
BOOL CHelpDlg::PreTranslateMessage(MSG* pMsg) 
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
	
	return CDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
//	Name:	OnHelpInfo
//	Desc:	Supply control-level context Help.
//	Args:	(See MFC doc)
//	Ret:	TRUE if handled.
/////////////////////////////////////////////////////////////////////////////
BOOL CHelpDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("CHelpDlg::OnHelpInfo iCtrlId = %x hItemHandle = %x dwContextId = %x\n", pHelpInfo->iCtrlId, pHelpInfo->hItemHandle, pHelpInfo->dwContextId);
	
	if (pHelpInfo->iContextType == HELPINFO_WINDOW) {
		AfxGetApp()->WinHelp(pHelpInfo->dwContextId, HELP_CONTEXTPOPUP);
	}

	return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//	Name:	OnHelpButton
//	Desc:	Handler for button with ID = IDHELP. You can place a button in your
//			dialog with this ID, and this will handle the routing to standard
//			Help for your dialog.
/////////////////////////////////////////////////////////////////////////////
void CHelpDlg::OnHelpButton() 
{
	// TODO: Add your control notification handler code here
	OnCommandHelp(0, 0);
}
