// Point3D.cpp: implementation of the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Calques3D.h"
#include "..\Prefs\Prefs.h"

#include "Vector4.h"
#include "Point3D.h"
#include "Droite3D.h"
#include "Plan3D.h"
#include "Cercle3D.h"
#include "Sphere3D.h"

#include "..\fraction.h"
#include "..\OGLTools\OGLT.h"
#include "..\OGLTools\glut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPoint3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CPoint3D::CPoint3D()
{
    nDepth = 1; // un point libre est au sommet du graphe
    //if (pObjectShape) delete pObjectShape;
    pObjectShape = CShape(CShape::PointShape,TPref::TPoint.nPtFree);
    pObjectShape.clrObject = TPref::TPoint.clrDefault;
}

CPoint3D::~CPoint3D()
{

}


CPoint3D::CPoint3D(CPoint pt) :
    Visual_pt(pt),
    Concept_pt(pt.x,pt.y,0,1)
{
    nDepth = 1; // un point libre est au sommet du graphe
    //if (pObjectShape) delete pObjectShape;
    pObjectShape = CShape(CShape::PointShape,TPref::TPoint.nPtFree);
    pObjectShape.clrObject = TPref::TPoint.clrDefault;
}

CPoint3D::CPoint3D(CVector4 pt):
    Visual_pt((CPoint)pt),
    Concept_pt(pt)
{
    nDepth = 1; // un point libre est au sommet du graphe
    //if (pObjectShape) delete pObjectShape;
    pObjectShape = CShape(CShape::PointShape,TPref::TPoint.nPtFree);
    pObjectShape.clrObject = TPref::TPoint.clrDefault;
}

CPoint3D::CPoint3D(const CObject3D &obj) : CObject3D(obj)
{
    Concept_pt =  ((CPoint3D&)obj).Concept_pt;
    Visual_pt =   ((CPoint3D&)obj).Visual_pt;
    //if (pObjectShape) delete pObjectShape;
    pObjectShape = CShape(CShape::PointShape,TPref::TPoint.nPtFree);
    pObjectShape.clrObject = TPref::TPoint.clrDefault;
}

CObject3D* CPoint3D::CopyObject()
{
    CPoint3D *temp = new CPoint3D(*this);
    return temp;
}

void CPoint3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << Concept_pt;
        ar << Visual_pt;
        ar << bUp;
    }
    else
    {
        ar >> Concept_pt;
        ar >> Visual_pt;
        ar >> bUp;
    }
}

CxSchemeSet* CPoint3D::GetRedefineSchemes(CxSchemeSet* pSet)
{
    if (!pSet)
        pSet = new CxSchemeSet();

    /// Redefine the free-point as a Point on line
    CRedefineScheme *pObj = new CRedefineScheme();
    pObj->nObjResID = CTX_POINTSUR_DROITE;
    pObj->nAvail = ID_CONSTRUCTION_POINTON_LINE;
    pObj->nRedefStep = 1;
    pObj->nTaskResID[0] = CTX_SELECT_DROITE;
    pObj->nStepID[0] = TAllDroiteClass;
    pObj->pNewObj = new CPointSurD3D();
    pSet->Add(pObj);

    /// Redefine the free-point as a Point on plane
    pObj = new CRedefineScheme();
    pObj->nObjResID = CTX_POINTSUR_PLAN;
    pObj->nAvail = ID_CONSTRUCTION_POINTON_PLANE;
    pObj->nRedefStep = 1;
    pObj->nTaskResID[0] = CTX_SELECT_PLAN1;
    pObj->nStepID[0] = TAllPlanClass;
    pObj->pNewObj = new CPointSurP3D();
    pSet->Add(pObj);

    /// Redefine the free-point as a Point on circle
    pObj = new CRedefineScheme();
    pObj->nRedefStep = 1;
    pObj->nObjResID = CTX_POINTSUR_CERCLE;
    pObj->nAvail = ID_CONSTRUCTION_POINTON_CIRCLE;
    pObj->nTaskResID[0] = CTX_SELECT_CIRCLE;
    pObj->nStepID[0] = TAllCercleClass;
    pObj->pNewObj = new CPointSurC3D();
    pSet->Add(pObj);

    /// Redefine the free-point as a Point on sphere
    pObj = new CRedefineScheme();
    pObj->nRedefStep = 1;
    pObj->nObjResID = CTX_POINTSUR_SPHERE;
    pObj->nAvail = ID_CONSTRUCTION_POINTON_SPHERE;
    pObj->nTaskResID[0] = CTX_SELECT_SPHERE;
    pObj->nStepID[0] = TSphere3DClass;
    pObj->pNewObj = new CPointSurS3D();
    pSet->Add(pObj);

    return pSet;
}

void CPoint3D::CopyPointPosition(CVector4 pt)
{
    Concept_pt = pt;
}

void CPoint3D::CopyPointPosition(CObject3D* src)
{
    if (!src) return;
    if (src->isA() == TPoint3DClass)
    {
        CopyPointPosition(((CPoint3D*)src)->Concept_pt);
        //Concept_pt =  ((CPoint3D*)src)->Concept_pt;
    }
}

void CPoint3D::TranslateBy(CVector4 ptVec)
{
    Concept_pt = Concept_pt + ptVec;
}


