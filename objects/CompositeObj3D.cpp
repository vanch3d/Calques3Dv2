// CompositeObj3D.cpp: implementation of the CCompositeObj3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "CompositeObj3D.h"

#include "..\Prefs\Prefs.h"
#include "ObjectPropPage.h"
#include "..\HelpPrSheet.h"

#include "Point3D.h"
#include "Plan3D.h"
#include "Droite3D.h"
#include "Cercle3D.h"
#include "Sphere3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCompositeObj3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CCompositeObj3D::CCompositeObj3D() : CObject3D()
{
    m_cSubObjects.RemoveAll();
    nStartShow = 0;
    m_bExpandGraph = FALSE;
}

CCompositeObj3D::CCompositeObj3D(const CObject3D & src): CObject3D(src)
{
    m_bExpandGraph = FALSE;
    m_cSubObjects.RemoveAll();

    int nb = ((CCompositeObj3D &)src).m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = ((CCompositeObj3D &)src).m_cSubObjects.GetAt(i);
        if (!obj) continue;
        CObject3D * obj2 = obj->CopyObject();
        m_cSubObjects.Add(obj2);
     }
    nStartShow = ((CCompositeObj3D &)src).nStartShow;
}

void CCompositeObj3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << nStartShow;
    }
    else
    {
        ar >> nStartShow;
        m_bExpandGraph = FALSE;
    }
    m_cSubObjects.Serialize(ar);
    if (ar.IsLoading())
    {
        for (int i=0;i<m_cSubObjects.GetSize();i++)
        {
            CObject3D *pObj = m_cSubObjects.GetAt(i);
            if (pObj) pObj->SetInGraph();
        }
    }
}


CCompositeObj3D::~CCompositeObj3D()
{
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        delete obj;
     }
    m_cSubObjects.RemoveAll();
}


CObject3D* CCompositeObj3D::CopyObject()
{
    CObject3D *temp = new CCompositeObj3D((CObject3D&)*this);
    return temp;
}

BOOL CCompositeObj3D::IsInActiveArea(CPoint thePt)
{
    BOOL bRet = FALSE;
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;(i<nb) && !bRet;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (obj->bValidate && obj->bVisible)
            bRet = obj->IsInActiveArea(thePt);
     }
    return bRet;
}

//////////////////////////////////////////////////////////////////////
/// Get the polygons defined in the object.
/// This virtual method is for ensuring the detection of polygons embedded in composite object.
/// \param pList    A pointer to the object list to fill in.
//////////////////////////////////////////////////////////////////////
void CCompositeObj3D::GetPolygons(CxObject3DSet* pList)
{
    if (!pList) return;
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = m_cSubObjects.GetAt(i);
        if (!pObj) continue;
		pObj->GetPolygons(pList);
	}
}

void CCompositeObj3D::GetDependList(CxObject3DSet* pList,BOOL bAll)
{
    if (!pList) return;
    TRACE("CCompositeObj3D::GetDependList : %s\n",GetObjectDef());

    CObject3D::GetDependList(pList,bAll);

    if (!bAll) return;

    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = m_cSubObjects.GetAt(i);
        if (!pObj) continue;
        TRACE("\t\tm_cSubObjects : %s\n",pObj->GetObjectDef());

        int nb2 = pObj->cDependList.GetSize();
        pObj->AddObjToDependList(pList);
        for (int j=0;j<nb2;j++)
        {
            CObject3D *pObj2 = pObj->cDependList.GetAt(j);
            if (!pObj2) continue;
            if (pObj2->pComposite == this) continue;
            if (bAll==2)
            {
                if (pObj2->pComposite)
                    pObj2->pComposite->AddObjToDependList(pList);
                else
                    pObj2->AddObjToDependList(pList);
                /*CxObject3DSet pTempList;
                pObj2->GetDependList(&pTempList,2);
                for (int j=0;j<pTempList.GetSize();j++)
                {
                    CObject3D *pObjTT = pTempList.GetAt(j);
                    if (pObjTT)
                        pList->Add(pObjTT);

                }*/
            }
            else
                pObj2->GetDependList(pList);
        }
    }

/*  nb = cDependList.GetSize();
    for (i=0;i<nb;i++)
    {
        CObject3D *pObj = cDependList.GetAt(i);
        if (!pObj) continue;
        pObj->GetDependList(pList);
    }*/
}


BOOL CCompositeObj3D::IsInCalque(int CalcNum)
{
//  BOOL bTest = CObject3D::IsInCalque(CalcNum);
//  return bTest;

    BOOL bRet = FALSE;
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;(i<nb) && !bRet;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (obj->bValidate && obj->bVisible)
            bRet = obj->IsInCalque(CalcNum);
     }
    return bRet;
}

void CCompositeObj3D::SetVisible(BOOL bVis)
{
    CObject3D::SetVisible(bVis);
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;i<nb;i++)
    {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        if (obj->bNotInHisto) continue;
        //obj->bVisible = bVis;
        obj->SetVisible(bVis);
    }
}

void CCompositeObj3D::SetInGraph(BOOL bAdd)
{
    CObject3D::SetInGraph(bAdd);

    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObjDep = m_cSubObjects.GetAt(i);
        if (!pObjDep) continue;
        pObjDep->SetInGraph(bAdd);
    }
}


