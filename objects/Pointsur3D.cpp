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
// Pointsur3D.cpp: implementation of the CPointSur3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Point3D.h"
#include "Droite3D.h"
#include "Cercle3D.h"
#include "Plan3D.h"
#include "Sphere3d.h"
#include "Cylinder3D.h"
#include "Cone3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//extern void CalcUnit(CVector4 I,CVector4 J,CVector4 K,
//							CVector4 P1,int t,FCoord r,CVector4 & V);
//extern void DrawUnit(CVector4& deb,CVector4& fin,CVector4& P1,
//					CDC* HandleDC,CVisualParam *mV,
//					CPen& curPen,CPen& curPenH,CPen& disPen,CPen& disPenH);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSur3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointSur3D::CPointSur3D() : CPoint3D() 
{
}

CPointSur3D::CPointSur3D(const CObject3D & src): CPoint3D(src)
{
}

CxSchemeSet* CPointSur3D::GetRedefineSchemes(CxSchemeSet* pSet) 
{ 
	if (!pSet) pSet = new CxSchemeSet();

	/// Redefine the free-point as a Point on line
	CRedefineScheme *pObj = new CRedefineScheme();
	pObj->nRedefStep = 0;
	pObj->nObjResID = CTX_POINT;
	pObj->nAvail = ID_OBJECT_POINT;
	pObj->pNewObj = new CPoint3D(Concept_pt);
	pSet->Add(pObj);

	return pSet;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSurD3D, CPointSur3D, VERSIONABLE_SCHEMA | 1)

CPointSurD3D::CPointSurD3D() : CPointSur3D() 
{
	S = NULL;
	lambda = 0.;
}

CPointSurD3D::CPointSurD3D(CDroite3D *s1): CPointSur3D()
{
	S = s1;
	nDepth = s1->nDepth+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtConst;
}

CPointSurD3D::CPointSurD3D(const CObject3D &src): CPointSur3D(src)
{
	S =   ((CPointSurD3D&)src).S;
	lambda = ((CPointSurD3D&)src).lambda;

}

int CPointSurD3D::SetDepth()
{
	if (S)
		nDepth = S->nDepth+1;
	return nDepth;
}

CxSchemeSet* CPointSurD3D::GetRedefineSchemes(CxSchemeSet* pSet) 
{ 
	pSet = CPointSur3D::GetRedefineSchemes(pSet) ;
	if (!pSet) pSet = new CxSchemeSet();

	/// Redefine the point on line as a intersection Dr/Dr
	CRedefineScheme *pObj = new CRedefineScheme();
	pObj->nRedefStep = 1;
	pObj->nObjResID = CTX_INTERSECTION_LL;
	pObj->nAvail = ID_CONSTRUCTION_INTERSECTION_LINELINE;
	pObj->nStepID[0] = TAllDroiteClass;
	pObj->nTaskResID[0] = CTX_SELECT_DROITE;
	pObj->pNewObj = new CPointInterDD3D();
	((CPointInterDD3D*)pObj->pNewObj)->D1 = S;
	pSet->Add(pObj);

	/// Redefine the point on line as a intersection Dr/Pl
	pObj = new CRedefineScheme();
	pObj->nRedefStep = 1;
	pObj->nObjResID = CTX_INTERSECTION_LP;
	pObj->nAvail = ID_CONSTRUCTION_INTERSECTION_LINEPLANE;
	pObj->nStepID[0] = TAllPlanClass;
	pObj->nTaskResID[0] = CTX_SELECT_PLAN1;
	pObj->pNewObj = new CPointInterDP3D();
	((CPointInterDP3D*)pObj->pNewObj)->D1 = S;
	pSet->Add(pObj);

	return pSet;
}

CString CPointSurD3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (/*bValidate && */S)
	{
        CString strFunc,strName,strObj1;
		strName = GetObjectNameRedux();
		strObj1 = S->GetObjectNameRedux();

		CObjectId nType = S->isA();
		if (nType == TSegment3DClass)
			strFunc = _T("PointOnSegment");
		else if (nType == TDemiDroite3DClass)
			strFunc = _T("PointOnRay");
		else //if (nType == TDroite3DClass)
			strFunc = _T("PointOnLine");

		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("%s[%s,%s];"),strFunc,strName,strObj1);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("%s(%s,%s);"),strFunc,strName,strObj1);
	}
	return mstr;
}




void CPointSurD3D::Serialize( CArchive& ar )
{
	CPointSur3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << lambda;
		ar << ((S) ? S->nObjectId : -1);
	}
	else
	{
		ar >> lambda;
		S = (CDroite3D*)SerializeObj(ar);
	}
}


CObject3D* CPointSurD3D::CopyObject()
{
	CPointSurD3D *temp = new CPointSurD3D(*this);
	return temp;
}

CxObject3DSet* CPointSurD3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(S);
	return list;
}

BOOL CPointSurD3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet) return FALSE;
	int nb = pSet->GetSize();

	if (nb == 1 && !S)
	{
		S = (CDroite3D*)pSet->GetAt(0);
	}
	else if (!nb && S)
	{
	}
	else return FALSE;

	
	return TRUE;
}


void CPointSurD3D::CopyPointPosition(CObject3D* src)
{
	CPointSur3D::CopyPointPosition(src);
	if (!src) return;
	CObjectId ff = src->isA();
	if (ff == TPointSurD3DClass)
	{
		//Concept_pt =  ((CPointSurD3D*)src)->Concept_pt;
		lambda =  ((CPointSurD3D*)src)->lambda;
	}
}

void CPointSurD3D::CopyPointPosition(CVector4 pt)
{
	if (S->bIsSegment)
	 {
		CVector4 AB = S->P1->Concept_pt - S->P2->Concept_pt;
		CVector4 AM = S->P1->Concept_pt - pt;
		FCoord a = AM.Norme(),b = AB.Norme();
		FCoord dd = AB * AM;
		lambda = (AB.NullVector()) ? 0. : a/b;
		if (dd < 0.0)
			lambda = -lambda;
		if (!S->IsInLimit(lambda))
		{
			if (lambda < 0.0) lambda = 0.0;
			else lambda = 1.0;
		}

	 }
	else
	 {
		CVector4 A = S->GetBasePoint();
		CVector4 dir = S->GetDirVector();
		CVector4 AM = A - pt;
		FCoord pos = dir * AM;
		pos = (pos<0) ? -1. : 1.;
		lambda = -pos*AM.Norme();
		if (!S->IsInLimit(lambda))
		{
			if (lambda < 0.0) lambda = 0.0;
		}
	 }
	CVector4 M(0,0,0);
	if (S->bIsSegment)
	 {
		CVector4 AB = S->P1->Concept_pt - S->P2->Concept_pt;
		M = AB * (-lambda);
		M = M + S->P1->Concept_pt;
	 }
	else
	 {
		CVector4 A = S->GetBasePoint();
		CVector4 Dir = S->GetDirVector();
		//Dir = RealMult(1/Norme(Dir),Dir);
		Dir = Dir.Normalized();
		M = A + (Dir * lambda);
	 }
	Concept_pt = M;
}


CString CPointSurD3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???"));
	if (S) sn1 = S->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1);
	return mstr;
}


UINT  CPointSurD3D::CalculConceptuel()
{
	bValidate = (S->bValidate);
	if (!bValidate)
		return ERR_SUPPORTOBJECT;

/************************
	Vector4 V(S->GetDirVector());
	Vector4 D(MakeVector(S->P1->Concept_pt,Concept_pt));

	FCoord	u = DotProduct(V,V),
			v = DotProduct(D,V);
	 lambda = v / u;

	Vector4 M(S->P1->Concept_pt.x + lambda*V.x,
				 S->P1->Concept_pt.y + lambda*V.y,
				 S->P1->Concept_pt.z + lambda*V.z,
				 1);

	Concept_pt = M;  ***********************/
		/////************************//////////
	CVector4 M(0,0,0);
	if (S->bIsSegment)
	 {
		CVector4 AB = S->P1->Concept_pt - S->P2->Concept_pt;
		M = AB * (-lambda);
		M = M + S->P1->Concept_pt;
	 }
	else
	 {
		CVector4 A = S->GetBasePoint();
		CVector4 Dir = S->GetDirVector();
		//Dir = RealMult(1/Norme(Dir),Dir);
		Dir = Dir.Normalized();
		M = A + (Dir * lambda);
	 }
	Concept_pt = M;

/*			VvW	= (P) ?
				P->VecNorm :
				VectProduct(MakeVector(P1->Concept_pt,P2->Concept_pt),
							MakeVector(P1->Concept_pt,P3->Concept_pt)),
			AB = MakeVector(D1->P1->Concept_pt, (P) ?
							P->P1->Concept_pt : P1->Concept_pt);

	if ( FCZero(DotProd = DotProduct(U,VvW)) )
	  { Validate = FALSE;
		return 1;
		}

	lambda	= DotProduct(AB,VvW) / DotProd;*/
	 return 0;
}

