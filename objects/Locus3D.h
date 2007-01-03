// Locus3D.h: interface for the CLocus3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCUS3D_H__71BE2320_F769_11D4_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_LOCUS3D_H__71BE2320_F769_11D4_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

#ifndef __ARRAY_EX_H__
	#include "..\Collection\ArrayEx.h"
#endif

class CPoint3D;

/////////////////////////////////////////////////////////////////////////////
/// CLocus3D
///
/////////////////////////////////////////////////////////////////////////////
class CLocus3D : public CObject3D  
{
public:
	DECLARE_SERIAL(CLocus3D);

	CPoint3D	*m_pSource,		///< Base-point of the locus
				*m_pLocus;		///< Reference point of the locus

	int			nDeltaT;		///< Size of the locus (ie nb of points used in calculation)

private:
	BOOL		m_bFirstLoaded;
	
	typedef CArrayEx<CVector4,CVector4>		CVArray;
	CArrayEx<CVArray, CVArray &>			a3D;		///< Two-dimensional list of Vector4

	typedef CArrayEx<CPoint,CPoint>			CPArray;
	CArrayEx<CPArray, CPArray &>			a2D;		///< Two-dimensional list of Point

	//CArray<CVector4,CVector4>	m_cCpts;
	CxVectorSet	m_cCpts;
	CArray<CPoint,CPoint>		m_cVpts;

public:
	CLocus3D();
	CLocus3D(CPoint3D *pSource,CPoint3D *pLocus);
	CLocus3D(const CObject3D & );
	virtual ~CLocus3D();

	virtual UINT GetNameID() const { return IDS_NAME_LOCUS;};
	virtual UINT GetHelpID() const { return IDS_HELP_LOCUS;};
	virtual UINT GetDefID() const { return IDS_DEF_LOCUS;};

	virtual void Serialize( CArchive& ar );
	virtual CObject3D* CopyObject();
	virtual CxObject3DSet* GetParents();
	virtual CString GetObjectDef();
	virtual int SetDepth();

	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);

	virtual DWORD isA() const { return TLocus3DClass; }
	virtual BOOL IsEqual(CObject3D &other);

	virtual BOOL IsInActiveArea(CPoint);
	virtual CRgn* InvalideRect();

	virtual UINT  CalculConceptuel();
	virtual void CalculVisuel(CVisualParam *);
	virtual void Draw(CDC*,CVisualParam *,BOOL bSM=FALSE);
	virtual void		Draw3DRendering();

	virtual CString ExportSymbolic(int nFormat); 
};

#endif // !defined(AFX_LOCUS3D_H__71BE2320_F769_11D4_A2FA_00D0B71C8709__INCLUDED_)
