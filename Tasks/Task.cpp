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
// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\MainFrm.h"
#include "..\Calques3D.h"

#include "..\Calques3DDoc.h"

#include "..\Objects\Vector4.h"
#include "..\Objects\Object3D.h"
#include "..\objects\Text3D.h"

#include "..\ViewUniv.h"

#include "Task.h"
#include "AmbiguityList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
/// Generic constructor for the task
///
/// \param pParent  A pointer to the main Universe view
/// \param nID      The identifier of the task (as in the command ID)
//////////////////////////////////////////////////////////////////////
CTask::CTask(CView *pParent,UINT nID):
        m_nTaskID(nID)
{
    m_pParent =DYNAMIC_DOWNCAST(CViewUniv,pParent);
    m_pObject = NULL;

    m_nStep = 0;
    m_bPersistent = FALSE;
    m_bIsRepereFixed = FALSE;
    GetContextualHelp();
}

//////////////////////////////////////////////////////////////////////
/// Generic destructor for the task
//////////////////////////////////////////////////////////////////////
CTask::~CTask()
{
    m_nStep = 0;
    m_pObject = NULL;
    m_cObjectFound.RemoveAll();
//    m_cStrObjFound.RemoveAll();
    if (m_pParent->GetSelectedObject())
    {
        m_pParent->GetSelectedObject()->SetSelected(FALSE);
        if (m_pParent->GetSafeHwnd())
        {
            InvalidateParent(TRUE);
            //m_pParent->Invalidate();
            //m_pParent->UpdateWindow();
        }
    }
    GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,NULL);
}

//////////////////////////////////////////////////////////////////////
/// Called, when the task is ended, to perform any internal cleaning
//////////////////////////////////////////////////////////////////////
void CTask::CancelTask()
{
}


//////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// Return a pointer to the main document
//////////////////////////////////////////////////////////////////////
CCalques3DDoc*  CTask::GetDocument()
{
    CDocument * mpDoc = m_pParent->GetDocument();
    CCalques3DDoc *pDoc=DYNAMIC_DOWNCAST(CCalques3DDoc,mpDoc);
    return pDoc;
}

//////////////////////////////////////////////////////////////////////
/// Return the resource identifier for the help message associated with the current step
/// of the task
///
/// Both #GetHelpResID() and #GetTaskResID() are used to populate the Status Bar
/// with a message describing the current stage of the task.
//////////////////////////////////////////////////////////////////////
unsigned CTask::GetHelpResID()
{
    return CTX_DEFAULT_PROP;
}

//////////////////////////////////////////////////////////////////////
/// Return the resource identifier for the title of the task
///
/// Both #GetHelpResID() and #GetTaskResID() are used to populate the Status Bar
/// with a message describing the current stage of the task.
//////////////////////////////////////////////////////////////////////
unsigned CTask::GetTaskResID() const
{
    return CTX_DEFAULT;
}



//////////////////////////////////////////////////////////////////////
/// Return a mask used to filter the objects that the user will be able to
/// designate on the main view.
///
/// The filter is a bitwise combination of objects identifiers (see #TObject3DClass and others).
/// See CObject3D#MaskObject and CObject3D#isA for description of the filter mechanism.
//////////////////////////////////////////////////////////////////////
CObjectId CTask::GetMask()
{
    return TObject3DClass;
}

//////////////////////////////////////////////////////////////////////
/// Called by the task to create and initialise the resulting object, if any,
/// and insert it in the universe.
//////////////////////////////////////////////////////////////////////
void CTask::CreateObject3D()
{
}

BOOL CTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
    return FALSE;
}


BOOL CTask::OnDoTasksOption(UINT nID)
{
    return FALSE;
}

void CTask::InvalidateParent(BOOL bForce/*=FALSE*/)
{
    if (m_pParent && m_pParent->GetSafeHwnd())
    {
        m_pParent->Invalidate();
        if (bForce)
            m_pParent->UpdateWindow();
    }
}

void CTask::DoLaunchView(UINT nID)
{
	CMainFrame *pMFrame = DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd());
	if (!pMFrame) return;

	::LockWindowUpdate(pMFrame->m_hWndMDIClient);
	CFrameWnd* tt = pMFrame->LaunchView(NULL,nID);
	HWND hwndT = ::GetWindow(pMFrame->m_hWndMDIClient, GW_CHILD);
	CArray<CFrameWnd*,CFrameWnd*> dd;
	while (hwndT != NULL)
	{
		CBCGPMDIChildWnd* pFrame = DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, 
		CWnd::FromHandle (hwndT));
		if (pFrame != NULL)
		{
			dd.Add(pFrame);
		}
		hwndT=::GetWindow(hwndT,GW_HWNDNEXT);
	}
	
	for (int i=0;i<dd.GetSize();i++)
	{
		CFrameWnd* pFrame = dd.GetAt(i);
		if (pFrame==NULL) continue;
		if (pFrame==tt || pFrame==m_pParent->GetParentFrame())
				::ShowWindow(pFrame->GetSafeHwnd(),SW_RESTORE);
			else
				::ShowWindow(pFrame->GetSafeHwnd(),SW_MINIMIZE);
	}
	m_pParent->GetParentFrame()->ActivateFrame();
	::SendMessage(pMFrame->m_hWndMDIClient,WM_MDITILE, MDITILE_VERTICAL,0);	
	::LockWindowUpdate(NULL);

}

