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
/// @file ConicSection3D.cpp
/// Implementation of the CConicSection3D class.
//
/// $Date: 2007-10-28 12:00:15+00 $
/// $Revision: 1.3 $
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\calques3d.h"

#include "..\prefs\Prefs.h"

#include "Cone3D.h"
#include "Plan3D.h"
#include "Droite3D.h"
#include "Point3D.h"

#include "..\OGLTools\glut.h"

#include "ConicSection3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CConicSection3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CConicSection3D::CConicSection3D()
{
    m_pCone = NULL;
    m_pPlan = NULL;
	m_nConicType = CONIC_NONE;
	m_fCutAngle = -1;
	m_fDistVertex = -1;
	m_nDelta = 36;
}


CConicSection3D::CConicSection3D(CCone3D *p1,CPlan3D *p2): CObject3D()
{
    m_pCone = p1;
    m_pPlan = p2;
 	m_nConicType = CONIC_NONE;
	m_fCutAngle = -1;
	m_fDistVertex = -1;
	m_nDelta = 36;
   SetDepth();
}

CConicSection3D::CConicSection3D(const CObject3D & src): CObject3D(src)
{
    m_pCone = ((CConicSection3D&)src).m_pCone;
    m_pPlan = ((CConicSection3D&)src).m_pPlan;
 	m_nConicType = ((CConicSection3D&)src).m_nConicType;
	m_fCutAngle = ((CConicSection3D&)src).m_fCutAngle;
	m_fDistVertex = ((CConicSection3D&)src).m_fDistVertex;
	m_nDelta = 36;
}

CConicSection3D::~CConicSection3D()
{

}

int CConicSection3D::SetDepth()
{
    if (m_pCone && m_pPlan)
        nDepth = max(m_pCone->nDepth,m_pPlan->nDepth)+1;
    return nDepth;
}

void CConicSection3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((m_pCone) ? m_pCone->nObjectId : -1);
        ar << ((m_pPlan) ? m_pPlan->nObjectId : -1);
    }
    else
    {
        m_pCone = (CCone3D*)SerializeObj(ar);
        m_pPlan = (CPlan3D*)SerializeObj(ar);
    }

}

//////////////////////////////////////////////////////////////////////
// Definition
//////////////////////////////////////////////////////////////////////

UINT CConicSection3D::GetHelpID() const
{
    return IDS_HELP_CONIC;
}

UINT CConicSection3D::GetDefID() const
{
    return IDS_DEF_OBJECT;
}

UINT CConicSection3D::GetNameID() const 
{ 
	return IDS_NAME_CONIC;
}



CObject3D* CConicSection3D::CopyObject()
{
    CConicSection3D *temp = new CConicSection3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CConicSection3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (m_pCone) list->Add(m_pCone);
    if (m_pPlan) list->Add(m_pPlan);
    return list;
}

CString CConicSection3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (m_pCone) sn1 = m_pCone->GetObjectHelp();
    if (m_pPlan) sn2 = m_pPlan->GetObjectHelp();

	sFormat = _T("%s is a conic defined by the intersection of %s and %s");
	switch(m_nConicType)
	{
	case CONIC_LINE:
		 sFormat += _T(" (a single line, degenerate case of a parabola).");
		break;
	case CONIC_PARABOLA:
		sFormat += _T(" (a parabola).");
	    break;
	case CONIC_2LINES:
		sFormat += _T(" (two intersecting lines, a degenerate case of an hyperbola).");
	    break;
	case CONIC_HYPERBOLA:
		sFormat += _T(" (an hyperbola).");	    
		break;
	case CONIC_POINT:
		sFormat += _T(" (a point, a degenerate case of an ellipse).");	    
		break;
	case CONIC_CIRCLE:
		sFormat += _T(" (a circle, a degenerate case of an ellipse).");	    
		break;
	case CONIC_ELLIPSE:
		sFormat += _T(" (an ellipse).");
		break;
	default:
		sFormat += _T(".");
	    break;
	}

	//if (FCZero(m_fCutAngle-M_PI/2))
    mstr.Format(sFormat,sName,sn1,sn2);
    return mstr;
}

BOOL CConicSection3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