BOOL CCompositeObj3D::IsVisible()
{
    BOOL bVis = 0;
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;i<nb;i++)
    {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        if (obj->bNotInHisto) continue;
        if (i==nStartShow)
            bVis = obj->IsVisible();//bVisible;
        else
        {
            if (obj->IsVisible() != bVis)
            {
                bVis = 2;
                break;
            }
        }
    }
    //bVisible = bVis;
    return bVis;

}


BOOL CCompositeObj3D::AddInCalque(int CalcNum,BOOL add)
{
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (obj) obj->AddInCalque(CalcNum,add);
        //if (add)  // ajoute dans le calque CalcNum
        //  obj->nCalque |= (int)pow(2,CalcNum);
        //else      // retire du calque CalcNum
        //  obj->nCalque &= ~((int)pow(2,CalcNum));
     }
    return CObject3D::AddInCalque(CalcNum,add);
}

CRgn* CCompositeObj3D::InvalideRect()
{
    return NULL;
}

int CCompositeObj3D::ValideCompositeObject(int nErr)
{
    BOOL bVal = !((BOOL)nErr);
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        obj->bValidate = bVal;
     }
    return nErr;
}

void CCompositeObj3D::TranslateBy(CVector4 ptVec)
{
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        obj->TranslateBy(ptVec);
    }
}


CObject3D* CCompositeObj3D::HitTest(CPoint pt,UINT mask,int nCalcNum,BOOL bSub,CxObject3DSet* pSet)
{
//  if (!bSub) return CObject3D::HitTest(pt,mask,nCalcNum,bSub,pSet);

    CObject3D *pObj=NULL;
    CxObject3DSet myLocalSet;

    if (!bSub)
    {
        return CObject3D::HitTest(pt,mask,nCalcNum,bSub,pSet);
    }

    pObj = CObject3D::HitTest(pt,mask,nCalcNum,bSub,pSet);
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        pObj = obj->HitTest(pt,mask,nCalcNum,bSub,&myLocalSet);
    }
    int nbLoc = myLocalSet.GetSize();
    if (bSub)
    {
        for (int i=0;i<nbLoc;i++)
        {
            pSet->Add(myLocalSet.GetAt(i));
        }
    }
    else
    {
        if (nbLoc)
        {
            pObj = this;
            pSet->Add(this);
        }
    }
    return pObj;
}

UINT  CCompositeObj3D::CalculConceptuel()
{
    int ret=0;
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        UINT temp = obj->CalculConceptuel();
        obj->pComposite = this;
        if (temp)
        {
            ret = temp;
        }
     }
    return ret;
}

void CCompositeObj3D::CalculVisuel(CVisualParam *myVisuParam)
{
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        obj->CalculVisuel(myVisuParam);
        //if (rGraphRect == CRect(-1,-1,-1,-1))
            obj->rGraphRect = rGraphRect;
     }

}

void CCompositeObj3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSM)
{
    if ((!IsVisible()) || (!bValidate)) return;
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (DYNAMIC_DOWNCAST(CPolygon3D,obj)) continue;

        obj->Draw(pDC,vp,bSM);
     }

}

void CCompositeObj3D::DrawSelected(CDC* pDC,CVisualParam *vp)
{
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;i<nb;i++)
    {
        CObject3D * pObj = m_cSubObjects.GetAt(i);
        if (pObj) pObj->DrawSelected(pDC,vp);
    }
}



void CCompositeObj3D::ClearHistorique()
{
    pHistItem = NULL;
    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;i<nb;i++)
    {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (obj) obj->ClearHistorique();
    }
}


HTREEITEM CCompositeObj3D::DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent)
{
    HTREEITEM pHItem = NULL;
    if (!mListCtrl) return NULL;

    pHItem = CObject3D::DrawHistorique(mListCtrl,pParent);
    if (!pHItem) return NULL;

    int nb = m_cSubObjects.GetSize();
    for (int i=nStartShow;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        obj->DrawHistorique(mListCtrl,pHItem);
    }
    return pHItem;
}


void CCompositeObj3D::DrawDepGraph(CDC* pDC,CImageList *pImgList,int nTrace,BOOL bDrawNode,BOOL bDrawLink)
{
    CObject3D::DrawDepGraph(pDC,pImgList,nTrace,bDrawNode,bDrawLink);

    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D* pPar = m_cSubObjects.GetAt(i);
        if (!pPar) continue;
        pPar->rGraphRect = rGraphRect;
        pPar->bDrawInGraph = FALSE;

        pPar->DrawDepGraph(pDC,pImgList,nTrace,FALSE,bDrawLink);
    }

}


void CCompositeObj3D::HandleObjectError(int nErr,BOOL bShow)
{
    CObject3D::HandleObjectError(nErr,bShow);
}

int CCompositeObj3D::SetObjectID(int nID)
{
    int nb = CObject3D::SetObjectID(nID);
    nb++;
    int nObj = m_cSubObjects.GetSize();
    for (int i=0;i<nObj;i++)
    {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        nb = obj->SetObjectID(nb);
        nb++;
    }
    nb--;
    return nb;
}

void CCompositeObj3D::SetAvailHisto()
{
    CObject3D::SetAvailHisto();
    int nObj = m_cSubObjects.GetSize();
    for (int i=0;i<nObj;i++)
    {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        obj->SetAvailHisto();
    }
}


