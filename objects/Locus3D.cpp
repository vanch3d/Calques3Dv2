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
	for (i=0;i<limit;i++)
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
    curPen.CreatePenIndirect(&(pSrc->pObjectShape.GetPenStyle()));

    disPen.CreatePenIndirect(&(pSrc->pObjectShape.GetHiddenPenStyle(
                pSrc->pObjectShape.GetObjectHiddenColor())));

    CPen curPen2,disPen2;
    LOGPEN lPen1,lPen2;

    curPen.GetLogPen(&lPen1);
    lPen1.lopnStyle = PS_DOT;
    lPen1.lopnColor = RGB(0,192,0);//pObjectShape.GetObjectHiddenColor();
    disPen.GetLogPen(&lPen2);
    lPen2.lopnStyle = PS_DOT;
    lPen2.lopnColor = RGB(0,255,0);//pObjectShape.GetObjectHiddenColor();

    curPen2.CreatePenIndirect(&lPen1);
    disPen2.CreatePenIndirect(&lPen2);

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

		BOOL isVisible = (mV->IsPointVisible(vertex3D[0]) && mV->IsPointVisible(vertex3D[1]));
		pDC->SelectObject((isVisible)?(isFront? &curPen:&disPen):(isFront? &curPen2:&disPen2));
		pDC->MoveTo(vertex2D[0]);
		pDC->LineTo(vertex2D[1]);

		isVisible = (mV->IsPointVisible(vertex3D[1]) && mV->IsPointVisible(vertex3D[2]));
		pDC->SelectObject((isVisible)?(isFront? &curPen:&disPen):(isFront? &curPen2:&disPen2));
		pDC->MoveTo(vertex2D[1]);
		pDC->LineTo(vertex2D[2]);

// 		isVisible = (mV->IsPointVisible(vertex3D[2]) && mV->IsPointVisible(vertex3D[0]));
// 		pDC->SelectObject((isVisible)?((dot<0)? &curPen:&disPen):((dot<0)? &curPen2:&disPen2));
// 		pDC->MoveTo(vertex2D[2]);
// 		pDC->LineTo(vertex2D[0]);
	}

    pDC->SelectObject(pOldP);
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
    m_cCpts.RemoveAll();
    m_cVpts.RemoveAll();
    m_cCpts.SetSize(nDeltaT);
    m_cVpts.SetSize(nDeltaT);
    m_bFirstLoaded = FALSE;
}

CLocus3D::CLocus3D(CPoint3D *pSource,CPoint3D *pLocus) : CObject3D()
{
	m_nMaxLenght = 350;
    m_pSource = pSource;
    m_pLocus = pLocus;
    nDeltaT = 20;
    m_cCpts.RemoveAll();
    m_cVpts.RemoveAll();
    m_cCpts.SetSize(nDeltaT);
    m_cVpts.SetSize(nDeltaT);
    SetDepth();//nDepth = max(m_pSource->nDepth,m_pLocus->nDepth)+1;
    m_bFirstLoaded = FALSE;
}

CLocus3D::CLocus3D(const CObject3D &src ) : CObject3D(src)
{
	m_nMaxLenght = ((CLocus3D&)src).m_nMaxLenght;
    m_pSource = ((CLocus3D&)src).m_pSource;
    m_pLocus = ((CLocus3D&)src).m_pLocus;
    nDeltaT = ((CLocus3D&)src).nDeltaT;
    m_cCpts.RemoveAll();
    m_cVpts.RemoveAll();
    m_bFirstLoaded = FALSE;
/*  for (int i=0;i<nDeltaT;i++)
    {
        CVector4 mPt4= ((CLocus3D&)src).m_cCpts.GetAt(i);
        CPoint mPt = ((CLocus3D&)src).m_cVpts.GetAt(i);
        m_cCpts.Add(mPt4);
        m_cVpts.Add(mPt);
    }*/

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
    bool bIsInSeg = FALSE;

    int nb1 = mesh2D.GetSize();

    for (int i=0;i<nb1 && !bIsInSeg;i++)
    {
        int nb2 = mesh2D[i].GetSize();
        for (int j=0;j<nb2-1 && !bIsInSeg;j++)
        {
            CPoint  p1,p2;

            p1 = mesh2D[i][j];
            p2 = mesh2D[i][j+1];
            CRgn *pRgn = DoSegRgn(p1,p2);
            bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
            delete  pRgn;
        }
    }

/*  for (int t=0;((t<nDeltaT-1) && (!bIsInSeg));t++)
     {
        CPoint  p1,p2;

        p1 = m_cVpts.GetAt(t);
        p2 = m_cVpts.GetAt(t+1);

        CRgn *pRgn = DoSegRgn(p1,p2);
        bIsInSeg = (pRgn && pRgn->PtInRegion(thePt));
        delete  pRgn;
     }*/
    return bIsInSeg;
}

