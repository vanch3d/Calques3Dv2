// Point3D.h: interface for the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3D_H__E53DB5CE_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_POINT3D_H__E53DB5CE_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CDroite3D;
class CSegment3D;
class CCercle3D;
class CPlan3D;
class CSphere3D;
class CCylinder3D;

/////////////////////////////////////////////////////////////////////////////
/// CPoint3D:
/// See Point3D.cpp for the implementation of this class
///
/////////////////////////////////////////////////////////////////////////////
class CPoint3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CPoint3D);
	CVector4	Concept_pt;		// Conceptual Coord
	CPoint		Visual_pt;		// Visual Coord
	BOOL		bUp;			// Above horiz. plane ?

	CPoint3D();
	CPoint3D(CPoint pt);
	CPoint3D(CVector4 pt);
	CPoint3D(const CObject3D & );
	virtual ~CPoint3D();

	virtual void Serialize( CArchive& ar );
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet);

	virtual CObject3D* CopyObject();
	virtual BOOL IsEqual(CObject3D &other);
	virtual BOOL GraftOn(CObject3D *pNew);
	virtual BOOL SetParents(CxObject3DSet* pSet) { return TRUE;};

	virtual UINT GetNameID() const { return IDS_NAME_POINT;};
	virtual UINT GetHelpID() const { return IDS_HELP_POINT;};
	virtual UINT GetDefID() const { return IDS_DEF_POINT;};

	virtual COLORREF	GetDefaultColor();

	virtual unsigned long isA() const { return TPoint3DClass; }

	virtual BOOL IsInActiveArea(CPoint pt);
	virtual CRgn* InvalideRect();

	virtual UINT	IsAlignedWith(CObject3D *pObj1,CObject3D *pObj2);

	virtual void	TranslateBy(CVector4 ptVec);

	virtual int SetDepth() { return nDepth=1;};

	virtual void CopyPointPosition(CObject3D* );
	virtual void CopyPointPosition(CVector4 );
	virtual int  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void DrawRetro(CDC *pDC,CVisualParam *vp);
	virtual void DrawSelected(CDC* pDC,CVisualParam *vp);
	virtual CString DrawSymbolic(); 
	virtual void	GetRange(CVector4 &min,CVector4 &max);
	virtual void	Draw3DRendering();

	virtual BOOL GetDistanceFrom(CObject3D *pObj,FCoord& dis);

	virtual BOOL MoveObject(CVisualParam *,UINT,CPoint ,CVector4 &);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointCalc3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointCalc3D);
	CObject3D	*pSource;
public:
	CPointCalc3D();
	CPointCalc3D(CObject3D *s);
	CPointCalc3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_POINTCALC;};
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet) { return pSet;};
	virtual BOOL SetParents(CxObject3DSet* pSet) { return FALSE;};

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual void Serialize( CArchive& ar );

	virtual int SetDepth();

	virtual unsigned long isA() const { return TPointCalc3DClass; };

	virtual void HandleObjectError(int,BOOL bShow=FALSE);
	virtual int  CalculConceptuel() {return 0;};
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointMilieu3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointMilieu3D);
	CPoint3D	*P1,*P2;		// milieu défini par 2 pts existants
	CSegment3D	*S;				// milieu d'ubn segment