int CCompositeObj3D::UpdateID(int nIdx)
{
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
     {
        CObject3D * obj = m_cSubObjects.GetAt(i);
        if (!obj) continue;
        obj->nObjectId = ++(nIdx);
     }
     return nIdx;
}

int CCompositeObj3D::SetProperties(CxObject3DSet *pSet)
{
    CString mstr = GetObjectName();

    //CString strDlg;
    //strDlg.LoadString(AFX_IDS_OBJPROP);

    CHelpPrSheet pSheet(AFX_IDS_OBJPROP);
    CObjectPropPage pPage;
    CObjectInfoPage pPage2;
    CObjectDepPage pPage3;
    CObjectCpntPage pPage4;

    ////// Set Object Property Page
    BOOL bViss = IsVisible();
    pPage.m_bHidden = bViss;
//  pPage.m_bHidden = IsVisible();
    if (pPage.m_bHidden!=2)
        pPage.m_bHidden = !pPage.m_bHidden;
    pPage.m_bMarked = bMarked;
    //pPage.m_nShape = pObjectShape.nShapeId;

    //pPage.m_cShapePick.SetType(pObjectShape.GetShapeType());
    //pPage.m_cShapePick.SetShape(pObjectShape.nShapeId);
	pPage.m_cShape.SetShape((int)pObjectShape.GetShapeType(),pObjectShape.nShapeId);

/*    pPage.m_cStrShape.RemoveAll();
    for (int i=0;i<pObjectShape.GetShapeSize();i++)
    {
        CString mstr = pObjectShape.GetShapeDef(i);
        pPage.m_cStrShape.Add(mstr);
    }*/
    pPage.m_strName = GetObjectName();

    CString mstr2;
    mstr2.LoadString(GetNameID());
    pPage.m_strDefName.Format("%s%d",mstr2,nObjectId);

    pPage.m_clrObj = pObjectShape.clrObject;
    pPage.m_clrDefObj = GetDefaultColor();//pObjectShape.clrObject;
    //pPage.m_strDepth.Format(_T("%d"),nDepth);

    ////// Set Object Information Page
    pPage2.m_strObjDef = GetObjectDef();
    pPage2.m_strObjHelp = GetObjectHelp();

//  if (MaskObject(TMacro3DClass))
//  {
//      pPage2.m_bModif = TRUE;
//  }


    ////// Set Object Dependencies Page
    pPage3.m_pList = &cDependList;
    CxObject3DSet pMyList;
    CxObject3DSet pMyList2;
    GetDependList(&pMyList,FALSE);
    for (int jj=0;jj<pMyList.GetSize();jj++)
    {
        CObject3D *pObj = pMyList.GetAt(jj);
        if (!pObj) continue;
        if (pObj == this) continue;

        if (!pObj->pComposite)
            pMyList2.Add(pObj);
    }
    pPage3.m_pList = &pMyList2;

    ////// Set Object Dependencies Page
    pPage4.m_pList = &m_cSubObjects;

    pSheet.AddPage(&pPage);
    pSheet.AddPage(&pPage2);
    pSheet.AddPage(&pPage3);
    pSheet.AddPage(&pPage4);
    int nRet = pSheet.DoModal();
    if (nRet == IDOK)
    {
        if (pPage.m_bHidden != 2)
            SetVisible(!(pPage.m_bHidden));
        bMarked = pPage.m_bMarked;
        SetColor(pPage.m_clrObj);
        //int nn = pPage.m_cShapePick.GetShape();
		int nn = pPage.m_cShape.GetShape();
        SetStyle(nn);
        //SetStyle(pPage.m_nShape);
        SetName(pPage.m_strName);


//      if (MaskObject(TMacro3DClass))
//      {
//          strObjectHelp = pPage2.m_strObjHelp;
//          strObjectDef = pPage2.m_strObjDef;
//      }
    }
    return (nRet == IDOK);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCube3D, CCompositeObj3D, VERSIONABLE_SCHEMA | 1)

CCube3D::CCube3D() : CCompositeObj3D()
{
    ptA = ptB = NULL;
    cr=NULL;
    m_nAlpha = 0.0;
    pObjectShape = CShape(CShape::VolShape,TPref::TCube.nShape);
    pObjectShape.clrObject = RGB(0,0,255);
    for (int i=0;i<8;i++)
        sommet[i]=NULL;
    for (i=0;i<12;i++)
        arete[i]=NULL;
}


CCube3D::CCube3D(CPoint3D* p1,CPoint3D* p2,FCoord nAlpha):
    CCompositeObj3D()
{
    //if (pObjectShape) delete pObjectShape;
    ptA = p1;
    ptB = p2;
    SetDepth(); //nDepth = max(ptA->nDepth,ptB->nDepth)+1;
    m_nAlpha = nAlpha;
    pObjectShape = CShape(CShape::VolShape,TPref::TCube.nShape);
    pObjectShape.clrObject = RGB(0,0,255);
    cr=NULL;
    for (int i=0;i<8;i++)
        sommet[i]=NULL;
    for (i=0;i<12;i++)
        arete[i]=NULL;
    InitCube();
}