BOOL CPoint3D::IsEqual(CObject3D &obj)
{
    if (obj.MaskObject(TAllPointClass))
     {
        CVector4 p1 = ((CPoint3D &)obj).Concept_pt - Concept_pt;
        return (p1.NullVector());
     }
    else
        return false;
}

UINT CPoint3D::IsAlignedWith(CObject3D *pObj1,CObject3D *pObj2)
{
    UINT res = VER_ERROR;

    if (!pObj1 || !pObj2) return res;
    if (!bValidate || !pObj1->bValidate || !pObj2->bValidate) return res;

    CPoint3D *pPt1 = DYNAMIC_DOWNCAST(CPoint3D,pObj1);
    CPoint3D *pPt2 = DYNAMIC_DOWNCAST(CPoint3D,pObj2);

    if (!pPt1 || !pPt2) return res;

    CDroite3D dr1(this,pPt1);
    CDroite3D dr2(this,pPt2);

    dr1.CalculConceptuel();
    dr2.CalculConceptuel();

    UINT res2= dr1.IsParallelTo(&dr2);
    if (res2==VER_PAR_ANALYTIC)
        res = VER_ALIGN_ANALYTIC;
    else
        res = VER_ALIGN_NO;

    return res;
}


BOOL CPoint3D::IsInActiveArea(CPoint thePt)
{
    int Dp = TPref::TUniv.nDefPres;
    CRect temp( Visual_pt.x-Dp, Visual_pt.y-Dp, Visual_pt.x+Dp, Visual_pt.y+Dp);
    return temp.PtInRect(thePt);
}

CRgn* CPoint3D::InvalideRect()
{
    int Dp = TPref::TUniv.nDefPres;

    //CRect temp( Visual_pt.x-Dp, Visual_pt.y-Dp, Visual_pt.x+Dp, Visual_pt.y+Dp);
    CRgn *mrgn = new CRgn();
    mrgn->CreateRectRgn(Visual_pt.x-Dp, Visual_pt.y-Dp, Visual_pt.x+Dp, Visual_pt.y+Dp);
    return mrgn;
}


UINT CPoint3D::CalculConceptuel()
{
    bValidate = TRUE;
    return 0;
}

void CPoint3D::CalculVisuel(CVisualParam *myVisuParam)
{
    CVector4 T(myVisuParam->ProjectPoint(Concept_pt));
    Visual_pt.x = (long)T.x; // + WndWidth/2;
    Visual_pt.y = (long)T.y; //WndHeight/2 - T.y;
    bUp = (Concept_pt.z > 0) ;
//  ShadowPoint = Concept_pt;
//  ShadowPoint.z =0.0;
//  ShadowPoint = myVisuParam->ProjectPoint(ShadowPoint);
    if (isA() == TPointSurP3DClass)
    {
        CPointSurP3D *pl = (CPointSurP3D*)this;
        if (pl)
            if (pl->P)
                pl->P->CalculVisuel(myVisuParam);
    }
}

void CPoint3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(mV->nCalqueNum))) return;

    COLORREF    ObjCl= pObjectShape.GetObjectColor(),
                ObjClH= pObjectShape.GetObjectHiddenColor();
    CPen disPen(PS_SOLID,1,ObjClH);
    CPen curPen(PS_SOLID,1,ObjCl);
    CBrush disBrush(ObjClH);
    CBrush curBrush(ObjCl);

    if (bMarked && mV->bDrawMark)
        DrawRetro(pDC,mV);
    BOOL testvi = mV->IsPointVisible(this);
    CPen *oldP = pDC->SelectObject(testvi ? &curPen : &disPen);
    CBrush *oldB = pDC->SelectObject(testvi ? &curBrush : &disBrush);


    int nSize = 2;


    CRect theR(Visual_pt.x-nSize, Visual_pt.y-nSize,Visual_pt.x+nSize, Visual_pt.y+nSize);
    pObjectShape.Draw(pDC,theR);
    pDC->SelectObject(oldB);
    pDC->SelectObject(oldP);
    if (bIsSelected)
    {
        DrawSelected(pDC,mV);
        if (!(bMarked && mV->bDrawMark)) DrawRetro(pDC,mV);
     }

    if (bShowTrace)
    {
        CPen trPen(PS_SOLID,1,TPref::TUniv.clrTrace);
        CBrush trBrush(TPref::TUniv.clrTrace);

        oldP = pDC->SelectObject(&trPen);
        oldB = pDC->SelectObject(&trBrush);
        int nbTP = cTracePosition.GetSize();
        for (int i=0;i<nbTP;i++)
        {
            CVector4 pt = cTracePosition.GetAt(i);
            CPoint ptt = (CPoint)mV->ProjectPoint(pt);

            CRect theR(ptt.x-nSize/2, ptt.y-nSize/2,ptt.x+nSize/2, ptt.y+nSize/2);
            pDC->Rectangle(theR);
        }
        pDC->SelectObject(oldB);
        pDC->SelectObject(oldP);
    }
}

void CPoint3D::DrawSelected(CDC* pDC,CVisualParam *vp)
{
    if ((!bVisible) || (!bValidate) || (!IsInCalque(vp->nCalqueNum))) return;

    int Ds = TPref::TPoint.nSize+2;
    CPoint  Pt = Visual_pt;
    CRect theR(Pt.x-Ds+1,Pt.y-Ds+1,Pt.x+Ds,Pt.y+Ds);
    pDC->InvertRect(theR);
}