void CTask::OnMouseL(UINT, CPoint pt)
{
    int nb = FindObject(pt,GetMask(),FALSE,FALSE);
    CObject3D *pObj = GetObject();
    if (m_pParent->GetSelectedObject() || pObj)
    {
        GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_SEL,pObj);
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
//  if (pObj)
//  {
//      GetDocument()->ModifyPropObject(pObj);
//  }
}

void CTask::OnMouseR(UINT, CPoint)
{
}

void CTask::OnMouseMove(UINT, CPoint pt)
{
    int nb = FindObject(pt,GetMask(),TRUE,FALSE);
}

void CTask::OnMouseLUp(UINT, CPoint pt)
{
}

void CTask::OnMouseLDC(UINT, CPoint pt)
{
}

UINT CTask::GetTaskCursor()
{
    UINT curID = -1;
    int rep = m_cObjectFound.GetSize();
    if (rep) curID = IDC_POINT;
    return curID;
}


int  CTask::FindObject(CPoint theLoc,CObjectId mask,BOOL bShowLabel,BOOL bSub)
{
    int trouve=0;
    CString strObj(_T(""));

    m_cObjectFound.RemoveAll();
//    m_cStrObjFound.RemoveAll();

//  CViewUniv *pUniv = DYNAMIC_DOWNCAST(CViewUniv,m_pParent);

//  BOOL bIsInC = FALSE;
    int nb = GetDocument()->m_cObjectSet.GetSize();
    int nCalcNum = m_pParent->GetVisualParam()->nCalqueNum;

    for (int i=0;i<nb;i++)
    {
        CObject3D *obj = GetDocument()->m_cObjectSet.GetAt(i);
        if (!obj) continue;
        if (DYNAMIC_DOWNCAST(CEquation3D,obj)) continue;
        obj->HitTest(theLoc,mask,nCalcNum,bSub,&m_cObjectFound);

//      if (pUniv)
//      {
//          bIsInC = obj->IsInCalque(pUniv->GetVisualParam()->nCalqueNum);
//      }
/*      if (obj->bVisible && obj->bValidate)
        {
            if (obj->MaskObject(TCompositeObject3DClass))
            {
                if (bIsInC)
                    obj->HitTest(theLoc,mask,bSub,&m_cObjectFound);
            }
            else
                if (obj->MaskObject(mask) && bIsInC)
                    obj->HitTest(theLoc,mask,bSub,&m_cObjectFound);
        }*/
    }
    trouve = m_cObjectFound.GetSize();
    CxObject3DSet mObjFound;
    if (bShowLabel)
    {
        for (int i=0;i<trouve;i++)
        {
            CObject3D *obj = m_cObjectFound.GetAt(i);
            if (!obj) continue;
            CString mstr = obj->GetObjectHelp();
//            m_cStrObjFound.Add(mstr);
            mObjFound.Add(obj);
        }
        //m_pParent->SendMessage(WM_SHOWOBJ_TOOLTIPS,(WPARAM)(&m_cStrObjFound),(LPARAM)trouve);
        m_pParent->SendMessage(WM_SHOWOBJ_TOOLTIPS,(WPARAM)(&mObjFound),(LPARAM)trouve);
    }
    else
        //m_pParent->SendMessage(WM_SHOWOBJ_TOOLTIPS,(WPARAM)(&m_cStrObjFound),(LPARAM)bShowLabel);
        m_pParent->SendMessage(WM_SHOWOBJ_TOOLTIPS,NULL,FALSE);

    return trouve;
}

