// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"

//#include "..\Objects\Vector4.h"
//#include "..\Objects\Object3D.h"
#include "..\Objects\Point3D.h"
#include "..\Calques3DDoc.h"
#include "..\Objects\Droite3D.h"
#include "..\Objects\Cercle3D.h"
#include "..\Objects\Plan3D.h"

#include "..\ViewUniv.h"

#include "Task.h"

#include "SuppressDlg.h"
#include "VerifyDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define TM_ANIMATION        1200                ///< Timer for the Animation

////////////////////////////////////////////////////////////////////////
// Definition de TAmbigList
////////////////////////////////////////////////////////////////////////
CSymetricTask::CSymetricTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
    pSrc = pSym = NULL;

}

CSymetricTask::~CSymetricTask()
{
    if (pSrc)   pSrc->SetSelected(FALSE);
    if (pSym)   pSym->SetSelected(FALSE);
    if (pSrc || pSym)
        InvalidateParent();
        //m_pParent->Invalidate();
}

unsigned CSymetricTask::GetHelpResID()
{
    return ((m_nStep) ? CTX_SELECT_SYM : CTX_SELECT_POINT1);
}

DWORD CSymetricTask::GetMask()
{
    DWORD mask;
//  mask = TAllPointClass;

    if (!pSrc)
        mask = TAllPointClass;
    else
        mask = TAllPointClass | TAllDroiteClass | TAllPlanClass;
    return mask;
}

void CSymetricTask::OnMouseL(UINT, CPoint thepos)
{
    CObject3D *temp;
    int index = FindObject(thepos,GetMask(),FALSE);
    if (!index) return;

    temp = GetObject();
    if (!temp) return;

    temp->SetSelected(TRUE);
    GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
    m_pParent->Invalidate(0);

    if (!pSrc)
    {
        pSrc = temp;
        m_ptOld = thepos;
        m_nStep++;
    }
    else
    {
        if (temp != pSrc)
            pSym = temp;
        else
        {
            ShowErrorMsg(ERR_2SAMEPTS);
            return;
        }
    }
    if (pSrc && pSym)
    {
        CreateObject3D();
    }
    GetContextualHelp();
}

void CSymetricTask::OnMouseMove(UINT, CPoint thepoint)
{
    FindObject(thepoint,GetMask(),TRUE);
    m_ptOld = thepoint;
    if (pSrc)
        m_pParent->Invalidate(FALSE);
}

void CSymetricTask::CreateObject3D()
{
    CObject3D *temp=NULL;
    if (!pSrc || !pSym) return;

    if (pSym->MaskObject(TAllPointClass))
        temp = new CPointSymetric3D((CPoint3D*)pSrc,(CPoint3D*)pSym);
    else if (pSym->MaskObject(TAllDroiteClass))
        temp = new CPointSymetric3D((CPoint3D*)pSrc,(CDroite3D*)pSym);
    else if (pSym->MaskObject(TAllPlanClass))
        temp = new CPointSymetric3D((CPoint3D*)pSrc,(CPlan3D*)pSym);

    if (temp)
    {
        temp->CalculConceptuel();
        PrepareAddedObject(temp);
    }

    if (pSym)   pSym->SetSelected(FALSE);
    if (pSrc)   pSrc->SetSelected(FALSE);
    if (pSym || pSrc)
        m_pParent->Invalidate(0);
    pSym = pSrc = 0;
    m_nStep = 0;
    GetContextualHelp();
}

void CSymetricTask::DrawFeedBack(CDC *pDC)
{
    if (!pSrc) return;
    CPoint a = ((CPoint3D *)pSrc)->Visual_pt;
    CPoint b = m_ptOld;
    CSize dx = b-a;
    CPoint c =  b + dx;
    CPen thePen(PS_DOT,1,RGB(0,0,0));
    CPen *oldP = pDC->SelectObject(&thePen);
    pDC->MoveTo(a);
    pDC->LineTo(c);
    pDC->SelectObject(oldP);
    pDC->Ellipse(c.x-3,c.y-3,c.x+4,c.y+4);
}

////////////////////////////////////////////////////////////////////////
// Definition de CVerify3DTask
////////////////////////////////////////////////////////////////////////
CVerify3DTask::CVerify3DTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
    CancelTache();

    m_pVerifyDlg = NULL;
    m_pVerifyDlg = new CVerifyDlg();
    m_pVerifyDlg->Create(CVerifyDlg::IDD,m_pParent);
    m_pVerifyDlg->m_pView = m_pParent;
    m_pVerifyDlg->ShowWindow(SW_SHOW);
    m_pParent->SetFocus();

    m_nStep = 0;
}

CVerify3DTask::~CVerify3DTask()
{
    if (m_pVerifyDlg) delete m_pVerifyDlg;
    m_pVerifyDlg = NULL;
    CancelTache();
}

void CVerify3DTask::CancelTache()
{
    m_nStep = 0;
    for (int i=0;i<m_cObjectTarget.GetSize();i++)
    {
        CObject3D *pObj = m_cObjectTarget.GetAt(i);
        if (!pObj) continue;
        pObj->SetSelected(FALSE);
    }
    if (m_pParent->GetSafeHwnd())
    {
        m_pParent->GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,NULL);
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
    m_cObjectTarget.RemoveAll();
    m_nCurrTask = 0;
}

unsigned CVerify3DTask::GetHelpResID()
{
    DWORD mask;

    int nb = m_cObjectTarget.GetSize();

    if (!m_nStep)
        mask = CTX_VERIFY_PROP;
    else
    {
        switch (m_nCurrTask){
        case ID_EXPLORATION_VERIFY_PAR :
        case ID_EXPLORATION_VERIFY_PERP:
            //if (!nb)
                mask = CTX_SELECT_DROITEPLAN;
            /*else
            {
                CObject3D *pObj = m_cObjectTarget.GetAt(0);
                if (pObj->MaskObject(TAllDroiteClass))
                    mask = CTX_SELECT_DROITE2;
                else
                    mask = CTX_SELECT_PLAN2;
            }*/
            break;
        case ID_EXPLORATION_VERIFY_PTON:
            if (!nb)
                mask = CTX_SELECT_POINT1;
            else
                mask = CTX_SELECT_OBJECT;
            break;
        case ID_EXPLORATION_VERIFY_PTALIGN:
            mask = CTX_SELECT_POINT1;
            break;
        default:
            mask = 0L;
            break;
        }
    }
    return mask;
}

