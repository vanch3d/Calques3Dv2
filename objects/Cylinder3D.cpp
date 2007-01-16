// Cylinder3D.cpp: implementation of the CCylinder3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"

#include "..\Prefs\Prefs.h"

#include "Point3D.h"
#include "Droite3D.h"
#include "Cercle3D.h"
#include "Plan3D.h"

#include "Cylinder3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern FCoord GetDistancePt(CVector4 Nf, CVector4 pt);

BOOL intcyl(
        CVector4        raybase,    // Base of the intersection ray
        CVector4        raycos,     // Direction cosines of the ray
        CVector4        base,       // Base of the cylinder
        CVector4        axis,       // Axis of the cylinder
        FCoord      radius,     // Radius of the cylinder
        FCoord      *in,            // Entering distance
        FCoord      *out            // Leaving distance
    );



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCylinder3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CCylinder3D::CCylinder3D() : CObject3D()
{
    P1 = P2 = Rad = NULL;
    C1 = C2 = C3 = NULL;
    Base1 = Base2 = Base3 = NULL;


}

CCylinder3D::CCylinder3D(const CObject3D &src ) : CObject3D(src)
{
    P1 =  ((CCylinder3D&)src).P1;
    P2 =   ((CCylinder3D&)src).P2;
    Rad =   ((CCylinder3D&)src).Rad;
    Base1 =   new CPoint3D(((CCylinder3D&)src).Base1->Concept_pt);
    Base2 =   new CPoint3D(((CCylinder3D&)src).Base2->Concept_pt);
    Base3 =   new CPoint3D(((CCylinder3D&)src).Base3->Concept_pt);
    C1 = new CCercle3D(Base1,CVector4(1,0,0),1.);
    C2 = new CCercle3D(Base2,CVector4(1,0,0),1.);
    C3 = new CCercle3D(Base3,CVector4(1,0,0),1.);
    nRayon =   ((CCylinder3D&)src).nRayon;
    nDeltaT =   ((CCylinder3D&)src).nDeltaT;

    for (int i=0;i<((CCylinder3D&)src).cPointsList.GetSize();i++)
    {
        CPoint3D *pt= (CPoint3D*)((CCylinder3D&)src).cPointsList.GetAt(i);
        cPointsList.Add(pt->CopyObject());
    }
    for (i=0;i<((CCylinder3D&)src).cGenerList.GetSize();i++)
    {
        CObject3D  *obj = ((CCylinder3D&)src).cGenerList.GetAt(i);
        CSegment3D *sg = (CSegment3D*)obj->CopyObject();
        sg->P1 = (CPoint3D*)cPointsList.GetAt(2*i);
        sg->P2 = (CPoint3D*)cPointsList.GetAt(2*i+1);
        cGenerList.Add(sg);
    }
}

CCylinder3D::CCylinder3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *rad): CObject3D()
{
    P1 = p1;
    P2 = p2;
    Rad = rad;
    Base1 = new CPoint3D(P1->Concept_pt);
    Base2 = new CPoint3D(P2->Concept_pt);
    Base3 = new CPoint3D(CVector4(1,0,0));
    SetDepth();//nDepth = max(p1->nDepth,max(p2->nDepth,rad->nDepth))+1;
    C1 = new CCercle3D(Base1,CVector4(1,0,0),1.);
    C2 = new CCercle3D(Base2,CVector4(1,0,0),1.);
    C3 = new CCercle3D(Base3,CVector4(1,0,0),1.);
    pObjectShape.clrObject = RGB(0,0,255);
    C1->pObjectShape.clrObject = pObjectShape.clrObject;
    C2->pObjectShape.clrObject = pObjectShape.clrObject;
    C3->pObjectShape.nShapeId = 0;
    C3->pObjectShape.clrObject = pObjectShape.clrObject;//RGB(0,0,0);

    nRayon  = 0.;
    nDeltaT = 2;

    cGenerList.RemoveAll();
    cPointsList.RemoveAll();
    //PointsList = new TObject3DList(2*deltaT, 0, 5);
}

int CCylinder3D::SetDepth()
{
    if (P1 && P2 && Rad)
        nDepth = max(P1->nDepth,max(P2->nDepth,Rad->nDepth))+1;
    return nDepth;
}


