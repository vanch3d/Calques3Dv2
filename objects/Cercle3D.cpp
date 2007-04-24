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
// Cercle3D.cpp: implementation of the CCercle3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Cercle3D.h"
#include "Droite3D.h"
#include "Plan3D.h"
#include "Cylinder3D.h"
#include "Sphere3D.h"

#include "Point3D.h"
#include "../OGLTools/glut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern CRgn* DoSegRgn(CPoint p1,CPoint p2);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCercle3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CCercle3D::CCercle3D() : CObject3D()
{
    P1 = P2 = P3 = 0;
    bCircCen = FALSE;
    nArcAngle = 2*M_PI;
    Radius = 0.;
    VecNorm = CVector4(0,0,0);
    nDeltaT = TPref::TCercle.nDelta;
    m_cConcept_pts.RemoveAll();
    m_cVisual_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    m_cVisual_pts.SetSize(nDeltaT);
}

CCercle3D::CCercle3D(CPoint3D *s1,CPoint3D *s2,CPoint3D *s3,bool bypt) : CObject3D()
{
    P1 = s1;
    P2 = s2;
    P3 = s3;
    bCircCen = bypt;
    nArcAngle = 2*M_PI;

    pObjectShape.clrObject = RGB(0,0,255);
    nDepth = max(s1->nDepth,max(s2->nDepth,s3->nDepth))+1;
    nDeltaT = TPref::TCercle.nDelta;
    m_cConcept_pts.RemoveAll();
    m_cVisual_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    m_cVisual_pts.SetSize(nDeltaT);
}

CCercle3D::CCercle3D(CPoint3D *p1,CVector4 Norm,FCoord ray) : CObject3D()
{
    P1 = p1;
    P2 = P3 = 0;
    bCircCen = TRUE;
    nArcAngle = 2*M_PI;
    Radius = ray;
    VecNorm = Norm;
    nDepth = p1->nDepth+1;
    nDeltaT = TPref::TCercle.nDelta;
    m_cConcept_pts.RemoveAll();
    m_cVisual_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    m_cVisual_pts.SetSize(nDeltaT);
}

CCercle3D::CCercle3D(const CObject3D &src ) : CObject3D(src)
{
    P1 = ((CCercle3D&)src).P1;
    P2 = ((CCercle3D&)src).P2;
    P3 = ((CCercle3D&)src).P3;
    bCircCen = ((CCercle3D&)src).bCircCen;
    nArcAngle = ((CCercle3D&)src).nArcAngle ;
    Radius = ((CCercle3D&)src).Radius ;
    VecNorm = ((CCercle3D&)src).VecNorm ;
    nDeltaT = ((CCercle3D&)src).nDeltaT;
    m_cConcept_pts.RemoveAll();
    m_cVisual_pts.RemoveAll();
    for (int i=0;i<nDeltaT;i++)
    {
        CVector4 mPt4= ((CCercle3D&)src).m_cConcept_pts.GetAt(i);
        CPoint mPt = ((CCercle3D&)src).m_cVisual_pts.GetAt(i);
        m_cConcept_pts.Add(mPt4);
        m_cVisual_pts.Add(mPt);
    }
}

int CCercle3D::SetDepth()
{
    if (P1 && P2 && P3)
        nDepth = max(P1->nDepth,max(P2->nDepth,P3->nDepth))+1;
    else if (P1)
        nDepth = P1->nDepth+1;
    return nDepth;
}


BOOL CCercle3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
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

void CCercle3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << bCircCen;
        ar << VecNorm;
        ar << Center;
        ar << Radius;
        ar << nArcAngle;
        ar << nDeltaT;
        ar << ((P1) ? P1->nObjectId : -1);
        ar << ((P2) ? P2->nObjectId : -1);
        ar << ((P3) ? P3->nObjectId : -1);
    }
    else
    {
        ar >> bCircCen;
        ar >> VecNorm;
        ar >> Center;
        ar >> Radius;
        ar >> nArcAngle;
        ar >> nDeltaT;
        P1 = (CPoint3D*)SerializeObj(ar);
        P2 = (CPoint3D*)SerializeObj(ar);
        P3 = (CPoint3D*)SerializeObj(ar);
    }
}



