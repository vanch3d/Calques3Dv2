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
/// @file Cone3D.cpp
/// @brief Implementation of the CCone3D class.
/// $Date: 2007-10-28 11:19:57+00 $
/// $Revision: 1.12 $
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\calques3d.h"
#include "..\Prefs\Prefs.h"
#include "..\OGLTools\glut.h"

#include "Geom.h"

#include "Cone3D.h"
#include "Point3D.h"
#include "Cercle3D.h"
#include "Droite3D.h"
#include "Plan3D.h"
#include "Sphere3D.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//***************************************************************************
// CCone3D
//***************************************************************************
IMPLEMENT_SERIAL(CCone3D, CVolumeObject3D, VERSIONABLE_SCHEMA | 1)


CCone3D::CCone3D() : CVolumeObject3D()
{
	pObjectShape.clrObject = TPref::TCone.clrDefault;
	pApex = pAxis = pRadius = NULL;
	C1 = C2 = C3 = NULL;
	Base1 = Base2 = Base3 = NULL;
	nDelta = TPref::TCone.nbDelta;
}

CCone3D::CCone3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *rad) : CVolumeObject3D()
{
	pObjectShape.clrObject = TPref::TCone.clrDefault;
	pApex =  p1;
	pAxis =   p2;
	pRadius =   rad;
	SetDepth();
	C1 = C2 = C3 = NULL;
	Base1 = Base2 = Base3 = NULL;
	nDelta = TPref::TCone.nbDelta;
}

CCone3D::CCone3D(const CObject3D & src) : CVolumeObject3D(src)
{
    pApex =  ((CCone3D&)src).pApex;
    pAxis =   ((CCone3D&)src).pAxis;
    pRadius =   ((CCone3D&)src).pRadius;
 	SetDepth();
	C1 = C2 = C3 = NULL;
	Base1 = Base2 = Base3 = NULL;
	nDelta = TPref::TCone.nbDelta;
}

CCone3D::~CCone3D()
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

CObject3D* CCone3D::CopyObject()
{
    CObject3D *temp = new CCone3D((CObject3D&)*this);
    return temp;
}

void CCone3D::Serialize( CArchive& ar )
{
    CVolumeObject3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((pApex) ? pApex->nObjectId : -1);
        ar << ((pAxis) ? pAxis->nObjectId : -1);
        ar << ((pRadius) ? pRadius->nObjectId : -1);
    }
    else
    {
        pApex = (CPoint3D*)SerializeObj(ar);
        pAxis = (CPoint3D*)SerializeObj(ar);
        pRadius = (CPoint3D*)SerializeObj(ar);
		C1 = C2 = C3 = NULL;
		Base1 = Base2 = Base3 = NULL;
		nDelta = TPref::TCone.nbDelta;
    }
}

CxObject3DSet* CCone3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(pApex);
    list->Add(pAxis);
    list->Add(pRadius);
    return list;
}

CString CCone3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (pApex) sn1 = pApex->GetObjectName();
    if (pAxis) sn2 = pAxis->GetObjectName();
    if (pRadius) sn3 = pRadius->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2,sn3);
    return mstr;
}

int CCone3D::SetDepth()
{
	if (pApex && pAxis && pRadius)
        nDepth = max(pApex->nDepth,max(pAxis->nDepth,pRadius->nDepth))+1;
    return nDepth;
}


int CCone3D::SetProperties(CxObject3DSet *pSet)
{
    int ret = CVolumeObject3D::SetProperties(pSet);
    if (ret!=IDOK) return ret;

	if (C1)
	{
		C1->pObjectShape.clrObject = pObjectShape.clrObject;
		C1->pObjectShape.nShapeId = pObjectShape.nShapeId;
	}
	if (C2)
	{
		//C2->pObjectShape.clrObject = pObjectShape.clrObject;
        //C2->pObjectShape.nShapeId = pObjectShape.nShapeId;
	}
    if (C3)
    {
		C3->pObjectShape.clrObject = pObjectShape.clrObject;
		C1->pObjectShape.nShapeId = pObjectShape.nShapeId;
	}
    return ret;
}


CString CCone3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */pApex && pAxis && pRadius)
    {
        CString strName,strObj1,strObj2,strObj3;
		strName = GetObjectNameRedux();
		strObj1 = pApex->GetObjectNameRedux();
		strObj2 = pAxis->GetObjectNameRedux();
		strObj3 = pRadius->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("ConeD[%s,%s,%s,%s];"),strName,strObj1,strObj2,strObj3);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("ConeD(%s,%s,%s,%s);"),strName,strObj1,strObj2,strObj3);

    }
    return mstr;
}


