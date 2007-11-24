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
// Locus3D.cpp: implementation of the CLocus3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "..\Prefs\Prefs.h"
#include "..\OGLTools\glut.h"

#include "Locus3D.h"

#include "Point3D.h"
#include "Plan3D.h"
#include "Cercle3D.h"
#include "Droite3D.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLocusMesh::CLocusMesh()
{
	bIsLinear = false;
}

CLocusMesh::CLocusMesh(CVector4 pt1,CVector4 pt2, CVector4 pt3)
{
	bIsLinear = false;
	vertex3D[0] = pt1;
	vertex3D[1] = pt2;
	vertex3D[2] = pt3;
}

CLocusMesh::CLocusMesh(CVector4 pt1,CVector4 pt2)
{
	bIsLinear = true;
	vertex3D[0] = pt1;
	vertex3D[1] = pt2;
}

inline void CLocusMesh::operator =(const CLocusMesh& other)
{
	bIsLinear = other.bIsLinear;
	int limit = (bIsLinear)? 2:3;
	for (int i=0;i<limit;i++)
		vertex3D[i] = other.vertex3D[i];
	for (int i=0;i<limit;i++)
		vertex2D[i] = other.vertex2D[i];
}

inline FCoord CLocusMesh::GetLength()
{
	if (bIsLinear)
		return	(vertex3D[1] - vertex3D[0]).Norme();
	else
		return	(vertex3D[2] - vertex3D[1]).Norme() + 
				(vertex3D[1] - vertex3D[0]).Norme() + 
				(vertex3D[0] - vertex3D[2]).Norme();
}

void CLocusMesh::CalculVisuel(CVisualParam *mV)
{
	if (mV==NULL) return;
	int limit = (bIsLinear)? 2:3;
	for (int i=0;i<limit;i++)
		vertex2D[i] = mV->ProjectPoint(vertex3D[i]);
}

void CLocusMesh::Draw(CDC* pDC,CVisualParam *mV,CLocus3D *pSrc)
{
	if (!mV || !pSrc) return;

    CPointSurS3D *pPtSurS = DYNAMIC_DOWNCAST(CPointSurS3D,pSrc->m_pSource);

    CPen curPen,disPen;
    CPen curPen2,disPen2;

	if (!pSrc->bVisible && TPref::TUniv.bShowHidden)
	{
	    curPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
		disPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
	    curPen2.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
		disPen2.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
	}
	else
	{
	    curPen.CreatePenIndirect(&(pSrc->pObjectShape.GetPenStyle()));

		disPen.CreatePenIndirect(&(pSrc->pObjectShape.GetHiddenPenStyle(
                pSrc->pObjectShape.GetObjectHiddenColor())));

		LOGPEN lPen1,lPen2;
	    curPen.GetLogPen(&lPen1);
		lPen1.lopnStyle = PS_DOT;
		lPen1.lopnWidth = CPoint(1,1);
		disPen.GetLogPen(&lPen2);
		lPen2.lopnStyle = PS_SOLID;
		lPen2.lopnWidth = CPoint(1,1);
	    curPen2.CreatePenIndirect(&lPen1);
		disPen2.CreatePenIndirect(&lPen2);
	}
    
	
	//CPen curPen,disPen;
    //curPen.CreatePenIndirect(&(pSrc->pObjectShape.GetPenStyle()));

    //disPen.CreatePenIndirect(&(pSrc->pObjectShape.GetHiddenPenStyle(
    //            pSrc->pObjectShape.GetObjectHiddenColor())));

//     CPen curPen2,disPen2;
//     LOGPEN lPen1,lPen2;
// 
//     curPen.GetLogPen(&lPen1);
//     lPen1.lopnStyle = PS_DOT;
//     lPen1.lopnWidth = CPoint(1,1);
//     //lPen1.lopnColor = RGB(0,192,0);//pObjectShape.GetObjectHiddenColor();
//     disPen.GetLogPen(&lPen2);
//     lPen2.lopnStyle = PS_DOT;
//     lPen2.lopnWidth = CPoint(1,1);
//     //lPen2.lopnColor = RGB(0,255,0);//pObjectShape.GetObjectHiddenColor();
// 
//     curPen2.CreatePenIndirect(&lPen1);
//     disPen2.CreatePenIndirect(&lPen2);

    CPen *pOldP = pDC->SelectObject(&curPen);

	if (bIsLinear)
	{
		BOOL isVisible = (mV->IsPointVisible(vertex3D[0]) && mV->IsPointVisible(vertex3D[1]));
		pDC->SelectObject((isVisible)?&curPen:&disPen);
		pDC->MoveTo(vertex2D[0]);
		pDC->LineTo(vertex2D[1]);
	}
	else
	{
		CVector4    FaceNorm = ((vertex3D[1] - vertex3D[0]) % (vertex3D[2] - vertex3D[0]));
		FCoord n1 = FaceNorm.Norme();
		FaceNorm = FaceNorm * (1/n1);
		CVector4 oeil= mV->GetEyePos();
		CVector4 origin(0,0,0,1);
		CVector4 VisuNorm= oeil -
			((mV->bParProj) ? origin : vertex3D[0]);
		FCoord n2 = VisuNorm.Norme();
		VisuNorm = VisuNorm*(1/n2);
		FCoord dot = VisuNorm * FaceNorm;
		BOOL isFront = (pPtSurS) ? (dot > 0) : (dot < 0);
		
		CPen *selPen = NULL;
		BOOL isVisible = (mV->IsPointVisible(vertex3D[0]) && mV->IsPointVisible(vertex3D[1]));

		pDC->SelectObject((isVisible)?(isFront? &curPen:&curPen2):(isFront? &disPen:&disPen2));
		pDC->MoveTo(vertex2D[0]);
		pDC->LineTo(vertex2D[1]);

		isVisible = (mV->IsPointVisible(vertex3D[1]) && mV->IsPointVisible(vertex3D[2]));
		pDC->SelectObject((isVisible)?(isFront? &curPen:&curPen2):(isFront? &disPen:&disPen2));
		pDC->MoveTo(vertex2D[1]);
		pDC->LineTo(vertex2D[2]);

// 		isVisible = (mV->IsPointVisible(vertex3D[2]) && mV->IsPointVisible(vertex3D[0]));
// 		pDC->SelectObject((isVisible)?(isFront? &curPen:&curPen2):(isFront? &disPen:&disPen2));
// 		pDC->MoveTo(vertex2D[2]);
// 		pDC->LineTo(vertex2D[0]);
	}

    pDC->SelectObject(pOldP);
}

