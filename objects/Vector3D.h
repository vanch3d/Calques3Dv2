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
/// @file Vector3D.h
/// @brief interface of the CVector3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VECTOR3D_H__C03A9B21_9492_11D5_A2F4_B80C2A89502F__INCLUDED_)
#define AFX_VECTOR3D_H__C03A9B21_9492_11D5_A2F4_B80C2A89502F__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

#include "Droite3D.h"

//////////////////////////////////////////////////////////////////////
/// CVector3D
//////////////////////////////////////////////////////////////////////
class CVector3D : public CSegment3D  
{
	DECLARE_SERIAL(CVector3D);
public:
	CVector3D();
	CVector3D(const CObject3D & );
	virtual ~CVector3D();

public:
	virtual UINT GetNameID() const { return IDS_NAME_VECTOR;};
	virtual UINT GetHelpID() const { return IDS_HELP_VECTOR;};
	virtual UINT GetDefID() const { return IDS_DEF_VECTOR;};
	virtual DWORD isA() const { return TVector3DClass; }

public:
	virtual CObject3D* CopyObject();

	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint pt);
	virtual CRgn* InvalideRect();

	virtual void Serialize( CArchive& ar );
	virtual int SetDepth();

	virtual CString GetObjectDef();

	virtual void CalculVisuel(CVisualParam *);
	virtual UINT  CalculConceptuel();
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=0);

};

#endif // !defined(AFX_VECTOR3D_H__C03A9B21_9492_11D5_A2F4_B80C2A89502F__INCLUDED_)
