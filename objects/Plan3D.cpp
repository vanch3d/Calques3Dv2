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
// Plan3D.cpp: implementation of the CPlan3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Plan3D.h"

#include "Point3D.h"
#include "Plan3D.h"
#include "Droite3D.h"
#include "Cercle3D.h"
#include "Sphere3D.h"
#include "CompositeObj3D.h"

#include "Geom.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PLANEMARGE 20

//extern CRgn* DoSegRgn(CPoint p1,CPoint p2);

int CxVectorSSSet::AddSorted(CVector4SSide theo)
{
    int nb = GetSize();
    for (int i=0;i<nb;i++)
    {
        CVector4SSide myo = GetAt(i);
        if (theo < myo)
        {
            InsertAt(i,theo);
            return i;
        }
    }
    Add(theo);
    return nb;
}

void CxVectorSSSet::AddtoList(CDroite3D *d, CSegment3D *p,CPoint3D  *myA[4],int *nbpt,
							  FCoord dp1,  CVector4 base, int n,bool b)
{
    CPointInterDD3D *pt = new CPointInterDD3D(d,p);
    if (!pt->CalculConceptuel())
     {
        CVector4 cpt =  pt->Concept_pt;
        myA[(*nbpt)++]=pt;
        //AddSorted(mylist,CVector4SSide(cpt,dp1+(FCoord)(cpt - base),n,b));
        AddSorted(CVector4SSide(cpt,dp1+(FCoord)(cpt - base),n,b));
     }
    else delete pt;
}


FCoord CPlan3D::GetDistancePt(CVector4 Nf, CVector4 pt)
{
    FCoord norm = sqrtl(Nf.x*Nf.x+Nf.y*Nf.y+Nf.z*Nf.z);
    FCoord den = Nf.x*pt.x + Nf.y*pt.y + Nf.z*pt.z + Nf.w;
    FCoord dis = den / norm;
    dis = fabsl(dis);
    return dis;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPlan3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CPlan3D::CPlan3D(CPoint3D *s1,CPoint3D *s2,CPoint3D *s3) : CObject3D()
{
    P1 = s1;
    P2 = s2;
    P3 = s3;
    VecList=0;
    bUpdateMe = 0;
    if (P1 && P2)
        nDepth = max(s1->nDepth,max(s2->nDepth,s3->nDepth))+1;
}

CPlan3D::CPlan3D(CPoint3D *s1,CVector4 plNorm): CObject3D()
{
    P1 = s1;
    P2 = 0;
    P3 = 0;
    VecNorm = plNorm;
    VecList=0;
    nDepth = s1->nDepth+1;
    bUpdateMe = 1;

}

CPlan3D::CPlan3D(const CObject3D & src) : CObject3D(src)
{
    P1 =  ((CPlan3D&)src).P1;
    P2 =   ((CPlan3D&)src).P2;
    P3 =   ((CPlan3D&)src).P3;
    VecNorm = ((CPlan3D&)src).VecNorm;
    VecList=0;
    bUpdateMe = 0;

}

int CPlan3D::SetDepth()
{
    if (P1 && P2 && P3)
        nDepth = max(P1->nDepth,max(P2->nDepth,P3->nDepth))+1;
    else if (P1 && !P2 && !P3)
        nDepth = P1->nDepth+1;
    return nDepth;
}


CPlan3D::~CPlan3D()
{
}

BOOL CPlan3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
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
    else if (P3 == pOld)
    {
        SetInGraph(FALSE);
        P3 = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}


void CPlan3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << VecNorm;
        ar << ((P1) ? P1->nObjectId : -1);
        ar << ((P2) ? P2->nObjectId : -1);
        ar << ((P3) ? P3->nObjectId : -1);
    }
    else
    {
        ar >> VecNorm;

        P1 = (CPoint3D*)SerializeObj(ar);
        P2 = (CPoint3D*)SerializeObj(ar);
        P3 = (CPoint3D*)SerializeObj(ar);
        p1 = CVector4(0,0,0);
        p2 = CVector4(0,0,0);
        p3 = CVector4(0,0,0);
        p4 = CVector4(0,0,0);
    }
}


CObject3D* CPlan3D::CopyObject()
{
    CObject3D *temp = new CPlan3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CPlan3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(P1);
    if (P2) list->Add(P2);
    if (P3) list->Add(P3);
    return list;
}

CString CPlan3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (P1) sn1 = P1->GetObjectHelp();
    if (P2) sn2 = P2->GetObjectHelp();
    if (P3) sn3 = P3->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2,sn3);
    return mstr;

}

BOOL CPlan3D::IsEqual(CObject3D &o)
{
    if (!o.MaskObject(TAllPlanClass)) return false;

    CVector4 n1 = GetNormalForm();
    CVector4 n2 = ((CPlan3D &)o).GetNormalForm();
    CVector4 d1 = n2-n1;
    d1.z = 0.;
    CVector4 d2 = n2-n1;
    d2.z = n2.w - n1.w;
    return (d1.NullVector() && d2.NullVector());
}

BOOL CPlan3D::IsInActiveArea(CPoint thePt)
{
    CRgn *p1,*p2,*p3,*p4;
    p1 = DoSegRgn(tp1,tp2);
    p2 = DoSegRgn(tp2,tp3);
    p3 = DoSegRgn(tp3,tp4);
    p4 = DoSegRgn(tp4,tp1);

    BOOL bRet = p1->PtInRegion(thePt) ||
        p2->PtInRegion(thePt) ||
        p3->PtInRegion(thePt) ||
        (p4->m_hObject && p4->PtInRegion(thePt));

    delete p1;
    delete p2;
    delete p3;
    delete p4;
    return bRet;
}


UINT CPlan3D::IsParallelTo(CObject3D *pObj)
{
    UINT res = VER_ERROR;

    if (!pObj) return res;
    if (!bValidate || !pObj->bValidate) return res;

    CDroite3D *pDr = DYNAMIC_DOWNCAST(CDroite3D,pObj);
    CPlan3D *pPl = DYNAMIC_DOWNCAST(CPlan3D,pObj);

    if (pPl)
    {
        CVector4 pV1 = VecNorm;
        CVector4 pV2 = pPl->VecNorm;

        CVector4 pVC = pV1 % pV2;

        return (pVC.NullVector()) ? VER_PAR_ANALYTIC : VER_PAR_NO ;
    }
    if (pDr) return pDr->IsParallelTo(this);

    return VER_ERROR;
}

UINT CPlan3D::IsPerpendicularTo(CObject3D *pObj)
{
    UINT res = VER_ERROR;

    if (!pObj) return res;
    if (!bValidate || !pObj->bValidate) return res;

    CDroite3D *pDr = DYNAMIC_DOWNCAST(CDroite3D,pObj);
    CPlan3D *pPl = DYNAMIC_DOWNCAST(CPlan3D,pObj);

    if (pPl)
    {
        CVector4 pV1 = VecNorm;
        CVector4 pV2 = pPl->VecNorm;

        FCoord pVC = pV1 * pV2;
        return (FCZero(pVC)) ? VER_PERP_ANALYTIC : VER_PERP_NO ;
    }
    if (pDr) return pDr->IsPerpendicularTo(this);

    return VER_ERROR;
}



CRgn* CPlan3D::InvalideRect()
{
    CRgn *p1,*p2,*p3,*p4;
    p1 = DoSegRgn(tp1,tp2);
    p2 = DoSegRgn(tp2,tp3);
    p3 = DoSegRgn(tp3,tp4);
    p4 = DoSegRgn(tp4,tp1);

    CRgn *rgU1 = new CRgn();
    rgU1->CreateRectRgn(0,0,0,0);
    rgU1->CopyRgn(p1);
    rgU1->CombineRgn(rgU1,p2,RGN_OR);
    rgU1->CombineRgn(rgU1,p3,RGN_OR);
    rgU1->CombineRgn(rgU1,p4,RGN_OR);

    delete p1;
    delete p2;
    delete p3;
    delete p4;

    return rgU1;
}

#define MRG_ZERO 1.0e-8


