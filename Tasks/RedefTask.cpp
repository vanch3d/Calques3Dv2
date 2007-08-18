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
// RedefTask.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"

#include "..\Calques3DDoc.h"
#include "..\ViewUniv.h"
//#include "..\MainFrm.h"

#include "Task.h"
#include "AmbiguityList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CRedefine3DTask::CRedefine3DTask(CView *AParent,UINT nID) : CTask(AParent,nID)
{
    m_pObj = NULL;
    m_pOptionSet = NULL;
    m_pOption = NULL;
    m_nStep = 0;
    GetContextualHelp();
}


void CRedefine3DTask::CleanUp()
{
    if (m_pObj) m_pObj->SetSelected(FALSE);
    m_pObj = NULL;
    if (m_pOptionSet)
    {
        int nb = m_pOptionSet->GetSize();
        for (int i=0;i<nb;i++)
        {
            CRedefineScheme *pRed = m_pOptionSet->GetAt(i);
            if (!pRed) continue;
            delete pRed;
        }
        m_pOptionSet->RemoveAll();
        delete m_pOptionSet;
        m_pOptionSet = NULL;
    }
    m_pOption = NULL;
    m_cTarget.RemoveAll();
}


CRedefine3DTask::~CRedefine3DTask()
{
    CleanUp();
}

void CRedefine3DTask::OnMouseL(UINT, CPoint thepos)
{
    CObject3D *temp=NULL;

    int index = FindObject(thepos,GetMask(),FALSE,FALSE);
    temp = GetObject();
    if (!temp) return;

    if (!m_pObj)
    {

        if (!m_pOptionSet)
        {
            m_pOptionSet = temp->GetRedefineSchemes(NULL);
            if (!m_pOptionSet) return;
        }
        m_pObj = temp;
        m_pObj->SetSelected(TRUE);
        GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,m_pObj);
        m_pParent->Invalidate(0);
        m_pParent->UpdateWindow();

//      if (!m_pOption)
//      {
            int nb = m_pOptionSet->GetSize();

            CAmbiguityList pPickUp(IDS_REDEFINE,m_pParent);
            for (int i=0;i<nb;i++)
            {
                CRedefineScheme *pRedef = m_pOptionSet->GetAt(i);
                if (!pRedef)
                {
                    // This should not happen, but just in case...
                    pPickUp.AddString();
                }
                else
                {
                    int nbPt    = GetDocument()->IsTaskAvailable(pRedef->nAvail);
                    int nbO = pPickUp.AddString(pRedef->nObjResID,!nbPt);

                    CString strHelp;
                    strHelp.LoadString(pRedef->nAvail);
                    strHelp = strHelp.SpanExcluding("\n");
                    pPickUp.AddHelper(nbO,strHelp);
                }
            }
            int rrr = pPickUp.Execute();
            if (rrr!=-1)
            {
                m_pOption = m_pOptionSet->GetAt(rrr);
            }
            if (!m_pOption)
            {
                CleanUp();
                return;
            }
//      }
//      m_pObj = temp;
//      m_pObj->bIsSelected = TRUE;
        int nMaxStep = m_pOption->nRedefStep;

        if (!nMaxStep)
            CreateObject3D();
        else
        {
            m_nStep++;
            GetContextualHelp();
        }
        return;

    }

/*  if (!m_pObj)
    {
        m_pObj = temp;

        if (!nMaxStep)
            CreateObject3D();
        else
        {
            m_nStep++;
            GetContextualHelp();
        }
        return;
    }*/

    if (m_pObj)
    {
        int nMaxStep = m_pOption->nRedefStep;
        BOOL bDup = FALSE;
        CxObject3DSet *pSet = m_pObj->GetParents();
        if (pSet)
        {
            for (int i=0;i<pSet->GetSize();i++)
            {
                CObject3D *pTemp = pSet->GetAt(i);
                if (!pTemp) continue;
                if (pTemp == temp)
                {
                    bDup = TRUE;
                    break;
                }
            }
            delete pSet;
        }
        if (bDup && !m_pOption->bRelax)
        {
            ShowErrorMsg(ERR_REDEFINE_PARENTS);
            return;
        }
        else if (!bDup && m_pOption->bRelax)
        {
            ShowErrorMsg(ERR_REDEFINE_NOPARENTS);
            return;
        }
        m_cTarget.Add(temp);
        if (m_nStep == nMaxStep)
        {
            CreateObject3D();
        }
        else
        {
            m_nStep++;
        }
    }
}

void CRedefine3DTask::OnMouseR(UINT modkey, CPoint thepos)
{
}

void CRedefine3DTask::OnMouseMove(UINT, CPoint thepoint)
{
    FindObject(thepoint,GetMask(),TRUE,FALSE);
}