BOOL CCube3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (ptA == pOld)
    {
        SetInGraph(FALSE);
        ptA = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (ptB == pOld)
    {
        SetInGraph(FALSE);
        ptB = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}

int CCube3D::SetDepth()
{
    if (ptA && ptB)
        nDepth = max(ptA->nDepth,ptB->nDepth)+1;
    return nDepth;
}


void CCube3D::InitCube(BOOL bLoad)
{
    sommet[0] = ptA;
    sommet[1] = ptB;
    SetDepth();//nDepth = max(ptA->nDepth,ptA->nDepth)+1;
    CVector4 pNorm(10,10,10);
    FCoord   pRay = 150.;
    if (bLoad)
    {
        CObject3D* mmm = m_cSubObjects.GetAt(0);
        cr = (CCercle3D*) mmm;
    }
    else
    {
        cr = new CCercle3D(ptB,pNorm,pRay);
        m_cSubObjects.Add(cr);
    }
    cr->bVisible = 0;
    cr->pObjectShape.nShapeId = 0;
    cr->pObjectShape.clrObject = RGB(0,0,0);
    //cr->SetInGraph();

    CPointSurC3D* temp =NULL;
    if (bLoad)
    {
        CObject3D* mmm = m_cSubObjects.GetAt(1);
        temp = (CPointSurC3D*)mmm;
        m_nAlpha = temp->lambda;
    }
    else
    {
        temp = new CPointSurC3D(cr);
        m_cSubObjects.Add(temp);
        temp->lambda = m_nAlpha;
    }
    //temp->SetInGraph();
    sommet[2] = temp;
    nStartShow = 1;

    for (int i=0;i<5;i++)
    {
        CVector4 pt;

        CPoint3D *obj = NULL;
        if (bLoad)
        {
            CObject3D* mmm = m_cSubObjects.GetAt(2+i);
            obj = (CPoint3D *)mmm;
        }
        else
        {
            obj = new CPointCalc3D(this);
            m_cSubObjects.Add(obj);
        }
        //obj->nDepth = nDepth+1;
        //obj->SetInGraph();
        sommet[i+3] = obj;
    }
    int nObjOrd[12][2];
    for (i=0;i<4;i++)
     {
        nObjOrd[i][0] = 0+i;
        nObjOrd[i][1] = (i==3) ? 0 : i+1;
        nObjOrd[4+i][0] = 4+i;
        nObjOrd[4+i][1] = (i==3) ? 4 : i+5;
        nObjOrd[8+i][0] = 0+i;
        nObjOrd[8+i][1] = 4+i;
     }
    for (i=0;i<12;i++)
    {
        int index = nObjOrd[i][0];
        CObject3D* p1 = sommet[index];
        index = nObjOrd[i][1];
        CObject3D* p2 = sommet[index];

        CSegment3D *obj = NULL;
        if (bLoad)
        {
            CObject3D* mmm = m_cSubObjects.GetAt(7+i);
            obj = (CSegment3D *)mmm;
        }
        else
        {
            obj = new CSegment3D((CPoint3D*)p1,(CPoint3D*)p2);
            m_cSubObjects.Add(obj);
            obj->pObjectShape.nShapeId = 4;
        }
        //obj->SetInGraph();
        arete[i] = obj;
     }
    InitFace(0,0,0,9,4,8);
    InitFace(1,1,1,10,5,9);
    InitFace(2,2,2,11,6,10);
    InitFace(3,3,3,8,7,11);
    InitFace(4,0,0,1,2,3);
    InitFace(5,5,4,7,6,5);
}

void CCube3D::InitFace(int facNum,int som,int s1,int s2,int s3,int s4)
{
    facette[facNum].base = sommet[som];
    facette[facNum].seg[0] = arete[s1];
    facette[facNum].seg[1] = arete[s2];
    facette[facNum].seg[2] = arete[s3];
    facette[facNum].seg[3] = arete[s4];

/*  CxObject3DSet pSet;
    pSet.Add(sommet[s1]);
    pSet.Add(sommet[s2]);
    pSet.Add(sommet[s3]);
    pSet.Add(sommet[s4]);
    facette[facNum].pFace = new CPolygon3D(&pSet);
    facette[facNum].pFace->CalculConceptuel();*/

}


CCube3D::CCube3D(const CObject3D &src):
    CCompositeObj3D(src)
{
    //if (pObjectShape) delete pObjectShape;
    pObjectShape = CShape(CShape::LineShape,TPref::TCube.nShape);
    pObjectShape.clrObject = src.pObjectShape.clrObject;
    ptA = ((CCube3D &)src).ptA;
    ptB = ((CCube3D &)src).ptB;
    m_nAlpha = ((CCube3D &)src).m_nAlpha;
//  cr = new CCercle3D(*((CCube3D &)src).cr);
//  for (int i=0;i<12;i++)
//   {
//      if (i<8)
//          sommet[i]=((CCube3D &)src).sommet[i];
//      arete[i]=((CCube3D &)src).arete[i];
//   }
    cr=NULL;
    nStartShow = 1;
    InitCube();
}

void CCube3D::Serialize( CArchive& ar )
{
    CCompositeObj3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_nAlpha;
        ar << ((ptA) ? ptA->nObjectId : -1);
        ar << ((ptB) ? ptB->nObjectId : -1);
    }
    else
    {
        ar >> m_nAlpha;
        ptA = (CPoint3D*)SerializeObj(ar);
        ptB = (CPoint3D*)SerializeObj(ar);
        InitCube(TRUE);
    }
}

CObject3D* CCube3D::CopyObject()
{
    CObject3D *temp = new CCube3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CCube3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(ptA);
    list->Add(ptB);
    return list;
}

BOOL CCube3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

