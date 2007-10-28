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
/// @file Droite3D.cpp
/// @brief implementation of the CDroite3D class.
/// $Date: $
/// $Revision: $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"
#include "..\OGLTools\OGLT.h"
#include "..\OGLTools\glut.h"

#include "Vector4.h"
#include "Point3D.h"

#include "Droite3D.h"
#include "Plan3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MRG_ZERO 1.0e-8


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDroite3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CDroite3D::CDroite3D() : CObject3D()
{
    LP1 = LP2 = LP3 = NULL;
    CLP1 = CLP2 = NULL;
    LPt1 = LPt2 = NULL;
    P1 = P2 = NULL;
    D2 = NULL;
    pObjectShape.clrObject = RGB(0,0,255);
    VisualPts.RemoveAll();
}

CDroite3D::~CDroite3D()
{
    if (LP1)    delete LP1;
    if (LP2)    delete LP2;
    if (LP3)    delete LP3;
    if (CLP1)   delete CLP1;
    if (CLP2)   delete CLP2;
    if (LPt1)   delete LPt1;
    if (LPt2)   delete LPt2;
    VisualPts.RemoveAll();
}

CDroite3D::CDroite3D(CPoint3D *s1,CPoint3D *s2): CObject3D()
{
    P1 = s1;
    P2 = s2;
    D2 = 0;
    LP1=LP2=LP3=0;
    CLP1=CLP2=0;
    LPt1=LPt2=0;
    //VisualPts = 0;
    if (s1 && s2)
        nDepth = __max(s1->nDepth,s2->nDepth)+1;
    pObjectShape.clrObject = RGB(0,0,255);
    bIsSegment = FALSE;
    VisualPts.RemoveAll();
}

CDroite3D::CDroite3D(CPoint3D *s1,CDroite3D *s2): CObject3D()
{
    P1 = s1;
    P2 = 0;
    D2 = s2;

    LP1=LP2=LP3=0;
    CLP1=CLP2=0;
    LPt1=LPt2=0;
    //VisualPts = 0;
    nDepth = __max(s1->nDepth,s2->nDepth)+1;
    pObjectShape.clrObject = RGB(0,0,255);
    bIsSegment = FALSE;
    VisualPts.RemoveAll();
}

int CDroite3D::SetDepth()
{
    if (P1 && P2)
        nDepth = __max(P1->nDepth,P2->nDepth)+1;
    else if (P1 && D2)
        nDepth = __max(P1->nDepth,D2->nDepth)+1;
    return nDepth;
}


CDroite3D::CDroite3D(const CObject3D &src): CObject3D(src)
{
    bIsSegment = ((CDroite3D&)src).bIsSegment;
    P1 =  ((CDroite3D&)src).P1;
    P2 =   ((CDroite3D&)src).P2;
    D2 =   ((CDroite3D&)src).D2;
    //VisualPts = 0;
    LP1=LP2=LP3=0;
    CLP1=CLP2=0;
    LPt1=LPt2=0;
    VisualPts.RemoveAll();
}

BOOL CDroite3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
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
    else return FALSE;
    return TRUE;
}


void CDroite3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << bIsSegment;
        ar << CDirVector;
        ar << ((P1) ? P1->nObjectId : -1);
        ar << ((P2) ? P2->nObjectId : -1);
        ar << ((D2) ? D2->nObjectId : -1);
    }
    else
    {
        ar >> bIsSegment;
        ar >> CDirVector;
        LP1=LP2=LP3=0;
        CLP1=CLP2=0;
        LPt1=LPt2=0;

        P1 = (CPoint3D*)SerializeObj(ar);
        P2 = (CPoint3D*)SerializeObj(ar);
        D2 = (CDroite3D*)SerializeObj(ar);
    }
}


CObject3D* CDroite3D::CopyObject()
{
    CObject3D *temp = new CDroite3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CDroite3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(P1);
    if (!D2)
        list->Add(P2);
    else
        list->Add(D2);
    return list;
}

CString CDroite3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (P1) sn1 = P1->GetObjectHelp();
    if (P2)
        sn2 = P2->GetObjectHelp();
    else if (D2)
        sn2 = D2->GetObjectHelp();

    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}


BOOL CDroite3D::IsEqual(CObject3D &o)
{
    if (!o.MaskObject(TAllDroiteClass)) return false;

    DWORD ThisIsSeg = (isA() == TAllSegmentClass);
    DWORD OtherIsSeg = (o.isA() == TAllSegmentClass);

    if (OtherIsSeg && ThisIsSeg)
     {
        CVector4 v1 = ((CSegment3D &)o).P1->Concept_pt - P1->Concept_pt;
        CVector4 v2 = ((CSegment3D &)o).P2->Concept_pt - P2->Concept_pt;
        CVector4 v3 = ((CSegment3D &)o).P2->Concept_pt - P1->Concept_pt;
        CVector4 v4 = ((CSegment3D &)o).P1->Concept_pt - P2->Concept_pt;
        return ((v1.NullVector() && v2.NullVector()) ||
                    (v3.NullVector() && v4.NullVector()));
     }
    else
    if (!OtherIsSeg && !ThisIsSeg)
     {
        CVector4 dir = GetDirVector() % ((CDroite3D &)o).GetDirVector();
        if (dir.NullVector())
         {
            CPoint3D p1(CVector4(0,0,0,1));
            CPoint3D p2(CVector4(100,0,0,1));
            CPoint3D p3(CVector4(0,100,0,1));
            CPoint3D p4(CVector4(0,0,100,1));
            CPoint3D p2p(CVector4(100,0,100,1));
            CPoint3D p3p(CVector4(0,100,100,1));
            CPlan3D pl(&p1,&p2,&p3);
            pl.CalculConceptuel();

            CPointInterDP3D a1(this,&pl);
            UINT res = a1.CalculConceptuel();
            if (res)
             {
                //TPlan3D pl2(0,&p1,&p2,&p4);
                CPlan3D pl2(&p1,&p2p,&p3p);
                pl2.CalculConceptuel();

                CPointInterDP3D a1b(this,&pl2);
                res = a1b.CalculConceptuel();
                CPointInterDP3D a2((CDroite3D *)&o,&pl2);
                UINT res2 = a2.CalculConceptuel();
                return a1b.IsEqual(a2);
             }
            else
             {
                CPointInterDP3D a2((CDroite3D *)&o,&pl);
                a2.CalculConceptuel();
                return a1.IsEqual(a2);
             }
         }
        else return false;
     }
    else
        return false;
}

BOOL CDroite3D::IsInLimit(FCoord x)
{
    return true;
}

const int WndHeight=1000;
const int WndWidth=1000;

