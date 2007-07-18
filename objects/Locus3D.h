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
/// @file Locus3D.h
/// @brief interface of the CLocus3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOCUS3D_H__71BE2320_F769_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_LOCUS3D_H__71BE2320_F769_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

#ifndef __ARRAY_EX_H__
	#include "..\Collection\ArrayEx.h"
#endif

class CPoint3D;
class CLocus3D;

/////////////////////////////////////////////////////////////////////////////
/// CLocusMesh
///
/////////////////////////////////////////////////////////////////////////////
class CLocusMesh
{
public:
	bool		bIsLinear;
	CVector4	vertex3D[3];
	CPoint		vertex2D[3];

public: 
	CLocusMesh();
	CLocusMesh(CVector4 pt1,CVector4 pt2, CVector4 pt3);
	CLocusMesh(CVector4 pt1,CVector4 pt2);

	void operator =(const CLocusMesh& other);				// Assignment

public:
	FCoord	GetLength();
	void	CalculVisuel(CVisualParam *);
	void	Draw(CDC*,CVisualParam *,CLocus3D *);
	BOOL	IsInActiveArea(CPoint);
};

/////////////////////////////////////////////////////////////////////////////
/// CLocus3D
///
/////////////////////////////////////////////////////////////////////////////
class CLocus3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CLocus3D);

	CPoint3D	*m_pSource,		///< Base-point of the locus
				*m_pLocus;		///< Reference point of the locus

	int			nDeltaT;		///< Size of the locus (ie nb of points used in calculation)
	int			m_nMaxLenght;	///< Maximum length authorized for the mesh elements

private:
	BOOL		m_bFirstLoaded;
	
	typedef CArrayEx<CVector4,CVector4>		CVArray;
	CArrayEx<CVArray, CVArray &>			mesh3D;		///< Two-dimensional list of Vector4

	CArray<CLocusMesh,CLocusMesh>		m_cTriangles;	///< List of mesh elements

private:
	void GenerateLinear(CxObject3DSet* pDirectList);
	void GenerateSurface(CxObject3DSet* pDirectList);
	void GenerateMesh();
public:
	CLocus3D();
	CLocus3D(CPoint3D *pSource,CPoint3D *pLocus);
	CLocus3D(const CObject3D & );
	virtual ~CLocus3D();

	virtual UINT GetNameID() const { return IDS_NAME_LOCUS;};
	virtual UINT GetHelpID() const { return IDS_HELP_LOCUS;};
	virtual UINT GetDefID() const { return IDS_DEF_LOCUS;};

	virtual void Serialize( CArchive& ar );
	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();
	virtual int SetDepth();

	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual DWORD isA() const { return TLocus3DClass; }
	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint);
	virtual CRgn* InvalideRect();

	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *,BOOL bSM=FALSE);
	virtual void		Draw3DRendering(int nVolMode);

	virtual CString ExportSymbolic(int nFormat); 
};

#endif // !defined(AFX_LOCUS3D_H__71BE2320_F769_11D4_A2FA_00D0B71C8709__INCLUDED_)
