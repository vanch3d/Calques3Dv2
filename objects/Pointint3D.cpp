// Point3D.cpp: implementation of the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Point3D.h"
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
IMPLEMENT_SERIAL(CPointInter3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointInter3D::CPointInter3D() : CPoint3D() 
{
}

CPointInter3D::CPointInter3D(const CObject3D & src): CPoint3D(src)
{
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointInterDD3D, CPointInter3D, VERSIONABLE_SCHEMA | 1)

CPointInterDD3D::CPointInterDD3D() : CPointInter3D() 
{
	D1 = NULL;
	D2 = NULL;
	lambda = mu = 0.;
	pObjectShape.nShapeId = TPref::TPoint.nPtDep;
};

CPointInterDD3D::CPointInterDD3D(CDroite3D *s1,CDroite3D *s2): CPointInter3D()
{
	D1 = s1;
	D2 = s2;
	nDepth = max(s1->nDepth,s2->nDepth)+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtDep;
	lambda = mu = 0.;

}

CPointInterDD3D::CPointInterDD3D(const CObject3D & src) : CPointInter3D(src)
{
	D1 =  ((CPointInterDD3D&)src).D1;
	D2 =  ((CPointInterDD3D&)src).D2;
	lambda = mu = 0.;
}

int CPointInterDD3D::SetDepth()
{
	if (D1 && D2)
		nDepth = max(D1->nDepth,D2->nDepth)+1;
	return nDepth;
}

CxSchemeSet* CPointInterDD3D::GetRedefineSchemes(CxSchemeSet* pSet) 
{ 
	if (!pSet) pSet = new CxSchemeSet();

	/// Redefine the free-point as a Point on line
	CRedefineScheme *pObj = new CRedefineScheme();
	pObj->bRelax = TRUE;
	pObj->nRedefStep = 1;
	pObj->nTaskResID[0] = CTX_SELECT_DROITE;
	pObj->nStepID[0] = TAllDroiteClass;
	pObj->nObjResID = CTX_POINTSUR_DROITE;
	pObj->nAvail = ID_CONSTRUCTION_POINTON_LINE;
	pObj->pNewObj = new CPointSurD3D();
	//((CPointSurD3D*)pObj->pNewObj)->S = D1;
	pSet->Add(pObj);

	return pSet;
}


void CPointInterDD3D::Serialize( CArchive& ar )
{
	CPointInter3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << ((D1) ? D1->nObjectId : -1);
		ar << ((D2) ? D2->nObjectId : -1);
	}
	else
	{
		D1 = (CDroite3D*)SerializeObj(ar);
		D2 = (CDroite3D*)SerializeObj(ar);
	}
}


CObject3D* CPointInterDD3D::CopyObject()
{
	CObject3D *temp = new CPointInterDD3D((CObject3D&)*this);
	return temp;
}

CxObject3DSet* CPointInterDD3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(D1);
	list->Add(D2);
	return list;
}

BOOL CPointInterDD3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet /*|| D1 || D2*/) return FALSE;
	int nb = pSet->GetSize();

	if (nb == 2 )
	{
		D1 = (CDroite3D*)pSet->GetAt(0);
		D2 = (CDroite3D*)pSet->GetAt(1);
	}
	else if (nb==1 && D1)
	{
		D2 = (CDroite3D*)pSet->GetAt(0);
	}
	else return FALSE;
	
	return TRUE;
}



CString CPointInterDD3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???")),sn2(_T("???"));
	if (D1) sn1 = D1->GetObjectHelp();
	if (D2)	sn2 = D2->GetObjectHelp();

	mstr.Format(sFormat,sName,sn1,sn2);
	return mstr;
}



UINT  CPointInterDD3D::CalculConceptuel()
{
	bValidate = ((D1->bValidate) && (D2->bValidate));
	if (!bValidate) return ERR_NOINTER;

	FCoord DotProd;
	CVector4 A = D1->GetBasePoint();
	CVector4 C = D2->GetBasePoint();
	CVector4 U(D1->GetDirVector()),
			 V(D2->GetDirVector()),
			 UvV = U % V,
			 AC = C - A;
		  /*		AC(D2->P1->Concept_pt.x - D1->P1->Concept_pt.x,
					D2->P1->Concept_pt.y - D1->P1->Concept_pt.y,
					D2->P1->Concept_pt.z - D1->P1->Concept_pt.z,
						  1);  */

	if (UvV.NullVector())
	 { 
		bValidate = FALSE;
		return ERR_DROITEPAR;
	 }
	DotProd = AC * UvV;
	if ( !FCZero(DotProd) )
	 { 
		bValidate = FALSE;
		//DotProd = 0.;
		return ERR_DROITNOCOPLAN;
	 }

	CVector4 	VT = UvV % V,
				UT = UvV % U;

	lambda	= (AC * VT) / (U * VT);
	mu		= - (AC * UT) / (V * UT);

	CVector4 M(	A.x + lambda*U.x,
					A.y + lambda*U.y,
					A.z + lambda*U.z,
					1);
	Concept_pt = M;

//	if ( ((D1->isA() == TSegment3DClass) && (!InSeg(lambda)) ) ||
//			 ((D2->isA() == TSegment3DClass) && (!InSeg(mu)) ))
	if (!(D1->IsInLimit(lambda)) || !(D2->IsInLimit(mu)))
	 { 
		bValidate = FALSE;
		return ERR_INTERINSEG;
	 }
	bValidate=TRUE;
	return 0;
}