UINT  CCone3D::CalculConceptuel()
{
	bValidate = ((pApex->bValidate) && (pAxis->bValidate) && (pRadius->bValidate));
    if (!bValidate)
        return ERR_CONE_UNDEFINED;

	// Check the axis
    CVector4 p1p2 = pAxis->Concept_pt - pApex->Concept_pt;
	FCoord dAxis = p1p2.Norme();
	p1p2=p1p2.Normalized();
    if (p1p2.NullVector())
    {
        bValidate = 0;
        return ERR_CONE_NOAXIS;
    }

	// Check the radius
    nRadius = CGeom::GetDistanceToLine(pRadius->Concept_pt,pApex->Concept_pt,p1p2);
    if (FCZero(nRadius))
    {
        bValidate = 0;
        return ERR_CONE_NORADIUS;
    }

	// Check the envelope
    CVector4 p1rad = pRadius->Concept_pt - pApex->Concept_pt;
	FCoord proj = p1rad * p1p2;
    if (FCZero(proj) || proj<0)
    {
        bValidate = 0;
        return ERR_CONE_NOPROJAXIS;
    }

	CVector4 ptProj = pApex->Concept_pt + p1p2*proj;
	if (Base1!=NULL) delete Base1;
	if (Base2!=NULL) delete Base2;
	if (Base3!=NULL) delete Base3;
	Base1 = new CPoint3D(pAxis->Concept_pt);
	Base2 = new CPoint3D(ptProj);
	FCoord secdis = max(dAxis,proj);
	Base3 = new CPoint3D(pApex->Concept_pt +  p1p2*(-secdis));

	CDemiDroite3D drAxis(pApex,pAxis);
	CDemiDroite3D drEnv(pApex,pRadius);
	drAxis.CalculConceptuel();
	drEnv.CalculConceptuel();
	CPlanPerp3D plBase1(Base1,&drAxis);
	CPlanPerp3D plBase2(Base2,&drAxis);

	plBase1.CalculConceptuel();
	plBase2.CalculConceptuel();
	CPointInterDP3D ptRad1(&drEnv,&plBase1);
	CPointInterDP3D ptRad2(&drEnv,&plBase2);
	ptRad1.CalculConceptuel();
	ptRad2.CalculConceptuel();
		
    FCoord dr1 = CGeom::GetDistanceToLine(ptRad1.Concept_pt,pApex->Concept_pt,p1p2);
    FCoord dr2 = CGeom::GetDistanceToLine(ptRad2.Concept_pt,pApex->Concept_pt,p1p2);
	
	nRadius = max(dr1,dr2);
	nHeight = secdis;
	nOpenAngle = atan(nRadius/nHeight);

	FCoord ss = RTD(nOpenAngle);

	if (C1!=NULL) delete C1;
	if (C2!=NULL) delete C2;
	if (C3!=NULL) delete C3;

	//C1 = new CCercle3D(Base1,-p1p2,dr1);
	//C2 = new CCercle3D(Base2,-p1p2,dr2);
	if (dr1==dr2)
	{
		C1 = new CCercle3D(Base1,-p1p2,dr1);
		C1->pObjectShape.clrObject = pObjectShape.clrObject;
		C2 = NULL;
	}
	else if (dr1>dr2)
	{
		C1 = new CCercle3D(Base1,-p1p2,dr1);
		C2 = new CCercle3D(Base2,-p1p2,dr2);
		C1->pObjectShape.clrObject = pObjectShape.clrObject;
		C2->pObjectShape.nShapeId = 0;
		C2->pObjectShape.clrObject = RGB(0,0,0);
	}
	else
	{
		C1 = new CCercle3D(Base2,-p1p2,dr2);
		C2 = new CCercle3D(Base1,-p1p2,dr1);
		C1->pObjectShape.clrObject = pObjectShape.clrObject;
		C2->pObjectShape.nShapeId = 0;
		C2->pObjectShape.clrObject = RGB(0,0,0);
	}
	C3 = new CCercle3D(Base3,-p1p2,nRadius);
    C3->pObjectShape.clrObject = pObjectShape.clrObject;

    LocRep.O = pApex->Concept_pt;
    LocRep.K = p1p2;
    LocRep.K = LocRep.K.Normalized();
    LocRep.J = pRadius->Concept_pt - ptProj;
    LocRep.J = LocRep.J.Normalized();
    LocRep.I = LocRep.J % LocRep.K;

	if (C1) C1->CalculConceptuel();
	if (C2) C2->CalculConceptuel();
	if (C3) C3->CalculConceptuel();
	return 0;
}

