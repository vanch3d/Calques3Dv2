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
// PrefDiscovery.cpp: implementation of the CPrefDiscovery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefDiscovery.h"

#include "Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefCAS 

CPrefCAS::CPrefCAS()
{
	m_strName = _T("");
	m_bFound = FALSE;
	m_strVersion = _T("");
	m_strPath = _T("");
	m_strRuntime = _T("");
	m_strScript = _T("");
}


/////////////////////////////////////////////////////////////////////////////
// CPrefDiscovery dialog
IMPLEMENT_DYNCREATE(CPrefDiscovery, CPropertyPage)


CPrefDiscovery::CPrefDiscovery()
	: CPropertyPage(CPrefDiscovery::IDD)
{
	//{{AFX_DATA_INIT(CPrefDiscovery)
	m_nCAS = -1;
	m_strRuntime = TPref::Verif.strRunTime;
	m_strScript = TPref::Verif.strConvert;
	m_strVersion = _T("");
	//}}AFX_DATA_INIT

	// Add CoCoa
	CPrefCAS *pCAS = new CPrefCAS;
	pCAS->m_strName = _T("CoCoa");

	m_cCASList.Add(pCAS);

	// Add Mathematica
//	pCAS = new CPrefCAS;
//	pCAS->m_strName = _T("Mathematica");

//	m_cCASList.Add(pCAS);
}

CPrefDiscovery::~CPrefDiscovery()
{
	for (int i=0;i<m_cCASList.GetSize();i++)
	{
		delete m_cCASList.GetAt(i);
	}
	m_cCASList.RemoveAll();
}



void CPrefDiscovery::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefDiscovery)
	DDX_Control(pDX, IDC_DISCOV_CAS, m_wndCAS);
	DDX_CBIndex(pDX, IDC_DISCOV_CAS, m_nCAS);
	DDX_Text(pDX, IDC_DISCOV_RUNTIME, m_strRuntime);
	DDX_Text(pDX, IDC_DISCOV_SCRIPT, m_strScript);
	DDX_Text(pDX, IDC_DISCOV_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		TPref::Verif.strRunTime = m_strRuntime;
		TPref::Verif.strConvert = m_strScript;

	}
}


