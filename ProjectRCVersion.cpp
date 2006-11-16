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