CVector4 calculGeneratrice(FCoord theta,CVector4 Center,
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

void CCone3D::CalculVisuel(CVisualParam *vp)
{
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

	if (C1) C1->CalculVisuel(vp);
	if (C2) C2->CalculVisuel(vp);
	if (C3) C3->CalculVisuel(vp);
    CVector4 p1p2 = C1->VecNorm;
    CVector4 oeil= vp->GetEyePos();
    CVector4 beep = oeil;// * ((FCoord)1/Norme(oeil));
    CVector4 beep2 = p1p2;// * ((FCoord)1/Norme(p1p2));
    CVector4 aa = beep % beep2;

    FCoord normK = 1 / p1p2.Norme();
    CVector4 K = p1p2 * normK,
             I,J;
    if (aa.NullVector())
     {
        I = oeil;//CVector4(-K.y,K.x,0,1);
	    if (I.NullVector())
			I = CVector4(1,0,0,1);
		K = p1p2;
        normK = K.Norme();
        if (FCZero(normK))
            { I.x = I.z =0; I.y = 1;    }
        else
            I = I * (1/normK);
     }
    else
        I = aa * (1 /aa.Norme());
	//I =oeil.Normalized();
	//I.Norme();
    J = (I % K);
    //CLocalRep IntH = CLocalRep(pApex->Concept_pt,I,J,K);

// 	CVector4 VisuNorm= vp->GetEyePos() - CVector4(0,0,0,1);
// 	CPoint3D pta(pApex->Concept_pt + VisuNorm);
// 	CDroite3D dr(pApex,&pta);
// 	dr.CalculConceptuel();
// 	CPoint3D ff(C1->Center);
// 	CPlan3D pl(&ff,(pApex->Concept_pt-pAxis->Concept_pt).Normalized());
// 	pl.CalculConceptuel();
// 	CPointInterDP3D fff(&dr,&pl);
// 	fff.CalculConceptuel();
// 	CPoint3D rr(C1->Center+CVector4(0,0,1)*C1->Radius);
// 	CPoint3D rd(C1->Center);
// 	CSphere3D sp1(&rd,&rr);
// 	sp1.CalculConceptuel();
// 	CPointMilieu3D mil(&fff,&ff);
// 	mil.CalculConceptuel();
// 	CSphere3D sp2(&mil,&ff);
// 	sp2.CalculConceptuel();
// 	CCercleInterSS3D ci(&sp1,&sp2);
// 	ci.CalculConceptuel();
// 	CInterCircPlane3D ptint(&ci,&pl);
// 	UINT res = ptint.CalculConceptuel();

	nDelta =  TPref::TCone.nbDelta;
    for (int t=0;t<nDelta;t++)
    {
		FCoord theta = /*start+*/2*M_PI*t/nDelta;
        CVector4 V1 = calculGeneratrice(theta,C1->Center,nRadius,LocRep.I,LocRep.J,LocRep.K);
        CVector4 V2;
		if (TPref::TCone.bDoubleCone)
			V2 = calculGeneratrice(M_PI+theta,C3->Center,nRadius,LocRep.I,LocRep.J,LocRep.K);
		else
			V2 = pApex->Concept_pt;

		if (theta>2*M_PI) theta = theta - 2*M_PI;

        CPoint3D    *spt1 = NULL ,
                    *spt2 = NULL;

		spt1 = new CPoint3D(V1);
        spt2 = new CPoint3D(V2);
        cPointsList.Add(spt1);
		cPointsList.Add(spt2);

        spt1->CalculConceptuel();
        spt1->CalculVisuel(vp);
        spt2->CalculConceptuel();
        spt2->CalculVisuel(vp);
        
		CSegment3D *seg = 0;

		seg = new CSegment3D(spt1,spt2);
		cGenerList.Add(seg);
        seg->pObjectShape.clrObject = pObjectShape.clrObject;

        seg->CalculConceptuel();
        seg->CalculVisuel(vp);
        seg->nCalque = nCalque;

		{ 
            seg->pObjectShape.clrObject = pObjectShape.clrObject;
			seg->pObjectShape.nShapeId = pObjectShape.nShapeId;

		}
/*		{
			double h,s,l;
			CBCGPDrawManager::RGBtoHSL(pObjectShape.clrObject,&h,&s,&l);
			l = l+3*l/4;
			COLORREF ff = CBCGPDrawManager::HLStoRGB_ONE(h,220./255.,s);
			seg->pObjectShape.clrObject = ff;
			seg->pObjectShape.nShapeId = pObjectShape.nShapeId;
		}*/

     }
}

// uncomment this line to draw the debug feedbacks
//#define __DEBUG_CONE 1

void CCone3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSm)
{
    if ((!bVisible && !TPref::TUniv.bShowHidden) || (!bValidate) || (!IsInCalque(vp->nCalqueNum))) return;

	if (!bVisible && TPref::TUniv.bShowHidden)
	{
		if (C1) C1->bVisible = FALSE;
		if (C2) C2->bVisible = FALSE;
		if (C3) C3->bVisible = FALSE;
	}
	else
	{
		if (C1) C1->bVisible = TRUE;
		if (C2) C2->bVisible = TRUE;
		if (C3) C3->bVisible = TRUE;
	}

	if (C1) C1->Draw(pDC,vp,bSm);
	if (C2) C2->Draw(pDC,vp,bSm);
	if (C3 && TPref::TCone.bDoubleCone) C3->Draw(pDC,vp,bSm);

#ifdef __DEBUG_CONE
	{	// Draw the local referential
 		pDC->MoveTo(vp->ProjectPoint(C1->Center));
 		pDC->LineTo(vp->ProjectPoint(C1->Center+LocRep.I*50.));
 		pDC->MoveTo(vp->ProjectPoint(C1->Center));
 		pDC->LineTo(vp->ProjectPoint(C1->Center+LocRep.J*100.));
		pDC->MoveTo(vp->ProjectPoint(C1->Center));
		pDC->LineTo(vp->ProjectPoint(C1->Center+LocRep.K*50.));
	}
#endif

	// Compute the visual envelope
	CVector4 oeil= vp->GetEyePos();
	CVector4 origin(0,0,0,1);
	CVector4 VisuNorm= oeil - origin;
	CPoint3D pta(pApex->Concept_pt + VisuNorm);
	CDroite3D dr(pApex,&pta);
	dr.CalculConceptuel();
	CPoint3D ff(C1->Center);
	CPlan3D pl(&ff,(pApex->Concept_pt-pAxis->Concept_pt).Normalized());
	pl.CalculConceptuel();
	CPointInterDP3D fff(&dr,&pl);
	fff.CalculConceptuel();
	CPoint3D rr(C1->Center+CVector4(0,0,1)*C1->Radius);
	CPoint3D rd(C1->Center);
	CSphere3D sp1(&rd,&rr);
	sp1.CalculConceptuel();
	CPointMilieu3D mil(&fff,&ff);
	mil.CalculConceptuel();
	CSphere3D sp2(&mil,&ff);
	sp2.CalculConceptuel();
	CCercleInterSS3D ci(&sp1,&sp2);
	ci.CalculConceptuel();
	CInterCircPlane3D ptint(&ci,&pl);
	UINT res = ptint.CalculConceptuel();

	FCoord end=0,end2=0;
	CVector4 I(0,0,0),J(0,0,0),K(0,0,0);

	if (res==0)
	{
#ifdef __DEBUG_CONE
// 		// Draw the two extreme edges of the envelope
// 		CSegment3D seg1(pApex,ptint.ptA);
// 		CSegment3D seg2(pApex,ptint.ptB);
// 		seg1.pObjectShape.clrObject = RGB(255,0,0);
// 		seg1.pObjectShape.nShapeId = 6;
// 		seg2.pObjectShape.clrObject = RGB(0,255,0);
// 		seg2.pObjectShape.nShapeId = 6;
// 		seg1.CalculConceptuel();
// 		seg2.CalculConceptuel();
// 
// 		seg1.CalculVisuel(vp);
// 		seg1.Draw(pDC,vp,bSm);
// 		seg2.CalculVisuel(vp);
// 		seg2.Draw(pDC,vp,bSm);
#endif

		CVector4 ptLimA =  ptint.ptA->Concept_pt;
		CVector4 ptLimB =  ptint.ptB->Concept_pt;
		I = (ptLimA - C1->Center).Normalized();
		I.Norme();
		J = (ptLimB - C1->Center).Normalized();
		J.Norme();
		K = I  % J;
		FCoord dd = K * LocRep.K;
		if (dd<0)
		{
			CVector4 f = I;
			K = I;
			I = J;
			J = K;
			K = I%J;
			J = K%I;
			end = I * f;
			end2 = J * f;
		}
		else
		{
			CVector4 f = J;
			J = K%I;
			end = I * f;
			end2 = J * f;
		}
#ifdef __DEBUG_CONE
		pDC->MoveTo(vp->ProjectPoint(C1->Center));
		pDC->LineTo(vp->ProjectPoint(C1->Center+I*50.));
		pDC->MoveTo(vp->ProjectPoint(C1->Center));
		pDC->LineTo(vp->ProjectPoint(C1->Center+J*100.));
		pDC->MoveTo(vp->ProjectPoint(C1->Center));
		pDC->LineTo(vp->ProjectPoint(C1->Center+K*150.));
#endif
	}

	FCoord start =0 ;

	//start =acos(start);
	end =acos(end);
// 	if (LocRep.J*I < 0)
// 		start = M_PI + start;
	if (end2 < 0)
		end = M_PI + end;
	//FCoord dend = RTD(end);
	//FCoord dstart = RTD(start);

	for (int t=0;t<cGenerList.GetSize();t++)
    {
        CSegment3D *seg = (CSegment3D*)cGenerList[t];
		if (!seg) continue;

		CVector4 pt = seg->P1->Concept_pt - C1->Center;
		pt = pt.Normalized();
		pt.Norme();

		//if (res!=0) continue;
		FCoord ca = I * pt;
		FCoord sa = J * pt;
		ca =acos(ca);
		if (sa < 0)
			ca = M_PI+ca;
		
		//if (t==0)
		{
			//CPen *oldP = NULL;

			//CPen curPen(PS_SOLID,1,RGB(255,0,0)),
			//	 disPen(PS_SOLID,1,RGB(0,255,0));
			BOOL isInside = ca <end;
			//if (dd<0) isInside = (ca<start || ca >end);
			seg->bVisible = TRUE;
			if (!bVisible && TPref::TUniv.bShowHidden)
			{
				seg->bVisible = FALSE;
			}
			else if (isInside)
			{
				//oldP = pDC->SelectObject(&curPen);
				double h,s,l;
				CBCGPDrawManager::RGBtoHSL(pObjectShape.clrObject,&h,&s,&l);
				l = l+3*l/4;
				COLORREF ff = CBCGPDrawManager::HLStoRGB_ONE(h,220./255.,s);
				seg->pObjectShape.clrObject = ff;
				//seg->pObjectShape.nShapeId = 6;
			}
			else
			{
				//oldP = pDC->SelectObject(&disPen);
				seg->pObjectShape.clrObject = pObjectShape.clrObject;
				seg->pObjectShape.nShapeId = pObjectShape.nShapeId;
			}
			//pDC->SelectObject(oldP);
			int nb = 0;
		}
		seg->Draw(pDC,vp,bSm);
	}
}


