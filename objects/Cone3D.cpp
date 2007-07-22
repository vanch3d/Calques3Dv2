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
// Cone3D.cpp: implementation of the CCone3D class.
//
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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCone3D, CVolumeObject3D, VERSIONABLE_SCHEMA | 1)


CCone3D::CCone3D() : CVolumeObject3D()
{
	pObjectShape.clrObject = TPref::TCone.clrDefault;
	pApex = pAxis = pRadius = NULL;
	C1 = C2 = C3 = NULL;
	Base1 = Base2 = Base3 = NULL;
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
}

CCone3D::CCone3D(const CObject3D & src) : CVolumeObject3D(src)
{
    pApex =  ((CCone3D&)src).pApex;
    pAxis =   ((CCone3D&)src).pAxis;
    pRadius =   ((CCone3D&)src).pRadius;
 	SetDepth();
	C1 = C2 = C3 = NULL;
	Base1 = Base2 = Base3 = NULL;
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
    LocRep.I = LocRep.K % LocRep.J;

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

void CCone3D::CalculVisuel(CVisualParam *pVisParam)
{
	if (C1) C1->CalculVisuel(pVisParam);
	if (C2) C2->CalculVisuel(pVisParam);
	if (C3) C3->CalculVisuel(pVisParam);
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
    CLocalRep IntH = CLocalRep(pApex->Concept_pt,I,J,K);

    int nbPts = cPointsList.GetSize();
    int nbSeg = cGenerList.GetSize();
    CVector4 U,Center;
	int nDeltaT = 32;
    for (int t=0;t<nDeltaT;t++)
    {
        CVector4 V1 = calculGeneratrice(2*M_PI*t/nDeltaT,C1->Center,nRadius,I,J,K);
        CVector4 V2;
		if (TPref::TCone.bDoubleCone)
			V2 = calculGeneratrice(M_PI+2*M_PI*t/nDeltaT,C3->Center,nRadius,I,J,K);
		else
			V2 = pApex->Concept_pt;

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
		FCoord sa = IntH.I * (seg->P1->Concept_pt - C1->Center);
        FCoord ca = IntH.J * (seg->P1->Concept_pt - C1->Center);
		if (sa<0 && sa<0)
		{ 
			double h,s,l;
			CBCGPDrawManager::RGBtoHSL(pObjectShape.clrObject,&h,&s,&l);
			l = l+3*l/4;
			COLORREF ff = CBCGPDrawManager::HLStoRGB_ONE(h,220./255.,s);
			seg->pObjectShape.clrObject = ff;

		}
		else
            seg->pObjectShape.clrObject = pObjectShape.clrObject;

     }
}

void CCone3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSm)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(vp->nCalqueNum))) return;
	if (C1) C1->Draw(pDC,vp,bSm);
	if (C2) C2->Draw(pDC,vp,bSm);
	if (C3 && TPref::TCone.bDoubleCone) C3->Draw(pDC,vp,bSm);

	pDC->MoveTo(vp->ProjectPoint(C1->Center));
	pDC->LineTo(vp->ProjectPoint(C1->Center+LocRep.I*50.));
	pDC->MoveTo(vp->ProjectPoint(C1->Center));
	pDC->LineTo(vp->ProjectPoint(C1->Center+LocRep.J*100.));
	pDC->MoveTo(vp->ProjectPoint(C1->Center));
	pDC->LineTo(vp->ProjectPoint(C1->Center+LocRep.K*150.));

	    for (int t=0;t<cGenerList.GetSize();t++)
    {
        CSegment3D *seg = (CSegment3D*)cGenerList[t];
		if (!seg) continue;

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

	glPushMatrix();
	//glTranslated(bx, by, bz);
	//glRotated(dd,drot.x,drot.y,drot.z);
	glColor3f(.0f,.0f,1.0f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	glutSolidCone(base,height,16,64);
	glPopMatrix();

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