UINT  CPlan3D::CalculConceptuel()
{
    if (P2 && P3)
        bValidate = ((P1->bValidate) && (P2->bValidate) && (P3->bValidate));
//  else
//      Validate = P1->Validate;
    if (!bValidate)
        return ERR_NOPLAN;

    CVector4 U,V;
    if (P2 && P3)
     {  U = P2->Concept_pt - P1->Concept_pt;
        V = P3->Concept_pt - P1->Concept_pt;
        VecNorm =U % V;
        plRep.I = U;// * (1 / U.Norme());
        plRep.J = V;// * (1 / V.Norme());
        plRep.K = VecNorm;// * (1 / VecNorm.Norme());
        plRep.O = P1->Concept_pt;
     }
    if (VecNorm.NullVector())
     {
        bValidate = FALSE;
        return ERR_PTSALIGN;
     }

    if (!P2 || !P3)
     {
        FCoord normK = 1 / VecNorm.Norme();
        CVector4    K = VecNorm * normK;
        V = CVector4(-K.y,K.x,0,1);
        normK = V.Norme();
        if (FCZero(normK))
            { V.x = V.z =0; V.y = 1;    }
        else
            V = V * (1/normK);
        U = V % K;
        plRep.I = U;
        plRep.J = V;
        plRep.K = VecNorm;
        plRep.O = P1->Concept_pt;
     }

    {
        CVector4    U = plRep.I,
                    V = plRep.J;
        FCoord normK = 1 / VecNorm.Norme();
        CVector4    K = VecNorm * normK;
        CVector4    J(K.y,-K.x,0,1);
        normK = J.Norme();
        if (FCZero(normK))
            { J.x = J.z =0; J.y = 1;    }
        else
            J = J * (-1/normK);
        CVector4    I = J % K;

        ptonRep.I = I;
        ptonRep.J = J;
        ptonRep.K = K;
        ptonRep.O = P1->Concept_pt;
    }

    bValidate=TRUE;
    bUpdateMe = TRUE;
    return 0;
}

//////////////////////////////////////////////////////////////////////
/// Method CPlan3D::GetProjectedMinMax
///
/// \param pObj (CObject3D)     The object to project to the plan
/// \param P1 (CVector4)        The center of the plan
/// \param I (CVector4)         The I-axis of the plan local rep
/// \param J (CVector4)         The J-axis of the plan local rep
/// \param K (CVector4)         The K-axis of the plan local rep
/// \param minmax (CVector4)    Contains the min on (x,y) and max on (x,y)
//////////////////////////////////////////////////////////////////////
void CPlan3D::GetProjectedMinMax(CObject3D* pObj,CVector4 P1,CVector4 I,CVector4 J,CVector4 K, CVector4& minmax)
{
    if (!pObj) return;

    //////////////
    if (pObj->MaskObject(TCercleInterPS3D))
    {
		FCoord dd = ((CCercleInterPS3D*)pObj)->SP->Rayon;
	    minmax.x = min(minmax.x,-dd);
		minmax.y = min(minmax.y,-dd);
		minmax.z = max(minmax.z,dd);
		minmax.w = max(minmax.w,dd);
        return;
    }

    CVector4 thept;

    if (pObj->MaskObject(TAllPointClass))
    {
        CPoint3D* i = (CPoint3D *)(pObj);
        if (!i) return;
        thept =  i->Concept_pt;
    }
    else if (pObj->MaskObject(TDroitePerp3DClass))
    {
        CDroitePerp3D *dr = ((CDroitePerp3D*)pObj);
        if (!dr) return;
        thept = dr->GetIntersectionPoint();
    }
    else if (pObj->MaskObject(TDroiteInterPP3DClass))
    {
        //CDroiteInterPP3D *dr = ((CDroiteInterPP3D*)pObj);
        //if (!dr) return;
        //thept = dr->PtonDr;
        return;
    }
    else return;

    CVector4 rr= thept - P1;
    CVector4 ap3(I * rr, J * rr, K * rr);
    minmax.x = min(minmax.x,ap3.x);
    minmax.y = min(minmax.y,ap3.y);
    minmax.z = max(minmax.z,ap3.x);
    minmax.w = max(minmax.w,ap3.y);
}

void CPlan3D::CalculVisuel(CVisualParam *pVisParam)
{
    if (!bValidate) return;
    if (bUpdateMe)
     {
        CVector4    U = plRep.I,
                    V = plRep.J;
        FCoord normK = 1 / VecNorm.Norme();
        CVector4    K = VecNorm * normK;
        CVector4    J(-K.y,K.x,0,1);
        normK = J.Norme();
        if (FCZero(normK))
            { J.x = J.z =0; J.y = 1;    }
        else
            J = J * (1/normK);
        CVector4    I = J % K;

        CVector4 ap1(0,0,0);
        CVector4 ap2(I * U, J * U, K * U);
        CVector4 ap3(I * V, J * V, K * V);
        planeBorder = CVector4(min(ap1.x,min(ap2.x,ap3.x)),
								min(ap1.y,min(ap2.y,ap3.y)),
								max(ap1.x,max(ap2.x,ap3.x)),
								max(ap1.y,max(ap2.y,ap3.y)));

        if (isA()==TPlanPerp3DClass)
        {
            CPlanPerp3D* mypl = (CPlanPerp3D*)this;
            if (mypl)
             {
                CVector4 thept = mypl->IntPt;
                CVector4 rr= thept - P1->Concept_pt;
                CVector4 ap3(I * rr, J * rr, K * rr);
                planeBorder.x = min(planeBorder.x,ap3.x);
                planeBorder.y = min(planeBorder.y,ap3.y);
                planeBorder.z = max(planeBorder.z,ap3.x);
                planeBorder.w = max(planeBorder.w,ap3.y);
            }
        }
        int nbobj = cDependList.GetSize();
        int nbobj2 = cDependList.GetUpperBound();
        for (int i=0;i<nbobj;i++)
        {
            CObject3D *pObj = cDependList.GetAt(i);
            if (!pObj) continue;

            /*if (    pObj->MaskObject(TAllPointClass) ||
                    pObj->MaskObject(TDroitePerp3DClass) || //)
                    pObj->MaskObject(TCercleInterPS3D) )*/

                if (pObj->bVisible && (pObj->bValidate))
                {
					if (pObj->MaskObject(TInterCircPl3DClass))
					{
						GetProjectedMinMax(((CInterCircPlane3D*)pObj)->ptA,P1->Concept_pt,I,J,K,planeBorder);
						GetProjectedMinMax(((CInterCircPlane3D*)pObj)->ptB,P1->Concept_pt,I,J,K,planeBorder);
					}
					else
						GetProjectedMinMax(pObj,P1->Concept_pt,I,J,K,planeBorder);
                }
        }
        bUpdateMe = 0;
        planeBorder.x-=PLANEMARGE;
        planeBorder.y-=PLANEMARGE;
        planeBorder.z+=PLANEMARGE;
        planeBorder.w+=PLANEMARGE;
        CVector4 lim[4] = {
                CVector4(planeBorder.x,planeBorder.y,0),
                CVector4(planeBorder.x,planeBorder.w,0),
                CVector4(planeBorder.z,planeBorder.w,0),
                CVector4(planeBorder.z,planeBorder.y,0)};
        for (int t=0;t<4;t++)
         {
            CVector4 U = lim[t];
            lim[t].x =  I.x*U.x + J.x*U.y + K.x*U.z + P1->Concept_pt.x,
            lim[t].y =  I.y*U.x + J.y*U.y + K.y*U.z + P1->Concept_pt.y,
            lim[t].z =  I.z*U.x + J.z*U.y + K.z*U.z + P1->Concept_pt.z,
            lim[t].w =  1;
        }
        p1 = lim[0];
        p2 = lim[1];
        p3 = lim[2];
        p4 = lim[3];

/*      re.x+=PLANEMARGE;
        re.y+=PLANEMARGE;
        re.z-=PLANEMARGE;
        re.w-=PLANEMARGE;
        for (i=0;i<nbobj;i++)
        {
            CObject3D *pObj = cDependList.GetAt(i);
            if (!pObj) continue;

            if (pObj->MaskObject(TDroiteInterPP3DClass))

                if (pObj->bVisible && (pObj->bValidate))
                {
                    GetMinMax(pObj,this,P1->Concept_pt,I,J,K,re);
                }
        }
        re.x-=PLANEMARGE;
        re.y-=PLANEMARGE;
        re.z+=PLANEMARGE;
        re.w+=PLANEMARGE;
        CVector4 lim2[4] = {
                CVector4(re.x,re.y,0),
                CVector4(re.x,re.w,0),
                CVector4(re.z,re.w,0),
                CVector4(re.z,re.y,0)};
        for (t=0;t<4;t++)
         {
            CVector4 U = lim2[t];
            lim2[t].x = I.x*U.x + J.x*U.y + K.x*U.z + P1->Concept_pt.x,
            lim2[t].y = I.y*U.x + J.y*U.y + K.y*U.z + P1->Concept_pt.y,
            lim2[t].z = I.z*U.x + J.z*U.y + K.z*U.z + P1->Concept_pt.z,
            lim2[t].w = 1;
        }
        p1 = lim2[0];
        p2 = lim2[1];
        p3 = lim2[2];
        p4 = lim2[3];*/
    }
    tp1 = (CPoint) pVisParam->ProjectPoint(p1);
    tp2 = (CPoint) pVisParam->ProjectPoint(p2);
    tp3 = (CPoint) pVisParam->ProjectPoint(p3);
    tp4 = (CPoint) pVisParam->ProjectPoint(p4);

//  ptonRep.I = (p2 - p1) * (1/(p2-p1).Norme());
//  ptonRep.J = (p4 - p1) * (1/(p4-p1).Norme());
//  ptonRep.K = VecNorm * (1/VecNorm.Norme());
//  ptonRep.O = p1;
}




