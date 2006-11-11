// Text3D.cpp: implementation of the CText3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\calques3d.h"
#include "..\Calques3DDoc.h"
#include "..\Prefs\Prefs.h"

#include "..\MathParser\MathParser.h"

#include "..\fraction.h"

#include "Text3D.h"

#include "Point3D.h"
#include "Droite3D.h"
#include "Plan3D.h"
#include "Sphere3D.h"
#include "Cercle3D.h"
#include "CompositeObj3D.h"
#include "Cylinder3D.h"

#include "ObjectPropPage.h"
#include "..\HelpPrSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define COLOR_VAR RGB(128,16,0)

///extern CxObject3DSet*   GLOBALObjectSet;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CText3D, CObject3D, VERSIONABLE_SCHEMA | 1)

CText3D::CText3D() : CObject3D()
{
    bDrawInGraph = FALSE;
    rActZone.SetRectEmpty();
    mTextFont.CreatePointFont (90, _T ("Arial"));
    mColorText = RGB(0,0,0);
    mStrText =_T("");
    dwFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER ;
}

CText3D::~CText3D()
{
}

CText3D::CText3D(const CObject3D & src )
{
    bDrawInGraph = FALSE;
    rActZone = ((CText3D&)src).rActZone;
    mColorText = ((CText3D&)src).mColorText;
    mTextFont.CreatePointFont (90, _T ("Arial"));
    mStrText =_T("");
    dwFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER ;
}

CObject3D* CText3D::CopyObject()
{
    CObject3D *temp = new CText3D((CObject3D&)*this);
    return temp;
}

void CText3D::Serialize( CArchive& ar )
{
    CObject3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << rActZone;
        ar << mColorText;
    }
    else
    {
        ar >> rActZone;
        ar >> mColorText;
        bDrawInGraph = FALSE;
    }
}

BOOL CText3D::IsInActiveArea(CPoint thePt)
{
    CRect temp = rActZone;
    return temp.PtInRect(thePt);
    //return FALSE;
}

CRgn* CText3D::InvalideRect()
{
    CRect m = rActZone;
    CRgn *mrgn = new CRgn();
    mrgn->CreateRectRgn(m.left,m.top,m.right,m.bottom);
    return mrgn;
}

void CText3D::SetFont(LOGFONT *pLogFont)
{
    if (!pLogFont) return;

    mTextFont.CreateFontIndirect(pLogFont);
}

int  CText3D::CalculConceptuel()
{
    bValidate = TRUE;
    return 0;
}

void CText3D::CalculVisuel(CVisualParam *vp)
{
    bVisible = TRUE;
    //nCalque = pSource->nCalque;
}

CString CText3D::GetText()
{
    return mStrText;
}

int CText3D::SetProperties(CxObject3DSet *pSet)
{
    CString mstr = GetObjectName();

    //CHelpPrSheet pSheet("Object Properties");
    CHelpPrSheet pSheet(AFX_IDS_OBJPROP);
    CObjectTextPage pPage;
    CObjectInfoPage pPage2;
    CObjectDepPage pPage3;

    ////// Set Text Information Page
    pPage.m_bMod = isA();
    pPage.m_bMarked = bMarked;

    LOGFONT lFont;
    mTextFont.GetLogFont(&lFont);
    pPage.m_lFont = lFont;

    if (!strObjectName.IsEmpty())
        pPage.m_strVarName = strObjectName;

    ////// Set Object Information Page
    pPage2.m_strObjDef = GetObjectDef();
    pPage2.m_strObjHelp = GetObjectHelp();

    ////// Set Object Dependencies Page
    pPage3.m_pList = &cDependList;
    CxObject3DSet pMyList;
    CxObject3DSet pMyList2;
    GetDependList(&pMyList,FALSE);
    for (int jj=0;jj<pMyList.GetSize();jj++)
    {
        CObject3D *pObj = pMyList.GetAt(jj);
        if (!pObj) continue;
        if (pObj == this) continue;

        if (!pObj->pComposite)
            pMyList2.Add(pObj);
    }
    pPage3.m_pList = &pMyList2;

    pSheet.AddPage(&pPage);
    pSheet.AddPage(&pPage2);
    pSheet.AddPage(&pPage3);
    int nRet = pSheet.DoModal();
    if (nRet == IDOK)
    {
        bMarked = pPage.m_bMarked;

        lFont = pPage.m_lFont;
        mTextFont.DeleteObject();
        SetFont(&lFont);
        if (!pPage.m_strVarName.IsEmpty())
            strObjectName = pPage.m_strVarName;
        else
            strObjectName.Empty();
    }

    return (nRet == IDOK);

}

void CText3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
}

HTREEITEM CText3D::DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent)
{
    HTREEITEM pHItem = NULL;
    return NULL;
}

void CText3D::DrawDepGraph(CDC* pDC,CImageList *pImgList,int nTrace,BOOL bDrawNode,BOOL bDrawLink)
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLabel3D, CText3D, VERSIONABLE_SCHEMA | 1)

CLabel3D::CLabel3D() : CText3D()
{
    mDeltaPos = CSize(7,7);
    pSource = NULL;
}

CLabel3D::~CLabel3D()
{
}

CLabel3D::CLabel3D(CObject3D* pObj): CText3D()
{
    mDeltaPos = CSize(7,7);
    pSource = pObj;
    nDepth = pObj->nDepth+1;
}

CLabel3D::CLabel3D(const CObject3D & src ) : CText3D(src)
{
    pSource = ((CLabel3D&)src).pSource;
    mDeltaPos = ((CLabel3D&)src).mDeltaPos;
}

CObject3D* CLabel3D::CopyObject()
{
    CLabel3D *temp = new CLabel3D((CObject3D&)*this);
    return temp;
}

CxObject3DSet* CLabel3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(pSource);
    return list;
}

BOOL CLabel3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (pSource == pOld)
    {
        SetInGraph(FALSE);
        pSource = pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;

    return TRUE;
}


void CLabel3D::Serialize( CArchive& ar )
{
    CText3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((pSource) ? pSource->nObjectId : -1);
    }
    else
    {
        pSource = SerializeObj(ar);
    }
}

int  CLabel3D::CalculConceptuel()
{
    bValidate = (pSource && pSource->bValidate);
    return 0;
}

void CLabel3D::CalculVisuel(CVisualParam *vp)
{
    if (!pSource) return;
    if (rActZone.IsRectEmpty())
        mDeltaPos = CSize(7,7);

    if (pSource->MaskObject(TAllPointClass))
    //if ((pSource->isA() & TAllPointClass))
    {
        rActZone.left = ((CPoint3D*)pSource)->Visual_pt.x;
        rActZone.top = ((CPoint3D*)pSource)->Visual_pt.y;
    }
    else if (pSource->MaskObject(TSegment3DClass))
    //else if ((pSource->isA() & TSegment3DClass))
    {
        CVector4 ptSrc = ((CSegment3D*)pSource)->GetBasePoint();
        CVector4 ptDir = ((CSegment3D*)pSource)->GetDirVector();
        ptDir = ptDir * (0.5);
        ptSrc = ptSrc + ptDir;
        CPoint pt = (CPoint)vp->ProjectPoint(ptSrc);
        rActZone.left = pt.x;
        rActZone.top = pt.y;
    }
    rActZone += mDeltaPos;
    bVisible = pSource->bVisible;
    nCalque = pSource->nCalque;
}

CString CLabel3D::GetText()
{
    if (pSource)
        mStrText = pSource->GetObjectName();
    return mStrText;
}


void CLabel3D::Draw(CDC* pDC,CVisualParam *mV,BOOL bSm)
{
    if (!pSource) return;
    if (bSm) return;
    if ((!bVisible) || (!bValidate) || (mV && !IsInCalque(mV->nCalqueNum))) return;

    if (pSource->GetObjectName() == _T("")) return;

    CString mstr = GetText();

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    pDC->DrawText(mstr,rActZone,DT_CALCRECT);
    pDC->DrawText(mstr,rActZone,dwFormat);

    if (pSource->IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }
    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CEquation3D, CText3D, VERSIONABLE_SCHEMA | 1)

CEquation3D::CEquation3D() : CText3D()
{
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    pSource = NULL;
    m_bVal = FALSE;
    //m_cSources.RemoveAll();
}

CEquation3D::CEquation3D(CObject3D* pObj): CText3D()
{
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    pSource = pObj;
    nDepth = pObj->nDepth+1;
    m_bVal = FALSE;
    //m_cSources.Add(pObj);
}

CEquation3D::CEquation3D(const CObject3D & src): CText3D(src)
{
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    pSource = ((CEquation3D&)src).pSource;
    m_bVal = ((CEquation3D&)src).m_bVal;
    m_nValue = ((CEquation3D&)src).m_nValue;
    //m_cSources.RemoveAll();
    //m_cSources.Copy(((CEquation3D&)src).m_cSources);
}

CEquation3D::~CEquation3D()
{
}