CRgn* CLocus3D::InvalideRect()
{
/*  typedef CArrayEx<CVector4,CVector4> CIntArray;

    int i,j;
    CString strFormat;
    CArrayEx<CIntArray, CIntArray &> a2D;

    // add 5 rows
    for(i = 0;i < 5; i++)
    {
        CIntArray aInt;
        a2D.Add(aInt);
    }

    // add 10 column for each row
    for(i = 0;i < a2D.GetSize(); i++)
    {
        for(j = 0;j < 10;j ++)
        {
            a2D[i].Add(CVector4(i,j,0));
        }
    }

    // print 2d matrix
    for(i = 0;i < a2D.GetSize(); i++)
    {
        //CString strOutput;
        //strOutput.Empty();
        for(j = 0;j < a2D[i].GetSize();j ++)
        {
            strFormat.Format("%g %g %g",a2D[i][j].x,a2D[i][j].y,a2D[i][j].z);
            //stOutput += strFormat;
        }
    }
*/
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
	TRACE1("dimension of mesh: %d\n",nb1);
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
	}
	else
	{
		for (int i=0;i<nb1-1;i++)
		{
			int nb2 = mesh3D[i].GetSize();
			int nb3 = mesh3D[i+1].GetSize();
			for (int j=0;j<nb2-1;j++)
			{
				CVector4 A = mesh3D[i].GetAt(j);
				CVector4 B = mesh3D[i].GetAt(j+1);
				if ((j+1)<nb3)
				{
					CVector4 C = mesh3D[i+1].GetAt(j+1);
					CVector4 D = mesh3D[i+1].GetAt(j);

					CLocusMesh elt = CLocusMesh(A,B,C);
					if (elt.GetLength() <= m_nMaxLenght)
						m_cTriangles.Add(elt);
					elt = CLocusMesh(C,D,A);
					if (elt.GetLength() <= m_nMaxLenght)
						m_cTriangles.Add(elt);
				}
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
    for (ii=0;ii<pDirectList.GetSize();ii++)
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

//     FCoord oldPt,oldPt2;
//     if (pPtSurD)
//         oldPt = pPtSurD->lambda;
//     else if (pPtSurC)
//         oldPt = pPtSurC->lambda;
//     else if (pPtSurS)
//     {
//         oldPt = pPtSurS->latit;
//         oldPt2 = pPtSurS->longi;
//     }
//     else if (pPtSurP)
//     {
//         oldPt = pPtSurP->alpha;
//         oldPt2 = pPtSurP->beta;
//     }
// 
//     mesh3D.RemoveAll();
// 
//     CVArray aInt;

    if (pPtSurS || pPtSurP)
    {
		GenerateSurface(&pDirectList);
    }
    else
    {
		GenerateLinear(&pDirectList);
    }
	GenerateMesh();

//     if (pPtSurD)
//         pPtSurD->lambda = oldPt;
//     else if (pPtSurC)
//         pPtSurC->lambda = oldPt;
//     else if (pPtSurS)
//     {
//         pPtSurS->latit = oldPt;
//         pPtSurS->longi = oldPt2;
//     }
//     else if (pPtSurP)
//     {
//         pPtSurP->alpha = oldPt;
//         pPtSurP->beta = oldPt2;
//     }
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
    //m_cVpts.RemoveAll();
    //m_cVpts.SetSize(nDeltaT);
    if (m_bFirstLoaded)
    {
        CPointSurP3D *pPtSurP = DYNAMIC_DOWNCAST(CPointSurP3D,m_pSource);
        if (pPtSurP)
        {
            CalculConceptuel();
        }
        m_bFirstLoaded = FALSE;
    }


    CVector4 mvvv;
    CPArray aPt;
    mesh2D.RemoveAll();

    int nbbb = mesh3D.GetSize();
    for (int ii=0;ii<nbbb;ii++)
    {
        mesh2D.Add(aPt);
        int nbb3 = mesh3D[ii].GetSize();
        for (int jj=0;jj<nbb3;jj++)
        {
            mvvv = mesh3D[ii].GetAt(jj);
            CPoint pt  = (CPoint)pVisParam->ProjectPoint(mvvv);
            mesh2D[ii].Add(pt);
        }
    }

/*  for (int t=0;t<nDeltaT;t++)
    {
        CVector4 V = m_cCpts.GetAt(t);
        CPoint pt = (CPoint)myVisuParam->ProjectPoint(V);
        m_cVpts.SetAt(t,pt);
    }*/

	// Compute the visual location of the mash
	for (int k=0;k<m_cTriangles.GetSize();k++)
	{
		CLocusMesh trg = m_cTriangles.GetAt(k);
		trg.CalculVisuel(pVisParam);
		m_cTriangles.SetAt(k,trg);
	}

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
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

    CPen curPen,disPen;
    curPen.CreatePenIndirect(&(pObjectShape.GetPenStyle()));

    disPen.CreatePenIndirect(&(pObjectShape.GetHiddenPenStyle(
                pObjectShape.GetObjectHiddenColor())));

    CPen curPen2,disPen2;
    LOGPEN lPen1,lPen2;

    curPen.GetLogPen(&lPen1);
    lPen1.lopnStyle = PS_SOLID;
    lPen1.lopnColor = RGB(192,192,192);//pObjectShape.GetObjectHiddenColor();
    disPen.GetLogPen(&lPen2);
    lPen2.lopnStyle = PS_SOLID;
    lPen2.lopnColor = RGB(192,192,192);//pObjectShape.GetObjectHiddenColor();

    curPen2.CreatePenIndirect(&lPen1);
    disPen2.CreatePenIndirect(&lPen2);

// 	// Draw the alternative mesh
//     CPen *pOldPn = pDC->SelectObject(&curPen2);
// 	for (int k=0;k<m_cTriangles.GetSize();k++)
// 	{
// 		CLocusMesh trg = m_cTriangles.GetAt(k);
// 		trg.Draw(pDC,mV,this);
// 	}
//     pDC->SelectObject(pOldPn);
// 
// 	return;

	
    CBrush curBrush(pObjectShape.clrObject);
    CBrush *pOldB = pDC->SelectObject(&curBrush);

	CArray<CPoint,CPoint> draw1,draw2,draw3,draw4;

    CPointSurS3D *pPtSurS = DYNAMIC_DOWNCAST(CPointSurS3D,m_pSource);
    CPointSurP3D *pPtSurP = DYNAMIC_DOWNCAST(CPointSurP3D,m_pSource);

    int nb1 = mesh2D.GetSize();
    for (int i=0;i<nb1;i+=1)
    {
        int nb2 = mesh2D[i].GetSize();
        for (int j=0;j<nb2-1;j++)
        {
            CVector4    U,V;
            bool        bVis1=FALSE;

            U = mesh3D[i].GetAt(j),
            V = mesh3D[i].GetAt(j+1);


            FCoord dis = (V-U).Norme();

            bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));

            if (dis>600) continue;

            CPen *pOldP = NULL;
            int x=-1;
            if (nb1>1 && (pPtSurS||pPtSurP))
            {
                CVector4 p1 = mesh3D[i][j];
                CVector4 p2 = mesh3D[i][j+1];
                CVector4 /*p3,*/p4;
                if (i!=nb1-1)
                {
                    //p3 = mesh3D[i+1][j];
					int sarr = mesh3D[i+1].GetSize();
					if (j+1>=sarr)
						 x = bVis1 ? 0 : 1;
					else
						p4 = mesh3D[i+1][j+1];
                }
                else
                {
					int sarr = mesh3D[0].GetSize();
					if (j+1>=sarr)
						 x = bVis1 ? 0 : 1;
					else
						p4 = mesh3D[0][j+1];
                }

				if (x==-1)
				{
					CVector4    FaceNorm = ((p2 - p1) % (p4 - p1));
					FCoord n1 = FaceNorm.Norme();
					FaceNorm = FaceNorm * (1/n1);
					CVector4 oeil= mV->GetEyePos();
					CVector4 origin(0,0,0,1);
					CVector4 VisuNorm= oeil -
						((mV->bParProj) ? origin : p1);
					FCoord n2 = VisuNorm.Norme();
					VisuNorm = VisuNorm*(1/n2);
					FCoord dot = VisuNorm * FaceNorm;
					int vis = 0;
					if (pPtSurS)
						vis = (dot > 0) ? 0 : 4;
					if (pPtSurP)
						vis = (dot < 0) ? 0 : 4;

					if (vis)
					{
						x = bVis1 ? 2 : 3;
					}
					else
					{
						x = bVis1 ? 0 : 1;
					}
				}
            }
            else
            {
                x = bVis1 ? 0 : 1;
            }

            CPoint start = mesh2D[i][j];
            CPoint endpt = mesh2D[i][j+1];

            if (x==0)
            {
				draw1.Add(start);
				draw1.Add(endpt);
            }
            if (x==1)
            {
				draw2.Add(start);
				draw2.Add(endpt);

            }
            if (x==2)
            {
				draw3.Add(start);
				draw3.Add(endpt);
            }
            if (x==3)
            {
				draw4.Add(start);
				draw4.Add(endpt);
            }
        }
    }
    if (nb1>1)
    {
        for (int i=0;i<nb1-1;i++)
        {
            int nb2 = mesh2D[i].GetSize();
            for (int j=0;j<nb2;j++)
            {
                CVector4    U,V;
                bool        bVis1=FALSE;

                int nbb1 = mesh3D[i].GetSize();
                int nbb2 = mesh3D[i+1].GetSize();

                if (!nbb1 || !nbb2) continue;
				if (j>=nbb1 || (j+1)>=nbb2) continue;

                U = mesh3D[i].GetAt(j),
                V = mesh3D[i+1].GetAt(j);

                bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));

                FCoord dis = (V-U).Norme();
                if (dis>600) continue;
                CPen *pOldP = NULL;

                int x=-1;

                if (pPtSurS || pPtSurP)
                {
                    CVector4 p1,p2,p3/*,p4*/;
                    p1 = mesh3D[i][j];
                    p3 = mesh3D[i+1][j];
                    if (j!=nb2-1)
                    {	
						int sarr = mesh3D[i].GetSize();
						if (j+1>=sarr)
		                    x = bVis1 ? 0 : 1;
						else
							p2 = mesh3D[i][j+1];
							//p4 = mesh3D[i+1][j+1];
                    }
                    else
                    {
						int sarr = mesh3D[i].GetSize();
						if (0>=sarr)
		                    x = bVis1 ? 0 : 1;
						else
							p2 = mesh3D[i][0];
							//p4 = mesh3D[i+1][0];
                    }

					if (x==-1)
					{
						CVector4    FaceNorm = ((p2 - p1) % (p3 - p1));
						FCoord n1 = FaceNorm.Norme();
						FaceNorm = FaceNorm * (1/n1);
						CVector4 oeil= mV->GetEyePos();
						CVector4 origin(0,0,0,1);
						CVector4 VisuNorm= oeil -
							((mV->bParProj) ? origin : p1);
						FCoord n2 = VisuNorm.Norme();
						VisuNorm = VisuNorm*(1/n2);
						FCoord dot = VisuNorm * FaceNorm;
						int vis = 0;
						if (pPtSurS)
							vis = (dot > 0) ? 0 : 4;
						if (pPtSurP)
							vis = (dot < 0) ? 0 : 4;

						if (vis)
						{
							//**pOldP = pDC->SelectObject(bVis1 ? &curPen2 : &disPen2);
							x = bVis1 ? 2 : 3;
						}
						else
						{
							//**pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
							x = bVis1 ? 0 : 1;
						}
					}
                }
                else
                {
                    //**pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
                    x = bVis1 ? 0 : 1;
                }

                CPoint start = mesh2D[i][j];
                CPoint endpt = mesh2D[i+1][j];
            if (x==0)
            {
//                 bpt1[npt1] = PT_MOVETO;
//                 pt1[npt1++] = start;
//                 bpt1[npt1] = PT_LINETO;
//                 pt1[npt1++] = endpt;
				draw1.Add(start);
				draw1.Add(endpt);
            }
            if (x==1)
            {
//                 bpt2[npt2] = PT_MOVETO;
//                 pt2[npt2++] = start;
//                 bpt2[npt2] = PT_LINETO;
//                 pt2[npt2++] = endpt;
				draw2.Add(start);
				draw2.Add(endpt);
            }
            if (x==2)
            {
//                 bpt3[npt3] = PT_MOVETO;
//                 pt3[npt3++] = start;
//                 bpt3[npt3] = PT_LINETO;
//                 pt3[npt3++] = endpt;
				draw3.Add(start);
				draw3.Add(endpt);
            }
            if (x==3)
            {
//                 bpt4[npt4] = PT_MOVETO;
//                 pt4[npt4++] = start;
//                 bpt4[npt4] = PT_LINETO;
//                 pt4[npt4++] = endpt;
				draw4.Add(start);
				draw4.Add(endpt);
            }
//**                pDC->MoveTo(start);
//**                pDC->LineTo(endpt);

/*          if (j==0)
            {
                CRect mrect(start,start);
                mrect.InflateRect(5,5,5,5);
                CRect mrect2(endpt,endpt);
                mrect2.InflateRect(5,5,5,5);
                pDC->Rectangle(mrect);
                pDC->Ellipse(mrect2);
            }*/

//**                pDC->SelectObject(pOldP);
            }
        }
    }