/*bool testv(CVector4 o,CVisualParam *myVisuParamExt)
{
    bool t=0;
    switch (myVisuParamExt->nVisuKind)
     {
        case CVisualParam::VisuPlane:
            t = (o.z < -MRG_ZERO);
            break;
        case CVisualParam::VisuClois:
            t = (   (o.x < -MRG_ZERO) ||
                    (o.y < -MRG_ZERO) ||
                    (o.z < -MRG_ZERO));
            break;
        default :
            t = 0;
            break;
     }
    return t;
}*/

int codeCSx(CVector4 pt)
{
    int c = 0;
    if (pt.y < 0.) c |= 1;
    if (pt.z < 0.) c |= 4;
    return c;
}

int codeCSy(CVector4 pt)
{
    int c = 0;
    if (pt.x < 0.) c |= 1;
    if (pt.z < 0.) c |= 4;
    return c;
}

int codeCSz(CVector4 pt)
{
    int c = 0;
    if (pt.x < 0.) c |= 1;
    if (pt.y < 0.) c |= 4;
    return c;
}

 int codeCS(int code,CVector4 cA)
{
    int ccA;
    if (code==1) ccA =codeCSx(cA);
    if (code==2) ccA =codeCSy(cA);
    if (code==3) ccA =codeCSz(cA);
    return ccA;
}

CVector4 changept(int code,int ccA,CVector4 cA,CVector4 nf)
{
    FCoord  x = -nf.w / nf.x,
                y = -nf.w / nf.y,
                z = -nf.w / nf.z;

    switch (code)
     {  case 1:
            if (ccA == 5)
                if (y > 0.)
                    cA = CVector4(0.,y, 0.);
                else
                    cA = CVector4(0.,0.,z);
            else
            if (ccA == 4)
                cA = CVector4(0.,-nf.w / nf.y, 0.);
            else
                cA = CVector4(0.,0.,-nf.w / nf.z);
            break;
      case 2:
            if (ccA == 5)
                if (x > 0.)
                    cA = CVector4(x,0.,0.);
                else
                    cA = CVector4(0.,0.,z);
            else
            if (ccA == 4)
                cA = CVector4(-nf.w / nf.x, 0., 0.);
            else
                cA = CVector4(0.,0.,-nf.w / nf.z);
            break ;
      case 3:
            if (ccA == 5)
                if (x > 0.)
                    cA = CVector4(x,0., 0.);
                else
                    cA = CVector4(0.,y,0.);
            else
            if (ccA == 4)
                cA = CVector4(-nf.w / nf.x, 0., 0.);
            else
                cA = CVector4(0.,-nf.w / nf.y, 0.);
            break  ;
     }
    return cA;
}


void MyDraw22(int code,CPoint3D  *myA[4],int *nbpt,CDC* HandleDC,
                    CVisualParam *mV, CVector4 nf,BOOL bDraw)
{
    CVector4 cA,cB;
    if (*nbpt >= 2 && bDraw)
     {
        cA = (myA[0])->Concept_pt,
        cB = (myA[1])->Concept_pt;

        int     ccA=codeCS(code,cA),
                ccB=codeCS(code,cB),
                ccC = ccA & ccB;

        if (!ccC)
         {
            if (ccA)    cA = changept(code,ccA,cA,nf);
            if (ccB)    cB = changept(code,ccB,cB,nf);

            ccA=codeCS(code,cA);
            ccB=codeCS(code,cB);
            if (!ccA && !ccB)
             {

            CPen toto(PS_DOT,1,RGB(0,0,0));
            CPen *oldP = HandleDC->SelectObject(&toto);

            CPoint aa1 = (CPoint)mV->ProjectPoint(cA);
            CPoint aa2 = (CPoint)mV->ProjectPoint(cB);
            HandleDC->MoveTo(aa1);
            HandleDC->LineTo(aa2);
            HandleDC->SelectObject(oldP );
        }}
     }
    for (int i = 0;i<4;i++)
        if (myA[i])
         { delete myA[i];
            myA[i]=0;
         }
    *nbpt=0;
}



void CPlan3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

    CBrush curBrush(pObjectShape.clrObject);
    CPen curPen,disPen;
    curPen.CreatePenIndirect(&(pObjectShape.GetPenStyle()));
    disPen.CreatePenIndirect(&(pObjectShape.GetHiddenPenStyle(
                                    pObjectShape.GetObjectHiddenColor())));
// 	COLORREF	curC = RGB(0,0,0),
// 				curC2 = RGB(192,192,192);
// 	CPen	curP(PS_DOT,1,pObjectShape.GetObjectColor()); 
// 	CPen	curP2(PS_DOT,1,curC2); 
// 	CPen	curPa(PS_SOLID,1,RGB(255,0,0)); 
// 	CPen	curPb(PS_SOLID,1,RGB(0,255,0)); 
// 	CPen	curPc(PS_SOLID,1,RGB(0,0,255)); 
// 	CPen	curPd(PS_SOLID,1,RGB(255,255,0)); 

    CVector4 nf = GetNormalForm();
    if (bSm)
     {
        // Draw simplified shape
        CPen *oldP = pDC->SelectObject(&curPen);
        pDC->MoveTo(tp1);
        pDC->LineTo(tp2);
        pDC->LineTo(tp3);
        pDC->LineTo(tp4);
        pDC->LineTo(tp1);
        pDC->SelectObject(oldP);
        return;
     }