BOOL CDroite3D::IsInActiveArea(CPoint thePt)
{
    if (VP1.x == VP2.x)
    {
        CRect theRect(VP1.x-TPref::TUniv.nDefPres,
                (!bIsSegment) ? -1 : __min(VP1.y,VP2.y),
                VP1.x+TPref::TUniv.nDefPres,
                (!bIsSegment) ? WndHeight+1 : __max(VP1.y,VP2.y));
        return(theRect.PtInRect(thePt));
    }

    if (VP1.y == VP2.y)
    {
        CRect theRect((!bIsSegment) ? -1 : __min(VP1.x,VP2.x),
                VP1.y-TPref::TUniv.nDefPres,
                (!bIsSegment) ? WndWidth+1 : __max(VP1.x,VP2.x),
                VP2.y+TPref::TUniv.nDefPres);
        return(theRect.PtInRect(thePt));
    }

    FCoord ypos = (FCoord)(VP1.y-VP2.y) / (FCoord)(VP1.x-VP2.x);
    ypos = ypos*(thePt.x-VP1.x) + VP1.y;

    FCoord xpos = (FCoord)(VP1.x-VP2.x) / (FCoord)(VP1.y-VP2.y);
    xpos = xpos*(thePt.y-VP1.y) + VP1.x;

    CRect theRect(  (int)(thePt.x-TPref::TUniv.nDefPres),
                    (int)(ypos-TPref::TUniv.nDefPres),
                    (int)(thePt.x+TPref::TUniv.nDefPres),
                    (int)(ypos+TPref::TUniv.nDefPres));
    CRect theRect2( (int)(xpos-TPref::TUniv.nDefPres),
                    (int)(thePt.y-TPref::TUniv.nDefPres),
                    (int)(xpos+TPref::TUniv.nDefPres),
                    (int)(thePt.y+TPref::TUniv.nDefPres));

    BOOL test = theRect.PtInRect(thePt) || theRect2.PtInRect(thePt);
    if ((test) && (bIsSegment))
        test = ( (thePt.x >= __min(VP1.x,VP2.x))
                && (thePt.x <= __max(VP1.x,VP2.x))
                && (thePt.y >= __min(VP1.y,VP2.y))
                && (thePt.y <= __max(VP1.y,VP2.y)));
    return test;
}

UINT CDroite3D::IsParallelTo(CObject3D *pObj)
{
    UINT res = VER_ERROR;

    if (!pObj) return res;
    if (!bValidate || !pObj->bValidate) return res;

    CDroite3D *pDr = DYNAMIC_DOWNCAST(CDroite3D,pObj);
    CPlan3D *pPl = DYNAMIC_DOWNCAST(CPlan3D,pObj);

    CVector4 pV1 = GetDirVector();
    if (pDr)
    {
        CVector4 pV2 = pDr->GetDirVector();
        CVector4 pVC = pV1 % pV2;
        return (pVC.NullVector()) ? VER_PAR_ANALYTIC : VER_PAR_NO ;
    }
    if (pPl)
    {
        CVector4 pV2 = pPl->VecNorm ;
        FCoord DotProd = pV1 * pV2;
        return (FCZero(DotProd)) ? VER_PAR_ANALYTIC : VER_PAR_NO ;
    }
    return VER_ERROR;
}


UINT CDroite3D::IsPerpendicularTo(CObject3D *pObj)
{
    UINT res = VER_ERROR;

    if (!pObj) return res;
    if (!bValidate || !pObj->bValidate) return res;

    CDroite3D *pDr = DYNAMIC_DOWNCAST(CDroite3D,pObj);
    CPlan3D *pPl = DYNAMIC_DOWNCAST(CPlan3D,pObj);

    if (pDr)
    {
        CVector4 pV1 = GetDirVector();
        CVector4 pV2 = pDr->GetDirVector();

        FCoord pVC = pV1 * pV2;
        return (FCZero(pVC)) ? VER_PERP_ANALYTIC : VER_PERP_NO ;
    }
    if (pPl)
    {
        CVector4 pV1 = GetDirVector();
        CVector4 pV2 = pPl->VecNorm;

        CVector4 pProd = pV1 % pV2;

        return (pProd.NullVector()) ? VER_PERP_ANALYTIC : VER_PERP_NO;

        FCoord pVC = pV1 * pV2;
        return (FCZero(pVC)) ? VER_PERP_ANALYTIC : VER_PERP_NO ;
    }
    return VER_ERROR;
}


CRgn* CDroite3D::InvalideRect()
{
    return NULL;
}

enum TCloison {Front,Gauche,Horiz};
bool testin(CVector4* o,CVisualParam *myVisuParam,TCloison cl=Front)
{
    bool t=0;
    switch (cl)
     {
        case Front:
            if (myVisuParam->nVisuKind > CVisualParam::VisuPlane)
                t= ((o->y >= 0.) && (o->z >= 0.0));
            break;
        case Gauche:
            if (myVisuParam->nVisuKind > CVisualParam::VisuPlane)
                t = ((o->x >= 0.0) && (o->z >= 0.0));
            break;
        case Horiz:
            if (myVisuParam->nVisuKind == CVisualParam::VisuPlane)
                t =1;
            else
            if (myVisuParam->nVisuKind > CVisualParam::VisuPlane)
                t = ((o->x >= 0.0) && (o->y >= 0.0));
            break;
        default :
            break;
     }
    return t;
}

/*bool testv(CDVector4 o,CVisualParam *myVisuParamExt)
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


void CDroite3D::AddVisualPt(CVector4SDist mv)
{
    int nb = VisualPts.GetSize();
    BOOL bFound = FALSE;
    for (int i=0;i<nb;i++)
    {
        CVector4SDist mbb = VisualPts.GetAt(i);
        if (mv < mbb)
        {
            bFound = TRUE;
            VisualPts.InsertAt(i,mv);
            return;
        }
    }
    VisualPts.Add(mv);
}


void CDroite3D::CalculVisuel(CVisualParam *pVisParam)
{
    if (LPt1) delete LPt1;
    if (LPt2) delete LPt2;

    CVector4 A = GetBasePoint();
    CVector4 dir = GetDirVector();

    VisualPts.RemoveAll();

    if (CLP1)
     {
        CVector4SDist ttt(*CLP1,0);
        ttt.in = TRUE;
        CVector4 AM = A - *CLP1;
        ttt.dis = dir * AM; //AM - dir;
        AddVisualPt(ttt);
     }
    if (CLP2)
     {
        CVector4SDist ttt(*CLP2,0);
        ttt.in = TRUE;
        CVector4 AM = A - *CLP2;
        ttt.dis = dir * AM;
        AddVisualPt(ttt);
     }
    if (LP1 && testin(LP1,pVisParam,Front))
     {
        CVector4SDist ttt(*LP1,0);
        ttt.in = ((LP1->y >= 0.0) && (LP1->z >= 0.0));
        CVector4 AM = A - *LP1;
        ttt.dis = dir * AM;
        AddVisualPt(ttt);
      }
    if (LP2 && testin(LP2,pVisParam,Gauche))
     {
        CVector4SDist ttt(*LP2,0);
        ttt.in = ((LP2->x >= 0.0) && (LP2->z >= 0.0));
        CVector4 AM = A - *LP2;
        ttt.dis = dir * AM;
        AddVisualPt(ttt);
     }
    if (LP3 && testin(LP3,pVisParam,Horiz))
     {
        CVector4SDist ttt(*LP3,0);
        ttt.in = (pVisParam->nVisuKind == CVisualParam::VisuPlane) ?
                        1 : ((LP3->x >= 0.0) && (LP3->y >= 0.0));
        CVector4 AM = A - *LP3;
        ttt.dis = dir * AM;
        AddVisualPt(ttt);
     }

    if (bIsSegment)
     {
        VP1 = pVisParam->ProjectPoint(P1->Concept_pt);
        VP2 = pVisParam->ProjectPoint(P2->Concept_pt);
     }
    else
     {
        CVector4 dir = GetDirVector();
        CVector4 a = GetBasePoint();
        CVector4 b = a+dir;
        VP1 = pVisParam->ProjectPoint(a);
        VP2 = pVisParam->ProjectPoint(b);
     }
}

UINT  CDroite3D::CalculConceptuel()
{
    if (P2)
        bValidate = ((P2->bValidate) && (P1->bValidate));
    else
        bValidate = ((P1->bValidate) && (D2->bValidate));
    if (!bValidate)
        return ERR_NODROITE;

    CVector4 dir=GetDirVector();
    if (dir.NullVector())
    {
        bValidate = false;
        return  ERR_DRPTSID;
    }

    /*int nb2 = */ClippingDroite();
    return 0;
}