BEGIN_MESSAGE_MAP(CPrefDiscovery, CPropertyPage)
	//{{AFX_MSG_MAP(CPrefDiscovery)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DISCOV_BROWSE, OnBrowseScript)
	ON_BN_CLICKED(IDC_DISCOV_BROWSECMD, OnBrowseCmdLine)
	ON_CBN_SELCHANGE(IDC_DISCOV_CAS, OnChangeCAS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefDiscovery message handlers

BOOL CPrefDiscovery::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	int nb = m_cCASList.GetSize();

	CPrefCAS *pCAS = m_cCASList.GetAt(0);
	if (pCAS)
	{
		CBCGPRegistry mReg(FALSE,TRUE);
		CString strReg("Software\\CoCoA");
		if (mReg.Open (strReg))
		{
			CStringArray SubKeys;
			if (mReg.ReadSubKeys(SubKeys))
			{
				m_strVersion = SubKeys.GetAt(SubKeys.GetSize()-1);
				pCAS->m_strVersion = m_strVersion;
				UpdateData(FALSE);
				CBCGPRegistry mReg2(FALSE,TRUE);
				CString strReg("Software\\CoCoA");
				strReg += _T("\\") + m_strVersion + _T("\\Environment");
				if (mReg2.Open (strReg))
				{
					CString strVal;
					if (mReg2.Read(_T("COCOA_LIBRARY"),strVal))
					{
						char strs[255];
						GetShortPathName(strVal,strs,255);

						pCAS->m_strPath = strs;
						pCAS->m_bFound = TRUE;
						m_nCAS = 0;
					}
				}
			}
		}
	}

/*	if (nb>=2)
	{
		CPrefCAS *pCAS = m_cCASList.GetAt(1);
		if (pCAS)
		{

			CBCGRegistry mMReg(TRUE,TRUE);
			CString strReg = CString("Software\\Wolfram Research\\Mathematica");
			if (mMReg.Open (strReg))
			{
				CStringArray SubKeys;
				if (mMReg.ReadSubKeys(SubKeys))
				{
					m_strVersion = SubKeys.GetAt(SubKeys.GetSize()-1);
					pCAS->m_strVersion = m_strVersion;
					UpdateData(FALSE);

					CBCGRegistry mReg2(TRUE,TRUE);
					CString strReg("Software\\Wolfram Research\\Mathematica");
					strReg += _T("\\") + m_strVersion;
					if (mReg2.Open (strReg))
					{
						CString strVal;
						if (mReg2.Read(_T("InstallDir"),strVal))
						{
							pCAS->m_strPath = strVal;
							pCAS->m_bFound = TRUE;
						}
					}
				}
			}
		}
	}
*/
	m_wndCAS.ResetContent();
	for (int i=0;i<m_cCASList.GetSize();i++)
	{
		CPrefCAS *pCAS = m_cCASList.GetAt(i);
		if (!pCAS) continue;
		if (!pCAS->m_bFound) continue;

		m_wndCAS.AddString(pCAS->m_strName);
	}
	m_wndCAS.SetCurSel(m_nCAS);
	m_wndCAS.EnableWindow(FALSE);

	UINT nCID[] = {	IDC_STATIC_CMD,IDC_STATIC_SCRIPT,IDC_DISCOV_RUNTIME,
					IDC_DISCOV_SCRIPT,IDC_DISCOV_BROWSECMD,IDC_DISCOV_BROWSE};

	for (i=0;i<6;i++)
	{
		CWnd *pWnd = GetDlgItem(nCID[i]);
		if (!pWnd) continue;
		pWnd->EnableWindow(m_nCAS!=-1);
		pWnd->ShowWindow((m_nCAS!=-1)?SW_SHOW:SW_HIDE);
	}
	CWnd *pWnd = GetDlgItem(IDC_STATIC_NONE);
	if (pWnd)
	{
		pWnd->EnableWindow(m_nCAS==-1);
		pWnd->ShowWindow((m_nCAS==-1)?SW_SHOW:SW_HIDE);
	}

	


	OnChangeCAS() ;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefDiscovery::OnDestroy() 
{
	UpdateData();
	CPropertyPage::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CPrefDiscovery::OnBrowseCmdLine() 
{
	// TODO: Add your control notification handler code here
	CString mstr;
	static char BASED_CODE szFilter[] = "Batch File (*.bat)||";
 
	CFileDialog mdlg(TRUE,"bat","cocoa.bat",
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_NOLONGNAMES,
			szFilter,
			this);

	//DWORD mFlag = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	//mdlg.m_ofn.Flags |= mFlag;

	CPrefCAS * pCAS = m_cCASList.GetAt(0);
	if (pCAS)
	{
		mdlg.m_ofn.lpstrInitialDir = pCAS->m_strPath;
	}
	int ret = mdlg.DoModal();
	if (ret==IDOK)
	{
		char strshort[255];
		//m_strRuntime = mdlg.m_ofn.lpstrFile;

		GetShortPathName(mdlg.m_ofn.lpstrFile,strshort,255);
		m_strRuntime = strshort;
		m_strRuntime += _T(" -q");
		UpdateData(FALSE);
	}	
	
}

void CPrefDiscovery::OnBrowseScript() 
{
	// TODO: Add your control notification handler code here
	CString mstr;
	static char BASED_CODE szFilter[] = "CoCoa Source (*.coc)||";
 
	CFileDialog mdlg(TRUE,"coc","*.coc",
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
			szFilter,
			this);

	CPrefCAS * pCAS = m_cCASList.GetAt(0);
	int ret = mdlg.DoModal();
	if (ret==IDOK)
	{
		m_strScript = mdlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}	
	
}

void CPrefDiscovery::OnChangeCAS() 
{
	// TODO: Add your control notification handler code here
	int nb = m_wndCAS.GetCurSel();
	
	m_strRuntime = _T("");
	//m_strScript = _T("");
	m_strVersion = _T("");

	if (nb>=0)
	{
		CPrefCAS * pCAS = m_cCASList.GetAt(nb);
		if (!pCAS) return;
		if (!pCAS->m_bFound) return;

		m_strVersion = pCAS->m_strVersion;
		m_nCAS = nb;

		m_strRuntime = pCAS->m_strPath + _T("\\cocoa.bat -q");
		//m_strRuntime = 
	}
	UpdateData(FALSE);
}
