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

