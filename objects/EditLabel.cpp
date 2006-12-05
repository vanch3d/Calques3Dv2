// EditLabel.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "EditLabel.h"
#include "Text3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLabel

CEditLabel::CEditLabel(CWnd* pParent,CObject3D* pObj,CxObject3DSet* pSet)
{
	CString sInitText(_T(""));
	CRect rect(0,0,0,0);
	UINT nID = 15000;
	m_pObj = pObj;
	m_pSet = pSet;

	CText3D *pt = DYNAMIC_DOWNCAST(CText3D,pObj);
    
	m_Rect = pt->rActZone;  // For bizarre CE bug.
	m_InitRect = pt->rActZone;  // For bizarre CE bug.


    DWORD dwStyle = ES_LEFT; //ES_RIGHT, ES_CENTER;
    DWORD dwEditStyle = WS_BORDER|WS_CHILD|WS_VISIBLE| ES_AUTOHSCROLL | dwStyle;
	if (pt->IsMultiLine())
		dwEditStyle |= ES_MULTILINE;

    if (!Create(dwEditStyle, rect, pParent, nID)) return;
	SetFont(&(pt->mTextFont));
	SetWindowText(pt->GetText());

		//SetFocus();
	SetSel(0,-1);
    SendMessage(WM_CHAR, VK_LBUTTON);
}

CEditLabel::~CEditLabel()
{
}

BEGIN_MESSAGE_MAP(CEditLabel, CEdit)
	//{{AFX_MSG_MAP(CEditLabel)
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLabel message handlers

void CEditLabel::EndEdit()
{
    CString str;

    // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
    // that validates input can cause this to get called multiple times
    // causing assertions because the edit control goes away the first time.
    static BOOL bAlreadyEnding = FALSE;

    if(bAlreadyEnding)
        return;

    bAlreadyEnding = TRUE;
    GetWindowText(str);

    CWnd* pOwner = GetOwner();
    //if (pOwner)
    //    pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
	CText3D *pt = DYNAMIC_DOWNCAST(CText3D,m_pObj);
	CMathOp3D *pMath = DYNAMIC_DOWNCAST(CMathOp3D,m_pObj);
	if (pt)
	{
		pt->SetText(str);
		if (pMath)
			pMath->OnUpdateVariables(m_pSet);
		if (pOwner)
			pOwner->Invalidate();
	}


    // Close this window (PostNcDestroy will delete this)
    if (IsWindow(GetSafeHwnd()))
        SendMessage(WM_CLOSE, 0, 0);
    bAlreadyEnding = FALSE;
}


// As soon as this edit loses focus, kill it.
void CEditLabel::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
    EndEdit();	
}

BOOL CEditLabel::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CEditLabel::PostNcDestroy() 
{
	//m_pPopup.DestroyMenu();
	CEdit::PostNcDestroy();
	delete this;	
}

UINT CEditLabel::OnGetDlgCode() 
{
	//return CEdit::OnGetDlgCode();
	return DLGC_WANTALLKEYS;

}

void CEditLabel::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
    CString str;
    GetWindowText( str );

    // add some extra buffer
    str += _T("  ");
    
    CWindowDC dc(this);
    CRect rName;
    rName.SetRectEmpty();
    CFont *pFontDC = dc.SelectObject(GetFont());
    dc.DrawText(str,rName,DT_CALCRECT|DT_NOCLIP   );

    CSize size = dc.GetTextExtent( str );
    dc.SelectObject( pFontDC );
       
    // Get client rect
    CRect ParentRect;
    GetParent()->GetClientRect( &ParentRect );
    
    // Check whether control needs to be resized
    // and whether there is space to grow
    if (rName != m_Rect)
    {
        //if( size.cx + m_Rect.left < ParentRect.right )
            //m_Rect.right = m_Rect.left + size.cx;
        //else
        //    m_Rect.right = ParentRect.right;
		CSize ms;
		ms.cx = __max(rName.Size().cx,m_InitRect.Size().cx);
		ms.cy = __max(rName.Size().cy,m_InitRect.Size().cy);
		m_Rect = CRect(m_Rect.TopLeft(),ms+CSize(size.cy+5,size.cy+5));
        MoveWindow( &m_Rect );
    }
}

