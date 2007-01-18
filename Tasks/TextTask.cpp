// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

#include "..\prefs\prefs.h"

//#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\Objects\Text3D.h"
//#include "..\Objects\Point3D.h"
//#include "..\Objects\Plan3D.h"

#include "..\Calques3DDoc.h"
#include "..\ViewUniv.h"
//#include "..\MainFrm.h"

#include "Task.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLabelTask::CLabelTask(CView *pParent,UINT taskID) : CTask(pParent,taskID)
{
    m_cSelObject.RemoveAll();
    pEditZone = NULL;
    pSource = NULL;
    m_bSingleObj = FALSE;
}

CLabelTask::~CLabelTask()
{
    if (pEditZone)
        delete pEditZone;
    for (int i=0;i< m_cSelObject.GetSize();i++)
    {
        m_cSelObject.GetAt(i)->SetSelected(FALSE);
    }
    if (pSource)
    {
        pSource->SetSelected(FALSE);
    }
    InvalidateParent();
    //if (m_pParent->GetSafeHwnd())
    //  m_pParent->Invalidate();
}

unsigned CLabelTask::GetTaskResID() const
{
    if (m_nTaskID== ID_EXPLORATION_LABEL)
        return CTX_LABEL;
    else //if (m_nTaskID== ID_EXPLORATION_EQUATION)
        return CTX_EQUATION;
}

unsigned CLabelTask::GetHelpResID()
{
    return CTX_SELECT_OBJECT;
}

DWORD CLabelTask::GetMask()
{
	if (m_nTaskID== ID_EXPLORATION_LABEL)
		return (TAllPointClass /*| TAllDroiteClass | TAllPlanClass | TSphere3DClass*/);
    else //if (m_nTaskID== ID_EXPLORATION_EQUATION)
		return (TAllPointClass | TAllDroiteClass | TAllPlanClass | TSphere3DClass);
}

void CLabelTask::CreateObject3D()
{
    //string beep(((TEditObjectName*)editzone)->GetObjectName());
    //TObject3D *temp = ((TEditObjectName*)editzone)->source;
    //if (!temp) return;
    //*(temp->ObjectName) = beep;
    //TFindText test = {0};
    //temp->DependList->ForEach(SearchText,&test);
    //if (!test.aText)
     //{
    if (!pSource) return;

    CObject3D *temp=NULL;
    if (m_nTaskID== ID_EXPLORATION_LABEL)
    {
        temp = new CLabel3D(pSource);
        m_bSingleObj = TRUE;
    }
    else if (m_nTaskID== ID_EXPLORATION_EQUATION)
    {
        temp = new CEquation3D(pSource);
        m_bSingleObj = FALSE;
    }
    if (!temp) return;

    BOOL bFound = FALSE;
    if (m_bSingleObj)
    {
        CxObject3DSet cDependList;
        pSource->GetDependList(&cDependList,FALSE);
        int nb = cDependList.GetSize();

        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = cDependList.GetAt(i);
            if (!pObj) continue;
            if (pObj->MaskObject(temp->isA()))//temp->isA()))
            {
                bFound = TRUE;
                break;
            }
        }
    }

    if (bFound)
    {
        Beep(15000,1000);
        delete temp;
    }
    else
    {
        if (m_nTaskID== ID_EXPLORATION_EQUATION)
        {
			DoLaunchView(ID_VIEW_ANALYTIC);
           m_cSelObject.Add(pSource);
            pSource->SetSelected(TRUE);
            GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,pSource);
        }
        temp->CalculConceptuel();
        PrepareAddedObject(temp);
    }
    pSource=NULL;
    m_nStep = 0;
    InvalidateParent();
    //m_pParent->Invalidate();
}

void CLabelTask::OnMouseL(UINT, CPoint thepos)
{
    CObject3D *temp=0;

    int index = FindObject(thepos,GetMask(),FALSE);
    if (!index) return;
    temp = GetObject();

    if (!temp) return;

    pSource = temp;
    CreateObject3D();
}

