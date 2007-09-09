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
/// @file Cone3D.h
/// @brief interface of the CCone3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CONE3D_H__A3934306_BBF1_4C76_B690_4826B9B8A7F0__INCLUDED_)
#define AFX_CONE3D_H__A3934306_BBF1_4C76_B690_4826B9B8A7F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VolumeObject3D.h"
#include "CompositeObj3D.h"

class CPoint3D;
class CCercle3D;
class CDroite3D;

//////////////////////////////////////////////////////////////////////
/// The cone
//////////////////////////////////////////////////////////////////////
class CCone3D : public CVolumeObject3D
{
public:
	CPoint3D 		*pApex;			///< The apex of the cone
	CPoint3D 		*pAxis;			///< The point defining the axis of the cone
	CPoint3D		*pRadius;		///< The point defining the envelope of the cone

	CCercle3D		*C1,*C2,*C3;
	CPoint3D		*Base1,*Base2,*Base3;

	CxObject3DSet	cGenerList;		///< List of the outer edge vectors
	CxObject3DSet	cPointsList;	///< List of the points on the base's circle

public:
	FCoord			nRadius;		///< Radius of the cone at its base
	FCoord			nHeight;		///< Height of the cone
	FCoord			nOpenAngle;		///< Opening angle of the cube (ie between axis and outer edge)
	CLocalRep 		LocRep;			///< Local referential
	int				nDelta;

public:
	DECLARE_SERIAL(CCone3D);
	CCone3D();
	CCone3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *rad);
	CCone3D(const CObject3D & src);
	virtual ~CCone3D();

	virtual CObject3D* CopyObject();
	virtual CObjectId isA() const { return TCone3DClass; }
	virtual int SetDepth();
	virtual void Serialize( CArchive& ar );

	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual BOOL IsInActiveArea(CPoint pt);

	virtual UINT GetNameID() const { return IDS_NAME_CONE;};
	virtual UINT GetHelpID() const { return IDS_HELP_CONE;};
	virtual UINT GetDefID() const { return IDS_DEF_CONE;};
	virtual CString GetObjectDef();
	virtual CxObject3DSet* GetParents();

	virtual int SetProperties(CxObject3DSet *pSet=NULL);
	virtual void Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void DrawRetro(CDC*,CVisualParam *vp);
	virtual void Draw3DRendering(int nVolMode);

public:
	UINT IntersectPlan();
	UINT IntersectLine(CDroite3D *dr,CVector4 &in,CVector4 &out);

	CVector4 GetConeApex();
	CVector4 GetConeAxis();
};

//////////////////////////////////////////////////////////////////////
/// The bi-point at the intersection of a cone and a line
//////////////////////////////////////////////////////////////////////
class CInterConeDr3D : public CCompositeObj3D
{
	DECLARE_SERIAL(CInterConeDr3D);
public:
	CCone3D		*Cone;		///< Pointer to the cone
	CDroite3D	*Dr;		///< Pointer to the line
	CPoint3D	*ptA;		///< The first point of the intersection
	CPoint3D	*ptB;		///< The second point of the intersection

	CInterConeDr3D();
	CInterConeDr3D(CCone3D* sp1,CDroite3D* dr2);
	CInterConeDr3D(const CObject3D & );

	virtual int SetDepth();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();

	virtual CObjectId isA() const { return TInterConeDr3DClass; }

	virtual UINT GetDefID() const { return IDS_DEF_INTCONEDR;};

	virtual void Serialize( CArchive& ar );

	virtual CString GetObjectDef();
	virtual UINT  CalculConceptuel();

	virtual CString ExportSymbolic(int nFormat); 
};

#endif // !defined(AFX_CONE3D_H__A3934306_BBF1_4C76_B690_4826B9B8A7F0__INCLUDED_)