BOOL CCylinder3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (P1 == pOld)
    {
        SetInGraph(FALSE);
        P1 = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (P2 == pOld)
    {
        SetInGraph(FALSE);
        P2 = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (Rad == pOld)
    {
        SetInGraph(FALSE);
        Rad = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}


void CCylinder3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << nDeltaT;
        ar << nRayon;
        ar << ((P1) ? P1->nObjectId : -1);
        ar << ((P2) ? P2->nObjectId : -1);
        ar << ((Rad) ? Rad->nObjectId : -1);
    }
    else
    {
        ar >> nDeltaT;
        ar >> nRayon;
        P1 = (CPoint3D*)SerializeObj(ar);
        P2 = (CPoint3D*)SerializeObj(ar);
        Rad = (CPoint3D*)SerializeObj(ar);
        Base1 = new CPoint3D(P1->Concept_pt);
        Base2 = new CPoint3D(P2->Concept_pt);
        Base3 = new CPoint3D(CVector4(1,0,0));
        C1 = new CCercle3D(Base1,CVector4(1,0,0),1.);
        C2 = new CCercle3D(Base2,CVector4(1,0,0),1.);
        C3 = new CCercle3D(Base3,CVector4(1,0,0),1.);
        //pObjectShape.clrObject = RGB(0,0,255);
        C1->pObjectShape.clrObject = pObjectShape.clrObject;
        C2->pObjectShape.clrObject = pObjectShape.clrObject;
        C3->pObjectShape.nShapeId = 0;
        C3->pObjectShape.clrObject = pObjectShape.clrObject;//RGB(0,0,0);
    }
}


CCylinder3D::~CCylinder3D()
{
    if (Base1) delete Base1;
    if (Base2) delete Base2;
    if (Base3) delete Base3;
    if (C1) delete C1;
    if (C2) delete C2;
    if (C3) delete C3;
    if (cGenerList.GetSize())
    {
        for (int i=0;i<cGenerList.GetSize();i++)
            delete cGenerList.GetAt(i);
        cGenerList.RemoveAll();
    }
    if (cPointsList.GetSize())
    {
        for (int i=0;i<cPointsList.GetSize();i++)
            delete cPointsList.GetAt(i);
        cPointsList.RemoveAll();
    }
}

CObject3D* CCylinder3D::CopyObject()
{
    CObject3D *temp = new CCylinder3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CCylinder3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(P1);
    list->Add(P2);
    list->Add(Rad);
    return list;
}


int CCylinder3D::SetProperties(CxObject3DSet *pSet)
{
    int ret = CObject3D::SetProperties(pSet);
    if (ret==IDOK)
     {
        if (C1)
        {
            C1->pObjectShape.clrObject = pObjectShape.clrObject;
            C1->pObjectShape.nShapeId = pObjectShape.nShapeId;
        }
        if (C2)
        {
            C2->pObjectShape.clrObject = pObjectShape.clrObject;
            C2->pObjectShape.nShapeId = pObjectShape.nShapeId;
        }
        if (C3)
        {
            C3->pObjectShape.clrObject = pObjectShape.clrObject;
        }
        for (int i = 0;i<nDeltaT;i++)
        {
            cGenerList[i]->pObjectShape.clrObject = pObjectShape.clrObject;
            cGenerList[i]->pObjectShape.nShapeId = pObjectShape.nShapeId;
        }
     }
    return ret;
}


BOOL CCylinder3D::IsEqual(CObject3D &other)
{
    if (!(other.MaskObject(TCylindre3DClass))) return false;
    if (!bValidate || !(other.bValidate)) return false;
/*  bool        SameCenter  = NullVector(MakeVector(P1->Concept_pt,
                                            ((TSphere3D&)o).P1->Concept_pt));
    FCoord  rad1 = Norme(Rayon),
                rad2 = Norme(((TSphere3D&)o).Rayon);
    bool        SameRadius  = FCZero(rad1 - rad2);
    return (SameCenter &&  SameRadius);*/
    return false;

}

BOOL CCylinder3D::IsInActiveArea(CPoint thePt)
{
    BOOL bRet = FALSE;
    if (!bRet && C1)    bRet = C1->IsInActiveArea(thePt);
    if (!bRet && C2)    bRet = C2->IsInActiveArea(thePt);
    if (!bRet)
        for (int i = 0;i<nDeltaT && !bRet;i++)
        {
            if (!cGenerList.GetSize()) return bRet;
            CObject3D *pObj = cGenerList.GetAt(i);
            if (pObj)
                bRet = pObj->IsInActiveArea(thePt);
        }
    return bRet;
}