CString CPointInterDD3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (bValidate && D1 && D2)
	{
		CString mstr2,strName,strObj1,strObj2;
		mstr2.LoadString(GetNameID());
		strName.Format("%s%d",mstr2,nObjectId);
		mstr2.LoadString(D1->GetNameID());
		strObj1.Format("%s%d",mstr2,D1->nObjectId);
		mstr2.LoadString(D2->GetNameID());
		strObj2.Format("%s%d",mstr2,D2->nObjectId);

		UINT nType1 = D1->isA();
		UINT nType2 = D2->isA();
		if (nType1==nType2)
		{
			//if (nType1==TDroite3DClass)
			if (nType1==TSegment3DClass)
				mstr.Format(_T("IntersectionSegments[%s,%s,%s];"),strName,strObj1,strObj2);
			else 
				mstr.Format(_T("IntersectionLines[%s,%s,%s];"),strName,strObj1,strObj2);
		}
		else
			mstr.Format(_T("IntersectionLines[%s,%s,%s];"),strName,strObj1,strObj2);
	}
	return mstr;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointInterDP3D, CPointInter3D, VERSIONABLE_SCHEMA | 1)

CPointInterDP3D::CPointInterDP3D() : CPointInter3D() 
{
	D1 = NULL;
	P  = NULL;
	P1 = P2 = P3 = NULL;
	pObjectShape.nShapeId = TPref::TPoint.nPtDep;
};

CPointInterDP3D::CPointInterDP3D(CDroite3D *s1,CPlan3D *s2): CPointInter3D()
{
	D1 = s1;
	P  = s2;
	P1 = P2 = P3 = NULL;
	nDepth = max(s1->nDepth,s2->nDepth)+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtDep;

}