CString CDroite3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && P2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = P2->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("LineD[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
	        mstr.Format(_T("LineD(%s,%s,%s);"),strName,strObj1,strObj2);
    }
    return mstr;
}


void CDroite3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible && !TPref::TUniv.bShowHidden) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

    CBrush curBrush;//(pObjectShape.GetObjectColor());
    CPen curPen,disPen;
//    curPen.CreatePenIndirect(&pObjectShape.GetPenStyle()); //(*ObjectColor,0,PS_SOLID);
  //  disPen.CreatePenIndirect(&pObjectShape.GetHiddenPenStyle(
    //                            pObjectShape.GetObjectHiddenColor()));

	if (!bVisible && TPref::TUniv.bShowHidden)
	{
		curBrush.CreateSolidBrush(TPref::TUniv.clrShowHidden);
	    curPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
		disPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
	}
	else
	{
		//curBrush = CBrush(pObjectShape.GetObjectColor());
		curBrush.CreateSolidBrush(pObjectShape.GetObjectColor());
	    curPen.CreatePenIndirect(&pObjectShape.GetPenStyle()); //(*ObjectColor,0,PS_SOLID);
		disPen.CreatePenIndirect(&pObjectShape.GetHiddenPenStyle(
                                pObjectShape.GetObjectHiddenColor()));
	}
/*  if (small)
     {
        if ((CLP1) && (CLP2))
         {
            HandleDC.SelectObject(curPen);
            HandleDC.MoveTo((TPoint)mV->ProjectPoint(*CLP1));
            HandleDC.LineTo((TPoint)mV->ProjectPoint(*CLP2));
            HandleDC.RestorePen();
         }
        else
         {
            HandleDC.SelectObject(curPen);
            if (P1) HandleDC.MoveTo((TPoint)mV->ProjectPoint(P1->Concept_pt));
            if (P2) HandleDC.LineTo((TPoint)mV->ProjectPoint(P2->Concept_pt));
            HandleDC.RestorePen();
         }

     }
    else {*/
    CBrush *oldB = pDC->SelectObject(&curBrush);
    CPen *oldP = pDC->SelectObject(&curPen);

    if (LP1)
    if (testin(LP1,mV,Front))
     {  //**MyOrder a(LP1);
        CRect mrect(
            (CPoint)mV->ProjectPoint(*LP1) - CSize(1,1),CSize(3,3));
                pDC->Rectangle(mrect);

     }
    if (LP2)
    if (testin(LP2,mV,Gauche))
     {  //**MyOrder a(LP2);
        CRect mrect(
            (CPoint)mV->ProjectPoint(*LP2) - CSize(1,1),CSize(3,3));
                pDC->Rectangle(mrect);
     }
    if (LP3)
    if (testin(LP3,mV,Horiz))
     {  //**MyOrder a(LP3);
        CRect mrect(
            (CPoint)mV->ProjectPoint(*LP3) - CSize(1,1),CSize(3,3));
                pDC->Rectangle(mrect);
     }

    pDC->SelectObject(oldP);

    CVector4 base = GetBasePoint();
    CVector4 dir = GetDirVector();

    CVector4SDist start = VisualPts.GetAt(0);
    int gggg = VisualPts.GetSize();
    for (int gg = 1;gg < gggg;gg++)
    {
		CVector4SDist mm = VisualPts.GetAt(gg);
		//bool insi = !(testv(start,mV) || testv(mm,mV));
		bool insi = mV->IsPointVisible(start) && mV->IsPointVisible(mm);
		oldP = pDC->SelectObject(insi ? &curPen : &disPen);
		pDC->MoveTo((CPoint)mV->ProjectPoint(start));
		pDC->LineTo((CPoint)mV->ProjectPoint(mm));
		pDC->SelectObject(oldP);
		start= mm;
    }
    /*if (VisualPts)
     {
        MyOrder *A = 0;
        MyOrder *B = 0;
        if (P1)
         {
            A = new MyOrder(&(P1->Concept_pt));
            Vector4 AM = MakeVector(P1->Concept_pt,base);
            A->lambda = DotProduct(dir,AM);
            //VisualPts->Add(ttt);
            VisualPts->Add(A);
            A->in = 1;
         }
        if (P2)
         {
            B = new MyOrder(&(P2->Concept_pt));
            Vector4 AM = MakeVector(P2->Concept_pt,base);
            B->lambda = DotProduct(dir,AM);
            VisualPts->Add(B);
            B->in = 1;
         }
        int de = VisualPts->GetItemsInContainer();

        MyOrder* start,*end;
        int bp = 0;
        start = (*VisualPts)[bp];
        HandleDC.RestorePen();
        for (int r=bp+1;r<de;r++)
         {
            end = (*VisualPts)[r];
            bool insi = !(testv(start,mV) || testv(end,mV));
            HandleDC.SelectObject(insi ? curPen : disPen);
            HandleDC.MoveTo((TPoint)mV->ProjectPoint(*(start->v)));
            HandleDC.LineTo((TPoint)mV->ProjectPoint(*(end->v)));
            HandleDC.RestorePen();
            start= end;
         }
     }*/

    pDC->SelectObject(&curPen);
    /*if (CLP1 && CLP2)
    {
        CPoint ptCLP1 = (CPoint)mV->ProjectPoint(*CLP1);
        CPoint ptCLP2 = (CPoint)mV->ProjectPoint(*CLP2);
        pDC->MoveTo(ptCLP1);
        pDC->LineTo(ptCLP2);
    }
*/
    if (bIsSelected)
     {
        CPoint A,B;
        A = (CPoint)mV->ProjectPoint((CLP1)? *CLP1 : P1->Concept_pt);
        B = (CPoint)mV->ProjectPoint((CLP2)? *CLP2 : P2->Concept_pt);

        CPoint M1((A.x+B.x)/2 ,(A.y+B.y)/2);
        CPoint M2((A.x+M1.x)/2 ,(A.y+M1.y)/2);
        CPoint M3((M1.x+B.x)/2 ,(M1.y+B.y)/2);
        int         Ds = TPref::TPoint.nSize;
        CRect theR1(M1.x-Ds,M1.y-Ds,M1.x+Ds,M1.y+Ds);
        CRect theR2(M2.x-Ds,M2.y-Ds,M2.x+Ds,M2.y+Ds);
        CRect theR3(M3.x-Ds,M3.y-Ds,M3.x+Ds,M3.y+Ds);
        pDC->InvertRect(theR1);
        pDC->InvertRect(theR2);
        pDC->InvertRect(theR3);
     }
    pDC->SelectObject(oldB);
    pDC->SelectObject(oldP);