int CEquation3D::SetProperties(CxObject3DSet *pSet)
{
    return CText3D::SetProperties(pSet);
    /*CString mstr = GetObjectName();

    //CHelpPrSheet pSheet("Object Properties");
    CHelpPrSheet pSheet(AFX_IDS_OBJPROP);
    CObjectTextPage pPage;
    CObjectInfoPage pPage2;
    CObjectDepPage pPage3;

    ////// Set Text Information Page



    ////// Set Object Information Page
    pPage2.m_strObjDef = GetObjectDef();
    pPage2.m_strObjHelp = GetObjectHelp();

    ////// Set Object Dependencies Page
    pPage3.m_pList = &cDependList;
    CxObject3DSet pMyList;
    CxObject3DSet pMyList2;
    GetDependList(&pMyList,FALSE);
    for (int jj=0;jj<pMyList.GetSize();jj++)
    {
        CObject3D *pObj = pMyList.GetAt(jj);
        if (!pObj) continue;
        if (pObj == this) continue;

        if (!pObj->pComposite)
            pMyList2.Add(pObj);
    }
    pPage3.m_pList = &pMyList2;

    pSheet.AddPage(&pPage);
    pSheet.AddPage(&pPage2);
    pSheet.AddPage(&pPage3);
    int nRet = pSheet.DoModal();
    if (nRet == IDOK)
    {
    }

    return (nRet == IDOK);*/
}



void CEquation3D::SetSelected(BOOL bSel /*=TRUE*/)
{
    CText3D::SetSelected(bSel);
    if (pSource)
        pSource->SetSelected(bSel);
}

int  CEquation3D::CalculConceptuel()
{
    int res = CText3D::CalculConceptuel();
    bValidate = (pSource && pSource->bValidate);
    return res;
}

FCoord CEquation3D::RoundNorm(FCoord nVal)
{
    /*if (FCZero(nVal))
        return 0.00;
    else
        return (nVal/TPref::UnitRep)*TPref::MathPad.UnitScale;*/
    return (FCZero(nVal)) ? 0.00 : (nVal/TPref::UnitRep)*TPref::MathPad.UnitScale;
}

FCoord CEquation3D::Round(FCoord nVal)
{
    /*of (FCZero(nVal))
        return 0.00;
    else
        return nVal;*/
    return (FCZero(nVal)) ? 0.00 : nVal;
}

CString CEquation3D::GetText()
{
    mStrText.Empty();
    if (!pSource) return mStrText;

    if (pSource->MaskObject(TAllPointClass))
        mStrText = GetPointText(pSource);
    else if (pSource->MaskObject(TAllDroiteClass))
        mStrText = GetLineText(pSource);
    else if (pSource->MaskObject(TAllPlanClass))
        mStrText = GetPlaneText(pSource);
    else if (pSource->MaskObject(TSphere3DClass))
        mStrText = GetSphereText(pSource);
    //mStrText = pSource->GetObjectName();
    return mStrText;
}

/*CString SetSign(FCoord mValue)
{
    CString mstr;

    CString strPrec,strObj2;
    strPrec.Format(_T("%%.%df"),TPref::MathPad.PrecReal);

    BOOL bPos = (mValue>=0);
    strObj2.Format(strPrec,(bPos) ? mValue : fabs(mValue));

    mstr.Format(_T("%s %s"),
        (bPos) ? _T("+") : _T("-"),
        strObj2);

    return mstr;
}*/

//////////////////////////////////////////////////////////////////////
/// Method CEquation3D::FormatNumber
///
/// \param mValue   The value
/// \param bValide  TRUE if the source is valide, FALSE otherwise
/// \param bSign    TRUE for displaying the sign, FALSE otherwise
/// \return         A CString containing the formatted value
//////////////////////////////////////////////////////////////////////
CString CEquation3D::FormatNumber(FCoord nValue, BOOL bValide,BOOL bSign)
{
    BOOL bShowFraction = TPref::MathPad.ShowFraction;
    int  nPrecFraction = TPref::MathPad.PrecFraction;

    CString strPrec,strObj;
    strObj = _T("####");

    if (!bValide) return strObj;

    strPrec.Format(_T("%%.%df"),TPref::MathPad.PrecReal);

    if (!bShowFraction)
    {
        if (bSign)
        {
            CString strObj2;
            BOOL bPos = (nValue>=0);
            strObj2.Format(strPrec,(bPos) ? nValue : fabs(nValue));

            strObj.Format(_T("%s %s"),
                    (bPos) ? _T("+") : _T("-"),
                    strObj2);
        }
        else
            strObj.Format(strPrec,nValue);
    }
    else
    {
        CFraction F1;
        if (!bSign)
        {
            F1= nValue;
            strObj= F1.ToString(nPrecFraction);
        }
        else
        {
            BOOL bPos = (nValue>=0);
            F1 = fabs(nValue);
            strObj.Format(_T("%s %s"),
                    (bPos) ? _T("+") : _T("-"),
                    F1.ToString(nPrecFraction));

        }
    }
    return strObj;
}

CString CEquation3D::GetPointText(CObject3D *pObj)
{
    CString strObj(_T(""));

    if (!pObj || !pObj->MaskObject(TAllPointClass))
        return strObj;

//  CFraction F1,F2,F3;
    CVector4 mypt = ((CPoint3D*)pObj)->Concept_pt;

//  BOOL bApp = TPref::MathPad.ShowFraction;
//  int  nApp = TPref::MathPad.PrecFraction;

    CString strMask = _T("X: %s\nY: %s\nZ: %s");


    CString strN1 = FormatNumber(RoundNorm(mypt.x),pObj->bValidate);
    CString strN2 = FormatNumber(RoundNorm(mypt.y),pObj->bValidate);
    CString strN3 = FormatNumber(RoundNorm(mypt.z),pObj->bValidate);

    m_nValue = CVector4(RoundNorm(mypt.x),RoundNorm(mypt.y),RoundNorm(mypt.z));
    m_bVal = TRUE;
    strObj.Format(strMask,strN1,strN2,strN3);

/*  F1= myRound(mypt.x);
    F2= myRound(mypt.y);
    F3= myRound(mypt.z);
    if (!pObj->bValidate)
    {
        strObj.Format(_T("X: ####\nY: ####\nZ: ####"));
        return strObj;
    }

    if (!bApp)
    {
        CString strPrec,strObj2;
        strPrec.Format(_T("%%.%df"),TPref::MathPad.PrecReal);
        strObj2.Format(_T("X: %s \nY: %s \nZ: %s "),
                strPrec,strPrec,strPrec);
        strObj.Format(strObj2,
                myRound(mypt.x),
                myRound(mypt.y),
                myRound(mypt.z));
    }
    else
        strObj.Format(_T("X: %s \nY: %s \nZ: %s "),
                F1.ToString(nApp),
                F2.ToString(nApp),
                F3.ToString(nApp));*/

    return strObj;
}

CString CEquation3D::GetLineText(CObject3D *pObj)
{
    CString strObj(_T(""));

    if (!pObj || !pObj->MaskObject(TAllDroiteClass))
        return strObj;

    CVector4 ptSrc = ((CSegment3D*)pObj)->GetBasePoint();
    CVector4 ptDir = ((CSegment3D*)pObj)->GetDirVector();
    FCoord nn = ptDir.Norme();
    ptDir = ptDir.Normalized();


    CString strMask = _T("  |X: %s\n"
                         "P |Y: %s\n"
                         "  |Z: %s\n\n"
                         "  |X: %s\n"
                         "V |Y: %s\n"
                         "  |Z: %s");


    CString strN1 = FormatNumber(RoundNorm(ptSrc.x),pObj->bValidate);
    CString strN2 = FormatNumber(RoundNorm(ptSrc.y),pObj->bValidate);
    CString strN3 = FormatNumber(RoundNorm(ptSrc.z),pObj->bValidate);
    CString strN4 = FormatNumber(Round(ptDir.x),pObj->bValidate);
    CString strN5 = FormatNumber(Round(ptDir.y),pObj->bValidate);
    CString strN6 = FormatNumber(Round(ptDir.z),pObj->bValidate);

    strObj.Format(strMask,strN1,strN2,strN3,strN4,strN5,strN6);
    m_bVal = FALSE;

/*  if (pObj->bValidate)
    {
        CString strPrec,strObj2;
        strPrec.Format(_T("%%.%df"),TPref::MathPad.PrecReal);

        strObj2.Format(_T("  |X: %s\n"
                         "P |Y: %s\n"
                         "  |Z: %s\n\n"
                         "  |X: %s\n"
                         "V |Y: %s\n"
                         "  |Z: %s"),
                strPrec,strPrec,strPrec,strPrec,strPrec,strPrec);

        strObj.Format(strObj2,
                myRound(ptSrc.x),
                myRound(ptSrc.y),
                myRound(ptSrc.z),
                myRound2(ptDir.z),
                myRound2(ptDir.y),
                myRound2(ptDir.x));
    }
    else
        strObj.Format(_T("X: ##### \t ##### \n"
                       "Y: ##### \t ##### \n"
                       "Z: ##### \t #####"));*/
    return strObj;
}