BOOL CLocusMesh::IsInActiveArea(CPoint thePt)
{
    bool bIsInSeg = FALSE;

	if (bIsLinear)
	{
		CRgn *pRgn = CObject3D::DoSegRgn(vertex2D[0],vertex2D[1]);
		bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
        delete  pRgn;
	}
	else
	{
		CRgn *pRgn = CObject3D::DoSegRgn(vertex2D[0],vertex2D[1]);
		bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
        delete  pRgn;
		if (!bIsInSeg)
		{
			CRgn *pRgn = CObject3D::DoSegRgn(vertex2D[1],vertex2D[2]);
			bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
			delete  pRgn;
		}
// 		if (!bIsInSeg)
// 		{
// 			CRgn *pRgn = DoSegRgn(vertex2D[2],vertex2D[0]);
// 			bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
// 			delete  pRgn;
// 		}
	}
    return bIsInSeg;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLocus3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CLocus3D::CLocus3D() : CObject3D()
{
	m_nMaxLenght = 350;
    m_pSource = m_pLocus = NULL;
    nDeltaT = 20;
    m_bFirstLoaded = FALSE;
}

CLocus3D::CLocus3D(CPoint3D *pSource,CPoint3D *pLocus) : CObject3D()
{
	m_nMaxLenght = 350;
    m_pSource = pSource;
    m_pLocus = pLocus;
    nDeltaT = 20;
    SetDepth();//nDepth = max(m_pSource->nDepth,m_pLocus->nDepth)+1;
    m_bFirstLoaded = FALSE;
}

CLocus3D::CLocus3D(const CObject3D &src ) : CObject3D(src)
{
	m_nMaxLenght = ((CLocus3D&)src).m_nMaxLenght;
    m_pSource = ((CLocus3D&)src).m_pSource;
    m_pLocus = ((CLocus3D&)src).m_pLocus;
    nDeltaT = ((CLocus3D&)src).nDeltaT;
    m_bFirstLoaded = FALSE;
}

int CLocus3D::SetDepth()
{
    if (m_pSource && m_pLocus)
        nDepth = max(m_pSource->nDepth,m_pLocus->nDepth)+1;
    return nDepth;
}



BOOL CLocus3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (m_pSource == pOld)
    {
        SetInGraph(FALSE);
        m_pSource = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (m_pLocus == pOld)
    {
        SetInGraph(FALSE);
        m_pLocus = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}


CLocus3D::~CLocus3D()
{

}

void CLocus3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << nDeltaT;
        ar << ((m_pSource) ? m_pSource->nObjectId : -1);
        ar << ((m_pLocus) ? m_pLocus->nObjectId : -1);
    }
    else
    {
        ar >> nDeltaT;
        m_pSource = (CPoint3D*)SerializeObj(ar);
        m_pLocus = (CPoint3D*)SerializeObj(ar);
        m_bFirstLoaded = TRUE;
    }
}