CString CCube3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (ptA) sn1 = ptA->GetObjectHelp();
    if (ptB) sn2 = ptB->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}

UINT  CCube3D::CalculConceptuel()
{
    bValidate = ((ptA->bValidate) && (ptB->bValidate));
    if (!bValidate)
        return ValideCompositeObject(ERR_INDEPEND);

    bValidate = 0;
    CVector4 U = ptB->Concept_pt - ptA->Concept_pt;
    FCoord r = U.Norme();
    if (FCZero(r))
        return ValideCompositeObject(ERR_CBPTSID);

    CVector4 K = U * (1/r);
    CVector4 J(-K.y,K.x,0,1);
    FCoord normJ = J.Norme();
    if (FCZero(normJ))
        { J.x = J.z =0; J.y = 1;    }
    else
        J = J * (1/normJ);
    CVector4    I = (J % K);
/*  CVector4    temp(ptB->Concept_pt.x + r*I.x,
                     ptB->Concept_pt.y + r*I.y,
                     ptB->Concept_pt.z + r*I.z);
    ptc1->Concept_pt = temp;
    temp = Vector4(ptB->Concept_pt.x + r*J.x,
                        ptB->Concept_pt.y + r*J.y,
                        ptB->Concept_pt.z + r*J.z);
    ptc2->Concept_pt = temp;*/
    cr->VecNorm = U;
    cr->Radius = r;
    cr->CalculConceptuel();
    sommet[2]->CalculConceptuel();
    CVector4    C =  sommet[2]->Concept_pt;
    CVector4    BA = ptA->Concept_pt - ptB->Concept_pt,
                BC = C - ptB->Concept_pt,
                BCvBA = (BC % BA);

    CVector4 VecNorm = BCvBA;
    r = BA.Norme();
    if (BCvBA.NullVector())
    {
        bValidate = FALSE;
        return ValideCompositeObject(ERR_CBPTSID);
    }
    FCoord normK = 1 / BCvBA.Norme();
    K = BCvBA * normK;

    CPoint3D* obj = sommet[5];
    obj->Concept_pt.x = ptB->Concept_pt.x + r*K.x;
    obj->Concept_pt.y = ptB->Concept_pt.y + r*K.y;
    obj->Concept_pt.z = ptB->Concept_pt.z + r*K.z;
    obj->Concept_pt.w = 1;
    obj = sommet[3];
    obj->Concept_pt.x = ptA->Concept_pt.x + BC.x;
    obj->Concept_pt.y = ptA->Concept_pt.y + BC.y;
    obj->Concept_pt.z = ptA->Concept_pt.z + BC.z;
    obj->Concept_pt.w = 1;
    obj = sommet[4];
    obj->Concept_pt.x = ptA->Concept_pt.x + r*K.x;
    obj->Concept_pt.y = ptA->Concept_pt.y + r*K.y;
    obj->Concept_pt.z = ptA->Concept_pt.z + r*K.z;
    obj->Concept_pt.w = 1;
    obj = sommet[6];
    obj->Concept_pt.x = C.x + r*K.x;
    obj->Concept_pt.y = C.y + r*K.y;
    obj->Concept_pt.z = C.z + r*K.z;
    obj->Concept_pt.w = 1;
    C = sommet[3]->Concept_pt;
    obj = sommet[7];
    obj->Concept_pt.x = C.x + r*K.x;
    obj->Concept_pt.y = C.y + r*K.y;
    obj->Concept_pt.z = C.z + r*K.z;
    obj->Concept_pt.w = 1;

    CCompositeObj3D::CalculConceptuel();

    bValidate=TRUE;
    return ValideCompositeObject(0);
}

void CCube3D::CalculVisuel(CVisualParam *myVisuParam)
{
    if (cr) cr->CalculVisuel(myVisuParam);
    CCompositeObj3D::CalculVisuel(myVisuParam);

    if (pObjectShape.nShapeId == 2) return;
    for (int i=0;i<12;i++)
     {
        //TSegment3D* obj = (TSegment3D*) ((*SubObjects)[i+3]);
        CSegment3D* obj = arete[i];
        obj->pObjectShape.nShapeId = (pObjectShape.nShapeId) ? -1 : 4;
     }
    if (!pObjectShape.nShapeId) return;
    for (i=0;i<6;i++)
     {
        CPoint3D* p1 = facette[i].base;
        CPoint3D* p2 = facette[i].seg[0]->P2;
        CPoint3D* p3 = facette[i].seg[3]->P1;
        CSegment3D* s1 =facette[i].seg[0];
        CSegment3D* s2 =facette[i].seg[3];

        CVector4    FaceNorm1 = (
                (p2->Concept_pt - p1->Concept_pt) %
                (p3->Concept_pt - p1->Concept_pt));

        CVector4    FaceNorm= (s2->GetDirVector() % s1->GetDirVector());
        FCoord n1 = FaceNorm.Norme();
        FaceNorm = FaceNorm * (1/n1);

        CVector4 oeil= myVisuParam->GetEyePos();
        CVector4 origin(0,0,0,1);
        CVector4 VisuNorm= oeil -
                ((myVisuParam->bParProj) ? origin : p1->Concept_pt);
        FCoord n2 = VisuNorm.Norme();
        VisuNorm = VisuNorm*(1/n2);
        FCoord dot = VisuNorm * FaceNorm;
        int vis = (dot > 0) ? 0 : 4;
        for (int j=0;j<4;j++)
         {
//          TSegment3D* obj = (TSegment3D*) ((*SubObjects)[faces[i][j]]);
            CSegment3D* obj = facette[i].seg[j];
            //TSegment3D* obj = arete[faces[i][j]];
            int tr = obj->pObjectShape.nShapeId;
            if ((tr == -1)  || (tr == 0))
                obj->pObjectShape.nShapeId = vis;
         }
    }
}

