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
// AboutInfoDlg.cpp: implementation of the CAboutInfoPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"

#include "AboutInfoDlg.h"
#include "ProjectRCVersion.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include <winver.h>

/////////////////////////////////////////////////////////////////////////////
// CAboutInfoPage dialog


CAboutInfoPage::CAboutInfoPage()
	: CPropertyPage(CAboutInfoPage::IDD)
{
	//{{AFX_DATA_INIT(CAboutInfoPage)
	//}}AFX_DATA_INIT
}


void CAboutInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutInfoPage)
	DDX_Control(pDX, IDC_SUPPORT, m_cSupport);
	DDX_Control(pDX, IDC_URL, m_cURL);
	DDX_Control(pDX, IDC_MAIL, m_cMail);
	DDX_Control(pDX, IDC_VERSION, m_cVersion);
	DDX_Control(pDX, IDC_INFO, m_cInfo);
	DDX_Control(pDX, IDC_DEBUG, m_cDebug);
	DDX_Control(pDX, IDC_COPYRIGHT, m_cCopyright);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutInfoPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutInfoPage)
//	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutInfoPage message handlers

BOOL CAboutInfoPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LPSTR prodName = 0, prodVersion = 0, copyright = 0, debug = 0;

	// Process the VERSIONINFO.
	CProjectRCVersion applVersion;

	// Get the product name and product version strings.
	if (applVersion.GetProductName(prodName) && applVersion.GetProductVersion(prodVersion)) {
		  // IDC_VERSION is the product name and version number, the initial value of IDC_VERSION is
		  // the word Version (in whatever language) product name VERSION product version.
		  char    buffer[255];
		  char    versionName[128];

		  buffer[0] = '\0';
		  versionName[0] = '\0';

		  m_cVersion.GetWindowText(versionName, sizeof(versionName));
		  wsprintf(buffer, "%s %s", versionName, prodVersion);

		  m_cVersion.SetWindowText(buffer);
	 }

	 //Get the legal copyright string.
	 if (applVersion.GetCopyright(copyright))
		  m_cCopyright.SetWindowText(copyright);

	 // Only get the SpecialBuild text if the VERSIONINFO resource is there.
	 if (applVersion.GetDebug(debug))
		  m_cDebug.SetWindowText(debug);

	m_cInfo.SetWindowText(_T(
		"This program is free software; you can redistribute it and/or "
		"modify it under the terms of the GNU General Public License "
		"as published by the Free Software Foundation; either version 2 "
		"of the License, or (at your option) any later version."
		"\r\n"
		"This program is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
		"GNU General Public License for more details."
// 		"\r\n\r\n"
// 		"You should have received a copy of the GNU General Public License "
// 		"along with this program; if not, write to the Free Software "
// 		"Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA"
		));

 	m_cMail.SetURLPrefix (_T("mailto:"));
	m_cMail.SetURL (_T("n.vanlabeke@scre.ac.uk"));
	m_cMail.SizeToContent ();
	m_cMail.SetTooltip (_T("Send mail to author"));
	m_cMail.m_bDrawFocus = FALSE;

 	m_cURL.SetURLPrefix (_T("http://"));
	m_cURL.SetURL (_T("www.calques3d.org"));
	m_cURL.SetTooltip (_T("http://www.calques3d.org"));
	m_cURL.SizeToContent ();
	m_cURL.m_bDrawFocus = FALSE;

	m_cSupport.SetURLPrefix (_T("http://"));
	m_cSupport.SetURL (_T("www.calques3d.org/donation.html"));
	m_cSupport.SetTooltip (_T("Donate online"));
	m_cSupport.SizeToContent (TRUE,TRUE);
	m_cSupport.m_bDrawFocus = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
// CAboutCalquePage dialog


CAboutCalquePage::CAboutCalquePage()
	: CPropertyPage(CAboutCalquePage::IDD)
{
	//{{AFX_DATA_INIT(CAboutCalquePage)
	//}}AFX_DATA_INIT
}


void CAboutCalquePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutCalquePage)
	DDX_Control(pDX, IDC_ABOUTCALQUE, m_wndIcon);
