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
// DelTask.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Calques3DDoc.h"

#include "..\ViewUniv.h"

#include "Task.h"

#include "SuppressDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////
// Definition de TAmbigList
////////////////////////////////////////////////////////////////////////

CDeleteObjectTask::CDeleteObjectTask(CView *AParent,UINT taskID):
    CTask(AParent,taskID)
{
}

DWORD CDeleteObjectTask::GetMask()
{
    return CTask::GetMask();
}

unsigned CDeleteObjectTask::GetHelpResID()
{
    return CTX_SELECT_OBJECT;
}


void CDeleteObjectTask::OnMouseL(UINT, CPoint thepos)
{
    CObject3D *temp;

    int index = FindObject(thepos,GetMask(),FALSE,FALSE);
    //if (!index) return;
    temp = GetObject();
    if (!temp) return;

//  int rep = Parent->MessageBox("Voulez-vous vraiment détruire l'objet ?","Attention !!!",
//      MB_ICONQUESTION | MB_OKCANCEL);

    CxObject3DSet mySet;
    temp->GetDependList(&mySet);

    GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_DEP,temp);


    CSuppressDlg myDlg;
    myDlg.m_pList = &mySet;

    int rep = myDlg.DoModal();
    if (rep == IDOK)
    {
        BOOL bRes = GetDocument()->RemoveObject(temp);
        if (bRes)
        {
            InvalidateParent(TRUE);
            //m_pParent->Invalidate();
            //m_pParent->UpdateWindow();
        }
    }

    //TObject3DSet* list = ((calque3dDoc&)Parent->GetDocument()).GetDependList(temp);
    //TObject3DSupprDlg objSupp(Geom3DApp->GetMainWindow(),list);
    //int rep = objSupp.Execute();
    //delete list;

    //if (rep != IDOK) return;
    //int objnb = ((calque3dDoc&)Parent->GetDocument()).ObjectList->Find(temp);
    //((calque3dDoc&)Parent->GetDocument()).DeleteObject(objnb);
    //AdjustMenu();
    //Parent->Invalidate(0);
    //
}

void CDeleteObjectTask::OnMouseR(UINT, CPoint)
{
}

void CDeleteObjectTask::OnMouseMove(UINT, CPoint thepos)
{
    FindObject(thepos,GetMask(),TRUE,FALSE);
}

////////////////////////////////////////////////////////////////////////
// Definition de TAmbigList
////////////////////////////////////////////////////////////////////////
CCopyShapeTask::CCopyShapeTask(CView *AParent,UINT taskID): CTask(AParent,taskID)
{
    pShape = NULL;
    nMaskTarget = 0;
    m_nStep = 0;
    nSubTaskID = TSHAPE_DUP;
}

CCopyShapeTask::~CCopyShapeTask()
{
}


DWORD CCopyShapeTask::GetMask()
{
	DWORD nMask = TObject3DClass;
    //if (nMaskTarget)
//      nMask = nMaskTarget;
    return nMask;
}

unsigned CCopyShapeTask::GetHelpResID()
{
    switch (nSubTaskID){
    case TSHAPE_HID:
    case TSHAPE_MAR:
        return CTX_SELECT_OBJECT;
    case TSHAPE_DUP:
        {
            if (pShape)
                return CTX_SELECT_OBJECT;
            else
            {
                return CTX_SELECT_OBJECT;
            }
        }
    default:
        return CTask::GetHelpResID();
    }
}

