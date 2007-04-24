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
/// @file CompositeObj3D.h
/// @brief interface of the CCompositeObj3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_COMPOSITEOBJ3D_H__D97907A0_D720_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_COMPOSITEOBJ3D_H__D97907A0_D720_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CPoint3D;
class CSegment3D;
class CDroite3D;
class CCercle3D;
class CSphere3D;
class CPolygon3D;

///
/// This is the main class
///
class CCompositeObj3D : public CObject3D
{
public:
	DECLARE_SERIAL(CCompositeObj3D);
	CxObject3DSet m_cSubObjects;			// List of sub-objects
	int nStartShow;						// Index of available objects

	BOOL m_bExpandGraph;				// Expand the component in the DepGraph
public:
	CCompositeObj3D();
	CCompositeObj3D(const CObject3D & );
	~CCompositeObj3D();

	virtual CObject3D* CopyObject();

	virtual DWORD isA() const { return TCompositeObject3DClass; }
	virtual int UpdateID(int nIdx);

	virtual void Serialize( CArchive& ar );

	virtual UINT GetNameID() const { return IDS_NAME_CPTOBJECT;};
	virtual UINT GetHelpID() const { return IDS_HELP_CPTOBJECT;};
	virtual UINT GetDefID() const { return IDS_DEF_CPTOBJECT;};

	virtual int SetObjectID(int nID);
	virtual void SetHistoryVisibility();
	virtual void SetVisible(BOOL bVis);
	virtual void SetInGraph(BOOL bAdd=TRUE);

	virtual BOOL IsInActiveArea(CPoint pt);
	virtual BOOL IsInCalque(int CalcNum);
	virtual BOOL AddInCalque(int CalcNum,BOOL bAdd=TRUE);
	virtual BOOL IsVisible();
	virtual CRgn* InvalideRect();
	virtual int ValideCompositeObject(int nIdx);

	virtual void GetDependList(CxObject3DSet*,BOOL bAll=TRUE);
	virtual void	GetPolygons(CxObject3DSet*);
	virtual void	TranslateBy(CVector4 ptVec);

	virtual CObject3D* HitTest(CPoint pt,UINT mask=0,int nCalcNum = 0,BOOL bSub = TRUE,CxObject3DSet* pSet = NULL);

	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSM=FALSE);
	virtual void DrawSelected(CDC* pDC,CVisualParam *vp);
	virtual void HandleObjectError(int,BOOL bShow=FALSE);

	virtual void DrawDepGraph(CDC* pDC,CImageList *pImgList=NULL,int nTrace=GRAPH_NONE,BOOL bDrawNode=TRUE,BOOL bDrawLink=TRUE);
	virtual HTREEITEM DrawHistory(CTreeCtrl& mListCtrl,HTREEITEM pParent=TVI_ROOT);
	virtual void		Draw3DRendering(int nVolMode);
	virtual void ClearHistory();

	virtual int SetProperties(CxObject3DSet *pSet=NULL);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CCube3D : public CCompositeObj3D
{
protected:

	//////////////////////////////////////////////////////////////////////
	/// Internal structure for cube's faces.
	//////////////////////////////////////////////////////////////////////
	class CFacette
	{
	public:
		CFacette() 
			{	base = NULL;
				seg[0]=seg[1]=seg[2]=seg[3]=NULL;
			}
		
		CPoint3D *base;
		CSegment3D *seg[4];
		CPolygon3D* pFace;
	};
	
public:
	DECLARE_SERIAL(CCube3D);
	CPoint3D* 	ptA;  		// origine de l'arete intiale du cube
	CPoint3D* 	ptB;			// extremite de l'arete

	CCercle3D*	cr;			// cercle centre sur l'extremite de l'arete
//	CPoint3D* 	ptc1;			// extremite de l'arete
//	CPoint3D* 	ptc2;			// extremite de l'arete
	CPoint3D* 	sommet[8];
	CSegment3D* 	arete[12];
	CFacette	facette[6];
	FCoord		m_nAlpha;

	CCube3D();
	CCube3D(CPoint3D* p1,CPoint3D* p2,FCoord nAlpha);
	CCube3D(const CObject3D & );

	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL IsEqual(CObject3D &other);
	virtual int SetDepth();

	virtual void Serialize( CArchive& ar );
	virtual int SetProperties(CxObject3DSet *pSet=NULL);
	virtual void SetColor(COLORREF rColor);
	virtual void SetStyle(int nStyle);

	virtual UINT GetNameID() const { return IDS_NAME_CUBE;};
	virtual UINT GetHelpID() const { return IDS_HELP_CUBE;};
	virtual UINT GetDefID() const { return IDS_DEF_CUBE;};

	virtual DWORD isA() const { return TCube3DClass; }

	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSM=FALSE);
	virtual void PrepareMoveObject(BOOL bMove = TRUE);

	void InitCube(BOOL bLoad = FALSE);
	void InitFace(int facNum,int som,int s1,int s2,int s3,int s4);
	virtual CString GetObjectDef();
	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);

	virtual void	GetRange(CVector4 &min,CVector4 &max);

};

