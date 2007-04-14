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
/// @file ProjectRCVersion.h
/// @brief interface of the CProjectRCVersion class.
///
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_PROJECTRCVERSION_H__E079ABD9_D702_4500_997C_D45A16A581E3__INCLUDED_)
#define AFX_PROJECTRCVERSION_H__E079ABD9_D702_4500_997C_D45A16A581E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
/// CProjectRCVersion: Reading the VERSIONINFO resource.
///
/////////////////////////////////////////////////////////////////////////////
class CProjectRCVersion  
{
public:
	CProjectRCVersion();
	virtual ~CProjectRCVersion();

	BOOL GetProductName (LPSTR &prodName);
	BOOL GetProductVersion (LPSTR &prodVersion);
	BOOL GetProductVersion (int &major,int& minor,int& revision);
	BOOL GetCopyright (LPSTR &copyright);
	BOOL GetDebug (LPSTR &debug);

protected:
    LPBYTE      TransBlock;
    void FAR    *FVData;

private:
    // Don't allow this object to be copied.
    CProjectRCVersion (const CProjectRCVersion &);
    CProjectRCVersion & operator =(const CProjectRCVersion &);

};

#endif // !defined(AFX_PROJECTRCVERSION_H__E079ABD9_D702_4500_997C_D45A16A581E3__INCLUDED_)