DWORD CVerify3DTask::GetMask()
{
    DWORD mask;

    int nb = m_cObjectTarget.GetSize();

    switch (m_nCurrTask){
    case ID_EXPLORATION_VERIFY_PAR :
    case ID_EXPLORATION_VERIFY_PERP:
        //if (!nb)
            mask = TAllDroiteClass | TAllPlanClass;
        /*else
        {
            CObject3D *pObj = m_cObjectTarget.GetAt(0);
            if (pObj->MaskObject(TAllDroiteClass))
                mask = TAllDroiteClass;
            else
                mask = TAllPlanClass;
        }*/
        break;
    case ID_EXPLORATION_VERIFY_PTON:
        if (!nb)
            mask = TAllPointClass;
        else
            mask = TAllPlanClass | TAllDroiteClass | TAllCercleClass | TSphere3DClass;
        break;
    case ID_EXPLORATION_VERIFY_PTALIGN:
        mask = TAllPointClass;
        break;
    default:
        mask = 0L;
        break;
    }
    return mask;
}


void CVerify3DTask::OnMouseR(UINT, CPoint point)
{
    theApp.ShowPopupMenu (IDR_POPUP_VERIFY, point, m_pParent);
}

BOOL CVerify3DTask::OnUpdateTasksOption(CCmdUI* pCmdUI)
{
    BOOL bDone = TRUE;
    BOOL bCheck = FALSE;
    BOOL bEnab = FALSE;

    BOOL bRes = m_pParent->GetDocument()->IsTaskAvailable(pCmdUI->m_nID);

    switch (pCmdUI->m_nID){
    case ID_EXPLORATION_VERIFY_PAR :
    case ID_EXPLORATION_VERIFY_PERP:
    case ID_EXPLORATION_VERIFY_PTON:
    case ID_EXPLORATION_VERIFY_PTALIGN:
        bEnab = TRUE;
        bCheck = (m_nCurrTask == pCmdUI->m_nID);
        break;
    default:
        bDone = FALSE;
        break;
    }
    pCmdUI->Enable(bEnab);
    pCmdUI->SetCheck(bCheck);
    return bDone;
}


BOOL CVerify3DTask::OnDoTasksOption(UINT nID)
{
    BOOL bDone = TRUE;
    switch (nID){
    case ID_EXPLORATION_VERIFY_PAR :
    case ID_EXPLORATION_VERIFY_PERP:
    case ID_EXPLORATION_VERIFY_PTON:
    case ID_EXPLORATION_VERIFY_PTALIGN:
        //CreateObject3D();
        CancelTache();
        m_nCurrTask = nID;
        break;
    default:
        m_nCurrTask = 0;
        bDone = FALSE;
        break;
    }

    int nOld = m_nCurrTask;
    if (m_nCurrTask)
    {
        CancelTache();
        m_nStep=1;
        m_pVerifyDlg->ClearTarget();
        m_pVerifyDlg->m_strResult = _T("");
        m_pVerifyDlg->m_bShowCE = FALSE;
        m_pVerifyDlg->m_nTask = 0;
        //m_pVerifyDlg->m_pObj1 = NULL;
        //m_pVerifyDlg->m_pObj2 = NULL;
        m_pVerifyDlg->m_pObjFree = NULL;
        m_pVerifyDlg->m_bIsCE=FALSE;
        m_pVerifyDlg->UpdateData(FALSE);
        //m_pVerifyDlg->ShowWindow(SW_HIDE);

        m_nCurrTask = nOld;
        m_pVerifyDlg->m_nTask = m_nCurrTask;
        m_pVerifyDlg->UpdateData(FALSE);
        m_pVerifyDlg->ShowWindow(SW_SHOW);
        GetContextualHelp();
    }
    else
        m_pVerifyDlg->ShowWindow(SW_HIDE);
    return bDone;
}

BOOL CVerify3DTask::GetFreePoint(CObject3D* pObj,CxObject3DSet& pSet)
{
    if (pObj->MaskObject(TPoint3DClass|TPointSurD3DClass|
                         TPointSurC3DClass|TPointSurP3DClass|
                         TPointSurS3DClass))
    {
        pSet.Add(pObj);
        return TRUE;
    }
    else
    {
        CxObject3DSet* pParentSet = pObj->GetParents();
        if (!pParentSet) return FALSE;
        for (int i=0;i<pParentSet->GetSize();i++)
        {
            CObject3D *pParent = pParentSet->GetAt(i);
            if (!pParent) continue;
            GetFreePoint(pParent,pSet);
        }
        delete pParentSet;
    }
    return TRUE;
}

#define POSVER 50

BOOL CVerify3DTask::OnVerifyParallel(CObject3D *pObj1,CObject3D *pObj2)
{
    if (!pObj1 || !pObj2) return FALSE;

    UINT res = VER_ERROR;

    //***********
    if (m_nCurrTask==ID_EXPLORATION_VERIFY_PAR)
        res = pObj1->IsParallelTo(pObj2);
    else if (m_nCurrTask==ID_EXPLORATION_VERIFY_PERP)
        res = pObj1->IsPerpendicularTo(pObj2);
    //***********

    if (res==VER_PAR_ANALYTIC || res==VER_PERP_ANALYTIC)
    {
        CxObject3DSet mSet;
        GetFreePoint(pObj1,mSet);
        GetFreePoint(pObj2,mSet);

        CVector4 tempPt[4] = {  CVector4(POSVER,0,0),
                                CVector4(0,POSVER,0),
                                CVector4(0,0,POSVER),
                                CVector4(POSVER,POSVER,POSVER)};

        res=(m_nCurrTask==ID_EXPLORATION_VERIFY_PAR)?VER_PAR_CONSTRUCTION:VER_PERP_CONSTRUCTION;

        for (int i=0;i<mSet.GetSize();i++)
        {
            if (res != VER_PAR_CONSTRUCTION && res != VER_PERP_CONSTRUCTION) break;
            CPoint3D *pFree = DYNAMIC_DOWNCAST(CPoint3D,mSet.GetAt(i));
            if (!pFree) continue;

            CVector4 ptOrig = pFree->Concept_pt;

            CxObject3DSet mList;
            pFree->GetDependList(&mList);

            for (int j=0;j<4;j++)
            {
                CVector4 ptt = ptOrig + tempPt[j];

                pFree->Concept_pt = ptt;
                for (int k=0;k<mList.GetSize();k++)
                {
                    CObject3D *pObj = mList.GetAt(k);
                    if (!pObj) continue;
                    pObj->CalculConceptuel();
                }

                UINT res2=VER_ERROR;

                //***********
                if (m_nCurrTask==ID_EXPLORATION_VERIFY_PAR)
                    res2 = pObj1->IsParallelTo(pObj2);
                else if (m_nCurrTask==ID_EXPLORATION_VERIFY_PERP)
                    res2 = pObj1->IsPerpendicularTo(pObj2);
                //***********

                if (res2==VER_PAR_NO || res2==VER_PERP_NO)
                {
                    res=VER_PAR_ANALYTIC;
			        res=(m_nCurrTask==ID_EXPLORATION_VERIFY_PAR)?VER_PAR_ANALYTIC:VER_PERP_ANALYTIC;

                    m_pVerifyDlg->m_pObjFree = pFree;
                    m_pVerifyDlg->m_vNewPt = ptt;
                    m_pVerifyDlg->m_vOldPt = ptOrig;
                    m_pVerifyDlg->m_bIsCE=FALSE;

                    break;
                }
            }
            pFree->Concept_pt = ptOrig;
            for (int k=0;k<mList.GetSize();k++)
            {
                CObject3D *pObj = mList.GetAt(k);
                if (!pObj) continue;
                pObj->CalculConceptuel();
            }
        }
    }

    CString strMsg2,strMsg,strProp,strTarget,strTarget2;
    strMsg.LoadString(res);

    //if (m_nCurrTask==ID_EXPLORATION_VERIFY_PAR)
    //    strProp.LoadString(VER_PAR);
    //else if (m_nCurrTask==ID_EXPLORATION_VERIFY_PERP)
    //    strProp.LoadString(VER_PERP);

    strTarget = pObj1->GetObjectHelp();
    strTarget2 = pObj2->GetObjectHelp();

    strMsg2.Format(strMsg,strTarget,strTarget2);

    m_pVerifyDlg->m_strResult = strMsg2;
    m_pVerifyDlg->m_bShowCE = (res==VER_PAR_ANALYTIC||res==VER_PERP_ANALYTIC);
    m_pVerifyDlg->UpdateData(FALSE);
    m_pVerifyDlg->ShowWindow(SW_SHOW);

    return TRUE;
}

