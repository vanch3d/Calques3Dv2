// ProjectRCVersion.h: interface for the CProjectRCVersion class.
//
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