/********
    int XX = MYm_cConcept_pts.GetSize();
    for (int g = 0;g<XX;g++)
    {
        CVector4 pt= MYm_cConcept_pts.GetAt(g);
        CPoint tt = (CPoint)mV->ProjectPoint(pt);
        CRect mrect(tt,tt);
        mrect.InflateRect(7,7);
        pDC->Rectangle(mrect);
    }*/

}

void CDroite3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;
	CVector4 base = GetBasePoint();
	GLdouble bx = (base.x/TPref::TUniv.nUnitRep/3);
	GLdouble by = (base.y/TPref::TUniv.nUnitRep/3);
	GLdouble bz = (base.z/TPref::TUniv.nUnitRep/3);

	CVector4 dl = GetDirVector().Normalized();
	CVector4 dz(0,0,1);
	CVector4 drot = dz % dl;
	double dd = 0;
		drot = drot.Normalized();
		drot.Norme();
	if (drot.N!=0)
	{
		FCoord cosangle = dz * dl;
		dd = acos(cosangle);
		dd = RTD(dd);
	}
	else
	{
		dd = (dl.z>0) ? 0 : 180;
		drot = CVector4(0,1,0);
	}

	GLdouble	size=20;
	if (isA()==TSegment3DClass)
	{
		CVector4 base = GetDirVector();
		FCoord N = base.Norme();
		size = (N/TPref::TUniv.nUnitRep/3);
	}
	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
    float mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	GLUquadricObj*m_quadrObj=gluNewQuadric();
// 	gluQuadricNormals(m_quadrObj,GLU_SMOOTH);
// 	gluQuadricTexture(m_quadrObj,GL_TRUE);
// 	gluQuadricDrawStyle(m_quadrObj,GLU_FILL);
// 	gluQuadricOrientation(m_quadrObj,GLU_OUTSIDE);
	glPushMatrix();
	glTranslated(bx, by, bz);
	glRotated(dd,drot.x,drot.y,drot.z);
//	glColor3f(.0f,.0f,1.0f);
	COLORREF clr = pObjectShape.GetObjectColor();
	glColor3f(GetRValue(clr)/255.f,GetGValue(clr)/255.f,GetBValue(clr)/255.f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	if (isA()==TDemiDroite3DClass || isA()==TSegment3DClass)
	{
		gluCylinder(m_quadrObj,0.02,0.02,size,16,16);
	}
	else
	{
		gluCylinder(m_quadrObj,0.02,0.02,size,16,16);
		glRotated(180,drot.x,drot.y,drot.z);
		gluCylinder(m_quadrObj,0.02,0.02,size,16,16);
	}
	//glutSolidTorus (0.02,1.00,16,64);
	glPopMatrix();
}

COLORREF CDroite3D::GetDefaultColor()
{
    return TPref::TLine.clrDefault;
}



CVector4  CDroite3D::GetDirVector()
{
    if (P2 == 0) // droite parallelle
        CDirVector = D2->GetDirVector();
    else
        CDirVector = P2->Concept_pt - P1->Concept_pt;
    return CDirVector;
}


CVector4  CDroite3D::GetBasePoint()
{
    return P1->Concept_pt;
}

static int Clipped (FCoord p, FCoord q, FCoord &u0, FCoord &u1)
{
    FCoord  r;
    if ( p < 0 )
     {  r = q/p;
        if ( r > u1 ) return 0;
        if ( r > u0 ) u0 = r;
        return 1;
     }
    if ( p > 0 )
     { r = q/p;
        if ( r < u0 ) return 0;
        if ( r < u1 ) u1 = r;
        return 1;
     }
    // p == 0
    return q >= 0;
}


int Clip3D (CVector4 box[2], CVector4 line[2])
{
    FCoord u0 = 0, u1 = 1;
    FCoord dx = line[1].x-line[0].x;
    if ( !Clipped(-dx,line[0].x-box[0].x,u0,u1)
     ||   !Clipped(+dx,box[1].x-line[0].x,u0,u1) )
     {
          return 0;
     }

    FCoord dy = line[1].y-line[0].y;
     if ( !Clipped(-dy,line[0].y-box[0].y,u0,u1)
     ||   !Clipped(+dy,box[1].y-line[0].y,u0,u1) )
     {
          return 0;
     }

    FCoord dz = line[1].z-line[0].z;
     if ( !Clipped(-dz,line[0].z-box[0].z,u0,u1)
     ||   !Clipped(+dz,box[1].z-line[0].z,u0,u1) )
     {
          return 0;
     }

     if ( u1 < 1 )
     {
          line[1].x = line[0].x+u1*dx;
          line[1].y = line[0].y+u1*dy;
          line[1].z = line[0].z+u1*dz;
     }

     if ( u0 > 0 )
     {
          line[0].x = line[0].x+u0*dx;
          line[0].y = line[0].y+u0*dy;
          line[0].z = line[0].z+u0*dz;
     }
     return 1;
}