BOOL CEditLabel::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEdit::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CEditLabel::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    SetFocus();

	CMenu	m_pPopup;
	VERIFY(m_pPopup.LoadMenu (IDR_CONTEXT_MENU));
	
	CMenu * menu = m_pPopup.GetSubMenu(0);
	int nCount = menu->GetMenuItemCount();

    BOOL bReadOnly = GetStyle() & ES_READONLY;
	UINT flags = (CanUndo() && !bReadOnly) ? MF_ENABLED : MF_GRAYED;
	menu->EnableMenuItem(ID_EDIT_UNDO,flags);

    DWORD sel = GetSel();
    flags = (LOWORD(sel) == HIWORD(sel)) ? MF_GRAYED : MF_ENABLED;
	menu->EnableMenuItem(ID_EDIT_COPY,flags);

    flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : MF_ENABLED;
	menu->EnableMenuItem(ID_EDIT_CUT,flags);
	menu->EnableMenuItem(ID_EDIT_CLEAR,flags);

	flags = IsClipboardFormatAvailable(CF_TEXT) && !bReadOnly ? MF_ENABLED : MF_GRAYED;
	menu->EnableMenuItem(ID_EDIT_PASTE,flags);

	int len = GetWindowTextLength();
    flags = (!len || (LOWORD(sel) == 0 && HIWORD(sel) == len)) ? MF_GRAYED : MF_ENABLED;
	menu->EnableMenuItem(ID_EDIT_SELECT_ALL,flags);

	menu->EnableMenuItem(nCount-1,MF_GRAYED|MF_BYPOSITION);
	/*CMenu *pVars = menu->GetSubMenu(nCount-1);
	if (pVars)
	{
		UINT varID = pVars->GetMenuItemID(0);
		pVars->AppendMenu(MF_STRING,varID,"ddfssdfsfd");
	}*/

	if (point.x == -1 || point.y == -1)
	{
		CRect rc;
		GetClientRect(&rc);
		point = rc.CenterPoint();
		ClientToScreen(&point);
	}

    menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);

//	CMenu * menu = m_pPopup.GetSubMenu(0);
//	ASSERT(menu != NULL);
	//theApp.GetContextMenuManager()->ShowPopupMenu (menu->GetSafeHmenu(), point.x, point.y,this,FALSE,FALSE);
//	UINT msg = theApp.GetContextMenuManager()->TrackPopupMenu (menu->GetSafeHmenu(), point.x, point.y,AfxGetMainWnd());
//	if (msg)
//		SendMessage(WM_COMMAND,msg);
}

void CEditLabel::OnEditCut() 
{
	// TODO: Add your command handler code here
	
}

void CEditLabel::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
	
}

BOOL CEditLabel::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	BOOL bRet = FALSE;
	// TODO: Add your specialized code here and/or call the base class
	switch (LOWORD(wParam))
    {
		case ID_EDIT_UNDO:
			bRet = SendMessage(WM_UNDO);
			break;
		case ID_EDIT_CUT:
			bRet = SendMessage(WM_CUT);
			break;
		case ID_EDIT_COPY:
			bRet = SendMessage(WM_COPY);
			break;
		case ID_EDIT_PASTE:
			bRet = SendMessage(WM_PASTE);
			break;
		case ID_EDIT_CLEAR:
			bRet = SendMessage(WM_CLEAR);
			break;
		case ID_EDIT_SELECT_ALL:
			bRet = SendMessage (EM_SETSEL, 0, -1);
			break;
		default:
			return CEdit::OnCommand(wParam, lParam);
    }
    SendMessage(WM_CHAR, VK_LBUTTON);
	return bRet;
}
