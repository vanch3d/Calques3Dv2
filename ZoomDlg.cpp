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
// ZoomDlg.cpp: implementation of the CZoomDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"

#include "Calques3DDoc.h"

#include "ViewUniv.h"

#include "ZoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog

CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomDlg)
	m_nZoom = 0;
	//}}AFX_DATA_INIT
}


void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDlg)
	DDX_Control(pDX, IDC_ZOOM_VALUE, m_cZoomValue);
	DDX_Control(pDX, IDC_ZOOM_SLIDER, m_cZoom);
	DDX_Slider(pDX, IDC_ZOOM_SLIDER, m_nZoom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
	//{{AFX_MSG_MAP(CZoomDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ZOOM_SLIDER, OnReleasedCaptureSlider)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg message handlers

BOOL CZoomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nb = m_font.CreatePointFont (100, _T ("MS Sans Serif"));

	m_cZoom.SetFont(&m_font);
	m_cZoom.SetRange(0,25);
	m_cZoom.SetTicFreq(1);

	int rr = 25+5*m_nZoom;
	CString mstr;
	mstr.Format(_T("%d %%"),rr);
	m_cZoomValue.SetWindowText(mstr);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CZoomDlg::OnOK()
{
	CDialog::OnOK();
}

void CZoomDlg::OnCancel()
{
	UpdateData(TRUE);
	CDialog::OnCancel();
}

void CZoomDlg::OnReleasedCaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////
/// Method CZoomDlg::OnHScroll
///
/// \todo Replace the direct access to CViewUniv by Registred Message (see CSliderToolbar for example).
//////////////////////////////////////////////////////////////////////
void CZoomDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nb = -1;
	if (pScrollBar) 
		nb = pScrollBar->GetScrollPos();
	nb = m_cZoom.GetPos();
	int rr = 25+5*nb;
	CString mstr;
	mstr.Format(_T("%d %%"),rr);
	m_cZoomValue.SetWindowText(mstr);
	if (m_pParentWnd)
	{
		m_pParentWnd->SendMessage(WM_UPDATEZOOM_MANUAL,rr);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CZoomDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