void CCopyShapeTask::OnMouseL(UINT modkey, CPoint pt)
{
    BOOL bSubobj = (modkey & MK_SHIFT);
    int nb = FindObject(pt,GetMask(),FALSE,bSubobj);
    CObject3D *pObj = GetObject();

    if (!pObj)
    {
        pShape = NULL;
        nMaskTarget = 0;
        m_nStep = 0;
        GetContextualHelp();
        return;
    }

    CObject3DAttr mAttr = pObj->GetAttributes();

    if (nSubTaskID == TSHAPE_HID)
    {
        ////// ***** pObj->SetVisible(FALSE);
        mAttr.m_bVisible = FALSE;
    }
    else if (nSubTaskID == TSHAPE_MAR)
    {
        ///// ****** pObj->bMarked = !(pObj->bMarked);
        mAttr.m_bMarked = !(pObj->bMarked);
    }
    else if (pShape)
    {
        //pObj->pObjectShape.nShapeId = pShape->nShapeId;
        //pObj->pObjectShape.clrObject = pShape->clrObject;

        mAttr.m_pObjectShape.nShapeId = pShape->nShapeId;
        mAttr.m_pObjectShape.clrObject = pShape->clrObject;
    }
    else
    {
        nMaskTarget = MAKELONG(LOWORD(pObj->isA()),1);

        DWORD ppp = pObj->isA();
        DWORD ttt= TAllPointClass;
        WORD rrr = LOWORD(ppp);
        DWORD sss= MAKELONG(rrr,1);
        pShape  = &(pObj->pObjectShape);
        m_nStep++;
        return;
    }
    GetDocument()->ModifyPropObject(pObj,&mAttr);
    //UpdateAllViews(NULL,WM_UPDATEOBJ_MOD,pObj);
    InvalidateParent(TRUE);
    //m_pParent->Invalidate();
    //m_pParent->UpdateWindow();
}

void CCopyShapeTask::OnMouseR(UINT, CPoint point)
{
    theApp.ShowPopupMenu (IDR_POPUP_SHAPE, point, m_pParent);
}

void CCopyShapeTask::OnMouseMove(UINT modkey, CPoint pt)
{
    BOOL bSubobj = (modkey & MK_SHIFT);
    int nb = FindObject(pt,GetMask(),TRUE,bSubobj);
}

UINT CCopyShapeTask::GetTaskCursor()
{
    UINT curID = -1;

    int rep = m_cObjectFound.GetSize();
    if (!rep) return curID;

    if (nSubTaskID == TSHAPE_MAR)
        curID = IDC_DROP_MARK;
    else if (nSubTaskID == TSHAPE_HID)
        curID = IDC_DROP_ERASE;
    else
    {
        if (!m_nStep)
            curID = IDC_DROP_SELECT;
        else
            curID = IDC_DROP_APPLY;
    }
    return curID;
}

BOOL CCopyShapeTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
    BOOL bDone = TRUE;
    BOOL bCheck = FALSE;
    BOOL bEnab = FALSE;

    switch (pCmdUI->m_nID){
    case ID_VISUALISATION_STYLE_HIDE :
        bEnab = TRUE;
        bCheck = (nSubTaskID == TSHAPE_HID);
        break;
    case ID_VISUALISATION_STYLE_COPY  :
        bEnab = TRUE;
        bCheck = (nSubTaskID == TSHAPE_DUP);
        break;
    case ID_VISUALISATION_STYLE_MARK:
        bEnab = TRUE;
        bCheck = (nSubTaskID == TSHAPE_MAR);
        break;
    default:
        bDone = FALSE;
        break;
    }
    pCmdUI->Enable(bEnab);
    pCmdUI->SetCheck(bCheck);
    return bDone;

}

BOOL CCopyShapeTask::OnDoTasksOption(UINT nID)
{
    BOOL bDone = TRUE;
    switch (nID){
    case ID_VISUALISATION_STYLE_HIDE :
        if (nSubTaskID != TSHAPE_HID)
            nSubTaskID = TSHAPE_HID;
        break;
    case ID_VISUALISATION_STYLE_COPY  :
        if (nSubTaskID != TSHAPE_DUP)
        {
            nSubTaskID = TSHAPE_DUP;
            m_nStep = 0;
            pShape = NULL;
            nMaskTarget = 0;
        }

        break;
    case ID_VISUALISATION_STYLE_MARK:
        if (nSubTaskID != TSHAPE_MAR)
            nSubTaskID = TSHAPE_MAR;
        break;
        break;
    default:
        bDone = FALSE;
        break;
    }
    return bDone;
}