CObjectId CRedefine3DTask::GetMask()
{
    CObjectId mask;

    if (m_pOption)
        mask = m_pOption->nStepID[m_nStep-1];
    else
        mask = TPoint3DClass | TPointSurD3DClass | TPointSurC3DClass |
               TPointSurP3DClass | TPointSurS3DClass | TPointInterDD3DClass |
               TPointInterDP3DClass;

    return mask;
}

unsigned CRedefine3DTask::GetHelpResID()
{
	DWORD mask;

    if (m_pOption)
        mask = m_pOption->nTaskResID[m_nStep-1];
    else
        mask = CTX_SELECT_POINT1;

    return mask;
}

BOOL CRedefine3DTask::PrepareAddedObject(CObject3D *pObj)
{
    if (!pObj) return FALSE;

    //GetDocument()->m_cObjectSet.Add(pObj);
    if (GetDocument()->AddObject(pObj,FALSE))
    {
        GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pObj);
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
    return TRUE;
}


void CRedefine3DTask::CreateObject3D()
{
    if (!m_pObj) return;

    int nb = m_cTarget.GetSize();

    CxObject3DSet mList;
    m_pObj->GetDependList(&mList);
    int nbDep = mList.GetSize();

    BOOL bFound = FALSE;
    for (int i=0;i<nbDep && !bFound;i++)
    {
        CObject3D *pObj = mList.GetAt(i);
        if (!pObj) continue;
        for (int j=0;j<nb && !bFound;j++)
        {
            CObject3D *pObjT = m_cTarget.GetAt(j);
            if (!pObjT) continue;
            bFound = (BOOL)(pObj == pObjT);
        }
    }

    if (bFound)
    {
        ShowErrorMsg(ERR_REDEFINE_INGRAPH);
    }
    else
    {
        CObject3D * pNewObj = m_pOption->pNewObj;

        //CDialog mdlg(IDD_REDEFINE_DLG);
        //mdlg.DoModal();
        BOOL bRes = pNewObj->SetParents(&m_cTarget);

        BOOL bCalc = pNewObj->CalculConceptuel();
        if (bCalc)
        {
            ShowErrorMsg(bCalc);
        }

        m_pObj->SetInGraph(FALSE);
        CVector4 pppt = ((CPoint3D*)m_pObj)->Concept_pt;
        if (bRes = (bRes && m_pObj->GraftOn(pNewObj)))
        {
            //pMacro->cParentList.Add(temp);
            //pMacro->AddParent(temp);
            //m_cBasePoints.RemoveAt(0);
            //pMacro->RemoveSubObject(0);
            int nbObj = GetDocument()->m_cObjectSet.GetSize();
            CObject3D *pMyObj = NULL;
            for (int i=0;i<nbObj;i++)
            {
                CObject3D* pO = GetDocument()->m_cObjectSet.GetAt(i);
                if (pO != m_pObj) continue;

                pMyObj = m_pObj;
                break;
            }
            if (pMyObj)
            {
                GetDocument()->m_cObjectSet.RemoveAt(i);
                GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_DEL,pMyObj);
            }
            delete m_pObj;
            m_pObj = NULL;
        }

        if (bRes)
        {
            pNewObj->CopyPointPosition(pppt);
            pNewObj->CalculConceptuel();

            mList.RemoveAll();
            pNewObj->GetDependList(&mList);
            nbDep = mList.GetSize();
            for (int i=0;i<nbDep;i++)
            {
                CObject3D *pObj = mList.GetAt(i);
                if (!pObj) continue;

                pObj->SetDepth();
                pObj->CalculConceptuel();
            }
            m_pOption->pNewObj = NULL;
            PrepareAddedObject(pNewObj);

            CxObject3DSet pChildList;
            pNewObj->GetDependList(&pChildList,TRUE);

            int nb = pChildList.GetSize();

            for (i=0;i<nb;i++)
            {
                CObject3D *pObj = pChildList.GetAt(i);
                if (!pObj) continue;
                if (pObj==pNewObj) continue;

                int nbDoc = GetDocument()->m_cObjectSet.GetSize();
                for (int j=0;j<nbDoc;j++)
                {
                    CObject3D *pObj2 = GetDocument()->m_cObjectSet.GetAt(j);
                    if (!pObj2) continue;
                    if (pObj == pObj2)
                    {
                        int r=0;
                        r++;
                        GetDocument()->m_cObjectSet.RemoveAt(j);
                        GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_DEL,pObj2);
                        GetDocument()->m_cObjectSet.Add(pObj2);
                        GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pObj2);
                        InvalidateParent(TRUE);
                        //m_pParent->Invalidate();
                        //m_pParent->UpdateWindow();

                    }
                }
            }


        }
    }

    CleanUp();
    m_nStep=0;
    GetContextualHelp();
}

void CRedefine3DTask::DrawFeedBack(CDC *pDC)
{
}