/*CVector4 GetPrjPt(CVisualParam *myVisuParam,CVector4& res)
{
	FCoord
		st = myVisuParam->ST,
		sp = myVisuParam->SP,
		ct = myVisuParam->CT,
		cp = myVisuParam->CP,
		RCx = myVisuParam->ptRepCoord.x,
		RCy = myVisuParam->ptRepCoord.y,
		x0 = res.x-RCx,
		y0 = RCy-res.y;

	////////////// ZOOM
	x0 = (1/myVisuParam->nZoom)*x0;
	y0 = (1/myVisuParam->nZoom)*y0;

	FCoord
		z0 = myVisuParam->ProjParam.dis,
		Rh = myVisuParam->ProjParam.rho,
		z = -sp*z0 + sp*Rh + y0*cp,
		y = -sp*y0*st + x0*ct -cp*z0*st + cp*Rh*st,
		x = -ct*sp*y0 - ct*cp*z0 + ct*cp*Rh - x0*st;
	CVector4 ecran(x,y,z);
	return ecran;
}*/

/*CVector4 GetPrjPt2(CVisualParam *myVisuParam,CVector4& res)
{
	FCoord
		st = myVisuParam->ST,
		sp = myVisuParam->SP,
		ct = myVisuParam->CT,
		cp = myVisuParam->CP,
		RCx = myVisuParam->ptRepCoord.x,
		RCy = myVisuParam->ptRepCoord.y,
		x0 = res.x-RCx,
		y0 = RCy-res.y;

	////////////// ZOOM
	x0 = (1/myVisuParam->nZoom)*x0;
	y0 = (1/myVisuParam->nZoom)*y0;

	FCoord
		z0 = res.z,
		Rh = myVisuParam->ProjParam.rho,
		z = -sp*z0 + sp*Rh + y0*cp,
		y = -sp*y0*st + x0*ct -cp*z0*st + cp*Rh*st,
		x = -ct*sp*y0 - ct*cp*z0 + ct*cp*Rh - x0*st;
	CVector4 ecran(x,y,z);
	return ecran;
}*/

BOOL CPointSurD3D::MoveObject(CVisualParam *myVisuParam,UINT,CPoint MouseClic,CVector4&TempCpt)
{
	CVector4 dir = S->GetDirVector();
	CVector4 dir2(0,0,0);
	CPoint pt1 = (CPoint)myVisuParam->ProjectPoint(dir2);
	CPoint pt2 = (CPoint)myVisuParam->ProjectPoint(dir2+dir);
	CVector4 aa(pt2.x-pt1.x,pt2.y-pt1.y,0);
	if (aa.NullVector()) return 0;
	FCoord normK = 1 / aa.Norme();
	CVector4 aa2 = aa * normK;
	FCoord DotProd = fabsl(aa2.x);

	CVector4 res(MouseClic.x,MouseClic.y,0,1);
	//CVector4 ecran = GetPrjPt(myVisuParam,res);
	CVector4 ecran = myVisuParam->GetProjectedPoint(res);
	CVector4 d1(res.x,res.y,0),d2;
	if (DotProd > 0.7)
		d2 = CVector4(res.x,res.y+100.,0);
	else
		d2 = CVector4(res.x+100.,res.y,0);
	//d1 = GetPrjPt2(myVisuParam,d1);
	//d2 = GetPrjPt2(myVisuParam,d2);
	d1 = myVisuParam->GetScreenProjectionInf(d1);
	d2 = myVisuParam->GetScreenProjectionInf(d2);
	CVector4*	r = S->IntersectPlan(ecran,d1,d2,false);
	if (!r)
	 {
		//Geom3DApp->ShowErrorMsg(ERR_BADVIEWPT);
		return 0;
	 }
	TempCpt = *r;
	delete r;

	/////************************//////////
	if (S->bIsSegment)
	 {
		CVector4 AB = S->P1->Concept_pt - S->P2->Concept_pt;
		CVector4 AM = S->P1->Concept_pt - TempCpt;
		FCoord a = AM.Norme(),b = AB.Norme();
		FCoord dd = AB * AM;
		lambda = (AB.NullVector()) ? 0. : a/b;
		if (dd < 0.0)
			lambda = -lambda;
		if (!S->IsInLimit(lambda))
		{
			if (lambda < 0.0) lambda = 0.0;
			else lambda = 1.0;
		}

	 }
	else
	 {
		CVector4 A = S->GetBasePoint();
		CVector4 dir = S->GetDirVector();
		CVector4 AM = A - TempCpt;
		FCoord pos = dir * AM;
		pos = (pos<0) ? -1. : 1.;
		lambda = -pos*AM.Norme();
		if (!S->IsInLimit(lambda))
		{
			if (lambda < 0.0) lambda = 0.0;
		}
	 }

	return 1;

/*	Vector4 res(MouseClic.x,MouseClic.y,0,1);
	Vector4 ecran = GetPrjPt(myVisuParam,res);
	Vector4 d1(res.x,res.y,0);
	d1 = GetPrjPt2(myVisuParam,d1);
	TPoint a = (TPoint)myVisuParam->ProjectPoint(S->GetDirVector());
	TPoint b(a.y,-a.x);
	Vector4 d2(res.x+b.x,res.y+b.y,0);
	d2 = GetPrjPt2(myVisuParam,d2);
	Vector4*	r = S->IntersectPlan(ecran,d1,d2);
	if (!r) return 0;
	TempCpt = *r;*/

/*	TPoint3D pt(0,ecran);
	Vector4 plNorm(S->GetDirVector());
	TPlan3D pl(0,&pt,plNorm);
	UINT res1 = pl.CalculConceptuel();
	TPointInterDP3D dp(0,S,&pl);
	UINT res2 = dp.CalculConceptuel();
	if (res2) return 0;
	TempCpt = dp.Concept_pt;
*/



//	return 1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSurC3D, CPointSur3D, VERSIONABLE_SCHEMA | 1)

CPointSurC3D::CPointSurC3D() : CPointSur3D() 
{
	S = NULL;
	lambda = 0.;
}

CPointSurC3D::CPointSurC3D(CCercle3D *s1): CPointSur3D()
{
	S = s1;
	nDepth = s1->nDepth+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtConst;

}

CPointSurC3D::CPointSurC3D(const CObject3D & src): CPointSur3D(src)
{
	S =   ((CPointSurC3D&)src).S;
	lambda = ((CPointSurC3D&)src).lambda;
}

int CPointSurC3D::SetDepth()
{
	if (S)
		nDepth = S->nDepth+1;
	return nDepth;
}




void CPointSurC3D::Serialize( CArchive& ar )
{
	CPointSur3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << lambda;
		ar << ((S) ? S->nObjectId : -1);
	}
	else
	{
		ar >> lambda;
		S = (CCercle3D*)SerializeObj(ar);
	}
}


CObject3D* CPointSurC3D::CopyObject()
{
	CObject3D *temp = new CPointSurC3D((CObject3D&)*this);
	return temp;
}

CxObject3DSet* CPointSurC3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(S);
	return list;
}

BOOL CPointSurC3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet || S) return FALSE;
	int nb = pSet->GetSize();

	if (nb != 1 ) return FALSE;

	S = (CCercle3D*)pSet->GetAt(0);
	
	return TRUE;
}

void CPointSurC3D::CopyPointPosition(CVector4 pt)
{
	CVector4 UvV = S->VecNorm;
	FCoord r = S->Radius;

	if (UvV.NullVector()) return ;

	CLocalRep rep = S->LocRep;
	CVector4 U = pt - S->Center;
	U = U * (1/U.Norme());
	FCoord sa = rep.I * U;
	FCoord ca = rep.J * U;
	if (fabsl(sa) > 1.)
	{
		sa = (sa < 0.00) ? -1.00 : 1.00;
	}

	lambda = r / U.Norme();
	CVector4 V = S->Center + U*lambda;

	lambda = acosl(sa);

	if (ca <0) lambda = 2*M_PI - lambda;

	if (S->isA() == TArcCercle3DClass)
	 {
		FCoord test = S->nArcAngle;

		if (lambda  > (test+1E-5))
		 {
			FCoord lim = test +(2*M_PI - test)/2;
			if (lambda < lim)
				lambda = test;
			else lambda = 0.;
		 }
	 }
	Concept_pt = V;
}


void CPointSurC3D::CopyPointPosition(CObject3D* src)
{
	CPointSur3D::CopyPointPosition(src);
	if (!src) return;
	if (src->MaskObject(isA()))
	{
		//Concept_pt =  ((CPoint3D*)src)->Concept_pt;
		lambda =  ((CPointSurC3D*)src)->lambda;
	}
}

CString CPointSurC3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???"));
	if (S) sn1 = S->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1);
	return mstr;
}