void CPoint3D::DrawRetro(CDC *pDC,CVisualParam *mV)
{
    mV->DrawFeedBack(pDC,this);
}

void CPoint3D::GetRange(CVector4 &min,CVector4 &max)
{
    CObject3D::GetRange(min,max);
    if (bValidate && IsVisible())
    {
        min = Concept_pt;
        max = Concept_pt;
    }
}

COLORREF CPoint3D::GetDefaultColor()
{
    return TPref::TPoint.clrDefault;
}


BOOL CPoint3D::GetDistanceFrom(CObject3D *pObj,FCoord& dis)
{
    CDroite3D *pDr = DYNAMIC_DOWNCAST(CDroite3D,pObj);
    CPlan3D *pPl = DYNAMIC_DOWNCAST(CPlan3D,pObj);
    CSphere3D *pSp = DYNAMIC_DOWNCAST(CSphere3D,pObj);

    if (pDr)
    {
        CVector4 p0 = Concept_pt;
        CVector4 p1 = pDr->GetBasePoint();
        CVector4 pd = pDr->GetDirVector();

        pd = pd.Normalized();

        if (pd.NullVector()) return FALSE;

        FCoord denom = sqrt(pd.x*pd.x+pd.y*pd.y+pd.z*pd.z);
        FCoord num =    pow(pd.z*(p0.y-p1.y)-pd.y*(p0.z-p1.z),2) +
                        pow(pd.x*(p0.z-p1.z)-pd.z*(p0.x-p1.x),2) +
                        pow(pd.y*(p0.x-p1.x)-pd.x*(p0.y-p1.y),2);
        num = sqrt(num);
        dis = num / denom;
    }
    else if (pPl)
    {
        CVector4 p0 = Concept_pt;
        CVector4 pl = pPl->GetNormalForm();

        FCoord denom = sqrt(pl.x*pl.x+pl.y*pl.y+pl.z*pl.z);

        if (FCZero(denom)) return FALSE;

        FCoord num = pl.x*p0.x + pl.y*p0.y + pl.z*p0.z + pl.w;
        dis = fabsl(num / denom);
    }
    else if (pSp)
    {
        CVector4 p0 = Concept_pt;
        CVector4 p1 = pSp->P1->Concept_pt;
        CVector4 pd = p1-p0;

        dis = pd.Norme();
        dis = dis - pSp->Rayon;
    }
    else
        return FALSE;

    return TRUE;
}