CCercle3D::~CCercle3D()
{
    m_cConcept_pts.RemoveAll();
    m_cVisual_pts.RemoveAll();
}

CObject3D* CCercle3D::CopyObject()
{
    CObject3D *temp = new CCercle3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CCercle3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(P1);
    if (P2) list->Add(P2);
    if (P3) list->Add(P3);
    return list;
}

CString CCercle3D::GetObjectDef()
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

COLORREF CCercle3D::GetDefaultColor()
{
    return TPref::TCercle.clrDefault;
}


BOOL CCercle3D::IsEqual(CObject3D &o)
{
    if (!(o.MaskObject(TAllCercleClass))) return false;
    if (!bValidate || !(o.bValidate)) return false;
    CVector4    dir1 = VecNorm,
                dir2 = ((CCercle3D&)o).VecNorm;
    BOOL        SameVecNorm = CVector4(dir1 % dir2).NullVector();
    BOOL        SameCenter  = CVector4(((CCercle3D&)o).Center - Center).NullVector();
    BOOL        SameRadius  = FCZero(Radius - ((CCercle3D&)o).Radius);
    return (SameVecNorm && SameCenter &&  SameRadius);
}

BOOL CCercle3D::IsInActiveArea(CPoint thePt)
{
    bool bIsInSeg = FALSE;
    for (int t=0;((t<nDeltaT) && (!bIsInSeg));t++)
     {
        CPoint  p1 = m_cVisual_pts.GetAt(t),
                p2;
        if (isA() ==  TArcCercle3DClass)
         {
            if (t==(nDeltaT-1))
                p2 = P3->Visual_pt;
            else
                p2 = m_cVisual_pts.GetAt(t+1);
         }
        else
            p2 = m_cVisual_pts.GetAt((t==(nDeltaT-1)) ?0 : t+1);

        CRgn *pRgn = DoSegRgn(p1,p2);
        bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
        delete  pRgn;
     }
    return bIsInSeg;
}

CRgn* CCercle3D::InvalideRect()
{
    return NULL;
}

