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
// SuppressDlg.cpp: implementation of the CSuppressDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\objects\object3d.h"

#include "SuppressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuppressDlg dialog


CSuppressDlg::CSuppressDlg(CWnd* pParent /*=NULL*/)
	: CHelpDlg(CSuppressDlg::IDD, pParent)
{
	m_bShwoList = FALSE;
	m_pList = NULL;
	//{{AFX_DATA_INIT(CSuppressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSuppressDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuppressDlg)
	DDX_Control(pDX, IDC_SUPP_DELETELIST, m_cDelList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSuppressDlg, CHelpDlg)
	//{{AFX_MSG_MAP(CSuppressDlg)
	ON_BN_CLICKED(IDC_SUPP_SHOWLIST, OnShowList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SUPP_DELETELIST, OnDeleteItemChanged)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_SUPP_DELETELIST, OnDeleteItemChanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuppressDlg message handlers

void CSuppressDlg::OnShowList() 
{
	// TODO: Add your control notification handler code here
	m_bShwoList = !m_bShwoList;
	ShowDependList(m_bShwoList);

}

void CSuppressDlg::OnDeleteItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CSuppressDlg::OnDeleteItemChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

BOOL CSuppressDlg::OnInitDialog() 
{
	CHelpDlg::OnInitDialog();

	int nb = m_ImageList.Create(IDB_OBJECTS_ICONS,16,0,RGB(192,192,192));
	m_cDelList.SetImageList(&m_ImageList,LVSIL_NORMAL);

	if (m_pList)
	{
		int nb = m_pList->GetSize();
		for (int i=0;i<nb;i++)
		{
			CObject3D *pDep = m_pList->GetAt(i);
			if (!pDep) continue;

			CString mstr = pDep->GetObjectHelp();

			LVITEM pItem;
			pItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
			pItem.pszText = mstr.GetBuffer(mstr.GetLength());
			pItem.lParam = (LPARAM) pDep;
			pItem.iItem = i;
			pItem.iSubItem = 0;
			pItem.iImage = pDep->GetNameID() - IDS_NAME_OBJECT;
			m_cDelList.InsertItem(&pItem);
		}
	}
	ShowDependList(m_bShwoList);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSuppressDlg::ShowDependList(BOOL bShow)
{
	CRect theR,iR;
	GetWindowRect(theR);
	
	CWnd *pWnd = GetDlgItem(IDC_SUPP_DELETELIST);
	pWnd->GetWindowRect(iR);
	MoveWindow(theR.left,theR.top,
				(bShow) ? iR.right-theR.left+7 : iR.left-theR.left-2,
				theR.bottom-theR.top,TRUE);
}
