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
/// @file ParamGeoBar.cpp
/// Implementation of the classes used in defining the ParamGeo3D user interface.
///
/// $Date: 2007-11-10 17:55:11+00 $
/// $Revision: 1.0 $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "ParamGeoBar.h"
#include "MainFrm.h"
#include "Calques3DDoc.h"
#include "objects/Text3D.h"
#include "prefs/Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const UINT idList = 1;
const UINT idEdit = 2;
const UINT idButton1 = 3;
const UINT idButton2 = 4;
const UINT idProgress = 5;


CParamGeoEditCtrl::CParamGeoEditCtrl()
{
}

CParamGeoEditCtrl::~CParamGeoEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CParamGeoEditCtrl, CBCGPEditCtrl)
	//{{AFX_MSG_MAP(CCustomEditCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CParamGeoEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CParamGeoEditCtrl::OnGetTipText (CString& strTipString)
{
	CPoint point;
	::GetCursorPos (&point);
	ScreenToClient (&point);

	CString strText;
	BOOL bIsHyperlink = m_bEnableHyperlinkSupport && GetHyperlinkToolTip (strText);
	BOOL bIsHiddenTextFromPoint = !bIsHyperlink && m_bEnableOutlining && GetHiddenTextFromPoint (point, strText);
	BOOL bIsWordFromPoint = !bIsHyperlink && !bIsHiddenTextFromPoint && GetWordFromPoint (point, strText);

	if ((bIsHiddenTextFromPoint || bIsHyperlink) && strText == strTipString)
	{
		return TRUE;
	}
	else if (m_mTipString.Lookup(strTipString,strTipString))
	{
		return TRUE; 
	}
	else if (IsIntelliSenseEnabled() && !bIsWordFromPoint)
	{
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CControlsContainer

CControlsContainer::CControlsContainer() :
	m_nTextHeight (0)
{
#ifdef _C3D_PARAMGEO
    kv= NULL;  /* Maple kernel handle */
#endif
}

CControlsContainer::~CControlsContainer()
{
#ifdef _C3D_PARAMGEO
    kv= NULL;  /* Maple kernel handle */
#endif
}


BEGIN_MESSAGE_MAP(CControlsContainer, CWnd)
	//{{AFX_MSG_MAP(CControlsContainer)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
    ON_COMMAND(ID_PARAMGEO_EXEC, OnRunCommand)
    ON_COMMAND(ID_PARAMGEO_CLEAR, OnClearOutput)
	ON_UPDATE_COMMAND_UI(ID_PARAMGEO_EXEC, OnUpdateRunCommand)
	ON_UPDATE_COMMAND_UI(ID_PARAMGEO_CLEAR, OnUpdateClearOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CControlsContainer message handlers

void CControlsContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Layout ();
}

BOOL CControlsContainer::Create(const RECT& rect, CWnd* pParentWnd) 
{
	return CWnd::Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 
						rect, pParentWnd, 10);
}

BOOL CControlsContainer::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID (pDC);

	CRect rectClient;
	GetClientRect (rectClient);

	CBCGPVisualManager::GetInstance ()->OnFillTasksPaneBackground (pDC,rectClient);

	return TRUE;
}

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