//	DDX_Control(pDX, IDC_BUTTON1, m_wndff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutCalquePage, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutCalquePage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutCalquePage message handlers

BOOL CAboutCalquePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//m_wndff.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
	//	m_wndff.SetWindowText (_T(""));
	//m_wndff.m_bTransparent = TRUE;
	//m_wndff.m_bHighlightChecked=FALSE;
//m_wndff.m_bGrayDisabled=FALSE;
//m_wndff.m_bResponseOnButtonDown=FALSE;
//m_wndff.m_bDrawFocus=FALSE;
	//m_wndff.SizeToContent ();
	//CBCGPButton::EnableWinXPTheme (FALSE);
//	m_wndff.EnableWindow(FALSE);
//	m_wndff.SetImage(IDB_SPLASH,0,IDB_SPLASH);

	m_wndIcon.m_cImage.SetImageSize(CSize(297,298));
	m_wndIcon.m_cImage.SetTransparentColor(RGB(255,0,0));
	m_wndIcon.m_cImage.Load(IDB_SPLASH);
//m_wndff.ShowWindow(FALSE);
	m_wndIcon.SetImage(0);

//	m_cImages.Load(IDB_SPLASH);
//	m_cImages.SetTransparentColor(RGB(192,192,192));
//	m_cImages.SetImageSize(CSize(297,298));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutCalquePage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	CWnd * pWnd = GetDlgItem(IDC_ABOUTCALQUE);
	if (pWnd && pWnd->GetSafeHwnd())
		pWnd->SetWindowPos (NULL, 0, 0, cx, cy,
			/*SWP_NOMOVE |*/ SWP_NOACTIVATE | SWP_NOZORDER);

	
}

/////////////////////////////////////////////////////////////////////////////
// CAboutCreditPage dialog

CAboutCreditPage::CAboutCreditPage()
	: CPropertyPage(CAboutCreditPage::IDD)
{
	//{{AFX_DATA_INIT(CAboutCreditPage)
	//}}AFX_DATA_INIT
}


void CAboutCreditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutCreditPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutCreditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutCreditPage)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutCreditPage message handlers

BOOL CAboutCreditPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	srand((unsigned)time(NULL));
	
	// initialize credits control...

	// Content
	CString s;
	s = "<font color='255,255,255' face='verdana' size='11' align='center'>";
	s += "<font size='32' style='b' color='51,51,153'>Calques 3D</font><br>";
	s += "<font color='51,51,153'>version 2.1</font><p><p>";
	s += " <font style='bu' align='left'>The authors:<br></font><vspace size='5'>";
	s += "    <font style='b' color='1,1,1' align='left'><a href='http://www.w3c.org' color='51,51,153'>Nicolas VAN LABEKE</a><br></font>";
	s += "            <font color='1,1,1' align='left'>ESRC CREDIT<br></font>";
	s += "            <font color='1,1,1' align='left'>School of Psychology<br></font>";
	s += "            <font color='1,1,1' align='left'>University Park<br></font>";
	s += "            <font color='1,1,1' align='left'>NOTTINGHAM NG7 2RD<br></font>";
	s += "            <font color='1,1,1' align='left'>UK<p></font>";
	s += "    <font style='b' color='1,1,1' align='left'>Philippe BERNAT<br></font>";
	s += "    <font style='b' color='1,1,1' align='left'>Josette MORINET-LAMBERT<br></font>";
	s += "            <font color='1,1,1' align='left'>Equipe Informatique et Formation<br></font>";
	s += "            <font color='1,1,1' align='left'>LORIA/CNRS - Université Henri Poincaré<br></font>";
	s += "            <font color='1,1,1' align='left'>Bâtiment LORIA - BP 239<br></font>";
	s += "            <font color='1,1,1' align='left'>F-54506 Vandoeuvre les Nancy cedex<br></font>";
	s += "            <font color='1,1,1' align='left'>France<p><p></font>";
	s += " <font style='bu' align='left'>Acknowledgements:<br></font><vspace size='5'>";
	s += "  <font color='1,1,1' align='left'>The authors would like to thank the teachers and <br>  didacticians who, as part of a research group, <br>  collaborated to the definition of the pedagogical <br>  objectives and content of Calques 3D :<br></font>";
	s += "<font color='1,1,1' align='left' style='b'>";
	s += "         - Jean-Claude DEMOLY<br>";
	s += "         - Christian GINET<br>";
	s += "         - Jean-Pierre GIORGI<br>";     
	s += "         - Norbert GRESS<br>";
	s += "         - Françoise JEAN <br>";
	s += "         - Philippe LOMBARD<br>";
	s += "         - Christine MANCIAUX<br>";        
	s += "         - Marie-Hélène MUNIER<br>";
	s += "         - Bernard PARZYSZ<br>";            
	s += "         - Bernard PIERROT<br>";
	s += "         - Serge REMY<br>";                
	s += "         - Michèle THIRY<br>";
	s += "         - Agnès VOLPI<p></font>";
	s += "  <font color='1,1,1' align='left'>This project was initially supported by a grant from <br>  the <font style='b'><a href='http://www.w3c.org' color='51,51,153' style='u'>Conseil Régional de Lorraine</a></font>.<p><p>";