void CLabelTask::OnMouseMove(UINT, CPoint pt)
{
    int nb = FindObject(pt,GetMask(),TRUE,TRUE);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMeasureTask::CMeasureTask(CView *AParent,UINT nID):CLabelTask(AParent,nID)
{
    pEnd = NULL;
    pEnd2 = NULL;
    m_nTargets = -1;
    m_nOptionID = ID_EXPLORATION_MEASURE_DISTANCE;
}

CMeasureTask::~CMeasureTask()
{
/*  if (pEnd)
    {
        pEnd->SetSelected(FALSE);
    }
    if (pEnd2)
    {
        pEnd2->SetSelected(FALSE);
    }*/
    CancelTask();
}

void CMeasureTask::CancelTask()
{
    for (int i=0;i<m_cObjectsTarget.GetSize();i++)
    {
        CObject3D *pObj = m_cObjectsTarget.GetAt(i);
        if (!pObj) continue;
        pObj->SetSelected(FALSE);
    }
    m_cObjectsTarget.RemoveAll();
    m_nStep = 0;
    InvalidateParent(TRUE);
    //if (m_pParent && m_pParent->GetSafeHwnd())
    //      m_pParent->Invalidate();
}


unsigned CMeasureTask::GetHelpResID()
{
    return CTX_SELECT_OBJECT;
}

DWORD CMeasureTask::GetMask()
{
    DWORD mask = 0;
    switch (m_nOptionID){
    case ID_EXPLORATION_MEASURE_DISTANCE :
        //if (pSource)
            mask = TAllPointClass;
        //else
//          mask = TAllPointClass|TAllCercleClass;
            m_nTargets = 2;
        break;
    case ID_EXPLORATION_MEASURE_ANGLE  :
        if (!m_nStep)
        {
            mask = TAllPointClass|TAllPlanClass;
            m_nTargets = 2;
        }
        else
        {
            if (m_nTargets==2)
                mask = TAllPlanClass;
            else
                mask = TAllPointClass;
        }

        break;
    case ID_EXPLORATION_MEASURE_VOLUME :
        mask = TCube3DClass|TSphere3DClass|TCylindre3DClass;
            m_nTargets = 1;
        break;
    case ID_EXPLORATION_MEASURE_AREA:
        mask = TCercle3DClass|TSphere3DClass|TCylindre3DClass|TPolygon3DClass;
        m_nTargets = 1;
        break;
    case ID_EXPLORATION_MEASURE_LENGTH:
        mask = TAllCercleClass|TAllSegmentClass;
        m_nTargets = 1;
        break;
    }
    return mask;
}

void CMeasureTask::CreateObject3D()
{
    //if (!pSource && !pEnd) return;

    int nObj = m_cObjectsTarget.GetSize();

    if (nObj!=m_nTargets) return;

    CObject3D *temp=NULL;

    switch (m_nOptionID){
    case ID_EXPLORATION_MEASURE_DISTANCE :
            //temp = new CDistance3D(pSource,pEnd);
            temp = new CDistance3D(m_cObjectsTarget.GetAt(0),m_cObjectsTarget.GetAt(1));
        break;
    case ID_EXPLORATION_MEASURE_ANGLE  :
        if (m_nTargets==2)
            temp = new CAngle3D((CPlan3D* )m_cObjectsTarget.GetAt(0),
                                (CPlan3D* )m_cObjectsTarget.GetAt(1));
        else
            temp = new CAngle3D((CPoint3D* )m_cObjectsTarget.GetAt(0),
                                (CPoint3D* )m_cObjectsTarget.GetAt(1),
                                (CPoint3D* )m_cObjectsTarget.GetAt(2));
        break;
    case ID_EXPLORATION_MEASURE_VOLUME :
        temp = new CVolume3D(m_cObjectsTarget.GetAt(0));
        break;
    case ID_EXPLORATION_MEASURE_AREA:
        temp = new CArea3D(m_cObjectsTarget.GetAt(0));
        break;
    case ID_EXPLORATION_MEASURE_LENGTH:
        temp = new CDistance3D(m_cObjectsTarget.GetAt(0));
        break;
    }


    if (!temp) return;

	DoLaunchView(ID_VIEW_ANALYTIC);
    BOOL bFound = FALSE;
/*  CxObject3DSet cDependList;
    pSource->GetDependList(&cDependList,FALSE);
    int nb = cDependList.GetSize();

    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = cDependList.GetAt(i);
        if (!pObj) continue;
        if (pObj->MaskObject(temp->isA()))//temp->isA()))
        {
            bFound = TRUE;
            break;
        }
    }*/

    if (bFound)
    {
        Beep(15000,1000);
        delete temp;
    }
    else
    {
        temp->CalculConceptuel();
        PrepareAddedObject(temp);
    }
/*  pSource->SetSelected(FALSE);
    if (pEnd) pEnd->SetSelected(FALSE);
    if (pEnd2) pEnd2->SetSelected(FALSE);
    pSource=pEnd=NULL;*/

    CancelTask();
}

void CMeasureTask::OnMouseR(UINT, CPoint point)
{
    theApp.ShowPopupMenu (IDR_POPUP_MEASURE, point, m_pParent);
}


void CMeasureTask::OnMouseL(UINT, CPoint thepos)
{
    CObject3D *temp=0;

    int index = 0;

    //if (m_nOptionID==ID_EXPLORATION_MEASURE_VOLUME)
    //  index = FindObject(thepos,GetMask(),FALSE);
    //else
    index = FindObject(thepos,GetMask(),FALSE);

    if (!index) return;
    temp = GetObject();
    if (!temp) return;

    int nObj = m_cObjectsTarget.GetSize();

    for (int i=0;i<nObj;i++)
    {
        CObject3D *pObj = m_cObjectsTarget.GetAt(i);
        if (!pObj) continue;
        if (pObj==temp)
        {
            ShowErrorMsg(ERR_2SAMEPTS);
            return;
        }
    }

    m_cObjectsTarget.Add(temp);
    nObj++;

/*  if (!pSource)
        pSource = temp;
    else if (!pEnd)
    {
        if (temp != pSource)
            pEnd = temp;
        else
        {
            ShowErrorMsg(ERR_2SAMEPTS);
            return;
        }
    }
    else if ((temp != pSource) && (temp != pEnd))
        pEnd2 = temp;
    else
    {
        ShowErrorMsg(ERR_2SAMEPTS);
        return;
    }*/

    if (m_nOptionID==ID_EXPLORATION_MEASURE_ANGLE)
    {
        if (temp->MaskObject(TAllPointClass))
            m_nTargets = 3;
        else
            m_nTargets = 2;
    }


    temp->SetSelected(TRUE);
    GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
    //temp->CalculVisuel(m_pParent->GetVisualParam());
    m_pParent->Invalidate(0);

    if (m_nTargets==nObj)
    {
        CreateObject3D();
    }
    else
    {
        m_nStep++;
        GetContextualHelp();
    }

/*  switch (m_nOptionID){
    case ID_EXPLORATION_MEASURE_DISTANCE :
        if (pSource && pEnd)
        {
            CreateObject3D();
            return;
        }
        else if (pSource)
        {
            //m_ptOld = ptA->Visual_pt;
            m_nStep++;
            GetContextualHelp();
        }
        break;
    case ID_EXPLORATION_MEASURE_ANGLE  :
        if (pSource && pEnd && pEnd2)
        {
            CreateObject3D();
            return;
        }
        else //if (pSource)
        {
            //m_ptOld = ptA->Visual_pt;
            m_nStep++;
            GetContextualHelp();
        }
        break;
    case ID_EXPLORATION_MEASURE_VOLUME :
    case ID_EXPLORATION_MEASURE_LENGTH:
    case ID_EXPLORATION_MEASURE_AREA:
        if (pSource)
        {
            CreateObject3D();
            return;
        }
        break;
    }*/
}

BOOL CMeasureTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
    BOOL bDone = TRUE;
    BOOL bCheck = FALSE;
    BOOL bEnab = FALSE;

    switch (pCmdUI->m_nID){
    case ID_EXPLORATION_MEASURE_DISTANCE :
    case ID_EXPLORATION_MEASURE_ANGLE  :
    case ID_EXPLORATION_MEASURE_VOLUME :
    case ID_EXPLORATION_MEASURE_AREA:
    case ID_EXPLORATION_MEASURE_LENGTH:
        bEnab = TRUE;
        bCheck = m_nOptionID==pCmdUI->m_nID;
        break;
    default:
        bDone = FALSE;
        break;
    }
    pCmdUI->Enable(bEnab);
    pCmdUI->SetCheck(bCheck);
    return bDone;
}



BOOL CMeasureTask::OnDoTasksOption(UINT nID)
{
    BOOL bDone = TRUE;
    switch (nID){
    case ID_EXPLORATION_MEASURE_DISTANCE :
    case ID_EXPLORATION_MEASURE_ANGLE  :
    case ID_EXPLORATION_MEASURE_VOLUME :
    case ID_EXPLORATION_MEASURE_AREA:
    case ID_EXPLORATION_MEASURE_LENGTH:
        bDone = TRUE;
        m_nOptionID = nID;
        break;
    default:
        bDone = FALSE;
        break;
    }
    if (bDone) CancelTask();
    return bDone;
}
