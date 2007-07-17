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
// PrefHistory.cpp: implementation file.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "PrefHistory.h"
#include "Prefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefHistory property page

IMPLEMENT_DYNCREATE(CPrefHistory, CBCGPPropertyPage)

CPrefHistory::CPrefHistory() : CBCGPPropertyPage(CPrefHistory::IDD)
{
	//{{AFX_DATA_INIT(CPrefHistory)
	m_bShowHidden = TPref::THistory.bShowSelectHidden;
	//}}AFX_DATA_INIT
}

CPrefHistory::~CPrefHistory()
{
}

void CPrefHistory::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefHistory)
	DDX_Check(pDX, IDC_PHISTO_SHOWSEL, m_bShowHidden);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		TPref::THistory.bShowSelectHidden = m_bShowHidden;
	}
}


BEGIN_MESSAGE_MAP(CPrefHistory, CBCGPPropertyPage)
	//{{AFX_MSG_MAP(CPrefHistory)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefHistory message handlers