CString CEquation3D::GetPlaneText(CObject3D *pObj)
{
    CString strObj(_T(""));

    if (!pObj || !pObj->MaskObject(TAllPlanClass))
        return strObj;

    CVector4 nForm = ((CPlan3D*)pObj)->GetNormalForm();
/*  if (!pObj->bValidate)
    {
        strObj.Format(_T("####"));
        return strObj;
    }*/

    CString strMask = _T("%s * X %s * Y %s * Z = %s");


    CString strN1 = FormatNumber(Round(nForm.x),pObj->bValidate);
    CString strN2 = FormatNumber(Round(nForm.y),pObj->bValidate,TRUE);
    CString strN3 = FormatNumber(Round(nForm.z),pObj->bValidate,TRUE);
    CString strN4 = FormatNumber(RoundNorm(-nForm.w),pObj->bValidate);

    m_nValue = CVector4(Round(nForm.x),Round(nForm.y),Round(nForm.z),RoundNorm(-nForm.w));
    m_bVal = TRUE;

    strObj.Format(strMask,strN1,strN2,strN3,strN4);



/*  if (!TPref::MathPad.ShowFraction)
        strObj.Format(_T("%.4f * X %s * Y  %s * Z %s = 0"),
                    myRound(nForm.x),
                    SetSign(myRound(nForm.y)),
                    SetSign(myRound(nForm.z)),
                    SetSign(myRound(nForm.w)));
    else
    {
        int nApp = TPref::MathPad.PrecFraction;
        CFraction F1,F2,F3,F4;
        F1= myRound(nForm.x);
        F2= myRound(nForm.y);
        F3= myRound(nForm.z);
        F4= myRound(nForm.w);
        strObj.Format(_T("%s * X + %s * Y + %s * Z + %s = 0"),
                F1.ToString(nApp),
                F2.ToString(nApp),
                F3.ToString(nApp),
                F4.ToString(nApp));
    }*/

    return strObj;
}

CString CEquation3D::GetSphereText(CObject3D *pObj)
{
    CString strObj(_T(""));

    if (!pObj || !pObj->MaskObject(TSphere3DClass))
        return strObj;

    CVector4 ptSrc = ((CSphere3D*)pObj)->P1->Concept_pt;
    FCoord   nRay  = ((CSphere3D*)pObj)->Rayon;
    nRay /= TPref::UnitRep;

    CString strMask = _T("(X %s) � + (Y %s) � + (Z %s) � = (%s) �");


    CString strN1 = FormatNumber(RoundNorm(ptSrc.x),pObj->bValidate,TRUE);
    CString strN2 = FormatNumber(RoundNorm(ptSrc.y),pObj->bValidate,TRUE);
    CString strN3 = FormatNumber(RoundNorm(ptSrc.z),pObj->bValidate,TRUE);
    CString strN4 = FormatNumber(nRay,pObj->bValidate);

    m_nValue = CVector4(RoundNorm(ptSrc.x),RoundNorm(ptSrc.y),RoundNorm(ptSrc.z),nRay);
    m_bVal = TRUE;

    strObj.Format(strMask,strN1,strN2,strN3,strN4);

/*

    if (pObj->bValidate)
    {
        nRay /= TPref::UnitRep;
        FCoord
            d = - myRound(ptSrc.x),
            e = - myRound(ptSrc.y),
            f = - myRound(ptSrc.z),
            g = nRay*nRay - (d*d + e*e + f*f);
            strObj.Format(_T("(X %s) � + (Y %s) � + (Z %s) � = (%.4f) � \n\n"
                           "X� + Y� + Z� %s * X %s * Y %s * Z %s = 0"),
                SetSign(myRound(ptSrc.x)),
                SetSign(myRound(ptSrc.y)),
                SetSign(myRound(ptSrc.z)),
                nRay,
                SetSign(2*d),SetSign(2*e),SetSign(2*f),SetSign(g));
    }
    else
        strObj.Format(_T("X: ##### \t ##### \n"
                       "Y: ##### \t ##### \n"
                       "Z: ##### \t #####"));*/
    return strObj;
}

void CEquation3D::CalculVisuel(CVisualParam *vp)
{
    //CText3D::CalculVisuel(vp);
    //bValidate = (pSource && pSource->bValidate);

    if (rActZone.IsRectEmpty())
    {
        //mDeltaPos = CSize(7,7);
    }

    //nCalque = 1;
}

HTREEITEM CEquation3D::DrawHistorique(CTreeCtrl& mListCtrl,HTREEITEM pParent)
{
    return CText3D::DrawHistorique(mListCtrl,pParent);
}

void CEquation3D::DrawMathPad(CDC* pDC)
{
    if (!pSource) return;
    if (!bVisible) return;

    //if (pSource->GetObjectName() == _T("")) return;
    dwFormat = DT_LEFT | DT_EXPANDTABS;

    CString mstr = GetText();
    CString nstr = pSource->GetObjectName();

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rEqu,rName;
    rEqu.SetRectEmpty();
    rName.SetRectEmpty();
    pDC->DrawText(mstr,rEqu,DT_CALCRECT);
    pDC->DrawText(nstr,rName,DT_CALCRECT);

    int nY = (rEqu.Height() - rName.Height())/2;
    int nX = rName.Width()+10;

    rEqu.OffsetRect(nX,0);
    rName.OffsetRect(0,nY);

    rEqu.OffsetRect(rActZone.TopLeft());
    rName.OffsetRect(rActZone.TopLeft());

    rActZone = rEqu | rName;

    COLORREF oldC = pDC->SetTextColor(COLOR_VAR);
    pDC->DrawText(nstr,rName,dwFormat);
    pDC->SetTextColor(oldC);
    pDC->DrawText(mstr,rEqu,dwFormat);

    CPen pLine(PS_SOLID,1,COLOR_VAR);
    CPen *oldP = pDC->SelectObject(&pLine);
    CPoint mpt = rEqu.TopLeft();
    mpt.Offset(-6,2);
    pDC->MoveTo(mpt);
    mpt.Offset(0,rEqu.Height()-4);
    pDC->LineTo(mpt);
    pDC->SelectObject(oldP);

    if (pSource->IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }
    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}


void CEquation3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSm)
{
}

CxObject3DSet* CEquation3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    list->Add(pSource);
    return list;
}

BOOL CEquation3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (pSource == pOld)
    {
        SetInGraph(FALSE);
        pSource = pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;

    return TRUE;
}



void CEquation3D::Serialize( CArchive& ar )
{
    CText3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((pSource) ? pSource->nObjectId : -1);
    }
    else
    {
        pSource = SerializeObj(ar);
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CValue3D, CEquation3D, VERSIONABLE_SCHEMA | 1)

CValue3D::CValue3D(): CEquation3D()
{
    m_nValue = 0.0;
}

CValue3D::CValue3D(const CObject3D &src): CEquation3D(src)
{
    m_nValue = ((CValue3D&)src).m_nValue;
}

void CValue3D::Serialize( CArchive& ar )
{
    CEquation3D::Serialize(ar);

    if (ar.IsStoring())
    {
    }
    else
    {
    }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDistance3D, CValue3D, VERSIONABLE_SCHEMA | 1)

CDistance3D::CDistance3D(): CValue3D()
{
    pSource = pSource2 = NULL;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
}

CDistance3D::CDistance3D(CObject3D* P1,CObject3D* P2) : CValue3D()
{
    pSource = P1;
    pSource2 = P2;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = max(P1->nDepth,P2->nDepth)+1;
}

CDistance3D::CDistance3D(CObject3D* P1): CValue3D()
{
    pSource = P1;
    pSource2 = NULL;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = P1->nDepth+1;
}

CDistance3D::CDistance3D(const CObject3D &src): CValue3D(src)
{
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    pSource2 = ((CDistance3D&)src).pSource2;
}

CDistance3D::~CDistance3D()
{
}

