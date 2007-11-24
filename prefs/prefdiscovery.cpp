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
// CPrefDiscovery dialog
IMPLEMENT_DYNCREATE(CPrefDiscovery, CBCGPPropertyPage)


CPrefDiscovery::CPrefDiscovery()
	: CBCGPPropertyPage(CPrefDiscovery::IDD)
{
	//{{AFX_DATA_INIT(CPrefDiscovery)
	m_bActivate = TRUE;
	m_strPackage = TPref::TParamGeo.strPackagePath;
	m_strTranslator = TPref::TParamGeo.strTranslatorPath;
	//}}AFX_DATA_INIT

}

CPrefDiscovery::~CPrefDiscovery()
{

}



void CPrefDiscovery::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefDiscovery)
	DDX_Control(pDX, IDC_DISCOV_ACTIVATE, m_wndActivate);
	DDX_Control(pDX, IDC_DISCOV_TRANSLT, m_wndTranslator);
	DDX_Control(pDX, IDC_DISCOV_PACKAGE, m_wndPackage);
	DDX_Check(pDX, IDC_DISCOV_ACTIVATE, m_bActivate);
	DDX_Text(pDX, IDC_DISCOV_PACKAGE, m_strPackage);
	DDX_Text(pDX, IDC_DISCOV_TRANSLT, m_strTranslator);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		TPref::TParamGeo.strPackagePath = m_strPackage;
		TPref::TParamGeo.strTranslatorPath = m_strTranslator;
		BOOL test = (m_strPackage.IsEmpty() || m_strTranslator.IsEmpty());
		TPref::TParamGeo.bIsActivated = !test;
	}
	else
	{
		m_wndActivate.EnableWindow(FALSE);
		ActivateWidgets(m_bActivate);
	}
}


BEGIN_MESSAGE_MAP(CPrefDiscovery, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefDiscovery)
	ON_BN_CLICKED(IDC_DISCOV_ACTIVATE, OnActivateParamGeo)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefDiscovery message handlers

BOOL CPrefDiscovery::OnInitDialog() 
{
	CBCGPPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	ActivateWidgets(m_bActivate);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrefDiscovery::ActivateWidgets(BOOL act)
{
	static char BASED_CODE szFilter[] = "Maple Command Line File (*.mpl)|*.mpl||";

	m_wndPackage.EnableWindow(m_bActivate);
	m_wndTranslator.EnableWindow(m_bActivate);
	if (m_bActivate)
	{
		m_wndPackage.EnableFileBrowseButton (_T("*.txt"),szFilter);
		m_wndTranslator.EnableFileBrowseButton (_T("*.txt"),szFilter);
	}
	else
	{
		m_wndPackage.EnableBrowseButton(FALSE);
		m_wndTranslator.EnableBrowseButton(FALSE);
	}
	

}

void CPrefDiscovery::OnActivateParamGeo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
		ActivateWidgets(m_bActivate);
}

void CPrefDiscovery::OnDestroy() 
{
	UpdateData();
	CBCGPPropertyPage::OnDestroy();
}