UINT  CCercle3D::CalculConceptuel()
{
    if (!P2 && !P3)
    {
        bValidate = P1->bValidate;
        if (!bValidate)
            return ERR_NOCERCLE;
        Center = P1->Concept_pt;
        if (VecNorm.NullVector())
         {
            bValidate = FALSE;
            return ERR_PTSALIGN;
         }
    }
    else
    {
        bValidate = ((P1->bValidate) && (P2->bValidate) && (P3->bValidate));
        if (!bValidate)
            return ERR_NOCERCLE;

        CVector4    U = P2->Concept_pt - P1->Concept_pt,
                    V = P3->Concept_pt - P1->Concept_pt,
                    UvV = (U % V);

        VecNorm =UvV;
        if (UvV.NullVector())
        {
            bValidate = FALSE;
            return ERR_PTSALIGN;
        }
        if (!bCircCen)
         {
            // The center in barycentric coordinates is C = u1*P1+u2*P2+u3*P3
            // where u1+u2+u3=1, 0 < u1 < 1, 0 < u2 < 1, and 0 < u3 < 1.  The
            // center is equidistant from the three points, so
            // |C-P1| = |C-P2| = |C-P3| = R where R is the radius of the circle.
            //
            // From these conditions,
            //   C-P1 = u1*A + u2*B - A
            //   C-P2 = u1*A + u2*B - B
            //   C-P3 = u1*A + u2*B
            // where A = P1-P3 and B = P2-P3, which leads to
            //   r^2 = |u1*A+u2*B|^2 - 2*Dot(A,u1*A+u2*B) + |A|^2
            //   r^2 = |u1*A+u2*B|^2 - 2*Dot(B,u1*A+u2*B) + |B|^2
            //   r^2 = |u1*A+u2*B|^2
            // Subtracting the last equation from the first two and writing
            // the equations as a linear system,
            //
            // +-                 -++   -+       +-        -+
            // | Dot(A,A) Dot(A,B) || u1 | = 0.5 | Dot(A,A) |
            // | Dot(A,B) Dot(B,B) || u2 |       | Dot(B,B) |
            // +-                 -++   -+       +-        -+
            //
            // The following code solves this system for u1 and u2, then
            // evaluates the third equation in r^2 to obtain r.

            CVector4 p1 = P1->Concept_pt,
                     p2 = P2->Concept_pt,
                     p3 = P3->Concept_pt;
            CVector4 A(p1.x-p3.x, p1.y-p3.y, p1.z-p3.z);
            CVector4 B(p2.x-p3.x, p2.y-p3.y, p2.z-p3.z);
            FCoord AA = A.x*A.x+A.y*A.y+A.z*A.z;
            FCoord AB = A.x*B.x+A.y*B.y+A.z*B.z;
            FCoord BB = B.x*B.x+B.y*B.y+B.z*B.z;
            FCoord halfInvDet = .5/(AA*BB-AB*AB);
            FCoord u1 = halfInvDet*BB*(AA-AB);
            FCoord u2 = halfInvDet*AA*(BB-AB);
            FCoord u3 = 1.-u1-u2;
            CVector4 tmp(u1*A.x+u2*B.x, u1*A.y+u2*B.y, u1*A.z+u2*B.z);
            Radius = sqrtl(tmp.x*tmp.x+tmp.y*tmp.y+tmp.z*tmp.z);
            Center.x = u1*p1.x+u2*p2.x+u3*p3.x;
            Center.y = u1*p1.y+u2*p2.y+u3*p3.y;
            Center.z = u1*p1.z+u2*p2.z+u3*p3.z;
         }
        else
        {
            Radius = U.Norme();
            Center = P1->Concept_pt;
        }
     }

    CVector4 I,J,K;
    FCoord normK = 1 / VecNorm.Norme();
    K = VecNorm * normK;
    if (P2 && P3)
    {
        I = ((!bCircCen) ? P1->Concept_pt : P2->Concept_pt) - Center;
        normK = I.Norme();
        I = I * (1/normK);
        J = (K % I);
    }
    else
    {
        J = CVector4(-K.y,K.x,0,1);
        normK = J.Norme();
        if (FCZero(normK))
            { J.x = J.z =0; J.y = 1;    }
        else
            J = J * (1/normK);
        I = (J % K);
    }
    LocRep = CLocalRep(Center,I,J,K);

    m_cConcept_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    for (int t=0;t<nDeltaT;t++)
    {
        CVector4 U,V;
        U.x=    Radius*cos(nArcAngle*t/nDeltaT);
        U.y=    Radius*sin(nArcAngle*t/nDeltaT);
        U.z=    0;
        U.w=    1;

        V.x=    I.x*U.x + J.x*U.y + K.x*U.z + Center.x,
        V.y=    I.y*U.x + J.y*U.y + K.y*U.z + Center.y,
        V.z=    I.z*U.x + J.z*U.y + K.z*U.z + Center.z,
        V.w=    1;
        m_cConcept_pts.SetAt(t,V);
    }
    bValidate=TRUE;
    return 0;
}

void CCercle3D::CalculVisuel(CVisualParam *pVisParam)
{
    m_cVisual_pts.RemoveAll();
    m_cVisual_pts.SetSize(nDeltaT);

    for (int t=0;t<nDeltaT;t++)
     {
        CVector4 V = m_cConcept_pts.GetAt(t);
        CPoint pt = (CPoint)pVisParam->ProjectPoint(V);
        m_cVisual_pts.SetAt(t,pt);
     }
}

CString CCercle3D::ExportSymbolic(int nFormat)
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
	        mstr.Format(_T("CircleD[%s,%s,%s,%s];"),strName,strObj1,strObj2,strObj3);
		else if (nFormat==EXPORT_MAPLE)
	        mstr.Format(_T("CircleD(%s,%s,%s,%s);"),strName,strObj1,strObj2,strObj3);

    }
    return mstr;
}