// 	int nb = 0;
// 	CVector4 vec1,vec2;
//     CPen *oldP = pDC->SelectObject(&curP);
// 	if (mV->nVisuKind == CVisualParam::VisuClois) 
// 	{
// 		int res = CGeom::TTintersect_with_isectline(p1,p2,p4,CVector4(0,0,0),CVector4(1000,0,0),CVector4(0,0,1000),nb,vec1,vec2);
// 		if (res==1)
// 		{
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p2,p3,p4,CVector4(0,0,0),CVector4(1000,0,0),CVector4(0,0,1000),nb,vec1,vec2);
// 		if (res==1)
// 		{
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 
// 		res = CGeom::TTintersect_with_isectline(p2,p3,p4,CVector4(0,0,0),CVector4(0,1000,0),CVector4(0,0,1000),nb,vec1,vec2);
// 		if (res==1)
// 		{
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p1,p2,p4,CVector4(0,0,0),CVector4(0,1000,0),CVector4(0,0,1000),nb,vec1,vec2);
// 		if (res==1)
// 		{
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p2,p3,p4,CVector4(0,0,0),CVector4(1000,0,0),CVector4(0,1000,0),nb,vec1,vec2);
// 		if (res==1)
// 		{
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p1,p2,p4,CVector4(0,0,0),CVector4(1000,0,0),CVector4(0,1000,0),nb,vec1,vec2);
// 		if (res==1)
// 		{
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 	}
//     if (mV->nVisuKind == CVisualParam::VisuPlane )
// 	{
// 		int x = TPref::TUniv.nRepPas*TPref::TUniv.nUnitRep*10;
// 		int res = CGeom::TTintersect_with_isectline(p1,p2,p4,CVector4(-x,-x,0),CVector4(x,-x,0),CVector4(-x,x,0),nb,vec1,vec2);
// 		if (res==1)
// 		{ pDC->SelectObject(&curPa);
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p2,p3,p4,CVector4(-x,-x,0),CVector4(x,-x,0),CVector4(-x,x,0),nb,vec1,vec2);
// 		if (res==1)
// 		{ pDC->SelectObject(&curPb);
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p2,p3,p4,CVector4(x,x,0),CVector4(x,-x,0),CVector4(-x,x,0),nb,vec1,vec2);
// 		if (res==1)
// 		{ pDC->SelectObject(&curPc);
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 		res = CGeom::TTintersect_with_isectline(p1,p2,p4,CVector4(x,x,0),CVector4(x,-x,0),CVector4(-x,x,0),nb,vec1,vec2);
// 		if (res==1)
// 		{ pDC->SelectObject(&curPd);
// 					CPoint aa1 = (CPoint)mV->ProjectPoint(vec1);
// 					CPoint aa2 = (CPoint)mV->ProjectPoint(vec2);
// 					pDC->MoveTo(aa1);
// 					pDC->LineTo(aa2);
// 		}
// 	}
//         pDC->SelectObject(oldP);

    // Calculate adaptative shape
    FCoord  dp1 = 0,
            dp2 = (FCoord)(p2 - p1),
            dp3 = dp2 + (FCoord)(p3 - p2),
            dp4 = dp3 + (FCoord)(p4 - p3),
            dp5 = dp4 + (FCoord)(p1 - p4);
    CVector4    pNorm = GetNormalForm();
    CVector4    oeil(mV->GetEyePos());
    oeil = oeil * 100.;
    CPoint3D    vispt(oeil);
    CVector4    origin(0,0,0,1);
    CVector4    VisuNorm= vispt.Concept_pt - origin;
    CPlan3D vispl(&vispt,VisuNorm);

    UINT hh = vispl.CalculConceptuel();
    int theside=-1,howmuch=-4;
    FCoord base;
    if (!hh)
     {
        CVector4    pNorm1 = vispl.GetNormalForm();
        FCoord  m1 = CPlan3D::GetDistancePt(pNorm1,p1);

        base = m1;
        theside=1;
        howmuch=4;
        FCoord  m2 = CPlan3D::GetDistancePt(pNorm1,p2);
        if (m2 == base) howmuch=theside;
        if (m2 < base)
            { base = m2;theside = 2;howmuch=1;}
        FCoord  m3 = CPlan3D::GetDistancePt(pNorm1,p3);
        if (m3 == base) howmuch=theside;
        if (m3 < base)
            { base = m3;theside = 3;howmuch=2;}
        FCoord  m4 = CPlan3D::GetDistancePt(pNorm1,p4);
        if (m4 == base) howmuch=theside;
        if (m4 < base)
            { base = m4;theside = 4;howmuch=3;}
        if (m1==m2 && m3==m4 && m2==m3)
            howmuch=theside = -1;
     }

    CxVectorSSSet mylist;

    mylist.AddSorted(CVector4SSide(p1,dp1,1,
            (theside == 1 || howmuch==1 || theside == 4 || howmuch==4)));
    mylist.AddSorted(CVector4SSide(p2,dp2,2,
            (theside == 2 || howmuch==2 || theside == 1 || howmuch==1)));
    mylist.AddSorted(CVector4SSide(p3,dp3,3,
            (theside == 2 || howmuch==2 || theside == 3 || howmuch==3)));
    mylist.AddSorted(CVector4SSide(p4,dp4,4,
            (theside == 3 || howmuch==3 || theside == 4 || howmuch==4)));
    mylist.AddSorted(CVector4SSide(p1,dp5,1,
            (theside == 4 || howmuch==4 || theside == 1 || howmuch==1)));

    if (mV->nVisuKind == CVisualParam::VisuPlane)
     {
        //CVector4 *A = 0,*B = 0;
        CPoint3D Ap2(p2);
        CPoint3D Ap3(p3);
        CSegment3D p2p3(&Ap2,&Ap3);

        CPointInterDP3D Hp2p3(&p2p3,mV->H);
        UINT r = Hp2p3.CalculConceptuel();
        if (!r)
         {
            CVector4 A(Hp2p3.Concept_pt);
            CVector4 B(A + (p4 - p3));
            if (bMarked && mV->bDrawMark)
             {
                //TPen toto(ObjectShape->ObjectColor,1,PS_DOT);
                CPen toto(PS_DOT,1,RGB(0,0,0));
                CPen *oldP = pDC->SelectObject(&toto);

                CPoint aa1 = (CPoint)mV->ProjectPoint(A);
                CPoint aa2 = (CPoint)mV->ProjectPoint(B);
                pDC->MoveTo(aa1);
                pDC->LineTo(aa2);
                pDC->SelectObject(oldP);
             }
            mylist.AddSorted(CVector4SSide(A,dp2,2,(theside == 2 || howmuch==2)));
            mylist.AddSorted(CVector4SSide(B,dp4,4,(theside == 4 || howmuch==4)));
         }
     }
    else if (mV->nVisuKind == CVisualParam::VisuClois)
     {

        CDroiteInterPP3D d1(this,mV->F);
        CDroiteInterPP3D d2(this,mV->G);
        CDroiteInterPP3D d3(this,mV->H);

        UINT r1 = d1.CalculConceptuel();
        UINT r2 = d2.CalculConceptuel();
        UINT r3 = d3.CalculConceptuel();

        CPoint3D Ap1(p1);
        CPoint3D Ap2(p2);
        CPoint3D Ap3(p3);
        CPoint3D Ap4(p4);
        CSegment3D p1p2(&Ap1,&Ap2);
        CSegment3D p2p3(&Ap2,&Ap3);
        CSegment3D p3p4(&Ap3,&Ap4);
        CSegment3D p4p1(&Ap4,&Ap1);

        CPoint3D  *myA[4]={0,0,0,0};
        int nbpt=0;

        if (!r1)
         {
            mylist.AddtoList(&d1,&p1p2,myA,&nbpt,dp1,p1, 1,(theside == 1 || howmuch==1));
            mylist.AddtoList(&d1,&p2p3,myA,&nbpt,dp2,p2, 2,(theside == 2 || howmuch==2));
            mylist.AddtoList(&d1,&p3p4,myA,&nbpt,dp3,p3, 3,(theside == 3 || howmuch==3));
            mylist.AddtoList(&d1,&p4p1,myA,&nbpt,dp4,p4, 4,(theside == 4 || howmuch==4));
            MyDraw22(1,myA,&nbpt,pDC,mV,nf,(bMarked && mV->bDrawMark));
        }
        if (!r2)
         {
            mylist.AddtoList(&d2,&p1p2,myA,&nbpt,dp1,p1, 1,(theside == 1 || howmuch==1));
            mylist.AddtoList(&d2,&p2p3,myA,&nbpt,dp2,p2, 2,(theside == 2 || howmuch==2));
            mylist.AddtoList(&d2,&p3p4,myA,&nbpt,dp3,p3, 3,(theside == 3 || howmuch==3));
            mylist.AddtoList(&d2,&p4p1,myA,&nbpt,dp4,p4, 4,(theside == 4 || howmuch==4));
            MyDraw22(2,myA,&nbpt,pDC,mV,nf,(bMarked && mV->bDrawMark));
         }

        if (!r3)
         {
            mylist.AddtoList(&d3,&p1p2,myA,&nbpt,dp1,p1, 1,(theside == 1 || howmuch==1));
            mylist.AddtoList(&d3,&p2p3,myA,&nbpt,dp2,p2, 2,(theside == 2 || howmuch==2));
            mylist.AddtoList(&d3,&p3p4,myA,&nbpt,dp3,p3, 3,(theside == 3 || howmuch==3));
            mylist.AddtoList(&d3,&p4p1,myA,&nbpt,dp4,p4, 4,(theside == 4 || howmuch==4));
            MyDraw22(3,myA,&nbpt,pDC,mV,nf,(bMarked && mV->bDrawMark));
         }

        //gpc_free_polygon(&subject);
     }

    int nbItem = mylist.GetSize();
    BOOL bfirst =TRUE;
    CVector4SSide start;
    for (int i=0;i<nbItem;i++)
    {
        CVector4SSide current = mylist.GetAt(i);
        if (bfirst)
        {
            bfirst = FALSE;
            start = current;
        }
        else
         {
            CPoint aa1 = (CPoint)mV->ProjectPoint(start);
            CPoint aa2 = (CPoint)mV->ProjectPoint(current);
            //BOOL insi = !(testv(start,mV) || testv(current,mV));
            BOOL bInside = mV->IsPointVisible(start) && mV->IsPointVisible(current);
	        //ASSERT(insi == insi2);
            LOGPEN plogp;
            (bInside ? curPen.GetLogPen(&plogp) : disPen.GetLogPen(&plogp));
            CPoint ppp(2,2);
            if (start.vis && current.vis && plogp.lopnStyle == PS_SOLID)
             { 
				// Set the visible border to twice the thickness of the plane border
				plogp.lopnWidth.x = plogp.lopnWidth.x*2;
                plogp.lopnWidth.y = plogp.lopnWidth.y*2;
                //plogp.lopnWidth = ppp;
             }
            CPen mypen2;
            mypen2.CreatePenIndirect(&plogp);
            CPen *oldP = pDC->SelectObject(&mypen2);
            pDC->MoveTo(aa1);
            pDC->LineTo(aa2);
            start = current;
            pDC->SelectObject(oldP);
         }
    }
    if (bMarked && mV->bDrawMark)
    {
        DrawRetro(pDC,mV);
        //Graham gm(myiter,mV);
    }

    if (bIsSelected)
     {
        int         Ds = TPref::TPoint.nSize;
        CRect theR1(tp1.x-Ds,tp1.y-Ds,tp1.x+Ds,tp1.y+Ds);
        CRect theR2(tp2.x-Ds,tp2.y-Ds,tp2.x+Ds,tp2.y+Ds);
        CRect theR3(tp3.x-Ds,tp3.y-Ds,tp3.x+Ds,tp3.y+Ds);
        CRect theR4(tp4.x-Ds,tp4.y-Ds,tp4.x+Ds,tp4.y+Ds);
        pDC->InvertRect(theR1);
        pDC->InvertRect(theR2);
        pDC->InvertRect(theR3);
        pDC->InvertRect(theR4);
     }

    /* DEBUG
    CRect mrect(tp1,tp1);
    mrect.InflateRect(3,3);
    pDC->Rectangle(mrect);
    mrect = CRect(tp2,tp2);
    mrect.InflateRect(3,3);
    pDC->Ellipse(mrect);

    CPoint ptO = (CPoint)mV->ProjectPoint(ptonRep.O);
    CPoint ptA1 = (CPoint)mV->ProjectPoint(ptonRep.O + (ptonRep.I * 50.));
    CPoint ptA2 = (CPoint)mV->ProjectPoint(ptonRep.O + (ptonRep.J * 150.));
    pDC->MoveTo(ptO);
    pDC->LineTo(ptA1);
    pDC->MoveTo(ptO);
    pDC->LineTo(ptA2);
    */
}