int CControlsContainer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create controls:
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	if (!m_wndButton1.Create (_T("&Add"), BS_PUSHBUTTON, rectDummy, this, idButton1) ||
		!m_wndButton2.Create (_T("&Delete"), BS_PUSHBUTTON, rectDummy, this, idButton2) ||
		!m_wndProgress.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH,rectDummy, this, idProgress) ||
		!m_wndInputCtrl.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 2) ||
		!m_wndOutputCtrl.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("CControlsContainer: Unable to create controls\n");
		return -1;
	}
	m_wndInputCtrl.m_nLeftMarginWidth = 16;
	m_wndInputCtrl.SetReadOnly (FALSE);
	m_wndInputCtrl.m_nDlgCode = DLGC_WANTALLKEYS | DLGC_WANTMESSAGE;
	m_wndOutputCtrl.m_nLeftMarginWidth = 16;
	m_wndOutputCtrl.SetReadOnly (TRUE);
	m_wndOutputCtrl.m_nDlgCode = DLGC_WANTALLKEYS | DLGC_WANTMESSAGE;
	BOOL res = m_wndInputCtrl.LoadXMLSettings(_T("bcgpedit-maple.xml"));
	res = m_wndOutputCtrl.LoadXMLSettings(_T("bcgpedit-maple.xml"));

	m_wndToolBar.Create (this,dwDefaultToolbarStyle,idProgress);
	m_wndToolBar.LoadToolBar (IDR_PARAMGEOTOOL_TB, 0, 0, TRUE /* Locked bar */);
	m_wndToolBar.SetOwner (this);
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetBarStyle (
	//	m_wndToolBar.GetBarStyle () & 
//			~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetWindowText ("Test");


	m_wndProgress.SetRange(0,100);
	m_wndProgress.SetPos(50);
	//m_wndList.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

	// Set fonts:
	m_Font.CreateStockObject (DEFAULT_GUI_FONT);

	for (CWnd* pWndChild = GetWindow (GW_CHILD); pWndChild != NULL;
		pWndChild = pWndChild->GetNextWindow ())
	{
		ASSERT_VALID (pWndChild);
		pWndChild->SetFont (&m_Font, FALSE);
	}

	// Calculate text height:
	CClientDC dc (this);
	CFont* pOldFont = dc.SelectObject (&m_Font);
	ASSERT (pOldFont != NULL);

	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);

	m_nTextHeight = tm.tmHeight;

	dc.SelectObject (pOldFont);

	Layout ();
	return 0;
}

