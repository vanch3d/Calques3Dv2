// POVTool.h: interface for the CPOVTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POVTOOL_H__C425CC6A_ED36_431B_AC67_3D666B272463__INCLUDED_)
#define AFX_POVTOOL_H__C425CC6A_ED36_431B_AC67_3D666B272463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\objects\Vector4.h"


/////////////////////////////////////////////////////////////////////////////
/// CPOVUserTool
///
/////////////////////////////////////////////////////////////////////////////
class CPOVUserTool : public CObject  
{
	DECLARE_SERIAL(CPOVUserTool)
public:
// Construction
	CPOVUserTool();
//	CPOVUserTool(CPOVUserTool& src);
	virtual ~CPOVUserTool();

//	virtual void operator =(const CPOVUserTool& src);

// Overrides
	virtual void Serialize (CArchive& ar);

// Attributes:
public:
	CString			m_strLabel;		///< The name of the current point of view
	BOOL			m_bCanRemove;	///< TRUE if it can be removed. FALSE otherwise
	SProjection 	m_projParam;	///< The projection parameters of the current point of view
};

/////////////////////////////////////////////////////////////////////////////
/// List of undoable/redoable objects
/////////////////////////////////////////////////////////////////////////////
class CxPOVObjectSet : public CTypedPtrList<CObList,CPOVUserTool*>
{};


#endif // !defined(AFX_POVTOOL_H__C425CC6A_ED36_431B_AC67_3D666B272463__INCLUDED_)
