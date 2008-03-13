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
// WarningDialog.cpp: implementation of the CWarningDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "WarningDialog.h"
#include "MainFrm.h"
#include "Prefs\Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog dialog

BEGIN_MESSAGE_MAP(CWarningDialog, CDialog)
	//{{AFX_MSG_MAP(CWarningDialog)
	ON_WM_CLOSE()
	//ON_WM_CTLCOLOR()
	//ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/// @param pParent	A pointer to the parent window
/////////////////////////////////////////////////////////////////////////////
CWarningDialog::CWarningDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarningDialog)
	m_bDontAsk = FALSE;
	m_strMessage = _T("");
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
	m_nMessage = WARNING_MATHPAD;
}

/////////////////////////////////////////////////////////////////////////////
/// Initialisation of the dialog
/////////////////////////////////////////////////////////////////////////////
BOOL CWarningDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndIcon.m_cImage.SetImageSize(CSize(32,32));
	m_wndIcon.m_cImage.SetTransparentColor(RGB(99,140,231));
	m_wndIcon.m_cImage.Load(IDB_MSG_WARNING);
	m_wndIcon.SetImage(0);
	
	CFont* fnt = m_wndTitle.GetFont();
	LOGFONT fft;
	fnt->GetLogFont(&fft);
	fft.lfWeight = FW_BOLD;
	CFont fnt2;
	fnt2.CreateFontIndirect(&fft);
	m_wndTitle.SetFont(&fnt2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
/// Data validation and exchange
/// @param pDX	A pointer to the data exchange object.
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarningDialog)
	DDX_Control(pDX, IDC_MESSAGE_TITLE, m_wndTitle);
	DDX_Control(pDX, IDC_MESSAGE_IMG, m_wndIcon);
	DDX_Check(pDX, IDC_MESSAGE_CHECK, m_bDontAsk);
	DDX_Text(pDX, IDC_MESSAGE_STRING, m_strMessage);
	DDX_Text(pDX, IDC_MESSAGE_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
/// Launch the dialog in modeless/modal fashion
/// @param msg	The unique identifier of the warning message to be handled
/// @see CWarningDialog::TWarning
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::DoModeless(TWarning msg)
{
	m_nMessage = msg;

	CString temp;
	temp.LoadString(IDS_WARNING_TITLE);
	int nb = temp.Find(_T("\n"),0);
	m_strTitle= temp.Left(nb);
	m_strMessage = temp.Right(temp.GetLength()-nb-1);

// 	Create(CWarningDialog::IDD);
// 	CenterWindow();
// 	ShowWindow(SW_SHOW);
// 	SetActiveWindow();
	DoModal();
}

/////////////////////////////////////////////////////////////////////////////
/// Interpret the user's action on the dialog
/// @param action	The identifier of the action (IDOK or IDCANCEL)
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::PerformMessage(UINT action)
{
	if (m_nMessage==WARNING_MATHPAD)
	{
		if (action==IDOK)
		{
			CMainFrame *pMFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
			if (!pMFrame) return;
			CMDIChildWnd*pChild = pMFrame->MDIGetActive();
			if (!pChild) return;
			CDocument* pDoc = pChild->GetActiveDocument();
			pMFrame->LaunchView(pDoc,ID_VIEW_ANALYTIC);
			if (m_bDontAsk) TPref::TMathPad.nShowView = 1;
		}
		else
		{
			if (m_bDontAsk) TPref::TMathPad.nShowView = 2;
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CWarningDialog message handlers

/////////////////////////////////////////////////////////////////////////////
/// Called when the dialog is about to be closed
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::OnClose() 
{
	CDialog::OnClose();
	PerformMessage(IDOK);
// 	delete this;
}

/////////////////////////////////////////////////////////////////////////////
/// Called when the user is canceling the dialog
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::OnCancel() 
{
	CDialog::OnCancel();
	PerformMessage(IDCANCEL);
// 	delete this;
}

/////////////////////////////////////////////////////////////////////////////
/// Called when the user is validating the dialog
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::OnOK() 
{
	CDialog::OnOK();
	PerformMessage(IDOK);
// 	delete this;
}

/////////////////////////////////////////////////////////////////////////////
/// Customized drawing of the controls
/// @deprecated The warning is now displayed as a standard dialog box.
/////////////////////////////////////////////////////////////////////////////
HBRUSH CWarningDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
// 	if (pWnd->GetDlgCtrlID() == IDD_MESSAGE_STRING)
// 		return (HBRUSH)GetStockObject(WHITE_BRUSH);
	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
/// Customized drawing of the controls
/// @deprecated The warning is now displayed as a standard dialog box.
/////////////////////////////////////////////////////////////////////////////
void CWarningDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Get paint area for the big static control
	CWnd* pStatic = GetDlgItem(IDC_MESSAGE_IMG);
	CRect rect;
	pStatic->GetWindowRect(&rect);
	ScreenToClient(&rect);

	// Paint the background white.
	CBrush brush;
	brush.CreateStockObject(WHITE_BRUSH);
	dc.FillRect(rect, &brush);

	// Load bitmap and get dimensions of the bitmap
	CBitmap bmp;
	bmp.LoadBitmap(IDB_MSG_WARNING);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// Draw bitmap in top corner and validate only top portion of window
	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dc);
	dcTmp.SelectObject(&bmp);
	rect.bottom = bmpInfo.bmHeight + rect.top;
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
		&dcTmp, 0, 0, SRCCOPY);

	// Draw out "Did you know..." message next to the bitmap
	CString strMessage;
	strMessage.LoadString(IDS_WARNING_TITLE);
	rect.left += bmpInfo.bmWidth;
	dc.DrawText(strMessage, rect, DT_VCENTER | DT_SINGLELINE);

	// Do not call CDialog::OnPaint() for painting messages
}

