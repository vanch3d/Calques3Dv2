// PrefSession.cpp : implementation file
//

#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefSession.h"

#include "Prefs.h"
#include "..\BCGXMLSettings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefSession dialog
IMPLEMENT_DYNCREATE(CPrefSession, CBCGPPropertyPage)


CPrefSession::CPrefSession()
	: CBCGPPropertyPage(CPrefSession::IDD)
{
	//{{AFX_DATA_INIT(CPrefSession)
	m_strStart = TPref::strProfile;
	//}}AFX_DATA_INIT
}


void CPrefSession::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefSession)
	DDX_Control(pDX, IDC_PSESS_DESCR, m_cSessionDescr);
	DDX_Control(pDX, IDC_PSESS_LIST, m_cStartList);
	DDX_LBString(pDX, IDC_PSESS_LIST, m_strStart);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		int nSel = m_cStartList.FindString(-1,m_strStart);
		if (nSel == LB_ERR) return;
		DWORD nData = m_cStartList.GetItemData(nSel);
		if (nData == -1) 
		{
			TPref::strProfile.Empty();
			return;
		}
		CString mstr = m_lPathFile.GetAt(nData);
		TPref::strProfile = mstr;
	}
}


BEGIN_MESSAGE_MAP(CPrefSession, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefSession)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PSESS_SAVEAS, OnSaveAs)
	ON_LBN_SELCHANGE(IDC_PSESS_LIST, OnSelchangeReglist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefSession message handlers

void CPrefSession::OnDestroy() 
{
	UpdateData();
	CBCGPPropertyPage::OnDestroy();
}

void CPrefSession::OnSaveAs() 
{
	// TODO: Add your control notification handler code here
	CString strProfileName;

	CSaveProfileDlg dlg (NULL);
	if (dlg.DoModal () == IDOK)
	{
		strProfileName = dlg.GetProfileName ();
	}
	
	if (!strProfileName.IsEmpty ())
	{
		strProfileName+= _T(".cfg");

		//AfxGetMainWnd()
		//SaveState(0, pFrame);

		CWnd *pWn = AfxGetMainWnd();

		CBCGPMDIFrameWnd *pObj= DYNAMIC_DOWNCAST(CBCGPMDIFrameWnd,pWn);
		CBCGPRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGXMLSettings));
		theApp.SaveState (pObj);

		// Save settings:
		CBCGXMLSettings::WriteXMLToFile (FALSE, strProfileName);
		//CBCGRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGRegistry));
		CBCGPRegistrySP::SetRuntimeClass (RUNTIME_CLASS (CBCGPRegistry));

		//theApp.CleanState();
		//exit(EXIT_SUCCESS);
		//theApp.LoadState(pObj);
		//CString strCommandLine;
		//CString strSettingsPath = theApp.GetRegSectionPath ();
		//// to be on the safe side and do not export the entire registry :)
		//if (strSettingsPath.Find (CCalques3DApp::g_strRegistryBase) >= 0)
		//{
		//	int nb = strSettingsPath.GetLength();
		//	strSettingsPath.Delete(nb-1);
		//	//strCommandLine.Format (_T ("regedit.exe /s /E \"%s.cfg\" \"HKEY_CURRENT_USER\\%s\""), 
		//	//						strProfileName, strSettingsPath);
		//
		//	//theApp.ImportExportRegFile (strCommandLine);
		//}
		UpdateProfileList();
	}
}

void CPrefSession::UpdateProfileList()
{
	m_lPathFile.RemoveAll();
	m_cStartList.ResetContent();

	TCHAR szCurrDir [_MAX_PATH + 1];
	GetCurrentDirectory (_MAX_PATH, szCurrDir);

	CFileFind finder;
    BOOL bWorking = finder.FindFile("*.cfg");

	CString mStrDef;
	mStrDef.LoadString(ID_SESSION_DEF);

	int nIdx = m_cStartList.AddString(mStrDef);
	m_cStartList.SetItemData(nIdx,(DWORD)-1);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		CString mstr;
		//mstr.Format(_T("%s\t%s"),finder.GetFileTitle(),finder.GetFilePath());
		mstr.Format(_T("%s"),finder.GetFilePath());
		int nIdx = m_cStartList.AddString (mstr);//finder.GetFileTitle());
		int nAdd = m_lPathFile.Add(finder.GetFilePath());
		m_cStartList.SetItemData(nIdx,(DWORD)nAdd);
	}
	int nSel = m_cStartList.FindString(-1,m_strStart);
	if (nSel != LB_ERR) 
		m_cStartList.SetCurSel(nSel);
	else 
		m_cStartList.SetCurSel(0);

	CString mStrDesc;
	mStrDesc.LoadString(ID_SESSION_DESCR);
	m_cSessionDescr.SetWindowText(mStrDesc);
}


BOOL CPrefSession::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateProfileList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPrefSession::OnSelchangeReglist() 
{
	// TODO: Add your control notification handler code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CSaveProfileDlg dialog


CSaveProfileDlg::CSaveProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveProfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveProfileDlg)
	m_strProfileName = _T("");
	m_strCurSelProfileName = _T("");
	//}}AFX_DATA_INIT
}


void CSaveProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveProfileDlg)
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDC_PROFILE_LIST, m_lstProfiles);
	DDX_Text(pDX, IDC_PROFILE_NAME, m_strProfileName);
	DDX_LBString(pDX, IDC_PROFILE_LIST, m_strCurSelProfileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveProfileDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveProfileDlg)
	ON_LBN_SELCHANGE(IDC_PROFILE_LIST, OnSelchangeListProfiles)
	ON_EN_UPDATE(IDC_PROFILE_NAME, OnUpdateEditProfileName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveProfileDlg message handlers

void CSaveProfileDlg::OnSelchangeListProfiles() 
{
	UpdateData ();
	m_strProfileName = m_strCurSelProfileName;
	UpdateData (FALSE);
	OnUpdateEditProfileName ();
}

void CSaveProfileDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL CSaveProfileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	TCHAR szCurrDir [_MAX_PATH + 1];
	GetCurrentDirectory (_MAX_PATH, szCurrDir);

	CFileFind finder;
    BOOL bWorking = finder.FindFile("*.cfg");
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		m_lstProfiles.AddString (finder.GetFileTitle());
	}

	OnUpdateEditProfileName();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CSaveProfileDlg::GetProfileName () const
{
	TCHAR szCurrDir [_MAX_PATH + 1];
	GetCurrentDirectory (_MAX_PATH, szCurrDir);
	CString strFullName;
	strFullName.Format (_T ("%s\\%s"), szCurrDir, m_strProfileName);
	return strFullName;
}

void CSaveProfileDlg::OnUpdateEditProfileName() 
{
	UpdateData ();
	m_wndOK.EnableWindow (!m_strProfileName.IsEmpty ());
}