BOOL CConicSection3D::IsInActiveArea(CPoint thePt)
{
    bool bIsInSeg = FALSE;
	int nDeltaT = m_cVisual_pts.GetSize();

	if (m_nConicType==CONIC_2LINES)
	{
		for (int t=0;((t<nDeltaT) && (!bIsInSeg));t++)
		 {
			CPoint  p1 = m_pCone->pApex->Visual_pt,
					p2 = m_cVisual_pts.GetAt(t);

			CRgn *pRgn = DoSegRgn(p1,p2);
			bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
			delete  pRgn;
		 }
	}
    else for (int t=0;(t<(nDeltaT-1) && (!bIsInSeg));t++)
     {
        CPoint  p1 = m_cVisual_pts.GetAt(t),
                p2 = m_cVisual_pts.GetAt(t+1);

        CRgn *pRgn = DoSegRgn(p1,p2);
        bIsInSeg = (pRgn && pRgn->m_hObject && pRgn->PtInRegion(thePt));
        delete  pRgn;
     }
    return bIsInSeg;
}

UINT  CConicSection3D::CalculConceptuel()
{
	m_cConcept_pts.RemoveAll();
	m_cVisual_pts.RemoveAll();

    bValidate = ((m_pCone->bValidate) && (m_pPlan->bValidate));
    if (!bValidate)
        return ERR_NOCERCLE;

	m_fDistVertex = CPlan3D::GetDistancePt(m_pPlan->GetNormalForm(),m_pCone->GetConeApex());

	CVector4 N1 = m_pPlan->VecNorm.Normalized();
	N1.Norme();
	CVector4 N2 = m_pCone->GetConeAxis().Normalized();
	N2.Norme();
	FCoord as1= N1 * N2;
	FCoord s1 = asin(as1);
	FCoord da1 = RTD(s1);
	if (as1<0) s1 = -s1;
	da1 = RTD(s1);

	m_fCutAngle = s1;
	
	if (FCZero(m_fCutAngle-m_pCone->nOpenAngle))
	{
		if (FCZero(m_fDistVertex))	m_nConicType=CONIC_LINE;
		else m_nConicType=CONIC_PARABOLA;
	}
	else if (m_fCutAngle<m_pCone->nOpenAngle)
	{
		if (FCZero(m_fDistVertex)) 	m_nConicType=CONIC_2LINES;
		else m_nConicType=CONIC_HYPERBOLA;
	}
	else if (m_fCutAngle>m_pCone->nOpenAngle)
	{
		if (FCZero(m_fDistVertex)) 	m_nConicType=CONIC_POINT;
		else if (FCZero(m_fCutAngle-M_PI/2))
			m_nConicType=CONIC_CIRCLE;
		else
			m_nConicType=CONIC_ELLIPSE;
	}

	int res = ComputeGeneralEquation();
	if (!res)
		ComputeAttributes();

	return res;
}

