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
/// @file VolumeObject3D.h
/// @brief interface of the CVolumeObject3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_VOLUMEOBJECT3D_H__8E2F48C7_ECE8_4B81_BD3A_64E06A25ACDE__INCLUDED_)
#define AFX_VOLUMEOBJECT3D_H__8E2F48C7_ECE8_4B81_BD3A_64E06A25ACDE__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

//////////////////////////////////////////////////////////////////////
/// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class CVolumeObject3D : public CObject3D 
{
public:
	DECLARE_SERIAL(CVolumeObject3D);
	
	CVolumeObject3D();
	CVolumeObject3D(const CObject3D & );
	virtual ~CVolumeObject3D();

public:
	virtual FCoord GetVolume();
	virtual FCoord GetSurface();
};

#endif // !defined(AFX_VOLUMEOBJECT3D_H__8E2F48C7_ECE8_4B81_BD3A_64E06A25ACDE__INCLUDED_)