void CPlan3D::GetRange(CVector4 &min,CVector4 &max)
{
    CObject3D::GetRange(min,max);

    if (bValidate && IsVisible())
    {

    min.x = min(min.x,p1.x);
    min.x = min(min.x,p2.x);
    min.x = min(min.x,p3.x);
    min.x = min(min.x,p4.x);

    min.y = min(min.y,p1.y);
    min.y = min(min.y,p2.y);
    min.y = min(min.y,p3.y);
    min.y = min(min.y,p4.y);

    min.z = min(min.z,p1.z);
    min.z = min(min.z,p2.z);
    min.z = min(min.z,p3.z);
    min.z = min(min.z,p4.z);

    max.x = max(max.x,p1.x);
    max.x = max(max.x,p2.x);
    max.x = max(max.x,p3.x);
    max.x = max(max.x,p4.x);

    max.y = max(max.y,p1.y);
    max.y = max(max.y,p2.y);
    max.y = max(max.y,p3.y);
    max.y = max(max.y,p4.y);

    max.z = max(max.z,p1.z);
    max.z = max(max.z,p2.z);
    max.z = max(max.z,p3.z);
    max.z = max(max.z,p4.z);
    }
}



CString CPlan3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && P2 && P3)
    {
        CString strName,strObj1,strObj2,strObj3;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = P2->GetObjectNameRedux();
		strObj3 = P3->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("PlaneD[%s,%s,%s,%s];"),strName,strObj1,strObj2,strObj3);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("PlaneD(%s,%s,%s,%s);"),strName,strObj1,strObj2,strObj3);

    }
    return mstr;
}

void CPlan3D::DrawRetro(CDC* pDC,CVisualParam *mV)
{
        // dessin du vecteur normal
    CVector4 ff = VecNorm;
    CVector4 oeil= mV->GetEyePos();
    CVector4 origin(0,0,0,1);
    CVector4 VisuNorm= oeil - ((mV->bParProj) ? origin : origin);
    if (!ff.NullVector() && !VisuNorm.NullVector())
     {
        FCoord ggg= VisuNorm * VecNorm;
        CPen penV(PS_SOLID,1,RGB(0,0,0));
        CPen penI(PS_SOLID,1,RGB(192,192,192));
        CPen *oldP=NULL;
        if (ggg >= 0)
            oldP = pDC->SelectObject(&penV);
        else
            oldP = pDC->SelectObject(&penI);
        CVector4 u = ff * (1/ff.Norme());

        // vecteur
        CVector4 p1start = p1+ (p3 - p1) * 0.5;
        CVector4 p1end =  p1start+ u * 100.;
        CPoint aa1 = (CPoint)mV->ProjectPoint(p1start);
        CPoint aa2 = (CPoint)mV->ProjectPoint(p1end);
        pDC->MoveTo(aa1);
        pDC->LineTo(aa2);

        // fleche
        CVector4 p1p2 = p2 - p1;
        p1p2 = p1p2 * (1/p1p2.Norme());
        CVector4 p1end2 =  p1start+u * 90.;
        p1end2 =  p1end2+ p1p2 * 5.;
        aa1 = (CPoint)mV->ProjectPoint(p1end2);
        pDC->MoveTo(aa1);
        pDC->LineTo(aa2);
        p1end2 =  p1start + u * 90.;
        p1end2 =  p1end2 + p1p2 * -5.;
        aa1 = (CPoint)mV->ProjectPoint(p1end2);
        pDC->MoveTo(aa1);
        pDC->LineTo(aa2);
        pDC->SelectObject(oldP);
     }
}

void CPlan3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;

	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {255/255., 255/255.,100/255. , 1.0f};
    float mat_diffuse[] = {0.9f, 0.1f, 0.1f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	CVector4 vec=VecNorm;
	vec = vec.Normalized();
	GLdouble vertex[]=
	{
		(p1.x/TPref::TUniv.nUnitRep/3),
		(p1.y/TPref::TUniv.nUnitRep/3),
		(p1.z/TPref::TUniv.nUnitRep/3),
		(p2.x/TPref::TUniv.nUnitRep/3),
		(p2.y/TPref::TUniv.nUnitRep/3),
		(p2.z/TPref::TUniv.nUnitRep/3),
		(p3.x/TPref::TUniv.nUnitRep/3),
		(p3.y/TPref::TUniv.nUnitRep/3),
		(p3.z/TPref::TUniv.nUnitRep/3),
		(p4.x/TPref::TUniv.nUnitRep/3),
		(p4.y/TPref::TUniv.nUnitRep/3),
		(p4.z/TPref::TUniv.nUnitRep/3)
	};
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor3f(.0f,1.00f,.0f);
// 			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
// 			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
// 			glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
			glNormal3d(vec.x, vec.y, vec.z);   //N1
			glVertex3d( vertex[0], vertex[1], vertex[2]);   //V2
			glVertex3d( vertex[3], vertex[4], vertex[5]);   //V2
			glVertex3d( vertex[6], vertex[7], vertex[8]);   //V2
			glVertex3d( vertex[9], vertex[10], vertex[11]);   //V2
		glEnd();
	glPopMatrix();
}

/*CVector4 GetNormalForm(CVector4 Pt1,CVector4 Pt2,CVector4 Pt3)
{
    FCoord  x1 = Pt1.x,x2 = Pt2.x,x3 = Pt3.x,
                y1 = Pt1.y,y2 = Pt2.y,y3 = Pt3.y,
                z1 = Pt1.z,z2 = Pt2.z,z3 = Pt3.z;

    FCoord A = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
    FCoord B = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
    FCoord C = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);
    FCoord D = -( x1*(y2*z3 - y3*z2) + x2*(y3*z1 - y1*z3) + x3*(y1*z2 - y2*z1));
    FCoord N = sqrtl(A*A+B*B+C*C);
    FCoord a=1.0;
    //if (!N) ::MessageBox(0,"dss","dfsfsdffsd",MB_OK);

    if (D) a *= (D > MRG_ZERO) ? -1 : 1;
    else
    if (C) a*= (C > MRG_ZERO) ? 1 : -1;
    else
    a*= (B > MRG_ZERO) ? 1 : -1;
    N*=a;
    A/=N;
    B/=N;
    C/=N;
    D/=N;
    return CVector4(A,B,C,D);
}*/


CVector4 CPlan3D::GetNormalForm()
{
    /// equation du plan Ax+By+Cz+D=0
    FCoord A,B,C,D;
    if (P2 && P3)
     {  CVector4 Pt1 = P1->Concept_pt;
        CVector4 Pt2 = P2->Concept_pt;
        CVector4 Pt3 = P3->Concept_pt;
        A = Pt1.y*(Pt2.z - Pt3.z) + Pt2.y*(Pt3.z - Pt1.z) + Pt3.y*(Pt1.z - Pt2.z);
        B = Pt1.z*(Pt2.x - Pt3.x) + Pt2.z*(Pt3.x - Pt1.x) + Pt3.z*(Pt1.x - Pt2.x);
        C = Pt1.x*(Pt2.y - Pt3.y) + Pt2.x*(Pt3.y - Pt1.y) + Pt3.x*(Pt1.y - Pt2.y);
        D = -(Pt1.x*(Pt2.y*Pt3.z - Pt3.y*Pt2.z) + Pt2.x*(Pt3.y*Pt1.z - Pt1.y*Pt3.z)
                    + Pt3.x*(Pt1.y*Pt2.z - Pt2.y*Pt1.z));
     }
    else
     {
        CVector4 Pt1 = P1->Concept_pt;
        A = VecNorm.x;
        B = VecNorm.y;
        C = VecNorm.z;
        D = -(Pt1.x*VecNorm.x + Pt1.y*VecNorm.y + Pt1.z*VecNorm.z);
     }
    FCoord N = sqrtl(A*A+B*B+C*C);
    //if (!N) ::MessageBox(0,"classe","dffddf",MB_OK);
    FCoord a=1.0;
    if (D) a *= (D >= 0) ? -1 : 1;
    else
    if (C) a*= (C >= 0) ? 1 : -1;
    else
    a*= (B >=0) ? 1 : -1;
    N*=a;

    return CVector4(A/N,B/N,C/N,D/N);
}