UINT CConicSection3D::ComputeGeneralEquation()
{
	if (m_nConicType==CONIC_2LINES)
	{
		CVector4 cAxis = m_pCone->GetConeAxis();
		cAxis= cAxis.Normalized();
		cAxis.Norme();
		CVector4 pAxis = m_pPlan->VecNorm;
		pAxis= pAxis.Normalized();
		pAxis.Norme();
		CVector4 perp = cAxis % pAxis;
		perp= perp.Normalized();
		perp.Norme();

		CVector4 center = m_pCone->Base1->Concept_pt;
		CVector4 pt1 = center + perp * m_pCone->nRadius;
		CVector4 pt2 = center + perp * (-m_pCone->nRadius);
		m_cConcept_pts.SetSize(2);
		m_cConcept_pts.SetAt(0,pt1);
		m_cConcept_pts.SetAt(1,pt2);
		return 1;
	}
	// construct the axis of the cone
	CDroite3D axis(m_pCone->pApex,m_pCone->pAxis);
	axis.CalculConceptuel();

	// construct the intersection of axis and cutting plane
	CPointInterDP3D ptC(&axis,m_pPlan);
	int res = ptC.CalculConceptuel();
	if (res) 
		return res;	// The cutting plane is parallel to the cone's axis

	m_ptCenter = ptC.Concept_pt;

	// construct the base plane
	CPlanPerp3D plP(&ptC,&axis);
	plP.CalculConceptuel();

	// construct the intersection of the two planes
	CDroiteInterPP3D perpAxis(&plP,m_pPlan);
	res = perpAxis.CalculConceptuel();

	FCoord theta = 0.;

	// Get the radius at the base
	FCoord fHeight = (ptC.Concept_pt - m_pCone->GetConeApex()).Norme();
	FCoord radBase = tan(m_pCone->nOpenAngle) * fHeight;
	FCoord rho = radBase;
	if (res)
	{
		//return res;	// The cutting plane is perpendicular to the cone's axis
		m_LocRep = m_pPlan->ptonRep;
		m_LocRep.I = m_LocRep.I.Normalized();
		m_LocRep.I.Norme();
		m_LocRep.J = m_LocRep.J.Normalized();
		m_LocRep.J.Norme();
		m_LocRep.K = m_LocRep.K.Normalized();
		m_LocRep.K.Norme();

		m_LocRep.O = ptC.Concept_pt;
	}
	else
	{
		// construct the normal of the cutting plane
		CDroitePerp3D plNorme(&ptC,m_pPlan);
		plNorme.CalculConceptuel();
	
		// construct the axis of the cutting plane
		CDroitePerpDD3D cutAxis(&perpAxis,&plNorme);	
		cutAxis.CalculConceptuel();
		// construct the axis of the base plane
		CDroitePerpDD3D baseAxis(&perpAxis,&axis);	
		baseAxis.CalculConceptuel();

		CVector4 axdir = baseAxis.GetDirVector();
		axdir = axdir.Normalized();
		axdir.Norme();
		CVector4 vv = axdir * radBase;
		vv = ptC.Concept_pt + vv;
		CPoint3D ptB(vv);

		CDroite3D drNappe(m_pCone->pApex,&ptB);
		drNappe.CalculConceptuel();
		CPointInterDD3D ptD(&cutAxis,&drNappe);
		ptD.CalculConceptuel();

		FCoord phi = (ptD.Concept_pt - ptC.Concept_pt).Norme();

		CVector4 axdir2 = cutAxis.GetDirVector();
		axdir2 = axdir2.Normalized();
		axdir2.Norme();

		theta = acos(axdir2 * axdir);
		if (theta > M_PI/2) theta = M_PI-theta;

		CVector4 tt1 = ptD.Concept_pt;
		axdir2 = cutAxis.GetDirVector();
		axdir2 = axdir2.Normalized();
		axdir2.Norme();
		CVector4 tt2 = tt1 + (axdir2) * (m_fEMajor);
		CVector4 I = ptC.Concept_pt - ptD.Concept_pt;
		I = I.Normalized();
		I.Norme();
		CVector4 K = plNorme.GetDirVector();
		K = K.Normalized();
		K.Norme();
		CVector4 J = K%I;
		m_LocRep = CLocalRep(ptC.Concept_pt,I,J,K);
	}

	FCoord da1 = RTD(theta);
	FCoord da2 = RTD(m_fCutAngle);
	FCoord da3 = RTD(m_pCone->nOpenAngle);
	FCoord alpha = M_PI/2-m_pCone->nOpenAngle;
	FCoord da4 = RTD(alpha);

	m_cGeneralEq.A = sin(alpha)*sin(alpha) - sin(theta)*sin(theta);
	m_cGeneralEq.B = 0;
	m_cGeneralEq.C = sin(alpha)*sin(alpha);
	m_cGeneralEq.D = rho*sin(2*alpha)*sin(theta);
	m_cGeneralEq.E = 0;
	m_cGeneralEq.F = - rho*rho*sin(alpha)*sin(alpha);

	m_fEccent = sin(theta)/sin(alpha);


	return 0;
}

