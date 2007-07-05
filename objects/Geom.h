// Geom.h: interface for the CGeom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOM_H__5AB61FC8_358D_4CA2_AD60_7FC34CA97CAD__INCLUDED_)
#define AFX_GEOM_H__5AB61FC8_358D_4CA2_AD60_7FC34CA97CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector4.h"

class CGeom  
{
public:
	CGeom();
	virtual ~CGeom();

	static int TTintersect_with_isectline(CVector4 V0,CVector4 V1,CVector4 V2,
				     CVector4 U0,CVector4 U1,CVector4 U2,int& coplanar,
				     CVector4& isectpt1,CVector4& isectpt2);
private:
	static int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3],
				     float U0[3],float U1[3],float U2[3],int *coplanar,
				     float isectpt1[3],float isectpt2[3]);
};

#endif // !defined(AFX_GEOM_H__5AB61FC8_358D_4CA2_AD60_7FC34CA97CAD__INCLUDED_)
