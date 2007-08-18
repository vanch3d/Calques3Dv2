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
/// @file Plan3D.h
/// @brief interface of the CPlan3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PLAN3D_H__F7E630B5_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_PLAN3D_H__F7E630B5_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CPoint3D;
class CDroite3D;
class CSegment3D;

//////////////////////////////////////////////////////////////////////
/// Specialization of CVector4 for sorting according to planar distance.
/// Note that both the distance, the visibility and side flags are computed externally.
//////////////////////////////////////////////////////////////////////
class CVector4SSide : public CVector4
{
public:
    int vis,side;
    FCoord dis;
public:
    CVector4SSide() : CVector4() {};
    CVector4SSide(CVector4 v,FCoord n,int s=0,int vi=1) : CVector4(v)
        {dis=n;vis=vi;side=s;};

	inline bool operator <(const CVector4SSide& other) const
	{
		return (dis < other.dis);
	}

};

/////////////////////////////////////////////////////////////////////////////
/// Redefinition of CArray<CVector4SDist,CVector4SDist> as CxVectorSSSet
/////////////////////////////////////////////////////////////////////////////
class CxVectorSSSet : public CArray<CVector4SSide,CVector4SSide>
{
public:
	int AddSorted(CVector4SSide theo);
	void AddtoList(CDroite3D *d, CSegment3D *p,CPoint3D  *myA[4],int *nbpt,
				   FCoord dp1,  CVector4 base, int n,bool b);
};
//typedef CArray<CVector4SSide,CVector4SSide> CSVectorSet;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPlan3D : public CObject3D  
{
	BOOL		bUpdateMe;
public:
	DECLARE_SERIAL(CPlan3D);

	CPoint3D 	*P1,*P2,*P3;
	CVector4	VecNorm;
	CVector4	p1,p2,p3,p4;
	CVector4	planeBorder;
	CPoint		tp1,tp2,tp3,tp4;

	void		*VecList;			// Liste des successeurs
	CLocalRep	plRep;
	CLocalRep	ptonRep;

	CPlan3D() : CObject3D(), bUpdateMe(1) {};
	CPlan3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *p3);
	CPlan3D(CPoint3D *p1,CVector4 plNom);
	CPlan3D(const CObject3D & );
	virtual ~CPlan3D();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual void Serialize( CArchive& ar );
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
	virtual int SetDepth();
	virtual void	GetRange(CVector4 &min,CVector4 &max);

	virtual UINT GetNameID() const { return IDS_NAME_PLANE;};
	virtual UINT GetHelpID() const { return IDS_HELP_PLANE;};
	virtual UINT GetDefID() const { return IDS_DEF_PLAN;};
	virtual CString GetObjectDef();

	virtual CObjectId isA() const { return TPlan3DClass; }
	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint);
	virtual BOOL IsPointInside(CVector4& pt,BOOL bLim=TRUE) { return TRUE;};
	virtual CRgn* InvalideRect();

	void UpdateContour(bool upd=1) { bUpdateMe = upd;};
	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
	virtual void DrawRetro(CDC*,CVisualParam *vp);
	virtual void	Draw3DRendering(int nVolMode);

	virtual CString ExportSymbolic(int nFormat); 

	virtual UINT IsParallelTo(CObject3D *pObj);
	virtual UINT IsPerpendicularTo(CObject3D *pObj);


	CVector4 GetNormalForm();
	virtual void HandleObjectError(int,BOOL);


	static FCoord GetDistancePt(CVector4 Nf, CVector4 pt);
	static void GetProjectedMinMax(CObject3D* pObj,CVector4 P1,CVector4 I,CVector4 J,CVector4 K, CVector4& minmax);

};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPlanPerp3D : public CPlan3D
{
public:
	DECLARE_SERIAL(CPlanPerp3D);
	CDroite3D	*D;
	CVector4	IntPt;

	CPlanPerp3D() : CPlan3D() {};
	CPlanPerp3D(CPoint3D *p1,CDroite3D *dr);
	CPlanPerp3D(const CObject3D & );
	virtual ~CPlanPerp3D();
	virtual int SetDepth();

	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual UINT  CalculConceptuel();
	virtual CString GetObjectDef();

	virtual UINT GetDefID() const { return IDS_DEF_PLANPERP;};

	virtual CObjectId isA() const { return TPlanPerp3DClass; }

	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
	virtual CString ExportSymbolic(int nFormat); 

};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPolygon3D : public CPlan3D
{
public:
	enum { gNotConvex, gNotConvexDegenerate,
	       gConvexDegenerate, gConvexCCW, gConvexCW };

	DECLARE_SERIAL(CPolygon3D);
	CxObject3DSet	m_pPointSet;
	CVector4		m_vCentroid;
	BOOL			m_bConvex;
	FCoord			m_nArea;
	FCoord			m_nZFact;

	CPolygon3D();
	CPolygon3D(CxObject3DSet *pSet);
	CPolygon3D(const CObject3D & );
	virtual ~CPolygon3D();
	virtual int SetDepth();
	virtual void	GetRange(CVector4 &min,CVector4 &max);

	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual UINT  CalculConceptuel();
	virtual CString GetObjectDef();
	virtual void	GetPolygons(CxObject3DSet*);
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual UINT GetHelpID() const { return IDS_HELP_POLYGON;};
	virtual UINT GetDefID() const { return IDS_DEF_POLYGON;};

	int		CalculCentroid();
	int		CheckConvex();

	int WhichSide(CVector4 p,CVector4 q,CVector4 r);
	int Compare(CVector4 p,CVector4 q);

	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);
	virtual void DrawRetro(CDC*,CVisualParam *vp);
	virtual void	Draw3DRendering(int nVolMode);
	virtual CString ExportSymbolic(int nFormat); 

	virtual BOOL IsInActiveArea(CPoint);

	virtual BOOL IsPointInside(CVector4& pt,BOOL bLim=TRUE);

	virtual CObjectId isA() const { return TPolygon3DClass; }
};



#endif // !defined(AFX_PLAN3D_H__F7E630B5_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