CString CDistance3D::GetText()
{
    if (!pSource && !pSource2) return mStrText;

    mStrText = _T("###");
    //if (!bValidate) return mStrText;


    if (pSource && !pSource2)
    {
        FCoord ndist = 0.;
        if (pSource->MaskObject(TAllCercleClass))
        {
            FCoord rad = ((CCercle3D*)pSource)->Radius;
            FCoord ang = ((CCercle3D*)pSource)->nArcAngle;
            ndist = ang*rad;
        }
        else if (pSource->MaskObject(TSegment3DClass))
        {
            ndist = (FCoord)((CSegment3D*)pSource)->GetDirVector();
        }

        m_nValue = RoundNorm(ndist);//ndist;

        CString strMask = _T("%s");
        CString strN1 = FormatNumber(RoundNorm(ndist),pSource->bValidate);
        mStrText.Format(strMask,strN1);

/*      CFraction F1 = RoundNorm(ndist);

        BOOL bApp = TPref::MathPad.ShowFraction;
        int  nApp = TPref::MathPad.PrecFraction;
        if (!bApp)
            mStrText.Format(_T("%.4f"),RoundNorm(ndist));
        else
            mStrText.Format(_T("%s"),F1.ToString(nApp));*/
    }

    else if (pSource->MaskObject(TAllPointClass) &&
        pSource2->MaskObject(TAllPointClass))
    {
        CVector4 mpt1 = ((CPoint3D*)pSource)->Concept_pt;
        CVector4 mpt2 = ((CPoint3D*)pSource2)->Concept_pt;

        FCoord ndist = (FCoord)(mpt2-mpt1);
        m_nValue = RoundNorm(ndist);//ndist;

        CString strMask = _T("%s");
        CString strN1 = FormatNumber(RoundNorm(ndist),pSource->bValidate && pSource2->bValidate);
        mStrText.Format(strMask,strN1);

/*      CFraction F1 = RoundNorm(ndist);

        BOOL bApp = TPref::MathPad.ShowFraction;
        int  nApp = TPref::MathPad.PrecFraction;
        if (!bApp)
            mStrText.Format(_T("%.4f"),RoundNorm(ndist));
        else
            mStrText.Format(_T("%s"),F1.ToString(nApp));*/

    }

    if (TPref::MathPad.UnitLinear)
    {
        CString mstr = mStrText;
        mStrText.Format(_T("%s %s"),mstr,TPref::strUnits.GetAt(TPref::MathPad.UnitLinear));
    }

    return mStrText;
}

CxObject3DSet* CDistance3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    if (pSource) list->Add(pSource);
    if (pSource2) list->Add(pSource2);
    return list;
}

BOOL CDistance3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    if (pSource == pOld)
    {
        SetInGraph(FALSE);
        pSource = pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else if (pSource2 == pOld)
    {
        SetInGraph(FALSE);
        pSource2 = pNew;
        if (bUpGraph) SetInGraph(TRUE);
    }
    else return FALSE;
    return TRUE;
}

void CDistance3D::SetSelected(BOOL bSel /*=TRUE*/)
{
    CText3D::SetSelected(bSel);
    //CEquation3D::SetSelected(bSel);
    //if (pSource2)
    //  pSource2->SetSelected(bSel);
}




void CDistance3D::Serialize( CArchive& ar )
{
    CValue3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << ((pSource2) ? pSource2->nObjectId : -1);
    }
    else
    {
        pSource2 = SerializeObj(ar);
    }
}

void CDistance3D::CalculVisuel(CVisualParam *vp)
{
    //CValue3D::CalculVisuel(vp);
}

int  CDistance3D::CalculConceptuel()
{
    int res = CText3D::CalculConceptuel();
    if (pSource2 && pSource)
        bValidate = (pSource2->bValidate && pSource->bValidate);
    else if (pSource)
        bValidate = (pSource->bValidate);
    else
        bValidate = FALSE;
    return res;
}

void CDistance3D::DrawMathPad(CDC* pDC)
{
    if (!pSource2 && !pSource) return;
    if (!bVisible) return;

    //if (pSource->GetObjectName() == _T("")) return;
    dwFormat = DT_LEFT | DT_EXPANDTABS;

    CString mstr = GetText();
    CString nstr;
    if (pSource2 && pSource)
    {
        CString nstr1 = pSource->GetObjectName();
        CString nstr2 = pSource2->GetObjectName();

        nstr.Format(_T("d(%s,%s) = "),nstr1,nstr2);
    }
    else if (pSource)
    {
        CString nstr1 = pSource->GetObjectName();

        nstr.Format(_T("l(%s) = "),nstr1);
    }

    CString strVar;
    strVar.Empty();
    if (!strObjectName.IsEmpty())
    {
        strVar = strObjectName + _T(" = ");
        nstr = strVar + nstr;
    }

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rEqu,rName;
    rEqu.SetRectEmpty();
    rName.SetRectEmpty();
    pDC->DrawText(mstr,rEqu,DT_CALCRECT);
    pDC->DrawText(nstr,rName,DT_CALCRECT);

    int nY = (rEqu.Height() - rName.Height())/2;
    int nX = rName.Width();

    rEqu.OffsetRect(nX,0);
    rName.OffsetRect(0,nY);

    rEqu.OffsetRect(rActZone.TopLeft());
    rName.OffsetRect(rActZone.TopLeft());

    rActZone = rEqu | rName;

    COLORREF oldC = pDC->SetTextColor(COLOR_VAR);
    pDC->DrawText(nstr,rName,dwFormat);
    pDC->SetTextColor(oldC);
    pDC->DrawText(mstr,rEqu,dwFormat);

//  if ((pSource && pSource->IsSelected()) ||
//      (pSource2 && pSource2->IsSelected()))
    if (IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }
    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}



void CDistance3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSm)
{
    if (!bValidate) return;
    if (IsSelected())
    {
        if (pSource && !pSource2 && pSource->MaskObject(TAllCercleClass))
        {
            CObject3D *pObj = pSource->CopyObject();
            pObj->CalculConceptuel();
            pObj->CalculVisuel(vp);

            pObj->pObjectShape.nShapeId = 5;
            pObj->pObjectShape.clrObject = COLOR_VAR;

            //pObj->nCalque = pObj1->nCalque & pObj2->nCalque;
            pObj->Draw(pDC,vp,TRUE);

            delete pObj;
        }
        else if (pSource && !pSource2 && pSource->MaskObject(TSegment3DClass))
        {
            CObject3D *pObj = pSource->CopyObject();
            pObj->CalculConceptuel();
            pObj->CalculVisuel(vp);

            pObj->pObjectShape.nShapeId = 5;
            pObj->pObjectShape.clrObject = COLOR_VAR;

            pObj->Draw(pDC,vp,TRUE);

            delete pObj;
        }
        else if (pSource && pSource2 && pSource->MaskObject(TAllPointClass) &&
                pSource2->MaskObject(TAllPointClass))
        {
            //CPoint pt = ((CPoint3D*)pSource)->Visual_pt;
            //CPoint pt2 = ((CPoint3D*)pSource2)->Visual_pt;

            CPoint3D pt1(((CPoint3D*)pSource)->Concept_pt);
            CPoint3D pt2(((CPoint3D*)pSource2)->Concept_pt);

            CSegment3D pseg(&pt1,&pt2);
            pseg.CalculConceptuel();
            pseg.CalculVisuel(vp);
            pseg.nCalque = pSource->nCalque & pSource2->nCalque;
            pseg.pObjectShape.nShapeId = 5;
            pseg.pObjectShape.clrObject = COLOR_VAR;
            pseg.Draw(pDC,vp,TRUE);

            //CPen mPen(PS_SOLID,2,RGB(0,0,0));
            //CPen* oldP = pDC->SelectObject(&mPen);
            //pDC->MoveTo(pt);
            //pDC->LineTo(pt2);
            //pDC->SelectObject(oldP);
        }
    }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CAngle3D, CValue3D, VERSIONABLE_SCHEMA | 1)

CAngle3D::CAngle3D(): CValue3D()
{
    m_pSource.RemoveAll();
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
}

CAngle3D::CAngle3D(CPoint3D* P1,CPoint3D* P2,CPoint3D* P3) : CValue3D()
{
    m_pSource.RemoveAll();
    m_pSource.Add(P1);
    m_pSource.Add(P2);
    m_pSource.Add(P3);
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = max(P1->nDepth,max(P2->nDepth,P3->nDepth))+1;
}

CAngle3D::CAngle3D(CPlan3D* Pl1,CPlan3D* Pl2): CValue3D()
{
    m_pSource.RemoveAll();
    m_pSource.Add(Pl1);
    m_pSource.Add(Pl2);
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = max(Pl1->nDepth,Pl2->nDepth)+1;
}

CAngle3D::CAngle3D(CPlan3D* Pl,CDroite3D* Dr): CValue3D()
{
    m_pSource.RemoveAll();
    m_pSource.Add(Pl);
    m_pSource.Add(Dr);
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = max(Pl->nDepth,Dr->nDepth)+1;
}

CAngle3D::CAngle3D(CDroite3D* Dr1,CDroite3D* Dr2): CValue3D()
{
    m_pSource.RemoveAll();
    m_pSource.Add(Dr1);
    m_pSource.Add(Dr2);
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = max(Dr1->nDepth,Dr2->nDepth)+1;
}

CAngle3D::CAngle3D(const CObject3D &src): CValue3D()
{
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    m_pSource.RemoveAll();
    m_pSource.Copy(((CAngle3D&)src).m_pSource);
    m_fAngle = ((CAngle3D&)src).m_fAngle;
}

CAngle3D::~CAngle3D()
{
    m_pSource.RemoveAll();
}