public:
	CPointMilieu3D();
	CPointMilieu3D(CPoint3D *s1,CPoint3D *s2);
	CPointMilieu3D(CSegment3D *s1);
	CPointMilieu3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_POINTMID;};
	virtual void Serialize( CArchive& ar );
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual int SetDepth();

	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet) { return pSet;};

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual CString GetObjectDef();

	virtual unsigned long isA() const { return TPointMilieu3DClass; }

	virtual CString DrawSymbolic(); 

	virtual int  CalculConceptuel();
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSur3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointSur3D);
	CPointSur3D();
	CPointSur3D(const CObject3D & );
	//virtual void Serialize( CArchive& ar );
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet);


	static BOOL IntersectSphereRay(CVector4 raybase,CVector4 raycos,CVector4 center,
							FCoord radius,FCoord *rin,FCoord* rout);

	static BOOL IntersectCylindreRay(CVector4 raybase,CVector4 raycos,CVector4 base,CVector4 axis,
							FCoord radius,FCoord *rin,FCoord* rout);

	static BOOL	IntersectPlanDroite(CVector4& PP1,CVector4& U,
							CVector4& s1,CVector4& nom,CVector4* res);


};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSurD3D : public CPointSur3D
{
public:
	DECLARE_SERIAL(CPointSurD3D);
	CDroite3D	*S;
	FCoord		lambda;
public:
	CPointSurD3D();
	CPointSurD3D(CDroite3D *s1);
	CPointSurD3D(const CObject3D & );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual void Serialize( CArchive& ar );
	virtual BOOL SetParents(CxObject3DSet* pSet);

	virtual int SetDepth();

	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet);

	virtual UINT GetDefID() const { return IDS_DEF_POINTSUR;};
	virtual CString GetObjectDef();

	virtual unsigned long isA() const { return TPointSurD3DClass; }

	virtual void CopyPointPosition(CObject3D* );
	virtual void CopyPointPosition(CVector4 );
	virtual int  CalculConceptuel();

	virtual CString DrawSymbolic(); 

	virtual BOOL MoveObject(CVisualParam *,UINT,CPoint ,CVector4&);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSurC3D : public CPointSur3D
{
public:
	DECLARE_SERIAL(CPointSurC3D);
	CCercle3D	*S;
	FCoord		lambda;
public:
	CPointSurC3D();
	CPointSurC3D(CCercle3D *s1);
	CPointSurC3D(const CObject3D & );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual void Serialize( CArchive& ar );
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual int SetDepth();

	virtual UINT GetDefID() const { return IDS_DEF_POINTSUR;};
	virtual CString GetObjectDef();

	virtual unsigned long isA() const { return TPointSurC3DClass; }

	virtual void CopyPointPosition(CObject3D* );
	virtual void CopyPointPosition(CVector4 );
	virtual int  CalculConceptuel();
	virtual void DrawRetro(CDC*,CVisualParam *vp);

	virtual CString DrawSymbolic(); 

	virtual BOOL MoveObject(CVisualParam *,UINT,CPoint ,CVector4&);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSurP3D : public CPointSur3D
{
public:
	DECLARE_SERIAL(CPointSurP3D);
	CPlan3D		*P;
	FCoord		alpha,beta,gamma;
public:
	CPointSurP3D();
	CPointSurP3D(CPlan3D *s1);
	CPointSurP3D(const CObject3D & );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual void Serialize( CArchive& ar );
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual int SetDepth();

	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet);

	virtual UINT GetDefID() const { return IDS_DEF_POINTSUR;};
	virtual CString GetObjectDef();

	virtual unsigned long isA() const { return TPointSurP3DClass; }

	virtual void CopyPointPosition(CObject3D* );
	virtual void CopyPointPosition(CVector4 );
	virtual int  CalculConceptuel();
	virtual void DrawRetro(CDC*,CVisualParam *vp);
	virtual CString DrawSymbolic(); 

	virtual BOOL MoveObject(CVisualParam *,UINT,CPoint ,CVector4&);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSurS3D : public CPointSur3D
{
public:
	DECLARE_SERIAL(CPointSurS3D);
	CSphere3D	*S;
	FCoord		latit;
	FCoord		longi;
	BOOL			front,first;
public:
	CPointSurS3D();
	CPointSurS3D(CSphere3D *s1);
	CPointSurS3D(const CObject3D & );
	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual int SetDepth();


	virtual UINT GetDefID() const { return IDS_DEF_POINTSUR;};
	virtual CString GetObjectDef();

	virtual unsigned long isA() const { return TPointSurS3DClass; }

	virtual void CopyPointPosition(CObject3D* );
	virtual void CopyPointPosition(CVector4 );
	virtual int  CalculConceptuel();

	virtual void DrawRetro(CDC*,CVisualParam *vp);
	virtual CString DrawSymbolic(); 

	virtual BOOL MoveObject(CVisualParam *,UINT,CPoint ,CVector4&);
};


//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSurCyl3D : public CPointSur3D
{
public:
	DECLARE_SERIAL(CPointSurCyl3D);
	CCylinder3D	*Cyl;				// Cylinder 
	FCoord		alpha,				// position on circle
				beta,
				gamma;				// position on axis
	BOOL		front,first;
	CVector4	pt1,pt2;
public:
	CPointSurCyl3D();
	CPointSurCyl3D(CCylinder3D *s1);
	CPointSurCyl3D(const CObject3D & );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual void Serialize( CArchive& ar );
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual int SetDepth();

	virtual UINT GetDefID() const { return IDS_DEF_POINTSUR;};
	virtual CString GetObjectDef();

	virtual unsigned long isA() const { return TPointSurCyl3DClass; }

	virtual void CopyPointPosition(CObject3D* );
	virtual void CopyPointPosition(CVector4 );
	virtual int  CalculConceptuel();
	virtual void DrawRetro(CDC*,CVisualParam *vp);

	virtual CString DrawSymbolic(); 

	virtual BOOL MoveObject(CVisualParam *,UINT,CPoint ,CVector4&);
};


//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointInter3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointInter3D);
	CPointInter3D();
	CPointInter3D(const CObject3D & );
	//virtual void Serialize( CArchive& ar );
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointInterDD3D : public CPointInter3D
{
public:
	DECLARE_SERIAL(CPointInterDD3D);
	CDroite3D	*D1,*D2;		//milieu défini par 2 pts existants
	//TSegment3D	*S;
	FCoord		lambda,
				mu;

public:
	CPointInterDD3D();
	CPointInterDD3D(CDroite3D *s1,CDroite3D *s2);
	//TPointMilieu3D(TWindow *AParent,TSegment3D *s1);
	CPointInterDD3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_POINTIDD;};
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet);
	virtual int SetDepth();


	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual CString GetObjectDef();
	virtual void Serialize( CArchive& ar );

	virtual unsigned long isA() const { return TPointInterDD3DClass; }

	virtual CString DrawSymbolic(); 

	virtual int  CalculConceptuel();
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointInterDP3D : public CPointInter3D
{
public:
	DECLARE_SERIAL(CPointInterDP3D);
	CDroite3D	*D1;		//milieu défini par 2 pts existants
	CPlan3D		*P;
	CPoint3D	*P1,*P2,*P3;
public:
	CPointInterDP3D();
	CPointInterDP3D(CDroite3D *s1,CPlan3D *s2);
	CPointInterDP3D(CDroite3D *s1,CPoint3D *p1,CPoint3D *p2,CPoint3D *p3);
	CPointInterDP3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_POINTIDP;};
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet);
	virtual int SetDepth();


	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual CObject3D* CopyObject();
	virtual CString GetObjectDef();
	virtual void Serialize( CArchive& ar );
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual unsigned long isA() const { return TPointInterDP3DClass; }
	virtual int  CalculConceptuel();

	virtual CString DrawSymbolic(); 

};


