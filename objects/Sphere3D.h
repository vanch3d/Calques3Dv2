// Sphere3D.h: interface for the CSphere3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHERE3D_H__16C5C800_D1EA_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_SPHERE3D_H__16C5C800_D1EA_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CDroite3D;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CSphere3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CSphere3D);
	CPoint3D	*P1,			// centre de la sphere
				*P2;			// passant par ce point
	CVector4	Rayon;			// rayon de la sphère
private:
	CVisualParam*	locVisParam;

public:
	CSphere3D();
	CSphere3D(CPoint3D *p1,CPoint3D *p2);
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

	virtual CString DrawSymbolic(); 

	BOOL		IntersectLine(CDroite3D *dr,CVector4 *in,CVector4 *out);


	static BOOL IsPointVisible(CVisualParam* myVisuParam,CVector4& P1,CVector4& P);


	static void DrawUnit(CVector4& deb,CVector4& fin,CVector4& P1,
					CDC *pDC,CVisualParam *mV,
					CPen& curPen,CPen& curPenH,CPen& disPen,CPen& disPenH);

	static void CalcUnit(CVector4 I,CVector4 J,CVector4 K,
					CVector4 P1,int t,FCoord r,CVector4 & V);
};

#endif // !defined(AFX_SPHERE3D_H__16C5C800_D1EA_11D4_A2FA_00D0B71C8709__INCLUDED_)