void CControlsContainer::Layout ()
{
	if (GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect (rectClient);

	//rectClient.DeflateRect (nMargin, nMargin);

	BOOL bShow = TRUE;

	if (rectClient.Width () < 20 ||
		rectClient.Height () < 20)
	{
		// Hide all controls to prevent a bad look:
		bShow = FALSE;
	}
	else
	{
		// Set input control:
		m_wndInputCtrl.SetWindowPos (NULL,
			rectClient.left, rectClient.top, rectClient.Width (), 6* m_nTextHeight,
			SWP_NOZORDER | SWP_NOACTIVATE);
		rectClient.top += 6* m_nTextHeight;


		// Set buttons:
		m_wndToolBar.SetWindowPos (NULL,
			rectClient.left, rectClient.top, 
			rectClient.Width (), 24,
			SWP_NOZORDER | SWP_NOACTIVATE);
		rectClient.top += 24;

		// Set list control:
		CRect rectList = rectClient;
		rectList.bottom -= 24 ;

		m_wndOutputCtrl.SetWindowPos (NULL,
			rectList.left, rectList.top, rectList.Width (), rectList.Height ()+1,
			SWP_NOZORDER | SWP_NOACTIVATE);

		CRect rectButtons = rectClient;
		rectButtons.top = rectButtons.bottom  - 22;

		m_wndStatusBar.SetWindowPos (NULL,
			rectButtons.left, rectButtons.top, rectButtons.Width (), rectButtons.Height () ,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

/*	for (CWnd* pWndChild = GetWindow (GW_CHILD); pWndChild != NULL;
		pWndChild = pWndChild->GetNextWindow ())
	{
		ASSERT_VALID (pWndChild);
		pWndChild->ShowWindow (bShow ? SW_SHOWNOACTIVATE : SW_HIDE);
	}*/
}

void CControlsContainer::textCallBack( void *data, int tag, char *output )
{
    printf("%s\n",output);
}

#ifdef _C3D_PARAMGEO
/* simple program to print a prompt, get input, evaluate it, 
   and display results 
*/
/* callback used for directing result output */
static void M_DECL textCallBack2( void *data, int tag, char *output )
{
    //printf("%s\n",output);
	CMainFrame *pFrame= DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	if (pFrame)
	{
		CString strType[]={
			"", // none
			"[DIAG]",	// MAPLE_TEXT_DIAG 1
			"[MISC]",	// MAPLE_TEXT_MISC 2
			"[OUTPUT]",	// MAPLE_TEXT_OUTPUT 3
			"[QUIT]",	// MAPLE_TEXT_QUIT 4
			"[WARN]",	// MAPLE_TEXT_WARNING 5
			"[ERROR]",	// MAPLE_TEXT_ERROR 6
			"[STATUS]",	// MAPLE_TEXT_STATUS 7
			"[PRETTY]",	// MAPLE_TEXT_PRETTY 8
			"[HELP]",	// MAPLE_TEXT_HELP 9
			"[DEBUG]"	// MAPLE_TEXT_DEBUG 10
		};
		CString str(output);

		CBCGPEditCtrl* pCtrl = &((CParamGeoBar*)pFrame->GetParamGeoBar())->m_wndContainer.m_wndOutputCtrl;

		pCtrl->EndOfText (FALSE);
		pCtrl->InsertText (str + "\n", pCtrl->GetCurOffset(), TRUE);
		pCtrl->Invalidate ();

		//((CParamGeoBar*)pFrame->GetParamGeoBar())->m_wndContainer.m_wndOutputCtrl.AddString(strType[tag] + " " + str);
	}


}

static void M_DECL errorCallBack( void *data, M_INT offset, char *msg)
{
	CString str(msg);
    AfxGetMainWnd()->MessageBox(str);
}

static void M_DECL statusCallBack( void *data, long kilobytesUsed,
                                       long kilobytesAlloc, double cpuTime )
{
	static long maxAlloc = 0;

	CString str;
	maxAlloc = max(kilobytesAlloc,maxAlloc);
	int percent = 0;
	if (maxAlloc!=0)
	{
		percent = 100.*kilobytesAlloc/maxAlloc;
	}
	str.Format("cputime:%f\nKB used: %d (%f)",cpuTime,kilobytesAlloc,(1.00*kilobytesAlloc)/maxAlloc);
    //AfxGetMainWnd()->MessageBox(str);
	CMainFrame *pFrame= DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	if (pFrame)
	{
		((CParamGeoBar*)pFrame->GetParamGeoBar())->m_wndContainer.m_wndProgress.SetPos(percent);
	}
}
#endif

void CControlsContainer::OnUpdateRunCommand(CCmdUI* pCmdUI)
{
	BOOL isEmpty = m_wndInputCtrl.GetText().IsEmpty();
	pCmdUI->Enable(!isEmpty);
}

void CControlsContainer::OnUpdateClearOutput(CCmdUI* pCmdUI)
{
	BOOL isEmpty = m_wndOutputCtrl.GetText().IsEmpty();
	pCmdUI->Enable(!isEmpty);
}

void CControlsContainer::OnClearOutput()
{
	m_wndOutputCtrl.EndOfText (FALSE);
	m_wndOutputCtrl.SetWindowText("");
	m_wndInputCtrl.EndOfText (FALSE);
	m_wndInputCtrl.SetWindowText("");

	m_wndOutputCtrl.Invalidate ();
	m_wndInputCtrl.Invalidate ();
}

void CControlsContainer::OnRunCommand()
{
#ifdef _C3D_PARAMGEO
	CString strInput = m_wndInputCtrl.GetText();
	if (strInput.IsEmpty()) return;

	
	m_wndOutputCtrl.EndOfText (FALSE);
	m_wndOutputCtrl.InsertText ("# " + strInput + "\n", m_wndOutputCtrl.GetCurOffset(), TRUE);
	ALGEB r = EvalMapleStatement(kv,strInput.GetBuffer(strInput.GetLength()));
	
	m_wndInputCtrl.SetWindowText (_T(""));
	m_wndInputCtrl.Invalidate ();
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CParamGeoBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CParamGeoBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PARAMGEO_START, OnParamGeoStart)
	ON_UPDATE_COMMAND_UI(ID_PARAMGEO_START, OnUpdateParamGeoStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CParamGeoBar::CParamGeoBar()
{

}


CParamGeoBar::~CParamGeoBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CParamGeoBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create toolbar:
	if (!m_wndToolBar.Create (this, dwDefaultToolbarStyle, AFX_IDW_TOOLBAR + 1) ||
		!m_wndToolBar.LoadToolBar(IDR_PARAMGEO_TB, 0, 0, TRUE /* Is locked */))
	{
		TRACE0("Failed to create toolbar\n");
		return -1; // fail to create
	}

	m_wndToolBar.ReplaceButton (ID_PARAMGEO_DOC, CParamGeoBarLabel (ID_PARAMGEO_DOC, _T("< none >")));
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
		
	m_wndToolBar.SetBarStyle (
		m_wndToolBar.GetBarStyle () & 
			~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner (this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame (FALSE);


	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!m_wndTabs.Create (CBCGPTabWnd::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}
	m_wndTabs.SetFlatFrame (FALSE, TRUE);

	// Create list windows.
	// TODO: create your own window here:
	const DWORD dwViewStyle =	
		LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	
	if (!m_wndSymbolicCtrl.Create (WS_VISIBLE | WS_CHILD, rectDummy, &m_wndTabs, 2))
	{
		TRACE0("Failed to create output view\n");
		return -1;      // fail to create
	}

	m_wndSymbolicCtrl.m_nLeftMarginWidth = 16;
	m_wndSymbolicCtrl.SetReadOnly (FALSE);
	m_wndSymbolicCtrl.EnableToolTips ();
	m_wndSymbolicCtrl.m_nDlgCode = DLGC_WANTALLKEYS | DLGC_WANTMESSAGE;
	BOOL res = m_wndSymbolicCtrl.LoadXMLSettings(_T("bcgpedit-maple.xml"));



	// Create tabs window:
	if (!m_wndContainer.Create (rectDummy, &m_wndTabs))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	// Attach list windows to tab:
	m_wndTabs.AddTab (&m_wndContainer, _T("Maple Command"), -1);
	m_wndTabs.AddTab (&m_wndSymbolicCtrl, _T("Exported Figure"), -1);

	return 0;
}

void CParamGeoBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout ();
}

void CParamGeoBar::AdjustLayout ()
{
	CRect rectClient;
	GetClientRect (rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout (FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos (NULL, 0, 0, 
								rectClient.Width (), cyTlb,
								SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndTabs.SetWindowPos (NULL, 0, cyTlb, rectClient.Width (), rectClient.Height () - cyTlb,
							SWP_NOACTIVATE | SWP_NOZORDER);
}
void CParamGeoToolBar::AdjustLayout ()
{
	CBCGPToolBar::AdjustLayout ();

	//((CParamGeoBar*) GetParent ())->AdjustLayout ();
}

#define GUID_LENGTH        39

void CParamGeoBar::OnUpdateParamGeoStart(CCmdUI* pCmdUI)
{
	BOOL bready = TPref::TParamGeo.bIsActivated && !TPref::TParamGeo.strTranslatorPath.IsEmpty()
					&& !TPref::TParamGeo.strPackagePath.IsEmpty();
	pCmdUI->Enable(bready);
}

void CParamGeoBar::OnParamGeoStart()
{

#ifdef _C3D_PARAMGEO

	m_wndSymbolicCtrl.m_mTipString.RemoveAll();
	CMainFrame *pFrame= DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	if (!pFrame) return;

	CMDIChildWnd*pChild = pFrame->MDIGetActive();
	if (!pChild) return;
		
	CCalques3DDoc *pDoc = DYNAMIC_DOWNCAST(CCalques3DDoc,pChild->GetActiveDocument());
	if (!pDoc) return;

	CString strSymb = pDoc->OnExportSymbolic(CObject3D::EXPORT_MAPLE);
	m_wndSymbolicCtrl.SetWindowText(strSymb);
	int nb = pDoc->m_cObjectSet.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D* pObj = pDoc->m_cObjectSet.GetAt(i);
		if (!pObj) continue;
		if (DYNAMIC_DOWNCAST(CText3D,pObj)) continue;
		CString mstr = pObj->GetObjectNameRedux();
		m_wndSymbolicCtrl.SetWordColor(mstr,RGB(255,0,0),-1,TRUE);
		m_wndSymbolicCtrl.m_mTipString.SetAt(mstr,pObj->GetObjectDef());
	}
	m_wndSymbolicCtrl.EndOfText ();
	m_wndSymbolicCtrl.Invalidate ();
	CBCGPToolbarButton*pBtn = m_wndToolBar.GetButton(1);
	if (pBtn) 
	{
		pBtn->m_strText = pDoc->GetTitle();
		m_wndToolBar.AdjustLayout ();
		m_wndToolBar.Invalidate(FALSE);
		m_wndToolBar.UpdateWindow();
	}

    CString strCommand("");
	char err[2048];  /* command input and error string buffers */

	MCallBackVectorDesc cb = {  
		textCallBack2, 
		errorCallBack,
		statusCallBack,
		0,   /* readLineCallBack not used */
		0,   /* redirectCallBack not used */
		0,   /* streamCallBack not used */
		0,   /* queryInterrupt not used */ 
		0    /* callBackCallBack not used */
	};
	ALGEB r, l;  /* Maple data-structures */
		
	/* initialize Maple */
	if( m_wndContainer.kv==NULL && (m_wndContainer.kv=StartMaple(NULL,NULL,&cb,NULL,NULL,err)) == NULL )
	{
		printf("Fatal error, %s\n",err);
		errorCallBack(NULL,0,err);
		return;
	}
	r = MapleKernelOptions(m_wndContainer.kv,"mapledir",NULL);

	m_wndContainer.m_wndOutputCtrl.EndOfText (FALSE);
	m_wndContainer.m_wndOutputCtrl.InsertText ("# Start Maple\n", m_wndContainer.m_wndOutputCtrl.GetCurOffset(), TRUE);
	CString maplepath(_T(""));
	if( IsMapleString(m_wndContainer.kv,r) )
	{
		maplepath = _T("Maple in \"") + CString(MapleToString(m_wndContainer.kv,r)) + _T("\"\n");
		m_wndContainer.m_wndOutputCtrl.InsertText (maplepath, m_wndContainer.m_wndOutputCtrl.GetCurOffset(), TRUE);
	}


	strCommand="restart;";
	r = EvalMapleStatement(m_wndContainer.kv,strCommand.GetBuffer(strCommand.GetLength()));
	m_wndContainer.m_wndOutputCtrl.InsertText ("# Load ParamGeo\n", m_wndContainer.m_wndOutputCtrl.GetCurOffset(), TRUE);
	CString path = TPref::TParamGeo.strPackagePath;
	path.Replace(_T("\\"),_T("\\\\"));
	strCommand="read(`"+ path + "`);";
	path = TPref::TParamGeo.strTranslatorPath;
	path.Replace(_T("\\"),_T("\\\\"));
 	strCommand+="read(`"+ path + "`);";
	r = EvalMapleStatement(m_wndContainer.kv,strCommand.GetBuffer(strCommand.GetLength()));
	m_wndContainer.m_wndOutputCtrl.InsertText ("# Load Figure\n", m_wndContainer.m_wndOutputCtrl.GetCurOffset(), TRUE);
 	strCommand=strSymb;
	r = EvalMapleStatement(m_wndContainer.kv,strCommand.GetBuffer(strCommand.GetLength()));
	m_wndContainer.m_wndOutputCtrl.InsertText ("# Ready for query ...\n", m_wndContainer.m_wndOutputCtrl.GetCurOffset(), TRUE);

	m_wndContainer.m_wndOutputCtrl.Invalidate ();
#endif
}


IMPLEMENT_SERIAL(CParamGeoBarLabel, CBCGPToolbarButton, 1)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParamGeoBarLabel::CParamGeoBarLabel (UINT uiID, LPCTSTR lpszText)
{
	if (lpszText != NULL)	
	{		
		m_strText = lpszText;	
	}	

	m_bText = TRUE;	
	m_nID = uiID;
}

CParamGeoBarLabel::~CParamGeoBarLabel()
{

}

void CParamGeoBarLabel::OnDraw (CDC* pDC, const CRect& rect, CBCGPToolBarImages* pImages, 
							  BOOL bHorz, BOOL bCustomizeMode, BOOL bHighlight,	BOOL bDrawBorder,					BOOL bGrayDisabledButtons)
{
	UINT nStyle = m_nStyle;	
	m_nStyle &= ~TBBS_DISABLED;   	
	CBCGPToolbarButton::OnDraw (pDC, rect, pImages, bHorz, bCustomizeMode, 
								bHighlight, bDrawBorder, bGrayDisabledButtons);   	
	m_nStyle = nStyle;
}

SIZE CParamGeoBarLabel::OnCalculateSize (CDC* pDC, const CSize& sizeDefault, BOOL bHorz)
{
	m_iImage = -1;
	return CBCGPToolbarButton::OnCalculateSize (pDC, sizeDefault, bHorz);
}