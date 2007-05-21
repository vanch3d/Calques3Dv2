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
// VerifyDlg.cpp: implementation of the CVerifyDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "VerifyDlg.h"

#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog


CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
    : CHelpDlg(CVerifyDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVerifyDlg)
    m_strProp = _T("");
    m_strResult = _T("");
    //}}AFX_DATA_INIT
    m_pObjFree = NULL;
    m_bIsCE=FALSE;
    m_pView = NULL;
	m_bShowCE = FALSE;

    m_pPropImageList.Create(IDR_VERIFY_TB,16,0,RGB(192,192,192));
    m_pObjImageList.Create(IDB_OBJECTS_ICONS,16,0,RGB(192,192,192));
    m_pBtnImageList.Create(IDB_BTNICON,15,0,RGB(192,192,192));
}


void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
    CHelpDlg::DoDataExchange(pDX);
    if (!pDX->m_bSaveAndValidate)
    {
        CMenu menu;
        CString mstr;

        //GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);
        menu.LoadMenu(IDR_POPUP_VERIFY);
        menu.GetMenuString(m_nTask,mstr,MF_BYCOMMAND);
        menu.Detach();
        m_strProp = mstr;
    }
    //{{AFX_DATA_MAP(CVerifyDlg)
    //DDX_Control(pDX, IDC_VERIFY_TARGETS, m_cTargetList);
    DDX_Control(pDX, IDC_VERIFY_COUNTER, m_cShowCE);
    DDX_Control(pDX, IDC_VERIFY_PROP_ICON, m_cPropIcon);
    DDX_Text(pDX, IDC_VERIFY_PROP, m_strProp);
    DDX_Text(pDX, IDC_VERIFY_RESULT, m_strResult);
    //}}AFX_DATA_MAP
    DDX_GridControl(pDX, IDC_VERIFY_TARGET, m_cColorlist);
    if (!pDX->m_bSaveAndValidate)
    {
        m_cPropIcon.SetIcon(m_pPropImageList.ExtractIcon(m_nTask-ID_EXPLORATION_VERIFY_PAR));
        m_cShowCE.EnableWindow(m_bShowCE);
    }
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CHelpDlg)
    //{{AFX_MSG_MAP(CVerifyDlg)
    ON_BN_CLICKED(IDC_VERIFY_COUNTER, OnShowExample)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg message handlers
void CVerifyDlg::OnShowExample()
{
    // TODO: Add your control notification handler code here

    if (!m_pObjFree) return;

    CxObject3DSet mList;
    m_pObjFree->GetDependList(&mList);

    CVector4 ptt;
    if (!m_bIsCE)
        ptt = m_vNewPt;
    else
        ptt = m_vOldPt;

    CVector4 dpt = ptt - m_pObjFree->Concept_pt ;
    CVector4 dpt2 = m_pObjFree->Concept_pt ;
    dpt = dpt * 0.1;

    CVector4 ptt2 = ptt;

    for (int i=0;i<10;i++)
    {
        ptt = dpt2 + (dpt*(i*1.));
        if (i==9)
            ptt = ptt2;


        m_pObjFree->Concept_pt = ptt;
        for (int k=0;k<mList.GetSize();k++)
        {
            CObject3D *pObj = mList.GetAt(k);
            if (!pObj) continue;
            pObj->CalculConceptuel();
        }

        for (int l=0;l<1000000;l++);

        if (i!=9)
        {
            m_pView->Invalidate();
            m_pView->UpdateWindow();
        }
    }

    if (m_pView)
    {
        m_pView->GetDocument()->UpdateAllViews(m_pView,WM_UPDATEOBJ_MOV,m_pObjFree);
        m_pView->Invalidate();
        m_pView->UpdateWindow();
    }

    m_bIsCE = !m_bIsCE;
}

BOOL CVerifyDlg::OnInitDialog()
{
    CHelpDlg::OnInitDialog();

    // TODO: Add extra initialization here
    m_cShowCE.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
    m_cShowCE.SetImage(m_pBtnImageList.ExtractIcon(0));
    m_cShowCE.m_bRighImage = TRUE;
    m_cShowCE.m_bDrawFocus = FALSE;


    //m_cTargetList.SetImageList(&m_pObjImageList,LVSIL_SMALL);

    m_cColorlist.SetGridBkColor(::GetSysColor(COLOR_3DFACE));
    m_cColorlist.SetTextBkColor(::GetSysColor(COLOR_3DFACE));
    m_cColorlist.EnableSelection(FALSE);
    m_cColorlist.SetRowResize(FALSE);
    m_cColorlist.SetColumnResize(FALSE);
    m_cColorlist.SetImageList(&m_pObjImageList);
    m_cColorlist.SetFixedColumnCount(0);
    m_cColorlist.SetRowCount(0);
    m_cColorlist.SetEditable(FALSE);
    m_cColorlist.SetColumnCount(1);
    m_cColorlist.SetFixedRowCount(0);
    m_cColorlist.SetGridLines(GVL_NONE);

    m_cColorlist.AutoSizeColumns();
    CRect rect;
    m_cColorlist.GetClientRect(rect);
    m_cColorlist.SetColumnWidth(0,rect.Width());


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CVerifyDlg::AddTarget(CObject3D *pObj)
{
    if (!pObj) return;

    CString mstr = pObj->GetObjectHelp();

/*  LVITEM pItem;
    pItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    pItem.pszText = mstr.GetBuffer(mstr.GetLength());
    pItem.lParam = (LPARAM) pObj;
    pItem.iItem = m_cTargetList.GetItemCount();
    pItem.iSubItem = 0;
    pItem.iImage = pObj->GetNameID() - IDS_NAME_OBJECT;
    m_cTargetList.InsertItem(&pItem);
*/

    int nb = m_cColorlist.GetRowCount();

    GV_ITEM Item;
    Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_IMAGE;

    Item.row = nb;
    Item.col = 0;
    Item.nFormat = DT_LEFT |DT_SINGLELINE| DT_VCENTER|DT_END_ELLIPSIS ; //DT_CENTER|DT_VCENTER
    //Item.szText.Format(_T("%s"),CS->m_dataName);
        //Item.strText.Format(_T("%s"),CS->m_dataName);
    Item.strText = mstr.GetBuffer(mstr.GetLength());
    Item.iImage = pObj->GetNameID() - IDS_NAME_OBJECT;
    m_cColorlist.SetRowCount(nb+1);
    m_cColorlist.SetItem(&Item);
    m_cColorlist.AutoSizeColumns();
    CRect rect;
    m_cColorlist.GetClientRect(rect);
    m_cColorlist.SetColumnWidth(0,rect.Width()-2);
    m_cColorlist.SetRowHeight(nb,rect.Height()/2);
}

void CVerifyDlg::ClearTarget()
{
    //m_cTargetList.DeleteAllItems();
    m_cColorlist.SetRowCount(0);
}