CString CPoint3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (bValidate)
    {
        CString mstr2,strName;
        mstr2.LoadString(GetNameID());
        strName.Format("%s%d",mstr2,nObjectId);

        CFraction F1,F2,F3;

        __int64 x1,x2,x3;
        __int64 y1,y2,y3;
        __int64 z1,z2,z3;

        F1 = (Concept_pt.x/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
        F1.Parts(x1,x2,x3,100);
        if (x3!=0) x1 = x1*x3 + x2;
        F2 = (Concept_pt.y/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
        F2.Parts(y1,y2,y3,100);
        if (y3!=0) y1 = y1*y3 + y2;
        F3 = (Concept_pt.z/TPref::TUniv.nUnitRep)*TPref::TMathPad.UnitScale;
        F3.Parts(z1,z2,z3,100);
        if (z3!=0) z1 = z1*z3 + z2;

        CString strX;
        if (x2==0 && x3==0)  strX.Format(_T("%d"),(int)x1);
        else if (x3==0) strX.Format(_T("0"));
        else if (x3==1) strX.Format(_T("%d"),(int)x1);
        else strX.Format(_T("%d/%d"),(int)x1,(int)x3);
        CString strY;
        if (y2==0 && y3==0)  strY.Format(_T("%d"),(int)y1);
        else if (y3==0) strY.Format(_T("0"));
        else if (y3==1) strY.Format(_T("%d"),(int)y1);
        else strY.Format(_T("%d/%d"),(int)y1,(int)y3);
        CString strZ;
        if (z2==0 && z3==0)  strZ.Format(_T("%d"),(int)z1);
        else if (z3==0) strZ.Format(_T("0"));
        else if (z3==1) strZ.Format(_T("%d"),(int)z1);
        else strZ.Format(_T("%d/%d"),(int)z1,(int)z3);

        CString strCoord;
        strCoord.Format(_T("%s,%s,%s"),strX,strY,strZ);

        mstr.Format(_T("FreePointD[%s,%s];"),strName,strCoord);
    }
    return mstr;
}


void CPoint3D::Draw3DRendering()
{
    if ((!bVisible) || (!bValidate)) return;

	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_ambient_color[] = {255/255., 255/255.,100/255. , 1.0f};
    float mat_diffuse[] = {0.9f, 0.1f, 0.1f, 1.0f};
    float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	GLdouble x = (Concept_pt.x/TPref::TUniv.nUnitRep/3);
	GLdouble y = (Concept_pt.y/TPref::TUniv.nUnitRep/3);
	GLdouble z = (Concept_pt.z/TPref::TUniv.nUnitRep/3);
	GLUquadricObj*m_quadrObj=gluNewQuadric();
// 	gluQuadricNormals(m_quadrObj,GLU_SMOOTH);
// 	gluQuadricTexture(m_quadrObj,GL_TRUE);
// 	gluQuadricDrawStyle(m_quadrObj,GLU_FILL);
// 	gluQuadricOrientation(m_quadrObj,GLU_OUTSIDE);
// 
	glPushMatrix();
	glTranslated(x, y, z);
	glColor3f(.2f,.5f,.8f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	gluSphere(m_quadrObj,0.04,16,16);
	glPopMatrix();
	/*glBegin (GL_POINTS);
	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f (x,y,z);
	glEnd ();*/
}



BOOL CPoint3D::MoveObject(CVisualParam *myVisuParam,UINT modkey,CPoint MouseClic ,CVector4& TempCpt)
{
    BOOL ret=0;
    CPoint3D a(TempCpt);
    CPoint3D b(CVector4(TempCpt.x,TempCpt.y+100,TempCpt.z));
    CPoint3D c(CVector4(TempCpt.x,TempCpt.y,TempCpt.z+100));
    CPoint3D d(CVector4(TempCpt.x+100,TempCpt.y,TempCpt.z));
    switch (TPref::TUniv.nMoveType)
    {   case TPref::MV_HORIZ :
         {

            if (modkey & MK_SHIFT)
            //if (modkey & MK_RBUTTON)
             {
                CDroite3D dr(&a,&c);
                dr.CalculConceptuel();
                CPointSurD3D ptd(&dr);
                ret = ptd.MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
             }
            else
             {
                CPlan3D pl(&a,&b,&d);
                pl.CalculConceptuel();
                CPointSurP3D ptp(&pl);
                ret = ptp.MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
             }
            break;
         }
        case TPref::MV_LEFT :
         {
            CPlan3D pl(&a,&d,&c);
            pl.CalculConceptuel();
            CPointSurP3D ptp(&pl);

            CDroite3D dr(&a,&b);
            dr.CalculConceptuel();
            CPointSurD3D ptd(&dr);
            if (modkey & MK_SHIFT)
                ret = ptd.MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
            else
                ret = ptp.MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
            break;
          }
        case TPref::MV_RIGHT :
         {
            CPlan3D pl(&a,&b,&c);
            pl.CalculConceptuel();
            CPointSurP3D ptp(&pl);

            CDroite3D dr(&a,&d);
            dr.CalculConceptuel();
            CPointSurD3D ptd(&dr);
            if (modkey & MK_SHIFT)
                ret = ptd.MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
            else
                ret = ptp.MoveObject(myVisuParam,modkey,MouseClic,TempCpt);
            break;
          }
        default:
            break;
        }
    if (TPref::TUniv.bMagnet)
        myVisuParam->ApplyMagnet(TempCpt);
    return ret;
/*  MouseClic.x-= myVisuParam->RepCoord.x;
    MouseClic.y = myVisuParam->RepCoord.y- MouseClic.y ;
////////////// ZOOM
    MouseClic.x = (1/myVisuParam->Zoom)*MouseClic.x;
    MouseClic.y = (1/myVisuParam->Zoom)*MouseClic.y;
    long double theXe = (long double) MouseClic.x,
                    theYe = (long double) MouseClic.y,
                    theX =  (long double) TempCpt.x,
                    theY =  (long double) TempCpt.y,
                    theZ = (long double) TempCpt.z;
    long double CT=myVisuParam->CT,
                    CP=myVisuParam->CP,
                    ST=myVisuParam->ST,
                    SP=myVisuParam->SP;
    CT = zero2(CT);     CP = zero2(CP);
    SP = zero2(SP);     ST = zero2(ST);

    if (modkey & MK_SHIFT)
     { */
/******     long double theZ1 = (theX*SP + theYe*CT + SP*ST*theXe) / (CT*CP);
        long double theZ2 = (SP*theY + theYe*ST - theXe*CT*SP) /    (ST*CP);
        theZ = (abs(theZ1-theZ) < abs(theZ2-theZ)) ? theZ1 : theZ2;
        TempCpt.z = theZ;*/
/*      theY = (ST*CP*theZ + theXe*CT*SP - theYe*ST) / SP;
        TempCpt.y = theY;
     }
    else
     {*/
/*  ******  theX = (theZ*CT*CP - theYe*CT - SP*ST*theXe) / SP;
        theY = (ST*CP*theZ + theXe*CT*SP - theYe*ST) / SP;
        TempCpt.x = theX;
        TempCpt.y = theY;*/
/*      theX = (theZ*CT*CP - theYe*CT - SP*ST*theXe) / SP;
        long double theZ1 = (theX*SP + theYe*CT + SP*ST*theXe) / (CT*CP);
        long double theZ2 = (SP*theY + theYe*ST - theXe*CT*SP) /    (ST*CP);
        theZ = (abs(theZ1-theZ) < abs(theZ2-theZ)) ? theZ1 : theZ2;
        TempCpt.x = theX;
        TempCpt.z = theZ;
     }

    return 1;*/
}

BOOL CPoint3D::GraftOn(CObject3D * pNew)
{
//  if (isA() != TPoint3DClass) return FALSE;
    if (!pNew) return FALSE;

    BOOL bRes = TRUE;
    CxObject3DSet mList;
    GetDependList(&mList,FALSE);
    for (int i=0;i<mList.GetSize();i++)
    {
        CObject3D *pObj = mList.GetAt(i);
        if (!pObj) continue;
        if (pObj == this) continue;

        bRes &= pObj->ChangeParent(this,pNew,TRUE);
    }
    return bRes;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointCalc3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointCalc3D::CPointCalc3D() : CPoint3D()
{
    pSource = NULL;
}

CPointCalc3D::CPointCalc3D(CObject3D *s): CPoint3D(CVector4(0,0,0))
{
    pSource = s;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
    nDepth = s->nDepth +1;
}

CPointCalc3D::CPointCalc3D(const CObject3D &src)
{
    pSource =  ((CPointCalc3D&)src).pSource;
    nDepth = src.nDepth +1;
}

CObject3D* CPointCalc3D::CopyObject()
{
    CObject3D *temp = new CPointCalc3D((CObject3D&)*this);
    return temp;
}

int CPointCalc3D::SetDepth()
{
    if (pSource)
        nDepth = pSource->nDepth +1;
    return nDepth;
}


CxObject3DSet* CPointCalc3D::GetParents()
{
    //CxObject3DSet* list = new CxObject3DSet();
    //list->Add(pSource);
    //return list;
    return NULL;
}

void CPointCalc3D::Serialize( CArchive& ar )
{
    CPoint3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((pSource) ? pSource->nObjectId : -1);
    }
    else
    {
        pSource = SerializeObj(ar);
    }
}

void CPointCalc3D::HandleObjectError(int errNo,BOOL bShow)
{
    //bValidate = (errNo ==0);
    if (!bShow) return;
    if (bValidate) return;

    CString mstr;
    mstr.LoadString(ERR_INTCALC);
    AfxGetMainWnd()->MessageBox(mstr);
    //Geom3DApp->ShowErrorMsg(errNo);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointMilieu3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointMilieu3D::CPointMilieu3D() : CPoint3D()
{
    P1 = NULL;
    P2 = NULL;
    S = NULL;
}

CPointMilieu3D::CPointMilieu3D(CPoint3D *s1,CPoint3D *s2) :
    CPoint3D()
{
    P1 = s1;
    P2 = s2;
    S = NULL;
    nDepth = max(s1->nDepth,s2->nDepth)+1;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

CPointMilieu3D::CPointMilieu3D(CSegment3D *s1) : CPoint3D()
{
    P1 = P2 = NULL;
    S = s1;
    nDepth = s1->nDepth+1;
}

CPointMilieu3D::CPointMilieu3D(const CObject3D &src) : CPoint3D(src)
{
    P1 =  ((CPointMilieu3D&)src).P1;
    P2 =   ((CPointMilieu3D&)src).P2;
    S =   ((CPointMilieu3D&)src).S;
}

int CPointMilieu3D::SetDepth()
{
    if (P1 && P2)
        nDepth = max(P1->nDepth,P2->nDepth)+1;
    else if (S)
        nDepth = S->nDepth+1;
    return nDepth;
}


BOOL CPointMilieu3D::SetParents(CxObject3DSet* pSet)
{
    if (!pSet || P1 || P2 || S) return FALSE;
    int nb = pSet->GetSize();

    if (nb == 2 )
    {
        P1 = (CPoint3D*)pSet->GetAt(0);
        P2 = (CPoint3D*)pSet->GetAt(1);
    }
    else if (nb==1)
    {
        S = (CSegment3D*)pSet->GetAt(0);
    }
    else return FALSE;

    return TRUE;
}


BOOL CPointMilieu3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (P1 == pOld)
    {
        SetInGraph(FALSE);
        P1 = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (P2 == pOld)
    {
        SetInGraph(FALSE);
        P2 = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}

void CPointMilieu3D::Serialize( CArchive& ar )
{
    CPoint3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((P1) ? P1->nObjectId : -1);
        ar << ((P2) ? P2->nObjectId : -1);
        ar << ((S) ? S->nObjectId : -1);
    }
    else
    {
        P1 = (CPoint3D*)SerializeObj(ar);
        P2 = (CPoint3D*)SerializeObj(ar);
        S = (CSegment3D*)SerializeObj(ar);
    }
}


CObject3D* CPointMilieu3D::CopyObject()
{
    CObject3D *temp = new CPointMilieu3D((CObject3D&)*this);
    return temp;
}

CString CPointMilieu3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    UINT mID = GetDefID();
    if (P1 && P2) mID=IDS_DEF_POINTMID2;
    sFormat.LoadString(mID);

    CString sn1(_T("???")),sn2(_T("???")),sn3(_T("???"));
    if (P1) sn1 = P1->GetObjectHelp();
    if (P2) sn2 = P2->GetObjectHelp();
    if (S)  sn3 = S->GetObjectHelp();

    if (P1 && P2)
        mstr.Format(sFormat,sName,sn1,sn2);
    else
        mstr.Format(sFormat,sName,sn3);
    return mstr;
}

CxObject3DSet* CPointMilieu3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (!S)
     {
        list->Add(P1);
        list->Add(P2);
     }
    else
        list->Add(S);
    return list;
}

UINT  CPointMilieu3D::CalculConceptuel()
{
    CVector4 V1,V2;
    if (P1 == NULL)
     {
        V1 = S->P1->Concept_pt;
        V2 = S->P2->Concept_pt;
        if (!S->bValidate)
         {
            bValidate=FALSE;
            return ERR_NOMIDDLE1;
         }
     }
    else
     {
        V1 = P1->Concept_pt;
        V2 = P2->Concept_pt;
        if ((!P1->bValidate)  || (!P2->bValidate))
         {
            bValidate=FALSE;
            return ERR_NOMIDDLE2;
         }
     }
    Concept_pt.x= (V1.x + V2.x) / 2.00;
    Concept_pt.y= (V1.y + V2.y) / 2.00;
    Concept_pt.z= (V1.z + V2.z) / 2.00;
    Concept_pt.w=1;
    bValidate=TRUE;
    return 0;
}

CString CPointMilieu3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    if (bValidate && P1 && P2)
    {
        CString mstr2,strName,strObj1,strObj2;
        mstr2.LoadString(GetNameID());
        strName.Format("%s%d",mstr2,nObjectId);
        mstr2.LoadString(P1->GetNameID());
        strObj1.Format("%s%d",mstr2,P1->nObjectId);
        mstr2.LoadString(P2->GetNameID());
        strObj2.Format("%s%d",mstr2,P2->nObjectId);

        mstr.Format(_T("MidPoint[%s,%s,%s];"),strName,strObj1,strObj2);
    }
    else if (bValidate && S)
    {
        CString mstr2,strName,strObj1,strObj2;
        mstr2.LoadString(GetNameID());
        strName.Format("%s%d",mstr2,nObjectId);
        mstr2.LoadString(S->P1->GetNameID());
        strObj1.Format("%s%d",mstr2,S->P1->nObjectId);
        mstr2.LoadString(S->P2->GetNameID());
        strObj2.Format("%s%d",mstr2,S->P2->nObjectId);

        mstr.Format(_T("MidPoint[%s,%s,%s];"),strName,strObj1,strObj2);
    }
    return mstr;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointCenter3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointCenter3D::CPointCenter3D() : CPoint3D()
{
    C = NULL;
}

CPointCenter3D::CPointCenter3D(CCercle3D *s1) : CPoint3D()
{
    C = s1;
    nDepth = s1->nDepth+1;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

CPointCenter3D::CPointCenter3D(const CObject3D & src) : CPoint3D(src)
{
    C = ((CPointCenter3D&)src).C;
}

CObject3D* CPointCenter3D::CopyObject()
{
    CObject3D *temp = new CPointCenter3D((CObject3D&)*this);
    return temp;
}

int CPointCenter3D::SetDepth()
{
    if (C)
        nDepth = C->nDepth +1;
    return nDepth;
}


void CPointCenter3D::Serialize( CArchive& ar )
{
    CPoint3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((C) ? C->nObjectId : -1);
    }
    else
    {
        C=(CCercle3D*)SerializeObj(ar);
    }
}



CxObject3DSet* CPointCenter3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(C);
    return list;
}

BOOL CPointCenter3D::SetParents(CxObject3DSet* pSet)
{
    if (!pSet || C) return FALSE;
    int nb = pSet->GetSize();

    if (nb != 1 ) return FALSE;

    C = (CCercle3D*)pSet->GetAt(0);

    return TRUE;
}



UINT  CPointCenter3D::CalculConceptuel()
{
    bValidate = (C->bValidate);
    if (!bValidate)
        return ERR_SUPPORTOBJECT;

    Concept_pt = C->Center;
    bValidate=1;
    return 0;
}

CString CPointCenter3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sn1(_T("???"));
    if (C) sn1 = C->GetObjectHelp();
    mstr.Format(sFormat,sName,sn1);
    return mstr;
}