CString CCylinder3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (P1) sn1 = P1->GetObjectName();
    if (P2) sn2 = P2->GetObjectName();
    if (Rad) sn3 = Rad->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2,sn3);
    return mstr;
}

CVector4 CCylinder3D::calculGeneratrice(FCoord theta,CVector4 Center,
                    FCoord Rayon,CVector4 I,CVector4 J,CVector4 K)
{
    CVector4 U;
    U.x=    Rayon*cos(theta);
    U.y=    Rayon*sin(theta);
    U.z=    0;
    U.w=    1;

    CVector4 V;
    V.x=    I.x*U.x + J.x*U.y + K.x*U.z + Center.x,
    V.y=    I.y*U.x + J.y*U.y + K.y*U.z + Center.y,
    V.z=    I.z*U.x + J.z*U.y + K.z*U.z + Center.z,
    V.w=    1;
    return V;
}


FCoord CCylinder3D::DistPtToLine(CVector4 pt,CVector4 src,CVector4 dir)
{
    FCoord a1 = (pt.y-src.y)*dir.z - (pt.z-src.z)*dir.y;
    FCoord a2 = (pt.z-src.z)*dir.x - (pt.x-src.x)*dir.z;
    FCoord a3 = (pt.x-src.x)*dir.y - (pt.y-src.y)*dir.x;
    FCoord nom = a1*a1+a2*a2+a3*a3;
    FCoord denom = dir.x*dir.x+dir.y*dir.y+dir.z*dir.z;
    FCoord dis = sqrtl(nom/denom);
    return dis;
}

UINT  CCylinder3D::CalculConceptuel()
{
    bValidate = ((P1->bValidate) && (P2->bValidate) && (Rad->bValidate));
    if (!bValidate)
        return ERR_NOCYLIND;
    CVector4 p1p2 = P2->Concept_pt - P1->Concept_pt;
    if (p1p2.NullVector())
    {
        bValidate = 0;
        return ERR_PTSALIGN;
    }
    FCoord mydist = DistPtToLine(Rad->Concept_pt,P1->Concept_pt,p1p2);
    nRayon = mydist;
    if (FCZero(nRayon))
    {
        bValidate = 0;
        return ERR_PTSALIGN;
    }

    FCoord pas = 15.*nRayon/70;
    pas = min(max(20,pas),70);

    // Projection du point sur l'axe P1P2
    FCoord l= (FCoord)p1p2;
    CVector4 p1  = P1->Concept_pt;
    CVector4 p2  = P2->Concept_pt;
    CVector4 p3  = Rad->Concept_pt;
    FCoord m = (p3.x-p1.x)*(p2.x-p1.x) +
                  (p3.y-p1.y)*(p2.y-p1.y) +
                  (p3.z-p1.z)*(p2.z-p1.z);
    FCoord projRad = m / (l*l);

    if (projRad < 0.)
    {
        Base3->Concept_pt = P1->Concept_pt;
        Base2->Concept_pt = P2->Concept_pt;
        Base1->Concept_pt = P1->Concept_pt + p1p2 * projRad;
    }
    else if (projRad > 1.)
    {
        Base1->Concept_pt = P1->Concept_pt;
        Base3->Concept_pt = P2->Concept_pt;
        Base2->Concept_pt = P1->Concept_pt + p1p2 * projRad;
    }
    else
    {
        Base1->Concept_pt = P1->Concept_pt;
        Base2->Concept_pt = P2->Concept_pt;
        Base3->Concept_pt = P1->Concept_pt + p1p2 * projRad;
    }

    if (C1)
    {
        C1->nDeltaT = (int)pas;
        C1->Radius = nRayon;
        C1->VecNorm = p1p2;
        C1->CalculConceptuel();
    }
    if (C2)
    {
        C2->nDeltaT = (int)pas;
        C2->Radius = nRayon;
        C2->VecNorm = p1p2;
        C2->CalculConceptuel();
    }
    if (C3)
     {
        C3->nDeltaT = (int)pas;
        C3->Radius = nRayon;
        C3->VecNorm = p1p2;
        C3->CalculConceptuel();
     }

    LocRep.O = Base1->Concept_pt;
    LocRep.K = Base2->Concept_pt - Base1->Concept_pt;
    LocRep.K = LocRep.K.Normalized();
    LocRep.J = Rad->Concept_pt - (P1->Concept_pt + p1p2 * projRad);
    LocRep.J = LocRep.J.Normalized();
    LocRep.I = LocRep.K % LocRep.J;
    //LocRep.I = LocRep.I.Normalized();

    bValidate=TRUE;
    return 0;
}