int CDroite3D::ClippingDroite()
{
    static  FCoord lim = 5000.;
    static  CVector4 limt[8]= {
          CVector4(-lim,-lim,-lim),
          CVector4(lim,-lim,-lim),
          CVector4(lim,lim,-lim),
          CVector4(-lim,lim,-lim),
          CVector4(-lim,-lim,lim),
          CVector4(lim,-lim,lim),
          CVector4(lim,lim,lim),
          CVector4(-lim,lim,lim),
     };

    if (LP1)    delete LP1;
    if (LP2)    delete LP2;
    if (LP3)    delete LP3;
    if (CLP1)   delete CLP1;
    if (CLP2)   delete CLP2;
    LP1=LP2=LP3=0;
    CLP1=CLP2=0;

    int nb = TPref::TUniv.nRepPas*TPref::TUniv.nUnitRep;
    CVector4 t[7] = {
        CVector4(0,0,0),        // origin
        CVector4(0,nb,0),       // y
        CVector4(0,0,nb/2),     // z
        CVector4(nb,0,0),       // x
        CVector4(0,nb,nb/2),    // yz
        CVector4(nb,0,nb/2),  // xz
        CVector4(nb,nb,0)       // xy
    };

    CVector4* toto;
    toto = IntersectPlan(t[0],t[1],t[2]); // frontal
    if (toto)
//      if ((toto->y >= 0.0) && (toto->z >= 0.0))
         {
            toto->x = 0.0;
            LP1 = toto;
         }
    else delete toto;
    toto = IntersectPlan(t[0],t[2],t[3]);   // Gauche
    if (toto)
//      if ((toto->x >= 0.0) && (toto->z >= 0.0))
         {
            toto->y = 0.0;
            LP2 = toto;
         }
    else delete toto;
    toto = IntersectPlan(t[0],t[1],t[3]);   // Horizontal
    if (toto)
//      if ((toto->x >= 0.0) && (toto->y >= 0.0))
         {
            toto->z = 0.0;
            LP3 = toto;
         }
    else delete toto;

    CVector4 base = GetBasePoint();
    CVector4 vec = GetDirVector();
    FCoord nordir = (FCoord) vec;
    vec = vec * (1/nordir);

    CVector4 base1,base2;
    if ((isA() == TDemiDroite3DClass) ||
         (isA() == TSegment3DClass))
        base1 = base;
    else
        base1 = base + vec * -2000.;
    if (isA() == TSegment3DClass)
        base2 = base + GetDirVector();
    else
        base2 = base + vec * 2000.;

//  if ((isA() == TDemiDroite3DClass) && (!InDemiDr(lambda)))
//      return result;


    CVector4 box[2]={limt[0],limt[6]};
    CVector4 line[2]={base1,base2};

    int rrr = Clip3D(box,line);
    CLP1 = new CVector4(line[0]);
    CLP2 = new CVector4(line[1]);
    return rrr;
}

CVector4* CDroite3D::IntersectPlan(CVector4 s1,CVector4 s2,CVector4 s3,BOOL bLim)
{
    CVector4*   result=0;
    CVector4    PP1 = GetBasePoint();
    CVector4    U   = GetDirVector(),
                VvW = (s2 - s1) % (s3 - s2),
                AB  = s1 - PP1;

    FCoord n = VvW.Norme();
    if (!n) return 0;
    VvW.x /= n;
    VvW.y /= n;
    VvW.z /= n;
    FCoord DotProd;
    if ( FCZero(DotProd = (U * VvW)) )
        return result;

    FCoord lambda   = (AB * VvW) / DotProd;

    CVector4 M( PP1.x + lambda*U.x,
                    PP1.y + lambda*U.y,
                    PP1.z + lambda*U.z,
                    1);

    if (bLim)
      if (!IsInLimit(lambda))
        return result;

//  if ((isA() == TSegment3DClass) && (!InSeg2(lambda)))
//      return result;
//  if ((isA() == TDemiDroite3DClass) && (!InDemiDr(lambda)))
//      return result;

    result = new CVector4(M);
    return result;
}

CVector4* CDroite3D::IntersectDroite(CVector4 s1,CVector4 s2)
{
    return NULL;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSegment3D, CDroite3D, VERSIONABLE_SCHEMA | 1)

CSegment3D::CSegment3D(CPoint3D *s1,CPoint3D *s2) : CDroite3D(s1,s2)
{
    bIsSegment = TRUE;
}

CSegment3D::CSegment3D(const CObject3D & src) : CDroite3D(src)
{
    bIsSegment = TRUE;
}

CObject3D* CSegment3D::CopyObject()
{
    CObject3D *temp = new CSegment3D((CObject3D&)*this);
    return temp;
}

BOOL CSegment3D::IsInLimit(FCoord x)
{
    return ((x <= 1.000001) && (x >= -1E-12));
}

CRgn*   CSegment3D::InvalideRect()
{
    CPoint  p1(P1->Visual_pt),
            p2(P2->Visual_pt);
    return DoSegRgn(p1,p2);
}

CString CSegment3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && P2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = P2->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
		    mstr.Format(_T("SegmentD[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
	        mstr.Format(_T("SegmentD(%s,%s,%s);"),strName,strObj1,strObj2);
    }
    return mstr;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDemiDroite3D, CDroite3D, VERSIONABLE_SCHEMA | 1)

CDemiDroite3D::CDemiDroite3D(CPoint3D *s1,CPoint3D *s2) : CDroite3D()
{
    bIsSegment = FALSE;
    P1 = s1;
    P2 = s2;
    nDepth = __max(s1->nDepth,s2->nDepth)+1;
}

CDemiDroite3D::CDemiDroite3D(const CObject3D & src) : CDroite3D(src)
{
    bIsSegment = FALSE;
    P1 = ((CDemiDroite3D&)src).P1;
    P2 = ((CDemiDroite3D&)src).P2;
}

CObject3D* CDemiDroite3D::CopyObject()
{
    CObject3D *temp = new CDemiDroite3D((CObject3D&)*this);
    return temp;
}

BOOL CDemiDroite3D::IsInLimit(FCoord x)
{
    return (x >= -1E-12);
}

BOOL CDemiDroite3D::IsInActiveArea(CPoint thePt)
{
    BOOL ret = CDroite3D::IsInActiveArea(thePt);
    if (ret)
     {
        CVector4 A(VP2.x-VP1.x,VP2.y-VP1.y,0);
        CVector4 B(thePt.x-VP1.x,thePt.y-VP1.y,0);
        FCoord lambda = A.x*B.x + A.y*B.y;
        FCoord beta = A * B;
        return IsInLimit(beta);
     }
    return ret;
}

CString CDemiDroite3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && P2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = P2->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
		    mstr.Format(_T("RayD[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
	        mstr.Format(_T("RayD(%s,%s,%s);"),strName,strObj1,strObj2);

    }
    return mstr;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDroitePar3D, CDroite3D, VERSIONABLE_SCHEMA | 1)

CDroitePar3D::CDroitePar3D(CPoint3D *s1,CDroite3D *s2) : CDroite3D(s1,s2)
{
    bIsSegment = FALSE;
}

CDroitePar3D::CDroitePar3D(const CObject3D & src) : CDroite3D(src)
{
    bIsSegment = FALSE;
}

CObject3D* CDroitePar3D::CopyObject()
{
    CObject3D *temp = new CDroitePar3D((CObject3D&)*this);
    return temp;
}

CString CDroitePar3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && D2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = D2->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("ParallelLine[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
		    mstr.Format(_T("ParallelLine(%s,%s,%s);"),strName,strObj1,strObj2);

    }
    return mstr;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDroiteInterPP3D, CDroite3D, VERSIONABLE_SCHEMA | 1)

CDroiteInterPP3D::CDroiteInterPP3D(CPlan3D *s1,CPlan3D *s2) : CDroite3D()
{
	bUpdateMe = TRUE;
    bIsSegment = FALSE;
    Pl1 = s1;
    Pl2 = s2;
    nDepth = __max(s1->nDepth,s2->nDepth)+1;
    PtonDr = CVector4(0,0,0);
}