UINT CConicSection3D::ComputeAttributes()
{
    // compute rotation for coordinate frame of standard conic
    // and coefficients of rotated conic
    FCoord theta;
    if (m_cGeneralEq.A == m_cGeneralEq.C) 
		theta = M_PI/4;
    else
        theta = 0.5*atan(m_cGeneralEq.B/(m_cGeneralEq.A - m_cGeneralEq.C));
    FCoord Sa =  sin(theta);
    FCoord Ca =  cos(theta);
    FCoord Ar =  m_cGeneralEq.A*Ca*Ca + m_cGeneralEq.B*Sa*Ca + m_cGeneralEq.C*Sa*Sa;
    FCoord Cr =  m_cGeneralEq.A*Sa*Sa - m_cGeneralEq.B*Sa*Ca + m_cGeneralEq.C*Ca*Ca;
    FCoord Dr =  m_cGeneralEq.D*Ca + m_cGeneralEq.E*Sa;
    FCoord Er = -m_cGeneralEq.D*Sa + m_cGeneralEq.E*Ca;
    FCoord Fr =  m_cGeneralEq.F;

    m_fRotation = theta;

    // compute offset of coordinate frame of standard conic
    // and coefficients of translated conic
    FCoord Xo, Yo, a2, b2, c, p;
	
	FCoord R[6];
	for (int i = 0; i < 6; i++)                        // init coefficients to zero
        R[i] = 0;
    
	if ((m_nConicType==CONIC_ELLIPSE)||(m_nConicType==CONIC_CIRCLE))
    {
        // circle, ellipse
        Xo = -Dr/(2*Ar);
        Yo = -Er/(2*Cr);
        m_ptTrans = CVector4(Xo,Yo,0);

        R[0] = Ar/(Ar*Xo*Xo + Cr*Yo*Yo - Fr);
        R[2] = Cr/(Ar*Xo*Xo + Cr*Yo*Yo - Fr);
        R[5] = -1.0;

        if (fabs(R[0]) < fabs(R[2]))       // foci on x-axis for ellipse
        {
            a2 = fabs(1 / R[0]);
            b2 = fabs(1 / R[2]);
            //if (m_nConicType==CONIC_ELLIPSE)                            // ellipse
            {
                c = sqrtl( a2 - b2 );
                m_ptFocus = CVector4(c,0,0);
                m_fEccent = c / sqrtl(a2);
            }
			m_fEMajor = sqrtl(a2);
			m_fEMinor = sqrtl(b2);

        }
        else                                       // foci on y-axis for ellipse
        {
            a2 = fabs(1/R[2]);
            b2 = fabs(1/R[0]);
           //if (m_nConicType==CONIC_ELLIPSE)                            // ellipse
            {
                c = sqrtl( a2 - b2 );
                m_ptFocus = CVector4(0,c,0);
				m_fEccent = c / sqrtl(a2);
            }
  			m_fEMajor = sqrtl(a2);
			m_fEMinor = sqrtl(b2);
      }
	m_cConcept_pts.SetSize(m_nDelta+1);

	CVector4 I = m_LocRep.I;
// 	if (m_ptTrans.x<0) I = I * (-1.00);
// 	else if (m_ptTrans.y<0) I = I * (-1.00);
// 	else if (m_ptTrans.z<0) I = I * (-1.00);
	FCoord nArcAngle = 2*M_PI;
		for (int t=0;t<m_nDelta+1;t++)
		{
	       CVector4 U,V;
	       U.x=  - m_ptTrans.x +  m_fEMajor*cos(nArcAngle*t/m_nDelta);
	       U.y=   -m_ptTrans.y + m_fEMinor*sin(nArcAngle*t/m_nDelta);
	       U.z=    0;
	       U.w=    1;

	       V.x=    I.x*U.x + m_LocRep.J.x*U.y + m_LocRep.K.x*U.z + m_ptCenter.x;
	      V.y=    I.y*U.x + m_LocRep.J.y*U.y + m_LocRep.K.y*U.z  + m_ptCenter.y;
	       V.z=   I.z*U.x + m_LocRep.J.z*U.y + m_LocRep.K.z*U.z  + m_ptCenter.z;
	      V.w=    1;
		        m_cConcept_pts.SetAt(t,V);

		}


    }    
    else if (m_nConicType == CONIC_HYPERBOLA)                               // hyperbola
    {
        // circle, ellipse, or hyperbola
        Xo = -Dr/(2*Ar);
        Yo = -Er/(2*Cr);
        m_ptTrans = CVector4(Xo,Yo,0);

        R[0] = Ar/(Ar*Xo*Xo + Cr*Yo*Yo - Fr);
        R[2] = Cr/(Ar*Xo*Xo + Cr*Yo*Yo - Fr);
        R[5] = -1.0;

        if (R[0] > R[2])                           // foci on x-axis
        {
            a2 = fabs(1 / R[0]);
            b2 = fabs(1 / R[2]);

            c = sqrtl( a2 + b2 );
			m_ptFocus = CVector4(c,0,0);
			m_fEccent = c / sqrtl(a2);
        }
        else                                       // foci on y-axis
        {
            a2 = fabs(1/R[2]);
            b2 = fabs(1/R[0]);
            c = sqrtl( a2 + b2 );
			m_ptFocus = CVector4(0,c,0);
			m_fEccent = c / sqrtl(a2);
        }
			m_fEMajor = sqrtl(a2);
			m_fEMinor = sqrtl(b2);
		m_cConcept_pts.SetSize(72);
		for (int t=35;t>=0;t--)
		{
			FCoord t1 = t/20.;
			CVector4 U,V;
			U.x= -m_ptTrans.x +  m_fEMajor*cosh(t1);
			U.y= -m_ptTrans.y +  m_fEMinor*sinh(t1);
			U.z= 0;
			U.w= 1;
			
			V.x= m_LocRep.I.x*U.x + m_LocRep.J.x*U.y + m_LocRep.K.x*U.z + m_ptCenter.x;
			V.y= m_LocRep.I.y*U.x + m_LocRep.J.y*U.y + m_LocRep.K.y*U.z  + m_ptCenter.y;
			V.z= m_LocRep.I.z*U.x + m_LocRep.J.z*U.y + m_LocRep.K.z*U.z  + m_ptCenter.z;
			V.w= 1;
			m_cConcept_pts.SetAt(35-t,V);

			U.y= -m_ptTrans.y -  m_fEMinor*sinh(t1);
			V.x= m_LocRep.I.x*U.x + m_LocRep.J.x*U.y + m_LocRep.K.x*U.z + m_ptCenter.x;
			V.y= m_LocRep.I.y*U.x + m_LocRep.J.y*U.y + m_LocRep.K.y*U.z  + m_ptCenter.y;
			V.z= m_LocRep.I.z*U.x + m_LocRep.J.z*U.y + m_LocRep.K.z*U.z  + m_ptCenter.z;
			V.w= 1;
			m_cConcept_pts.SetAt(35+t+1,V);
		}
    }
    else if (m_nConicType == CONIC_PARABOLA)                               // parabola
    {
        if (fabs(Ar) > fabs(Cr))           // form: x² - 4·p·y = 0
        {
            Xo = Dr/(2*Ar);
            Yo = (Fr - Dr*Dr/(4*Ar))/Er;
	        m_ptTrans = CVector4(Xo,Yo,0);
            
            R[0] = 1;
            R[4] = Er / Ar;
            p = -R[4]/4;
    			m_ptFocus = CVector4(0,p,0);
        }
        else                                       // form: y² - 4·p·x = 0
        {
            Xo = (Fr - Er*Er/(4*Cr))/Dr;
            Yo = Er/(2*Cr);
	        m_ptTrans = CVector4(Xo,Yo,0);

            R[2] = 1;
            R[3] = Dr / Cr;
            p = -R[3]/4;
    			m_ptFocus = CVector4(p,0,0);
        }
		m_fEccent = 1;
		m_cConcept_pts.SetSize(72);
		for (int t=35;t>=0;t--)
		{
			FCoord t1 = t/5.;
			CVector4 U,V;
			U.x= -m_ptTrans.x +  p*t1*t1;
			U.y= -m_ptTrans.y + 2*p*t1;
			U.z= 0;
			U.w= 1;
			
			V.x= -m_LocRep.I.x*U.x + m_LocRep.J.x*U.y + m_LocRep.K.x*U.z + m_ptCenter.x;
			V.y= -m_LocRep.I.y*U.x + m_LocRep.J.y*U.y + m_LocRep.K.y*U.z  + m_ptCenter.y;
			V.z= -m_LocRep.I.z*U.x + m_LocRep.J.z*U.y + m_LocRep.K.z*U.z  + m_ptCenter.z;
			V.w= 1;
			m_cConcept_pts.SetAt(35-t,V);

			U.y= -U.y;
			V.x= -m_LocRep.I.x*U.x + m_LocRep.J.x*U.y + m_LocRep.K.x*U.z + m_ptCenter.x;
			V.y= -m_LocRep.I.y*U.x + m_LocRep.J.y*U.y + m_LocRep.K.y*U.z  + m_ptCenter.y;
			V.z= -m_LocRep.I.z*U.x + m_LocRep.J.z*U.y + m_LocRep.K.z*U.z  + m_ptCenter.z;
			V.w= 1;
			m_cConcept_pts.SetAt(35+t+1,V);
		}
    }	
	return 0;
}