void CCylinder3D::CalculVisuel(CVisualParam *pVisParam)
{
    if (C1)
     {
        C1->nCalque = nCalque;
        C1->CalculVisuel(pVisParam);
     }
    if (C2)
     {  C2->nCalque = nCalque;
        C2->CalculVisuel(pVisParam);
     }
    if (C3)
     {  C3->nCalque = nCalque;
        C3->CalculVisuel(pVisParam);
     }

    CVector4 p1p2 = C1->VecNorm;
    CVector4 oeil= pVisParam->GetEyePos();
    CVector4 beep = oeil;// * ((FCoord)1/Norme(oeil));
    CVector4 beep2 = p1p2;// * ((FCoord)1/Norme(p1p2));
    CVector4 aa = beep % beep2;

    FCoord normK = 1 / p1p2.Norme();
    CVector4 K = p1p2 * normK,
             I,J;
    if (aa.NullVector())
     {
        I = CVector4(-K.y,K.x,0,1);
        normK = J.Norme();
        if (FCZero(normK))
            { I.x = I.z =0; I.y = 1;    }
        else
            I = I * (1/normK);
     }
    else
        I = aa * (1 /aa.Norme());
    J = (I % K);
    IntH = CLocalRep(P1->Concept_pt,I,J,K);

    int nbPts = cPointsList.GetSize();
    int nbSeg = cGenerList.GetSize();
    CVector4 U,Center;
    for (int t=0;t<nDeltaT;t++)
    {
        CVector4 V1 = calculGeneratrice(2*M_PI*t/nDeltaT,Base1->Concept_pt,nRayon,I,J,K);
        CVector4 V2 = calculGeneratrice(2*M_PI*t/nDeltaT,Base2->Concept_pt,nRayon,I,J,K);
        CPoint3D    *spt1 = NULL ,
                    *spt2 = NULL;
        if (!nbPts)
         {
            spt1 = new CPoint3D(V1);
            spt2 = new CPoint3D(V2);
            cPointsList.Add(spt1);
            cPointsList.Add(spt2);
         }
        else
         {
            spt1 = (CPoint3D*)cPointsList.GetAt(2*t);
            spt2 = (CPoint3D*)cPointsList.GetAt(2*t+1);
            spt1->Concept_pt = V1;
            spt2->Concept_pt = V2;
         }
        spt1->CalculConceptuel();
        spt1->CalculVisuel(pVisParam);
        spt2->CalculConceptuel();
        spt2->CalculVisuel(pVisParam);
        CSegment3D *seg = 0;
        if (!nbSeg)
        {
            seg = new CSegment3D(spt1,spt2);
            cGenerList.Add(seg);
            seg->pObjectShape.clrObject = pObjectShape.clrObject;
        }
        else
        {
            seg = (CSegment3D*)cGenerList.GetAt(t);
        }
        seg->CalculConceptuel();
        seg->CalculVisuel(pVisParam);
        seg->nCalque = nCalque;
     }
}