CString CAngle3D::GetText()
{
    FCoord ac= 0.0;
    BOOL bVal = FALSE;
    mStrText.Empty();
    if (m_pSource.GetSize()==3) // 3 points
    {
        CObject3D *pObj1 = m_pSource.GetAt(0);
        CObject3D *pObj2 = m_pSource.GetAt(1);
        CObject3D *pObj3 = m_pSource.GetAt(2);

        bVal = pObj1->bValidate && pObj2->bValidate && pObj3->bValidate;

        CVector4 P1 = ((CPoint3D*)pObj1)->Concept_pt;
        CVector4 P2 = ((CPoint3D*)pObj2)->Concept_pt;
        CVector4 P3 = ((CPoint3D*)pObj3)->Concept_pt;

        CVector4 N1 = P1-P2;
        CVector4 N2 = P3-P2;

        N1 = N1.Normalized();
        N2 = N2.Normalized();

        ac = N1 * N2;


        //return mStrText;
    }
    else if (m_pSource.GetSize()==2) // 2 plans
    {
        CObject3D *pObj1 = m_pSource.GetAt(0);
        CObject3D *pObj2 = m_pSource.GetAt(1);

        if (!pObj1 || !pObj2) return mStrText;

        mStrText = _T("###");
        //if (!pObj1->bValidate || !pObj1->bValidate)
        //  return mStrText;

        bVal = pObj1->bValidate && pObj2->bValidate;


        CVector4 N1 = ((CPlan3D*)pObj1)->VecNorm;
        CVector4 N2 = ((CPlan3D*)pObj2)->VecNorm;

        //N2 = N2 * (-1.0);

        N1 = N1.Normalized();
        N2 = N2.Normalized();

        ac = N1 * N2;
    }
    else
        return mStrText;


    if (ac>=1.0) ac=1.0;
    if (ac<=-1.0) ac=-1.;

    BOOL bApp = TPref::MathPad.ShowFraction;
    int  nApp = TPref::MathPad.PrecFraction;

    m_fAngle = acos(ac);
    m_nValue = m_fAngle;

    CString strMask = _T("%s %s");
    CString strUnit = (TPref::MathPad.UnitAngular) ? _T("rad") : _T("deg");
    CString strN1 = FormatNumber(
        (TPref::MathPad.UnitAngular) ? m_fAngle : m_fAngle*180/M_PI,
        bVal);
    mStrText.Format(strMask,strN1,strUnit);


/*
    if (!bApp)
    {
        if (TPref::MathPad.UnitAngular)
            mStrText.Format(_T("%.3f rad"),m_fAngle);
        else
            mStrText.Format(_T("%.3f deg"),m_fAngle*180/M_PI);
    }
    else
    {
        if (TPref::MathPad.UnitAngular)
        {
            CFraction F1 = m_fAngle / M_PI;
            mStrText.Format(_T("%s Pi rad"),F1.ToString(nApp));
        }
        else
        {
            CFraction F1 = m_fAngle*180/M_PI;
            mStrText.Format(_T("%s deg"),F1.ToString(12));
        }
    }*/

    return mStrText;
}

CxObject3DSet* CAngle3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    for (int i=0;i<m_pSource.GetSize();i++)
    {
        list->Add(m_pSource.GetAt(i));
    }
    return list;
}

BOOL CAngle3D::ChangeParent(CObject3D *pOld,CObject3D *pNew,BOOL bUpGraph)
{
    BOOL bDone = FALSE;
    for (int i=0;i<m_pSource.GetSize();i++)
    {
        CObject3D *pObj = m_pSource.GetAt(i);
        if (pObj == pOld)
        {
            SetInGraph(FALSE);
            m_pSource.SetAt(i,pNew);
            if (bUpGraph) SetInGraph(TRUE);
            bDone = TRUE;
            break;
        }
    }
    return bDone;
}

void CAngle3D::SetSelected(BOOL bSel /*=TRUE*/)
{
    CEquation3D::SetSelected(bSel);
//  for (int i=0;i<m_pSource.GetSize();i++)
//  {
//      CObject3D *pObj = m_pSource.GetAt(i);
//      if (pObj)
//          pObj->SetSelected(bSel);
//  }
}




void CAngle3D::Serialize( CArchive& ar )
{
    CValue3D::Serialize(ar);

    if (ar.IsStoring())
    {
        ar << m_pSource.GetSize();
        for (int i=0;i<m_pSource.GetSize();i++)
        {
            CObject3D *pObj = m_pSource.GetAt(i);
            ar << ((pObj) ? pObj->nObjectId : -1);
        }
    }
    else
    {
        int nb;
        ar >> nb;
        for (int i=0;i<nb;i++)
        {
            CObject3D *pObj = SerializeObj(ar);
            m_pSource.Add(pObj);
        }
    }
}

void CAngle3D::CalculVisuel(CVisualParam *vp)
{
    //CValue3D::CalculVisuel(vp);
    //bValidate = TRUE;
}

int  CAngle3D::CalculConceptuel()
{
    int res = CText3D::CalculConceptuel();

    BOOL bVal = TRUE;
    for (int i=0;i<m_pSource.GetSize();i++)
    {
        CObject3D *pObj = m_pSource.GetAt(i);
        if (pObj)
            bVal = bVal && (pObj->bValidate);
    }

    bValidate = bVal;
    return res;
}

void CAngle3D::DrawMathPad(CDC* pDC)
{
    if (!bVisible) return;

    //if (pSource->GetObjectName() == _T("")) return;
    dwFormat = DT_LEFT | DT_EXPANDTABS;

    CString mstr = GetText();
    CString nstr;
/*  if (pSource2 && pSource)
    {
        CString nstr1 = pSource->GetObjectName();
        CString nstr2 = pSource2->GetObjectName();

        nstr.Format(_T("d(%s,%s) = "),nstr1,nstr2);
    }
    else if (pSource)
    {
        CString nstr1 = pSource->GetObjectName();

        nstr.Format(_T("l(%s) = "),nstr1);
    }*/
    BOOL mbSel = FALSE;
    for (int i=0;i<m_pSource.GetSize();i++)
    {
        CString nstr1 = m_pSource.GetAt(i)->GetObjectName();
        if (i) nstr += _T(",");
        nstr += nstr1;
        mbSel |= m_pSource.GetAt(i)->IsSelected();
    }
    CString nstr2 = nstr;
    nstr += _T(" = ");

    CString strVar;
    strVar.Empty();
    if (!strObjectName.IsEmpty())
    {
        strVar = strObjectName + _T(" = ");
        nstr = strVar + nstr;
    }

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rEqu,rName,rName2,rName3;
    rEqu.SetRectEmpty();
    rName.SetRectEmpty();
    rName2.SetRectEmpty();
    rName3.SetRectEmpty();
    pDC->DrawText(mstr,rEqu,DT_CALCRECT);
    pDC->DrawText(nstr,rName,DT_CALCRECT);
    pDC->DrawText(nstr2,rName2,DT_CALCRECT);
    pDC->DrawText(strVar,rName3,DT_CALCRECT);

    int nY = (rEqu.Height() - rName.Height())/2;
    int nX = rName.Width();

    rEqu.OffsetRect(nX,0);
    rName.OffsetRect(0,nY);
    rName2.OffsetRect(0,nY);
    rName3.OffsetRect(0,nY);

    rEqu.OffsetRect(rActZone.TopLeft());
    rName.OffsetRect(rActZone.TopLeft());
    rName2.OffsetRect(rActZone.TopLeft());
    rName3.OffsetRect(rActZone.TopLeft());

    rActZone = rEqu | rName;

    COLORREF oldC = pDC->SetTextColor(COLOR_VAR);
    pDC->DrawText(nstr,rName,dwFormat);
    pDC->SetTextColor(oldC);
    pDC->DrawText(mstr,rEqu,dwFormat);

    CPen mPen(PS_SOLID,1,COLOR_VAR);
    CPen *oldP = pDC->SelectObject(&mPen);

    pDC->MoveTo(rName2.left+rName3.Width(),rName2.top-2);
    pDC->LineTo(rName2.left + rName2.Width()/2+rName3.Width(),rName2.top-7);
    pDC->LineTo(rName2.right+rName3.Width(),rName2.top-2);

    pDC->SelectObject(oldP);

    if (mbSel || IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }

    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}