void CConicSection3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSM)
{
    if ((!bVisible && !TPref::TUniv.bShowHidden) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

	m_cVisual_pts.RemoveAll();
	m_cVisual_pts.SetSize(m_cConcept_pts.GetSize());
    for (int t=0;t<m_cConcept_pts.GetSize();t++)
     {
        CVector4 V = m_cConcept_pts.GetAt(t);
        CPoint pt = (CPoint)mV->ProjectPoint(V);
        m_cVisual_pts.SetAt(t,pt);
     }
	
	if (m_nConicType==CONIC_2LINES)
	{
	    for (int t=0;t<m_cConcept_pts.GetSize();t++)
		 {
			CVector4 V = m_cConcept_pts.GetAt(t);
			CPoint3D pt(V);
			CDemiDroite3D sr(m_pCone->pApex,&pt);
			sr.pObjectShape = pObjectShape;
		
			sr.CalculConceptuel();
			sr.CalculVisuel(mV);
			sr.Draw(pDC,mV,bSM);
		}
		return;

	}
    CPen curPen,disPen;
//    curPen.CreatePenIndirect(&(pObjectShape.GetPenStyle()));

 //   disPen.CreatePenIndirect(&(pObjectShape.GetHiddenPenStyle(
 //               pObjectShape.GetObjectHiddenColor())));

	if (!bVisible && TPref::TUniv.bShowHidden)
	{
	    curPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
		disPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
	}
	else
	{
	    curPen.CreatePenIndirect(&(pObjectShape.GetPenStyle()));
	   disPen.CreatePenIndirect(&(pObjectShape.GetHiddenPenStyle(
		          pObjectShape.GetObjectHiddenColor())));

	}

    bool bVis1;

    CBrush curBrush(pObjectShape.clrObject);
    CBrush *pOldB = pDC->SelectObject(&curBrush);
    //  HandleDC.SelectObject(curPen);

    for (t=0;t<m_cConcept_pts.GetSize()-1;t++)
     {
        CPen *pOldP = NULL;
        if (!bSM)
         {
            CVector4    U = m_cConcept_pts.GetAt(t),
                        V;
			V = m_cConcept_pts.GetAt((t==(m_cConcept_pts.GetSize()-1)) ?0 : t+1);
            bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));
            pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
         }
        else
            pOldP = pDC->SelectObject(&curPen);

        CPoint start = m_cVisual_pts[t];
        CPoint endpt= m_cVisual_pts[t+1];
        pDC->MoveTo(start);
        pDC->LineTo(endpt);
        if (pOldP) pDC->SelectObject(pOldP);
     }
    if (pOldB) pDC->SelectObject(pOldB);

}