void CCercle3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSM)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

    CPen curPen,disPen;
    curPen.CreatePenIndirect(&(pObjectShape.GetPenStyle()));

    disPen.CreatePenIndirect(&(pObjectShape.GetHiddenPenStyle(
                pObjectShape.GetObjectHiddenColor())));

    bool bVis1;

    CBrush curBrush(pObjectShape.clrObject);
    CBrush *pOldB = pDC->SelectObject(&curBrush);
    //  HandleDC.SelectObject(curPen);

    for (int t=0;t<nDeltaT;t++)
     {
        CPen *pOldP = NULL;
        if (!bSM)
         {
            CVector4    U = m_cConcept_pts.GetAt(t),
                        V;
            if (isA() ==  TArcCercle3DClass)
            {
                if (t==(nDeltaT-1))
                    V = P3->Concept_pt;
                else
                    V = m_cConcept_pts.GetAt(t+1);
            }
            else
                V = m_cConcept_pts.GetAt((t==(nDeltaT-1)) ?0 : t+1);
            bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));
            pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
         }
        else
            pOldP = pDC->SelectObject(&curPen);

        CPoint start = m_cVisual_pts[t];
        CPoint endpt;
        if (isA() ==  TArcCercle3DClass)
         {
            if (t==(nDeltaT-1))
                endpt = P3->Visual_pt;
            else
                endpt = m_cVisual_pts[t+1];
         }
        else
            endpt = m_cVisual_pts[(t==(nDeltaT-1)) ?0 : t+1];
        pDC->MoveTo(start);
        pDC->LineTo(endpt);
        if (pOldP) pDC->SelectObject(pOldP);
     }
    if (pOldB) pDC->SelectObject(pOldB);
    if (bIsSelected)
     {
        int         Ds = TPref::TPoint.nSize;

        CPoint tp1 = m_cVisual_pts[0];
        CPoint tp2 = m_cVisual_pts[nDeltaT/2];
        CPoint tp3 = m_cVisual_pts[nDeltaT/4];
        CPoint tp4 = m_cVisual_pts[3*nDeltaT/4];
        CRect theR1(tp1.x-Ds,tp1.y-Ds,tp1.x+Ds,tp1.y+Ds);
        CRect theR2(tp2.x-Ds,tp2.y-Ds,tp2.x+Ds,tp2.y+Ds);
        CRect theR3(tp3.x-Ds,tp3.y-Ds,tp3.x+Ds,tp3.y+Ds);
        CRect theR4(tp4.x-Ds,tp4.y-Ds,tp4.x+Ds,tp4.y+Ds);
        pDC->InvertRect(theR1);
        pDC->InvertRect(theR2);
        pDC->InvertRect(theR3);
        pDC->InvertRect(theR4);
     }

}

void CCercle3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;

	CVector4 base = Center;
	GLdouble bx = (base.x/TPref::TUniv.nUnitRep/3);
	GLdouble by = (base.y/TPref::TUniv.nUnitRep/3);
	GLdouble bz = (base.z/TPref::TUniv.nUnitRep/3);
	CVector4 dl = VecNorm;
	dl = dl.Normalized();
	dl.Norme();
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
		drot = dz;
	}
	FCoord rad = Radius/TPref::TUniv.nUnitRep/3;
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
//	glRotated(90.,dl.x,dl.y,dl.z);
	//glColor3f(.8f,.5f,.8f);
	glColor3f(.0f,.0f,1.0f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	glutSolidTorus (0.02,rad,16,64);
	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CArcCercle3D, CCercle3D, VERSIONABLE_SCHEMA | 1)

CArcCercle3D::CArcCercle3D() : CCercle3D()
{
}

CArcCercle3D::CArcCercle3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *p3) :
    CCercle3D(p1,p2,p3)
{
}

CArcCercle3D::CArcCercle3D(const CObject3D &src) : CCercle3D(src)
{}

CObject3D* CArcCercle3D::CopyObject()
{
    CArcCercle3D *temp = new CArcCercle3D((CObject3D&)*this);
    return temp;
}

BOOL CArcCercle3D::IsEqual(CObject3D &other)
{
    return CCercle3D::IsEqual(other);
}

