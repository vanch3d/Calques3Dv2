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
// MacroInfoDlg.cpp: implementation of the CMacroInfoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "MacroInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMacroInfoDlg dialog


CMacroInfoDlg::CMacroInfoDlg(CWnd* pParent /*=NULL*/)
	: CHelpDlg(CMacroInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMacroInfoDlg)
	m_strDef = _T("");
	m_strHelp = _T("");
	m_strName = _T("");
	m_strPath = _T("");
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
}


void CMacroInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMacroInfoDlg)
	DDX_Control(pDX, IDC_DMACRO_SAVEAS, m_wndSaveAs);
	DDX_Text(pDX, IDC_DMACRO_DEF, m_strDef);
	DDX_Text(pDX, IDC_DMACRO_HELP, m_strHelp);
	DDX_Text(pDX, IDC_DMACRO_NAME, m_strName);
	DDX_Text(pDX, IDC_DMACRO_PATH, m_strPath);
	//}}AFX_DATA_MAP
	if (!pDX->m_bSaveAndValidate)
	{
		CWnd *pWnd = GetDlgItem(IDOK);
		if (pWnd)
			pWnd->EnableWindow(m_strPath != _T(""));
	}
}


BEGIN_MESSAGE_MAP(CMacroInfoDlg, CHelpDlg)
	//{{AFX_MSG_MAP(CMacroInfoDlg)
	ON_BN_CLICKED(IDC_DMACRO_SAVEAS, OnSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacroInfoDlg message handlers

void CMacroInfoDlg::OnSaveAs() 
{
	CString mstr;
	static char BASED_CODE szFilter[] = "Macro-construction (*.m3d)||";
 
	CFileDialog mdlg(FALSE,"m3d","*.m3d",
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			szFilter,
			this);

	DWORD mFlag = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	mdlg.m_ofn.Flags |= mFlag;
	int ret = mdlg.DoModal();
	if (ret==IDOK)
	{
		UpdateData();
		m_strPath = mdlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}	
}

BOOL CMacroInfoDlg::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndSaveAs.EnableWindow(!m_bModify);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