void CAngle3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSm)
{
    if (!((bMarked && vp->bDrawMark) || IsSelected())) return;
    int nb = m_pSource.GetSize();
    if (nb==3)
    {
        CObject3D *pObj1 = m_pSource.GetAt(0);
        CObject3D *pObj2 = m_pSource.GetAt(1);
        CObject3D *pObj3 = m_pSource.GetAt(2);

        if (!pObj1->bValidate || !pObj2->bValidate || !pObj3->bValidate)
            return;
        //if (!IsSelected()) return;

        CVector4 N1 = ((CPoint3D*)pObj2)->Concept_pt+(((CPoint3D*)pObj1)->Concept_pt-((CPoint3D*)pObj2)->Concept_pt) * .5;
        CVector4 N2 = ((CPoint3D*)pObj2)->Concept_pt+(((CPoint3D*)pObj3)->Concept_pt-((CPoint3D*)pObj2)->Concept_pt)* .5;
        CVector4 N2P = (N1+N2) * .5;

        CPoint3D pt0(((CPoint3D*)pObj2)->Concept_pt);
        CPoint3D pt1(N1);
        CPoint3D pt2(N2);
        CPoint3D pt3(N2P);
        pt0.CalculConceptuel();
        pt0.CalculVisuel(vp);
        pt1.CalculConceptuel();
        pt1.CalculVisuel(vp);
        pt2.CalculConceptuel();
        pt2.CalculVisuel(vp);
        pt3.CalculConceptuel();
        pt3.CalculVisuel(vp);

        CSphere3D *pSp = NULL;
        if (N1.Norme()<N2.Norme())
            pSp = new CSphere3D(&pt0,&pt1);
        else
            pSp = new CSphere3D(&pt0,&pt2);

        pSp->CalculConceptuel();

        CDemiDroite3D   pSeg1(&pt0,&pt1);
        CDemiDroite3D pSeg2(&pt0,&pt2);
        CDemiDroite3D pSeg3(&pt0,&pt3);
        pSeg1.CalculConceptuel();
        pSeg2.CalculConceptuel();
        pSeg3.CalculConceptuel();
        pSeg1.CalculVisuel(vp);
        pSeg2.CalculVisuel(vp);
        pSeg3.CalculVisuel(vp);

        CInterSphDr3D pI1(pSp,&pSeg1);
        CInterSphDr3D pI2(pSp,&pSeg2);
        CInterSphDr3D pI3(pSp,&pSeg3);
        pI1.CalculConceptuel();
        pI2.CalculConceptuel();
        pI3.CalculConceptuel();

        pI1.CalculVisuel(vp);
        pI2.CalculVisuel(vp);
        pI3.CalculVisuel(vp);

        CPoint3D *ppt1=NULL;
        CPoint3D *ppt2=NULL;
        CPoint3D *ppt3=NULL;

        if (pI1.ptA && pI1.ptA->bValidate)
            ppt1 = pI1.ptA;
        else
            ppt1 = pI1.ptB;
        if (pI2.ptA && pI2.ptA->bValidate)
            ppt2 = pI2.ptA;
        else
            ppt2 = pI2.ptB;
        if (pI3.ptA && pI3.ptA->bValidate)
            ppt3 = pI3.ptA;
        else
            ppt3 = pI3.ptB;

        CArcCercle3D pCC(ppt1,ppt3,ppt2);
        pCC.CalculConceptuel();
        pCC.CalculVisuel(vp);

        pCC.pObjectShape.nShapeId = 5;
        pCC.pObjectShape.clrObject = COLOR_VAR;

        pCC.nCalque = pObj1->nCalque & pObj2->nCalque;
        pCC.Draw(pDC,vp,TRUE);
        pSeg1.pObjectShape.clrObject = COLOR_VAR;
        pSeg2.pObjectShape.clrObject = COLOR_VAR;
        pSeg1.pObjectShape.nShapeId = 0;
        pSeg2.pObjectShape.nShapeId = 0;
        pSeg1.Draw(pDC,vp,TRUE);
        pSeg2.Draw(pDC,vp,TRUE);


        delete pSp;
        return;
    }
    if (nb!=2) return;

    CObject3D *pObj1 = m_pSource.GetAt(0);
    CObject3D *pObj2 = m_pSource.GetAt(1);

    if (!pObj1->bValidate || !pObj2->bValidate)
        return;


    if (IsSelected())
    {
        CVector4 N1 = (((CPlan3D*)pObj1)->p3+((CPlan3D*)pObj1)->p1) * .5;
        CVector4 N2 = (((CPlan3D*)pObj2)->p3+((CPlan3D*)pObj2)->p1) * .5;


        CVector4 N1V = ((CPlan3D*)pObj1)->VecNorm.Normalized();
        CVector4 N2V = ((CPlan3D*)pObj2)->VecNorm.Normalized();

        FCoord ac = N1 * N2;

        if (ac<=0)
            ac=-1.;
        else
            ac=1;

        CVector4 N1b = N1 + (N1V * (ac*50.));
        CVector4 N2b = N2 + (N1V * (ac*50.));

        CVector4 N3 = (N1b + N2b) * .5;

        CPoint3D pt1(N1);
        CPoint3D pt2(N2);
        CPoint3D pt3(N3);
        pt1.CalculConceptuel();
        pt1.CalculVisuel(vp);
        pt2.CalculConceptuel();
        pt2.CalculVisuel(vp);
        pt3.CalculConceptuel();
        pt3.CalculVisuel(vp);

        CArcCercle3D pCC(&pt1,&pt3,&pt2);
        pCC.CalculConceptuel();
        pCC.CalculVisuel(vp);

        pCC.pObjectShape.nShapeId = 5;
        pCC.pObjectShape.clrObject = RGB(0,0,0);

        pCC.nCalque = pObj1->nCalque & pObj2->nCalque;
        pCC.Draw(pDC,vp,TRUE);
    }
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CVolume3D, CAngle3D, VERSIONABLE_SCHEMA | 1)

CVolume3D::CVolume3D(): CAngle3D()
{
}

CVolume3D::CVolume3D(CObject3D* P1) : CAngle3D()
{
    m_pSource.RemoveAll();
    m_pSource.Add(P1);
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = P1->nDepth+1;
}

CVolume3D::CVolume3D(const CObject3D &src): CAngle3D(src)
{
}

CString CVolume3D::GetText()
{
    mStrText.Empty();

    if (m_pSource.GetSize()!=1) return mStrText;

    CObject3D *pObj1 = m_pSource.GetAt(0);
    if (!pObj1) return mStrText;

    if (pObj1->MaskObject(TCube3DClass))
    {
        CVector4 p1 = ((CCube3D*)pObj1)->ptA->Concept_pt;
        CVector4 p2 = ((CCube3D*)pObj1)->ptB->Concept_pt;

        CVector4 pVec = p2-p1;
        m_fAngle = RoundNorm((FCoord)pVec);
        m_fAngle = m_fAngle * m_fAngle * m_fAngle;
    }
    else if (pObj1->MaskObject(TSphere3DClass))
    {
        FCoord rad = ((CSphere3D*)pObj1)->Rayon;
        m_fAngle = RoundNorm(rad);
        m_fAngle = (4 * M_PI / 3.0) * (m_fAngle*m_fAngle*m_fAngle);
    }
    else if (pObj1->MaskObject(TCylindre3DClass))
    {
        FCoord rad = ((CCylinder3D*)pObj1)->nRayon;
        CVector4 p1 = ((CCylinder3D*)pObj1)->Base1->Concept_pt;
        CVector4 p2 = ((CCylinder3D*)pObj1)->Base2->Concept_pt;
        CVector4 pVec = p2-p1;
        m_fAngle = RoundNorm(rad);
        m_fAngle = m_fAngle*m_fAngle*M_PI;
        m_fAngle = m_fAngle * RoundNorm((FCoord)pVec);
    }
    else return mStrText;

    BOOL bApp = TPref::MathPad.ShowFraction;
    int  nApp = TPref::MathPad.PrecFraction;

    m_nValue = m_fAngle;

    CString strMask = _T("%s");
    CString strN1 = FormatNumber(m_fAngle,pObj1->bValidate);
    mStrText.Format(strMask,strN1);


/*  if (!bApp)
    {
        mStrText.Format(_T("%.4f"),m_fAngle);
    }
    else
    {
        CFraction F1 = m_fAngle;
        mStrText.Format(_T("%s"),F1.ToString(nApp));
    }

    if (TPref::MathPad.UnitLinear)
    {
        CString mstr = mStrText;
        mStrText.Format(_T("%s %s�"),mstr,TPref::strUnits.GetAt(TPref::MathPad.UnitLinear));
    }*/

    return mStrText;
}

void CVolume3D::DrawMathPad(CDC* pDC)
{
    if (!bVisible || !bValidate) return;

    //if (pSource->GetObjectName() == _T("")) return;
    dwFormat = DT_LEFT | DT_EXPANDTABS;

    CString mstr = GetText();
    CString nstr;

    nstr.Format(_T("V(%s) = "),m_pSource.GetAt(0)->GetObjectName());

    CString strVar;
    strVar.Empty();
    if (!strObjectName.IsEmpty())
    {
        strVar = strObjectName + _T(" = ");
        nstr = strVar + nstr;
    }

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rEqu,rName,rName2;
    rEqu.SetRectEmpty();
    rName.SetRectEmpty();
    pDC->DrawText(mstr,rEqu,DT_CALCRECT);
    pDC->DrawText(nstr,rName,DT_CALCRECT);

    int nY = (rEqu.Height() - rName.Height())/2;
    int nX = rName.Width();

    rEqu.OffsetRect(nX,0);
    rName.OffsetRect(0,nY);
    rName2.OffsetRect(0,nY);

    rEqu.OffsetRect(rActZone.TopLeft());
    rName.OffsetRect(rActZone.TopLeft());

    rActZone = rEqu | rName;

    COLORREF oldC = pDC->SetTextColor(COLOR_VAR);
    pDC->DrawText(nstr,rName,dwFormat);
    pDC->SetTextColor(oldC);
    pDC->DrawText(mstr,rEqu,dwFormat);

    //if (m_pSource.GetAt(0)->IsSelected())
    if (IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }

    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}