BOOL CVerify3DTask::OnVerifyPerpendicular()
{
    return TRUE;
}

BOOL CVerify3DTask::OnVerifyPointOn(CObject3D *pOPt,CObject3D *pObj)
{
    UINT res = VER_ERROR;
    CPoint3D *pPt = DYNAMIC_DOWNCAST(CPoint3D,pOPt);

    if (!pPt || !pObj) return FALSE;

    CString strMsg2,strMsg,strProp,strTarget;
    FCoord dis = 0.0;
    BOOL bRet = pPt->GetDistanceFrom(pObj,dis);

    if (!bRet) return FALSE;

    res = (FCZero(dis)) ? VER_BELONG_ANALYTIC : VER_BELONG_NO;

    if (res == VER_BELONG_ANALYTIC)
    {
        CxObject3DSet mSet;
        GetFreePoint(pPt,mSet);
        GetFreePoint(pObj,mSet);

        CVector4 tempPt[4] = {  CVector4(POSVER,0,0),
                                CVector4(0,POSVER,0),
                                CVector4(0,0,POSVER),
                                CVector4(POSVER,POSVER,POSVER)};

        res=VER_BELONG_CONSTRUCTION;

        for (int i=0;i<mSet.GetSize();i++)
        {
            if (res != VER_BELONG_CONSTRUCTION) break;
            CPoint3D *pFree = DYNAMIC_DOWNCAST(CPoint3D,mSet.GetAt(i));
            if (!pFree) continue;

            CVector4 ptOrig = pFree->Concept_pt;

            CxObject3DSet mList;
            pFree->GetDependList(&mList);

            for (int j=0;j<4;j++)
            {
                CVector4 ptt = ptOrig + tempPt[j];

                pFree->Concept_pt = ptt;
                for (int k=0;k<mList.GetSize();k++)
                {
                    CObject3D *pObj = mList.GetAt(k);
                    if (!pObj) continue;
                    pObj->CalculConceptuel();
                }

                UINT res2=VER_ERROR;

                //***********
                bRet = pPt->GetDistanceFrom(pObj,dis);
                if (!bRet) res2 = VER_BELONG_ANALYTIC;
                else
                    res2 = (!FCZero(dis)) ? VER_BELONG_NO : VER_BELONG_ANALYTIC;
                //***********

                if (res2==VER_BELONG_NO)
                {
                    res=VER_BELONG_ANALYTIC;

                    m_pVerifyDlg->m_pObjFree = pFree;
                    m_pVerifyDlg->m_vNewPt = ptt;
                    m_pVerifyDlg->m_vOldPt = ptOrig;
                    m_pVerifyDlg->m_bIsCE=FALSE;

                    break;
                }
            }
            pFree->Concept_pt = ptOrig;
            for (int k=0;k<mList.GetSize();k++)
            {
                CObject3D *pObj = mList.GetAt(k);
                if (!pObj) continue;
                pObj->CalculConceptuel();
            }
        }
    }

    strMsg.LoadString(res);
    strTarget = pPt->GetObjectHelp();
    strProp = pObj->GetObjectHelp();
    strMsg2.Format(strMsg,strTarget,strProp);

    m_pVerifyDlg->m_strResult = strMsg2;
    m_pVerifyDlg->m_bShowCE = (res==VER_BELONG_ANALYTIC);
    m_pVerifyDlg->UpdateData(FALSE);
    m_pVerifyDlg->ShowWindow(SW_SHOW);

/*  CPoint3D *pPt = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(0));
    CDroite3D *pDr = DYNAMIC_DOWNCAST(CDroite3D,m_cObjectTarget.GetAt(1));
    CPlan3D *pPl = DYNAMIC_DOWNCAST(CPlan3D,m_cObjectTarget.GetAt(1));

    CString strMsg2,strMsg,strProp,strTarget;

    if (pPt && pDr)
    {
        CVector4 p0 = pPt->Concept_pt;
        CVector4 p1 = pDr->GetBasePoint();
        CVector4 pd = pDr->GetDirVector();

        pd = pd.Normalized();

        if (!pd.NullVector())
        {
            FCoord denom = sqrt(pd.x*pd.x+pd.y*pd.y+pd.z*pd.z);
            FCoord num =    pow(pd.z*(p0.y-p1.y)-pd.y*(p0.z-p1.z),2) +
                            pow(pd.x*(p0.z-p1.z)-pd.z*(p0.x-p1.x),2) +
                            pow(pd.y*(p0.x-p1.x)-pd.x*(p0.y-p1.y),2);
            num = sqrt(num);
            num = num / denom;

            res = (FCZero(num)) ? VER_ON_ANALYTIC : VER_ON_NO;
        }
        strProp = pDr->GetObjectHelp();
        strTarget = pPt->GetObjectHelp();
    }
    else if (pPt && pPl)
    {
        CVector4 p0 = pPt->Concept_pt;
        CVector4 pl = pPl->GetNormalForm();

        FCoord denom = sqrt(pl.x*pl.x+pl.y*pl.y+pl.z*pl.z);
        if (!FCZero(denom))
        {
            FCoord num = pl.x*p0.x + pl.y*p0.y + pl.z*p0.z + pl.w;
            num = fabsl(num / denom);

            res = (FCZero(num)) ? VER_ON_ANALYTIC : VER_ON_NO;
        }
        strProp = pPl->GetObjectHelp();
        strTarget = pPt->GetObjectHelp();
    }

    strMsg.LoadString(res);
    strMsg2.Format(strMsg,strTarget,strProp);

    m_pVerifyDlg->m_strResult = strMsg2;
    m_pVerifyDlg->m_bShowCE = (res==VER_ON_ANALYTIC);
    m_pVerifyDlg->UpdateData(FALSE);
    m_pVerifyDlg->ShowWindow(SW_SHOW);*/

    return TRUE;
}