UINT  CPointSurC3D::CalculConceptuel()
{
	bValidate = (S->bValidate);
	if (!bValidate)
		return ERR_SUPPORTOBJECT;

//	Vector4	U = MakeVector(S->P1->Concept_pt,S->P2->Concept_pt),
//				V = MakeVector(S->P1->Concept_pt,S->P3->Concept_pt),
//				UvV = VectProduct(U,V);
	CVector4 V;
	CVector4 UvV = S->VecNorm;
	FCoord r = S->Radius;
	if (UvV.NullVector())
	 {	
		bValidate = FALSE;
		return ERR_SUPPORTOBJECT;
	 }

/*	FCoord normK = 1 / Norme(UvV);
	Vector4	K = RealMult(normK,UvV);
//	Vector4 J= U;
	Vector4	J(-K.y,K.x,0,1);
	normK = Norme(J);
	if (FCZero(normK))
		{ J.x = J.z =0; J.y = 1;	}
	else
		J = RealMult(1/normK,J);
	Vector4	I = VectProduct(J,K);*/

//	FCoord normK = 1 / Norme(UvV);
//	Vector4	K = RealMult(normK,UvV);
//	Vector4	I = MakeVector(S->Center,S->P1->Concept_pt);
//	normK = Norme(I);
//	I = RealMult(1/normK,I);
//	Vector4	J = VectProduct(K,I);
	if (S->isA() == TArcCercle3DClass)
	 {
		FCoord test = S->nArcAngle;

		if (lambda  > (test+1E-5))
		 {
			FCoord lim = test +(2*M_PI - test)/2;
			if (lambda < lim)
				lambda = test;
			else lambda = 0.;
		 }
	}

	CLocalRep rep = S->LocRep;
	CVector4	U = CVector4(r*cosl(lambda),r*sinl(lambda),0,1);
	V.x=	rep.I.x*U.x + rep.J.x*U.y + rep.K.x*U.z + S->Center.x,
	V.y=	rep.I.y*U.x + rep.J.y*U.y + rep.K.y*U.z + S->Center.y,
	V.z=	rep.I.z*U.x + rep.J.z*U.y + rep.K.z*U.z + S->Center.z,
	V.w=	1;
	Concept_pt = V;
	bValidate=1;
	return 0;
}

CString CPointSurC3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (/*bValidate && */S)
	{
        CString strFunc,strName,strObj1;
		strName = GetObjectNameRedux();
		strObj1 = S->GetObjectNameRedux();

		CObjectId nType = S->isA();
		if (nType == TCercle3DClass)
			strFunc = _T("PointOnCircle");
		else if (nType == TArcCercle3DClass)
			strFunc = _T("PointOnCircleArc");
		else //if (nType == TDroite3DClass)
			return CObject3D::ExportSymbolic(nFormat);

		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("%s[%s,%s];"),strFunc,strName,strObj1);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("%s(%s,%s);"),strFunc,strName,strObj1);

	}
	return mstr;
}


void CPointSurC3D::DrawRetro(CDC* pDC,CVisualParam *vp)
{
	CPointSur3D::DrawRetro(pDC,vp);
	if (S->bVisible)
	 {	COLORREF curC = RGB(0,0,0);
		CPen curP(PS_DOT,1,curC);
		CPen *oldP = pDC->SelectObject(&curP);
		pDC->MoveTo((CPoint)vp->ProjectPoint(S->Center));
		pDC->LineTo(Visual_pt);
		pDC->SelectObject(oldP);
	 }
}

BOOL CPointSur3D::IntersectPlanDroite(CVector4& PP1,CVector4& U,
										CVector4& s1,CVector4& nom,CVector4 *res)
{
	CVector4	VvW	= nom,
				AB 	= s1 - PP1;

	FCoord n = VvW.Norme();
	if (!n) return 0;
	VvW.x /= n;
	VvW.y /= n;
	VvW.z /= n;
	FCoord DotProd;
	if ( FCZero(DotProd = (U*VvW)) )
		return FALSE;

	FCoord lambda	= (AB*VvW) / DotProd;

	CVector4 M(	PP1.x + lambda*U.x,
					PP1.y + lambda*U.y,
					PP1.z + lambda*U.z,
					1);

	*res = M;
	return TRUE;
}


BOOL CPointSurC3D::MoveObject(CVisualParam *myVisuParam,UINT,CPoint MouseClic,CVector4& TempCpt)
{
	CVector4 res(MouseClic.x,MouseClic.y,0,1);
	//CVector4 ecran = GetPrjPt(myVisuParam,res);
	CVector4 ecran = myVisuParam->GetScreenProjection(res);
	CVector4 d1(res.x,res.y,0);
//	Vector4 d1(res.x,0,0);
	//d1 = GetPrjPt2(myVisuParam,d1);
	d1 = myVisuParam->GetScreenProjectionInf(d1);
//	Vector4*	tmp = IntersectPlanDroite(ecran,MakeVector(ecran,d1),
//															S->P1->Concept_pt,
//															S->P2->Concept_pt,
//															S->P3->Concept_pt);
//	CVector4*	tmp = IntersectPlanDroite(ecran,d1-ecran,
//															S->Center/*S->P1->Concept_pt*/,
//															S->VecNorm);
	CVector4 tmp;
	BOOL ret = IntersectPlanDroite(ecran,d1-ecran,S->Center,S->VecNorm,&tmp);
	if (!ret) return FALSE;

//	Vector4	U = MakeVector(S->P1->Concept_pt,S->P2->Concept_pt),
//				V = MakeVector(S->P1->Concept_pt,S->P3->Concept_pt),
//				UvV = VectProduct(U,V);
//	FCoord r = Norme(U);
	CVector4 UvV = S->VecNorm;
	FCoord r = S->Radius;

	if (UvV.NullVector())
		return FALSE;

	CLocalRep rep = S->LocRep;
//	FCoord normK = 1 / Norme(UvV);
//	Vector4	K = RealMult(normK,UvV);
//	Vector4	I = MakeVector(S->Center,S->P1->Concept_pt);
//	normK = Norme(I);
//	I = RealMult(1/normK,I);
//	Vector4	J = VectProduct(K,I);


/*	Vector4	J(-K.y,K.x,0,1);
//	Vector4 J= U;
	normK = Norme(J);
	if (FCZero(normK))
		{ J.x = J.z =0; J.y = 1;	}
	else
		J = RealMult(1/normK,J);
	Vector4	I = VectProduct(J,K);*/
/*	lambda = DotProduct(I,MakeVector(S->P1->Concept_pt,*tmp));
	normK = 1 / Norme(MakeVector(S->P1->Concept_pt,*tmp));  //cos
	lambda = acosl(lambda*normK);
	FCoord sl = sinl(lambda);
	if (sl < 0) lambda += M_PI;
	U = Vector4(r*cosl(lambda),r*sinl(lambda),0,1);
	//Vector4 V;
	V.x=	I.x*U.x + J.x*U.y + K.x*U.z + S->P1->Concept_pt.x,
	V.y=	I.y*U.x + J.y*U.y + K.y*U.z + S->P1->Concept_pt.y,
	V.z=	I.z*U.x + J.z*U.y + K.z*U.z + S->P1->Concept_pt.z,
	V.w=	1;

	TempCpt = V;*/
	//lambda *= normK;
	CVector4 U = tmp - S->Center;

	U = U * (1/U.Norme());
	FCoord sa = rep.I * U;
	FCoord ca = rep.J * U;
	if (fabsl(sa) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			sa = (sa < 0.00) ? -1.00 : 1.00;
			//return 0;
		}
	//FCoord ltmp = lambda;
	lambda = r / U.Norme();
	CVector4 V = S->Center + U*lambda;

	lambda = acosl(sa);

	if (ca <0) lambda = 2*M_PI - lambda;

	if (S->isA() == TArcCercle3DClass)
	 {
		FCoord test = S->nArcAngle;

		if (lambda  > (test+1E-5))
		 {
			FCoord lim = test +(2*M_PI - test)/2;
			if (lambda < lim)
				lambda = test;
			else lambda = 0.;
		 }
		//if (ca <0)
		//	lambda = 0;
		//if (lambda < 0.) lambda = 0.;
		//if ((lambda <0) || (lambda > (test + 1E-5)))
		//	return 0;
	 }


/*		Vector4 	P0(0,0,0);
		Vector4 	P1 = Vector4(r*cosl(lambda),r*sinl(lambda),0,1);
		Vector4	A = Vector4(r*cosl(0.),r*sinl(0.),0,1);
		Vector4	B = Vector4(r*cosl(S->ArcAngle),r*sinl(S->ArcAngle),0,1);
		FCoord 	a1 = (A.y - P0.y)*(P1.x - P0.x) - (A.x - P0.x)*(P1.y - P0.y);
		FCoord 	b1 = (B.y - P0.y)*(P1.x - P0.x) - (B.x - P0.x)*(P1.y - P0.y);
	 }*/

//	FCoord cb = cosl(lambda);
//	FCoord sb = sinl(lambda);
	TempCpt = V;


	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSurP3D, CPointSur3D, VERSIONABLE_SCHEMA | 1)

