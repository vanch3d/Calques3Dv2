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
// Macro3D.cpp: implementation of the CMacro3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "..\Prefs\Prefs.h"

#include "Macro3D.h"

#include "Point3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMacro3D, CCompositeObj3D, VERSIONABLE_SCHEMA | 1)

CMacro3D::CMacro3D()
{
    nDepth = 1;
}

CMacro3D::~CMacro3D()
{
    int nb = cParentList.GetSize();
    cParentList.RemoveAll();
}

CxObject3DSet* CMacro3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    int nb = cParentList.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = cParentList.GetAt(i);
        if (!pObj) continue;
        list->Add(pObj);

    }
    return list;
}

BOOL CMacro3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    int nb = cParentList.GetSize();

    if (!nb) return FALSE;
    BOOL bFound = FALSE;

    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = cParentList.GetAt(i);
        if (!pObj) continue;

        if (pOld != pObj) continue;
        SetInGraph(FALSE);
        cParentList.RemoveAt(i);
        cParentList.InsertAt(i,pNew);

        if (bUpGraph) SetInGraph(TRUE);
        bFound = TRUE;
        break;
    }
    return bFound;
}


int CMacro3D::SetDepth()
{
    int oldDepth = nDepth;
    nDepth = 0;
    int nb = cParentList.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = cParentList.GetAt(i);
        if (!pObj) continue;
        nDepth = max(pObj->nDepth,nDepth);
    }
    nDepth++;
    nb = m_cSubObjects.GetSize();
    for (i=0;i<nb;i++)
    {
        CObject3D * pObj = m_cSubObjects.GetAt(i);
        if (!pObj) continue;
        pObj->SetDepth();
        if (pObj->isA()==TPoint3DClass)
            pObj->nDepth+=nDepth-1;
    }
    return nDepth;
}


void CMacro3D::AddParent(CObject3D* pObj)
{
    if (!pObj) return;

    cParentList.Add(pObj);
    nDepth = 1;
    int nb = cParentList.GetSize();
    for (int i=0;i<nb;i++)
    {
        CObject3D *pObj = cParentList.GetAt(i);
        if (!pObj) continue;
        nDepth = max(pObj->nDepth,nDepth)+1;
    }
}

void CMacro3D::RemoveSubObject(int Idx,BOOL bDel)
{
    int nb = m_cSubObjects.GetSize();
    //if (Idx >= nb) return;
    CObject3D* pObj = m_cSubObjects.GetAt(Idx);
    m_cSubObjects.RemoveAt(Idx);
    if (bDel) delete pObj;
}


void CMacro3D::Serialize( CArchive& ar )
{
    CCompositeObj3D::Serialize(ar);
    int nb = cParentList.GetSize();

    if (ar.IsStoring())
    {
        ar << strObjectHelp;
        ar << strObjectDef;
        ar << nb;
        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = cParentList.GetAt(i);
            if (!pObj) continue;
            ar << ((pObj) ? pObj->nObjectId : -1);
        }
    }
    else
    {
        ar >> strObjectHelp;
        ar >> strObjectDef;

        int nb;
        ar >> nb;

        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = SerializeObj(ar);
            if (!pObj) continue;
            cParentList.Add(pObj);
        }
        //SetDepth();
    }
}

int CMacro3D::SetProperties(CxObject3DSet *pSet)
{
    int bRet = CCompositeObj3D::SetProperties(pSet);
    return bRet;
}

CString CMacro3D::GetObjectDef()
{
    if (strObjectDef == _T(""))
        return CCompositeObj3D::GetObjectDef();
    else
    {
        CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
        sName = GetObjectName();
        sFormat = strObjectDef;
        mstr.Format(sFormat,sName);
        return mstr;
    }
}

CString CMacro3D::GetObjectHelp()
{
    if (strObjectHelp == _T(""))
        return CCompositeObj3D::GetObjectHelp();
    else
    {
        CString mstr,mstr2;
        mstr = strObjectHelp;
        mstr2.Format(mstr,GetObjectName());
        return mstr2;
    }
}

CString CMacro3D::GetObjectName()
{
    return CCompositeObj3D::GetObjectName();
}

CString CMacro3D::ExportSymbolic(int nFormat)
{
    CString mstr;
	int nb = m_cSubObjects.GetSize();
	for (int i=0;i<nb;i++)
	{
		CObject3D * pObj = m_cSubObjects.GetAt(i);
		if (!pObj) continue;
		mstr += pObj->ExportSymbolic(nFormat);
		mstr += _T("\n");
	}
	return mstr;
}

void CMacro3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSM)
{
    if ((!IsVisible()) || (!bValidate) /*|| (!IsInCalque(vp->nCalqueNum))*/) return;
    CCompositeObj3D::Draw(pDC,vp,bSM);
    if (bIsSelected)
    {
        int nb = m_cSubObjects.GetSize();
        for (int i=nStartShow;i<nb;i++)
        {
            CObject3D * pObj = m_cSubObjects.GetAt(i);
            if (!pObj) continue;
            //if (!pObj->MaskObject(TAllPointClass)) continue;

            if ((!pObj->IsVisible()) || (!pObj->bValidate) ||
                (!pObj->IsInCalque(vp->nCalqueNum))) continue;

            pObj->DrawSelected(pDC,vp);
            //obj->Draw(pDC,vp,bSM);
            //int Ds = TPref::TPoint.Size+2;
            //CPoint    Pt = ((CPoint3D*)pObj)->Visual_pt;
            //CRect theR(Pt.x-Ds+1,Pt.y-Ds+1,Pt.x+Ds,Pt.y+Ds);
            //pDC->InvertRect(theR);
        }
    }
}