/*BOOL CVerify3DTask::OnVerifyProp()
{
    BOOL bRes = FALSE;
    int nb = m_cObjectTarget.GetSize();
    switch (m_nCurrTask){
    case ID_EXPLORATION_VERIFY_PAR :
    case ID_EXPLORATION_VERIFY_PERP:
    case ID_EXPLORATION_VERIFY_PTON:
        bRes = (nb==2);
        break;
    case ID_EXPLORATION_VERIFY_PTALIGN:
        bRes = (nb==3);
        break;
    default:
        break;
    }
    if (!bRes) return FALSE;

    CxObject3DSet mSet;
    CVector4 tempPt[4] = {  CVector4(POSVER,0,0),
                            CVector4(0,POSVER,0),
                            CVector4(0,0,POSVER),
                            CVector4(POSVER,POSVER,POSVER)};

    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = m_cObjectTarget.GetAt(i);
        if (!pObj) return FALSE;

        GetFreePoint(pObj,mSet);
    }

    UINT res = VER_ERROR;

    //*******************
    switch (m_nCurrTask){
    case ID_EXPLORATION_VERIFY_PAR :
        {
            res = m_cObjectTarget.GetAt(0)->IsParallelTo(m_cObjectTarget.GetAt(1));
        }
        break;
    case ID_EXPLORATION_VERIFY_PERP:
        {
            res = m_cObjectTarget.GetAt(0)->IsPerpendicularTo(m_cObjectTarget.GetAt(1));
        }
        break;
    case ID_EXPLORATION_VERIFY_PTON:
        {
            FCoord dis = 0.0;
            CPoint3D *pPt = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(0));
            BOOL bRet = pPt->GetDistanceFrom(m_cObjectTarget.GetAt(1),dis);
            if (!bRet) return FALSE;
            res = (FCZero(dis)) ? VER_ON_ANALYTIC : VER_ON_NO;
        }
        break;
    case ID_EXPLORATION_VERIFY_PTALIGN:
        {
            CPoint3D *pPt1 = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(0));
            CPoint3D *pPt2 = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(1));
            CPoint3D *pPt3 = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(2));
            res = pPt1->IsAlignedWith(pPt2,pPt3);
        }
        break;
    default:
        return FALSE;
    }
    //*******************

    if (res == VER_ALIGN_ANALYTIC)
    {
        res=VER_ALIGN_CONSTRUCTION;

        for (int i=0;i<mSet.GetSize();i++)
        {
            if (res != VER_ALIGN_CONSTRUCTION) break;
            CPoint3D *pFree = DYNAMIC_DOWNCAST(CPoint3D,mSet.GetAt(i));
            if (!pFree) continue;

            CVector4 ptOrig = pFree->Concept_pt;

            CxObject3DSet mList;
            pFree->GetDependList(&mList);

            for (int j=0;j<4;j++)
            {
                CVector4 ptt = ptOrig + tempPt[j];

                pFree->Concept_pt = ptt;
                for (int k=0;k<mList.GetSize();k++)
                {
                    CObject3D *pObj = mList.GetAt(k);
                    if (!pObj) continue;
                    pObj->CalculConceptuel();
                }

                UINT res2 = VER_ERROR;

                //***********
                switch (m_nCurrTask){
                case ID_EXPLORATION_VERIFY_PAR :
                    {
                        res2 = m_cObjectTarget.GetAt(0)->IsParallelTo(m_cObjectTarget.GetAt(1));
                    }
                    break;
                case ID_EXPLORATION_VERIFY_PERP:
                    {
                        res2 = m_cObjectTarget.GetAt(0)->IsPerpendicularTo(m_cObjectTarget.GetAt(1));
                    }
                    break;
                case ID_EXPLORATION_VERIFY_PTON:
                    {
                        FCoord dis = 0.0;
                        CPoint3D *pPt = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(0));
                        BOOL bRet = pPt->GetDistanceFrom(m_cObjectTarget.GetAt(1),dis);
                        if (!bRet)
                            res2 = VER_ON_ANALYTIC;
                        else
                            res2 = (!FCZero(dis)) ? VER_ON_NO : VER_ON_ANALYTIC;
                    }
                    break;
                case ID_EXPLORATION_VERIFY_PTALIGN:
                    {
                        CPoint3D *pPt1 = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(0));
                        CPoint3D *pPt2 = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(1));
                        CPoint3D *pPt3 = DYNAMIC_DOWNCAST(CPoint3D,m_cObjectTarget.GetAt(2));
                        res2 = pPt1->IsAlignedWith(pPt2,pPt3);
                    }
                    break;
                default:
                    return FALSE;
                }
                //***********

                if (res2==VER_ALIGN_NO)
                {
                    res=VER_ALIGN_ANALYTIC;

                    m_pVerifyDlg->m_pObjFree = pFree;
                    m_pVerifyDlg->m_vNewPt = ptt;
                    m_pVerifyDlg->m_vOldPt = ptOrig;
                    m_pVerifyDlg->m_bIsCE=FALSE;

                    break;
                }
            }
            pFree->Concept_pt = ptOrig;
            for (int k=0;k<mList.GetSize();k++)
            {
                CObject3D *pObj = mList.GetAt(k);
                if (!pObj) continue;
                pObj->CalculConceptuel();
            }
        }
    }

    CString strMsg2,strMsg,strProp,strProp2,strTarget;
    strMsg.LoadString(res);

    //***********
    switch (m_nCurrTask){
    case ID_EXPLORATION_VERIFY_PAR :
        {
            strProp.LoadString(VER_PAR);
            strTarget = m_cObjectTarget.GetAt(0)->GetObjectHelp();
            strProp2 = m_cObjectTarget.GetAt(1)->GetObjectHelp();
            strMsg2.Format(strMsg,strTarget,strProp2,strProp);
        }
        break;
    case ID_EXPLORATION_VERIFY_PERP:
        {
            strProp.LoadString(VER_PERP);
            strTarget = m_cObjectTarget.GetAt(0)->GetObjectHelp();
            strProp2 = m_cObjectTarget.GetAt(1)->GetObjectHelp();
            strMsg2.Format(strMsg,strTarget,strProp2,strProp);
        }
        break;
    case ID_EXPLORATION_VERIFY_PTON:
        {
            strTarget = m_cObjectTarget.GetAt(0)->GetObjectHelp();
            strProp = m_cObjectTarget.GetAt(1)->GetObjectHelp();
            strMsg2.Format(strMsg,strTarget,strProp);
        }
        break;
    case ID_EXPLORATION_VERIFY_PTALIGN:
        {
            strTarget = m_cObjectTarget.GetAt(0)->GetObjectHelp();
            strProp = m_cObjectTarget.GetAt(1)->GetObjectHelp();
            strProp2 = m_cObjectTarget.GetAt(2)->GetObjectHelp();
            strMsg2.Format(strMsg,strTarget,strProp,strProp2);
        }
        break;
    default:
        return FALSE;
    }
    //***********

    m_pVerifyDlg->m_strResult = strMsg2;
    m_pVerifyDlg->m_bShowCE = (res==VER_ALIGN_ANALYTIC);
    m_pVerifyDlg->UpdateData(FALSE);
    m_pVerifyDlg->ShowWindow(SW_SHOW);

    return TRUE;
}*/


