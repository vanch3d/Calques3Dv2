// Droite3D.h: interface for the CDroite3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROITE3D_H__F7E630B3_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_DROITE3D_H__F7E630B3_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

class CPoint3D;
class CPlan3D;


//void AddInList(CVector4*p,
class CDVector4 : public CVector4
{
public:
	FCoord	dis;
	BOOL	in;
	CDVector4() : CVector4() {dis = 0.0; in = FALSE;};
	CDVector4(CVector4 v,FCoord n) :
		CVector4(v) 
		{dis=n;};
	 bool operator <(const CDVector4& other) const;
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroite3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CDroite3D);
	CPoint3D		*P1,*P2;		//droite définie par 2 pts existants
	CDroite3D		*D2;			//ou par un point P1 et une droite D2

	CVector4 		CDirVector;		//vecteur directeur de la droite

	CVector4		*LP1,*LP2,*LP3;	// intersection avec les cloisons
	CVector4		*CLP1,*CLP2;	// clipping

	CPoint		VP1,VP2;				//points visuels limites de la droite
	CPoint		*LPt1,*LPt2;
	//TMyOrderL*	VisualPts;
	typedef CArray<CDVector4,CDVector4>	CDVectorSet;
	CDVectorSet		VisualPts;

	BOOL			bIsSegment;		//c'est un segment

	CDroite3D();
	CDroite3D(CPoint3D *s1,CPoint3D *s2);
	CDroite3D(CPoint3D *s1,CDroite3D *s2);
	CDroite3D(const CObject3D & );
	virtual ~CDroite3D();

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual COLORREF	GetDefaultColor();

	virtual UINT GetNameID() const { return IDS_NAME_LINE;};
	virtual UINT GetHelpID() const { return IDS_HELP_LINE;};
	virtual UINT GetDefID() const { return IDS_DEF_LINE;};

	virtual DWORD isA() const { return TDroite3DClass; }

	virtual BOOL IsEqual(CObject3D &other);
	virtual BOOL IsInLimit(FCoord x);

	virtual BOOL IsInActiveArea(CPoint pt);
	virtual CRgn* InvalideRect();

	virtual UINT IsParallelTo(CObject3D *pObj);
	virtual UINT IsPerpendicularTo(CObject3D *pObj);

	virtual void Serialize( CArchive& ar );
	virtual int SetDepth();

	virtual CString GetObjectDef();

	void AddVisualPt(CDVector4);

	virtual void CalculVisuel(CVisualParam *);
	virtual UINT  CalculConceptuel();
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=0);

	virtual CString ExportSymbolic(int nFormat); 

	virtual CVector4  GetDirVector();
	virtual CVector4  GetBasePoint();

	int ClippingDroite();
	CVector4* 	IntersectPlan(CVector4 s1,CVector4 s2,CVector4 s3,BOOL bLim=true);
	CVector4* 	IntersectDroite(CVector4 s1,CVector4 s2);
};

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CSegment3D : public CDroite3D
{
public:
	DECLARE_SERIAL(CSegment3D);
	CSegment3D() : CDroite3D() {};
	CSegment3D(CPoint3D *s1,CPoint3D *s2);
	CSegment3D(const CObject3D & );

	virtual CObject3D* CopyObject();
	virtual BOOL IsInLimit(FCoord x);

	virtual UINT GetNameID() const { return IDS_NAME_SEG;};
	virtual UINT GetHelpID() const { return IDS_HELP_SEG;};
	virtual UINT GetDefID() const { return IDS_DEF_SEG;};

	virtual DWORD isA() const { return TSegment3DClass; }

	virtual CString ExportSymbolic(int nFormat); 

	virtual CRgn* InvalideRect();
} ;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDemiDroite3D : public CDroite3D
{
public:
	DECLARE_SERIAL(CDemiDroite3D);
	CDemiDroite3D() : CDroite3D() {};
	CDemiDroite3D(CPoint3D *s1,CPoint3D *s2);
	CDemiDroite3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_DEMIDR;};

	virtual CObject3D* CopyObject();
	virtual BOOL IsInLimit(FCoord x);
	virtual BOOL IsInActiveArea(CPoint pt);

	virtual CString ExportSymbolic(int nFormat); 

	virtual DWORD isA() const { return TDemiDroite3DClass; }
};


//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroitePar3D : public CDroite3D
{
public:
	DECLARE_SERIAL(CDroitePar3D);
	CDroitePar3D() : CDroite3D() {};
	CDroitePar3D(CPoint3D *s1,CDroite3D *s2);
	CDroitePar3D(const CObject3D & );

	virtual UINT GetDefID() const { return IDS_DEF_DROITEPAR;};
	virtual CObject3D* CopyObject();

	virtual CString ExportSymbolic(int nFormat); 

	virtual DWORD isA() const { return TDroitePar3DClass; }
} ;

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CDroiteInterPP3D : public CDroite3D
{
public:
	DECLARE_SERIAL(CDroiteInterPP3D);
	CPlan3D 	*Pl1;
	CPlan3D 	*Pl2;
	CVector4	DrDir;
	CVector4	DrPt;
	CVector4	PtonDr;

	virtual UINT GetDefID() const { return IDS_DEF_DROITEIPP;};

	CDroiteInterPP3D() : CDroite3D() {};
	CDroiteInterPP3D(CPlan3D *s1,CPlan3D *s2);
	CDroiteInterPP3D(const CObject3D & );
	virtual int SetDepth();

	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();

	virtual DWORD isA() const { return TDroiteInterPP3DClass; }

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
public:
	CDroite3D		*D3;			//perp to D2 and D3
	CVector4	DrDir;
	CVector4	DrPt;

	DECLARE_SERIAL(CDroitePerpDD3D);

	virtual UINT GetDefID() const { return IDS_DEF_CROSSPRODUCT;};

	CDroitePerpDD3D() : CDroite3D() {};
	CDroitePerpDD3D(CDroite3D *s1,CDroite3D *s2);
	CDroitePerpDD3D(const CObject3D & );
	virtual int SetDepth();

	virtual void Serialize( CArchive& ar );

	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();

	virtual DWORD isA() const { return TDroitePerpDD3DClass; }

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
public:
	DECLARE_SERIAL(CDroitePerp3D);
	CPoint3D	*P3,*P4;
	CPlan3D		*Pl1;
	CVector4	IntPt;

	virtual UINT GetDefID() const { return IDS_DEF_DROITEPERP;};

	CDroitePerp3D() : CDroite3D() {};
	CDroitePerp3D(CPoint3D *s1,CDroite3D *s2);
	CDroitePerp3D(CPoint3D *s1,CPlan3D *s2);
	CDroitePerp3D(CPoint3D *s1,CPoint3D *p1,CPoint3D *p2,CPoint3D *p3);
	CDroitePerp3D(const CObject3D & );
	virtual int SetDepth();

	virtual CObject3D* CopyObject();
	virtual void Serialize( CArchive& ar );

	virtual DWORD isA() const { return TDroitePerp3DClass; }

	virtual CString GetObjectDef();

	virtual CxObject3DSet* GetParents();

	virtual CVector4  GetDirVector();
	virtual CVector4  GetBasePoint();
	virtual UINT  CalculConceptuel();

	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=FALSE);

	virtual CString ExportSymbolic(int nFormat); 
} ;

#endif // !defined(AFX_DROITE3D_H__F7E630B3_D1B9_11D4_A2FA_00D0B71C8709__INCLUDED_)
