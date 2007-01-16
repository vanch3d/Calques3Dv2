// AmbiguityList.cpp : implementation file
//

#include "stdafx.h"
#include "..\MainFrm.h"
#include "..\calques3d.h"

#include "AmbiguityList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CAmbiguityList, CBCGPToolBar)
	//{{AFX_MSG_MAP(CAmbiguityList)
	ON_WM_MENUSELECT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/// @param title	The string containing the title of the menu
/// @param parent	A pointer to the window owning the menu
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::CAmbiguityList(const char far* title, CWnd* parent)

{
	m_wndPopup.CreatePopupMenu();
	if (title)
	{
		m_wndPopup.AppendMenu(MF_GRAYED, 1, title);
		m_wndPopup.AppendMenu(MF_SEPARATOR);
	}
	m_nCount = 1;
	m_wndParent = parent;
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/// @param sid		The identifier of the string resource containing the title of the menu
/// @param parent	A pointer to the window owning the menu
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::CAmbiguityList(UINT sid, CWnd* parent)
{
	m_wndPopup.CreatePopupMenu();
	CString mstr(_T(""));
	mstr.LoadString(sid);
	if (!sid || (mstr == _T("")))	return;
	m_wndPopup.AppendMenu(MF_GRAYED, 1, mstr);
	m_wndPopup.AppendMenu(MF_SEPARATOR);
	m_nCount = 1;
	m_wndParent = parent;
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::~CAmbiguityList()
{
	m_wndPopup.DestroyMenu();
	DestroyWindow();
}


/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/// @param n		The zero-based index of the menu item.
/// @param mstr		The string containing the contextual help.
/////////////////////////////////////////////////////////////////////////////
void CAmbiguityList::AddHelper(int n,CString mstr)
{
	m_cHelpString.SetAtGrow(n,mstr);
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
int CAmbiguityList::AddString(UINT sid,BOOL dis,BOOL checked)
{
	UINT flag = (dis) ? MF_GRAYED : MF_ENABLED;
	if (checked) flag |= MF_CHECKED;
	CString mstr(_T(""));
	mstr.LoadString(sid);
	if (!sid || (mstr==_T("")))
		m_wndPopup.AppendMenu(MF_SEPARATOR, ++m_nCount);
	else
		m_wndPopup.AppendMenu(flag, ++m_nCount, mstr);
	return m_nCount;
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
int CAmbiguityList::AddString(CString str,BOOL dis,BOOL checked)
{
	UINT flag = (dis) ? MF_GRAYED : MF_ENABLED;
	if (checked) flag |= MF_CHECKED;
	m_wndPopup.AppendMenu(flag, ++m_nCount, str);
	return m_nCount;
}

/////////////////////////////////////////////////////////////////////////////
/// Execute the popup menu and get the index of the selected object (if any)
///
/// \warning The popup menu containing the ambiguous objects starts with a title AND a
/// separator, so the first object in the list starts at index 2. 
///
/// \todo There is a problem with the TrackPopupMenu's handling of status bar message: it assumes the index
/// to be the command ID, so look for such a resource ID. I don't think there is any way out of this issue.
///
/// \return		The zero-based index of the selected object, -1 if none
/////////////////////////////////////////////////////////////////////////////
int CAmbiguityList::Execute()
{
	CPoint loc;
	GetCursorPos(&loc);
	CRect mrect(0,0,0,0);
	m_nIndex = 0;
	Create (AfxGetMainWnd());
	SetRouteCommandsViaFrame (TRUE);
	// Call the MenuManager's TrackPopupMenu to get the selected item
	m_nIndex = theApp.GetContextMenuManager()->TrackPopupMenu(m_wndPopup, loc.x, loc.y, this);

	// Subtract 2 from the index (to take into account title and separator)
	return (m_nIndex>=2) ? m_nIndex-2 : -1;
}

void CAmbiguityList::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	CString mstr;
	if (!(nFlags & MF_GRAYED) && (nItemID<=m_cHelpString.GetSize()))
	{
		mstr = m_cHelpString.GetAt(nItemID); 
	}
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST (CMainFrame, theApp.m_pMainWnd);
	if (pMainFrame)
	{
		CWnd* pMessageBar = GetParentFrame()->GetMessageBar();
		if (pMessageBar) pMessageBar->SetWindowText(mstr);
	}
	CBCGPToolBar::OnMenuSelect(nItemID, nFlags, hSysMenu);
	
}

void CAmbiguityList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBCGPToolBar::OnMouseMove(nFlags, point);
}