BOOL CVerify3DTask::OnVerifyAlign(CObject3D *pOPt,CObject3D *pObj2,CObject3D *pObj3)
{
    UINT res = VER_ERROR;
    CPoint3D *pPt1 = DYNAMIC_DOWNCAST(CPoint3D,pOPt);
    CPoint3D *pPt2 = DYNAMIC_DOWNCAST(CPoint3D,pObj2);
    CPoint3D *pPt3 = DYNAMIC_DOWNCAST(CPoint3D,pObj3);

    if (!pPt1 || !pPt2 || !pPt3) return FALSE;

    CString strMsg2,strMsg,strProp,strProp2,strTarget;

    //*******************
    res = pPt1->IsAlignedWith(pPt2,pPt3);
    //*******************

    if (res == VER_ALIGN_ANALYTIC)
    {
        CxObject3DSet mSet;
        GetFreePoint(pPt1,mSet);
        GetFreePoint(pPt2,mSet);
        GetFreePoint(pPt3,mSet);

        CVector4 tempPt[4] = {  CVector4(POSVER,0,0),
                                CVector4(0,POSVER,0),
                                CVector4(0,0,POSVER),
                                CVector4(POSVER,POSVER,POSVER)};

        res=VER_ALIGN_CONSTRUCTION;

        for (int i=0;i<mSet.GetSize();i++)
        {
            if (res != VER_ALIGN_CONSTRUCTION) break;
            CPoint3D *pFree = DYNAMIC_DOWNCAST(CPoint3D,mSet.GetAt(i));
            if (!pFree) continue;

            CVector4 ptOrig = pFree->Concept_pt;

            CxObject3DSet mList;
            pFree->GetDependList(&mList);

            for (int j=0;j<4;j++)
            {
                CVector4 ptt = ptOrig + tempPt[j];

                pFree->Concept_pt = ptt;
                for (int k=0;k<mList.GetSize();k++)
                {
                    CObject3D *pObj = mList.GetAt(k);
                    if (!pObj) continue;
                    pObj->CalculConceptuel();
                }

                //***********
                UINT res2 = pPt1->IsAlignedWith(pPt2,pPt3);
                //***********

                if (res2==VER_ALIGN_NO)
                {
                    res=VER_ALIGN_ANALYTIC;

                    m_pVerifyDlg->m_pObjFree = pFree;
                    m_pVerifyDlg->m_vNewPt = ptt;
                    m_pVerifyDlg->m_vOldPt = ptOrig;
                    m_pVerifyDlg->m_bIsCE=FALSE;

                    break;
                }
            }
            pFree->Concept_pt = ptOrig;
            for (int k=0;k<mList.GetSize();k++)
            {
                CObject3D *pObj = mList.GetAt(k);
                if (!pObj) continue;
                pObj->CalculConceptuel();
            }
        }
    }

    strMsg.LoadString(res);
    strTarget = pPt1->GetObjectHelp();
    strProp = pPt2->GetObjectHelp();
    strProp2 = pPt3->GetObjectHelp();
    strMsg2.Format(strMsg,strTarget,strProp,strProp2);

    m_pVerifyDlg->m_strResult = strMsg2;
    m_pVerifyDlg->m_bShowCE = (res==VER_ALIGN_ANALYTIC);
    m_pVerifyDlg->UpdateData(FALSE);
    m_pVerifyDlg->ShowWindow(SW_SHOW);

    return TRUE;
}

void CVerify3DTask::CreateObject3D()
{
    int nb = m_cObjectTarget.GetSize();

    BOOL bRes = FALSE;

    if (nb<2) return;
    CObject3D *pObj1 = m_cObjectTarget.GetAt(0);
    CObject3D *pObj2 = m_cObjectTarget.GetAt(1);

    switch (m_nCurrTask){
    case ID_EXPLORATION_VERIFY_PAR :
    case ID_EXPLORATION_VERIFY_PERP:
        bRes = OnVerifyParallel(pObj1,pObj2);
        break;
    case ID_EXPLORATION_VERIFY_PTON:
        bRes = OnVerifyPointOn(pObj1,pObj2);
        break;
    case ID_EXPLORATION_VERIFY_PTALIGN:
        {
            if (nb!=3) return;
            CObject3D *pObj3 = m_cObjectTarget.GetAt(2);
            bRes = OnVerifyAlign(pObj1,pObj2,pObj3);
        }
        break;
    default:
        break;
    }
    m_nStep=0;
}