CPointSurP3D::CPointSurP3D() : CPointSur3D() 
{
	P = NULL;
	alpha = beta = gamma = 0.;

}

CPointSurP3D::CPointSurP3D(CPlan3D *s1) : CPointSur3D()
{
	P = s1;
	nDepth = s1->nDepth+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtConst;
}

CPointSurP3D::CPointSurP3D(const CObject3D & src) : CPointSur3D(src)
{
	P =   ((CPointSurP3D&)src).P;
	alpha = ((CPointSurP3D&)src).alpha;
	beta = ((CPointSurP3D&)src).beta;
	gamma = ((CPointSurP3D&)src).gamma;
}

int CPointSurP3D::SetDepth()
{
	if (P)
		nDepth = P->nDepth+1;
	return nDepth;
}

CxSchemeSet* CPointSurP3D::GetRedefineSchemes(CxSchemeSet* pSet) 
{ 
	pSet = CPointSur3D::GetRedefineSchemes(pSet) ;
	if (!pSet) pSet = new CxSchemeSet();

	/// Redefine the point on line as a intersection Dr/Pl
	CRedefineScheme *pObj = new CRedefineScheme();
	pObj->nRedefStep = 1;
	pObj->nObjResID = CTX_INTERSECTION_LP;
	pObj->nAvail = ID_CONSTRUCTION_INTERSECTION_LINEPLANE;
	pObj->nStepID[0] = TAllDroiteClass;
	pObj->nTaskResID[0] = CTX_SELECT_DROITE;
	pObj->pNewObj = new CPointInterDP3D();
	((CPointInterDP3D*)pObj->pNewObj)->P = P;
	pSet->Add(pObj);

	return pSet;
}


void CPointSurP3D::Serialize( CArchive& ar )
{
	CPointSur3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << alpha;
		ar << beta;
		ar << gamma;
		ar << ((P) ? P->nObjectId : -1);
	}
	else
	{
		ar >> alpha;
		ar >> beta;
		ar >> gamma;
		P = (CPlan3D*)SerializeObj(ar);
	}
}

CObject3D* CPointSurP3D::CopyObject()
{
	CObject3D *temp = new CPointSurP3D((CObject3D&)*this);
	return temp;
}

CxObject3DSet* CPointSurP3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(P);
	return list;
}

BOOL CPointSurP3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet) return FALSE;
	int nb = pSet->GetSize();

	if (nb == 1 && !P)
	{
		P = (CPlan3D*)pSet->GetAt(0);
	}
	else if (!nb && P)
	{
	}
	else return FALSE;
	return TRUE;
}



void CPointSurP3D::CopyPointPosition(CObject3D* src)
{
	CPointSur3D::CopyPointPosition(src);
	if (!src) return;
	if (src->MaskObject(isA()))
	{
		alpha =  ((CPointSurP3D*)src)->alpha;
		beta =  ((CPointSurP3D*)src)->beta;
		gamma =  ((CPointSurP3D*)src)->gamma;
		//Concept_pt =  ((CPoint3D*)src)->Concept_pt;
	 }


}

void CPointSurP3D::CopyPointPosition(CVector4 pt)
{
	CVector4	//U = MakeVector(P->P1->Concept_pt,P->P2->Concept_pt),
				//V = MakeVector(P->P1->Concept_pt,P->P3->Concept_pt),
				W = pt - P->P1->Concept_pt,
				UvV = P->VecNorm;//VectProduct(U,V);

	FCoord normK = 1 / UvV.Norme();
	CVector4	K = UvV * normK;
	CVector4	J(-K.y,K.x,0,1);
	normK = J.Norme();
	if (FCZero(normK))
		{ J.x = J.z =0; J.y = 1;	}
	else
		J = J * (1/normK);
	CVector4	I = J % K;


	I = P->ptonRep.I;
	J = P->ptonRep.J;
	CVector4	O = P->ptonRep.O;

	//I1 = I1 * (1/I1.Norme());
	//J1 = J1 * (1/J1.Norme());


	//U = RealMult(1/Norme(U),U);
	//V = RealMult(1/Norme(V),V);
	if (W.NullVector())
	 {
		alpha = beta = 0.;
	 }
	else
	 {	//W = RealMult(1/Norme(W),W);
		alpha = I * W;
		beta = J * W;
	 }
}


CString CPointSurP3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???"));
	if (P) sn1 = P->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1);
	return mstr;
}


/*BOOL TangenteInters(CVector4& p3,CVector4& p1,CVector4& p2,CVector4& inter)
{
	CVector4 p1p2 = p2 - p1;
	FCoord denom = p1p2.Norme();
	if (p1p2.NullVector())	return FALSE;
	denom = denom*denom;

	FCoord nom =
		(p3.x - p1.x)*(p2.x-p1.x) +
		(p3.y - p1.y)*(p2.y-p1.y) +
		(p3.z - p1.z)*(p2.z-p1.z);
	denom = nom / denom;
	inter = CVector4(	p1.x + denom*(p2.x-p1.x),
							p1.y + denom*(p2.y-p1.y),
							p1.z + denom*(p2.z-p1.z));
	return TRUE;
}*/


UINT  CPointSurP3D::CalculConceptuel()
{
	bValidate = (P->bValidate);
	if (!bValidate)
		return ERR_SUPPORTOBJECT;

	CVector4	//U = MakeVector(P->P1->Concept_pt,P->P2->Concept_pt),
				//V = MakeVector(P->P1->Concept_pt,P->P3->Concept_pt),
				UvV = P->VecNorm;//VectProduct(U,V);

	FCoord normK = 1 / UvV.Norme();
	CVector4	K = UvV * normK;
	CVector4	J(-K.y,K.x,0,1);
	normK = J.Norme();
	if (FCZero(normK))
		{ J.x = J.z =0; J.y = 1;	}
	else
		J = J * (1/normK);
	CVector4	I = J % K;
	CVector4	O = P->P1->Concept_pt;

//	CVector4 I = P->plRep.I;
//	CVector4 J = P->plRep.J;
//	CVector4 O = P->plRep.O;

//	I = I * (1/I.Norme());
//	J = J * (1/J.Norme());

	I = P->ptonRep.I;
	J = P->ptonRep.J;
	O = P->ptonRep.O;

	CVector4 TempCpt = O + (I * alpha) + (J * beta);
//	CVector4	TempCpt = P->P1->Concept_pt + 
//						  (I * alpha) + (J * beta);
	Concept_pt = TempCpt;
////****29/07	P->CalculConceptuel();
/*	Vector4 oeil = Concept_pt;
	Vector4 inter,TempCp;
	FCoord temp;

	TangenteInters(oeil,P->P1->Concept_pt,P->P2->Concept_pt,inter);
	TempCp = MakeVector(inter,oeil);
	temp = Norme(TempCp);
	if (temp != alpha)
		if (NullVector(TempCp))
		 {
			Concept_pt = inter;
		 }
		else
		 {
			TempCp = RealMult(1/temp,TempCp);
			Concept_pt = RealMult(alpha,TempCp);
			Concept_pt = Concept_pt + inter;
		 }
	TangenteInters(oeil,P->P1->Concept_pt,P->P3->Concept_pt,inter);
	TempCp = MakeVector(inter,oeil);
	temp = Norme(TempCp);
	if (temp != beta)
		if (NullVector(TempCp))
		 {
			Concept_pt = inter;
		 }
		else
		 {
			TempCp = RealMult(1/temp,TempCp);
			Concept_pt = RealMult(beta,TempCp);
			Concept_pt = Concept_pt + inter;
		 }
	TangenteInters(oeil,P->P2->Concept_pt,P->P3->Concept_pt,inter);
	TempCp = MakeVector(inter,oeil);
	temp = Norme(TempCp);
	if (temp != gamma)
		if (NullVector(TempCp))
		 {
			Concept_pt = inter;
		 }
		else
		 {
			TempCp = RealMult(1/temp,TempCp);
			Concept_pt = RealMult(gamma,TempCp);
			Concept_pt = Concept_pt + inter;
		 }

	//Concept_pt = TempCpt;*/
	return 0;
}