void CVolume3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSm)
{
    if (IsSelected())
    {
        CObject3D *pObj = m_pSource.GetAt(0)->CopyObject();
        pObj->CalculConceptuel();
        pObj->CalculVisuel(vp);

        if (pObj->MaskObject(TCube3DClass))
        {
            for (int i=0;i<12;i++)
                ((CCube3D*)pObj)->arete[i]->SetStyle(5);
            pObj->SetStyle(2);
        }
        else
            pObj->pObjectShape.nShapeId = 5;
        pObj->SetColor(COLOR_VAR);

        pObj->Draw(pDC,vp,!pObj->MaskObject(TCube3DClass));

        delete pObj;
    }

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CArea3D, CAngle3D, VERSIONABLE_SCHEMA | 1)

CArea3D::CArea3D(): CAngle3D()
{
}

CArea3D::CArea3D(CObject3D* P1) : CAngle3D()
{
    m_pSource.RemoveAll();
    m_pSource.Add(P1);
    m_fAngle = 0.;
    dwFormat = DT_LEFT | DT_WORDBREAK | DT_TABSTOP;
    nDepth = P1->nDepth+1;
}

CArea3D::CArea3D(const CObject3D &src): CAngle3D(src)
{
}

CString CArea3D::GetText()
{
    mStrText.Empty();

    if (m_pSource.GetSize()!=1) return mStrText;

    CObject3D *pObj1 = m_pSource.GetAt(0);
    if (!pObj1) return mStrText;


    mStrText = _T("###");
    //if (!pObj1->bValidate) return mStrText;


    if (pObj1->MaskObject(TCercle3DClass))
    {
        m_fAngle = ((CCercle3D*)pObj1)->Radius;

        m_fAngle = RoundNorm((FCoord)m_fAngle);
        m_fAngle = M_PI * m_fAngle * m_fAngle;
    }
    else if (pObj1->MaskObject(TSphere3DClass))
    {
        FCoord rad = ((CSphere3D*)pObj1)->Rayon;
        m_fAngle = RoundNorm(rad);
        m_fAngle = (4 * M_PI) * (m_fAngle*m_fAngle);
    }
    else if (pObj1->MaskObject(TPolygon3DClass))
    {
        FCoord rad = ((CPolygon3D*)pObj1)->m_nArea;
        m_fAngle = RoundNorm(rad);
        m_fAngle = RoundNorm(m_fAngle);
    }
    else if (pObj1->MaskObject(TCylindre3DClass))
    {
        FCoord rad = ((CCylinder3D*)pObj1)->nRayon;
        CVector4 p1 = ((CCylinder3D*)pObj1)->Base1->Concept_pt;
        CVector4 p2 = ((CCylinder3D*)pObj1)->Base2->Concept_pt;
        CVector4 pVec = p2-p1;
        m_fAngle = RoundNorm(rad);
        m_fAngle = 2*m_fAngle*M_PI;
        m_fAngle = m_fAngle * RoundNorm((FCoord)pVec);
    }
    else return mStrText;

    m_nValue = m_fAngle;

    CString strMask = _T("%s");
    CString strN1 = FormatNumber(m_fAngle,pObj1->bValidate);
    mStrText.Format(strMask,strN1);


/*  BOOL bApp = TPref::MathPad.ShowFraction;
    int  nApp = TPref::MathPad.PrecFraction;

    m_nValue = m_fAngle;
    if (!bApp)
    {
        mStrText.Format(_T("%.4f"),m_fAngle);
    }
    else
    {
        CFraction F1 = m_fAngle;
        mStrText.Format(_T("%s"),F1.ToString(nApp));
    }

    if (TPref::MathPad.UnitLinear)
    {
        CString mstr = mStrText;
        mStrText.Format(_T("%s %s�"),mstr,TPref::strUnits.GetAt(TPref::MathPad.UnitLinear));
    }*/

    return mStrText;
}

void CArea3D::DrawMathPad(CDC* pDC)
{
    if (!bVisible) return;

    //if (pSource->GetObjectName() == _T("")) return;
    dwFormat = DT_LEFT | DT_EXPANDTABS;

    CString mstr = GetText();
    CString nstr;

    nstr.Format(_T("A(%s) = "),m_pSource.GetAt(0)->GetObjectName());

    CString strVar;
    strVar.Empty();
    if (!strObjectName.IsEmpty())
    {
        strVar = strObjectName + _T(" = ");
        nstr = strVar + nstr;
    }

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rEqu,rName,rName2;
    rEqu.SetRectEmpty();
    rName.SetRectEmpty();
    pDC->DrawText(mstr,rEqu,DT_CALCRECT);
    pDC->DrawText(nstr,rName,DT_CALCRECT);

    int nY = (rEqu.Height() - rName.Height())/2;
    int nX = rName.Width();

    rEqu.OffsetRect(nX,0);
    rName.OffsetRect(0,nY);
    rName2.OffsetRect(0,nY);


    rEqu.OffsetRect(rActZone.TopLeft());
    rName.OffsetRect(rActZone.TopLeft());

    rActZone = rEqu | rName;

    COLORREF oldC = pDC->SetTextColor(COLOR_VAR);
    pDC->DrawText(nstr,rName,dwFormat);
    pDC->SetTextColor(oldC);
    pDC->DrawText(mstr,rEqu,dwFormat);

    if (m_pSource.GetAt(0)->IsSelected() || IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }

    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}


void CArea3D::Draw(CDC *pDC,CVisualParam *vp,BOOL bSm)
{
    int nb = m_pSource.GetSize();
    if (nb !=1) return;

    CObject3D *pObj1 = m_pSource.GetAt(0);

    if (!pObj1->bValidate) return;

    if (IsSelected())
    {
        CObject3D *pObj = pObj1->CopyObject();
        pObj->CalculConceptuel();
        pObj->CalculVisuel(vp);

        if (pObj->MaskObject(TPolygon3DClass))
            pObj->pObjectShape.nShapeId = 7;
        else
            pObj->pObjectShape.nShapeId = 5;
        pObj->pObjectShape.clrObject = COLOR_VAR;

        //pObj->nCalque = pObj1->nCalque & pObj2->nCalque;
        BOOL bD = !pObj->MaskObject(TPolygon3DClass);
        pObj->Draw(pDC,vp,bD);

        delete pObj;
    }

}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CComment3D, CEquation3D, VERSIONABLE_SCHEMA | 1)

CComment3D::CComment3D(): CEquation3D()
{
}

CComment3D::CComment3D(CString mstr): CEquation3D()
{
    m_strComment  = mstr;
}

CComment3D::CComment3D(const CObject3D &src): CEquation3D(src)
{
    m_strComment = ((CComment3D&)src).m_strComment;
}

CComment3D::~CComment3D()
{
}

CString CComment3D::GetText()
{
    return m_strComment;
}

void CComment3D::Serialize( CArchive& ar )
{
    CEquation3D::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_strComment;
    }
    else
    {
        ar >> m_strComment;
    }
}

void CComment3D::CalculVisuel(CVisualParam *)
{
    bValidate = TRUE;
}

int  CComment3D::CalculConceptuel()
{
    return CEquation3D::CalculConceptuel();
}

int CComment3D::SetProperties(CxObject3DSet *pSet)
{
    //return CText3D::SetProperties(pSet);

    CString mstr = GetObjectName();

    //CHelpPrSheet pSheet("Object Properties");
    CHelpPrSheet pSheet(AFX_IDS_OBJPROP);
    CObjectTextPage pPage;
    CObjectInfoPage pPage2;
    CObjectDepPage pPage3;

    ////// Set Text Information Page
    pPage.m_bMod = isA();

    LOGFONT lFont;
    mTextFont.GetLogFont(&lFont);
    pPage.m_lFont = lFont;
    pPage.m_strText = m_strComment;


    ////// Set Object Information Page
    pPage2.m_strObjDef = GetObjectDef();
    pPage2.m_strObjHelp = GetObjectHelp();

    ////// Set Object Dependencies Page
    pPage3.m_pList = &cDependList;
    CxObject3DSet pMyList;
    CxObject3DSet pMyList2;
    GetDependList(&pMyList,FALSE);
    for (int jj=0;jj<pMyList.GetSize();jj++)
    {
        CObject3D *pObj = pMyList.GetAt(jj);
        if (!pObj) continue;
        if (pObj == this) continue;

        if (!pObj->pComposite)
            pMyList2.Add(pObj);
    }
    pPage3.m_pList = &pMyList2;

    pSheet.AddPage(&pPage);
    pSheet.AddPage(&pPage2);
    pSheet.AddPage(&pPage3);
    int nRet = pSheet.DoModal();
    if (nRet == IDOK)
    {
        m_strComment = pPage.m_strText;
        lFont = pPage.m_lFont;

        mTextFont.DeleteObject();
        SetFont(&lFont);
    }

    return (nRet == IDOK);
}

