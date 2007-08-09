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
/// @file Geom.h
/// @brief interface of the CGeom class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_GEOM_H__5AB61FC8_358D_4CA2_AD60_7FC34CA97CAD__INCLUDED_)
#define AFX_GEOM_H__5AB61FC8_358D_4CA2_AD60_7FC34CA97CAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vector4.h"

//////////////////////////////////////////////////////////////////////
/// A wrapper for common Computational Geometry routines
//////////////////////////////////////////////////////////////////////
class CGeom  
{
public:
	static FCoord CGeom::GetDistanceToLine(CVector4 pt,CVector4 src,CVector4 dir);
	static int GetTrianglesIntersection(CVector4 V[3],CVector4 U[3],int& coplanar,
				     CVector4& isectpt1,CVector4& isectpt2);
private:
	static int tri_tri_intersect_with_isectline(float V0[3],float V1[3],float V2[3],
				     float U0[3],float U1[3],float U2[3],int *coplanar,
				     float isectpt1[3],float isectpt2[3]);
};

#endif // !defined(AFX_GEOM_H__5AB61FC8_358D_4CA2_AD60_7FC34CA97CAD__INCLUDED_)