CString CPointSurP3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (/*bValidate && */P)
	{
        CString strName,strObj1;
		strName = GetObjectNameRedux();
		strObj1 = P->GetObjectNameRedux();

		CObjectId nType = P->isA();
		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("PointOnPlane[%s,%s];"),strName,strObj1);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("PointOnPlane(%s,%s);"),strName,strObj1);

	}
	return mstr;
}



	
void CPointSurP3D::DrawRetro(CDC* pDC,CVisualParam *mV)
{
	CPointSur3D::DrawRetro(pDC,mV);
	if (P->bVisible)
	 {
		CVector4 M  	= (P->p2 - P->p1) * .5;
		M = P->p1 + M;
		CVector4 dir = P->p3 - P->p2;
		CPoint3D Ap1(M);
		CPoint3D Ap2(M+dir);
		CSegment3D Mdir(&Ap1,&Ap2);
		Mdir.CalculConceptuel();

		CDroitePar3D parp1p2(this,&Mdir);
		parp1p2.CalculConceptuel();

		CPoint3D Bp1(P->p1);
		CPoint3D Bp2(P->p2);
		CDroite3D p1p2(&Bp1,&Bp2);
		p1p2.CalculConceptuel();
		CPointInterDD3D inter(&p1p2,&parp1p2);
		inter.CalculConceptuel();

		CPoint deb =(CPoint)mV->ProjectPoint(inter.Concept_pt);
		CPoint fin =(CPoint)mV->ProjectPoint(inter.Concept_pt+dir);

		COLORREF curC = RGB(0,0,0);
		CPen curP(PS_DOT,1,curC);
		CPen *oldP = pDC->SelectObject(&curP);
		pDC->MoveTo(deb);
		pDC->LineTo(fin);
		pDC->SelectObject(oldP);
	 }

}

BOOL CPointSurP3D::MoveObject(CVisualParam *myVisuParam,UINT,CPoint MouseClic,CVector4& TempCpt)
{
	CVector4 res(MouseClic.x,MouseClic.y,0,1);
	//CVector4 ecran = GetPrjPt(myVisuParam,res);
	CVector4 ecran = myVisuParam->GetProjectedPoint(MouseClic);
	CPoint aa = (CPoint) myVisuParam->ProjectPoint(ecran);
	CVector4 oeil= myVisuParam->GetEyePos();
	CVector4 origin(0,0,0,1);
	CVector4 VisuNorm= oeil - ((myVisuParam->bParProj) ?
				origin : origin);
	CVector4 ecran2 = ecran + VisuNorm;
	CPoint3D x(ecran);
	CPoint3D y(ecran + VisuNorm);
	CDroite3D dxy(&x,&y);
	dxy.CalculConceptuel();
	CPointInterDP3D dp(&dxy,(CPlan3D*)P);
	UINT r = dp.CalculConceptuel();
	if (r)	return 0;

	CVector4 ppt = dp.Concept_pt;
	P->IsPointInside(ppt,FALSE);
	dp.Concept_pt = ppt;
	TempCpt = ppt;


	/*oeil = dp.Concept_pt;
	Vector4 inter;
	TangenteInters(oeil,P->P1->Concept_pt,P->P2->Concept_pt,inter);
	inter = MakeVector(inter,oeil);
	alpha = Norme(inter);
	TangenteInters(oeil,P->P1->Concept_pt,P->P3->Concept_pt,inter);
	inter = MakeVector(inter,oeil);
	beta = Norme(inter);
	TangenteInters(oeil,P->P2->Concept_pt,P->P3->Concept_pt,inter);
	inter = MakeVector(inter,oeil);
	gamma = Norme(inter);*/
	//aa = (TPoint) myVisuParam->ProjectPoint(oeil);
	oeil = dp.Concept_pt;
	CVector4	//U = MakeVector(P->P1->Concept_pt,P->P2->Concept_pt),
				//V = MakeVector(P->P1->Concept_pt,P->P3->Concept_pt),
				W = oeil - P->P1->Concept_pt,
				UvV = P->VecNorm;//VectProduct(U,V);

	FCoord normK = 1 / UvV.Norme();
	CVector4	K = UvV * normK;
	CVector4	J(-K.y,K.x,0,1);
	normK = J.Norme();
	if (FCZero(normK))
		{ J.x = J.z =0; J.y = 1;	}
	else
		J = J * (1/normK);
	CVector4	I = J % K;


	I = P->ptonRep.I;
	J = P->ptonRep.J;
 CVector4	O = P->ptonRep.O;

	//I1 = I1 * (1/I1.Norme());
	//J1 = J1 * (1/J1.Norme());


	//U = RealMult(1/Norme(U),U);
	//V = RealMult(1/Norme(V),V);
	if (W.NullVector())
	 {
		alpha = beta = 0.;
	 }
	else
	 {	//W = RealMult(1/Norme(W),W);
		alpha = I * W;
		beta = J * W;
	 }
	CVector4	oeil2 = P->P1->Concept_pt+ (I*alpha) + (J*beta);
	////****29/07

	P->UpdateContour();

	//TempCpt = oeil2;
	return 1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSurS3D, CPointSur3D, VERSIONABLE_SCHEMA | 1)

CPointSurS3D::CPointSurS3D() : CPointSur3D() 
{
	S = NULL;
	latit = longi = 0.;
	front = first = 0;
};

CPointSurS3D::CPointSurS3D(CSphere3D *s1) : CPointSur3D()
{
	S = s1;
	nDepth = s1->nDepth+1;
	latit = longi = 0.;
	front = first = 0;
	pObjectShape.nShapeId = TPref::TPoint.nPtConst;
}

CPointSurS3D::CPointSurS3D(const CObject3D & src ) : CPointSur3D(src)
{
	S =   ((CPointSurS3D&)src).S;
	latit = longi = 0.;
	front = first = 0;
}

int CPointSurS3D::SetDepth()
{
	if (S)
		nDepth = S->nDepth+1;
	return nDepth;
}



void CPointSurS3D::Serialize( CArchive& ar )
{
	CPointSur3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << latit;
		ar << longi;
		ar << first;
		ar << front;
		ar << ((S) ? S->nObjectId : -1);
	}
	else
	{
		ar >> latit;
		ar >> longi;
		ar >> first;
		ar >> front;
		S = (CSphere3D*)SerializeObj(ar);
	}
}


CObject3D* CPointSurS3D::CopyObject()
{
	CObject3D *temp = new CPointSurS3D((CObject3D&)*this);
	return temp;
}

CxObject3DSet* CPointSurS3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(S);
	return list;
}

BOOL CPointSurS3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet || S) return FALSE;
	int nb = pSet->GetSize();

	if (nb!=1) return FALSE;

	S = (CSphere3D*)pSet->GetAt(0);
	return TRUE;
}


void CPointSurS3D::CopyPointPosition(CObject3D* src )
{
	CPointSur3D::CopyPointPosition(src);
	if (!src) return;
	if (src->MaskObject(isA()))
	 {
		//Concept_pt =  ((CPointSurS3D*)src)->Concept_pt;
		latit =  ((CPointSurS3D*)src)->latit;
		longi =  ((CPointSurS3D*)src)->longi;
		front =  ((CPointSurS3D*)src)->front;
		first =  ((CPointSurS3D*)src)->first;
	 }

}

void CPointSurS3D::CopyPointPosition(CVector4 pt)
{
	CVector4 p3 = S->P1->Concept_pt;
	CVector4 M(pt);
	CVector4 I(pt);
	I.z = p3.z;
	CVector4 OM = M - p3;
	OM = OM * (1/OM.Norme());
	CVector4 OI = I - p3;
	OI = OI * (1/OI.Norme());
	FCoord ac = OI * OM;

	if (fabsl(ac) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			ac = (ac < 0.00) ? -1.00 : 1.00;
			//return 0;
		}


	ac = acosl(ac);
	latit = (M.z < p3.z) ? -ac : ac;
	CVector4 OH(1,0,0);
	FCoord as = OH * OI;

	if (fabsl(as) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			as = (as < 0.00) ? -1.00 : 1.00;
			//return 0;
		}


	as = acosl(as);
	longi = (I.y < p3.y) ? -as : as;
}

CString CPointSurS3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???"));
	if (S) sn1 = S->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1);
	return mstr;
}

UINT  CPointSurS3D::CalculConceptuel()
{
	bValidate = (S->bValidate);
	if (!bValidate)
		return ERR_SUPPORTOBJECT;

	FCoord 	r = S->Rayon.Norme();
	CVector4	P1 = S->P1->Concept_pt;
	CVector4 	I(1,0,0,1);
	CVector4 	J(0,1,0,1);
	CVector4 	K(0,0,1,1);

	CVector4 V(r*cosl(longi),r*sinl(longi),0,1);
	CVector4 M = V+P1;

	I = V * (1/V.Norme());
	K = CVector4(I.y,-I.x,0,1);
	K = K * (1/K.Norme());
	J = K % I;

	CVector4 U(r*cosl(latit),r*sinl(latit),0,1);
	V.x=	I.x*U.x + J.x*U.y + K.x*U.z + P1.x,
	V.y=	I.y*U.x + J.y*U.y + K.y*U.z + P1.y,
	V.z=	I.z*U.x + J.z*U.y + K.z*U.z + P1.z,
	V.w=	1;

	Concept_pt = V;
	bValidate=1;
	return 0;
}