void CPlan3D::HandleObjectError(int nErr,BOOL bShow)
{
    CObject3D::HandleObjectError(nErr,bShow);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPlanPerp3D, CPlan3D, VERSIONABLE_SCHEMA | 1)

CPlanPerp3D::CPlanPerp3D(CPoint3D *p1,CDroite3D *dr) : CPlan3D(p1,NULL,NULL)
{
    D = dr;
    nDepth = max(p1->nDepth,dr->nDepth)+1;
}

CPlanPerp3D::CPlanPerp3D(const CObject3D & src) : CPlan3D(src)
{
    D =  ((CPlanPerp3D&)src).D;
}

CPlanPerp3D::~CPlanPerp3D()
{
}

int CPlanPerp3D::SetDepth()
{
    if (P1 && D)
        nDepth = max(P1->nDepth,D->nDepth)+1;

    return nDepth;
}


void CPlanPerp3D::Serialize( CArchive& ar )
{
    CPlan3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << IntPt;
        ar << ((D) ? D->nObjectId : -1);
    }
    else
    {
        ar >> IntPt;
        D = (CDroite3D*)SerializeObj(ar);
    }
}



CString CPlanPerp3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (P1) sn1 = P1->GetObjectHelp();
    if (D) sn2 = D->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2,sn3);
    return mstr;

}

CObject3D* CPlanPerp3D::CopyObject()
{
    CObject3D *temp = new CPlanPerp3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CPlanPerp3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(P1);
    if (D)  list->Add(D);
    return list;
}

UINT  CPlanPerp3D::CalculConceptuel()
{
    if (P1 && D)
        bValidate = ((P1->bValidate) && (D->bValidate));
    if (!bValidate)
        return ERR_NOPLAN;
    VecNorm = D->CDirVector;
    UINT ret = CPlan3D::CalculConceptuel();

    if (bValidate)
     {
        CPointInterDP3D ptint(D,this);
        ptint.CalculConceptuel();
        IntPt = ptint.Concept_pt;
     }
    return ret;
}

void CPlanPerp3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;
    CPlan3D::Draw(pDC,mV,bSm);

    CVector4 dis = IntPt - P1->Concept_pt;
    if (!dis.NullVector())
     {
        COLORREF curC=RGB(0,0,0);
        CBrush br(curC);
        CPen curP(PS_SOLID,1,curC);
        CPen *oldP = pDC->SelectObject(&curP);
        CBrush *oldB = pDC->SelectObject(&br);
        CRect mrect((CPoint)mV->ProjectPoint(IntPt) - CSize(1,1),CSize(3,3));
        pDC->Rectangle(mrect);
        pDC->SelectObject(oldB);
        pDC->SelectObject(oldP );
     }

}


CString CPlanPerp3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && D)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = D->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("NormalPlane[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
		    mstr.Format(_T("NormalPlane(%s,%s,%s);"),strName,strObj1,strObj2);
    }
    return mstr;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPolygon3D, CPlan3D, VERSIONABLE_SCHEMA | 1)

CPolygon3D::CPolygon3D() : CPlan3D()
{
    pObjectShape = CShape(CShape::PolShape,1);
    m_pPointSet.RemoveAll();
}

CPolygon3D::CPolygon3D(CxObject3DSet *pSet) : CPlan3D()
{
    pObjectShape = CShape(CShape::PolShape,1);
    m_pPointSet.RemoveAll();
    m_pPointSet.Copy(*pSet);
}


CPolygon3D::CPolygon3D(const CObject3D & src) : CPlan3D(src)
{
    pObjectShape = CShape(CShape::PolShape,1);
    m_pPointSet.RemoveAll();
    m_pPointSet.Copy(((CPolygon3D&)src).m_pPointSet);
}

CObject3D* CPolygon3D::CopyObject()
{
    CObject3D *temp = new CPolygon3D((CObject3D&)*this);
    return temp;
}

CPolygon3D::~CPolygon3D()
{
}

int CPolygon3D::SetDepth()
{
    int nb = 0;
    for (int i=0;i<m_pPointSet.GetSize();i++)
    {
        nb = max(m_pPointSet.GetAt(i)->nDepth,nb);
    }
    nDepth = nb+1;
    return nDepth;
}

void CPolygon3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    int nb = m_pPointSet.GetSize();

    if (ar.IsStoring())
    {
        ar << VecNorm;
        ar << nb;

        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = m_pPointSet.GetAt(i);
            ar << ((pObj) ? pObj->nObjectId : -1);
        }
    }
    else
    {
        nb = 0;
        ar >> VecNorm;
        ar >> nb;
        m_pPointSet.RemoveAll();
        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = (CObject3D*)SerializeObj(ar);
            m_pPointSet.Add(pObj);
        }
    }
}

CxObject3DSet* CPolygon3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Copy(m_pPointSet);
    //list->Add(P1);
    return list;
}

//////////////////////////////////////////////////////////////////////
/// Get the polygons defined in the object.
///
/// \param pList    A pointer to the object list to fill in.
//////////////////////////////////////////////////////////////////////
void CPolygon3D::GetPolygons(CxObject3DSet* pList)
{
	if (pList) pList->Add(this);
}


BOOL CPolygon3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    for (int i=0;i<m_pPointSet.GetSize();i++)
    {
		CObject3D *pObj = m_pPointSet.GetAt(i);
		if (!pObj) continue;

		if (pObj == pOld)
		{
			SetInGraph(FALSE);
			m_pPointSet.RemoveAt(i);
			m_pPointSet.InsertAt(i,pNew);
			if (bUpGraph) SetInGraph(TRUE);
			CPoint3D *pA = (CPoint3D *)m_pPointSet.GetAt(0);
			CPoint3D *pB = (CPoint3D *)m_pPointSet.GetAt(1);
			CPoint3D *pC = (CPoint3D *)m_pPointSet.GetAt(2);

			P1 = pA;
			P2 = pB;
			P3 = pC;
			return TRUE;
		}
    }
    return FALSE;
}

BOOL CPolygon3D::IsPointInside(CVector4& pt,BOOL bLim/*=TRUE*/)
{
    BOOL bInside = TRUE;

    int nb = m_pPointSet.GetSize();
    if (nb< 3) return FALSE;

    int nSide = -1;

    for (int i=0;i<nb;i++)
    {
        CPoint3D *pObj1 = (CPoint3D *)m_pPointSet.GetAt(i);
        CPoint3D *pObj2 = NULL;
        if (i==nb-1)
            pObj2 = (CPoint3D *)m_pPointSet.GetAt(0);
        else
            pObj2 = (CPoint3D *)m_pPointSet.GetAt(i+1);

        CVector4 p1 = pObj1->Concept_pt - plRep.O;
        CVector4 p2 = pObj2->Concept_pt - plRep.O;
        CVector4 px = pt - plRep.O;

        CVector4 ap1(plRep.I * p1, plRep.J *p1, plRep.K * p1);
        CVector4 ap2(plRep.I * p2, plRep.J * p2, plRep.K * p2);
        CVector4 apx(plRep.I * px, plRep.J * px, plRep.K * px);

        FCoord dd = (apx.y-ap1.y)*(ap2.x-ap1.x) - (apx.x-ap1.x)*(ap2.y-ap1.y);

        nSide = (dd < 0) ? 0 : 1;

        if (m_bConvex==3 && nSide)
        {
            int n = (dd < 0) ? 0 : 1;
            bInside = FALSE;
            nSide = i;
                break;

        }
        else if (m_bConvex==4 && nSide)
        {
            int n = (dd < 0) ? 0 : 1;
            bInside = FALSE;
            nSide = i;
                break;
        }

    }

    if (!bInside && !bLim && nSide!=-1)
    {
        CPoint3D *pObj1 = (CPoint3D *)m_pPointSet.GetAt(nSide);
        CPoint3D *pObj2 = NULL;
        if (nSide==nb-1)
            pObj2 = (CPoint3D *)m_pPointSet.GetAt(0);
        else
            pObj2 = (CPoint3D *)m_pPointSet.GetAt(nSide+1);

        CPoint3D pt1(pObj1->Concept_pt);
        CPoint3D pt2(pObj2->Concept_pt);
        CPoint3D pt3(pt);
        CPoint3D pt4(m_vCentroid);
        CSegment3D pSeg(&pt1,&pt2);
        CDemiDroite3D pDem(&pt4,&pt3);
        pSeg.CalculConceptuel();
        pDem.CalculConceptuel();
        CPointInterDD3D pPt(&pSeg,&pDem);
        UINT h = pPt.CalculConceptuel();
        if (!h)
            pt = pPt.Concept_pt;
    }
    return bInside;
}