//  CPen *pOldP = pDC->SelectObject(bVis1 ? &curPen2 : &disPen2);
//  x = bVis1 ? 2 : 3;
//  }

    CPen *pOldP = pDC->SelectObject(&curPen2);
	int nn = draw3.GetSize();
	for (i=0;i<nn;i=i+2)
	{
		pDC->MoveTo(draw3[i]);
		pDC->LineTo(draw3[i+1]);
	}
//     for (i=0;i<npt3;i++)
//     {
//         if (bpt3[i]==PT_MOVETO)
//             pDC->MoveTo(pt3[i]);
//         else
//             pDC->LineTo(pt3[i]);
//     }
    //pDC->PolyDraw(pt3,bpt3,npt3);
    pDC->SelectObject(&disPen2);
	nn = draw4.GetSize();
	for (i=0;i<nn;i=i+2)
	{
		pDC->MoveTo(draw4[i]);
		pDC->LineTo(draw4[i+1]);
	}
//     for (i=0;i<npt4;i++)
//     {
//         if (bpt4[i]==PT_MOVETO)
//             pDC->MoveTo(pt4[i]);
//         else
//             pDC->LineTo(pt4[i]);
//     }
//  pDC->PolyDraw(pt4,bpt4,npt4);
    pDC->SelectObject(&curPen);
	nn = draw1.GetSize();
	for (i=0;i<nn;i=i+2)
	{
		pDC->MoveTo(draw1[i]);
		pDC->LineTo(draw1[i+1]);
	}