void CCylinder3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

    CPen curPen,disPen;
    LOGPEN logP = pObjectShape.GetPenStyle(); //(*ObjectColor,0,PS_SOLID);
    LOGPEN logPH = pObjectShape.GetHiddenPenStyle(
                            pObjectShape.GetObjectHiddenColor());
    curPen.CreatePenIndirect(&logP);
    disPen.CreatePenIndirect(&logPH);

    CPen curPenH(PS_DOT,1,pObjectShape.clrObject);
    CPen disPenH(PS_DOT,1,pObjectShape.GetObjectHiddenColor());


    CVector4 Vtt = calculGeneratrice(M_PI/2,P1->Concept_pt,nRayon,IntH.I,IntH.J,IntH.K);
    CVector4 Vtp = calculGeneratrice(M_PI/2,P2->Concept_pt,nRayon,IntH.I,IntH.J,IntH.K);
    CVector4 oeil = mV->GetEyePos();
    oeil = oeil * 100.;
    CPoint3D    vispt(oeil);
    CVector4 origin(0,0,0,1);
    CVector4 VisuNorm= vispt.Concept_pt - origin;
    CPlan3D vispl(&vispt,VisuNorm);
    UINT hh = vispl.CalculConceptuel();

    if (C1)
    {
        C1->pObjectShape.clrObject = pObjectShape.clrObject;
        C1->pObjectShape.nShapeId = pObjectShape.nShapeId;
    }
    if (C2)
    {
        C2->pObjectShape.clrObject = pObjectShape.clrObject;
        C2->pObjectShape.nShapeId = pObjectShape.nShapeId;
    }
    CCercle3D *hidCr=NULL,*visCr=NULL;
    if (!hh)
    {
        CVector4    pNorm1 = vispl.GetNormalForm();
        FCoord  m1 = CPlan3D::GetDistancePt(pNorm1,Vtt);
        FCoord  m2 = CPlan3D::GetDistancePt(pNorm1,Vtp);
        hidCr = (m1<m2) ? C2 : C1;
        visCr = (m1<m2) ? C1 : C2;
     }

    /// les retroactions
    if (bMarked && mV->bDrawMark)
        DrawRetro(pDC,mV);

    /// les bases
    C3->Draw(pDC,mV,bSm);
    if (!hidCr)
    {
        C1->Draw(pDC,mV,bSm);
        C2->Draw(pDC,mV,bSm);
    }
    else
    {
        visCr->Draw(pDC,mV,bSm);
        for (int t=0;t<hidCr->nDeltaT;t++)
        {
            CPen *oldP=NULL;
            if (!bSm)
            {
                CVector4 U = hidCr->m_cConcept_pts[t];
                CVector4 V = hidCr->m_cConcept_pts[(t==(hidCr->nDeltaT-1)) ? 0 : t+1];
                BOOL Vis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));

                CVector4 Up = U - hidCr->Center;
                Up = Up * (1/Up.Norme());
                CVector4 Vp = V - hidCr->Center;
                Vp = Vp * (1/Vp.Norme());

                FCoord sa = IntH.J * Up;
                FCoord ca = IntH.J * Up;
                //if (fabsl(sa) > 1.)
                //  sa = (sa < 0.00) ? -1.00 : 1.00;
                //sa = acosl(sa);
                if (sa >0 && ca > 0)
                    oldP = pDC->SelectObject(Vis1 ? &curPenH : &disPenH);
                else
                    oldP = pDC->SelectObject(Vis1 ? &curPen : &disPen);
             }
            else
                oldP = pDC->SelectObject(&curPen);
            CPoint start = hidCr->m_cVisual_pts[t];
            CPoint endpt = hidCr->m_cVisual_pts[(t==(hidCr->nDeltaT-1)) ?0 : t+1];
            pDC->MoveTo(start);
            pDC->LineTo(endpt);
            pDC->SelectObject(oldP);
         }
     }

    //TBrush curBrush(ObjectShape->ObjectColor);
    //HandleDC.SelectObject(curBrush);
    //HandleDC.SelectObject(curPen);

    /// les génératrices
    for (int t=0;t<nDeltaT;t++)
    {
        CObject3D *seg = cGenerList[t];
        seg->pObjectShape.clrObject = pObjectShape.clrObject;
        seg->pObjectShape.nShapeId = pObjectShape.nShapeId;
        seg->Draw(pDC,mV,bSm);
     }

    CPoint aa = (CPoint) mV->ProjectPoint(Vtt);
    CPoint bb = (CPoint) mV->ProjectPoint(Vtp);

    if (bIsSelected)
    {
        for (int i=0;i<cPointsList.GetSize();i++)
        {
            CPoint3D * pt = (CPoint3D *)cPointsList[i];
            int Ds = TPref::TPoint.nSize;
            CPoint  Pt = pt->Visual_pt;
            CRect theR(Pt.x-Ds+1,Pt.y-Ds+1,Pt.x+Ds,Pt.y+Ds);
            pDC->InvertRect(theR);
        }
    }
    //pDC->Rectangle(aa,TSize(2,2));
    //pDC->Rectangle(bb,TSize(2,2));


    //HandleDC.RestoreObjects();
}

void CCylinder3D::DrawRetro(CDC*,CVisualParam *vp)
{
    CPoint3D pt1(CVector4(0,0,0));
    CPoint3D pt2(CVector4(100,0,0));
    CPoint3D pt3(CVector4(0,100,0));
    CPlan3D pl(&pt1,&pt2,&pt3);
    pl.CalculConceptuel();
/* NVL
    CEllipse3D ell(0,this,&pl);
    UINT ret = ell.CalculConceptuel();
    if (!ret)
     {
        ell.ObjectShape->AShape = 0;
        ell.ObjectShape->ObjectColor = TColor(0,0,0);
        ell.CalculVisuel(mV);
        ell.Draw(HandleDC,mV,1);
     }
*/
}