// Given a directed line pq, determine
// whether qr turns CW or CCW.
int CPolygon3D::WhichSide(CVector4 p,CVector4 q,CVector4 r)
{
    FCoord result;
    result = (p.x - q.x) * (q.y - r.y) - (p.y - q.y) * (q.x - r.x);
    if (result < 0) return -1;      // q lies to the left  (qr turns CW).
    if (result > 0) return  1;      // q lies to the right (qr turns CCW).
    return 0;                       // q lies on the line from p to r.
}

// Lexicographic comparison of p and q
int CPolygon3D::Compare(CVector4 p,CVector4 q)
{
    if (p.x < q.x) return -1;   // p is less than q.
    if (p.x > q.x) return  1;   // p is greater than q.
    if (p.y < q.y) return -1;   // p is less than q.
    if (p.y > q.y) return  1;   // p is greater than q.
    return 0;                   // p is equal to q.
}

int CPolygon3D::CheckConvex()
{
    int curDir, thisDir, thisSign, angleSign = 0, dirChanges = 0;
    int  first=0, second=1, third=2, saveFirst, saveSecond;

    int nb = m_pPointSet.GetSize();

    if (nb < 3) return gConvexDegenerate;

    saveFirst = first;
    saveSecond = second;

    CPoint3D *pObj1 = (CPoint3D *)m_pPointSet.GetAt(first);
    CPoint3D *pObj2 = (CPoint3D *)m_pPointSet.GetAt(second);


    curDir = Compare(pObj1->Concept_pt, pObj2->Concept_pt);
    while (third!=nb)
    {
        CPoint3D *pObj1 = (CPoint3D *)m_pPointSet.GetAt(first);
        CPoint3D *pObj2 = (CPoint3D *)m_pPointSet.GetAt(second);
        CPoint3D *pObj3 = (CPoint3D *)m_pPointSet.GetAt(third);
        if ((thisDir = Compare(pObj2->Concept_pt, pObj3->Concept_pt)) == -curDir )
            ++dirChanges;
        curDir = thisDir;
        if ( thisSign = WhichSide(pObj1->Concept_pt, pObj2->Concept_pt, pObj3->Concept_pt) )
        {
            if ( angleSign == -thisSign )
                return gNotConvex;
            angleSign = thisSign;
        }
        first = second; second = third;
        third++;
    }

    /* Must check that end of list continues back to start properly */
    pObj1 = (CPoint3D *)m_pPointSet.GetAt(second);
    pObj2 = (CPoint3D *)m_pPointSet.GetAt(saveFirst);

    if ( Compare(pObj1->Concept_pt, pObj2->Concept_pt) )
    {
        third = saveFirst;
        CPoint3D *pObj1 = (CPoint3D *)m_pPointSet.GetAt(first);
        CPoint3D *pObj2 = (CPoint3D *)m_pPointSet.GetAt(second);
        CPoint3D *pObj3 = (CPoint3D *)m_pPointSet.GetAt(third);
        if ((thisDir = Compare(pObj2->Concept_pt, pObj3->Concept_pt)) == -curDir )
            ++dirChanges;
        curDir = thisDir;
        if ( thisSign = WhichSide(pObj1->Concept_pt, pObj2->Concept_pt, pObj3->Concept_pt) )
        {
            if ( angleSign == -thisSign )
                return gNotConvex;
            angleSign = thisSign;
        }
        first = second; second = third;
    }
    third = saveSecond;
    pObj1 = (CPoint3D *)m_pPointSet.GetAt(first);
    pObj2 = (CPoint3D *)m_pPointSet.GetAt(second);
    CPoint3D *pObj3 = (CPoint3D *)m_pPointSet.GetAt(third);
    if ((thisDir = Compare(pObj2->Concept_pt, pObj3->Concept_pt)) == -curDir )
        ++dirChanges;
    curDir = thisDir;
    if ( thisSign = WhichSide(pObj1->Concept_pt, pObj2->Concept_pt, pObj3->Concept_pt) )
    {
        if ( angleSign == -thisSign )
            return gNotConvex;
        angleSign = thisSign;
    }

    if ( dirChanges > 2 ) return angleSign ? gNotConvex : gNotConvexDegenerate;
    if ( angleSign  > 0 ) return gConvexCCW;
    if ( angleSign  < 0 ) return gConvexCW;
    return gConvexDegenerate;

}

int CPolygon3D::CalculCentroid()
{
    int nb = m_pPointSet.GetSize();

    FCoord ai, atmp = 0., xtmp = 0., ytmp = 0.;
    if (nb < 3) return 1;

    int i,j;

    for (i = nb-1, j = 0; j < nb; i = j, j++)
    {
        CPoint3D *pObj1 = (CPoint3D *)m_pPointSet.GetAt(i);
        CPoint3D *pObj2 = (CPoint3D *)m_pPointSet.GetAt(j);

        CVector4 U = pObj1->Concept_pt - plRep.O;
        CVector4 V = pObj2->Concept_pt - plRep.O;

        CVector4 ap1(plRep.I * U, plRep.J * U, plRep.K * U);
        CVector4 ap2(plRep.I * V, plRep.J * V, plRep.K * V);


        ai = ap1.x * ap2.y - ap2.x * ap1.y;
        atmp += ai;
        xtmp += (ap2.x + ap1.x) * ai;
        ytmp += (ap2.y + ap1.y) * ai;
    }

    m_nArea = fabs(atmp / 2);
    if (atmp == 0.)
    {
        return 2;
    }

    CVector4 U(xtmp / (3 * atmp),ytmp / (3 * atmp),0);

    m_vCentroid.x = plRep.I.x*U.x + plRep.J.x*U.y + plRep.K.x*U.z + plRep.O.x,
    m_vCentroid.y = plRep.I.y*U.x + plRep.J.y*U.y + plRep.K.y*U.z + plRep.O.y,
    m_vCentroid.z = plRep.I.z*U.x + plRep.J.z*U.y + plRep.K.z*U.z + plRep.O.z,
    m_vCentroid.w = 1;


    return 0;
}

UINT  CPolygon3D::CalculConceptuel()
{
    int nb = m_pPointSet.GetSize();
    if (nb <3)
    {
        bValidate = FALSE;
        return ERR_POLYGON_3POINT;
    }

    bValidate = TRUE;
    for (int i=0;i<m_pPointSet.GetSize();i++)
    {
        bValidate = bValidate && m_pPointSet.GetAt(i)->bValidate;
    }
    if (!bValidate) return ERR_NOPOLYGON;

    CPoint3D *pA = (CPoint3D *)m_pPointSet.GetAt(0);
    CPoint3D *pB = (CPoint3D *)m_pPointSet.GetAt(1);
    CPoint3D *pC = (CPoint3D *)m_pPointSet.GetAt(2);

    P1 = pA;
    P2 = pB;
    P3 = pC;

    CVector4    U = pB->Concept_pt - pA->Concept_pt,
                V = pC->Concept_pt - pA->Concept_pt;

    VecNorm = U % V;
    plRep.I = U.Normalized();
    plRep.K = VecNorm.Normalized();
    plRep.J = (U % VecNorm).Normalized();
    plRep.O = pA->Concept_pt;

    CPlan3D pl(pA,pB,pC);
    pl.CalculConceptuel();
    CVector4 vN = pl.GetNormalForm();

    for (i=3;i<m_pPointSet.GetSize();i++)
    {
        CPoint3D*   pT = (CPoint3D *)m_pPointSet.GetAt(i);
        CVector4    U1 = pT->Concept_pt - pA->Concept_pt;
        CVector4    U2 = pT->Concept_pt;

        FCoord dd = vN.x * U2.x + vN.y * U2.y + vN.z * U2.z + vN.w;


        CVector4 UvU1 = U % U1;
        CVector4 VvU1 = V % U1;
        UvU1 = UvU1.Normalized();
        VvU1 = VvU1.Normalized();

        CVector4 UvW2 = plRep.K % UvU1;
        CVector4 UvV2 = plRep.K % VvU1;

        //if (!UvW2.NullVector() || !UvV2.NullVector())
        if (!FCZero(dd))
        {
            bValidate = FALSE;
            return ERR_POLYGON_NOPLANE2;
        }
    }


    if (VecNorm.NullVector())
    {
        bValidate = FALSE;
        return ERR_PTSALIGN;
    }

    int nConv = CheckConvex();
    if (nConv<gConvexDegenerate)
    {
        bValidate = FALSE;
        return ERR_POLYGON_NOCONVEX;
    }

    m_bConvex = nConv;

    if (CalculCentroid())
    {
        bValidate = FALSE;
        return ERR_PTSALIGN;
    }

    {
        CVector4    U = plRep.I,
                    V = plRep.J;
        FCoord normK = 1 / VecNorm.Norme();
        CVector4    K = VecNorm * normK;
        CVector4    J(K.y,-K.x,0,1);
        normK = J.Norme();
        if (FCZero(normK))
            { J.x = J.z =0; J.y = 1;    }
        else
            J = J * (-1/normK);
        CVector4    I = J % K;

        ptonRep.I = I;
        ptonRep.J = J;
        ptonRep.K = K;
        ptonRep.O = pA->Concept_pt;
    }

    p1 = P1->Concept_pt;
    p2 = P2->Concept_pt;
    p3 = P3->Concept_pt;
    p4 = P3->Concept_pt;

    return 0;
}

