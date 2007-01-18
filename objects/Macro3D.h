// Macro3D.h: interface for the CMacro3D class.
//
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