UINT  CArcCercle3D::CalculConceptuel()
{
    bValidate = ((P1->bValidate) && (P2->bValidate) && (P3->bValidate));
    if (!bValidate)
        return ERR_NOCERCLE;

    CVector4    U = P2->Concept_pt - P1->Concept_pt,
                V = P3->Concept_pt - P1->Concept_pt,
                UvV = U % V;

    VecNorm =UvV;
    if (UvV.NullVector())
    {
        bValidate = FALSE;
        return ERR_PTSALIGN;
     }
    CVector4 p1 = P1->Concept_pt;
    CVector4 p2 = P2->Concept_pt;
    CVector4 p3 = P3->Concept_pt;
    CVector4 A(p1.x-p3.x, p1.y-p3.y, p1.z-p3.z);
    CVector4 B(p2.x-p3.x, p2.y-p3.y, p2.z-p3.z);
    FCoord AA = A.x*A.x+A.y*A.y+A.z*A.z;
    FCoord AB = A.x*B.x+A.y*B.y+A.z*B.z;
    FCoord BB = B.x*B.x+B.y*B.y+B.z*B.z;
    FCoord halfInvDet = .5/(AA*BB-AB*AB);
    FCoord u1 = halfInvDet*BB*(AA-AB);
    FCoord u2 = halfInvDet*AA*(BB-AB);
    FCoord u3 = 1.-u1-u2;
    CVector4 tmp(u1*A.x+u2*B.x, u1*A.y+u2*B.y, u1*A.z+u2*B.z);
    Radius = sqrtl(tmp.x*tmp.x+tmp.y*tmp.y+tmp.z*tmp.z);
    Center.x = u1*p1.x+u2*p2.x+u3*p3.x;
    Center.y = u1*p1.y+u2*p2.y+u3*p3.y;
    Center.z = u1*p1.z+u2*p2.z+u3*p3.z;

    FCoord normK = 1 / UvV.Norme();
    CVector4    K = UvV * normK;
    CVector4    I = P1->Concept_pt - Center;
    normK = I.Norme();
    I = I * (1/normK);
    CVector4    J = (K % I);

    CVector4    Ip = P3->Concept_pt - Center;
    normK = Ip.Norme();
    Ip = Ip * (1/normK);
    FCoord tha = (I * Ip);
    FCoord thi = (J * Ip);
    if (fabsl(tha) > 1.)
        tha = (tha < 0.00) ? -1.00 : 1.00;
    nArcAngle = acosl(tha);
    if (thi < 0)
        nArcAngle = 2*M_PI-nArcAngle;

    return CCercle3D::CalculConceptuel();
}

CString CArcCercle3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (P1) sn1 = P1->GetObjectHelp();
    if (P2) sn2 = P2->GetObjectHelp();
    if (P3) sn3 = P3->GetObjectHelp();

    mstr.Format(sFormat,sName,sn1,sn3,sn2);
    return mstr;
}

CString CArcCercle3D::ExportSymbolic(int nFormat)
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
	        mstr.Format(_T("CircleArcD[%s,%s,%s,%s];"),strName,strObj1,strObj2,strObj3);
		else if (nFormat==EXPORT_MAPLE)
	        mstr.Format(_T("CircleArcD(%s,%s,%s,%s);"),strName,strObj1,strObj2,strObj3);
    }
    return mstr;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEllipse3D, CCercle3D, VERSIONABLE_SCHEMA | 1)

CEllipse3D::CEllipse3D(): CCercle3D()
{
    Cy=NULL;
    Pl=NULL;
}

CEllipse3D::CEllipse3D(CCylinder3D *p1,CPlan3D *p2): CCercle3D()
{
    Cy = p1;
    Pl = p2;
    SetDepth();
}

CEllipse3D::CEllipse3D(const CObject3D & src): CCercle3D(src)
{
    Cy = ((CEllipse3D&)src).Cy;
    Pl = ((CEllipse3D&)src).Pl;
}

int CEllipse3D::SetDepth()
{
    if (Cy && Pl)
        nDepth = max(Cy->nDepth,Pl->nDepth)+1;
    return nDepth;
}

CObject3D* CEllipse3D::CopyObject()
{
    CEllipse3D *temp = new CEllipse3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CEllipse3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (Cy) list->Add(Cy);
    if (Pl) list->Add(Pl);
    return list;
}

CString CEllipse3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (Cy) sn1 = Cy->GetObjectHelp();
    if (Pl) sn2 = Pl->GetObjectHelp();

    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}

BOOL CEllipse3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

