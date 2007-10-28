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
/// @file ConicSection3D.h
/// Interface of the CConicSection3D class.
///
/// $Date: 2007-10-28 12:07:55+00 $
/// $Revision: 1.1 $
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CONICSECTION3D_H__3BBB8B3E_CC71_4107_9553_FB9C1B829F8B__INCLUDED_)
#define AFX_CONICSECTION3D_H__3BBB8B3E_CC71_4107_9553_FB9C1B829F8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CCone3D;
class CPlan3D;

/////////////////////////////////////////////////////////////////////////////
/// CConicSection3D - The conic at the intersection of a cone and a plane.
/////////////////////////////////////////////////////////////////////////////
class CConicSection3D : public CObject3D  
{
	DECLARE_SERIAL(CConicSection3D);
public:
	CCone3D		*m_pCone;		///< A pointer to the cone.
	CPlan3D 	*m_pPlan;		///< A pointer to the plane.

	/////////////////////////////////////////////////////////////////////////////
	/// The type of the conic section
	/////////////////////////////////////////////////////////////////////////////
	enum TConicType 
	{ 
		CONIC_NONE = 0,		///< The conic is not defined.
		CONIC_POINT,		///< The conic is a point (degenerate case of ellipse).
		CONIC_CIRCLE,		///< The conic is a circle (particular case of ellipse).
		CONIC_ELLIPSE,		///< The conic is an ellipse.
		CONIC_2LINES,		///< The conic is two intersecting lines (degenerate case of hyperbola).
		CONIC_HYPERBOLA,	///< The conic is an hyperbola.
		CONIC_LINE,			///< The conic is a single line (degenerate case of a parabola).
		CONIC_PARABOLA		///< The conic is a parabola.
	};

	/////////////////////////////////////////////////////////////////////////////
	/// The general equation of the conic section.
	/// The general form of the equation of a conic section is
	/// @code
	///		A·x^2 + B·x·y + C·y^2 + D·x + E·y + F = 0
	/// @endcode
	/////////////////////////////////////////////////////////////////////////////
	class TConicEq 
	{
	public:
		FCoord A;
		FCoord B;
		FCoord C;
		FCoord D;
		FCoord E;
		FCoord F;
	};

private:
	CVector4	m_IntPt;		///< Intersection with cone's axis.
	FCoord		m_fCutAngle;	///< Angle between cutting plane and cone.
	FCoord		m_fDistVertex;	///< Distance to the vertex of the cone.
	TConicType	m_nConicType;	///< The type of the conic.
	TConicEq	m_cGeneralEq;	///< The general equation of the conic

	FCoord		m_fEccent;		///< Eccentricity of the conic.
	FCoord		m_fRotation;	///< Rotation of the conic
	CVector4	m_ptTrans;
	CVector4	m_ptFocus;
	CVector4	m_ptCenter;
	FCoord		m_fEMajor;
	FCoord		m_fEMinor;
	CLocalRep	m_LocRep;
	int			m_nDelta;

	CxVectorSet				m_cConcept_pts;
	CArray<CPoint,CPoint>		m_cVisual_pts;

public:
	CConicSection3D();
	CConicSection3D(CCone3D *p1,CPlan3D *p2);
	CConicSection3D(const CObject3D & );
	virtual ~CConicSection3D();

	UINT ComputeGeneralEquation();
	UINT ComputeAttributes();

public:
	virtual CObjectId isA() const { return TConicSection3DClass; }
	virtual UINT	GetDefID() const; 
	virtual UINT	GetHelpID() const;
	virtual UINT	GetNameID() const;
	virtual int		SetDepth();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();

	virtual BOOL	IsEqual(CObject3D &other);
	virtual void	Serialize( CArchive& ar );
	virtual BOOL	IsInActiveArea(CPoint);

	virtual UINT	CalculConceptuel();
	virtual void	Draw(CDC*,CVisualParam *,BOOL bSM=FALSE);
	virtual void	Draw3DRendering(int nVolMode);
	virtual CString	ExportSymbolic(int nFormat); 

};

#endif // !defined(AFX_CONICSECTION3D_H__3BBB8B3E_CC71_4107_9553_FB9C1B829F8B__INCLUDED_)