CObject3D* CLocus3D::CopyObject()
{
    CObject3D *temp = new CLocus3D((CObject3D&)*this);
    return temp;

}

CxObject3DSet* CLocus3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (m_pSource) list->Add(m_pSource);
    if (m_pLocus) list->Add(m_pLocus);
    return list;

}

CString CLocus3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???")),sn2(_T("???"));
    if (m_pSource) sn1 = m_pSource->GetObjectHelp();
    if (m_pLocus) sn2 = m_pLocus->GetObjectHelp();

    mstr.Format(sFormat,sName,sn2,sn1);
    return mstr;
}

BOOL CLocus3D::IsEqual(CObject3D &other)
{
    return FALSE;
}

BOOL CLocus3D::IsInActiveArea(CPoint thePt)
{
    BOOL bIsInSeg = FALSE;
	if (m_bFirstLoaded) return bIsInSeg; 

	for (int k=0;k<m_cTriangles.GetSize()&& !bIsInSeg;k++)
	{
		CLocusMesh trg = m_cTriangles.GetAt(k);
		bIsInSeg = trg.IsInActiveArea(thePt);
	}
    return bIsInSeg;
}

CRgn* CLocus3D::InvalideRect()
{
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
/// Create the mesh from the linear or planar distribution
///
/////////////////////////////////////////////////////////////////////////////
void CLocus3D::GenerateMesh()
{
	m_cTriangles.RemoveAll();
    int nb1 = mesh3D.GetSize();
	//TRACE1("dimension of mesh: %d\n",nb1);
	if (nb1<=0) return;

	if (nb1==1)
	{
		int nb2 = mesh3D[0].GetSize();
		for (int j=0;j<nb2-1;j++)
		{
			CVector4 A = mesh3D[0].GetAt(j);
			CVector4 B = mesh3D[0].GetAt(j+1);

			CLocusMesh elt = CLocusMesh(A,B);
			if (elt.GetLength() <= m_nMaxLenght)
				m_cTriangles.Add(elt);

		}
		return;
	}
	for (int i=0;i<nb1-1;i++)
	{
		CVArray fstLine = mesh3D[i];
		CVArray secLine = mesh3D[i+1];

		int nb2 = fstLine.GetSize();
		int nb3 = secLine.GetSize();
		int nItem = min(nb2,nb3);
		for (int j=0;j<nItem-1;j++)
		{
			CVector4 *pt1=NULL,*pt2=NULL,*pt3=NULL,*pt4=NULL;

			if (j<nb2)
				pt1 = &fstLine.GetAt(j);
			if ((j+1)<nb2)
				pt2 = &fstLine.GetAt(j+1);
			if ((j+1)<nb3)
				pt3 = &secLine.GetAt(j+1);
			if (j<nb3)
				pt4 = &secLine.GetAt(j);

			if (pt1 && pt2 && pt3)
			{
				CLocusMesh elt = CLocusMesh(*pt1,*pt2,*pt3);
				FCoord l1 = elt.GetLength() ;
				if (l1 <= m_nMaxLenght)
					m_cTriangles.Add(elt);
			}
			if (pt3 && pt4 && pt1)
			{
				CLocusMesh elt2 = CLocusMesh(*pt3,*pt4,*pt1);
				FCoord l1 = elt2.GetLength() ;
				if (l1 <= m_nMaxLenght)
					m_cTriangles.Add(elt2);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
/// Generate the linear distribution of the mesh's points
///
/////////////////////////////////////////////////////////////////////////////
void CLocus3D::GenerateLinear(CxObject3DSet* pDirectList)
{
    CPointSurD3D *pPtSurD = DYNAMIC_DOWNCAST(CPointSurD3D,m_pSource);
    CPointSurC3D *pPtSurC = DYNAMIC_DOWNCAST(CPointSurC3D,m_pSource);

    FCoord initX;
    if (pPtSurD)
        initX = pPtSurD->lambda;
    else if (pPtSurC)
        initX = pPtSurC->lambda;

    mesh3D.RemoveAll();

    CVArray aInt;
	mesh3D.Add(aInt);

    for (int i=0;i<nDeltaT;i++)
    {
        if (pPtSurD)
        {
            if (pPtSurD->S->bIsSegment)
                pPtSurD->lambda = (FCoord)i/(FCoord)(nDeltaT-1);
            else
            {
                CVector4 pDirV =pPtSurD->S->GetDirVector();
                FCoord pVecN = pDirV.Norme();
                pPtSurD->lambda = initX + (FCoord)(i-nDeltaT/2)*(FCoord)(nDeltaT-1);//*pDirV;
            }
        }
        else if (pPtSurC)
            pPtSurC->lambda = (FCoord)(i * pPtSurC->S->nArcAngle)/(FCoord)(nDeltaT-1);

        BOOL bOK = TRUE;
        for (int j=0;j<pDirectList->GetSize();j++)
        {
            CObject3D *pObj = pDirectList->GetAt(j);
            if (!pObj) continue;
            if (DYNAMIC_DOWNCAST(CLocus3D,pObj)) continue;
            UINT res = pObj->CalculConceptuel();
            if (res)
            {
                bOK = FALSE;
                continue;
            }
            if (pObj == m_pLocus) break;
        }
        CVector4 newpt = m_pLocus->Concept_pt;
        if (bOK) mesh3D[0].Add(newpt);
    }

    if (pPtSurD)
        pPtSurD->lambda = initX;
    else if (pPtSurC)
        pPtSurC->lambda = initX;

}

/////////////////////////////////////////////////////////////////////////////
/// Generate the planar distribution of the mesh's points
///
/////////////////////////////////////////////////////////////////////////////
void CLocus3D::GenerateSurface(CxObject3DSet* pDirectList)
{
    CPointSurS3D *pPtSurS = DYNAMIC_DOWNCAST(CPointSurS3D,m_pSource);
    CPointSurP3D *pPtSurP = DYNAMIC_DOWNCAST(CPointSurP3D,m_pSource);

    FCoord oldPt,oldPt2;
    if (pPtSurS)
    {
        oldPt = pPtSurS->latit;
        oldPt2 = pPtSurS->longi;
    }
    else if (pPtSurP)
    {
        oldPt = pPtSurP->alpha;
        oldPt2 = pPtSurP->beta;
    }

    mesh3D.RemoveAll();

	for (int i=0;i<=nDeltaT;i++)
	{
	    CVArray aInt;
		mesh3D.Add(aInt);
		for (int j=0;j<=nDeltaT;j++)
		{
			if (pPtSurS)
			{
				pPtSurS->latit = (FCoord)((i-nDeltaT/2) * M_PI)/(FCoord)(nDeltaT);
				pPtSurS->longi = (FCoord)(j * 2* M_PI)/(FCoord)(nDeltaT);
			}
			else
			{
				CVector4 ppa = pPtSurP->P->p1 - pPtSurP->P->ptonRep.O;
				CVector4 ppb = pPtSurP->P->p2 - pPtSurP->P->ptonRep.O;
				FCoord aa1 = pPtSurP->P->ptonRep.I * (ppa);
				FCoord aa2 = pPtSurP->P->ptonRep.J * (ppa);
				FCoord ee = (pPtSurP->P->p2 - pPtSurP->P->p1).Norme();//+40;
				FCoord dd = (pPtSurP->P->p4 - pPtSurP->P->p1).Norme();//+40;

				pPtSurP->alpha = (FCoord)(i * dd)/(FCoord)(nDeltaT-1) + aa1;
				pPtSurP->beta = (FCoord)(j * ee)/(FCoord)(nDeltaT-1) +aa2;
			}
			BOOL bOK = TRUE;
			for (int k=0;k<pDirectList->GetSize();k++)
			{
				CObject3D *pObj = pDirectList->GetAt(k);
				if (!pObj) continue;
				if (DYNAMIC_DOWNCAST(CLocus3D,pObj)) continue;
				UINT res = pObj->CalculConceptuel();
				if (res)
				{
					bOK  = FALSE;
					continue;
				}
				if (pObj == m_pLocus) break;
			}
			CVector4 newpt = m_pLocus->Concept_pt;
			if (bOK) mesh3D[i].Add(newpt);
		}
	}

    if (pPtSurS)
    {
        pPtSurS->latit = oldPt;
        pPtSurS->longi = oldPt2;
    }
    else if (pPtSurP)
    {
        pPtSurP->alpha = oldPt;
        pPtSurP->beta = oldPt2;
    }

}

UINT  CLocus3D::CalculConceptuel()
{
    bValidate = (m_pSource->bValidate && m_pLocus->bValidate);
    if (!bValidate)
        return ERR_NOLOCUS;

    CPointSurD3D *pPtSurD = DYNAMIC_DOWNCAST(CPointSurD3D,m_pSource);
    CPointSurC3D *pPtSurC = DYNAMIC_DOWNCAST(CPointSurC3D,m_pSource);
    CPointSurS3D *pPtSurS = DYNAMIC_DOWNCAST(CPointSurS3D,m_pSource);
    CPointSurP3D *pPtSurP = DYNAMIC_DOWNCAST(CPointSurP3D,m_pSource);
    if (!pPtSurD && !pPtSurC && !pPtSurS  && !pPtSurP)
    {
        bValidate = FALSE;
        return ERR_LOCUS_NOPTSUR;
    }

    CxObject3DSet pList;
    m_pSource->GetDependList(&pList);
    int nb = pList.GetSize();

    CxObject3DSet pDirectList;
    pDirectList.Add(m_pSource);
    for (int ii=0;ii<nb;ii++)
    {
        CObject3D *pObj = pList.GetAt(ii);
        if (!pObj) continue;
        if (pObj==m_pSource) continue;
        if (pObj==m_pLocus)
        {
            pDirectList.Add(pObj);
            break;
        }
        CxObject3DSet pList2;
        pObj->GetDependList(&pList2);
        for (int j=0;j<pList2.GetSize();j++)
        {
            CObject3D *pObj2 = pList2.GetAt(j);
            if (pObj2==m_pLocus)
            {
                pDirectList.Add(pObj);
                break;
            }
        }
    }
    for (int ii=0;ii<pDirectList.GetSize();ii++)
    {
        CObject3D *pObj = pDirectList.GetAt(ii);
    }

    BOOL bFound = FALSE;
    for (int i=0;i<nb && !bFound;i++)
    {
        CObject3D *pObj = pList.GetAt(i);
        if (pObj == m_pLocus)
        {
            bFound = TRUE;
            break;
        }
    }
    if (!bFound)
    {
        bValidate = FALSE;
        return ERR_LOCUS_NOREL;
    }

   if (pPtSurS || pPtSurP)
    {
		GenerateSurface(&pDirectList);
    }
    else
    {
		GenerateLinear(&pDirectList);
    }
	GenerateMesh();

	// Recompute the original position of the construction
    for (int j=0;j<nb;j++)
    {
        CObject3D *pObj = pList.GetAt(j);
        if (!pObj) continue;
        if (DYNAMIC_DOWNCAST(CLocus3D,pObj)) continue;
        pObj->CalculConceptuel();
        if (pObj == m_pLocus) break;
    }

    //InvalideRect();
    return 0;
}

void CLocus3D::CalculVisuel(CVisualParam *pVisParam)
{
    if (m_bFirstLoaded)
    {
        CPointSurP3D *pPtSurP = DYNAMIC_DOWNCAST(CPointSurP3D,m_pSource);
        if (pPtSurP)
        {
            CalculConceptuel();
        }
    }

	// Compute the visual location of the mesh
	for (int k=0;k<m_cTriangles.GetSize();k++)
	{
		CLocusMesh trg = m_cTriangles.GetAt(k);
		trg.CalculVisuel(pVisParam);
		m_cTriangles.SetAt(k,trg);
	}
	m_bFirstLoaded = FALSE;
}

CString CLocus3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (/*bValidate &&*/ m_pSource && m_pLocus)
    {
        CString strName,strObj1,strObj2;
		strName = GetObjectNameRedux();
		strObj1 = m_pSource->GetObjectNameRedux();
		strObj2 = m_pLocus->GetObjectNameRedux();

		if (nFormat==EXPORT_MATHEMATICA)
	        mstr.Format(_T("Locus[%s,%s,%s];"),strName,strObj2,strObj1);
		else if (nFormat==EXPORT_MAPLE)
		    mstr.Format(_T("Locus(%s,%s,%s);"),strName,strObj2,strObj1);
    }
    return mstr;
}



void CLocus3D::Draw(CDC *pDC,CVisualParam *mV,BOOL bSM)
{
    if ((!bVisible && !TPref::TUniv.bShowHidden) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;
//     CPen curPen,disPen;
//     CPen curPen2,disPen2;
// 
// 	if (!bVisible && TPref::TUniv.bShowHidden)
// 	{
// 	    curPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
// 		disPen.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
// 	    curPen2.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
// 		disPen2.CreatePen(PS_DOT,1,TPref::TUniv.clrShowHidden);
// 	}
// 	else
// 	{
// 	    curPen.CreatePenIndirect(&(pObjectShape.GetPenStyle()));
// 
// 		disPen.CreatePenIndirect(&(pObjectShape.GetHiddenPenStyle(
//                 pObjectShape.GetObjectHiddenColor())));
// 
// 		LOGPEN lPen1,lPen2;
// 	    curPen.GetLogPen(&lPen1);
// 		lPen1.lopnStyle = PS_SOLID;
// 		lPen1.lopnColor = RGB(192,192,192);//pObjectShape.GetObjectHiddenColor();
// 		disPen.GetLogPen(&lPen2);
// 		lPen2.lopnStyle = PS_SOLID;
// 		lPen2.lopnColor = RGB(192,192,192);//pObjectShape.GetObjectHiddenColor();
// 	    curPen2.CreatePenIndirect(&lPen1);
// 		disPen2.CreatePenIndirect(&lPen2);
// 	}

	// Draw the alternative mesh
//    CPen *pOldPn = pDC->SelectObject(&curPen2);
	for (int k=0;k<m_cTriangles.GetSize();k++)
	{
		CLocusMesh trg = m_cTriangles.GetAt(k);
		trg.Draw(pDC,mV,this);
	}
//    pDC->SelectObject(pOldPn);
}

void CLocus3D::Draw3DRendering(int nVolMode)
{
    float mat_diffuse[] = {0.9f, 0.1f, 0.1f, 1.0f};
    float no_shininess = 0.0f;
    if ((!bVisible) || (!bValidate)) return;
    int nb1 = mesh3D.GetSize();
	if (nb1==1)
	{
	}
	else
	{
		glPushMatrix();
		if (nVolMode==RENDER_STIPPLE) glEnable(GL_POLYGON_STIPPLE);

		for (int k1=0;k1<m_cTriangles.GetSize();k1++)
		{
			CLocusMesh trg = m_cTriangles.GetAt(k1);

			CVector4 pt1 = trg.vertex3D[0];
				CVector4 pt2 = trg.vertex3D[1];
				CVector4 pt3 = trg.vertex3D[2];
				CVector4 n = pt2-pt1;
				CVector4 m = pt3-pt1;
				CVector4 k = n % m;
				k.Normalized();
				
				FCoord scale = 1/(TPref::TUniv.nUnitRep*3.);
				pt1=pt1*scale;
				pt2=pt2*scale;
				pt3=pt3*scale;
				//pt4=pt4*scale;
				if (nVolMode==RENDER_STIPPLE) glPolygonStipple(stippleMask[4]);
				//glBegin (GL_TRIANGLES);
				//glBegin (GL_QUADS);
				if (nVolMode==RENDER_SILHOUETTE)
				{
					glBegin (GL_LINES);
					glNormal3d (k.x, k.y, k.z);
					glColor3f (1.0f, 0.0f, 0.f);
					glVertex3d (pt1.x, pt1.y, pt1.z);
					glVertex3d (pt2.x, pt2.y, pt2.z);
					glVertex3d (pt2.x, pt2.y, pt2.z);
					glVertex3d (pt3.x, pt3.y, pt3.z);
					glVertex3d (pt3.x, pt3.y, pt3.z);
					//glVertex3d (pt4.x, pt4.y, pt4.z);
					//glVertex3d (pt4.x, pt4.y, pt4.z);
					glVertex3d (pt1.x, pt1.y, pt1.z);
					glEnd();
				}
				else
				{
					glBegin (GL_TRIANGLES);
	//				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_diffuse);
	//				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	// 				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, no_shininess);
					glNormal3d (k.x, k.y, k.z);
					glColor3f (1.0f, 0.0f, 0.f);
					glVertex3d (pt1.x, pt1.y, pt1.z);
					glColor3f (0.0f, 1.0f, 0.f);
					glVertex3d (pt2.x, pt2.y, pt2.z);
					glColor3f (1.0f, 0.0f, 1.0f);
					glVertex3d (pt3.x, pt3.y, pt3.z);
					//glColor3f (1.0f, 1.0f, 1.0f);
					//glVertex3d (pt4.x, pt4.y, pt4.z);
					glEnd();
				}
		}
		if (nVolMode==RENDER_STIPPLE) glDisable(GL_POLYGON_STIPPLE);
		glPopMatrix();

	}
}