void CCone3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;

	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {255/255.f, 255/255.f,100/255.f , 1.0f};
    float mat_diffuse[] = {0.9f, 0.1f, 0.1f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	GLdouble base = (nRadius/TPref::TUniv.nUnitRep/3);
	GLdouble height = (nHeight/TPref::TUniv.nUnitRep/3);

	GLdouble tx = (C1->Center.x/TPref::TUniv.nUnitRep/3);
	GLdouble ty = (C1->Center.y/TPref::TUniv.nUnitRep/3);
	GLdouble tz = (C1->Center.z/TPref::TUniv.nUnitRep/3);

	GLdouble px = -((pApex->Concept_pt.x-C1->Center.x)/TPref::TUniv.nUnitRep/3);
	GLdouble py = -((pApex->Concept_pt.y-C1->Center.y)/TPref::TUniv.nUnitRep/3);
	GLdouble pz = -((pApex->Concept_pt.z-C1->Center.z)/TPref::TUniv.nUnitRep/3);

	CVector4 dl = LocRep.K;
	CVector4 dz(0,0,1);
	CVector4 drot = dz % dl;
	double dd = 0;
	drot = drot.Normalized();
	drot.Norme();
	if (drot.N!=0)
	{
		FCoord cosangle = dz * dl;
		dd = acos(cosangle);
		dd = -(180-RTD(dd));
	}
	else
	{
		drot = CVector4(1,0,0);
	}

	glPushMatrix();
	glTranslated(tx, ty, tz);
	glRotated(dd,drot.x,drot.y,drot.z);
//	glColor3f(.0f,.0f,.50f);
	COLORREF clr = pObjectShape.GetObjectColor();
	glColor3f(GetRValue(clr)/255.f,GetGValue(clr)/255.f,GetBValue(clr)/255.f);
	if (nVolMode==RENDER_SILHOUETTE)
		glutWireCone(base,height,32,16);
	else if (nVolMode==RENDER_FILL || nVolMode==RENDER_STIPPLE)
	{
		if (nVolMode==RENDER_STIPPLE)
		{
			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(stippleMask[8]);
		}
		glutSolidCone(base,height,32,16);
		if (nVolMode==RENDER_STIPPLE)
			glDisable(GL_POLYGON_STIPPLE);
	}
	glPopMatrix();
	if (TPref::TCone.bDoubleCone)
	{
		glPushMatrix();
		glTranslated(tx, ty, tz);
		glRotated(180+dd,drot.x,drot.y,drot.z);
		glTranslated(0, 0,- 2*nHeight/TPref::TUniv.nUnitRep/3);
//		glColor3f(.0f,.50f,.50f);
		glColor3f(GetRValue(clr)/255.f,GetGValue(clr)/255.f,GetBValue(clr)/255.f);
		if (nVolMode==RENDER_SILHOUETTE)
			glutWireCone(base,height,32,16);
		else if (nVolMode==RENDER_FILL || nVolMode==RENDER_STIPPLE)
		{
			if (nVolMode==RENDER_STIPPLE)
			{
				glEnable(GL_POLYGON_STIPPLE);
				glPolygonStipple(stippleMask[8]);
			}
			glutSolidCone(base,height,32,16);
			if (nVolMode==RENDER_STIPPLE)
				glDisable(GL_POLYGON_STIPPLE);
		}
		glPopMatrix();
	}

}

