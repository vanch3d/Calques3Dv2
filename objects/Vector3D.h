//////////////////////////////////////////////////////////////////////
/// @file Vector3D.h
/// @brief interface of the CVector3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VECTOR3D_H__C03A9B21_9492_11D5_A2F4_B80C2A89502F__INCLUDED_)
#define AFX_VECTOR3D_H__C03A9B21_9492_11D5_A2F4_B80C2A89502F__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif

#include "Droite3D.h"

//////////////////////////////////////////////////////////////////////
/// CVector3D
//////////////////////////////////////////////////////////////////////
class CVector3D : public CSegment3D  
{
	DECLARE_SERIAL(CVector3D);
public:
	CVector3D();
	CVector3D(const CObject3D & );
	virtual ~CVector3D();

public:
	virtual UINT GetNameID() const { return IDS_NAME_VECTOR;};
	virtual UINT GetHelpID() const { return IDS_HELP_VECTOR;};
	virtual UINT GetDefID() const { return IDS_DEF_VECTOR;};
	virtual DWORD isA() const { return TVector3DClass; }

public:
	virtual CObject3D* CopyObject();

	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint pt);
	virtual CRgn* InvalideRect();

	virtual void Serialize( CArchive& ar );
	virtual int SetDepth();

	virtual CString GetObjectDef();

	virtual void CalculVisuel(CVisualParam *);
	virtual UINT  CalculConceptuel();
	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSm=0);

};

#endif // !defined(AFX_VECTOR3D_H__C03A9B21_9492_11D5_A2F4_B80C2A89502F__INCLUDED_)
