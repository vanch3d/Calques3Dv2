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
// ProjectRCVersion.cpp: implementation of the CProjectRCVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "calques3d.h"
#include "ProjectRCVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectRCVersion::CProjectRCVersion()
{
	 char    appFName[255];
	 char    subBlockName[255];
	 DWORD   fvHandle;
	 UINT    vSize;

	 FVData = 0;

	 ::GetModuleFileName(AfxGetInstanceHandle(), appFName,sizeof(appFName));
	 OemToChar(appFName, appFName);
	 DWORD dwSize = ::GetFileVersionInfoSize(appFName, &fvHandle);
	 if (dwSize) {
		  FVData  = (void FAR *)new char[(UINT)dwSize];
		  if (::GetFileVersionInfo(appFName, fvHandle, dwSize, FVData)) {
				// Copy string to buffer so if the -dc compiler switch (Put constant strings in code segments)
				// is on VerQueryValue will work under Win16.  This works around a problem in Microsoft's ver.dll
				// which writes to the string pointed to by subBlockName.
				strcpy(subBlockName, "\\VarFileInfo\\Translation");
				if (!::VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&TransBlock, &vSize)) {
					 delete FVData;
                FVData = 0;
				} else
					 // Swap the words so wsprintf will print the lang-charset in the correct format.
					 *(DWORD *)TransBlock = MAKELONG(HIWORD(*(DWORD *)TransBlock), LOWORD(*(DWORD *)TransBlock));
		  }
	 }
}

CProjectRCVersion::~CProjectRCVersion()
{
	if (FVData) delete FVData;
}

BOOL CProjectRCVersion::GetProductName (LPSTR &prodName)
{
    UINT    vSize;
    char    subBlockName[255];

    wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"ProductName");
	 return FVData ? ::VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&prodName, &vSize) : false;
}


BOOL CProjectRCVersion::GetProductVersion (LPSTR &prodVersion)
{
	UINT    vSize;
	char    subBlockName[255];

	wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"ProductVersion");
    return FVData ? ::VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&prodVersion, &vSize) : false;
}

BOOL CProjectRCVersion::GetProductVersion (int &major,int& minor,int& revision)
{
	LPSTR prodVersion = 0;
	BOOL ret = GetProductVersion (prodVersion);
	if (ret)
	{
		CString str(prodVersion);

		int rr = sscanf(str,"%d.%d.%d",&major,&minor,&revision);
		if (rr==0||rr==EOF)
			rr = sscanf(str,"%d,%d,%d",&major,&minor,&revision);
		return (rr!=0&&rr!=EOF);
	}
	return ret;
}



BOOL CProjectRCVersion::GetCopyright (LPSTR &copyright)
{
	 UINT    vSize;
	 char    subBlockName[255];

	 wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"LegalCopyright");
	 return FVData ? ::VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&copyright, &vSize) : false;
}


BOOL CProjectRCVersion::GetDebug (LPSTR &debug)
{
	 UINT    vSize;
	 char    subBlockName[255];

	 wsprintf(subBlockName, "\\StringFileInfo\\%08lx\\%s", *(DWORD *)TransBlock, (LPSTR)"SpecialBuild");
	 return FVData ? ::VerQueryValue(FVData, subBlockName, (void FAR* FAR*)&debug, &vSize) : false;
}