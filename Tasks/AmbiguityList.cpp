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

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::CAmbiguityList()
{
}


/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::CAmbiguityList(const char far* title, CWnd* parent)
			:  CWnd(), m_nCount(01),m_wndParent(parent)
{
  m_wndPopup.CreatePopupMenu();
  if (title) {
	 m_wndPopup.AppendMenu(MF_GRAYED, 1, title);
	 m_wndPopup.AppendMenu(MF_SEPARATOR);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::CAmbiguityList(UINT sid, CWnd* parent)
			:	CWnd(), m_nCount(01),m_wndParent(parent)
{
	m_wndPopup.CreatePopupMenu();
	CString mstr(_T(""));
	mstr.LoadString(sid);
	if (!sid || (mstr == _T("")))	return;
	m_wndPopup.AppendMenu(MF_GRAYED, 1, mstr);
	m_wndPopup.AppendMenu(MF_SEPARATOR);
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
CAmbiguityList::~CAmbiguityList()
{
	m_wndPopup.DestroyMenu();
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAmbiguityList, CWnd)
	//{{AFX_MSG_MAP(CAmbiguityList)
	ON_WM_MENUSELECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAmbiguityList message handlers

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
BOOL CAmbiguityList::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	//return CWnd::OnCommand(wParam, lParam);
	m_nIndex = wParam;
	return 0;

}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
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
	CWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
	
}


/////////////////////////////////////////////////////////////////////////////
// CAmbiguityList Operations

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
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

/*
/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
int CAmbiguityList::AddString(CBitmap& bmp,BOOL dis,BOOL checked)
{
	uint flag = (dis) ? MF_GRAYED : MF_ENABLED;
	if (checked) flag |= MF_CHECKED;
	::AppendMenu(m_wndPopup, flag | MF_BITMAP, ++Count, (const char *)HBITMAP(bmp));
	return m_nCount;
}

/////////////////////////////////////////////////////////////////////////////
/// CAmbiguityList
/////////////////////////////////////////////////////////////////////////////
int CAmbiguityList::AddString(HMENU hpopup,CString str,BOOL dis,BOOL checked)
{
	uint flag = (dis) ? MF_GRAYED : MF_ENABLED;
	if (checked) flag |= MF_CHECKED;
//	::AppendMenu(m_wndPopup, flag | MF_BITMAP, ++Count, (const char *)HBITMAP(bmp));
	::AppendMenu(m_wndPopup, flag | MF_POPUP, (UINT) hpopup,str);

	return m_nCount;
}
*/

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
	Create(NULL,"Ambiguity",WS_CHILD,mrect,m_wndParent,0);
	m_nIndex = 0;

	// Call the MenuManager's TrackPopupMenu to get the selected item
	m_nIndex = theApp.GetContextMenuManager()->TrackPopupMenu(m_wndPopup, loc.x, loc.y, this);

	// Substract 2 from the index (to take into account title and separator)
	return (m_nIndex>=2) ? m_nIndex-2 : -1;
}