//////////////////////////////////////////////////////////////////////
/// Method CPointSur3D::InterSphRay
///
/// \param raybase (CVector4)	Base of the intersection ray
/// \param raycos (CVector4)	Direction cosines of the ray
/// \param center (CVector4)	Center of the sphere	
/// \param radius (FCoord)		Radius of the sphere
/// \param rin (FCoord)			Entering distance
/// \param rout (FCoord)		Leaving distance
/// \return						TRUE if ray intersects sphere, FALSE otherwise
//////////////////////////////////////////////////////////////////////
BOOL CPointSur3D::IntersectSphereRay(CVector4 raybase,CVector4 raycos,CVector4 center,
						FCoord radius,FCoord *rin,FCoord* rout)
{
	BOOL	hit;			/* True if ray intersects sphere*/
	FCoord	dx, dy, dz;		/* Ray base to sphere center	*/
	FCoord	bsq, u, disc;
	FCoord	root;


	dx   = raybase.x - center.x;
	dy   = raybase.y - center.y;
	dz   = raybase.z - center.z;
	bsq  = dx*raycos.x + dy*raycos.y + dz*raycos.z;
	u    = dx*dx + dy*dy + dz*dz - radius*radius;
	disc = bsq*bsq - u;

	hit  = (disc >= 0.0);
	if  (!hit)
		disc = 0.0;

	//if  (!hit) { 				/* If ray hits sphere	*/
	root  =  sqrt(disc);
	*rin  = -bsq - root;		/*    entering distance	*/
	*rout = -bsq + root;		/*    leaving distance	*/
	//}

	return (hit);
}


BOOL CPointSurS3D::MoveObject(CVisualParam *myVisuParam,UINT,CPoint MouseClic ,CVector4& TempCpt )
{
	CVector4 res(MouseClic.x,MouseClic.y,0,1);
	//CVector4 p1 = GetPrjPt(myVisuParam,res);
	CVector4 p1 = myVisuParam->GetScreenProjection(res);
	CVector4 p2(res.x,res.y,0);
	//p2 = GetPrjPt2(myVisuParam,p2);
	p2 = myVisuParam->GetScreenProjectionInf(p2);

	CVector4 t  = p2 - p1;
	t = t * (1/t.Norme());
	CVector4 p3 = S->P1->Concept_pt;
	FCoord r = S->Rayon.Norme();
	FCoord rin,rout;
	BOOL test = IntersectSphereRay(p1,t,p3,r,&rin,&rout);
	if (!test)
	 {
		if (!first)
		 {
			front = !front;
			first =1;
		 }
//		return 0;
	 }
	else first = 0;

	t = t * ((front? rin : rout)/t.Norme());
	TempCpt = p1+t;

	CVector4 M(TempCpt);
	CVector4 I(TempCpt);
	I.z = p3.z;
	CVector4 OM = M - p3;
	OM = OM * (1/OM.Norme());
	CVector4 OI = I - p3;
	OI = OI * (1/OI.Norme());
	FCoord ac = OI * OM;

	if (fabsl(ac) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			ac = (ac < 0.00) ? -1.00 : 1.00;
			//return 0;
		}


	ac = acosl(ac);
	latit = (M.z < p3.z) ? -ac : ac;
	CVector4 OH(1,0,0);
	FCoord as = OH * OI;

	if (fabsl(as) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			as = (as < 0.00) ? -1.00 : 1.00;
			//return 0;
		}


	as = acosl(as);
	longi = (I.y < p3.y) ? -as : as;

	return 1;
}

CString CPointSurS3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (/*bValidate && */S)
	{
        CString strName,strObj1;
		strName = GetObjectNameRedux();
		strObj1 = S->GetObjectNameRedux();

		CObjectId nType = S->isA();
		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("PointOnSphere[%s,%s];"),strName,strObj1);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("PointOnSphere(%s,%s);"),strName,strObj1);
	}
	return mstr;
}



void CPointSurS3D::DrawRetro(CDC* pDC,CVisualParam *mV)
{
	CPointSur3D::DrawRetro(pDC,mV);
	if (!S->bVisible) return;

	CVector4 P3 = Concept_pt;
	P3.z = S->P1->Concept_pt.z;
	CVector4 R = P3 - S->P1->Concept_pt;
	FCoord ptn= R.Norme();
	FCoord r = S->Rayon.Norme();

	if (!FCZero(ptn))
	 {
		COLORREF curC = RGB(0,0,0);
		CPen curPen(PS_DOT,1,curC);
		CPen disPen(PS_DOT,1,curC);
		CPen disPenH(PS_DOT,1,RGB(192,192,192));
		CPen curPenH(PS_DOT,1,RGB(192,192,192));

		CVector4 J2 = R * (1/ptn);
		CVector4 K2 = CVector4(J2.y,-J2.x,0,1);
		FCoord normK  = K2.Norme();
		K2 = K2 * (1/normK);
		CVector4 I2 = K2 % J2;
		CVector4 /*a3,*/d3,f3;
		CSphere3D::CalcUnit(I2,J2,K2,S->P1->Concept_pt,0,r,d3);
		//a3=d3;
		for (int t=1;t<(1+(TPref::TCercle.nDelta)/2);t++)
		 {
			CSphere3D::CalcUnit(I2,J2,K2,S->P1->Concept_pt,t,r,f3);
			CSphere3D::DrawUnit(d3,f3,S->P1->Concept_pt,pDC,mV,curPen,curPen,curPen,curPen);
			d3=f3;
		 }
		//DrawUnit(a3,f3,S->P1->Concept_pt,HandleDC,mV,curPen,curPenH,disPen,disPenH);
	 }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSurCyl3D, CPointSur3D, VERSIONABLE_SCHEMA | 1)

CPointSurCyl3D::CPointSurCyl3D() : CPointSur3D() 
{
	Cyl = NULL;
	alpha= 0.;
	gamma = 0.;
	beta = 0.;
	front = first = 0;
	pt1 = CVector4(0,0,0,0);
	pt2 = CVector4(0,0,0,0);
}

CPointSurCyl3D::CPointSurCyl3D(CCylinder3D *s1): CPointSur3D()
{
	Cyl = s1;
	nDepth = s1->nDepth+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtConst;
	alpha= 0.;
	gamma = 0.;
	beta = 0.;
	front = first = 0;
	pt1 = CVector4(0,0,0,0);
	pt2 = CVector4(0,0,0,0);
}

CPointSurCyl3D::CPointSurCyl3D(const CObject3D & src): CPointSur3D(src)
{
	Cyl =   ((CPointSurCyl3D&)src).Cyl;
	alpha = ((CPointSurCyl3D&)src).alpha;
	gamma = ((CPointSurCyl3D&)src).gamma;
	beta = ((CPointSurCyl3D&)src).beta;
	front = first = 0;
}

int CPointSurCyl3D::SetDepth()
{
	if (Cyl)
		nDepth = Cyl->nDepth+1;
	return nDepth;
}




void CPointSurCyl3D::Serialize( CArchive& ar )
{
	CPointSur3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << alpha;
		ar << beta;
		ar << gamma;
		ar << ((Cyl) ? Cyl->nObjectId : -1);
	}
	else
	{
		ar >> alpha;
		ar >> beta;
		ar >> gamma;
		Cyl = (CCylinder3D*)SerializeObj(ar);
		pt1 = CVector4(0,0,0,0);
		pt2 = CVector4(0,0,0,0);
	}
}


CObject3D* CPointSurCyl3D::CopyObject()
{
	CObject3D *temp = new CPointSurCyl3D((CObject3D&)*this);
	return temp;
}

CxObject3DSet* CPointSurCyl3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(Cyl);
	return list;
}

BOOL CPointSurCyl3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet || Cyl) return FALSE;
	int nb = pSet->GetSize();

	if (nb != 1 ) return FALSE;

	Cyl = (CCylinder3D*)pSet->GetAt(0);
	
	return TRUE;
}

void CPointSurCyl3D::CopyPointPosition(CVector4 pt)
{
}


void CPointSurCyl3D::CopyPointPosition(CObject3D* src)
{
	CPointSur3D::CopyPointPosition(src);
	if (!src) return;
	if (src->MaskObject(isA()))
	{
		//Concept_pt =  ((CPoint3D*)src)->Concept_pt;
		alpha =  ((CPointSurCyl3D*)src)->alpha;
		beta =  ((CPointSurCyl3D*)src)->beta;
		gamma =  ((CPointSurCyl3D*)src)->gamma;
	}
}

CString CPointSurCyl3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???"));
	if (Cyl) sn1 = Cyl->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1);
	return mstr;
}