CObject3D* CTask::GetObject()
{
    CObject3D* pObj = NULL;
    int nb = m_cObjectFound.GetSize();
    if (!nb)
    {
        m_pObject = NULL;
        return NULL;
    }

    if (nb==1)
    {
        pObj = m_cObjectFound.GetAt(0);
    }
    else
    {
        //CBCGPopupMenu *pMen=NULL;
        BOOL bFound= FALSE;
        CAmbiguityList pPickUp(IDS_AMBIGUITY,m_pParent);
        for (int i=0;i<nb;i++)
        {
            CObject3D *obj = m_cObjectFound.GetAt(i);
            if (!obj) continue;
            if (m_bPersistent && m_pObject == obj)
            {
                pObj = m_pObject;
                bFound= TRUE;
                break;
            }
            CString mstr = obj->GetObjectHelp();
//            m_cStrObjFound.Add(mstr);
            int nbO = pPickUp.AddString(mstr);
            pPickUp.AddHelper(nbO,obj->GetObjectDef());
        }
        if (!bFound)
        {
            int rrr = pPickUp.Execute();
            if (rrr!=-1)
            {
                pObj = m_cObjectFound.GetAt(rrr);
            }
        }
    }
    m_pObject = pObj;
    return pObj;
}

BOOL CTask::PrepareAddedObject(CObject3D *pObj)
{
    if (!pObj) return FALSE;

    //GetDocument()->m_cObjectSet.Add(pObj);
    if (GetDocument()->AddObject(pObj))
    {
        GetDocument()->UpdateAllViews(NULL,WM_UPDATEOBJ_ADD,pObj);
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
    return TRUE;
}


void CTask::DoContextMenu(UINT, CPoint&)
{
}

//////////////////////////////////////////////////////////////////////
/// Called by the task framework to get the contextual help associated with this task.
/// @return A String containing thye description of the current task 
//////////////////////////////////////////////////////////////////////
CString CTask::GetContextualHelp()
{
    CWnd *pwnd = AfxGetMainWnd();
    //CMainFrame *pUniv = DYNAMIC_DOWNCAST(CMainFrame,pwnd);
    CString strTask,strHelp;
    strTask.LoadString(GetTaskResID());
    strHelp.LoadString(GetHelpResID());

    CString mstr = strTask + _T(" : ") + strHelp;

    pwnd->SendMessage(WM_SHOWTASK_CTXTHELP,(WPARAM)(&mstr),(LPARAM)m_nTaskID);
    return mstr;
}

//////////////////////////////////////////////////////////////////////
/// Called by the task framework to calculate the visual feedback along
/// the various steps of the task
/// \deprecated Object are temporarily constructed and used to provide feeback
//////////////////////////////////////////////////////////////////////
void CTask::CalculFeedBack()
{
}

//////////////////////////////////////////////////////////////////////
/// Called by the main view to draw the visual feedback associated with the current
/// step of the task
//////////////////////////////////////////////////////////////////////
void CTask::DrawFeedBack(CDC *)
{
}


BOOL CTask::ShowErrorMsg(UINT sTextID,UINT sTextSubID,UINT flag)
{
    CString mstr,substr;
    mstr.LoadString(sTextID);
    if (sTextSubID!=-1)
    {
        substr.LoadString(sTextSubID);
        mstr += _T(" \n") + sTextSubID;
    }


    AfxGetMainWnd()->MessageBox(mstr,NULL,flag);
    return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDefTask::CDefTask(CView *pParent,UINT nID) : CTask(pParent,nID)
{
    m_bPersistent = TRUE;
    bMoveRep = FALSE;
}

CDefTask::~CDefTask()
{
    bMoveRep = FALSE;
}

void CDefTask::OnMouseLDC(UINT , CPoint thePt)
{
    if (!m_pObject)
    {
        int nb = FindObject(thePt,GetMask(),FALSE,FALSE);
        CObject3D *pObj = GetObject();
    }
    if (m_pObject)
    {
        GetDocument()->ModifyPropObject(m_pObject);
    }
}

void CDefTask::OnMouseR(UINT, CPoint point)
{
    theApp.ShowPopupMenu (IDR_POPUP_DEFAULT, point, m_pParent);
}

void CDefTask::OnMouseL(UINT modkey, CPoint theP)
{
    if (modkey & MK_SHIFT)
    {
        ptRepCenter = theP;
        bMoveRep = TRUE;
    }
    else
        CTask::OnMouseL(modkey,theP);
}

void CDefTask::OnMouseMove(UINT modkey, CPoint theP)
{
    if (bMoveRep)
    {
        CVisualParam *mv = m_pParent->GetVisualParam();
        if (!mv)
        {
            bMoveRep = FALSE;
            return;
        }
        CPoint r = mv->ptRepCoord;
        CSize dt = ptRepCenter - theP;
        mv->ptRepCoord -= dt;
        ptRepCenter = theP;
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
    else
        CTask::OnMouseMove(modkey,theP);
}

void CDefTask::OnMouseLUp(UINT modkey, CPoint theP)
{
    if (bMoveRep)
    {
        bMoveRep = FALSE;
    }
    else
        CTask::OnMouseLUp(modkey,theP);
}