CString CPointCenter3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    CString mstr2,strName,strObj1;
    mstr2.LoadString(GetNameID());
    strName.Format("%s%d",mstr2,nObjectId);

    if (bValidate && C)
    {
        mstr2.LoadString(C->GetNameID());
        strObj1.Format("%s%d",mstr2,C->nObjectId);

        mstr.Format(_T("CircleCenter[%s,%s];"),strName,strObj1);

    }
    return mstr;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointSymetric3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointSymetric3D::CPointSymetric3D() : CPoint3D()
{
    Src = NULL;
    Pt = NULL;
    Dr = NULL;
    Pl = NULL;
};

CPointSymetric3D::CPointSymetric3D(CPoint3D *src,CPoint3D *d) : CPoint3D()
{
    Src = src;
    Pt = d;
    Dr = 0;
    Pl = 0;
    nDepth = max(d->nDepth,src->nDepth)+1;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

CPointSymetric3D::CPointSymetric3D(CPoint3D *src,CDroite3D *s1) : CPoint3D()
{
    Src = src;
    Pt = 0;
    Dr = s1;
    Pl = 0;
    nDepth = max(s1->nDepth,src->nDepth)+1;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

CPointSymetric3D::CPointSymetric3D(CPoint3D *src,CPlan3D *s1) : CPoint3D()
{
    Src = src;
    Pt = 0;
    Dr = 0;
    Pl = s1;
    nDepth = max(s1->nDepth,src->nDepth)+1;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

int CPointSymetric3D::SetDepth()
{
    if (Src && Pt)
        nDepth = max(Src->nDepth,Pt->nDepth)+1;
    else if (Src && Dr)
        nDepth = max(Src->nDepth,Dr->nDepth)+1;
    else if (Src && Pl)
        nDepth = max(Src->nDepth,Pl->nDepth)+1;

    return nDepth;
}



BOOL CPointSymetric3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (Src == pOld)
    {
        SetInGraph(FALSE);
        Src = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (Pt == pOld)
    {
        SetInGraph(FALSE);
        Pt = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}


CPointSymetric3D::CPointSymetric3D(const CObject3D &src ) : CPoint3D(src)
{
    Src=   ((CPointSymetric3D&)src).Src;
    Pt =   ((CPointSymetric3D&)src).Pt;
    Dr =   ((CPointSymetric3D&)src).Dr;
    Pl =   ((CPointSymetric3D&)src).Pl;
}

void CPointSymetric3D::Serialize( CArchive& ar )
{
    CPoint3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((Src) ? Src->nObjectId : -1);
        ar << ((Pt) ? Pt->nObjectId : -1);
        ar << ((Dr) ? Dr->nObjectId : -1);
        ar << ((Pl) ? Pl->nObjectId : -1);
    }
    else
    {
        Src = (CPoint3D*)SerializeObj(ar);
        Pt = (CPoint3D*)SerializeObj(ar);
        Dr = (CDroite3D*)SerializeObj(ar);
        Pl = (CPlan3D*)SerializeObj(ar);
    }
}

CString CPointSymetric3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sp(_T("???")),st(_T("???"));
    if (Src) sp = Src->GetObjectHelp();
    if (Pt) st = Pt->GetObjectHelp();
    else if (Dr) st = Dr->GetObjectHelp();
    else if (Pl) st = Pl->GetObjectHelp();
    mstr.Format(sFormat,sName,sp,st);
    return mstr;
}

CObject3D* CPointSymetric3D::CopyObject()
{
    CObject3D *temp = new CPointSymetric3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CPointSymetric3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(Src);
    if (Pt) list->Add(Pt);
    else if (Dr)    list->Add(Dr);
    else if (Pl)    list->Add(Pl);
    return list;
}


BOOL CPointSymetric3D::SetParents(CxObject3DSet* pSet)
{
    if (!pSet || Pt || Dr || Pl) return FALSE;

    int nb = pSet->GetSize();
    if (nb!=2) return FALSE;

    Src = (CPoint3D*)pSet->GetAt(0);

    CObject3D *pObj = pSet->GetAt(1);

    if (pObj->MaskObject(TAllPointClass))
        Src = (CPoint3D*)pObj;
    else if (pObj->MaskObject(TAllDroiteClass))
        Dr = (CDroite3D*)pObj;
    else if (pObj->MaskObject(TAllPlanClass))
        Pl = (CPlan3D*)pObj;
    else return FALSE;
    return TRUE;
}



UINT CPointSymetric3D::CalculConceptuel()
{
    bValidate = Src->bValidate;

    if (Pt) bValidate = bValidate && (Pt->bValidate);
    else if (Dr)    bValidate = bValidate && (Dr->bValidate);
    else if (Pl)    bValidate = bValidate && (Pl->bValidate);

    if (!bValidate)
        return ERR_SUPPORTOBJECT;

    if (Dr)
     {
        CDroitePerp3D dpr(Src,Dr);
        if (!dpr.CalculConceptuel())
        {
            CPointInterDD3D pin(Dr,&dpr);
            pin.CalculConceptuel();
            CVector4 dir = pin.Concept_pt - Src->Concept_pt;
            Concept_pt = pin.Concept_pt + dir;
        }
        else
        {
            Concept_pt = Src->Concept_pt;
        }
     }
    else    if (Pl)
     {
        CDroitePerp3D dpr(Src,Pl);
        dpr.CalculConceptuel();
        CPointInterDP3D pin(&dpr,Pl);
        pin.CalculConceptuel();
        CVector4 dir = pin.Concept_pt - Src->Concept_pt;
        Concept_pt = pin.Concept_pt + dir;
     }
    else if (Pt)
     {
        CVector4 dir = Pt->Concept_pt - Src->Concept_pt;
        Concept_pt = Pt->Concept_pt + dir;
     }
    bValidate=1;
    return 0;
}

CString CPointSymetric3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    CString mstr2,strName,strObj1,strObj2;
    mstr2.LoadString(GetNameID());
    strName.Format("%s%d",mstr2,nObjectId);

    if (bValidate && Src && Pt)
    {
        mstr2.LoadString(Src->GetNameID());
        strObj1.Format("%s%d",mstr2,Src->nObjectId);
        mstr2.LoadString(Pt->GetNameID());
        strObj2.Format("%s%d",mstr2,Pt->nObjectId);

        mstr.Format(_T("SymmetricalPoint[%s,%s,%s];"),strName,strObj1,strObj2);

    }
    else if (bValidate && Src && Dr)
    {
        mstr2.LoadString(Src->GetNameID());
        strObj1.Format("%s%d",mstr2,Src->nObjectId);
        mstr2.LoadString(Dr->GetNameID());
        strObj2.Format("%s%d",mstr2,Dr->nObjectId);

        mstr.Format(_T("SymmetricalLine[%s,%s,%s];"),strName,strObj1,strObj2);
    }
    else if (bValidate && Src && Pl)
    {
        mstr2.LoadString(Src->GetNameID());
        strObj1.Format("%s%d",mstr2,Src->nObjectId);
        mstr2.LoadString(Pl->GetNameID());
        strObj2.Format("%s%d",mstr2,Pl->nObjectId);

        mstr.Format(_T("SymmetricalPlane[%s,%s,%s];"),strName,strObj1,strObj2);
    }
    return mstr;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPointTranslat3D, CPoint3D, VERSIONABLE_SCHEMA | 1)

CPointTranslat3D::CPointTranslat3D() : CPoint3D()
{
    Src = NULL;
    Org = NULL;
    Dir = NULL;
};

CPointTranslat3D::CPointTranslat3D(CPoint3D *o,CPoint3D *d,CPoint3D *s): CPoint3D()
{
    Src = s;
    Org = o;
    Dir = d;
    nDepth = max(s->nDepth,max(o->nDepth,d->nDepth))+1;
    pObjectShape.nShapeId = TPref::TPoint.nPtDep;
}

CPointTranslat3D::CPointTranslat3D(const CObject3D &src) : CPoint3D(src)
{
    Src=   ((CPointTranslat3D&)src).Src;
    Org=   ((CPointTranslat3D&)src).Org;
    Dir=   ((CPointTranslat3D&)src).Dir;
}

int CPointTranslat3D::SetDepth()
{
    if (Src && Org && Dir)
        nDepth = max(Src->nDepth,max(Org->nDepth,Dir->nDepth))+1;

    return nDepth;
}


BOOL CPointTranslat3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (Src == pOld)
    {
        SetInGraph(FALSE);
        Src = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (Org == pOld)
    {
        SetInGraph(FALSE);
        Org = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (Dir == pOld)
    {
        SetInGraph(FALSE);
        Dir = (CPoint3D*)pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}

void CPointTranslat3D::Serialize( CArchive& ar )
{
    CPoint3D::Serialize(ar);

    //Concept_pt.Serialize(ar);
    if (ar.IsStoring())
    {
        ar << ((Src) ? Src->nObjectId : -1);
        ar << ((Org) ? Org->nObjectId : -1);
        ar << ((Dir) ? Dir->nObjectId : -1);
    }
    else
    {
        Src = (CPoint3D*)SerializeObj(ar);
        Org = (CPoint3D*)SerializeObj(ar);
        Dir = (CPoint3D*)SerializeObj(ar);
    }
}


CObject3D* CPointTranslat3D::CopyObject()
{
    CObject3D *temp = new CPointTranslat3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CPointTranslat3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(Src);
    list->Add(Org);
    list->Add(Dir);
    return list;
}

BOOL CPointTranslat3D::SetParents(CxObject3DSet* pSet)
{
    if (!pSet || Src || Org|| Dir) return FALSE;

    int nb = pSet->GetSize();
    if (nb!=3) return FALSE;

    Src = (CPoint3D*)pSet->GetAt(0);
    Org = (CPoint3D*)pSet->GetAt(1);
    Dir = (CPoint3D*)pSet->GetAt(2);

    return TRUE;
}


CString CPointTranslat3D::GetObjectDef()
{
    CString mstr(_T("???")),sFormat(_T("???")),sName(_T("???"));
    sName = GetObjectName();
    sFormat.LoadString(GetDefID());

    CString sp(_T("???")),st1(_T("???")),st2(_T("???"));
    if (Src) sp = Src->GetObjectHelp();
    if (Org) st1 = Org->GetObjectName();
    if (Dir) st2 = Dir->GetObjectName();
    mstr.Format(sFormat,sName,sp,st1,st2);
    return mstr;
}

UINT  CPointTranslat3D::CalculConceptuel()
{
    bValidate = Src->bValidate && Org->bValidate && Dir->bValidate;

    if (!bValidate)
        return ERR_SUPPORTOBJECT;

    CVector4 transvec = Dir->Concept_pt - Org->Concept_pt;
    Concept_pt = Src->Concept_pt + transvec;
    bValidate=1;
    return 0;
}

CString CPointTranslat3D::ExportSymbolic(int nFormat)
{
    CString mstr;
    mstr.Empty();

    CString mstr2,strName,strObj1,strObj2,strObj3;
    mstr2.LoadString(GetNameID());
    strName.Format("%s%d",mstr2,nObjectId);

    if (bValidate && Src && Org && Dir)
    {
        mstr2.LoadString(Src->GetNameID());
        strObj1.Format("%s%d",mstr2,Src->nObjectId);
        mstr2.LoadString(Org->GetNameID());
        strObj2.Format("%s%d",mstr2,Org->nObjectId);
        mstr2.LoadString(Dir->GetNameID());
        strObj3.Format("%s%d",mstr2,Dir->nObjectId);

        mstr.Format(_T("TranslatedPoint[%s,%s,%s,%s];"),strName,strObj1,strObj2,strObj3);

    }
    return mstr;
}
