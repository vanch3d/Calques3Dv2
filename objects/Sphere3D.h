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
/// @file Sphere3D.h
/// @brief interface of the CSphere3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SPHERE3D_H__16C5C800_D1EA_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_SPHERE3D_H__16C5C800_D1EA_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CDroite3D;
class CSegment3D;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CSphere3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CSphere3D);
	CPoint3D	*P1,		///< Center of the sphere
				*P2;		///< Point on the envelope of the sphere
	CSegment3D	*Seg;		///< Segment defining the radius of a sphere centered on P1
	CVector4	Rayon;		///< Radius of the sphere
private:
	CVisualParam*	locVisParam;

public:
	CSphere3D();
	CSphere3D(CPoint3D *p1,CPoint3D *p2);
	CSphere3D(CPoint3D *p1,CSegment3D *seg);
	CSphere3D(const CObject3D & );
	~CSphere3D();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual UINT GetNameID() const { return IDS_NAME_SPHERE;};
	virtual UINT GetHelpID() const { return IDS_HELP_SPHERE;};
	virtual UINT GetDefID() const { return IDS_DEF_SPHERE;};
	virtual CString GetObjectDef();
	virtual int SetDepth();
	virtual void	GetRange(CVector4 &min,CVector4 &max);

	virtual DWORD isA() const { return TSphere3DClass; }
	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint pt);
	virtual CRgn* InvalideRect();

	virtual void Serialize( CArchive& ar );

	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void DrawRetro(CDC*,CVisualParam *vp);
	virtual void		Draw3DRendering(int nVolMode);

	virtual CString ExportSymbolic(int nFormat); 

	BOOL		IntersectLine(CDroite3D *dr,CVector4 *in,CVector4 *out);


	static BOOL IsPointVisible(CVisualParam* myVisuParam,CVector4& P1,CVector4& P);


	static void DrawUnit(CVector4& deb,CVector4& fin,CVector4& P1,
					CDC *pDC,CVisualParam *mV,
					CPen& curPen,CPen& curPenH,CPen& disPen,CPen& disPenH);

	static void CalcUnit(CVector4 I,CVector4 J,CVector4 K,
					CVector4 P1,int t,FCoord r,CVector4 & V);
};

#endif // !defined(AFX_SPHERE3D_H__16C5C800_D1EA_11D4_A2FA_00D0B71C8709__INCLUDED_)