void CCube3D::PrepareMoveObject(BOOL bMove)
{
    if (cr) cr->bVisible = bMove;
}

void CCube3D::GetRange(CVector4 &min,CVector4 &max)
{
    CObject3D::GetRange(min,max);

    for (int i=0;i<8;i++)
        if (sommet[i] && sommet[i]->bValidate && sommet[i]->IsVisible())
        {
            CVector4 pt = sommet[i]->Concept_pt;

            min.x = min(pt.x,min.x);
            min.y = min(pt.y,min.y);
            min.z = min(pt.z,min.z);
            max.x = max(pt.x,max.x);
            max.y = max(pt.y,max.y);
            max.z = max(pt.z,max.z);
        }
}


void CCube3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSM)
{
    if (cr && cr->bVisible) cr->Draw(pDC,vp,TRUE);
    CCompositeObj3D::Draw(pDC,vp,bSM);

    if ((!IsVisible()) || (!bValidate)) return;
//  if (!bIsSelected) return;
//  if (!IsInCalque(vp->nCalqueNum)) return;
    if (bIsSelected) DrawSelected(pDC,vp);
/*  for (int i=0;i<8;i++)
    {
        CPoint3D *pSom = sommet[i];
        if (!pSom) continue;
        if (pSom->bValidate && pSom->bVisible)
        {
            int Ds = TPref::TPoint.Size+2;
            CPoint Pt = pSom->Visual_pt;
            CRect theR(Pt.x-Ds+1,Pt.y-Ds+1,Pt.x+Ds,Pt.y+Ds);
            pDC->InvertRect(theR);
        }
    }*/
}

void CCube3D::SetColor(COLORREF rColor)
{
    CCompositeObj3D::SetColor(rColor);
    for (int i=2;i<8;i++)
    {
        CPoint3D *pSom = sommet[i];
        if (!pSom) continue;
        pSom->pObjectShape.clrObject = pObjectShape.clrObject;
        //pObjectShape.nShapeId = pPage.m_nShape;
    }
    for (i=0;i<12;i++)
    {
        CSegment3D *pSom = arete[i];
        if (!pSom) continue;
        pSom->pObjectShape.clrObject = pObjectShape.clrObject;
        //pObjectShape.nShapeId = pPage.m_nShape;
    }
}

void CCube3D::SetStyle(int nStyle)
{
    CCompositeObj3D::SetStyle(nStyle);
}


int CCube3D::SetProperties(CxObject3DSet *pSet)
{
    int nb = CCompositeObj3D::SetProperties(pSet);
    //if (nb != IDOK) return nb;

    return nb;

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CInterSphDr3D, CCompositeObj3D, VERSIONABLE_SCHEMA | 1)

CInterSphDr3D::CInterSphDr3D() : CCompositeObj3D()
{
    Sph = NULL;
    Dr = NULL;
    ptA = ptB = NULL;
}

CInterSphDr3D::CInterSphDr3D(CSphere3D* sp1,CDroite3D* dr2) :
    CCompositeObj3D()
{
    Sph = sp1;
    Dr = dr2;
    ptA = ptB = NULL;
    ptA = new CPointCalc3D(this);
    ptB = new CPointCalc3D(this);

    SetDepth();//nDepth = max(Sph->nDepth,Dr->nDepth)+1;
    //ptA->nDepth = nDepth+1;
    //ptB->nDepth = nDepth+1;
    ptA->SetInGraph();
    ptB->SetInGraph();

    m_cSubObjects.Add(ptA);
    m_cSubObjects.Add(ptB);
    InitInterSphDr();
}

CInterSphDr3D::CInterSphDr3D(const CObject3D & obj) :
    CCompositeObj3D(obj)
{
    Sph = ((CInterSphDr3D &)obj).Sph;
    Dr = ((CInterSphDr3D &)obj).Dr;
    ptA = ptB = NULL;
    ptA = new CPointCalc3D(this);
    ptB = new CPointCalc3D(this);
    SetDepth();
    //ptA->nDepth = nDepth+1;
    //ptB->nDepth = nDepth+1;
    ptA->SetInGraph();
    ptB->SetInGraph();

    m_cSubObjects.Add(ptA);
    m_cSubObjects.Add(ptB);
    InitInterSphDr();
}

int CInterSphDr3D::SetDepth()
{
    if (Sph && Dr)
    {
        nDepth = max(Sph->nDepth,Dr->nDepth)+1;
        if (ptA) ptA->nDepth = nDepth+1;
        if (ptB) ptB->nDepth = nDepth+1;
    }
    return nDepth;
}

CObject3D* CInterSphDr3D::CopyObject()
{
    CObject3D *temp = new CInterSphDr3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CInterSphDr3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(Sph);
    list->Add(Dr);
    return list;
}

BOOL CInterSphDr3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