void CVerify3DTask::OnMouseL(UINT, CPoint thepos)
{
    if (!m_nStep) return;
    int nb = m_cObjectTarget.GetSize();
    CObject3D *temp;
    int index = FindObject(thepos,GetMask(),FALSE);
    if (!index)
    {
        return;
    }

    temp = GetObject();
    if (!temp) return;

    temp->SetSelected(TRUE);
    GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
    m_pParent->Invalidate(0);

    if (!nb)
    {
        m_cObjectTarget.Add(temp);
        //m_pVerifyDlg->m_pObj1 = temp;
        m_pVerifyDlg->UpdateData(FALSE);
        m_pVerifyDlg->ShowWindow(SW_SHOW);
        m_pVerifyDlg->AddTarget(temp);
        m_nStep++;
    }
    else if (nb==1)
    {
        m_cObjectTarget.Add(temp);
        //m_pVerifyDlg->m_pObj2 = temp;
        m_pVerifyDlg->AddTarget(temp);
        m_pVerifyDlg->UpdateData(FALSE);
        m_pVerifyDlg->ShowWindow(SW_SHOW);
        if (m_nCurrTask!=ID_EXPLORATION_VERIFY_PTALIGN)
            CreateObject3D();
    }
    else if (nb==2)
    {
        m_cObjectTarget.Add(temp);
        //m_pVerifyDlg->m_pObj2 = temp;
        m_pVerifyDlg->AddTarget(temp);
        m_pVerifyDlg->UpdateData(FALSE);
        m_pVerifyDlg->ShowWindow(SW_SHOW);
        CreateObject3D();
    }
    GetContextualHelp();
}

void CVerify3DTask::OnMouseMove(UINT, CPoint thepoint)
{
    if (!m_nStep)
        FindObject(thepoint,0,FALSE);
    else
        FindObject(thepoint,GetMask(),TRUE);
}


////////////////////////////////////////////////////////////////////////
// Definition de CVerify3DTask
////////////////////////////////////////////////////////////////////////

CAnimation3DTask::CAnimation3DTask(CView *AParent,UINT nID): CTask(AParent,nID)
{
    m_pObjAnimated = NULL;
    m_nTimer=0;
}

CAnimation3DTask::~CAnimation3DTask()
{
    CancelTache();
}