void CCone3D::DrawRetro(CDC*,CVisualParam *vp)
{
}

BOOL CCone3D::IsInActiveArea(CPoint thePt)
{
    BOOL bRet = FALSE;
	if (C1 && C1->IsInActiveArea(thePt)) return TRUE;
	if (TPref::TCone.bDoubleCone && C3 && C3->IsInActiveArea(thePt)) return TRUE;

    return bRet;
}


UINT CCone3D::IntersectPlan()
{
	return 0;
}	

CVector4 CCone3D::GetConeApex() 
{ 
	return pApex->Concept_pt;
}

CVector4 CCone3D::GetConeAxis() 
{ 
	return (pAxis->Concept_pt - pApex->Concept_pt).Normalized();
}


UINT CCone3D::IntersectLine(CDroite3D *dr,CVector4 &in,CVector4 &out)
{
	if (!dr) return 0;
	UINT m_iIntersectionType =0;
	int m_iQuantity = 0;

	CVector4 m_akPoint[2];

    // information about the intersection set
    enum
    {
        IT_EMPTY =0,
        IT_POINT ,
        IT_SEGMENT,
        IT_RAY ,
        IT_LINE ,
        IT_POLYGON,
        IT_PLANE,
        IT_POLYHEDRON,
        IT_OTHER
    };
    CVector4 coneAxis = pAxis->Concept_pt - pApex->Concept_pt;
	coneAxis = coneAxis.Normalized();
	coneAxis.Norme();
	CVector4 dir = dr->GetDirVector();
	dir = dir.Normalized();
	dir.Norme();

    // Set up the quadratic Q(t) = c2*t^2 + 2*c1*t + c0 that corresponds to
    // the cone.  Let the vertex be V, the unit-length direction vector be A,
    // and the angle measured from the cone axis to the cone wall be Theta,
    // and define g = cos(Theta).  A point X is on the cone wall whenever
    // Dot(A,(X-V)/|X-V|) = g.  Square this equation and factor to obtain
    //   (X-V)^T * (A*A^T - g^2*I) * (X-V) = 0
    // where the superscript T denotes the transpose operator.  This defines
    // a double-sided cone.  The line is L(t) = P + t*D, where P is the line
    // origin and D is a unit-length direction vector.  Substituting
    // X = L(t) into the cone equation above leads to Q(t) = 0.  Since we
    // want only intersection points on the single-sided cone that lives in
    // the half-space pointed to by A, any point L(t) generated by a root of
    // Q(t) = 0 must be tested for Dot(A,L(t)-V) >= 0.
	// Check the axis
    FCoord fAdD = coneAxis * dir;
    FCoord fCosSqr = cos(nOpenAngle)*cos(nOpenAngle);
    CVector4 kE = dr->GetBasePoint() - pApex->Concept_pt;
	//kE = kE.Normalized();
	//kE.Norme();
	FCoord fAdE = coneAxis * kE;
    FCoord fDdE = dir * kE;
    FCoord fEdE = kE * kE;
    FCoord fC2 = fAdD*fAdD - fCosSqr;
    FCoord fC1 = fAdD*fAdE - fCosSqr*fDdE;
    FCoord fC0 = fAdE*fAdE - fCosSqr*fEdE;
    FCoord fDot;

    // Solve the quadratic.  Keep only those X for which Dot(A,X-V) >= 0.
	if (!FCZero(fC2))
    //if (Math<Real>::FAbs(fC2) >= Math<Real>::ZERO_TOLERANCE)
    {
        // c2 != 0
        FCoord fDiscr = fC1*fC1 - fC0*fC2;
        if (fDiscr < (FCoord)0.0)
        {
            // Q(t) = 0 has no real-valued roots.  The line does not
            // intersect the double-sided cone.
            m_iIntersectionType = IT_EMPTY;
            m_iQuantity = 0;
        }
		//else if (fDiscr > Math<Real>::ZERO_TOLERANCE)
		else if (fDiscr > M_ZERO)
        {
            // Q(t) = 0 has two distinct real-valued roots.  However, one or
            // both of them might intersect the portion of the double-sided
            // cone "behind" the vertex.  We are interested only in those
            // intersections "in front" of the vertex.
            FCoord fRoot = sqrt(fDiscr);
            FCoord fInvC2 = ((FCoord)1.0)/fC2;
            m_iQuantity = 0;

            FCoord fT = (-fC1 - fRoot)*fInvC2;
            m_akPoint[m_iQuantity] = CVector4(dr->GetBasePoint() + dir*fT);
            kE = m_akPoint[m_iQuantity] - pApex->Concept_pt;
            fDot = kE * coneAxis;
            if (fDot > (FCoord)0.0)
            {
                m_iQuantity++;
            }

            fT = (-fC1 + fRoot)*fInvC2;
            m_akPoint[m_iQuantity] = dr->GetBasePoint() + dir*fT;
            kE = m_akPoint[m_iQuantity] - pApex->Concept_pt;
            fDot = kE*coneAxis;
            if (fDot > (FCoord)0.0)
            {
                m_iQuantity++;
            }

            if (m_iQuantity == 2)
            {
                // The line intersects the single-sided cone in front of the
                // vertex twice.
                m_iIntersectionType = IT_SEGMENT;
            }
            else if (m_iQuantity == 1)
            {
                // The line intersects the single-sided cone in front of the
                // vertex once.  The other intersection is with the
                // single-sided cone behind the vertex.
                m_iIntersectionType = IT_RAY;
                m_akPoint[m_iQuantity++] = dir;
            }
            else
            {
                // The line intersects the single-sided cone behind the vertex
                // twice.
                m_iIntersectionType = IT_EMPTY;
            }
        }
        else
        {
            // one repeated real root (line is tangent to the cone)
            m_akPoint[0] = dr->GetBasePoint() - dir*(fC1/fC2);
            kE = m_akPoint[0] - pApex->Concept_pt;
            if (kE * coneAxis > (FCoord)0.0)
            {
                m_iIntersectionType = IT_POINT;
                m_iQuantity = 1;
            }
            else
            {
                m_iIntersectionType = IT_EMPTY;
                m_iQuantity = 0;
            }
        }
    }
    //else if (Math<Real>::FAbs(fC1) >= Math<Real>::ZERO_TOLERANCE)
	else if (!FCZero(fC1))
    {
        // c2 = 0, c1 != 0 (D is a direction vector on the cone boundary)
        m_akPoint[0] = dr->GetBasePoint() - dir*(((FCoord)0.5)*fC0/fC1);
        kE = m_akPoint[0] - pApex->Concept_pt;
        fDot = kE * coneAxis;
        if (fDot > (FCoord)0.0)
        {
            m_iIntersectionType = IT_RAY;
            m_iQuantity = 2;
            m_akPoint[1] = dir;
        }
        else
        {
            m_iIntersectionType = IT_EMPTY;
            m_iQuantity = 0;
        }
    }
    //else if (Math<Real>::FAbs(fC0) >= Math<Real>::ZERO_TOLERANCE)
	else if (!FCZero(fC0))
    {
        // c2 = c1 = 0, c0 != 0
        m_iIntersectionType = IT_EMPTY;
        m_iQuantity = 0;
    }
    else
    {
        // c2 = c1 = c0 = 0, cone contains ray V+t*D where V is cone vertex
        // and D is the line direction.
        m_iIntersectionType = IT_RAY;
        m_iQuantity = 2;
        m_akPoint[0] = pApex->Concept_pt;
        m_akPoint[1] = dir;
    }

	in =  m_akPoint[0];
	out =  m_akPoint[1];
    return m_iIntersectionType;
}