UINT  CPointSurCyl3D::CalculConceptuel()
{
	bValidate = (Cyl->bValidate);
	if (!bValidate)
		return ERR_SUPPORTOBJECT;

	CVector4 p1 = Cyl->Base1->Concept_pt;
	CVector4 p2 = Cyl->Base2->Concept_pt;

	CVector4 p1p2 = p2 - p1;

	FCoord rad = Cyl->nRayon;
	if (p1p2.NullVector() || FCZero(rad))
	 {	
		bValidate = FALSE;
		return ERR_SUPPORTOBJECT;
	 }

	CVector4 M(0,0,0);

//	M = p1p2 * (beta);
//	M = M + p1;

	CLocalRep rep = Cyl->LocRep;


	FCoord dd = Cyl->LocRep.K * (Concept_pt - Cyl->LocRep.O);
	CVector4	U = CVector4(0,0,dd,1);
	M = rep.O;
	M.x=	rep.I.x*U.x + rep.J.x*U.y + rep.K.x*U.z + M.x;
	M.y=	rep.I.y*U.x + rep.J.y*U.y + rep.K.y*U.z + M.y;
	M.z=	rep.I.z*U.x + rep.J.z*U.y + rep.K.z*U.z + M.z;
	M.w=	1;

	CVector4 hh = Concept_pt - M;
	hh =hh.Normalized();

	hh = hh * Cyl->nRayon;
	hh = M + hh;

	alpha= rep.I * (hh-rep.O);
	beta = rep.J * (hh-rep.O);

	FCoord ff = (Cyl->Base2->Concept_pt - Cyl->Base1->Concept_pt).Norme();

	M = rep.O;
//	CVector4	U = CVector4(rad*cosl(lambda),rad*sinl(lambda),0,1);
	U = CVector4(alpha,beta,gamma,1);
	M.x=	rep.I.x*U.x + rep.J.x*U.y + rep.K.x*U.z*ff + M.x;
	M.y=	rep.I.y*U.x + rep.J.y*U.y + rep.K.y*U.z*ff + M.y;
	M.z=	rep.I.z*U.x + rep.J.z*U.y + rep.K.z*U.z*ff + M.z;
	M.w=	1;

	Concept_pt = M;

	bValidate=1;
	return 0;
}

CString CPointSurCyl3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (/*bValidate && */Cyl)
	{
        CString strName,strObj1;
		strName = GetObjectNameRedux();
		strObj1 = Cyl->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("PointOnCylinder[%s,%s];"),strName,strObj1);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("PointOnCylinder(%s,%s);"),strName,strObj1);
	}
	return mstr;
}


void CPointSurCyl3D::DrawRetro(CDC* pDC,CVisualParam *vp)
{
	CPointSur3D::DrawRetro(pDC,vp);

/*	if (pt1 == CVector4(0,0,0,0)) return;

	CPoint ptt1 = (CPoint)vp->ProjectPoint(pt1);
	CPoint ptt2 = (CPoint)vp->ProjectPoint(pt2);

	CRect mrect(ptt1,ptt1);
	mrect.InflateRect(5,5);

	pDC->Rectangle(mrect);

	mrect = CRect (ptt2,ptt2);
	mrect.InflateRect(2,2);

	pDC->Rectangle(mrect);

	pDC->MoveTo((CPoint)vp->ProjectPoint(Cyl->LocRep.O));
	pDC->LineTo((CPoint)vp->ProjectPoint(Cyl->LocRep.O + Cyl->LocRep.I*Cyl->nRayon));

	ptt2 = (CPoint)vp->ProjectPoint(Cyl->LocRep.O + Cyl->LocRep.I*Cyl->nRayon);

	mrect = CRect (ptt2,ptt2);
	mrect.InflateRect(2,2);

	pDC->Rectangle(mrect);



	pDC->MoveTo((CPoint)vp->ProjectPoint(Cyl->LocRep.O));
	pDC->LineTo((CPoint)vp->ProjectPoint(Cyl->LocRep.O + Cyl->LocRep.J*Cyl->nRayon));

	pDC->MoveTo((CPoint)vp->ProjectPoint(Cyl->LocRep.O));
	pDC->LineTo((CPoint)vp->ProjectPoint(Cyl->LocRep.O + Cyl->LocRep.K*Cyl->nRayon));*/

	FCoord dd = Cyl->LocRep.K * (Concept_pt - Cyl->LocRep.O);
	//CVector4	U = CVector4(0,0,dd,1);
	//CLocalRep rep = Cyl->LocRep;
	
	CVector4	M = Cyl->LocRep.O + Cyl->LocRep.K * dd ;

//	M.x=	rep.K.x*U.z + M.x;
//	M.y=	rep.K.y*U.z + M.y;
//	M.z=	rep.K.z*U.z + M.z;
//	M.w=	1;

	CPoint3D	mPC(M);
	mPC.CalculConceptuel();
	//mPC.CalculVisuel(vp);
	//mPC.Draw(pDC,vp);

	FCoord pas = 15.*Cyl->nRayon/70;
	pas = min(max(20,pas),70);

	CCercle3D mCC(&mPC,CVector4(1,0,0),1.);
	mCC.pObjectShape.nShapeId = 0;
	mCC.pObjectShape.clrObject = RGB(0,0,0);
	mCC.nDeltaT = (int)pas;
	mCC.Radius = Cyl->nRayon;
	mCC.VecNorm = Cyl->LocRep.K;
	mCC.CalculConceptuel();
	mCC.CalculVisuel(vp);
	mCC.Draw(pDC,vp,TRUE);
}


//////////////////////////////////////////////////////////////////////
/// Method CPointSur3D::IntersectCylindreRay
///
/// \param raybase (CVector4)	Base of the intersection ray
/// \param raycos (CVector4)	Direction cosines of the ray
/// \param base (CVector4)		Base of the cylinder
/// \param axis (CVector4)		axis of the cylinder
/// \param radius (FCoord)		Radius of the cylinder
/// \param rin (FCoord)			Entering distance
/// \param rout (FCoord)		Leaving distance
/// \return						TRUE if ray intersects the cylinder, FALSE otherwise
//////////////////////////////////////////////////////////////////////
BOOL CPointSur3D::IntersectCylindreRay(CVector4 raybase,CVector4 raycos,CVector4 base,CVector4 axis,
						FCoord radius,FCoord *rin,FCoord* rout)
{
	BOOL		hit;	// True if ray intersects cyl
	CVector4	RC;		// Ray base to cylinder base
	double		d;		// Shortest distance between the ray and the cylinder
	double		t, s;	// Distances along the ray
	CVector4		n, D, O;
	double		ln;
	const	double		pinf = HUGE;	/* Positive infinity		*/


	RC.x = raybase.x - base.x;
	RC.y = raybase.y - base.y;
	RC.z = raybase.z - base.z;

	n = raycos % axis;
	//V3Cross (raycos,axis,&n);

	ln = n.Norme();
	if  ( ln == 0. ) {	/* ray parallel to cyl	*/
	//if  ( (ln = V3Length (&n)) == 0. ) {	/* ray parallel to cyl	*/
	  //  d	 = V3Dot (&RC,axis);
	    d	 = RC * axis;
	    D.x	 = RC.x - d*axis.x;
	    D.y	 = RC.y - d*axis.y;
	    D.z	 = RC.z - d*axis.z;
	    //d	 = V3Length (&D);
	    d	 = D.Norme();
	    *rin	 = -pinf;
	    *rout =  pinf;
	    return (d <= radius);		/* true if ray is in cyl*/
	}

	//V3Normalize (&n);
	n = n.Normalized();
	//d    = fabs (V3Dot (&RC,&n));		/* shortest distance	*/
	d    = fabs (RC * n);		/* shortest distance	*/
	hit  = (d <= radius);

	if (d>= radius) d = radius;
//	if  (hit) {				/* if ray hits cylinder */
	    //V3Cross (&RC,axis,&O);
		O = RC % axis;

	    //t = - V3Dot (&O,&n) / ln;
	    t = - (O * n) / ln;
	    //V3Cross (&n,axis,&O);

		O = n % axis;

	    //V3Normalize (&O);
		O = O.Normalized();

	    //s = fabs (sqrt(radius*radius - d*d) / V3Dot (raycos,&O));
		//if (hit)
			s = fabs (sqrt(radius*radius - d*d) / (raycos * O));
		//else 
		//	s=0;
	    *rin	 = t - s;			/* entering distance	*/
	    *rout = t + s;			/* exiting  distance	*/
//	}
//	else
//	{
//	    *rin	 = t ;			/* entering distance	*/
//	    *rout = t ;			/* exiting  distance	*/
//	}

	return (hit);
}