CDroiteInterPP3D::CDroiteInterPP3D(const CObject3D & src) : CDroite3D(src)
{
	bUpdateMe = TRUE;
    bIsSegment = FALSE;
    Pl1 = ((CDroiteInterPP3D&)src).Pl1;
    Pl2 = ((CDroiteInterPP3D&)src).Pl2;
    PtonDr = ((CDroiteInterPP3D&)src).PtonDr;
}

int CDroiteInterPP3D::SetDepth()
{
    if (Pl1 && Pl2)
        nDepth = __max(Pl1->nDepth,Pl2->nDepth)+1;
    return nDepth;
}

void CDroiteInterPP3D::Serialize( CArchive& ar )
{
    CDroite3D::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << DrDir;
        ar << DrPt;
        ar << ((Pl1) ? Pl1->nObjectId : -1);
        ar << ((Pl2) ? Pl2->nObjectId : -1);
    }
    else
    {
        ar >> DrDir;
        ar >> DrPt;

        Pl1 = (CPlan3D*)SerializeObj(ar);
        Pl2 = (CPlan3D*)SerializeObj(ar);
    }
}


CObject3D* CDroiteInterPP3D::CopyObject()
{
    CObject3D *temp = new CDroiteInterPP3D((CObject3D&)*this);
    return temp;
}

CString CDroiteInterPP3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (Pl1) sn1 = Pl1->GetObjectHelp();
    if (Pl2) sn2 = Pl2->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;

}

CxObject3DSet* CDroiteInterPP3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(Pl1);
    list->Add(Pl2);
    return list;
}

CVector4  CDroiteInterPP3D::GetDirVector()
{
    CDirVector = DrDir;
    return (CDirVector * 100.);
}

CVector4  CDroiteInterPP3D::GetBasePoint()
{
    return DrPt;
}

#define MRG_ZERO 1.0e-8
//#define CALCUL_INTER_2

UINT  CDroiteInterPP3D::CalculConceptuel()
{
    bValidate = ((Pl1->bValidate) && (Pl2->bValidate));
    if (!bValidate)
        return ERR_NODROITE;

    CVector4 N1 = Pl1->GetNormalForm();
    CVector4 N2 = Pl2->GetNormalForm();

#ifdef CALCUL_INTER_2
    DrDir = (N1 % N2);  /* dir droite */
    FCoord invdet;  /* inverse of 2x2 matrix determinant */
    CVector4 dir2(  DrDir.x * DrDir.x,
                        DrDir.y * DrDir.y,
                        DrDir.z * DrDir.z);
        /* holds the squares of the coordinates of DrDir */

    if (DrDir.NullVector())
     {
        bValidate = FALSE;
        return ERR_PLANEPAR;
     }


    if (dir2.z > dir2.y && dir2.z > dir2.x && dir2.z > MRG_ZERO)
//  if (dir2.z > MRG_ZERO)
     {
     /* then get a point on the XY plane */
     invdet = 1.0 / DrDir.z;
     /* solve < pl1.x * DrPt.x + pl1.y * DrPt.y = - pl1.w >
                 < pl2.x * DrPt.x + pl2.y * DrPt.y = - pl2.w > */
     DrPt = CVector4(N1.y * N2.w - N2.y * N1.w,
                         N2.x * N1.w - N1.x * N2.w,
                        0.0);
     }
    else if (dir2.y > dir2.x && dir2.y > MRG_ZERO)
    //else if (dir2.y > MRG_ZERO)
     {
     /* then get a point on the XZ plane */
     invdet = -1.0 / DrDir.y;
     /* solve < pl1.x * DrPt.x + pl1.z * DrPt.z = -pl1.w >
                 < pl2.x * DrPt.x + pl2.z * DrPt.z = -pl2.w > */
     DrPt = CVector4(N1.z * N2.w - N2.z * N1.w, 0.0,
                        N2.x * N1.w - N1.x * N2.w);
     }
    else if (dir2.x > MRG_ZERO)
     {
     /* then get a point on the YZ plane */
     invdet = 1.0 / DrDir.x;
     /* solve < pl1.y * DrPt.y + pl1.z * DrPt.z = - pl1.w >
                 < pl2.y * DrPt.y + pl2.z * DrPt.z = - pl2.w > */
     DrPt = CVector4(0.0, N1.z * N2.w - N2.z * N1.w,
                        N2.y * N1.w - N1.y * N2.w);
     }
    else
     {
        bValidate = FALSE;
        return ERR_PLANEPAR;
     }
    DrPt = (DrPt * invdet);
    invdet = 1.0 / (FCoord)sqrtl(dir2.x + dir2.y + dir2.z);
    DrDir = (DrDir * invdet);

#else
     FCoord n00 = (N1 * N1);
     FCoord n01 = (N1 * N2);
     FCoord n11 = (N2 * N2);
     FCoord det = n00*n11-n01*n01;

     const double tolerance = MRG_ZERO;
     if ( fabs(det) < tolerance )
     {
         bValidate = FALSE;
         return ERR_PLANEPAR;
     }

     FCoord invDet = 1.0f/det;
     FCoord c0 = -(n11*N1.w - n01*N2.w)*invDet;
     FCoord c1 = -(n00*N2.w - n01*N1.w)*invDet;

    CVector4 N1p(N1.x,N1.y,N1.z,1);
    CVector4 N2p(N2.x,N2.y,N2.z,1);

    DrPt = N1p*c0 + N2p*c1;
    DrDir=  N1p % N2p;
#endif

    /*int nb2 = */ClippingDroite();
	bUpdateMe = TRUE;
    return 0;
}

void ComputePlaneLimits(CDroite3D* ,CPlan3D *myplan,CPoint3D* mypt)
{
	CVector4    U = myplan->plRep.I,
	V = myplan->plRep.J;
	FCoord normK = 1 / myplan->VecNorm.Norme();
	CVector4    K = myplan->VecNorm * normK;
	CVector4    J(-K.y,K.x,0,1);
	normK = J.Norme();
	if (FCZero(normK))
		{ J.x = J.z =0; J.y = 1;    }
	else
		J = J * (1/normK);
	CVector4    I = J % K;
	CVector4 re = myplan->planeBorder;
	int PLANEMARGE = 20;
	re.x+=PLANEMARGE;
	re.y+=PLANEMARGE;
	re.z-=PLANEMARGE;
	re.w-=PLANEMARGE;

	myplan->GetProjectedMinMax(mypt,myplan->P1->Concept_pt,I,J,K,re);

	re.x-=PLANEMARGE;
	re.y-=PLANEMARGE;
	re.z+=PLANEMARGE;
	re.w+=PLANEMARGE;


        CVector4 lim[4] = {
                CVector4(re.x,re.y,0),
                CVector4(re.x,re.w,0),
                CVector4(re.z,re.w,0),
                CVector4(re.z,re.y,0)};
        for (int t=0;t<4;t++)
         {
            CVector4 U = lim[t];
            lim[t].x =  I.x*U.x + J.x*U.y + K.x*U.z + myplan->P1->Concept_pt.x,
            lim[t].y =  I.y*U.x + J.y*U.y + K.y*U.z + myplan->P1->Concept_pt.y,
            lim[t].z =  I.z*U.x + J.z*U.y + K.z*U.z + myplan->P1->Concept_pt.z,
            lim[t].w =  1;
        }
        myplan->p1 = lim[0];
        myplan->p2 = lim[1];
        myplan->p3 = lim[2];
       myplan-> p4 = lim[3];
}