void CInterSphDr3D::Serialize( CArchive& ar )
{
    CCompositeObj3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((Sph) ? Sph->nObjectId : -1);
        ar << ((Dr) ? Dr->nObjectId : -1);
    }
    else
    {
        Sph = (CSphere3D*)SerializeObj(ar);
        Dr = (CDroite3D*)SerializeObj(ar);
        int nb = m_cSubObjects.GetSize();
        for (int i=0;i<nb;i++)
        {
            CObject3D* ppp = m_cSubObjects.GetAt(i);
            if (i==0)
                ptA = (CPoint3D*)ppp;
            else if (i==1)
                ptB = (CPoint3D*)ppp;
            if (ptA) ptA->SetInGraph();
            if (ptB) ptB->SetInGraph();
        }
        InitInterSphDr();
    }
}

void CInterSphDr3D::SetColor(COLORREF rColor)
{
    CCompositeObj3D::SetColor(rColor);
}

void CInterSphDr3D::SetStyle(int nStyle)
{
    CCompositeObj3D::SetStyle(nStyle);
}

CString CInterSphDr3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (Sph) sn1 = Sph->GetObjectHelp();
    if (Dr) sn2 = Dr->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}

void CInterSphDr3D::InitInterSphDr()
{
    nStartShow = 0;
}


UINT  CInterSphDr3D::CalculConceptuel()
{
    bValidate = ((Sph->bValidate) && (Dr->bValidate));
    if (!bValidate)
        return ValideCompositeObject(ERR_NOINTER);

    CVector4 in,out,comp;
    BOOL bTest= Sph->IntersectLine(Dr,&in,&out);
    if (!bTest)
    {
        bValidate=FALSE;
        return ValideCompositeObject(ERR_INTERSPHDR);
    }

    comp = out - in;
    if (comp.NullVector())
    {
        //StartShow = 1;
        ptA->bValidate = TRUE;
        ptB->bValidate = TRUE;
     }
    else
     {
        //StartShow = 0;
        ptA->bValidate = TRUE;
        ptB->bValidate = TRUE;
     }
    ptA->Concept_pt = in;
    ptB->Concept_pt = out;

    CVector4 PP1 = Dr->GetBasePoint();
    CVector4 U = Dr->GetDirVector();
    CVector4 VvW = U;
    FCoord ll = (FCoord)VvW;
    VvW = VvW * (1/ll);
    FCoord DotProd = U * VvW;

    comp = in - PP1;
    FCoord lambda = (comp * VvW) / DotProd;
    comp = out - PP1;
    FCoord beta = (comp * VvW) / DotProd;
    BOOL    va = Dr->IsInLimit(lambda),
            vb = Dr->IsInLimit(beta);

    if (!va && !vb)
     {
        bValidate=0;
        return ValideCompositeObject(ERR_INTERSPHDR);
     }
    else
     {
        //int ret = ValideCompositeObject(0);
        if (!vb)
            ptB->bValidate = 0;
        else if (!va)
            ptA->bValidate = 0;
        CCompositeObj3D::CalculConceptuel();

        bValidate=TRUE;
        return 0;
     }
}


void CInterSphDr3D::CalculVisuel(CVisualParam *myVisuParam)
{
    CCompositeObj3D::CalculVisuel(myVisuParam);
}

void CInterSphDr3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSM)
{
    CCompositeObj3D::Draw(pDC,vp,bSM);

    if (bIsSelected)
        DrawSelected(pDC,vp);

/*  if (!bIsSelected) return;
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
    {
        CPoint3D *pSom = (CPoint3D *)m_cSubObjects.GetAt(i);
        if (!pSom) continue;
        if (pSom->bValidate && pSom->bVisible)
        {
            int Ds = TPref::TPoint.Size+2;
            CPoint Pt = pSom->Visual_pt;
            CRect theR(Pt.x-Ds+1,Pt.y-Ds+1,Pt.x+Ds,Pt.y+Ds);
            pDC->InvertRect(theR);
        }
    }*/
}

void CInterSphDr3D::GetRange(CVector4 &min,CVector4 &max)
{
    CObject3D::GetRange(min,max);

    if (ptA && ptA->bValidate && ptA->IsVisible())
    {
        CVector4 pt = ptA->Concept_pt;
        min.x = min(pt.x,min.x);
        min.y = min(pt.y,min.y);
        min.z = min(pt.z,min.z);
        max.x = max(pt.x,max.x);
        max.y = max(pt.y,max.y);
        max.z = max(pt.z,max.z);
    }
    if (ptB && ptB->bValidate && ptB->IsVisible())
    {
        CVector4 pt = ptB->Concept_pt;
        min.x = min(pt.x,min.x);
        min.y = min(pt.y,min.y);
        min.z = min(pt.z,min.z);
        max.x = max(pt.x,max.x);
        max.y = max(pt.y,max.y);
        max.z = max(pt.z,max.z);
    }
}



