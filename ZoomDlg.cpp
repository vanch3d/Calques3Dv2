// ZoomDlg.cpp : implementation file
//

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
