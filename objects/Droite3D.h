//////////////////////////////////////////////////////////////////////
/// @file Droite3D.h
/// @brief interface of the CDroite3D and derived classes.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DROITE3D_H__F7E630B3_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_DROITE3D_H__F7E630B3_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif 

#include "Object3D.h"

class CPoint3D;
class CPlan3D;

//////////////////////////////////////////////////////////////////////
/// Specialization of CVector4 for sorting according to linear distance.
/// Note that both the distance and the visibility flag are computed externally.
//////////////////////////////////////////////////////////////////////
class CVector4SDist : public CVector4
{
public:
	FCoord	dis;	///< The distance between this point and the source
	BOOL	in;		///< TRUE if the point is visible, FALSE otherwise

public:
	CVector4SDist() : CVector4() 
		{dis = 0.0; in = FALSE;};

	CVector4SDist(CVector4 v,FCoord n) : CVector4(v) 
		{dis=n;};
	
	inline bool operator <(const CVector4SDist& other) const 
	{
		return (dis < other.dis);
	}
};

/////////////////////////////////////////////////////////////////////////////
/// Redefinition of CArray<CVector4SDist,CVector4SDist> as CxVector4SDSet
/////////////////////////////////////////////////////////////////////////////
class CxVectorSDSet : public CArray<CVector4SDist,CVector4SDist>
{};
//typedef CArray<CVector4SDist,CVector4SDist>	CxVector4SDSet;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroite3D : public CObject3D  
{
	DECLARE_SERIAL(CDroite3D);
public:
	CPoint3D*	P1;			///< Base-point of this line
	CPoint3D*	P2;			///< This line is defined by a second point
	CDroite3D*	D2;			///< This line is defined by another line

	CVector4 	CDirVector;	///< Direction vector of the line
	BOOL		bIsSegment;	///< TRUE if this object is a segment line

protected:
	CVector4*	LP1;		///< Intersection points with the wall (F).
	CVector4*	LP2;		///< Intersection points with the wall (G).
	CVector4*	LP3;		///< Intersection points with the wall (H).
	CVector4*	CLP1;		///< Intersection point with the vision frustum.
	CVector4*	CLP2;		///< Intersection point with the vision frustum.

	CPoint		VP1,VP2;				//points visuels limites de la droite
	CPoint		*LPt1,*LPt2;

	CxVectorSDSet		VisualPts;	///< List of all the visual points used to draw the line.

public:
	CDroite3D();
	CDroite3D(CPoint3D *s1,CPoint3D *s2);
	CDroite3D(CPoint3D *s1,CDroite3D *s2);
	CDroite3D(const CObject3D & );
	virtual ~CDroite3D();

public:
	virtual UINT GetNameID() const	{ return IDS_NAME_LINE;};
	virtual UINT GetHelpID() const	{ return IDS_HELP_LINE;};
	virtual UINT GetDefID() const	{ return IDS_DEF_LINE;};
	virtual DWORD isA() const		{ return TDroite3DClass; };

	virtual CObject3D* CopyObject();

	virtual int SetDepth();
	virtual CxObject3DSet* GetParents();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual COLORREF	GetDefaultColor();


	virtual CRgn* InvalideRect();
	virtual BOOL IsInActiveArea(CPoint pt);
	virtual BOOL IsEqual(CObject3D &other);
	virtual BOOL IsInLimit(FCoord x);
	virtual UINT IsParallelTo(CObject3D *pObj);
	virtual UINT IsPerpendicularTo(CObject3D *pObj);

	virtual void Serialize( CArchive& ar );

	virtual CString GetObjectDef();
	virtual void CalculVisuel(CVisualParam *);
	virtual UINT  CalculConceptuel();
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=0);
	virtual CString ExportSymbolic(int nFormat); 

public:
	virtual CVector4	GetDirVector();
	virtual CVector4	GetBasePoint();

	CVector4* 	IntersectPlan(CVector4 s1,CVector4 s2,CVector4 s3,BOOL bLim=true);
	CVector4* 	IntersectDroite(CVector4 s1,CVector4 s2);