BOOL CCylinder3D::IntersectLine(CDroite3D *Dr,CVector4 *pt1,CVector4 *pt2)
{
    CVector4 raybase = Dr->GetBasePoint();
    CVector4 raycos = Dr->GetDirVector();

    if (raycos.NullVector()) return 0;
    raycos = raycos * (1/raycos.Norme());
    CVector4 base = P1->Concept_pt;
    CVector4 axis = P2->Concept_pt - P1->Concept_pt;
    if (raycos.NullVector()) return 0;
    axis = axis * (1./axis.Norme());

    FCoord      radius = nRayon;        // Radius of the cylinder
    FCoord      in,out;
    //pt1 =pt2 = 0;

    BOOL ret = CCylinder3D::InterCylLine(raybase,raycos,base,axis,radius,&in,&out);
    if (ret)
     {
        CVector4 dir2 = raycos * (in/raycos.Norme());
        *pt1 = raybase+dir2;
        dir2 = raycos * (out/raycos.Norme());
        *pt2 = raybase+dir2;

     }
    return ret;
}

/////////////////////////////////////////////////////////////////////////////
/// Determine the intersection of a ray with a cylinder. 
/// Note that in and/or out may be negative indicating the cylinder is located behind the 
/// origin of the ray.
///		ANSI C code from the article "Intersecting a Ray with a Cylinder"
///		by Joseph M. Cychosz and Warren N. Waggenspack, Jr.
///		"Graphics Gems IV", Academic Press, 1994
/// @param raybase	Base of the intersection ray
/// @param raycos	Direction cosines of the ray
/// @param base		Base of the cylinder
/// @param axis		Axis of the cylinder
/// @param radius	Radius of the cylinder
/// @param in		Entering distance
/// @param out		Leaving distance
/// @return			TRUE if the ray intersects the cylinder, FALSE otherwise
/////////////////////////////////////////////////////////////////////////////
BOOL CCylinder3D::InterCylLine(CVector4 raybase,CVector4 raycos,CVector4 base,CVector4 axis,
							   FCoord radius,FCoord* in,FCoord* out)
{
    BOOL        hit;			// True if ray intersects cyl
    CVector4    RC;				// Ray base to cylinder base
    FCoord      d;				// Shortest distance between the ray and the cylinder
    FCoord      t, s;			// Distances along the ray
    CVector4    n, D, O;
    FCoord      ln;

    RC = raybase - base;
    n = raycos % axis;
    ln = n.Norme();

    if  ( n.NullVector())
    {   /* ray parallel to cyl  */
         d   = RC * axis;
         axis = axis*d;
         D = RC - axis;
         d = D.Norme();
         *in = -HUGE;
         *out =  HUGE;
         return (d <= radius);      /* true if ray is in cyl*/
    }

    ln = 1/ln;
    n = n * ln;
    d    = fabsl (RC * n);      /* shortest distance    */
    hit  = (d <= radius);

    if  (hit) 
	{	/* if ray hits cylinder */
		O = RC % axis;
		t = - (O * n) / ln;
		O = (n % axis);
		ln = 1 / O.Norme();
		n = n * ln;
		s = fabsl (sqrt(radius*radius - d*d) / (raycos * O));
		*in  = t - s;           /* entering distance    */
		*out = t + s;          /* exiting  distance    */
    }
    return (hit);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCone3D, CCylinder3D, VERSIONABLE_SCHEMA | 1)


CCone3D::CCone3D() : CCylinder3D()
{
}

CCone3D::CCone3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *rad) : CCylinder3D(p1,p2,rad)
{
    if (C1) delete C1;
    if (C2) delete C2;
    C1 = new CCercle3D(P1,CVector4(1,0,0),1.);
    C2 = NULL;//new TCercle3D(0,P2,Vector4(1,0,0),1.);
    pObjectShape.clrObject = RGB(0,0,255);
    C1->pObjectShape.clrObject = pObjectShape.clrObject;
}

CCone3D::CCone3D(const CObject3D & src) : CCylinder3D()
{
}

CCone3D::~CCone3D()
{
}

CObject3D* CCone3D::CopyObject()
{
    return NULL;
}

UINT  CCone3D::CalculConceptuel()
{
    return 0;
}

void CCone3D::CalculVisuel(CVisualParam *pVisParam)
{
}

void CCone3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSm)
{
}

void CCone3D::DrawRetro(CDC*,CVisualParam *vp)
{
}