UINT  CEllipse3D::CalculConceptuel()
{
    bValidate = ((Pl->bValidate) && (Cy->bValidate));
    if (!bValidate)
        return ERR_NOCERCLE;

    CSegment3D seg(Cy->P1,Cy->P2);
    seg.CalculConceptuel();
    CPointInterDP3D ptint(&seg,Pl);
    UINT ret = ptint.CalculConceptuel();
    if (ret)
     {
        bValidate = 0;
        return ERR_NOCERCLE;
     }

    LocRep.O = ptint.Concept_pt;
    CVector4 U = Pl->VecNorm;
    CVector4 V = seg.GetDirVector();
    FCoord normU = 1 / U.Norme();
    FCoord normV = 1 / V.Norme();
    U = U*normU;
    V = V*normV;
    LocRep.K = U;
    //IntH.J = V%U;
    LocRep.J = (V % U);
    FCoord normJ = LocRep.J.Norme();
    if (FCZero(normJ))
     {
        // { LocRep.J.x = LocRep.J.z =0; LocRep.J.y = 1;    }
        LocRep.J = CVector4(-U.y,U.x,0,1);
        normJ = LocRep.J.Norme();
        if (FCZero(normJ))
            LocRep.J = CVector4(0,1,0);
        else
            LocRep.J = LocRep.J * (1/normJ);
        LocRep.I = (LocRep.J % LocRep.K);
        FCoord normI = 1 / LocRep.I.Norme();
        LocRep.I = LocRep.I*normI;
     }
    else
     {
        LocRep.J = LocRep.J*(1/normJ);
        //IntH.I = U%IntH.J;
        LocRep.I = (U % LocRep.J);
        FCoord normI = 1 / LocRep.I.Norme();
        LocRep.I = LocRep.I*normI;
     }

    FCoord cosa = V*U;
    FCoord A = Cy->nRayon/cosa;
    FCoord B = Cy->nRayon;

    m_cConcept_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    for (int t=0;t<nDeltaT;t++)
     {

        FCoord temp = ((FCoord)(2*M_PI*t))/nDeltaT;
        CVector4 X(A*cosl(temp),B*sinl(temp),0),Y;
      //    U.x=    radius*cos(ArcAngle*t/deltaT);
      //    U.y=    radius*sin(ArcAngle*t/deltaT);
      //    U.z=    0;
      //    U.w=    1;

        Y.x=    LocRep.I.x*X.x + LocRep.J.x*X.y + LocRep.K.x*X.z + LocRep.O.x;
        Y.y=    LocRep.I.y*X.x + LocRep.J.y*X.y + LocRep.K.y*X.z + LocRep.O.y;
        Y.z=    LocRep.I.z*X.x + LocRep.J.z*X.y + LocRep.K.z*X.z + LocRep.O.z;
        m_cConcept_pts.SetAt(t,Y);
     }
    bValidate = 1;
    return 0;
}

void CEllipse3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;
}

CString CEllipse3D::ExportSymbolic(int nFormat)
{
	return CObject3D::ExportSymbolic(nFormat);
}


void CEllipse3D::Serialize( CArchive& ar )
{
    CCercle3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((Cy) ? Cy->nObjectId : -1);
        ar << ((Pl) ? Pl->nObjectId : -1);
    }
    else
    {
        Cy = (CCylinder3D*)SerializeObj(ar);
        Pl = (CPlan3D*)SerializeObj(ar);
    }

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCercleInterSS3D, CCercle3D, VERSIONABLE_SCHEMA | 1)

CCercleInterSS3D::CCercleInterSS3D() : CCercle3D()
{
}

CCercleInterSS3D::CCercleInterSS3D(CSphere3D *p1,CSphere3D *p2) : CCercle3D()
{
    SP1 = p1;
    SP2 = p2;
}

CCercleInterSS3D::CCercleInterSS3D(const CObject3D &src) : CCercle3D(src)
{
    SP1 = ((CCercleInterSS3D&)src).SP1;
    SP2 = ((CCercleInterSS3D&)src).SP2;
}

CObject3D* CCercleInterSS3D::CopyObject()
{
    CCercleInterSS3D *temp = new CCercleInterSS3D((CObject3D&)*this);
    return temp;
}

int CCercleInterSS3D::SetDepth()
{
    if (SP1 && SP2)
        nDepth = max(SP1->nDepth,SP2->nDepth)+1;
    return nDepth;
}