//***************************************************************************
// CInterConeDr3D
//***************************************************************************
IMPLEMENT_SERIAL(CInterConeDr3D, CCompositeObj3D, VERSIONABLE_SCHEMA | 1)


CInterConeDr3D::CInterConeDr3D() : CCompositeObj3D()
{
    Cone = NULL;
    Dr = NULL;
    ptA = ptB = NULL;
}

CInterConeDr3D::CInterConeDr3D(CCone3D* sp1,CDroite3D* dr2) : CCompositeObj3D()
{
    Cone = sp1;
    Dr = dr2;
    ptA = new CPointCalc3D(this);
    ptB = new CPointCalc3D(this);

    SetDepth();
    ptA->SetInGraph();
    ptB->SetInGraph();

    m_cSubObjects.Add(ptA);
    m_cSubObjects.Add(ptB);
}

CInterConeDr3D::CInterConeDr3D(const CObject3D & obj) : CCompositeObj3D(obj)
{
    Cone = ((CInterConeDr3D &)obj).Cone;
    Dr = ((CInterConeDr3D &)obj).Dr;
    ptA = new CPointCalc3D(this);
    ptB = new CPointCalc3D(this);

    SetDepth();
    ptA->SetInGraph();
    ptB->SetInGraph();

    m_cSubObjects.Add(ptA);
    m_cSubObjects.Add(ptB);
}