//     for (i=0;i<npt1;i++)
//     {
//         if (bpt1[i]==PT_MOVETO)
//             pDC->MoveTo(pt1[i]);
//         else
//             pDC->LineTo(pt1[i]);
//     }
//  pDC->PolyDraw(pt1,bpt1,npt1);
    pDC->SelectObject(&disPen);
	nn = draw2.GetSize();
	for (i=0;i<nn;i=i+2)
	{
		pDC->MoveTo(draw2[i]);
		pDC->LineTo(draw2[i+1]);
	}
//     for (i=0;i<npt2;i++)
//     {
//         if (bpt2[i]==PT_MOVETO)
//             pDC->MoveTo(pt2[i]);
//         else
//             pDC->LineTo(pt2[i]);
//     }
    //pDC->PolyDraw(pt2,bpt2,npt2);
    pDC->SelectObject(pOldP);


//     delete []bpt1;
//     delete []bpt2;
//     delete []bpt3;
//     delete []bpt4;
//     delete []pt1;
//     delete []pt2;
//     delete []pt3;
//     delete []pt4;


    //  else
//  {
//  pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
//  x = bVis1 ? 0 : 1;

/*  for (int t=0;t<nDeltaT-1;t++)
    {
        CVector4    U,V;
        bool        bVis1=FALSE;

        U = m_cCpts.GetAt(t),
        V = m_cCpts.GetAt(t+1);

        bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));

        CPen *pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);

        CPoint start = m_cVpts[t];
        CPoint endpt = m_cVpts[t+1];
        pDC->MoveTo(start);
        pDC->LineTo(endpt);

        pDC->SelectObject(pOldP);
    }*/
    pDC->SelectObject(pOldB);
    if (bIsSelected)
    {
        int         Ds = TPref::TPoint.nSize;

    /*  CPoint tp1 = m_cVpts[0];
        CPoint tp2 = m_cVpts[nDeltaT/2];
        CPoint tp3 = m_cVpts[nDeltaT/4];
        CPoint tp4 = m_cVpts[3*nDeltaT/4];
        CPoint tp5 = m_cVpts[nDeltaT-1];
        CRect theR1(tp1.x-Ds,tp1.y-Ds,tp1.x+Ds,tp1.y+Ds);
        CRect theR2(tp2.x-Ds,tp2.y-Ds,tp2.x+Ds,tp2.y+Ds);
        CRect theR3(tp3.x-Ds,tp3.y-Ds,tp3.x+Ds,tp3.y+Ds);
        CRect theR4(tp4.x-Ds,tp4.y-Ds,tp4.x+Ds,tp4.y+Ds);
        CRect theR5(tp5.x-Ds,tp5.y-Ds,tp5.x+Ds,tp5.y+Ds);
        pDC->InvertRect(theR1);
        pDC->InvertRect(theR2);
        pDC->InvertRect(theR3);
        pDC->InvertRect(theR4);
        pDC->InvertRect(theR5);*/
     }
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
		for (int i=0;i<nb1-1;i+=1)
		{
			int nb2 = mesh3D[i].GetSize();
			int nb3 = mesh3D[i+1].GetSize();
			for (int j=0;j<__min(nb2,nb3)-1;j++)
			{
				CVector4 pt1 = mesh3D[i].GetAt(j);
				CVector4 pt2 = mesh3D[i].GetAt(j+1);
				CVector4 pt3 = mesh3D[i+1].GetAt(j+1);
				CVector4 pt4 = mesh3D[i+1].GetAt(j);
				CVector4 n = pt2-pt1;
				CVector4 m = pt4-pt1;
				CVector4 k = n % m;
				k.Normalized();
				
				FCoord scale = 1/(TPref::TUniv.nUnitRep*3.);
				pt1=pt1*scale;
				pt2=pt2*scale;
				pt3=pt3*scale;
				pt4=pt4*scale;
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
					glVertex3d (pt4.x, pt4.y, pt4.z);
					glVertex3d (pt4.x, pt4.y, pt4.z);
					glVertex3d (pt1.x, pt1.y, pt1.z);
					glEnd();
				}
				else
				{
					glBegin (GL_QUADS);
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
					glColor3f (1.0f, 1.0f, 1.0f);
					glVertex3d (pt4.x, pt4.y, pt4.z);
					glEnd();
				}
			}
		}
		if (nVolMode==RENDER_STIPPLE) glDisable(GL_POLYGON_STIPPLE);
		glPopMatrix();

	}
}