CxObject3DSet* CCercleInterSS3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (SP1) list->Add(SP1);
    if (SP2) list->Add(SP2);
    return list;
}

void CCercleInterSS3D::Serialize( CArchive& ar )
{
    CCercle3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((SP1) ? SP1->nObjectId : -1);
        ar << ((SP2) ? SP2->nObjectId : -1);
    }
    else
    {
        SP1 = (CSphere3D*)SerializeObj(ar);
        SP2 = (CSphere3D*)SerializeObj(ar);
    }
}



BOOL CCercleInterSS3D::IsEqual(CObject3D &other)
{
    return CCercle3D::IsEqual(other);
}

UINT  CCercleInterSS3D::CalculConceptuel()
{
    bValidate = ((SP1->bValidate) && (SP1->bValidate));
    if (!bValidate)
        return ERR_NOCERCLE;

    // plane of intersection must have N as its normal
    CVector4 kN = SP2->P1->Concept_pt - SP1->P1->Concept_pt;
    FCoord fNSqrLen = kN.Norme();
    fNSqrLen = fNSqrLen*fNSqrLen;

    FCoord fRSum = SP1->Rayon.Norme() + SP2->Rayon.Norme();
    if ( fNSqrLen > fRSum*fRSum )
    {
        // sphere centers are too far apart for intersection
        bValidate = 0;
        return ERR_INTER_SPHERESPHERE;
    }

    FCoord fR0Sqr = SP1->Rayon*SP1->Rayon;
    FCoord fR1Sqr = SP2->Rayon*SP2->Rayon;
    FCoord fInvNSqrLen = 1.0f/fNSqrLen;
    FCoord fT = 0.5f*(1.0f+(fR0Sqr-fR1Sqr)*fInvNSqrLen);
    //if ( fT < 0.0f || fT > 1.0f )
    //{
    //  bValidate = 0;
    //    return ERR_NOCERCLE;
    //}

    FCoord fRSqr = fR0Sqr - fT*fT*fNSqrLen;
    //if ( fRSqr < 0.0f )
    //{
    //  bValidate = 0;
    //  return ERR_NOCERCLE;
    //}

    // center and radius of circle of intersection
    Center = SP1->P1->Concept_pt + (kN*fT);
    Radius = sqrtl(fRSqr);

    // compute U and V for plane of circle
    VecNorm = kN;
    kN = kN * (FCoord)sqrtl(fInvNSqrLen);

    CVector4 I,J,K;
    FCoord normK = 1 / VecNorm.Norme();
    K = VecNorm * normK;

    J = CVector4(-K.y,K.x,0,1);
    normK = J.Norme();
    if (FCZero(normK))
        { J.x = J.z =0; J.y = 1;    }
    else
        J = J * (1/normK);
    I = (J % K);

    LocRep = CLocalRep(Center,I,J,K);

    m_cConcept_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    for (int t=0;t<nDeltaT;t++)
    {
        CVector4 U,V;
        U.x=    Radius*cos(nArcAngle*t/nDeltaT);
        U.y=    Radius*sin(nArcAngle*t/nDeltaT);
        U.z=    0;
        U.w=    1;

        V.x=    I.x*U.x + J.x*U.y + K.x*U.z + Center.x,
        V.y=    I.y*U.x + J.y*U.y + K.y*U.z + Center.y,
        V.z=    I.z*U.x + J.z*U.y + K.z*U.z + Center.z,
        V.w=    1;
        m_cConcept_pts.SetAt(t,V);
    }
    bValidate=TRUE;

    return 0;
}

CString CCercleInterSS3D::ExportSymbolic(int nFormat)
{
	return CObject3D::ExportSymbolic(nFormat);
}

CString CCercleInterSS3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (SP1) sn1 = SP1->GetObjectHelp();
    if (SP2) sn2 = SP2->GetObjectHelp();

    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}