CString CInterSphDr3D::DrawSymbolic()
{
    CString mstr;
    mstr.Empty();

    int nb = m_cSubObjects.GetSize();

    if (nb !=2) return mstr;

    if (bValidate && Dr && Sph)
    {
        int nObj1=-1,nObj2=-1;

        CObject3D* p1 = m_cSubObjects.GetAt(0);
        CObject3D* p2 = m_cSubObjects.GetAt(1);

        if (!p1 || !p2) return mstr;


        CString mstr2,strName,strObj1,strObj2,strObj3,strObj4;
        mstr2.LoadString(GetNameID());
        strName.Format("%s%d",mstr2,nObjectId);
        mstr2.LoadString(p1->GetNameID());
        strObj1.Format("%s%d",mstr2,p1->nObjectId);
        mstr2.LoadString(p2->GetNameID());
        strObj2.Format("%s%d",mstr2,p2->nObjectId);

        mstr2.LoadString(Dr->GetNameID());
        strObj3.Format("%s%d",mstr2,Dr->nObjectId);
        mstr2.LoadString(Sph->GetNameID());
        strObj4.Format("%s%d",mstr2,Sph->nObjectId);

        mstr.Format(_T("Intersection1LineSphere[%s,%s,%s];\nIntersection2LineSphere[%s,%s,%s];"),
            strObj1,strObj4,strObj3,strObj2,strObj4,strObj3);
    }
    return mstr;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDivSegment3D, CCompositeObj3D, VERSIONABLE_SCHEMA | 1)

CDivSegment3D::CDivSegment3D() : CCompositeObj3D()
{
    Seg = NULL;         // sphere
    m_nDiv= 3;

}

CDivSegment3D::CDivSegment3D(CSegment3D* pSeg) :
    CCompositeObj3D()
{
    Seg = pSeg;
    m_nDiv= 3;

    SetDepth();
    for (int i=0;i<m_nDiv-1;i++)
    {
        CPointCalc3D * ptA = new CPointCalc3D(this);
        ptA->nDepth = nDepth+1;
        ptA->SetInGraph();
        m_cSubObjects.Add(ptA);
    }
}

CDivSegment3D::CDivSegment3D(const CObject3D & obj) :
    CCompositeObj3D(obj)
{
    Seg = ((CDivSegment3D &)obj).Seg;
    m_nDiv = ((CDivSegment3D &)obj).m_nDiv;

    SetDepth();
    for (int i=0;i<m_nDiv-1;i++)
    {
        CPointCalc3D * ptA = new CPointCalc3D(this);
        ptA->nDepth = nDepth+1;
        ptA->SetInGraph();
        m_cSubObjects.Add(ptA);
    }

}

int CDivSegment3D::SetDepth()
{
    if (Seg)
    {
        nDepth = Seg->nDepth+1;
    }
    return nDepth;
}

CObject3D* CDivSegment3D::CopyObject()
{
    CObject3D *temp = new CDivSegment3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CDivSegment3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(Seg);
    return list;
}

BOOL CDivSegment3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

void CDivSegment3D::Serialize( CArchive& ar )
{
    CCompositeObj3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((Seg) ? Seg->nObjectId : -1);
        ar << m_nDiv;
    }
    else
    {
        Seg = (CSegment3D*)SerializeObj(ar);
        ar >> m_nDiv;
    }
}

void CDivSegment3D::SetColor(COLORREF rColor)
{
    CCompositeObj3D::SetColor(rColor);
}

void CDivSegment3D::SetStyle(int nStyle)
{
    CCompositeObj3D::SetStyle(nStyle);
}

CString CDivSegment3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (Seg) sn1 = Seg->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1);
    return mstr;
}

UINT  CDivSegment3D::CalculConceptuel()
{
    bValidate = (Seg->bValidate);
    if (!bValidate)
        return ValideCompositeObject(ERR_NOINTER);

    CVector4 vSeg = Seg->GetDirVector();

    if (vSeg.NullVector())
    {
        bValidate=FALSE;
        return ValideCompositeObject(ERR_INTERSPHDR);
    }

    ValideCompositeObject(0);

    for (int i=0;i<m_nDiv-1;i++)
    {
        CPointCalc3D * ptA = (CPointCalc3D*)m_cSubObjects.GetAt(i);

        ptA->Concept_pt = vSeg * (FCoord)(1+i);
        ptA->Concept_pt = ptA->Concept_pt * (FCoord)(1./m_nDiv);
        ptA->Concept_pt = ptA->Concept_pt + Seg->P1->Concept_pt ;

    }
    bValidate=TRUE;
    return 0;
}


void CDivSegment3D::CalculVisuel(CVisualParam *myVisuParam)
{
    CCompositeObj3D::CalculVisuel(myVisuParam);
}

void CDivSegment3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSM)
{
    CCompositeObj3D::Draw(pDC,vp,bSM);

    if (bIsSelected)
        DrawSelected(pDC,vp);

/*  if (!bIsSelected) return;
    int nb = m_cSubObjects.GetSize();
    for (int i=0;i<nb;i++)
    {
        CPoint3D *pSom = (CPoint3D *)m_cSubObjects.GetAt(i);
        if (!pSom) continue;
        if (pSom->bValidate && pSom->bVisible)
        {
            int Ds = TPref::TPoint.Size+2;
            CPoint Pt = pSom->Visual_pt;
            CRect theR(Pt.x-Ds+1,Pt.y-Ds+1,Pt.x+Ds,Pt.y+Ds);
            pDC->InvertRect(theR);
        }
    }*/
}

void CDivSegment3D::GetRange(CVector4 &min,CVector4 &max)
{
    CObject3D::GetRange(min,max);
}



CString CDivSegment3D::DrawSymbolic()
{
    CString mstr;
    mstr.Empty();

    int nb = m_cSubObjects.GetSize();

    if (nb !=2) return mstr;

    if (bValidate && Seg)
    {
    }
    return mstr;
}