//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointCenter3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointCenter3D);
	CCercle3D	*C;
public:
	CPointCenter3D();
	CPointCenter3D(CCercle3D *s1);
	CPointCenter3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_POINTCENTER;};

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual CString GetObjectDef();
	virtual void Serialize( CArchive& ar );
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet) { return pSet;};
	virtual int SetDepth();


	virtual unsigned long isA() const { return TPointCenter3DClass; }

	virtual CString DrawSymbolic(); 

	virtual int  CalculConceptuel();
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointSymetric3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointSymetric3D);
	CPoint3D		*Src;
	CPoint3D		*Pt;
	CDroite3D		*Dr;
	CPlan3D			*Pl;
public:
	CPointSymetric3D();
	CPointSymetric3D(CPoint3D *pt,CPoint3D *s1);
	CPointSymetric3D(CPoint3D *pt,CDroite3D *s1);
	CPointSymetric3D(CPoint3D *pt,CPlan3D *s1);
	CPointSymetric3D(const CObject3D & );
	virtual void Serialize( CArchive& ar );
	virtual int SetDepth();


	virtual UINT GetDefID() const { return IDS_DEF_POINTSYM;};
	virtual CString GetObjectDef();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet) { return pSet;};

	virtual unsigned long isA() const { return TPointSymetric3DClass; }

	virtual CString DrawSymbolic(); 

	virtual int  CalculConceptuel();
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CPointTranslat3D : public CPoint3D
{
public:
	DECLARE_SERIAL(CPointTranslat3D);
	CPoint3D		*Src;
	CPoint3D		*Org;
	CPoint3D		*Dir;
public:
	CPointTranslat3D();
	CPointTranslat3D(CPoint3D *pt,CPoint3D *s1,CPoint3D *s2);
	CPointTranslat3D(const CObject3D & );
	virtual void Serialize( CArchive& ar );

	virtual UINT GetDefID() const { return IDS_DEF_POINTTRANS;};
	virtual CString GetObjectDef();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
	virtual int SetDepth();


	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL SetParents(CxObject3DSet* pSet);
	virtual CxSchemeSet* GetRedefineSchemes(CxSchemeSet* pSet) { return pSet;};

	virtual unsigned long isA() const { return TPointTranslat3DClass; }

	virtual CString DrawSymbolic(); 

	virtual int  CalculConceptuel();
};

#endif // !defined(AFX_POINT3D_H__E53DB5CE_D0E2_11D4_A2FA_00D0B71C8709__INCLUDED_)