protected:
	int			ClippingDroite();
	void		AddVisualPt(CVector4SDist);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CSegment3D : public CDroite3D
{
	DECLARE_SERIAL(CSegment3D);
public:
	CSegment3D() : CDroite3D() {};
	CSegment3D(CPoint3D *s1,CPoint3D *s2);
	CSegment3D(const CObject3D & );

public:
	virtual UINT GetNameID() const { return IDS_NAME_SEG;};
	virtual UINT GetHelpID() const { return IDS_HELP_SEG;};
	virtual UINT GetDefID() const { return IDS_DEF_SEG;};
	virtual DWORD isA() const { return TSegment3DClass; }

	virtual CObject3D* CopyObject();
	virtual BOOL IsInLimit(FCoord x);
	virtual CString ExportSymbolic(int nFormat); 
	virtual CRgn* InvalideRect();
} ;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDemiDroite3D : public CDroite3D
{
	DECLARE_SERIAL(CDemiDroite3D);
public:
	CDemiDroite3D() : CDroite3D() {};
	CDemiDroite3D(CPoint3D *s1,CPoint3D *s2);
	CDemiDroite3D(const CObject3D & );

public:
	virtual UINT GetDefID() const { return IDS_DEF_DEMIDR;};
	virtual DWORD isA() const { return TDemiDroite3DClass; }

	virtual CObject3D* CopyObject();
	virtual BOOL IsInLimit(FCoord x);
	virtual BOOL IsInActiveArea(CPoint pt);
	virtual CString ExportSymbolic(int nFormat); 

};


//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroitePar3D : public CDroite3D
{
	DECLARE_SERIAL(CDroitePar3D);
public:
	CDroitePar3D() : CDroite3D() {};
	CDroitePar3D(CPoint3D *s1,CDroite3D *s2);
	CDroitePar3D(const CObject3D & );

public:
	virtual UINT GetDefID() const { return IDS_DEF_DROITEPAR;};
	virtual DWORD isA() const { return TDroitePar3DClass; }

	virtual CObject3D* CopyObject();
	virtual CString ExportSymbolic(int nFormat); 
} ;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroiteInterPP3D : public CDroite3D
{
	DECLARE_SERIAL(CDroiteInterPP3D);
public:
	CPlan3D 	*Pl1;	///< First plane for the intersection
	CPlan3D 	*Pl2;	///< Second plane for the intersection
private:
	CVector4	DrDir;	///< Direction vector for the resulting line.
	CVector4	DrPt;	///< Base point for the resulting line.
	CVector4	PtonDr;	///< Reference point for all points on this line.

public:
	CDroiteInterPP3D() : CDroite3D() {};
	CDroiteInterPP3D(CPlan3D *s1,CPlan3D *s2);
	CDroiteInterPP3D(const CObject3D & );

public:
	virtual UINT GetDefID() const { return IDS_DEF_DROITEIPP;};
	virtual DWORD isA() const { return TDroiteInterPP3DClass; }

	virtual int SetDepth();
	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();


	virtual CVector4  GetDirVector();
	virtual CVector4  GetBasePoint();
	virtual UINT  CalculConceptuel();

	virtual CString ExportSymbolic(int nFormat); 

} ;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroitePerpDD3D : public CDroite3D
{
	DECLARE_SERIAL(CDroitePerpDD3D);
public:
	CDroite3D*	D3;		///< Second line for the cross-product (the other one is CDroite3D::D2).
private:
	CVector4	DrDir;	///< Direction vector for the resulting line.
	CVector4	DrPt;	///< Base point for the resulting line.

public:
	CDroitePerpDD3D() : CDroite3D() {};
	CDroitePerpDD3D(CDroite3D *s1,CDroite3D *s2);
	CDroitePerpDD3D(const CObject3D & );

public:
	virtual UINT GetDefID() const { return IDS_DEF_CROSSPRODUCT;};
	virtual DWORD isA() const { return TDroitePerpDD3DClass; }

	virtual int SetDepth();

	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();


	virtual CVector4  GetDirVector();
	virtual CVector4  GetBasePoint();
	virtual UINT  CalculConceptuel();

	virtual CString ExportSymbolic(int nFormat); 

} ;


//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroitePerp3D : public CDroite3D
{
	DECLARE_SERIAL(CDroitePerp3D);
public:
	CPoint3D	*P3,*P4;
	CPlan3D		*Pl1;
private:
	CVector4	IntPt;		///< The intersection point between the plane and the line

public:
	CDroitePerp3D() : CDroite3D() {};
	CDroitePerp3D(CPoint3D *s1,CDroite3D *s2);
	CDroitePerp3D(CPoint3D *s1,CPlan3D *s2);
	CDroitePerp3D(CPoint3D *s1,CPoint3D *p1,CPoint3D *p2,CPoint3D *p3);
	CDroitePerp3D(const CObject3D & );

public:
	virtual UINT GetDefID() const { return IDS_DEF_DROITEPERP;};
	virtual DWORD isA() const { return TDroitePerp3DClass; }

	virtual int SetDepth();

	virtual CObject3D* CopyObject();
	virtual void Serialize( CArchive& ar );


	virtual CString GetObjectDef();

	virtual CxObject3DSet* GetParents();

	virtual CVector4  GetDirVector();
	virtual CVector4  GetBasePoint();
	virtual CVector4  GetIntersectionPoint();
	virtual UINT  CalculConceptuel();

	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);

	virtual CString ExportSymbolic(int nFormat); 
} ;

#endif // !defined(AFX_DROITE3D_H__F7E630B3_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
