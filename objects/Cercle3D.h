// Cercle3D.h: interface for the CCercle3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CERCLE3D_H__F7E630B6_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_CERCLE3D_H__F7E630B6_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CPoint3D;
class CCylinder3D;
class CPlan3D;
class CSphere3D;

///
/// This is the main class
///
class CCercle3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CCercle3D);
	BOOL		bCircCen;
	CPoint3D 	*P1,			// centre du cercle
				*P2,			// passant par ce point
				*P3;			// definit le plan du cercle
	CVector4	VecNorm;		// Normale au cercle
	CVector4	Center;
	FCoord		Radius;
	FCoord		nArcAngle;	// pour les arcs de cercles
	int			nDeltaT;		// pas de resolution du cercle (def 40)
	
	CLocalRep	LocRep;
	//TConceptList* 	Concept_pts;	// listes des coord. conceptuelles
	//TVisualList* 	Visual_pts;		// listes des coord. visuelles

	//CArray<CVector4,CVector4>	m_cConcept_pts;
	CxVectorSet	m_cConcept_pts;
	CArray<CPoint,CPoint>		m_cVisual_pts;

	virtual UINT GetNameID() const { return IDS_NAME_CIRCLE;};
	virtual UINT GetHelpID() const { return IDS_HELP_CIRCLE;};
	virtual UINT GetDefID() const { return IDS_DEF_CIRCLE;};

	virtual COLORREF	GetDefaultColor();

	CCercle3D();
	CCercle3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *p3,bool bypt=FALSE);
	CCercle3D(CPoint3D *p1,CVector4 Norm,FCoord ray);
	CCercle3D(const CObject3D & );
	virtual ~CCercle3D();

	virtual void Serialize( CArchive& ar );
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();
	virtual int SetDepth();
	
	virtual DWORD isA() const { return TCercle3DClass; }
	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint);
	virtual CRgn* InvalideRect();

	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *,BOOL bSM=FALSE);
	virtual void		Draw3DRendering();

	virtual CString ExportSymbolic(int nFormat); 
};


///
/// This is the main class
///
class CArcCercle3D : public CCercle3D
{
public:
	DECLARE_SERIAL(CArcCercle3D);
	CArcCercle3D();
	CArcCercle3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *p3);
	CArcCercle3D(const CObject3D & );

	virtual CObject3D* CopyObject();

	virtual UINT GetDefID() const { return IDS_DEF_ARCCIRCLE;};
	virtual DWORD isA() const { return TArcCercle3DClass; }
	virtual CString GetObjectDef();

	virtual BOOL IsEqual(CObject3D &other);
	virtual UINT  CalculConceptuel();

	virtual CString ExportSymbolic(int nFormat); 

};

///
/// This is the main class
///
class CEllipse3D : public CCercle3D
{
public:
	DECLARE_SERIAL(CEllipse3D);

	CCylinder3D		*Cy;
	CPlan3D 		*Pl;

	CEllipse3D();
	CEllipse3D(CCylinder3D *p1,CPlan3D *p2);
	CEllipse3D(const CObject3D & );

	virtual int SetDepth();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();

	virtual DWORD isA() const { return TEllipse3DClass; }
	virtual UINT GetDefID() const { return IDS_DEF_ELLIPSE;};

	virtual BOOL IsEqual(CObject3D &other);
	virtual void Serialize( CArchive& ar );

	virtual UINT  CalculConceptuel();
	virtual void		Draw3DRendering();
	virtual CString ExportSymbolic(int nFormat); 
};


///
/// This is the main class
///
class CCercleInterSS3D : public CCercle3D
{
public:
	CSphere3D *SP1;
	CSphere3D *SP2;

	DECLARE_SERIAL(CCercleInterSS3D);
	CCercleInterSS3D();
	CCercleInterSS3D(CSphere3D *p1,CSphere3D *p2);
	CCercleInterSS3D(const CObject3D & );

	virtual int SetDepth();
	virtual CxObject3DSet* GetParents();

	virtual CObject3D* CopyObject();

	virtual UINT GetDefID() const { return IDS_DEF_CIRCLEISS;};
	virtual DWORD isA() const { return TCercleInterSS3D; }
	virtual CString GetObjectDef();

	virtual BOOL IsEqual(CObject3D &other);
	virtual UINT  CalculConceptuel();

	//virtual CString ExportSymbolic(int nFormat); 
	virtual void Serialize( CArchive& ar );
	virtual CString ExportSymbolic(int nFormat); 
};



#endif // !defined(AFX_CERCLE3D_H__F7E630B6_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