int CInterConeDr3D::SetDepth()
{
    if (Cone && Dr)
    {
        nDepth = max(Cone->nDepth,Dr->nDepth)+1;
        if (ptA) ptA->nDepth = nDepth+1;
        if (ptB) ptB->nDepth = nDepth+1;
    }
    return nDepth;
}

CObject3D* CInterConeDr3D::CopyObject()
{
    CObject3D *temp = new CInterConeDr3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CInterConeDr3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(Cone);
    list->Add(Dr);
    return list;
}

void CInterConeDr3D::Serialize( CArchive& ar )
{
    CCompositeObj3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((Cone) ? Cone->nObjectId : -1);
        ar << ((Dr) ? Dr->nObjectId : -1);
    }
    else
    {
        Cone = (CCone3D*)SerializeObj(ar);
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
    }
}

CString CInterConeDr3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (Cone) sn1 = Cone->GetObjectHelp();
    if (Dr) sn2 = Dr->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}

UINT  CInterConeDr3D::CalculConceptuel()
{
    bValidate = ((Cone->bValidate) && (Dr->bValidate));
    if (!bValidate)
        return ValideCompositeObject(ERR_NOINTER);
	
	CVector4 in,out;
	UINT res = Cone->IntersectLine(Dr,in,out);
	if (res==0)
	{
        bValidate=0;
        return ValideCompositeObject(ERR_INTER_CONEDR);
	}
	ptA->bValidate = TRUE;
	ptA->Concept_pt = in;
	if (res==1)
	{
		ptB->bValidate = FALSE;
	}
	else
	{
		ptB->Concept_pt = out;
		ptB->bValidate = TRUE;
	}
	CCompositeObj3D::CalculConceptuel();
	
	bValidate=TRUE;
    return 0;
}

CString CInterConeDr3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate && */Cone && Dr)
    {
        CString strName1,strName2,strObj1,strObj2;
		strName1 = ptA->GetObjectNameRedux();
		strName2 = ptB->GetObjectNameRedux();
		strObj1 = Cone->GetObjectNameRedux();
		strObj2 = Dr->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
		{
	        mstr.Format(_T("Intersection1LineCone[%s,%s,%s];\nIntersection2LineCone[%s,%s,%s];"),
					strName1,strObj2,strObj1,strName2,strObj2,strObj1);
		}
		else if (nFormat==EXPORT_MAPLE)
		{
			mstr.Format(_T("Intersection1LineCone(%s,%s,%s);\nIntersection2LineCone(%s,%s,%s);"),
					strName1,strObj2,strObj1,strName2,strObj2,strObj1);
		}

    }
    return mstr;
}