CString CPolygon3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???")),sObjName;
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());
    sObjName.Empty();

    for (int i=0;i<m_pPointSet.GetSize();i++)
    {
        CPoint3D*   pT = (CPoint3D *)m_pPointSet.GetAt(i);
        if (i)
            sObjName += _T(", ");
        sObjName += pT->GetObjectName();
    }


    //CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    //if (P1) sn1 = P1->GetObjectHelp();
    //if (D) sn2 = D->GetObjectHelp();
    mstr.Format(sFormat,sName,sObjName);
    return mstr;

}


void CPolygon3D::CalculVisuel(CVisualParam *pVisParam)
{
    p1 = m_vCentroid;
    p3 = m_vCentroid;


    CVector4 oeil= pVisParam->GetEyePos();

//  oeil = oeil*-150.;
    CVector4 origin(0,0,0,1);
    CVector4 VisuNorm= oeil - origin;

    CPoint3D pt1(origin);
    CPoint3D pt2(oeil);
    CDroite3D dr(&pt1,&pt2);
    dr.CalculConceptuel();

    CPlanPerp3D pperp(&pt2,&dr);
    pperp.CalculConceptuel();
    CVector4 nf = pperp.GetNormalForm();

    CVector4 cVe = m_vCentroid;

    CVector4    FaceNorm= VecNorm;
    FCoord n1 = FaceNorm.Norme();
    FaceNorm = FaceNorm * (1/n1);

    FCoord n2 = VisuNorm.Norme();
    VisuNorm = VisuNorm*(1/n2);
    FCoord dot = VisuNorm * FaceNorm;

    m_nZFact=(nf.x*cVe.x + nf.y*cVe.y + nf.z*cVe.z + nf.w) /
                sqrt(cVe.x*cVe.x+cVe.y*cVe.y+cVe.z*cVe.z);

    if (dot>0.)
        m_nZFact = fabs(m_nZFact);
    else
        m_nZFact = -fabs(m_nZFact);

}

void CPolygon3D::DrawRetro(CDC* pDC,CVisualParam *mV)
{
    // dessin du vecteur normal
    CPlan3D::DrawRetro(pDC,mV);

    CPoint pt = (CPoint)mV->ProjectPoint(m_vCentroid);
    CRect mrect(pt,pt);
    mrect.InflateRect(2,2);
    pDC->Rectangle(mrect);
}

CString CPolygon3D::ExportSymbolic(int nFormat)
{
	return CObject3D::ExportSymbolic(nFormat);
}


void CPolygon3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;
	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {255/255., 255/255.,100/255. , 1.0f};
    float mat_diffuse[] = {0.9f, 0.1f, 0.1f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	CVector4 vec=VecNorm;
	vec = vec.Normalized();

	int nb = m_pPointSet.GetSize();
	glPushMatrix();
	if (nb<=3)
		glBegin(GL_TRIANGLES);
	else 
		glBegin(GL_QUADS);
	glNormal3d(vec.x, vec.y, vec.z);   //N1
	COLORREF clr = pObjectShape.GetObjectColor();
	glColor3f(GetRValue(clr)/255.,GetGValue(clr)/255.,GetBValue(clr)/255.);

	for (int i=0;i<nb;i++)
    {
        CPoint3D *pObj = (CPoint3D *)m_pPointSet.GetAt(i);
		if (!pObj) continue;
		CVector4 pt = pObj->Concept_pt;
		GLdouble vertex[]=
		{
			(pt.x/TPref::TUniv.nUnitRep/3),
			(pt.y/TPref::TUniv.nUnitRep/3),
			(pt.z/TPref::TUniv.nUnitRep/3),
		};
		glVertex3d( vertex[0], vertex[1], vertex[2]);   //V2
	}
	glEnd();
	glPopMatrix();
}

void CPolygon3D::GetRange(CVector4 &min,CVector4 &max)
{
    CObject3D::GetRange(min,max);
}




void CPolygon3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;


    int nb = m_pPointSet.GetSize();
    if (/*!bSm && */pObjectShape.nShapeId!=0)
    {
        CPoint pt = (CPoint)mV->ProjectPoint(m_vCentroid);

        CPoint mpoly[20];
        for (int i=0;i<nb;i++)
        {
            CPoint3D* temp = (CPoint3D*)m_pPointSet.GetAt(i);
            CPoint pt  = temp->Visual_pt;
            mpoly[i] = pt;
        }

        CVector4    FaceNorm= VecNorm;
        FCoord n1 = FaceNorm.Norme();
        FaceNorm = FaceNorm * (1/n1);

        CVector4 oeil= mV->GetEyePos();
        CVector4 origin(0,0,0,1);
        CVector4 VisuNorm= oeil -
            ((mV->bParProj) ? origin : m_vCentroid);
        FCoord n2 = VisuNorm.Norme();
        VisuNorm = VisuNorm*(1/n2);
        FCoord dot = VisuNorm * FaceNorm;

        CBrush pBrush;
        CPen mPen(PS_SOLID   ,1,(dot > 0) ? pObjectShape.GetObjectColor():
                                            pObjectShape.GetObjectHiddenColor());
        //CPen mPen(PS_NULL,0,RGB(192,192,192));
        //pBrush.CreateHatchBrush(HS_CROSS,RGB(192,192,192));
        pBrush.CreateSolidBrush((dot > 0) ? pObjectShape.GetObjectColor():
                                            pObjectShape.GetObjectHiddenColor());

        CPen mPenLine;
        mPenLine.CreatePenIndirect(&pObjectShape.GetPenStyle(pObjectShape.nShapeId-2));

        int oldM = pDC->SetBkMode(TRANSPARENT);

        CBrush *pOld = pDC->SelectObject(&pBrush);
        CPen *pOldp = NULL;

        if (!bSm && pObjectShape.nShapeId==1)
        {
            pOldp = pDC->SelectObject(&mPen);
            pDC->Polygon(mpoly,nb);
        }
        else
        {
            mpoly[nb] = mpoly[0];
			int nshape = pObjectShape.nShapeId-2;
			if (bSm && pObjectShape.nShapeId==1)
				nshape = 4;

            for (int i=0;i<nb;i++)
            {
                CPoint3D pt1 = *((CPoint3D*)m_pPointSet.GetAt(i));
                CPoint3D pt2= *((CPoint3D*)m_pPointSet.GetAt((i==nb-1)? 0 : i+1));
                CSegment3D seg(&pt1,&pt2);
                seg.pObjectShape.nShapeId = nshape;
                seg.pObjectShape.clrObject = pObjectShape.clrObject;
                seg.nCalque = nCalque;
                seg.CalculConceptuel();
                seg.CalculVisuel(mV);
                seg.Draw(pDC,mV);
            }
            pOldp = pDC->SelectObject(&mPenLine);
            //pDC->Polyline(mpoly,nb+1);
        }
        pDC->SelectObject(pOldp);
        pDC->SelectObject(pOld);
        pDC->SetBkMode(oldM);
    }

    if (bMarked && mV->bDrawMark)
    {
        DrawRetro(pDC,mV);
    }
    if (bIsSelected)
    {
        for (int i=0;i<nb;i++)
        {
            CPoint3D* temp = (CPoint3D*)m_pPointSet.GetAt(i);
            CPoint pt  = temp->Visual_pt;

            int Ds = TPref::TPoint.nSize;
            CRect theR(pt.x-Ds,pt.y-Ds,pt.x+Ds,pt.y+Ds);
            pDC->InvertRect(theR);
        }
    }

}

BOOL CPolygon3D::IsInActiveArea(CPoint thePt)
{
    BOOL bIsInSeg = FALSE;
    int nb = m_pPointSet.GetSize();
    for (int t=0;(t<nb) && (!bIsInSeg);t++)
     {
        CPoint3D* pA = (CPoint3D*)m_pPointSet.GetAt(t);
        CPoint3D* pB = NULL;

        if (t==(nb-1))
            pB = (CPoint3D*)m_pPointSet.GetAt(0);
        else
            pB = (CPoint3D*)m_pPointSet.GetAt(t+1);

        CRgn *pRgn = DoSegRgn(pA->Visual_pt,pB->Visual_pt);
        bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
        delete  pRgn;
     }
    return bIsInSeg;
}