//	s += "  <font style='bu' align='left'>Font:<br></font><font color='0,200,100' size='14' face='Comic Sans MS'>Comic Sans MS</font><br><font color='0,200,100' size='14' face='Courier New'>Courier New</font><br><font color='0,200,100' size='14' face='Tahoma'>Tahoma</font><br>";
//	s += "  <font style='bu' align='left'>Size:<br></font><font color='150,100,255'><font size='42'>W</font><font size='36'>H</font><font size='28'>O</font><font size='18'>O</font><font size='14'>O</font><font size='18'>O</font><font size='28'>O</font><font size='36'>O</font><font size='42'>W</font></font><br>";
//	s += "  <font style='bu' align='left'>Styles:<br></font><font color='40,120,240' size='14'><font style='b'>bold</font>  <font style='i'>italic</font>  <font style='u'>underline</font>  <font style='s'>strikeout</font></font><br>";
//	s += "  <font style='bu' align='left'>Background:<br></font><font color='255,255,255' size='14' style='b'><font background='0,0,255'>Blue</font>  <font background='255,0,0'>Red</font>  <font background='210,150,40'>Brown</font>  <font background='0,0,255' color='none'>Ghost Font</font></font><br>";
//	s += "  <font style='bu' align='left'>Alignment:<br></font><font color='255,150,40' size='14' style='b'><font align='right'>right <br></font><font align='center'>center<br></font><font align='left'> left<br></font></font>";
//	s += "  <font style='bu' align='left'>Vertical Alignment:<br></font><font color='0,220,160' size='12' style='b'><font valign='top'>top</font><font valign='middle'>middle</font><font valign='bottom'>bottom</font><font size='48'>BIG</font></font><br>";
//	s += "  <font style='bu' align='left'>Resource Bitmap:<br></font><font color='30,70,255' size='12' style='b'><img src='#%d'><font color='255,0,0'>    red </font><font valign='bottom'>bo</font><font valign='middle'>rd</font><font valign='top'>er </font><img src='#%d' border='4' color='255,0,0'></font><br>";
//	s += "  <font style='bu' align='left'>Links:<br></font><font color='100,160,220' size='14' style='b'><a href='#Hello World!#%d' color='100,220,160'>Click Me!</a><br><a href='notepad.exe' color='100,220,160' style='bui'>notepad</a><br><a href='http://www.w3c.org' color='100,220,160' face='Courier New' size='16'>www.w3c.org</a><br><a href='mailto:devix@devix.cjb.net?SUBJECT=Hello World!! ;)' color='100,220,160' face='Comic Sans MS' size='18'>devix@devix.cjb.net</a><br><font size='36' color='255,200,0' style='u'><a href='#\nDo or do not. There is no try.\n\n\t\t-Yoda#%d' color='none' background='255,200,0' style='-uis'>Click Here</a></font><vspace size='4'><font valign='top' size='12' style='-b'>image link:  </font><a href='#%d#%d' color='255,200,0'><img src='#%d' border='2' color='none'></a></font><vspace size='80'>";
//	s += "<font size='28' face='Comic Sans MS' style='ui'>CCreditsCtrl </font><p><font size='18' style='u' color='255,200,0'>About</font><p>Coded in January 2001<br>by <font style='b'>Marc Richarme</font><vspace size='5'><font align='left' size='11'><hspace size='30'><font style='i'>mail:   </font><font style='ub'><a href='mailto:devix@devix.cjb.net?SUBJECT=Hello World!' color='255,200,0'>devix@devix.cjb.net</a></font><br><hspace size='30'><font style='i'>web:   </font><font style='ub'><a href='http://devix.cjb.net' color='255,200,0'>http://devix.cjb.net</a></font><p></font><font size='18' style='u' color='255,200,0'>Credits</font><p>Thanks to <font style='ub'><a href='http://www.codeproject.com' color='255,200,0'>The Code Project</a></font> for being<br> the coolest VC++/MFC site out there!<br></font><vspace size='80'>";
	s += "</font><vspace size='80'>";

	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_CREDITS), 
											_T("CREDITS"));
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(IDR_CREDITS), _T("CREDITS"));

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);

	if (hGlobal != NULL)
	{
		s = _T("");
		LPCSTR pData = (LPCSTR)LockResource(hGlobal);
		CString mstr(pData);
		mstr.Replace(_T("\r\n"),_T(""));
		mstr.Replace(_T("\t"),_T("       "));

		s = mstr;

		UnlockResource(hGlobal);
		FreeResource(hGlobal);
	}




	// Content Data
	m_wndCredits.FormatDataString(s,IDB_MENU_LOGO,IDB_MENU_LOGO,(long)0/*CCreditsDlgDlg::TestFunction*/,(long)0/*CCreditsDlgDlg::TestFunction*/,
		(long)0/*this*/,(long)0/*CCreditsDlgDlg::TestFunction2*/,IDB_MENU_LOGO);
	
	//m_wndCredits.SetDataString(s);
	// use our own function for background drawing
	//m_wndCredits.m_pBackgroundPaint = CCreditsDlgDlg::DrawCreditsBackground;
	//m_wndCredits.m_dwBackgroundPaintLParam = TRUE;

	// as our background is going to be black, use black as default transparent color
	m_wndCredits.m_crInternalTransparentColor = RGB(0,0,0);
	
	// now, we're ready to begin... create the window