CPointInterDP3D::CPointInterDP3D(CDroite3D *s1,CPoint3D *p1,CPoint3D *p2,CPoint3D *p3): CPointInter3D()
{
	D1 = s1;
	P  = NULL;
	P1 = p1;
	P2 = p2;
	P3 = p3;
	nDepth = max(s1->nDepth,max(p1->nDepth,max(p2->nDepth,p3->nDepth)))+1;
	pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

int CPointInterDP3D::SetDepth()
{
	if (D1 && P)
		nDepth = max(P->nDepth,D1->nDepth)+1;
	else if (D1 && P1 && P2 && P3)
		nDepth = max(D1->nDepth,max(P1->nDepth,max(P2->nDepth,P3->nDepth)))+1;
	return nDepth;
}

CxSchemeSet* CPointInterDP3D::GetRedefineSchemes(CxSchemeSet* pSet) 
{ 
	if (!pSet) pSet = new CxSchemeSet();

	/// Redefine the intersection as a Point on line
	CRedefineScheme *pObj = new CRedefineScheme();
	pObj->nRedefStep = 0;
	pObj->nObjResID = CTX_POINTSUR_DROITE;
	pObj->nAvail = ID_CONSTRUCTION_POINTON_LINE;
	pObj->pNewObj = new CPointSurD3D();
	((CPointSurD3D*)pObj->pNewObj)->S = D1;
	pSet->Add(pObj);

	/// Redefine the intersection as a Point on plane
	pObj = new CRedefineScheme();
	pObj->nRedefStep = 0;
	pObj->nObjResID = CTX_POINTSUR_PLAN;
	pObj->nAvail = ID_CONSTRUCTION_POINTON_PLANE;
	pObj->pNewObj = new CPointSurP3D();
	((CPointSurP3D*)pObj->pNewObj)->P = P;
	pSet->Add(pObj);

	return pSet;
}


CPointInterDP3D::CPointInterDP3D(const CObject3D &src ): CPointInter3D(src)
{
	D1 =  ((CPointInterDP3D&)src).D1;
	P =   ((CPointInterDP3D&)src).P;
	P1 =   ((CPointInterDP3D&)src).P1;
	P2 =   ((CPointInterDP3D&)src).P2;
	P3 =   ((CPointInterDP3D&)src).P3;
}

BOOL CPointInterDP3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
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

void CPointInterDP3D::Serialize( CArchive& ar )
{
	CPointInter3D::Serialize(ar);

	//Concept_pt.Serialize(ar);
	if (ar.IsStoring())
	{
		ar << ((D1) ? D1->nObjectId : -1);
		ar << ((P) ? P->nObjectId : -1);
		ar << ((P1) ? P1->nObjectId : -1);
		ar << ((P2) ? P2->nObjectId : -1);
		ar << ((P3) ? P3->nObjectId : -1);
	}
	else
	{
		D1 = (CDroite3D*)SerializeObj(ar);
		P = (CPlan3D*)SerializeObj(ar);
		P1 = (CPoint3D*)SerializeObj(ar);
		P2 = (CPoint3D*)SerializeObj(ar);
		P3 = (CPoint3D*)SerializeObj(ar);
	}
}


CxObject3DSet* CPointInterDP3D::GetParents()
{
	CxObject3DSet* list = new CxObject3DSet();
	list->Add(D1);
	if (!P)
	 {
		list->Add(P1);
		list->Add(P2);
		list->Add(P3);
	 }
	else
		list->Add(P);
	return list;
}

BOOL CPointInterDP3D::SetParents(CxObject3DSet* pSet)
{
	if (!pSet) return FALSE;
	int nb = pSet->GetSize();

	if (nb == 2 && !D1 && !P ) 
	{
		D1 = (CDroite3D*)pSet->GetAt(0);
		P = (CPlan3D*)pSet->GetAt(1);
	}
	else if (nb==4 && !D1 && !P1 && !P2 && !P3)
	{
		D1 = (CDroite3D*)pSet->GetAt(0);
		P1 = (CPoint3D*)pSet->GetAt(1);
		P2 = (CPoint3D*)pSet->GetAt(2);
		P3 = (CPoint3D*)pSet->GetAt(3);
	}
	else if (nb=1 && D1)
	{
		P = (CPlan3D*)pSet->GetAt(0);
	}
	else if (nb=1 && P)
	{
		D1 = (CDroite3D*)pSet->GetAt(0);
	}
	else return FALSE;
	
	return TRUE;
}



CObject3D* CPointInterDP3D::CopyObject()
{
	CObject3D *temp = new CPointInterDP3D((CObject3D&)*this);
	return temp;
}

UINT  CPointInterDP3D::CalculConceptuel()
{
	if (P)
		bValidate = ((D1->bValidate) && (P->bValidate));
	else
		bValidate = ((D1->bValidate) && (P1->bValidate) && (P2->bValidate)
						&& (P3->bValidate));
	if (!bValidate)
		return ERR_NOINTER;


	FCoord DotProd;
	CVector4 U 	= D1->GetDirVector(),
			VvW	= (P) ?
				P->VecNorm :
				((P2->Concept_pt - P1->Concept_pt) % (P3->Concept_pt,P1->Concept_pt)),
				//VectProduct(MakeVector(P1->Concept_pt,P2->Concept_pt),
				//			MakeVector(P1->Concept_pt,P3->Concept_pt)),
			AB = (((P) ? P->P1->Concept_pt : P1->Concept_pt) - D1->GetBasePoint());
				//MakeVector(D1->GetBasePoint(), (P) ?
				//			P->P1->Concept_pt : P1->Concept_pt);

	if ( FCZero(DotProd = U * VvW) )
	 { 
		bValidate = FALSE;
		return ERR_DRPLANPAR;
	 }

	FCoord 	lambda	= (AB * VvW) / DotProd;
	CVector4 	M(	D1->GetBasePoint().x + lambda*U.x,
					D1->GetBasePoint().y + lambda*U.y,
					D1->GetBasePoint().z + lambda*U.z,
					1);

	Concept_pt = M;

	if (!D1->IsInLimit(lambda))
//	if ((D1->isA() == TSegment3DClass) && (!InSeg(lambda)))
	 { 
		bValidate = FALSE;
		return ERR_INTERINSEG;
	 }

	if (!P->IsPointInside(Concept_pt,TRUE))
	{
		bValidate = FALSE;
		return ERR_INTERINSEG;
	}
	////*****30/07 P->CalculConceptuel();
	if (P)	P->UpdateContour();
	bValidate=TRUE;
	return 0;
}

CString CPointInterDP3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	CString sn1(_T("???")),sn2(_T("???"));
	if (D1) sn1 = D1->GetObjectHelp();
	if (P)	sn2 = P->GetObjectHelp();

	mstr.Format(sFormat,sName,sn1,sn2);
	return mstr;
}

CString CPointInterDP3D::ExportSymbolic(int nFormat)
{
	CString mstr;
	mstr.Empty();

	if (bValidate && D1 && P)
	{
		CString mstr2,strName,strObj1,strObj2;
		mstr2.LoadString(GetNameID());
		strName.Format("%s%d",mstr2,nObjectId);
		mstr2.LoadString(D1->GetNameID());
		strObj1.Format("%s%d",mstr2,D1->nObjectId);
		mstr2.LoadString(P->GetNameID());
		strObj2.Format("%s%d",mstr2,P->nObjectId);

		UINT nType1 = D1->isA();
		UINT nType2 = P->isA();

		mstr.Format(_T("IntersectionLinePlane[%s,%s,%s];"),strName,strObj1,strObj2);
	}
	return mstr;
}