/*CString CCercleInterSS3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (bValidate && P1 && P2 && P3)
    {
        CString strName,strObj1,strObj2,strObj3;
		strName = GetObjectNameRedux();
		strObj1 = P1->GetObjectNameRedux();
		strObj2 = P2->GetObjectNameRedux();
		strObj3 = P3->GetObjectNameRedux();
		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("CircleD[%s,%s,%s,%s];"),strName,strObj1,strObj2,strObj3);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("CircleD(%s,%s,%s,%s);"),strName,strObj1,strObj2,strObj3);
    }
    return mstr;
}*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCercleInterPS3D, CCercle3D, VERSIONABLE_SCHEMA | 1)

CCercleInterPS3D::CCercleInterPS3D() : CCercle3D()
{
}

CCercleInterPS3D::CCercleInterPS3D(CPlan3D *pl,CSphere3D *sp) : CCercle3D()
{
    SP = sp;
    PL = pl;
}

CCercleInterPS3D::CCercleInterPS3D(const CObject3D &src) : CCercle3D(src)
{
    SP = ((CCercleInterPS3D&)src).SP;
    PL = ((CCercleInterPS3D&)src).PL;
}

CObject3D* CCercleInterPS3D::CopyObject()
{
    CCercleInterPS3D *temp = new CCercleInterPS3D((CObject3D&)*this);
    return temp;
}

int CCercleInterPS3D::SetDepth()
{
    if (SP && PL)
        nDepth = max(SP->nDepth,PL->nDepth)+1;
    return nDepth;
}

CxObject3DSet* CCercleInterPS3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (SP) list->Add(SP);
    if (PL) list->Add(PL);
    return list;
}

void CCercleInterPS3D::Serialize( CArchive& ar )
{
    CCercle3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((SP) ? SP->nObjectId : -1);
        ar << ((PL) ? PL->nObjectId : -1);
    }
    else
    {
        SP = (CSphere3D*)SerializeObj(ar);
        PL = (CPlan3D*)SerializeObj(ar);
    }
}



BOOL CCercleInterPS3D::IsEqual(CObject3D &other)
{
    return CCercle3D::IsEqual(other);
}

UINT  CCercleInterPS3D::CalculConceptuel()
{
    bValidate = ((SP->bValidate) && (PL->bValidate));
    if (!bValidate)
        return ERR_NOCERCLE;

	CDroitePerp3D perp(SP->P1,PL);
	UINT res = perp.CalculConceptuel();
	CPointInterDP3D inter(&perp,PL);
	inter.CalculConceptuel();
	
	Center = inter.Concept_pt;
	CVector4 d = Center - SP->P1->Concept_pt;
	FCoord dist = d.Norme();
	FCoord ray = SP->Rayon;
	if (ray<dist)
	{
		bValidate = FALSE;
		return ERR_INTER_PLANESPHERE;
	}

	Radius = sqrtl(ray*ray-dist*dist);

    VecNorm = PL->VecNorm;
	VecNorm.Normalized();
    CVector4 I,J,K;
    FCoord normK = 1 / VecNorm.Norme();
    K = VecNorm * normK;

    J = CVector4(-K.y,K.x,0,1);
    normK = J.Norme();
    if (FCZero(normK))
        { J.x = J.z =0; J.y = 1;    }
    else
        J = J * (1/normK);
    I = (J % K);

    LocRep = CLocalRep(Center,I,J,K);

    m_cConcept_pts.RemoveAll();
    m_cConcept_pts.SetSize(nDeltaT);
    for (int t=0;t<nDeltaT;t++)
    {
        CVector4 U,V;
        U.x=    Radius*cos(nArcAngle*t/nDeltaT);
        U.y=    Radius*sin(nArcAngle*t/nDeltaT);
        U.z=    0;
        U.w=    1;

        V.x=    I.x*U.x + J.x*U.y + K.x*U.z + Center.x,
        V.y=    I.y*U.x + J.y*U.y + K.y*U.z + Center.y,
        V.z=    I.z*U.x + J.z*U.y + K.z*U.z + Center.z,
        V.w=    1;
        m_cConcept_pts.SetAt(t,V);
    }
    bValidate=TRUE;
	return 0;
}

CString CCercleInterPS3D::ExportSymbolic(int nFormat)
{
	return CObject3D::ExportSymbolic(nFormat);
}

CString CCercleInterPS3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (SP) sn1 = SP->GetObjectHelp();
    if (PL) sn2 = PL->GetObjectHelp();

    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}
