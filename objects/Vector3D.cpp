// Vector3D.cpp: implementation of the CVector3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"

#include "Vector3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CVector3D, CSegment3D, VERSIONABLE_SCHEMA | 1)

CVector3D::CVector3D() : CSegment3D()
{
}

CVector3D::CVector3D(const CObject3D & src): CSegment3D(src)
{
}

CVector3D::~CVector3D()
{
}


CObject3D* CVector3D::CopyObject()
{
	CObject3D *temp = new CVector3D((CObject3D&)*this);
	return temp;
}

BOOL CVector3D::IsEqual(CObject3D &other)
{
	return CSegment3D::IsEqual(other);
}

BOOL CVector3D::IsInActiveArea(CPoint pt)
{
	return CSegment3D::IsInActiveArea(pt);
}

CRgn* CVector3D::InvalideRect()
{
	return NULL;
}

void CVector3D::Serialize( CArchive& ar )
{
	CSegment3D::Serialize(ar);
}

int CVector3D::SetDepth()
{
	nDepth = 1;
	return nDepth;
}

CString CVector3D::GetObjectDef()
{
	CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
	sName = GetObjectName();
	sFormat.LoadString(GetDefID());

	mstr.Format(sFormat,sName);
	return mstr;
}

void CVector3D::CalculVisuel(CVisualParam *pVisParam)
{
	CSegment3D::CalculVisuel(pVisParam);
}

UINT  CVector3D::CalculConceptuel()
{
	UINT res = CSegment3D::CalculConceptuel();
	return 0;
}

void CVector3D::Draw(CDC*pDC,CVisualParam *vp,BOOL bSm)
{
	CSegment3D::Draw(pDC,vp,bSm);
}