void CAnimation3DTask::CancelTache()
{
    m_pList.RemoveAll();
    if (m_nTimer && m_pParent->GetSafeHwnd())
    {
        m_pParent->KillTimer(m_nTimer);//TM_ANIMATION
    }
    if (m_pObjAnimated) m_pObjAnimated->SetSelected(FALSE);
    if (m_pParent->GetSafeHwnd())
    {
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
    m_pObjAnimated = NULL;
    m_nTimer=0;
}

DWORD CAnimation3DTask::GetMask()
{
    DWORD mask = TPointSurD3DClass | TPointSurC3DClass;
        //  | TPoint3DClass |
        //  TPointSurP3DClass | TPointSurS3DClass
        //      );
    return mask;
}

void CAnimation3DTask::OnMouseMove(UINT, CPoint thepos)
{
    if (m_nTimer) return;
    FindObject(thepos,GetMask(),TRUE);
    m_ptOld = thepos;

    if (m_pObjAnimated)
        m_pParent->Invalidate(FALSE);

}




void CAnimation3DTask::OnMouseL(UINT, CPoint thepos)
{
    if (m_pObjAnimated) return;

    //CObject3D *temp;
    int index = FindObject(thepos,GetMask(),FALSE);
    if (!index)
    {
        return;
    }

    CObject3D *temp = GetObject();

    m_pObjAnimated = DYNAMIC_DOWNCAST(CPoint3D,temp);
    if (!m_pObjAnimated) return;

    m_pObjAnimated->SetSelected(TRUE);
    m_pParent->Invalidate(0);
    m_ptOld = thepos;
}

void CAnimation3DTask::OnMouseLUp(UINT, CPoint mpt)
{
    if (!m_pObjAnimated) return;
    CPointSurC3D *pPtSurC = DYNAMIC_DOWNCAST(CPointSurC3D,m_pObjAnimated);
    CPointSurD3D *pPtSurD = DYNAMIC_DOWNCAST(CPointSurD3D,m_pObjAnimated);
    if (!pPtSurC && !pPtSurD) return;

    CSize msize = mpt-m_pObjAnimated->Visual_pt;
    int dd = (int)sqrtl(msize.cx*msize.cx+msize.cy*msize.cy);

    if (dd==0)
        m_nFreq=500;
    else
        m_nFreq=(int)(1000./sqrtl(dd*1.0));
    CreateObject3D();
}

void CAnimation3DTask::DrawFeedBack(CDC *pDC)
{
    if (!m_pObjAnimated) return;
    if (m_nTimer) return;
//  pDC->MoveTo(pPtSurC->Visual_pt);
//  pDC->LineTo(m_ptOld);

    CSize ms = m_ptOld - m_pObjAnimated->Visual_pt;
    CSize ms2 = ms;
    ms2.cx /=15;
    ms2.cy /=15;

    CSize ms3;
    ms3.cx = -ms.cy;
    ms3.cy = ms.cx;

    double ddd= sqrtl(ms3.cx*ms3.cx+ms3.cy*ms3.cy);
    double a = 12*ms3.cx / ddd;
    double b = 12*ms3.cy / ddd;

    CPoint pt[17];

    pt[0] = m_ptOld;
    pt[16] = m_pObjAnimated->Visual_pt;
    pt[8].x = (pt[0].x+pt[16].x)/2;
    pt[8].y = (pt[0].y+pt[16].y)/2;
    pt[4].x = (pt[0].x+pt[8].x)/2;
    pt[4].y = (pt[0].y+pt[8].y)/2;
    pt[2].x = (pt[0].x+pt[4].x)/2;
    pt[2].y = (pt[0].y+pt[4].y)/2;
    pt[6].x = (pt[8].x+pt[4].x)/2;
    pt[6].y = (pt[8].y+pt[4].y)/2;
    pt[12].x = (pt[8].x+pt[16].x)/2;
    pt[12].y = (pt[8].y+pt[16].y)/2;
    pt[14].x = (pt[12].x+pt[16].x)/2;
    pt[14].y = (pt[12].y+pt[16].y)/2;
    pt[10].x = (pt[12].x+pt[8].x)/2;
    pt[10].y = (pt[12].y+pt[8].y)/2;

    pt[1].x = (pt[0].x+pt[2].x)/2;
    pt[1].y = (pt[0].y+pt[2].y)/2;
    pt[3].x = (pt[2].x+pt[4].x)/2;
    pt[3].y = (pt[2].y+pt[4].y)/2;
    pt[5].x = (pt[6].x+pt[4].x)/2;
    pt[5].y = (pt[6].y+pt[4].y)/2;
    pt[7].x = (pt[6].x+pt[8].x)/2;
    pt[7].y = (pt[6].y+pt[8].y)/2;
    pt[9].x = (pt[10].x+pt[8].x)/2;
    pt[9].y = (pt[10].y+pt[8].y)/2;
    pt[11].x = (pt[10].x+pt[12].x)/2;
    pt[11].y = (pt[10].y+pt[12].y)/2;
    pt[13].x = (pt[14].x+pt[12].x)/2;
    pt[13].y = (pt[14].y+pt[12].y)/2;
    pt[15].x = (pt[14].x+pt[16].x)/2;
    pt[15].y = (pt[14].y+pt[16].y)/2;

/*  for (int i=1;i<16;i++)
    {
        pt[i].x = m_ptOld.x - i*ms2.cx;
        pt[i].y = m_ptOld.y - i*ms2.cy;
    }*/

    for (int i=1;i<16;i++)
    {
        if (i%2==0)
        {
            pt[i].x += (int)a;
            pt[i].y += (int)b;
        }
        else
        {
            pt[i].x -= (int)a;
            pt[i].y -= (int)b;
        }
    }

    pDC->MoveTo(m_ptOld);
    for ( i=0;i<17;i++)
    {
        pDC->LineTo(pt[i]);
        //CRect mrect(pt[i],pt[i]);
        //mrect.InflateRect(2,2);
        //pDC->Ellipse(mrect);
    }
    pDC->LineTo(m_pObjAnimated->Visual_pt);

}


void CAnimation3DTask::CreateObject3D()
{
    CPointSurC3D *pPtSurC = DYNAMIC_DOWNCAST(CPointSurC3D,m_pObjAnimated);
    CPointSurD3D *pPtSurD = DYNAMIC_DOWNCAST(CPointSurD3D,m_pObjAnimated);
    if (!pPtSurC && !pPtSurD)
    {
        CancelTache();
        return;
    }

    if (m_pObjAnimated->pComposite)
        m_pObjAnimated->pComposite->GetDependList(&m_pList);
    else
        m_pObjAnimated->GetDependList(&m_pList);
    int nb = m_pList.GetSize();

    m_nDeltaT = 20;
    int ddd = 0;

    if (pPtSurC)
    {
        m_nOldN = pPtSurC->lambda;
            ddd = (int)((m_nOldN * (FCoord)(m_nDeltaT-1))/pPtSurC->S->nArcAngle);
    }
    else
    {
        m_nOldN = pPtSurD->lambda;
        if (pPtSurD->S->bIsSegment)
            ddd = (int)(pPtSurD->lambda  * (FCoord)(m_nDeltaT-1));
        else
        {
            CVector4 pDirV =pPtSurD->S->GetDirVector();
            FCoord pVecN = pDirV.Norme();
            ddd = m_nDeltaT/2 + (pPtSurD->lambda -  m_nOldN) / (FCoord)(m_nDeltaT-1);//*pDirV;
        }
    }

    m_nCurrD = ddd;
    m_bInc = TRUE;
    m_nTimer = TM_ANIMATION;
    m_pParent->SetTimer(TM_ANIMATION,(int)m_nFreq,NULL);
}

void CAnimation3DTask::OnTimer(UINT nIDEvent)
{
    if (!m_nTimer || m_nTimer!=TM_ANIMATION) return;
    if (!m_pObjAnimated) return;

    if (m_bInc)
        m_nCurrD++;
    else
        m_nCurrD--;

    if (m_nCurrD>=m_nDeltaT && m_bInc)
    {
        m_nCurrD=m_nDeltaT-1;
        m_bInc = !m_bInc;

    }
    else if (m_nCurrD==0 && !m_bInc)
    {
        m_nCurrD=0;
        m_bInc = !m_bInc;

    }

    CPointSurC3D *pPtSurC = DYNAMIC_DOWNCAST(CPointSurC3D,m_pObjAnimated);
    CPointSurD3D *pPtSurD = DYNAMIC_DOWNCAST(CPointSurD3D,m_pObjAnimated);

    if (!pPtSurC && !pPtSurD)
    {
        CancelTache();
        return;
    }

    if (pPtSurC)
        pPtSurC->lambda = (FCoord)(m_nCurrD * pPtSurC->S->nArcAngle)/(FCoord)(m_nDeltaT-1);
    else
    {

        if (pPtSurD->S->bIsSegment)
            pPtSurD->lambda = (FCoord)m_nCurrD/(FCoord)(m_nDeltaT-1);
        else
        {
            CVector4 pDirV =pPtSurD->S->GetDirVector();
            FCoord pVecN = pDirV.Norme();
            pPtSurD->lambda = m_nOldN + (FCoord)(m_nCurrD-m_nDeltaT/2)*(FCoord)(m_nDeltaT-1);//*pDirV;
        }
}


    for (int j=0;j<m_pList.GetSize();j++)
    {
        ///CObject3D *pObj = pList.GetAt(j);
        CObject3D *pObj = m_pList.GetAt(j);
        if (!pObj) continue;
        UINT res = pObj->CalculConceptuel();
        //pObj->CalculVisuel();
    }
    GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_MOV,m_pObjAnimated);
    InvalidateParent(TRUE);
    //m_pParent->Invalidate();
    //m_pParent->UpdateWindow();
}


////////////////////////////////////////////////////////////////////////
// Definition de CPolygonTask
////////////////////////////////////////////////////////////////////////

CPolygonTask::CPolygonTask(CView *AParent,UINT nID)  : CTask(AParent,nID)
{
    CancelTache();
}


CPolygonTask::~CPolygonTask()
{
    CancelTache();
}

void CPolygonTask::CancelTache()
{
    m_nStep = 0;
    for (int i=0;i<m_cObjectsTarget.GetSize();i++)
    {
        CObject3D *pObj = m_cObjectsTarget.GetAt(i);
        if (!pObj) continue;
        pObj->SetSelected(FALSE);
    }
    if (m_pParent->GetSafeHwnd())
    {
        InvalidateParent(TRUE);
        //m_pParent->Invalidate();
        //m_pParent->UpdateWindow();
    }
    m_cObjectsTarget.RemoveAll();
}

unsigned CPolygonTask::GetHelpResID()
{
    return CTX_SELECT_POINT1;
}

DWORD CPolygonTask::GetMask()
{
    return TAllPointClass;
}

