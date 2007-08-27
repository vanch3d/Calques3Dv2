// POVTool.cpp: implementation of the CPOVTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "POVTool.h"
#include "Prefs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPOVUserTool, CObject, VERSIONABLE_SCHEMA | 1)

CPOVUserTool::CPOVUserTool()
{
	m_strLabel.Empty();
	m_bCanRemove = TRUE;
	m_projParam = TPref::TUniv.sDefParam;
}

// CPOVUserTool::CPOVUserTool(CPOVUserTool& src)
// {
// 	m_strLabel = src.m_strLabel;
// 	m_bCanRemove = src.m_bCanRemove;
// 	m_projParam = src.m_projParam;
// }

CPOVUserTool::~CPOVUserTool()
{
	//DeleteIcon ();
}

// void CPOVUserTool::operator =(const CPOVUserTool& src)
// {
// 	m_bCanRemove = src.m_bCanRemove;
// 	m_bCanRemove = src.m_bCanRemove;
// 	m_projParam = src.m_projParam;
// }
// 

void CPOVUserTool::Serialize (CArchive& ar)
{
	CObject::Serialize (ar);

	if (ar.IsLoading ())
	{
		ar >> m_strLabel;
		ar >> m_bCanRemove;
		ar >> m_projParam.rho;
		ar >> m_projParam.theta;
		ar >> m_projParam.phi;
		ar >> m_projParam.dis;
	}
	else
	{
		ar << m_strLabel;
		ar << m_bCanRemove;
		ar << m_projParam.rho;
		ar << m_projParam.theta;
		ar << m_projParam.phi;
		ar << m_projParam.dis;
	}
}
