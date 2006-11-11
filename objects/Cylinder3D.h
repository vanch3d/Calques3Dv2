// Cylinder3D.h: interface for the CCylinder3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYLINDER3D_H__CB060980_E54D_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_CYLINDER3D_H__CB060980_E54D_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CDroite3D;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CCylinder3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CCylinder3D);
	int				nDeltaT;			// nb de generatrice (def 2)
	CxObject3DSet	cGenerList;
	CxObject3DSet	cPointsList;

	CPoint3D 		*P1,			// centre de la premiere base
					*P2;			// centre de la deuxieme base
	CPoint3D		*Rad;			// point envellope

	CCercle3D		*C1,*C2,*C3;
	CPoint3D		*Base1,*Base2,*Base3;

	FCoord			nRayon;			// rayon du cylindre
	CLocalRep 		IntH;			// repere local pour l'intersection plan H.
	CLocalRep 		LocRep;			// repere local pour l'intersection plan H.

	CCylinder3D();
	CCylinder3D(const CObject3D & );
	CCylinder3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *rad);
	virtual ~CCylinder3D();
	virtual int SetDepth();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual void Serialize( CArchive& ar );

	virtual BOOL IsEqual(CObject3D &other);
	virtual BOOL IsInActiveArea(CPoint pt);

	virtual UINT GetNameID() const { return IDS_NAME_CYLIN;};
	virtual UINT GetHelpID() const { return IDS_HELP_CYLIN;};
	virtual UINT GetDefID() const { return IDS_DEF_CYLIN;};

	virtual unsigned long isA() const { return TCylindre3DClass; }

	virtual CString GetObjectDef();
	virtual int SetProperties(CxObject3DSet *pSet=NULL);

	CVector4	 calculGeneratrice(FCoord th,CVector4 C,FCoord R,CVector4 I,CVector4 J,CVector4 K);
	FCoord		 DistPtToLine(CVector4 pt,CVector4 src,CVector4 dir);

	virtual int  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void DrawRetro(CDC*,CVisualParam *vp);

	BOOL	IntersectLine(CDroite3D *dr,CVector4 *in,CVector4 *out);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CCone3D : public CCylinder3D
{
public:
	DECLARE_SERIAL(CCone3D);
	CCone3D();
	CCone3D(CPoint3D *p1,CPoint3D *p2,CPoint3D *rad);
	CCone3D(const CObject3D & src);
	virtual ~CCone3D();

	virtual CObject3D* CopyObject();
	virtual unsigned long isA() const { return TCone3DClass; }

	virtual int  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);

	virtual void Draw(CDC* pDC,CVisualParam *vp,BOOL bSm=0);
	virtual void DrawRetro(CDC*,CVisualParam *vp);
};

#endif // !defined(AFX_CYLINDER3D_H__CB060980_E54D_11D4_A2FA_00D0B71C8709__INCLUDED_)
