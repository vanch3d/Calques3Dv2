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
/// @file Macro3D.h
/// @brief interface of the CMacro3D class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MACRO3D_H__57FE9900_04C1_11D5_A2FA_00D0B71C8709__INCLUDED_)
#define AFX_MACRO3D_H__57FE9900_04C1_11D5_A2FA_00D0B71C8709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CompositeObj3D.h"

/////////////////////////////////////////////////////////////////////////////
/// CMacro3D
///
/////////////////////////////////////////////////////////////////////////////
class CMacro3D : public CCompositeObj3D  
{
public:
	DECLARE_SERIAL(CMacro3D);

	CxObject3DSet cParentList;		///< List of the free-points of the macro (ie graft targets)

	CMacro3D();
	virtual ~CMacro3D();

	virtual DWORD isA() const { return TMacro3DClass; }

	virtual void Serialize( CArchive& ar );
	int SetProperties(CxObject3DSet *pSet=NULL);
	virtual int SetDepth();

	virtual CxObject3DSet* GetParents();
	virtual BOOL ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph=FALSE);
	virtual void AddParent(CObject3D* pObj);
	virtual void RemoveSubObject(int Idx,BOOL bDel=TRUE);

	virtual CString GetObjectDef();
	virtual CString GetObjectHelp();
	virtual CString GetObjectName();

	virtual void Draw(CDC*,CVisualParam *vp,BOOL bSM=FALSE);
	virtual CString		ExportSymbolic(int nFormat); 

};

#endif // !defined(AFX_MACRO3D_H__57FE9900_04C1_11D5_A2FA_00D0B71C8709__INCLUDED_)