void CComment3D::DrawMathPad(CDC* pDC)
{
    dwFormat = DT_LEFT | DT_EXPANDTABS;
    CString mstr = GetText();

    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rName;
    rName.SetRectEmpty();
    pDC->DrawText(mstr,rName,DT_CALCRECT);

    int nY = rName.Height();
    int nX = rName.Width();

    //rName.OffsetRect(nX,nY);

    rName.OffsetRect(rActZone.TopLeft());

    pDC->DrawText(mstr,rName,dwFormat);

    rActZone = rName;

    if (IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }

    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);

}



void CComment3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSm/*=FALSE*/)
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CMathOp3D, CAngle3D, VERSIONABLE_SCHEMA | 1)

CMathOp3D::CMathOp3D(): CAngle3D()
{
    m_pMathParser = NULL;
    m_pMathVar = NULL;
    m_pMathValue=NULL;

    if (!m_pMathParser)
        m_pMathParser = new CMathParser();
    if (!m_pMathVar)
        m_pMathVar = new CStrMap();
    if (!m_pMathValue)
        m_pMathValue = new double[20];
}

CMathOp3D::~CMathOp3D()
{
    if (m_pMathParser) delete m_pMathParser;
    if (m_pMathVar) delete m_pMathVar;
    if (m_pMathValue) delete[] m_pMathValue;
}


CMathOp3D::CMathOp3D(CString mstr): CAngle3D()
{
    m_strComment  = mstr;
    m_pMathParser = NULL;
    m_pMathVar = NULL;
    m_pMathValue=NULL;

    if (!m_pMathParser)
        m_pMathParser = new CMathParser();
    if (!m_pMathVar)
        m_pMathVar = new CStrMap();
    if (!m_pMathValue)
        m_pMathValue = new double[20];
}


CMathOp3D::CMathOp3D(const CObject3D &src): CAngle3D(src)
{
    m_strComment = ((CMathOp3D&)src).m_strComment;
    m_pMathParser = NULL;
    m_pMathVar = NULL;
    m_pMathValue=NULL;

    if (!m_pMathParser)
        m_pMathParser = new CMathParser();
    if (!m_pMathVar)
        m_pMathVar = new CStrMap();
    if (!m_pMathValue)
        m_pMathValue = new double[20];
}

CString CMathOp3D::GetText()
{
    return m_strComment;//CAngle3D::GetText();
}

void CMathOp3D::Serialize( CArchive& ar )
{
    CValue3D::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_strComment;
    }
    else
    {
        ar >> m_strComment;

        OnUpdateVariables(CCalques3DDoc::GLOBALObjectSet);
    }
}

CxObject3DSet* CMathOp3D::GetParents()
{
    CxObject3DSet* list = new CxObject3DSet();
    for (int i=0;i<m_pSource.GetSize();i++)
    {
        list->Add(m_pSource.GetAt(i));
    }
    return list;
    //return NULL;
}

void CMathOp3D::OnUpdateVariables(CxObject3DSet *pSet)
{
    m_pSource.RemoveAll();
    SetInGraph(FALSE);
    for (int i=0;i<pSet->GetSize();i++)
    {
        CObject3D*pObj = pSet->GetAt(i);
        if (!pObj) continue;
        CValue3D *pVal = DYNAMIC_DOWNCAST(CValue3D,pObj);
        if (!pVal) continue;

        if (pObj->strObjectName.IsEmpty()) continue;
        CString str = pObj->strObjectName;

        int nb = m_strComment.Find(str);
        if (nb==-1) continue;

        str.MakeUpper();
        char tt[255];
        strcpy(tt,str.GetBuffer(str.GetLength()));
            //varmap.AddString(tt, varvalues );
        m_pMathVar->AddString(tt,m_pMathValue/*+i*/);
        m_pSource.Add(pVal);
    }
    SetInGraph();
}


int CMathOp3D::SetProperties(CxObject3DSet *pSet)
{
    //return CText3D::SetProperties(pSet);
    CString mstr = GetObjectName();

    //CHelpPrSheet pSheet("Object Properties");
    CHelpPrSheet pSheet(AFX_IDS_OBJPROP);
    CObjectTextPage pPage;
    CObjectInfoPage pPage2;
    CObjectDepPage pPage3;

    ////// Set Text Information Page
    pPage.m_bMod = isA();

    LOGFONT lFont;
    mTextFont.GetLogFont(&lFont);
    pPage.m_lFont = lFont;
    pPage.m_strEqu = m_strComment;

    if (!strObjectName.IsEmpty())
        pPage.m_strVarName = strObjectName;

    ////// Set the Variable Names
    if (pSet)
    {
        for (int i=0;i<pSet->GetSize();i++)
        {
            CObject3D*pObj = pSet->GetAt(i);
            if (!pObj) continue;
            if (!DYNAMIC_DOWNCAST(CValue3D,pObj)) continue;
            if (pObj==this) continue;

            pPage.m_pSource.Add(pObj);

        }
    }


    ////// Set Object Information Page
    pPage2.m_strObjDef = GetObjectDef();
    pPage2.m_strObjHelp = GetObjectHelp();

    ////// Set Object Dependencies Page
    pPage3.m_pList = &cDependList;
    CxObject3DSet pMyList;
    CxObject3DSet pMyList2;
    GetDependList(&pMyList,FALSE);
    for (int jj=0;jj<pMyList.GetSize();jj++)
    {
        CObject3D *pObj = pMyList.GetAt(jj);
        if (!pObj) continue;
        if (pObj == this) continue;

        if (!pObj->pComposite)
            pMyList2.Add(pObj);
    }
    pPage3.m_pList = &pMyList2;

    pSheet.AddPage(&pPage);
    pSheet.AddPage(&pPage2);
    pSheet.AddPage(&pPage3);
    int nRet = pSheet.DoModal();
    if (nRet == IDOK)
    {
        m_strComment = pPage.m_strEqu;
        lFont = pPage.m_lFont;
        mTextFont.DeleteObject();
        SetFont(&lFont);
        if (!pPage.m_strVarName.IsEmpty())
            strObjectName = pPage.m_strVarName;
        else
            strObjectName.Empty();

        OnUpdateVariables(pSet);

        //if (m_pMathValue) delete[] m_pMathValue;
        //m_pMathValue = NULL;
        //m_pMathValue = new double[10];

    }

    return (nRet == IDOK);

}

void CMathOp3D::DrawMathPad(CDC* pDC)
{
    dwFormat = DT_LEFT | DT_EXPANDTABS;

    CString mstr2 = GetText();
    CString mstr,nstr;
    mstr.Format(_T("%s = "),mstr2);

//  CMathParser mp;
//  CStrMap varmap;
//  double varvalues[] = { 5, 6.1,0.1 };
//  varmap.AddString( "X", varvalues );
//  varmap.AddString( "Y", varvalues+1 );

    for (int i=0;i<m_pSource.GetSize();i++)
    {
        CObject3D * pObj = m_pSource.GetAt(i);
        if (!pObj) continue;
        CValue3D *pVal = DYNAMIC_DOWNCAST(CValue3D,pObj);
        if (!pVal) continue;

        CString str = pObj->strObjectName;
        str.MakeUpper();
        char tt[255];
        strcpy(tt,str.GetBuffer(str.GetLength()));
        m_pMathValue[i]=pVal->m_nValue;

        m_pMathVar->Replace(tt,m_pMathValue+i);
    }

    //char formula[]=GetText();
    double result;

    m_pMathParser->Parameters=m_pMathVar;
    char *ErrMsg = m_pMathParser->Parse( GetText(), &result );

    if (ErrMsg)
        nstr = ErrMsg;
    else
    {
        nstr.Format(_T("%.4f"),result);
        m_nValue = result;
    }

    CString strVar;
    strVar.Empty();
    if (!strObjectName.IsEmpty())
    {
        strVar = strObjectName + _T(" = ");
        mstr = strVar + mstr;
    }


    int oldBM = pDC->SetBkMode(TRANSPARENT);
    CFont *oldF = pDC->SelectObject(&mTextFont);

    CRect rEqu,rName,rName2;
    rEqu.SetRectEmpty();
    rName.SetRectEmpty();
    pDC->DrawText(nstr,rEqu,DT_CALCRECT);
    pDC->DrawText(mstr,rName,DT_CALCRECT);

    int nY = (rEqu.Height() - rName.Height())/2;
    int nX = rName.Width();

    rEqu.OffsetRect(nX,0);
    rName.OffsetRect(0,nY);

    rEqu.OffsetRect(rActZone.TopLeft());
    rName.OffsetRect(rActZone.TopLeft());

    rActZone = rEqu | rName;


    COLORREF oldC = pDC->SetTextColor(COLOR_VAR);
    pDC->DrawText(mstr,rName,dwFormat);
    pDC->SetTextColor(oldC);
    pDC->DrawText(nstr,rEqu,dwFormat);

    if (IsSelected())
    {
        CRect mrect = rActZone;
        mrect.InflateRect(1,1,1,1);
        pDC->DrawFocusRect(mrect);
    }

    pDC->SelectObject(oldF);
    pDC->SetBkMode(oldBM);
}


void CMathOp3D::Draw(CDC* pDC,CVisualParam *vp,BOOL bSm/*=FALSE*/)
{
}