void ComputeIntersections(CDroiteInterPP3D* mydroite,CPlan3D *myplan,CVector4 ppp1)
{
	CPoint3D pt1(ppp1);
	CDroitePerp3D dr1(&pt1,mydroite);
	if (dr1.CalculConceptuel()!=0) return;
	
	CPointInterDD3D pti(mydroite,&dr1);
	if (pti.CalculConceptuel()!=0) return;

	ComputePlaneLimits(mydroite,mydroite->Pl1,&pti);
	ComputePlaneLimits(mydroite,mydroite->Pl2,&pti);
}



void CDroiteInterPP3D::CalculVisuel(CVisualParam *pVisParam)
{
	CDroite3D::CalculVisuel(pVisParam);
	if  (!bUpdateMe) return;

// 	CVector4 pterr1 = Pl1->p1;
// 	CVector4 pterr2 = Pl1->p2;
// 	CVector4 pterr3 = Pl1->p3;
// 	CVector4 pterr4 = Pl1->p4;
// 	CVector4 pters1 = Pl2->p1;
// 	CVector4 pters2 = Pl2->p2;
// 	CVector4 pters3 = Pl2->p3;
// 	CVector4 pters4 = Pl2->p4;
// 	ComputeIntersections(this,Pl1,pterr1);
	bUpdateMe = FALSE;

}

CString CDroiteInterPP3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */Pl1 && Pl2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = Pl1->GetObjectNameRedux();
		strObj2 = Pl2->GetObjectNameRedux();

		CObjectId nType1 = Pl1->isA();
        CObjectId nType2 = Pl2->isA();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("IntersectionPlanes[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
		    mstr.Format(_T("IntersectionPlanes(%s,%s,%s);"),strName,strObj1,strObj2);

    }
    return mstr;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDroitePerp3D, CDroite3D, VERSIONABLE_SCHEMA | 1)

CDroitePerp3D::CDroitePerp3D(CPoint3D *s1,CDroite3D *s2) : CDroite3D(s1,s2)
{
    P3 = P4 = 0;
    Pl1 = 0;
    bIsSegment = FALSE;
}

CDroitePerp3D::CDroitePerp3D(CPoint3D *s1,CPlan3D *s2) : CDroite3D()
{
    P1 = s1;
    P3 = P4 = NULL;
    Pl1 = s2;
    bIsSegment = FALSE;
    nDepth = __max(s1->nDepth,s2->nDepth)+1;
    //Concept_pt = s2->GetDirVector();
}

CDroitePerp3D::CDroitePerp3D(CPoint3D *s1,CPoint3D *p1,CPoint3D *p2,CPoint3D *p3) : CDroite3D()
{
    P1 = s1;
    P2 = p1;
    P3 = p2;
    P4 = p3;
    Pl1 = 0;
    bIsSegment = FALSE;
    nDepth = __max(s1->nDepth,__max(p1->nDepth,__max(p2->nDepth,p3->nDepth)))+1;

}

int CDroitePerp3D::SetDepth()
{
    if (P1 && Pl1)
        nDepth = __max(P1->nDepth,Pl1->nDepth)+1;
    else if (P1 && P2 && P3 && P4)
        nDepth = __max(P1->nDepth,__max(P2->nDepth,__max(P3->nDepth,P4->nDepth)))+1;

    return nDepth;
}


void CDroitePerp3D::Serialize( CArchive& ar )
{
    CDroite3D::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << IntPt;
        ar << ((P3) ? P3->nObjectId : -1);
        ar << ((P4) ? P4->nObjectId : -1);
        ar << ((Pl1) ? Pl1->nObjectId : -1);
    }
    else
    {
        ar >> IntPt;

        P3 = (CPoint3D*)SerializeObj(ar);
        P4 = (CPoint3D*)SerializeObj(ar);
        Pl1 = (CPlan3D*)SerializeObj(ar);
    }
}


CDroitePerp3D::CDroitePerp3D(const CObject3D & src) : CDroite3D(src)
{
    bIsSegment = FALSE;
    P3 =  ((CDroitePerp3D&)src).P3;
    P4 =   ((CDroitePerp3D&)src).P4;
    Pl1 =   ((CDroitePerp3D&)src).Pl1;

}

CObject3D* CDroitePerp3D::CopyObject()
{
    CObject3D *temp = new CDroitePerp3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CDroitePerp3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(P1);
    if (D2) list->Add(D2);
    else
    if (Pl1)    list->Add(Pl1);
    else
     {
        list->Add(P2);
        list->Add(P3);
        list->Add(P4);
     }
    return list;
}

CString CDroitePerp3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (P1) sn1 = P1->GetObjectHelp();
    if (D2)
        sn2 = D2->GetObjectHelp();
    else if (Pl1)
        sn2 = Pl1->GetObjectHelp();

    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}



CVector4  CDroitePerp3D::GetDirVector()
{
    return (CDirVector * 100.);
}

CVector4  CDroitePerp3D::GetBasePoint()
{
    return CDroite3D::GetBasePoint();
}

CVector4  CDroitePerp3D::GetIntersectionPoint()
{
	return IntPt;
}


UINT  CDroitePerp3D::CalculConceptuel()
{
    if (D2)
        bValidate = ((P1->bValidate) && (D2->bValidate));
    else
    if (Pl1)
        bValidate = ((P1->bValidate) && (Pl1->bValidate));
    else
        bValidate = ((P1->bValidate) && (P2->bValidate) &&
                        (P3->bValidate) && (P4->bValidate));
    if (!bValidate)
        return ERR_NODROITE;

    CVector4 p1(P1->Concept_pt);
    if (D2)
     {
        CVector4 p2(D2->GetBasePoint());
        CVector4 p3(D2->GetDirVector());
        p3 = p2+p3;
        CPoint3D a(p1);
        CPoint3D b(p2);
        CPoint3D c(p3);
        CPlan3D plan(&a,&b,&c);
        UINT h = plan.CalculConceptuel();
        if (h)
         {
            bValidate = 0;
            return ERR_PTONLINE;
         }


        p3 = D2->GetDirVector();
        p3 = p3 * (1/p3.Norme());
        CVector4 Norm = plan.VecNorm;
        Norm = Norm * (1/Norm.Norme());
        CDirVector = (p3 % Norm);

        CPointInterDD3D pt(this,D2);
        h = pt.CalculConceptuel();
        IntPt = pt.Concept_pt;
     }
    else
     {
        CVector4 Norm;
        CPointInterDP3D* mypt = 0;
        if (Pl1)
         {
            Norm = Pl1->VecNorm;
            Norm = Norm * (1/Norm.Norme());
            mypt = new CPointInterDP3D(this,Pl1);
         }
        else
         {
            CPlan3D plan(P2,P3,P4);
            plan.CalculConceptuel();
            Norm = plan.VecNorm;
            Norm = Norm * (1/Norm.Norme());
            mypt = new CPointInterDP3D(this,Pl1);
         }
        CDirVector = Norm;
        if (mypt)
         {  UINT h = mypt->CalculConceptuel();
            IntPt = mypt->Concept_pt;
            delete mypt;
         }
     }

    /*int nb2 = */ClippingDroite();
    return 0;
}