void CConicSection3D::Draw3DRendering(int nVolMode)
{
    if ((!bVisible) || (!bValidate)) return;
	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
    float mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	FCoord nArcAngle = 2*M_PI;
	CVector4 I = m_LocRep.I;
	CVector4 K = m_LocRep.K;
	CVector4 J = m_LocRep.J;
	CVector4 pts;
	BOOL bStart = TRUE;

	GLUquadricObj*m_quadrObj=gluNewQuadric();

 	int	nDeltaT = m_cConcept_pts.GetSize();
    for (int t=0;t<nDeltaT-1;t++)
    {
        CVector4 U = m_cConcept_pts[t],
				 V = m_cConcept_pts[t+1];

		GLdouble bx = (U.x/TPref::TUniv.nUnitRep/3);
		GLdouble by = (U.y/TPref::TUniv.nUnitRep/3);
		GLdouble bz = (U.z/TPref::TUniv.nUnitRep/3);
		
		CVector4 dl = (V-U).Normalized();;
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
		FCoord N = (V-U).Norme();
		size = (N/TPref::TUniv.nUnitRep/3);
		glPushMatrix();
		glTranslated(bx, by, bz);
		glRotated(dd,drot.x,drot.y,drot.z);
		COLORREF clr = pObjectShape.GetObjectColor();
		glColor3f(GetRValue(clr)/255.f,GetGValue(clr)/255.f,GetBValue(clr)/255.f);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
			gluCylinder(m_quadrObj,0.02,0.02,size,16,16);
		glPopMatrix();
	}

}


CString CConicSection3D::ExportSymbolic(int nFormat)
{
	return CObject3D::ExportSymbolic(nFormat);
}