void CPolygonTask::OnMouseL(UINT, CPoint thepos)
{
    int nb = m_cObjectsTarget.GetSize();

    CObject3D *temp;
    int index = FindObject(thepos,GetMask(),FALSE);
    if (!index)
    {
        if (!nb)
            GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,NULL);
        return;
    }
    temp = GetObject();
    if (!temp) return;

    temp->SetSelected(TRUE);
    GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
    m_pParent->Invalidate(0);

    if (!nb)
    {
        m_cObjectsTarget.Add(temp);
    }
    else
    {
        CObject3D* pStart = m_cObjectsTarget.GetAt(0);
        if (temp == pStart && nb < 3)
        {
            ShowErrorMsg(ERR_2SAMEPTS);
            return;
        }
        else if (temp == pStart && nb >= 3)
        {
            CreateObject3D();
        }
        else
        {
            for (int i=1;i<nb;i++)
            {
                CObject3D *pObj = m_cObjectsTarget.GetAt(i);
                if (temp==pObj)
                {
                    ShowErrorMsg(ERR_2SAMEPTS);
                    return;
                }
            }
            if (nb >= 3)
            {
                CPoint3D *pA = (CPoint3D *)pStart;
                CPoint3D *pB = (CPoint3D *)m_cObjectsTarget.GetAt(1);
                CPoint3D *pC = (CPoint3D *)m_cObjectsTarget.GetAt(2);

                CVector4 U = pB->Concept_pt - pA->Concept_pt;
                CVector4 V = pC->Concept_pt - pA->Concept_pt;
                CVector4 UvV = U % V;
                UvV = UvV.Normalized();

                CPoint3D *pD = (CPoint3D*)temp;
                CVector4 W = pD->Concept_pt - pA->Concept_pt;
                CVector4 UvW = U % W;
                CVector4 VvW = V % W;
                UvW = UvW.Normalized();
                VvW = VvW.Normalized();

                CVector4 UvW2 = UvV % UvW;
                CVector4 UvV2 = UvV % VvW;

                if (!UvW2.NullVector() || !UvV2.NullVector())
                {
                    ShowErrorMsg(ERR_POLYGON_NOPLANE);
                    temp->SetSelected(FALSE);
                    m_pParent->Invalidate(0);
                    return;
                }
            }
            m_cObjectsTarget.Add(temp);
        }
    }
}

void CPolygonTask::OnMouseMove(UINT, CPoint thepoint)
{
    FindObject(thepoint,GetMask(),TRUE);
    m_ptOld = thepoint;
    if (m_pParent->GetSafeHwnd())
        m_pParent->Invalidate(FALSE);

}

void CPolygonTask::CreateObject3D()
{
    int nb = m_cObjectsTarget.GetSize();
    if (nb <3) return;

    CPolygon3D* temp = new CPolygon3D(&m_cObjectsTarget);

    UINT res = temp->CalculConceptuel();
    if (res)
    {
        temp->HandleObjectError(res,TRUE);
        delete temp;
    }
    else
        PrepareAddedObject(temp);

    CancelTache();
}


void CPolygonTask::DrawFeedBack(CDC *pDC)
{
    CPoint mpoly[20];
    int nb = m_cObjectsTarget.GetSize();
    for (int i=0;i<nb;i++)
    {
        CPoint3D* temp = (CPoint3D*)m_cObjectsTarget.GetAt(i);
        CPoint pt  = temp->Visual_pt;
        mpoly[i] = pt;
    }
    mpoly[nb] = m_ptOld;

    CBrush pBrush;
    CPen mPen(PS_INSIDEFRAME,1,RGB(192,192,192));
    pBrush.CreateHatchBrush(HS_CROSS,RGB(192,192,192));

    int oldM = pDC->SetBkMode(TRANSPARENT);
    //COLORREF oldB = pDC->SetBkColor(RGB(0,255,0));
    CBrush *pOld = pDC->SelectObject(&pBrush);
    CPen *pOldp = pDC->SelectObject(&mPen);
    //COLORREF oldC = pDC->SetTextColor(RGB(255,0,0));
    pDC->Polygon(mpoly,nb+1);
    //pDC->SetTextColor(oldC);
    pDC->SelectObject(pOldp);
    pDC->SelectObject(pOld);
    //pDC->SetBkColor(oldB);
    pDC->SetBkMode(oldM);
//  bmp.DeleteObject();
}

////////////////////////////////////////////////////////////////////////
// Definition de TAmbigList
////////////////////////////////////////////////////////////////////////
CCenterOn3DTask::CCenterOn3DTask(CView *AParent,UINT taskID) : CTask(AParent,taskID)
{
    ptA = NULL;
    GetContextualHelp();

}

CCenterOn3DTask::~CCenterOn3DTask()
{
    if (ptA)    ptA->SetSelected(FALSE);
    //if (ptA && m_pParent && m_pParent->GetSafeHwnd()) m_pParent->Invalidate();
    if (ptA)
        InvalidateParent();

}

unsigned CCenterOn3DTask::GetHelpResID()
{
    return CTX_SELECT_POINT1;
}

DWORD CCenterOn3DTask::GetMask()
{
    DWORD mask = TAllPointClass;
    return mask;
}

void CCenterOn3DTask::OnMouseL(UINT, CPoint thepos)
{
    CObject3D *temp;
    int index = FindObject(thepos,GetMask(),FALSE);
    if (!index) return;

    temp = GetObject();
    if (!temp) return;
    temp->SetSelected(TRUE);//bIsSelected = 1;
    GetDocument()->UpdateAllViews(m_pParent,WM_UPDATEOBJ_SEL,temp);
    m_pParent->Invalidate(0);

    ptA = (CPoint3D*)temp;
    CreateObject3D();
}

void CCenterOn3DTask::OnMouseMove(UINT, CPoint thepos)
{
    FindObject(thepos,GetMask(),TRUE);
}

void CCenterOn3DTask::CreateObject3D()
{
    if (!ptA) return;

    CVector4 ptNewOrg = -(ptA->Concept_pt);

    int nb = GetDocument()->m_cObjectSet.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D * pObj = GetDocument()->m_cObjectSet.GetAt(i);
        //if (!pObj->MaskObject(TPoint3DClass)) continue;
        //CPoint3D* pPt = DYNAMIC_DOWNCAST(CPoint3D,pObj);
        if (!pObj) continue;
        pObj->TranslateBy(ptNewOrg);
        //CVector4 ptA = pPt->Concept_pt;
        //ptA = ptA + ptNewOrg;
        //pPt->Concept_pt = ptA;
    }
    for (i=0;i<nb;i++)
    {
        CObject3D * pObj = GetDocument()->m_cObjectSet.GetAt(i);
        if (!pObj) continue;

        pObj->CalculConceptuel();
    }
    GetContextualHelp();
    if (ptA)    ptA->SetSelected(FALSE);
    ptA = NULL;
    InvalidateParent();
    //m_pParent->Invalidate();
}