void CDroitePerp3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible && !TPref::TUniv.bShowHidden) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;
    CDroite3D::Draw(pDC,mV,bSm);

    CVector4 dis = IntPt - P1->Concept_pt;
    if (!dis.NullVector() && (bMarked && mV->bDrawMark))
     {
        COLORREF curC = RGB(0,0,0);
        CBrush br(curC);
        CPen curP(PS_SOLID,1,curC);
        CPen *oldP = pDC->SelectObject(&curP);
        CBrush *oldB = pDC->SelectObject(&br);
        CRect mrect((CPoint)mV->ProjectPoint(IntPt) - CSize(1,1),CSize(3,3));
        pDC->Rectangle(mrect);
        pDC->SelectObject(oldP);
        pDC->SelectObject(oldB);
     }

}

CString CDroitePerp3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */P1 && D2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = D2->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("PerpendicularLine[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
		   mstr.Format(_T("PerpendicularLine(%s,%s,%s);"),strName,strObj1,strObj2);

    }
    else if (/*bValidate && */P1 && Pl1)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = Pl1->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("NormalLine[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
		   mstr.Format(_T("NormalLine(%s,%s,%s);"),strName,strObj1,strObj2);
    }
    return mstr;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDroitePerpDD3D, CDroite3D, VERSIONABLE_SCHEMA | 1)

CDroitePerpDD3D::CDroitePerpDD3D(CDroite3D *s1,CDroite3D *s2) : CDroite3D()
{
    bIsSegment = FALSE;
    D2 = s1;
    D3 = s2;
    nDepth = __max(s1->nDepth,s2->nDepth)+1;
}

CDroitePerpDD3D::CDroitePerpDD3D(const CObject3D & src) : CDroite3D(src)
{
    bIsSegment = FALSE;
    D2 = ((CDroitePerpDD3D&)src).D2;
    D3 = ((CDroitePerpDD3D&)src).D3;
}

int CDroitePerpDD3D::SetDepth()
{
    if (D2 && D3)
        nDepth = __max(D2->nDepth,D3->nDepth)+1;
    return nDepth;
}

void CDroitePerpDD3D::Serialize( CArchive& ar )
{
    CDroite3D::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << DrDir;
        ar << DrPt;
        ar << ((D3) ? D3->nObjectId : -1);
    }
    else
    {
        ar >> DrDir;
        ar >> DrPt;

        D3 = (CDroite3D*)SerializeObj(ar);
    }
}


CObject3D* CDroitePerpDD3D::CopyObject()
{
    CObject3D *temp = new CDroitePerpDD3D((CObject3D&)*this);
    return temp;
}

CString CDroitePerpDD3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (D2) sn1 = D2->GetObjectHelp();
    if (D3) sn2 = D3->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;

}

CxObject3DSet* CDroitePerpDD3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(D2);
    list->Add(D3);
    return list;
}

CVector4  CDroitePerpDD3D::GetDirVector()
{
    CDirVector = DrDir;
    return (CDirVector * 100.);
}

CVector4  CDroitePerpDD3D::GetBasePoint()
{
    return DrPt;
}

UINT  CDroitePerpDD3D::CalculConceptuel()
{
    bValidate = ((D2->bValidate) && (D3->bValidate));
    if (!bValidate)
        return ERR_NODROITE;

    CVector4 p1 = D2->GetBasePoint();
    CVector4 p2 = p1 + D2->GetDirVector();

    CVector4 p3 = D3->GetBasePoint();
    CVector4 p4 = p3 + D3->GetDirVector();

    CVector4    p13,p43,p21;
    CVector4    pa,pb;
    FCoord      d1343,d4321,d1321,d4343,d2121;
    FCoord      numer,denom;

    FCoord      mua,mub;

    p13 = p1 - p3;
    p43 = p4 - p3;

    //p13.x = p1.x - p3.x;
    //p13.y = p1.y - p3.y;
    //p13.z = p1.z - p3.z;
    //p43.x = p4.x - p3.x;
    //p43.y = p4.y - p3.y;
    //p43.z = p4.z - p3.z;

    //if (FCZero(p43.x)  && FCZero(p43.y)  && FCZero(p43.z))
    if (p43.NullVector())
    {
        bValidate = FALSE;
        return ERR_NODROITE;
    }

    p21 = p2 - p1;

    //p21.x = p2.x - p1.x;
    //p21.y = p2.y - p1.y;
    //p21.z = p2.z - p1.z;
    //if (FCZero(p21.x) && FCZero(p21.y) && FCZero(p21.z) )
    if (p21.NullVector())
    {
        bValidate = FALSE;
        return ERR_NODROITE;
    }

    d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
    d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
    d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
    d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
    d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

    denom = d2121 * d4343 - d4321 * d4321;
    if (FCZero(denom))
    {
        bValidate = FALSE;
        return ERR_DROITEPAR;
    }
    numer = d1343 * d4321 - d1321 * d4343;

    mua = numer / denom;
    mub = (d1343 + d4321 * mua) / d4343;

    if (!D2->IsInLimit(mua) || !D3->IsInLimit(mub))
    {
        bValidate = FALSE;
        return ERR_INTERINSEG;
    }

    pa = p1 + p21 * mua;
    pb = p3 + p43 * mub;

    //pa.x = p1.x + mua * p21.x;
    //pa.y = p1.y + mua * p21.y;
    //pa.z = p1.z + mua * p21.z;
    //pb.x = p3.x + mub * p43.x;
    //pb.y = p3.y + mub * p43.y;
    //pb.z = p3.z + mub * p43.z;

    DrPt = pa;
    DrDir = pb - pa;

    if (DrDir.NullVector())
    {
        //bValidate = FALSE;
        //return ERR_NODROITE;
        DrDir = D2->GetDirVector() % D3->GetDirVector();
    }
    DrDir = DrDir.Normalized();

    ClippingDroite();

    return 0;
}


CString CDroitePerpDD3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */D3 && D2)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = D2->GetObjectNameRedux();
		strObj2 = D3->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("CommonPerpendicularLine[%s,%s,%s];"),strName,strObj1,strObj2);
		else if (nFormat==EXPORT_MAPLE)
		   mstr.Format(_T("CommonPerpendicularLine(%s,%s,%s);"),strName,strObj1,strObj2);

    }
    return mstr;
}
