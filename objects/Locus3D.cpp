// Locus3D.cpp: implementation of the CLocus3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "..\Prefs\Prefs.h"

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

//extern CRgn* DoSegRgn(CPoint p1,CPoint p2);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLocus3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CLocus3D::CLocus3D() : CObject3D()
{
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

    int nb1 = a2D.GetSize();

    for (int i=0;i<nb1 && !bIsInSeg;i++)
    {
        int nb2 = a2D[i].GetSize();
        for (int j=0;j<nb2-1 && !bIsInSeg;j++)
        {
            CPoint  p1,p2;

            p1 = a2D[i][j];
            p2 = a2D[i][j+1];
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

    FCoord oldPt,oldPt2;
    if (pPtSurD)
        oldPt = pPtSurD->lambda;
    else if (pPtSurC)
        oldPt = pPtSurC->lambda;
    else if (pPtSurS)
    {
        oldPt = pPtSurS->latit;
        oldPt2 = pPtSurS->longi;
    }
    else if (pPtSurP)
    {
        oldPt = pPtSurP->alpha;
        oldPt2 = pPtSurP->beta;
    }

    //m_cCpts.RemoveAll();
    //m_cCpts.SetSize(nDeltaT);

    a3D.RemoveAll();

    CVArray aInt;

    if (pPtSurS || pPtSurP)
    {
        for (i=0;i<=nDeltaT;i++)
        {
            a3D.Add(aInt);
            for (int j=0;j<=nDeltaT;j++)
            {
                if (pPtSurS)
                {
                    pPtSurS->latit = (FCoord)((i-nDeltaT/2) * M_PI)/(FCoord)(nDeltaT);
                    //if (j%2==0)
                    //  pPtSurS->longi = (FCoord)(j *2 * M_PI)/(FCoord)(nDeltaT-1);
                    //else
                    pPtSurS->longi = (FCoord)(j * 2* M_PI)/(FCoord)(nDeltaT);
                }
                else
                {
                    CVector4 ppa = pPtSurP->P->p1 - pPtSurP->P->ptonRep.O;
                    CVector4 ppb = pPtSurP->P->p2 - pPtSurP->P->ptonRep.O;
                    FCoord aa1 = pPtSurP->P->ptonRep.I * (ppa);
                    FCoord aa2 = pPtSurP->P->ptonRep.J * (ppa);
                    //FCoord aa3 = pPtSurP->P->ptonRep.I * (ppb);
                    //FCoord aa4 = pPtSurP->P->ptonRep.J * (ppb);
                    FCoord ee = (pPtSurP->P->p2 - pPtSurP->P->p1).Norme();//+40;
                    FCoord dd = (pPtSurP->P->p4 - pPtSurP->P->p1).Norme();//+40;
                    //FCoord rr = pPtSurP->P->p1.Norme();
                    //FCoord rr2 = pPtSurP->P->p2.Norme();

                //  dd = pPtSurP->P->ptonRep.I;
                //  ee = pPtSurP->P->ptonRep.J;

                    pPtSurP->alpha = (FCoord)(i * dd)/(FCoord)(nDeltaT-1) + aa1;
                    pPtSurP->beta = (FCoord)(j * ee)/(FCoord)(nDeltaT-1) +aa2;
                }
                BOOL bOK = TRUE;
                for (int k=0;k<pDirectList.GetSize();k++)
//              for (int k=0;k<nb;k++)
                {
//                  CObject3D *pObj = pList.GetAt(k);
                    CObject3D *pObj = pDirectList.GetAt(k);
                    if (!pObj) continue;
                    if (DYNAMIC_DOWNCAST(CLocus3D,pObj)) continue;
                    UINT res = pObj->CalculConceptuel();
                    if (res)
                    {
                        //bValidate = FALSE;
                        //return ERR_LOCUS_INTERNAL;
                        bOK  = FALSE;
                        continue;
                    }
                    if (pObj == m_pLocus) break;
                }
                CVector4 newpt = m_pLocus->Concept_pt;
                //m_cCpts.SetAt(i,newpt);
                if (bOK)
                    a3D[i].Add(newpt);
            }
        }
    }
    else
    {
        a3D.Add(aInt);

        for (i=0;i<nDeltaT;i++)
        {
            if (pPtSurD)
            {
                if (pPtSurD->S->bIsSegment)
                    pPtSurD->lambda = (FCoord)i/(FCoord)(nDeltaT-1);
                else
                {
                    CVector4 pDirV =pPtSurD->S->GetDirVector();
                    FCoord pVecN = pDirV.Norme();
                    pPtSurD->lambda = oldPt + (FCoord)(i-nDeltaT/2)*(FCoord)(nDeltaT-1);//*pDirV;
                }
            }
            else if (pPtSurC)
                //pPtSurC->lambda = (FCoord)(i * 2*M_PI)/(FCoord)(nDeltaT-1);
                pPtSurC->lambda = (FCoord)(i * pPtSurC->S->nArcAngle)/(FCoord)(nDeltaT-1);

            BOOL bOK = TRUE;
//          for (int j=0;j<nb;j++)
            for (int j=0;j<pDirectList.GetSize();j++)
            {
                ///CObject3D *pObj = pList.GetAt(j);
                CObject3D *pObj = pDirectList.GetAt(j);
                if (!pObj) continue;
                if (DYNAMIC_DOWNCAST(CLocus3D,pObj)) continue;
                UINT res = pObj->CalculConceptuel();
                if (res)
                {
                    //bValidate = FALSE;
                    //return ERR_LOCUS_INTERNAL;
                    bOK = FALSE;
                    continue;
                }
                if (pObj == m_pLocus) break;
            }
            CVector4 newpt = m_pLocus->Concept_pt;
            //m_cCpts.SetAt(i,newpt);
            if (bOK)
                a3D[0].Add(newpt);
        }
    }

    if (pPtSurD)
        pPtSurD->lambda = oldPt;
    else if (pPtSurC)
        pPtSurC->lambda = oldPt;
    else if (pPtSurS)
    {
        pPtSurS->latit = oldPt;
        pPtSurS->longi = oldPt2;
    }
    else if (pPtSurP)
    {
        pPtSurP->alpha = oldPt;
        pPtSurP->beta = oldPt2;
    }
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

void CLocus3D::CalculVisuel(CVisualParam *myVisuParam)
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
    a2D.RemoveAll();

    int nbbb = a3D.GetSize();
    for (int ii=0;ii<nbbb;ii++)
    {
        a2D.Add(aPt);
        int nbb3 = a3D[ii].GetSize();
        for (int jj=0;jj<nbb3;jj++)
        {
            mvvv = a3D[ii].GetAt(jj);
            CPoint pt  = (CPoint)myVisuParam->ProjectPoint(mvvv);
            a2D[ii].Add(pt);
        }
    }

/*  for (int t=0;t<nDeltaT;t++)
    {
        CVector4 V = m_cCpts.GetAt(t);
        CPoint pt = (CPoint)myVisuParam->ProjectPoint(V);
        m_cVpts.SetAt(t,pt);
    }*/
}

CString CLocus3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (bValidate && m_pSource && m_pLocus)
    {
        CString mstr2,strName,strObj1,strObj2;
        mstr2.LoadString(GetNameID());
        strName.Format("%s%d",mstr2,nObjectId);
        mstr2.LoadString(m_pSource->GetNameID());
        strObj1.Format("%s%d",mstr2,m_pSource->nObjectId);
        mstr2.LoadString(m_pLocus->GetNameID());
        strObj2.Format("%s%d",mstr2,m_pLocus->nObjectId);

        mstr.Format(_T("Locus[%s,%s,%s];"),strName,strObj2,strObj1);
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


    CBrush curBrush(pObjectShape.clrObject);
    CBrush *pOldB = pDC->SelectObject(&curBrush);

    BYTE  *bpt1 = new BYTE[10000];
    BYTE  *bpt2= new BYTE[10000];
    BYTE  *bpt3= new BYTE[10000];
    BYTE  *bpt4= new BYTE[10000];
    POINT *pt1= new POINT[10000];
    POINT *pt2= new POINT[10000];
    POINT *pt3= new POINT[10000];
    POINT *pt4= new POINT[10000];

    int npt1=0;
    int npt2=0;
    int npt3=0;
    int npt4=0;

    CPointSurS3D *pPtSurS = DYNAMIC_DOWNCAST(CPointSurS3D,m_pSource);
    CPointSurP3D *pPtSurP = DYNAMIC_DOWNCAST(CPointSurP3D,m_pSource);

    int nb1 = a2D.GetSize();
    for (int i=0;i<nb1;i+=1)
    {
        int nb2 = a2D[i].GetSize();
        for (int j=0;j<nb2-1;j++)
        {
            CVector4    U,V;
            bool        bVis1=FALSE;

            U = a3D[i].GetAt(j),
            V = a3D[i].GetAt(j+1);


            FCoord dis = (V-U).Norme();

            bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));

            if (dis>600) continue;

            CPen *pOldP = NULL;
            int x=-1;
            if (nb1>1 && (pPtSurS||pPtSurP))
            {
                CVector4 p1 = a3D[i][j];
                CVector4 p2 = a3D[i][j+1];
                CVector4 p3,p4;
                if (i!=nb1-1)
                {
                    p3 = a3D[i+1][j];
                    p4 = a3D[i+1][j+1];
                }
                else
                {
                    p3 = a3D[0][j];
                    p4 = a3D[0][j+1];
                }

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
                //if (pPtSurS && (i%2==0))
                //  vis = (vis==0) ? 4 : 0;

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
            else
            {
                //**pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
                x = bVis1 ? 0 : 1;
            }

            CPoint start = a2D[i][j];
            CPoint endpt = a2D[i][j+1];

            if (x==0)
            {
                bpt1[npt1] = PT_MOVETO;
                pt1[npt1++] = start;
                bpt1[npt1] = PT_LINETO;
                pt1[npt1++] = endpt;
            }
            if (x==1)
            {
                bpt2[npt2] = PT_MOVETO;
                pt2[npt2++] = start;
                bpt2[npt2] = PT_LINETO;
                pt2[npt2++] = endpt;
            }
            if (x==2)
            {
                bpt3[npt3] = PT_MOVETO;
                pt3[npt3++] = start;
                bpt3[npt3] = PT_LINETO;
                pt3[npt3++] = endpt;
            }
            if (x==3)
            {
                bpt4[npt4] = PT_MOVETO;
                pt4[npt4++] = start;
                bpt4[npt4] = PT_LINETO;
                pt4[npt4++] = endpt;
            }

//**            pDC->MoveTo(start);
//**            pDC->LineTo(endpt);


/*          if (j==0)
            {
                CRect mrect(start,start);
                mrect.InflateRect(5,5,5,5);
                CRect mrect2(endpt,endpt);
                mrect2.InflateRect(5,5,5,5);
                pDC->Rectangle(mrect);
                pDC->Ellipse(mrect2);
            }*/

        //**    pDC->SelectObject(pOldP);
        }
    }
    if (nb1>1)
    {
        for (int i=0;i<nb1-1;i++)
        {
            int nb2 = a2D[i].GetSize();
            for (int j=0;j<nb2;j++)
            {
                CVector4    U,V;
                bool        bVis1=FALSE;

                int nbb1 = a3D[i].GetSize();
                int nbb2 = a3D[i+1].GetSize();

                if (!nbb1 || !nbb2) continue;

                U = a3D[i].GetAt(j),
                V = a3D[i+1].GetAt(j);

                bVis1 = (mV->IsPointVisible(U) && mV->IsPointVisible(V));

                FCoord dis = (V-U).Norme();
                if (dis>600) continue;
                CPen *pOldP = NULL;

                int x=-1;

                if (pPtSurS || pPtSurP)
                {
                    CVector4 p1,p2,p3,p4;
                    p1 = a3D[i][j];
                    p3 = a3D[i+1][j];
                    if (j!=nb2-1)
                    {
                        p2 = a3D[i][j+1];
                        p4 = a3D[i+1][j+1];
                    }
                    else
                    {
                        p2 = a3D[i][0];
                        p4 = a3D[i+1][0];
                    }

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
                else
                {
                    //**pOldP = pDC->SelectObject(bVis1 ? &curPen : &disPen);
                    x = bVis1 ? 0 : 1;
                }

                CPoint start = a2D[i][j];
                CPoint endpt = a2D[i+1][j];
            if (x==0)
            {
                bpt1[npt1] = PT_MOVETO;
                pt1[npt1++] = start;
                bpt1[npt1] = PT_LINETO;
                pt1[npt1++] = endpt;
            }
            if (x==1)
            {
                bpt2[npt2] = PT_MOVETO;
                pt2[npt2++] = start;
                bpt2[npt2] = PT_LINETO;
                pt2[npt2++] = endpt;
            }
            if (x==2)
            {
                bpt3[npt3] = PT_MOVETO;
                pt3[npt3++] = start;
                bpt3[npt3] = PT_LINETO;
                pt3[npt3++] = endpt;
            }
            if (x==3)
            {
                bpt4[npt4] = PT_MOVETO;
                pt4[npt4++] = start;
                bpt4[npt4] = PT_LINETO;
                pt4[npt4++] = endpt;
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
    for (i=0;i<npt3;i++)
    {
        if (bpt3[i]==PT_MOVETO)
            pDC->MoveTo(pt3[i]);
        else
            pDC->LineTo(pt3[i]);
    }
    //pDC->PolyDraw(pt3,bpt3,npt3);
    pDC->SelectObject(&disPen2);
    for (i=0;i<npt4;i++)
    {
        if (bpt4[i]==PT_MOVETO)
            pDC->MoveTo(pt4[i]);
        else
            pDC->LineTo(pt4[i]);
    }
//  pDC->PolyDraw(pt4,bpt4,npt4);
    pDC->SelectObject(&curPen);
    for (i=0;i<npt1;i++)
    {
        if (bpt1[i]==PT_MOVETO)
            pDC->MoveTo(pt1[i]);
        else
            pDC->LineTo(pt1[i]);
    }
//  pDC->PolyDraw(pt1,bpt1,npt1);
    pDC->SelectObject(&disPen);
    for (i=0;i<npt2;i++)
    {
        if (bpt2[i]==PT_MOVETO)
            pDC->MoveTo(pt2[i]);
        else
            pDC->LineTo(pt2[i]);
    }
    //pDC->PolyDraw(pt2,bpt2,npt2);
    pDC->SelectObject(pOldP);


    delete []bpt1;
    delete []bpt2;
    delete []bpt3;
    delete []bpt4;
    delete []pt1;
    delete []pt2;
    delete []pt3;
    delete []pt4;


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