//	int nOldSpeed = m_wndCredits.m_nTimerSpeed;
     m_wndCredits.m_nTimerSpeed = 10;

	 CRect mrect(0,0,200,200);

//	 CWnd *pWnd = GetDlgItem(IDC_PLACEHOLDER);
	 CWnd *pWnd2 = GetDlgItem(250);

	m_wndCredits.Create(WS_EX_CLIENTEDGE,WS_VISIBLE|WS_CHILD,IDC_PLACEHOLDER,this,0,IDC_DRAG);
	//m_wndCredits.Create(WS_EX_CLIENTEDGE,WS_VISIBLE|WS_CHILD,mrect,this);
//m_wndCredits.m_nTimerSpeed = nOldSpeed;
	// for details on what more can be customized, RTFM.

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CAboutCreditPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO: Add your specialized creation code here
	return 0;
}

void CAboutCreditPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	if (m_wndCredits.GetSafeHwnd())
		m_wndCredits.SetWindowPos (NULL, 0, 0, cx, cy,
			/*SWP_NOMOVE | */SWP_NOACTIVATE | SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg

IMPLEMENT_DYNAMIC(CAboutDlg, CPropertySheet)

CAboutDlg::CAboutDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	//m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CAboutDlg::CAboutDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CAboutDlg::~CAboutDlg()
{
}


BEGIN_MESSAGE_MAP(CAboutDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

//#define _countof(array) (sizeof(array)/sizeof(array[0]))
//AFX_STATIC_DATA int _afxPropSheetButtons[] = { IDOK};//, IDCANCEL, ID_APPLY_NOW, IDHELP };

BOOL CAboutDlg::OnInitDialog() 
{
	//CPropertySheet::OnInitDialog();
	// change tab style if scrolling tabs desired (stacked tabs are default)
	if (!m_bStacked)
	{
		HWND hWndTab = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
		if (hWndTab != NULL)
			CWnd::ModifyStyle(hWndTab, TCS_MULTILINE, TCS_SINGLELINE, 0);
	}

	// resize the tab control so the layout is less restrictive
	HWND hWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
	ASSERT(hWnd != NULL);
	CRect rectOld;
	::GetWindowRect(hWnd, &rectOld);
	ScreenToClient(rectOld);
	CRect rectNew(0, 0, 0, 0);
	::MapDialogRect(m_hWnd, &rectNew);
	if (rectNew.bottom < rectOld.bottom)
	{
		// move tab control
		int cyDiff = rectOld.Height() - rectNew.bottom;
		::SetWindowPos(hWnd, NULL, 0, 0, rectOld.Width(), rectNew.bottom,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			// move buttons by similar amount
	//	for (int i = 0; i < _countof(_afxPropSheetButtons); i++)
		{
			//hWnd = ::GetDlgItem(m_hWnd, _afxPropSheetButtons[i]);
			hWnd = ::GetDlgItem(m_hWnd, IDOK);
			if (hWnd != NULL)
			{
				::GetWindowRect(hWnd, &rectOld);
				ScreenToClient(&rectOld);
				::SetWindowPos(hWnd, NULL,
					rectOld.left, rectOld.top - cyDiff,
					0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
			// resize property sheet itself similarly
		GetWindowRect(&rectOld);
		SetWindowPos(NULL, 0, 0, rectOld.Width(), rectOld.Height() - cyDiff,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL bResult = (BOOL)Default();

//	if (m_bModeless && !IsWizard())
	{
		// layout property sheet so button area is not accounted for
		CRect rectWnd;
		GetWindowRect(rectWnd);
		CRect rectButton;
		HWND hWnd = ::GetDlgItem(m_hWnd, IDOK);
		ASSERT(hWnd != NULL);
		::GetWindowRect(hWnd, rectButton);
		SetWindowPos(NULL, 0, 0,
			rectWnd.Width(), rectButton.top - rectWnd.top,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		// remove standard buttons for modeless dialogs
	//	for (int i = 0; i < _countof(_afxPropSheetButtons); i++)
		{
			//HWND hWnd = ::GetDlgItem(m_hWnd, _afxPropSheetButtons[i]);
			HWND hWnd = ::GetDlgItem(m_hWnd, IDOK);
			if (hWnd != NULL)
			{
				::ShowWindow(hWnd, SW_HIDE);
				::EnableWindow(hWnd, FALSE);
			}
		}
	}

	// center the property sheet relative to the parent window
	if (!(GetStyle() & WS_CHILD))
		CenterWindow();

 /*   CWnd *pWnd = GetDlgItem( IDOK );
	if (pWnd->GetSafeHwnd())
		pWnd->DestroyWindow();
    pWnd = GetDlgItem( IDHELP );
	if (pWnd->GetSafeHwnd())
		pWnd->DestroyWindow();
    pWnd = GetDlgItem( ID_APPLY_NOW );
	if (pWnd->GetSafeHwnd())
		pWnd->DestroyWindow();
    pWnd = GetDlgItem( IDCANCEL );
	if (pWnd->GetSafeHwnd())
		pWnd->DestroyWindow();*/

	// TODO: Add extra initialization here
	m_imageTab.Create(IDB_WORKSPACE, 16, 0, RGB(255, 0,255));
	
	CTabCtrl *pTab = GetTabControl();
	pTab->SetImageList( &m_imageTab );

	int nbC = pTab->GetItemCount();
	
	int nIdx[] = {2,0,1,0,0,0,0,0};
	TC_ITEM tcItem;
	tcItem.mask = TCIF_IMAGE;
	for( int i = 0; i < nbC; i++ )
    {
		tcItem.iImage = nIdx[i];
		pTab->SetItem( i, &tcItem );
	}
	
	m_imageTab.Detach(); 


/*		
		
		ShowWindow( FALSE );
    pWnd = GetDlgItem( IDHELP );
	pWnd->ShowWindow( FALSE );
    pWnd = GetDlgItem( ID_APPLY_NOW );
	pWnd->ShowWindow( FALSE );
    pWnd = GetDlgItem( IDCANCEL );
	pWnd->ShowWindow( FALSE );
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