BOOL CPointSurCyl3D::MoveObject(CVisualParam *myVisuParam,UINT,CPoint MouseClic,CVector4& TempCpt)
{
	CVector4 res(MouseClic.x,MouseClic.y,0,1);
	//CVector4 p1 = GetPrjPt(myVisuParam,res);
	CVector4 p1 = myVisuParam->GetScreenProjection(res);
	CVector4 p2(res.x,res.y,0);
	//p2 = GetPrjPt2(myVisuParam,p2);
	p2 = myVisuParam->GetScreenProjectionInf(p2);

	CVector4 t  = p2 - p1;
	t = t * (1/t.Norme());

	CVector4 p3 = Cyl->Base1->Concept_pt;
	CVector4 p4 = Cyl->Base2->Concept_pt;
	CVector4 p3p4 = p4 - p3;
	p3p4 = p3p4.Normalized();

	FCoord rad = Cyl->nRayon;
	FCoord rin,rout;


	BOOL test = IntersectCylindreRay(p1,t,p3,p3p4,rad,&rin,&rout);

	if (!test)
	 {
		if (!first)
		 {
			front = !front;
			first =1;
		 }
		//return 0;
	 }
	else first = 0;

	FCoord gg = t.Norme();
	pt1 = p1+t * rin;
	pt2 = p1+t * rout;
	//t = t * (rin/t.Norme());
	//t = t * ((front? rin : rout)/t.Norme());

	TempCpt = p1+t * (front? rin : rout);


	CLocalRep rep = Cyl->LocRep;

	if (test)
	{
		alpha = rep.I * (TempCpt-rep.O);
		beta = rep.J * (TempCpt-rep.O);
	}
	gamma = rep.K * (TempCpt-rep.O);

/*	FCoord dd = Cyl->LocRep.K * (Concept_pt - Cyl->LocRep.O);
	CVector4	U = CVector4(0,0,dd,1);
	CVector4	M = rep.O;
	M.x=	rep.I.x*U.x + rep.J.x*U.y + rep.K.x*U.z + M.x;
	M.y=	rep.I.y*U.x + rep.J.y*U.y + rep.K.y*U.z + M.y;
	M.z=	rep.I.z*U.x + rep.J.z*U.y + rep.K.z*U.z + M.z;
	M.w=	1;

	CVector4 hh = TempCpt - M;
	hh =hh.Normalized();

	hh = hh * Cyl->nRayon;
	hh = M + hh;

	alpha = rep.I * (hh-rep.O);
	beta = rep.J * (hh-rep.O);*/


	FCoord ff = (Cyl->Base2->Concept_pt - Cyl->Base1->Concept_pt).Norme();

	gamma = gamma / ff;
	if (gamma < 0.) gamma = 0.;
	if (gamma > 1.) gamma = 1.;
	
	/*CVector4 AM = TempCpt - Cyl->Base1->Concept_pt;

	//AM = AM.Normalized();

	FCoord a = AM.Norme(),b = p3p4.Norme();
	FCoord dd = p3p4 * AM;
	beta = dd;//(p3p4.NullVector()) ? 0. : a/b;
	//if (dd < 0.0)
	//	beta = -beta;

//	beta = Cyl->IntH.K * AM;

	CVector4 A = Cyl->Base1->Concept_pt;
	CVector4 dir = p3p4;
	CVector4 AM2 = A - TempCpt;
	FCoord pos = dir * AM2;
	pos = (pos<0) ? -1. : 1.;
	beta = -pos*AM2.Norme();*/

/*	CVector4 AB = p4 - p3;
	CVector4 AM = TempCpt - p3;
	FCoord a = AM.Norme(),b = AB.Norme();
	FCoord dd = AB * AM;
	beta = (AB.NullVector()) ? 0. : a/b;
	if (dd < 0.0)
		beta = -beta;
	if ((beta > 1.000001) || (beta < -1E-12))
	{
		if (beta < 0.0) beta = 0.0;
		else beta = 1.0;
	}

	CLocalRep rep = Cyl->LocRep;
	CVector4 U = TempCpt - rep.O;
	U = U * (1/U.Norme());
	FCoord sa = rep.I * U;
	FCoord ca = rep.J * U;
	if (fabsl(sa) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			sa = (sa < 0.00) ? -1.00 : 1.00;
			//return 0;
		}
	//FCoord ltmp = lambda;
	lambda = rad / U.Norme();
	CVector4 V = p3 + U*lambda;

	lambda = acosl(sa);

//	if (ca <0) 
//		lambda = 2*M_PI - lambda;


/*	CLocalRep rep = Cyl->IntH;

	CVector4 U = TempCpt - Cyl->Base1->Concept_pt;
	U = U * (1/U.Norme());
	FCoord sa = rep.I * U;
	FCoord ca = rep.J * U;
	if (fabsl(sa) > 1.)
		{
			//::MessageBox(0,"erreur arcos","ici",MB_OK);
			sa = (sa < 0.00) ? -1.00 : 1.00;
			//return 0;
		}
	//FCoord ltmp = lambda;
	//lambda = r / U.Norme();
	//CVector4 V = S->Center + U*lambda;

	lambda = acosl(sa);

	if (ca <0) lambda = 2*M_PI - lambda;*/

	return 1;

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSurCone3D, CPointSur3D, VERSIONABLE_SCHEMA | 1)

CPointSurCone3D::CPointSurCone3D() : CPointSur3D() 
{
	Cone = NULL;
	alpha= 0.;
	gamma = 0.;
	beta = 0.;
	front = first = 0;
	pt1 = CVector4(0,0,0,0);
	pt2 = CVector4(0,0,0,0);
}

CPointSurCone3D::CPointSurCone3D(CCone3D *s1): CPointSur3D()
{
	Cone = s1;
	nDepth = s1->nDepth+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtConst;
	alpha= 0.;
	gamma = 0.;
	beta = 0.;
	front = first = 0;
	pt1 = CVector4(0,0,0,0);
	pt2 = CVector4(0,0,0,0);
}

CPointSurCone3D::CPointSurCone3D(const CObject3D & src): CPointSur3D(src)
{
	Cone =   ((CPointSurCone3D&)src).Cone;
	alpha = ((CPointSurCone3D)src).alpha;
	gamma = ((CPointSurCone3D&)src).gamma;
	beta = ((CPointSurCone3D&)src).beta;
	front = first = 0;
}

CObject3D* CPointSurCone3D::CopyObject()
{
	CObject3D *temp = new CPointSurCone3D((CObject3D&)*this);
	return temp;
}

CxObject3DSet* CPointSurCone3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(Cone);
	return list;
}

void CPointSurCone3D::Serialize( CArchive& ar )
{
	CPointSur3D::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << alpha;
		ar << beta;
		ar << gamma;
		ar << ((Cone) ? Cone->nObjectId : -1);
	}
	else
	{
		ar >> alpha;
		ar >> beta;
		ar >> gamma;
		Cone = (CCone3D*)SerializeObj(ar);
		pt1 = CVector4(0,0,0,0);
		pt2 = CVector4(0,0,0,0);
	}
}

BOOL CPointSurCone3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet || Cone) return FALSE;
	int nb = pSet->GetSize();

	if (nb != 1 ) return FALSE;

	Cone = (CCone3D*)pSet->GetAt(0);
	
	return TRUE;
}

int CPointSurCone3D::SetDepth()
{
	if (Cone)
		nDepth = Cone->nDepth+1;
	return nDepth;
}

CString CPointSurCone3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???"));
	if (Cone) sn1 = Cone->GetObjectHelp();
	mstr.Format(sFormat,sName,sn1);
	return mstr;
}

void CPointSurCone3D::CopyPointPosition(CVector4 pt)
{
}


void CPointSurCone3D::CopyPointPosition(CObject3D* src)
{
	CPointSur3D::CopyPointPosition(src);
	if (!src) return;
	if (src->MaskObject(isA()))
	{
		//Concept_pt =  ((CPoint3D*)src)->Concept_pt;
		alpha =  ((CPointSurCone3D*)src)->alpha;
		beta =  ((CPointSurCone3D*)src)->beta;
		gamma =  ((CPointSurCone3D*)src)->gamma;
	}
}


UINT  CPointSurCone3D::CalculConceptuel()
{
	bValidate = (Cone->bValidate);
	if (!bValidate)
		return ERR_SUPPORTOBJECT;

	return 0;
}

void CPointSurCone3D::DrawRetro(CDC* pDC,CVisualParam *vp)
{
	CPointSur3D::DrawRetro(pDC,vp);
}

CString CPointSurCone3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (/*bValidate && */Cone)
	{
        CString strName,strObj1;
		strName = GetObjectNameRedux();
		strObj1 = Cone->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
			mstr.Format(_T("PointOnCone[%s,%s];"),strName,strObj1);
		else if (nFormat==EXPORT_MAPLE)
			mstr.Format(_T("PointOnCone(%s,%s);"),strName,strObj1);
	}
	return mstr;
}

BOOL CPointSurCone3D::MoveObject(CVisualParam *myVisuParam,UINT,CPoint MouseClic,CVector4& TempCpt)
{
	return TRUE;
}