///
/// This is the main class
///
class CInterSphDr3D : public CCompositeObj3D
{
public:
	DECLARE_SERIAL(CInterSphDr3D);
	CSphere3D	*Sph;  		// sphere
	CDroite3D	*Dr;			// droite
	CPoint3D	*ptA,*ptB;	// les points intersection

	CInterSphDr3D();
	CInterSphDr3D(CSphere3D* sp1,CDroite3D* dr2);
	CInterSphDr3D(const CObject3D & );

	virtual int SetDepth();
	void InitInterSphDr();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL IsEqual(CObject3D &other);

	virtual DWORD isA() const { return TInterSphDr3DClass; }

	virtual UINT GetDefID() const { return IDS_DEF_INTSPDR;};

	virtual void Serialize( CArchive& ar );
	virtual void SetColor(COLORREF rColor);
	virtual void SetStyle(int nStyle);

	virtual CString GetObjectDef();
	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSM=FALSE);

	virtual CString ExportSymbolic(int nFormat); 
	virtual void	GetRange(CVector4 &min,CVector4 &max);
};

///
/// This is the main class
///
class CDivSegment3D : public CCompositeObj3D
{
public:
	DECLARE_SERIAL(CDivSegment3D);
	CSegment3D	*Seg;  		// sphere
	int			m_nDiv;

	CDivSegment3D();
	CDivSegment3D(CSegment3D* pSeg);
	CDivSegment3D(const CObject3D & );

	virtual int SetDepth();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL IsEqual(CObject3D &other);

	virtual DWORD isA() const { return TDivSegment3DClass; }

	virtual UINT GetDefID() const { return IDS_DEF_SECTIONSEG;};

	virtual void Serialize( CArchive& ar );
	virtual void SetColor(COLORREF rColor);
	virtual void SetStyle(int nStyle);

	virtual CString GetObjectDef();
	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSM=FALSE);

	virtual CString ExportSymbolic(int nFormat); 
	virtual void	GetRange(CVector4 &min,CVector4 &max);
};

///
/// This is the main class
///
class CInterCircDr3D : public CCompositeObj3D
{
public:
	DECLARE_SERIAL(CInterCircDr3D);
	CCercle3D	*Circ;  		// sphere
	CDroite3D	*Dr;			// droite
	CPoint3D	*ptA,*ptB;		// les points intersection

	CInterCircDr3D();
	CInterCircDr3D(CCercle3D* sp1,CDroite3D* dr2);
	CInterCircDr3D(const CObject3D & );

	virtual int SetDepth();
	void InitIntersection();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL IsEqual(CObject3D &other);

	virtual DWORD isA() const { return TInterCircDr3DClass; }

	virtual UINT GetDefID() const { return IDS_DEF_INTCIRCDR;};

	virtual void Serialize( CArchive& ar );
	virtual void SetColor(COLORREF rColor);
	virtual void SetStyle(int nStyle);

	virtual CString GetObjectDef();
	virtual UINT  CalculConceptuel();
	//virtual void CalculVisuel(CVisualParam *);
	//virtual void Draw(CDC*,CVisualParam *vp,BOOL bSM=FALSE);

	//virtual CString ExportSymbolic(int nFormat); 
	//virtual void	GetRange(CVector4 &min,CVector4 &max);
};

///
/// This is the main class
///
class CInterCircPlane3D : public CCompositeObj3D
{
public:
	DECLARE_SERIAL(CInterCircPlane3D);
	CCercle3D	*Circ;  		// sphere
	CPlan3D		*Pl;			// plane
	CPoint3D	*ptA,*ptB;		// les points intersection

	CInterCircPlane3D();
	CInterCircPlane3D(CCercle3D* sp1,CPlan3D* dr2);
	CInterCircPlane3D(const CObject3D & );

	virtual int SetDepth();
	void InitIntersection();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();

	virtual DWORD isA() const { return TInterCircPl3DClass; }

	virtual UINT GetDefID() const { return IDS_DEF_INTCIRCDR;};

	virtual void Serialize( CArchive& ar );

	virtual CString GetObjectDef();
	virtual UINT  CalculConceptuel();
};

#endif // !defined(AFX_COMPOSITEOBJ3D_H__D97907A0_D720_11D4_A2FA_00D0B71C8709__INCLUDED_)
